#include "CAVE.H"

extern u8 MsgFieldFlippedSwitch[];

extern u8 MsgRunpaBunza[];
extern u8 MsgRunpaDodonpaNeverIntention[];
extern u8 MsgRunpaMoment[];
extern u8 MsgRunpaThinkSawSomeone[];

extern u8 MsgRunpaBunzaAsksAboutUnfinishedBusiness[];
extern u8 MsgRunpaGeraldAsksAboutUnfinishedBusiness[];
extern u8 MsgRunpaGeraldAsksIfNotRiding[];
extern u8 MsgRunpaGeraldChecksNothingLeft[];
extern u8 MsgRunpaLetsTakeWagon[];
extern u8 MsgRunpaMiaAsksIfRidingAfter[];
extern u8 MsgRunpaTotallyConfusedChanged[];

extern u8 MsgRunpaMeaningWontRide[];

extern u8 MsgRunpaGeraldAsksAboutThingsTo[];

extern u8 MsgRunpaDontUnfinishedBusiness[];
extern u8 MsgRunpaInsistStick[];

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

/* The cave's actors stand only in its own scene. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.scene == (s32)&SceneId_RunpaDou) {
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

    puddle = Object_GetById(ACTOR_HIDDEN_PUDDLE);
    if (puddle != NULL) {
        Engine_ActorSetSpriteFlags(puddle, 0);
    }
}

/* The pillar raised from the south puddle changes the ground it stands on. */
void SouthPuddle_Freeze(void)
{
    struct FieldActor *puddle;

    puddle = Object_GetById(ACTOR_SOUTH_PUDDLE);
    if (puddle != NULL) {
        puddle->priority_flags = ACTOR_PRIORITY_AUTOMATIC;
        puddle->motion_flags = 0;
    }
    Engine_MapCopyCellAttributes(7, 32, 1, 1, 8, 32);
    Engine_GameFlagSet(FLAG_CAVE_SOUTH_PILLAR);
}

/* The gate rises onto the frozen pillar and stays there. */
void Gate_DrawPropped(void)
{
    struct FieldActor *pillar;

    pillar = Object_GetById(ACTOR_GATE_PUDDLE);
    Engine_ActorSetAnimation(ACTOR_GATE_PUDDLE, PUDDLE_ANIM_FROZEN);
    if (pillar != NULL) {
        Engine_ActorSetSpriteFlags(pillar, 0);
        pillar->priority_flags = ACTOR_PRIORITY_AUTOMATIC;
    }
    Engine_MapCopyCells(41, 87, 2, 5, 21, 59);
    Engine_TaskWait(4);
    Engine_MapCopyCells(3, 93, 1, 1, 24, 62);
    Engine_MapCopyCells(1, 94, 1, 1, 21, 55);
    Engine_MapCopyCells(43, 87, 2, 5, 21, 58);
    Engine_TaskWait(4);
    Engine_MapCopyCells(41, 87, 2, 5, 21, 58);
    Engine_TaskWait(4);
    Engine_TaskWait(4);
    Engine_MapCopyCellAttributes(21, 11, 2, 2, 21, 13);
    Engine_MapCopyCellAttributes(21, 11, 1, 1, 22, 15);
    Engine_MapCopyCellAttributes(19, 17, 1, 1, 21, 14);
}

void GatePuddle_Freeze(void)
{
    struct FieldActor *pillar;

    pillar = Object_GetById(ACTOR_GATE_PUDDLE);
    Engine_GameFlagSet(FLAG_CAVE_GATE_PROPPED);
    if (pillar != NULL) {
        Engine_ActorSetSpriteFlags(pillar, 0);
        pillar->priority_flags = ACTOR_PRIORITY_AUTOMATIC;
    }
    if (Engine_GameFlagIsSet(FLAG_CAVE_GATE_RAISED) == 0) {
        Engine_AudioPlayCue(SOUND_GATE_MOVE);
        Gate_DrawPropped();
        Engine_AudioPlayCue(SOUND_PUZZLE_SOLVED);
        Engine_GameFlagSet(FLAG_CAVE_GATE_RAISED);
    }
}

void NorthPuddle_Freeze(void)
{
    Engine_GameFlagSet(FLAG_CAVE_NORTH_PILLAR);
}

void Gate_Lower(void)
{
    if (Engine_GameFlagIsSet(FLAG_CAVE_GATE_RAISED) != 0) {
        Engine_MapCopyCells(41, 86, 2, 6, 21, 57);
        Engine_TaskWait(4);
        Engine_MapCopyCells(43, 86, 2, 6, 21, 57);
        Engine_TaskWait(4);
        Engine_MapCopyCells(41, 86, 2, 6, 21, 58);
        Engine_TaskWait(4);
        Engine_MapCopyCells(43, 86, 2, 6, 21, 58);
        Engine_TaskWait(4);
    }
    Engine_MapCopyCells(2, 93, 1, 1, 24, 62);
    Engine_MapCopyCells(2, 94, 1, 1, 21, 55);
    Engine_MapCopyCells(41, 86, 2, 6, 21, 59);
    Engine_TaskWait(4);
    Engine_MapCopyCells(1, 93, 1, 1, 24, 62);
    Engine_MapCopyCells(3, 94, 1, 1, 21, 55);
    Engine_MapCopyCells(43, 86, 2, 6, 21, 59);
    Engine_TaskWait(4);
    Engine_ActorSetSpritePriority(ACTOR_GATE_PUDDLE, GATE_PILLAR_PRIORITY);
    Engine_MapCopyCellAttributes(19, 17, 1, 1, 22, 15);
}

