#include "TYPES.H"
#include "SCENE.H"
#include "FIXED_MATH.H"
#include "RUNTIME_INTERFACES.H"
extern struct SaveWorkspace *gSaveWorkspace;
extern u8 Flash_Handler0[];
s32 Math_ModU(s32, s32);
extern u8 Data_03001f1c[];

/* save/state/select_write_slot.c */
u32 Random16(void);

s32 SaveState_CompareBytes(u8 *left, u8 *right, s32 count)
{
    s32 difference = 0;

    while (count != 0) {
        difference = *left - *right;
        if (difference != 0)
            break;
        count--;
        left++;
        right++;
    }
    return difference;
}
