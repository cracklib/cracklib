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
#include "portable_endian.h"
#include "packer.h"

#define DEBUG 0

#define STORE_ORDER_BE 1
#define STORE_ORDER_LE 2

#ifndef STORE_ORDER
#define STORE_ORDER STORE_ORDER_LE
#endif

#if STORE_ORDER == STORE_ORDER_LE
#if DEBUG
#warning "Choosing LITTLE ENDIAN"
#endif
#define STORE_32(x) htole32(x)
#define STORE_16(x) htole16(x)
#define FETCH_32(x, wrong) (wrong == 'n' ? le32toh(x) : be32toh(x))
#define FETCH_16(x, wrong) (wrong == 'n' ? le16toh(x) : be16toh(x))
#else
#if DEBUG
#warning "Choosing BIG ENDIAN"
#endif
#define STORE_32(x) htobe32(x)
#define STORE_16(x) htobe16(x)
#define FETCH_32(x, wrong) (wrong == 'n' ? be32toh(x) : le32toh(x))
#define FETCH_16(x, wrong) (wrong == 'n' ? be16toh(x) : le16toh(x))
#endif

static size_t
write_header(struct pi_header *header, void *ifp, char *wrong_endian)
{
    struct pi_header header_copy;
    *wrong_endian = 'n';
    header_copy.pih_magic = STORE_32(header->pih_magic);
    header_copy.pih_numwords = STORE_32(header->pih_numwords);
    header_copy.pih_blocklen = STORE_16(header->pih_blocklen);
    header_copy.pih_pad = STORE_16(header->pih_pad);
    return fwrite((char *) &header_copy, sizeof(header_copy), 1, ifp);
}

static size_t
read_header(struct pi_header *header, void *ifp, char *wrong_endian)
{
    size_t retval;
    struct pi_header header_copy;
    *wrong_endian = '?';
    retval = fread((char *) &header_copy, sizeof(header_copy), 1, ifp);
    if (retval) {
        if (FETCH_32(header_copy.pih_magic, 'n') == PIH_MAGIC) {
            *wrong_endian = 'n';
        } else if (FETCH_32(header_copy.pih_magic, 'y') == PIH_MAGIC) {
            *wrong_endian = 'y';
        }
        header->pih_magic = FETCH_32(header_copy.pih_magic, *wrong_endian);
        header->pih_numwords = FETCH_32(header_copy.pih_numwords, *wrong_endian);
        header->pih_blocklen = FETCH_16(header_copy.pih_blocklen, *wrong_endian);
        header->pih_pad = FETCH_16(header_copy.pih_pad, *wrong_endian);
    }
    return retval;
}

static size_t
write_hwms(uint32_t hwms[256], void* wfp)
{
    uint32_t hwms_copy[256];
    int i;
    for (i = 0; i < 256; ++i) {
        hwms_copy[i] = STORE_32(hwms[i]);
    }
    return fwrite(&hwms_copy, sizeof(hwms_copy), 1, wfp);
}

static size_t
read_hwms(uint32_t hwms[256], void* wfp, char wrong_endian)
{
    size_t retval;
    uint32_t hwms_copy[256];
    int i;
    retval = fread(&hwms_copy, sizeof(hwms_copy), 1, wfp);
    if (retval) {
        for (i = 0; i < 256; ++i) {
            hwms[i] = FETCH_32(hwms_copy[i], wrong_endian);
        }
    }
    return retval;
}


static size_t
write_index(uint32_t index, void* ifp)
{
    index = STORE_32(index);
    return fwrite(&index, sizeof(index), 1, ifp);
}

static size_t
read_index(uint32_t* index, void* ifp, char wrong_endian)
{
    size_t retval;
    retval = fread(index, sizeof(*index), 1, ifp);
    if (retval) {
        *index = FETCH_32(*index, wrong_endian);
    }
    return retval;
}

