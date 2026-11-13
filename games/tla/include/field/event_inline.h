#ifndef ALCHEMY_EVENT_INLINE_H
#define ALCHEMY_EVENT_INLINE_H

/// @file
/// The event inlines: one typed inline for each event command an event calls
/// by its own name, passing it on to the main image's import
/// (Actor_FaceEachOther calls Actor_TurnToFaceEachOther).
///
/// It is a habit of the whole game written once: a layer, not a
/// compiler-steering device. An event writes
/// `Actor_FaceEachOther(4, 8, 30);` and the inline makes the
/// call. Where an inline would have its routine's name, the routine keeps it
/// and the inline takes the Field prefix, as FieldActor_Jump does, or Event in
/// place of a Field the routine's name already has (EventCamera_GetActor).

#include "event.h"
#include "system_call.h"

/// Turns two actors toward each other, as a conversation between them
/// begins.
static inline void Actor_FaceEachOther(s32 actorId, s32 other, s32 frames) {
  Actor_TurnToFaceEachOther(actorId, other, frames);
}

/// Switches an actor to an animation of its sprite.
static inline void Actor_SetAnimation(s32 actorId, s32 animation) {
  Actor_SetModeById(actorId, animation);
}

/// Actor_SetAnimation as the events that take Actor_SetModeById for a routine
/// returning int make the call.
static inline void FieldActor_SetAnimation(s32 actorId, s32 animation) {
#ifdef UBFIX
  Actor_SetModeById(actorId, animation);
#else
  // UB: Actor_SetModeById returns nothing, but these calls are made as to a
  // routine returning int, which moves their argument registers.
  ((s32 (*)(u32, s32))Actor_SetModeById)(actorId, animation);
#endif
}

/// Runs an event's palette fade, from the source colours to the target ones,
/// over frames.
static inline void PaletteFade_Interpolate(s32 frames) {
  PaletteFade_StartInterpolation(frames);
}

/// Draws an item's icon, ready for an event to show it.
static inline void Item_LoadIcon(s32 item) { Icon_DrawItem(item); }

/// The sine of an angle, a full turn being TRIG_TURN, in 16.16.
static inline s32 Math_Sin(s32 angle) { return Trig_Sin(angle); }

/// Shows a message in the middle of the screen rather than in a speech window.
static inline void Message_ShowCentered(s32 message, s32 flags) {
  FieldMessage_Show(message, flags);
}

/// Gives the party an item from an event and says who got it; -1 when it was
/// left behind.
static inline s32 Party_GiveItem(s32 item, s32 flags) {
  return PartyInventory_GiveItem(item, flags);
}

/// Loads a scene's sprite picture into VRAM; returns its first tile number, 0
/// when there's no room.
static inline s32 Vram_Load(s32 block, s32 size, const void *data) {
  return VramCache_Load(block, size, data);
}

/// Puts an actor on the leader and sends it walking off by (dx, dz),
/// then starts the movement type facing names (Actor_LaunchFromLeader).
static inline void FieldActor_LaunchFromLeader(s32 actorId, s32 dx, s32 dz,
                                               s32 facing) {
  Actor_LaunchFromLeader(actorId, dx, dz, facing);
}

/// Moves a point radius along a heading on the ground plane
/// (Vector_AddPolarOffset).
static inline void FieldVector_AddPolarOffset(s32 radius, s32 heading,
                                              s32 *probe) {
  Vector_AddPolarOffset(radius, heading, probe);
}

/// Changes the cell behavior of the cell at (x, y), in 16.16 pixels,
/// opening or closing the way (MapGrid_SetCellBehaviorAt).
static inline void Field_SetCellBehaviorAt(s32 layer, s32 x, s32 y,
                                           s32 behavior) {
  MapGrid_SetCellBehaviorAt(layer, x, y, behavior);
}

/// Changes one 512-pixel tile of the World Map, drawn at once when it's next to
/// the view (WorldMap_SetWindowCellTile).
static inline void Field_SetWorldMapWindowCellTile(s32 x, s32 y, s32 px,
                                                   s32 py) {
  WorldMap_SetWindowCellTile(x, y, px, py);
}

/// Holds the event for frames while the field goes on (Event_Wait).
static inline void FieldEvent_Wait(s32 frames) { Event_Wait(frames); }

/// Turns an actor to a direction and holds the event until it has turned.
static inline void Actor_FaceDirectionAndWait(s32 actorId, s32 facing) {
  Actor_FaceDirectionById(actorId, facing);
}

