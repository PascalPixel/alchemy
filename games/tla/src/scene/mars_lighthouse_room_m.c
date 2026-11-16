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

void MarsLighthouseRoomM_BreakFloorTile(s32 x, s32 z, s32 sourceX, s32 sourceY,
                                        s32 cellX, s32 cellY);

void MarsLighthouseRoomM_CycleLedgeFlame(void);

void MarsLighthouseRoomM_CycleHallFlames(void);

void MarsLighthouseRoomM_StartDoorways(s32 entrance);

void Ride_Create(void);

void MarsLighthouseRoomM_FlashDoorways(void);

void Actor_Blink(struct Actor *object);

extern u32 gCrumbs_Script[];
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
/// The colours of BG palettes 8 and 9.
struct HallColors {
  u16 colors[2 * PALETTE_COLORS];
};
u16 gPaletteCycleBright BSS_DATA;
u16 *gPaletteCycleSlots BSS_DATA;
u16 gPaletteCycleTimer BSS_DATA;
u16 gPaletteCycleDelay BSS_DATA ALIGNED(4);
u16 *gPaletteSlots BSS_DATA;
u16 *gPaletteColors BSS_DATA;
u16 *gPaletteCycleColors BSS_DATA;
u16 gPaletteCycleLength BSS_DATA;
u16 gPaletteCycleIndex BSS_DATA ALIGNED(4);
u16 gMarsLighthouseRoomM_LedgeFlameColors[2 * PALETTE_COLORS] BSS_DATA
    ALIGNED(4);
