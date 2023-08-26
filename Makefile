#================================[[ beg-code ]]================================#



#===(manditory)============================================#
# basename of executable, header, and all code files
NAME_BASE  = yMARK
# additional standard and outside libraries
LIB_STD    = 
# all heatherly libraries used, debug versions whenever available
LIB_CORE   = -lyLOG_debug   -lyURG_debug   -lySTR_debug
LIB_VIKEYS = -lyMODE_debug  -lyKEYS_debug  -lySRC_debug
LIB_OTHER  = -lyPARSE_debug -lyREGEX_debug -lySORT_debug
LIB_GRAPH  = -lyVIHUB_debug
LIB_MINE   = $(LIB_CORE)   $(LIB_VIKEYS)   $(LIB_OTHER)   $(LIB_GRAPH) 
# directory for production code, no trailing slash
INST_DIR   = 



#===(optional)=============================================#
# extra include directories required
INC_MINE   = 
# utilities generated, separate from main program
NAME_UTIL  = 
# libraries only for the utilities
LIB_UTIL   = 



#===(post-processing)======================================#
# create a rule for...
#install_prep       :
#remove_prep        :



#===(master template)======================================#
include /usr/local/sbin/make_program



#===(post-processing)======================================#
# create a rule for...
#install_post       :
#remove_post        :



#================================[[ end-code ]]================================#
