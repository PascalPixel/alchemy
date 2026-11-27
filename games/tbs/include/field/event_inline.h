#ifndef ALCHEMY_EVENT_INLINE_H
#define ALCHEMY_EVENT_INLINE_H

/* @file */
/* The event inlines: one typed inline for each event command an event calls */
/* by its own name, passing it on to the main image's import */
/* (fdActorFaceEachOther calls fdActorTurnToFaceEachOther). */
/* */
/* An event writes `fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 30);`
 */
/* and the inline makes the call. Where an inline would have its function's */
/* name, the function keeps it and the inline takes the FieldActor_ prefix, as
 */
/* syFieldActorJumpDownLedge does. */

#include "fixed_math.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "field/field_sequence.h"
#include "stubs.h"
#include "system.h"
#include "game/system_call.h"

/* Turns two actors toward each other, as a conversation between them */
/* begins. */
static inline void fdActorFaceEachOther(s32 actorId, s32 other, s32 frames) {
	fdActorTurnToFaceEachOther(actorId, other, frames);
}

/* Holds the event until the camera's pan is done. */
static inline void fdCameraWaitForMove(void) { fdCameraWaitForArrival(); }

/* Runs an event's palette fade, from the source colours to the target ones, */
/* over frames. */
static inline void fdPaletteFadeInterpolate(s32 frames) {
	fdPaletteFadeStartInterpolation(frames);
}

/* Draws an item's icon, ready for an event to show it. */
static inline void gmItemLoadIcon(s32 item) { mnIconDrawItem(item); }

/* Holds an item's picture up over the leader, as when the party finds it. */
static inline void gmItemShowFound(s32 item, s32 flags) {
	fdItemDispShowItem(item, flags);
}

/* Redraws the room around the point the camera follows, after an event has */
/* moved it. */
static inline void fdMapGridRedraw(void) { fdMapGridCenterOnFollowedPoint(); }

/* Shows a message in the middle of the screen rather than in a speech window.
 */
static inline void mnMessageShowCentered(s32 message, s32 flags) {
	mnFieldMessageShow(message, flags);
}

/* Gives the party an item from an event and says who got it; -1 when it was */
/* left behind. */
static inline s32 gmPartyGiveItem(s32 item, s32 flags) {
	return fdPartyInventoryGiveItem(item, flags);
}

/* Ends an event's wave effect and restores the colours it changed. */
static inline void syPsynergyCancel(void) { fdPsynergyStopMindReadWave(); }

/* Loads a scene's sprite picture into VRAM; returns its first tile number, 0 */
/* when there's no room. */
static inline s32 mnVramLoad(s32 block, s32 size, const void *data) {
	return syVramCacheLoad(block, size, data);
}

/* fdActorSetDestinationOffset, by a second inline over the same function. */
static inline void syFieldActorSetDestinationOffset(s32 actorId, s32 xOffset,
													s32 zOffset) {
	fdActorSetDestinationOffsetById(actorId, xOffset, zOffset);
}

/* syTaskAddFrame (system_call.h) under a second name. */
static inline s32 syTaskAddOrUpdateFrame(TASK_FUNC func, s32 priority) {
	return syTaskAddFrame(func, priority);
}

/* Changes the cell behavior of the cell at (x, y), in whole pixels, opening */
/* or closing the way (fdMapGridSetCellBehaviorAt): */
/* `fdSetCellBehaviorAt(0, 544, 288, CELL_BEHAVIOR_CLIMB)`. */
static inline void fdSetCellBehaviorAt(s32 layer, s32 x, s32 y, s32 behavior) {
	fdMapGridSetCellBehaviorAt(layer, toFIXED(x), toFIXED(y), behavior);
}

/* Changes one 512-pixel tile of the World Map, drawn at once when it's next to
 */
/* the view (fdWorldMapSetGridTile). */
static inline void fdSetWorldMapGridTile(s32 x, s32 y, s32 cellX, s32 cellY) {
	fdWorldMapSetGridTile(x, y, cellX, cellY);
}

/* Puts an actor on the leader and sends it walking off by (dx, dz), then */
/* turns it to face facing (fdActorLaunchFromLeader). */
static inline void syFieldActorLaunchFromLeader(s32 actorId, s32 dx, s32 dz,
												s32 facing) {
	fdActorLaunchFromLeader(actorId, dx, dz, facing);
}

/* Makes an actor follow another and starts a script on it */
/* (fdActorWatchAndStartScript). */
static inline void syFieldActorWatchAndStartScript(s32 object, s32 target,
												   const s32 *actions) {
	fdActorWatchAndStartScript(object, target, actions);
}

/* Moves a point radius, in 16.16 fixed point, along a heading on the ground */
/* plane (syVectorAddPolarOffset). */
static inline void syFieldVectorAddPolarOffsetFX(FIXED radius, s32 heading,
												 FIELD_POS *probe) {
	syVectorAddPolarOffset(radius, heading, (FIXED *)probe);
}

/* Moves a point a radius in whole pixels along a heading: */
/* `syFieldVectorAddPolarOffset(16, heading, &probe)`. */
#define syFieldVectorAddPolarOffset(radius, heading, probe)                    \
	syFieldVectorAddPolarOffsetFX(toFIXED(radius), heading, probe)

#endif
