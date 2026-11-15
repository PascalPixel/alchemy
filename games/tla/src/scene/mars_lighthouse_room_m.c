/// @file
/// The Mars Lighthouse's central hall and the ways to its four towers, in six
/// rooms that share this overlay.
///
/// In the hall (SceneId_MarsLighthouseRoomMInscriptions) four inscriptions name
/// the peoples who ruled earth, water, fire and wind, and the great dragon
/// statue speaks: without the Mars Star it sends the party away; with it, once
/// the lighthouse has turned to fire, it bids them set the four spirits aflame.
/// Four ledges in the rain (M2 to M5) lead from the hall to the four towers,
/// and each tower comes out on a beacon ledge (M6), where Blaze lights its
/// element's flame. With all four lit the statue says the heavens await, the
/// floor before it breaks open, and Teleport there carries the party up to the
/// aerie.

#include "system/task.h"
#include "graphics/palette_buffers.h"
#include "sprite_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "scene/ride.h"
#include "battle/battle.h"
#include "field/field_sequence.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "scene_ids.h"
#include "system/ram_buffer.h"
#include "system/frame.h"
#include "field/actor.h"
#include "field/event_inline.h"
#include "system/main_routine_work.h"
#include "system/iwram_call.h"
#include "system/io_write_queue.h"
#include "system/io_reg.h"
#include "field/call.h"
#include "version.h"
#include "system/dma.h"
#include "shared/mars_lighthouse_entrance_room.h"
#include "scene/dehkan_plateau.h"
#include "battle/battle_unit.h"
#include "system/vector3.h"
#include "fixed_math.h"
#include "shared/tables.h"
#include "shared/event_check_spheres_overlap.h"
#include "graphics/display_transition.h"
#include "window/item_icon.h"
#include "field/map_grid.h"
#include "field/field_sprite.h"
#include "macros.h"
#include "field/actor_script.h"
#include "field/sprite.h"
#include "text/msg_ids.h"
#include "system/input.h"

/// All four flames lit: the floor before the statue has broken open.
#define FLAG_WAY_TO_AERIE_OPEN 0xa31
#define FLAG_VENUS_FLAME_LIT 0xa32
#define FLAG_MERCURY_FLAME_LIT 0xa33
#define FLAG_MARS_FLAME_LIT 0xa34
#define FLAG_JUPITER_FLAME_LIT 0xa35
/// "If you wish to scale the heavens, set the four spirits aflame!"
#define FLAG_TOLD_TO_LIGHT_FLAMES 0xa2e
/// "If you do not hold the star, you shall not ascend to the heavens...
/// Begone!"
#define FLAG_TOLD_TO_BEGONE 0xa2f

/// The beacon ledge (M6) is reached from each tower by its own entrance.
enum {
  ENTRANCE_FROM_VENUS_TOWER = 1,
  ENTRANCE_FROM_MERCURY_TOWER = 2,
  ENTRANCE_FROM_MARS_TOWER = 3,
  ENTRANCE_FROM_JUPITER_TOWER = 4
};

/// Each element's flame among the flame sprite's animations.
enum {
  FLAME_ANIMATION_MARS = 2,
  FLAME_ANIMATION_JUPITER = 3,
  FLAME_ANIMATION_VENUS = 4,
  FLAME_ANIMATION_MERCURY = 5
};

/// The beacon ledge (M6): the torch Blaze is cast at, tinted to its tower's
/// element, its flame, the point above it Blaze reaches, and two statues.
enum {
  ACTOR_LEDGE_TORCH = 8,
  ACTOR_LEDGE_FLAME = 9,
  ACTOR_LEDGE_BLAZE_TARGET = 10
};

/// The central hall (M1): four torches (actors 8 to 11), the four elements'
/// flames on them, shown once lit, and the statue's mouth, which lets the
/// fireball fall that breaks the floor.
enum {
  ACTOR_VENUS_FLAME = 12,
  ACTOR_MERCURY_FLAME = 13,
  ACTOR_MARS_FLAME = 14,
  ACTOR_JUPITER_FLAME = 15,
  ACTOR_STATUE_MOUTH = 16,
  ACTOR_FIREBALL = 18
};

struct Actor;
struct Vector3;

s32 Leader_KnockBack(void);

s32 Fireball_CarryLeader(void);
void Actor_LaunchAhead(struct Actor *self);

void Teleport_StartShimmer(void);

void MarsLighthouseRoomM_FadeDoorway(void);

void MarsLighthouseRoomM_BreakFloorTile(s32 x, s32 z, s32 c, s32 d, s32 e,
                                        s32 f);

void MarsLighthouseRoomM_CycleLedgeFlame(void);

void MarsLighthouseRoomM_CycleHallFlames(void);

void MarsLighthouseRoomM_StartDoorways(s32 entrance);

void Ride_Create(void);

void MarsLighthouseRoomM_FlashDoorways(void);

void Actor_Blink(struct Actor *object);

extern u32 gMarsLighthouseRoomM_CrumbScript[];
extern u32 gEmbers_Script[];

void MarsLighthouseRoomM_LeaveLedge(void);

void MarsLighthouseRoomM_BeginBlazeOnTorch(void);

void MarsLighthouseRoomM_WatchBlazeReachFlame(void);

void MarsLighthouseRoomM_TeleportToAerie(void);

void MarsLighthouseRoomM_ReadEarthInscription(void);
void MarsLighthouseRoomM_ReadWaterInscription(void);
void MarsLighthouseRoomM_ReadFireInscription(void);

void MarsLighthouseRoomM_ReadWindInscription(void);

void MarsLighthouseRoomM_HearDragonStatue(void);

extern s32 gMarsLighthouseRoomM_StatueScript[];

void Actor_Release(void *handle);
void FieldCamera_MoveToActorById(s32 a, s32 b);
void FieldCamera_WaitForArrival(void);
void Teleport_PlaySparks(void);
extern u16 gMarsLighthouseRoomM_LedgeSavedPalette[];
/// The colours of BG palettes 8 and 9.
struct HallColors {
  u16 colors[2 * PALETTE_COLORS];
};
extern struct HallColors gMarsLighthouseRoomM_HallFlameColors;
extern struct HallColors gMarsLighthouseRoomM_HallPalette;
extern const u16 gMarsLighthouseRoomM_UnlitFlameColors[];
extern s16 gMarsLighthouseRoomM_DoorwayBlend;
extern s16 gMarsLighthouseRoomM_DoorwayBlendTarget;
extern s16 gMarsLighthouseRoomM_Doorway;
extern s16 gMarsLighthouseRoomM_DoorwayStep;
void Map_CopyPalette(s32 scene, s32 a1);
void TilesetAnim_Resume(u32 no);

void FieldEffect_StartBlankOverlay(void);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

/// The ride work seen from one flame thrower's entry: the work's header, then
/// the entry, as the getters below index it.
struct RideRow {
  u8 beforeEntries[20];
  struct RideEntry entry;
};

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

void Teleport_StopShimmer(void);

/// The ledge's torch and Blaze's mark start with their sprite flags clear.
s32 MarsLighthouseRoomM_SetUpStatue(struct Actor *object) {
  Actor_SetSpriteFlags(object, 0);
  return 0;
}

/// Shows a flame on its torch, 14 pixels up, in its element's animation.
s32 MarsLighthouseRoomM_ShowFlame(s32 id, s32 value) {
  struct Actor *object = Actor_Get(id);

  object->motionFlags = 0;
  object->y.fixed = toFIXED(14);
  Actor_SetSpriteFlags(object, 0);
  Actor_SelectAnimation(object, value);
#ifdef UBFIX
  return 0;
#endif
}

#ifndef TLA_MARS_LIGHTHOUSE_ROOM_M_H
#define TLA_MARS_LIGHTHOUSE_ROOM_M_H

struct MarsLighthouseRoomMVector {
  s32 x;
  s32 y;
  s32 z;
};

/// A sprite as the beacon ledge reads it: the priority bits of its OAM
/// attribute 2 (layer), its VRAM block, the bit that keeps the block cached,
/// and its rotation.
struct MarsLighthouseRoomMAnimation {
  u8 beforeLayer[9];
  u8 tileHigh : 2;
  u8 layer : 2;
  u8 beforeVramBlock[6];
  u8 vramBlock;
  u8 keepEntry : 1;
  u16 angle;
};

struct MarsLighthouseRoomMRideWork {
  s32 counter;
  s16 active;
  u8 unk06[6];
  u16 value0c;
  s16 value0e;
  u8 unk10[0x60];
  struct Actor *handle;
};

extern u8 gInventorySnapshot[];

void Actor_StartScript(struct Actor *object, const void *script);

void Flame_UpdateForward(struct Actor *object);
void Actor_ResetScripted(struct Actor *object);
void Actor_SetupScriptedPart(struct Actor *object, s32 layer);
void Flame_UpdateLanding(struct Actor *object);

extern s32 gScriptedPart_Script[];
extern struct ActorPlacement gMarsLighthouseRoomM_Placements[];
extern struct ActorPlacement gMarsLighthouseRoomM_Room6Placements[];
extern struct ActorPlacement gMarsLighthouseRoomM_Room1Placements[];

extern u16 gPaletteCycle_Flash;
extern u16 gPaletteCycle_Countdown;
extern u16 gPaletteCycle_Period;
extern u16 *gPaletteCycle_Slots;
extern u16 gPaletteCycle_Count;
extern u16 gPaletteCycle_Step;
extern struct Trigger gMarsLighthouseRoomM_Triggers[];
extern struct Trigger gMarsLighthouseRoomM_Room6Triggers[];
extern struct Trigger gMarsLighthouseRoomM_Room1Triggers[];
extern struct Trigger gMarsLighthouseRoomM_Room5Triggers[];
extern s32 gMarsLighthouseRoomM_RubbleBounceScript[];
extern s32 gMarsLighthouseRoomM_RubbleFallScript[];

s32 List_CountEntries(u16 *list);
void PaletteCycle_Fade(void);
void Teleport_StopShimmer(void);
void Teleport_StartFastShimmer(void);
s32 Actor_IsAbove(struct Actor *a, struct Actor *b);
void Ride_UpdateDrift(struct Actor *object);
void Actor_SetupScripted(struct Actor *object);
void MarsLighthouseRoomM_SpawnRubble(s32 x, s32 z, s32 kind);
void Actor_HoldDuringEvent(struct Actor *object);
void Event_End(void);
void Teleport_PlayWhirl(void);
s32 Trig_Cos(s32 angle);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);

#endif

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_MARS_LIGHTHOUSE_ROOM_M_FLAME)
    return gMarsLighthouseRoomM_Room6Placements;
  if (scene == SCENE_MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS)
    return gMarsLighthouseRoomM_Room1Placements;
  return gMarsLighthouseRoomM_Placements;
}

