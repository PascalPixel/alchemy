// Lunpa, the town the righteous thief Lunpa founded, and the Lunpa Fortress Gates above it. While
// Dodonpa, Donpa's son, holds Hammet in Lunpa Fortress, his men keep the town sealed: two guards
// stand outside the shut gate in the log palisade at the south end, where the road from the World
// Map ends, and turn the party away. The way in is through West Lunpa Cave, whose gate Frost opens;
// it comes out at the cave mouth in the town's north-west cliff (metatile (7, 4)).
//
// Two areas share this overlay, each a scene of its own:
//
// - Lunpa (SceneIdLunpa): the town. Doors lead into three homes, the weapon
//       and armor shop, the item shop, the inn, the temple and the crate room
// (RockRooms, rock_rooms.c). The road out of the top of the town (metatiles (13, 0) to (16, 0))
// climbs to the Lunpa Fortress Gates.
// - Lunpa Fortress Gates (SceneIdLunpaFortressGates): the slope up to the
//       golden fortress door in the cliff. Two log palisades cross it, each
// with a gap in the middle, and two guards stand in the yard between them, just
//       below the upper gap. The party slips past them under Cloak.
//
// The getters choose by MainRoutineStateWork.next. The town's cues also choose by
// FLAG_DODONPA_LOCKED_UP, which Donpa's judgment in the fortress sets. Before it the townspeople
// live under Dodonpa's men (LunpaSealedCues). After it they talk of the intruders in the fortress
// (LunpaReopenedCues). Both tables share the roads and doors (LUNPA_ROADS_AND_DOORS) and Reveal's
// secrets (LUNPA_SECRETS).
//
// Reveal turns the town grey and shows three secrets (scLunpaRevealSecrets): a blue puddle between
// the two log pillars below the road to the gates, which Frost freezes into an ice pillar; a
// sparkle on the cliff at the west edge of town (metatile (3, 16)), the switch that opens the
// passage to Lunpa's hidden room, searched from metatile (3, 18) below it; and a Psynergy Stone,
// found by searching metatile (7, 16), south of the inn. Every entry and the end of Reveal hide
// them again (scLunpaHideSecrets). Searching the sparkle opens a dark cave mouth in the cliff
// beside it, and the passage can be walked into only while Reveal shows it
// (scLunpaEnterHiddenPassage). The room beyond (StoneRooms) holds six chests.
//
// At the gates, walking uncloaked into the yard before the guards
// (METATILE_BEHAVIOR_LUNPA_FORTRESS_GATES_GUARDED_GROUND) makes them step into the upper gap and
// warn the party off. An uncloaked leader in that gap, or one still in the yard as Cloak fades, is
// caught and marched back down through the lower palisade (scLunpaGuardsCatchParty).
//
// A party caught inside the fortress is thrown out by the guards and down the slope
// (scLunpaGuardsThrowIsaacOut). After the rescue it comes out of the fortress door and Isaac sneaks
// past the guards under Cloak (scLunpaLeaderSneaksOut).
//
// Every arrival at the gates clears the fortress's puzzle and beaten-guard flags, so the fortress
// starts over.
//
// Between Donpa's judgment and the meeting with Bunza outside West Lunpa Cave
// (FLAG_LUNPA_CAVE_REUNION_SEEN), Garet stops Isaac from going back up through the upper gap
// (scLunpaGaretTurnsIsaacBack).

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "sounds.h"
#include "field/field_effect.h"
#include "scene/floating_nut.h"
#include "scene/catch_nut.h"
#include "game/system_call.h"
#include "text/msg_ids.h"

extern SCENE_ENTRANCE LunpaEntrances[];
extern SCENE_ENTRANCE GateEntrances[];
extern Uint32         LunpaExits[];
extern Uint32         GateExits[];
extern ACTOR_PLACE    LunpaActorPlacements[];
extern ACTOR_PLACE    GateActorPlacements[];
extern CUE            LunpaSealedCues[];
extern CUE            LunpaReopenedCues[];
extern CUE            GateCues[];

// Lunpa's and the gates' entrances and exits are in scene_exits.h.

// Unused: the exit of a third road out of the gates
// (METATILE_BEHAVIOR_LUNPA_FORTRESS_GATES_THIRD_ROAD), which no metatile of the gates' map has.
// GateExits has no row for it either: gmTakeExit, reaching the end of the gates' exits, would fall
// back to the script of their first exit at ENTRANCE_DEFAULT, LunpaFortressGroundFloor's default
// entrance.
enum
{
    GATE_EXIT_THIRD = 3
};

// The first of the rooms in RockRooms that Lunpa's doors lead into.
enum
{
    ROOM_ENTRANCE_FIRST = 1
};

// The RockRooms entrance a door's exit leads to.
#define ROOM_ENTRANCE(exit) ((exit) - LUNPA_TO_FIRST_HOME + ROOM_ENTRANCE_FIRST)

// The metatile behaviors Lunpa's cues answer.
typedef enum LUNPA_METATILE_BEHAVIOR
{
    METATILE_BEHAVIOR_LUNPA_WORLD_MAP_ROAD = 1,
    METATILE_BEHAVIOR_LUNPA_GATE_ROAD,
    METATILE_BEHAVIOR_LUNPA_CAVE_ROAD,
    METATILE_BEHAVIOR_LUNPA_FIRST_HOME_DOOR,
    METATILE_BEHAVIOR_LUNPA_SECOND_HOME_DOOR,
    METATILE_BEHAVIOR_LUNPA_THIRD_HOME_DOOR,
    METATILE_BEHAVIOR_LUNPA_ARMS_SHOP_DOOR,
    METATILE_BEHAVIOR_LUNPA_ITEM_SHOP_DOOR,
    METATILE_BEHAVIOR_LUNPA_INN_DOOR,
    METATILE_BEHAVIOR_LUNPA_TEMPLE_DOOR,
    METATILE_BEHAVIOR_LUNPA_CRATE_ROOM_DOOR,
    METATILE_BEHAVIOR_LUNPA_HIDDEN_PASSAGE,
    METATILE_BEHAVIOR_LUNPA_HIDDEN_SWITCH    = 20,
    METATILE_BEHAVIOR_LUNPA_WELL             = 90,
    METATILE_BEHAVIOR_LUNPA_GRAVE_SLEEP_BOMB = 100,
    METATILE_BEHAVIOR_LUNPA_BARREL_NUT,
    METATILE_BEHAVIOR_LUNPA_PSYNERGY_STONE
} LUNPA_METATILE_BEHAVIOR;

// The metatile behaviors the Lunpa Fortress Gates' cues answer. The gates' map has no metatile of
// the third road, the fortress approach or the well: the approach is raised by
// scLunpaWatchForFortressApproach instead, and the other two rows never fire.
typedef enum LUNPA_FORTRESS_GATES_METATILE_BEHAVIOR
{
    METATILE_BEHAVIOR_LUNPA_FORTRESS_GATES_FORTRESS_DOOR = 1,
    METATILE_BEHAVIOR_LUNPA_FORTRESS_GATES_LUNPA_ROAD,
    METATILE_BEHAVIOR_LUNPA_FORTRESS_GATES_THIRD_ROAD,
    METATILE_BEHAVIOR_LUNPA_FORTRESS_GATES_GUARDED_GROUND    = 10,
    METATILE_BEHAVIOR_LUNPA_FORTRESS_GATES_FORTRESS_APPROACH = 20,
    METATILE_BEHAVIOR_LUNPA_FORTRESS_GATES_WELL              = 90
} LUNPA_FORTRESS_GATES_METATILE_BEHAVIOR;

// The script the gates' guards raise on spotting the uncloaked party.
typedef enum LUNPA_FORTRESS_GATES_RAISED_CUE
{
    RAISED_PARTY_SPOTTED = 91
} LUNPA_FORTRESS_GATES_RAISED_CUE;

// Lunpa's doors that open as the leader walks in (LunpaDoors), counted from the first home's; at
// the temple's he goes through without first stepping into the doorway.
enum
{
    DOOR_COUNT  = 8,
    DOOR_TEMPLE = METATILE_BEHAVIOR_LUNPA_TEMPLE_DOOR - METATILE_BEHAVIOR_LUNPA_FIRST_HOME_DOOR
};

// Lunpa's placed actors, numbered in the order of LunpaActorPlacements from ACTOR_FIRST_PLACED. The
// townspeople are named by their sprites and by what they say before Donpa's judgment.
typedef enum LUNPA_ACTOR_ID
{
    // The blue puddle Reveal shows and Frost freezes into an ice pillar.
    ACTOR_HIDDEN_PUDDLE = ACTOR_FIRST_PLACED,
    // A man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR).
    ACTOR_MAN_ASKING_HOW_LONG_IN_LUNPA,
    // A woman with fair hair in a white apron (SPRITE_WOMAN_WITH_FAIR_HAIR).
    ACTOR_WOMAN_WITH_NO_FREEDOM,
    // A stout man with brown hair (SPRITE_STOUT_MAN).
    ACTOR_MAN_ON_HAMMETS_KIDNAPPING,
    // A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS).
    ACTOR_WOMAN_SAYING_DONPA_IS_TOO_OLD,
    // A bent old man with a cane (SPRITE_OLD_MAN_WITH_CANE), who wanders.
    ACTOR_OLD_MAN_ON_LUNPA_AND_DONPA,
    // An old woman with white hair (SPRITE_OLD_WOMAN).
    ACTOR_OLD_WOMAN_ON_DONPA,
    // A small boy with fair hair (SPRITE_BOY_WITH_FAIR_HAIR).
    ACTOR_BOY_ASKING_TO_SEE_DONPA,
    // A little girl with orange pigtails (SPRITE_GIRL_WITH_PIGTAILS), who wanders.
    ACTOR_GIRL_ON_THE_CHEFS_MEALS,
    // The two soldiers in spiked helmets (SPRITE_SOLDIER_IN_SPIKED_HELMET); this one keeps the road
    // in from the World Map.
    ACTOR_WEST_GUARD,
    ACTOR_EAST_GUARD, // wants Dodonpa's authorization
                      // The hidden switch's sparkle, shown by Reveal.
    ACTOR_SWITCH_SPARKLE,
    // The Nut that floats over the town, for Catch.
    ACTOR_FLOATING_NUT
} LUNPA_ACTOR_ID;

