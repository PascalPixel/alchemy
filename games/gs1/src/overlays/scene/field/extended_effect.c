#include "types.h"
#include "scene.h"

#include "facing_object.h"

typedef struct {
    u8 filler0[6];
    u16 unk6;
    u8 filler8[72];
    u8 *unk50;
} Ent;

typedef struct {
    u8 filler0[6];
    u16 unk6;
} Ent;

typedef struct {
    u8 unk[9];
    u8 unk0 : 2;
    u8 mode : 2;
    u8 unk4 : 4;
} SpriteMode;

typedef struct {
    u8 filler0[12];
    s32 unkC;
    u8 filler10[8];
    s32 unk18;
    s32 unk1C;
    u8 filler20[28];
    s32 unk3C;
} Ent2;

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
    u8 filler14[16];
    s32 unk24;
    u8 filler28[4];
    s32 unk2C;
    u8 filler30[8];
    s32 unk38;
    u8 filler3C[4];
    s32 unk40;
} Ent3;

/* resource_381 owner at 0x02002e0c (80 bytes), the twin of resource_371:3fb4.
 * Ported from that byte-exact owner: same field layout, same two calls. */
struct Actor {
    u8 filler00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c, only ever advanced by 0x10000 */
    s32 z;                          /* 0x10 */
    u8 filler14[4];
    s32 amplitude_x;                /* 0x18 */
    s32 amplitude_y;                /* 0x1c */
    u8 filler20[0x44];
    u16 frame;                      /* 0x64 */
    u8 filler66[2];
    struct Actor *anchor;  /* 0x68 */
};

/* resource_381 owner at 0x02002e5c (82 bytes), the twin of resource_371:4004.
 * Ported from that byte-exact owner: same field layout, same two calls. */
struct Actor2 {
    u8 filler00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c, only ever advanced by 0x10000 */
    s32 z;                          /* 0x10 */
    u8 filler14[4];
    s32 amplitude_x;                /* 0x18 */
    s32 amplitude_y;                /* 0x1c */
    u8 filler20[0x44];
    u16 frame;                      /* 0x64 */
    u8 filler66[2];
    struct Actor2 *anchor;  /* 0x68 */
};

extern u8 *gIw[];
extern s16 gCell[];
extern s32 gOv;
extern s32 gOv2;
extern s32 gOv3;
extern s32 gOv4;
extern s32 gOv5[];
extern s32 gIw2;
extern s32 gOv6[][2];
extern s32 gOv7[];

u8 *Field_unk19_4(s32);

void *Field_unk20_4();

void *Field_unk21_4();

u8 *Field_unk22_4(s32);

u8 *Field_unk23_4(s32);

Ent *Field_unk24_4(s32);

u8 *Field_unk25_4(s32, s32, s32, s32);

Ent *Field_unk26_4(s32);

/*
 * Table getters published from the header words of overlay resource_381.
 * Only one overlay is resident at 0x02000000 at a time, so an address that
 * matches another overlay's names different bytes, not the same table.
 */

/* Set up the scene actors, run the two presentation phases, and dismiss the
 * actors through the shared exit sequence. Retained pending an exact build. */

static __inline__ s32 InstallTask(s32 (*f)(void (*)(), s32), void (*callback)(), s32 key)
{
    void Field_unk27_4();
    void Field_unk28_4();
    void Field_unk29_4();
    void Field_unk30_4();

    return f(callback, key);
}

/*
 * Returns the in-image table address 0x0200b6d4, loaded and returned
 * without being dereferenced. The eight-byte owner includes its one pool
 * word, which sits past the bx lr. Published from the overlay header, not
 * reached by any call.
 */
u8 *SceneData_GetTableB6D4(void)
{
    return (u8 *)0x0200b6d4;
}

s32 Field_unk31_4(void)
{
    return 0;
}

/*
 * Returns the in-image table address 0x0200b704, loaded and returned
 * without being dereferenced. The eight-byte owner includes its one pool
 * word, which sits past the bx lr. Published from the overlay header, not
 * reached by any call.
 */
u8 *SceneData_GetTableB704(void)
{
    return (u8 *)0x0200b704;
}

/*
 * Returns the in-image table address 0x0200b710, loaded and returned
 * without being dereferenced. The eight-byte owner includes its one pool
 * word, which sits past the bx lr. Published from the overlay header, not
 * reached by any call.
 */
u8 *SceneData_GetTableB710(void)
{
    return (u8 *)0x0200b710;
}

