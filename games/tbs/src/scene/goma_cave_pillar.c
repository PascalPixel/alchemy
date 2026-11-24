/// @file
/// Goma Cave (SceneId_GomaCavePillar), the middle floor of the cave in the
/// Goma Range that the party passes through between Vault and Bilibin. Three
/// things here are moved with Move: a tall stone pillar by the north wall (the
/// west pillar), a squat stump, and a second tall pillar on the floor in the
/// east.
///
/// The party comes down the west stairs from the cave's first room
/// (goma_cave_stream.c). At the far east of the floor the second pillar can be
/// pushed by walking into it or with Move, into its socket at the foot of a
/// ledge, cell (38, 14). It settles there with a thump and a puff of dust and
/// stays (FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE). Its top is then level with the
/// ledges above, a stepping stone between the ledge where a man stands and the
/// ledge of Forge, a Mars Djinni.
///
/// Later the party comes back up the stairs from the pond room below
/// (goma_cave_pond.c). Move slides the west pillar a step east or back west
/// (FLAG_GOMA_CAVE_WEST_PILLAR_MOVED), and where it stands decides which
/// squares beside it Isaac can jump across. Beyond it, Move pushes the stump
/// east a step at a time. At the edge it rocks, shedding dust, tips over and
/// drops with a crash into the water of the pond room
/// (FLAG_GOMA_CAVE_STUMP_FELL), where it opens the way east, back up to Forge.
/// A chest on the ledge east of the west pillar holds a Lucky Medal.
///
/// The man beside Forge fought it and lost it: "Did you see something strange
/// over there? I fought it, but it fled." Talking to Forge makes him jump in
/// surprise, and the Djinni's battle begins. After a win he fidgets while
/// Forge joins the party, then is amazed and walks away for good
/// (FLAG_GOMA_CAVE_WITNESS_LEFT). After a loss or an escape Forge hides until
/// the room is entered again (FLAG_DJINNI_HIDING(DJINNI_FORGE)), and the man
/// says "Hmmm... It must be hiding somewhere again...".
///
/// All three moved things are area temp flags, so the room is as it started
/// again once the party leaves Goma Cave's four scenes.
///
/// Two stairways lead back to the first room
/// (GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_WEST and
/// GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_EAST) and three down to the pond room
/// (GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_WEST,
/// GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_SOUTH and
/// GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_EAST).
///
/// ENTRANCE_STUMP_FALL, which no exit names and only the debug scene selector
/// reaches, plays the stump being pushed to the edge and falling.

#include "ability_ids.h"
#include "djinn_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_effect.h"
#include "field/pushed_actor.h"
#include "field/scene_tables.h"
#include "field/script_operands.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/goma_cave.h"
#include "scene/import.h"
#include "sound/sound.h"
#include "system/point.h"
#include "system/heap_slots.h"
#include "system/system.h"
#include "text/msg_ids.h"

extern struct EffectDurationScripts gGomaCavePillar_FragmentScripts;
extern s32 gGomaCavePillar_FragmentScriptShort[];
extern s32 gGomaCavePillar_FragmentScriptMedium[];
extern s32 gGomaCavePillar_FragmentScriptLong[];
extern struct SceneEntrance gGomaCavePillar_Entrances[];
extern u32 gGomaCavePillar_Exits[];
extern struct ActorPlacement gGomaCavePillar_ActorPlacements[];
extern struct Trigger gGomaCavePillar_Triggers[];

/// The two axes a dust fragment moves along (GomaCavePillar_UpdateFragment).
enum GomaCavePillarFragmentPlane { PLANE_XY = 0, PLANE_XZ = 1, PLANE_YZ = 2 };

