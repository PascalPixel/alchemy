/// @file
/// The level of Jupiter Lighthouse above its central room, with the red and
/// blue locked doors that lead out to the two towers. Felix first comes up here
/// by the Cyclone teleporter in front of the red door, at cell (9, 22), and the
/// Red Key opens it (JupiterLighthouse_UseRedKey); the teleporter in front of
/// the blue door, at cell (41, 22), only takes him back down
/// (JupiterLighthouse_LeaveByRedDoorCyclone,
/// JupiterLighthouse_LeaveByBlueDoorCyclone), and the Blue Key opens that door
/// (JupiterLighthouse_UseBlueKey). In the room at the back, Reveal shows the
/// hidden floor across to the jumping puzzle room, and once the lighthouse is
/// activated a hidden block among six hovering ones, the way to the Psy
/// Crystal (JupiterLighthouse_RevealThePassage). Further south, the beam of
/// light from the central room rises through the floor, lights circling up it
/// (JupiterLighthouse_SendUpBeamLights), and brings Felix up among a row of
/// hovering blocks; the grey block in the pit west of them, Moved one cell
/// east, rises into line with the row, the way to the Potion's chest
/// (JupiterLighthouse_FinishMovingBlock).

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
#include "system/main_routine_state.h"
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
#include "window/window.h"
#include "scene/lighthouse_palettes.h"

/// Set as each locked door opens to its key.
#define FLAG_RED_DOOR_OPENED 0xa07
#define FLAG_BLUE_DOOR_OPENED 0xa08
/// Set once Move has set the bridge block on its cell, where it rises to bridge
/// the gap.
#define FLAG_BRIDGE_BLOCK_RAISED 0xa06

/// The bridge block's cell, at the west end of the row of hovering blocks
/// round the beam, where it rises into line with them.
enum { BRIDGE_BLOCK_CELL_X = 10, BRIDGE_BLOCK_CELL_Z = 32 };
/// The level's own cell behaviors.
enum JupiterLighthouseRoomLockedDoorsCellBehavior {
  CELL_BEHAVIOR_RED_DOOR = 20,  ///< cell (10, 20)
  CELL_BEHAVIOR_BLUE_DOOR = 21, ///< cell (40, 20)
  /// The teleporter pads in front of the red and the blue door, cells (9, 22)
  /// and (41, 22).
  CELL_BEHAVIOR_RED_DOOR_TELEPORTER = 40,
  CELL_BEHAVIOR_BLUE_DOOR_TELEPORTER
};

/// The room's blocks, numbering the rows of gActorPlacements.
enum JupiterLighthouseBlockId {
  /// The first of the six blocks that rise once the lighthouse is activated.
  ACTOR_FIRST_BLOCK = ACTOR_FIRST_PLACED,
  /// The block Reveal shows on the hidden passage.
  ACTOR_HIDDEN_BLOCK = ACTOR_FIRST_PLACED + 6,
  /// The block in the pit west of the beam's row, which Move sets on its cell
  /// to bridge the gap.
  ACTOR_BRIDGE_BLOCK,
  /// The first of three more that rise once the lighthouse is activated.
  ACTOR_LAST_BLOCKS,
  /// The unseen block at the top of the beam of light, (232, 520), in the
  /// row of hovering blocks: once the lighthouse is activated, Felix carried
  /// up the beam stands on it, nothing drawn under him.
  ACTOR_BEAM_TOP_BLOCK = ACTOR_LAST_BLOCKS + 3
};

/// The beam of light from the room below: where it stands, and how far below
/// the floor its lights, and the party it carries up, start rising.
enum { BEAM_X = 232, BEAM_Z = 520, BEAM_DEPTH = 120 };
/// The most steps the beam lifts the party, 3 pixels each; the floor stops it
/// first.
#define BEAM_RISE_MAX_STEPS 90
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
/// The cells CrackedTile_Step watches, and the cell it copies over the one the
/// leader steps on.
struct CellTarget gCrackedTiles BSS_DATA;
void JupiterLighthouse_HideThePassage(void);
void JupiterLighthouse_LeaveByRedDoorCyclone(void);
void JupiterLighthouse_LeaveByBlueDoorCyclone(void);
void JupiterLighthouse_TryLockedDoor(void);
void LightSprites_InitA(void);
void MapGrid_SetCellHeight();

void Actor_ArriveInRing(s32 id);
void MapGrid_SetCellAttributesAt();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// Felix casts Reveal in the room at the back: a hidden stretch of floor shows
/// at cell (19, 10), and he can jump across the gap there. Once the lighthouse
/// is activated, Reveal also shows a hidden block hovering at cell (31, 10),
/// in the middle of the six hovering blocks, which he can jump across too.
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

