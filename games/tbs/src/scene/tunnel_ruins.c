/* @file */
/* The Tunnel Ruins run from the basement of Babi Lighthouse in Lalivero to */
/* the upper floors of Venus Lighthouse: the way on after the lighthouse's */
/* own base ends in a dead end. They are two scenes of Psynergy puzzles, with */
/* no story event, and a chest of armour at the end of each of two routes. */
/* */
/* Both have the area name Tunnel Ruins, but for the rooms of Babi */
/* Lighthouse's basement where the slides of Babi Lighthouse Entry land, */
/* which the area names call Babi Lighthouse. */
/* */
/* - TunnelRuinsGreatStatue (SceneId_TunnelRuinsGreatStatue) is entered by */
/*   the stone door Reveal finds in Babi Lighthouse. Its first room holds a */
/*   giant pink statue, with a small model of Venus Lighthouse on the floor */
/*   before it: searching it tells the party to seek "the lady's leave", the */
/*   great statue in Venus Lighthouse, whose answer opens one of two routes. */
/*   South of it, Isaac pushes one of two gold-topped pillars so that he can */
/*   hop across their tops past a wall. Beyond, a room of stone columns in a */
/*   pit gets one more column for each route opened. One route goes by a */
/*   second pair of pillars and a room where Whirlwind blows ivy off a small */
/*   pillar, then by the balance and its carved stones to the Oracle's Robe. */
/*   The other goes by a room of two pillars that sink onto water crests, */
/*   raising stepping stones from a pit, to the Asura's Armor. */
/* - TunnelRuinsLadyStatues (SceneId_TunnelRuinsLadyStatues) holds the rest: */
/*   Babi Lighthouse's basement with its two grey pillars; the balance, two */
/*   platforms Carry weighs down with three carved stones; the two white */
/*   statues that Reveal makes vanish from the chests they stand on; a room */
/*   of four walkways over a pit; and a pillar to push, Douse and Frost. */
/*   Last come five coloured statues of a woman that open the door to Venus */
/*   Lighthouse once all five stand on their marks */
/*   (FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN). */
/* */
/* The getters choose by MainRoutineStateWork.next. The actor placements of */
/* TunnelRuinsLadyStatues answer any other scene number, and */
/* scGetActorPlacements prepares each table's actors to appear only */
/* in view. */
/* */
/* scInit works room by room, one case for each room's */
/* entrances. */
/* */
/* - It puts each puzzle back as it was left. */
/* - It redraws the first room and the walkways for the path the statue in */
/*   Venus Lighthouse opened (FLAG_VENUS_LIGHTHOUSE_FIRST_PATH or */
/*   FLAG_VENUS_LIGHTHOUSE_SECOND_PATH). */
/* - It sets the retreat point. */
/* - In the rooms that need it, it starts a ground mask that follows the */
/*   leader south of a depth set per room */
/*   (scTunnelRuinsUpdateGroundMaskLeader). */
/* */
/* The walking events, keyed on the cell behaviors under the leader, are shared
 */
/* by both scenes. Walking against a one-cell gap or a pillar makes Isaac leap
 */
/* over it to the cell beyond (scTunnelRuinsHopLeaderTo), from a ledge onto a */
/* pillar's top, from top to top, or over a pillar standing on a walkway. */
/* Others jump him down a ledge or hop him down a step, and walking into a */
/* pillar, a carved stone or a statue pushes it a cell. */
/* */
/* The puzzles that must survive a visit keep story flags */
/* (FLAG_TUNNEL_RUINS_IVY_BLOWN_AWAY, */
/* FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_IN_FLOOR, */
/* FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_IN_FLOOR, */
/* FLAG_TUNNEL_RUINS_EAST_PILLAR_A_PUSHED_WEST and */
/* FLAG_TUNNEL_RUINS_EAST_PILLAR_B_PUSHED_WEST). */
/* */
/* The rest keep scene flags, which mean different things in each scene: */
/* */
/* - TunnelRuinsGreatStatue: the two halves of the ivy over the small pillar */
/*   blown away (FLAG_TUNNEL_RUINS_IVY_WEST_HALF_GONE and _EAST_HALF_GONE), and
 */
/*   in the sinking pillars' room, on the same two flags, the two pillars' */
/*   Frost targets frozen; and the south-east pillar soaked by Douse. */
/* - TunnelRuinsLadyStatues: the carved stones set on the balance */
/*   (FLAG_TUNNEL_RUINS_FIRST_STONE_SET on), and the Douse pillar soaked and */
/*   its Frost target frozen. */
/* */
/* Each balance platform's timer is its weight. Both start at 3. A carved stone
 */
/* set on one adds one to it and takes one from the other. */
/* Balance_PlatformHeights turns each weight into the platform's height. */
/* */
/* Coming into the balance's room from the north, out of the first scene, sets
 */
/* the balance up empty. Coming back up from the south */
/* (TUNNEL_RUINS_LADY_STATUES_BALANCE_SOUTH) sets all three stones on it, one */
/* stone on the west platform and two stacked on the east, weights 2 and 4. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "debug/debug.h"
#include "field/event_inline.h"
#include "field/ground_mask.h"
#include "graphics/graphics.h"
#include "sdk/sound_driver_symbols.h"
#include "text/msg_ids.h"

/* The scene flags, which mean different things in each scene. */

/* In TunnelRuinsGreatStatue's small pillars' room, Whirlwind has blown away */
/* the west half of the clump of ivy on the ledge. */
#define FLAG_TUNNEL_RUINS_IVY_WEST_HALF_GONE FLAG_SCENE_TEMP_00
/* The east half of that clump is gone. */
#define FLAG_TUNNEL_RUINS_IVY_EAST_HALF_GONE FLAG_SCENE_TEMP_01
/* In TunnelRuinsGreatStatue's sinking pillars' room, Frost has raised a */
/* pillar of ice from the north-west pillar, and Isaac can no longer hop over */
/* it. */
#define FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_FROZEN FLAG_SCENE_TEMP_00
/* The same for the south-east pillar. */
#define FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_FROZEN FLAG_SCENE_TEMP_01
/* Douse has filled the south-east pillar with water, so that it sinks on its */
/* crest. */
#define FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_SOAKED FLAG_SCENE_TEMP_02
/* In TunnelRuinsLadyStatues, the first of the three carved stones is set on */
/* the balance; the next two flags are the second and third. */
#define FLAG_TUNNEL_RUINS_FIRST_STONE_SET FLAG_SCENE_TEMP_00
/* In TunnelRuinsLadyStatues, Douse has soaked the pillar, and its Frost target
 */
/* follows it. */
#define FLAG_TUNNEL_RUINS_DOUSE_PILLAR_SOAKED FLAG_SCENE_TEMP_03
/* In TunnelRuinsLadyStatues, Frost has raised a pillar of ice from the soaked
 */
/* pillar's hidden target, 32 pixels below the pillar. */
#define FLAG_TUNNEL_RUINS_DOUSE_FROST_TARGET_FROZEN FLAG_SCENE_TEMP_04

extern u32 Balance_PlatformHeights[];
extern s32 TunnelRuins_DouseSplashScript[];
extern u16 TunnelRuins_DoorOpenSteps[];
extern u16 TunnelRuins_DoorCloseSteps[];
extern SCENE_ENTRANCE TunnelRuinsEntrancesOther[];
extern SCENE_ENTRANCE TunnelRuinsGreatStatueEntrances[];
extern SCENE_ENTRANCE TunnelRuinsLadyStatuesEntrances[];
extern SCENE_EXIT_REGION TunnelRuinsLadyStatuesRegions[];
extern u32 TunnelRuinsExits[];
extern ACTOR_PLACE TunnelRuinsGreatStatueActorPlacements[];
extern ACTOR_PLACE TunnelRuinsActorPlacementsOther[];
extern TRIGGER TunnelRuinsGreatStatueTriggers[];
extern TRIGGER TunnelRuinsTriggersOther[];

/* The cell behaviors TunnelRuinsGreatStatue's triggers answer, past its exits.
 */
enum {
	/* The ledge east of the first pair of pillars, cell (14, 42). */
	CELL_BEHAVIOR_TUNNEL_RUINS_FIRST_PILLARS_LEDGE = 23,
	/* The floor the first pair of pillars is pushed across. */
	CELL_BEHAVIOR_TUNNEL_RUINS_FIRST_PILLARS_FLOOR,
	/* The step down in the south-east of the small pillars' room, cell */
	/* (33, 54). */
	CELL_BEHAVIOR_TUNNEL_RUINS_SMALL_PILLARS_STEP,
	/* The floor the second pair of pillars is pushed across. */
	CELL_BEHAVIOR_TUNNEL_RUINS_SECOND_PILLARS_FLOOR,
	/* The ledge west of the second pair of pillars, cell (27, 33). */
	CELL_BEHAVIOR_TUNNEL_RUINS_SECOND_PILLARS_LEDGE,
	/* The floor the small pillars are pushed across. */
	CELL_BEHAVIOR_TUNNEL_RUINS_SMALL_PILLARS_FLOOR,
	/* The north walkway of the sinking pillars' room, where its pillar moves.
	 */
	CELL_BEHAVIOR_TUNNEL_RUINS_NORTH_WALKWAY,
	/* The south walkway, where the grey pillar moves. */
	CELL_BEHAVIOR_TUNNEL_RUINS_SOUTH_WALKWAY,
	/* The north walkway's east end, cell (47, 8), beside the pit. */
	CELL_BEHAVIOR_TUNNEL_RUINS_NORTH_WALKWAY_END,
	/* The south walkway's west end, cell (47, 14), beside the pit. */
	CELL_BEHAVIOR_TUNNEL_RUINS_SOUTH_WALKWAY_END,
	/* Before the giant statue, cell (12, 7). */
	CELL_BEHAVIOR_TUNNEL_RUINS_GIANT_STATUE = 40,
	/* A small pillar's top, and the ledge above them at cell (32, 50). */
	CELL_BEHAVIOR_TUNNEL_RUINS_SMALL_PILLAR_TOP = 50
};

/* The cell behaviors the triggers of TunnelRuinsLadyStatues answer, past its */
/* exits. */
enum {
	/* The door east of the five coloured statues, cell (32, 47), while shut. */
	CELL_BEHAVIOR_TUNNEL_RUINS_STATUE_DOOR = 20,
	/* The floor the Douse pillar is pushed across. */
	CELL_BEHAVIOR_TUNNEL_RUINS_DOUSE_PILLAR_FLOOR = 25,
	/* The ledge north-east of the Douse pillar's floor, cell (28, 10). */
	CELL_BEHAVIOR_TUNNEL_RUINS_DOUSE_PILLAR_LEDGE,
	/* The step east of the Douse pillar's floor, cell (28, 11). */
	CELL_BEHAVIOR_TUNNEL_RUINS_DOUSE_PILLAR_STEP,
	/* The floor north of the balance, where the carved stones stand. */
	CELL_BEHAVIOR_TUNNEL_RUINS_CARVED_STONES_FLOOR,
	/* The row of Babi Lighthouse's two grey pillars, cells (44, 19) and */
	/* (47, 19). */
	CELL_BEHAVIOR_TUNNEL_RUINS_GREY_PILLARS_ROW,
	/* A balance platform, or a carved stone on one, risen near the floor's */
	/* height; copied from cell (4, 9). */
	CELL_BEHAVIOR_TUNNEL_RUINS_RISEN_PLATFORM,
	/* The floor the five coloured statues are moved across. */
	CELL_BEHAVIOR_TUNNEL_RUINS_LADY_STATUES_FLOOR = 41,
	/* A coloured statue's cell, copied from cell (20, 56). */
	CELL_BEHAVIOR_TUNNEL_RUINS_LADY_STATUE
};

/* The actors of TunnelRuinsGreatStatue, in the order of its actor placements.
 */
/* */
/* They are two pairs of pillars to push or Move, three small pillars, three */
/* patches of ivy for Whirlwind, two pillars that sink into the floor, and two
 */
/* hidden targets for Frost, both placed with the first sinking pillar. */
enum {
	ACTOR_FIRST_PILLAR_A = ACTOR_FIRST_PLACED,
	ACTOR_FIRST_PILLAR_B,
	ACTOR_SECOND_PILLAR_A,
	ACTOR_SECOND_PILLAR_B,
	/* Three small stone pillars with white tops (SPRITE_SMALL_PILLAR, the size
	 */
	/* of the sinking pillars). The leader pushes them by walking into them */
	/* (scTunnelRuinsPushSmallPillars) or shifts them with Move. This one stands
	 */
	/* south of the ivy's ledge. */
	ACTOR_SMALL_PILLAR_A,
	/* Beside ACTOR_SMALL_PILLAR_A. */
	ACTOR_SMALL_PILLAR_B,
	/* On the ledge in the room's north-west corner, hidden in the clump of ivy
	 */
	/* until Whirlwind has blown away both its halves. */
	ACTOR_SMALL_PILLAR_C,
	/* The west half of the clump of ivy on the ledge above the small pillars.
	 */
	/* It is drawn with SPRITE_IVY, the sprite of the ivy that traps the stump
	   at */
	/* Goma Cave's entrance. */
	ACTOR_IVY_A,
	/* The east half of the clump. */
	ACTOR_IVY_B,
	/* Ivy filling the arched doorway in the room's east wall, the way on to the
	 */
	/* long passage. */
	ACTOR_IVY_C,
	ACTOR_NORTHWEST_SINKING_PILLAR,
	ACTOR_SOUTHEAST_SINKING_PILLAR,
	/* Frost's target on the north-west pillar. It is SPRITE_PUDDLE in */
	/* SPRITE_COLOR_HIDDEN, which moves onto the pillar when it sinks. */
	ACTOR_NORTHWEST_PILLAR_FROST_TARGET,
	/* The same hidden target for the south-east pillar. */
	ACTOR_SOUTHEAST_PILLAR_FROST_TARGET
};

/* The actors of TunnelRuinsLadyStatues and any other scene. */
/* */
/* They are a pillar for Douse with a hidden target for Frost, two balance */
/* platforms with three carved stones to Carry onto them, two statues that */
/* vanish under Reveal, two pillars to push in the east, and five coloured */
/* statues of a woman to Move before the door. */
enum {
	ACTOR_DOUSE_PILLAR = ACTOR_FIRST_PLACED,
	/* Frost's target on the Douse pillar. It is SPRITE_PUDDLE in */
	/* SPRITE_COLOR_HIDDEN, held 32 pixels below the soaked pillar. */
	ACTOR_DOUSE_PILLAR_FROST_TARGET,
	ACTOR_BALANCE_WEST,
	ACTOR_BALANCE_EAST,
	ACTOR_WEST_CARRY_STONE,
	ACTOR_MIDDLE_CARRY_STONE,
	ACTOR_EAST_CARRY_STONE,
	ACTOR_WEST_VANISHING_STATUE,
	ACTOR_EAST_VANISHING_STATUE,
	ACTOR_EAST_PILLAR_A,
	ACTOR_EAST_PILLAR_B,
	/* The five coloured statues of a woman, by colour, in their places at the
	 */
	/* start: violet north-west, green north-east, blue south-west, red */
	/* south-east and yellow, gold in colour, south of the middle. Mind Read on
	 */
	/* each finds a hint where another belongs: the violet one "Red shines */
	/* northwest of me.", the green one "Yellow shines northeast of me.", the */
	/* blue one "Red shines due west of me.", and the red one and the yellow */
	/* one "Violet shines southeast of me." */
	ACTOR_VIOLET_LADY_STATUE,
	ACTOR_GREEN_LADY_STATUE,
	ACTOR_BLUE_LADY_STATUE,
	ACTOR_RED_LADY_STATUE,
	ACTOR_YELLOW_LADY_STATUE
};

/* The chests of TunnelRuinsLadyStatues, each on the cell of a vanishing */
/* statue, which hides it until Reveal makes the statue go. */
enum {
	TREASURE_SPOT_ORACLES_ROBE = TREASURE_SPOT_FIRST_ID,
	TREASURE_SPOT_ASURAS_ARMOR
};

static UNREAD_RESULT scTunnelRuinsRideStoneBelow(ACTOR *actor);
static void scTunnelRuinsUpdateFirstPillarCells(void);
static void scTunnelRuinsUpdateSecondPillarCells(void);
static void scTunnelRuinsUpdateSmallPillarCells(void);
static void scTunnelRuinsUpdateGreyPillarCells(void);

#include "shared/pushed_actor.c"

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"

/* Takes the shadow off the small Venus Lighthouse on the floor before the */
/* giant statue, once TunnelRuins_SmallLighthouseScript has made it small. */
static s32 scTunnelRuinsHideLighthouseShadow(ACTOR *actor) {
	fdActorSetSprFlags(actor, 0);
	return 0;
}

/* Later patches land at the cell's centre. The base release uses the target */
/* pixel. */
static inline s32 scTunnelRuinsGetLandingPixel(FIXED coordinate) {
#if TBS_PATCH_1
	return (FIELD_CELL(coordinate) << 4) + 8;
#else
	return (s16)(FIXEDtoINT(coordinate));
#endif
}

/* Isaac crouches, leaps over the cell ahead with the jump sound and lands on */
/* the target, when that cell is no wall and the target is clear: across a gap
 */
