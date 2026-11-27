/* @file */
/* The Shaman Village Cave, on the way east from Hesperia to the Shaman */
/* Village. It has three rooms. The first, entered from the world map at its */
/* south, has three passages in its north wall. The back room behind them has */
/* raised ledges with four round boulders standing in the gaps between them, */
/* and Eddy, the Mercury Djinni, on the ledge in the middle. The pillar room, */
/* east of it, has the cave's east mouth. */
/* */
/* On the first visit, Whirlwind clears the weeds off the first room's west */
/* wall, Felix climbs to the west passage, crosses the back room to the pillar
 */
/* room and Moves the stone pillar two cells east into the gap in its ledge, */
/* which makes a short cut. Once Isaac has joined, with Lift, the party lifts */
/* the boulders, freezes the puddle and clears the back room's weeds to climb */
/* up to Eddy, who can no longer hop away once the third boulder is up. In a */
/* game carried over from a The Broken Seal save that won Colosso, Satrage, */
/* Azart and Navampa, the gladiators Isaac beat there, catch the party in the */
/* first room for a rematch, and give up a Golden Shirt when they lose. */
#include "item_ids.h"
#include "version.h"
#include "field/actor_script.h"
#include "field/call.h"
#include "field/event_inline.h"
#include "field/field_sequence.h"
#include "field/field_sequence_event.h"
#include "field/field_sprite.h"
#include "game/game.h"
#include "scene/scene.h"
#include "text/msg_ids.h"

/* Set once Move has pushed the stone pillar into the gap in the pillar room's
 */
/* ledge, at cell (38, 19). */
#define FLAG_SHAMAN_VILLAGE_CAVE_PILLAR_DROPPED 0x9a8
/* Set once Whirlwind has scattered the weeds on the first room's west wall. */
#define FLAG_SHAMAN_VILLAGE_CAVE_FIRST_WEEDS_SCATTERED 0x9a9
/* Set once Whirlwind has scattered the weeds in the back room. */
#define FLAG_SHAMAN_VILLAGE_CAVE_SECOND_WEEDS_SCATTERED 0x9aa
/* Set as the gladiators from Colosso find the party: "I found Isaac!" */
#define FLAG_SHAMAN_VILLAGE_CAVE_FOUND_BY_GLADIATORS 0xa70
/* Set while the party has lost to the gladiators, who wait for a rematch. */
#define FLAG_SHAMAN_VILLAGE_CAVE_LOST_TO_GLADIATORS 0xa71
/* Set once the party has beaten the gladiators. */
#define FLAG_SHAMAN_VILLAGE_CAVE_BEAT_GLADIATORS 0xa72
/* Set while the Golden Shirt the beaten gladiators leave lies in the cave. */
#define FLAG_SHAMAN_VILLAGE_CAVE_GOLDEN_SHIRT_LEFT 0xa73
/* Set as Lift raises one of the back room's four boulders, n counting from 0 */
/* in the west. While the second or third stands in its gap, the party can jump
 */
/* across the gap in the ledges above it; lifted, it fills the gap and there is
 */
/* no jump. */
#define FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(n) (FLAG_SCENE_TEMP_00 + (n))
/* Set once Eddy, its way south blocked by the lifted third boulder, has */
/* winced at the party. */
#define FLAG_SHAMAN_VILLAGE_CAVE_EDDY_CORNERED FLAG_SCENE_TEMP_04
/* Set once Eddy has been fought or has joined: it no longer hops away from the
 */
/* party. */
#define FLAG_SHAMAN_VILLAGE_CAVE_EDDY_STOPS_FLEEING FLAG_SCENE_TEMP_05
/* Set as the battle with Eddy begins: back in the cave, Eddy stands on the */
/* ledge between the third and fourth boulders. */
#define FLAG_SHAMAN_VILLAGE_CAVE_EDDY_FOUGHT FLAG_SCENE_TEMP_06
/* Set once Frost has frozen the back room's puddle into a pillar of ice. */
#define FLAG_SHAMAN_VILLAGE_CAVE_PUDDLE_FROZEN FLAG_AREA_TEMP_02

/* The cave's ways between its rooms. Each passage's cell, and the cell at */
/* each room's edge, holds the number of the exit it leads by. */
typedef enum SHAMAN_VILLAGE_CAVE_EXIT {
	/* The first room's west passage, cell (2, 6), at the top of the wall the */
	/* first weeds cover: to the back room's west end. */
	EXIT_WEST_PASSAGE = 2,
	/* The first room's middle passage, cell (12, 8): to the middle of the back
	 */
	/* room. */
	EXIT_MIDDLE_PASSAGE = 3,
	/* The first room's east passage, cell (22, 14): to the pillar room's floor.
	 */
	EXIT_EAST_PASSAGE = 4,
	/* The back room's west end, cell (4, 59): back below the west passage. */
	EXIT_BACK_ROOM_WEST = 5,
	/* The back room's middle, cell (14, 59): back below the middle passage. */
	EXIT_BACK_ROOM_MIDDLE = 6,
	/* The back room's east end, cell (26, 55): to the pillar room's ledge. */
	EXIT_BACK_ROOM_EAST = 7,
	/* The passage behind the pillar room's ledge, cell (33, 18): to the back */
	/* room's east end. */
	EXIT_PILLAR_ROOM_PASSAGE = 8,
	/* The pillar room's floor, cell (31, 25): back below the east passage. */
	EXIT_PILLAR_ROOM_FLOOR = 9
} SHAMAN_VILLAGE_CAVE_EXIT;

/* Where the party comes in: from another room, or back from the gladiators' */
/* battle, having beaten them or lost. */
typedef enum SHAMAN_VILLAGE_CAVE_ENTRANCE {
	ENTRANCE_BELOW_WEST_PASSAGE = 2,
	ENTRANCE_BELOW_MIDDLE_PASSAGE = 3,
	ENTRANCE_BELOW_EAST_PASSAGE = 4,
	ENTRANCE_BACK_ROOM_WEST = 5,
	ENTRANCE_BACK_ROOM_MIDDLE = 6,
	ENTRANCE_BACK_ROOM_EAST = 7,
	ENTRANCE_PILLAR_ROOM_LEDGE = 8,
	ENTRANCE_PILLAR_ROOM_FLOOR = 9,
	ENTRANCE_AFTER_BEATING_GLADIATORS = 20,
	ENTRANCE_AFTER_LOSING_TO_GLADIATORS = 21
} SHAMAN_VILLAGE_CAVE_ENTRANCE;

