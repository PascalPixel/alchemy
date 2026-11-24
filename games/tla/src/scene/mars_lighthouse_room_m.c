/// @file
/// The Mars Lighthouse's central hall, the four ledges in the rain that lead
/// from it to the element towers, and the beacon ledge at the top of each
/// tower, all sharing this overlay.
///
/// Felix reaches the hall up the stairs from the lighthouse's entrance. The
/// great dragon statue at its head speaks as he steps in: without the Mars
/// Star it tells him to begone; with it, once the lighthouse has thawed, it
/// bids him set the four spirits aflame. Four inscriptions hang beside the
/// statue, two on the floor by its chin and two on the gallery by its head,
/// naming the peoples who ruled earth, water, fire and wind. Each tower comes
/// out on a beacon ledge in the rain, where Blaze at the torch lights that
/// element's flame. Each flame lit burns on one of the hall's four robed
/// statues and makes its element's inscription glow. With all four lit the
/// statue says the heavens await, a fireball from its mouth breaks the floor
/// before it to show a round carving, and Teleport there carries the party up
/// to the aerie. Once the lighthouse has thawed, the gallery's arched doorways
/// light purple as Felix walks up to them.

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
#include "system/main_routine_state.h"
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
#include "window/window.h"
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

/// The hall's own entrances, through its four doorways: arriving from the
/// passage toward each element's tower, the party comes in under that
/// doorway's arch with its light shown.
enum {
  ENTRANCE_THROUGH_VENUS_DOORWAY = 8,
  ENTRANCE_THROUGH_MERCURY_DOORWAY = 10,
  ENTRANCE_THROUGH_MARS_DOORWAY = 12,
  ENTRANCE_THROUGH_JUPITER_DOORWAY = 14
};

/// The step cells before the hall's doorways, two to each in the doorways'
/// order: the cell under the arch, which fades its light in, then the cells
/// beside it, which fade it out.
enum {
  CELL_BEHAVIOR_VENUS_DOORWAY = 20,
  CELL_BEHAVIOR_MERCURY_DOORWAY = 22,
  CELL_BEHAVIOR_MARS_DOORWAY = 24,
  CELL_BEHAVIOR_JUPITER_DOORWAY = 26
};

/// The hall's doorways, in the order of their step cells
/// (gMarsLighthouseRoomM_Doorway).
enum { DOORWAY_VENUS, DOORWAY_MERCURY, DOORWAY_MARS, DOORWAY_JUPITER };

/// The hall's other cells: the carving Teleport is cast on, the four
/// inscriptions, and the steps up from the stairs where the dragon statue
/// speaks.
enum {
  CELL_AERIE_CARVING = 1,
  CELL_EARTH_INSCRIPTION = 30,
  CELL_WATER_INSCRIPTION,
  CELL_FIRE_INSCRIPTION,
  CELL_WIND_INSCRIPTION,
  CELL_BEFORE_DRAGON_STATUE
};

/// The beacon ledge's doorway back into its tower.
#define CELL_LEDGE_DOORWAY 1

/// The inscriptions' colours: BG palettes 8 and 9 hold six for each element,
/// the hall's four inscriptions glowing in them once that element's flame is
/// lit, and the ledge's inscription takes its tower's six into palette 13.
#define INSCRIPTION_FIRST_PALETTE 8
#define LEDGE_INSCRIPTION_PALETTE 13
/// The hall's palette whose last six colours are an unlit inscription's.
#define UNLIT_INSCRIPTION_PALETTE 2
/// An inscription's glow: a run of INSCRIPTION_GLOW_COLORS colours that turns
/// round, one colour on every INSCRIPTION_GLOW_STEP_FRAMES frames.
#define INSCRIPTION_GLOW_COLORS 6
#define INSCRIPTION_GLOW_STEP_FRAMES 8
/// The first of each element's six colours in palettes 8 and 9, and of the
/// ledge inscription's in palette 13.
enum {
  JUPITER_GLOW = 4,
  VENUS_GLOW = 10,
  MERCURY_GLOW = PALETTE_COLORS + 4,
  MARS_GLOW = PALETTE_COLORS + 10,
  LEDGE_GLOW = 10
};

/// Each element's flame among the flame sprite's animations.
enum {
  FLAME_ANIMATION_MARS = 2,
  FLAME_ANIMATION_JUPITER = 3,
  FLAME_ANIMATION_VENUS = 4,
  FLAME_ANIMATION_MERCURY = 5
};

/// The beacon ledge: the torch Blaze is cast at, tinted to its tower's element,
/// its flame, the point above it Blaze reaches, and two dragon statues.
enum {
  ACTOR_LEDGE_TORCH = 8,
  ACTOR_LEDGE_FLAME = 9,
  ACTOR_LEDGE_BLAZE_TARGET = 10
};

/// The hall: four robed statues (actors 8 to 11), the four elements' flames on
/// them, shown once lit, and the dragon statue's mouth, which lets fall the
/// fireball that breaks the floor.
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

void MarsLighthouseRoomM_CycleLedgeInscription(void);

void MarsLighthouseRoomM_CycleHallInscriptions(void);

void MarsLighthouseRoomM_StartDoorways(s32 entrance);

void Ride_Create(void);

void MarsLighthouseRoomM_FlashDoorways(void);

void Actor_Blink(struct Actor *actor);

extern u32 gCrumbs_Script[];
extern u32 gEmbers_Script[];

extern s32 gMarsLighthouseRoomM_TorchScript[];

void Actor_Release(void *handle);
void FieldCamera_MoveToActorById(s32 a, s32 b);
void FieldCamera_WaitForArrival(void);
void Teleport_PlaySparks(void);
/// The colours of BG palettes 8 and 9.
struct HallColors {
  u16 colors[2 * PALETTE_COLORS];
};
/// The teleport circle's shimmer, a palette cycle that PaletteCycle_Fade
/// steps.
u16 gPaletteCycleBright BSS_DATA;
u16 *gPaletteCycleSlots BSS_DATA;
u16 gPaletteCycleTimer BSS_DATA;
u16 gPaletteCycleDelay BSS_DATA ALIGNED(4);
u16 *gPaletteSlots BSS_DATA;
u16 *gPaletteColors BSS_DATA;
u16 *gPaletteCycleColors BSS_DATA;
u16 gPaletteCycleLength BSS_DATA;
u16 gPaletteCycleIndex BSS_DATA ALIGNED(4);
/// The beacon ledge inscription's colours: the element palettes as loaded, the
/// palette the glow is built in, and the inscription's own palette as loaded.
u16 gMarsLighthouseRoomM_LedgeElementColors[2 * PALETTE_COLORS] BSS_DATA
    ALIGNED(4);
u16 gMarsLighthouseRoomM_LedgePalette[2 * PALETTE_COLORS] BSS_DATA;
u16 gMarsLighthouseRoomM_LedgeSavedPalette[PALETTE_COLORS] BSS_DATA;
struct HallColors gMarsLighthouseRoomM_HallElementColors BSS_DATA;
struct HallColors gMarsLighthouseRoomM_HallPalette BSS_DATA;
/// BG palette 2's first ten colours; it is saved whole, with the unlit
/// inscription colours after them.
u16 gMarsLighthouseRoomM_HallSavedPalette[10] BSS_DATA;
u16 gMarsLighthouseRoomM_UnlitInscriptionColors[6] BSS_DATA;
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

