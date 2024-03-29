/*============================----beg-of-source---============================*/
#ifndef yMARK
#define yMARK yes

#include   <ySTR_solo.h>

typedef  unsigned char        uchar;
typedef  unsigned short       ushort;




/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(internal)-------------*/
char*       yMARK_version           (void);
char        yMARK_init              (void);
char        yMARK_init_after        (void);
char        yMARK_wrap              (void);
/*---(interface)------------*/
char        yMARK_config            (void *a_regex, void *a_unfind, void *a_hint);
char        yMARK_search            (uchar *a_search);
char        yMARK_check             (char a_label [LEN_LABEL], short u, short x, short y, short z, char a_ctype, char a_source [LEN_RECD], char a_string [LEN_RECD], double a_act, char a_print [LEN_RECD]);
char        yMARK_found             (uchar *a_label, ushort u, ushort x, ushort y, ushort z);
char        yMARK_lost              (uchar *a_label);
char        yMARK_execute           (uchar *a_search);
char        yMARK_srch_direct       (uchar *a_string);
char        yMARK_find_hmode        (uchar a_major, uchar a_minor);
/*---(unittest)-------------*/
char*       yMARK__unit             (char *a_question, int n);

char        yMARK_marking           (char *a_label);

char*       yMARK_mark__data        (uchar a_mark);
char        yMARK_mark_list         (char *a_show, char *a_list);
char        yMARK_mark_writer_all   (void);
char        yMARK_mark_reader       (int n, char *a_verb);
char        yMARK_mark_status       (char a_size, short a_wide, char *a_list);

char        yMARK_find_status       (char a_size, short a_wide, char *a_list);


#endif
/*============================----end-of-source---============================*/
