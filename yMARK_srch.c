/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "yMARK.h"
#include    "yMARK_priv.h"

/*
 * metis ß dc4#∑ ß get very simple unit test for regex searching in place                 ß M255NH ß  8 ß
 * metis ß wc4∑∑ ß add regex searching to metis to test interface                         ß M255Pp ß  ∑ ß
 * metis ß wc4∑∑ ß replace regex searching in gyges to test interface                     ß M255Pq ß  ∑ ß
 *
 *
 *
 *
 */

/*
 *
 *  highlighting/selecting an area or range, restricts search to that space
 *
 *  simple reuse (0-9)
 *     //0     repeats last search
 *     //2     repeats n - 2 searches ago
 *
 *  marking ¥ repeated searches can be marked a-z for shorthand reuse
 *     //ma  marks last search as 'a'
 *     marked searches are saved in the file with the data for later use
 *
 *  reuse of marked
 *     //a     repeats search marked 'a'
 *     //A     brings 'a' up for editing rather than immediate execution
 *
 *  search command
 *     :search a>b   copy a to b
 *     :search a]b   move a to b, clearing a
 *     :search a)b   swap a and b
 *     :search a=∑∑∑ force regex directly into marked/saved
 *     :search a+    accept a from clip file
 *     :search a-    save a to clip file
 *     :search a#    clear
 *
 *  naming  ¥ common searches can be saved to global library with short name
 *     these searches can also be loaded into Ë to ˇ to keep them separate
 * 
 *
 *
 *  multi-searching use/need marked searches, should be separately tested first
 *
 *  //a          repeats search 'a'
 *  //!a         not
 *
 *  //a–b   µ&   and    (TT only)
 *  //a—b   µ|   or     (TT, TF, and FT)  all but FF
 *
 *  //a‘b   µk   nand   (TF, FT, and FF)  all but TT
 *  //a’b   µj   nor    (FF only)
 *
 *  //a“b   µv   xor    (TF and FT)       one or the other
 *  //a”b   µi   nxor   (TT or FF only)   both same
 *
 *
 *
 *
 *
 *
 */



#define        S_SRCH_MAX    100
static uchar   S_SRCH_LIST   [S_SRCH_MAX];
static uchar   s_nlist         = 0;
static uchar   s_search  [LEN_LABEL]  = "-";
static tSRCH  *G_SRCHS       [S_SRCH_MAX];




/*====================------------------------------------====================*/
/*===----                       program level                         ----===*/
/*====================------------------------------------====================*/
static void  o___PROGRAM_________o () { return; }

