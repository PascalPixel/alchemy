#ifndef ALCHEMY_OBJECT_LOOKUP_H
#define ALCHEMY_OBJECT_LOOKUP_H

#if GAME_TLA
#include "object_runtime.h"
#define ObjectTable_Get ((void *(*)())ObjectTable_Get)
#else
#endif

#endif
