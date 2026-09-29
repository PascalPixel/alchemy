#ifndef CAVE_H
#define CAVE_H

#include "TYPES.H"
#include "ABILITY_IDS.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "FLAG_IDS.H"
#include "SOUND_IDS.H"

/*
 * West Lunpa Cave, the passage between Lunpa and the world map. Its gate
 * can only be worked from the Lunpa side: one switch lowers it, another
 * raises it, and freezing the puddle beneath it with Frost props it open
 * for good. After Hammet is freed, the party finds Bunza waiting here with
 * his wagon, and the player chooses whether to ride back to Kalay with them
 * or to stay behind.
 */

enum CaveEntrance {
    CAVE_ENTRANCE_FROM_WORLD_MAP = 1,
    CAVE_ENTRANCE_FROM_LUNPA = 2,
    CAVE_ENTRANCE_THIRD = 3
};

enum CaveExit {
    CAVE_EXIT_TO_WORLD_MAP = 1,
    CAVE_EXIT_TO_LUNPA = 2,
    CAVE_EXIT_BY_WAGON = 3
};

/* Where the cave's exits arrive in the scenes they lead to. */
enum {
    WORLD_MAP_ENTRANCE_FROM_CAVE = 49,
    LUNPA_ENTRANCE_FROM_CAVE = 3
};

enum CaveTrigger {
    TRIGGER_WORLD_MAP_PASSAGE = 1,
    TRIGGER_LUNPA_PASSAGE = 2,
    TRIGGER_LOWERING_SWITCH = 3,
    TRIGGER_RAISING_SWITCH = 4,
    TRIGGER_BUNZA_HIDING_PLACE = 10
};

/* The gate's position and the pillars Frost has raised. */
enum CaveFlag {
    FLAG_CAVE_GATE_PROPPED = 0x200,
    FLAG_CAVE_GATE_LOWERED = 0x201,
    FLAG_CAVE_GATE_RAISED = 0x202,
    FLAG_CAVE_NORTH_PILLAR = 0x203,
    FLAG_CAVE_SOUTH_PILLAR = 0x204
};

/* The scene hides an actor it no longer needs at the map's corner. */
enum CaveActor {
    ACTOR_HIDDEN_PUDDLE = ACTOR_FIRST_PLACED,
    ACTOR_SOUTH_PUDDLE,
    ACTOR_GATE_PUDDLE,
    ACTOR_NORTH_PUDDLE,
    ACTOR_BUNZA,
    ACTOR_HAMMET
};

enum CaveSprite {
    SPRITE_HAMMET = 0x31,
    SPRITE_BUNZA = 0x44,
    SPRITE_PUDDLE = 0xe3
};

/* A frozen puddle stands as a pillar of ice. */
enum {
    PUDDLE_ANIM_FROZEN = 5
};

/* The pillar drawn behind the lowered gate. */
enum {
    GATE_PILLAR_PRIORITY = 3
};



/* The party hears someone in the dark. */
enum SightingLine {
    SIGHTING_GERALD_SAW_SOMEONE,
    SIGHTING_MIA_SAW_SOMETHING,
    SIGHTING_IVAN_ASKS_IF_FOUND,
    SIGHTING_GERALD_WILL_FIGHT,
    SIGHTING_GERALD_ASKS_WHAT_ELSE
};

/* Hammet and Bunza know each other's voices. */
enum RecognitionLine {
    RECOGNITION_HAMMET_CALLS_OUT,
    RECOGNITION_HAMMET_NAMES_BUNZA,
    RECOGNITION_BUNZA_KNOWS_VOICE,
    RECOGNITION_BUNZA_NAMES_HAMMET
};

