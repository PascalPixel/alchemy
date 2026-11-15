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
#include "field/field.h"
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

extern s16 gLightSpriteTile;
extern s16 gLightSpriteCount;
extern struct Actor *gLightSpriteActors[16];
extern struct LightOam gLightSpriteOam[];
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
extern u16 gPaletteCycleColors[];
extern u16 gPaletteCycleUnlitPalette[];
extern u16 gPaletteCycleUnlitColors[];
extern s32 gWhirlwindRiding;

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

extern struct Trigger gJupiterLighthouseBasement_Triggers[];
extern struct Trigger gJupiterLighthouseBasement_Room2Triggers[];

void MadPlant_LeapAtLeader(s32 a, s32 b, s32 c);

static __inline__ void JupiterLighthouse_CopyTouchedCells(s32 a, s32 b, s32 c,
                                                          s32 d, s32 e, s32 f,
                                                          s32 g) {
  TileCopies_CopyTouchedCells(a, b, c, d, e, f, g);
}

static __inline__ void JupiterLighthouse_MadPlantLeaps(s32 a, s32 b, s32 c) {
  MadPlant_LeapAtLeader(a, b, c);
}

void JupiterLighthouse_ClearFirstBushes(void) {
  JupiterLighthouse_CopyTouchedCells(69, 0, 4, 4, 67, 24, 1);
}

void JupiterLighthouse_ClearSecondBushes(void) {
  JupiterLighthouse_CopyTouchedCells(64, 0, 4, 3, 89, 28, 1);
}

void JupiterLighthouse_UncoverMadPlant(void) {
  JupiterLighthouse_CopyTouchedCells(64, 0, 4, 3, 89, 28, 1);
  if (!Event_GetFlag(FLAG_MAD_PLANT(3))) {
    JupiterLighthouse_MadPlantLeaps(SPRITE_MAD_PLANT, 424, 488);
    gMainRoutineWork.pendingDjinni = IF_FLAG_SET(FLAG_MAD_PLANT(3));
    Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
    Field_RequestBattleFromEncounterTable(ENCOUNTER_TABLE_MAD_PLANTS, 3);
  }
}

void JupiterLighthouse_ClearThirdBushes(void) {
  JupiterLighthouse_CopyTouchedCells(69, 1, 4, 3, 108, 31, 1);
}

void JupiterLighthouse_TeleportToEntrance(void) {
  Import_EventSetExitRequest(4);
}

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
void JupiterLighthouse_RestartSpark(struct Actor *object) {
  if (Random16() & 0xff) {
    s32 position[3];
    s32 radius;
    s32 angle;

    SET_VECTOR(position, px(200), px(-32), px(164));
    radius = Random16();
    angle = Random16();
    FieldVector_AddPolarOffsetFixed((radius << 5) + px(8), angle, position);
    SET_ACTOR_POSITION(object, position[0], position[1], position[2]);
  }
  object->userValue = object->timer;
  if (object->timer == 212)
    Actor_SelectAnimation(object, 9);
  else
    Actor_SelectAnimation(object, 10);
}

void TilesetAnim_SuspendAll(void);
void TilesetAnim_ResumeAll(void);
void JupiterLighthouse_LoadPalettes(s32 mode);

/// A spark's update: it stands still while Psynergy or a menu is up, waits out
/// its starting delay, and otherwise rises, swaying, until it restarts.
void JupiterLighthouse_UpdateSpark(struct Actor *object) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  if (work->psynergy != 0 || work->fieldMenu != 0 || work->pauseMenu != 0 ||
      work->item != 0) {
    Actor_SetAnimationSpeed(object, 0);
    TilesetAnim_SuspendAll();
    return;
  }
  Actor_SetAnimationSpeed(object, 16);
  TilesetAnim_ResumeAll();
  if (object->riseEnabled != 0) {
    if ((gFrameCount & 7) == 0)
      object->riseEnabled--;
    return;
  }
  if (object->userValue == 0)
    JupiterLighthouse_RestartSpark(object);
  object->userValue--;
  object->x.fixed +=
      Trig_Cos(((object->z.fixed >> 14) + (object->y.fixed >> 15)) << 8) / 2;
  object->y.fixed += object->speedLimit;
}

