#include "TYPES.H"
#include "SCENE.H"
#include "RAM_BUFFER.H"
extern u8 Data_03001e80[];
extern u8 Data_03001ae8[];
void BattleCamera_SetRange(s32, s32, s32, s32, s32);

s32 Battle_FindTaggedSlotByValue(u32 value);
void BattlePres_AdjustCameraByShoulderKeysAlt(void);
void Battle_ReservedNoOp9B2C(void);

/* battle/get_tagged_slot_value.c */
s16 Battle_GetTaggedSlotValue(s32 arg0)
{
    u8 *base = (u8 *)Ram_HeapSlots->battle_work;
    s32 offset;

    if ((arg0 & 0x80) != 0) {
        offset = (arg0 & 0xF) * 2 + 0x64;
        base += 2;
    } else {
        offset = (arg0 & 0xF) * 2 + 0x58;
    }
    return *(s16 *)(base + offset);
}
