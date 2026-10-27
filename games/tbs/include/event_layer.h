#ifndef ALCHEMY_EVENT_LAYER_H
#define ALCHEMY_EVENT_LAYER_H

/// @file
/// The event layer: the constants and small inlines a scene's events share,
/// over the main-image routines they call by those routines' own names
/// (Event_Start, Actor_Jump, Event_SetMessageId and the rest), whose
/// declarations it includes.
///
/// It is a habit of the whole game written once, as declarations and macros: a
/// layer, not a compiler-steering device. event_inline.h's inlines are
/// written over it. An event reads `Event_Start(); Actor_Stop(ACTOR_GARET);
/// Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0); Event_End();`.

#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "unit/game/game.h"
#include "unit/sound/sound.h"
#include "unit/shop/shop.h"
#include "unit/window/window.h"
#include "unit/menu/menu.h"
#include "unit/scene/common/scroll_initialize_hblank_dma.h"
#include "fixed_math.h"
#include "map_grid_copy.h"

#include "static_assert.h"

#include "save_block.h"

// A scene stores into its own const tables: an overlay's data is in RAM.
// The games' compiler takes the store as written; a current GCC refuses it,
// so the modern build names the place without its const.
#ifdef MODERN
/// The comma expression removes qualifiers without promoting narrow integers.
#define STORE_CONST(place) (*(__typeof__(((void)0, (place))) *)&(place))
#else
#define STORE_CONST(place) (place)
#endif

/// 0x78 bytes of EWRAM where scene overlays keep state of their own, such as
/// Altmiller Cave's rock puzzle. It lies outside the heap, so a scene change
/// leaves it alone; SaveBlock_InitializeDefaults clears it for a new game.
extern u8 gSceneLocalState[];

/// The Colosso race work, heap slot HEAP_SLOT_COLOSSO_RACE under its own name.
/// Colosso's code reads it under either name, each site as it matched; the
/// slot's literal is gHeapSlots+236, the same address through another
/// relocation, so no byte differs. Where only the gauge is read, a struct
/// ColossoGaugeWork pointer (its first member) compiles as ->gauge does.
extern u8 *gColossoWork;

#include "frame.h"

#include "actor.h"

/// Animation numbers of an actor's sprite, for
/// Import_ActorSetAnimation.
enum ActorAnimation {
  /// Standing still: nearly every sprite's first frame held, without the
  /// blinks and small gestures of ANIM_STAND.
  ANIM_STILL = 0,
  ANIM_STAND = 1,
  ANIM_WALK = 2,
  ANIM_NOD = 3,
  ANIM_SHAKE_HEAD = 4,
  /// Running, leaning forward, four frames a step to the walk's six. This and
  /// the leader's other poses below are as Isaac's sprite (spr_000.png) draws
  /// them.
  ANIM_LEADER_RUN = 5,
  ANIM_LEADER_SPRING = 6, ///< leaning back for a moment before a jump
  ANIM_LEADER_LEAP = 7,   ///< in mid-stride, leaning forward
  /// The leader leaning into what it pushes, as scenes play it while a statue
  /// or stone pillar slides ahead of it.
  ANIM_PUSH = 8,
  /// ANIM_STAND's idle with more glances to each side, as when a whirlwind
  /// catches the leader.
  ANIM_LEADER_LOOK_AROUND = 9,
  ANIM_LEADER_CLIMB = 10, ///< climbing a wall, seen from behind
  /// Stepping off the top of a ladder, as onto the roof beside Isaac's house.
  ANIM_LEADER_CLIMB_OVER_TOP = 11,
  ANIM_LEADER_CLING = 12, ///< still on a wall, seen from behind
  /// ANIM_LEADER_CLIMB_OVER_TOP played backwards: stepping from the top of a
  /// ladder onto its rungs.
  ANIM_LEADER_CLIMB_DOWN_FROM_TOP = 13,
  ANIM_LEADER_CLIMB_EAST = 14, ///< reaching along a wall as he climbs east
  ANIM_LEADER_CLIMB_WEST = 15, ///< reaching along a wall as he climbs west
  /// One leg forward, braced, held briefly: the frame ANIM_LEADER_LEAP holds
  /// and the run starts on, as when he lunges into a fight or pulls someone
  /// from a river.
  ANIM_LEADER_STRIDE = 16,
  /// Holding out a glowing hand, as the party's sprites cast Psynergy
  /// (spr_000.png, spr_001.png).
  ANIM_CAST_PSYNERGY = 17,
  /// Sitting waist-deep in water and splashing, as in a Lamakan Desert oasis.
  ANIM_LEADER_BATHE = 18,
  /// Lying collapsed on the ground; every party member's sheet maps this
  /// number to such a pose (spr_000.png to spr_003.png).
  ANIM_COLLAPSE = 19,
  ANIM_LEADER_LEAN_BACK = 22, ///< leaning back, his head thrown back
  /// The arrivals and leavings of the animated exits (enum ExitStyle), each
  /// four frames of the leader turning: an arrival grows from a small figure
  /// seen side-on to full size facing the player, a leaving turns from his
  /// back to side-on as he shrinks and is then hidden. The alternate pair has
  /// its own drawings of the same turns.
  ///@{
  ANIM_LEADER_ARRIVE_ALTERNATE = 23,
  ANIM_LEADER_ARRIVE = 24,
  ANIM_LEADER_LEAVE_ALTERNATE = 25,
  ANIM_LEADER_LEAVE = 26,
  ///@}
  /// Spinning round once, then drawn blank (ANIMCMD_BLANK).
  ANIM_LEADER_SPIN_AWAY = 27,
  ANIM_LEADER_ARMS_RAISED = 28, ///< both arms raised over his head
  /// Swinging an arm out to throw, as he tosses a coin or medal at Tolbi's
  /// spring.
  ANIM_LEADER_TOSS = 29,
  /// Held in ANIM_LEADER_SPRING's crouch, knees bent, as after a lost
  /// match.
  ANIM_LEADER_CROUCH = 35
};

