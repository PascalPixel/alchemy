// Draft, differ 174 of 193, skeleton gap 44 (tla-en, 2026-11-01)
// Advances Aqua Rock's two water streams one cell along the map: each stream
// follows the turn its cell's kind gives, leaves a splash on every open cell
// (255) it crosses, stops on a wall kind and counts the cells of the given kind
// it reaches; then it clears the splashes past the new count and opens the
// cells under those left.
//
// Left is allocation. The listing has splash r5, count r6, stream r7, state r4
// (saved round the call), i r8, again r10, turn r9, first r11. Here splash,
// state, again and first land; turn takes r7, stream r6, count r8 and i r9.
// global.c ranks floor_log2(refs) * refs / live: splash 1.96, stream 1.09, turn
// 0.65, state 0.484, count 0.484, again 0.45, i 0.39, first 0.36. Live lengths
// double in update_equiv_regs where a pseudo's first set is a constant: state,
// stream, again, turn and i, but not count, whose first set is a load. Setting
// again early in each arm and count late is what put state ahead of again and
// count ahead of i; the arm order here comes from that, not from the listing.
// Next step: turn must fall below i. It has 7 references at weight 3, and 15 or
// fewer would do it. Setting turn = 0 first in its arm lengthens its life to 154
// (0.55) and measures the same. A temporary for the table entry is folded back
// by CSE, because turn lives longer. In a scratch copy with turn held in r9 the
// gap falls to 32, though state then takes r7, so turn is not all of it. Stream
// in r7 with count in r6 is unexplained: stream outranks count by more than 2,
// yet takes the later register. An s16 turn, u8 again and reordered declarations
// change nothing; an s8 turn is worse. The 3 is a pool load while turn is low.
// Also left: the first read of stream->x in the do body. PRE reuses the if's
// load and inserts one on the back edge, which adds a b.n, makes the loop
// multi-entry for loop.c and lengthens the outer branch to blt plus b; the
// listing reloads. Reading the test as long stops it, for a different alias set,
// and loses 9 instructions of differ and 1 of gap until allocation lands, so it
// is left out for now; retry it once turn is placed. Reading through
// state->streams[i] also stops it, but then the outer loop does not reverse.
// The active store comes before again = 1, or CSE gives the store again's
// register, which the listing does not.
// The two tail loops are label and goto: the for form lets loop.c hoist the 0
// and 128 out, which the listing keeps inside; the reversed counters are written
// out to match. Whether the source really had such loops is open.
#include "types.h"
#include "ram_buffer.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

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

void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *position);
s32 Func_020036b8(s32 first, s32 target) {
  s32 reached = 0;
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
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
        u8 *cells = FIELD_AT(map, u8 *, 312);
        s32 kind =
            cells[(((stream->x >> 20) + ((stream->z >> 20) << 7)) << 2) + 2];

        if (kind == 255) {          splash->x = stream->x;
          splash->z = stream->z;
          splash->y = stream->y;
          splash->active = 1;
          again = 1;
          splash++;
          state->count++;
          turn = 0;
        } else if (kind >= first && kind <= first + 7) {
          again = 0;
          turn = gData_0200534c[(stream->direction << 3) + kind - first];
          if (turn != 8)
            count = state->count;
        } else if (kind == target) {
          again = 0;
          turn = 8;
          reached++;
          count = state->count;
        } else if (kind == 0) {
          again = 0;
          turn = 8;
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
      u8 *cell = FIELD_AT(map, u8 *, 312) +
                 (((splash->x >> 20) + ((splash->z >> 20) << 7)) << 2);

      cell[2] = 0;
      cell[3] |= 128;
    }
    splash++;
    if (--i != 0)
      goto open;
  }
  return reached;
}