void LoweringSwitch_Flip(void)
{
    if (Engine_GameFlagIsSet(FLAG_CAVE_GATE_LOWERED) == 0) {
        if (Engine_GameFlagIsSet(FLAG_CAVE_GATE_PROPPED) == 0) {
            Engine_MessageShowCentered((s32)MsgFieldFlippedSwitch, 1);
            Engine_AudioPlayCue(SOUND_GATE_MOVE);
            Gate_Lower();
            Engine_GameFlagSet(FLAG_CAVE_GATE_LOWERED);
            Engine_GameFlagClear(FLAG_CAVE_GATE_RAISED);
        }
    }
}

void Gate_Raise(void)
{
    Engine_MapCopyCells(41, 87, 2, 5, 21, 59);
    Engine_TaskWait(4);
    Engine_MapCopyCells(2, 93, 1, 1, 24, 62);
    Engine_MapCopyCells(2, 94, 1, 1, 21, 55);
    Engine_MapCopyCells(43, 87, 2, 5, 21, 58);
    Engine_TaskWait(4);
    Engine_MapCopyCells(3, 93, 1, 1, 24, 62);
    Engine_MapCopyCells(1, 94, 1, 1, 21, 55);
    Engine_MapCopyCells(41, 87, 2, 5, 21, 58);
    Engine_MapCopyCellAttributes(21, 11, 2, 2, 21, 13);
    Engine_MapCopyCellAttributes(19, 17, 1, 1, 21, 14);
}

void RaisingSwitch_Flip(void)
{
    if (Engine_GameFlagIsSet(FLAG_CAVE_GATE_PROPPED) == 0) {
        if (Engine_GameFlagIsSet(FLAG_CAVE_GATE_RAISED) == 0) {
            Engine_MessageShowCentered((s32)MsgFieldFlippedSwitch, 1);
            Engine_AudioPlayCue(SOUND_GATE_MOVE);
            Gate_Raise();
            Engine_GameFlagSet(FLAG_CAVE_GATE_RAISED);
            Engine_GameFlagClear(FLAG_CAVE_GATE_LOWERED);
        }
    }
}

/* Bunza and Hammet meet again at the cave mouth once Hammet is free. */

/*
 * Once Hammet is free, Bunza steps out from where he hid, Hammet and the
 * party gather around him, and the two recognize each other.
 */
