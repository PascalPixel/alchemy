#include "types.h"

struct Runtime_080a3e88 {
    u8 padding_000[0x20];
    s32 window;
    u8 padding_024[0x1a4];
    u16 entries[32];
    u8 padding_208[0x10];
    u8 entry_count;
};

extern struct Runtime_080a3e88 *Data_03001f2c;

void *Func_08077008(s32 kind);
u32 Func_080a3ddc(void *source, u16 *destination, s32 mode);
void Func_08015270(s32 window);
void Func_080a1cb0(s32 mode);
void Func_080a3e28(u16 *entries, s32 mode);
s32 Func_080a3d6c(s32 kind);
void Func_08015080(s32 message, s32 window, s32 x, s32 y);

void Func_080a3e88(s32 kind, s32 mode)
{
    struct Runtime_080a3e88 *runtime;
    void *source;
    u16 *entries;

    runtime = Data_03001f2c;
    source = Func_08077008(kind);
    entries = runtime->entries;
    runtime->entry_count = Func_080a3ddc(source, entries, 0);
    Func_08015270(runtime->window);
    Func_080a1cb0(mode);
    Func_080a3e28(entries, 0);
    if (Func_080a3d6c(kind) == 0)
        Func_08015080(0xad7, runtime->window, 8, 24);
}

void Func_080a3eec(void)
{
}
