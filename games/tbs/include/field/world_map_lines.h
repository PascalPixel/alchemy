#ifndef ALCHEMY_WORLD_MAP_LINES_H
#define ALCHEMY_WORLD_MAP_LINES_H

/// @file
/// The World Map's tilt, drawn line by line (world_map_lines.c).

#include "types.h"

struct WorldMapAffineLine;
struct Vector3;
struct WorldMapScanline;

/// The world map camera's yaw, in 1/65536 turns, the scanline parameters were
/// built for; WorldMap_UpdateAffineLines rebuilds them when it changes.
extern u32 gScanlineAngle;
/// The world map camera's pitch the scanline parameters were built for.
extern u32 gScanlinePitch;

void WorldMap_UpdateAffineLines(struct Vector3 *eye, struct Vector3 *target,
                                struct WorldMapScanline *line,
                                struct WorldMapAffineLine *affine);

#endif
