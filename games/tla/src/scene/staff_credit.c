/// @file
/// The staff credits, played over the world map after the farewell at Prox:
/// each entrance is one stretch of the credits. Between them the camera flies
/// to each of the four lighthouses and shows its beacon's light
/// (StaffCredit_ShowBeacon), and last the four beams meet over Mt. Aleph and a
/// light falls from where they meet; then the scene goes on to the summit.
///
/// The overlay links the world map's flight, gauge and placed actors as well;
/// what of them the credits never call is marked UNUSED.
#include "flags.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "system/io_write_queue.h"
#include "system/io_reg.h"
#include "system/iwram_call.h"
#include "system/ram_buffer.h"
#include "system/main_routine_work.h"
#include "actor.h"
#include "field/field_sequence_event.h"
#include "bg_scroll.h"
#include "field/event_inline.h"
#include "system/frame.h"
#include "module/field/field.h"
#include "module/scene/gabomba_statue.h"
#include "module/scene/scene.h"
#include "module/scene/staff_credit.h"
#include "module/system/system.h"
#include "module/tla.h"
#include "graphics/resource.h"
#include "scene_ids.h"
#include "field_sprite.h"
#include "graphics/vram_tab.h"
#include "actor_view.h"
#include "call.h"
#include "system/task.h"
#include "macros.h"
#include "shared/tables.h"

extern s32 gStaffCredit_BeamsLightPalette[];

extern u16 gStaffCredit_BlendLevel;
extern u16 gStaffCredit_BlendSteady;
extern u8 gStaffCredit_BeaconPalettes[];
extern u8 gStaffCredit_BeamsPalette[];
extern u8 gStaffCredit_BeamsMaps[];
extern u8 gStaffCredit_BeamsTiles[];
void WorldMap_ReloadPendingAreaGraphics(void);
void StaffCredit_RunCredits(s32 value);
s32 WorldMap_GetBlockPlace(s32 *position);
extern s16 gGauge_Vram;
extern u8 gGauge_SourceTiles[][32];
extern u32 gGauge_Sprite[3];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