/*
 * Returns the in-image table address 0x0200b998, loaded and returned
 * without being dereferenced. The eight-byte owner includes its one pool
 * word, which sits past the bx lr. Published from the overlay header, not
 * reached by any call.
 */
u8 *SceneData_GetTableB998(void)
{
    return (u8 *)0x0200b998;
}

void Scene_RunTwoPhaseActorSequence(void)
{
    void Field_unk27_4();
    void Field_unk28_4();
    void Field_unk29_4();
    void Field_unk30_4();

    u32 i;
    u8 *record;
    s32 base5_3001ec4;
    s32 base5_3001ebc;
    u8 *p7;

    base5_3001ec4 = (s32)gIw;
    p7 = *(u8 **)base5_3001ec4;
    Field_unk32_4();
    *(s32 *)(((s32)p7 + 0x40c)) = 0;
    Field_unk21_4(141);
    Field_Place(0x10000, 0x10000, 0x10000);
    Field_unk2_3(0, 0xe80000, 0x9c0000);
    Field_unk3_3(1, 0xda0000, 0xac0000);
    Field_unk4_3(0, 0xb000, 0);
    Field_unk5_3(1, 0xd000, 0);
    Field_unk6_3(5, 0x1db0000, 0x14c0000);
    Field_unk7_3(9, 0x1eb0000, 0x14c0000);
    Field_unk8_3(11, 0x1cb0000, 0x15c0000);
    Field_unk9_3(10, 0x1fb0000, 0x15c0000);
    Field_unk10_3(13, 0x1d70000, 0x1320000);
    Field_unk11_3(14, 0x1df0000, 0x16a0000);
    Field_unk33_4(15, 1);
    base5_3001ebc = base5_3001ec4 - 8;
    Field_Run(0xe80000, -1, 0x9c0000, 0);
    Field_unk34_4();
    *(s32 *)((*(s32 *)base5_3001ebc + 0x1c8)) = 8;
    Field_unk35_4();
    Field_unk36_4();
    Field_unk2_4(0x7fff, 0);
    Field_unk37_4(4);
    Field_unk38_4(4);
    Field_unk3_4(0x10000, 0);
    Field_unk39_4(4);
    Field_unk40_4(4);
    Field_unk20_4();
    Field_unk4_4(0x7fff, 0);
    Field_unk41_4(4);
    Field_unk42_4(16);
    Field_unk30_4(144);
    Field_unk5_4(0x10000, 0);
    Field_unk43_4(4);
    Field_unk44_4(4);
    Field_unk6_4(0x7fff, 0);
    Field_unk45_4(4);
    Field_unk46_4(4);
    Field_unk47_4(144);
    Field_unk7_4(0x10000, 0);
    Field_unk29_4(48);
    Field_unk48_4(48);
    Field_unk49_4(0, 6, 0);
    Field_unk50_4(1, 6, 20);
    Field_unk51_4(1, 20, 20);
    Field_unk52_4(0, 20, 40);
    Field_Do(0x10cd);
    Field_unk12_3(11, 0, 20);
    Field_unk8_4(10, 0);
    Field_unk53_4(1, 20, 0);
    Field_unk54_4(5, 0, 20);
    Field_unk55_4(0, 20, 0);
    Field_unk56_4(14, 0, 20);
    Field_unk57_4(9, 0, 20);
    Field_unk58_4(0, 1, 0);
    Field_unk59_4(40);
    Field_unk60_4(0, 2);
    Field_unk61_4(1, 2);
    Field_unk62_4(20);
    Field_unk13_3(0, 0xb000, 0);
    Field_unk14_3(1, 0xd000, 30);
    Field_unk63_4(1, 20, 0);
    Field_unk64_4(0, 2);
    Field_unk61_4(1, 2);
    Field_unk65_4(20);
    Field_unk66_4(0, 20, 20);
    record = Field_unk19_4(15);
    Field_Apply2(record, 0);
    Field_unk15_3(15, 0x1450000, 0x12e0000);
    record = Field_unk22_4(15);
    record[85] = 5;
    *(s32 *)(((s32)p7 + 0x40c)) = 1;
    Field_unk2_2(0x121);
    Field_unk16_3(-1, -1, 0xe666);
    Field_unk67_4();
    Field_unk68_4(150);
    Field_unk69_4(11, 0, 20);
    Field_unk70_4(5, 0);
    Field_unk71_4(10, 0, 10);
    Field_unk58_4(0, 1, 20);
    Field_unk17_3(0, 0x3000, 0);
    Field_unk18_3(1, 0x3000, 10);
    Field_unk19_3(5, 0x1db0000, 0x14c0000);
    Field_unk15_3(9, 0x1eb0000, 0x14c0000);
    Field_unk20_3(11, 0x1cb0000, 0x15c0000);
    Field_unk21_3(10, 0x1fb0000, 0x15c0000);
    Field_unk22_3(13, 0x1d70000, 0x1320000);
    Field_unk23_3(14, 0x1df0000, 0x16a0000);
    Field_unk9_4(0x66666, 0xcccc);
    Field_unk10_4(0x1480000, -1, 0x12b0000, 1);
    Field_unk72_4();
    Field_unk73_4(167);
    Field_unk11_4(0x205294, 2);
    Field_unk74_4(20);
    Field_unk75_4(20);
    Field_unk12_4(0x10000, 2);
    Field_unk76_4(20);
    Field_unk68_4(200);
    Field_Apply(0x1001, 0);
    if (Field_Check(0, 0) == 0) {
        Field_unk3_2(0x10d6);
    } else {
        Field_unk4_2(0x10d7);
    }
    Field_unk24_3(0x1001, 0, 80);
    Field_unk5_2(0x10d8);
    Field_unk77_4(9, 0, 20);
    Field_unk78_4(1, 20, 0);
    *(s32 *)(((s32)p7 + 0x40c)) = 0;
    Field_unk79_4(141);
    Field_unk25_3(0x10000, 0x20000, 0x10000);
    Field_unk80_4(80);
    *(s32 *)(((s32)p7 + 0x40c)) = 1;
    Field_unk6_2(0x121);
    Field_unk26_3(-1, -1, 0xe666);
    Field_unk81_4();
    Field_unk82_4(0, 20, 60);
    Field_unk83_4(14, 0, 30);
    Field_unk27_3(15, 0xa000, 40);
    Field_unk84_4(1, 20, 20);
    Field_unk85_4(0, 20, 20);
    Field_unk28_3(0x1001, 0, 30);
    Field_unk29_3(15, 0x1000, 40);
    Field_unk86_4(1, 20, 20);
    Field_unk87_4(0, 20, 20);
    Field_unk88_4(5, 0, 30);
    Field_unk89_4();
    InstallTask(Field_Apply3, (void (*)())0x020090c5, 0xc80);
    InstallTask(Field_Apply4, (void (*)())0x0200935d, 0xc80);
    Field_unk90_4(240);
    Field_unk91_4(10, 0, 30);
    Field_unk30_3(5, 0x1db0000, 0x14c0000);
    Field_unk31_3(9, 0x1eb0000, 0x14c0000);
    Field_unk32_3(11, 0x1cb0000, 0x15c0000);
    Field_unk33_3(10, 0x1fb0000, 0x15c0000);
    Field_unk34_3(13, 0x1d70000, 0x1320000);
    Field_unk35_3(14, 0x1df0000, 0x16a0000);
    Field_unk36_3(5, 0x8000, 0);
    Field_unk37_3(9, 0x8000, 0);
    Field_unk38_3(11, 0x8000, 0);
    Field_unk39_3(10, 0x8000, 0);
    Field_unk40_3(13, 0x8000, 0);
    Field_unk41_3(14, 0x8000, 0);
    record = Field_unk19_4(5);
    Field_unk13_4((*(s16 *)((s32)record + 10) << 16), -1, (*(s16 *)((s32)record + 18) << 16), 1);
    Field_unk92_4();
    Field_unk93_4(40);
    Field_unk94_4(13, 2);
    Field_unk95_4(20);
    Field_unk96_4(13, 0);
    Field_unk97_4(11, 2);
    Field_unk98_4(11, 0, 10);
    Field_unk99_4(14, 4);
    Field_unk100_4(14, 0, 20);
    Field_unk101_4(10, 11, 40);
    Field_unk102_4(10, 4);
    Field_unk103_4(10, 0, 10);
    Field_unk42_3(5, 0x100, 0);
    Field_unk43_3(5, 0x3000, 40);
    Field_unk44_3(0x4005, 0, 10);
    Field_unk45_3(10, 0xb000, 0);
    Field_unk46_3(11, 0xd000, 40);
    Field_unk104_4(10, 11, 40);
    Field_unk47_3(10, 0x105, 40);
    Field_unk48_3(10, 0xb000, 10);
    Field_unk105_4(10, 0, 10);
    Field_unk49_3(10, 0x8000, 10);
    Field_unk106_4(11, 2);
    Field_unk107_4(11, 0, 10);
    Field_unk108_4(10, 3);
    Field_unk109_4(10, 0, 40);
    Field_unk50_3(14, 0xb000, 60);
    Field_unk110_4(14, 3);
    Field_unk69_4(14, 0, 10);
    Field_unk51_3(13, 0x3000, 0);
    Field_unk52_3(5, 0x3000, 0);
    Field_unk53_3(9, 0x5000, 0);
    Field_unk54_3(11, 0x3000, 0);
    Field_unk55_3(10, 0x5000, 20);
    Field_unk56_3(14, 0xd000, 20);
    Field_unk111_3(14, 4);
    Field_unk112_3(14, 0, 10);
    Field_unk57_3(13, 0x103, 0);
    Field_unk58_3(13, 0x10000, 0x8000);
    Field_unk59_3(13, 0x1d7, 0x13a);
    Field_unk60_4(13, 3);
    Field_unk69_4(13, 0, 10);
    Field_unk60_3(14, 0xb000, 10);
    Field_unk113_3(14, 4);
    Field_unk114_3(14, 0, 10);
    Field_unk115_3(13, 3);
    Field_unk116_3(13, 0, 10);
    Field_unk61_4(9, 2);
    Field_unk61_3(0x4009, 0, 10);
    Field_unk62_3(10, 0xb000, 20);
    Field_unk117_3(10, 0, 30);
    Field_unk118_3(11, 0, 20);
    Field_unk119_3(11, 2);
    Field_unk68_4(30);
    Field_unk120_3(11, 3);
    Field_unk68_4(30);
    Field_unk63_3(11, 0xd000, 30);
    Field_unk111_3(11, 4);
    Field_unk121_3(10);
    Field_unk122_3(11, 0, 10);
    Field_unk64_3(13, 0x5000, 20);
    Field_unk123_3(13, 2);
    Field_unk68_4(20);
    Field_unk124_3(10, 11, 0);
    Field_unk125_3(30);
    Field_unk126_3(10, 3);
    Field_unk127_3(11, 3);
    Field_unk65_3(11, 0xd000, 0);
    Field_unk66_3(10, 0xb000, 0);
    Field_unk128_3(40);
    Field_unk129_3(13, 1);
    Field_unk130_3(30);
    Field_unk67_3(13, 0x8000, 0);
    Field_unk131_3(40);
    Field_unk68_3(13, 0x102, 0);
    Field_unk132_3(60);
    Field_unk54_3(14, 0xb000, 0);
    Field_unk133_3(30);
    Field_unk134_3(14, 4);
    Field_unk69_3(0x200e, 0, 30);
    Field_unk70_3(9, 0xb000, 0);
    Field_unk71_3(5, 0xb000, 0);
    Field_unk72_3(13, 0x3000, 20);
    Field_unk73_3(5, 0x102, 40);
    Field_unk74_3(0x2005, 0, 40);
    Field_unk75_3(13, 0x105, 60);
    Field_unk135_3(13, 3);
    Field_unk76_3(10, 0x101, 0);
    Field_unk77_3(11, 0x101, 60);
    Field_unk136_3(14, 2);
    Field_unk69_4(14, 0, 60);
    Field_unk111_3(13, 3);
    Field_unk137_3(40);
    Field_unk78_3(5, 0x102, 0);
    Field_unk79_3(9, 0x102, 40);
    Field_unk58_4(10, 11, 20);
    Field_unk138_3(10, 3);
    Field_unk139_3(11, 3);
    Field_unk68_4(20);
    Field_unk80_3(10, 0x9999, 0x4ccc);
    Field_unk81_3(11, 0x9999, 0x4ccc);
    Field_unk82_3(11, 0x1db, 0x15c);
    Field_unk83_3(10, 0x1eb, 0x15c);
    Field_unk140_3(11);
    Field_unk141_2(10);
    Field_unk142_2(11, 1);
    Field_unk143_2(10, 1);
    Field_unk84_3(11, 0xd000, 0);
    Field_unk85_3(10, 0xb000, 0);
    Field_unk54_3(13, 0x3000, 10);
    Field_unk86_3(11, 0x103, 0);
    Field_unk144_2(11, 2);
    Field_unk14_4(0x200b, 0);
    Field_unk80_3(11, 0x19999, 0xcccc);
    Field_unk87_3(5, 0x13333, 0x9999);
    Field_unk88_3(11, 0x1db, 0x152);
    record = Field_unk23_4(11);
    ((struct FacingObject *)record)->facing_flags = (u8)(254 & ((struct FacingObject *)record)->facing_flags);
    Field_unk89_3(11, 0x1db, 0x15c);
    Field_unk145_2(5, 1);
    Field_unk146_2(5, 4, 0);
    Field_unk90_3(5, 0x1cb, 0x13c);
    Field_unk91_3(5, 0x8000, 0x4000);
    Field_unk92_3(13, 0x103, 0);
    Field_unk147_2(13, 3);
    Field_unk148_2(13, 0, 30);
    Field_unk149_2(11, 1);
    Field_unk150_2(20);
    ((struct FacingObject *)record)->facing_flags |= 1;
    Field_unk93_3(11, 0x10000, 0x8000);
    Field_unk94_3(11, 0x1db, 0x14c);
    Field_unk95_3(11, 0xb000, 20);
    Field_unk15_4(0x200b, 0);
    Field_unk96_3(10, 0xb000, 0);
    Field_unk151_2(10, 2);
    Field_unk69_4(10, 0, 20);
    Field_unk97_3(11, 0x102, 20);
    Field_unk54_3(5, 0x3000, 0);
    Field_unk98_3(11, 0x3000, 20);
    Field_unk152_2(10, 4);
    Field_unk153_2(11, 3);
    Field_unk99_3(11, 0xb000, 20);
    Field_unk154_2(11, 3);
    Field_unk111_3(13, 3);
    Field_unk155_2(20);
    Field_unk100_3(5, 0x1b0, 0x13c);
    Field_unk90_3(13, 0x1a6, 0x137);
    Field_unk156_2(5, 1);
    Field_unk101_3(5, 0x3000, 0);
    Field_unk102_3(13, 0x3000, 20);
    Field_unk157_2(10, 2);
    Field_unk103_3(10, 0xd000, 20);
    Field_unk104_3(0x100a, 0, 40);
    Field_unk158_2(9, 2);
    Field_unk159_2(20);
    Field_unk105_3(9, 0xb000, 30);
    Field_unk106_3(9, 0x8000, 0x4000);
    Field_unk90_3(9, 0x1eb, 0x128);
    Field_unk107_3(9, 0x5000, 40);
    Field_unk108_3(10, 0x10000, 0x8000);
    Field_unk80_3(11, 0x10000, 0x8000);
    Field_unk109_3(13, 0x10000, 0x8000);
    Field_unk110_3(14, 0x10000, 0x8000);
    Field_unk111_2(10, 0x1d7, 0x134);
    Field_unk112_2(10, 0x5000, 0);
    Field_unk113_2(11, 0x1c7, 0x134);
    Field_unk114_2(11, 0x3000, 0);
    Field_unk115_2(14, 0x1e7, 0x134);
    Field_unk116_2(10, 0xc000, 0);
    Field_unk117_2(11, 0xd000, 0);
    Field_unk118_2(14, 0xb000, 0);
    Field_unk119_2(5, 0xd000, 0);
    Field_unk120_2(13, 0xd000, 0);
    Field_unk16_4(0x8000, 0x1000);
    Field_unk17_4(0x1d80000, -1, 0x12c0000, 1);
    Field_unk160_2();
    Field_unk161_2(40);
    Field_unk7_2(0x246);
    Field_unk121_2(10, 0x8000, 0x4000);
    Field_unk122_2(11, 0x8000, 0x4000);
    Field_unk123_2(14, 0x8000, 0x4000);
    Field_unk162_2(10, 3);
    Field_unk163_2(10);
    Field_unk164_2(9, 2);
    Field_unk165_2(9);
    Field_unk111_3(11, 3);
    Field_unk166_2(11);
    Field_unk124_2(5, 0x9000, 40);
    Field_unk60_4(5, 2);
    Field_unk125_2(0x2005, 0, 40);
    Field_unk126_2(13, 0x3000, 0);
    Field_unk127_2(5, 0xb000, 30);
    Field_unk128_2(13, 0xe000, 0);
    Field_unk167_2(5);
    Field_unk168_2(13);
    Field_unk129_2(14, 0x7000, 40);
    Field_unk169_2(14, 0, 30);
    Field_unk170_2(14, 4);
    Field_unk68_4(20);
    Field_unk171_2(14, 0, 30);
    Field_unk172_2(14);
    Field_unk173_2();
    Field_unk174_2(5);
}

