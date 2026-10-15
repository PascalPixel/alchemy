#include "global_cells.h"
#ifndef ALCHEMY_MAP_RENDER_WORK_H
#define ALCHEMY_MAP_RENDER_WORK_H

#include "types.h"
#include "layout_guard.h"

struct MapRenderWork {
  u8 unk000[0xfc];
  s8 active;
  u8 unk0fd[3];
  s16 first;
  s16 second;
  u8 unk104[0x16];
  u16 rotation;
};

LAYOUT_OFFSET_GUARD(MapRenderWork_Active, struct MapRenderWork, active, 0xfc);
LAYOUT_OFFSET_GUARD(MapRenderWork_Window, struct MapRenderWork, first, 0x100);
LAYOUT_OFFSET_GUARD(MapRenderWork_Rotation, struct MapRenderWork, rotation,
                    0x11a);

#endif
