/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "yMARK.h"
#include    "yMARK_priv.h"


tMY         myMARK;
char        g_print     [LEN_RECD];





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
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (!yMODE_check_prep  (MODE_SEARCH)) {
      DEBUG_YMARK   yLOG_note    ("status is not ready for init");
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(hook to yVIHUB)-----------------*/
   yVIHUB_from_yMARK (yMARK_find_hmode, yMARK_execute);
   /*---(terms)--------------------------*/
   DEBUG_YMARK   yLOG_note    ("initialize globals callbacks");
   rc = ymark_srch_init ();
   DEBUG_YMARK   yLOG_value   ("srch"      , rc);
   rc = ymark_find_init ();
   DEBUG_YMARK   yLOG_value   ("find"      , rc);
   myMARK.e_hint     = NULL;
   rc = ymark_mark_init ();
   DEBUG_YMARK   yLOG_value   ("mark"      , rc);
   /*---(update status)------------------*/
   /*> rc = yMODE_init_set   (MODE_SEARCH, NULL, ymark__unit_stub);                   <*/
   /*> DEBUG_YMARK   yLOG_value   ("mark"      , rc);                                 <*/
   rc = yMODE_init_set   (SMOD_HINT  , NULL, ymark__unit_stub);
   DEBUG_YMARK   yLOG_value   ("hint"      , rc);
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yMARK_init_after         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_YMARK   yLOG_enter   (__FUNCTION__);
   /*---(other updates)------------------*/
   rc = yVIHUB_yVIEW_switch_add ('s', "mark"   , "", yMARK_mark_status    , "details of visual selection"                );
   rc = yVIHUB_yCMD_add (YVIHUB_M_EDIT  , "mark"        , ""    , "a"    , ymark_mark_direct          , "" );
   rc = yVIHUB_yVIEW_switch_add ('s', "find"  , "" , yMARK_find_status    , "list of current search finds"               );
   rc = yPARSE_handler_max (UMOD_MARK    , "loc_mark"  , 7.1, "cL----------", -1, yMARK_mark_reader , yMARK_mark_writer_all  , "------------" , "a,label", "vikeys location marks"    );
   yMODE_after_set  (MODE_SEARCH);
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yMARK_wrap               (void)
{
   ymark_find_wrap ();
   ymark_srch_wrap ();
   return  0;
}

char
yMARK_config            (void *a_regex, void *a_unfind, void *a_hint)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_YMARK  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (!yMODE_check_needs  (MODE_SEARCH)) {
      DEBUG_YMARK   yLOG_note    ("init must complete before config");
      DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update regex)-------------------*/
   DEBUG_YMARK   yLOG_point   ("a_regex"   , a_regex);
   --rce;  if (a_regex   == NULL) {
      if (!yMODE_operational (MODE_SEARCH)) {
         DEBUG_YMARK   yLOG_note    ("without regex searcher callback, search can not function");
         myMARK.e_regex    = NULL;
         myMARK.e_unfind   = NULL;
         DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   } else {
      myMARK.e_regex   = a_regex;
   }
   /*---(update unregex)-----------------*/
   DEBUG_YMARK   yLOG_point   ("a_unfind"  , a_unfind);
   --rce;  if (a_unfind == NULL) {
      if (!yMODE_operational (MODE_SEARCH)) {
         DEBUG_YMARK   yLOG_note    ("without regex clearer callback, search can not function");
         myMARK.e_regex    = NULL;
         myMARK.e_unfind   = NULL;
         DEBUG_YMARK   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   } else {
      myMARK.e_unfind  = a_unfind;
   }
   /*---(update status)------------------*/
   if (a_regex != NULL && a_unfind != NULL) {
      yMODE_conf_set   (MODE_SEARCH, '1');
   }
   /*---(update hint)--------------------*/
   DEBUG_YMARK   yLOG_point   ("a_hint"    , a_hint);
   --rce;  if (a_hint    == NULL) {
      if (!yMODE_operational (SMOD_HINT)) {
         DEBUG_YMARK   yLOG_note    ("without hint marker callback, hinting can not function");
         myMARK.e_hint     = NULL;
      }
   } else {
      myMARK.e_hint    = a_hint;
      yMODE_conf_set   (SMOD_HINT  , '1');
   }
   /*---(complete)-----------------------*/
   DEBUG_YMARK   yLOG_exit    (__FUNCTION__);
   return 0;
}


