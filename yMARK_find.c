/*============================---beg-of-source---============================*/ /*---(headers)---------------------------*/
#include    "yMARK.h"
#include    "yMARK_priv.h"



/*====================------------------------------------====================*/
/*===----                    memory allocation                        ----===*/
/*====================------------------------------------====================*/
static void  o___MEMORY__________o () { return; }

char
ymark_find_new          (uchar *a_label, char a_force, tFIND **r_new)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tFIND      *x_new       = NULL;
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
   /*---(default)------------------------*/
   *r_new = NULL;
   /*---(defense)------------------------*/
   DEBUG_SRCH   yLOG_point   ("a_label"   , a_label);
   --rce;  if (a_label == NULL || strlen (a_label) <= 0) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SRCH   yLOG_info    ("a_label"   , a_label);
   /*---(existing)-----------------------*/
   ymark_find_by_label (a_label, r_new);
   DEBUG_SRCH   yLOG_point   ("*r_new"    , *r_new);
   if (*r_new != NULL) {
      DEBUG_SRCH   yLOG_note    ("already found, increasing counter");
      if ((*r_new)->c < 255)  ++((*r_new)->c);
      DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
      return 1;
   }
   /*---(allocate)-----------------------*/
   while (x_new == NULL) {
      ++x_tries;
      x_new = (tFIND *) malloc (sizeof (tFIND));
      if (x_tries > 3)   break;
   }
   DEBUG_SRCH   yLOG_value   ("x_tries"   , x_tries);
   DEBUG_SRCH   yLOG_point   ("x_new"     , x_new);
   --rce;  if (x_new == NULL) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(wipe)---------------------------*/
   strlcpy (x_new->label, a_label, LEN_LABEL);
   x_new->u = x_new->x = x_new->y = x_new->z = 0;
   x_new->c = 1;
   /*---(into btree)---------------------*/
   rc = ySORT_hook (B_FIND, x_new, x_new->label, &x_new->btree);
   DEBUG_SRCH   yLOG_value   ("btree"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update)-------------------------*/
   rc = ySORT_prepare (B_FIND);
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
ymark_find_free         (tFIND **r_old)
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
   rc = ySORT_prepare (B_FIND);
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
/*===----                       program level                         ----===*/
/*====================------------------------------------====================*/
static void  o___PROGRAM_________o () { return; }

char
ymark_find_init         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_SRCH   yLOG_enter   (__FUNCTION__);
   /*---(find btree)---------------------*/
   rc = ySORT_btree (B_FIND, "finds");
   DEBUG_SRCH   yLOG_value   ("btree"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(purge)--------------------------*/
   rc = ymark_find_purge ();
   DEBUG_SRCH   yLOG_value   ("purge"     , rc);
   /*---(complete)-----------------------*/
   DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ymark_find_purge        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tFIND      *x_curr      = NULL;
   tFIND      *x_next      = NULL;
   /*---(header)-------------------------*/
   DEBUG_SRCH   yLOG_enter   (__FUNCTION__);
   /*---(walk-through)-------------------*/
   DEBUG_SRCH   yLOG_value   ("count"     , ySORT_count (B_FIND));
   rc = ySORT_by_cursor (B_FIND, YDLST_HEAD, &x_curr);
   DEBUG_PROG   yLOG_point   ("x_curr"     , x_curr);
   while (x_curr != NULL) {
      rc = ySORT_by_cursor (B_FIND, YDLST_NEXT, &x_next);
      DEBUG_PROG   yLOG_point   ("x_next"     , x_next);
      DEBUG_SRCH   yLOG_point   ("x_curr"    , x_curr);
      DEBUG_SRCH   yLOG_info    ("->label"   , x_curr->label);
      rc = ymark_find_free (&x_curr);
      x_curr = x_next;
   }
   /*---(check)--------------------------*/
   DEBUG_SRCH   yLOG_value   ("count"     , ySORT_count (B_FIND));
   --rce;  if (ySORT_count (B_FIND) > 0) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ymark_find_wrap         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_SRCH   yLOG_enter   (__FUNCTION__);
   /*---(purge)--------------------------*/
   rc = ymark_find_purge ();
   DEBUG_SRCH   yLOG_value   ("purge"     , rc);
   /*---(complete)-----------------------*/
   DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        searching                             ----===*/
/*====================------------------------------------====================*/
static void  o___SEARCH__________o () { return; }

int  ymark_find_count         (void)                          { return ySORT_count     (B_FIND); }
char ymark_find_by_label      (uchar *a_name, tFIND **r_find) { return ySORT_by_name   (B_FIND, a_name, r_find); }
char ymark_find_by_index      (int n, tFIND **r_find)         { return ySORT_by_index  (B_FIND, n, r_find); }
char ymark_find_by_cursor     (uchar a_dir, tFIND **r_find)   { return ySORT_by_cursor (B_FIND, a_dir, r_find); }

char*
ymark_find_list         (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =    0;
   char        rce         =  -10;
   tFIND      *x_curr      = NULL;
   char        x_entry     [LEN_LABEL];
   /*---(walk the list)------------------*/
   strlcpy (g_print, ",", LEN_RECD);
   ymark_find_by_cursor (YDLST_HEAD, &x_curr);
   while (x_curr != NULL) {
      if (i > 10)          break;
      if (x_curr->label != NULL)   sprintf  (x_entry, "%s,", x_curr->label);
      else                         strlcpy  (x_entry, "??", LEN_LABEL);
      strlcat    (g_print, x_entry, LEN_RECD);
      ymark_find_by_cursor (YDLST_NEXT, &x_curr);
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

char
yMARK_found             (uchar *a_label, ushort u, ushort x, ushort y, ushort z)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tFIND      *x_curr      = NULL;
   /*---(header)-------------------------*/
   DEBUG_SRCH   yLOG_enter   (__FUNCTION__);
   /*---(allocate)-----------------------*/
   rc = ymark_find_new  (a_label, '-', &x_curr);
   DEBUG_SRCH   yLOG_value   ("new"       , rc);
   --rce;  if (rc < 0) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(populate)-----------------------*/
   DEBUG_SRCH   yLOG_note    ("populate");
   x_curr->u       = u;
   x_curr->x       = x;
   x_curr->y       = y;
   x_curr->z       = z;
   /*---(complete)-----------------------*/
   DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yMARK_lost              (uchar *a_label)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tFIND      *x_curr      = NULL;
   /*---(header)-------------------------*/
   DEBUG_SRCH   yLOG_enter   (__FUNCTION__);
   /*---(existing)-----------------------*/
   ymark_find_by_label (a_label, &x_curr);
   DEBUG_SRCH   yLOG_point   ("x_curr"    , x_curr);
   --rce;  if (x_curr == NULL) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(allocate)-----------------------*/
   rc = ymark_find_free (&x_curr);
   DEBUG_SRCH   yLOG_value   ("free"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
   return 0;
}