u16 gMarsLighthouseRoomM_LedgePalette[2 * PALETTE_COLORS] BSS_DATA;
u16 gMarsLighthouseRoomM_LedgeSavedPalette[PALETTE_COLORS] BSS_DATA;
struct HallColors gMarsLighthouseRoomM_HallFlameColors BSS_DATA;
struct HallColors gMarsLighthouseRoomM_HallPalette BSS_DATA;
/// BG palette 2's first ten colours; it is saved whole, with the unlit flame
/// colours after them.
u16 gMarsLighthouseRoomM_HallSavedPalette[10] BSS_DATA;
u16 gMarsLighthouseRoomM_UnlitFlameColors[6] BSS_DATA;
s16 gMarsLighthouseRoomM_DoorwayBlend BSS_DATA;
s16 gMarsLighthouseRoomM_DoorwayBlendTarget BSS_DATA;
s16 gMarsLighthouseRoomM_Doorway BSS_DATA;
s16 gMarsLighthouseRoomM_DoorwayStep BSS_DATA;
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
FIXED Trig_Cos(s32 angle);
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
                         (void *)&BG_PLTT_COLOR(13, 0),
                         sizeof(gMarsLighthouseRoomM_LedgeSavedPalette));
    Iwram_CopyWordsEntry(gMarsLighthouseRoomM_LedgeFlameColors,
                         (void *)&BG_PLTT_COLOR(8, 0),
                         sizeof(gMarsLighthouseRoomM_LedgeFlameColors));
    Iwram_CopyWordsEntry(gMarsLighthouseRoomM_LedgePalette,
                         gMarsLighthouseRoomM_LedgeFlameColors,
                         sizeof(gMarsLighthouseRoomM_LedgeFlameColors));
    gMarsLighthouseRoomM_LedgeCycleFrame = 0;
  }
  base = gMarsLighthouseRoomM_LedgeCycleFrame / 8 % 6;
  Iwram_CopyWordsEntry(gMarsLighthouseRoomM_LedgePalette,
                       gMarsLighthouseRoomM_LedgeSavedPalette,
                       sizeof(gMarsLighthouseRoomM_LedgeSavedPalette));
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
                      (u32)gMarsLighthouseRoomM_LedgePalette,
                      DMA_COPY_WORDS(8));
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
                         (void *)&BG_PLTT_COLOR(2, 0), PALETTE_COLORS * 2);
    Iwram_CopyWordsEntry(gMarsLighthouseRoomM_HallFlameColors.colors,
                         (void *)&BG_PLTT_COLOR(8, 0),
                         sizeof(gMarsLighthouseRoomM_HallFlameColors));
    Iwram_CopyWordsEntry(gMarsLighthouseRoomM_HallPalette.colors,
                         gMarsLighthouseRoomM_HallFlameColors.colors,
                         sizeof(gMarsLighthouseRoomM_HallFlameColors));
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
                         gMarsLighthouseRoomM_UnlitFlameColors,
                         sizeof(gMarsLighthouseRoomM_UnlitFlameColors));
  if (!Event_GetFlag(FLAG_MERCURY_FLAME_LIT))
    Iwram_CopyWordsEntry(&gMarsLighthouseRoomM_HallPalette.colors[20],
                         gMarsLighthouseRoomM_UnlitFlameColors,
                         sizeof(gMarsLighthouseRoomM_UnlitFlameColors));
  if (!Event_GetFlag(FLAG_MARS_FLAME_LIT))
    Iwram_CopyWordsEntry(&gMarsLighthouseRoomM_HallPalette.colors[26],
                         gMarsLighthouseRoomM_UnlitFlameColors,
                         sizeof(gMarsLighthouseRoomM_UnlitFlameColors));
  if (!Event_GetFlag(FLAG_JUPITER_FLAME_LIT))
    Iwram_CopyWordsEntry(&gMarsLighthouseRoomM_HallPalette.colors[4],
                         gMarsLighthouseRoomM_UnlitFlameColors,
                         sizeof(gMarsLighthouseRoomM_UnlitFlameColors));
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(8, 0),
                      (u32)gMarsLighthouseRoomM_HallPalette.colors,
                      DMA_COPY_WORDS(16));
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
  IO_WRITE_QUEUE_PUSH(
      (u32)&REG_BLDALPHA,
      BLEND_ALPHA(gMarsLighthouseRoomM_DoorwayBlend,
                  BLEND_WEIGHT_MAX - gMarsLighthouseRoomM_DoorwayBlend),
      IO_WRITE_HALFWORD);
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
  if (gMarsLighthouseRoomM_DoorwayBlend == BLEND_WEIGHT_MAX) {
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
    *flash = BLEND_WEIGHT_MAX;
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
    gMarsLighthouseRoomM_DoorwayBlendTarget = BLEND_WEIGHT_MAX;
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
  IO_WRITE_QUEUE_PUSH(
      (u32)&REG_BLDALPHA,
      BLEND_ALPHA(gMarsLighthouseRoomM_DoorwayBlend,
                  BLEND_WEIGHT_MAX - gMarsLighthouseRoomM_DoorwayBlend),
      IO_WRITE_HALFWORD);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT,
                      BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL,
                      IO_WRITE_HALFWORD);
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
        entrance = (s32)&OBJ_PLTT_COLOR(0, 15);
        *(u16 *)entrance = RGB(31, 31, 31);
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
        actor->launchedActorId = ACTOR_FIREBALL;
        actor->userValue = FACING_SOUTH;
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

