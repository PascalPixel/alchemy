#ifndef ALCHEMY_FIELD_SEQUENCE_FUNCTIONS_H
#define ALCHEMY_FIELD_SEQUENCE_FUNCTIONS_H

/* @file */
/* The field-sequence module's functions, declared once each in the order of */
/* its stub table, without the inlines field_sequence.h brings in with */
/* field.h and its other headers. It is for the units that must not see those */
/* inlines: field_effect_advance_motion_priority.c, field_effect_advance_spin.c
 */
/* and the scenes' shared scene_set_arrival_flags.c, which have it through */
/* event.h, and actor_scripts.c, cracked_tile_mask.c, field_effect_spawn.c and
 */
/* field_effect_spawn_resident.c. field_sequence.h includes it for everyone */
/* else. */

#include "types.h"
#include "fixed_math.h"
#include "field/field_particle.h"

struct ACTOR;
struct ACTOR_FLAG;
struct ACTOR_PLACE;
struct EFFECT_SPR;
struct FIELD_PARTICLE;
struct TRIGGER_VIEW;
struct VECTOR3;
struct VIEWED_PLACEMENT;
struct WORLD_MAP_PLACE;

/* The field-sequence module's functions in the order of its stub table */
/* (src/field/sequence_stubs.s). */
/* The field sequence: runs the current scene, which the party entered by */
/* entrance, until it leaves; returns the exit kind it left by, for */
/* scTakeExit, or EXIT_KEEP_DESTINATION when the next scene is already in */
/* MainRoutineStateWork: a random battle, a party that fell outside battle, */
/* Retreat or an event's destination. */
/* A unit whose ROM code is compiled against another form defines */
/* OWN_FieldSequence_Run and declares that form itself. */
#ifndef OWN_FieldSequence_Run
s32 fdSequenceRun(void);
#endif
/* The main routine: runs one sequence after another and never returns. mode, */
/* an GAME_START, chooses the first scene. As The Broken Seal's, with this
 */
/* game's four debug starts, rendering re-enabled after DMA 0 stops, and three
 */
/* minigames, Super Lucky Dice the third. */
void gmMainRoutineRun(s32 mode);
/* Waits the requested frames through sySynch(), unless debug fast-forward */
/* (fdEventUpdateDebugSkip) is on: the wait events use. */
void fdEventWait(s32 frames);
/* Begins an event: closes any area name window, stops the leader, ends the */
/* lasting Psynergy if a trigger is running, resets the message and timing */
/* state, and starts the debug fast-forward task with the leader as the */
/* message speaker. */
void fdEventStart(void);
/* Ends an event: stops the debug fast-forward task, gives the camera back to */
/* the leader (or to the first placed actor, the ship, while */
/* MainRoutineStateWork.ownerOverride is set) and refreshes the Lure Cap's flag
 */
/* (gmFlagRefreshLureCap). */
/* A unit whose ROM code is compiled against another form defines OWN_Event_End
 */
/* and declares that form itself. */
#ifndef OWN_Event_End
void fdEventEnd(void);
#endif
/* Rebuilds the leader's actor from its placement row (fdActorResetForLeader),
 */
/* waits a frame and returns the leader's actor. */
struct ACTOR *fdEventResetLeaderAndWait(s32 table);
/* Spawns the actors of an actor placement table after the last actor in use. */
void fdEventSpawnActorsAfterLast(s32 table);
/* Gives each actor placement with condition 0 a condition saying whether */
/* it lies within the map's view bounds, so that only those inside appear. */
void scAssignViewFlags(struct VIEWED_PLACEMENT *entry);
/* Replaces the scene's trigger table with events. */
void fdEventSetSceneEvents(s32 events);
/* Adds unit to the party. With announce nonzero it also shows `<name> joined */
/* your party.` (mnPartyShowJoinedMessage, which receives announce too). */
void fdEventAddPartyMember(s32 unit, s32 announce);
/* Adds two units to the party and announces them together ("<a> and <b> */
/* joined your party.", mnPartyShowPairJoinedMessage). */
void fdEventAddTwoPartyMembers(s32 first, s32 second);
/* Takes unit out of the party, fully restoring its HP and PP and curing its */
/* poison. */
void fdPartyRemoveMemberRestored(s32 unit);
/* The event command "give item": gives item to the first party member with */
/* room for it and says so; with no room, the player chooses what to drop. */
s32 fdPartyInventoryGiveItem(s32 item, s32 unused);
/* Gives item to one party member. Returns unit, or -1 if its bag is full. */
s32 fdInventoryTryAddAndReturnMember(s32 item, s32 unused, s32 unit);
/* The event command "choose yes or no": once the message on screen has been */
/* read, asks yes or no below it, or above it when the message windows reach */
/* past YES_NO_BELOW_LAST_ROW (unless forceBottom is set). The speaker nods */
/* (ANIM_NOD) at yes and shakes its head (ANIM_SHAKE_HEAD) at no; at the */
/* fastest message speed neither is waited for. */
s32 fdEventAskYesNoWithActorAnimation(s32 actor, s32 forceBottom);
/* Gives the party item from actor: with no room it shows the item, says it */
/* was found and left behind and returns -1; otherwise it adds the item, sets */
/* flag (unless -1) and returns 0. */
s32 fdEventPickUpItem(s32 actor, s32 item, s32 flag);
/* Teaches unit the Psynergy ability and says so with its jingle, waiting for */
/* the player to read it. */
void fdPartyLearnAbility(s32 unit, s32 ability);
/* Returns an actor, or NULL if it does not exist. */
struct ACTOR *fdActorGetById(u32 actorId);
/* The event command "release actor": releases the actor and empties its slot */
/* in the field state's actor table. */
void fdActorReleaseById(s32 actorId);
/* The event command "set actor speed": its top speed and acceleration, 16.16 */
/* pixels per frame. Writes the actor's speedLimit and acceleration. */
void fdActorSetSpeedParms(s32 actorId, FIXED speedLimit, FIXED acceleration);
/* Gives an actor a movement type or actor script (fdActorSetMovementType) */
/* and makes it face where it moves (ACTOR_FACES_MOTION). */
void fdActorSetMovementTypeById(s32 actorId, const void *script);
/* Waits, at most 300 frames, for an actor's script to reach its end (SC_END).
 */
void fdActorWaitForScriptEndById(u32 actorId);
/* The event command "stop actor": stops its movement type and puts it back */
/* under script control, on the idle script (a lone SC_END), and makes it face
 */
/* where it moves. */
void fdActorStop(s32 actorId);
/* Starts a movement type or supplied actor script, as */
/* fdActorSetMovementTypeById does, then waits up to 300 frames for its script
 */
/* to end. */
void fdActorSetMovementTypeAndWaitById(s32 actorId, const void *script);
/* The event command "set actor destination": the actor heads for (x, z), in */
/* pixels, keeping its current animation. */
void fdActorSetDestinationById(u32 actorId, s32 x, s32 z);
/* The event command "move actor and wait": as fdActorSetDestinationById, then
 */
/* waits (at most 600 frames) for the actor to arrive. */
void fdActorMoveToAndWaitById(u32 actorId, s32 x, s32 z);
/* The event command "walk actor to": the actor walks toward (x, z), in pixels.
 */
void fdActorWalkToById(s32 actorId, s32 x, s32 z);
/* The event command "walk actor to and wait": walks the actor to (x, z), in */
/* pixels, waits for it to arrive and stands it still. */
void fdActorWalkToAndWaitById(s32 actorId, s32 x, s32 z);
/* The event command "center actor and walk": walks the actor sideways to the */
/* middle of its 16-pixel column, sets its sprite priority to priority */
/* (fdActorSetSprPriority), then sends it zOffset pixels along z. */
void fdActorCenterOnColumn(u32 actorId, s32 priority, s32 zOffset);
/* The event command "set actor destination offset": the actor heads (xOffset,
 */
/* zOffset) pixels from where it stands. */
void fdActorSetDestinationOffsetById(s32 actorId, s32 xOffset, s32 zOffset);
/* The event command "wait for actor": waits (at most 600 frames) for the actor
 */
/* to arrive and stands it still. */
void fdActorWaitForMoveById(s32 actorId);
/* The event command "set actor position": puts the actor at (x, z), 16.16 */
/* pixels, at rest; an actor that follows the ground keeps its height above */
/* it. */
void fdActorSetHorizontalPosWithTerrain(u32 actorId, FIXED x, FIXED z);
/* As fdActorSetHorizontalPosWithTerrain, and turns the actor to angle. */
/* Written in the old style: the angle arrives as a word and is taken as a */
/* halfword. */
void fdActorSetHorizontalPosAndAngWithTerrain(s32 actorId, s32 x, s32 z,
											  s32 angle);
/* Puts the actor at (x, y, z), 16.16 pixels, at rest, raised by the ground */
/* height there if it follows the ground. */
void fdActorSetPosWithTerrain(u32 actorId, FIXED x, FIXED y, FIXED z);
/* Puts the first actor where the second stands, at rest, on the same ground */
/* height. */
void fdActorPlaceOnActor(u32 actorId, u32 otherId);
/* The event command "set actor animation" (an ACTOR_ANIMATION: ANIM_STAND,
 */