PWDICT *
PWOpen(const char *prefix, char *mode)
{
    PWDICT *pdesc;
    char iname[STRINGSIZE];
    char dname[STRINGSIZE];
    char wname[STRINGSIZE];

    pdesc = malloc(sizeof(*pdesc));
    if (pdesc == NULL)
        return NULL;

    memset(pdesc, '\0', sizeof(*pdesc));

    snprintf(iname, STRINGSIZE, "%s.pwi", prefix);
    snprintf(dname, STRINGSIZE, "%s.pwd", prefix);
    snprintf(wname, STRINGSIZE, "%s.hwm", prefix);

    if (mode[0] == 'r')
    {
        pdesc->flags &= ~PFOR_USEZLIB;
        /* first try the normal db file */
        if (!(pdesc->dfp = fopen(dname, mode)))
        {
#ifdef HAVE_ZLIB_H
            pdesc->flags |= PFOR_USEZLIB;
            /* try extension .gz */
            snprintf(dname, STRINGSIZE, "%s.pwd.gz", prefix);
            if (!(pdesc->dfp = gzopen(dname, mode)))
            {
                    perror(dname);
                    free(pdesc);
                    return NULL;
            }
#else
            perror(dname);
            free(pdesc);
            return NULL;
#endif
        }
    }
    else
    {
        pdesc->flags &= ~PFOR_USEZLIB;
        /* write mode: use fopen */
        if (!(pdesc->dfp = fopen(dname, mode)))
        {
            perror(dname);
            free(pdesc);
            return NULL;
        }
    }

    if (!(pdesc->ifp = fopen(iname, mode)))
    {
#ifdef HAVE_ZLIB_H
        if (pdesc->flags & PFOR_USEZLIB)
            gzclose(pdesc->dfp);
        else
#endif
            fclose(pdesc->dfp);
        perror(iname);
        free(pdesc);
        return NULL;
    }

    if ((pdesc->wfp = fopen(wname, mode)))
    {
        pdesc->flags |= PFOR_USEHWMS;
    }

    if (mode[0] == 'w')
    {
        pdesc->flags |= PFOR_WRITE;
        pdesc->header.pih_magic = PIH_MAGIC;
        pdesc->header.pih_blocklen = NUMWORDS;
        pdesc->header.pih_numwords = 0;
        write_header(&pdesc->header, pdesc->ifp, &pdesc->wrong_endian);
    } else
    {
        pdesc->flags &= ~PFOR_WRITE;

        if (!read_header(&pdesc->header, pdesc->ifp, &pdesc->wrong_endian))
        {
            fprintf(stderr, "%s: error reading header\n", prefix);

            fclose(pdesc->ifp);
#ifdef HAVE_ZLIB_H
            if (pdesc->flags & PFOR_USEZLIB)
                gzclose(pdesc->dfp);
            else
#endif
                fclose(pdesc->dfp);
            if (pdesc->wfp)
            {
                fclose(pdesc->wfp);
            }
            free(pdesc);
            return NULL;
        }

        if (pdesc->header.pih_magic != PIH_MAGIC)
        {
            fprintf(stderr, "%s: magic mismatch\n", prefix);

            fclose(pdesc->ifp);
#ifdef HAVE_ZLIB_H
            if (pdesc->flags & PFOR_USEZLIB)
                gzclose(pdesc->dfp);
            else
#endif
                fclose(pdesc->dfp);

            if (pdesc->wfp)
            {
                fclose(pdesc->wfp);
            }
            free(pdesc);
            return NULL;
        }

        if (pdesc->header.pih_numwords < 1)
        {
            fprintf(stderr, "%s: invalid word count\n", prefix);

            fclose(pdesc->ifp);
#ifdef HAVE_ZLIB_H
            if (pdesc->flags & PFOR_USEZLIB)
                gzclose(pdesc->dfp);
            else
#endif
                fclose(pdesc->dfp);
            if (pdesc->wfp)
            {
                fclose(pdesc->wfp);
            }
            free(pdesc);
            return NULL;
        }

        if (pdesc->header.pih_blocklen != NUMWORDS)
        {
            fprintf(stderr, "%s: size mismatch\n", prefix);

            fclose(pdesc->ifp);
#ifdef HAVE_ZLIB_H
            if (pdesc->flags & PFOR_USEZLIB)
                gzclose(pdesc->dfp);
            else
#endif
                fclose(pdesc->dfp);
            if (pdesc->wfp)
            {
                fclose(pdesc->wfp);
            }
            free(pdesc);
            return NULL;
        }

        if (pdesc->flags & PFOR_USEHWMS)
        {
            if (read_hwms(pdesc->hwms, pdesc->wfp, pdesc->wrong_endian) != sizeof(pdesc->hwms))
            {
                pdesc->flags &= ~PFOR_USEHWMS;
            }
#if DEBUG
            int i;
            for (i=1; i<=0xff; i++)
            {
                fprintf(stderr, "hwm[%02x] = %d\n", i, pdesc->hwms[i]);
            }
#endif
        }
    }

    return (pdesc);
}