// Speaker bits, above the actor id in the low 12 bits of a message command's
// speaker, as FieldMessage_ShowAtActor reads them.
#define SPEAKER_PORTRAIT_LEFT 0x1000  ///< the portrait left of the speaker
#define SPEAKER_PORTRAIT_RIGHT 0x2000 ///< the portrait right of the speaker
#define SPEAKER_WINDOW_ABOVE 0x4000   ///< the message window above the speaker
#define SPEAKER_WINDOW_BELOW 0x8000   ///< the message window never above it

/// One step of a cell animation: copy the width by height cells at sourceX,
/// sourceY over the destination, then wait frames.
#define MAPGRID_ANIMATION_STEP(sourceX, sourceY, width, height, frames)        \
  (sourceX), (sourceY), (width), (height), (frames)
#define MAPGRID_ANIMATION_STEPS_END                                            \
  0xffff ///< a source column that ends the steps

/// A cell animation, such as a door opening: its steps and the cell they are
/// copied to (MapGrid_PlayMetatileCopySequence, which blocks until the last).
struct MapGridCellAnimation {
  /// MAPGRID_ANIMATION_STEP entries, ended by MAPGRID_ANIMATION_STEPS_END, for
  /// MapGrid_PlayMetatileCopySequence.
  const u16 *steps;
  u16 x; ///< destination cell column
  u16 y; ///< destination cell row
};

// The imports below keep a name of their own, prefixed Import_: the scenes'
// calls of these routines were compiled against declarations that differ from
// the routines' own, as the bytes measure. Import_EventSetExitRequest,
// Import_ActorSetPosition, Import_ActorSetAnimation and
// Import_RandomNext are called with differing argument counts, so they keep
// old-style declarations; Event_SetExitRequest also takes a u16 and
// Random16 returns a u32, where the scenes pass and read an int. Imil's and Sol
// Sanctum's Psynergy Stone room's calls of GameFlag_Set, which returns the
// flag, compile only as calls of a void routine (Import_GameFlagSet), where
// every other scene's match through GameFlag_Set itself, and the scenes pass
// Event_ShowTwoMessagesAndWait eleven arguments where it takes ten
// (Import_EventShowTwoMessagesAndWait).
/// Sets the field's exit request (Event_SetExitRequest); 999 leaves for
/// gSaveBlock's scene and entrance.
void Import_EventSetExitRequest();
/// Puts an actor at rest at (x, z), in 16.16 pixels; one that follows
/// the ground keeps its height above the terrain
/// (Actor_SetHorizontalPositionWithTerrain).
void Import_ActorSetPosition();
/// Switches an actor's sprite to an ActorAnimation
/// (Actor_SetModeById).
void Import_ActorSetAnimation();
/// Sets a game flag (GameFlag_Set), for the two scenes whose calls compile only
/// as calls of a void routine.
void Import_GameFlagSet(s32 flag);
/// Advances the game's random number generator and returns 0 to 0xffff
/// (Random16).
s32 Import_RandomNext();
/// The remainder of an unsigned division, from the ARM routine in IWRAM that
/// also serves as __umodsi3 (IwramUnsignedRemainder).
u32 Math_RemainderUnsigned(u32 dividend, u32 divisor);
/// Signed division, the compiler's helper for `/`, imported from the ARM
/// routine in IWRAM (IwramSignedDivide).
s32 __divsi3(s32 dividend, s32 divisor);
/// Frees heap slot's block, and with it every later block of the same pool.
void FreeHeapSlot(s32 slot);
/// The engine encodes either a movement type or an actor-script address.
static inline void Actor_SetScript(s32 actorId, const s32 *script) {
  Actor_SetMovementTypeById(actorId, (s32)script);
}

