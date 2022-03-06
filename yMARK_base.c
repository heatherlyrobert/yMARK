/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "yMARK.h"
#include    "yMARK_priv.h"


tMY         myMARK;





/*====================------------------------------------====================*/
/*===----                           utility                            ----===*/
/*====================------------------------------------====================*/
static void      o___UTILITY_________________o (void) {;}

char        yMARK_ver [200] = "";

char*        /*--> return library versioning info --------[ leaf-- [ ------ ]-*/
yMARK_version           (void)
{
   char        t [20] = "";
#if    __TINYC__ > 0
   strncpy (t, "[tcc built  ]", 15);
#elif  __GNUC__  > 0
   strncpy (t, "[gnu gcc    ]", 15);
#elif  __HEPH__  > 0
   strncpy (t, "[hephaestus ]", 18);
#else
   strncpy (t, "[unknown    ]", 15);
#endif
   snprintf (yMARK_ver, 100, "%s   %s : %s", t, P_VERNUM, P_VERTXT);
   return yMARK_ver;
}



/*====================------------------------------------====================*/
/*===----                      program level                           ----===*/
/*====================------------------------------------====================*/
static void      o___PROGRAM_________________o (void) {;}

char
yMARK_init               (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_YCMD   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (!yMODE_check_prep  (MODE_SEARCH)) {
      DEBUG_YCMD   yLOG_note    ("status is not ready for init");
      DEBUG_YCMD   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(terms)--------------------------*/
   DEBUG_YCMD   yLOG_note    ("initialize globals callbacks");
   myMARK.e_regex    = NULL;
   myMARK.e_unregex  = NULL;
   myMARK.e_hint     = NULL;
   myMARK.e_unhint   = NULL;
   /*---(other updates)------------------*/
   /*> rc = yFILE_dump_add ("cmds"      , "", "inventory of commands"       , ycmd_dump          );   <*/
   /*---(update status)------------------*/
   DEBUG_YCMD   yLOG_note    ("update status");
   yMODE_init_set   (MODE_SEARCH, NULL, ySRC_mode);
   /*---(complete)-----------------------*/
   DEBUG_YCMD   yLOG_exit    (__FUNCTION__);
   return 0;
}

/*> char                                                                                               <* 
 *> ycmd__purge             (void)                                                                     <* 
 *> {                                                                                                  <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                                        <* 
 *>    char        rce         =  -10;                                                                 <* 
 *>    int         i           =    0;                                                                 <* 
 *>    tLINK      *x_curr      = NULL;                                                                 <* 
 *>    tLINK      *x_next      = NULL;                                                                 <* 
 *>    /+---(header)-------------------------+/                                                        <* 
 *>    DEBUG_YCMD   yLOG_enter   (__FUNCTION__);                                                       <* 
 *>    /+---(defense)------------------------+/                                                        <* 
 *>    x_curr = myMARK.c_head;                                                                                <* 
 *>    /+---(clear)--------------------------+/                                                        <* 
 *>    DEBUG_YCMD   yLOG_value   ("c_ncmd"    , myMARK.c_ncmd);                                               <* 
 *>    while (x_curr != NULL) {                                                                        <* 
 *>       DEBUG_YCMD   yLOG_complex ("focus"     , "%c %s", x_curr->data->base, x_curr->data->name);   <* 
 *>       x_next = x_curr->m_next;                                                                     <* 
 *>       if (x_curr->data->base != CMDS_BASE) {                                                       <* 
 *>          free (x_curr->data);                                                                      <* 
 *>       }                                                                                            <* 
 *>       x_curr->data = NULL;                                                                         <* 
 *>       free (x_curr);                                                                               <* 
 *>       x_curr = NULL;                                                                               <* 
 *>       x_curr = x_next;                                                                             <* 
 *>    }                                                                                               <* 
 *>    /+---(initialize pointers)------------+/                                                        <* 
 *>    DEBUG_YCMD   yLOG_note    ("pointers");                                                         <* 
 *>    myMARK.c_head   = NULL;                                                                                <* 
 *>    myMARK.c_found  = NULL;                                                                                <* 
 *>    /+---(initialize counters)------------+/                                                        <* 
 *>    DEBUG_YCMD   yLOG_note    ("counters");                                                         <* 
 *>    myMARK.c_ncmd   = 0;                                                                                   <* 
 *>    myMARK.c_nbase  = 0;                                                                                   <* 
 *>    /+---(fields)-------------------------+/                                                        <* 
 *>    for (i = 0; i < 10; ++i)  myMARK.p_fields [i][0] = '\0';                                         <* 
 *>    s_nfield  = 0;                                                                                  <* 
 *>    myMARK.p_all [0] = '\0';                                                                         <* 
 *>    /+---(complete)-----------------------+/                                                        <* 
 *>    DEBUG_YCMD   yLOG_exit    (__FUNCTION__);                                                       <* 
 *>    return 0;                                                                                       <* 
 *> }                                                                                                  <*/

char
yMARK_wrap               (void)
{
   return  0;
}



char
yMARK_config            (void *a_regex, void *a_unregex, void *a_hint, void *a_unhint)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_SRCH  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (!yMODE_check_needs  (MODE_SEARCH)) {
      DEBUG_SRCH   yLOG_note    ("init must complete before config");
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update regex)-------------------*/
   DEBUG_SRCH   yLOG_point   ("a_regex"   , a_regex);
   --rce;  if (a_regex   == NULL) {
      DEBUG_SRCH   yLOG_note    ("without regex searcher callback, search can not function");
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   myMARK.e_regex   = a_regex;
   /*---(update unregex)-----------------*/
   DEBUG_SRCH   yLOG_point   ("a_unregex" , a_unregex);
   --rce;  if (a_unregex == NULL) {
      DEBUG_SRCH   yLOG_note    ("without regex clearer callback, search can not function");
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   myMARK.e_unregex = a_unregex;
   /*---(update status)------------------*/
   yMODE_conf_set   (MODE_SEARCH, '1');
   /*---(update hint)--------------------*/
   DEBUG_SRCH   yLOG_point   ("a_hint"    , a_hint);
   --rce;  if (a_hint    == NULL) {
      DEBUG_SRCH   yLOG_note    ("without hint marker callback, hinting can not function");
   } else {
      myMARK.e_hint    = a_hint;
   }
   /*---(update unhint)------------------*/
   DEBUG_SRCH   yLOG_point   ("a_unhint"  , a_unhint);
   --rce;  if (a_unhint  == NULL) {
      DEBUG_SRCH   yLOG_note    ("without hint clearer callback, hinting can not function");
   } else {
      myMARK.e_unhint  = a_unhint;
   }
   /*---(update status)------------------*/
   yMODE_conf_set   (SMOD_HINT  , '1');
   /*---(complete)-----------------------*/
   DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
   return 0;
}