void Scene_RunScene381(s32 a0)
{
    s32 p10;
    u8 *rec7;
    s32 recA;
    s32 rec2;
    s32 base6_0;

    p10 = a0;
    recA = Field_unk3(8);
    *(s32 *)(recA + 24) = 0x10000;
    *(s32 *)(recA + 28) = 0x10000;
    Field_unk130_2(p10, 0x1d7, 0x122);
    Field_unk131_2(p10, 0xc000, 0);
    Field_unk175_2(10);
    Field_unk132_2(8, 0x1d70000, 0x1220000);
    rec7 = Field_unk2(p10);
    rec2 = Field_unk4(p10);
    Field_unk176_2(rec2, 0);
    Field_unk18_4(p10, 0x100);
    rec7[85] = 0;
    Field_unk177_2(201);
    base6_0 = 0;
    do {
        *(volatile s32 *)((s32)rec7 + 12) += 0x8000;
        Field_unk178_2(1);
        base6_0 = ((u32)((base6_0 + 1) << 24) >> 24);
    } while (base6_0 != 60);
    Field_unk179_2(190);
    base6_0 = 0;
    do {
        *(volatile s32 *)((s32)rec7 + 12) += 0x1999;
        *(volatile s32 *)((s32)rec7 + 24) += -0x28f;
        *(volatile s32 *)((s32)rec7 + 28) += -0x28f;
        *(volatile s32 *)(recA + 24) += -0x28f;
        *(volatile s32 *)(recA + 28) += -0x28f;
        Field_unk180_2(1);
        base6_0 = ((u32)((base6_0 + 1) << 24) >> 24);
    } while (base6_0 != 90);
    Field_unk181_2(p10, 0, 0);
    Field_unk182_2(8, 0, 0);
}

