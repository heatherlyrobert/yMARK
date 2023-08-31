/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "yMARK.h"
#include    "yMARK_priv.h"

/*
 * metis ß dc4#∑ ß get very simple unit test for regex searching in place                 ß M255NH ß  8 ß
 * metis ß wc4#∑ ß add regex searching to metis to test interface                         ß M255Pp ß 14 ß
 * metis ß wc4#∑ ß replace regex searching in gyges to test interface                     ß M255Pq ß 13 ß
 * metis ß tn4#∑ ß build unit test for simple execute interface                           ß M2F52k ß  1 ß
 * metis ß tn4#∑ ß build unit test for direct/command interface                           ß M2F53Y ß  1 ß
 *
 * metis ß wv2<∑ ß yMARK search cursoring with [><] not working right                     ß M2OLDm ß  ∑ ß
 *
 * metis ß mv2∑∑ ß application logic for searching only in visual select area             ß M2K2tg ß  ∑ ß
 *
 *
 * METIS ß wn2∑∑ ß ABCDEFGHIJKLMNOPQRSTUVWXYZ                                             ß M2IKV4 ß  ∑ ß
 * METIS ß wn2∑∑ ß abcdefghijklmnopqrstuvwxyz                                             ß M2IKVO ß  ∑ ß
 * METIS ß wn2∑∑ ß 0123456789                                                             ß M2IKVZ ß  ∑ ß
 * METIS ß wn2∑∑ ß ËÈÍÎÏÌÓÔÒÚÛÙıˆ˜¯˘˙˚¸˝˛ˇ                                               ß M2IKVi ß  ∑ ß
 */

/*
 *
 *  highlighting/selecting an area or range, restricts search to that space
 *
 *  add-on searches...
 *     /first      initial search
 *     /—second    conducts a second on top and then OR's the results
 *     /–second    conducts a second on top and then AND's the results
 *     /“second    conducts a second on top and then XOR's the results
 *
 *  reverse searches...
 *     /ôfirst     all things NOT matching
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
 *     :search _     show search/find status
 *     :search ?     history window
 *     :search +     results of search highlighted
 *     :search -     results of search not highlighted
 *     :search a>    put in unnammed (∂) sreg
 *     :search a<    load from unnammed (∂) sreg
 *     :search a<∑∑∑ load from central repository file
 *     :search a~∑∑∑ load from user repository file
 *
 *  cursoring...
 *     :search [
 *     :search <
 *     :search >
 *     :search ]
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
static uchar   s_nlist       = 0;
static tSRCH  *S_SRCHS       [S_SRCH_MAX];
static int     s_seq         = 0;




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
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   /*---(pointers)-----------------------*/
   DEBUG_YMARK   yLOG_note    ("ground external pointers");
   myMARK.e_regex    = NULL;
   myMARK.e_unfind   = NULL;
   /*---(search btree)-------------------*/
   rc = ySORT_btree (B_SRCH, "searches");
   DEBUG_YMARK   yLOG_value   ("btree"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(search abbr list)---------------*/
   strlcpy (S_SRCH_LIST, YSTR_NUMBER, S_SRCH_MAX);
   strlcpy (S_SRCH_LIST, YSTR_LOWER , S_SRCH_MAX);
   strlcpy (S_SRCH_LIST, YSTR_UPPER , S_SRCH_MAX);
   strlcat (S_SRCH_LIST, YSTR_GREEK , S_SRCH_MAX);
   strlcat (S_SRCH_LIST, "."        , S_SRCH_MAX);  /* temp spot */
   DEBUG_YMARK   yLOG_info    ("LIST"      , S_SRCH_LIST);
   s_nlist  = strlen (S_SRCH_LIST);
   DEBUG_YMARK   yLOG_value   ("s_nlist"   , s_nlist);
   s_seq    = 0;
   /*---(purge marks)--------------------*/
   for (i = 0; i < s_nlist; ++i)  S_SRCHS [i] = NULL;
   /*---(mode)---------------------------*/
   rc = yMODE_init_set   (MODE_SEARCH, NULL, ySRC_mode);
   DEBUG_YMARK   yLOG_value   ("mode set"  , rc);
   /*---(purge)--------------------------*/
   /*> rc = ymark_srch_purge ();                                                      <*/
   DEBUG_YMARK   yLOG_value   ("purge"     , rc);
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
   return 0;
}

/*> char                                                                                        <* 
 *> ymark_srch_purge        (void)                                                              <* 
 *> {                                                                                           <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                                 <* 
 *>    char        rce         =  -10;                                                          <* 
 *>    char        rc          =    0;                                                          <* 
 *>    tSRCH      *x_curr      = NULL;                                                          <* 
 *>    tSRCH      *x_next      = NULL;                                                          <* 
 *>    /+---(header)-------------------------+/                                                 <* 
 *>    DEBUG_YMARK   yLOG_enter   (__FUNCTION__);                                               <* 
 *>    /+---(globals)------------------------+/                                                 <* 
 *>    s_layers = 0;                                                                            <* 
 *>    /+---(walk-through)-------------------+/                                                 <* 
 *>    DEBUG_YMARK   yLOG_value   ("count"     , ySORT_count (B_SRCH));                         <* 
 *>    rc = ySORT_by_cursor (B_SRCH, YDLST_HEAD, &x_curr);                                      <* 
 *>    DEBUG_YMARK   yLOG_point   ("x_curr"     , x_curr);                                      <* 
 *>    while (x_curr != NULL) {                                                                 <* 
 *>       rc = ySORT_by_cursor (B_SRCH, YDLST_NEXT, &x_next);                                   <* 
 *>       DEBUG_YMARK   yLOG_point   ("x_next"     , x_next);                                   <* 
 *>       DEBUG_YMARK   yLOG_point   ("x_curr"     , x_curr);                                   <* 
 *>       /+> rc = ymark_srch_free (&x_curr);                                             <+/   <* 
 *>       x_curr = x_next;                                                                      <* 
 *>    }                                                                                        <* 
 *>    /+---(check)--------------------------+/                                                 <* 
 *>    DEBUG_YMARK   yLOG_value   ("count"     , ySORT_count (B_SRCH));                         <* 
 *>    --rce;  if (ySORT_count (B_SRCH) > 0) {                                                  <* 
 *>       DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                                       <* 
 *>       return rce;                                                                           <* 
 *>    }                                                                                        <* 
 *>    /+---(complete)-----------------------+/                                                 <* 
 *>    DEBUG_YMARK   yLOG_exit    (__FUNCTION__);                                               <* 
 *>    return 0;                                                                                <* 
 *> }                                                                                           <*/

char
ymark_srch_wrap         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   /*---(purge)--------------------------*/
   /*> rc = ymark_srch_purge ();                                                      <*/
   DEBUG_YMARK   yLOG_value   ("purge"     , rc);
   /*---(pointers)-----------------------*/
   DEBUG_YMARK   yLOG_note    ("ground external pointers");
   myMARK.e_regex    = NULL;
   myMARK.e_unfind   = NULL;
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                    memory allocation                        ----===*/
/*====================------------------------------------====================*/
static void  o___MEMORY__________o () { return; }

