#ifndef gml_parser_h
#define gml_parser_h

#include "gml_scanner.h"

union GML_pair_val {
    long integer;
    double floating;
    char* string;
    struct GML_pair* list;
};

struct GML_pair {
    char* key;
    GML_value kind;
    union GML_pair_val value;
    struct GML_pair* next;
};

struct GML_list_elem {
    char* key;
    struct GML_list_elem* next;
};

struct GML_stat {
    struct GML_error err;
    struct GML_list_elem* key_list;
};

struct GML_pair* GML_parser (FILE*, struct GML_stat*, int);

void GML_free_list (struct GML_pair*, struct GML_list_elem*);
void GML_print_list (struct GML_pair*, int);

#endif




