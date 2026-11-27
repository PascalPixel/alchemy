/* @file */
/* The Madra Catacombs, a deserted village in a cave under the town. Madra's */
/* two ladders, the one inside the walls and the one in the Champa's hole, */
/* come down into a small ladder room, and the tunnel the Champa dug runs from
 */
/* there to the village. On the first visit Felix comes in along the tunnel's */
/* upper path, Moves a pillar off a ledge into a pit to cross it, and takes the
 */
/* Tremor Bit from the village's broken building. Once Briggs is caught that */
/* way is blocked off, and the village is reached from the south, through a */
/* golden door whose push button only Reveal shows. Inside, Frost freezes a */
/* puddle, Move shifts two pillars to climb by, Lash ties a rope across, and */
/* Tremor rocks a bookshelf on the building's upper floor until the chest on */
/* top falls through to the floor below; it holds the Ruin Key, which opens the
 */
/* red double door down the stairs, lighting the braziers before Moloch's */
/* tablet. */

#include "item_ids.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "field/actor_script.h"
#include "game/game.h"
#include "text/msg_ids.h"

/* The ropes Lash ties, the tunnel's and the village's, which stay tied until */
/* the party leaves. */
#define FLAG_TUNNEL_ROPE_TIED  FLAG_SCENE_TEMP_00
#define FLAG_VILLAGE_ROPE_TIED FLAG_SCENE_TEMP_01
/* Frost on the village's puddle, which stays a pillar of ice until the party */
/* leaves. */
#define FLAG_VILLAGE_PUDDLE_FROZEN FLAG_SCENE_TEMP_02
/* Set while the bookshelf leans as Tremor shakes it, so that the chest on it */
/* is nudged once each time it comes upright. */
#define FLAG_BOOKSHELF_LEANING FLAG_SCENE_TEMP_04

/* Set once the pillar Move pushed onto cell (40, 13) has dropped into the pit
 */
/* in the village's northern ledge and filled it. */
#define FLAG_MADRA_CATACOMBS_HOLE_FILLED 0x8c8
/* Set once the Ruin Key has opened the red double door before Moloch's */
/* tablet; its braziers burn from then on. */
#define FLAG_MADRA_CATACOMBS_TABLET_DOOR_OPEN 0x8c9
/* Set once Move has left the village's plain stone pillar on cell (44, 26). */
#define FLAG_MADRA_CATACOMBS_PILLAR_PLACED 0x8ca
/* Set once the push button Reveal shows has opened the village's golden door.
 */
#define FLAG_MADRA_CATACOMBS_VILLAGE_DOOR_OPEN 0x8cb
/* Set once Tremor has shaken the chest off the bookshelf into the room below.
 */
#define FLAG_MADRA_CATACOMBS_CHEST_FELL 0x8cc
/* Set once Move has left the vine-covered pillar on cell (35, 17). */
#define FLAG_MADRA_CATACOMBS_VINE_PILLAR_PLACED 0x9ef
/* Set while Reveal shows the push button by the closed golden door. */
#define FLAG_MADRA_CATACOMBS_BUTTON_REVEALED FLAG_SCENE_TEMP_03

/* The deserted village's actors that its events move. */
typedef enum MADRA_CATACOMBS_VILLAGE_ACTOR_ID {
	/* The vine-covered pillar Move carries onto the upper level to climb by. */
	ACTOR_VINE_PILLAR = ACTOR_FIRST_PLACED,
	/* The posts at the two ends of the rope Lash makes taut. */
	ACTOR_VILLAGE_ROPE_LOW_POST,
	ACTOR_VILLAGE_ROPE_HIGH_POST,
	/* The pillar on the northern ledge that Move pushes into the pit. */
	ACTOR_HOLE_PILLAR = ACTOR_FIRST_PLACED + 3,
	/* The plain stone pillar Move pushes onto cell (44, 26). */
	ACTOR_PUSHED_PILLAR,
	/* The puddle Frost freezes into a pillar. */
	ACTOR_VILLAGE_PUDDLE,
	/* The chest by the rope, the map's first treasure spot, drawn over the */
	/* ledge. */
	ACTOR_VILLAGE_CHEST = FIELD_TREASURE_ACTOR
} MADRA_CATACOMBS_VILLAGE_ACTOR_ID;

/* The tunnel the Champa dug (MadraCatacombsChampaTunnel), by the rows of its */
/* table. */
typedef enum MADRA_CATACOMBS_TUNNEL_ACTOR_ID {
	/* The posts at the two ends of the tunnel's rope. */
	ACTOR_TUNNEL_ROPE_EAST_POST = ACTOR_FIRST_PLACED,
	ACTOR_TUNNEL_ROPE_WEST_POST,
	/* A man in the tunnel until Briggs is caught: "I can't believe the Champa
	 */
	/* just stumbled onto this cave accidentally..." */
	ACTOR_MAN_ON_THE_CHAMPAS_CAVE
} MADRA_CATACOMBS_TUNNEL_ACTOR_ID;

/* The far end of the Champa tunnel's upper path, cell (51, 13): the number of
 */
/* both the tunnel's entrance there and its exit back the way the party came. */
enum { CHAMPA_TUNNEL_UPPER_PATH_END = 8 };

/* The cells whose triggers run the catacombs' events. */
typedef enum MADRA_CATACOMBS_CELL_BEHAVIOR {
	/* The red double door before Moloch's tablet, cells (9, 22) and (10, 22).
	 */
	CELL_BEHAVIOR_MADRA_CATACOMBS_TABLET_DOOR = 10,
	/* Cell (33, 17), west of the vine-covered pillar's place. */
	CELL_BEHAVIOR_MADRA_CATACOMBS_VINE_PILLAR_PUSH = 11,
	/* The village's wall east of the golden door, cell (36, 39), where Reveal
	 */
	/* shows the push button; the village's only cell of this behavior. */
	CELL_BEHAVIOR_MADRA_CATACOMBS_BUTTON_WALL = 10
} MADRA_CATACOMBS_CELL_BEHAVIOR;

/* The braziers that flare up down each side of the hall before Moloch's */
/* tablet. */
#define FLARES_PER_SIDE 5

/* The tablet hall's actors: the two halves of the red double door, the */
/* brazier flames that flare up down each side as it opens, numbered from the */
/* tablet down to the door, and an unseen marker that stands on each flame once
 */
/* they burn. */
typedef enum MADRA_CATACOMBS_TABLET_ROOM_ACTOR_ID {
	ACTOR_LEFT_DOOR = ACTOR_FIRST_PLACED,
	ACTOR_RIGHT_DOOR,
	ACTOR_FIRST_WEST_FLARE,
	ACTOR_FIRST_EAST_FLARE = ACTOR_FIRST_WEST_FLARE + FLARES_PER_SIDE,
	ACTOR_FIRST_FLAME_MARKER = ACTOR_FIRST_EAST_FLARE + FLARES_PER_SIDE
} MADRA_CATACOMBS_TABLET_ROOM_ACTOR_ID;

/* The bookshelf on the broken building's upper floor, and the chest on top of
 */
/* it that Tremor shakes off; the room below places the fallen chest. */
typedef enum MADRA_CATACOMBS_BOOKSHELF_ACTOR_ID {
	ACTOR_BOOKSHELF = ACTOR_FIRST_PLACED,
	ACTOR_SHELF_CHEST = FIELD_TREASURE_ACTOR,
	ACTOR_FALLEN_CHEST
} MADRA_CATACOMBS_BOOKSHELF_ACTOR_ID;

