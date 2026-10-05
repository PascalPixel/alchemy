#include "TYPES.H"
#include "OBJDISP.H"
#include "SCENE.H"
#include "OBJECT_EFX.H"
#include "FIXED_MATH.H"
#include "SYSTEM.H"
#include "FRAME.H"
#include "OBJECT_DISPATCH.H"

struct OrbitingParticle;

/* ☀️'s main orbit step, which in ⚓️ returns 0 to its dispatcher. */
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

void BattleFx_UpdateOrbitingParticleFade(void *object)
{
  s32 primary_fade;
  u8 *object_bytes;
  if (object != ((void *) 0))
  {
    primary_fade = *((s32 *)(((u8 *)object) + 0x18));
    primary_fade = primary_fade + 0xFFFFF000;
    *((s32 *)((object_bytes = (u8 *)object) + 0x1C)) = (s32)((*((s32 *)(object_bytes + 0x1C))) + 0xFFFFF000);
    *((s32 *)(object_bytes + 0x18)) = primary_fade;
    if (primary_fade <= 0x1000)
    {
      ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)BattleFx_CommonParticleScript);
    }
  }
}

struct OrbitingParticleVector {
    s32 x;
    s32 y;
    s32 z;
};

void Vector_AddPolarOffset(s32 magnitude, s32 angle, struct OrbitingParticleVector *pos);

/* Counts the particle's life down, placing it on a shrinking orbit around its
   centre that turns one way; at zero it fades. */
void BattleFx_UpdateOrbitingParticleLeft(struct OrbitingParticle *particle)
{
    u8 *arg = (u8 *)particle;
    struct OrbitingParticleVector local;
    s16 life;
    s32 cnt;

    if (arg != 0) {
        cnt = *(u16 *)(arg + 100) - 1;
        *(u16 *)(arg + 100) = cnt;
        life = (s16)cnt;
        if (life != 0) {
            local.x = *(s32 *)(arg + 56);
            local.y = *(s32 *)(arg + 60);
            local.z = *(s32 *)(arg + 64);
            Vector_AddPolarOffset(life << 17,
                          *(s16 *)(arg + 102) + (life << 11),
                          &local);
            *(s32 *)(arg + 8) = local.x;
            *(s32 *)(arg + 12) = local.y;
            *(s32 *)(arg + 16) = local.z;
        } else {
            *(s32 *)(arg + 108) = (s32)BattleFx_UpdateOrbitingParticleFade;
        }
    }
}

/* The same orbit turning the other way. */
void BattleFx_UpdateOrbitingParticleRight(struct OrbitingParticle *particle)
{
    u8 *arg = (u8 *)particle;
    struct OrbitingParticleVector local;
    s16 battle_value;
    s32 raw_value;

    if (arg != 0) {
        raw_value = *(u16 *)(arg + 100) - 1;
        *(u16 *)(arg + 100) = raw_value;
        battle_value = (s16)raw_value;
        if (battle_value != 0) {
            local.x = *(s32 *)(arg + 56);
            local.y = *(s32 *)(arg + 60);
            local.z = *(s32 *)(arg + 64);
            Vector_AddPolarOffset(battle_value << 17,
                          *(s16 *)(arg + 102) - (battle_value << 11),
                          &local);
            *(s32 *)(arg + 8) = local.x;
            *(s32 *)(arg + 12) = local.y;
            *(s32 *)(arg + 16) = local.z;
        } else {
            *(s32 *)(arg + 108) = (s32)BattleFx_UpdateOrbitingParticleFade;
        }
    }
}
