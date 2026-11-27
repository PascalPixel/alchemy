#ifndef ALCHEMY_SCRIPT_INLINE_H
#define ALCHEMY_SCRIPT_INLINE_H

/* The script inlines: one typed inline for each script command a script calls
   by its own name, passing it on to the main image's import
   (fdActorFaceEachOther calls fdActorTurnToFaceEachOther).

   A script writes `fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 30);`
   and the inline makes the call. Where an inline would have its function's
   name, the function keeps it and the inline takes the syFieldActor prefix, as
   syFieldActorJumpDownLedge does. */

#include "fixed_math.h"
#include "field/script.h"
#include "field/script_commands.h"
#include "field/field.h"
#include "field/field_mode.h"
#include "stubs.h"
#include "system.h"
#include "game/system_call.h"

/* Turns two actors toward each other, as a conversation between them begins. */
static inline void fdActorFaceEachOther(
	Sint32 actorId, Sint32 other, Sint32 ticks)
{
	fdActorTurnToFaceEachOther(actorId, other, ticks);
}

/* Holds the script until the camera's pan is done. */
static inline void fdCameraWaitForMove(void)
{
	fdCameraWaitForArrival();
}

/* Runs a script's palette fade, from the source colours to the target ones,
   over ticks. */
static inline void fdPaletteFadeInterpolate(Sint32 ticks)
{
	fdPaletteFadeStartInterpolation(ticks);
}

/* Draws an item's icon, ready for a script to show it. */
static inline void gmItemLoadIcon(Sint32 item)
{
	mnIconDrawItem(item);
}

/* Holds an item's picture up over the leader, as when the party finds it. */
static inline void gmItemShowFound(Sint32 item, Sint32 flg)
{
	fdItemDispShowItem(item, flg);
}

/* Redraws the room around the point the camera follows, after a script has
   moved it. */
static inline void fdMapGridRedraw(void)
{
	fdMapGridCenterOnFollowedPoint();
}

/* Shows a message in the middle of the screen rather than in a speech
   window. */
static inline void mnMessageShowCentered(Sint32 msg, Sint32 flg)
{
	mnFieldMessageShow(msg, flg);
}

/* Gives the party an item from a script and says who got it; -1 when it was
   left behind. */
static inline Sint32 gmPartyGiveItem(Sint32 item, Sint32 flg)
{
	return fdPartyInventoryGiveItem(item, flg);
}

/* Ends a script's wave effect and restores the colours it changed. */
static inline void syPsynergyCancel(void)
{
	fdPsynergyStopMindReadWave();
}

/* Loads a scene's sprite picture into VRAM; returns its first tile number, 0
   when there's no room. */
static inline Sint32 mnVRAMLoad(Sint32 block, Sint32 size, const void *data)
{
	return syVRAMCacheLoad(block, size, data);
}

/* fdActorSetDestinationOffset, by a second inline over the same function. */
static inline void syFieldActorSetDestinationOffset(
	Sint32 actorId, Sint32 xOfs, Sint32 zOfs)
{
	fdActorSetDestinationOffsetById(actorId, xOfs, zOfs);
}

/* sySetTickEvent (system_call.h) under a second name. */
static inline Sint32 sySetOrUpdateTickEvent(EVENT_FUNC func, Sint32 pri)
{
	return sySetTickEvent(func, pri);
}

/* Changes the metatile behavior of the metatile at (x, y), in whole pixels,
   opening or closing the way (fdMapGridSetMetatileBehaviorAt):
   `fdSetMetatileBehaviorAt(0, 544, 288, METATILE_BEHAVIOR_CLIMB)`. */
static inline void fdSetMetatileBehaviorAt(
	Sint32 layer, Sint32 x, Sint32 y, Sint32 behavior)
{
	fdMapGridSetMetatileBehaviorAt(layer, toFIXED(x), toFIXED(y), behavior);
}

/* Changes one 512-pixel tile of the World Map, drawn at once when it's next to
   the view (fdWorldMapSetGridTile). */
static inline void fdSetWorldMapGridTile(
	Sint32 x, Sint32 y, Sint32 metatileX, Sint32 metatileY)
{
	fdWorldMapSetGridTile(x, y, metatileX, metatileY);
}

/* Puts an actor on the leader and sends it walking off by (dx, dz), then turns
   it to face facing (fdActorLaunchFromLeader). */
static inline void syFieldActorLaunchFromLeader(
	Sint32 actorId, Sint32 dx, Sint32 dz, Sint32 facing)
{
	fdActorLaunchFromLeader(actorId, dx, dz, facing);
}

/* Makes an actor follow another and starts a script on it
   (fdActorWatchAndStartScript). */
static inline void syFieldActorWatchAndStartScript(
	Sint32 object, Sint32 tgt, const Sint32 *actions)
{
	fdActorWatchAndStartScript(object, tgt, actions);
}

/* Moves a point radius, in 16.16 fixed point, along a heading on the ground
   plane (syVectorAddPolarOffset). */
static inline void syFieldVectorAddPolarOffsetFX(
	FIXED radius, Sint32 heading, FIELD_POS *probe)
{
	syVectorAddPolarOffset(radius, heading, (FIXED *)probe);
}

/* Moves a point a radius in whole pixels along a heading:
   `syFieldVectorAddPolarOffset(16, heading, &probe)`. */
#define syFieldVectorAddPolarOffset(radius, heading, probe) \
	syFieldVectorAddPolarOffsetFX(toFIXED(radius), heading, probe)

#endif
