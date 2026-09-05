#include "types.h"

typedef struct {
    s16 filler0[50];
    s32 unk64;
} Struct_22a4;

typedef struct {
    s32 filler0[3];
    s32 unkC;
} Struct_22a4b;

extern Struct_22a4b *Func_0200715e(s32);

s32 SceneActor_SetHeightAboveLinkedRecord(Struct_22a4 *obj)
{
    Struct_22a4b *rec;

    rec = Func_0200715e(((s16 *)obj)[50]);
    ((s32 *)obj)[3] = rec->unkC + 0x100000;
    return 0;
}
