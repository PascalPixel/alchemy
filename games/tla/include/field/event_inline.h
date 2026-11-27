#ifndef ALCHEMY_EVENT_INLINE_H
#define ALCHEMY_EVENT_INLINE_H

/* @file */
/* The event inlines: one typed inline for each event command an event calls */
/* by its own name, passing it on to the main image's import */
/* (fdActorFaceEachOther calls fdActorTurnToFaceEachOther). */
/* */
/* An event writes `fdActorFaceEachOther(4, 8, 30);` and the inline makes the */
/* call. Where an inline would have its function's name, the function keeps it
 */
/* and the inline takes the Field prefix, as scFieldActorJump does, or Event in
 */
/* place of a Field the function's name already has (syEventCameraGetActor). */

#include "event.h"
#include "fixed_math.h"
#include "system.h"
#include "game/system_call.h"

/* Turns two actors toward each other, as a conversation between them */
/* begins. */
static inline void fdActorFaceEachOther(s32 actorId, s32 other, s32 frames) {
	fdActorTurnToFaceEachOther(actorId, other, frames);
}

/* Switches an actor to an animation of its sprite. */
static inline void fdActorSetAnimation(s32 actorId, s32 animation) {
	fdActorSetAnimationById(actorId, animation);
}

/* fdActorSetAnimation as the events that take fdActorSetAnimationById for a */
/* function returning int make the call. */
static inline void scFieldActorSetAnimation(s32 actorId, s32 animation) {
#ifdef UBFIX
	fdActorSetAnimationById(actorId, animation);
#else
	/* UB: fdActorSetAnimationById returns nothing, but these calls are made as
	 * to */
	/* a function returning int, which moves their argument registers. */
	((s32 (*)(u32, s32))fdActorSetAnimationById)(actorId, animation);
#endif
}

/* Runs an event's palette fade, from the source colours to the target ones, */
/* over frames. */
static inline void fdPaletteFadeInterpolate(s32 frames) {
	fdPaletteFadeStartInterpolation(frames);
}

/* Draws an item's icon, ready for an event to show it. */
static inline void gmItemLoadIcon(s32 item) { mnIconDrawItem(item); }

/* The sine of an angle, a full turn being TRIG_TURN, in 16.16. */
static inline FIXED syMathSin(ANGLE angle) { return sySin(angle); }

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

/* Loads a scene's sprite picture into VRAM; returns its first tile number, 0 */
/* when there's no room. */
static inline s32 mnVramLoad(s32 block, s32 size, const void *data) {
	return syVramCacheLoad(block, size, data);
}

/* Puts an actor on the leader and sends it walking off by (dx, dz), */
/* then starts the movement type facing names (fdActorLaunchFromLeader). */
static inline void scFieldActorLaunchFromLeader(s32 actorId, s32 dx, s32 dz,
												s32 facing) {
	fdActorLaunchFromLeader(actorId, dx, dz, facing);
}

/* Moves a point radius, in 16.16 fixed point, along a heading on the ground */
/* plane (syVectorAddPolarOffset). */
static inline void syFieldVectorAddPolarOffsetFX(FIXED radius, s32 heading,
												 FIXED *probe) {
	syVectorAddPolarOffset(radius, heading, probe);
}

/* Moves a point a radius in whole pixels along a heading: */
/* `syFieldVectorAddPolarOffset(16, heading, &probe)`. */
#define syFieldVectorAddPolarOffset(radius, heading, probe)                    \
	syFieldVectorAddPolarOffsetFX(toFIXED(radius), heading, probe)

/* Changes the cell behavior of the cell at (x, y), in 16.16 pixels, */
/* opening or closing the way (fdMapGridSetCellBehaviorAt). */
static inline void fdSetCellBehaviorAt(s32 layer, FIXED x, FIXED y,
									   s32 behavior) {
	fdMapGridSetCellBehaviorAt(layer, x, y, behavior);
}

/* Changes one 512-pixel tile of the World Map, drawn at once when it's next to
 */
/* the view (fdWorldMapSetWindowCellTile). */
static inline void fdSetWorldMapWindowCellTile(s32 x, s32 y, s32 cellX,
											   s32 cellY) {
	fdWorldMapSetWindowCellTile(x, y, cellX, cellY);
}

/* Holds the event for frames while the field goes on (fdEventWait). */
static inline void syFieldEventWait(s32 frames) { fdEventWait(frames); }

