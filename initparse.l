%{

// initparse.l
// parses an initialization file into a useful format

#include <stdio.h>
#include <stdlib.h>

#include "initparse.tab.h"

%}

%option noyywrap

number [-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?

%%

PARTICLE            { return PARTTOK; }
\{                  { return LEFTBRACE; }
\}                  { return RIGHTBRACE; }

MASS                { return MASSTOK; }
CHARGE              { return CHARGETOK; }
RADIUS              { return RADIUSTOK; }
XPOS                { return XPTOK; }
YPOS                { return YPTOK; }
ZPOS                { return ZPTOK; }
XVEL                { return XVTOK; }
YVEL                { return YVTOK; }
ZVEL                { return ZVTOK; }

{number}            { yylval.str = strdup(yytext); return NUMBERTOK; }

.                   {}
\n                  { return EOLN; }
<<EOF>>             { return EOFTOK; }

%%