/* ANIM_WALK, ANIM_NOD, ANIM_SHAKE_HEAD and so on). */
void fdActorSetAnimationById(u32 actorId, s32 animation);
/* Sets the animation timer steps of each of an actor's sprites. */
void fdActorSetAnimationSpeedById(s32 actorId, s32 steps);
/* The event command "set actor animation and wait": plays the animation and */
/* waits for it to end. */
void fdActorSetAnimationAndWait(s32 actorId, s32 animation);
/* Waits, at most 90 frames, for an actor's animation to change or for its */
/* first sprite animation's script to reach its hold command (ANIMCMD_HOLD). */
/* Only an actor drawn as a sprite (ACTOR_SPRITE_ONE) is waited for. */
void fdActorWaitForAnimationChange(u32 actorId);
/* The event command "actor jump": throws the actor upward at speed pixels per
 */
/* frame with a jump sound (a higher one above 5), then waits frames frames. */
void fdActorJump(s32 actorId, s32 speed, s32 frames);
/* The event command "start repeated motion": the actor squashes and springs */
/* back count times (1 to REPEATED_MOTION_MAX), through Actor_VariantScripts. */
void fdActorStartRepeatedMotion(u32 actorId, s32 count);
/* The event command "run repeated motion": as fdActorStartRepeatedMotion, then
 */
/* waits for it to end. */
void fdActorRunRepeatedMotion(u32 actorId, s32 count);
/* Makes an actor jump down off a ledge in front of it: it shows pose for 10 */
/* frames, hops 12 pixels south from the middle of its 16-pixel column, turns */
/* toward FACING_SOUTH_SOUTHWEST while sinking 28 pixels with a dust puff every
 */
/* other frame in the colour mode dust (none when dust is -1), then jumps on to
 */
/* 16 pixels south of where it started and lands, waiting at most 180 frames */
/* for it unless dust has LEDGE_JUMP_NO_LANDING_WAIT. */
void fdActorJumpDownLedge(s32 actorId, s32 pose, s32 dust);
/* The event command "actor face actor": turns the first actor at once to face
 */
/* the second, then waits the given number of frames. */
void fdActorFaceActorById(s32 firstId, s32 secondId, s32 frames);
/* The event command "actors face each other": turns two actors toward each */
/* other, then waits the given number of frames. */
void fdActorTurnToFaceEachOther(s32 firstId, s32 secondId, s32 frames);
/* The event command "set actor sprite colour": gives every sprite animation of
 */
/* the actor the colour mode (fdActorSetSprCol); with SPRITE_COLOR_BLINK */
/* the actor blinks instead (fdActorUpdateBlink). */
void fdActorConfigureSprCol(s32 actorId, s32 colorMode);
/* Sets the colour mode (SPR_COL_MODE) of every running sprite animation of
 */
/* an actor's sprite. Events set SPRITE_COLOR_NORMAL or SPRITE_COLOR_HIDDEN, */
/* and alternate the two to make an actor flash. */
void fdActorSetSprCol(struct ACTOR *actor, s32 colorMode);
/* Makes an actor follow the actor in targetId's ACTOR_WATCH_ACTOR_MASK bits */
/* and starts script on it. Unless targetId has ACTOR_WATCH_KEEP_MOTION it */
/* keeps FOLLOW_DISTANCE, takes the target's top speed with twice its */
/* acceleration, to keep up with it, and no collision flags, so it cannot block
 */
/* the target. */
void fdActorWatchAndStartScript(u32 actorId, s32 targetId, const void *script);
/* The event command "set message": the number of the event's next message, */
/* written to the field state's message. */
void fdEventSetMessageId(s32 message);
/* The event command "open message": opens the event's current message in a */
/* speech window next to the speaking actor, with the speaker's portrait beside
 */
/* it when it has one, and moves on to the next message. */
s32 fdMessageShowAtActor(s32 speaker, s32 unused);
/* Opens the event's current message at the speaker and waits for its window; */
/* after ten seconds, or with Select, R, L and A held, the text is finished at
 */
/* once. */
void fdMessageShowAtActorAndWait(s32 speaker, s32 flags);
/* The event command "show message and wait": shows the message, then waits the
 */
/* given number of frames. */
void fdEventShowMessageThenWait(s32 speaker, s32 flags, s32 frames);
/* The event command "show message": opens the current message at the speaker */
/* (fdMessageShowAtActor) and waits until it has been read and its window */
/* has closed; the speaker's portrait is closed once the text is typed out. */
void fdEventShowMessageAndWait(s32 speaker, s32 flags);
/* The event command "ask yes or no": opens the question at the speaker, lets */
/* the leader answer, then shows the next message for yes or the one after it */
/* for no. */
s32 fdEventAskYesNoAndShowResponse(s32 speaker, s32 flags);
/* An empty hook; it does nothing in the released game. */
void fdEventNoOpUpdate(void);
/* Unused: nothing calls it. It would record the speaker, open the message near
 */
/* it and wait unless fast-forwarding, then advance the message index. It */
/* places the message as fdEventShowMessageAtPos does, before passing the */
/* position to a window function that takes tiles; with no caller, the units it
 */
/* expected can't be told. */
void fdEventShowSpeakerMessage(s32 speaker);
/* Unused: nothing calls it; forwards its arguments to */
/* fdEventShowMessageAtPos. */
void fdEventForwardShowMessageAtPos(s32 unusedA, s32 unusedB, s32 x, s32 y);
/* Unused: nothing calls it. It would open the event's message at (x, y), wait
 */
/* for it and advance the message index; its units are as unknown as */
/* fdEventShowSpeakerMessage's. */
void fdEventShowMessageAtPos(s32 unusedA, s32 unusedB, s32 x, s32 y);
/* The event command "actor face direction": the actor turns toward angle */
/* (TRIG_TURN a full turn; fdActorStepAng), then the event waits the given */
/* number of frames. */
void fdActorFaceDirectionById(s32 actorId, s32 angle, s32 frames);
/* Turns an actor a step toward angle (Actor_StepAngleScript) and waits for */
/* the script to end. */
void fdActorFaceDirectionAndRefreshById(s32 actorId, s32 angle);
/* Clears an actor's destination, starts its idle script and stands it */
/* still. */
void fdActorClearDestinationAndStand(u32 actorId);
/* Closes the portrait of an actor's sprite, if it has one. */
void fdActorClosePortrait(s32 actorId);
/* The event command "show two messages and wait": two actors speak at once, */
/* each message at its own (x, y) with its portrait at (portraitX, */
/* portraitY), all in tiles. When delay is nonzero, the second message waits */
/* for the first to finish, checking every delay frames. Waits for A, B, L or */
/* R, then closes both. */
void fdEventShowTwoMessagesAndWait(s32 first, s32 firstX, s32 firstY,
								   s32 firstPortraitX, s32 firstPortraitY,
								   s32 second, s32 secondX, s32 secondY,
								   s32 secondPortraitX, s32 secondPortraitY,
								   s32 delay);
/* A message's voice from its speaker, and a message shown at a place. */
void fdMessageSetVoiceFromActor(s32 key);
/* The event command "set actor sprite priority": fixes the priority of the */
/* actor's sprite and its shadow, which then no longer follow its depth: */
/* clears ACTOR_PRIORITY_AUTOMATIC. */
void fdActorSetSprPriority(u32 actorId, s32 priority);
/* Stops actor id moving and colliding and hides it. */
void fdActorDisableById(u32 id);
/* The event command "actor show emote": shows an emote bubble (SPRITE_EMOTE) */
/* over an actor, then waits the given number of frames. Returns the bubble. */
struct ACTOR *fdActorShowEmoteById(s32 actorId, s32 emote, s32 frames);
/* The event command "actor set attached effect", by ATTACHED_EFFECT_FLAG: */
/* removes the actor attached to an actor, attaches one */
/* (SPRITE_RAY_ARC, animation 1) unless one already is, or shows a */
/* one-off one (animation 2) that ends by itself (Actor_WaitScript). */
void fdEffectSetAttached(s32 actorId, s32 flags);
/* Makes the target actor's sprite show the source actor's graphics (its VRAM */
/* block and tile). Neither actor is checked for NULL. */
void fdActorCopyGraphics(u32 targetId, u32 sourceId);
/* The event command "camera follow actor": the camera follows the actor, put */
/* on it at once unless keepPosition leaves it where it is to set out from. */
void fdCameraFollowActor(s32 actorId, s32 keepPosition);
/* The event command "camera set speed": the camera's top speed and */
/* acceleration, 16.16 pixels per frame. */
void fdCameraSetSpeedParms(FIXED speed, FIXED acceleration);
/* The event command "camera move to": sends the camera to the requested axes,
 */
/* 16.16 pixels (-1 keeps that axis), kept far enough inside the map that the */
/* screen never shows past its edge. With pan 0 the camera cuts there at once;
 */
/* otherwise it pans there at its own speed. */
void fdCameraMoveWithinMapBounds(FIXED requestedX, FIXED requestedY,
								 FIXED requestedZ, s32 pan);
