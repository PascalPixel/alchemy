#ifndef ALCHEMY_EVENT_H
#define ALCHEMY_EVENT_H

/// @file
/// The calls events are written in: the small inlines a scene's events share,
/// over the main-image routines they call by those routines' own names
/// (Actor_ArmCallback, Event_ShowMessageAndWait, Actor_SetSpeedParameters and
/// the rest), whose declarations it holds. An overlay's import stub takes the
/// name of the routine it reaches, except the few the overlays share under
/// another (Math_Sine, Import_ActorCreate, Import_EventSetExitRequest).
///
/// It is a habit of the whole game written once, as declarations and inlines,
/// not a compiler-steering device. An inline's parameters are its own, so where
/// an event calls a routine again with the same constant, the constant is built
/// again for each call rather than kept in a register, as the ROM shows: in the
/// scene overlays 482 such calls build their constant again and none shares
/// one (in The Broken Seal, 301 and 11). event_inline.h's inlines are written
/// over it. An event reads `Actor_FaceDirection(4, 0xc000, 0);
/// FieldEvent_Wait(20);`.

#include "types.h"
#include "task.h"

struct Actor;
struct Actor;

void Event_ShowMessageAndWait(s32 actor, s32 flags);
void Event_ResolvePendingActions(s32 skip);
s32 Party_GetLeaderActor(void);
void Event_Wait(s32 frames);
void Event_Start(void);
void Event_SetDestination(s32 scene, s32 entrance);
void Event_SetExitRequest(u16 countdown);
void Event_SetMessageId(s32 message);
s32 FieldMessage_ShowAtActor(s32 speaker, s32 flags);
s32 Event_AskYesNoAndShowResponse(s32 speaker, s32 flags);
struct Actor *Actor_GetById(u32 objectId);
void Actor_SetHorizontalPositionWithTerrain(s32 actorId, s32 x, s32 z);
void Actor_SetSpeedParameters(s32 actorId, s32 speed, s32 acceleration);
void Actor_ResetAndSetPosition(u32 objectId, s32 x, s32 z);
void Actor_SetDestinationOffsetById(s32 actorId, s32 dx, s32 dz);
void Actor_WalkToById(s32 actorId, s32 x, s32 z);
void Actor_OffsetPositionAndReset(u32 objectId, s32 x_offset, s32 z_offset);
void Actor_ArmCallback(s32 actorId, s32 angle, s32 frames);
void Actor_FaceActorById(s32 actorId, s32 target, s32 frames);
s32 Actor_ShowEmoteById(s32 actorId, s32 emote, s32 frames);
void FieldEffect_SetAttached(s32 actorId, s32 effect);
void Camera_SetSpeedParameters(s32 speed, s32 acceleration);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 pan);
void Map_CopyCellsRect(s32 srcX, s32 srcY, s32 width, s32 height, s32 dst_x,
                       s32 dst_y);
void Map_CopyCellAttributeRect(s32 srcX, s32 srcY, s32 width, s32 height,
                               s32 dst_x, s32 dst_y);
void Map_SetScreenShake(s32 horizontal, s32 vertical, s32 decay);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Sound_Play(s32 sound);
void Actor_SetMoveTarget(struct Actor *actor, s32 x, s32 y, s32 z);
struct Actor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
void Map_PlayMapEdit(const void *steps, s32 x, s32 y);
void ObjectEvent_PlaceOnGround(s32 object, s32 x, s32 z);
void Actor_CenterOnColumn(u32 objectId, s32 action, s32 z_offset);
void Actor_WalkByAndWaitById(u32 objectId, s32 x_offset, s32 z_offset);
void Event_ShowMessageThenWait(s32 speaker, s32 flags, s32 frames);
void Actor_ConfigureSpriteColor(s32 actorId, s32 value);
void Map_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, s32 dst_x, s32 dst_y,
                                    s32 width, s32 height);
