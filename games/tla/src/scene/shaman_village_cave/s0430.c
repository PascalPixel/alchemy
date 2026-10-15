#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "call.h"
#include "text/msg_ids.h"
#include "scene_ids.h"

void Battle_WaitMode0();
void Func_02001080();
void Func_020010f0();
void Func_02001138();
void Func_02001148();
void Func_02001158();
void Func_02001160();
void Func_02001180();
void Func_020011a0();
void Func_020011e0();
void Func_020011f0();
void GameFlag_SetBit();
void GameFlag_Test();
void Motion_CamBounds();
void Motion_SetModeAndWaitAnimation();
void Motion_SetVarCbAndRefresh();
void ObjectMotion_ArmCallback();
void ObjectMotion_CommitCurrentPositionAndActivate();
void ObjectMotion_OffsetPositionAndReset();
void ObjectMotion_OffsetPositionAndResetMotion();
void ObjectMotion_ResetAndSetPositionInMode2();
void ObjectMotion_SetPositionAndReset();
void Object_LinkPair();
void Object_SetModeById();
void Party_SetFields1eeAnd1f0();
void Party_SetFields1f2And1f4();

#define SHAMAN_VILLAGE_CAVE_EMOTE(actor, emote, frames)                                          \
    {                                                                                            \
        /* FAKEMATCH: the native loads the emote and the frames before the actor at every */     \
        /* emote of this overlay; pinned to their argument registers they are loaded first, */   \
        /* as there. Through an inline the actor comes one row early wherever the emote takes */ \
        /* two instructions, and an empty asm barrier at each call moves the literal pools of */ \
        /* a long scene. */                                                                      \
        register s32 emote_ __asm__("r1") = (emote);                                             \
        /* FAKEMATCH: the frames, pinned with the emote above. */                                \
        register s32 frames_ __asm__("r2") = (frames);                                           \
        Func_02001160((actor), emote_, frames_);                                                 \
    }

