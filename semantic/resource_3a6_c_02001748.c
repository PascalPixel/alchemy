typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/*
 * resource_3a6 owner at 0x02001748, complete 40-byte span through its one-word
 * pool: play cue 123, then dispatch the signed scene value at workspace +364.
 */

extern u8 *Data_03001ebc;
extern void Func_0808a248(s32 value);
extern void Func_080f9010(s32 cue);

void Func_02001748(void)
{
    s32 value = *(s16 *)(Data_03001ebc + 364);

    Func_080f9010(123);
    Func_0808a248(value);
}
