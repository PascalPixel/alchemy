typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/*
 * resource_3bc long scripted-scene owner at 0x02001474, complete 1,432-byte
 * span including its branch-skipped mid-function pool and final message word.
 * The literal call sequence is intentionally left visible: it is the scene
 * script, and each site is independently audited against the overlay bytes.
 */

extern void Func_080000c0();
extern void Func_080000d0();
extern void Func_080000d8();
extern void Func_080091e0();
extern void Func_08009208();
extern void Func_08077260();
extern void Func_0808a010();
extern void Func_0808a018();
extern void Func_0808a020();
extern u8 *Func_0808a080();
extern void Func_0808a088();
extern void Func_0808a090();
extern void Func_0808a098();
extern void Func_0808a0c8();
extern void Func_0808a0d0();
extern void Func_0808a0e8();
extern void Func_0808a0f0();
extern void Func_0808a100();
extern void Func_0808a110();
extern void Func_0808a130();
extern void Func_0808a138();
extern void Func_0808a170();
extern void Func_0808a180();
extern void Func_0808a1b8();
extern void Func_0808a1e8();
extern void Func_0808a200();
extern void Func_0808a330();
extern void Func_0808a348();
extern void Func_0808a360();
extern void Func_0808a368();
extern void Func_0808a370();
extern void Func_0808a588();
extern void Func_080f9010();

