/// @file
/// Jupiter Lighthouse's central room, and north of it the room of the five
/// grey blocks. In the middle of the central room a beam of light rises from a
/// purple pedestal between four winged statues, pink sparks circling it. Felix
/// comes back here from the east ledge: he pushes the energy block off the
/// ledge, drops after it and pushes it into the gap at the head of the path at
/// cell (20, 39), and the path lights up to a purple Hover pad
/// (JupiterLighthouse_DropEnergyBlock). Hovering from there, he floats west
/// into the beam, which carries him up to the room of the locked doors
/// (JupiterLighthouse_RideTheBeam). Two purple Cyclone pads by the doorway
/// north of the beam whirl him up onto the high ledge, towards the stairs, or
/// back down (JupiterLighthouse_CycloneLiftsLeader,
/// JupiterLighthouse_CycloneDropsLeader). In the north room, once the
/// lighthouse is activated, four of the five grey blocks hover over their
/// sockets, and the rightmost, Moved one cell west onto the empty socket, rises
/// to join them as a bridge to the room's east side
/// (JupiterLighthouse_RaiseSocketBlock).

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
#include "field/scene_tables.h"
#include "field/field_sequence_event.h"
#include "field/event_commands.h"
#include "field/cell_target.h"
#include "scene/scene.h"
#include "system/main_routine_state.h"
#include "field/event_inline.h"
#include "field/actor.h"
#include "field/field_sprite.h"
#include "system/ram_buffer.h"
#include "field/field_event.h"
#include "system/iwram_call.h"
#include "macros.h"
#include "system/io_reg.h"
#include "system/io_write.h"
#include "field/call.h"
#include "shared/jupiter_lighthouse_aerie.h"
#include "system/dma.h"
#include "system/frame.h"
#include "system/irq.h"
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

/// The energy block lies in its gap, lighting the path.
#define FLAG_BEAM_ROOM_ENERGY_BLOCK_IN_GAP 0xa02
/// The fifth grey block, Moved onto the empty socket, has risen to hover over
/// it.
#define FLAG_BEAM_ROOM_SOCKET_BLOCK_RAISED 0xa03

/// The room's actors, numbering the rows of gActorPlacements.
enum JupiterLighthouseRoomBeamActorId {
  /// The four grey blocks that hover over their sockets once the lighthouse is
  /// activated, west to east.
  ACTOR_FIRST_HOVERING_BLOCK = ACTOR_FIRST_PLACED,
  ACTOR_LAST_HOVERING_BLOCK = ACTOR_FIRST_HOVERING_BLOCK + 3,
  /// The fifth, east of the row's empty socket.
  ACTOR_SOCKET_BLOCK,
  /// The four winged statues round the beam.
  ACTOR_NORTHWEST_STATUE,
  ACTOR_NORTHEAST_STATUE,
  ACTOR_SOUTHWEST_STATUE,
  ACTOR_SOUTHEAST_STATUE,
  /// The block on the east ledge, which lights the path from the gap below.
  ACTOR_ENERGY_BLOCK
};

/// The gap at the head of the path below the east ledge, which the energy block
/// fills.
#define BEAM_ROOM_GAP_CELL_X 20
#define BEAM_ROOM_GAP_CELL_Z 39
/// The empty socket at the east end of the grey blocks' row.
#define BEAM_ROOM_EMPTY_SOCKET_CELL_X 18
#define BEAM_ROOM_EMPTY_SOCKET_CELL_Z 6
/// The room's own cell behaviors.
enum JupiterLighthouseRoomBeamCellBehavior {
  /// The central room's east doorway, whose cell answers exit 4 (to the north
  /// room's east end).
  CELL_BEHAVIOR_JUPITER_LIGHTHOUSE_EAST_DOORWAY = 4,
  /// The beam's floor, cells (12, 40) to (14, 42).
  CELL_BEHAVIOR_BEAM = 20
};
/// The north half's west end, where the beam floor's west doorway leads.
#define ENTRANCE_NORTH_HALF_WEST 3
/// The pink sparks that circle the beam.
#define BEAM_PARTICLES 22
/// The steps the beam lifts a leader, 3 pixels every other frame, before the
/// room of locked doors takes over.
#define BEAM_RIDE_STEPS 110