/* The event command "camera wait for move": waits (at most 600 frames) for */
/* the camera to arrive, then 2 frames more. */
void fdCameraWaitForArrival(void);
/* The event command "camera move to actor": fdCameraMoveWithinMapBounds to */
/* the actor's x and z. */
void fdCameraMoveToActorById(s32 actorId, s32 pan);
/* As fdCameraMoveToActorById, to the actor's height too. */
void fdCameraMoveToActor(s32 actorId, s32 pan);
/* The event command "get camera": returns the camera's own actor, which the */
/* view centres on. */
struct ACTOR *fdCameraGetActor(void);
/* Sets the map coordinates the camera keeps the screen inside. */
void fdMapGridSetCameraBounds(s32 minX, s32 minZ, s32 maxX, s32 maxZ);
/* Makes the event leave for the given entrance of the given scene: raises the
 */
/* exit request EXIT_KEEP_DESTINATION and writes MainRoutineStateWork.next and
 */
/* entrance. */
void fdEventSetDestination(s32 sceneId, s32 entrance);
/* Makes the event leave and records sceneId and entrance as the restart */
/* point. */
void fdEventSetRestartPoint(s32 sceneId, s32 entrance);
/* The event command "request exit": sets the field's exit request */
/* (EXIT_KEEP_DESTINATION leaves for MainRoutineStateWork's next and entrance).
 */
void fdEventSetExitRequest(u16 exit);
/* Sets up a battle with the formation at row and column of the encounter */
/* table. */
void fdRequestBattleFromEncounterTable(s32 row, s32 column);
/* Sets up the battle with Djinni djinn, DJINN_PER_ELEMENT to an element; an */
/* element's last Djinni stands for the one of The Broken Seal's chosen for it
 */
/* (SCENE_STATE_CHOSEN_DJINN; none: nothing happens). The low 8 bits of flags */
/* are the Djinni's actor, which hops first unless DJINN_NO_HOP is set. */
void fdRequestDjinnBattle(u32 flags, u32 djinn);
/* Sets the after-battle destination (MainRoutineStateWork.afterBattleScene and
 */
/* afterBattleEntrance), where the party goes once the coming battle is won. */
void fdEventSetAfterBattleDestination(s32 sceneId, s32 entrance);
/* Sets the defeat destination (MainRoutineStateWork.defeatScene and */
/* defeatEntrance), where the party goes if it loses the coming battle. */
void fdEventSetDefeatDestination(s32 sceneId, s32 entrance);
/* Makes the event leave for entrance entrance of scene sceneId: sets */
/* MainRoutineStateWork.next and entrance and raises the exit request */
/* EXIT_KEEP_DESTINATION, so the field sequence leaves on its next pass. */
void fdEventLeaveFor(u16 sceneId, u16 entrance);
/* Writes the actor's position on screen, in pixels, to position[0] and */
/* position[1]; an actor with a sprite is raised by its frame box. Returns -1 */
/* when the actor does not exist. */
s32 fdActorGetScrPos(s32 actorId, s32 *position);
/* Returns the cell behavior of the map cell 16 pixels ahead of the party */
/* leader when there is something to search there: a fixed search spot */
/* (CELL_BEHAVIOR_JAR to CELL_BEHAVIOR_WELL) whose ground is level with the */
/* leader or up to 16 pixels above it, or a behavior the scene has an examine */
/* trigger for; aboard the ship on the World Map, SHIP_SEA_EXAMINE_CODE before
 */
/* sea of collision 3. Returns 0 otherwise. */
s32 fdActorGetTriggerCellAheadOfCurrent(void);
/* Picks the sprite a scene placement's character id shows: the ship */
/* (PLACEMENT_SHIP) is SPRITE_WINGED_SHIP_SMALL on the world map and */
/* SPRITE_WINGED_SHIP elsewhere, each one's flying picture once FLAG_SHIP_FLIES
 */
/* is set. */
s32 fdPartyGetCharacterSpr(s32 character);
/* Picks the actor nearest actor id, under 32 pixels away and 48 pixels in */
/* height. Distances to treasure spots count double and to actors with */
/* ACTOR_NEAR_TALK ten thirteenths; beyond 11 pixels the actor must also lie */
/* within three sixteenths of a turn of id's facing. Returns -1 when none */
/* qualifies. */
s32 fdFindNearbyTalkTarget(s32 id);
/* When Move is cast on a target, the pushing hand appears beside it and pushes
 */
/* it 16 pixels the way the d-pad points, for as long as the player pushes; A,
 */
/* B, L or R end it. */
void fdPsynergySteerPushedTarget(s32 unused);
/* Cloak the caster: flicker it CLOAK_BLINKS times, leave it half transparent,
 */
/* then check the ground under it every frame. When it cannot hide where it */
/* stands, show it again at once. */
void fdPsynergyStartCloak(void);
/* Returns the actor nearest in front of actor self that field Psynergy kind */
/* can reach, or -1. */
s32 fdFindNearbyPsynergyTarget(s32 self, s32 kind);
/* The leader jumps two cells forward, over the cell in front, when it faces */
/* a quarter turn, the map allows it (fdMapGridBlocksJump and the step check) */
/* or a platform actor (ACTOR_PRIORITY_PLATFORM) waits 16 pixels below the */
/* landing, and no solid actor stands there. A log underfoot (a */
/* SPRITE_LOG_WEST_EAST or SPRITE_LOG_NORTH_SOUTH under the party) dips 2 */
/* pixels with the leader. Each jump adds to the walking distance, and the */
/* leader keeps jumping while it lands on CELL_BEHAVIOR_JUMP_FORWARD. Returns */
/* 0 after a jump, -1 when the leader cannot jump. */
s32 fdTryJumpForward(void);
/* Climbs down onto the wall below the leader, as The Broken Seal's */
/* fdClimbDown does, or lets go and falls when it cannot. */
s32 fdClimbDown(void);
/* Climbs up the wall in front of the leader, as The Broken Seal's */
/* fdClimbUp does. */
s32 fdClimbUp(void);
/* Stops the leader where it stands: no horizontal velocity, the normal walking
 */
/* speed (1.0) and acceleration (0.5), no destination, and the standing pose, */
/* or ANIM_LEADER_CLING while the leader is on a climbable wall. */
void fdLeaderStopMoving(void);
/* Unused: nothing calls fdGroundParticlesStartStub. The Broken Seal's rain, */
/* kept whole where this game has its own fdWeatherStartRain: 64 sprites placed
 */
/* at the leader's position on the terrain, their tiles decoded into VRAM, the
 */
/* blend set, and fdGroundParticlesUpdate scheduled to move them. */
void fdGroundParticlesStart(void);
/* Starts the storm's lightning (fdWeatherUpdateLightning): builds the dark */
/* (PALETTE_TINT_WARM) and lit (PALETTE_TINT_PALE) BG colours and their 12-step
 */
/* fade, and arms the first burst to end in 600 frames, repeating. */
void fdWeatherStartLightning(void);
/* Stops the lightning: no flash for 32767 frames, and none after. */
void fdWeatherStopLightning(void);
/* One last burst of lightning, its first flash two frames from now (timer 12;
 */
/* fdWeatherUpdateLightning flashes at 11), then no more. */
void fdWeatherLastLightning(void);
/* Restarts the lightning: the next burst ends in 128 frames, and bursts */
/* repeat. */
void fdWeatherResumeLightning(void);
/* Flashes the palette with the storm's task: one burst of three flashes of */
/* palette to, each fading back to palette from (fdPaletteFadeBuild modes) on */
/* the BG colours, ending 120 frames from now, and no more after. */
void fdWeatherStartLightningFlash(s32 from, s32 to);
/* Starts the rising particles, as the Northern Reaches does: allocates and */
/* clears heap slot */
/* HEAP_SLOT_WEATHER, decodes the particle tiles into VRAM, sets each of the 64
 */
/* particles' sprite words and places it on the ground at a random point of the
 */
/* strip (x 288 to 488 pixels, z 60 to 80), staggering their timers by 2 */
/* frames, and schedules the update. */
void fdRisingParticlesStart(void);
/* Unused: nothing calls fdEffectInitSparklesStub; Aqua Rock and the */
/* Apojii Islands keep their own copy (shared/ground_particles_start.h). Starts
 */
/* the sparkles (fdEffectUpdateSparkles): loads their tiles and lays every */
/* sparkle at the map's top left corner with a staggered timer. */
void fdEffectInitSparkles(void);
/* Starts window transition mode at value 0, with no duration: the caller moves
 */
/* it. */
void fdScrTransitionStartWindowHeld(s32 mode);
/* Starts a dissolve of the given mode, its value held at 0, and waits two */
/* seconds. */
void fdScrTransitionStartDissolveHeld(s32 mode);
/* Fades the screen in over frames frames. mode is a SCENE_TRANSITION: its */
/* high byte an SCR_TRANSITION_STYLE (from black, from the backdrop */
/* colour, a window transition whose low byte is an WINDOW_TRANSITION_MODE,
 */
/* a dissolve, or opening from the centre line, which a non-zero low byte makes
 */
