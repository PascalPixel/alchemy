typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

extern void Func_0808a018(void);
extern void Func_0808a210(s32, s32, s32, s32);
extern void Func_080000c0(s32);
extern void Func_0808a100(s32, s32);
extern void Func_0808a0f0(s32, s32, s32);
extern u8 *Func_0808a080(s32);
extern void Func_0808a5c0(s32, s32);
extern void Func_0808a200(s32, s32);
extern void Func_0808a360(void);
extern void Func_0808a090(s32, s32, s32);
extern void Func_0808a0c0(s32, s32, s32);
extern void Func_0808a010(s32);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern void Func_0808a248(s32);

/* Move actor 8 through the complete staged scale-and-position approach. */
void Func_020032f0(void)
{
    u8 *actor;

    Func_0808a018();
    Func_0808a210(-1, -1, -1, 0);
    Func_080000c0(1);
    Func_0808a100(8, 2);
    Func_0808a0f0(8, 0x13080000, 0x03280000);
    actor = Func_0808a080(8);
    *(u16 *)(actor + 6) = 0xa000;
    Func_080000c0(1);
    Func_0808a5c0(0x13333, 1);
    Func_0808a0f0(0, 0, 0);
    Func_0808a200(8, 1);
    Func_080000c0(1);
    *(s32 *)(*(u8 **)0x03001ebc + 448) = 256;
    Func_0808a360();

    Func_0808a090(8, 0x6666, 0x3333);
    Func_0808a0c0(8, 0x12d80000, 0x2c8);
    Func_0808a0c0(8, 0x12a80000, 0x268);
    Func_0808a090(8, 0x4ccc, 0x2666);
    Func_0808a0c0(8, 0x12a80000, 0x1d8);
    Func_0808a090(8, 0x3333, 0x1999);
    Func_0808a0c0(8, 0x12980000, 0x1c8);
    Func_0808a090(8, 0x1999, 0x0ccc);
    Func_0808a0c0(8, 0x12980000, 0x1b8);
    Func_0808a100(8, 1);
    Func_0808a010(40);
    Func_0808a368();
    Func_0808a370();
    Func_0808a248(110);
}
