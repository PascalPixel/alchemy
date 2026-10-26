#ifndef FIELD_SERVICE_H
#define FIELD_SERVICE_H

/// @file
/// The map service layer: one typed inline for each engine service a map
/// calls by its own name, passing it on to the main image's import
/// (Actor_Jump calls ObjectMotion_Launch).
///
/// It is a habit of the whole game written once: a layer, not a
/// compiler-steering device. A map writes
/// `Actor_Jump(ACTOR_VENUS_DJINNI, 4, 30);` and the inline makes the
/// call.

#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "script_layer.h"
#include "engine_call.h"
#include "farcall.h"

/// Turns two actors toward each other, as a conversation between them
/// begins.
static inline void Actor_FaceEachOther(s32 actorId, s32 other, s32 frames) {
  Actor_LinkPair(actorId, other, frames);
}

/// Makes an actor hop, height being its launch speed in pixels a frame,
/// and lets frames pass while it comes down.
static inline void Actor_Jump(s32 actorId, s32 height, s32 frames) {
  ObjectMotion_Launch(actorId, height, frames);
}

/// Makes an actor bob, squashing and springing back a few times, and
/// waits for it to finish.
static inline void Actor_RunRepeatedMotion(s32 actorId, s32 repeats) {
  ObjectMotion_SetVarCbAndRefresh(actorId, repeats);
}

/// Switches an actor to an ActorAnimation, such as ANIM_NOD.
static inline void Actor_SetAnimation(s32 actorId, s32 animation) {
  Import_ActorSetAnimation(actorId, animation);
}

/// Plays an actor's animation through before the script goes on.
static inline void Actor_SetAnimationAndWait(s32 actorId, s32 animation) {
  ObjectMotion_SetAnimationAndWait(actorId, animation);
}

/// Draws an actor at a fixed priority against the scenery instead of by
/// its depth.
static inline void Actor_SetSpritePriority(s32 actorId, s32 priority) {
  ObjectMotion_SetActionVariant(actorId, priority);
}

/// Starts an actor bobbing while the script carries on.
static inline void Actor_StartRepeatedMotion(s32 actorId, s32 repeats) {
  ObjectMotion_SetVariantCallback(actorId, repeats);
}

/// Stops whatever an actor is doing and hands it to the script.
static inline void Actor_Stop(s32 actorId) {
  ObjectMotion_EnableActionAndResetMotion(actorId);
}

/// Holds the script until an actor has reached where it was sent.
static inline void Actor_WaitForMove(s32 actorId) {
  ObjectMotion_CommitCurrentPositionAndActivate(actorId);
}

/// Holds the script until the camera's pan is done.
static inline void FieldCamera_WaitForMove(void) {
  FieldCamera_WaitForArrival();
}

/// Runs a script's palette fade, from the source colours to the target ones,
/// over frames.
static inline void PaletteFade_Interpolate(s32 frames) {
  PaletteFade_StartInterpolation(frames);
}

/// Starts an event, taking the field out of the player's hands.
static inline void Script_Start(void) { FieldScript_Start(); }

/// Lets the player answer yes or no to the line on screen; 0 means yes.
static inline s32 Script_ChooseYesNo(s32 actorId, s32 flags) {
  return FieldScript_AskYesNoWithActorAnimation(actorId, flags);
}

/// Ends an event and gives the field back to the player.
static inline void Script_End(void) { FieldScript_End(); }

/// Returns the camera object, for a script that moves the view itself.
static inline struct Actor *Script_GetViewCenter(void) {
  return FieldCamera_GetScriptObject();
}

/// Starts the script's screen transition, which Script_WaitForScreen waits out.
static inline void Script_OpenScreen(void) {
  FieldScript_StartScreenTransition();
}

/// Sends the party out of the room by one of its exits, as a door would.
static inline void Script_RequestExit(s32 exit) {
  Import_ScriptRequestExit(exit);
}

/// Picks the line of dialogue the script's next message shows.
static inline void Script_SetMessage(s32 message) {
  FieldScript_SetMessageId(message);
}

/// Pauses the script for frames, a pause debug fast-forward skips.
static inline void Script_Wait(s32 frames) { FieldScript_WaitFrames(frames); }

/// Holds the script until its screen transition is over.
static inline void Script_WaitForScreen(void) {
  FieldScript_WaitTransitionFrames();
}

/// Takes a work block for a map from its heap slot, or returns the block the
/// slot already holds.
static inline void *Heap_Allocate(s32 slot, s32 size) {
  return AllocHeapSlot(slot, size);
}

/// Frees a map's work block once the map is done with it.
static inline void Heap_Release(s32 slot) { FreeHeapSlot(slot); }

/// Draws an item's icon, ready for a script to show it.
static inline void Item_LoadIcon(s32 item) { Icon_DrawItem(item); }

/// Holds an item's picture up over the leader, as when the party finds it.
static inline void Item_ShowFound(s32 item, s32 height) {
  ItemDisplay_ShowItem(item, height);
}

/// Redraws the room around the point the camera follows, after a script has
/// moved it.
static inline void MapGrid_Redraw(void) { MapGrid_CenterOnFollowedPoint(); }

