#include "TYPES.H"
#include "CONFIGURED_EFFECT_SPAWN.H"

#define Lifted_02004610 Func_02004610


/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02004814(s32, s32, s32, s32, s32, s32, s32, const struct ConfiguredEffectOptions *);
s32 Func_02009442(void);
s32 Func_02009456(void);
s32 Func_0200946a(void);
void Func_02009480(s32, s32);
s32 Func_02009488(void);
void Func_020094b2(s32);
void Func_020094c4(void);
void Func_020094dc(s32, s32);
s32 Func_020094e2(s32);
void Func_020094e4(s32);
void Func_02009562(s32, s32, s32, s32, s32, s32);
void Func_02009578(s32, s32);
void Func_02009586(s32, s32, s32, s32, s32, s32);
void Func_020095e6(s32, s32);
void Func_02009600(s32, s32, s32, s32);
void Func_02009608(s32);
void Func_02009612_setup(void);
void Func_02009626(void);
void Func_02009632(void);
void Func_02009636(s32);
void Func_0200965a(s32);
void Func_020096fa(void);
void Func_02009704(s32, s32, s32, s32);
void Func_02009736(s32);

void Lifted_02004610(void)
{
    s32 z;
    struct ConfiguredEffectOptions params;
    s32 record;
    s32 value;
    s32 retry;
    s32 row;
    s32 base;
    s32 i;

    *(s32 *)((*(u8 **)Data_03001ebc + 0x1c0)) = 0x202;
    Func_020094c4();
    record = Func_020094e2(0);
    Func_02009480(record, 0);
    Func_02009578(0, 15);
    Func_02009636(170);
    Func_02009612_setup();
    Func_02009626();
    Func_020094e4(40);
    Func_0200965a(162);
    Func_020095e6(0x8000, 0x1000);
    Func_02009600(0x1b80000, -1, 0x1680000, 1);
    retry = 0;
    row = 0;
    z = 0;
    do {
        value = Func_02009442();
        params.accum18 = (((u32)(value << 1) >> 16) * 0x4ccc) + 0x17ffc;
        value = Func_02009456();
        params.accum1c = (((u32)(value << 1) >> 16) * 0x4ccc) + 0x17ffc;
        value = Func_0200946a();
        params.step = (((u32)(value << 12) >> 16) + 0xf800);
        L_020046c8:;
        i = 0;
        base = 0xc00000 + z;
        do {
            value = Func_02009488();
            Func_02004814((((u32)((value << 3) - value) >> 16) << 19) + 0x1a00000,
                0, base, 0, 0, 0, 0x880000, &params);
            i = i + 1;
            base = base + 0x40000;
        } while ((u32)i <= 3);
        Func_020094b2(3);
        if (row == 3) {
            if ((u32)retry <= 2) {
                retry = retry + 1;
                goto L_020046c8;
            }
            if (retry == 3) {
                Func_020094dc(0x200c5f1, 0xc80);
            }
        }
        Func_02009562(53, row + 12, 26, row + 12, 3, 1);
        row = row + 1;
        z = z + 0x100000;
    } while ((u32)row <= 12);
    Func_02009586(81, 41, 89, 14, 9, 2);
    Func_020096fa();
    Func_02009704(-1, -1, -1, 0);
    Func_020094e4(60);
    Func_02009608(0x306);
    Func_02009736(19);
    Func_02009632();
}
