/// @file
/// The room of hovering blocks and locked doors in Jupiter Lighthouse. The Red
/// Key and the Blue Key open its two doors (JupiterLighthouse_UseRedKey,
/// JupiterLighthouse_UseBlueKey), Reveal shows a hidden passage and its block
/// (JupiterLighthouse_RevealThePassage), and once the lighthouse is activated
/// its blocks hover, Move setting one on its cell to bridge the gap
/// (JupiterLighthouse_FinishMovingBlock). Two Cyclone teleporters lead on
/// (JupiterLighthouse_LeaveByFirstCyclone,
/// JupiterLighthouse_LeaveBySecondCyclone), a beam of light carries the party
/// up from the room below, and lights circle up the beam
/// (JupiterLighthouse_SendUpBeamLights). Its cracked tiles' code comes last.

#include "shared/tables.h"
#include "flags.h"
#include "item_ids.h"
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
#include "system/ram_buffer.h"
#include "field/field_event.h"
#include "field/event_inline.h"
#include "system/iwram_call.h"
#include "field/actor.h"
#include "field/field_sprite.h"
#include "macros.h"
#include "system/main_routine_work.h"
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "field/call.h"
#include "system/field_effect.h"
#include "field/map_grid.h"
#include "graphics/vram_tab.h"
#include "system/task.h"
#include "window/item_icon.h"
#include "field/cell_target.h"
#include "field/actor_script.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/frame.h"
#include "fixed_math.h"
#include "system/heap_release.h"
#include "system/io_write.h"
#include "field/field_effect.h"
#include "text/msg_ids.h"

/// Set as each locked door opens to its key.
#define FLAG_RED_DOOR_OPENED 0xa07
#define FLAG_BLUE_DOOR_OPENED 0xa08
/// Set once Move has set the bridge block on its cell, where it rises to bridge
/// the gap.
#define FLAG_BRIDGE_BLOCK_RAISED 0xa06

/// The bridge block's cell, where it rises to hover in line with the others.
enum { BRIDGE_BLOCK_CELL_X = 10, BRIDGE_BLOCK_CELL_Z = 32 };

/// The room's blocks, numbering the rows of gActorPlacements.
enum JupiterLighthouseBlockId {
  /// The first of the six blocks that rise once the lighthouse is activated.
  ACTOR_FIRST_BLOCK = ACTOR_FIRST_PLACED,
  /// The block Reveal shows on the hidden passage.
  ACTOR_HIDDEN_BLOCK = ACTOR_FIRST_PLACED + 6,
  /// The block Move sets on its cell to bridge the gap.
  ACTOR_BRIDGE_BLOCK,
  /// The first of three more that rise once the lighthouse is activated.
  ACTOR_LAST_BLOCKS,
  /// The unseen block at the top of the beam of light, between the hovering
  /// blocks: the party the beam carries up stands on it. A probe of the
  /// activated room by the beam (render probe --scene
  /// JupiterLighthouseRoomLockedDoors --entrance 11 --set 0xa00) finds it
  /// raised at the beam's place, (232, 520), and the leader 16 pixels above it
  /// at the beam's top among the hovering blocks, nothing drawn under him.
  ACTOR_BEAM_TOP_BLOCK = ACTOR_LAST_BLOCKS + 3
};

/// The beam of light from the room below: where it stands, and how far below
/// the floor its lights, and the party it carries up, start rising.
enum { BEAM_X = 232, BEAM_Z = 520, BEAM_DEPTH = 120 };
/// The most steps the beam lifts the party, 3 pixels each; the floor stops it
/// first.
#define BEAM_RISE_MAX_STEPS 90
/// SPRITE_PARTICLE's animations a beam light takes: its full glow twinkling,
/// and the glow shrinking to a speck and gone.
enum { ANIM_PARTICLE_FADE = 2, ANIM_PARTICLE_TWINKLE = 7 };
/// The step at which a beam light starts to fade, 6 pixels below the floor.
#define BEAM_LIGHT_FADE_STEP 38

void Actor_RaiseFromCell(s32 id);
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
void JupiterLighthouse_HideThePassage(void);
void JupiterLighthouse_LeaveByFirstCyclone(void);
void JupiterLighthouse_LeaveBySecondCyclone(void);
void JupiterLighthouse_TryLockedDoor(void);
void LightSprites_InitA(void);
void MapGrid_SetCellHeight();