void MapGrid_CopyCellPictureCellsRect(s32 sourceX, s32 sourceY,
                                      s32 destinationX, s32 destinationY,
                                      s32 width, s32 height);
void BattleEffect_ApplyColorToSourceBuffer(s32 value, s32 mode);
void BattleEffect_ApplyColorToTargetBuffer(s32 value, s32 mode);
void Actor_MoveToAndWaitById(u32 objectId, s32 x, s32 z);
void Actor_WalkToAndWaitById(s32 actorId, s32 x, s32 z);
void Battle_ShowPairedUnitWorkAndWait(s32 speaker, s32 x, s32 y, s32 arg,
                                      s32 extra, s32 otherSpeaker, s32 otherX,
                                      s32 otherY, s32 otherArg, s32 otherExtra,
                                      s32 flags);
void Actor_SetMovementTypeById(s32 actorId, const void *script);
void Actor_SetMovementTypeAndWaitById(s32 actorId, const void *script);
void Actor_TurnToFaceEachOther(s32 actorId, s32 other, s32 frames);
void Actor_SetModeById(u32 actorId, s32 animation);
void PaletteFade_StartInterpolation(s32 frames);
void Icon_DrawItem(s32 item);
s32 Math_Sine(s32 angle);
void FieldMessage_Show(s32 message, s32 flags);
s32 PartyInventory_GiveItem(s32 item, s32 flags);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
void Actor_LaunchFromLeader(s32 actorId, s32 dx, s32 dz, s32 facing);
void Vector_AddPolarOffsetFar(s32 radius, s32 heading, s32 *probe);
void SetMapCellCollision(s32 layer, s32 x, s32 y, s32 behavior);
void Map_SetWindowCellTile(s32 x, s32 y, s32 px, s32 py);
void Message_DecodeRawUnbounded(s32 message, u16 *buffer);
void MapGrid_SetCameraBounds(s32 minX, s32 minY, s32 maxX, s32 maxY);
s32 GameFlag_GetByte(s32 flag);
s32 Party_ListMembers(s16 *owners);
s32 Unit_AddHp(s32 owner, s32 delta);
void wait(s32 frames);
void Actor_ArmCallbackAndRefresh(s32 actorId, s32 angle);
void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 actorId, s32 x, s32 z,
                                                    s32 angle);
void Motion_SetModeAndWaitAnimation(s32 actorId, s32 animation);
void Actor_Jump(s32 actorId, s32 speed, s32 frames);
void Motion_SetVarCbAndRefresh(u32 objectId, s32 variant);
void Actor_SetSpritePriority(u32 actorId, s32 priority);
void Actor_StartRepeatedMotion(u32 objectId, s32 variant);
void Actor_WatchAndSetScript(s32 actorId, s32 watchedActorId);
void Actor_WaitForMoveById(s32 actorId);
void Camera_FollowActor(s32 id, s32 flag);
void Actor_SetPositionAndResetMotion(struct Actor *object, s32 x, s32 y, s32 z);
void Actor_RefreshSelectorById(u32 actorId);
void Actor_SetPartAttribute(struct Actor *object, s32 attribute);
void Actor_Stop(s32 actorId);
void Import_EventSetExitRequest(s32 countdown);
void Script_WaitForEventTimeout(struct Actor *actor);
s32 PartyInventory_FindMember(s32 item_id);
void FieldMessage_ShowCentered(s32 message, s32 mode, s32 yOffset);
s32 Event_AskYesNoWithActorAnimation(s32 actorId, s32 flags);
void Party_SetFields1eeAnd1f0(s32 sceneId, s32 entrance);
void Party_SetFields1f2And1f4(s32 sceneId, s32 entrance);
void Task_RemoveFar(TaskFunc func);
void Djinn_JoinParty(s32 slot, s32 element, s32 djinn);
void FieldPsynergy_StartMindReadWave(s32 x, s32 y);
struct Actor *FieldCamera_GetActor(void);
s32 FieldYesNo_Ask(s32 atBottom);
void WorldMap_StartZoom(s32 zoom, s32 frames);
void WindowColor_Apply(s32 phase, s32 brightness);
void Field_RequestDjinnBattle(s32 flag, s32 no);
void Event_PrepareObjectAndApplyValue(s32 unit, s32 announce);
void MapGrid_WaitForShakeToSettle(void);
s32 GetMapCellCollision(s32 layer, s32 x, s32 y);
void Actor_SetActionById(s32 actorId, s32 steps);
void Actor_ShowItemIconById(s32 actorId, s32 item);
void Actor_SetItemIcon(struct Actor *actor, s32 item);
void WorldMap_SetWindowCellTileAndDraw(s32 x, s32 y, s32 px, s32 py);
s32 FieldCamera_MoveToActor(s32 actorId, s32 pan);
void Actor_SetSpriteRotationById(s32 actorId, s32 angle);
s32 MapGrid_SetCellBehavior(s32 x, s32 z, s32 layer, s32 behavior);

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

