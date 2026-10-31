// Near miss, differ 788 of 961 (tla-en, 2026-10-31): one stage of the staff
// credits, from the camera pan through the tilemap and palette pushes to the
// fade out; stage 4 grows actor 8 and swaps its pictures instead. The calls,
// branches, loops and accesses follow the native; what is left is register
// allocation and GCSE. The native keeps stage in r11, the buffer in r10 and
// the frame in r7 with a 16-byte frame; here stage, the buffer, the palette and
// stage * 4 sit on the stack (24 bytes) and the frame takes r9, which moves
// every pixel-loop variable. GCSE's PRE keeps stage * 4 from the first camera
// call to the last, inserts the gData_02003d84 address on the pan loop's back
// edge, and hoists y << 4, y << 5 out of the tile loop and base + line out of
// the pair loop, where the native recomputes each. CSE shares the camera
// calls' -1 in r6, where the native builds it at each call (a forwarding
// inline for Camera_MoveWithinMapBounds, as s1364.c has, splits it but moves
// the allocation further: 832).
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
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02001fdc(void);
void Func_0200208c(s32 value, s32 frames);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02002074(void);
void Func_02001f54(void *source, void *destination);
void Func_02002024(void);
void Func_02002084(s32 value);
void Task_Add(void (*func)(void), s32 priority);
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

#define MAP_WORD(map, offset) (*(u16 *)((map) + (offset)))

void Func_0200020c(s32 stage) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 palette;
  u8 *buffer;
  s32 panFrame;
  s32 frame;
  s32 fadeFrame;

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
  Camera_MoveWithinMapBounds(gData_02003d20[stage][0] << 16, -1,
                             gData_02003d20[stage][1] << 16, 0);
  Func_02001fdc();
  Func_0200208c(gData_02003d5c[stage], 1);
  {
    /* FAKEMATCH(temporary): the two halfwords go through s32 locals, so they
     * are built with movs and lsls as the native builds them; as plain
     * stores of 0x1000 and 0xd000, the u16 constants come from the literal
     * pool and 806 of 961 instructions differ in tla-en (788 here). */
    s32 scroll = 0x1000;
    s32 start = 0xd000;

    MAP_WORD(map, 0x11e) = scroll;
    MAP_WORD(map, 0x11c) = start;
  }
  Iwram_ClearWords((void *)0x050001c0, 32);
  wait(1);
  Actor_GetById(gMainRoutineWork.currentOwner)->animationKind = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->delay = 64;
  Event_StartScreenTransition();
  Camera_MoveWithinMapBounds(gData_02003d34[stage][0] << 16, -1,
                             gData_02003d34[stage][1] << 16, 1);
  for (panFrame = 0; panFrame != gData_02003d84[stage]; panFrame++) {
    {
      s32 length = gData_02003d84[stage];

      MAP_WORD(map, 0x11e) = (length - panFrame) << 5;
      if (panFrame == length - 59)
        Func_0200208c(gData_02003d70[stage], 60);
    }
    if (panFrame >= gData_02003d84[stage] - 64)
      // Pointer arithmetic folds gData_02003d88 + 1 into one literal, as the
      // native does; indexed as [stage + 1] the 1 is added at run time.
      MAP_WORD(map, 0x11c) += *(gData_02003d88 + stage + 1);
    wait(1);
  }
  Func_02002074();
  if (stage == 4) {
    u8 *frames = buffer + 0x2000;

    Func_02001f54(Data_02002469, buffer);
    Func_02001f54(Data_0200217c, frames);
    IO_WRITE_QUEUE_PUSH(0x06000000, (u32)buffer, 0x84000800);
    IO_WRITE_QUEUE_PUSH(0x06002000, (u32)frames, 0x84000140);
    wait(1);
  } else {
    u16 *tiles = (u16 *)buffer;
    s32 x, y;

    IO_WRITE_QUEUE_PUSH(0x050001c0, (u32)(Data_020020bc + palette * 32),
                        0x80000010);
    Iwram_ClearWords((void *)0x06001000, 0x2000);
    wait(1);
    // goto loops: the native recomputes y << 4, y << 5 and the 0x400 offset
    // in every pass; as for loops, 828 of 961 differ (788 here).
    y = 0;
  tileRow:
    x = 0;
  tile:
    {
      s32 tile = y * 16 + x + 128;

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
  // 990 of 961 differ (788 here).
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
        IO_WRITE_QUEUE_PUSH(0x06002000, (u32)(buffer + 0x2000 + step * 0x500),
                            0x84000140);
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
    u8 *grid = buffer + 0x2000;

    if (frame + 8 <= 63) {
      s32 offset = 0;
      s32 by, bx, row, k, i;
      s32 rowBase;
      s32 base, line;
      u8 *out;

      Func_02000138(grid, frame + 8, 0x10000);
      // goto loops: the native jumps to the block-row and block tests and
      // recomputes grid + k * 2 + base + line in every pass; as for loops,
      // loop.c strength-reduces the pair pointer and drops bx (784 of 961
      // differ, but no longer in the native's shape).
      by = 0;
      rowBase = 0;
      goto blockRowTest;
    blockRow:
      out = buffer + offset;
      i = 0;
    blank:
      *out = 0x11;
      i++;
      offset++;
      out++;
      if (i != 256)
        goto blank;
      base = rowBase * 8;
      bx = 0;
      goto blockTest;
    block:
      row = 0;
      line = 0;
    pixelRow:
      out = buffer + offset;
      k = 0;
    pixelPair:
      {
        u8 *pair = grid + (k * 2 + (base + line));

        k++;
        *out = pair[1] << 4;
        *out |= pair[0];
        offset++;
        out++;
      }
      if (k != 4)
        goto pixelPair;
      row++;
      line += 64;
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
    Camera_MoveWithinMapBounds(gData_02003d48[stage][0] << 16, -1,
                               gData_02003d48[stage][1] << 16, 1);
    Func_0200208c(0xb333, 60);
    // A goto loop: as a for loop, 810 of 961 differ (788 here).
    fadeFrame = 0;
  fadeOut:
    MAP_WORD(map, 0x11e) += 0xffe0;
    if (fadeFrame == 60) {
      ((struct EventWork *)Ram_HeapSlots->eventWork)->delay = 64;
      Event_FinishScreenTransition();
    }
    fadeFrame++;
    wait(1);
    if (fadeFrame != 124)
      goto fadeOut;
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
