#ifndef ALCHEMY_SYSTEM_CALL_H
#define ALCHEMY_SYSTEM_CALL_H

/* @file */
/* The names the scene and debug overlays call these main-image functions */
/* by: one typed inline each, which calls the function through the overlay's */
/* import stub. An overlay writes `syTaskAddFrame(func, TASK_PASS_FRAME)` where
 */
/* the main image writes sySetEvent. event_inline.h includes it. */

#include "field/event.h"
#include "graphics/resource.h"
#include "game/game_functions.h"
#include "window/window.h"

/* Adds `func` as a task of priority `priority` (TASK_PASS_FRAME */
/* runs it once a frame), or gives the task already running it that */
/* priority. Returns the slot it wrote, or -1 when all 24 task slots are */
/* taken. sySetEvent under the name overlays import it by. */
static inline s32 syTaskAddFrame(TASK_FUNC func, s32 priority) {
	return sySetEvent(func, priority);
}

/* syTaskAddFrame, for a caller that leaves the slot unread. */
static inline void syFieldTaskAddFrame(TASK_FUNC func, s32 priority) {
	sySetEvent(func, priority);
}

/* Decodes a tagged LZ stream (a first byte of 0 or 1) from `source` into */
/* `destination`. */
static inline s32 syFieldDecompressTaggedLz(const void *source,
											void *destination) {
	return syDecompressTaggedLz(source, destination);
}

static inline void fdMessageDecodeRawUnbounded(s32 message, u16 *buffer) {
	mnMessageDecodeRawUnbounded(message, buffer);
}

static inline void fdSetCameraBounds(s32 minX, s32 minY, s32 maxX, s32 maxY) {
	fdMapGridSetCameraBounds(minX, minY, maxX, maxY);
}

static inline s32 scFieldGameFlagGetByte(s32 flag) {
	return syGameFlagGetByte(flag);
}

static inline s32 syFieldPartyListMembers(s16 *owners) {
	return gmPartyListMembers(owners);
}

static inline s32 scFieldUnitAddHp(s32 owner, s32 delta) {
	return gmUnitAddHp(owner, delta);
}

/* Lets frames of the game pass (sySynch()). */
static inline void syWait(s32 frames) { sySynch(frames); }

/* syCloseEvent, from an overlay. */
static inline void syFieldTaskRemove(TASK_FUNC func) { syCloseEvent(func); }

#endif
