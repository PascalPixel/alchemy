#include "scene.h"
#include "effect_runtime.h"
#include "global_cells.h"
#include "types.h"
#include "battle_effect_runtime.h"
#include "object_lookup.h"
#include "fixed_math.h"
#include "object_effect.h"

/* battle/effects/play_cue_and_start_emitter_on_target.c */
void WaitFrames(s32);
extern void Object_SetMode(struct ParticleEffectObject *, s32);

s32 Object_GetById(u32);
s32 Audio_PlayCue(s32);

s32 BattleFx_PlayCueAndStartEmitterOnTarget(s32 effect, s32 target, s32 mode)
{
    s32 object;
    s32 result;

    object = Object_GetById(target);
    result = 0;
    if (object != 0) {
        Audio_PlayCue(0x7C);
        Object_SetMode(object, 4);
        WaitFrames(0xC);
        result = Battle_Apply(effect, mode);
    }
    return result;
}