extern s32 gMarsLighthouseRoomM_LedgeCycleFrame;
extern u16 gMarsLighthouseRoomM_LedgeFlameColors[];
extern u16 gMarsLighthouseRoomM_LedgePalette[];

/// Each frame on the beacon ledge: once its flame is lit, its element's six
/// colours turn round in the flame's palette.
void MarsLighthouseRoomM_CycleLedgeFlame(void) {
  s32 entrance = gMainRoutineWork.entrance;
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 base;
  s32 i;
  s8 busy;

  if (work->psynergy != 0)
    return;
  busy = work->revealActive;
  if (busy != 0)
    return;
  if (gMarsLighthouseRoomM_LedgeCycleFrame == -1) {
    Iwram_CopyWordsEntry(gMarsLighthouseRoomM_LedgeSavedPalette,
                         (void *)&BG_PLTT_COLOR(13, 0), 32);
    Iwram_CopyWordsEntry(gMarsLighthouseRoomM_LedgeFlameColors,
                         (void *)&BG_PLTT_COLOR(8, 0), 64);
    Iwram_CopyWordsEntry(gMarsLighthouseRoomM_LedgePalette,
                         gMarsLighthouseRoomM_LedgeFlameColors, 64);
    gMarsLighthouseRoomM_LedgeCycleFrame = 0;
  }
  base = gMarsLighthouseRoomM_LedgeCycleFrame / 8 % 6;
  Iwram_CopyWordsEntry(gMarsLighthouseRoomM_LedgePalette,
                       gMarsLighthouseRoomM_LedgeSavedPalette, 32);
  if (entrance == ENTRANCE_FROM_VENUS_TOWER &&
      Event_GetFlag(FLAG_VENUS_FLAME_LIT)) {
    for (i = 0; i <= 5; i++)
      gMarsLighthouseRoomM_LedgePalette[i + 10] =
          gMarsLighthouseRoomM_LedgeFlameColors[(base + i) % 6 + 10];
  }
  if (entrance == ENTRANCE_FROM_MERCURY_TOWER &&
      Event_GetFlag(FLAG_MERCURY_FLAME_LIT)) {
    for (i = 0; i <= 5; i++)
      gMarsLighthouseRoomM_LedgePalette[i + 10] =
          gMarsLighthouseRoomM_LedgeFlameColors[(base + i) % 6 + 20];
  }
  if (entrance == ENTRANCE_FROM_MARS_TOWER &&
      Event_GetFlag(FLAG_MARS_FLAME_LIT)) {
    for (i = 0; i <= 5; i++)
      gMarsLighthouseRoomM_LedgePalette[i + 10] =
          gMarsLighthouseRoomM_LedgeFlameColors[(base + i) % 6 + 26];
  }
  if (entrance == ENTRANCE_FROM_JUPITER_TOWER &&
      Event_GetFlag(FLAG_JUPITER_FLAME_LIT)) {
    for (i = 0; i <= 5; i++)
      gMarsLighthouseRoomM_LedgePalette[i + 10] =
          gMarsLighthouseRoomM_LedgeFlameColors[(base + i) % 6 + 4];
  }
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(13, 0),
                      (u32)gMarsLighthouseRoomM_LedgePalette, 0x84000008);
  gMarsLighthouseRoomM_LedgeCycleFrame++;
}

/// The ledge's way back leads into the tower it was reached from.
void MarsLighthouseRoomM_LeaveLedge(void) {
  Event_PlaySound(SE_MAP_EXIT);
  FieldEvent_SetExitRequest(gMainRoutineWork.entrance);
}

// XXX: what Blaze does otherwise once the field work's blazeLongSpray is set
// (its mode 3 for 2 when it finds no torch); the rest of blaze.c's reading of
// the mode would settle it.
/// Blaze is cast at the ledge's torch.
void MarsLighthouseRoomM_BeginBlazeOnTorch(void) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;

  event->blazeLongSpray = 1;
}

/// The camera follows Blaze up to the flame.
void MarsLighthouseRoomM_WatchBlazeReachFlame(void) {
  FieldCamera_MoveToActorById(ACTOR_LEDGE_BLAZE_TARGET, 1);
  FieldCamera_WaitForArrival();
}

/// Blaze ends on the flame: a flash, and the tower's element's flame is lit.
void MarsLighthouseRoomM_LightFlame(void) {
  struct FieldWork *work;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_Interpolate(30);
  FieldEvent_Wait(30);
  Event_PlaySound(SE_WARP);
  switch (gMainRoutineWork.entrance) {
  case ENTRANCE_FROM_VENUS_TOWER:
    Event_SetFlag(FLAG_VENUS_FLAME_LIT);
    break;
  case ENTRANCE_FROM_MERCURY_TOWER:
    Event_SetFlag(FLAG_MERCURY_FLAME_LIT);
    break;
  case ENTRANCE_FROM_MARS_TOWER:
    Event_SetFlag(FLAG_MARS_FLAME_LIT);
    break;
  case ENTRANCE_FROM_JUPITER_TOWER:
    Event_SetFlag(FLAG_JUPITER_FLAME_LIT);
    break;
  }
  work = Ram_HeapSlots->fieldWork;
  work->psynergy = 0;
  FieldEvent_Wait(30);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_Interpolate(30);
  FieldEvent_Wait(30);
  FieldEvent_Wait(10);
  Event_End();
}

extern s32 gMarsLighthouseRoomM_HallCycleFrame;
extern u16 gMarsLighthouseRoomM_HallSavedPalette[];

/// Each frame in the hall: the lit flames' colours turn round, and an unlit
/// one shows the palette's unlit colours.
void MarsLighthouseRoomM_CycleHallFlames(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 base;
  s32 i;
  s8 busy;

  if (work->psynergy != 0)
    return;
  busy = work->revealActive;
  if (busy != 0)
    return;
  if (gMarsLighthouseRoomM_HallCycleFrame == -1) {
    Iwram_CopyWordsEntry(gMarsLighthouseRoomM_HallSavedPalette,
                         (void *)&BG_PLTT_COLOR(2, 0), 32);
    Iwram_CopyWordsEntry(gMarsLighthouseRoomM_HallFlameColors.colors,
                         (void *)&BG_PLTT_COLOR(8, 0), 64);
    Iwram_CopyWordsEntry(gMarsLighthouseRoomM_HallPalette.colors,
                         gMarsLighthouseRoomM_HallFlameColors.colors, 64);
    gMarsLighthouseRoomM_HallCycleFrame = 0;
  }
  base = gMarsLighthouseRoomM_HallCycleFrame / 8 % 6;
  for (i = 0; i <= 5; i++) {
    gMarsLighthouseRoomM_HallPalette.colors[i + 4] =
        gMarsLighthouseRoomM_HallFlameColors.colors[(base + i) % 6 + 4];
    gMarsLighthouseRoomM_HallPalette.colors[i + 10] =
        gMarsLighthouseRoomM_HallFlameColors.colors[(base + i) % 6 + 10];
    gMarsLighthouseRoomM_HallPalette.colors[i + 20] =
        gMarsLighthouseRoomM_HallFlameColors.colors[(base + i) % 6 + 20];
    gMarsLighthouseRoomM_HallPalette.colors[i + 26] =
        gMarsLighthouseRoomM_HallFlameColors.colors[(base + i) % 6 + 26];
  }
  if (!Event_GetFlag(FLAG_VENUS_FLAME_LIT))
    Iwram_CopyWordsEntry(&gMarsLighthouseRoomM_HallPalette.colors[10],
                         gMarsLighthouseRoomM_UnlitFlameColors, 12);
  if (!Event_GetFlag(FLAG_MERCURY_FLAME_LIT))
    Iwram_CopyWordsEntry(&gMarsLighthouseRoomM_HallPalette.colors[20],
                         gMarsLighthouseRoomM_UnlitFlameColors, 12);
  if (!Event_GetFlag(FLAG_MARS_FLAME_LIT))
    Iwram_CopyWordsEntry(&gMarsLighthouseRoomM_HallPalette.colors[26],
                         gMarsLighthouseRoomM_UnlitFlameColors, 12);
  if (!Event_GetFlag(FLAG_JUPITER_FLAME_LIT))
    Iwram_CopyWordsEntry(&gMarsLighthouseRoomM_HallPalette.colors[4],
                         gMarsLighthouseRoomM_UnlitFlameColors, 12);
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(8, 0),
                      (u32)gMarsLighthouseRoomM_HallPalette.colors, 0x84000010);
  gMarsLighthouseRoomM_HallCycleFrame++;
}

// XXX: what the hall's four doorways are to the player (a layer that fades in
// and out over them, their floor turning from picture 122 to 125, the way
// through them opening when it is fully shown); a render of the thawed hall
// beside cells 20 to 27 would settle it.
/// Each frame the doorway's layer fades one step toward its target; fully
/// faded in or out, the cells under it open or close.
void MarsLighthouseRoomM_FadeDoorway(void) {
  s32 step;

  if (gMarsLighthouseRoomM_DoorwayBlend ==
      gMarsLighthouseRoomM_DoorwayBlendTarget)
    return;
  step = gMarsLighthouseRoomM_DoorwayBlendTarget -
         gMarsLighthouseRoomM_DoorwayBlend;
  if (step < 0)
    step = -1;
  else
    step = 1;
  gMarsLighthouseRoomM_DoorwayBlend += step;
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA,
                      BLEND_ALPHA(gMarsLighthouseRoomM_DoorwayBlend,
                                  16 - gMarsLighthouseRoomM_DoorwayBlend),
                      0x20000);
  if (gMarsLighthouseRoomM_DoorwayBlend == 0) {
    if (gMarsLighthouseRoomM_Doorway == 0)
      MapGrid_CopyCellAttributes(34, 8, 3, 3, 17, 8);
    if (gMarsLighthouseRoomM_Doorway == 1)
      MapGrid_CopyCellAttributes(30, 8, 3, 3, 8, 8);
    if (gMarsLighthouseRoomM_Doorway == 2)
      MapGrid_CopyCellAttributes(34, 8, 3, 3, 17, 8);
    if (gMarsLighthouseRoomM_Doorway == 3)
      MapGrid_CopyCellAttributes(30, 8, 3, 3, 8, 8);
  }
  if (gMarsLighthouseRoomM_DoorwayBlend == 16) {
    if (gMarsLighthouseRoomM_Doorway == 0)
      MapGrid_CopyCellAttributes(34, 0, 3, 3, 17, 8);
    if (gMarsLighthouseRoomM_Doorway == 1)
      MapGrid_CopyCellAttributes(30, 4, 3, 3, 8, 8);
    if (gMarsLighthouseRoomM_Doorway == 2)
      MapGrid_CopyCellAttributes(34, 4, 3, 3, 17, 8);
    if (gMarsLighthouseRoomM_Doorway == 3)
      MapGrid_CopyCellAttributes(30, 0, 3, 3, 8, 8);
  }
}

