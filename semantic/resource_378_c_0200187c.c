#include "types.h"

/*
 * resource_378 scripted scene owner at 0x0200187c (3,400 bytes).
 *
 * Boundary evidence: `push {r5,r6,lr}` at 0x0200187c; the owner has a
 * single epilogue `pop {r5,r6} / pop {r0} / bx r0` at
 * 0x020025b6-0x020025ba.  The two trailing pool words at
 * 0x020025bc-0x020025c3 belong to this owner, and the next prologue is
 * 0x020025c4.  The control-flow walk also accounts for the branch-skipped
 * pools at 0x02001bb0-0x02001bcf and 0x02002224-0x0200222b.
 *
 * This is one coherent straight-line cutscene script.  Calls retain the
 * observed literal/register arguments where the byte sequence establishes
 * them.  Old-style declarations are intentional: these shared scene
 * helpers have several historical signatures across the overlays.  The
 * only back-edge is the 32-iteration actor-record update loop.
 *
 * Call accounting: 400 resolved BL sites, 29 distinct targets.  The loop
 * is represented once per machine BL site, as required by the ordered and
 * per-target multiset checks.
 */

extern u8 *Data_03001ebc;

void Func_020025c4();
void Func_02002660();
void Func_020031b8();
void Func_0808a010();
u8 *Func_0808a080();
void Func_0808a090();
void Func_0808a0c0();
void Func_0808a0c8();
void Func_0808a0d0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a130();
void Func_0808a138();
void Func_0808a148();
void Func_0808a150();
void Func_0808a158();
void Func_0808a170();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a1f0();
void Func_0808a208();
void Func_0808a210();
void Func_0808a218();
void Func_0808a330();
void Func_0808a348();
void Func_080f9010();

