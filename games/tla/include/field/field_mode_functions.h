#ifndef ALCHEMY_FIELD_MODE_FUNCTIONS_H
#define ALCHEMY_FIELD_MODE_FUNCTIONS_H

/* The field-mode module's functions, declared once each in the order of its
   stub table, without the inlines field_mode.h brings in with field.h and its
   other headers. It is for the units that must not see those inlines:
   field_effect_advance_motion_priority.c, field_effect_advance_spin.c and the
   scenes' shared scene_set_arrival_flags.c, which have it through script.h, and
   actor_scripts.c, cracked_tile_mask.c, field_effect_spawn.c and
   field_effect_spawn_resident.c. field_mode.h includes it for everyone else. */

#include "types.h"
#include "fixed_math.h"
#include "field/field_particle.h"

struct ACTOR;
struct ACTOR_FLAG;
struct ACTOR_PLACE;
struct EFFECT_SPR;
struct FIELD_PARTICLE;
struct CUE_VIEW;
struct VECTOR3;
struct VIEWED_PLACEMENT;
struct WORLD_MAP_PLACE;

/* The field-mode module's functions in the order of its stub table
   (src/field/mode_stubs.s). The field mode: runs the current scene, which the
   party entered by entrance, until it leaves; returns the exit kind it left by,
   for gmTakeExit, or EXIT_KEEP_DESTINATION when the next scene is already in
   MainRoutineStateWork: a random battle, a party that fell outside battle,
   Retreat or a script's destination. A unit whose ROM code is compiled against
   another form defines OWN_fdModeRun and declares that form itself. */
#ifndef OWN_fdModeRun
Sint32 fdModeRun(void);
#endif
/* The main routine: runs one mode after another and never returns. mode, an
   GAME_START, chooses the first scene. As The Broken Seal's, with this game's
   four debug starts, rendering re-enabled after DMA 0 stops, and three
   minigames, Super Lucky Dice the third. */
void gmMainRoutineRun(Sint32 start);
/* Waits the requested ticks through sySynch(), unless debug fast-forward
   (fdScriptUpdateDebugSkip) is on: the wait scripts use. */
void fdScriptWait(Sint32 ticks);
/* Begins a script: closes any area name window, stops the leader, ends the
   lasting Psynergy if a cue is running, resets the message and timing state,
   and starts the debug fast-forward event with the leader as the message
   speaker. */
void fdScriptStart(void);
/* Ends a script: stops the debug fast-forward event, gives the camera back to
   the leader (or to the first placed actor, the ship, while
   MainRoutineStateWork.ownerOverride is set) and refreshes the Lure Cap's flag
   (gmFlagRefreshLureCap). A unit whose ROM code is compiled against another
   form defines OWN_fdScriptEnd and declares that form itself. */
#ifndef OWN_fdScriptEnd
void fdScriptEnd(void);
#endif
/* Rebuilds the leader's actor from its placement row (fdActorResetForLeader),
   waits a tick and returns the leader's actor. */
struct ACTOR *fdScriptResetLeaderAndWait(Sint32 tbl);
/* Spawns the actors of an actor placement table after the last actor in use. */
void fdScriptSpawnActorsAfterLast(Sint32 tbl);
/* Gives each actor placement with condition 0 a condition saying whether it
   lies within the map's view bounds, so that only those inside appear. */
void fdAssignViewFlags(struct VIEWED_PLACEMENT *entry);
/* Replaces the scene's cue table with scripts. */
void fdScriptSetSceneCues(Sint32 events);
/* Adds unit to the party. With announce nonzero it also shows `<name> joined
   your party.` (mnPartyShowJoinedMessage, which receives announce too). */
void fdScriptAddPartyMember(Sint32 unit, Sint32 announce);
/* Adds two units to the party and announces them together ("<a> and <b> joined
   your party.", mnPartyShowPairJoinedMessage). */
void fdScriptAddTwoPartyMembers(Sint32 first, Sint32 second);
/* Takes unit out of the party, fully restoring its HP and PP and curing its
   poison. */
void fdPartyRemoveMemberRestored(Sint32 unit);
/* The script command "give item": gives item to the first party member with
   room for it and says so; with no room, the player chooses what to drop. */
Sint32 fdPartyInventoryGiveItem(Sint32 item, Sint32 unused);
/* Gives item to one party member. Returns unit, or -1 if its bag is full. */
Sint32 fdInventoryTryAddAndReturnMember(
	Sint32 item, Sint32 unused, Sint32 unit);
/* The script command "choose yes or no": once the message on screen has been
   read, asks yes or no below it, or above it when the message windows reach
   past YES_NO_BELOW_LAST_ROW (unless forceBottom is set). The speaker nods
   (ANIM_NOD) at yes and shakes its head (ANIM_SHAKE_HEAD) at no; at the fastest
   message speed neither is waited for. */
Sint32 fdScriptAskYesNoWithActorAnimation(Sint32 actor, Sint32 forceBottom);
/* Gives the party item from actor: with no room it shows the item, says it was
   found and left behind and returns -1; otherwise it adds the item, sets flag
   (unless -1) and returns 0. */
Sint32 fdScriptPickUpItem(Sint32 actor, Sint32 item, Sint32 flg);
/* Teaches unit the Psynergy ability and says so with its jingle, waiting for
   the player to read it. */
void fdPartyLearnAbility(Sint32 unit, Sint32 ability);
/* Returns an actor, or NULL if it does not exist. */
struct ACTOR *fdActorGetById(Uint32 actorId);
/* The script command "release actor": releases the actor and empties its slot
   in the field state's actor table. */
void fdActorReleaseById(Sint32 actorId);
/* The script command "set actor speed": its top speed and acceleration, 16.16
   pixels per tick. Writes the actor's speedLimit and acceleration. */
void fdActorSetSpeedParms(Sint32 actorId, FIXED speedLimit, FIXED acceleration);
/* Gives an actor a movement type or actor script (fdActorSetMovementType) and
   makes it face where it moves (ACTOR_FACES_MOTION). */
void fdActorSetMovementTypeById(Sint32 actorId, const void *script);
/* Waits, at most 300 ticks, for an actor's script to reach its end
   (SC_END). */
void fdActorWaitForScriptEndById(Uint32 actorId);
/* The script command "stop actor": stops its movement type and puts it back
   under script control, on the idle script (a lone SC_END), and makes it face
   where it moves. */
void fdActorStop(Sint32 actorId);
/* Starts a movement type or supplied actor script, as
   fdActorSetMovementTypeById does, then waits up to 300 ticks for its script
   to end. */
void fdActorSetMovementTypeAndWaitById(Sint32 actorId, const void *script);
/* The script command "set actor destination": the actor heads for (x, z), in
   pixels, keeping its current animation. */
void fdActorSetDestinationById(Uint32 actorId, Sint32 x, Sint32 z);
/* The script command "move actor and wait": as fdActorSetDestinationById, then
   waits (at most 600 ticks) for the actor to arrive. */
void fdActorMoveToAndWaitById(Uint32 actorId, Sint32 x, Sint32 z);
/* The script command "walk actor to": the actor walks toward (x, z), in
   pixels. */
void fdActorWalkToById(Sint32 actorId, Sint32 x, Sint32 z);
/* The script command "walk actor to and wait": walks the actor to (x, z), in
   pixels, waits for it to arrive and stands it still. */
void fdActorWalkToAndWaitById(Sint32 actorId, Sint32 x, Sint32 z);
/* The script command "center actor and walk": walks the actor sideways to the
   middle of its 16-pixel column, sets its sprite priority to priority
   (fdActorSetSprPriority), then sends it zOfs pixels along z. */
void fdActorCenterOnColumn(Uint32 actorId, Sint32 pri, Sint32 zOfs);
/* The script command "set actor destination offset": the actor heads (xOfs,
   zOfs) pixels from where it stands. */
void fdActorSetDestinationOffsetById(Sint32 actorId, Sint32 xOfs, Sint32 zOfs);
/* The script command "wait for actor": waits (at most 600 ticks) for the actor
   to arrive and stands it still. */
void fdActorWaitForMoveById(Sint32 actorId);
/* The script command "set actor position": puts the actor at (x, z), 16.16
   pixels, at rest; an actor that follows the ground keeps its height above
   it. */
void fdActorSetHorizontalPosWithTerrain(Uint32 actorId, FIXED x, FIXED z);
/* As fdActorSetHorizontalPosWithTerrain, and turns the actor to angle. Written
   in the old style: the angle arrives as a word and is taken as a halfword. */
void fdActorSetHorizontalPosAndAngWithTerrain(
	Sint32 actorId, Sint32 x, Sint32 z, Sint32 ang);
/* Puts the actor at (x, y, z), 16.16 pixels, at rest, raised by the ground
   height there if it follows the ground. */
void fdActorSetPosWithTerrain(Uint32 actorId, FIXED x, FIXED y, FIXED z);
/* Puts the first actor where the second stands, at rest, on the same ground
   height. */
void fdActorPlaceOnActor(Uint32 actorId, Uint32 otherId);
/* The script command "set actor animation" (an ACTOR_ANIMATION: ANIM_STAND,
   ANIM_WALK, ANIM_NOD, ANIM_SHAKE_HEAD and so on). */
