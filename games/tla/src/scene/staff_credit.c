/// @file
/// The scene code of overlay 0x64c that its scene tables belong to, in ROM order; the rest of the overlay is in staff_credit/.

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_work.h"
#include "bg_scroll.h"
#include "event_inline.h"
#include "frame.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

extern u16 gData_02003dbc;
extern u16 gData_02003dbe;

/// Sets the layer blend, switching its alpha every two frames unless
/// gData_02003dbe is set.
void Func_0200005c(void) {
  u32 alphaReg = (u32)&REG_BLDALPHA;

  QueueIoWriteDelay2((u32)&REG_BLDCNT,
                     BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
  if ((gFrameCount & 2) && gData_02003dbe == 0) {
    QueueIoWriteDelay2(alphaReg, gData_02003dbc | 15);
  } else {
    QueueIoWriteDelay2(alphaReg, gData_02003dbc | 16);
  }
}

/// Adds a red, green and blue step to each colour of a palette, clamped to 0 to
/// 31.
void Func_020000c0(u16 *colors, s32 count, s32 red, s32 green, s32 blue) {
  s32 i;

  for (i = 0; i != count; i++) {
    s32 r = (*colors & 31) + red;
    s32 g = (*colors >> 5 & 31) + green;
    s32 b = (*colors >> 10 & 31) + blue;

    if (r > 31)
      r = 31;
    if (r < 0)
      r = 0;
    if (g > 31)
      g = 31;
    if (g < 0)
      g = 0;
    if (b > 31)
      b = 31;
    if (b < 0)
      b = 0;
    *colors++ = (b << 10) | (g << 5) | r;
  }
}

s32 Math_Cosine(s32 angle);

void Func_02000138(u8 *grid, s32 count, s32 scale) {
  // Through a local pointer: the routine's address loads before the arguments.
  s32 (*fill)(void *destination, s32 size, u32 value) = Iwram_FillWords;
  s32 i = 0;
  // The corner the rings are centred on; each holds 63 for its whole life.
  s32 cornerColumn = 63;
  s32 cornerRow = 63;

  fill(grid, 0x1000, 0x01010101);
  if (count == 1)
    return;
  do {
    s32 y = 0;
    s32 last = count - 1;
    s32 level;
    s32 x = i;
    s32 error = i;

    level = (Math_Cosine((i << 14) / last) * 15) >> 16;
    level = level * scale / 0x10000;
    if (level > 15)
      level = 15;
    if (level <= 0)
      level = 1;
    while (x >= y) {
      s32 lastColumn = 63;
      s32 column = cornerColumn - x;
      s32 row = cornerRow - y;
      u8 *cell;
      // row << 6 is row * 64, spelled as the shift it matches with.
      cell = &grid[(row << 6) + column];
      cell[0] = level;
      if (column < lastColumn)
        cell[1] = level;
      // The same point mirrored across the diagonal.
      column = row;
      row = cornerRow - x;
      cell = &grid[(row << 6) + column];
      cell[0] = level;
      if (column < lastColumn)
        cell[1] = level;
      error = error - y * 2 - 1;
      if (error < 0) {
        error = error + x * 2 - 2;
        x--;
      }
      y++;
    }
    i++;
  } while (i != count - 1);
}

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

static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 z, s32 mode) {
  Camera_MoveWithinMapBounds(x, -1, z, mode);
}
void Func_02001fdc(void);
void Func_02002074(void);
// The veneer of Resource_DecodeType01Far.
s32 Func_02001f54(const void *source, void *destination);
void Func_02002024(void);

void Func_0200207c(void);
void Func_02001f74(void);

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
  FieldTask_Remove(Func_0200005c);
  if (stage != 4) {
    FieldCamera_SetSpeed(0x60000, 0xc000);
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
  Event_SetFlag(0x101);
}

extern char ResourceId_FarCall00A;
extern char SceneId_Ov64c;
extern char SceneId_Ov6b4;

void Func_0200101c(void);
void Func_02001f8c(s32 resource);
void Func_02002054(s32 scene, s32 entrance);
void Func_020020ac(s32 value);

