/// @file
/// The terraces high outside Jupiter Lighthouse, over the clouds, which link
/// its main tower, by the red and blue doors, with its left and right towers.
/// On the walkway west to the left tower, a whirlwind blows Felix back east if
/// he walks into its path (JupiterLighthouse_RideUpperWhirlwind). Outside the
/// upper doorway, the grey block pushed one cell east onto a trapdoor and the
/// purple button in front of the doorway make a shortcut: standing on the
/// button opens the trapdoor and the block drops through
/// (JupiterLighthouse_OpenFloor). Pushed into its gap, the energy block draws a
/// bolt of lightning that lights the path to a Hover pad
/// (JupiterLighthouse_FinishMovingBlock); Hovering from it, Felix is caught by
/// a whirlwind that takes him on towards the right tower
/// (JupiterLighthouse_RideWhirlwind). Once both halves of the well's lid are
/// lifted, on the way back to the aerie the party finds Isaac's party: Garet
/// hangs from the ledge, Mia unable to pull him up, while Isaac and Ivan fight
/// Karst and Agatio (JupiterLighthouse_StartTheFight); the party arrives to
/// watch (JupiterLighthouse_PartyArrivesAtTheFight), and Isaac and Ivan are
/// hurt (JupiterLighthouse_IsaacAndIvanAreHurt) and fall
/// (JupiterLighthouse_IsaacAndIvanFall).
#include "flags.h"
#include "game/character_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "field/field_sequence.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/palette_buffers.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/scene_tables.h"
#include "system/main_routine_work.h"
#include "field/field_event.h"
#include "field/event_inline.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "item_ids.h"

void Leader_CenterInCell(void);
#include "system/io_reg.h"
#include "system/dma.h"
#include "system/io_write.h"
#include "field/call.h"
#include "field/field_sprite.h"
#include "system/frame.h"
#include "field/field_sequence_event.h"
#include "field/event_commands.h"
#include "field/cell_target.h"
#include "scene/scene.h"
#include "graphics/resource.h"
#include "scene_ids.h"
#include "version.h"
#include "shared/jupiter_lighthouse_aerie.h"
#include "system/iwram_call.h"
#include "system/field_effect.h"
#include "field/field_coordinate.h"
#include "field/map_grid.h"
#include "graphics/vram_tab.h"
#include "system/task.h"
#include "window/item_icon.h"
#include "system/heap_release.h"
#include "shared/tables.h"
#include "field/field_effect.h"
#include "field/actor_script.h"
#include "fixed_math.h"

/// The energy block pushed into its gap: lightning has struck it, and the
/// path's grooves light up to the Hover pad beyond.
#define FLAG_ENERGY_BLOCK_IN_GAP 0xa0c
/// The grey block pushed over the trapdoor has dropped through it, a shortcut
/// to the rooms below.
#define FLAG_GRAY_BLOCK_DROPPED 0xa0d
/// For this visit: Lash has tied the rope between the two tightrope posts.
#define FLAG_JUPITER_LIGHTHOUSE_ROPE_TIED FLAG_SCENE_TEMP_00
#include "macros.h"

struct CellTarget;
struct CellTargetEntry;

void PaletteCycle_SetPausesInEvents(s32 value);

extern u32 gJupiterLighthouse_GaretHangsScript[];
extern u32 gJupiterLighthouse_MiaScript[];
extern u32 gJupiterLighthouse_FelixLooksAroundScript[];
extern u32 gJupiterLighthouse_KradenArrivesWestScript[];
extern u32 gJupiterLighthouse_KradenArrivesEastScript[];
extern u32 gJupiterLighthouse_PiersArrivesWestScript[];
extern u32 gJupiterLighthouse_JennaArrivesEastScript[];
extern u32 gJupiterLighthouse_ShebaArrivesWestScript[];

extern u32 gJupiterLighthouse_AgatioStepsUpScript[];
extern u32 gJupiterLighthouse_KarstTurnsScript[];
extern u32 gJupiterLighthouse_KarstFacesSouthScript[];
extern u32 gJupiterLighthouse_AgatioFacesEastScript[];
extern u32 gJupiterLighthouse_IsaacFacesWestScript[];
extern u32 gJupiterLighthouse_IvanFacesNorthScript[];

extern u32 gJupiterLighthouse_PiersStepsBackScript[];

extern u32 gJupiterLighthouse_FelixWaitsScript[];
extern u32 gJupiterLighthouse_KradenWaitsScript[];
extern u32 gJupiterLighthouse_PiersStepsUpScript[];
extern u32 gJupiterLighthouse_ShebaStepsUpScript[];
extern u32 gJupiterLighthouse_JennaStepsUpScript[];

extern u32 gJupiterLighthouse_WalkToAerieScript[];
extern u32 gJupiterLighthouse_LeaveForAerieScript[];

void JupiterLighthouse_LoadPalettes(s32 mode);

void JupiterLighthouse_RideUpperWhirlwind(void);

void JupiterLighthouse_RideWhirlwind(void);

void JupiterLighthouse_OpenFloor(void);

void JupiterLighthouse_CloseFloor(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];

s32 gWhirlwindRiding BSS_DATA;
u16 gPaletteCycleColors[16] BSS_DATA;
/// Palette 13's first two colours; it is copied whole, with the unlit colours
/// after them.
u16 gPaletteCycleUnlitPalette[2] BSS_DATA;
u16 gPaletteCycleUnlitColors[14] BSS_DATA;
s16 gLightSpriteTile BSS_DATA;
s16 gLightSpriteCount BSS_DATA;
struct Actor *gLightSpriteActors[16] BSS_DATA;
struct LightOam gLightSpriteOams[16] BSS_DATA;
s16 gLightSpritesLarge BSS_DATA;
s16 gLightSpritePriority BSS_DATA;
struct CellTarget gCrackedTiles BSS_DATA;

s32 BattleEffect_GetObjectPriority(struct Actor *actor);

void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);

void CrackedTile_Fall(struct CellTarget *table, struct CellTargetEntry *entry);

void Party_ClearUnusableShortcuts(void);
void HangingVine_Place(s32 id, s32 mode);
void Rope_Initialize(void);
void Rope_Place(s32 a, s32 b, s32 c, s32 d);
void MapGrid_SetAttributeBAt();
s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);

#include "text/msg_ids.h"

/// An action script's call: clears the actor's sprite flags.
s32 JupiterLighthouse_ClearScriptedActorSpriteFlags(s32 object) {
  Actor_SetSpriteFlags(object, 0);
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

/// Where the battle this room starts returns the party, won or lost; and an
/// entrance no exit or battle leads to, which plays Isaac and Ivan's fall
/// again without its flags.
enum { ENTRANCE_AFTER_THE_FIGHT = 77, ENTRANCE_ISAAC_AND_IVAN_FALL = 99 };

/// The energy block's gap: the cell it drops into and lights the path from.
enum { ENERGY_BLOCK_GAP_CELL_X = 63, ENERGY_BLOCK_GAP_CELL_Z = 15 };
/// The terraces' own cell behaviors.
enum JupiterLighthouseRoomMCellBehavior {
  /// The west walkway's cells (13, 9) to (20, 9), in the west whirlwind's
  /// path.
  CELL_BEHAVIOR_WEST_WHIRLWIND_PATH = 30,
  /// Cell (65, 42), past the Hover pad the energy block lights, in the east
  /// whirlwind's path.
  CELL_BEHAVIOR_EAST_WHIRLWIND_PATH,
  /// The purple button in front of the upper doorway, cell (41, 39), and the
  /// eight cells round it.
  CELL_BEHAVIOR_TRAPDOOR_BUTTON,
  CELL_BEHAVIOR_OFF_TRAPDOOR_BUTTON
};

/// The bits of a lightning map entry that number the map's tiles from 512;
/// cleared, the map draws the tiles at the start of character block 0.
#define LIGHTNING_MAP_TILE_BASE_MASK 0x300

/// More HP or PP than anyone has, so that taking it away leaves none; and the
/// HP Isaac is left with for the fight that cannot be won.
enum { ALL_HP_OR_PP = 10000, ISAAC_HP_IN_THE_FIGHT = 173 };

/// The actors this file moves by number: what the placements put where, and
/// the people at the fight (gActorPlacements's last rows).
enum {
  /// The stone block Move pushes into its gap, which the lightning strikes
  /// (JupiterLighthouse_FlashSky).
  ACTOR_ENERGY_BLOCK = 20,
  /// The two tightrope posts Lash ties the rope between.
  ACTOR_EAST_ROPE_POST = 21,
  ACTOR_WEST_ROPE_POST = 22,
  /// The grey block pushed onto the trapdoor (JupiterLighthouse_OpenFloor).
  ACTOR_GRAY_BLOCK = 23,
  /// The unseen points the west and east whirlwinds blow out from.
  ACTOR_WEST_UPPER_GUST = 24,
  ACTOR_EAST_UPPER_GUST = 25,
  /// "And I'm Agatio."
  ACTOR_AGATIO = 26,
  /// "Me? Why, I'm Karst."
  ACTOR_KARST = 27,
  ACTOR_KRADEN = 28,
  /// The point by Garet's hand on the ledge, where he strains every 128
  /// frames (JupiterLighthouse_KeepGaretHanging).
  ACTOR_GARETS_GRIP = 29
};

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

struct Actor *Whirlwind_Spawn(struct Actor *source);

/// Felix walks into the west whirlwind's path, on the walkway towards the left
/// tower at cells (13, 9) to (20, 9): a whirlwind blows out at him, catches him
/// and carries him spinning back east along the walkway, over the clouds, to
/// the doorway (Whirlwind_CarryLeader). Nothing happens while a whirlwind is
/// already carrying him.
void JupiterLighthouse_RideUpperWhirlwind(void) {
  if (gWhirlwindRiding == 0) {
    Whirlwind_Spawn(Actor_Get(ACTOR_WEST_UPPER_GUST));
    gWhirlwindRiding = 1;
  }
}

/// Felix casts Hover on the pad the energy block lights, at cell (65, 42), and
/// rises in its glow: the east whirlwind catches him and carries him, still in
/// Hover's rings, east along the chained walkway over the clouds to the right
/// tower, and sets him down on the walkway's east end by the tower wall, beside
/// the eye-shaped switch by its door (Whirlwind_CarryLeader). His Hover's
/// movement ends as he is caught. Walking there without Hover does nothing.
void JupiterLighthouse_RideWhirlwind(void) {
  if (gWhirlwindRiding == 0) {
    struct MainRoutineWork *party = &gMainRoutineWork;

    if (party->movementMode == LEADER_MOVEMENT_HOVER) {
      struct Actor *actor = Actor_Get(party->leaderActorId);

      party->movementMode = LEADER_MOVEMENT_NONE;
      actor->speedLimit = 0;
      actor->acceleration = 0;
      Whirlwind_Spawn(Actor_Get(ACTOR_EAST_UPPER_GUST));
      gWhirlwindRiding = 1;
    }
  }
}

s32 Actor_EmitRisingSparks(struct Actor *actor);

/// Lightning strikes the energy block in a white flash, the ground shakes,
/// Felix jumps back, sparks rise from the block, and the path lights up to the
/// Hover pad beyond the gap, at cell (65, 10). The bolt is a picture drawn on
/// the first background layer, faded out over the scene.
void JupiterLighthouse_FlashSky(void) {
  u16 *buffer;
  u16 *p;
  u32 i;

  {
    u8 *enabled = &gCpuLoadDisplayEnabled;

    *enabled = 0;
  }
  REG_DISPCNT &= ~DISPCNT_BG0_ON;
  Io_Write16(BGCNT_SCREEN_BASE(LIGHTNING_SCREEN_BLOCK), &REG_BG0CNT);
  buffer = Heap_Alloc(LIGHTNING_TILES * TILE_SIZE_4BPP);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_LIGHTNING_TILES), buffer);
  Dma_Set(buffer, (void *)BG_CHAR_BLOCK(0),
          DMA_COPY_WORDS(LIGHTNING_TILES * TILE_SIZE_4BPP / 4), REG_DMA3);
  Heap_Release(buffer);
  Dma_Fill16(0, BG_SCREEN_BLOCK(LIGHTNING_SCREEN_BLOCK), BG_SCREEN_SIZE);
  buffer = Heap_Alloc(LIGHTNING_MAP_ROWS * LIGHTNING_MAP_COLUMNS * sizeof(u16));
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_LIGHTNING_MAP), buffer);
  p = buffer;
  for (i = 0; i != LIGHTNING_MAP_COLUMNS * LIGHTNING_MAP_ROWS; i++)
    *p++ &= ~LIGHTNING_MAP_TILE_BASE_MASK;
  p = buffer;
  for (i = 0; i != LIGHTNING_MAP_ROWS; i++) {
    Dma_Set(p,
            BG_SCREEN_BLOCK(LIGHTNING_SCREEN_BLOCK) +
                i * BG_SCREEN_WIDTH * sizeof(u16),
            DMA_COPY_WORDS(LIGHTNING_MAP_COLUMNS * sizeof(u16) / 4), REG_DMA3);
    p += LIGHTNING_MAP_COLUMNS;
  }
  Heap_Release(buffer);
  REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, BLEND_WEIGHT_MAX);
  REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  gBgScroll[0].x = -104;
  gBgScroll[0].y = 16;
  System_Wait(30);
  PaletteFade_ApplySource(PALETTE_TINT_WARM, 0);
  PaletteFade_ApplyTarget(RGB_WHITE, 0);
  PaletteFade_Interpolate(1);
  System_Wait(1);
  PaletteFade_ApplySource(RGB_WHITE, 0);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, 0);
  PaletteFade_Interpolate(8);
  REG_DISPCNT |= DISPCNT_BG0_ON;
  Event_PlaySound(SE_CLATTER);
  MapGrid_SetShake(5, 7, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 2);
  FieldActor_Jump(ACTOR_FELIX, 6, 0);
  Actor_Get(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkBy(ACTOR_FELIX, -16, 0);
  Actor_Get(ACTOR_ENERGY_BLOCK)->update = Actor_EmitRisingSparks;
  for (i = 0; i != 2 * BLEND_WEIGHT_MAX; i++) {
    REG_BLDALPHA =
        (BLEND_WEIGHT_MAX - (i >> 1)) | BLEND_ALPHA(0, BLEND_WEIGHT_MAX);
    System_Wait(1);
  }
  MapGrid_CopyCellsLayered(104, 42, 5, 11, 61, 42);
  MapGrid_CopyCellsLayered(106, 52, 1, 1, 63, 84);
  FieldMapGrid_SetCellBehavior(toFIXED(1040), toFIXED(160), 2,
                               CELL_BEHAVIOR_HOVER);
  PaletteFade_ApplySource(PALETTE_TINT_WARM, 0);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 0);
  PaletteFade_Interpolate(60);
  Window_LoadGraphics();
  DebugText_Clear();
  FieldWindowColor_Apply(gMainRoutineWork.windowHue,
                         gMainRoutineWork.windowBrightness);
  REG_BLDALPHA = BLEND_ALPHA(8, 16);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  REG_DISPCNT |= DISPCNT_BG0_ON;
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
  System_Wait(1);
  gCpuLoadDisplayEnabled = 1;
  MapGrid_SetShake(0, 0, 0);
}

