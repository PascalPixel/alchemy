typedef unsigned char u8; typedef signed short s16; typedef signed int s32;
extern s32 Func_080770c0(s32); extern void Func_080770c8(s32); extern void Func_0808a018(void);
extern u8 *Func_0808a080(s32); extern void Func_0808a148(s32,s32,s32); extern void Func_0808a130(s32,s32);
extern void Func_0808a010(s32); extern void Func_0808a1e8(s32,s32,s32); extern void Func_0808a170(s32);
extern void Func_0808a180(s32,s32); extern void Func_0808a090(s32,s32,s32); extern void Func_0808a100(s32,s32);
extern void Func_0808a0c8(s32,s32,s32); extern void Func_0808a0e8(s32); extern void Func_0808a020(void);
extern void Func_080000d0(u8 *,s32);
void Func_02000ff0(void)
{
    u8 *leader;
    if (Func_080770c0(0x244) != 0)
        return;
    Func_080770c8(0x244);
    Func_0808a018();
    leader = Func_0808a080(0);
    Func_0808a148(8, 0, 0);
    Func_0808a148(9, 0, 0);
    Func_0808a130(8, 1);
    Func_0808a130(9, 1);
    Func_0808a010(20);
    Func_0808a1e8(8, 0x102, 60);
    Func_0808a170(0x2409);
    Func_0808a180(8, 0);
    Func_0808a090(0, 0x20000, 0x10000);
    Func_0808a090(8, 0x20000, 0x10000);
    Func_0808a090(9, 0x20000, 0x10000);
    Func_0808a100(9, 4);
    Func_0808a010(35);
    Func_0808a170(0x240a);
    Func_0808a180(9, 0);
    Func_0808a1e8(8, 0x103, 30);
    Func_0808a170(0x240b);
    Func_0808a180(8, 0);
    Func_0808a100(9, 3);
    Func_0808a010(25);
    Func_0808a170(0x240c);
    Func_0808a180(9, 0);

    Func_0808a0c8(8, *(s16 *)(leader + 10) - 1, *(s16 *)(leader + 18));
    Func_0808a0e8(8);
    Func_0808a0c8(0, 160, 216);
    Func_0808a0c8(8, 152, 200);
    Func_0808a0c8(9, 168, 200);
    Func_0808a0e8(8);
    Func_0808a0e8(9);
    Func_0808a0e8(0);
    Func_0808a148(8, 0, 0);
    Func_0808a148(9, 0, 0);
    Func_0808a010(12);
    Func_0808a0c8(0, 160, 272);
    Func_0808a0c8(8, 152, 256);
    Func_0808a0c8(9, 168, 256);
    Func_0808a0e8(8);
    Func_0808a0e8(9);
    Func_0808a0e8(0);
    Func_0808a020();
    Func_080000d0((u8 *)0x02009241, 3200);
}