void Actor_Release(struct Actor *actor);

void Actor_RaiseFromCell(s32 id);

void LightSprites_InitB(void);

void LightSprites_Add(s32 id, s32 value);

void FadeOverlay_FillTiles(s32 a);

void JupiterLighthouse_LoadPalettes(s32 mode);

struct EffectSprite gJupiterLighthouse_GlowSprite BSS_DATA ALIGNED(16);
/// Where the glow over the Cyclone pad in use stands.
s32 gJupiterLighthouse_GlowPosition[3] BSS_DATA;

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

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);

s32 Position_FlatDistance(FIXED *a, FIXED *b);

s32 BattleEffect_GetObjectPriority(struct Actor *actor);

void Leader_CenterInCell(void);

/// The Cyclone pads' cells, which are also the triggers their Psynergy raises
/// (FieldState.raisedTrigger): the west pad at cell (9, 36) and the east one at
/// cell (16, 35), with Felix below the ledge or (as copied in for the ledge's
/// layer) on it.
enum {
  TRIGGER_CYCLONE_UP_WEST = 30,
  TRIGGER_CYCLONE_UP_EAST,
  TRIGGER_CYCLONE_DOWN_WEST,
  TRIGGER_CYCLONE_DOWN_EAST
};

void JupiterLighthouse_CycloneUpOnWestPad(void);
void JupiterLighthouse_CycloneUpOnEastPad(void);

void JupiterLighthouse_CycloneDownOnEastPad(void);

void JupiterLighthouse_RideTheBeam(void);

void JupiterLighthouse_StartHover(void);