void fdActorSetAnimationById(Uint32 actorId, Sint32 anm);
/* Sets the animation timer steps of each of an actor's sprites. */
void fdActorSetAnimationSpeedById(Sint32 actorId, Sint32 steps);
/* The script command "set actor animation and wait": plays the animation and
   waits for it to end. */
void fdActorSetAnimationAndWait(Sint32 actorId, Sint32 anm);
/* Waits, at most 90 ticks, for an actor's animation to change or for its first
   sprite animation's script to reach its hold command (ANIMCMD_HOLD). Only an
   actor drawn as a sprite (ACTOR_SPRITE_ONE) is waited for. */
void fdActorWaitForAnimationChange(Uint32 actorId);
/* The script command "actor jump": throws the actor upward at speed pixels per
   tick with a jump sound (a higher one above 5), then waits ticks ticks. */
void fdActorJump(Sint32 actorId, Sint32 speed, Sint32 ticks);
/* The script command "start repeated motion": the actor squashes and springs
   back count times (1 to REPEATED_MOTION_MAX), through ActorVariantScripts. */
void fdActorStartRepeatedMotion(Uint32 actorId, Sint32 cnt);
/* The script command "run repeated motion": as fdActorStartRepeatedMotion, then
   waits for it to end. */
void fdActorRunRepeatedMotion(Uint32 actorId, Sint32 cnt);
/* Makes an actor jump down off a ledge in front of it: it shows pose for 10
   ticks, hops 12 pixels south from the middle of its 16-pixel column, turns
   toward FACING_SOUTH_SOUTHWEST while sinking 28 pixels with a dust puff every
   other tick in the colour mode dust (none when dust is -1), then jumps on to
   16 pixels south of where it started and lands, waiting at most 180 ticks for
   it unless dust has LEDGE_JUMP_NO_LANDING_WAIT. */
void fdActorJumpDownLedge(Sint32 actorId, Sint32 pose, Sint32 dust);
/* The script command "actor face actor": turns the first actor at once to face
   the second, then waits the given number of ticks. */
void fdActorFaceActorById(Sint32 firstId, Sint32 secondId, Sint32 ticks);
/* The script command "actors face each other": turns two actors toward each
   other, then waits the given number of ticks. */
void fdActorTurnToFaceEachOther(Sint32 firstId, Sint32 secondId, Sint32 ticks);
/* The script command "set actor sprite colour": gives every sprite animation of
   the actor the colour mode (fdActorSetSprCol); with SPRITE_COLOR_BLINK the
   actor blinks instead (fdActorUpdateBlink). */
void fdActorConfigureSprCol(Sint32 actorId, Sint32 colMode);
/* Sets the colour mode (SPR_COL_MODE) of every running sprite animation of an
   actor's sprite. Scripts set SPRITE_COLOR_NORMAL or SPRITE_COLOR_HIDDEN, and
   alternate the two to make an actor flash. */
void fdActorSetSprCol(struct ACTOR *actor, Sint32 colMode);
/* Makes an actor follow the actor in tgtId's ACTOR_WATCH_ACTOR_MASK bits and
   starts script on it. Unless tgtId has ACTOR_WATCH_KEEP_MOTION it keeps
   FOLLOW_DISTANCE, takes the target's top speed with twice its acceleration, to
   keep up with it, and no collision flags, so it cannot block the target. */
void fdActorWatchAndStartScript(
	Uint32 actorId, Sint32 tgtId, const void *script);
/* The script command "set message": the number of the script's next message,
   written to the field state's message. */
void fdScriptSetMessageId(Sint32 msg);
/* The script command "open message": opens the script's current message in a
   speech window next to the speaking actor, with the speaker's portrait beside
   it when it has one, and moves on to the next message. */
Sint32 fdMessageShowAtActor(Sint32 speaker, Sint32 unused);
/* Opens the script's current message at the speaker and waits for its window;
   after ten seconds, or with Select, R, L and A held, the text is finished at
   once. */
void fdMessageShowAtActorAndWait(Sint32 speaker, Sint32 flg);
/* The script command "show message and wait": shows the message, then waits the
   given number of ticks. */
void fdScriptShowMessageThenWait(Sint32 speaker, Sint32 flg, Sint32 ticks);
/* The script command "show message": opens the current message at the speaker
   (fdMessageShowAtActor) and waits until it has been read and its window has
   closed; the speaker's portrait is closed once the text is typed out. */
void fdScriptShowMessageAndWait(Sint32 speaker, Sint32 flg);
/* The script command "ask yes or no": opens the question at the speaker, lets
   the leader answer, then shows the next message for yes or the one after it
   for no. */
Sint32 fdScriptAskYesNoAndShowResponse(Sint32 speaker, Sint32 flg);
/* An empty hook; it does nothing in the released game. */
void fdScriptNoOperationUpdate(void);
/* Unused: nothing calls it. It would record the speaker, open the message near
   it and wait unless fast-forwarding, then advance the message index. It places
   the message as fdScriptShowMessageAtPos does, before passing the position to
   a window function that takes tiles; with no caller, the units it expected
   can't be told. */
void fdScriptShowSpeakerMessage(Sint32 speaker);
/* Unused: nothing calls it; forwards its arguments to
   fdScriptShowMessageAtPos. */
void fdScriptForwardShowMessageAtPos(
	Sint32 unusedA, Sint32 unusedB, Sint32 x, Sint32 y);
/* Unused: nothing calls it. It would open the script's message at (x, y), wait
   for it and advance the message index; its units are as unknown as
   fdScriptShowSpeakerMessage's. */
void fdScriptShowMessageAtPos(
	Sint32 unusedA, Sint32 unusedB, Sint32 x, Sint32 y);
/* The script command "actor face direction": the actor turns toward angle
   (TRIG_TURN a full turn; fdActorStepAng), then the script waits the given
   number of ticks. */
void fdActorFaceDirectionById(Sint32 actorId, Sint32 ang, Sint32 ticks);
/* Turns an actor a step toward angle (ActorStepAngleScript) and waits for the
   script to end. */
void fdActorFaceDirectionAndRefreshById(Sint32 actorId, Sint32 ang);
/* Clears an actor's destination, starts its idle script and stands it still. */
void fdActorClearDestinationAndStand(Uint32 actorId);
/* Closes the portrait of an actor's sprite, if it has one. */
void fdActorClosePortrait(Sint32 actorId);
/* The script command "show two messages and wait": two actors speak at once,
   each message at its own (x, y) with its portrait at (portraitX, portraitY),
   all in tiles. When delay is nonzero, the second message waits for the first
   to finish, checking every delay ticks. Waits for A, B, L or R, then closes
   both. */
void fdScriptShowTwoMessagesAndWait(Sint32 first, Sint32 firstX, Sint32 firstY,
	Sint32 firstPortraitX, Sint32 firstPortraitY, Sint32 second, Sint32 secondX,
	Sint32 secondY, Sint32 secondPortraitX, Sint32 secondPortraitY,
	Sint32 delay);
/* A message's voice from its speaker, and a message shown at a place. */
void fdMessageSetVoiceFromActor(Sint32 key);
/* The script command "set actor sprite priority": fixes the priority of the
   actor's sprite and its shadow, which then no longer follow its depth: clears
   ACTOR_PRIORITY_AUTOMATIC. */
void fdActorSetSprPriority(Uint32 actorId, Sint32 pri);
/* Stops actor id moving and colliding and hides it. */
void fdActorDisableById(Uint32 id);
/* The script command "actor show emote": shows an emote bubble (SPRITE_EMOTE)
   over an actor, then waits the given number of ticks. Returns the bubble. */
struct ACTOR *fdActorShowEmoteById(Sint32 actorId, Sint32 emote, Sint32 ticks);
/* The script command "actor set attached effect", by ATTACHED_EFFECT_FLAG:
   removes the actor attached to an actor, attaches one (SPRITE_RAY_ARC,
   animation 1) unless one already is, or shows a one-off one (animation 2) that
   ends by itself (ActorWaitScript). */
void fdEffectSetAttached(Sint32 actorId, Sint32 flg);
/* Makes the target actor's sprite show the source actor's graphics (its VRAM
   block and tile). Neither actor is checked for NULL. */
void fdActorCopyGraphics(Uint32 tgtId, Uint32 srcId);
/* The script command "camera follow actor": the camera follows the actor, put
   on it at once unless keepPos leaves it where it is to set out from. */
void fdCameraFollowActor(Sint32 actorId, Sint32 keepPos);
/* The script command "camera set speed": the camera's top speed and
   acceleration, 16.16 pixels per tick. */
void fdCameraSetSpeedParms(FIXED speed, FIXED acceleration);
/* The script command "camera move to": sends the camera to the requested axes,
   16.16 pixels (-1 keeps that axis), kept far enough inside the map that the
   screen never shows past its edge. With pan 0 the camera cuts there at once;
   otherwise it pans there at its own speed. */
void fdCameraMoveWithinMapBounds(
	FIXED requestedX, FIXED requestedY, FIXED requestedZ, Sint32 pan);
