#include "types.h"

#define SceneData_StoreRecord1Field12 Func_020003dc

typedef struct {
    u8 filler0[12];
    s32 unk12;
} T;

extern s32 Data_02009810;
extern T *Func_020017b8(s32);

s32 SceneData_StoreRecord1Field12(void)
{
    s32 *p;
    T *rec;

    p = &Data_02009810;
    rec = Func_020017b8(1);
    *p = rec->unk12;
    return 0;
}
