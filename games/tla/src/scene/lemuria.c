/* @file */
/* Lemuria, the hidden city beyond the Sea of Time, which Piers leads the party
 */
/* into. At the top of the avenue the two palace guards stop the party until */
/* Piers speaks for it, and Kraden runs up from the paths above the spring to */
/* admire the palace, calling Felix back if he heads down the stairs through */
/* the colonnade first. The Senate, Piers's uncle's house and a girl's house */
/* open off the west of the town, the Spring of Lemuria is in the middle, and */
/* in the east stands Lunpa's tower, whose stuck front door sends the party up
 */
/* a rope to the window above it. Growth on a sprout at the cliff's edge below
 */
/* the tower grows a vine down to the cow's pasture, whose weeds Cyclone */
/* scatters (one clump hides a Hard Nut), and Scoop digs up a Lucky Medal, a */
/* Rusty Sword, a Bone and Star Dust about the town. The water ripples and its
 */
/* colours cycle. */

/* This unit declares its own form of fdActorTakeLeaderLayer, below. */
#define OWN_Actor_TakeLeaderLayer

#include "scene/event_layer.h"
#include "macros.h"
#include "field/event_inline.h"
#include "field/call.h"
#include "game/game.h"
#include "field/field_sequence_event.h"
#include "field/actor_script.h"
#include "scene/scene.h"
#include "text/msg_ids.h"
#include "field/field_sequence.h"

/* Lemuria, by the rows of ActorPlacements; Piers' row goes to his party */
/* slot. */
typedef enum LEMURIA_ACTOR_ID {
	/* The posts of the rope Lash ties up the east side of Lunpa's tower: the */
	/* low one on the grass, where the rope lies coiled, and the high one on the
	 */
	/* tower's middle balcony. */
	ACTOR_ROPE_LOW_POST = ACTOR_FIRST_PLACED,
	ACTOR_ROPE_HIGH_POST,
	ACTOR_SPROUT, /* grown into a hanging vine (FLAG_LEMURIA_VINE_GROWN) */
	ACTOR_SIGN,	  /* "House of the Senate", below the Senate's door */
	/* A girl at the south of the town, whom the events dress as Kraden to speak
	 */
	/* his lines, and as Lunpa on the top balcony of his tower; */
	/* scLemuriaRestoreGirl puts her back. */
	ACTOR_KRADEN_STAND_IN,
	/* "Lemuria has no disease, no war..." */
	ACTOR_WOMAN_ON_PARADISE,
	/* "In Lemuria, we do not..." */
	ACTOR_MAN_ON_LEMURIAN_WAYS,
	/* "Life never changes here..." */
	ACTOR_LEMURIAN_ON_UNCHANGING_LIFE,
	/* "I ought to feel surprised to see outsiders here..." */
	ACTOR_OLD_WOMAN,
	/* "How old am I again?" */
	ACTOR_GIRL_FORGETTING_HER_AGE,
	ACTOR_SPOILED_CHILD, /* scLemuriaTalkToSpoiledChild */
	ACTOR_LONELY_CHILD,	 /* scLemuriaTalkToLonelyChild */
	ACTOR_CAT,
	ACTOR_DOG,
	ACTOR_COW,
	ACTOR_CHICKEN,
	/* The two guards at the palace stair. "Who goes there!" */
	ACTOR_WEST_PALACE_GUARD,
	ACTOR_EAST_PALACE_GUARD,
	/* "If you and Lunpa..." */
	ACTOR_GIRL_ON_LUNPA,
	/* The rope Lunpa throws down from his tower's middle balcony */
	/* (FLAG_LEMURIA_ROPE_LET_DOWN): its end on the ground, and the post at the
	 */
	/* window it lies coiled at and is thrown from (fdRopePlace, fdRopeLash). */
	ACTOR_LET_DOWN_ROPE_LOW_END,
	ACTOR_LET_DOWN_ROPE_HIGH_POST,
	ACTOR_GIRL_BY_THE_SPRING,
	ACTOR_MAN_AT_THE_SPRING, /* scLemuriaOfferSpringLuck */
	ACTOR_SPRING			 /* scLemuriaDrinkFromSpring */
} LEMURIA_ACTOR_ID;

/* The cells whose step, examine and Psynergy triggers run Lemuria's events: */
/* before the palace guards, the paths west and east of the avenue above the */
/* spring, the stairs down through the colonnade, the spring's pool, Lunpa's */
/* stuck front door, the clumps of weeds Cyclone scatters and the spots Scoop */
/* digs. A door's or an exit's cells take the number of the exit they lead */
/* through (LEMURIA_TO_), which scLemuriaEnterDoor passes to */
/* syFieldEventSetExitRequest. A Scoop spot's number is its row in the scene's
 */
/* treasure spots and items. */
typedef enum LEMURIA_CELL_BEHAVIOR {
	CELL_BEHAVIOR_LEMURIA_BEFORE_PALACE = 20,
	CELL_BEHAVIOR_LEMURIA_WEST_PATH,
	CELL_BEHAVIOR_LEMURIA_EAST_PATH,
	CELL_BEHAVIOR_LEMURIA_COLONNADE_STAIRS,
	CELL_BEHAVIOR_LEMURIA_SPRING = 25,
	CELL_BEHAVIOR_LEMURIA_WEST_WEEDS = 30,
	CELL_BEHAVIOR_LEMURIA_PASTURE_WEEDS,
	CELL_BEHAVIOR_LEMURIA_WEEDS_SOUTH_OF_COW,
	CELL_BEHAVIOR_LEMURIA_WEEDS_NORTH_OF_COW,
	CELL_BEHAVIOR_LEMURIA_HARD_NUT_WEEDS = 38,
	CELL_BEHAVIOR_LEMURIA_STUCK_DOOR = 40,
	CELL_BEHAVIOR_LEMURIA_LUCKY_MEDAL = 104,
	CELL_BEHAVIOR_LEMURIA_RUSTY_SWORD,
	CELL_BEHAVIOR_LEMURIA_BONE = 107,
	CELL_BEHAVIOR_LEMURIA_STAR_DUST
} LEMURIA_CELL_BEHAVIOR;

/* Set once the party has taken the test of luck offered at the Spring of */
/* Lemuria (scLemuriaOfferSpringLuck). */
#define FLAG_LEMURIA_SPRING_LUCK_TRIED 0x908
/* Set once Growth has grown the vine (scLemuriaGrowVine); the set-up grows it
 */
/* again on every visit. */
#define FLAG_LEMURIA_VINE_GROWN 0x963
/* Set once the party has marvelled at the city ("This place is remarkable... */
/* Truly remarkable..."). */
#define FLAG_LEMURIA_CITY_ADMIRED 0x964
/* Set once Lunpa has told the party from his balcony that his door is broken */
/* and to climb through the window above it ("The door's broken, I'm afraid.").
 */
#define FLAG_LEMURIA_TOLD_DOOR_IS_BROKEN 0x968
/* Set once the Lemurian has let a rope down from the window ("There! Is that */
/* better? Now climb up here."). */
#define FLAG_LEMURIA_ROPE_LET_DOWN 0x96f

void scLemuriaTakeLeaderLayer(void);
extern s32 Lemuria_FirstWalkerPaces[];
extern s32 Lemuria_SecondWalkerPaces[];
extern u16 Lemuria_DoorOpening[];
extern u16 Lemuria_WideDoorOpening[];

#include "shared/position_distance.h"

/* Set once the party has taken the Hard Nut hidden in the pasture's weeds */
/* (Lemuria's scene item 106). */
#define FLAG_LEMURIA_HARD_NUT 0xfbb

/* Set once Lash has tied the rope between the low post and the high one; */
/* fdRopePlace keeps it tied. */
#define FLAG_LEMURIA_ROPE_TIED 0x962

/* For this visit: each Scoop spot dug, by the treasure it holds. Only four */
/* spots have a trigger; the Water of Life, Nut, Bramble Seed and Vial are */
/* never dug. */
#define FLAG_WATER_OF_LIFE_DUG_UP FLAG_AREA_TEMP_00
#define FLAG_NUT_DUG_UP			  FLAG_AREA_TEMP_01
#define FLAG_BRAMBLE_SEED_DUG_UP  FLAG_AREA_TEMP_02
#define FLAG_LUCKY_MEDAL_DUG_UP	  FLAG_AREA_TEMP_03
#define FLAG_RUSTY_SWORD_DUG_UP	  FLAG_AREA_TEMP_04
#define FLAG_BONE_DUG_UP		  FLAG_AREA_TEMP_06
#define FLAG_STAR_DUST_DUG_UP	  FLAG_AREA_TEMP_07
#define FLAG_VIAL_DUG_UP		  FLAG_AREA_TEMP_08

/* The actors of Lemuria's treasure spots, in the order of its rows in */
/* SceneItems (the scene items 101 to 109): eight buried for Scoop, and the */
/* Hard Nut in the pasture's weeds. */
typedef enum LEMURIA_TREASURE_ACTOR_ID {
	ACTOR_BURIED_WATER_OF_LIFE = FIELD_TREASURE_ACTOR,
	ACTOR_BURIED_NUT,
	ACTOR_BURIED_BRAMBLE_SEED,
	ACTOR_BURIED_LUCKY_MEDAL,
	ACTOR_BURIED_RUSTY_SWORD,
	ACTOR_HARD_NUT,
	ACTOR_BURIED_BONE,
	ACTOR_BURIED_STAR_DUST,
	ACTOR_BURIED_VIAL
} LEMURIA_TREASURE_ACTOR_ID;

#include "shared/actor_is_facing_near.h"

/* An update for the two walking children: each stands while it is talked to, */
/* while Mind Read shows its thoughts, or while it faces the other child or the
 */
/* leader close by, and walks on otherwise or once the leader is right by it. */
s32 scLemuriaNoticeLeader(ACTOR *self) {
	FIELD_STATE *event = Ram_HeapSlots->fieldState;
	u8 *state;
	u32 mode;
	ACTOR *other;

	fdActorSetSprPriorityDirect(self, fdActorGetSprPriority(fdActorGet(
										  MainRoutineStateWork.leaderActorId)));
	mode = ABILITY_ID(event->psynergy);
	state = &self->paused;
	if(mode != ABILITY_MIND_READ ||
	   (s8)((FIELD_EFFECT_STATE *)Ram_HeapSlots->fieldEffState)->msgShown ==
		   0) {
		if(self->tmr != 1) {
			other = fdActorGet(MainRoutineStateWork.leaderActorId);
			if(scPosDistance(&self->x.fixed, &other->x.fixed) <=
				   CHILD_WALKS_ON_RANGE ||
			   (fdActorIsFacingNear(self, (ACTOR *)self->watchedActor,
									CHILD_SEES_CHILD_RANGE) == 0 &&
				fdActorIsFacingNear(
					self, fdActorGet(MainRoutineStateWork.leaderActorId),
					CHILD_SEES_LEADER_RANGE) == 0)) {
				*state = 0;
				fdActorSelectAnimation(self, ANIM_WALK);
				return 0;
			}
		}
	}
	*state = 1;
	fdActorSelectAnimation(self, ANIM_STAND);
	return 0;
}

