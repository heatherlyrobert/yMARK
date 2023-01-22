/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "yMARK.h"
#include    "yMARK_priv.h"



static char s_mark_head   = '·';
static char s_mark_tail   = '·';
static char s_mark_curr   = '·';
static char s_mark_prev   = '·';



#define     S_MARK_MAX       100
typedef  struct cMARK  tMARK;
struct cMARK {
   char        label       [LEN_LABEL];
   ushort      u;
   ushort      x;
   ushort      y;
   ushort      z;
   char        source;
};
static tMARK  s_mark_info [S_MARK_MAX];
static uchar  S_MARK_LIST [S_MARK_MAX];
static int    s_nmark       = 0;


#define     MARK_NONE     '-'
#define     MARK_USER     'u'
#define     MARK_IMPORT   'i'
#define     MARK_AUTO     'a'

static char s_marking  = '-';



/*====================------------------------------------====================*/
/*===----                       program level                          ----===*/
/*====================------------------------------------====================*/
static void  o___PROGRAM_________o () { return; }

char         /*-> initialize all marks ---------------[ shoot  [gz.311.001.01]*/ /*-[00.0000.102.4]-*/ /*-[--.---.---.--]-*/
ymark_mark_init         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (!yMODE_check_prep  (UMOD_MARK)) {
      DEBUG_YMARK   yLOG_note    ("status is not ready for init");
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(macro abbrev list)--------------*/
   strlcpy (S_MARK_LIST, "'"           , S_MARK_MAX);
   strlcat (S_MARK_LIST, YSTR_LOWER    , S_MARK_MAX);
   strlcat (S_MARK_LIST, YSTR_UPPER    , S_MARK_MAX);
   strlcat (S_MARK_LIST, YSTR_NUMBER   , S_MARK_MAX);
   strlcat (S_MARK_LIST, YSTR_GREEK    , S_MARK_MAX);
   strlcat (S_MARK_LIST, "<>¤"         , S_MARK_MAX);
   DEBUG_YMARK   yLOG_info    ("LIST"      , S_MARK_LIST);
   s_nmark = strlen (S_MARK_LIST);
   /*---(clear)--------------------------*/
   ymark_mark__purge  (YSTR_IFULL);
   /*---(globals)------------------------*/
   s_marking = '-';
   /*---(update status)------------------*/
   yMODE_init_set   (UMOD_MARK, NULL, ymark_mark_smode);
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> remove marks -----------------------[ ------ [gz.642.141.12]*/ /*-[01.0000.033.3]-*/ /*-[--.---.---.--]-*/
ymark_mark__purge       (char a_scope)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   char        x_abbr      =    0;
   /*---(header)-------------------------*/
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YMARK   yLOG_char    ("a_scope"   , a_scope);
   --rce;  switch (a_scope) {
   case YSTR_ILOWER  : case YSTR_IUPPER  : case YSTR_INUMBER :
   case YSTR_IGREEK  : case YSTR_IFULL   :
      break;
   default :
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(clear)--------------------------*/
   DEBUG_YMARK   yLOG_value   ("nmark"     , s_nmark);
   for (i = 0; i < s_nmark; ++i) {
      x_abbr = S_MARK_LIST [i];
      if (a_scope == YSTR_IUPPER  && strchr (YSTR_UPPER , x_abbr) == NULL)  continue;
      if (a_scope == YSTR_ILOWER  && strchr (YSTR_LOWER , x_abbr) == NULL)  continue;
      if (a_scope == YSTR_INUMBER && strchr (YSTR_NUMBER, x_abbr) == NULL)  continue;
      if (a_scope == YSTR_IGREEK  && strchr (YSTR_GREEK , x_abbr) == NULL)  continue;
      ymark_mark__unset   (x_abbr);
   }
   /*---(globals)------------------------*/
   DEBUG_YMARK   yLOG_note    ("initialize globals");
   s_mark_head  = '·';
   s_mark_tail  = '·';
   s_mark_curr  = '·';
   s_mark_prev  = '·';
   ymark_mark__range   ();
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       supporting functions                   ----===*/
/*====================------------------------------------====================*/
static void  o___SUPPORT_________o () { return; }

int  
ymark_mark__index       (uchar a_abbr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =   -1;
   /*---(header)-------------------------*/
   DEBUG_YMARK   yLOG_senter  (__FUNCTION__);
   /*---(check)--------------------------*/
   DEBUG_YMARK   yLOG_snote   ("check");
   n  = strlchr (S_MARK_LIST, a_abbr, S_MARK_MAX);
   DEBUG_YMARK   yLOG_sint    (n);
   --rce;  if (n  < 0) {
      DEBUG_YMARK   yLOG_sexitr  (__FUNCTION__, n);
      return n;
   }
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_sexit   (__FUNCTION__);
   return n;
}



/*====================------------------------------------====================*/
/*===----                        setting and unsetting                 ----===*/
/*====================------------------------------------====================*/
static void  o___SETTING_________o () { return; }

char
ymark_mark__history     (uchar a_mark)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         n           =    0;
   char        c           =  '-';
   /*---(header)-------------------------*/
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   DEBUG_YMARK   yLOG_complex ("marks"     , "ask = %c, cur = %c, prv = %c", a_mark, s_mark_curr, s_mark_prev);
   /*---(check for duplicate)------------*/
   if (a_mark == s_mark_curr) {
      DEBUG_YMARK   yLOG_note    ("duplicate move");
      DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(cycle history)------------------*/
   if (a_mark == '\'') {
      c           = s_mark_prev;
      s_mark_prev = s_mark_curr;
      s_mark_curr = c;
   } else {
      s_mark_prev = s_mark_curr;
      s_mark_curr = a_mark;
   }
   DEBUG_YMARK   yLOG_complex ("update"    , "ask = %c, cur = %c, prv = %c", a_mark, s_mark_curr, s_mark_prev);
   /*---(set prev mark)------------------*/
   if (s_mark_prev != '·') {
      n = ymark_mark__index  (s_mark_prev);
   } else {
      n = ymark_mark__index  (s_mark_curr);
   }
   DEBUG_YMARK   yLOG_value   ("n"         , n);
   /*---(fill in data)-------------------*/
   strlcpy (s_mark_info [0].label, s_mark_info [n].label, LEN_LABEL);
   DEBUG_YMARK   yLOG_info    ("'"         , s_mark_info [0].label);
   s_mark_info [0].u  = s_mark_info [n].u;
   s_mark_info [0].x  = s_mark_info [n].x;
   s_mark_info [0].y  = s_mark_info [n].y;
   s_mark_info [0].z  = s_mark_info [n].z;
   s_mark_info [0].source = MARK_AUTO;
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
}

char         /*-> tbd --------------------------------[ ------ [ge.A52.153.55]*/ /*-[01.0000.033.8]-*/ /*-[--.---.---.--]-*/
ymark_mark__set               (uchar a_mark)
{
   /*---(locals)-----------+-----------+-*/
   char        rc          = 0;
   char        rce         = -10;
   int         n           =   0;
   char        x_label     [10];
   ushort      b, x, y, z;
   /*---(header)-------------------------*/
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   DEBUG_YMARK   yLOG_char    ("a_mark"    , a_mark);
   /*---(check mark)---------------------*/
   n = ymark_mark__index (a_mark);
   DEBUG_YMARK   yLOG_value   ("n"   , n);
   --rce;  if (n < 0) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (n == 0) {
      DEBUG_YMARK   yLOG_note    ("never directly set previous mark");
      DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(get current)--------------------*/
   rc = yVIHUB_yMAP_current  (x_label, &b, &x, &y, &z);
   --rce;  if (rc < 0) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(mark label)---------------------*/
   DEBUG_YMARK   yLOG_note    ("save current position");
   strlcpy (s_mark_info [n].label, x_label, LEN_LABEL);
   s_mark_info [n].u  = b;
   s_mark_info [n].x  = x;
   s_mark_info [n].y  = y;
   s_mark_info [n].z  = z;
   s_mark_info [n].source = MARK_USER;
   /*---(set history)--------------------*/
   ymark_mark__history (a_mark);
   /*---(update range)-------------------*/
   DEBUG_YMARK   yLOG_note    ("update the range");
   ymark_mark__range   ();
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> clear a mark -----------------------[ ------ [ge.732.142.12]*/ /*-[01.0000.033.8]-*/ /*-[--.---.---.--]-*/
ymark_mark__unset       (uchar a_mark)
{
   /*---(locals)-----------+-----------+-*/
   char        rc          = 0;
   char        rce         = -10;
   int         x_index     =   0;
   char        x_label     [10];
   /*---(header)-------------------------*/
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   DEBUG_YMARK   yLOG_char    ("a_mark"    , a_mark);
   /*---(check mark)---------------------*/
   x_index = ymark_mark__index (a_mark);
   --rce;  if (x_index < 0) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(clear mark)---------------------*/
   DEBUG_YMARK   yLOG_note    ("clear values");
   strlcpy (s_mark_info [x_index].label, "-", LEN_LABEL);
   s_mark_info [x_index].source  = MARK_NONE;
   s_mark_info [x_index].u   = 0;
   s_mark_info [x_index].x   = 0;
   s_mark_info [x_index].y   = 0;
   s_mark_info [x_index].z   = 0;
   /*---(update range)-------------------*/
   DEBUG_YMARK   yLOG_note    ("update the range");
   ymark_mark__range   ();
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> tbd --------------------------------[ ------ [ge.E83.174.A7]*/ /*-[01.0000.013.#]-*/ /*-[--.---.---.--]-*/
ymark_mark__return      (uchar a_mark)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         x_index     =    0;
   char        x_label     [10];
   char        x_prev      =   -1;
   /*---(header)-------------------------*/
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   DEBUG_YMARK   yLOG_char    ("a_mark"    , a_mark);
   /*---(look for sequences)-------------*/
   DEBUG_YMARK   yLOG_note    ("check special shortcuts");
   switch (a_mark) {
   case '[' : a_mark = s_mark_head;             break;
   case '<' : a_mark = ymark_mark__prev   ();   break;
   case '>' : a_mark = ymark_mark__next   ();   break;
   case ']' : a_mark = s_mark_tail;             break;
   }
   --rce;  if (a_mark < 0) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check mark)---------------------*/
   DEBUG_YMARK   yLOG_char    ("a_mark"    , a_mark);
   x_index = ymark_mark__index (a_mark);
   DEBUG_YMARK   yLOG_value   ("x_index"   , x_index);
   --rce;  if (x_index < 0) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check for existance)------------*/
   strlcpy (x_label, s_mark_info [x_index].label, LEN_LABEL);
   DEBUG_YMARK   yLOG_info    ("x_label"   , x_label);
   DEBUG_YMARK   yLOG_char    ("source"    , s_mark_info [x_index].source);
   --rce;  if (s_mark_info [x_index].source == MARK_NONE) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(move)---------------------------*/
   DEBUG_YMARK   yLOG_note    ("jump to mark");
   yVIHUB_yMAP_jump (s_mark_info [x_index].u, s_mark_info [x_index].x, s_mark_info [x_index].y, s_mark_info [x_index].z);
   /*---(set history)--------------------*/
   ymark_mark__history (a_mark);
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> tbd --------------------------------[ ------ [ge.A52.153.55]*/ /*-[01.0000.033.8]-*/ /*-[--.---.---.--]-*/
ymark_mark__copy              (char a_src, char a_dst)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   int         x_src       =   0;
   int         x_dst       =   0;
   /*---(header)-------------------------*/
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   DEBUG_YMARK   yLOG_char    ("a_src"     , a_src);
   /*---(check mark)---------------------*/
   x_src = ymark_mark__index (a_src);
   DEBUG_YMARK   yLOG_value   ("x_src"   , x_src);
   --rce;  if (x_src < 0) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_dst = ymark_mark__index (a_dst);
   DEBUG_YMARK   yLOG_value   ("x_dst"   , x_dst);
   --rce;  if (x_dst < 0) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(mark label)---------------------*/
   DEBUG_YMARK   yLOG_note    ("copy position");
   strlcpy (s_mark_info [x_dst].label, s_mark_info [x_src].label, LEN_LABEL);
   s_mark_info [x_dst].u  = s_mark_info [x_src].u;
   s_mark_info [x_dst].x  = s_mark_info [x_src].x;
   s_mark_info [x_dst].y  = s_mark_info [x_src].y;
   s_mark_info [x_dst].z  = s_mark_info [x_src].z;
   s_mark_info [x_dst].source = s_mark_info [x_src].source;
   /*---(update range)-------------------*/
   DEBUG_YMARK   yLOG_note    ("update the range");
   ymark_mark__range   ();
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      location marks                          ----===*/
/*====================------------------------------------====================*/
static void  o___FINDING_________o () { return; }

char         /*-> tbd --------------------------------[ leaf   [gc.430.022.40]*/ /*-[01.0000.013.5]-*/ /*-[--.---.---.--]-*/
ymark_mark__which       (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   char        x_label     [LEN_LABEL];
   /*---(get current)--------------------*/
   rc = yVIHUB_yMAP_current  (x_label, NULL, NULL, NULL, NULL);
   --rce;  if (rc < 0) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(find label)---------------------*/
   --rce;  for (i = 1; i < s_nmark; ++i) {
      if (s_mark_info [i].source == MARK_NONE)          continue;
      if (strcmp (s_mark_info [i].label, x_label) != 0) continue;
      return S_MARK_LIST [i];
   }
   /*---(complete)-----------------------*/
   return rce;
}

char         /*-> tbd --------------------------------[ leaf   [ge.732.133.30]*/ /*-[01.0000.024.#]-*/ /*-[--.---.---.--]-*/
ymark_mark__find        (char *a_label)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   DEBUG_YMARK   yLOG_point   ("a_label"   , a_label);
   /*---(defense)------------------------*/
   --rce;  if (a_label == NULL) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YMARK   yLOG_info    ("a_label"   , a_label);
   --rce;  if (strcmp (a_label, "-") == 0) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(search)-------------------------*/
   DEBUG_YMARK   yLOG_note    ("searching");
   for (i = 1; i < s_nmark; ++i) {
      if (strcmp (s_mark_info [i].label, "")      == 0) continue;
      if (strcmp (s_mark_info [i].label, "-")     == 0) continue;
      if (strcmp (s_mark_info [i].label, a_label) != 0) continue;
      DEBUG_YMARK   yLOG_value   ("i"         , i);
      DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
      return i;
   }
   /*---(complete)-----------------------*/
   --rce;
   DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
   return rce;
}



/*====================------------------------------------====================*/
/*===----                      location marks                          ----===*/
/*====================------------------------------------====================*/
static void  o___SEQUENCE________o () { return; }

char         /*-> tbd --------------------------------[ leaf   [gz.640.021.20]*/ /*-[01.0000.044.!]-*/ /*-[--.---.---.--]-*/
ymark_mark__range       (void)
{
   /*---(design notes)-------------------*/
   /*
    *  do not include single quote register #0 in searches
    */
   /*---(locals)-----------+-----------+-*/
   int         i           =    0;
   /*---(find next)----------------------*/
   s_mark_head = '·';
   for (i = 1; i < s_nmark; ++i) {
      if (s_mark_info [i].source == MARK_NONE) continue;
      s_mark_head = S_MARK_LIST [i];
      break;
   }
   /*---(find last)----------------------*/
   s_mark_tail = '·';
   for (i = s_nmark - 1; i >  0; --i) {
      if (s_mark_info [i].source == MARK_NONE) continue;
      s_mark_tail = S_MARK_LIST [i];
      break;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char         /*-> tbd --------------------------------[ ------ [ge.732.043.21]*/ /*-[01.0000.014.!]-*/ /*-[--.---.---.--]-*/
ymark_mark__prev        (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        x_mark      =  '-';
   int         x_index     =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   /*---(check mark)---------------------*/
   x_index = ymark_mark__index  (s_mark_curr);
   --rce;  if (x_index < 0) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YMARK   yLOG_value   ("x_index"   , x_index);
   /*---(find previous)------------------*/
   DEBUG_YMARK   yLOG_note    ("search for previous mark");
   for (i = x_index - 1; i > 0; --i) {
      if (s_mark_info [i].source == MARK_NONE)  continue;
      DEBUG_YMARK   yLOG_value   ("found"     , i);
      x_mark = S_MARK_LIST [i];
      DEBUG_YMARK   yLOG_char    ("x_mark"    , x_mark);
      DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
      return x_mark;
   }
   DEBUG_YMARK   yLOG_note    ("not found");
   --rce;
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
   return rce;
}

char         /*-> tbd --------------------------------[ ------ [ge.842.053.21]*/ /*-[01.0000.014.!]-*/ /*-[--.---.---.--]-*/
ymark_mark__next        (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        x_mark      =  '-';
   int         x_index     =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   /*---(check mark)---------------------*/
   x_index = ymark_mark__index  (s_mark_curr);
   --rce;  if (x_index < 0) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YMARK   yLOG_value   ("x_index"   , x_index);
   /*---(find next)----------------------*/
   DEBUG_YMARK   yLOG_note    ("search for next mark");
   for (i = x_index + 1; i < s_nmark; ++i) {
      if (s_mark_info [i].source == MARK_NONE)  continue;
      DEBUG_YMARK   yLOG_value   ("found"     , i);
      x_mark = S_MARK_LIST [i];
      DEBUG_YMARK   yLOG_char    ("x_mark"    , x_mark);
      DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
      return x_mark;
   }
   DEBUG_YMARK   yLOG_note    ("not found");
   --rce;
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
   return rce;
}



/*====================------------------------------------====================*/
/*===----                      location marks                          ----===*/
/*====================------------------------------------====================*/
static void  o___STATUS__________o () { return; }

char*        /*-> tbd --------------------------------[ ------ [ge.420.132.11]*/ /*-[00.0000.114.!]-*/ /*-[--.---.---.--]-*/
ymark_mark__data        (uchar a_mark)
{
   /*---(locals)-----------+-----+-----+-*/
   int         n           =    0;
   /*---(status)-------------------------*/
   n = ymark_mark__index (a_mark);
   if        (n < 0) {
      strlcpy (g_print, " ·  ·  ··········   ·u    ·x     ·y     ·z ´", LEN_RECD);
   } else if (s_mark_info [n].source == MARK_NONE) {
      sprintf (g_print, " %c  ·  ··········   ·u    ·x     ·y     ·z ´", a_mark);
   } else {
      sprintf (g_print, " %c  %c  %-10.10s  %2du  %3dx  %4dy  %4dz ´", a_mark, s_mark_info [n].source, s_mark_info [n].label, s_mark_info [n].u, s_mark_info [n].x, s_mark_info [n].y, s_mark_info [n].z);
   }
   return g_print;
}

char         /*-> tbd --------------------------------[ ------ [ge.420.132.11]*/ /*-[00.0000.114.!]-*/ /*-[--.---.---.--]-*/
ymark_mark_infowin      (char *a_entry, int a_index)
{
   /*---(locals)-----------+-----+-----+-*/
   int         a           =    0;
   int         n           =    0;
   char        t           [LEN_RECD]  = "";
   /*---(status)-------------------------*/
   strlcpy (a_entry, " -  -              -     -     -      -  -              -     -     -         ", LEN_RECD);
   if (a_index <   0)  return 0;
   if (a_index >= 26)  return 0;
   a = 'a' + a_index;
   n = ymark_mark__index (a);
   if (s_mark_info [n].source == MARK_NONE)  sprintf (t, " %c  -              -     -     -     ", a);
   else                                      sprintf (t, " %c  %-10.10s %5d,%5d,%5d     "        , a, s_mark_info [n].label, s_mark_info [n].u, s_mark_info [n].x, s_mark_info [n].y, s_mark_info [n].z);
   strlcpy (a_entry, t, LEN_RECD);
   a = 'A' + a_index;
   n = ymark_mark__index (a);
   if (s_mark_info [n].source == MARK_NONE)  sprintf (t, " %c  -              -     -     -     ", a);
   else                                      sprintf (t, " %c  %-10.10s %5d,%5d,%5d     "        , a, s_mark_info [n].label, s_mark_info [n].x, s_mark_info [n].y, s_mark_info [n].z);
   strlcat (a_entry, t, LEN_RECD);
   /*---(complete)-----------------------*/
   return 0;
}

char
yMARK_mark_status       (char a_size, short a_wide, char *a_list)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_list      [LEN_RECD]  = "";
   int         c           =    0;
   /*---(defenses)-----------------------*/
   --rce;  if (a_list  == NULL)  return rce;
   /*---(status)-------------------------*/
   ymark_mark_listplus   (x_list);
   c = strldcnt (x_list, ':', LEN_RECD);
   snprintf (a_list, 200, "marking  %c  %c  %c  %c  %c  %2d  %s", s_marking, s_mark_head, s_mark_tail, s_mark_curr, s_mark_prev, c, x_list);
   /*---(complete)-----------------------*/
   return 0;
}

char
yMARK_marking           (char *a_label)
{
   char        n           =    0;
   char        c           =  '-';
   if (s_marking == 'y') {
      n = ymark_mark__find   (a_label);
      if (n < 0)  return 0;
      c = S_MARK_LIST [n];
      return c;
   }
   return 0;
}

char         /*-> tbd --------------------------------[ leaf   [ge.540.142.30]*/ /*-[01.0000.103.!]-*/ /*-[--.---.---.--]-*/
ymark_mark_marklist     (char *a_list)
{
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;
   char        rce         = -10;
   char        x_entry     [20];
   /*---(defenses)-----------------------*/
   --rce;  if (a_list  == NULL)  return rce;
   strncpy (a_list, "-", LEN_RECD);   /* special for a null list */
   /*---(walk the list)------------------*/
   strncpy (a_list, ",", LEN_RECD);
   for (i = 0; i < s_nmark; ++i) {
      if (s_mark_info [i].source == MARK_NONE) continue;
      sprintf    (x_entry, "%s,", s_mark_info [i].label);
      strncat    (a_list, x_entry, LEN_RECD);
   }
   /*---(catch empty)--------------------*/
   if (strcmp (a_list, ",") == 0)   strcpy (a_list, ".");
   /*---(complete)-----------------------*/
   return 0;
}

char         /*-> tbd --------------------------------[ leaf   [ge.540.142.30]*/ /*-[01.0000.123.!]-*/ /*-[--.---.---.--]-*/
ymark_mark_listplus     (char *a_list)
{
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;
   char        rce         = -10;
   char        x_entry     [20];
   /*---(defenses)-----------------------*/
   --rce;  if (a_list  == NULL)  return rce;
   strncpy (a_list, "-", LEN_RECD);   /* special for a null list */
   /*---(walk the list)------------------*/
   strncpy (a_list, ",", LEN_RECD);
   for (i = 1; i < s_nmark; ++i) {
      if (s_mark_info [i].source == MARK_NONE) continue;
      sprintf    (x_entry, "%c:%s,", S_MARK_LIST [i], s_mark_info [i].label);
      strncat    (a_list, x_entry, LEN_RECD);
   }
   /*---(catch empty)--------------------*/
   if (strcmp (a_list, ",") == 0)   strcpy (a_list, ".");
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     file input and output                    ----===*/
/*====================------------------------------------====================*/
static void  o___FILE____________o () { return; }

char         /*-> tbd --------------------------------[ ------ [ge.732.124.21]*/ /*-[02.0000.01#.#]-*/ /*-[--.---.---.--]-*/
ymark_mark_writer       (int c, uchar a_abbr)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        n           =    0;
   char        x_cmd       [LEN_RECD ];
   /*---(header)-------------------------*/
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   /*---(clear output)-------------------*/
   yPARSE_outclear  ();
   /*---(defense)------------------------*/
   DEBUG_YMARK   yLOG_char    ("a_abbr"    , a_abbr);
   n  = ymark_mark__index (a_abbr);
   DEBUG_YMARK   yLOG_char    ("index"     , n);
   --rce; if (n  < 0) { 
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(label)--------------------------*/
   DEBUG_YMARK   yLOG_char    ("label"     , s_mark_info [n].label);
   if (strcmp (s_mark_info [n].label, "-") == 0) {
      DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(write)-----------------------*/
   yPARSE_vprintf (c, "loc_mark", a_abbr, s_mark_info [n].label);
   /*---(complete)-----------------------*/
   DEBUG_YMARK  yLOG_exit    (__FUNCTION__);
   return 1;
}

char
yMARK_mark_writer_all   (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_end       =    0;
   int         i           =    0;
   char        c           =    0;
   /*---(prepare)------------------------*/
   x_end = strlen (S_MARK_LIST);
   /*> yPARSE_verb_begin ("loc_mark");                                                <*/
   /*---(walk list)----------------------*/
   for (i = 0; i <= x_end; ++i) {
      rc = ymark_mark_writer   (c, S_MARK_LIST [i]);
      if (rc < 1)  continue;
      ++c;
      /*> yPARSE_verb_break (c);                                                      <*/
   }
   /*---(wrap-up)------------------------*/
   /*> yPARSE_verb_end   (c);                                                         <*/
   /*---(complete)-----------------------*/
   return c;
}

char
yMARK_mark_reader       (int n, char *a_verb)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -11;
   char        rc          =    0;
   char        x_abbr      =    0;
   int         c           =    0;
   char        x_label     [LEN_LABEL];
   /*---(header)-------------------------*/
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   DEBUG_YMARK   yLOG_value   ("n"         , n);
   /*---(get verb)-----------------------*/
   DEBUG_YMARK   yLOG_info    ("a_verb"    , a_verb);
   --rce;  if (strcmp ("loc_mark", a_verb) != 0) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(label)--------------------------*/
   rc = yPARSE_popchar (&x_abbr);
   DEBUG_YMARK   yLOG_value   ("pop abbr"  , rc);
   DEBUG_YMARK   yLOG_char    ("abbr"      , x_abbr);
   c  = ymark_mark__index (x_abbr);
   DEBUG_YMARK   yLOG_char    ("c"         , c);
   --rce; if (c < 0) { 
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get command)--------------------*/
   rc = yPARSE_popstr (x_label);
   DEBUG_YMARK   yLOG_value   ("pop verb"  , rc);
   DEBUG_YMARK   yLOG_info    ("x_label"   , x_label);
   /*---(save)---------------------------*/
   DEBUG_YMARK   yLOG_note    ("saving values");
   strlcpy (s_mark_info [c].label, x_label, LEN_LABEL);
   rc = yVIHUB_yMAP_locator (s_mark_info [c].label, &s_mark_info [c].u, &s_mark_info [c].x, &s_mark_info [c].y, &s_mark_info [c].z);
   --rce;  if (rc < 0) {
      ymark_mark__unset   (x_abbr);
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   s_mark_info [c].source = MARK_IMPORT;
   /*---(update range)-------------------*/
   ymark_mark__range   ();
   /*---(complete)-----------------------*/
   DEBUG_YMARK  yLOG_exit    (__FUNCTION__);
   return 1;
}



/*====================------------------------------------====================*/
/*===----                      mark command line                       ----===*/
/*====================------------------------------------====================*/
static void  o___COMMAND_________o () { return; }

char         /*-> enter a mark directly --------------[ ------ [ge.D54.139.83]*/ /*-[02.0000.00#.O]-*/ /*-[--.---.---.--]-*/
ymark_mark_direct       (char *a_string)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_src       =  '·';
   char        x_div       =  '·';
   char        x_dst       =  '·';
   int         x_len       =    0;
   int         x_index     =   -1;
   char        x_label     [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   DEBUG_YMARK   yLOG_point   ("a_string"  , a_string);
   /*---(defense)------------------------*/
   --rce;  if (a_string == NULL) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YMARK   yLOG_info    ("a_string"  , a_string);
   /*---(check for purge)----------------*/
   if (strcmp ("purge", a_string) == 0) {
      ymark_mark__purge (YSTR_IFULL);
      DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(prepare)------------------------*/
   x_len = strlen (a_string);
   DEBUG_YMARK   yLOG_value   ("x_len"     , x_len);
   --rce;  if (x_len <  1) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_src = a_string [0];
   x_div = a_string [1];
   x_dst = a_string [2];
   /*---(check for singles)--------------*/
   --rce;  if (x_len == 1) {
      DEBUG_YMARK   yLOG_note    ("one character option");
      switch (x_src) {
      case '_' :
         DEBUG_YMARK   yLOG_note    ("show status");
         break;
      case '!' :
         DEBUG_YMARK   yLOG_note    ("display marks");
         break;
      case '%' :
         DEBUG_YMARK   yLOG_note    ("hide marks");
         break;
      case '?' :
         DEBUG_YMARK   yLOG_note    ("information window");
         break;
      default  :
         DEBUG_YMARK   yLOG_note    ("unknown action");
         DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
         break;
      }
      DEBUG_YMARK   yLOG_value   ("rc"        , rc);
      DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(check for doubles)--------------*/
   --rce;  if (x_len == 2) {
      DEBUG_YMARK   yLOG_note    ("two character option");
      switch (x_div) {
      case 'x' : case 'd' : case '=' :
         DEBUG_YMARK   yLOG_note    ("unset a mark");
         rc = ymark_mark__unset   (x_src);
         break;
      case '~' :
         DEBUG_YMARK   yLOG_note    ("copy to unnammed sreg");
         x_index = ymark_mark__index (x_src);
         DEBUG_YMARK   yLOG_value   ("x_index"   , x_index);
         --rce;  if (x_index < 0) {
            DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
         DEBUG_YMARK   yLOG_char    ("a_mark"    , x_src);
         rc = ySRC_push  ('"', s_mark_info [x_index].label);
         break;
      default  :
         DEBUG_YMARK   yLOG_note    ("unknown action");
         DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
         break;
      }
      DEBUG_YMARK   yLOG_value   ("rc"        , rc);
      DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(check for triples)--------------*/
   --rce;  if (x_len == 3) {
      DEBUG_YMARK   yLOG_note    ("three character option");
      switch (x_div) {
      case '>' :
         DEBUG_YMARK   yLOG_note    ("called copy");
         rc = ymark_mark__copy  (x_src, x_dst);
         break;
      case ']' :
         DEBUG_YMARK   yLOG_note    ("called move");
         rc = ymark_mark__copy  (x_src, x_dst);
         rc = ymark_mark__unset (x_src);
         break;
      case ')' :
         DEBUG_YMARK   yLOG_note    ("called swap");
         rc = ymark_mark__copy  (x_dst, '¤');
         rc = ymark_mark__copy  (x_src, x_dst);
         rc = ymark_mark__copy  ('¤'  , x_src);
         break;
      case '~' :
         DEBUG_YMARK   yLOG_note    ("copy to unnammed sreg");
         x_index = ymark_mark__index (x_src);
         DEBUG_YMARK   yLOG_value   ("x_index"   , x_index);
         --rce;  if (x_index < 0) {
            DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
         DEBUG_YMARK   yLOG_char    ("a_mark"    , x_src);
         rc = ySRC_push  (x_dst, s_mark_info [x_index].label);
         break;
      default  :
         DEBUG_YMARK   yLOG_note    ("unknown action");
         DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
         break;
      }
      DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(create a mark)------------------*/
   if  (x_div == '=') {
      DEBUG_YMARK   yLOG_note    ("creating a mark");
      /*---(check mark)---------------------*/
      x_index = ymark_mark__index (x_src);
      DEBUG_YMARK   yLOG_value   ("x_index"   , x_index);
      --rce;  if (x_index < 0) {
         DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_YMARK   yLOG_char    ("a_mark"    , x_src);
      /*---(save)---------------------------*/
      strlcpy (s_mark_info [x_index].label, a_string + 2, LEN_LABEL);
      rc = yVIHUB_yMAP_locator   (s_mark_info [x_index].label, &s_mark_info [x_index].u, &s_mark_info [x_index].x, &s_mark_info [x_index].y, &s_mark_info [x_index].z);
      --rce;  if (rc < 0) {
         ymark_mark__unset   (x_src);
         DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      rc = yVIHUB_yMAP_addresser (s_mark_info [x_index].label, s_mark_info [x_index].u, s_mark_info [x_index].x, s_mark_info [x_index].y, s_mark_info [x_index].z);
      s_mark_info [x_index].source = MARK_IMPORT;
   }
   /*---(go there)-----------------------*/
   /*> yVIHUB_yMAP_jump (s_mark_info [x_index].u, s_mark_info [x_index].x, s_mark_info [x_index].y, s_mark_info [x_index].z);   <*/
   /*---(set history)--------------------*/
   /*> DEBUG_YMARK   yLOG_note    ("set history");                                    <*/
   /*> ymark_mark__history (x_src);                                                   <*/
   /*---(update range)-------------------*/
   DEBUG_YMARK   yLOG_note    ("update the range");
   ymark_mark__range   ();
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         mode handling                        ----===*/
/*====================------------------------------------====================*/
static void  o___MODE____________o () { return; }

char         /*-> process keys for marks -------------[ leaf   [ge.UD8.24#.JA]*/ /*-[03.0000.102.E]-*/ /*-[--.---.---.--]-*/
ymark_mark_smode        (uchar a_major, uchar a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char       *x_majors    = "m'";
   /*---(header)-------------------------*/
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   DEBUG_YMARK   yLOG_char    ("a_major"   , a_major);
   DEBUG_YMARK   yLOG_char    ("a_minor"   , chrvisible (a_minor));
   /*> myVIKEYS.info_win = '-';                                                       <*/
   /*---(defenses)-----------------------*/
   DEBUG_YMARK   yLOG_char    ("mode"      , yMODE_curr ());
   --rce;  if (yMODE_not (UMOD_MARK   )) {
      DEBUG_YMARK   yLOG_note    ("not the correct mode");
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(exit mode)----------------------*/
   if (a_minor == G_KEY_ESCAPE || a_minor == G_KEY_RETURN) {
      DEBUG_YMARK   yLOG_note    ("escape/return means leave");
      yMODE_exit ();
      DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
      return  0;
   }
   /*---(major check)--------------------*/
   DEBUG_YMARK   yLOG_info    ("x_majors"  , x_majors);
   --rce;  if (strchr (x_majors, a_major) == NULL) {
      DEBUG_YMARK   yLOG_note    ("major not valid");
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(mode change)--------------------*/
   if (a_minor == '*') {
      DEBUG_YMARK   yLOG_note    ("enter visual mode from < to >");
      yMODE_exit ();
      ymark_mark__return    ('<');
      yMODE_enter  (UMOD_VISUAL);
      /*> rc = VISU_mark    ();                                                       <*/
      /*> if (rc < 0) {                                                               <* 
       *>    yMODE_exit ();                                                           <* 
       *>    DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                          <* 
       *>    return rce;                                                              <* 
       *> }                                                                           <*/
      ymark_mark__return    ('>');
      DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(common quick)-------------------*/
   --rce;  if (strchr("_!%#+-?", a_minor) != NULL) {
      switch (a_minor) {
      case '#' :
         DEBUG_YMARK   yLOG_note    ("unset mark under cursor");
         rc = ymark_mark__which   ();
         if (rc < 0) {
            yMODE_exit ();
            DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
         ymark_mark__unset   (rc);
         break;
      case '!' :
         DEBUG_YMARK   yLOG_note    ("turn marks visible");
         s_marking = 'y';
         break;
      case '%' :
         DEBUG_YMARK   yLOG_note    ("turn marks hidden");
         s_marking = '-';
         break;
      case '_' :
         DEBUG_YMARK   yLOG_note    ("use mark status bar");
         yVIHUB_yCMD_direct (":status mark");
         break;
      case '?' :
         DEBUG_YMARK   yLOG_note    ("show mark info window");
         /*> myVIKEYS.info_win = 'y';                                                    <*/
         /*> DEBUG_YMARK   yLOG_exit    (__FUNCTION__);                               <*/
         /*> return a_major;                                                          <*/
         break;
      }
      yMODE_exit ();
      DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
      return  0;
   }
   /*---(check for setting)--------------*/
   --rce;  if (a_major == 'm') {
      DEBUG_YMARK   yLOG_note    ("handling mark (m)");
      rc = ymark_mark__set   (a_minor);
      DEBUG_YMARK   yLOG_value   ("rc"        , rc);
      if (rc < 0) {
         yMODE_exit ();
         DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      yMODE_exit ();
      DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(check for returning)------------*/
   --rce;  if (a_major == '\'') {
      DEBUG_YMARK   yLOG_note    ("handling return (')");
      rc = ymark_mark__return   (a_minor);
      DEBUG_YMARK   yLOG_value   ("rc"        , rc);
      if (rc < 0)  {
         yMODE_exit ();
         DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      yMODE_exit ();
      DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(failure)------------------------*/
   --rce;
   yMODE_exit ();
   DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
   return rce;
}

