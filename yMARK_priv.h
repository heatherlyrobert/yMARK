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
#define     P_VERNUM    "2.0d"
#define     P_VERTXT    "added marks and unit testing for them"

#define     P_PRIORITY  "direct, simple, brief, vigorous, and lucid (h.w. fowler)"
#define     P_PRINCIPAL "[grow a set] and build your wings on the way down (r. bradbury)"
#define     P_REMINDER  "there are many better options, but i *own* every byte of this one"

/*===[[ END_HEADER ]]=========================================================*/

/*---(ansi-c standard)-------------------*/
#include    <stdio.h>             /* clibc  standard input/output             */
#include    <stdlib.h>            /* clibc  standard general purpose          */
#include    <string.h>            /* clibc  standard string handling          */
/*---(custom core)-----------------------*/
#include    <yURG.h>              /* heatherly urgent processing              */
#include    <yLOG.h>              /* heatherly program logging                */
#include    <ySTR.h>              /* heatherly string processing              */
/*---(custom vikeys)---------------------*/
#include    <yMODE.h>             /* heatherly vi-keys mode processing        */
#include    <yKEYS.h>             /* heatherly vi-keys mode processing        */
#include    <yFILE.h>             /* heatherly vi-keys content file handling  */
#include    <yMAP.h>              /* heatherly vi-keys location management    */
#include    <yMACRO.h>            /* heatherly vi-keys macro processing       */
#include    <yCMD.h>
#include    <ySRC.h>              /* heatherly vi-keys source editing         */
#include    <yVIEW.h>             /* heatherly vi-keys view management        */
/*---(custom other)----------------------*/
#include    <yPARSE.h>            /* heatherly file reading and writing       */
#include    <yDLST_solo.h>
#include    <ySORT.h>             /* heatherly sorting and searching          */



#define      SEARCH_REPO         "/var/lib/vikeys/search.repo"
#define      SEARCH_LOCAL        "/.vikeys/search.repo"



typedef   struct   cSRCH   tSRCH;
struct cSRCH {
   /*---(master)------------*/
   uchar       runtime     [LEN_TERSE];
   uchar       seq         [LEN_TERSE];
   char       *text;
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
   char      (*e_regex)      (uchar a_not, uchar *a_search);
   char      (*e_unfind)     (uchar *a_label, ushort u, ushort x, ushort y, ushort z);
   char      (*e_hint)       (uchar *a_hint);
};
extern tMY         myMARK;


extern char g_print     [LEN_RECD];



/*===[[ yMARK_base.c ]]=======================================================*/
/*··········>·······················>·········································*/
char        ymark__unit_quiet       (void);
char        ymark__unit_loud        (void);
char        ymark__unit_end         (void);
char        ymark__unit_stub        (void);
char        ymark__unit_regex       (uchar a_not, uchar *a_search);
char        ymark__unit_unfind      (ushort u, ushort x, ushort y, ushort z);
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
char        ymark_srch_by_name      (uchar *a_name, tSRCH **r_srch);
char        ymark_srch_by_index     (int n, tSRCH **r_srch);
char        ymark_srch_by_cursor    (uchar a_dir, tSRCH **r_srch);
char        ymark_srch_by_text      (uchar *a_text, tSRCH **r_srch);
char*       ymark_srch_list         (void);
char*       ymark_srch__entry       (int n, uchar a_abbr, tSRCH *a_srch);
char*       ymark_srch_entry        (int n);
char*       ymark_srch_marked       (uchar a_abbr);
char*       ymark_srch_marks        (void);
/*---(execute)--------------*/
char        yMARK_execute           (uchar *a_search);
/*---(moves)----------------*/
char        ymark_srch_index        (uchar a_abbr);
char        ymark_srch_mark         (uchar a_abbr);
char        ymark_srch_unmark       (uchar a_abbr);
char        ymark_srch_export       (uchar a_abbr);
char        ymark_srch_import       (uchar a_abbr);
char        ymark_srch_copy         (uchar a_src, uchar a_dst);
char        ymark_srch_full         (uchar a_abbr, uchar *a_text);;
char        yMARK_srch_direct       (uchar *a_string);
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



/*===[[ yMARK_mark.c ]]=======================================================*/
/*··········>·······················>·········································*/
/*---(program)--------------*/
char        ymark_mark_init         (void);
char        ymark_mark__purge       (char a_scope);
/*---(support)--------------*/
int         ymark_mark__index       (uchar a_abbr);
/*---(setting)--------------*/
char        ymark_mark__history     (uchar a_mark);
char        ymark_mark__set         (uchar a_mark);
char        ymark_mark__unset       (uchar a_mark);
char        ymark_mark__return      (uchar a_mark);
/*---(finding)--------------*/
char        ymark_mark__which       (void);
char        ymark_mark__find        (char *a_label);
/*---(sequence)-------------*/
char        ymark_mark__range       (void);
char        ymark_mark__prev        (void);
char        ymark_mark__next        (void);
/*---(status)---------------*/
char*       ymark_mark__data        (uchar a_mark);
char        ymark_mark_info         (char *a_entry, int a_index);
char        ymark_mark_status       (char *a_status);
char        yMARK_marking           (char *a_label);
char        ymark_mark_marklist     (char *a_list);
char        ymark_mark_listplus     (char *a_list);
/*---(file))----------------*/
char        ymark_mark_writer       (int c, uchar a_abbr);
char        yMARK_mark_writer_all   (void);
char        yMARK_mark_reader       (int n, char *a_verb);
/*---(command)--------------*/
char        ymark_mark_direct       (char *a_string);
/*---(mode)-----------------*/
char        ymark_mark_smode        (uchar a_major, uchar a_minor);
/*---(done)-----------------*/


#endif

