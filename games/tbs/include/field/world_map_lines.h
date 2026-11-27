#ifndef ALCHEMY_WORLD_MAP_LINES_H
#define ALCHEMY_WORLD_MAP_LINES_H

/* The World Map's tilt, drawn line by line (world_map_lines.c). */

#include "types.h"

struct WORLD_MAP_AFFINE_LINE;
struct VECTOR3;
struct WORLD_MAP_SCANLINE;

/* The world map camera's yaw, in 1/65536 turns, the scanline parameters were
   built for; fdWorldMapUpdateAffineLines rebuilds them when it changes. */
extern Uint32 ScanlineAngle;
/* The world map camera's pitch the scanline parameters were built for. */
extern Uint32 ScanlinePitch;

void fdWorldMapUpdateAffineLines(struct VECTOR3 *eye, struct VECTOR3 *target,
	struct WORLD_MAP_SCANLINE *line, struct WORLD_MAP_AFFINE_LINE *affine);

#endif
