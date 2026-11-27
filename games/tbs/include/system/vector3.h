#ifndef ALCHEMY_VECTOR3_H
#define ALCHEMY_VECTOR3_H

// Vector3, a three-coordinate vector.
//
// A file of its own, which system.h includes in place, because the units built as ARM code include
// it without the rest of system.h: another inline seen before their functions would renumber their
// labels. Its types and work areas are in system/definitions.h and system/globals.h.

#include "system/definitions.h"
#include "system/globals.h"

// Returns the vector's three coordinates as an array. UB: transform functions index three
// coordinates from separate scalar members. UBFIX gives those functions a real array while
// preserving named components.
static inline const Sint32 *syVector3Coordinates(const VECTOR3 *vector)
{
#if FIX_POINTER_OUTSIDE_OBJECT
    return vector->coordinates;
#else
    return &vector->x;
#endif
}

#endif