void Resource381_NoOpCallbackA(void)
{
}

void Resource381_NoOpCallbackB(void)
{
}

s32 Scene_RunWhenWord225Is10(void)
{
    if (gCell[225] == 10) {
        Field_unk183_2();
        Field_unk184_2();
    }
    return 0;
}

s32 OvObj_SetRecordAngleFromHeading(Ent *p)
{
    *(u16 *)(p->unk50 + 30) = p->unk6 + 0x4000;
    return 1;
}

void State_InitStateWordsAndSlots(void)
{
    s32 *p;
    u32 i;

    gOv = 63;
    gOv2 = 0;
    gOv3 = 0;
    gOv4 = 120;
    p = gOv5;
    for (i = 0; i < 16; i++) {
        *p++ = 0;
    }
}

void State_UpdateRandomTimerLevel(void)
{
    u32 v;

    if (gOv4 != 0) {
        gOv4--;
        return;
    }
    if (gOv3 != 0) {
        gOv3--;
    } else {
        gOv3 = (u32)(Field_unk5() << 2) >> 16;
    }
    v = gOv3;
    switch (v) {
    case 3:
        gOv = v;
        gOv4 = ((u32)(Field_unk6() * 20) >> 16) + 40;
        break;
    case 2:
        gOv = 15;
        gOv4 = ((u32)(Field_unk7() * 40) >> 16) + 80;
        break;
    case 1:
        gOv = 63;
        gOv4 = ((u32)(Field_unk8() * 80) >> 16) + 160;
        break;
    default:
        gOv = 127;
        gOv4 = ((u32)(Field_unk9() * 160) >> 16) + 320;
        break;
    }
}

