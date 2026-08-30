#include "battle_effect_runtime.h"
#include "types.h"

void Func_080090d0(void *object);
s32 ObjectTable_FindLastActiveId(void);
void Func_0808b3ec(s32 event_id, s32 state);

#define BattleEffect_ClearAllObjects Func_0808b98c

void BattleEffect_ClearAllObjects(void)
{
    struct BattleRuntime *runtime = Data_03001ebc;
    s32 offset = 0x34;
    s32 remaining = 57;
    s32 mask = -2;
    s32 cleared = 0;
    s32 event_id;

    do {
        void *object = *(void **)((u8 *)runtime + offset);

        if (object != 0) {
            u8 *visual;

            *(u8 *)((u8 *)object + 84) = 1;
            visual = *(u8 **)((u8 *)object + 80);
            visual[29] &= mask;
            Func_080090d0(object);
            *(s32 *)((u8 *)runtime + offset) = cleared;
        }
        remaining--;
        offset += 4;
    } while (remaining >= 0);

    event_id = *(s32 *)((u8 *)runtime + 4);
    *(s32 *)((u8 *)runtime + 4) = cleared;
    *(s32 *)((u8 *)runtime + 8) = cleared;
    *(s32 *)((u8 *)runtime + 12) = cleared;
    if (event_id != 0) {
        Func_0808b3ec(event_id, ObjectTable_FindLastActiveId());
    }
}
