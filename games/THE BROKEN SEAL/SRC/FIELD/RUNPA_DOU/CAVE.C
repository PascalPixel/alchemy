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

enum CaveMessage {
    MSG_FLIPPED_SWITCH = 0x1528,
    MSG_GERALD_ASKS_IF_NOT_RIDING = 0x2549,
    MSG_MIA_ASKS_IF_RIDING_AFTER_ALL = 0x254a,
    MSG_GERALD_ASKS_ABOUT_UNFINISHED_BUSINESS = 0x254d,
    MSG_BUNZA_ASKS_ABOUT_UNFINISHED_BUSINESS = 0x2557,
    MSG_GERALD_ASKS_ABOUT_THINGS_TO_DO = 0x255c,
    MSG_GERALD_CHECKS_NOTHING_LEFT = 0x255d
};

/*
 * The conversations are runs of consecutive messages. Each script finds its
 * run's first message and speaks the lines by their place in the run.
 */
extern u8 LinkedMessage_GeraldSawSomeone;
extern u8 LinkedMessage_HammetCallsOut;
extern u8 LinkedMessage_BunzaAsksAboutRelease;
extern u8 LinkedMessage_HammetAsksPlan;
extern u8 LinkedMessage_BunzaLeadsToWagon;
extern u8 LinkedMessage_IvanIsConfused;
extern u8 LinkedMessage_IvanAsksIfStaying;
extern u8 LinkedMessage_GeraldStays;
extern u8 LinkedMessage_GeraldHeadsForKalay;

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

/* This scene's own id, resolved when the overlay links. */
extern u8 LinkedScene_RunpaDou;

extern const struct SceneEntrance gCaveEntrances[];
extern const u32 gCaveExits[];
extern const struct ScenePlacement gCaveNoPlacements[];
extern const struct ScenePlacement gCavePlacements[];
extern const struct SceneEvent gCaveEvents[];

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

const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gCaveEntrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    return NULL;
}

const u32 *Scene_GetExits(void)
{
    return gCaveExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.scene == (s32)&LinkedScene_RunpaDou) {
        return gCavePlacements;
    }
    return gCaveNoPlacements;
}

const struct SceneEvent *Scene_GetEvents(void)
{
    return gCaveEvents;
}

void HiddenPuddle_Freeze(void)
{
    struct FieldActor *puddle;

    puddle = Actor_Get(ACTOR_HIDDEN_PUDDLE);
    if (puddle != NULL) {
        Actor_SetSpriteFlags(puddle, 0);
    }
}

/* The pillar raised from the south puddle changes the ground it stands on. */
void SouthPuddle_Freeze(void)
{
    struct FieldActor *puddle;

    puddle = Actor_Get(ACTOR_SOUTH_PUDDLE);
    if (puddle != NULL) {
        puddle->priority_flags = ACTOR_PRIORITY_AUTOMATIC;
        puddle->motion_flags = 0;
    }
    Map_CopyCellAttributes(7, 32, 1, 1, 8, 32);
    GameFlag_Set(FLAG_CAVE_SOUTH_PILLAR);
}

/* The gate rises onto the frozen pillar and stays there. */
void Gate_DrawPropped(void)
{
    struct FieldActor *pillar;

    pillar = Actor_Get(ACTOR_GATE_PUDDLE);
    Actor_SetAnimation(ACTOR_GATE_PUDDLE, PUDDLE_ANIM_FROZEN);
    if (pillar != NULL) {
        Actor_SetSpriteFlags(pillar, 0);
        pillar->priority_flags = ACTOR_PRIORITY_AUTOMATIC;
    }
    Map_CopyCells(41, 87, 2, 5, 21, 59);
    Task_Wait(4);
    Map_CopyCells(3, 93, 1, 1, 24, 62);
    Map_CopyCells(1, 94, 1, 1, 21, 55);
    Map_CopyCells(43, 87, 2, 5, 21, 58);
    Task_Wait(4);
    Map_CopyCells(41, 87, 2, 5, 21, 58);
    Task_Wait(4);
    Task_Wait(4);
    Map_CopyCellAttributes(21, 11, 2, 2, 21, 13);
    Map_CopyCellAttributes(21, 11, 1, 1, 22, 15);
    Map_CopyCellAttributes(19, 17, 1, 1, 21, 14);
}

void GatePuddle_Freeze(void)
{
    struct FieldActor *pillar;

    pillar = Actor_Get(ACTOR_GATE_PUDDLE);
    GameFlag_Set(FLAG_CAVE_GATE_PROPPED);
    if (pillar != NULL) {
        Actor_SetSpriteFlags(pillar, 0);
        pillar->priority_flags = ACTOR_PRIORITY_AUTOMATIC;
    }
    if (GameFlag_IsSet(FLAG_CAVE_GATE_RAISED) == 0) {
        Audio_PlayCue(SOUND_GATE_MOVE);
        Gate_DrawPropped();
        Audio_PlayCue(SOUND_PUZZLE_SOLVED);
        GameFlag_Set(FLAG_CAVE_GATE_RAISED);
    }
}

void NorthPuddle_Freeze(void)
{
    GameFlag_Set(FLAG_CAVE_NORTH_PILLAR);
}

