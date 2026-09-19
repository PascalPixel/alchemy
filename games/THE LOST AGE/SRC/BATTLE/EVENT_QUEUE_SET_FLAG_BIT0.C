#include "TYPES.H"
struct BattleEventFlagWork {
    u8 unknown_00[0x16c];
    u32 flags;
};
void Battle_SetRuntimeFlagBit0(struct BattleEventFlagWork *runtime, u32 operand)
{
    runtime->flags |= 1;
}
