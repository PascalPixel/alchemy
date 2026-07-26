#include "types.h"

struct GlobalData {
    u8 filler[0x1F4];
    s32 selected_object;
};

extern struct GlobalData Data_02000240;

u8 *Func_08077080(s32);
s32 Func_0808ba1c(u32);
s32 Func_0808e4b4(s32, u16, void *);
void Func_08096fb0(s32, s32);
void Func_080970f8(s32, s32);
s32 Func_08096b28(void *, s32, s32);
void Func_08096af0(void);
void Func_08097174(void);
void Func_08097194(void);

s32 Func_0808e5d8(s32 value)
{
    s32 output;
    s32 index;
    s32 mode;
    u8 object;
    void *first;
    void *second;

    index = value & 0x3FF;
    mode = ((u32)value >> 10) & 0xF;
    object = Func_08077080(index)[0xC];
    Func_0808ba1c(Data_02000240.selected_object);
    first = (void *)Func_0808e4b4(0x30000005, object, &output);
    second = (void *)Func_0808e4b4(0x20000005, object, &output);
    Func_08096fb0(index, 0);
    Func_080970f8(Data_02000240.selected_object, output);
    Func_08096b28(first, mode, output);
    Func_08096af0();
    Func_08097174();
    Func_08096b28(second, mode, output);
    Func_08097194();
    return 0;
}
