#include "version.h"

#define stringify(s) _stringifyDo(s)
#define _stringifyDo(s) #s

const char *project() { return stringify(PIO_SRC_NAM); }
const char *version() { return stringify(PIO_SRC_TAG); }
const char *branch() { return stringify(PIO_SRC_BRH); }
const char *build() { return stringify(PIO_SRC_REV); }
const char *board() { return stringify(PIO_BOARD); }