/* flicker). */
void fdScrTransitionStart(s32 mode, s32 frames);
/* Fades the screen out over frames frames, as fdScrTransitionStart fades it */
/* in. */
void fdScrTransitionFinish(s32 mode, s32 frames);
/* Clears the map's display-control bits for backgrounds 3, 2 and 1 */
/* (DISPCNT_BG3_ON, DISPCNT_BG2_ON and DISPCNT_BG1_ON) for each argument that */
/* is set. */
void fdScrTransitionHideBackgrounds(s32 clearBg3, s32 clearBg2, s32 clearBg1);
/* Talks to actor id. A container (actors from FIELD_TREASURE_ACTOR, and the */
/* Scoop spot, SCOOPED_ITEM_ACTOR) is checked instead: a chest gives what it */
/* holds, a Mimic starts its battle, a Psynergy Stone restores the party's PP */
/* and a stone tablet teaches its summon. Any other actor shows its event's */
/* message or calls its event function, stopped and turned as its placement */
/* says; the actor Halt has stopped answers its TRIGGER_TALK_HALTED trigger */
/* first. */
s32 scTalkToActor(s32 id);
/* The palette fades: their steps, their channels kept in range, and the */
/* backdrop colour. */
void fdPaletteFadeInit(void);
/* Sets the colours the palette fade moves toward (see fdPaletteFadeBuild); */
/* does nothing without a palette fade. */
void fdPaletteFadeSetTarget(s32 mode, s32 half);
/* Sets the colours the palette fade shows now (see fdPaletteFadeBuild). */
void fdPaletteFadeSetSource(s32 mode, s32 half);
/* Sets each colour channel of step to the amount to add each frame for source
 */
/* to reach target in frames frames; with frames 0 or less step is left as it */
/* was. */
void fdPaletteFadeCalcSteps(s16 *source, s16 *target, s16 *step, s32 frames);
/* Starts the fade from the shown colours to the target colours, over frames */
/* frames. */
void fdPaletteFadeStartInterpolation(s32 frames);
/* Stops the palette fade and frees it. */
void fdPaletteFadeRelease(void);
/* Sets the backdrop colour (BG colour 0) of the palette fade's copy of the */
/* palettes, from which it builds its colours. */
void fdPaletteFadeSetBackdrop(u32 color);
/* The event command "open screen": starts the event's screen transition and */
/* sets transitionActive. */
void fdEventStartScrTransition(void);
/* The event command "close screen": finishes the event's screen transition */
/* and clears transitionActive. */
void fdEventFinishScrTransition(void);
/* The event command "wait for screen": waits as long as the transition runs */
/* (transitionFrm). */
void fdEventWaitTransitionFrames(void);
/* Starts the line scroll: its state (heap slot HEAP_SLOT_LINE_SCROLL) holds */
/* the mode and, for x and y, how fast the wave runs from frame to frame, its */
/* step from one scanline to the next and its amplitude; two tasks, one in the
 */
/* vertical blank pass and one in the frame pass, build and send each frame's */
/* line offsets. */
void fdLineScrollInitWave(s32 mode, s32 freqX, s32 stepX, s32 ampX, s32 freqY,
						  s32 stepY, s32 ampY);
/* Stops the line scroll: removes its two tasks and stops DMA 0. */
void fdLineScrollStop(void);
/* Unused: only the stub table refers to it. Opens a chest: if actor target */
/* exists, plays SE_CHEST_OPEN, opens its lid and, 12 frames later, */
/* shows itemId in front of actor actorId with sparkles. */
s32 fdChestOpenAndShowItem(s32 actorId, s32 target, s32 itemId);
/* Shows a found item's picture as flags choose (ITEM_DISPLAY_FLAG), with waits
 */
/* of 60 and 80 frames around its raising when both are set. */
void fdItemDispShowPicture(struct ACTOR *actor, s32 flags);
/* Shows the picture of item 36 pixels above the party leader for 80 frames, */
/* as flags choose (ITEM_DISPLAY_FLAG). */
void fdItemDispShowItem(s32 item, s32 flags);
/* Returns the actor, or NULL. */
struct ACTOR *fdTreasureSpotGetActor(u32 actorId);
/* Opens the chest of the actor with SE_CHEST_OPEN, and waits 12 frames. */
void fdTreasureSpotOpenChest(u32 actorId);
/* Closes the chest of the actor again (its item could not be carried), with */
/* SE_CHEST_CLOSE, and waits 12 frames. */
void fdTreasureSpotCloseChest(u32 actorId);
/* Shows the chest of the actor open, with no sound. */
void fdTreasureSpotShowOpened(u32 actorId);
/* Moves the actor to (x, z) on the ground, 16.16 pixels; -1 keeps its home */
/* (homeX, homeZ) on that axis. */
void fdTreasureSpotSetActorPos(u32 actorId, FIXED x, FIXED z);
/* Releases the actor and empties its slot. */
void fdTreasureSpotRemove(u32 actorId);
/* Unused: only the stub table refers to it. Keeps the party leader off the */
/* treasure spots: a leader within 16 pixels of a treasure spot's actor is */
/* moved 20 pixels from it, in the direction it stood, and loses its move */
/* target. */
void fdTreasureSpotMoveLeaderAway(void);
/* TREASURE_SPOT_PUT_AWAY moves the actor to x and z 0 and marks it with */
/* sceneWord TREASURE_SPOT_ON_CELL; any other mode draws its sprite wide, 16 by
 */
/* 8 pixels, 4 pixels lower. */
void fdActorSetWideSpr(u32 actorId, s32 mode);
/* RETURN_HOME_CLEAR_Z moves the actor to z 0; RETURN_HOME_DUG_UP sets flag, */
/* draws a moved actor square again and drops it back home (homeX, homeZ) at */
/* Scoop's anchor. */
void fdActorReturnHome(s32 mode, u32 actorId, s32 flag);
/* On the World Map, with no area graphics already pending: marks them */
/* pending, pauses the camera's actor and, when a screen transition is */
/* running, ends it at once and waits 2 frames; then shows the still image */
/* (scWorldMapStartStillImageStub). */
void fdWorldMapFreeze(void);
/* On the World Map, loads the area graphics fdWorldMapFreeze left pending and
 */
/* lets the camera's actor move again. */
void fdWorldMapReloadPendingAreaGraphics(void);
/* Creates a scene's actors on entering it, as The Broken Seal's */
/* fdActorResetForLeader does: the party leader where the main routine's state
 */
/* puts it, the scene's placement table from actor slot 8, and the camera */
/* that follows the leader; it also chooses the leader's control mode. */
void fdActorResetForLeader(s32 table);
/* Creates the actors of a scene's actor placement table (ended by sprite -1),
 */
/* and records the table among the scene's tables. */
void fdEventSpawnActorTable(struct ACTOR_PLACE *entry, s32 slot);
/* Releases the actor in slot actorIndex of the field state's table, if there */
/* is one, and empties the slot. */
void fdActorReleaseAtIndex(u32 actorIndex);
/* Returns the actor in slot actorId (below LOOKUP_ID_COUNT), or NULL. */
struct ACTOR *fdActorLookup(u32 actorId);
/* Looks point id up in the map's scene points (its scene_points.tsv, ended by
 */
/* id 0xff) and writes its x and z, in 16.16 pixels, to position; returns 0, */
/* or -1 when the map has no such point. */
s32 fdScenePointGetPos(s32 id, FIXED *position);
/* Starts the fade overlay on an actor (the party leader when NULL): loads its
 */
/* solid tiles, sets the blending and draws it each frame. Unused: nothing */
/* calls it or its stub call, fdEffectStartFadeOverlayStub. */
void fdEffectStartFadeOverlay(void *actor);
/* Stops drawing the fade overlay, keeping its state. */
void fdFadeOverlaySuspend(void);
/* Draws the fade overlay again after fdFadeOverlaySuspend. */
void fdFadeOverlayResume(void);
/* Like fdEffectStartFadeOverlay with blank tiles and no blending: */
/* loads 512 bytes of blank tiles into VRAM_CACHE_ENTRY_FADE_OVERLAY and starts
 */