/// Sets the layer blend, switching its alpha every two frames unless
/// gStaffCredit_BlendSteady is set.
void StaffCredit_BlendBeacon(void) {
  u32 alphaReg = (u32)&REG_BLDALPHA;

  IoWriteQueue_Store16((u32)&REG_BLDCNT,
                       BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
  if ((gFrameCount & 2) && gStaffCredit_BlendSteady == 0) {
    IoWriteQueue_Store16(alphaReg, gStaffCredit_BlendLevel | 15);
  } else {
    IoWriteQueue_Store16(alphaReg, gStaffCredit_BlendLevel | 16);
  }
}

/// Adds a red, green and blue step to each colour of a palette, clamped to 0 to
/// 31.
void StaffCredit_StepPaletteColors(u16 *colors, s32 count, s32 red, s32 green,
                                   s32 blue) {
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

/// Draws the beacon's glow as rings of light about a corner, brightest at the
/// middle.
void StaffCredit_DrawGlowRings(u8 *grid, s32 count, s32 scale) {
  // Through a local pointer: the routine's address loads before the arguments.
  s32 (*fill)(void *destination, s32 size, u32 value) = Iwram_FillWordsEntry;
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

    level = (Trig_Cos((i << 14) / last) * 15) >> 16;
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

extern s16 gStaffCredit_FlightStarts[][2];
extern s16 gStaffCredit_FlightEnds[][2];
extern s16 gStaffCredit_FlightReturns[][2];
extern s32 gStaffCredit_StartZooms[];
extern s32 gStaffCredit_EndZooms[];
extern u8 gStaffCredit_FlightFrames[];
extern u8 gStaffCredit_FlightTurns[];
extern u16 gStaffCredit_GlowFadeIn[];
extern s8 gStaffCredit_GlowScrolls[][2];

static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 z, s32 mode) {
  FieldCamera_MoveToFixed(x, -1, z, mode);
}

/// Flies the camera to a lit lighthouse and shows its beacon's glow, then flies
/// back; stage 4 is the four beams meeting over Mt. Aleph.
void StaffCredit_ShowBeacon(s32 stage) {
  struct WorldCamera *map = (struct WorldCamera *)Ram_HeapSlots->mapWork;
  s32 palette;
  u8 *buffer;
  u8 *grid;
  s32 x;
  s32 y;
  s32 i;
  s32 frame;

  Actor_SetPosition(8, 0, 0);
  gStaffCredit_BlendSteady = 0;
  wait(1);
  Event_ClearFlag(FLAG_KEEP_PARTY_POSITION);
  gFieldWork->transition = 0;
  palette = stage;
  buffer = Heap_AllocEwram(0x6000);
  Actor_SetSpriteFlags(Actor_GetById(8), 0);
  if (stage == 4)
    palette = 0;
  gBgScroll[0].y = 0;
  gBgScroll[0].x = 0;
  Camera_MoveWithinMapBounds_(gStaffCredit_FlightStarts[stage][0] << 16,
                              gStaffCredit_FlightStarts[stage][1] << 16, 0);
  WorldMap_UpdateCurrentTileBlock();
  FieldEffect_ScheduleRatioTransition(gStaffCredit_StartZooms[stage], 1);
  map->yaw = 0x1000;
  map->unk11c = 0xd000;
  Iwram_ClearWordsEntry((void *)0x050001c0, 32);
  wait(1);
  Actor_GetById(gMainRoutineWork.currentOwner)->animationKind = 0;
  gFieldWork->transitionFrames = 64;
  Event_StartScreenTransition();
  Camera_MoveWithinMapBounds_(gStaffCredit_FlightEnds[stage][0] << 16,
                              gStaffCredit_FlightEnds[stage][1] << 16, 1);
  for (frame = 0; frame != gStaffCredit_FlightFrames[stage]; frame++) {
    {
      s32 length = gStaffCredit_FlightFrames[stage];

      map->yaw = (length - frame) << 5;
      if (frame == length - 59)
        FieldEffect_ScheduleRatioTransition(gStaffCredit_EndZooms[stage], 60);
    }
    if (frame >= gStaffCredit_FlightFrames[stage] - 64) {
      u16 *turn = (u16 *)&map->unk11c;
      u8 *step = gStaffCredit_FlightTurns + 1;

      *turn += step[stage];
    }
    wait(1);
  }
  WorldMap_Freeze();
  if (stage == 4) {
    u8 *frames = buffer + 0x2000;

    Decompress_TaggedLz(gStaffCredit_BeamsTiles, buffer);
    Decompress_TaggedLz(gStaffCredit_BeamsMaps, frames);
    IO_WRITE_QUEUE_PUSH(0x06000000, (u32)buffer, 0x84000800);
    IO_WRITE_QUEUE_PUSH(0x06002000, (u32)frames, 0x84000140);
    grid = frames;
    wait(1);
  } else {
    u16 *tiles = (u16 *)buffer;

    IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(14, 0),
                        (u32)(gStaffCredit_BeaconPalettes + palette * 32),
                        0x80000010);
    {
      void (*clear)(void *, s32) = (void (*)(void *, s32))Iwram_ClearWordsEntry;
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
  Event_Start();
  Event_ResolvePendingActions(0);
  Task_Add(StaffCredit_BlendBeacon, 0x480);
  Event_PlaySound(SE_GLITTER);
  for (frame = 0; frame != 172; frame++) {
    if (frame == 0) {
      if (stage == 4) {
        gBgScroll[0].x = 8;
        gBgScroll[0].y = frame;
        IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(14, 0),
                            (u32)gStaffCredit_BeamsPalette, 0x80000010);
        IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(15, 0),
                            (u32)gStaffCredit_BeamsLightPalette, 0x80000010);
      } else {
        gBgScroll[0].x = gStaffCredit_GlowScrolls[stage][0];
        gBgScroll[0].y = gStaffCredit_GlowScrolls[stage][1];
        IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(14, 0),
                            (u32)(gStaffCredit_BeaconPalettes + palette * 32),
                            0x80000010);
      }
    }
    if (stage == 4) {
      s32 zero;

      {
        s16 *camera = (s16 *)gStaffCredit_FlightEnds;
        s32 top;
        s32 left;
        s32 z;
        s32 x0;

        zero = 0;
        top = camera[8];
        left = camera[9];
        z = -0x1e0000;
        x0 = -0x320000;

        Actor_SetPositionFixed(8, (top << 16) + x0, (left << 16) + z);
      }
      Actor_SetBlendMode(Actor_GetById(8), 1);
      Actor_SetAnimation(8, 0);
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
        gStaffCredit_BlendLevel = 0xa00;
      }
      if (frame == 32) {
        gStaffCredit_BlendSteady = 1;
        gStaffCredit_BlendLevel = 0xa00;
      }
      if (frame == 36)
        gStaffCredit_BlendLevel = 0xb00;
      if (frame == 40)
        gStaffCredit_BlendLevel = 0xc00;
      if (frame == 44)
        gStaffCredit_BlendLevel = 0xd00;
      if (frame == 48)
        gStaffCredit_BlendLevel = 0xe00;
      if (frame == 52)
        gStaffCredit_BlendLevel = 0xf00;
      if (frame == 56)
        gStaffCredit_BlendLevel = 0x1000;
      if (frame == 70)
        IO_WRITE_QUEUE_PUSH(0x06002000, (u32)(buffer + 0x4300), 0x84000140);
      if (frame == 72)
        IO_WRITE_QUEUE_PUSH(0x06002000, (u32)(buffer + 0x4800), 0x84000140);
      if (frame == 74)
        IO_WRITE_QUEUE_PUSH(0x06002000, (u32)(buffer + 0x4d00), 0x84000140);
      if (frame == 76)
        IO_WRITE_QUEUE_PUSH(0x06002000, (u32)(buffer + 0x5200), 0x84000140);
      if (frame > 153) {
        gStaffCredit_BlendLevel = 0;
        StaffCredit_StepPaletteColors((u16 *)&BG_PLTT_COLOR(0, 0), 256, -2, -2,
                                      -2);
        StaffCredit_StepPaletteColors((u16 *)&OBJ_PLTT_COLOR(0, 0), 256, -2, -2,
                                      -2);
      } else if (frame > 79 && (frame & 1)) {
        StaffCredit_StepPaletteColors((u16 *)&BG_PLTT_COLOR(0, 0), 256, 1, 1,
                                      1);
        StaffCredit_StepPaletteColors((u16 *)&OBJ_PLTT_COLOR(0, 0), 256, 1, 1,
                                      1);
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

        StaffCredit_DrawGlowRings(grid, frame + 8, 0x10000);
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
      u16 *alpha = &gStaffCredit_BlendLevel;
      u32 late = frame - 32;

      *alpha = frame <= 6 ? gStaffCredit_GlowFadeIn[frame % 6] : 0x1000;
      gStaffCredit_BlendLevel = 0x1000;
      if (frame == 138)
        gStaffCredit_BlendSteady = 1;
      if (frame > 64 && (frame & 1))
        StaffCredit_StepPaletteColors((u16 *)&BG_PLTT_COLOR(14, 0), 16, 1, 1,
                                      1);
      if (frame > 140)
        StaffCredit_StepPaletteColors((u16 *)&BG_PLTT_COLOR(14, 0), 16, -2, -2,
                                      -2);
      if (late <= 95)
        gBgScroll[0].y++;
    }
    wait(1);
  }
  FieldTask_Remove(StaffCredit_BlendBeacon);
  if (stage != 4) {
    FieldCamera_SetSpeed(6);
    WorldMap_ReloadPendingAreaGraphics();
    Camera_MoveWithinMapBounds_(gStaffCredit_FlightReturns[stage][0] << 16,
                                gStaffCredit_FlightReturns[stage][1] << 16, 1);
    FieldEffect_ScheduleRatioTransition(45875, 60);
    for (frame = 0; frame != 124;) {
      map->yaw += 0xffe0;
      if (frame == 60) {
        gFieldWork->transitionFrames = 64;
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
  DebugText_Clear();
  IoWriteQueue_Store16((u32)&REG_BLDALPHA, gStaffCredit_BlendLevel);
  IoWriteQueue_Store16((u32)&REG_BLDCNT, 0);
  Event_SetFlag(FLAG_SKIP_SCENE_CHANGE_SOUND);
}

void WorldMap_StartPlacedActors(void);

/// Chooses where the credits go next by the entrance they were entered from:
/// entrances 2 to 5 first show a lighthouse's beacon, entrance 6 the beams
/// meeting, and each but the last runs the next stretch of credits.
s32 Scene_Initialize(void) {
  struct FieldWork *work;

  Event_SetFlag(FLAG_RETREAT_BLOCKED);
  work = Ram_HeapSlots->fieldWork;
  work->transition = 0;
  work->transitionFrames = 16;
  System_Wait(1);
  FieldCamera_SetSpeed(8);
  Event_ClearFlag(FLAG_SHOW_AREA_NAME);
  WorldMap_StartPlacedActors();
  switch (gMainRoutineWork.entrance) {
  case 0:
    Resource_ReservedNoOp((s32)&ResourceId_FarCall00A);
    StaffCredit_RunCredits(2);
    Event_SetDestination((s32)&SceneId_MtAlephSummit, 2);
    break;
  case 1:
    Resource_ReservedNoOp((s32)&ResourceId_FarCall00A);
    StaffCredit_RunCredits(3);
    Event_SetDestination((s32)&SceneId_StaffCredit, 2);
    break;
  case 2:
    StaffCredit_ShowBeacon(0);
    Resource_ReservedNoOp((s32)&ResourceId_FarCall00A);
    StaffCredit_RunCredits(4);
    Event_SetDestination((s32)&SceneId_StaffCredit, 3);
    break;
  case 3:
    StaffCredit_ShowBeacon(1);
    Resource_ReservedNoOp((s32)&ResourceId_FarCall00A);
    StaffCredit_RunCredits(5);
    Event_SetDestination((s32)&SceneId_StaffCredit, 4);
    break;
  case 4:
    StaffCredit_ShowBeacon(2);
    Resource_ReservedNoOp((s32)&ResourceId_FarCall00A);
    StaffCredit_RunCredits(6);
    Event_SetDestination((s32)&SceneId_StaffCredit, 5);
    break;
  case 5:
    StaffCredit_ShowBeacon(3);
    Resource_ReservedNoOp((s32)&ResourceId_FarCall00A);
    StaffCredit_RunCredits(7);
    Event_SetDestination((s32)&SceneId_StaffCredit, 6);
    break;
  case 6:
    StaffCredit_ShowBeacon(4);
    Event_SetDestination((s32)&SceneId_MtAlephSummit, 3);
    break;
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

#include "shared/tile_sprite_reset.h"

#include "shared/world_map_objects.c"

/// The flying ship's flicker and its sparks, in flight mode 1.
void StaffCredit_FlashInFlight(struct Actor *object) {
  if ((gFrameCount & 2) != 0) {
    Actor_SetSpriteColorMode(object, 7);
  } else {
    Actor_SetSpriteColorMode(object, 0);
  }
  if ((gFrameCount & 15) == 0) {
    WorldMap_SpawnSparkPair(object);
  }
}

/* VIEW(first): routine gStaffCredit_ColorStep as this part declared it; the
 * joined file has another; tried as the definition's extern LATE_DATA u32
 * gStaffCredit_ColorStep[], it does not compile in tla-ja:
 * games/tla/src/scene/staff_credit.c:540: `gStaffCredit_ColorStep' undeclared
 * (first use in this function). */
extern s32 gStaffCredit_ColorStep__first __asm__("gStaffCredit_ColorStep");
#define gStaffCredit_ColorStep gStaffCredit_ColorStep__first

void Actor_CycleChildValues(struct Actor *object) {
  if (gFrameCount & 1) {
    Actor_SetSpriteColorMode(object,
                             gStaffCredit_ColorModes[gStaffCredit_ColorStep]);
    gStaffCredit_ColorStep = (gStaffCredit_ColorStep + 1) & 3;
  }
  if (!(gFrameCount & 3)) {
    WorldMap_SpawnParticle(object);
  }
}

void Actor_SetFlightMode(struct Actor *object, s32 mode) {
  if (mode == 0) {
    object->update = 0;
    Actor_SetSpriteColorMode(object, 0);
    Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  } else if (mode == 1) {
    object->update = (void (*)(void))StaffCredit_FlashInFlight;
  } else {
    object->update = (void (*)(void))Actor_CycleChildValues;
    Event_PlaySound(SE_291);
  }
}

#include "shared/leader_search.h"

UNUSED void StaffCredit_EndFlight(void) {
  Actor_SetFlightMode(Actor_Get(FieldParty_GetLeaderActor()), 0);
}

extern u16 gStaffCredit_PictureBlend;

/// Sets the layer blend, switching its alpha every two frames.
void StaffCredit_BlendPicture(void) {
  u32 alphaRegister = (u32)&REG_BLDALPHA;

  IoWriteQueue_Store16((u32)&REG_BLDCNT,
                       BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
  if (gFrameCount & 2) {
    IoWriteQueue_Store16(alphaRegister, gStaffCredit_PictureBlend | 12);
  } else {
    IoWriteQueue_Store16(alphaRegister, gStaffCredit_PictureBlend | 16);
  }
}

static inline void CameraMove(s32 x, s32 y, s32 z, s32 mode) {
  FieldCamera_MoveToFixed(x, y, z, mode);
}

/// Shakes the camera diagonally by one pixel every three frames.
#include "shared/camera_jitter.h"

#define PUSH_PICTURE(offset)                                                   \
  IO_WRITE_QUEUE_PUSH(0x06002000, (u32)(buffer + (offset)), 0x84000140)

UNUSED void StaffCredit_ShowPicture(s32 mode) {
  u8 *buffer;
  u32 level;

  /* FAKEMATCH(temporary): the blend level is read into level, a u32 local,
   * before each IoWriteQueue_Store16 call. */
  System_Wait(1);
  Event_ClearFlag(FLAG_KEEP_PARTY_POSITION);
  gFieldWork->transition = 0;
  Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;
  gFieldWork->transitionFrames = 16;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  if (mode == 2) {
    MapGrid_SetShake(2, 3, 1);
    Event_PlaySound(SE_RUMBLE);
  }
  WorldMap_Freeze();
  buffer = Heap_AllocEwram(0x4000);
  Decompress_TaggedLz(gWorldMapPackedTiles, buffer);
  Decompress_TaggedLz(gWorldMapPackedPictures, buffer + 0x1000);
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(14, 0),
                      (u32)(gWorldMapPalettes + mode * 32), 0x80000010);
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
  Task_AddFrame(StaffCredit_BlendPicture, 0x480);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_GLITTER);
  gStaffCredit_PictureBlend = 0xe00;
  PUSH_PICTURE(0x3480);
  FieldEvent_Wait(2);
  gStaffCredit_PictureBlend = 0xd00;
  PUSH_PICTURE(0x2e80);
  FieldEvent_Wait(2);
  gStaffCredit_PictureBlend = 0xc00;
  PUSH_PICTURE(0x2880);
  FieldEvent_Wait(2);
  gStaffCredit_PictureBlend = 0xb00;
  PUSH_PICTURE(0x2280);
  FieldEvent_Wait(2);
  gStaffCredit_PictureBlend = 0xa00;
  PUSH_PICTURE(0x1c80);
  FieldEvent_Wait(2);
  gStaffCredit_PictureBlend = 0x900;
  PUSH_PICTURE(0x1680);
  FieldEvent_Wait(2);
  gStaffCredit_PictureBlend = 0x800;
  PUSH_PICTURE(0x1080);
  FieldEvent_Wait(140);
  PUSH_PICTURE(0x1680);
  FieldEvent_Wait(4);
  PUSH_PICTURE(0x1c80);
  FieldEvent_Wait(4);
  PUSH_PICTURE(0x2280);
  FieldEvent_Wait(4);
  gStaffCredit_PictureBlend = 0x900;
  PUSH_PICTURE(0x2880);
  FieldEvent_Wait(4);
  gStaffCredit_PictureBlend = 0xa00;
  PUSH_PICTURE(0x2e80);
  FieldEvent_Wait(4);
  gStaffCredit_PictureBlend = 0xb00;
  PUSH_PICTURE(0x3480);
  FieldEvent_Wait(4);
  gStaffCredit_PictureBlend = 0xc00;
  PUSH_PICTURE(0x3a80);
  if (mode == 1) {
    MapGrid_SetShake(2, 2, 1);
    Event_PlaySound(SE_RUMBLE);
    gStaffCredit_PictureBlend = 0xd00;
    FieldEvent_Wait(4);
    gStaffCredit_PictureBlend = 0xe00;
    FieldEvent_Wait(4);
    gStaffCredit_PictureBlend = 0xf00;
    FieldEvent_Wait(4);
    gStaffCredit_PictureBlend = 0x1000;
    FieldEvent_Wait(45);
    FieldTask_Remove(StaffCredit_BlendPicture);
    DebugText_Clear();
    level = gStaffCredit_PictureBlend;
    IoWriteQueue_Store16((u32)&REG_BLDALPHA, level);
    IoWriteQueue_Store16((u32)&REG_BLDCNT, 0);
  } else {
    gStaffCredit_PictureBlend = 0xd00;
    FieldEvent_Wait(4);
    gStaffCredit_PictureBlend = 0xe00;
    FieldEvent_Wait(4);
    gStaffCredit_PictureBlend = 0xf00;
    FieldEvent_Wait(4);
    gStaffCredit_PictureBlend = 0x1000;
    FieldEvent_Wait(45);
    FieldTask_Remove(StaffCredit_BlendPicture);
    DebugText_Clear();
    level = gStaffCredit_PictureBlend;
    IoWriteQueue_Store16((u32)&REG_BLDALPHA, level);
    IoWriteQueue_Store16((u32)&REG_BLDCNT, 0);
  }
  Heap_Release(buffer);
  Event_SetFlag(FLAG_SKIP_SCENE_CHANGE_SOUND);
}

UNUSED void StaffCredit_FlashAndEmit(struct Actor *object) {
  if ((gFrameCount & 2) != 0) {
    Actor_SetSpriteColorMode(object, 7);
  } else {
    Actor_SetSpriteColorMode(object, 0);
  }
  if ((gFrameCount & 15) == 0) {
    StaffCredit_SpawnGrowingPair(object);
  }
}

void Actor_AnimateAndEmit(struct Actor *object) {
  u32 frame = gFrameCount;

  if (frame & 1)
    Actor_SetSpriteColorMode(object, (frame >> 1) % 6);
  if ((gFrameCount & 15) == 0)
    StaffCredit_SpawnGrowingPair(object);
}

#include "shared/actor_animate.h"

#include "shared/actor_grow_up.h"

#include "shared/actor_grow_down.h"

/// Adds a pair of sparks under an actor, one growing and one shrinking.
void StaffCredit_SpawnGrowingPair(struct Actor *parent) {
  struct Actor *pair[2];
  struct Actor *child;
  struct FieldSprite *sprite;
  struct LinkedEffectWork *work = Ram_HeapSlots->effectWork;
  s32 i;

  for (i = 0; i < 2; i++) {
    child = (FieldActor_Create(271, parent->x, parent->y, parent->z));
    pair[i] = child;
    if (child != NULL) {
      child->terrainHeight = parent->terrainHeight;
      sprite = child->sprite;
      child->motionFlags = 0;
      child->timer = 0;
      child->watchedObject = parent;
      if (sprite != NULL) {
        Animation_ApplyChildArgument((struct ActorSpriteView *)sprite, 0);
        sprite->nextFrame = 0;
        VramCache_FreeEntry(sprite->vramBlock);
        sprite->vramBlock = work->vramBlock;
        /* FAKEMATCH(signature): the sprite written through LinkedSpriteView. */
        ((struct LinkedSpriteView *)sprite)->retainCacheEntry = 1;
        sprite->tile =
            (gResourceTableEntries[sprite->vramBlock].offset >> 5) & 0x3ff;
        sprite->fullColor = 0;
        sprite->shape = 1;
        ((struct LinkedSpriteView *)sprite)->size = 2;
        ((struct LinkedDetail *)sprite->record)->field16 = 0;
      }
    }
  }
  {
    struct Actor *p = pair[0];
    struct FieldSprite *sp = p->sprite;

    p->update = (void *)Actor_GrowDown;
    sp->priority = 1;
  }
  {
    struct Actor *p = pair[1];
    struct FieldSprite *sp = p->sprite;

    sp->priority = 1;
    p->update = (void *)Actor_GrowUp;
    p->priorityFlags = 2;
  }
}

extern s16 gGauge_Shown;
extern u8 gGauge_Tiles[][32];

void Gauge_Draw(s32 progress) {
  s32 level = ((progress * 30 + 0x8000) >> 16) + 1;
  s32 kind = 0;
  s32 count, base, i;

  if (progress != 0)
    kind = 100;
  if (level <= 15 && (gFrameCount & 15) <= 1)
    kind = 200;
  if (level <= 7 && (gFrameCount & 7) <= 1)
    kind = 200;
  if (kind + level == gGauge_Shown)
    return;
  gGauge_Shown = kind + level;
  Iwram_ClearWords((s32)(gGauge_Tiles), (s32)(0x100));
  {
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;

    if (kind == 0) {
      copy(gGauge_Tiles, gGauge_SourceTiles[8], 0x80);
      goto done;
    }
    if (kind == 100)
      copy(gGauge_Tiles, gGauge_SourceTiles[4], 0x80);
    else
      copy(gGauge_Tiles, gGauge_SourceTiles[8], 0x80);
  }
  base = 0;
  count = level;
  if (count > 7) {
    Iwram_CopyWordsEntry(gGauge_Tiles, gGauge_SourceTiles, 32);
    base = 8;
    count -= 8;
  }
  if (level > 15) {
    Iwram_CopyWordsEntry(gGauge_Tiles[1], gGauge_SourceTiles[1], 32);
    count = level;
    base = 16;
    count -= 16;
  }
  if (level > 23) {
    Iwram_CopyWordsEntry(gGauge_Tiles[2], gGauge_SourceTiles[2], 32);
    count = level;
    base = 24;
    count -= 24;
  }
  if (level > 30) {
    Iwram_CopyWordsEntry(gGauge_Tiles[3], gGauge_SourceTiles[3], 32);
    base = 0;
    count = 0;
  }
  if (count != 0) {
    i = 0;
    if (i >= count)
      goto done;
  loop:
    {
      s32 j;
      u8 *source = &gGauge_SourceTiles[base / 8][4] + i / 2;
      u8 *dest = &gGauge_Tiles[base / 8][4] + i / 2;
      /* FAKEMATCH(barrier): the empty asm on dest changes its allocation
       * rank. */
      asm("" : "=r"(dest) : "0"(dest));

      j = 2;
      if ((i & 1) == 0) {
        j = 1;
      even:
        *dest = (*dest & 240) | (*source & 15);
        j++;
        source += 4;
        dest += 4;
        if (j > 3)
          goto next;
        goto even;
      } else {
      oddLoop:
        *dest = (*dest & 15) | (*source & 240);
        j--;
        source += 4;
        dest += 4;
        if (j >= 0)
          goto oddLoop;
      }
    }
  next:
    i++;
    if (i < count)
      goto loop;
  }
done:
  Vram_Load(gGauge_Vram, 0x100, gGauge_Tiles);
}

extern u32 gInput;
extern s16 gGauge_Countdown;

// The same cursor as the world map's, in this overlay: counts down while A or B
// is held, redraws the gauge and queues its sprite over the leader.
/// Counts down while A or B is held, redraws the gauge and queues its sprite.
void Gauge_Update(void) {
  u32 *oam = gGauge_Sprite;
  s32 tile = gResourceTableEntries[gGauge_Vram].offset >> 5;
  struct Actor *leader = Actor_Get(FieldParty_GetLeaderActor());
  s32 screen[3];
  u32 attributes;

  if ((gInput & 3) != 0 && Event_GetFlag(FLAG_MENU_OPEN) == 0 &&
      leader->y > px(4)) {
    if (--gGauge_Countdown <= 0) {
      Party_DrainPp(1);
      gGauge_Countdown = 10;
    }
  }
  Gauge_Draw(Party_GetPpRatio());
  attributes = 0x80004000;
  Call2((void (*)())Render_ProjectPoint, (s32)(&leader->x), (s32)(screen));
  {
    s32 x = screen[0] - 16;
    s32 y = screen[1] - 32;

    *oam++ = 0;
    *oam++ = (y | (x << 16)) | attributes;
  }
  *oam = tile | 0x400;
  Oam_AddSpriteAtDepthClamped(gGauge_Sprite, 255);
}

void Gauge_Init(void) {
  Decompress_TaggedLz(gWorldMapPackedTilesC, gGauge_SourceTiles);
  gGauge_Vram = VramCache_FindFreeEntry();
  Vram_Load(gGauge_Vram, 0x100, 0);
  gGauge_Shown = -1;
  gGauge_Countdown = 10;
  Task_AddFrame(Gauge_Update, TASK_PASS_VBLANK - 10);
}

void Gauge_End(void) {
  Decompress_TaggedLz(gWorldMapPackedTilesC, gGauge_SourceTiles);
  VramCache_FreeEntry(gGauge_Vram);
  gGauge_Shown = -1;
  FieldTask_Remove(Gauge_Update);
}

s32 gWorldMap_SparkScript[] = {
    46,
    (s32)WorldMap_RiseFromAnchor,
    38,
};

// clang-format off
u32 gWorldMap_ParticleScript[] = {
    0, 39, 22, 5, 0, 22,
    7, 0, 0, 1, 38,
};

u32 gStaffCredit_ColorModes[] = {
    0, 9, 0, 6,
};
// clang-format on

#undef PUSH_PICTURE
#undef gStaffCredit_ColorStep

LATE_DATA struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 9850, 0, 9748, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 10150, 0, 8180, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 10144, 0, 8330, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 10144, 0, 8240, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {4, CONDITION_ALWAYS, 10137, 0, 8602, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {5, CONDITION_ALWAYS, 10423, 0, 8354, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {6, CONDITION_ALWAYS, 10190, 0, 8883, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {7, CONDITION_ALWAYS, 10578, 0, 8952, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {8, CONDITION_ALWAYS, 10635, 0, 9040, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {9, CONDITION_ALWAYS, 10168, 0, 9630, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 10271, 0, 9540, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {11, CONDITION_ALWAYS, 10271, 0, 9420, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {12, CONDITION_ALWAYS, 10380, 0, 9370, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {13, CONDITION_ALWAYS, 10460, 0, 9400, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {14, CONDITION_ALWAYS, 11125, 0, 10695, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {15, CONDITION_ALWAYS, 12070, 0, 10470, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {16, CONDITION_ALWAYS, 11880, 0, 10090, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {17, CONDITION_ALWAYS, 10976, 0, 9960, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {18, CONDITION_ALWAYS, 11736, 0, 9530, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {19, CONDITION_ALWAYS, 11910, 0, 9748, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {20, CONDITION_ALWAYS, 11633, 0, 9406, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {21, CONDITION_ALWAYS, 9240, 0, 9532, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {22, CONDITION_ALWAYS, 9344, 0, 9530, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {23, CONDITION_ALWAYS, 9148, 0, 9048, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {24, CONDITION_ALWAYS, 9148, 0, 8988, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {25, CONDITION_ALWAYS, 9200, 0, 8440, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {26, CONDITION_ALWAYS, 9060, 0, 8290, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {27, CONDITION_ALWAYS, 9256, 0, 7950, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {28, CONDITION_ALWAYS, 9546, 0, 8316, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {29, CONDITION_ALWAYS, 12770, 0, 11570, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {30, CONDITION_ALWAYS, 11724, 0, 8064, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {31, CONDITION_ALWAYS, 11680, 0, 6678, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {32, CONDITION_ALWAYS, 13486, 0, 7482, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {33, CONDITION_ALWAYS, 12921, 0, 9639, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {34, CONDITION_ALWAYS, 12921, 0, 9752, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {35, CONDITION_ALWAYS, 12770, 0, 10220, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {36, CONDITION_ALWAYS, 14478, 0, 8410, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {37, CONDITION_ALWAYS, 14250, 0, 8728, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {38, CONDITION_ALWAYS, 13370, 0, 6070, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {39, CONDITION_ALWAYS, 13452, 0, 6204, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {40, CONDITION_ALWAYS, 10898, 0, 6300, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {41, CONDITION_ALWAYS, 11198, 0, 6132, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {42, CONDITION_ALWAYS, 12472, 0, 7592, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {43, CONDITION_ALWAYS, 12090, 0, 7165, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {44, CONDITION_ALWAYS, 12972, 0, 1240, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {45, CONDITION_ALWAYS, 13088, 0, 1090, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {46, CONDITION_ALWAYS, 9490, 0, 10960, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {47, CONDITION_ALWAYS, 5836, 0, 10120, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {48, CONDITION_ALWAYS, 8582, 0, 7420, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {49, CONDITION_ALWAYS, 6314, 0, 6223, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {50, CONDITION_ALWAYS, 8848, 0, 4650, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {51, CONDITION_ALWAYS, 8970, 0, 7500, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {52, CONDITION_ALWAYS, 7256, 0, 8325, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {53, CONDITION_ALWAYS, 7280, 0, 8180, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {54, CONDITION_ALWAYS, 7714, 0, 7760, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {55, CONDITION_ALWAYS, 7275, 0, 7365, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {56, CONDITION_ALWAYS, 7383, 0, 5753, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {57, CONDITION_ALWAYS, 7504, 0, 5650, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {58, CONDITION_ALWAYS, 7662, 0, 5558, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {59, CONDITION_ALWAYS, 8960, 0, 5856, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {60, CONDITION_ALWAYS, 8933, 0, 3800, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {61, CONDITION_ALWAYS, 8916, 0, 1640, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {62, CONDITION_ALWAYS, 9000, 0, 1307, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {63, CONDITION_ALWAYS, 9040, 0, 1245, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {64, CONDITION_ALWAYS, 1992, 0, 7272, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {65, CONDITION_ALWAYS, 2032, 0, 7600, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {67, CONDITION_ALWAYS, 11633, 0, 9406, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {68, CONDITION_ALWAYS, 10640, 0, 9075, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {69, CONDITION_ALWAYS, 10640, 0, 9075, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {71, CONDITION_ALWAYS, 10720, 0, 9075, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {72, CONDITION_ALWAYS, 7279, 0, 7307, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {80, CONDITION_ALWAYS, 11754, 0, 8030, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {81, CONDITION_ALWAYS, 11700, 0, 8034, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {82, CONDITION_ALWAYS, 11596, 0, 10204, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {83, CONDITION_ALWAYS, 10240, 0, 9142, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {84, CONDITION_ALWAYS, 9020, 0, 882, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {85, CONDITION_ALWAYS, 9288, 0, 9434, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {86, CONDITION_ALWAYS, 9288, 0, 9640, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {87, CONDITION_ALWAYS, 12640, 0, 4390, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {88, CONDITION_ALWAYS, 7868, 0, 9120, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {89, CONDITION_ALWAYS, 9479, 0, 5060, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {98, CONDITION_ALWAYS, 8916, 0, 1640, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {99, CONDITION_ALWAYS, 10271, 0, 9540, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {100, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {101, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {102, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {103, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS_END,
};

LATE_DATA struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(666, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 9020, 0, 882,
                    FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct Trigger gSceneEvents[] = {
    {TRIGGER_TABLE_END},
};

// clang-format off
LATE_DATA s16 gStaffCredit_FlightStarts[][2] = {
    {9276, 1782}, {10754, 3137}, {10160, 6268}, {8243, 7334}, {8926, 5100},
};

LATE_DATA s16 gStaffCredit_FlightEnds[][2] = {
    {9020, 782}, {10101, 4284}, {10179, 7270}, {7255, 7305}, {9801, 5100},
};

LATE_DATA s16 gStaffCredit_FlightReturns[][2] = {
    {9570, 2573}, {12179, 4639}, {8727, 7317}, {9751, 6829}, {9751, 5220},
};

LATE_DATA s32 gStaffCredit_StartZooms[] = {
    0x8000, 0xcccc, 0x18000, 0x9999, 0xe666,
};

LATE_DATA s32 gStaffCredit_EndZooms[] = {
    0x10000, 0xcccc, 0xb333, 0x14ccc, 0x9999,
};

LATE_DATA u8 gStaffCredit_FlightFrames[] = {
    130, 170, 120, 126,
};

LATE_DATA u8 gStaffCredit_FlightTurns[] = {
    107, 108, 64, 108, 108, 108,
};

LATE_DATA u16 gStaffCredit_GlowFadeIn[] = {
    0xe00, 0xd00, 0xc00, 0xb00, 0xa00, 0x900, 0x800,
};
// clang-format on

LATE_DATA s8 gStaffCredit_GlowScrolls[][2] = {
    {22, -4}, {8, -8}, {2, -28}, {14, 12}, {8, 8}, {0, 0},
};

LATE_DATA s32 gWorldMap_PlacedActorScript[] = {
    46,
    (s32)TileSprite_Reset,
    17,
    0,
};

LATE_DATA u32 gStaffCredit_ColorStep[] = {
    0,
};
