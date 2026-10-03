/* Scene tables, the primary sequence and the facing actors. */
#include "TOREBI.H"
#include "CALL.H"

extern u8 MsgTorebiHeWontSailShipEven[];
extern u8 MsgTorebiHeyWhatsThis[];

extern u8 MsgTorebiColossoFinalsFinally[];
extern u8 MsgTorebiComeWayKalay[];
extern u8 MsgTorebiFirstTimeTolbi[];
extern u8 MsgTorebiLookStrongGo[];
extern u8 MsgTorebiRequireLotHealing[];
extern u8 MsgTorebiRightOneWell[];

extern u8 MsgTorebiIfCanMakeNameFor[];
extern u8 MsgTorebiMaybeCloseShop[];
extern u8 MsgTorebiWasntAbleWatch[];

extern u8 MsgTorebiLookLikeWarrior[];

extern u8 MsgTorebiGrrrChefInBadMood[];

extern u8 MsgTorebiHeeHeeLook[];
extern u8 MsgTorebiHello[];
extern u8 MsgTorebiHoHumFine[];
extern u8 MsgTorebiWantStay[];

extern u8 MsgTorebiGrrrScamWhyWontThey[];
extern u8 MsgTorebiHehHehSheJustHid[];
extern u8 MsgTorebiThingFoundDefinitelySameAs[];

extern u8 MsgTorebiWarriorRemember[];

extern u8 MsgTorebiMissFinalsTolbis[];
extern u8 MsgTorebiShipsArentGoing[];
extern u8 MsgTorebiWasteStuckHereWhenSuch[];
extern u8 MsgTorebiHeyaBabiShowedHimselfAtFinals[];

u8 *SceneData_GetTable8BB4(void)
{
    return Data_02008bb4;
}

s32 Func_02000038(void)
{
    return 0;
}

u8 *SceneData_GetTable8dac(void)
{
    return Data_02008dac;
}

s32 SceneData_SelectTable8e00ByFlag(void)
{
    if (Engine_GameFlagIsSet(0x950) != 0) {
        return (s32)Data_02009040;
    }
    return (s32)Data_02008e00;
}

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
void SceneDialogue_AskBabiWasMissing(s32 subject)
{
    s32 msg;

    msg = (s32)MsgTorebiHeyaBabiShowedHimselfAtFinals;
    Engine_EventSetMessage(msg);
    Engine_EventOpenMessage(subject, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(10);
        Engine_EventSetMessage(msg + 1);
    } else {
        Engine_EventSetMessage(msg + 2);
    }
    Engine_EventShowMessage(subject, 0);
}
#endif

u8 *SceneData_SelectTable9310ByFlags(void)
{
    if (Engine_GameFlagIsSet(0x950) != 0) {
        return Data_020099d0;
    }
    if (Engine_GameFlagIsSet(0x962) != 0) {
        return Data_02009670;
    }
    return Data_02009310;
}

void SceneState_SetWork1c0AndRun(void)
{
    extern u8 *Data_03001ebc;

    u8 *state = Data_03001ebc;

    *(s32 *)(state + 0x1C0) = 0x201;
    *(s32 *)(state + 0x1C8) = 24;
    ((void (*)(void))Engine_EventRequestExit)();
}

void FieldScene_RunPrimarySequence(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    u8 *p6;
    s32 n;

    p6 = *(u8 **)Data_03001ebc;
    for (i = 8; i < 66; i++) {
        record = Object_GetById(i);
        if (record != 0) {
            *(u8 *)(record + 85) = 0;
        }
    }
    p6 = p6 + 0x16c;
    n = *(s16 *)p6 - 14;
    Engine_AudioPlayCue(158);
    Call3(Engine_MapAnimateCells, Data_02009dcc[n].a, Data_02009dcc[n].b, Data_02009dcc[n].c);
    Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    *(u8 *)((s32)Object_GetById(0) + 85) = 0;
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
    Engine_EventRequestExit(*(s16 *)p6);
}

