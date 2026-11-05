// Draft, differ 554 of 961, skeleton gap 120 (tla-en, 2026-11-01): one stage of
// the staff credits, from the camera pan through the tilemap and palette pushes
// to the fade out; stage 4 grows actor 8 and swaps its pictures instead.
// Left, most of it one cause: frame sits in r10 (native r7) and the buffer in r9
// (native r10), about thirty extra moves and most of the differ. The native's
// pair loop holds bx r0, base r4, x r5, i r6, frame r7 with row ip, rowBase lr,
// by r9, so frame outranks row. Here, from --rtl, frame is 0.47 against row 1.7,
// bx 1.7, i 1.5 and base 2.4 because flow.c weights refs by loop depth; no
// sharing reaches it (row merged with y is still 1.4, the pan loop in frame 0.6,
// and the buffer would need about 64 refs, it has 41, to take r10 first).
// GCSE's PRE hoists y << 4 and y << 5 out of the tile loops and the pan table
// address out of the pan loop; the native recomputes them and reuses that
// address only inside the loop, as classic GCSE would. alchemy-gcc's gcse.c
// takes classic GCSE, and flow.c unweighted refs, only under optimize_size. That
// is a compiler and option question for Pascal (K3), out of scope here.
// Closed this round (680 to 554): the camera words as struct fields give movs
// without locals; the stage 4 pushes use frames, with grid set after them; the
// real return types of Task_Add and Func_02001f54; u16 tile; the fade as a for
// loop. u16 tile brings the tile loop's registers level with the native (38
// fewer differ) but adds two extension instructions it lacks; written s32 or
// inline, tile takes r4, because the hoisted shifts hold the registers.
// Forms that steer allocation, kept and still measured necessary: grid is
// function scope, set for stage 4 and read by its frame swap, which keeps it
// live round the credits loop so it loses its register (the native rebuilds
// buffer + 0x2000 there; split, stage goes to the stack, differ 608 against 596
// then, gap 152 against 120). The last camera call reads palette, equal to stage
// on that path, or PRE carries stage * 4 across the function (about 90 worse);
// CSE then reads the stage == 4 test from palette's stack slot. The native's
// frame swap has no grid, and (x * 2 + (base + i)) is its grouping, but PRE
// hoists base + i (gap 172).
// Tried, no gain: without the camera inline, frame sharing the pan and fade
// loops, i in y or its own j, default-0 scale, a cast to swap offset + buffer
// (a disguise; offset + buffer folds back), late inline, count as a local, for
// forms of the tile, fill, credits, blank, pixel row and pair loops (the blank
// and row ones lower differ by 9 and 3 but raise the gap by 13 and 7).
#include "types.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_work.h"
#include "bg_scroll.h"

extern u16 gData_02003dbc;
extern u16 gData_02003dbe;
extern s16 gData_02003d20[][2];
extern s16 gData_02003d34[][2];
extern s16 gData_02003d48[][2];
extern s32 gData_02003d5c[];
extern s32 gData_02003d70[];
extern u8 gData_02003d84[];
extern u8 gData_02003d88[];
extern u16 gData_02003d8e[];
extern s8 gData_02003d9c[][2];
extern u8 Data_020020bc[];
extern u8 Data_0200213c[];
extern u8 gData_0200215c[];
extern u8 Data_0200217c[];
extern u8 Data_02002469[];

void wait(s32 frames);
void GameFlag_ClearBit(s32 flag);
void GameFlag_SetBit(s32 flag);
struct Actor *Actor_GetById(s32 id);
void Func_02002034(s32 id, s32 x, s32 z);
void *Heap_AllocEwram(s32 size);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 z, s32 mode) {
  Camera_MoveWithinMapBounds(x, -1, z, mode);
}
void Func_02001fdc(void);
void Func_0200208c(s32 value, s32 frames);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02002074(void);
// The veneer of Resource_DecodeType01Far.
s32 Func_02001f54(const void *source, void *destination);
void Func_02002024(void);
void Func_02002084(s32 value);
s32 Task_Add(void (*func)(void), s32 order);
void Task_RemoveFar(void (*func)(void));
void Func_0200005c(void);
void Func_020020b4(s32 cue);
void Animation_SetStateFlags(struct Actor *object, s32 flags);
void Func_020000c0(u16 *colors, s32 count, s32 red, s32 green, s32 blue);
void Func_02000138(u8 *grid, s32 count, s32 scale);
void Func_02002044(s32 a, s32 b);
void Func_0200207c(void);
void Func_02001f74(void);
void Heap_Release(void *pointer);

struct WorldCamera {
  u8 unk000[0x11c];
  u16 unk11c;
  u16 yaw;
};

