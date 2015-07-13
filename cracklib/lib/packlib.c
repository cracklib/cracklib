/*
 * This program is copyright Alec Muffett 1993, portions copyright other authors.
 * The authors disclaim all responsibility or liability with respect to it's usage
 * or its effect upon hardware or computer systems.
 */

#include "config.h"
#include <string.h>
#include <stdlib.h>
#ifdef HAVE_ZLIB_H
#include <zlib.h>
#endif
#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif
#include "packer.h"

static const char vers_id[] = "packlib.c : v2.3p2 Alec Muffett 18 May 1993";

#define DEBUG 0

/* Structures for processing "broken" 64bit dictionary files */

struct pi_header64
{
    uint64_t pih_magic;
    uint64_t pih_numwords;
    uint16_t pih_blocklen;
    uint16_t pih_pad;
};

typedef struct
{
    void *ifp;
    void *dfp;
    void *wfp;
    uint64_t flags;
    uint64_t hwms[256];
    struct pi_header64 header;
    int count;
    char data_put[NUMWORDS][MAXWORDLEN];
    char data_get[NUMWORDS][MAXWORDLEN];
} PWDICT64;


static int
_PWIsBroken64(FILE *ifp)
{
    PWDICT64 pdesc64;

    rewind(ifp);
    if (!fread((char *) &pdesc64.header, sizeof(pdesc64.header), 1, ifp))
    {
       return 0;
    }

    return (pdesc64.header.pih_magic == PIH_MAGIC);
}


