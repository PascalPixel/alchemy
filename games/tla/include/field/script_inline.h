#ifndef ALCHEMY_SCRIPT_INLINE_H
#define ALCHEMY_SCRIPT_INLINE_H

// The script inlines: one typed inline for each script command a script calls by its own name,
// passing it on to the main image's import (fdActorFaceEachOther calls fdActorTurnToFaceEachOther).
//
// A script writes `fdActorFaceEachOther(4, 8, 30);` and the inline makes the call. Where an inline
// would have its function's name, the function keeps it and the inline takes the Field prefix, as
// scFieldActorJump does, or Script in place of a Field the function's name already has
// (syScriptCameraGetActor).

#include "script.h"
#include "fixed_math.h"
#include "system.h"
#include "game/system_call.h"

// Turns two actors toward each other, as a conversation between them begins.
static inline void fdActorFaceEachOther(Sint32 actorId, Sint32 other, Sint32 ticks)
{
	fdActorTurnToFaceEachOther(actorId, other, ticks);
}

// Switches an actor to an animation of its sprite.
static inline void fdActorSetAnimation(Sint32 actorId, Sint32 animation)
{
	fdActorSetAnimationById(actorId, animation);
}

// fdActorSetAnimation as the scripts that take fdActorSetAnimationById for a function returning int
// make the call.
static inline void scFieldActorSetAnimation(Sint32 actorId, Sint32 animation)
{
#if FIX_FUNCTION_SIGNATURES
	fdActorSetAnimationById(actorId, animation);
#else
	// UB: fdActorSetAnimationById returns nothing, but these calls are made as to a function
	// returning int, which moves their argument registers.
	((Sint32 (*)(Uint32, Sint32))fdActorSetAnimationById)(actorId, animation);
#endif
}

// Runs a script's palette fade, from the source colours to the target ones, over ticks.
static inline void fdPaletteFadeInterpolate(Sint32 ticks)
{
	fdPaletteFadeStartInterpolation(ticks);
}

// Draws an item's icon, ready for a script to show it.
static inline void gmItemLoadIcon(Sint32 item)
{
	mnIconDrawItem(item);
}

// The sine of an angle, a full turn being TRIG_TURN, in 16.16.
static inline FIXED syMathSin(ANGLE angle)
{
	return sySin(angle);
}

// Shows a message in the middle of the screen rather than in a speech window.
static inline void mnMessageShowCentered(Sint32 msg, Sint32 flags)
{
	mnFieldMessageShow(msg, flags);
}

// Gives the party an item from a script and says who got it; -1 when it was left behind.
static inline Sint32 gmPartyGiveItem(Sint32 item, Sint32 flags)
{
	return fdPartyInventoryGiveItem(item, flags);
}

// Loads a scene's sprite picture into VRAM; returns its first tile number, 0 when there's no room.
static inline Sint32 mnVRAMLoad(Sint32 block, Sint32 size, const void *data)
{
	return syVRAMCacheLoad(block, size, data);
}

// Puts an actor on the leader and sends it walking off by (dx, dz), then starts the movement type
// facing names (fdActorLaunchFromLeader).
static inline void scFieldActorLaunchFromLeader(Sint32 actorId, Sint32 dx, Sint32 dz, Sint32 facing)
{
	fdActorLaunchFromLeader(actorId, dx, dz, facing);
}

// Moves a point radius, in 16.16 fixed point, along a heading on the ground plane
// (syVectorAddPolarOffset).
static inline void syFieldVectorAddPolarOffsetFX(FIXED radius, Sint32 heading, FIXED *probe)
{
	syVectorAddPolarOffset(radius, heading, probe);
}

// Moves a point a radius in whole pixels along a heading: `syFieldVectorAddPolarOffset(16, heading,
// &probe)`.
#define syFieldVectorAddPolarOffset(radius, heading, probe)                                        \
	syFieldVectorAddPolarOffsetFX(toFIXED(radius), heading, probe)

// Changes the metatile behavior of the metatile at (x, y), in 16.16 pixels, opening or closing the
// way (fdMapGridSetMetatileBehaviorAt).
static inline void fdSetMetatileBehaviorAt(Sint32 layer, FIXED x, FIXED y, Sint32 behavior)
{
	fdMapGridSetMetatileBehaviorAt(layer, x, y, behavior);
}

// Changes one 512-pixel tile of the World Map, drawn at once when it's next to the view
// (fdWorldMapSetWindowMetatileTile).
static inline void
fdSetWorldMapWindowMetatileTile(Sint32 x, Sint32 y, Sint32 metatileX, Sint32 metatileY)
{
	fdWorldMapSetWindowMetatileTile(x, y, metatileX, metatileY);
}

