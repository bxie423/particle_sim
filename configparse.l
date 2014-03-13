%{

// configparse.l
// parses a configuration file into a useful format

#include <stdio.h>
#include <stdlib.h>

#include "configparse.tab.h"

%}

%option noyywrap

number [-+]?[0-9]*\.?[0-9]+

%%

SIMTIME             { return SIMTIMETOK; }
TIMESTEP            { return TIMESTEPTOK; }
WRITEFREQ           { return WRITEFREQTOK; }
GRAVITY             { return GRAVTOK; }
BFIELD              { return BTOK; }
EFIELD              { return ETOK; }
X                   { return XTOK; }
Y                   { return YTOK; }
Z                   { return ZTOK; }
POS                 { return POSTOK; }
NEG                 { return NEGTOK; }
CONFIG              { return CONFIGTOK; }
GLOBAL              { return GLOBALTOK; }
LOCAL               { return LOCALTOK; }

{number}            { yylval.str = strdup(yytext); return NUMBERTOK; }

.                   {}
\n                  { return EOLN; }
<<EOF>>             { return EOFTOK; }

%%
