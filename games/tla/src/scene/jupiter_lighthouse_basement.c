/// @file
#include "flags.h"
#include "shared/whirlwind.h"
#include "shared/jupiter_lighthouse.h"
#include "shared/actors.h"
#include "shared/effects.h"
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
#include "macros.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "field/event_inline.h"
#include "system/frame.h"
#include "field_sprite.h"
#include "debug.h"
#include "field/cell_target.h"
#include "event_work.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/scene/scene.h"
#include "module/scene/mars_lighthouse_entrance.h"
#include "module/tla.h"
#include "shared/jupiter_lighthouse_aerie.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "field_event.h"
#include "system/irq.h"
#include "call.h"
#include "system/field_effect.h"
#include "item_icon.h"
#include "field/field_coordinate.h"
#include "system/task.h"
#include "map.h"
#include "graphics/vram_tab.h"
#include "shared/cracked_tile.h"
#include "system/heap_release.h"
#include "system/io_write.h"
#include "shared/tables.h"
#include "module/field/field_effect.h"
#include "objcmd.h"

struct EffectOptions;

void Actor_DepartInRing(s32 id);

void MapGrid_SetAttributeBAt();

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

extern s16 gLightSpriteTile;
extern s16 gLightSpriteCount;
extern struct Actor *gLightSpriteActors[];
extern struct LightOam Data_02002434[];

void Leader_CenterInCell(void);

void Func_02000074(void);

void Func_020000f8(void);

void Func_02000118(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

void TileCopies_CopyTouchedCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                 s32 g);
extern u16 gPaletteCycleColors[];
extern u16 gPaletteCycleUnlitPalette[];
extern u16 gPaletteCycleUnlitColors[];
extern s32 Data_020023ac;

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

struct MainRoutineWork254 {
  u8 unk000[0x254];
  u16 value;
};

extern struct Trigger gJupiterLighthouseBasement_Triggers[];
extern struct Trigger gJupiterLighthouseBasement_Room2Triggers[];

void FieldItem_JumpToLeader(s32 a, s32 b, s32 c);

static __inline__ void Func_02001d1c_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  TileCopies_CopyTouchedCells(a, b, c, d, e, f, g);
}

static __inline__ void Func_02001d44_(s32 a, s32 b, s32 c) {
  FieldItem_JumpToLeader(a, b, c);
}

void Func_02000054(void) { Func_02001d1c_(69, 0, 4, 4, 67, 24, 1); }

void Func_02000074(void) { Func_02001d1c_(64, 0, 4, 3, 89, 28, 1); }

void Func_02000094(void) {
  Func_02001d1c_(64, 0, 4, 3, 89, 28, 1);
  if (!Event_GetFlag(0x9b8)) {
    Func_02001d44_(249, 424, 488);
    ((struct MainRoutineWork254 *)&gMainRoutineWork)->value = 0x19b8;
    gMainRoutineWork.battleRule24b = 2;
    Field_RequestBattleFromEncounterTable(106, 3);
  }
}

void Func_020000f8(void) { Func_02001d1c_(69, 1, 4, 3, 108, 31, 1); }

void Func_02000118(void) { Import_EventSetExitRequest(4); }

void Func_02000124(void) { Import_EventSetExitRequest(6); }

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov699Room2)
    return gJupiterLighthouseBasement_Room2Triggers;
  return gJupiterLighthouseBasement_Triggers;
}

void Func_0200015c(struct Actor *object) {
  if (Random16() & 0xff) {
    s32 position[3];
    s32 radius;
    s32 angle;

    SET_VECTOR(position, 200 << 16, -32 << 16, 164 << 16);
    radius = Random16();
    angle = Random16();
    FieldVector_AddPolarOffset((radius << 5) + 0x80000, angle, position);
    SET_POSITION(object, position[0], position[1], position[2]);
  }
  object->unk66 = object->timer;
  if (object->timer == 212)
    Actor_SelectAnimation(object, 9);
  else
    Actor_SelectAnimation(object, 10);
}

struct EventWork1ac {
  u8 unk000[0x1ac];
  s32 value;
};

void TilesetAnim_SuspendAll(void);
void TilesetAnim_ResumeAll(void);
void JupiterLighthouse_LoadPalettes(s32 mode);

