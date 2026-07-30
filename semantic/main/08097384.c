#include "types.h"
#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void Func_08097384(void) {
    s32 temp_r5;
    s32 var_r5;
    void *temp_r4;

    temp_r5 = M2C_FIELD((void *)0x03001EBC, s32 *, 0x14);
    temp_r4 = M2C_FIELD((void *)0x03001EBC, void **, 0);
    M2C_FIELD((void *)0x040000D4, s32 *, 0) = (s32) (temp_r5 + 0x1340);
    M2C_FIELD((void *)0x040000D4, void **, 4) = (void *) (temp_r4 + 0x776);
    M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x84000150;
    if (M2C_FIELD(temp_r4, s16 *, 0xCB8) == 0) {
        M2C_FIELD((void *)0x040000D4, s32 *, 0) = (s32) (temp_r5 + 0xE00);
        M2C_FIELD((void *)0x040000D4, void **, 4) = (void *) (temp_r4 + 0x236);
        M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x84000150;
    }
    M2C_FIELD((void *)0x040000D4, s32 *, 0) = (s32) (temp_r5 + 0xE00);
    M2C_FIELD((void *)0x040000D4, void **, 4) = (void *) (temp_r5 + 0x380);
    M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x840002A0;
    var_r5 = *(s32 *)0x03001E40 & 7;
    if (Func_080770c0(0x148) != 0) {
        var_r5 = 0;
    }
    if (Func_080770c0(0x149) != 0) {
        var_r5 = 1;
    }
    if (Func_080770c0(0x14A) != 0) {
        var_r5 = 2;
    }
    if (Func_080770c0(0x14B) != 0) {
        var_r5 = 3;
    }
    if (Func_080770c0(0x14C) != 0) {
        var_r5 = 4;
    }
    if (Func_080770c0(0x14D) != 0) {
        var_r5 = 5;
    }
    if (Func_080770c0(0x14E) != 0) {
        var_r5 = 6;
    }
    if (Func_080770c0(0x14F) != 0) {
        var_r5 = 7;
    }
    Func_08091200(M2C_FIELD((var_r5 * 4), s32 *, 0x080A0108), 1);
    Func_08091254(8);
}
