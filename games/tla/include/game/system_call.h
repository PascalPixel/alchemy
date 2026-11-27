#ifndef ALCHEMY_SYSTEM_CALL_H
#define ALCHEMY_SYSTEM_CALL_H

/* The names the scene and debug overlays call these main-image functions by:
   one typed inline each, which calls the function through the overlay's import
   stub. An overlay writes `sySetFrameEvent(func, EVENT_PASS_FRAME)` where the
   main image writes sySetEvent. script_inline.h includes it. */

#include "field/script.h"
#include "graphics/resource.h"
#include "game/game_functions.h"
#include "window/window.h"

/* Adds `func` as an event of priority `priority` (EVENT_PASS_FRAME runs it once
   a frame), or gives the event already running it that priority. Returns the
   slot it wrote, or -1 when all 24 event slots are taken. sySetEvent under the
   name overlays import it by. */
static inline Sint32 sySetFrameEvent(EVENT_FUNC func, Sint32 priority)
{
	return sySetEvent(func, priority);
}

/* sySetFrameEvent, for a caller that leaves the slot unread. */
static inline void syFieldSetFrameEvent(EVENT_FUNC func, Sint32 priority)
{
	sySetEvent(func, priority);
}

/* Decodes a tagged LZ stream (a first byte of 0 or 1) from `source` into
   `destination`. */
static inline Sint32 syFieldDecompressTaggedLZ(
	const void *source, void *destination)
{
	return syDecompressTaggedLZ(source, destination);
}

static inline void fdMessageDecodeRawUnbounded(Sint32 message, Uint16 *buffer)
{
	mnMessageDecodeRawUnbounded(message, buffer);
}

static inline void fdSetCameraBounds(
	Sint32 minX, Sint32 minY, Sint32 maxX, Sint32 maxY)
{
	fdMapGridSetCameraBounds(minX, minY, maxX, maxY);
}

static inline Sint32 scFieldGameFlagGetByte(Sint32 flag)
{
	return syGameFlagGetByte(flag);
}

static inline Sint32 syFieldPartyListMembers(Sint16 *owners)
{
	return gmPartyListMembers(owners);
}

static inline Sint32 scFieldUnitAddHp(Sint32 owner, Sint32 delta)
{
	return gmUnitAddHp(owner, delta);
}

/* Lets frames of the game pass (sySynch()). */
static inline void syWait(Sint32 frames)
{
	sySynch(frames);
}

/* syCloseEvent, from an overlay. */
static inline void syFieldCloseEvent(EVENT_FUNC func)
{
	syCloseEvent(func);
}

#endif
