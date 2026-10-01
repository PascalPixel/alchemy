#include "ENTRY_SETUP.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_EFFECT.H"

void BattleFx_SetQueuedSoundAndPlay(s32 cue);
void SceneState_CallWith432And32(void);

void FieldScene_CallWith560And44(void);

/* The spray's motion script. */
extern const s32 gVinasuSprayScript[];

struct FieldActor;
s32 VinasuHeya_UpdateRisingSpray(struct FieldActor *actor);

/* Rubble falling from the ceiling: on every fourth frame a coin toss drops
 * either a sinking piece near (x, z) or a slower one over x's own band, both
 * shrunk to 0.7 and given a random spin. */
void VinasuHeya_SpawnRandomParticles(s32 x, s32 z)
{
    struct EffectOptions options;
    struct EffectOptions *p = &options;
    register struct EffectOptions *opts asm("r8"); /* FAKEMATCH: the options pointer is held in r8 once both scales are stored through p */
    s32 phase;
    u32 coin;

    p->start_scale_x = 0xb333;
    p->start_scale_y = 0xb333;
    opts = p;
    {
        s32 spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
        u32 *frame = &gFrameCount; /* FAKEMATCH: the counter's address is loaded before the spin is stored */

        opts->spin = spin;
        phase = *frame & 3;
    }
    if (phase == 0) {
        coin = (u32)(Engine_RandomNext() << 1) >> 16;
        if (coin != 0) {
            s32 r = Engine_RandomNext();
            s32 vz = Engine_MathDivide((((u32)(Engine_RandomNext() * 5) >> 16) << 16) + 0x70000, 10);

            Effect_Spawn((x + (((u32)(r << 1) >> 16) << 4)) << 16, 0, z << 19, 0, phase, vz, 0x880000, opts);
        } else {
            s32 r = Engine_RandomNext();

            Effect_Spawn((x + ((u32)(r * 17) >> 16)) << 16, 0, (x << 19) - 0x40000, 0, coin, coin, 0x880000, opts);
        }
    }
}

/*
 * Two rubble-fall task callbacks: each spawns the random particles over its
 * own band.  The 16-byte owners load no literal, so they carry no pool word
 * and no alignment halfword.  432 is built from a shifted immediate and
 * passed straight to the callee as a value, not used as a displacement.
 */
void SceneState_CallWith432And32(void)
{
    VinasuHeya_SpawnRandomParticles(432, 32);
}

void FieldScene_CallWith560And44(void)
{
    VinasuHeya_SpawnRandomParticles(0x230, 44);
}

/* The ceiling gives way: rubble rains down in thirteen rows, the fourth row
 * four times, while the map fills in behind it. */
void Scene_RunParticleWaveSequence(void)
{
    struct EffectOptions options;
    u32 repeat;
    /* The spawn velocities, held in one register for the stack arguments. */
    s32 zero;
    u32 row;
    s32 offset;
    s32 z;
    u32 i;
    struct EffectOptions *opts;

    gEventWork->start_transition = 0x202;
    Engine_EventBegin();
    Engine_ActorSetSpriteFlags(Object_GetById(0), 0);
    Engine_ActorSetChildValue(0, 15);
    BattleFx_SetQueuedSoundAndPlay(170);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Engine_AudioPlayCue(162);
    Engine_CameraSetSpeed(0x8000, 0x1000);
    Engine_CameraMoveTo(0x1b80000, -1, 0x1680000, 1);
    repeat = 0;
    row = 0;
    opts = &options;
    offset = 0;
    do {
        options.start_scale_x = ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
        options.start_scale_y = ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
        options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
    again:
        z = 0xc00000;
        for (i = 0, zero = 0, z += offset; i <= 3; i++) {
            Effect_Spawn((((u32)(Engine_RandomNext() * 7) >> 16) << 19) + 0x1a00000, 0, z, 0, zero, zero, 0x880000, opts);
            z += 0x40000;
        }
        Engine_TaskWait(3);
        if (row == 3) {
            if (repeat <= 2) {
                repeat++;
                goto again;
            }
            if (repeat == 3)
                Engine_TaskAddCallback(SceneState_CallWith432And32, 3200);
        }
        Map_CopyCellsTo(53, row + 12, 26, row + 12, 3, 1);
        offset += 0x100000;
        row++;
    } while (row <= 12);
    Map_CopyCellsTo(81, 41, 89, 14, 9, 2);
    Engine_CameraWaitForMove();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_EventWait(60);
    Engine_GameFlagSet(0x306);
    Engine_EventRequestExit(19);
    Engine_EventEnd();
}

/* The ceiling gives way: rubble rains down in thirteen rows, the fourth row
 * four times, while the map fills in behind it. */