void Gate_Lower(void)
{
    if (GameFlag_IsSet(FLAG_CAVE_GATE_RAISED) != 0) {
        Map_CopyCells(41, 86, 2, 6, 21, 57);
        Task_Wait(4);
        Map_CopyCells(43, 86, 2, 6, 21, 57);
        Task_Wait(4);
        Map_CopyCells(41, 86, 2, 6, 21, 58);
        Task_Wait(4);
        Map_CopyCells(43, 86, 2, 6, 21, 58);
        Task_Wait(4);
    }
    Map_CopyCells(2, 93, 1, 1, 24, 62);
    Map_CopyCells(2, 94, 1, 1, 21, 55);
    Map_CopyCells(41, 86, 2, 6, 21, 59);
    Task_Wait(4);
    Map_CopyCells(1, 93, 1, 1, 24, 62);
    Map_CopyCells(3, 94, 1, 1, 21, 55);
    Map_CopyCells(43, 86, 2, 6, 21, 59);
    Task_Wait(4);
    Actor_SetSpritePriority(ACTOR_GATE_PUDDLE, GATE_PILLAR_PRIORITY);
    Map_CopyCellAttributes(19, 17, 1, 1, 22, 15);
}

void LoweringSwitch_Flip(void)
{
    if (GameFlag_IsSet(FLAG_CAVE_GATE_LOWERED) == 0) {
        if (GameFlag_IsSet(FLAG_CAVE_GATE_PROPPED) == 0) {
            Message_ShowCentered(MSG_FLIPPED_SWITCH, 1);
            Audio_PlayCue(SOUND_GATE_MOVE);
            Gate_Lower();
            GameFlag_Set(FLAG_CAVE_GATE_LOWERED);
            GameFlag_Clear(FLAG_CAVE_GATE_RAISED);
        }
    }
}

void Gate_Raise(void)
{
    Map_CopyCells(41, 87, 2, 5, 21, 59);
    Task_Wait(4);
    Map_CopyCells(2, 93, 1, 1, 24, 62);
    Map_CopyCells(2, 94, 1, 1, 21, 55);
    Map_CopyCells(43, 87, 2, 5, 21, 58);
    Task_Wait(4);
    Map_CopyCells(3, 93, 1, 1, 24, 62);
    Map_CopyCells(1, 94, 1, 1, 21, 55);
    Map_CopyCells(41, 87, 2, 5, 21, 58);
    Map_CopyCellAttributes(21, 11, 2, 2, 21, 13);
    Map_CopyCellAttributes(19, 17, 1, 1, 21, 14);
}

void RaisingSwitch_Flip(void)
{
    if (GameFlag_IsSet(FLAG_CAVE_GATE_PROPPED) == 0) {
        if (GameFlag_IsSet(FLAG_CAVE_GATE_RAISED) == 0) {
            Message_ShowCentered(MSG_FLIPPED_SWITCH, 1);
            Audio_PlayCue(SOUND_GATE_MOVE);
            Gate_Raise();
            GameFlag_Set(FLAG_CAVE_GATE_RAISED);
            GameFlag_Clear(FLAG_CAVE_GATE_LOWERED);
        }
    }
}

/*
 * Once Hammet is free, Bunza steps out from where he hid, Hammet and the
 * party gather around him, and the two recognize each other.
 */
