#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
extern u8 Data_03001e98[];

s32 Resource_ResetEntry(u32 index);
void Resource_ResetPendingTransfer(void)
{
    u32 offset = 0x30c;
    u8 *work = *(u8 **)((u32)&Data_03001e98) + offset;

    if (*(u16 *)(work + 0x0a) != 0) {
        Resource_ResetEntry(*(u16 *)(work + 0x0c));
        *(u16 *)(work + 0x0a) = offset = 0;
    }
}
