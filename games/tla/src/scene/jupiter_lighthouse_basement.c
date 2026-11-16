/// @file
/// Beneath Jupiter Lighthouse, reached by the Cyclone teleporter in its
/// grounds (SceneId_JupiterLighthouseBasementTeleporters). Cyclone clears three
/// patches of bushes, the second hiding a Mad Plant
/// (JupiterLighthouse_UncoverMadPlant); Hover on the purple tile crosses to the
/// far teleporter, whose Cyclone takes the party up to the ground floor inside
/// the main gates (JupiterLighthouse_TeleportToGroundFloor), and the near one
/// back up to the grounds. Its side room
/// (SceneId_JupiterLighthouseBasementPsynergyStone) is lit by sixteen sparks
/// that rise and drift over its floor.
#include "flags.h"
#include "battle/battle.h"
#include "field/field_sequence.h"
#include "field/field.h"
#include "scene/event_layer.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "macros.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "field/event_inline.h"
#include "system/frame.h"
#include "field/field_sprite.h"
#include "debug/debug.h"
#include "field/cell_target.h"
#include "field/field_sequence_event.h"
#include "field/event_commands.h"
#include "scene/scene.h"
#include "scene/mars_lighthouse_entrance.h"
#include "shared/jupiter_lighthouse_aerie.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "field/field_event.h"
#include "system/irq.h"
#include "field/call.h"
#include "system/field_effect.h"
#include "window/item_icon.h"
#include "field/field_coordinate.h"
#include "system/task.h"
#include "field/map_grid.h"
#include "graphics/vram_tab.h"
#include "system/heap_release.h"
#include "system/io_write.h"
#include "shared/tables.h"
#include "field/field_effect.h"
#include "field/actor_script.h"
#include "fixed_math.h"

struct EffectOptions;

void Actor_DepartInRing(s32 id);

void MapGrid_SetAttributeBAt();

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

s32 gWhirlwindRiding BSS_DATA;
u16 gPaletteCycleColors[16] BSS_DATA;
/// Palette 13's first two colours; it is copied whole, with the unlit colours
/// after them.
u16 gPaletteCycleUnlitPalette[2] BSS_DATA;
u16 gPaletteCycleUnlitColors[14] BSS_DATA;
s16 gLightSpriteTile BSS_DATA;
s16 gLightSpriteCount BSS_DATA;
struct Actor *gLightSpriteActors[16] BSS_DATA;
struct LightOam gLightSpriteOam[16] BSS_DATA;
s16 gLightSpriteLargeOam BSS_DATA;
s16 gJupiterLighthouse_LightLevel BSS_DATA;
struct CellTarget gCrackedTiles BSS_DATA;
/// Sets a map cell's ground height (MapGrid_SetCellHeight, through the
/// overlay's import).
void Import_MapGridSetCellHeight();

void Leader_CenterInCell(void);

void JupiterLighthouse_ClearSecondBushes(void);

void JupiterLighthouse_ClearThirdBushes(void);

void JupiterLighthouse_TeleportToEntrance(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

void TileCopies_CopyTouchedCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                 s32 g);

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

extern struct Trigger gJupiterLighthouseBasement_Triggers[];
extern struct Trigger gJupiterLighthouseBasement_Room2Triggers[];

/// Cyclone clears the first patch of the basement's bushes.
void JupiterLighthouse_ClearFirstBushes(void) {
  FieldTileCopies_CopyTouchedCells(69, 0, 4, 4, 67, 24, 1);
}

/// Cyclone clears the second patch of bushes.
void JupiterLighthouse_ClearSecondBushes(void) {
  FieldTileCopies_CopyTouchedCells(64, 0, 4, 3, 89, 28, 1);
}

/// Cyclone clears the bushes hiding a Mad Plant: until it is beaten it leaps at
/// the leader and its battle begins.
void JupiterLighthouse_UncoverMadPlant(void) {
  FieldTileCopies_CopyTouchedCells(64, 0, 4, 3, 89, 28, 1);
  if (!Event_GetFlag(FLAG_MAD_PLANT(3))) {
    FieldMadPlant_LeapAtLeader(SPRITE_MAD_PLANT, 424, 488);
    gMainRoutineWork.pendingDjinni = IF_FLAG_SET(FLAG_MAD_PLANT(3));
    Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
    Field_RequestBattleFromEncounterTable(ENCOUNTER_TABLE_MAD_PLANTS, 3);
  }
}

