#ifndef ALCHEMY_UNIT_FIELD_VERTICES_H
#define ALCHEMY_UNIT_FIELD_VERTICES_H

#include "types.h"

struct BgAffine;
struct Vector3;
struct ScanVertex;

void Transform_UpdateVertices(struct Vector3 *eye, struct Vector3 *target,
                              struct ScanVertex *line, struct BgAffine *affine);

#endif