void Actor_ArriveInRing(s32 id);
void MapGrid_SetAttributeBAt();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// Reveal: shows the hidden passage and its block, and lets the party jump
/// the gap there; once the lighthouse is activated the block hovers.
void JupiterLighthouse_RevealThePassage(void) {
  MapGrid_CopyCellsLayered(17, 10, 1, 4, 19, 10);
  MapGrid_CopyCellAttributes(17, 10, 1, 1, 19, 10);
  FieldMapGrid_SetCellBehavior(toFIXED(304), toFIXED(160), 0,
                               CELL_BEHAVIOR_JUMP_FORWARD);
  Actor_SetPosition(ACTOR_HIDDEN_BLOCK, 504, 168);
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED)) {
    Actor_RaiseFromCell(ACTOR_HIDDEN_BLOCK);
    FieldMapGrid_SetCellBehavior(toFIXED(496), toFIXED(160), 0,
                                 CELL_BEHAVIOR_JUMP_FORWARD);
  }
}

/// As Reveal fades, and on entering the room: covers the passage again and
/// puts its block away.
void JupiterLighthouse_HideThePassage(void) {
  MapGrid_CopyCellsLayered(16, 10, 1, 4, 19, 10);
  MapGrid_CopyCellAttributes(16, 10, 1, 1, 19, 10);
  Actor_SetPosition(ACTOR_HIDDEN_BLOCK, 0, 0);
  MapGrid_CopyCellAttributes(30, 10, 1, 1, 31, 10);
}

/// As Move is cast on the bridge block: puts its sprite at priority 2.
void JupiterLighthouse_StartMovingBlock(s32 unused, s32 id) {
  Actor_SetSpritePriority(id, 2);
}

/// After Move: once the bridge block sits on its cell over the gap, it rises
/// 32 pixels and hovers there for good.
void JupiterLighthouse_FinishMovingBlock(s32 unused, s32 id) {
  struct Actor *actor;
  s32 i;

  actor = Actor_Get(id);
  if (FIELD_CELL(actor->x.fixed) == BRIDGE_BLOCK_CELL_X &&
      FIELD_CELL(actor->z.fixed) == BRIDGE_BLOCK_CELL_Z) {
    actor->motionFlags = 0;
    for (i = 0; i < 16; i++) {
      actor->y.fixed += toFIXED(2);
      System_Wait(1);
    }
    Actor_RaiseFromCell(id);
    Event_SetFlag(FLAG_BRIDGE_BLOCK_RAISED);
  }
}

/// Takes the party down the first Cyclone teleporter, to the southwest of
/// the Erinyes Tunic's room.
void JupiterLighthouse_LeaveByFirstCyclone(void) {
  Import_EventSetExitRequest(
      JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST);
}

/// Takes the party down the second Cyclone teleporter, to the southeast of
/// the Erinyes Tunic's room.
void JupiterLighthouse_LeaveBySecondCyclone(void) {
  Import_EventSetExitRequest(
      JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHEAST);
}

/// Draws the red door open and lets the party through it.
void JupiterLighthouse_OpenRedDoor(void) {
  MapGrid_CopyCellsLayered(73, 18, 1, 4, 74, 18);
  MapGrid_CopyCellAttributes(10, 9, 1, 1, 10, 20);
}

/// Draws the blue door open and lets the party through it.
void JupiterLighthouse_OpenBlueDoor(void) {
  MapGrid_CopyCellsLayered(103, 18, 1, 4, 104, 18);
  MapGrid_CopyCellAttributes(40, 9, 1, 1, 40, 20);
}

/// The Red Key used on its door: the door opens with a thud and the key is
/// used up.
void JupiterLighthouse_UseRedKey(s32 item) {
  Event_SetFlag(FLAG_RED_DOOR_OPENED);
  Event_PlaySound(SE_HEAVY_IMPACT);
  JupiterLighthouse_OpenRedDoor();
  PartyInventory_Remove(item);
}

/// The Blue Key used on its door: the door opens with a thud and the key is
/// used up.
void JupiterLighthouse_UseBlueKey(s32 item) {
  Event_SetFlag(FLAG_BLUE_DOOR_OPENED);
  Event_PlaySound(SE_HEAVY_IMPACT);
  JupiterLighthouse_OpenBlueDoor();
  PartyInventory_Remove(item);
}