/// Runs a script, waiting at most 300 frames for its final command.
static inline void Actor_RunScriptAndWait(s32 actorId, const s32 *script) {
  Actor_SetMovementTypeAndWaitById(actorId, (s32)script);
}

/// Two actors speak at once, each message and portrait at its own tile
/// position, until A, B, L or R closes both
/// (Event_ShowTwoMessagesAndWait). The routine takes ten arguments and
/// never reads flags.
void Import_EventShowTwoMessagesAndWait(s32 speaker, s32 x, s32 y, s32 arg,
                                        s32 extra, s32 other_speaker,
                                        s32 otherX, s32 other_y, s32 other_arg,
                                        s32 other_extra, s32 flags);

#ifdef FIELD_STAGED_ACTOR_IMPORTS
/// Declared for the scenes that define FIELD_STAGED_ACTOR_IMPORTS in place
/// of Event_PlaySong. Nothing calls it and no import resolves it: those scenes
/// call Sound_PlaySong.
void Audio_PlayCue(s32 cue);
#endif

/// Moves the event's message number count messages on, past lines the event
/// doesn't show.
static inline void Event_SkipMessages(s32 count) {
  u16 *message = &gFieldWork->message;

  *message = *message + count;
}

/// Shows the event's next line of dialogue and waits for the player to read
/// it.
static inline void Event_ShowMessage(s32 speaker, s32 flags) {
  Event_ShowMessageAndWait(speaker, flags);
}

/// Opens the event's next line and returns its window once the text is typed
/// out, so the event can act while it is still on screen.
static inline s32 Event_OpenMessage(s32 speaker, s32 flags) {
  return (s32)FieldMessage_ShowAtActor(speaker, flags);
}

/// Puts the current line to the player as a question and shows the line that
/// answers it; returns 0 for yes.
static inline s32 Event_AskYesNo(s32 speaker, s32 flags) {
  return Event_AskYesNoAndShowResponse(speaker, flags);
}

/// Looks up an actor for an event, NULL when it doesn't exist
/// (Actor_GetById).
static inline struct Actor *Actor_Get(s32 actorId) {
  return Actor_GetById(actorId);
}

/// Places an actor at a point at once, without walking it there.
static inline void Actor_SetPosition(s32 actorId, s32 fixed_x, s32 fixed_z) {
  Import_ActorSetPosition(actorId, fixed_x, fixed_z);
}

/// Sets how fast an actor moves, and how quickly it gets up to speed,
/// for the event's next moves.
static inline void Actor_SetSpeed(s32 actorId, s32 speed, s32 acceleration) {
  Actor_SetSpeedParameters(actorId, speed, acceleration);
}

/// Moves an actor to a point in the pose it has, without walking; the
/// event carries on meanwhile.
static inline void Actor_SetDestination(s32 actorId, s32 x, s32 z) {
  Actor_ResetAndSetPosition(actorId, x, z);
}

#ifndef FIELD_STAGED_ACTOR_IMPORTS
/// Moves an actor some distance from where it stands, without waiting
/// for it.
static inline void Actor_SetDestinationOffset(s32 actorId, s32 dx, s32 dz) {
  Actor_OffsetPositionAndResetMotion(actorId, dx, dz);
}
#endif

/// Walks an actor to a point; the event goes on while it walks.
static inline void Actor_WalkTo(s32 actorId, s32 x, s32 z) {
  Actor_ResetAndSetPositionInMode2(actorId, x, z);
}

/// Walks an actor some distance from where it stands, without waiting
/// for it to get there.
static inline void Actor_WalkBy(s32 actorId, s32 dx, s32 dz) {
  Actor_OffsetPositionAndReset(actorId, dx, dz);
}

