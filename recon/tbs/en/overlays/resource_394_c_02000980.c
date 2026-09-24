#include "DMA.H"



extern u8 Data_00000001[];
void Func_02000b9e(void);
void Func_0200148e(u16 *cells);
s32 Func_02001542(void *cells, s32 value);
void Func_020018d8(s32 amount);
void Func_02001a20(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_02001a38(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_02001a48(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
s32 Func_02001a66(s32 flag);
u8 *Func_02001ac4(s32 id);
u8 *Func_02001ace(s32 id);
void Func_02001aee(s32 id, s32 value);
s32 Func_02001af6(s32 flag);

/* NONMATCHING: 272 of 272 bytes, 25 halfword edits (2026-09-24). Remaining:
 * r8/sl swap for the two cursor pointers, the 0xc000 store after the pool
 * dump, and 0x204 derived from 0x1c0 instead of built alone. */
s32 Func_02000980(void)
{
    u8 *base;
    u8 *obj;

    base = (u8 *)0x02001000;
    *(u8 **)0x020092c4 = base;
    *(u8 **)0x020092c8 = base + 2;
    *(u8 **)0x020092c0 = base + 4;
    Func_02001a20(32, 0, 64, 32, 0, 64);
    Func_02001a38(0, 0, 32, 32, 0, 64);
    Func_02001a48(32, 0, 32, 32, 0, 32);
    if (Func_02001a66(0x109) == 0) {
        Dma_Set((const void *)0x0200911c, *(void **)0x020092c0, 0x84000012, (volatile u32 *)0x040000d4);
        **(u16 **)0x020092c4 = 0;
        {
            /* FAKEMATCH: the one is a HImode pool constant, whose short
             * pool range dumps the literal pool mid-function. */
            s32 one = (u16)(u32)Data_00000001;

            **(u16 **)0x020092c8 = one;
        }
    }
    Func_0200148e(*(u16 **)0x020092c0);
    Func_02001542((void *)0x0200911c, 255);
    Func_02000b9e();
    Func_02001aee(9, 0);
    Func_02001ac4(9)[85] = 0;
    obj = Func_02001ace(10);
    { s32 eight = 8; *(u16 *)(obj + 32) = eight; }
    *(s32 *)(obj + 24) = 0xc000;
    *(s32 *)(obj + 28) = 0xc000;
    *(s32 *)(*(u8 **)0x03001ebc + 0x1c0) = 0x204;
    if (Func_02001af6(0x845) == 0)
        Func_020018d8(4);
    return 0;
}
