/// @file
/// The hall of hover tiles at the top of Jupiter Lighthouse's right tower,
/// below the aerie. Hover here lasts as long as the party stays on the hall's
/// floor (JupiterLighthouse_StartHovering), and stone faces blow whirlwinds
/// across the floor and the upper ledges as the party passes their cells
/// (JupiterLighthouse_RaiseWhirlwind, JupiterLighthouse_RaiseUpperWhirlwind).
/// Cracked tiles give way under a second step
/// (JupiterLighthouse_StepOnLowerCrackedTile,
/// JupiterLighthouse_StepOnUpperCrackedTile); the way on leads out to the
/// aerie.
///
/// The cracked tiles' code is in
/// jupiter_lighthouse_room_phaetons_blade_chest/, in two parts the shared field
/// effect objects link between.

#include "shared/tables.h"
#include "flags.h"
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
#include "macros.h"
#include "field/scene_tables.h"
#include "system/main_routine_work.h"
#include "field/event_inline.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "field/call.h"
#include "system/field_effect.h"
#include "field/field_coordinate.h"
#include "field/map_grid.h"
#include "graphics/vram_tab.h"
#include "system/task.h"
#include "window/item_icon.h"
#include "field/cell_target.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "field/field_event.h"
#include "system/frame.h"
#include "field/field_sprite.h"
#include "system/heap_release.h"
#include "system/io_write.h"
#include "field/field_effect.h"

/// Where a stone face's whirlwind starts: an unseen actor moved to the face
/// that blows, which the whirlwind takes its place and heading from.
enum { ACTOR_WHIRLWIND_SOURCE = ACTOR_FIRST_PLACED };

struct Actor;
struct LightOam;
void CrackedTile_PlaceLanding(s32 x, s32 z, s32 effect);
void JupiterLighthouse_LoadPalettes(s32 mode);
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
void JupiterLighthouse_StopHovering(void);
void Whirlwind_CarryLeader(void);
void MapGrid_SetCellHeight();
void MapGrid_SetAttributeBAt();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

s32 CrackedTiles_Step();
struct Actor *Whirlwind_SpawnUpper();

/// As Hover lifts the party: while the leader hovers, walls off the cell at
/// (544, 272).
void JupiterLighthouse_StartHovering(void) {
  if (gMainRoutineWork.movementMode == LEADER_MOVEMENT_HOVER)
    FieldMapGrid_SetCellBehavior(toFIXED(544), toFIXED(272), 0,
                                 CELL_BEHAVIOR_WALL);
}

/// As Hover ends: gives the cell at (544, 272) back its event 6.
void JupiterLighthouse_StopHovering(void) {
  MapGrid_SetCellBehavior(toFIXED(544), toFIXED(272), 0, 6);
}

/// Stepping on event 30 or 31 on the floor: unless a whirlwind is already
/// carrying the party, the stone face there blows one across the hall.
void JupiterLighthouse_RaiseWhirlwind(s32 id) {
  struct Actor *object;

  if (gWhirlwindRiding == 0) {
    object = Actor_Get(ACTOR_WHIRLWIND_SOURCE);
    if (id == 30) {
      Actor_SetPosition(ACTOR_WHIRLWIND_SOURCE, 296, 600);
      object->y.fixed = 0;
      object->facing = FACING_WEST;
    } else if (id == 31) {
      Actor_SetPosition(ACTOR_WHIRLWIND_SOURCE, 136, 680);
      object->y.fixed = 0;
      object->facing = 0;
    }
    Whirlwind_SpawnUpper(object);
    gWhirlwindRiding = 1;
  }
}

/// On the upper ledges, events 50 to 53: unless a whirlwind is already
/// carrying the party, the stone face there blows a slower one, at a pixel a
/// frame, from 32 pixels up.
void JupiterLighthouse_RaiseUpperWhirlwind(s32 id) {
  struct Actor *object;
  struct Actor *motion;

  if (gWhirlwindRiding == 0 &&
      Actor_Get(gMainRoutineWork.leaderActorId)->y.fixed > 0) {
    object = Actor_Get(ACTOR_WHIRLWIND_SOURCE);
    if (id == 50) {
      Actor_SetPosition(ACTOR_WHIRLWIND_SOURCE, 488, 280);
      object->y.fixed = toFIXED(32);
      object->facing = FACING_WEST;
    } else if (id == 51) {
      Actor_SetPosition(ACTOR_WHIRLWIND_SOURCE, 472, 184);
      object->y.fixed = toFIXED(32);
      object->facing = FACING_WEST;
    } else if (id == 52) {
      Actor_SetPosition(ACTOR_WHIRLWIND_SOURCE, 616, 280);
      object->y.fixed = toFIXED(32);
      object->facing = 0;
    } else if (id == 53) {
      Actor_SetPosition(ACTOR_WHIRLWIND_SOURCE, 632, 184);
      object->y.fixed = toFIXED(32);
      object->facing = 0;
    }
    motion = Whirlwind_SpawnUpper(object);
    if (motion != NULL)
      motion->speedLimit = toFIXED(1);
    gWhirlwindRiding = 1;
  }
}

