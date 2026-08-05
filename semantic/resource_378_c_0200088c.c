#include "types.h"

/*
 * resource_378 scripted scene owner at 0x0200088c (4,080 bytes).
 *
 * Boundary evidence: `push {r5,lr}` at 0x0200088c; the only return is
 * `pop {r5} / pop {r0} / bx r0` at 0x0200186c-0x02001870.  The
 * alignment word at 0x02001872 and the two literal-pool words at
 * 0x02001874-0x0200187b belong to this owner; the next prologue is
 * 0x0200187c.  The control-flow walk also accounts for the embedded
 * pools at 0x02000b48-0x02000b5f, 0x02000f3c-0x02000f43, and
 * 0x02001600-0x02001607, all skipped by branches.
 *
 * This is a long, straight-line cutscene script.  Calls retain their
 * observed register arguments where the literal arithmetic establishes
 * them; old-style declarations are intentional because these shared
 * scene helpers are called with several historical signatures.  The
 * three scene-state tests preserve their source branch arms.  Loop
 * back-edges are represented by one source statement per distinct BL
 * site, matching the machine-site accounting used by the checkers.
 *
 * Call accounting: 489 resolved BL sites (the apparent BL at 0x020017ca,
 * named Func_020017ca by the raw target walk, is an intra-owner branch
 * target in the literal-pool path, not a callee).
 */

extern u8 *Data_03001ebc;
#define SKIP_BEATS (*(u16 *)(Data_03001ebc + 0x1d8))

void Func_0200187c();
void Func_020025c4();
void Func_02002660();
void Func_02002750();
void Func_0200290c();
s32 Func_080770c0();
void Func_080770c8();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
s32 Func_0808a070();
u8 *Func_0808a080();
void Func_0808a090();
void Func_0808a098();
void Func_0808a0d0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a118();
void Func_0808a130();
void Func_0808a138();
void Func_0808a148();
void Func_0808a150();
void Func_0808a158();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a1f0();
void Func_0808a210();
void Func_0808a218();
void Func_0808a248();
void Func_0808a330();
void Func_0808a348();
void Func_0808a360();
void Func_0808a370();
void Func_080f9010();
void Func_080f9078();

