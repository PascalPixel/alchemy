#ifndef ALCHEMY_MERCURY_LIGHTHOUSE_H
#define ALCHEMY_MERCURY_LIGHTHOUSE_H

/* Includes script.h with FIELD_STAGED_ACTOR_IMPORTS defined, so a scene that
   includes this before any other header (mercury_lighthouse.c) gets the pushed
   actor imports. Defined after script.h, the name would switch nothing. */

#define FIELD_STAGED_ACTOR_IMPORTS
#include "field/script.h"

#endif