/* The cells that run the cave's other events. */
typedef enum SHAMAN_VILLAGE_CAVE_CELL {
	/* West of the gap in the back room's ledges above the second boulder, cell
	 */
	/* (12, 50), walking east. */
	GAP_ABOVE_SECOND_BOULDER_WEST_CELL = 41,
	/* East of that gap, cell (14, 50), walking west. */
	GAP_ABOVE_SECOND_BOULDER_EAST_CELL = 42,
	/* West of the gap above the third boulder, cell (17, 49), walking east. */
	GAP_ABOVE_THIRD_BOULDER_WEST_CELL = 43,
	/* East of that gap, cell (19, 49), walking west. */
	GAP_ABOVE_THIRD_BOULDER_EAST_CELL = 44,
	/* Close to Eddy on its ledge: cells (16, 45), (20, 45), (16, 49), (20, 49)
	 */
	/* and (15, 50). */
	NEAR_EDDY_CELL = 50,
	/* Halfway up the first room, cells (12, 14) and (13, 14), where the */
	/* gladiators catch the party. */
	GLADIATORS_AMBUSH_CELL = 100
} SHAMAN_VILLAGE_CAVE_CELL;

/* The cave's actors, numbering the rows of ActorPlacements. */
typedef enum SHAMAN_VILLAGE_CAVE_ACTOR_ID {
	/* The weeds Whirlwind scatters from the wall below the first room's west */
	/* passage, over the steps that climb it (SPRITE_WEEDS). */
	ACTOR_FIRST_WEEDS = ACTOR_FIRST_PLACED,
	/* A round boulder placed in the map's top left corner, above every room's
	 */
	/* view; nothing moves it. */
	ACTOR_HIDDEN_BOULDER,
	/* The second clump of weeds, on a wall in the back room, over the steps up
	 */
	/* toward Eddy. */
	ACTOR_SECOND_WEEDS,
	/* Four round boulders standing in the gaps between the back room's ledges,
	 */
	/* west to east (SPRITE_ROUND_BOULDER). */
	ACTOR_FIRST_BOULDER,
	ACTOR_LAST_BOULDER = ACTOR_FIRST_BOULDER + 3,
	/* The stone pillar on the pillar room's ledge, which Move pushes */
	/* (SPRITE_STONE_PILLAR). */
	ACTOR_PILLAR,
	/* The puddle Frost freezes, on the back room's west side (SPRITE_PUDDLE).
	 */
	ACTOR_PUDDLE,
	/* Eddy, the Mercury Djinni, placed until it joins */
	/* (FLAG_DJINNI(DJINNI_EDDY)). */
	ACTOR_EDDY,
	ACTOR_KRADEN,
	/* Colosso's three gladiators, come to call Isaac a cheat: "The name's */
	/* Satrage.", "And I'm Azart!", "And don't forget Navampa!" */
	ACTOR_SATRAGE,
	ACTOR_AZART,
	ACTOR_NAVAMPA,
	/* The Golden Shirt the gladiators leave */
	/* (scShamanVillageCaveTakeGoldenShirt). */
	ACTOR_GOLDEN_SHIRT
	/* The placement table's last row, in Isaac's sprite, is ACTOR_ISAAC. */
} SHAMAN_VILLAGE_CAVE_ACTOR_ID;

extern u32 ShamanVillageCave_GladiatorLeaves[];

extern s32 ShamanVillageCave_GladiatorFollowsOut[];

extern s32 ShamanVillageCave_WeedsScript[];
extern u32 ShamanVillageCave_PillarScript[];

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

/* Whirlwind blows a clump of weeds away for good: the one on the first room's
 */
/* west wall, cell (2, 8), uncovering the steps up to the west passage, or the
 */
/* one on the back room's wall, cell (14, 38), uncovering the steps up toward */
/* Eddy. The leaves flying are the field engine's own Whirlwind; here the */
/* clump's flag is set and its cell made climbable, copied from a spare */
/* climbable cell at the map's west edge. */
void scShamanVillageCaveScatterWeeds(s32 unused, s32 weeds) {
	if(weeds == ACTOR_FIRST_WEEDS) {
		fdEventSetFlag(FLAG_SHAMAN_VILLAGE_CAVE_FIRST_WEEDS_SCATTERED);
		fdMapGridCopyCellAttributes(0, 2, 1, 1, 2, 8);
	}
	if(weeds == ACTOR_SECOND_WEEDS) {
		fdEventSetFlag(FLAG_SHAMAN_VILLAGE_CAVE_SECOND_WEEDS_SCATTERED);
		fdMapGridCopyCellAttributes(0, 32, 1, 1, 14, 38);
	}
}

/* The stone pillar drops into the gap in the ledge until it lands on the floor
 */
/* below, or for a second at most, and stands there without its shadow. */
void scShamanVillageCaveDropPillarIntoGap(s32 id) {
	ACTOR *actor = fdActorGet(id);
	s32 frames = 60;

	actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	while(frames != 0) {
		syWait(1);
		if(actor->velY == 0)
			break;
		frames--;
	}
	syWait(10);
	fdActorSetSprFlags(actor, 0);
	actor->motionFlg = 0;
}

/* Nothing visible happens as Move takes hold of the stone pillar in the pillar
 */
/* room. Before the push, the game walls in the pillar's lane along the ledge,
 */
/* to its north, south and west, so that it can only travel east; the walls */
/* are a spare ring of wall cells copied over the ledge's cells (35, 18) to */
/* (38, 20) on the map's middle layer (columns 99 to 102), and come down again
 */
/* in scShamanVillageCaveFinishMovingPillar. */
void scShamanVillageCaveStartMovingPillar(void) {
	fdMapGridCopyCellAttributes(99, 10, 4, 3, 99, 18);
}

/* Each push of the stone pillar ends here. Pushed two cells east, to the gap */
/* in the pillar room's ledge at cell (38, 19), it drops into the gap and */
/* stays, its top level with the ledge, and the party can hop across on it: the
 */
/* short cut. Either way the walls round its lane come down again. */
void scShamanVillageCaveFinishMovingPillar(s32 unused, s32 id) {
	ACTOR *actor = fdActorGet(id);
	s32 x = FIELD_CELL(actor->x.fixed);

	if(x == 38) {
		fdEventSetFlag(FLAG_SHAMAN_VILLAGE_CAVE_PILLAR_DROPPED);
		actor->priFlg = 0;
		scShamanVillageCaveDropPillarIntoGap(id);
		fdMapGridCopyCellAttributes(40, 19, 1, 1, x, 19);
	}
	fdMapGridCopyCellAttributes(99, 6, 4, 3, 99, 18);
}

/* Frost freezes the puddle on the back room's west side, cell (6, 42), into a
 */
