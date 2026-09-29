#include "HASHIRA.H"

/*
 * Each Func_ name labels the call word of one call site rather than a
 * runtime address.  The first call is made before r0 is disturbed, so the
 * index is passed straight through instead of being materialised again.  The
 * two exits differ: the thirty-two frame cap returns without pinning, while
 * the clamp path pins the record to exactly 0x1999.
 */
void StagedActor_StepDownUntilClamp(s32 index)
{
    u8 *obj = Actor_Get(index);
    u32 cnt;

    obj[0x55] = 0;

    cnt = 0;
    for (;;) {
        if (cnt > 31) return;
        Stage_Wait(1);
        *(s32 *)(obj + 28) += -0x1999;
        *(s32 *)(obj + 12) += -0xcccc;
        cnt++;
        if (*(s32 *)(obj + 28) <= 0x1998) {
            *(s32 *)(obj + 28) = 0x1999;
            return;
        }
    }
}

void FieldScene_RunPrimarySequence(void)
{
    s32 FieldScene_RunScene3b3SequenceD();

    s32 rec;
    s32 flag;
    s32 p6;
    s32 p5;
    s32 record;
    s32 v1;
    s32 v2;
    s32 v3;
    u8 *base;
    u8 slot16[40];

    rec = Actor_Get(ACTOR_PARTY_LEADER);
    flag = gFrameCount & 3;
    if (flag == 0) {
        base = slot16;
        *(s32 *)(base + 4) = 10;
        *(s32 *)(base + 8) = 0xb333;
        *(s32 *)(base + 12) = 0xb333;
        v1 = Random_Next();
        p6 = *(s32 *)(rec + 8) + ((((u32)((v1 << 4) + v1) >> 16) - 8) << 16);
        v2 = Random_Next();
        p5 = *(s32 *)(rec + 16) + ((((u32)((v2 << 4) + v2) >> 16) - 8) << 16);
        v3 = Random_Next();
        record = Math_Divide((((u32)((v3 << 2) + v3) >> 16) << 16) + 0x30000, 10);
        Call8(Effect_Spawn, p6, *(s32 *)(rec + 12), p5, 0, record, flag, 0x90001, (s32)base);
    }
}

s32 FieldScene_RunScene3b3SequenceD(void)
{
    s32 FieldScene_RunScene3b3SequenceD();

    u8 *rec;
    u8 *pflag;
    s32 saved;
    s32 mode;
    s32 *p;
    s32 buf[3];

    rec = Actor_Get(ACTOR_PARTY_LEADER);
    pflag = rec + 85;
    saved = *pflag;
    mode = (*(u16 *)(rec + 6) + 0x2000) & 0xc000;
    if (gCell[249][0] != 0) {
        return 0;
    }
    p = buf;
    p[0] = (*(s32 *)(rec + 8) & -0x100000) + 0x80000;
    p[1] = *(s32 *)(rec + 12);
    p[2] = (*(s32 *)(rec + 16) & -0x100000) + 0x80000;
    Call3(Vector_AddPolarOffset, 0x100000, mode, (s32)p);
    if (Value2(Object_CheckMovementCollision, (s32)rec, (s32)p) == 1) {
        goto reject;
    }
    if (Value2(StagedActor_FindAtTile, (s32)p, (s32)rec) != 0) {
        goto reject;
    }
    p[0] = (*(s32 *)(rec + 8) & -0x100000) + 0x80000;
    p[1] = *(s32 *)(rec + 12);
    p[2] = (*(s32 *)(rec + 16) & -0x100000) + 0x80000;
    Call3(Vector_AddPolarOffset, 0x200000, mode, (s32)p);
    if (Value2(StagedActor_FindAtTile, (s32)p, (s32)rec) != 0) {
        goto reject;
    }
    if (Value2(Object_CheckMovementCollision, (s32)rec, (s32)p) != 0) {
        goto reject;
    }
    Event_Begin();
    Stage_SetMode((s32)rec, 6);
    Stage_Wait(6);
    Audio_PlayCue(152);
    Stage_SetMode((s32)rec, 7);
    *(s32 *)(rec + 48) = 0x30000;
    *(s32 *)(rec + 52) = 0x20000;
    *(s32 *)(rec + 40) = 0x40000;
    *pflag &= 126;
    Actor_SetSpriteFlags((s32)rec, 0);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, *(s16 *)((u8 *)p + 2), *(s16 *)((u8 *)p + 10));
    Stage_SetMode((s32)rec, 6);
    Actor_SetSpriteFlags((s32)rec, 1);
    *pflag = saved;
    Event_End();
    return 1;
reject:
    return 0;
}

void FieldScene_RunScene3b3SequenceE(union FieldObject *object)
{
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;

    object->effect.x += object->effect.velocity_x;
    object->effect.y += object->effect.velocity_y;
    object->effect.z += object->effect.velocity_z;
    velocity_x = object->effect.velocity_x;
    velocity_y = object->effect.velocity_y;
    velocity_z = object->effect.velocity_z;
    object->effect.velocity_x = velocity_x - Math_Divide(velocity_x, 10);
    object->effect.velocity_y = velocity_y - Math_Divide(velocity_y, 3);
    object->effect.velocity_z = velocity_z - Math_Divide(velocity_z, 10);
    object->effect.scale_x += object->effect.scale_rate_x;
    object->effect.scale_y += object->effect.scale_rate_y;
    object->effect.sprite->rotation += object->effect.spin;
}

s32 SceneActor_ApplyCounterLowBitsAsMode(u8 *actor)
{
    Object_SetPalette(actor, *(u16 *)(actor + 100) & 15);
    return 0;
}