/* or over a pillar. Returns 1 when there is no hop. */
static s32 scTunnelRuinsHopLeaderTo(FIELD_POS *target) {
	ACTOR *actor = fdActorGet(ACTOR_PARTY_LEADER);
	u8 savedMotionFlags = actor->motionFlg;
	FIELD_POS adjacentTile;

	adjacentTile.x = (actor->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
	adjacentTile.y = actor->y.fixed;
	adjacentTile.z = (actor->z.fixed & FIELD_CELL_MASK) + toFIXED(8);

	syFieldVectorAddPolarOffset(16, FACING_NEAREST_CARDINAL(actor->facing),
								&adjacentTile);

	if(syFieldActorCheckMovementCollision(actor, &adjacentTile) != 1 &&
	   syFieldActorCheckMovementCollision(actor, target) == 0) {
		fdEventStart();
		fdActorSelectAnimation(actor, ANIM_LEADER_SPRING);
		sySynch(6);
		sySndPlay(SE_JUMP);
		fdActorSelectAnimation(actor, ANIM_LEADER_LEAP);

		actor->speedLimit = toFIXED(3);
		actor->acceleration = toFIXED(2);
		actor->velY = toFIXED(4);
		actor->motionFlg &= ACTOR_JUMP_KEEPS_MOTION;

		fdActorSetSprFlags(actor, 0);
		fdActorMoveToAndWait(ACTOR_PARTY_LEADER,
							 scTunnelRuinsGetLandingPixel(target->x),
							 scTunnelRuinsGetLandingPixel(target->z));
		fdActorSelectAnimation(actor, ANIM_LEADER_SPRING);
		fdActorSetSprFlags(actor, SPRITE_FLAG_SHADOW);
		sySynch(6);

		actor->motionFlg = savedMotionFlags;
		fdEventEnd();
		return 0;
	}
	return 1;
}

/* Walking south against a one-cell gap or a pillar makes Isaac leap over it */
/* and land two cells on: from the ledge onto a small pillar's top, from top */
/* to top, or over a pillar on its walkway in the sinking pillars' room. */
/* */
/* It also runs from the row of Babi Lighthouse's two grey pillars in the */
/* basement (cells (44, 19) and (47, 19)). */
static void scTunnelRuinsHopSouth(void) {
	FIELD_POS target;
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);

	target.x = leader->x.fixed;
	target.y = leader->y.fixed;
	target.z = leader->z.fixed + toFIXED(32);
	scTunnelRuinsHopLeaderTo(&target);
}

/* Walking north against a one-cell gap or a pillar makes Isaac leap over it */
/* and land two cells on: north over a pillar on its walkway in the sinking */
/* pillars' room, or from the row of Babi Lighthouse's two grey pillars in the
 */
/* basement (cells (44, 19) and (47, 19)). */
static void scTunnelRuinsHopNorth(void) {
	FIELD_POS target;
	ACTOR *actor = fdActorGet(ACTOR_PARTY_LEADER);

	target.x = actor->x.fixed;
	target.y = actor->y.fixed;
	target.z = actor->z.fixed - toFIXED(32);
	scTunnelRuinsHopLeaderTo(&target);
}

/* The ground mask (fdEffectStartGroundMask) follows the leader only */
/* south of a row of half cells set per room. */
static void scTunnelRuinsUpdateGroundMaskLeader(void) {
	ACTOR *leader;
	GROUND_MASK_STATE *effectState;
	s32 boundaryRow;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	effectState = GroundMaskStateWork;
	boundaryRow = 0;
	if(MainRoutineStateWork.next == SCENE_TUNNEL_RUINS_GREAT_STATUE) {
		switch(MainRoutineStateWork.entrance) {
		case TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_FROM_STATUE:
		case TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_FROM_COLUMNS_NORTH:
			boundaryRow = 94;
			break;
		case TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_FROM_COLUMNS_NORTH:
		case TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_FROM_WHIRLWIND_ROOM:
			boundaryRow = 74;
			break;
		case TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_FROM_SECOND_PILLARS:
		case TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_FROM_LONG_PASSAGE:
			boundaryRow = 118;
			break;
		}
	}
	else if(MainRoutineStateWork.entrance ==
			TUNNEL_RUINS_LADY_STATUES_REVEAL_STATUE_WEST) {
		boundaryRow = 93;
	}
	if(FIELD_HALF_CELL(leader->z.fixed) <= boundaryRow) {
		effectState->actor = NULL;
	}
	else {
		effectState->actor = leader;
	}
}

#include "shared/actor_depth.c"

/* While Douse rains on a pillar: its two frames alternate, and every fourth */
/* frame a small splash appears on its top. */
static UNREAD_RESULT scTunnelRuinsSplashDousedPillar(ACTOR *actor) {
	EFFECT_OPTIONS options;
	FIXED x;
	FIXED z;

	if(FrameCount & 2)
		fdActorSelectAnimation(actor, 1);
	else
		fdActorSelectAnimation(actor, 2);
	if(FrameCount & 3)
		RETURN_UNREAD(0);
	options.startScaleX = 3 * FIXED_ONE / 10;
	options.startScaleY = 3 * FIXED_ONE / 10;
	options.pal = SPRITE_COLOR_BRG;
	x = actor->x.fixed + INTtoFIXED(syRandomBelow(7) - 3);
	z = actor->z.fixed + INTtoFIXED(syRandomBelow(7) - 3);
	fdEffectSpawn(x, actor->y.fixed + toFIXED(16), z, 0, 0, 0,
				  EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE |
					  EFFECT_SCRIPT_MEDIUM,
				  &options);
	RETURN_UNREAD(0);
}

/* Once the Douse pillar is soaked, its hidden Frost target lies 32 pixels */
/* below it while the pillar stands in map row 10 and Isaac is no more than 48
 */
/* pixels down. Anywhere else the target waits at the map's corner, out of */
/* Frost's reach. */
static UNREAD_RESULT scTunnelRuinsFrostTargetFollowsPillar(ACTOR *frostTarget) {
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);

	if(leader->y.fixed > -toFIXED(48) &&
	   (FIELD_CELL(fdActorGet(ACTOR_DOUSE_PILLAR)->z.fixed)) == 10) {
		frostTarget->x.fixed = fdActorGet(ACTOR_DOUSE_PILLAR)->x.fixed;
		frostTarget->y.fixed = -toFIXED(32);
		frostTarget->z.fixed = fdActorGet(ACTOR_DOUSE_PILLAR)->z.fixed;
	}
	else {
		frostTarget->x.fixed = 0;
		frostTarget->y.fixed = 0;
		frostTarget->z.fixed = 0;
	}
	RETURN_UNREAD(0);
}

/* Waits up to 60 frames for landing, then stops vertical motion at the */
/* ground. */
static void scTunnelRuinsWaitForActorToLand(ACTOR *actor) {
	s32 remainingFrames = 60;
	FIXED groundHeight;

	for(;;) {
		if(remainingFrames != 0) {
			FIXED height;

			sySynch(1);
			height = actor->y.fixed;
			groundHeight = actor->groundHeight;
			remainingFrames--;
			if(height <= groundHeight) {
				break;
			}
			continue;
		}
		groundHeight = actor->groundHeight;
		break;
	}
	actor->velY = 0;
	actor->y.fixed = groundHeight;
	actor->tgtY = ACTOR_NO_TARGET;
}

#include "shared/field_effect_advance.c"

/* Walking south off the step in the south-east of the small pillars' room */
/* (cell (33, 54)), or off the east edge of the Douse pillar's floor (cell */
/* (28, 11)), makes Isaac hop down it with a thud and a puff of dust. */
/* */
/* He leaps 12 pixels forward, then sinks 16 pixels to the floor below over 8 */
/* frames while the dust flies out behind him. */
static void scTunnelRuinsHopDownStep(void) {
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);
	EFFECT_OPTIONS options;
	u32 i;

	fdEventStart();
	fdActorSelectAnimation(leader, ANIM_LEADER_SPRING);
	fdActorWaitForAnimationChange(ACTOR_PARTY_LEADER);
	fdActorSelectAnimation(leader, ANIM_STAND);
	fdActorSetSprFlags(leader, 0);
	leader->motionFlg |= ACTOR_FALLS;
	sySndPlay(SE_JUMP);
	leader->velY = toFIXED(4);
	syFieldActorSetMoveTargetFX(leader, leader->x.fixed, leader->y.fixed,
								leader->z.fixed + toFIXED(12));
	sySynch(6);
	leader->motionFlg = 0;
	options.update = (void (*)(ACTOR *))scFieldEffectAdvanceMotion;
	sySndPlay(SE_LANDING_THUD);
	for(i = 0; i < 8; i++) {
		leader->y.fixed -= toFIXED(2);
		leader->tgtY = leader->y.fixed;
		sySynch(1);
		if(i & 1) {
			s32 horizontalRoll = (u32)syRandomNext() % 10 - 5;
			s32 velocityX = horizontalRoll * (2 * (toFIXED(1) / 10));
			FIXED velocityZ;

			velocityZ = (u32)syRandomNext() % 10 * -(toFIXED(1) / 10) -
						5 * (toFIXED(1) / 10);
			fdEffectSpawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
						  velocityX, 0, velocityZ,
						  EFFECT_USE_UPDATE | EFFECT_SCRIPT_MEDIUM, &options);
		}
	}
	fdActorSetSprFlags(leader, SPRITE_FLAG_SHADOW);
	leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdEventEnd();
}

/* The update functions the stones take, cast to the update slot's type */
/* where they return a value. */
#ifdef UBFIX
#define UPDATE_BY_RELATIVE_DEPTH scActorUpdateBlockDrawOrder
#else
#define UPDATE_BY_RELATIVE_DEPTH                                               \
	((void (*)(ACTOR *))scActorUpdateBlockDrawOrder)
#endif
#ifdef UBFIX
#define UPDATE_RIDE_STONE_BELOW scTunnelRuinsRideStoneBelow
#else
#define UPDATE_RIDE_STONE_BELOW ((void (*)(ACTOR *))scTunnelRuinsRideStoneBelow)
#endif

#include "shared/field_effect_radial.c"

/* The entrances of TunnelRuinsGreatStatue, TunnelRuinsLadyStatues, or */
/* a table for any other scene number. */
const SCENE_ENTRANCE *scGetEntrances(void) {
	s16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_TUNNEL_RUINS_GREAT_STATUE) {
		return TunnelRuinsGreatStatueEntrances;
	}
	if(sceneId == SCENE_TUNNEL_RUINS_LADY_STATUES) {
		return TunnelRuinsLadyStatuesEntrances;
	}
	return TunnelRuinsEntrancesOther;
}

const SCENE_EXIT_REGION *scGetExitRegions(void) {
	if(MainRoutineStateWork.next == SCENE_TUNNEL_RUINS_LADY_STATUES) {
		return TunnelRuinsLadyStatuesRegions;
	}
	return 0;
}

const u32 *scGetExits(void) { return TunnelRuinsExits; }

/* The actors of TunnelRuinsGreatStatue or of TunnelRuinsLadyStatues */
/* (the enums above), each prepared to appear only when it lies in the view. */
const ACTOR_PLACE *scGetActorPlacements(void) {
	ACTOR_PLACE *table;

	if(MainRoutineStateWork.next == SCENE_TUNNEL_RUINS_GREAT_STATUE) {
		table = TunnelRuinsGreatStatueActorPlacements;
	}
	else {
		table = TunnelRuinsActorPlacementsOther;
	}
	scAssignViewFlags(table);
	return table;
}

/* Walking into one of the two gold-topped pillars in the room south of the */
/* giant statue pushes it a cell, across their floor (cells (9, 38) to */
/* (13, 43)). */
/* */
/* Pushing one east lets Isaac hop from the ledge east of them onto its top */
/* and on past the wall. */
static void scTunnelRuinsPushFirstPillars(void) {
	fdEventStart();
	fdMapGridCopyCellAttributes(73, 38, 5, 5, 9, 38);
	scPushedActorAdvancePair();
	scTunnelRuinsUpdateFirstPillarCells();
	fdEventEnd();
}

/* Walking west against a one-cell gap or a pillar makes Isaac leap over it */
/* and land two cells on. In the room south of the giant statue he leaps from */
/* the ledge at cell (14, 42) onto the top of a pillar pushed below it. In the
 */
/* sinking pillars' room he leaps from the south walkway's west end (cell */
/* (47, 14)) onto the stepping stone the south-east pillar raised from the pit,
 */
/* or over a pillar on its walkway. */
static void scTunnelRuinsHopWest(void) {
	FIELD_POS target;
	ACTOR *actor = fdActorGet(ACTOR_PARTY_LEADER);

	target.x = actor->x.fixed - toFIXED(32);
	target.y = actor->y.fixed;
	target.z = actor->z.fixed;
	scTunnelRuinsHopLeaderTo(&target);
}

/* Walking into one of the second pair of gold-topped pillars, on the route */
/* past the small pillars, pushes it a cell, across their floor (cells */
/* (28, 30) to (34, 34)). */
/* */
/* With both pushed down and to the west, Isaac can hop onto their tops from */
/* the ledge at cell (27, 33) and cross to the next room. */
static void scTunnelRuinsPushSecondPillars(void) {
	fdEventStart();
	fdMapGridCopyCellAttributes(93, 30, 6, 5, 29, 30);
	scPushedActorAdvancePair();
	scTunnelRuinsUpdateSecondPillarCells();
	fdEventEnd();
}

/* Walking south off the ledge beside the pillars in the room south of the */
/* giant statue (cell (14, 42)) or in the second pillars' room (cell (27, 33)),
 */
/* or off the ledge north-east of the Douse pillar (cell (28, 10)), makes Isaac
 */
/* jump down to the floor below. */
static void scTunnelRuinsJumpDownLedge(void) {
	syFieldActorJumpDownLedge(ACTOR_PARTY_LEADER, 6, 0);
}

/* Walking east against a one-cell gap or a pillar makes Isaac leap over it */
/* and land two cells on. In the second pillars' room he leaps from the ledge */
/* at cell (27, 33) onto the top of a pillar pushed beside it; in the small */
/* pillars' room, from one pillar's top to the next. In the sinking pillars' */
/* room he leaps from the north walkway's east end (cell (47, 8)) onto the */
/* stepping stone the north-west pillar raised from the pit. */
static void scTunnelRuinsHopEast(void) {
	FIELD_POS target;
	ACTOR *actor = fdActorGet(ACTOR_PARTY_LEADER);

	target.x = actor->x.fixed + toFIXED(32);
	target.y = actor->y.fixed;
	target.z = actor->z.fixed;
	scTunnelRuinsHopLeaderTo(&target);
}

/* Walking into one of the three small white-topped pillars on the floor */
/* below the ivy's ledge pushes it a cell. */
static void scTunnelRuinsPushSmallPillars(void) {
	fdEventStart();
	fdMapGridCopyCellAttributes(89, 49, 3, 2, 25, 49);
	fdMapGridCopyCellAttributes(89, 51, 8, 5, 25, 51);
	scPushedActorAdvancePair();
	scTunnelRuinsUpdateSmallPillarCells();
	fdEventEnd();
}

/* Walking south on the floor of the small pillars makes Isaac leap over a gap
 */
/* if there is one, and push the small pillar in front of him otherwise. */
static void scTunnelRuinsHopSouthOrPushSmallPillars(void) {
	ACTOR *actor = fdActorGet(ACTOR_PARTY_LEADER);
	FIELD_POS target;

	target.x = actor->x.fixed;
	target.y = actor->y.fixed;
	target.z = actor->z.fixed + toFIXED(32);
	if(scTunnelRuinsHopLeaderTo(&target) != 0) {
		scTunnelRuinsPushSmallPillars();
	}
}

/* Whirlwind on the west half of the clump of ivy on the ledge, in the */
/* north-west corner of the small pillars' room, blows its leaves away. */
/* */
/* Once the east half has gone too, the small pillar hidden in the clump can */
/* be reached with Move. Unlike the doorway's ivy, the clump is kept only in a
 */
/* scene flag, not for good. */
static void scTunnelRuinsBlowAwayIvyA(void) {
	gmFlagSet(FLAG_TUNNEL_RUINS_IVY_WEST_HALF_GONE);
	if(gmFlagGet(FLAG_TUNNEL_RUINS_IVY_EAST_HALF_GONE) != 0) {
		fdActorGet(ACTOR_SMALL_PILLAR_C)->sceneCnt = 0;
		fdActorGet(ACTOR_SMALL_PILLAR_C)->collisionFlg &= ~ACTOR_NOT_TARGETED;
	}
	else {
		fdActorGet(ACTOR_SMALL_PILLAR_C)->sceneCnt = 1;
		fdActorGet(ACTOR_SMALL_PILLAR_C)->collisionFlg |= ACTOR_NOT_TARGETED;
	}
}

/* Whirlwind on the east half of the clump of ivy on the ledge blows its */
/* leaves away, and once the west half has gone too, the small pillar hidden */
/* in the clump can be reached with Move. */
static void scTunnelRuinsBlowAwayIvyB(void) {
	gmFlagSet(FLAG_TUNNEL_RUINS_IVY_EAST_HALF_GONE);
	if(gmFlagGet(FLAG_TUNNEL_RUINS_IVY_WEST_HALF_GONE) != 0) {
		fdActorGet(ACTOR_SMALL_PILLAR_C)->sceneCnt = 0;
		fdActorGet(ACTOR_SMALL_PILLAR_C)->collisionFlg &= ~ACTOR_NOT_TARGETED;
	}
	else {
		fdActorGet(ACTOR_SMALL_PILLAR_C)->sceneCnt = 1;
		fdActorGet(ACTOR_SMALL_PILLAR_C)->collisionFlg |= ACTOR_NOT_TARGETED;
	}
}

