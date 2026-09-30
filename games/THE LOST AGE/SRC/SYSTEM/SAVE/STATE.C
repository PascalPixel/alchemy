#include "TYPES.H"
#include "SCENE.H"
#include "FIXED_MATH.H"
#include "RUNTIME_INTERFACES.H"

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
