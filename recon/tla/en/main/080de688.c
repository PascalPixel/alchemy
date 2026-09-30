#include "TYPES.H"
#include "SCENE.H"
#include "OBJECT_EFX.H"
#include "FIXED_MATH.H"
#include "SYSTEM.H"
extern u8 Data_03001e40[];
void BattleEffect_InitializeSharedScene(void);
s32 BattleFx_RunEventAction(void *resource, s32 battle_mode, s32 size);

/* battle/effects/orbiting_particles/update_main.c */
struct OrbitingParticle;
void Animation_ApplyChildValuesFar(struct OrbitingParticle *particle, s32 battle_mode);

void BattleFx_RunOrbitingParticles(void)
{
    s32 resource_size;
    struct OrbitingParticleVector position;
    struct OrbitingParticleVector *p;
    struct OrbitingParticleScene *scene;
    struct OrbitingParticle *main_particle;
    struct OrbitingParticle *particle;
    void *resource;
    s32 entry_count;

    scene = gEffectWork;
    main_particle = scene->main_particle;
    BattleEffect_InitializeSharedScene();
    Audio_PlayCue(0x73);

    p = &position;
    entry_count = 15;
    do {
        particle = Object_Spawn(0xe8, 0, 0, 0);
        if (particle != NULL) {
            u32 initial_scale;
            s32 magnitude;

            initial_scale = (Random16() >> 1) + 0x8000;
            particle->scale_y = initial_scale;
            particle->scale_x = initial_scale;
            if ((Random16() & 1) != 0)
                particle->update = BattleFx_UpdateOrbitingParticleLeft;
            else
                particle->update = BattleFx_UpdateOrbitingParticleRight;

            particle->rotation = Random16();
            particle->lifetime = 60;
            particle->orbit_angle = Random16();
            Animation_ApplyChildValuesFar(particle, 9);

            p->x = scene->origin.x;
            p->y = scene->origin.y;
            p->z = scene->origin.z;
            magnitude = (Random16() << 2) + 0x20000;
            Vector_AddPolarOffset(magnitude, Random16(), p);
            particle->orbit_center.x = p->x;
            particle->orbit_center.y = p->y;
            particle->orbit_center.z = p->z;
        }

        WaitFrames(3);
        entry_count--;
    } while (entry_count >= 0);

    WaitFrames(10);
    Audio_PlayCue(0x73);
    WaitFrames(50);

    if (main_particle != NULL && scene->skip_main_animation == 0) {
        Audio_PlayCue(0xd4);

        entry_count = 15;
        do {
            Animation_ApplyChildValuesFar(main_particle, 7);
            WaitFrames(1);
            Animation_ApplyChildValuesFar(main_particle, 0);
            WaitFrames(4);
            entry_count--;
        } while (entry_count >= 0);

        if (scene->skip_main_finish == 0) {
            Audio_PlayCue(0xdc);
            Object_SetMode(main_particle, 2);
        }

        main_particle->update = BattleFx_UpdateOrbitingParticleMain;
        resource = BattleFx_FindMatchingEvent(0x50000005, 6, &resource_size);
        if (resource != NULL) {
            BattleFx_RunEventAction(
                resource,
                gGameState.resource_mode,
                resource_size);
        }
        WaitFrames(20);
    }

    BattleFx_PrepareBufferInterpolation();
}