/// The room's actors, numbered in the order of
/// gGomaCavePillar_ActorPlacements.
enum GomaCavePillarActorId {
  /// The tall pillar by the north wall, which slides between two places.
  ACTOR_WEST_PILLAR = ACTOR_FIRST_PLACED,
  /// Pushed over the edge into the room below.
  ACTOR_STUMP,
  /// The tall pillar in the east of the floor, pushed into its socket at the
  /// foot of the ledges.
  ACTOR_EAST_PILLAR,
  /// Hides the stump where it drops behind the floor in front of the ledge.
  ///
  /// It is a crate drawn solid black (SPRITE_COLOR_BLACK) and translucent at
  /// sprite priority 2, behind the floor, and ahead of the stump in the sprite
  /// list. So where it covers the stump the player sees the floor. Moved away
  /// in a render of the fall, the stump shows over the floor instead.
  ///
  /// It waits in the room's corner until the stump teeters at the edge, and
  /// goes back there once the stump has fallen.
  ACTOR_MASK_FOR_STUMP,
  /// Forge, a Mars Djinni, until it joins the party.
  ACTOR_FORGE,
  /// The man who fought Forge and lost it, a man with golden-brown hair in
  /// orange and red.
  ACTOR_WITNESS
};

/// The entrance no exit names, which plays the stump being pushed to
/// the edge and falling into the room below.
///
/// Only the debug scene selector reaches it.
enum { ENTRANCE_STUMP_FALL = 99 };

/// The cell on Forge's ledge from which walking west jumps onto the east
/// pillar.
enum { CELL_BEHAVIOR_GOMA_CAVE_JUMP_TO_PILLAR = 20 };

/// The trigger the man's timer raises once Forge has joined the party
/// (GomaCavePillar_WatchForgeJoin).
enum GomaCavePillarRaisedTrigger { RAISED_FORGE_JOINED = 99 };

#include "shared/pushed_actor.c"

