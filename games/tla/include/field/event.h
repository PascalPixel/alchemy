#ifndef ALCHEMY_EVENT_H
#define ALCHEMY_EVENT_H

/// @file
/// The calls events are written in: the small inlines a scene's events share,
/// over the main-image routines they call by those routines' own names
/// (Actor_FaceDirectionById, Event_ShowMessageAndWait, Actor_SetSpeedParameters
/// and the rest), whose declarations it holds. An overlay's import stub takes
/// the name of the routine it reaches, except the few the overlays share under
/// another (Import_ActorCreate, Import_EventSetExitRequest).
/// event_inline.h's inlines are written over them. An event reads
/// `Actor_FaceDirection(4, FACING_NORTH, 0); FieldEvent_Wait(20);`.

#include "field/emote.h"
#include "field/speaker.h"
#include "fixed_math.h"
#include "sounds.h"
#include "types.h"
#include "system/iwram_call.h"
#include "system/task.h"

struct Actor;

/// One step of a cell animation: copy the width by height cells at sourceX,
/// sourceY over the destination, then wait frames.
#define MAPGRID_ANIMATION_STEP(sourceX, sourceY, width, height, frames)        \
  (sourceX), (sourceY), (width), (height), (frames)

/// A cell animation, such as a door opening: its steps and the cell they are
/// copied to (MapGrid_PlayMapEdit, which blocks until the last).
struct MapGridCellAnimation {
  /// MAPGRID_ANIMATION_STEP entries, ended by MAPGRID_ANIMATION_STEPS_END, for
  /// MapGrid_PlayMapEdit.
  const u16 *steps;
  u16 x; ///< destination cell column
  u16 y; ///< destination cell row
};

void Event_ShowMessageAndWait(s32 actor, s32 flags);
void Event_EndLastingPsynergy(s32 skip);
s32 Party_GetLeaderActor(void);
void Event_Wait(s32 frames);
void Event_Start(void);
void Event_SetDestination(s32 scene, s32 entrance);
void Event_SetExitRequest(u16 exit);
void Event_SetMessageId(s32 message);
s32 FieldMessage_ShowAtActor(s32 speaker, s32 flags);
s32 Event_AskYesNoAndShowResponse(s32 speaker, s32 flags);
struct Actor *Actor_GetById(u32 actorId);
void Actor_SetHorizontalPositionWithTerrain(u32 actorId, FIXED x, FIXED z);
void Actor_SetSpeedParameters(s32 actorId, FIXED speed, FIXED acceleration);
void Actor_SetDestinationById(u32 actorId, s32 x, s32 z);
void Actor_SetDestinationOffsetById(s32 actorId, s32 dx, s32 dz);
void Actor_WalkToById(s32 actorId, s32 x, s32 z);
void Actor_WalkByById(u32 actorId, s32 xOffset, s32 zOffset);
void Actor_FaceDirectionById(s32 actorId, s32 angle, s32 frames);
void Actor_FaceActorById(s32 actorId, s32 target, s32 frames);
struct Actor *Actor_ShowEmoteById(s32 actorId, s32 emote, s32 frames);
void FieldEffect_SetAttached(s32 actorId, s32 effect);
void Camera_SetSpeedParameters(FIXED speed, FIXED acceleration);
void FieldCamera_MoveWithinMapBounds(FIXED x, FIXED y, FIXED z, s32 pan);
void FieldCamera_MoveToActorById(s32 actorId, s32 glide);
void FieldCamera_WaitForArrival(void);
void Map_CopyCellsRect(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
                       s32 dstY);
void Map_CopyCellAttributeRect(s32 srcX, s32 srcY, s32 width, s32 height,
                               s32 dstX, s32 dstY);