struct ACTOR;

extern SCENE_ENTRANCE SceneEntrances[];
extern SCENE_ENTRANCE MadraCatacombs_StoneDoorEntrances[];

extern SCENE_ENTRANCE MadraCatacombs_BookshelfEntrances[];

extern SCENE_REGION MadraCatacombs_LadderRoomRegions[];

extern ACTOR_PLACE MadraCatacombs_Placements[];

extern ACTOR_PLACE MadraCatacombs_EntrancePlacements[];

extern ACTOR_PLACE MadraCatacombs_ChampaTunnelPlacements[];

extern ACTOR_PLACE MadraCatacombs_BookshelfPlacements[];
extern ACTOR_PLACE MadraCatacombs_TabletPlacements[];
extern TRIGGER MadraCatacombs_EntranceTriggers[];
extern TRIGGER MadraCatacombs_LaddersTriggers[];
extern TRIGGER MadraCatacombs_ChampaTunnelTriggers[];
extern TRIGGER MadraCatacombs_StoneDoorTriggers[];
extern TRIGGER MadraCatacombs_BookshelfTriggers[];
extern TRIGGER MadraCatacombs_TabletTriggers[];

void scMadraCatacombsSetUpVillage(void);

void scMadraCatacombsSetUpChampaTunnel(void);

void scMadraCatacombsSetUpBookshelfRoom(void);

void scMadraCatacombsSetUpTabletRoom(void);

extern s16 MadraCatacombs_PushedPillar[];
extern s16 MadraCatacombs_Puddles[];

extern s32 MadraCatacombs_FlameFlaresUp[];

extern s32 MadraCatacombs_GreatFlameFlaresUp[];
extern s16 MadraCatacombs_VinePillar[];
extern s32 Leader_WalkThroughDoorScript[];

void scMadraCatacombsRockChestWithBookshelf(void);

/* Arriving in the village from the Champa tunnel's west end once that way is */
/* blocked off: Felix vanishes and the screen fades back to the tunnel. */
void scMadraCatacombsLeaveBlockedVillage(void);

#include "shared/field_rope_place.c"

/* Raises each listed actor's cells: the shape grid and the cell flags under it,
 */
/* then lifts the leader if the new terrain is above the leader. */
#include "shared/pillar_raise.h"

#include "shared/pillar_lower_held_cells.h"

#include "shared/held_actor_raise_cell.h"

extern TRIGGER MadraCatacombs_Triggers[];

void scMadraCatacombsSetUpStoneDoorRoom(void);

const SCENE_ENTRANCE *scGetEntrances(void) {
	if(MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_STONE_DOOR)
		return MadraCatacombs_StoneDoorEntrances;
	if(MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_BOOKSHELF)
		return MadraCatacombs_BookshelfEntrances;
	return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void) {
	s16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_MADRA_CATACOMBS_LADDERS)
		return MadraCatacombs_LadderRoomRegions;
	return 0;
}

const u32 *scGetExits(void) { return SceneExits; }

const ACTOR_PLACE *scGetActorPlacements(void) {
	if(MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_ENTRANCE)
		return MadraCatacombs_EntrancePlacements;
	if(MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_CHAMPA_TUNNEL)
		return MadraCatacombs_ChampaTunnelPlacements;
	if(MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_BOOKSHELF)
		return MadraCatacombs_BookshelfPlacements;
	if(MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_TABLET)
		return MadraCatacombs_TabletPlacements;
	return MadraCatacombs_Placements;
}

const TRIGGER *scGetTriggers(void) {
	if(MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_ENTRANCE)
		return MadraCatacombs_EntranceTriggers;
	if(MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_LADDERS)
		return MadraCatacombs_LaddersTriggers;
	if(MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_CHAMPA_TUNNEL)
		return MadraCatacombs_ChampaTunnelTriggers;
	if(MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_STONE_DOOR)
		return MadraCatacombs_StoneDoorTriggers;
	if(MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_BOOKSHELF)
		return MadraCatacombs_BookshelfTriggers;
	if(MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_TABLET)
		return MadraCatacombs_TabletTriggers;
	return MadraCatacombs_Triggers;
}

/* Examining the red double door at the foot of the hall before Moloch's tablet
 */
/* (cells (9, 22) and (10, 22)) before the Ruin Key has opened it: "The door */
/* won't budge. ...Must be locked." */
void scMadraCatacombsTryLockedDoor(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	/* "The door won't budge. ...Must be locked." */
	mnFieldMessageShow((s32)&MsgFieldDoorWontBudge, 1);
	state->fieldMenu = 1;
	fdEventEnd();
}

#include "shared/actor_drop_and_wait.h"

/* Felix Moves the pillar on the village's northern ledge one cell east, off */
/* the edge (onto cell (40, 13)): it drops into the pit there and fills it, */
/* its top level with the ledge, so the party can walk across. It stays there */
/* for good. A push that leaves it anywhere else does nothing. */
void scMadraCatacombsDropPillarIntoHole(void) {
	ACTOR *actor = fdActorGetById(ACTOR_HOLE_PILLAR);
	s32 z;

	if(FIELD_CELL(actor->x.fixed) == 40) {
		z = FIELD_CELL(actor->z.fixed);
		if(z == 13) {
			fdEventStart();
			fdEventEndLastingPsynergy(0);
			fdActorDropAndWait(actor);
			fdMapGridCopyCellAttributes(41, 13, 2, 1, 39, z);
			fdEventSetFlag(FLAG_MADRA_CATACOMBS_HOLE_FILLED);
			fdEventEnd();
		}
	}
}

/* Nothing visible happens as Move takes hold of the plain stone pillar east of
 */
/* the village's middle; the game frees the ground it stood on, so that */
/* it can slide. */
void scMadraCatacombsStartMovingPushedPillar(void) {
	fdActorLiftTargetBlock(MadraCatacombs_PushedPillar);
}

/* Felix Moves the plain stone pillar east of the village's middle; once it is
 */
/* one cell east, on cell (44, 26), it stays there on later visits, a stepping
 */
/* stone to jump across from the upper level. */
void scMadraCatacombsFinishMovingPushedPillar(void) {
	ACTOR *actor = fdActorGetById(ACTOR_PUSHED_PILLAR);

	fdActorPlaceMovedBlock();
	if(FIELD_CELL(actor->x.fixed) == 44 && FIELD_CELL(actor->z.fixed) == 26)
		fdEventSetFlag(FLAG_MADRA_CATACOMBS_PILLAR_PLACED);
}

/* Felix casts Frost on the puddle just east of the broken building's south */
/* doorway: it freezes into a pillar of ice Felix can jump onto, until the */
/* party leaves the catacombs. */
void scMadraCatacombsFreezePuddle(void) {
	fdFlagBlockApplyFlags(MadraCatacombs_Puddles);
}

void scMadraCatacombsLightFlames(void);