/// Cyclone clears the third patch of bushes.
void JupiterLighthouse_ClearThirdBushes(void) {
  FieldTileCopies_CopyTouchedCells(69, 1, 4, 3, 108, 31, 1);
}

/// The teleporter's ring takes the party to the entrance (exit 4).
void JupiterLighthouse_TeleportToEntrance(void) {
  Import_EventSetExitRequest(4);
}

/// The teleporter's ring takes the party to the ground floor (exit 6).
void JupiterLighthouse_TeleportToGroundFloor(void) {
  Import_EventSetExitRequest(6);
}

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next ==
      (s32)&SceneId_JupiterLighthouseBasementPsynergyStone)
    return gJupiterLighthouseBasement_Room2Triggers;
  return gJupiterLighthouseBasement_Triggers;
}

/// Sends a spark back to a random place within its circle on the side room's
/// floor, to rise for its lifetime again (116 or 212 frames).
void JupiterLighthouse_RestartSpark(struct Actor *actor) {
  if (Random16() & 0xff) {
    s32 position[3];
    s32 radius;
    s32 angle;

    SET_VECTOR(position, toFIXED(200), toFIXED(-32), toFIXED(164));
    radius = Random16();
    angle = Random16();
    FieldVector_AddPolarOffsetFX((radius << 5) + toFIXED(8), angle, position);
    SET_ACTOR_POSITION(actor, position[0], position[1], position[2]);
  }
  actor->userValue = actor->timer;
  if (actor->timer == 212)
    Actor_SelectAnimation(actor, 9);
  else
    Actor_SelectAnimation(actor, 10);
}

void TilesetAnim_SuspendAll(void);
void TilesetAnim_ResumeAll(void);
void JupiterLighthouse_LoadPalettes(s32 mode);

/// A spark's update: it stands still while Psynergy or a menu is up, waits out
/// its starting delay, and otherwise rises, swaying, until it restarts.
void JupiterLighthouse_UpdateSpark(struct Actor *actor) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  if (work->psynergy != 0 || work->fieldMenu != 0 || work->pauseMenu != 0 ||
      work->item != 0) {
    Actor_SetAnimationSpeed(actor, 0);
    TilesetAnim_SuspendAll();
    return;
  }
  Actor_SetAnimationSpeed(actor, ANIMATION_STEP_NORMAL);
  TilesetAnim_ResumeAll();
  if (actor->riseEnabled != 0) {
    if ((gFrameCount & 7) == 0)
      actor->riseEnabled--;
    return;
  }
  if (actor->userValue == 0)
    JupiterLighthouse_RestartSpark(actor);
  actor->userValue--;
  actor->x.fixed +=
      Trig_Cos(((actor->z.fixed >> 14) + (actor->y.fixed >> 15)) << 8) / 2;
  actor->y.fixed += actor->speedLimit;
}

/// Places the side room's sixteen sparks, each starting a little later and
/// rising a little faster than the one before.
void JupiterLighthouse_SpawnSparks(void) {
  s32 i;
  s32 next;

  struct Actor *actor;
  s16 *action;
  s16 *timer;

  i = 0;
  do {
    actor = (FieldActor_Create(SPRITE_PARTICLE, 0, 0, 0));

    actor->motionFlags = 0;
    actor->priorityFlags = 0;
    actor->speedLimit = (i << 12) + toFIXED(1) / 2;
    actor->acceleration = (i << 12) + toFIXED(1) / 2;
    actor->update = (void (*)(void))JupiterLighthouse_UpdateSpark;
    actor->riseEnabled = i << 3;
    Actor_SetSpriteFlags(actor, 0);
    FieldActor_SetSpriteColor(actor, SPRITE_COLOR_RBG);
    ((struct FieldSprite *)actor->sprite)->priority = 1;
    Actor_SetBlendMode(actor, 1);
    action = &actor->timer;
    timer = (s16 *)&actor->userValue;
    next = i + 1;
    if (i & 1) {
      s32 parameter = 212;
      *action = parameter;
      *timer = 0;
    } else {
      s32 parameter = 116;
      *action = parameter;
      *timer = 0;
    }
    i = next;
  } while (i != 16);
}