/* Whirlwind on the ivy filling the arched doorway in the east wall of the */
/* small pillars' room blows it away, opening the way to the long passage. */
/* */
/* Unlike the clump on the ledge, this ivy stays gone for good */
/* (FLAG_TUNNEL_RUINS_IVY_BLOWN_AWAY). */
static void scTunnelRuinsBlowAwayIvyC(void) {
	gmFlagSet(FLAG_TUNNEL_RUINS_IVY_BLOWN_AWAY);
}

/* Walking west from a small pillar's top makes Isaac leap over the gap */
/* beside it, as scTunnelRuinsHopWest does, except from the ledge's cell */
/* (32, 50) beside the top of the stairs. */
static void scTunnelRuinsHopWestBySmallPillars(void) {
	ACTOR *actor = fdActorGet(ACTOR_PARTY_LEADER);

	if(FIELD_CELL(actor->x.fixed) != 32 || (FIELD_CELL(actor->z.fixed)) != 50) {
		scTunnelRuinsHopWest();
	}
}

/* Walking north from a small pillar's top makes Isaac leap over the gap */
/* before it, as scTunnelRuinsHopNorth does, except from cell (30, 52), where */
/* the first small pillar starts. */
static void scTunnelRuinsHopNorthBySmallPillars(void) {
	ACTOR *actor = fdActorGet(ACTOR_PARTY_LEADER);

	if(FIELD_CELL(actor->x.fixed) != 30 || (FIELD_CELL(actor->z.fixed)) != 52) {
		scTunnelRuinsHopNorth();
	}
}

/* The stepping stone in the pit rises one stage with a thump, and the screen */
/* shakes up and down, dying away. */
static inline void scTunnelRuinsRaiseSteppingStone(s32 sourceColumn, s32 column,
												   s32 row) {
	sySndPlay(SE_THUMP);
	fdMapGridCopyCellsTo(sourceColumn, 8, column, row, 1, 3);
	fdMapGridSetShake(0, 5, 1);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
}

/* The map column of the water crest each sinking pillar sinks onto: cell */
/* (46, 8) on the north walkway, cell (48, 14) on the south one. */
#define NORTHWEST_PILLAR_CREST_COLUMN 46
#define SOUTHEAST_PILLAR_CREST_COLUMN 48
/* How far a sinking pillar sinks into the floor, in pixels. */
#define PILLAR_SINK_DEPTH 16

/* When the water-filled pillar on the north walkway of the sinking pillars' */
/* room has been pushed or moved onto the water crest at cell (46, 8), it sinks
 */
/* into the floor in a cloud of dust. With two thumps that shake the room, a */
/* stepping stone bearing the same crest rises out of the pit east of the */
/* walkway (cell (49, 8)), and Isaac can hop across to it. */
/* */
/* The pillar sinks a pixel every three frames, 16 in all. Its top is left */
/* flush with the floor, a square of water where Frost's hidden target now */
/* lies. */
static void scTunnelRuinsSinkNorthwestPillar(void) {
	u32 i;
	ACTOR *overlay;
	FIXED effectX;
	FIXED effectZ;
	EFFECT_OPTIONS options;

	fdEventStart();
	if(FIELD_CELL(fdActorGet(ACTOR_NORTHWEST_SINKING_PILLAR)->x.fixed) ==
	   NORTHWEST_PILLAR_CREST_COLUMN) {
		fdEventWait(30);
		overlay = fdActorPrepare(744, 0, 184, SPRITE_CAPPED_PILLAR);
		/* The pillar sinks a pixel every three frames, raising dust around it.
		 */
		options.startScaleX = 3 * FIXED_ONE / 5;
		options.startScaleY = 3 * FIXED_ONE / 5;
		options.pal = SPRITE_COLOR_WHITE;
		fdActorGet(ACTOR_NORTHWEST_SINKING_PILLAR)->motionFlg = 0;
		sySndPlay(SE_PUSH);
		for(i = 0; i < PILLAR_SINK_DEPTH; i++) {
			sySynch(3);
			fdActorGet(ACTOR_NORTHWEST_SINKING_PILLAR)->y.fixed -= toFIXED(1);
			effectX = INTtoFIXED(syRandomBelow(16)) + toFIXED(736);
			effectZ = INTtoFIXED(syRandomBelow(18)) + toFIXED(128);
			fdEffectSpawn(effectX, 0, effectZ, 0, 0, 0,
						  EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE,
						  &options);
		}
		fdMapGridCopyCellAttributes(51, 8, 1, 1, 49, 8);
		fdEventWait(30);
		fdActorGet(ACTOR_NORTHWEST_SINKING_PILLAR)->priFlg |=
			ACTOR_PRIORITY_UNDERFOOT;
		fdActorSetAnimation(ACTOR_NORTHWEST_SINKING_PILLAR, 3);
		fdActorRelease(overlay);
		fdMapGridCopyCellAttributes(45, 4, 1, 1, 46, 8);
		fdActorSetPos(ACTOR_NORTHWEST_PILLAR_FROST_TARGET, 744, 136);
		scTunnelRuinsRaiseSteppingStone(58, 49, 8);
		fdEventWait(20);
		scTunnelRuinsRaiseSteppingStone(59, 49, 8);
		fdMapGridWaitForShakeToSettle();
		fdEventWait(10);
		gmFlagSet(FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_IN_FLOOR);
	}
	fdEventEnd();
}

/* Walking into the water-filled pillar on the north walkway of the sinking */
/* pillars' room pushes it a cell, and it sinks if that brings it onto its */
/* water crest. Once it has sunk it no longer moves */
/* (FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_IN_FLOOR). */
static void scTunnelRuinsPushNorthwestSinkingPillar(void) {
	fdEventStart();
	scPushedActorAdvancePair();
	fdEventEnd();
	scTunnelRuinsSinkNorthwestPillar();
}

/* Frost on the water of the sunken pillar on the north walkway raises a */
/* pillar of ice from it. */
static void scTunnelRuinsFreezeNorthwestPillarFrostTarget(void) {
	fdActorSetAnimation(ACTOR_NORTHWEST_PILLAR_FROST_TARGET,
						PUDDLE_ANIMATION_PUDDLE);
	fdActorSetSprColById(ACTOR_NORTHWEST_PILLAR_FROST_TARGET,
						 SPRITE_COLOR_NORMAL);
	fdActorSetAnimation(ACTOR_NORTHWEST_PILLAR_FROST_TARGET,
						PUDDLE_ANIMATION_FREEZE);
	fdActorGet(ACTOR_NORTHWEST_PILLAR_FROST_TARGET)->priFlg &=
		~ACTOR_PRIORITY_UNDERFOOT;
	gmFlagSet(FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_FROZEN);
}

/* When the grey pillar on the south walkway of the sinking pillars' room */
/* stands on the water crest at cell (48, 14) and Douse has filled it with */
/* water, it sinks into the floor as the north walkway's pillar does. A */
/* stepping stone rises out of the pit west of the walkway (cell (45, 14)). */
/* */
/* Unwatered, it stands on the crest and nothing happens. */
static void scTunnelRuinsSinkSoutheastPillar(void) {
	u32 i;
	ACTOR *overlay;
	FIXED effectX;
	FIXED effectZ;
	EFFECT_OPTIONS options;

	fdEventStart();
	if(FIELD_CELL(fdActorGet(ACTOR_SOUTHEAST_SINKING_PILLAR)->x.fixed) ==
	   SOUTHEAST_PILLAR_CREST_COLUMN) {
		if(gmFlagGet(FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_SOAKED) != 0) {
			fdEventWait(30);
			overlay = fdActorPrepare(770, 0, 274, SPRITE_CRATE);
			options.startScaleX = 3 * FIXED_ONE / 5;
			options.startScaleY = 3 * FIXED_ONE / 5;
			options.pal = SPRITE_COLOR_WHITE;
			fdActorGet(ACTOR_SOUTHEAST_SINKING_PILLAR)->motionFlg = 0;
			sySndPlay(SE_PUSH);
			for(i = 0; i < PILLAR_SINK_DEPTH; i++) {
				sySynch(3);
				fdActorGet(ACTOR_SOUTHEAST_SINKING_PILLAR)->y.fixed -=
					toFIXED(1);
				effectX = INTtoFIXED(syRandomBelow(16)) + toFIXED(768);
				effectZ = INTtoFIXED(syRandomBelow(18)) + toFIXED(224);
				fdEffectSpawn(effectX, 0, effectZ, 0, 0, 0,
							  EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE,
							  &options);
			}
			fdMapGridCopyCellAttributes(51, 8, 1, 1, 45, 14);
			fdEventWait(30);
			fdActorGet(ACTOR_SOUTHEAST_SINKING_PILLAR)->priFlg |=
				ACTOR_PRIORITY_UNDERFOOT;
			fdActorSetAnimation(ACTOR_SOUTHEAST_SINKING_PILLAR, 3);
			fdActorRelease(overlay);
			fdMapGridCopyCellAttributes(45, 4, 1, 1, 48, 14);
			fdActorSetPos(ACTOR_SOUTHEAST_PILLAR_FROST_TARGET, 776, 232);
			scTunnelRuinsRaiseSteppingStone(58, 45, 14);
			fdEventWait(20);
			scTunnelRuinsRaiseSteppingStone(59, 45, 14);
			fdMapGridWaitForShakeToSettle();
			fdEventWait(10);
			gmFlagSet(FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_IN_FLOOR);
		}
	}
	fdEventEnd();
}

/* Walking into the grey pillar on the south walkway of the sinking pillars' */
/* room pushes it a cell, and it sinks if it is full of water and that brings */
/* it onto its crest. */
static void scTunnelRuinsPushSoutheastSinkingPillar(void) {
	fdEventStart();
	scPushedActorAdvancePair();
	fdEventEnd();
	scTunnelRuinsSinkSoutheastPillar();
}

/* As Douse rains on the grey pillar on the south walkway, water splashes on */
/* its top. */
static void scTunnelRuinsDouseStartsOnSoutheastPillar(void) {
	fdActorSetScript(ACTOR_SOUTHEAST_SINKING_PILLAR,
					 TunnelRuins_DouseSplashScript);
}

/* Douse fills the grey pillar on the south walkway with water, and it sinks */
/* at once if it already stands on its crest. */
static void scTunnelRuinsDouseSoutheastPillar(void) {
	gmFlagSet(FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_SOAKED);
	scTunnelRuinsSinkSoutheastPillar();
}

/* Frost on the water of the sunken pillar on the south walkway raises a */
/* pillar of ice from it. */
static void scTunnelRuinsFreezeSoutheastPillarFrostTarget(void) {
	fdActorSetAnimation(ACTOR_SOUTHEAST_PILLAR_FROST_TARGET,
						PUDDLE_ANIMATION_PUDDLE);
	fdActorSetSprColById(ACTOR_SOUTHEAST_PILLAR_FROST_TARGET,
						 SPRITE_COLOR_NORMAL);
	fdActorSetAnimation(ACTOR_SOUTHEAST_PILLAR_FROST_TARGET,
						PUDDLE_ANIMATION_FREEZE);
	fdActorGet(ACTOR_SOUTHEAST_PILLAR_FROST_TARGET)->priFlg &=
		~ACTOR_PRIORITY_UNDERFOOT;
	gmFlagSet(FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_FROZEN);
}

/* Searching the giant pink statue in the first room of the Tunnel Ruins, from
 */
/* cell (12, 7) before it: "The statue speaks to Isaac's soul..." It bids */
/* those who seek to climb Venus Lighthouse to "seek the lady's leave", and */
/* says the lady will keep safe at her feet the place they seek. */
/* */
/* Once the great statue in Venus Lighthouse has opened a path, it says */
/* instead that the lady knows their desires: "Here, your path shall not be */
/* opened." */
static void scTunnelRuinsSearchGiantStatue(void) {
	fdEventStart();
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	if(gmFlagGet(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH) != 0 ||
	   gmFlagGet(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH) != 0) {
		mnFieldMessageShow(MSGNO(MsgTunnelRuinsStatueSpeaksAfterAnswer), 1);
	}
	else {
		mnFieldMessageShow(MSGNO(MsgTunnelRuinsStatueSpeaksIsaacSoulYe), 1);
	}
	fdEventEnd();
}

/* Nothing more happens when Move pushes the grey pillar in the Douse pillar's
 */
/* room: the push is all there is. Its trigger stops once the pillar of ice */
/* stands below it. */
static void scTunnelRuinsMoveDousePillar(void) {
	fdEventStart();
	fdEventEnd();
}

/* Walking into the grey pillar in the Douse pillar's room pushes it a cell, */
/* across its floor (cells (23, 10) to (27, 13)). */
static void scTunnelRuinsPushDousePillar(void) {
	fdEventStart();
	scPushedActorAdvancePair();
	fdEventEnd();
	scTunnelRuinsMoveDousePillar();
}

/* Douse on the grey pillar in the Douse pillar's room fills it with water, */
/* which splashes on its top as the rain falls. */
/* */
/* From then on its hidden Frost target follows it */
/* (scTunnelRuinsFrostTargetFollowsPillar). */
static void scTunnelRuinsDouseStartsOnPillar(void) {
	fdActorSetScript(ACTOR_DOUSE_PILLAR, TunnelRuins_DouseSplashScript);
	gmFlagSet(FLAG_TUNNEL_RUINS_DOUSE_PILLAR_SOAKED);
	fdActorGet(ACTOR_DOUSE_PILLAR_FROST_TARGET)->update =
		(void (*)(ACTOR *))scTunnelRuinsFrostTargetFollowsPillar;
}

/* Isaac casts Frost on the grey pillar in the Douse pillar's room, once Douse
 */
/* has filled its top with water and it stands in map row 10: snowflakes */
/* swirl, and a tall column of ice rises from the pillar's top, standing up */
/* the screen over the pit's edge just north of it, where Isaac stands, so */
/* that only his head and shoulders show above it. Its target stands 32 */
/* pixels down in the pit at the pillar's spot, and its cell takes the */
/* attributes of map cell (26, 8), a square Isaac can hop onto: the way on to */
/* the next room. Frost cast before Douse has soaked */
/* the pillar finds no target, and nothing happens but the cast. */
static void scTunnelRuinsFreezeDousePillarFrostTarget(void) {
	s32 column;
	s32 row;

	fdEventStart();
	fdActorSetSprPriority(ACTOR_DOUSE_PILLAR_FROST_TARGET, 1);
	fdActorSetAnimation(ACTOR_DOUSE_PILLAR_FROST_TARGET,
						PUDDLE_ANIMATION_PUDDLE);
	fdActorSetSprColById(ACTOR_DOUSE_PILLAR_FROST_TARGET, SPRITE_COLOR_NORMAL);
	fdActorSetAnimation(ACTOR_DOUSE_PILLAR_FROST_TARGET,
						PUDDLE_ANIMATION_FREEZE);

	fdActorGet(ACTOR_DOUSE_PILLAR_FROST_TARGET)->priFlg &=
		~ACTOR_PRIORITY_UNDERFOOT;

	gmFlagSet(FLAG_TUNNEL_RUINS_DOUSE_FROST_TARGET_FROZEN);

	column = fdActorGet(ACTOR_DOUSE_PILLAR_FROST_TARGET)->x.fixed;
	row = FIELD_CELL(fdActorGet(ACTOR_DOUSE_PILLAR_FROST_TARGET)->z.fixed);
	fdMapGridCopyCellAttributes(26, 8, 1, 1, FIELD_CELL(column), row);

	fdActorGet(ACTOR_DOUSE_PILLAR_FROST_TARGET)->update =
		UPDATE_BY_RELATIVE_DEPTH;

	fdActorGet(ACTOR_DOUSE_PILLAR)->update = UPDATE_BY_RELATIVE_DEPTH;

	fdEventEnd();
}

/* A carved stone set on the balance rides up and down with the platform or */
/* stone it rests on, 16 pixels above it. Its timer holds that actor. */
static UNREAD_RESULT scTunnelRuinsRideStoneBelow(ACTOR *supportedActor) {
	ACTOR *supportingActor = fdActorGet(supportedActor->tmr);

	supportedActor->y.fixed = supportingActor->y.fixed + toFIXED(16);
	RETURN_UNREAD(0);
}