void Actor_SetSpriteColorMode(struct Actor *actor, s32 value);

void Teleport_StopShimmer(void);

/// A script call of the beacon ledge's torch and of Blaze's mark above it:
/// they stand without a shadow.
s32 MarsLighthouseRoomM_SetUpTorch(struct Actor *actor) {
  Actor_SetSpriteFlags(actor, 0);
  return 0;
}

/// Shows a flame on its torch, 14 pixels up, in its element's animation.
s32 MarsLighthouseRoomM_ShowFlame(s32 id, s32 animation) {
  struct Actor *actor = Actor_Get(id);

  actor->motionFlags = 0;
  actor->y.fixed = toFIXED(14);
  Actor_SetSpriteFlags(actor, 0);
  Actor_SelectAnimation(actor, animation);
#ifdef UBFIX
  return 0;
#endif
}

void Actor_StartScript(struct Actor *actor, const void *script);

void Flame_UpdateForward(struct Actor *actor);
void Actor_ResetScripted(struct Actor *actor);
void Actor_SetupScriptedPart(struct Actor *actor, s32 layer);
void Flame_UpdateLanding(struct Actor *actor);

extern s32 gScriptedPart_Script[];
extern struct ActorPlacement gMarsLighthouseRoomM_Placements[];
extern struct ActorPlacement gMarsLighthouseRoomM_FlamePlacements[];
extern struct ActorPlacement gMarsLighthouseRoomM_InscriptionsPlacements[];

extern struct Trigger gMarsLighthouseRoomM_Triggers[];
extern struct Trigger gMarsLighthouseRoomM_FlameTriggers[];
extern struct Trigger gMarsLighthouseRoomM_InscriptionsTriggers[];
extern struct Trigger gMarsLighthouseRoomM_LedgesTriggers[];
extern s32 gMarsLighthouseRoomM_FlamePuffSwellScript[];
extern s32 gMarsLighthouseRoomM_FlamePuffThrownScript[];

s32 List_CountEntries(u16 *list);
void PaletteCycle_Fade(void);
void Teleport_StopShimmer(void);
void Teleport_StartFastShimmer(void);
s32 Actor_IsAbove(struct Actor *a, struct Actor *b);
void Ride_UpdateDrift(struct Actor *actor);
void Actor_SetupScripted(struct Actor *actor);
void MarsLighthouseRoomM_SpawnFlamePuff(s32 x, s32 z, s32 kind);
void Actor_HoldDuringEvent(struct Actor *actor);
void Event_End(void);
void Teleport_PlayWhirl(void);
FIXED Trig_Cos(s32 angle);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_MARS_LIGHTHOUSE_ROOM_M_FLAME)
    return gMarsLighthouseRoomM_FlamePlacements;
  if (scene == SCENE_MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS)
    return gMarsLighthouseRoomM_InscriptionsPlacements;
  return gMarsLighthouseRoomM_Placements;
}

extern s32 gMarsLighthouseRoomM_LedgeCycleFrame;

/// Each frame on the beacon ledge: once its flame is lit, the inscription on
/// the wall behind it glows in its element's colours, turning round.
void MarsLighthouseRoomM_CycleLedgeInscription(void) {
  s32 entrance = gMainRoutineState.entrance;
  struct FieldState *state = Ram_HeapSlots->fieldState;
  s32 base;
  s32 i;
  s8 busy;

  if (state->psynergy != 0)
    return;
  busy = state->revealActive;
  if (busy != 0)
    return;
  if (gMarsLighthouseRoomM_LedgeCycleFrame == -1) {
    Iwram_CopyWordsEntry(gMarsLighthouseRoomM_LedgeSavedPalette,
                         (void *)&BG_PLTT_COLOR(LEDGE_INSCRIPTION_PALETTE, 0),
                         sizeof(gMarsLighthouseRoomM_LedgeSavedPalette));
    Iwram_CopyWordsEntry(gMarsLighthouseRoomM_LedgeElementColors,
                         (void *)&BG_PLTT_COLOR(INSCRIPTION_FIRST_PALETTE, 0),
                         sizeof(gMarsLighthouseRoomM_LedgeElementColors));
    Iwram_CopyWordsEntry(gMarsLighthouseRoomM_LedgePalette,
                         gMarsLighthouseRoomM_LedgeElementColors,
                         sizeof(gMarsLighthouseRoomM_LedgeElementColors));
    gMarsLighthouseRoomM_LedgeCycleFrame = 0;
  }
  base = gMarsLighthouseRoomM_LedgeCycleFrame / INSCRIPTION_GLOW_STEP_FRAMES %
         INSCRIPTION_GLOW_COLORS;
  Iwram_CopyWordsEntry(gMarsLighthouseRoomM_LedgePalette,
                       gMarsLighthouseRoomM_LedgeSavedPalette,
                       sizeof(gMarsLighthouseRoomM_LedgeSavedPalette));
  if (entrance ==
          MARS_LIGHTHOUSE_ROOM_M_FLAME_FROM_MARS_LIGHTHOUSE_VENUS_TOWER &&
      Event_GetFlag(FLAG_VENUS_FLAME_LIT)) {
    for (i = 0; i < INSCRIPTION_GLOW_COLORS; i++)
      gMarsLighthouseRoomM_LedgePalette[LEDGE_GLOW + i] =
          gMarsLighthouseRoomM_LedgeElementColors[VENUS_GLOW +
                                                  (base + i) %
                                                      INSCRIPTION_GLOW_COLORS];
  }
  if (entrance ==
          MARS_LIGHTHOUSE_ROOM_M_FLAME_FROM_MARS_LIGHTHOUSE_MERCURY_TOWER &&
      Event_GetFlag(FLAG_MERCURY_FLAME_LIT)) {
    for (i = 0; i < INSCRIPTION_GLOW_COLORS; i++)
      gMarsLighthouseRoomM_LedgePalette[LEDGE_GLOW + i] =
          gMarsLighthouseRoomM_LedgeElementColors[MERCURY_GLOW +
                                                  (base + i) %
                                                      INSCRIPTION_GLOW_COLORS];
  }
  if (entrance ==
          MARS_LIGHTHOUSE_ROOM_M_FLAME_FROM_MARS_LIGHTHOUSE_MARS_TOWER &&
      Event_GetFlag(FLAG_MARS_FLAME_LIT)) {
    for (i = 0; i < INSCRIPTION_GLOW_COLORS; i++)
      gMarsLighthouseRoomM_LedgePalette[LEDGE_GLOW + i] =
          gMarsLighthouseRoomM_LedgeElementColors[MARS_GLOW +
                                                  (base + i) %
                                                      INSCRIPTION_GLOW_COLORS];
  }
  if (entrance ==
          MARS_LIGHTHOUSE_ROOM_M_FLAME_FROM_MARS_LIGHTHOUSE_JUPITER_TOWER &&
      Event_GetFlag(FLAG_JUPITER_FLAME_LIT)) {
    for (i = 0; i < INSCRIPTION_GLOW_COLORS; i++)
      gMarsLighthouseRoomM_LedgePalette[LEDGE_GLOW + i] =
          gMarsLighthouseRoomM_LedgeElementColors[JUPITER_GLOW +
                                                  (base + i) %
                                                      INSCRIPTION_GLOW_COLORS];
  }
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(LEDGE_INSCRIPTION_PALETTE, 0),
                      (u32)gMarsLighthouseRoomM_LedgePalette,
                      DMA_COPY_WORDS(8));
  gMarsLighthouseRoomM_LedgeCycleFrame++;
}