extern MAP_GRID_CELL_ANIMATION Lemuria_UnclesDoor;
extern MAP_GRID_CELL_ANIMATION Lemuria_GirlsHouseDoor;
extern MAP_GRID_CELL_ANIMATION Lemuria_SenateDoor;
extern MAP_GRID_CELL_ANIMATION Lemuria_LunpasWindow;
extern MAP_GRID_CELL_ANIMATION Lemuria_PalaceFrontDoor;
extern MAP_GRID_CELL_ANIMATION Lemuria_PalaceUpperDoor;
extern MAP_GRID_CELL_ANIMATION Lemuria_LunpasDoor;

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

const ACTOR_PLACE *scGetActorPlacements(void) { return ActorPlacements; }

/* Puts the girl who stood in for Kraden or Lunpa back in her place at the */
/* south of the town, as herself. */
void scLemuriaRestoreGirl(void) {
	fdActorSetPos(ACTOR_KRADEN_STAND_IN, 488, 1256);
	fdActorSetFirstSprAnimation(fdActorGet(ACTOR_KRADEN_STAND_IN),
								SPRITE_LEMURIAN_GIRL);
	fdActorSetAnimation(ACTOR_KRADEN_STAND_IN, 3);
	fdActorFaceDirection(ACTOR_KRADEN_STAND_IN, FACING_WEST, 0);
}

/* The door opens and Felix walks in through it, lined up with it first */
/* (`mode` 0) or straight ahead (`mode` 1), as the screen closes. */
void scLemuriaOpenDoorAndEnter(const MAP_GRID_CELL_ANIMATION *door, s32 exit,
							   s32 mode) {
	s32 id;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdEventPlaySnd(SE_DOOR_OPEN);
	fdMapGridAnimateCells(door->steps, door->x, door->y);
	fdActorGet(MainRoutineStateWork.leaderActorId)->motionFlg = ACTOR_FALLS;
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_WALK);
	id = MainRoutineStateWork.leaderActorId;
	if(mode == 0)
		fdActorCenterAndWalk(id, 2, -8);
	else
		fdActorSetDestinationOffset(id, 0, -8);
	syFieldEventWait(10);
	syFieldEventSetExitRequest(exit);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdEventEnd();
}

/* Felix walks north into one of Lemuria's doors: the door opens and he goes */
/* in. The doors are those of Piers's uncle's house left of the spring, cell */
/* (22, 47); the girl's house, cell (35, 47); the House of the Senate in the */
/* west, cell (9, 40); the window above Lunpa's door, cell (54, 45), and */
/* Lunpa's own door below it, cell (54, 50); and the palace's front and upper */
/* doors, cells (28, 20) and (28, 13). Once the party has met King Hydros the */
/* Senate's door stays shut: "The door is locked... It sounds as if there's a */
/* raucous caucus going on inside..." */
void scLemuriaEnterDoor(s32 exit) {
	const MAP_GRID_CELL_ANIMATION *door = &Lemuria_UnclesDoor;
	s32 mode = 0;

	if(exit == LEMURIA_TO_LEMURIA_ROOMS_WEST_C &&
	   fdEventGetFlag(FLAG_MET_KING_HYDROS)) {
		syFieldEventSetMessageId((s32)&MsgLemuriaDoorIsLockedItSounds);
		/* "The door is locked... It sounds as if there's a raucous caucus
		 * going..." */
		fdEventShowMessage(-1, 0);
		return;
	}
	if(exit == LEMURIA_TO_LEMURIA_ROOMS_WEST_A) {
		door = &Lemuria_UnclesDoor;
		mode = 0;
	}
	if(exit == LEMURIA_TO_LEMURIA_ROOMS_WEST_B) {
		door = &Lemuria_GirlsHouseDoor;
		mode = 0;
	}
	if(exit == LEMURIA_TO_LEMURIA_ROOMS_WEST_C) {
		door = &Lemuria_SenateDoor;
		mode = 1;
	}
	if(exit == LEMURIA_TO_LEMURIA_ROOMS_EAST_A) {
		door = &Lemuria_LunpasWindow;
		mode = 0;
	}
	if(exit == LEMURIA_TO_LEMURIA_PALACE_SOUTH) {
		door = &Lemuria_PalaceFrontDoor;
		mode = 1;
	}
	if(exit == LEMURIA_TO_LEMURIA_PALACE_NORTHEAST) {
		door = &Lemuria_PalaceUpperDoor;
		mode = 1;
	}
	if(exit == LEMURIA_TO_LEMURIA_ROOMS_EAST_C) {
		door = &Lemuria_LunpasDoor;
		mode = 0;
	}
	scLemuriaOpenDoorAndEnter(door, exit, mode);
}

/* Felix talks to the child who walks about with another in the west of the */
/* town: the child stops and turns to him, "It has been many, many years since
 */
/* a new child was born in Lemuria." Mind Read hears the child think, */
/* "Everyone spoils me so much... I don't ever want to grow up!" Afterwards */
/* the child faces the way it was walking again. */
void scLemuriaTalkToSpoiledChild(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR *actor = fdActorGet(ACTOR_SPOILED_CHILD);
	s16 angle = actor->facing;

	actor->tmr = 1;
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	if(state->psynergy == 0) {
		syFieldEventSetMessageId((s32)&MsgLemuriaItHasBeenManyMany);
		fdActorFaceEachOther(ACTOR_SPOILED_CHILD,
							 MainRoutineStateWork.leaderActorId, 2);
	}
	else {
		syFieldEventSetMessageId((s32)&MsgLemuriaEveryoneSpoilsMeSoMuch);
	}
	fdActorSetAnimation(ACTOR_SPOILED_CHILD, 0);
	/* "It has been many, many years since a new child was born in Lemuria." or,
	 */
	/* to Mind Read, "Everyone spoils me so much... I don't ever want to grow
	 * up!" */
	syFieldEventShowMessageThenWait(ACTOR_SPOILED_CHILD, 0, 10);
	fdEventEnd();
	actor->tmr = 0;
	actor->facing = angle;
	syWait(1);
}

/* Felix talks to the other walking child in the west of the town: the child */
/* stops and turns to him, "We are the only children in Lemuria. I often wish */
/* there were other children to play with." Mind Read hears the child wonder, */
/* "Are there other children in the lands beyond the Sea of Time?" */
void scLemuriaTalkToLonelyChild(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR *actor = fdActorGet(ACTOR_LONELY_CHILD);
	s16 angle = actor->facing;

	actor->tmr = 1;
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	if(state->psynergy == 0) {
		syFieldEventSetMessageId((s32)&MsgLemuriaWeAreOnlyChildren);
		fdActorFaceEachOther(ACTOR_LONELY_CHILD,
							 MainRoutineStateWork.leaderActorId, 2);
	}
	else {
		syFieldEventSetMessageId((s32)&MsgLemuriaAreThereOtherChildren);
	}
	fdActorSetAnimation(ACTOR_LONELY_CHILD, 0);
	/* "We are the only children in Lemuria. I often wish there were other..."
	 * or, */
	/* to Mind Read, "Are there other children in the lands beyond the Sea
	 * of..." */
	syFieldEventShowMessageThenWait(ACTOR_LONELY_CHILD, 0, 10);
	fdEventEnd();
	actor->tmr = 0;
	actor->facing = angle;
	syWait(1);
}

/* Felix casts Cyclone on the lone clump of weeds south-east of the spring, */
/* cells (39, 67) to (40, 68): leaves whirl up and the clump is gone, leaving */
/* bare grass. */
void scLemuriaCycloneWestWeeds(void) {
	syFieldTileCopiesCopyTouchedCells(37, 72, 2, 2, 39, 71, 2);
}

/* Felix casts Cyclone on the weeds beside the tree in the cow's pasture, below
 */
/* the cliff under Lunpa's tower, cells (49, 67) to (52, 69): leaves whirl up */
/* and the clumps are gone, leaving bare grass. */
void scLemuriaCyclonePastureWeeds(void) {
	syFieldTileCopiesCopyTouchedCells(59, 64, 4, 3, 49, 71, 2);
}

/* Felix casts Cyclone on the clump of weeds just south of the cow in its */
/* pasture, cell (53, 65): leaves whirl up and the clump is gone. */
void scLemuriaCycloneWeedsSouthOfCow(void) {
	syFieldTileCopiesCopyTouchedCells(52, 69, 1, 1, 53, 69, 2);
}

/* Felix casts Cyclone on the clump of weeds just north of the cow, at the foot
 */
/* of the cliff the vine hangs down, cell (52, 60): leaves whirl up and the */
/* clump is gone. */
void scLemuriaCycloneWeedsNorthOfCow(void) {
	syFieldTileCopiesCopyTouchedCells(53, 64, 1, 1, 52, 64, 2);
}

/* Where the Hard Nut shows once its weeds are scattered. */
enum { HARD_NUT_X = 808, HARD_NUT_Z = 1176 };

/* Felix casts Cyclone over the south of the weeds beside the tree in the */
/* cow's pasture, cell (50, 69): the clumps are gone as for */
/* scLemuriaCyclonePastureWeeds, and a Hard Nut lies in the grass where they */
/* were until it is picked up. If Felix stands on that very spot he hops a step
 */
/* east off it. */
void scLemuriaCycloneHardNutWeeds(void) {
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);

	syFieldTileCopiesCopyTouchedCells(59, 64, 4, 3, 49, 71, 2);
	if(fdEventGetFlag(FLAG_LEMURIA_HARD_NUT) == 0) {
		fdActorSetPos(ACTOR_HARD_NUT, HARD_NUT_X, HARD_NUT_Z);
		if(FIELD_CELL(leader->x.fixed) == FIELD_CELL(toFIXED(HARD_NUT_X)) &&
		   FIELD_CELL(leader->z.fixed) == FIELD_CELL(toFIXED(HARD_NUT_Z))) {
			fdEventStart();
			syFieldEventEndLastingPsynergy(0);
			fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
									 ATTACHED_EFFECT_IN_FRONT |
										 ATTACHED_EFFECT_ONCE);
			fdActorSetDestinationOffset(MainRoutineStateWork.leaderActorId, 16,
										0);
			leader->velY = toFIXED(6);
			scFieldActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
			fdEventEnd();
		}
	}
}

