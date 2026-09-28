/* resource_3b8:02008070..020083b0 (832 bytes), still linked from the
 * listing. Remaining differences, all link-time values the game loads from
 * literal pools where integers become immediates or folded constants:
 * SceneData_SelectDataBySelectorAndFlags compares with 0x8b loaded from the
 * pool; FieldScene_RunBranchedSteps1FF1, RunOpeningAuxiliarySequence,
 * FieldScene_RunScene3b8_02000264 and FieldScene_RunBranchedSteps2006 load
 * one message (0x1ff1, 0x1ff8, 0x2241, 0x2006) and derive the next ones as
 * base + n, which an integer message propagates into separate constants;
 * the 0x105 emote is loaded from the pool too. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
/* Declarations and helpers: games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/KYUDEN.H. */

s32 SceneData_SelectDataBySelectorAndFlags(void)
{

    s16 *tbl = (s16 *)Data_02000240;

    if (tbl[0xe0] == 0x8b)
        return (s32)Data_0200cb3c;
    if (GameFlag_IsSet(0x950) != 0)
        return (s32)Data_0200ce6c;
    if (GameFlag_IsSet(0x962) != 0)
        return (s32)Data_0200cd64;
    return (s32)Data_0200cb84;
}

void FieldScene_RunBranchedSteps1FF1(s32 a)
{
    s32 k = 0x1ff1;

    Event_SetMessage(k);
    Event_OpenMessage(a, 0);
    if (Event_ChooseYesNo(0, 0) == 0)
        Event_SetMessage(k + 1);
    else
        Event_SetMessage(k + 2);
    Event_ShowMessage(a, 0);
}

void RunOpeningAuxiliarySequence(s32 a)
{

    u8 *ret;
    s16 v;
    s32 c;
    s32 t;

    ret = Actor_Get(ACTOR_PARTY_LEADER);
    v = (*(u16 *)(ret + 6) + 0x2000) & 0xc000;
    Event_Begin();
    Battle_ResetEffectCounterFar();
    if (Value1_scene_effect_sequence_head(Engine_GameFlagIsSet, 512) == 0) {
        Call1_scene_effect_sequence_head(Engine_GameFlagSet, 512);
        GameFlag_Clear(0x969);
        Event_SetMessage(MSG_WE_HAVE_JUST_ENOUGH_EXTRA);
        Event_ShowMessage(a, 0);
        Event_Wait(10);
        t = v << 16;
        c = 0x4000;
        if (t == (0x4000 << 16)) {
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 40, 104);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
        }
        Call3_scene_effect_sequence_head(Engine_ActorSetSpeed, a, 0x10000, 0x8000);
        Call3_scene_effect_sequence_head(Engine_ActorWalkByAndWait, a, 0, -48);
        Actor_WalkByAndWait(a, 64, 0);
        Actor_FaceDirection(a, c, 0);
    } else {
        Call1_scene_effect_sequence_head(Engine_GameFlagClear, 512);
        GameFlag_Set(0x969);
        Call3_scene_effect_sequence_head(Engine_ActorFaceDirection, a, 0x4000, 0);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 96);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        Event_Wait(20);
        c = 0x1ff8;
        Event_SetMessage(c);
        Event_OpenMessage(a, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_SetMessage(c + 1);
            Event_ShowMessage(a, 0);
        } else {
            Event_SetMessage(c + 2);
            Event_ShowMessage(a, 0);
        }
        Event_Wait(10);
        Actor_SetAnimationAndWait(a, 3);
        Event_Wait(20);
        Call3_scene_effect_sequence_head(Engine_ActorWalkByAndWait, a, -64, 0);
        Actor_WalkByAndWait(a, 0, 48);
    }
    Event_End();
}

void FieldScene_RunScene3b8_02000264(s32 a0)
{
    u32 i;
    s32 record;
    s32 base6_2241;

    Event_Begin();
    Battle_ResetEffectCounterFar();
    if (GameFlag_IsSet(0x966) == 0) {
        GameFlag_Set(0x966);
        GameFlag_Set(0x967);
        Actor_FaceDirection(a0, 0x4000, 0);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 96);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        Event_Wait(20);
        base6_2241 = (s32)0x2241;
        Event_SetMessage(base6_2241);
        Event_OpenMessage(a0, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Engine_EventWait(10);
            Event_SetMessage((base6_2241 + 1));
        } else {
            Event_SetMessage((base6_2241 + 2));
        }
        Event_ShowMessage(a0, 0);
        Event_Wait(10);
        Engine_ActorSetAnimationAndWait(a0, 3);
        Engine_EventWait(20);
        Actor_SetSpeed(a0, 0x10000, 0x8000);
        Actor_WalkByAndWait(a0, -64, 0);
        Engine_ActorWalkByAndWait(a0, 0, 48);
    } else {
        Call1(Engine_EventSetMessage, 0x2245);
        Event_OpenMessage(a0, 0);
    }
    Event_End();
}

void FieldScene_RunBranchedSteps2006(s32 a)
{
    s32 k = 0x2006;

    Event_SetMessage(k);
    Event_OpenMessage(a, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Actor_ShowEmote(a, 0x102, 0x28);
        Event_SetMessage(k + 1);
    } else {
        Event_Wait(10);
        Actor_ShowEmote(a, 0x105, 0x28);
        Event_SetMessage(k + 2);
    }
    Event_ShowMessage(a, 0);
}