void Reunion_Begin(void)
{
    struct FieldActor *leader;
    s32 sighting;
    s32 recognition;

    if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        GameFlag_Set(FLAG_LUNPA_CAVE_REUNION_SEEN);
        Event_Begin();
        Actor_SetPosition(ACTOR_BUNZA, PIXELS(144), PIXELS(400));
        Actor_SetSpeed(ACTOR_BUNZA, 0x18000, 0xc000);
        Actor_WalkTo(ACTOR_BUNZA, 184, 400);
        Actor_WaitForMove(ACTOR_BUNZA);
        Actor_SetAnimation(ACTOR_BUNZA, ANIM_STAND);
        Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
        Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
        Event_Wait(30);
        Camera_SetSpeed(0x8000, 0x1000);
        Camera_MoveTo(PIXELS(192), -1, PIXELS(432), 1);
        leader = Actor_Get(ACTOR_PARTY_LEADER);
        if (leader != NULL) {
            Actor_SetPosition(ACTOR_HAMMET, leader->x.fixed, leader->z.fixed);
        }
        Actor_SetSpeed(ACTOR_HAMMET, 0x14ccc, 0xa666);
        Actor_WalkTo(ACTOR_HAMMET, 168, 464);
        Actor_WaitForMove(ACTOR_HAMMET);
        Actor_FaceDirection(ACTOR_HAMMET, FACING_NORTH, 0);
        leader = Actor_Get(ACTOR_PARTY_LEADER);
        if (leader != NULL) {
            Actor_SetPosition(ACTOR_IVAN, leader->x.fixed, leader->z.fixed);
        }
        Actor_SetSpeed(ACTOR_IVAN, 0x14ccc, 0xa666);
        Actor_WalkTo(ACTOR_IVAN, 152, 488);
        Actor_WaitForMove(ACTOR_IVAN);
        Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
        leader = Actor_Get(ACTOR_PARTY_LEADER);
        if (leader != NULL) {
            Actor_SetPosition(ACTOR_MIA, leader->x.fixed, leader->z.fixed);
        }
        Actor_SetSpeed(ACTOR_MIA, 0x14ccc, 0xa666);
        Actor_WalkTo(ACTOR_MIA, 168, 488);
        Actor_WaitForMove(ACTOR_MIA);
        Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
        leader = Actor_Get(ACTOR_PARTY_LEADER);
        if (leader != NULL) {
            Actor_SetPosition(ACTOR_GERALD, leader->x.fixed, leader->z.fixed);
        }
        Actor_SetSpeed(ACTOR_GERALD, 0x14ccc, 0xa666);
        Actor_WalkTo(ACTOR_GERALD, 184, 488);
        Actor_WaitForMove(ACTOR_GERALD);
        Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
        Event_Wait(30);

        Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
        sighting = (s32)&LinkedMessage_GeraldSawSomeone;
        Event_SetMessage(sighting + SIGHTING_GERALD_SAW_SOMEONE);
        Event_ShowMessage(ACTOR_GERALD, 0);
        Event_Wait(30);
        Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
        Event_Wait(10);
        Event_SetMessage(sighting + SIGHTING_MIA_SAW_SOMETHING);
        Event_ShowMessage(ACTOR_MIA, 0);
        Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | 2, 70);
        Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
        Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
        Event_SetMessage(sighting + SIGHTING_IVAN_ASKS_IF_FOUND);
        Event_OpenMessage(ACTOR_IVAN, 0);
        Actor_FaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
        Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
        Event_Wait(30);
        if (Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
            Event_SetMessage(sighting + SIGHTING_GERALD_WILL_FIGHT);
            Event_ShowMessage(ACTOR_GERALD, 0);
        } else {
            Event_SetMessage(sighting + SIGHTING_GERALD_ASKS_WHAT_ELSE);
            Event_ShowMessage(ACTOR_GERALD, 0);
        }

        Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | 0, 70);
        recognition = (s32)&LinkedMessage_HammetCallsOut;
        Event_SetMessage(recognition + RECOGNITION_HAMMET_CALLS_OUT);
        Event_ShowMessage(ACTOR_HAMMET, 0);
        Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
        Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
        Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
        Actor_StartRepeatedMotion(ACTOR_MIA, 2);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
        Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
        Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
        Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
        Actor_SetAnimation(ACTOR_HAMMET, ANIM_WALK);
        Actor_WalkBy(ACTOR_HAMMET, 0, -16);
        Actor_WaitForMove(ACTOR_HAMMET);
        Actor_SetAnimation(ACTOR_HAMMET, ANIM_STAND);
        Event_SetMessage(recognition + RECOGNITION_HAMMET_NAMES_BUNZA);
        Event_ShowMessage(ACTOR_HAMMET, 0);
        Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 0, 65);
        Event_SetMessage(recognition + RECOGNITION_BUNZA_KNOWS_VOICE);
        Event_ShowMessage(ACTOR_BUNZA, 0);
        Actor_SetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
        Event_Wait(80);
        Actor_SetSpeed(ACTOR_BUNZA, 0x6666, 0x3333);
        Actor_WalkBy(ACTOR_BUNZA, -13, 0);
        Actor_WaitForMove(ACTOR_BUNZA);
        Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH, 0);
        Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 2, 70);
        Event_SetMessage(recognition + RECOGNITION_BUNZA_NAMES_HAMMET);
        Event_ShowMessage(ACTOR_BUNZA, 0);
        Actor_WalkTo(ACTOR_BUNZA, 168, 432);
        Event_Wait(40);
        Event_CloseScreen();
        Event_WaitForScreen();
        Event_Wait(20);
        Event_End();
        Reunion_Converse();
    }
}