/* Unused: teleports the party, with sparks and a fade, to the door of the */
/* girl's house. No trigger or call of the scene reaches it. */
UNUSED void scLemuriaTeleportToGirlsHouse(void) {
	MAIN_ROUTINE_STATE *party;

	fdTeleportPlaySparksAndFade();
	fdEventSetDestination(SCENE_LEMURIA, LEMURIA_FROM_LEMURIA_ROOMS_WEST_B);
	party = &MainRoutineStateWork;

	party->teleportState = TELEPORT_ARRIVING;
}

/* Felix casts Growth on the sprout at the cliff's edge below Lunpa's tower: a
 */
/* vine grows down the cliff, and from then on Felix can climb it down to the */
/* cow's pasture and back up. The vine stays grown. */
void scLemuriaGrowVine(void) {
	FIXED x;
	FIXED z;
	s32 attribute;

	fdEventSetFlag(FLAG_LEMURIA_VINE_GROWN);
	x = toFIXED(856);
	z = toFIXED(1016);
	attribute = fdMapGridGetCellAttributesAt(x, z, 0);
	fdMapGridSetCellAttributesAt(x, z, 0, attribute | MAP_CELL_CLIMBABLE);
	z = toFIXED(1032);
	fdMapGridSetCellAttributesAt(
		x, z, 0, fdMapGridGetCellAttributesAt(x, z, 0) | MAP_CELL_CLIMBABLE);
}

/* Unused: Scoop digs up the Water of Life of Lemuria's first treasure spot. No
 */
/* trigger reaches it. */
UNUSED void scLemuriaScoopWaterOfLife(s32 id) {
	fdEventDigUpTreasure(id, ACTOR_BURIED_WATER_OF_LIFE,
						 FLAG_WATER_OF_LIFE_DUG_UP);
}

/* Unused: Scoop digs up the Nut of Lemuria's second treasure spot. No trigger
 */
/* reaches it. */
UNUSED void scLemuriaScoopNut(s32 id) {
	fdEventDigUpTreasure(id, ACTOR_BURIED_NUT, FLAG_NUT_DUG_UP);
}

/* Unused: Scoop digs up the Bramble Seed of Lemuria's third treasure spot. No
 */
/* trigger reaches it. */
UNUSED void scLemuriaScoopBrambleSeed(s32 id) {
	fdEventDigUpTreasure(id, ACTOR_BURIED_BRAMBLE_SEED,
						 FLAG_BRAMBLE_SEED_DUG_UP);
}

/* Felix casts Scoop on the patch of ground right of the Spring of Lemuria, */
/* where a butterfly flutters, cell (33, 61): he digs up a Lucky Medal. */
void scLemuriaScoopLuckyMedal(s32 id) {
	fdEventDigUpTreasure(id, ACTOR_BURIED_LUCKY_MEDAL, FLAG_LUCKY_MEDAL_DUG_UP);
}

/* Felix casts Scoop on the patch of ground near Lunpa's tower in the east of */
/* the town, where a butterfly flutters, cell (55, 58): he digs up a Rusty */
/* Sword. */
void scLemuriaScoopRustySword(s32 id) {
	fdEventDigUpTreasure(id, ACTOR_BURIED_RUSTY_SWORD, FLAG_RUSTY_SWORD_DUG_UP);
}

/* Felix casts Scoop on the patch of ground near the dog, where a butterfly */
/* flutters, cell (23, 38): he digs up a Bone. */
void scLemuriaScoopBone(s32 id) {
	fdEventDigUpTreasure(id, ACTOR_BURIED_BONE, FLAG_BONE_DUG_UP);
}

/* Felix casts Scoop on the patch of ground left of the palace, where a */
/* butterfly flutters, cell (18, 26): he digs up Star Dust. */
void scLemuriaScoopStarDust(s32 id) {
	fdEventDigUpTreasure(id, ACTOR_BURIED_STAR_DUST, FLAG_STAR_DUST_DUG_UP);
}

/* Unused: Scoop digs up the Vial of Lemuria's ninth treasure spot. No trigger
 */
/* reaches it. */
UNUSED void scLemuriaScoopVial(s32 id) {
	fdEventDigUpTreasure(id, ACTOR_BURIED_VIAL, FLAG_VIAL_DUG_UP);
}

/* Felix checks the front door of Lunpa's tower in the east of the town, cell */
/* (54, 52): "The door doesn't want to open." Once Piers has gone to his */
/* mother's grave, the first time, Lunpa looks down from the top balcony and */
/* asks whether they are Piers's traveling companions: "The door's */
/* broken, I'm afraid. But I opened up a window just above the door." If no */
/* one in the party has Lash and the rope east of the tower is not tied, Felix
 */
/* just looks up, and Lunpa throws a rope down from the middle balcony: "There!
 */
/* Is that better? Now climb up here." From up on the tower, or once Lunpa */
/* has burst the door open, checking it does nothing. A girl of the town stands
 */
/* in for Lunpa. */
void scLemuriaTryStuckDoor(void) {
	ACTOR *actor;

	if(fdActorGet(MainRoutineStateWork.leaderActorId)->y.fixed > toFIXED(96) ||
	   fdEventGetFlag(FLAG_MET_LUNPA)) {
		FieldStateWork->fieldMenu = 1;
		return;
	}
	if(!fdEventGetFlag(FLAG_PIERS_AT_MOTHERS_GRAVE) ||
	   fdEventGetFlag(FLAG_LEMURIA_TOLD_DOOR_IS_BROKEN)) {
		syFieldEventSetMessageId((s32)&MsgLemuriaDoorDoesntWantToOpen);
		/* "The door doesn't want to open." */
		fdEventShowMessage(-1, 0);
		return;
	}
	fdEventSetFlag(FLAG_LEMURIA_TOLD_DOOR_IS_BROKEN);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	syFieldEventSetMessageId((s32)&MsgLemuriaAreYouKidsPierssTraveling);
	fdActorSetPos(ACTOR_KRADEN_STAND_IN, 872, 880);
	actor = fdActorGet(ACTOR_KRADEN_STAND_IN);
	fdActorSetFirstSprAnimation(actor, SPRITE_LUNPA);
	fdActorSetAnimation(ACTOR_KRADEN_STAND_IN, 3);
	fdActorFaceDirection(ACTOR_KRADEN_STAND_IN, FACING_SOUTH_SOUTHWEST, 0);
	/* "Are you kids Piers's traveling companions?" */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 10);
	fdCameraMoveTo(872, CAMERA_KEEP, 776, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(20);
	fdActorShowEmote(ACTOR_KRADEN_STAND_IN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP,
					 30);
	/* "The door's broken, I'm afraid." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_KRADEN_STAND_IN, 4);
	/* "But I opened up a window just above the door." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 10);
	fdCameraMoveTo(872, CAMERA_KEEP, 856, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(20);
	/* "Sorry about this, but you'll have to climb through there." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 10);
	fdCameraMoveTo(872, CAMERA_KEEP, 928, 1);
	fdCameraWaitForArrival();
	if(gmPartyFindOwnerWithAction(ABILITY_LASH) == -1 &&
	   !fdEventGetFlag(FLAG_LEMURIA_ROPE_TIED)) {
		syFieldEventSetMessageId((s32)&MsgLemuriaWhatsMatterCantYouClimb);
		syFieldEventWait(20);
		fdActorWalkToAndWait(ACTOR_FELIX, 856, 952);
		fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_NORTH);
		syFieldEventWait(10);
		fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
		fdCameraMoveTo(872, CAMERA_KEEP, 792, 1);
		fdCameraWaitForArrival();
		syFieldEventWait(20);
		fdActorShowEmote(ACTOR_KRADEN_STAND_IN, EMOTE_IN_FRONT | EMOTE_QUESTION,
						 30);
		/* "What's the matter? Can't you climb up? ...Fine. Just a moment." */
		syFieldEventShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 10);
		fdCameraMoveTo(872, CAMERA_KEEP, 856, 1);
		scFieldActorJump(ACTOR_KRADEN_STAND_IN, 4, 0);
		fdActorWalkToAndWait(ACTOR_KRADEN_STAND_IN, 872, 912);
		syFieldEventWait(20);
		fdActorSetSpeed(ACTOR_KRADEN_STAND_IN, 1);
		fdActorWalkByAndWait(ACTOR_KRADEN_STAND_IN, -32, 0);
		fdActorFaceDirectionAndWait(ACTOR_KRADEN_STAND_IN,
									FACING_SOUTH_SOUTHEAST);
		fdCameraMoveTo(848, CAMERA_KEEP, 904, 1);
		fdCameraWaitForArrival();
		syFieldEventWait(15);
		/* "You came all this way and then got stuck here? You can't be very */
		/* bright." */
		syFieldEventShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 5);
		fdActorSetPos(ACTOR_LET_DOWN_ROPE_HIGH_POST, 840, 920);
		fdActorSetPos(ACTOR_LET_DOWN_ROPE_LOW_END, 840, 936);
		Value3((s32 (*)())fdRopeCoil, 1, ACTOR_LET_DOWN_ROPE_HIGH_POST,
			   ACTOR_LET_DOWN_ROPE_LOW_END);
		fdRopeLash(1, ACTOR_LET_DOWN_ROPE_HIGH_POST,
				   ACTOR_LET_DOWN_ROPE_LOW_END);
		syFieldEventWait(30);
		/* "There! Is that better? Now climb up here." */
		syFieldEventShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 5);
		fdActorWalkByAndWait(ACTOR_KRADEN_STAND_IN, 32, 0);
		syEventCameraFollowActor(ACTOR_FELIX, 1);
		fdActorWalkByAndWait(ACTOR_KRADEN_STAND_IN, 0, -32);
		fdEventSetFlag(FLAG_LEMURIA_ROPE_LET_DOWN);
	}
	scLemuriaRestoreGirl();
	fdEventEnd();
}

/* Felix drinks from the spring in the middle of the town, the turtles' pool */
/* beside the man who offers the test of luck, cells (27, 57) to (30, 58): */
/* "Felix and the others drank of the spring. Their HP and PP were fully */
/* replenished!" From up on the spring's rim nothing happens. */
void scLemuriaDrinkFromSpring(void) {
	FIXED y = fdActorGet(MainRoutineStateWork.leaderActorId)->y.fixed;

	if(y < toFIXED(90)) {
		/* "Felix and the others drank of the spring. Their HP and PP were..."
		 */
		mnMessageShowCentered((s32)MsgAndTheOthers, 1);
		fdEventPlaySnd(SE_RECOVERY);
		gmPartyRestoreHpPp(1);
	}
	else {
		FieldStateWork->fieldMenu = 1;
	}
}

/* Felix talks to the man before the spring: "Do you wish to test your luck at
 */
