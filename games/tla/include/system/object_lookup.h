#ifndef ALCHEMY_OBJECT_LOOKUP_H
#define ALCHEMY_OBJECT_LOOKUP_H

/// @file
/// Actor_Lookup as the field Psynergy calls it: through a pointer with no
/// prototype, returning an untyped actor, so each argument is passed as the
/// caller holds it rather than converted to the u32 the declaration takes.

#include "field/actor.h"
#define Actor_Lookup ((void *(*)())Actor_Lookup)

#endif
