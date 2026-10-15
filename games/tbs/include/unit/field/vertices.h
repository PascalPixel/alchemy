#ifndef ALCHEMY_UNIT_FIELD_VERTICES_H
#define ALCHEMY_UNIT_FIELD_VERTICES_H

#include "types.h"

struct BgAffine;
struct Position;
struct ScanVertex;

void Transform_UpdateVertices(
    struct Position *eye, struct Position *target, struct ScanVertex *vtx, struct BgAffine *out);

#endif