/// Stepping on one of the cells before a doorway (20 to 27, two to each): the
/// last doorway's floor goes dark, this one's lights, and its layer fades in
/// from the near cell or out from the far one.
void MarsLighthouseRoomM_StepTowardDoorway(s32 state) {
  s32 next;
  s32 odd;
  u16 *flash;

  if (state == gMarsLighthouseRoomM_DoorwayStep)
    return;
  if ((state ^ gMarsLighthouseRoomM_DoorwayStep) & 1)
    Event_PlaySound(SE_MARS_LIGHTHOUSE_DOORWAY);
  gMarsLighthouseRoomM_DoorwayStep = state;
  odd = state & 1;
  flash = (u16 *)&gMarsLighthouseRoomM_DoorwayBlendTarget;
  next = state - 20;
  if (odd)
    *flash = 0;
  else
    *flash = 16;
  if (gMarsLighthouseRoomM_Doorway == 0)
    MapGrid_CopyCellsLayered(0, 122, 3, 3, 17, 67);
  if (gMarsLighthouseRoomM_Doorway == 1)
    MapGrid_CopyCellsLayered(0, 122, 3, 3, 8, 72);
  if (gMarsLighthouseRoomM_Doorway == 2)
    MapGrid_CopyCellsLayered(0, 122, 3, 3, 17, 72);
  if (gMarsLighthouseRoomM_Doorway == 3)
    MapGrid_CopyCellsLayered(0, 122, 3, 3, 8, 67);
  gMarsLighthouseRoomM_Doorway = next / 2;
  if (gMarsLighthouseRoomM_Doorway == 0)
    MapGrid_CopyCellsLayered(0, 125, 3, 3, 17, 67);
  if (gMarsLighthouseRoomM_Doorway == 1)
    MapGrid_CopyCellsLayered(0, 125, 3, 3, 8, 72);
  if (gMarsLighthouseRoomM_Doorway == 2)
    MapGrid_CopyCellsLayered(0, 125, 3, 3, 17, 72);
  if (gMarsLighthouseRoomM_Doorway == 3)
    MapGrid_CopyCellsLayered(0, 125, 3, 3, 8, 67);
}

/// As the statue speaks, the four doorways' floors light and their layer
/// flickers, then all goes dark again.
void MarsLighthouseRoomM_FlashDoorways(void) {
  MapGrid_CopyCellsLayered(0, 125, 3, 3, 17, 67);
  MapGrid_CopyCellsLayered(0, 125, 3, 3, 8, 72);
  MapGrid_CopyCellsLayered(0, 125, 3, 3, 17, 72);
  MapGrid_CopyCellsLayered(0, 125, 3, 3, 8, 67);
  System_Wait(1);
  gMarsLighthouseRoomM_DoorwayBlendTarget = 12;
  System_Wait(12);
  gMarsLighthouseRoomM_DoorwayBlendTarget = 8;
  System_Wait(4);
  gMarsLighthouseRoomM_DoorwayBlendTarget = 12;
  System_Wait(4);
  gMarsLighthouseRoomM_DoorwayBlendTarget = 8;
  System_Wait(4);
  gMarsLighthouseRoomM_DoorwayBlendTarget = 12;
  System_Wait(4);
  gMarsLighthouseRoomM_DoorwayBlendTarget = 0;
  System_Wait(12);
  MapGrid_CopyCellsLayered(0, 122, 3, 3, 17, 67);
  MapGrid_CopyCellsLayered(0, 122, 3, 3, 8, 72);
  MapGrid_CopyCellsLayered(0, 122, 3, 3, 17, 72);
  MapGrid_CopyCellsLayered(0, 122, 3, 3, 8, 67);
  System_Wait(1);
}

void BlendScript_Suspend(void);

/// On arriving in the hall: the doorway the party came through is shown, or
/// none.
void MarsLighthouseRoomM_StartDoorways(s32 entrance) {
  s32 cell = -1;
  s16 *blend;
  BlendScript_Suspend();
  blend = &gMarsLighthouseRoomM_DoorwayBlend;
  if (entrance == 8)
    cell = 20;
  if (entrance == 10)
    cell = 22;
  if (entrance == 12)
    cell = 24;
  if (entrance == 14)
    cell = 26;
  if (cell == -1) {
    *blend = 0;
    gMarsLighthouseRoomM_DoorwayBlendTarget = 0;
    gMarsLighthouseRoomM_Doorway = cell;
    gMarsLighthouseRoomM_DoorwayStep = cell;
  } else {
    s32 step;
    *blend = 15;
    gMarsLighthouseRoomM_DoorwayBlendTarget = 16;
    step = (cell - 20) / 2;
    gMarsLighthouseRoomM_Doorway = step;
    gMarsLighthouseRoomM_DoorwayStep = cell;
    if (step == 0)
      MapGrid_CopyCellsLayered(0, 125, 3, 3, 17, 67);
    if (gMarsLighthouseRoomM_Doorway == 1)
      MapGrid_CopyCellsLayered(0, 125, 3, 3, 8, 72);
    if (gMarsLighthouseRoomM_Doorway == 2)
      MapGrid_CopyCellsLayered(0, 125, 3, 3, 17, 72);
    if (gMarsLighthouseRoomM_Doorway == 3)
      MapGrid_CopyCellsLayered(0, 125, 3, 3, 8, 67);
  }
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA,
                      gMarsLighthouseRoomM_DoorwayBlend |
                          (16 - gMarsLighthouseRoomM_DoorwayBlend) << 8,
                      0x20000);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, 0x3f42, 0x20000);
  Task_AddFrame(MarsLighthouseRoomM_FadeDoorway, TASK_PASS_VBLANK);
}

/// Teleport on the broken floor: the party leaves for the aerie.
void MarsLighthouseRoomM_TeleportToAerie(void) {
  Teleport_StartShimmer();
  Teleport_PlaySparks();
  gMainRoutineWork.teleportState = 2;
  FieldEvent_SetExitRequest(1);
  FieldEvent_Wait(40);
  Teleport_StopShimmer();
  FieldTask_Remove(MarsLighthouseRoomM_FadeDoorway);
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_MARS_LIGHTHOUSE_ROOM_M_FLAME)
    return gMarsLighthouseRoomM_Room6Triggers;
  if (scene == SCENE_MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS)
    return gMarsLighthouseRoomM_Room1Triggers;
  if (scene == SCENE_MARS_LIGHTHOUSE_MERCURY_LEDGE ||
      scene == SCENE_MARS_LIGHTHOUSE_JUPITER_LEDGE ||
      scene == SCENE_MARS_LIGHTHOUSE_ROOM_M_ALASTORS_HOOD_CHEST ||
      scene == SCENE_MARS_LIGHTHOUSE_VENUS_LEDGE)
    return gMarsLighthouseRoomM_Room5Triggers;
  return gMarsLighthouseRoomM_Triggers;
}

void Palette_SetColorsFade(u16 *slots, u16 *colours, u16 *a, u16 *b);

extern s16 gMarsLighthouseRoomM_FadeSlots[];
extern s16 gMarsLighthouseRoomM_FadeColorsFrom[];
extern s16 gMarsLighthouseRoomM_FadeColorsTo[];
extern u32 gMarsLighthouseRoomM_FadeOrder[];

s32 Scene_Initialize(void) {
  s32 entrance;
  s32 scene;

  scene = gMainRoutineWork.next;
  entrance = gMainRoutineWork.entrance;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_SetFlag(FLAG_IN_MARS_LIGHTHOUSE);
  Palette_SetColorsFade((s32)gMarsLighthouseRoomM_FadeSlots,
                        (s32)gMarsLighthouseRoomM_FadeColorsFrom,
                        (s32)gMarsLighthouseRoomM_FadeColorsTo,
                        (s32)gMarsLighthouseRoomM_FadeOrder);
  if (scene != SCENE_MARS_LIGHTHOUSE_ROOM_M_FLAME &&
      scene == SCENE_MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS) {
    Call1((void (*)())FieldEffect_StartBlankOverlay, 0);
    gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if ((u32)(entrance - 1) <= 1 || entrance == 8 || entrance == 10 ||
        entrance == 12 || entrance == 14) {
      s32 unset = Event_GetFlag(FLAG_WAY_TO_AERIE_OPEN);

      if (unset == 0 && Event_GetFlag(FLAG_VENUS_FLAME_LIT) &&
          Event_GetFlag(FLAG_MERCURY_FLAME_LIT) &&
          Event_GetFlag(FLAG_MARS_FLAME_LIT) &&
          Event_GetFlag(FLAG_JUPITER_FLAME_LIT)) {
        struct Actor *actor;

        Event_Start();
        FieldEvent_ResolvePendingActions(0);
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        Event_PlaySound(SOUND_FADE_OUT_BGM);
        FieldCamera_SetSpeed(2);
        FieldCamera_MoveTo(224, 64, 216, 1);
        FieldCamera_WaitForArrival();
        FieldEvent_Wait(20);
        PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
        PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_ALL);
        PaletteFade_Interpolate(20);
        System_Wait(20);
        entrance = 0x500021e;
        *(u16 *)entrance = 0x7fff;
        System_Wait(1);
        EventMessage_ShowCentered(
            (s32)MsgMarsLighthouseRoomMYouHaveProvenYourWorth, 0, 4);
        PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
        PaletteFade_Interpolate(20);
        System_Wait(20);
        FieldCamera_SetSpeed(1);
        FieldCamera_MoveTo(224, 16, 216, 1);
        FieldCamera_WaitForArrival();
        FieldEvent_Wait(40);
        actor = Actor_Get(ACTOR_STATUE_MOUTH);
        actor->motionFlags = unset;
        actor->y.fixed = toFIXED(16);
        actor->groundHeight = toFIXED(16);
        ((u8 *)actor)[98] = ACTOR_FIREBALL;
        actor->userValue = 0x4000;
        actor->watchedActor = (struct Actor *)unset;
        Actor_LaunchAhead(actor);
        System_Wait(40);
        MapGrid_SetShake(2, 2, 1);
        MarsLighthouseRoomM_BreakFloorTile(216, 216, 65, 65, 13, 13);
        MarsLighthouseRoomM_BreakFloorTile(232, 216, 66, 65, 14, 13);
        MarsLighthouseRoomM_BreakFloorTile(232, 232, 66, 66, 14, 14);
        MarsLighthouseRoomM_BreakFloorTile(216, 200, 65, 64, 13, 12);
        MarsLighthouseRoomM_BreakFloorTile(248, 200, 67, 64, 15, 12);
        MarsLighthouseRoomM_BreakFloorTile(248, 232, 67, 66, 15, 14);
        MarsLighthouseRoomM_BreakFloorTile(216, 232, 65, 66, 13, 14);
        MarsLighthouseRoomM_BreakFloorTile(200, 216, 64, 65, 12, 13);
        MarsLighthouseRoomM_BreakFloorTile(232, 200, 66, 64, 14, 12);
        MarsLighthouseRoomM_BreakFloorTile(248, 216, 67, 65, 15, 13);
        MarsLighthouseRoomM_BreakFloorTile(200, 232, 64, 66, 12, 14);
        MarsLighthouseRoomM_BreakFloorTile(200, 200, 64, 64, 12, 12);
        MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        MapGrid_CopyCellAttributes(30, 12, 4, 3, 12, 12);
        FieldEvent_Wait(40);
        Event_SetFlag(FLAG_WAY_TO_AERIE_OPEN);
        Event_PlaySound(MUS_PUZZLE_SOLVED);
        Sound_WaitForBgmRestore();
        Scene_PlayMusic();
        Event_End();
      }
    }
  }
  return 0;
}