void Reunion_Begin(void)
{
    struct FieldActor *leader;
    s32 sighting;
    s32 recognition;

    if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        Engine_GameFlagSet(FLAG_LUNPA_CAVE_REUNION_SEEN);
        Engine_EventBegin();
        Engine_ActorSetPosition(ACTOR_BUNZA, PIXELS(144), PIXELS(400));
        Engine_ActorSetSpeed(ACTOR_BUNZA, 0x18000, 0xc000);
        Engine_ActorWalkTo(ACTOR_BUNZA, 184, 400);
        Engine_ActorWaitForMove(ACTOR_BUNZA);
        Engine_ActorSetAnimation(ACTOR_BUNZA, ANIM_STAND);
        Engine_ActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
        Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
        Engine_EventWait(30);
        Engine_CameraSetSpeed(0x8000, 0x1000);
        Engine_CameraMoveTo(PIXELS(192), -1, PIXELS(432), 1);
        leader = Object_GetById(ACTOR_PARTY_LEADER);
        if (leader != NULL) {
            Engine_ActorSetPosition(ACTOR_HAMMET, leader->x.fixed, leader->z.fixed);
        }
        Engine_ActorSetSpeed(ACTOR_HAMMET, 0x14ccc, 0xa666);
        Engine_ActorWalkTo(ACTOR_HAMMET, 168, 464);
        Engine_ActorWaitForMove(ACTOR_HAMMET);
        Engine_ActorFaceDirection(ACTOR_HAMMET, FACING_NORTH, 0);
        leader = Object_GetById(ACTOR_PARTY_LEADER);
        if (leader != NULL) {
            Engine_ActorSetPosition(ACTOR_IVAN, leader->x.fixed, leader->z.fixed);
        }
        Engine_ActorSetSpeed(ACTOR_IVAN, 0x14ccc, 0xa666);
        Engine_ActorWalkTo(ACTOR_IVAN, 152, 488);
        Engine_ActorWaitForMove(ACTOR_IVAN);
        Engine_ActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
        leader = Object_GetById(ACTOR_PARTY_LEADER);
        if (leader != NULL) {
            Engine_ActorSetPosition(ACTOR_MIA, leader->x.fixed, leader->z.fixed);
        }
        Engine_ActorSetSpeed(ACTOR_MIA, 0x14ccc, 0xa666);
        Engine_ActorWalkTo(ACTOR_MIA, 168, 488);
        Engine_ActorWaitForMove(ACTOR_MIA);
        Engine_ActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
        leader = Object_GetById(ACTOR_PARTY_LEADER);
        if (leader != NULL) {
            Engine_ActorSetPosition(ACTOR_GERALD, leader->x.fixed, leader->z.fixed);
        }
        Engine_ActorSetSpeed(ACTOR_GERALD, 0x14ccc, 0xa666);
        Engine_ActorWalkTo(ACTOR_GERALD, 184, 488);
        Engine_ActorWaitForMove(ACTOR_GERALD);
        Engine_ActorFaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
        Engine_EventWait(30);

        Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 1);
        sighting = (s32)MsgRunpaThinkSawSomeone;
        Engine_EventSetMessage(sighting + SIGHTING_GERALD_SAW_SOMEONE);
        Engine_EventShowMessage(ACTOR_GERALD, 0);
        Engine_EventWait(30);
        Engine_ActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
        Engine_EventWait(10);
        Engine_EventSetMessage(sighting + SIGHTING_MIA_SAW_SOMETHING);
        Engine_EventShowMessage(ACTOR_MIA, 0);
        Engine_ActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | 2, 70);
        Engine_ActorFaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
        Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
        Engine_EventSetMessage(sighting + SIGHTING_IVAN_ASKS_IF_FOUND);
        Engine_EventOpenMessage(ACTOR_IVAN, 0);
        Engine_ActorFaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
        Engine_ActorFaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
        Engine_EventWait(30);
        if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
            Engine_EventSetMessage(sighting + SIGHTING_GERALD_WILL_FIGHT);
            Engine_EventShowMessage(ACTOR_GERALD, 0);
        } else {
            Engine_EventSetMessage(sighting + SIGHTING_GERALD_ASKS_WHAT_ELSE);
            Engine_EventShowMessage(ACTOR_GERALD, 0);
        }

        Engine_ActorShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | 0, 70);
        recognition = (s32)MsgRunpaMoment;
        Engine_EventSetMessage(recognition + RECOGNITION_HAMMET_CALLS_OUT);
        Engine_EventShowMessage(ACTOR_HAMMET, 0);
        Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
        Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
        Engine_ActorStartRepeatedMotion(ACTOR_IVAN, 2);
        Engine_ActorStartRepeatedMotion(ACTOR_MIA, 2);
        Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
        Engine_ActorFaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
        Engine_ActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
        Engine_ActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
        Engine_ActorSetAnimation(ACTOR_HAMMET, ANIM_WALK);
        Engine_ActorWalkBy(ACTOR_HAMMET, 0, -16);
        Engine_ActorWaitForMove(ACTOR_HAMMET);
        Engine_ActorSetAnimation(ACTOR_HAMMET, ANIM_STAND);
        Engine_EventSetMessage(recognition + RECOGNITION_HAMMET_NAMES_BUNZA);
        Engine_EventShowMessage(ACTOR_HAMMET, 0);
        Engine_ActorShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 0, 65);
        Engine_EventSetMessage(recognition + RECOGNITION_BUNZA_KNOWS_VOICE);
        Engine_EventShowMessage(ACTOR_BUNZA, 0);
        Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
        Engine_EventWait(80);
        Engine_ActorSetSpeed(ACTOR_BUNZA, 0x6666, 0x3333);
        Engine_ActorWalkBy(ACTOR_BUNZA, -13, 0);
        Engine_ActorWaitForMove(ACTOR_BUNZA);
        Engine_ActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH, 0);
        Engine_ActorShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 2, 70);
        Engine_EventSetMessage(recognition + RECOGNITION_BUNZA_NAMES_HAMMET);
        Engine_EventShowMessage(ACTOR_BUNZA, 0);
        Engine_ActorWalkTo(ACTOR_BUNZA, 168, 432);
        Engine_EventWait(40);
        Engine_EventCloseScreen();
        Engine_EventWaitForScreen();
        Engine_EventWait(20);
        Engine_EventEnd();
        Reunion_Converse();
    }
}

