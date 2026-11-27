// The Alhafran Cave under the town, in two scenes that share this overlay: the cave behind the rear
// entrance by the mayor's home, which the hungry boy's father lets the party into once they give
// the boy Large Bread (SceneIdAlhafranCave), and the town's jail with the caves beyond it
// (SceneIdAlhafraJail).
//
// After Briggs and his crew are jailed, his wife Chaucha and their son Eoleo visit him as Felix
// walks into the jail, and the boy lifts the metatile's key with Psynergy of his own until his
// father stops him (scAlhafranCaveChauchaAndEoleoVisit). In the caves, Pound and Lash reach three
// chests; once Briggs has escaped, a pillar no longer blocks the way, and Frost on a puddle makes a
// pillar of ice up to three more (scAlhafranCaveFreezePuddle).

// This unit declares its own form of fdActorTakeLeaderLayer, below.
#define OWN_fdActorTakeLeaderLayer
// This unit declares its own form of fdActorCycleSpriteColors, below.
#define OWN_fdActorCycleSpriteColors

#include "scene/script_layer.h"
#include "game/game.h"
#include "macros.h"
#include "field/field_mode.h"
#include "field/field_mode_script.h"
#include "scene/scene.h"
#include "scene/ground_effects.h"
#include "field/script_inline.h"
#include "edition.h"
#include "field/call.h"
#include "text/msg_ids.h"
#include "field/actor_script.h"
#include "scene/dehkan_plateau.h"

void          scPillarPlace(const Uint16 *table);
void          scAlhafranCaveTakeLeadersLayer(void);
extern Sint32 AlhafranCaveGuardStepsAsideScript[];
extern Uint32 AlhafranCaveVisitorsLeaveScript[];
void          scAlhafranCaveRaiseBridgeMetatiles(void);
void          scAlhafranCaveLowerBridgeMetatiles(void);
void          scAlhafranCaveCastSand(void);
void          scAlhafranCaveStartMovingCrate(void);
void          scAlhafranCaveFinishMovingCrate(void);

// Set while the ground under the cave's bridges stands at their height, so that the leader walks
// over them rather than under.
#define FLAG_BRIDGE_METATILES_RAISED     FLAG_SCENE_TEMP_04
// Set once Lash has tied the jail's rope.
#define FLAG_JAIL_ROPE_TIED              FLAG_SCENE_TEMP_00
// Set once Pound has sunk the jail's stake.
#define FLAG_JAIL_STAKE_POUNDED          FLAG_SCENE_TEMP_01
// Set once Frost has frozen the cave's puddle.
#define FLAG_ALHAFRAN_CAVE_PUDDLE_FROZEN FLAG_SCENE_TEMP_03

// The cave's actors (SceneIdAlhafranCave), numbering the rows of AlhafranCaveCaveActorPlacements.
typedef enum ALHAFRAN_CAVE_ACTOR_ID
{
	// Three stone bridges, raised twelve pixels over the channel and the chasm.
	ACTOR_CAVE_FIRST_BRIDGE = ACTOR_FIRST_PLACED,
	ACTOR_CAVE_SECOND_BRIDGE,
	ACTOR_CAVE_THIRD_BRIDGE,
	// A broken bridge: its two ends on their legs (SPRITE_WOODEN_SIGN, in its animations 2 and 3)
	// and its middle fallen below them, turned a sixteenth of a turn.
	ACTOR_CAVE_BROKEN_BRIDGE_WEST_END,
	ACTOR_CAVE_BROKEN_BRIDGE_EAST_END,
	ACTOR_CAVE_BROKEN_BRIDGE_MIDDLE,
	// Two stone pillars, one in front of a ladder and one across a passage, that stand 16 pixels
	// west once the mast is cleared and the jail is empty.
	ACTOR_CAVE_PILLAR_AT_LADDER,
	ACTOR_CAVE_PILLAR_IN_PASSAGE,
	// The puddle Frost freezes into a pillar of ice.
	ACTOR_CAVE_PUDDLE
} ALHAFRAN_CAVE_ACTOR_ID;

// The jail's actors (SceneIdAlhafraJail), numbering the rows of AlhafranCaveJailActorPlacements.
typedef enum ALHAFRA_JAIL_ACTOR_ID
{
	// The crate Move pushes.
	ACTOR_CRATE = ACTOR_FIRST_PLACED,
	// The stake Pound sinks.
	ACTOR_JAIL_STAKE,
	// The posts of the rope Lash ties, on the floor and on the ledge.
	ACTOR_JAIL_ROPE_LOW_POST,
	ACTOR_JAIL_ROPE_HIGH_POST,
	// A broken bridge's two ends and its fallen middle, as the cave's.
	ACTOR_JAIL_BROKEN_BRIDGE_WEST_END,
	ACTOR_JAIL_BROKEN_BRIDGE_EAST_END,
	ACTOR_JAIL_BROKEN_BRIDGE_MIDDLE,
	// The key on the guard's table outside the metatile, which Eoleo lifts with a Move hand of his
	// own.
	ACTOR_JAIL_KEY,
	// Briggs and his crew in their metatile, once Briggs is caught. "I hate prison. The food is
	// terrible..."
	ACTOR_PIRATE_WHO_HATES_PRISON,
	// "It sounds like the mayor of Alhafra lost interest in fixing that boat."
	ACTOR_PIRATE_ON_THE_BOAT,
	ACTOR_BRIGGS,
	// "Everything would have been fine if it hadn't been for you meddling kids..."
	ACTOR_PIRATE_BLAMING_THE_PARTY,
	// "I saw you staring at Eoleo..."
	ACTOR_PIRATE_GUARDING_EOLEOS_SECRET,
	ACTOR_EOLEO,
	ACTOR_CHAUCHA,
	// "Everything's so boring here... Why doesn't anybody do anything...?"
	ACTOR_BORED_GUARD,
	ACTOR_GUARD
} ALHAFRA_JAIL_ACTOR_ID;

// The jail's way up through the hole the crate hides, from the tunnel behind the metatile.
typedef enum ALHAFRA_JAIL_ENTRANCE
{
	ENTRANCE_UNDER_CRATE = 9
} ALHAFRA_JAIL_ENTRANCE;

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	return 0;
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

// The ROM's caller here is compiled against this form, which takes nothing where the definition
// takes the actor.
void                                        fdActorCycleSpriteColors(void);
// The ROM's caller here is compiled against this form, which takes nothing and returns nothing
// where the definition takes the actor and returns an Sint32.
void                                        fdActorTakeLeaderLayer(void);
void                                        scAlhafranCaveSetUpCave(void);
void                                        scAlhafranCaveSetUpJail(void);