void Actor_MoveTo232_125AndFace4000(s32 no)
{
    Ent *rec;

    rec = Field_unk24_4(no);
    Field_unk134_2(no, 0xe80000, 0x7d0000);
    rec->unk6 = 0x4000;
    Field_Apply5(no, 3);
}

void State_ApplyRectsByCondition(s32 a)
{
    if (a != 0) {
        s32 x;
        s32 y;
        x = 1;
        Field_SetRect(8, 47, 64, 7, x, x);
        y = 2;
        Field_unk2_6(7, 48, 63, 8, y, x);
        Field_unk3_6(7, 49, 63, 9, y, x);
    } else {
        s32 x;
        x = 1;
        Field_unk4_6(56, 0, 64, 7, x, x);
        Field_unk5_6(56, 0, 63, 8, x, x);
        Field_unk6_6(56, 0, 63, 9, 2, x);
        Field_unk7_6(58, 25, 64, 8, x, x);
    }
    Field_unk185_2();
}

void State_ApplyRectPairByFlag(s32 a)
{
    if (a != 0) {
        s32 n;
        n = 2;
        Field_unk8_6(9, 45, 65, 5, n, n);
        Field_unk9_6(11, 46, 67, 6, 1, n);
    } else {
        s32 n;
        n = 2;
        Field_unk10_5(89, 2, 65, 5, n, n);
        Field_unk11_5(102, 32, 67, 6, 1, n);
    }
    Field_unk186_2();
}