/// Turns an actor to a direction and holds the event for a few frames.
static inline void Actor_FaceDirection(s32 actorId, s32 facing, s32 frames) {
  Actor_ArmCallback(actorId, facing, frames);
}

/// Has one actor look at another at once, then pauses the event.
static inline void Actor_FaceActor(s32 actorId, s32 target, s32 frames) {
  Actor_SetAngleToward(actorId, target, frames);
}

/// Pops an emote bubble up over an actor, then lets frames pass before
/// the event goes on.
static inline void Actor_ShowEmote(s32 actorId, s32 emote, s32 frames) {
  FieldEffect_SpawnAttached(actorId, emote, frames);
}

/// Attaches a field effect to an actor, takes it off, or plays one
/// over it once.
static inline void Actor_SetAttachedEffect(s32 actorId, s32 effect) {
  FieldEffect_SetAttached(actorId, effect);
}

/// Sets how fast the camera pans for the event's next camera moves.
static inline void FieldCamera_SetSpeed(s32 speed, s32 acceleration) {
  FieldCamera_SetSpeedParameters(speed, acceleration);
}

/// Pans the camera to a point in the room, or cuts there when pan is 0.
static inline void FieldCamera_MoveTo(s32 fixed_x, s32 fixed_y, s32 fixed_z,
                                      s32 pan) {
  FieldCamera_MoveWithinMapBounds(fixed_x, fixed_y, fixed_z, pan);
}

/// Changes part of the room in place, copying whole cells, picture and
/// metatile behavior alike, from another part of the map.
static inline void MapGrid_CopyCells(s32 sourceX, s32 sourceY, s32 width,
                                     s32 height, s32 destinationX,
                                     s32 destinationY) {
  MapGrid_CopyMetatileCellsRect(sourceX, sourceY, width, height, destinationX,
                                destinationY);
}

/// Changes where the party can walk in part of the room, copying only the
/// cells' metatile behaviors and flags.
static inline void MapGrid_CopyCellAttributes(s32 sourceX, s32 sourceY,
                                              s32 width, s32 height,
                                              s32 destinationX,
                                              s32 destinationY) {
  MapGrid_CopyCellAttributeRect(sourceX, sourceY, width, height, destinationX,
                                destinationY);
}

/// Sets shake amplitudes in fixed-point pixels and the per-frame decay factor.
/// A negative argument keeps that component unchanged; Q16_ONE keeps the shake.
static inline void MapGrid_SetShake(s32 horizontal, s32 vertical, s32 decay) {
  MapGrid_SetScreenShake(horizontal, vertical, decay);
}

// Most events call GameFlag_Get, GameFlag_Set and GameFlag_Clear
// directly; the events that match only through an inline call spell them
// Event_GetFlag, Event_SetFlag and Event_ClearFlag. An inline call's
// argument is a pseudo of its own, so a flag number two calls share is loaded
// twice instead of kept in a register (MATCHING.md).

/// GameFlag_Get, from an event: 1 when the FLAG_ is set.
static inline s32 Event_GetFlag(s32 flag) { return GameFlag_Get(flag); }

/// GameFlag_Set, from an event.
static inline void Event_SetFlag(s32 flag) { GameFlag_Set(flag); }

/// GameFlag_Clear, from an event.
static inline void Event_ClearFlag(s32 flag) { GameFlag_Clear(flag); }

#ifndef FIELD_STAGED_ACTOR_IMPORTS
/// Plays a sound effect or music from an event: the SE command of script.h.
static inline void Event_PlaySong(s32 cue) { Sound_PlaySong(cue); }
#endif

/// A random number from 0 to 0xffff for an event.
static inline s32 Random_Next(void) { return Import_RandomNext(); }

/// Signed division written as a call; the `/` compiles to __divsi3.
static inline s32 Math_Divide(s32 dividend, s32 divisor) {
  return dividend / divisor;
}

/// Sends an actor toward a point, where it coasts to a stop.
static inline void FieldActor_SetMoveTarget(struct Actor *actor, s32 fixed_x,
                                            s32 fixed_y, s32 fixed_z) {
  Actor_SetMoveTarget(actor, fixed_x, fixed_y, fixed_z);
}

/// Sets the colour mode (SpriteColorMode) of a one-sprite actor's layers,
/// except those drawn hidden.
static inline void Actor_SetColorMode(struct Actor *actor, s32 mode) {
  Anim_ApplyChildValues(actor, mode);
}