extern void MapGrid_SetCellHeight(s32 x, s32 z, s32 a, s32 b);

/// Nothing visible happens as Felix starts to push the energy block, by
/// walking into it or with Move. The cell the block stands on is kept at its
/// top, 48 pixels down, a step above the walkway 64 pixels down (cell (60, 15)
/// as the room is entered); as the push starts the game sets that cell back to
/// the walkway's 64 pixels down, so the cell the block leaves is plain walkway
/// again. Where the block stops, JupiterLighthouse_FinishMovingBlock raises its
/// new cell to 48 pixels down.
void JupiterLighthouse_StartMovingBlock(s32 unused, s32 id) {
  struct Actor *actor = Actor_Get(id);

  Call4(MapGrid_SetCellHeight, actor->x.fixed, actor->z.fixed, 0, -8);
}

/// MapGrid_SetCellHeight, as the block's landing calls it.
static __inline__ void JupiterLighthouse_SetCellHeight(s32 x, s32 z, s32 a,
                                                       s32 b) {
  MapGrid_SetCellHeight(x, z, a, b);
}

/// The energy block, pushed or Moved three cells east into the gap at cell (63,
/// 15), drops in with a heavy thud. Felix steps back west of the gap, the light
/// turns warm, he looks about with a question mark and leans back, and
/// lightning strikes the block (JupiterLighthouse_FlashSky); the camera then
/// follows the lit path to the Hover pad as the puzzle-solved jingle plays.
/// Anywhere else, the block simply stands where it is put.
void JupiterLighthouse_FinishMovingBlock(s32 unused, s32 id) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *actor = Actor_GetById(id);
  s32 x;
  s32 frames;

  if (FIELD_CELL(actor->x.fixed) == ENERGY_BLOCK_GAP_CELL_X &&
      FIELD_CELL(actor->z.fixed) == ENERGY_BLOCK_GAP_CELL_Z) {
    Event_SetFlag(FLAG_ENERGY_BLOCK_IN_GAP);
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    System_Wait(2);
    frames = 0;
    if (actor->y.fixed > actor->groundHeight || actor->velocityY != 0) {
      do {
        System_Wait(1);
        frames++;
        if (frames > 29)
          break;
      } while (actor->y.fixed > actor->groundHeight || actor->velocityY != 0);
    }
    actor->motionFlags = 0;
    Event_PlaySound(SE_HEAVY_IMPACT);
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldCamera_MoveTo(1016, CAMERA_KEEP, 232, 1);
    FieldCamera_WaitForArrival();
    Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1);
    x = FIELD_CELL(leader->x.fixed);
    if (x == ENERGY_BLOCK_GAP_CELL_X + 1) {
      Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 1032, 232);
      Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 1000, 232);
    } else if (x == ENERGY_BLOCK_GAP_CELL_X) {
      Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 1000, 232);
    }
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 1000, 248);
    Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_EAST);
    PaletteFade_ApplySource(PALETTE_TINT_NONE, 0);
    PaletteFade_ApplyTarget(PALETTE_TINT_WARM, 0);
    PaletteFade_Interpolate(60);
    FieldEvent_Wait(50);
    Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId,
                               FACING_SOUTHWEST);
    FieldEvent_Wait(20);
    Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId,
                               FACING_SOUTHEAST);
    FieldEvent_Wait(20);
    Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId,
                               FACING_SOUTHWEST);
    FieldEvent_Wait(20);
    Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId,
                               FACING_SOUTHEAST);
    FieldEvent_Wait(20);
    Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_SOUTH);
    FieldEvent_Wait(20);
    FieldActor_ShowEmote(gMainRoutineWork.leaderActorId,
                         EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
    Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_EAST);
    FieldEvent_Wait(20);
    Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_LEADER_LEAN_BACK);
    FieldEvent_Wait(40);
    PaletteCycle_SetPausesInEvents(0);
    JupiterLighthouse_FlashSky();
    Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);
    Actor_GetById(gMainRoutineWork.leaderActorId)->steeringFlags |=
        ACTOR_FACES_MOTION;
    FieldActor_StartRepeatedMotion(ACTOR_FELIX, 3);
    Actor_SetAttachedEffect(ACTOR_FELIX,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    FieldEvent_Wait(40);
    FieldEvent_Wait(60);
    FieldCamera_SetSpeed(3);
    FieldCamera_MoveTo(1048, 0, 174, 1);
    FieldCamera_WaitForArrival();
    FieldEvent_Wait(10);
    actor->update = 0;
    Event_PlaySound(MUS_PUZZLE_SOLVED);
    Sound_WaitForJingle();
    FieldEvent_Wait(10);
    FieldCamera_MoveTo(984, -64, 248, 1);
    FieldCamera_WaitForArrival();
    FieldEvent_Wait(10);
    PaletteCycle_SetPausesInEvents(1);
    Event_End();
  }
  JupiterLighthouse_SetCellHeight(actor->x.fixed, actor->z.fixed, 0, -6);
}

/// Felix steps onto the purple button in front of the upper doorway, cell (41,
/// 39): the camera turns to the trapdoor of two green panels nearby, which
/// swing open. If the grey block has been pushed one cell east onto it, it
/// falls through, the first time only, a few levels down to the rooms below
/// with a crash and a shake: over the trapdoor's middle it first lands on a
/// ledge with a shake and slides off sideways. Once the well's lid is lifted
/// the button does nothing.
void JupiterLighthouse_OpenFloor(void) {
  struct Actor *actor = Actor_GetById(ACTOR_GRAY_BLOCK);
  s32 spot = 0;
  s32 x;
  s32 frames;

  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_LID_LIFTED))
    return;
  GameFlag_Set(FLAG_SCENE_TEMP_02);
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_CHEST_OPEN);
  FieldCamera_MoveTo(776, CAMERA_KEEP, 208, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(6);
  Sound_Play(SE_FLOOR_SWITCH);
  MapGrid_CopyCellsLayered(49, 103, 4, 3, 49, 76);
  Event_Wait(6);
  MapGrid_CopyCellsLayered(49, 106, 4, 3, 49, 76);
  x = FIELD_CELL(actor->x.fixed);
  if (x == 49)
    spot = 1;
  if (x == 50)
    spot = 2;
  if (x == 51)
    spot = 3;
  if (x == 52)
    spot = 4;
  if (actor->y.fixed != 0)
    spot = 0;
  if (Event_GetFlag(FLAG_GRAY_BLOCK_DROPPED))
    spot = 0;
  if (spot != 0)
    GameFlag_Set(FLAG_GRAY_BLOCK_DROPPED);
  if ((u32)(spot - 2) <= 1) {
    FieldCamera_SetSpeed(4);
    EventCamera_FollowActor(ACTOR_GRAY_BLOCK, 1);
    FieldCamera_WaitForArrival();
    Sound_Play(SE_FALL);
    actor->gravity = toFIXED(1) / 2;
    actor->motionFlags = ACTOR_FALLS;
    actor->groundHeight = toFIXED(-80);
    frames = 0;
    if (actor->y.fixed > actor->groundHeight) {
      do {
        System_Wait(1);
        frames++;
        if (frames > 29)
          break;
      } while (actor->y.fixed > actor->groundHeight);
    }
    MapGrid_SetShake(3, 3, 0.95);
    Sound_Play(SE_HEAVY_IMPACT);
    actor->gravity = toFIXED(1);
    actor->velocityY = toFIXED(6);
    Actor_SetSpeed(ACTOR_GRAY_BLOCK, 1.5);
    if (spot == 2)
      Actor_SetDestinationOffset(ACTOR_GRAY_BLOCK, -32, 0);
    else
      Actor_SetDestinationOffset(ACTOR_GRAY_BLOCK, 32, 0);
    Event_Wait(7);
    FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    actor->gravity = toFIXED(1) / 2;
    actor->groundHeight = toFIXED(-240);
    Event_Wait(80);
    MapGrid_SetShake(5, 5, 1);
    Sound_Play(SE_CLATTER);
    Event_Wait(20);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    Actor_SetPosition(ACTOR_GRAY_BLOCK, 0, 0);
    Event_Wait(10);
  } else if (spot == 1 || spot == 4) {
    FieldActor_SetSpritePriority(ACTOR_GRAY_BLOCK, 1);
    FieldCamera_SetSpeed(4);
    EventCamera_FollowActor(ACTOR_GRAY_BLOCK, 1);
    FieldCamera_WaitForArrival();
    Sound_Play(SE_FALL);
    actor->gravity = toFIXED(1) / 2;
    actor->motionFlags = ACTOR_FALLS;
    actor->groundHeight = toFIXED(-240);
    Event_Wait(20);
    FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    Event_Wait(60);
    MapGrid_SetShake(5, 5, 1);
    Sound_Play(SE_CLATTER);
    Event_Wait(20);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    Actor_SetPosition(ACTOR_GRAY_BLOCK, 0, 0);
    Event_Wait(10);
  }
  Event_Wait(20);
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  Event_End();
}

