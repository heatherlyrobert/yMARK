/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "yMARK.h"
#include    "yMARK_priv.h"

/*
 * metis § dc4#· § get very simple unit test for regex searching in place                 § M255NH §  8 §
 * metis § wc4·· § add regex searching to metis to test interface                         § M255Pp §  · §
 * metis § wc4·· § replace regex searching in gyges to test interface                     § M255Pq §  · §
 * metis § tn4#· § build unit test for simple execute interface                           § M2F52k §  1 §
 * metis § tn4<· § build unit test for direct/command interface                           § M2F53Y §  · §
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
 *  marking ´ repeated searches can be marked a-z for shorthand reuse
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
 *     :search a=··· force regex directly into marked/saved
 *     :search a+    accept a from clip file
 *     :search a-    save a to clip file
 *     :search a#    clear
 *
 *  naming  ´ common searches can be saved to global library with short name
 *     these searches can also be loaded into è to ÿ to keep them separate
 * 
 *
 *
 *  multi-searching use/need marked searches, should be separately tested first
 *
 *  //a          repeats search 'a'
 *  //!a         not
 *
 *  //aÐb   µ&   and    (TT only)
 *  //aÑb   µ|   or     (TT, TF, and FT)  all but FF
 *
 *  //aÔb   µk   nand   (TF, FT, and FF)  all but TT
 *  //aÕb   µj   nor    (FF only)
 *
 *  //aÒb   µv   xor    (TF and FT)       one or the other
 *  //aÓb   µi   nxor   (TT or FF only)   both same
 *
 *
 *
 *
 *
 *
 */



