#include "types.h"

extern void Func_02004000();
extern s32 Func_02001bcc();
extern void Func_02001bdc();
extern void Func_02002344();
extern void Func_02004122();
extern void Func_02004036();

void Func_020011c4(void) {

    Func_02004000();
    Func_02001bcc(0x8f0000, 0, 0x1220000, 223);
    Func_02001bdc(0x790000, 0, 0x11e0000, 253);
    Func_02002344();
    Func_02004122(13);
    Func_02004036();
}
