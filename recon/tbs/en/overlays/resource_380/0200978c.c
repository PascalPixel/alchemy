/* Draft of resource_380 0x0200978c (Scene_UnmaskGarcia), built with
 * games/THE BROKEN SEAL/SRC/FIELD/SORU_STAR/STAR.H. Remaining difference:
 * the ROM loads messages 0x10b0 and 0x10b2 from the literal pool where the
 * lines are shown and derives the following lines from them, as link-time
 * message values would; the C constants are hoisted and folded, which
 * shifts the register choices through the rest of the function. The
 * listing keeps these rows. */
#include "STAR.H"


void Scene_UnmaskGarcia(void)
{
    u8 *obj;
    s32 other;
    s32 tbl;
    s32 left;
    s32 cnt;
    s32 mes_a;
    s32 mes_b;

    Audio_PlayCue(161);
    Actor_RunRepeatedMotion(ACTOR_GARCIA_MASKED, 3);
    Event_Wait(40);
    other = Value1(Engine_ActorGet, ACTOR_GARCIA_MASKED);
    if (other != 0) {
        Actor_SetPosition(ACTOR_GARCIA, *(s32 *)(other + 8), *(s32 *)(other + 16));
    }
    Actor_SetPosition(ACTOR_GARCIA_MASKED, 0, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GARCIA, 0x3000, 40);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 3);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 3);
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Event_SayThenWait(5, 20);
    Actor_SetAnimationAndWait(ACTOR_GARCIA, 3);
    Event_Wait(10);
    Actor_StartRepeatedMotion(ACTOR_JASMINE, 3);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x8000, 10);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_SayThenWait(9, 40);
    Actor_SetAnimationAndWait(ACTOR_JASMINE, 3);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 40);
    Actor_RunRepeatedMotion(ACTOR_GARCIA, 2);
    Event_SayThenWait(13, 20);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 1);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_GARCIA, 3);
    Event_Wait(10);
    Event_SayThenWait(13, 40);
    Actor_StartRepeatedMotion(ACTOR_SATUROS, 1);
    Actor_SetAnimationAndWait(ACTOR_SATUROS, 3);
    Event_Wait(10);
    Event_SayThenWait(10, 10);
    Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
    Actor_SetAnimation(ACTOR_MENARDI, 3);
    Event_SayThenWait(11, 80);
    Actor_RunRepeatedMotion(ACTOR_GARCIA, 2);
    Event_SayThenWait(13, 40);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Event_Wait(10);
    Event_SayThenWait(5, 10);
    Actor_StartRepeatedMotion(ACTOR_GARCIA, 2);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_Wait(80);
    Actor_SetAnimationAndWait(ACTOR_JASMINE, 4);
    Event_Wait(20);
    Event_SayThenWait(5, 80);
    Actor_SetAnimationAndWait(ACTOR_GARCIA, 4);
    Event_SayThenWait(13, 80);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Event_Wait(4);
    Event_SayThenWait(5, 20);
    Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
    Actor_SetAnimation(ACTOR_SATUROS, 3);
    Event_SayThenWait(10, 10);
    Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
    Event_SayThenWait(11, 10);
    Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
    Event_SayThenWait(10, 10);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 80);
    Actor_ShowEmote(ACTOR_SUKURETA, 0x105, 80);
    Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
    Actor_FaceDirection(ACTOR_MENARDI, 0x5000, 40);
    Actor_StartRepeatedMotion(ACTOR_MENARDI, 2);
    Event_SayThenWait(11, 20);
    Event_CloseScreen();
    Event_WaitForScreen();
    Camera_MoveTo(0x1050000, -1, 0x1d20000, 0);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x3000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xb000, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_SetSpeed(ACTOR_GERALD, 0x8000, 0x4000);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 244, 0x1de);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x104, 0x1ea);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x3000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xb000, 20);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
    Event_Wait(20);
    tbl = Owner_GetState(1);
    cnt = 0;
    tbl += 216;
    left = 14;
    do {
        u32 id = *(u16 *)(tbl)& 0x1ff;
        tbl = (tbl + 2);
        if (id == 220 || id == 221 || id == 223)
            cnt++;
        left--;
    } while (left >= 0);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        mes_a = MSG_TAKE_THE_STARS_TO_THEM;
        Event_SetMessage(mes_a);
        Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
        Event_Wait(10);
        if (cnt <= 2) {
            Event_SayThenWait(1, 30);
            Actor_WalkToAndWait(ACTOR_GERALD, 252, 0x1e6);
            Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
            Event_Wait(10);
            UiText_ShowCenteredMessage((mes_a + 1), 1, 0);
        } else {
            Event_SetMessage(MSG_GERALD_ILL_TAKE_THEM);
            Event_SayThenWait(ACTOR_GERALD, 30);
        }
    } else {
        if (cnt <= 2) {
            mes_b = MSG_THEY_WONT_LET_JASMINE_GO;
            Event_SetMessage(mes_b);
            Actor_RunRepeatedMotion(ACTOR_GERALD, 3);
            Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
            Event_SayThenWait(1, 10);
            Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
            Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
            Actor_SetSpeed(ACTOR_GERALD, 0x20000, 0x10000);
            obj = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
            obj[90] &= 254;
            Actor_WalkToAndWait(ACTOR_GERALD, 244, 0x1de);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
            Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
            Actor_SetDestination(ACTOR_PARTY_LEADER, 218, 0x1d7);
            Actor_WaitForMove(ACTOR_PARTY_LEADER);
            UiText_ShowCenteredMessage((mes_b + 1), 1, 0);
            Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 30);
            {
                u8 flags = obj[90] | 1;

                obj[90] = flags;
            }
        } else {
            Event_SetMessage(MSG_GERALD_THEY_WONT_LET_JASMINE_GO);
            Actor_RunRepeatedMotion(ACTOR_GERALD, 3);
            Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
            Event_SayThenWait(1, 10);
            Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 30);
        }
    }
    Camera_SetSpeed(0x8000, 0x1000);
    Camera_FollowActor(ACTOR_GERALD, 1);
    Camera_WaitForMove();
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 30);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_SetSpeed(ACTOR_GERALD, 0x8000, 0x4000);
    obj = Value1(Engine_ActorGet, ACTOR_GERALD);
    obj[90] &= 254;
    Actor_WalkToAndWait(ACTOR_GERALD, 0x108, 0x1e2);
    {
        u8 flags = obj[90] | 1;

        obj[90] = flags;
    }
    Actor_WalkToAndWait(ACTOR_GERALD, 0x116, 0x1e0);
    *(s32 *)(obj + 48) = 0x30000;
    *(s32 *)(obj + 52) = 0x20000;
    Audio_PlayCue(153);
    *(s32 *)(obj + 40) = 0x60000;
    Actor_SetAnimation(ACTOR_GERALD, 7);
    Actor_MoveToAndWait(ACTOR_GERALD, 0x138, 0x1d6);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Event_Wait(30);
    Audio_PlayCue(153);
    *(s32 *)(obj + 40) = 0x60000;
    Actor_SetAnimation(ACTOR_GERALD, 7);
    Actor_MoveToAndWait(ACTOR_GERALD, 0x156, 0x1d6);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Event_Wait(30);
    Audio_PlayCue(153);
    *(s32 *)(obj + 40) = 0x60000;
    Actor_SetAnimation(ACTOR_GERALD, 7);
    Actor_MoveToAndWait(ACTOR_GERALD, 0x178, 0x1d6);
    Actor_SetAnimation(ACTOR_GERALD, 1);
}