void MapGrid_SetScreenShake(FIXED horizontal, FIXED vertical, FIXED decay);
s32 GameFlag_Get(s32 flag);
void GameFlag_Set(s32 flag);
void GameFlag_Clear(s32 flag);
void Sound_Play(s32 sound);
void Actor_SetMoveTarget(struct Actor *actor, FIXED x, FIXED y, FIXED z);
/// Creates an actor at (x, y, z), 16.16 pixels, or returns 0 when every slot is
/// taken (Actor_Create, through the overlay's own import row).
struct Actor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
void MapGrid_PlayMapEdit(const void *steps, s32 x, s32 y);
void TreasureSpot_SetActorPosition(u32 actorId, FIXED x, FIXED z);
void Actor_CenterOnColumn(u32 actorId, s32 priority, s32 zOffset);
void Actor_WalkByAndWaitById(u32 actorId, s32 xOffset, s32 zOffset);
void Event_ShowMessageThenWait(s32 speaker, s32 flags, s32 frames);
void Actor_ConfigureSpriteColor(s32 actorId, s32 colorMode);
void MapGrid_CopyCellTilesIndicesRect(s32 sourceX, s32 sourceY,
                                      s32 destinationX, s32 destinationY,
                                      s32 width, s32 height);
void MapGrid_CopyCellTilesCellsRect(s32 sourceX, s32 sourceY, s32 width,
                                    s32 height, s32 destinationX,
                                    s32 destinationY);
void PaletteFade_SetSource(s32 value, s32 mode);
void PaletteFade_SetTarget(s32 value, s32 mode);
void Actor_MoveToAndWaitById(u32 actorId, s32 x, s32 z);
void Actor_WalkToAndWaitById(s32 actorId, s32 x, s32 z);
void Event_ShowTwoMessagesAndWait(s32 first, s32 firstX, s32 firstY,
                                  s32 firstPortraitX, s32 firstPortraitY,
                                  s32 second, s32 secondX, s32 secondY,
                                  s32 secondPortraitX, s32 secondPortraitY,
                                  s32 delay);
void Actor_SetMovementTypeById(s32 actorId, const void *script);
void Actor_SetMovementTypeAndWaitById(s32 actorId, const void *script);
void Actor_TurnToFaceEachOther(s32 actorId, s32 other, s32 frames);
void Actor_SetAnimationById(u32 actorId, s32 animation);

/// What the scenes ask of a treasure spot's actor (Actor_SetWideSprite).
enum TreasureSpotSpriteMode {
  /// Off the map at x and z 0, marked TREASURE_SPOT_ON_CELL.
  TREASURE_SPOT_PUT_AWAY = 0,
  TREASURE_SPOT_WIDE = 1 ///< (or any other) drawn 16 by 8, 4 pixels lower
};

/// What Actor_ReturnHome does with a buried treasure's actor.
enum ReturnHomeMode {
  RETURN_HOME_CLEAR_Z = 1, ///< its z set to 0
  /// Scoop has dug it up: its flag set, back at its home, dropped from hand.
  RETURN_HOME_DUG_UP = 2
};

void Actor_ReturnHome(s32 mode, u32 actorId, s32 flag);
void Actor_SetWideSprite(u32 actorId, s32 mode);
void PaletteFade_StartInterpolation(s32 frames);
void Icon_DrawItem(s32 item);
FIXED Trig_Sin(s32 angle);
void FieldMessage_Show(s32 message, s32 flags);
s32 PartyInventory_GiveItem(s32 item, s32 flags);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
void Actor_LaunchFromLeader(s32 actorId, s32 dx, s32 dz, s32 facing);
void Vector_AddPolarOffset(FIXED radius, s32 heading, s32 *probe);
void MapGrid_SetCellBehaviorAt(s32 layer, FIXED x, FIXED y, s32 behavior);
void WorldMap_SetWindowCellTile(s32 x, s32 y, s32 cellX, s32 cellY);
void Message_DecodeRawUnbounded(s32 message, u16 *buffer);
void MapGrid_SetCameraBounds(s32 minX, s32 minY, s32 maxX, s32 maxY);
s32 GameFlag_GetByte(s32 flag);
s32 Party_ListMembers(s16 *owners);
s32 Unit_AddHp(s32 owner, s32 delta);
void wait(s32 frames);
void Actor_FaceDirectionAndRefreshById(s32 actorId, s32 angle);
void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 actorId, s32 x, s32 z,
                                                    s32 angle);
