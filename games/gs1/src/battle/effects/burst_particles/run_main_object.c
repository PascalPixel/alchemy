#include "types.h"
#include "global_cells.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_08098698(void);
void Object_SetMode(s32, s32);
void Func_080091e0(void *, s32);
void WaitFrames(s32);
void Audio_PlayCue(s32);
void BattleEffect_PrepareBufferInterpolation(void);

void RunBurstParticleMainObject(void) {
    u8 *object;
    u8 *flags;
    u8 battle_value;

    object = FIELD_AT_OFFSET(*(void **)ADDR_03001F30, u8 **, 0x14);
    if (object != 0) {
        Func_08098698();
        Object_SetMode((s32)object, 2);
        object[0x59] = 0;
        Func_080091e0(object, 0);
        flags = object + 0x23;
        battle_value = 2;
        battle_value |= *flags;
        *flags = battle_value;
        WaitFrames(0xAU);
        Audio_PlayCue(0x7E);
        WaitFrames(0x28U);
        BattleEffect_PrepareBufferInterpolation();
    }
}