/// Examining a locked door: says it is locked.
void JupiterLighthouse_TryLockedDoor(void) {
  gFieldWork->fieldMenu = 1;
  Message_ShowCentered(MsgFieldDoorLocked, 1);
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// A light's step up the beam: it circles the beam at its angle (timer) and
/// radius and rises 3 pixels with each step (userValue), twinkling from its
/// first step and fading near the floor, and starts over once above it; drawn
/// in front of the beam while the sine is negative and behind it otherwise.
void JupiterLighthouse_CircleUpTheBeam(struct Actor *actor) {
  s16 angle = actor->timer;
  s16 height = actor->userValue;

  SET_ACTOR_POSITION(
      actor, Iwram_MulFX(Trig_Cos(angle), actor->speedLimit) + toFIXED(BEAM_X),
      (height * toFIXED(3)) - toFIXED(BEAM_DEPTH), toFIXED(BEAM_Z));
  if ((s16)actor->userValue == 1)
    Actor_SelectAnimation(actor, ANIM_PARTICLE_TWINKLE);
  if ((s16)actor->userValue == BEAM_LIGHT_FADE_STEP)
    Actor_SelectAnimation(actor, ANIM_PARTICLE_FADE);
  if (actor->y.fixed > 0)
    actor->userValue = -1;
  if (Math_Sin(angle) < 0)
    ((struct FieldSprite *)actor->sprite)->priority = 1;
  else
    ((struct FieldSprite *)actor->sprite)->priority = 3;
  actor->timer += actor->acceleration;
  actor->userValue++;
}

/// Sends seven lights circling up the beam, each at its own radius, speed and
/// angle and five steps of rise apart.
void JupiterLighthouse_SendUpBeamLights(void) {
  struct Actor *actor;
  s32 i;
  s32 phase;

  i = 0;
  for (phase = 0; i != 7; i++) {
    actor = (FieldActor_Create(SPRITE_PARTICLE, 0, 0, 0));
    actor->motionFlags = 0;
    actor->priorityFlags = 0;
    actor->speedLimit = (Random16() << 2) + toFIXED(6);
    actor->acceleration = ((Random16() << 11) >> 16) + DEGtoANG(45) / 4;
    actor->update = (void (*)(void))JupiterLighthouse_CircleUpTheBeam;
    actor->timer = Random16();
    actor->userValue = phase;
    Actor_SetSpriteFlags(actor, 0);
    FieldActor_SetSpriteColor(actor, SPRITE_COLOR_RBG);
    ((struct FieldSprite *)actor->sprite)->priority = 1;
    phase += 5;
  }
}

s32 Scene_Initialize(void) {
  struct Actor *leader;
  s32 flag;
  s32 i;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  JupiterLighthouse_HideThePassage();
  if (Event_GetFlag(FLAG_RED_DOOR_OPENED))
    JupiterLighthouse_OpenRedDoor();
  if (Event_GetFlag(FLAG_BLUE_DOOR_OPENED))
    JupiterLighthouse_OpenBlueDoor();
  JupiterLighthouse_SendUpBeamLights();
  flag = Event_GetFlag(FLAG_KEEP_PARTY_POSITION);
  if (flag == 0 &&
      gMainRoutineWork.entrance ==
          JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_FROM_JUPITER_LIGHTHOUSE_ROOM_BEAM) {
    leader = Actor_Get(gMainRoutineWork.leaderActorId);
    leader->priorityFlags &= ~ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldActor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
    Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
    Actor_SetAnimationSpeed(leader, ANIMATION_STEP_NORMAL * 4);
    leader->motionFlags = flag;
    leader->y.fixed = leader->groundHeight = -toFIXED(BEAM_DEPTH);
    Event_StartScreenTransition();
    FieldEvent_Wait(10);
    for (i = 0; i < BEAM_RISE_MAX_STEPS && leader->y.fixed < 0; i++) {
      leader->groundHeight = leader->y.fixed = leader->y.fixed + toFIXED(3);
      leader->facing += TRIG_TURN * 3 / 32;
      System_Wait(2);
    }
    leader->groundHeight = leader->y.fixed = 0;
    leader->facing = FACING_SOUTH;
    leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    leader->velocityY = toFIXED(3);
    leader->gravity = toFIXED(1) / 2;
    Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);
    FieldEvent_Wait(40);
    leader->gravity = toFIXED(1);
    leader->priorityFlags |= ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
    Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_AUTOMATIC;
    Event_End();
  }
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION) &&
      gMainRoutineWork.entrance ==
          JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST)
    Actor_ArriveInRing(gMainRoutineWork.leaderActorId);
  return 0;
}