/* tall pillar of ice, whose top the party can hop across on. */
void scShamanVillageCaveFreezePuddle(void) {
	ACTOR *actor = fdActorGet(ACTOR_PUDDLE);

	fdEventSetFlag(FLAG_SHAMAN_VILLAGE_CAVE_PUDDLE_FROZEN);
	actor->motionFlg = 0;
	actor->priFlg = 0;
	fdMapGridCopyCellAttributes(8, 42, 1, 1, 6, 42);
}

/* Eddy, the Mercury Djinni on the ledge in the middle of the back room, hops */
/* away from the leader who comes near it on its side of the third boulder's */
/* column: two cells south (north when the leader is south of it), four cells */
/* east or west away from the leader, the last three in a leap, and back. Once
 */
/* the third boulder is lifted, its first hop south is its last: it lands, */
/* steps west, jumps, backs away and winces, cornered. */
void scShamanVillageCaveEddyFlees(void) {
	ACTOR *actor = fdActorGetById(ACTOR_EDDY);
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	s32 leaderX = FIELD_HALF_CELL(leader->x.fixed);
	s32 leaderZ = FIELD_HALF_CELL(leader->z.fixed);
	s32 eddyX = FIELD_HALF_CELL(actor->x.fixed);
	s32 dx;
	s32 dz;

	if(leaderX <= 36 && eddyX > 37)
		return;
	if(leaderX > 37 && eddyX <= 36)
		return;
	dx = 1;
	if(leaderX > 36)
		dx = -dx;
	dz = -1;
	if(leaderZ <= 95)
		dz = 1;
	if(dz == 1 &&
	   fdEventGetFlag(FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(2)) != 0) {
		if(fdEventGetFlag(FLAG_SHAMAN_VILLAGE_CAVE_EDDY_CORNERED) != 0)
			return;
		fdEventStart();
		syFieldEventEndLastingPsynergy(0);
		scFieldActorJump(ACTOR_EDDY, 4, 20);
		fdActorSetDestinationOffset(ACTOR_EDDY, 0, 32);
		scFieldActorWaitForMoveById(ACTOR_EDDY);
		fdCameraMoveToActorById(ACTOR_EDDY, 1);
		scFieldActorStartRepeatedMotion(ACTOR_EDDY, 2);
		fdActorFaceDirection(ACTOR_EDDY, FACING_WEST, 0);
		fdActorSetDestinationOffset(ACTOR_EDDY, -8, 0);
		scFieldActorWaitForMoveById(ACTOR_EDDY);
		scFieldActorJump(ACTOR_EDDY, 4, 20);
		fdActorSetAttachedEffect(ACTOR_EDDY, ATTACHED_EFFECT_IN_FRONT |
												 ATTACHED_EFFECT_ONCE);
		syFieldEventWait(30);
		fdActorGetById(ACTOR_EDDY)->steeringFlg &= ~ACTOR_FACES_MOTION;
		fdActorWalkBy(ACTOR_EDDY, 8, 0);
		scFieldActorWaitForMoveById(ACTOR_EDDY);
		syFieldEventWait(30);
		fdActorShowEmote(ACTOR_EDDY, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
		syFieldEventWait(30);
		fdEventEnd();
		fdEventSetFlag(FLAG_SHAMAN_VILLAGE_CAVE_EDDY_CORNERED);
		return;
	}
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	dz *= 32;
	scFieldActorJump(ACTOR_EDDY, 4, 20);
	fdActorSetDestinationOffset(ACTOR_EDDY, 0, dz);
	scFieldActorWaitForMoveById(ACTOR_EDDY);
	fdActorSetDestinationOffset(ACTOR_EDDY, dx * 16, 0);
	scFieldActorWaitForMoveById(ACTOR_EDDY);
	fdActorSetDestinationOffset(ACTOR_EDDY, dx * 48, 0);
	actor->velY = toFIXED(8);
	dz = -dz;
	scFieldActorWaitForMoveById(ACTOR_EDDY);
	fdActorSetDestinationOffset(ACTOR_EDDY, 0, dz);
	scFieldActorWaitForMoveById(ACTOR_EDDY);
	fdEventEnd();
}

/* Talking to Eddy, the Mercury Djinni, once the party has cornered it, starts
 */
/* its battle. */
void scShamanVillageCaveFightEddy(void) {
	fdEventSetFlag(FLAG_SHAMAN_VILLAGE_CAVE_EDDY_STOPS_FLEEING);
	fdEventSetFlag(FLAG_SHAMAN_VILLAGE_CAVE_EDDY_FOUGHT);
	fdEventRequestDjinnBattle(ACTOR_EDDY, DJINNI_EDDY);
}

/* The weeds' set-up, called by their script: they cast no shadow, stand still,
 */
/* and are drawn at priority 2, the priority of the cave's walls. */
s32 scShamanVillageCaveSetUpWeeds(ACTOR *actor) {
	fdActorSetSprFlags(actor, 0);
	((FIELD_SPR *)actor->spr)->pri = 2;
	actor->motionFlg = 0;
	return 0;
}

const ACTOR_PLACE *scGetActorPlacements(void) { return ActorPlacements; }

/* Lift raises the leftmost of the back room's four boulders, cell (8, 50), out
 */
/* of its gap, one of the steps on the way up to Eddy. Like each of the four, */
/* it stays up until the party leaves the cave. */
void scShamanVillageCaveLiftFirstBoulder(void) {
	fdEventSetFlag(FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(0));
}

/* Lift raises the second boulder from the west in the back room, cell */
/* (13, 50), out of its gap. Up in the gap, it stops the jump across the gap in
 */
/* the ledges above (scShamanVillageCaveJumpForward). */
void scShamanVillageCaveLiftSecondBoulder(void) {
	fdEventSetFlag(FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(1));
}

/* Lift raises the third boulder from the west in the back room, cell (18, 49),
 */
/* reached from behind once the fourth is up. Up in the gap beside Eddy's */
/* ledge, it cuts off Eddy's way south: the next time it hops south, it is */
/* cornered. */
void scShamanVillageCaveLiftThirdBoulder(void) {
	fdEventSetFlag(FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(2));
}

/* Lift raises the rightmost of the back room's four boulders, cell (22, 49), */
/* opening the way behind it to the third. */
void scShamanVillageCaveLiftFourthBoulder(void) {
	fdEventSetFlag(FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(3));
}

const TRIGGER *scGetTriggers(void) { return SceneEvents; }

/* Talking to the Golden Shirt the gladiators leave on the first room's floor */
/* picks it up, if there is room. */
void scShamanVillageCaveTakeGoldenShirt(void) {
	s32 result;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	result = Value3((s32 (*)())fdEventPickUpItem, ACTOR_GOLDEN_SHIRT,
					ITEM_GOLDEN_SHIRT, -1);
	if(result == 0)
		fdEventClearFlag(FLAG_SHAMAN_VILLAGE_CAVE_GOLDEN_SHIRT_LEFT);
	fdEventEnd();
}

/* The three gladiators stand in the first room, where their rematch left */
/* them. */
void scShamanVillageCavePlaceGladiators(void) {
	s32 angle;

	angle = FACING_SOUTH_SOUTHWEST;
	fdActorSetPosAndFacing(ACTOR_SATRAGE, toFIXED(231), toFIXED(199), angle);
	fdActorSetPosAndFacing(ACTOR_AZART, toFIXED(220), toFIXED(191), angle);
	fdActorSetPosAndFacing(ACTOR_NAVAMPA, toFIXED(204), toFIXED(191), angle);
}

/* The Golden Shirt lies where the gladiators left it */
/* (FLAG_SHAMAN_VILLAGE_CAVE_GOLDEN_SHIRT_LEFT). */
void scShamanVillageCavePlaceGoldenShirt(void) {
	s32 angle;

	scFieldActorShowItemIconById(ACTOR_GOLDEN_SHIRT, ITEM_GOLDEN_SHIRT);
	angle = FACING_NORTH;
	fdActorSetPosAndFacing(ACTOR_GOLDEN_SHIRT, toFIXED(212), toFIXED(199),
						   angle);
	fdEventSetFlag(FLAG_SHAMAN_VILLAGE_CAVE_GOLDEN_SHIRT_LEFT);
}

/* Walking halfway up the first room, cells (12, 14) and (13, 14), once Isaac */
/* has joined, in a game carried over from a The Broken Seal save that won */
/* Colosso: Satrage, Azart and Navampa, the gladiators Isaac beat there, come */
/* out of the middle passage, call the win a cheat and fight the party. */
void scShamanVillageCaveMeetGladiators(void) {
	s32 angle;

	if(fdEventGetFlag(FLAG_TRANSFERRED_WON_COLOSSO) == 0)
		return;
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_ISAAC)) == 0)
		return;
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdEventSetFlag(FLAG_SHAMAN_VILLAGE_CAVE_FOUND_BY_GLADIATORS);
	syFieldEventSetMessageId((s32)MsgIFoundName);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_STAND);
	/* "I found Isaac!" */
	syFieldEventShowMessageThenWait(ACTOR_SATRAGE, 0, 1);
	fdActorWalkToAndWait(ACTOR_FELIX, 200, 221);
	fdActorRunRepeatedMotion(ACTOR_FELIX, 2);
	syFieldEventWait(5);
	fdActorWalkBy(ACTOR_FELIX, -16, 8);
	scFieldActorLaunchFromLeader(ACTOR_ISAAC, 0, 0, FACING_NORTH);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, 8, 16, FACING_NORTH);
	scFieldActorWaitForMoveById(ACTOR_ISAAC);
	scFieldActorWaitForMoveById(ACTOR_KRADEN);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_SOUTH, 0);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	syFieldEventWait(10);
	fdActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdCameraMoveTo(199, CAMERA_KEEP, 177, 1);
	fdCameraWaitForArrival();
	angle = FACING_SOUTH;
	fdActorSetPosAndFacing(ACTOR_SATRAGE, toFIXED(199), toFIXED(130), angle);
	fdActorWalkTo(ACTOR_SATRAGE, 231, 199);
	syFieldEventWait(20);
	fdActorSetPosAndFacing(ACTOR_AZART, toFIXED(199), toFIXED(130), angle);
	fdActorWalkTo(ACTOR_AZART, 220, 191);
	syFieldEventWait(20);
	fdActorSetPosAndFacing(ACTOR_NAVAMPA, toFIXED(199), toFIXED(130), angle);
	fdActorWalkTo(ACTOR_NAVAMPA, 204, 191);
	fdCameraMoveTo(199, CAMERA_KEEP, 225, 1);
	fdCameraWaitForArrival();
	scFieldActorWaitForMoveById(ACTOR_SATRAGE);
	fdActorFaceDirection(ACTOR_SATRAGE, FACING_SOUTH_SOUTHWEST, 0);
	scFieldActorWaitForMoveById(ACTOR_AZART);
	fdActorFaceDirection(ACTOR_AZART, FACING_SOUTH_SOUTHWEST, 0);
	scFieldActorWaitForMoveById(ACTOR_NAVAMPA);
	fdActorFaceDirectionAndWait(ACTOR_NAVAMPA, FACING_SOUTH_SOUTHWEST);
	fdActorRunRepeatedMotion(ACTOR_NAVAMPA, 2);
	/* "We've been looking for you, Isaac!" */
	syFieldEventShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "Who are you guys?" */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorShowEmote(ACTOR_SATRAGE, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "The name's Satrage. You took me out in the last round at Colosso." */
	syFieldEventShowMessageThenWait(ACTOR_SATRAGE, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_AZART, 2);
	syFieldEventWait(5);
	/* "And I'm Azart!" */
	syFieldEventShowMessageThenWait(ACTOR_AZART, 0, 5);
	fdActorShowEmote(ACTOR_NAVAMPA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "And don't forget Navampa!" */
	syFieldEventShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHEAST);
	fdActorSetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
	syFieldEventWait(10);
	/* "Oh. It's you guys." */
	syFieldEventShowMessageThenWait(ACTOR_ISAAC, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_SATRAGE, 2);
	syFieldEventWait(5);
	/* "Is that any way to say hello to some old friends?" */
	syFieldEventShowMessageThenWait(ACTOR_SATRAGE, 0, 5);
	fdActorFaceEachOther(ACTOR_KRADEN, ACTOR_FELIX, 0);
	syFieldEventWait(10);
	fdActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "What's your problem?" */
	syFieldEventShowMessageThenWait(ACTOR_ISAAC, 0, 5);
	fdActorShowEmote(ACTOR_AZART, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "We want to talk to you about your fighting style at Colosso." */
	syFieldEventShowMessageThenWait(ACTOR_AZART, 0, 5);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTHEAST);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	syFieldEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_NAVAMPA, ANIM_SHAKE_HEAD);
	/* "Yeah. And we think you shouldn't have won, Isaac. We think you..." */
	syFieldEventShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "It sounds like you resent the fact that Isaac beat you..." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorSetAnimation(ACTOR_NAVAMPA, ANIM_SHAKE_HEAD);
	fdActorSetAnimation(ACTOR_SATRAGE, ANIM_SHAKE_HEAD);
	fdActorSetAnimationAndWait(ACTOR_AZART, ANIM_SHAKE_HEAD);
	/* "We've just been thinking about it, and there's no way that Isaac..." */
	syFieldEventShowMessageThenWait(ACTOR_SATRAGE, 0, 5);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "Right. So you think I cheated just because I used my Pynergy..." */
	syFieldEventShowMessageThenWait(ACTOR_ISAAC, 0, 5);
	fdActorFaceEachOther(ACTOR_AZART, ACTOR_NAVAMPA, 0);
	syFieldEventWait(10);
	fdActorShowEmote(ACTOR_AZART, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "I don't know how you did it. I'm just sayin' I think you cheated is..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_AZART, 0, 5);
	fdActorFaceDirection(ACTOR_AZART, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_NAVAMPA, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTHEAST);
	syFieldEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "Ah, but Psynergy is a warrior's skill, every bit as much as your..." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_NAVAMPA, ANIM_SHAKE_HEAD);
	/* "You're not listening, are you, old man? That fight was fixed, and..." */
	syFieldEventShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
	fdActorSetDestinationOffset(ACTOR_AZART, -16, 24);
	fdActorSetDestinationOffset(ACTOR_NAVAMPA, -16, 24);
	fdActorSetDestinationOffset(ACTOR_SATRAGE, -16, 24);
	scFieldActorWaitForMoveById(ACTOR_SATRAGE);
	btSetType(BATTLE_TYPE_NO_ESCAPE);
	syFieldEventSetAfterBattleDestination(SCENE_SHAMAN_VILLAGE_CAVE,
										  ENTRANCE_AFTER_BEATING_GLADIATORS);
	fdEventSetDefeatDestination(SCENE_SHAMAN_VILLAGE_CAVE,
								ENTRANCE_AFTER_LOSING_TO_GLADIATORS);
	fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 2);
}

