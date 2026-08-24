#include "types.h"

/* Shared 22-byte head leaf proved identical for this overlay family. */
struct EffectRec {
    u8 pad[9];
    u8 flags_lo : 2;
    u8 mode : 2;
    u8 flags_hi : 4;
};

struct EffectWork {
    u8 pad[80];
    struct EffectRec *rec;
};

#define SetEffectMode Func_02000030

void SetEffectMode(struct EffectWork *work, s32 mode)
{
    work->rec->mode = mode;
}