void JupiterLighthouse_LoadPalettes(s32 value);

s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  JupiterLighthouse_LoadPalettes(0);
  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED)) {
    Actor_RaiseFromCell(ACTOR_FIRST_BLOCK);
    Actor_RaiseFromCell(ACTOR_FIRST_BLOCK + 1);
    Actor_RaiseFromCell(ACTOR_FIRST_BLOCK + 2);
    Actor_RaiseFromCell(ACTOR_FIRST_BLOCK + 3);
    Actor_RaiseFromCell(ACTOR_FIRST_BLOCK + 4);
    Actor_RaiseFromCell(ACTOR_FIRST_BLOCK + 5);
    Actor_RaiseFromCell(ACTOR_LAST_BLOCKS);
    Actor_RaiseFromCell(ACTOR_LAST_BLOCKS + 1);
    Actor_RaiseFromCell(ACTOR_LAST_BLOCKS + 2);
    Actor_RaiseFromCell(ACTOR_BEAM_TOP_BLOCK);
    Actor_Get(ACTOR_BEAM_TOP_BLOCK)->gravity = toFIXED(3);
  }
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BEAM_TOP_BLOCK), 0);
  if (Event_GetFlag(FLAG_BRIDGE_BLOCK_RAISED)) {
    Actor_SetPosition(ACTOR_BRIDGE_BLOCK, 168, 520);
    Actor_RaiseFromCell(ACTOR_BRIDGE_BLOCK);
  }
  return 0;
}

#include "shared/scene_set_arrival_flags.c"

void GameFlag_SetByte(s32 flag, s32 value);
void MapGrid_CenterOnFollowedPoint(void);

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

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);
void Actor_StartScript(struct Actor *actor, const s32 *script);

#include "shared/whirlwind_update.h"

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/whirlwind_spawn.h"

// Unused here: nothing in the room sets it as an actor's update.
#include "shared/actor_emit_rising_sparks.h"

void Actor_SetSpriteColorMode(struct Actor *actor, s32 value);

// Unused here: the aerie's angels and lid are not in this room.
#include "shared/jupiter_lighthouse_place_raised_actors.h"

s32 BattleEffect_GetObjectPriority(struct Actor *actor);

#include "shared/light_sprites_draw.h"

static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add.h"

void LightSprites_SetPriority(s32 value) { gLightSpritePriority = value; }

#include "../field/actor_set_sprite_priority.c"

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];

// clang-format off
#include "shared/field_effect_duration_scripts.c"

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(JupiterLighthouseRoomLockedDoors),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_ROOM_RED_KEY, JupiterLighthouseRoomRedKey, JUPITER_LIGHTHOUSE_ROOM_RED_KEY_FROM_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH, JupiterLighthouseRoomErinyesTunicChest, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_FROM_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_NORTH),
    SCENE_EXIT(3, JupiterLighthouseRoomLockedDoors, 4),
    SCENE_EXIT(4, JupiterLighthouseRoomLockedDoors, 3),
    SCENE_EXIT(5, JupiterLighthouseRoomLockedDoors, 6),
    SCENE_EXIT(6, JupiterLighthouseRoomLockedDoors, 5),
    SCENE_EXIT(7, JupiterLighthouseRoomLockedDoors, 8),
    SCENE_EXIT(8, JupiterLighthouseRoomLockedDoors, 7),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_ROOM_M_WEST, JupiterLighthouseRoomM, JUPITER_LIGHTHOUSE_ROOM_M_FROM_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_WEST),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_ROOM_M_EAST, JupiterLighthouseRoomM, JUPITER_LIGHTHOUSE_ROOM_M_FROM_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_EAST),
    SCENE_EXIT(11, JupiterLighthouseRoomLockedDoors, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_FROM_JUPITER_LIGHTHOUSE_ROOM_BEAM),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST, JupiterLighthouseRoomErinyesTunicChest, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_FROM_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_SOUTHWEST),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHEAST, JupiterLighthouseRoomErinyesTunicChest, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_FROM_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_SOUTHEAST),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 136, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 136, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 168, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 168, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 200, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 200, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -8, 0, -8, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 520, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 520, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 520, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 520, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, BEAM_X, 0, BEAM_Z, FACING_EAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
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
    {TRIGGER_EXAMINE, 20, FLAG_RED_DOOR_OPENED, EVENT(JupiterLighthouse_TryLockedDoor)},
    {TRIGGER_EXAMINE, 21, FLAG_BLUE_DOOR_OPENED, EVENT(JupiterLighthouse_TryLockedDoor)},
    {USE_ITEM(ITEM_RED_KEY), 20, FLAG_RED_DOOR_OPENED, EVENT(JupiterLighthouse_UseRedKey)},
    {USE_ITEM(ITEM_BLUE_KEY), 21, FLAG_BLUE_DOOR_OPENED, EVENT(JupiterLighthouse_UseBlueKey)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RevealThePassage)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(JupiterLighthouse_HideThePassage)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_BRIDGE_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_BRIDGE_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_FinishMovingBlock)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 40, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LeaveByFirstCyclone)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 41, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LeaveBySecondCyclone)},
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