/// Chooses where the credits go next by the entrance they were entered from:
/// entrances 0 to 5 run resource FarCall00A with 2 to 7 and continue, 2 to 6
/// after Func_0200020c's stage 0 to 4; entrance 6 leaves for SceneId_Ov6b4.
s32 Func_02000b64(void) {
  struct EventWork *work;

  Event_SetFlag(0x144);
  work = Ram_HeapSlots->eventWork;
  work->screenEffect = 0;
  work->delay = 16;
  System_Wait(1);
  FieldCamera_SetSpeed(0x80000, 0x10000);
  Event_ClearFlag(0x12f);
  Func_0200101c();
  switch (gMainRoutineWork.entrance) {
  case 0:
    Func_02001f8c((s32)&ResourceId_FarCall00A);
    Func_020020ac(2);
    Func_02002054((s32)&SceneId_Ov6b4, 2);
    break;
  case 1:
    Func_02001f8c((s32)&ResourceId_FarCall00A);
    Func_020020ac(3);
    Func_02002054((s32)&SceneId_Ov64c, 2);
    break;
  case 2:
    Func_0200020c(0);
    Func_02001f8c((s32)&ResourceId_FarCall00A);
    Func_020020ac(4);
    Func_02002054((s32)&SceneId_Ov64c, 3);
    break;
  case 3:
    Func_0200020c(1);
    Func_02001f8c((s32)&ResourceId_FarCall00A);
    Func_020020ac(5);
    Func_02002054((s32)&SceneId_Ov64c, 4);
    break;
  case 4:
    Func_0200020c(2);
    Func_02001f8c((s32)&ResourceId_FarCall00A);
    Func_020020ac(6);
    Func_02002054((s32)&SceneId_Ov64c, 5);
    break;
  case 5:
    Func_0200020c(3);
    Func_02001f8c((s32)&ResourceId_FarCall00A);
    Func_020020ac(7);
    Func_02002054((s32)&SceneId_Ov64c, 6);
    break;
  case 6:
    Func_0200020c(4);
    Func_02002054((s32)&SceneId_Ov6b4, 3);
    break;
  }
  return 0;
}

struct TileSprite {
  u8 unk00[18];
  u16 tile;
  u8 unk14[6];
  u8 frame;
};

s32 Scene_PrepareMap(void) { return 0; }

s32 Func_02000c9c(struct Actor *object) {
  u16 *tile = (u16 *)((u8 *)Ram_HeapSlots->mapWork + 286);
  struct TileSprite *sprite = object->sprite;

  sprite->tile = *tile;
  sprite->frame = 0;
  return 1;
}

/// The two countdown choices the world map (overlay 64b) also has, under this
/// overlay's names.
#define Func_02003600 Func_02000cbc
#define Func_02003654 Func_02000d10
#define Func_02004e30 Sound_Play
#define gData_02006c08 gData_02003dc0
#include "world_map/s3600.c"

/// The world map's placed objects (overlay 64b), shown and hidden as the view
/// moves, under this overlay's names.

#define Func_020036a8 Func_02000d64
#define Func_02004e08 Func_02002094
#define Func_02004e10 Party_GetLeaderActor
#define Func_02004b30 Import_ActorCreate
#define Func_02004b20 Func_02001fc4
#define Func_02004b18 Func_02001fbc
#define Func_02004b38 Func_02001fd4
#define gData_02006bd4 gData_02003da8
#include "world_map/s36a8.c"

/// The blend queue and the cell table reset the world map (overlay 64b) also
/// has, under this overlay's names.
#define Func_020038e4 Func_02000fa0
#define Func_02003960 Func_0200101c
#include "world_map/s3960.c"

s32 Func_02001044(s32 x, s32 z) { return x / 0x100000 + (z / 0x100000) * 128; }

s32 Func_02001064(struct Actor *object) {
  struct Actor *anchor = object->watchedObject;
  s32 frame;
  s32 scale;

  object->timer++;
  frame = object->timer;
  if (frame > 31)
    return 0;
  scale = Iwram_MulQ16(Math_Sin(frame << 10), 0x14000);
  object->scaleX = scale;
  object->scaleY = scale;
  object->x = anchor->x;
  object->y += 0x10000;
  object->z = anchor->z;
  return 1;
}

/// The spark pair and the random particle the world map (overlay 64b) also
/// has, under this overlay's names.
#define Func_020039f4 Func_020010b0
#define Func_02003aac Func_02001168
#define gData_02004f90 gData_02002e74
#define gData_02004f9c gData_02002e80
#include "world_map/s39f4.c"

void Animation_ApplyChildValues(struct Actor *object, s32 value);

void Func_020011f4(struct Actor *object) {
  if ((gFrameCount & 2) != 0) {
    Animation_ApplyChildValues(object, 7);
  } else {
    Animation_ApplyChildValues(object, 0);
  }
  if ((gFrameCount & 15) == 0) {
    Func_020010b0(object);
  }
}

extern s32 gData_02003db8;
extern const s32 gData_02002eac[];

void Func_0200122c(struct Actor *object) {
  if (gFrameCount & 1) {
    Animation_ApplyChildValues(object, gData_02002eac[gData_02003db8]);
    gData_02003db8 = (gData_02003db8 + 1) & 3;
  }
  if (!(gFrameCount & 3)) {
    Func_02001168(object);
  }
}

