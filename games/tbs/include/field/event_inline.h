#ifndef ALCHEMY_EVENT_INLINE_H
#define ALCHEMY_EVENT_INLINE_H

/// @file
/// The event inlines: one typed inline for each event command an event calls
/// by its own name, passing it on to the main image's import
/// (Actor_FaceEachOther calls Actor_TurnToFaceEachOther).
///
/// An event writes `Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 30);`
/// and the inline makes the call. Where an inline would have its routine's
/// name, the routine keeps it and the inline takes the FieldActor_ prefix, as
/// FieldActor_JumpDownLedge does.

#include "field/field_sequence.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "system/system.h"
#include "event.h"
#include "system_call.h"
#include "far_call.h"

/// Turns two actors toward each other, as a conversation between them
/// begins.
static inline void Actor_FaceEachOther(s32 actorId, s32 other, s32 frames) {
  Actor_TurnToFaceEachOther(actorId, other, frames);
}

/// Holds the event until the camera's pan is done.
static inline void FieldCamera_WaitForMove(void) {
  FieldCamera_WaitForArrival();
}

/// Runs an event's palette fade, from the source colours to the target ones,
/// over frames.
static inline void PaletteFade_Interpolate(s32 frames) {
  PaletteFade_StartInterpolation(frames);
}

/// Draws an item's icon, ready for an event to show it.
static inline void Item_LoadIcon(s32 item) { Icon_DrawItem(item); }

/// Holds an item's picture up over the leader, as when the party finds it.
static inline void Item_ShowFound(s32 item, s32 height) {
  ItemDisplay_ShowItem(item, height);
}

/// Redraws the room around the point the camera follows, after an event has
/// moved it.
static inline void MapGrid_Redraw(void) { MapGrid_CenterOnFollowedPoint(); }

/// Shows a message in the middle of the screen rather than in a speech window.
static inline void Message_ShowCentered(s32 message, s32 flags) {
  FieldMessage_Show(message, flags);
}

/// Gives the party an item from an event and says who got it; -1 when it was
/// left behind.
static inline s32 Party_GiveItem(s32 item, s32 flags) {
  return PartyInventory_GiveItem(item, flags);
}

/// Ends an event's wave effect and restores the colours it changed.
static inline void Psynergy_Cancel(void) { FieldPsynergy_StopMindReadWave(); }

/// Loads a scene's sprite picture into VRAM; returns its first tile number, 0
/// when there's no room.
static inline s32 Vram_Load(s32 block, s32 size, const void *data) {
  return VramCache_Load(block, size, data);
}

/// Actor_SetDestinationOffset, by a second inline over the same routine.
static inline void FieldActor_SetDestinationOffset(s32 actorId, s32 xOffset,
                                                   s32 zOffset) {
  Actor_SetDestinationOffsetById(actorId, xOffset, zOffset);
}

/// Task_AddFrame (system_call.h) under a second name.
static inline s32 Task_AddOrUpdateFrame(TaskFunc func, s32 priority) {
  return Task_AddFrame(func, priority);
}

/// Changes the cell behavior of the cell at (x, y), in whole pixels, opening
/// or closing the way (MapGrid_SetCellBehaviorAt):
/// `Field_SetCellBehaviorAt(0, 544, 288, CELL_BEHAVIOR_CLIMB)`.
static inline void Field_SetCellBehaviorAt(s32 layer, s32 x, s32 y,
                                           s32 behavior) {
  MapGrid_SetCellBehaviorAt(layer, px(x), px(y), behavior);
}

/// Changes one 512-pixel tile of the World Map, drawn at once when it's next to
/// the view (WorldMap_SetWindowCellTile).
static inline void Field_SetWorldMapWindowCellTile(s32 x, s32 y, s32 cellX,
                                                   s32 cellY) {
  WorldMap_SetWindowCellTile(x, y, cellX, cellY);
}

/// Puts an actor on the leader and sends it walking off by (dx, dz),
/// then starts the movement type facing names (Actor_LaunchFromLeader).
static inline void FieldActor_LaunchFromLeader(s32 actorId, s32 dx, s32 dz,
                                               s32 facing) {
  Actor_LaunchFromLeader(actorId, dx, dz, facing);
}

/// Makes an actor follow another and starts a script on it
/// (Actor_WatchAndStartScript).
static inline void FieldActor_WatchAndStartScript(s32 object, s32 target,
                                                  const s32 *actions) {
  Actor_WatchAndStartScript(object, target, actions);
}

/// Moves a point radius, in 16.16 fixed point, along a heading on the ground
/// plane (Vector_AddPolarOffset).
static inline void
FieldVector_AddPolarOffsetFixed(s32 radius, s32 heading,
                                struct FieldPosition *probe) {
  Vector_AddPolarOffset(radius, heading, probe);
}

/// Moves a point a radius in whole pixels along a heading:
/// `FieldVector_AddPolarOffset(16, heading, &probe)`.
#define FieldVector_AddPolarOffset(radius, heading, probe)                     \
  FieldVector_AddPolarOffsetFixed(Q16(radius), heading, probe)

#endif