enum ReunionLine {
    REUNION_BUNZA_ASKS_ABOUT_RELEASE,
    REUNION_HAMMET_CREDITS_IVAN,
    REUNION_IVAN_CREDITS_LEADER,
    REUNION_GERALD_QUESTIONS_IVAN,
    REUNION_IVAN_CREDITS_EVERYONE,
    REUNION_HAMMET_THANKS_PARTY,
    REUNION_HAMMET_THANKS_IVAN,
    REUNION_MIA_PRAISES_BUNZA,
    REUNION_HAMMET_PRAISES_BUNZA,
    REUNION_BUNZA_CALLS_IT_CHANCE,
    REUNION_BUNZA_AVOIDED_LUNPA,
    REUNION_BUNZA_KNEW_OF_PRISON,
    REUNION_GERALD_ASKS_WHY_BUNZA_CAME,
    REUNION_BUNZA_RECALLS_ADVICE,
    REUNION_MIA_ASKS_ABOUT_TRADE,
    REUNION_BUNZA_MEANS_WISDOM,
    REUNION_HAMMET_ON_APPEARANCES,
    REUNION_BUNZA_ON_UNPLEASANT_PLACES,
    REUNION_HAMMET_ON_SELLING,
    REUNION_IVAN_ASKS_ABOUT_SERVING,
    REUNION_IVAN_ON_FATE,
    REUNION_GERALD_ASKS_ABOUT_ENTRY,
    REUNION_BUNZA_WAS_REFUSED,
    REUNION_MIA_ASKS_WHY_BUNZA_STILL_CAME,
    REUNION_BUNZA_MENTIONS_COMMOTION,
    REUNION_IVAN_EXPLAINS_COMMOTION,
    REUNION_BUNZA_LINKS_COMMOTION,
    REUNION_BUNZA_HAD_TO_KNOW,
    REUNION_GERALD_ASKS_ABOUT_CAVE,
    REUNION_MIA_ON_GATE,
    REUNION_BUNZA_HID,
    REUNION_HAMMET_STARTLED_BUNZA,
    REUNION_BUNZA_IS_GLAD,
    REUNION_IVAN_ASKS_WHY,
    REUNION_BUNZA_WARNS_OF_SEARCH,
    REUNION_BUNZA_FEARS_CAPTURE,
    REUNION_MIA_URGES_ESCAPE,
    REUNION_IVAN_WANTS_STEALTH,
    REUNION_GERALD_ASKS_TO_FIGHT,
    REUNION_BUNZA_DISCOURAGES_FIGHT,
    REUNION_BUNZA_AGREES_NOT_TO_FIGHT
};

enum PlanLine {
    PLAN_HAMMET_ASKS_PLAN,
    PLAN_BUNZA_HAS_WAGON,
    PLAN_MIA_ASKS_ABOUT_WAGON,
    PLAN_BUNZA_OFFERS_RIDE,
    PLAN_IVAN_DOUBTS_WAGON,
    PLAN_BUNZA_REASSURES,
    PLAN_GERALD_AGREES,
    PLAN_BUNZA_IS_UNSUSPECTED
};

enum WagonLine {
    WAGON_BUNZA_LEADS_THE_WAY,
    WAGON_GERALD_ASKS_TO_RIDE
};

enum ConfusionLine {
    CONFUSION_IVAN_IS_CONFUSED,
    CONFUSION_GERALD_ASKS_AGAIN
};

enum WarningLine {
    WARNING_IVAN_ASKS_IF_STAYING,
    WARNING_BUNZA_CANNOT_WAIT,
    WARNING_MIA_ASKS_ABOUT_BUSINESS
};

enum FarewellLine {
    FAREWELL_GERALD_STAYS,
    FAREWELL_MIA_STAYS,
    FAREWELL_IVAN_STAYS,
    FAREWELL_HAMMET_LETS_IVAN_GO,
    FAREWELL_BUNZA_SAYS_GOODBYE,
    FAREWELL_GERALD_SIGHS,
    FAREWELL_MIA_HOPES_FOR_SAFETY,
    FAREWELL_IVAN_REASSURES,
    FAREWELL_GERALD_MOVES_ON
};

enum DepartureLine {
    DEPARTURE_GERALD_HEADS_FOR_KALAY,
    DEPARTURE_IVAN_THINKS_OF_LAYANA,
    DEPARTURE_HAMMET_LONGS_FOR_LAYANA,
    DEPARTURE_BUNZA_SETS_OFF
};


extern const struct SceneEntrance gCaveEntrances[];
extern const u32 gCaveExits[];
extern const struct ScenePlacement gCaveNoPlacements[];
extern const struct ScenePlacement gCavePlacements[];
extern const struct SceneEvent gCaveEvents[];

void HiddenPuddle_Freeze(void);
void SouthPuddle_Freeze(void);
void GatePuddle_Freeze(void);
void NorthPuddle_Freeze(void);
void LoweringSwitch_Flip(void);
void RaisingSwitch_Flip(void);
void Reunion_Begin(void);
void Gate_DrawPropped(void);
void Gate_Lower(void);
void Gate_Raise(void);
void Reunion_Converse(void);
void WagonChoice_Run(void);
u8 Leader_AnswersYes(void);
u8 Gerald_AsksIfNotRiding(void);
u8 Gerald_AsksAboutUnfinishedBusiness(void);
u8 Party_ConfirmsStaying(void);
u8 Bunza_AsksAboutUnfinishedBusiness(void);
u8 Gerald_ChecksNothingLeftToDo(void);
u8 Mia_AsksIfRidingAfterAll(void);
u8 Bunza_CannotWait(void);
u8 Gerald_AsksAboutThingsToDo(void);
void Party_StaysBehind(void);
void Party_RidesWagon(void);

#endif
