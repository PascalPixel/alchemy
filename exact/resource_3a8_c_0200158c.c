#include "types.h"

extern s32 *Data_03001ee0;
extern s32 Func_02005168(s32);
extern void Func_0200518c(s32);

void Func_0200158c(void) {
    if (Func_02005168(512) != 0) {
        Data_03001ee0[6] = 0;
        Func_0200518c(512);
    }
}