/* Felix uses the Ruin Key at the red double door down the stairs (cells (9, */
/* 22) and (10, 22)): the two halves rumble apart, the camera climbs the hall */
/* beyond to Moloch's tablet, and the braziers down both sides flare up in */
/* pairs from the door upwards, the two great pairs by the tablet last. The */
/* key is used up. */
void scMadraCatacombsUnlockWithRuinKey(void) {
	ACTOR *left = fdActorGetById(ACTOR_LEFT_DOOR);
	ACTOR *right = fdActorGetById(ACTOR_RIGHT_DOOR);
	u32 i;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventPlaySnd(SE_STONE_RUMBLE);
	for(i = 0; i < 120; i++) {
		left->x.fixed += -(toFIXED(1) / 10);
		right->x.fixed += toFIXED(1) / 10;
		sySynch(1);
	}
	fdCameraSetSpeed(3.2);
	fdCameraMoveTo(160, CAMERA_KEEP, 240, 1);
	fdCameraWaitForArrival();
	fdCameraSetSpeed(0.4);
	fdCameraMoveTo(160, CAMERA_KEEP, 186, 1);
	fdEventPlaySnd(SE_UNLOCK);
	fdActorSetPos(ACTOR_FIRST_WEST_FLARE + 4, 136, 258);
	fdActorSetPos(ACTOR_FIRST_EAST_FLARE + 4, 184, 258);
	fdActorSetScript(ACTOR_FIRST_WEST_FLARE + 4, MadraCatacombs_FlameFlaresUp);
	fdActorSetScript(ACTOR_FIRST_EAST_FLARE + 4, MadraCatacombs_FlameFlaresUp);
	fdEventWait(40);
	fdEventPlaySnd(SE_UNLOCK);
	fdActorSetPos(ACTOR_FIRST_WEST_FLARE + 3, 120, 226);
	fdActorSetPos(ACTOR_FIRST_EAST_FLARE + 3, 200, 226);
	fdActorSetScript(ACTOR_FIRST_WEST_FLARE + 3, MadraCatacombs_FlameFlaresUp);
	fdActorSetScript(ACTOR_FIRST_EAST_FLARE + 3, MadraCatacombs_FlameFlaresUp);
	fdEventWait(40);
	fdEventPlaySnd(SE_UNLOCK);
	fdActorSetPos(ACTOR_FIRST_WEST_FLARE + 2, 120, 194);
	fdActorSetPos(ACTOR_FIRST_EAST_FLARE + 2, 200, 194);
	fdActorSetScript(ACTOR_FIRST_WEST_FLARE + 2, MadraCatacombs_FlameFlaresUp);
	fdActorSetScript(ACTOR_FIRST_EAST_FLARE + 2, MadraCatacombs_FlameFlaresUp);
	fdEventWait(60);
	fdEventPlaySnd(SE_SPLASH);
	fdActorSetPos(ACTOR_FIRST_WEST_FLARE + 1, 136, 162);
	fdActorSetPos(ACTOR_FIRST_EAST_FLARE + 1, 184, 162);
	fdActorSetPos(ACTOR_FIRST_WEST_FLARE, 136, 138);
	fdActorSetPos(ACTOR_FIRST_EAST_FLARE, 184, 138);
	fdActorSetScript(ACTOR_FIRST_WEST_FLARE + 1,
					 MadraCatacombs_GreatFlameFlaresUp);
	fdActorSetScript(ACTOR_FIRST_EAST_FLARE + 1,
					 MadraCatacombs_GreatFlameFlaresUp);
	fdActorSetScript(ACTOR_FIRST_WEST_FLARE, MadraCatacombs_GreatFlameFlaresUp);
	fdActorSetScript(ACTOR_FIRST_EAST_FLARE, MadraCatacombs_GreatFlameFlaresUp);
	fdEventWait(80);
	scMadraCatacombsLightFlames();
	gmPartyInventoryRemove(ITEM_RUIN_KEY);
	fdEventSetFlag(FLAG_MADRA_CATACOMBS_TABLET_DOOR_OPEN);
	fdEventEnd();
}

/* The vine-covered pillar becomes ground to stand on where Move left it; on */
/* cell (35, 17) it stays there on later visits. */
void scMadraCatacombsSettleVinePillar(ACTOR *actor);
/* Coming up into the far end of the Champa tunnel's upper path (cell (51, */
/* 13)) once that way is blocked off: Felix vanishes and the screen fades back
 */
/* the way he came. */
void scMadraCatacombsLeaveBlockedTunnel(void);

/* Casting Reveal in the village shows a push button in the wall just east of */
/* the golden door on the south side (cell (36, 38)), pressed in once the door
 */
/* has opened. */
void scMadraCatacombsRevealButton(void) {
	if(fdEventGetFlag(FLAG_MADRA_CATACOMBS_VILLAGE_DOOR_OPEN) != 0)
		fdMapGridCopyCellsTo(72, 63, 36, 38, 1, 2);
	else
		fdMapGridCopyCellsTo(71, 63, 36, 38, 1, 2);
	fdEventSetFlag(FLAG_MADRA_CATACOMBS_BUTTON_REVEALED);
}

/* As Reveal wears off, the push button by the golden door disappears. */
void scMadraCatacombsHideButton(void) {
	fdMapGridCopyCellsTo(70, 63, 36, 38, 1, 2);
	fdEventClearFlag(FLAG_MADRA_CATACOMBS_BUTTON_REVEALED);
}

/* Examining the wall just east of the golden door on the village's south side
 */
/* (cell (36, 39)) reads "Look upon me with eyes of truth... Then the door will
 */
/* open..." While Reveal shows the push button there, Felix presses it instead:
 */
/* it rattles in, and the golden door slides open with a thump and stays open.
 */
void scMadraCatacombsHearEyesOfTruth(void) {
	if(fdEventGetFlag(FLAG_MADRA_CATACOMBS_BUTTON_REVEALED) != 0 &&
	   fdEventGetFlag(FLAG_MADRA_CATACOMBS_VILLAGE_DOOR_OPEN) == 0) {
		fdEventStart();
		fdEventEndLastingPsynergy(1);
		fdEventPlaySnd(SE_RATTLE);
		fdMapGridCopyCellsTo(72, 63, 36, 38, 1, 2);
		fdEventWait(40);
		fdEventPlaySnd(SE_THUMP);
		fdMapGridCopyCellsTo(71, 66, 97, 39, 2, 2);
		sySynch(2);
		fdMapGridCopyCellsTo(73, 66, 97, 39, 2, 2);
		sySynch(2);
		fdMapGridCopyCellsTo(75, 66, 97, 39, 2, 2);
		fdMapGridCopyCellAttributes(33, 37, 2, 2, 33, 39);
		sySynch(1);
		fdEventSetFlag(FLAG_MADRA_CATACOMBS_VILLAGE_DOOR_OPEN);
		fdEventEnd();
	}
	else {
		fdEventStart();
		fdEventEndLastingPsynergy(0);
		/* "Look upon me with eyes of truth... Then the door will open..." */
		mnFieldMessageShow((s32)&MsgFieldEyesOfTruth, 1);
		fdEventEnd();
	}
}

/* Felix casts Tremor on the bookshelf in the bedroom behind the west door of */
/* the small hall on the broken building's upper floor: it rocks, and the red */
/* chest on top creeps west each time it rights itself. Once the chest is far */
/* enough over the edge it falls with a puff of dust, through the floor to the
 */
/* room below, where it waits with the Ruin Key. */
void scMadraCatacombsShakeBookshelf(void) {
	ACTOR *actor;
	FIXED x;

	if(fdEventGetFlag(FLAG_MADRA_CATACOMBS_CHEST_FELL) == 0) {
		actor = fdActorGetById(ACTOR_SHELF_CHEST);
		x = actor->x.fixed;
		if(x < (toFIXED(186))) {
			fdEventStart();
			fdEventEndLastingPsynergy(0);
			fdEventPlaySnd(SE_DUST_PUFF);
			actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
			sySynch(10);
			fdActorSetPos(ACTOR_SHELF_CHEST, 0, 0);
			fdEventSetFlag(FLAG_MADRA_CATACOMBS_CHEST_FELL);
			fdEventEnd();
		}
	}
}

