#include "types.h"

/* Sixteen call sites reaching eleven distinct import veneers. Byte equality
 * needs one symbol per slot, so repeated imports appear under several names;
 * the semantic reconstruction records which slots share a main-image target. */
extern void Func_02001c10(void);
extern void Func_02001c0e(s32);
extern void Func_02001d32(s32);
extern void Func_02001d3a(s32);
extern void Func_02001c00(s32);
extern void Func_02001c06(s32);
extern void Func_02001c3c(s32);
extern void Func_02001c7a(s32, s32, s32);
extern void Func_02001cc2(s32, s32);
extern void Func_02001ccc(s32, s32, s32);
extern void Func_02001cd2(s32, s32, s32);
extern void Func_02001c80(s32);
extern void Func_02001d6e(s32);
extern void Func_02001c6c(s32);
extern void Func_02001c72(s32);
extern void Func_02001cae(void);

void Func_02000184(void)
{
    u8 *workspace = *(u8 **)0x03001EBC;

    Func_02001c10();
    Func_02001c0e(10);
    if (*(s16 *)(workspace + 364) == 4) {
        Func_02001d32(188);
    } else {
        Func_02001d3a(158);
    }
    Func_02001c00(1);
    Func_02001c06(2);
    Func_02001c3c(10);
    Func_02001c7a(0, 0x8000, 0x4000);
    Func_02001cc2(0, 2);
    if (*(s16 *)(workspace + 364) == 4) {
        Func_02001ccc(0, 0, -16);
    } else {
        Func_02001cd2(0, 3, -16);
    }
    Func_02001c80(16);
    Func_02001d6e(*(s16 *)(workspace + 364));
    Func_02001c6c(1);
    Func_02001c72(2);
    Func_02001cae();
}