/*> char                                                                              <* 
 *> ymark_srch_new          (uchar *a_text, char a_force, tSRCH **r_new)              <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                       <* 
 *>    char        rce         =  -10;                                                <* 
 *>    char        rc          =    0;                                                <* 
 *>    tSRCH      *x_new       = NULL;                                                <* 
 *>    int         x_tries     =    0;                                                <* 
 *>    /+---(header)-------------------------+/                                       <* 
 *>    DEBUG_YMARK   yLOG_enter   (__FUNCTION__);                                     <* 
 *>    /+---(check return)-------------------+/                                       <* 
 *>    DEBUG_YMARK   yLOG_point   ("r_new"     , r_new);                              <* 
 *>    --rce;  if (r_new == NULL) {                                                   <* 
 *>       DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                             <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    DEBUG_YMARK   yLOG_point   ("*r_new"    , *r_new);                             <* 
 *>    --rce;  if (a_force != 'y' && *r_new != NULL) {                                <* 
 *>       DEBUG_YMARK   yLOG_note    ("pointer is already set, not in force mode");   <* 
 *>       DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                             <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    DEBUG_YMARK   yLOG_point   ("a_text"    , a_text);                             <* 
 *>    --rce;  if (a_text == NULL || strlen (a_text) <= 0) {                          <* 
 *>       DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                             <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    DEBUG_YMARK   yLOG_info    ("a_text"    , a_text);                             <* 
 *>    /+---(existing)-----------------------+/                                       <* 
 *>    ymark_srch_by_text (a_text, r_new);                                            <* 
 *>    DEBUG_YMARK   yLOG_point   ("*r_new"    , *r_new);                             <* 
 *>    --rce;  if (*r_new != NULL) {                                                  <* 
 *>       DEBUG_YMARK   yLOG_note    ("already found, increasing counter");           <* 
 *>       str2mongo (time (NULL), (*r_new)->runtime);                                 <* 
 *>       strlpadn  (++s_seq, (*r_new)->seq, '.', '>', 6);                            <* 
 *>       rc = ySORT_prepare (B_SRCH);                                                <* 
 *>       if (rc < 0) {                                                               <* 
 *>          DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                          <* 
 *>          return rce;                                                              <* 
 *>       }                                                                           <* 
 *>       DEBUG_YMARK   yLOG_exit    (__FUNCTION__);                                  <* 
 *>       return 1;                                                                   <* 
 *>    }                                                                              <* 
 *>    /+---(allocate)-----------------------+/                                       <* 
 *>    while (x_new == NULL) {                                                        <* 
 *>       ++x_tries;                                                                  <* 
 *>       x_new = (tSRCH *) malloc (sizeof (tSRCH));                                  <* 
 *>       if (x_tries > 3)   break;                                                   <* 
 *>    }                                                                              <* 
 *>    DEBUG_YMARK   yLOG_value   ("x_tries"   , x_tries);                            <* 
 *>    DEBUG_YMARK   yLOG_point   ("x_new"     , x_new);                              <* 
 *>    --rce;  if (x_new == NULL) {                                                   <* 
 *>       DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                             <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    /+---(wipe)---------------------------+/                                       <* 
 *>    x_new->text = strdup (a_text);                                                 <* 
 *>    x_new->runs = x_new->found = 0;                                                <* 
 *>    /+---(add timestamp)------------------+/                                       <* 
 *>    str2mongo (time (NULL), x_new->runtime);                                       <* 
 *>    strlpadn  (++s_seq, x_new->seq, '.', '>', 6);                                  <* 
 *>    /+---(into btree)---------------------+/                                       <* 
 *>    rc = ySORT_hook (B_SRCH, x_new, x_new->seq, &x_new->btree);                    <* 
 *>    DEBUG_YMARK   yLOG_value   ("btree"     , rc);                                 <* 
 *>    --rce;  if (rc < 0) {                                                          <* 
 *>       DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                             <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    /+---(update)-------------------------+/                                       <* 
 *>    rc = ySORT_prepare (B_SRCH);                                                   <* 
 *>    if (rc < 0) {                                                                  <* 
*>       DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                             <* 
*>       return rce;                                                                 <* 
*>    }                                                                              <* 
*>    /+---(save return)--------------------+/                                       <* 
*>    *r_new = x_new;                                                                <* 
*>    /+---(complete)-----------------------+/                                       <* 
*>    DEBUG_YMARK   yLOG_exit    (__FUNCTION__);                                     <* 
*>    return rc;                                                                     <* 
*> }                                                                                 <*/

/*> char                                                                              <* 
 *> ymark_srch_free         (tSRCH **r_old)                                           <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                       <* 
 *>    char        rce         =  -10;                                                <* 
 *>    char        rc          =    0;                                                <* 
 *>    /+---(header)-------------------------+/                                       <* 
 *>    DEBUG_YMARK   yLOG_enter   (__FUNCTION__);                                     <* 
 *>    /+---(check return)-------------------+/                                       <* 
 *>    DEBUG_YMARK   yLOG_point   ("r_old"     , r_old);                              <* 
 *>    --rce;  if (r_old == NULL) {                                                   <* 
 *>       DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                             <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    DEBUG_YMARK   yLOG_point   ("*r_old"    , *r_old);                             <* 
 *>    --rce;  if (*r_old == NULL) {                                                  <* 
 *>       DEBUG_YMARK   yLOG_note    ("pointer was never set");                       <* 
 *>       DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                             <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    /+---(unhook from btree)--------------+/                                       <* 
 *>    rc = ySORT_unhook (&(*r_old)->btree);                                          <* 
 *>    DEBUG_YMARK   yLOG_value   ("btree"     , rc);                                 <* 
 *>    --rce;  if (rc < 0) {                                                          <* 
 *>       DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                             <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    /+---(update)-------------------------+/                                       <* 
 *>    rc = ySORT_prepare (B_SRCH);                                                   <* 
 *>    if (rc < 0) {                                                                  <* 
 *>       DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                             <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    /+---(clear and return)---------------+/                                       <* 
 *>    free ((*r_old)->text);                                                         <* 
 *>    free (*r_old);                                                                 <* 
 *>    *r_old = NULL;                                                                 <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    DEBUG_YMARK   yLOG_exit    (__FUNCTION__);                                     <* 
 *>    return 0;                                                                      <* 
 *> }                                                                                 <*/



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
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   /*---(check return)-------------------*/
   DEBUG_YMARK   yLOG_point   ("r_srch"    , r_srch);
   --rce;  if (r_srch == NULL) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   *r_srch = NULL;
   DEBUG_YMARK   yLOG_point   ("a_text"    , a_text);
   --rce;  if (a_text == NULL) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YMARK   yLOG_info    ("a_text"    , a_text);
   /*---(walk the list)------------------*/
   ymark_srch_by_cursor (YDLST_HEAD, &x_curr);
   while (x_curr != NULL) {
      if (strcmp (x_curr->text, a_text) == 0) {
         DEBUG_YMARK   yLOG_note    ("found the match");
         *r_srch = x_curr;
         DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
         return 1;
      }
      ymark_srch_by_cursor (YDLST_NEXT, &x_curr);
   }
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
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
   char        s           [LEN_TERSE] = "  ∑";
   char        t           [LEN_TERSE] = "  ∑";
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   DEBUG_YMARK   yLOG_value   ("n"         , n);
   DEBUG_YMARK   yLOG_char    ("a_abbr"    , a_abbr);
   DEBUG_YMARK   yLOG_point   ("a_srch"    , a_srch);
   if (a_srch == NULL) {
      DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
      return "n/a";
   }
   if (a_srch->text != NULL)  sprintf (r, "%2dÂ%.40sÊ", strlen (a_srch->text), a_srch->text);
   if (a_srch->found > 0)     sprintf (s, "%3d", a_srch->found);
   if (a_srch->runs  > 0)     sprintf (t, "%3d", a_srch->runs);
   if (a_abbr == 0)  sprintf (g_print, "%-2d %-6.6s %-6.6s %-40.40s %-3.3s %-3.3s", n, a_srch->seq, a_srch->runtime, r, s, t);
   else              sprintf (g_print, "%c  %-6.6s %-6.6s %-40.40s %-3.3s %-3.3s", a_abbr, a_srch->seq, a_srch->runtime, r, s, t);
   DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
   return g_print;
}

