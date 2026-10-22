#ifndef FIELD_SERVICE_H
#define FIELD_SERVICE_H

/// @file
/// The scene service layer: one typed inline for each engine service a scene
/// calls by its own name, passing it on to the main image's import (Actor_Jump
/// calls Engine_ActorJump).
///
/// It is a habit of the whole game written once: a layer, not a
/// compiler-steering device (AGENTS.md S2). A scene writes
/// `Actor_Jump(ACTOR_VENUS_DJINNI, 4, 30);` and the inline makes the call.

#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "field_event.h"
#include "engine_call.h"
#include "farcall.h"

/// Turns two actors toward each other, as a conversation between them begins.
static inline void Actor_FaceEachOther(s32 actor, s32 other, s32 frames) {
  Engine_ActorFaceEachOther(actor, other, frames);
}

/// Makes an actor hop, height being its launch speed in pixels a frame, and
/// lets frames pass while it comes down.
static inline void Actor_Jump(s32 actor, s32 height, s32 frames) {
  Engine_ActorJump(actor, height, frames);
}

/// Makes an actor bob, squashing and springing back a few times, and waits for
/// it to finish.
static inline void Actor_RunRepeatedMotion(s32 actor, s32 repeats) {
  Engine_ActorRunRepeatedMotion(actor, repeats);
}

/// Switches an actor to an ActorAnimation, such as ANIM_NOD.
static inline void Actor_SetAnimation(s32 actor, s32 animation) {
  Engine_ActorSetAnimation(actor, animation);
}

/// Plays an actor's animation through before the scene goes on.
static inline void Actor_SetAnimationAndWait(s32 actor, s32 animation) {
  Engine_ActorSetAnimationAndWait(actor, animation);
}

/// Turns a one-sprite actor's shadow on or off, bit 0 of its sprite's flags.
static inline void Actor_SetSpriteFlags(struct FieldActor *actor, s32 flags) {
  Engine_ActorSetSpriteFlags(actor, flags);
}

/// Draws an actor at a fixed priority against the scenery instead of by its
/// depth.
static inline void Actor_SetSpritePriority(s32 actor, s32 priority) {
  Engine_ActorSetSpritePriority(actor, priority);
}

/// Starts an actor bobbing while the scene carries on.
static inline void Actor_StartRepeatedMotion(s32 actor, s32 repeats) {
  Engine_ActorStartRepeatedMotion(actor, repeats);
}

/// Stops whatever an actor is doing and hands it to the scene.
static inline void Actor_Stop(s32 actor) { Engine_ActorStop(actor); }

/// Holds the scene until an actor has reached where it was sent.
static inline void Actor_WaitForMove(s32 actor) {
  Engine_ActorWaitForMove(actor);
}

/// Holds the scene until the camera's pan is done.
static inline void Camera_WaitForMove(void) { Engine_CameraWaitForMove(); }

/// Runs a scene's colour fade, from the source colours to the target ones, over
/// frames.
static inline void ColorBuffer_Interpolate(s32 frames) {
  Engine_ColorBufferInterpolate(frames);
}

/// Starts a scripted scene, taking the field out of the player's hands.
static inline void Event_Begin(void) { Engine_EventBegin(); }

/// Lets the player answer yes or no to the line on screen; 0 means yes.
static inline s32 Event_ChooseYesNo(s32 actor, s32 flags) {
  return Engine_EventChooseYesNo(actor, flags);
}

/// Ends a scripted scene and gives the field back to the player.
static inline void Event_End(void) { Engine_EventEnd(); }

/// Returns the camera object, for a scene that moves the view itself.
static inline struct FieldActor *Event_GetViewCenter(void) {
  return Engine_EventGetViewCenter();
}

/// Starts the scene's screen transition, which Event_WaitForScreen waits out.
static inline void Event_OpenScreen(void) { Engine_EventOpenScreen(); }

/// Sends the party out of the room by one of its exits, as a door would.
static inline void Event_RequestExit(s32 exit) {
  Engine_EventRequestExit(exit);
}

/// Picks the line of dialogue the scene's next message shows.
static inline void Event_SetMessage(s32 message) {
  Engine_EventSetMessage(message);
}

/// Pauses the scene for frames, a pause debug fast-forward skips.
static inline void Event_Wait(s32 frames) { Engine_EventWait(frames); }

/// Holds the scene until its screen transition is over.
static inline void Event_WaitForScreen(void) { Engine_EventWaitForScreen(); }

/// Sends an object toward a point, where it coasts to a stop.
static inline void FieldObject_SetPosition(struct FieldActor *object,
                                           s32 fixed_x, s32 fixed_y,
                                           s32 fixed_z) {
  Engine_ObjectSetPosition(object, fixed_x, fixed_y, fixed_z);
}

/// Takes a work block for a scene from its heap slot, or returns the block the
/// slot already holds.
static inline void *Heap_Allocate(s32 slot, s32 size) {
  return Engine_HeapAllocate(slot, size);
}

/// Frees a scene's work block once the scene is done with it.
static inline void Heap_Release(s32 slot) { Engine_HeapRelease(slot); }

/// Draws an item's icon, ready for a scene to show it.
static inline void Item_LoadIcon(s32 item) { Engine_ItemLoadIcon(item); }