/* fdBlankOverlayDrawMarks on an actor (the party leader when 0). Aqua Rock's */
/* rooms start it; its marks are all colour 0, so nothing shows. */
void fdEffectStartBlankOverlay(s32 actor);
/* Suspends the fdBlankOverlayDrawMarks overlay task. */
void fdBlankOverlaySuspend(void);
/* Resumes the fdBlankOverlayDrawMarks overlay task. */
void fdBlankOverlayResume(void);
/* Runs one step of the scene's field Psynergy: cast with no target, cast on */
/* the chosen target (secondActorId), Mind Read's end, or what a lasting */
/* Psynergy does when it ends. */
void fdPsynergyRunHandler(s32 kind);
/* Prepares the state of the field Psynergy for ability (heap slot */
/* fieldEffState): its kind, the free VRAM it starts with, the map's shake */
/* to restore, and whether the place is lit. While another field Psynergy is */
/* resolving, only the ability and kind are set. */
void fdPsynergyLoadResources(s32 abilityId, s32 mode);
/* Records the caster and the target of a field Psynergy. */
void fdPsynergySetupActorPair(s32 userId, s32 targetId);
/* Takes the caster out of its casting update and waits a frame. */
void fdBattleEffectStopMainActor(void);
/* Ends a field Psynergy: frees the field effect sprites still in use and, */
/* unless another is being resolved, waits for the swelling arcs to end, */
/* restores the map's shake and releases the state. */
void fdPsynergyEnd(void);
/* The field Psynergy, each cast as the event commands run it, with the */
/* functions that end a lasting one, act on a target or resume it. */
void fdPsynergyStartCast(void);
/* Tints the field for a Psynergy animation: a tint chosen by the frame */
/* counter (so each cast looks a little different), fixed for some */
/* animations, and forced by the tint flags; then fades towards it over eight */
/* frames. The palettes are copied by fdStartColTransition. */
void fdPsynergyFadeToCastCol(void);
/* Fades the palettes back from a cast's colours to the saved ones over 8 */
/* frames; while Reveal is active the BG half fades to Reveal's grey */
/* instead (PALETTE_TINT_GRAYSCALE). */
void fdPsynergyFadeFromCastCol(void);
/* Ends every lasting Psynergy (fdEventEndLastingPsynergy with none skipped). */
void fdEventEndAllLastingPsynergy(void);
/* Ends the lasting Psynergy and runs their end events: Reveal, the ring of */
/* light and Sand (while the party is sand, MainRoutineStateWork.sandActive), */
/* each unless skip holds its bit (PSYNERGY_END_SKIP). */
void fdEventEndLastingPsynergy(s32 skip);
/* Starts Mind Read's wave between the caster and the one it reads */
/* (HEAP_SLOT_MIND_READ): a hue taken from the frame count, the field faded */
/* toward it, the message window greyed for thoughts, and */
/* fdPsynergyUpdateMindReadWave run each frame. */
void fdPsynergyStartMindReadWave(s32 casterId, s32 targetId);
/* Stops the wave effect: restores the caster's colours and the palettes and */
/* releases the Psynergy's state. */
void fdPsynergyStopMindReadWave(void);
/* Greys the message window's white and most of its other colours */
/* (MIND_READ_GREY), so messages read as thoughts, and starts the BG0 line */
/* scroll (fdMindReadStartRipple) at each VBlank. */
void fdPsynergyGreyMindReadPalette(void);
/* Stops the BG0 line scroll and restores WINDOW_PALETTE. */
void fdPsynergyRestoreMindReadPalette(void);
/* Returns whether the scene has a trigger for the field Psynergy kind on what
 */
/* it would act on (scSelectPsynergyTarget), at any stage. */
s32 scHasPsynergyTrigger(s32 fieldPsynergy);
/* Returns whether the scene has an event for item and what the leader faces. */
u32 scHasItemTrigger(u16 item);
/* Makes sourceSprite share sharedSprite's VRAM block, releasing its own; with
 */
/* no sharedSprite it keeps its block. */
void *fdSprShareVramCacheEntry(void *sourceSprite, void *sharedSprite);
/* Returns the battle background chosen for the current scene. */
s16 fdBattleBackgroundGetSelected(void);
/* Move's hand appears at a quarter size 16 pixels above the caster, with a */
/* crack and eight sparkles that scatter from it, and sets out 16 pixels the */
/* way the caster faces, to the nearest quarter turn. */
void *fdEffectSpawnMoveHand(void *object);
/* Grows a hand to full size at once and waits for it to reach where it is */
/* going. */
void fdEffectGrowHandAndWait(struct ACTOR *actor);
/* The hand spins as it rises for BURST_RISE_FRAMES, shrinks, and bursts into */
/* BURST_SPARKLES sparkles (SPRITE_SPARKLE). */
void fdEffectBurstHand(void *object);
/* Gives each character in the party amount PP (PP regenerates as the party */
/* walks). */
void fdPartyAddPp(s32 amount, s32 unused);
/* Changes each party member's HP by amount, or by that percentage of maximum */
/* HP when scaled is set; a loss flashes the screen orange. */
void fdPartyAddHp(s32 amount, s32 scaled);
/* Counts the free blocks of the sprite VRAM cache. */
s32 fdResourceTableCountFreeBlocks(void);
/* Asks yes or no at the top of the screen, or at the bottom with atBottom set.
 */
/* Returns 0 for yes, 1 for no. */
s32 fdYesNoAsk(s32 atBottom);
/* Returns the formation in slot (0 to 7) of zone. */
u16 fdEncounterGetFormationAtSlot(s32 zone, s32 slot);
/* Returns the formation of Djinni battle djinni, which the table gives as a */
/* zone and slot. */
u16 fdEncounterGetDjinnFormation(s32 djinni);
/* Starts the current scene's field music, chosen by scChooseMusic. */
void scPlayMusic(void);
/* Chooses the current scene's field music (MainRoutineStateWork.cue) from the
 */
/* first rule matching the scene or its area and the entrance; SOUND_KEEP_BGM */
/* when none does. */
void scChooseMusic(void);
/* A Djinni joins. The element's last Djinni stands for the one of The Broken */
/* Seal's chosen for it (SCENE_STATE_CHOSEN_DJINN; none: nothing happens). */
/* While FLAG_DJINN_LESSON is set it joins the leader by its flag alone, */
/* otherwise the party member with the fewest; if it found one, the element's */
/* join effect plays at the actor (none for actor -1), the Djinni is added, */
/* and the actor's event slot is cleared (from patch 1; the Japanese release */
/* leaves it set). */
void fdDjinnJoinParty(s32 actor, s32 element, s32 djinni);
/* Updates one field effect sprite: counts its age, runs its callback (after */
/* any delay), moves it and draws it. */
void fdParticleUpdate(FIELD_PARTICLE *effect);
/* Reports an unfinished target only for effects that stop on arrival. */
u32 fdParticleHasPendingTarget(FIELD_PARTICLE *effect);
/* Places a field effect sprite at (x, z), at rest with no target. */
void fdParticleSetPos(FIELD_PARTICLE *effect, s32 x, s32 z);
/* Sets the animation of a field effect sprite. */
void fdParticleSetAnimation(FIELD_PARTICLE *effect, s32 mode);
/* Gives a field effect sprite its per-frame callback, starting at state 0. */
void fdParticleSetCallback(FIELD_PARTICLE *effect, EFFECT_CALLBACK callback);
/* Starts a field particle with a sprite of the given kind at (x, z), at full */
/* size and in front of everything: it will fly to its target at up to 2 */
/* pixels a frame, turning a limited step at a time, and stop there. */
void fdParticleInit(FIELD_PARTICLE *effect, s32 kind, s32 x, s32 z);
/* Frees a field effect sprite and its sprite, clearing it to zero (inactive).
 */
void fdEffectClearOwnedSlot(FIELD_PARTICLE *effect);
/* Converts a world position (16.16 pixels) to the screen: x and z receive the
 */
/* screen's x and y and y becomes 0. */
void fdCameraWorldToScr(struct VECTOR3 *position);
/* Returns the voice of sprite id's messages: the first of its four blip */
/* sounds, SE_VOICE_FIRST plus its definition's value, or 0 for none */
/* (SPEAKER_VOICE_NONE) or with the Speech option off */
/* (MainRoutineStateWork.speech). mnMessageOpenWindowStub takes it in the high
 */
/* half of its last argument. */
s32 fdSpeakerGetVoice(s32 id);
/* Clears each button shortcut whose Psynergy can no longer be cast: zeroes */
/* MainRoutineStateWork.firstShortcut or secondShortcut. */
void fdPartyClearUnusableShortcuts(void);
/* Allocates and clears the field effect sprites (HEAP_SLOT_FIELD_EFFECT) and */
/* starts their update task. */
void fdEffectInitSlots(void);
/* Stops the field effect sprites: removes their task, frees every slot still */
/* in use and releases their heap block, the field Psynergy's state with it. */
void fdEffectReleaseSlots(void);
/* Places an actor on the leader, a pixel nearer the back, and sends it walking
 */
/* (xOffset, zOffset) pixels away at speed 0.8 and acceleration 0.4, through */
/* the launch script; the actor then takes movement type action. */
void fdActorLaunchFromLeader(s32 actorId, s32 xOffset, s32 zOffset, s32 action);
/* Walks the actor (xOffset, zOffset) pixels from where it stands, through the
 */
/* offset script. */
void fdActorWalkByById(u32 actorId, s32 xOffset, s32 zOffset);
/* Walks the actor (xOffset, zOffset) pixels, waits for it to arrive and stands
 */
/* it still. */
void fdActorWalkByAndWaitById(u32 actorId, s32 xOffset, s32 zOffset);
/* Makes an actor follow another (the watched actor script): writes its */
/* watchedActor. */
void fdActorWatchAndSetScript(s32 actorId, s32 watchedActorId);
/* Holds an actor still under Halt, keeping its per-frame update and its first
 */
