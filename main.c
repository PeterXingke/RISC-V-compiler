#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

typedef enum{
  TK_PUNCT,  //operator
  TK_NUM,  //number
  TK_EOF,  //eof
}TokenKind;

typedef struct Token Token;
struct Token{
  TokenKind Kind;
  Token *Next;
  int Val;
  char *Loc;
  int Len;
};
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                

static void error(char *Fmt, ...){
  va_list VA;
  //VA get Fmt all pars
  va_start(VA, Fmt);
  vfprintf(stderr,Fmt,VA);
  fprintf(stderr,"\n");
  va_end(VA);
  exit(1);
}

static bool equal(Token *Tok, char *Str){
  return memcmp(Tok->Loc,Str,Tok->Len) == 0 && Str[Tok->Len] == '\0';
}

static Token *skip(Token *Tok, char *Str){
  if(!equal(Tok, Str))
    error("expect '%s'",Str);
  return Tok->Next;
}

static int getNumber(Token * Tok){
  if(Tok->Kind != TK_NUM)
    error("expect a number");
  return Tok->Val;
}

static Token *newToken(TokenKind Kind, char *Start, char* End){
  Token *Tok = calloc(1,sizeof(Token));
  Tok->Kind = Kind;
  Tok->Loc = Start;
  Tok->Len = End - Start;
  return Tok;
}

//terminal analysis
static Token *tokenize(char *P){
  Token Head = {};
  Token *Cur = &Head;

  while(*P){
    if(isspace(*P)){
      ++P;
      continue;
    }

    if(isdigit(*P)){
      Cur->Next = newToken(TK_NUM,P,P);
      Cur = Cur->Next;
      const char* OldPtr = P;
      Cur->Val = strtoul(P,&P,10);
      Cur->Len = P - OldPtr;
      continue;
    }

    if(*P == '+' || *P == '-'){
      Cur->Next = newToken(TK_PUNCT,P,P+1);
      Cur = Cur->Next;
      ++P;
      continue;
    }

    error("invalid token: %c", *P);
  }
  Cur->Next = newToken(TK_EOF,P,P);
  return Head.Next;  

}


int main(int Argc, char ** Argv){
    if(Argc != 2){
        error("%s: invalid number of arguments", Argv[0]);
        return 1;
    }

    Token *Tok = tokenize(Argv[1]);

    char *P = Argv[1]; //P saved str, not const
    printf("  .globl main\n");
    printf("main:\n");
    printf("  li a0,%ld\n", strtol(P, &P, 10));//str, left, 10进制
    Tok = Tok->Next;
    
    while(Tok->Kind != TK_EOF){
      if(equal(Tok,"+")){
        Tok = Tok->Next;
        printf("  addi a0,a0,%d\n", getNumber(Tok));
        Tok = Tok->Next;
        continue;
      }
      //skip -
      Tok = skip(Tok,"-");
      printf("  addi a0,a0,-%d\n", getNumber(Tok));
      Tok = Tok->Next;
      
    }
		printf("  ret\n");
    
    return 0;
  
  }