/// Sets up the basement room being entered: the teleporters' palettes and the
/// ring the party arrives in, or the Psynergy stone room's blended sparks.
/// Returns 0.
s32 Scene_Initialize(void) {
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gMainRoutineWork.next ==
      (s32)&SceneId_JupiterLighthouseBasementTeleporters) {
    JupiterLighthouse_LoadPalettes(1);
    if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION) &&
        gMainRoutineWork.entrance == 4)
      Actor_DepartInRing(gMainRoutineWork.leaderActorId);
    if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION) &&
        gMainRoutineWork.entrance == 6)
      Actor_DepartInRing(gMainRoutineWork.leaderActorId);
  }
  if (gMainRoutineWork.next ==
      (s32)&SceneId_JupiterLighthouseBasementPsynergyStone) {
    REG_BLDALPHA = BLEND_ALPHA(8, 16);
    REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
    JupiterLighthouse_SpawnSparks();
  }
  return 0;
}

/// Sets the scene's arrival flags before the map is built. Returns 0.
s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  return 0;
}

#include "shared/scene_set_arrival_flags.c"

extern s32 gPaletteCyclePausesInEvents;

#include "shared/palette_cycle_set_pauses_in_events.h"

extern s32 gPaletteCycleFrame;

/// Sets the counter, its value built as a word.
static inline void PaletteCycle_SetFrame(s32 value) {
  gPaletteCycleFrame = value;
}

#include "shared/palette_cycle_step_at.h"

#define PALETTE_CYCLE_TASK PaletteCycle_StepAt
#include "shared/jupiter_lighthouse_load_palettes.h"

/// Lifts an actor 32 pixels over its ground as a platform others may stand on,
/// and raises the map cell under it six height steps to match.
void Actor_RaiseFromCell(s32 id) {
  struct Actor *actor;
  s32 height;

  actor = Actor_Get(id);
  actor->motionFlags = ACTOR_FLOATS;
  height = actor->groundHeight;
  actor->bounce = 0;
  actor->y.fixed = height + toFIXED(32);
  actor->priorityFlags |= ACTOR_PRIORITY_PLATFORM;
  height = Map_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);
  Import_MapGridSetCellHeight(actor->x.fixed, actor->z.fixed, 0,
                              (height >> 19) + 6);
  MapGrid_SetAttributeBAt(actor->x.fixed, actor->z.fixed, 0, 128);
}

#include "shared/whirlwind_carry_leader_lower.h"

#include "shared/position_flat_distance.h"

extern s32 gWhirlwindEndScript[];

void Actor_StartScript(struct Actor *actor, const s32 *script);

#include "shared/whirlwind_update_lower.h"

void Actor_SetSpriteFlags(struct Actor *actor, u32 value);

#include "shared/whirlwind_spawn.h"

/// An actor's update: every eighth frame, a sparkle rising from a random point
/// of it.
s32 JupiterLighthouse_RaiseSparkles(struct Actor *actor) {
  struct EffectOptions params;

  if ((gFrameCount & 7) == 0) {
    FIXED x;
    FIXED y;

    params.priority = 3;
    params.startScaleX = FIXED_ONE * 7 / 10;
    params.startScaleY = FIXED_ONE * 7 / 10;
    params.palette = 14;
    x = actor->x.fixed + toFIXED((Random_Below(9)) - 4);
    y = actor->y.fixed + toFIXED(32 - (Random_Below(20)));
    FieldEffect_Spawn(x, y, actor->z.fixed, 0,
                      (s32)(toFIXED(Random_Below(5)) + toFIXED(5)) / 10, 0,
                      EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY |
                          EFFECT_USE_PALETTE,
                      &params);
  }
  return 0;
}

#include "shared/jupiter_lighthouse_place_raised_actors.h"

