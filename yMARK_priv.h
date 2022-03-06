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
#define     P_VERNUM    "2.0a"
#define     P_VERTXT    "initial files created"

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


#define     CMDS_BASE        'b'  /* fully standard in base           */
#define     CMDS_CUST        'c'  /* fully custom, not in base        */

#define     MAX_BASE    500
typedef  struct cCMDS   tCMDS;
struct  cCMDS {
   uchar       base;
   uchar       menu;                        /* category                       */
   uchar       name        [LEN_LABEL];     /* full command name              */
   uchar       abbr        [LEN_TERSE];     /* abbreviation of name           */
   union {
      char        (*v     ) (void);
      char        (*c     ) (char);
      char        (*cc    ) (char , char );
      char        (*i     ) (int);
      char        (*ii    ) (int, int);
      char        (*iii   ) (int, int, int);
      char        (*isss  ) (int, char*, char*, char*);
      char        (*is    ) (int  , char*);
      char        (*s     ) (char*);
      char        (*ss    ) (char*, char*);
      char        (*si    ) (char*, int  );
      char        (*sii   ) (char*, int, int);
      char        (*css   ) (char, char*, char*);
      char        (*cs    ) (char, char*);
      char        (*f     ) (float);
      char        (*cciiii) (char, char, int, int, int, int);
      char        (*iiii  ) (int, int, int, int);
      char        (*ci    ) (char, int);
   } f;
   uchar       terms       [LEN_TERSE];     /* type of terms/args             */
   uchar       desc        [LEN_DESC];      /* descriptive label              */
};


#define     MAX_TERMS         50
typedef    struct   cTERMS  tTERMS;
struct cTERMS {
   char        name        [LEN_LABEL];
   char        disp        [LEN_DESC ];
};
extern const tTERMS  s_terms [];


typedef  struct cLINK   tLINK;
struct cLINK {
   tCMDS      *data;     /* actual command                  */
   char        nlen;     /* length of name (speed searches) */
   char        alen;     /* length of name (speed searches) */
   char        nterms;   /* index to terms db (speed exec)  */
   tLINK      *m_next;
};



/*===[[ MENUS ]]==============================================================*/
#define    LEN_NAME        11
#define    MAX_MENU        1000
typedef    struct   cMENU   tMENU;
struct cMENU {
   /*---(original)----------*/
   uchar       base;                        /* original item (not added)      */
   /*---(organize)----------*/
   uchar       top;                         /* first level key                */
   uchar       mid;                         /* second level key               */
   uchar       bot;                         /* third level key                */
   /*---(master)------------*/
   uchar       name        [LEN_NAME];      /* name for use on screens        */
   uchar       active;                      /* program activation             */
   uchar       type;                        /* entry type   > ! = ·           */
   uchar       keys        [LEN_TITLE];     /* command or keys to execute     */
   /*---(done)--------------*/
};
#define     MENU_BASE        'b'  /* fully standard in base           */
#define     MENU_HELD        'h'  /* base saved name but not keys     */
#define     MENU_CUSTOM      'c'  /* fully custom, not in base        */

#define     MENU_PLACE       'p'  /* put new menu in heirarchy        */
#define     MENU_FIND        'f'  /* find entry in heirarchy          */
#define     MENU_MENU        'm'  /* find entry for menu              */
#define     MENU_ACTIVE      'y'  /* mark entry active in heirarchyi  */
#define     MENU_GREY        '-'  /* mark entry inactive in heirarchy */
#define     MENU_HIDE        '/'  /* hide entry in heirarchy          */

#define     MENU_GROUP       '>'  /* group type entry                 */
#define     MENU_NAMER       '!'  /* end item name/place holder       */
#define     MENU_EXACT       '·'  /* end item with all keys needed    */
#define     MENU_CMD         ':'  /* end item command with all keys   */
#define     MENU_MORE        '='  /* end item needing more keys       */
#define     MENU_TBD         '?'  /* end item not yet completed       */

typedef    struct   cMLINK   tMLINK;
struct cMLINK {
   char        active;                      /* current active status          */
   tMENU      *data;                        /* pointer to menu entry          */
   tMLINK     *m_next;                      /* master list next               */
   tMLINK     *m_prev;                      /* master list prev               */
   tMLINK     *s_next;                      /* next sibling                   */
   tMLINK     *c_first;                     /* first child                    */
   uchar       c_count;                     /* count of children              */
};


#define  MAX_PLACE   50
typedef    struct   cPLACE   tPLACE;
struct cPLACE {
   tMLINK*      item;
   char        align;
   int         x, xo;
   int         y, yo;
};