/* animation's colour mode in MainRoutineStateWork for */
/* fdPsynergyResumeHaltedActor. */
void fdPsynergyHaltActor(s32 actorId);
/* Lets a halted actor move again, with the update and colour mode kept by */
/* fdPsynergyHaltActor. */
void fdPsynergyResumeHaltedActor(s32 actorId);
/* Returns the first of the scene's placed actors (from ACTOR_FIRST_PLACED) */
/* showing spriteId, or -1. */
s32 fdActorFindBySpr(s32 spriteId);
/* Returns the sprite (the first animation's id) of the actor whose id is */
/* key's under SPEAKER_ACTOR_MASK, or -1 if there is no such animated actor. */
s32 fdActorGetSprById(s32 key);
/* Saves up to 32 actors into SnapshotObjects, with each one's slot, */
/* animation, sprite flags and priority, so that they can be put back. */
void fdActorTableSnapshot(void);
/* Puts back the actors the snapshot saved, and moves the camera to the party */
/* leader's height. */
void fdActorRestore(void);
/* Starts a zoom to zoom (16.16) over frames frames, on the World Map only: */
/* fdWorldMapStepZoom takes the view's MAP_STATE.zoom to farPlane / zoom. */
void fdWorldMapStartZoom(FIXED zoom, s32 frames);
/* Waits for a zoom transition to end (on the World Map only, at most 300 */
/* frames). */
void fdEventWaitForZoomTransition(void);
/* The area name number for `scene` entered at `entrance`, as the save files */
/* and the world map show it. */
s32 fdAreaGetNameIndex(s32 sceneId, s32 entrance);
/* Unused: nothing calls fdPairTableFindStub. Looks key up in PairTable's */
/* {key, value} rows and returns its value, or -1; the table holds only its */
/* end, so every key gets -1. */
s32 fdPairTableFind(s32 key);
/* Unused: nothing calls fdPairTableFindAndIgnoreStub. Looks key up as */
/* fdPairTableFind does and returns 0. */
s32 fdPairTableFindAndIgnore(s32 key);
/* Unused: nothing calls fdPairTableFindCopyStub. fdPairTableFind again. */
s32 fdPairTableFindCopy(s32 key);
/* Unused: only the stub table refers to it. Draws heading picture */
/* `heading` of the headings resource into a 26 by 5 cell window and opens */
/* the window from the centre. */
void fdHeadingShow(s32 heading);
/* Waits up to two seconds, stopping early when a button is pressed. */
void fdEventWaitTwoSecondsOrPress(void);
/* Unused: only the stub table refers to it. Closes fdHeadingShow's */
/* picture window: fades the palette out while wiping every one of the */
/* picture's 26 by 5 tiles from its top left corner to its bottom right, a */
/* diagonal every two frames (Heading_WipeSteps: a tile's pixels a diagonal at
 */
/* a time, each ended by a negative entry and the last by HEADING_WIPE_END), */
/* then turns BG0 and the window off, clears the screen block, reloads the */
/* window tiles and turns BG0 back on. */
void fdHeadingClose(void);
/* Starts an actor's movement type: an MOVEMENT_TYPE, or any other value as
 */
/* the actor script to run itself. */
void fdActorSetMovementType(struct ACTOR *actor, s32 kind);
/* Plays SE_STOP_EXIT_SE, then soundId (SOUND_FADE_OUT_ACTION_SE for */
/* -1), and remembers soundId for scPlayQueuedMusic in the field state's */
/* queuedSnd; -1 is remembered as none, so it plays SOUND_FADE_OUT_ACTION_SE
 */
/* now and nothing later. */
void scQueueMusic(s32 soundId);
/* An event that does nothing. */
void fdEventNoOp(void);
/* Plays the sound scQueueMusic remembered, if any. */
void scPlayQueuedMusic(void);
/* Does nothing; nothing calls fdActorReservedNoOpStub. */
void fdActorReservedNoOp(void);
/* The ropes Lash ties between two posts, given by their actor ids: placing, */
/* tying, coiling and swinging them, laying their nodes and drawing them. */
void fdRopeInit(void);
/* Lays rope index tied between posts id and other, and shows both posts with */
/* the rope wound round them. */
/* A unit whose ROM code is compiled against another form defines OWN_Rope_Tie
 */
/* and declares that form itself. */
#ifndef OWN_Rope_Tie
void fdRopeTie(s32 index, s32 id, s32 other);
#endif
/* Coils rope index at post firstId, its length reaching post secondId: the */
/* rope as it waits to be lashed. */
void fdRopeCoil(s32 index, s32 firstId, s32 secondId);
/* Lash: throws rope index from post fromId to post toId and winds it round */
/* both. A rope coiled too short to reach falls back to its post. */
/* A unit whose ROM code is compiled against another form defines OWN_Rope_Lash
 */
/* and declares that form itself. */
#ifndef OWN_Rope_Lash
void fdRopeLash(s32 index, s32 fromId, s32 toId);
#endif
/* Unties rope index between posts fromId and toId (fdRopeRecoil). */
void fdRopeUntie(s32 index, s32 fromId, s32 toId);
/* Allocates the tightropes' state, loads their node picture and starts */
/* fdTightropeDraw and fdTightropeUpdate. */
void fdTightropeInit(void);
/* Lays tightrope slotNo straight between posts firstId and secondId, and */
/* shows the rope wound round both. Declared s32, as the scenes call it, though
 */
/* it returns no value; no caller reads one. */
s32 fdTightropePlace(s32 slotNo, s32 firstId, s32 secondId);
/* Returns rope index's ROPE_SLOT_STATE, or -1 while the ropes have no */
/* state. */
s32 fdRopeGetState(s32 index);
/* Gives rope index its flag and places it between posts firstId and */
/* secondId: tied when the flag is set, else coiled at the first. A scene */
/* places each of its ropes with it. */
void fdRopePlace(s32 index, s32 flag, s32 firstId, s32 secondId);
/* Returns the flag entry table, 20-byte entries ended by a zero flag. */
const struct WORLD_MAP_PLACE *fdWorldMapPlacesGetTable(void);
/* Sets up a scene's flag blocks, the puddles Frost turns into pillars of ice */
/* (actor and flag pairs, ended by ACTOR_LIST_END): each stops being solid and
 */
/* blocks its cell; one whose flag is set shows the ice pillar and has the */
/* flag cleared, the others the puddle. Then fdFlagBlockApplyFlags raises the */
/* frozen ones and sets their flags again, and the leader is lifted onto the */
/* ground if it now lies below it. A scene with such puddles calls it as it */
/* starts. */
void fdFlagBlockPlaceAll(struct ACTOR_FLAG *actors);
/* Sets up Alhafra's and the Alhafran Cave's puddles as fdFlagBlockPlaceAll */
/* does, and marks the open cells on each puddle's four sides a gap to jump */
/* across (fdMapGridMarkJumpIfOpen), so the leader jumps onto a frozen one. */
void fdFlagBlockPlaceAllMarkingEdges(struct ACTOR_FLAG *list);
/* For each flag block whose animation disagrees with its flag, turns its cell
 */
/* and flag over: with the flag set and the ice pillar not showing, the cell */
/* is lowered 32 pixels to a wall and the flag cleared; with the flag clear */
/* and the puddle not showing (Frost freezing it), the cell is raised 32 */
/* pixels, open and a gap to jump across, and the flag set. The field state */
/* remembers the block. */
void fdFlagBlockApplyFlags(struct ACTOR_FLAG *list);
/* The rest of what the event commands share: actors, cells, sound and the */
/* party. */
s32 fdActorGetCellIndex(struct ACTOR *actor);
/* Makes each listed actor (ids, ended by ACTOR_LIST_END) a block in the map: */
/* its cell's shape is tagged BLOCK_HEIGHT_STEPS above the ground, so the party
 */
/* can stand on it, its cell is marked MAP_CELL_JUMP, a gap to jump onto from */
/* another block (the old mark kept in the actor), and the overhead layer's */
/* cell is a wall. A */
/* leader below the new ground is lifted onto it. Scenes call it as they */
/* start, for the blocks Move pushes. */
void fdActorPlaceBlocks(u16 *actors);
/* Takes the block about to move out of the map: the field effect's target */
/* while one runs, else the actor ahead of the leader, if it is one of the */
/* listed actors. Its cell goes back to the ground and its old mark, and the */
/* event state remembers it for fdActorPlaceMovedBlock. */
void fdActorLiftTargetBlock(u16 *actors);
/* Puts the block fdActorLiftTargetBlock remembered back into the map at the */
/* cell it has moved to, as fdActorPlaceBlocks places each. */
void fdActorPlaceMovedBlock(void);
/* Returns the index of the map's shape entry (MapGridShapes, MAP_SHAPE) of */
/* shape whose first height is height, in 8-pixel steps, writing it into the */
/* first free entry (MAP_SHAPE_FREE) when there is none: the entry a raised */
/* or lowered cell's shape grid byte takes. The layer goes unread. */
s32 fdMapShapeFindOrAdd(s32 layer, s32 shape, s32 height);
/* Returns the first of the placed actors (not the treasure spots) on */
/* position's cell, at its height too when checkY is set, or NULL. */
struct ACTOR *fdActorFindAtCell(s32 *position, s32 checkY);
/* Whether the actor 16 pixels in front of the leader, along its facing rounded
 */
/* to a quarter turn, can be pushed on by 16 pixels: 1 when nothing solid */
/* stands on the cell beyond it or above it and its terrain test passes there,
 */