/* Everyone gathers by the cave mouth and talks over how Hammet was saved. */
void Reunion_Converse(void)
{
    s32 reunion;
    s32 plan;

    Event_Begin();
    Actor_SetPosition(ACTOR_GERALD, PIXELS(200), PIXELS(544));
    Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(184), PIXELS(544));
    Actor_SetPosition(ACTOR_MIA, PIXELS(168), PIXELS(544));
    Actor_SetPosition(ACTOR_IVAN, PIXELS(212), PIXELS(528));
    Actor_SetPosition(ACTOR_HAMMET, PIXELS(200), PIXELS(512));
    Actor_SetPosition(ACTOR_BUNZA, PIXELS(168), PIXELS(512));
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_HAMMET, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_BUNZA, FACING_EAST, 0);
    Actor_SetAnimation(ACTOR_BUNZA, ANIM_STAND);
    Camera_FollowActor(ACTOR_PARTY_LEADER, 0);
    Task_Wait(1);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);

    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(30);
    Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 2, 70);
    reunion = (s32)&LinkedMessage_BunzaAsksAboutRelease;
    Event_SetMessage(reunion + REUNION_BUNZA_ASKS_ABOUT_RELEASE);
    Event_ShowMessage(ACTOR_BUNZA, 0);
    Actor_SetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
    Event_SetMessage(reunion + REUNION_HAMMET_CREDITS_IVAN);
    Event_ShowMessage(ACTOR_HAMMET, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_HAMMET, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Event_Wait(40);
    Event_SetMessage(reunion + REUNION_IVAN_CREDITS_LEADER);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 7, 80);
    Event_SetMessage(reunion + REUNION_GERALD_QUESTIONS_IVAN);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GERALD, 0);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | 0, 70);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_GERALD, 0);
    Event_SetMessage(reunion + REUNION_IVAN_CREDITS_EVERYONE);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
    Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTH - FACING_STEP, 0);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTH + FACING_STEP, 0);
    Event_Wait(60);
    Event_SetMessage(reunion + REUNION_HAMMET_THANKS_PARTY);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
    Event_Wait(30);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
    Actor_SetAnimation(ACTOR_GERALD, ANIM_NOD);
    Event_Wait(120);
    Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTH - FACING_STEP, 0);
    Event_Wait(30);
    Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | 8, 80);
    Event_SetMessage(reunion + REUNION_HAMMET_THANKS_IVAN);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_HAMMET, 0);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | 8, 60);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(30);

    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | 1, 40);
    Actor_FaceActor(ACTOR_MIA, ACTOR_BUNZA, 0);
    Event_SetMessage(reunion + REUNION_MIA_PRAISES_BUNZA);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0);
    Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | 0, 60);
    Actor_FaceActor(ACTOR_HAMMET, ACTOR_BUNZA, 0);
    Event_SetMessage(reunion + REUNION_HAMMET_PRAISES_BUNZA);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
    Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 2, 70);
    Event_SetMessage(reunion + REUNION_BUNZA_CALLS_IT_CHANCE);
    Event_ShowMessage(ACTOR_BUNZA, 0);
    Actor_RunRepeatedMotion(ACTOR_BUNZA, 1);
    Event_Wait(20);
    Event_SetMessage(reunion + REUNION_BUNZA_AVOIDED_LUNPA);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    Actor_FaceEachOther(ACTOR_HAMMET, ACTOR_IVAN, 0);
    Event_Wait(60);
    Actor_FaceActor(ACTOR_HAMMET, ACTOR_BUNZA, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_BUNZA, 0);
    Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
    Event_Wait(30);
    Event_SetMessage(reunion + REUNION_BUNZA_KNEW_OF_PRISON);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_BUNZA, 0);
    Event_SetMessage(reunion + REUNION_GERALD_ASKS_WHY_BUNZA_CAME);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GERALD, 0);
    Actor_FaceActor(ACTOR_BUNZA, ACTOR_GERALD, 0);
    Event_Wait(20);
    Event_SetMessage(reunion + REUNION_BUNZA_RECALLS_ADVICE);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    Actor_FaceActor(ACTOR_MIA, ACTOR_BUNZA, 0);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | 1, 70);
    Event_SetMessage(reunion + REUNION_MIA_ASKS_ABOUT_TRADE);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0);
    Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
    Event_Wait(40);
    Event_SetMessage(reunion + REUNION_BUNZA_MEANS_WISDOM);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | 0, 70);
    Event_SetMessage(reunion + REUNION_HAMMET_ON_APPEARANCES);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
    Actor_FaceActor(ACTOR_BUNZA, ACTOR_HAMMET, 0);
    Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
    Event_Wait(30);
    Event_SetMessage(reunion + REUNION_BUNZA_ON_UNPLEASANT_PLACES);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    Actor_SetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
    Event_Wait(30);
    Event_SetMessage(reunion + REUNION_HAMMET_ON_SELLING);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
    Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 8, 70);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
    Event_Wait(30);
    Event_SetMessage(reunion + REUNION_IVAN_ASKS_ABOUT_SERVING);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_BUNZA, ACTOR_IVAN, 0);
    Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
    Event_Wait(30);
    Event_SetMessage(reunion + REUNION_IVAN_ON_FATE);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_SetMessage(reunion + REUNION_GERALD_ASKS_ABOUT_ENTRY);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GERALD, 0);
    Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
    Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
    Event_SetMessage(reunion + REUNION_BUNZA_WAS_REFUSED);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Event_SetMessage(reunion + REUNION_MIA_ASKS_WHY_BUNZA_STILL_CAME);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0);
    Actor_FaceActor(ACTOR_BUNZA, ACTOR_MIA, 0);
    Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 0, 60);
    Event_SetMessage(reunion + REUNION_BUNZA_MENTIONS_COMMOTION);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    Actor_FaceEachOther(ACTOR_IVAN, ACTOR_HAMMET, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(30);
    Event_SetMessage(reunion + REUNION_IVAN_EXPLAINS_COMMOTION);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
    Actor_SetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
    Actor_FaceActor(ACTOR_HAMMET, ACTOR_BUNZA, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_BUNZA, 0);
    Event_Wait(20);
    Event_SetMessage(reunion + REUNION_BUNZA_LINKS_COMMOTION);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    Actor_SetAttachedEffect(ACTOR_BUNZA, EMOTE_IN_FRONT | 2);
    Event_SetMessage(reunion + REUNION_BUNZA_HAD_TO_KNOW);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_BUNZA, 0);
    Event_SetMessage(reunion + REUNION_GERALD_ASKS_ABOUT_CAVE);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GERALD, 0);
    Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 5, 70);
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
    Event_SetMessage(reunion + REUNION_MIA_ON_GATE);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0);
    Actor_RunRepeatedMotion(ACTOR_BUNZA, 1);
    Event_SetMessage(reunion + REUNION_BUNZA_HID);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    Actor_RunRepeatedMotion(ACTOR_BUNZA, 1);
    Event_SetMessage(reunion + REUNION_HAMMET_STARTLED_BUNZA);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
    Event_Wait(30);
    Actor_FaceActor(ACTOR_BUNZA, ACTOR_HAMMET, 0);
    Event_Wait(60);
    Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
    Event_SetMessage(reunion + REUNION_BUNZA_IS_GLAD);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    Actor_SetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
    Event_Wait(30);

    /* Bunza goes to look out of the cave, then hurries back. */
    Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH, 0);
    Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 0, 60);
    Actor_SetSpeed(ACTOR_BUNZA, 0x18000, 0xc000);
    Actor_WalkTo(ACTOR_BUNZA, 144, 528);
    Actor_WaitForMove(ACTOR_BUNZA);
    Actor_WalkTo(ACTOR_BUNZA, 168, 560);
    Actor_WaitForMove(ACTOR_BUNZA);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH + FACING_STEP, 0);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
    Event_Wait(40);
    Actor_WalkTo(ACTOR_BUNZA, 144, 528);
    Actor_WaitForMove(ACTOR_BUNZA);
    Actor_WalkTo(ACTOR_BUNZA, 168, 488);
    Event_SetMessage(reunion + REUNION_IVAN_ASKS_WHY);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
    Actor_WaitForMove(ACTOR_BUNZA);
    Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
    Actor_SetAnimation(ACTOR_BUNZA, ANIM_STAND);
    Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    Event_SetMessage(reunion + REUNION_BUNZA_WARNS_OF_SEARCH);
    Event_ShowMessage(ACTOR_BUNZA, 0);
    Event_Wait(20);
    Actor_FaceEachOther(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
    Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 2, 60);
    Event_SetMessage(reunion + REUNION_BUNZA_FEARS_CAPTURE);
    Event_ShowMessage(ACTOR_BUNZA, 0);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Event_SetMessage(reunion + REUNION_MIA_URGES_ESCAPE);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | 2, 60);
    Event_SetMessage(reunion + REUNION_IVAN_WANTS_STEALTH);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | 2, 40);
    Event_SetMessage(reunion + REUNION_GERALD_ASKS_TO_FIGHT);
    Event_OpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GERALD, 0);
    if (Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
        Event_SetMessage(reunion + REUNION_BUNZA_DISCOURAGES_FIGHT);
        Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    } else {
        Event_SetMessage(reunion + REUNION_BUNZA_AGREES_NOT_TO_FIGHT);
        Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    }

    Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_BUNZA, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_BUNZA, 0);
    Actor_FaceActor(ACTOR_MIA, ACTOR_BUNZA, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_BUNZA, 0);
    Actor_FaceActor(ACTOR_HAMMET, ACTOR_BUNZA, 0);
    Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | 1, 60);
    plan = (s32)&LinkedMessage_HammetAsksPlan;
    Event_SetMessage(plan + PLAN_HAMMET_ASKS_PLAN);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 8, 60);
    Event_SetMessage(plan + PLAN_BUNZA_HAS_WAGON);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | 1, 60);
    Event_SetMessage(plan + PLAN_MIA_ASKS_ABOUT_WAGON);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0);
    Actor_FaceActor(ACTOR_BUNZA, ACTOR_MIA, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
    Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
    Event_SetMessage(plan + PLAN_BUNZA_OFFERS_RIDE);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_BUNZA, 0);
    Event_SetMessage(plan + PLAN_IVAN_DOUBTS_WAGON);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
    Event_SetMessage(plan + PLAN_BUNZA_REASSURES);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    Event_Wait(60);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_BUNZA, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_BUNZA, 0);
    Event_SetMessage(plan + PLAN_GERALD_AGREES);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GERALD, 0);
    Actor_FaceActor(ACTOR_BUNZA, ACTOR_GERALD, 0);
    Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
    Event_Wait(20);
    Event_SetMessage(plan + PLAN_BUNZA_IS_UNSUSPECTED);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    Actor_FaceEachOther(ACTOR_HAMMET, ACTOR_IVAN, 0);
    Event_Wait(60);
    Actor_SetAnimation(ACTOR_HAMMET, ANIM_NOD);
    Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(60);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimation(ACTOR_GERALD, ANIM_NOD);
    Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
    Actor_FaceActor(ACTOR_BUNZA, ACTOR_PARTY_LEADER, 0);
    Actor_SetAnimation(ACTOR_BUNZA, ANIM_NOD);
    Event_Wait(60);
    WagonChoice_Run();
}