// The Lunpa Fortress Gates' placed actors, numbered in the order of GateActorPlacements from
// ACTOR_FIRST_PLACED. Garet's row, the third, takes his own actor, GARET, as every row of a party
// member's sprite does.
typedef enum GATE_ACTOR_ID
{
    ACTOR_LEFT_GUARD = ACTOR_FIRST_PLACED,
    ACTOR_RIGHT_GUARD
} GATE_ACTOR_ID;

// How the switch's sparkle is stretched while Reveal shows it.
enum
{
    SPARKLE_SCALE_X = toFIXED(6) / 5,
    SPARKLE_SCALE_Y = toFIXED(3) / 2
};

// Where Retreat at the gates sends the party (MAIN_ROUTINE_STATE.retreatEntrance).
enum
{
    GATE_RETREAT_ENTRANCE = 10
};

// The gate guards' lines when they catch the party uncloaked, in order.
typedef enum CATCH_MESSAGE
{
    CATCH_LEFT_GUARD_CHALLENGES,
    CATCH_RIGHT_GUARD_WONDERS,
    CATCH_LEFT_GUARD_REFUSES_ENTRY,
    CATCH_RIGHT_GUARD_SENDS_PARTY_OFF
} CATCH_MESSAGE;

// The left guard's two lines once he has seen Hammet with the party.
typedef enum RECOGNITION_MESSAGE
{
    RECOGNITION_SEEN_THAT_MAN,
    RECOGNITION_IMPOSSIBLE
} RECOGNITION_MESSAGE;

// The leader lying where the guards threw him down the steps.
enum
{
    ANIM_SPRAWLED = 19
};

// How high the ice pillar's top is, and a metatile: 16 pixels each.
enum
{
    PILLAR_TOP_HEIGHT = toFIXED(16),
    METATILE_SIZE     = toFIXED(16)
};

// The treasure spot of the town's Psynergy Stone.
enum
{
    TREASURE_SPOT_PSYNERGY_STONE = 102
};

extern MAP_GRID_METATILE_ANIMATION LunpaDoors[];

static void                        scLunpaWatchForUncloakedParty(void);

// Unused: nothing in the scene reads them.
enum
{
    PALETTE_TRANSLUCENT_EFFECT = 14,
    PALETTE_EFFECT             = 15
};

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"

// Frost has frozen the hidden puddle into an ice pillar to stand on.
#define FLAG_LUNPA_PUDDLE_FROZEN                      FLAG_SCENE_TEMP_00
// The town's secrets are hidden: Reveal is not shining.
#define FLAG_LUNPA_SECRETS_HIDDEN                     FLAG_SCENE_TEMP_01
// The hidden switch has opened the passage to the hidden room.
#define FLAG_LUNPA_HIDDEN_PASSAGE_OPEN                FLAG_SCENE_TEMP_02
// The guards have stepped in front of the fortress door.
#define FLAG_LUNPA_FORTRESS_GATES_GUARDS_AT_DOOR      FLAG_SCENE_TEMP_40
// Cloak has been cast at the gates and has not yet faded.
#define FLAG_LUNPA_FORTRESS_GATES_CLOAK_CAST          FLAG_SCENE_TEMP_41
// The leader is going through the fortress door.
#define FLAG_LUNPA_FORTRESS_GATES_ENTERING_FORTRESS   FLAG_SCENE_TEMP_42
// Garet's warning against going back up to the fortress is under way.
#define FLAG_LUNPA_FORTRESS_GATES_GARET_WARNING       FLAG_SCENE_TEMP_43
// The guards have caught the party and are marching it down the steps.
#define FLAG_LUNPA_FORTRESS_GATES_GUARDS_CAUGHT_PARTY FLAG_SCENE_TEMP_44

#include "shared/field_effect_duration_scripts.c"

const Sint32 *FieldEffectDurationScripts[] = {
    FieldEffectShortScript,
    FieldEffectMediumScript,
    FieldEffectLongScript,
};

const SCENE_ENTRANCE *scGetEntrances(void)
{
    Sint32 sceneId = MainRoutineStateWork.next;

    if (sceneId == SCENE_LUNPA)
    {
        return LunpaEntrances;
    }
    if (sceneId == SCENE_LUNPA_FORTRESS_GATES)
    {
        return GateEntrances;
    }
    return LunpaEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
    return NULL;
}

const Uint32 *scGetExits(void)
{
    Sint32 sceneId = MainRoutineStateWork.next;

    if (sceneId == SCENE_LUNPA)
    {
        return LunpaExits;
    }
    if (sceneId == SCENE_LUNPA_FORTRESS_GATES)
    {
        return GateExits;
    }
    return LunpaExits;
}

// The townsfolk of Lunpa, or the guards of the Lunpa Fortress Gates.
const ACTOR_PLACE *scGetActorPlacements(void)
{
    Sint32 sceneId = MainRoutineStateWork.next;

    if (sceneId == SCENE_LUNPA)
    {
        return LunpaActorPlacements;
    }
    if (sceneId == SCENE_LUNPA_FORTRESS_GATES)
    {
        return GateActorPlacements;
    }
    return LunpaActorPlacements;
}

// Catch on the Nut in the tree just west of Lunpa's shut south gate, outside the palisade, brings
// it down to Isaac.
static void scLunpaCatchFloatingNut(void)
{
    scCatchNut(ACTOR_FLOATING_NUT, FLAG_LUNPA_NUT_CAUGHT);
}

// Makes the frozen puddle's metatile, (14, 4), and the metatiles either side of it into the ice
// pillar's top and sides, from copies kept at (17, 4) and (15, 3).
static inline void scLunpaApplyFrozenPuddleCollision(void)
{
    fdMapGridCopyMetatileAttributes(17, 4, 1, 1, 14, 4);
    fdMapGridCopyMetatileAttributes(15, 3, 1, 1, 15, 4);
    fdMapGridCopyMetatileAttributes(15, 3, 1, 1, 13, 4);
}

// Frost on the blue puddle that Reveal shows between the two log pillars at the top of Lunpa
// (metatile (14, 4)) freezes it into an ice pillar to stand on.
static void scLunpaFreezeHiddenPuddle(void)
{
    ACTOR *pillar;

    pillar = fdActorGet(ACTOR_HIDDEN_PUDDLE);
    // DEAD: the leader looked up and dropped; the ROM keeps the call.
    fdActorGet(ACTOR_PARTY_LEADER);
    scLunpaApplyFrozenPuddleCollision();
    if (pillar != NULL)
    {
        fdActorSetSpriteFlags(pillar, 0);
        pillar->motionFlags   = ACTOR_FALLS;
        pillar->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
    }
    gmFlagSet(FLAG_LUNPA_PUDDLE_FROZEN);
}

// Every tick while Reveal shows the puddle: once Isaac is up on the ice pillar, it is drawn under
// him.
static void scLunpaUpdateIcePillarDrawOrder(void)
{
    if (fdActorGet(ACTOR_PARTY_LEADER)->y.fixed >= PILLAR_TOP_HEIGHT)
    {
        fdActorGet(ACTOR_HIDDEN_PUDDLE)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    }
    else
    {
        fdActorGet(ACTOR_HIDDEN_PUDDLE)->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
    }
}

// Draws one picture of the hidden passage's cave mouth in the cliff at the west edge of town, from
// a set of copies kept at (srcColumn, srcRow). The second and third copies are the same mouth on
// the map's other layers.
static inline void scLunpaDrawHiddenPassagePicture(Sint32 srcColumn, Sint32 srcRow)
{
    fdMapGridCopyMetatiles(srcColumn, srcRow, 3, 4, 1, 14);
    fdMapGridCopyMetatiles(srcColumn + 3, srcRow, 3, 4, 33, 14);
    fdMapGridCopyMetatiles(srcColumn + 6, srcRow, 3, 4, 1, 46);
}

// Casting Reveal in Lunpa turns the town grey and shows its secrets: the blue puddle between the
// log pillars at the top of town, the sparkle of the hidden switch on the west cliff (or, once it
// has been pressed, the cave mouth it opened), and the Psynergy Stone's spot at metatile (7, 16).
// Isaac standing on that spot is first walked a step south off it.
static void scLunpaRevealSecrets(void)
{
    ACTOR *actor;
    Sint32 metatileX;
    Sint32 metatileZ;

    actor     = fdActorGet(ACTOR_PARTY_LEADER);
    metatileX = actor->x.fixed / METATILE_SIZE;
    metatileZ = actor->z.fixed / METATILE_SIZE;
    if (gmFlagGet(FLAG_LUNPA_PSYNERGY_STONE) == 0)
    {
        if (metatileX == 7 && metatileZ == 16)
        {
            fdActorWalkByAndWait(ACTOR_PARTY_LEADER, 0, 16);
        }
        fdTreasureSpotSetPos(TREASURE_SPOT_PSYNERGY_STONE, -1, -1);
        // The stone's metatile takes the attributes kept for it at (28, 31).
        fdMapGridCopyMetatileAttributes(28, 31, 1, 1, 7, 16);
    }
    // The puddle's metatile on the middle layer (the map's columns from 32), and the metatiles
    // round it, (13, 3) to (15, 5), take the copies kept for Reveal.
    fdMapGridCopyMetatiles(47, 4, 1, 1, 46, 4);
    fdMapGridCopyMetatileAttributes(34, 37, 3, 3, 13, 3);
    fdActorSetPos(ACTOR_HIDDEN_PUDDLE, 232, 72);
    fdActorGet(ACTOR_HIDDEN_PUDDLE)->y.fixed = 0;
#if LUNPA_REVEAL_KEEPS_ICE_PILLAR_COLLISION
    if (gmFlagGet(FLAG_LUNPA_PUDDLE_FROZEN))
    {
        scLunpaApplyFrozenPuddleCollision();
    }
#endif
    // The opened passage replaces the sparkle; otherwise show the hidden switch.
    if (gmFlagGet(FLAG_LUNPA_HIDDEN_PASSAGE_OPEN) != 0)
    {
        scLunpaDrawHiddenPassagePicture(41, 49);
    }
    else
    {
        fdActorSetPos(ACTOR_SWITCH_SPARKLE, 56, 268);
        fdActorSetSpriteFlags(fdActorGet(ACTOR_SWITCH_SPARKLE), 0);
        actor = fdActorGet(ACTOR_SWITCH_SPARKLE);
        if (actor != NULL)
        {
            actor->motionFlags   = ACTOR_FLOATS_HIGH;
            actor->y.fixed       = toFIXED(16);
            actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
            actor->scaleX        = SPARKLE_SCALE_X;
            actor->scaleY        = SPARKLE_SCALE_Y;
        }
    }
    sySetTickEvent(scLunpaUpdateIcePillarDrawOrder, EVENT_PASS_TICK);
    gmFlagClear(FLAG_LUNPA_SECRETS_HIDDEN);
}