void Func_02001474(s32 direction)
{
    u8 *record;
    u8 *workspace;

    Func_0808a088(0x27);
    Func_0808a088(0x28);
    Func_08077260(1);
    Func_080f9010(0x11);
    Func_0808a018();
    Func_0808a0f0(8, 0x06080000, 0x00c00000);
    if (direction < 0)
        Func_0808a100(8, 0x0a);
    else
        Func_0808a100(8, 8);

    Func_0808a098(8, 0x0200d668);
    Func_0808a0f0(0, 0x05e00000, 0x00c00000);
    record = Func_0808a080(0);
    *(s16 *)(record + 6) = 0;
    Func_0808a098(0, 0x0200d738);
    Func_0808a100(0, 0x23);
    Func_0808a090(1, 0x10000, 0x8000);
    Func_0808a090(2, 0x10000, 0x8000);
    Func_0808a090(3, 0x10000, 0x8000);
    Func_0808a0f0(1, 0x05b80000, 0x00b80000);
    Func_0808a0f0(2, 0x05b80000, 0x00c80000);
    Func_0808a0f0(3, 0x05a80000, 0x00c00000);
    record = Func_0808a080(1);
    *(s16 *)(record + 6) = 0;
    record = Func_0808a080(2);
    *(s16 *)(record + 6) = 0;
    record = Func_0808a080(3);
    *(s16 *)(record + 6) = 0;

    Func_080000c0(1);
    Func_0808a200(0, 0);
    workspace = *(u8 **)0x03001ebc;
    *(s32 *)(workspace + 0x1c0) = 0x100;
    Func_0808a330(0x10001, 1);
    Func_0808a360();
    Func_0808a370();
    Func_0808a170(0x20f1);
    Func_0808a010(0x3c);

    Func_0808a098(0, 0x0200d950);
    record = Func_0808a080(0);
    *(s32 *)(record + 0x18) = 0x10000;
    record = Func_0808a080(0);
    *(s32 *)(record + 0x1c) = 0x10000;
    Func_0808a110(0, 0x24);
    record = Func_0808a080(0);
    *(s32 *)(record + 8) += 0x30000;
    Func_0808a010(0x0a);
    Func_080091e0(Func_0808a080(0), 0);
    Func_0808a010(0x14);

    Func_0808a098(0, 0x0200d808);
    Func_0808a180(1, 0);
    Func_0808a010(0x14);
    Func_0808a0d0(1, 0x5e0, 0xb0);
    Func_0808a1b8(1, 0x4000, 0x0a);
    Func_0808a1e8(1, 0x100, 0x14);
    Func_0808a180(2, 0);
    Func_0808a588(1, 2);
    Func_0808a010(0x1e);
    Func_0808a0d0(2, 0x5d0, 0xb0);
    Func_0808a0c8(1, 0x5f0, 0xb8);
    Func_0808a0d0(2, 0x5e0, 0xb0);
    Func_0808a100(1, 1);
    Func_0808a1b8(1, 0x6000, 0);
    Func_0808a1b8(2, 0x4000, 0x0a);
    Func_0808a138(2, 2);
    Func_0808a010(0x0a);
    Func_0808a588(2, 1);
    Func_0808a010(0x1e);
    Func_0808a110(1, 4);
    Func_0808a010(0x1e);
    Func_0808a180(3, 0);
    Func_0808a588(1, 3);
    Func_0808a588(2, 3);
    Func_0808a0d0(3, 0x5d0, 0xb8);
    Func_0808a588(2, 0);
    Func_0808a588(1, 0);
    Func_0808a1e8(1, 0x102, 0x3c);
    Func_0808a588(2, 1);
    Func_0808a588(1, 2);
    Func_0808a010(0x28);
    Func_0808a138(3, 2);
    Func_0808a010(0x0a);
    Func_0808a588(2, 3);
    Func_0808a588(1, 3);
    Func_0808a010(0x14);
    Func_0808a180(3, 0);

    Func_0808a098(0, 0x0200d8ac);
    Func_0808a138(1, 2);
    Func_0808a588(1, 0);
    Func_0808a010(0x14);
    Func_0808a180(1, 0);
    Func_0808a138(2, 2);
    Func_0808a588(2, 0);
    Func_0808a010(0x14);
    Func_0808a180(2, 0);
    Func_0808a138(3, 2);
    Func_0808a588(3, 0);
    Func_0808a010(0x14);
    Func_0808a180(3, 0);
    Func_0808a098(0, 0x0200d950);
    Func_0808a010(0x3c);

    *(s32 *)0x0200db80 = 9;
    Func_080000d0(0x020093c1, 0xc80);
    Func_0808a010(5);
    Func_080000d8(0x020093c1);
    Func_0808a010(0x37);
    Func_0808a1e8(1, 0x101, 0x3c);
    Func_080000d0(0x020093c1, 0xc80);
    Func_0808a010(0x14);
    Func_080000d8(0x020093c1);
    Func_0808a010(0x28);
    Func_0808a1e8(2, 0x101, 0x3c);

    *(s32 *)0x0200db80 = 9;
    Func_080000d0(0x020093c1, 0xc80);
    Func_0808a010(0x23);
    Func_080000d8(0x020093c1);
    Func_0808a010(0x19);
    Func_0808a1e8(3, 0x102, 0x3c);

    *(s32 *)0x0200db80 = 9;
    Func_080000d0(0x020093c1, 0xc80);
    Func_0808a010(0x23);
    Func_080000d8(0x020093c1);
    Func_0808a010(0x19);
    Func_0808a1e8(2, 0x102, 0x3c);
    Func_0808a588(3, 2);
    Func_0808a588(2, 3);
    Func_0808a010(0x3c);
    Func_0808a588(3, 0);
    Func_0808a588(2, 0);

    *(s32 *)0x0200db80 = 9;
    Func_080000d0(0x020093c1, 0xc80);
    Func_0808a010(0x23);
    Func_080000d8(0x020093c1);
    Func_0808a010(0x19);
    Func_0808a1e8(3, 0x108, 0x3c);
    Func_0808a130(1, 3);
    Func_0808a130(2, 3);
    Func_0808a138(3, 3);
    Func_0808a588(3, 2);
    Func_0808a588(1, 2);

    *(s32 *)0x0200db80 = 9;
    Func_080000d0(0x020093c1, 0xc80);
    Func_0808a100(1, 3);
    Func_0808a100(2, 3);
    Func_0808a100(3, 3);
    Func_0808a010(0x3c);
    Func_0808a0c8(3, 0x5b8, 0xc8);
    Func_0808a010(5);
    Func_0808a0c8(2, 0x558, 0xb8);
    Func_0808a010(3);
    Func_0808a0d0(1, 0x5e8, 0xb8);
    Func_0808a0c8(1, 0x558, 0xb8);
    Func_0808a0e8(3);
    Func_0808a100(3, 1);
    Func_0808a588(3, 0);
    Func_0808a010(0x3c);
    Func_0808a0d0(3, 0x598, 0xc8);
    Func_0808a0c8(3, 0x558, 0xb8);
    Func_0808a368();
    Func_0808a370();
    Func_0808a010(0x1e);
    Func_0808a0f0(1, 0x05e80000, 0x00b00000);
    Func_0808a0f0(2, 0x05b80000, 0x00c00000);
    Func_0808a0f0(3, 0x06180000, 0x00c80000);
    Func_08009208();
    Func_0808a330(0x10000, 2);
    Func_0808a348(1);
    Func_0808a170(0x214c);
    Func_0808a180(1, 0);
    Func_0808a180(2, 0);
    Func_0808a180(3, 0);
    Func_0808a010(0x3c);
    Func_0808a020();
}