/*
 * Bunza leads the way to his wagon and Gerald asks whether the party rides
 * too. The party questions an answer that seems to change its mind until
 * the choice is settled either way.
 */
void WagonChoice_Run(void)
{
    s32 wagon;
    s32 insisted;
    s32 confusion;

    wagon = (s32)&LinkedMessage_BunzaLeadsToWagon;
    Event_SetMessage(wagon + WAGON_BUNZA_LEADS_THE_WAY);
    Event_ShowMessage(ACTOR_BUNZA, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Event_SetMessage(wagon + WAGON_GERALD_ASKS_TO_RIDE);
    Event_OpenMessage(ACTOR_GERALD, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_HAMMET, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_BUNZA, ACTOR_PARTY_LEADER, 0);

ask_to_ride:
    if (Leader_AnswersYes()) {
ask_about_business:
        if (!Bunza_AsksAboutUnfinishedBusiness()) {
            goto ride;
        }
        insisted = FALSE;
        if (!Gerald_AsksAboutThingsToDo()) {
insist_nothing_left:
            insisted = TRUE;
check_nothing_left:
            Gerald_ChecksNothingLeftToDo();
            if (Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
                goto ride;
            }
        }
        if (Bunza_CannotWait()) {
            goto stay;
        }
        if (!insisted) {
            goto stay;
        }
        goto check_nothing_left;
    } else {
        if (Gerald_AsksIfNotRiding()) {
            if (Gerald_AsksAboutUnfinishedBusiness()) {
                goto stay;
            }
            goto insist_nothing_left;
        } else {
            if (Mia_AsksIfRidingAfterAll()) {
                goto ask_about_business;
            }
            confusion = (s32)&LinkedMessage_IvanIsConfused;
            Event_SetMessage(confusion + CONFUSION_IVAN_IS_CONFUSED);
            Event_ShowMessage(ACTOR_IVAN, 0);
            Event_SetMessage(confusion + CONFUSION_GERALD_ASKS_AGAIN);
            Event_OpenMessage(ACTOR_GERALD, 0);
            goto ask_to_ride;
ride:
            Party_RidesWagon();
            goto done;
        }
    }
stay:
    Party_ConfirmsStaying();
    Party_StaysBehind();
done:;
}

u8 Leader_AnswersYes(void)
{
    return Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

u8 Gerald_AsksIfNotRiding(void)
{
    Event_SetMessage(MSG_GERALD_ASKS_IF_NOT_RIDING);
    Event_OpenMessage(ACTOR_GERALD, 0);
    return Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

u8 Gerald_AsksAboutUnfinishedBusiness(void)
{
    Event_SetMessage(MSG_GERALD_ASKS_ABOUT_UNFINISHED_BUSINESS);
    Event_OpenMessage(ACTOR_GERALD, 0);
    return Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

u8 Party_ConfirmsStaying(void)
{
    return TRUE;
}

u8 Bunza_AsksAboutUnfinishedBusiness(void)
{
    Event_SetMessage(MSG_BUNZA_ASKS_ABOUT_UNFINISHED_BUSINESS);
    Event_OpenMessage(ACTOR_BUNZA, 0);
    return Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

u8 Gerald_ChecksNothingLeftToDo(void)
{
    Event_SetMessage(MSG_GERALD_CHECKS_NOTHING_LEFT);
    Event_OpenMessage(ACTOR_GERALD, 0);
    return TRUE;
}

u8 Mia_AsksIfRidingAfterAll(void)
{
    Event_SetMessage(MSG_MIA_ASKS_IF_RIDING_AFTER_ALL);
    Event_OpenMessage(ACTOR_MIA, 0);
    return Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

/* Bunza cannot wait any longer, and Mia asks whether the party stays. */
u8 Bunza_CannotWait(void)
{
    s32 warning;

    Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | 2, 60);
    warning = (s32)&LinkedMessage_IvanAsksIfStaying;
    Event_SetMessage(warning + WARNING_IVAN_ASKS_IF_STAYING);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
    Event_SetMessage(warning + WARNING_BUNZA_CANNOT_WAIT);
    Event_ShowMessage(ACTOR_BUNZA, 0);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | 2, 60);
    Event_SetMessage(warning + WARNING_MIA_ASKS_ABOUT_BUSINESS);
    Event_OpenMessage(ACTOR_MIA, 0);
    return Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

u8 Gerald_AsksAboutThingsToDo(void)
{
    Event_SetMessage(MSG_GERALD_ASKS_ABOUT_THINGS_TO_DO);
    Event_OpenMessage(ACTOR_GERALD, 0);
    return Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

/*
 * Hammet and Bunza say goodbye and leave for the wagon; the party watches
 * them go, then falls in behind the leader.
 */
void Party_StaysBehind(void)
{
    struct FieldActor *leader;
    s32 farewell;

    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 5, 60);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    farewell = (s32)&LinkedMessage_GeraldStays;
    Event_SetMessage(farewell + FAREWELL_GERALD_STAYS);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Event_SetMessage(farewell + FAREWELL_MIA_STAYS);
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_HAMMET, 0);
    Event_Wait(60);
    Event_SetMessage(farewell + FAREWELL_IVAN_STAYS);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_HAMMET, ACTOR_IVAN, 0);
    Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | 5, 70);
    Event_SetMessage(farewell + FAREWELL_HAMMET_LETS_IVAN_GO);
    Event_ShowMessage(ACTOR_HAMMET, 0);
    Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
    Event_SetMessage(farewell + FAREWELL_BUNZA_SAYS_GOODBYE);
    Event_ShowMessage(ACTOR_BUNZA, 0);
    Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimation(ACTOR_GERALD, ANIM_NOD);
    Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
    Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
    Event_Wait(60);
    Actor_SetDestinationOffset(ACTOR_HAMMET, -16, 0);
    Actor_WaitForMove(ACTOR_HAMMET);
    Actor_SetAnimation(ACTOR_HAMMET, ANIM_STAND);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
    Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTH + FACING_STEP, 0);
    Event_Wait(30);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimation(ACTOR_GERALD, ANIM_NOD);
    Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
    Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
    Actor_WalkTo(ACTOR_BUNZA, 156, 528);
    Event_Wait(20);
    Actor_WalkTo(ACTOR_HAMMET, 164, 528);
    Actor_WaitForMove(ACTOR_BUNZA);
    Actor_WalkTo(ACTOR_BUNZA, 168, 640);
    Actor_WaitForMove(ACTOR_HAMMET);
    Actor_WalkTo(ACTOR_HAMMET, 168, 640);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH + FACING_STEP, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH, 0);
    Event_Wait(60);
    Actor_SetPosition(ACTOR_HAMMET, 0, 0);
    Actor_SetPosition(ACTOR_BUNZA, 0, 0);
    Event_Wait(110);

    Event_SetMessage(farewell + FAREWELL_GERALD_SIGHS);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    Event_Wait(30);
    Event_SetMessage(farewell + FAREWELL_MIA_HOPES_FOR_SAFETY);
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_SetMessage(farewell + FAREWELL_IVAN_REASSURES);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_Wait(140);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Event_SetMessage(farewell + FAREWELL_GERALD_MOVES_ON);
    Event_ShowMessage(ACTOR_GERALD, 0);

    Actor_SetAnimation(ACTOR_GERALD, ANIM_WALK);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
        Actor_SetDestination(ACTOR_GERALD, leader->x.part.pixel, leader->z.part.pixel);
    }
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Actor_SetAnimation(ACTOR_IVAN, ANIM_WALK);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
        Actor_SetDestination(ACTOR_IVAN, leader->x.part.pixel, leader->z.part.pixel);
    }
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Actor_SetAnimation(ACTOR_MIA, ANIM_WALK);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
        Actor_SetDestination(ACTOR_MIA, leader->x.part.pixel, leader->z.part.pixel);
    }
    Actor_WaitForMove(ACTOR_MIA);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    Event_Wait(30);
    Camera_MoveToActor(ACTOR_PARTY_LEADER, 1);
    Camera_WaitForMove();
    Camera_FollowActor(ACTOR_PARTY_LEADER, 0);
    GameFlag_Set(FLAG_PARTY_STAYED_IN_LUNPA);
}