/// Places an actor at a point at once, as Actor_SetPosition does, facing a
/// direction.
static inline void Actor_SetPositionAndFacing(s32 actorId, s32 fixed_x,
                                              s32 fixed_z, s32 facing) {
  Actor_SetHorizontalPositionAndAngleWithTerrain(actorId, fixed_x, fixed_z,
                                                 facing);
}

/// Switches an actor to an animation and holds the event until it has played
/// (Motion_SetModeAndWaitAnimation).
static inline void Actor_SetAnimationAndWait(s32 actorId, s32 animation) {
  Motion_SetModeAndWaitAnimation(actorId, animation);
}

/// Actor_SetAnimationAndWait as the events that take
/// Motion_SetModeAndWaitAnimation for a routine returning int make the call.
static inline void FieldActor_SetAnimationAndWait(s32 actorId, s32 animation) {
#ifdef UBFIX
  Motion_SetModeAndWaitAnimation(actorId, animation);
#else
  // UB: Motion_SetModeAndWaitAnimation returns nothing, but these calls are
  // made as to a routine returning int, which moves their argument registers.
  ((s32 (*)(u32, s32))Motion_SetModeAndWaitAnimation)(actorId, animation);
#endif
}

/// Actor_Jump, from an event.
static inline void FieldActor_Jump(s32 actorId, s32 speed, s32 frames) {
  Actor_Jump(actorId, speed, frames);
}

/// Runs an actor's repeated motion count times and holds the event until it
/// ends (Motion_SetVarCbAndRefresh).
static inline void Actor_RunRepeatedMotion(s32 actorId, s32 count) {
  Motion_SetVarCbAndRefresh(actorId, count);
}

/// Actor_SetSpritePriority, from an event.
static inline void FieldActor_SetSpritePriority(s32 actorId, s32 priority) {
  Actor_SetSpritePriority(actorId, priority);
}

/// Actor_StartRepeatedMotion, from an event.
static inline void FieldActor_StartRepeatedMotion(s32 actorId, s32 count) {
  Actor_StartRepeatedMotion(actorId, count);
}

/// Actor_WatchAndSetScript, from an event.
static inline void FieldActor_WatchAndSetScript(s32 actorId,
                                                s32 watchedActorId) {
  Actor_WatchAndSetScript(actorId, watchedActorId);
}

/// Actor_WaitForMoveById, from an event.
static inline void FieldActor_WaitForMoveById(s32 actorId) {
  Actor_WaitForMoveById(actorId);
}

/// Event_ResolvePendingActions, from an event.
static inline void FieldEvent_ResolvePendingActions(s32 skip) {
  Event_ResolvePendingActions(skip);
}

/// Event_SetMessageId, from an event.
static inline void FieldEvent_SetMessageId(s32 message) {
  Event_SetMessageId(message);
}

/// Makes the camera follow an actor (FieldCamera_FollowActor).
static inline void EventCamera_FollowActor(s32 actorId, s32 keepPosition) {
  FieldCamera_FollowActor(actorId, keepPosition);
}

/// Actor_SetPositionAndResetMotion, from an event.
static inline void FieldActor_SetPositionAndResetMotion(struct Actor *actor,
                                                        s32 fixed_x,
                                                        s32 fixed_y,
                                                        s32 fixed_z) {
  Actor_SetPositionAndResetMotion(actor, fixed_x, fixed_y, fixed_z);
}

/// Actor_RefreshSelectorById, from an event.
static inline void FieldActor_RefreshSelectorById(s32 actorId) {
  Actor_RefreshSelectorById(actorId);
}

/// Sets a sprite param of an actor's sprite (Actor_SetSpriteColor).
static inline void FieldActor_SetSpriteColor(struct Actor *actor, s32 param) {
  Actor_SetSpriteColor(actor, param);
}

/// Actor_Stop, from an event.
static inline void FieldActor_Stop(s32 actorId) { Actor_Stop(actorId); }

/// Event_SetExitRequest, from an event.
static inline void FieldEvent_SetExitRequest(s32 request) {
  Import_EventSetExitRequest(request);
}

/// Holds the event until an actor has reached its destination
/// (Actor_WaitForMove).
static inline void FieldActor_WaitForMove(void *actor) {
  Actor_WaitForMove(actor);
}

/// PartyInventory_FindMember, from an event.
static inline s32 FieldPartyInventory_FindMember(s32 item) {
  return PartyInventory_FindMember(item);
}

/// Shows a message in a window in the middle of the screen
/// (FieldMessage_ShowCentered).
static inline void EventMessage_ShowCentered(s32 message, s32 mode,
                                             s32 yOffset) {
  FieldMessage_ShowCentered(message, mode, yOffset);
}

