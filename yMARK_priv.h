/*===============================[[ beg code ]]===============================*/
#ifndef yMARK_priv
#define yMARK_priv yes


/*===[[ HEADER BEG ]]=========================================================*/
/*                      ´·········1·········2·········3·········4·········5·········6·········7*/
/*--------- 12345678901 ´123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/
/*········· ··········· ´·····························´········································*/
#define     P_NAME      "yMARK"
/*········· ··········· ´·····························´········································*/
#define     P_FOCUS     "RS (run-time support)"
#define     P_NICHE     "us (user control)"
#define     P_SUBJECT   "vikeys searching and marking"
#define     P_PURPOSE   ""
/*········· ··········· ´·····························´········································*/
#define     P_NAMESAKE  "ponos-daimon (hard labor and toil)"
#define     P_PRONOUNCE ""
#define     P_HERITAGE  ""
#define     P_BRIEFLY   ""
#define     P_IMAGERY   ""
#define     P_REASON    ""
/*········· ··········· ´·····························´········································*/
#define     P_ONELINE   P_NAMESAKE " " P_SUBJECT
/*········· ··········· ´·····························´········································*/
#define     P_HOMEDIR   "/home/system/yMARK.vikeys_searching_and_marking"
#define     P_BASENAME  "yMARK"
#define     P_FULLNAME  "/usr/local/lib64/libyMARK"
#define     P_SUFFIX    "n/a"
#define     P_CONTENT   "n/a"
/*········· ··········· ´·····························´········································*/
#define     P_SYSTEM    "gnu/linux   (powerful, ubiquitous, technical, and hackable)"
#define     P_LANGUAGE  "ansi-c      (wicked, limitless, universal, and everlasting)"
#define     P_COMPILER  "gcc 11.3.0"
#define     P_CODESIZE  "large       (appoximately 10,000 slocl)"
/*········· ··········· ´·····························´········································*/
#define     P_DEPSTDC   "stdio,stdlib,string"
#define     P_DEPPOSIX  ""
#define     P_DEPCORE   "yURG,yLOG,ySTR"
#define     P_DEPVIKEYS "yVIHUB,yMODE,yKEYS,ySRC"
#define     P_DEPOTHER  "yPARSE,ySORT"
#define     P_DEPGRAPH  ""
#define     P_DEPHEAD   "yDLST_solo"
/*········· ··········· ´·····························´········································*/
#define     P_AUTHOR    "heatherlyrobert"
#define     P_CREATED   ""
/*········· ··········· ´·····························´········································*/
#define     P_VERMAJOR  "2.--, clean, improve, and expand"
#define     P_VERMINOR  "2.0-, separated into independent library"
#define     P_VERNUM    "2.0n"
#define     P_VERTXT    "missed old chrvisible references"
/*········· ··········· ´·····························´········································*/
#define     P_PRIORITY  "direct, simple, brief, vigorous, and lucid (h.w. fowler)"
#define     P_PRINCIPAL "[grow a set] and build your wings on the way down (r. bradbury)"
#define     P_REMINDER  "there are many better options, but i *own* every byte of this one"
/*········· ··········· ´·····························´········································*/
/*--------- 12345678901 ´123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/
/*                      ´·········1·········2·········3·········4·········5·········6·········7*/

/*---(ansi-c standard)-------------------*/
#include    <stdio.h>             /* clibc  standard input/output             */
#include    <stdlib.h>            /* clibc  standard general purpose          */
#include    <string.h>            /* clibc  standard string handling          */
/*---(custom core)-----------------------*/
#include    <yURG.h>              /* heatherly urgent processing              */
#include    <yLOG.h>              /* heatherly program logging                */
#include    <ySTR.h>              /* heatherly string processing              */
/*---(custom vikeys)---------------------*/
#include    <yVIHUB.h>
#include    <yMODE.h>             /* heatherly vi-keys mode processing        */
#include    <yKEYS.h>             /* heatherly vi-keys mode processing        */
#include    <ySRC.h>              /* heatherly vi-keys source editing         */
/*---(custom other)----------------------*/
#include    <yPARSE.h>            /* heatherly file reading and writing       */
#include    <yREGEX.h>       /* CUSTOM : heatherly regular expressions        */
#include    <ySORT.h>             /* heatherly sorting and searching          */
#include    <yDLST_solo.h>



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
   char        sort      [LEN_TITLE];
   tSORT      *btree;
   /*---(done)--------------*/
};

#define    B_FIND    'n'
#define    B_SRCH    '/'



typedef    struct    cMY    tMY;
struct cMY {
   char      (*e_regex)      (char a_scope);
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
char        ymark__unit_regex       (char a_scope);
char        ymark__unit_regex_OLD   (uchar a_not, uchar *a_search);
char        ymark__unit_unfind      (ushort u, ushort x, ushort y, ushort z);
char*       yMARK__unit             (char *a_question, int n);

/*===[[ yMARK_srch.c ]]=======================================================*/
/*··········>·······················>·········································*/
/*---(program)--------------*/
char        ymark_srch_init         (void);
/*> char        ymark_srch_purge        (void);                                       <*/
char        ymark_srch_wrap         (void);
/*---(memory)---------------*/
/*> char        ymark_srch_new          (uchar *a_label, char a_force, tSRCH **r_new);   <*/
/*> char        ymark_srch_free         (tSRCH **r_old);                              <*/
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
char        ymark_srch__prepare     (char b_search [LEN_RECD], char *r_type, char *r_join, char *r_not, char *r_hist, char *r_comp, double *r_exp);
char        ymark_srch__run         (char a_search [LEN_RECD], char a_scope, char a_type, char a_join);
char        yMARK_check             (char a_label [LEN_LABEL], short u, short x, short y, short z, char a_ctype, char a_source [LEN_RECD], char a_string [LEN_RECD], double a_act, char a_print [LEN_RECD]);
char        ymark_srch__prune       (char a_join);
char        yMARK_execute           (uchar *a_search);
/*---(moves)----------------*/
/*> char        ymark_srch_index        (uchar a_abbr);                               <*/
/*> char        ymark_srch_mark         (uchar a_abbr);                               <*/
/*> char        ymark_srch_unmark       (uchar a_abbr);                               <*/
/*> char        ymark_srch_export       (uchar a_abbr);                               <*/
/*> char        ymark_srch_import       (uchar a_abbr);                               <*/
/*> char        ymark_srch_copy         (uchar a_src, uchar a_dst);                   <*/
/*> char        ymark_srch_full         (uchar a_abbr, uchar *a_text);;               <*/
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
char*       yMARK_mark__data        (uchar a_mark);
char        ymark_mark_info         (char *a_entry, int a_index);
char        ymark_mark_status       (char *a_status);
char        yMARK_marking           (char *a_label);
char        ymark_mark_marklist     (char *a_list);
char        ymark_mark_listplus     (char *a_list);
/*---(file))----------------*/
char        ymark_mark_writer       (int c, uchar a_abbr);
char        yMARK_mark_writer_all   (void);
char        yMARK_mark_reader       (int n, char *a_verb);
char        ymark_mark_dump         (void *f);
/*---(command)--------------*/
char        ymark_mark_direct       (char *a_string);
/*---(mode)-----------------*/
char        ymark_mark_smode        (uchar a_major, uchar a_minor);
/*---(done)-----------------*/


#endif