void Func_0200088c(void)
{
    u8 *actor;

    Func_0808a018(); /* 0x088e */
    if (Func_080770c0(0x201) != 0) {
        Func_0808a150(0, 8, 0); /* 0x08a2 */
        Func_0808a148(1, 0, 0); /* 0x08ac */
        Func_0808a148(0xb, 0, 0); /* 0x08b6 */
        Func_0808a148(0xc, 0, 0); /* 0x08c0 */
        Func_0808a148(9, 0, 0); /* 0x08ca */
        Func_0808a148(0xa, 0, 0); /* 0x08d4 */
        Func_0808a210(0xc00000, -1, 0xa00000, 1); /* 0x08e6 */
        Func_0808a218(); /* 0x08ea */
        Func_0808a360(); /* 0x0904 */
        Func_0808a370(); /* 0x0908 */
        Func_0808a010(0x78); /* 0x090e */
    }

    Func_0808a330(0x10002, 0); /* 0x091a */
    Func_0808a348(1); /* 0x0920 */
    Func_0808a010(1); /* 0x0926 */
    Func_0808a210(0xc00000, -1, 0xa00000, 1); /* 0x0938 */
    Func_0808a218(); /* 0x093c */
    Func_0808a360(); /* 0x094e */
    Func_0808a370(); /* 0x0952 */
    Func_0200290c(); /* 0x0956 */
    Func_0808a330(0x10000, 0); /* 0x0960 */
    Func_0808a348(0x3c); /* 0x0966 */
    Func_0808a010(0x64); /* 0x096c */
    Func_0808a100(0, 1); /* 0x0974 */
    Func_0808a100(1, 1); /* 0x097c */
    Func_0808a010(0x1e); /* 0x0982 */
    Func_0808a148(0, 0xc, 0); /* 0x098c */
    Func_0808a148(1, 0xc, 0); /* 0x0996 */
    Func_0808a010(0x14); /* 0x099c */
    Func_0808a098(8, 1); /* 0x09a4 */
    Func_0808a098(0xc, 1); /* 0x09ac */
    Func_0808a090(0, 0xcccc, 0x6666); /* 0x09b6 */
    Func_0808a090(1, 0xcccc, 0x6666); /* 0x09c0 */
    Func_0808a090(0xb, 0xcccc, 0x6666); /* 0x09ca */
    Func_0808a090(0xc, 0xcccc, 0x6666); /* 0x09d4 */
    Func_0808a090(9, 0xcccc, 0x6666); /* 0x09de */
    Func_0808a090(0xa, 0xcccc, 0x6666); /* 0x09e8 */
    Func_0808a090(8, 0xcccc, 0x6666); /* 0x09f2 */
    Func_0808a010(0x1e); /* 0x09f8 */
    Func_0808a138(0xc, 2); /* 0x0a00 */
    Func_0808a170(0x1138); /* 0x0a06 */
    Func_0808a180(0xc, 0); /* 0x0a0e */
    Func_0808a010(0xa); /* 0x0a14 */
    Func_0808a100(0, 3); /* 0x0a1c */
    Func_0808a110(1, 3); /* 0x0a24 */
    Func_0808a010(0x1e); /* 0x0a2a */
    Func_0808a110(0xc, 3); /* 0x0a32 */
    Func_0808a010(0x14); /* 0x0a38 */
    Func_0808a138(0xb, 2); /* 0x0a40 */
    Func_0808a010(0x14); /* 0x0a46 */
    Func_0808a148(0, 0xb, 0); /* 0x0a50 */
    Func_0808a148(1, 0xb, 0); /* 0x0a5a */
    Func_0808a010(0x14); /* 0x0a60 */
    Func_0808a178(0xb, 0); /* 0x0a68 */
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(0x14); /* 0x0a7a */
        Func_0808a110(0xb, 3); /* 0x0a82 */
        Func_0808a010(0x14); /* 0x0a88 */
        Func_0808a180(0xb, 0); /* 0x0a90 */
        SKIP_BEATS++;
    } else {
        Func_0808a010(0x14); /* 0x0aa6 */
        Func_0808a110(0xb, 4); /* 0x0aae */
        Func_0808a010(0x14); /* 0x0ab4 */
        Func_0808a180(0xb, 0); /* 0x0aca */
        SKIP_BEATS++;
    }
    Func_0808a010(0x14); /* 0x0ad0 */
    Func_0808a138(9, 1); /* 0x0ad8 */
    Func_0808a010(0x14); /* 0x0ade */
    Func_0808a148(0, 9, 0); /* 0x0ae8 */
    Func_0808a148(1, 9, 0); /* 0x0af2 */
    Func_0808a148(0xb, 9, 0); /* 0x0afc */
    Func_0808a010(0x14); /* 0x0b02 */
    Func_0808a178(9, 0); /* 0x0b0a */
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(0x14); /* 0x0b1c */
        Func_0808a110(9, 3); /* 0x0b24 */
        Func_0808a010(0x14); /* 0x0b2a */
        Func_0808a180(9, 0); /* 0x0b32 */
    } else {
        Func_0808a010(0x14); /* 0x0b62 */
        Func_0808a110(9, 4); /* 0x0b6a */
        SKIP_BEATS++;
        Func_0808a010(0x14); /* 0x0b70 */
        SKIP_BEATS++;
        Func_0808a180(9, 0); /* 0x0b88 */
    }
    Func_0808a010(0x14); /* 0x0b8e */
    Func_0808a100(0, 3); /* 0x0b96 */
    Func_0808a100(1, 3); /* 0x0b9e */
    Func_0808a100(0xb, 3); /* 0x0ba6 */
    Func_0808a100(0xc, 3); /* 0x0bae */
    Func_0808a100(9, 3); /* 0x0bb6 */
    Func_0808a110(0xa, 3); /* 0x0bbe */
    Func_0808a010(0x1e); /* 0x0bc4 */
    Func_0808a1e8(0xc, 0x101, 0); /* 0x0bce */
    Func_0808a010(0x3c); /* 0x0bd4 */
    Func_0808a148(0xc, 8, 0); /* 0x0bde */
    Func_0808a010(0x14); /* 0x0be4 */
    Func_0808a0d0(0xc, 0xe0, 0x78); /* 0x0bee */
    Func_0808a010(0xa); /* 0x0bf4 */
    Func_0808a180(0xc, 0); /* 0x0bfc */
    Func_0808a010(0x14); /* 0x0c02 */
    Func_0808a148(0, 8, 0); /* 0x0c0c */
    Func_0808a148(1, 8, 0); /* 0x0c16 */
    Func_0808a148(0xb, 8, 0); /* 0x0c20 */
    Func_0808a148(9, 8, 0); /* 0x0c2a */
    Func_0808a148(0xa, 8, 0); /* 0x0c34 */
    Func_0808a010(0xa); /* 0x0c3a */
    Func_0808a138(0xb, 1); /* 0x0c42 */
    Func_0808a010(0xa); /* 0x0c48 */
    Func_0808a180(0xb, 0); /* 0x0c50 */
    Func_0808a010(0x1e); /* 0x0c56 */
    Func_0808a138(8, 3); /* 0x0c5e */
    Func_0808a1e8(8, 0x100, 0); /* 0x0c6a */
    Func_0808a010(0x3c); /* 0x0c70 */
    Func_0808a148(8, 0xc, 0); /* 0x0c7a */
    Func_0808a010(0x14); /* 0x0c80 */
    Func_0808a110(8, 3); /* 0x0c88 */
    Func_0808a010(0xa); /* 0x0c8e */
    Func_0808a180(8, 0); /* 0x0c96 */
    Func_0808a010(0x14); /* 0x0c9c */
    Func_0808a1b8(0xc, 0x5000, 0); /* 0x0ca8 */
    Func_0808a010(0x3c); /* 0x0cae */
    Func_0808a148(0xc, 8, 0); /* 0x0cb8 */
    Func_0808a010(0x14); /* 0x0cbe */
    Func_0808a110(0xc, 3); /* 0x0cc6 */
    Func_0808a010(0xa); /* 0x0ccc */
    Func_0808a180(0xc, 0); /* 0x0cd4 */
    Func_0808a010(0xa); /* 0x0cda */
    Func_0808a110(8, 3); /* 0x0ce2 */
    Func_0808a010(0x14); /* 0x0ce8 */
    Func_0808a1b8(8, 0x3000, 0); /* 0x0cf4 */
    Func_0808a010(0xa); /* 0x0cfa */
    Func_0808a180(8, 0); /* 0x0d02 */
    Func_0808a010(0x14); /* 0x0d08 */
    Func_0808a1b8(0xc, 0x5000, 0); /* 0x0d14 */
    Func_0808a010(0x3c); /* 0x0d1a */
    Func_0808a148(0xc, 8, 0); /* 0x0d24 */
    Func_0808a010(0x32); /* 0x0d2a */
    Func_0808a1e8(0xc, 0x101, 0); /* 0x0d34 */
    Func_0808a010(0x28); /* 0x0d3a */
    Func_0808a1e8(0, 0x101, 0); /* 0x0d44 */
    Func_0808a1e8(1, 0x101, 0); /* 0x0d4e */
    Func_0808a1e8(0xb, 0x101, 0); /* 0x0d58 */
    Func_0808a1e8(9, 0x101, 0); /* 0x0d62 */
    Func_0808a1e8(0xa, 0x101, 0); /* 0x0d6c */
    Func_0808a010(0x3c); /* 0x0d72 */
    Func_0808a138(8, 1); /* 0x0d7a */
    Func_0808a010(0xa); /* 0x0d80 */
    Func_0808a180(8, 0); /* 0x0d88 */
    Func_0808a010(0xa); /* 0x0d8e */
    Func_0808a1e8(0, 0x100, 0); /* 0x0d9a */
    Func_0808a1e8(1, 0x100, 0); /* 0x0da6 */
    Func_0808a1e8(0xb, 0x100, 0); /* 0x0db2 */
    Func_0808a1e8(0xc, 0x100, 0); /* 0x0dbe */
    Func_0808a1e8(9, 0x100, 0); /* 0x0dca */
    Func_0808a1e8(0xa, 0x100, 0); /* 0x0dd6 */
    Func_0808a010(0x3c); /* 0x0ddc */
    Func_0808a138(1, 2); /* 0x0de4 */
    Func_0808a010(0xa); /* 0x0dea */
    Func_0808a180(1, 0); /* 0x0df2 */
    Func_0808a010(0x14); /* 0x0df8 */
    Func_0808a110(8, 3); /* 0x0e00 */
    Func_0808a010(0xa); /* 0x0e06 */
    Func_0808a180(8, 0); /* 0x0e0e */
    Func_0808a010(0xa); /* 0x0e14 */
    Func_0808a1e8(0xc, 0x102, 0); /* 0x0e20 */
    Func_0808a130(0, 1); /* 0x0e28 */
    Func_0808a130(1, 1); /* 0x0e30 */
    Func_0808a130(0xb, 1); /* 0x0e38 */
    Func_0808a130(9, 1); /* 0x0e40 */
    Func_0808a138(0xa, 1); /* 0x0e48 */
    Func_0808a010(0xa); /* 0x0e4e */
    Func_0808a110(8, 4); /* 0x0e56 */
    Func_0808a010(0xa); /* 0x0e5c */
    Func_0808a180(8, 0); /* 0x0e64 */
    Func_0808a010(0x1e); /* 0x0e6a */
    Func_0808a148(0xc, 0, 0); /* 0x0e74 */
    Func_0808a148(0xb, 0, 0); /* 0x0e7e */
    Func_0808a010(0x14); /* 0x0e84 */
    Func_0808a138(0, 2); /* 0x0e8c */
    Func_0808a010(0x3c); /* 0x0e92 */
    Func_0808a138(0xc, 2); /* 0x0e9a */
    Func_0808a010(0x14); /* 0x0ea0 */
    Func_0808a148(0xc, 8, 0); /* 0x0eaa */
    Func_0808a010(0x14); /* 0x0eb0 */
    Func_0808a180(0xc, 0); /* 0x0eb8 */
    Func_0808a010(0x1e); /* 0x0ebe */
    Func_0808a138(0xb, 2); /* 0x0ec6 */
    Func_0808a010(0x14); /* 0x0ecc */
    Func_0808a148(8, 0, 0); /* 0x0ed6 */
    Func_0808a010(0x14); /* 0x0edc */
    Func_0808a180(0xb, 0); /* 0x0ee4 */
    Func_0808a010(0x14); /* 0x0eea */
    Func_0808a010(0x28); /* 0x0ef0 */
    Func_0808a138(8, 2); /* 0x0ef8 */
    Func_0808a100(8, 0); /* 0x0f00 */
    Func_0808a158(8, 0x100); /* 0x0f0c */
    Func_020025c4(); /* 0x0f12 */
    Func_080f9010(0xc4); /* 0x0f18 */
    Func_02002750(8, 0x1200); /* 0x0f20 */
    Func_0808a010(0x20); /* 0x0f26 */
    Func_02002750(8, 0x1200); /* 0x0f2e */
    Func_0808a010(0x10); /* 0x0f34 */
    Func_02002750(8, 0x1200); /* 0x0f4a */
    Func_0808a010(8); /* 0x0f52 */
    Func_0808a010(8); /* 0x0f60 */
    Func_02002750(8, 0x1200); /* 0x0f68 */
    Func_0808a010(0x20); /* 0x0f6e */
    Func_02002750(8, 0x1200); /* 0x0f76 */
    Func_0808a010(0x60); /* 0x0f7c */
    Func_0808a010(0x20); /* 0x0f82 */
    Func_0808a158(8, 0); /* 0x0f8a */
    Func_0808a010(0x1e); /* 0x0f90 */
    Func_02002660(); /* 0x0f94 */
    Func_0808a100(8, 1); /* 0x0f9c */
    Func_0808a138(8, 2); /* 0x0fa4 */
    Func_0808a010(0x14); /* 0x0faa */
    Func_0808a180(8, 0); /* 0x0fb2 */
    Func_0808a010(0x14); /* 0x0fb8 */
    Func_0808a1f0(0xc, 0x102); /* 0x0fc2 */
    Func_0808a1f0(0, 0x102); /* 0x0fcc */
    Func_0808a1f0(1, 0x102); /* 0x0fd6 */
    Func_0808a1f0(0xb, 0x102); /* 0x0fe0 */
    Func_0808a1f0(9, 0x102); /* 0x0fea */
    Func_0808a1f0(0xa, 0x102); /* 0x0ff4 */
    Func_0808a010(0x3c); /* 0x0ffa */
    Func_0808a138(0xa, 1); /* 0x1002 */
    Func_0808a010(0x14); /* 0x1008 */
    Func_0808a180(0xa, 0); /* 0x1010 */
    Func_0808a010(0x14); /* 0x1016 */
    Func_0808a150(0, 1, 0); /* 0x1020 */
    Func_0808a010(0x3c); /* 0x1026 */
    Func_0808a110(8, 4); /* 0x102e */
    Func_0808a010(0x14); /* 0x1034 */
    Func_0808a148(0, 8, 0); /* 0x103e */
    Func_0808a148(1, 8, 0); /* 0x1048 */
    Func_0808a010(0x14); /* 0x104e */
    Func_0808a180(8, 0); /* 0x1056 */
    Func_0808a010(0x1e); /* 0x105c */
    Func_0808a150(0, 1, 0); /* 0x1066 */
    Func_0808a010(0x14); /* 0x106c */
    Func_0808a100(0, 3); /* 0x1074 */
    Func_0808a100(1, 3); /* 0x107c */
    Func_0808a150(9, 0xa, 0); /* 0x1086 */
    Func_0808a010(0x14); /* 0x108c */
    Func_0808a100(9, 3); /* 0x1094 */
    Func_0808a100(0xa, 3); /* 0x109c */
    Func_0808a118(0xa); /* 0x10a2 */
    Func_0808a010(0x1e); /* 0x10a8 */
    Func_0808a150(0, 0xb, 0); /* 0x10b2 */
    Func_0808a150(1, 0xc, 0); /* 0x10bc */
    Func_0808a010(0x14); /* 0x10c2 */
    Func_0808a100(0, 3); /* 0x10ca */
    Func_0808a100(0xb, 3); /* 0x10d2 */
    Func_0808a100(1, 3); /* 0x10da */
    Func_0808a100(0xc, 3); /* 0x10e2 */
    Func_0808a118(0xc); /* 0x10e8 */
    Func_0808a010(0x3c); /* 0x10ee */
    Func_0808a138(8, 2); /* 0x10f6 */
    Func_0808a100(8, 0); /* 0x10fe */
    Func_0808a158(8, 0x100); /* 0x1108 */
    Func_020025c4(); /* 0x110c */
    Func_080f9010(0xc4); /* 0x1112 */
    Func_02002750(8, 0x1200); /* 0x111a */
    Func_0808a010(0x20); /* 0x1120 */
    Func_02002750(8, 0x1200); /* 0x1128 */
    Func_0808a148(0, 8, 0); /* 0x1132 */
    Func_0808a148(1, 8, 0); /* 0x113c */
    Func_0808a148(0xb, 8, 0); /* 0x1146 */
    Func_0808a148(0xc, 8, 0); /* 0x1150 */
    Func_0808a148(9, 8, 0); /* 0x115a */
    Func_0808a148(0xa, 8, 0); /* 0x1164 */
    Func_0808a010(0x10); /* 0x116a */
    Func_0808a130(0, 1); /* 0x1172 */
    Func_0808a130(1, 1); /* 0x117a */
    Func_0808a130(0xb, 1); /* 0x1182 */
    Func_0808a130(0xc, 1); /* 0x118a */
    Func_0808a130(9, 1); /* 0x1192 */
    Func_0808a130(0xa, 1); /* 0x119a */
    Func_02002750(8, 0x1200); /* 0x11a6 */
    Func_0808a010(8); /* 0x11ae */
    Func_0808a010(8); /* 0x11bc */
    Func_02002750(8, 0x1200); /* 0x11c4 */
    Func_0808a010(0x20); /* 0x11ca */
    Func_02002750(8, 0x1200); /* 0x11d2 */
    Func_0808a010(0x80); /* 0x11d8 */
    Func_0808a158(8, 0); /* 0x11e0 */
    Func_0808a010(0x1e); /* 0x11e6 */
    Func_02002660(); /* 0x11ea */
    Func_0808a100(8, 1); /* 0x11f2 */
    Func_0808a138(8, 2); /* 0x11fa */
    Func_0808a010(0x1e); /* 0x1200 */
    Func_0808a1e8(0xc, 0x105, 0); /* 0x120a */
    Func_0808a010(0x14); /* 0x1210 */
    Func_0808a150(9, 0xa, 0); /* 0x121a */
    Func_0808a010(0x28); /* 0x1220 */
    Func_0808a148(9, 8, 0); /* 0x122a */
    Func_0808a148(0xa, 8, 0); /* 0x1234 */
    Func_0808a180(0xc, 0); /* 0x123c */
    Func_0808a010(0x14); /* 0x1242 */
    Func_0808a138(0xb, 2); /* 0x124a */
    Func_0808a010(0x14); /* 0x1250 */
    Func_0808a180(0xb, 0); /* 0x1258 */
    Func_0808a010(0x14); /* 0x125e */
    Func_0808a130(0, 1); /* 0x1266 */
    Func_0808a130(1, 1); /* 0x126e */
    Func_0808a150(0, 1, 0); /* 0x1278 */
    Func_0808a130(9, 1); /* 0x1280 */
    Func_0808a130(0xa, 1); /* 0x1288 */
    Func_0808a150(9, 0xa, 0); /* 0x1292 */
    Func_0808a1f0(0xc, 0x102); /* 0x129c */
    Func_0808a010(0x3c); /* 0x12a2 */
    Func_0808a180(8, 0); /* 0x12aa */
    Func_0808a010(0x14); /* 0x12b0 */
    Func_0808a148(0, 8, 0); /* 0x12ba */
    Func_0808a148(1, 8, 0); /* 0x12c4 */
    Func_0808a148(9, 8, 0); /* 0x12ce */
    Func_0808a148(0xa, 8, 0); /* 0x12d8 */
    Func_0808a010(0x14); /* 0x12de */
    Func_0808a180(1, 0); /* 0x12e6 */
    Func_0808a010(0x14); /* 0x12ec */
    Func_0808a110(8, 3); /* 0x12f4 */
    Func_0808a010(0x14); /* 0x12fa */
    Func_0808a180(8, 0); /* 0x1302 */
    Func_0808a010(0x14); /* 0x1308 */
    Func_0808a1e8(0, 0x101, 0); /* 0x1312 */
    Func_0808a1e8(1, 0x101, 0); /* 0x131c */
    Func_0808a1e8(0xb, 0x101, 0); /* 0x1326 */
    Func_0808a1e8(0xc, 0x101, 0); /* 0x1330 */
    Func_0808a1e8(9, 0x101, 0); /* 0x133a */
    Func_0808a1e8(0xa, 0x101, 0); /* 0x1344 */
    Func_0808a010(0x3c); /* 0x134a */
    Func_0808a110(8, 4); /* 0x1352 */
    Func_0808a010(0x14); /* 0x1358 */
    Func_0808a180(8, 0); /* 0x1360 */
    Func_0808a010(0x14); /* 0x1366 */
    Func_0808a100(0, 3); /* 0x136e */
    Func_0808a100(1, 3); /* 0x1376 */
    Func_0808a100(0xb, 3); /* 0x137e */
    Func_0808a100(0xc, 3); /* 0x1386 */
    Func_0808a100(9, 3); /* 0x138e */
    Func_0808a100(0xa, 3); /* 0x1396 */
    Func_0808a118(0xa); /* 0x139c */
    Func_0808a010(0x14); /* 0x13a2 */
    Func_0808a138(0xc, 1); /* 0x13aa */
    Func_0808a010(0x14); /* 0x13b0 */
    Func_0808a180(0xc, 0); /* 0x13b8 */
    Func_0808a010(0x14); /* 0x13be */
    Func_0808a148(8, 0xc, 0); /* 0x13c8 */
    Func_0808a010(0x14); /* 0x13ce */
    Func_0808a110(8, 3); /* 0x13d6 */
    Func_0808a010(0x14); /* 0x13dc */
    Func_0808a180(8, 0); /* 0x13e4 */
    Func_0808a010(0x32); /* 0x13ea */
    Func_0808a148(8, 0, 0); /* 0x13f4 */
    Func_0808a010(0x14); /* 0x13fa */
    Func_0808a180(8, 0); /* 0x1402 */
    Func_0808a010(0x1e); /* 0x1408 */
    Func_0808a1e8(0, 0x102, 0); /* 0x1414 */
    Func_0808a1e8(1, 0x102, 0); /* 0x1420 */
    Func_0808a1e8(0xb, 0x102, 0); /* 0x142c */
    Func_0808a1e8(0xc, 0x102, 0); /* 0x1438 */
    Func_0808a1e8(9, 0x102, 0); /* 0x1444 */
    Func_0808a1e8(0xa, 0x102, 0); /* 0x1450 */
    Func_0808a010(0x50); /* 0x1456 */
    Func_0808a138(0xb, 1); /* 0x145e */
    Func_0808a010(0x14); /* 0x1464 */
    Func_0808a180(0xb, 0); /* 0x146c */
    Func_0808a010(0x14); /* 0x1472 */
    Func_0808a110(8, 4); /* 0x147a */
    Func_0808a010(0x14); /* 0x1480 */
    Func_0808a1e8(0, 0x101, 0); /* 0x148a */
    Func_0808a1e8(1, 0x101, 0); /* 0x1494 */
    Func_0808a1e8(0xb, 0x101, 0); /* 0x149e */
    Func_0808a1e8(0xc, 0x101, 0); /* 0x14a8 */
    Func_0808a1e8(9, 0x101, 0); /* 0x14b2 */
    Func_0808a1e8(0xa, 0x101, 0); /* 0x14bc */
    Func_0808a010(0x50); /* 0x14c2 */
    Func_0808a138(8, 1); /* 0x14ca */
    Func_0808a010(0x14); /* 0x14d0 */
    Func_0808a180(8, 0); /* 0x14d8 */
    Func_0808a010(0x14); /* 0x14de */
    Func_0808a1f0(0, 0x102); /* 0x14e8 */
    Func_0808a1f0(1, 0x102); /* 0x14f2 */
    Func_0808a1f0(0xb, 0x102); /* 0x14fc */
    Func_0808a1f0(0xc, 0x102); /* 0x1506 */
    Func_0808a1f0(9, 0x102); /* 0x1510 */
    Func_0808a1f0(0xa, 0x102); /* 0x151a */
    Func_0808a010(0x3c); /* 0x1520 */
    Func_0808a110(0xc, 4); /* 0x1528 */
    Func_0808a010(0x14); /* 0x152e */
    Func_0808a180(0xc, 0); /* 0x1536 */
    Func_0808a010(0x14); /* 0x153c */
    Func_0808a138(8, 1); /* 0x1544 */
    Func_0808a010(0x14); /* 0x154a */
    Func_0808a110(8, 4); /* 0x1552 */
    Func_0808a010(0x14); /* 0x1558 */
    Func_0808a180(8, 0); /* 0x1560 */
    Func_0808a010(0x28); /* 0x1566 */
    Func_0808a150(0, 1, 0); /* 0x1570 */
    Func_0808a150(9, 0xa, 0); /* 0x157a */
    Func_0808a148(0xc, 0, 0); /* 0x1584 */
    Func_0808a010(0x3c); /* 0x158a */
    Func_0808a148(0, 8, 0); /* 0x1594 */
    Func_0808a148(1, 8, 0); /* 0x159e */
    Func_0808a148(0xb, 8, 0); /* 0x15a8 */
    Func_0808a148(0xc, 8, 0); /* 0x15b2 */
    Func_0808a148(9, 8, 0); /* 0x15bc */
    Func_0808a148(0xa, 8, 0); /* 0x15c6 */
    Func_0808a010(0x14); /* 0x15cc */
    Func_0808a138(0xb, 1); /* 0x15d4 */
    Func_0808a010(0x14); /* 0x15da */
    Func_0808a180(0xb, 0); /* 0x15e2 */
    Func_0808a010(0x3c); /* 0x15e8 */
    Func_0808a0d0(8, 0xc8, 0x88); /* 0x15f2 */
    Func_0808a148(0, 0, 0); /* 0x1608 */
    Func_0808a148(0xc, 0, 0); /* 0x1612 */
    Func_0808a150(8, 1, 0); /* 0x161c */
    Func_0808a010(0x28); /* 0x1622 */
    Func_0808a110(8, 3); /* 0x162a */
    Func_0808a010(0x32); /* 0x1630 */
    Func_0808a148(8, 0, 0); /* 0x163a */
    Func_0808a010(0x28); /* 0x1640 */
    Func_0808a110(8, 3); /* 0x1648 */
    Func_0808a010(0x32); /* 0x164e */
    Func_0808a138(8, 1); /* 0x1656 */
    Func_0808a010(0x14); /* 0x165c */
    Func_0808a180(8, 0); /* 0x1664 */
    Func_0808a010(0x1e); /* 0x166a */
    Func_0808a1e8(0, 0x102, 0); /* 0x1676 */
    Func_0808a1e8(1, 0x102, 0); /* 0x1682 */
    Func_0808a010(0x3c); /* 0x1688 */
    Func_0808a1f0(0xb, 0x102); /* 0x1692 */
    Func_0808a138(0xb, 1); /* 0x169a */
    Func_0808a180(0xb, 0); /* 0x16a2 */
    Func_0808a010(0x1e); /* 0x16a8 */
    Func_0808a110(8, 3); /* 0x16b0 */
    Func_0808a010(0x1e); /* 0x16b6 */
    Func_0808a1e8(0xc, 0x102, 0); /* 0x16c2 */
    Func_0808a130(0xc, 1); /* 0x16ca */
    Func_0808a010(0x14); /* 0x16d0 */
    Func_0808a180(0xc, 0); /* 0x16d8 */
    Func_0808a010(0x1e); /* 0x16de */
    Func_0808a0d0(8, 0xa8, 0x78); /* 0x16e8 */
    Func_0808a148(0, 8, 0); /* 0x16f2 */
    Func_0808a148(1, 8, 0); /* 0x16fc */
    Func_0808a148(0xc, 8, 0); /* 0x1706 */
    Func_0808a1b8(8, 0xd000, 0); /* 0x1712 */
    Func_0808a010(0x14); /* 0x1718 */
    Func_0808a110(8, 4); /* 0x1720 */
    Func_0808a010(0x14); /* 0x1726 */
    Func_0808a180(8, 0); /* 0x172e */
    Func_0808a010(0x1e); /* 0x1734 */
    Func_0808a1e8(0, 0x102, 0); /* 0x1740 */
    Func_0808a1e8(1, 0x102, 0); /* 0x174c */
    Func_0808a010(0x3c); /* 0x1752 */
    Func_0808a110(8, 3); /* 0x175a */
    Func_0808a010(0xa); /* 0x1760 */
    Func_0808a180(8, 0); /* 0x1768 */
    Func_0808a010(0xa); /* 0x176e */
    Func_0808a148(8, 0, 0); /* 0x1778 */
    Func_0808a010(0x14); /* 0x177e */
    Func_0808a180(8, 0); /* 0x1786 */
    Func_0808a010(0x14); /* 0x178c */
    Func_0808a148(0xc, 0, 0); /* 0x1796 */
    Func_0808a1f0(1, 0x102); /* 0x17a0 */
    Func_0808a138(1, 2); /* 0x17a8 */
    Func_0808a148(1, 0, 0); /* 0x17b2 */
    Func_0808a010(0x14); /* 0x17b8 */
    Func_0808a180(1, 0); /* 0x17c0 */
    Func_0808a010(0x1e); /* 0x17c6 */
    Func_0808a170(0x1162); /* 0x17cc */
    Func_0808a138(8, 1); /* 0x17d4 */
    Func_0808a010(0x14); /* 0x17da */
    Func_0808a178(8, 0); /* 0x17e2 */
    if (Func_0808a070(0, 0) == 0) {
        Func_0200187c(); /* 0x17f2 */
        Func_0808a330(0, 0); /* 0x17fa */
        Func_0808a348(0x78); /* 0x1800 */
        Func_0808a010(0x78); /* 0x1806 */
        Func_080f9010(0x56); /* 0x180c */
        Func_080f9078(); /* 0x1810 */
        Func_080770c8(0x9f0); /* 0x1818 */
        Func_0808a248(0x1e); /* 0x181e */
    } else {
        Func_0808a010(0x1e); /* 0x1826 */
        Func_0808a138(0xc, 1); /* 0x182e */
        Func_0808a110(0xc, 4); /* 0x1836 */
        Func_0808a010(0x14); /* 0x183c */
        Func_0808a180(0xc, 0); /* 0x1844 */
        actor[0x23] &= (u8)~2;
        Func_0808a010(0x14); /* 0x184a */
        actor = Func_0808a080(0xa); /* 0x1850 */
        actor = Func_0808a080(0xa); /* 0x1860 */
        *(u32 *)(actor + 0x6c) = 0x02008849;
    }
    Func_0808a020(); /* 0x1868 */
}