void scMadraCatacombsSettleVinePillar(ACTOR *actor) {
	syHeldActorRaiseCell();
	if(FIELD_CELL(actor->x.fixed) == 35 && FIELD_CELL(actor->z.fixed) == 17)
		fdEventSetFlag(FLAG_MADRA_CATACOMBS_VINE_PILLAR_PLACED);
}

/* Walking east into the vine-covered pillar from cell (33, 17) while Move */
/* holds it pushes it a cell on. */
void scMadraCatacombsPushVinePillar(void) {
	ACTOR *actor;

	if(fdLeaderCanPushObjectAhead() != 0) {
		actor = fdActorGetById(
			fdFindNearbyTalkTarget(MainRoutineStateWork.leaderActorId));
		scPillarLowerHeldCells(MadraCatacombs_VinePillar);
		fdLeaderPushObjectAhead();
		scMadraCatacombsSettleVinePillar(actor);
	}
}

/* Nothing visible happens as Move takes hold of the vine-covered pillar north
 */
/* of the village's middle; the game lowers the ground it made, so that it can
 */
/* slide. */
void scMadraCatacombsStartMovingVinePillar(void) {
	scPillarLowerHeldCells(MadraCatacombs_VinePillar);
}

void scMadraCatacombsOpenDoorAndEnter(s32 sourceX, s32 sourceZ, s32 x, s32 z,
									  s32 width, s32 height);

/* Felix Moves the vine-covered pillar north of the village's middle; where it
 */
/* comes to rest becomes ground to stand on, so he can climb its vines to the */
/* upper level, and one cell east, on cell (35, 17), it stays on later visits.
 */
void scMadraCatacombsFinishMovingVinePillar(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;

	if(state->movedBlock != 0)
		scMadraCatacombsSettleVinePillar(state->movedBlock);
}

/* The braziers down the hall before Moloch's tablet burn: each flame stands */
/* in its place, a marker stands on each, and the doorway can be walked */
/* through. */
void scMadraCatacombsLightFlames(void) {
	u32 i;
	s32 target = ACTOR_FIRST_WEST_FLARE;
	s32 actor = ACTOR_FIRST_FLAME_MARKER;

	fdActorSetPos(ACTOR_FIRST_WEST_FLARE + 4, 136, 258);
	fdActorSetPos(ACTOR_FIRST_EAST_FLARE + 4, 184, 258);
	fdActorSetPos(ACTOR_FIRST_WEST_FLARE + 3, 120, 226);
	fdActorSetPos(ACTOR_FIRST_EAST_FLARE + 3, 200, 226);
	fdActorSetPos(ACTOR_FIRST_WEST_FLARE + 2, 120, 194);
	fdActorSetPos(ACTOR_FIRST_EAST_FLARE + 2, 200, 194);
	fdActorSetPos(ACTOR_FIRST_WEST_FLARE + 1, 136, 162);
	fdActorSetPos(ACTOR_FIRST_EAST_FLARE + 1, 184, 162);
	fdActorSetPos(ACTOR_FIRST_WEST_FLARE, 136, 138);
	fdActorSetPos(ACTOR_FIRST_EAST_FLARE, 184, 138);
	for(i = 0; i < FLARES_PER_SIDE * 2; i++) {
		fdActorSetAnimation(actor, 0);
		fdActorPlaceOnActor(actor, target);
		target++;
		actor++;
	}
	fdMapGridCopyCellAttributes(0, 0, 2, 2, 9, 22);
	sySynch(1);
}

/* The red chest on the bookshelf tilts as the bookshelf rocks, and each time */
/* the bookshelf comes upright again the chest hops two pixels west with a */
/* scrape. */
void scMadraCatacombsRockChestWithBookshelf(void) {
	ACTOR *bookshelf = fdActorGetById(ACTOR_BOOKSHELF);
	ACTOR *chest = fdActorGetById(ACTOR_SHELF_CHEST);
	SPR *sprite = chest->spr;

	sprite->rotation = ((SPR *)bookshelf->spr)->rotation;
	if(fdEventGetFlag(FLAG_BOOKSHELF_LEANING) != 0 && sprite->rotation == 0) {
		fdEventPlaySnd(SE_BOOKSHELF_SLIDE);
		chest->x.fixed += toFIXED(-2);
		chest->velY = toFIXED(2);
		fdEventClearFlag(FLAG_BOOKSHELF_LEANING);
	}
	if(sprite->rotation != 0)
		fdEventSetFlag(FLAG_BOOKSHELF_LEANING);
}

/* Felix walks north into one of the dark doorways of the village's broken */
/* building, or the opening on its east side: he steps to its middle, walks in
 */
/* growing smaller, and the screen closes to the room beyond. The doorway at */
/* cell (17, 20) leads to the small hall of the upper floor, the one at (18, */
/* 22) is the main entrance into the ground floor's hall, the one at (21, 17) */
/* opens into the east room with the stairs down to the red double door, and */
/* the opening at (54, 18) leads into the Champa tunnel. */
void scMadraCatacombsEnterDoor(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;
	ACTOR *actor;
	s32 cell;

	actor = fdActorGetById(party->leaderActorId);
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdActorSetAnimation(party->leaderActorId, ANIM_WALK);
	fdActorSetSpeed(party->leaderActorId, 0.5);
	cell = FIXEDtoINT(actor->x.fixed);
	fdActorSetMoveTarget(
		actor, toFIXED(8 - cell % MAPGRID_CELL_PIXELS) + actor->x.fixed,
		actor->y.fixed, actor->z.fixed);
	scFieldActorWaitForMove(actor);
	actor->facing = FACING_NORTH;
	fdActorSetScript(party->leaderActorId, Leader_WalkThroughDoorScript);
	sySynch(12);
	fdEventPlaySnd(SE_MAP_EXIT);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syImportEventSetExitRequest(state->stepTrigger);
}

/* A door opens, its open picture copied over it from (sourceX, sourceZ), and */
/* Felix walks through: a stone door two cells wide slides with a thump, a door
 */
/* one cell wide opens as he steps to its middle. */
void scMadraCatacombsOpenDoorAndEnter(s32 sourceX, s32 sourceZ, s32 x, s32 z,
									  s32 width, s32 height) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR *actor;
	s32 cell;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	if(width == 2)
		fdEventPlaySnd(SE_THUMP);
	else
		fdEventPlaySnd(SE_DOOR_OPEN);
	fdMapGridCopyCellsTo(sourceX, sourceZ, x, z, width, height);
	fdEventWait(20);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	if(width == 1) {
		actor = fdActorGetById(MainRoutineStateWork.leaderActorId);
		cell = FIXEDtoINT(actor->x.fixed);
		fdActorSetMoveTarget(
			actor, toFIXED(8 - cell % MAPGRID_CELL_PIXELS) + actor->x.fixed,
			actor->y.fixed, actor->z.fixed);
		scFieldActorWaitForMove(actor);
		actor->facing = FACING_NORTH;
	}
	fdActorSetScript(MainRoutineStateWork.leaderActorId,
					 Leader_WalkThroughDoorScript);
	sySynch(12);
	fdEventPlaySnd(SE_MAP_EXIT);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syImportEventSetExitRequest(state->stepTrigger);
}