/// Felix steps off the purple button in front of the upper doorway: the camera
/// turns to the trapdoor, which swings shut with a thud. Once the well's lid is
/// lifted the trapdoor stays as it is.
void JupiterLighthouse_CloseFloor(void) {
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_LID_LIFTED) != 0)
    return;
  Event_ClearFlag(FLAG_SCENE_TEMP_02);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_CHEST_CLOSE);
  FieldCamera_MoveTo(776, CAMERA_KEEP, 208, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(6);
  MapGrid_CopyCellsLayered(49, 103, 4, 3, 49, 76);
  FieldEvent_Wait(6);
  Event_PlaySound(SE_IMPACT);
  MapGrid_CopyCellsLayered(49, 100, 4, 3, 49, 76);
  FieldEvent_Wait(20);
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  Event_End();
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Talking to Isaac, Ivan, Kraden, Jenna or Sheba once Isaac and Ivan have
/// fallen: each answers with a line of their own.
void JupiterLighthouse_TalkAfterTheFight(s32 actor) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  switch (actor) {
  case ACTOR_ISAAC:
    Actor_RunRepeatedMotion(ACTOR_ISAAC, 2);
    FieldEvent_SetMessageId(MsgNoWhyAre);
    // "Felix... No! Why are you helping them?"
    Event_ShowMessage(ACTOR_ISAAC, 0);
    break;
  case ACTOR_IVAN:
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    FieldEvent_SetMessageId(MsgDonTWorry);
    // "Don't worry about us... Help Garet and Mia first."
    Event_ShowMessage(ACTOR_IVAN, 0);
    break;
  case ACTOR_KRADEN:
    FieldEvent_SetMessageId(MsgWeHaveTo);
    // "We have to help Garet. He can't hold on for much longer."
    Event_ShowMessage(ACTOR_KRADEN, 0);
    break;
  case ACTOR_JENNA:
    FieldEvent_SetMessageId(MsgIDonT);
    // "Isaac! I don't know how you survived that attack, but I'm so glad..."
    Event_ShowMessage(ACTOR_JENNA, 0);
    break;
  case ACTOR_SHEBA:
    Actor_SetAnimation(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
    FieldEvent_Wait(33);
    FieldEvent_SetMessageId(MsgYouVeBeen);
    // "Ivan... You've been injured. Don't worry about the others. Let's..."
    Event_ShowMessage(ACTOR_SHEBA, 0);
    break;
  }
  Event_End();
}

/// Garet's sprite's rotation swings by the sine over this: up to a 128th of a
/// turn either way.
#define GARET_SWING_DIVISOR 128

/// A frame task while Garet hangs from the ledge: his sprite, pivoting on his
/// hand, swings on a sine about a tilt of a 32nd of a turn, and every 128
/// frames his grip strains.
s32 JupiterLighthouse_KeepGaretHanging(void) {
  struct Actor *actor = Actor_Get(ACTOR_GARET);

  if (actor != NULL) {
    struct Sprite *sprite = actor->sprite;

    if ((gFrameCount & 127) == 0)
      Actor_SetAttachedEffect(ACTOR_GARETS_GRIP,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    if (sprite != NULL) {
      // Drawn down and to the left of his point, below his grip.
      sprite->offsetY = 30;
      sprite->offsetX = -4;
      sprite->rotation =
          Math_Sin(gFrameCount * DEGtoANG(4)) / GARET_SWING_DIVISOR -
          TRIG_TURN / 32;
    }
  }
  return 1;
}

/// An action script's call during the fight: the sound of one of five blows,
/// at random.
s32 JupiterLighthouse_PlayRandomBlowSound(void) {
  switch (Random16() & 7) {
  case 0:
    Event_PlaySound(SE_RECOVERY);
    break;
  case 1:
    Event_PlaySound(SE_POWER_UP);
    break;
  case 2:
    Event_PlaySound(SE_HEAVY_IMPACT);
    break;
  case 3:
  case 5:
  case 6:
  default:
    Event_PlaySound(SE_IMPACT);
    break;
  case 4:
  case 7:
    Event_PlaySound(SE_CAST);
    break;
  }
  return 0;
}

/// An action script's call: stops the actor turning to face where it moves.
s32 JupiterLighthouse_StopFacingMotion(struct Actor *actor) {
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  return 0;
}

extern u32 gJupiterLighthouse_IsaacFightsScript[];
extern u32 gJupiterLighthouse_AgatioFightsScript[];
extern u32 gJupiterLighthouse_IvanFightsScript[];
extern u32 gJupiterLighthouse_KarstFightsScript[];

/// Places an actor at (x, z), in pixels, facing a direction.
#define PLACE(id, x, z, facing)                                                \
  {                                                                            \
    s32 facing_ = (facing);                                                    \
    Actor_SetPositionAndFacing(id, toFIXED(x), toFIXED(z), facing_);           \
  }

/// Sets four actors' speeds, places them and starts their action scripts.
void JupiterLighthouse_StartTheFight(void) {
  Actor_SetSpeed(ACTOR_AGATIO, 3);
  Actor_SetSpeed(ACTOR_KARST, 3);
  Actor_SetSpeed(ACTOR_ISAAC, 3);
  Actor_SetSpeed(ACTOR_IVAN, 3);
  PLACE(ACTOR_ISAAC, 776, 202, FACING_WEST);
  PLACE(ACTOR_IVAN, 731, 232, FACING_NORTH);
  PLACE(ACTOR_KARST, 731, 213, FACING_SOUTH);
  PLACE(ACTOR_AGATIO, 737, 202, FACING_EAST);
  Actor_SetScript(ACTOR_ISAAC, gJupiterLighthouse_IsaacFightsScript);
  Actor_SetScript(ACTOR_IVAN, gJupiterLighthouse_IvanFightsScript);
  Actor_SetScript(ACTOR_AGATIO, gJupiterLighthouse_AgatioFightsScript);
  Actor_SetScript(ACTOR_KARST, gJupiterLighthouse_KarstFightsScript);
  Actor_SetScript(ACTOR_MIA, gJupiterLighthouse_MiaScript);
}

/// Places Isaac, Ivan and Mia where the fight is, and Garet hanging from the
/// ledge by his grip, swaying.
void JupiterLighthouse_PlaceForTheFight(void) {
  PLACE(ACTOR_IVAN, 757, 216, FACING_SOUTHEAST);
  PLACE(ACTOR_ISAAC, 761, 219, FACING_SOUTHEAST);
  PLACE(ACTOR_MIA, 816, 285, FACING_SOUTHEAST);
  PLACE(ACTOR_GARET, 824, 302, FACING_NORTH);
  PLACE(ACTOR_GARETS_GRIP, 821, 302, FACING_NORTH);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET), 0);
  Actor_SetScript(ACTOR_GARET, gJupiterLighthouse_GaretHangsScript);
  {
    s32 priority = TASK_PASS_FRAME;

    FieldTask_AddFrame((void (*)(void))JupiterLighthouse_KeepGaretHanging,
                       priority);
  }
  Actor_SetAnimation(ACTOR_ISAAC, 38);
  Actor_SetAnimation(ACTOR_IVAN, 31);
  Actor_SetAnimation(ACTOR_MIA, 9);
}

extern u32 gJupiterLighthouse_FelixArrivesWestScript[];
extern u32 gJupiterLighthouse_FelixArrivesEastScript[];
extern u32 gJupiterLighthouse_PiersArrivesEastScript[];
extern u32 gJupiterLighthouse_JennaArrivesWestScript[];
extern u32 gJupiterLighthouse_ShebaArrivesEastScript[];
extern u32 gJupiterLighthouse_KarstStepsUpScript[];

