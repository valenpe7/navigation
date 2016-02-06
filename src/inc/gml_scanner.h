#ifndef gml_scanner_h
#define gml_scanner_h

#include <stdio.h>

#define INITIAL_SIZE 1024

typedef enum {
    GML_KEY, GML_INT, GML_DOUBLE, GML_STRING, GML_L_BRACKET, 
    GML_R_BRACKET, GML_END, GML_LIST, GML_ERROR
} GML_value; 


typedef enum {
    GML_UNEXPECTED, GML_SYNTAX, GML_PREMATURE_EOF, GML_TOO_MANY_DIGITS,
    GML_OPEN_BRACKET, GML_TOO_MANY_BRACKETS, GML_OK
} GML_error_value;


struct GML_error {
    GML_error_value err_num;
    int line;
    int column;
};

union GML_tok_val {
    long integer;
    double floating;
    char* string;
    struct GML_error err;
};

struct GML_token { 
    GML_value kind;
    union GML_tok_val value;
};

extern unsigned int GML_line;
extern unsigned int GML_column;

void GML_init ();

struct GML_token GML_scanner (FILE*);

#endif