int
PWClose(PWDICT *pwp)
{
    if (pwp->header.pih_magic != PIH_MAGIC)
    {
        fprintf(stderr, "PWClose: close magic mismatch\n");
        return (-1);
    }

    if (pwp->flags & PFOR_WRITE)
    {
        pwp->flags |= PFOR_FLUSH;
        PutPW(pwp, NULL);        /* flush last index if necess */

        if (fseek(pwp->ifp, 0L, 0))
        {
            fprintf(stderr, "index magic fseek failed\n");
            free(pwp);
            return (-1);
        }

        if (!write_header(&pwp->header, pwp->ifp, &pwp->wrong_endian))
        {
            fprintf(stderr, "index magic fwrite failed\n");
            free(pwp);
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
                fprintf(stderr, "hwm[%02x] = %d\n", i, pwp->hwms[i]);
#endif
            }
            write_hwms(pwp->hwms, pwp->wfp);
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
    free(pwp);

    return (0);
}

int
PutPW(PWDICT *pwp, char *string)
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
        register char *ostr;

        write_index((uint32_t)ftell(pwp->dfp), pwp->ifp);

        fputs(pwp->data_put[0], pwp->dfp);
        putc(0, (FILE*) pwp->dfp);

        ostr = pwp->data_put[0];

        for (i = 1; i < NUMWORDS; i++)
        {
            register int j;
            register char *nstr;
            nstr = pwp->data_put[i];

            if (nstr[0])
            {
                for (j = 0; ostr[j] && nstr[j] && (ostr[j] == nstr[j]); j++);
                putc(j & 0xff, (FILE*) pwp->dfp);
                fputs(nstr + j, pwp->dfp);
            }
            putc(0, (FILE*) pwp->dfp);

            ostr = nstr;
        }

        memset(pwp->data_put, '\0', sizeof(pwp->data_put));
        pwp->count = 0;
    }
    return (0);
}

char *
GetPW(PWDICT *pwp, uint32_t number)
{
    uint32_t datum;
    register int i;
    register char *ostr;
    register char *nstr;
    register char *bptr;
    char buffer[NUMWORDS * MAXWORDLEN];
    uint32_t thisblock;

    thisblock = number / NUMWORDS;

#if DEBUG
    fprintf(stderr, "seeking index at: 0x%lx\n", thisblock *sizeof(uint32_t));
#endif
    if (fseek(pwp->ifp, sizeof(struct pi_header) + (thisblock * sizeof(uint32_t)), 0))
    {
        perror("(index fseek failed)");
        return NULL;
    }

    if (!read_index(&datum, pwp->ifp, pwp->wrong_endian))
    {
        perror("(index fread failed)");
        return NULL;
    }
#if DEBUG
    fprintf(stderr, "read index: 0x%x, wrong_endian = %c\n", datum, pwp->wrong_endian);
#endif

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
FindPW(PWDICT *pwp, char *string)
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
    fprintf(stderr, "---- %u, %u ----\n", lwm, hwm);
#endif

    for (;;)
    {
        int cmp;

        middle = lwm + ((hwm - lwm + 1) / 2);

#if DEBUG
        fprintf(stderr, "lwm = %u,  middle = %u,  hwm = %u\n", lwm, middle, hwm);
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

        if (cmp < 0)
        {
            if (middle == lwm)
            {
#if DEBUG
                fprintf(stderr, "at terminal subdivision from right, stopping search\n");
#endif
                break;
            }
            hwm = middle - 1;
        }
        else if (cmp > 0)
        {
            if (middle == hwm)
            {
#if DEBUG
                fprintf(stderr, "at terminal subdivision from left, stopping search\n");
#endif
                break;
            }
            lwm = middle + 1;
        }
    }

    return (PW_WORDS(pwp));
}