void Weather_StartRain(s32 a);

static __inline__ void SetBlend(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                s32 g) {
  LineScroll_InitWave(a, b, c, d, e, f, g);
}

s32 Scene_PrepareMap(void) {
  s16 scene = gMainRoutineWork.next;
  s32 entrance = gMainRoutineWork.entrance;
  struct Actor *object;

  if (scene == SCENE_MARS_LIGHTHOUSE_ROOM_M_FLAME ||
      scene == SCENE_MARS_LIGHTHOUSE_MERCURY_LEDGE ||
      scene == SCENE_MARS_LIGHTHOUSE_JUPITER_LEDGE ||
      scene == SCENE_MARS_LIGHTHOUSE_ROOM_M_ALASTORS_HOOD_CHEST ||
      scene == SCENE_MARS_LIGHTHOUSE_VENUS_LEDGE) {
    Weather_StartRain(1);
    SetBlend(1, 0x10000, 0x40000, 0x20000, 0x10000, 0x8000, 0x4000);
  }
  if (scene == SCENE_MARS_LIGHTHOUSE_ROOM_M_FLAME) {
    switch (entrance) {
    case ENTRANCE_FROM_VENUS_TOWER:
      FieldActor_SetSpriteColor(Actor_Get(ACTOR_LEDGE_TORCH), SPRITE_COLOR_GRB);
      MarsLighthouseRoomM_ShowFlame(ACTOR_LEDGE_FLAME, FLAME_ANIMATION_VENUS);
      TilesetAnim_Resume(4);
      TilesetAnim_Resume(5);
      break;
    case ENTRANCE_FROM_MERCURY_TOWER:
      FieldActor_SetSpriteColor(Actor_Get(ACTOR_LEDGE_TORCH), SPRITE_COLOR_BGR);
      MarsLighthouseRoomM_ShowFlame(ACTOR_LEDGE_FLAME, FLAME_ANIMATION_MERCURY);
      TilesetAnim_Resume(6);
      TilesetAnim_Resume(7);
      break;
    case ENTRANCE_FROM_MARS_TOWER:
      FieldActor_SetSpriteColor(Actor_Get(ACTOR_LEDGE_TORCH),
                                SPRITE_COLOR_NORMAL);
      MarsLighthouseRoomM_ShowFlame(ACTOR_LEDGE_FLAME, FLAME_ANIMATION_MARS);
      TilesetAnim_Resume(0);
      TilesetAnim_Resume(1);
      break;
    case ENTRANCE_FROM_JUPITER_TOWER:
      FieldActor_SetSpriteColor(Actor_Get(ACTOR_LEDGE_TORCH), SPRITE_COLOR_GBR);
      MarsLighthouseRoomM_ShowFlame(ACTOR_LEDGE_FLAME, FLAME_ANIMATION_JUPITER);
      TilesetAnim_Resume(2);
      TilesetAnim_Resume(3);
      break;
    }
    object = Actor_Get(ACTOR_LEDGE_BLAZE_TARGET);
    object->motionFlags = 0;
    object->z.fixed = toFIXED(104);
    FieldTask_AddFrame(MarsLighthouseRoomM_CycleLedgeFlame, TASK_PASS_VBLANK);
  } else if (scene == SCENE_MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS) {
    if (entrance == 1 || entrance == 2 || entrance == 8 || entrance == 10 ||
        entrance == 12 || entrance == 14) {
      Ride_Create();
      if (Event_GetFlag(FLAG_WAY_TO_AERIE_OPEN)) {
        MapGrid_CopyCellAttributes(30, 12, 4, 3, 12, 12);
        MapGrid_CopyCellsLayered(64, 64, 4, 3, 76, 12);
      }
      MarsLighthouseRoomM_StartDoorways(entrance);
      {
        s32 priority = 0x480;

        FieldTask_AddFrame(MarsLighthouseRoomM_CycleHallFlames, priority);
      }
      MarsLighthouseRoomM_ShowFlame(ACTOR_VENUS_FLAME, FLAME_ANIMATION_VENUS);
      MarsLighthouseRoomM_ShowFlame(ACTOR_MERCURY_FLAME,
                                    FLAME_ANIMATION_MERCURY);
      MarsLighthouseRoomM_ShowFlame(ACTOR_MARS_FLAME, FLAME_ANIMATION_MARS);
      MarsLighthouseRoomM_ShowFlame(ACTOR_JUPITER_FLAME,
                                    FLAME_ANIMATION_JUPITER);
      if (!Event_GetFlag(FLAG_VENUS_FLAME_LIT))
        Actor_SetPosition(ACTOR_VENUS_FLAME, 0, 0);
      if (!Event_GetFlag(FLAG_MERCURY_FLAME_LIT))
        Actor_SetPosition(ACTOR_MERCURY_FLAME, 0, 0);
      if (!Event_GetFlag(FLAG_MARS_FLAME_LIT))
        Actor_SetPosition(ACTOR_MARS_FLAME, 0, 0);
      if (!Event_GetFlag(FLAG_JUPITER_FLAME_LIT))
        Actor_SetPosition(ACTOR_JUPITER_FLAME, 0, 0);
    }
  }
  if (Event_GetFlag(FLAG_MARS_LIGHTHOUSE_THAWED)) {
    extern char SceneId_MarsLighthouseJupiterLedgeSecond __asm__(
        "SceneId_MarsLighthouseJupiterLedge");

    Map_CopyPalette(
        gMainRoutineWork.mapId +
            ((s32)&SceneId_MarsLighthouseJupiterLedgeSecond - SCENE_PROX),
        0x5000000);
  }
  return 0;
}