/// Felix comes out by the aerie to find Jenna, Sheba, Piers and Kraden
/// watching the fight, and the party is placed to watch with them; arg chooses
/// between the two ways of arriving.
void JupiterLighthouse_PartyArrivesAtTheFight(s32 arg) {
  struct Actor *actor;
  s32 angle;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetMessageId(MsgAhhhh);
  Event_PlaySound(SE_CHEST_OPEN);
  FieldEvent_Wait(26);
  Event_PlaySound(SE_FLOOR_SWITCH);
  Actor_SetScript(ACTOR_FELIX, gJupiterLighthouse_FelixLooksAroundScript);
  if (arg != 0) {
    // "Ahhhh!!!" and "Mia!"
    FieldEvent_ShowTwoMessagesAndWait(ACTOR_MIA, 20, 9, 15, 8, ACTOR_GARET, 20,
                                      2, 15, 1, 25);
    Event_PlaySound(SE_FALL);
    FieldEvent_Wait(30);
    Event_PlaySound(SE_CLATTER);
    // "Noooo!!!" and "Oof!"
    FieldEvent_ShowTwoMessagesAndWait(ACTOR_MIA, 21, 9, 16, 8, ACTOR_GARET, 21,
                                      2, 16, 1, 0);
    // "Mia!" and "Garet!"
    FieldEvent_ShowTwoMessagesAndWait(ACTOR_ISAAC, 20, 9, 15, 8, ACTOR_IVAN, 20,
                                      2, 15, 1, 0);
  } else {
    // "Ahhhh!!!" and "Mia!"
    FieldEvent_ShowTwoMessagesAndWait(ACTOR_MIA, 5, 9, 0, 8, ACTOR_GARET, 5, 2,
                                      0, 1, 25);
    Event_PlaySound(SE_FALL);
    FieldEvent_Wait(30);
    Event_PlaySound(SE_CLATTER);
    // "Noooo!!!" and "Oof!"
    FieldEvent_ShowTwoMessagesAndWait(ACTOR_MIA, 5, 9, 0, 8, ACTOR_GARET, 5, 2,
                                      0, 1, 0);
    // "Mia!" and "Garet!"
    FieldEvent_ShowTwoMessagesAndWait(ACTOR_ISAAC, 5, 9, 0, 8, ACTOR_IVAN, 5, 2,
                                      0, 1, 0);
  }
  FieldActor_Stop(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  FieldActor_LaunchFromLeader(ACTOR_PIERS, 16, 0, FACING_WEST);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, -16, 0, FACING_EAST);
  FieldActor_LaunchFromLeader(ACTOR_SHEBA, 16, 24, FACING_WEST);
  FieldActor_LaunchFromLeader(ACTOR_JENNA, -16, 24, FACING_EAST);
  EventCamera_FollowActor(ACTOR_JENNA, 1);
  FieldActor_WaitForMoveById(ACTOR_JENNA);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Is that... It's Isaac and the others!"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH_NORTHWEST, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHEAST, 0);
  // "Isaac? Isn't he a friend of yours, Felix?" Yes: "They both come from
  // Vale..." No: "Don't be that way! You two have much in common..."
  Event_OpenMessage(ACTOR_PIERS, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
    FieldEvent_Wait(20);
    FieldEvent_Wait(10);
    // "They both come from Vale, and regardless of what Isaac believes..."
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
    (gFieldWork->message)++;
  } else {
    Actor_SetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
    FieldEvent_Wait(33);
    (gFieldWork->message)++;
    // "Don't be that way! You two have much in common, even if you are at..."
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  }
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  FieldEvent_Wait(5);
  // "But Isaac and the others came all this way just to try and stop us!"
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 5);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "But didn't you hear that? It sounded like a cry for help... What if..."
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 5);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_SetSpeed(ACTOR_FELIX, 1.2);
  Actor_SetScript(ACTOR_FELIX, arg != 0
                                   ? gJupiterLighthouse_FelixArrivesWestScript
                                   : gJupiterLighthouse_FelixArrivesEastScript);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_SetSpeed(ACTOR_JENNA, 1);
  Actor_SetSpeed(ACTOR_PIERS, 1);
  Actor_SetSpeed(ACTOR_SHEBA, 1);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_SetScript(ACTOR_KRADEN,
                  arg != 0 ? gJupiterLighthouse_KradenArrivesWestScript
                           : gJupiterLighthouse_KradenArrivesEastScript);
  Actor_SetScript(ACTOR_PIERS, arg != 0
                                   ? gJupiterLighthouse_PiersArrivesWestScript
                                   : gJupiterLighthouse_PiersArrivesEastScript);
  Actor_SetScript(ACTOR_JENNA, arg != 0
                                   ? gJupiterLighthouse_JennaArrivesWestScript
                                   : gJupiterLighthouse_JennaArrivesEastScript);
  Actor_SetScript(ACTOR_SHEBA, arg != 0
                                   ? gJupiterLighthouse_ShebaArrivesWestScript
                                   : gJupiterLighthouse_ShebaArrivesEastScript);
  FieldCamera_SetSpeed(0.5);
  if (arg != 0) {
    FieldCamera_MoveTo(677, -64, 287, 1);
    FieldCamera_WaitForArrival();
  } else {
    FieldCamera_MoveTo(950, -64, 287, 1);
    FieldCamera_WaitForArrival();
  }
  FieldActor_WaitForScriptEndById(ACTOR_FELIX);
  FieldActor_WaitForScriptEndById(ACTOR_PIERS);
  FieldActor_WaitForScriptEndById(ACTOR_JENNA);
  FieldActor_WaitForScriptEndById(ACTOR_SHEBA);
  FieldActor_WaitForScriptEndById(ACTOR_KRADEN);
  FieldEvent_Wait(20);
  angle = FACING_SOUTHEAST;
  Actor_SetPositionAndFacing(ACTOR_IVAN, toFIXED(741), toFIXED(232), angle);
  FieldCamera_SetSpeed(4);
  FieldCamera_MoveTo(800, 0, 270, 1);
  FieldCamera_WaitForArrival();
  Event_PlaySound(MUS_ALEX);
  FieldEvent_Wait(50);
  Actor_RunRepeatedMotion(ACTOR_ISAAC, 2);
  // "Hang on, Mia! Don't worry, Garet! Everything will be fine!"
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 5);
  Actor_SetAnimation(ACTOR_MIA, ANIM_STILL);
  Actor_FaceDirectionAndWait(ACTOR_MIA, FACING_NORTHWEST);
  // "Don't worry about me... I'm fine, but Garet..."
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_MIA, angle);
  Actor_SetAnimation(ACTOR_MIA, 9);
  // "Garet, you're only using one arm to hang on... What's the matter?"
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  FieldEvent_Wait(5);
  // "I don't know... I think I hurt my arm when I fell... It's totally..."
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 5);
  FieldActor_StartRepeatedMotion(ACTOR_MIA, 3);
  Actor_SetAttachedEffect(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  // "You shouldn't have tried to save me when I fell into that hole..."
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  FieldEvent_Wait(5);
  // "Oh, Mia, don't blame yourself... I wasn't going to leave you down there!"
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 5);
  Actor_Get(ACTOR_MIA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetSpeed(ACTOR_MIA, 0.1);
  Actor_SetDestinationOffset(ACTOR_MIA, 4, 2);
  FieldActor_WaitForMoveById(ACTOR_MIA);
  Actor_SetAnimation(ACTOR_MIA, 9);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Actor_SetDestinationOffset(ACTOR_MIA, -2, 2);
  FieldActor_WaitForMoveById(ACTOR_MIA);
  Actor_SetAnimation(ACTOR_MIA, 9);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Actor_SetDestinationOffset(ACTOR_MIA, 2, 1);
  FieldActor_WaitForMoveById(ACTOR_MIA);
  Actor_SetAnimation(ACTOR_MIA, 9);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Actor_SetDestinationOffset(ACTOR_MIA, -2, -1);
  FieldActor_WaitForMoveById(ACTOR_MIA);
  Actor_SetAnimation(ACTOR_MIA, 9);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  FieldEvent_Wait(40);
  Actor_SetDestinationOffset(ACTOR_MIA, 0, -2);
  FieldActor_WaitForMoveById(ACTOR_MIA);
  Actor_SetSpeed(ACTOR_MIA, 2);
  Actor_AnimateThenWait(ACTOR_MIA, ANIM_SHAKE_HEAD, 33);
  // "It's no use! I can't pull you up, Garet! I'm not strong enough!"
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 5);
  Actor_Get(ACTOR_MIA)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetDestinationOffset(ACTOR_MIA, -2, -8);
  FieldActor_WaitForMoveById(ACTOR_MIA);
  Actor_SetAnimation(ACTOR_MIA, ANIM_STAND);
  Actor_FaceDirectionAndWait(ACTOR_MIA, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  // "Isaac, please! You've got to save Garet!"
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 5);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_STAND);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_STAND);
  Actor_FaceEachOther(ACTOR_ISAAC, ACTOR_IVAN, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "This doesn't look good... We have to save Garet, and fast!"
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 5);
  Actor_AnimateThenWait(ACTOR_IVAN, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  Actor_SetScript(ACTOR_MIA, gJupiterLighthouse_MiaScript);
  Actor_SetPositionAndFacing(ACTOR_AGATIO, toFIXED(665), toFIXED(128), angle);
  Actor_SetPositionAndFacing(ACTOR_KARST, toFIXED(641), toFIXED(128), angle);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  // "No, I'm afraid you won't be doing that..."
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 5);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  FieldCamera_MoveTo(720, 0, 210, 1);
  Actor_SetSpeedFX(ACTOR_AGATIO, toFIXED(1) / 4, angle);
  Actor_SetSpeedFX(ACTOR_KARST, toFIXED(1) / 4, angle);
  Actor_WalkTo(ACTOR_AGATIO, 714, 193);
  Actor_WalkTo(ACTOR_KARST, 698, 209);
  Actor_SetScript(ACTOR_AGATIO, gJupiterLighthouse_AgatioStepsUpScript);
  Actor_SetScript(ACTOR_KARST, gJupiterLighthouse_KarstStepsUpScript);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Who are you!?"
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 5);
  Event_PlaySound(MUS_KARST_AND_AGATIO);
  Actor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Me? Why, I'm Karst."
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 5);
  Actor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "And I'm Agatio."
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 5);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "If you're not here to help my friend, then get out of my way, so I..."
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 5);
  FieldActor_WaitForScriptEndById(ACTOR_AGATIO);
  FieldActor_WaitForScriptEndById(ACTOR_KARST);
  Actor_AnimateThenWait(ACTOR_KARST, ANIM_SHAKE_HEAD, 33);
  // "Your friends will have to take care of themselves. You have a little..."
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 5);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "Debt? What are you talking about? We've never even seen you before!"
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 5);
  Actor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Regardless, you have done Karst here a great wrong, and you're..."
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 5);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceEachOther(ACTOR_ISAAC, ACTOR_IVAN, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHWEST);
  // "Wait a second... You two look familiar somehow... Do you know..."
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 5);
  Actor_AnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "So, you're not such a fool after all, Isaac! You are right. We are..."
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 5);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Prox? Never heard of it."
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  // "That doesn't surprise me. Our town hangs on the brink of extinction."
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_AGATIO, 2);
  FieldEvent_Wait(5);
  // "And the seal placed on Alchemy is responsible!"
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 5);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "What are you saying? That doesn't make any sense."
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_KARST, 2);
  FieldEvent_Wait(5);
  // "It matters not! For soon, Prox will recover its lost power..."
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, angle);
  FieldEvent_Wait(10);
  // "We shall bring Prox back from the edge, and then all the people of..."
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 5);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "You think we'd let you, after hearing all that nonsense?"
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 5);
  Actor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "See! You're a hindrance!"
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 5);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  // "And Felix was trying to help you?"
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 5);
  Actor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Actually...we don't know what Felix's objectives are."
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 5);
  Actor_AnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "And we don't care, as long as he lights the beacon on Jupiter..."
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  FieldEvent_Wait(5);
  // "Then get out of our way! We're here to stop Felix!"
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 5);
  Actor_SetAnimation(ACTOR_KARST, ANIM_SHAKE_HEAD);
  Actor_AnimateThenWait(ACTOR_AGATIO, ANIM_SHAKE_HEAD, 33);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Move, or we'll have to move you ourselves!"
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 5);
  Actor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "Did you hear that, Karst? They actually want to fight?"
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 5);
  Actor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "You mean to tell us that you would leave your friend...hanging?"
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 5);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, angle);
  Actor_SetAnimation(ACTOR_ISAAC, 38);
  FieldCamera_MoveTo(784, 0, 274, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(60);
  FieldCamera_MoveTo(720, 0, 210, 1);
  FieldCamera_WaitForArrival();
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_STAND);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  // "So this was all part of your plan?"
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 5);
  Actor_FaceEachOther(ACTOR_AGATIO, ACTOR_KARST, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  Actor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Cowards! Stop playing dirty and fight fair!"
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 5);
  Actor_FaceDirection(ACTOR_AGATIO, angle, 0);
  Actor_FaceDirectionAndWait(ACTOR_KARST, angle);
  Actor_AnimateThenWait(ACTOR_AGATIO, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "If you really are the brats who killed Saturos and Menardi, then..."
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 5);
  Actor_AnimateThenWait(ACTOR_KARST, ANIM_SHAKE_HEAD, 33);
  // "But answer me this: would you still cry foul if you were fighting us..."
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 5);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  // "That's why you were waiting up here? To set a trap and make Mia and..."
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 5);
  Actor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "We didn't plan on snaring both of them, though... That was a..."
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_ISAAC, 2);
  FieldEvent_Wait(5);
  // "So that's the deal, is it? You came up here to fight us?"
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 5);
  Actor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Oh, did you figure that out all on your own? Impressive! I'm quite..."
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_NORTHEAST);
  FieldEvent_Wait(10);
  // "I hope you don't think we're afraid of a fair fight, two on two..."
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 5);
  Actor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "A fair fight?"
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 5);
  Actor_AnimateThenWait(ACTOR_KARST, ANIM_SHAKE_HEAD, 33);
  // "I'm sorry if I misled you, but we have a third... A Water Adept... Alex!"
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_KARST, FACING_NORTHWEST);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_SetSpeed(ACTOR_KARST, 2);
  Actor_WalkByAndWait(ACTOR_KARST, -24, -24);
  FieldEvent_Wait(5);
  Actor_FaceDirectionAndWait(ACTOR_KARST, FACING_WEST);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_KARST, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_KARST, FACING_EAST);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_KARST, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Alex? ...Where is he?"
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 5);
  // "What's wrong, Karst?"
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_KARST, angle);
  FieldEvent_Wait(10);
  // "It's Alex! He's gone!"
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  // "What!?"
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 5);
  Actor_FaceEachOther(ACTOR_ISAAC, ACTOR_IVAN, 0);
  FieldEvent_Wait(10);
  // "Alex!" and "Alex?"
  FieldEvent_ShowTwoMessagesAndWait(ACTOR_ISAAC, 20, 7, 15, 6, ACTOR_IVAN, 17,
                                    13, 12, 12, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  // "Did anyone see where he went?"
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 5);
  Actor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Forget about him! Honestly, I was planning to do the same to him..."
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, angle);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "My only regret is that I won't be able to wipe that sneering smile..."
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 5);
  Actor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Actor_WalkBy(ACTOR_KARST, 24, 24);
  Actor_SetScript(ACTOR_KARST, gJupiterLighthouse_KarstTurnsScript);
  // "Well, take out all of your aggressions on these brats... You'll feel..."
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 5);
  FieldActor_WaitForScriptEndById(ACTOR_KARST);
  Actor_FaceEachOther(ACTOR_AGATIO, ACTOR_KARST, 30);
  Actor_SetAnimation(ACTOR_AGATIO, ANIM_NOD);
  Actor_AnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
  FieldEvent_Wait(30);
  Actor_SetSpeed(ACTOR_AGATIO, 3);
  Actor_SetSpeed(ACTOR_KARST, 3);
  Actor_SetSpeed(ACTOR_ISAAC, 3);
  Actor_SetSpeed(ACTOR_IVAN, 3);
  Actor_WalkTo(ACTOR_AGATIO, 737, 202);
  Actor_WalkTo(ACTOR_KARST, 731, 213);
  Actor_SetScript(ACTOR_KARST, gJupiterLighthouse_KarstFacesSouthScript);
  Actor_SetScript(ACTOR_AGATIO, gJupiterLighthouse_AgatioFacesEastScript);
  FieldEvent_Wait(5);
  Actor_WalkTo(ACTOR_ISAAC, 776, 202);
  Actor_WalkTo(ACTOR_IVAN, 731, 232);
  Actor_SetScript(ACTOR_ISAAC, gJupiterLighthouse_IsaacFacesWestScript);
  Actor_SetScript(ACTOR_IVAN, gJupiterLighthouse_IvanFacesNorthScript);
  FieldActor_WaitForScriptEndById(ACTOR_ISAAC);
  FieldActor_WaitForScriptEndById(ACTOR_IVAN);
  FieldActor_WaitForScriptEndById(ACTOR_AGATIO);
  FieldActor_WaitForScriptEndById(ACTOR_KARST);
  FieldEvent_Wait(40);
  JupiterLighthouse_StartTheFight();
  FieldEvent_Wait(200);
  EventCamera_FollowActor(ACTOR_FELIX, 1);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(5);
  // "Felix! Agatio and Karst are attacking Isaac and the others!"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "Do you think Isaac and Ivan can handle them without Mia and Garet's..."
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 5);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "I'm disgusted that Karst would set a trap for them. It's dishonorable!"
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 5);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "What if Garet loses his grip while they're still fighting?"
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH);
  FieldEvent_Wait(10);
  // "There's still time! We have to help Isaac!"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_AnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  Actor_FaceActor(ACTOR_JENNA, ACTOR_FELIX, 0);
  Actor_FaceActor(ACTOR_PIERS, ACTOR_FELIX, 0);
  Actor_FaceActor(ACTOR_SHEBA, ACTOR_FELIX, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_AnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_WALK);
  actor = Actor_Get(ACTOR_FELIX);
  if (actor != NULL)
    Actor_SetDestination(ACTOR_PIERS, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_WALK);
  actor = Actor_Get(ACTOR_FELIX);
  if (actor != NULL)
    Actor_SetDestination(ACTOR_SHEBA, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_WALK);
  actor = Actor_Get(ACTOR_FELIX);
  if (actor != NULL)
    Actor_SetDestination(ACTOR_JENNA, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  actor = Actor_Get(ACTOR_FELIX);
  if (actor != NULL)
    Actor_SetDestination(ACTOR_KRADEN, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  Actor_SetScript(ACTOR_PIERS, gJupiterLighthouse_PiersStepsBackScript);
  Actor_SetScript(ACTOR_SHEBA, gJupiterLighthouse_PiersStepsBackScript);
  Actor_SetScript(ACTOR_JENNA, gJupiterLighthouse_PiersStepsBackScript);
  Actor_SetScript(ACTOR_KRADEN, gJupiterLighthouse_PiersStepsBackScript);
  FieldActor_WaitForScriptEndById(ACTOR_PIERS);
  FieldActor_WaitForScriptEndById(ACTOR_SHEBA);
  FieldActor_WaitForScriptEndById(ACTOR_JENNA);
  FieldActor_WaitForScriptEndById(ACTOR_KRADEN);
  Event_End();
}

/// Unit_AddPp as an event calls it, beside system_call.h's FieldUnit_AddHp.
static inline s32 FieldUnit_AddPp(s32 owner, s32 delta) {
  return Unit_AddPp(owner, delta);
}

/// The fight goes against Isaac and Ivan: Ivan can take no more, Agatio mocks
/// Isaac's concern for him, and the battle that cannot be won begins, the two
/// the whole party, Ivan with no HP, Isaac with ISAAC_HP_IN_THE_FIGHT, and
/// neither with PP.
void JupiterLighthouse_IsaacAndIvanAreHurt(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  PLACE(ACTOR_ISAAC, 776, 202, FACING_WEST);
  PLACE(ACTOR_IVAN, 731, 232, FACING_NORTHWEST);
  PLACE(ACTOR_KARST, 731, 213, FACING_SOUTH);
  PLACE(ACTOR_AGATIO, 737, 202, FACING_EAST);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_STAND);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_STAND);
  Actor_SetAnimation(ACTOR_AGATIO, ANIM_STAND);
  Actor_SetAnimation(ACTOR_KARST, ANIM_STAND);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetMessageId(MsgIMSorry);
  // "Isaac... I'm sorry... I can't take any more..."
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 5);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  FieldCamera_MoveTo(705, 0, 177, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(30);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_IVAN), 0);
  Actor_SetAnimation(ACTOR_IVAN, 32);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTHWEST);
  // "Keep it together, Ivan!"
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 5);
  Actor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Ha! You're a fool! Never let concern distract you in battle!"
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 5);
  Actor_WalkBy(ACTOR_AGATIO, 24, 0);
  FieldEvent_Wait(20);
  Battle_SetType(BATTLE_TYPE_LOST);
  FieldEvent_SetAfterBattleDestination(SCENE_JUPITER_LIGHTHOUSE_ROOM_M,
                                       ENTRANCE_AFTER_THE_FIGHT);
  Event_SetDefeatDestination(SCENE_JUPITER_LIGHTHOUSE_ROOM_M,
                             ENTRANCE_AFTER_THE_FIGHT);
  Party_RemoveMember(FELIX);
  Party_RemoveMember(PIERS);
  Party_RemoveMember(JENNA);
  Party_RemoveMember(SHEBA);
  Party_RemoveMember(IVAN);
  Party_RemoveMember(ISAAC);
  Party_AddMember(IVAN);
  Party_AddMember(ISAAC);
  Party_RemoveMember(MIA);
  Party_RemoveMember(GARET);
  FieldUnit_AddHp(IVAN, -ALL_HP_OR_PP);
  FieldUnit_AddHp(ISAAC, -ALL_HP_OR_PP);
  Unit_Calculate(ISAAC);
  Unit_Calculate(IVAN);
  FieldUnit_AddHp(ISAAC, ISAAC_HP_IN_THE_FIGHT);
  FieldUnit_AddPp(IVAN, -ALL_HP_OR_PP);
  Unit_AddPp(ISAAC, -ALL_HP_OR_PP);
  gMainRoutineWork.leaderActorId = ACTOR_ISAAC;
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 4);
}

