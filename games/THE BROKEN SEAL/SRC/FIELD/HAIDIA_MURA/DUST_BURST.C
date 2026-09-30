#include "STAGED_MOTION.H"

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