/// A step on a cracked tile of the floor: a fall drops the party to the floor
/// below (exit 13).
void JupiterLighthouse_StepOnLowerCrackedTile(s32 arg) {
  if (Actor_Get(gMainRoutineWork.leaderActorId)->y.fixed == 0 &&
      CrackedTiles_Step(arg) != 0)
    FieldEvent_SetExitRequest(13);
}

/// A step on a cracked tile of the upper ledges, on the floor's level: two
/// cells draw the leader behind the ledge, and a fall drops the party below
/// (exit 14).
void JupiterLighthouse_StepOnUpperCrackedTile(s32 arg) {
  struct Actor *leader;
  s32 x;
  s32 z;

  if (Actor_Get(gMainRoutineWork.leaderActorId)->y.fixed == 0) {
    leader = Actor_Get(gMainRoutineWork.leaderActorId);
    x = FIELD_CELL(leader->x.fixed);
    z = FIELD_CELL(leader->z.fixed);
    if (x == 18 && z == 34)
      FieldActor_SetSpritePriority(gMainRoutineWork.leaderActorId, 3);
    if (x == 8 && z == 39)
      FieldActor_SetSpritePriority(gMainRoutineWork.leaderActorId, 3);
    if (CrackedTiles_Step(arg) != 0)
      FieldEvent_SetExitRequest(14);
  }
}

/// A step on event 34: on the floor, a lower cracked tile; up on the ledges,
/// the four cells that raise the upper whirlwinds.
void JupiterLighthouse_StepOnCrackedTileOrWhirlwind(s32 arg) {
  struct Actor *leader;
  s32 x;
  s32 z;

  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  if (leader->y.fixed <= 0) {
    JupiterLighthouse_StepOnLowerCrackedTile(arg);
  } else {
    x = FIELD_CELL(leader->x.fixed);
    z = FIELD_CELL(leader->z.fixed);
    if (x == 25) {
      if (z == 17)
        JupiterLighthouse_RaiseUpperWhirlwind(50);
      if (z == 11)
        JupiterLighthouse_RaiseUpperWhirlwind(51);
    }
    if (x == 43) {
      if (z == 17)
        JupiterLighthouse_RaiseUpperWhirlwind(52);
      if (z == 11)
        JupiterLighthouse_RaiseUpperWhirlwind(53);
    }
  }
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Sets up the hall: its cracked tiles, and the landing of a leader who fell
/// through one from the floor above.
s32 Scene_Initialize(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  FieldActor_SetSpritePriority(64, 3);
  gWhirlwindRiding = 0;
  Actor_Get(ACTOR_WHIRLWIND_SOURCE)->motionFlags = 0;
  if ((u16)(gMainRoutineWork.entrance - 6) <= 1) {
    CrackedTiles_Find(33, 0x280, 0x380, 48, 1);
    CrackedTile_PlaceLanding(
        0, 0,
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE));
  } else if (gMainRoutineWork.entrance == 3 || gMainRoutineWork.entrance == 5 ||
             gMainRoutineWork.entrance == 13) {
    CrackedTiles_Find(43, 0x280, 0x380, 49, 1);
    CrackedTile_PlaceLanding(
        -21, 32,
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE));
  } else {
    CrackedTiles_Find(43, 0x280, 0x380, 0, 0);
    CrackedTile_PlaceLanding(
        -2, -27,
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE));
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  JupiterLighthouse_LoadPalettes(0);
  return 0;
}

#include "shared/scene_set_arrival_flags.c"

void GameFlag_SetByte(s32 flag, s32 value);
void MapGrid_CenterOnFollowedPoint(void);

extern s32 gPaletteCyclePausesInEvents;

#include "shared/palette_cycle_set_pauses_in_events.h"

extern s32 gPaletteCycleFrame;

/// Sets the counter, its value built as a word.
static inline void PaletteCycle_SetFrame(s32 value) {
  gPaletteCycleFrame = value;
}

#include "shared/palette_cycle_step.h"

#include "shared/jupiter_lighthouse_load_palettes.h"