/// Places Isaac and Ivan lying hurt after the fight, with Jenna, Sheba and
/// Kraden beside them.
void JupiterLighthouse_PlaceTheHurt(void) {
  PLACE(ACTOR_ISAAC, 761, 219, FACING_NORTHWEST);
  PLACE(ACTOR_IVAN, 741, 232, FACING_NORTHWEST);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ISAAC), 0);
  Actor_SetAnimation(ACTOR_ISAAC, 39);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_IVAN), 0);
  Actor_SetAnimation(ACTOR_IVAN, 32);
  PLACE(ACTOR_JENNA, 761, 203, FACING_SOUTH);
  PLACE(ACTOR_SHEBA, 725, 232, FACING_EAST);
  PLACE(ACTOR_KRADEN, 729, 203, FACING_SOUTHEAST);
  Actor_SetScript(ACTOR_MIA, gJupiterLighthouse_MiaScript);
}

s32 Inventory_Equip();

/// After the battle with Karst and Agatio: the two stand over the fallen Isaac
/// and Ivan until Felix and the others step in, Felix promises to light the
/// beacon, and the Mythril Bag holding the Jupiter Star is swapped for a
/// plain one (ITEM_MYTHRIL_BAG_B) that Felix equips.
void JupiterLighthouse_IsaacAndIvanFall(void) {
  s32 index;
  s32 answer;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(737, 0, 209, 0);
  gMainRoutineWork.leaderActorId = ACTOR_FELIX;
  Party_RemoveMember(ISAAC);
  Party_RemoveMember(IVAN);
  Party_RemoveMember(JENNA);
  Party_RemoveMember(SHEBA);
  Party_RemoveMember(PIERS);
  Party_RemoveMember(MIA);
  Party_RemoveMember(GARET);
  Party_AddMember(FELIX);
  Actor_SetSpeed(ACTOR_FELIX, 1);
  {
    Actor_SetPositionAndFacing(ACTOR_FELIX, toFIXED(664), toFIXED(88),
                               FACING_SOUTH);
  }
  {
    Actor_SetPositionAndFacing(ACTOR_KARST, toFIXED(717), toFIXED(208),
                               FACING_SOUTHEAST);
  }
  Actor_SetPositionAndFacing(ACTOR_AGATIO, toFIXED(737), toFIXED(195),
                             FACING_SOUTHEAST);
  {
    Actor_SetPositionAndFacing(ACTOR_ISAAC, toFIXED(761), toFIXED(219),
                               FACING_NORTHWEST);
  }
  Actor_SetPositionAndFacing(ACTOR_IVAN, toFIXED(741), toFIXED(232),
                             FACING_NORTHWEST);
  Actor_SetScript(ACTOR_MIA, (s32)gJupiterLighthouse_MiaScript);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ISAAC), 0);
  Actor_SetAnimation(ACTOR_ISAAC, 39);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_IVAN), 0);
  Actor_SetAnimation(ACTOR_IVAN, 32);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetMessageId((s32)MsgWeDidItAgatio);
  // "We did it, Agatio!"
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 5);
  Actor_SetAnimation(ACTOR_AGATIO, 8);
  FieldEvent_Wait(40);
  // "It was close, but we did it."
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_KARST, FACING_EAST);
  FieldEvent_Wait(10);
  // "Heh... He's no warrior. His concern for his friend gave us the..."
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 5);
  FieldActor_StartRepeatedMotion(ACTOR_ISAAC, 2);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  FieldEvent_Wait(5);
  FieldActor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "What's this? He's still alive!? Even after that blow?"
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_KARST, FACING_NORTHEAST);
  FieldEvent_Wait(10);
  // "It would be a shame if you and I ever had to regret letting him live..."
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 5);
  Actor_SetAnimation(ACTOR_AGATIO, ANIM_STAND);
  FieldEvent_Wait(30);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, FACING_SOUTH_SOUTHWEST);
  FieldEvent_Wait(20);
  Actor_AnimateThenWait(ACTOR_AGATIO, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  EventCamera_GetActor()->motionFlags = 0;
  EventCamera_FollowActor(ACTOR_FELIX, 1);
  FieldEvent_Wait(20);
  FieldActor_LaunchFromLeader(ACTOR_SHEBA, 0, 0, FACING_SOUTH);
  EventCamera_FollowActor(ACTOR_SHEBA, 1);
  FieldActor_LaunchFromLeader(ACTOR_PIERS, 16, 0, FACING_SOUTH);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, -16, 24, FACING_SOUTHEAST);
  FieldActor_LaunchFromLeader(ACTOR_JENNA, -16, 0, FACING_SOUTHEAST);
  Actor_WalkByAndWait(ACTOR_FELIX, 16, 24);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "Isaac! Watch out!"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  // "He's still alive! If we're going to act, Felix, we have to act now!"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 35);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  Actor_WalkTo(ACTOR_FELIX, 683, 186);
  Actor_WalkTo(ACTOR_KRADEN, 667, 202);
  Actor_SetScript(ACTOR_FELIX, (s32)gJupiterLighthouse_FelixWaitsScript);
  Actor_SetScript(ACTOR_KRADEN, (s32)gJupiterLighthouse_KradenWaitsScript);
  Actor_FaceDirection(ACTOR_AGATIO, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_KARST, FACING_WEST, 0);
  FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "They're going to kill Isaac!"
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 5);
  Actor_AnimateThenWait(ACTOR_JENNA, ANIM_SHAKE_HEAD, 33);
  // "No, they aren't! I'm not about to let that happen!"
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 5);
  Actor_AnimateThenWait(ACTOR_PIERS, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  Actor_FaceActor(ACTOR_JENNA, ACTOR_PIERS, 0);
  Actor_FaceActor(ACTOR_SHEBA, ACTOR_PIERS, 0);
  Actor_FaceActor(ACTOR_PIERS, ACTOR_JENNA, 0);
  // "None of us are, Jenna!"
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 5);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_AnimateThenWait(ACTOR_PIERS, ANIM_NOD, 20);
  FieldEvent_Wait(20);
  Actor_WalkBy(ACTOR_PIERS, 0, 40);
  Actor_WalkBy(ACTOR_SHEBA, 0, 40);
  Actor_WalkBy(ACTOR_JENNA, 0, 40);
  Actor_SetScript(ACTOR_PIERS, (s32)gJupiterLighthouse_PiersStepsUpScript);
  Actor_SetScript(ACTOR_SHEBA, (s32)gJupiterLighthouse_ShebaStepsUpScript);
  Actor_SetScript(ACTOR_JENNA, (s32)gJupiterLighthouse_JennaStepsUpScript);
  EventCamera_FollowActor(ACTOR_FELIX, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Felix? What's going on?"
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 5);
  FieldActor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Be grateful! We just did you a favor!"
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, FACING_WEST);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, FACING_SOUTH);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, FACING_WEST);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, FACING_WEST);
  // "I understand that when Venus was lit, the ground shook so fiercely..."
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 5);
  Actor_AnimateThenWait(ACTOR_KARST, ANIM_SHAKE_HEAD, 33);
  // "You haven't lit the beacon yet, have you? And after all we've done..."
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(5);
  // "We're going to light the beacon right now..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  // "If that's true, then you'd better go do it!"
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 5);
  Actor_AnimateThenWait(ACTOR_SHEBA, ANIM_SHAKE_HEAD, 33);
  // "But we're not leaving Isaac behind."
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 5);
  Actor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Oh, great... Are you going to betray us now?" Yes: "Typical. And you're
  // going to try to stop us..." No: "Don't lie... We can see it in your eyes."
  Event_OpenMessage(ACTOR_KARST, 0);
  Actor_FaceActor(ACTOR_KRADEN, ACTOR_FELIX, 0);
  Actor_FaceActor(ACTOR_JENNA, ACTOR_FELIX, 0);
  Actor_FaceActor(ACTOR_SHEBA, ACTOR_FELIX, 0);
  Actor_FaceActor(ACTOR_PIERS, ACTOR_FELIX, 0);
  FieldEvent_Wait(10);
  answer = FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0);
  if (answer == 0) {
    Actor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    // "Typical. And you're going to try to stop us from finishing them off..."
    FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 5);
    gFieldWork->message++;
  } else {
    Actor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    gFieldWork->message++;
    // "Don't lie... We can see it in your eyes. We can see what you truly..."
    FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 5);
  }
  Actor_FaceActor(ACTOR_PIERS, ACTOR_AGATIO, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "If you're only here to clear the way for us to light the beacon..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 5);
  Actor_AnimateThenWait(ACTOR_AGATIO, ANIM_SHAKE_HEAD, 33);
  // "Isaac has proven to be a greater foe than we'd imagined. We can't..."
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 5);
  Actor_AnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "Consider how powerful these two were without the aid of a Water..."
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 5);
  Actor_AnimateThenWait(ACTOR_JENNA, ANIM_SHAKE_HEAD, 33);
  // "If you want them, you're going to have to face us first!"
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 5);
  Actor_FaceEachOther(ACTOR_AGATIO, ACTOR_KARST, 0);
  FieldActor_WaitForScriptEndById(ACTOR_AGATIO);
  FieldActor_WaitForScriptEndById(ACTOR_KARST);
  FieldActor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  Actor_FaceActor(ACTOR_KRADEN, ACTOR_KARST, 0);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD, 33);
  Actor_FaceActor(ACTOR_AGATIO, ACTOR_KRADEN, 0);
  Actor_FaceActor(ACTOR_KARST, ACTOR_KRADEN, 0);
  // "You're both quite badly injured... You're in no shape to fight all..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  FieldEvent_Wait(60);
  Actor_AnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "We haven't much choice..."
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_AGATIO, 2);
  FieldEvent_Wait(5);
  // "Karst, no!"
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 5);
  Actor_AnimateThenWait(ACTOR_KARST, ANIM_SHAKE_HEAD, 33);
  // "Agatio, if we fight Felix now...we'll be destroyed."
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 5);
  FieldActor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Isaac killed your sister! Where is your anger!?"
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 5);
  FieldActor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "I am filled with rage, but not so much as to be blinded by it."
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 5);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_AGATIO, 2);
  FieldEvent_Wait(5);
  // "What, then? Do we give up?"
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, FACING_WEST);
  FieldEvent_Wait(10);
  Actor_AnimateThenWait(ACTOR_AGATIO, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "Fine! But this is not the end."
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_KARST, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  // "Do with Isaac as you will."
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 5);
  Actor_FaceActor(ACTOR_PIERS, ACTOR_FELIX, 0);
  Actor_FaceEachOther(ACTOR_SHEBA, ACTOR_JENNA, 0);
  Actor_FaceEachOther(ACTOR_FELIX, ACTOR_KRADEN, 0);
  FieldActor_WaitForScriptEndById(ACTOR_FELIX);
  FieldActor_WaitForScriptEndById(ACTOR_KRADEN);
  FieldActor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  FieldActor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Actor_FaceActor(ACTOR_FELIX, ACTOR_AGATIO, 0);
  Actor_FaceActor(ACTOR_JENNA, ACTOR_AGATIO, 0);
  Actor_FaceActor(ACTOR_SHEBA, ACTOR_AGATIO, 0);
  Actor_FaceActor(ACTOR_PIERS, ACTOR_AGATIO, 0);
  Actor_FaceActor(ACTOR_KRADEN, ACTOR_AGATIO, 0);
  Actor_AnimateThenWait(ACTOR_AGATIO, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "But swear this oath to us: if we leave now, you will light the beacon's
  // fire." Yes: "I don't care if you want to help Garet or not!..." No: "We
  // do not have time to wait for you to save Garet!"
  Event_OpenMessage(ACTOR_AGATIO, 0);
  answer = FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0);
  if (answer == 0) {
    Actor_AnimateThenWait(ACTOR_AGATIO, ANIM_SHAKE_HEAD, 33);
    // "I don't care if you want to help Garet or not! The lighthouse comes..."
    FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 5);
    gFieldWork->message++;
  } else {
    Actor_AnimateThenWait(ACTOR_AGATIO, ANIM_SHAKE_HEAD, 33);
    gFieldWork->message++;
    // "We do not have time to wait for you to save Garet!"
    FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 5);
  }
  Actor_FaceEachOther(ACTOR_KRADEN, ACTOR_FELIX, 0);
  FieldActor_WaitForScriptEndById(ACTOR_KRADEN);
  FieldEvent_Wait(10);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "Go, Felix. I will tend to Garet."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_FaceEachOther(ACTOR_FELIX, ACTOR_KRADEN, 0);
  FieldActor_WaitForScriptEndById(ACTOR_FELIX);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(40);
  Actor_AnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
  FieldEvent_Wait(30);
  Actor_FaceEachOther(ACTOR_AGATIO, ACTOR_KARST, 0);
  FieldActor_WaitForScriptEndById(ACTOR_AGATIO);
  FieldActor_WaitForScriptEndById(ACTOR_KARST);
  FieldActor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  FieldActor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Actor_FaceActor(ACTOR_AGATIO, ACTOR_FELIX, 0);
  Actor_FaceActor(ACTOR_FELIX, ACTOR_KARST, 0);
  Actor_WalkToAndWait(ACTOR_KARST, 699, 202);
  Actor_AnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "And be sure to bring with you the Mars Star that Isaac carries on him."
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 5);
  FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  // "How did you know about the Mars Star?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Did you think this was about nothing more than revenge? Alex told us..."
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 5);
  Actor_FaceEachOther(ACTOR_FELIX, ACTOR_PIERS, 0);
  Actor_FaceEachOther(ACTOR_SHEBA, ACTOR_JENNA, 0);
  FieldActor_WaitForScriptEndById(ACTOR_SHEBA);
  FieldActor_WaitForScriptEndById(ACTOR_JENNA);
  FieldEvent_Wait(40);
  Actor_FaceActor(ACTOR_FELIX, ACTOR_AGATIO, 0);
  Actor_FaceActor(ACTOR_PIERS, ACTOR_AGATIO, 0);
  Actor_FaceActor(ACTOR_SHEBA, ACTOR_AGATIO, 0);
  Actor_FaceActor(ACTOR_JENNA, ACTOR_AGATIO, 0);
  Actor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  Actor_FaceDirection(ACTOR_KARST, FACING_EAST, 0);
  // "Take the Mars Star, or we'll be forced to make you!"
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 5);
  Actor_SetSpeed(ACTOR_PIERS, 4);
  Actor_SetSpeed(ACTOR_KARST, 2);
  Actor_WalkBy(ACTOR_PIERS, 8, 24);
  Actor_WalkByAndWait(ACTOR_KARST, 8, 4);
  FieldActor_WaitForMoveById(ACTOR_PIERS);
  FieldEvent_Wait(5);
  Actor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  Actor_FaceActor(ACTOR_AGATIO, ACTOR_PIERS, 0);
  Actor_FaceActor(ACTOR_KARST, ACTOR_PIERS, 0);
  // "Do not force our hand on this!"
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_ISAAC, 2);
  FieldEvent_Wait(5);
  // "Go on, Felix. Take it."
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 5);
  Actor_FaceActor(ACTOR_AGATIO, ACTOR_ISAAC, 0);
  Actor_FaceActor(ACTOR_KARST, ACTOR_ISAAC, 0);
  Actor_RunRepeatedMotion(ACTOR_ISAAC, 2);
  FieldEvent_Wait(5);
  // "I don't know why you're doing this, but I trust you... Take the Mars..."
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 5);
  FieldEvent_Wait(40);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTHEAST);
  FieldEvent_Wait(20);
  Actor_AnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  FieldActor_WatchAndSetScript(ACTOR_AGATIO, ACTOR_FELIX);
  FieldActor_WatchAndSetScript(ACTOR_KARST, ACTOR_FELIX);
  Actor_WalkByAndWait(ACTOR_FELIX, 5, 32);
  Actor_WalkByAndWait(ACTOR_FELIX, 56, -2);
  FieldEvent_Wait(30);
  FieldActor_StartRepeatedMotion(ACTOR_ISAAC, 2);
  FieldEvent_Wait(4);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 2);
  PartyInventory_Remove(ITEM_MYTHRIL_BAG_JUPITER_STAR);
  PartyInventory_Add(ITEM_MYTHRIL_BAG_B);
  // "Felix took the Mars Star."
  Message_ShowCentered((s16)gFieldWork->message++, 5);
  index = 0;
  for (index = 0; index <= INVENTORY_SLOTS - 1; index++) {
    if ((Unit_Get(FELIX)->inventory[index] & INVENTORY_ITEM_ID_MASK) ==
        ITEM_MYTHRIL_BAG_B) {
      Inventory_Equip(FELIX, index);
      break;
    }
  }
  Actor_AnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "That's better, Felix. We'll be waiting for you on the aerie."
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 5);
  Actor_FaceActor(ACTOR_FELIX, ACTOR_KARST, 0);
  FieldEvent_Wait(20);
  Actor_AnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_PIERS, 2);
  Actor_SetSpeed(ACTOR_AGATIO, 0.5);
  Actor_SetSpeed(ACTOR_KARST, 0.5);
  Actor_SetSpeed(ACTOR_FELIX, 0.5);
  Actor_SetScript(ACTOR_AGATIO, (s32)gJupiterLighthouse_WalkToAerieScript);
  FieldEvent_Wait(40);
  Actor_SetDestinationOffset(ACTOR_PIERS, -16, 0);
  FieldEvent_Wait(10);
  Actor_SetScript(ACTOR_KARST, (s32)gJupiterLighthouse_WalkToAerieScript);
  FieldEvent_Wait(110);
  Actor_SetScript(ACTOR_FELIX, (s32)gJupiterLighthouse_WalkToAerieScript);
  FieldActor_WaitForScriptEndById(ACTOR_AGATIO);
  Actor_SetScript(ACTOR_AGATIO, (s32)gJupiterLighthouse_LeaveForAerieScript);
  FieldActor_WaitForScriptEndById(ACTOR_KARST);
  Actor_SetScript(ACTOR_KARST, (s32)gJupiterLighthouse_LeaveForAerieScript);
  FieldActor_WaitForScriptEndById(ACTOR_FELIX);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_STAND);
  FieldActor_StartRepeatedMotion(ACTOR_PIERS, 2);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  // "Felix, wait..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 5);
  EventCamera_FollowActor(ACTOR_SHEBA, 1);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "We cannot trust them. Take me with you..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 5);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  // "Good thinking. If something happens up there, you'll be safer with..."
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
  FieldEvent_Wait(10);
  Actor_AnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  // "We'll be fine. Take Piers with you..."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
  FieldEvent_Wait(10);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "Listen to the others... Safety in numbers, as they say..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_AnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  Actor_SetSpeed(ACTOR_PIERS, 1);
  Actor_SetScript(ACTOR_PIERS, (s32)gJupiterLighthouse_WalkToAerieScript);
  FieldActor_WaitForScriptEndById(ACTOR_PIERS);
  Party_AddMember(PIERS);
  EventCamera_FollowActor(ACTOR_FELIX, 1);
  Actor_SetPosition(ACTOR_PIERS, 0, 0);
  FieldEvent_Wait(20);