/// Party_GetLeaderActor, from an event.
static inline s32 FieldParty_GetLeaderActor(void) {
  return Party_GetLeaderActor();
}

/// Event_AskYesNoWithActorAnimation, from an event.
static inline s32 FieldEvent_AskYesNoWithActorAnimation(s32 actorId,
                                                        s32 flags) {
  return Event_AskYesNoWithActorAnimation(actorId, flags);
}

/// Sets where the party goes after the event's battle
/// (Event_SetAfterBattleDestination).
static inline void FieldEvent_SetAfterBattleDestination(s32 sceneId,
                                                        s32 entrance) {
  Event_SetAfterBattleDestination(sceneId, entrance);
}

/// Sets where the party goes if it loses the event's battle
/// (Party_SetFields1f2And1f4).
static inline void Event_SetDefeatDestination(s32 sceneId, s32 entrance) {
  Party_SetFields1f2And1f4(sceneId, entrance);
}

/// Adds a Djinni to the party (Djinn_JoinParty).
static inline void FieldDjinn_JoinParty(s32 slot, s32 element, s32 djinn) {
  Djinn_JoinParty(slot, element, djinn);
}

/// The actor the camera follows (FieldCamera_GetActor).
static inline struct Actor *EventCamera_GetActor(void) {
  return FieldCamera_GetActor();
}

/// Asks the player yes or no (FieldYesNo_Ask).
static inline s32 EventYesNo_Ask(s32 atBottom) {
  return FieldYesNo_Ask(atBottom);
}

/// Schedules a zoom of the field (WorldMap_StartZoom).
static inline void FieldEffect_ScheduleRatioTransition(s32 zoom, s32 frames) {
  WorldMap_StartZoom(zoom, frames);
}

/// Applies a window colour (WindowColor_Apply).
static inline void FieldWindowColor_Apply(s32 phase, s32 brightness) {
  WindowColor_Apply(phase, brightness);
}

/// Requests a battle with a Djinni (Field_RequestDjinnBattle).
static inline void Event_RequestDjinnBattle(s32 flag, s32 no) {
  Field_RequestDjinnBattle(flag, no);
}

/// Event_PrepareObjectAndApplyValue, from an event.
static inline void Event_PrepareActorAndApplyValue(s32 unit, s32 announce) {
  Event_PrepareObjectAndApplyValue(unit, announce);
}

/// Holds the event until the screen shake dies down
/// (MapGrid_WaitForShakeToSettle).
static inline void FieldMapGrid_WaitForShakeToSettle(void) {
  MapGrid_WaitForShakeToSettle();
}

/// The cell behavior of the cell at (x, y) (MapGrid_GetCellBehaviorAt).
static inline s32 FieldMapGrid_GetCellBehaviorAt(s32 layer, s32 x, s32 y) {
  return MapGrid_GetCellBehaviorAt(layer, x, y);
}

/// Sets how fast an actor's animation runs (Actor_SetActionById).
static inline void Actor_SetAnimationSpeedById(s32 actorId, s32 steps) {
  Actor_SetActionById(actorId, steps);
}

/// Draws an item's icon into an actor's sprite, or blanks it
/// (Actor_ShowItemIconById).
static inline void FieldActor_ShowItemIconById(s32 actorId, s32 item) {
  Actor_ShowItemIconById(actorId, item);
}

/// Draws an item's icon into an actor's sprite (Actor_SetItemIcon).
static inline void FieldActor_SetItemIcon(struct Actor *actor, s32 item) {
  Actor_SetItemIcon(actor, item);
}

/// Changes one tile of the World Map and draws it
/// (WorldMap_SetWindowCellTileAndDraw).
static inline void FieldWorldMap_SetWindowCellTileAndDraw(s32 x, s32 y, s32 px,
                                                          s32 py) {
  WorldMap_SetWindowCellTileAndDraw(x, y, px, py);
}

/// Moves the camera onto an actor, keeping its height
/// (FieldCamera_MoveToActor).
static inline s32 EventCamera_MoveToActor(s32 actorId, s32 pan) {
  return FieldCamera_MoveToActor(actorId, pan);
}

/// Rotates an actor's sprite (Actor_SetSpriteRotationById).
static inline void FieldActor_SetSpriteRotationById(s32 actorId, s32 angle) {
  Actor_SetSpriteRotationById(actorId, angle);
}

/// Sets the cell behavior of a cell (MapGrid_SetCellBehavior).
static inline s32 FieldMapGrid_SetCellBehavior(s32 x, s32 z, s32 layer,
                                               s32 behavior) {
  return MapGrid_SetCellBehavior(x, z, layer, behavior);
}

#endif