/* else 0. fdLeaderPushObjectAhead, after it, makes the push. */
/* Since patch 1 the actor Halt has stopped (MainRoutineStateWork.haltedActor)
 */
/* cannot be pushed; the Japanese release has no such test. */
s32 fdLeaderCanPushObjectAhead(void);
/* Pushes the actor ahead of the leader on by 16 pixels, as */
/* fdLeaderCanPushObjectAhead allowed. */
void fdLeaderPushObjectAhead(void);
/* Returns the actor one cell, or failing that two cells, ahead of the leader */
/* at the leader's height, or NULL. */
struct ACTOR *fdLeaderFindActorAhead(void);
/* The pieces of the casts: their particles, hands and sparks, the tiles they */
/* leave, and the scripts and tables they run. */
struct ACTOR *fdActorSpawn(s32 kind, s32 x, s32 y, s32 z);
/* Clears what Cyclone blew away: each cell of FIELD_STATE.psynergyCells that */
/* falls in the width by height window at (windowX, windowZ) of layerIndex */
/* (counted from the layer's scroll and the leader's height) takes the cell */
/* picture from the same place of the window at (sourceX, sourceZ), is opened */
/* on layer 2, and is recorded (up to 64) in TouchCopiesWork to be made again.
 */
/* A unit whose ROM code is compiled against another form defines */
/* OWN_TileCopies_CopyTouchedCells and declares that form itself. */
#ifndef OWN_TileCopies_CopyTouchedCells
void fdTileCopiesCopyTouchedCells(s32 sourceX, s32 sourceZ, s32 width,
								  s32 height, s32 windowX, s16 windowZ,
								  s32 layerIndex);
#endif
/* Whether the leader can move as sand on the cell at (x, z): the overhead */
/* layer's CELL_BEHAVIOR_SAND, or on the world map collision */
/* WORLD_MAP_COLLISION_WALKABLE_FIRST or shallow water; */
/* FIELD_STATE.sceneAttributeHigh turns the answer round. */
s32 fdMapGridIsSandCell(s32 x, s32 z, s32 layer);
/* Whether the cell at (x, z) can be climbed (MAP_CELL_CLIMBABLE): Sand starts
 */
/* up a wall face only there (fdActorMoveAsSand) and ends its climb off one. */
/* A unit whose ROM code is compiled against another form defines */
/* OWN_MapGrid_IsClimbableCell and declares that form itself. */
#ifndef OWN_MapGrid_IsClimbableCell
u8 fdMapGridIsClimbableCell(s32 x, s32 z, s32 layer);
#endif
/* Returns the leader's actor id, or ACTOR_FIRST_PLACED while */
/* MainRoutineStateWork.ownerOverride is set. */
s32 fdPartyGetLeaderActor(void);
/* Hangs a vine at once from high above actor id down to its feet */
/* (fdHangingVineHang): an actor that is not moving is lifted and moved 16 */
/* pixels north first, which fdHangingVineHang undoes; a moving one has its */
/* vine only when force is set. */
void fdHangingVinePlace(s32 id, s32 force);
/* The hanging vines, each hung from an actor. */
void fdHangingVineHang(s32 objectId, s32 grow);
/* Whirlwind on the vine hanging from actor id: the vine swings out to the */
/* leader's side and the leader takes hold. While the leader hangs on, B, Down
 */
/* or the direction away from the vine lets go; any other button leaps, and the
 */
/* vine carries the leader across to the far side. */
void fdHangingVineSwing(s32 id);
/* fdExitPrepareAnimation for any actor: centres the actor on its cell at the */
/* cell's south edge, stops it, hides its shadow and plays ANIM_LEADER_LEAVE */
/* (side 1) or ANIM_LEADER_LEAVE_ALTERNATE over 18 frames, */
/* its sprite gaining the doorway (SPRITE_DOORWAY) as a second part. */
void fdExitPrepareActorAnimation(s32 actorId, s32 side);
/* An actor appears in a whirl of sparks, as Alex does on Venus Lighthouse: */
/* APPEAR_SPARK_COUNT sparkles (RESOURCE_SPARKLE_FADE) circle round where it */
/* stands, each rising and closing in for SPARK_LIFE frames. The actor spins */
/* and widens from nothing to full width over APPEAR_WIDEN_FRAMES, when its */
/* shadow shows again, and the sparks stop coming APPEAR_SPAWN_FRAMES later. */
void fdActorAppearInSparks(s32 actorId);
/* Turns actor id's sprite to rotation (1/65536 turns). */
void fdActorSetSprRotationById(s32 id, s32 rotation);
/* Lays actor id down along angle, a quarter turn rounded: shows its first */
/* frame turned, shifted by the side it falls to, and blocks its cell and the */
/* cell beyond it. */
void fdActorFallOverById(u32 id, u32 angle);
/* fdActorFallOverById with an animation of the actor's sprite sheet (-1 for */
/* none) and the fall moving the actor itself half a cell the way it falls. */
void fdActorFallOverWithAnimationById(s32 id, u32 angle, s32 animation);
/* Fills an effect sprite: OAM attributes 0 and 1 as given (no affine matrix, */
/* 256 colours), the tiles, unscaled and unrotated at depth 255, and the */
/* picture's size from its shape, pivoted at (pivotX, pivotY). */
void fdEffectSprInit(struct EFFECT_SPR *sprite, s32 pivotX, s32 pivotY,
					 u32 attributes, s32 tile);
/* Draws a field effect's sprite. An unscaled, unrotated sprite needs no */
/* affine matrix; any other takes one, and one scaled past 1 is drawn in a */
/* double-size box. The picture's pivot is turned and scaled with it, so the */
/* pivot stays on the sprite's position. A sprite wholly off screen is not */
/* queued; the rest join the OAM bucket of their depth. */
void fdEffectSprDraw(struct EFFECT_SPR *sprite);
/* Places sprite at the screen position of world position and draws it. */
void fdEffectSprDrawAtWorldPos(struct EFFECT_SPR *sprite, s32 *position);
/* Moves a 2D point by its velocity, damped and pulled by gravity */
/* (fdAdvanceWithGravity2D). */
void fdBattleEffectStepVector2(s32 *vector, s32 damping, s32 gravity);
/* Moves a 3D point by its velocity, damped and pulled by gravity */
/* (fdAdvanceWithGravity3D). */
void fdBattleEffectStepVector3(s32 *vector, s32 damping, s32 gravity);
/* Returns the OBJ priority of the actor's sprite. */
s32 fdActorGetSprPriority(struct ACTOR *actor);
/* Returns the depth slot the actor's sprite takes, so that a sprite drawn */
/* with it sorts with it: from its projected depth on the world map, otherwise
 */
/* from its screen row. */
s32 fdActorGetSprDepth(struct ACTOR *actor);
/* Fades the palettes to color over frames frames, keeping the field's colours
 */
/* for fdStartPaletteTransition; FLAG_PSYNERGY_TINTED is set while it */
/* holds. */
void fdStartColTransition(s32 frames, s32 color);
/* Fades the palettes back from fdStartColTransition's colour over frames */
/* frames, the BG half to grey while Reveal is active; clears */
/* FLAG_PSYNERGY_TINTED. */
void fdStartPaletteTransition(s32 frames);
/* Starts the swelling arcs over a caster: loads their tiles and starts */
/* fdBattleEffectUpdateArcEffect with no pair showing. */
void fdBattleEffectStartArcEffect(void);
/* Ends the swelling arcs: frees their tiles and their state, and stops their */
/* task. */
void fdBattleEffectEndArcEffect(void);
/* Waits, at most ARC_GROUPS_WAIT_FRAMES, until both pairs of swelling arcs */
/* have run their 32 frames. */
void fdBattleEffectWaitForArcGroups(void);
/* Gives the caster its look while it casts (CASTER_LOOK). */
void fdBattleEffectSetObjectUpdateMode(struct ACTOR *actor, s32 mode);
/* Says whether an actor stands where it may not: 1 when, outside the World */
/* Map, fdMapGridCheckCollisionAtStub reports its cell, ORed with 2 when it */
/* overlaps any other solid actor (each radius less 2 pixels). */
s32 fdActorCheckBlocked(struct ACTOR *actor);
/* Returns the behaviour (collision code) of the map cell at x, z in layer. */
/* A unit whose ROM code is compiled against another form defines */
/* OWN_MapGrid_GetCellBehavior and declares that form itself. */
#ifndef OWN_MapGrid_GetCellBehavior
u8 fdMapGridGetCellBehavior(s32 x, s32 z, s32 layer);
#endif
/* The map's cells: their attributes and what sand, water and Cyclone make of */
/* them. */
s32 fdMapGridGetCellAttributesAt(s32 x, s32 z, s32 layer);
/* Sets the cell behavior of the map cell at x, z in layer, and returns it. */
u8 fdMapGridSetCellBehavior(s32 x, s32 z, s32 layer, s32 value);
/* Sets the attribute byte of the map cell at x, z in layer. */
u8 fdMapGridSetCellAttributesAt(s32 x, s32 z, s32 layer, s32 value);
/* Sets bits in the attribute byte of the map cell at x, z in layer. */
u8 fdMapGridSetCellAttributeBitsAt(s32 x, s32 z, s32 layer, s32 bits);
/* Clears bits in the attribute byte of the map cell at x, z in layer. */
u8 fdMapGridClearAttributeBBitsAt(s32 x, s32 z, s32 layer, s32 bits);
/* Shows an actor as an item's icon, its sprite's only picture: the item, or */
/* for coins (SCOOPED_COINS) COIN_ICON_ITEM's. */
void fdActorShowItemIconById(s32 id, s32 item);
/* Frees tightrope index's nodes; it is no longer drawn. */
void fdTightropeRelease(s32 index);
/* Stops tightrope index's wobble and marks its nodes not to be walked. */
void fdTightropeSettle(s32 index);
/* Gives an actor the leader's sprite priority, so it is drawn in front of */
/* and behind what he is, and keeps it there. */
/* A unit whose ROM code is compiled against another form defines */
/* OWN_Actor_TakeLeaderLayer and declares that form itself. */
#ifndef OWN_Actor_TakeLeaderLayer
s32 fdActorTakeLeaderLayer(struct ACTOR *actor);
#endif
/* The actor leaves in a whirlwind (SPRITE_WHIRLWIND): as the screen */
/* transition starts it circles inside it for DEPART_FRAMES, from 96 pixels up
 */