char*
ymark_srch_entry        (int n)
{
   tSRCH      *x_srch      = NULL;
   ymark_srch_by_index (n, &x_srch);
   return ymark_srch__entry (n, 0, x_srch);
}

/*> char*                                                                             <* 
 *> ymark_srch_marked       (uchar a_abbr)                                            <* 
 *> {                                                                                 <* 
 *>    char        n           =    0;                                                <* 
 *>    n = ymark_srch_index (a_abbr);                                                 <* 
 *>    if (n < 0)  return "n/a";                                                      <* 
 *>    return ymark_srch__entry (n, a_abbr, S_SRCHS [n]);                             <* 
 *> }                                                                                 <*/

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

static char       s_layers     =    0;
static char       s_scope      =  'u';
static char       s_type       =  '∑';
static char       s_join       =  '∑';
static char       s_not        =  '∑';
static char       s_hist       =  '∑';
static char       s_comp       =  '∑';
static double     s_exp        =  0.0;

char
ymark_srch__prepare     (char b_search [LEN_RECD], char *r_type, char *r_join, char *r_not, char *r_hist, char *r_comp, double *r_exp)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_len       =    0;
   char        x_join      =  '∑';
   char        x_not       =  '∑';
   char        x_hist      =  '∑';
   char        x_type      =  '∑';
   char        x_comp      =  '∑';
   double      x_exp       =  0.0;
   int         i           =    0;
   char        x_srch      [LEN_RECD]  = "";
   /*---(header)-------------------------*/
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   if (r_type != NULL)  *r_type = '∑';
   if (r_join != NULL)  *r_join = '∑';
   if (r_not  != NULL)  *r_not  = '∑';
   if (r_hist != NULL)  *r_hist = '∑';
   if (r_comp != NULL)  *r_comp = '∑';
   if (r_exp  != NULL)  *r_exp  = 0.0;
   /*---(ready)--------------------------*/
   --rce;  if (!yMODE_operational (MODE_SEARCH)) {
      DEBUG_YMARK   yLOG_note    ("can not execute until operational");
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(defense)------------------------*/
   DEBUG_YMARK   yLOG_point   ("b_search"  , b_search);
   --rce;  if (b_search == NULL) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YMARK   yLOG_info    ("b_search"  , b_search);
   if (b_search [0] != '/') {
      DEBUG_YMARK   yLOG_note    ("must start with a forward slash");
      strlcpy (b_search, "", LEN_RECD);
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   x_len  = strlen (b_search);
   DEBUG_YMARK   yLOG_value   ("x_len"     , x_len);
   /*---(simple clear)-------------------*/
   --rce;  if (x_len == 1) {
      DEBUG_YMARK   yLOG_note    ("requested a specific purge of finds");
      ymark_find_purge ();
      strlcpy (b_search, "", LEN_RECD);
      DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(look for history)--------------*/
   strlcpy (x_srch, b_search, LEN_RECD);
   --rce;  if (strncmp (x_srch, "//", 2) == 0) {
      DEBUG_YCMD   yLOG_note    ("this is a history request");
      rc = yVIHUB_yVIEW_direct  (x_srch);
      DEBUG_YCMD   yLOG_value   ("direct"    , rc);
      if (rc < 1) {
         DEBUG_YCMD   yLOG_exit    (__FUNCTION__);
         return rc;
      }
      DEBUG_YCMD   yLOG_info    ("after"     , x_srch);
   }
   /*---(history)------------------------*/
   rc = yVIHUB_yVIEW_history (MODE_SEARCH , x_srch);
   DEBUG_YCMD   yLOG_value   ("history"   , rc);
   /*---(check for forced history)------*/
   if (x_srch [1] == '/') {
      strlcpy (x_srch, x_srch + 1, LEN_RECD);
      x_hist = 'y';
   } else {
      strlcpy (x_srch, x_srch    , LEN_RECD);
   }
   /*---(check for late-joins)-----------*/
   --rce;  for (i = 0; i < strlen (YSTR_JOIN); ++i) {
      if (strchr (x_srch + 2, YSTR_JOIN [i]) != NULL) {
         DEBUG_YMARK   yLOG_note    ("can not join (–—“”‘’) in normal search");
         strlcpy (x_srch, "", LEN_RECD);
         DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(prepare)------------------------*/
   DEBUG_YMARK   yLOG_char    ("joiner"    , x_srch [1]);
   /*---(simple not)---------------------*/
   if (x_srch [1] == '!') {
      DEBUG_YMARK   yLOG_note    ("simple reverse regex search, purge and go");
      ymark_find_purge ();
      snprintf (x_srch, LEN_RECD, "%c%s", x_srch [0], x_srch + 2);
      x_not  = 'y';
   }
   /*---(join search)-------------------*/
   else if (strchr (YSTR_JOIN, x_srch [1]) != NULL) {
      DEBUG_YMARK   yLOG_note    ("join/concatinating regex search, NO purge");
      x_join = x_srch [1];
      if (x_srch [2] == '!') {
         snprintf (x_srch, LEN_RECD, "%c%s", x_srch [0], x_srch + 3);
         x_not  = 'y';
      } else {
         snprintf (x_srch, LEN_RECD, "%c%s", x_srch [0], x_srch + 2);
      }
   }
   /*---(normal search)------------------*/
   else {
      DEBUG_YMARK   yLOG_note    ("simple regex search, purge and go");
      ymark_find_purge ();
   }
   /*---(save search to history)---------*/
   /*> rc = ymark_srch_new (a_search, '-', &x_new);                                   <* 
    *> DEBUG_YMARK   yLOG_point   ("x_new"     , x_new);                              <* 
    *> --rce;  if (x_new == NULL) {                                                   <* 
    *>    DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                             <* 
    *>    return rce;                                                                 <* 
    *> }                                                                              <*/
   /*---(check num types)----------------*/
   if (strchr ("=<>‹›ﬁ", x_srch [1]) != NULL) {
      x_comp = x_srch [1];
      rc = strl2num (x_srch + 2, &x_exp, LEN_RECD);
      if (rc >= 0)  x_type = 'i';
      if (strchr (x_srch + 2, '.') != NULL)  x_type = 'f';
      if (strchr (x_srch + 2, 'À') != NULL)  x_type = 'f';
   }
   if (x_srch [1] == '◊')                    x_type = 's';
   if (x_srch [1] == '÷')                    x_type = 'p';
   /*---(to static vars)-----------------*/
   s_type = x_type;
   s_join = x_join;
   s_not  = x_not;
   s_hist = x_hist;
   s_comp = x_comp;
   s_exp  = x_exp;
   /*---(save-back)----------------------*/
   strlcpy (b_search, x_srch, LEN_RECD);
   if (r_type != NULL)  *r_type = x_type;
   if (r_join != NULL)  *r_join = x_join;
   if (r_not  != NULL)  *r_not  = x_not;
   if (r_comp != NULL)  *r_comp = x_comp;
   if (r_hist != NULL)  *r_hist = x_hist;
   if (r_comp != NULL)  *r_comp = x_comp;
   if (r_exp  != NULL)  *r_exp  = x_exp;
   /*---(report-out)---------------------*/
   if (r_type != NULL)   DEBUG_YCMD   yLOG_char    ("r_type"    , *r_type);
   if (r_join != NULL)   DEBUG_YCMD   yLOG_char    ("r_join"    , *r_join);
   if (r_not  != NULL)   DEBUG_YCMD   yLOG_char    ("r_not "    , *r_not );
   if (r_comp != NULL)   DEBUG_YCMD   yLOG_char    ("r_comp"    , *r_comp);
   if (r_hist != NULL)   DEBUG_YCMD   yLOG_char    ("r_hist"    , *r_hist);
   if (r_comp != NULL)   DEBUG_YCMD   yLOG_char    ("r_comp"    , *r_comp);
   if (r_exp  != NULL)   DEBUG_YCMD   yLOG_double  ("r_exp"     , *r_exp );
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
   return 1;
}

/*> #define     YSTR_SJOIN    "–—“"                                                   <* 
 *> #define     YSTR_CJOIN    "–—“”‘’÷"                                               <*/
/*
 *  //a–b   µ&   and    (TT only)                                  == 2
 *      ¥) search for 1st
 *      ¥) search for 2nd
 *      ¥) retain 2 (1)
 *
 *  //a—b   µ|   or     (TT, TF, and FT)  all but FF               >= 1
 *      ¥) search for 1st
 *      ¥) search for 2nd
 *      ¥) retain all
 *
 *
 *  //a“b   µv   xor    (TF and FT)       one or the other         == 1
 *      ¥) search for 1st
 *      ¥) search for 2nd
 *      ¥) retain 1 (not 2)
 *
 *
 *  require world/tab start
 *
 *  //a‘b   µk   nand   (TF, FT, and FF)  all but TT               <= 2
 *      ¥) search for all
 *      ¥) search for 1st
 *      ¥) search for 2nd
 *      ¥) only retain  1, and 2 (NOT 3)
 *
 *  //a’b   µj   nor    (FF only)                                  == 0
 *      ¥) search for all
 *      ¥) search for 1st
 *      ¥) search for 2nd
 *      ¥) only retain  1 (NOT 2 or 3)
 *
 *
 *  //a”b   µi   nxor   (TT or FF only)   both same                == 0 || == 2
 *      ¥) search for all
 *      ¥) search for 1st
 *      ¥) search for 2nd
 *      ¥) only retain  1 and 3 (NOT 2)
 *
 */

char
ymark_srch__run       (char a_search [LEN_RECD], char a_scope, char a_type, char a_join)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_rc        =    0;
   /*---(header)-------------------------*/
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   /*---(check for handler)-------------*/
   DEBUG_YMARK   yLOG_point   ("e_regex"   , myMARK.e_regex);
   DEBUG_YMARK   yLOG_point   ("unit"      , ymark__unit_regex);
   --rce;  if (myMARK.e_regex == NULL) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)----------------------------*/
   --rce;  if (strchr ("∑-sp", a_type) != NULL) {
      if (strchr ("∑-", a_type) != NULL)  rc = yREGEX_comp (a_search + 1);
      else                                rc = yREGEX_comp (a_search + 2);
      DEBUG_YMARK   yLOG_value   ("comp rc"   , rc);
      if (rc < 0) {
         DEBUG_YMARK   yLOG_note    ("could not compile search");
         DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(call handler)------------------*/
   DEBUG_YMARK   yLOG_info    ("a_search"  , a_search);
   x_rc = myMARK.e_regex (a_scope);
   DEBUG_YMARK   yLOG_value   ("x_rc"      , x_rc);
   --rce;  if (x_rc < 0) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update layers)-----------------*/
   if (a_join != '∑')  ++s_layers;
   else                  s_layers = 1;
   DEBUG_YMARK   yLOG_value   ("s_layers"  , s_layers);
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
   return x_rc;
}

char
yMARK_check           (char a_label [LEN_LABEL], short u, short x, short y, short z, char a_ctype, char a_source [LEN_RECD], char a_string [LEN_RECD], double a_act, char a_print [LEN_RECD])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =   -1;
   int         x_exp       =    0;
   int         x_act       =    0;
   char        x_exist     =    0;
   /*---(header)-------------------------*/
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   DEBUG_YMARK   yLOG_complex ("args"      , "%-10.10s, tab %2d, col %3d, row %4d, %c, %-20.20s, %-20.20s, %lf, %-20.20s", a_label, u, x, y, a_ctype, a_source, a_string, a_act, a_print);
   DEBUG_YMARK   yLOG_complex ("static"    , "s_type %c, s_join %c, s_not  %c, s_comp %c %ld", s_type, s_join, s_not, s_comp, s_exp);
   /*---(string type)--------------*/
   if (s_type == '-' || s_type == '∑') {
      switch (a_ctype) {
      case 's' :
         DEBUG_YMARK   yLOG_info    ("a_source"  , a_source);
         if (strchr ("‘’”", s_join) != NULL)   yMARK_found (a_label, u, x, y, 0);
         rc = yREGEX_filter (a_source);
         break;
      case '#' : case '5' :
         DEBUG_YMARK   yLOG_info    ("a_string"  , a_string);
         if (strchr ("‘’”", s_join) != NULL)   yMARK_found (a_label, u, x, y, 0);
         rc = yREGEX_filter (a_string);
         break;
      default          :
         DEBUG_YMARK   yLOG_note    ("can not process cell type");
         break;
      }
   }
   /*---(source type)--------------*/
   else if (s_type == 's') {
      DEBUG_YMARK   yLOG_info    ("a_source"  , a_source);
      if (strchr ("‘’”", s_join) != NULL)   yMARK_found (a_label, u, x, y, 0);
      rc = yREGEX_filter (a_source);
   }
   /*---(printable type)-----------*/
   else if (s_type == 'p') {
      DEBUG_YMARK   yLOG_info    ("a_print"   , a_print);
      if (strchr ("‘’”", s_join) != NULL)   yMARK_found (a_label, u, x, y, 0);
      rc = yREGEX_filter (a_print);
   }
   /*---(number type)--------------*/
   else if (s_type == 'i') {
      x_act = (int) a_act;
      x_exp = (int) s_exp;
      switch (a_ctype) {
      case 'n' : case '=' : case '9' :
         DEBUG_YMARK   yLOG_value   ("x_act"          , x_act);
         if (strchr ("‘’”", s_join) != NULL)   yMARK_found (a_label, u, x, y, 0);
         switch (s_comp) {
         case '='  :  if (x_act == x_exp)  rc = 1;   break;
         case '<'  :  if (x_act <  x_exp)  rc = 1;   break;
         case '>'  :  if (x_act >  x_exp)  rc = 1;   break;
         case '‹'  :  if (x_act <= x_exp)  rc = 1;   break;
         case '›'  :  if (x_act >= x_exp)  rc = 1;   break;
         case 'ﬁ'  :  if (x_act != x_exp)  rc = 1;   break;
         }
         break;
      }
   }
   /*---(float type)---------------*/
   else if (s_type == 'f') {
      switch (a_ctype) {
      case 'n' : case '=' : case '9' :
         DEBUG_YMARK   yLOG_double  ("a_act"          , a_act);
         if (strchr ("‘’”", s_join) != NULL)   yMARK_found (a_label, u, x, y, 0);
         switch (s_comp) {
         case '='  :  if (a_act == s_exp)  rc = 1;   break;
         case '<'  :  if (a_act <  s_exp)  rc = 1;   break;
         case '>'  :  if (a_act >  s_exp)  rc = 1;   break;
         case '‹'  :  if (a_act <= s_exp)  rc = 1;   break;
         case '›'  :  if (a_act >= s_exp)  rc = 1;   break;
         case 'ﬁ'  :  if (a_act != s_exp)  rc = 1;   break;
         }
         break;
      }
   }
   /*---(handle results)-----------*/
   x_exist = ymark_find_by_label (a_label, NULL);
   DEBUG_YMARK   yLOG_complex ("handle"    , "%c %4d %4d", s_not, rc, x_exist);
   if (s_not != 'y' && rc >  0) {
      DEBUG_YMARK   yLOG_note    ("positive find");
      yMARK_found (a_label, u, x, y, 0);
   } else if (s_not == 'y' && rc <= 0) {
      DEBUG_YMARK   yLOG_note    ("negative find");
      yMARK_found (a_label, u, x, y, 0);
      rc = 1;
   } else if (x_exist >= 0) {
      rc = 1;
   }
   DEBUG_YMARK   yLOG_value   ("rc"        , rc);
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ymark_srch__prune     (char a_join)
{
   /*---(locals)-----------+-----+-----+-*/
   tFIND      *x_find      = NULL;
   int         n           =    0;
   /*---(header)-------------------------*/
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   /*---(quick-out)----------------------*/
   DEBUG_YMARK   yLOG_char    ("a_join"    , a_join);
   if (strchr (YSTR_JOIN, a_join) == NULL) {
      yVIHUB_yVIEW_stats ('y', ymark_find_count ());
      DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(quick-out)----------------------*/
   DEBUG_YMARK   yLOG_note    ("rationalize finds based on join");
   DEBUG_YMARK   yLOG_value   ("before"    , ymark_find_count ());
   ymark_find_by_index  (n, &x_find);
   while (x_find != NULL) {
      /*---(handle joins)----------------*/
      DEBUG_YMARK   yLOG_complex ("x_find"    , "%-10.10s, %2du, %3dx, %4dy, %dc", x_find->label, x_find->u, x_find->x, x_find->y, x_find->c);
      switch (a_join) {
      case '—' : /* OR   (simple, take all)  */
         DEBUG_YMARK   yLOG_complex ("— keep"    , "%3d#, %2dc %s", n, x_find->c, x_find->label);
         ++n;
         break;
      case '–' : /* AND  (take only 2's) */
         if (x_find->c == 2) {
            DEBUG_YMARK   yLOG_complex ("– keep"    , "%3d#, %2dc %s", n, x_find->c, x_find->label);
            ++n;
         } else  {
            DEBUG_YMARK   yLOG_complex ("– remove"  , "%3d#, %2dc %s", n, x_find->c, x_find->label);
            myMARK.e_unfind (x_find->label, x_find->u, x_find->x, x_find->y, 0);
            ymark_find_free (&x_find);
         }
         break;
      case '“' : /* XOR  (take only 1's) */
         if (x_find->c == 1) {
            DEBUG_YMARK   yLOG_complex ("“ keep"    , "%3d#, %2dc %s", n, x_find->c, x_find->label);
            ++n;
         } else {
            DEBUG_YMARK   yLOG_complex ("“ remove"  , "%3d#, %2dc %s", n, x_find->c, x_find->label);
            myMARK.e_unfind (x_find->label, x_find->u, x_find->x, x_find->y, 0);
            ymark_find_free (&x_find);
         }
         break;
      case '‘' : /* NAND (take 1's and 2's only) */
         if (x_find->c == 1 || x_find->c == 2) {
            DEBUG_YMARK   yLOG_complex ("‘ keep"    , "%3d#, %2dc %s", n, x_find->c, x_find->label);
            ++n;
         } else {
            DEBUG_YMARK   yLOG_complex ("‘ remove"  , "%3d#, %2dc %s", n, x_find->c, x_find->label);
            myMARK.e_unfind (x_find->label, x_find->u, x_find->x, x_find->y, 0);
            ymark_find_free (&x_find);
         }
         break;
      case '’' : /* NOR  (take 1's only) */
         if (x_find->c == 1) {
            DEBUG_YMARK   yLOG_complex ("’ keep"    , "%3d#, %2dc %s", n, x_find->c, x_find->label);
            ++n;
         } else {
            DEBUG_YMARK   yLOG_complex ("’ remove"  , "%3d#, %2dc %s", n, x_find->c, x_find->label);
            myMARK.e_unfind (x_find->label, x_find->u, x_find->x, x_find->y, 0);
            ymark_find_free (&x_find);
         }
         break;
      case '”' : /* NXOR (take 1's and 3's only) */
         if (x_find->c == 1 || x_find->c == 3) {
            DEBUG_YMARK   yLOG_complex ("” keep"    , "%3d#, %2dc %s", n, x_find->c, x_find->label);
            ++n;
         } else {
            DEBUG_YMARK   yLOG_complex ("” remove"  , "%3d#, %2dc %s", n, x_find->c, x_find->label);
            myMARK.e_unfind (x_find->label, x_find->u, x_find->x, x_find->y, 0);
            ymark_find_free (&x_find);
         }
         break;
      }
      /*---(reset count)-----------------*/
      if (x_find != NULL)  x_find->c = 1;
      /*---(get next)--------------------*/
      ymark_find_by_index (n, &x_find);
      /*---(done)------------------------*/
   }
   DEBUG_YMARK   yLOG_value   ("after"     , ymark_find_count ());
   yVIHUB_yVIEW_stats ('y', ymark_find_count ());
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
   return 0;
}

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
   char        x_type      =  '∑';
   char        x_join      =  '∑';
   char        x_not       =  '∑';
   char        x_comp      =  '∑';
   char        x_hist      =  '∑';
   double      x_exp       =  0.0;
   char        x_search    [LEN_RECD]  = "";
   int         n           =    0;
   /*---(header)-------------------------*/
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (!yMODE_operational (MODE_SEARCH)) {
      DEBUG_YMARK   yLOG_note    ("can not execute until operational");
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (a_search != NULL)  strlcpy (x_search, a_search, LEN_RECD);
   /*---(prepare)-----------------------*/
   rc = ymark_srch__prepare (x_search, &x_type, &x_join, &x_not, &x_hist, &x_comp, &x_exp);
   DEBUG_YMARK   yLOG_value   ("prepare"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (rc == 0) {
      DEBUG_YMARK   yLOG_note    ("purge only request");
      DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(handle)------------------------*/
   rc = ymark_srch__run     (x_search, 'u', x_type, x_join);
   DEBUG_YMARK   yLOG_value   ("run"       , rc);
   --rce;  if (rc < 0) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(handle joins)-------------------*/
   rc = ymark_srch__prune   (x_join);
   DEBUG_YMARK   yLOG_value   ("prune"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(jump to head)-------------------*/
   /*> yMARK_find_hmode (' ', '[');                                                   <*/
   /*---(history)------------------------*/
   /*> rc = yVIHUB_yVIEW_history (MODE_SEARCH, x_srch);                               <*/
   DEBUG_YCMD   yLOG_value   ("history"   , rc);
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
   return x_rc;
}



/*====================------------------------------------====================*/
/*===----                        data movement                         ----===*/
/*====================------------------------------------====================*/
static void  o___DATA____________o () { return; }

/*> char                                                                              <* 
 *> ymark_srch_index        (uchar a_abbr)                                            <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                       <* 
 *>    char        rce         =  -10;                                                <* 
 *>    char       *p           = NULL;                                                <* 
 *>    char       *q           = NULL;                                                <* 
 *>    int         n           =   -1;                                                <* 
 *>    /+---(header)-------------------------+/                                       <* 
 *>    DEBUG_YMARK   yLOG_senter  (__FUNCTION__);                                     <* 
 *>    /+---(check)--------------------------+/                                       <* 
 *>    DEBUG_YMARK   yLOG_schar   (a_abbr);                                           <* 
 *>    --rce;  if (a_abbr == 0) {                                                     <* 
 *>       DEBUG_YMARK   yLOG_sexitr  (__FUNCTION__, rce);                             <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    DEBUG_YMARK   yLOG_snote   ("check");                                          <* 
 *>    p  = strchr (S_SRCH_LIST, a_abbr);                                             <* 
 *>    DEBUG_YMARK   yLOG_spoint  (p);                                                <* 
 *>    --rce;  if (p == NULL) {                                                       <* 
 *>       DEBUG_YMARK   yLOG_sexitr  (__FUNCTION__, rce);                             <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    q  = strchr (S_SRCH_LIST, 'a');                                                <* 
 *>    n = p - q;                                                                     <* 
 *>    DEBUG_YMARK   yLOG_sint    (n);                                                <* 
 *>    --rce;  if (n  < 0) {                                                          <* 
 *>       DEBUG_YMARK   yLOG_sexitr  (__FUNCTION__, n);                               <* 
 *>       return n;                                                                   <* 
 *>    }                                                                              <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    DEBUG_YMARK   yLOG_sexit   (__FUNCTION__);                                     <* 
 *>    return n;                                                                      <* 
 *> }                                                                                 <*/

/*> char                                                                              <* 
 *> ymark_srch_mark         (uchar a_abbr)                                            <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                       <* 
 *>    char        rce         =  -10;                                                <* 
 *>    int         n           =   -1;                                                <* 
 *>    tSRCH      *x_tail      = NULL;                                                <* 
 *>    /+---(header)-------------------------+/                                       <* 
 *>    DEBUG_YMARK   yLOG_enter   (__FUNCTION__);                                     <* 
 *>    /+---(check)--------------------------+/                                       <* 
 *>    DEBUG_YMARK   yLOG_char    ("a_abbr"    , a_abbr);                             <* 
 *>    --rce;  if (a_abbr == 0) {                                                     <* 
 *>       DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                             <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    n  = ymark_srch_index (a_abbr);                                                <* 
 *>    DEBUG_YMARK   yLOG_value   ("n"         , n);                                  <* 
 *>    --rce;  if (n  < 0) {                                                          <* 
 *>       DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, n);                               <* 
 *>       return n;                                                                   <* 
 *>    }                                                                              <* 
 *>    /+---(save mark)----------------------+/                                       <* 
 *>    ymark_srch_by_cursor (YDLST_TAIL, &x_tail);                                    <* 
 *>    DEBUG_YMARK   yLOG_point   ("x_tail"    , x_tail);                             <* 
 *>    --rce;  if (x_tail == NULL) {                                                  <* 
 *>       DEBUG_YMARK   yLOG_exitr (__FUNCTION__, rce);                               <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    S_SRCHS [n] = x_tail;                                                          <* 
 *>    DEBUG_YMARK   yLOG_point   ("S_SRCHS"   , S_SRCHS [n]);                        <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    DEBUG_YMARK   yLOG_exit    (__FUNCTION__);                                     <* 
 *>    return 0;                                                                      <* 
 *> }                                                                                 <*/

/*> char                                                                              <* 
 *> ymark_srch_unmark       (uchar a_abbr)                                            <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                       <* 
 *>    char        rce         =  -10;                                                <* 
 *>    int         n           =    0;                                                <* 
 *>    /+---(header)-------------------------+/                                       <* 
 *>    DEBUG_YMARK   yLOG_enter   (__FUNCTION__);                                     <* 
 *>    /+---(defense)------------------------+/                                       <* 
 *>    DEBUG_YMARK   yLOG_char    ("a_abbr"    , a_abbr);                             <* 
 *>    n = ymark_srch_index  (a_abbr);                                                <* 
 *>    DEBUG_YMARK   yLOG_value   ("n"         , n);                                  <* 
 *>    --rce;  if (n < 0) {                                                           <* 
 *>       DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                             <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    /+---(clear)--------------------------+/                                       <* 
 *>    S_SRCHS [n] = NULL;                                                            <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    DEBUG_YMARK   yLOG_exit    (__FUNCTION__);                                     <* 
 *>    return 0;                                                                      <* 
 *> }                                                                                 <*/

/*> char                                                                              <* 
 *> ymark_srch_export       (uchar a_abbr)                                            <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                       <* 
 *>    char        rce         =  -10;                                                <* 
 *>    char        rc          =    0;                                                <* 
 *>    char        n           =   -1;                                                <* 
 *>    /+---(header)-------------------------+/                                       <* 
 *>    DEBUG_YMARK   yLOG_enter   (__FUNCTION__);                                     <* 
 *>    /+---(defense)------------------------+/                                       <* 
 *>    DEBUG_YMARK   yLOG_char    ("a_abbr"    , a_abbr);                             <* 
 *>    n = ymark_srch_index  (a_abbr);                                                <* 
 *>    --rce;  if (n <  0)  {                                                         <* 
 *>       DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                             <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    /+---(write)--------------------------+/                                       <* 
 *>    rc = strlexport (0, S_SRCHS [n]->text);                                        <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    DEBUG_YMARK   yLOG_exit    (__FUNCTION__);                                     <* 
 *>    return rc;                                                                     <* 
 *> }                                                                                 <*/

/*> char                                                                              <* 
 *> ymark_srch_import       (uchar a_abbr)                                            <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                       <* 
 *>    char        rce         =  -10;                                                <* 
 *>    char        rc          =    0;                                                <* 
 *>    char        x_recd      [LEN_RECD];                                            <* 
 *>    int         n           =    0;                                                <* 
 *>    tSRCH      *x_new       = NULL;                                                <* 
 *>    /+---(header)-------------------------+/                                       <* 
 *>    DEBUG_YMARK   yLOG_enter   (__FUNCTION__);                                     <* 
 *>    /+---(defense)------------------------+/                                       <* 
 *>    DEBUG_YMARK   yLOG_char    ("a_abbr"    , a_abbr);                             <* 
 *>    n = ymark_srch_index  (a_abbr);                                                <* 
 *>    DEBUG_YMARK   yLOG_value   ("n"         , n);                                  <* 
 *>    --rce;  if (n < 0) {                                                           <* 
 *>       DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                             <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    /+---(read)---------------------------+/                                       <* 
 *>    rc = strlimport  (0, x_recd, NULL);                                            <* 
 *>    DEBUG_YMARK   yLOG_value   ("read"      , rc);                                 <* 
 *>    --rce;  if (rc < 0) {                                                          <* 
 *>       DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                             <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    DEBUG_YMARK   yLOG_info    ("x_recd"    , x_recd);                             <* 
 *>    /+---(save search to history)---------+/                                       <* 
 *>    rc = ymark_srch_new (x_recd, '-', &x_new);                                     <* 
 *>    DEBUG_YMARK   yLOG_point   ("x_new"     , x_new);                              <* 
 *>    --rce;  if (x_new == NULL) {                                                   <* 
 *>       DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                             <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    S_SRCHS [n] = x_new;                                                           <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    DEBUG_YMARK   yLOG_exit    (__FUNCTION__);                                     <* 
 *>    return rc;                                                                     <* 
 *> }                                                                                 <*/

/*> char                                                                              <* 
 *> ymark_srch_copy         (uchar a_src, uchar a_dst)                                <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                       <* 
 *>    char        rce         =  -10;                                                <* 
 *>    char        rc          =    0;                                                <* 
 *>    char        s           =   -1;                                                <* 
 *>    char        d           =   -1;                                                <* 
 *>    /+---(header)-------------------------+/                                       <* 
 *>    DEBUG_YMARK   yLOG_enter   (__FUNCTION__);                                     <* 
 *>    /+---(defense)------------------------+/                                       <* 
 *>    DEBUG_YMARK   yLOG_char    ("a_src"       , a_src);                            <* 
 *>    s = ymark_srch_index  (a_src);                                                 <* 
 *>    DEBUG_YMARK   yLOG_value   ("s"         , s);                                  <* 
 *>    DEBUG_YMARK   yLOG_point   ("S_SRCHS"   , S_SRCHS [s]);                        <* 
 *>    --rce;  if (s <  0 || S_SRCHS [s] == NULL) {                                   <* 
 *>       DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                             <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    DEBUG_YMARK   yLOG_char    ("a_dst"       , a_dst);                            <* 
 *>    d = ymark_srch_index  (a_dst);                                                 <* 
 *>    DEBUG_YMARK   yLOG_value   ("d"         , d);                                  <* 
 *>    --rce;  if (s <  0) {                                                          <* 
 *>       DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                             <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    /+---(copy)---------------------------+/                                       <* 
 *>    S_SRCHS [d] = S_SRCHS [s];                                                     <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    DEBUG_YMARK   yLOG_exit    (__FUNCTION__);                                     <* 
 *>    return rc;                                                                     <* 
 *> }                                                                                 <*/

/*> char                                                                              <* 
 *> ymark_srch_full         (uchar a_abbr, uchar *a_text)                             <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                       <* 
 *>    char        rce         =  -10;                                                <* 
 *>    char        rc          =    0;                                                <* 
 *>    char        n           =   -1;                                                <* 
 *>    char        x_text      [LEN_RECD]  = "";                                      <* 
 *>    tSRCH      *x_new       = NULL;                                                <* 
 *>    /+---(header)-------------------------+/                                       <* 
 *>    DEBUG_YMARK   yLOG_enter   (__FUNCTION__);                                     <* 
 *>    /+---(defense)------------------------+/                                       <* 
 *>    DEBUG_YMARK   yLOG_char    ("a_abbr"      , a_abbr);                           <* 
 *>    n = ymark_srch_index  (a_abbr);                                                <* 
 *>    DEBUG_YMARK   yLOG_value   ("n"         , n);                                  <* 
 *>    --rce;  if (n <  0) {                                                          <* 
 *>       DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                             <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    DEBUG_YMARK   yLOG_point   ("a_text"    , a_text);                             <* 
 *>    --rce;  if (a_text == NULL) {                                                  <* 
 *>       DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                             <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    DEBUG_YMARK   yLOG_info    ("a_text"    , a_text);                             <* 
 *>    /+---(check quoting)------------------+/                                       <* 
 *>    strlcpy     (x_text, a_text, LEN_RECD);                                        <* 
 *>    strltrim    (x_text, ySTR_BOTH, LEN_RECD);                                     <* 
 *>    strldequote (x_text, LEN_RECD);                                                <* 
 *>    /+---(save search to history)---------+/                                       <* 
 *>    rc = ymark_srch_new (x_text, '-', &x_new);                                     <* 
 *>    DEBUG_YMARK   yLOG_point   ("x_new"     , x_new);                              <* 
 *>    --rce;  if (x_new == NULL) {                                                   <* 
 *>       DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                             <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    /+---(add mark)-----------------------+/                                       <* 
 *>    rc = ymark_srch_mark (a_abbr);                                                 <* 
 *>    DEBUG_YMARK   yLOG_value   ("marking"   , rc);                                 <* 
 *>    if (rc < 0) {                                                                  <* 
 *>       DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                             <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    DEBUG_YMARK   yLOG_exit    (__FUNCTION__);                                     <* 
 *>    return rc;                                                                     <* 
 *> }                                                                                 <*/

char
ymark_srch_central      (uchar a_abbr, uchar *a_key)
{
   /*> /+---(locals)-----------+-----+-----+-+/                                                 <* 
    *> char        rce         =  -10;                                                          <* 
    *> char        rc          =    0;                                                          <* 
    *> char        n           =    0;                                                          <* 
    *> FILE       *f           = NULL;                                                          <* 
    *> char        x_recd      [LEN_RECD];                                                      <* 
    *> char        x_seek      [LEN_LABEL];                                                     <* 
    *> char        x_key       [LEN_RECD];                                                      <* 
    *> int         x_len       =    0;                                                          <* 
    *> char        x_direct    [LEN_RECD];                                                      <* 
    *> char        x_found     =  '-';                                                          <* 
    *> int         c           =    0;                                                          <* 
    *> char       *p           = NULL;                                                          <* 
    *> char       *q           = "ß";                                                          <* 
    *> char       *r           = NULL;                                                          <* 
    *> /+---(header)-------------------------+/                                                 <* 
    *> DEBUG_YMARK   yLOG_enter   (__FUNCTION__);                                               <* 
    *> /+---(defense)------------------------+/                                                 <* 
    *> DEBUG_YMARK   yLOG_char    ("a_abbr"      , a_abbr);                                     <* 
    *> n = ymark_srch_index  (a_abbr);                                                          <* 
    *> DEBUG_YMARK   yLOG_value   ("n"         , n);                                            <* 
    *> --rce;  if (n <  0) {                                                                    <* 
    *>    DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                                       <* 
    *>    return rce;                                                                           <* 
    *> }                                                                                        <* 
    *> DEBUG_YMARK   yLOG_point   ("a_key"     , a_key);                                        <* 
    *> --rce;  if (a_key == NULL) {                                                             <* 
    *>    DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                                       <* 
    *>    return rce;                                                                           <* 
    *> }                                                                                        <* 
    *> DEBUG_YMARK   yLOG_info    ("a_key"     , a_key);                                        <* 
    *> x_len = strlen (x_key);                                                                  <* 
    *> DEBUG_YMARK   yLOG_value   ("x_len"     , x_len);                                        <* 
    *> --rce;  if (x_len < 1) {                                                                 <* 
    *>    DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                                       <* 
    *>    return rce;                                                                           <* 
    *> }                                                                                        <* 
    *> /+---(open)---------------------------+/                                                 <* 
    *> f = fopen (SEARCH_REPO, "rt");                                                           <* 
    *> DEBUG_YMARK   yLOG_point   ("f"         , f);                                            <* 
    *> --rce;  if (f == NULL)  {                                                                <* 
    *>    DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                                       <* 
    *>    return rce;                                                                           <* 
    *> }                                                                                        <* 
    *> /+---(read)---------------------------+/                                                 <* 
    *> --rce;  while (x_found == '-') {                                                         <* 
    *>    /+---(read)------------------------+/                                                 <* 
    *>    if (feof (f))                   break;                                                <* 
    *>    fgets (x_recd, LEN_RECD, f);                                                          <* 
    *>    ++c;                                                                                  <* 
    *>    /+---(filter)----------------------+/                                                 <* 
    *>    if (x_recd [0] == '\0')         continue;                                             <* 
    *>    if (x_recd [0] == '\n')         continue;                                             <* 
    *>    if (x_recd [0] == '#')          continue;                                             <* 
    *>    if (x_recd [0] == ' ')          continue;                                             <* 
    *>    if (strlen (x_recd) < 22)       continue;                                             <* 
    *>    /+---(check key)-------------------+/                                                 <* 
    *>    DEBUG_YMARK   yLOG_info    ("x_recd"    , x_recd);                                    <* 
    *>    p = strtok_r (x_recd, q, &r);                                                         <* 
    *>    if (p == NULL || strcmp (p, a_key) != 0)  continue;                                   <* 
    *>    /+---(found)-----------------------+/                                                 <* 
    *>    x_found = 'y';                                                                        <* 
    *>    p = strtok_r (NULL, q, &r);                                                           <* 
    *>    strltrim (p, ySTR_BOTH, LEN_RECD);                                                    <* 
    *>    break;                                                                                <* 
    *>    /+---(done)------------------------+/                                                 <* 
    *> }                                                                                        <* 
    *> /+---(close)--------------------------+/                                                 <* 
    *> fclose (f);                                                                              <* 
    *> DEBUG_YMARK   yLOG_value   ("c"         , c);                                            <* 
    *> DEBUG_YMARK   yLOG_char    ("x_found"   , x_found);                                      <* 
   *> --rce;  if (x_found != 'y') {                                                            <* 
      *>    DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                                       <* 
         *>    return rce;                                                                           <* 
         *> }                                                                                        <* 
         *> /+---(save)---------------------------+/                                                 <* 
         *> /+> rc = ymark_srch_full (a_abbr, p);                                              <*    <* 
         *>  *> DEBUG_YMARK   yLOG_value   ("full"      , rc);                                 <*    <* 
         *>  *> --rce;  if (rc < 0) {                                                          <*    <* 
            *>  *>    DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                             <*    <* 
               *>  *>    return rce;                                                                 <*    <* 
               *>  *> }                                                                              <+/   <* 
               *> /+---(complete)-----------------------+/                                                 <* 
               *> DEBUG_YMARK   yLOG_exit    (__FUNCTION__);                                               <* 
               *> return rc;                                                                               <*/
}

char         /*-> tbd --------------------------------[ ------ [ge.#M5.1C#.#7]*/ /*-[03.0000.013.L]-*/ /*-[--.---.---.--]-*/
yMARK_srch_direct       (uchar *a_string)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_len       =    0;
   char        x_abbr      =  '-';
   char        x_div       =  '-';
   uchar      *x_divs      = "#-+>])=!";
   /*---(header)-------------------------*/
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (!yMODE_operational (MODE_SEARCH)) {
      DEBUG_YMARK   yLOG_note    ("can not execute until operational");
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(defense)-----------------------*/
   DEBUG_YMARK   yLOG_point   ("a_string"  , a_string);
   --rce;  if (a_string == NULL) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rc;
   }
   /*---(parse)-------------------------*/
   x_len = strlen (a_string);
   DEBUG_YMARK   yLOG_value   ("x_len"     , x_len);
   --rce;  if (x_len <= 1) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_abbr  = a_string [0];
   DEBUG_YMARK   yLOG_char    ("x_abbr"      , x_abbr);
   x_div   = a_string [1];
   DEBUG_YMARK   yLOG_char    ("x_div"     , x_div);
   /*---(check dividers)-----------------*/
   DEBUG_YMARK   yLOG_info    ("x_divs"    , x_divs);
   --rce;  if (strchr (x_divs, x_div) == NULL)  {
      DEBUG_YMARK   yLOG_note    ("divider (x_div) not understood");
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check for shorts)---------------*/
   /*> --rce;  if (x_len == 2) {                                                                <* 
    *>    switch (x_div) {                                                                      <* 
    *>    /+> case '#' : rc = ymark_srch_unmark (x_abbr);        break;                   <+/   <* 
    *>    case '-' : rc = ymark_srch_export (x_abbr);        break;                             <* 
    *>    case '+' : rc = ymark_srch_import (x_abbr);        break;                             <* 
    *>    default  :                                                                            <* 
    *>               DEBUG_YMARK   yLOG_note    ("divider (x_div) not a 2-char version");       <* 
    *>               DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);                            <* 
    *>               return rce;                                                                <* 
    *>    }                                                                                     <* 
    *>    DEBUG_YMARK   yLOG_exit    (__FUNCTION__);                                            <* 
    *>    return rc;                                                                            <* 
    *> }                                                                                        <*/
   /*---(copy)---------------------------*/
   /*> --rce;  if (x_len == 3 && x_div == '>') {                                      <* 
    *>    rc = ymark_srch_copy   (x_abbr, a_string [2]);                              <* 
    *>    DEBUG_YMARK   yLOG_exit    (__FUNCTION__);                                  <* 
    *>    return rc;                                                                  <* 
    *> }                                                                              <*/
   /*---(move)---------------------------*/
   /*> --rce;  if (x_len == 3 && x_div == ']') {                                                <* 
    *>    rc = ymark_srch_copy   (x_abbr, a_string [2]);                                        <* 
    *>    /+> rc = ymark_srch_unmark (x_abbr);                                            <+/   <* 
    *>    DEBUG_YMARK   yLOG_exit    (__FUNCTION__);                                            <* 
    *>    return rc;                                                                            <* 
    *> }                                                                                        <*/
   /*---(swap)---------------------------*/
   /*> --rce;  if (x_len == 3 && x_div == ')') {                                                <* 
    *>    rc = ymark_srch_copy   (a_string [2], '.');                                           <* 
    *>    rc = ymark_srch_copy   (x_abbr      , a_string [2]);                                  <* 
    *>    rc = ymark_srch_copy   ('.'         , x_abbr);                                        <* 
    *>    /+> rc = ymark_srch_unmark ('.');                                               <+/   <* 
    *>    DEBUG_YMARK   yLOG_exit    (__FUNCTION__);                                            <* 
    *>    return rc;                                                                            <* 
    *> }                                                                                        <*/
   /*---(forcing)------------------------*/
   /*> --rce;  if (x_len >  2 && x_div == '=') {                                      <* 
    *>    rc = ymark_srch_full  (x_abbr, a_string + 2);                               <* 
    *>    DEBUG_YMARK   yLOG_exit    (__FUNCTION__);                                  <* 
    *>    return rc;                                                                  <* 
    *> }                                                                              <*/
   /*---(global)-------------------------*/
   --rce;  if (x_len >  2 && x_div == '!') {
      /*> rc = yMACRO_central    (x_abbr, a_string);                                  <*/
      DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
   return rc;
}