extern Sint16                               AlhafranCavePuddles[];
extern Uint16                               AlhafranCavePoundStakes[];
extern ACTOR_PLACE                          AlhafranCaveCaveActorPlacements[];
extern ACTOR_PLACE                          AlhafranCaveJailActorPlacements[];
extern CUE                                  AlhafranCaveCaveCues[];
extern CUE                                  AlhafranCaveJailCues[];
EFFECT_SPR AlhafranCaveCrateSprite BSS_DATA ALIGNED(16);
// The tick count of scAlhafranCaveIdleEvent, cleared as the event is added, as other scenes clear
// the count their script's event keeps; the event is empty, so nothing counts it or reads it.
Sint32 AlhafranCaveIdleEventTick            BSS_DATA;

// The current scene's actors: the cave's or the jail's.
const ACTOR_PLACE                          *scGetActorPlacements(void)
{
	Sint32 scene = MainRoutineStateWork.next;

	if (scene == SCENE_ALHAFRAN_CAVE)
		return AlhafranCaveCaveActorPlacements;
	if (scene == SCENE_ALHAFRA_JAIL)
		return AlhafranCaveJailActorPlacements;
	return AlhafranCaveCaveActorPlacements;
}

// Felix casts Pound on the stake in the caves beyond the jail and drives it into the ground.
void scAlhafranCavePoundStake(Sint32 mode, Sint32 frame)
{
	fdPoundSinkTarget(AlhafranCavePoundStakes, mode, frame);
}

// Nothing more than the move itself happens as Felix starts casting Move on the crate in the jail.
void scAlhafranCaveStartMovingCrate(void)
{
}

// Nothing more than the move itself happens once Move has set the jail's crate down.
void scAlhafranCaveFinishMovingCrate(void)
{
}

// Felix talks to Briggs in his metatile, who asks whether he has stopped by for a visit, and
// answers yes or no.
void scAlhafranCaveTalkToBriggs(Sint32 id)
{
	syFieldScriptSetMessageId((Sint32)MsgOhStoppedBy);
	// "Oh, stopped by for a visit, did you?" Yes: "I wish that I'd met you under different
	// circumstances..." No: "Ah, so you just came here to check up on us? Hah! Well, as you can..."
	fdScriptOpenMessage(id, 0);
	if (syFieldScriptAskYesNoWithActorAnimation(syFieldPartyGetLeaderActor(), 0) == 0)
	{
		syFieldScriptWait(10);
		syFieldScriptSetMessageId((Sint32)MsgOhStoppedBy + 1);
	}
	else
	{
		syFieldScriptWait(20);
		syFieldScriptSetMessageId((Sint32)MsgOhStoppedBy + 2);
	}
	fdScriptShowMessage(id, 0);
}

// Felix casts Frost on the puddle below the three chests in the cave, and it freezes into a pillar
// of ice that lifts the way up to them.
void scAlhafranCaveFreezePuddle(void)
{
	fdFlagBlockApplyFlags(AlhafranCavePuddles);
}

// Nothing visible happens as Felix walks east onto metatile (18, 8), on the ledge 32 pixels up
// along the cave's north wall; the game raises the ground under the southern stone bridge
// (metatiles (25, 19) to (27, 19)) and under the broken bridge's two ends (metatiles (18, 24) and
// (21, 24)) to the height of the ledges they join, so that from the ledges he walks over the
// bridges rather than under them. Walking back west along the ledge onto metatile (17, 7), the
// metatile north-west of it, lowers them again (scAlhafranCaveLowerBridgeMetatiles).
//
// The ground goes up four steps, and the metatile north-west of the southern bridge becomes a wall
// so that he cannot step off its end.
void scAlhafranCaveRaiseBridgeMetatiles(void)
{
	fdScriptSetFlag(FLAG_BRIDGE_METATILES_RAISED);
	syFieldMapGridSetMetatileHeight(toFIXED(400), toFIXED(304), 0, 4);
	syFieldMapGridSetMetatileHeight(toFIXED(416), toFIXED(304), 0, 4);
	fdMapGridSetMetatileHeight(toFIXED(432), toFIXED(304), 0, 4);
	syFieldMapGridSetMetatileBehavior(toFIXED(384), toFIXED(288), 0, METATILE_BEHAVIOR_WALL);
	syFieldMapGridSetMetatileHeight(toFIXED(288), toFIXED(384), 0, 4);
	fdMapGridSetMetatileHeight(toFIXED(336), toFIXED(384), 0, 4);
}

// Nothing visible happens as Felix walks past metatile (17, 7) or (15, 25); the game lowers the
// ground under the bridges to the cave's floor again, so that down there he walks under them.
void scAlhafranCaveLowerBridgeMetatiles(void)
{
	fdScriptClearFlag(FLAG_BRIDGE_METATILES_RAISED);
	syFieldMapGridSetMetatileHeight(toFIXED(400), toFIXED(304), 0, 0);
	syFieldMapGridSetMetatileHeight(toFIXED(416), toFIXED(304), 0, 0);
	fdMapGridSetMetatileHeight(toFIXED(432), toFIXED(304), 0, 0);
	syFieldMapGridSetMetatileBehavior(toFIXED(384), toFIXED(288), 0, 0);
	syFieldMapGridSetMetatileHeight(toFIXED(288), toFIXED(384), 0, 0);
	fdMapGridSetMetatileHeight(toFIXED(336), toFIXED(384), 0, 0);
}

// Felix casts Sand in the cave: the world turns grey and he sinks into the floor as a mound of
// sand. The game draws the mound's sand trail at priority 2 here rather than the usual 3.
void scAlhafranCaveCastSand(void)
{
	FIELD_STATE *state       = RamHeapSlots->fieldState;

	state->sandTrailPriority = 2;
}

// The current scene's cues: the cave's or the jail's.
const CUE *scGetCues(void)
{
	Sint32 scene = MainRoutineStateWork.next;

	if (scene == SCENE_ALHAFRAN_CAVE)
		return AlhafranCaveCaveCues;
	if (scene == SCENE_ALHAFRA_JAIL)
		return AlhafranCaveJailCues;
	return AlhafranCaveCaveCues;
}

// Nothing visible: each tick the game notes whether Felix stands on the cave's upper level, for
// wading.
void scAlhafranCaveCheckUpperLevel(void)
{
	FIXED y = fdActorGet(MainRoutineStateWork.leaderActorId)->y.fixed;

	if (y < toFIXED(8))
		MainRoutineStateWork.onUpperLevel = 0;
	else
		MainRoutineStateWork.onUpperLevel = 1;
}

