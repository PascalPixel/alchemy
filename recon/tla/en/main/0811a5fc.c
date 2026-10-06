#include "TYPES.H"
#include "SCENE.H"

s32 ResourceSlot_LoadFar(u32 slot, u32 *buffer, s32 number, u32 variant);
extern u8 Data_03001e74[];

s32 BattleMotion_ReleaseObjectSlotByValue(s32 value)
{
    u8 *base;
    s32 offset;
    s32 index;
    s16 item;

    base = *(u8 **)((u32)&Data_03001e74);
    index = 0;
    do {
        offset = index * 2 + 4;
        item = *(s16 *)(offset + (u32)base);
        if (item == value) {
            ResourceSlot_LoadFar(index, 0, 0, 0);
            *(s16 *)(offset + (u32)base) = 0;
        }
        index++;
    } while (index <= 5);
}
