/// @file
/// The Mars Lighthouse's central hall and the ways to its four towers, in six
/// rooms that share this overlay.
///
/// In the hall (SceneId_MarsLighthouseRoomM1) four inscriptions name the
/// peoples who ruled earth, water, fire and wind, and the great dragon statue
/// speaks: without the Mars Star it sends the party away; with it, once the
/// lighthouse has turned to fire, it bids them set the four spirits aflame.
/// Four ledges in the rain (M2 to M5) lead from the hall to the four towers,
/// and each tower comes out on a beacon ledge (M6), where Blaze lights its
/// element's flame. With all four lit the statue says the heavens await, the
/// floor before it breaks open, and Teleport there carries the party up to the
/// aerie.

#include "flags.h"
#include "item_ids.h"
#include "module/scene/ride.h"
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
#include "field/field_sequence_event.h"
#include "module/scene/scene.h"
#include "module/sound/sound.h"
#include "module/system/system.h"
#include "module/tla.h"
#include "scene_ids.h"
#include "system/ram_buffer.h"
#include "system/frame.h"
#include "actor.h"
#include "field/event_inline.h"
#include "system/main_routine_work.h"
#include "system/iwram_call.h"
#include "system/io_write_queue.h"
#include "system/io_reg.h"
#include "call.h"
#include "version.h"
#include "system/dma.h"
#include "shared/mars_lighthouse_entrance_room.h"
#include "field/field_sequence_event.h"
#include "module/field/field.h"
#include "module/scene/dehkan_plateau.h"
#include "battle/battle_unit.h"
#include "system/vector3.h"
#include "fixed_math.h"
#include "shared/tables.h"
#include "graphics/display_transition.h"
#include "item_icon.h"
#include "map.h"
#include "field_sprite.h"
#include "macros.h"
#include "objcmd.h"
#include "field/sprite.h"

/// The Mars Star is set: the lighthouse has turned to fire and its ice is gone.
#define FLAG_MARS_LIGHTHOUSE_THAWED 0xa30
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

void Leader_KnockBack(void);

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

#include "text/msg_ids.h"

void Actor_Release(void *handle);
void FieldCamera_MoveToActorById(s32 a, s32 b);
void FieldCamera_WaitForArrival(void);
void Teleport_PlaySparks(void);
extern u16 gMarsLighthouseRoomM_LedgeSavedPalette[];
extern volatile u16 gMarsLighthouseRoomM_HallFlameColors[];
extern u16 gMarsLighthouseRoomM_HallPalette[];
extern const u16 gMarsLighthouseRoomM_UnlitFlameColors[];
extern s16 gMarsLighthouseRoomM_DoorwayBlend;
extern s16 gMarsLighthouseRoomM_DoorwayBlendTarget;
extern s16 gMarsLighthouseRoomM_Doorway;
extern s16 gMarsLighthouseRoomM_DoorwayStep;
void TileMap_ConvertBuffer(s32 scene, s32 a1);
u32 Map_ClearLayerEntryFlag(struct FieldSprite *sprite, u32 frame);

void FieldEffect_StartBlankOverlay(void);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

struct RideRow {
  u8 unk00[20];
  s16 a;
  u8 unk16[2];
  s16 b;
  s16 c;
  u8 unk1c[4];
  s32 d;
};

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

void Teleport_StopShimmer(void);

/// The ledge's torch and Blaze's mark start with their sprite flags clear.
s32 MarsLighthouseRoomM_SetUpStatue(struct Actor *object) {
  Actor_SetSpriteFlags(object, 0);
  return 0;
}

/// Shows a flame on its torch, 14 pixels up, in its element's animation.
void MarsLighthouseRoomM_ShowFlame(s32 id, s32 value) {
  struct Actor *object = Actor_Get(id);

  object->motionFlags = 0;
  object->y = px(14);
  Actor_SetSpriteFlags(object, 0);
  Actor_SelectAnimation(object, value);
}

#ifndef TLA_MARS_LIGHTHOUSE_ROOM_M_H
#define TLA_MARS_LIGHTHOUSE_ROOM_M_H

#include "types.h"
#include "system/system.h"
#include "scene_tables.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "map.h"
#include "system/input.h"
#include "system/iwram_call.h"
#include "text/msg_ids.h"
#include "system/frame.h"

struct MarsLighthouseRoomMVector {
  s32 x;
  s32 y;
  s32 z;
};

struct MarsLighthouseRoomMAnimation {
  u8 unk00[9];
  u8 unused09 : 2;
  u8 layer : 2;
  u8 unk0a[6];
  u8 value10;
  u8 flag11 : 1;
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
extern char SceneId_MarsLighthouseRoomM1;
extern char SceneId_MarsLighthouseRoomM2;
extern char SceneId_MarsLighthouseRoomM3;
extern char SceneId_MarsLighthouseRoomM4;
extern char SceneId_MarsLighthouseRoomM5;
extern char SceneId_MarsLighthouseRoomM6;

void Actor_SelectAnimation(struct Actor *object, s32 value);
void Actor_StartScript(struct Actor *object, const void *script);

void Flame_UpdateForward(struct Actor *object);
void Actor_ResetScripted(struct Actor *object);
void Actor_SetupScriptedPart(struct Actor *object, s32 layer);
void Flame_UpdateLanding(struct Actor *object);

extern u32 gScriptedPart_Script[];
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
extern u32 gMarsLighthouseRoomM_RubbleFallScript[];

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

const struct ActorPlacement *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_MarsLighthouseRoomM6)
    return gMarsLighthouseRoomM_Room6Placements;
  if (scene == (s32)&SceneId_MarsLighthouseRoomM1)
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
                         (void *)0x050001a0, 32);
    Iwram_CopyWordsEntry(gMarsLighthouseRoomM_LedgeFlameColors,
                         (void *)0x05000100, 64);
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