void Func_0200187c(void)
{
    u8 *actor;
    s32 delta = -36355;
    s32 i;

    Func_0808a1f0(1, 0x102); /* 0x1884 */
    Func_0808a138(1, 2); /* 0x188c */
    Func_0808a010(0x14); /* 0x1892 */
    Func_0808a090(0, 0xcccc, 0x6666); /* 0x189c */
    Func_0808a090(1, 0xcccc, 0x6666); /* 0x18a6 */
    Func_0808a090(0xb, 0xcccc, 0x6666); /* 0x18b0 */
    Func_0808a090(0xc, 0xcccc, 0x6666); /* 0x18ba */
    Func_0808a090(9, 0xcccc, 0x6666); /* 0x18c4 */
    Func_0808a090(0xa, 0xcccc, 0x6666); /* 0x18ce */
    Func_0808a090(8, 0xcccc, 0x6666); /* 0x18d8 */
    Func_0808a210(0xc00000, -1, 0xa00000, 1); /* 0x18ea */
    Func_0808a218(); /* 0x18ee */
    Func_0808a148(1, 0, 0); /* 0x18f8 */
    Func_0808a148(0xc, 0, 0); /* 0x1902 */
    Func_0808a148(9, 0, 0); /* 0x190c */
    Func_0808a148(0xa, 0, 0); /* 0x1916 */
    Func_0808a150(0, 0xb, 0); /* 0x1920 */
    Func_0808a010(0xa); /* 0x1926 */
    Func_0808a100(0, 3); /* 0x192e */
    Func_0808a110(0xb, 3); /* 0x1936 */
    Func_0808a010(0x3c); /* 0x193c */
    Func_0808a100(8, 3); /* 0x1944 */
    Func_0808a100(0xc, 3); /* 0x194c */
    Func_0808a100(9, 3); /* 0x1954 */
    Func_0808a110(0xa, 3); /* 0x195c */
    Func_0808a010(0x32); /* 0x1962 */
    Func_0808a148(0, 8, 0); /* 0x196c */
    Func_0808a148(0xb, 8, 0); /* 0x1976 */
    Func_0808a148(0xc, 8, 0); /* 0x1980 */
    Func_0808a148(9, 8, 0); /* 0x198a */
    Func_0808a148(0xa, 8, 0); /* 0x1994 */
    Func_0808a010(0x1e); /* 0x199a */
    Func_0808a110(1, 4); /* 0x19a2 */
    Func_0808a010(0x14); /* 0x19a8 */
    Func_0808a170(0x1171); /* 0x19ae */
    Func_0808a180(1, 0); /* 0x19b6 */
    Func_0808a010(0x14); /* 0x19bc */
    Func_0808a148(0xc, 1, 0); /* 0x19c6 */
    Func_0808a130(0xc, 2); /* 0x19ce */
    Func_0808a1e8(0xc, 0x103, 0); /* 0x19d8 */
    Func_0808a010(0x3c); /* 0x19de */
    Func_0808a180(0xc, 0); /* 0x19e6 */
    Func_0808a010(0x14); /* 0x19ec */
    Func_0808a138(1, 2); /* 0x19f4 */
    Func_0808a010(0x14); /* 0x19fa */
    Func_0808a148(1, 0xc, 0); /* 0x1a04 */
    Func_0808a010(0x14); /* 0x1a0a */
    Func_0808a110(1, 3); /* 0x1a12 */
    Func_0808a010(0x28); /* 0x1a18 */
    Func_0808a148(1, 8, 0); /* 0x1a22 */
    Func_0808a148(0xc, 8, 0); /* 0x1a2c */
    Func_0808a010(0x14); /* 0x1a32 */
    Func_0808a110(8, 3); /* 0x1a3a */
    Func_0808a010(0x14); /* 0x1a40 */
    Func_0808a1b8(8, 0xd000, 0); /* 0x1a4c */
    Func_0808a010(0x1e); /* 0x1a52 */
    Func_0808a110(8, 3); /* 0x1a5a */
    Func_0808a010(0x1e); /* 0x1a60 */
    Func_0808a180(8, 0); /* 0x1a68 */
    Func_0808a010(0x50); /* 0x1a6e */
    Func_080f9010(0x11); /* 0x1a74 */
    Func_0808a330(0x10005, 1); /* 0x1a7c */
    Func_0808a348(0x3c); /* 0x1a82 */
    Func_0808a010(0x28); /* 0x1a88 */
    Func_0808a208(0x6666, 0xccc); /* 0x1a90 */
    Func_0808a210(0xc00000, -1, 0x680000, 1); /* 0x1aa2 */
    Func_0808a010(0x78); /* 0x1aa8 */
    Func_080f9010(0x15); /* 0x1aae */
    Func_080f9010(0x134); /* 0x1ab6 */
    Func_0808a0f0(0xd, 0xc80000, 0x80000); /* 0x1ac4 */
    Func_0808a090(0xd, 0x6666, 0x3333); /* 0x1ace */
    Func_0808a0c0(0xd, 0xc8, 0x48); /* 0x1ad8 */
    Func_080f9010(0x120); /* 0x1ae0 */
    Func_0808a010(0x1e); /* 0x1ae6 */
    Func_0808a138(8, 2); /* 0x1aee */
    Func_0808a100(8, 0); /* 0x1af6 */
    Func_020025c4(); /* 0x1afa */
    Func_0808a148(0, 0xd, 0); /* 0x1b04 */
    Func_0808a148(1, 0xd, 0); /* 0x1b0e */
    Func_0808a148(0xb, 0xd, 0); /* 0x1b18 */
    Func_0808a148(0xc, 0xd, 0); /* 0x1b22 */
    Func_0808a148(9, 0xd, 0); /* 0x1b2c */
    Func_0808a148(0xa, 0xd, 0); /* 0x1b36 */
    Func_0808a130(0, 2); /* 0x1b3e */
    Func_0808a130(1, 2); /* 0x1b46 */
    Func_0808a130(0xb, 2); /* 0x1b4e */
    Func_0808a130(0xc, 2); /* 0x1b56 */
    Func_0808a130(9, 2); /* 0x1b5e */
    Func_0808a130(0xa, 2); /* 0x1b66 */
    Func_0808a010(0x28); /* 0x1b6c */
    Func_0808a180(0xd, 0); /* 0x1b74 */
    Func_0808a010(0x14); /* 0x1b7a */
    Func_0808a010(0x28); /* 0x1b80 */
    Func_0808a180(0xd, 0); /* 0x1b88 */
    Func_0808a010(0x3c); /* 0x1b8e */
    Func_0808a158(0xd, 0x100); /* 0x1b98 */
    Func_080f9010(0x11); /* 0x1b9e */
    Func_080f9010(0x134); /* 0x1ba6 */

    /* The four call sites in this loop update actor 13 by one fixed-point delta. */
    for (i = 0; i < 32; i++) {
        Func_020031b8(0xd); /* 0x1bd2 */
        Func_0808a010(4); /* 0x1bd8 */
        actor = Func_0808a080(13); /* 0x1bde */
        *(s32 *)(actor + 0x18) += delta;
        actor = Func_0808a080(13); /* 0x1bea */
        *(s32 *)(actor + 0x1c) += delta;
    }

    Func_080f9010(0x120); /* 0x1bfe */
    Func_0808a158(0xd, 0); /* 0x1c06 */
    Func_0808a0f0(0xd, 0, 0); /* 0x1c10 */
    Func_0808a010(0x28); /* 0x1c16 */
    Func_0808a210(0xc00000, -1, 0xa00000, 1); /* 0x1c28 */
    Func_0808a218(); /* 0x1c2c */
    Func_0808a330(0x10000, 0); /* 0x1c36 */
    Func_0808a348(0x3c); /* 0x1c3c */
    Func_0808a010(0x78); /* 0x1c42 */
    Func_02002660(); /* 0x1c46 */
    Func_0808a100(8, 1); /* 0x1c4e */
    Func_080f9010(2); /* 0x1c54 */
    Func_0808a010(0x3c); /* 0x1c5a */
    Func_0808a138(8, 2); /* 0x1c62 */
    Func_0808a010(0x14); /* 0x1c68 */
    Func_0808a130(0, 1); /* 0x1c70 */
    Func_0808a130(1, 1); /* 0x1c78 */
    Func_0808a130(0xb, 1); /* 0x1c80 */
    Func_0808a130(0xc, 1); /* 0x1c88 */
    Func_0808a130(9, 1); /* 0x1c90 */
    Func_0808a138(0xa, 1); /* 0x1c98 */
    Func_0808a010(0x1e); /* 0x1c9e */
    Func_0808a180(0xb, 0); /* 0x1ca6 */
    Func_0808a010(0x1e); /* 0x1cac */
    Func_0808a148(8, 0, 0); /* 0x1cb6 */
    Func_0808a010(0x1e); /* 0x1cbc */
    Func_0808a148(0xc, 8, 0); /* 0x1cc6 */
    Func_0808a010(0x14); /* 0x1ccc */
    Func_0808a148(0, 8, 0); /* 0x1cd6 */
    Func_0808a148(1, 8, 0); /* 0x1ce0 */
    Func_0808a148(0xb, 8, 0); /* 0x1cea */
    Func_0808a148(9, 8, 0); /* 0x1cf4 */
    Func_0808a148(0xa, 8, 0); /* 0x1cfe */
    Func_0808a010(0x14); /* 0x1d04 */
    Func_0808a180(0xc, 0); /* 0x1d0c */
    Func_0808a010(0x14); /* 0x1d12 */
    Func_0808a148(8, 0xc, 0); /* 0x1d1c */
    Func_0808a010(0x14); /* 0x1d22 */
    Func_0808a110(8, 3); /* 0x1d2a */
    Func_0808a010(0x14); /* 0x1d30 */
    Func_0808a100(0, 3); /* 0x1d38 */
    Func_0808a110(1, 3); /* 0x1d40 */
    Func_0808a010(0x14); /* 0x1d46 */
    Func_0808a148(8, 0, 0); /* 0x1d50 */
    Func_0808a010(0x14); /* 0x1d56 */
    Func_0808a110(8, 3); /* 0x1d5e */
    Func_0808a010(0x1e); /* 0x1d64 */
    Func_0808a180(8, 0); /* 0x1d6c */
    Func_0808a010(0x14); /* 0x1d72 */
    Func_0808a1e8(0, 0x102, 0); /* 0x1d7e */
    Func_0808a1e8(1, 0x102, 0); /* 0x1d8a */
    Func_0808a1e8(0xb, 0x102, 0); /* 0x1d96 */
    Func_0808a1e8(0xc, 0x102, 0); /* 0x1da2 */
    Func_0808a1e8(9, 0x102, 0); /* 0x1dae */
    Func_0808a1e8(0xa, 0x102, 0); /* 0x1dba */
    Func_0808a010(0x1e); /* 0x1dc0 */
    Func_0808a1e8(8, 0x102, 0); /* 0x1dcc */
    Func_0808a010(0x1e); /* 0x1dd2 */
    Func_0808a180(8, 0); /* 0x1dda */
    Func_0808a010(0x14); /* 0x1de0 */
    Func_0808a100(0, 3); /* 0x1de8 */
    Func_0808a100(1, 3); /* 0x1df0 */
    Func_0808a100(0xb, 3); /* 0x1df8 */
    Func_0808a100(0xc, 3); /* 0x1e00 */
    Func_0808a100(9, 3); /* 0x1e08 */
    Func_0808a110(0xa, 3); /* 0x1e10 */
    Func_0808a010(0x14); /* 0x1e16 */
    Func_0808a110(8, 3); /* 0x1e1e */
    Func_0808a010(0xb4); /* 0x1e24 */
    Func_0808a1e8(0xc, 0x105, 0); /* 0x1e2e */
    Func_0808a010(0x3c); /* 0x1e34 */
    Func_0808a150(0, 1, 0); /* 0x1e3e */
    Func_0808a150(9, 0xa, 0); /* 0x1e48 */
    Func_0808a010(0x28); /* 0x1e4e */
    Func_0808a148(0, 8, 0); /* 0x1e58 */
    Func_0808a148(1, 8, 0); /* 0x1e62 */
    Func_0808a148(9, 8, 0); /* 0x1e6c */
    Func_0808a148(0xa, 8, 0); /* 0x1e76 */
    Func_0808a010(0x28); /* 0x1e7c */
    Func_0808a138(1, 2); /* 0x1e84 */
    Func_0808a010(0x14); /* 0x1e8a */
    Func_0808a180(1, 0); /* 0x1e92 */
    Func_0808a010(0x1e); /* 0x1e98 */
    Func_0808a1e8(8, 0x101, 0); /* 0x1ea2 */
    Func_0808a010(0x3c); /* 0x1ea8 */
    Func_0808a130(0, 1); /* 0x1eb0 */
    Func_0808a130(1, 1); /* 0x1eb8 */
    Func_0808a130(0xb, 1); /* 0x1ec0 */
    Func_0808a130(0xc, 1); /* 0x1ec8 */
    Func_0808a130(9, 1); /* 0x1ed0 */
    Func_0808a138(0xa, 1); /* 0x1ed8 */
    Func_0808a010(0x1e); /* 0x1ede */
    Func_0808a180(0xb, 0); /* 0x1ee6 */
    Func_0808a010(0x14); /* 0x1eec */
    Func_0808a1e8(8, 0x101, 0); /* 0x1ef6 */
    Func_0808a010(0x50); /* 0x1efc */
    Func_0808a1e8(0xc, 0x102, 0); /* 0x1f08 */
    Func_0808a010(0x3c); /* 0x1f0e */
    Func_0808a180(0xc, 0); /* 0x1f16 */
    Func_0808a010(0x1e); /* 0x1f1c */
    Func_0808a1e8(8, 0x105, 0); /* 0x1f26 */
    Func_0808a010(0x3c); /* 0x1f2c */
    Func_0808a138(8, 1); /* 0x1f34 */
    Func_0808a1e8(8, 0x106, 0); /* 0x1f40 */
    Func_0808a010(0x14); /* 0x1f46 */
    Func_0808a130(0, 1); /* 0x1f4e */
    Func_0808a130(1, 1); /* 0x1f56 */
    Func_0808a130(0xb, 1); /* 0x1f5e */
    Func_0808a130(0xc, 1); /* 0x1f66 */
    Func_0808a130(9, 1); /* 0x1f6e */
    Func_0808a138(0xa, 1); /* 0x1f76 */
    Func_0808a010(0x28); /* 0x1f7c */
    Func_0808a180(8, 0); /* 0x1f84 */
    Func_0808a010(0x14); /* 0x1f8a */
    Func_0808a100(0, 3); /* 0x1f92 */
    Func_0808a100(1, 3); /* 0x1f9a */
    Func_0808a100(0xb, 3); /* 0x1fa2 */
    Func_0808a100(0xc, 3); /* 0x1faa */
    Func_0808a100(9, 3); /* 0x1fb2 */
    Func_0808a110(0xa, 3); /* 0x1fba */
    Func_0808a010(0x1e); /* 0x1fc0 */
    Func_0808a180(8, 0); /* 0x1fc8 */
    Func_0808a010(0x1e); /* 0x1fce */
    Func_0808a100(0, 3); /* 0x1fd6 */
    Func_0808a100(1, 3); /* 0x1fde */
    Func_0808a100(0xb, 3); /* 0x1fe6 */
    Func_0808a100(0xc, 3); /* 0x1fee */
    Func_0808a100(9, 3); /* 0x1ff6 */
    Func_0808a110(0xa, 3); /* 0x1ffe */
    Func_0808a010(0x78); /* 0x2004 */
    Func_0808a138(1, 1); /* 0x200c */
    Func_0808a010(0x14); /* 0x2012 */
    Func_0808a180(1, 0); /* 0x201a */
    Func_0808a010(0x1e); /* 0x2020 */
    Func_0808a110(8, 3); /* 0x2028 */
    Func_0808a010(0x14); /* 0x202e */
    Func_0808a180(8, 0); /* 0x2036 */
    Func_0808a010(0x14); /* 0x203c */
    Func_0808a1e8(0, 0x105, 0); /* 0x2046 */
    Func_0808a1e8(1, 0x105, 0); /* 0x2050 */
    Func_0808a1e8(0xb, 0x105, 0); /* 0x205a */
    Func_0808a1e8(0xc, 0x100, 0); /* 0x2066 */
    Func_0808a1e8(9, 0x105, 0); /* 0x2070 */
    Func_0808a1e8(0xa, 0x105, 0); /* 0x207a */
    Func_0808a010(0x3c); /* 0x2080 */
    Func_0808a138(0xc, 1); /* 0x2088 */
    Func_0808a180(0xc, 0); /* 0x2090 */
    Func_0808a010(0x14); /* 0x2096 */
    Func_0808a148(8, 0xc, 0); /* 0x20a0 */
    Func_0808a010(0x14); /* 0x20a6 */
    Func_0808a110(8, 4); /* 0x20ae */
    Func_0808a010(0x14); /* 0x20b4 */
    Func_0808a180(8, 0); /* 0x20bc */
    Func_0808a010(0x14); /* 0x20c2 */
    Func_0808a1e8(0, 0x100, 0); /* 0x20ce */
    Func_0808a1e8(1, 0x100, 0); /* 0x20da */
    Func_0808a1e8(0xb, 0x100, 0); /* 0x20e6 */
    Func_0808a1e8(0xc, 0x100, 0); /* 0x20f2 */
    Func_0808a1e8(9, 0x100, 0); /* 0x20fe */
    Func_0808a1e8(0xa, 0x100, 0); /* 0x210a */
    Func_0808a010(0x3c); /* 0x2110 */
    Func_0808a148(8, 0, 0); /* 0x211a */
    Func_0808a010(0x14); /* 0x2120 */
    Func_0808a110(8, 3); /* 0x2128 */
    Func_0808a180(8, 0); /* 0x2130 */
    Func_0808a010(0x28); /* 0x2136 */
    Func_0808a0d0(8, 0xa8, 0xb0); /* 0x2140 */
    Func_0808a148(0, 8, 0); /* 0x214a */
    Func_0808a148(1, 8, 0); /* 0x2154 */
    Func_0808a148(0xb, 8, 0); /* 0x215e */
    Func_0808a148(0xc, 8, 0); /* 0x2168 */
    Func_0808a148(9, 8, 0); /* 0x2172 */
    Func_0808a148(0xa, 8, 0); /* 0x217c */
    Func_0808a0d0(8, 0xc8, 0xc8); /* 0x2186 */
    Func_0808a148(0, 8, 0); /* 0x2190 */
    Func_0808a148(1, 8, 0); /* 0x219a */
    Func_0808a148(0xc, 8, 0); /* 0x21a4 */
    Func_0808a1b8(0xb, 0, 0); /* 0x21ae */
    Func_0808a1b8(9, 0x8000, 0); /* 0x21ba */
    Func_0808a1b8(0xa, 0x8000, 0); /* 0x21c6 */
    Func_0808a0c8(8, 0xc8, 0x110); /* 0x21d2 */
    Func_0808a010(0x28); /* 0x21d8 */
    Func_0808a1b8(0xb, 0x3000, 0); /* 0x21e4 */
    Func_0808a1b8(9, 0x5000, 0); /* 0x21f0 */
    Func_0808a1b8(0xa, 0x5000, 0); /* 0x21fc */
    Func_0808a0e8(8); /* 0x2202 */
    Func_0808a0f0(8, 0, 0); /* 0x220c */
    Func_0808a010(0x3c); /* 0x2212 */
    Func_0808a1e8(0, 0x102, 0); /* 0x221e */
    Func_0808a1e8(1, 0x102, 0); /* 0x2234 */
    Func_0808a1e8(0xb, 0x102, 0); /* 0x2240 */
    Func_0808a1e8(0xc, 0x102, 0); /* 0x224c */
    Func_0808a1e8(9, 0x102, 0); /* 0x2258 */
    Func_0808a1e8(0xa, 0x102, 0); /* 0x2264 */
    Func_0808a010(0x3c); /* 0x226a */
    Func_0808a010(0x78); /* 0x2270 */
    Func_0808a148(0, 9, 0); /* 0x227a */
    Func_0808a148(1, 0xa, 0); /* 0x2284 */
    Func_0808a148(0xb, 9, 0); /* 0x228e */
    Func_0808a148(0xc, 9, 0); /* 0x2298 */
    Func_0808a010(0x78); /* 0x229e */
    Func_0808a1e8(9, 0x105, 0); /* 0x22a8 */
    Func_0808a010(0x3c); /* 0x22ae */
    Func_0808a148(9, 0, 0); /* 0x22b8 */
    Func_0808a010(0xa); /* 0x22be */
    Func_0808a138(9, 2); /* 0x22c6 */
    Func_0808a010(0xa); /* 0x22cc */
    Func_0808a1b8(9, 0x5000, 0); /* 0x22d8 */
    Func_0808a010(0x14); /* 0x22de */
    Func_0808a1e8(9, 0x102, 0); /* 0x22ea */
    Func_0808a010(0x50); /* 0x22f0 */
    Func_0808a148(9, 0xa, 0); /* 0x22fa */
    Func_0808a138(9, 1); /* 0x2302 */
    Func_0808a010(0x14); /* 0x2308 */
    Func_0808a180(9, 0); /* 0x2310 */
    Func_0808a010(0x14); /* 0x2316 */
    Func_0808a1b8(0xa, 0x5000, 0); /* 0x2322 */
    Func_0808a010(0x14); /* 0x2328 */
    Func_0808a1e8(0xa, 0x101, 0); /* 0x2332 */
    Func_0808a010(0x3c); /* 0x2338 */
    Func_0808a138(9, 2); /* 0x2340 */
    Func_0808a010(0x14); /* 0x2346 */
    Func_0808a148(0xa, 0, 0); /* 0x2350 */
    Func_0808a010(0x1e); /* 0x2356 */
    Func_0808a138(0xa, 2); /* 0x235e */
    Func_0808a010(0x1e); /* 0x2364 */
    Func_0808a1b8(0xa, 0x5000, 0); /* 0x2370 */
    Func_0808a010(0x14); /* 0x2376 */
    Func_0808a180(0xa, 0); /* 0x237e */
    Func_0808a010(0x14); /* 0x2384 */
    Func_0808a100(9, 3); /* 0x238c */
    Func_0808a110(0xa, 3); /* 0x2394 */
    Func_0808a010(0x14); /* 0x239a */
    Func_0808a0c8(9, 0xc8, 0x110); /* 0x23a6 */
    Func_0808a0d0(0xa, 0xc8, 0x110); /* 0x23b2 */
    Func_0808a0f0(9, 0, 0); /* 0x23bc */
    Func_0808a0f0(0xa, 0, 0); /* 0x23c6 */
    Func_0808a1e8(0xc, 0x105, 0); /* 0x23d0 */
    Func_0808a010(0x3c); /* 0x23d6 */
    Func_0808a0d0(0xc, 0xc8, 0x88); /* 0x23e0 */
    Func_0808a150(0, 0xc, 0); /* 0x23ea */
    Func_0808a148(1, 0xc, 0); /* 0x23f4 */
    Func_0808a148(0xb, 0xc, 0); /* 0x23fe */
    Func_0808a010(0x1e); /* 0x2404 */
    Func_0808a110(0xc, 4); /* 0x240c */
    Func_0808a010(0x14); /* 0x2412 */
    Func_0808a180(0xc, 0); /* 0x241a */
    Func_0808a010(0x1e); /* 0x2420 */
    Func_0808a100(0, 3); /* 0x2428 */
    Func_0808a110(1, 3); /* 0x2430 */
    Func_0808a010(0x14); /* 0x2436 */
    Func_0808a180(0xb, 0); /* 0x243e */
    Func_0808a010(0x14); /* 0x2444 */
    Func_0808a0d0(0xb, 0xa8, 0xa8); /* 0x244e */
    Func_0808a148(0xb, 0xc, 0); /* 0x2458 */
    Func_0808a010(0x14); /* 0x245e */
    Func_0808a148(0, 0xb, 0); /* 0x2468 */
    Func_0808a148(1, 0xb, 0); /* 0x2472 */
    Func_0808a010(0x14); /* 0x2478 */
    Func_0808a110(0xb, 4); /* 0x2480 */
    Func_0808a010(0x14); /* 0x2486 */
    Func_0808a180(0xb, 0); /* 0x248e */
    Func_0808a010(0x14); /* 0x2494 */
    Func_0808a1e8(0, 0x102, 0); /* 0x24a0 */
    Func_0808a1e8(1, 0x102, 0); /* 0x24ac */
    Func_0808a010(0x3c); /* 0x24b2 */
    Func_0808a110(0xc, 4); /* 0x24ba */
    Func_0808a010(0x14); /* 0x24c0 */
    Func_0808a148(0, 0xc, 0); /* 0x24ca */
    Func_0808a148(1, 0xc, 0); /* 0x24d4 */
    Func_0808a010(0x14); /* 0x24da */
    Func_0808a180(0xc, 0); /* 0x24e2 */
    Func_0808a010(0x14); /* 0x24e8 */
    Func_0808a138(0xb, 2); /* 0x24f0 */
    Func_0808a010(0x14); /* 0x24f6 */
    Func_0808a180(0xb, 0); /* 0x24fe */
    Func_0808a010(0x14); /* 0x2504 */
    Func_0808a1e8(0, 0x102, 0); /* 0x2510 */
    Func_0808a1e8(1, 0x102, 0); /* 0x251c */
    Func_0808a010(0x3c); /* 0x2522 */
    Func_0808a1e8(0xc, 0x102, 0); /* 0x252e */
    Func_0808a010(0x3c); /* 0x2534 */
    Func_0808a180(0xc, 0); /* 0x253c */
    Func_0808a010(0x14); /* 0x2542 */
    Func_0808a130(0, 2); /* 0x254a */
    Func_0808a130(1, 2); /* 0x2552 */
    Func_0808a1e8(0, 0x100, 0); /* 0x255e */
    Func_0808a1e8(1, 0x100, 0); /* 0x256a */
    Func_0808a010(0x3c); /* 0x2570 */
    Func_0808a110(0xb, 3); /* 0x2578 */
    Func_0808a010(0x14); /* 0x257e */
    Func_0808a180(0xb, 0); /* 0x2586 */
    Func_0808a010(0x14); /* 0x258c */
    Func_0808a148(0, 0xb, 0); /* 0x2596 */
    Func_0808a010(0x1e); /* 0x259c */
    Func_0808a100(0xb, 3); /* 0x25a4 */
    Func_0808a010(0x1e); /* 0x25aa */
    Func_0808a100(0xc, 3); /* 0x25b2 */
}