#include "shared/actor_raise_from_cell.h"

#include "shared/whirlwind_carry_leader.h"

#include "shared/position_flat_distance.h"

extern s32 gWhirlwindEndScript[];

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);
void Actor_StartScript(struct Actor *object, const s32 *script);

#include "shared/whirlwind_update.h"

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/whirlwind_spawn_upper.h"

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

extern u32 gFieldEffect_LongScript[];

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
    SCENE_EXITS(JupiterLighthouseRoomPhaetonsBladeChest),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST_TO_JUPITER_LIGHTHOUSE_WHORL, JupiterLighthouseWhorl, JUPITER_LIGHTHOUSE_WHORL_FROM_JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST),
    SCENE_EXIT(2, JupiterLighthouseRoomPhaetonsBladeChest, 3),
    SCENE_EXIT(3, JupiterLighthouseRoomPhaetonsBladeChest, 2),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST_TO_JUPITER_LIGHTHOUSE_AERIE, JupiterLighthouseAerie, JUPITER_LIGHTHOUSE_AERIE_FROM_JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST),
    SCENE_EXIT(5, JupiterLighthouseRoomPhaetonsBladeChest, 6),
    SCENE_EXIT(6, JupiterLighthouseRoomPhaetonsBladeChest, 5),
    SCENE_EXIT(7, JupiterLighthouseRoomPhaetonsBladeChest, 8),
    SCENE_EXIT(8, JupiterLighthouseRoomPhaetonsBladeChest, 7),
    SCENE_EXIT(13, JupiterLighthouseRoomPhaetonsBladeChest, 13),
    SCENE_EXIT(14, JupiterLighthouseRoomPhaetonsBladeChest, 14),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, 11, -0x80000, 0, -0x80000, FACING_WEST, 2, 0},
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_CHILD, 424, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_CHILD, 440, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_CHILD, 664, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_CHILD, 680, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_CHILD, 440, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_CHILD, 408, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_CHILD, 424, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_CHILD, 440, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_CHILD, 456, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_CHILD, 472, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_CHILD, 488, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_CHILD, 504, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_CHILD, 520, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_CHILD, 536, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_CHILD, 552, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_CHILD, 568, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_CHILD, 584, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_CHILD, 600, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_CHILD, 616, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_CHILD, 632, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_CHILD, 648, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_CHILD, 664, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_CHILD, 680, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_CHILD, 696, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseWhirlwind)},
    {TRIGGER_STEP, 31, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseWhirlwind)},
    {TRIGGER_STEP, 50, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseUpperWhirlwind)},
    {TRIGGER_STEP, 51, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseUpperWhirlwind)},
    {TRIGGER_STEP, 52, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseUpperWhirlwind)},
    {TRIGGER_STEP, 53, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseUpperWhirlwind)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(Whirlwind_CarryLeader)},
    {TRIGGER_STEP, 33, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnLowerCrackedTile)},
    {TRIGGER_STEP, 34, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnCrackedTileOrWhirlwind)},
    {TRIGGER_STEP, 35, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnLowerCrackedTile)},
    {TRIGGER_STEP, 43, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnUpperCrackedTile)},
    {TRIGGER_STEP, 44, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnUpperCrackedTile)},
    {TRIGGER_STEP, 45, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnUpperCrackedTile)},
    {PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StartHovering)},
    {PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_AFTER_END), 0, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StopHovering)},
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

#include "../field/field_effect_move.c"

// The hover-tile hall of Jupiter Lighthouse's right tower: its copy of the
// effect spawner, and the cracked tiles, found, cracked and fallen through.

extern const s32 *gFieldEffect_DurationScripts[];

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#include "../field/field_effect_spawn.c"

/// The cells CrackedTiles_Step watches, and the cell it copies over the one the
/// leader steps on.
#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map_cell_sprite.c"

#include "shared/cracked_tiles_find.h"

#include "shared/leader_center_in_cell.h"

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
#include "shared/cracked_tile_crack.h"

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY
// The hover-tile hall of Jupiter Lighthouse's right tower: the cracked tiles'
// steps and landings.

struct CellTarget;
struct CellTargetEntry;
void CrackedTile_PlaceLanding(s32 x, s32 z, s32 effect);
void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);
void JupiterLighthouse_StopHovering(void);
void Whirlwind_CarryLeader(void);
extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];

#include "shared/cracked_tile_drop_in.h"

void GameFlag_SetByte(s32 flag, s32 value);

#include "shared/cracked_tiles_step.h"

#include "shared/cracked_tile_place_landing.h"