#if TLA_PATCH_1
  Party_ClearUnusableShortcuts();
#endif
  Actor_WalkToAndWait(ACTOR_FELIX, 664, 70);
  Event_End();
}

/// Sets up the terraces: the eight hanging chains (the first placed actors)
/// and the rope, the trapdoor as it was left open, and once the lid is
/// off the reunion as far as it has gone: the fight to watch, the fight
/// itself, Isaac and Ivan fallen, or the hurt left behind once they have gone
/// up to the aerie.
s32 Scene_Initialize(void) {
  struct Actor *actor;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  HangingVine_Place(ACTOR_FIRST_PLACED, 1);
  HangingVine_Place(ACTOR_FIRST_PLACED + 1, 1);
  HangingVine_Place(ACTOR_FIRST_PLACED + 2, 1);
  HangingVine_Place(ACTOR_FIRST_PLACED + 3, 1);
  HangingVine_Place(ACTOR_FIRST_PLACED + 4, 1);
  HangingVine_Place(ACTOR_FIRST_PLACED + 5, 1);
  HangingVine_Place(ACTOR_FIRST_PLACED + 6, 1);
  HangingVine_Place(ACTOR_FIRST_PLACED + 7, 1);
  Rope_Initialize();
  Rope_Place(0, FLAG_JUPITER_LIGHTHOUSE_ROPE_TIED, ACTOR_EAST_ROPE_POST,
             ACTOR_WEST_ROPE_POST);
  gWhirlwindRiding = 0;
  actor = Actor_Get(ACTOR_WEST_UPPER_GUST);
  actor->motionFlags = 0;
  actor->y.fixed = 0;
  actor->groundHeight = 0;
  actor = Actor_Get(ACTOR_EAST_UPPER_GUST);
  actor->motionFlags = 0;
  actor->y.fixed = toFIXED(32);
  actor->groundHeight = toFIXED(32);
  if (Event_GetFlag(FLAG_SCENE_TEMP_02)) {
    MapGrid_CopyCellsLayered(49, 106, 4, 3, 49, 76);
    MapGrid_CopyCellAttributes(49, 45, 4, 1, 49, 44);
  }
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_LID_LIFTED)) {
    MapGrid_CopyCellsLayered(49, 106, 4, 3, 49, 76);
    MapGrid_CopyCellAttributes(49, 45, 4, 1, 49, 44);
    Actor_SetPosition(ACTOR_GRAY_BLOCK, 0, 0);
    if (Event_GetFlag(FLAG_ISAAC_AND_IVAN_FELL)) {
      if (!Event_GetFlag(FLAG_JUPITER_BEACON_LIT)) {
        JupiterLighthouse_PlaceForTheFight();
        JupiterLighthouse_PlaceTheHurt();
      }
    } else if (!Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_FIGHT_WATCHED)) {
      if (gMainRoutineWork.entrance ==
          JUPITER_LIGHTHOUSE_ROOM_M_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST) {
        JupiterLighthouse_PlaceForTheFight();
        Event_SetFlag(FLAG_JUPITER_LIGHTHOUSE_FIGHT_WATCHED);
        JupiterLighthouse_PartyArrivesAtTheFight(
            gMainRoutineWork.entrance ==
            JUPITER_LIGHTHOUSE_ROOM_M_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST);
      }
    } else {
      JupiterLighthouse_PlaceForTheFight();
      if (gMainRoutineWork.entrance ==
          JUPITER_LIGHTHOUSE_ROOM_M_FROM_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_WEST) {
        JupiterLighthouse_IsaacAndIvanAreHurt();
      } else if (gMainRoutineWork.entrance == ENTRANCE_AFTER_THE_FIGHT) {
        JupiterLighthouse_IsaacAndIvanFall();
        Event_SetFlag(FLAG_ISAAC_AND_IVAN_FELL);
        Event_ClearFlag(FLAG_JUPITER_LIGHTHOUSE_FIGHT_WATCHED);
      } else {
        JupiterLighthouse_StartTheFight();
      }
    }
  }
  Actor_Get(ACTOR_GRAY_BLOCK)->motionFlags = 0;
  if (Event_GetFlag(FLAG_GRAY_BLOCK_DROPPED))
    Actor_SetPosition(ACTOR_GRAY_BLOCK, 0, 0);
  if (gMainRoutineWork.entrance == ENTRANCE_ISAAC_AND_IVAN_FALL)
    JupiterLighthouse_IsaacAndIvanFall();
  return 0;
}