/// A task: queues the lit actors' sprites, 16 by 16 or, when large, 32 by 64,
/// at their animation picture and depth, skipping those off screen.
void LightSprites_Draw(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, tile, viewY;
  struct LightOam *oam;
  s32 i;
  viewX = view[0] & FIELD_PIXEL_MASK;
  viewZ = view[1] & FIELD_PIXEL_MASK;
  tile = gVramCacheEntries[gLightSpriteTile].offset >> 5;
  viewY = map->origin[1];
  oam = gLightSpriteOam;

  for (i = 0; i < gLightSpriteCount; i++) {
    struct Actor *actor = gLightSpriteActors[i];
    if (actor != 0 && actor->x.fixed != 0) {
      s32 big, ox;
      FIXED y, z;
      s32 oy, depth;
      s32 frame;
      u32 *word;
      ox = actor->x.fixed - viewX;
      y = actor->y.fixed - viewY + toFIXED(8);
      z = actor->z.fixed - viewZ - viewY;
      oy = z - y;
      depth = FIXEDtoINT(y + z) + 58;
      big = gLightSpriteLargeOam;
      big = gLightSpriteLargeOam;
      if (big == 0) {
        s32 x, sy;
        frame = actor->timer;
        x = FIXEDtoINT(ox);
        sy = FIXEDtoINT(oy);
        ox = x - 8;
        oy = sy - 16;
        if ((u32)(x + 7) > 334 || oy <= -16 || oy > 239)
          continue;
        ox &= 0x1ff;
        oy &= 0xff;
        word = (u32 *)oam;
        *word++ = 0;
        *word++ = oy | (ox << 16) | 0x40002000;
        *word = (tile + frame * 8) | 0x800;
        if (gJupiterLighthouse_LightLevel == -1)
          oam->priority = BattleEffect_GetObjectPriority(actor);
        else
          oam->priority = gJupiterLighthouse_LightLevel;
        Oam_AddSpriteAtDepthClamped(oam, depth);
        oam++;
      } else {
        s32 x, sy;
        frame = actor->timer;
        x = FIXEDtoINT(ox);
        sy = FIXEDtoINT(oy);
        ox = x - 8;
        oy = sy - 64;
        if ((u32)(x + 23) > 350 || oy <= -64 || oy > 175)
          continue;
        ox &= 0x1ff;
        oy &= 0xff;
        word = (u32 *)oam;
        *word++ = 0;
        *word++ = oy | (ox << 16) | 0xc000a000;
        *word = (tile + frame * 8) | 0x800;
        if (gJupiterLighthouse_LightLevel == -1)
          oam->priority = BattleEffect_GetObjectPriority(actor);
        else
          oam->priority = gJupiterLighthouse_LightLevel;
        Oam_AddSpriteAtDepthClamped(oam, depth);
        oam++;
      }
    }
  }
}

static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

/// Loads the first set of light pictures into VRAM and starts drawing the lit
/// actors (none yet), small.
void LightSprites_InitA(void) {
  u8 *buffer = BumpAlloc(3072);

  Iwram_ClearWordsEntry(gLightSpriteActors, sizeof(gLightSpriteActors));
  Decompress_TaggedLz(gField_CachedTilesA, buffer);
  gLightSpriteTile = VramCache_FindFreeEntry();
  VramCache_Load(gLightSpriteTile, 3072, buffer);
  Heap_Release(buffer);
  Task_Add(LightSprites_Draw, TASK_PASS_FRAME);
  gLightSpriteCount = 0;
  gLightSpriteLargeOam = 0;
  gJupiterLighthouse_LightLevel = -1;
}

/// Loads the second set of light pictures into VRAM and starts drawing the lit
/// actors (none yet), small.
void LightSprites_InitB(void) {
  u8 *buffer = Heap_Alloc(3072);

  Iwram_ClearWordsEntry(gLightSpriteActors, sizeof(gLightSpriteActors));
  Decompress_TaggedLz(gField_CachedTilesB, buffer);
  gLightSpriteTile = VramCache_FindFreeEntry();
  Vram_Load(gLightSpriteTile, 2048, buffer);
  Heap_Release(buffer);
  Task_AddFrame(LightSprites_Draw, TASK_PASS_FRAME);
  gLightSpriteCount = 0;
  gLightSpriteLargeOam = 0;
  gJupiterLighthouse_LightLevel = -1;
}