void FieldScene_RunScene3b6SequenceA(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgTorebiHeyWhatsThis);
    Engine_EventWait(40);
    rec7 = Engine_ObjectCreate(0x11c, 0x2580000, 0, 0x3380000);
    Engine_ActorSetSpriteFlags(rec7, 0);
    Object_SetMode(rec7, 6);
    Engine_EventWait(10);
    Object_SetMode(rec7, 1);
    Engine_EventWait(40);
    Engine_ObjectDispatchRelease(rec7);
    Engine_EventWait(2);
    Actor_ShowEmote(25, 0x100, 50);
    Actor_SetSpeed(25, 0x10000, 0x8000);
    Actor_WalkToAndWait(25, 0x258, 0x350);
    Actor_FaceDirection(25, 0xc000, 0);
    Engine_EventWait(40);
    Event_ShowMessage(25, 0);
    Engine_ActorRunRepeatedMotion(25, 2);
    Engine_EventWait(30);
    Actor_WalkToAndWait(25, 0x238, 0x350);
    Actor_FaceDirection(25, 0xc000, 0);
    Engine_EventWait(30);
    Actor_ShowEmote(25, 0x108, 50);
    Engine_EventWait(20);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
    Engine_EventWait(20);
    Actor_FaceDirection(25, 0x3000, 0);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(25, 2);
    Engine_EventWait(20);
    Event_ShowMessage(25, 0);
    Engine_EventWait(20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 50);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(25, 4);
    Engine_EventWait(20);
    Event_ShowMessage(25, 0);
    Engine_EventWait(30);
    Actor_ShowEmote(25, 0x102, 50);
    Event_ShowMessage(25, 0);
    Actor_SetSpeed(25, 0x16666, 0xb333);
    Actor_WalkByAndWait(25, 16, 0);
    Actor_WalkByAndWait(25, 0, 32);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(25, 3);
    Engine_EventWait(20);
    Event_ShowMessage(25, 0);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 16, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Engine_EventWait(20);
    Actor_SetSpeed(25, 0x1cccc, 0xe666);
    Actor_WalkByAndWait(25, 0, 48);
    Actor_SetPosition(25, 0, 0);
    Engine_EventEnd();
}