/* The balance platforms move to the heights their weights give them (their */
/* timers index Balance_PlatformHeights), with sounds unless frames is 0. */
/* */
/* A platform, or the top stone on one, that comes to rest just below the */
/* floor's height becomes a square Isaac can step onto and leap from, east or */
/* west (CELL_BEHAVIOR_TUNNEL_RUINS_RISEN_PLATFORM). */
static void scTunnelRuinsSettleBalance(s32 frames) {
	u32 i;

	fdActorSetSpeed(ACTOR_BALANCE_WEST, 0.5);
	fdActorSetSpeed(ACTOR_BALANCE_EAST, 0.5);
	if(frames != 0) {
		sySndPlay(SE_SETTLE);
	}
	syFieldActorSetMoveTargetFX(
		fdActorGet(ACTOR_BALANCE_WEST), fdActorGet(ACTOR_BALANCE_WEST)->x.fixed,
		Balance_PlatformHeights[fdActorGet(ACTOR_BALANCE_WEST)->tmr],
		fdActorGet(ACTOR_BALANCE_WEST)->z.fixed);
	syFieldActorSetMoveTargetFX(
		fdActorGet(ACTOR_BALANCE_EAST), fdActorGet(ACTOR_BALANCE_EAST)->x.fixed,
		Balance_PlatformHeights[fdActorGet(ACTOR_BALANCE_EAST)->tmr],
		fdActorGet(ACTOR_BALANCE_EAST)->z.fixed);
	fdActorWaitForMoveById(ACTOR_BALANCE_WEST);
	fdActorWaitForMoveById(ACTOR_BALANCE_EAST);
	fdActorGet(ACTOR_BALANCE_WEST)->y.fixed =
		Balance_PlatformHeights[fdActorGet(ACTOR_BALANCE_WEST)->tmr];
	fdActorGet(ACTOR_BALANCE_EAST)->y.fixed =
		Balance_PlatformHeights[fdActorGet(ACTOR_BALANCE_EAST)->tmr];
	if(frames != 0) {
		sySndPlay(SOUND_FADE_OUT_ACTION_SE);
	}
	for(i = 0; i < 5; i++) {
		if(fdActorGet(i + ACTOR_BALANCE_WEST)->y.fixed / FIXED_ONE < 0 &&
		   fdActorGet(i + ACTOR_BALANCE_WEST)->y.fixed / FIXED_ONE > -30) {
			fdMapGridCopyCellAttributeRect(
				4, 9, 1, 1,
				FIELD_CELL(fdActorGet(i + ACTOR_BALANCE_WEST)->x.fixed),
				FIELD_CELL(fdActorGet(i + ACTOR_BALANCE_WEST)->z.fixed));
		}
	}
	fdEventWait(frames);
}

/* A carried stone drops onto the highest platform or carved stone in its cell
 */
/* and lands with a thud and a ring of dust. */
static void scTunnelRuinsDropStoneOnStack(s32 actorId) {
	FIXED highestHeight = -toFIXED(80);
	u32 i;

	for(i = 0; i <= 4; i++) {
		s32 candidateId = i + ACTOR_BALANCE_WEST;

		if(candidateId == actorId)
			continue;

		if(FIELD_CELL(fdActorGet(candidateId)->x.fixed) !=
		   (FIELD_CELL(fdActorGet(actorId)->x.fixed)))
			continue;
		if(FIELD_CELL(fdActorGet(candidateId)->z.fixed) !=
		   (FIELD_CELL(fdActorGet(actorId)->z.fixed)))
			continue;

		if(highestHeight > fdActorGet(candidateId)->y.fixed + toFIXED(16))
			continue;

		highestHeight = fdActorGet(candidateId)->y.fixed + toFIXED(16);
		fdActorGet(actorId)->tmr = candidateId;
	}

	fdActorSetSpeed(actorId, 4);

	{
		ACTOR *actor = fdActorGet(actorId);
		ACTOR *sourceActor = fdActorGet(actorId);
		FIXED z = fdActorGet(actorId)->z.fixed;

		syFieldActorSetMoveTargetFX(actor, sourceActor->x.fixed, highestHeight,
									z);
	}

	fdActorWaitForMoveById(actorId);
	sySndPlay(SE_THUMP);
	scFieldEffectSpawnNineRadialEffects(actorId);
	fdEventWait(30);
}

/* Carry on one of the three carved stones north of the balance lifts it, and */
/* where Isaac sets it down decides what happens. */
/* */
/* Set down over one of the balance's two white platforms (map row 9), the */
/* camera turns to the balance, the stone drops onto the platform or onto the */
/* stone already there with a thud and a ring of dust, and that platform sinks
 */
/* a step while the other rises: two stones on the east platform raise the */
/* west one high enough to step onto. Set down where it falls below the */
/* floor, the stone drops out of sight and is gone for this visit. */
/* */
/* The stones look alike, so the one set down swaps places with the first */
/* stone not yet set, and the stones are always counted set from the first. */
static void scTunnelRuinsSetStoneOnBalance(void) {
	ACTOR savedPosition;
	s32 i;
	s32 j;
	s32 settledIndex;

	fdEventStart();
	for(i = 0; i <= 2; i++) {
		if(fdActorGet(i + ACTOR_WEST_CARRY_STONE)->spr->image.pri == 3 &&
		   fdEventGetFlag(i + FLAG_TUNNEL_RUINS_FIRST_STONE_SET) == 0) {
			scTunnelRuinsWaitForActorToLand(
				fdActorGet(i + ACTOR_WEST_CARRY_STONE));
			fdActorSetPos(i + ACTOR_WEST_CARRY_STONE, 0, 0);
			gmFlagSet(i + FLAG_TUNNEL_RUINS_FIRST_STONE_SET);
			break;
		}
		if(FIELD_CELL(fdActorGet(i + ACTOR_WEST_CARRY_STONE)->z.fixed) == 9 &&
		   gmFlagGet(i + FLAG_TUNNEL_RUINS_FIRST_STONE_SET) == 0) {
			fdActorGet(i + ACTOR_WEST_CARRY_STONE)->groundHeight = 0;
			fdActorGet(i + ACTOR_WEST_CARRY_STONE)->velY = 0;
			fdActorGet(i + ACTOR_WEST_CARRY_STONE)->tgtY = ACTOR_NO_TARGET;
			fdActorGet(i + ACTOR_WEST_CARRY_STONE)->motionFlg = 0;
			fdActorGet(i + ACTOR_WEST_CARRY_STONE)->tmr = 0;
			/* The carved stone set down takes the place of the first carved
			 * stone not */
			/* yet set. */
			settledIndex = i;
			for(j = 0; j < i; j++) {
				if(gmFlagGet(FLAG_TUNNEL_RUINS_FIRST_STONE_SET + j) == 0) {
					savedPosition.x.fixed =
						fdActorGet(i + ACTOR_WEST_CARRY_STONE)->x.fixed;
					savedPosition.y.fixed =
						fdActorGet(i + ACTOR_WEST_CARRY_STONE)->y.fixed;
					savedPosition.z.fixed =
						fdActorGet(i + ACTOR_WEST_CARRY_STONE)->z.fixed;
					fdActorGet(i + ACTOR_WEST_CARRY_STONE)->x.fixed =
						fdActorGet(j + ACTOR_WEST_CARRY_STONE)->x.fixed;
					fdActorGet(i + ACTOR_WEST_CARRY_STONE)->y.fixed =
						fdActorGet(j + ACTOR_WEST_CARRY_STONE)->y.fixed;
					fdActorGet(i + ACTOR_WEST_CARRY_STONE)->z.fixed =
						fdActorGet(j + ACTOR_WEST_CARRY_STONE)->z.fixed;
					fdActorGet(j + ACTOR_WEST_CARRY_STONE)->x.fixed =
						savedPosition.x.fixed;
					fdActorGet(j + ACTOR_WEST_CARRY_STONE)->y.fixed =
						savedPosition.y.fixed;
					fdActorGet(j + ACTOR_WEST_CARRY_STONE)->z.fixed =
						savedPosition.z.fixed;
					settledIndex = j;
					break;
				}
			}
			fdActorGet(settledIndex + ACTOR_WEST_CARRY_STONE)->groundHeight = 0;
			fdActorGet(settledIndex + ACTOR_WEST_CARRY_STONE)->velY = 0;
			fdActorGet(settledIndex + ACTOR_WEST_CARRY_STONE)->tgtY =
				ACTOR_NO_TARGET;
			fdActorGet(settledIndex + ACTOR_WEST_CARRY_STONE)->motionFlg = 0;
			fdActorGet(settledIndex + ACTOR_WEST_CARRY_STONE)->tmr = 0;
			/* The camera turns to the balance as the carved stone drops and the
			 */
			/* platforms move. */
			fdCameraSetSpeed(3);
			fdCameraGetActor()->motionFlg = 0;
			fdCameraMoveTo(168, 8, 184, 1);
			fdCameraWaitForMove();
			scTunnelRuinsDropStoneOnStack(settledIndex +
										  ACTOR_WEST_CARRY_STONE);
			if(FIELD_CELL(fdActorGet(settledIndex + ACTOR_WEST_CARRY_STONE)
							  ->x.fixed) == 8) {
				fdActorGet(ACTOR_BALANCE_WEST)->tmr++;
				(fdActorGet(ACTOR_BALANCE_EAST)->tmr)--;
			}
			else {
				(fdActorGet(ACTOR_BALANCE_WEST)->tmr)--;
				fdActorGet(ACTOR_BALANCE_EAST)->tmr++;
			}
			fdActorGet(settledIndex + ACTOR_WEST_CARRY_STONE)->update =
				(void (*)(ACTOR *))scTunnelRuinsRideStoneBelow;
			scTunnelRuinsSettleBalance(40);
			fdActorGet(settledIndex + ACTOR_WEST_CARRY_STONE)->priFlg |=
				ACTOR_PRIORITY_UNDERFOOT;
			gmFlagSet(FLAG_TUNNEL_RUINS_FIRST_STONE_SET + settledIndex);
			break;
		}
	}
	fdEventEnd();
}

/* Walking into one of the three carved stones north of the balance pushes */
/* it a cell. */
static void scTunnelRuinsPushCarvedStone(void) {
	fdEventStart();
	scPushedActorAdvancePair();
	fdEventEnd();
}

/* The map columns Babi Lighthouse's two grey pillars stand in once each is */
/* pushed a cell west of its place. */
#define EAST_PILLAR_A_PUSHED_COLUMN 45
#define EAST_PILLAR_B_PUSHED_COLUMN 46

/* Nothing visible happens after Move shifts one of the two grey pillars in */
/* Babi Lighthouse's basement: the game remembers whether each now stands a */
/* cell west of its place, so that the pillars stay where Isaac left them */
/* when he comes back. */
static void scTunnelRuinsMoveEastPillar(void) {
	fdEventStart();

	if(FIELD_CELL(fdActorGet(ACTOR_EAST_PILLAR_A)->x.fixed) ==
	   EAST_PILLAR_A_PUSHED_COLUMN) {
		gmFlagSet(FLAG_TUNNEL_RUINS_EAST_PILLAR_A_PUSHED_WEST);
	}
	else {
		gmFlagClear(FLAG_TUNNEL_RUINS_EAST_PILLAR_A_PUSHED_WEST);
	}

	if(FIELD_CELL(fdActorGet(ACTOR_EAST_PILLAR_B)->x.fixed) ==
	   EAST_PILLAR_B_PUSHED_COLUMN) {
		gmFlagSet(FLAG_TUNNEL_RUINS_EAST_PILLAR_B_PUSHED_WEST);
	}
	else {
		gmFlagClear(FLAG_TUNNEL_RUINS_EAST_PILLAR_B_PUSHED_WEST);
	}

	scTunnelRuinsUpdateGreyPillarCells();
	fdEventEnd();
}

/* Walking east into one of the two grey pillars in Babi Lighthouse's */
/* basement pushes it a cell, and the game remembers where it stands. */
static void scTunnelRuinsPushEastPillar(void) {
	fdEventStart();
	scPushedActorAdvancePair();
	scTunnelRuinsMoveEastPillar();
	fdEventEnd();
}

/* The two white statues stand over the chests of the Oracle's Robe and the */
/* Asura's Armor, in the rooms at the ends of the two routes, and the chests */
/* are hidden. So it is on entering either room, and again as Reveal's light */
/* fades: a chest can be opened only while Reveal lasts. */
/* */
/* The statues block their cells, and the chests are put away at the map's */
/* corner. */
static void scTunnelRuinsShowVanishingStatues(void) {
	fdMapGridCopyCellAttributes(72, 49, 1, 1, 8, 49);
	fdMapGridCopyCellAttributes(113, 43, 1, 1, 49, 43);
	fdTreasureSpotSetPos(TREASURE_SPOT_ORACLES_ROBE, 0, 0);
	fdTreasureSpotSetPos(TREASURE_SPOT_ASURAS_ARMOR, 0, 0);
	fdActorSetPos(ACTOR_WEST_VANISHING_STATUE, 136, 792);
	fdActorSetPos(ACTOR_EAST_VANISHING_STATUE, 792, 696);
}

/* Reveal makes the white statue in the room vanish, and the chest it stood */
/* on appears: the Oracle's Robe at cell (8, 49) in the west room, the Asura's
 */
/* Armor at cell (49, 43) in the east one. */
static void scTunnelRuinsHideVanishingStatues(void) {
	fdMapGridCopyCellAttributes(8, 113, 1, 1, 8, 49);
	fdMapGridCopyCellAttributes(49, 107, 1, 1, 49, 43);
	fdTreasureSpotSetPos(TREASURE_SPOT_ORACLES_ROBE, -1, -1);
	fdTreasureSpotSetPos(TREASURE_SPOT_ASURAS_ARMOR, -1, -1);
	fdActorSetPos(ACTOR_WEST_VANISHING_STATUE, 0, 0);
	fdActorSetPos(ACTOR_EAST_VANISHING_STATUE, 0, 0);
}

/* The five marks the statues must stand on, west of the door: the corners */
/* and middle of the square of cells (23, 47) to (25, 49). Each corner statue */
/* belongs on the opposite corner, and the yellow statue in the middle. */
enum {
	MARKS_WEST_COLUMN = 23,
	MARKS_MIDDLE_COLUMN,
	MARKS_EAST_COLUMN,
	MARKS_NORTH_ROW = 47,
	MARKS_MIDDLE_ROW,
	MARKS_SOUTH_ROW
};

/* After Move shifts one of the five coloured statues of a woman, the door in */
/* the room's east wall (cell (32, 47)) opens once all five stand on their */
/* marks: red north-west, blue north-east, yellow in the middle, green */
/* south-west and violet south-east. */
/* */
/* The camera slides over to the door and it opens with the door sound; it */
/* shuts again the same way if a statue is moved off its mark afterwards. */
/* Otherwise nothing visible happens. */
static void scTunnelRuinsMoveLadyStatue(void) {
	s32 correctPositions;

	correctPositions = 0;
	fdEventStart();
	fdMapGridCopyCellAttributes(83, 45, 11, 8, 19, 45);
	fdMapGridCopyCellAttributes(
		20, 56, 1, 1, FIELD_CELL(fdActorGet(ACTOR_VIOLET_LADY_STATUE)->x.fixed),
		FIELD_CELL(fdActorGet(ACTOR_VIOLET_LADY_STATUE)->z.fixed));
	fdMapGridCopyCellAttributes(
		20, 56, 1, 1, FIELD_CELL(fdActorGet(ACTOR_GREEN_LADY_STATUE)->x.fixed),
		FIELD_CELL(fdActorGet(ACTOR_GREEN_LADY_STATUE)->z.fixed));
	fdMapGridCopyCellAttributes(
		20, 56, 1, 1, FIELD_CELL(fdActorGet(ACTOR_BLUE_LADY_STATUE)->x.fixed),
		FIELD_CELL(fdActorGet(ACTOR_BLUE_LADY_STATUE)->z.fixed));
	fdMapGridCopyCellAttributes(
		20, 56, 1, 1, FIELD_CELL(fdActorGet(ACTOR_RED_LADY_STATUE)->x.fixed),
		FIELD_CELL(fdActorGet(ACTOR_RED_LADY_STATUE)->z.fixed));
	fdMapGridCopyCellAttributes(
		20, 56, 1, 1, FIELD_CELL(fdActorGet(ACTOR_YELLOW_LADY_STATUE)->x.fixed),
		FIELD_CELL(fdActorGet(ACTOR_YELLOW_LADY_STATUE)->z.fixed));
	if(FIELD_CELL(fdActorGet(ACTOR_VIOLET_LADY_STATUE)->x.fixed) ==
	   MARKS_EAST_COLUMN) {
		if(FIELD_CELL(fdActorGet(ACTOR_VIOLET_LADY_STATUE)->z.fixed) ==
		   MARKS_SOUTH_ROW) {
			correctPositions = 1;
		}
	}
	if(FIELD_CELL(fdActorGet(ACTOR_GREEN_LADY_STATUE)->x.fixed) ==
	   MARKS_WEST_COLUMN) {
		if(FIELD_CELL(fdActorGet(ACTOR_GREEN_LADY_STATUE)->z.fixed) ==
		   MARKS_SOUTH_ROW) {
			correctPositions++;
		}
	}
	if(FIELD_CELL(fdActorGet(ACTOR_BLUE_LADY_STATUE)->x.fixed) ==
	   MARKS_EAST_COLUMN) {
		if(FIELD_CELL(fdActorGet(ACTOR_BLUE_LADY_STATUE)->z.fixed) ==
		   MARKS_NORTH_ROW) {
			correctPositions++;
		}
	}
	if(FIELD_CELL(fdActorGet(ACTOR_RED_LADY_STATUE)->x.fixed) ==
	   MARKS_WEST_COLUMN) {
		if(FIELD_CELL(fdActorGet(ACTOR_RED_LADY_STATUE)->z.fixed) ==
		   MARKS_NORTH_ROW) {
			correctPositions++;
		}
	}
	if(FIELD_CELL(fdActorGet(ACTOR_YELLOW_LADY_STATUE)->x.fixed) ==
	   MARKS_MIDDLE_COLUMN) {
		if(FIELD_CELL(fdActorGet(ACTOR_YELLOW_LADY_STATUE)->z.fixed) ==
		   MARKS_MIDDLE_ROW) {
			correctPositions++;
		}
	}
	/* The door moves only when the arrangement is completed or broken. */
	if(correctPositions == 5) {
		if(fdEventGetFlag(FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN) != 0) {
			fdEventEnd();
			return;
		}
		fdEventWait(20);
		fdCameraSetSpeed(0.8);
		fdCameraMoveTo(472, CAMERA_KEEP, 780, 1);
		fdCameraWaitForMove();
		fdEventWait(30);
		gmFlagSet(FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN);
		sySndPlay(SE_DOOR_OPEN);
		fdMapGridAnimateCells(TunnelRuins_DoorOpenSteps, 32, 46);
		fdMapGridCopyCellAttributes(24, 60, 1, 1, 32, 47);
		fdEventWait(40);
	}
	else {
		if(fdEventGetFlag(FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN) != 0) {
			fdEventWait(20);
			fdCameraSetSpeed(0.8);
			fdCameraMoveTo(472, CAMERA_KEEP, 780, 1);
			fdCameraWaitForMove();
			fdEventWait(30);
			gmFlagClear(FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN);
			sySndPlay(SE_DOOR_CLOSE);
			fdMapGridAnimateCells(TunnelRuins_DoorCloseSteps, 32, 46);
			fdMapGridCopyCellAttributes(31, 47, 1, 1, 32, 47);
			fdEventWait(40);
		}
	}
	fdEventEnd();
}

