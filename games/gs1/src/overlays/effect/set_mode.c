#include "types.h"

struct EffectRec {
    u8 pad[9];
    u8 f0 : 2;
    u8 f1 : 2;
};

struct EffectWork {
    u8 pad[0x50];
    struct EffectRec *rec;
};

#define SetEffectMode Func_02000030

void SetEffectMode(struct EffectWork *work, u32 mode)
{
    work->rec->f1 = mode;
}