/* The script command "camera wait for move": waits (at most 600 ticks) for the
   camera to arrive, then 2 ticks more. */
void fdCameraWaitForArrival(void);
/* The script command "camera move to actor": fdCameraMoveWithinMapBounds to the
   actor's x and z. */
void fdCameraMoveToActorById(Sint32 actorId, Sint32 pan);
/* As fdCameraMoveToActorById, to the actor's height too. */
void fdCameraMoveToActor(Sint32 actorId, Sint32 pan);
/* The script command "get camera": returns the camera's own actor, which the
   view centres on. */
struct ACTOR *fdCameraGetActor(void);
/* Sets the map coordinates the camera keeps the screen inside. */
void fdMapGridSetCameraBounds(
	Sint32 minX, Sint32 minZ, Sint32 maxX, Sint32 maxZ);
/* Makes the script leave for the given entrance of the given scene: raises the
   exit request EXIT_KEEP_DESTINATION and writes MainRoutineStateWork.next and
   entrance. */
void fdScriptSetDestination(Sint32 sceneId, Sint32 entrance);
/* Makes the script leave and records sceneId and entrance as the restart
   point. */
void fdScriptSetRestartPoint(Sint32 sceneId, Sint32 entrance);
/* The script command "request exit": sets the field's exit request
   (EXIT_KEEP_DESTINATION leaves for MainRoutineStateWork's next and
   entrance). */
void fdScriptSetExitRequest(Uint16 exit);
/* Sets up a battle with the formation at row and column of the encounter
   table. */
void fdRequestBattleFromEncounterTable(Sint32 row, Sint32 column);
/* Sets up the battle with Djinni djinn, DJINN_PER_ELEMENT to an element; an
   element's last Djinni stands for the one of The Broken Seal's chosen for it
   (SCENE_STATE_CHOSEN_DJINN; none: nothing happens). The low 8 bits of flags
   are the Djinni's actor, which hops first unless DJINN_NO_HOP is set. */
void fdRequestDjinnBattle(Uint32 flg, Uint32 djinn);
/* Sets the after-battle destination (MainRoutineStateWork.afterBattleScene and
   afterBattleEntrance), where the party goes once the coming battle is won. */
void fdScriptSetAfterBattleDestination(Sint32 sceneId, Sint32 entrance);
/* Sets the defeat destination (MainRoutineStateWork.defeatScene and
   defeatEntrance), where the party goes if it loses the coming battle. */
void fdScriptSetDefeatDestination(Sint32 sceneId, Sint32 entrance);
/* Makes the script leave for entrance entrance of scene sceneId: sets
   MainRoutineStateWork.next and entrance and raises the exit request
   EXIT_KEEP_DESTINATION, so the field mode leaves on its next pass. */
void fdScriptLeaveFor(Uint16 sceneId, Uint16 entrance);
/* Writes the actor's position on screen, in pixels, to position[0] and
   position[1]; an actor with a sprite is raised by its tick box. Returns -1
   when the actor does not exist. */
Sint32 fdActorGetScreenPos(Sint32 actorId, Sint32 *pos);
/* Returns the metatile behavior of the map metatile 16 pixels ahead of the
   party leader when there is something to search there: a fixed search spot
   (METATILE_BEHAVIOR_JAR to METATILE_BEHAVIOR_WELL) whose ground is level with
   the leader or up to 16 pixels above it, or a behavior the scene has an
   examine cue for; aboard the ship on the World Map, SHIP_SEA_EXAMINE_CODE
   before sea of collision 3. Returns 0 otherwise. */
Sint32 fdActorGetCueMetatileAheadOfCurrent(void);
/* Picks the sprite a scene placement's character id shows: the ship
   (PLACEMENT_SHIP) is SPRITE_WINGED_SHIP_SMALL on the world map and
   SPRITE_WINGED_SHIP elsewhere, each one's flying picture once FLAG_SHIP_FLIES
   is set. */
Sint32 fdPartyGetCharacterSpr(Sint32 character);
/* Picks the actor nearest actor id, under 32 pixels away and 48 pixels in
   height. Distances to treasure spots count double and to actors with
   ACTOR_NEAR_TALK ten thirteenths; beyond 11 pixels the actor must also lie
   within three sixteenths of a turn of id's facing. Returns -1 when none
   qualifies. */
Sint32 fdFindNearbyTalkTarget(Sint32 id);
/* When Move is cast on a target, the pushing hand appears beside it and pushes
   it 16 pixels the way the d-pad points, for as long as the player pushes; A,
   B, L or R end it. */
void fdPsynergySteerPushedTarget(Sint32 unused);
/* Cloak the caster: flicker it CLOAK_BLINKS times, leave it half transparent,
   then check the ground under it every tick. When it cannot hide where it
   stands, show it again at once. */
void fdPsynergyStartCloak(void);
/* Returns the actor nearest in front of actor self that field Psynergy kind can
   reach, or -1. */
Sint32 fdFindNearbyPsynergyTarget(Sint32 self, Sint32 kind);
/* The leader jumps two metatiles forward, over the metatile in front, when it
   faces a quarter turn, the map allows it (fdMapGridBlocksJump and the step
   check) or a platform actor (ACTOR_PRIORITY_PLATFORM) waits 16 pixels below
   the landing, and no solid actor stands there. A log underfoot (a
   SPRITE_LOG_WEST_EAST or SPRITE_LOG_NORTH_SOUTH under the party) dips 2 pixels
   with the leader. Each jump adds to the walking distance, and the leader keeps
   jumping while it lands on METATILE_BEHAVIOR_JUMP_FORWARD. Returns 0 after a
   jump, -1 when the leader cannot jump. */
Sint32 fdTryJumpForward(void);
/* Climbs down onto the wall below the leader, as The Broken Seal's fdClimbDown
   does, or lets go and falls when it cannot. */
Sint32 fdClimbDown(void);
/* Climbs up the wall in front of the leader, as The Broken Seal's fdClimbUp
   does. */
Sint32 fdClimbUp(void);
/* Stops the leader where it stands: no horizontal velocity, the normal walking
   speed (1.0) and acceleration (0.5), no destination, and the standing pose, or
   ANIM_LEADER_CLING while the leader is on a climbable wall. */
void fdLeaderStopMoving(void);
/* Unused: nothing calls fdGroundParticlesStartStub. The Broken Seal's rain,
   kept whole where this game has its own fdWeatherStartRain: 64 sprites placed
   at the leader's position on the terrain, their tiles decoded into VRAM, the
   blend set, and fdGroundParticlesUpdate scheduled to move them. */
void fdGroundParticlesStart(void);
/* Starts the storm's lightning (fdWeatherUpdateLightning): builds the dark
   (PALETTE_TINT_WARM) and lit (PALETTE_TINT_PALE) BG colours and their 12-step
   fade, and arms the first burst to end in 600 ticks, repeating. */
void fdWeatherStartLightning(void);
/* Stops the lightning: no flash for 32767 ticks, and none after. */
void fdWeatherStopLightning(void);
/* One last burst of lightning, its first flash two ticks from now (timer 12;
   fdWeatherUpdateLightning flashes at 11), then no more. */
void fdWeatherLastLightning(void);
/* Restarts the lightning: the next burst ends in 128 ticks, and bursts
   repeat. */
void fdWeatherResumeLightning(void);
/* Flashes the palette with the storm's event: one burst of three flashes of
   palette to, each fading back to palette from (fdPaletteFadeBuild modes) on
   the BG colours, ending 120 ticks from now, and no more after. */
void fdWeatherStartLightningFlash(Sint32 from, Sint32 to);
/* Starts the rising particles, as the Northern Reaches does: allocates and
   clears heap slot HEAP_SLOT_WEATHER, decodes the particle tiles into VRAM,
   sets each of the 64 particles' sprite words and places it on the ground at a
   random point of the strip (x 288 to 488 pixels, z 60 to 80), staggering their
   timers by 2 ticks, and schedules the update. */
void fdRisingParticlesStart(void);
/* Unused: nothing calls fdEffectInitSparklesStub; Aqua Rock and the Apojii
   Islands keep their own copy (shared/ground_particles_start.h). Starts the
   sparkles (fdEffectUpdateSparkles): loads their tiles and lays every sparkle
   at the map's top left corner with a staggered timer. */
void fdEffectInitSparkles(void);
/* Starts window transition mode at value 0, with no duration: the caller moves
   it. */
void fdScreenTransitionStartWindowHeld(Sint32 mode);
/* Starts a dissolve of the given mode, its value held at 0, and waits two
   seconds. */
void fdScreenTransitionStartDissolveHeld(Sint32 mode);
/* Fades the screen in over ticks ticks. mode is a SCENE_TRANSITION: its high
   byte an SCREEN_TRANSITION_STYLE (from black, from the backdrop colour, a
   window transition whose low byte is an WINDOW_TRANSITION_MODE, a dissolve, or
   opening from the centre line, which a non-zero low byte makes flicker). */
void fdScreenTransitionStart(Sint32 mode, Sint32 ticks);
/* Fades the screen out over ticks ticks, as fdScreenTransitionStart fades it
   in. */