typedef    struct    cMY    tMY;
struct cMY {
   char      (*e_regex)      (char *a_search);
   char      (*e_unregex)    (int u, short x, short y, short z);
   char      (*e_hint)       (char *a_search);
   char      (*e_unhint)     (int u, short x, short y, short z);
};
extern tMY         myMARK;


extern char g_print     [LEN_RECD];



/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(base)-----------------*/
char        ycmd__purge             (void);



/*===[[ yCMD_terms.c ]]=======================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(program)--------------*/
char        ycmd_terms_init         (void);
char        ycmd_terms_clear        (void);
/*---(accessor)-------------*/
char        ycmd_terms_count        (void);
char*       ycmd_terms_show         (char n);
/*---(new)------------------*/
char        ycmd_terms              (uchar *a_terms);
/*---(exec)-----------------*/
char        ycmd_launch             (tLINK *a_link, char *a_rc);
/*---(done)-----------------*/



/*===[[ yCMD_load.c ]]========================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(program)--------------*/
char        ycmd_load_init          (void);
/*---(support)--------------*/
char        ycmd_check_name         (uchar *a_name, char a_type, int *r_len);
/*---(search)---------------*/
int         ycmd_load_count         (void);
char        ycmd_load_by_name       (uchar *a_name, tCMDS **r_cmds);
/*---(memory)---------------*/
char        ycmd_new_cmd            (uchar a_menu, uchar *a_name, uchar *a_abbr, uchar *a_terms, void *a_func, uchar *a_desc, tCMDS **r_cmds);
char        ycmd_load               (void);
char        ycmd_new_link           (tCMDS *a_cmd, tLINK **r_link);



/*===[[ yCMD_exec.c ]]========================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        ycmd_exec_purge         (void);
int         ycmd_exec_count         (void);
char        ycmd_check_dup          (uchar a_base, uchar *a_name, uchar *a_abbr);
char        ycmd_exec_by_name       (uchar *a_name, tLINK **r_link);
char        ycmd_exec_by_index      (short a_index, tLINK **r_link);
char        ycmd_parse              (uchar *a_str);



/*===[[ yCMD_exec.c ]]========================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        ycmd__unit_stub         (void);
char        ycmd__unit_parrot       (char a_value);
char        ycmd__unit_length       (char *a_string);
char        ycmd__unit_tester       (char a_mode, char a_value);


char        ycmd_dump               (FILE *f);



/*===[[ yCMD_exec.c ]]========================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(support)--------------*/
char        ycmd__menu_fix_path     (uchar *a_path, int *a_len, uchar *a_fixed);
/*---(memory)---------------*/
char        ycmd__menu_newlink      (tMENU *a_menu);
char        ycmd__menu_new          (uchar *a_path, char *a_name, char *a_keys);
/*---(actions)--------------*/
int         ycmd__menu_action       (uchar *a_path, int a_max, int a_lvl, tMLINK *a_parent, char a_act, tMLINK *a_new);
int         ycmd_menu_action        (char a_act, uchar *a_path, tMLINK *a_link);
char        ycmd_menu_place         (tMLINK *a_new);
int         ycmd_menu_delete        (uchar *a_path);
int         ycmd_menu_find          (uchar *a_path);
int         ycmd_menu_menu          (uchar *a_path);
int         yCMD_menu_active        (uchar *a_path);
int         yCMD_menu_grey          (uchar *a_path);
int         yCMD_menu_hide          (uchar *a_path);
/*---(program)--------------*/
char        ycmd__menu_purge        (void);
char        yCMD_menu_config        (char a_layout);
char        ycmd_menu_init          (void);
/*---(base)-----------------*/
char        ycmd__menu_in_base      (uchar *a_path);
char        ycmd__menu_base_num     (int n);
char        ycmd__menu_base_path    (uchar *a_path);
char        ycmd__menu_base_load    (void);
/*---(placement)------------*/
char        ycmd__menu_place_clear  (void);
char        ycmd__menu_place_round  (void);
char        ycmd__menu_place_cols   (char a_level, tMLINK *a_curr);
/*---(update)---------------*/
char        yCMD_menu_alter         (uchar *a_path, char *a_keys);
char        yCMD_menu_add           (uchar *a_path, char *a_name, char *a_keys);
/*---(menu)-----------------*/
char        ycmd_menu_reanchor      (char a_anchor);
char        ycmd_menu_final         (void);
/*---(mode)-----------------*/
char        ycmd_menu_start         (void);
char        ycmd_menu_smode         (uchar a_major, uchar a_minor);
/*---(done)-----------------*/

char*       ycmd_menu_counts        (void);
char*       ycmd_menu_by_index      (int n);



#endif

