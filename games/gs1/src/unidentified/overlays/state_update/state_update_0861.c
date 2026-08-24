#include "types.h"

extern u8 *Func_0200168c(s32);
extern void Func_02001642(s32, s32, s32, s32, s32, s32);
extern u8 *Func_020016ac(s32);
extern void Func_02001698(s32);

void Func_02000554(void)
{
    u8 *record = Func_0200168c(17);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 23;
    s32 sixth = 34;

    Func_02001642(26, 30, 1, 1, fifth, sixth);

    if (record != 0) {
        /* The record is reloaded with the same selector before this store. */
        Func_020016ac(17)[85] = 0;
        record[35] = 1;
    }

    Func_02001698(0x201);
}