/// Walking back into the doorway at the top of the beacon ledge's steps
/// (cell (31, 17)) takes Felix back into the tower he came up through.
void MarsLighthouseRoomM_LeaveLedge(void) {
  Event_PlaySound(SE_MAP_EXIT);
  FieldEvent_SetExitRequest(gMainRoutineState.entrance);
}

/// Nothing visible happens yet when Felix casts Blaze at the torch on the
/// beacon ledge; the game only marks this cast's jet of flame as the long one,
/// so that it carries past the torch up to the beacon flame.
void MarsLighthouseRoomM_StartBlazeOnTorch(void) {
  struct FieldState *event = Ram_HeapSlots->fieldState;

  event->blazeLongSpray = 1;
}

/// As Felix's Blaze on the beacon ledge's torch flies on past it, the camera
/// follows the fire up to the unlit beacon.
void MarsLighthouseRoomM_WatchBlazeReachFlame(void) {
  FieldCamera_MoveToActorById(ACTOR_LEDGE_BLAZE_TARGET, 1);
  FieldCamera_WaitForArrival();
}

/// Blaze reaches the beacon above the ledge's torch: the screen flashes white
/// with a chime, and the beacon of the tower Felix came up through is lit.
void MarsLighthouseRoomM_LightFlame(void) {
  struct FieldState *state;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  PaletteFade_ApplyTarget(RGB_WHITE, PALETTES_ALL);
  PaletteFade_Interpolate(30);
  FieldEvent_Wait(30);
  Event_PlaySound(SE_WARP);
  switch (gMainRoutineState.entrance) {
  case MARS_LIGHTHOUSE_ROOM_M_FLAME_FROM_MARS_LIGHTHOUSE_VENUS_TOWER:
    Event_SetFlag(FLAG_VENUS_FLAME_LIT);
    break;
  case MARS_LIGHTHOUSE_ROOM_M_FLAME_FROM_MARS_LIGHTHOUSE_MERCURY_TOWER:
    Event_SetFlag(FLAG_MERCURY_FLAME_LIT);
    break;
  case MARS_LIGHTHOUSE_ROOM_M_FLAME_FROM_MARS_LIGHTHOUSE_MARS_TOWER:
    Event_SetFlag(FLAG_MARS_FLAME_LIT);
    break;
  case MARS_LIGHTHOUSE_ROOM_M_FLAME_FROM_MARS_LIGHTHOUSE_JUPITER_TOWER:
    Event_SetFlag(FLAG_JUPITER_FLAME_LIT);
    break;
  }
  state = Ram_HeapSlots->fieldState;
  state->psynergy = 0;
  FieldEvent_Wait(30);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_Interpolate(30);
  FieldEvent_Wait(30);
  FieldEvent_Wait(10);
  Event_End();
}

extern s32 gMarsLighthouseRoomM_HallCycleFrame;

/// Each frame in the hall: the inscription of each element whose flame is lit
/// glows in its colours, turning round, and the others show unlit.
void MarsLighthouseRoomM_CycleHallInscriptions(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  s32 base;
  s32 i;
  s8 busy;

  if (state->psynergy != 0)
    return;
  busy = state->revealActive;
  if (busy != 0)
    return;
  if (gMarsLighthouseRoomM_HallCycleFrame == -1) {
    Iwram_CopyWordsEntry(gMarsLighthouseRoomM_HallSavedPalette,
                         (void *)&BG_PLTT_COLOR(UNLIT_INSCRIPTION_PALETTE, 0),
                         PALETTE_COLORS * 2);
    Iwram_CopyWordsEntry(gMarsLighthouseRoomM_HallElementColors.colors,
                         (void *)&BG_PLTT_COLOR(INSCRIPTION_FIRST_PALETTE, 0),
                         sizeof(gMarsLighthouseRoomM_HallElementColors));
    Iwram_CopyWordsEntry(gMarsLighthouseRoomM_HallPalette.colors,
                         gMarsLighthouseRoomM_HallElementColors.colors,
                         sizeof(gMarsLighthouseRoomM_HallElementColors));
    gMarsLighthouseRoomM_HallCycleFrame = 0;
  }
  base = gMarsLighthouseRoomM_HallCycleFrame / INSCRIPTION_GLOW_STEP_FRAMES %
         INSCRIPTION_GLOW_COLORS;
  for (i = 0; i < INSCRIPTION_GLOW_COLORS; i++) {
    gMarsLighthouseRoomM_HallPalette.colors[JUPITER_GLOW + i] =
        gMarsLighthouseRoomM_HallElementColors
            .colors[JUPITER_GLOW + (base + i) % INSCRIPTION_GLOW_COLORS];
    gMarsLighthouseRoomM_HallPalette.colors[VENUS_GLOW + i] =
        gMarsLighthouseRoomM_HallElementColors
            .colors[VENUS_GLOW + (base + i) % INSCRIPTION_GLOW_COLORS];
    gMarsLighthouseRoomM_HallPalette.colors[MERCURY_GLOW + i] =
        gMarsLighthouseRoomM_HallElementColors
            .colors[MERCURY_GLOW + (base + i) % INSCRIPTION_GLOW_COLORS];
    gMarsLighthouseRoomM_HallPalette.colors[MARS_GLOW + i] =
        gMarsLighthouseRoomM_HallElementColors
            .colors[MARS_GLOW + (base + i) % INSCRIPTION_GLOW_COLORS];
  }
  if (!Event_GetFlag(FLAG_VENUS_FLAME_LIT))
    Iwram_CopyWordsEntry(&gMarsLighthouseRoomM_HallPalette.colors[VENUS_GLOW],
                         gMarsLighthouseRoomM_UnlitInscriptionColors,
                         sizeof(gMarsLighthouseRoomM_UnlitInscriptionColors));
  if (!Event_GetFlag(FLAG_MERCURY_FLAME_LIT))
    Iwram_CopyWordsEntry(&gMarsLighthouseRoomM_HallPalette.colors[MERCURY_GLOW],
                         gMarsLighthouseRoomM_UnlitInscriptionColors,
                         sizeof(gMarsLighthouseRoomM_UnlitInscriptionColors));
  if (!Event_GetFlag(FLAG_MARS_FLAME_LIT))
    Iwram_CopyWordsEntry(&gMarsLighthouseRoomM_HallPalette.colors[MARS_GLOW],
                         gMarsLighthouseRoomM_UnlitInscriptionColors,
                         sizeof(gMarsLighthouseRoomM_UnlitInscriptionColors));
  if (!Event_GetFlag(FLAG_JUPITER_FLAME_LIT))
    Iwram_CopyWordsEntry(&gMarsLighthouseRoomM_HallPalette.colors[JUPITER_GLOW],
                         gMarsLighthouseRoomM_UnlitInscriptionColors,
                         sizeof(gMarsLighthouseRoomM_UnlitInscriptionColors));
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(INSCRIPTION_FIRST_PALETTE, 0),
                      (u32)gMarsLighthouseRoomM_HallPalette.colors,
                      DMA_COPY_WORDS(16));
  gMarsLighthouseRoomM_HallCycleFrame++;
}