/// The line scroll's wave across and down that the rain ledges start with the
/// rain. Called plainly, LineScroll_InitWave takes its arguments in another
/// order (16 of 237 instructions of Scene_PrepareMap differ in tla-en).
static __inline__ void MarsLighthouseRoomM_StartRainWave(s32 mode, FIXED freqX,
                                                         s32 stepX, FIXED ampX,
                                                         FIXED freqY, s32 stepY,
                                                         FIXED ampY) {
  LineScroll_InitWave(mode, freqX, stepX, ampX, freqY, stepY, ampY);
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
    MarsLighthouseRoomM_StartRainWave(1, FIXED_ONE, FIXED_ONE * 4,
                                      FIXED_ONE * 2, FIXED_ONE, FIXED_ONE / 2,
                                      FIXED_ONE / 4);
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
        (s32)BG_PLTT);
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
  *color = RGB(31, 31, 31);
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
  *color = RGB(31, 31, 31);
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
  *color = RGB(31, 31, 31);
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
  *color = RGB(31, 31, 31);
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

  object = FieldActor_Create(SPRITE_FLAME_PUFF, toFIXED(x), 0, toFIXED(z));
  if (object != 0) {
    Actor_SetSpriteFlags(object, 0);
    Actor_SelectAnimation(object, 3);
    if (kind != 0) {
      ((struct PlacedSprite *)object->sprite)->priority = 2;
      Actor_StartScript(object, gMarsLighthouseRoomM_RubbleBounceScript);
    } else {
      ((struct PlacedSprite *)object->sprite)->priority = 1;
      object->velocityX = ((Random_Below(8)) - 4) << 16;
      object->velocityY = toFIXED(4);
      Actor_StartScript(object, gMarsLighthouseRoomM_RubbleFallScript);
    }
  }
}