/* Walking into one of the five coloured statues pushes it a cell, and the */
/* door opens or shuts as moving it with Move would make it. */
static void scTunnelRuinsPushLadyStatue(void) {
	fdEventStart();
	fdMapGridCopyCellAttributes(83, 45, 11, 8, 19, 45);
	scPushedActorAdvancePair();
	scTunnelRuinsMoveLadyStatue();
	fdEventEnd();
}

/* Searching any of the five coloured statues of a woman: "The statue seems */
/* to speak to your soul..." Mind Read then hears where each colour belongs. */
static void scTunnelRuinsSearchLadyStatue(void) {
	fdEventStart();
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	mnFieldMessageShow(MSGNO(MsgTunnelRuinsStatueSeemsSpeakYourSoul), 1);
	fdEventEnd();
}

/* Searching the door in the east wall of the five statues' room, while it is */
/* shut, finds it tightly locked. */
static void scTunnelRuinsSearchLockedDoor(void) {
	fdEventStart();
	mnFieldMessageShow(MSGNO(MsgFieldDoorTightlyLocked), 1);
	fdEventEnd();
}

/* The triggers of TunnelRuinsGreatStatue, or of TunnelRuinsLadyStatues and */
/* any other scene. */
const TRIGGER *scGetTriggers(void) {
	if(MainRoutineStateWork.next == SCENE_TUNNEL_RUINS_GREAT_STATUE) {
		return TunnelRuinsGreatStatueTriggers;
	}
	return TunnelRuinsTriggersOther;
}

/* Nothing visible happens after Move shifts one of the two gold-topped pillars
 */
/* in the room south of the giant statue. The game resets their floor and */
/* makes each pillar's top a hop square, so that Isaac can leap onto it. */
/* */
/* It also runs after a push and on entering the room. The floor's own */
/* behaviours are kept in a spare copy at cells (73, 38) to (77, 43); the hop */
/* square is copied from cell (2, 36). */
static void scTunnelRuinsUpdateFirstPillarCells(void) {
	s32 row;
	s32 columnA;
	s32 columnB;

	fdMapGridCopyCellAttributes(73, 38, 5, 5, 9, 38);
	scActorSwapPossByDepth(ACTOR_FIRST_PILLAR_B, ACTOR_FIRST_PILLAR_A);

	columnA = FIELD_CELL(fdActorGet(ACTOR_FIRST_PILLAR_A)->x.fixed);
	row = FIELD_CELL(fdActorGet(ACTOR_FIRST_PILLAR_A)->z.fixed);
	fdMapGridCopyCellAttributes(2, 36, 1, 1, columnA, row);

	columnB = FIELD_CELL(fdActorGet(ACTOR_FIRST_PILLAR_B)->x.fixed);
	row = FIELD_CELL(fdActorGet(ACTOR_FIRST_PILLAR_B)->z.fixed);
	fdMapGridCopyCellAttributes(2, 36, 1, 1, columnB, row);
}

/* Nothing visible happens after Move shifts one of the second pair of */
/* gold-topped pillars. As for the first pair, the game resets their floor */
/* (kept at cells (92, 30) to (98, 34)) and makes each pillar's top a hop */
/* square. */
static void scTunnelRuinsUpdateSecondPillarCells(void) {
	s32 row;
	s32 columnA;
	s32 columnB;

	fdMapGridCopyCellAttributes(93, 30, 6, 5, 29, 30);
	scActorSwapPossByDepth(ACTOR_SECOND_PILLAR_B, ACTOR_SECOND_PILLAR_A);

	columnA = FIELD_CELL(fdActorGet(ACTOR_SECOND_PILLAR_A)->x.fixed);
	row = FIELD_CELL(fdActorGet(ACTOR_SECOND_PILLAR_A)->z.fixed);
	fdMapGridCopyCellAttributes(2, 36, 1, 1, columnA, row);

	columnB = FIELD_CELL(fdActorGet(ACTOR_SECOND_PILLAR_B)->x.fixed);
	row = FIELD_CELL(fdActorGet(ACTOR_SECOND_PILLAR_B)->z.fixed);
	fdMapGridCopyCellAttributes(2, 36, 1, 1, columnB, row);
}

/* Nothing visible happens after Move shifts one of the three small pillars. */
/* The game resets their floor (kept at cells (89, 49) to (96, 56)) and makes */
/* each pillar's top a square Isaac can leap from in any direction. */
static void scTunnelRuinsUpdateSmallPillarCells(void) {
	fdMapGridCopyCellAttributes(89, 49, 3, 2, 25, 49);
	fdMapGridCopyCellAttributes(89, 51, 8, 5, 25, 51);
	fdActorGet(ACTOR_SMALL_PILLAR_C)->layer = 1;
	fdMapGridCopyCellAttributes(
		22, 52, 1, 1, FIELD_CELL(fdActorGet(ACTOR_SMALL_PILLAR_A)->x.fixed),
		FIELD_CELL(fdActorGet(ACTOR_SMALL_PILLAR_A)->z.fixed));
	fdMapGridCopyCellAttributes(
		22, 52, 1, 1, FIELD_CELL(fdActorGet(ACTOR_SMALL_PILLAR_B)->x.fixed),
		FIELD_CELL(fdActorGet(ACTOR_SMALL_PILLAR_B)->z.fixed));
	fdMapGridCopyCellAttributes(
		22, 52, 1, 1, FIELD_CELL(fdActorGet(ACTOR_SMALL_PILLAR_C)->x.fixed),
		FIELD_CELL(fdActorGet(ACTOR_SMALL_PILLAR_C)->z.fixed));
}

/* Resets the row of Babi Lighthouse's two grey pillars (kept at cells (108, */
/* 19) to (111, 19)) and makes each pillar's cell a wall. */
static void scTunnelRuinsUpdateGreyPillarCells(void) {
	fdMapGridCopyCellAttributes(108, 19, 4, 1, 44, 19);
	scMapGridFillCollisionRectangle(
		0, FIELD_CELL(fdActorGet(ACTOR_EAST_PILLAR_A)->x.fixed),
		FIELD_CELL(fdActorGet(ACTOR_EAST_PILLAR_A)->z.fixed), 1, 1,
		CELL_BEHAVIOR_WALL);
	scMapGridFillCollisionRectangle(
		0, FIELD_CELL(fdActorGet(ACTOR_EAST_PILLAR_B)->x.fixed),
		FIELD_CELL(fdActorGet(ACTOR_EAST_PILLAR_B)->z.fixed), 1, 1,
		CELL_BEHAVIOR_WALL);
}

