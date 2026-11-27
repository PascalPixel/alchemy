#ifndef ALCHEMY_SYSTEM_CALL_H
#define ALCHEMY_SYSTEM_CALL_H

/* The names the scene and debug overlays call these main-image functions by:
   one typed inline each, which calls the function through the overlay's import
   stub. An overlay writes `sySetTickEvent(func, EVENT_PASS_TICK)` where the
   main image writes sySetEvent. script_inline.h includes it. */

#include "field/script.h"
#include "graphics/resource.h"
#include "game/game_functions.h"
#include "window/window.h"

/* Adds `func` as an event of priority `pri` (EVENT_PASS_TICK runs it once
   a tick), or gives the event already running it that priority. Returns the
   slot it wrote, or -1 when all 24 event slots are taken. sySetEvent under the
   name overlays import it by. */
static inline Sint32 sySetTickEvent(EVENT_FUNC func, Sint32 pri)
{
	return sySetEvent(func, pri);
}

/* sySetTickEvent, for a caller that leaves the slot unread. */
static inline void syFieldSetTickEvent(EVENT_FUNC func, Sint32 pri)
{
	sySetEvent(func, pri);
}

/* Decodes a tagged LZ stream (a first byte of 0 or 1) from `src` into
   `dst`. */
static inline Sint32 syFieldDecompressTaggedLZ(const void *src, void *dst)
{
	return syDecompressTaggedLZ(src, dst);
}

static inline void fdMessageDecodeRawUnbounded(Sint32 msg, Uint16 *buf)
{
	mnMessageDecodeRawUnbounded(msg, buf);
}

static inline void fdSetCameraBounds(
	Sint32 minX, Sint32 minY, Sint32 maxX, Sint32 maxY)
{
	fdMapGridSetCameraBounds(minX, minY, maxX, maxY);
}

static inline Sint32 scFieldGameFlagGetByte(Sint32 flg)
{
	return syGameFlagGetByte(flg);
}

static inline Sint32 syFieldPartyListMembers(Sint16 *owners)
{
	return gmPartyListMembers(owners);
}

static inline Sint32 scFieldUnitAddHp(Sint32 owner, Sint32 delta)
{
	return gmUnitAddHp(owner, delta);
}

/* Lets ticks of the game pass (sySynch()). */
static inline void syWait(Sint32 ticks)
{
	sySynch(ticks);
}

/* syCloseEvent, from an overlay. */
static inline void syFieldCloseEvent(EVENT_FUNC func)
{
	syCloseEvent(func);
}

#endif