void Scene_RunRandomHalfBranch(void)
{
    if ((gIw2 & 1) == 0) {
        if (Field_Apply6(Field_unk10(), 100) > 50) {
            Field_unk8_2(1);
        } else {
            Field_unk9_2(0);
        }
    }
}

void Scene_RunLateRandomHalfBranch(void)
{
    if ((gIw2 & 1) == 0) {
        if (Field_Apply7(Field_unk11(), 100) > 50) {
            Field_unk10_2(1);
        } else {
            Field_unk11_2(0);
        }
    }
}

void Scene_RunFourWayEffectSequence(u32 mode)
{
    extern u8 *gOv8[];
    void Field_unk27_4(s32, s32, s32);
    void Field_unk28_4(s32, s32);
    void Field_unk29_4(s32);
    void Field_unk30_4(s32);

    u32 i, zero;
    s32 x, y, z;
    s32 *pos;
    u8 *obj, *sprite;

    for (i = 0; i <= 15; i++)
        Field_unk12_2(i + 16);
    switch (mode) {
    case 0: Field_unk28_4(0x4039d2, 1); break;
    case 1: Field_unk28_4(0x4049d2, 1); break;
    case 2: Field_unk28_4(0x404a4e, 1); break;
    case 3: Field_unk28_4(0x403a52, 1); break;
    }
    Field_unk29_4(60);
    Field_unk30_4(214);
    i = 0;
    zero = i;
    for (pos = &gOv6[0][0]; i <= 9; i++, pos += 2) {
        x = pos[0];
        y = pos[1];
        z = 0;
        switch (mode) {
        case 0: x += 0xe80000; z = 0x900000; break;
        case 1: x += 0xe80000; z = 0x1d00000; break;
        case 2: x += 0x2c70000; z = 0x900000; break;
        case 3: x += 0x2c70000; z = 0x1d00000; break;
        }
        gOv7[i] = zero;
        obj = Field_unk25_4(284, x, y, z);
        gOv8[i] = obj;
        obj[85] = zero;
        sprite = *(u8 **)(obj + 80);
        sprite[38] = zero;
        ((SpriteMode *)sprite)->mode = 1;
        Field_Apply8(obj, 6);
        Field_unk13_2(6);
    }
    if (mode == 0) {
        Field_unk133_2(0, 256, 0);
        Field_unk133_2(1, 256, 0);
    }
    Field_unk13_2(20);
    Field_Apply9(Effect_AdvanceTenEntryTimers, 3200);
    Field_unk30_4(246);
    gOv7[0] = 1; Field_unk13_2(6);
    gOv7[1] = 1; Field_unk13_2(6);
    gOv7[2] = 1; Field_unk13_2(6);
    gOv7[3] = 1; Field_unk13_2(6);
    gOv7[4] = 1; Field_unk13_2(6);
    gOv7[5] = 1; Field_unk13_2(6);
    gOv7[6] = 1; Field_unk13_2(6);
    gOv7[7] = 1; Field_unk13_2(6);
    gOv7[8] = 1; Field_unk13_2(6);
    gOv7[9] = 1; Field_unk13_2(6);
    for (;;) {
        for (i = 0; i <= 9; i++) {
            if (gOv7[i] != 0) {
                i = 888;
                break;
            }
        }
        if (i != 888)
            break;
        Field_unk13_2(1);
    }
    Field_unk13_2(40);
    Field_unk14_2(Effect_AdvanceTenEntryTimers);
    Field_unk28_4(65536, 1);
    Field_unk29_4(40);
}