/// Creates an actor in an empty slot of gActors, outside the actor ids, or
/// returns NULL when every slot is taken.
static inline struct Actor *FieldActor_Create(s32 type, s32 fixed_x,
                                              s32 fixed_y, s32 fixed_z) {
  return Actor_Create(type, fixed_x, fixed_y, fixed_z);
}

/// OAM blend modes for Actor_SetBlendMode:
/// ACTOR_BLEND_TRANSLUCENT alpha-blends the sprite with what lies behind it.
enum ActorBlendMode { ACTOR_BLEND_NORMAL = 0, ACTOR_BLEND_TRANSLUCENT = 1 };

/// Plays a cell animation, such as a door opening, and holds the event until
/// its last step.
static inline void MapGrid_AnimateCells(const u16 *steps, s32 destinationX,
                                        s32 destinationY) {
  MapGrid_PlayMetatileCopySequence((u16 *)steps, destinationX, destinationY);
}

/// Moves a treasure spot's actor to a point on the ground.
static inline void TreasureSpot_SetPosition(s32 object, s32 fixed_x,
                                            s32 fixed_z) {
  TreasureSpot_SetActorPosition(object, fixed_x, fixed_z);
}

/// Lines an actor up with the middle of its column, then walks it
/// straight along z.
static inline void Actor_CenterAndWalk(s32 actorId, s32 priority, s32 dz) {
  Actor_CenterOnColumn(actorId, priority, dz);
}

/// Walks an actor some distance and waits until it is there.
static inline void Actor_WalkByAndWait(s32 actorId, s32 dx, s32 dz) {
  Actor_CommitPositionAndActivate(actorId, dx, dz);
}

/// Shows a line of dialogue, then pauses before the event goes on.
static inline void FieldEvent_ShowMessageThenWait(s32 speaker, s32 flags,
                                                  s32 frames) {
  Event_ShowMessageThenWait(speaker, flags, frames);
}

/// Sets a sprite param on every part of an actor's sprite, or makes it
/// blink.
static inline void Actor_SetSpriteColorById(s32 actorId, s32 value) {
  Actor_ConfigureSpriteColor(actorId, value);
}

/// Not used in TBS.
#define ACTION_TABLE_STOP ((const u8 *)1)

/// Changes what part of the room shows, copying the cells' pictures but not
/// their metatile behaviors.
static inline void MapGrid_CopyCellsTo(s32 sourceX, s32 sourceY,
                                       s32 destinationX, s32 destinationY,
                                       s32 width, s32 height) {
  MapGrid_CopyMetatileIndicesRect(sourceX, sourceY, destinationX, destinationY,
                                  width, height);
}

/// Changes the pictures of part of the room as MapGrid_CopyCellsTo does,
/// through MapGrid_CopyCellIndexRect, which takes the size first.
static inline void MapGrid_CopyCellsLayered(s32 sourceX, s32 sourceY,
                                            s32 destinationX, s32 destinationY,
                                            s32 width, s32 height) {
  MapGrid_CopyCellIndexRect(sourceX, sourceY, destinationX, destinationY, width,
                            height);
}

/// Sets the colours the screen shows now, where an event's palette fade starts.
static inline void PaletteFade_ApplySource(s32 value, s32 mode) {
  PaletteFade_SetSource(value, mode);
}

/// Sets the colours an event's palette fade ends on.
static inline void PaletteFade_ApplyTarget(s32 value, s32 mode) {
  PaletteFade_SetTarget(value, mode);
}

/// Moves an actor to a point in the pose it has and waits for it.
static inline void Actor_MoveToAndWait(s32 actorId, s32 x, s32 z) {
  Actor_SetPositionAndCommit(actorId, x, z);
}

/// Walks an actor to a point and waits for it, then stands it still.
static inline void Actor_WalkToAndWait(s32 actorId, s32 x, s32 z) {
  Actor_SetPositionAndReset(actorId, x, z);
}

/// Has two actors speak at once, each window placed by the event.
static inline void
FieldEvent_ShowTwoMessagesAndWait(s32 speaker, s32 x, s32 y, s32 arg, s32 extra,
                                  s32 otherSpeaker, s32 otherX, s32 otherY,
                                  s32 otherArg, s32 otherExtra, s32 flags) {
  Import_EventShowTwoMessagesAndWait(speaker, x, y, arg, extra, otherSpeaker,
                                     otherX, otherY, otherArg, otherExtra,
                                     flags);
}

#endif