// As Reveal ends, and on every arrival in Lunpa, the puddle, the sparkle, the cave mouth and the
// Psynergy Stone's spot are hidden again, and a frozen puddle thaws.
static void scLunpaHideSecrets(void)
{
    // DEAD: the leader looked up and dropped; the ROM keeps the call.
    fdActorGet(ACTOR_PARTY_LEADER);
    fdActorSetPos(ACTOR_SWITCH_SPARKLE, 0, 0);
    fdActorSetPos(ACTOR_HIDDEN_PUDDLE, 0, 0);
    // The puddle's metatile and the metatiles round it as they are without it, from the copies kept
    // at (38, 38) and (37, 37), whose attributes (14, 2) takes too.
    fdMapGridCopyMetatiles(38, 38, 1, 1, 46, 4);
    fdMapGridCopyMetatileAttributes(37, 37, 3, 3, 13, 3);
    fdMapGridCopyMetatileAttributes(37, 37, 1, 1, 14, 2);
    // The stone's metatile plain again, as the metatile east of it.
    fdMapGridCopyMetatileAttributes(8, 16, 1, 1, 7, 16);
    fdTreasureSpotSetPos(TREASURE_SPOT_PSYNERGY_STONE, 0, 0);
    // The cave mouth's lower metatiles, (1, 15) to (3, 16), shut again from the copy kept at (32,
    // 42).
    fdMapGridCopyMetatiles(32, 42, 3, 2, 1, 15);
    gmFlagClear(FLAG_LUNPA_PUDDLE_FROZEN);
    fdActorSetAnimation(ACTOR_HIDDEN_PUDDLE, ANIM_STAND);
    fdActorGet(ACTOR_HIDDEN_PUDDLE)->update = NULL;
    fdActorSetColorMode(fdActorGet(ACTOR_HIDDEN_PUDDLE), 0);
    syCloseEvent(scLunpaUpdateIcePillarDrawOrder);
    gmFlagSet(FLAG_LUNPA_SECRETS_HIDDEN);
}

// Searching the well in the middle of Lunpa (metatiles (16, 13) to (17, 14)), Isaac peers in and
// hears frogs inside. The gates' table has a row for a well too, but no metatile there answers it.
static void scLunpaSearchWell(void)
{
    fdScriptStart();
    mnFieldMessageShow(MsgFieldPeeredWell, 1);
    mnFieldMessageShow(MsgLunpaWellFrogs, 1);
    fdScriptEnd();
}

// The cues of the Lunpa Fortress Gates, or the town's before and after Donpa's judgment.
const CUE *scGetCues(void)
{
    Sint32 sceneId = MainRoutineStateWork.next;

    if (sceneId == SCENE_LUNPA_FORTRESS_GATES)
    {
        // DEAD: the flag read and dropped; the ROM keeps the call.
        gmFlagGet(FLAG_DODONPA_LOCKED_UP);
        return GateCues;
    }
    if (sceneId == SCENE_LUNPA && gmFlagGet(FLAG_DODONPA_LOCKED_UP) != 0)
    {
        return LunpaReopenedCues;
    }
    return LunpaSealedCues;
}

// Before Donpa's judgment, the guard on the left of Lunpa's shut south gate winces and asks whether
// the party wants to enter Lunpa, and sends them back where they came from whatever they answer.
static void scLunpaTalkToWestGuard(void)
{
    fdScriptStart();
    fdActorShowEmote(ACTOR_WEST_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    fdScriptSetMessageId(MsgLunpaWestGuardAsksAboutEntering);
    // "What's with you? You wanna enter Lunpa?" Yes: "Well, you can't! Go back to where you came
    // from!" No: "You came all this way for nothing? That doesn't make any..."
    fdScriptAskYesNo(ACTOR_WEST_GUARD, 0);
    fdScriptEnd();
}

// Before Donpa's judgment, the guard on the right of Lunpa's shut south gate says no one may enter
// without Dodonpa's authorization. Once Hammet's caravan has gone north to Lunpa
// (FLAG_LUNPA_GUARDS_SUSPECT_KALAY), he asks instead whether the party is from Kalay, and then
// whether they have come to save Hammet, and warns them off by their answers.
static void scLunpaTalkToEastGuard(void)
{
    fdScriptStart();
    if (gmFlagGet(FLAG_LUNPA_GUARDS_SUSPECT_KALAY) == 0)
    {
        fdScriptSetMessageId(MsgLunpaEastGuardDemandsAuthorization);
        // "This is Lunpa. You cannot enter without Dodonpa's authorization."
        fdScriptShowMessage(ACTOR_EAST_GUARD, 0);
    }
    else
    {
        fdScriptSetMessageId(MsgLunpaEastGuardAsksIfFrom);
        // "You're not from Kalay, are you?"
        fdScriptOpenMessage(ACTOR_EAST_GUARD, 0);
        if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
        {
            FieldStateWork->msg++;
            // "You're not here to save Hammet, are you?"
            fdScriptOpenMessage(ACTOR_EAST_GUARD, 0);
            if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1)
            {
                FieldStateWork->msg++;
            }
        }
        fdScriptShowMessage(ACTOR_EAST_GUARD, 0);
    }
    fdScriptEnd();
}

// Mind Read on the guard on the right of Lunpa's south gate. Before Donpa's judgment he fears
// Dodonpa will blame the guards for any trouble outsiders cause, or, once Hammet's caravan has
// come, trusts that the gate in the cave opens only from the inside; after it he thinks the
// merchant is harmless.
static void scLunpaReadEastGuardsThoughts(void)
{
    Sint32 thoughts;

    if (gmFlagGet(FLAG_DODONPA_LOCKED_UP) == 0)
    {
        thoughts = gmFlagGet(FLAG_LUNPA_GUARDS_SUSPECT_KALAY);
        if (thoughts == 0)
        {
            // "If outsiders cause any trouble in the village, Dodonpa will..."
            thoughts = MsgLunpaEastGuardFearsBlame;
        }
        else
        {
            // "Even if they enter through the cave, they can't get into..."
            thoughts = MsgLunpaEastGuardTrustsCaveGate;
        }
        fdScriptSetMessageId(thoughts);
        fdScriptShowMessage(ACTOR_EAST_GUARD, 0);
    }
    else
    {
        fdScriptSetMessageId(MsgLunpaEastGuardThinksMerchantHarmless);
        // "That merchant was desperate to do business in the village...."
        fdScriptShowMessage(ACTOR_EAST_GUARD, 0);
    }
}

// Before Donpa's judgment, a young man with brown hair near the inn asks whether the party has been
// in Lunpa long, and takes a No to mean they are new members of Dodonpa's gang.
static void scLunpaAskAboutLengthOfStay(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgLunpaVillagerAAsksHowLong);
    // "Have you been in Lunpa long?" Yes: "Hmm... Your face doesn't ring a bell. I guess you
    // don't..." No: "Oh, so you're a new member of Dodonpa's gang?"
    fdScriptAskYesNo(ACTOR_MAN_ASKING_HOW_LONG_IN_LUNPA, 0);
    fdScriptEnd();
}

// Before Donpa's judgment, a stout man in Lunpa asks whether it is true that Dodonpa kidnapped
// Hammet and extorted ransom for him.
static void scLunpaAskAboutHammetsKidnapping(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgLunpaVillagerCAsksAboutKidnapping);
    // "Is it true that Dodonpa kidnapped Hammet and extorted ransom..." Yes: "Then I guess
    // Dodonpa's men were telling the truth." No: "I didn't think so. Not even Dodonpa would do such
    // a terrible..."
    fdScriptAskYesNo(ACTOR_MAN_ON_HAMMETS_KIDNAPPING, 0);
    fdScriptEnd();
}

// Before Donpa's judgment, a small boy at the top of Lunpa asks whether the party wants to see
// Donpa: he is in the fortress, or, on a No, impossible to see now.
static void scLunpaAskAboutSeeingDonpa(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgLunpaVillagerGAsksAboutDonpa);
    // "You want to see Donpa?" Yes: "Donpa is in the fortress. You'll have to go there if you..."
    // No: "I didn't think so... It's impossible to see Donpa now."
    fdScriptAskYesNo(ACTOR_BOY_ASKING_TO_SEE_DONPA, 0);
    fdScriptEnd();
}

// After Donpa's judgment, a woman with fair hair in Lunpa shivers and complains that it has gotten
// colder.
static void scLunpaComplainAboutCold(void)
{
    fdScriptStart();
    fdActorRunRepeatedMotion(ACTOR_WOMAN_WITH_NO_FREEDOM, 3);
    fdScriptSetMessageId(MsgLunpaVillagerBShivers);
    // "It's gotten colder... I'm shiverin' just standin' here."
    fdScriptShowMessage(ACTOR_WOMAN_WITH_NO_FREEDOM, 0);
    fdScriptEnd();
}

// After Donpa's judgment, a woman in a green dress in Lunpa offers to tell what caused the
// commotion at the fortress: someone snuck in to save Hammet and ran away.
static void scLunpaAskAboutFortressCommotion(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgLunpaVillagerDAsksAboutCommotion);
    // "Do you want to know what caused the commotion at the fortress?" Yes: "Someone snuck into the
    // fortress to save the merchant Hammet!..." No: "They're foolhardy to invade such a well-
    // guarded fortress!..."
    fdScriptAskYesNo(ACTOR_WOMAN_SAYING_DONPA_IS_TOO_OLD, 0);
    fdScriptEnd();
}