void Actor_SetAnimationAndWait(s32 actorId, s32 animation);
void Actor_Jump(s32 actorId, s32 speed, s32 frames);
void Actor_RunRepeatedMotion(u32 actorId, s32 count);
void Actor_SetSpritePriority(u32 actorId, s32 priority);
void Actor_StartRepeatedMotion(u32 actorId, s32 count);
void Actor_WatchAndSetScript(s32 actorId, s32 watchedActorId);
void Actor_WaitForMoveById(s32 actorId);
void FieldCamera_FollowActor(s32 actorId, s32 keepPosition);
void Actor_SetPositionAndResetMotion(struct Actor *actor, FIXED x, FIXED y,
                                     FIXED z);
void Actor_WaitForScriptEndById(u32 actorId);
void Actor_SetSpriteColor(struct Actor *actor, s32 colorMode);
void Actor_Stop(s32 actorId);
/// Sets the field's exit request (Event_SetExitRequest, through the overlay's
/// own import row).
void Import_EventSetExitRequest(s32 exit);
void Actor_WaitForMove(struct Actor *actor);
s32 PartyInventory_FindMember(s32 itemId);
void FieldMessage_ShowCentered(s32 message, s32 mode, s32 yOffset);
s32 Event_AskYesNoWithActorAnimation(s32 actorId, s32 flags);
void Event_SetAfterBattleDestination(s32 sceneId, s32 entrance);
void Event_SetDefeatDestination(s32 sceneId, s32 entrance);
void Djinn_JoinParty(s32 slot, s32 element, s32 djinn);
void FieldPsynergy_StartMindReadWave(s32 x, s32 y);
struct Actor *FieldCamera_GetActor(void);
s32 FieldYesNo_Ask(s32 atBottom);
void WorldMap_StartZoom(FIXED zoom, s32 frames);
void WindowColor_Apply(s32 color, s32 brightness);
void Field_RequestDjinnBattle(u32 flags, u32 djinn);
void Event_AddPartyMember(s32 unit, s32 announce);
void MapGrid_WaitForShakeToSettle(void);
s32 MapGrid_GetCellBehaviorAt(s32 layer, FIXED x, FIXED y);
void Actor_SetAnimationSpeedById(s32 actorId, s32 steps);
void Actor_ShowItemIconById(s32 actorId, s32 item);
void Actor_SetItemIcon(struct Actor *actor, s32 item);
void WorldMap_SetWindowCellTileAndDraw(s32 x, s32 y, s32 cellX, s32 cellY);
void FieldCamera_MoveToActor(s32 actorId, s32 pan);
void Actor_SetSpriteRotationById(s32 actorId, s32 angle);
u8 MapGrid_SetCellBehavior(s32 x, s32 z, s32 layer, s32 behavior);

/// Shows the event's next line of dialogue and waits for the player to read
/// it.
static inline void Event_ShowMessage(s32 speaker, s32 flags) {
  Event_ShowMessageAndWait(speaker, flags);
}

