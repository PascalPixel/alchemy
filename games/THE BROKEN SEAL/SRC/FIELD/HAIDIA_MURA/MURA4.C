#include "STAGED_MOTION.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "CALL.H"

/* FAKEMATCH: calls through a cast of Object_GetById keep the unprototyped call
 * this file's code made before it shared the header's declaration. */
extern u8 MsgHaidiaDoorWontOpen[];

s32 SceneActor_RunStep18WhenTargetSet(s32 *p)
{
    s32 t = Actor_Get(ACTOR_PARTY_LEADER);
    if (p[14] == (s32)0x80000000 && p[16] == (s32)0x80000000)
        return 0;
    HaidiaMura_TestFacing((s32)p, t, 18, 0);
    return 0;
}

void Effect_ConfigureSpawnedParticle(struct SourceEntity *source)
{
    s32 spawn_position[3];
    s32 particle_index;
    struct StagedParticle *particle;
    spawn_position[0] = source->f08;
    spawn_position[1] = source->f0c - (((s32 (*)())Engine_RandomNext)(source) << 4) + (s32)0xfff80000;
    spawn_position[2] = source->f10;
    particle_index = Random_Next();
    Vector_AddPolarOffset(((particle_index << 1) + particle_index) << 4, Random_Next(), spawn_position);
    particle = Engine_ObjectCreate(0x11d, spawn_position[0], spawn_position[1], spawn_position[2]);
    if (particle != 0) {
        particle->f55 = 2;
        particle->f48 = 0x1999;
        particle->f5e = 12;
        Actor_SetSpriteFlags(particle, 0);
        Object_SetAnimation(particle, 0);
        Object_SetScript(particle, (s32)gDustBurstScript);
        {
            struct ParticleRecord *record = particle->f50;
            s32 record_flags = ~12;
            record_flags &= record->f09;
            record_flags |= 4;
            record->f09 = record_flags;
        }
    }
    Audio_PlayCue(0x8a);
}

void Effect_SpawnRisingDustBurst(struct Resource373Emitter *emitter)
{
    s32 frame_countdown;

    Audio_PlayCue(154);

    for (frame_countdown = 30; frame_countdown >= 0; frame_countdown--) {
        emitter->y += 0x10000;              /* 0x80 << 9. */
        emitter->field06 = (u16)(emitter->field06 + 0x2000);  /* 0x80 << 6. */
        emitter->field18 += -2048;          /* The pool word 0xfffff800. */
        emitter->field1c += -2048;
        Task_Wait(1);
    }

    for (frame_countdown = 7; frame_countdown >= 0; frame_countdown--) {
        struct Resource373Particle *particle =
            Engine_ObjectCreate(0x11d, emitter->x, emitter->y, emitter->z);

        if (particle != 0) {
            s32 vertical_speed;

            Actor_SetSpriteFlags(particle, 0);
            Object_SetScript(particle, (const void *)&gDustBurstScript[1]);

            vertical_speed = Random_Next() + 0x10000;
            particle->field34 = 0x10000;
            particle->field30 = vertical_speed;
            particle->field55 = 2;
            particle->field48 = 0x0a3d;

            particle->lifetime = Random_Next() - Random_Next();

            Effect_UpdateParticlePosition(
                particle,
                ((Random_Next() * 3) << 3) + 0x80000,
                Random_Next());
        }
    }

    Audio_PlayCue(131);

    emitter->x = 0;
    emitter->y = 0;
    emitter->z = 0;
    emitter->field38 = (s32)0x80000000;
    emitter->field3c = (s32)0x80000000;
    emitter->field40 = (s32)0x80000000;
    emitter->field24 = 0;
    emitter->field28 = 0;
    emitter->field2c = 0;
}

void Effect_UpdateParticlePosition(s32 *particle, s32 delta_x, s32 delta_z)
{
    s32 position[3];
    if (particle != 0) {
        position[0] = particle[2];
        position[1] = particle[3];
        position[2] = particle[4];
        Vector_AddPolarOffset(delta_x, delta_z, position);
        Object_SetPosition((s32)particle, position[0], position[1], position[2]);
    }
}

void SceneState_ApplyRectAndRunTwo(void)
{
    s32 e = 22;
    s32 f = 36;
    Map_CopyCellAttributes(17, 0, 3, 1, e, f);
    StagedActor_AdvancePair();
    HaidiaMura_OpenVillagerLane();
}

/* Opens the lane beside whichever villager stands in it: the cells copied depend on the villager's column, and the matching flag is set. */
void HaidiaMura_OpenVillagerLane(void)
{
    struct FieldActor *actor;
    s32 column;

    Call6((void (*)())Engine_MapCopyCellAttributes, 17, 0, 3, 1, 22, 36);
    if (Engine_GameFlagIsSet(0x87a))
        actor = Object_GetById(21);
    else
        actor = Object_GetById(20);
    if (actor == NULL)
        return;
    Call1((void (*)())Engine_GameFlagClear, 0x314);
    Call1((void (*)())Engine_GameFlagClear, 0x315);
    Call1((void (*)())Engine_GameFlagClear, 0x316);
    column = actor->x.fixed >> 20;
    if (column == 22) {
        Engine_MapCopyCellAttributes(17, 1, 1, 1, column, 36);
        Engine_GameFlagSet(0x314);
    } else if (column == 23) {
        Engine_MapCopyCellAttributes(17, 1, 1, 1, column, 36);
        Engine_GameFlagSet(0x315);
    } else {
        Call6((void (*)())Engine_MapCopyCellAttributes, 17, 1, 1, 1, 24, 36);
        Engine_GameFlagSet(0x316);
    }
}