// XXX: what Blaze does otherwise once the field work's unkd38 is set (its
// mode 3 for 2 when it finds no torch); the rest of blaze.c's reading of the
// mode would settle it.
/// Blaze is cast at the ledge's torch.
void MarsLighthouseRoomM_BeginBlazeOnTorch(void) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;

  event->unkd38 = 1;
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
  PaletteFade_ApplyTarget(0x7fff, 0);
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
  PaletteFade_ApplyTarget(0x10000, 0);
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
                         (void *)0x05000040, 32);
    Iwram_CopyWordsEntry((void *)gMarsLighthouseRoomM_HallFlameColors,
                         (void *)0x05000100, 64);
    Iwram_CopyWordsEntry(gMarsLighthouseRoomM_HallPalette,
                         (void *)gMarsLighthouseRoomM_HallFlameColors, 64);
    gMarsLighthouseRoomM_HallCycleFrame = 0;
  }
  /* FAKEMATCH(volatile): the saved colours read as volatile. */
  base = gMarsLighthouseRoomM_HallCycleFrame / 8 % 6;
  for (i = 0; i <= 5; i++) {
    gMarsLighthouseRoomM_HallPalette[i + 4] =
        gMarsLighthouseRoomM_HallFlameColors[(base + i) % 6 + 4];
    gMarsLighthouseRoomM_HallPalette[i + 10] =
        gMarsLighthouseRoomM_HallFlameColors[(base + i) % 6 + 10];
    gMarsLighthouseRoomM_HallPalette[i + 20] =
        gMarsLighthouseRoomM_HallFlameColors[(base + i) % 6 + 20];
    gMarsLighthouseRoomM_HallPalette[i + 26] =
        gMarsLighthouseRoomM_HallFlameColors[(base + i) % 6 + 26];
  }
  if (!Event_GetFlag(FLAG_VENUS_FLAME_LIT))
    Iwram_CopyWordsEntry(&gMarsLighthouseRoomM_HallPalette[10],
                         gMarsLighthouseRoomM_UnlitFlameColors, 12);
  if (!Event_GetFlag(FLAG_MERCURY_FLAME_LIT))
    Iwram_CopyWordsEntry(&gMarsLighthouseRoomM_HallPalette[20],
                         gMarsLighthouseRoomM_UnlitFlameColors, 12);
  if (!Event_GetFlag(FLAG_MARS_FLAME_LIT))
    Iwram_CopyWordsEntry(&gMarsLighthouseRoomM_HallPalette[26],
                         gMarsLighthouseRoomM_UnlitFlameColors, 12);
  if (!Event_GetFlag(FLAG_JUPITER_FLAME_LIT))
    Iwram_CopyWordsEntry(&gMarsLighthouseRoomM_HallPalette[4],
                         gMarsLighthouseRoomM_UnlitFlameColors, 12);
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(8, 0),
                      (u32)gMarsLighthouseRoomM_HallPalette, 0x84000010);
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
    Event_PlaySound(SE_560);
  gMarsLighthouseRoomM_DoorwayStep = state;
  /* FAKEMATCH(temporary): the low bit held in a local. */
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
  Task_AddFrame(MarsLighthouseRoomM_FadeDoorway, 0x480);
}

/// Teleport on the broken floor: the party leaves for the aerie.
void MarsLighthouseRoomM_TeleportToAerie(void) {
  Teleport_StartShimmer();
  Teleport_PlaySparks();
  {
    u8 *party = (u8 *)&gMainRoutineWork;

    party[706] = 2;
  }
  FieldEvent_SetExitRequest(1);
  FieldEvent_Wait(40);
  Teleport_StopShimmer();
  FieldTask_Remove(MarsLighthouseRoomM_FadeDoorway);
}

const struct Trigger *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_MarsLighthouseRoomM6)
    return gMarsLighthouseRoomM_Room6Triggers;
  if (scene == (s32)&SceneId_MarsLighthouseRoomM1)
    return gMarsLighthouseRoomM_Room1Triggers;
  if (scene == (s32)&SceneId_MarsLighthouseRoomM2 ||
      scene == (s32)&SceneId_MarsLighthouseRoomM3 ||
      scene == (s32)&SceneId_MarsLighthouseRoomM4 ||
      scene == (s32)&SceneId_MarsLighthouseRoomM5)
    return gMarsLighthouseRoomM_Room5Triggers;
  return gMarsLighthouseRoomM_Triggers;
}

void Palette_SetColorsFade(u16 *slots, u16 *colours, u16 *a, u16 *b);
/* VIEW(s0b64): routine Actor_LaunchAhead as void Actor_LaunchAhead(void);
 * mars_lighthouse_room_m has void Actor_LaunchAhead(struct Actor *self); tried
 * as the definition's void Actor_LaunchAhead(struct Actor *self), which the
 * object already declares, it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_room_m.c:631: too few arguments to
 * function `Actor_LaunchAhead'. */
extern void Actor_LaunchAhead__s0b64(void) __asm__("Actor_LaunchAhead");

extern s16 gMarsLighthouseRoomM_FadeSlots[];
extern s16 gMarsLighthouseRoomM_FadeColorsFrom[];
extern s16 gMarsLighthouseRoomM_FadeColorsTo[];
extern u32 gMarsLighthouseRoomM_FadeOrder[];