/// Loads the third set of light pictures into VRAM and starts drawing the lit
/// actors (none yet), large.
void LightSprites_InitC(void) {
  u8 *buffer = BumpAlloc(2048);

  Iwram_ClearWordsEntry(gLightSpriteActors, sizeof(gLightSpriteActors));
  Decompress_TaggedLz(gField_CachedTilesC, buffer);
  gLightSpriteTile = VramCache_FindFreeEntry();
  Vram_Load(gLightSpriteTile, 2048, buffer);
  Heap_Release(buffer);
  Task_AddFrame(LightSprites_Draw, TASK_PASS_FRAME);
  gLightSpriteCount = 0;
  gLightSpriteLargeOam = 1;
  gJupiterLighthouse_LightLevel = -1;
}

#include "shared/light_sprites_add.h"

/// Sets the priority the lit actors are drawn at; -1 takes each one's own.
void JupiterLighthouse_SetLightLevel(s32 value) {
  gJupiterLighthouse_LightLevel = value;
}

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

extern const s32 *gFieldEffect_DurationScripts[];

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#include "../field/field_effect_spawn.c"

#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map_cell_sprite.c"

#include "shared/cracked_tile_find_lower.h"

void TilesetAnim_SuspendAll(void);
void TilesetAnim_ResumeAll(void);
void JupiterLighthouse_RestartSpark(struct Actor *actor);
void JupiterLighthouse_LoadPalettes(s32 mode);

#include "shared/leader_center_in_cell.h"

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTile_Step calls it the first time the leader steps on
// one of the table's cells.
#include "shared/cracked_tile_crack.h"

struct CellTargetEntry;

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY
#include "shared/cracked_tile_drop_in.h"

void CrackedTile_Crack();
void CrackedTile_Fall();

#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"

void JupiterLighthouse_ClearFirstBushes(void);
void JupiterLighthouse_UncoverMadPlant(void);
void JupiterLighthouse_TeleportToGroundFloor(void);

// clang-format off
#include "shared/field_effect_duration_scripts.c"

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(JupiterLighthouseBasementTeleporters),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS_TO_JUPITER_LIGHTHOUSE_BASEMENT_PSYNERGY_STONE, JupiterLighthouseBasementPsynergyStone, JUPITER_LIGHTHOUSE_BASEMENT_PSYNERGY_STONE_FROM_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS),
    SCENE_EXIT(2, JupiterLighthouseBasementTeleporters, 3),
    SCENE_EXIT(3, JupiterLighthouseBasementTeleporters, 2),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS_TO_JUPITER_LIGHTHOUSE_ENTRANCE, JupiterLighthouseEntrance, JUPITER_LIGHTHOUSE_ENTRANCE_FROM_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS_TO_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON, JupiterLighthouseRoomGateButton, JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_FROM_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS),
    SCENE_EXITS(JupiterLighthouseBasementPsynergyStone),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_BASEMENT_PSYNERGY_STONE_TO_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS, JupiterLighthouseBasementTeleporters, JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS_FROM_JUPITER_LIGHTHOUSE_BASEMENT_PSYNERGY_STONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {SCENE_TABLE_END},
};

struct Trigger gJupiterLighthouseBasement_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 30, CONDITION_ALWAYS, EVENT(JupiterLighthouse_ClearFirstBushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 31, CONDITION_ALWAYS, EVENT(JupiterLighthouse_ClearSecondBushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 32, CONDITION_ALWAYS, EVENT(JupiterLighthouse_ClearThirdBushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 33, CONDITION_ALWAYS, EVENT(JupiterLighthouse_TeleportToEntrance)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 34, CONDITION_ALWAYS, EVENT(JupiterLighthouse_TeleportToGroundFloor)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 38, CONDITION_ALWAYS, EVENT(JupiterLighthouse_UncoverMadPlant)},
    {TRIGGER_TABLE_END},
};

struct Trigger gJupiterLighthouseBasement_Room2Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_TABLE_END},
};

s32 gPaletteCycleFrame = -1;

s32 gPaletteCyclePausesInEvents = 1;

s32 gWhirlwindEndScript[] = {
    SC_ANIMATION(5),
    SC_WAIT(12),
    SC_RELEASE,
};
// clang-format on

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};
