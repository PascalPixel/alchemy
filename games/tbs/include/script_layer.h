#ifndef ALCHEMY_SCRIPT_LAYER_H
#define ALCHEMY_SCRIPT_LAYER_H

/// @file
/// The script layer: the constants and small inlines a map's scripts share,
/// over the main-image routines they call by those routines' own names
/// (FieldScript_Start, ObjectMotion_Launch, FieldScript_SetMessageId and the
/// rest), whose declarations it includes.
///
/// It is a habit of the whole game written once, as declarations and macros: a
/// layer, not a compiler-steering device. The script.h commands
/// and field_service.h are written over it. A map's script reads
/// `SCRIPT_START(); ObjectEvent_Stop(9); OBJECT_FACE(9, LOCALID_PARTY_LEADER,
/// 0); SCRIPT_END();`.

#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
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

// A map stores into its own const tables: an overlay's data is in RAM.
// The games' compiler takes the store as written; a current GCC refuses it,
// so the modern build names the place without its const.
#ifdef MODERN
/// The comma expression removes qualifiers without promoting narrow integers.
#define STORE_CONST(place) (*(__typeof__(((void)0, (place))) *)&(place))
#else
#define STORE_CONST(place) (place)
#endif

/// 0x78 bytes of EWRAM where map overlays keep state of their own, such as
/// Altmiller Cave's rock puzzle. It lies outside the heap, so a map change
/// leaves it alone; SaveBlock_InitializeDefaults clears it for a new game.
extern u8 gSceneLocalState[];

/// The Colosso race work, heap slot HEAP_SLOT_COLOSSO_RACE under its own name.
/// Colosso's code reads it under either name, each site as it matched; the
/// slot's literal is gHeapSlots+236, the same address through another
/// relocation, so no byte differs. Where only the gauge is read, a struct
/// ColossoGaugeWork pointer (its first member) compiles as ->gauge does.
extern u8 *gColossoWork;

#include "frame.h"

#include "object_event.h"

/// The main loop pass's task priority, as TASK_PRIORITY_MAIN_LOOP: a map's
/// per-frame callbacks run once a frame with the field's own.
enum { TASK_PRIORITY_MAP = 3200 };