PWDICT *
PWOpen(prefix, mode)
    const char *prefix;
    char *mode;
{
    int use64 = 0;
    static PWDICT pdesc;
    static PWDICT64 pdesc64;
    char iname[STRINGSIZE];
    char dname[STRINGSIZE];
    char wname[STRINGSIZE];
    void *dfp;
    void *ifp;
    void *wfp;

    if (pdesc.header.pih_magic == PIH_MAGIC)
    {
	fprintf(stderr, "%s: another dictionary already open\n", prefix);
	return NULL;
    }

    memset(&pdesc, '\0', sizeof(pdesc));
    memset(&pdesc64, '\0', sizeof(pdesc64));

    snprintf(iname, STRINGSIZE, "%s.pwi", prefix);
    snprintf(dname, STRINGSIZE, "%s.pwd", prefix);
    snprintf(wname, STRINGSIZE, "%s.hwm", prefix);

    if (mode[0] == 'r')
    {
		pdesc.flags &= ~PFOR_USEZLIB;
		/* first try the normal db file */
		if (!(pdesc.dfp = fopen(dname, mode)))
		{
#ifdef HAVE_ZLIB_H
			pdesc.flags |= PFOR_USEZLIB;
			/* try extension .gz */
			snprintf(dname, STRINGSIZE, "%s.pwd.gz", prefix);
			if (!(pdesc.dfp = gzopen(dname, mode)))
			{
				perror(dname);
				return NULL;
			}
#else
		perror(dname);
		return NULL;
#endif
		}
	}
	else
	{
		pdesc.flags &= ~PFOR_USEZLIB;
		/* write mode: use fopen */
		if (!(pdesc.dfp = fopen(dname, mode)))
		{
			perror(dname);
			return NULL;
		}
	}

    if (!(pdesc.ifp = fopen(iname, mode)))
    {
#ifdef HAVE_ZLIB_H
		if (pdesc.flags & PFOR_USEZLIB)
			gzclose(pdesc.dfp);
		else
#endif
			fclose(pdesc.dfp);
	perror(iname);
	return NULL;
    }

    if ((pdesc.wfp = fopen(wname, mode)))
    {
	pdesc.flags |= PFOR_USEHWMS;
    }

    ifp = pdesc.ifp;
    dfp = pdesc.dfp;
    wfp = pdesc.wfp;

    if (mode[0] == 'w')
    {
	pdesc.flags |= PFOR_WRITE;
	pdesc.header.pih_magic = PIH_MAGIC;
	pdesc.header.pih_blocklen = NUMWORDS;
	pdesc.header.pih_numwords = 0;

	fwrite((char *) &pdesc.header, sizeof(pdesc.header), 1, ifp);
    } else
    {
	pdesc.flags &= ~PFOR_WRITE;

	if (!fread((char *) &pdesc.header, sizeof(pdesc.header), 1, ifp))
	{
	    fprintf(stderr, "%s: error reading header\n", prefix);

	    pdesc.header.pih_magic = 0;
	    fclose(ifp);
#ifdef HAVE_ZLIB_H
		if (pdesc.flags & PFOR_USEZLIB)
			gzclose(dfp);
		else
#endif
			fclose(dfp);
	    if (wfp)
	    {
		fclose(wfp);
	    }
	    return NULL;
	}

        if ((pdesc.header.pih_magic == 0) || (pdesc.header.pih_numwords == 0))
        {
            /* uh-oh. either a broken "64-bit" file or a garbage file. */
            rewind (ifp);
            if (!fread((char *) &pdesc64.header, sizeof(pdesc64.header), 1, ifp))
            {
                fprintf(stderr, "%s: error reading header\n", prefix);

                pdesc.header.pih_magic = 0;
                fclose(ifp);
#ifdef HAVE_ZLIB_H
				if (pdesc.flags & PFOR_USEZLIB)
					gzclose(dfp);
				else
#endif
					fclose(dfp);
		if (wfp)
		{
			fclose(wfp);
		}
                return NULL;
            }
            if (pdesc64.header.pih_magic != PIH_MAGIC)
            {
                /* nope, not "64-bit" after all */
                fprintf(stderr, "%s: error reading header\n", prefix);

                pdesc.header.pih_magic = 0;
                fclose(ifp);
#ifdef HAVE_ZLIB_H
				if (pdesc.flags & PFOR_USEZLIB)
					gzclose(dfp);
				else
#endif
					fclose(dfp);

		if (wfp)
		{
			fclose(wfp);
		}
                return NULL;
            }
            pdesc.header.pih_magic = pdesc64.header.pih_magic;
            pdesc.header.pih_numwords = pdesc64.header.pih_numwords;
            pdesc.header.pih_blocklen = pdesc64.header.pih_blocklen;
            pdesc.header.pih_pad = pdesc64.header.pih_pad;
            use64 = 1;
        }

	if (pdesc.header.pih_magic != PIH_MAGIC)
	{
	    fprintf(stderr, "%s: magic mismatch\n", prefix);

	    pdesc.header.pih_magic = 0;
	    fclose(ifp);
#ifdef HAVE_ZLIB_H
		if (pdesc.flags & PFOR_USEZLIB)
			gzclose(dfp);
		else
#endif
			fclose(dfp);

	    if (wfp)
	    {
		fclose(wfp);
	    }
	    return NULL;
	}

        if (pdesc.header.pih_numwords < 1)
        {
            fprintf(stderr, "%s: invalid word count\n", prefix);

            pdesc.header.pih_magic = 0;
            fclose(ifp);
#ifdef HAVE_ZLIB_H
			if (pdesc.flags & PFOR_USEZLIB)
				gzclose(dfp);
			else
#endif
				fclose(dfp);
	    if (wfp)
	    {
		fclose(wfp);
	    }
            return NULL;
        }

	if (pdesc.header.pih_blocklen != NUMWORDS)
	{
	    fprintf(stderr, "%s: size mismatch\n", prefix);

	    pdesc.header.pih_magic = 0;
	    fclose(ifp);
#ifdef HAVE_ZLIB_H
		if (pdesc.flags & PFOR_USEZLIB)
			gzclose(dfp);
		else
#endif
			fclose(dfp);
		if (wfp)
	    {
		fclose(wfp);
	    }
	    return NULL;
	}

	if (pdesc.flags & PFOR_USEHWMS)
	{
            int i;

            if (use64)
            {
                if (fread(pdesc64.hwms, 1, sizeof(pdesc64.hwms), wfp) != sizeof(pdesc64.hwms))
                {
                    pdesc.flags &= ~PFOR_USEHWMS;
                }
                for (i = 0; i < sizeof(pdesc.hwms) / sizeof(pdesc.hwms[0]); i++)
                {
                    pdesc.hwms[i] = pdesc64.hwms[i];
                }
            }
            else if (fread(pdesc.hwms, 1, sizeof(pdesc.hwms), wfp) != sizeof(pdesc.hwms))
	    {
		pdesc.flags &= ~PFOR_USEHWMS;
	    }
#if DEBUG
            for (i=1; i<=0xff; i++)
            {
                printf("hwm[%02x] = %d\n", i, pdesc.hwms[i]);
            }
#endif
	}
    }

    return (&pdesc);
}