/* On entering a room, restores the puzzle as it was left, and the path the */
/* statue in Venus Lighthouse opened (FLAG_VENUS_LIGHTHOUSE_FIRST_PATH or */
/* _SECOND_PATH). */
s32 scInit(void) {
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	if(MainRoutineStateWork.next == SCENE_TUNNEL_RUINS_GREAT_STATUE ||
	   MainRoutineStateWork.next == SCENE_TUNNEL_RUINS_LADY_STATUES) {
		fdEffectStartGroundMask(0);
		MainRoutineStateWork.retreatEntrance =
			TUNNEL_RUINS_GREAT_STATUE_FROM_BABI_LIGHTHOUSE_STONE_DOOR;
		MainRoutineStateWork.retreatScene = SCENE_TUNNEL_RUINS_GREAT_STATUE;
	}
	if(MainRoutineStateWork.next == SCENE_TUNNEL_RUINS_GREAT_STATUE) {
		switch(MainRoutineStateWork.entrance) {
		case TUNNEL_RUINS_GREAT_STATUE_FROM_BABI_LIGHTHOUSE_STONE_DOOR:
		case TUNNEL_RUINS_GREAT_STATUE_STATUE_FROM_FIRST_PILLARS:
			/* The round floor lamps before the model of the lighthouse, with
			 * the */
			/* path the great statue in Venus Lighthouse has opened lit: gold
			 * lines */
			/* join the lamps along the first path's route, or along the
			 * second's, as */
			/* scVenusLighthouseDrawFirstPath and _DrawSecondPath light them in
			 * the */
			/* statue's own room. With neither flag the lamps stay dark. */
			if(gmFlagGet(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH)) {
				fdMapGridCopyCellsTo(121, 4, 74, 9, 5, 8);
				fdMapGridCopyCellsTo(18, 83, 9, 73, 3, 2);
				fdMapGridCopyCellsTo(18, 81, 9, 75, 3, 2);
				fdMapGridCopyCellsTo(18, 83, 9, 77, 3, 2);
				fdMapGridCopyCellsTo(18, 83, 9, 79, 3, 2);
				fdMapGridCopyCellsTo(18, 83, 11, 78, 3, 2);
				fdMapGridCopyCellsTo(18, 83, 13, 79, 3, 2);
			}
			else {
				if(!gmFlagGet(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH))
					break;
				fdMapGridCopyCellsTo(121, 13, 74, 9, 5, 8);
				fdMapGridCopyCellsTo(18, 85, 11, 74, 3, 2);
				fdMapGridCopyCellsTo(18, 83, 13, 75, 3, 2);
				fdMapGridCopyCellsTo(18, 85, 11, 76, 3, 2);
				fdMapGridCopyCellsTo(18, 83, 11, 78, 3, 2);
				fdMapGridCopyCellsTo(18, 83, 13, 79, 3, 2);
			}
			break;
		case TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_FROM_STATUE:
		case TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_FROM_COLUMNS_NORTH:
			scTunnelRuinsUpdateFirstPillarCells();
			fdActorGet(ACTOR_FIRST_PILLAR_A)->motionFlg = 0;
			fdActorGet(ACTOR_FIRST_PILLAR_B)->motionFlg = 0;
			fdActorSetSprFlags(fdActorGet(ACTOR_FIRST_PILLAR_A), 0);
			fdActorSetSprFlags(fdActorGet(ACTOR_FIRST_PILLAR_B), 0);
			fdActorGet(ACTOR_FIRST_PILLAR_A)->update = UPDATE_BY_RELATIVE_DEPTH;
			fdActorGet(ACTOR_FIRST_PILLAR_B)->update = UPDATE_BY_RELATIVE_DEPTH;
			syTaskAddFrame(scTunnelRuinsUpdateGroundMaskLeader,
						   TASK_PASS_FRAME);
			break;
		case TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_FIRST_PILLARS:
		case TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_SECOND_PILLARS:
		case TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_SINKING_PILLARS:
			/* Each path the great statue has opened stands one more stone
			 * column in */
			/* this room's pit, a stepping stone across it. */
			if(gmFlagGet(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH))
				fdMapGridCopyCells(23, 17, 1, 2, 30, 8);
			if(gmFlagGet(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH))
				fdMapGridCopyCells(23, 17, 1, 2, 32, 10);
			break;
		case TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_FROM_COLUMNS_NORTH:
		case TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_FROM_WHIRLWIND_ROOM:
			scTunnelRuinsUpdateSecondPillarCells();
			fdActorGet(ACTOR_SECOND_PILLAR_A)->motionFlg = 0;
			fdActorGet(ACTOR_SECOND_PILLAR_B)->motionFlg = 0;
			fdActorSetSprFlags(fdActorGet(ACTOR_SECOND_PILLAR_A), 0);
			fdActorSetSprFlags(fdActorGet(ACTOR_SECOND_PILLAR_B), 0);
			fdActorGet(ACTOR_SECOND_PILLAR_A)->update =
				UPDATE_BY_RELATIVE_DEPTH;
			fdActorGet(ACTOR_SECOND_PILLAR_B)->update =
				UPDATE_BY_RELATIVE_DEPTH;
			syTaskAddFrame(scTunnelRuinsUpdateGroundMaskLeader,
						   TASK_PASS_FRAME);
			break;
		case TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_FROM_COLUMNS_NORTH:
		case TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_FROM_COLUMNS_SOUTH:
			fdActorSetSprFlags(fdActorGet(ACTOR_NORTHWEST_SINKING_PILLAR), 0);
			fdActorSetSprFlags(fdActorGet(ACTOR_SOUTHEAST_SINKING_PILLAR), 0);
			fdActorSetAnimation(ACTOR_NORTHWEST_SINKING_PILLAR, 2);
			fdActorSetSprFlags(fdActorGet(ACTOR_NORTHWEST_PILLAR_FROST_TARGET),
							   0);
			fdActorSetSprFlags(fdActorGet(ACTOR_SOUTHEAST_PILLAR_FROST_TARGET),
							   0);
			fdActorSetSprColById(ACTOR_NORTHWEST_PILLAR_FROST_TARGET,
								 SPRITE_COLOR_HIDDEN);
			fdActorSetSprColById(ACTOR_SOUTHEAST_PILLAR_FROST_TARGET,
								 SPRITE_COLOR_HIDDEN);
			/* A pillar that has sunk lies flush with the floor, a pool of water
			 * on */
			/* its top, with its hidden Frost target on it: the floor's cells
			 * are */
			/* drawn so, the pillar and target put there, and the way past
			 * opened. */
			if(gmFlagGet(FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_IN_FLOOR)) {
				fdMapGridCopyCellsTo(59, 8, 49, 8, 1, 3);
				fdMapGridCopyCellAttributes(51, 8, 1, 1, 49, 8);
				fdActorGet(ACTOR_NORTHWEST_SINKING_PILLAR)->priFlg |=
					ACTOR_PRIORITY_UNDERFOOT;
				fdActorSetAnimation(ACTOR_NORTHWEST_SINKING_PILLAR, 3);
				fdMapGridCopyCellAttributes(45, 4, 1, 1, 46, 8);
				fdActorSetPos(ACTOR_NORTHWEST_SINKING_PILLAR, 744, 136);
				fdActorGet(ACTOR_NORTHWEST_SINKING_PILLAR)->y.fixed =
					-toFIXED(16);
				fdActorSetPos(ACTOR_NORTHWEST_PILLAR_FROST_TARGET, 744, 136);
			}
			if(gmFlagGet(FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_FROZEN)) {
				fdActorSetSprColById(ACTOR_NORTHWEST_PILLAR_FROST_TARGET,
									 SPRITE_COLOR_NORMAL);
				fdActorSetAnimation(ACTOR_NORTHWEST_PILLAR_FROST_TARGET,
									PUDDLE_ANIMATION_PILLAR);
			}
			if(fdEventGetFlag(FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_SOAKED))
				fdActorSetAnimation(ACTOR_SOUTHEAST_SINKING_PILLAR, 2);
			if(gmFlagGet(FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_IN_FLOOR)) {
				fdMapGridCopyCellsTo(59, 8, 45, 14, 1, 3);
				fdMapGridCopyCellAttributes(51, 8, 1, 1, 45, 14);
				fdActorGet(ACTOR_SOUTHEAST_SINKING_PILLAR)->priFlg |=
					ACTOR_PRIORITY_UNDERFOOT;
				fdActorSetAnimation(ACTOR_SOUTHEAST_SINKING_PILLAR, 3);
				fdMapGridCopyCellAttributes(45, 4, 1, 1, 48, 14);
				fdActorSetPos(ACTOR_SOUTHEAST_SINKING_PILLAR, 776, 232);
				fdActorGet(ACTOR_SOUTHEAST_SINKING_PILLAR)->y.fixed =
					-toFIXED(16);
				fdActorSetPos(ACTOR_SOUTHEAST_PILLAR_FROST_TARGET, 776, 232);
				gmFlagSet(FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_SOAKED);
			}
			if(gmFlagGet(FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_FROZEN)) {
				fdActorSetSprColById(ACTOR_SOUTHEAST_PILLAR_FROST_TARGET,
									 SPRITE_COLOR_NORMAL);
				fdActorSetAnimation(ACTOR_SOUTHEAST_PILLAR_FROST_TARGET,
									PUDDLE_ANIMATION_PILLAR);
			}
			break;
		case TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_FROM_SECOND_PILLARS:
		case TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_FROM_LONG_PASSAGE:
			scTunnelRuinsUpdateSmallPillarCells();
			fdActorGet(ACTOR_SMALL_PILLAR_A)->motionFlg = 0;
			fdActorGet(ACTOR_SMALL_PILLAR_B)->motionFlg = 0;
			fdActorSetSprFlags(fdActorGet(ACTOR_IVY_A), 0);
			fdActorSetSprFlags(fdActorGet(ACTOR_IVY_B), 0);
			fdActorSetSprFlags(fdActorGet(ACTOR_IVY_C), 0);
			fdActorSetSprFlags(fdActorGet(ACTOR_SMALL_PILLAR_A), 0);
			fdActorSetSprFlags(fdActorGet(ACTOR_SMALL_PILLAR_B), 0);
			fdActorSetSprFlags(fdActorGet(ACTOR_SMALL_PILLAR_C), 0);
			fdActorGet(ACTOR_SMALL_PILLAR_A)->update = UPDATE_BY_RELATIVE_DEPTH;
			fdActorGet(ACTOR_SMALL_PILLAR_B)->update = UPDATE_BY_RELATIVE_DEPTH;
			fdActorGet(ACTOR_SMALL_PILLAR_C)->update = UPDATE_BY_RELATIVE_DEPTH;
			syTaskAddFrame(scTunnelRuinsUpdateGroundMaskLeader,
						   TASK_PASS_FRAME);
			break;
		}
	}
	else {
		switch(MainRoutineStateWork.entrance) {
		case ENTRANCE_DEFAULT:
			break;
		case TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_WEST:
		case TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_NORTH:
		case TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_SOUTH:
			MainRoutineStateWork.retreatEntrance =
				BABI_LIGHTHOUSE_ENTRY_FROM_ROAD_TO_BABI_LIGHTHOUSE;
			MainRoutineStateWork.retreatScene = SCENE_BABI_LIGHTHOUSE_ENTRY;
			gmFlagClear(FLAG_SHOW_AREA_NAME);
			fdActorSetSprColById(ACTOR_EAST_PILLAR_A, SPRITE_COLOR_GREY);
			fdActorSetSprColById(ACTOR_EAST_PILLAR_B, SPRITE_COLOR_GREY);
			if(gmFlagGet(FLAG_TUNNEL_RUINS_EAST_PILLAR_A_PUSHED_WEST))
				fdActorSetPos(ACTOR_EAST_PILLAR_A, 728, 312);
			if(gmFlagGet(FLAG_TUNNEL_RUINS_EAST_PILLAR_B_PUSHED_WEST))
				fdActorSetPos(ACTOR_EAST_PILLAR_B, 744, 312);
			scTunnelRuinsUpdateGreyPillarCells();
			break;
		case TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_SOUTHWEST:
		case TUNNEL_RUINS_LADY_STATUES_DOUSE_PILLAR_SOUTH:
			fdActorSetSprPriority(ACTOR_DOUSE_PILLAR, 1);
			fdActorGet(ACTOR_DOUSE_PILLAR)->motionFlg = 0;
			fdActorSetSprFlags(fdActorGet(ACTOR_DOUSE_PILLAR), 0);
			fdActorSetSprPriority(ACTOR_DOUSE_PILLAR_FROST_TARGET, 1);
			fdActorSetSprColById(ACTOR_DOUSE_PILLAR_FROST_TARGET,
								 SPRITE_COLOR_HIDDEN);
			fdActorSetSprFlags(fdActorGet(ACTOR_DOUSE_PILLAR_FROST_TARGET), 0);
			fdActorGet(ACTOR_DOUSE_PILLAR_FROST_TARGET)->motionFlg = 0;
			if(!gmFlagGet(FLAG_TUNNEL_RUINS_DOUSE_FROST_TARGET_FROZEN))
				break;
			fdActorSetSprColById(ACTOR_DOUSE_PILLAR_FROST_TARGET,
								 SPRITE_COLOR_NORMAL);
			fdActorSetAnimation(ACTOR_DOUSE_PILLAR_FROST_TARGET,
								PUDDLE_ANIMATION_PILLAR);
			fdMapGridCopyCellAttributeRect(
				26, 8, 1, 1,
				FIELD_CELL(
					fdActorGet(ACTOR_DOUSE_PILLAR_FROST_TARGET)->x.fixed),
				FIELD_CELL(
					fdActorGet(ACTOR_DOUSE_PILLAR_FROST_TARGET)->z.fixed));
			fdActorGet(ACTOR_DOUSE_PILLAR_FROST_TARGET)->update =
				UPDATE_BY_RELATIVE_DEPTH;
			fdActorGet(ACTOR_DOUSE_PILLAR)->update = UPDATE_BY_RELATIVE_DEPTH;
			break;
		case TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_NORTH:
		case TUNNEL_RUINS_LADY_STATUES_BALANCE_SOUTH:
			if(!gmFlagGet(FLAG_KEEP_PARTY_POSITION)) {
				fdActorGet(ACTOR_BALANCE_WEST)->motionFlg = 0;
				fdActorGet(ACTOR_BALANCE_EAST)->motionFlg = 0;
				fdActorGet(ACTOR_BALANCE_WEST)->y.fixed = -toFIXED(48);
				fdActorGet(ACTOR_BALANCE_EAST)->y.fixed = -toFIXED(48);
				fdActorGet(ACTOR_BALANCE_WEST)->priFlg |=
					ACTOR_PRIORITY_UNDERFOOT;
				fdActorGet(ACTOR_BALANCE_EAST)->priFlg |=
					ACTOR_PRIORITY_UNDERFOOT;
				fdActorGet(ACTOR_BALANCE_WEST)->collisionFlg &= ~ACTOR_SOLID;
				fdActorGet(ACTOR_BALANCE_EAST)->collisionFlg &= ~ACTOR_SOLID;
				fdActorGet(ACTOR_BALANCE_WEST)->tmr = 3;
				fdActorGet(ACTOR_BALANCE_EAST)->tmr = 3;
				fdActorSetSprPriority(ACTOR_BALANCE_WEST, 1);
				fdActorSetSprPriority(ACTOR_BALANCE_EAST, 1);
				fdActorGet(ACTOR_WEST_CARRY_STONE)->motionFlg = 0;
				fdActorGet(ACTOR_MIDDLE_CARRY_STONE)->motionFlg = 0;
				fdActorGet(ACTOR_EAST_CARRY_STONE)->motionFlg = 0;
				fdActorSetSprFlags(fdActorGet(ACTOR_WEST_CARRY_STONE), 0);
				fdActorSetSprFlags(fdActorGet(ACTOR_MIDDLE_CARRY_STONE), 0);
				fdActorSetSprFlags(fdActorGet(ACTOR_EAST_CARRY_STONE), 0);
				fdActorGet(ACTOR_WEST_CARRY_STONE)->tmr = 0;
				fdActorGet(ACTOR_MIDDLE_CARRY_STONE)->tmr = 0;
				fdActorGet(ACTOR_EAST_CARRY_STONE)->tmr = 0;
				if(MainRoutineStateWork.entrance !=
				   TUNNEL_RUINS_LADY_STATUES_BALANCE_SOUTH)
					break;
				fdActorGet(ACTOR_BALANCE_WEST)->y.fixed = -toFIXED(32);
				fdActorGet(ACTOR_BALANCE_EAST)->y.fixed = -toFIXED(64);
				fdActorGet(ACTOR_BALANCE_WEST)->tmr = 2;
				fdActorGet(ACTOR_BALANCE_EAST)->tmr = 4;
				fdActorSetPos(ACTOR_WEST_CARRY_STONE, 200, 152);
				fdActorGet(ACTOR_WEST_CARRY_STONE)->tmr = ACTOR_BALANCE_EAST;
				fdActorGet(ACTOR_WEST_CARRY_STONE)->update =
					UPDATE_RIDE_STONE_BELOW;
				fdActorGet(ACTOR_WEST_CARRY_STONE)->priFlg |=
					ACTOR_PRIORITY_UNDERFOOT;
				fdActorSetPos(ACTOR_MIDDLE_CARRY_STONE, 200, 152);
				fdActorGet(ACTOR_MIDDLE_CARRY_STONE)->tmr =
					ACTOR_WEST_CARRY_STONE;
				fdActorGet(ACTOR_MIDDLE_CARRY_STONE)->update =
					UPDATE_RIDE_STONE_BELOW;
				fdActorGet(ACTOR_MIDDLE_CARRY_STONE)->priFlg |=
					ACTOR_PRIORITY_UNDERFOOT;
				fdActorSetPos(ACTOR_EAST_CARRY_STONE, 136, 152);
				fdActorGet(ACTOR_EAST_CARRY_STONE)->tmr = ACTOR_BALANCE_WEST;
				fdActorGet(ACTOR_EAST_CARRY_STONE)->update =
					UPDATE_RIDE_STONE_BELOW;
				fdActorGet(ACTOR_EAST_CARRY_STONE)->priFlg |=
					ACTOR_PRIORITY_UNDERFOOT;
				fdEventWait(2);
				gmFlagSet(FLAG_TUNNEL_RUINS_FIRST_STONE_SET);
				gmFlagSet(FLAG_TUNNEL_RUINS_FIRST_STONE_SET + 1);
				gmFlagSet(FLAG_TUNNEL_RUINS_FIRST_STONE_SET + 2);
			}
			scTunnelRuinsSettleBalance(0);
			break;
		case TUNNEL_RUINS_LADY_STATUES_PATHS_SOUTHWEST:
		case TUNNEL_RUINS_LADY_STATUES_PATHS_NORTHWEST:
		case TUNNEL_RUINS_LADY_STATUES_PATHS_SOUTHEAST:
		case TUNNEL_RUINS_LADY_STATUES_PATHS_NORTHEAST:
			/* Here too: the first path stands a column in the gap west of the
			 */
			/* middle platform, the second in the gap east of it. */
			if(gmFlagGet(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH))
				fdMapGridCopyCells(10, 30, 1, 2, 16, 30);
			if(gmFlagGet(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH))
				fdMapGridCopyCells(10, 30, 1, 2, 22, 30);
			gmFlagSet(FLAG_TUNNEL_RUINS_FOUR_PATHS_ROOM_ENTERED);
			break;
		case TUNNEL_RUINS_LADY_STATUES_REVEAL_STATUE_WEST:
			fdMapGridCopyCellAttributes(8, 49, 1, 1, 8, 113);
			scTunnelRuinsShowVanishingStatues();
			syTaskAddFrame(scTunnelRuinsUpdateGroundMaskLeader,
						   TASK_PASS_FRAME);
			break;
		case TUNNEL_RUINS_LADY_STATUES_FIVE_STATUES_NORTH:
		case TUNNEL_RUINS_LADY_STATUES_FIVE_STATUES_EAST:
			fdEventWait(1);
			/* With the five statues gathered as the door asks, the arch east of
			 */
			/* them is drawn as an open doorway and the statues stand where they
			 */
			/* were left. */
			if(gmFlagGet(FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN)) {
				fdMapGridCopyCells(24, 59, 1, 2, 32, 46);
				fdActorSetPos(ACTOR_VIOLET_LADY_STATUE, 408, 792);
				fdActorSetPos(ACTOR_GREEN_LADY_STATUE, 376, 792);
				fdActorSetPos(ACTOR_BLUE_LADY_STATUE, 408, 760);
				fdActorSetPos(ACTOR_RED_LADY_STATUE, 376, 760);
				fdActorSetPos(ACTOR_YELLOW_LADY_STATUE, 392, 776);
			}
			fdActorGet(ACTOR_VIOLET_LADY_STATUE)->motionFlg &=
				~ACTOR_FOLLOWS_TERRAIN;
			fdActorGet(ACTOR_GREEN_LADY_STATUE)->motionFlg &=
				~ACTOR_FOLLOWS_TERRAIN;
			fdActorGet(ACTOR_BLUE_LADY_STATUE)->motionFlg &=
				~ACTOR_FOLLOWS_TERRAIN;
			fdActorGet(ACTOR_RED_LADY_STATUE)->motionFlg &=
				~ACTOR_FOLLOWS_TERRAIN;
			fdActorGet(ACTOR_YELLOW_LADY_STATUE)->motionFlg &=
				~ACTOR_FOLLOWS_TERRAIN;
			fdActorSetSprColById(ACTOR_VIOLET_LADY_STATUE, SPRITE_COLOR_GBR);
			fdActorSetSprColById(ACTOR_GREEN_LADY_STATUE, SPRITE_COLOR_GRB);
			fdActorSetSprColById(ACTOR_BLUE_LADY_STATUE, SPRITE_COLOR_GBR);
			fdActorSetSprColById(ACTOR_RED_LADY_STATUE, SPRITE_COLOR_RED_TINT);
			fdActorSetSprColById(ACTOR_YELLOW_LADY_STATUE, SPRITE_COLOR_NORMAL);
			fdActorSetAnimation(ACTOR_VIOLET_LADY_STATUE, 2);
			fdActorSetAnimation(ACTOR_YELLOW_LADY_STATUE, 2);
			fdMapGridCopyCellAttributeRect(
				20, 56, 1, 1,
				FIELD_CELL(fdActorGet(ACTOR_VIOLET_LADY_STATUE)->x.fixed),
				FIELD_CELL(fdActorGet(ACTOR_VIOLET_LADY_STATUE)->z.fixed));
			fdMapGridCopyCellAttributeRect(
				20, 56, 1, 1,
				FIELD_CELL(fdActorGet(ACTOR_GREEN_LADY_STATUE)->x.fixed),
				FIELD_CELL(fdActorGet(ACTOR_GREEN_LADY_STATUE)->z.fixed));
			fdMapGridCopyCellAttributeRect(
				20, 56, 1, 1,
				FIELD_CELL(fdActorGet(ACTOR_BLUE_LADY_STATUE)->x.fixed),
				FIELD_CELL(fdActorGet(ACTOR_BLUE_LADY_STATUE)->z.fixed));
			fdMapGridCopyCellAttributeRect(
				20, 56, 1, 1,
				FIELD_CELL(fdActorGet(ACTOR_RED_LADY_STATUE)->x.fixed),
				FIELD_CELL(fdActorGet(ACTOR_RED_LADY_STATUE)->z.fixed));
			fdMapGridCopyCellAttributeRect(
				20, 56, 1, 1,
				FIELD_CELL(fdActorGet(ACTOR_YELLOW_LADY_STATUE)->x.fixed),
				FIELD_CELL(fdActorGet(ACTOR_YELLOW_LADY_STATUE)->z.fixed));
			break;
		case TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_SOUTHEAST:
			fdMapGridCopyCellAttributes(49, 43, 1, 1, 49, 107);
			scTunnelRuinsShowVanishingStatues();
			break;
		}
	}
	return 0;
}

#include "shared/pushed_actor_tables.c"

#include "shared/field_effect_duration_scripts.c"

#include "shared/balance_heights.c"

const s32 *FieldEffect_DurationScripts[] = {
	FieldEffect_ShortScript,
	FieldEffect_MediumScript,
	FieldEffect_LongScript,
};

s32 TunnelRuins_SmallLighthouseScript[] = {
	SC_SET(SW_SCALE_X, 3 * FIXED_ONE / 5),
	SC_SET(SW_SCALE_Y, 3 * FIXED_ONE / 5),
	SC_SET(SW_COLLISION, 0),
	SC_CALL(scTunnelRuinsHideLighthouseShadow),
	SC_END,
};

/* The ivy stands 8/5 as tall as its sprite. */
s32 TunnelRuins_StretchScript[] = {
	SC_SET(SW_SCALE_Y, 8 * FIXED_ONE / 5),
	SC_END,
};

/* Douse splashes on the pillar from frame 35 for 130 frames. */
s32 TunnelRuins_DouseSplashScript[] = {
	SC_WAIT(35),
	SC_SET(SW_HOOK, (s32)scTunnelRuinsSplashDousedPillar),
	SC_WAIT(130),
	SC_SET(SW_HOOK, 0),
	SC_ANIMATION(ANIM_WALK),
	SC_END,
};

/* The east wall's door opening once all five statues stand on their marks, */
/* and shutting again. */
u16 TunnelRuins_DoorOpenSteps[] = {
	MAPGRID_ANIMATION_STEP(28, 59, 1, 2, 4),
	MAPGRID_ANIMATION_STEP(26, 59, 1, 2, 4),
	MAPGRID_ANIMATION_STEP(24, 59, 1, 2, 4),
	MAPGRID_ANIMATION_STEPS_END,
};
u16 TunnelRuins_DoorCloseSteps[] = {
	MAPGRID_ANIMATION_STEP(26, 59, 1, 2, 4),
	MAPGRID_ANIMATION_STEP(28, 59, 1, 2, 4),
	MAPGRID_ANIMATION_STEP(30, 59, 1, 2, 4),
	MAPGRID_ANIMATION_STEPS_END,
};