/// As Reveal wears off, and on entering the level: the hidden floor and the
/// hidden block vanish again.
void JupiterLighthouse_HideThePassage(void) {
  MapGrid_CopyCellsLayered(16, 10, 1, 4, 19, 10);
  MapGrid_CopyCellAttributes(16, 10, 1, 1, 19, 10);
  Actor_SetPosition(ACTOR_HIDDEN_BLOCK, 0, 0);
  MapGrid_CopyCellAttributes(30, 10, 1, 1, 31, 10);
}

/// Nothing visible happens as Move takes hold of the grey block in the pit west
/// of the beam's row; its sprite's drawing order is set
/// (`Actor_SetSpritePriority`) for the slide.
void JupiterLighthouse_StartMovingBlock(s32 unused, s32 id) {
  Actor_SetSpritePriority(id, 2);
}

/// The grey block in the pit west of the beam's row, Moved one cell east to
/// cell (10, 32), rises out of the pit and hovers there for good in line with
/// the others, a step for Felix on the way to the Potion's chest. Moved
/// anywhere else, it stays in the pit.
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

/// Felix casts Cyclone on the purple teleporter pad in front of the red door,
/// cell (9, 22): the party goes back down to the south hall below, by its west
/// side.
void JupiterLighthouse_LeaveByRedDoorCyclone(void) {
  Import_EventSetExitRequest(
      JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST);
}

/// Felix casts Cyclone on the purple teleporter pad in front of the blue door,
/// cell (41, 22): the party goes back down to the south hall below, by its east
/// side.
void JupiterLighthouse_LeaveByBlueDoorCyclone(void) {
  Import_EventSetExitRequest(
      JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHEAST);
}

/// Shows the red door at cell (10, 20) open and lets the party through it.
void JupiterLighthouse_OpenRedDoor(void) {
  MapGrid_CopyCellsLayered(73, 18, 1, 4, 74, 18);
  MapGrid_CopyCellAttributes(10, 9, 1, 1, 10, 20);
}

/// Shows the blue door at cell (40, 20) open and lets the party through it.
void JupiterLighthouse_OpenBlueDoor(void) {
  MapGrid_CopyCellsLayered(103, 18, 1, 4, 104, 18);
  MapGrid_CopyCellAttributes(40, 9, 1, 1, 40, 20);
}

/// Felix uses the Red Key at the red door, cell (10, 20), north of the
/// teleporter pad: the door opens with a heavy thud and the key is gone.
void JupiterLighthouse_UseRedKey(s32 item) {
  Event_SetFlag(FLAG_RED_DOOR_OPENED);
  Event_PlaySound(SE_HEAVY_IMPACT);
  JupiterLighthouse_OpenRedDoor();
  PartyInventory_Remove(item);
}

/// Felix uses the Blue Key at the blue door, cell (40, 20), north of the
/// teleporter pad: the door opens with a heavy thud and the key is gone.
void JupiterLighthouse_UseBlueKey(s32 item) {
  Event_SetFlag(FLAG_BLUE_DOOR_OPENED);
  Event_PlaySound(SE_HEAVY_IMPACT);
  JupiterLighthouse_OpenBlueDoor();
  PartyInventory_Remove(item);
}

/// Felix checks the red or the blue door before it is open: "The door's
/// locked."
void JupiterLighthouse_TryLockedDoor(void) {
  // As A pressed with nothing ahead does, this asks for the field menu, which
  // the field opens once the event is over.
  gFieldState->fieldMenu = 1;
  // "The door's locked."
  Message_ShowCentered(MsgFieldDoorLocked, FIELD_MESSAGE_ZOOM);
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
    actor->update = (ObjectUpdate)JupiterLighthouse_CircleUpTheBeam;
    actor->timer = Random16();
    actor->userValue = phase;
    Actor_SetSpriteFlags(actor, 0);
    FieldActor_SetSpriteColor(actor, SPRITE_COLOR_RBG);
    ((struct FieldSprite *)actor->sprite)->priority = 1;
    phase += 5;
  }
}

/// Sets up the level as the party enters: the hidden floor hidden, each opened
/// door open, the lights sent up the beam, and Felix, come from the central
/// room, carried spinning up the beam to land among the hovering blocks, or,
/// come by Cyclone from the south hall, arriving in a ring. Returns 0.
s32 Scene_Initialize(void) {
  struct Actor *leader;
  s32 flag;
  s32 i;

  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  JupiterLighthouse_HideThePassage();
  if (Event_GetFlag(FLAG_RED_DOOR_OPENED))
    JupiterLighthouse_OpenRedDoor();
  if (Event_GetFlag(FLAG_BLUE_DOOR_OPENED))
    JupiterLighthouse_OpenBlueDoor();
  JupiterLighthouse_SendUpBeamLights();
  flag = Event_GetFlag(FLAG_KEEP_PARTY_POSITION);
  if (flag == 0 &&
      gMainRoutineState.entrance ==
          JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_FROM_JUPITER_LIGHTHOUSE_ROOM_BEAM) {
    leader = Actor_Get(gMainRoutineState.leaderActorId);
    leader->priorityFlags &= ~ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    FieldActor_SetSpritePriority(gMainRoutineState.leaderActorId, 1);
    Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_WALK);
    Actor_SetAnimationSpeed(leader, ANIM_STEP_NORMAL * 4);
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
    Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_STAND);
    FieldEvent_Wait(40);
    leader->gravity = toFIXED(1);
    leader->priorityFlags |= ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
    Actor_Get(gMainRoutineState.leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_AUTOMATIC;
    Event_End();
  }
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION) &&
      gMainRoutineState.entrance ==
          JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST)
    Actor_ArriveInRing(gMainRoutineState.leaderActorId);
  return 0;
}