/* Felix walks north up the purple carpet of the broken building's main hall */
/* into the golden stone door at its end (cells (14, 33) and (15, 33)): it */
/* slides open and he goes through to the room beyond, where the chest from */
/* the bookshelf falls. */
void scMadraCatacombsOpenStoneDoor(void) {
	scMadraCatacombsOpenDoorAndEnter(78, 29, 78, 31, 2, 2);
}

/* Felix walks into the west door of the small hall on the broken building's */
/* upper floor (cell (45, 5)): it opens and he goes through to the bedroom with
 */
/* the bookshelf. */
void scMadraCatacombsOpenBookshelfRoomWestDoor(void) {
	scMadraCatacombsOpenDoorAndEnter(109, 1, 109, 3, 1, 2);
}

/* Felix walks into the east door of the small hall on the broken building's */
/* upper floor (cell (54, 5)): it opens and he goes through to the bedroom with
 */
/* the broken floor. */
void scMadraCatacombsOpenBookshelfRoomEastDoor(void) {
	scMadraCatacombsOpenDoorAndEnter(118, 1, 118, 3, 1, 2);
}

/* Felix walks into the door in the north-west corner of the small room over */
/* the drop (cell (28, 33)): it opens and he goes through to the narrow passage
 */
/* with a red chest. */
void scMadraCatacombsOpenTabletRoomDoor(void) {
	scMadraCatacombsOpenDoorAndEnter(92, 29, 92, 31, 1, 2);
}

/* Arriving by a way that is blocked off once Briggs is caught, Felix vanishes
 */
/* and the screen fades out to `exit`. */
#define DEFINE_LEAVE_BLOCKED_WAY(name, exit)                                   \
	void name(void) {                                                          \
		fdEventStart();                                                        \
		fdEventEndLastingPsynergy(0);                                          \
		fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);              \
		fdActorSetSprFlags(fdActorGetById(MainRoutineStateWork.leaderActorId), \
						   0);                                                 \
		fdActorSetSprCol(fdActorGetById(MainRoutineStateWork.leaderActorId),   \
						 SPRITE_COLOR_HIDDEN);                                 \
		sySynch(1);                                                            \
		fdEventStartScrTransition();                                           \
		fdEventWaitTransitionFrames();                                         \
		fdEventWait(40);                                                       \
		syImportEventSetExitRequest(exit);                                     \
	}

DEFINE_LEAVE_BLOCKED_WAY(
	scMadraCatacombsLeaveBlockedVillage,
	MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_WEST)

DEFINE_LEAVE_BLOCKED_WAY(scMadraCatacombsLeaveBlockedTunnel,
						 CHAMPA_TUNNEL_UPPER_PATH_END)

s32 scInit(void) {
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	if(MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_ENTRANCE)
		scMadraCatacombsSetUpVillage();
	else if(MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_CHAMPA_TUNNEL)
		scMadraCatacombsSetUpChampaTunnel();
	else if(MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_STONE_DOOR)
		scMadraCatacombsSetUpStoneDoorRoom();
	else if(MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_BOOKSHELF)
		scMadraCatacombsSetUpBookshelfRoom();
	else if(MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_TABLET)
		scMadraCatacombsSetUpTabletRoom();
	return 0;
}

s32 scPrepareMap(void) { return 0; }

/* Sets up the deserted village: its rope, the pillars Move has placed, the */
/* frozen puddle, the opened golden door and, once Briggs is caught, the */
/* Champa tunnel's way in blocked off. */
void scMadraCatacombsSetUpVillage(void) {
	fdEventClearFlag(FLAG_MADRA_CATACOMBS_BUTTON_REVEALED);
	fdActorSetSprPriority(ACTOR_VILLAGE_CHEST, 3);
	fdRopeInit();
	scFieldRopePlace(1, FLAG_VILLAGE_ROPE_TIED, ACTOR_VILLAGE_ROPE_LOW_POST,
					 ACTOR_VILLAGE_ROPE_HIGH_POST);
	fdActorDisableById(ACTOR_HOLE_PILLAR);
	if(fdEventGetFlag(FLAG_MADRA_CATACOMBS_HOLE_FILLED) != 0) {
		fdMapGridCopyCellAttributes(41, 13, 2, 1, 39, 13);
		scFieldActorSetPosAndResetMotion(fdActorGetById(ACTOR_HOLE_PILLAR),
										 toFIXED(648), 0, toFIXED(216));
	}
	if(fdEventGetFlag(FLAG_MADRA_CATACOMBS_PILLAR_PLACED) != 0)
		fdActorSetPos(ACTOR_PUSHED_PILLAR, 712, 424);
	fdActorPlaceBlocks(MadraCatacombs_PushedPillar);
	if(fdEventGetFlag(FLAG_MADRA_CATACOMBS_VINE_PILLAR_PLACED) != 0)
		fdActorSetPos(ACTOR_VINE_PILLAR, 568, 280);
	fdActorGetById(ACTOR_VINE_PILLAR)->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	scPillarRaise(MadraCatacombs_VinePillar);
	fdFlagBlockPlaceAll(MadraCatacombs_Puddles);
	if(fdEventGetFlag(FLAG_MADRA_CATACOMBS_VILLAGE_DOOR_OPEN) != 0) {
		fdMapGridCopyCellsTo(75, 66, 97, 39, 2, 2);
		fdMapGridCopyCellAttributes(33, 37, 2, 2, 33, 39);
		sySynch(1);
	}
	if(fdEventGetFlag(FLAG_BRIGGS_CAUGHT) != 0) {
		fdMapGridCopyCellsTo(82, 74, 116, 16, 5, 3);
		fdMapGridCopyCellAttributes(52, 11, 5, 1, 52, 18);
		if(MainRoutineStateWork.entrance ==
		   MADRA_CATACOMBS_ENTRANCE_FROM_MADRA_CATACOMBS_CHAMPA_TUNNEL_WEST)
			scMadraCatacombsLeaveBlockedVillage();
		else
			sySynch(10);
	}
}

/* Sets up the Champa tunnel: its rope and, once Briggs is caught, the way on */
/* to the village blocked off and the man who marvels at the cave gone. */
void scMadraCatacombsSetUpChampaTunnel(void) {
	fdRopeInit();
	scFieldRopePlace(0, FLAG_TUNNEL_ROPE_TIED, ACTOR_TUNNEL_ROPE_EAST_POST,
					 ACTOR_TUNNEL_ROPE_WEST_POST);
	if(fdEventGetFlag(FLAG_BRIGGS_CAUGHT) != 0) {
		fdActorSetPos(ACTOR_MAN_ON_THE_CHAMPAS_CAVE, 0, 0);
		fdMapGridCopyCellsTo(68, 70, 113, 11, 5, 3);
		fdMapGridCopyCellsTo(50, 74, 50, 75, 2, 1);
		fdMapGridCopyCellAttributes(49, 9, 5, 1, 49, 13);
		if(MainRoutineStateWork.entrance == CHAMPA_TUNNEL_UPPER_PATH_END)
			scMadraCatacombsLeaveBlockedTunnel();
		else
			sySynch(1);
	}
}

/* Sets up the broken building's ground floor, below the bookshelf: the chest */
/* with the Ruin Key is there only once Tremor has shaken it down. */
void scMadraCatacombsSetUpStoneDoorRoom(void) {
	if(fdEventGetFlag(FLAG_MADRA_CATACOMBS_CHEST_FELL) == 0)
		fdActorSetPos(ACTOR_FALLEN_CHEST, 0, 0);
}