/* Unused: the entrances of any scene but the two this overlay serves, so no */
/* scene reads them. Entrance 10 would set the party at (440, 552), facing */
/* south. */
SCENE_ENTRANCE TunnelRuinsEntrancesOther[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 744, 0, 104, FACING_SOUTH, 0, -1, -1,
	 -1, -1, 0},
	{10, CONDITION_ALWAYS, 440, 0, 552, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE TunnelRuinsGreatStatueEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 744, 0, 104, FACING_SOUTH, 0, -1, -1,
	 -1, -1, 0},
	{TUNNEL_RUINS_GREAT_STATUE_FROM_BABI_LIGHTHOUSE_STONE_DOOR,
	 CONDITION_ALWAYS, 104, 0, 152, FACING_SOUTH, 0, 48, 48, 288, 368, 0},
	{TUNNEL_RUINS_GREAT_STATUE_STATUE_FROM_FIRST_PILLARS, CONDITION_ALWAYS, 200,
	 0, 328, FACING_NORTH, 0, 48, 48, 288, 368, 0},
	{TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_FROM_STATUE, CONDITION_ALWAYS, 184,
	 0, 552, FACING_SOUTH, 0, 64, 464, 304, 816, 0},
	{TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_FROM_COLUMNS_NORTH,
	 CONDITION_ALWAYS, 168, 0, 816, FACING_NORTH, 0, 64, 464, 304, 816, 0},
	{TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_FIRST_PILLARS,
	 CONDITION_ALWAYS, 520, 0, 104, FACING_SOUTH, 0, 336, 48, 576, 288, 0},
	{TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_SECOND_PILLARS,
	 CONDITION_ALWAYS, 392, 0, 192, FACING_NORTH, 0, 336, 48, 576, 288, 0},
	{TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_SINKING_PILLARS,
	 CONDITION_ALWAYS, 520, 0, 256, FACING_NORTH, 0, 336, 48, 576, 288, 0},
	{TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_FROM_COLUMNS_NORTH,
	 CONDITION_ALWAYS, 568, 0, 440, FACING_SOUTH, 0, 384, 352, 624, 640, 0},
	{TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_FROM_WHIRLWIND_ROOM,
	 CONDITION_ALWAYS, 440, 0, 648, FACING_NORTH, 0, 384, 352, 624, 640, 0},
	{TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_FROM_COLUMNS_NORTH,
	 CONDITION_ALWAYS, 744, 0, 104, FACING_SOUTH, 0, 624, 40, 864, 288, 0},
	{TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_FROM_COLUMNS_SOUTH,
	 CONDITION_ALWAYS, 680, 0, 256, FACING_NORTH, 0, 624, 40, 864, 288, 0},
	{TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_FROM_SECOND_PILLARS,
	 CONDITION_ALWAYS, 504, 0, 792, FACING_SOUTH, 0, 368, 720, 608, 976, 0},
	{TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_FROM_LONG_PASSAGE,
	 CONDITION_ALWAYS, 568, 0, 872, FACING_SOUTH, 0, 368, 720, 608, 976, 0},
	{TUNNEL_RUINS_GREAT_STATUE_LONG_PASSAGE_FROM_WHIRLWIND_ROOM,
	 CONDITION_ALWAYS, 776, 0, 432, FACING_NORTH, 0, 688, 368, 928, 640, 0},
	{TUNNEL_RUINS_GREAT_STATUE_FROM_TUNNEL_RUINS_LADY_STATUES_NORTH,
	 CONDITION_ALWAYS, 840, 0, 608, FACING_NORTH, 0, 688, 368, 928, 640, 0},
	{TUNNEL_RUINS_GREAT_STATUE_COLUMNS_SOUTH_FROM_SINKING_PILLARS,
	 CONDITION_ALWAYS, 744, 0, 792, FACING_SOUTH, 0, 688, 736, 928, 976, 0},
	{TUNNEL_RUINS_GREAT_STATUE_FROM_TUNNEL_RUINS_LADY_STATUES_SOUTHWEST,
	 CONDITION_ALWAYS, 744, 0, 944, FACING_NORTH, 0, 688, 736, 928, 976, 0},
	{TUNNEL_RUINS_GREAT_STATUE_FROM_TUNNEL_RUINS_LADY_STATUES_SOUTHEAST,
	 CONDITION_ALWAYS, 872, 0, 880, FACING_NORTH, 0, 688, 736, 928, 976, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE TunnelRuinsLadyStatuesEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
	 -1, -1, 0},
	{TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_WEST,
	 CONDITION_ALWAYS, 632, 0, 344, FACING_SOUTH, 0, 592, 64, 880, 400, 0},
	{TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_NORTH,
	 CONDITION_ALWAYS, 840, 0, 120, FACING_SOUTH, 0, 592, 64, 880, 400, 0},
	{TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_SOUTH,
	 CONDITION_ALWAYS, 824, 0, 344, FACING_SOUTH, 0, 592, 64, 880, 400, 0},
	{TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_NORTH,
	 CONDITION_ALWAYS, 216, 0, 104, FACING_SOUTH, 0, 32, 48, 272, 320, 0},
	{TUNNEL_RUINS_LADY_STATUES_BALANCE_SOUTH, CONDITION_ALWAYS, 216, 0, 296,
	 FACING_NORTH, 0, 32, 48, 272, 320, 0},
	{TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_SOUTHWEST,
	 CONDITION_ALWAYS, 360, 0, 104, FACING_SOUTH, 0, 304, 48, 544, 352, 0},
	{TUNNEL_RUINS_LADY_STATUES_DOUSE_PILLAR_SOUTH, CONDITION_ALWAYS, 488, 0,
	 328, FACING_NORTH, 0, 304, 48, 544, 352, 0},
	{TUNNEL_RUINS_LADY_STATUES_PATHS_SOUTHWEST, CONDITION_ALWAYS, 120, 0, 568,
	 FACING_NORTH, 0, 80, 400, 448, 592, 0},
	{TUNNEL_RUINS_LADY_STATUES_PATHS_NORTHWEST, CONDITION_ALWAYS, 216, 0, 456,
	 FACING_SOUTH, 0, 80, 400, 448, 592, 0},
	{TUNNEL_RUINS_LADY_STATUES_PATHS_SOUTHEAST, CONDITION_ALWAYS, 312, 0, 568,
	 FACING_NORTH, 0, 80, 400, 448, 592, 0},
	{TUNNEL_RUINS_LADY_STATUES_PATHS_NORTHEAST, CONDITION_ALWAYS, 408, 0, 456,
	 FACING_SOUTH, 0, 80, 400, 448, 592, 0},
	{TUNNEL_RUINS_LADY_STATUES_REVEAL_STATUE_WEST, CONDITION_ALWAYS, 136, 0,
	 712, FACING_SOUTH, 0, 16, 656, 256, 880, 0},
	{TUNNEL_RUINS_LADY_STATUES_FIVE_STATUES_NORTH, CONDITION_ALWAYS, 392, 0,
	 712, FACING_SOUTH, 0, 288, 656, 560, 864, 0},
	{TUNNEL_RUINS_LADY_STATUES_FIVE_STATUES_EAST, CONDITION_ALWAYS, 520, 0, 776,
	 FACING_SOUTH, 0, 288, 656, 560, 864, 0},
	{TUNNEL_RUINS_LADY_STATUES_TREE_DOOR_SOUTH, CONDITION_ALWAYS, 648, 0, 952,
	 FACING_NORTH, 0, 608, 816, 848, 976, 0},
	{TUNNEL_RUINS_LADY_STATUES_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR,
	 CONDITION_ALWAYS, 776, 0, 904, FACING_SOUTH, 0, 608, 816, 848, 976, 0},
	{TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_SOUTHEAST,
	 CONDITION_ALWAYS, 696, 0, 680, FACING_SOUTH, 0, 624, 592, 864, 768, 0},
	{SCENE_TABLE_END},
};

SCENE_EXIT_REGION TunnelRuinsLadyStatuesRegions[] = {
	{624, 16, 336, 640, 32, 352, -1,
	 TUNNEL_RUINS_LADY_STATUES_TO_BABI_LIGHTHOUSE_ENTRY_WEST},
	{816, 16, 336, 832, 32, 352, -1,
	 TUNNEL_RUINS_LADY_STATUES_TO_BABI_LIGHTHOUSE_ENTRY_SOUTH},
	{832, 16, 112, 848, 32, 128, -1,
	 TUNNEL_RUINS_LADY_STATUES_TO_BABI_LIGHTHOUSE_ENTRY_NORTH},
	{-1, 0, 0, 0, 0, 0, 0, 0},
};

u32 TunnelRuinsExits[] = {
	SCENE_EXITS(TunnelRuinsGreatStatue),
	SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_TO_BABI_LIGHTHOUSE_STONE_DOOR,
			   BabiLighthouseStoneDoor,
			   BABI_LIGHTHOUSE_STONE_DOOR_FROM_TUNNEL_RUINS_GREAT_STATUE),
	SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_STATUE_TO_FIRST_PILLARS,
			   TunnelRuinsGreatStatue,
			   TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_FROM_STATUE),
	SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_TO_STATUE,
			   TunnelRuinsGreatStatue,
			   TUNNEL_RUINS_GREAT_STATUE_STATUE_FROM_FIRST_PILLARS),
	SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_TO_COLUMNS_NORTH,
			   TunnelRuinsGreatStatue,
			   TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_FIRST_PILLARS),
	SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_FIRST_PILLARS,
			   TunnelRuinsGreatStatue,
			   TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_FROM_COLUMNS_NORTH),
	SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_SECOND_PILLARS,
			   TunnelRuinsGreatStatue,
			   TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_FROM_COLUMNS_NORTH),
	SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_SINKING_PILLARS,
			   TunnelRuinsGreatStatue,
			   TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_FROM_COLUMNS_NORTH),
	SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_TO_COLUMNS_NORTH,
			   TunnelRuinsGreatStatue,
			   TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_SECOND_PILLARS),
	SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_TO_WHIRLWIND_ROOM,
			   TunnelRuinsGreatStatue,
			   TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_FROM_SECOND_PILLARS),
	SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_TO_COLUMNS_NORTH,
			   TunnelRuinsGreatStatue,
			   TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_SINKING_PILLARS),
	SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_TO_COLUMNS_SOUTH,
			   TunnelRuinsGreatStatue,
			   TUNNEL_RUINS_GREAT_STATUE_COLUMNS_SOUTH_FROM_SINKING_PILLARS),
	SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_TO_SECOND_PILLARS,
			   TunnelRuinsGreatStatue,
			   TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_FROM_WHIRLWIND_ROOM),
	SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_TO_LONG_PASSAGE,
			   TunnelRuinsGreatStatue,
			   TUNNEL_RUINS_GREAT_STATUE_LONG_PASSAGE_FROM_WHIRLWIND_ROOM),
	SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_LONG_PASSAGE_TO_WHIRLWIND_ROOM,
			   TunnelRuinsGreatStatue,
			   TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_FROM_LONG_PASSAGE),
	SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_NORTH,
			   TunnelRuinsLadyStatues,
			   TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_NORTH),
	SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_COLUMNS_SOUTH_TO_SINKING_PILLARS,
			   TunnelRuinsGreatStatue,
			   TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_FROM_COLUMNS_SOUTH),
	SCENE_EXIT(
		TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_SOUTHWEST,
		TunnelRuinsLadyStatues,
		TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_SOUTHWEST),
	SCENE_EXIT(
		TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_SOUTHEAST,
		TunnelRuinsLadyStatues,
		TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_SOUTHEAST),
	SCENE_EXITS(TunnelRuinsLadyStatues),
	SCENE_EXIT(TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_NORTH,
			   TunnelRuinsGreatStatue,
			   TUNNEL_RUINS_GREAT_STATUE_FROM_TUNNEL_RUINS_LADY_STATUES_NORTH),
	SCENE_EXIT(TUNNEL_RUINS_LADY_STATUES_TO_PATHS_NORTHWEST,
			   TunnelRuinsLadyStatues,
			   TUNNEL_RUINS_LADY_STATUES_PATHS_NORTHWEST),
	SCENE_EXIT(
		TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_SOUTHWEST,
		TunnelRuinsGreatStatue,
		TUNNEL_RUINS_GREAT_STATUE_FROM_TUNNEL_RUINS_LADY_STATUES_SOUTHWEST),
	SCENE_EXIT(TUNNEL_RUINS_LADY_STATUES_TO_PATHS_NORTHEAST,
			   TunnelRuinsLadyStatues,
			   TUNNEL_RUINS_LADY_STATUES_PATHS_NORTHEAST),
	SCENE_EXIT(TUNNEL_RUINS_LADY_STATUES_TO_BALANCE_SOUTH,
			   TunnelRuinsLadyStatues, TUNNEL_RUINS_LADY_STATUES_BALANCE_SOUTH),
	SCENE_EXIT(TUNNEL_RUINS_LADY_STATUES_TO_DOUSE_PILLAR_SOUTH,
			   TunnelRuinsLadyStatues,
			   TUNNEL_RUINS_LADY_STATUES_DOUSE_PILLAR_SOUTH),
	SCENE_EXIT(TUNNEL_RUINS_LADY_STATUES_TO_REVEAL_STATUE_WEST,
			   TunnelRuinsLadyStatues,
			   TUNNEL_RUINS_LADY_STATUES_REVEAL_STATUE_WEST),
	SCENE_EXIT(TUNNEL_RUINS_LADY_STATUES_TO_FIVE_STATUES_NORTH,
			   TunnelRuinsLadyStatues,
			   TUNNEL_RUINS_LADY_STATUES_FIVE_STATUES_NORTH),
	SCENE_EXIT(TUNNEL_RUINS_LADY_STATUES_TO_PATHS_SOUTHWEST,
			   TunnelRuinsLadyStatues,
			   TUNNEL_RUINS_LADY_STATUES_PATHS_SOUTHWEST),
	SCENE_EXIT(TUNNEL_RUINS_LADY_STATUES_TO_PATHS_SOUTHEAST,
			   TunnelRuinsLadyStatues,
			   TUNNEL_RUINS_LADY_STATUES_PATHS_SOUTHEAST),
	SCENE_EXIT(TUNNEL_RUINS_LADY_STATUES_TO_TREE_DOOR_SOUTH,
			   TunnelRuinsLadyStatues,
			   TUNNEL_RUINS_LADY_STATUES_TREE_DOOR_SOUTH),
	SCENE_EXIT(
		TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_SOUTHEAST,
		TunnelRuinsGreatStatue,
		TUNNEL_RUINS_GREAT_STATUE_FROM_TUNNEL_RUINS_LADY_STATUES_SOUTHEAST),
	SCENE_EXIT(TUNNEL_RUINS_LADY_STATUES_TO_FIVE_STATUES_EAST,
			   TunnelRuinsLadyStatues,
			   TUNNEL_RUINS_LADY_STATUES_FIVE_STATUES_EAST),
	SCENE_EXIT(TUNNEL_RUINS_LADY_STATUES_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR,
			   VenusLighthouseFirstFloor,
			   VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_TUNNEL_RUINS_LADY_STATUES),
	SCENE_EXIT(TUNNEL_RUINS_LADY_STATUES_TO_BABI_LIGHTHOUSE_ENTRY_WEST,
			   BabiLighthouseEntry,
			   BABI_LIGHTHOUSE_ENTRY_FROM_TUNNEL_RUINS_LADY_STATUES_WEST),
	SCENE_EXIT(TUNNEL_RUINS_LADY_STATUES_TO_BABI_LIGHTHOUSE_ENTRY_SOUTH,
			   BabiLighthouseEntry,
			   BABI_LIGHTHOUSE_ENTRY_FROM_TUNNEL_RUINS_LADY_STATUES_SOUTH),
	SCENE_EXIT(TUNNEL_RUINS_LADY_STATUES_TO_BABI_LIGHTHOUSE_ENTRY_NORTH,
			   BabiLighthouseEntry,
			   BABI_LIGHTHOUSE_ENTRY_FROM_TUNNEL_RUINS_LADY_STATUES_NORTH),
	SCENE_EXITS_END,
};

