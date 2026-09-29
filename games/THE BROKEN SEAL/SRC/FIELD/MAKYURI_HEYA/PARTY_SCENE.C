/* Mercury Lighthouse: the girl the party saved asks for the truth until
 * it is told, then joins with her Djinni. */
#define FIELD_STAGED_ACTOR_IMPORTS
#include "MAKYURI_HEYA.H"

extern u8 MsgMakyuriSavedMeAgain[];
extern u8 MsgMakyuriDontHideTruth[];
extern u8 MsgMakyuriThoughtSo[];

void Event_PrepareObjectAndApplyValue();
s32 Djinn_AddToOwner();
void Djinn_Activate();
void Owner_RecalculateStats();

/* FAKEMATCH: a call spelled through this value wrapper sets r0 last of its
 * arguments, as the game does at those sites. */
static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

void MakyuriHeya_RunPartyScene(void)
{
    s32 base;
    s32 position;
    s32 event;
    u8 *actor;

    position = *(s32 *)((u8 *)Actor_Get(8) + 8) / 0x100000;
    if (position != 48) {
        return;
    }
    Engine_EventBegin();
    base = (s32)MsgMakyuriSavedMeAgain;
    Engine_EventSetMessage(base);
    Battle_WaitMode0(20);
    Engine_ActorRunRepeatedMotion(3, 1);
    Actor_FaceDirection(0, 32768, 20);
    Event_ShowMessageAndWait(3, 0, 20);
    Actor_SetAnimationAndWait(3, 3);
    Battle_WaitMode0(20);
    Engine_ActorSetAnimationAndWait(0, 3);
    Battle_WaitMode0(20);
    Battle_WaitMode0(60);
    Actor_SetAnimation(3, 16);
    Battle_WaitMode0(50);
    Actor_SetAnimation(3, 1);
    Engine_EventOpenMessage(3, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
        Battle_WaitMode0(20);
        Actor_RunRepeatedMotion(3, 2);
        Battle_WaitMode0(20);
        Event_ShowMessageAndWait(3, 0, 20);
        Actor_SetAnimationAndWait(3, 4);
        Battle_WaitMode0(20);
        Engine_EventShowMessageAndWait(3, 0, 20);
        Engine_ActorSetAnimationAndWait(3, 3);
        Event_Wait(20);
        Engine_EventOpenMessage(3, 0);
        if (Engine_EventChooseYesNo(0, 0) == 1) {
            Event_Wait(20);
            Value2((s32 (*)())Engine_ActorSetAnimationAndWait, 3, 4);
            Event_Wait(20);
            event = base + 5;
            for (;;) {
                Engine_EventSetMessage(event);
                Engine_EventOpenMessage(3, 0);
                if (Engine_EventChooseYesNo(0, 0) != 1) {
                    break;
                }
                Battle_WaitMode0(20);
                Value2((s32 (*)())Engine_ActorSetAnimationAndWait, 3, 4);
                Battle_WaitMode0(20);
                event = (s32)MsgMakyuriDontHideTruth;
            }
        }
    }
    Engine_EventSetMessage((s32)MsgMakyuriThoughtSo);
    Actor_SetSpeed(3, 52428, 26214);
    Engine_ActorWalkToAndWait(3, 728, 632);
    Battle_WaitMode0(20);
    Engine_EventShowMessageAndWait(3, 0, 20);
    Object_SetModeById(3, 16);
    Engine_EventShowMessageAndWait(3, 0, 20);
    Object_SetModeById(3, 1);
    Engine_ActorFaceActor(3, 0, 20);
    Engine_ActorSetAnimationAndWait(3, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(3, 0, 20);
    Actor_ShowEmote(3, 261, 90);
    Engine_ActorSetAnimationAndWait(3, 3);
    Battle_WaitMode0(20);
    Event_ShowMessageAndWait(3, 0, 20);
    Event_PrepareObjectAndApplyValue(3, 1);
    Engine_GameFlagSet(68);
    Djinn_AddToOwner(3, 1, 0);
    Djinn_Activate(3, 1, 0);
    Owner_RecalculateStats(3);
    Object_SetModeById(3, 2);
    actor = Object_GetById(0);
    if (actor != 0) {
        Engine_ActorSetDestination(3, *(s16 *)(actor + 10), *(s16 *)(actor + 18));
    }
    Actor_WaitForMove(3);
    Engine_ActorSetPosition(3, 0, 0);
    Map_CopyCellAttributes(110, 39, 5, 1, 46, 39);
    Engine_GameFlagSet(2163);
    Engine_EventEnd();
}
