#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "HEYA.H"

#include "STAGED_ACTOR.H"
extern u8 MsgFuneGivesMeChillsThinkCould[];
extern u8 MsgFuneHaHaHaRowingFeel[];
extern u8 MsgFuneHoHoPersonGoingGet[];
extern u8 MsgFuneIfYouveChosenOarsmanLets[];
extern u8 MsgFuneNotThinkingMaking[];
extern u8 MsgFuneOarsmanGiveBreak[];
extern u8 MsgFuneOhhhhNooooGoingMakeMe[];
extern u8 MsgFunePeopleAskingFrail[];
extern u8 MsgFunePoorOarsmanFeel[];
extern u8 MsgFuneRobinYouveGotGoodEye[];
extern u8 MsgFuneThatsWhoYouPickedOarsman[];
extern u8 MsgFuneYouveBeenChosenAsOarsman[];

/* Message ids. */

struct SceneActor {
    u8 pad00[99];
    u8 mode;
};

struct EffectRecord {
    u8 pad00[6];
    u16 angle;
    u8 pad08[83];
    u8 state;
    u8 pad5c[6];
    u8 active;
};
void FuneHeya_TurnActorToOpenSide();
s32 SceneState_ApplyLevelFromFlags();
s32 Object_GetByIdFar();

void FieldScene_RunScene3b1SequenceA(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x300) != 0) {
        rec7 = SceneState_ApplyLevelFromFlags();
        FuneHeya_TurnActorToOpenSide();
        Event_SetMessage((s32)MsgFuneGivesMeChillsThinkCould);
        FieldScene_RunStepThen10(12);
        Actor_SetAnimation(rec7, 2);
        record = Object_GetByIdFar(0);
        if (record != 0) {
            Actor_SetDestination(rec7, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove(rec7);
        Actor_SetPosition(rec7, 0, 0);
    } else {
        Actor_RunRepeatedMotion(12, 2);
        Event_Wait(20);
        Event_SetMessage((s32)MsgFuneOhhhhNooooGoingMakeMe);
        Event_OpenMessage(12, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            FieldScene_RunStepThen10(12);
            Actor_SetAnimation(12, 2);
            record = Object_GetByIdFar(0);
            if (record != 0) {
                Actor_SetDestination(12, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Actor_WaitForMove(12);
            Actor_SetPosition(12, 0, 0);
            GameFlag_Set(0x300);
            if (GameFlag_IsSet(0x92b) != 0) {
                GameFlag_Set(0x994);
                goto L_02000c9a;
            }
            if (GameFlag_IsSet(0x92a) != 0) {
                GameFlag_Set(0x91b);
                goto L_02000c9a;
            }
            if (GameFlag_IsSet(0x929) != 0) {
                GameFlag_Set(0x939);
                goto L_02000c9a;
            }
            GameFlag_Set(0x930);
        } else {
            bump_step(1);
            FieldScene_RunStepThen10(12);
        }
    }
    L_02000c9a:;
    Event_End();
}

void FieldScene_RunScene3b1SequenceB(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x300) != 0) {
        rec7 = SceneState_ApplyLevelFromFlags();
        FuneHeya_TurnActorToOpenSide();
        Event_SetMessage((s32)MsgFuneRobinYouveGotGoodEye);
        FieldScene_RunStepThen10(9);
        Actor_SetAnimation(rec7, 2);
        record = Object_GetByIdFar(0);
        if (record != 0) {
            Actor_SetDestination(rec7, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        ((void (*)())Engine_ActorWaitForMove)(rec7);
        Actor_SetPosition(rec7, 0, 0);
    } else {
        Event_SetMessage((s32)MsgFuneHaHaHaRowingFeel);
        ((void (*)())Engine_EventShowMessageAndWait)(9, 0, 60);
        Actor_RunRepeatedMotion(9, 1);
        Event_OpenMessage(9, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            FieldScene_RunStepThen10(9);
            Actor_SetAnimation(9, 2);
            record = Object_GetByIdFar(0);
            if (record != 0) {
                Actor_SetDestination(9, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Actor_WaitForMove(9);
            Actor_SetPosition(9, 0, 0);
            GameFlag_Set(0x300);
            if (GameFlag_IsSet(0x92b) != 0) {
                GameFlag_Set(0x991);
                goto L_02000de0;
            }
            if (GameFlag_IsSet(0x92a) != 0) {
                GameFlag_Set(0x918);
                goto L_02000de0;
            }
            if (GameFlag_IsSet(0x929) != 0) {
                GameFlag_Set(0x936);
                goto L_02000de0;
            }
            GameFlag_Set(0x92d);
        } else {
            bump_step(1);
            FieldScene_RunStepThen10(9);
        }
    }
    L_02000de0:;
    Event_End();
}

void SceneDialogue_RunActorThirteenFlag300Branch(void)
{
    if (GameFlag_IsSet(0x300) != 0) {
        u8 *o = SceneState_ApplyLevelFromFlags();
        u8 *p;

        Event_Begin();
        FuneHeya_TurnActorToOpenSide(o);
        Event_SetMessage((s32)MsgFuneThatsWhoYouPickedOarsman);
        FieldScene_RunStepThen10(13);
        Actor_SetAnimation(o, 2);
        p = Object_GetByIdFar(0);
        if (p != 0) {
            Actor_SetDestination(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Actor_WaitForMove(o);
        Actor_SetPosition(o, 0, 0);
        Event_End();
    } else if (GameFlag_IsSet(0x92b) != 0) {
        FieldScene_RunPrimarySequence(13, (s32)MsgFuneNotThinkingMaking, 0x995);
    } else if (GameFlag_IsSet(0x92a) != 0) {
        FieldScene_RunPrimarySequence(13, (s32)MsgFuneNotThinkingMaking, 0x91c);
    } else if (GameFlag_IsSet(0x929) != 0) {
        FieldScene_RunPrimarySequence(13, (s32)MsgFuneNotThinkingMaking, 0x93a);
    } else {
        FieldScene_RunPrimarySequence(13, (s32)MsgFuneNotThinkingMaking, 0x931);
    }
}

void FieldScene_RunFlag300BranchDialogue(void)
{
    if (GameFlag_IsSet(0x300) != 0) {
        u8 *o = SceneState_ApplyLevelFromFlags();
        u8 *p;

        Event_Begin();
        FuneHeya_TurnActorToOpenSide(o);
        Event_SetMessage((s32)MsgFuneIfYouveChosenOarsmanLets);
        FieldScene_RunStepThen10(14);
        Actor_SetAnimation(o, 2);
        p = Object_GetByIdFar(0);
        if (p != 0) {
            Actor_SetDestination(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Actor_WaitForMove(o);
        Actor_SetPosition(o, 0, 0);
        Event_End();
    } else if (GameFlag_IsSet(0x92b) != 0) {
        FieldScene_RunPrimarySequence(14, (s32)MsgFuneOarsmanGiveBreak, 0x996);
    } else if (GameFlag_IsSet(0x92a) != 0) {
        FieldScene_RunPrimarySequence(14, (s32)MsgFuneOarsmanGiveBreak, 0x91d);
    } else if (GameFlag_IsSet(0x929) != 0) {
        FieldScene_RunPrimarySequence(14, (s32)MsgFuneOarsmanGiveBreak, 0x93b);
    } else {
        FieldScene_RunPrimarySequence(14, (s32)MsgFuneOarsmanGiveBreak, 0x932);
    }
}

void FieldScene_RunActor15FlagDialogue(void)
{
    if (GameFlag_IsSet(0x300) != 0) {
        u8 *o = SceneState_ApplyLevelFromFlags();
        u8 *p;

        Event_Begin();
        FuneHeya_TurnActorToOpenSide(o);
        Event_SetMessage((s32)MsgFuneYouveBeenChosenAsOarsman);
        FieldScene_RunStepThen10(15);
        Actor_SetAnimation(o, 2);
        p = Object_GetByIdFar(0);
        if (p != 0) {
            Actor_SetDestination(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Actor_WaitForMove(o);
        Actor_SetPosition(o, 0, 0);
        Event_End();
    } else if (GameFlag_IsSet(0x92b) != 0) {
        FieldScene_RunPrimarySequence(15, (s32)MsgFunePoorOarsmanFeel, 0x997);
    } else if (GameFlag_IsSet(0x92a) != 0) {
        FieldScene_RunPrimarySequence(15, (s32)MsgFunePoorOarsmanFeel, 0x91e);
    } else if (GameFlag_IsSet(0x929) != 0) {
        FieldScene_RunPrimarySequence(15, (s32)MsgFunePoorOarsmanFeel, 0x93c);
    } else {
        FieldScene_RunPrimarySequence(15, (s32)MsgFunePoorOarsmanFeel, 0x933);
    }
}

void FieldScene_RunActor16FlagDialogue(void)
{
    s32 obj;
    u8 *actor;

    if (GameFlag_IsSet(0x300) != 0) {
        obj = SceneState_ApplyLevelFromFlags();
        Event_Begin();
        FuneHeya_TurnActorToOpenSide(obj);
        Event_SetMessage((s32)MsgFuneHoHoPersonGoingGet);
        FieldScene_RunStepThen10(16);
        Actor_SetAnimation(obj, 2);

        actor = Object_GetByIdFar(0);
        if (actor != 0) {
            Actor_SetDestination(obj, *(s16 *)(actor + 10),
                          *(s16 *)(actor + 18));
        }

        Actor_WaitForMove(obj);
        Actor_SetPosition(obj, 0, 0);
        Event_End();
    } else {
        if (GameFlag_IsSet(0x92b) != 0) {
            FieldScene_RunPrimarySequence(16, (s32)MsgFunePeopleAskingFrail, 0x998);
        } else if (GameFlag_IsSet(0x92a) != 0) {
            FieldScene_RunPrimarySequence(16, (s32)MsgFunePeopleAskingFrail, 0x91f);
        } else if (GameFlag_IsSet(0x929) != 0) {
            FieldScene_RunPrimarySequence(16, (s32)MsgFunePeopleAskingFrail, 0x93d);
        } else {
            FieldScene_RunPrimarySequence(16, (s32)MsgFunePeopleAskingFrail, 0x934);
        }
    }
}

struct FieldActor *FindActorNearPosition(s32 x, s32 y)
{
    struct EventWork *work;
    struct FieldActor **actor;
    struct FieldActor *current;
    u32 i;
    s32 actor_x;
    s32 actor_y;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;

    work = gEventWork;
    i = 8;
    left = x - 12;
    right = x + 12;
    top = y - 12;
    bottom = y + 12;
    actor = work->placed_actors;
    while (i <= 65) {
        current = *actor++;
        actor_x = current->x.part.pixel;
        actor_y = current->z.part.pixel;
        if (left < actor_x && right > actor_x &&
            top < actor_y && bottom > actor_y)
            return current;
        i++;
    }
    return 0;
}