// Talking to the left guard at the Lunpa Fortress Gates under Cloak, he could swear he just heard
// someone. Uncloaked, he grumbles that Dodonpa is devious; after Donpa's judgment, until the
// meeting with Bunza, he thinks he has seen the man with the party before, then decides it is
// impossible.
static void scLunpaTalkToLeftGuard(void)
{
    Sint32 recognitionMsgBase;

    if (MainRoutineStateWork.cloaked != 0)
    {
        fdScriptSetMessageId(MsgLunpaLeftGuardHearsSomeone);
    }
    else if (gmFlagGet(FLAG_DODONPA_LOCKED_UP) != 0 && gmFlagGet(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0)
    {
        fdActorShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
        recognitionMsgBase = MsgLunpaLeftGuardRecognizesHammet;
        fdScriptSetMessageId(recognitionMsgBase + RECOGNITION_SEEN_THAT_MAN);
        fdScriptShowMessage(ACTOR_LEFT_GUARD, 0);
        fdActorStartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
        fdScriptSetMessageId(recognitionMsgBase + RECOGNITION_IMPOSSIBLE);
        gmFlagSet(FLAG_LUNPA_GATE_GUARD_SAW_HAMMET);
    }
    else
    {
        fdScriptSetMessageId(MsgLunpaLeftGuardResentsDodonpa);
    }
    fdScriptShowMessage(ACTOR_LEFT_GUARD, 0);
}

// Talking to the right guard at the Lunpa Fortress Gates, he boasts that not even a mouse could get
// in while he is on watch. Under Cloak he answers his partner that the place is kind of creepy;
// after Donpa's judgment he wonders how the warriors from Kalay got in.
static void scLunpaTalkToRightGuard(void)
{
    if (MainRoutineStateWork.cloaked != 0)
    {
        fdScriptSetMessageId(MsgLunpaRightGuardFeelsCreepy);
    }
    else if (gmFlagGet(FLAG_DODONPA_LOCKED_UP) != 0)
    {
        fdScriptSetMessageId(MsgLunpaRightGuardWondersHow);
    }
    else
    {
        fdScriptSetMessageId(MsgLunpaRightGuardBoasts);
    }
    fdScriptShowMessage(ACTOR_RIGHT_GUARD, 0);
}

// Mind Read on the left guard at the Lunpa Fortress Gates: he thinks keeping Hammet to squeeze
// money out of Kalay is genius. After Donpa's judgment, until the meeting with Bunza, he turns to
// Isaac sure he has seen that man with him before (later, that the man following them looks like
// that merchant), then dismisses the thought.
static void scLunpaReadLeftGuardsThoughts(void)
{
    Sint32 recognizedHammet;

    if (MainRoutineStateWork.cloaked == 0 && gmFlagGet(FLAG_DODONPA_LOCKED_UP) != 0
        && gmFlagGet(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0)
    {
        recognizedHammet = fdScriptGetFlag(FLAG_LUNPA_GATE_GUARD_SAW_HAMMET);
        if (recognizedHammet == 0)
        {
            FieldStateWork->psynergy = recognizedHammet;
            fdPsynergyStopMindReadWave();
            fdActorFaceActor(ACTOR_LEFT_GUARD, ACTOR_PARTY_LEADER, 0);
            fdActorShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
            fdScriptSetMessageId(MsgLunpaLeftGuardRecognizesHammet);
            gmFlagSet(FLAG_LUNPA_GATE_GUARD_SAW_HAMMET);
        }
        else
        {
            fdScriptSetMessageId(MsgLunpaLeftGuardGlimpsedMerchant);
        }
        fdScriptShowMessage(ACTOR_LEFT_GUARD, 0);
        fdActorStartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
        fdScriptSetMessageId(MsgLunpaLeftGuardDismissesThought);
    }
    else
    {
        fdScriptSetMessageId(MsgLunpaLeftGuardThoughts);
    }
    fdScriptShowMessage(ACTOR_LEFT_GUARD, 0);
}

// Mind Read on the right guard at the Lunpa Fortress Gates: you would have to be a shadow to sneak
// past him. After Donpa's judgment he can't see how anyone got by them.
static void scLunpaReadRightGuardsThoughts(void)
{
    if (gmFlagGet(FLAG_DODONPA_LOCKED_UP) != 0)
    {
        fdScriptSetMessageId(MsgLunpaRightGuardReopenedThoughts);
    }
    else
    {
        fdScriptSetMessageId(MsgLunpaYoudShadowSneak);
    }
    fdScriptShowMessage(ACTOR_RIGHT_GUARD, 0);
}

// Every tick at the Lunpa Fortress Gates between Donpa's judgment and the meeting with Bunza: Isaac
// stepping into the upper palisade's gap (map row 10) raises Garet's warning,
// scLunpaGaretTurnsIsaacBack.
static void scLunpaWatchForFortressApproach(void)
{
    Sint32 leaderRow = fdActorGet(ACTOR_PARTY_LEADER)->z.fixed / METATILE_SIZE;

    if (fdScriptGetFlag(FLAG_LUNPA_FORTRESS_GATES_GARET_WARNING) == 0 && leaderRow == 10)
    {
        gmFlagSet(FLAG_LUNPA_FORTRESS_GATES_GARET_WARNING);
        FieldStateWork->stepCue = METATILE_BEHAVIOR_LUNPA_FORTRESS_GATES_FORTRESS_APPROACH;
    }
}

// After Donpa's judgment, walking up into the gap in the upper palisade at the Lunpa Fortress
// Gates, Garet reminds Isaac they can't go in there, and Isaac winces and steps back.
static void scLunpaGaretTurnsIsaacBack(void)
{
    fdScriptStart();
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    fdScriptSetMessageId(MsgLunpaGaretRefusesToReturn);
    // "Isaac! Don't be stupid! We can't go in there, remember?"
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 100);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, 12);
    fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    gmFlagClear(FLAG_LUNPA_FORTRESS_GATES_GARET_WARNING);
    fdScriptEnd();
}

// Walking north into one of Lunpa's doors, it swings open and Isaac goes through into the home,
// shop, inn, temple or crate room behind it. The townspeople stand still meanwhile.
static void scLunpaEnterDoor(void)
{
    FIELD_STATE *state;
    ACTOR       *placedActor;
    Uint32       actorId;
    Sint32       doorIndex;

    state = FieldStateWork;
    fdScriptStart();
    // Freeze placed actors before opening the selected door and crossing it.
    for (actorId = ACTOR_FIRST_PLACED; actorId <= ACTOR_LAST_ID; actorId++)
    {
        placedActor = fdActorGet(actorId);
        if (placedActor != NULL)
        {
            placedActor->motionFlags = 0;
        }
    }
    sySoundPlay(SE_DOOR_OPEN);
    doorIndex = state->stepCue - METATILE_BEHAVIOR_LUNPA_FIRST_HOME_DOOR;
    fdMapGridAnimateMetatiles(
        LunpaDoors[doorIndex].steps, LunpaDoors[doorIndex].x, LunpaDoors[doorIndex].y);
    fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
    fdActorGet(ACTOR_PARTY_LEADER)->motionFlags = 0;
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    if (doorIndex != DOOR_TEMPLE)
    {
        fdActorCenterAndWalk(ACTOR_PARTY_LEADER, 2, -8);
        fdScriptWait(10);
    }
    syImportScriptSetExitRequest(state->stepCue);
    fdScriptFinishScreenTransition();
    fdScriptWaitTransitionTicks();
    fdScriptEnd();
}

// Walking north into the cave mouth the hidden switch opened in Lunpa's west cliff (metatile (2,
// 19)) takes Isaac into Lunpa's hidden room. It works only while Reveal shows the mouth; otherwise
// nothing happens there.
static void scLunpaEnterHiddenPassage(void)
{
    if (gmFlagGet(FLAG_LUNPA_HIDDEN_PASSAGE_OPEN) != 0)
    {
        if (gmFlagGet(FLAG_LUNPA_SECRETS_HIDDEN) == 0)
        {
            fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.2);
            fdActorGet(ACTOR_PARTY_LEADER)->motionFlags = 0;
            fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
            fdActorCenterAndWalk(ACTOR_PARTY_LEADER, 2, -8);
            fdScriptWait(13);
            syImportScriptSetExitRequest(LUNPA_TO_STONE_ROOMS);
        }
    }
}

// Walking up to the golden door in the cliff at the top of the Lunpa Fortress Gates (metatiles (9,
// 5) and (10, 5)), it opens and Isaac goes into Lunpa Fortress.
static void scLunpaEnterFortress(void)
{
    gmFlagSet(FLAG_LUNPA_FORTRESS_GATES_ENTERING_FORTRESS);
    fdScriptStart();
    fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.2);
    fdActorGet(ACTOR_PARTY_LEADER)->motionFlags = 0;
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    fdActorWalkBy(ACTOR_PARTY_LEADER, 0, -8);
    sySoundPlay(SE_DOOR_OPEN);
    fdMapGridCopyMetatiles(53, 4, 2, 2, 41, 4);
    fdScriptWait(10);
    fdMapGridCopyMetatiles(53, 6, 2, 2, 41, 4);
    fdScriptWait(10);
    syImportScriptSetExitRequest(LUNPA_FORTRESS_GATES_TO_LUNPA_FORTRESS_GROUND_FLOOR);
    fdScriptFinishScreenTransition();
    fdScriptWaitTransitionTicks();
    fdScriptEnd();
}

static void UNUSED scLunpaTryJumpForward(void)
{
    fdTryJumpForward();
}

// Unused: plays the treasure jingle while Reveal is shining. Nothing calls it.
UNUSED static void scLunpaPlayRevealSound(void)
{
    if (gmFlagGet(FLAG_LUNPA_SECRETS_HIDDEN) == 0)
    {
        sySoundPlay(MUS_TREASURE_FOUND);
    }
}

// Searching the sparkle Reveal shows on Lunpa's west cliff, from metatile (3,
// 18) below it, makes the rock crumble away into a dark cave mouth, the passage
// to Lunpa's hidden room.
static void scLunpaPressHiddenSwitch(void)
{
    if (gmFlagGet(FLAG_LUNPA_SECRETS_HIDDEN) != 0)
    {
        return;
    }
    if (fdScriptGetFlag(FLAG_LUNPA_HIDDEN_PASSAGE_OPEN) != 0)
    {
        return;
    }
    fdActorSetPos(ACTOR_SWITCH_SPARKLE, 0, 0);
    sySoundPlay(SE_HIDDEN_PASSAGE_OPEN);
    sySynch(1);
    scLunpaDrawHiddenPassagePicture(32, 45);
    sySynch(10);
    scLunpaDrawHiddenPassagePicture(41, 45);
    sySynch(10);
    scLunpaDrawHiddenPassagePicture(50, 45);
    sySynch(10);
    scLunpaDrawHiddenPassagePicture(32, 49);
    sySynch(10);
    scLunpaDrawHiddenPassagePicture(41, 49);
    sySynch(10);
    gmFlagSet(FLAG_LUNPA_HIDDEN_PASSAGE_OPEN);
}