/* the Spring of Lemuria?" Said no, he will take a turn himself; said yes, he */
/* steps aside so that the party can walk up to the spring, and from then on */
/* he only wishes the party good luck. */
void scLemuriaOfferSpringLuck(void) {
	if(fdEventGetFlag(FLAG_LEMURIA_SPRING_LUCK_TRIED)) {
		fdActorFaceActor(ACTOR_MAN_AT_THE_SPRING,
						 MainRoutineStateWork.leaderActorId, 0);
		scFieldActorWaitForScriptEndById(ACTOR_MAN_AT_THE_SPRING);
		syFieldEventSetMessageId((s32)MsgLemuriaYouLookLikeYoure);
		/* "You look like you're in for a spell of good luck. After all,
		 * your..." */
		fdEventShowMessage(ACTOR_MAN_AT_THE_SPRING, 0);
		fdActorFaceDirectionAndWait(ACTOR_MAN_AT_THE_SPRING,
									FACING_NORTH_NORTHEAST);
		return;
	}
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	syFieldEventSetMessageId((s32)MsgLemuriaDoYouWishToTest);
	fdActorShowEmote(ACTOR_MAN_AT_THE_SPRING,
					 EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorFaceActor(ACTOR_MAN_AT_THE_SPRING,
					 MainRoutineStateWork.leaderActorId, 0);
	scFieldActorWaitForScriptEndById(ACTOR_MAN_AT_THE_SPRING);
	syFieldEventWait(5);
	/* "Do you wish to test your luck at the Spring of Lemuria?" */
	fdEventOpenMessage(ACTOR_MAN_AT_THE_SPRING, 0);
	if(syFieldEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0)) {
		syFieldEventWait(30);
		/* "Well, then, I'll just take a turn... If you change your mind, let
		 * me..." */
		syFieldEventShowMessageThenWait(ACTOR_MAN_AT_THE_SPRING, 0, 0);
	}
	else {
		syFieldEventWait(20);
		fdActorSetAnimationAndWait(ACTOR_MAN_AT_THE_SPRING, 3);
		(FieldStateWork->msg)++;
		/* "Go ahead. Test your luck!" */
		syFieldEventShowMessageThenWait(ACTOR_MAN_AT_THE_SPRING, 0, 5);
		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 464, 1040);
		fdActorFaceDirectionAndWait(MainRoutineStateWork.leaderActorId,
									FACING_NORTH);
		syFieldEventWait(5);
		fdActorSetSpeed(ACTOR_MAN_AT_THE_SPRING, 1.2);
		fdActorWalkToAndWait(ACTOR_MAN_AT_THE_SPRING, 440, 1048);
		fdEventSetFlag(FLAG_LEMURIA_SPRING_LUCK_TRIED);
	}
	fdActorFaceDirectionAndWait(ACTOR_MAN_AT_THE_SPRING,
								FACING_NORTH_NORTHEAST);
	fdEventEnd();
}

const TRIGGER *scGetTriggers(void) { return SceneEvents; }

/* Which of Lemuria_RippleLines' two tables the HBlank DMA reads; the other is
 */
/* filled for the next frame. */
u8 Lemuria_RippleBuffer BSS_DATA;
/* The water's two tables of BG2 horizontal offsets, a halfword a screen line.
 */
s16 Lemuria_RippleLines[2][160] BSS_DATA ALIGNED(16);

/* Background palette `n`, as the water's colour cycle turns it. */
#define WATER_PALETTE(n) ((volatile u16 *)&BG_PLTT_COLOR(n, 0))

/* The water's cycling colours in each of its palettes, 1 to 14: all but the */
/* palette's first and last. */
enum { WATER_CYCLE_FIRST_COLOR = 1, WATER_CYCLE_LAST_COLOR = 14 };

/* Every fourth frame rotates the water's colours of background palettes 7 and
 */
/* 8 by one place. */
void scLemuriaCycleWaterCols(void) {
	if((FrameCount & 3) == 0) {
		s16 saved;
		s32 i;

		saved = WATER_PALETTE(7)[WATER_CYCLE_LAST_COLOR];
		for(i = WATER_CYCLE_LAST_COLOR; i != WATER_CYCLE_FIRST_COLOR; i--)
			WATER_PALETTE(7)[i] = WATER_PALETTE(7)[i - 1];
		WATER_PALETTE(7)[WATER_CYCLE_FIRST_COLOR] = saved;
		saved = WATER_PALETTE(8)[WATER_CYCLE_LAST_COLOR];
		for(i = WATER_CYCLE_LAST_COLOR; i != WATER_CYCLE_FIRST_COLOR; i--)
			WATER_PALETTE(8)[i] = WATER_PALETTE(8)[i - 1];
		WATER_PALETTE(8)[WATER_CYCLE_FIRST_COLOR] = saved;
	}
}

