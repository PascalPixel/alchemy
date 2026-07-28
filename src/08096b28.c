#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_080072f0(s32, s32);
void Func_08015040(s32, s32);
s32 Func_080770c0(s32);
void Func_080916b0();
void Func_08091750();
void Func_08092b94(s32);
void Func_08092f84(s32, s32);

s32 Func_08096b28(void *arg0, s32 arg1, s32 arg2) {
    s32 resource;

    if (arg0 != NULL) {
        resource = M2C_FIELD(arg0, s32 *, 8);
        if (resource != 0) {
            if (resource < 0x10000) {
                Func_080916b0();
                Func_08092b94(M2C_FIELD(arg0, s32 *, 8));
                Func_08092f84(arg2, 0);
                Func_08091750();
            } else {
                Func_080072f0(arg1, arg2);
            }
        }
        if (Func_080770c0(0x142) != 0) {
            Func_080916b0();
            Func_08015040(0x927, 1);
            Func_08091750();
        }
    }
    return 0;
}
