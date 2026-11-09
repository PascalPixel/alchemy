#include "types.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_work.h"
#include "bg_scroll.h"
#include "event_inline.h"

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

void *Heap_AllocEwram(s32 size);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 z, s32 mode) {
  Camera_MoveWithinMapBounds(x, -1, z, mode);
}
void Func_02001fdc(void);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02002074(void);
// The veneer of Resource_DecodeType01Far.
s32 Func_02001f54(const void *source, void *destination);
void Func_02002024(void);
void Func_0200005c(void);
void Animation_SetStateFlags(struct Actor *object, s32 flags);
void Func_020000c0(u16 *colors, s32 count, s32 red, s32 green, s32 blue);
void Func_02000138(u8 *grid, s32 count, s32 scale);
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

  Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
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
  Func_080d46a4(gData_02003d5c[stage], 1);
  map->yaw = 0x1000;
  map->unk11c = 0xd000;
  Iwram_ClearWords((void *)0x050001c0, 32);
  wait(1);
  Actor_GetById(gMainRoutineWork.currentOwner)->animationKind = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->delay = 64;
  Event_StartScreenTransition();
  Camera_MoveWithinMapBounds_(gData_02003d34[stage][0] << 16,
                              gData_02003d34[stage][1] << 16, 1);
  for (frame = 0; frame != gData_02003d84[stage]; frame++) {
    {
      s32 length = gData_02003d84[stage];

      map->yaw = (length - frame) << 5;
      if (frame == length - 59)
        Func_080d46a4(gData_02003d70[stage], 60);
    }
    if (frame >= gData_02003d84[stage] - 64)
    // Pointer arithmetic folds gData_02003d88 + 1 into one literal, as the
    // native does; indexed as [stage + 1] the 1 is added at run time.
    {
      u16 *turn = (u16 *)&map->unk11c;
      u8 *step = gData_02003d88 + 1;

      *turn += step[stage];
    }
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
    {
      void (*clear)(void *, s32) = (void (*)(void *, s32))Iwram_ClearWords;
      s32 size = 0x2000;
      void *tilesAt = (void *)0x06001000;

      clear(tilesAt, size);
    }
    wait(1);
    for (y = 0; y != 8; y++) {
      for (x = 0; x != 16; x++) {
        s32 tile = y * 16 + x + 128;

        tiles[y * 32 + x] = tile | (s16)0xe000;
        tiles[y * 32 - x + 31] = tile | (s16)0xe400;
        tiles[(15 - y) * 32 + x] = tile | (s16)0xe800;
        tiles[(15 - y) * 32 - x + 31] = tile | (s16)0xec00;
      }
    }
    for (y = 0; y != 16; y++) {
      for (x = 0; x != 32; x++)
        tiles[y * 32 + x + 0x200] = 0xe080;
    }
    IO_WRITE_QUEUE_PUSH(0x06002000, (u32)buffer, 0x80000400);
    wait(1);
  }
  Func_02002024();
  Event_ResolvePendingActions(0);
  Task_Add(Func_0200005c, 0x480);
  Sound_Play(246);
  for (frame = 0; frame != 172; frame++) {
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
      s32 zero;

      {
        s16 *camera = (s16 *)gData_02003d34;
        s32 top;
        s32 left;
        s32 z;
        s32 x0;

        zero = 0;
        /* FAKEMATCH(barrier): the native holds the scale's 0 from here; as a
         * plain 0, 30 of 961 instructions differ in tla-en. */
        asm volatile("" : "+l"(zero));
        top = camera[8];
        left = camera[9];
        z = -0x1e0000;
        x0 = -0x320000;

        Actor_SetHorizontalPositionWithTerrain(8, (top << 16) + x0,
                                               (left << 16) + z);
      }
      Animation_SetStateFlags(Actor_GetById(8), 1);
      Actor_SetModeById(8, 0);
      Actor_GetById(8)->scaleX = zero;
      Actor_GetById(8)->scaleY = zero;
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
          IO_WRITE_QUEUE_PUSH(0x06002000, (u32)(buffer + step * 0x500 + 0x2000),
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
      grid = buffer + 0x2000;

      if (frame + 8 <= 63) {
        s32 offset = 0;
        s32 by, bx, row;
        s32 rowBase;
        s32 base;
        u8 *out;
        u8 *fill;

        Func_02000138(grid, frame + 8, 0x10000);
        // goto loops: the native jumps to the block-row and block tests and
        // recomputes grid + x * 2 + base + i in every pass; with the pair loop
        // as a for, differ 623 and gap 169.
        by = 0;
        rowBase = 0;
        goto blockRowTest;
      blockRow:
        bx = 0;
        fill = (u8 *)offset + (s32)buffer;
      blank:
        *fill = 0x11;
        bx++;
        offset++;
        fill++;
        if (bx != 256)
          goto blank;
        bx = 0;
        base = rowBase * 8;
        goto blockTest;
      block:
        row = 0;
        i = 0;
      pixelRow:
        x = 0;
        out = (u8 *)offset + (s32)buffer;
      pixelPair:
        {
          u8 *pair = grid + (x * 2 + (base + i));

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
      u16 *alpha = &gData_02003dbc;
      u32 late = frame - 32;

      *alpha = frame <= 6 ? gData_02003d8e[frame % 6] : 0x1000;
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
    wait(1);
  }
  Task_RemoveFar(Func_0200005c);
  if (stage != 4) {
    Camera_SetSpeedParameters(0x60000, 0xc000);
    Func_0200207c();
    Camera_MoveWithinMapBounds_(gData_02003d48[stage][0] << 16,
                                gData_02003d48[stage][1] << 16, 1);
    Func_080d46a4(0xb333, 60);
    for (frame = 0; frame != 124;) {
      map->yaw += 0xffe0;
      if (frame == 60) {
        ((struct EventWork *)Ram_HeapSlots->eventWork)->delay = 64;
        Event_FinishScreenTransition();
      }
      frame++;
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