/* Puts the per-line BG2 horizontal scroll on an HBlank DMA. */
void scLemuriaStartRippleDma(void) {
	s16 *lines = Lemuria_RippleLines[Lemuria_RippleBuffer];
	volatile u16 *dma;

	syIoWrite16(lines[0], &REG_BG2HOFS);
	dma = REG_DMA0;

	dma[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
	dma[5] &= ~DMA_ENABLE;
	lines++;
	(void)dma[5];
	syDMAXCopy(lines, (void *)&REG_BG2HOFS, DMA_HBLANK_HALFWORD_PER_LINE,
			   (volatile u32 *)dma);
}

/* The ripple's sway: a sine shifted down to at most 2 pixels either way. */
enum { RIPPLE_SWAY_SHIFT = 15 };

/* A task: fills the back buffer of line scrolls with the water's ripple, a */
/* sine of the line and the frame, and swaps the buffers. */
void scLemuriaFillRippleLines(void) {
	MAP_LAYER *layer = fdMapGridGetLayer(1);
	s16 *lines = Lemuria_RippleLines[Lemuria_RippleBuffer ^ 1];
	FIXED v[2];
	s32 *count = &v[1];
	FIXED sine;

	for(*count = 0; *count != DISPLAY_HEIGHT; (*count)++) {
		sine = syMathSin((*count + (u8)FrameCount + FIXEDtoINT(layer->y)) << 9);
		v[0] = sine;
		*lines++ =
			FIXEDtoINT(layer->x) + (sine >> RIPPLE_SWAY_SHIFT) + *count % 3 - 1;
	}
	Lemuria_RippleBuffer ^= 1;
}

/* Starts the water's ripple: its line scrolls each VBlank and their DMA each */
/* frame. */
void scLemuriaStartRipple(void) {
	s32 priority;

	priority = TASK_PASS_FRAME;
	syTaskAddFrame(scLemuriaFillRippleLines, priority);
	priority = TASK_PASS_VBLANK;
	syTaskAddFrame(scLemuriaStartRippleDma, priority);
}

/* Sets up the town: the water's colours cycling, the two children walking */
/* together, the palace guards, the spring's man, Piers by his mother's grave,
 */
/* the ropes, the vine and Lunpa's opened window as the story leaves them. */
s32 scInit(void) {
	ACTOR *actor;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	syFieldTaskAddFrame((s32)scLemuriaCycleWaterCols, TASK_PASS_VBLANK);
	fdEffectStartBlankOverlay(0);
	fdActorSetSprFlags(fdActorGet(ACTOR_SIGN), 0);
	scFieldActorSetSprPriority(ACTOR_ROPE_HIGH_POST, 3);
	scFieldActorSetSprPriority(ACTOR_GIRL_BY_THE_SPRING, 3);
	fdActorGet(ACTOR_DOG)->update = scLemuriaTakeLeaderLayer;
	fdActorSetAnimation(ACTOR_COW, 3);
	fdActorSetPos(ACTOR_SPOILED_CHILD, 40, 952);
	fdActorSetScript(ACTOR_SPOILED_CHILD, Lemuria_FirstWalkerPaces);
	actor = fdActorGet(ACTOR_SPOILED_CHILD);
	actor->watchedActor = (union FIELD_OBJECT *)fdActorGet(ACTOR_LONELY_CHILD);
	actor->update = scLemuriaNoticeLeader;
	fdActorSetPos(ACTOR_LONELY_CHILD, 88, 904);
	fdActorSetScript(ACTOR_LONELY_CHILD, Lemuria_SecondWalkerPaces);
	actor = fdActorGet(ACTOR_LONELY_CHILD);
	actor->watchedActor = (union FIELD_OBJECT *)fdActorGet(ACTOR_SPOILED_CHILD);
	actor->update = scLemuriaNoticeLeader;
	if(fdEventGetFlag(FLAG_LEMURIA_SPRING_LUCK_TRIED)) {
		fdActorSetPos(ACTOR_MAN_AT_THE_SPRING, 440, 1048);
	}
	if(fdEventGetFlag(FLAG_LEMURIA_CITY_ADMIRED) &&
	   !fdEventGetFlag(FLAG_MET_LUNPA)) {
		fdActorSetPos(ACTOR_WEST_PALACE_GUARD, 456, 520);
		fdActorSetPos(ACTOR_EAST_PALACE_GUARD, 472, 520);
	}
	if(!fdEventGetFlag(FLAG_MET_LUNPA)) {
		fdActorSetPos(ACTOR_GIRL_ON_LUNPA, 0, 0);
	}
	else {
		fdActorGet(ACTOR_GIRL_ON_LUNPA)->update = scLemuriaTakeLeaderLayer;
	}
	if(fdEventGetFlag(FLAG_PIERS_AT_MOTHERS_GRAVE) &&
	   !fdEventGetFlag(FLAG_MET_LUNPA)) {
		fdActorSetPos(ACTOR_PIERS, 56, 1016);
		fdActorSetAnimation(ACTOR_PIERS, 9);
	}
	fdRopeInit();
	fdRopePlace(0, FLAG_LEMURIA_ROPE_TIED, ACTOR_ROPE_LOW_POST,
				ACTOR_ROPE_HIGH_POST);
	fdActorGet(ACTOR_ROPE_LOW_POST)->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	fdActorGet(ACTOR_ROPE_HIGH_POST)->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	if(fdEventGetFlag(FLAG_LEMURIA_ROPE_LET_DOWN)) {
		scFieldActorSetSprPriority(ACTOR_LET_DOWN_ROPE_HIGH_POST, 2);
		scFieldActorSetSprPriority(ACTOR_LET_DOWN_ROPE_LOW_END, 2);
		fdActorSetPos(ACTOR_LET_DOWN_ROPE_HIGH_POST, 840, 920);
		fdActorSetPos(ACTOR_LET_DOWN_ROPE_LOW_END, 840, 936);
		Call4(fdRopePlace, 1, FLAG_LEMURIA_ROPE_LET_DOWN,
			  ACTOR_LET_DOWN_ROPE_HIGH_POST, ACTOR_LET_DOWN_ROPE_LOW_END);
	}
	fdActorSetSprFlags(fdActorGet(ACTOR_LET_DOWN_ROPE_HIGH_POST), 0);
	if(fdEventGetFlag(FLAG_LEMURIA_VINE_GROWN)) {
		fdHangingVinePlace(ACTOR_SPROUT, 1);
		scLemuriaGrowVine();
	}
	/* Each treasure still buried has its actor taken off to the map's corner */
	/* and marked as standing for a map cell instead. */
	if(!fdEventGetFlag(FLAG_LUCKY_MEDAL_DUG_UP)) {
		fdActorSetWideSpr(ACTOR_BURIED_LUCKY_MEDAL, TREASURE_SPOT_PUT_AWAY);
	}
	if(!fdEventGetFlag(FLAG_RUSTY_SWORD_DUG_UP)) {
		fdActorSetWideSpr(ACTOR_BURIED_RUSTY_SWORD, TREASURE_SPOT_PUT_AWAY);
	}
	if(!fdEventGetFlag(FLAG_BONE_DUG_UP)) {
		fdActorSetWideSpr(ACTOR_BURIED_BONE, TREASURE_SPOT_PUT_AWAY);
	}
	if(!fdEventGetFlag(FLAG_STAR_DUST_DUG_UP)) {
		fdActorSetWideSpr(ACTOR_BURIED_STAR_DUST, TREASURE_SPOT_PUT_AWAY);
	}
	/* Lunpa has opened the window above his door. */
	if(fdEventGetFlag(FLAG_PIERS_AT_MOTHERS_GRAVE)) {
		fdMapGridCopyCellsLayered(3, 1, 1, 1, 54, 45);
	}
	actor = fdActorGet(ACTOR_SPRING);
	actor->motionFlg = 0;
	actor->groundHeight = toFIXED(64);
	actor->y.fixed = toFIXED(64);
	return 0;
}

s32 scPrepareMap(void) { return 0; }

/* Felix steps onto the stone path west of the spring's avenue, cells (22, 56)
 */
/* to (23, 57), or east of it, cells (34, 56) to (35, 57), before the party has
 */
/* gone up to the palace: Kraden comes out, looks round, "Oh! Goodness!", and */
/* runs up the avenue and the stairs through the colonnade, jumping for joy: */
/* "Felix! Look over here! This palace is magnificient!" Felix follows him up */
/* the stairs. */
void scLemuriaAdmirePalace(s32 id) {
	ACTOR *leader;
	s32 a;
	s32 b;
	s32 c;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	syFieldEventSetMessageId((s32)MsgLemuriaOh);
	fdActorSetFirstSprAnimation(fdActorGet(ACTOR_KRADEN_STAND_IN),
								SPRITE_KRADEN);
	if(id == CELL_BEHAVIOR_LEMURIA_WEST_PATH) {
		fdActorWalkToAndWait(ACTOR_FELIX, 376, 968);
		fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
		scFieldActorLaunchFromLeader(ACTOR_KRADEN_STAND_IN, 16, -16,
									 FACING_EAST);
		a = FACING_EAST;
		b = FACING_WEST + FACING_NUDGE;
		c = FACING_SOUTH_SOUTHWEST;
	}
	else {
		fdActorWalkToAndWait(ACTOR_FELIX, 552, 968);
		fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
		scFieldActorLaunchFromLeader(ACTOR_KRADEN_STAND_IN, -16, -16,
									 FACING_WEST);
		a = FACING_WEST;
		b = FACING_EAST - FACING_NUDGE;
		c = FACING_SOUTH_SOUTHEAST;
	}
	scFieldActorWaitForMoveById(ACTOR_KRADEN_STAND_IN);
	scFieldActorWatchAndSetScript(ACTOR_FELIX, ACTOR_KRADEN_STAND_IN);
	fdActorSetSpeed(ACTOR_KRADEN_STAND_IN, 1);
	syEventCameraFollowActor(ACTOR_KRADEN_STAND_IN, 1);
	fdActorWalkToAndWait(ACTOR_KRADEN_STAND_IN, 464, 952);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN_STAND_IN, a);
	syFieldEventWait(10);
	fdActorShowEmote(ACTOR_KRADEN_STAND_IN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
					 30);
	/* "Oh!" */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN_STAND_IN, b);
	syFieldEventWait(10);
	fdActorShowEmote(ACTOR_KRADEN_STAND_IN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
					 30);
	/* "Goodness!" */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 5);
	fdActorWalkToAndWait(ACTOR_KRADEN_STAND_IN, 464, 784);
	syFieldEventWait(10);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	scFieldActorJump(ACTOR_KRADEN_STAND_IN, 6, 15);
	scFieldActorJump(ACTOR_KRADEN_STAND_IN, 6, 23);
	syFieldEventWait(10);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN_STAND_IN, c);
	syFieldEventWait(10);
	/* "Felix! Look over here! This palace is magnificient!" */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 5);
	syFieldEventWait(20);
	scFieldActorSetSprPriority(ACTOR_FELIX, 1);
	fdActorSetPos(ACTOR_FELIX, 464, 856);
	fdActorWalkToAndWait(ACTOR_FELIX, 464, 796);
	syFieldEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_KRADEN_STAND_IN, 3);
	syFieldEventWait(10);
	fdActorSetSpeed(ACTOR_KRADEN_STAND_IN, 1.2);
	fdActorSetAnimation(ACTOR_KRADEN_STAND_IN, 2);
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestination(ACTOR_KRADEN_STAND_IN, leader->x.part[0].integer,
							  leader->z.part[0].integer);
	scFieldActorWaitForMoveById(ACTOR_KRADEN_STAND_IN);
	fdActorSetPos(ACTOR_KRADEN_STAND_IN, 0, 0);
	syFieldEventWait(10);
	fdActorGet(ACTOR_FELIX)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorWalkToAndWait(ACTOR_FELIX, 464, 776);
	scLemuriaRestoreGirl();
	fdEventEnd();
}

/* Felix heads down the stairs through the colonnade, cells (28, 45) and */
/* (29, 45), before the party has gone up to the palace: Kraden comes up behind
 */
/* him, "Where are you going, Felix? Let's take a longer look around!", and */
/* Felix steps back up. */
void scLemuriaKeepLookingAround(void) {
	ACTOR *leader;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	syFieldEventSetMessageId((s32)MsgLemuriaWhereAreYouGoingFelix);
	fdActorSetFirstSprAnimation(fdActorGet(ACTOR_KRADEN_STAND_IN),
								SPRITE_KRADEN);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
	scFieldActorSetSprPriority(ACTOR_KRADEN_STAND_IN, 1);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN_STAND_IN, 0, 16,
								 FACING_NORTH_NORTHEAST);
	scFieldActorWaitForMoveById(ACTOR_KRADEN_STAND_IN);
	syFieldEventWait(5);
	/* "Where are you going, Felix? Let's take a longer look around!" */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 5);
	fdActorSetSpeed(ACTOR_KRADEN_STAND_IN, 1.2);
	fdActorSetAnimation(ACTOR_KRADEN_STAND_IN, 2);
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestination(ACTOR_KRADEN_STAND_IN, leader->x.part[0].integer,
							  leader->z.part[0].integer);
	scFieldActorWaitForMoveById(ACTOR_KRADEN_STAND_IN);
	fdActorSetPos(ACTOR_KRADEN_STAND_IN, 0, 0);
	syFieldEventWait(10);
	fdActorWalkByAndWait(ACTOR_FELIX, 0, -16);
	scLemuriaRestoreGirl();
	fdEventEnd();
}