/// Isaac talks to Forge, the Mars Djinni on the ledge in the east: the man
/// beside it jumps in surprise, and the battle with the Djinni begins.
static void GomaCavePillar_FightForge(void) {
  Actor_ShowEmote(ACTOR_WITNESS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_Jump(ACTOR_WITNESS, 2, 0);
  Field_RequestDjinnBattle(ACTOR_FORGE, DJINNI_FORGE);
}

/// Moves a fragment of dust by its two rates each frame, in the plane its
/// place names.
static void GomaCavePillar_UpdateFragment(struct Actor *fragment) {
  // GomaCavePillar_SpawnFragment keeps the two rates in speedLimit and
  // acceleration, and the plane in userValue.
  s16 plane = (s16)fragment->userValue;

  switch (plane) {
  case PLANE_XY:
    fragment->x.fixed += fragment->speedLimit;
    fragment->targetX = fragment->x.fixed;
    fragment->y.fixed += fragment->acceleration;
    fragment->targetY = fragment->y.fixed;
    break;
  case PLANE_XZ:
    fragment->x.fixed += fragment->speedLimit;
    fragment->targetX = fragment->x.fixed;
    fragment->z.fixed += fragment->acceleration;
    fragment->targetZ = fragment->z.fixed;
    break;
  case PLANE_YZ:
    fragment->y.fixed += fragment->speedLimit;
    fragment->targetY = fragment->y.fixed;
    fragment->z.fixed += fragment->acceleration;
    fragment->targetZ = fragment->z.fixed;
    break;
  }
}

/// Hides a fragment of dust at the end of its script.
static s32 GomaCavePillar_FinishFragment(struct Actor *actor) {
  Actor_SetSpriteColor(actor, SPRITE_COLOR_HIDDEN);
  return 0;
}

/// A fragment's style: its kind in the low bits, which picks its script (and
/// its animation, the next number), and its sprite colour from bit 16.
#define FRAGMENT_KIND(style) ((style) & 0xf)
#define FRAGMENT_COLOR(style) (((u32)(style) >> 16) & 0xf)
/// A fragment's place: its map plane in the low half, and its sprite priority
/// (0 for the leader's) in the high half.
#define FRAGMENT_PRIORITY(place) ((u32)(place) >> 16)

/// Creates a fragment with two axis rates, a style (FRAGMENT_KIND,
/// FRAGMENT_COLOR) and a place (its plane and FRAGMENT_PRIORITY).
static void GomaCavePillar_SpawnFragment(s32 x, s32 y, s32 z, s32 firstRate,
                                         s32 secondRate, s32 style, s32 place) {
  struct EffectDurationScripts table;
  struct Actor *leader;
  struct Actor *fragment;
  struct Sprite *sprite;
  u32 priority;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  table = gGomaCavePillar_FragmentScripts;
  fragment = FieldActor_Create(SPRITE_DUST_PUFF, x, y, z);
  if (fragment == NULL)
    return;
  sprite = fragment->sprite;
  Actor_SelectAnimation(fragment, FRAGMENT_KIND(style + 1));
  Actor_StartScript(fragment, table.script[FRAGMENT_KIND(style)]);
  Actor_SetSpriteColor(fragment, FRAGMENT_COLOR(style));
  fragment->motionFlags = 0;
  sprite->flags = 0;
  fragment->update = GomaCavePillar_UpdateFragment;
  fragment->speedLimit = firstRate;
  fragment->acceleration = secondRate;
  fragment->userValue = (u16)place;
  priority = FRAGMENT_PRIORITY(place);
  switch (priority) {
  case 0:
    sprite->image.priority = leader->sprite->image.priority;
    break;
  case 1:
  case 2:
  case 3:
    fragment->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    sprite->image.priority = priority;
    break;
  }
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gGomaCavePillar_Entrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gGomaCavePillar_Exits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gGomaCavePillar_ActorPlacements;
}

/// Copies the cells around the west pillar for one of its two places from the
/// spare stretch of map that holds them.
static inline void GomaCavePillar_CopyWestPillarCells(s32 sourceColumn) {
  MapGrid_CopyCellAttributes(sourceColumn, 40, 6, 3, 18, 6);
}

/// Isaac casts Move on the west pillar, by the north wall: it slides a step
/// east, or back west to where it started, and the squares beside it that
/// can be jumped across change with it.
///
/// Where it started, the jump squares are on its east side; moved east, they
/// are on its west side. Both sets are copied from a spare stretch of the
/// map below the room.
static void GomaCavePillar_MoveWestPillar(void) {
  s32 x;

  x = Actor_Get(ACTOR_WEST_PILLAR)->x.fixed;
  if (x < 0)
    x += toFIXED(16) - 1;
  x = FIELD_CELL(x);
  Event_Start();
  if (x == 20) {
    GomaCavePillar_CopyWestPillarCells(18);
    GameFlag_Clear(FLAG_GOMA_CAVE_WEST_PILLAR_MOVED);
  } else {
    GomaCavePillar_CopyWestPillarCells(24);
    GameFlag_Set(FLAG_GOMA_CAVE_WEST_PILLAR_MOVED);
  }
  Event_End();
}

/// Drops one fragment of dust from somewhere along the stump's width, falling
/// at a random speed of up to four tenths of a pixel a frame.
static inline void GomaCavePillar_DropStumpFragment(void) {
  FIXED fixedX;
  FIXED fixedZ;
  struct Actor *heightSource;
  struct Actor *xSource;
  s32 randomX, style, speedBand, bandScaled, fallingSpeed;

  xSource = Actor_Get(ACTOR_STUMP);
  randomX = Random_Next();
  fixedX = xSource->x.fixed;
  fixedX += INTtoFIXED((u32)randomX * 12 >> 16);
  heightSource = Actor_GetById(ACTOR_STUMP);
  fixedZ = Actor_Get(ACTOR_STUMP)->z.fixed;
  fixedZ += toFIXED(6);
  speedBand = Random_Below(5);
  // These shifts produce speedBand * 6553: a tenth-pixel falling step.
  bandScaled = (((speedBand << 1) + speedBand) << 2) + speedBand;
  fallingSpeed = (bandScaled << 6) - bandScaled;
  fallingSpeed <<= 3;
  fallingSpeed += speedBand;
  style = Random_Below(2);
  fallingSpeed = -fallingSpeed;
  GomaCavePillar_SpawnFragment(fixedX, heightSource->y.fixed, fixedZ, 0,
                               fallingSpeed, style, PLANE_XY);
}

/// While the stump teeters, dust falls from it: one fragment every eighth
/// frame, two on every sixteenth.
static void GomaCavePillar_DropStumpFragments(void) {
  if ((gFrameCount & 7) == 0) {
    GomaCavePillar_DropStumpFragment();
    if ((gFrameCount & 15) == 0)
      GomaCavePillar_DropStumpFragment();
  }
}

/// The frames the witness has watched Forge join, counted by
/// GomaCavePillar_WatchForgeJoin.
s32 gGomaCavePillar_WitnessFrames;

/// Isaac casts Move on the stump in the middle of the room. Pushed to the
/// edge, it rocks there shedding dust, tips over and drops out of sight, and
/// the screen shakes as it lands in the water of the pond room below
/// (FLAG_GOMA_CAVE_STUMP_FELL).
///
/// Short of the edge, Move pushes it a step and the event has nothing to do.
/// A black crate (`ACTOR_MASK_FOR_STUMP`) is moved in front of the edge while
/// it falls, so the stump seems to drop behind the floor.
static void GomaCavePillar_PushStumpOver(void) {
  s32 column;
  FIXED fixedX;
  FIXED fixedZ;
  struct Actor *heightSource;
  struct Actor *stump;
  s32 style, speedBand, bandScaled, fallingSpeed;

  stump = Actor_Get(ACTOR_STUMP);
  column = stump->x.fixed / toFIXED(16);
  Event_Start();
  if (column == 25) {
    Actor_Get(ACTOR_MASK_FOR_STUMP)->layer = 1;
    Actor_SetSpriteFlags(Actor_Get(ACTOR_MASK_FOR_STUMP), 0);
    // XXX (no evidence): why the mask is drawn black and translucent rather
    // than in SPRITE_COLOR_HIDDEN, which would hide the stump the same way.
    Actor_SetSpriteColorById(ACTOR_MASK_FOR_STUMP, SPRITE_COLOR_BLACK);
    Actor_SetBlendMode(Actor_Get(ACTOR_MASK_FOR_STUMP),
                       ACTOR_BLEND_TRANSLUCENT);
    Actor_SetPosition(ACTOR_MASK_FOR_STUMP, 414, 240);
    Event_Wait(10);
    // Dust falls while the stump rocks on the edge.
    Task_AddFrame(GomaCavePillar_DropStumpFragments, TASK_PASS_FRAME);
    Sound_Play(SE_RUMBLE);
    Actor_SetDestinationOffset(ACTOR_STUMP, 1, 0);
    Actor_WaitForMoveById(ACTOR_STUMP);
    Event_Wait(10);
    Actor_SetDestinationOffset(ACTOR_STUMP, 2, 0);
    Actor_WaitForMoveById(ACTOR_STUMP);
    Actor_Get(ACTOR_STUMP)->bounce = 0;
    Actor_Get(ACTOR_STUMP)->gravity = toFIXED(3) / 5;
    Event_Wait(3);
    // It tips over the edge.
    Actor_SetSpeedAndAcceleration(ACTOR_STUMP, 2.5, 0.25);
    Sound_Play(SE_STOP_ACTION_SE);
    Actor_SetDestination(ACTOR_STUMP, 416, 200);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_STUMP), 0);
    Task_Remove(GomaCavePillar_DropStumpFragments);
    Event_Wait(12);
    Sound_Play(SE_STONE_GRIND);
    fixedX = Actor_Get(ACTOR_STUMP)->x.fixed + INTtoFIXED(Random_Below(12));
    heightSource = Actor_Get(ACTOR_STUMP);
    fixedZ = Actor_Get(ACTOR_STUMP)->z.fixed + toFIXED(6);
    speedBand = Random_Below(5);
    // A last piece of dust, with the same tenth-pixel speed bands.
    bandScaled = (((speedBand << 1) + speedBand) << 2) + speedBand;
    fallingSpeed = (bandScaled << 6) - bandScaled;
    fallingSpeed <<= 3;
    fallingSpeed += speedBand;
    style = Random_Below(2);
    fallingSpeed = -fallingSpeed;
    GomaCavePillar_SpawnFragment(fixedX, heightSource->y.fixed, fixedZ, 0,
                                 fallingSpeed, style, PLANE_XY);
    Event_Wait(20);
    // It lands below; the shake dies down before the map grid cells change.
    Sound_Play(SE_CAST);
    MapGrid_SetShake(5, 5, 1);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    MapGrid_WaitForShakeToSettle();
    Actor_SetPosition(ACTOR_STUMP, 0, 0);
    Actor_SetPosition(ACTOR_MASK_FOR_STUMP, 0, 0);
    GameFlag_Set(FLAG_GOMA_CAVE_STUMP_FELL);
    MapGrid_CopyCellAttributes(21, 45, 4, 2, 21, 11);
  }
  Event_End();
}

