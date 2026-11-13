#ifndef ALCHEMY_MODULE_FIELD_WORLD_MAP_LINES_H
#define ALCHEMY_MODULE_FIELD_WORLD_MAP_LINES_H

#include "types.h"

struct BgAffine;
struct Vector3;
struct ScanVertex;

/// The world map camera's yaw, in 1/65536 turns, the scanline parameters were
/// built for; the vertex update rebuilds them when it changes.
extern u32 gScanlineAngle;
/// The world map camera's pitch the scanline parameters were built for.
extern u32 gScanlinePitch;

void Transform_UpdateVertices(struct Vector3 *eye, struct Vector3 *target,
                              struct ScanVertex *line, struct BgAffine *affine);

#endif