// Copies the metatile at (srcColumn, srcRow) over the three metatiles (7,
// 11) to (9, 11) at the Lunpa Fortress Gates, just below the west half of the
// upper palisade.
static inline void scLunpaCopyGatewayCollision(Sint32 srcColumn, Sint32 srcRow)
{
    fdMapGridCopyMetatileAttributes(srcColumn, srcRow, 1, 1, 7, 11);
    fdMapGridCopyMetatileAttributes(srcColumn, srcRow, 1, 1, 8, 11);
    fdMapGridCopyMetatileAttributes(srcColumn, srcRow, 1, 1, 9, 11);
}

// Walking uncloaked into the yard between the palisades at the Lunpa Fortress Gates (metatiles (7,
// 11) to (14, 14)), the two guards cry out, step up into the upper palisade's gap and warn the
// party not to come any closer to the fortress. The three metatiles west of them are rewritten as
// Cloak's cast does.
static void scLunpaGuardsBlockGate(void)
{
    if (MainRoutineStateWork.cloaked == 0)
    {
        fdScriptStart();
        fdActorShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 2);
        fdActorShowEmote(ACTOR_RIGHT_GUARD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 15);
        fdScriptWait(30);
        fdActorWalkTo(ACTOR_LEFT_GUARD, 152, 168);
        fdActorWalkTo(ACTOR_RIGHT_GUARD, 168, 168);
        fdActorWaitForMoveById(ACTOR_LEFT_GUARD);
        fdActorWaitForMoveById(ACTOR_RIGHT_GUARD);
        fdActorStop(ACTOR_LEFT_GUARD);
        fdActorSetAnimation(ACTOR_LEFT_GUARD, ANIM_STILL);
        fdActorFaceDirection(ACTOR_LEFT_GUARD, FACING_SOUTH_SOUTHEAST, 0);
        fdActorStop(ACTOR_RIGHT_GUARD);
        fdActorSetAnimation(ACTOR_RIGHT_GUARD, ANIM_STILL);
        fdActorFaceDirection(ACTOR_RIGHT_GUARD, FACING_SOUTH_SOUTHWEST, 0);
        fdScriptSetMessageId(MsgLunpaGuardsWarnPartyAway);
        // "Don't come any closer to the fortress!"
        fdScriptShowMessage(ACTOR_LEFT_GUARD, 0);
        gmFlagSet(FLAG_LUNPA_FORTRESS_GATES_GUARDS_AT_DOOR);
        scLunpaCopyGatewayCollision(6, 11);
        fdScriptEnd();
    }
}

static void UNUSED scLunpaNoOperation(void)
{
}

// Casting Cloak at the Lunpa Fortress Gates rewrites the three metatiles (7,
// 11) to (9, 11) from the one at (6, 11); scLunpaCloakFades puts them back.
static void scLunpaCastCloak(void)
{
    scLunpaCopyGatewayCollision(6, 11);
    gmFlagSet(FLAG_LUNPA_FORTRESS_GATES_CLOAK_CAST);
}

// When the guards at the Lunpa Fortress Gates spot the party, they turn and shout who are you,
// wonder how anyone got past them, and march Isaac back out through the gap in the lower palisade.
static void scLunpaGuardsCatchParty(void)
{
    ACTOR *leader;
    Sint32 warningMsgBase;

    if (fdScriptGetFlag(FLAG_LUNPA_FORTRESS_GATES_GUARDS_CAUGHT_PARTY) != 0)
    {
        return;
    }
    gmFlagSet(FLAG_LUNPA_FORTRESS_GATES_GUARDS_CAUGHT_PARTY);
    fdScriptStart();
    leader = fdActorGet(ACTOR_PARTY_LEADER);
    fdActorFaceActor(ACTOR_LEFT_GUARD, ACTOR_PARTY_LEADER, 0);
    fdActorFaceActor(ACTOR_RIGHT_GUARD, ACTOR_PARTY_LEADER, 0);
    fdActorStartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
    fdActorStartRepeatedMotion(ACTOR_RIGHT_GUARD, 1);
    fdScriptWait(20);
    fdActorShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    // Warn the party before escorting everyone down the two sections of the gate.
    warningMsgBase = MsgLunpaGuardsCatchParty;
    fdScriptSetMessageId(warningMsgBase + CATCH_LEFT_GUARD_CHALLENGES);
    fdScriptShowMessage(ACTOR_LEFT_GUARD, 0);
    fdActorSetSpeed(ACTOR_PARTY_LEADER, 2);
    fdActorSetSpeed(ACTOR_LEFT_GUARD, 2);
    fdActorSetSpeed(ACTOR_RIGHT_GUARD, 2);
    fdActorSetAnimation(ACTOR_RIGHT_GUARD, ANIM_SHAKE_HEAD);
    fdScriptWait(35);
    fdScriptSetMessageId(warningMsgBase + CATCH_RIGHT_GUARD_WONDERS);
    fdScriptShowMessage(ACTOR_RIGHT_GUARD, 0);
    fdActorShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
    fdScriptSetMessageId(warningMsgBase + CATCH_LEFT_GUARD_REFUSES_ENTRY);
    fdScriptShowMessage(ACTOR_LEFT_GUARD, 0);
    fdActorSetAnimation(ACTOR_RIGHT_GUARD, ANIM_NOD);
    fdScriptWait(25);
    fdScriptSetMessageId(warningMsgBase + CATCH_RIGHT_GUARD_SENDS_PARTY_OFF);
    fdScriptShowMessage(ACTOR_RIGHT_GUARD, 0);
    fdActorWalkTo(ACTOR_LEFT_GUARD, leader->x.part[0].integer - 1, leader->z.part[0].integer);
    fdActorWaitForMoveById(ACTOR_LEFT_GUARD);
    fdActorWalkTo(ACTOR_PARTY_LEADER, 160, 216);
    fdActorWalkTo(ACTOR_LEFT_GUARD, 152, 200);
    fdActorWalkTo(ACTOR_RIGHT_GUARD, 168, 200);
    fdActorWaitForMoveById(ACTOR_LEFT_GUARD);
    fdActorWaitForMoveById(ACTOR_RIGHT_GUARD);
    fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
    fdActorFaceActor(ACTOR_LEFT_GUARD, ACTOR_PARTY_LEADER, 0);
    fdActorFaceActor(ACTOR_RIGHT_GUARD, ACTOR_PARTY_LEADER, 0);
    fdScriptWait(12);
    fdActorWalkTo(ACTOR_PARTY_LEADER, 160, 272);
    fdActorWalkTo(ACTOR_LEFT_GUARD, 152, 256);
    fdActorWalkTo(ACTOR_RIGHT_GUARD, 168, 256);
    fdActorWaitForMoveById(ACTOR_LEFT_GUARD);
    fdActorWaitForMoveById(ACTOR_RIGHT_GUARD);
    fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
    fdScriptEnd();
    sySetTickEvent(scLunpaWatchForUncloakedParty, EVENT_PASS_TICK);
}

// Puts back the three metatiles (7, 11) to (9, 11) that Cloak and the guards at the door rewrite,
// from the metatile at (7, 12).
static void scLunpaReopenGateway(void)
{
    scLunpaCopyGatewayCollision(7, 12);
}

// As Cloak fades at the Lunpa Fortress Gates, a party still in the yard between the palisades is
// caught by the guards.
static void scLunpaCloakFades(void)
{
#if CLOAK_FADE_RAISES_SPOTTED_TRIGGER
    FIELD_STATE *state;
#endif
    ACTOR *leader;

#if CLOAK_FADE_RAISES_SPOTTED_TRIGGER
    state = FieldStateWork;
#endif
    gmFlagClear(FLAG_LUNPA_FORTRESS_GATES_CLOAK_CAST);
    gmFlagClear(FLAG_LUNPA_FORTRESS_GATES_GUARDS_AT_DOOR);
    leader = fdActorGet(ACTOR_PARTY_LEADER);
    if (leader->x.fixed > toFIXED(104) && leader->x.fixed < toFIXED(240)
        && leader->z.fixed > toFIXED(160) && leader->z.fixed < toFIXED(248))
    {
        syCloseEvent(scLunpaWatchForUncloakedParty);
#if CLOAK_FADE_RAISES_SPOTTED_TRIGGER
        state->raisedCue = RAISED_PARTY_SPOTTED;
#else
        scLunpaGuardsCatchParty();
#endif
    }
    scLunpaReopenGateway();
    gmFlagClear(FLAG_LUNPA_FORTRESS_GATES_GUARDS_CAUGHT_PARTY);
}

// Every tick at the Lunpa Fortress Gates: an uncloaked Isaac in the gap of the upper palisade is
// spotted by the guards.
static void scLunpaWatchForUncloakedParty(void)
{
    FIELD_STATE *state;
    ACTOR       *leader;

    state  = FieldStateWork;
    leader = fdActorGet(ACTOR_PARTY_LEADER);
    if (MainRoutineStateWork.cloaked == 0
        && (leader->x.fixed >= toFIXED(144) && leader->x.fixed <= toFIXED(176))
        && leader->z.fixed >= toFIXED(168) && leader->z.fixed < toFIXED(176))
    {
        syCloseEvent(scLunpaWatchForUncloakedParty);
        state->raisedCue = RAISED_PARTY_SPOTTED;
    }
}

// Every 16 ticks while Isaac is dragged and thrown: a puff of dust at his feet.
static void scLunpaKickUpLeaderDust(void)
{
    ACTOR *leader;
    Sint32 dustAngle;
    Sint32 dustVelocity[3];

    leader = fdActorGet(ACTOR_PARTY_LEADER);
    if ((TickCount & 15) == 0)
    {
        dustAngle = (syRandomBelow(52)) * 64 + 230;
        SET_VECTOR(dustVelocity, syCos(dustAngle) / 4, 0, sySin(dustAngle) / 2);
        fdEffectSpawn(leader->x.fixed,
                      leader->y.fixed,
                      leader->z.fixed,
                      dustVelocity[0],
                      dustVelocity[1],
                      dustVelocity[2],
                      0,
                      NULL);
    }
}