/// Once the east pillar reaches its socket at the foot of the ledge, cell
/// (38, 14), it settles into it with a thump and a puff of dust.
static void GomaCavePillar_SettleEastPillar(void) {
  s32 x;
  s32 z;

  x = Actor_Get(ACTOR_EAST_PILLAR)->x.fixed / toFIXED(16);
  z = Actor_Get(ACTOR_EAST_PILLAR)->z.fixed / toFIXED(16);
  if (x == 38 && z == 14) {
    Actor_Get(ACTOR_EAST_PILLAR)->y.fixed = -toFIXED(2);
    Actor_Get(ACTOR_EAST_PILLAR)->targetY =
        Actor_Get(ACTOR_EAST_PILLAR)->y.fixed;
    Sound_Play(SE_THUMP);
    GomaCavePillar_SpawnFragment(Actor_Get(ACTOR_EAST_PILLAR)->x.fixed,
                                 Actor_Get(ACTOR_EAST_PILLAR)->y.fixed,
                                 Actor_Get(ACTOR_EAST_PILLAR)->z.fixed,
                                 toFIXED(1) / 2, 0, 0, PLANE_XZ);
    GomaCavePillar_SpawnFragment(Actor_Get(ACTOR_EAST_PILLAR)->x.fixed,
                                 Actor_Get(ACTOR_EAST_PILLAR)->y.fixed,
                                 Actor_Get(ACTOR_EAST_PILLAR)->z.fixed,
                                 toFIXED(2) / 5, toFIXED(2) / 5, 0, PLANE_XZ);
    GomaCavePillar_SpawnFragment(Actor_Get(ACTOR_EAST_PILLAR)->x.fixed,
                                 Actor_Get(ACTOR_EAST_PILLAR)->y.fixed,
                                 Actor_Get(ACTOR_EAST_PILLAR)->z.fixed,
                                 -toFIXED(2) / 5, toFIXED(2) / 5, 0, PLANE_XZ);
    GomaCavePillar_SpawnFragment(Actor_Get(ACTOR_EAST_PILLAR)->x.fixed,
                                 Actor_Get(ACTOR_EAST_PILLAR)->y.fixed,
                                 Actor_Get(ACTOR_EAST_PILLAR)->z.fixed,
                                 -toFIXED(1) / 2, 0, 0, PLANE_XZ);
    GomaCavePillar_SpawnFragment(Actor_Get(ACTOR_EAST_PILLAR)->x.fixed,
                                 Actor_Get(ACTOR_EAST_PILLAR)->y.fixed,
                                 Actor_Get(ACTOR_EAST_PILLAR)->z.fixed,
                                 toFIXED(2) / 5, -toFIXED(2) / 5, 0, PLANE_XZ);
    GomaCavePillar_SpawnFragment(Actor_Get(ACTOR_EAST_PILLAR)->x.fixed,
                                 Actor_Get(ACTOR_EAST_PILLAR)->y.fixed,
                                 Actor_Get(ACTOR_EAST_PILLAR)->z.fixed,
                                 -toFIXED(2) / 5, -toFIXED(2) / 5, 0, PLANE_XZ);
    GameFlag_Set(FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE);
  }
}

