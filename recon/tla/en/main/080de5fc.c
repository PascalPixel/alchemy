/* 2026-10-03: removed the unused competing ApplyChildValuesFar declaration.
   Still cannot compile: OrbitingParticleVector is incomplete and
   BattleFx_UpdateOrbitingParticleFade is undeclared. These prior blockers
   are unchanged; no new byte comparison is possible. */
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