void FieldScene_RunActorsThirtyOneToThirtyThreeChoreography(void)
{
    void Engine_EventWait(s32);
    void Engine_EventSetMessage(s32);

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgTorebiHeWontSailShipEven);
    Engine_EventWait(30);
    /* Same import, same first two arguments, differing only in the third.
     * Two call sites, not a loop. */
    Engine_ActorJump(31, 4, 13);
    Engine_ActorJump(31, 4, 30);
    Engine_EventShowMessage(31, 0);
    Engine_EventWait(10);
    /* r1 = 129 << 1 = 0x102. Argument registers are set r1, r2, r0. */
    Engine_ActorShowEmote(32, 0x102, 50);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(32, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(32, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(33, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(33, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(31, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(31, 0);
    Engine_EventWait(10);
    /* Repeats the (32, 3) call made above; a second site, deliberately not
     * folded with the first. */
    Engine_ActorSetAnimationAndWait(32, 3);
    Engine_EventWait(30);
    Engine_EventEnd();
}

/* Keep the first byte store and zero initialization as one assignment. */
s32 Scene_InitFacingActors(void)
{
    u8 *record;
    s32 none;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    if (Engine_GameFlagIsSet(0x950) != 0) {
        Call6(Engine_MapCopyCellAttributes, 51, 47, 3, 1, 51, 45);
        record = Object_GetById(31);
        record[35] = none = 0;
        (*(s8 **)(record + 80))[9] = ((-13 & (*(s8 **)(record + 80))[9]) | 8);
        record = Object_GetById(32);
        record[35] = none;
        (*(s8 **)(record + 80))[9] = ((-13 & (*(s8 **)(record + 80))[9]) | 8);
        if (Value1(Engine_GameFlagIsSet, 0x8bc) != 0) {
            Call3(Engine_ActorSetPosition, 25, 0x2300000, 0x2a80000);
            Call3(Engine_ActorFaceDirection, 25, 0x8000, 0);
        }
        if (gGameState.entrance == 19) {
            if (Value1(Engine_GameFlagIsSet, 0x8bc) == 0) {
                Engine_GameFlagSet(0x8bc);
                Engine_EventOpenScreen();
                FieldScene_RunScene3b6SequenceA();
            }
        }
        if (gGameState.entrance == 16) {
            if (Value1(Engine_GameFlagIsSet, 0x300) == 0) {
                Engine_GameFlagSet(0x300);
                Engine_EventOpenScreen();
                FieldScene_RunActorsThirtyOneToThirtyThreeChoreography();
            }
        }
        if (Engine_GameFlagIsSet(0x8ab) != 0) {
            Engine_ActorSetPosition(35, 0, 0);
            Engine_ActorSetPosition(36, 0, 0);
        }
    }
    return 0;
}

/* The facing prompts: a shop when the leader faces the counter, otherwise a
 * line chosen by the story flags. The yes-or-no lines load their first
 * message once and add to it for the two answers. */
void SceneDialogue_RunFacingPrompt(s32 no)
{
    u8 *actor = Object_GetById(0);
    s32 msg;
    if ((u16)((*(u16 *)(actor + 6) + 0x2000) & ~0x3fff) == 0x8000) {
        Engine_ShopOpen(28, no);
    } else if (Engine_GameFlagIsSet(0x950) != 0) {
        Engine_EventSetMessage((s32)MsgTorebiRightOneWell);
        Engine_EventShowMessage(no, 0);
    } else if (Engine_GameFlagIsSet(0x962) != 0) {
        Engine_EventSetMessage((s32)MsgTorebiRequireLotHealing);
        Engine_EventShowMessage(no, 0);
    } else {
        msg = (s32)MsgTorebiFirstTimeTolbi;
        Engine_EventSetMessage(msg);
        Engine_EventOpenMessage(no, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            Engine_EventWait(10);
            Engine_EventSetMessage(msg + 1);
        } else {
            Engine_EventSetMessage(msg + 2);
        }
        Engine_EventShowMessage(no, 0);
    }
}

void SceneDialogue_RunFacingActionPrompt(s32 no)
{
    u8 *actor = Object_GetById(0);
    s32 msg;
    if ((u16)((*(u16 *)(actor + 6) + 0x2000) & ~0x3fff) == 0xc000) {
        Engine_ShopOpen(26, no);
    } else if (Engine_GameFlagIsSet(0x950) != 0) {
        msg = (s32)MsgTorebiComeWayKalay;
        Engine_EventSetMessage(msg);
        Engine_EventOpenMessage(no, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            Engine_EventWait(10);
            Engine_EventSetMessage(msg + 1);
        } else {
            Engine_EventSetMessage(msg + 2);
        }
        Engine_EventShowMessage(no, 0);
    } else if (Engine_GameFlagIsSet(0x962) != 0) {
        Engine_EventSetMessage((s32)MsgTorebiColossoFinalsFinally);
        Engine_EventShowMessage(no, 0);
    } else {
        Engine_EventSetMessage((s32)MsgTorebiLookStrongGo);
        Engine_EventShowMessage(no, 0);
        Engine_ActorShowEmote(no, 0x106, 0);
        Engine_EventWait(40);
        Engine_EventShowMessage(no, 0);
    }
}

/* The facing action line. */
void SceneDialogue_RunFacingAction(s32 no)
{
    u8 *actor = Object_GetById(0);
    if ((u16)((*(u16 *)(actor + 6) + 0x2000) & ~0x3fff) == 0xc000) {
        Engine_ShopOpen(27, no);
    } else {
        if (Engine_GameFlagIsSet(0x950) != 0) {
            Engine_EventSetMessage((s32)MsgTorebiWasntAbleWatch);
            Engine_EventShowMessage(no, 0);
        } else if (Engine_GameFlagIsSet(0x962) != 0) {
            Engine_EventSetMessage((s32)MsgTorebiMaybeCloseShop);
            Engine_EventShowMessage(no, 0);
        } else {
            Engine_EventSetMessage((s32)MsgTorebiIfCanMakeNameFor);
            Engine_EventShowMessage(no, 0);
        }
    }
}

/* Asks whether the party was watching Colosso. */
void SceneDialogue_AskWatchingColosso(s32 subject)
{
    s32 message;

    Engine_EventBegin();

    message = (s32)MsgTorebiLookLikeWarrior;
    Engine_EventSetMessage(message);
    Engine_EventOpenMessage(subject, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(10);
        Engine_EventSetMessage(message + 1);
    } else {
        Engine_EventSetMessage(message + 2);
    }

    Engine_EventShowMessage(subject, 0);
    Engine_EventEnd();
}

/* The chef's line. */
void SceneDialogue_RunActorLine23a1(s32 no)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgTorebiGrrrChefInBadMood);
    Engine_EventShowMessage(no, 0);
    Engine_EventEnd();
}

/* The offer to stay, and the excited girl's lines. */
void SceneDialogue_AskStay(s32 subject)
{
    s32 msg;

    Engine_EventBegin();

    msg = (s32)MsgTorebiWantStay;
    Engine_EventSetMessage(msg);
    Engine_EventOpenMessage(subject, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(10);
        Engine_EventSetMessage(msg + 1);
    } else {
        Engine_EventSetMessage(msg + 2);
    }

    Engine_EventShowMessage(subject, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunExcitedLines(s32 a0)
{
    s32 msg;

    Engine_EventBegin();
    if (GameFlag_IsSet(0x8bd) == 0) {
        msg = (s32)MsgTorebiHeeHeeLook;
        Engine_EventSetMessage(msg);
        Event_OpenMessage(a0, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            Engine_EventWait(10);
            Engine_EventSetMessage(msg + 1);
        } else {
            Engine_EventSetMessage(msg + 2);
        }
        Event_ShowMessage(a0, 0);
    } else {
        if (GameFlag_IsSet(0x8be) == 0) {
            GameFlag_Set(0x8be);
            Engine_EventSetMessage((s32)MsgTorebiHello);
            Event_ShowMessage(a0, 0);
            Engine_EventWait(10);
            Engine_ActorRunRepeatedMotion(a0, 2);
            Engine_EventWait(20);
        }
        Engine_EventSetMessage((s32)MsgTorebiHoHumFine);
        Event_ShowMessage(a0, 0);
    }
    Engine_EventEnd();
}

/* Flagged lines. */
void SceneDialogue_RunActor25FlaggedLine(void)
{
    void Engine_EventBegin(void);

    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x8BE) == 0) {
        Engine_EventSetMessage((s32)MsgTorebiHehHehSheJustHid);
    } else {
        Engine_EventSetMessage((s32)MsgTorebiThingFoundDefinitelySameAs);
    }
    Engine_EventShowMessage(25, 0);
    Engine_EventEnd();
}

void FieldScene_RunScene3b6_02000898(s32 a0)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgTorebiGrrrScamWhyWontThey);
    Actor_ShowEmote(31, 0x103, 40);
    Event_ShowMessage(a0, 0);
    Engine_EventEnd();
}