char
ymark_srch_init         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_SRCH   yLOG_enter   (__FUNCTION__);
   /*---(pointers)-----------------------*/
   DEBUG_SRCH   yLOG_note    ("ground external pointers");
   myMARK.e_regex    = NULL;
   myMARK.e_unregex  = NULL;
   /*---(search btree)-------------------*/
   rc = ySORT_btree (B_SRCH, "searches");
   DEBUG_SRCH   yLOG_value   ("btree"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(search abbr list)---------------*/
   strlcpy (S_SRCH_LIST, YSTR_LOWER , S_SRCH_MAX);
   strlcat (S_SRCH_LIST, YSTR_GREEK , S_SRCH_MAX);
   DEBUG_SRCH   yLOG_info    ("LIST"      , S_SRCH_LIST);
   s_nlist  = strlen (S_SRCH_LIST);
   DEBUG_SRCH   yLOG_value   ("s_nlist"   , s_nlist);
   /*---(mode)---------------------------*/
   rc = yMODE_init_set   (MODE_SEARCH, NULL, ySRC_mode);
   DEBUG_SRCH   yLOG_value   ("mode set"  , rc);
   /*---(purge)--------------------------*/
   rc = ymark_srch_purge ();
   DEBUG_SRCH   yLOG_value   ("purge"     , rc);
   /*---(complete)-----------------------*/
   DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ymark_srch_purge        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tSRCH      *x_curr      = NULL;
   tSRCH      *x_next      = NULL;
   /*---(header)-------------------------*/
   DEBUG_SRCH   yLOG_enter   (__FUNCTION__);
   /*---(walk-through)-------------------*/
   DEBUG_SRCH   yLOG_value   ("count"     , ySORT_count (B_SRCH));
   rc = ySORT_by_cursor (B_SRCH, YDLST_HEAD, &x_curr);
   DEBUG_PROG   yLOG_point   ("x_curr"     , x_curr);
   while (x_curr != NULL) {
      rc = ySORT_by_cursor (B_SRCH, YDLST_NEXT, &x_next);
      DEBUG_PROG   yLOG_point   ("x_next"     , x_next);
      DEBUG_SRCH   yLOG_point   ("x_curr"     , x_curr);
      rc = ymark_srch_free (&x_curr);
      x_curr = x_next;
   }
   /*---(check)--------------------------*/
   DEBUG_SRCH   yLOG_value   ("count"     , ySORT_count (B_SRCH));
   --rce;  if (ySORT_count (B_SRCH) > 0) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ymark_srch_wrap         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_SRCH   yLOG_enter   (__FUNCTION__);
   /*---(purge)--------------------------*/
   rc = ymark_srch_purge ();
   DEBUG_SRCH   yLOG_value   ("purge"     , rc);
   /*---(pointers)-----------------------*/
   DEBUG_SRCH   yLOG_note    ("ground external pointers");
   myMARK.e_regex    = NULL;
   myMARK.e_unregex  = NULL;
   /*---(complete)-----------------------*/
   DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                    memory allocation                        ----===*/
/*====================------------------------------------====================*/
static void  o___MEMORY__________o () { return; }

char
ymark_srch_new          (uchar *a_text, char a_force, tSRCH **r_new)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tSRCH      *x_new       = NULL;
   int         x_tries     =    0;
   /*---(header)-------------------------*/
   DEBUG_SRCH   yLOG_enter   (__FUNCTION__);
   /*---(check return)-------------------*/
   DEBUG_SRCH   yLOG_point   ("r_new"     , r_new);
   --rce;  if (r_new == NULL) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SRCH   yLOG_point   ("*r_new"    , *r_new);
   --rce;  if (a_force != 'y' && *r_new != NULL) {
      DEBUG_SRCH   yLOG_note    ("pointer is already set, not in force mode");
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SRCH   yLOG_point   ("a_text"    , a_text);
   --rce;  if (a_text == NULL) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SRCH   yLOG_info    ("a_text"    , a_text);
   /*---(existing)-----------------------*/
   ymark_srch_by_text (a_text, r_new);
   DEBUG_SRCH   yLOG_point   ("*r_new"    , *r_new);
   if (*r_new != NULL) {
      DEBUG_SRCH   yLOG_note    ("already found, increasing counter");
      /* update date for ordering */
      DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
      return 1;
   }
   /*---(allocate)-----------------------*/
   while (x_new == NULL) {
      ++x_tries;
      x_new = (tSRCH *) malloc (sizeof (tSRCH));
      if (x_tries > 3)   break;
   }
   DEBUG_SRCH   yLOG_value   ("x_tries"   , x_tries);
   DEBUG_SRCH   yLOG_point   ("x_new"     , x_new);
   --rce;  if (x_new == NULL) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(wipe)---------------------------*/
   strlcpy (x_new->text, a_text, LEN_LABEL);
   x_new->runs = x_new->found = 0;
   /*---(add timestamp)------------------*/
   str2mongo (time (NULL), x_new->runtime);
   /*---(into btree)---------------------*/
   rc = ySORT_hook (B_SRCH, x_new, x_new->runtime, &x_new->btree);
   DEBUG_SRCH   yLOG_value   ("btree"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update)-------------------------*/
   rc = ySORT_prepare (B_SRCH);
   if (rc < 0) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save return)--------------------*/
   *r_new = x_new;
   /*---(complete)-----------------------*/
   DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ymark_srch_free         (tSRCH **r_old)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_SRCH   yLOG_enter   (__FUNCTION__);
   /*---(check return)-------------------*/
   DEBUG_SRCH   yLOG_point   ("r_old"     , r_old);
   --rce;  if (r_old == NULL) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SRCH   yLOG_point   ("*r_old"    , *r_old);
   --rce;  if (*r_old == NULL) {
      DEBUG_SRCH   yLOG_note    ("pointer was never set");
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(unhook from btree)--------------*/
   rc = ySORT_unhook (&(*r_old)->btree);
   DEBUG_SRCH   yLOG_value   ("btree"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update)-------------------------*/
   rc = ySORT_prepare (B_SRCH);
   if (rc < 0) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(clear and return)---------------*/
   free (*r_old);
   *r_old = NULL;
   /*---(complete)-----------------------*/
   DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        searching                             ----===*/
/*====================------------------------------------====================*/
static void  o___SEARCH__________o () { return; }

int  ymark_srch_count         (void)                          { return ySORT_count     (B_SRCH); }
char ymark_srch_by_index      (int n, tSRCH **r_find)         { return ySORT_by_index  (B_SRCH, n, r_find); }
char ymark_srch_by_cursor     (uchar a_dir, tSRCH **r_find)   { return ySORT_by_cursor (B_SRCH, a_dir, r_find); }

char
ymark_srch_by_text      (uchar *a_text, tSRCH **r_find)
{
   return 0;
}


/*====================------------------------------------====================*/
/*===----                        searching                             ----===*/
/*====================------------------------------------====================*/
static void  o___EXECUTE_________o () { return; }

char         /*-> tbd --------------------------------[ ------ [ge.#M5.1C#.#7]*/ /*-[03.0000.013.L]-*/ /*-[--.---.---.--]-*/
yMARK_execute         (uchar *a_search)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_rc        =    0;
   /*---(header)-------------------------*/
   DEBUG_SRCH   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (!yMODE_operational (MODE_SEARCH)) {
      DEBUG_SRCH   yLOG_note    ("can not execute until operational");
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(defense)-----------------------*/
   DEBUG_SRCH   yLOG_point   ("a_search"  , a_search);
   --rce;  if (a_search == NULL) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rc;
   }
   DEBUG_SRCH   yLOG_info    ("a_search"  , a_search);
   if (strcmp (a_search, "/") == 0) {
      DEBUG_SRCH   yLOG_note    ("requested a purge of finds");
      ymark_srch_purge ();
      DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(handle)------------------------*/
   DEBUG_SRCH   yLOG_point   ("e_regex"   , myMARK.e_regex);
   DEBUG_SRCH   yLOG_point   ("unit"      , ymark__unit_regex);
   --rce;  if (myMARK.e_regex == NULL) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = myMARK.e_regex (a_search);
   DEBUG_SRCH   yLOG_value   ("rc"        , rc);
   /*---(position to first)--------------*/
   /*> strlcpy (s_search, a_search, LEN_LABEL);                                       <*/
   /*---(position to first)--------------*/
   /*> if (s_nsrch > 0)  yvikeys_srch_by_cursor (YDLST_DHEAD, NULL);                  <*/
   /*---(complete)-----------------------*/
   DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                        data movement                         ----===*/
/*====================------------------------------------====================*/
static void  o___DATA____________o () { return; }

char 
ymark_srch_index        (uchar a_abbr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =   -1;
   /*---(header)-------------------------*/
   DEBUG_SRCH   yLOG_senter  (__FUNCTION__);
   /*---(check)--------------------------*/
   DEBUG_SRCH   yLOG_schar   (a_abbr);
   --rce;  if (a_abbr == 0) {
      DEBUG_SRCH   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SRCH   yLOG_snote   ("check");
   n  = strchr (S_SRCH_LIST, a_abbr);
   DEBUG_SRCH   yLOG_sint    (n);
   --rce;  if (n  < 0) {
      DEBUG_SRCH   yLOG_sexitr  (__FUNCTION__, n);
      return n;
   }
   /*---(complete)-----------------------*/
   DEBUG_SRCH   yLOG_sexit   (__FUNCTION__);
   return n;
}

char
ymark_srch_clear        (uchar a_abbr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =    0;
   /*---(header)-------------------------*/
   DEBUG_SRCH   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_SRCH   yLOG_char    ("a_abbr"    , a_abbr);
   n = ymark_srch_index  (a_abbr);
   DEBUG_SRCH   yLOG_value   ("n"         , n);
   --rce;  if (n < 0) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(clear)--------------------------*/
   /*> strlcpy (G_SEARCHES [n], "", LEN_RECD);                                        <*/
   /*> ysrc_sreg__wipeall ('-', &g_sregs [n]);                                        <*/
   /*---(complete)-----------------------*/
   DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> tbd --------------------------------[ ------ [ge.#M5.1C#.#7]*/ /*-[03.0000.013.L]-*/ /*-[--.---.---.--]-*/
ymark_srch_direct       (uchar *a_text)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_len       =    0;
   char        x_abbr      =  '-';
   char        x_div       =  '-';
   uchar      *x_divs      = "#-+>])=";
   /*---(header)-------------------------*/
   DEBUG_SRCH   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (!yMODE_operational (MODE_SEARCH)) {
      DEBUG_SRCH   yLOG_note    ("can not execute until operational");
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(defense)-----------------------*/
   DEBUG_SRCH   yLOG_point   ("a_text"    , a_text);
   --rce;  if (a_text == NULL) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rc;
   }
   /*---(parse)-------------------------*/
   x_len = strlen (a_text);
   DEBUG_SRCH   yLOG_value   ("x_len"     , x_len);
   --rce;  if (x_len <= 1) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_abbr  = a_text [0];
   DEBUG_SRCH   yLOG_char    ("x_abbr"      , x_abbr);
   x_div   = a_text [1];
   DEBUG_SRCH   yLOG_char    ("x_div"     , x_div);
   /*---(check dividers)-----------------*/
   DEBUG_SRCH   yLOG_info    ("x_divs"    , x_divs);
   --rce;  if (strchr (x_divs, x_div) == NULL)  {
      DEBUG_SRCH   yLOG_note    ("divider (x_div) not understood");
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check for shorts)---------------*/
   --rce;  if (x_len == 2) {
      switch (x_div) {
      case '#' : rc = ysrc_sreg_clear       (x_abbr);        break;
      case '-' : rc = ysrc_sreg__export     (x_abbr);        break;
      case '+' : rc = ysrc_sreg__import     (x_abbr);        break;
      default  :
                 DEBUG_SRCH   yLOG_note    ("divider (x_div) not a 2-char version");
                 DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
                 return rce;
      }
      DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
      return rc;
   }





   if (strcmp (a_text, "/") == 0) {
      DEBUG_SRCH   yLOG_note    ("requested a purge of finds");
      ymark_srch_purge ();
      DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(handle)------------------------*/
   /*---(complete)-----------------------*/
   DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
   return rc;
}



