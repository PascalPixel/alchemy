#include "TYPES.H"

s32 GameFlag_TestFar(s32);

s32 Party_RemapCharacterIdByFlags(s32 arg0)
{
    s32 result;

    result = arg0;
    if (result <= 8) {
        if (GameFlag_TestFar(0x20) != 0) {
            if (result == 0) {
                result = 0x12;
            }
            if (result == 1) {
                result = 0x13;
            }
        } else if ((GameFlag_TestFar(0x21) != 0) && (result == 0)) {
            result = 0x11;
        }
    }
    return result;
}
