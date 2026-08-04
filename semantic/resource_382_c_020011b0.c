typedef unsigned char u8; typedef signed short s16; typedef signed int s32;
extern u8 *Func_0808a080(int); extern void Func_0808a090(int,int,int); extern void Func_080091e0(u8 *,int);
extern void Func_0808a0c0(int,int,int); extern void Func_0808a0f0(int,int,int); extern void Func_080000c0(int);
void Func_020011b0(int actor, int x, int z, int field40)
{
    u8 *record = Func_0808a080(actor); int frames;
    Func_0808a090(actor, 0x30000, 0x18000); *(s32 *)(record + 72) = 0x8000;
    *(s32 *)(record + 68) = 0; *(s32 *)(record + 40) = field40; Func_080091e0(record, 0);
    Func_0808a0c0(actor, x, z); Func_0808a0f0(actor, x << 16, z << 16);
    for (frames = 60; frames > 0; --frames) { Func_080000c0(1); if (*(s16 *)(record + 42) == 0) break; }
    Func_080091e0(record, 1); *(s32 *)(record + 72) = 0x20000;
}