// The jail's event: draws the crate's top where the crate stands.
Sint32 scAlhafranCaveDrawCrateSprite(void)
{
	ACTOR *actor = fdActorGet(ACTOR_CRATE);

	fdEffectSpriteDrawAtWorldXYZ(
		&AlhafranCaveCrateSprite, actor->x.fixed, actor->y.fixed + toFIXED(28), actor->z.fixed);
	return 0;
}

// Sets up the cave or the jail.
Sint32 scInit(void)
{
// The Japanese edition does not check the entrance.
#if !REVISED_ENTRANCE_TRANSITIONS
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	if (MainRoutineStateWork.next == SCENE_ALHAFRAN_CAVE)
		scAlhafranCaveSetUpCave();
	else if (MainRoutineStateWork.next == SCENE_ALHAFRA_JAIL)
		scAlhafranCaveSetUpJail();
	return 0;
#else
	Sint32 scene = MainRoutineStateWork.next;

	if (scene == SCENE_ALHAFRAN_CAVE)
	{
		FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
		scAlhafranCaveSetUpCave();
	}
	else if (scene == SCENE_ALHAFRA_JAIL)
	{
		if (MainRoutineStateWork.entrance == ALHAFRA_JAIL_FROM_ALHAFRA
		    || MainRoutineStateWork.entrance == ENTRANCE_UNDER_CRATE)
			FieldStateWork->transition =
				SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
		else
			FieldStateWork->transition =
				SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
		scAlhafranCaveSetUpJail();
	}
	return 0;
#endif
}

// Sets up the cave: its bridges, the broken bridge, the pillars, the puddle, and the metatiles
// under the bridges if the leader was on the ledges.
void scAlhafranCaveSetUpCave(void)
{
	MAP_STATE_GROUND_EFFECTS *map = (MAP_STATE_GROUND_EFFECTS *)RamHeapSlots->mapGrid;
	ACTOR                    *actor;

	fdActorGet(MainRoutineStateWork.leaderActorId)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
	map->wading                                                    = MAP_WADING_UPPER_LEVEL;
	sySetTickEvent(scAlhafranCaveCheckUpperLevel, EVENT_PASS_TICK);
	if (fdScriptGetFlag(FLAG_BRIDGE_METATILES_RAISED))
		scAlhafranCaveRaiseBridgeMetatiles();
	actor              = fdActorGet(ACTOR_CAVE_FIRST_BRIDGE);
	actor->motionFlags = 0;
	actor->y.fixed     = toFIXED(12);
	actor              = fdActorGet(ACTOR_CAVE_SECOND_BRIDGE);
	actor->motionFlags = 0;
	actor->y.fixed     = toFIXED(12);
	actor              = fdActorGet(ACTOR_CAVE_THIRD_BRIDGE);
	actor->motionFlags = 0;
	actor->y.fixed     = toFIXED(12);
	fdActorSetSpriteColorMode(fdActorGet(ACTOR_CAVE_FIRST_BRIDGE), SPRITE_COLOR_GREY);
	fdActorSetSpriteColorMode(fdActorGet(ACTOR_CAVE_SECOND_BRIDGE), SPRITE_COLOR_GREY);
	fdActorSetSpriteColorMode(fdActorGet(ACTOR_CAVE_THIRD_BRIDGE), SPRITE_COLOR_GREY);
	actor              = fdActorGet(ACTOR_CAVE_BROKEN_BRIDGE_EAST_END);
	actor->motionFlags = 0;
	actor->y.fixed     = toFIXED(16);
	actor              = fdActorGet(ACTOR_CAVE_BROKEN_BRIDGE_WEST_END);
	actor->motionFlags = 0;
	actor->y.fixed     = toFIXED(16);
	scFieldActorSetSpriteRotationById(ACTOR_CAVE_BROKEN_BRIDGE_MIDDLE, TRIG_TURN / 16);
	fdActorSetAnimation(ACTOR_CAVE_BROKEN_BRIDGE_WEST_END, 2);
	fdActorSetAnimation(ACTOR_CAVE_BROKEN_BRIDGE_EAST_END, 3);
	fdActorSetAnimation(ACTOR_CAVE_BROKEN_BRIDGE_MIDDLE, 2);
	scFieldActorSetSpritePriority(ACTOR_CAVE_BROKEN_BRIDGE_WEST_END, 2);
	scFieldActorSetSpritePriority(ACTOR_CAVE_BROKEN_BRIDGE_EAST_END, 2);
	scFieldActorSetSpritePriority(ACTOR_CAVE_BROKEN_BRIDGE_MIDDLE, 3);
	fdActorGet(ACTOR_CAVE_BROKEN_BRIDGE_WEST_END)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
	fdActorGet(ACTOR_CAVE_BROKEN_BRIDGE_EAST_END)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
	fdActorGet(ACTOR_CAVE_BROKEN_BRIDGE_MIDDLE)->priorityFlags   |= ACTOR_PRIORITY_UNDERFOOT;
	fdActorSetSpriteColorMode(fdActorGet(ACTOR_CAVE_BROKEN_BRIDGE_WEST_END), SPRITE_COLOR_GREY);
	fdActorSetSpriteColorMode(fdActorGet(ACTOR_CAVE_BROKEN_BRIDGE_EAST_END), SPRITE_COLOR_GREY);
	fdActorSetSpriteColorMode(fdActorGet(ACTOR_CAVE_BROKEN_BRIDGE_MIDDLE), SPRITE_COLOR_GREY);
	if (fdScriptGetFlag(FLAG_ALHAFRA_MAST_CLEARED))
	{
		fdActorSetPos(ACTOR_CAVE_PILLAR_AT_LADDER, 200, 136);
		fdActorSetPos(ACTOR_CAVE_PILLAR_IN_PASSAGE, 392, 360);
	}
	actor               = fdActorGet(ACTOR_CAVE_PILLAR_AT_LADDER);
	actor->motionFlags  = 0;
	actor->groundHeight = 0;
	actor->y.fixed      = 0;
	fdMapGridSetMetatileHeight(actor->x.fixed, actor->z.fixed, 0, 4);
	actor               = fdActorGet(ACTOR_CAVE_PILLAR_IN_PASSAGE);
	actor->motionFlags  = 0;
	actor->groundHeight = 0;
	actor->y.fixed      = 0;
	fdMapGridSetMetatileHeight(actor->x.fixed, actor->z.fixed, 0, 4);
	fdFlagBlockPlaceAllMarkingEdges(AlhafranCavePuddles);
	fdActorSetMovementType(fdActorGet(ACTOR_CAVE_PUDDLE), MOVEMENT_TYPE_TAKE_LEADER_LAYER);
	// Until the mast is cleared, the ladder behind the first pillar is drawn as rock, with two more
	// stretches of the cave.
	if (!fdScriptGetFlag(FLAG_ALHAFRA_MAST_CLEARED))
	{
		fdMapGridCopyMetatilesLayered(0, 58, 12, 7, 13, 58);
		fdMapGridCopyMetatilesLayered(0, 58, 2, 9, 25, 66);
		fdMapGridCopyMetatilesLayered(0, 58, 10, 5, 15, 74);
	}
}