/// Places the side room's sixteen sparks, each starting a little later and
/// rising a little faster than the one before.
void JupiterLighthouse_SpawnSparks(void) {
  s32 i;
  s32 next;

  struct Actor *object;
  s16 *action;
  s16 *timer;

  i = 0;
  do {
    object = (FieldActor_Create(SPRITE_PARTICLE, 0, 0, 0));

    object->motionFlags = 0;
    object->priorityFlags = 0;
    object->speedLimit = (i << 12) + px(1) / 2;
    object->acceleration = (i << 12) + px(1) / 2;
    object->update = (void (*)(void))JupiterLighthouse_UpdateSpark;
    object->riseEnabled = i << 3;
    Actor_SetSpriteFlags(object, 0);
    FieldActor_SetSpriteColor(object, 2);
    ((struct FieldSprite *)object->sprite)->priority = 1;
    Actor_SetBlendMode(object, 1);
    action = &object->timer;
    timer = (s16 *)&object->userValue;
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

s32 Scene_Initialize(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
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

void JupiterLighthouse_LoadPalettes(s32 mode) {
  gPaletteCyclePausesInEvents = 1;
  if (mode != 2) {
    Iwram_CopyWordsEntry(gPaletteCycleColors, (void *)&BG_PLTT_COLOR(12, 0),
                         32);
    Iwram_CopyWordsEntry(gPaletteCycleUnlitPalette,
                         (void *)&BG_PLTT_COLOR(13, 0), 32);
  }
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED) || mode == 1) {
    PaletteCycle_SetFrame(0);
    Task_Add(PaletteCycle_StepAt, TASK_PASS_VBLANK);
  } else {
    Dma_Set(gPaletteCycleUnlitColors, (void *)&BG_PLTT_COLOR(12, 2),
            DMA_COPY_WORDS(2), REG_DMA3);
  }
}

void Actor_RaiseFromCell(s32 id) {
  struct Actor *object;
  s32 height;

  object = Actor_Get(id);
  object->motionFlags = 4;
  height = object->groundHeight;
  object->bounce = 0;
  object->y.fixed = height + px(32);
  object->priorityFlags |= 128;
  height =
      Map_GetTerrainHeight(object->layer, object->x.fixed, object->z.fixed);
  Import_MapGridSetCellHeight(object->x.fixed, object->z.fixed, 0,
                              (height >> 19) + 6);
  MapGrid_SetAttributeBAt(object->x.fixed, object->z.fixed, 0, 128);
}

#include "shared/whirlwind_carry_leader_lower.h"

#include "shared/position_flat_distance.h"

extern s32 gWhirlwindEndScript[];

void Actor_StartScript(struct Actor *object, const s32 *script);

#include "shared/whirlwind_update_lower.h"

extern s16 gJupiterLighthouse_LightLevel;

void Actor_SetSpriteFlags(struct Actor *object, u32 value);

#include "shared/whirlwind_spawn.h"

s32 JupiterLighthouse_RaiseSparkles(struct Actor *object) {
  struct EffectOptions params;

  if ((gFrameCount & 7) == 0) {
    s32 x;
    s32 y;

    params.priority = 3;
    params.startScaleX = Q16_ONE * 7 / 10;
    params.startScaleY = Q16_ONE * 7 / 10;
    params.palette = 14;
    x = object->x.fixed + px((Random_Below(9)) - 4);
    y = object->y.fixed + px(32 - (Random_Below(20)));
    FieldEffect_Spawn(x, y, object->z.fixed, 0,
                      (s32)(px(Random_Below(5)) + px(5)) / 10, 0, px(11),
                      &params);
  }
  return 0;
}

#include "shared/jupiter_lighthouse_place_raised_actors.h"
extern s16 gLightSpriteLargeOam;

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
      s32 big, ox, y, z, oy, depth;
      s32 frame;
      u32 *word;
      ox = actor->x.fixed - viewX;
      y = actor->y.fixed - viewY + px(8);
      z = actor->z.fixed - viewZ - viewY;
      oy = z - y;
      depth = FIELD_PIXEL(y + z) + 58;
      big = gLightSpriteLargeOam;
      big = gLightSpriteLargeOam;
      if (big == 0) {
        s32 x, sy;
        frame = actor->timer;
        x = FIELD_PIXEL(ox);
        sy = FIELD_PIXEL(oy);
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
        x = FIELD_PIXEL(ox);
        sy = FIELD_PIXEL(oy);
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

void LightSprites_InitA(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWordsEntry(gLightSpriteActors, sizeof(gLightSpriteActors));
  Decompress_TaggedLz(gField_CachedTilesA, buffer);
  gLightSpriteTile = VramCache_FindFreeEntry();
  VramCache_Load(gLightSpriteTile, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(LightSprites_Draw, TASK_PASS_VBLANK);
  gLightSpriteCount = 0;
  gLightSpriteLargeOam = 0;
  gJupiterLighthouse_LightLevel = -1;
}

void LightSprites_InitB(void) {
  u8 *buffer = Heap_Alloc(0xc00);

  Iwram_ClearWordsEntry(gLightSpriteActors, sizeof(gLightSpriteActors));
  Decompress_TaggedLz(gField_CachedTilesB, buffer);
  gLightSpriteTile = VramCache_FindFreeEntry();
  Vram_Load(gLightSpriteTile, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(LightSprites_Draw, TASK_PASS_VBLANK);
  gLightSpriteCount = 0;
  gLightSpriteLargeOam = 0;
  gJupiterLighthouse_LightLevel = -1;
}

void LightSprites_InitC(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWordsEntry(gLightSpriteActors, sizeof(gLightSpriteActors));
  Decompress_TaggedLz(gField_CachedTilesC, buffer);
  gLightSpriteTile = VramCache_FindFreeEntry();
  Vram_Load(gLightSpriteTile, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(LightSprites_Draw, TASK_PASS_VBLANK);
  gLightSpriteCount = 0;
  gLightSpriteLargeOam = 1;
  gJupiterLighthouse_LightLevel = -1;
}

#include "shared/light_sprites_add.h"

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

#include "shared/cracked_tiles_find_lower.h"

void TilesetAnim_SuspendAll(void);
void TilesetAnim_ResumeAll(void);
void JupiterLighthouse_RestartSpark(struct Actor *object);
void JupiterLighthouse_LoadPalettes(s32 mode);

#include "shared/leader_center_in_cell.h"

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
#include "shared/cracked_tile_crack.h"

struct CellTargetEntry;

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY
#include "shared/cracked_tile_drop_in.h"

extern struct CellTarget gCrackedTiles;
void CrackedTile_Crack();
void CrackedTile_Fall();

#include "shared/cracked_tiles_step.h"

#include "shared/cracked_tile_place_landing.h"

void JupiterLighthouse_ClearFirstBushes(void);
void JupiterLighthouse_UncoverMadPlant(void);
void JupiterLighthouse_TeleportToGroundFloor(void);

// clang-format off
u32 gFieldEffect_ShortScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(22, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_RELEASE,                                                     
      SC_END,                                                                  
  };

u32 gFieldEffect_MediumScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(44, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_RELEASE,                                                     
      SC_END,                                                                  
  };

u32 gFieldEffect_LongScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(126, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_RELEASE,                                                     
      SC_END,                                                                  
  };

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

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