void JupiterLighthouse_EndHover(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

u8 MapGrid_SetAttributeBAt(s32 x, s32 z, s32 layer, s32 value);
void EffectSprite_InitWithFadeOverlayBlended(const void *script, s32 a, s32 b,
                                             s32 c);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// Lets Cyclone on either pad take Felix the other way from where he stands:
/// on the high ledge it carries him down, below it lifts him up.
void JupiterLighthouse_SetCyclonePadsForLeader(void) {
  struct Actor *actor;

  actor = Actor_Get(gMainRoutineState.leaderActorId);
  if (actor->layer == MAPGRID_LAYER_MIDDLE) {
    FieldMapGrid_SetCellBehavior(toFIXED(144), toFIXED(576),
                                 MAPGRID_LAYER_OVERHEAD,
                                 CELL_BEHAVIOR_CYCLONE_CARRY);
    FieldMapGrid_SetCellBehavior(toFIXED(256), toFIXED(560),
                                 MAPGRID_LAYER_OVERHEAD,
                                 CELL_BEHAVIOR_CYCLONE_CARRY);
  } else {
    FieldMapGrid_SetCellBehavior(toFIXED(144), toFIXED(576),
                                 MAPGRID_LAYER_OVERHEAD,
                                 CELL_BEHAVIOR_CYCLONE_LIFT);
    FieldMapGrid_SetCellBehavior(toFIXED(256), toFIXED(560),
                                 MAPGRID_LAYER_OVERHEAD,
                                 CELL_BEHAVIOR_CYCLONE_LIFT);
  }
}

/// A task: draws the glow over the Cyclone pad in use.
void JupiterLighthouse_DrawGlow(void) {
  EffectSprite_DrawAtWorldXYZ(
      (s32)&gJupiterLighthouse_GlowSprite, gJupiterLighthouse_GlowPosition[0],
      gJupiterLighthouse_GlowPosition[1], gJupiterLighthouse_GlowPosition[2]);
}

/// Felix casts Cyclone standing on a purple pad up on the high ledge north of
/// the beam (`kind` says the west or east pad): he sinks, and a whirlwind spins
/// him down to the pad on the floor below.
void JupiterLighthouse_CycloneDropsLeader(s32 kind) {
  s32 owner = gMainRoutineState.leaderActorId;
  struct Actor *leader = Actor_Get(owner);
  struct Actor *effect;
  s32 frame;
  s32 *target;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  target = gJupiterLighthouse_GlowPosition;
  if (kind == TRIGGER_CYCLONE_DOWN_WEST) {
    target[0] = toFIXED(136);
    target[1] = toFIXED(144);
    target[2] = toFIXED(592);
  } else {
    target[0] = toFIXED(248);
    target[1] = toFIXED(144);
    target[2] = toFIXED(576);
  }
  {
    s32 priority = TASK_PASS_FRAME;

    Task_AddFrame(JupiterLighthouse_DrawGlow, priority);
  }
  EventCamera_FollowActor(owner, 1);
  for (frame = 0; frame < 36; frame++) {
    leader->y.fixed += -toFIXED(1);
    System_Wait(1);
  }
  leader->scaleX = FIXED_ONE;
  leader->scaleY = FIXED_ONE;
  effect = Actor_Spawn(SPRITE_WHIRLWIND, leader->x.fixed, leader->y.fixed,
                       leader->z.fixed);
  Actor_SelectAnimation(effect, 2);
  effect->scaleX = FIXED_ONE * 3 / 2;
  effect->scaleY = FIXED_ONE;
  ((struct FieldSprite *)effect->sprite)->priority = 1;
  while (leader->y.fixed > toFIXED(32)) {
    leader->y.fixed += -toFIXED(1);
    leader->facing += DEGtoANG(45);
    effect->x.fixed = leader->x.fixed;
    effect->y.fixed = leader->y.fixed + -toFIXED(6);
    effect->z.fixed = leader->z.fixed;
    System_Wait(1);
  }
  Actor_SelectAnimation(effect, 6);
  for (frame = 0; frame < 15; frame++) {
    leader->facing += DEGtoANG(45);
    System_Wait(1);
  }
  Actor_Release(effect);
  Actor_SetSpriteFlags(Actor_Get(owner), SPRITE_FLAG_SHADOW);
  leader->facing = FACING_SOUTH;
  leader->layer = MAPGRID_LAYER_GROUND;
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  FieldTask_Remove(JupiterLighthouse_DrawGlow);
  JupiterLighthouse_SetCyclonePadsForLeader();
  Event_End();
}

/// Felix casts Cyclone standing on a purple pad north of the beam's winged
/// statues, the west one at cell (9, 36) or the east one at cell (16, 35)
/// (`kind` says which): a whirlwind spins him up the wall, the camera rising
/// with him, and he vanishes into it and reappears small on the pad on the
/// high ledge 144 pixels above, growing back to his size.
void JupiterLighthouse_CycloneLiftsLeader(s32 kind) {
  s32 owner = gMainRoutineState.leaderActorId;
  struct Actor *leader = Actor_Get(owner);
  struct Actor *effect;
  s32 *target;
  s32 frame;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  target = gJupiterLighthouse_GlowPosition;
  if (kind == TRIGGER_CYCLONE_UP_WEST) {
    target[0] = toFIXED(136);
    target[1] = toFIXED(144);
    target[2] = toFIXED(592);
  } else {
    target[0] = toFIXED(248);
    target[1] = toFIXED(144);
    target[2] = toFIXED(576);
  }
  {
    s32 priority = TASK_PASS_FRAME;

    Task_AddFrame(JupiterLighthouse_DrawGlow, priority);
  }
  EventCamera_FollowActor(owner, 1);
  leader->scaleX = FIXED_ONE;
  leader->scaleY = FIXED_ONE;
  effect = Actor_Spawn(SPRITE_WHIRLWIND, leader->x.fixed, leader->y.fixed,
                       leader->z.fixed);
  Actor_SelectAnimation(effect, 2);
  effect->scaleX = FIXED_ONE * 3 / 2;
  effect->scaleY = FIXED_ONE;
  ((struct FieldSprite *)effect->sprite)->priority = 1;
  while (leader->y.fixed < toFIXED(108)) {
    leader->y.fixed += toFIXED(1);
    leader->facing += DEGtoANG(45);
    effect->x.fixed = leader->x.fixed;
    effect->y.fixed = leader->y.fixed + -toFIXED(6);
    effect->z.fixed = leader->z.fixed;
    System_Wait(1);
  }
  leader->scaleX = 0;
  effect->scaleX = 0;
  while (leader->y.fixed < toFIXED(144)) {
    leader->y.fixed += toFIXED(1);
    effect->y.fixed += toFIXED(1);
    System_Wait(1);
  }
  effect->scaleX = FIXED_ONE / 2;
  effect->scaleY = FIXED_ONE * 3 / 4;
  for (frame = 0; frame < 32; frame++) {
    effect->scaleX += FIXED_ONE / 32;
    effect->scaleY += FIXED_ONE / 128;
    System_Wait(1);
  }
  effect->scaleX = FIXED_ONE * 3 / 2;
  effect->scaleY = FIXED_ONE;
  Actor_SelectAnimation(effect, 6);
  leader->scaleX = FIXED_ONE * 3 / 8;
  leader->scaleY = FIXED_ONE * 3 / 8;
  for (frame = 0; frame < 20; frame++) {
    leader->facing += DEGtoANG(45);
    leader->scaleX += FIXED_ONE / 32;
    leader->scaleY = leader->scaleX;
    System_Wait(1);
  }
  Actor_Release(effect);
  Actor_SetSpriteFlags(Actor_Get(owner), SPRITE_FLAG_SHADOW);
  leader->facing = FACING_SOUTH;
  leader->layer = MAPGRID_LAYER_MIDDLE;
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  leader->y.fixed = toFIXED(144);
  leader->groundHeight = toFIXED(144);
  FieldTask_Remove(JupiterLighthouse_DrawGlow);
  JupiterLighthouse_SetCyclonePadsForLeader();
  Event_End();
}

/// Felix casts Cyclone on the west pad below the ledge, cell (9, 36). Nothing
/// is seen yet; the whirlwind that lifts him follows
/// (JupiterLighthouse_CycloneLiftsLeader).
void JupiterLighthouse_CycloneUpOnWestPad(void) {
  struct FieldState *event = Ram_HeapSlots->fieldState;
  struct FieldEffectState *effect = Ram_HeapSlots->fieldEffectState;

  event->raisedTrigger = TRIGGER_CYCLONE_UP_WEST;
  effect->childOption = 1;
}

/// Felix casts Cyclone on the east pad below the ledge, cell (16, 35). Nothing
/// is seen yet; the whirlwind that lifts him follows.
void JupiterLighthouse_CycloneUpOnEastPad(void) {
  struct FieldState *event = Ram_HeapSlots->fieldState;
  struct FieldEffectState *effect = Ram_HeapSlots->fieldEffectState;

  event->raisedTrigger = TRIGGER_CYCLONE_UP_EAST;
  effect->childOption = 1;
}

/// Felix casts Cyclone on the west pad up on the ledge. Nothing is seen yet;
/// the whirlwind that takes him down follows
/// (JupiterLighthouse_CycloneDropsLeader).
void JupiterLighthouse_CycloneDownOnWestPad(void) {
  gFieldState->raisedTrigger = TRIGGER_CYCLONE_DOWN_WEST;
}

/// Felix casts Cyclone on the east pad up on the ledge. Nothing is seen yet;
/// the whirlwind that takes him down follows.
void JupiterLighthouse_CycloneDownOnEastPad(void) {
  gFieldState->raisedTrigger = TRIGGER_CYCLONE_DOWN_EAST;
}

/// Felix floats into the beam of light in the middle of the room while
/// Hovering, over cells (12, 40) to (14, 42): he is drawn to its centre and
/// rises up it, spinning, to the room of the locked doors. Walking onto the
/// beam's floor without Hover (less than 32 pixels up) does nothing.
void JupiterLighthouse_RideTheBeam(void) {
  struct Actor *actor = Actor_Get(gMainRoutineState.leaderActorId);
  s32 i;
  s32 angle;
  FIXED y;

  y = actor->y.fixed;
  if (y >= toFIXED(32)) {
    Actor_SetSpeed(gMainRoutineState.leaderActorId, 2);
    FieldActor_SetSpritePriority(gMainRoutineState.leaderActorId, 1);
    Actor_WalkTo(gMainRoutineState.leaderActorId, 216, 680);
    Actor_SetAnimationSpeed(actor, ANIM_STEP_NORMAL * 4);
    actor->motionFlags = 0;
    for (i = 0; i < BEAM_RIDE_STEPS; i++) {
      actor->y.fixed += toFIXED(3);
      angle = actor->facing;
      angle += TRIG_TURN * 3 / 32;
      actor->facing = angle;
      System_Wait(2);
    }
    FieldEvent_SetExitRequest(
        JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS);
  }
}

/// Nothing visible happens as Felix starts to push the energy block on the east
/// ledge, by walking into it or with Move. The block stops following the
/// ground while it slides, and it and Felix are raised a step in the sprites'
/// drawing order (`FieldActor_SetSpritePriority`) for the push.
void JupiterLighthouse_PushEnergyBlock(s32 unused, s32 id) {
  Actor_Get(id)->motionFlags = 0;
  FieldActor_SetSpritePriority(id, 1);
  FieldActor_SetSpritePriority(gMainRoutineState.leaderActorId, 1);
}

/// The energy block lands with a thump when Felix pushes it off the east
/// ledge. Pushed on into the gap at the head of the path below, cell (20, 39),
/// it fills the gap and the path lights up a stretch at a time, with a sound
/// each, down to the purple Hover pad at its end, cell (21, 41).
void JupiterLighthouse_DropEnergyBlock(s32 unused, s32 id) {
  struct Actor *actor = Actor_Get(id);
  struct MainRoutineState *party;
  s32 frame;

  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Actor_Get(id)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  party = &gMainRoutineState;
  Actor_Get(party->leaderActorId)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  if (FIELD_CELL(actor->x.fixed) == BEAM_ROOM_GAP_CELL_X &&
      FIELD_CELL(actor->z.fixed) == BEAM_ROOM_GAP_CELL_Z)
    FieldActor_SetSpritePriority(id, 3);
  if (actor->groundHeight !=
      Map_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed)) {
    System_Wait(2);
    for (frame = 0; frame <= 29 && (actor->y.fixed > actor->groundHeight ||
                                    actor->velocityY != 0);
         frame++)
      System_Wait(1);
    Event_PlaySound(SE_THUMP);
  }
  if (FIELD_CELL(actor->x.fixed) == BEAM_ROOM_GAP_CELL_X &&
      FIELD_CELL(actor->z.fixed) == BEAM_ROOM_GAP_CELL_Z) {
    Event_SetFlag(FLAG_BEAM_ROOM_ENERGY_BLOCK_IN_GAP);
    Actor_SetSpriteFlags(Actor_Get(id), 0);
    FieldActor_SetSpritePriority(id, 3);
    Actor_SetPosition(id, 0, 0);
    // The filled gap from just past the room's east edge, then the lit path
    // down from it on the middle layer (from column 64), to the Hover pad.
    MapGrid_CopyCellAttributes(28, 39, 1, 1, BEAM_ROOM_GAP_CELL_X,
                               BEAM_ROOM_GAP_CELL_Z);
    MapGrid_CopyCellsLayered(28, 39, 1, 1, BEAM_ROOM_GAP_CELL_X,
                             BEAM_ROOM_GAP_CELL_Z);
    MapGrid_CopyCellsLayered(92, 39, 1, 1, 84, 39);
    FieldEvent_Wait(30);
    Event_PlaySound(SE_MAP_TRANSITION);
    MapGrid_CopyCellsLayered(92, 40, 1, 1, 84, 40);
    FieldEvent_Wait(20);
    Event_PlaySound(SE_MAP_TRANSITION);
    MapGrid_CopyCellsLayered(92, 41, 1, 1, 84, 41);
    FieldEvent_Wait(20);
    Event_PlaySound(SE_MAP_TRANSITION);
    MapGrid_CopyCellsLayered(93, 41, 1, 1, 85, 41);
    FieldMapGrid_SetCellBehavior(toFIXED(336), toFIXED(656),
                                 MAPGRID_LAYER_OVERHEAD, CELL_BEHAVIOR_HOVER);
  }
}