int
PWClose(pwp)
    PWDICT *pwp;
{
    if (pwp->header.pih_magic != PIH_MAGIC)
    {
	fprintf(stderr, "PWClose: close magic mismatch\n");
	return (-1);
    }

    if (pwp->flags & PFOR_WRITE)
    {
	pwp->flags |= PFOR_FLUSH;
	PutPW(pwp, NULL);	/* flush last index if necess */

	if (fseek(pwp->ifp, 0L, 0))
	{
	    fprintf(stderr, "index magic fseek failed\n");
	    return (-1);
	}

	if (!fwrite((char *) &pwp->header, sizeof(pwp->header), 1, pwp->ifp))
	{
	    fprintf(stderr, "index magic fwrite failed\n");
	    return (-1);
	}

	if (pwp->flags & PFOR_USEHWMS)
	{
	    int i;
	    for (i=1; i<=0xff; i++)
	    {
	    	if (!pwp->hwms[i])
	    	{
	    	    pwp->hwms[i] = pwp->hwms[i-1];
	    	}
#if DEBUG
	    	printf("hwm[%02x] = %d\n", i, pwp->hwms[i]);
#endif
	    }
	    fwrite(pwp->hwms, 1, sizeof(pwp->hwms), pwp->wfp);
	}
    }

    fclose(pwp->ifp);
#ifdef HAVE_ZLIB_H
	if (pwp->flags & PFOR_USEZLIB)
		gzclose(pwp->dfp);
	else
#endif
		fclose(pwp->dfp);
    if (pwp->wfp)
    {
        fclose(pwp->wfp);
    }

    pwp->header.pih_magic = 0;

    return (0);
}

int
PutPW(pwp, string)
    PWDICT *pwp;
    char *string;
{
    if (!(pwp->flags & PFOR_WRITE))
    {
	return (-1);
    }

    if (string)
    {
	strncpy(pwp->data_put[pwp->count], string, MAXWORDLEN);
	pwp->data_put[pwp->count][MAXWORDLEN - 1] = '\0';

	pwp->hwms[string[0] & 0xff]= pwp->header.pih_numwords;

	++(pwp->count);
	++(pwp->header.pih_numwords);

    } else if (!(pwp->flags & PFOR_FLUSH))
    {
	return (-1);
    }

    if ((pwp->flags & PFOR_FLUSH) || !(pwp->count % NUMWORDS))
    {
	int i;
	uint32_t datum;
	register char *ostr;

	datum = (uint32_t) ftell(pwp->dfp);

	fwrite((char *) &datum, sizeof(datum), 1, pwp->ifp);

	fputs(pwp->data_put[0], pwp->dfp);
	putc(0, pwp->dfp);

	ostr = pwp->data_put[0];

	for (i = 1; i < NUMWORDS; i++)
	{
	    register int j;
	    register char *nstr;
	    nstr = pwp->data_put[i];

	    if (nstr[0])
	    {
		for (j = 0; ostr[j] && nstr[j] && (ostr[j] == nstr[j]); j++);
		putc(j & 0xff, pwp->dfp);
		fputs(nstr + j, pwp->dfp);
	    }
	    putc(0, pwp->dfp);

	    ostr = nstr;
	}

	memset(pwp->data_put, '\0', sizeof(pwp->data_put));
	pwp->count = 0;
    }
    return (0);
}

