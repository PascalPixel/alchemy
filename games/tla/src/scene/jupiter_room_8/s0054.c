#include "types.h"

struct Probe {
    s32 unknown_00[6];
};

s32 Func_02001f60(void);
void Func_02002270(s32 value);
s32 Func_02000c44(struct Probe *probe);
void Func_02000ec8(struct Probe probe);

void Func_02000054(void)
{
    if (Func_02001f60())
        Func_02002270(13);
}

void Func_02000068(void)
{
    struct Probe probe;

    if (Func_02000c44(&probe))
        Func_02000ec8(probe);
}

void Func_02000090(void)
{
}