/*
 * Everyone heads for the wagon: Hammet and Bunza lead, the party falls in
 * behind the leader, and the leader follows them out of the cave.
 */
void Party_RidesWagon(void)
{
    struct FieldActor *leader;
    s32 departure;

    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH + FACING_STEP, 0);
    departure = (s32)&LinkedMessage_GeraldHeadsForKalay;
    Event_SetMessage(departure + DEPARTURE_GERALD_HEADS_FOR_KALAY);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_SetMessage(departure + DEPARTURE_IVAN_THINKS_OF_LAYANA);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_HAMMET, ACTOR_IVAN, 0);
    Actor_SetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
    Event_Wait(20);
    Event_SetMessage(departure + DEPARTURE_HAMMET_LONGS_FOR_LAYANA);
    Event_ShowMessage(ACTOR_HAMMET, 0);
    Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
    Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
    Event_Wait(30);
    Event_SetMessage(departure + DEPARTURE_BUNZA_SETS_OFF);
    Event_ShowMessage(ACTOR_BUNZA, 0);
    Actor_SetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimation(ACTOR_GERALD, ANIM_NOD);
    Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
    Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
    Event_Wait(80);
    Actor_SetDestinationOffset(ACTOR_HAMMET, -16, 0);
    Actor_WaitForMove(ACTOR_HAMMET);
    Actor_SetAnimation(ACTOR_HAMMET, ANIM_STAND);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
    Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTH + FACING_STEP, 0);
    Event_Wait(30);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimation(ACTOR_GERALD, ANIM_NOD);
    Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
    Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
    Actor_WalkTo(ACTOR_BUNZA, 152, 528);
    Event_Wait(20);
    Actor_WalkTo(ACTOR_HAMMET, 160, 528);
    Actor_WaitForMove(ACTOR_BUNZA);
    Actor_WalkTo(ACTOR_BUNZA, 168, 640);
    Actor_WaitForMove(ACTOR_HAMMET);
    Actor_WalkTo(ACTOR_HAMMET, 168, 640);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH + FACING_STEP, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH, 0);
    Event_Wait(200);
    Actor_SetPosition(ACTOR_HAMMET, 0, 0);
    Actor_SetPosition(ACTOR_BUNZA, 0, 0);

    Actor_SetAnimation(ACTOR_GERALD, ANIM_WALK);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
        Actor_SetDestination(ACTOR_GERALD, leader->x.part.pixel, leader->z.part.pixel);
    }
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Actor_SetAnimation(ACTOR_IVAN, ANIM_WALK);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
        Actor_SetDestination(ACTOR_IVAN, leader->x.part.pixel, leader->z.part.pixel);
    }
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Actor_SetAnimation(ACTOR_MIA, ANIM_WALK);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
        Actor_SetDestination(ACTOR_MIA, leader->x.part.pixel, leader->z.part.pixel);
    }
    Actor_WaitForMove(ACTOR_MIA);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    Event_Wait(30);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, -16, 0);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Camera_MoveToActor(ACTOR_PARTY_LEADER, 1);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 168, 640);
    Event_Wait(60);
    Event_CloseScreen();
    Event_RequestExit(CAVE_EXIT_BY_WAGON);
}