/// Isaac casts Move on the tall pillar in the east of the floor: it slides a
/// step, and settles into its socket once it reaches it
/// (FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE).
static void GomaCavePillar_MoveEastPillar(void) {
  Event_Start();
  GomaCavePillar_SettleEastPillar();
  Event_End();
}

/// Isaac walks into the tall pillar in the east of the floor and pushes it a
/// step, until it settles into its socket at the foot of the ledge.
static void GomaCavePillar_PushEastPillar(void) {
  Event_Start();
  PushedActor_AdvancePair();
  GomaCavePillar_SettleEastPillar();
  Event_End();
}

/// Isaac, walking west from Forge's ledge at cell (40, 14), jumps across the
/// gap onto the top of the east pillar, back towards the man's ledge.
///
/// The cell only becomes a jump when the party comes in on the east side with
/// the pillar in its socket (Scene_Initialize).
static void GomaCavePillar_JumpGapWest(void) {
#if TBS_PATCH_1
  Field_TryJumpForward();
#else
  struct Actor *leader;
  struct FieldPosition target;
  u8 *motionFlags;
  u8 savedMotionFlags;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  motionFlags = &leader->motionFlags;
  savedMotionFlags = *motionFlags;
  target.x = Actor_Get(ACTOR_PARTY_LEADER)->x.fixed - toFIXED(32);
  /* LEFTOVER(template): destination holds &target, the landing reached through
   * a pointer. */
  target.y = Actor_Get(ACTOR_PARTY_LEADER)->y.fixed;
  target.z = Actor_Get(ACTOR_PARTY_LEADER)->z.fixed;
  if (FieldActor_CheckMovementCollision(leader, &target) == 0) {
    Event_Start();
    Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
    wait(6);
    Sound_Play(SE_JUMP);
    Actor_SelectAnimation(leader, ANIM_LEADER_LEAP);
    leader->speedLimit = toFIXED(3);
    leader->acceleration = toFIXED(2);
    leader->velocityY = toFIXED(4);
    *motionFlags &= ACTOR_JUMP_KEEPS_MOTION;
    Actor_SetSpriteFlags(leader, 0);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, (s16)(FIXEDtoINT((u32)target.x)),
                        (s16)(FIXEDtoINT((u32)target.z)));
    Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
    Actor_SetSpriteFlags(leader, SPRITE_FLAG_SHADOW);
    *motionFlags = savedMotionFlags;
    Event_End();
  }