/// "Mankind... With the power of wisdom, they ruled the earth."
void MarsLighthouseRoomM_ReadEarthInscription(void) {
  u16 *color = (u16 *)&OBJ_PLTT_COLOR(0, 15);

  Event_Start();
  Event_ResolvePendingActions(0);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_SetTarget(PALETTE_TINT_WARM, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  *color = 0x7fff;
  wait(1);
  EventMessage_ShowCentered((s32)MsgMarsLighthouseRoomMMankindWithPowerOfWisdom,
                            0, 0);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  Event_End();
}

/// "Fish... With cold courage, they ruled the water."
void MarsLighthouseRoomM_ReadWaterInscription(void) {
  u16 *color = (u16 *)&OBJ_PLTT_COLOR(0, 15);

  Event_Start();
  Event_ResolvePendingActions(0);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_SetTarget(PALETTE_TINT_WARM, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  *color = 0x7fff;
  wait(1);
  EventMessage_ShowCentered((s32)MsgMarsLighthouseRoomMFishWithColdCourageThey,
                            0, 0);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  Event_End();
}

/// "Dragons... Burning with might, they ruled the fires."
void MarsLighthouseRoomM_ReadFireInscription(void) {
  u16 *color = (u16 *)&OBJ_PLTT_COLOR(0, 15);

  Event_Start();
  Event_ResolvePendingActions(0);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_SetTarget(PALETTE_TINT_WARM, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  *color = 0x7fff;
  wait(1);
  EventMessage_ShowCentered(
      (s32)MsgMarsLighthouseRoomMDragonsBurningWithMightThey, 0, 0);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  Event_End();
}

/// "Bird... On the wings of truth, they ruled the winds."
void MarsLighthouseRoomM_ReadWindInscription(void) {
  u16 *color = (u16 *)&OBJ_PLTT_COLOR(0, 15);

  Event_Start();
  Event_ResolvePendingActions(0);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_SetTarget(PALETTE_TINT_WARM, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  *color = 0x7fff;
  wait(1);
  EventMessage_ShowCentered((s32)MsgMarsLighthouseRoomMBirdOnWingsOfTruth, 0,
                            0);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  Event_End();
}

/// A piece of the broken floor: one that bounces up and back, or one that
/// flies off at random.
void MarsLighthouseRoomM_SpawnRubble(s32 x, s32 z, s32 kind) {
  struct Actor *object;

  object = FieldActor_Create(403, toFIXED(x), 0, toFIXED(z));
  if (object != 0) {
    Actor_SetSpriteFlags(object, 0);
    Actor_SelectAnimation(object, 3);
    if (kind != 0) {
      ((struct MarsLighthouseRoomMAnimation *)object->sprite)->layer = 2;
      Actor_StartScript(object, gMarsLighthouseRoomM_RubbleBounceScript);
    } else {
      ((struct MarsLighthouseRoomMAnimation *)object->sprite)->layer = 1;
      object->velocityX = ((Random_Below(8)) - 4) << 16;
      object->velocityY = toFIXED(4);
      Actor_StartScript(object, gMarsLighthouseRoomM_RubbleFallScript);
    }
  }
}

/// One floor tile before the statue breaks with a clatter and is gone.
void MarsLighthouseRoomM_BreakFloorTile(s32 x, s32 z, s32 c, s32 d, s32 e,
                                        s32 f) {
  u32 i;
  s32 px;
  s32 pz;

  Event_PlaySound(SE_CLATTER);
  MarsLighthouseRoomM_SpawnRubble(x, z + 12, 1);
  for (i = 0; i < 3; i++) {
    px = x + (Random_Below(12)) - 6;
    pz = z + (Random_Below(12)) - 6;
    MarsLighthouseRoomM_SpawnRubble(px, pz, 0);
  }
  wait(4);
  MapGrid_CopyCellsTo(c, d, e + 64, f, 1, 1);
}

/// Before the great dragon statue: without the Mars Star it sends the party
/// away, once; with it (and, from patch 1, the lighthouse turned to fire) it
/// bids them set the four spirits aflame, and the four doorways flash.
void MarsLighthouseRoomM_HearDragonStatue(void) {
  if (PartyInventory_FindMember(ITEM_MYTHRIL_BAG_MARS_STAR) == -1) {
    if (!Event_GetFlag(FLAG_TOLD_TO_BEGONE)) {
      u16 *color = (volatile u16 *)&OBJ_PLTT_COLOR(0, 15);

      Event_Start();
      Event_ResolvePendingActions(0);
      FieldCamera_SetSpeed(2);
      FieldCamera_MoveTo(224, 64, 216, 1);
      FieldCamera_WaitForArrival();
      FieldEvent_Wait(20);
      PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
      PaletteFade_SetTarget(PALETTE_TINT_WARM, 0);
      PaletteFade_StartInterpolation(20);
      wait(20);
      *color = 0x7fff;
      wait(1);
      EventMessage_ShowCentered((s32)MsgMarsLighthouseRoomMIfYouDoNotHold, 0,
                                4);
      PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
      PaletteFade_StartInterpolation(20);
      wait(20);
      Event_SetFlag(FLAG_TOLD_TO_BEGONE);
      Event_End();
    }
  }
#if !TLA_PATCH_1
  else {
#else
  else if (Event_GetFlag(FLAG_MARS_LIGHTHOUSE_THAWED)) {
#endif
    u16 *color = (u16 *)&OBJ_PLTT_COLOR(0, 15);

    Event_Start();
    Event_ResolvePendingActions(0);
    FieldCamera_SetSpeed(2);
    FieldCamera_MoveTo(224, 64, 216, 1);
    FieldCamera_WaitForArrival();
    FieldEvent_Wait(20);
    PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
    PaletteFade_SetTarget(PALETTE_TINT_WARM, 0);
    PaletteFade_StartInterpolation(20);
    wait(20);
    *color = 0x7fff;
    wait(1);
    EventMessage_ShowCentered((s32)MsgMarsLighthouseRoomMYouWhoHoldStarOpen, 0,
                              4);
    wait(10);
    EventMessage_ShowCentered((s32)MsgMarsLighthouseRoomMYouWhoHoldStarOpen + 1,
                              0, 3);
    FieldCamera_MoveTo(224, 88, 216, 1);
    FieldCamera_WaitForArrival();
    FieldEvent_Wait(20);
    Event_PlaySound(SE_MARS_LIGHTHOUSE_DOORWAY);
    MarsLighthouseRoomM_FlashDoorways();
    FieldEvent_Wait(20);
    PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
    PaletteFade_StartInterpolation(20);
    wait(40);
    Event_SetFlag(FLAG_TOLD_TO_LIGHT_FLAMES);
    Event_End();
  }
}

extern u16 *gPaletteCycle_FlashColors;
extern u16 *gPaletteCycle_Colors;
extern u16 *gPaletteCycle_Order;

void Teleport_ArriveWhirling(void);

/// Records the four palette lists, writes each listed palette entry from its
/// red, green and blue, then copies two palettes from block32 and starts the
/// blend.
void Palette_SetColorsFade(u16 *slots, u16 *colours, u16 *a, u16 *b) {
  u8 *saved = Ram_HeapSlots->paletteFade;

  gPaletteCycle_Slots = slots;
  gPaletteCycle_FlashColors = colours;
  gPaletteCycle_Colors = a;
  gPaletteCycle_Order = b;
  if (*slots != 0xffff && *colours != 0xffff) {
  next:
    {
      int red = *colours++;
      int index = *slots++;
      int green = *colours++;
      int blue = *colours++;
      SET_RGB(((u16 *)BG_PLTT)[index], red, green, blue);
    }
    if (*slots != 0xffff && *colours != 0xffff)
      goto next;
  }
  Dma_Set((const void *)BG_PLTT, saved, DMA_COPY_WORDS(112), REG_DMA3);
  Dma_Set((const void *)OBJ_PLTT, saved + 448, DMA_COPY_WORDS(112), REG_DMA3);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  if (gMainRoutineWork.teleportState == 2)
    Teleport_ArriveWhirling();
}

/// Starts the shimmer of the circle's colours, stepping first after `countdown`
/// frames and then quicker down to every other frame.
#define DEFINE_TELEPORT_START_SHIMMER(name, period, countdown)                 \
  void name(void) {                                                            \
    gPaletteCycle_Step = 0;                                                    \
    gPaletteCycle_Count = List_CountEntries(gPaletteCycle_Slots);              \
    gPaletteCycle_Period = period;                                             \
    gPaletteCycle_Countdown = countdown;                                       \
    gPaletteCycle_Flash = 1;                                                   \
    Task_AddFrame(PaletteCycle_Fade, TASK_PASS_VBLANK);                        \
  }

DEFINE_TELEPORT_START_SHIMMER(Teleport_StartShimmer, 15, 16)

extern u8 gInventorySnapshot[];
void Actor_SetSpriteColorMode(struct Actor *object, s32 value);
void Actor_Release(void *handle);
s32 Leader_KnockBack(void);
s32 Fireball_CarryLeader(void);
void Actor_LaunchAhead(struct Actor *self);

void FieldCamera_MoveToActorById(s32 a, s32 b);
void FieldCamera_WaitForArrival(void);
void Teleport_StartShimmer(void);
void Teleport_PlaySparks(void);
void Teleport_StopShimmer(void);
void MarsLighthouseRoomM_FadeDoorway(void);
void PaletteCycle_Fade(void);

/// Stops the shimmer.
void Teleport_StopShimmer(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  FieldTask_Remove(PaletteCycle_Fade);
}

DEFINE_TELEPORT_START_SHIMMER(Teleport_StartFastShimmer, 2, 2)

#include "shared/list_count_entries.h"

extern u16 *gPaletteCycle_Order;
extern u16 *gPaletteCycle_FlashColors;
extern u16 *gPaletteCycle_Colors;

void PaletteCycle_Fade(void) {
  s32 step;
  u32 i;
  u16 *src;
  volatile u16 *dst;
  int red, green, blue;

  step = 0;
  if (gPaletteCycle_Countdown != 0) {
    if (--gPaletteCycle_Countdown == 0) {
      step = 1;
      gPaletteCycle_Countdown = gPaletteCycle_Period;
      if (gPaletteCycle_Period > 2)
        gPaletteCycle_Period--;
    }
  }
  if (step == 0)
    return;
  src = gPaletteCycle_Colors;
  for (i = 0; i < gPaletteCycle_Count; i++) {
    dst =
        &BG_PLTT[gPaletteCycle_Order[gPaletteCycle_Step * gPaletteCycle_Count +
                                     i]];
    red = *src++;
    green = *src++;
    blue = *src++;
    SET_RGB(*dst, red, green, blue);
  }
  dst = &BG_PLTT[gPaletteCycle_Slots[gPaletteCycle_Count]];
  if (gPaletteCycle_Flash != 0)
    src = gPaletteCycle_FlashColors + gPaletteCycle_Count * 3;
  else
    src = gPaletteCycle_Colors + gPaletteCycle_Count * 3;
  red = *src++;
  green = *src++;
  blue = *src++;
  SET_RGB(*dst, red, green, blue);
  gPaletteCycle_Flash ^= 1;
  if (++gPaletteCycle_Step == gPaletteCycle_Count)
    gPaletteCycle_Step = 0;
}

/// Arriving by Teleport: the screen comes up from white as the circle whirls.
void Teleport_ArriveWhirling(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 saved;

  saved = work->transition;
  PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(8, 8, 8), PALETTES_BG);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Teleport_StartFastShimmer();
  Teleport_PlayWhirl();
  System_Wait(40);
  Teleport_StopShimmer();
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_Interpolate(16);
  System_Wait(16);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->teleportState = 0;
  }
  work->transition = saved;
}

#include "shared/actor_is_touching.h"

#include "shared/actor_is_beside.h"

s32 Actor_IsTouching(struct Actor *a, struct Actor *b);
s32 Actor_IsBeside(struct Actor *a, struct Actor *b);

#include "shared/ride_update_carried.h"

void Actor_SetSpriteFlags(struct Actor *object, u32 flags);

#include "shared/actor_reset_scripted.h"

void Ride_SpawnForward(struct MarsLighthouseRoomMVector *position, s32 value,
                       s32 flag) {
  struct Actor *object = FieldActor_Create(SPRITE_BLAZE_FLAME, position->x,
                                           position->y, position->z);

  if (object != 0) {
    object->bounce = 0;
    object->gravity = 0;
    object->riseBaseY = value;
    ((void (*)())Actor_ResetScripted)(object, 2);
    object->update = Flame_UpdateForward;
    if (flag != 0)
      object->retainSpriteCache = 2;
    Actor_SelectAnimation(object, 2);
  }
}

extern u8 gInventorySnapshot[];

s32 Actor_IsTouching(struct Actor *a, struct Actor *b);
s32 Actor_IsBeside(struct Actor *a, struct Actor *b);

#include "shared/ride_update_landing.h"

void Actor_SetSpriteFlags(struct Actor *object, u32 flags);

#include "shared/actor_setup_scripted_part.h"

void Ride_SpawnBackward(struct MarsLighthouseRoomMVector *position, s32 value,
                        s32 flag) {
  struct Actor *object = FieldActor_Create(SPRITE_BLAZE_FLAME, position->x,
                                           position->y, position->z);

  if (object != 0) {
    object->bounce = 0;
    object->gravity = 0;
    object->riseBaseY = -value;
    Actor_SetupScriptedPart(object, 3);
    object->update = Flame_UpdateForward;
    if (flag != 0)
      object->retainSpriteCache = 2;
    Actor_SelectAnimation(object, 2);
  }
}

void Ride_SpawnLandingForward(struct MarsLighthouseRoomMVector *position,
                              s32 value, s32 flag) {
  struct Actor *object = FieldActor_Create(SPRITE_BLAZE_FLAME, position->x,
                                           position->y, position->z);

  if (object != 0) {
    object->bounce = 0;
    object->gravity = 0;
    object->riseBaseY = value;
    Actor_SetupScriptedPart(object, 3);
    object->update = Flame_UpdateLanding;
    if (flag != 0)
      object->retainSpriteCache = 2;
    Actor_SelectAnimation(object, 2);
  }
}

void Ride_SpawnLandingBackward(struct MarsLighthouseRoomMVector *position,
                               s32 value, s32 flag) {
  struct Actor *object = FieldActor_Create(SPRITE_BLAZE_FLAME, position->x,
                                           position->y, position->z);

  if (object != 0) {
    object->bounce = 0;
    object->gravity = 0;
    object->riseBaseY = -value;
    Actor_SetupScriptedPart(object, 3);
    object->update = Flame_UpdateLanding;
    if (flag != 0)
      object->retainSpriteCache = 2;
    Actor_SelectAnimation(object, 2);
  }
}

s32 Actor_IsAbove(struct Actor *a, struct Actor *b) {
  s32 height;

  if ((a->x.fixed - b->x.fixed >= 0 ? a->x.fixed - b->x.fixed
                                    : b->x.fixed - a->x.fixed) < toFIXED(12)) {
    height = a->y.fixed - b->y.fixed;
    if (height > 0 && height < toFIXED(16)) {
      if ((a->z.fixed - b->z.fixed >= 0 ? a->z.fixed - b->z.fixed
                                        : b->z.fixed - a->z.fixed) < toFIXED(8))
        return 1;
    }
  }
  return 0;
}

void Ride_UpdateDrift(struct Actor *object) {
  struct Actor *target = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 vx;
  s32 vz;
  s32 rate;
  struct MarsLighthouseRoomMAnimation *animation;

  vx = object->bounce;
  object->x.fixed += vx;
  object->y.fixed += object->gravity;
  vz = object->riseBaseY;
  object->z.fixed += vz;
  object->bounce = vx - vx / 18;
  object->riseBaseY = vz - vz / 16;
  rate = object->speedLimit;
  object->scaleX += rate;
  animation = object->sprite;
  object->scaleY += object->acceleration;
  animation->angle += object->timer;
  if (target->y.fixed == target->groundHeight &&
      Actor_IsAbove(object, target)) {
    object->z.fixed -= object->riseBaseY;
    object->gravity += object->riseBaseY >> 2;
    object->riseBaseY = 0;
  }
}

#include "shared/actor_setup_scripted.h"

void Ride_SpawnSpark(struct MarsLighthouseRoomMVector *position, s32 value,
                     s32 mode) {
  struct Actor *object = FieldActor_Create(
      SPRITE_DUST_CLOUD, position->x, position->y, position->z - toFIXED(2));
  s32 spin;

  if (object != 0) {
    object->bounce = Trig_Cos(((u32)(Random16() * 3 << 12) >> 16) + 0x3000);
    object->gravity = (u16)Random16();
    object->riseBaseY = ((u32)(Random16() << 17) >> 16) + value;
    spin = (u16)Random16() - 0x8000;
    object->timer = spin;
    ((void (*)())Actor_SetupScripted)(object, 2);
    object->update = Ride_UpdateDrift;
    Actor_SetBlendMode(object, 1);
  }
}

typedef struct Actor Actor;

/// A flame thrower of the ride work, as FlameThrowers_Puff reads it.
typedef struct Entry {
  s16 active;
  s16 timer; ///< frames to its next puff
  u16 flags; ///< enum FlameThrowerFlags: which flame, or none
  /// Puffs even off screen, and its flames keep their sprites.
  s16 offScreen;
  struct Actor *actor;
  FIXED reach; ///< 16.16 pixels, from the flags' low four bits
} Entry;

/// The ride work as FlameThrowers_Puff reads it: a flame reaching its stop
/// sets stopped, which calls back once.
typedef struct State {
  u8 unusedBeforeStopped[6];
  s16 stopped;
  s16 wasStopped;
  u8 unusedAfterWasStopped[4];
  s16 runsDuringPsynergy; ///< Ride_SetMode: not paused by Psynergy
  void (*onStopped)();
  Entry entries[4];
} State;

/// The heap slots as FlameThrowers_Puff reads them: the field work through a
/// union, which keeps its read after the stores before it.
typedef struct HeapSlotsView {
  u8 unusedBeforeMapGrid[32];
  struct MapWork *mapGrid;
  u8 unusedBeforeFieldWork[108 - 36];
  union {
    s32 word;
    void *pointer;
  } fieldWork;
} HeapSlotsView;

/// Each frame, every flame thrower in view sends its flame: forward, landing
/// forward or back, back, or a spark, by its flags, every ninth frame. They
/// pause while a menu is open, or a Psynergy is used unless the ride work runs
/// them through it; reaching a stop calls the stop's callback once.
void FlameThrowers_Puff(void) {
  s32 saved[2];
  State *state;
  Entry *entry;
  struct MapWork *map;
  s32 *view;
  s32 cameraX, cameraZ;
  s32 v;
  s32 i;

  state = (State *)RIDE_WORK;
  map = ((HeapSlotsView *)Ram_HeapSlots)->mapGrid;
  entry = state->entries;
  view = &map->viewX;
  cameraX = view[0];
  cameraX &= FIELD_PIXEL_MASK;
  cameraZ = view[1] & FIELD_PIXEL_MASK;
  saved[1] = map->origin[1];
  saved[0] = ((HeapSlotsView *)Ram_HeapSlots)->fieldWork.word;
  if (state->stopped != 0 && state->wasStopped == 0 && state->onStopped != 0)
    state->onStopped();
  v = (u16)state->stopped;
  state->wasStopped = v;
  state->stopped = 0;
  for (i = 3; i >= 0; i--, entry++) {
    struct Actor *actor;
    s32 flag, x, y, offScreen, value, dx, dz;
    u8 *mark;

    if (entry->active == 0)
      continue;
    actor = entry->actor;
    if (actor == 0)
      continue;
    flag = GameFlag_Get(FLAG_MENU_OPEN);
    if (((struct FieldWork *)saved[0])->psynergy != 0)
      flag |= 1;
    mark = &actor->paused;
    *mark = flag;
    if (state->runsDuringPsynergy != 0)
      *mark = GameFlag_Get(FLAG_MENU_OPEN);
    dz = actor->z.fixed;
    dx = actor->x.fixed - cameraX;
    y = actor->y.fixed;
    dz -= cameraZ;
    y = dz - y;
    offScreen = entry->offScreen;
    x = dx >> 16;
    y = (y >> 16) - 8;
    if (offScreen == 0) {
      if ((u32)(x + 7) > 334)
        continue;
      if (y <= -48)
        continue;
      if (y > 239)
        continue;
    }
    y = (u16)entry->timer;
    if (entry->timer <= 0) {
      y = entry->flags & 0xf0;
      switch (y) {
      case FLAME_SPARK:
        Ride_SpawnSpark((void *)&entry->actor->x, entry->reach, offScreen);
        break;
      case FLAME_BACKWARD:
        Ride_SpawnBackward(&entry->actor->x.fixed, entry->reach, offScreen);
        break;
      case FLAME_LANDING_BACKWARD:
        Ride_SpawnLandingBackward(&entry->actor->x.fixed, entry->reach,
                                  offScreen);
        break;
      case FLAME_LANDING_FORWARD:
        Ride_SpawnLandingForward(&entry->actor->x.fixed, entry->reach,
                                 offScreen);
        break;
      case FLAME_FORWARD:
        Ride_SpawnForward(&entry->actor->x.fixed, entry->reach, offScreen);
        break;
      }
      value = 8;
    } else {
      value = y - 1;
    }
    entry->timer = value;
  }
}

void Actor_Release(void *handle);
s32 MapGrid_CheckCollision(struct Actor *object, s32 *position);

void Ride_Create(void) {
  struct MarsLighthouseRoomMRideWork *work =
      (struct MarsLighthouseRoomMRideWork *)RIDE_WORK;
  struct Actor *object;
  struct MarsLighthouseRoomMAnimation *animation;

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    Iwram_ClearWordsEntry(work, 116);
  object = (struct Actor *)FieldActor_Create(SPRITE_SMALL_FLAME, 1, 0, 0);
  object->retainSpriteCache = 2;
  Actor_SelectAnimation(object, 1);
  animation = object->sprite;
  work->handle = object;
  work->value0c = animation->vramBlock;
  animation->keepEntry = 1;
}

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

#include "shared/ride_release_handle.h"

void Ride_SetMode(s16 value) { RIDE_WORK->value0e = value; }

#include "shared/beam_start.h"

s32 Ride_GetEntryValue(s32 index) {
  return ((struct RideRow *)((u8 *)RIDE_WORK + index * 16))->entry.active;
}

void Ride_SetEntryValue(s32 index, s16 value) {
  ((struct RideRow *)((u8 *)RIDE_WORK + index * 16))->entry.active = value;
}

s32 Ride_GetCounter(void) { return RIDE_WORK->counter; }

#include "shared/ride_advance_counter.h"

#include "shared/ride_set_entry.h"

void Ride_SetEntryCount(s32 index, s16 value) {
  RIDE_WORK->entries[index].offScreen = value;
}

s32 Ride_GetEntryFlags(s32 index) {
  return ((struct RideRow *)((u8 *)RIDE_WORK + index * 16))->entry.flags;
}

#include "shared/ride_set_slot.h"

#include "shared/party_take_fall_damage.h"

#include "../field/field_effect_advance_spin.c"

#include "shared/actor_blink.h"

s32 MapGrid_GetCellAttributeLowNibbleOrBump(s32 terrain, s32 x, s32 z);

#define KNOCK_BACK Leader_KnockBack
#define KNOCK_BACK_TAKE_DAMAGE Party_TakeFallDamage
#define KNOCK_BACK_BLINK Actor_Blink
#define KNOCK_BACK_SET_ANIMATION Actor_SelectAnimation
#define KNOCK_BACK_CELL_ATTRIBUTE MapGrid_GetCellAttributeLowNibbleOrBump
#define KNOCK_BACK_CHECK_COLLISION MapGrid_CheckCollision
#include "shared/leader_knock_back.c"
#undef KNOCK_BACK
#undef KNOCK_BACK_TAKE_DAMAGE
#undef KNOCK_BACK_BLINK
#undef KNOCK_BACK_SET_ANIMATION
#undef KNOCK_BACK_CELL_ATTRIBUTE
#undef KNOCK_BACK_CHECK_COLLISION

/// Hit by the flames, the leader is knocked back.
void MarsLighthouseRoomM_KnockBackIfHit(void) {
  if (RIDE_WORK->active)
    Leader_KnockBack();
}

s32 MapGrid_GetCellAttributeLowNibbleOrBump(s32 layer, s32 x, s32 z);
void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

s32 Fireball_CarryLeader(void) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct FlameHitWork *ride;
  struct Actor *leader;
  // Unused: gives the stack frame its 12 bytes.
  UNUSED s32 unused[3];

  if (event->transitionActive == 0) {
    ScreenTransition_Start(event->transition, event->transitionFrames);
    event->transitionActive = 1;
  }
  Event_ClearFlag(FLAG_MENU_OPEN);
  event->psynergy = 0;
  event->fieldMenu = 0;
  event->pauseMenu = 0;
  event->aButton = 0;
  event->encounter = 0;
#ifdef UBFIX
  Party_TakeFallDamage();
#else
  // UB: the fall damage takes no arguments and returns nothing.
  ((s32 (*)())Party_TakeFallDamage)(-10, 0);
#endif
  FieldCamera_SetSpeed(7);
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  ride = RIDE_WORK;
  Event_SetFlag(FLAG_MENU_OPEN);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_ClearFlag(FLAG_MENU_OPEN);
  leader->update = (void (*)(void))Actor_Blink;
  leader->facing = FACING_SOUTH;
  Actor_SelectAnimation(leader, 49);
  while (ride->car->timer == 0) {
    s32 kind = MapGrid_GetCellAttributeLowNibbleOrBump(0, leader->x.fixed,
                                                       leader->z.fixed);

    if (kind != 7) {
      leader->x.fixed += ride->car->stepX;
      leader->z.fixed += ride->car->stepZ;
    } else {
      if (ride->car->stepX != 0)
        leader->x.fixed = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
      if (ride->car->stepZ != 0)
        leader->z.fixed = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
    }
    ride->active = 0;
    System_Wait(1);
  }
  FieldEvent_Wait(30);
  leader->update = NULL;
  Actor_SetSpriteColorMode(leader, 0);
  ride->active = 0;
#ifdef UBFIX
  return 0;
#endif
}

void Actor_Release(void *handle);

void FieldCamera_MoveToActorById(s32 a, s32 b);
void FieldCamera_WaitForArrival(void);
void Teleport_PlaySparks(void);

#include "shared/ride_if_active.h"

s32 MarsLighthouseRoomM_IsSliding(void) { return RIDE_WORK->active; }

/// A sprite as the crumbs copy it from the flame's: its OAM attributes, scale,
/// VRAM block, size and layer count, and its animation record.
struct SpriteView {
  u8 beforeOam[4];
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 attr2Rest : 6;
  u8 afterOam[2];
  s32 scale;
  u8 vramBlock;
  u8 unk11_0 : 1;
  u8 unk11_1 : 7;
  u16 rotation;
  u8 dimensions[2]; ///< width and height
  u8 beforeLayerCount[5];
  u8 layerCount;
  u8 beforeRecord[12];
  void *record;
};

/// The ride work as the crumbs read it: the VRAM block of the flame Ride_Create
/// makes, whose sprite the crumbs share, and that flame.
struct Work {
  u8 beforeVramBlock[12];
  u16 vramBlock;
  u8 beforeFlame[98];
  struct Actor *flame;
};

#define WORK ((struct Work *)RIDE_WORK)

void MarsLighthouseRoomM_ScatterCrumbs(struct Actor *actor) {
  struct Work *work = WORK;
  s32 i;

  if (work->flame == NULL)
    return;
  // A once-only loop: one object per call.
  for (i = 0; i >= 0; i--) {
    struct Actor *object;
    s32 a, b, c, d, e;
    s32 x, y, z;

    a = Random16();
    b = Random16();
    x = actor->x.fixed + a * 16;
    x -= b * 16;
    c = Random16();
    d = Random16();
    e = Random16();
    z = actor->z.fixed + d * 16 - e * 16;
    y = c * 8 + actor->y.fixed + FIXED_ONE * 4;
    object = (struct Actor *)Import_ActorCreate(489, x, y, z);
    if (object != NULL) {
      s32 angle, radius;
      u32 sum;
      s32 scale;
      struct SpriteView *sprite;
      struct SpriteView *source;
      s32 velocity;

      angle = Random16();
      radius = Random16();
      sum = Random16();
      sum += Random16();
      scale = (sum >> 2) + FIXED_ONE / 2;
      Actor_StartScript(object, gMarsLighthouseRoomM_CrumbScript);
      Actor_SetSpriteFlags(object, 0);
      object->velocityY = angle * 4 + FIXED_ONE * 2;
      radius <<= 3;
      object->velocityZ = Iwram_MulFX(radius, Trig_Cos(angle));
      velocity = Iwram_MulFX(radius, Trig_Sin(angle));
      object->gravity = toFIXED(33) / 100;
      object->scaleX = scale;
      object->scaleY = scale;
      sprite = object->sprite;
      object->velocityX = velocity;
      object->acceleration = 0;
      object->bounce = FIXED_ONE / 2;
      source = work->flame->sprite;
      VramCache_FreeEntry(sprite->vramBlock);
      sprite->unk11_0 = 1;
      sprite->vramBlock = work->vramBlock;
      SpriteAnimation_Clear(sprite->record);
      sprite->record = NULL;
      sprite->layerCount = 0;
      sprite->dimensions[0] = source->dimensions[0];
      sprite->dimensions[1] = source->dimensions[1];
      sprite->shape = source->shape;
      sprite->size = source->size;

      sprite->tile = source->tile;
    }
  }
}

#include "shared/embers_spawn.h"

void Actor_HoldDuringEvent(struct Actor *object) {
  object->paused = Event_GetFlag(FLAG_FIELD_HANDLING_REQUESTS);
}

#define ABS(x) ((x) < 0 ? -(x) : (x))

/// The ride work as a rolling actor reads it when it reaches the leader:
/// whether a hit is under way, the step trigger a hit raises, and the actor
/// that carries the leader off.
struct RideHitView {
  u8 beforeActive[4];
  u16 active;
  u8 beforeHitTrigger[4];
  u16 hitTrigger;
  u8 beforeCar[0x6c - 0x0c];
  struct Actor *car;
};

/// An actor rolls forward, crumbling the cells it passes.
void Actor_RollAndCrumble(struct Actor *actor) {
  struct Vector3 position;
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 state = actor->timer;

  if (state == 0) {
    s32 dz;
    s32 dx;
    s32 tileX;
    struct MapCell *cellsA;
    struct MapCell *cellsB;
    s32 widthA;
    s32 widthB;
    s32 offset;
    s32 zB;
    s32 tileZ;
    s32 heightA;
    s32 heightB;
    s32 x;
    s32 z;
    s32 xA;
    s32 xB;
    s32 zA;
    s32 flag;
    struct MapWork *map;
    struct MapCell *cell;
    struct FieldWork *fieldWork;
    struct RideHitView *other;
    struct Actor *target;

    flag = Event_GetFlag(FLAG_MENU_OPEN);
    if (work->psynergy != 0)
      flag |= 1;
    actor->paused = flag;

    position.x = actor->x.fixed;
    position.y = actor->y.fixed;
    position.z = actor->z.fixed;
    FieldVector_AddPolarOffset(8, actor->facing, &position);
    if (MapGrid_GetCellAttributeLowNibbleOrBump(0, position.x, position.z) ==
        7) {
      actor->targetZ = TARGET_UNSET;
      actor->targetY = TARGET_UNSET;
      actor->targetX = TARGET_UNSET;
      actor->acceleration = toFIXED(1) / 4;
      actor->timer = 1;
      Event_PlaySound(SE_CRASH);
      MapGrid_SetShake(5, 5, 1);
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
      if (actor->watchedActor != NULL)
        ((void (*)(struct Actor *))actor->watchedActor)(actor);
    }

    map = Ram_HeapSlots->mapGrid;
    tileX = FIELD_CELL(actor->x.fixed);
    tileZ = FIELD_CELL(actor->z.fixed);
    cellsA = &map->layers[1].cells[tileX + (tileZ << 7)];
    cellsB = &map->layers[2].cells[tileX + (tileZ << 7)];
    widthA = FIELD_CELL(map->layers[1].offsetX);
    heightA = FIELD_CELL(map->layers[1].offsetY);
    widthB = FIELD_CELL(map->layers[2].offsetX);
    heightB = FIELD_CELL(map->layers[2].offsetY);

    for (dz = -1, zB = tileZ + heightB - 2, z = tileZ - 1,
        zA = tileZ + heightA - 1;
         dz <= 1; zB++, z++, zA++, dz++) {
      for (dx = -1, xB = tileX + widthB - 1, x = tileX - 1,
          xA = tileX + widthA - 1;
           dx <= 1; xB++, x++, xA++, dx++) {
        offset = ((dz << 7) + dx) * 4;
        cell = (struct MapCell *)(offset + (s32)cellsA);
        if (cell->collisionCode == 77) {
          cell->collisionCode = 0;
          MapGrid_CopyCellsLayered(64, 64, 1, 1, xA, zA);
          MapGrid_CopyCellAttributes(64, 64, 1, 1, x, z);
          Event_PlaySound(SE_ICE_CRACK);
          Embers_Spawn(FIELD_CELL_CENTER(x), actor->y.fixed,
                       FIELD_CELL_CENTER(z), actor->facing);
        }
        cell = (struct MapCell *)(offset + (s32)cellsB);
        if (cell->collisionCode == 77) {
          cell->collisionCode = 0;
          MapGrid_CopyCellsLayered(64, 64, 1, 2, xB, zB);
          MapGrid_CopyCellAttributes(64, 64, 1, 1, x, z);
          Event_PlaySound(SE_CRUMBLE);
          Embers_Spawn(FIELD_CELL_CENTER(x), actor->y.fixed,
                       FIELD_CELL_CENTER(z), actor->facing);
        }
      }
    }

    if (actor->scaleX < FIXED_ONE) {
      actor->scaleX += FIXED_ONE / 16;
      actor->scaleY += FIXED_ONE / 16;
    }
    FieldActor_SetMoveTargetFX(actor, position.x, position.y, position.z);

    fieldWork = Ram_HeapSlots->fieldWork;
    other = (struct RideHitView *)RIDE_WORK;
    target = Actor_Get(gMainRoutineWork.leaderActorId);
    if (ABS(actor->x.fixed - target->x.fixed) < toFIXED(20) &&
        (u32)(actor->y.fixed - target->y.fixed + 0x7ffff) <= 0x1ffffe &&
        ABS(actor->z.fixed - target->z.fixed) < toFIXED(20)) {
      other->active = 2;
      other->car = actor;
      fieldWork->stepTrigger = other->hitTrigger + 2;
    }
  } else if (state == 1) {
    actor->paused = 0;
    if (actor->scaleX > 0) {
      actor->scaleX -= FIXED_ONE / 16;
      actor->scaleY -= FIXED_ONE / 16;
      MarsLighthouseRoomM_ScatterCrumbs(actor);
    } else {
      actor->z.fixed = 0;
      actor->y.fixed = 0;
      actor->x.fixed = 0;
      actor->velocityZ = 0;
      actor->velocityY = 0;
      actor->velocityX = 0;
      actor->update = NULL;
    }
  }
}

void Actor_Attach(s32 sourceId, s32 id, s32 value, s32 angle,
                  const void *script) {
  struct Actor *source = Actor_Get(sourceId);
  struct Actor *object = Actor_Get(id);

  FieldActor_SetPositionAndResetMotion(object, source->x.fixed, source->y.fixed,
                                       source->z.fixed);
  Actor_StartScript(object, script);
  Actor_SetSpriteFlags(object, 0);
  object->motionFlags = 0;
  Actor_SelectAnimation(object, 1);
  object->watchedActor = source;
  object->timer = 0;
  object->userValue = angle;
  ((u8 *)object)[0x62] = value;
  object->update = Actor_HoldDuringEvent;
}

#include "shared/actor_launch_ahead.h"

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Actor_SetSpritePriorityDirect(struct Actor *object, s32 mode);

#include "shared/actor_pop_up_slow.h"

#include "shared/actor_pop_up.h"

#include "shared/actor_park.h"

void MarsLighthouseRoomM_LightFlame(void);

s32 gMarsLighthouseRoomM_StatueScript[] = {
    SC_CALL(MarsLighthouseRoomM_SetUpStatue),
    SC_END,
};

// clang-format off
s32 gScriptedPart_Script[] = {                                                               
      SC_WAIT(30), SC_SET(SW_HOOK, 0), SC_POSITION(0, 0, 0), SC_WAIT(1),       
      SC_RELEASE,                                                                 
  };

u32 gMarsLighthouseRoomM_CrumbScript[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u32 gEmbers_Script[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

s16 gMarsLighthouseRoomM_FadeSlots[] = {
    201, 202, 203, 204, 205, 206, 207, -1,
};

s16 gMarsLighthouseRoomM_FadeColorsFrom[] = {
    23, 25, 22, 23, 25, 22, 23, 25,
    22, 23, 25, 22, 23, 25, 22, 23,
    25, 22, 23, 25, 22, -1,
};

s16 gMarsLighthouseRoomM_FadeColorsTo[] = {
    30, 27, 31, 22, 21, 31, 20, 18,
    31, 13, 11, 27, 11, 8, 24, 8,
    5, 21, 31, 31, 31, -1,
};

u32 gMarsLighthouseRoomM_FadeOrder[] = {
    0xc900ce, 0xcb00ca, 0xcd00cc, 0xce00cd, 0xca00c9, 0xcc00cb,
    0xcd00cc, 0xc900ce, 0xcb00ca, 0xcc00cb, 0xce00cd, 0xca00c9,
    0xcb00ca, 0xcd00cc, 0xc900ce, 0xca00c9, 0xcc00cb, 0xce00cd,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(MarsLighthouseRoomMFlame),
    SCENE_EXIT(MARS_LIGHTHOUSE_ROOM_M_FLAME_TO_MARS_LIGHTHOUSE_VENUS_TOWER, MarsLighthouseVenusTower, MARS_LIGHTHOUSE_VENUS_TOWER_FROM_MARS_LIGHTHOUSE_ROOM_M_FLAME),
    SCENE_EXIT(MARS_LIGHTHOUSE_ROOM_M_FLAME_TO_MARS_LIGHTHOUSE_MERCURY_TOWER, MarsLighthouseMercuryTower, MARS_LIGHTHOUSE_MERCURY_TOWER_FROM_MARS_LIGHTHOUSE_ROOM_M_FLAME),
    SCENE_EXIT(MARS_LIGHTHOUSE_ROOM_M_FLAME_TO_MARS_LIGHTHOUSE_MARS_TOWER, MarsLighthouseMarsTower, MARS_LIGHTHOUSE_MARS_TOWER_FROM_MARS_LIGHTHOUSE_ROOM_M_FLAME),
    SCENE_EXIT(MARS_LIGHTHOUSE_ROOM_M_FLAME_TO_MARS_LIGHTHOUSE_JUPITER_TOWER, MarsLighthouseJupiterTower, MARS_LIGHTHOUSE_JUPITER_TOWER_FROM_MARS_LIGHTHOUSE_ROOM_M_FLAME),
    SCENE_EXITS(MarsLighthouseRoomMInscriptions),
    SCENE_EXIT(MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS_TO_MARS_LIGHTHOUSE_AERIE, MarsLighthouseAerie, MARS_LIGHTHOUSE_AERIE_FROM_MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS),
    SCENE_EXIT(MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS_TO_MARS_LIGHTHOUSE_ENTRANCE_FUGUE, MarsLighthouseEntranceFugue, MARS_LIGHTHOUSE_ENTRANCE_FUGUE_FROM_MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS),
    SCENE_EXIT(MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS_TO_MARS_LIGHTHOUSE_VENUS_LEDGE, MarsLighthouseVenusLedge, MARS_LIGHTHOUSE_VENUS_LEDGE_FROM_MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS),
    SCENE_EXIT(MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS_TO_MARS_LIGHTHOUSE_MERCURY_LEDGE, MarsLighthouseMercuryLedge, MARS_LIGHTHOUSE_MERCURY_LEDGE_FROM_MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS),
    SCENE_EXIT(MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS_TO_MARS_LIGHTHOUSE_ROOM_M_ALASTORS_HOOD_CHEST, MarsLighthouseRoomMAlastorsHoodChest, MARS_LIGHTHOUSE_ROOM_M_ALASTORS_HOOD_CHEST_FROM_MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS),
    SCENE_EXIT(MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS_TO_MARS_LIGHTHOUSE_JUPITER_LEDGE, MarsLighthouseJupiterLedge, MARS_LIGHTHOUSE_JUPITER_LEDGE_FROM_MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS),
    SCENE_EXIT(7, MarsLighthouseRoomMInscriptions, 8),
    SCENE_EXIT(8, MarsLighthouseRoomMInscriptions, 7),
    SCENE_EXIT(9, MarsLighthouseRoomMInscriptions, 10),
    SCENE_EXIT(10, MarsLighthouseRoomMInscriptions, 9),
    SCENE_EXIT(11, MarsLighthouseRoomMInscriptions, 12),
    SCENE_EXIT(12, MarsLighthouseRoomMInscriptions, 11),
    SCENE_EXIT(13, MarsLighthouseRoomMInscriptions, 14),
    SCENE_EXIT(14, MarsLighthouseRoomMInscriptions, 13),
    SCENE_EXITS(MarsLighthouseMercuryLedge),
    SCENE_EXIT(MARS_LIGHTHOUSE_MERCURY_LEDGE_TO_MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS, MarsLighthouseRoomMInscriptions, MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS_FROM_MARS_LIGHTHOUSE_MERCURY_LEDGE),
    SCENE_EXIT(MARS_LIGHTHOUSE_MERCURY_LEDGE_TO_MARS_LIGHTHOUSE_MERCURY_TOWER, MarsLighthouseMercuryTower, MARS_LIGHTHOUSE_MERCURY_TOWER_FROM_MARS_LIGHTHOUSE_MERCURY_LEDGE),
    SCENE_EXITS(MarsLighthouseJupiterLedge),
    SCENE_EXIT(MARS_LIGHTHOUSE_JUPITER_LEDGE_TO_MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS, MarsLighthouseRoomMInscriptions, MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS_FROM_MARS_LIGHTHOUSE_JUPITER_LEDGE),
    SCENE_EXIT(MARS_LIGHTHOUSE_JUPITER_LEDGE_TO_MARS_LIGHTHOUSE_JUPITER_TOWER, MarsLighthouseJupiterTower, MARS_LIGHTHOUSE_JUPITER_TOWER_FROM_MARS_LIGHTHOUSE_JUPITER_LEDGE),
    SCENE_EXITS(MarsLighthouseRoomMAlastorsHoodChest),
    SCENE_EXIT(MARS_LIGHTHOUSE_ROOM_M_ALASTORS_HOOD_CHEST_TO_MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS, MarsLighthouseRoomMInscriptions, MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS_FROM_MARS_LIGHTHOUSE_ROOM_M_ALASTORS_HOOD_CHEST),
    SCENE_EXIT(MARS_LIGHTHOUSE_ROOM_M_ALASTORS_HOOD_CHEST_TO_MARS_LIGHTHOUSE_MARS_TOWER, MarsLighthouseMarsTower, MARS_LIGHTHOUSE_MARS_TOWER_FROM_MARS_LIGHTHOUSE_ROOM_M_ALASTORS_HOOD_CHEST),
    SCENE_EXITS(MarsLighthouseVenusLedge),
    SCENE_EXIT(MARS_LIGHTHOUSE_VENUS_LEDGE_TO_MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS, MarsLighthouseRoomMInscriptions, MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS_FROM_MARS_LIGHTHOUSE_VENUS_LEDGE),
    SCENE_EXIT(MARS_LIGHTHOUSE_VENUS_LEDGE_TO_MARS_LIGHTHOUSE_VENUS_TOWER, MarsLighthouseVenusTower, MARS_LIGHTHOUSE_VENUS_TOWER_FROM_MARS_LIGHTHOUSE_VENUS_LEDGE),
    SCENE_EXITS_END,
};

struct ActorPlacement gMarsLighthouseRoomM_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gMarsLighthouseRoomM_Room6Placements[] = {
    ACTOR_PLACEMENT(418, CONDITION_ALWAYS, (s32)gMarsLighthouseRoomM_StatueScript, 504, 0, 184, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, 7, 504, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, (s32)gMarsLighthouseRoomM_StatueScript, 504, 0, 112, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 232, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gMarsLighthouseRoomM_Room1Placements[] = {
    ACTOR_PLACEMENT(416, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 184, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(416, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 184, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(416, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 216, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(416, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 216, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 224, 0, 144, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gMarsLighthouseRoomM_Triggers[] = {
    {TRIGGER_TABLE_END},
};

s32 gMarsLighthouseRoomM_LedgeCycleFrame = -1;

struct Trigger gMarsLighthouseRoomM_Room6Triggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomM_LeaveLedge)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 8, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomM_BeginBlazeOnTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS), 10, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomM_WatchBlazeReachFlame)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 10, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomM_LightFlame)},
    {TRIGGER_TABLE_END},
};

s32 gMarsLighthouseRoomM_HallCycleFrame = -1;

struct Trigger gMarsLighthouseRoomM_Room1Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_STEP, 20, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), EVENT(MarsLighthouseRoomM_StepTowardDoorway)},
    {TRIGGER_STEP, 21, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), EVENT(MarsLighthouseRoomM_StepTowardDoorway)},
    {TRIGGER_STEP, 22, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), EVENT(MarsLighthouseRoomM_StepTowardDoorway)},
    {TRIGGER_STEP, 23, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), EVENT(MarsLighthouseRoomM_StepTowardDoorway)},
    {TRIGGER_STEP, 24, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), EVENT(MarsLighthouseRoomM_StepTowardDoorway)},
    {TRIGGER_STEP, 25, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), EVENT(MarsLighthouseRoomM_StepTowardDoorway)},
    {TRIGGER_STEP, 26, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), EVENT(MarsLighthouseRoomM_StepTowardDoorway)},
    {TRIGGER_STEP, 27, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), EVENT(MarsLighthouseRoomM_StepTowardDoorway)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), 1, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomM_TeleportToAerie)},
    {TRIGGER_EXAMINE, 30, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomM_ReadEarthInscription)},
    {TRIGGER_EXAMINE, 31, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomM_ReadWaterInscription)},
    {TRIGGER_EXAMINE, 32, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomM_ReadFireInscription)},
    {TRIGGER_EXAMINE, 33, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomM_ReadWindInscription)},
    {TRIGGER_STEP, 34, FLAG_TOLD_TO_LIGHT_FLAMES, EVENT(MarsLighthouseRoomM_HearDragonStatue)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMarsLighthouseRoomM_Room5Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TABLE_END},
};

s32 gMarsLighthouseRoomM_RubbleBounceScript[] = {
    SC_ADD(SW_SCALE_X, 0x3000),
    SC_ADD(SW_SCALE_Y, 0x3000),
    SC_ADD(SW_Y, -0x1800),
    SC_WAIT(1),
    SC_LOOP(8, SC_TOP),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -FIXED_ONE * 3 / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE * 3 / 16),
    SC_ADD(SW_Y, 0x1800),
    SC_WAIT(1),
    SC_LOOP(12, SC_TO(1)),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
};

s32 gMarsLighthouseRoomM_RubbleFallScript[] = {
    SC_WAIT(20),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
};
// clang-format on