#define OBJECT_BYTE(object, offset) (((u8 *)(object))[offset])

void Func_020001d8(struct Actor *object) {
  u8 *work = Ram_HeapSlots->eventWork;

  if (*(s16 *)(work + 358) != 0 || *(s16 *)(work + 346) != 0 ||
      *(s16 *)(work + 350) != 0 || *(s16 *)(work + 360) != 0) {
    Actor_SetAnimationSpeed(object, 0);
    TilesetAnim_SuspendAll();
    return;
  }
  Actor_SetAnimationSpeed(object, 16);
  TilesetAnim_ResumeAll();
  if (OBJECT_BYTE(object, 99) != 0) {
    if ((gFrameCount & 7) == 0)
      OBJECT_BYTE(object, 99)--;
    return;
  }
  if (*(s16 *)((u8 *)object + 102) == 0)
    Func_0200015c(object);
  (*(s16 *)((u8 *)object + 102))--;
  /* FAKEMATCH(dead): an empty once-only block, whose loop note ends the
   * scheduling block. */
  do {
  } while (0);
  object->x += Trig_Cos(((object->z >> 14) + (object->y >> 15)) << 8) / 2;
  object->y += object->speedLimit;
}

void Func_02000298(void) {
  s32 i;
  s32 next;

  struct Actor *object;
  s16 *action;
  s16 *timer;

  i = 0;
  do {
    object = (FieldActor_Create(0x2a0, 0, 0, 0));

    object->motionFlags = 0;
    object->priorityFlags = 0;
    object->speedLimit = (i << 12) + 0x8000;
    object->acceleration = (i << 12) + 0x8000;
    object->update = (void (*)(void))Func_020001d8;
    OBJECT_BYTE(object, 99) = i << 3;
    Actor_SetSpriteFlags(object, 0);
    FieldActor_SetSpriteColor(object, 2);
    ((struct FieldSprite *)object->sprite)->priority = 1;
    Actor_SetBlendMode(object, 1);
    action = &object->timer;
    timer = (s16 *)&object->unk66;
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
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 0x204;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov699Room1) {
    JupiterLighthouse_LoadPalettes(1);
    if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION) &&
        gMainRoutineWork.entrance == 4)
      Actor_DepartInRing(gMainRoutineWork.currentOwner);
    if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION) &&
        gMainRoutineWork.entrance == 6)
      Actor_DepartInRing(gMainRoutineWork.currentOwner);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov699Room2) {
    *(volatile u16 *)0x04000052 = 0x1008;
    *(volatile u16 *)0x04000050 = 0x3f10;
    Func_02000298();
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  return 0;
}

#include "shared/scene_set_arrival_flags.c"

extern s32 gPaletteCyclePausesInEvents;

void PaletteCycle_SetPausesInEvents(s32 value) {
  gPaletteCyclePausesInEvents = value;
}

extern s32 gPaletteCycleFrame;

/// Sets the counter, its value built as a word.
static inline void Counter_Set(s32 value) { gPaletteCycleFrame = value; }

DEFINE_PALETTE_CYCLE_STEP_AT(PaletteCycle_StepAt, gPaletteCycleFrame,
                             gPaletteCyclePausesInEvents, gPaletteCycleColors,
                             0x05000184)

DEFINE_JUPITER_LIGHTHOUSE_LOAD_PALETTES(JupiterLighthouse_LoadPalettes,
                                        gPaletteCyclePausesInEvents,
                                        gPaletteCycleColors,
                                        gPaletteCycleUnlitPalette,
                                        PaletteCycle_StepAt,
                                        gPaletteCycleUnlitColors)

DEFINE_ACTOR_RAISE_FROM_CELL(Actor_RaiseFromCell, Func_02001c74,
                             MapGrid_SetAttributeBAt)

DEFINE_WHIRLWIND_CARRY_LEADER_LOWER(Whirlwind_CarryLeaderLower)

DEFINE_POSITION_FLAT_DISTANCE(Position_FlatDistance)

extern s32 gWhirlwindEndScript[];

void Actor_StartScript(struct Actor *object, const s32 *script);