void fdScreenTransitionFinish(Sint32 mode, Sint32 ticks);
/* Clears the map's display-control bits for backgrounds 3, 2 and 1
   (DISPCNT_BG3_ON, DISPCNT_BG2_ON and DISPCNT_BG1_ON) for each argument that is
   set. */
void fdScreenTransitionHideBackgrounds(
	Sint32 clearBg3, Sint32 clearBg2, Sint32 clearBg1);
/* Talks to actor id. A container (actors from FIELD_TREASURE_ACTOR, and the
   Scoop spot, SCOOPED_ITEM_ACTOR) is checked instead: a chest gives what it
   holds, a Mimic starts its battle, a Psynergy Stone restores the party's PP
   and a stone tablet teaches its summon. Any other actor shows its script's
   message or calls its script function, stopped and turned as its placement
   says; the actor Halt has stopped answers its CUE_TALK_HALTED cue first. */
Sint32 fdTalkToActor(Sint32 id);
/* The palette fades: their steps, their channels kept in range, and the
   backdrop colour. */
void fdPaletteFadeInit(void);
/* Sets the colours the palette fade moves toward (see fdPaletteFadeBuild); does
   nothing without a palette fade. */
void fdPaletteFadeSetTarget(Sint32 mode, Sint32 half);
/* Sets the colours the palette fade shows now (see fdPaletteFadeBuild). */
void fdPaletteFadeSetSource(Sint32 mode, Sint32 half);
/* Sets each colour channel of step to the amount to add each tick for source
   to reach target in ticks ticks; with ticks 0 or less step is left as it
   was. */
void fdPaletteFadeCalcSteps(
	Sint16 *src, Sint16 *tgt, Sint16 *step, Sint32 ticks);
/* Starts the fade from the shown colours to the target colours, over ticks
   ticks. */
void fdPaletteFadeStartInterpolation(Sint32 ticks);
/* Stops the palette fade and frees it. */
void fdPaletteFadeRelease(void);
/* Sets the backdrop colour (BG colour 0) of the palette fade's copy of the
   palettes, from which it builds its colours. */
void fdPaletteFadeSetBackdrop(Uint32 col);
/* The script command "open screen": starts the script's screen transition and
   sets transitionActive. */
void fdScriptStartScreenTransition(void);
/* The script command "close screen": finishes the script's screen transition
   and clears transitionActive. */
void fdScriptFinishScreenTransition(void);
/* The script command "wait for screen": waits as long as the transition runs
   (transitionFrm). */
void fdScriptWaitTransitionTicks(void);
/* Starts the line scroll: its state (heap slot HEAP_SLOT_LINE_SCROLL) holds the
   mode and, for x and y, how fast the wave runs from tick to tick, its step
   from one scanline to the next and its amplitude; two events, one in the
   vertical blank pass and one in the tick pass, build and send each tick's
   line offsets. */
void fdLineScrollInitWave(Sint32 mode, Sint32 freqX, Sint32 stepX, Sint32 ampX,
	Sint32 freqY, Sint32 stepY, Sint32 ampY);
/* Stops the line scroll: removes its two events and stops DMA 0. */
void fdLineScrollStop(void);
/* Unused: only the stub table refers to it. Opens a chest: if actor target
   exists, plays SE_CHEST_OPEN, opens its lid and, 12 ticks later, shows itemId
   in front of actor actorId with sparkles. */
Sint32 fdChestOpenAndShowItem(Sint32 actorId, Sint32 tgt, Sint32 itemId);
/* Shows a found item's picture as flags choose (ITEM_DISPLAY_FLAG), with waits
   of 60 and 80 ticks around its raising when both are set. */
void fdItemDispShowPicture(struct ACTOR *actor, Sint32 flg);
/* Shows the picture of item 36 pixels above the party leader for 80 ticks, as
   flags choose (ITEM_DISPLAY_FLAG). */
void fdItemDispShowItem(Sint32 item, Sint32 flg);
/* Returns the actor, or NULL. */
struct ACTOR *fdTreasureSpotGetActor(Uint32 actorId);
/* Opens the chest of the actor with SE_CHEST_OPEN, and waits 12 ticks. */
void fdTreasureSpotOpenChest(Uint32 actorId);
/* Closes the chest of the actor again (its item could not be carried), with
   SE_CHEST_CLOSE, and waits 12 ticks. */
void fdTreasureSpotCloseChest(Uint32 actorId);
/* Shows the chest of the actor open, with no sound. */
void fdTreasureSpotShowOpened(Uint32 actorId);
/* Moves the actor to (x, z) on the ground, 16.16 pixels; -1 keeps its home
   (homeX, homeZ) on that axis. */
void fdTreasureSpotSetActorPos(Uint32 actorId, FIXED x, FIXED z);
/* Releases the actor and empties its slot. */
void fdTreasureSpotRemove(Uint32 actorId);
/* Unused: only the stub table refers to it. Keeps the party leader off the
   treasure spots: a leader within 16 pixels of a treasure spot's actor is moved
   20 pixels from it, in the direction it stood, and loses its move target. */
void fdTreasureSpotMoveLeaderAway(void);
/* TREASURE_SPOT_PUT_AWAY moves the actor to x and z 0 and marks it with
   sceneWord TREASURE_SPOT_ON_METATILE; any other mode draws its sprite wide, 16
   by 8 pixels, 4 pixels lower. */
void fdActorSetWideSpr(Uint32 actorId, Sint32 mode);
/* RETURN_HOME_CLEAR_Z moves the actor to z 0; RETURN_HOME_DUG_UP sets flag,
   draws a moved actor square again and drops it back home (homeX, homeZ) at
   Scoop's anchor. */
void fdActorReturnHome(Sint32 mode, Uint32 actorId, Sint32 flg);
/* On the World Map, with no area graphics already pending: marks them pending,
   pauses the camera's actor and, when a screen transition is running, ends it
   at once and waits 2 ticks; then shows the still image
   (fdWorldMapStartStillImageStub). */
void fdWorldMapFreeze(void);
/* On the World Map, loads the area graphics fdWorldMapFreeze left pending and
   lets the camera's actor move again. */
void fdWorldMapReloadPendingAreaGraphics(void);
/* Creates a scene's actors on entering it, as The Broken Seal's
   fdActorResetForLeader does: the party leader where the main routine's state
   puts it, the scene's placement table from actor slot 8, and the camera that
   follows the leader; it also chooses the leader's control mode. */
void fdActorResetForLeader(Sint32 tbl);
/* Creates the actors of a scene's actor placement table (ended by sprite -1),
   and records the table among the scene's tables. */
void fdScriptSpawnActorTable(struct ACTOR_PLACE *entry, Sint32 slot);
/* Releases the actor in slot actorIdx of the field state's table, if there is
   one, and empties the slot. */
void fdActorReleaseAtIndex(Uint32 actorIdx);
/* Returns the actor in slot actorId (below LOOKUP_ID_COUNT), or NULL. */
struct ACTOR *fdActorLookup(Uint32 actorId);
/* Looks point id up in the map's scene points (its scene_points.tsv, ended by
   id 0xff) and writes its x and z, in 16.16 pixels, to position; returns 0, or
   -1 when the map has no such point. */
Sint32 fdScenePointGetPos(Sint32 id, FIXED *pos);
/* Starts the fade overlay on an actor (the party leader when NULL): loads its
   solid tiles, sets the blending and draws it each tick. Unused: nothing calls
   it or its stub call, fdEffectStartFadeOverlayStub. */
void fdEffectStartFadeOverlay(void *actor);
/* Stops drawing the fade overlay, keeping its state. */
void fdFadeOverlaySuspend(void);
/* Draws the fade overlay again after fdFadeOverlaySuspend. */
void fdFadeOverlayResume(void);
/* Like fdEffectStartFadeOverlay with blank tiles and no blending:
   loads 512 bytes of blank tiles into VRAM_CACHE_ENTRY_FADE_OVERLAY and starts
   fdBlankOverlayDrawMarks on an actor (the party leader when 0). Aqua Rock's
   rooms start it; its marks are all colour 0, so nothing shows. */
void fdEffectStartBlankOverlay(Sint32 actor);
/* Suspends the fdBlankOverlayDrawMarks overlay event. */
void fdBlankOverlaySuspend(void);
/* Resumes the fdBlankOverlayDrawMarks overlay event. */
void fdBlankOverlayResume(void);
/* Runs one step of the scene's field Psynergy: cast with no target, cast on the
   chosen target (secondActorId), Mind Read's end, or what a lasting Psynergy
   does when it ends. */
void fdPsynergyRunHandler(Sint32 kind);
/* Prepares the state of the field Psynergy for ability (heap slot
   fieldEffState): its kind, the free VRAM it starts with, the map's shake to
   restore, and whether the place is lit. While another field Psynergy is
   resolving, only the ability and kind are set. */
void fdPsynergyLoadResources(Sint32 abilityId, Sint32 mode);
/* Records the caster and the target of a field Psynergy. */
void fdPsynergySetupActorPair(Sint32 userId, Sint32 tgtId);
/* Takes the caster out of its casting update and waits a tick. */
void fdBattleEffectStopMainActor(void);
/* Ends a field Psynergy: frees the field effect sprites still in use and,
   unless another is being resolved, waits for the swelling arcs to end,
   restores the map's shake and releases the state. */