/// Opens the event's next line and returns its window once the text is typed
/// out, so the event can act while it is still on screen.
static inline s32 Event_OpenMessage(s32 speaker, s32 flags) {
  return FieldMessage_ShowAtActor(speaker, flags);
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

/// Places an actor at a point at once, without walking it there; `x` and `z`
/// are 16.16 fixed-point pixels, as a stored position holds them.
static inline void Actor_SetPositionFX(s32 actorId, FIXED x, FIXED z) {
  Actor_SetHorizontalPositionWithTerrain(actorId, x, z);
}

/// Places an actor at a point in whole pixels at once, without walking it
/// there: `Actor_SetPosition(8, 880, 502)`.
#define Actor_SetPosition(actorId, x, z)                                       \
  Actor_SetPositionFX(actorId, toFIXED(x), toFIXED(z))

/// Sets how fast an actor moves, and how quickly it gets up to speed, for the
/// event's next moves; both are 16.16 fixed point, as stored values hold them.
static inline void Actor_SetSpeedFX(s32 actorId, FIXED speed,
                                    FIXED acceleration) {
  Actor_SetSpeedParameters(actorId, speed, acceleration);
}

/// Sets an actor's top speed for the event's next moves, in pixels a frame,
/// and an acceleration of half that: `Actor_SetSpeed(8, 0.5)`.
#define Actor_SetSpeed(actorId, speed)                                         \
  Actor_SetSpeedFX(actorId, toFIXED(speed), toFIXED(speed) / 2)

/// Sets an actor's top speed, in pixels a frame, and its acceleration, in
/// pixels a frame per frame, where the acceleration isn't half the speed.
#define Actor_SetSpeedAndAcceleration(actorId, speed, acceleration)            \
  Actor_SetSpeedFX(actorId, toFIXED(speed), toFIXED(acceleration))

/// Moves an actor to a point in the pose it has, without walking; the
/// event carries on meanwhile.
static inline void Actor_SetDestination(s32 actorId, s32 x, s32 z) {
  Actor_SetDestinationById(actorId, x, z);
}

/// Moves an actor some distance from where it stands, without waiting
/// for it.
static inline void Actor_SetDestinationOffset(s32 actorId, s32 dx, s32 dz) {
  Actor_SetDestinationOffsetById(actorId, dx, dz);
}

/// Walks an actor to a point; the event goes on while it walks.
static inline void Actor_WalkTo(s32 actorId, s32 x, s32 z) {
  Actor_WalkToById(actorId, x, z);
}

/// Walks an actor some distance from where it stands, without waiting
/// for it to get there.
static inline void Actor_WalkBy(s32 actorId, s32 dx, s32 dz) {
  Actor_WalkByById(actorId, dx, dz);
}

/// Turns an actor to a direction and holds the event for a few frames.
static inline void Actor_FaceDirection(s32 actorId, s32 facing, s32 frames) {
  Actor_FaceDirectionById(actorId, facing, frames);
}

/// Has one actor look at another at once, then pauses the event.
static inline void Actor_FaceActor(s32 actorId, s32 target, s32 frames) {
  Actor_FaceActorById(actorId, target, frames);
}

/// Pops an emote bubble up over an actor, then lets frames pass before
/// the event goes on.
static inline struct Actor *Actor_ShowEmote(s32 actorId, s32 emote,
                                            s32 frames) {
  return Actor_ShowEmoteById(actorId, emote, frames);
}

/// Actor_ShowEmote, for an event that leaves its result unread.
static inline void FieldActor_ShowEmote(s32 actorId, s32 emote, s32 frames) {
  Actor_ShowEmoteById(actorId, emote, frames);
}

/// Attaches a field effect to an actor, takes it off, or plays one
/// over it once.
static inline void Actor_SetAttachedEffect(s32 actorId, s32 effect) {
  FieldEffect_SetAttached(actorId, effect);
}

/// Sets how fast the camera pans for the event's next camera moves, and how
/// quickly it gets up to speed, both in 16.16 fixed point.
static inline void FieldCamera_SetSpeedFX(FIXED speed, FIXED acceleration) {
  Camera_SetSpeedParameters(speed, acceleration);
}

/// Sets the camera's top panning speed for the event's next camera moves, in
/// pixels a frame, and an acceleration of an eighth of that:
/// `FieldCamera_SetSpeed(0.8)`.
#define FieldCamera_SetSpeed(speed)                                            \
  FieldCamera_SetSpeedFX(toFIXED(speed), toFIXED(speed) / 8)

/// Sets the camera's top panning speed, in pixels a frame, and its
/// acceleration, in pixels a frame per frame, where the acceleration isn't an
/// eighth of the speed.
#define FieldCamera_SetSpeedAndAcceleration(speed, acceleration)               \
  FieldCamera_SetSpeedFX(toFIXED(speed), toFIXED(acceleration))

/// Keeps a camera axis where it is (FieldCamera_MoveTo).
#define CAMERA_KEEP (-1)

/// Pans the camera to a point in the room, or cuts there when pan is 0; the
/// axes are 16.16 fixed-point pixels, or CAMERA_KEEP.
static inline void FieldCamera_MoveToFX(FIXED x, FIXED y, FIXED z, s32 pan) {
  FieldCamera_MoveWithinMapBounds(x, y, z, pan);
}

/// A camera axis in whole pixels as 16.16 fixed point, CAMERA_KEEP kept; it
/// reads its argument twice, so the macros below take only constants.
#define EVENT_CAMERA_FIXED(axis)                                               \
  ((axis) == CAMERA_KEEP ? CAMERA_KEEP : toFIXED(axis))

/// Pans the camera to a point in whole pixels, or cuts there when pan is 0;
/// CAMERA_KEEP keeps an axis: `FieldCamera_MoveTo(168, CAMERA_KEEP, 158, 1)`.
#define FieldCamera_MoveTo(x, y, z, pan)                                       \
  FieldCamera_MoveToFX(EVENT_CAMERA_FIXED(x), EVENT_CAMERA_FIXED(y),           \
                       EVENT_CAMERA_FIXED(z), pan)

/// Changes part of the room in place, copying whole cells, cell tiles and
/// cell behavior alike, from another part of the map.
static inline void MapGrid_CopyCells(s32 sourceX, s32 sourceY, s32 width,
                                     s32 height, s32 destinationX,
                                     s32 destinationY) {
  Map_CopyCellsRect(sourceX, sourceY, width, height, destinationX,
                    destinationY);
}

/// Changes where the party can walk in part of the room, copying only the
/// cells' cell behaviors and flags.
static inline void MapGrid_CopyCellAttributes(s32 sourceX, s32 sourceY,
                                              s32 width, s32 height,
                                              s32 destinationX,
                                              s32 destinationY) {
  Map_CopyCellAttributeRect(sourceX, sourceY, width, height, destinationX,
                            destinationY);
}

/// Keeps a shake component as it is (MapGrid_SetShake).
#define SHAKE_KEEP (-1)

/// Sets shake amplitudes in 16.16 fixed-point pixels and the per-frame decay
/// factor in 16.16; SHAKE_KEEP keeps that component, FIXED_ONE keeps the shake.
static inline void MapGrid_SetShakeFX(FIXED horizontal, FIXED vertical,
                                      FIXED decay) {
  MapGrid_SetScreenShake(horizontal, vertical, decay);
}

/// A shake amplitude in pixels or a decay factor, as a decimal, in 16.16
/// fixed point, SHAKE_KEEP kept; it reads its argument twice, so the macro
/// below takes only constants.
#define EVENT_SHAKE_FIXED(value)                                               \
  ((value) == SHAKE_KEEP ? SHAKE_KEEP : toFIXED(value))

/// Shakes the screen by amplitudes in pixels, shrunk each frame by a decay
/// factor (1 keeps the shake going); SHAKE_KEEP keeps a component:
/// `MapGrid_SetShake(2, 2, 1)`.
#define MapGrid_SetShake(horizontal, vertical, decay)                          \
  MapGrid_SetShakeFX(EVENT_SHAKE_FIXED(horizontal),                            \
                     EVENT_SHAKE_FIXED(vertical), EVENT_SHAKE_FIXED(decay))

/// GameFlag_Get, from an event: 1 when the flag is set.
static inline s32 Event_GetFlag(s32 flag) { return GameFlag_Get(flag); }

/// GameFlag_Set, from an event.
static inline void Event_SetFlag(s32 flag) { GameFlag_Set(flag); }

/// GameFlag_Clear, from an event.
static inline void Event_ClearFlag(s32 flag) { GameFlag_Clear(flag); }

/// Plays a sound effect or music from an event: the SE command of
/// actor_script.h.
static inline void Event_PlaySound(s32 sound) { Sound_Play(sound); }

/// Divides, rounding toward zero.
static inline s32 Math_Divide(s32 dividend, s32 divisor) {
  return dividend / divisor;
}

/// Sends an actor toward a point in 16.16 fixed-point pixels, where it coasts
/// to a stop.
static inline void FieldActor_SetMoveTargetFX(struct Actor *actor, FIXED x,
                                              FIXED y, FIXED z) {
  Actor_SetMoveTarget(actor, x, y, z);
}

/// Sends an actor toward a point in whole pixels, where it coasts to a stop.
#define FieldActor_SetMoveTarget(actor, x, y, z)                               \
  FieldActor_SetMoveTargetFX(actor, toFIXED(x), toFIXED(y), toFIXED(z))

/// Creates an actor in an empty slot of gActors, outside the actor ids, or
/// returns NULL when every slot is taken.
static inline struct Actor *FieldActor_Create(s32 type, FIXED x, FIXED y,
                                              FIXED z) {
  return (struct Actor *)Import_ActorCreate(type, x, y, z);
}

/// Plays a cell animation, such as a door opening, and holds the event until
/// its last step.
static inline void MapGrid_AnimateCells(const u16 *steps, s32 destinationX,
                                        s32 destinationY) {
  MapGrid_PlayMapEdit(steps, destinationX, destinationY);
}

/// Moves a treasure spot's actor to a point on the ground.
static inline void TreasureSpot_SetPosition(s32 actorId, FIXED x, FIXED z) {
  TreasureSpot_SetActorPosition(actorId, x, z);
}

/// Lines an actor up with the middle of its column, sets its sprite priority,
/// then walks it straight along z.
static inline void Actor_CenterAndWalk(s32 actorId, s32 priority, s32 dz) {
  Actor_CenterOnColumn(actorId, priority, dz);
}

/// Walks an actor some distance and waits until it is there.
static inline void Actor_WalkByAndWait(s32 actorId, s32 dx, s32 dz) {
  Actor_WalkByAndWaitById(actorId, dx, dz);
}

/// Shows a line of dialogue, then pauses before the event goes on.
static inline void FieldEvent_ShowMessageThenWait(s32 speaker, s32 flags,
                                                  s32 frames) {
  Event_ShowMessageThenWait(speaker, flags, frames);
}

/// Sets a colour mode (enum SpriteColorMode) on every part of an actor's
/// sprite, or SPRITE_COLOR_BLINK to make it blink.
static inline void Actor_SetSpriteColorById(s32 actorId, s32 value) {
  Actor_ConfigureSpriteColor(actorId, value);
}

/// Changes what part of the room shows, copying the cells' tiles but not
/// their cell behaviors.
static inline void MapGrid_CopyCellsTo(s32 sourceX, s32 sourceY,
                                       s32 destinationX, s32 destinationY,
                                       s32 width, s32 height) {
  MapGrid_CopyCellTilesIndicesRect(sourceX, sourceY, destinationX, destinationY,
                                   width, height);
}

/// Changes the pictures of part of the room as MapGrid_CopyCellsTo does, the
/// rectangle's size before its destination, through
/// MapGrid_CopyCellTilesCellsRect, which keeps each cell's attribute bits.
static inline void MapGrid_CopyCellsLayered(s32 sourceX, s32 sourceY, s32 width,
                                            s32 height, s32 destinationX,
                                            s32 destinationY) {
  MapGrid_CopyCellTilesCellsRect(sourceX, sourceY, width, height, destinationX,
                                 destinationY);
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
  Actor_MoveToAndWaitById(actorId, x, z);
}

/// Walks an actor to a point and waits for it, then stands it still.
static inline void Actor_WalkToAndWait(s32 actorId, s32 x, s32 z) {
  Actor_WalkToAndWaitById(actorId, x, z);
}

/// Has two actors speak at once, each window placed by the event
/// (Event_ShowTwoMessagesAndWait): places in tiles, delay in frames.
static inline void FieldEvent_ShowTwoMessagesAndWait(
    s32 first, s32 firstX, s32 firstY, s32 firstPortraitX, s32 firstPortraitY,
    s32 second, s32 secondX, s32 secondY, s32 secondPortraitX,
    s32 secondPortraitY, s32 delay) {
  Event_ShowTwoMessagesAndWait(first, firstX, firstY, firstPortraitX,
                               firstPortraitY, second, secondX, secondY,
                               secondPortraitX, secondPortraitY, delay);
}

/// Gives an actor a movement type (enum MovementType) or an actor script to
/// run; the event goes on meanwhile.
static inline void Actor_SetScript(s32 actorId, const s32 *script) {
  Actor_SetMovementTypeById(actorId, script);
}

/// Runs a script, waiting at most 300 frames for its final command.
static inline void Actor_RunScriptAndWait(s32 actorId, const s32 *script) {
  Actor_SetMovementTypeAndWaitById(actorId, script);
}

#endif