/* Felix walks up to the palace at the top of the avenue, cells (28, 33) and */
/* (29, 33), the first time: Kraden marvels, "This place is remarkable... */
/* Truly remarkable...", and the two guards at the palace stair stop the */
/* party, "Who goes there! I've not seen you in Lemuria before!", until Piers */
/* steps forward and asks to see King Hydros. */
void scLemuriaMarvelAtCity(void) {
	ACTOR *leader;
	s32 result;

	fdEventSetFlag(FLAG_LEMURIA_CITY_ADMIRED);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	syFieldEventSetMessageId((s32)MsgLemuriaThisPlaceIsRemarkableTruly);
	fdActorSetSpeed(ACTOR_WEST_PALACE_GUARD, 1);
	fdActorSetSpeed(ACTOR_EAST_PALACE_GUARD, 1);
	fdActorSetFirstSprAnimation(fdActorGet(ACTOR_KRADEN_STAND_IN),
								SPRITE_KRADEN);
	fdActorWalkToAndWait(ACTOR_FELIX, 472, 568);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN_STAND_IN, -16, 0,
								 FACING_NORTH_NORTHEAST);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	scFieldActorWaitForMoveById(ACTOR_KRADEN_STAND_IN);
	fdCameraMoveTo(464, CAMERA_KEEP, 544, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(20);
	scFieldActorJump(ACTOR_KRADEN_STAND_IN, 6, 15);
	scFieldActorJump(ACTOR_KRADEN_STAND_IN, 6, 23);
	/* "This place is remarkable... Truly remarkable..." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 10);
	fdActorShowEmote(ACTOR_WEST_PALACE_GUARD,
					 EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorWalkByAndWait(ACTOR_WEST_PALACE_GUARD, 16, 0);
	fdActorFaceDirection(ACTOR_WEST_PALACE_GUARD, FACING_SOUTH_SOUTHEAST, 0);
	syFieldEventWait(20);
	/* "Who goes there! I've not seen you in Lemuria before!" */
	syFieldEventShowMessageThenWait(ACTOR_WEST_PALACE_GUARD, 0, 10);
	fdActorFaceDirection(ACTOR_KRADEN_STAND_IN, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
	syFieldEventWait(45);
	fdActorFaceDirection(ACTOR_KRADEN_STAND_IN, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	syFieldEventWait(10);
	fdActorWalkByAndWait(ACTOR_EAST_PALACE_GUARD, -16, 0);
	/* "Wait a moment..." */
	syFieldEventShowMessageThenWait(ACTOR_EAST_PALACE_GUARD, 0, 10);
	fdActorWalkToAndWait(ACTOR_EAST_PALACE_GUARD, 464, 552);
	fdActorFaceDirection(ACTOR_EAST_PALACE_GUARD, FACING_SOUTH_SOUTHEAST, 0);
	syFieldEventWait(30);
	scFieldActorStartRepeatedMotion(ACTOR_FELIX, 3);
	fdActorSetAttachedEffect(ACTOR_FELIX,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(40);
	scFieldActorSetSprPriority(ACTOR_EAST_PALACE_GUARD, 2);
	fdActorWalkByAndWait(ACTOR_EAST_PALACE_GUARD, 16, 0);
	fdActorFaceDirection(ACTOR_EAST_PALACE_GUARD, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(30);
	scFieldActorStartRepeatedMotion(ACTOR_FELIX, 3);
	fdActorSetAttachedEffect(ACTOR_FELIX,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(40);
	fdActorWalkByAndWait(ACTOR_EAST_PALACE_GUARD, -8, 0);
	fdActorFaceDirection(ACTOR_EAST_PALACE_GUARD, FACING_SOUTH_SOUTHEAST, 0);
	syFieldEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_EAST_PALACE_GUARD, 3);
	syFieldEventWait(20);
	fdActorWalkToAndWait(ACTOR_EAST_PALACE_GUARD, 472, 520);
	fdActorFaceDirection(ACTOR_EAST_PALACE_GUARD, FACING_WEST, 0);
	syFieldEventWait(20);
	/* "You must be the travelers who arrived with Piers. Is your name..." */
	syFieldEventShowMessageThenWait(ACTOR_EAST_PALACE_GUARD, 0, 10);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_KRADEN_STAND_IN, 3);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_WEST_PALACE_GUARD, FACING_EAST, 0);
	syFieldEventWait(20);
	fdActorShowEmote(ACTOR_WEST_PALACE_GUARD, EMOTE_IN_FRONT | EMOTE_QUESTION,
					 30);
	/* "Piers? You mean Piers, the Lost One?" */
	syFieldEventShowMessageThenWait(ACTOR_WEST_PALACE_GUARD, 0, 10);
	scFieldActorLaunchFromLeader(ACTOR_PIERS, -8, -16, FACING_NORTH);
	scFieldActorWaitForMoveById(ACTOR_PIERS);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	syFieldEventWait(10);
	/* "Have I earned a title in my absence? Well, worry not! Piers is lost..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	scFieldActorStartRepeatedMotion(ACTOR_WEST_PALACE_GUARD, 3);
	fdActorSetAttachedEffect(ACTOR_WEST_PALACE_GUARD,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	scFieldActorStartRepeatedMotion(ACTOR_EAST_PALACE_GUARD, 3);
	fdActorSetAttachedEffect(ACTOR_EAST_PALACE_GUARD,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(45);
	fdActorFaceDirection(ACTOR_WEST_PALACE_GUARD, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_EAST_PALACE_GUARD, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	syFieldEventWait(10);
	/* "I seek counsel with King Hydros. Has His Highness been told of my..." */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorFaceDirection(ACTOR_WEST_PALACE_GUARD, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_EAST_PALACE_GUARD, FACING_WEST, 0);
	syFieldEventWait(35);
	fdActorFaceDirection(ACTOR_EAST_PALACE_GUARD, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_EAST_PALACE_GUARD, 3);
	syFieldEventWait(10);
	/* "Word was sent, but I do not know if it has reached the king." */
	syFieldEventShowMessageThenWait(ACTOR_EAST_PALACE_GUARD, 0, 10);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "What do you mean?" */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorFaceDirection(ACTOR_WEST_PALACE_GUARD, FACING_SOUTH_SOUTHEAST, 0);
	syFieldEventWait(20);
	/* "Lord Conservato is meeting with His Highness as we speak." */
	syFieldEventShowMessageThenWait(ACTOR_WEST_PALACE_GUARD, 0, 10);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "Not Conservato..." */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorShowEmote(ACTOR_KRADEN_STAND_IN, EMOTE_IN_FRONT | EMOTE_QUESTION,
					 30);
	/* "What's wrong, Piers? You don't look pleased to hear that name..." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_PIERS, 2);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	syFieldEventWait(30);
	/* "Kraden... What do you think of what you have seen of Lemuria?" */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorShowEmote(ACTOR_KRADEN_STAND_IN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
					 60);
	scFieldActorWatchAndSetScript(ACTOR_FELIX, ACTOR_KRADEN_STAND_IN);
	scFieldActorWatchAndSetScript(ACTOR_PIERS, ACTOR_KRADEN_STAND_IN);
	scFieldActorWatchAndSetScript(ACTOR_WEST_PALACE_GUARD,
								  ACTOR_KRADEN_STAND_IN);
	scFieldActorWatchAndSetScript(ACTOR_EAST_PALACE_GUARD,
								  ACTOR_KRADEN_STAND_IN);
	fdActorSetSpeed(ACTOR_KRADEN_STAND_IN, 1);
	syEventCameraFollowActor(ACTOR_KRADEN_STAND_IN, 1);
	fdActorWalkToAndWait(ACTOR_KRADEN_STAND_IN, 424, 600);
	syFieldEventWait(20);
	fdCameraMoveTo(400, CAMERA_KEEP, 648, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_KRADEN_STAND_IN, FACING_SOUTH_SOUTHEAST, 0);
	syFieldEventWait(40);
	fdActorFaceDirection(ACTOR_KRADEN_STAND_IN, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(40);
	fdActorFaceDirection(ACTOR_KRADEN_STAND_IN, FACING_SOUTH_SOUTHEAST, 0);
	syFieldEventWait(40);
	fdActorFaceDirection(ACTOR_KRADEN_STAND_IN, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(40);
	syEventCameraFollowActor(ACTOR_KRADEN_STAND_IN, 1);
	fdActorWalkToAndWait(ACTOR_KRADEN_STAND_IN, 432, 536);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_KRADEN_STAND_IN, FACING_NORTH_NORTHWEST, 0);
	syFieldEventWait(40);
	fdActorFaceDirection(ACTOR_KRADEN_STAND_IN, FACING_NORTH_NORTHEAST, 0);
	syFieldEventWait(40);
	fdActorFaceDirection(ACTOR_KRADEN_STAND_IN, FACING_NORTH_NORTHWEST, 0);
	syFieldEventWait(40);
	fdActorFaceDirection(ACTOR_KRADEN_STAND_IN, FACING_NORTH_NORTHEAST, 0);
	syFieldEventWait(40);
	fdCameraMoveTo(440, CAMERA_KEEP, 288, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(60);
	fdCameraMoveTo(440, CAMERA_KEEP, 528, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_KRADEN_STAND_IN, 3);
	syFieldEventWait(10);
	syEventCameraFollowActor(ACTOR_KRADEN_STAND_IN, 1);
	fdActorWalkToAndWait(ACTOR_KRADEN_STAND_IN, 456, 568);
	fdCameraMoveTo(464, CAMERA_KEEP, 544, 1);
	fdCameraWaitForArrival();
	fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_WEST_PALACE_GUARD, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_EAST_PALACE_GUARD, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN_STAND_IN, FACING_NORTH_NORTHEAST, 0);
	syFieldEventWait(30);
	/* "It is a magnificent city! Or do you mean the castle? Both are..." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 10);
	fdActorSetSpeed(ACTOR_PIERS, 0.7);
	fdActorWalkByAndWait(ACTOR_PIERS, 8, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	syFieldEventWait(20);
	/* "And you, Felix? Do you agree?" */
	fdEventOpenMessage(ACTOR_PIERS, 0);
	result = syFieldEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0);
	if(result == 0) {
		syFieldEventWait(30);
		fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
		/* "Any newcomer to our town would doubtless feel the same at first." */
		syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
		FieldStateWork->msg++;
	}
	else {
		syFieldEventWait(40);
		fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
		syFieldEventWait(10);
		FieldStateWork->msg++;
		/* "Ah! Yes, you marvel not at its present condition, but at the
		 * greater..." */
		syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	}
	scFieldActorWatchAndSetScript(ACTOR_FELIX, ACTOR_PIERS);
	scFieldActorWatchAndSetScript(ACTOR_KRADEN_STAND_IN, ACTOR_PIERS);
	scFieldActorWatchAndSetScript(ACTOR_WEST_PALACE_GUARD, ACTOR_PIERS);
	scFieldActorWatchAndSetScript(ACTOR_EAST_PALACE_GUARD, ACTOR_PIERS);
	scFieldActorSetSprPriority(ACTOR_PIERS, 2);
	fdActorWalkByAndWait(ACTOR_PIERS, 32, 0);
	fdActorFaceDirection(ACTOR_WEST_PALACE_GUARD, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_EAST_PALACE_GUARD, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	syFieldEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	/* "In ages past, Lemuria was a far lovelier, more grand, and more
	 * lively..." */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_PIERS, 2);
	syFieldEventWait(10);
	/* "His Highness, the king, tells of days gone by, of a more brilliant..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "Over the countless years, our people have lost vigor, enthusiasm..." */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
	syFieldEventWait(20);
	/* "Lemuria as you see it is only a shadow of the city of legend." */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN_STAND_IN, FACING_EAST, 0);
	syFieldEventWait(50);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN_STAND_IN, FACING_EAST, 0);
	syFieldEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_KRADEN_STAND_IN, 3);
	syFieldEventWait(10);
	/* "If only I had the chance to see the city in all its splendor. But
	 * how..." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 10);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "King Hydros believes that our decay is related in some way to Alchemy."
	 */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorShowEmote(ACTOR_KRADEN_STAND_IN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
					 30);
	/* "Alchemy!?" */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 10);
	scFieldActorJump(ACTOR_KRADEN_STAND_IN, 6, 15);
	scFieldActorJump(ACTOR_KRADEN_STAND_IN, 6, 23);
	syFieldEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_PIERS, 2);
	syFieldEventWait(10);
	/* "King Hydros wants to find a way to stop the ruin of Lemuria." */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_WEST_PALACE_GUARD, 2);
	syFieldEventWait(10);
	/* "If there were anything we could do to help stop it, we would." */
	syFieldEventShowMessageThenWait(ACTOR_WEST_PALACE_GUARD, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_EAST_PALACE_GUARD, 2);
	syFieldEventWait(10);
	/* "But the senators are unwilling to take action! They're stubborn, and..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_EAST_PALACE_GUARD, 0, 10);
	fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN_STAND_IN, FACING_EAST, 0);
	syFieldEventWait(50);
	fdActorFaceDirection(ACTOR_EAST_PALACE_GUARD, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN_STAND_IN, FACING_NORTH_NORTHEAST, 0);
	syFieldEventWait(30);
	fdActorShowEmote(ACTOR_KRADEN_STAND_IN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "Senate? What is a senate?" */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	/* "The senate is the group of elders who determine policy in Lemuria." */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_WEST_PALACE_GUARD, 3);
	syFieldEventWait(10);
	/* "Lord Conservato holds the highest office in the senate." */
	syFieldEventShowMessageThenWait(ACTOR_WEST_PALACE_GUARD, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_EAST_PALACE_GUARD, 2);
	syFieldEventWait(10);
	/* "The senate usually opposes any new ideas or proposals introduced..." */
	syFieldEventShowMessageThenWait(ACTOR_EAST_PALACE_GUARD, 0, 10);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 45);
	fdActorWalkByAndWait(ACTOR_PIERS, -16, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
	syFieldEventWait(10);
	/* "Word of our arrival will never reach the king as long as Conservato..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_WEST_PALACE_GUARD, 3);
	syFieldEventWait(10);
	/* "And his conversations with the king are incredibly long." */
	syFieldEventShowMessageThenWait(ACTOR_WEST_PALACE_GUARD, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_EAST_PALACE_GUARD, 4);
	/* "We have no choice but to wait patiently here." */
	syFieldEventShowMessageThenWait(ACTOR_EAST_PALACE_GUARD, 0, 10);
	fdActorWalkByAndWait(ACTOR_PIERS, -16, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	syFieldEventWait(10);
	/* "Why don't we spend our time looking around Lemuria until then..." */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN_STAND_IN, FACING_EAST, 0);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_KRADEN_STAND_IN, 3);
	syFieldEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorSetSpeed(ACTOR_PIERS, 1.2);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_WALK);
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestination(ACTOR_PIERS, leader->x.part[0].integer,
							  leader->z.part[0].integer);
	scFieldActorWaitForMoveById(ACTOR_PIERS);
	fdActorSetPos(ACTOR_PIERS, 0, 0);
	fdActorSetSpeed(ACTOR_KRADEN_STAND_IN, 1.2);
	fdActorSetAnimation(ACTOR_KRADEN_STAND_IN, 2);
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestination(ACTOR_KRADEN_STAND_IN, leader->x.part[0].integer,
							  leader->z.part[0].integer);
	scFieldActorWaitForMoveById(ACTOR_KRADEN_STAND_IN);
	fdActorSetPos(ACTOR_KRADEN_STAND_IN, 0, 0);
	scLemuriaRestoreGirl();
	fdEventEnd();
}