/// Each frame, the purple light in the arched doorway Felix last stepped up to
/// fades one step in or out. Fully lit, the doorway opens a way through the
/// wall (the cells under its arch are copied from the open template); faded
/// out, the wall closes again.
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
    if (gMarsLighthouseRoomM_Doorway == DOORWAY_VENUS)
      MapGrid_CopyCellAttributes(34, 8, 3, 3, 17, 8);
    if (gMarsLighthouseRoomM_Doorway == DOORWAY_MERCURY)
      MapGrid_CopyCellAttributes(30, 8, 3, 3, 8, 8);
    if (gMarsLighthouseRoomM_Doorway == DOORWAY_MARS)
      MapGrid_CopyCellAttributes(34, 8, 3, 3, 17, 8);
    if (gMarsLighthouseRoomM_Doorway == DOORWAY_JUPITER)
      MapGrid_CopyCellAttributes(30, 8, 3, 3, 8, 8);
  }
  if (gMarsLighthouseRoomM_DoorwayBlend == BLEND_WEIGHT_MAX) {
    if (gMarsLighthouseRoomM_Doorway == DOORWAY_VENUS)
      MapGrid_CopyCellAttributes(34, 0, 3, 3, 17, 8);
    if (gMarsLighthouseRoomM_Doorway == DOORWAY_MERCURY)
      MapGrid_CopyCellAttributes(30, 4, 3, 3, 8, 8);
    if (gMarsLighthouseRoomM_Doorway == DOORWAY_MARS)
      MapGrid_CopyCellAttributes(34, 4, 3, 3, 17, 8);
    if (gMarsLighthouseRoomM_Doorway == DOORWAY_JUPITER)
      MapGrid_CopyCellAttributes(30, 0, 3, 3, 8, 8);
  }
}

/// Once the lighthouse has thawed, stepping under one of the hall's four
/// arched doorways, two each side of the dragon statue, lights its opening
/// purple with a hum, and stepping to the cells beside it fades it out again.
/// The arch under Venus's doorway is cell (18, 10), Mercury's (9, 11), Mars's
/// (18, 11) and Jupiter's (9, 10); the last doorway lit is put away first.
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
  next = state - CELL_BEHAVIOR_VENUS_DOORWAY;
  if (odd)
    *flash = 0;
  else
    *flash = BLEND_WEIGHT_MAX;
  if (gMarsLighthouseRoomM_Doorway == DOORWAY_VENUS)
    MapGrid_CopyCellsLayered(0, 122, 3, 3, 17, 67);
  if (gMarsLighthouseRoomM_Doorway == DOORWAY_MERCURY)
    MapGrid_CopyCellsLayered(0, 122, 3, 3, 8, 72);
  if (gMarsLighthouseRoomM_Doorway == DOORWAY_MARS)
    MapGrid_CopyCellsLayered(0, 122, 3, 3, 17, 72);
  if (gMarsLighthouseRoomM_Doorway == DOORWAY_JUPITER)
    MapGrid_CopyCellsLayered(0, 122, 3, 3, 8, 67);
  gMarsLighthouseRoomM_Doorway = next / 2;
  if (gMarsLighthouseRoomM_Doorway == DOORWAY_VENUS)
    MapGrid_CopyCellsLayered(0, 125, 3, 3, 17, 67);
  if (gMarsLighthouseRoomM_Doorway == DOORWAY_MERCURY)
    MapGrid_CopyCellsLayered(0, 125, 3, 3, 8, 72);
  if (gMarsLighthouseRoomM_Doorway == DOORWAY_MARS)
    MapGrid_CopyCellsLayered(0, 125, 3, 3, 17, 72);
  if (gMarsLighthouseRoomM_Doorway == DOORWAY_JUPITER)
    MapGrid_CopyCellsLayered(0, 125, 3, 3, 8, 67);
}

