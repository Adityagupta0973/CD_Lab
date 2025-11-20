%{
#include <stdio.h>
#include <stdlib.h>
%}

/* Regular expression definitions */
%%
"if"            { printf("KEYWORD\t: %s\n", yytext); }
"else"          { printf("KEYWORD\t: %s\n", yytext); }
"while"         { printf("KEYWORD\t: %s\n", yytext); }
"int"           { printf("KEYWORD\t: %s\n", yytext); }
"float"         { printf("KEYWORD\t: %s\n", yytext); }

[0-9]+(\.[0-9]+)?   { printf("NUMBER\t: %s\n", yytext); }

[a-zA-Z_][a-zA-Z0-9_]*  { printf("IDENTIFIER\t: %s\n", yytext); }

"=="|"<="|">="|"!="|"="|"+"|"-"|"*"|"/"|"("|")"|";"|"{"|"}"  {
    printf("OPERATOR/PUNCTUATION\t: %s\n", yytext);
}

[ \t\n]+        ;   /* ignore whitespace and newlines */

.               { printf("UNKNOWN TOKEN\t: %s\n", yytext); }
%%

int main() {
    printf("Enter code (Ctrl+D to stop):\n\n");
    yylex();  /* call the lexer */
    return 0;
}

int yywrap() {
    return 1;
}

/* flex FILENAME ; gcc lex.yy.c ; a.exe */