// Sets up the jail and the cave: who is held and who visits, by how far the story has gone (Briggs
// caught, his family's visit, the mast cleared), and the crate pushed aside as the party climbs up
// through the hole under it.
void scAlhafranCaveSetUpJail(void)
{
	EFFECT_SPR  *crate;
	FIELD_STATE *state;

	fdActorGet(MainRoutineStateWork.leaderActorId)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
	fdActorGet(ACTOR_CRATE)->priorityFlags                        |= ACTOR_PRIORITY_IGNORES_RISES;
	fdEffectStartBlankOverlay(0);
	crate = &AlhafranCaveCrateSprite;
	fdFadeOverlayFillTiles(0);
	Call4((void (*)())fdEffectSpriteInitWithFadeOverlay, (Sint32)crate, 8, 15, OBJ_SIZE_16X16);
	syFieldSetTickEvent(scAlhafranCaveDrawCrateSprite, EVENT_PASS_TICK);
	if (MainRoutineStateWork.entrance == ALHAFRA_JAIL_FROM_ALHAFRA
	    || MainRoutineStateWork.entrance == ENTRANCE_UNDER_CRATE)
	{
		state                    = RamHeapSlots->fieldState;

		state->psynergyDrawFlags = PSYNERGY_DRAW_NO_SCENE_SPRITE | PSYNERGY_DRAW_FIXED_PRIORITY;
	}
	fdRopeInit();
	fdRopePlace(0, FLAG_JAIL_ROPE_TIED, ACTOR_JAIL_ROPE_LOW_POST, ACTOR_JAIL_ROPE_HIGH_POST);
	fdActorGet(ACTOR_JAIL_ROPE_LOW_POST)->priorityFlags  |= ACTOR_PRIORITY_IGNORES_RISES;
	fdActorGet(ACTOR_JAIL_ROPE_HIGH_POST)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
	scPillarPlace(AlhafranCavePoundStakes);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_JAIL_STAKE), 0);
	scFieldActorSetSpriteRotationById(ACTOR_JAIL_BROKEN_BRIDGE_MIDDLE, TRIG_TURN / 16);
	fdActorSetAnimation(ACTOR_JAIL_BROKEN_BRIDGE_WEST_END, 0);
	fdActorSetAnimation(ACTOR_JAIL_BROKEN_BRIDGE_EAST_END, 1);
	fdActorSetAnimation(ACTOR_JAIL_BROKEN_BRIDGE_MIDDLE, 2);
	scFieldActorSetSpritePriority(ACTOR_JAIL_BROKEN_BRIDGE_WEST_END, 1);
	scFieldActorSetSpritePriority(ACTOR_JAIL_BROKEN_BRIDGE_EAST_END, 1);
	fdActorGet(ACTOR_JAIL_BROKEN_BRIDGE_WEST_END)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
	fdActorGet(ACTOR_JAIL_BROKEN_BRIDGE_EAST_END)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
	fdActorGet(ACTOR_JAIL_BROKEN_BRIDGE_MIDDLE)->priorityFlags   |= ACTOR_PRIORITY_UNDERFOOT;
	fdActorSetSpriteColorMode(fdActorGet(ACTOR_JAIL_BROKEN_BRIDGE_WEST_END), SPRITE_COLOR_GREY);
	fdActorSetSpriteColorMode(fdActorGet(ACTOR_JAIL_BROKEN_BRIDGE_EAST_END), SPRITE_COLOR_GREY);
	fdActorSetSpriteColorMode(fdActorGet(ACTOR_JAIL_BROKEN_BRIDGE_MIDDLE), SPRITE_COLOR_GREY);
	if (fdScriptGetFlag(FLAG_ALHAFRA_MAST_CLEARED))
	{
		fdActorSetPos(ACTOR_JAIL_KEY, 0, 0);
		fdActorSetPos(ACTOR_BRIGGS, 0, 0);
		fdActorSetPos(ACTOR_PIRATE_WHO_HATES_PRISON, 0, 0);
		fdActorSetPos(ACTOR_PIRATE_ON_THE_BOAT, 0, 0);
		fdActorSetPos(ACTOR_PIRATE_BLAMING_THE_PARTY, 0, 0);
		fdActorSetPos(ACTOR_PIRATE_GUARDING_EOLEOS_SECRET, 0, 0);
		fdActorSetPos(ACTOR_EOLEO, 0, 0);
		fdActorSetPos(ACTOR_CHAUCHA, 0, 0);
		fdActorSetPos(ACTOR_GUARD, 0, 0);
		fdActorGet(ACTOR_BORED_GUARD)->update = scAlhafranCaveTakeLeadersLayer;
	}
	else if (fdScriptGetFlag(FLAG_BRIGGS_CAUGHT))
	{
		fdActorSetPos(ACTOR_BORED_GUARD, 0, 0);
		scFieldActorSetSpritePriority(ACTOR_BRIGGS, 2);
		scFieldActorSetSpritePriority(ACTOR_PIRATE_WHO_HATES_PRISON, 2);
		scFieldActorSetSpritePriority(ACTOR_PIRATE_ON_THE_BOAT, 2);
		scFieldActorSetSpritePriority(ACTOR_PIRATE_BLAMING_THE_PARTY, 2);
		scFieldActorSetSpritePriority(ACTOR_PIRATE_GUARDING_EOLEOS_SECRET, 2);
		if (!fdScriptGetFlag(FLAG_FAMILY_VISITED_BRIGGS))
		{
			fdActorSetPos(ACTOR_PIRATE_ON_THE_BOAT, 712, 216);
			fdActorSetPos(ACTOR_PIRATE_BLAMING_THE_PARTY, 744, 216);
		}
		else
		{
			fdActorSetPos(ACTOR_CHAUCHA, 0, 0);
			fdActorSetPos(ACTOR_EOLEO, 0, 0);
		}
	}
	else
	{
		fdActorSetPos(ACTOR_BRIGGS, 0, 0);
		fdActorSetPos(ACTOR_PIRATE_WHO_HATES_PRISON, 0, 0);
		fdActorSetPos(ACTOR_PIRATE_ON_THE_BOAT, 0, 0);
		fdActorSetPos(ACTOR_PIRATE_BLAMING_THE_PARTY, 0, 0);
		fdActorSetPos(ACTOR_PIRATE_GUARDING_EOLEOS_SECRET, 0, 0);
		fdActorSetPos(ACTOR_EOLEO, 0, 0);
		fdActorSetPos(ACTOR_CHAUCHA, 0, 0);
		fdActorSetPos(ACTOR_GUARD, 0, 0);
		fdActorGet(ACTOR_BORED_GUARD)->update = scAlhafranCaveTakeLeadersLayer;
	}
	if (MainRoutineStateWork.entrance == ENTRANCE_UNDER_CRATE
	    && !fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION))
	{
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionTicks();
		fdActorSetSpeed(ACTOR_CRATE, 0.3);
		fdActorSetDestinationOffset(ACTOR_CRATE, 0, 16);
		scFieldActorWaitForMoveById(ACTOR_CRATE);
	}
}