/* Everyone gathers by the cave mouth and talks over how Hammet was saved. */
void Reunion_Converse(void)
{
    s32 reunion;
    s32 plan;

    Engine_EventBegin();
    Engine_ActorSetPosition(ACTOR_GERALD, PIXELS(200), PIXELS(544));
    Engine_ActorSetPosition(ACTOR_PARTY_LEADER, PIXELS(184), PIXELS(544));
    Engine_ActorSetPosition(ACTOR_MIA, PIXELS(168), PIXELS(544));
    Engine_ActorSetPosition(ACTOR_IVAN, PIXELS(212), PIXELS(528));
    Engine_ActorSetPosition(ACTOR_HAMMET, PIXELS(200), PIXELS(512));
    Engine_ActorSetPosition(ACTOR_BUNZA, PIXELS(168), PIXELS(512));
    Engine_ActorFaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Engine_ActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Engine_ActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
    Engine_ActorFaceDirection(ACTOR_HAMMET, FACING_WEST, 0);
    Engine_ActorFaceDirection(ACTOR_BUNZA, FACING_EAST, 0);
    Engine_ActorSetAnimation(ACTOR_BUNZA, ANIM_STAND);
    Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
    Engine_TaskWait(1);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);

    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Engine_EventWait(30);
    Engine_ActorShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 2, 70);
    reunion = (s32)MsgRunpaDodonpaNeverIntention;
    Engine_EventSetMessage(reunion + REUNION_BUNZA_ASKS_ABOUT_RELEASE);
    Engine_EventShowMessage(ACTOR_BUNZA, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
    Engine_EventSetMessage(reunion + REUNION_HAMMET_CREDITS_IVAN);
    Engine_EventShowMessage(ACTOR_HAMMET, 0);
    Engine_ActorFaceActor(ACTOR_IVAN, ACTOR_HAMMET, 0);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 1);
    Engine_EventWait(40);
    Engine_EventSetMessage(reunion + REUNION_IVAN_CREDITS_LEADER);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
    Engine_ActorFaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Engine_EventWait(10);
    Engine_ActorShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 7, 80);
    Engine_EventSetMessage(reunion + REUNION_GERALD_QUESTIONS_IVAN);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GERALD, 0);
    Engine_ActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | 0, 70);
    Engine_ActorFaceActor(ACTOR_IVAN, ACTOR_GERALD, 0);
    Engine_EventSetMessage(reunion + REUNION_IVAN_CREDITS_EVERYONE);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
    Engine_ActorFaceDirection(ACTOR_HAMMET, FACING_SOUTH - FACING_STEP, 0);
    Engine_EventWait(60);
    Engine_ActorFaceDirection(ACTOR_HAMMET, FACING_SOUTH + FACING_STEP, 0);
    Engine_EventWait(60);
    Engine_EventSetMessage(reunion + REUNION_HAMMET_THANKS_PARTY);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_GERALD, ANIM_NOD);
    Engine_EventWait(120);
    Engine_ActorFaceDirection(ACTOR_HAMMET, FACING_SOUTH - FACING_STEP, 0);
    Engine_EventWait(30);
    Engine_ActorShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | 8, 80);
    Engine_EventSetMessage(reunion + REUNION_HAMMET_THANKS_IVAN);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
    Engine_ActorFaceActor(ACTOR_IVAN, ACTOR_HAMMET, 0);
    Engine_ActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | 8, 60);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Engine_EventWait(30);

    Engine_ActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | 1, 40);
    Engine_ActorFaceActor(ACTOR_MIA, ACTOR_BUNZA, 0);
    Engine_EventSetMessage(reunion + REUNION_MIA_PRAISES_BUNZA);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0);
    Engine_ActorShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | 0, 60);
    Engine_ActorFaceActor(ACTOR_HAMMET, ACTOR_BUNZA, 0);
    Engine_EventSetMessage(reunion + REUNION_HAMMET_PRAISES_BUNZA);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
    Engine_ActorShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 2, 70);
    Engine_EventSetMessage(reunion + REUNION_BUNZA_CALLS_IT_CHANCE);
    Engine_EventShowMessage(ACTOR_BUNZA, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_BUNZA, 1);
    Engine_EventWait(20);
    Engine_EventSetMessage(reunion + REUNION_BUNZA_AVOIDED_LUNPA);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    Engine_ActorFaceEachOther(ACTOR_HAMMET, ACTOR_IVAN, 0);
    Engine_EventWait(60);
    Engine_ActorFaceActor(ACTOR_HAMMET, ACTOR_BUNZA, 0);
    Engine_ActorFaceActor(ACTOR_IVAN, ACTOR_BUNZA, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
    Engine_EventWait(30);
    Engine_EventSetMessage(reunion + REUNION_BUNZA_KNEW_OF_PRISON);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 1);
    Engine_EventWait(20);
    Engine_ActorFaceActor(ACTOR_GERALD, ACTOR_BUNZA, 0);
    Engine_EventSetMessage(reunion + REUNION_GERALD_ASKS_WHY_BUNZA_CAME);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GERALD, 0);
    Engine_ActorFaceActor(ACTOR_BUNZA, ACTOR_GERALD, 0);
    Engine_EventWait(20);
    Engine_EventSetMessage(reunion + REUNION_BUNZA_RECALLS_ADVICE);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    Engine_ActorFaceActor(ACTOR_MIA, ACTOR_BUNZA, 0);
    Engine_ActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | 1, 70);
    Engine_EventSetMessage(reunion + REUNION_MIA_ASKS_ABOUT_TRADE);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
    Engine_EventWait(40);
    Engine_EventSetMessage(reunion + REUNION_BUNZA_MEANS_WISDOM);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    Engine_ActorShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | 0, 70);
    Engine_EventSetMessage(reunion + REUNION_HAMMET_ON_APPEARANCES);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
    Engine_ActorFaceActor(ACTOR_BUNZA, ACTOR_HAMMET, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
    Engine_EventWait(30);
    Engine_EventSetMessage(reunion + REUNION_BUNZA_ON_UNPLEASANT_PLACES);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
    Engine_EventWait(30);
    Engine_EventSetMessage(reunion + REUNION_HAMMET_ON_SELLING);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
    Engine_ActorShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 8, 70);
    Engine_ActorStartRepeatedMotion(ACTOR_IVAN, 1);
    Engine_EventWait(30);
    Engine_EventSetMessage(reunion + REUNION_IVAN_ASKS_ABOUT_SERVING);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
    Engine_ActorFaceActor(ACTOR_BUNZA, ACTOR_IVAN, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
    Engine_EventWait(30);
    Engine_EventSetMessage(reunion + REUNION_IVAN_ON_FATE);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 1);
    Engine_EventSetMessage(reunion + REUNION_GERALD_ASKS_ABOUT_ENTRY);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GERALD, 0);
    Engine_ActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
    Engine_EventSetMessage(reunion + REUNION_BUNZA_WAS_REFUSED);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 1);
    Engine_EventSetMessage(reunion + REUNION_MIA_ASKS_WHY_BUNZA_STILL_CAME);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0);
    Engine_ActorFaceActor(ACTOR_BUNZA, ACTOR_MIA, 0);
    Engine_ActorShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 0, 60);
    Engine_EventSetMessage(reunion + REUNION_BUNZA_MENTIONS_COMMOTION);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    Engine_ActorFaceEachOther(ACTOR_IVAN, ACTOR_HAMMET, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Engine_EventWait(30);
    Engine_EventSetMessage(reunion + REUNION_IVAN_EXPLAINS_COMMOTION);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
    Engine_ActorFaceActor(ACTOR_HAMMET, ACTOR_BUNZA, 0);
    Engine_ActorFaceActor(ACTOR_IVAN, ACTOR_BUNZA, 0);
    Engine_EventWait(20);
    Engine_EventSetMessage(reunion + REUNION_BUNZA_LINKS_COMMOTION);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    Engine_ActorSetAttachedEffect(ACTOR_BUNZA, EMOTE_IN_FRONT | 2);
    Engine_EventSetMessage(reunion + REUNION_BUNZA_HAD_TO_KNOW);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    Engine_ActorFaceActor(ACTOR_GERALD, ACTOR_BUNZA, 0);
    Engine_EventSetMessage(reunion + REUNION_GERALD_ASKS_ABOUT_CAVE);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GERALD, 0);
    Engine_ActorShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 5, 70);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
    Engine_EventSetMessage(reunion + REUNION_MIA_ON_GATE);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_BUNZA, 1);
    Engine_EventSetMessage(reunion + REUNION_BUNZA_HID);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_BUNZA, 1);
    Engine_EventSetMessage(reunion + REUNION_HAMMET_STARTLED_BUNZA);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
    Engine_EventWait(30);
    Engine_ActorFaceActor(ACTOR_BUNZA, ACTOR_HAMMET, 0);
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
    Engine_EventSetMessage(reunion + REUNION_BUNZA_IS_GLAD);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
    Engine_EventWait(30);

    /* Bunza goes to look out of the cave, then hurries back. */
    Engine_ActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH, 0);
    Engine_ActorShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 0, 60);
    Engine_ActorSetSpeed(ACTOR_BUNZA, 0x18000, 0xc000);
    Engine_ActorWalkTo(ACTOR_BUNZA, 144, 528);
    Engine_ActorWaitForMove(ACTOR_BUNZA);
    Engine_ActorWalkTo(ACTOR_BUNZA, 168, 560);
    Engine_ActorWaitForMove(ACTOR_BUNZA);
    Engine_EventWait(60);
    Engine_ActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
    Engine_EventWait(40);
    Engine_ActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH + FACING_STEP, 0);
    Engine_EventWait(40);
    Engine_ActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
    Engine_EventWait(40);
    Engine_ActorWalkTo(ACTOR_BUNZA, 144, 528);
    Engine_ActorWaitForMove(ACTOR_BUNZA);
    Engine_ActorWalkTo(ACTOR_BUNZA, 168, 488);
    Engine_EventSetMessage(reunion + REUNION_IVAN_ASKS_WHY);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
    Engine_ActorWaitForMove(ACTOR_BUNZA);
    Engine_ActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
    Engine_ActorSetAnimation(ACTOR_BUNZA, ANIM_STAND);
    Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    Engine_EventSetMessage(reunion + REUNION_BUNZA_WARNS_OF_SEARCH);
    Engine_EventShowMessage(ACTOR_BUNZA, 0);
    Engine_EventWait(20);
    Engine_ActorFaceEachOther(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(60);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
    Engine_ActorShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 2, 60);
    Engine_EventSetMessage(reunion + REUNION_BUNZA_FEARS_CAPTURE);
    Engine_EventShowMessage(ACTOR_BUNZA, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 1);
    Engine_EventSetMessage(reunion + REUNION_MIA_URGES_ESCAPE);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0);
    Engine_ActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | 2, 60);
    Engine_EventSetMessage(reunion + REUNION_IVAN_WANTS_STEALTH);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
    Engine_ActorFaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Engine_ActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | 2, 40);
    Engine_EventSetMessage(reunion + REUNION_GERALD_ASKS_TO_FIGHT);
    Engine_EventOpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GERALD, 0);
    if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
        Engine_EventSetMessage(reunion + REUNION_BUNZA_DISCOURAGES_FIGHT);
        Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    } else {
        Engine_EventSetMessage(reunion + REUNION_BUNZA_AGREES_NOT_TO_FIGHT);
        Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    }

    Engine_ActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_BUNZA, 0);
    Engine_ActorFaceActor(ACTOR_GERALD, ACTOR_BUNZA, 0);
    Engine_ActorFaceActor(ACTOR_MIA, ACTOR_BUNZA, 0);
    Engine_ActorFaceActor(ACTOR_IVAN, ACTOR_BUNZA, 0);
    Engine_ActorFaceActor(ACTOR_HAMMET, ACTOR_BUNZA, 0);
    Engine_ActorShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | 1, 60);
    plan = (s32)MsgRunpaBunza;
    Engine_EventSetMessage(plan + PLAN_HAMMET_ASKS_PLAN);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
    Engine_EventWait(20);
    Engine_ActorShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 8, 60);
    Engine_EventSetMessage(plan + PLAN_BUNZA_HAS_WAGON);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    Engine_ActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | 1, 60);
    Engine_EventSetMessage(plan + PLAN_MIA_ASKS_ABOUT_WAGON);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0);
    Engine_ActorFaceActor(ACTOR_BUNZA, ACTOR_MIA, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
    Engine_ActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
    Engine_EventSetMessage(plan + PLAN_BUNZA_OFFERS_RIDE);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    Engine_ActorFaceActor(ACTOR_IVAN, ACTOR_BUNZA, 0);
    Engine_EventSetMessage(plan + PLAN_IVAN_DOUBTS_WAGON);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
    Engine_EventSetMessage(plan + PLAN_BUNZA_REASSURES);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    Engine_EventWait(60);
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_BUNZA, 0);
    Engine_ActorFaceActor(ACTOR_GERALD, ACTOR_BUNZA, 0);
    Engine_EventSetMessage(plan + PLAN_GERALD_AGREES);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GERALD, 0);
    Engine_ActorFaceActor(ACTOR_BUNZA, ACTOR_GERALD, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
    Engine_EventWait(20);
    Engine_EventSetMessage(plan + PLAN_BUNZA_IS_UNSUSPECTED);
    Engine_EventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    Engine_ActorFaceEachOther(ACTOR_HAMMET, ACTOR_IVAN, 0);
    Engine_EventWait(60);
    Engine_ActorSetAnimation(ACTOR_HAMMET, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
    Engine_EventWait(60);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_GERALD, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
    Engine_ActorFaceActor(ACTOR_BUNZA, ACTOR_PARTY_LEADER, 0);
    Engine_ActorSetAnimation(ACTOR_BUNZA, ANIM_NOD);
    Engine_EventWait(60);
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

    wagon = (s32)MsgRunpaLetsTakeWagon;
    Engine_EventSetMessage(wagon + WAGON_BUNZA_LEADS_THE_WAY);
    Engine_EventShowMessage(ACTOR_BUNZA, 0);
    Engine_ActorFaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Engine_EventSetMessage(wagon + WAGON_GERALD_ASKS_TO_RIDE);
    Engine_EventOpenMessage(ACTOR_GERALD, 0);
    Engine_ActorFaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
    Engine_ActorFaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
    Engine_ActorFaceActor(ACTOR_HAMMET, ACTOR_PARTY_LEADER, 0);
    Engine_ActorFaceActor(ACTOR_BUNZA, ACTOR_PARTY_LEADER, 0);

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
            if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
                goto ride;
            }
        }