#undef FLAG_RED_DOOR_OPENED
#undef FLAG_BLUE_DOOR_OPENED
#undef FLAG_BRIDGE_BLOCK_RAISED

struct CellTarget;
void Leader_CenterInCell(void);
void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);

extern const s32 *gFieldEffect_DurationScripts[];

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

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY

struct CellTarget;
struct CellTargetEntry;
void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);
void JupiterLighthouse_HideThePassage(void);
void JupiterLighthouse_LeaveByFirstCyclone(void);
void JupiterLighthouse_LeaveBySecondCyclone(void);
void JupiterLighthouse_TryLockedDoor(void);
extern u32 gFieldEffect_LongScript[];

#include "shared/cracked_tile_drop_in.h"

void GameFlag_SetByte(s32 flag, s32 value);

/// Watches the leader's steps over the cracked tiles: the first step on one
/// cracks it, a second step after leaving it drops the party through, saving
/// the cell in the flag bytes for the floor below. Returns 1 on a fall.
s32 CrackedTile_Step(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 x = FIELD_CELL(actor->x.fixed);
  s32 z = FIELD_CELL(actor->z.fixed);
  struct CellTarget *puzzle = &gCrackedTiles;
  struct CellTargetEntry *tile = puzzle->entries;
  s32 i;

  for (i = 0; i < puzzle->count; i++, tile++) {
    if (tile->x == x && tile->z == z) {
      if (!Event_GetFlag(tile->flag)) {
        CrackedTile_Crack(puzzle, tile);
        Event_SetFlag(tile->flag);
        puzzle->current = i;
        break;
      }
      if (i == puzzle->current)
        break;
      puzzle->current = i;
      CrackedTile_Fall(puzzle, tile);
      GameFlag_SetByte(puzzle->flagByte, x);
      GameFlag_SetByte(puzzle->flagByte + 8, z);
      return 1;
    }
  }
  return 0;
}

/// On landing below a cracked tile, puts the leader on the cell the flag bytes
/// saved, offset by (dx, dz), and starts the screen transition.
void CrackedTile_PlaceLanding(s32 dx, s32 dz, s32 transition) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct CellTarget *work = &gCrackedTiles;
  s32 x = ((s32)FieldGameFlag_GetByte(work->flagByte));
  s32 z = ((s32)FieldGameFlag_GetByte(work->flagByte + 8));

  if (x != 0 || z != 0) {
    struct Actor *camera;

    GameFlag_SetByte(work->flagByte, 0);
    GameFlag_SetByte(work->flagByte + 8, 0);
    actor->x.fixed = FIELD_CELL_CENTER(x + dx);
    actor->z.fixed = FIELD_CELL_CENTER(z + dz);
    camera = event->camera;
    camera->x.fixed = actor->x.fixed;
    camera->z.fixed = actor->z.fixed;
    MapGrid_CenterOnFollowedPoint();
    CrackedTile_DropIn();
    gFieldWork->transition = transition;
  }
}
