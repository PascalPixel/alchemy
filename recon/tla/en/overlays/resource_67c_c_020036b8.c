// Measured under -Os, 5 November 2026: differ 45 (tla-en); notes below may predate it.
// Draft, differ 168 of 193, skeleton gap 40 (tla-en, 2026-11-01)
// Advances Aqua Rock's two water streams one cell along the map: each stream
// follows the turn its cell's kind gives, leaves a splash on every open cell
// (255) it crosses, stops on a wall kind and counts the cells of the given kind
// it reaches; then it clears the splashes past the new count and opens the
// cells under those left.
//
// The open-cell tail now matches instruction for instruction: it is Func_02000b24's
// form (map->layers[0].cells, a cells local, &cells[x + z * 128], collisionCode
// and attributeB), which replaced raw offsets with no device.
// Left is allocation. The listing has splash r5, count r6, stream r7, state r4
// (saved round the call), i r8, again r10, turn r9, first r11. Here splash, state,
// again and first land; turn takes r7, stream r6, count r8 and i r9. Turn being
// low causes most of the skeleton gap: the pooled 3 (movs in the listing), the extra
// mov for each high state read, and the outer branch going blt plus b.
// Priorities, floor_log2(refs) * refs / live: splash 1.98, stream 1.09, turn
// 0.65 (21 refs, 130 live; 0.55 at best with turn = 0 first in its arm, since it
// cannot live longer than stream), state 0.479, count 0.478, again 0.45, i 0.385.
// The listing's order needs turn and again under i, which means 15 refs or fewer
// for each (floor_log2 drops a step at 16); turn has 7 references at weight 3.
// Count above stream cannot come from priority at all, so r6/r7 is unexplained:
// no hard-reg preference for r6 was found either. An s8 local for the table entry
// folds back into turn through CSE. A pin in a scratch copy changes too much else
// to tell whether the source is otherwise right.
// The listing's statement order is NOT what is written in the arms: there, again
// = 0 comes after the turn != 8 test in the second arm and last in the third and
// fourth (the b into `mov r10, r2` is cross-jumping), and again = 1 before the
// active store. Those orders raise again's priority and measure worse now
// (differ 177, gap 51 for arms two and four). Restore them first once turn is
// placed. The third arm alone in listing order gives gap 38 at differ 174. A u16
// turn (166) or a for loop for the clear (164) lowers differ, but the gap rises
// to 52 and 44.
// Also left: the first read of stream->x in the do body. PRE reuses the if's load
// and inserts one on the back edge, which adds a b.n and makes the loop
// multi-entry; the listing reloads. Only a different alias set for the test stops
// it (long), and that is a device in disguise, so it stays out.
// The two tail loops are label and goto: the for form lets loop.c hoist the 0
// and 128 out, which the listing keeps inside. Whether the source really had such
// loops is open.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"

struct Splash {
  s32 x;
  s32 y;
  s32 z;
  u16 active;
  u16 direction;
  u32 sprite[3];
};

struct Stream {
  s32 x;
  s32 y;
  s32 z;
  u16 unk0c;
  u16 direction;
};

struct Splashes {
  u16 resource;
  u16 count;
  struct Stream streams[2];
  struct Splash splashes[32];
};

extern struct Splashes gData_02005970;
extern const s8 gData_0200534c[];

void Vector_AddPolarOffsetFar(s32 length, s32 angle, s32 *position);
s32 Func_020036b8(s32 first, s32 target) {
  s32 reached = 0;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Splashes *state = &gData_02005970;
  s32 count = state->count;
  s32 again = 0;
  s32 turn;
  s32 i;
  struct Stream *stream = state->streams;
  struct Splash *splash;

  for (i = 0; i < 2; i++) {
    if (stream->x != 0) {
      splash = &state->splashes[state->count];
      do {
        struct MapCell *cells = map->layers[0].cells;
        struct MapCell *cell =
            &cells[(stream->x >> 20) + (stream->z >> 20) * 128];
        s32 kind = cell->collisionCode;

        if (kind == 255) {
          splash->x = stream->x;
          splash->z = stream->z;
          splash->y = stream->y;
          again = 1;
          splash->active = 1;
          splash++;
          state->count++;
          turn = 0;
        } else if (kind >= first && kind <= first + 7) {
          turn = gData_0200534c[(stream->direction << 3) + kind - first];
          again = 0;
          if (turn != 8)
            count = state->count;
        } else if (kind == target) {
          turn = 8;
          reached++;
          again = 0;
          count = state->count;
        } else if (kind == 0) {
          turn = 8;
          again = 0;
        }
        if (turn != 8) {
          stream->direction = (stream->direction + turn) & 3;
          Vector_AddPolarOffsetFar(0x100000, stream->direction << 14,
                                   &stream->x);
        }
      } while (again != 0);
      state->count = count;
    }
    stream++;
  }
  splash = &state->splashes[count];
  if (count < 32) {
    i = 32 - count;
  clear:
    if (splash->active != 0)
      splash->active = 0;
    splash++;
    if (--i != 0)
      goto clear;
  }
  splash = state->splashes;
  if (count != 0) {
    i = count;
  open:
    if (splash->active != 0) {
      struct MapCell *cells = map->layers[0].cells;
      struct MapCell *cell =
          &cells[(splash->x >> 20) + (splash->z >> 20) * 128];

      cell->collisionCode = 0;
      cell->attributeB |= 0x80;
    }
    splash++;
    if (--i != 0)
      goto open;
  }
  return reached;
}
