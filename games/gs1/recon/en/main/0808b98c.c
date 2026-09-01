#include "battle_effect_runtime.h"
#include "types.h"

void Func_080090d0(void *object);
s32 ObjectTable_FindLastActiveId(void);
void Func_0808b3ec(s32 event_id, s32 state);

#define BattleEffect_ClearAllObjects Func_0808b98c

void BattleEffect_ClearAllObjects(void)
{
    u8 *runtime = (u8 *)Data_03001ebc;
    s32 mask = -2;
    s32 cleared = 0;
    s32 event_id;
    s32 i;

    for (i = 0; i < 58; i++) {
        void *object = *(void **)(runtime + (i * 4 + 0x34));

        if (object != 0) {
            u8 *visual;

            *(u8 *)((u8 *)object + 84) = 1;
            visual = *(u8 **)((u8 *)object + 80);
            visual[29] &= mask;
            Func_080090d0(object);
            *(void **)(runtime + (i * 4 + 0x34)) = cleared;
        }
    }

    event_id = *(s32 *)(runtime + 4);
    *(s32 *)(runtime + 4) = 0;
    *(s32 *)(runtime + 8) = 0;
    *(s32 *)(runtime + 12) = 0;
    if (event_id != 0) {
        Func_0808b3ec(event_id, ObjectTable_FindLastActiveId());
    }
}
