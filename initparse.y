%{

// initparse.y
// Prints to stdout a more computer-friendly version of the initialization file

#include <stdio.h>
#include <stdlib.h>

void yyerror(char *);
int yylex(void);

void initpdata(char **);
void printparticle(char **);

char *pdata[9];

%}

%union {
  char *str;
}

%token PARTTOK LEFTBRACE RIGHTBRACE MASSTOK CHARGETOK RADIUSTOK XPTOK YPTOK ZPTOK XVTOK YVTOK ZVTOK EOLN EOFTOK

%token<str> NUMBERTOK

%type<str> pline

%start input

%%

input
: lines EOFTOK { YYACCEPT; }
;

lines
:
| lines particle
;
particle
: particleinit particlelines particlestop
| EOLN
| error { fprintf(stderr, "Error in parsing particle!\n"); }
;

particleinit
: PARTTOK EOLN LEFTBRACE EOLN { initpdata(pdata); }
;

particlelines
:
| particlelines pline
;

pline
: MASSTOK NUMBERTOK { pdata[0] = $2; }
| CHARGETOK NUMBERTOK { pdata[1] = $2; }
| RADIUSTOK NUMBERTOK { pdata[2] = $2; }
| XPTOK NUMBERTOK { pdata[3] = $2; }
| YPTOK NUMBERTOK { pdata[4] = $2; }
| ZPTOK NUMBERTOK { pdata[5] = $2; }
| XVTOK NUMBERTOK { pdata[6] = $2; }
| YVTOK NUMBERTOK { pdata[7] = $2; }
| ZVTOK NUMBERTOK { pdata[8] = $2; }
| EOLN
;

particlestop
: RIGHTBRACE EOLN { printparticle(pdata); }
;

%%

void yyerror(char *msg) {}

int main(void) {
  yyparse();
}

void initpdata(char **p) {
  int i;
  for (i = 0; i < 9; i++) {
    p[i] = NULL;
  }
}

void printparticle(char **p) {
  int i;
  for (i = 0; i < 8; i++) {
    if (p[i] == NULL) {
      fprintf(stderr, "ERROR: Unspecified parameter!\n");
      exit(1);
    }
    printf("%s\t", p[i]);
  }
  printf("%s\n", p[8]);
}