/* Asks whether the would-be warrior remembers the speaker. */
void SceneDialogue_AskRememberWarrior(s32 subject)
{
    s32 message;

    Engine_EventBegin();

    message = (s32)MsgTorebiWarriorRemember;
    Engine_EventSetMessage(message);
    Engine_EventOpenMessage(subject, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(10);
        Engine_EventSetMessage(message + 1);
    } else {
        Engine_EventSetMessage(message + 2);
    }

    Engine_EventShowMessage(subject, 0);
    Engine_EventEnd();
}

/* The facing message. */
void SceneDialogue_RunFacingMessage(s32 no)
{
    s32 GameFlag_IsSet(s32 flag);

    u8 *actor = Object_GetById(0);
    if ((u16)((*(u16 *)(actor + 6) + 0x2000) & ~0x3fff) == 0xc000) {
        Engine_SanctumOpen(no);
    } else {
        if (Engine_GameFlagIsSet(0x950) != 0) {
            Engine_EventSetMessage((s32)MsgTorebiShipsArentGoing);
            Engine_EventShowMessage(no, 0);
        } else if (Engine_GameFlagIsSet(0x962) != 0) {
            Engine_EventSetMessage((s32)MsgTorebiMissFinalsTolbis);
            Engine_EventShowMessage(no, 0);
        } else {
            Engine_EventSetMessage((s32)MsgTorebiWasteStuckHereWhenSuch);
            Engine_EventShowMessage(no, 0);
        }
    }
}