void fdPsynergyEnd(void);
/* The field Psynergy, each cast as the script commands run it, with the
   functions that end a lasting one, act on a target or resume it. */
void fdPsynergyStartCast(void);
/* Tints the field for a Psynergy animation: a tint chosen by the tick counter
   (so each cast looks a little different), fixed for some animations, and
   forced by the tint flags; then fades towards it over eight ticks. The
   palettes are copied by fdStartColTransition. */
void fdPsynergyFadeToCastCol(void);
/* Fades the palettes back from a cast's colours to the saved ones over 8
   ticks; while Reveal is active the BG half fades to Reveal's grey instead
   (PALETTE_TINT_GRAYSCALE). */
void fdPsynergyFadeFromCastCol(void);
/* Ends every lasting Psynergy (fdScriptEndLastingPsynergy with none
   skipped). */
void fdScriptEndAllLastingPsynergy(void);
/* Ends the lasting Psynergy and runs their end scripts: Reveal, the ring of
   light and Sand (while the party is sand, MainRoutineStateWork.sandActive),
   each unless skip holds its bit (PSYNERGY_END_SKIP). */
void fdScriptEndLastingPsynergy(Sint32 skip);
/* Starts Mind Read's wave between the caster and the one it reads
   (HEAP_SLOT_MIND_READ): a hue taken from the tick count, the field faded
   toward it, the message window greyed for thoughts, and
   fdPsynergyUpdateMindReadWave run each tick. */
void fdPsynergyStartMindReadWave(Sint32 casterId, Sint32 tgtId);
/* Stops the wave effect: restores the caster's colours and the palettes and
   releases the Psynergy's state. */
void fdPsynergyStopMindReadWave(void);
/* Greys the message window's white and most of its other colours
   (MIND_READ_GREY), so messages read as thoughts, and starts the BG0 line
   scroll (fdMindReadStartRipple) at each VBlank. */
void fdPsynergyGreyMindReadPalette(void);
/* Stops the BG0 line scroll and restores WINDOW_PALETTE. */
void fdPsynergyRestoreMindReadPalette(void);
/* Returns whether the scene has a cue for the field Psynergy kind on what it
   would act on (fdSelectPsynergyTarget), at any stage. */
Bool fdHasPsynergyCue(Sint32 fieldPsynergy);
/* Returns whether the scene has a script for item and what the leader faces. */
Uint32 fdHasItemCue(Uint16 item);
/* Makes srcSpr share sharedSpr's VRAM block, releasing its own; with
   no sharedSpr it keeps its block. */
void *fdSprShareVRAMCacheEntry(void *srcSpr, void *sharedSpr);
/* Returns the battle background chosen for the current scene. */
Sint16 fdBattleBackgroundGetSelected(void);
/* Move's hand appears at a quarter size 16 pixels above the caster, with a
   crack and eight sparkles that scatter from it, and sets out 16 pixels the way
   the caster faces, to the nearest quarter turn. */
void *fdEffectSpawnMoveHand(void *object);
/* Grows a hand to full size at once and waits for it to reach where it is
   going. */
void fdEffectGrowHandAndWait(struct ACTOR *actor);
/* The hand spins as it rises for BURST_RISE_TICKS, shrinks, and bursts into
   BURST_SPARKLES sparkles (SPRITE_SPARKLE). */
void fdEffectBurstHand(void *object);
/* Gives each character in the party amount PP (PP regenerates as the party
   walks). */
void fdPartyAddPp(Sint32 amount, Sint32 unused);
/* Changes each party member's HP by amount, or by that percentage of maximum HP
   when scaled is set; a loss flashes the screen orange. */
void fdPartyAddHp(Sint32 amount, Sint32 scaled);
/* Counts the free blocks of the sprite VRAM cache. */
Sint32 fdResourceTableCountFreeBlocks(void);
/* Asks yes or no at the top of the screen, or at the bottom with atBottom set.
   Returns 0 for yes, 1 for no. */
Sint32 fdYesNoAsk(Sint32 atBottom);
/* Returns the formation in slot (0 to 7) of zone. */
Uint16 fdEncounterGetFormationAtSlot(Sint32 zone, Sint32 slot);
/* Returns the formation of Djinni battle djinni, which the table gives as a
   zone and slot. */
Uint16 fdEncounterGetDjinnFormation(Sint32 djinni);
/* Starts the current scene's field music, chosen by fdChooseMusic. */
void gmPlayMusic(void);
/* Chooses the current scene's field music (MainRoutineStateWork.snd) from the
   first rule matching the scene or its area and the entrance; SOUND_KEEP_BGM
   when none does. */
void fdChooseMusic(void);
/* A Djinni joins. The element's last Djinni stands for the one of The Broken
   Seal's chosen for it (SCENE_STATE_CHOSEN_DJINN; none: nothing happens). While
   FLAG_DJINN_LESSON is set it joins the leader by its flag alone, otherwise the
   party member with the fewest; if it found one, the element's join effect
   plays at the actor (none for actor -1), the Djinni is added, and the actor's
   script slot is cleared (in every edition but the Japanese; the Japanese
   release leaves it set). */
void fdDjinnJoinParty(Sint32 actor, Sint32 element, Sint32 djinni);
/* Updates one field effect sprite: counts its age, runs its callback (after any
   delay), moves it and draws it. */
void fdParticleUpdate(FIELD_PARTICLE *effect);
/* Reports an unfinished target only for effects that stop on arrival. */
Uint32 fdParticleHasPendingTarget(FIELD_PARTICLE *effect);
/* Places a field effect sprite at (x, z), at rest with no target. */
void fdParticleSetPos(FIELD_PARTICLE *effect, Sint32 x, Sint32 z);
/* Sets the animation of a field effect sprite. */
void fdParticleSetAnimation(FIELD_PARTICLE *effect, Sint32 mode);
/* Gives a field effect sprite its per-tick callback, starting at state 0. */
void fdParticleSetCallback(FIELD_PARTICLE *effect, EFFECT_CALLBACK func);
/* Starts a field particle with a sprite of the given kind at (x, z), at full
   size and in front of everything: it will fly to its target at up to 2 pixels
   a tick, turning a limited step at a time, and stop there. */
void fdParticleInit(FIELD_PARTICLE *effect, Sint32 kind, Sint32 x, Sint32 z);
/* Frees a field effect sprite and its sprite, clearing it to zero
   (inactive). */
void fdEffectClearOwnedSlot(FIELD_PARTICLE *effect);
/* Converts a world position (16.16 pixels) to the screen: x and z receive the
   screen's x and y and y becomes 0. */
void fdCameraWorldToScreen(struct VECTOR3 *pos);
/* Returns the voice of sprite id's messages: the first of its four blip sounds,
   SE_VOICE_FIRST plus its definition's value, or 0 for none
   (SPEAKER_VOICE_NONE) or with the Speech option off
   (MainRoutineStateWork.speech). mnMessageOpenWindowStub takes it in the high
   half of its last argument. */
Sint32 fdSpeakerGetVoice(Sint32 id);
/* Clears each button shortcut whose Psynergy can no longer be cast: zeroes
   MainRoutineStateWork.firstShortcut or secondShortcut. */
void fdPartyClearUnusableShortcuts(void);
/* Allocates and clears the field effect sprites (HEAP_SLOT_FIELD_EFFECT) and
   starts their update event. */
void fdEffectInitSlots(void);
/* Stops the field effect sprites: removes their event, frees every slot still
   in use and releases their heap block, the field Psynergy's state with it. */
void fdEffectReleaseSlots(void);
/* Places an actor on the leader, a pixel nearer the back, and sends it walking
   (xOfs, zOfs) pixels away at speed 0.8 and acceleration 0.4, through the
   launch script; the actor then takes movement type action. */
void fdActorLaunchFromLeader(
	Sint32 actorId, Sint32 xOfs, Sint32 zOfs, Sint32 action);
/* Walks the actor (xOfs, zOfs) pixels from where it stands, through the
   offset script. */
void fdActorWalkByById(Uint32 actorId, Sint32 xOfs, Sint32 zOfs);
/* Walks the actor (xOfs, zOfs) pixels, waits for it to arrive and stands
   it still. */
void fdActorWalkByAndWaitById(Uint32 actorId, Sint32 xOfs, Sint32 zOfs);
/* Makes an actor follow another (the watched actor script): writes its
   watchedActor. */
void fdActorWatchAndSetScript(Sint32 actorId, Sint32 watchedActorId);
/* Holds an actor still under Halt, keeping its per-tick update and its first
   animation's colour mode in MainRoutineStateWork for
   fdPsynergyResumeHaltedActor. */
void fdPsynergyHaltActor(Sint32 actorId);
/* Lets a halted actor move again, with the update and colour mode kept by
   fdPsynergyHaltActor. */
void fdPsynergyResumeHaltedActor(Sint32 actorId);
/* Returns the first of the scene's placed actors (from ACTOR_FIRST_PLACED)
   showing sprId, or -1. */