// Once the mast is cleared and the jail stands empty, the passage where the tunnel from under the
// crate comes out (entrance 10) is drawn into the rope's cave.
Sint32 scPrepareMap(void)
{
	if (MainRoutineStateWork.next == SCENE_ALHAFRA_JAIL
	    && fdScriptGetFlag(FLAG_ALHAFRA_MAST_CLEARED))
	{
		fdMapGridCopyMetatilesLayered(23, 46, 4, 5, 21, 14);
		fdMapGridCopyMetatilesLayered(86, 44, 5, 6, 84, 12);
		fdMapGridCopyMetatileAttributes(22, 45, 5, 6, 20, 13);
	}
	return 0;
}

// An empty event, added as Eoleo starts crying and never removed; it does nothing.
void scAlhafranCaveIdleEvent(void)
{
}

extern Uint8 MsgYouHereTo[];

void         scAlhafranCaveCycleKeyColors(void);

// Felix walks west into the jail after Briggs is caught (metatiles (49, 16) to (49, 19)): the guard
// asks if he is here to see Briggs, and Briggs's wife Chaucha and their son Eoleo visit him. Eoleo
// lifts the metatile's key off the guard's table with Psynergy of his own until Briggs cries out
// his name, then bursts into tears, and Chaucha takes him home. It happens once, and not after the
// mast is cleared.
void         scAlhafranCaveChauchaAndEoleoVisit(void)
{
	ACTOR *eoleo;
	ACTOR *key;
	ACTOR *effect;
	FIXED  pos[3];

	if (syGameFlagGet(FLAG_BRIGGS_CAUGHT) == 0)
		return;
	if (syGameFlagGet(FLAG_ALHAFRA_MAST_CLEARED) == 1)
		return;
	fdScriptSetFlag(FLAG_FAMILY_VISITED_BRIGGS);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorSetSpeed(ACTOR_CHAUCHA, 1);
	fdActorSetSpeed(ACTOR_EOLEO, 1);
	fdActorSetSpeed(ACTOR_PIRATE_ON_THE_BOAT, 1);
	fdActorSetSpeed(ACTOR_PIRATE_BLAMING_THE_PARTY, 1);
	fdActorSetSpeed(ACTOR_PIRATE_GUARDING_EOLEOS_SECRET, 1);
	syFieldScriptSetMessageId((Sint32)MsgYouHereTo);
	// "You here to see Briggs?"
	syFieldScriptShowMessageThenWait(ACTOR_GUARD, 0, 10);
	fdActorWalkToAndWait(ACTOR_FELIX, 800, 288);
	fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
	fdCameraMoveTo(744, CAMERA_KEEP, 264, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
	syFieldScriptWait(20);
	// "Chaucha... It's good to see you."
	syFieldScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
	syFieldScriptWait(30);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST, 0);
	syFieldScriptWait(20);
	// "And you, too, Eoleo."
	syFieldScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	scFieldActorJump(ACTOR_EOLEO, 6, 15);
	scFieldActorJump(ACTOR_EOLEO, 6, 23);
	// "Goo. Goo goo ga."
	syFieldScriptShowMessageThenWait(ACTOR_EOLEO, 0, 10);
	fdActorShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	// "How...is your metatile? Is there anything I can do to make this easier..."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_PIRATE_WHO_HATES_PRISON, 2);
	syFieldScriptWait(10);
	// "I'm fine, Chaucha. My only complaint is that the food here is terrible."
	syFieldScriptShowMessageThenWait(ACTOR_PIRATE_WHO_HATES_PRISON, 0, 10);
	fdActorShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	// "What!? Are they forcing you to eat this disgusting gruel?"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0, 10);
	fdActorFaceDirection(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHEAST, 0);
	syFieldScriptWait(30);
	(void *)fdActorShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_SCOWL, 50);
	fdCameraMoveTo(744, CAMERA_KEEP, 312, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(20);
	fdActorSetAnimation(ACTOR_EOLEO, ANIM_NOD);
	scFieldActorStartRepeatedMotion(ACTOR_GUARD, 3);
	fdActorSetAttachedEffect(ACTOR_GUARD, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(40);
	syScriptCameraFollowActor(ACTOR_GUARD, 1);
	fdCameraWaitForArrival();
	fdActorSetSpeed(ACTOR_GUARD, 1.5);
	fdActorSetScript(ACTOR_GUARD, AlhafranCaveGuardStepsAsideScript);
	syFieldScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_EOLEO, ANIM_NOD);
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_EOLEO, FACING_SOUTH_SOUTHEAST, 0);
	scFieldActorWaitForScriptEndById(ACTOR_GUARD);
	fdActorFaceDirection(ACTOR_CHAUCHA, FACING_EAST, 0);
	fdCameraMoveTo(744, CAMERA_KEEP, 272, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	// "Now, wait a second..."
	syFieldScriptShowMessageThenWait(ACTOR_GUARD, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_GUARD, ANIM_SHAKE_HEAD);
	syFieldScriptWait(10);
	// "First of all, it's not disgusting. It's nutritious! And besides..."
	syFieldScriptShowMessageThenWait(ACTOR_GUARD, 0, 10);
	fdActorFaceDirection(ACTOR_GUARD, FACING_NORTH_NORTHWEST, 0);
	syFieldScriptWait(20);
	fdActorShowEmote(ACTOR_GUARD, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
	// "You're IN JAIL! What did you expect, gourmet cuisine?"
	syFieldScriptShowMessageThenWait(ACTOR_GUARD, 0, 10);
	eoleo = fdActorGet(ACTOR_EOLEO);
	key   = fdActorGet(ACTOR_JAIL_KEY);
	fdScriptStartColorTransition(8, RGB(16, 16, 4));
	fdBattleEffectStartArcEffect();
	fdBattleEffectSetObjectUpdateMode(fdActorGet(ACTOR_EOLEO), CASTER_LOOK_CYCLING_COLORS);
	fdScriptPlaySound(SE_ASCEND);
	syFieldScriptWait(40);
	effect               = fdEffectSpawnMoveHand(eoleo);
	pos[0]               = key->x.fixed;
	effect->watchedActor = (union FIELD_OBJECT *)key;
	pos[1]               = key->y.fixed + toFIXED(16);
	pos[2]               = key->z.fixed;
	syFieldVectorAddPolarOffset(16, FACING_NORTH, pos);
	scFieldActorSetMoveTargetFX(effect, pos[0], pos[1], pos[2]);
	fdEffectGrowHandAndWait(effect);
	fdActorSelectAnimation(effect, 4);
	key->update = scAlhafranCaveCycleKeyColors;
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTHWEST, 0);
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_PIRATE_ON_THE_BOAT, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_PIRATE_BLAMING_THE_PARTY, FACING_WEST, 0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	syFieldScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_PIRATE_ON_THE_BOAT, ANIM_NOD);
	syFieldScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_PIRATE_BLAMING_THE_PARTY, ANIM_NOD);
	fdActorFaceDirection(ACTOR_PIRATE_GUARDING_EOLEOS_SECRET, FACING_SOUTH_SOUTHEAST, 0);
	syFieldScriptWait(30);
	fdCameraMoveTo(808, CAMERA_KEEP, 288, 1);
	fdCameraWaitForArrival();
	scFieldActorStartRepeatedMotion(ACTOR_FELIX, 3);
	fdActorSetAttachedEffect(ACTOR_FELIX, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(50);
	fdActorShowEmote(ACTOR_PIRATE_GUARDING_EOLEOS_SECRET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdCameraMoveTo(744, CAMERA_KEEP, 272, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(20);
	fdActorWalkByAndWait(ACTOR_PIRATE_GUARDING_EOLEOS_SECRET, -16, 0);
	syFieldScriptWait(20);
	(void *)fdActorShowEmote(
		ACTOR_PIRATE_GUARDING_EOLEOS_SECRET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHEAST, 0);
	syFieldScriptWait(40);
	scFieldActorStartRepeatedMotion(ACTOR_BRIGGS, 3);
	fdActorSetAttachedEffect(ACTOR_BRIGGS, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(40);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST, 0);
	syFieldScriptWait(30);
	// "Eoleo!"
	syFieldScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 20);
	fdActorShowEmote(ACTOR_EOLEO, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	key->update = 0;
	fdActorSetSpriteColorMode(key, 0);
	fdEffectBurstHand(effect);
	fdBattleEffectSetObjectUpdateMode(fdActorGet(ACTOR_EOLEO), CASTER_LOOK_NORMAL);
	fdBattleEffectWaitForArcGroups();
	fdBattleEffectEndArcEffect();
	fdStartPaletteTransition(8);
	syFieldScriptWait(30);
	fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_PIRATE_WHO_HATES_PRISON, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_PIRATE_ON_THE_BOAT, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_PIRATE_BLAMING_THE_PARTY, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_PIRATE_GUARDING_EOLEOS_SECRET, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GUARD, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST, 0);
	syFieldScriptWait(20);
	scFieldActorStartRepeatedMotion(ACTOR_EOLEO, 3);
	fdActorSetAttachedEffect(ACTOR_EOLEO, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(40);
	fdActorFaceDirection(ACTOR_EOLEO, FACING_NORTH, 0);
	syFieldScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_SHAKE_HEAD);
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_PIRATE_WHO_HATES_PRISON, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_PIRATE_ON_THE_BOAT, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_PIRATE_BLAMING_THE_PARTY, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_PIRATE_GUARDING_EOLEOS_SECRET, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_GUARD, FACING_WEST, 0);
	syFieldScriptWait(40);
	fdActorShowEmote(ACTOR_EOLEO, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorSetAnimationAndWait(ACTOR_EOLEO, ANIM_SHAKE_HEAD);
	syFieldScriptWait(10);
	fdActorFaceDirection(ACTOR_EOLEO, FACING_SOUTH_SOUTHEAST, 0);
	syFieldScriptWait(20);
	fdStartColorTransition(8, RGB(16, 16, 4));
	fdBattleEffectStartArcEffect();
	fdBattleEffectSetObjectUpdateMode(fdActorGet(ACTOR_EOLEO), CASTER_LOOK_CYCLING_COLORS);
	fdScriptPlaySound(SE_ASCEND);
	syFieldScriptWait(40);
	// "Eoleo!"
	syFieldScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	fdBattleEffectSetObjectUpdateMode(fdActorGet(ACTOR_EOLEO), CASTER_LOOK_NORMAL);
	fdBattleEffectWaitForArcGroups();
	fdBattleEffectEndArcEffect();
	fdStartPaletteTransition(8);
	fdActorFaceDirection(ACTOR_PIRATE_WHO_HATES_PRISON, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_PIRATE_ON_THE_BOAT, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_PIRATE_BLAMING_THE_PARTY, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_PIRATE_GUARDING_EOLEOS_SECRET, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GUARD, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST, 0);
	syFieldScriptWait(40);
	fdActorFaceDirection(ACTOR_EOLEO, FACING_NORTH_NORTHEAST, 0);
	syFieldScriptWait(20);
	// "Hooo..."
	syFieldScriptShowMessageThenWait(ACTOR_EOLEO, 0, 10);
	(void *)fdActorShowEmote(ACTOR_EOLEO, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 50);
	fdActorFaceDirection(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHWEST, 0);
	syFieldScriptWait(30);
	fdActorShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
	fdActorRunRepeatedMotion(ACTOR_EOLEO, 2);
	syFieldScriptWait(20);
	// "Ahhh waaah..."
	syFieldScriptShowMessageThenWait(ACTOR_EOLEO, 0, 10);
	fdActorFaceDirection(ACTOR_PIRATE_WHO_HATES_PRISON, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_PIRATE_ON_THE_BOAT, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_PIRATE_BLAMING_THE_PARTY, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_PIRATE_GUARDING_EOLEOS_SECRET, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_GUARD, FACING_WEST, 0);
	syFieldScriptWait(20);
	fdActorSetAttachedEffect(ACTOR_EOLEO, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
	// "WAHHH!"
	syFieldScriptShowMessageThenWait(ACTOR_EOLEO, 0, 20);
	AlhafranCaveIdleEventTick = 0;
	sySetTickEvent(scAlhafranCaveIdleEvent, EVENT_PASS_TICK);
	fdActorShowEmote(ACTOR_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	// "I don't know why he's crying! You're his mother! Can't you do..."
	syFieldScriptShowMessageThenWait(ACTOR_GUARD, 0, 10);
	fdActorFaceDirection(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST, 0);
	syFieldScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
	syFieldScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHWEST, 0);
	syFieldScriptWait(20);
	fdActorWalkByAndWait(ACTOR_CHAUCHA, 0, 8);
	fdActorFaceDirection(ACTOR_CHAUCHA, FACING_WEST, 0);
	syFieldScriptWait(20);
	// "There there, Eoleo... What's the matter?"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_EOLEO, 2);
	syFieldScriptWait(10);
	fdActorSetAttachedEffect(ACTOR_EOLEO, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
	// "Waa waa waa!"
	syFieldScriptShowMessageThenWait(ACTOR_EOLEO, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
	syFieldScriptWait(10);
	// "Did the mean old guard frighten you?"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0, 10);
	fdActorFaceDirection(ACTOR_EOLEO, FACING_EAST, 0);
	syFieldScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_EOLEO, ANIM_NOD);
	syFieldScriptWait(10);
	// "Waa waa..."
	syFieldScriptShowMessageThenWait(ACTOR_EOLEO, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
	syFieldScriptWait(10);
	// "Don't let him bother you... He's just a big bully! Come on! Let's..."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_EOLEO, ANIM_NOD);
	syFieldScriptWait(10);
	// "Waa waa..."
	syFieldScriptShowMessageThenWait(ACTOR_EOLEO, 0, 10);
	fdActorFaceDirection(ACTOR_CHAUCHA, FACING_EAST_NORTHEAST, 0);
	syFieldScriptWait(30);
	// "We'll be back again later. Will that be all right?"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_GUARD, ANIM_NOD);
	syFieldScriptWait(10);
	// "Yeah, yeah. That's fine."
	syFieldScriptShowMessageThenWait(ACTOR_GUARD, 0, 10);
	fdActorFaceDirection(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST, 0);
	syFieldScriptWait(20);
	// "We'll come back soon, dear."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
	syFieldScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
	syFieldScriptWait(30);
	fdActorFaceDirection(ACTOR_CHAUCHA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_EOLEO, FACING_EAST, 0);
	syFieldScriptWait(30);
	fdActorSetAnimation(ACTOR_CHAUCHA, ANIM_NOD);
	syFieldScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_EOLEO, ANIM_NOD);
	syFieldScriptWait(20);
	scFieldActorWatchAndSetScript(ACTOR_GUARD, ACTOR_EOLEO);
	fdActorWalkBy(ACTOR_CHAUCHA, 0, -16);
	fdActorWalkByAndWait(ACTOR_EOLEO, 0, -16);
	fdActorWalkBy(ACTOR_CHAUCHA, 52, 0);
	fdActorWalkByAndWait(ACTOR_EOLEO, 52, 0);
	syFieldScriptWait(15);
	scFieldActorStartRepeatedMotion(ACTOR_CHAUCHA, 3);
	fdActorSetAttachedEffect(ACTOR_CHAUCHA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(40);
	scFieldActorWatchAndSetScript(ACTOR_FELIX, ACTOR_EOLEO);
	scFieldActorSetSpritePriority(ACTOR_CHAUCHA, 2);
	scFieldActorSetSpritePriority(ACTOR_EOLEO, 2);
	fdActorSetScript(ACTOR_CHAUCHA, AlhafranCaveVisitorsLeaveScript);
	syFieldScriptWait(10);
	fdActorSetScript(ACTOR_EOLEO, AlhafranCaveVisitorsLeaveScript);
	syFieldScriptWait(10);
	fdActorWalkByAndWait(ACTOR_PIRATE_GUARDING_EOLEOS_SECRET, 16, 0);
	fdActorFaceDirection(ACTOR_PIRATE_GUARDING_EOLEOS_SECRET, FACING_SOUTH_SOUTHEAST, 0);
	fdActorWalkBy(ACTOR_PIRATE_ON_THE_BOAT, 0, 16);
	fdActorWalkByAndWait(ACTOR_PIRATE_BLAMING_THE_PARTY, 0, 16);
	syFieldScriptWait(5);
	fdActorFaceDirection(ACTOR_PIRATE_ON_THE_BOAT, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_PIRATE_BLAMING_THE_PARTY, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHEAST, 0);
	scFieldActorWaitForScriptEndById(ACTOR_EOLEO);
	scFieldActorStop(ACTOR_GUARD);
	scFieldActorStop(ACTOR_FELIX);
	fdActorSetPos(ACTOR_CHAUCHA, 0, 0);
	fdActorSetPos(ACTOR_EOLEO, 0, 0);
	fdActorSetMovementType(fdActorGet(ACTOR_GUARD), MOVEMENT_TYPE_TAKE_LEADER_LAYER);
	syScriptCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdScriptEnd();
}

// An actor's update: it takes the party leader's display layer.
void scAlhafranCaveTakeLeadersLayer(void)
{
	fdActorTakeLeaderLayer();
}

// The key's update while Eoleo's Psynergy holds it: its colours cycle.
void scAlhafranCaveCycleKeyColors(void)
{
	fdActorCycleSpriteColors();
}

#include "shared/pillar_place.h"

#include "shared/pound_sink_target.h"

#include "shared/actor_set_metatile_open.h"

// clang-format off
// The puddle Frost freezes into a pillar, and the flag set while it stands.
Sint16 AlhafranCavePuddles[] = {
	ACTOR_CAVE_PUDDLE, FLAG_ALHAFRAN_CAVE_PUDDLE_FROZEN, -1, 0,
};

Sint32 AlhafranCaveGuardStepsAsideScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE_BY(16, 0, 0),
	SC_SYNC,
	SC_MOVE_BY(0, 0, -60),
	SC_SYNC,
	SC_MOVE_BY(-4, 0, 0),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(1),
	SC_END,
};

Uint32 AlhafranCaveVisitorsLeaveScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(780, 0, 288),
	SC_SYNC,
	SC_MOVE_BY(0, 0, 12),
	SC_SYNC,
	SC_MOVE_BY(64, 0, 0),
	SC_SYNC,
	SC_MOVE_BY(32, 0, 32),
	SC_SYNC,
	SC_MOVE_BY(0, 0, 54),
	SC_SYNC,
	SC_END,
};

// The jail's stake that Pound sinks: its actor, its kind and the flag set once it is sunk.
Uint16 AlhafranCavePoundStakes[] = {
	ACTOR_JAIL_STAKE, 1, FLAG_JAIL_STAKE_POUNDED, ACTOR_LIST_END,
};

#include "shared/scene_entrances_default.c"

Uint32 SceneExits[] = {
	SCENE_EXITS(AlhafranCave),
	SCENE_EXIT_UNLESS(ALHAFRAN_CAVE_TO_ALHAFRA, Alhafra, ALHAFRA_FROM_ALHAFRAN_CAVE, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(2, AlhafranCave, 3, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(3, AlhafranCave, 2, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ALHAFRAN_CAVE_TO_ALHAFRA_JAIL_WEST, AlhafraJail, ALHAFRA_JAIL_FROM_ALHAFRAN_CAVE_WEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ALHAFRAN_CAVE_TO_ALHAFRA_JAIL_EAST, AlhafraJail, ALHAFRA_JAIL_FROM_ALHAFRAN_CAVE_EAST, EXIT_FLAG_NONE),
	SCENE_EXITS(AlhafraJail),
	SCENE_EXIT_UNLESS(ALHAFRA_JAIL_TO_ALHAFRAN_CAVE_WEST, AlhafranCave, ALHAFRAN_CAVE_FROM_ALHAFRA_JAIL_WEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ALHAFRA_JAIL_TO_ALHAFRAN_CAVE_EAST, AlhafranCave, ALHAFRAN_CAVE_FROM_ALHAFRA_JAIL_EAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ALHAFRA_JAIL_TO_ALHAFRA, Alhafra, ALHAFRA_FROM_ALHAFRA_JAIL, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(9, AlhafraJail, 10, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(10, AlhafraJail, 9, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

ACTOR_PLACE AlhafranCaveCaveActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 120, 0, 296, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 472, 0, 200, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 424, 0, 312, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WOODEN_SIGN, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 296, 0, 392, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WOODEN_SIGN, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 344, 0, 392, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 324, 0, 416, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE AlhafranCaveJailActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_TALL_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 256, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_WOODEN_SIGN, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 184, 0, 284, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WOODEN_SIGN, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 232, 0, 284, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 196, 0, 304, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JAIL_KEY, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 720, 0, 336, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 232, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 232, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BRIGGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 232, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 232, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 232, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_EOLEO, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 304, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHAUCHA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 296, FACING_NORTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 744, 0, 320, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 736, 0, 360, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

CUE AlhafranCaveCaveCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT, 5, CONDITION_ALWAYS, 5},
	{CUE_STEP, 20, CONDITION_ALWAYS, SCRIPT_REF(scAlhafranCaveLowerBridgeMetatiles)},
	{CUE_STEP, 21, CONDITION_ALWAYS, SCRIPT_REF(scAlhafranCaveRaiseBridgeMetatiles)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_CAVE_PUDDLE, FLAG_ALHAFRAN_CAVE_PUDDLE_FROZEN, SCRIPT_REF(scAlhafranCaveFreezePuddle)},
	{PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, SCRIPT_REF(scAlhafranCaveCastSand)},
	{CUE_TABLE_END},
};

CUE AlhafranCaveJailCues[] = {
	{CUE_EXIT, 6, CONDITION_ALWAYS, 6},
	{CUE_EXIT, 7, CONDITION_ALWAYS, 7},
	{CUE_EXIT, 8, CONDITION_ALWAYS, 8},
	{CUE_EXIT, 9, CONDITION_ALWAYS, 9},
	{CUE_EXIT, 10, CONDITION_ALWAYS, 10},
	{CUE_STEP, 20, FLAG_FAMILY_VISITED_BRIGGS, SCRIPT_REF(scAlhafranCaveChauchaAndEoleoVisit)},
	{CUE_TALK, ACTOR_BORED_GUARD, FLAG_ALHAFRA_MAST_CLEARED, MSGNO(MsgEverythingSSo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BORED_GUARD, FLAG_ALHAFRA_MAST_CLEARED, MSGNO(MsgIDonTKnow)},
	{CUE_TALK, ACTOR_BORED_GUARD, CONDITION_ALWAYS, MSGNO(MsgBeforeIKnew)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BORED_GUARD, CONDITION_ALWAYS, MSGNO(MsgNoMatterHow)},
	{CUE_TALK, ACTOR_GUARD, CONDITION_ALWAYS, MSGNO(MsgIVeBeen)},
	{CUE_TALK, ACTOR_BRIGGS, CONDITION_ALWAYS, SCRIPT_REF(scAlhafranCaveTalkToBriggs)},
	{CUE_TALK, ACTOR_PIRATE_WHO_HATES_PRISON, CONDITION_ALWAYS, MSGNO(MsgIHatePrison)},
	{CUE_TALK, ACTOR_PIRATE_ON_THE_BOAT, CONDITION_ALWAYS, MSGNO(MsgItSoundsLike)},
	{CUE_TALK, ACTOR_PIRATE_BLAMING_THE_PARTY, CONDITION_ALWAYS, MSGNO(MsgEverythingWouldHave)},
	{CUE_TALK, ACTOR_PIRATE_GUARDING_EOLEOS_SECRET, CONDITION_ALWAYS, MSGNO(MsgISawYou)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUARD, CONDITION_ALWAYS, MSGNO(MsgItSSo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BRIGGS, CONDITION_ALWAYS, MSGNO(MsgHasStrangePowers)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PIRATE_WHO_HATES_PRISON, CONDITION_ALWAYS, MSGNO(MsgIMNotStaying)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PIRATE_ON_THE_BOAT, CONDITION_ALWAYS, MSGNO(MsgIfTheyDon)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PIRATE_BLAMING_THE_PARTY, CONDITION_ALWAYS, MSGNO(MsgOhWhatS)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PIRATE_GUARDING_EOLEOS_SECRET, CONDITION_ALWAYS, MSGNO(MsgIHearSome)},
	{PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_JAIL_ROPE_LOW_POST, FLAG_JAIL_ROPE_TIED, 0},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_JAIL_STAKE, FLAG_JAIL_STAKE_POUNDED, SCRIPT_REF(scAlhafranCavePoundStake)},
	// The cave's Frost row, repeated in the jail's table.
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_CAVE_PUDDLE, FLAG_ALHAFRAN_CAVE_PUDDLE_FROZEN, SCRIPT_REF(scAlhafranCaveFreezePuddle)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_CRATE, CONDITION_ALWAYS, SCRIPT_REF(scAlhafranCaveStartMovingCrate)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CRATE, CONDITION_ALWAYS, SCRIPT_REF(scAlhafranCaveFinishMovingCrate)},
	{CUE_TABLE_END},
};
