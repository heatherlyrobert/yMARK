/*============================---beg-of-source---============================*/ /*---(headers)---------------------------*/
#include    "yMARK.h"
#include    "yMARK_priv.h"




/*====================------------------------------------====================*/
/*===----                     support functions                       ----===*/
/*====================------------------------------------====================*/
static void  o___SUPPORT_________o () { return; }

char
ymark_to_sort           (char a_label [LEN_LABEL], char r_sort [LEN_TITLE])
{
   char        rce         =  -10;
   char        rc          =    0;
   int         u, x, y;
   char        x_sort      [LEN_TITLE] = "";
   if (r_sort != NULL)  strcpy (r_sort, "");
   --rce;  if (a_label == NULL)  return rce;
   --rce;  if (r_sort  == NULL)  return rce;
   rc = ystr2gyges (a_label, &u, &x, &y, NULL, NULL, 0, YSTR_ADAPT);
   if (rc < 0)   ystrlcpy (x_sort, a_label, LEN_TITLE);
   else          sprintf (x_sort, "%2d�%3d�%4d %s", u, x, y, a_label);
   ystrlcpy (r_sort, x_sort, LEN_TITLE);
   return 0;
}



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
   char        x_sort      [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   /*---(check return)-------------------*/
   DEBUG_YMARK   yLOG_point   ("r_new"     , r_new);
   --rce;  if (r_new == NULL) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YMARK   yLOG_point   ("*r_new"    , *r_new);
   --rce;  if (a_force != 'y' && *r_new != NULL) {
      DEBUG_YMARK   yLOG_note    ("pointer is already set, not in force mode");
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(default)------------------------*/
   *r_new = NULL;
   /*---(defense)------------------------*/
   DEBUG_YMARK   yLOG_point   ("a_label"   , a_label);
   --rce;  if (a_label == NULL || strlen (a_label) <= 0) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YMARK   yLOG_info    ("a_label"   , a_label);
   /*---(existing)-----------------------*/
   ymark_find_by_label (a_label, r_new);
   DEBUG_YMARK   yLOG_point   ("*r_new"    , *r_new);
   if (*r_new != NULL) {
      DEBUG_YMARK   yLOG_note    ("already found, increasing counter");
      if ((*r_new)->c < 255)  ++((*r_new)->c);
      DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
      return 1;
   }
   /*---(allocate)-----------------------*/
   while (x_new == NULL) {
      ++x_tries;
      x_new = (tFIND *) malloc (sizeof (tFIND));
      if (x_tries > 3)   break;
   }
   DEBUG_YMARK   yLOG_value   ("x_tries"   , x_tries);
   DEBUG_YMARK   yLOG_point   ("x_new"     , x_new);
   --rce;  if (x_new == NULL) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(populate)-----------------------*/
   ystrlcpy (x_new->label, a_label, LEN_LABEL);
   x_new->u = x_new->x = x_new->y = x_new->z = 0;
   x_new->c = 1;
   ymark_to_sort (x_new->label, x_new->sort);
   /*---(into btree)---------------------*/
   rc = ySORT_hook (B_FIND, x_new, x_new->sort, &x_new->btree);
   DEBUG_YMARK   yLOG_value   ("btree"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update)-------------------------*/
   rc = ySORT_prepare (B_FIND);
   if (rc < 0) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save return)--------------------*/
   *r_new = x_new;
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ymark_find_free         (tFIND **r_old)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_rc        =    0;
   /*---(header)-------------------------*/
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   /*---(check return)-------------------*/
   DEBUG_YMARK   yLOG_point   ("r_old"     , r_old);
   --rce;  if (r_old == NULL) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YMARK   yLOG_point   ("*r_old"    , *r_old);
   --rce;  if (*r_old == NULL) {
      DEBUG_YMARK   yLOG_note    ("pointer was never set");
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(handle unfind)-----------------*/
   DEBUG_YMARK   yLOG_point   ("e_unfind"  , myMARK.e_unfind);
   DEBUG_YMARK   yLOG_point   ("unit"      , ymark__unit_unfind);
   --rce;  if (myMARK.e_unfind != NULL) {
      /* must not fatal if unconfigured to allow initial purge */
      DEBUG_YMARK   yLOG_complex ("freeing"   , "%s, %3du, %3dx, %3dy, %3dz", (*r_old)->label, (*r_old)->u, (*r_old)->x, (*r_old)->y, (*r_old)->z);
      x_rc = myMARK.e_unfind ((*r_old)->label, (*r_old)->u, (*r_old)->x, (*r_old)->y, (*r_old)->z);
      DEBUG_YMARK   yLOG_value   ("x_rc"      , x_rc);
      --rce;  if (x_rc < 0) {
         DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(unhook from btree)--------------*/
   rc = ySORT_unhook (&(*r_old)->btree);
   DEBUG_YMARK   yLOG_value   ("btree"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update)-------------------------*/
   rc = ySORT_prepare (B_FIND);
   if (rc < 0) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(clear and return)---------------*/
   free (*r_old);
   *r_old = NULL;
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
   return x_rc;
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
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   /*---(find btree)---------------------*/
   rc = ySORT_btree (B_FIND, "finds");
   DEBUG_YMARK   yLOG_value   ("btree"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(purge)--------------------------*/
   rc = ymark_find_purge ();
   DEBUG_YMARK   yLOG_value   ("purge"     , rc);
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
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
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   /*---(walk-through)-------------------*/
   DEBUG_YMARK   yLOG_value   ("count"     , ySORT_count (B_FIND));
   rc = ySORT_by_cursor (B_FIND, YDLST_HEAD, &x_curr);
   DEBUG_YMARK   yLOG_point   ("x_curr"     , x_curr);
   while (x_curr != NULL) {
      rc = ySORT_by_cursor (B_FIND, YDLST_NEXT, &x_next);
      DEBUG_YMARK   yLOG_point   ("x_next"     , x_next);
      DEBUG_YMARK   yLOG_point   ("x_curr"    , x_curr);
      DEBUG_YMARK   yLOG_info    ("->label"   , x_curr->label);
      rc = ymark_find_free (&x_curr);
      x_curr = x_next;
   }
   /*---(check)--------------------------*/
   DEBUG_YMARK   yLOG_value   ("count"     , ySORT_count (B_FIND));
   --rce;  if (ySORT_count (B_FIND) > 0) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ymark_find_wrap         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   /*---(purge)--------------------------*/
   rc = ymark_find_purge ();
   DEBUG_YMARK   yLOG_value   ("purge"     , rc);
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        searching                             ----===*/
/*====================------------------------------------====================*/
static void  o___SEARCH__________o () { return; }

int  ymark_find_count         (void)                          { return ySORT_count     (B_FIND); }
char ymark_find_by_index      (int n, tFIND **r_find)         { return ySORT_by_index  (B_FIND, n, r_find); }
char ymark_find_by_cursor     (uchar a_dir, tFIND **r_find)   { return ySORT_by_cursor (B_FIND, a_dir, r_find); }

char
ymark_find_by_label      (uchar *a_name, tFIND **r_find) 
{
   char        x_sort      [LEN_TITLE] = "";
   ymark_to_sort (a_name, x_sort);
   return ySORT_by_name   (B_FIND, x_sort, r_find);
}

char*
ymark_find_list         (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =    0;
   char        rce         =  -10;
   tFIND      *x_curr      = NULL;
   char        x_entry     [LEN_LABEL];
   /*---(save current point)-------------*/
   ySORT_push (B_FIND);
   /*---(walk the list)------------------*/
   ystrlcpy (g_print, ",", LEN_RECD);
   ymark_find_by_cursor (YDLST_HEAD, &x_curr);
   while (x_curr != NULL) {
      if (i > 10)          break;
      if (x_curr->label != NULL)   sprintf  (x_entry, "%s,", x_curr->label);
      else                         ystrlcpy  (x_entry, "??", LEN_LABEL);
      ystrlcat    (g_print, x_entry, LEN_RECD);
      ymark_find_by_cursor (YDLST_NEXT, &x_curr);
   }
   /*---(catch empty)--------------------*/
   if (strcmp (g_print, ",") == 0)   strcpy (g_print, ".");
   /*---(return current point)-----------*/
   ySORT_pop  (B_FIND);
   /*---(complete)-----------------------*/
   return g_print;
}

char
yMARK_find_status       (char a_size, short a_wide, char *a_list)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_on        =  '�';
   char        x_list      [LEN_RECD]  = "";
   int         c           =    0;
   tFIND      *x_curr      = NULL;
   char        x_label     [LEN_LABEL] = "�";
   /*---(defenses)-----------------------*/
   --rce;  if (a_list  == NULL)  return rce;
   /*---(get current)--------------------*/
   ymark_find_by_cursor (YDLST_CURR, &x_curr);
   if (x_curr != NULL)  ystrlcpy (x_label, x_curr->label, LEN_LABEL);
   /*---(status)-------------------------*/
   c = ymark_find_count ();
   if (c > 0) x_on = 'y';
   ystrlcpy (x_list, ymark_find_list (), LEN_RECD);
   snprintf (a_list, 200, "finds    %c  %-8.8s  %4d  %s", x_on, x_label, c, x_list);
   /*---(complete)-----------------------*/
   return 0;
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
   char        x_sort      [LEN_TITLE];
   /*---(header)-------------------------*/
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   /*---(allocate)-----------------------*/
   rc = ymark_find_new  (a_label, '-', &x_curr);
   DEBUG_YMARK   yLOG_value   ("new"       , rc);
   --rce;  if (rc < 0) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(populate)-----------------------*/
   x_curr->u = u;
   x_curr->x = x;
   x_curr->y = y;
   x_curr->z = z;
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yMARK_lost              (uchar *a_label)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tFIND      *x_curr      = NULL;
   char        x_sort      [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   /*---(create sort key)----------------*/
   ymark_to_sort (a_label, x_sort);
   /*---(existing)-----------------------*/
   ymark_find_by_label (x_sort, &x_curr);
   DEBUG_YMARK   yLOG_point   ("x_curr"    , x_curr);
   --rce;  if (x_curr == NULL) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(allocate)-----------------------*/
   rc = ymark_find_free (&x_curr);
   DEBUG_YMARK   yLOG_value   ("free"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        mode handling                         ----===*/
/*====================------------------------------------====================*/
static void  o___MODE____________o () { return; }

char
yMARK_find_hmode        (uchar a_major, uchar a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =   -1;
   tFIND      *x_find      = NULL;
   int         c           =    0;
   /*---(quick out)----------------------*/
   if (a_major != G_KEY_SPACE)              return 0;
   if (a_minor == 0)                        return 0;
   if (strchr ("[<.>]", a_minor) == NULL)   return 0;
   /*---(header)-------------------------*/
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   c = ymark_find_count ();
   DEBUG_YMARK   yLOG_value   ("c"         , c);
   --rce;  if (c <= 0)  {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   ymark_find_by_cursor (a_minor, &x_find);
   DEBUG_YMARK   yLOG_point   ("x_find"    , x_find);
   if (x_find == NULL) {
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yVIHUB_yMAP_jump (x_find->u, x_find->x, x_find->y, x_find->z);
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
   return 0;
}