/// One floor tile before the statue breaks with a clatter and is gone: the
/// cell (cellX, cellY) takes the broken picture at (sourceX, sourceY).
void MarsLighthouseRoomM_BreakFloorTile(s32 x, s32 z, s32 sourceX, s32 sourceY,
                                        s32 cellX, s32 cellY) {
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
  MapGrid_CopyCellsTo(sourceX, sourceY, cellX + 64, cellY, 1, 1);
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
      *color = RGB(31, 31, 31);
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
    *color = RGB(31, 31, 31);
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

void Teleport_ArriveWhirling(void);

extern u8 gInventorySnapshot[];
void Actor_SetSpriteColorMode(struct Actor *object, s32 value);
void Actor_Release(void *handle);
s32 Leader_KnockBack(void);
s32 Fireball_CarryLeader(void);
void Actor_LaunchAhead(struct Actor *self);

void FieldCamera_MoveToActorById(s32 a, s32 b);
void FieldCamera_WaitForArrival(void);
void Teleport_PlaySparks(void);
void MarsLighthouseRoomM_FadeDoorway(void);

#include "shared/teleport_shimmer.c"

#include "shared/actor_is_touching.h"

#include "shared/actor_is_beside.h"

s32 Actor_IsTouching(struct Actor *a, struct Actor *b);
s32 Actor_IsBeside(struct Actor *a, struct Actor *b);

#include "shared/ride_update_carried.h"

void Actor_SetSpriteFlags(struct Actor *object, u32 flags);

#include "shared/actor_reset_scripted.h"

/// A flame thrower's flame sent forward from `position` at speed `value`;
/// `flag` keeps its sprite when off screen.
void Ride_SpawnForward(struct Vector3 *position, s32 value, s32 flag) {
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

/// A flame thrower's flame sent backward from `position` at speed `value`.
void Ride_SpawnBackward(struct Vector3 *position, s32 value, s32 flag) {
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

/// A flame thrower's flame that lands as it goes forward from `position`.
void Ride_SpawnLandingForward(struct Vector3 *position, s32 value, s32 flag) {
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

/// A flame thrower's flame that lands as it goes backward from `position`.
void Ride_SpawnLandingBackward(struct Vector3 *position, s32 value, s32 flag) {
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

/// Whether `a` is just above `b`: within 12 pixels across, 8 deep and up to
/// 16 above.
s32 Actor_IsAbove(struct Actor *a, struct Actor *b) {
  FIXED height;

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

/// Each frame a spark drifts, slowing, growing and turning; it rides up over
/// the leader standing in its way.
void Ride_UpdateDrift(struct Actor *object) {
  struct Actor *target = Actor_Get(gMainRoutineWork.leaderActorId);
  FIXED vx;
  FIXED vz;
  s32 rate;
  struct Sprite *animation;

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
  animation->rotation += object->timer;
  if (target->y.fixed == target->groundHeight &&
      Actor_IsAbove(object, target)) {
    object->z.fixed -= object->riseBaseY;
    object->gravity += object->riseBaseY >> 2;
    object->riseBaseY = 0;
  }
}

#include "shared/actor_setup_scripted.h"

/// A spark puffed from a flame thrower, drifting off in a random direction.
void Ride_SpawnSpark(struct Vector3 *position, s32 value, s32 mode) {
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

#include "shared/flame_throwers_puff.c"

void Actor_Release(void *handle);
s32 MapGrid_CheckCollision(struct Actor *object, s32 *position);

/// Clears the flame throwers' work (unless the party keeps its place) and
/// makes the flame whose sprite the crumbs share, keeping its VRAM entry.
void Ride_Create(void) {
  struct FlameHitWork *work = RIDE_WORK;
  struct Actor *object;
  struct Sprite *animation;

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    Iwram_ClearWordsEntry(work, sizeof(*work));
  object = (struct Actor *)FieldActor_Create(SPRITE_SMALL_FLAME, 1, 0, 0);
  object->retainSpriteCache = 2;
  Actor_SelectAnimation(object, 1);
  animation = object->sprite;
  work->handle = object;
  work->flameVramBlock = animation->slot;
  animation->retainCacheEntry = 1;
}

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

#include "shared/ride_release_handle.h"

/// Sets whether the flame throwers keep puffing while Psynergy is used.
void Ride_SetMode(s16 value) { RIDE_WORK->runsDuringPsynergy = value; }

#include "shared/beam_start.h"

/// Whether flame thrower `index` is active.
s32 Ride_GetEntryValue(s32 index) {
  return ((struct RideRow *)((u8 *)RIDE_WORK + index * 16))->entry.active;
}

/// Turns flame thrower `index` on or off.
void Ride_SetEntryValue(s32 index, s16 value) {
  ((struct RideRow *)((u8 *)RIDE_WORK + index * 16))->entry.active = value;
}

/// The flame throwers' counter, which Ride_AdvanceCounter steps.
s32 Ride_GetCounter(void) { return RIDE_WORK->counter; }

#include "shared/ride_advance_counter.h"

#include "shared/ride_set_entry.h"

/// Sets whether flame thrower `index` puffs even off screen.
void Ride_SetEntryCount(s32 index, s16 value) {
  RIDE_WORK->entries[index].offScreen = value;
}

/// Flame thrower `index`'s flags: which flame it sends and how far.
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

/// Hit by a rolling fireball: the leader is hurt, blinks, and is carried along
/// with it until it stops.
s32 Fireball_CarryLeader(void) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct FlameHitWork *ride;
  struct Actor *leader;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
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

/// Whether a flame or fireball has hit the leader.
s32 MarsLighthouseRoomM_IsSliding(void) { return RIDE_WORK->active; }

#include "shared/actor_scatter_crumbs.c"

#include "shared/embers_spawn.h"

/// An attached actor holds still while the field handles an event.
void Actor_HoldDuringEvent(struct Actor *object) {
  object->paused = Event_GetFlag(FLAG_FIELD_HANDLING_REQUESTS);
}

#define ABS(x) ((x) < 0 ? -(x) : (x))

#include "shared/actor_roll_and_crumble.c"

/// Puts actor `id` on actor `sourceId`, to send `value` rolling at `angle`
/// when its script says.
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
  object->launchedActorId = value;
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

u32 gCrumbs_Script[] = {
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
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH_LIT, CONDITION_ALWAYS, (s32)gMarsLighthouseRoomM_StatueScript, 504, 0, 184, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, 7, 504, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, (s32)gMarsLighthouseRoomM_StatueScript, 504, 0, 112, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 232, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gMarsLighthouseRoomM_Room1Placements[] = {
    ACTOR_PLACEMENT(SPRITE_ROBED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 184, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROBED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 184, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_ROBED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 216, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_ROBED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 216, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, 7, 224, 0, 144, FACING_SOUTH, TALK_FACING_KEEP, 0),
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
