#include "lexical.h"
#include "basevisitor.h"

int yyparse(BaseVisitor&);
int yyset_lineno(int);
int yylex_destroy();
extern FILE* yyin;

extern int lineno;

int lexical::generate(FILE* file, BaseVisitor& vis) {
    // Explicitly sets lineno to 1 (first line in lex). Subsequent calls start counting lines from 1 this way.
    yyset_lineno(1);
    lineno = 1;
    if (file)
        yyin = file;

    int parsed = yyparse(vis);
    yylex_destroy();
    return parsed;
}

int lexical::generate(const std::string& filename, BaseVisitor& vis) {
    FILE* file = std::fopen(filename.c_str(), "r");
    if (!file) {
        std::cerr << "Could not open " << filename << std::endl;
        return -1;
    }
    int parsed = generate(file, vis);
    fclose(file);
    return parsed;
}