// Caught inside Lunpa Fortress, Isaac is dragged out of the door by the two guards, struggling,
// down through both palisades, and thrown sprawling onto the slope below. And don't come back, says
// one, and they return to their posts.
static void scLunpaGuardsThrowIsaacOut(void)
{
    fdScriptStart();
    fdActorSetPos(ACTOR_PARTY_LEADER, 160, 128);
    fdActorSetPos(ACTOR_LEFT_GUARD, 152, 112);
    fdActorSetPos(ACTOR_RIGHT_GUARD, 168, 112);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
#if FIX_GATE_GUARDS_ACTOR_IDS
    fdActorFaceDirection(ACTOR_LEFT_GUARD, FACING_SOUTH_SOUTHEAST, 0);
    fdActorFaceDirection(ACTOR_RIGHT_GUARD, FACING_SOUTH_SOUTHWEST, 0);
#else
    // BUG: the town's guards, whose actor ids no actor at the gates holds, so the gates' guards
    // keep their headings.
    fdActorFaceDirection(ACTOR_WEST_GUARD, FACING_SOUTH_SOUTHEAST, 0);
    fdActorFaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH_SOUTHWEST, 0);
#endif
    fdCameraFollowActor(ACTOR_PARTY_LEADER, 0);
    fdScriptStartScreenTransition();
    fdScriptWait(30);
    fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.8);
    fdActorSetSpeed(ACTOR_LEFT_GUARD, 1.8);
    fdActorSetSpeed(ACTOR_RIGHT_GUARD, 1.8);
    fdActorWalkTo(ACTOR_LEFT_GUARD, 152, 288);
    fdActorWalkTo(ACTOR_RIGHT_GUARD, 168, 288);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
    fdActorSetDestination(ACTOR_PARTY_LEADER, 160, 296);
    sySetTickEvent(scLunpaKickUpLeaderDust, EVENT_PASS_TICK);
    fdScriptWait(1);
    sySoundPlay(SE_SCUFFLE);
    fdScriptWait(20);
    fdActorSetSpritePriority(ACTOR_LEFT_GUARD, 3);
    fdActorSetSpritePriority(ACTOR_RIGHT_GUARD, 3);
    sySoundPlay(SE_SCUFFLE);
    fdScriptWait(30);
    fdActorGet(ACTOR_LEFT_GUARD)->priorityFlags  |= ACTOR_PRIORITY_AUTOMATIC;
    fdActorGet(ACTOR_RIGHT_GUARD)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
    sySoundPlay(SE_SCUFFLE);
    fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
    fdActorSetAnimation(ACTOR_LEFT_GUARD, ANIM_STAND);
    fdActorSetAnimation(ACTOR_RIGHT_GUARD, ANIM_STAND);
    syCloseEvent(scLunpaKickUpLeaderDust);
    // Throw the leader down the steps, then wait for the first landing.
    fdActorGet(ACTOR_PARTY_LEADER)->motionFlags |= ACTOR_FALLS;
    fdActorGet(ACTOR_PARTY_LEADER)->velocityY    = toFIXED(6);
    fdActorGet(ACTOR_PARTY_LEADER)->velocityZ    = toFIXED(6);
    fdScriptWait(1);
    while (fdActorGet(ACTOR_PARTY_LEADER)->y.fixed != 0)
    {
        fdScriptWait(1);
    }
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SPRAWLED);
    sySoundPlay(SE_LANDING_THUD);
    fdActorSetAttachedEffect(ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    sySetTickEvent(scLunpaKickUpLeaderDust, EVENT_PASS_TICK);
    fdScriptWait(2);
    // A smaller second bounce ends with the guards' final warning.
    fdActorGet(ACTOR_PARTY_LEADER)->velocityY = toFIXED(3);
    fdScriptWait(1);
    while (fdActorGet(ACTOR_PARTY_LEADER)->y.fixed != 0)
    {
        fdScriptWait(1);
    }
    fdActorSetAttachedEffect(ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdScriptWait(10);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    syCloseEvent(scLunpaKickUpLeaderDust);
    fdScriptWait(50);
    fdScriptSetMessageId(MsgLunpaGuardForbidsReturn);
    // "And don't come back!"
    fdScriptShowMessage(ACTOR_LEFT_GUARD, 0);
    fdActorGet(ACTOR_LEFT_GUARD)->priorityFlags  |= ACTOR_PRIORITY_AUTOMATIC;
    fdActorGet(ACTOR_RIGHT_GUARD)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
    fdActorSetSpeed(ACTOR_LEFT_GUARD, 1);
    fdActorSetSpeed(ACTOR_RIGHT_GUARD, 1);
    fdActorWalkTo(ACTOR_LEFT_GUARD, 144, 200);
    fdActorWalkTo(ACTOR_RIGHT_GUARD, 176, 200);
    fdActorWaitForMoveById(ACTOR_LEFT_GUARD);
    fdActorWaitForMoveById(ACTOR_RIGHT_GUARD);
    fdActorSetAnimation(ACTOR_LEFT_GUARD, ANIM_STAND);
    fdActorSetAnimation(ACTOR_RIGHT_GUARD, ANIM_STAND);
    fdScriptWait(30);
    fdActorFaceDirection(ACTOR_LEFT_GUARD, FACING_SOUTH_SOUTHEAST, 0);
    fdActorFaceDirection(ACTOR_RIGHT_GUARD, FACING_SOUTH_SOUTHWEST, 0);
    fdScriptEnd();
}

// After the rescue, Isaac comes out of the fortress door, casts Cloak in the upper palisade's gap
// and slips round behind the guards, west along the palisade and down the west side of the slope.
static void scLunpaLeaderSneaksOut(void)
{
    fdScriptStart();
    fdScriptStartScreenTransition();
    fdActorWalkTo(ACTOR_PARTY_LEADER, 152, 168);
    fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
    fdScriptWait(20);
    // Cast Cloak before following the route around the guards.
    fdPsynergyLoadResources(ABILITY_CLOAK, 1);
    fdPsynergySetupActorPair(ACTOR_PARTY_LEADER, 0);
    fdPsynergyStartCast();
    fdPsynergyRunHandler(1);
    fdPsynergyStopCasterUpdate();
    fdActorWalkTo(ACTOR_PARTY_LEADER, 144, 184);
    fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
    fdActorWalkTo(ACTOR_PARTY_LEADER, 88, 184);
    fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
    fdActorWalkTo(ACTOR_PARTY_LEADER, 88, 200);
    fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
    fdActorWalkTo(ACTOR_PARTY_LEADER, 72, 200);
    fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
    fdActorWalkTo(ACTOR_PARTY_LEADER, 72, 288);
    fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
    fdActorWalkTo(ACTOR_PARTY_LEADER, 88, 288);
    fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
    fdScriptEnd();
}

// Sets up Lunpa (secrets hidden, the Nut in its tree) or the Lunpa Fortress Gates (the throw-out or
// the sneak-out, Garet's watch, the guards' watch and the fortress started over).
Sint32 scInit(void)
{
    ACTOR *puddle;

    if (MainRoutineStateWork.next == SCENE_LUNPA)
    {
        FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
        scLunpaHideSecrets();
        if (gmFlagGet(FLAG_LUNPA_NUT_CAUGHT) == 0)
        {
            scFloatNut(ACTOR_FLOATING_NUT);
        }
        puddle = fdActorGet(ACTOR_HIDDEN_PUDDLE);
        if (puddle != NULL)
        {
            fdActorSetSpriteFlags(puddle, 0);
        }
        gmFlagSet(FLAG_LUNPA_SECRETS_HIDDEN);
    }
    if (MainRoutineStateWork.next == SCENE_LUNPA_FORTRESS_GATES)
    {
        FieldStateWork->transition           = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
        MainRoutineStateWork.retreatEntrance = GATE_RETREAT_ENTRANCE;
        if (MainRoutineStateWork.entrance == LUNPA_FORTRESS_GATES_AFTER_CAUGHT
            && gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0)
        {
            scLunpaGuardsThrowIsaacOut();
        }
        if (MainRoutineStateWork.entrance == LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_TOADONPA
            && gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0)
        {
            scLunpaLeaderSneaksOut();
        }
        if (gmFlagGet(FLAG_DODONPA_LOCKED_UP) != 0 && gmFlagGet(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0)
        {
            sySetTickEvent(scLunpaWatchForFortressApproach, EVENT_PASS_TICK);
        }
#if LUNPA_GATES_RELOAD_REAPPLIES_CLOAK
        if (MainRoutineStateWork.cloaked != 0)
            scLunpaCastCloak();
#endif
        sySetTickEvent(scLunpaWatchForUncloakedParty, EVENT_PASS_TICK);
        // The fortress's puzzles and beaten guards are reset.
        gmFlagClear(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_IVY_BLOWN_AWAY);
        gmFlagClear(FLAG_LUNPA_FORTRESS_PASSAGE_IVY_BLOWN_AWAY);
        gmFlagClear(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_IVY_BLOWN_AWAY);
        gmFlagClear(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_FLIPPED);
        gmFlagClear(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_FLIPPED);
        gmFlagClear(FLAG_LUNPA_FORTRESS_CRATES_MOVED);
        gmFlagClear(FLAG_LUNPA_FORTRESS_MIDDLE_HALL_GUARD_BEATEN);
        gmFlagClear(FLAG_LUNPA_FORTRESS_THREE_GUARDS_BEATEN);
        gmFlagClear(FLAG_LUNPA_FORTRESS_DONPAS_DOOR_GUARD_BEATEN);
        gmFlagClear(FLAG_LUNPA_FORTRESS_METATILE_KEY_ROOM_GUARD_BEATEN);
    }
    return 0;
}

#include "shared/floating_nut.c"

// An entrance of the town, with its camera free, and of the Fortress gates, its camera kept in
// their bounds.
#define LUNPA_ENTRANCE(entrance, x, z, facing)                                                     \
    {(entrance), CONDITION_ALWAYS, (x), 0, (z), (facing), 0, -1, -1, -1, -1, 0}
#define GATE_ENTRANCE(entrance, x, z, facing)                                                      \
    {(entrance), CONDITION_ALWAYS, (x), 0, (z), (facing), 0, 32, 48, 272, 344, 0}

SCENE_ENTRANCE LunpaEntrances[] = {
    LUNPA_ENTRANCE(ENTRANCE_DEFAULT, 172, 149, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_WORLD_MAP, 224, 472, FACING_NORTH),
    LUNPA_ENTRANCE(LUNPA_FROM_LUNPA_FORTRESS_GATES, 232, 32, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_WEST_LUNPA_CAVE, 120, 104, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_FIRST_HOME, 248, 184, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_SECOND_HOME, 344, 232, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_THIRD_HOME, 344, 328, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_WEAPON_AND_ARMOR_SHOP, 232, 360, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_ITEM_SHOP, 328, 120, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_INN, 104, 184, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_TEMPLE, 48, 168, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_CRATE_ROOM, 216, 344, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_STONE_ROOMS, 40, 328, FACING_SOUTH),
    {SCENE_TABLE_END},
};

SCENE_ENTRANCE GateEntrances[] = {
    GATE_ENTRANCE(ENTRANCE_DEFAULT, 152, 328, FACING_NORTH),
    GATE_ENTRANCE(LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_GROUND_FLOOR, 160, 104, FACING_SOUTH),
    GATE_ENTRANCE(LUNPA_FORTRESS_GATES_FROM_LUNPA, 152, 328, FACING_NORTH),
    GATE_ENTRANCE(LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_TOADONPA, 144, 104, FACING_SOUTH),
    GATE_ENTRANCE(LUNPA_FORTRESS_GATES_AFTER_CAUGHT, 160, 224, FACING_EAST),
    {SCENE_TABLE_END},
};

// A door's exit into its room of RockRooms.
#define ROOM_EXIT(exit) SCENE_EXIT((exit), RockRooms, ROOM_ENTRANCE(exit))

Uint32 LunpaExits[] = {
    SCENE_EXITS(Lunpa),
    SCENE_EXIT(LUNPA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_LUNPA),
    SCENE_EXIT(LUNPA_TO_LUNPA_FORTRESS_GATES, LunpaFortressGates, LUNPA_FORTRESS_GATES_FROM_LUNPA),
    SCENE_EXIT(LUNPA_TO_WEST_LUNPA_CAVE, WestLunpaCave, WEST_LUNPA_CAVE_FROM_LUNPA),
    ROOM_EXIT(LUNPA_TO_FIRST_HOME),
    ROOM_EXIT(LUNPA_TO_SECOND_HOME),
    ROOM_EXIT(LUNPA_TO_THIRD_HOME),
    ROOM_EXIT(LUNPA_TO_WEAPON_AND_ARMOR_SHOP),
    ROOM_EXIT(LUNPA_TO_ITEM_SHOP),
    ROOM_EXIT(LUNPA_TO_INN),
    ROOM_EXIT(LUNPA_TO_TEMPLE),
    ROOM_EXIT(LUNPA_TO_CRATE_ROOM),
    SCENE_EXIT(LUNPA_TO_STONE_ROOMS, StoneRooms, STONE_ROOMS_FROM_LUNPA),
    SCENE_EXITS_END,
};

Uint32 GateExits[] = {
    SCENE_EXITS(LunpaFortressGates),
    SCENE_EXIT(LUNPA_FORTRESS_GATES_TO_LUNPA_FORTRESS_GROUND_FLOOR,
               LunpaFortressGroundFloor,
               LUNPA_FORTRESS_GROUND_FLOOR_FROM_LUNPA_FORTRESS_GATES),
    SCENE_EXIT(LUNPA_FORTRESS_GATES_TO_LUNPA, Lunpa, LUNPA_FROM_LUNPA_FORTRESS_GATES),
    SCENE_EXITS_END,
};

// clang-format off
ACTOR_PLACE LunpaActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      520, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      112, 0, 168, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      280, 0, 352, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      120, 0, 312, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      136, 0, 328, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
      368, 0, 256, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      280, 0, 200, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      216, 0, 40, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
      152, 0, 184, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      192, 0, 448, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      256, 0, 448, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      520, 0, 520, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_LUNPA_NUT_CAUGHT, MOVEMENT_TYPE_RESET_ACTION,
      120, 0, 472, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END, CONDITION_ALWAYS },
};