/* Talking to any of the three gladiators, who wait in the first room after */
/* beating the party, starts the rematch. */
void scShamanVillageCaveFightGladiators(void) {
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	btSetType(BATTLE_TYPE_NO_ESCAPE);
	syFieldEventSetAfterBattleDestination(SCENE_SHAMAN_VILLAGE_CAVE,
										  ENTRANCE_AFTER_BEATING_GLADIATORS);
	fdEventSetDefeatDestination(SCENE_SHAMAN_VILLAGE_CAVE,
								ENTRANCE_AFTER_LOSING_TO_GLADIATORS);
	fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 2);
}

/* Back in the cave after losing the rematch, the gladiators crow over the */
/* party: "Hey, these guys aren't very strong at all!" */
void scShamanVillageCaveLoseToGladiators(void) {
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdCameraMoveTo(199, CAMERA_KEEP, 273, 0);
	syFieldEventSetMessageId((s32)MsgHeyTheseGuys);
	fdActorSetPosAndFacing(ACTOR_FELIX, toFIXED(184), toFIXED(229),
						   FACING_NORTHWEST);
	fdActorSetPosAndFacing(ACTOR_ISAAC, toFIXED(200), toFIXED(221),
						   FACING_NORTHWEST);
	fdActorSetPosAndFacing(ACTOR_KRADEN, toFIXED(218), toFIXED(320),
						   FACING_NORTH);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_COLLAPSE);
	fdActorSetAnimation(ACTOR_ISAAC, 39);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdActorRunRepeatedMotion(ACTOR_NAVAMPA, 2);
	syFieldEventWait(5);
	fdActorShowEmote(ACTOR_NAVAMPA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "Hey, these guys aren't very strong at all!" */
	syFieldEventShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
	fdActorShowEmote(ACTOR_SATRAGE, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "Ha! We were right! They were cheating at Colosso!" */
	fdEventOpenMessage(ACTOR_SATRAGE, 0);
	if(syEventYesNoAsk(1) == 0) {
		mnMessageCloseFinishedWindows();
		/* Yes: "I knew it!" */
		syFieldEventShowMessageThenWait(ACTOR_AZART, 0, 5);
		FieldStateWork->msg++;
	}
	else {
		mnMessageCloseFinishedWindows();
		FieldStateWork->msg++;
		/* No: "It takes a lot to reach the finals at Colosso... A lot more..."
		 */
		syFieldEventShowMessageThenWait(ACTOR_AZART, 0, 5);
	}
	fdActorSetAnimationAndWait(ACTOR_NAVAMPA, ANIM_SHAKE_HEAD);
	/* "You look like you've got business in here, but there's no way you're..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_SATRAGE, ANIM_NOD);
	syFieldEventWait(10);
	/* "Give it up! We're staying here. We've got our eyes peeled, and..." */
	syFieldEventShowMessageThenWait(ACTOR_SATRAGE, 0, 5);
	fdCameraSetSpeed(0.5);
	fdCameraMoveTo(199, CAMERA_KEEP, 373, 1);
	syFieldEventWait(100);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 1);
	syFieldEventWait(20);
	fdEventSetDestination((s16)MainRoutineStateWork.restartScene,
						  (s16)MainRoutineStateWork.restartEntrance);
#if TLA_PATCH_1
	fdWorldMapSetReturnLeavingScene(MainRoutineStateWork.next,
									MainRoutineStateWork.entrance);
#endif
	FieldStateWork->transition = 0;
}

