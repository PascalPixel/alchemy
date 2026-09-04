#include "types.h"

#define SceneData_StoreRecord2Field12 Func_02000414

typedef struct {
    u8 filler0[12];
    s32 unk12;
} T;

extern s32 Data_02009818;
extern T *Func_020017f0(s32);

s32 SceneData_StoreRecord2Field12(void)
{
    s32 *d;
    T *p;

    d = &Data_02009818;
    p = Func_020017f0(2);
    *d = p->unk12;
    return 0;
}
