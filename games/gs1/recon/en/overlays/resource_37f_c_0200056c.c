/* Contiguous unnamed state-owner run for resource_37f. */

#include "types.h"

extern s32 *Func_0200213e(s32);
extern void Func_0200211c(s32);
extern void Func_02002122(s32);
extern void Func_02002124(s32);
extern void Func_02002130(s32);

void Func_0200056c(void)
{
    s32 *record = Func_0200213e(9);
    s32 heading;

    if (record == 0) {
        return;
    }
    heading = record[2] >> 20;
    Func_0200211c(0x302);
    Func_02002122(0x303);
    if (heading == 93) {
        Func_02002124(0x303);
    } else if (heading == 95) {
        Func_02002130(0x302);
    }
}

#include "types.h"

extern s32 *Func_0200217e(s32);
extern void Func_0200215e(s32);
extern void Func_02002164(s32);
extern void Func_02002168(s32);
extern void Func_02002174(s32);

void Func_020005ac(void)
{
    s32 *record = Func_0200217e(10);
    s32 heading;

    if (record == 0) {
        return;
    }
    heading = record[2] >> 20;
    Func_0200215e(0x300);
    Func_02002164(0x301);
    if (heading == 115) {
        Func_02002168(0x300);
    } else if (heading == 113) {
        Func_02002174(0x301);
    }
}

#include "types.h"

extern s32 *Func_020021be(s32);
extern void Func_0200219e(s32);
extern void Func_020021a4(s32);
extern void Func_020021a6(s32);
extern void Func_020021b4(s32);
extern void Func_02001de6(s32);

void Func_020005ec(void)
{
    s32 *record = Func_020021be(9);
    s32 heading;

    if (record == 0) {
        return;
    }
    heading = record[2] >> 20;
    Func_0200219e(0x310);
    Func_020021a4(0x311);
    if (heading == 99) {
        Func_020021a6(0x311);
    } else if (heading == 101) {
        Func_020021b4(0x310);
    }
    Func_02001de6(0);
}

#include "types.h"

extern s32 *Func_02002206(s32);
extern void Func_020021e4(s32);
extern void Func_020021ea(s32);
extern void Func_020021ec(s32);
extern void Func_020021f8(s32);
extern void Func_02001e2a(s32);

void Func_02000634(void)
{
    s32 *record = Func_02002206(10);
    s32 heading;

    if (record == 0) {
        return;
    }
    heading = record[2] >> 20;
    Func_020021e4(0x312);
    Func_020021ea(0x313);
    if (heading == 103) {
        Func_020021ec(0x313);
    } else if (heading == 105) {
        Func_020021f8(0x312);
    }
    Func_02001e2a(0);
}

#include "types.h"

extern s32 *Func_0200224e(s32);
extern void Func_0200222e(s32);
extern void Func_02002234(s32);
extern void Func_02002236(s32);
extern void Func_02002244(s32);
extern void Func_02001e76(s32);

void Func_0200067c(void)
{
    s32 *record = Func_0200224e(11);
    s32 heading;

    if (record == 0) {
        return;
    }
    heading = record[2] >> 20;
    Func_0200222e(0x314);
    Func_02002234(0x315);
    if (heading == 107) {
        Func_02002236(0x315);
    } else if (heading == 109) {
        Func_02002244(0x314);
    }
    Func_02001e76(0);
}

#include "types.h"

extern s32 *Func_02002296(s32);
extern void Func_02002274(s32);
extern void Func_0200227a(s32);
extern void Func_0200227c(s32);
extern void Func_02002288(s32);
extern void Func_02001eba(s32);

void Func_020006c4(void)
{
    s32 *record = Func_02002296(12);
    s32 heading;

    if (record == 0) {
        return;
    }
    heading = record[2] >> 20;
    Func_02002274(0x316);
    Func_0200227a(0x317);
    if (heading == 111) {
        Func_0200227c(0x317);
    } else if (heading == 113) {
        Func_02002288(0x316);
    }
    Func_02001eba(0);
}

#include "types.h"

extern s32 *Func_020022de(s32);
extern void Func_020022be(s32);
extern void Func_020022c4(s32);
extern void Func_020022c6(s32);
extern void Func_020022d4(s32);
extern void Func_02001f06(s32);

void Func_0200070c(void)
{
    s32 *record = Func_020022de(13);
    s32 heading;

    if (record == 0) {
        return;
    }
    heading = record[2] >> 20;
    Func_020022be(0x318);
    Func_020022c4(0x319);
    if (heading == 115) {
        Func_020022c6(0x319);
    } else if (heading == 117) {
        Func_020022d4(0x318);
    }
    Func_02001f06(0);
}

#include "types.h"

extern s32 *Func_02002326(s32);
extern void Func_02002304(s32);
extern void Func_0200230a(s32);
extern void Func_0200230c(s32);
extern void Func_02002318(s32);
extern void Func_02001f4a(s32);

void Func_02000754(void)
{
    s32 *record = Func_02002326(14);
    s32 heading;

    if (record == 0) return;
    heading = record[2] >> 20;
    Func_02002304(0x31a);
    Func_0200230a(0x31b);
    if (heading == 119) {
        Func_0200230c(0x31b);
    } else if (heading == 121) {
        Func_02002318(0x31a);
    }
    Func_02001f4a(0);
}

#include "types.h"

extern u8 *Data_03001ebc;

s32 *Func_0200079c(s32 arg0, s32 arg1) {
    s32 **slots = (s32 **)(Data_03001ebc + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if (arg0 == (p[2] >> 20) && arg1 == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

