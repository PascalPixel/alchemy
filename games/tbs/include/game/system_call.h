#ifndef ALCHEMY_SYSTEM_CALL_H
#define ALCHEMY_SYSTEM_CALL_H

/* The system call layer: an inline for each of the thirteen main-image
   functions below that the scene, menu and debug overlays import, which every
   overlay call of them goes through.

   An overlay writes `sySetFrameEvent(func, EVENT_PASS_FRAME)`, and the inline
   calls sySetEvent through the overlay's import stub. Main-image code calls the
   functions by their own names, except in scene/shared/lemurian_ship_ripple.c,
   which field/script_commands_line_scroll.c includes as lemurian_ship.c does.
   script_inline.h includes this layer; a room without the script commands
   includes it itself. */

#include "types.h"
#include "field/script_commands.h"
#include "game/game.h"
#include "scene/import.h"
#include "system.h"
#include "window/window.h"

struct ACTOR;
struct FIELD_POS;
struct MENU_SPR;

/* Adds `func` as an event of priority `priority` (EVENT_PASS_FRAME runs it once
   a frame), or gives the event already running it that priority. */
static inline Sint32 sySetFrameEvent(EVENT_FUNC func, Sint32 priority)
{
	return sySetEvent(func, priority);
}

/* The overlays' names for the functions they import, each the function
   itself. */
static inline void syFieldMenuArrowCreate(MENU_SPR *cursor, Sint32 *slot)
{
	mnArrowCreate(cursor, slot);
}

static inline void syFieldMenuArrowSetPos(MENU_SPR *cursor, Sint32 x, Sint32 y)
{
	mnArrowSetPos(cursor, x, y);
}

static inline void mnFieldMessageDecodeRawUnbounded(
	Sint32 message, Uint16 *buffer)
{
	mnMessageDecodeRawUnbounded(message, buffer);
}

static inline void fdSetCameraBounds(
	Sint32 minX, Sint32 minY, Sint32 maxX, Sint32 maxY)
{
	fdMapGridSetCameraBounds(minX, minY, maxX, maxY);
}

static inline void syFieldActorJumpDownLedge(
	Sint32 actorId, Sint32 mode, Sint32 action)
{
	fdActorJumpDownLedge(actorId, mode, action);
}

static inline Sint32 syFieldActorCheckMovementCollision(
	ACTOR *actor, FIELD_POS *position)
{
	return fdMapGridCheckCollision(actor, position);
}

static inline Sint32 syFieldGameFlagGetByte(Sint32 flag)
{
	return gmFlagGetByte(flag);
}

static inline void syFieldGameFlagSetByte(Sint32 flag, Sint32 value)
{
	gmFlagSetByte(flag, value);
}

static inline Sint32 syFieldPartyListMembers(Sint16 *owners)
{
	return gmPartyListMembers(owners);
}

static inline Sint32 syFieldUnitAddHp(Sint32 owner, Sint32 delta)
{
	return gmUnitAddHp(owner, delta);
}

/* Decodes a tagged LZ stream (a first byte of 0 or 1) from `source` into
   `destination`. */
static inline Sint32 syFieldDecompressTaggedLZ(
	const void *source, void *destination)
{
	return syDecompressTaggedLZ(source, destination);
}

static inline void fdLineScrollStartWave(Sint32 mode, Sint32 freqX,
	Sint32 stepX, Sint32 ampX, Sint32 freqY, Sint32 stepY, Sint32 ampY)
{
	scLineScrollInitWave(mode, freqX, stepX, ampX, freqY, stepY, ampY);
}

#endif
