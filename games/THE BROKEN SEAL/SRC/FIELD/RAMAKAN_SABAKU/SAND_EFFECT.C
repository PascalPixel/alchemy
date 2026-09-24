#include "TYPES.H"

void Engine_AudioPlayCue();
s32 Engine_RandomNext();
void Effect_Spawn();

struct EffectParams {
    u8 pad00[8];
    s32 scaleX;
    s32 scaleY;
    u8 pad10[18];
    u16 angle;
    u8 pad24[4];
};

/* Every eighth frame play the sand cue; every sixteenth spawn a sand effect
 * at the actor with a random heading. */
s32 RamakanSabaku_EmitSandEffect(u8 *actor)
{
    struct EffectParams params;
    s32 phase;

    if ((*(volatile u32 *)0x03001e40 & 7) == 0) {
        Engine_AudioPlayCue(118);
    }
    phase = *(volatile u32 *)0x03001e40 & 15;
    if (phase != 0) {
        return 0;
    }
    params.scaleX = 0xcccc;
    params.scaleY = 0xcccc;
    params.angle = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
    Effect_Spawn(*(s32 *)(actor + 8), *(s32 *)(actor + 12), *(s32 *)(actor + 16), 0, phase, phase, 0x00880001, &params);
    return 0;
}