Sint32 fdActorFindBySpr(Sint32 sprId);
/* Returns the sprite (the first animation's id) of the actor whose id is key's
   under SPEAKER_ACTOR_MASK, or -1 if there is no such animated actor. */
Sint32 fdActorGetSprById(Sint32 key);
/* Saves up to 32 actors into SnapshotObjects, with each one's slot, animation,
   sprite flags and priority, so that they can be put back. */
void fdActorTableSnapshot(void);
/* Puts back the actors the snapshot saved, and moves the camera to the party
   leader's height. */
void fdActorRestore(void);
/* Starts a zoom to zoom (16.16) over ticks ticks, on the World Map only:
   fdWorldMapStepZoom takes the view's MAP_STATE.zoom to farPlane / zoom. */
void fdWorldMapStartZoom(FIXED zoom, Sint32 ticks);
/* Waits for a zoom transition to end (on the World Map only, at most 300
   ticks). */
void fdScriptWaitForZoomTransition(void);
/* The area name number for `scene` entered at `entrance`, as the save files and
   the world map show it. */
Sint32 fdAreaGetNameIndex(Sint32 sceneId, Sint32 entrance);
/* Unused: nothing calls fdPairTableFindStub. Looks key up in PairTable's {key,
   value} rows and returns its value, or -1; the table holds only its end, so
   every key gets -1. */
Sint32 fdPairTableFind(Sint32 key);
/* Unused: nothing calls fdPairTableFindAndIgnoreStub. Looks key up as
   fdPairTableFind does and returns 0. */
Sint32 fdPairTableFindAndIgnore(Sint32 key);
/* Unused: nothing calls fdPairTableFindCopyStub. fdPairTableFind again. */
Sint32 fdPairTableFindCopy(Sint32 key);
/* Unused: only the stub table refers to it. Draws heading picture `heading` of
   the headings resource into a 26 by 5 metatile window and opens the window
   from the centre. */
void fdHeadingShow(Sint32 heading);
/* Waits up to two seconds, stopping early when a button is pressed. */
void fdScriptWaitTwoSecondsOrPress(void);
/* Unused: only the stub table refers to it. Closes fdHeadingShow's picture
   window: fades the palette out while wiping every one of the picture's 26 by 5
   tiles from its top left corner to its bottom right, a diagonal every two
   ticks (HeadingWipeSteps: a tile's pixels a diagonal at a time, each ended
   by a negative entry and the last by HEADING_WIPE_END), then turns BG0 and the
   window off, clears the screen block, reloads the window tiles and turns BG0
   back on. */
void fdHeadingClose(void);
/* Starts an actor's movement type: an MOVEMENT_TYPE, or any other value as the
   actor script to run itself. */
void fdActorSetMovementType(struct ACTOR *actor, Sint32 kind);
/* Plays SE_STOP_EXIT_SE, then sndId (SOUND_FADE_OUT_ACTION_SE for -1), and
   remembers sndId for fdPlayQueuedMusic in the field state's queuedSnd; -1 is
   remembered as none, so it plays SOUND_FADE_OUT_ACTION_SE now and nothing
   later. */
void fdQueueMusic(Sint32 sndId);
/* A script that does nothing. */
void fdScriptNoOperation(void);
/* Plays the sound fdQueueMusic remembered, if any. */
void fdPlayQueuedMusic(void);
/* Does nothing; nothing calls fdActorReservedNoOperationStub. */
void fdActorReservedNoOperation(void);
/* The ropes Lash ties between two posts, given by their actor ids: placing,
   tying, coiling and swinging them, laying their nodes and drawing them. */
void fdRopeInit(void);
/* Lays rope index tied between posts id and other, and shows both posts with
   the rope wound round them. A unit whose ROM code is compiled against another
   form defines OWN_fdRopeTie and declares that form itself. */
#ifndef OWN_fdRopeTie
void fdRopeTie(Sint32 idx, Sint32 id, Sint32 other);
#endif
/* Coils rope index at post firstId, its length reaching post secondId: the rope
   as it waits to be lashed. */
void fdRopeCoil(Sint32 idx, Sint32 firstId, Sint32 secondId);
/* Lash: throws rope index from post fromId to post toId and winds it round
   both. A rope coiled too short to reach falls back to its post. A unit whose
   ROM code is compiled against another form defines OWN_fdRopeLash and declares
   that form itself. */
#ifndef OWN_fdRopeLash
void fdRopeLash(Sint32 idx, Sint32 fromId, Sint32 toId);
#endif
/* Unties rope index between posts fromId and toId (fdRopeRecoil). */
void fdRopeUntie(Sint32 idx, Sint32 fromId, Sint32 toId);
/* Allocates the tightropes' state, loads their node picture and starts
   fdTightropeDraw and fdTightropeUpdate. */
void fdTightropeInit(void);
/* Lays tightrope slotNo straight between posts firstId and secondId, and shows
   the rope wound round both. Declared Sint32, as the scenes call it, though it
   returns no value; no caller reads one. */
Sint32 fdTightropePlace(Sint32 slotNo, Sint32 firstId, Sint32 secondId);
/* Returns rope index's ROPE_SLOT_STATE, or -1 while the ropes have no state. */
Sint32 fdRopeGetState(Sint32 idx);
/* Gives rope index its flag and places it between posts firstId and secondId:
   tied when the flag is set, else coiled at the first. A scene places each of
   its ropes with it. */
void fdRopePlace(Sint32 idx, Sint32 flg, Sint32 firstId, Sint32 secondId);
/* Returns the flag entry table, 20-byte entries ended by a zero flag. */
const struct WORLD_MAP_PLACE *fdWorldMapPlacesGetTable(void);
/* Sets up a scene's flag blocks, the puddles Frost turns into pillars of ice
   (actor and flag pairs, ended by ACTOR_LIST_END): each stops being solid and
   blocks its metatile; one whose flag is set shows the ice pillar and has the
   flag cleared, the others the puddle. Then fdFlagBlockApplyFlags raises the
   frozen ones and sets their flags again, and the leader is lifted onto the
   ground if it now lies below it. A scene with such puddles calls it as it
   starts. */
void fdFlagBlockPlaceAll(struct ACTOR_FLAG *actors);
/* Sets up Alhafra's and the Alhafran Cave's puddles as fdFlagBlockPlaceAll
   does, and marks the open metatiles on each puddle's four sides a gap to jump
   across (fdMapGridMarkJumpIfOpen), so the leader jumps onto a frozen one. */
void fdFlagBlockPlaceAllMarkingEdges(struct ACTOR_FLAG *list);
/* For each flag block whose animation disagrees with its flag, turns its
   metatile and flag over: with the flag set and the ice pillar not showing, the
   metatile is lowered 32 pixels to a wall and the flag cleared; with the flag
   clear and the puddle not showing (Frost freezing it), the metatile is raised
   32 pixels, open and a gap to jump across, and the flag set. The field state
   remembers the block. */
void fdFlagBlockApplyFlags(struct ACTOR_FLAG *list);
/* The rest of what the script commands share: actors, metatiles, sound and the
   party. */
Sint32 fdActorGetMetatileIndex(struct ACTOR *actor);
/* Makes each listed actor (ids, ended by ACTOR_LIST_END) a block in the map:
   its metatile's shape is tagged BLOCK_HEIGHT_STEPS above the ground, so the
   party can stand on it, its metatile is marked MAP_METATILE_JUMP, a gap to
   jump onto from another block (the old mark kept in the actor), and the
   overhead layer's metatile is a wall. A leader below the new ground is lifted
   onto it. Scenes call it as they start, for the blocks Move pushes. */
void fdActorPlaceBlocks(Uint16 *actors);
/* Takes the block about to move out of the map: the field effect's target while
   one runs, else the actor ahead of the leader, if it is one of the listed
   actors. Its metatile goes back to the ground and its old mark, and the script
   state remembers it for fdActorPlaceMovedBlock. */
void fdActorLiftTargetBlock(Uint16 *actors);
/* Puts the block fdActorLiftTargetBlock remembered back into the map at the
   metatile it has moved to, as fdActorPlaceBlocks places each. */
void fdActorPlaceMovedBlock(void);
/* Returns the index of the map's shape entry (MapGridShapes, MAP_SHAPE) of
   shape whose first height is height, in 8-pixel steps, writing it into the
   first free entry (MAP_SHAPE_FREE) when there is none: the entry a raised or
   lowered metatile's shape grid byte takes. The layer goes unread. */
Sint32 fdMapShapeFindOrAdd(Sint32 layer, Sint32 shape, Sint32 height);
/* Returns the first of the placed actors (not the treasure spots) on position's
   metatile, at its height too when checkY is set, or NULL. */
struct ACTOR *fdActorFindAtMetatile(Sint32 *pos, Sint32 checkY);
/* Whether the actor 16 pixels in front of the leader, along its facing rounded
   to a quarter turn, can be pushed on by 16 pixels: 1 when nothing solid stands
   on the metatile beyond it or above it and its terrain test passes there, else
   0. fdLeaderPushObjectAhead, after it, makes the push. In every edition but
   the Japanese the actor Halt has stopped (MainRoutineStateWork.haltedActor)
   cannot be pushed; the Japanese release has no such test. */
