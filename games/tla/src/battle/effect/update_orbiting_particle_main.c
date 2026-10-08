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
#include "object_efx.h"
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
        scale = object->scale_x - 0x1000;
        object->scale_y -= 0x1000;
        object->scale_x = scale;
        if (scale <= 0x1000)
            ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)BattleFx_CommonParticleScript);
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
            pos.x = particle->target_x;
            pos.y = particle->target_y;
            pos.z = particle->target_z;
            Vector_AddPolarOffset(life << 17, (s16)particle->unknown_66 + (life << 11), &pos);
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
            pos.x = particle->target_x;
            pos.y = particle->target_y;
            pos.z = particle->target_z;
            Vector_AddPolarOffset(life << 17, (s16)particle->unknown_66 - (life << 11), &pos);
            particle->x = pos.x;
            particle->y = pos.y;
            particle->z = pos.z;
        } else {
            particle->update = (void (*)(void))BattleFx_UpdateOrbitingParticleFade;
        }
    }
}

struct OrbitingParticleChild {
    u8 reserved_00[35];
    u8 flags;
};


void BattleFx_StartOrbitingParticles(void)
{
    struct BattleFxScene *state = Ram_HeapSlots->effect_work;
    struct OrbitingParticleChild *child = state->child;

    if (child != 0) {
        if (state->child_mode != 0) {
            state->enabled = 1;
        }
        child->flags |= 2;
        BattleFx_RunOrbitingParticles();
    }
}