// Holds the script for ticks while the field goes on (fdScriptWait).
static inline void syFieldScriptWait(Sint32 ticks)
{
	fdScriptWait(ticks);
}

// Turns an actor to a direction and holds the script until it has turned.
static inline void fdActorFaceDirectionAndWait(Sint32 actorId, Sint32 facing)
{
	fdActorFaceDirectionAndRefreshById(actorId, facing);
}

// Places an actor at a point at once, as fdActorSetPos does, facing a direction.
static inline void fdActorSetPosAndFacing(Sint32 actorId, FIXED x, FIXED z, Uint16 facing)
{
	fdActorSetHorizontalPosAndAngleWithTerrain(actorId, x, z, facing);
}

// fdActorSetAnimationAndWait as the scripts that take fdActorSetAnimationAndWait for a function
// returning int make the call.
static inline void scFieldActorSetAnimationAndWait(Sint32 actorId, Sint32 animation)
{
#if FIX_FUNCTION_SIGNATURES
	fdActorSetAnimationAndWait(actorId, animation);
#else
	// UB: fdActorSetAnimationAndWait returns nothing, but these calls are made as to a function
	// returning int, which moves their argument registers.
	((Sint32 (*)(Uint32, Sint32))fdActorSetAnimationAndWait)(actorId, animation);
#endif
}

// Throws an actor upward at speed pixels a tick, then holds the script for ticks.
static inline void scFieldActorJump(Sint32 actorId, Sint32 speed, Sint32 ticks)
{
	fdActorJump(actorId, speed, ticks);
}

// Fixes the priority of an actor's sprite, which then no longer follows its depth.
static inline void scFieldActorSetSpritePriority(Sint32 actorId, Sint32 priority)
{
	fdActorSetSpritePriority(actorId, priority);
}

// Squashes an actor and springs it back count times.
static inline void scFieldActorStartRepeatedMotion(Sint32 actorId, Sint32 count)
{
	fdActorStartRepeatedMotion(actorId, count);
}

// Makes an actor follow another.
static inline void scFieldActorWatchAndSetScript(Sint32 actorId, Sint32 watchedActorId)
{
	fdActorWatchAndSetScript(actorId, watchedActorId);
}

// Holds the script until an actor arrives, 600 ticks at most.
static inline void scFieldActorWaitForMoveById(Sint32 actorId)
{
	fdActorWaitForMoveById(actorId);
}

// Ends Reveal, the ring of light and Sand, each unless skip holds its bit.
static inline void syFieldScriptEndLastingPsynergy(Sint32 skip)
{
	fdScriptEndLastingPsynergy(skip);
}

// Makes message the script's next line.
static inline void syFieldScriptSetMessageId(Sint32 msg)
{
	fdScriptSetMessageId(msg);
}

// Makes the camera follow an actor (fdCameraFollowActor).
static inline void syScriptCameraFollowActor(Sint32 actorId, Sint32 keepPos)
{
	fdCameraFollowActor(actorId, keepPos);
}

// Places an actor at (x, y, z), 16.16 pixels, and stops it.
static inline void scFieldActorSetPosAndResetMotion(struct ACTOR *actor, FIXED x, FIXED y, FIXED z)
{
	fdActorSetPosAndResetMotion(actor, x, y, z);
}

// Holds the script until an actor's script ends, 300 ticks at most.
static inline void scFieldActorWaitForScriptEndById(Sint32 actorId)
{
	fdActorWaitForScriptEndById(actorId);
}

// Sets the colour mode of an actor's sprite (fdActorSetSpriteColor).
static inline void scFieldActorSetSpriteColor(struct ACTOR *actor, Sint32 colorMode)
{
	fdActorSetSpriteColor(actor, colorMode);
}

// Stops an actor's movement type and stands it on the idle script.
static inline void scFieldActorStop(Sint32 actorId)
{
	fdActorStop(actorId);
}

// Requests the exit the field leaves the scene by.
static inline void syFieldScriptSetExitRequest(Sint32 exit)
{
	syImportScriptSetExitRequest(exit);
}

// Holds the script until an actor has reached its destination (fdActorWaitForMove).
static inline void scFieldActorWaitForMove(void *actor)
{
	fdActorWaitForMove(actor);
}

// Who carries an item, asking the leader first, or -1.
static inline Sint32 syFieldPartyInventoryFindMember(Sint32 item)
{
	return gmPartyInventoryFindMember(item);
}

// Shows a message in a window in the middle of the screen (mnFieldMessageShowCentered).
static inline void syScriptMessageShowCentered(Sint32 msg, Sint32 mode, Sint32 yOffset)
{
	mnFieldMessageShowCentered(msg, mode, yOffset);
}

