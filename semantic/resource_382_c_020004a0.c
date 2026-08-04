typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

extern s32 Func_080770c0(s32); extern void Func_0808a248(s32); extern void Func_0808a018(void);
extern u8 *Func_0808a080(s32); extern void Func_0808a0f0(s32,s32,s32); extern void Func_0808a090(s32,s32,s32);
extern void Func_0808a0d0(s32,s32,s32); extern void Func_0808a208(s32,s32); extern void Func_0808a210(s32,s32,s32,s32);
extern void Func_0808a218(void); extern void Func_0808a150(s32,s32,s32); extern void Func_0808a010(s32);
extern void Func_0808a170(s32); extern void Func_0808a188(s32,s32,s32); extern void Func_0808a110(s32,s32);
extern s32 Func_02000450(void); extern void Func_0808a180(s32,s32); extern void Func_02000464(void);
extern void Func_080000c0(s32); extern void Func_0808a058(s32); extern void Func_0808a368(void);
extern void Func_0808a370(void); extern void Func_0808a020(void);

void Func_020004a0(void)
{
    u8 *workspace = *(u8 **)0x03001ebc;
    s32 state = *(s16 *)(workspace + 364);
    u8 *leader;

    if (Func_080770c0(0x855) != 0 || Func_080770c0(0x856) == 0) {
        Func_0808a248(state - 19);
        return;
    }

    Func_0808a018();
    leader = Func_0808a080(0);
    if (leader != 0)
        Func_0808a0f0(2, *(s32 *)(leader + 8), *(s32 *)(leader + 16));
    Func_0808a090(2, 0xcccc, 0x6666);

    if (state == 20) {
        Func_0808a0d0(2, 400, 448);
    } else {
        Func_0808a208(0xcccc, 0x1999);
        Func_0808a210(0x00e00000, -1, 0x00a20000, 1);
        Func_0808a0d0(2, 224, 162);
        Func_0808a218();
    }

    Func_0808a150(0, 2, 0);
    Func_0808a010(20);
    Func_0808a170(0x1327);
    Func_0808a188(0x9002, 0, 20);
    Func_0808a110(0, 3);
    if (Func_02000450() != 0) {
        Func_0808a170(0x132a);
        Func_0808a180(2, 0);
        Func_02000464();
        Func_080000c0(20);
    }
    Func_0808a058(2);
    Func_0808a248(state - 19);
    Func_0808a368();
    Func_0808a370();
    Func_0808a020();
}
