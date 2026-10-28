// Draft, differ 1088 of 961: one stage of the staff credits, from
// the camera pan through the tilemap and palette pushes to the fade out;
// stage 4 grows actor 8 and swaps its frames instead. Left: register
// allocation (the native keeps stage in r11 and the buffer in r10, this draft
// spills them, so its frame is 32 bytes against 16), the two map halfwords
// loaded from the pool, and the scheduling that follows.
#include "types.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "adventure.h"
#include "object_runtime.h"
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

void WaitFrames(s32 frames);
void GameFlag_ClearBit(s32 flag);
void GameFlag_SetBit(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void Func_02002034(s32 id, s32 x, s32 z);
void *Runtime_BumpAllocateAlternatePool(s32 size);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
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
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Func_0200005c(void);
void Func_020020b4(s32 cue);
void Animation_SetStateFlags(struct ObjectRuntime *object, s32 flags);
void Func_020000c0(u16 *colors, s32 count, s32 red, s32 green, s32 blue);
void Func_02000138(u8 *grid, s32 count, s32 scale);
void Func_02002044(s32 a, s32 b);
void Func_0200207c(void);
void Func_02001f74(void);
void MemFree(void *pointer);

#define MAP_WORD(map, offset) (*(u16 *)((map) + (offset)))

void Func_0200020c(s32 stage) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 palette;
  u8 *buffer;
  s32 frame;

  Func_02002034(8, 0, 0);
  gData_02003dbe = 0;
  WaitFrames(1);
  GameFlag_ClearBit(0x109);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0;
  palette = stage;
  buffer = Runtime_BumpAllocateAlternatePool(0x6000);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(8), 0);
  if (stage == 4)
    palette = 0;
  gBgScroll[0].y = 0;
  gBgScroll[0].x = 0;
  Camera_MoveWithinMapBounds(gData_02003d20[stage][0] << 16, -1,
                             gData_02003d20[stage][1] << 16, 0);
  Func_02001fdc();
  Func_0200208c(gData_02003d5c[stage], 1);
  MAP_WORD(map, 0x11e) = 0x1000;
  MAP_WORD(map, 0x11c) = 0xd000;
  Iwram_ClearWords((void *)0x050001c0, 32);
  WaitFrames(1);
  Object_GetById(gAdventure.currentOwner)->animationKind = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->delay = 64;
  Event_StartScreenTransition();
  Camera_MoveWithinMapBounds(gData_02003d34[stage][0] << 16, -1,
                             gData_02003d34[stage][1] << 16, 1);
  for (frame = 0; frame != gData_02003d84[stage]; frame++) {
    s32 length = gData_02003d84[stage];

    MAP_WORD(map, 0x11e) = (length - frame) << 5;
    if (frame == length - 59)
      Func_0200208c(gData_02003d70[stage], 60);
    if (frame >= gData_02003d84[stage] - 64)
      MAP_WORD(map, 0x11c) += gData_02003d88[stage + 1];
    WaitFrames(1);
  }
  Func_02002074();
  if (stage == 4) {
    u8 *frames = buffer + 0x2000;

    Func_02001f54(Data_02002469, buffer);
    Func_02001f54(Data_0200217c, frames);
    IO_WRITE_QUEUE_PUSH(0x06000000, (u32)buffer, 0x84000800);
    IO_WRITE_QUEUE_PUSH(0x06002000, (u32)frames, 0x84000140);
    WaitFrames(1);
  } else {
    u16 *tiles = (u16 *)buffer;
    s32 x, y;

    IO_WRITE_QUEUE_PUSH(0x050001c0, (u32)(Data_020020bc + palette * 32),
                        0x80000010);
    Iwram_ClearWords((void *)0x06001000, 0x2000);
    WaitFrames(1);
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
    WaitFrames(1);
  }
  Func_02002024();
  Func_02002084(0);
  Scheduler_AddOrUpdateCallback(Func_0200005c, 0x480);
  Func_020020b4(246);
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
      Func_02002034(8, (gData_02003d34[4][0] << 16) - 0x320000,
                    (gData_02003d34[4][1] << 16) - 0x1e0000);
      Animation_SetStateFlags(Object_GetById(8), 1);
      Object_SetModeById(8, 0);
      Object_GetById(8)->scaleX = 0;
      Object_GetById(8)->scaleY = 0;
      if (frame > 22) {
        s32 scale = (frame << 10) - 0x5400;

        if (scale > 0x10000)
          scale = 0x10000;
        Object_GetById(8)->scaleX = scale;
        Object_GetById(8)->scaleY = scale;
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
    } else if (frame >= 0 && frame + 8 <= 63) {
      u8 *grid = buffer + 0x2000;
      s32 offset = 0;
      s32 by, bx, row, k, i;
      s32 rowBase = 0;

      Func_02000138(grid, frame + 8, 0x10000);
      for (by = 0; by != 8; by++) {
        u8 *out = buffer + offset;
        s32 base;

        for (i = 0; i != 256; i++) {
          *out++ = 0x11;
          offset++;
        }
        base = rowBase * 8;
        for (bx = 0; bx != 8; bx++) {
          s32 line = 0;

          for (row = 0; row != 8; row++) {
            out = buffer + offset;
            for (k = 0; k != 4; k++) {
              u8 *pair = grid + base + line + k * 2;

              *out = pair[1] << 4;
              *out |= pair[0];
              offset++;
              out++;
            }
            line += 64;
          }
          base += 8;
        }
        rowBase += 64;
      }
      IO_WRITE_QUEUE_PUSH(0x06001000, (u32)buffer, 0x84000400);
    }
    if (stage != 4) {
      u32 late = frame - 32;

      if (frame <= 6)
        gData_02003dbc = gData_02003d8e[frame % 6];
      else
        gData_02003dbc = 0x1000;
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
    WaitFrames(1);
  }
  Scheduler_RemoveCallbackFar(Func_0200005c);
  if (stage != 4) {
    Func_02002044(0x60000, 0xc000);
    Func_0200207c();
    Camera_MoveWithinMapBounds(gData_02003d48[stage][0] << 16, -1,
                               gData_02003d48[stage][1] << 16, 1);
    Func_0200208c(0xb333, 60);
    for (frame = 0; frame != 124; frame++) {
      MAP_WORD(map, 0x11e) -= 32;
      if (frame == 60) {
        ((struct EventWork *)Ram_HeapSlots->eventWork)->delay = 64;
        Event_FinishScreenTransition();
      }
      WaitFrames(1);
    }
    Event_WaitTransitionFrames();
  } else {
    gBgScroll[0].x = 0;
    gBgScroll[0].y = 0;
  }
  MemFree(buffer);
  Func_02001f74();
  QueueIoWriteDelay2(0x04000052, gData_02003dbc);
  QueueIoWriteDelay2(0x04000050, 0);
  GameFlag_SetBit(0x101);
}
