/*
 * This program is copyright Alec Muffett 1993. The author disclaims all 
 * responsibility or liability with respect to it's usage or its effect 
 * upon hardware or computer systems, and maintains copyright as set out 
 * in the "LICENCE" document which accompanies distributions of Crack v4.0 
 * and upwards.
 */

/* This header expects the SUSv2 integer types to be available, if not,
   it will not compile. */


#include <stdio.h>
#include <ctype.h>

#define STRINGSIZE	1024
#define TRUNCSTRINGSIZE	(STRINGSIZE/4)

#ifndef NUMWORDS
#define NUMWORDS 	16
#endif
#define MAXWORDLEN	32
#define MAXBLOCKLEN 	(MAXWORDLEN * NUMWORDS)

struct pi_header
{
    uint32_t pih_magic;
    uint32_t pih_numwords;
    uint16_t pih_blocklen;
    uint16_t pih_pad;
};

typedef struct
{
    FILE *ifp;
    FILE *dfp;
    FILE *wfp;

    uint32_t flags;
#define PFOR_WRITE	0x0001
#define PFOR_FLUSH	0x0002
#define PFOR_USEHWMS	0x0004

    uint32_t hwms[256];

    struct pi_header header;

    int count;
    char data[NUMWORDS][MAXWORDLEN];
} PWDICT;

#define PW_WORDS(x) ((x)->header.pih_numwords)
#define PIH_MAGIC 0x70775631

extern PWDICT *PWOpen();
extern char *Mangle();
extern char *FascistCheck();

#define CRACK_TOLOWER(a) 	(isupper(a)?tolower(a):(a)) 
#define CRACK_TOUPPER(a) 	(islower(a)?toupper(a):(a)) 
#define STRCMP(a,b)		strcmp((a),(b))
