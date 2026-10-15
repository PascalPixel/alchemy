#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "objdisp.h"
#include "scene.h"
#include "object_effect_scripts.h"
#include "fixed_math.h"
#include "system.h"
#include "frame.h"
#include "object_dispatch.h"
#include "object_runtime.h"
#include "fx_scene.h"
#include "ram_buffer.h"

struct OrbitingParticle;

s32 BattleFx_UpdateOrbitingParticleMain(struct OrbitingParticle *particle)
{
    s32 phase = gFrameCount & 7;

    if (phase == 0) {
        Animation_ApplyChildValuesFar((struct DispatchObject *)particle, 2);
    } else if (phase == 2) {
        Animation_ApplyChildValuesFar((struct DispatchObject *)particle, 0);
    }
    return 0;
}

void BattleFx_UpdateOrbitingParticleFade(struct ObjectRuntime *object)
{
    s32 scale;

    if (object != NULL) {
        scale = object->scaleX - 0x1000;
        object->scaleY -= 0x1000;
        object->scaleX = scale;
        if (scale <= 0x1000)
            ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)gBattleFx_CommonParticleScript);
    }
}

struct OrbitingParticleVector {
    s32 x;
    s32 y;
    s32 z;
};

void Vector_AddPolarOffset(s32 magnitude, s32 angle, struct OrbitingParticleVector *pos);

void BattleFx_UpdateOrbitingParticleLeft(struct ObjectRuntime *particle)
{
    struct OrbitingParticleVector pos;
    s16 life;
    s32 count;

    if (particle != NULL) {
        count = (u16)particle->action - 1;
        particle->action = count;
        life = count;
        if (life != 0) {
            pos.x = particle->targetX;
            pos.y = particle->targetY;
            pos.z = particle->targetZ;
            Vector_AddPolarOffset(life << 17, (s16)particle->unk66 + (life << 11), &pos);
            particle->x = pos.x;
            particle->y = pos.y;
            particle->z = pos.z;
        } else {
            particle->update = (void (*)(void))BattleFx_UpdateOrbitingParticleFade;
        }
    }
}

void BattleFx_UpdateOrbitingParticleRight(struct ObjectRuntime *particle)
{
    struct OrbitingParticleVector pos;
    s16 life;
    s32 count;

    if (particle != NULL) {
        count = (u16)particle->action - 1;
        particle->action = count;
        life = count;
        if (life != 0) {
            pos.x = particle->targetX;
            pos.y = particle->targetY;
            pos.z = particle->targetZ;
            Vector_AddPolarOffset(life << 17, (s16)particle->unk66 - (life << 11), &pos);
            particle->x = pos.x;
            particle->y = pos.y;
            particle->z = pos.z;
        } else {
            particle->update = (void (*)(void))BattleFx_UpdateOrbitingParticleFade;
        }
    }
}

struct OrbitingParticleChild {
    u8 reserved00[35];
    u8 flags;
};


void BattleFx_StartOrbitingParticles(void)
{
    struct BattleFxScene *state = Ram_HeapSlots->effectWork;
    struct OrbitingParticleChild *child = state->child;

    if (child != 0) {
        if (state->childMode != 0) {
            state->enabled = 1;
        }
        child->flags |= 2;
        BattleFx_RunOrbitingParticles();
    }
}

struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);

// Sixteen particles, one every three frames, orbit points scattered 2 to 6
// pixels around the effect; the target then flashes eight times, takes the
// orbiting particles' main update and runs its pending event.
void BattleFx_RunOrbitingParticles(void)
{
    struct OrbitingParticleVector pos;
    struct OrbitingParticleVector *p;
    struct BattleFxScene *scene;
    struct ObjectRuntime *target;
    struct ObjectRuntime *particle;
    struct PendingEvent *ev;
    s32 cnt;

    scene = Ram_HeapSlots->effectWork;
    target = scene->child;
    BattleEffect_InitializeSharedScene();
    Audio_PlayCue(115);

    p = &pos;
    cnt = 15;
    do {
        particle = Object_Spawn(681, 0, 0, 0);
        if (particle != NULL) {
            u32 scale;
            s32 dist;

            scale = (Random16() >> 1) + 0x8000;
            particle->scaleY = scale;
            particle->scaleX = scale;
            if ((Random16() & 1) != 0)
                particle->update = (void (*)(void))BattleFx_UpdateOrbitingParticleLeft;
            else
                particle->update = (void (*)(void))BattleFx_UpdateOrbitingParticleRight;

            particle->angle = Random16();
            particle->action = 60;
            particle->unk66 = Random16();
            Animation_ApplyChildValuesFar((struct DispatchObject *)particle, 9);

            p->x = scene->x;
            p->y = scene->y;
            p->z = scene->z;
            dist = (Random16() << 2) + (2 << 16);
            Vector_AddPolarOffset(dist, Random16(), p);
            particle->targetX = p->x;
            particle->targetY = p->y;
            particle->targetZ = p->z;
        }

        WaitFrames(3);
        cnt--;
    } while (cnt >= 0);

    WaitFrames(10);
    Audio_PlayCue(115);
    WaitFrames(50);

    if (target != NULL && scene->enabled == 0) {
        Audio_PlayCue(212);

        cnt = 7;
        do {
            Animation_ApplyChildValuesFar((struct DispatchObject *)target, 7);
            WaitFrames(1);
            Animation_ApplyChildValuesFar((struct DispatchObject *)target, 0);
            WaitFrames(4);
            cnt--;
        } while (cnt >= 0);

        if (scene->childOption == 0) {
            Audio_PlayCue(220);
            Object_SetMode(target, 2);
        }

        target->update = (void (*)(void))BattleFx_UpdateOrbitingParticleMain;
        ev = Func_080ce458(0x50000005, 6, Func_080ce31c(6));
        if (ev != NULL)
            EventRuntime_RunPendingEvent(ev, scene->firstObjectId, scene->secondObjectId);
        WaitFrames(20);
    }

    BattleFx_PrepareBufferInterpolation();
}