char *
GetPW(pwp, number)
    PWDICT *pwp;
    uint32_t number;
{
    uint32_t datum;
    register int i;
    register char *ostr;
    register char *nstr;
    register char *bptr;
    char buffer[NUMWORDS * MAXWORDLEN];
    uint32_t thisblock;

    thisblock = number / NUMWORDS;

    if (_PWIsBroken64(pwp->ifp))
    {
       uint64_t datum64;
       if (fseek(pwp->ifp, sizeof(struct pi_header64) + (thisblock * sizeof(uint64_t)), 0))
       {
           perror("(index fseek failed)");
           return NULL;
       }

       if (!fread((char *) &datum64, sizeof(datum64), 1, pwp->ifp))
       {
           perror("(index fread failed)");
           return NULL;
       }
       datum = datum64;
    } else {
       if (fseek(pwp->ifp, sizeof(struct pi_header) + (thisblock * sizeof(uint32_t)), 0))
       {
           perror("(index fseek failed)");
           return NULL;
       }

       if (!fread((char *) &datum, sizeof(datum), 1, pwp->ifp))
       {
           perror("(index fread failed)");
           return NULL;
       }
    }

	int r = 1;
#ifdef HAVE_ZLIB_H
	if (pwp->flags & PFOR_USEZLIB)
	{
		r = gzseek(pwp->dfp, datum, 0);
		if (r >= 0)
			r = 0;
	}
	else
#endif
		r = fseek(pwp->dfp, datum, 0);


    if (r)
    {
	perror("(data fseek failed)");
	return NULL;
    }
	r = 0;

        memset(buffer, 0, sizeof(buffer));
#ifdef HAVE_ZLIB_H
	if (pwp->flags & PFOR_USEZLIB)
	{
		r = gzread(pwp->dfp, buffer, sizeof(buffer));
		if (r < 0)
			r = 0;
	}
	else
#endif
		r = fread(buffer, 1, sizeof(buffer), pwp->dfp);

    if (!r)
    {
	perror("(data fread failed)");
	return NULL;
    }

    bptr = buffer;

    for (ostr = pwp->data_get[0]; (*(ostr++) = *(bptr++)); /* nothing */ );

    ostr = pwp->data_get[0];

    for (i = 1; i < NUMWORDS; i++)
    {
	nstr = pwp->data_get[i];
	strcpy(nstr, ostr);

	ostr = nstr + *(bptr++);
	while ((*(ostr++) = *(bptr++)));

	ostr = nstr;
    }

    return (pwp->data_get[number % NUMWORDS]);
}

unsigned int
FindPW(pwp, string)
    PWDICT *pwp;
    char *string;
{
    register uint32_t lwm;
    register uint32_t hwm;
    register uint32_t middle;
    register char *this;
    int idx;

#if DEBUG
fprintf(stderr, "look for (%s)\n", string);
#endif

    if (pwp->flags & PFOR_USEHWMS)
    {
	idx = string[0] & 0xff;
    	lwm = idx ? pwp->hwms[idx - 1] : 0;
    	hwm = pwp->hwms[idx];

#if DEBUG
	fprintf(stderr, "idx = %d\n", idx);
	fprintf(stderr, "lwm = %d,  hwm = %d\n", lwm, hwm);
#endif
    } else
    {
    	lwm = 0;
    	hwm = PW_WORDS(pwp) - 1;
    }

    /* if the high water mark is lower than the low water mark, something is screwed up */
    if ( hwm < lwm )
    {
	lwm = 0;
	hwm = PW_WORDS(pwp) - 1;
    }

#if DEBUG
    fprintf(stderr, "---- %lu, %lu ----\n", lwm, hwm);
#endif

    middle = lwm + ((hwm - lwm + 1) / 2);

    for (;;)
    {
	int cmp;


#if DEBUG
	fprintf(stderr, "lwm = %lu,  middle = %lu,  hwm = %lu\n", lwm, middle, hwm);
#endif

	this = GetPW(pwp, middle);
	if ( ! this )
	{
#if DEBUG
		fprintf(stderr, "getpw returned null, returning null in FindPW\n");
#endif
		return(PW_WORDS(pwp));
	}
	else
	{
#if DEBUG
		fprintf(stderr, "comparing %s against found %s\n", string, this);
#endif
	}

	cmp = strcmp(string, this);
	if (cmp == 0)
	{
	    return(middle);
        }

        if (middle == hwm)
        {
#if DEBUG
		fprintf(stderr, "at terminal subdivision, stopping search\n");
#endif
		break;
        }

	if (cmp < 0)
	{
	    hwm = middle;
	    middle = lwm + ((hwm - lwm ) / 2);
	}
	else if (cmp > 0)
	{
	    lwm = middle;
	    middle = lwm + ((hwm - lwm + 1) / 2);
	}
    }

    return (PW_WORDS(pwp));
}
