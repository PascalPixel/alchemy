#include "types.h"

#define SceneActor_MoveTo232_125AndFace4000 Func_02002820

typedef struct {
    u8 filler0[6];
    u16 unk6;
} Ent;

extern Ent *Func_02005d12(s32);
extern void Func_02005d6a(s32, s32, s32);
extern void Func_02005de8(s32, s32);

void SceneActor_MoveTo232_125AndFace4000(s32 no)
{
    Ent *rec;

    rec = Func_02005d12(no);
    Func_02005d6a(no, 0xe80000, 0x7d0000);
    rec->unk6 = 0x4000;
    Func_02005de8(no, 3);
}