/// Nothing visible happens as Move takes hold of the rightmost of the five
/// grey blocks in the north room; its sprite's drawing order is set
/// (`Actor_SetSpritePriority`) for the slide onto the empty socket.
void JupiterLighthouse_MoveSocketBlock(s32 unused, s32 id) {
  Actor_SetSpritePriority(id, 2);
}

/// The rightmost grey block in the north room, Moved one cell west onto the
/// empty socket at cell (18, 6) once the lighthouse is activated, rises to
/// hover in line with the other four, a bridge to the room's east side.
void JupiterLighthouse_RaiseSocketBlock(s32 unused, s32 id) {
  struct Actor *actor = Actor_Get(id);
  s32 frame;

  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED) &&
      FIELD_CELL(actor->x.fixed) == BEAM_ROOM_EMPTY_SOCKET_CELL_X &&
      FIELD_CELL(actor->z.fixed) == BEAM_ROOM_EMPTY_SOCKET_CELL_Z) {
    actor->motionFlags = 0;
    for (frame = 0; frame < 16; frame++) {
      actor->y.fixed += toFIXED(2);
      System_Wait(1);
    }
    Actor_RaiseFromCell(id);
    Event_SetFlag(FLAG_BEAM_ROOM_SOCKET_BLOCK_RAISED);
  }
}