void Func_0200020c(s32 stage) {
  struct WorldCamera *map = (struct WorldCamera *)Ram_HeapSlots->mapWork;
  s32 palette;
  u8 *buffer;
  u8 *grid;
  s32 x;
  s32 y;
  s32 i;
  s32 frame;

  Func_02002034(8, 0, 0);
  gData_02003dbe = 0;
  wait(1);
  GameFlag_ClearBit(0x109);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0;
  palette = stage;
  buffer = Heap_AllocEwram(0x6000);
  Actor_SetSingleChildFlags(Actor_GetById(8), 0);
  if (stage == 4)
    palette = 0;
  gBgScroll[0].y = 0;
  gBgScroll[0].x = 0;
  Camera_MoveWithinMapBounds_(gData_02003d20[stage][0] << 16,
                              gData_02003d20[stage][1] << 16, 0);
  Func_02001fdc();
  Func_0200208c(gData_02003d5c[stage], 1);
  map->yaw = 0x1000;
  map->unk11c = 0xd000;
  Iwram_ClearWords((void *)0x050001c0, 32);
  wait(1);
  Actor_GetById(gMainRoutineWork.currentOwner)->animationKind = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->delay = 64;
  Event_StartScreenTransition();
  Camera_MoveWithinMapBounds_(gData_02003d34[stage][0] << 16,
                              gData_02003d34[stage][1] << 16, 1);
  for (i = 0; i != gData_02003d84[stage]; i++) {
    {
      s32 length = gData_02003d84[stage];

      map->yaw = (length - i) << 5;
      if (i == length - 59)
        Func_0200208c(gData_02003d70[stage], 60);
    }
    if (i >= gData_02003d84[stage] - 64)
      // Pointer arithmetic folds gData_02003d88 + 1 into one literal, as the
      // native does; indexed as [stage + 1] the 1 is added at run time.
      map->unk11c += *(gData_02003d88 + stage + 1);
    wait(1);
  }
  Func_02002074();
  if (stage == 4) {
    u8 *frames = buffer + 0x2000;

    Func_02001f54(Data_02002469, buffer);
    Func_02001f54(Data_0200217c, frames);
    IO_WRITE_QUEUE_PUSH(0x06000000, (u32)buffer, 0x84000800);
    IO_WRITE_QUEUE_PUSH(0x06002000, (u32)frames, 0x84000140);
    grid = frames;
    wait(1);
  } else {
    u16 *tiles = (u16 *)buffer;

    IO_WRITE_QUEUE_PUSH(0x050001c0, (u32)(Data_020020bc + palette * 32),
                        0x80000010);
    Iwram_ClearWords((void *)0x06001000, 0x2000);
    wait(1);
    // goto loops: as for loops the gap rises by 92.
    y = 0;
  tileRow:
    x = 0;
  tile:
    {
      u16 tile = y * 16 + x + 128;

      tiles[y * 32 + x] = tile | (s16)0xe000;
      tiles[y * 32 - x + 31] = tile | (s16)0xe400;
      tiles[(15 - y) * 32 + x] = tile | (s16)0xe800;
      tiles[(15 - y) * 32 - x + 31] = tile | (s16)0xec00;
    }
    x++;
    if (x != 16)
      goto tile;
    y++;
    if (y != 8)
      goto tileRow;
    y = 0;
  fillRow:
    x = 0;
  fill:
    tiles[y * 32 + x + 0x200] = 0xe080;
    x++;
    if (x != 32)
      goto fill;
    y++;
    if (y != 16)
      goto fillRow;
    IO_WRITE_QUEUE_PUSH(0x06002000, (u32)buffer, 0x80000400);
    wait(1);
  }
  Func_02002024();
  Func_02002084(0);
  Task_Add(Func_0200005c, 0x480);
  Func_020020b4(246);
  // A goto loop: the native reloads every address inside it; as a for loop,
  // 810 of 961 differ.
  frame = 0;
credits:
  if (frame == 0) {
    if (stage == 4) {
      gBgScroll[0].x = 8;
      gBgScroll[0].y = frame;
      IO_WRITE_QUEUE_PUSH(0x050001c0, (u32)Data_0200213c, 0x80000010);
      IO_WRITE_QUEUE_PUSH(0x050001e0, (u32)gData_0200215c, 0x80000010);
    } else {
      gBgScroll[0].x = gData_02003d9c[stage][0];
      gBgScroll[0].y = gData_02003d9c[stage][1];
      IO_WRITE_QUEUE_PUSH(0x050001c0, (u32)(Data_020020bc + palette * 32),
                          0x80000010);
    }
  }
  if (stage == 4) {
    Func_02002034(8, (gData_02003d34[4][0] << 16) - 0x320000,
                  (gData_02003d34[4][1] << 16) - 0x1e0000);
    Animation_SetStateFlags(Actor_GetById(8), 1);
    Actor_SetModeById(8, 0);
    Actor_GetById(8)->scaleX = 0;
    Actor_GetById(8)->scaleY = 0;
    if (frame > 22) {
      s32 scale = (frame << 10) - 0x5400;

      if (scale > 0x10000)
        scale = 0x10000;
      Actor_GetById(8)->scaleX = scale;
      Actor_GetById(8)->scaleY = scale;
    }
    if (frame <= 31) {
      s32 step = frame / 4;

      if (step <= 6)
        IO_WRITE_QUEUE_PUSH(0x06002000, (u32)(grid + step * 0x500), 0x84000140);
      gData_02003dbc = 0xa00;
    }
    if (frame == 32) {
      gData_02003dbe = 1;
      gData_02003dbc = 0xa00;
    }
    if (frame == 36)
      gData_02003dbc = 0xb00;
    if (frame == 40)
      gData_02003dbc = 0xc00;
    if (frame == 44)
      gData_02003dbc = 0xd00;
    if (frame == 48)
      gData_02003dbc = 0xe00;
    if (frame == 52)
      gData_02003dbc = 0xf00;
    if (frame == 56)
      gData_02003dbc = 0x1000;
    if (frame == 70)
      IO_WRITE_QUEUE_PUSH(0x06002000, (u32)(buffer + 0x4300), 0x84000140);
    if (frame == 72)
      IO_WRITE_QUEUE_PUSH(0x06002000, (u32)(buffer + 0x4800), 0x84000140);
    if (frame == 74)
      IO_WRITE_QUEUE_PUSH(0x06002000, (u32)(buffer + 0x4d00), 0x84000140);
    if (frame == 76)
      IO_WRITE_QUEUE_PUSH(0x06002000, (u32)(buffer + 0x5200), 0x84000140);
    if (frame > 153) {
      gData_02003dbc = 0;
      Func_020000c0((u16 *)0x05000000, 256, -2, -2, -2);
      Func_020000c0((u16 *)0x05000200, 256, -2, -2, -2);
    } else if (frame > 79 && (frame & 1)) {
      Func_020000c0((u16 *)0x05000000, 256, 1, 1, 1);
      Func_020000c0((u16 *)0x05000200, 256, 1, 1, 1);
    }
  } else if (frame >= 0) {
    grid = buffer + 0x2000;

    if (frame + 8 <= 63) {
      s32 offset = 0;
      s32 by, bx, row;
      s32 rowBase;
      s32 base;
      u8 *out;

      Func_02000138(grid, frame + 8, 0x10000);
      // goto loops: the native jumps to the block-row and block tests and
      // recomputes grid + x * 2 + base + i in every pass; with the pair loop
      // as a for, differ 623 and gap 169.
      by = 0;
      rowBase = 0;
      goto blockRowTest;
    blockRow:
      out = buffer + offset;
      bx = 0;
    blank:
      *out = 0x11;
      bx++;
      offset++;
      out++;
      if (bx != 256)
        goto blank;
      base = rowBase * 8;
      bx = 0;
      goto blockTest;
    block:
      row = 0;
      i = 0;
    pixelRow:
      out = buffer + offset;
      x = 0;
    pixelPair:
      {
        u8 *pair = grid + (x * 2 + base + i);

        x++;
        *out = pair[1] << 4;
        *out |= pair[0];
        offset++;
        out++;
      }
      if (x != 4)
        goto pixelPair;
      row++;
      i += 64;
      if (row != 8)
        goto pixelRow;
      base += 8;
      bx++;
    blockTest:
      if (bx != 8)
        goto block;
      rowBase += 64;
      by++;
    blockRowTest:
      if (by != 8)
        goto blockRow;
      IO_WRITE_QUEUE_PUSH(0x06001000, (u32)buffer, 0x84000400);
    }
  }
  if (stage != 4) {
    u32 late = frame - 32;

    gData_02003dbc = frame <= 6 ? gData_02003d8e[frame % 6] : 0x1000;
    gData_02003dbc = 0x1000;
    if (frame == 138)
      gData_02003dbe = 1;
    if (frame > 64 && (frame & 1))
      Func_020000c0((u16 *)0x050001c0, 16, 1, 1, 1);
    if (frame > 140)
      Func_020000c0((u16 *)0x050001c0, 16, -2, -2, -2);
    if (late <= 95)
      gBgScroll[0].y++;
  }
  frame++;
  wait(1);
  if (frame != 172)
    goto credits;
  Task_RemoveFar(Func_0200005c);
  if (stage != 4) {
    Func_02002044(0x60000, 0xc000);
    Func_0200207c();
    Camera_MoveWithinMapBounds_(gData_02003d48[palette][0] << 16,
                                gData_02003d48[palette][1] << 16, 1);
    Func_0200208c(0xb333, 60);
    for (y = 0; y != 124;) {
    map->yaw += 0xffe0;
    if (y == 60) {
      ((struct EventWork *)Ram_HeapSlots->eventWork)->delay = 64;
      Event_FinishScreenTransition();
    }
    y++;
    wait(1);
    }
    Event_WaitTransitionFrames();
  } else {
    gBgScroll[0].x = 0;
    gBgScroll[0].y = 0;
  }
  Heap_Release(buffer);
  Func_02001f74();
  QueueIoWriteDelay2(0x04000052, gData_02003dbc);
  QueueIoWriteDelay2(0x04000050, 0);
  GameFlag_SetBit(0x101);
}
