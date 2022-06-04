/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "yMARK.h"
#include    "yMARK_priv.h"



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void  o___UNIT_TEST_______o () { return; }

char           unit_answer [LEN_FULL];

char       /*----: set up program urgents/debugging --------------------------*/
ymark__unit_quiet       (void)
{
   int         x_narg       = 1;
   char       *x_args [20]  = {"yMARK_unit" };
   yMODE_init (MODE_MAP);
   yMODE_handler_setup ();
   yFILE_init ();
   yCMD_init  ();
   yKEYS_init           ();
   yMAP__unit_wander    ();
   ySRC_init            ();
   yMACRO_init          ();
   yMARK_init ();
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
ymark__unit_loud        (void)
{
   int         x_narg       = 1;
   char       *x_args [20]  = {"yMARK_unit" };
   yURG_logger   (x_narg, x_args);
   yURG_urgs     (x_narg, x_args);
   yURG_name  ("kitchen"      , YURG_ON);
   yURG_name  ("ystr"         , YURG_ON);
   yURG_name  ("yview"        , YURG_ON);
   yURG_name  ("ymode"        , YURG_ON);
   yURG_name  ("ycmd"         , YURG_ON);
   yURG_name  ("ysrc"         , YURG_ON);
   yURG_name  ("ymap"         , YURG_ON);
   yURG_name  ("ykeys"        , YURG_ON);
   yURG_name  ("ymark"        , YURG_ON);
   yURG_name  ("ysort"        , YURG_ON);
   DEBUG_YMARK  yLOG_info     ("yMARK"     , yMARK_version   ());
   yMODE_init (MODE_MAP);
   yMODE_handler_setup ();
   yFILE_init ();
   yCMD_init  ();
   yKEYS_init           ();
   yMAP__unit_wander    ();
   ySRC_init            ();
   yMACRO_init          ();
   yMARK_init ();
   return 0;
}

char       /*----: stop logging ----------------------------------------------*/
ymark__unit_end         (void)
{
   DEBUG_YMARK  yLOG_enter   (__FUNCTION__);
   yCMD_wrap    ();
   DEBUG_YMARK  yLOG_exit    (__FUNCTION__);
   yLOGS_end    ();
   return 0;
}

char ymark__unit_stub        (void) { return 0; }

char
ymark__unit_regex       (uchar a_not, uchar *a_search)
{
   DEBUG_YMARK  yLOG_enter   (__FUNCTION__);
   DEBUG_YMARK  yLOG_point   ("a_search"  , a_search);
   if (a_search == NULL) {
      DEBUG_YMARK  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   if      (strcmp ("/1st", a_search) == 0) {
      DEBUG_YMARK   yLOG_note    ("found /1st");
      yMARK_found ("0k11"     ,   0,  10,  10,   0);
      yMARK_found ("0p12"     ,   0,  15,  11,   0);
      yMARK_found ("3d6"      ,   3,   3,   5,   0);
   }
   else if (strcmp ("/2nd", a_search) == 0) {
      DEBUG_YMARK   yLOG_note    ("found /2nd");
      yMARK_found ("0k11"     ,   0,  10,  10,   0);
      yMARK_found ("0b3"      ,   0,   1,   2,   0);
      yMARK_found ("2b5"      ,   2,   1,   4,   0);
      yMARK_found ("Za1"      ,  35,   0,   0,   0);
   }
   else if (strcmp ("/another", a_search) == 0) {
      DEBUG_YMARK   yLOG_note    ("found /another");
      yMARK_found ("2b5"      ,   2,   1,   4,   0);
   } else {
      DEBUG_YMARK   yLOG_note    ("nothing found");
   }
   DEBUG_YMARK  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ymark__unit_unfind      (ushort u, ushort x, ushort y, ushort z)
{
   DEBUG_YMARK  yLOG_enter   (__FUNCTION__);
   DEBUG_YMARK  yLOG_exit    (__FUNCTION__);
   return 0;
}

char           unit_answer [LEN_FULL];

char*        /*-> unit test accessor -----------------[ light  [us.420.111.11]*/ /*-[01.0000.00#.Z]-*/ /*-[--.---.---.--]-*/
yMARK__unit             (char *a_question, int n)
{
   /*---(locals)-----------+-----+-----+-*/
   char        r           [LEN_FULL]  = "";
   char        s           [LEN_FULL]  = "";
   char        t           [LEN_FULL]  = "";
   int         i           =    0;
   /*---(preprare)-----------------------*/
   strcpy  (unit_answer, "MARK unit        : question not understood");
   /*---(selection)----------------------*/
   if      (strcmp (a_question, "point"          )   == 0) {
      snprintf (unit_answer, LEN_FULL, "MARK point       : regx %-10p  unre %-10p  hint %p", myMARK.e_regex, myMARK.e_unfind, myMARK.e_hint);
   }
   /*> else if (strcmp (a_question, "p_field"        )   == 0) {                                                                                                                                                              <* 
    *>    snprintf (unit_answer, LEN_FULL, "CMD p_field (%2d) :", myCMD.p_nfield);                                                                                                                                            <* 
    *>    for (i = 1; i < 7; ++i)  {                                                                                                                                                                                          <* 
    *>       sprintf (s, " %2då%-.8sæ", strlen (myCMD.p_fields [i]), myCMD.p_fields [i]);                                                                                                                                     <* 
    *>       sprintf (t, "%-13.13s", s);                                                                                                                                                                                      <* 
    *>       if (i < 6) strlcat (unit_answer, t, LEN_FULL);                                                                                                                                                                   <* 
    *>       else       strlcat (unit_answer, s, LEN_FULL);                                                                                                                                                                   <* 
    *>    }                                                                                                                                                                                                                   <* 
    *> }                                                                                                                                                                                                                      <* 
    *> else if (strcmp (a_question, "master"         )   == 0) {                                                                                                                                                              <* 
    *>    ycmd_exec_by_index (n, &x_curr);                                                                                                                                                                                    <* 
    *>    if (x_curr == NULL)  snprintf (unit_answer, LEN_FULL, "CMD master  (%2d) : -   -åæ                      -åæ1234  -åæ          %p", n, NULL);                                                                        <* 
    *>    else {                                                                                                                                                                                                              <* 
    *>       p = x_curr->data;                                                                                                                                                                                                <* 
    *>       sprintf (r, "%2då%-.20sæ", x_curr->nlen, p->name);                                                                                                                                                               <* 
    *>       sprintf (s, "%1då%-.4sæ" , x_curr->alen, p->abbr);                                                                                                                                                               <* 
    *>       sprintf (t, "%1då%-.8sæ" , x_curr->nterms, p->terms);                                                                                                                                                            <* 
    *>       snprintf (unit_answer, LEN_FULL, "CMD master  (%2d) : %c  %-24.24s  %-7.7s  %-11.11s  %p",                                                                                                                       <* 
    *>             n, p->base, r, s, t, p->f.v);                                                                                                                                                                              <* 
    *>    }                                                                                                                                                                                                                   <* 
    *> }                                                                                                                                                                                                                      <* 
    *> else if (strcmp (a_question, "m_count"        )   == 0) {                                                                                                                                                              <* 
    *>    snprintf (unit_answer, LEN_FULL, "CMD m_count      : %s", ycmd_menu_counts ());                                                                                                                                     <* 
    *> }                                                                                                                                                                                                                      <* 
    *> else if (strcmp (a_question, "m_entry"        )   == 0) {                                                                                                                                                              <* 
    *>    snprintf (unit_answer, LEN_FULL, "CMD m_entry (%2d) : %s", n, ycmd_menu_by_index (n));                                                                                                                              <* 
    *> }                                                                                                                                                                                                                      <* 
    *> else if (strcmp (a_question, "m_found"        )   == 0) {                                                                                                                                                              <* 
    *>    ycmd_menu_line (myCMD.m_found, t);                                                                                                                                                                                  <* 
    *>    snprintf (unit_answer, LEN_FULL, "CMD m_found      : %s", t);                                                                                                                                                       <* 
    *> }                                                                                                                                                                                                                      <* 
    *> else if (strcmp (a_question, "m_place"        )   == 0) {                                                                                                                                                              <* 
    *>    snprintf (unit_answer, LEN_FULL, "CMD m_place (%2d) :  %ca   %4dx   %4dy   %4dxo  %4dyo", n, myCMD.m_places [n].align, myCMD.m_places [n].x, myCMD.m_places [n].y, myCMD.m_places [n].xo, myCMD.m_places [n].yo);   <* 
    *> }                                                                                                                                                                                                                      <*/
   /*---(complete)-----------------------*/
   return unit_answer;
}