/// Nothing visible happens when Felix starts to Hover anywhere in the room;
/// the central room's east doorway, cell (21, 37), is walled off while he
/// floats, so that he cannot drift out through it.
void JupiterLighthouse_StartHover(void) {
  struct MainRoutineState *party = &gMainRoutineState;

  if (party->movementMode == LEADER_MOVEMENT_HOVER)
    FieldMapGrid_SetCellBehavior(toFIXED(336), toFIXED(592),
                                 MAPGRID_LAYER_GROUND, CELL_BEHAVIOR_WALL);
}

/// Nothing visible happens when Felix's Hover ends; the central room's east
/// doorway leads out again.
void JupiterLighthouse_EndHover(void) {
  MapGrid_SetCellBehavior(toFIXED(336), toFIXED(592), MAPGRID_LAYER_GROUND,
                          CELL_BEHAVIOR_JUPITER_LIGHTHOUSE_EAST_DOORWAY);
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// A spark's update: it circles the beam at its own speed, rising until it
/// reaches the top and starts again from the floor, in front of the beam on
/// the near half of the circle and behind it on the far half.
void JupiterLighthouse_CircleTheBeam(struct Actor *actor) {
  s16 angle = actor->timer;
  s16 height = actor->userValue;

  SET_ACTOR_POSITION(
      actor, Iwram_MulFX(Trig_Cos(angle), actor->speedLimit) + toFIXED(214),
      (height * toFIXED(3)) + toFIXED(8), toFIXED(680));
  // Read signed, as the phase is above; the field is declared unsigned.
  if ((s16)actor->userValue == 0)
    Actor_SelectAnimation(actor, 7);
  if (actor->y.fixed > toFIXED(340))
    actor->userValue = -1;
  if (Math_Sin(angle) < 0)
    ((struct FieldSprite *)actor->sprite)->priority = 1;
  else
    ((struct FieldSprite *)actor->sprite)->priority = 2;
  actor->timer += actor->acceleration;
  actor->userValue++;
}

/// Places the pink sparks round the beam, each circling it at a random speed
/// and radius, each risen five frames' climb above the one before.
void JupiterLighthouse_SpawnBeamParticles(void) {
  s32 i;
  s32 phase;
  struct Actor *actor;

  for (i = 0, phase = 0; i != BEAM_PARTICLES; i++, phase += 5) {
    actor = (FieldActor_Create(SPRITE_PARTICLE, 0, 0, 0));
    actor->motionFlags = 0;
    actor->priorityFlags = 0;
    actor->speedLimit = (Random16() << 2) + toFIXED(6);
    actor->acceleration = ((Random16() << 11) >> 16) + DEGtoANG(45) / 4;
    actor->update = JupiterLighthouse_CircleTheBeam;
    actor->timer = Random16();
    actor->userValue = phase;
    Actor_SetSpriteFlags(actor, 0);
    FieldActor_SetSpriteColor(actor, SPRITE_COLOR_RBG);
    ((struct FieldSprite *)actor->sprite)->priority = 1;
  }
}

/// Sets up the room: the beam and its sparks, the energy block (glowing until
/// it lies in its gap, then gone into it with the path lit) and the grey
/// blocks, hovering once the lighthouse is activated.
s32 Scene_Initialize(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;

  state->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  FieldActor_SetSpritePriority(ACTOR_NORTHWEST_STATUE, 3);
  FieldActor_SetSpritePriority(ACTOR_NORTHEAST_STATUE, 3);
  Io_Write16(BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT2_ALL, &REG_BLDCNT);
  Io_Write16(BLEND_ALPHA(0, 16), &REG_BLDALPHA);
  FadeOverlay_FillTiles(0);
  Call4((void (*)())EffectSprite_InitWithFadeOverlayBlended,
        (s32)&gJupiterLighthouse_GlowSprite, 0, 0, OBJ_SIZE_32X32);
  JupiterLighthouse_SetCyclonePadsForLeader();
  JupiterLighthouse_SpawnBeamParticles();
  if (Event_GetFlag(FLAG_BEAM_ROOM_ENERGY_BLOCK_IN_GAP)) {
    Actor_SetPosition(ACTOR_ENERGY_BLOCK, 0, 0);
    MapGrid_CopyCellAttributes(28, 39, 1, 1, BEAM_ROOM_GAP_CELL_X,
                               BEAM_ROOM_GAP_CELL_Z);
    MapGrid_CopyCellsLayered(28, 39, 1, 1, BEAM_ROOM_GAP_CELL_X,
                             BEAM_ROOM_GAP_CELL_Z);
    MapGrid_CopyCellsLayered(92, 39, 2, 3, 84, 39);
    FieldMapGrid_SetCellBehavior(toFIXED(336), toFIXED(656),
                                 MAPGRID_LAYER_OVERHEAD, CELL_BEHAVIOR_HOVER);
  } else {
    LightSprites_InitB();
    LightSprites_Add(ACTOR_ENERGY_BLOCK, 0);
  }
  // Arriving in the north half, the leader may walk onto the hovering blocks.
  if (gMainRoutineState.entrance == ENTRANCE_NORTH_HALF_WEST ||
      gMainRoutineState.entrance ==
          JUPITER_LIGHTHOUSE_ROOM_BEAM_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH_A ||
      gMainRoutineState.entrance ==
          JUPITER_LIGHTHOUSE_ROOM_BEAM_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH_B)
    Actor_Get(gMainRoutineState.leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED)) {
    Actor_RaiseFromCell(ACTOR_FIRST_HOVERING_BLOCK);
    Actor_RaiseFromCell(ACTOR_FIRST_HOVERING_BLOCK + 1);
    Actor_RaiseFromCell(ACTOR_FIRST_HOVERING_BLOCK + 2);
    Actor_RaiseFromCell(ACTOR_LAST_HOVERING_BLOCK);
  }
  if (Event_GetFlag(FLAG_BEAM_ROOM_SOCKET_BLOCK_RAISED)) {
    Actor_SetPosition(ACTOR_SOCKET_BLOCK, 296, 104);
    Actor_RaiseFromCell(ACTOR_SOCKET_BLOCK);
  }
  return 0;
}