void LightSprites_InitC();
void LightSprites_Add();

/// Loads the lighthouse's palettes and the energy block's light; once the block
/// is in its gap, sets it there with the lit path and the Hover pad beyond.
s32 Scene_PrepareMap(void) {
  struct Actor *actor;

  Scene_SetArrivalFlags(0);
  JupiterLighthouse_LoadPalettes(1);
  LightSprites_InitC();
  LightSprites_Add(ACTOR_ENERGY_BLOCK, 0);
  actor = Actor_Get(ACTOR_ENERGY_BLOCK);
  if (Event_GetFlag(FLAG_ENERGY_BLOCK_IN_GAP)) {
    Actor_SetPosition(ACTOR_ENERGY_BLOCK, 1016, 248);
    MapGrid_CopyCellsLayered(104, 42, 5, 11, 61, 42);
    MapGrid_CopyCellsLayered(106, 52, 1, 1, 63, 84);
    FieldMapGrid_SetCellBehavior(toFIXED(1040), toFIXED(160), 2,
                                 CELL_BEHAVIOR_HOVER);
  }
  actor->motionFlags = 0;
  Call4(MapGrid_SetCellHeight, actor->x.fixed, actor->z.fixed, 0, -6);
  return 0;
}

#include "shared/scene_set_arrival_flags.c"

extern s32 gPaletteCyclePausesInEvents;

#include "shared/palette_cycle_set_pauses_in_events.h"

extern s32 gPaletteCycleFrame;

/// Sets the palette cycle's frame counter.
static inline void PaletteCycle_SetFrame(s32 value) {
  gPaletteCycleFrame = value;
}

#include "shared/palette_cycle_step.h"

#include "shared/jupiter_lighthouse_load_palettes.h"

#include "shared/actor_raise_from_cell.h"

#include "shared/whirlwind_carry_leader.h"

#include "shared/position_flat_distance.h"

extern s32 gWhirlwindEndScript[];

void Actor_StartScript(struct Actor *actor, const s32 *script);

#include "shared/whirlwind_update.h"

void Actor_SetSpriteFlags(struct Actor *actor, u32 value);
void Whirlwind_Update(struct Actor *self);
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/whirlwind_spawn.h"

#include "shared/actor_emit_rising_sparks.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

/// Queues the lit actors' sprites: for each listed actor with a position,
/// projects it from the map's view, skips it off screen, and writes a 16 by 16
/// sprite, or a 32 by 64 one when the big switch is set, at its animation
/// picture and depth.
#include "shared/light_sprites_draw.h"

static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add.h"

void LightSprites_SetPriority(s32 value) { gLightSpritePriority = value; }

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#undef WORD_AT
#undef PLACE
extern const s32 *gFieldEffect_DurationScripts[];

void Actor_SetSpriteFlags(struct Actor *actor, u32 value);

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#include "../field/field_effect_spawn.c"

/// The cells CrackedTile_Step watches, and the cell it copies over the one the
/// leader steps on.
#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map_cell_sprite.c"

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_cell.h"

#include "shared/cracked_tile_crack.h"

struct CellTargetEntry;

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/cracked_tile_drop_in.h"

void GameFlag_SetByte(s32 flag, s32 value);

#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"

// clang-format off
u32 gJupiterLighthouse_AgatioStepsUpScript[] = {
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

u32 gJupiterLighthouse_KarstStepsUpScript[] = {
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

u32 gJupiterLighthouse_KarstTurnsScript[] = {
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

u32 gJupiterLighthouse_KarstFacesSouthScript[] = {
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_END,
};

u32 gJupiterLighthouse_AgatioFacesEastScript[] = {
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_END,
};

u32 gJupiterLighthouse_IsaacFacesWestScript[] = {
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_END,
};

u32 gJupiterLighthouse_IvanFacesNorthScript[] = {
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_END,
};

u32 gJupiterLighthouse_FelixWaitsScript[] = {
    SC_SYNC,
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_ANIMATION(1),
    SC_END,
};

u32 gJupiterLighthouse_KradenWaitsScript[] = {
    SC_SYNC,
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_ANIMATION(1),
    SC_END,
};

u32 gJupiterLighthouse_PiersStepsUpScript[] = {                                                               
      SC_SYNC,    SC_MOVE_XZ(699, 170), SC_SYNC,                
      SC_WAIT(1), SC_SET(SW_FACING, FACING_SOUTHEAST), SC_ANIMATION(1),              
      SC_END,                                                                  
  };

u32 gJupiterLighthouse_ShebaStepsUpScript[] = {                                                               
      SC_SYNC,    SC_MOVE_XZ(679, 170), SC_SYNC,                
      SC_WAIT(1), SC_SET(SW_FACING, FACING_SOUTHEAST), SC_ANIMATION(1),              
      SC_END,                                                                  
  };

u32 gJupiterLighthouse_JennaStepsUpScript[] = {                                                               
      SC_SYNC,    SC_MOVE_XZ(659, 170), SC_SYNC,                
      SC_WAIT(1), SC_SET(SW_FACING, FACING_SOUTHEAST), SC_ANIMATION(1),              
      SC_END,                                                                  
  };
// clang-format on

extern u32 gFieldEffect_LongScript[];
extern s32 gJupiterLighthouse_ShowGustScript[];

// clang-format off
#define FIELD_EFFECT_DURATION_SCRIPT_SECTION LATE_DATA
#include "shared/field_effect_duration_scripts.c"

LATE_DATA s32 gJupiterLighthouse_ShowGustScript[] = {
    SC_CALL(JupiterLighthouse_ClearScriptedActorSpriteFlags),
    SC_END,
};

#define SCENE_ENTRANCES_SECTION LATE_DATA
#include "shared/scene_entrances_default.c"

LATE_DATA u32 gSceneExits[] = {                                                        
      SCENE_EXITS(JupiterLighthouseRoomM),         SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_M_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH, JupiterLighthouseRoomErinyesTunicChest, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_FROM_JUPITER_LIGHTHOUSE_ROOM_M_NORTH),    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_M_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST, JupiterLighthouseRoomErinyesTunicChest, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_FROM_JUPITER_LIGHTHOUSE_ROOM_M_SOUTHWEST), 
      SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_M_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHEAST, JupiterLighthouseRoomErinyesTunicChest, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_FROM_JUPITER_LIGHTHOUSE_ROOM_M_SOUTHEAST),  SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_M_TO_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_WEST, JupiterLighthouseRoomRedKey, JUPITER_LIGHTHOUSE_ROOM_RED_KEY_FROM_JUPITER_LIGHTHOUSE_ROOM_M_WEST),    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_M_TO_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_EAST, JupiterLighthouseRoomRedKey, JUPITER_LIGHTHOUSE_ROOM_RED_KEY_FROM_JUPITER_LIGHTHOUSE_ROOM_M_EAST),  
      SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_M_TO_JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY, JupiterLighthouseRoomBlueKey, JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_FROM_JUPITER_LIGHTHOUSE_ROOM_M),   SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_M_TO_JUPITER_LIGHTHOUSE_WHORL, JupiterLighthouseWhorl, JUPITER_LIGHTHOUSE_WHORL_FROM_JUPITER_LIGHTHOUSE_ROOM_M), SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_M_TO_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_WEST, JupiterLighthouseRoomLockedDoors, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_FROM_JUPITER_LIGHTHOUSE_ROOM_M_WEST), 
      SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_M_TO_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_EAST, JupiterLighthouseRoomLockedDoors, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_FROM_JUPITER_LIGHTHOUSE_ROOM_M_EAST), SCENE_EXITS_END,                                
  };