/// Animation numbers of an object event's sprite, for
/// Import_ObjectEventSetAnimation.
enum ObjectEventAnimation {
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
  /// The leader leaning into what it pushes, as maps play it while a statue
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

// Speaker bits, above the local id in the low 12 bits of a message command's
// speaker, as FieldMessage_ShowAtObject reads them.
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

// The imports below keep a name of their own, prefixed Import_: the maps' calls
// of these routines were compiled against declarations that differ from the
// routines' own, as the bytes measure. Import_ScriptRequestExit,
// Import_ObjectEventSetPosition, Import_ObjectEventSetAnimation and
// Import_RandomNext are called with differing argument counts, so they keep
// old-style declarations; FieldScript_SetExitRequest also takes a u16 and
// Random16 returns a u32, where the maps pass and read an int. Imil's and Sol
// Sanctum's Psynergy Stone room's calls of GameFlag_Set, which returns the
// flag, compile only as calls of a void routine (Import_GameFlagSet), where
// every other map's match through GameFlag_Set itself, and the maps pass
// FieldScript_ShowTwoMessagesAndWait eleven arguments where it takes ten
// (Import_ScriptShowTwoMessagesAndWait).
/// Sets the field's exit request (FieldScript_SetExitRequest); 999 leaves for
/// gSaveBlock's map and entrance.
void Import_ScriptRequestExit();
/// Puts an object event at rest at (x, z), in 16.16 pixels; one that follows
/// the ground keeps its height above the terrain
/// (ObjectMotion_SetHorizontalPositionWithTerrain).
void Import_ObjectEventSetPosition();
/// Switches an object event's sprite to an ObjectEventAnimation
/// (ObjectEvent_SetModeById).
void Import_ObjectEventSetAnimation();
/// Sets a game flag (GameFlag_Set), for the two maps whose calls compile only
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
/// The engine encodes either a built-in action or an action-script address.
static inline void ObjectEvent_SetActionScript(s32 localId, const s32 *script) {
  ObjectMotion_EnableActionAndSetCallback(localId, (s32)script);
}

/// Runs a script, waiting at most 300 frames for its final command.
static inline void ObjectEvent_RunActionScriptAndWait(s32 localId,
                                                      const s32 *script) {
  ObjectEvent_SetActionCallbackAndRefreshById(localId, (s32)script);
}

/// Starts one of the engine's own actions on an object event, an enum
/// MovementType, where ObjectEvent_SetActionScript gives it a map's script.
static inline void ObjectEvent_SetBuiltinAction(s32 localId, s32 action) {
  ObjectMotion_EnableActionAndSetCallback(localId, action);
}

/// Two object events speak at once, each message and portrait at its own tile
/// position, until A, B, L or R closes both
/// (FieldScript_ShowTwoMessagesAndWait). The routine takes ten arguments and
/// never reads flags.
void Import_ScriptShowTwoMessagesAndWait(s32 speaker, s32 x, s32 y, s32 arg,
                                         s32 extra, s32 other_speaker,
                                         s32 otherX, s32 other_y, s32 other_arg,
                                         s32 other_extra, s32 flags);

#ifdef FIELD_STAGED_OBJ_EVENT_IMPORTS
/// Declared for the maps that define FIELD_STAGED_OBJ_EVENT_IMPORTS in place
/// of Script_PlaySong. Nothing calls it and no import resolves it: those maps
/// call Sound_PlaySong.
void Audio_PlayCue(s32 cue);
#endif

/// Moves the script's message number count messages on, past lines the script
/// doesn't show.
static inline void Script_SkipMessages(s32 count) {
  u16 *message = &gFieldWork->message;

  *message = *message + count;
}

/// Shows the script's next line of dialogue and waits for the player to read
/// it.
static inline void Script_ShowMessage(s32 speaker, s32 flags) {
  FieldScript_ShowMessageAndWait(speaker, flags);
}

/// Opens the script's next line and returns its window once the text is typed
/// out, so the script can act while it is still on screen.
static inline s32 Script_OpenMessage(s32 speaker, s32 flags) {
  return (s32)FieldMessage_ShowAtObject(speaker, flags);
}

/// Puts the current line to the player as a question and shows the line that
/// answers it; returns 0 for yes.
static inline s32 Script_AskYesNo(s32 speaker, s32 flags) {
  return FieldScript_AskYesNoAndShowResponse(speaker, flags);
}

/// Looks up an object event for a script, NULL when it doesn't exist
/// (ObjectEvent_GetById).
static inline struct ObjectEvent *ObjectEvent_Get(s32 localId) {
  return ObjectEvent_GetById(localId);
}

/// Places an object event at a point at once, without walking it there.
static inline void ObjectEvent_SetPosition(s32 localId, s32 fixed_x,
                                           s32 fixed_z) {
  Import_ObjectEventSetPosition(localId, fixed_x, fixed_z);
}

/// Sets how fast an object event moves, and how quickly it gets up to speed,
/// for the script's next moves.
static inline void ObjectEvent_SetSpeed(s32 localId, s32 speed,
                                        s32 acceleration) {
  ObjectMotion_SetSpeedParameters(localId, speed, acceleration);
}

/// Moves an object event to a point in the pose it has, without walking; the
/// script carries on meanwhile.
static inline void ObjectEvent_SetDestination(s32 localId, s32 x, s32 z) {
  ObjectMotion_ResetAndSetPosition(localId, x, z);
}

#ifndef FIELD_STAGED_OBJ_EVENT_IMPORTS
/// Moves an object event some distance from where it stands, without waiting
/// for it.
static inline void ObjectEvent_SetDestinationOffset(s32 localId, s32 dx,
                                                    s32 dz) {
  ObjectMotion_OffsetPositionAndResetMotion(localId, dx, dz);
}
#endif

/// Walks an object event to a point; the script goes on while it walks.
static inline void ObjectEvent_WalkTo(s32 localId, s32 x, s32 z) {
  ObjectMotion_ResetAndSetPositionInMode2(localId, x, z);
}

/// Walks an object event some distance from where it stands, without waiting
/// for it to get there.
static inline void ObjectEvent_WalkBy(s32 localId, s32 dx, s32 dz) {
  ObjectMotion_OffsetPositionAndReset(localId, dx, dz);
}

/// Turns an object event to a direction and holds the script for a few frames.
static inline void ObjectEvent_FaceDirection(s32 localId, s32 facing,
                                             s32 frames) {
  ObjectMotion_ArmCallback(localId, facing, frames);
}

/// Has one object event look at another at once, then pauses the script.
static inline void ObjectEvent_FaceObjectEvent(s32 localId, s32 target,
                                               s32 frames) {
  ObjectMotion_SetAngleToward(localId, target, frames);
}

/// Pops an emote bubble up over an object event, then lets frames pass before
/// the script goes on.
static inline void ObjectEvent_ShowEmote(s32 localId, s32 emote, s32 frames) {
  FieldEffect_SpawnLinked(localId, emote, frames);
}

/// Attaches an effect object to an object event, takes it off, or plays one
/// over it once.
static inline void ObjectEvent_SetAttachedEffect(s32 localId, s32 effect) {
  FieldEffect_ConfigureLinkedObject(localId, effect);
}

/// Sets how fast the camera pans for the script's next camera moves.
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

// Most map scripts call GameFlag_Get, GameFlag_Set and GameFlag_Clear
// directly; the scripts that match only through an inline call spell them
// Script_GetFlag, Script_SetFlag and Script_ClearFlag. An inline call's
// argument is a pseudo of its own, so a flag number two calls share is loaded
// twice instead of kept in a register (MATCHING.md).

/// GameFlag_Get, from a script: 1 when the FLAG_ is set.
static inline s32 Script_GetFlag(s32 flag) { return GameFlag_Get(flag); }

/// GameFlag_Set, from a script.
static inline void Script_SetFlag(s32 flag) { GameFlag_Set(flag); }

/// GameFlag_Clear, from a script.
static inline void Script_ClearFlag(s32 flag) { GameFlag_Clear(flag); }

#ifndef FIELD_STAGED_OBJ_EVENT_IMPORTS
/// Plays a sound effect or music from a script: the SE command of script.h.
static inline void Script_PlaySong(s32 cue) { Sound_PlaySong(cue); }
#endif

/// A random number from 0 to 0xffff for a script.
static inline s32 Random_Next(void) { return Import_RandomNext(); }

/// Signed division written as a call; the `/` compiles to __divsi3.
static inline s32 Math_Divide(s32 dividend, s32 divisor) {
  return dividend / divisor;
}

// The main image reads an object through struct Object, where the maps read the
// same object through struct ObjectEvent; these inlines pass a map's object
// event to it.

/// Turns a one-sprite object event's shadow on or off, bit 0 of its sprite's
/// flags.
static inline void ObjectEvent_SetSpriteFlags(struct ObjectEvent *objectEvent,
                                              s32 flags) {
  Object_SetSpriteFlags((struct Object *)objectEvent, flags);
}

/// Sends an object toward a point, where it coasts to a stop.
static inline void Object_SetPosition(struct ObjectEvent *objectEvent,
                                      s32 fixed_x, s32 fixed_y, s32 fixed_z) {
  Object_SetMoveTarget((struct Object *)objectEvent, fixed_x, fixed_y, fixed_z);
}

/// Releases an object and its sprites, and empties its place in the object
/// table.
static inline void ObjectEvent_Release(struct ObjectEvent *objectEvent) {
  Object_Release((struct Object *)objectEvent);
}

/// Sets the colour mode (SpriteColorMode) of a one-sprite object's layers,
/// except those drawn hidden.
static inline void ObjectEvent_SetColorMode(struct ObjectEvent *objectEvent,
                                            s32 mode) {
  Anim_ApplyChildValues((struct Object *)objectEvent, mode);
}

/// Creates a free object in the room, outside the object event table, or
/// returns NULL when every object is taken.
static inline struct ObjectEvent *ObjectEvent_Create(s32 type, s32 fixed_x,
                                                     s32 fixed_y, s32 fixed_z) {
  return (struct ObjectEvent *)Object_CreateField(type, fixed_x, fixed_y,
                                                  fixed_z);
}

/// Starts an object on a sprite script from its first command.
static inline void ObjectEvent_SetScript(struct ObjectEvent *objectEvent,
                                         const s32 *script) {
  Object_Initialize((struct Object *)objectEvent, script);
}

/// OAM blend modes for Object_SetSpriteBlendMode:
/// OBJECT_BLEND_TRANSLUCENT alpha-blends the sprite with what lies behind it.
enum ObjectBlendMode { OBJECT_BLEND_NORMAL = 0, OBJECT_BLEND_TRANSLUCENT = 1 };

/// Sets a one-sprite object's OAM blend mode, an ObjectBlendMode.
static inline void ObjectEvent_SetBlendMode(struct ObjectEvent *objectEvent,
                                            s32 mode) {
  Object_SetSpriteBlendMode((struct Object *)objectEvent, mode);
}

/// Plays a cell animation, such as a door opening, and holds the script until
/// its last step.
static inline void MapGrid_AnimateCells(const u16 *steps, s32 destinationX,
                                        s32 destinationY) {
  MapGrid_PlayMetatileCopySequence((u16 *)steps, destinationX, destinationY);
}

/// Moves a treasure spot's object event to a point on the ground.
static inline void TreasureSpot_SetPosition(s32 object, s32 fixed_x,
                                            s32 fixed_z) {
  TreasureSpot_SetObjectEventPosition(object, fixed_x, fixed_z);
}

/// Lines an object event up with the middle of its column, then walks it
/// straight along z.
static inline void ObjectEvent_CenterAndWalk(s32 localId, s32 priority,
                                             s32 dz) {
  ObjectMotion_CenterOnColumn(localId, priority, dz);
}

/// Walks an object event some distance and waits until it is there.
static inline void ObjectEvent_WalkByAndWait(s32 localId, s32 dx, s32 dz) {
  ObjectMotion_CommitPositionAndActivate(localId, dx, dz);
}

/// Shows a line of dialogue, then pauses before the script goes on.
static inline void Script_ShowMessageAndWait(s32 speaker, s32 flags,
                                             s32 frames) {
  FieldScript_ShowMessageThenWait(speaker, flags, frames);
}

/// Sets a sprite param on every part of an object event's sprite, or makes it
/// blink.
static inline void ObjectEvent_SetSpriteColorById(s32 localId, s32 value) {
  ObjectEvent_ConfigureSpriteColor(localId, value);
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

/// Sets the colours the screen shows now, where a script's palette fade starts.
static inline void PaletteFade_ApplySource(s32 value, s32 mode) {
  PaletteFade_SetSource(value, mode);
}

/// Sets the colours a script's palette fade ends on.
static inline void PaletteFade_ApplyTarget(s32 value, s32 mode) {
  PaletteFade_SetTarget(value, mode);
}

/// Moves an object event to a point in the pose it has and waits for it.
static inline void ObjectEvent_MoveToAndWait(s32 localId, s32 x, s32 z) {
  ObjectMotion_SetPositionAndCommit(localId, x, z);
}

/// Walks an object event to a point and waits for it, then stands it still.
static inline void ObjectEvent_WalkToAndWait(s32 localId, s32 x, s32 z) {
  ObjectMotion_SetPositionAndReset(localId, x, z);
}

/// Has two object events speak at once, each window placed by the script.
static inline void Script_ShowTwoMessagesAndWait(s32 speaker, s32 x, s32 y,
                                                 s32 arg, s32 extra,
                                                 s32 otherSpeaker, s32 otherX,
                                                 s32 otherY, s32 otherArg,
                                                 s32 otherExtra, s32 flags) {
  Import_ScriptShowTwoMessagesAndWait(speaker, x, y, arg, extra, otherSpeaker,
                                      otherX, otherY, otherArg, otherExtra,
                                      flags);
}

#endif