s32 Scene_Initialize(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (gGameState.scene == (s32)&LinkedScene_RunpaDou) {
        Actor_SetSpriteFlags(Actor_Get(ACTOR_HIDDEN_PUDDLE), 0);
        Actor_SetSpriteFlags(Actor_Get(ACTOR_SOUTH_PUDDLE), 0);
        Actor_SetSpriteFlags(Actor_Get(ACTOR_GATE_PUDDLE), 0);
        Actor_SetSpriteFlags(Actor_Get(ACTOR_NORTH_PUDDLE), 0);
        Actor_Get(ACTOR_NORTH_PUDDLE)->scale_y = 0xf333;
        if (GameFlag_IsSet(FLAG_CAVE_GATE_LOWERED) != 0) {
            Gate_Lower();
        }
        if (GameFlag_IsSet(FLAG_CAVE_GATE_RAISED) != 0) {
            Gate_Raise();
        }
        if (GameFlag_IsSet(FLAG_CAVE_GATE_PROPPED) != 0) {
            Gate_DrawPropped();
        }
        if (GameFlag_IsSet(FLAG_CAVE_NORTH_PILLAR) != 0) {
            Actor_SetAnimation(ACTOR_NORTH_PUDDLE, PUDDLE_ANIM_FROZEN);
        }
        if (GameFlag_IsSet(FLAG_CAVE_SOUTH_PILLAR) != 0) {
            Actor_SetAnimation(ACTOR_SOUTH_PUDDLE, PUDDLE_ANIM_FROZEN);
        }
    }
    return 0;
}