/* Turns an actor to a direction and holds the event until it has turned. */
static inline void fdActorFaceDirectionAndWait(s32 actorId, s32 facing) {
	fdActorFaceDirectionAndRefreshById(actorId, facing);
}

/* Places an actor at a point at once, as fdActorSetPos does, facing a */
/* direction. */
static inline void fdActorSetPosAndFacing(s32 actorId, FIXED x, FIXED z,
										  u16 facing) {
	fdActorSetHorizontalPosAndAngWithTerrain(actorId, x, z, facing);
}

/* fdActorSetAnimationAndWait as the events that take */
/* fdActorSetAnimationAndWait for a function returning int make the call. */
static inline void scFieldActorSetAnimationAndWait(s32 actorId, s32 animation) {
#ifdef UBFIX
	fdActorSetAnimationAndWait(actorId, animation);
#else
	/* UB: fdActorSetAnimationAndWait returns nothing, but these calls are */
	/* made as to a function returning int, which moves their argument
	 * registers. */
	((s32 (*)(u32, s32))fdActorSetAnimationAndWait)(actorId, animation);
#endif
}

/* Throws an actor upward at speed pixels a frame, then holds the event for */
/* frames. */
static inline void scFieldActorJump(s32 actorId, s32 speed, s32 frames) {
	fdActorJump(actorId, speed, frames);
}

/* Fixes the priority of an actor's sprite, which then no longer follows its */
/* depth. */
static inline void scFieldActorSetSprPriority(s32 actorId, s32 priority) {
	fdActorSetSprPriority(actorId, priority);
}

/* Squashes an actor and springs it back count times. */
static inline void scFieldActorStartRepeatedMotion(s32 actorId, s32 count) {
	fdActorStartRepeatedMotion(actorId, count);
}

/* Makes an actor follow another. */
static inline void scFieldActorWatchAndSetScript(s32 actorId,
												 s32 watchedActorId) {
	fdActorWatchAndSetScript(actorId, watchedActorId);
}

/* Holds the event until an actor arrives, 600 frames at most. */
static inline void scFieldActorWaitForMoveById(s32 actorId) {
	fdActorWaitForMoveById(actorId);
}

/* Ends Reveal, the ring of light and Sand, each unless skip holds its bit. */
static inline void syFieldEventEndLastingPsynergy(s32 skip) {
	fdEventEndLastingPsynergy(skip);
}

/* Makes message the event's next line. */
static inline void syFieldEventSetMessageId(s32 message) {
	fdEventSetMessageId(message);
}

/* Makes the camera follow an actor (fdCameraFollowActor). */
static inline void syEventCameraFollowActor(s32 actorId, s32 keepPosition) {
	fdCameraFollowActor(actorId, keepPosition);
}

/* Places an actor at (x, y, z), 16.16 pixels, and stops it. */
static inline void scFieldActorSetPosAndResetMotion(struct ACTOR *actor,
													FIXED x, FIXED y, FIXED z) {
	fdActorSetPosAndResetMotion(actor, x, y, z);
}

/* Holds the event until an actor's script ends, 300 frames at most. */
static inline void scFieldActorWaitForScriptEndById(s32 actorId) {
	fdActorWaitForScriptEndById(actorId);
}

/* Sets the colour mode of an actor's sprite (fdActorSetSprCol). */
static inline void scFieldActorSetSprCol(struct ACTOR *actor, s32 colorMode) {
	fdActorSetSprCol(actor, colorMode);
}

/* Stops an actor's movement type and stands it on the idle script. */
static inline void scFieldActorStop(s32 actorId) { fdActorStop(actorId); }

/* Requests the exit the field leaves the scene by. */
static inline void syFieldEventSetExitRequest(s32 exit) {
	syImportEventSetExitRequest(exit);
}

/* Holds the event until an actor has reached its destination */
/* (fdActorWaitForMove). */
static inline void scFieldActorWaitForMove(void *actor) {
	fdActorWaitForMove(actor);
}

/* Who carries an item, asking the leader first, or -1. */
static inline s32 syFieldPartyInventoryFindMember(s32 item) {
	return gmPartyInventoryFindMember(item);
}

/* Shows a message in a window in the middle of the screen */
/* (mnFieldMessageShowCentered). */
static inline void syEventMessageShowCentered(s32 message, s32 mode,
											  s32 yOffset) {
	mnFieldMessageShowCentered(message, mode, yOffset);
}

