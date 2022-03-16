/*============================----beg-of-source---============================*/
#ifndef yMARK_priv
#define yMARK_priv yes



/*===[[ BEG_HEADER ]]=========================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-12345678901-12345678901-*/

/*===[[ ONE_LINERS ]]=========================================================*/

#define     P_FOCUS     "RS (run-time support)"
#define     P_NICHE     "us (user control)"
#define     P_SUBJECT   "vikeys searching and marking"
#define     P_PURPOSE   ""

#define     P_NAMESAKE  "ponos-daimon (hard labor and toil)"
#define     P_HERITAGE  ""
#define     P_IMAGERY   ""
#define     P_REASON    ""

#define     P_ONELINE   P_NAMESAKE " " P_SUBJECT

#define     P_BASENAME  "yMARK"
#define     P_FULLNAME  "/usr/local/lib64/libyMARK"
#define     P_SUFFIX    ""
#define     P_CONTENT   ""

#define     P_SYSTEM    "gnu/linux   (powerful, ubiquitous, technical, and hackable)"
#define     P_LANGUAGE  "ansi-c      (wicked, limitless, universal, and everlasting)"
#define     P_CODESIZE  "large       (appoximately 10,000 slocl)"
#define     P_DEPENDS   "none"

#define     P_AUTHOR    "heatherlyrobert"
#define     P_CREATED   ""

#define     P_VERMAJOR  "2.--, clean, improve, and expand"
#define     P_VERMINOR  "2.0-, separated into independent library"
#define     P_VERNUM    "2.0b"
#define     P_VERTXT    "added and unit tested basic find handling logic"

#define     P_PRIORITY  "direct, simple, brief, vigorous, and lucid (h.w. fowler)"
#define     P_PRINCIPAL "[grow a set] and build your wings on the way down (r. bradbury)"
#define     P_REMINDER  "there are many better options, but i *own* every byte of this one"

/*===[[ END_HEADER ]]=========================================================*/

/*---(ansi-c standard)-------------------*/
#include    <stdio.h>             /* clibc  standard input/output             */
#include    <stdlib.h>            /* clibc  standard general purpose          */
#include    <string.h>            /* clibc  standard string handling          */
/*---(custom)----------------------------*/
#include    <yURG.h>              /* heatherly urgent processing              */
#include    <yLOG.h>              /* heatherly program logging                */
#include    <ySTR.h>              /* heatherly string processing              */
#include    <yMODE.h>             /* heatherly vi-keys mode control           */
#include    <yKEYS.h>             /* heatherly vi-keys key handling           */
#include    <yMACRO.h>            /* heatherly vi-keys macro processing       */
#include    <yCMD.h>
#include    <yMAP.h>              /* heatherly vi-keys location management    */
#include    <ySRC.h>              /* heatherly vi-keys source editing         */
#include    <yVIEW.h>             /* heatherly vi-keys view management        */
#include    <yFILE.h>             /* heatherly vi-keys content file handling  */
#include    <yDLST_solo.h>
#include    <ySORT.h>             /* heatherly sorting and searching          */



typedef   struct   cSRCH   tSRCH;
struct cSRCH {
   /*---(master)------------*/
   uchar       runtime     [LEN_TERSE];
   char        text;
   uchar       runs;
   ushort      found;
   /*---(btree)-------------*/
   tSORT      *btree;
   /*---(done)--------------*/
};


typedef   struct   cFIND   tFIND;
struct cFIND {
   /*---(master)------------*/
   char        label     [LEN_LABEL];
   ushort      u;
   ushort      x;
   ushort      y;
   ushort      z;
   uchar       c;
   /*---(btree)-------------*/
   tSORT      *btree;
   /*---(done)--------------*/
};

#define    B_FIND    'n'
#define    B_SRCH    '/'



typedef    struct    cMY    tMY;
struct cMY {
   char      (*e_regex)      (char *a_search);
   char      (*e_unregex)    (int u, short x, short y, short z);
   char      (*e_hint)       (char *a_search);
   char      (*e_unhint)     (int u, short x, short y, short z);
};
extern tMY         myMARK;


extern char g_print     [LEN_RECD];



/*===[[ yMARK_base.c ]]=======================================================*/
/*··········>·······················>·········································*/
char        ymark__unit_quiet       (void);
char        ymark__unit_loud        (void);
char        ymark__unit_end         (void);
char        ymark__unit_stub        (void);
char        ymark__unit_regex       (char *a_search);
char        ymark__unit_unregex     (char *a_junk);
char*       yMARK__unit             (char *a_question, int n);

/*===[[ yMARK_srch.c ]]=======================================================*/
/*··········>·······················>·········································*/
/*---(program)--------------*/
char        ymark_srch_init         (void);
char        ymark_srch_purge        (void);
char        ymark_srch_wrap         (void);
/*---(memory)---------------*/
char        ymark_srch_new          (uchar *a_label, char a_force, tSRCH **r_new);
char        ymark_srch_free         (tSRCH **r_old);
/*---(search)---------------*/
int         ymark_srch_count        (void);
char        ymark_srch_by_name      (uchar *a_name, tSRCH **r_find);
char        ymark_srch_by_index     (int n, tSRCH **r_find);
char        ymark_srch_by_cursor    (uchar a_dir, tSRCH **r_find);
char        ymark_srch_by_text      (uchar *a_text, tSRCH **r_find);
/*---(execute)--------------*/
char        ymark_srch_direct       (uchar *a_text);
/*---(moves)----------------*/
char        ymark_srch_index        (uchar a_abbr);
char        ymark_srch_clear        (uchar a_abbr);
/*---(done)-----------------*/



/*===[[ yMARK_find.c ]]=======================================================*/
/*··········>·······················>·········································*/
/*---(memory)---------------*/
char        ymark_find_new          (uchar *a_label, char a_force, tFIND **r_new);
char        ymark_find_free         (tFIND **r_old);
/*---(program)--------------*/
char        ymark_find_init         (void);
char        ymark_find_purge        (void);
char        ymark_find_wrap         (void);
/*---(search)---------------*/
int         ymark_find_count        (void);
char        ymark_find_by_label     (uchar *a_name, tFIND **r_find);
char        ymark_find_by_index     (int n, tFIND **r_find);
char        ymark_find_by_cursor    (uchar a_dir, tFIND **r_find);
char*       ymark_find_list         (void);
/*---(execute)--------------*/
char        yMARK_found             (uchar *a_label, ushort u, ushort x, ushort y, ushort z);
char        yMARK_lost              (uchar *a_label);
/*---(done)-----------------*/



#endif