/* The ROM's caller here is compiled against this form, which takes nothing and
 */
/* returns nothing where the definition takes the actor and returns an s32. */
void fdActorTakeLeaderLayer(void);
/* An actor's per-frame hook: it is drawn on the party leader's layer. */
void scLemuriaTakeLeaderLayer(void) { fdActorTakeLeaderLayer(); }

extern u16 Lemuria_WindowOpening[];

/* clang-format off */
/* The spoiled child's walk. */
s32 Lemuria_FirstWalkerPaces[] = {                                                               
      SC_ANIMATION(2),                                                               
      SC_SPEED(1),                                               
      SC_ACCELERATION(1),                                               
      SC_LABEL(1),                                                             
      SC_MOVE(40, 0, 952),                                      
      SC_SYNC,                                                                 
      SC_MOVE(40, 0, 904),                                      
      SC_SYNC,                                                                 
      SC_MOVE(88, 0, 904),                                      
      SC_SYNC,                                                                 
      SC_MOVE(88, 0, 952),                                      
      SC_SYNC,                                                                 
      SC_LOOP(SC_FOREVER, SC_TO(1)),                                           
  };

/* The lonely child's walk. */
s32 Lemuria_SecondWalkerPaces[] = {                                                               
      SC_ANIMATION(2),                                                               
      SC_SPEED(1),                                               
      SC_ACCELERATION(1),                                               
      SC_LABEL(1),                                                             
      SC_MOVE(88, 0, 904),                                      
      SC_SYNC,                                                                 
      SC_MOVE(88, 0, 952),                                      
      SC_SYNC,                                                                 
      SC_MOVE(40, 0, 952),                                      
      SC_SYNC,                                                                 
      SC_MOVE(40, 0, 904),                                      
      SC_SYNC,                                                                 
      SC_LOOP(SC_FOREVER, SC_TO(1)),                                           
  };