/// Sets the scene's arrival flags and loads the lighthouse's palettes before
/// the map is built. Returns 0.
s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  JupiterLighthouse_LoadPalettes(0);
  return 0;
}

#include "shared/scene_set_arrival_flags.c"

extern s32 gPaletteCyclePausesInEvents;

#include "shared/palette_cycle_set_pauses_in_events.h"

extern s32 gPaletteCycleFrame;

/// Sets the palette cycle's frame (gPaletteCycleFrame).
static inline void PaletteCycle_SetFrame(s32 value) {
  gPaletteCycleFrame = value;
}

#include "shared/palette_cycle_step.h"

#include "shared/jupiter_lighthouse_load_palettes.h"

void MapGrid_SetCellHeight();

#include "shared/actor_raise_from_cell.h"

#include "shared/whirlwind_carry_leader_lower.h"

#include "shared/position_flat_distance.h"

extern s32 gWhirlwindEndScript[];

void Actor_StartScript(struct Actor *actor, const s32 *script);

#include "shared/whirlwind_update_lower.h"

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/whirlwind_spawn.h"

#include "shared/actor_emit_rising_sparks.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

#include "shared/light_sprites_draw.h"

static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add.h"

/// Unused: sets the priority the lit actors' sprites are drawn at.
UNUSED void LightSprites_SetPriority(s32 priority) {
  gLightSpritePriority = priority;
}

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