void Effect_PlayStepSound(void)
{
    if ((*(u32 *)&gFrameCount & 15) == 0)
        Audio_PlayCue(0x83);
}

void FieldScene_RunScriptedStepEE4(void)
{
    Event_Begin();
    Message_ShowCentered((s32)MsgHaidiaDoorWontOpen, 1);
    Event_End();
}

void FieldScene_RunScene373SequenceB(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    if (GameFlag_IsSet(0x241) != 0) {
        rec7 = GameFlag_IsSet(0x106);
        if (rec7 != 0) {
            goto L_02005a8a;
        }
        *(u8 *)((u8 *)Object_GetById(22) + 91) = rec7;
        GameFlag_Clear(0x241);
    } else {
        if (GameFlag_IsSet(0x106) != 0) {
            *(u8 *)((u8 *)Object_GetById(22) + 91) = 1;
            GameFlag_Set(0x241);
        }
    }
    L_02005a8a:;
}

void SceneActor_SetFlagByteBySlotZeroPosition(void)
{
    s32 *g = Actor_Get(ACTOR_PARTY_LEADER);
    u8 *q;
    if (GameFlag_IsSet(0x87a) != 0)
        q = Actor_Get(21);
    else
        q = Actor_Get(20);
    if (q != 0) {
        if (g[3] > 0xc80000)
            q[0x23] = 3;
        else
            q[0x23] = 1;
    }
}

s32 SceneEffect_UpdateOrbitPosition(s32 *p)
{
    s16 *q = (s16 *)p[20];
    s32 a, b;
    s32 d = Math_Sin(p[12]) * 2;
    if (d > 0)
        d = -d;
    p[2] = p[14] + Math_Cos(p[12]) * 2;
    p[3] = p[15] + d;
    q[15] = Math_Cos(p[12] + 0x8000) / 8;
    a = Random_Next();
    b = Random_Next();
    p[12] = p[12] + ((((u32)a << 9) >> 16) + (((u32)b << 9) >> 16)) + 0x400;
    return 0;
}

void InitializeStagedActorSceneOrbitingEffect(void)
{
    OrbitingSceneObject *actor;
    OrbitingSceneObjectSprite *sprite;
    u8 *transfer;
    s32 zero;

    actor = ((u8 * (*)())Object_GetById)();
    sprite = actor->sprite;
    sprite->flags_09_mode = 1;
    sprite->flags_05_bit_5 = 0;
    sprite->flags_09_high = 0;

    zero = 0;
    sprite->state = zero;
    Actor_SetSpriteFlags(actor, zero);
    actor->active = zero;
    actor->mode = zero;

    if (GameFlag_IsSet(0x109) == 0)
        actor->y += 0x200000;

    actor->flags_23 &= 0xfe;
    actor->visible = 1;

    transfer = Runtime_AllocateHeapBlock(17, 0x608);
    Item_LoadIcon(ITEM_NUT);
    transfer += 0x400;
    Vram_Load(sprite->palette, 128, transfer);
    Heap_Release(17);

    actor->orbit_center_x = actor->x;
    actor->orbit_angle = zero;
    actor->orbit_center_y = actor->y;
    actor->active = 1;
    actor->callback = (u32)SceneEffect_UpdateOrbitPosition;
    actor->state = zero;
}

void OverlayObject_UpdateOnFrameBit1(s32 p)
{

    if ((gFrameCount & 2) != 0)
        Object_SetPartPalettes(p, 7);
    else
        Object_SetPartPalettes(p, 0);
    if ((gFrameCount & 0xf) == 0)
        WorldMap_CreateLinkedEffects(p);
}

void SceneEffect_UpdateObjectOnOddFrames(s32 p)
{
    extern volatile u32 gFrameCount;

    if ((gFrameCount & 1) != 0)
        Object_SetPartPalettes(p, (gFrameCount >> 1) % 6);
    if ((gFrameCount & 0xf) == 0)
        WorldMap_CreateLinkedEffects(p);
}

void SceneEffect_UpdateObjectOnOddFramesOnly(s32 p)
{
    extern volatile u32 gFrameCount;

    if ((gFrameCount & 1) != 0)
        Object_SetPartPalettes(p, (gFrameCount >> 1) % 6);
}

void Effect_AnimateVerticalPositive(struct StagedVerticalEffect *effect)
{
    s32 *anchor = effect->f68;
    s32 frame = ++effect->f64;
    if (frame > 31) {
        Engine_ObjectDispatchRelease((s32)effect);
    } else {
        s32 amplitude = Math_Sin(frame << 10);
        s32 offset;
        effect->f18 = amplitude;
        effect->f1c = amplitude;
        effect->f8 = anchor[2];
        effect->fc += 0x10000;
        offset = 0x10000 - amplitude;
        effect->f10 = anchor[4] + offset * 5 + 0x80000;
    }
}

void Effect_AnimateVerticalNegative(struct StagedVerticalEffect *effect)
{
    s32 *anchor = effect->f68;
    s32 frame = ++effect->f64;
    if (frame > 31) {
        Engine_ObjectDispatchRelease((s32)effect);
    } else {
        s32 amplitude = Math_Sin(frame << 10);
        s32 offset;
        effect->f18 = amplitude;
        effect->f1c = -amplitude;
        effect->f8 = anchor[2];
        effect->fc += 0x10000;
        offset = 0x10000 - amplitude;
        effect->f10 = anchor[4] - offset * 5 + 0x100000;
    }
}