// The JA edition has its own form of this function; it stays on its listing in recon/tla/raw/overlays (S4).
#if !defined(TLA_EDITION_JA)
void Func_02000430(void)
{
    s32 angle;

    if (Value1((s32 (*)())GameFlag_Test, 0x821) == 0)
        return;
    if (Value1((s32 (*)())GameFlag_Test, 0) == 0)
        return;
    Func_02001080();
    Call1(Func_020011e0, 0);
    Call1(GameFlag_SetBit, 0xa70);
    Call1(Func_02001138, (s32)MsgIFound2);
    Call2(Object_SetModeById, 4, 1);
    Call3(Func_02001148, 19, 0, 1);
    Call3(ObjectMotion_SetPositionAndReset, 4, 200, 221);
    Call2(Motion_SetVarCbAndRefresh, 4, 2);
    Call1(Battle_WaitMode0, 5);
    Call3(ObjectMotion_OffsetPositionAndReset, 4, -16, 8);
    Call4(Func_020011f0, 0, 0, 0, 0xc000);
    Call4(Func_020011f0, 18, 8, 16, 0xc000);
    Call1(ObjectMotion_CommitCurrentPositionAndActivate, 0);
    Call1(ObjectMotion_CommitCurrentPositionAndActivate, 18);
    Call3(ObjectMotion_ArmCallback, 0, 0x4000, 0);
    Call1(Battle_WaitMode0, 20);
    Call3(ObjectMotion_ArmCallback, 0, 0xc000, 0);
    Call3(ObjectMotion_ArmCallback, 4, 0xc000, 0);
    Call3(ObjectMotion_ArmCallback, 18, 0xc000, 0);
    Call1(Battle_WaitMode0, 10);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see SHAMAN_VILLAGE_CAVE_EMOTE. */
    SHAMAN_VILLAGE_CAVE_EMOTE(0, 256, 0)
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see SHAMAN_VILLAGE_CAVE_EMOTE. */
    SHAMAN_VILLAGE_CAVE_EMOTE(18, 256, 0)
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see SHAMAN_VILLAGE_CAVE_EMOTE. */
    SHAMAN_VILLAGE_CAVE_EMOTE(4, 256, 40)
    Call4(Motion_CamBounds, 0xc70000, -1, 0xb10000, 1);
    Func_02001180();
    angle = 0x4000;
    Call4(Func_020010f0, 19, 0xc70000, 0x820000, angle);
    Call3(ObjectMotion_ResetAndSetPositionInMode2, 19, 231, 199);
    Call1(Battle_WaitMode0, 20);
    Call4(Func_020010f0, 20, 0xc70000, 0x820000, angle);
    Call3(ObjectMotion_ResetAndSetPositionInMode2, 20, 220, 191);
    Call1(Battle_WaitMode0, 20);
    Call4(Func_020010f0, 21, 0xc70000, 0x820000, angle);
    Call3(ObjectMotion_ResetAndSetPositionInMode2, 21, 204, 191);
    Call4(Motion_CamBounds, 0xc70000, -1, 0xe10000, 1);
    Func_02001180();
    Call1(ObjectMotion_CommitCurrentPositionAndActivate, 19);
    Call3(ObjectMotion_ArmCallback, 19, 0x5000, 0);
    Call1(ObjectMotion_CommitCurrentPositionAndActivate, 20);
    Call3(ObjectMotion_ArmCallback, 20, 0x5000, 0);
    Call1(ObjectMotion_CommitCurrentPositionAndActivate, 21);
    Call2(Func_02001158, 21, 0x5000);
    Call2(Motion_SetVarCbAndRefresh, 21, 2);
    Call3(Func_02001148, 21, 0, 5);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see SHAMAN_VILLAGE_CAVE_EMOTE. */
    SHAMAN_VILLAGE_CAVE_EMOTE(18, 258, 30)
    Call3(Func_02001148, 18, 0, 5);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see SHAMAN_VILLAGE_CAVE_EMOTE. */
    SHAMAN_VILLAGE_CAVE_EMOTE(19, 264, 30)
    Call3(Func_02001148, 19, 0, 5);
    Call2(Motion_SetVarCbAndRefresh, 20, 2);
    Call1(Battle_WaitMode0, 5);
    Call3(Func_02001148, 20, 0, 5);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see SHAMAN_VILLAGE_CAVE_EMOTE. */
    SHAMAN_VILLAGE_CAVE_EMOTE(21, 263, 30)
    Call3(Func_02001148, 21, 0, 5);
    Call2(Func_02001158, 0, 0xe000);
    Call2(Motion_SetModeAndWaitAnimation, 0, 3);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_02001148, 0, 0, 5);
    Call2(Motion_SetVarCbAndRefresh, 19, 2);
    Call1(Battle_WaitMode0, 5);
    Call3(Func_02001148, 19, 0, 5);
    Call3(Object_LinkPair, 18, 4, 0);
    Call1(Battle_WaitMode0, 10);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see SHAMAN_VILLAGE_CAVE_EMOTE. */
    SHAMAN_VILLAGE_CAVE_EMOTE(0, 257, 30)
    Call3(Func_02001148, 0, 0, 5);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see SHAMAN_VILLAGE_CAVE_EMOTE. */
    SHAMAN_VILLAGE_CAVE_EMOTE(20, 263, 30)
    Call3(Func_02001148, 20, 0, 5);
    Call3(ObjectMotion_ArmCallback, 4, 0xe000, 0);
    Call2(Func_02001158, 18, 0xe000);
    Call2(Object_SetModeById, 18, 3);
    Call2(Motion_SetModeAndWaitAnimation, 4, 3);
    Call1(Battle_WaitMode0, 10);
    Call2(Motion_SetModeAndWaitAnimation, 21, 4);
    Call3(Func_02001148, 21, 0, 5);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see SHAMAN_VILLAGE_CAVE_EMOTE. */
    SHAMAN_VILLAGE_CAVE_EMOTE(18, 265, 30)
    Call3(Func_02001148, 18, 0, 5);
    Call2(Object_SetModeById, 21, 4);
    Call2(Object_SetModeById, 19, 4);
    Call2(Motion_SetModeAndWaitAnimation, 20, 4);
    Call3(Func_02001148, 19, 0, 5);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see SHAMAN_VILLAGE_CAVE_EMOTE. */
    SHAMAN_VILLAGE_CAVE_EMOTE(4, 256, 0)
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see SHAMAN_VILLAGE_CAVE_EMOTE. */
    SHAMAN_VILLAGE_CAVE_EMOTE(18, 256, 30)
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see SHAMAN_VILLAGE_CAVE_EMOTE. */
    SHAMAN_VILLAGE_CAVE_EMOTE(0, 258, 30)
    Call3(Func_02001148, 0, 0, 5);
    Call3(Object_LinkPair, 20, 21, 0);
    Call1(Battle_WaitMode0, 10);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see SHAMAN_VILLAGE_CAVE_EMOTE. */
    SHAMAN_VILLAGE_CAVE_EMOTE(20, 263, 30)
    Call3(Func_02001148, 20, 0, 5);
    Call3(ObjectMotion_ArmCallback, 20, 0x5000, 0);
    Call3(ObjectMotion_ArmCallback, 21, 0x5000, 0);
    Call2(Func_02001158, 18, 0xe000);
    Call1(Battle_WaitMode0, 10);
    Call2(Motion_SetModeAndWaitAnimation, 18, 4);
    Call3(Func_02001148, 18, 0, 5);
    Call2(Motion_SetModeAndWaitAnimation, 21, 4);
    Call3(Func_02001148, 21, 0, 5);
    Call3(ObjectMotion_OffsetPositionAndResetMotion, 20, -16, 24);
    Call3(ObjectMotion_OffsetPositionAndResetMotion, 21, -16, 24);
    Call3(ObjectMotion_OffsetPositionAndResetMotion, 19, -16, 24);
    Call1(ObjectMotion_CommitCurrentPositionAndActivate, 19);
    /* FAKEMATCH: a one-pass block loads the party state before its offset is built (one reordered row). */
    do
    {
        gPartyState.battleRule24b = 2;
    } while (0);
    Party_SetFields1eeAnd1f0((s32)&SceneId_Ov695, 20);
    Party_SetFields1f2And1f4((s32)&SceneId_Ov695, 21);
    Call2(Func_020011a0, 102, 2);
}
#endif
