typedef unsigned char u8; typedef unsigned short u16; typedef signed short s16; typedef signed int s32;
extern u8 *Data_03001ebc; extern void Func_0808a018(void); extern u8 *Func_0808a080(s32);
extern void Func_080f9010(s32); extern void Func_08009178(s32,u16,u16);
extern void Func_0808a090(s32,s32,s32); extern void Func_0808a100(s32,s32);
extern void Func_0808a0d8(s32,s32,s32); extern void Func_0808a010(s32);
extern void Func_0808a248(s32); extern void Func_0808a368(void); extern void Func_0808a370(void); extern void Func_0808a020(void);
void Func_02000b6c(void)
{
    u8 *workspace = *(u8 **)Data_03001ebc;
    s32 slot;
    s32 index;
    const u8 *entry;

    Func_0808a018();
    for (slot = 8; slot <= 65; slot++) {
        u8 *actor = Func_0808a080(slot);
        if (actor != 0)
            actor[0x55] = 0;
    }
    Func_080f9010(158);
    index = *(s16 *)(workspace + 364) - 4;
    entry = (const u8 *)0x0200a50c + index * 8;
    Func_08009178(*(s32 *)entry, *(u16 *)(entry + 4), *(u16 *)(entry + 6));
    Func_0808a090(0, 0x8000, 0x4000);
    Func_0808a080(0)[0x55] = 0;
    Func_0808a100(0, 2);
    if (index != 6) {
        Func_0808a0d8(0, 2, -8);
        Func_0808a010(10);
    }
    Func_0808a248(*(s16 *)(workspace + 364));
    Func_0808a368();
    Func_0808a370();
    Func_0808a020();
}