/* The leader's actor id. */
static inline s32 syFieldPartyGetLeaderActor(void) {
	return fdPartyGetLeaderActor();
}

/* Asks yes or no as the speaker nods or shakes its head; returns 0 for yes. */
static inline s32 syFieldEventAskYesNoWithActorAnimation(s32 actorId,
														 s32 flags) {
	return fdEventAskYesNoWithActorAnimation(actorId, flags);
}

/* Sets where the party goes after the event's battle */
/* (fdEventSetAfterBattleDestination). */
static inline void syFieldEventSetAfterBattleDestination(s32 sceneId,
														 s32 entrance) {
	fdEventSetAfterBattleDestination(sceneId, entrance);
}

/* Adds a Djinni to the party (fdDjinnJoinParty). */
static inline void syFieldDjinnJoinParty(s32 slot, s32 element, s32 djinn) {
	fdDjinnJoinParty(slot, element, djinn);
}

/* The actor the camera follows (fdCameraGetActor). */
static inline struct ACTOR *syEventCameraGetActor(void) {
	return fdCameraGetActor();
}

/* Asks the player yes or no (fdYesNoAsk). */
static inline s32 syEventYesNoAsk(s32 atBottom) { return fdYesNoAsk(atBottom); }

/* Zooms the World Map's view to zoom, 16.16, over frames. */
static inline void syFieldWorldMapStartZoom(FIXED zoom, s32 frames) {
	fdWorldMapStartZoom(zoom, frames);
}

/* Applies a window colour (mnWindowColApply). */
static inline void syFieldWindowColApply(s32 color, s32 brightness) {
	mnWindowColApply(color, brightness);
}

/* Requests a battle with a Djinni (fdRequestDjinnBattle). */
static inline void fdEventRequestDjinnBattle(s32 actorId, s32 djinni) {
	fdRequestDjinnBattle(actorId, djinni);
}

/* Adds unit to the party, announcing it when announce is nonzero. */
static inline void syFieldEventAddPartyMember(s32 unit, s32 announce) {
	fdEventAddPartyMember(unit, announce);
}

/* Holds the event until the screen shake dies down */
/* (fdMapGridWaitForShakeToSettle). */
static inline void syFieldMapGridWaitForShakeToSettle(void) {
	fdMapGridWaitForShakeToSettle();
}

/* The cell behavior of the cell at (x, y) (fdMapGridGetCellBehaviorAt). */
static inline s32 syFieldMapGridGetCellBehaviorAt(s32 layer, FIXED x, FIXED y) {
	return fdMapGridGetCellBehaviorAt(layer, x, y);
}

/* Draws an item's icon into an actor's sprite, or blanks it */
/* (fdActorShowItemIconById). */
static inline void scFieldActorShowItemIconById(s32 actorId, s32 item) {
	fdActorShowItemIconById(actorId, item);
}

/* Draws an item's icon into an actor's sprite (fdActorSetItemIcon). */
static inline void scFieldActorSetItemIcon(struct ACTOR *actor, s32 item) {
	fdActorSetItemIcon(actor, item);
}

/* Changes one tile of the World Map and draws it */
/* (fdWorldMapSetWindowCellTileAndDraw). */
static inline void
syFieldWorldMapSetWindowCellTileAndDraw(s32 x, s32 y, s32 cellX, s32 cellY) {
	fdWorldMapSetWindowCellTileAndDraw(x, y, cellX, cellY);
}

/* Moves the camera onto an actor, to its height too */
/* (fdCameraMoveToActor). */
static inline void syEventCameraMoveToActor(s32 actorId, s32 pan) {
	fdCameraMoveToActor(actorId, pan);
}

/* Rotates an actor's sprite (fdActorSetSprRotationById). */
static inline void scFieldActorSetSprRotationById(s32 actorId, s32 angle) {
	fdActorSetSprRotationById(actorId, angle);
}

/* Sets the cell behavior of a cell (fdMapGridSetCellBehavior). */
static inline s32 syFieldMapGridSetCellBehavior(s32 x, s32 z, s32 layer,
												s32 behavior) {
	return fdMapGridSetCellBehavior(x, z, layer, behavior);
}

/* Scoop digs up a buried treasure: with `mode` RETURN_HOME_DUG_UP, the */
/* treasure actor comes up at its home and its flag is set (fdActorReturnHome).
 */
static inline void fdEventDigUpTreasure(s32 mode, s32 actorId, s32 flag) {
	fdActorReturnHome(mode, actorId, flag);
}

#endif