const struct SceneEntrance gCaveEntrances[] = {
    { 0, CONDITION_ALWAYS, 168, 0, 632, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { CAVE_ENTRANCE_FROM_WORLD_MAP, CONDITION_ALWAYS, 168, 0, 632, FACING_NORTH, 0, 88, 56, 648, 648, 0 },
    { CAVE_ENTRANCE_FROM_LUNPA, CONDITION_ALWAYS, 552, 0, 328, FACING_NORTH, 0, 88, 56, 648, 648, 0 },
    { CAVE_ENTRANCE_THIRD, CONDITION_ALWAYS, 552, 0, 328, FACING_NORTH, 0, 88, 56, 648, 488, 0 },
    { SCENE_TABLE_END },
};

const u32 gCaveExits[] = {
    SCENE_EXITS(SCENE_RUNPA_DOU),
    SCENE_EXIT(CAVE_EXIT_TO_WORLD_MAP, SCENE_WORLD_MAP, WORLD_MAP_ENTRANCE_FROM_CAVE),
    SCENE_EXIT(CAVE_EXIT_TO_LUNPA, SCENE_RUNPA_MURA, LUNPA_ENTRANCE_FROM_CAVE),
    SCENE_EXIT(CAVE_EXIT_BY_WAGON, SCENE_WORLD_MAP, WORLD_MAP_ENTRANCE_FROM_CAVE),
    SCENE_EXITS_END,
};

const struct ScenePlacement gCaveNoPlacements[] = {
    { SCENE_TABLE_END },
};

/* Hammet, Bunza and the party wait out of sight until the reunion places them. */
const struct ScenePlacement gCavePlacements[] = {
    { SPRITE_PUDDLE, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(136), 0, PIXELS(520), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(344), 0, PIXELS(248), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(424), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BUNZA, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_HAMMET, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_WEST, TALK_KEEP_FACING, 0 },
    { ACTOR_IVAN, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { ACTOR_MIA, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { ACTOR_GERALD, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

const struct SceneEvent gCaveNoEvents[] = {
    { SCENE_EVENTS_END },
};

const struct SceneEvent gCaveEvents[] = {
    { EVENT_EXIT, TRIGGER_WORLD_MAP_PASSAGE, CONDITION_ALWAYS, CAVE_EXIT_TO_WORLD_MAP },
    { EVENT_EXIT, TRIGGER_LUNPA_PASSAGE, CONDITION_ALWAYS, CAVE_EXIT_TO_LUNPA },
    { EVENT_TOUCH, TRIGGER_BUNZA_HIDING_PLACE, FLAG_LUNPA_CAVE_REUNION_SEEN,
      EVENT_SCRIPT(Reunion_Begin) },
    { SEARCH(SEARCH_UNNAMED), TRIGGER_LOWERING_SWITCH, CONDITION_ALWAYS,
      EVENT_SCRIPT(LoweringSwitch_Flip) },
    { SEARCH(SEARCH_UNNAMED), TRIGGER_RAISING_SWITCH, CONDITION_ALWAYS,
      EVENT_SCRIPT(RaisingSwitch_Flip) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_HIDDEN_PUDDLE, CONDITION_ALWAYS,
      EVENT_SCRIPT(HiddenPuddle_Freeze) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SOUTH_PUDDLE, CONDITION_ALWAYS,
      EVENT_SCRIPT(SouthPuddle_Freeze) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_GATE_PUDDLE, CONDITION_ALWAYS,
      EVENT_SCRIPT(GatePuddle_Freeze) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_NORTH_PUDDLE, CONDITION_ALWAYS,
      EVENT_SCRIPT(NorthPuddle_Freeze) },
    { SCENE_EVENTS_END },
};
