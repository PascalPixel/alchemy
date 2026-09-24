#include "TYPES.H"
#include "SCENE.H"

extern s16 gRom[];
extern s16 gRom2[];
s32 GameFlag_TestFar(s32);

s32 Party_LookupCharacterValueByFlag32(u32 index)
{
    if (index > 8) {
        return 0;
    }
    if (GameFlag_TestFar(32) == 0) {
        return gRom[index];
    }
    return gRom2[index];
}
