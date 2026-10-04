/* Ported from ☀️ games/THE BROKEN SEAL/SRC/BATTLE/EFFECT/EFFECT3.C. */
#include "RUNTIME_MEM.H"
#include "BATTLE_PARTY.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "SYSTEM.H"
#include "IWRAM_CALL.H"
#include "DMA.H"
#include "RESOURCE.H"
#include "RESOURCE_IDS.H"

void Graphics_AdvancePaletteCycle(void)
{
    s32 _c0 = ((u32)&Data_03001e74);
    s8 *table;
    u16 index;
    s32 next;
    struct State080c1084 *state;

    state = *(struct State080c1084 **)_c0;
    if ((state != NULL) && (state->field_650 != 0)) {
        FIELD_AT_OFFSET((void *)0x04000050, s16 *, 0) = 0x3F90;
        FIELD_AT_OFFSET((void *)0x04000050, s16 *, 2) = 0x10;
        table = Data_080c5c10;
        *(s16 *)0x04000054 = table[state->index];
        index = state->index;
        next = (index + 1) & 0xF;
        if ((u32)index > 0xEU) {
            next |= 0x10;
        }
        state->index = next;
    }
}