/// The cosine of an angle, a full turn being 0x10000, in 16.16, for a script's
/// circling motion.
static inline s32 Math_Cos(s32 angle) { return Trig_Cos(angle); }

/// The sine of an angle in 16.16, as Math_Cos takes it.
static inline s32 Math_Sin(s32 angle) { return Trig_Sin(angle); }

/// Shows a message in the middle of the screen rather than in a speech window.
static inline void Message_ShowCentered(s32 message, s32 flags) {
  FieldMessage_Show(message, flags);
}

/// Gives the party an item from a script and says who got it; -1 when it was
/// left behind.
static inline s32 Party_GiveItem(s32 item, s32 flags) {
  return PartyInventory_GiveItem(item, flags);
}

/// Ends a script's wave effect and restores the colours it changed.
static inline void Psynergy_Cancel(void) {
  FieldEffect_FinishWaveAndReleaseHeapBlock();
}

/// FieldScheduler_AddCallback (engine_call.h) under the map service layer's
/// name.
static inline s32 Task_AddCallback(void (*callback)(void), s32 priority) {
  return FieldScheduler_AddCallback(callback, priority);
}

/// Lets frames go by even under debug fast-forward, where Script_Wait would
/// skip them.
static inline void Task_Wait(s32 frames) { WaitFrames(frames); }

/// Loads a map's sprite picture into VRAM; returns its first tile number, 0
/// when there's no room.
static inline s32 Vram_Load(s32 block, s32 size, const void *data) {
  return VramCache_Load(block, size, data);
}

/// The walk and wait of Actor_WalkByAndWait, by a second inline over the
/// same routine.
static inline void FieldObjectMotion_CommitPositionAndActivate(s32 actorId,
                                                               s32 xOffset,
                                                               s32 zOffset) {
  ObjectMotion_CommitPositionAndActivate(actorId, xOffset, zOffset);
}

/// Actor_SetDestinationOffset, by a second inline over the same routine.
static inline void FieldObjectMotion_OffsetPositionAndResetMotion(s32 actorId,
                                                                  s32 xOffset,
                                                                  s32 zOffset) {
  ObjectMotion_OffsetPositionAndResetMotion(actorId, xOffset, zOffset);
}

/// Actor_CenterAndWalk, by a second inline over the same routine.
static inline void FieldObjectMotion_CenterOnColumn(s32 actorId, s32 priority,
                                                    s32 zOffset) {
  ObjectMotion_CenterOnColumn(actorId, priority, zOffset);
}

/// Actor_SetDestination as ObjectMotion_ResetAndSetPosition, the name of
/// the routine behind it.
static inline void FieldObjectMotion_ResetAndSetPosition(s32 actorId, s32 x,
                                                         s32 z) {
  ObjectMotion_ResetAndSetPosition(actorId, x, z);
}

/// Actor_MoveToAndWait, written with ObjectMotion_SetPositionAndCommit's
/// name.
static inline void Object_MotionSetPositionAndCommit(s32 actorId, s32 x,
                                                     s32 z) {
  ObjectMotion_SetPositionAndCommit(actorId, x, z);
}

/// FieldScheduler_AddCallback (engine_call.h) under a second name.
static inline s32 FieldScheduler_AddOrUpdateCallback(TaskFunc callback,
                                                     s32 priority) {
  return FieldScheduler_AddCallback(callback, priority);
}

/// Changes the metatile behavior of the cell at (x, y), in 16.16 pixels,
/// opening or closing the way (MapGrid_SetMetatileBehaviorAt).
static inline void Field_SetMetatileBehaviorAt(s32 layer, s32 x, s32 y,
                                               s32 behavior) {
  MapGrid_SetMetatileBehaviorAt(layer, x, y, behavior);
}

/// Changes one 512-pixel tile of the world map, drawn at once when it's next to
/// the view (WorldMap_SetWindowCellTile).
static inline void Field_SetWorldMapWindowCellTile(s32 x, s32 y, s32 px,
                                                   s32 py) {
  WorldMap_SetWindowCellTile(x, y, px, py);
}

/// Puts an actor on the leader and sends it walking off by (dx, dz),
/// then starts the action facing names (ObjectMotion_LaunchFromFocusedObject).
static inline void FieldMotion_LaunchFromFocusedObject(s32 actorId, s32 dx,
                                                       s32 dz, s32 facing) {
  ObjectMotion_LaunchFromFocusedObject(actorId, dx, dz, facing);
}

/// Makes an actor follow another and starts a script on it
/// (Actor_SetTargetAndCallback).
static inline void Object_SetTargetAndCallback(s32 object, s32 target,
                                               const s32 *actions) {
  Actor_SetTargetAndCallback(object, target, actions);
}

/// Moves a point radius along a heading on the ground plane
/// (Vector_AddPolarOffset).
static inline void FieldVector_AddPolarOffset(s32 radius, s32 heading,
                                              struct FieldPosition *probe) {
  Vector_AddPolarOffset(radius, heading, probe);
}

#endif