DEFINE_WHIRLWIND_UPDATE_LOWER(Whirlwind_UpdateLower, MapGrid_GetCellBehavior,
                              Position_FlatDistance, gWhirlwindEndScript,
                              Data_020023ac)

extern s16 gData_020024f6;

void Actor_SetSpriteFlags(struct Actor *object, u32 value);

DEFINE_WHIRLWIND_SPAWN(Whirlwind_Spawn, Actor_SelectAnimation,
                       Whirlwind_UpdateLower, FieldEffect_Spawn)

s32 Func_02000a64(struct Actor *object) {
  struct EffectOptions params;

  if ((gFrameCount & 7) == 0) {
    s32 x;
    s32 y;

    params.priority = 3;
    params.startScaleX = 0xb333;
    params.startScaleY = 0xb333;
    params.palette = 14;
    x = object->x + ((((Random16() * 9) >> 16) - 4) << 16);
    y = object->y + ((32 - ((Random16() * 20) >> 16)) << 16);
    FieldEffect_Spawn(x, y, object->z, 0,
                      (s32)((((Random16() * 5) >> 16) << 16) + 0x50000) / 10, 0,
                      0xb0000, &params);
  }
  return 0;
}

extern DEFINE_JUPITER_LIGHTHOUSE_PLACE_RAISED_ACTORS(
    JupiterLighthouse_PlaceRaisedActors) extern s16 gData_020024f4;

void LightSprites_Draw(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, tile, viewY;
  struct LightOam *oam;
  s32 i;
  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  tile = gResourceTableEntries[gLightSpriteTile].offset >> 5;
  viewY = map->origin[1];
  oam = Data_02002434;

  for (i = 0; i < gLightSpriteCount; i++) {
    struct Actor *actor = gLightSpriteActors[i];
    if (actor != 0 && actor->x != 0) {
      s32 big, ox, y, z, oy, depth;
      s32 frame;
      u32 *word;
      ox = actor->x - viewX;
      y = actor->y - viewY + 0x80000;
      z = actor->z - viewZ - viewY;
      oy = z - y;
      depth = ((y + z) >> 16) + 58;
      big = gData_020024f4;
      big = gData_020024f4;
      if (big == 0) {
        s32 x, sy;
        frame = actor->timer;
        x = ox >> 16;
        sy = oy >> 16;
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
        if (gData_020024f6 == -1)
          oam->priority = BattleEffect_GetObjectPriority(actor);
        else
          oam->priority = gData_020024f6;
        Oam_AddSpriteAtDepthClamped(oam, depth);
        oam++;
      } else {
        s32 x, sy;
        frame = actor->timer;
        x = ox >> 16;
        sy = oy >> 16;
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
        if (gData_020024f6 == -1)
          oam->priority = BattleEffect_GetObjectPriority(actor);
        else
          oam->priority = gData_020024f6;
        Oam_AddSpriteAtDepthClamped(oam, depth);
        oam++;
      }
    }
  }
}

static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_LIGHT_SPRITES_INIT_A(LightSprites_InitA, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gData_020024f4, gData_020024f6)

DEFINE_LIGHT_SPRITES_INIT_B(LightSprites_InitB, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gData_020024f4, gData_020024f6)

DEFINE_LIGHT_SPRITES_INIT_C(LightSprites_InitC, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gData_020024f4, gData_020024f6)

DEFINE_LIGHT_SPRITES_ADD(LightSprites_Add, gLightSpriteCount,
                         gLightSpriteActors)

void Func_02001008(s32 value) { gData_020024f6 = value; }

#include "../field/object/set_sprite_priority.c"

#include "../field/field_effect/move.c"