/* Sets up the broken building's upper floor: the red chest stands on top of */
/* the bookshelf until Tremor has shaken it down. */
void scMadraCatacombsSetUpBookshelfRoom(void) {
	ACTOR *actor;

	if(fdEventGetFlag(FLAG_MADRA_CATACOMBS_CHEST_FELL) != 0) {
		fdActorSetPos(ACTOR_SHELF_CHEST, 0, 0);
	}
	else {
		actor = fdActorGetById(ACTOR_SHELF_CHEST);
		actor->collisionFlg = ACTOR_NOT_TARGETED;
		actor->motionFlg = ACTOR_FALLS;
		actor->y.fixed = toFIXED(32);
		actor->groundHeight = toFIXED(32);
		actor->update = scMadraCatacombsRockChestWithBookshelf;
		fdActorGetById(ACTOR_BOOKSHELF)->scaleY = FIXED_ONE * 4 / 5;
	}
}

/* Sets up the hall before Moloch's tablet: the red double door's two halves */
/* stand closed; once the Ruin Key has opened it they stand 12 pixels apart */
/* (already so when the party comes back from a battle) and the braziers */
/* burn. */
void scMadraCatacombsSetUpTabletRoom(void) {
	ACTOR *left = fdActorGetById(ACTOR_LEFT_DOOR);
	ACTOR *right = fdActorGetById(ACTOR_RIGHT_DOOR);

	left->scaleX = FIXED_ONE * 5 / 4;
	right->scaleX = -FIXED_ONE * 5 / 4;
	left->motionFlg = 0;
	right->motionFlg = 0;
	left->y.fixed = 0;
	right->y.fixed = 0;
	if(fdEventGetFlag(FLAG_MADRA_CATACOMBS_TABLET_DOOR_OPEN) != 0) {
		if(fdEventGetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
			left->x.fixed += toFIXED(-12);
			right->x.fixed += toFIXED(12);
		}
		scMadraCatacombsLightFlames();
	}
}

/* clang-format off */
#include "shared/leader_walk_through_door_script.c"

/* A brazier's flame's actor script as it flares up. */
s32 MadraCatacombs_FlameFlaresUp[] = {
    SC_SET(SW_SCALE_X, FIXED_ONE / 16),
    SC_SET(SW_SCALE_Y, FIXED_ONE / 16),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 4),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(2)),
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_END,
};

/* The actor script of the great braziers by Moloch's tablet as they flare up. */
s32 MadraCatacombs_GreatFlameFlaresUp[] = {
    SC_SET(SW_SCALE_X, FIXED_ONE / 16),
    SC_SET(SW_SCALE_Y, FIXED_ONE / 16),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 2),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 4),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 8),
    SC_ADD(SW_SCALE_Y, FIXED_ONE * 3 / 8),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(2)),
    SC_LABEL(3),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 8),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 4),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(3)),
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_END,
};

/* The plain stone pillar Move pushes, ended by -1. */
s16 MadraCatacombs_PushedPillar[] = {
    ACTOR_PUSHED_PILLAR, -1,
};

/* The puddle Frost freezes into a pillar, with its flag. */
s16 MadraCatacombs_Puddles[] = {
    ACTOR_VILLAGE_PUDDLE, FLAG_VILLAGE_PUDDLE_FROZEN, -1,
};

/* The vine-covered pillar Move pushes, ended by -1. */
s16 MadraCatacombs_VinePillar[] = {
    ACTOR_VINE_PILLAR, -1, 0,
};

#include "shared/scene_entrances_default.c"

/* The entrances of the stone door's room. */
SCENE_ENTRANCE MadraCatacombs_StoneDoorEntrances[] = {
    {3, CONDITION_ALWAYS, 512, 0, 224, FACING_NORTH, 0, 392, 40, 632, 280, 0},
    {SCENE_TABLE_END},
};

/* The entrances of the bookshelf room. */
SCENE_ENTRANCE MadraCatacombs_BookshelfEntrances[] = {
    {5, CONDITION_ALWAYS, 728, 0, 88, FACING_SOUTH, 0, 680, 32, 920, 192, 0},
    {6, CONDITION_ALWAYS, 872, 0, 88, FACING_SOUTH, 0, 680, 32, 920, 192, 0},
    {7, CONDITION_ALWAYS, 872, 0, 120, FACING_EAST, 0, 680, 32, 920, 192, 0},
    {8, CONDITION_ALWAYS, 792, 0, 152, FACING_NORTH, 0, 680, 32, 920, 192, 0},
    {SCENE_TABLE_END},
};

/* Climbing either ladder up out of the ladder room takes the party up into */
/* Madra. */
SCENE_REGION MadraCatacombs_LadderRoomRegions[] = {
    {112, 48, 144, 128, 64, 160, CONDITION_ALWAYS, MADRA_CATACOMBS_LADDERS_TO_MADRA_WEST},
    {336, 48, 144, 352, 64, 160, CONDITION_ALWAYS, MADRA_CATACOMBS_LADDERS_TO_MADRA_EAST},
    {SCENE_TABLE_END},
};