/// As the dragon statue speaks, the four arched doorways light up purple
/// together, flicker and fade away.
void MarsLighthouseRoomM_FlashDoorways(void) {
  MapGrid_CopyCellsLayered(0, 125, 3, 3, 17, 67);
  MapGrid_CopyCellsLayered(0, 125, 3, 3, 8, 72);
  MapGrid_CopyCellsLayered(0, 125, 3, 3, 17, 72);
  MapGrid_CopyCellsLayered(0, 125, 3, 3, 8, 67);
  System_Wait(1);
  gMarsLighthouseRoomM_DoorwayBlendTarget = BLEND_WEIGHT_MAX * 3 / 4;
  System_Wait(12);
  gMarsLighthouseRoomM_DoorwayBlendTarget = BLEND_WEIGHT_MAX / 2;
  System_Wait(4);
  gMarsLighthouseRoomM_DoorwayBlendTarget = BLEND_WEIGHT_MAX * 3 / 4;
  System_Wait(4);
  gMarsLighthouseRoomM_DoorwayBlendTarget = BLEND_WEIGHT_MAX / 2;
  System_Wait(4);
  gMarsLighthouseRoomM_DoorwayBlendTarget = BLEND_WEIGHT_MAX * 3 / 4;
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

/// On arriving in the hall through one of its arched doorways, that doorway
/// is shown lit purple behind Felix (one step short of full, so the next fade
/// opens its way); arriving any other way, all four are dark.
void MarsLighthouseRoomM_StartDoorways(s32 entrance) {
  s32 cell = -1;
  s16 *blend;
  BlendScript_Suspend();
  blend = &gMarsLighthouseRoomM_DoorwayBlend;
  if (entrance == ENTRANCE_THROUGH_VENUS_DOORWAY)
    cell = CELL_BEHAVIOR_VENUS_DOORWAY;
  if (entrance == ENTRANCE_THROUGH_MERCURY_DOORWAY)
    cell = CELL_BEHAVIOR_MERCURY_DOORWAY;
  if (entrance == ENTRANCE_THROUGH_MARS_DOORWAY)
    cell = CELL_BEHAVIOR_MARS_DOORWAY;
  if (entrance == ENTRANCE_THROUGH_JUPITER_DOORWAY)
    cell = CELL_BEHAVIOR_JUPITER_DOORWAY;
  if (cell == -1) {
    *blend = 0;
    gMarsLighthouseRoomM_DoorwayBlendTarget = 0;
    gMarsLighthouseRoomM_Doorway = cell;
    gMarsLighthouseRoomM_DoorwayStep = cell;
  } else {
    s32 step;
    *blend = BLEND_WEIGHT_MAX - 1;
    gMarsLighthouseRoomM_DoorwayBlendTarget = BLEND_WEIGHT_MAX;
    step = (cell - CELL_BEHAVIOR_VENUS_DOORWAY) / 2;
    gMarsLighthouseRoomM_Doorway = step;
    gMarsLighthouseRoomM_DoorwayStep = cell;
    if (step == DOORWAY_VENUS)
      MapGrid_CopyCellsLayered(0, 125, 3, 3, 17, 67);
    if (gMarsLighthouseRoomM_Doorway == DOORWAY_MERCURY)
      MapGrid_CopyCellsLayered(0, 125, 3, 3, 8, 72);
    if (gMarsLighthouseRoomM_Doorway == DOORWAY_MARS)
      MapGrid_CopyCellsLayered(0, 125, 3, 3, 17, 72);
    if (gMarsLighthouseRoomM_Doorway == DOORWAY_JUPITER)
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
  Task_AddFrame(MarsLighthouseRoomM_FadeDoorway, TASK_PASS_FRAME);
}

/// Felix casts Teleport on the round carving the dragon statue's fireball
/// uncovered in the floor before it (cells (12, 12) to (15, 14)): the screen
/// turns red, sparks swirl round the party, and they arrive on the carved
/// octagon of the Mars Lighthouse Aerie in the storm.
void MarsLighthouseRoomM_TeleportToAerie(void) {
  Teleport_StartShimmer();
  Teleport_PlaySparks();
  gMainRoutineState.teleportState = 2;
  FieldEvent_SetExitRequest(
      MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS_TO_MARS_LIGHTHOUSE_AERIE);
  FieldEvent_Wait(40);
  Teleport_StopShimmer();
  FieldTask_Remove(MarsLighthouseRoomM_FadeDoorway);
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_MARS_LIGHTHOUSE_ROOM_M_FLAME)
    return gMarsLighthouseRoomM_FlameTriggers;
  if (scene == SCENE_MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS)
    return gMarsLighthouseRoomM_InscriptionsTriggers;
  if (scene == SCENE_MARS_LIGHTHOUSE_MERCURY_LEDGE ||
      scene == SCENE_MARS_LIGHTHOUSE_JUPITER_LEDGE ||
      scene == SCENE_MARS_LIGHTHOUSE_ROOM_M_ALASTORS_HOOD_CHEST ||
      scene == SCENE_MARS_LIGHTHOUSE_VENUS_LEDGE)
    return gMarsLighthouseRoomM_LedgesTriggers;
  return gMarsLighthouseRoomM_Triggers;
}

void Palette_SetColorsFade(u16 *slots, u16 *colours, u16 *a, u16 *b);

extern u16 gTeleportCircle_PaletteSlots[];
extern u16 gTeleportCircle_Colors[];
extern u16 gTeleportCircle_ShimmerColors[];
extern u16 gTeleportCircle_ShimmerSlots[];

s32 Scene_Initialize(void) {
  s32 entrance;
  s32 scene;

  scene = gMainRoutineState.next;
  entrance = gMainRoutineState.entrance;

  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_SetFlag(FLAG_IN_MARS_LIGHTHOUSE);
  Palette_SetColorsFade(
      (s32)gTeleportCircle_PaletteSlots, (s32)gTeleportCircle_Colors,
      (s32)gTeleportCircle_ShimmerColors, (s32)gTeleportCircle_ShimmerSlots);
  if (scene != SCENE_MARS_LIGHTHOUSE_ROOM_M_FLAME &&
      scene == SCENE_MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS) {
    Call1((void (*)())FieldEffect_StartBlankOverlay, 0);
    gFieldState->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
    if (entrance ==
            MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS_FROM_MARS_LIGHTHOUSE_AERIE ||
        entrance ==
            MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS_FROM_MARS_LIGHTHOUSE_ENTRANCE_FUGUE ||
        entrance == ENTRANCE_THROUGH_VENUS_DOORWAY ||
        entrance == ENTRANCE_THROUGH_MERCURY_DOORWAY ||
        entrance == ENTRANCE_THROUGH_MARS_DOORWAY ||
        entrance == ENTRANCE_THROUGH_JUPITER_DOORWAY) {
      s32 unset = Event_GetFlag(FLAG_WAY_TO_AERIE_OPEN);

      if (unset == 0 && Event_GetFlag(FLAG_VENUS_FLAME_LIT) &&
          Event_GetFlag(FLAG_MERCURY_FLAME_LIT) &&
          Event_GetFlag(FLAG_MARS_FLAME_LIT) &&
          Event_GetFlag(FLAG_JUPITER_FLAME_LIT)) {
        struct Actor *actor;

        Event_Start();
        FieldEvent_EndLastingPsynergy(0);
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
        // entrance, read for the last time above, now holds the colour's
        // address.
        entrance = (s32)&CENTERED_TEXT_COLOR;
        *(u16 *)entrance = RGB_WHITE;
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
        // unset is zero here: the mouth stands still and watches nothing.
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
/// rain.
static __inline__ void MarsLighthouseRoomM_StartRainWave(s32 mode, FIXED freqX,
                                                         s32 stepX, FIXED ampX,
                                                         FIXED freqY, s32 stepY,
                                                         FIXED ampY) {
  LineScroll_InitWave(mode, freqX, stepX, ampX, freqY, stepY, ampY);
}

s32 Scene_PrepareMap(void) {
  s16 scene = gMainRoutineState.next;
  s32 entrance = gMainRoutineState.entrance;
  struct Actor *actor;

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
    case MARS_LIGHTHOUSE_ROOM_M_FLAME_FROM_MARS_LIGHTHOUSE_VENUS_TOWER:
      FieldActor_SetSpriteColor(Actor_Get(ACTOR_LEDGE_TORCH), SPRITE_COLOR_GRB);
      MarsLighthouseRoomM_ShowFlame(ACTOR_LEDGE_FLAME, FLAME_ANIMATION_VENUS);
      TilesetAnim_Resume(4);
      TilesetAnim_Resume(5);
      break;
    case MARS_LIGHTHOUSE_ROOM_M_FLAME_FROM_MARS_LIGHTHOUSE_MERCURY_TOWER:
      FieldActor_SetSpriteColor(Actor_Get(ACTOR_LEDGE_TORCH), SPRITE_COLOR_BGR);
      MarsLighthouseRoomM_ShowFlame(ACTOR_LEDGE_FLAME, FLAME_ANIMATION_MERCURY);
      TilesetAnim_Resume(6);
      TilesetAnim_Resume(7);
      break;
    case MARS_LIGHTHOUSE_ROOM_M_FLAME_FROM_MARS_LIGHTHOUSE_MARS_TOWER:
      FieldActor_SetSpriteColor(Actor_Get(ACTOR_LEDGE_TORCH),
                                SPRITE_COLOR_NORMAL);
      MarsLighthouseRoomM_ShowFlame(ACTOR_LEDGE_FLAME, FLAME_ANIMATION_MARS);
      TilesetAnim_Resume(0);
      TilesetAnim_Resume(1);
      break;
    case MARS_LIGHTHOUSE_ROOM_M_FLAME_FROM_MARS_LIGHTHOUSE_JUPITER_TOWER:
      FieldActor_SetSpriteColor(Actor_Get(ACTOR_LEDGE_TORCH), SPRITE_COLOR_GBR);
      MarsLighthouseRoomM_ShowFlame(ACTOR_LEDGE_FLAME, FLAME_ANIMATION_JUPITER);
      TilesetAnim_Resume(2);
      TilesetAnim_Resume(3);
      break;
    }
    actor = Actor_Get(ACTOR_LEDGE_BLAZE_TARGET);
    actor->motionFlags = 0;
    actor->z.fixed = toFIXED(104);
    FieldTask_AddFrame(MarsLighthouseRoomM_CycleLedgeInscription,
                       TASK_PASS_FRAME);
  } else if (scene == SCENE_MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS) {
    if (entrance ==
            MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS_FROM_MARS_LIGHTHOUSE_AERIE ||
        entrance ==
            MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS_FROM_MARS_LIGHTHOUSE_ENTRANCE_FUGUE ||
        entrance == ENTRANCE_THROUGH_VENUS_DOORWAY ||
        entrance == ENTRANCE_THROUGH_MERCURY_DOORWAY ||
        entrance == ENTRANCE_THROUGH_MARS_DOORWAY ||
        entrance == ENTRANCE_THROUGH_JUPITER_DOORWAY) {
      Ride_Create();
      if (Event_GetFlag(FLAG_WAY_TO_AERIE_OPEN)) {
        MapGrid_CopyCellAttributes(30, 12, 4, 3, 12, 12);
        MapGrid_CopyCellsLayered(64, 64, 4, 3, 76, 12);
      }
      MarsLighthouseRoomM_StartDoorways(entrance);
      {
        s32 priority = TASK_PASS_FRAME;

        FieldTask_AddFrame(MarsLighthouseRoomM_CycleHallInscriptions, priority);
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
        gMainRoutineState.mapId +
            ((s32)&SceneId_MarsLighthouseJupiterLedgeSecond - SCENE_PROX),
        (s32)BG_PLTT);
  }
  return 0;
}

/// Felix reads the inscription on the hall's gallery east of the dragon
/// statue's head: the screen dims to green and "Mankind... With the power of
/// wisdom, they ruled the earth." appears across it.
void MarsLighthouseRoomM_ReadEarthInscription(void) {
  u16 *color = (u16 *)&CENTERED_TEXT_COLOR;

  Event_Start();
  Event_EndLastingPsynergy(0);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_SetTarget(PALETTE_TINT_WARM, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  *color = RGB_WHITE;
  wait(1);
  EventMessage_ShowCentered((s32)MsgMarsLighthouseRoomMMankindWithPowerOfWisdom,
                            0, 0);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  Event_End();
}

/// Felix reads the inscription on the hall's floor west of the dragon statue's
/// chin: the screen dims to green and "Fish... With cold courage, they ruled
/// the water." appears across it.
void MarsLighthouseRoomM_ReadWaterInscription(void) {
  u16 *color = (u16 *)&CENTERED_TEXT_COLOR;

  Event_Start();
  Event_EndLastingPsynergy(0);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_SetTarget(PALETTE_TINT_WARM, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  *color = RGB_WHITE;
  wait(1);
  EventMessage_ShowCentered((s32)MsgMarsLighthouseRoomMFishWithColdCourageThey,
                            0, 0);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  Event_End();
}

/// Felix reads the inscription on the hall's floor east of the dragon statue's
/// chin: the screen dims to green and "Dragons... Burning with might, they
/// ruled the fires." appears across it.
void MarsLighthouseRoomM_ReadFireInscription(void) {
  u16 *color = (u16 *)&CENTERED_TEXT_COLOR;

  Event_Start();
  Event_EndLastingPsynergy(0);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_SetTarget(PALETTE_TINT_WARM, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  *color = RGB_WHITE;
  wait(1);
  EventMessage_ShowCentered(
      (s32)MsgMarsLighthouseRoomMDragonsBurningWithMightThey, 0, 0);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  Event_End();
}

/// Felix reads the inscription on the hall's gallery west of the dragon
/// statue's head: the screen dims to green and "Bird... On the wings of truth,
/// they ruled the winds." appears across it.
void MarsLighthouseRoomM_ReadWindInscription(void) {
  u16 *color = (u16 *)&CENTERED_TEXT_COLOR;

  Event_Start();
  Event_EndLastingPsynergy(0);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_SetTarget(PALETTE_TINT_WARM, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  *color = RGB_WHITE;
  wait(1);
  EventMessage_ShowCentered((s32)MsgMarsLighthouseRoomMBirdOnWingsOfTruth, 0,
                            0);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  Event_End();
}

/// A puff of flame where the fireball breaks the floor: one that swells as it
/// rises and shrinks back, or one thrown off at random.
void MarsLighthouseRoomM_SpawnFlamePuff(s32 x, s32 z, s32 kind) {
  struct Actor *actor;

  actor = FieldActor_Create(SPRITE_FLAME_PUFF, toFIXED(x), 0, toFIXED(z));
  if (actor != 0) {
    Actor_SetSpriteFlags(actor, 0);
    Actor_SelectAnimation(actor, 3);
    if (kind != 0) {
      ((struct PlacedSprite *)actor->sprite)->priority = 2;
      Actor_StartScript(actor, gMarsLighthouseRoomM_FlamePuffSwellScript);
    } else {
      ((struct PlacedSprite *)actor->sprite)->priority = 1;
      actor->velocityX = toFIXED(Random_Below(8) - 4);
      actor->velocityY = toFIXED(4);
      Actor_StartScript(actor, gMarsLighthouseRoomM_FlamePuffThrownScript);
    }
  }
}

/// One floor tile before the statue breaks in a burst of flame with a clatter,
/// showing its part of the round carving beneath: the cell (cellX, cellY)
/// takes the picture at (sourceX, sourceY).
void MarsLighthouseRoomM_BreakFloorTile(s32 x, s32 z, s32 sourceX, s32 sourceY,
                                        s32 cellX, s32 cellY) {
  u32 i;
  s32 px;
  s32 pz;

  Event_PlaySound(SE_CLATTER);
  MarsLighthouseRoomM_SpawnFlamePuff(x, z + 12, 1);
  for (i = 0; i < 3; i++) {
    px = x + (Random_Below(12)) - 6;
    pz = z + (Random_Below(12)) - 6;
    MarsLighthouseRoomM_SpawnFlamePuff(px, pz, 0);
  }
  wait(4);
  MapGrid_CopyCellsTo(sourceX, sourceY, cellX + 64, cellY, 1, 1);
}

/// The great dragon statue at the head of the hall speaks as Felix steps in
/// from the stairs (cells (11, 16) to (16, 16)): the camera rises to it and
/// the screen dims to green. Without the Mars Star it says "If you do not hold
/// the star, you shall not ascend to the heavens... Begone!", once. With the
/// star it bids the party set the four spirits aflame and the gallery's four
/// doorways flash, after which it speaks no more; from patch 1 it waits for
/// the lighthouse to have thawed as well.
void MarsLighthouseRoomM_HearDragonStatue(void) {
  if (PartyInventory_FindMember(ITEM_MYTHRIL_BAG_MARS_STAR) == -1) {
    if (!Event_GetFlag(FLAG_TOLD_TO_BEGONE)) {
      u16 *color = (u16 *)&CENTERED_TEXT_COLOR;

      Event_Start();
      Event_EndLastingPsynergy(0);
      FieldCamera_SetSpeed(2);
      FieldCamera_MoveTo(224, 64, 216, 1);
      FieldCamera_WaitForArrival();
      FieldEvent_Wait(20);
      PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
      PaletteFade_SetTarget(PALETTE_TINT_WARM, 0);
      PaletteFade_StartInterpolation(20);
      wait(20);
      *color = RGB_WHITE;
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
    u16 *color = (u16 *)&CENTERED_TEXT_COLOR;

    Event_Start();
    Event_EndLastingPsynergy(0);
    FieldCamera_SetSpeed(2);
    FieldCamera_MoveTo(224, 64, 216, 1);
    FieldCamera_WaitForArrival();
    FieldEvent_Wait(20);
    PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
    PaletteFade_SetTarget(PALETTE_TINT_WARM, 0);
    PaletteFade_StartInterpolation(20);
    wait(20);
    *color = RGB_WHITE;
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

void Actor_SetSpriteColorMode(struct Actor *actor, s32 value);
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

void Actor_SetSpriteFlags(struct Actor *actor, u32 flags);

#include "shared/actor_reset_scripted.h"

/// A flame thrower's flame sent forward from `position` at speed `value`;
/// `flag` keeps its sprite when off screen.
void Ride_SpawnForward(struct Vector3 *position, s32 value, s32 flag) {
  struct Actor *actor = FieldActor_Create(SPRITE_BLAZE_FLAME, position->x,
                                          position->y, position->z);

  if (actor != 0) {
    actor->bounce = 0;
    actor->gravity = 0;
    actor->riseBaseY = value;
    ((void (*)())Actor_ResetScripted)(actor, 2);
    actor->update = Flame_UpdateForward;
    if (flag != 0)
      actor->retainSpriteCache = 2;
    Actor_SelectAnimation(actor, 2);
  }
}

s32 Actor_IsTouching(struct Actor *a, struct Actor *b);
s32 Actor_IsBeside(struct Actor *a, struct Actor *b);

#include "shared/ride_update_landing.h"

void Actor_SetSpriteFlags(struct Actor *actor, u32 flags);

#include "shared/actor_setup_scripted_part.h"

/// A flame thrower's flame sent backward from `position` at speed `value`.
void Ride_SpawnBackward(struct Vector3 *position, s32 value, s32 flag) {
  struct Actor *actor = FieldActor_Create(SPRITE_BLAZE_FLAME, position->x,
                                          position->y, position->z);

  if (actor != 0) {
    actor->bounce = 0;
    actor->gravity = 0;
    actor->riseBaseY = -value;
    Actor_SetupScriptedPart(actor, 3);
    actor->update = Flame_UpdateForward;
    if (flag != 0)
      actor->retainSpriteCache = 2;
    Actor_SelectAnimation(actor, 2);
  }
}

/// A flame thrower's flame that lands as it goes forward from `position`.
void Ride_SpawnLandingForward(struct Vector3 *position, s32 value, s32 flag) {
  struct Actor *actor = FieldActor_Create(SPRITE_BLAZE_FLAME, position->x,
                                          position->y, position->z);

  if (actor != 0) {
    actor->bounce = 0;
    actor->gravity = 0;
    actor->riseBaseY = value;
    Actor_SetupScriptedPart(actor, 3);
    actor->update = Flame_UpdateLanding;
    if (flag != 0)
      actor->retainSpriteCache = 2;
    Actor_SelectAnimation(actor, 2);
  }
}

/// A flame thrower's flame that lands as it goes backward from `position`.
void Ride_SpawnLandingBackward(struct Vector3 *position, s32 value, s32 flag) {
  struct Actor *actor = FieldActor_Create(SPRITE_BLAZE_FLAME, position->x,
                                          position->y, position->z);

  if (actor != 0) {
    actor->bounce = 0;
    actor->gravity = 0;
    actor->riseBaseY = -value;
    Actor_SetupScriptedPart(actor, 3);
    actor->update = Flame_UpdateLanding;
    if (flag != 0)
      actor->retainSpriteCache = 2;
    Actor_SelectAnimation(actor, 2);
  }
}

#include "shared/actor_is_above.h"

#include "shared/ride_update_drift.h"

#include "shared/actor_setup_scripted.h"

/// A spark puffed from a flame thrower, drifting off in a random direction.
void Ride_SpawnSpark(struct Vector3 *position, s32 value, s32 mode) {
  struct Actor *actor = FieldActor_Create(
      SPRITE_DUST_CLOUD, position->x, position->y, position->z - toFIXED(2));
  s32 spin;

  if (actor != 0) {
    actor->bounce =
        Trig_Cos(((u32)(Random16() * 3 << 12) >> 16) + TRIG_TURN * 3 / 16);
    actor->gravity = (u16)Random16();
    actor->riseBaseY = ((u32)(Random16() << 17) >> 16) + value;
    spin = (u16)Random16() - DEGtoANG(180);
    actor->timer = spin;
    ((void (*)())Actor_SetupScripted)(actor, 2);
    actor->update = Ride_UpdateDrift;
    Actor_SetBlendMode(actor, 1);
  }
}

typedef struct Actor Actor;

#include "shared/flame_throwers_puff.c"

void Actor_Release(void *handle);
s32 MapGrid_CheckCollision(struct Actor *actor, s32 *position);

#include "shared/ride_create.c"

void Actor_SetSpriteColorMode(struct Actor *actor, s32 value);

#include "shared/ride_release_handle.h"

/// Sets whether the flame throwers keep puffing while Psynergy is used.
void Ride_SetMode(s16 value) { RIDE_STATE->runsDuringPsynergy = value; }

#include "shared/beam_start.h"

#include "shared/ride_get_entry_value.c"

#include "shared/ride_set_entry_value.c"

/// The flame throwers' counter, which Ride_AdvanceCounter steps.
s32 Ride_GetCounter(void) { return RIDE_STATE->counter; }

#include "shared/ride_advance_counter.h"

#include "shared/ride_set_entry.h"

#include "shared/ride_set_entry_count.c"

#include "shared/ride_get_entry_flags.c"

#include "shared/ride_set_slot_actor.h"

#include "shared/party_take_fall_damage.h"

#include "../field/field_effect_advance_spin.c"

#include "shared/actor_blink.h"

s32 MapGrid_GetTerrainKindOrBumpAt(s32 terrain, s32 x, s32 z);

#include "shared/leader_knock_back.c"

/// Hit by the flames, the leader is knocked back.
void MarsLighthouseRoomM_KnockBackIfHit(void) {
  if (RIDE_STATE->active)
    Leader_KnockBack();
}

s32 MapGrid_GetTerrainKindOrBumpAt(s32 layer, s32 x, s32 z);
void Actor_SetSpriteColorMode(struct Actor *actor, s32 value);

/// Hit by a rolling fireball: the leader is hurt, blinks, and is carried along
/// with it until it stops.
s32 Fireball_CarryLeader(void) {
  struct FieldState *event = Ram_HeapSlots->fieldState;
  struct FlameHitState *ride;
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
  // UB: the fall damage takes no arguments and returns nothing; it is passed
  // the arguments it gives Party_AddHp.
  ((s32 (*)())Party_TakeFallDamage)(-FALL_DAMAGE, 0);
#endif
  FieldCamera_SetSpeed(7);
  leader = Actor_Get(gMainRoutineState.leaderActorId);
  ride = RIDE_STATE;
  Event_SetFlag(FLAG_MENU_OPEN);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Event_ClearFlag(FLAG_MENU_OPEN);
  leader->update = (ObjectUpdate)Actor_Blink;
  leader->facing = FACING_SOUTH;
  Actor_SelectAnimation(leader, ANIM_LEADER_SIT);
  while (ride->car->timer == 0) {
    s32 kind =
        MapGrid_GetTerrainKindOrBumpAt(0, leader->x.fixed, leader->z.fixed);

    if (kind != TERRAIN_ROUNDED_SQUARE) {
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
s32 Ride_IsActive(void) { return RIDE_STATE->active; }

#include "shared/actor_scatter_crumbs.c"

#include "shared/embers_spawn.h"

#include "shared/actor_hold_during_event.c"

#include "shared/actor_roll_and_crumble.c"

#include "shared/actor_attach.c"

#include "shared/actor_launch_ahead.h"

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Actor_SetSpritePriorityDirect(struct Actor *actor, s32 mode);

#include "shared/actor_pop_up_slow.h"

#include "shared/actor_pop_up.h"

#include "shared/actor_park.h"

s32 gMarsLighthouseRoomM_TorchScript[] = {
    SC_CALL(MarsLighthouseRoomM_SetUpTorch),
    SC_END,
};

// clang-format off
#include "shared/mars_lighthouse_part_scripts.c"

#define TELEPORT_CIRCLE_FIRST_ENTRY 201
#include "shared/teleport_circle_palette.c"

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

struct ActorPlacement gMarsLighthouseRoomM_FlamePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH_LIT, CONDITION_ALWAYS, (s32)gMarsLighthouseRoomM_TorchScript, 504, 0, 184, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 504, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, (s32)gMarsLighthouseRoomM_TorchScript, 504, 0, 112, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 232, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gMarsLighthouseRoomM_InscriptionsPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_ROBED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 184, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROBED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 184, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_ROBED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 216, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_ROBED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 216, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 224, 0, 144, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gMarsLighthouseRoomM_Triggers[] = {
    {TRIGGER_TABLE_END},
};

/// The beacon ledge inscription's glow, in frames; -1 before it starts.
s32 gMarsLighthouseRoomM_LedgeCycleFrame = -1;

struct Trigger gMarsLighthouseRoomM_FlameTriggers[] = {
    {TRIGGER_STEP, CELL_LEDGE_DOORWAY, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomM_LeaveLedge)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_LEDGE_TORCH, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomM_StartBlazeOnTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS), ACTOR_LEDGE_BLAZE_TARGET, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomM_WatchBlazeReachFlame)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), ACTOR_LEDGE_BLAZE_TARGET, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomM_LightFlame)},
    {TRIGGER_TABLE_END},
};

/// The hall inscriptions' glow, in frames; -1 before it starts.
s32 gMarsLighthouseRoomM_HallCycleFrame = -1;

struct Trigger gMarsLighthouseRoomM_InscriptionsTriggers[] = {
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
    {TRIGGER_STEP, CELL_BEHAVIOR_VENUS_DOORWAY, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), EVENT(MarsLighthouseRoomM_StepTowardDoorway)},
    {TRIGGER_STEP, CELL_BEHAVIOR_VENUS_DOORWAY + 1, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), EVENT(MarsLighthouseRoomM_StepTowardDoorway)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MERCURY_DOORWAY, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), EVENT(MarsLighthouseRoomM_StepTowardDoorway)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MERCURY_DOORWAY + 1, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), EVENT(MarsLighthouseRoomM_StepTowardDoorway)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MARS_DOORWAY, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), EVENT(MarsLighthouseRoomM_StepTowardDoorway)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MARS_DOORWAY + 1, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), EVENT(MarsLighthouseRoomM_StepTowardDoorway)},
    {TRIGGER_STEP, CELL_BEHAVIOR_JUPITER_DOORWAY, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), EVENT(MarsLighthouseRoomM_StepTowardDoorway)},
    {TRIGGER_STEP, CELL_BEHAVIOR_JUPITER_DOORWAY + 1, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), EVENT(MarsLighthouseRoomM_StepTowardDoorway)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), CELL_AERIE_CARVING, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomM_TeleportToAerie)},
    {TRIGGER_EXAMINE, CELL_EARTH_INSCRIPTION, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomM_ReadEarthInscription)},
    {TRIGGER_EXAMINE, CELL_WATER_INSCRIPTION, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomM_ReadWaterInscription)},
    {TRIGGER_EXAMINE, CELL_FIRE_INSCRIPTION, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomM_ReadFireInscription)},
    {TRIGGER_EXAMINE, CELL_WIND_INSCRIPTION, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomM_ReadWindInscription)},
    {TRIGGER_STEP, CELL_BEFORE_DRAGON_STATUE, FLAG_TOLD_TO_LIGHT_FLAMES, EVENT(MarsLighthouseRoomM_HearDragonStatue)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMarsLighthouseRoomM_LedgesTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TABLE_END},
};

s32 gMarsLighthouseRoomM_FlamePuffSwellScript[] = {
    SC_ADD(SW_SCALE_X, FIXED_ONE * 3 / 16),
    SC_ADD(SW_SCALE_Y, FIXED_ONE * 3 / 16),
    SC_ADD(SW_Y, -toFIXED(3) / 32),
    SC_WAIT(1),
    SC_LOOP(8, SC_TOP),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -FIXED_ONE * 3 / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE * 3 / 16),
    SC_ADD(SW_Y, toFIXED(3) / 32),
    SC_WAIT(1),
    SC_LOOP(12, SC_TO(1)),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
};

s32 gMarsLighthouseRoomM_FlamePuffThrownScript[] = {
    SC_WAIT(20),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
};
// clang-format on