#undef OBJECT_BYTE
// @file
extern const s32 *gFieldEffect_DurationScripts[];

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct Actor *party = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = ((struct FieldEffect *)Import_ActorCreate(options->type, x, y, z));
  else
    effect = ((struct FieldEffect *)Import_ActorCreate(285, x, y, z));
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Actor_SelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect,
                    gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect,
                               ((struct FieldSprite *)party->sprite)->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  if ((flags & EFFECT_USE_START_SCALE) != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 target;
    s32 frames;

    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    target = options->targetScaleX;
    frames = script[3];
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
      effect->scaleRateX = (target - effect->scaleX) / frames;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (target - 0x10000) / frames;
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Actor_SelectAnimation(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

#define CELL_TABLE gCrackedTiles
#define CELL_SPRITE_DRAW CrackedTiles_Update
#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map/cell_sprite.c"

DEFINE_CRACKED_TILES_FIND_LOWER(CrackedTiles_FindLower, gCrackedTiles,
                                CrackedTiles_Update)

void TilesetAnim_SuspendAll(void);
void TilesetAnim_ResumeAll(void);
void Func_0200015c(struct Actor *object);
void JupiterLighthouse_LoadPalettes(s32 mode);

#define OBJECT_BYTE(object, offset) (((u8 *)(object))[offset])

void Leader_CenterInCell(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = leader->x;
  s32 z = leader->z;

  Actor_SetSpeed(gMainRoutineWork.currentOwner, 4);

  x >>= 20;
  z >>= 20;
  Actor_MoveToAndWaitById(gMainRoutineWork.currentOwner, (x << 4) + 8,
                          (z << 4) + 8);
  System_Wait(1);
  FieldActor_SetPositionAndResetMotion(leader, (x << 20) + PIXELS(8), leader->y,
                                       (z << 20) + PIXELS(8));
  FieldEvent_Wait(4);
  FieldCamera_GetActor()->y -= 0x80000;
}

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
DEFINE_CRACKED_TILE_CRACK(CrackedTile_Crack, Leader_CenterInCell)

struct CellTargetEntry;

/* VIEW(s16c4): tag struct CellTarget as struct CellTarget__s16c4; cell_target.h
 * has another definition; tried with the definition the rest of the object has,
 * CrackedTile_Fall moves: 2 of 131 instructions in tla-ja. */
struct CellTarget__s16c4 {
  s16 firstFlag;
  s16 flagByte;
  s16 kind;
  s16 x;
  s16 z;
  s16 count;
  s16 current;
  s16 shown;
  s16 tile;
};

DEFINE_CRACKED_TILE_FALL(CrackedTile_Fall, CellTarget__s16c4,
                         Leader_CenterInCell)

#include "../field/field_effect/advance_motion.c"

#undef CELL_TABLE
#undef CELL_SPRITE_DRAW
#undef CELL_PUSH_SLOT_ENTRY
#undef OBJECT_BYTE
#undef Actor_StartScript
// @file
static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  FieldCamera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  MapGrid_SetScreenShake(a, b, c);
}

DEFINE_CRACKED_TILE_DROP_IN_LOWER(CrackedTile_DropInLower, FieldEffect_Spawn)

extern struct CellTarget gCrackedTiles;
void CrackedTile_Crack();
void CrackedTile_Fall();

DEFINE_CRACKED_TILES_STEP(CrackedTiles_Step, gCrackedTiles, CrackedTile_Crack,
                          CrackedTile_Fall)

DEFINE_CRACKED_TILE_PLACE_LANDING(CrackedTile_PlaceLanding, gCrackedTiles,
                                  CrackedTile_DropInLower)

void Func_02000054(void);
void Func_02000094(void);
void Func_02000124(void);

// clang-format off
DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_ShortScript, 22)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_MediumScript, 44)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_LongScript, 126)

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(252),
    SCENE_EXIT(1, 253, 1),
    SCENE_EXIT(2, 252, 3),
    SCENE_EXIT(3, 252, 2),
    SCENE_EXIT(4, 251, 4),
    SCENE_EXIT(6, 254, 6),
    SCENE_EXITS(253),
    SCENE_EXIT(1, 252, 1),
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
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 30, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 31, CONDITION_ALWAYS, EVENT(Func_02000074)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 32, CONDITION_ALWAYS, EVENT(Func_020000f8)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 33, CONDITION_ALWAYS, EVENT(Func_02000118)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 34, CONDITION_ALWAYS, EVENT(Func_02000124)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 38, CONDITION_ALWAYS, EVENT(Func_02000094)},
    {TRIGGER_TABLE_END},
};

struct Trigger gJupiterLighthouseBasement_Room2Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_TABLE_END},
};

s32 gPaletteCycleFrame = -1;

s32 gPaletteCyclePausesInEvents = 1;

s32 gWhirlwindEndScript[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};