ACTOR_PLACE GateActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      144, 0, 200, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      176, 0, 200, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};
// clang-format on

// A cue for talking to an actor, for Mind Read on it, and for walking north into a door.
#define TALK(actorId, value) {CUE_TALK, (actorId), CONDITION_ALWAYS, (value)}
#define MIND_READ(actorId, value)                                                                  \
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), (actorId), CONDITION_ALWAYS, (value)}
#define DOOR(cue)                                                                                  \
    {STEP_CUE_WALKING_INTO(FACING_NORTH), (cue), CONDITION_ALWAYS, SCRIPT_REF(scLunpaEnterDoor)}

// The roads out of town and the doors, which every one of the town's cue tables holds.
#define LUNPA_ROADS_AND_DOORS                                                                      \
    {CUE_EXIT, METATILE_BEHAVIOR_LUNPA_WORLD_MAP_ROAD, CONDITION_ALWAYS, LUNPA_TO_WORLD_MAP},      \
        {CUE_EXIT,                                                                                 \
         METATILE_BEHAVIOR_LUNPA_GATE_ROAD,                                                        \
         CONDITION_ALWAYS,                                                                         \
         LUNPA_TO_LUNPA_FORTRESS_GATES},                                                           \
        {CUE_EXIT, METATILE_BEHAVIOR_LUNPA_CAVE_ROAD, CONDITION_ALWAYS, LUNPA_TO_WEST_LUNPA_CAVE}, \
        DOOR(METATILE_BEHAVIOR_LUNPA_FIRST_HOME_DOOR),                                             \
        DOOR(METATILE_BEHAVIOR_LUNPA_SECOND_HOME_DOOR),                                            \
        DOOR(METATILE_BEHAVIOR_LUNPA_THIRD_HOME_DOOR),                                             \
        DOOR(METATILE_BEHAVIOR_LUNPA_ARMS_SHOP_DOOR),                                              \
        DOOR(METATILE_BEHAVIOR_LUNPA_ITEM_SHOP_DOOR), DOOR(METATILE_BEHAVIOR_LUNPA_INN_DOOR),      \
        DOOR(METATILE_BEHAVIOR_LUNPA_TEMPLE_DOOR), DOOR(METATILE_BEHAVIOR_LUNPA_CRATE_ROOM_DOOR),  \
    {                                                                                              \
        STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_LUNPA_HIDDEN_PASSAGE,               \
            CONDITION_ALWAYS, SCRIPT_REF(scLunpaEnterHiddenPassage)                                \
    }

// The hidden puddle and what Reveal shows, which the town's cue tables share.
#define LUNPA_SECRETS                                                                              \
    {PSYNERGY_ON_ACTOR(ABILITY_FROST),                                                             \
     ACTOR_HIDDEN_PUDDLE,                                                                          \
     FLAG_LUNPA_PUDDLE_FROZEN,                                                                     \
     SCRIPT_REF(scLunpaFreezeHiddenPuddle)},                                                       \
        {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS),                                   \
         0,                                                                                        \
         CONDITION_ALWAYS,                                                                         \
         SCRIPT_REF(scLunpaRevealSecrets)},                                                        \
        {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES),                                  \
         0,                                                                                        \
         CONDITION_ALWAYS,                                                                         \
         SCRIPT_REF(scLunpaHideSecrets)},                                                          \
        {EXAMINE_CUE(EXAMINE_SILENT),                                                              \
         METATILE_BEHAVIOR_LUNPA_HIDDEN_SWITCH,                                                    \
         FLAG_LUNPA_SECRETS_HIDDEN,                                                                \
         SCRIPT_REF(scLunpaPressHiddenSwitch)},                                                    \
        {EXAMINE_CUE(EXAMINE_SILENT),                                                              \
         METATILE_BEHAVIOR_LUNPA_PSYNERGY_STONE,                                                   \
         FLAG_LUNPA_PSYNERGY_STONE,                                                                \
         FIND_PSYNERGY_STONE},                                                                     \
        {EXAMINE_CUE(EXAMINE_GRAVE),                                                               \
         METATILE_BEHAVIOR_LUNPA_GRAVE_SLEEP_BOMB,                                                 \
         FLAG_LUNPA_GRAVE_SLEEP_BOMB,                                                              \
         FIND_ITEM(ITEM_SLEEP_BOMB)},                                                              \
        {EXAMINE_CUE(EXAMINE_BARREL),                                                              \
         METATILE_BEHAVIOR_LUNPA_BARREL_NUT,                                                       \
         FLAG_LUNPA_BARREL_NUT,                                                                    \
         FIND_ITEM(ITEM_NUT)},                                                                     \
        {PSYNERGY_ON_ACTOR(ABILITY_CATCH),                                                         \
         ACTOR_FLOATING_NUT,                                                                       \
         FLAG_LUNPA_NUT_CAUGHT,                                                                    \
         SCRIPT_REF(scLunpaCatchFloatingNut)},                                                     \
        {EXAMINE_CUE(EXAMINE_SILENT),                                                              \
         METATILE_BEHAVIOR_LUNPA_WELL,                                                             \
         CONDITION_ALWAYS,                                                                         \
         SCRIPT_REF(scLunpaSearchWell)},                                                           \
    {                                                                                              \
        CUE_TABLE_END                                                                              \
    }