s32 Scene_Initialize(void) {
  s32 entrance;
  s32 scene;

  scene = gMainRoutineWork.scene;
  entrance = gMainRoutineWork.entrance;

  gFieldWork->transition = 0x100;
  Event_SetFlag(FLAG_IN_MARS_LIGHTHOUSE);
  Palette_SetColorsFade((s32)gMarsLighthouseRoomM_FadeSlots,
                        (s32)gMarsLighthouseRoomM_FadeColorsFrom,
                        (s32)gMarsLighthouseRoomM_FadeColorsTo,
                        (s32)gMarsLighthouseRoomM_FadeOrder);
  if (scene != (s32)&SceneId_MarsLighthouseRoomM6 &&
      scene == (s32)&SceneId_MarsLighthouseRoomM1) {
    Call1((void (*)())FieldEffect_StartBlankOverlay, 0);
    gFieldWork->transition = 0x204;
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
        PaletteFade_ApplySource(0x10000, 0);
        PaletteFade_ApplyTarget(0x10003, 0);
        PaletteFade_Interpolate(20);
        System_Wait(20);
        entrance = 0x500021e;
        *(u16 *)entrance = 0x7fff;
        System_Wait(1);
        EventMessage_ShowCentered((s32)MsgYouHaveProven, 0, 4);
        PaletteFade_ApplyTarget(0x10000, 0);
        PaletteFade_Interpolate(20);
        System_Wait(20);
        FieldCamera_SetSpeed(1);
        FieldCamera_MoveTo(224, 16, 216, 1);
        FieldCamera_WaitForArrival();
        FieldEvent_Wait(40);
        actor = Actor_Get(ACTOR_STATUE_MOUTH);
        actor->motionFlags = unset;
        actor->y = px(16);
        actor->terrainHeight = px(16);
        ((u8 *)actor)[98] = ACTOR_FIREBALL;
        actor->unk66 = 0x4000;
        actor->watchedObject = (struct Actor *)unset;
        Actor_LaunchAhead__s0b64();
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
        Event_PlaySound(MUS_080);
        Sound_WaitForBgmRestore();
        Scene_PlayMusic();
        Event_End();
      }
    }
  }
  return 0;
}

/* VIEW(s0b64): routine MarsLighthouseRoomM_ShowFlame as s32
 * MarsLighthouseRoomM_ShowFlame(); the definition has void
 * MarsLighthouseRoomM_ShowFlame(s32 id, s32 value); tried as the definition's
 * void MarsLighthouseRoomM_ShowFlame(void), it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_room_m.c:667: conflicting types for
 * `MarsLighthouseRoomM_ShowFlame'. */
extern s32
MarsLighthouseRoomM_ShowFlame__s0b64() __asm__("MarsLighthouseRoomM_ShowFlame");
/* VIEW(mars_lighthouse_room_m): routine Map_ClearLayerEntryFlag as void
 * Map_ClearLayerEntryFlag(s32 value); module/scene/scene.h has u32
 * Map_ClearLayerEntryFlag(struct FieldSprite *sprite, u32 frame); tried as the
 * definition's void Map_ClearLayerEntryFlag(u32 no), it does not compile in
 * tla-ja: games/tla/src/scene/mars_lighthouse_room_m.c:673: conflicting types
 * for `Map_ClearLayerEntryFlag'. */
extern void
Map_ClearLayerEntryFlag__s0b64(s32 value) __asm__("Map_ClearLayerEntryFlag");
void Weather_StartRain(s32 a);

static __inline__ void SetBlend(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                s32 g) {
  LineScroll_InitWave(a, b, c, d, e, f, g);
}

