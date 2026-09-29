/* The palace guest rooms: the steward asks whether the party met Babi's
 * soldiers, the room keeper offers the four beds or a free rest, and the
 * last host asks about Colosso. Each question loads its first message once
 * and adds to it for the answers. */
/* FAKEMATCH: KYUDEN.H's inline call and value wrappers keep the game's
 * argument order and constant sharing at the calls that use them. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "KYUDEN.H"
extern u8 MsgTorebiTiredFeelFreeRest[];
extern u8 MsgTorebiCameRestBefore[];
extern u8 MsgTorebiPlanningEnterColosso[];
extern u8 MsgTorebiRunBabisSoldiers[];
extern u8 MsgTorebiUseFourBeds[];
extern u8 MsgTorebiWeHaveJustEnoughExtra[];

void FieldScene_RunBranchedSteps1FF1(s32 a)
{
    s32 k = (s32)MsgTorebiRunBabisSoldiers;

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
        Event_SetMessage((s32)MsgTorebiWeHaveJustEnoughExtra);
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
        c = (s32)MsgTorebiUseFourBeds;
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
        base6_2241 = (s32)MsgTorebiCameRestBefore;
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
        Call1(Engine_EventSetMessage, (s32)MsgTorebiTiredFeelFreeRest);
        Event_OpenMessage(a0, 0);
    }
    Event_End();
}

void FieldScene_RunBranchedSteps2006(s32 a)
{
    s32 k = (s32)MsgTorebiPlanningEnterColosso;

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
