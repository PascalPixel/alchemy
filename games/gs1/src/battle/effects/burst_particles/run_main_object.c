#include "types.h"
#include "scene.h"
#include "abi/battle/effects/burst_particles/run_main_object.h"
#include "global_cells.h"


void Object_SetMode(s32, s32);

void WaitFrames(s32);
void Audio_PlayCue(s32);
void BattleFx_PrepareBufferInterpolation(void);

void BattleFx_RunBurstParticleMainObject(void)
{
    u8 *object;
    u8 *flags;
    u8 battle_value;

    object = FIELD_AT_OFFSET(*(void **)ADDR_03001F30, u8 **, 0x14);
    if (object != 0) {
        Battle_Run();
        Object_SetMode((s32)object, 2);
        object[0x59] = 0;
        Battle_Apply(object, 0);
        flags = object + 0x23;
        battle_value = 2;
        battle_value |= *flags;
        *flags = battle_value;
        WaitFrames(0xAU);
        Audio_PlayCue(0x7E);
        WaitFrames(0x28U);
        BattleFx_PrepareBufferInterpolation();
    }
}
