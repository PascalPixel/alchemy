#include "types.h"

struct EffectDefinition {
    u8 padding_00[2];
    u8 placement;
};

extern u8 Value_00000182;
extern u8 Value_00000200;

struct EffectDefinition *Func_08077018(s32 effectId);
void Func_08015080(s32 effectId, s32 ownerId, s32 layer, s32 position);

void Func_080a9aec(s32 ownerId, void *slotData)
{
    s32 effectBase = (s32)&Value_00000182;
    s32 present;
    s32 effectId;
    s32 index;
    u16 *effectSlots;
    u16 slot;
    u8 placement;

    effectSlots = slotData;
    index = 14;
    do
    {
        slot = *effectSlots;
        present = slot & (s32)&Value_00000200;
        effectSlots++;
        if (present != 0)
        {
            effectId = slot & 0x1FF;
            placement = Func_08077018(effectId)->placement;
            switch (placement)
            {
            case 1:
                Func_08015080(effectId + effectBase, ownerId, 8, 8);
                break;
            case 2:
                Func_08015080(effectId + effectBase, ownerId, 8, 56);
                break;
            case 3:
                Func_08015080(effectId + effectBase, ownerId, 8, 40);
                break;
            case 4:
                Func_08015080(effectId + effectBase, ownerId, 8, 24);
                break;
            }
        }
        index--;
    } while (index >= 0);
}