/* Bunza will not wait for the party any longer. */
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
            confusion = (s32)MsgRunpaTotallyConfusedChanged;
            Engine_EventSetMessage(confusion + CONFUSION_IVAN_IS_CONFUSED);
            Engine_EventShowMessage(ACTOR_IVAN, 0);
            Engine_EventSetMessage(confusion + CONFUSION_GERALD_ASKS_AGAIN);
            Engine_EventOpenMessage(ACTOR_GERALD, 0);
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
    return Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

u8 Gerald_AsksIfNotRiding(void)
{
    Engine_EventSetMessage((s32)MsgRunpaGeraldAsksIfNotRiding);
    Engine_EventOpenMessage(ACTOR_GERALD, 0);
    return Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

u8 Gerald_AsksAboutUnfinishedBusiness(void)
{
    Engine_EventSetMessage((s32)MsgRunpaGeraldAsksAboutUnfinishedBusiness);
    Engine_EventOpenMessage(ACTOR_GERALD, 0);
    return Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

u8 Party_ConfirmsStaying(void)
{
    return TRUE;
}

u8 Bunza_AsksAboutUnfinishedBusiness(void)
{
    Engine_EventSetMessage((s32)MsgRunpaBunzaAsksAboutUnfinishedBusiness);
    Engine_EventOpenMessage(ACTOR_BUNZA, 0);
    return Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

u8 Gerald_ChecksNothingLeftToDo(void)
{
    Engine_EventSetMessage((s32)MsgRunpaGeraldChecksNothingLeft);
    Engine_EventOpenMessage(ACTOR_GERALD, 0);
    return TRUE;
}

u8 Mia_AsksIfRidingAfterAll(void)
{
    Engine_EventSetMessage((s32)MsgRunpaMiaAsksIfRidingAfter);
    Engine_EventOpenMessage(ACTOR_MIA, 0);
    return Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

/* Bunza cannot wait any longer, and Mia asks whether the party stays. */
u8 Bunza_CannotWait(void)
{
    s32 warning;

    Engine_ActorFaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
    Engine_ActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | 2, 60);
    warning = (s32)MsgRunpaMeaningWontRide;
    Engine_EventSetMessage(warning + WARNING_IVAN_ASKS_IF_STAYING);
    Engine_EventShowMessage(ACTOR_IVAN, 0);
    Engine_ActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
    Engine_EventSetMessage(warning + WARNING_BUNZA_CANNOT_WAIT);
    Engine_EventShowMessage(ACTOR_BUNZA, 0);
    Engine_ActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | 2, 60);
    Engine_EventSetMessage(warning + WARNING_MIA_ASKS_ABOUT_BUSINESS);
    Engine_EventOpenMessage(ACTOR_MIA, 0);
    return Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

u8 Gerald_AsksAboutThingsToDo(void)
{
    Engine_EventSetMessage((s32)MsgRunpaGeraldAsksAboutThingsTo);
    Engine_EventOpenMessage(ACTOR_GERALD, 0);
    return Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

/* The party either stays behind in the cave or rides the wagon out. */

/*
 * Hammet and Bunza say goodbye and leave for the wagon; the party watches
 * them go, then falls in behind the leader.
 */
void Party_StaysBehind(void)
{
    struct FieldActor *leader;
    s32 farewell;

    Engine_ActorShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 5, 60);
    Engine_ActorFaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    farewell = (s32)MsgRunpaInsistStick;
    Engine_EventSetMessage(farewell + FAREWELL_GERALD_STAYS);
    Engine_EventShowMessage(ACTOR_GERALD, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 1);
    Engine_EventSetMessage(farewell + FAREWELL_MIA_STAYS);
    Engine_EventShowMessage(ACTOR_MIA, 0);
    Engine_ActorFaceActor(ACTOR_IVAN, ACTOR_HAMMET, 0);
    Engine_EventWait(60);
    Engine_EventSetMessage(farewell + FAREWELL_IVAN_STAYS);
    Engine_EventShowMessage(ACTOR_IVAN, 0);
    Engine_ActorFaceActor(ACTOR_HAMMET, ACTOR_IVAN, 0);
    Engine_ActorShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | 5, 70);
    Engine_EventSetMessage(farewell + FAREWELL_HAMMET_LETS_IVAN_GO);
    Engine_EventShowMessage(ACTOR_HAMMET, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
    Engine_EventSetMessage(farewell + FAREWELL_BUNZA_SAYS_GOODBYE);
    Engine_EventShowMessage(ACTOR_BUNZA, 0);
    Engine_ActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_GERALD, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
    Engine_EventWait(60);
    Engine_ActorSetDestinationOffset(ACTOR_HAMMET, -16, 0);
    Engine_ActorWaitForMove(ACTOR_HAMMET);
    Engine_ActorSetAnimation(ACTOR_HAMMET, ANIM_STAND);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
    Engine_ActorFaceDirection(ACTOR_HAMMET, FACING_SOUTH + FACING_STEP, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_GERALD, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
    Engine_ActorWalkTo(ACTOR_BUNZA, 156, 528);
    Engine_EventWait(20);
    Engine_ActorWalkTo(ACTOR_HAMMET, 164, 528);
    Engine_ActorWaitForMove(ACTOR_BUNZA);
    Engine_ActorWalkTo(ACTOR_BUNZA, 168, 640);
    Engine_ActorWaitForMove(ACTOR_HAMMET);
    Engine_ActorWalkTo(ACTOR_HAMMET, 168, 640);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
    Engine_ActorFaceDirection(ACTOR_IVAN, FACING_SOUTH + FACING_STEP, 0);
    Engine_ActorFaceDirection(ACTOR_MIA, FACING_SOUTH + FACING_STEP, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, FACING_SOUTH + FACING_STEP, 0);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Engine_ActorFaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
    Engine_ActorFaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, FACING_SOUTH, 0);
    Engine_EventWait(60);
    Engine_ActorSetPosition(ACTOR_HAMMET, 0, 0);
    Engine_ActorSetPosition(ACTOR_BUNZA, 0, 0);
    Engine_EventWait(110);

    Engine_EventSetMessage(farewell + FAREWELL_GERALD_SIGHS);
    Engine_EventShowMessage(ACTOR_GERALD, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    Engine_EventWait(30);
    Engine_EventSetMessage(farewell + FAREWELL_MIA_HOPES_FOR_SAFETY);
    Engine_EventShowMessage(ACTOR_MIA, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Engine_EventSetMessage(farewell + FAREWELL_IVAN_REASSURES);
    Engine_EventShowMessage(ACTOR_IVAN, 0);
    Engine_EventWait(140);
    Engine_ActorFaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Engine_EventSetMessage(farewell + FAREWELL_GERALD_MOVES_ON);
    Engine_EventShowMessage(ACTOR_GERALD, 0);

    Engine_ActorSetAnimation(ACTOR_GERALD, ANIM_WALK);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
        Engine_ActorSetDestination(ACTOR_GERALD, leader->x.part.pixel, leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Engine_ActorSetPosition(ACTOR_GERALD, 0, 0);
    Engine_ActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
        Engine_ActorSetDestination(ACTOR_IVAN, leader->x.part.pixel, leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(ACTOR_IVAN);
    Engine_ActorSetPosition(ACTOR_IVAN, 0, 0);
    Engine_ActorSetAnimation(ACTOR_MIA, ANIM_WALK);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
        Engine_ActorSetDestination(ACTOR_MIA, leader->x.part.pixel, leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(ACTOR_MIA);
    Engine_ActorSetPosition(ACTOR_MIA, 0, 0);
    Engine_EventWait(30);
    Engine_CameraMoveToActor(ACTOR_PARTY_LEADER, 1);
    Engine_CameraWaitForMove();
    Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
    Engine_GameFlagSet(FLAG_PARTY_STAYED_IN_LUNPA);
}

/*
 * Everyone heads for the wagon: Hammet and Bunza lead, the party falls in
 * behind the leader, and the leader follows them out of the cave.
 */
void Party_RidesWagon(void)
{
    struct FieldActor *leader;
    s32 departure;

    Engine_ActorFaceDirection(ACTOR_GERALD, FACING_SOUTH + FACING_STEP, 0);
    departure = (s32)MsgRunpaDontUnfinishedBusiness;
    Engine_EventSetMessage(departure + DEPARTURE_GERALD_HEADS_FOR_KALAY);
    Engine_EventShowMessage(ACTOR_GERALD, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Engine_EventSetMessage(departure + DEPARTURE_IVAN_THINKS_OF_LAYANA);
    Engine_EventShowMessage(ACTOR_IVAN, 0);
    Engine_ActorFaceActor(ACTOR_HAMMET, ACTOR_IVAN, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
    Engine_EventWait(20);
    Engine_EventSetMessage(departure + DEPARTURE_HAMMET_LONGS_FOR_LAYANA);
    Engine_EventShowMessage(ACTOR_HAMMET, 0);
    Engine_ActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
    Engine_EventWait(30);
    Engine_EventSetMessage(departure + DEPARTURE_BUNZA_SETS_OFF);
    Engine_EventShowMessage(ACTOR_BUNZA, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_GERALD, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
    Engine_EventWait(80);
    Engine_ActorSetDestinationOffset(ACTOR_HAMMET, -16, 0);
    Engine_ActorWaitForMove(ACTOR_HAMMET);
    Engine_ActorSetAnimation(ACTOR_HAMMET, ANIM_STAND);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
    Engine_ActorFaceDirection(ACTOR_HAMMET, FACING_SOUTH + FACING_STEP, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_GERALD, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
    Engine_ActorWalkTo(ACTOR_BUNZA, 152, 528);
    Engine_EventWait(20);
    Engine_ActorWalkTo(ACTOR_HAMMET, 160, 528);
    Engine_ActorWaitForMove(ACTOR_BUNZA);
    Engine_ActorWalkTo(ACTOR_BUNZA, 168, 640);
    Engine_ActorWaitForMove(ACTOR_HAMMET);
    Engine_ActorWalkTo(ACTOR_HAMMET, 168, 640);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
    Engine_ActorFaceDirection(ACTOR_IVAN, FACING_SOUTH + FACING_STEP, 0);
    Engine_ActorFaceDirection(ACTOR_MIA, FACING_SOUTH + FACING_STEP, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, FACING_SOUTH + FACING_STEP, 0);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Engine_ActorFaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
    Engine_ActorFaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, FACING_SOUTH, 0);
    Engine_EventWait(200);
    Engine_ActorSetPosition(ACTOR_HAMMET, 0, 0);
    Engine_ActorSetPosition(ACTOR_BUNZA, 0, 0);

    Engine_ActorSetAnimation(ACTOR_GERALD, ANIM_WALK);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
        Engine_ActorSetDestination(ACTOR_GERALD, leader->x.part.pixel, leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Engine_ActorSetPosition(ACTOR_GERALD, 0, 0);
    Engine_ActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
        Engine_ActorSetDestination(ACTOR_IVAN, leader->x.part.pixel, leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(ACTOR_IVAN);
    Engine_ActorSetPosition(ACTOR_IVAN, 0, 0);
    Engine_ActorSetAnimation(ACTOR_MIA, ANIM_WALK);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
        Engine_ActorSetDestination(ACTOR_MIA, leader->x.part.pixel, leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(ACTOR_MIA);
    Engine_ActorSetPosition(ACTOR_MIA, 0, 0);
    Engine_EventWait(30);
    Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, -16, 0);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Engine_CameraMoveToActor(ACTOR_PARTY_LEADER, 1);
    Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 168, 640);
    Engine_EventWait(60);
    Engine_EventCloseScreen();
    Engine_EventRequestExit(CAVE_EXIT_BY_WAGON);
}

/* The cave's scene start: open with the window transition, hide the four
   puddles, set the north one's scale, and redraw the gate and the pillars
   its flags record. */
s32 Scene_Initialize(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (gGameState.scene == (s32)&SceneId_RunpaDou) {
        Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_HIDDEN_PUDDLE), 0);
        Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_SOUTH_PUDDLE), 0);
        Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_GATE_PUDDLE), 0);
        Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_NORTH_PUDDLE), 0);
        Object_GetById(ACTOR_NORTH_PUDDLE)->scale_y = 0xf333;
        if (Engine_GameFlagIsSet(FLAG_CAVE_GATE_LOWERED) != 0) {
            Gate_Lower();
        }
        if (Engine_GameFlagIsSet(FLAG_CAVE_GATE_RAISED) != 0) {
            Gate_Raise();
        }
        if (Engine_GameFlagIsSet(FLAG_CAVE_GATE_PROPPED) != 0) {
            Gate_DrawPropped();
        }
        if (Engine_GameFlagIsSet(FLAG_CAVE_NORTH_PILLAR) != 0) {
            Engine_ActorSetAnimation(ACTOR_NORTH_PUDDLE, PUDDLE_ANIM_FROZEN);
        }
        if (Engine_GameFlagIsSet(FLAG_CAVE_SOUTH_PILLAR) != 0) {
            Engine_ActorSetAnimation(ACTOR_SOUTH_PUDDLE, PUDDLE_ANIM_FROZEN);
        }
    }
    return 0;
}