// The leader's actor id.
static inline Sint32 syFieldPartyGetLeaderActor(void)
{
	return fdPartyGetLeaderActor();
}

// Asks yes or no as the speaker nods or shakes its head; returns 0 for yes.
static inline Sint32 syFieldScriptAskYesNoWithActorAnimation(Sint32 actorId, Sint32 flags)
{
	return fdScriptAskYesNoWithActorAnimation(actorId, flags);
}

// Sets where the party goes after the script's battle (fdScriptSetAfterBattleDestination).
static inline void syFieldScriptSetAfterBattleDestination(Sint32 sceneId, Sint32 entrance)
{
	fdScriptSetAfterBattleDestination(sceneId, entrance);
}

// Adds a Djinni to the party (fdDjinnJoinParty).
static inline void syFieldDjinnJoinParty(Sint32 slot, Sint32 element, Sint32 djinn)
{
	fdDjinnJoinParty(slot, element, djinn);
}

// The actor the camera follows (fdCameraGetActor).
static inline struct ACTOR *syScriptCameraGetActor(void)
{
	return fdCameraGetActor();
}

// Asks the player yes or no (fdYesNoAsk).
static inline Sint32 syScriptYesNoAsk(Sint32 atBottom)
{
	return fdYesNoAsk(atBottom);
}

// Zooms the World Map's view to zoom, 16.16, over ticks.
static inline void syFieldWorldMapStartZoom(FIXED zoom, Sint32 ticks)
{
	fdWorldMapStartZoom(zoom, ticks);
}

// Applies a window colour (mnWindowColorApply).
static inline void syFieldWindowColorApply(Sint32 color, Sint32 brightness)
{
	mnWindowColorApply(color, brightness);
}

// Requests a battle with a Djinni (fdRequestDjinnBattle).
static inline void fdScriptRequestDjinnBattle(Sint32 actorId, Sint32 djinni)
{
	fdRequestDjinnBattle(actorId, djinni);
}

// Adds unit to the party, announcing it when announce is nonzero.
static inline void syFieldScriptAddPartyMember(Sint32 unit, Sint32 announce)
{
	fdScriptAddPartyMember(unit, announce);
}

// Holds the script until the screen shake dies down (fdMapGridWaitForShakeToSettle).
static inline void syFieldMapGridWaitForShakeToSettle(void)
{
	fdMapGridWaitForShakeToSettle();
}

// The metatile behavior of the metatile at (x, y) (fdMapGridGetMetatileBehaviorAt).
static inline Sint32 syFieldMapGridGetMetatileBehaviorAt(Sint32 layer, FIXED x, FIXED y)
{
	return fdMapGridGetMetatileBehaviorAt(layer, x, y);
}

// Draws an item's icon into an actor's sprite, or blanks it (fdActorShowItemIconById).
static inline void scFieldActorShowItemIconById(Sint32 actorId, Sint32 item)
{
	fdActorShowItemIconById(actorId, item);
}

// Draws an item's icon into an actor's sprite (fdActorSetItemIcon).
static inline void scFieldActorSetItemIcon(struct ACTOR *actor, Sint32 item)
{
	fdActorSetItemIcon(actor, item);
}

// Changes one tile of the World Map and draws it (fdWorldMapSetWindowMetatileTileAndDraw).
static inline void
syFieldWorldMapSetWindowMetatileTileAndDraw(Sint32 x, Sint32 y, Sint32 metatileX, Sint32 metatileY)
{
	fdWorldMapSetWindowMetatileTileAndDraw(x, y, metatileX, metatileY);
}

// Moves the camera onto an actor, to its height too (fdCameraMoveToActor).
static inline void syScriptCameraMoveToActor(Sint32 actorId, Sint32 pan)
{
	fdCameraMoveToActor(actorId, pan);
}

// Rotates an actor's sprite (fdActorSetSpriteRotationById).
static inline void scFieldActorSetSpriteRotationById(Sint32 actorId, Sint32 angle)
{
	fdActorSetSpriteRotationById(actorId, angle);
}

// Sets the metatile behavior of a metatile (fdMapGridSetMetatileBehavior).
static inline Sint32
syFieldMapGridSetMetatileBehavior(Sint32 x, Sint32 z, Sint32 layer, Sint32 behavior)
{
	return fdMapGridSetMetatileBehavior(x, z, layer, behavior);
}

// Scoop digs up a buried treasure: with `mode` RETURN_HOME_DUG_UP, the treasure actor comes up at
// its home and its flag is set (fdActorReturnHome).
static inline void fdScriptDigUpTreasure(Sint32 mode, Sint32 actorId, Sint32 flag)
{
	fdActorReturnHome(mode, actorId, flag);
}

#endif