void Scene_RunEastParticleWaveSequence(void)
{
    struct EffectOptions options;
    u32 repeat;
    /* The spawn velocities, held in one register for the stack arguments. */
    s32 zero;
    u32 row;
    s32 offset;
    s32 z;
    u32 i;
    struct EffectOptions *opts;

    gEventWork->start_transition = 0x202;
    Engine_EventBegin();
    Engine_ActorSetSpriteFlags(Object_GetById(0), 0);
    Engine_ActorSetChildValue(0, 15);
    BattleFx_SetQueuedSoundAndPlay(170);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Engine_AudioPlayCue(162);
    Engine_CameraSetSpeed(0x8000, 0x1000);
    Engine_CameraMoveTo(0x2380000, -1, 0x1680000, 1);
    repeat = 0;
    row = 0;
    opts = &options;
    offset = 0;
    do {
        options.start_scale_x = ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
        options.start_scale_y = ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
        options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
    again:
        z = 0xc00000;
        for (i = 0, zero = 0, z += offset; i <= 3; i++) {
            Effect_Spawn((((u32)(Engine_RandomNext() * 7) >> 16) << 19) + 0x2200000, 0, z, 0, zero, zero, 0x880000, opts);
            z += 0x40000;
        }
        Engine_TaskWait(3);
        if (row == 3 && repeat <= 2) {
            repeat++;
            goto again;
        }
        Engine_TaskAddCallback(FieldScene_CallWith560And44, 3200);
        Map_CopyCellsTo(58, row + 12, 34, row + 12, 3, 1);
        offset += 0x100000;
        row++;
    } while (row <= 12);
    Map_CopyCellsTo(86, 41, 97, 14, 5, 2);
    Engine_CameraWaitForMove();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_EventWait(60);
    Engine_GameFlagSet(0x307);
    Engine_EventRequestExit(20);
    Engine_EventEnd();
}

/* Every fourth frame, sprays a rising effect (type 286) from a random point
 * within 24 pixels of actor, with cue 246 every eighth frame. */
s32 VinasuHeya_UpdateRisingSpray(struct FieldActor *actor)
{
    struct EffectOptions options;
    struct EffectOptions *o = &options;
    s32 phase;
    s32 x, y, z, rise;

    o->priority = 1;
    o->palette = 5;
    o->type = 286;
    o->script = gVinasuSprayScript;
    phase = *(volatile u32 *)&gFrameCount & 3;
    if (phase == 0) {
        if ((*(volatile u32 *)&gFrameCount & 7) == 0) {
            Engine_AudioPlayCue(246);
        }
        x = actor->x.fixed + ((((u32)(Engine_RandomNext() * 49) >> 16) - 24) << 16);
        y = actor->y.fixed + ((((u32)(Engine_RandomNext() * 49) >> 16) - 24) << 16);
        z = actor->z.fixed + ((((u32)(Engine_RandomNext() * 49) >> 16) - 24) << 16);
        rise = (((u32)(Engine_RandomNext() * 4) >> 16) << 15) + 0x8000;
        Effect_Spawn(x, y, z, 0, rise, phase, 0x330000, o);
    }
    return 0;
}

void FieldScene_RunLeaderDropSequence(void)
{
    u32 i;
    u8 *p8;
    s32 rec;
    u8 *rec8;
    s32 record;
    s32 none;
    s32 v2;
    s32 slot0;

    rec = Object_GetById(ACTOR_PARTY_LEADER);
    rec8 = Object_GetById(20);
    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Map_Redraw();
    Task_Wait(1);
    *(s32 *)(rec + 12) = 0x820000;
    *(s32 *)(rec + 72) = 0x8000;
    none = 0;
    *(s32 *)(rec + 68) = none;
    p8 = rec + 85;
    *p8 = none;
    Event_OpenScreen();
    Event_WaitForScreen();
    Audio_PlayCue(204);
    Event_Wait(30);
    *p8 = 3;
    Event_Wait(24);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x101);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 22);
    *p8 &= 254;
    *(s32 *)((s32)rec8 + 12) += -0x30000;
    *(s32 *)(rec + 12) += -0x30000;
    *(s32 *)(rec + 20) += -0x30000;
    Task_Wait(2);
    *(s32 *)((s32)rec8 + 12) += -0x20000;
    *(s32 *)(rec + 12) += -0x20000;
    *(s32 *)(rec + 20) += -0x20000;
    Task_Wait(10);
    *(s32 *)((s32)rec8 + 12) += 0x20000;
    *(s32 *)(rec + 12) += 0x20000;
    *(s32 *)(rec + 20) += 0x20000;
    Task_Wait(4);
    *(s32 *)((s32)rec8 + 12) += 0x20000;
    *(s32 *)(rec + 12) += 0x20000;
    *(s32 *)(rec + 20) += 0x20000;
    Task_Wait(4);
    *(s32 *)((s32)rec8 + 12) += 0x10000;
    *(s32 *)(rec + 12) += 0x10000;
    *(s32 *)(rec + 20) += 0x10000;
    *p8 = none;
    rec8[85] = none;
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x100);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Event_Wait(40);
    *(s32 *)(rec + 108) = (s32)VinasuHeya_UpdateRisingSpray;
    Event_Wait(60);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 1);
    Actor_SetSpritePriority(20, 1);
    Audio_PlayCue(17);
    Audio_PlayCue(0x134);
    GameFlag_Set(0x101);
    v2 = 0;
    do {
        *(s32 *)(rec + 12) += 0x10000;
        *(s32 *)(rec + 20) += 0x10000;
        *(s32 *)((s32)rec8 + 12) += 0x10000;
        slot0 = v2;
        Task_Wait(1);
        v2 = slot0;
        v2 = (v2 + 1);
    } while ((u32)v2 <= 127);
    Event_RequestExit(21);
}