s32 Scene_PrepareMap(void) {
  s16 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;
  struct Actor *object;

  if (scene == (s32)&SceneId_MarsLighthouseRoomM6 ||
      scene == (s32)&SceneId_MarsLighthouseRoomM2 ||
      scene == (s32)&SceneId_MarsLighthouseRoomM3 ||
      scene == (s32)&SceneId_MarsLighthouseRoomM4 ||
      scene == (s32)&SceneId_MarsLighthouseRoomM5) {
    Weather_StartRain(1);
    SetBlend(1, 0x10000, 0x40000, 0x20000, 0x10000, 0x8000, 0x4000);
  }
  if (scene == (s32)&SceneId_MarsLighthouseRoomM6) {
    switch (entrance) {
    case ENTRANCE_FROM_VENUS_TOWER:
      FieldActor_SetSpriteColor(Actor_Get(ACTOR_LEDGE_TORCH), SPRITE_COLOR_GRB);
      MarsLighthouseRoomM_ShowFlame__s0b64(ACTOR_LEDGE_FLAME,
                                           FLAME_ANIMATION_VENUS);
      Map_ClearLayerEntryFlag__s0b64(4);
      Map_ClearLayerEntryFlag__s0b64(5);
      break;
    case ENTRANCE_FROM_MERCURY_TOWER:
      FieldActor_SetSpriteColor(Actor_Get(ACTOR_LEDGE_TORCH), SPRITE_COLOR_BGR);
      MarsLighthouseRoomM_ShowFlame__s0b64(ACTOR_LEDGE_FLAME,
                                           FLAME_ANIMATION_MERCURY);
      Map_ClearLayerEntryFlag__s0b64(6);
      Map_ClearLayerEntryFlag__s0b64(7);
      break;
    case ENTRANCE_FROM_MARS_TOWER:
      FieldActor_SetSpriteColor(Actor_Get(ACTOR_LEDGE_TORCH),
                                SPRITE_COLOR_NORMAL);
      MarsLighthouseRoomM_ShowFlame__s0b64(ACTOR_LEDGE_FLAME,
                                           FLAME_ANIMATION_MARS);
      Map_ClearLayerEntryFlag__s0b64(0);
      Map_ClearLayerEntryFlag__s0b64(1);
      break;
    case ENTRANCE_FROM_JUPITER_TOWER:
      FieldActor_SetSpriteColor(Actor_Get(ACTOR_LEDGE_TORCH), SPRITE_COLOR_GBR);
      MarsLighthouseRoomM_ShowFlame__s0b64(ACTOR_LEDGE_FLAME,
                                           FLAME_ANIMATION_JUPITER);
      Map_ClearLayerEntryFlag__s0b64(2);
      Map_ClearLayerEntryFlag__s0b64(3);
      break;
    }
    object = Actor_Get(ACTOR_LEDGE_BLAZE_TARGET);
    object->motionFlags = 0;
    object->z = px(104);
    FieldTask_AddFrame(MarsLighthouseRoomM_CycleLedgeFlame, 0x480);
  } else if (scene == (s32)&SceneId_MarsLighthouseRoomM1) {
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
      MarsLighthouseRoomM_ShowFlame__s0b64(ACTOR_VENUS_FLAME,
                                           FLAME_ANIMATION_VENUS);
      MarsLighthouseRoomM_ShowFlame__s0b64(ACTOR_MERCURY_FLAME,
                                           FLAME_ANIMATION_MERCURY);
      MarsLighthouseRoomM_ShowFlame__s0b64(ACTOR_MARS_FLAME,
                                           FLAME_ANIMATION_MARS);
      MarsLighthouseRoomM_ShowFlame__s0b64(ACTOR_JUPITER_FLAME,
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
    extern char SceneId_MarsLighthouseRoomM3Second __asm__(
        "SceneId_MarsLighthouseRoomM3");

    TileMap_ConvertBuffer(
        gMainRoutineWork.map +
            ((s32)&SceneId_MarsLighthouseRoomM3Second - (s32)&SceneId_Prox2),
        0x5000000);
  }
  return 0;
}

/// "Mankind... With the power of wisdom, they ruled the earth."
void MarsLighthouseRoomM_ReadEarthInscription(void) {
  u16 *color = (u16 *)&OBJ_PLTT_COLOR(0, 15);

  Event_Start();
  Event_ResolvePendingActions(0);
  PaletteFade_ApplySource(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  *color = 0x7fff;
  wait(1);
  EventMessage_ShowCentered((s32)MsgMankindWithThe, 0, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  Event_End();
}

/// "Fish... With cold courage, they ruled the water."
void MarsLighthouseRoomM_ReadWaterInscription(void) {
  u16 *color = (u16 *)&OBJ_PLTT_COLOR(0, 15);

  Event_Start();
  Event_ResolvePendingActions(0);
  PaletteFade_ApplySource(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  *color = 0x7fff;
  wait(1);
  EventMessage_ShowCentered((s32)MsgFishWithCold, 0, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  Event_End();
}

/// "Dragons... Burning with might, they ruled the fires."
void MarsLighthouseRoomM_ReadFireInscription(void) {
  u16 *color = (u16 *)&OBJ_PLTT_COLOR(0, 15);

  Event_Start();
  Event_ResolvePendingActions(0);
  PaletteFade_ApplySource(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  *color = 0x7fff;
  wait(1);
  EventMessage_ShowCentered((s32)MsgDragonsBurningWith, 0, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  Event_End();
}

/// "Bird... On the wings of truth, they ruled the winds."
void MarsLighthouseRoomM_ReadWindInscription(void) {
  u16 *color = (u16 *)&OBJ_PLTT_COLOR(0, 15);

  Event_Start();
  Event_ResolvePendingActions(0);
  PaletteFade_ApplySource(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  *color = 0x7fff;
  wait(1);
  EventMessage_ShowCentered((s32)MsgBirdOnThe, 0, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  Event_End();
}

/// A piece of the broken floor: one that bounces up and back, or one that
/// flies off at random.
void MarsLighthouseRoomM_SpawnRubble(s32 x, s32 z, s32 kind) {
  struct Actor *object;

  /* FAKEMATCH(temporary): z shifted ahead of the call. */
  z <<= 16;
  object = (struct Actor *)Import_ActorCreate(403, x << 16, 0, z);

  if (object != 0) {
    Actor_SetSpriteFlags(object, 0);
    Actor_SelectAnimation(object, 3);
    if (kind != 0) {
      ((struct MarsLighthouseRoomMAnimation *)object->sprite)->layer = 2;
      Actor_StartScript(object, gMarsLighthouseRoomM_RubbleBounceScript);
    } else {
      ((struct MarsLighthouseRoomMAnimation *)object->sprite)->layer = 1;
      object->velocityX = (((u32)(Random16() * 8) >> 16) - 4) << 16;
      object->velocityY = px(4);
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
    px = x + ((u32)(Random16() * 12) >> 16) - 6;
    pz = z + ((u32)(Random16() * 12) >> 16) - 6;
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
      PaletteFade_ApplySource(0x10000, 0);
      BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
      PaletteFade_StartInterpolation(20);
      wait(20);
      *color = 0x7fff;
      wait(1);
      EventMessage_ShowCentered((s32)MsgIfYouDo, 0, 4);
      BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
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
    PaletteFade_ApplySource(0x10000, 0);
    BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
    PaletteFade_StartInterpolation(20);
    wait(20);
    *color = 0x7fff;
    wait(1);
    EventMessage_ShowCentered((s32)MsgYouWhoHold, 0, 4);
    wait(10);
    EventMessage_ShowCentered((s32)MsgYouWhoHold + 1, 0, 3);
    FieldCamera_MoveTo(224, 88, 216, 1);
    FieldCamera_WaitForArrival();
    FieldEvent_Wait(20);
    Event_PlaySound(SE_560);
    MarsLighthouseRoomM_FlashDoorways();
    FieldEvent_Wait(20);
    BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
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
  u8 *saved = Ram_HeapSlots->block32;

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
  PaletteFade_ApplyTarget(0x10000, 0);
  if ((s8)gMainRoutineWork.unk2c2 == 2)
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
    Task_AddFrame(PaletteCycle_Fade, 0x480);                                   \
  }

DEFINE_TELEPORT_START_SHIMMER(Teleport_StartShimmer, 15, 16)

extern u8 gInventorySnapshot[];
void Actor_SetSpriteColorMode(struct Actor *object, s32 value);
void Actor_Release(void *handle);
void Leader_KnockBack(void);
s32 Fireball_CarryLeader(void);
void Actor_LaunchAhead(struct Actor *self);

void Actor_SelectAnimation(struct Actor *object, s32 value);
void FieldCamera_MoveToActorById(s32 a, s32 b);
void FieldCamera_WaitForArrival(void);
void Teleport_StartShimmer(void);
void Teleport_PlaySparks(void);
void Teleport_StopShimmer(void);
void MarsLighthouseRoomM_FadeDoorway(void);
void PaletteCycle_Fade(void);

/// Stops the shimmer.
void Teleport_StopShimmer(void) {
  gFieldWork->transition = 256;
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
  PaletteFade_ApplyTarget(0x202108, 1);
  PaletteFade_ApplyTarget(0x10000, 2);
  gFieldWork->transition = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Teleport_StartFastShimmer();
  Teleport_PlayWhirl();
  System_Wait(40);
  Teleport_StopShimmer();
  PaletteFade_ApplyTarget(0x10000, 1);
  PaletteFade_Interpolate(16);
  System_Wait(16);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->unk2c2 = 0;
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
  struct Actor *object =
      FieldActor_Create(366, position->x, position->y, position->z);

  if (object != 0) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = value;
    ((void (*)())Actor_ResetScripted)(object, 2);
    object->update = Flame_UpdateForward;
    if (flag != 0)
      object->unk5c[0] = 2;
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
  struct Actor *object =
      FieldActor_Create(366, position->x, position->y, position->z);

  if (object != 0) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = -value;
    Actor_SetupScriptedPart(object, 3);
    object->update = Flame_UpdateForward;
    if (flag != 0)
      object->unk5c[0] = 2;
    Actor_SelectAnimation(object, 2);
  }
}

void Ride_SpawnLandingForward(struct MarsLighthouseRoomMVector *position,
                              s32 value, s32 flag) {
  struct Actor *object =
      FieldActor_Create(366, position->x, position->y, position->z);

  if (object != 0) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = value;
    Actor_SetupScriptedPart(object, 3);
    object->update = Flame_UpdateLanding;
    if (flag != 0)
      object->unk5c[0] = 2;
    Actor_SelectAnimation(object, 2);
  }
}

void Ride_SpawnLandingBackward(struct MarsLighthouseRoomMVector *position,
                               s32 value, s32 flag) {
  struct Actor *object =
      FieldActor_Create(366, position->x, position->y, position->z);

  if (object != 0) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = -value;
    Actor_SetupScriptedPart(object, 3);
    object->update = Flame_UpdateLanding;
    if (flag != 0)
      object->unk5c[0] = 2;
    Actor_SelectAnimation(object, 2);
  }
}

s32 Actor_IsAbove(struct Actor *a, struct Actor *b) {
  s32 height;

  if ((a->x - b->x >= 0 ? a->x - b->x : b->x - a->x) < 0xc0000) {
    height = a->y - b->y;
    if (height > 0 && height < 0x100000) {
      if ((a->z - b->z >= 0 ? a->z - b->z : b->z - a->z) < 0x80000)
        return 1;
    }
  }
  return 0;
}

void Ride_UpdateDrift(struct Actor *object) {
  struct Actor *target = Actor_Get(gMainRoutineWork.currentOwner);
  s32 vx;
  s32 vz;
  s32 rate;
  struct MarsLighthouseRoomMAnimation *animation;

  vx = object->bounce;
  object->x += vx;
  object->y += object->gravity;
  vz = object->unk4c;
  object->z += vz;
  object->bounce = vx - vx / 18;
  object->unk4c = vz - vz / 16;
  rate = object->speedLimit;
  object->scaleX += rate;
  animation = object->sprite;
  object->scaleY += object->acceleration;
  animation->angle += object->timer;
  if (target->y == target->terrainHeight && Actor_IsAbove(object, target)) {
    object->z -= object->unk4c;
    object->gravity += object->unk4c >> 2;
    object->unk4c = 0;
  }
}

#include "shared/actor_setup_scripted.h"

void Ride_SpawnSpark(struct MarsLighthouseRoomMVector *position, s32 value) {
  struct Actor *object = FieldActor_Create(285, position->x, position->y,
                                           position->z + 0xfffe0000);
  s32 angle;

  if (object != 0) {
    object->bounce = Trig_Cos(((u32)(Random16() * 3 << 12) >> 16) + 0x3000);
    object->gravity = (u16)Random16();
    object->unk4c = ((u32)(Random16() << 17) >> 16) + value;
    /* FAKEMATCH(temporary): the angle held in an s32 local keeps the zero
     * extension and the pool word 0xffff8000. */
    angle = (u16)Random16() - 0x8000;
    object->timer = angle;
    ((void (*)())Actor_SetupScripted)(object, 2);
    object->update = Ride_UpdateDrift;
    Actor_SetBlendMode(object, 1);
  }
}

/* VIEW(s1f94): tag struct Actor as struct Actor__s1f94; actor.h has another
 * definition; not tried: struct Actor__s1f94 was not found. */
typedef struct Actor__s1f94 {
  u8 pad0[8];
  s32 x;
  s32 y;
  s32 z;
  u8 pad14[91 - 20];
  u8 flag5b;
} Actor;

typedef struct Entry {
  s16 active;
  s16 timer;
  u16 flags;
  s16 unk_6;
  Actor *actor;
  s32 unk_c;
} Entry;

typedef struct State {
  u8 pad0[6];
  s16 unk_6;
  s16 unk_8;
  u8 pada[4];
  s16 unk_e;
  void (*callback)();
  Entry entries[4];
} State;

void Ride_SpawnForward();
void Ride_SpawnBackward();
void Ride_SpawnLandingForward();
void Ride_SpawnLandingBackward();
/* VIEW(s1f94): routine Ride_SpawnSpark as void Ride_SpawnSpark(); the parts
 * before it has another declaration; not tried: Ride_SpawnSpark's declaration
 * holds a FAKEMATCH tag. */
extern void Ride_SpawnSpark__s1f94() __asm__("Ride_SpawnSpark");
extern u8 gInventorySnapshot[];

typedef struct Position {
  s32 x;
  s32 z;
} Position;

typedef struct Camera {
  u8 pad0[228];
  Position position;
} Camera;

typedef struct IwramGlobals {
  u8 pad0[32];
  Camera *camera;
  u8 pad24[108 - 36];
  union {
    s32 word;
    void *pointer;
  } unk_6c;
} IwramGlobals;

#define IWRAM ((IwramGlobals *)Ram_HeapSlots)

void FlameThrowers_Puff(void) {
  s32 saved[2];
  State *state;
  Entry *entry;
  Camera *camera;
  Position *position;
  s32 cameraX, cameraZ;
  s32 v;
  s32 i;

  state = (State *)(gInventorySnapshot + 0x188);
  camera = IWRAM->camera;
  entry = state->entries;
  position = &camera->position;
  cameraX = position->x;
  cameraX &= 0xffff0000;
  cameraZ = position->z & 0xffff0000;
  saved[1] = (*(s32 **)camera)[1];
  saved[0] = IWRAM->unk_6c.word;
  if (state->unk_6 != 0 && state->unk_8 == 0 && state->callback != 0)
    state->callback();
  v = (u16)state->unk_6;
  state->unk_8 = v;
  state->unk_6 = 0;
  for (i = 3; i >= 0; i--, entry++) {
    Actor *actor;
    s32 flag, x, y, unk6, value, dx, dz;
    u8 *mark;

    if (entry->active == 0)
      continue;
    actor = entry->actor;
    if (actor == 0)
      continue;
    flag = GameFlag_Get(FLAG_MENU_OPEN);
    if (*(s16 *)(saved[0] + 358) != 0)
      flag |= 1;
    mark = &actor->flag5b;
    *mark = flag;
    if (state->unk_e != 0)
      *mark = GameFlag_Get(FLAG_MENU_OPEN);
    dx = actor->x;
    dz = actor->z;
    dx -= cameraX;
    /* FAKEMATCH(barrier): the empty asm on dx changes its allocation rank.
     */
    asm("" : : "r"(dx));
    y = actor->y;
    dz -= cameraZ;
    y = dz - y;
    unk6 = entry->unk_6;
    x = dx >> 16;
    y = (y >> 16) - 8;
    if (unk6 == 0) {
      if ((u32)(x + 7) > 334)
        continue;
      if (y <= -48)
        continue;
      if (y > 239)
        continue;
    }
    y = (u16)entry->timer;
    if (entry->timer <= 0) {
      y = entry->flags & 240;
      switch (y) {
      case 128:
        Ride_SpawnSpark__s1f94(&entry->actor->x, entry->unk_c, unk6);
        break;
      case 48:
        Ride_SpawnBackward(&entry->actor->x, entry->unk_c, unk6);
        break;
      case 32:
        Ride_SpawnLandingBackward(&entry->actor->x, entry->unk_c, unk6);
        break;
      case 16:
        Ride_SpawnLandingForward(&entry->actor->x, entry->unk_c, unk6);
        break;
      case 0:
        Ride_SpawnForward(&entry->actor->x, entry->unk_c, unk6);
        break;
      }
      value = 8;
    } else {
      value = y - 1;
    }
    entry->timer = value;
  }
}
#undef IWRAM

void Actor_Release(void *handle);
s32 MapGrid_CheckCollision(struct Actor *object, s32 *position);

void Ride_Create(void) {
  struct MarsLighthouseRoomMRideWork *work =
      (struct MarsLighthouseRoomMRideWork *)(gInventorySnapshot + 0x188);
  struct Actor *object;
  struct MarsLighthouseRoomMAnimation *animation;

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    Iwram_ClearWordsEntry(work, 116);
  object = (struct Actor *)FieldActor_Create(365, 1, 0, 0);
  object->unk5c[0] = 2;
  Actor_SelectAnimation(object, 1);
  animation = object->sprite;
  work->handle = object;
  work->value0c = animation->value10;
  animation->flag11 = 1;
}

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

#include "shared/ride_release_handle.h"

void Ride_SetMode(s16 value) {
  ((struct FlameHitWork *)(gInventorySnapshot + 0x188))->value0e = value;
}

#include "shared/beam_start.h"

s32 Ride_GetEntryValue(s32 index) {
  return ((struct RideRow *)(gInventorySnapshot + 0x188 + index * 16))->a;
}

void Ride_SetEntryValue(s32 index, s16 value) {
  ((struct RideRow *)(gInventorySnapshot + 0x188 + index * 16))->a = value;
}

s32 Ride_GetCounter(void) {
  return ((struct FlameHitWork *)(gInventorySnapshot + 0x188))->counter;
}

#include "shared/ride_advance_counter.h"

#include "shared/ride_set_entry.h"

void Ride_SetEntryCount(s32 index, s16 value) {
  ((struct FlameHitWork *)(gInventorySnapshot + 0x188))->entries[index].c =
      value;
}

s32 Ride_GetEntryFlags(s32 index) {
  return ((struct RideRow *)(gInventorySnapshot + 0x188 + index * 16))->b;
}

#include "shared/ride_set_slot.h"

/* VIEW(s2118): routine Unit_Get as struct BattleUnit *Unit_Get(s32 owner);
 * item_icon.h has void *Unit_Get(s32 owner); not tried: Unit_Get's declaration
 * holds a FAKEMATCH tag. */
struct BattleUnit *Unit_Get__s2118(s32 owner) __asm__("Unit_Get");
#define Unit_Get Unit_Get__s2118

#include "shared/party_take_fall_damage.h"

#include "../field/field_effect/advance_spin.c"

#include "shared/actor_blink.h"

s32 MapGrid_GetCellAttributeLowNibbleOrBump(s32 terrain, s32 x, s32 z);

#define KNOCK_BACK Leader_KnockBack
#define KNOCK_BACK_TAKE_DAMAGE Party_TakeFallDamage
#define KNOCK_BACK_BLINK Actor_Blink
#define KNOCK_BACK_SET_ANIMATION Actor_SelectAnimation
#define KNOCK_BACK_CELL_ATTRIBUTE MapGrid_GetCellAttributeLowNibbleOrBump
#define KNOCK_BACK_CHECK_COLLISION MapGrid_CheckCollision
#define KNOCK_BACK_SPHERES_OVERLAP Field_CheckSpheresOverlap
#include "shared/leader_knock_back.c"
#undef KNOCK_BACK
#undef KNOCK_BACK_TAKE_DAMAGE
#undef KNOCK_BACK_BLINK
#undef KNOCK_BACK_SET_ANIMATION
#undef KNOCK_BACK_CELL_ATTRIBUTE
#undef KNOCK_BACK_CHECK_COLLISION
#undef KNOCK_BACK_SPHERES_OVERLAP

/// A ride knocks the leader back while it runs.
void MarsLighthouseRoomM_ContinueSlide(void) {
  if (((struct FlameHitWork *)(gInventorySnapshot + 0x188))->active)
    Leader_KnockBack();
}

#undef Unit_Get
/* VIEW(s2708): routine Party_TakeFallDamage as int Party_TakeFallDamage(),
 * called with no declaration in scope as its bytes need; the definition has
 * void Party_TakeFallDamage(void); not tried: Party_TakeFallDamage's
 * declaration holds a FAKEMATCH tag. */
s32 Party_TakeFallDamage__s2708() __asm__("Party_TakeFallDamage");
#define Party_TakeFallDamage Party_TakeFallDamage__s2708
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
  Party_TakeFallDamage(-10, 0);
  FieldCamera_SetSpeed(7);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  ride = (struct FlameHitWork *)(gInventorySnapshot + 0x188);
  Event_SetFlag(FLAG_MENU_OPEN);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_ClearFlag(FLAG_MENU_OPEN);
  leader->update = (void (*)(void))Actor_Blink;
  leader->facing = 0x4000;
  Actor_SelectAnimation(leader, 49);
  while (ride->car->timer == 0) {
    s32 kind = MapGrid_GetCellAttributeLowNibbleOrBump(0, leader->x, leader->z);

    if (kind != 7) {
      leader->x += ((s32 *)ride->car->unk70)[0];
      leader->z += ((s32 *)ride->car->unk70)[2];
    } else {
      if (((s32 *)ride->car->unk70)[0] != 0)
        leader->x = (leader->x & 0xfff00000) + 0x80000;
      if (((s32 *)ride->car->unk70)[2] != 0)
        leader->z = (leader->z & 0xfff00000) + 0x80000;
    }
    ride->active = 0;
    System_Wait(1);
  }
  FieldEvent_Wait(30);
  leader->update = 0;
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

s32 MarsLighthouseRoomM_IsSliding(void) {
  return ((struct FlameHitWork *)(gInventorySnapshot + 0x188))->active;
}

struct Sprite {
  u8 unk00[4];
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
  u8 unk0a[2];
  s32 scale;
  u8 vramBlock;
  u8 unk11_0 : 1;
  u8 unk11_1 : 7;
  u16 rotation;
  u8 unk14[2];
  u8 unk16[5];
  u8 unk1b;
  u8 unk1c[12];
  void *record;
};

struct Work {
  u8 unk00[12];
  u16 vramBlock;
  u8 unk0e[98];
  struct Actor *owner;
};

#define WORK ((struct Work *)(gInventorySnapshot + 0x188))

void MarsLighthouseRoomM_ScatterCrumbs(struct Actor *actor) {
  struct Work *work = WORK;
  s32 i;

  if (work->owner == NULL)
    return;
  // A once-only loop: one object per call.
  for (i = 0; i >= 0; i--) {
    struct Actor *object;
    s32 a, b, c, d, e;
    s32 x, y, z;

    a = Random16();
    b = Random16();
    x = actor->x + a * 16;
    x -= b * 16;
    c = Random16();
    d = Random16();
    e = Random16();
    z = actor->z + d * 16 - e * 16;
    y = c * 8 + actor->y + Q16_ONE * 4;
    object = (struct Actor *)Import_ActorCreate(489, x, y, z);
    if (object != NULL) {
      s32 angle, radius;
      u32 sum;
      s32 scale;
      struct Sprite *sprite;
      struct Sprite *source;
      s32 velocity;

      angle = Random16();
      radius = Random16();
      sum = Random16();
      sum += Random16();
      scale = (sum >> 2) + Q16_ONE / 2;
      Actor_StartScript(object, gMarsLighthouseRoomM_CrumbScript);
      Actor_SetSpriteFlags(object, 0);
      object->velocityY = angle * 4 + Q16_ONE * 2;
      radius <<= 3;
      object->velocityZ = Iwram_MulQ16(radius, Trig_Cos(angle));
      velocity = Iwram_MulQ16(radius, Trig_Sin(angle));
      object->gravity = 0x547a;
      object->scaleX = scale;
      object->scaleY = scale;
      sprite = object->sprite;
      object->velocityX = velocity;
      object->acceleration = 0;
      object->bounce = Q16_ONE / 2;
      source = work->owner->sprite;
      VramCache_FreeEntry(sprite->vramBlock);
      sprite->unk11_0 = 1;
      sprite->vramBlock = work->vramBlock;
      SpriteAnimation_Clear(sprite->record);
      sprite->record = NULL;
      sprite->unk1b = 0;
      sprite->unk14[0] = source->unk14[0];
      sprite->unk14[1] = source->unk14[1];
      sprite->shape = source->shape;
      sprite->size = source->size;

      sprite->tile = source->tile;
    }
  }
}

#include "shared/embers_spawn.h"

void Actor_HoldDuringEvent(struct Actor *object) {
  object->movementState = Event_GetFlag(260);
}

#define ABS(x) ((x) < 0 ? -(x) : (x))

/* VIEW(s2708): tag struct Work020023c4 as struct Work020023c4__s2708;
 * module/scene/scene.h has another definition; tried with the definition the
 * rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_room_m.c:1669: redefinition of `struct
 * Work020023c4'. */
#define Work020023c4 Work020023c4__s2708
struct Work020023c4 {
  u8 unk00[4];
  u16 unk04;
  u8 unk06[4];
  u16 unk0a;
  u8 unk0c[0x6c - 0x0c];
  struct Actor *unk6c;
};

#include "shared/actor_roll_and_crumble.h"

void Actor_Attach(s32 sourceId, s32 id, s32 value, s32 angle,
                  const void *script) {
  struct Actor *source = Actor_Get(sourceId);
  struct Actor *object = Actor_Get(id);

  FieldActor_SetPositionAndResetMotion(object, source->x, source->y, source->z);
  Actor_StartScript(object, script);
  Actor_SetSpriteFlags(object, 0);
  object->motionFlags = 0;
  Actor_SelectAnimation(object, 1);
  object->watchedObject = source;
  object->timer = 0;
  object->unk66 = angle;
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
u32 gScriptedPart_Script[] = {                                                               
      SC_WAIT(30), SC_SET(SW_HOOK, 0), SC_POSITION(0, 0, 0), SC_WAIT(1),       
      SC_FREE,                                                                 
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
    SCENE_EXITS(MarsLighthouseRoomM6),
    SCENE_EXIT(1, MarsLighthouseRoomF4, 5),
    SCENE_EXIT(2, MarsLighthouseRoomF2, 8),
    SCENE_EXIT(3, MarsLighthouseRoomF1, 4),
    SCENE_EXIT(4, MarsLighthouseRoomF3, 15),
    SCENE_EXITS(MarsLighthouseRoomM1),
    SCENE_EXIT(1, MarsLighthouseAerie1, 1),
    SCENE_EXIT(2, MarsLighthouseEntrance2, 2),
    SCENE_EXIT(3, MarsLighthouseRoomM5, 1),
    SCENE_EXIT(4, MarsLighthouseRoomM2, 1),
    SCENE_EXIT(5, MarsLighthouseRoomM4, 1),
    SCENE_EXIT(6, MarsLighthouseRoomM3, 1),
    SCENE_EXIT(7, MarsLighthouseRoomM1, 8),
    SCENE_EXIT(8, MarsLighthouseRoomM1, 7),
    SCENE_EXIT(9, MarsLighthouseRoomM1, 10),
    SCENE_EXIT(10, MarsLighthouseRoomM1, 9),
    SCENE_EXIT(11, MarsLighthouseRoomM1, 12),
    SCENE_EXIT(12, MarsLighthouseRoomM1, 11),
    SCENE_EXIT(13, MarsLighthouseRoomM1, 14),
    SCENE_EXIT(14, MarsLighthouseRoomM1, 13),
    SCENE_EXITS(MarsLighthouseRoomM2),
    SCENE_EXIT(1, MarsLighthouseRoomM1, 4),
    SCENE_EXIT(2, MarsLighthouseRoomF2, 1),
    SCENE_EXITS(MarsLighthouseRoomM3),
    SCENE_EXIT(1, MarsLighthouseRoomM1, 6),
    SCENE_EXIT(2, MarsLighthouseRoomF3, 1),
    SCENE_EXITS(MarsLighthouseRoomM4),
    SCENE_EXIT(1, MarsLighthouseRoomM1, 5),
    SCENE_EXIT(2, MarsLighthouseRoomF1, 1),
    SCENE_EXITS(MarsLighthouseRoomM5),
    SCENE_EXIT(1, MarsLighthouseRoomM1, 3),
    SCENE_EXIT(2, MarsLighthouseRoomF4, 1),
    SCENE_EXITS_END,
};

struct ActorPlacement gMarsLighthouseRoomM_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gMarsLighthouseRoomM_Room6Placements[] = {
    ACTOR_PLACEMENT(418, CONDITION_ALWAYS, (s32)gMarsLighthouseRoomM_StatueScript, 504, 0, 184, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(365, CONDITION_ALWAYS, 7, 504, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, (s32)gMarsLighthouseRoomM_StatueScript, 504, 0, 112, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(413, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(413, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 232, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gMarsLighthouseRoomM_Room1Placements[] = {
    ACTOR_PLACEMENT(416, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 184, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(416, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 184, FACING_WEST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(416, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 216, FACING_EAST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(416, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 216, FACING_WEST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(365, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(365, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(365, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(365, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 224, 0, 144, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
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
    23, 9, 0x3000, 23, 10, 0x3000,
    23, 6, -0x1800, 0, 1, 13,
    8, 0, (s32)0x80010000, 23, 9, -0x3000,
    23, 10, -0x3000, 23, 6, 0x1800,
    0, 1, 13, 12, (s32)0xc0010000, 3,
    0, 1, 38,
};

u32 gMarsLighthouseRoomM_RubbleFallScript[] = {
    0, 20, 3, 0, 1, 38,
};
// clang-format on

#undef Work020023c4
#undef Party_TakeFallDamage