#endif
}

const struct Trigger *Scene_GetTriggers(void) {
  return gGomaCavePillar_Triggers;
}

/// Back from a won battle with Forge, the man who fought it winces, fidgets
/// and shakes his head while the Djinni joins the party, then raises
/// `RAISED_FORGE_JOINED`.
static void GomaCavePillar_WatchForgeJoin(void) {
  struct FieldState *state;
  s32 frame;

  state = gFieldState;
  frame = ++gGomaCavePillar_WitnessFrames;
  switch (frame) {
  case 60:
    Actor_FaceDirection(ACTOR_WITNESS, FACING_SOUTHEAST, 0);
    Actor_ShowEmote(ACTOR_WITNESS, EMOTE_WINCE, 0);
    break;
  case 180:
    Actor_StartRepeatedMotion(ACTOR_WITNESS, 3);
    break;
  case 240:
  case 270:
    Actor_Jump(ACTOR_WITNESS, 4, 0);
    break;
  case 480:
    Actor_SetAnimation(ACTOR_WITNESS, ANIM_SHAKE_HEAD);
    break;
  }
  if (gMainRoutineState.pendingDjinni == 0)
    state->raisedTrigger = RAISED_FORGE_JOINED;
}

/// Once Forge has joined the party, the man who fought it is amazed: "You
/// got it? Wow, that's pretty impressive!" He nods and walks away to the
/// south west, gone for good (FLAG_GOMA_CAVE_WITNESS_LEFT).
static void GomaCavePillar_WitnessCheersForgeCaught(void) {
  Task_Remove(GomaCavePillar_WatchForgeJoin);
  Event_Start();
  Actor_ShowEmote(ACTOR_WITNESS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_RunRepeatedMotion(ACTOR_WITNESS, 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Event_SetMessageId(MsgGomaGotWowThatsPrettyImpressive);
  // "You got it? Wow, that's pretty impressive!"
  // "Keep your eyes peeled for others like it."
  Event_ShowMessage(ACTOR_WITNESS, 0);
  Actor_SetAnimationAndWait(ACTOR_WITNESS, ANIM_NOD);
  Event_Wait(30);
  Actor_Get(ACTOR_EAST_PILLAR)->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetSpeed(ACTOR_WITNESS, 2);
  Actor_WalkToAndWait(ACTOR_WITNESS, 600, 216);
  Actor_WalkToAndWait(ACTOR_WITNESS, 600, 248);
  Actor_WalkToAndWait(ACTOR_WITNESS, 568, 296);
  Actor_SetPosition(ACTOR_WITNESS, 0, 0);
  Actor_Get(ACTOR_EAST_PILLAR)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  GameFlag_Set(FLAG_GOMA_CAVE_WITNESS_LEFT);
  Event_End();
}

/// Sets up the room as the party enters: the pillars and the stump where the
/// party left them, and the east pillar's top as a stepping stone when the
/// party comes in on the east side.
s32 Scene_Initialize(void) {
  struct FieldState *state;

  state = gFieldState;
  state->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  state->transitionFrames = 24;
  Actor_Get(ACTOR_STUMP)->collisionFlags |= ACTOR_NEAR_PSYNERGY;
  if (GameFlag_Get(FLAG_GOMA_CAVE_WEST_PILLAR_MOVED) != 0) {
    Actor_SetPosition(ACTOR_WEST_PILLAR, 344, 104);
    GomaCavePillar_CopyWestPillarCells(24);
  } else {
    GomaCavePillar_CopyWestPillarCells(18);
  }
  // The cells GomaCavePillar_PushStumpOver copies as it sets the flag, copied
  // again on each arrival.
  if (GameFlag_Get(FLAG_GOMA_CAVE_STUMP_FELL) != 0) {
    Actor_SetPosition(ACTOR_STUMP, 0, 0);
    MapGrid_CopyCellAttributes(21, 45, 4, 2, 21, 11);
  }
  if (GameFlag_Get(FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE) != 0) {
    Actor_SetPosition(ACTOR_EAST_PILLAR, 616, 232);
    if (gMainRoutineState.entrance ==
            GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_STREAM_EAST ||
        gMainRoutineState.entrance ==
            GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_EAST) {
      Actor_Get(ACTOR_EAST_PILLAR)->layer = 2;
      Actor_Get(ACTOR_EAST_PILLAR)->y.fixed--;
      Actor_Get(ACTOR_EAST_PILLAR)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      MapGrid_CopyCellAttributes(36, 48, 5, 1, 36, 14);
    }
  }
  if (gMainRoutineState.entrance == ENTRANCE_STUMP_FALL) {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Actor_SetPosition(ACTOR_STUMP, 384, 192);
    Event_Wait(60);
    Actor_Get(ACTOR_STUMP)->layer = 2;
    Actor_MoveToAndWait(ACTOR_STUMP, 408, 192);
    Event_Wait(60);
    GomaCavePillar_PushStumpOver();
  }
  // Coming back from a Djinni battle: the man reacts until it is over.
  if (gMainRoutineState.pendingDjinni != 0) {
    gGomaCavePillar_WitnessFrames = 0;
    Task_AddFrame(GomaCavePillar_WatchForgeJoin, TASK_PASS_FRAME);
  }
  return 0;
}

#include "shared/pushed_actor_tables.c"
// clang-format off

/// How long a fragment of dust lasts, chosen by FRAGMENT_KIND.
struct EffectDurationScripts gGomaCavePillar_FragmentScripts = {{
    gGomaCavePillar_FragmentScriptShort,
    gGomaCavePillar_FragmentScriptMedium,
    gGomaCavePillar_FragmentScriptLong,
}};

s32 gGomaCavePillar_FragmentScriptShort[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_CALL(GomaCavePillar_FinishFragment),
    SC_WAIT(1),
    SC_RELEASE,
};

s32 gGomaCavePillar_FragmentScriptMedium[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_CALL(GomaCavePillar_FinishFragment),
    SC_WAIT(1),
    SC_RELEASE,
};

s32 gGomaCavePillar_FragmentScriptLong[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_CALL(GomaCavePillar_FinishFragment),
    SC_WAIT(1),
    SC_RELEASE,
};

struct SceneEntrance gGomaCavePillar_Entrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_STREAM_WEST, CONDITION_ALWAYS, 88, 0, 184, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_STREAM_EAST, CONDITION_ALWAYS, 584, 0, 184, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_EAST, CONDITION_ALWAYS, 520, 0, 232, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_WEST, CONDITION_ALWAYS, 232, 0, 184, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_SOUTH, CONDITION_ALWAYS, 536, 0, 408, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { ENTRANCE_STUMP_FALL, CONDITION_ALWAYS, 376, 0, 200, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { SCENE_TABLE_END },
};

u32 gGomaCavePillar_Exits[] = {
    SCENE_EXITS(GomaCavePillar),
    SCENE_EXIT(GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_WEST, GomaCaveStream, GOMA_CAVE_STREAM_FROM_GOMA_CAVE_PILLAR_WEST),
    SCENE_EXIT(GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_WEST, GomaCavePond, GOMA_CAVE_POND_FROM_GOMA_CAVE_PILLAR_WEST),
    SCENE_EXIT(GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_SOUTH, GomaCavePond, GOMA_CAVE_POND_FROM_GOMA_CAVE_PILLAR_SOUTH),
    SCENE_EXIT(GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_EAST, GomaCavePond, GOMA_CAVE_POND_FROM_GOMA_CAVE_PILLAR_EAST),
    SCENE_EXIT(GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_EAST, GomaCaveStream, GOMA_CAVE_STREAM_FROM_GOMA_CAVE_PILLAR_EAST),
    SCENE_EXITS_END,
};

struct ActorPlacement gGomaCavePillar_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 384, 0, 192, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_FORGE), MOVEMENT_TYPE_STAND, 664, 0, 216, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_ORANGE, FLAG_GOMA_CAVE_WITNESS_LEFT, MOVEMENT_TYPE_STAND, 628, 0, 184, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct Trigger gGomaCavePillar_Triggers[] = {
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_WEST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_WEST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_SOUTH },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 8, CONDITION_ALWAYS, GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_EAST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 9, CONDITION_ALWAYS, GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_EAST },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, ACTOR_EAST_PILLAR, FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE, EVENT(GomaCavePillar_PushEastPillar) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_GOMA_CAVE_JUMP_TO_PILLAR, CONDITION_ALWAYS, EVENT(GomaCavePillar_JumpGapWest) },
    { TRIGGER_TALK, ACTOR_FORGE, CONDITION_ALWAYS, EVENT(GomaCavePillar_FightForge) },
    { TRIGGER_TALK, ACTOR_WITNESS, FLAG_DJINNI_HIDING(DJINNI_FORGE), MSGNO(MsgVaultDidYouSeeSomethingStrangeOver) },
    { TRIGGER_TALK, ACTOR_WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultHmmmItMustBeHidingSomewhere) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WITNESS, FLAG_DJINNI_HIDING(DJINNI_FORGE), MSGNO(MsgVaultItJustWhackedMeInThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultIKnowitsKindItllBeBack) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_PILLAR, CONDITION_ALWAYS, EVENT(GomaCavePillar_MoveWestPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STUMP, FLAG_GOMA_CAVE_STUMP_FELL, EVENT(GomaCavePillar_PushStumpOver) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_PILLAR, FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE, EVENT(GomaCavePillar_MoveEastPillar) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_GOMA_CAVE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { TRIGGER_RAISED, RAISED_FORGE_JOINED, CONDITION_ALWAYS, EVENT(GomaCavePillar_WitnessCheersForgeCaught) },
    { TRIGGER_TABLE_END },
};
// clang-format on
