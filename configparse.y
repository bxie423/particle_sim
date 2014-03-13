%{

// configparse.y
// Prints to stdout a more computer-friendly version of the configuration file

#include <stdio.h>
#include <stdlib.h>

void yyerror(char *);
int yylex(void);

char *checkifnull(char *);

char *configarray[3]; // allows us to print the options in the right order
char *gravarray[3];
char *earray[3];
char *barray[3];

char *globalstr = "%s\t%s\t%s\t%s\n";
char *localstr = "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n";

%}

%union {
  char *str;
}

%token SIMTIMETOK TIMESTEPTOK WRITEFREQTOK GRAVTOK BTOK ETOK XTOK YTOK ZTOK POSTOK NEGTOK EOLN EOFTOK CONFIGTOK GLOBALTOK LOCALTOK

%token<str> NUMBERTOK

%type<str> configline globalline localline axis direction

%start input

%%

input
: lines EOFTOK { YYACCEPT; }
;

lines
: CONFIGTOK config globaltok global localtok local
;

globaltok
: GLOBALTOK { printf("%s\t%s\t%s\n", configarray[0], configarray[1], configarray[2]); }
;

localtok
: LOCALTOK { printf(globalstr, "G_GLOBAL", checkifnull(gravarray[0]), checkifnull(gravarray[1]) , checkifnull(gravarray[2])); 
             printf(globalstr, "E_GLOBAL", checkifnull(earray[0]), checkifnull(earray[1]), checkifnull(earray[2])); 
	     printf(globalstr, "B_GLOBAL", checkifnull(barray[0]), checkifnull(barray[1]), checkifnull(barray[2])); }
;

config
:
| config configline
| error { fprintf(stderr, "Error in parsing configuration section!\n"); }
;

configline
: SIMTIMETOK NUMBERTOK { configarray[0] = $2; }
| TIMESTEPTOK NUMBERTOK { configarray[1] = $2; }
| WRITEFREQTOK NUMBERTOK { configarray[2] = $2; }
| EOLN
;

axis
: XTOK { $$ = "XDIR"; }
| YTOK { $$ = "YDIR"; }
| ZTOK { $$ = "ZDIR"; }
;

direction
: POSTOK { $$ = "POS"; }
| NEGTOK { $$ = "NEG"; }
;

global
:
| global globalline
| error { fprintf(stderr, "Error in parsing global variable section!\n"); }
;

globalline
: GRAVTOK NUMBERTOK NUMBERTOK NUMBERTOK { gravarray[0] = $2; gravarray[1] = $3; gravarray[2] = $4; }
| ETOK NUMBERTOK NUMBERTOK NUMBERTOK { earray[0] = $2; earray[1] = $3; earray[2] = $4; }
| BTOK NUMBERTOK NUMBERTOK NUMBERTOK { barray[0] = $2; barray[1] = $3; barray[2] = $4; }
| EOLN
;


local
:
| local localline
;

localline 
: ETOK axis direction NUMBERTOK NUMBERTOK NUMBERTOK NUMBERTOK NUMBERTOK NUMBERTOK NUMBERTOK { printf(localstr, "E_LOCAL", $2, $3, $4, $5, $6, $7, $8, $9, $10); }
| BTOK axis direction NUMBERTOK NUMBERTOK NUMBERTOK NUMBERTOK NUMBERTOK NUMBERTOK NUMBERTOK { printf(localstr, "B_LOCAL", $2, $3, $4, $5, $6, $7, $8, $9, $10); }
| EOLN
| error { fprintf(stderr, "Error in parsing local variable section!\n"); }
;

%%

void yyerror(char *msg) {}

int main(void) {
  yyparse();
}

char *checkifnull(char *str) {
  char *ret = (str == NULL) ? "0" : str;
  return ret;
}
