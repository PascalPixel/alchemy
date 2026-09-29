#include "SUKURETA.H"

extern u8 Sukureta_StrangerActions[];
void Object_SetTargetAndCallback();
void Audio_PlayCueFromEventWork(void);
extern u8 MsgHaidiaSaturosGo[];
extern u8 MsgHaidiaTheyKnowLittleOfThe[];
extern u8 MsgHaidiaYoureTheOnesSneakingAround[];

void Scene_OverhearSaturosAndMenardi(void)
{
    u8 *record;
    s32 x, z;
    s32 evt;
    s32 evt2;

    if (GameFlag_IsSet(FLAG_MET_SATUROS_AND_MENARDI) != 0) {
        return;
    }

    Event_Begin();
    Audio_PlayCue(17);
    GameFlag_Set(FLAG_MET_SATUROS_AND_MENARDI);

    evt = (s32)MsgHaidiaTheyKnowLittleOfThe;
    Event_SetMessage(evt);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);

    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x13333, 0x9999);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 30);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x188, 0x148);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 10);

    record = (u8 *)Engine_ActorGet(0);
    x = *(s16 *)(record + 10);
    z = *(s16 *)(record + 18);
    Actor_SetPosition(ACTOR_JASMINE, x << 16, z << 16);
    Actor_SetPosition(ACTOR_GERALD, x << 16, z << 16);

    Actor_SetSpeed(ACTOR_JASMINE, 0x13333, 0x9999);
    Actor_SetSpeed(ACTOR_GERALD, 0x13333, 0x9999);
    Actor_WalkTo(ACTOR_JASMINE, 0x178, 0x148);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x198, 0x148);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 0);
    Actor_SetAnimation(ACTOR_JASMINE, 0);
    Actor_SetAnimation(ACTOR_GERALD, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(ACTOR_JASMINE, 0x101, 30);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x4000, 0);
    Camera_SetSpeed(0x60000, 0xc000);
    Camera_MoveTo(0xd70000, -1, 0x1590000, 1);
    Engine_CameraWaitForMove();
    Event_Wait(20);
    Audio_PlayCue(61);

    Actor_SetAnimationAndWait(ACTOR_SATUROS, 4);
    Actor_SetAnimation(ACTOR_SATUROS, 4);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 20);
    Actor_FaceDirection(ACTOR_MENARDI, 0, 10);
    Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_SATUROS, 3);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
    Actor_FaceDirection(ACTOR_MENARDI, 0x4000, 60);
    Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
    Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_SATUROS, 3);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_MENARDI, 4);
    Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 6);
    Actor_ShowEmote(ACTOR_SATUROS, 0x100, 0);
    Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_MENARDI, 0, 10);
    Actor_ShowEmote(ACTOR_MENARDI, 0x101, 40);
    Actor_FaceDirection(ACTOR_SATUROS, 0, 60);
    Actor_FaceDirection(ACTOR_SATUROS, 0x8000, 40);
    Actor_FaceDirection(ACTOR_SATUROS, 0, 40);
    Actor_WalkToAndWait(ACTOR_SATUROS, 232, 0x168);
    Actor_FaceDirection(ACTOR_SATUROS, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_MENARDI, 3);
    Event_Wait(10);
    Actor_WalkTo(ACTOR_SATUROS, 0x188, 0x168);
    Actor_WalkToAndWait(ACTOR_MENARDI, 216, 0x168);
    Actor_WalkTo(ACTOR_MENARDI, 0x178, 0x168);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x4000, 0);
    Camera_MoveTo(0x1890000, -1, 0x1530000, 1);
    Actor_WalkTo(ACTOR_SATUROS, 0x188, 0x168);
    Actor_WalkToAndWait(ACTOR_MENARDI, 0x178, 0x168);
    Actor_SetAnimation(ACTOR_SATUROS, 0);
    Actor_SetAnimation(ACTOR_MENARDI, 0);
    Actor_FaceDirection(ACTOR_SATUROS, 0xd000, 0);
    Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 30);
    Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
    Actor_ShowEmote(ACTOR_GERALD, 258, 60);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_MENARDI, 4);
    Event_OpenMessage(0x100f, 0);

    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_SetMessage(evt + 10);
    } else {
        Event_SetMessage(evt + 11);
    }

    Event_ShowMessageAndWait(0x100f, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);

    evt2 = (s32)MsgHaidiaYoureTheOnesSneakingAround;
    Event_SetMessage(evt2);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_FaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 40);
    Actor_FaceDirection(ACTOR_SATUROS, 0xd000, 0);
    Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 20);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 60);
    Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_MENARDI, 3);
    Event_ShowMessageAndWait(0x100f, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Actor_SetAnimationAndWait(ACTOR_JASMINE, 3);
    Event_ShowMessageAndWait(0x1005, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
    Actor_FaceDirection(ACTOR_SATUROS, 0xa000, 20);
    Event_OpenMessage(ACTOR_SATUROS, 0);

    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_SetMessage(evt2 + 5);
    } else {
        Event_SetMessage(evt2 + 6);
    }

    Actor_FaceDirection(ACTOR_JASMINE, 0, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 20);
    Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
    Actor_FaceActor(ACTOR_SATUROS, ACTOR_GERALD, 30);
    Actor_FaceActor(ACTOR_SATUROS, ACTOR_JASMINE, 30);
    Actor_ShowEmote(ACTOR_SATUROS, 0x105, 80);
    Actor_SetAnimationAndWait(ACTOR_SATUROS, 4);
    Event_SetMessage((s32)MsgHaidiaSaturosGo);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 6);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Actor_ShowEmote(ACTOR_JASMINE, 0x100, 0);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_StartRepeatedMotion(ACTOR_JASMINE, 1);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x4000, 10);
    Event_ShowMessageAndWait(0x1005, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
    Actor_FaceDirection(ACTOR_MENARDI, 0, 10);
    Event_ShowMessageAndWait(0x100f, 0, 10);
    Actor_FaceDirection(ACTOR_SATUROS, 0x8000, 20);
    Actor_SetAnimationAndWait(ACTOR_SATUROS, 4);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_SATUROS, 0xb000, 0);
    Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 20);
    Actor_SetSpeed(ACTOR_MENARDI, 0x8000, 0x4000);
    Actor_SetSpeed(ACTOR_SATUROS, 0x8000, 0x4000);

    record = (u8 *)Engine_ActorGet(14);
    *(record + 90) &= 0xfe;
    record = (u8 *)Engine_ActorGet(15);
    *(record + 90) &= 0xfe;

    Actor_WalkTo(ACTOR_SATUROS, 0x188, 0x178);
    Actor_WalkToAndWait(ACTOR_MENARDI, 0x178, 0x178);
    Event_Wait(6);

    record = (u8 *)Engine_ActorGet(14);
    *(record + 90) |= 1;
    record = (u8 *)Engine_ActorGet(15);
    {
        /* FAKEMATCH: a result temporary, not the compound or-assign the
         * first occurrence above uses: the reference merges the byte into
         * the mask's register, which the two-address ORR does only when
         * the result is its own object. */
        u8 merged = (u8)(*(record + 90) | 1);

        *(record + 90) = merged;
    }

    Actor_SetAnimation(ACTOR_SATUROS, 0);
    Actor_SetAnimation(ACTOR_MENARDI, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 1, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Audio_PlayCue(17);

    Actor_SetAnimation(ACTOR_GERALD, 2);
    record = (u8 *)Engine_ActorGet(0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);

    Actor_SetAnimation(ACTOR_JASMINE, 2);
    record = (u8 *)Engine_ActorGet(0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_JASMINE, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(ACTOR_JASMINE);
    Actor_SetPosition(ACTOR_JASMINE, 0, 0);
    Call3(Object_SetTargetAndCallback, 14, 0x10000, (s32)Sukureta_StrangerActions);
    Call3(Object_SetTargetAndCallback, 15, 0x10000, (s32)Sukureta_StrangerActions);
    Audio_PlayCueFromEventWork();
    Event_End();
}