CUE LunpaSealedCues[] = {
    LUNPA_ROADS_AND_DOORS,
    TALK(ACTOR_MAN_ASKING_HOW_LONG_IN_LUNPA, SCRIPT_REF(scLunpaAskAboutLengthOfStay)),
    TALK(ACTOR_WOMAN_WITH_NO_FREEDOM, MSGNO(MsgLunpaVillageVillagerBSealed)),
    TALK(ACTOR_MAN_ON_HAMMETS_KIDNAPPING, SCRIPT_REF(scLunpaAskAboutHammetsKidnapping)),
    TALK(ACTOR_WOMAN_SAYING_DONPA_IS_TOO_OLD, MSGNO(MsgLunpaVillageVillagerDSealed)),
    TALK(ACTOR_OLD_MAN_ON_LUNPA_AND_DONPA, MSGNO(MsgLunpaVillageVillagerESealed)),
    TALK(ACTOR_OLD_WOMAN_ON_DONPA, MSGNO(MsgLunpaVillageVillagerFSealed)),
    TALK(ACTOR_BOY_ASKING_TO_SEE_DONPA, SCRIPT_REF(scLunpaAskAboutSeeingDonpa)),
    TALK(ACTOR_GIRL_ON_THE_CHEFS_MEALS, MSGNO(MsgLunpaVillageVillagerHSealed)),
    TALK(ACTOR_WEST_GUARD, SCRIPT_REF(scLunpaTalkToWestGuard)),
    TALK(ACTOR_EAST_GUARD, SCRIPT_REF(scLunpaTalkToEastGuard)),
    MIND_READ(ACTOR_MAN_ASKING_HOW_LONG_IN_LUNPA, MSGNO(MsgLunpaVillageVillagerASealedThoughts)),
    MIND_READ(ACTOR_WOMAN_WITH_NO_FREEDOM, MSGNO(MsgLunpaVillageVillagerBSealedThoughts)),
    MIND_READ(ACTOR_MAN_ON_HAMMETS_KIDNAPPING, MSGNO(MsgLunpaVillageVillagerCSealedThoughts)),
    MIND_READ(ACTOR_WOMAN_SAYING_DONPA_IS_TOO_OLD, MSGNO(MsgLunpaVillageVillagerDSealedThoughts)),
    MIND_READ(ACTOR_OLD_MAN_ON_LUNPA_AND_DONPA, MSGNO(MsgLunpaVillageVillagerESealedThoughts)),
    MIND_READ(ACTOR_OLD_WOMAN_ON_DONPA, MSGNO(MsgLunpaVillageVillagerFSealedThoughts)),
    MIND_READ(ACTOR_BOY_ASKING_TO_SEE_DONPA, MSGNO(MsgLunpaVillageVillagerGSealedThoughts)),
    MIND_READ(ACTOR_GIRL_ON_THE_CHEFS_MEALS, MSGNO(MsgLunpaVillageVillagerHSealedThoughts)),
    MIND_READ(ACTOR_WEST_GUARD, MSGNO(MsgLunpaVillageWestGuardSealedThoughts)),
    MIND_READ(ACTOR_EAST_GUARD, SCRIPT_REF(scLunpaReadEastGuardsThoughts)),
    LUNPA_SECRETS,
};

CUE LunpaReopenedCues[] = {
    LUNPA_ROADS_AND_DOORS,
    TALK(ACTOR_MAN_ASKING_HOW_LONG_IN_LUNPA, MSGNO(MsgLunpaVillageVillagerAReopened)),
    TALK(ACTOR_WOMAN_WITH_NO_FREEDOM, SCRIPT_REF(scLunpaComplainAboutCold)),
    TALK(ACTOR_MAN_ON_HAMMETS_KIDNAPPING, MSGNO(MsgLunpaVillageVillagerCReopened)),
    TALK(ACTOR_WOMAN_SAYING_DONPA_IS_TOO_OLD, SCRIPT_REF(scLunpaAskAboutFortressCommotion)),
    TALK(ACTOR_OLD_MAN_ON_LUNPA_AND_DONPA, MSGNO(MsgLunpaVillageVillagerEReopened)),
    TALK(ACTOR_OLD_WOMAN_ON_DONPA, MSGNO(MsgLunpaVillageVillagerFReopened)),
    TALK(ACTOR_BOY_ASKING_TO_SEE_DONPA, MSGNO(MsgLunpaVillageVillagerGReopened)),
    TALK(ACTOR_GIRL_ON_THE_CHEFS_MEALS, MSGNO(MsgLunpaVillageVillagerHReopened)),
    TALK(ACTOR_WEST_GUARD, MSGNO(MsgLunpaVillageWestGuardReopened)),
    TALK(ACTOR_EAST_GUARD, MSGNO(MsgLunpaVillageEastGuardReopened)),
    MIND_READ(ACTOR_MAN_ASKING_HOW_LONG_IN_LUNPA, MSGNO(MsgLunpaVillageVillagerAReopenedThoughts)),
    MIND_READ(ACTOR_WOMAN_WITH_NO_FREEDOM, MSGNO(MsgLunpaVillageVillagerBReopenedThoughts)),
    MIND_READ(ACTOR_MAN_ON_HAMMETS_KIDNAPPING, MSGNO(MsgLunpaVillageVillagerCReopenedThoughts)),
    MIND_READ(ACTOR_WOMAN_SAYING_DONPA_IS_TOO_OLD, MSGNO(MsgLunpaVillageVillagerDReopenedThoughts)),
    MIND_READ(ACTOR_OLD_MAN_ON_LUNPA_AND_DONPA, MSGNO(MsgLunpaVillageVillagerEReopenedThoughts)),
    MIND_READ(ACTOR_OLD_WOMAN_ON_DONPA, MSGNO(MsgLunpaVillageVillagerFReopenedThoughts)),
    MIND_READ(ACTOR_BOY_ASKING_TO_SEE_DONPA, MSGNO(MsgLunpaVillageVillagerGReopenedThoughts)),
    MIND_READ(ACTOR_GIRL_ON_THE_CHEFS_MEALS, MSGNO(MsgLunpaVillageVillagerHReopenedThoughts)),
    MIND_READ(ACTOR_WEST_GUARD, MSGNO(MsgLunpaVillageWestGuardReopenedThoughts)),
    MIND_READ(ACTOR_EAST_GUARD, SCRIPT_REF(scLunpaReadEastGuardsThoughts)),
    LUNPA_SECRETS,
};

// clang-format off
CUE GateCues[] = {
    { CUE_EXIT, METATILE_BEHAVIOR_LUNPA_FORTRESS_GATES_LUNPA_ROAD, CONDITION_ALWAYS, LUNPA_FORTRESS_GATES_TO_LUNPA },
    // Unused: no metatile of the gates' map has the third road's behavior.
    { CUE_EXIT, METATILE_BEHAVIOR_LUNPA_FORTRESS_GATES_THIRD_ROAD, CONDITION_ALWAYS, GATE_EXIT_THIRD },
    { PSYNERGY_ANYWHERE(ABILITY_CLOAK, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, SCRIPT_REF(scLunpaCastCloak) },
    { PSYNERGY_ANYWHERE(ABILITY_CLOAK, PSYNERGY_PHASE_AFTER_END), 0, CONDITION_ALWAYS, SCRIPT_REF(scLunpaCloakFades) },
    { CUE_STEP, METATILE_BEHAVIOR_LUNPA_FORTRESS_GATES_FORTRESS_DOOR, FLAG_LUNPA_FORTRESS_GATES_ENTERING_FORTRESS,
      SCRIPT_REF(scLunpaEnterFortress) },
    { CUE_STEP, METATILE_BEHAVIOR_LUNPA_FORTRESS_GATES_GUARDED_GROUND, FLAG_LUNPA_FORTRESS_GATES_GUARDS_AT_DOOR,
      SCRIPT_REF(scLunpaGuardsBlockGate) },
    { CUE_STEP, METATILE_BEHAVIOR_LUNPA_FORTRESS_GATES_FORTRESS_APPROACH, CONDITION_ALWAYS,
      SCRIPT_REF(scLunpaGaretTurnsIsaacBack) },
    { CUE_RAISED, RAISED_PARTY_SPOTTED, CONDITION_ALWAYS, SCRIPT_REF(scLunpaGuardsCatchParty) },
    TALK(ACTOR_LEFT_GUARD, SCRIPT_REF(scLunpaTalkToLeftGuard)),
    TALK(ACTOR_RIGHT_GUARD, SCRIPT_REF(scLunpaTalkToRightGuard)),
    MIND_READ(ACTOR_LEFT_GUARD, SCRIPT_REF(scLunpaReadLeftGuardsThoughts)),
    MIND_READ(ACTOR_RIGHT_GUARD, SCRIPT_REF(scLunpaReadRightGuardsThoughts)),
    { EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_LUNPA_FORTRESS_GATES_WELL, CONDITION_ALWAYS, SCRIPT_REF(scLunpaSearchWell) },
    { CUE_TABLE_END },
};
// clang-format on

// Each door's opening, as a map grid animation.
Uint16 LunpaDoorSteps[DOOR_COUNT][11] = {
    {MAPGRID_ANIMATION_STEP(38, 35, 2, 2, 5),
     MAPGRID_ANIMATION_STEP(40, 35, 2, 2, 5),
     MAPGRID_ANIMATION_STEPS_END},
    {MAPGRID_ANIMATION_STEP(42, 35, 2, 2, 5),
     MAPGRID_ANIMATION_STEP(44, 35, 2, 2, 5),
     MAPGRID_ANIMATION_STEPS_END},
    {MAPGRID_ANIMATION_STEP(46, 33, 2, 2, 5),
     MAPGRID_ANIMATION_STEP(48, 33, 2, 2, 5),
     MAPGRID_ANIMATION_STEPS_END},
    {MAPGRID_ANIMATION_STEP(46, 35, 2, 2, 5),
     MAPGRID_ANIMATION_STEP(48, 35, 2, 2, 5),
     MAPGRID_ANIMATION_STEPS_END},
    {MAPGRID_ANIMATION_STEP(50, 33, 2, 2, 5),
     MAPGRID_ANIMATION_STEP(52, 33, 2, 2, 5),
     MAPGRID_ANIMATION_STEPS_END},
    {MAPGRID_ANIMATION_STEP(50, 35, 2, 2, 5),
     MAPGRID_ANIMATION_STEP(52, 35, 2, 2, 5),
     MAPGRID_ANIMATION_STEPS_END},
    {MAPGRID_ANIMATION_STEP(42, 33, 4, 2, 5),
     MAPGRID_ANIMATION_STEP(38, 33, 4, 2, 5),
     MAPGRID_ANIMATION_STEPS_END},
    {MAPGRID_ANIMATION_STEP(54, 33, 2, 2, 5),
     MAPGRID_ANIMATION_STEP(56, 33, 2, 2, 5),
     MAPGRID_ANIMATION_STEPS_END},
};

// Each door's opening animation and the map grid metatile it plays at, by its step cue from
// METATILE_BEHAVIOR_LUNPA_FIRST_HOME_DOOR.
MAP_GRID_METATILE_ANIMATION LunpaDoors[] = {
    {LunpaDoorSteps[0], 46, 9 },
    {LunpaDoorSteps[1], 52, 12},
    {LunpaDoorSteps[2], 52, 18},
    {LunpaDoorSteps[3], 45, 20},
    {LunpaDoorSteps[4], 51, 5 },
    {LunpaDoorSteps[5], 37, 9 },
    {LunpaDoorSteps[6], 33, 6 },
    {LunpaDoorSteps[7], 44, 17},
};

// Unused: a zero word nothing reads.
Uint32 LunpaUnusedWord = 0;
