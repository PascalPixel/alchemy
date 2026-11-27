#ifndef ALCHEMY_SYSTEM_CALL_H
#define ALCHEMY_SYSTEM_CALL_H

/* @file */
/* The system call layer: an inline for each of the thirteen main-image */
/* functions below that the scene, menu and debug overlays import, which every
 */
/* overlay call of them goes through. */
/* */
/* An overlay writes `syTaskAddFrame(func, TASK_PASS_FRAME)`, and the inline */
/* calls sySetEvent through the overlay's import stub. Main-image code calls */
/* the functions by their own names, except in */
/* scene/shared/lemurian_ship_ripple.c, which */
/* field/event_commands_line_scroll.c includes as lemurian_ship.c does. */
/* event_inline.h includes this layer; a room without the event commands */
/* includes it itself. */

#include "types.h"
#include "field/event_commands.h"
#include "game/game.h"
#include "scene/import.h"
#include "system.h"
#include "window/window.h"

struct ACTOR;
struct FIELD_POS;
struct MENU_SPR;

/* Adds `func` as a task of priority `priority` (TASK_PASS_FRAME runs it once a
 */
/* frame), or gives the task already running it that priority. */
static inline s32 syTaskAddFrame(TASK_FUNC func, s32 priority) {
	return sySetEvent(func, priority);
}

/* The overlays' names for the functions they import, each the function */
/* itself. */
static inline void syFieldMenuArrowCreate(MENU_SPR *cursor, s32 *slot) {
	mnArrowCreate(cursor, slot);
}

static inline void syFieldMenuArrowSetPos(MENU_SPR *cursor, s32 x, s32 y) {
	mnArrowSetPos(cursor, x, y);
}

static inline void mnFieldMessageDecodeRawUnbounded(s32 message, u16 *buffer) {
	mnMessageDecodeRawUnbounded(message, buffer);
}

static inline void fdSetCameraBounds(s32 minX, s32 minY, s32 maxX, s32 maxY) {
	fdMapGridSetCameraBounds(minX, minY, maxX, maxY);
}

static inline void syFieldActorJumpDownLedge(s32 actorId, s32 mode,
											 s32 action) {
	fdActorJumpDownLedge(actorId, mode, action);
}

static inline s32 syFieldActorCheckMovementCollision(ACTOR *actor,
													 FIELD_POS *position) {
	return fdMapGridCheckCollision(actor, position);
}

static inline s32 syFieldGameFlagGetByte(s32 flag) {
	return gmFlagGetByte(flag);
}

static inline void syFieldGameFlagSetByte(s32 flag, s32 value) {
	gmFlagSetByte(flag, value);
}

static inline s32 syFieldPartyListMembers(s16 *owners) {
	return gmPartyListMembers(owners);
}

static inline s32 syFieldUnitAddHp(s32 owner, s32 delta) {
	return gmUnitAddHp(owner, delta);
}

/* Decodes a tagged LZ stream (a first byte of 0 or 1) from `source` into */
/* `destination`. */
static inline s32 syFieldDecompressTaggedLz(const void *source,
											void *destination) {
	return fdDecompressTaggedLz(source, destination);
}

static inline void fdLineScrollStartWave(s32 mode, s32 freqX, s32 stepX,
										 s32 ampX, s32 freqY, s32 stepY,
										 s32 ampY) {
	scLineScrollInitWave(mode, freqX, stepX, ampX, freqY, stepY, ampY);
}

#endif