u32 SceneExits[] = {
    SCENE_EXITS(MadraCatacombsEntrance),
    SCENE_EXIT(MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_BOOKSHELF, MadraCatacombsBookshelf, MADRA_CATACOMBS_BOOKSHELF_FROM_MADRA_CATACOMBS_ENTRANCE),
    SCENE_EXIT(MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_STONE_DOOR_NORTH, MadraCatacombsStoneDoor, MADRA_CATACOMBS_STONE_DOOR_FROM_MADRA_CATACOMBS_ENTRANCE_NORTH),
    SCENE_EXIT(MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_WEST, MadraCatacombsChampaTunnel, MADRA_CATACOMBS_CHAMPA_TUNNEL_FROM_MADRA_CATACOMBS_ENTRANCE_WEST),
    SCENE_EXIT(MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_STONE_DOOR_SOUTH, MadraCatacombsStoneDoor, MADRA_CATACOMBS_STONE_DOOR_FROM_MADRA_CATACOMBS_ENTRANCE_SOUTH),
    SCENE_EXIT(MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_EAST, MadraCatacombsChampaTunnel, MADRA_CATACOMBS_CHAMPA_TUNNEL_FROM_MADRA_CATACOMBS_ENTRANCE_EAST),
    SCENE_EXITS(MadraCatacombsLadders),
    SCENE_EXIT(MADRA_CATACOMBS_LADDERS_TO_MADRA_WEST, Madra, MADRA_FROM_MADRA_CATACOMBS_LADDERS_WEST),
    SCENE_EXIT(MADRA_CATACOMBS_LADDERS_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_NORTH, MadraCatacombsChampaTunnel, MADRA_CATACOMBS_CHAMPA_TUNNEL_FROM_MADRA_CATACOMBS_LADDERS_NORTH),
    SCENE_EXIT(MADRA_CATACOMBS_LADDERS_TO_MADRA_EAST, Madra, MADRA_FROM_MADRA_CATACOMBS_LADDERS_EAST),
    SCENE_EXIT(MADRA_CATACOMBS_LADDERS_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_SOUTH, MadraCatacombsChampaTunnel, MADRA_CATACOMBS_CHAMPA_TUNNEL_FROM_MADRA_CATACOMBS_LADDERS_SOUTH),
    SCENE_EXITS(MadraCatacombsChampaTunnel),
    SCENE_EXIT(MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_ENTRANCE_WEST, MadraCatacombsEntrance, MADRA_CATACOMBS_ENTRANCE_FROM_MADRA_CATACOMBS_CHAMPA_TUNNEL_WEST),
    SCENE_EXIT(2, MadraCatacombsChampaTunnel, 3),
    SCENE_EXIT(3, MadraCatacombsChampaTunnel, 2),
    SCENE_EXIT(MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_LADDERS_SOUTH, MadraCatacombsLadders, MADRA_CATACOMBS_LADDERS_FROM_MADRA_CATACOMBS_CHAMPA_TUNNEL_SOUTH),
    SCENE_EXIT(5, MadraCatacombsChampaTunnel, 8),
    SCENE_EXIT(MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_ENTRANCE_EAST, MadraCatacombsEntrance, MADRA_CATACOMBS_ENTRANCE_FROM_MADRA_CATACOMBS_CHAMPA_TUNNEL_EAST),
    SCENE_EXIT(MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_LADDERS_NORTH, MadraCatacombsLadders, MADRA_CATACOMBS_LADDERS_FROM_MADRA_CATACOMBS_CHAMPA_TUNNEL_NORTH),
    SCENE_EXIT(8, MadraCatacombsChampaTunnel, 5),
    SCENE_EXITS(MadraCatacombsStoneDoor),
    SCENE_EXIT(MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_BOOKSHELF_NORTHWEST, MadraCatacombsBookshelf, MADRA_CATACOMBS_BOOKSHELF_FROM_MADRA_CATACOMBS_STONE_DOOR_NORTHWEST),
    SCENE_EXIT(2, MadraCatacombsStoneDoor, 11),
    SCENE_EXIT(3, MadraCatacombsStoneDoor, 9),
    SCENE_EXIT(MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_BOOKSHELF_EAST, MadraCatacombsBookshelf, MADRA_CATACOMBS_BOOKSHELF_FROM_MADRA_CATACOMBS_STONE_DOOR_EAST),
    SCENE_EXIT(MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_TABLET_EAST, MadraCatacombsTablet, MADRA_CATACOMBS_TABLET_FROM_MADRA_CATACOMBS_STONE_DOOR_EAST),
    SCENE_EXIT(MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_TABLET_WEST, MadraCatacombsTablet, MADRA_CATACOMBS_TABLET_FROM_MADRA_CATACOMBS_STONE_DOOR_WEST),
    SCENE_EXIT(MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_ENTRANCE_NORTH, MadraCatacombsEntrance, MADRA_CATACOMBS_ENTRANCE_FROM_MADRA_CATACOMBS_STONE_DOOR_NORTH),
    SCENE_EXIT(8, MadraCatacombsStoneDoor, 12),
    SCENE_EXIT(9, MadraCatacombsStoneDoor, 3),
    SCENE_EXIT(MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_BOOKSHELF_SOUTH, MadraCatacombsBookshelf, MADRA_CATACOMBS_BOOKSHELF_FROM_MADRA_CATACOMBS_STONE_DOOR_SOUTH),
    SCENE_EXIT(11, MadraCatacombsStoneDoor, 2),
    SCENE_EXIT(12, MadraCatacombsStoneDoor, 8),
    SCENE_EXIT(MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_ENTRANCE_SOUTH, MadraCatacombsEntrance, MADRA_CATACOMBS_ENTRANCE_FROM_MADRA_CATACOMBS_STONE_DOOR_SOUTH),
    SCENE_EXITS(MadraCatacombsBookshelf),
    SCENE_EXIT(MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_STONE_DOOR_NORTHWEST, MadraCatacombsStoneDoor, MADRA_CATACOMBS_STONE_DOOR_FROM_MADRA_CATACOMBS_BOOKSHELF_NORTHWEST),
    SCENE_EXIT(MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_STONE_DOOR_EAST, MadraCatacombsStoneDoor, MADRA_CATACOMBS_STONE_DOOR_FROM_MADRA_CATACOMBS_BOOKSHELF_EAST),
    SCENE_EXIT(3, MadraCatacombsBookshelf, 5),
    SCENE_EXIT(4, MadraCatacombsBookshelf, 6),
    SCENE_EXIT(5, MadraCatacombsBookshelf, 3),
    SCENE_EXIT(6, MadraCatacombsBookshelf, 4),
    SCENE_EXIT(MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_STONE_DOOR_SOUTH, MadraCatacombsStoneDoor, MADRA_CATACOMBS_STONE_DOOR_FROM_MADRA_CATACOMBS_BOOKSHELF_SOUTH),
    SCENE_EXIT(MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_ENTRANCE, MadraCatacombsEntrance, MADRA_CATACOMBS_ENTRANCE_FROM_MADRA_CATACOMBS_BOOKSHELF),
    SCENE_EXITS(MadraCatacombsTablet),
    SCENE_EXIT(MADRA_CATACOMBS_TABLET_TO_MADRA_CATACOMBS_STONE_DOOR_WEST, MadraCatacombsStoneDoor, MADRA_CATACOMBS_STONE_DOOR_FROM_MADRA_CATACOMBS_TABLET_WEST),
    SCENE_EXIT(2, MadraCatacombsTablet, 3),
    SCENE_EXIT(3, MadraCatacombsTablet, 2),
    SCENE_EXIT(MADRA_CATACOMBS_TABLET_TO_MADRA_CATACOMBS_STONE_DOOR_EAST, MadraCatacombsStoneDoor, MADRA_CATACOMBS_STONE_DOOR_FROM_MADRA_CATACOMBS_TABLET_EAST),
    SCENE_EXITS_END,
};

/* The actors placed in the catacombs' rooms with no table of their own. */
ACTOR_PLACE MadraCatacombs_Placements[] = {
    {SCENE_TABLE_END},
};

/* The actors placed in the catacombs' entrance. */
ACTOR_PLACE MadraCatacombs_EntrancePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_VINE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/* The actors placed in the Champa tunnel. */
ACTOR_PLACE MadraCatacombs_ChampaTunnelPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 794, 0, 236, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

/* The actors placed in the bookshelf room. */
ACTOR_PLACE MadraCatacombs_BookshelfPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BOOKSHELF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 202, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/* The actors placed in the tablet room. */
ACTOR_PLACE MadraCatacombs_TabletPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 150, 0, 366, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 170, 0, 366, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/* The triggers of the catacombs' rooms with no table of their own. */
TRIGGER MadraCatacombs_Triggers[] = {
    {TRIGGER_TABLE_END},
};

