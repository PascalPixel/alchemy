#include "callback_scheduler.h"
#include "resource.h"
#include "types.h"

struct SlotEntry {
  struct SlotEntry *next;
  u8 y;
  u8 affine : 2;
  u8 mode : 2;
  u8 mosaic : 1;
  u8 colors : 1;
  u8 shape : 2;
  u16 x : 9;
  u16 param : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 prio : 2;
  u16 pal : 4;
};
struct MenuNode {
  s32 unk00;
  struct MenuNode *next;
  u16 offset;
  u16 active;
  u16 handle;
  u16 tileId;
  s16 x, y, dx, dy, xEnd, yEnd;
  u8 unk1c[6];
  s16 scale, scaleStep, scaleEnd;
  struct SlotEntry entry;
};
struct MenuSelection {
  struct MenuNode nodes[2];
  u8 unk68[0x27a];
  u16 mode;
};
extern u32 gVBlankCount;
extern u8 gData_0805c5c4[], gData_0805c1c4[];
extern s32 GameFlag_Test(s32 flag);

void Func_0803e488(struct MenuSelection *state, s32 index) {
  u32 frame = (gVBlankCount >> 2) & 7;
  struct SlotEntry *entry;
  u8 *frames;
  u8 *tmp;

  if (state->nodes[index].active == 0)
    return;
  entry = &state->nodes[index].entry;
  entry->x = state->nodes[index].x;
  entry->y = state->nodes[index].y;
  if (index != 0) {
    frames = gData_0805c5c4;
    if (state->nodes[1].offset != 0)
      entry->x = entry->x + state->nodes[1].offset;
  } else {
    frames = gData_0805c1c4;
    if (state->nodes[0].offset != 0)
      entry->x = entry->x - state->nodes[0].offset;
  }
  tmp = frames + frame * 128;
  entry->tile = VramCache_Load(state->nodes[index].handle, 128, tmp);
  if (GameFlag_Test(0x103)) {
    if (state->mode == 1)
      entry->mode = 1;
    else
      entry->mode = 0;
  }
  Runtime_PushSlotEntry((s32 *)entry, 238);
  if (state->nodes[index].offset != 0)
    state->nodes[index].offset--;
}