/// Places an actor at a point at once, without walking it there.
static inline void Actor_SetPosition(s32 actorId, s32 fixed_x, s32 fixed_z) {
  Actor_SetHorizontalPositionWithTerrain(actorId, fixed_x, fixed_z);
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
  Actor_OffsetPositionAndReset(actorId, dx, dz);
}

/// Turns an actor to a direction and holds the event for a few frames.
static inline void Actor_FaceDirection(s32 actorId, s32 facing, s32 frames) {
  Actor_ArmCallback(actorId, facing, frames);
}

/// Has one actor look at another at once, then pauses the event.
static inline void Actor_FaceActor(s32 actorId, s32 target, s32 frames) {
  Actor_FaceActorById(actorId, target, frames);
}

/// Pops an emote bubble up over an actor, then lets frames pass before
/// the event goes on.
static inline s32 Actor_ShowEmote(s32 actorId, s32 emote, s32 frames) {
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

/// Sets how fast the camera pans for the event's next camera moves.
static inline void FieldCamera_SetSpeed(s32 speed, s32 acceleration) {
  Camera_SetSpeedParameters(speed, acceleration);
}

/// Pans the camera to a point in the room, or cuts there when pan is 0.
static inline void FieldCamera_MoveTo(s32 fixed_x, s32 fixed_y, s32 fixed_z,
                                      s32 pan) {
  Camera_MoveWithinMapBounds(fixed_x, fixed_y, fixed_z, pan);
}

/// Changes part of the room in place, copying whole cells, picture and
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

/// Sets shake amplitudes in fixed-point pixels and the per-frame decay factor.
/// A negative argument keeps that component unchanged; Q16_ONE keeps the shake.
static inline void MapGrid_SetShake(s32 horizontal, s32 vertical, s32 decay) {
  Map_SetScreenShake(horizontal, vertical, decay);
}

/// GameFlag_Test, from an event: 1 when the flag is set.
static inline s32 Event_GetFlag(s32 flag) { return GameFlag_Test(flag); }

/// GameFlag_SetBit, from an event.
static inline void Event_SetFlag(s32 flag) { GameFlag_SetBit(flag); }

/// GameFlag_ClearBit, from an event.
static inline void Event_ClearFlag(s32 flag) { GameFlag_ClearBit(flag); }

/// Plays a sound effect or music from an event: the SE command of script.h.
static inline void Event_PlaySound(s32 sound) { Sound_Play(sound); }

/// Signed division written as a call; the `/` compiles to __divsi3.
static inline s32 Math_Divide(s32 dividend, s32 divisor) {
  return dividend / divisor;
}

/// Sends an actor toward a point, where it coasts to a stop.
static inline void FieldActor_SetMoveTarget(struct Actor *actor, s32 fixed_x,
                                            s32 fixed_y, s32 fixed_z) {
  Actor_SetMoveTarget(actor, fixed_x, fixed_y, fixed_z);
}

/// Creates an actor in an empty slot of gActors, outside the actor ids, or
/// returns NULL when every slot is taken.
static inline struct Actor *FieldActor_Create(s32 type, s32 fixed_x,
                                              s32 fixed_y, s32 fixed_z) {
  return (struct Actor *)Import_ActorCreate(type, fixed_x, fixed_y, fixed_z);
}

/// Plays a cell animation, such as a door opening, and holds the event until
/// its last step.
static inline void MapGrid_AnimateCells(const u16 *steps, s32 destinationX,
                                        s32 destinationY) {
  Map_PlayMapEdit(steps, destinationX, destinationY);
}

/// Moves a treasure spot's actor to a point on the ground.
static inline void TreasureSpot_SetPosition(s32 object, s32 fixed_x,
                                            s32 fixed_z) {
  ObjectEvent_PlaceOnGround(object, fixed_x, fixed_z);
}

/// Lines an actor up with the middle of its column, then walks it
/// straight along z.
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

/// Sets a sprite param on every part of an actor's sprite, or makes it
/// blink.
static inline void Actor_SetSpriteColorById(s32 actorId, s32 value) {
  Actor_ConfigureSpriteColor(actorId, value);
}

/// Changes what part of the room shows, copying the cells' pictures but not
/// their cell behaviors.
static inline void MapGrid_CopyCellsTo(s32 sourceX, s32 sourceY,
                                       s32 destinationX, s32 destinationY,
                                       s32 width, s32 height) {
  Map_CopyCellPictureIndicesRect(sourceX, sourceY, destinationX, destinationY,
                                 width, height);
}

/// Changes the pictures of part of the room as MapGrid_CopyCellsTo does,
/// through MapGrid_CopyCellPictureCellsRect, which keeps each cell's attribute
/// bits.
static inline void MapGrid_CopyCellsLayered(s32 sourceX, s32 sourceY,
                                            s32 destinationX, s32 destinationY,
                                            s32 width, s32 height) {
  MapGrid_CopyCellPictureCellsRect(sourceX, sourceY, destinationX, destinationY,
                                   width, height);
}

/// Sets the colours the screen shows now, where an event's palette fade starts.
static inline void PaletteFade_ApplySource(s32 value, s32 mode) {
  BattleEffect_ApplyColorToSourceBuffer(value, mode);
}

/// Sets the colours an event's palette fade ends on.
static inline void PaletteFade_ApplyTarget(s32 value, s32 mode) {
  BattleEffect_ApplyColorToTargetBuffer(value, mode);
}

/// Moves an actor to a point in the pose it has and waits for it.
static inline void Actor_MoveToAndWait(s32 actorId, s32 x, s32 z) {
  Actor_MoveToAndWaitById(actorId, x, z);
}

/// Walks an actor to a point and waits for it, then stands it still.
static inline void Actor_WalkToAndWait(s32 actorId, s32 x, s32 z) {
  Actor_WalkToAndWaitById(actorId, x, z);
}

/// Has two actors speak at once, each window placed by the event.
static inline void
FieldEvent_ShowTwoMessagesAndWait(s32 speaker, s32 x, s32 y, s32 arg, s32 extra,
                                  s32 otherSpeaker, s32 otherX, s32 otherY,
                                  s32 otherArg, s32 otherExtra, s32 flags) {
  Battle_ShowPairedUnitWorkAndWait(speaker, x, y, arg, extra, otherSpeaker,
                                   otherX, otherY, otherArg, otherExtra, flags);
}

/// The field encodes either a movement type or an actor-script address.
static inline void Actor_SetScript(s32 actorId, const s32 *script) {
  Actor_SetMovementTypeById(actorId, script);
}

/// Runs a script, waiting at most 300 frames for its final command.
static inline void Actor_RunScriptAndWait(s32 actorId, const s32 *script) {
  Actor_SetMovementTypeAndWaitById(actorId, script);
}

#endif