/// Sets the scene's arrival flags and loads the lighthouse's palettes; lets
/// Felix walk onto the blocks, raises them once the lighthouse is activated,
/// and puts the bridge block back in line. Returns 0.
s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  JupiterLighthouse_LoadPalettes(LIGHTHOUSE_PALETTES_BY_FLAG);
  Actor_Get(gMainRoutineState.leaderActorId)->priorityFlags |=
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
#include "shared/actor_emit_smoke.h"

void Actor_SetSpriteColorMode(struct Actor *actor, s32 value);

// Unused here: the aerie's angels and lid are not in this room.
#include "shared/jupiter_lighthouse_place_raised_actors.h"

s32 Actor_GetSpritePriority(struct Actor *actor);

#include "shared/light_sprites_draw.h"

static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add.h"

/// Sets the priority the lit actors' sprites are drawn at.
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
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_TERRACES_WEST, JupiterLighthouseTerraces, JUPITER_LIGHTHOUSE_TERRACES_FROM_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_WEST),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_TERRACES_EAST, JupiterLighthouseTerraces, JUPITER_LIGHTHOUSE_TERRACES_FROM_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_EAST),
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
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_ROOM_RED_KEY, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_ROOM_RED_KEY},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_TERRACES_WEST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_TERRACES_WEST},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_TERRACES_EAST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_TERRACES_EAST},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_RED_DOOR, FLAG_RED_DOOR_OPENED, EVENT(JupiterLighthouse_TryLockedDoor)},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_BLUE_DOOR, FLAG_BLUE_DOOR_OPENED, EVENT(JupiterLighthouse_TryLockedDoor)},
    {USE_ITEM(ITEM_RED_KEY), CELL_BEHAVIOR_RED_DOOR, FLAG_RED_DOOR_OPENED, EVENT(JupiterLighthouse_UseRedKey)},
    {USE_ITEM(ITEM_BLUE_KEY), CELL_BEHAVIOR_BLUE_DOOR, FLAG_BLUE_DOOR_OPENED, EVENT(JupiterLighthouse_UseBlueKey)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RevealThePassage)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(JupiterLighthouse_HideThePassage)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_BRIDGE_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_BRIDGE_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_FinishMovingBlock)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_RED_DOOR_TELEPORTER, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LeaveByRedDoorCyclone)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_BLUE_DOOR_TELEPORTER, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LeaveByBlueDoorCyclone)},
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

void Leader_CenterInCell(void);
void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);

extern const s32 *gFieldEffect_DurationScripts[];

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#include "../field/field_effect_spawn.c"

#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/cracked_tile_mask.c"

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_cell.h"

#include "shared/cracked_tile_crack.h"

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY

#include "shared/cracked_tile_drop_in.h"

void GameFlag_SetByte(s32 flag, s32 value);

/// Watches the leader's steps over the cracked tiles: the first step on one
/// cracks it, a second step after leaving it drops the party through, saving
/// the cell in the flag bytes for the floor below. Returns 1 on a fall.
s32 CrackedTile_Step(void) {
  struct Actor *actor = Actor_Get(gMainRoutineState.leaderActorId);
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
  struct Actor *actor = Actor_Get(gMainRoutineState.leaderActorId);
  struct FieldState *event = Ram_HeapSlots->fieldState;
  struct CellTarget *state = &gCrackedTiles;
  s32 x = ((s32)FieldGameFlag_GetByte(state->flagByte));
  s32 z = ((s32)FieldGameFlag_GetByte(state->flagByte + 8));

  if (x != 0 || z != 0) {
    struct Actor *camera;

    GameFlag_SetByte(state->flagByte, 0);
    GameFlag_SetByte(state->flagByte + 8, 0);
    actor->x.fixed = FIELD_CELL_CENTER(x + dx);
    actor->z.fixed = FIELD_CELL_CENTER(z + dz);
    camera = event->camera;
    camera->x.fixed = actor->x.fixed;
    camera->z.fixed = actor->z.fixed;
    MapGrid_CenterOnFollowedPoint();
    CrackedTile_DropIn();
    gFieldState->transition = transition;
  }
}