LATE_DATA struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 24, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 840, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 888, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1464, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1608, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, (s32)gJupiterLighthouse_ShowGustScript, 648, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, (s32)gJupiterLighthouse_ShowGustScript, 680, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, (s32)gJupiterLighthouse_ShowGustScript, 936, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, (s32)gJupiterLighthouse_ShowGustScript, 952, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_BLOCK, CONDITION_ALWAYS, (s32)gJupiterLighthouse_ShowGustScript, 968, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (s32)gJupiterLighthouse_ShowGustScript, 1144, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (s32)gJupiterLighthouse_ShowGustScript, 1064, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 200, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, (s32)gJupiterLighthouse_ShowGustScript, 200, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, (s32)gJupiterLighthouse_ShowGustScript, 1016, 0, 168, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ISAAC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FELIX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_AGATIO, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KARST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, (s32)gJupiterLighthouse_ShowGustScript, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
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
    {TRIGGER_STEP, CELL_BEHAVIOR_WEST_WHIRLWIND_PATH, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RideUpperWhirlwind)},
    {TRIGGER_STEP, CELL_BEHAVIOR_EAST_WHIRLWIND_PATH, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RideWhirlwind)},
    {TRIGGER_STEP, CELL_BEHAVIOR_TRAPDOOR_BUTTON, FLAG_SCENE_TEMP_02, EVENT(JupiterLighthouse_OpenFloor)},
    {TRIGGER_STEP, CELL_BEHAVIOR_OFF_TRAPDOOR_BUTTON, IF_FLAG_SET(FLAG_SCENE_TEMP_02), EVENT(JupiterLighthouse_CloseFloor)},
    {TRIGGER_TALK, ACTOR_ISAAC, IF_FLAG_SET(FLAG_ISAAC_AND_IVAN_FELL), EVENT(JupiterLighthouse_TalkAfterTheFight)},
    {TRIGGER_TALK, ACTOR_IVAN, IF_FLAG_SET(FLAG_ISAAC_AND_IVAN_FELL), EVENT(JupiterLighthouse_TalkAfterTheFight)},
    {TRIGGER_TALK, ACTOR_SHEBA, IF_FLAG_SET(FLAG_ISAAC_AND_IVAN_FELL), EVENT(JupiterLighthouse_TalkAfterTheFight)},
    {TRIGGER_TALK, ACTOR_JENNA, IF_FLAG_SET(FLAG_ISAAC_AND_IVAN_FELL), EVENT(JupiterLighthouse_TalkAfterTheFight)},
    {TRIGGER_TALK, ACTOR_KRADEN, IF_FLAG_SET(FLAG_ISAAC_AND_IVAN_FELL), EVENT(JupiterLighthouse_TalkAfterTheFight)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_ENERGY_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ENERGY_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_FinishMovingBlock)},
    {TRIGGER_PUSH, -1, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StartMovingBlock)},
    {TRIGGER_PUSHED, -1, CONDITION_ALWAYS, EVENT(JupiterLighthouse_FinishMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GRAY_BLOCK, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_EAST_ROPE_POST, FLAG_JUPITER_LIGHTHOUSE_ROPE_TIED, 0},
    {TRIGGER_RAISED, RAISED_RIDE_WHIRLWIND, CONDITION_ALWAYS, EVENT(Whirlwind_CarryLeader)},
    {TRIGGER_TABLE_END},
};

LATE_DATA u32 gJupiterLighthouse_GaretHangsScript[] = {
#if !TLA_PATCH_1
    SC_ANIMATION(14),
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Y, toFIXED(12)),
#else
    SC_ANIMATION(14),
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_Y, toFIXED(12)),
#endif
    SC_END,
};

LATE_DATA u32 gJupiterLighthouse_MiaScript[] = {
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_ANIMATION(9),
    SC_WAIT(120),
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(120),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

LATE_DATA u32 gJupiterLighthouse_IsaacFightsScript[] = {
    SC_ANIMATION(2),
    SC_CALL(JupiterLighthouse_StopFacingMotion),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(60),
    SC_MOVE_XZ(737, 202),
    SC_ANIMATION(2),
    SC_WAIT(4),
    SC_MOVE_XZ(776, 202),
    SC_CALL(JupiterLighthouse_PlayRandomBlowSound),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

LATE_DATA u32 gJupiterLighthouse_AgatioFightsScript[] = {
    SC_ANIMATION(2),
    SC_CALL(JupiterLighthouse_StopFacingMotion),
    SC_SYNC,
    SC_ANIMATION(9),
    SC_WAIT(60),
    SC_MOVE_XZ(776, 202),
    SC_ANIMATION(2),
    SC_WAIT(4),
    SC_MOVE_XZ(737, 202),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

LATE_DATA u32 gJupiterLighthouse_IvanFightsScript[] = {
    SC_CALL(JupiterLighthouse_StopFacingMotion),
    SC_MOVE_XZ(731, 213),
    SC_ANIMATION(2),
    SC_WAIT(4),
    SC_MOVE_XZ(731, 232),
    SC_CALL(JupiterLighthouse_PlayRandomBlowSound),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(53),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

LATE_DATA u32 gJupiterLighthouse_KarstFightsScript[] = {
    SC_ANIMATION(2),
    SC_CALL(JupiterLighthouse_StopFacingMotion),
    SC_MOVE_XZ(731, 232),
    SC_WAIT(4),
    SC_MOVE_XZ(731, 213),
    SC_SYNC,
    SC_ANIMATION(8),
    SC_WAIT(53),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

LATE_DATA u32 gJupiterLighthouse_FelixLooksAroundScript[] = {                                                               
      SC_SET(SW_FACING, FACING_SOUTH),     SC_WAIT(20),                        
      SC_SET(SW_FACING, FACING_SOUTHWEST), SC_WAIT(20),                        
      SC_SET(SW_FACING, FACING_SOUTH),     SC_WAIT(20),                        
      SC_SET(SW_FACING, FACING_SOUTHEAST), SC_WAIT(20),                        
      SC_LOOP(SC_FOREVER, SC_TOP),                                             
  };

LATE_DATA u32 gJupiterLighthouse_FelixArrivesWestScript[] = {
    SC_ANIMATION(5),
    SC_MOVE_BY(0, 0, 32),
    SC_SYNC,
    SC_MOVE_XZ(744, 328),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_END,
};

LATE_DATA u32 gJupiterLighthouse_FelixArrivesEastScript[] = {
    SC_ANIMATION(5),
    SC_MOVE_BY(-8, 0, 32),
    SC_SYNC,
    SC_MOVE_XZ(896, 328),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
    SC_END,
};

LATE_DATA u32 gJupiterLighthouse_KradenArrivesWestScript[] = {                                                               
      SC_ANIMATION(2),                                                              
      SC_MOVE_XZ(740, 312),                                      
      SC_SYNC,                                                                 
      SC_ANIMATION(1),                                                               
      SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),                           
      SC_END,                                                                  
  };

LATE_DATA u32 gJupiterLighthouse_KradenArrivesEastScript[] = {                                                               
      SC_ANIMATION(2),                                                              
      SC_MOVE_XZ(900, 312),                                     
      SC_SYNC,                                                                 
      SC_ANIMATION(1),                                                               
      SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),                       
      SC_END,                                                                  
  };

LATE_DATA u32 gJupiterLighthouse_PiersArrivesWestScript[] = {                                                               
      SC_ANIMATION(5),                                                              
      SC_MOVE_XZ(724, 312),                                      
      SC_SYNC,                                                                 
      SC_ANIMATION(1),                                                               
      SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),                           
      SC_END,                                                                  
  };

LATE_DATA u32 gJupiterLighthouse_PiersArrivesEastScript[] = {                                                               
      SC_ANIMATION(5),                                                              
      SC_MOVE_XZ(916, 312),                                     
      SC_SYNC,                                                                 
      SC_ANIMATION(1),                                                               
      SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),                       
      SC_END,                                                                  
  };

LATE_DATA u32 gJupiterLighthouse_JennaArrivesWestScript[] = {
    SC_ANIMATION(5),
    SC_MOVE_XZ(677, 287),
    SC_SYNC,
    SC_ANIMATION(5),
    SC_MOVE_XZ(740, 344),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_END,
};

LATE_DATA u32 gJupiterLighthouse_JennaArrivesEastScript[] = {
    SC_ANIMATION(5),
    SC_MOVE_XZ(950, 287),
    SC_SYNC,
    SC_ANIMATION(5),
    SC_MOVE_XZ(900, 344),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
    SC_END,
};

LATE_DATA u32 gJupiterLighthouse_ShebaArrivesWestScript[] = {                                                               
      SC_ANIMATION(5),                                                              
      SC_MOVE_XZ(708, 328),                                      
      SC_SYNC,                                                                 
      SC_ANIMATION(1),                                                               
      SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),                           
      SC_END,                                                                  
  };

LATE_DATA u32 gJupiterLighthouse_ShebaArrivesEastScript[] = {
    SC_ANIMATION(5),
    SC_MOVE_BY(-8, 0, 0),
    SC_SYNC,
    SC_ANIMATION(5),
    SC_MOVE_XZ(924, 328),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
    SC_END,
};

LATE_DATA u32 gJupiterLighthouse_PiersStepsBackScript[] = {
    1, 38,
};

LATE_DATA u32 gJupiterLighthouse_WalkToAerieScript[] = {
    SC_ANIMATION(2),
    SC_MOVE_XZ(706, 172),
    SC_SYNC,
    SC_MOVE_XZ(682, 151),
    SC_SYNC,
    SC_MOVE_XZ(682, 120),
    SC_SYNC,
    SC_MOVE_XZ(664, 106),
    SC_SYNC,
    SC_MOVE_XZ(664, 100),
    SC_SYNC,
    SC_END,
};

LATE_DATA u32 gJupiterLighthouse_LeaveForAerieScript[] = {
    SC_MOVE_XZ(664, 80),
    SC_SYNC,
    SC_RELEASE,
};

LATE_DATA s32 gPaletteCycleFrame = -1;

LATE_DATA s32 gPaletteCyclePausesInEvents = 1;

LATE_DATA s32 gWhirlwindEndScript[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

LATE_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};
