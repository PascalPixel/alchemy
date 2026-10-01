/* Draft: Torebi palace room keeper, IT.
 * 2026-10-01: The direct semantic reset has the right complete extent but four differing bytes: the r0 actor copy is scheduled after both coordinate shifts; the own-ROM sequence copies r0 before them. The adopted existing Call3 adapter preserves that order.
 * Approved TBS agscc and ordinary game flags; no output edits.
 */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "CALL.H"
#include "../../../../games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/KYUDEN.H"

extern u8 MsgTorebiWeHaveJustEnoughExtra[];
extern u8 MsgTorebiUseFourBeds[];

void RunOpeningAuxiliarySequence(s32 a)
{

    u8 *ret;
    s16 v;
    s32 c;
    s32 t;

    ret = Actor_Get(ACTOR_PARTY_LEADER);
    v = (*(u16 *)(ret + 6) + 0x2000) & 0xc000;
    Engine_EventBegin();
    Battle_ResetEffectCounterFar();
    if (Value1(Engine_GameFlagIsSet, 512) == 0) {
        Engine_GameFlagSet(512);
        GameFlag_Clear(0x969);
        Engine_EventSetMessage((s32)MsgTorebiWeHaveJustEnoughExtra);
        Event_ShowMessage(a, 0);
        Engine_EventWait(10);
        t = v << 16;
        c = 0x4000;
        if (t == (0x4000 << 16)) {
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 40, 104);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
        }
        Call3(Engine_ActorSetSpeed, a, 0x10000, 0x8000);
        Call3(Engine_ActorWalkByAndWait, a, 0, -48);
        Actor_WalkByAndWait(a, 64, 0);
        Actor_FaceDirection(a, c, 0);
    } else {
        Engine_GameFlagClear(512);
        GameFlag_Set(0x969);
        Call3(Engine_ActorFaceDirection, a, 0x4000, 0);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 96);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        Engine_EventWait(20);
        c = (s32)MsgTorebiUseFourBeds;
        Engine_EventSetMessage(c);
        Event_OpenMessage(a, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            Engine_EventSetMessage(c + 1);
            Event_ShowMessage(a, 0);
        } else {
            Engine_EventSetMessage(c + 2);
            Event_ShowMessage(a, 0);
        }
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(a, 3);
        Engine_EventWait(20);
        Call3(Engine_ActorWalkByAndWait, a, -64, 0);
        Actor_WalkByAndWait(a, 0, 48);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
        Engine_ActorSetPosition(a, 56 << 16, 120 << 16);
#endif
    }
    Engine_EventEnd();
}