#define        S_SRCH_MAX    100
static uchar   S_SRCH_LIST   [S_SRCH_MAX];
static uchar   s_nlist       = 0;
static tSRCH  *S_SRCHS       [S_SRCH_MAX];
static int     s_seq         = 0;
static uchar   s_layers      = 0;




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
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_SRCH   yLOG_enter   (__FUNCTION__);
   /*---(pointers)-----------------------*/
   DEBUG_SRCH   yLOG_note    ("ground external pointers");
   myMARK.e_regex    = NULL;
   myMARK.e_unfind   = NULL;
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
   strlcat (S_SRCH_LIST, "."        , S_SRCH_MAX);  /* temp spot */
   DEBUG_SRCH   yLOG_info    ("LIST"      , S_SRCH_LIST);
   s_nlist  = strlen (S_SRCH_LIST);
   DEBUG_SRCH   yLOG_value   ("s_nlist"   , s_nlist);
   s_seq    = 0;
   s_layers = 0;
   /*---(purge marks)--------------------*/
   for (i = 0; i < s_nlist; ++i)  S_SRCHS [i] = NULL;
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
   /*---(globals)------------------------*/
   s_layers = 0;
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
   myMARK.e_unfind   = NULL;
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
   --rce;  if (a_text == NULL || strlen (a_text) <= 0) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SRCH   yLOG_info    ("a_text"    , a_text);
   /*---(existing)-----------------------*/
   ymark_srch_by_text (a_text, r_new);
   DEBUG_SRCH   yLOG_point   ("*r_new"    , *r_new);
   --rce;  if (*r_new != NULL) {
      DEBUG_SRCH   yLOG_note    ("already found, increasing counter");
      str2mongo (time (NULL), (*r_new)->runtime);
      strlpadn  (++s_seq, (*r_new)->seq, '.', '>', 6);
      rc = ySORT_prepare (B_SRCH);
      if (rc < 0) {
         DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
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
   x_new->text = strdup (a_text);
   x_new->runs = x_new->found = 0;
   /*---(add timestamp)------------------*/
   str2mongo (time (NULL), x_new->runtime);
   strlpadn  (++s_seq, x_new->seq, '.', '>', 6);
   /*---(into btree)---------------------*/
   rc = ySORT_hook (B_SRCH, x_new, x_new->seq, &x_new->btree);
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
   free ((*r_old)->text);
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
char ymark_srch_by_index      (int n, tSRCH **r_srch)         { return ySORT_by_index  (B_SRCH, n, r_srch); }
char ymark_srch_by_cursor     (uchar a_dir, tSRCH **r_srch)   { return ySORT_by_cursor (B_SRCH, a_dir, r_srch); }

char
ymark_srch_by_text      (uchar *a_text, tSRCH **r_srch)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =    0;
   char        rce         =  -10;
   tSRCH      *x_curr      = NULL;
   char        x_entry     [LEN_LABEL];
   /*---(header)-------------------------*/
   DEBUG_SRCH   yLOG_enter   (__FUNCTION__);
   /*---(check return)-------------------*/
   DEBUG_SRCH   yLOG_point   ("r_srch"    , r_srch);
   --rce;  if (r_srch == NULL) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   *r_srch = NULL;
   DEBUG_SRCH   yLOG_point   ("a_text"    , a_text);
   --rce;  if (a_text == NULL) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SRCH   yLOG_info    ("a_text"    , a_text);
   /*---(walk the list)------------------*/
   ymark_srch_by_cursor (YDLST_HEAD, &x_curr);
   while (x_curr != NULL) {
      if (strcmp (x_curr->text, a_text) == 0) {
         DEBUG_SRCH   yLOG_note    ("found the match");
         *r_srch = x_curr;
         DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
         return 1;
      }
      ymark_srch_by_cursor (YDLST_NEXT, &x_curr);
   }
   /*---(complete)-----------------------*/
   DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
   return 0;
}

char*
ymark_srch_list         (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =    0;
   char        rce         =  -10;
   tSRCH      *x_curr      = NULL;
   char        x_entry     [LEN_LABEL];
   /*---(walk the list)------------------*/
   strlcpy (g_print, ",", LEN_RECD);
   ymark_srch_by_cursor (YDLST_HEAD, &x_curr);
   while (x_curr != NULL) {
      if (i > 10)          break;
      if (x_curr->text != NULL)    sprintf  (x_entry, "%s,", x_curr->text);
      else                         strlcpy  (x_entry, "??", LEN_LABEL);
      strlcat    (g_print, x_entry, LEN_RECD);
      ymark_srch_by_cursor (YDLST_NEXT, &x_curr);
   }
   /*---(catch empty)--------------------*/
   if (strcmp (g_print, ",") == 0)   strcpy (g_print, ".");
   /*---(complete)-----------------------*/
   return g_print;
}

char*
ymark_srch__entry       (int n, uchar a_abbr, tSRCH *a_srch)
{
   char        r           [LEN_HUND]  = "";
   char        s           [LEN_TERSE] = "  ·";
   char        t           [LEN_TERSE] = "  ·";
   DEBUG_SRCH   yLOG_enter   (__FUNCTION__);
   DEBUG_SRCH   yLOG_value   ("n"         , n);
   DEBUG_SRCH   yLOG_char    ("a_abbr"    , a_abbr);
   DEBUG_SRCH   yLOG_point   ("a_srch"    , a_srch);
   if (a_srch == NULL) {
      DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
      return "n/a";
   }
   if (a_srch->text != NULL)  sprintf (r, "%2då%.40sæ", strlen (a_srch->text), a_srch->text);
   if (a_srch->found > 0)     sprintf (s, "%3d", a_srch->found);
   if (a_srch->runs  > 0)     sprintf (t, "%3d", a_srch->runs);
   if (a_abbr == 0)  sprintf (g_print, "%-2d %-6.6s %-6.6s %-40.40s %-3.3s %-3.3s", n, a_srch->seq, a_srch->runtime, r, s, t);
   else              sprintf (g_print, "%c  %-6.6s %-6.6s %-40.40s %-3.3s %-3.3s", a_abbr, a_srch->seq, a_srch->runtime, r, s, t);
   DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
   return g_print;
}

char*
ymark_srch_entry        (int n)
{
   tSRCH      *x_srch      = NULL;
   ymark_srch_by_index (n, &x_srch);
   return ymark_srch__entry (n, 0, x_srch);
}

char*
ymark_srch_marked       (uchar a_abbr)
{
   char        n           =    0;
   n = ymark_srch_index (a_abbr);
   if (n < 0)  return "n/a";
   return ymark_srch__entry (n, a_abbr, S_SRCHS [n]);
}

char*
ymark_srch_marks        (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =    0;
   char        x_entry     [LEN_LABEL];
   strlcpy (g_print, ",", LEN_RECD);
   for (i = 0; i < s_nlist - 1; ++i) {
      if (S_SRCHS [i] != NULL) {
         sprintf  (x_entry, "%c,", S_SRCH_LIST [i]);
         strlcat    (g_print, x_entry, LEN_RECD);
      }
   }
   /*---(catch empty)--------------------*/
   if (strcmp (g_print, ",") == 0)   strcpy (g_print, ".");
   /*---(complete)-----------------------*/
   return g_print;
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
   int         x_len       =    0;
   char        x_rc        =    0;
   tSRCH      *x_new       = NULL;
   tFIND      *x_find      = NULL;
   uchar       x_join      =  '-';
   char        x_search    [LEN_RECD]  = "";
   int         n           =    0;
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
      return rce;
   }
   DEBUG_SRCH   yLOG_info    ("a_search"  , a_search);
   x_len = strlen (a_search);
   if (a_search [0] != '/') {
      DEBUG_SRCH   yLOG_note    ("must start with a forward slash");
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(mark request)------------------*/
   --rce;  if (x_len == 4 && strncmp ("//m", a_search, 3) == 0) {
      DEBUG_SRCH   yLOG_note    ("request to mark the last search");
      DEBUG_SRCH   yLOG_char    ("mark"      , a_search [3]);
      rc = ymark_srch_mark (a_search [3]);
      DEBUG_SRCH   yLOG_value   ("marking"   , rc);
      if (rc < 0) {
         DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(purge request)-----------------*/
   x_join = a_search [1];
   DEBUG_SRCH   yLOG_char    ("x_join"    , x_join);
   --rce;  if (x_len == 1 || strchr (YSTR_CJOIN, x_join) == NULL) {
      ymark_find_purge ();
      if (x_len == 1) {
         DEBUG_SRCH   yLOG_note    ("requested a specific purge of finds");
         DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
         return 0;
      }
      DEBUG_SRCH   yLOG_note    ("not a joining search, so purge first");
      strlcpy (x_search, a_search, LEN_RECD);
   } else {
      DEBUG_SRCH   yLOG_char    ("x_join"    , x_join);
      if (strchr (YSTR_SJOIN, x_join) == NULL) {
         DEBUG_SRCH   yLOG_note    ("can not use complex joins in normal search line");
         ymark_find_purge ();
         DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      snprintf (x_search, LEN_RECD, "/%s", a_search + 2);
      ++s_layers;
   }
   /*---(handle)------------------------*/
   DEBUG_SRCH   yLOG_point   ("e_regex"   , myMARK.e_regex);
   DEBUG_SRCH   yLOG_point   ("unit"      , ymark__unit_regex);
   --rce;  if (myMARK.e_regex == NULL) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SRCH   yLOG_value   ("s_layers"  , s_layers);
   DEBUG_SRCH   yLOG_info    ("x_search"  , x_search);
   x_rc = myMARK.e_regex (x_search);
   DEBUG_SRCH   yLOG_value   ("x_rc"      , x_rc);
   --rce;  if (x_rc < 0) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save search to history)---------*/
   rc = ymark_srch_new (a_search, '-', &x_new);
   DEBUG_SRCH   yLOG_point   ("x_new"     , x_new);
   --rce;  if (x_new == NULL) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(handle joins)-------------------*/
   if (strchr (YSTR_SJOIN, x_join) != NULL) {
      DEBUG_SRCH   yLOG_note    ("rationalize finds based on join");
      DEBUG_SRCH   yLOG_value   ("before"    , ymark_find_count ());
      ymark_find_by_index  (n, &x_find);
      while (x_find != NULL) {
         switch (x_join) {
         case G_CHAR_AND  : case 'Ð' :
            if (x_find->c < 2) {
               DEBUG_SRCH   yLOG_complex ("Ð remove"  , "%3d#, %2dc %s", n, x_find->c, x_find->label);
               ymark_find_free (&x_find);
            } else  {
               DEBUG_SRCH   yLOG_complex ("Ð skipped" , "%3d#, %2dc %s", n, x_find->c, x_find->label);
               ++n;
            }
            break;
         case G_CHAR_XOR  : case 'Ò' :
            if (x_find->c > 1) {
               DEBUG_SRCH   yLOG_complex ("Ò remove"  , "%3d#, %2dc %s", n, x_find->c, x_find->label);
               ymark_find_free (&x_find);
            } else {
               DEBUG_SRCH   yLOG_complex ("Ò skipped" , "%3d#, %2dc %s", n, x_find->c, x_find->label);
               ++n;
            }
            break;
         case G_CHAR_OR   : case 'Ñ' :
            DEBUG_SRCH   yLOG_complex ("Ñ skipped" , "%3d#, %2dc %s", n, x_find->c, x_find->label);
            ++n;
            break;
         }
         if (x_find != NULL)  x_find->c = 1;
         ymark_find_by_index (n, &x_find);
      }
      DEBUG_SRCH   yLOG_value   ("after"     , ymark_find_count ());
   }
   /*---(save result)--------------------*/
   x_new->found = ymark_find_count ();
   ++(x_new->runs);
   /*---(complete)-----------------------*/
   DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
   return x_rc;
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
   char       *p           = NULL;
   char       *q           = NULL;
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
   p  = strchr (S_SRCH_LIST, a_abbr);
   DEBUG_SRCH   yLOG_spoint  (p);
   --rce;  if (p == NULL) {
      DEBUG_SRCH   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   q  = strchr (S_SRCH_LIST, 'a');
   n = p - q;
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
ymark_srch_mark         (uchar a_abbr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =   -1;
   tSRCH      *x_tail      = NULL;
   /*---(header)-------------------------*/
   DEBUG_SRCH   yLOG_enter   (__FUNCTION__);
   /*---(check)--------------------------*/
   DEBUG_SRCH   yLOG_char    ("a_abbr"    , a_abbr);
   --rce;  if (a_abbr == 0) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   n  = ymark_srch_index (a_abbr);
   DEBUG_SRCH   yLOG_value   ("n"         , n);
   --rce;  if (n  < 0) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, n);
      return n;
   }
   /*---(save mark)----------------------*/
   ymark_srch_by_cursor (YDLST_TAIL, &x_tail);
   DEBUG_SRCH   yLOG_point   ("x_tail"    , x_tail);
   --rce;  if (x_tail == NULL) {
      DEBUG_SRCH   yLOG_exitr (__FUNCTION__, rce);
      return rce;
   }
   S_SRCHS [n] = x_tail;
   DEBUG_SRCH   yLOG_point   ("S_SRCHS"   , S_SRCHS [n]);
   /*---(complete)-----------------------*/
   DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ymark_srch_unmark       (uchar a_abbr)
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
   S_SRCHS [n] = NULL;
   /*---(complete)-----------------------*/
   DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ymark_srch_export       (uchar a_abbr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        n           =   -1;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_SRCH   yLOG_char    ("a_abbr"    , a_abbr);
   n = ymark_srch_index  (a_abbr);
   --rce;  if (n <  0)  {
      DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write)--------------------------*/
   rc = strlexport (0, S_SRCHS [n]->text);
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ymark_srch_import       (uchar a_abbr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_recd      [LEN_RECD];
   int         n           =    0;
   tSRCH      *x_new       = NULL;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_SRCH   yLOG_char    ("a_abbr"    , a_abbr);
   n = ymark_srch_index  (a_abbr);
   DEBUG_SRCH   yLOG_value   ("n"         , n);
   --rce;  if (n < 0) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(read)---------------------------*/
   rc = strlimport  (0, x_recd, NULL);
   DEBUG_YSRC   yLOG_value   ("read"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSRC   yLOG_info    ("x_recd"    , x_recd);
   /*---(save search to history)---------*/
   rc = ymark_srch_new (x_recd, '-', &x_new);
   DEBUG_SRCH   yLOG_point   ("x_new"     , x_new);
   --rce;  if (x_new == NULL) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   S_SRCHS [n] = x_new;
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ymark_srch_copy         (uchar a_src, uchar a_dst)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        s           =   -1;
   char        d           =   -1;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YSRC   yLOG_char    ("a_src"       , a_src);
   s = ysrc_sreg_index  (a_src);
   DEBUG_YSRC   yLOG_value   ("s"         , s);
   DEBUG_YSRC   yLOG_point   ("S_SRCHS"   , S_SRCHS [s]);
   --rce;  if (s <  0 || S_SRCHS [s] == NULL) {
      DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSRC   yLOG_char    ("a_dst"       , a_dst);
   d = ysrc_sreg_index  (a_dst);
   DEBUG_YSRC   yLOG_value   ("d"         , d);
   --rce;  if (s <  0) {
      DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(copy)---------------------------*/
   S_SRCHS [d] = S_SRCHS [s];
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return rc;
}

char         /*-> tbd --------------------------------[ ------ [ge.#M5.1C#.#7]*/ /*-[03.0000.013.L]-*/ /*-[--.---.---.--]-*/
ymark_srch_direct       (uchar *a_string)
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
   DEBUG_SRCH   yLOG_point   ("a_string"  , a_string);
   --rce;  if (a_string == NULL) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rc;
   }
   /*---(parse)-------------------------*/
   x_len = strlen (a_string);
   DEBUG_SRCH   yLOG_value   ("x_len"     , x_len);
   --rce;  if (x_len <= 1) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_abbr  = a_string [0];
   DEBUG_SRCH   yLOG_char    ("x_abbr"      , x_abbr);
   x_div   = a_string [1];
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
      case '#' : rc = ymark_srch_unmark (x_abbr);        break;
      case '-' : rc = ymark_srch_export (x_abbr);        break;
      case '+' : rc = ymark_srch_import (x_abbr);        break;
      default  :
                 DEBUG_SRCH   yLOG_note    ("divider (x_div) not a 2-char version");
                 DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
                 return rce;
      }
      DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(copy)---------------------------*/
   --rce;  if (x_len == 3 && x_div == '>') {
      rc = ymark_srch_copy   (x_abbr, a_string [2]);
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(move)---------------------------*/
   --rce;  if (x_len == 3 && x_div == ']') {
      rc = ymark_srch_copy   (x_abbr, a_string [2]);
      rc = ymark_srch_unmark (x_abbr);
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(swap)---------------------------*/
   --rce;  if (x_len == 3 && x_div == ')') {
      rc = ymark_srch_copy   (a_string [2], '.');
      rc = ymark_srch_copy   (x_abbr      , a_string [2]);
      rc = ymark_srch_copy   ('.'         , x_abbr);
      rc = ymark_srch_unmark ('.');
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(complete)-----------------------*/
   DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
   return rc;
}