/* and 32 out down to where it stood, then the whirlwind vanishes with a whoosh
 */
/* and the camera follows the actor again. */
void fdActorDepartInRing(s32 actorId);
/* The actor arrives in a whirlwind, fdActorDepartInRing's counterpart: once */
/* the screen transition is done, a whirlwind (SPRITE_WHIRLWIND) widens on the
 */
/* unseen, spinning actor for WHIRLWIND_RESIZE_FRAMES, then vanishes with a */
/* whoosh as the actor grows back from half its size and shows again. */
void fdActorArriveInRing(s32 actorId);
/* Drops an actor from Scoop's hand (if there is one) to fall to the ground, */
/* waits 50 frames, then asks for its talk, which opens it without its */
/* "checked the ..." message (FIELD_TALK_SILENT). */
void fdScoopDropFromHand(s32 id);
/* Shows message in an event and clears FLAG_NOTHING_HAPPENS; when someone in */
/* the party has Scoop, the field menu then opens. */
void fdEventShowMessageAndOfferScoop(s32 message);
/* Fills the overlay's 512 bytes of sprite tiles */
/* (VRAM_CACHE_ENTRY_FADE_OVERLAY) with PIXELS_8BPP(1) when solid is set, which
 */
/* in these 16-colour tiles is colour 1 in every other column (no caller sets */
/* it), else clears them. */
void fdFadeOverlayFillTiles(s32 solid);
/* Sets up sprite through fdEffectSprInit with the overlay's tiles */
/* (VRAM_CACHE_ENTRY_FADE_OVERLAY), at priority 1 in palette 0. */
void fdEffectSprInitWithFadeOverlay(struct EFFECT_SPR *sprite, s32 pivotX,
									s32 pivotY, u32 attributes);
/* Projects the world position (x, y, z) to the screen, stores it as the */
/* effect sprite's position and draws the sprite. */
void fdEffectSprDrawAtWorldXYZ(struct EFFECT_SPR *sprite, s32 x, s32 y, s32 z);
/* Per-frame update: sets every part of an actor's sprite to colour mode */
/* frame % 6, which cycles its colours. */
/* A unit whose ROM code is compiled against another form defines */
/* OWN_Actor_CycleSpriteColors and declares that form itself. */
#ifndef OWN_Actor_CycleSpriteColors
void fdActorCycleSprCols(struct ACTOR *actor);
#endif
/* Clears the weeds the Cyclone blew away: for each cell it touched (the field
 */
/* state's list from fdPsynergyGatherNearbyCells) inside the width by */
/* height rectangle at (x, z) of the layer, copies the matching cell tiles */
/* from the rectangle at (toX, toZ) over it, opens it on layer 2 and records */
/* the copy in TouchCopiesWork, up to 64. Gaia Rock calls it after a Cyclone. */
/* A unit whose ROM code is compiled against another form defines */
/* OWN_Cyclone_CopyTouchedCells and declares that form itself. */
#ifndef OWN_Cyclone_CopyTouchedCells
void fdCycloneCopyTouchedCells(s32 x, s32 z, s16 layerIndex, s16 width,
							   s16 height, s32 toX, s32 toZ);
#endif
/* Runs fdTeleportPlaySparks, fades the palettes back to the saved ones and */
/* waits 10 frames. */
void fdTeleportPlaySparksAndFade(void);
/* Keeps an actor turning to face another. */
void fdActorWatchActor(struct ACTOR *actor, struct ACTOR *target);
/* As fdActorStop, and also clears the actor's target and velocity, so it */
/* stands where it is. */
void fdActorResetTargetsAndVelocity(u32 actorId);
/* Leaves through a stairs-up exit (ANIM_LEADER_LEAVE); FLAG_ARRIVAL_STAIRS_UP
 */
/* makes the next scene play the matching arrival (fdEntrancePlayArrival). */
void fdExitStartStairsUp(void);
/* Leaves through a stairs-down exit (ANIM_LEADER_LEAVE_ALTERNATE), recorded in
 */
/* FLAG_ARRIVAL_STAIRS_DOWN. */
void fdExitStartStairsDown(void);
/* Unused: nothing calls fdMotesStartStub. Starts the 64 floating motes */
/* (fdMotesUpdate): loads their tiles and puts each under the leader with */
/* staggered timers, drawn semi-transparent. */
void fdMotesStart(void);
/* Starts fdWeatherUpdateRain's raindrops: allocates and clears */
/* HEAP_SLOT_WEATHER, decodes the streak tiles into VRAM, sets each of the 64 */
/* drops' sprite words and places it on the ground at the map's focus with its
 */
/* first fall staggered, turns on alpha blending of all layers at 16/16, */
/* schedules the update and, when asked, plays SE_KIRIN (scQueueMusic). */
void fdWeatherStartRain(s32 withCue);
/* Stops fdWeatherStartRain's particles. Unused: nothing calls */
/* fdWeatherStopRainStub. */
void fdWeatherStopRain(void);
/* Plays a summon tablet's reading (a CONTAINER_STONE_TABLET treasure, */
/* "examined the stone tablet"): pebbles fly off the tablet as it is struck, */
/* and its runes light up and fly round the leader, with their sounds, before */
/* mnSummonShowLearnedMessage. */
void fdSummonTabletPlayRunes(s32 targetId);
/* Counts one more weird answer Felix gives */
/* (MainRoutineStateWork.weirdAnswers). */
void fdEventCountAnswer(void);
/* Where the party comes out on the world map after reaching scene and */
/* entrance (Teleport's choice calls it). With FLAG_SHIP_CAN_SAIL set (the */
/* party has the ship, whose mooring the map screen marks), finds the */
/* first rule of Scene_ReturnRules for scene and entrance (-1 on either */
/* side matches any entrance) and copies its position, x and z in 16.16 and the
 */
/* third value as is, to MainRoutineStateWork.shipX, shipZ and shipFacing. */
void fdWorldMapSetReturnForScene(s32 scene, s32 entrance);
/* fdWorldMapSetReturnForScene's twin for the rule's return scene and entrance
 */
/* (its third and fourth values): with FLAG_SHIP_CAN_SAIL set, clears */
/* ownerOverride and copies the rule's position to the party's saved position.
 */
void fdWorldMapSetReturnLeavingScene(s32 scene, s32 entrance);
/* A Mad Plant (sprite) springs out of the bushes cleared at the cell (x, z): */
/* it grows, turns to the leader, hops twice and leaps at the leader in an arc
 */
/* before the battle. The Gabomba Catacombs, Taopo Swamp and Gaia Rock each */
/* hide one. */
void fdMadPlantLeapAtLeader(s32 sprite, s32 x, s32 z);
/* Returns 1 while fdScrTransitionUpdateScanline is the HBlank handler, that */
/* is, while a centre-line transition runs. */
s32 fdScrTransitionIsScanlineActive(void);
/* Checks whether the party can carry one more of item; when it cannot, says */
/* so and returns -1, else 0. */
s32 fdPartyCheckItemLimit(s32 item);
/* Returns the first of the scene's triggers of kind for target whose */
/* condition holds and whose push (STEP_TRIGGER_FLAG_WALKING) and facing */
/* (TRIGGER_FLAG_FACING) requirements are met, or NULL. TRIGGER_PUSH and */
/* TRIGGER_PUSHED, the triggers fdRunPushCell runs for an actor pushed and */
/* for the cell pushed against, answer any target; the facing test alone is */
/* skipped while MainRoutineStateWork.movementMode is LEADER_MOVEMENT_KEYS. */
struct TRIGGER_VIEW *scFindTrigger(s32 kind, s32 target);
/* Packs the target colours, three channels each, back into the palette copy as
 */
/* RGB555. */
void fdPaletteFadePackTarget(void);
/* Takes colors as the BG colours of the palette copy and makes them, */
/* unchanged, the BG colours' target. */
void fdPaletteFadeLoadPalette(const u16 *colors);

#endif
