/*============================----beg-of-source---============================*/
#ifndef yMARK
#define yMARK yes


typedef  unsigned char        uchar;




/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(internal)-------------*/
char*       yMARK_version           (void);
char        yMARK_init              (void);
char        yMARK_wrap              (void);
/*---(interface)------------*/
char        yMARK_config            (void *a_searcher, void *a_unsearcher, void *a_hinter, void *a_unhinter);
char        yMARK_direct            (uchar *a_command);
/*---(unittest)-------------*/
char*       yMARK__unit             (char *a_question, int n);



#endif
/*============================----end-of-source---============================*/
