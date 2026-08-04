#include "types.h"

extern u8 *Data_03001ebc;
extern void Func_0808a018(void);
extern void Func_08077268(void);
extern u8 *Func_0808a080(s32 id);
extern void Func_080091e0(u8 *record, s32 mode);
extern void Func_0808a210(s32 x, s32 y, s32 z, s32 mode);
extern void Func_080000c0(s32 frames);
extern void Func_08009128(void);
extern void Func_080f9010(s32 id);
extern void Func_080091f0(s32 x, s32 y, s32 z);
extern void Func_0808a360(void);
extern void Func_0808a370(void);
extern void Func_080091f8(void);
extern void Func_0808a010(s32 frames);
extern u8 *Func_0808a228(void);
extern void Func_0808a208(s32 from, s32 to);
extern void Func_0808a218(void);
extern void Func_0808a338(s32 value, s32 mode);
extern void Func_0808a330(s32 value, s32 mode);
extern void Func_0808a348(s32 frames);
extern void Func_0808a0f0(s32 id, s32 x, s32 y);
extern void Func_0808a100(s32 id, s32 mode);
extern void Func_0808a110(s32 id, s32 mode);
extern void Func_0808a1b8(s32 id, s32 value, s32 frames);
extern void Func_0808a130(s32 id, s32 mode);
extern void Func_0808a090(s32 id, s32 from, s32 to);
extern void Func_0808a0b8(s32 id, s32 x, s32 z);
extern void Func_0808a0e8(s32 id);
extern void Func_0808a020(void);

void Func_0200056c(void)
{
    u8 *record;

    Func_0808a018();
    Func_08077268();

    Func_080091e0(Func_0808a080(0), 0);
    Func_080091e0(Func_0808a080(1), 0);
    Func_080091e0(Func_0808a080(2), 0);
    Func_080091e0(Func_0808a080(3), 0);

    Func_0808a210(152 << 17, -1, 240 << 15, 0);
    Func_080000c0(1);
    Func_08009128();
    Func_080000c0(1);
    Func_080f9010(141);
    Func_080091f0(160 << 11, 160 << 11, 128 << 9);
    Func_080f9010(0x121);
    Func_080091f0(-1, -1, 0xe666);

    *(s32 *)(Data_03001ebc + 448) = 256;
    Func_0808a360();
    Func_0808a370();
    Func_080091f8();
    Func_0808a010(30);

    record = Func_0808a228();
    record[0x55] = 0;
    Func_0808a208(0xcccc, 0x1999);
    Func_0808a210(128 << 18, (s32)0xffe80000,
                  160 << 16, 1);
    Func_0808a218();
    Func_0808a338(128 << 9, 0);
    Func_0808a330(0x10005, 0);
    Func_0808a348(50);
    Func_0808a010(50);
    Func_0808a330(0x7fff, 0);
    Func_0808a348(30);
    Func_0808a010(30);

    Func_0808a0f0(0, 252 << 17, 168 << 16);
    Func_0808a0f0(1, 132 << 18, 144 << 16);
    Func_0808a0f0(2, 244 << 17, 144 << 16);
    Func_0808a0f0(3, 128 << 18, 152 << 16);
    Func_0808a100(0, 19);
    Func_0808a100(1, 19);
    Func_0808a100(2, 19);
    Func_0808a100(3, 19);
    Func_0808a010(10);

    Func_0808a330(128 << 9, 0);
    Func_0808a348(30);
    Func_0808a010(30);
    Func_0808a010(80);

    Func_080091e0(Func_0808a080(0), 1);
    Func_0808a100(0, 1);
    Func_0808a010(30);
    Func_0808a110(0, 4);
    Func_0808a1b8(0, 192 << 8, 20);
    Func_0808a130(0, 2);
    Func_0808a010(60);

    Func_080091e0(Func_0808a080(1), 1);
    Func_0808a100(1, 1);
    Func_0808a010(20);
    Func_0808a1b8(1, 128 << 6, 0);
    Func_0808a010(20);
    Func_0808a1b8(1, 192 << 7, 0);
    Func_0808a010(20);
    Func_0808a1b8(1, 0, 0);

    Func_080091e0(Func_0808a080(2), 1);
    Func_0808a100(2, 1);
    Func_0808a1b8(1, 192 << 7, 0);
    Func_0808a010(40);

    Func_080091e0(Func_0808a080(3), 1);
    Func_0808a100(3, 1);
    Func_0808a010(20);
    Func_0808a110(3, 3);
    Func_0808a010(20);

    Func_0808a090(1, 0xcccc, 0x6666);
    Func_0808a090(2, 0xcccc, 0x6666);
    Func_0808a090(3, 0xcccc, 0x6666);
    Func_0808a100(1, 2);
    Func_0808a100(2, 2);
    Func_0808a100(3, 2);

    record = Func_0808a080(0);
    if (record != 0)
        Func_0808a0b8(1, *(s16 *)(record + 10), *(s16 *)(record + 18));

    record = Func_0808a080(0);
    if (record != 0)
        Func_0808a0b8(2, *(s16 *)(record + 10), *(s16 *)(record + 18));

    record = Func_0808a080(0);
    if (record != 0)
        Func_0808a0b8(3, *(s16 *)(record + 10), *(s16 *)(record + 18));

    Func_0808a0e8(3);
    Func_0808a0f0(3, 0, 0);
    Func_0808a0e8(2);
    Func_0808a0f0(2, 0, 0);
    Func_0808a0e8(1);
    Func_0808a0f0(1, 0, 0);
    Func_0808a020();
}