Bool fdLeaderCanPushObjectAhead(void);
/* Pushes the actor ahead of the leader on by 16 pixels, as
   fdLeaderCanPushObjectAhead allowed. */
void fdLeaderPushObjectAhead(void);
/* Returns the actor one metatile, or failing that two metatiles, ahead of the
   leader at the leader's height, or NULL. */
struct ACTOR *fdLeaderFindActorAhead(void);
/* The pieces of the casts: their particles, hands and sparks, the tiles they
   leave, and the scripts and tables they run. */
struct ACTOR *fdActorSpawn(Sint32 kind, Sint32 x, Sint32 y, Sint32 z);
/* Clears what Cyclone blew away: each metatile of FIELD_STATE.psynergyMetatiles
   that falls in the width by height window at (windowX, windowZ) of layerIndex
   (counted from the layer's scroll and the leader's height) takes the metatile
   picture from the same place of the window at (sourceX, sourceZ), is opened on
   layer 2, and is recorded (up to 64) in TouchCopiesWork to be made again. A
   unit whose ROM code is compiled against another form defines
   OWN_fdTileCopiesCopyTouchedMetatiles and declares that form itself. */
#ifndef OWN_fdTileCopiesCopyTouchedMetatiles
void fdTileCopiesCopyTouchedMetatiles(Sint32 srcX, Sint32 srcZ, Sint32 width,
	Sint32 height, Sint32 winX, Sint16 winZ, Sint32 layerIdx);
#endif
/* Whether the leader can move as sand on the metatile at (x, z): the overhead
   layer's METATILE_BEHAVIOR_SAND, or on the world map collision
   WORLD_MAP_COLLISION_WALKABLE_FIRST or shallow water;
   FIELD_STATE.sceneAttributeHigh turns the answer round. */
Sint32 fdMapGridIsSandMetatile(Sint32 x, Sint32 z, Sint32 layer);
/* Whether the metatile at (x, z) can be climbed (MAP_METATILE_CLIMBABLE): Sand
   starts up a wall face only there (fdActorMoveAsSand) and ends its climb off
   one. A unit whose ROM code is compiled against another form defines
   OWN_fdMapGridIsClimbableMetatile and declares that form itself. */
#ifndef OWN_fdMapGridIsClimbableMetatile
Uint8 fdMapGridIsClimbableMetatile(Sint32 x, Sint32 z, Sint32 layer);
#endif
/* Returns the leader's actor id, or ACTOR_FIRST_PLACED while
   MainRoutineStateWork.ownerOverride is set. */
Sint32 fdPartyGetLeaderActor(void);
/* Hangs a vine at once from high above actor id down to its feet
   (fdHangingVineHang): an actor that is not moving is lifted and moved 16
   pixels north first, which fdHangingVineHang undoes; a moving one has its vine
   only when force is set. */
void fdHangingVinePlace(Sint32 id, Sint32 force);
/* The hanging vines, each hung from an actor. */
void fdHangingVineHang(Sint32 objectId, Sint32 grow);
/* Whirlwind on the vine hanging from actor id: the vine swings out to the
   leader's side and the leader takes hold. While the leader hangs on, B, Down
   or the direction away from the vine lets go; any other button leaps, and the
   vine carries the leader across to the far side. */
void fdHangingVineSwing(Sint32 id);
/* fdExitPrepareAnimation for any actor: centres the actor on its metatile at
   the metatile's south edge, stops it, hides its shadow and plays
   ANIM_LEADER_LEAVE (side 1) or ANIM_LEADER_LEAVE_ALTERNATE over 18 ticks, its
   sprite gaining the doorway (SPRITE_DOORWAY) as a second part. */
void fdExitPrepareActorAnimation(Sint32 actorId, Sint32 side);
/* An actor appears in a whirl of sparks, as Alex does on Venus Lighthouse:
   APPEAR_SPARK_COUNT sparkles (RESOURCE_SPARKLE_FADE) circle round where it
   stands, each rising and closing in for SPARK_LIFE ticks. The actor spins and
   widens from nothing to full width over APPEAR_WIDEN_TICKS, when its shadow
   shows again, and the sparks stop coming APPEAR_SPAWN_TICKS later. */
void fdActorAppearInSparks(Sint32 actorId);
/* Turns actor id's sprite to rotation (1/65536 turns). */
void fdActorSetSprRotationById(Sint32 id, Sint32 rotation);
/* Lays actor id down along angle, a quarter turn rounded: shows its first tick
   turned, shifted by the side it falls to, and blocks its metatile and the
   metatile beyond it. */
void fdActorFallOverById(Uint32 id, Uint32 ang);
/* fdActorFallOverById with an animation of the actor's sprite sheet (-1 for
   none) and the fall moving the actor itself half a metatile the way it
   falls. */
void fdActorFallOverWithAnimationById(Sint32 id, Uint32 ang, Sint32 anm);
/* Fills an effect sprite: OAM attributes 0 and 1 as given (no affine matrix,
   256 colours), the tiles, unscaled and unrotated at depth 255, and the
   picture's size from its shape, pivoted at (pivotX, pivotY). */
void fdEffectSprInit(struct EFFECT_SPR *spr, Sint32 pivotX, Sint32 pivotY,
	Uint32 attributes, Sint32 tile);
/* Draws a field effect's sprite. An unscaled, unrotated sprite needs no affine
   matrix; any other takes one, and one scaled past 1 is drawn in a double-size
   box. The picture's pivot is turned and scaled with it, so the pivot stays on
   the sprite's position. A sprite wholly off screen is not queued; the rest
   join the OAM bucket of their depth. */
void fdEffectSprDraw(struct EFFECT_SPR *spr);
/* Places sprite at the screen position of world position and draws it. */
void fdEffectSprDrawAtWorldPos(struct EFFECT_SPR *spr, Sint32 *pos);
/* Moves a 2D point by its velocity, damped and pulled by gravity
   (fdAdvanceWithGravity2D). */
void fdBattleEffectStepVector2(Sint32 *vector, Sint32 damping, Sint32 gravity);
/* Moves a 3D point by its velocity, damped and pulled by gravity
   (fdAdvanceWithGravity3D). */
void fdBattleEffectStepVector3(Sint32 *vector, Sint32 damping, Sint32 gravity);
/* Returns the OBJ priority of the actor's sprite. */
Sint32 fdActorGetSprPriority(struct ACTOR *actor);
/* Returns the depth slot the actor's sprite takes, so that a sprite drawn with
   it sorts with it: from its projected depth on the world map, otherwise from
   its screen row. */
Sint32 fdActorGetSprDepth(struct ACTOR *actor);
/* Fades the palettes to color over ticks ticks, keeping the field's colours
   for fdStartPaletteTransition; FLAG_PSYNERGY_TINTED is set while it holds. */
void fdStartColTransition(Sint32 ticks, Sint32 col);
/* Fades the palettes back from fdStartColTransition's colour over ticks
   ticks, the BG half to grey while Reveal is active; clears
   FLAG_PSYNERGY_TINTED. */
void fdStartPaletteTransition(Sint32 ticks);
/* Starts the swelling arcs over a caster: loads their tiles and starts
   fdBattleEffectUpdateArcEffect with no pair showing. */
void fdBattleEffectStartArcEffect(void);
/* Ends the swelling arcs: frees their tiles and their state, and stops their
   event. */
void fdBattleEffectEndArcEffect(void);
/* Waits, at most ARC_GROUPS_WAIT_TICKS, until both pairs of swelling arcs have
   run their 32 ticks. */
void fdBattleEffectWaitForArcGroups(void);
/* Gives the caster its look while it casts (CASTER_LOOK). */
void fdBattleEffectSetObjectUpdateMode(struct ACTOR *actor, Sint32 mode);
/* Says whether an actor stands where it may not: 1 when, outside the World Map,
   fdMapGridCheckCollisionAtStub reports its metatile, ORed with 2 when it
   overlaps any other solid actor (each radius less 2 pixels). */
Sint32 fdActorCheckBlocked(struct ACTOR *actor);
/* Returns the behaviour (collision code) of the map metatile at x, z in layer.
   A unit whose ROM code is compiled against another form defines
   OWN_fdMapGridGetMetatileBehavior and declares that form itself. */
#ifndef OWN_fdMapGridGetMetatileBehavior
Uint8 fdMapGridGetMetatileBehavior(Sint32 x, Sint32 z, Sint32 layer);
#endif
/* The map's metatiles: their attributes and what sand, water and Cyclone make
   of them. */
Sint32 fdMapGridGetMetatileAttributesAt(Sint32 x, Sint32 z, Sint32 layer);
/* Sets the metatile behavior of the map metatile at x, z in layer, and returns
   it. */
Uint8 fdMapGridSetMetatileBehavior(
	Sint32 x, Sint32 z, Sint32 layer, Sint32 value);
/* Sets the attribute byte of the map metatile at x, z in layer. */
Uint8 fdMapGridSetMetatileAttributesAt(
	Sint32 x, Sint32 z, Sint32 layer, Sint32 value);