extern const s32 *gFieldEffect_DurationScripts[];

void Actor_SetSpriteFlags(struct Actor *actor, u32 value);

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#include "../field/field_effect_spawn.c"

#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map_cell_sprite.c"

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_cell.h"

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

// clang-format off
#include "shared/field_effect_duration_scripts.c"

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(JupiterLighthouseRoomBeam),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON, JupiterLighthouseRoomGateButton, JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_FROM_JUPITER_LIGHTHOUSE_ROOM_BEAM),
    SCENE_EXIT(2, JupiterLighthouseRoomBeam, 3),
    SCENE_EXIT(3, JupiterLighthouseRoomBeam, 2),
    SCENE_EXIT(4, JupiterLighthouseRoomBeam, 5),
    SCENE_EXIT(5, JupiterLighthouseRoomBeam, 4),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH_A, JupiterLighthouseRoomErinyesTunicChest, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_FROM_JUPITER_LIGHTHOUSE_ROOM_BEAM_NORTH_A),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH_B, JupiterLighthouseRoomErinyesTunicChest, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_FROM_JUPITER_LIGHTHOUSE_ROOM_BEAM_NORTH_B),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST, JupiterLighthouseRoomErinyesTunicChest, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_FROM_JUPITER_LIGHTHOUSE_ROOM_BEAM_SOUTHWEST),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHEAST_A, JupiterLighthouseRoomErinyesTunicChest, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_FROM_JUPITER_LIGHTHOUSE_ROOM_BEAM_SOUTHEAST_A),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHEAST_B, JupiterLighthouseRoomErinyesTunicChest, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_FROM_JUPITER_LIGHTHOUSE_ROOM_BEAM_SOUTHEAST_B),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTH, JupiterLighthouseRoomErinyesTunicChest, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_FROM_JUPITER_LIGHTHOUSE_ROOM_BEAM_SOUTH),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS, JupiterLighthouseRoomLockedDoors, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_FROM_JUPITER_LIGHTHOUSE_ROOM_BEAM),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 104, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 104, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 104, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 104, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 104, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 600, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 600, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 728, FACING_SOUTHWEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 728, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 648, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH_A, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH_A},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH_B, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH_B},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHEAST_A, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHEAST_A},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHEAST_B, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHEAST_B},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTH, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTH},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS},
    {TRIGGER_STEP, CELL_BEHAVIOR_BEAM, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RideTheBeam)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), TRIGGER_CYCLONE_UP_WEST, CONDITION_ALWAYS, EVENT(JupiterLighthouse_CycloneUpOnWestPad)},
    {TRIGGER_RAISED, TRIGGER_CYCLONE_UP_WEST, CONDITION_ALWAYS, EVENT(JupiterLighthouse_CycloneLiftsLeader)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), TRIGGER_CYCLONE_UP_EAST, CONDITION_ALWAYS, EVENT(JupiterLighthouse_CycloneUpOnEastPad)},
    {TRIGGER_RAISED, TRIGGER_CYCLONE_UP_EAST, CONDITION_ALWAYS, EVENT(JupiterLighthouse_CycloneLiftsLeader)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), TRIGGER_CYCLONE_DOWN_WEST, CONDITION_ALWAYS, EVENT(JupiterLighthouse_CycloneDownOnWestPad)},
    {TRIGGER_RAISED, TRIGGER_CYCLONE_DOWN_WEST, CONDITION_ALWAYS, EVENT(JupiterLighthouse_CycloneDropsLeader)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), TRIGGER_CYCLONE_DOWN_EAST, CONDITION_ALWAYS, EVENT(JupiterLighthouse_CycloneDownOnEastPad)},
    {TRIGGER_RAISED, TRIGGER_CYCLONE_DOWN_EAST, CONDITION_ALWAYS, EVENT(JupiterLighthouse_CycloneDropsLeader)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_ENERGY_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PushEnergyBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ENERGY_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_DropEnergyBlock)},
    // A push trigger answers a push on any actor; its subject is not read.
    {TRIGGER_PUSH, 35, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PushEnergyBlock)},
    {TRIGGER_PUSHED, 35, CONDITION_ALWAYS, EVENT(JupiterLighthouse_DropEnergyBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SOCKET_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_MoveSocketBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SOCKET_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseSocketBlock)},
    {PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StartHover)},
    {PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_AFTER_END), 0, CONDITION_ALWAYS, EVENT(JupiterLighthouse_EndHover)},
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