/* Back in the cave after winning the rematch, the gladiators marvel: "These */
/* guys were phenomenal!" */
void scShamanVillageCaveBeatGladiators(void) {
	ACTOR *felix;

	fdEventClearFlag(FLAG_SHAMAN_VILLAGE_CAVE_LOST_TO_GLADIATORS);
	fdEventSetFlag(FLAG_SHAMAN_VILLAGE_CAVE_BEAT_GLADIATORS);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdCameraMoveTo(199, CAMERA_KEEP, 273, 0);
	syFieldEventSetMessageId((s32)MsgTheseGuysWere);
	fdActorSetPosAndFacing(ACTOR_FELIX, toFIXED(184), toFIXED(229),
						   FACING_NORTHEAST);
	fdActorSetPosAndFacing(ACTOR_ISAAC, toFIXED(200), toFIXED(221),
						   FACING_NORTHEAST);
	fdActorSetPosAndFacing(ACTOR_KRADEN, toFIXED(208), toFIXED(237),
						   FACING_NORTHEAST);
	fdActorSetAnimation(ACTOR_AZART, 8);
	fdActorSetAnimation(ACTOR_SATRAGE, 8);
	fdActorSetAnimation(ACTOR_NAVAMPA, 8);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdActorRunRepeatedMotion(ACTOR_AZART, 2);
	syFieldEventWait(5);
	/* "These guys were phenomenal!" */
	syFieldEventShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
	/* "Which one of you didn't think they won Colosso fair and square?" */
	syFieldEventShowMessageThenWait(ACTOR_SATRAGE, 0, 5);
	/* "It wasn't me! I think both you and Navampa were saying something..." */
	syFieldEventShowMessageThenWait(ACTOR_AZART, 0, 5);
	fdActorShowEmote(ACTOR_SATRAGE, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_NAVAMPA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	fdActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "So. You don't still think our victory at Colosso was fixed, do you?" */
	syFieldEventShowMessageThenWait(ACTOR_ISAAC, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_NAVAMPA, ANIM_NOD);
	syFieldEventWait(10);
	/* "No way, Isaac! We've seen the error of our ways!" */
	syFieldEventShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
	fdActorShowEmote(ACTOR_SATRAGE, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "We promise we won't badmouth you or call you a fraud ever again.
	 * Will..." */
	fdEventOpenMessage(ACTOR_SATRAGE, 0);
	while(syFieldEventAskYesNoWithActorAnimation(0, 0) != 0) {
		fdActorShowEmote(ACTOR_SATRAGE, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
		/* No: "Don't be that way! We've learned our lesson! Forgive us!" */
		fdEventOpenMessage(ACTOR_AZART, 0);
		FieldStateWork->msg--;
	}
	FieldStateWork->msg++;
	/* Yes: "You're not such a bad kid after all!" */
	syFieldEventShowMessageThenWait(ACTOR_AZART, 0, 5);
	fdActorSetAnimation(ACTOR_AZART, ANIM_STAND);
	fdActorSetAnimation(ACTOR_SATRAGE, ANIM_STAND);
	fdActorSetAnimation(ACTOR_NAVAMPA, ANIM_STAND);
	syFieldEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_NAVAMPA, ANIM_NOD);
	syFieldEventWait(10);
	/* "You came out here looking for treasure, didn't you?" */
	syFieldEventShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
	scShamanVillageCavePlaceGoldenShirt();
	syFieldEventWait(60);
	fdActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	fdActorSetAnimationAndWait(ACTOR_SATRAGE, ANIM_NOD);
	syFieldEventWait(10);
	/* "Here, this was in the chest we found." */
	syFieldEventShowMessageThenWait(ACTOR_SATRAGE, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
	syFieldEventWait(10);
	fdActorShowEmote(ACTOR_AZART, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 0);
	/* "You were nice enough to forgive us, so we think you should have it." */
	syFieldEventShowMessageThenWait(ACTOR_AZART, 0, 5);
	fdActorShowEmote(ACTOR_NAVAMPA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "Right, well... I guess we'll be leaving now." */
	syFieldEventShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_SATRAGE, ANIM_NOD);
	syFieldEventWait(10);
	/* "By the time the next Colosso rolls around, we ought to be strong..." */
	syFieldEventShowMessageThenWait(ACTOR_SATRAGE, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_AZART, ANIM_NOD);
	syFieldEventWait(10);
	/* "Maybe we'll see you there." */
	syFieldEventShowMessageThenWait(ACTOR_AZART, 0, 5);
	fdActorSetAnimation(ACTOR_ISAAC, ANIM_NOD);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	syFieldEventWait(10);
	fdActorSetSpeed(ACTOR_SATRAGE, 1);
	fdActorSetSpeed(ACTOR_AZART, 1);
	fdActorSetSpeed(ACTOR_NAVAMPA, 1);
	fdActorSetScript(ACTOR_SATRAGE, ShamanVillageCave_GladiatorLeaves);
	syFieldEventWait(20);
	fdActorSetScript(ACTOR_AZART, ShamanVillageCave_GladiatorFollowsOut);
	syFieldEventWait(20);
	fdActorSetScript(ACTOR_NAVAMPA, ShamanVillageCave_GladiatorFollowsOut);
	syFieldEventWait(100);
	fdActorFaceEachOther(ACTOR_ISAAC, ACTOR_FELIX, 0);
	fdActorFaceActor(ACTOR_KRADEN, ACTOR_FELIX, 0);
	syFieldEventWait(40);
	fdActorSetAnimation(ACTOR_ISAAC, ANIM_NOD);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	syFieldEventWait(10);
	fdActorSetAnimation(ACTOR_ISAAC, ANIM_WALK);
	felix = fdActorGet(ACTOR_FELIX);
	if(felix != NULL)
		fdActorSetDestination(ACTOR_ISAAC, felix->x.part[0].integer,
							  felix->z.part[0].integer);
	scFieldActorWaitForMoveById(ACTOR_ISAAC);
	fdActorSetPos(ACTOR_ISAAC, 0, 0);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
	felix = fdActorGet(ACTOR_FELIX);
	if(felix != NULL)
		fdActorSetDestination(ACTOR_KRADEN, felix->x.part[0].integer,
							  felix->z.part[0].integer);
	scFieldActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	fdEventEnd();
}

/* Walking north into one of the dark passages in the rock, Felix steps in and
 */
/* the screen wipes to the room it leads to. The first room has three, at the */
/* top of its west wall, cell (2, 6), to the back room's west end; in the */
/* middle, cell (12, 8), to the back room's middle; and in the east, cell */
/* (22, 14), to the pillar room's floor. The fourth, behind the pillar room's */
/* ledge at cell (33, 18), leads back to the back room's east end. Everyone on
 */
/* stage stops as he goes. */
void scShamanVillageCaveEnterPassage(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR *actor;
	u32 id;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	for(id = ACTOR_FIRST_PLACED; id < FIELD_TREASURE_ACTOR; id++) {
		actor = fdActorGet(id);
		if(actor)
			actor->motionFlg = 0;
	}
	fdActorGet(MainRoutineStateWork.leaderActorId)->motionFlg = 0;
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_WALK);
	fdActorCenterAndWalk(MainRoutineStateWork.leaderActorId, 2, -8);
	syFieldEventWait(5);
	fdEventPlaySnd(SE_MAP_EXIT);
	syFieldEventSetExitRequest(state->stepTrigger);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdEventEnd();
}

/* Sets the cave up as its flags left it: the scattered weeds gone, the pillar
 */
/* in its gap, the puddle frozen, Eddy where its battle was fought, and the */
/* gladiators back from their rematch, won or lost, or waiting for one. */
s32 scInit(void) {
	ACTOR *actor;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	if(fdEventGetFlag(FLAG_SHAMAN_VILLAGE_CAVE_FIRST_WEEDS_SCATTERED))
		fdActorSetPos(ACTOR_FIRST_WEEDS, 0, 0);
	if(fdEventGetFlag(FLAG_SHAMAN_VILLAGE_CAVE_SECOND_WEEDS_SCATTERED))
		fdActorSetPos(ACTOR_SECOND_WEEDS, 0, 0);
	if(fdEventGetFlag(FLAG_DJINNI(DJINNI_EDDY)) ||
	   fdEventGetFlag(FLAG_DJINNI_HIDING(DJINNI_EDDY)))
		fdEventSetFlag(FLAG_SHAMAN_VILLAGE_CAVE_EDDY_STOPS_FLEEING);
	if(fdEventGetFlag(FLAG_SHAMAN_VILLAGE_CAVE_PILLAR_DROPPED)) {
		actor = fdActorGet(ACTOR_PILLAR);
		actor->motionFlg = 0;
		actor->priFlg = 0;
		actor->x.fixed = toFIXED(616);
		actor->y.fixed = 0;
		fdActorSetSprFlags(actor, 0);
		fdMapGridCopyCellAttributes(40, 19, 1, 1, 38, 19);
	}
	fdActorSetSprFlags(fdActorGet(ACTOR_PUDDLE), 0);
	fdActorSetAnimation(ACTOR_PUDDLE, PUDDLE_ANIMATION_PUDDLE);
	if(fdEventGetFlag(FLAG_SHAMAN_VILLAGE_CAVE_PUDDLE_FROZEN)) {
		actor = fdActorGet(ACTOR_PUDDLE);
		fdActorSetAnimation(ACTOR_PUDDLE, 5);
		scFieldActorSetSprCol(fdActorGet(ACTOR_PUDDLE), SPRITE_COLOR_NORMAL);
		actor->motionFlg = 0;
		actor->priFlg = 0;
		actor->y.fixed = 0;
		fdMapGridCopyCellAttributes(8, 42, 1, 1, 6, 42);
	}
	if(fdEventGetFlag(FLAG_SHAMAN_VILLAGE_CAVE_EDDY_FOUGHT))
		fdActorSetPos(ACTOR_EDDY, 328, 792);
	/* The three gladiators share one sprite, told apart by its colours. */
	scFieldActorSetSprCol(fdActorGet(ACTOR_SATRAGE), SPRITE_COLOR_GRB);
	scFieldActorSetSprCol(fdActorGet(ACTOR_AZART), SPRITE_COLOR_NORMAL);
	scFieldActorSetSprCol(fdActorGet(ACTOR_NAVAMPA), SPRITE_COLOR_RBG);
	if((!fdEventGetFlag(FLAG_KEEP_PARTY_POSITION) &&
		(MainRoutineStateWork.entrance == ENTRANCE_AFTER_BEATING_GLADIATORS ||
		 MainRoutineStateWork.entrance ==
			 ENTRANCE_AFTER_LOSING_TO_GLADIATORS)) ||
	   fdEventGetFlag(FLAG_SHAMAN_VILLAGE_CAVE_LOST_TO_GLADIATORS)) {
		scShamanVillageCavePlaceGladiators();
		if(!fdEventGetFlag(FLAG_KEEP_PARTY_POSITION) &&
		   MainRoutineStateWork.entrance == ENTRANCE_AFTER_BEATING_GLADIATORS) {
			scShamanVillageCaveBeatGladiators();
		}
		else if(!fdEventGetFlag(FLAG_KEEP_PARTY_POSITION) &&
				MainRoutineStateWork.entrance ==
					ENTRANCE_AFTER_LOSING_TO_GLADIATORS) {
			fdEventSetFlag(FLAG_SHAMAN_VILLAGE_CAVE_LOST_TO_GLADIATORS);
			scShamanVillageCaveLoseToGladiators();
		}
	}
	if(fdEventGetFlag(FLAG_SHAMAN_VILLAGE_CAVE_GOLDEN_SHIRT_LEFT))
		scShamanVillageCavePlaceGoldenShirt();
	return 0;
}

/* The walls whose weeds Whirlwind has scattered are climbable again. */
s32 scPrepareMap(void) {
	s32 firstScattered;
	s32 secondScattered;

	firstScattered =
		fdEventGetFlag(FLAG_SHAMAN_VILLAGE_CAVE_FIRST_WEEDS_SCATTERED);
	if(firstScattered != 0)
		fdMapGridCopyCellAttributes(0, 2, 1, 1, 2, 8);
	secondScattered =
		fdEventGetFlag(FLAG_SHAMAN_VILLAGE_CAVE_SECOND_WEEDS_SCATTERED);
	if(secondScattered != 0)
		fdMapGridCopyCellAttributes(0, 32, 1, 1, 14, 38);
	return 0;
}

/* Walking east or west against the gap in the back room's ledges above the */
/* second or third boulder (cells (12, 50) and (14, 50), (17, 49) and */
/* (19, 49)), the leader jumps across it, while that boulder still stands in */
/* the gap below. */
void scShamanVillageCaveJumpForward(void) { fdTryJumpForward(); }

#include "shared/scene_entrances_default.c"

u32 SceneExits[] = {
	SCENE_EXITS(ShamanVillageCave),
	SCENE_EXIT_UNLESS(SHAMAN_VILLAGE_CAVE_TO_WORLD_MAP_WEST, WorldMap,
					  WORLD_MAP_FROM_SHAMAN_VILLAGE_CAVE_WEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(EXIT_WEST_PASSAGE, ShamanVillageCave,
					  ENTRANCE_BACK_ROOM_WEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(EXIT_MIDDLE_PASSAGE, ShamanVillageCave,
					  ENTRANCE_BACK_ROOM_MIDDLE, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(EXIT_EAST_PASSAGE, ShamanVillageCave,
					  ENTRANCE_PILLAR_ROOM_FLOOR, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(EXIT_BACK_ROOM_WEST, ShamanVillageCave,
					  ENTRANCE_BELOW_WEST_PASSAGE, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(EXIT_BACK_ROOM_MIDDLE, ShamanVillageCave,
					  ENTRANCE_BELOW_MIDDLE_PASSAGE, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(EXIT_BACK_ROOM_EAST, ShamanVillageCave,
					  ENTRANCE_PILLAR_ROOM_LEDGE, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(EXIT_PILLAR_ROOM_PASSAGE, ShamanVillageCave,
					  ENTRANCE_BACK_ROOM_EAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(EXIT_PILLAR_ROOM_FLOOR, ShamanVillageCave,
					  ENTRANCE_BELOW_EAST_PASSAGE, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(SHAMAN_VILLAGE_CAVE_TO_WORLD_MAP_EAST, WorldMap,
					  WORLD_MAP_FROM_SHAMAN_VILLAGE_CAVE_EAST, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

/* The weeds' script: they stand half as tall again, and are set up by */
/* scShamanVillageCaveSetUpWeeds. */
s32 ShamanVillageCave_WeedsScript[] = {
	SC_SET(SW_SCALE_Y_DUPLICATE, FIXED_ONE * 3 / 2),
	SC_CALL(scShamanVillageCaveSetUpWeeds),
	SC_END,
};

/* clang-format off */
/* The pillar's script: it stands still. */
u32 ShamanVillageCave_PillarScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_END,
};

ACTOR_PLACE ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)ShamanVillageCave_WeedsScript, 40, 0, 128, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)ShamanVillageCave_WeedsScript, 232, 0, 608, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 808, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 808, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 792, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 792, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, (s32)ShamanVillageCave_PillarScript, 584, 0, 312, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_EDDY), MOVEMENT_TYPE_STAND, 264, 0, 760, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ISAAC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

TRIGGER SceneEvents[] = {
    {TRIGGER_EXIT, SHAMAN_VILLAGE_CAVE_TO_WORLD_MAP_WEST, CONDITION_ALWAYS, SHAMAN_VILLAGE_CAVE_TO_WORLD_MAP_WEST},
    {STEP_TRIGGER_FACING(FACING_NORTH), EXIT_WEST_PASSAGE, CONDITION_ALWAYS, EVENT(scShamanVillageCaveEnterPassage)},
    {STEP_TRIGGER_FACING(FACING_NORTH), EXIT_MIDDLE_PASSAGE, CONDITION_ALWAYS, EVENT(scShamanVillageCaveEnterPassage)},
    {STEP_TRIGGER_FACING(FACING_NORTH), EXIT_EAST_PASSAGE, CONDITION_ALWAYS, EVENT(scShamanVillageCaveEnterPassage)},
    {TRIGGER_EXIT, EXIT_BACK_ROOM_WEST, CONDITION_ALWAYS, EXIT_BACK_ROOM_WEST},
    {TRIGGER_EXIT, EXIT_BACK_ROOM_MIDDLE, CONDITION_ALWAYS, EXIT_BACK_ROOM_MIDDLE},
    {TRIGGER_EXIT, EXIT_BACK_ROOM_EAST, CONDITION_ALWAYS, EXIT_BACK_ROOM_EAST},
    {STEP_TRIGGER_FACING(FACING_NORTH), EXIT_PILLAR_ROOM_PASSAGE, CONDITION_ALWAYS, EVENT(scShamanVillageCaveEnterPassage)},
    {TRIGGER_EXIT, EXIT_PILLAR_ROOM_FLOOR, CONDITION_ALWAYS, EXIT_PILLAR_ROOM_FLOOR},
    {TRIGGER_EXIT, SHAMAN_VILLAGE_CAVE_TO_WORLD_MAP_EAST, CONDITION_ALWAYS, SHAMAN_VILLAGE_CAVE_TO_WORLD_MAP_EAST},
    {TRIGGER_STEP, NEAR_EDDY_CELL, FLAG_SHAMAN_VILLAGE_CAVE_EDDY_STOPS_FLEEING, EVENT(scShamanVillageCaveEddyFlees)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), GAP_ABOVE_SECOND_BOULDER_WEST_CELL, FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(1), EVENT(scShamanVillageCaveJumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), GAP_ABOVE_SECOND_BOULDER_EAST_CELL, FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(1), EVENT(scShamanVillageCaveJumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), GAP_ABOVE_THIRD_BOULDER_WEST_CELL, FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(2), EVENT(scShamanVillageCaveJumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), GAP_ABOVE_THIRD_BOULDER_EAST_CELL, FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(2), EVENT(scShamanVillageCaveJumpForward)},
    {TRIGGER_STEP, GLADIATORS_AMBUSH_CELL, FLAG_SHAMAN_VILLAGE_CAVE_FOUND_BY_GLADIATORS, EVENT(scShamanVillageCaveMeetGladiators)},
    {TRIGGER_TALK, ACTOR_EDDY, CONDITION_ALWAYS, EVENT(scShamanVillageCaveFightEddy)},
    {TRIGGER_TALK, ACTOR_SATRAGE, CONDITION_ALWAYS, EVENT(scShamanVillageCaveFightGladiators)},
    {TRIGGER_TALK, ACTOR_AZART, CONDITION_ALWAYS, EVENT(scShamanVillageCaveFightGladiators)},
    {TRIGGER_TALK, ACTOR_NAVAMPA, CONDITION_ALWAYS, EVENT(scShamanVillageCaveFightGladiators)},
    {TRIGGER_TALK, ACTOR_GOLDEN_SHIRT, CONDITION_ALWAYS, EVENT(scShamanVillageCaveTakeGoldenShirt)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_WEEDS, CONDITION_ALWAYS, EVENT(scShamanVillageCaveScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_SECOND_WEEDS, CONDITION_ALWAYS, EVENT(scShamanVillageCaveScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FIRST_BOULDER, FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(0), EVENT(scShamanVillageCaveLiftFirstBoulder)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FIRST_BOULDER + 1, FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(1), EVENT(scShamanVillageCaveLiftSecondBoulder)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FIRST_BOULDER + 2, FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(2), EVENT(scShamanVillageCaveLiftThirdBoulder)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_LAST_BOULDER, FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(3), EVENT(scShamanVillageCaveLiftFourthBoulder)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE, FLAG_SHAMAN_VILLAGE_CAVE_PUDDLE_FROZEN, EVENT(scShamanVillageCaveFreezePuddle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR, FLAG_SHAMAN_VILLAGE_CAVE_PILLAR_DROPPED, EVENT(scShamanVillageCaveStartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR, FLAG_SHAMAN_VILLAGE_CAVE_PILLAR_DROPPED, EVENT(scShamanVillageCaveFinishMovingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(scShamanVillageCaveFinishMovingPillar)},
    {TRIGGER_TABLE_END},
};

/* The beaten gladiators walk off south through the first room, one after */
/* another. */
u32 ShamanVillageCave_GladiatorLeaves[] = {
    SC_ANIMATION(2),
    SC_MOVE_XZ(232, 227),
    SC_SYNC,
    SC_MOVE_XZ(217, 243),
    SC_SYNC,
    SC_MOVE_XZ(217, 334),
    SC_SYNC,
    SC_RELEASE,
};
/* clang-format on */

/* The gladiator behind the others steps up to their path and follows them out.
 */
s32 ShamanVillageCave_GladiatorFollowsOut[] = {
	SC_ANIMATION(2),
	SC_MOVE_XZ(224, 195),
	SC_SYNC,
	SC_GOTO(ShamanVillageCave_GladiatorLeaves),
};