void Effect_AdvanceTenEntryTimers(void)
{
    extern Ent *gOv8[];

    u32 i;
    s32 v;
    Ent2 *p;

    for (i = 0; i <= 9; i++) {
        v = gOv7[i];
        if (v != 0) {
            p = gOv8[i];
            if ((u32)v <= 8) {
                p->unk18 += -0x1ccc;
                p->unk1C += 0x8000;
                p->unkC += 0x4ccc;
                p->unk3C += 0x4ccc;
            } else {
                p->unkC += 0x140000;
                p->unk3C += 0x140000;
            }
            v = gOv7[i] + 1;
            gOv7[i] = v;
            if ((u32)v > 14) {
                gOv7[i] = 0;
            }
        }
    }
}

void Actor_PlaceAtTileAndRunSteps(s32 a, s32 b)
{
    Ent3 *p;

    p = Field_unk26_4(a);
    b = b << 16;
    a = a << 16;
    Field_SetMode(a, -1, b, 1);
    Field_Apply10(0, 0);
    Field_unk15_2(20);
    Field_unk16_2(40);
    p->unk10 = b;
    p->unk8 = a;
    p->unk38 = 0x80000000;
    p->unk40 = 0x80000000;
    p->unk24 = 0;
    p->unk2C = 0;
    Field_unk17_2(5);
    Field_unk187_2();
    Field_unk18_2(5);
    Field_Apply11(0x10000, 0);
    Field_unk19_2(20);
    Field_unk20_2(30);
}