/* The triggers of the catacombs' entrance. */
TRIGGER MadraCatacombs_EntranceTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_BOOKSHELF, CONDITION_ALWAYS, EVENT(scMadraCatacombsEnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_STONE_DOOR_NORTH, CONDITION_ALWAYS, EVENT(scMadraCatacombsEnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_WEST, CONDITION_ALWAYS, EVENT(scMadraCatacombsEnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_STONE_DOOR_SOUTH, CONDITION_ALWAYS, EVENT(scMadraCatacombsEnterDoor)},
    {TRIGGER_EXIT, MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_EAST, CONDITION_ALWAYS, MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_EAST},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_VILLAGE_ROPE_LOW_POST, FLAG_VILLAGE_ROPE_TIED, 0},
    /* A pushed trigger answers whatever cell is pushed against */
    /* (scFindTrigger): its 10 is never compared. */
    {TRIGGER_PUSHED, 10, FLAG_MADRA_CATACOMBS_HOLE_FILLED, EVENT(scMadraCatacombsDropPillarIntoHole)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_HOLE_PILLAR, FLAG_MADRA_CATACOMBS_HOLE_FILLED, EVENT(scMadraCatacombsDropPillarIntoHole)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PUSHED_PILLAR, FLAG_MADRA_CATACOMBS_PILLAR_PLACED, EVENT(scMadraCatacombsStartMovingPushedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PUSHED_PILLAR, FLAG_MADRA_CATACOMBS_PILLAR_PLACED, EVENT(scMadraCatacombsFinishMovingPushedPillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_MADRA_CATACOMBS_VINE_PILLAR_PUSH, FLAG_MADRA_CATACOMBS_VINE_PILLAR_PLACED, EVENT(scMadraCatacombsPushVinePillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_VINE_PILLAR, FLAG_MADRA_CATACOMBS_VINE_PILLAR_PLACED, EVENT(scMadraCatacombsStartMovingVinePillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_VINE_PILLAR, FLAG_MADRA_CATACOMBS_VINE_PILLAR_PLACED, EVENT(scMadraCatacombsFinishMovingVinePillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_VILLAGE_PUDDLE, FLAG_VILLAGE_PUDDLE_FROZEN, EVENT(scMadraCatacombsFreezePuddle)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(scMadraCatacombsRevealButton)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(scMadraCatacombsHideButton)},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_MADRA_CATACOMBS_BUTTON_WALL, CONDITION_ALWAYS, EVENT(scMadraCatacombsHearEyesOfTruth)},
    {TRIGGER_TABLE_END},
};

/* The triggers of the ladder room. */
TRIGGER MadraCatacombs_LaddersTriggers[] = {
    {TRIGGER_EXIT, MADRA_CATACOMBS_LADDERS_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_NORTH, CONDITION_ALWAYS, MADRA_CATACOMBS_LADDERS_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_NORTH},
    {TRIGGER_EXIT, MADRA_CATACOMBS_LADDERS_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_SOUTH, CONDITION_ALWAYS, MADRA_CATACOMBS_LADDERS_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_SOUTH},
    {TRIGGER_TABLE_END},
};
/* clang-format on */

/* The triggers of the Champa tunnel. */
TRIGGER MadraCatacombs_ChampaTunnelTriggers[] = {
	{TRIGGER_EXIT,
	 MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_ENTRANCE_WEST,
	 CONDITION_ALWAYS,
	 MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_ENTRANCE_WEST},
	{TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
	{TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
	{TRIGGER_EXIT,
	 MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_LADDERS_SOUTH,
	 CONDITION_ALWAYS,
	 MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_LADDERS_SOUTH},
	{TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
	{TRIGGER_EXIT,
	 MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_ENTRANCE_EAST,
	 CONDITION_ALWAYS,
	 MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_ENTRANCE_EAST},
	{TRIGGER_EXIT,
	 MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_LADDERS_NORTH,
	 CONDITION_ALWAYS,
	 MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_LADDERS_NORTH},
	{TRIGGER_EXIT, CHAMPA_TUNNEL_UPPER_PATH_END, CONDITION_ALWAYS,
	 CHAMPA_TUNNEL_UPPER_PATH_END},
	{PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_TUNNEL_ROPE_EAST_POST,
	 FLAG_TUNNEL_ROPE_TIED, 0},
	{TRIGGER_TALK, ACTOR_MAN_ON_THE_CHAMPAS_CAVE, CONDITION_ALWAYS,
	 MSGNO(MsgFieldCaveFoundByChampa)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_CHAMPAS_CAVE,
	 CONDITION_ALWAYS, MSGNO(MsgFieldWhoDugCave)},
	{TRIGGER_TABLE_END},
};

/* The triggers of the stone door's room. */
TRIGGER MadraCatacombs_StoneDoorTriggers[] = {
	{TRIGGER_EXIT,
	 MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_BOOKSHELF_NORTHWEST,
	 CONDITION_ALWAYS,
	 MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_BOOKSHELF_NORTHWEST},
	{TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
	{TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
	{TRIGGER_EXIT, MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_BOOKSHELF_EAST,
	 CONDITION_ALWAYS,
	 MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_BOOKSHELF_EAST},
	{TRIGGER_EXIT, MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_TABLET_EAST,
	 CONDITION_ALWAYS,
	 MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_TABLET_EAST},
	{TRIGGER_EXIT, MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_TABLET_WEST,
	 CONDITION_ALWAYS,
	 MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_TABLET_WEST},
	{TRIGGER_EXIT, MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_ENTRANCE_NORTH,
	 CONDITION_ALWAYS,
	 MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_ENTRANCE_NORTH},
	{TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS,
	 EVENT(scMadraCatacombsOpenStoneDoor)},
	{TRIGGER_EXIT,
	 MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_BOOKSHELF_SOUTH,
	 CONDITION_ALWAYS,
	 MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_BOOKSHELF_SOUTH},
	{TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
	{TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
	{TRIGGER_EXIT, MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_ENTRANCE_SOUTH,
	 CONDITION_ALWAYS,
	 MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_ENTRANCE_SOUTH},
	{TRIGGER_TABLE_END},
};

/* The triggers of the bookshelf room. */
TRIGGER MadraCatacombs_BookshelfTriggers[] = {
	{TRIGGER_EXIT,
	 MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_STONE_DOOR_NORTHWEST,
	 CONDITION_ALWAYS,
	 MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_STONE_DOOR_NORTHWEST},
	{TRIGGER_EXIT, MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_STONE_DOOR_EAST,
	 CONDITION_ALWAYS,
	 MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_STONE_DOOR_EAST},
	{TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
	{TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS,
	 EVENT(scMadraCatacombsOpenBookshelfRoomWestDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS,
	 EVENT(scMadraCatacombsOpenBookshelfRoomEastDoor)},
	{TRIGGER_EXIT,
	 MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_STONE_DOOR_SOUTH,
	 CONDITION_ALWAYS,
	 MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_STONE_DOOR_SOUTH},
	{TRIGGER_EXIT, MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_ENTRANCE,
	 CONDITION_ALWAYS, MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_ENTRANCE},
	{PSYNERGY_ON_ACTOR(ABILITY_TREMOR), ACTOR_BOOKSHELF, CONDITION_ALWAYS,
	 EVENT(scMadraCatacombsShakeBookshelf)},
	{TRIGGER_TABLE_END},
};

/* The triggers of the tablet room. */
TRIGGER MadraCatacombs_TabletTriggers[] = {
	{TRIGGER_EXIT, MADRA_CATACOMBS_TABLET_TO_MADRA_CATACOMBS_STONE_DOOR_WEST,
	 CONDITION_ALWAYS,
	 MADRA_CATACOMBS_TABLET_TO_MADRA_CATACOMBS_STONE_DOOR_WEST},
	{TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS,
	 EVENT(scMadraCatacombsOpenTabletRoomDoor)},
	{TRIGGER_EXIT, MADRA_CATACOMBS_TABLET_TO_MADRA_CATACOMBS_STONE_DOOR_EAST,
	 CONDITION_ALWAYS,
	 MADRA_CATACOMBS_TABLET_TO_MADRA_CATACOMBS_STONE_DOOR_EAST},
	{USE_ITEM(ITEM_RUIN_KEY), CELL_BEHAVIOR_MADRA_CATACOMBS_TABLET_DOOR,
	 CONDITION_ALWAYS, EVENT(scMadraCatacombsUnlockWithRuinKey)},
	{TRIGGER_EXAMINE, CELL_BEHAVIOR_MADRA_CATACOMBS_TABLET_DOOR,
	 FLAG_MADRA_CATACOMBS_TABLET_DOOR_OPEN,
	 EVENT(scMadraCatacombsTryLockedDoor)},
	{TRIGGER_TABLE_END},
};
