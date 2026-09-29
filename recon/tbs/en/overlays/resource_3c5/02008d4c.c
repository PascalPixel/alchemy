/* Draft of resource_3c5 0x02008d4c..0x02008eac (352 bytes with pool),
 * FieldScene_RunSupplementalSequenceOne; the listing keeps the rows. The C
 * compiles to the reference's instructions, but its Audio_PlayCue is the
 * field event header's inline into Engine_AudioPlayCue, while this overlay's
 * audio import veneer is named Audio_PlayCue for the linked staged-actor code;
 * linking it would give that veneer a second name. */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/BABI_IRIGUCHI/IRIGUCHI.H"

void FieldScene_RunSupplementalSequenceOne(s32 a0)
{
    u32 i;
    struct FieldActor *leader;
    s32 zero;
    struct EffectOptions options;
    s32 vec[3];

    Engine_EventBegin(a0);
    Camera_MoveTo(-1, -1, -1, 0);
    Map_Redraw();
    Iriguchi_TaskWait(1);
    *(s32 *)((u8 *)Engine_ActorGet(0) + 12) = 0x820000;
    *(s32 *)((u8 *)Engine_ActorGet(0) + 72) = 0x8000;
    zero = 0;
    *(s32 *)((u8 *)Engine_ActorGet(0) + 68) = zero;
    *(u8 *)((u8 *)Engine_ActorGet(0) + 85) = zero;
    Event_OpenScreen();
    Event_WaitForScreen();
    Iriguchi_Wait(30);
    Audio_PlayCue(204);
    *(u8 *)((u8 *)Engine_ActorGet(0) + 85) = 3;
    Iriguchi_Wait(24);
    leader = (struct FieldActor *)Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
    options.palette = 7;
    options.update = (void (*)(union FieldObject *))Effect_AdvanceMotion;
    options.start_scale_x = 0xcccc;
    options.start_scale_y = 0xcccc;
    for (i = 0; i < 17; i++) {
        vec[0] = Math_Cos(i << 12);
        vec[1] = 0;
        vec[2] = Math_Sin(i << 12);
        vec[0] += vec[0] / 2;
        Effect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed, vec[0], vec[1], vec[2],
                      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE | 1, &options);
    }
    Audio_PlayCue(188);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x101);
    Iriguchi_SetAnimation(ACTOR_PARTY_LEADER, 22);
    Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    MapRender_WaitForValues();
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x100);
    *(s32 *)((u8 *)Engine_ActorGet(0) + 72) = 0x10000;
    *(s32 *)((u8 *)Engine_ActorGet(0) + 68) = 0x4000;
    Event_End();
}