ACTOR_PLACE TunnelRuinsGreatStatueActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_CAPPED_PILLAR, 0, MOVEMENT_TYPE_STAND, 152, 0, 616,
					FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CAPPED_PILLAR, 0, MOVEMENT_TYPE_STAND, 168, 0, 616,
					FACING_SOUTH, TALK_FACING_KEEP,
					ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_CAPPED_PILLAR, 0, MOVEMENT_TYPE_STAND, 520, 0, 488,
					FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CAPPED_PILLAR, 0, MOVEMENT_TYPE_STAND, 536, 0, 488,
					FACING_SOUTH, TALK_FACING_KEEP,
					ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_SMALL_PILLAR, 0, MOVEMENT_TYPE_STAND, 488, 0, 840,
					FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SMALL_PILLAR, 0, MOVEMENT_TYPE_STAND, 520, 0, 840,
					FACING_SOUTH, TALK_FACING_KEEP,
					ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_SMALL_PILLAR, 0, MOVEMENT_TYPE_STAND, 424, 0, 792,
					FACING_SOUTH, TALK_FACING_KEEP,
					ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_IVY, 0, (s32)TunnelRuins_StretchScript, 416, 0, 794,
					FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_IVY, 0, (s32)TunnelRuins_StretchScript, 432, 0, 794,
					FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_IVY, FLAG_TUNNEL_RUINS_IVY_BLOWN_AWAY,
					(s32)TunnelRuins_StretchScript, 568, 0, 856, FACING_NORTH,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SINKING_PILLAR, 0, MOVEMENT_TYPE_STAND, 680, 0, 136,
					FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SINKING_PILLAR, 0, MOVEMENT_TYPE_STAND, 808, 0, 232,
					FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, 0, MOVEMENT_TYPE_STAND, 728, 0, 184,
					FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, 0, MOVEMENT_TYPE_STAND, 728, 0, 184,
					FACING_NORTH, TALK_FACING_KEEP, 0),
	/* A model of Venus Lighthouse on the floor before the giant statue: the */
	/* World Map's tower, at three fifths of its size, with no collision */
	/* and no shadow. */
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_VENUS_LIGHTHOUSE, 0,
					(s32)TunnelRuins_SmallLighthouseScript, 232, 0, 248,
					FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE TunnelRuinsActorPlacementsOther[] = {
	ACTOR_PLACEMENT(SPRITE_SINKING_PILLAR, 0, MOVEMENT_TYPE_STAND, 392, 0, 168,
					FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, 0, MOVEMENT_TYPE_STAND, 392, 0, 248,
					FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, 0, MOVEMENT_TYPE_RESET_ACTION, 136,
					0, 152, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, 0, MOVEMENT_TYPE_RESET_ACTION, 200,
					0, 152, FACING_NORTH, TALK_FACING_KEEP,
					ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, 0, MOVEMENT_TYPE_STAND, 152, 0, 136,
					FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, 0, MOVEMENT_TYPE_STAND, 184, 0, 136,
					FACING_NORTH, TALK_FACING_KEEP,
					ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, 0, MOVEMENT_TYPE_STAND, 216, 0, 136,
					FACING_NORTH, TALK_FACING_KEEP,
					ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STATUE_WITH_WREATH, 0, MOVEMENT_TYPE_STAND, 136, 0,
					792, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STATUE_WITH_WREATH, 0, MOVEMENT_TYPE_STAND, 792, 0,
					696, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CAPPED_PILLAR, 0, MOVEMENT_TYPE_STAND, 744, 0, 312,
					FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CAPPED_PILLAR, 0, MOVEMENT_TYPE_STAND, 760, 0, 312,
					FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, 0, MOVEMENT_TYPE_STAND, 344, 0, 760,
					FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, 0, MOVEMENT_TYPE_STAND, 440, 0, 760,
					FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, 0, MOVEMENT_TYPE_STAND, 344, 0, 808,
					FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, 0, MOVEMENT_TYPE_STAND, 440, 0, 808,
					FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, 0, MOVEMENT_TYPE_STAND, 392, 0, 824,
					FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

/* clang-format off */
TRIGGER TunnelRuinsGreatStatueTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, TUNNEL_RUINS_GREAT_STATUE_TO_BABI_LIGHTHOUSE_STONE_DOOR, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_TO_BABI_LIGHTHOUSE_STONE_DOOR},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_STATUE_TO_FIRST_PILLARS, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_STATUE_TO_FIRST_PILLARS},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_TO_STATUE, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_TO_STATUE},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_TO_COLUMNS_NORTH, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_TO_COLUMNS_NORTH},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_FIRST_PILLARS, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_FIRST_PILLARS},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_SECOND_PILLARS, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_SECOND_PILLARS},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_SINKING_PILLARS, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_SINKING_PILLARS},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_TO_COLUMNS_NORTH, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_TO_COLUMNS_NORTH},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_TO_WHIRLWIND_ROOM, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_TO_WHIRLWIND_ROOM},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_TO_COLUMNS_NORTH, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_TO_COLUMNS_NORTH},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_TO_COLUMNS_SOUTH, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_TO_COLUMNS_SOUTH},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_TO_SECOND_PILLARS, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_TO_SECOND_PILLARS},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_TO_LONG_PASSAGE, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_TO_LONG_PASSAGE},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_LONG_PASSAGE_TO_WHIRLWIND_ROOM, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_LONG_PASSAGE_TO_WHIRLWIND_ROOM},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_NORTH, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_NORTH},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_COLUMNS_SOUTH_TO_SINKING_PILLARS, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_COLUMNS_SOUTH_TO_SINKING_PILLARS},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_SOUTHWEST, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_SOUTHWEST},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_SOUTHEAST, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_SOUTHEAST},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_TUNNEL_RUINS_FIRST_PILLARS_LEDGE, CONDITION_ALWAYS, EVENT(scTunnelRuinsJumpDownLedge)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_TUNNEL_RUINS_FIRST_PILLARS_LEDGE, CONDITION_ALWAYS,
        EVENT(scTunnelRuinsHopWest)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_TUNNEL_RUINS_FIRST_PILLARS_FLOOR, CONDITION_ALWAYS, EVENT(scTunnelRuinsPushFirstPillars)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_TUNNEL_RUINS_SMALL_PILLARS_STEP, CONDITION_ALWAYS,
        EVENT(scTunnelRuinsHopDownStep)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_TUNNEL_RUINS_SECOND_PILLARS_FLOOR, CONDITION_ALWAYS, EVENT(scTunnelRuinsPushSecondPillars)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_TUNNEL_RUINS_SECOND_PILLARS_LEDGE, CONDITION_ALWAYS, EVENT(scTunnelRuinsJumpDownLedge)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_TUNNEL_RUINS_SECOND_PILLARS_LEDGE, CONDITION_ALWAYS,
        EVENT(scTunnelRuinsHopEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_TUNNEL_RUINS_SMALL_PILLAR_TOP, CONDITION_ALWAYS,
        EVENT(scTunnelRuinsHopWestBySmallPillars)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_TUNNEL_RUINS_SMALL_PILLAR_TOP, CONDITION_ALWAYS,
        EVENT(scTunnelRuinsHopEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_TUNNEL_RUINS_SMALL_PILLAR_TOP, CONDITION_ALWAYS,
        EVENT(scTunnelRuinsHopNorthBySmallPillars)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_TUNNEL_RUINS_SMALL_PILLAR_TOP, CONDITION_ALWAYS,
        EVENT(scTunnelRuinsHopSouth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_TUNNEL_RUINS_SMALL_PILLARS_FLOOR, CONDITION_ALWAYS, EVENT(scTunnelRuinsHopSouthOrPushSmallPillars)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_TUNNEL_RUINS_SMALL_PILLARS_FLOOR, CONDITION_ALWAYS, EVENT(scTunnelRuinsPushSmallPillars)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_TUNNEL_RUINS_NORTH_WALKWAY, FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_IN_FLOOR, EVENT(scTunnelRuinsPushNorthwestSinkingPillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_TUNNEL_RUINS_NORTH_WALKWAY, FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_FROZEN,
        EVENT(scTunnelRuinsHopEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_TUNNEL_RUINS_NORTH_WALKWAY, FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_FROZEN,
        EVENT(scTunnelRuinsHopSouth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_TUNNEL_RUINS_NORTH_WALKWAY, FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_FROZEN,
        EVENT(scTunnelRuinsHopNorth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_TUNNEL_RUINS_NORTH_WALKWAY_END, FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_FROZEN,
        EVENT(scTunnelRuinsHopWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_TUNNEL_RUINS_NORTH_WALKWAY_END, CONDITION_ALWAYS,
        EVENT(scTunnelRuinsHopEast)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_TUNNEL_RUINS_SOUTH_WALKWAY, FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_IN_FLOOR, EVENT(scTunnelRuinsPushSoutheastSinkingPillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_TUNNEL_RUINS_SOUTH_WALKWAY, FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_FROZEN,
        EVENT(scTunnelRuinsHopWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_TUNNEL_RUINS_SOUTH_WALKWAY, FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_FROZEN,
        EVENT(scTunnelRuinsHopSouth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_TUNNEL_RUINS_SOUTH_WALKWAY, FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_FROZEN,
        EVENT(scTunnelRuinsHopNorth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_TUNNEL_RUINS_SOUTH_WALKWAY_END, FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_SOAKED, EVENT(scTunnelRuinsPushSoutheastSinkingPillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_TUNNEL_RUINS_SOUTH_WALKWAY_END, FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_FROZEN,
        EVENT(scTunnelRuinsHopEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_TUNNEL_RUINS_SOUTH_WALKWAY_END, CONDITION_ALWAYS,
        EVENT(scTunnelRuinsHopWest)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_TUNNEL_RUINS_GIANT_STATUE, CONDITION_ALWAYS, EVENT(scTunnelRuinsSearchGiantStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR_A, CONDITION_ALWAYS, EVENT(scTunnelRuinsUpdateFirstPillarCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR_B, CONDITION_ALWAYS, EVENT(scTunnelRuinsUpdateFirstPillarCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_PILLAR_A, CONDITION_ALWAYS,
        EVENT(scTunnelRuinsUpdateSecondPillarCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_PILLAR_B, CONDITION_ALWAYS,
        EVENT(scTunnelRuinsUpdateSecondPillarCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SMALL_PILLAR_A, CONDITION_ALWAYS, EVENT(scTunnelRuinsUpdateSmallPillarCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SMALL_PILLAR_B, CONDITION_ALWAYS, EVENT(scTunnelRuinsUpdateSmallPillarCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SMALL_PILLAR_C, CONDITION_ALWAYS, EVENT(scTunnelRuinsUpdateSmallPillarCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_IVY_A, FLAG_TUNNEL_RUINS_IVY_WEST_HALF_GONE, EVENT(scTunnelRuinsBlowAwayIvyA)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_IVY_B, FLAG_TUNNEL_RUINS_IVY_EAST_HALF_GONE, EVENT(scTunnelRuinsBlowAwayIvyB)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_IVY_C, FLAG_TUNNEL_RUINS_IVY_BLOWN_AWAY, EVENT(scTunnelRuinsBlowAwayIvyC)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_NORTHWEST_SINKING_PILLAR, FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_IN_FLOOR, EVENT(scTunnelRuinsSinkNorthwestPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_NORTHWEST_PILLAR_FROST_TARGET, FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_FROZEN, EVENT(scTunnelRuinsFreezeNorthwestPillarFrostTarget)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SOUTHEAST_SINKING_PILLAR, FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_IN_FLOOR, EVENT(scTunnelRuinsSinkSoutheastPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SOUTHEAST_PILLAR_FROST_TARGET, FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_FROZEN, EVENT(scTunnelRuinsFreezeSoutheastPillarFrostTarget)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SOUTHEAST_SINKING_PILLAR, FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_SOAKED, EVENT(scTunnelRuinsDouseStartsOnSoutheastPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_SOUTHEAST_SINKING_PILLAR, FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_SOAKED, EVENT(scTunnelRuinsDouseSoutheastPillar)},
    {TRIGGER_TABLE_END},
};

TRIGGER TunnelRuinsTriggersOther[] = {
    {TRIGGER_EXIT, TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_NORTH, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_NORTH},
    {TRIGGER_EXIT, TUNNEL_RUINS_LADY_STATUES_TO_PATHS_NORTHWEST, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_PATHS_NORTHWEST},
    {TRIGGER_EXIT, TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_SOUTHWEST, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_SOUTHWEST},
    {TRIGGER_EXIT, TUNNEL_RUINS_LADY_STATUES_TO_PATHS_NORTHEAST, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_PATHS_NORTHEAST},
    {TRIGGER_EXIT, TUNNEL_RUINS_LADY_STATUES_TO_BALANCE_SOUTH, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_BALANCE_SOUTH},
    {TRIGGER_EXIT, TUNNEL_RUINS_LADY_STATUES_TO_DOUSE_PILLAR_SOUTH, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_DOUSE_PILLAR_SOUTH},
    {TRIGGER_EXIT, TUNNEL_RUINS_LADY_STATUES_TO_REVEAL_STATUE_WEST, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_REVEAL_STATUE_WEST},
    {TRIGGER_EXIT, TUNNEL_RUINS_LADY_STATUES_TO_FIVE_STATUES_NORTH, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_FIVE_STATUES_NORTH},
    {TRIGGER_EXIT, TUNNEL_RUINS_LADY_STATUES_TO_PATHS_SOUTHWEST, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_PATHS_SOUTHWEST},
    {TRIGGER_EXIT, TUNNEL_RUINS_LADY_STATUES_TO_PATHS_SOUTHEAST, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_PATHS_SOUTHEAST},
    {TRIGGER_EXIT, TUNNEL_RUINS_LADY_STATUES_TO_TREE_DOOR_SOUTH, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_TREE_DOOR_SOUTH},
    {TRIGGER_EXIT, TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_SOUTHEAST, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_SOUTHEAST},
    {TRIGGER_EXIT, TUNNEL_RUINS_LADY_STATUES_TO_FIVE_STATUES_EAST, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_FIVE_STATUES_EAST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, TUNNEL_RUINS_LADY_STATUES_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_TUNNEL_RUINS_DOUSE_PILLAR_FLOOR, CONDITION_ALWAYS, EVENT(scTunnelRuinsPushDousePillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_TUNNEL_RUINS_DOUSE_PILLAR_LEDGE, CONDITION_ALWAYS, EVENT(scTunnelRuinsJumpDownLedge)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_TUNNEL_RUINS_DOUSE_PILLAR_LEDGE, CONDITION_ALWAYS,
        EVENT(scTunnelRuinsHopWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_TUNNEL_RUINS_DOUSE_PILLAR_STEP, CONDITION_ALWAYS,
        EVENT(scTunnelRuinsHopDownStep)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_TUNNEL_RUINS_CARVED_STONES_FLOOR, CONDITION_ALWAYS, EVENT(scTunnelRuinsPushCarvedStone)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_TUNNEL_RUINS_RISEN_PLATFORM, CONDITION_ALWAYS,
        EVENT(scTunnelRuinsHopEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_TUNNEL_RUINS_RISEN_PLATFORM, CONDITION_ALWAYS,
        EVENT(scTunnelRuinsHopWest)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_TUNNEL_RUINS_RISEN_PLATFORM, CONDITION_ALWAYS, EVENT(scTunnelRuinsPushCarvedStone)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_TUNNEL_RUINS_GREY_PILLARS_ROW, CONDITION_ALWAYS, EVENT(scTunnelRuinsPushEastPillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_TUNNEL_RUINS_GREY_PILLARS_ROW, CONDITION_ALWAYS,
        EVENT(scTunnelRuinsHopSouth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_TUNNEL_RUINS_GREY_PILLARS_ROW, CONDITION_ALWAYS,
        EVENT(scTunnelRuinsHopNorth)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_TUNNEL_RUINS_LADY_STATUES_FLOOR, CONDITION_ALWAYS, EVENT(scTunnelRuinsPushLadyStatue)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_TUNNEL_RUINS_STATUE_DOOR, CONDITION_ALWAYS, EVENT(scTunnelRuinsSearchLockedDoor)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_TUNNEL_RUINS_LADY_STATUE, CONDITION_ALWAYS, EVENT(scTunnelRuinsSearchLadyStatue)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_ORACLES_ROBE, FLAG_TUNNEL_RUINS_ORACLES_ROBE, FIND_ITEM(ITEM_ORACLES_ROBE)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_ASURAS_ARMOR, FLAG_TUNNEL_RUINS_ASURAS_ARMOR, FIND_ITEM(ITEM_ASURAS_ARMOR)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_DOUSE_PILLAR, FLAG_TUNNEL_RUINS_DOUSE_FROST_TARGET_FROZEN, EVENT(scTunnelRuinsMoveDousePillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_DOUSE_PILLAR, FLAG_TUNNEL_RUINS_DOUSE_PILLAR_SOAKED, EVENT(scTunnelRuinsDouseStartsOnPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_DOUSE_PILLAR_FROST_TARGET, FLAG_TUNNEL_RUINS_DOUSE_FROST_TARGET_FROZEN, EVENT(scTunnelRuinsFreezeDousePillarFrostTarget)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_CARRY_STONE, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_MIDDLE_CARRY_STONE, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_CARRY_STONE, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_WEST_CARRY_STONE, CONDITION_ALWAYS, EVENT(scTunnelRuinsSetStoneOnBalance)},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_MIDDLE_CARRY_STONE, CONDITION_ALWAYS, EVENT(scTunnelRuinsSetStoneOnBalance)},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_EAST_CARRY_STONE, CONDITION_ALWAYS, EVENT(scTunnelRuinsSetStoneOnBalance)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS,
        EVENT(scTunnelRuinsHideVanishingStatues)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
        EVENT(scTunnelRuinsShowVanishingStatues)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_PILLAR_A, CONDITION_ALWAYS,
        EVENT(scTunnelRuinsMoveEastPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_PILLAR_B, CONDITION_ALWAYS,
        EVENT(scTunnelRuinsMoveEastPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_VIOLET_LADY_STATUE, CONDITION_ALWAYS, EVENT(scTunnelRuinsMoveLadyStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GREEN_LADY_STATUE, CONDITION_ALWAYS, EVENT(scTunnelRuinsMoveLadyStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_BLUE_LADY_STATUE, CONDITION_ALWAYS, EVENT(scTunnelRuinsMoveLadyStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_RED_LADY_STATUE, CONDITION_ALWAYS, EVENT(scTunnelRuinsMoveLadyStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_YELLOW_LADY_STATUE, CONDITION_ALWAYS, EVENT(scTunnelRuinsMoveLadyStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VIOLET_LADY_STATUE, CONDITION_ALWAYS,
        MSGNO(MsgTunnelRuinsRedShinesNorthwestOfMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GREEN_LADY_STATUE, CONDITION_ALWAYS,
        MSGNO(MsgTunnelRuinsYellowShinesNortheastOfMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BLUE_LADY_STATUE, CONDITION_ALWAYS,
        MSGNO(MsgTunnelRuinsRedShinesDueWestOfMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RED_LADY_STATUE, CONDITION_ALWAYS,
        MSGNO(MsgTunnelRuinsVioletShinesSoutheastOfMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YELLOW_LADY_STATUE, CONDITION_ALWAYS,
        MSGNO(MsgTunnelRuinsVioletPointsSoutheast)},
    {TRIGGER_TABLE_END},
};
/* clang-format on */