SCENE_ENTRANCE SceneEntrances[] = {
    {0, CONDITION_ALWAYS, 460, 0, 550, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 SceneExits[] = {
    SCENE_EXITS(Lemuria),
    SCENE_EXIT_UNLESS(LEMURIA_TO_LEMURIA_ROOMS_WEST_A, LemuriaRooms, LEMURIA_ROOMS_FROM_LEMURIA_WEST_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_TO_LEMURIA_ROOMS_WEST_B, LemuriaRooms, LEMURIA_ROOMS_FROM_LEMURIA_WEST_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_TO_LEMURIA_ROOMS_WEST_C, LemuriaRooms, LEMURIA_ROOMS_FROM_LEMURIA_WEST_C, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_TO_ANCIENT_LEMURIA, AncientLemuria, ANCIENT_LEMURIA_FROM_LEMURIA, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_TO_LEMURIA_SPRING, LemuriaSpring, LEMURIA_SPRING_FROM_LEMURIA, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_TO_LEMURIA_ROOMS_EAST_A, LemuriaRooms, LEMURIA_ROOMS_FROM_LEMURIA_EAST_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_TO_LEMURIA_ROOMS_EAST_B, LemuriaRooms, LEMURIA_ROOMS_FROM_LEMURIA_EAST_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_TO_LEMURIA_ROOMS_EAST_C, LemuriaRooms, LEMURIA_ROOMS_FROM_LEMURIA_EAST_C, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_TO_LEMURIA_PALACE_SOUTH, LemuriaPalace, LEMURIA_PALACE_FROM_LEMURIA_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_TO_LEMURIA_PALACE_NORTHEAST, LemuriaPalace, LEMURIA_PALACE_FROM_LEMURIA_NORTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_TO_LEMURIA_PALACE_NORTH, LemuriaPalace, LEMURIA_PALACE_FROM_LEMURIA_NORTH, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 952, 0, 920, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 920, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 856, 0, 1016, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SIGN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 880, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_GIRL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 1256, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 1040, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 1064, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_IN_TEAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 856, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 920, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_WITH_STAFF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 920, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_BOY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 40, 0, 952, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 904, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CAT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 80, 0, 1104, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 392, 0, 712, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_COW, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 840, 0, 1080, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHICKEN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 864, 0, 984, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 520, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 520, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_WITH_STAFF, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 552, 0, 440, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_GIRL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 1008, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 464, 0, 1016, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 488, 0, 984, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/* How a door opens, as two frames of cells copied over the closed door. */
u16 Lemuria_DoorOpening[] = {
    MAPGRID_ANIMATION_STEP(2, 1, 1, 1, 6),
    MAPGRID_ANIMATION_STEP(2, 2, 1, 1, 6),
    MAPGRID_ANIMATION_STEPS_END,
};

/* The same for the Senate's and the palace's doors, two cells wide. */
u16 Lemuria_WideDoorOpening[] = {
    MAPGRID_ANIMATION_STEP(0, 1, 2, 1, 6),
    MAPGRID_ANIMATION_STEP(0, 2, 2, 1, 6),
    MAPGRID_ANIMATION_STEPS_END,
};

/* The window on Lunpa's balcony opens in one frame. */
u16 Lemuria_WindowOpening[] = {
    MAPGRID_ANIMATION_STEP(3, 1, 1, 1, 0),
    MAPGRID_ANIMATION_STEPS_END,
};
/* clang-format on */

/* The opening of the door of Piers's uncle's house, at its place on the map */
/* grid. */
MAP_GRID_CELL_ANIMATION Lemuria_UnclesDoor = {Lemuria_DoorOpening, 22, 47};

/* The opening of the door of the girl's house, at its place on the map grid. */
MAP_GRID_CELL_ANIMATION Lemuria_GirlsHouseDoor = {Lemuria_DoorOpening, 35, 47};

/* The Senate's door's opening, at its place on the map grid. */
MAP_GRID_CELL_ANIMATION Lemuria_SenateDoor = {Lemuria_WideDoorOpening, 9, 40};

/* The opening of the window on the middle balcony of Lunpa's tower, which */
/* Lunpa opened for the party, at its place on the map grid. */
MAP_GRID_CELL_ANIMATION Lemuria_LunpasWindow = {Lemuria_WindowOpening, 54, 45};

/* The palace front door's opening, at its place on the map grid. */
MAP_GRID_CELL_ANIMATION Lemuria_PalaceFrontDoor = {Lemuria_WideDoorOpening, 28,
												   20};

/* The palace upper door's opening, at its place on the map grid. */
MAP_GRID_CELL_ANIMATION Lemuria_PalaceUpperDoor = {Lemuria_WideDoorOpening, 28,
												   13};

/* The opening of the front door of Lunpa's tower, at its place on the map */
/* grid. */
MAP_GRID_CELL_ANIMATION Lemuria_LunpasDoor = {Lemuria_DoorOpening, 54, 50};

TRIGGER SceneEvents[] = {
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), LEMURIA_TO_LEMURIA_ROOMS_WEST_A,
	 CONDITION_ALWAYS, EVENT(scLemuriaEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), LEMURIA_TO_LEMURIA_ROOMS_WEST_B,
	 CONDITION_ALWAYS, EVENT(scLemuriaEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), LEMURIA_TO_LEMURIA_ROOMS_WEST_C,
	 CONDITION_ALWAYS, EVENT(scLemuriaEnterDoor)},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, LEMURIA_TO_ANCIENT_LEMURIA,
	 CONDITION_ALWAYS, LEMURIA_TO_ANCIENT_LEMURIA},
	{TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH,
	 LEMURIA_TO_LEMURIA_SPRING, IF_FLAG_SET(FLAG_LEMURIA_SPRING_LUCK_TRIED),
	 LEMURIA_TO_LEMURIA_SPRING},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), LEMURIA_TO_LEMURIA_ROOMS_EAST_A,
	 IF_FLAG_SET(FLAG_PIERS_AT_MOTHERS_GRAVE), EVENT(scLemuriaEnterDoor)},
	{TRIGGER_EXIT, LEMURIA_TO_LEMURIA_ROOMS_EAST_B, CONDITION_ALWAYS,
	 LEMURIA_TO_LEMURIA_ROOMS_EAST_B},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), LEMURIA_TO_LEMURIA_ROOMS_EAST_C,
	 IF_FLAG_SET(FLAG_MET_LUNPA), EVENT(scLemuriaEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), LEMURIA_TO_LEMURIA_PALACE_SOUTH,
	 CONDITION_ALWAYS, EVENT(scLemuriaEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 LEMURIA_TO_LEMURIA_PALACE_NORTHEAST, CONDITION_ALWAYS,
	 EVENT(scLemuriaEnterDoor)},
	{TRIGGER_EXIT, LEMURIA_TO_LEMURIA_PALACE_NORTH, CONDITION_ALWAYS,
	 LEMURIA_TO_LEMURIA_PALACE_NORTH},
	{TRIGGER_STEP, CELL_BEHAVIOR_LEMURIA_BEFORE_PALACE,
	 FLAG_LEMURIA_CITY_ADMIRED, EVENT(scLemuriaMarvelAtCity)},
	{TRIGGER_STEP, CELL_BEHAVIOR_LEMURIA_WEST_PATH, FLAG_LEMURIA_CITY_ADMIRED,
	 EVENT(scLemuriaAdmirePalace)},
	{TRIGGER_STEP, CELL_BEHAVIOR_LEMURIA_EAST_PATH, FLAG_LEMURIA_CITY_ADMIRED,
	 EVENT(scLemuriaAdmirePalace)},
	{TRIGGER_STEP, CELL_BEHAVIOR_LEMURIA_COLONNADE_STAIRS,
	 FLAG_LEMURIA_CITY_ADMIRED, EVENT(scLemuriaKeepLookingAround)},
	{TRIGGER_TALK, ACTOR_KRADEN_STAND_IN, CONDITION_ALWAYS,
	 MSGNO(MsgLemuriaICannotRememberLastTime)},
	{TRIGGER_TALK, ACTOR_WOMAN_ON_PARADISE, CONDITION_ALWAYS,
	 MSGNO(MsgLemuriaHasNo)},
	{TRIGGER_TALK, ACTOR_MAN_ON_LEMURIAN_WAYS, CONDITION_ALWAYS,
	 MSGNO(MsgLemuriaInLemuriaWeDoNot)},
	{TRIGGER_TALK, ACTOR_LEMURIAN_ON_UNCHANGING_LIFE, CONDITION_ALWAYS,
	 MSGNO(MsgLemuriaLifeNeverChangesHere)},
	{TRIGGER_TALK, ACTOR_OLD_WOMAN, CONDITION_ALWAYS,
	 MSGNO(MsgLemuriaIOughtToFeelSurprised)},
	{TRIGGER_TALK, ACTOR_GIRL_FORGETTING_HER_AGE, CONDITION_ALWAYS,
	 MSGNO(MsgLemuriaHowOldAmIAgain)},
	{TRIGGER_TALK, ACTOR_SPOILED_CHILD, CONDITION_ALWAYS,
	 EVENT(scLemuriaTalkToSpoiledChild)},
	{TRIGGER_TALK, ACTOR_LONELY_CHILD, CONDITION_ALWAYS,
	 EVENT(scLemuriaTalkToLonelyChild)},
	{TRIGGER_TALK, ACTOR_CAT, CONDITION_ALWAYS, MSGNO(MsgLemuriaMeowMrrrfle)},
	{TRIGGER_TALK, ACTOR_DOG, CONDITION_ALWAYS, MSGNO(MsgLemuriaWoofRrrrrroof)},
	{TRIGGER_TALK, ACTOR_COW, CONDITION_ALWAYS, MSGNO(MsgLemuriaMoo)},
	{TRIGGER_TALK, ACTOR_CHICKEN, CONDITION_ALWAYS, MSGNO(MsgLemuriaBukbukbuk)},
	{TRIGGER_TALK, ACTOR_PIERS, CONDITION_ALWAYS, MSGNO(MsgLemuriaSilence)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KRADEN_STAND_IN,
	 CONDITION_ALWAYS, MSGNO(MsgLemuriaMoreOfSameToday)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_PARADISE,
	 CONDITION_ALWAYS, MSGNO(MsgLemuriaIHaventBeenSurprised)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_LEMURIAN_WAYS,
	 CONDITION_ALWAYS, MSGNO(MsgLemuriaIveAgedSoSlowly)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LEMURIAN_ON_UNCHANGING_LIFE,
	 CONDITION_ALWAYS, MSGNO(MsgLemuriaSometimesIThinkBuildingsFall)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN, CONDITION_ALWAYS,
	 MSGNO(MsgLemuriaLongAgoManNamedLunpa)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_FORGETTING_HER_AGE,
	 CONDITION_ALWAYS, MSGNO(MsgLemuriaKingHydrosIsFarOlder)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SPOILED_CHILD,
	 CONDITION_ALWAYS, EVENT(scLemuriaTalkToSpoiledChild)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LONELY_CHILD, CONDITION_ALWAYS,
	 EVENT(scLemuriaTalkToLonelyChild)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CAT, CONDITION_ALWAYS,
	 MSGNO(MsgLemuriaNoOneInLemuriaFeels)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DOG, CONDITION_ALWAYS,
	 MSGNO(MsgLemuriaTheresSoMuchTreasureBuried)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COW, CONDITION_ALWAYS,
	 MSGNO(MsgLemuriansAreSo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHICKEN, CONDITION_ALWAYS,
	 MSGNO(MsgLemuriaIfItWerentForPeople)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PIERS, CONDITION_ALWAYS,
	 MSGNO(MsgLemuriaMotherWhyHaveYouLeft)},
	{TRIGGER_TALK, ACTOR_WEST_PALACE_GUARD, IF_FLAG_SET(FLAG_MET_KING_HYDROS),
	 MSGNO(MsgLemuriaIveNeverSeenLordConservato)},
	{TRIGGER_TALK, ACTOR_EAST_PALACE_GUARD, IF_FLAG_SET(FLAG_MET_KING_HYDROS),
	 MSGNO(MsgLemuriaLordConservatoHasOrderedUs)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_PALACE_GUARD,
	 IF_FLAG_SET(FLAG_MET_KING_HYDROS),
	 MSGNO(MsgLemuriaImSureSenateHasSomething)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_PALACE_GUARD,
	 IF_FLAG_SET(FLAG_MET_KING_HYDROS),
	 MSGNO(MsgLemuriaHowCouldLordConservatoExpect)},
	{TRIGGER_TALK, ACTOR_WEST_PALACE_GUARD, IF_FLAG_SET(FLAG_MET_LUNPA),
	 MSGNO(MsgLemuriaKingHydrosAwaits)},
	{TRIGGER_TALK, ACTOR_EAST_PALACE_GUARD, IF_FLAG_SET(FLAG_MET_LUNPA),
	 MSGNO(MsgLemuriaHisHighnessKingHasInstructed)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_PALACE_GUARD,
	 IF_FLAG_SET(FLAG_MET_LUNPA), MSGNO(MsgLemuriaTheyAreClearlyUsingPsynergy)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_PALACE_GUARD,
	 IF_FLAG_SET(FLAG_MET_LUNPA), MSGNO(MsgLemuriaWhatDoesHisHighnessHave)},
	{TRIGGER_TALK, ACTOR_WEST_PALACE_GUARD, CONDITION_ALWAYS,
	 MSGNO(MsgLemuriaKingIsHoldingCounsel)},
	{TRIGGER_TALK, ACTOR_EAST_PALACE_GUARD, CONDITION_ALWAYS,
	 MSGNO(MsgLemuriaKingWillBeThrilled)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_PALACE_GUARD,
	 CONDITION_ALWAYS, MSGNO(MsgLemuriaHowMuchLongerWillConservato)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_PALACE_GUARD,
	 CONDITION_ALWAYS, MSGNO(MsgLemuriaIHadHeardPiers)},
	{TRIGGER_TALK, ACTOR_GIRL_ON_LUNPA, IF_FLAG_SET(FLAG_MET_KING_HYDROS),
	 MSGNO(MsgLemuriaIfYouAndLunpa)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ON_LUNPA,
	 IF_FLAG_SET(FLAG_MET_KING_HYDROS), MSGNO(MsgLemuriaIKnowWhyLemuriaEnded)},
	{TRIGGER_TALK, ACTOR_GIRL_ON_LUNPA, CONDITION_ALWAYS,
	 MSGNO(MsgLemuriaSenateHoldsKingHydros)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ON_LUNPA,
	 CONDITION_ALWAYS, MSGNO(MsgLemuriaITrustKingIfHis)},
	{TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_SIGN, CONDITION_ALWAYS,
	 MSGNO(MsgLemuriaHouseOfSenate)},
	{TRIGGER_TALK, ACTOR_GIRL_BY_THE_SPRING, CONDITION_ALWAYS,
	 MSGNO(MsgLemuriaDrinkingFromThisSpringRestores)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_BY_THE_SPRING,
	 CONDITION_ALWAYS, MSGNO(MsgLemuriaTheseTurtlesHaveLived)},
	{TRIGGER_TALK, ACTOR_MAN_AT_THE_SPRING, CONDITION_ALWAYS,
	 EVENT(scLemuriaOfferSpringLuck)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_AT_THE_SPRING,
	 CONDITION_ALWAYS, MSGNO(MsgLemuriaNoOneIsLuckierThan)},
	{TRIGGER_TALK, ACTOR_SPRING, CONDITION_ALWAYS,
	 EVENT(scLemuriaDrinkFromSpring)},
	{TRIGGER_EXAMINE, CELL_BEHAVIOR_LEMURIA_SPRING, CONDITION_ALWAYS,
	 EVENT(scLemuriaDrinkFromSpring)},
	{TRIGGER_EXAMINE, CELL_BEHAVIOR_LEMURIA_STUCK_DOOR, CONDITION_ALWAYS,
	 EVENT(scLemuriaTryStuckDoor)},
	{PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_ROPE_LOW_POST,
	 FLAG_LEMURIA_ROPE_TIED, 0},
	{PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_SPROUT, FLAG_LEMURIA_VINE_GROWN,
	 EVENT(scLemuriaGrowVine)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_LEMURIA_WEST_WEEDS, CONDITION_ALWAYS,
	 EVENT(scLemuriaCycloneWestWeeds)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_LEMURIA_PASTURE_WEEDS, CONDITION_ALWAYS,
	 EVENT(scLemuriaCyclonePastureWeeds)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_LEMURIA_WEEDS_SOUTH_OF_COW, CONDITION_ALWAYS,
	 EVENT(scLemuriaCycloneWeedsSouthOfCow)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_LEMURIA_WEEDS_NORTH_OF_COW, CONDITION_ALWAYS,
	 EVENT(scLemuriaCycloneWeedsNorthOfCow)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_LEMURIA_HARD_NUT_WEEDS, CONDITION_ALWAYS,
	 EVENT(scLemuriaCycloneHardNutWeeds)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_LEMURIA_LUCKY_MEDAL, FLAG_LUCKY_MEDAL_DUG_UP,
	 EVENT(scLemuriaScoopLuckyMedal)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_LEMURIA_RUSTY_SWORD, FLAG_RUSTY_SWORD_DUG_UP,
	 EVENT(scLemuriaScoopRustySword)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_LEMURIA_BONE, FLAG_BONE_DUG_UP, EVENT(scLemuriaScoopBone)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_LEMURIA_STAR_DUST, FLAG_STAR_DUST_DUG_UP,
	 EVENT(scLemuriaScoopStarDust)},
	{TRIGGER_TABLE_END},
};