/// Holds an item's picture up over the leader, as when the party finds it.
static inline void Item_ShowFound(s32 item, s32 height) {
  Engine_ItemShowFound(item, height);
}

/// Redraws the room around the point the camera follows, after a scene has
/// moved it.
static inline void Map_Redraw(void) { Engine_MapRedraw(); }

/// The cosine of an angle, a full turn being 0x10000, in 16.16, for a scene's
/// circling motion.
static inline s32 Math_Cos(s32 angle) { return Engine_MathCos(angle); }

/// The sine of an angle in 16.16, as Math_Cos takes it.
static inline s32 Math_Sin(s32 angle) { return Engine_MathSin(angle); }

/// Shows a message in the middle of the screen rather than in a speech window.
static inline void Message_ShowCentered(s32 message, s32 flags) {
  Engine_MessageShowCentered(message, flags);
}

/// Gives the party an item from a scene and says who got it; -1 when it was
/// left behind.
static inline s32 Party_GiveItem(s32 item, s32 flags) {
  return Engine_PartyGiveItem(item, flags);
}

/// Ends a scene's wave effect and restores the colours it changed.
static inline void Psynergy_Cancel(void) { Engine_PsynergyCancel(); }

/// FieldScheduler_AddCallback (engine_call.h) under the scene layer's name.
static inline s32 Task_AddCallback(void (*callback)(void), s32 priority) {
  return FieldScheduler_AddCallback(callback, priority);
}

/// Lets frames go by even under debug fast-forward, where Event_Wait would skip
/// them.
static inline void Task_Wait(s32 frames) { Engine_TaskWait(frames); }

/// Loads a scene's sprite picture into VRAM; returns its first tile number, 0
/// when there's no room.
static inline s32 Vram_Load(s32 block, s32 size, const void *data) {
  return Engine_VramLoad(block, size, data);
}

/// The walk and wait of Actor_WalkByAndWait, through the import named after the
/// routine itself (ObjectMotion_CommitPositionAndActivate).
static inline void FieldObjectMotion_CommitPositionAndActivate(s32 a0, s32 a1,
                                                               s32 a2) {
  ObjectMotion_CommitPositionAndActivate(a0, a1, a2);
}

/// Actor_SetDestinationOffset under its main-image routine's name,
/// ObjectMotion_OffsetPositionAndResetMotion.
static inline void
FieldObjectMotion_OffsetPositionAndResetMotion(s32 a0, s32 a1, s32 a2) {
  Engine_ActorSetDestinationOffset(a0, a1, a2);
}

/// Actor_CenterAndWalk by the import that bears ObjectMotion_CenterOnColumn's
/// own name.
static inline void FieldObjectMotion_CenterOnColumn(s32 a0, s32 a1, s32 a2) {
  ObjectMotion_CenterOnColumn(a0, a1, a2);
}

/// Actor_SetDestination as ObjectMotion_ResetAndSetPosition, the name of the
/// routine behind it.
static inline void FieldObjectMotion_ResetAndSetPosition(s32 a0, s32 a1,
                                                         s32 a2) {
  Engine_ActorSetDestination(a0, a1, a2);
}

/// Actor_MoveToAndWait, written with ObjectMotion_SetPositionAndCommit's name.
static inline void FieldObject_MotionSetPositionAndCommit(s32 actor, s32 x,
                                                          s32 z) {
  Engine_ObjectMotionSetPositionAndCommit(actor, x, z);
}

/// FieldScheduler_AddCallback (engine_call.h) under the scheduler's own name.
static inline s32 FieldScheduler_AddOrUpdateCallback(SchedulerCallback callback,
                                                     s32 priority) {
  return FieldScheduler_AddCallback(callback, priority);
}

/// Changes the collision code of the cell at (x, y), in 16.16 pixels, opening
/// or closing the way (SetMapCellCollision).
static inline void Field_SetMapCellCollision(s32 a0, s32 a1, s32 a2, s32 a3) {
  SetMapCellCollision(a0, a1, a2, a3);
}

/// Changes one 512-pixel tile of the world map, drawn at once when it's next to
/// the view (Map_SetWindowCellTile).
static inline void FieldMap_SetWindowCellTile(s32 a0, s32 a1, s32 a2, s32 a3) {
  Map_SetWindowCellTile(a0, a1, a2, a3);
}

/// Puts an actor on the leader and sends it walking off by (dx, dz), then
/// starts the action facing names (Motion_LaunchFromFocusedObject).
static inline void FieldMotion_LaunchFromFocusedObject(s32 actor, s32 dx,
                                                       s32 dz, s32 facing) {
  Motion_LaunchFromFocusedObject(actor, dx, dz, facing);
}

/// Makes an actor follow another and starts a script on it
/// (Object_SetTargetAndCallback).
static inline void FieldObject_SetTargetAndCallback(s32 object, s32 target,
                                                    const s32 *actions) {
  Object_SetTargetAndCallback(object, target, actions);
}

/// Moves a point radius along a heading on the ground plane
/// (Vector_AddPolarOffset).
static inline void FieldVector_AddPolarOffset(s32 radius, s32 heading,
                                              struct FieldPosition *probe) {
  Vector_AddPolarOffset(radius, heading, probe);
}

#endif