void Func_02001274(struct Actor *object, s32 mode) {
  if (mode == 0) {
    object->update = 0;
    Animation_ApplyChildValues(object, 0);
    Event_PlaySound(390);
  } else if (mode == 1) {
    object->update = (void (*)(void))Func_020011f4;
  } else {
    object->update = (void (*)(void))Func_0200122c;
    Event_PlaySound(291);
  }
}

s32 Func_02002004(s32 *position);
s32 Func_020020a4(s32 mode, s32 value);

void Func_020012ac(void) {
  struct Actor *object = Actor_Get(FieldParty_GetLeaderActor());
  s32 i;

  Event_PlaySound(212);
  Func_02001274(object, 1);
  for (i = 0; i < 20; i++) {
    s32 hit = Func_02002004(&object->x);

    if (hit != 0 && Func_020020a4(2, hit) != 0)
      break;
    System_Wait(1);
  }
  Func_02001274(object, 2);
}

void Func_020012f8(void) {
  Func_02001274(Actor_Get(FieldParty_GetLeaderActor()), 0);
}

extern u16 gData_02003fc0;

/// Sets the layer blend, switching its alpha every two frames.
void Func_0200130c(void) {
  u32 alphaRegister = (u32)&REG_BLDALPHA;

  QueueIoWriteDelay2((u32)&REG_BLDCNT,
                     BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
  if (gFrameCount & 2) {
    QueueIoWriteDelay2(alphaRegister, gData_02003fc0 | 12);
  } else {
    QueueIoWriteDelay2(alphaRegister, gData_02003fc0 | 16);
  }
}

static inline void CameraMove(s32 x, s32 y, s32 z, s32 mode) {
  /* FAKEMATCH(wrapper): the call forwarded through the inline, which sets the
   * arguments up in their own order: z is shifted before the -1 starts; as
   * direct Camera_MoveWithinMapBounds calls, 6 of 66 instructions differ in
   * tla-en. */
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

/// Shakes the camera diagonally by one pixel every three frames.
void Func_02001364(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = *(s16 *)((u8 *)object + 10);
  s32 z = *(s16 *)((u8 *)object + 18);

  if (gFrameCount % 3 == 0) {
    switch ((u32)(Random16Far() << 2) >> 16) {
    case 0:
      CameraMove((x << 16) - 0x10000, -1, (z << 16) + 0x10000, 1);
      break;
    case 1: {
      Camera_MoveWithinMapBounds((x << 16) + 0x10000, -1, (z << 16) - 0x10000,
                                 1);
      break;
    }
    case 2:
      CameraMove((x << 16) + 0x10000, -1, (z << 16) + 0x10000, 1);
      break;
    case 3:
      CameraMove((x << 16) - 0x10000, -1, (z << 16) - 0x10000, 1);
      break;
    }
  }
}

extern u8 gWorldMapPackedTiles[];
extern u8 gWorldMapPackedPictures[];
extern u8 gWorldMapPalettes[];

#define PUSH_PICTURE(offset)                                                   \
  IO_WRITE_QUEUE_PUSH(0x06002000, (u32)(buffer + (offset)), 0x84000140)

void Func_02001400(s32 mode) {
  u8 *buffer;
  u32 level;

  /* FAKEMATCH(temporary): the blend level is read into level, a u32 local,
   * before each QueueIoWriteDelay2 call; as QueueIoWriteDelay2(0x04000052,
   * gData_02003fc0), 4 of 593 instructions differ in tla-en. */
  System_Wait(1);
  Event_ClearFlag(0x109);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0;
  Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->delay = 16;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  if (mode == 2) {
    MapGrid_SetShake(0x20000, 0x30000, 0x10000);
    Event_PlaySound(141);
  }
  Func_02002074();
  buffer = Heap_AllocEwram(0x4000);
  Func_02001f54(gWorldMapPackedTiles, buffer);
  Func_02001f54(gWorldMapPackedPictures, buffer + 0x1000);
  IO_WRITE_QUEUE_PUSH(0x050001c0, (u32)(gWorldMapPalettes + mode * 32),
                      0x80000010);
  {
    struct IoWriteQueue *q = &gIoWriteQueue;
    u32 saved;
    s32 count;

    IRQ_LOCK(saved);
    count = q->count;
    if (count <= 31) {
      u32 *entry = q->entries[count];

      *entry++ = (u32)buffer;
      q->count = count + 1;
      *entry++ = 0x06001000;
      *entry = 0x84000400;
    }
    IRQ_UNLOCK(saved);
  }
  Task_AddFrame(Func_0200130c, 0x480);
  Func_02002024();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(246);
  gData_02003fc0 = 0xe00;
  PUSH_PICTURE(0x3480);
  FieldEvent_Wait(2);
  gData_02003fc0 = 0xd00;
  PUSH_PICTURE(0x2e80);
  FieldEvent_Wait(2);
  gData_02003fc0 = 0xc00;
  PUSH_PICTURE(0x2880);
  FieldEvent_Wait(2);
  gData_02003fc0 = 0xb00;
  PUSH_PICTURE(0x2280);
  FieldEvent_Wait(2);
  gData_02003fc0 = 0xa00;
  PUSH_PICTURE(0x1c80);
  FieldEvent_Wait(2);
  gData_02003fc0 = 0x900;
  PUSH_PICTURE(0x1680);
  FieldEvent_Wait(2);
  gData_02003fc0 = 0x800;
  PUSH_PICTURE(0x1080);
  FieldEvent_Wait(140);
  PUSH_PICTURE(0x1680);
  FieldEvent_Wait(4);
  PUSH_PICTURE(0x1c80);
  FieldEvent_Wait(4);
  PUSH_PICTURE(0x2280);
  FieldEvent_Wait(4);
  gData_02003fc0 = 0x900;
  PUSH_PICTURE(0x2880);
  FieldEvent_Wait(4);
  gData_02003fc0 = 0xa00;
  PUSH_PICTURE(0x2e80);
  FieldEvent_Wait(4);
  gData_02003fc0 = 0xb00;
  PUSH_PICTURE(0x3480);
  FieldEvent_Wait(4);
  gData_02003fc0 = 0xc00;
  PUSH_PICTURE(0x3a80);
  if (mode == 1) {
    MapGrid_SetShake(0x20000, 0x20000, 0x10000);
    Event_PlaySound(141);
    gData_02003fc0 = 0xd00;
    FieldEvent_Wait(4);
    gData_02003fc0 = 0xe00;
    FieldEvent_Wait(4);
    gData_02003fc0 = 0xf00;
    FieldEvent_Wait(4);
    gData_02003fc0 = 0x1000;
    FieldEvent_Wait(45);
    FieldTask_Remove(Func_0200130c);
    Func_02001f74();
    level = gData_02003fc0;
    QueueIoWriteDelay2(0x04000052, level);
    QueueIoWriteDelay2(0x04000050, 0);
  } else {
    gData_02003fc0 = 0xd00;
    FieldEvent_Wait(4);
    gData_02003fc0 = 0xe00;
    FieldEvent_Wait(4);
    gData_02003fc0 = 0xf00;
    FieldEvent_Wait(4);
    gData_02003fc0 = 0x1000;
    FieldEvent_Wait(45);
    FieldTask_Remove(Func_0200130c);
    Func_02001f74();
    level = gData_02003fc0;
    QueueIoWriteDelay2(0x04000052, level);
    QueueIoWriteDelay2(0x04000050, 0);
  }
  Heap_Release(buffer);
  Event_SetFlag(0x101);
}

void Func_02001ad0(struct Actor *object);

void Func_0200199c(struct Actor *object) {
  if ((gFrameCount & 2) != 0) {
    Animation_ApplyChildValues(object, 7);
  } else {
    Animation_ApplyChildValues(object, 0);
  }
  if ((gFrameCount & 15) == 0) {
    Func_02001ad0(object);
  }
}

void Func_020019d4(struct Actor *object) {
  u32 frame = gFrameCount;

  if (frame & 1)
    Animation_ApplyChildValues(object, (frame >> 1) % 6);
  if ((gFrameCount & 15) == 0)
    Func_02001ad0(object);
}

void Func_02001a0c(struct Actor *object) {
  u32 frame = gFrameCount;

  if (frame & 1)
    Animation_ApplyChildValues(object, (frame >> 1) % 6);
}

void Func_02001a34(struct Actor *object) {
  struct Actor *parent = object->watchedObject;
  s32 step = ++object->timer;
  s32 scale;
  s32 rise;

  if (step > 31) {
    Func_02001fd4(object);
    return;
  }
  scale = Math_Sin(step << 10);
  object->scaleX = scale;
  object->scaleY = scale;

  rise = 0x10000;
  object->x = parent->x;
  object->y += rise;
  object->z = parent->z + (rise - scale) * 5 + 0x80000;
}

void Func_02001a80(struct Actor *object) {
  struct Actor *parent = object->watchedObject;
  s32 step = ++object->timer;
  s32 scale;
  s32 rise;

  if (step > 31) {
    Func_02001fd4(object);
    return;
  }
  scale = Math_Sin(step << 10);
  object->scaleX = scale;
  object->scaleY = -scale;

  rise = 0x10000;
  object->x = parent->x;
  object->y += rise;
  object->z = parent->z - (rise - scale) * 5 + 0x100000;
}