/* Sets bits in the attribute byte of the map metatile at x, z in layer. */
Uint8 fdMapGridSetMetatileAttributeBitsAt(
	Sint32 x, Sint32 z, Sint32 layer, Sint32 bits);
/* Clears bits in the attribute byte of the map metatile at x, z in layer. */
Uint8 fdMapGridClearAttributeBBitsAt(
	Sint32 x, Sint32 z, Sint32 layer, Sint32 bits);
/* Shows an actor as an item's icon, its sprite's only picture: the item, or for
   coins (SCOOPED_COINS) COIN_ICON_ITEM's. */
void fdActorShowItemIconById(Sint32 id, Sint32 item);
/* Frees tightrope index's nodes; it is no longer drawn. */
void fdTightropeRelease(Sint32 idx);
/* Stops tightrope index's wobble and marks its nodes not to be walked. */
void fdTightropeSettle(Sint32 idx);
/* Gives an actor the leader's sprite priority, so it is drawn in front of and
   behind what he is, and keeps it there. A unit whose ROM code is compiled
   against another form defines OWN_fdActorTakeLeaderLayer and declares that
   form itself. */
#ifndef OWN_fdActorTakeLeaderLayer
Sint32 fdActorTakeLeaderLayer(struct ACTOR *actor);
#endif
/* The actor leaves in a whirlwind (SPRITE_WHIRLWIND): as the screen transition
   starts it circles inside it for DEPART_TICKS, from 96 pixels up and 32 out
   down to where it stood, then the whirlwind vanishes with a whoosh and the
   camera follows the actor again. */
void fdActorDepartInRing(Sint32 actorId);
/* The actor arrives in a whirlwind, fdActorDepartInRing's counterpart: once the
   screen transition is done, a whirlwind (SPRITE_WHIRLWIND) widens on the
   unseen, spinning actor for WHIRLWIND_RESIZE_TICKS, then vanishes with a
   whoosh as the actor grows back from half its size and shows again. */
void fdActorArriveInRing(Sint32 actorId);
/* Drops an actor from Scoop's hand (if there is one) to fall to the ground,
   waits 50 ticks, then asks for its talk, which opens it without its "checked
   the ..." message (FIELD_TALK_SILENT). */
void fdScoopDropFromHand(Sint32 id);
/* Shows message in a script and clears FLAG_NOTHING_HAPPENS; when someone in
   the party has Scoop, the field menu then opens. */
void fdScriptShowMessageAndOfferScoop(Sint32 msg);
/* Fills the overlay's 512 bytes of sprite tiles (VRAM_CACHE_ENTRY_FADE_OVERLAY)
   with PIXELS_8BPP(1) when solid is set, which in these 16-colour tiles is
   colour 1 in every other column (no caller sets it), else clears them. */
void fdFadeOverlayFillTiles(Sint32 solid);
/* Sets up sprite through fdEffectSprInit with the overlay's tiles
   (VRAM_CACHE_ENTRY_FADE_OVERLAY), at priority 1 in palette 0. */
void fdEffectSprInitWithFadeOverlay(
	struct EFFECT_SPR *spr, Sint32 pivotX, Sint32 pivotY, Uint32 attributes);
/* Projects the world position (x, y, z) to the screen, stores it as the effect
   sprite's position and draws the sprite. */
void fdEffectSprDrawAtWorldXYZ(
	struct EFFECT_SPR *spr, Sint32 x, Sint32 y, Sint32 z);
/* Per-tick update: sets every part of an actor's sprite to colour mode tick %
   6, which cycles its colours. A unit whose ROM code is compiled against
   another form defines OWN_fdActorCycleSprCols and declares that form
   itself. */
#ifndef OWN_fdActorCycleSprCols
void fdActorCycleSprCols(struct ACTOR *actor);
#endif
/* Clears the weeds the Cyclone blew away: for each metatile it touched (the
   field state's list from fdPsynergyGatherNearbyMetatiles) inside the width by
   height rectangle at (x, z) of the layer, copies the matching metatile tiles
   from the rectangle at (toX, toZ) over it, opens it on layer 2 and records the
   copy in TouchCopiesWork, up to 64. Gaia Rock calls it after a Cyclone. A unit
   whose ROM code is compiled against another form defines
   OWN_fdCycloneCopyTouchedMetatiles and declares that form itself. */
#ifndef OWN_fdCycloneCopyTouchedMetatiles
void fdCycloneCopyTouchedMetatiles(Sint32 x, Sint32 z, Sint16 layerIdx,
	Sint16 width, Sint16 height, Sint32 toX, Sint32 toZ);
#endif
/* Runs fdTeleportPlaySparks, fades the palettes back to the saved ones and
   waits 10 ticks. */
void fdTeleportPlaySparksAndFade(void);
/* Keeps an actor turning to face another. */
void fdActorWatchActor(struct ACTOR *actor, struct ACTOR *tgt);
/* As fdActorStop, and also clears the actor's target and velocity, so it stands
   where it is. */
void fdActorResetTargetsAndVelocity(Uint32 actorId);
/* Leaves through a stairs-up exit (ANIM_LEADER_LEAVE); FLAG_ARRIVAL_STAIRS_UP
   makes the next scene play the matching arrival (fdEntrancePlayArrival). */
void fdExitStartStairsUp(void);
/* Leaves through a stairs-down exit (ANIM_LEADER_LEAVE_ALTERNATE), recorded in
   FLAG_ARRIVAL_STAIRS_DOWN. */
void fdExitStartStairsDown(void);
/* Unused: nothing calls fdMotesStartStub. Starts the 64 floating motes
   (fdMotesUpdate): loads their tiles and puts each under the leader with
   staggered timers, drawn semi-transparent. */
void fdMotesStart(void);
/* Starts fdWeatherUpdateRain's raindrops: allocates and clears
   HEAP_SLOT_WEATHER, decodes the streak tiles into VRAM, sets each of the 64
   drops' sprite words and places it on the ground at the map's focus with its
   first fall staggered, turns on alpha blending of all layers at 16/16,
   schedules the update and, when asked, plays SE_KIRIN (fdQueueMusic). */
void fdWeatherStartRain(Sint32 withSnd);
/* Stops fdWeatherStartRain's particles. Unused: nothing calls
   fdWeatherStopRainStub. */
void fdWeatherStopRain(void);
/* Plays a summon tablet's reading (a CONTAINER_STONE_TABLET treasure, "examined
   the stone tablet"): pebbles fly off the tablet as it is struck, and its runes
   light up and fly round the leader, with their sounds, before
   mnSummonShowLearnedMessage. */
void fdSummonTabletPlayRunes(Sint32 tgtId);
/* Counts one more weird answer Felix gives
   (MainRoutineStateWork.weirdAnswers). */
void fdScriptCountAnswer(void);
/* Where the party comes out on the world map after reaching scene and entrance
   (Teleport's choice calls it). With FLAG_SHIP_CAN_SAIL set (the party has the
   ship, whose mooring the map screen marks), finds the first rule of
   Scene_ReturnRules for scene and entrance (-1 on either side matches any
   entrance) and copies its position, x and z in 16.16 and the third value as
   is, to MainRoutineStateWork.shipX, shipZ and shipFacing. */
void fdWorldMapSetReturnForScene(Sint32 scene, Sint32 entrance);
/* fdWorldMapSetReturnForScene's twin for the rule's return scene and entrance
   (its third and fourth values): with FLAG_SHIP_CAN_SAIL set, clears
   ownerOverride and copies the rule's position to the party's saved
   position. */
void fdWorldMapSetReturnLeavingScene(Sint32 scene, Sint32 entrance);
/* A Mad Plant (sprite) springs out of the bushes cleared at the metatile (x,
   z):
   it grows, turns to the leader, hops twice and leaps at the leader in an arc
   before the battle. The Gabomba Catacombs, Taopo Swamp and Gaia Rock each hide
   one. */
void fdMadPlantLeapAtLeader(Sint32 spr, Sint32 x, Sint32 z);
/* Returns 1 while fdScreenTransitionUpdateScanline is the HBlank handler, that
   is, while a centre-line transition runs. */
Bool fdScreenTransitionIsScanlineActive(void);
/* Checks whether the party can carry one more of item; when it cannot, says so
   and returns -1, else 0. */
Sint32 fdPartyCheckItemLimit(Sint32 item);
/* Returns the first of the scene's cues of kind for target whose condition
   holds and whose push (STEP_CUE_FLAG_WALKING) and facing (CUE_FLAG_FACING)
   requirements are met, or NULL. CUE_PUSH and CUE_PUSHED, the cues
   fdRunPushMetatile runs for an actor pushed and for the metatile pushed
   against, answer any target; the facing test alone is skipped while
   MainRoutineStateWork.movementMode is LEADER_MOVEMENT_KEYS. */
struct CUE_VIEW *fdFindCue(Sint32 kind, Sint32 tgt);
/* Packs the target colours, three channels each, back into the palette copy as
   RGB555. */
void fdPaletteFadePackTarget(void);
/* Takes colors as the BG colours of the palette copy and makes them, unchanged,
   the BG colours' target. */
void fdPaletteFadeLoadPalette(const Uint16 *cols);

#endif