void State_ConfigureEightCornerRegions(void)
{
    s32 x;
    s32 y;
    s32 z;
    s32 w;
    s32 v;

    x = 0;
    Field_unk12_5(14, 8, 1, 1, 10, x);
    Field_unk13_5(14, 28, 1, 1, 11, x);
    Field_unk14_5(44, 8, 1, 1, 12, x);
    Field_unk15_5(44, 28, 1, 1, 13, x);
    z = 14;
    y = 8;
    Field_unk16_5(13, 8, 1, 1, z, y);
    w = 28;
    Field_unk17_5(13, 28, 1, 1, z, w);
    v = 44;
    Field_unk18_5(43, 8, 1, 1, v, y);
    Field_unk19_5(43, 28, 1, 1, v, w);
}

void Scene_RunVariantStep(s32 a, s32 b, s32 c)
{
    if (a == 1) {
        Field_unk21_2(0x134);
        Field_Apply12(0x203a52, 1);
    } else {
        Field_unk22_2(0x121);
        Field_Apply13(0x10000, 1);
    }
    Field_unk23_2(b);
    if (c != 0) {
        Field_unk24_2(c);
    }
}

void Scene_RunStepByRuntimeBits(s32 a)
{
    if ((gIw2 & 2) != 0) {
        Field_Apply14(a, 7);
    } else {
        Field_Apply15(a, 0);
    }
    if ((gIw2 & 15) == 0) {
        Field_unk25_2(a);
    }
}

void State_ForwardByRuntimeWordBits(s32 a)
{
    volatile u32 *p = (u32 *)0x03001e40;

    if (*p & 1) {
        Field_Apply16(a, Field_Apply17(*p >> 1, 6));
    }
    if ((*p & 15) == 0) {
        Field_unk26_2(a);
    }
}

void Effect_UpdateArcOverAnchor(struct Actor *self)
{
    struct Actor *anchor;
    s32 frame;
    s32 amplitude;

    anchor = self->anchor;
    self->frame = (u16)(self->frame + 1);
    frame = (s16)self->frame;

    if (frame > 31) {
        Field_unk188_2(self);
        return;
    }

    amplitude = Field_unk12(frame << 10);
    self->amplitude_x = amplitude;
    self->amplitude_y = amplitude;
    self->x = anchor->x;
    self->y += 0x10000;
    self->z = anchor->z + (0x10000 - amplitude) * 5 + 0x80000;
}

void Effect_UpdateAnchoredRiseArc(struct Actor2 *obj)
{
    struct Actor2 *anchor;
    s32 frame;
    s32 amp;

    anchor = obj->anchor;
    obj->frame = (u16)(obj->frame + 1);
    frame = (s16)obj->frame;

    if (frame > 31) {
        Field_unk189_2(obj);
        return;
    }

    amp = Field_unk13(frame << 10);
    obj->amplitude_x = amp;
    obj->amplitude_y = -amp;
    obj->x = anchor->x;
    obj->y += 0x10000;
    obj->z = anchor->z - (0x10000 - amp) * 5 + 0x100000;
}

void State_SetValue140Mode0(void)
{
    Field_Apply18(140, 0);
}

void Scene_CallHelper(void)
{
    Field_unk190_2();
}

void Scene_RunActor15TwoStep(void)
{
    Field_unk27_2(15);
    Field_unk191_2();
}
