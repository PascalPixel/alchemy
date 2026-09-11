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

u8 *Field_unk_0200b4ec(s32);

void *Field_unk_020010bc();

void *Field_unk_0200368e();

u8 *Field_unk_0200379c(s32);

u8 *Field_unk_02003f4e(s32);

Ent *Field_unk_02005d12(s32);

u8 *Field_unk_0200b454(s32, s32, s32, s32);

Ent *Field_unk_020061fc(s32);

/*
 * Table getters published from the header words of overlay resource_381.
 * Only one overlay is resident at 0x02000000 at a time, so an address that
 * matches another overlay's names different bytes, not the same table.
 */

/* Set up the scene actors, run the two presentation phases, and dismiss the
 * actors through the shared exit sequence. Retained pending an exact build. */

static __inline__ s32 InstallTask(s32 (*f)(void (*)(), s32), void (*callback)(), s32 key)
{
    void Field_unk_0200b5ac();
    void Field_unk_0200b5ec();
    void Field_unk_0200b5f4();
    void Field_TestFlag121();

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

s32 get_default_result(void)
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
    void Field_unk_0200b5ac();
    void Field_unk_0200b5ec();
    void Field_unk_0200b5f4();
    void Field_TestFlag121();

    u32 i;
    u8 *record;
    s32 base5_3001ec4;
    s32 base5_3001ebc;
    u8 *p7;

    base5_3001ec4 = (s32)gIw;
    p7 = *(u8 **)base5_3001ec4;
    Field_unk_02003530();
    *(s32 *)(((s32)p7 + 0x40c)) = 0;
    Field_unk_0200368e(141);
    Field_unk_0200350e(0x10000, 0x10000, 0x10000);
    Field_unk_020035bc(0, 0xe80000, 0x9c0000);
    Field_unk_020035ca(1, 0xda0000, 0xac0000);
    Field_unk_0200363e(0, 0xb000, 0);
    Field_unk_0200364a(1, 0xd000, 0);
    Field_unk_020035ee(5, 0x1db0000, 0x14c0000);
    Field_unk_020035fa(9, 0x1eb0000, 0x14c0000);
    Field_unk_02003606(11, 0x1cb0000, 0x15c0000);
    Field_unk_02003612(10, 0x1fb0000, 0x15c0000);
    Field_unk_0200361e(13, 0x1d70000, 0x1320000);
    Field_unk_0200362a(14, 0x1df0000, 0x16a0000);
    Field_unk_020036a2(15, 1);
    base5_3001ebc = base5_3001ec4 - 8;
    Field_unk_020036d6(0xe80000, -1, 0x9c0000, 0);
    Field_unk_0200357a();
    *(s32 *)((*(s32 *)base5_3001ebc + 0x1c8)) = 8;
    Field_unk_0200b5fc();
    Field_unk_0200372e();
    Field_unk_0200371e(0x7fff, 0);
    Field_unk_0200372c(4);
    Field_unk_0200360a(4);
    Field_unk_02003734(0x10000, 0);
    Field_unk_02003742(4);
    Field_unk_02003620(4);
    Field_unk_020010bc();
    Field_unk_0200374c(0x7fff, 0);
    Field_unk_0200375a(4);
    Field_unk_02003638(16);
    Field_TestFlag121(144);
    Field_unk_02003768(0x10000, 0);
    Field_unk_02003776(4);
    Field_unk_02003654(4);
    Field_unk_0200377c(0x7fff, 0);
    Field_unk_0200378a(4);
    Field_unk_02003668(4);
    Field_unk_020037c6(144);
    Field_unk_0200b5ec(0x10000, 0);
    Field_unk_0200b5f4(48);
    Field_unk_02003684(48);
    Field_unk_0200370e(0, 6, 0);
    Field_unk_02003718(1, 6, 20);
    Field_unk_02002f1e(1, 20, 20);
    Field_unk_02002f28(0, 20, 40);
    Field_unk_02003762(0x10cd);
    Field_unk_0200b594(11, 0, 20);
    Field_unk_0200b58c(10, 0);
    Field_unk_02002f4a(1, 20, 0);
    Field_unk_020037a0(5, 0, 20);
    Field_unk_02002f5e(0, 20, 0);
    Field_unk_020037b4(14, 0, 20);
    Field_unk_020037be(9, 0, 20);
    Field_unk_0200b564(0, 1, 0);
    Field_unk_02003706(40);
    Field_unk_0200b554(0, 2);
    Field_unk_0200b55c(1, 2);
    Field_unk_0200371c(20);
    Field_unk_020037f8(0, 0xb000, 0);
    Field_unk_02003804(1, 0xd000, 30);
    Field_unk_02002fba(1, 20, 0);
    Field_unk_020037ce(0, 2);
    Field_unk_0200b55c(1, 2);
    Field_unk_02003754(20);
    Field_unk_02002fda(0, 20, 20);
    record = Field_unk_0200b4ec(15);
    Field_unk_0200b484(record, 0);
    Field_unk_0200b534(15, 0x1450000, 0x12e0000);
    record = Field_unk_0200379c(15);
    record[85] = 5;
    *(s32 *)(((s32)p7 + 0x40c)) = 1;
    Field_TestFlag121(0x121);
    Field_unk_02003764(-1, -1, 0xe666);
    Field_unk_02003770();
    Field_unk_0200b4cc(150);
    Field_unk_0200b594(11, 0, 20);
    Field_unk_0200b58c(5, 0);
    Field_unk_0200388a(10, 0, 10);
    Field_unk_0200b564(0, 1, 20);
    Field_unk_020038a8(0, 0x3000, 0);
    Field_unk_020038b4(1, 0x3000, 10);
    Field_unk_02003858(5, 0x1db0000, 0x14c0000);
    Field_unk_0200b534(9, 0x1eb0000, 0x14c0000);
    Field_unk_02003870(11, 0x1cb0000, 0x15c0000);
    Field_unk_0200387c(10, 0x1fb0000, 0x15c0000);
    Field_unk_02003888(13, 0x1d70000, 0x1320000);
    Field_unk_02003894(14, 0x1df0000, 0x16a0000);
    Field_unk_02003924(0x66666, 0xcccc);
    Field_unk_0200393c(0x1480000, -1, 0x12b0000, 1);
    Field_unk_02003948();
    Field_unk_020039a6(167);
    Field_unk_02003976(0x205294, 2);
    Field_unk_02003984(20);
    Field_unk_0200377a(20);
    Field_unk_0200398c(0x10000, 2);
    Field_unk_0200399a(20);
    Field_unk_0200b4cc(200);
    Field_unk_02003938(0x1001, 0);
    if (Field_Check(0, 0) == 0) {
        Field_unk_02003942(0x10d6);
    } else {
        Field_unk_02003998(0x10d7);
    }
    Field_unk_020039ba(0x1001, 0, 80);
    Field_unk_020039a8(0x10d8);
    Field_unk_020039ca(9, 0, 20);
    Field_unk_02003188(1, 20, 0);
    *(s32 *)(((s32)p7 + 0x40c)) = 0;
    Field_unk_02003a72(141);
    Field_unk_020038f2(0x10000, 0x20000, 0x10000);
    Field_unk_02003930(80);
    *(s32 *)(((s32)p7 + 0x40c)) = 1;
    Field_TestFlag121(0x121);
    Field_unk_02003910(-1, -1, 0xe666);
    Field_unk_0200391c();
    Field_unk_020031d2(0, 20, 60);
    Field_unk_02003a28(14, 0, 30);
    Field_unk_02003a3c(15, 0xa000, 40);
    Field_unk_020031f2(1, 20, 20);
    Field_unk_020031fc(0, 20, 20);
    Field_unk_02003a52(0x1001, 0, 30);
    Field_unk_02003a66(15, 0x1000, 40);
    Field_unk_0200321c(1, 20, 20);
    Field_unk_02003226(0, 20, 20);
    Field_unk_02003a7c(5, 0, 30);
    Field_unk_02001570();
    InstallTask(Field_unk_020038e2, (void (*)())0x020090c5, 0xc80);
    InstallTask(Field_unk_020038ec, (void (*)())0x0200935d, 0xc80);
    Field_unk_020039d2(240);
    Field_unk_02003aa4(10, 0, 30);
    Field_unk_02003a50(5, 0x1db0000, 0x14c0000);
    Field_unk_02003a5c(9, 0x1eb0000, 0x14c0000);
    Field_unk_02003a68(11, 0x1cb0000, 0x15c0000);
    Field_unk_02003a74(10, 0x1fb0000, 0x15c0000);
    Field_unk_02003a80(13, 0x1d70000, 0x1320000);
    Field_unk_02003a8c(14, 0x1df0000, 0x16a0000);
    Field_unk_02003b00(5, 0x8000, 0);
    Field_unk_02003b0c(9, 0x8000, 0);
    Field_unk_02003b18(11, 0x8000, 0);
    Field_unk_02003b24(10, 0x8000, 0);
    Field_unk_02003b30(13, 0x8000, 0);
    Field_unk_02003b3c(14, 0x8000, 0);
    record = Field_unk_0200b4ec(5);
    Field_unk_02003b82((*(s16 *)((s32)record + 10) << 16), -1, (*(s16 *)((s32)record + 18) << 16), 1);
    Field_unk_02003b8e();
    Field_unk_02003a94(40);
    Field_unk_02003b2c(13, 2);
    Field_unk_02003aa2(20);
    Field_unk_02003b6a(13, 0);
    Field_unk_02003b42(11, 2);
    Field_unk_02003b84(11, 0, 10);
    Field_unk_02003b34(14, 4);
    Field_unk_02003b96(14, 0, 20);
    Field_unk_02003b70(10, 11, 40);
    Field_unk_02003b50(10, 4);
    Field_unk_02003bb2(10, 0, 10);
    Field_unk_02003bd6(5, 0x100, 0);
    Field_unk_02003bd2(5, 0x3000, 40);
    Field_unk_02003bd4(0x4005, 0, 10);
    Field_unk_02003be8(10, 0xb000, 0);
    Field_unk_02003bf4(11, 0xd000, 40);
    Field_unk_02003bc6(10, 11, 40);
    Field_unk_02003c18(10, 0x105, 40);
    Field_unk_02003c14(10, 0xb000, 10);
    Field_unk_02003c16(10, 0, 10);
    Field_unk_02003c2a(10, 0x8000, 10);
    Field_unk_02003bf2(11, 2);
    Field_unk_02003c34(11, 0, 10);
    Field_unk_02003be4(10, 3);
    Field_unk_02003c46(10, 0, 40);
    Field_unk_02003c5a(14, 0xb000, 60);
    Field_unk_02003c0a(14, 3);
    Field_unk_0200b594(14, 0, 10);
    Field_unk_02003c78(13, 0x3000, 0);
    Field_unk_02003c84(5, 0x3000, 0);
    Field_unk_02003c90(9, 0x5000, 0);
    Field_unk_0200b59c(11, 0x3000, 0);
    Field_unk_02003ca8(10, 0x5000, 20);
    Field_unk_02003cb4(14, 0xd000, 20);
    Field_unk_0200b544(14, 4);
    Field_unk_02003cbe(14, 0, 10);
    Field_unk_02003ce0(13, 0x103, 0);
    Field_unk_02003c3e(13, 0x10000, 0x8000);
    Field_unk_02003c72(13, 0x1d7, 0x13a);
    Field_unk_0200b554(13, 3);
    Field_unk_0200b594(13, 0, 10);
    Field_unk_02003d08(14, 0xb000, 10);
    Field_unk_02003cb0(14, 4);
    Field_unk_02003d12(14, 0, 10);
    Field_unk_02003cda(13, 3);
    Field_unk_02003d24(13, 0, 10);
    Field_unk_0200b55c(9, 2);
    Field_unk_0200b594(0x4009, 0, 10);
    Field_unk_02003d4a(10, 0xb000, 20);
    Field_unk_02003d4c(10, 0, 30);
    Field_unk_02003d5e(11, 0, 20);
    Field_unk_02003d26(11, 2);
    Field_unk_0200b4cc(30);
    Field_unk_02003d1c(11, 3);
    Field_unk_0200b4cc(30);
    Field_unk_02003d86(11, 0xd000, 30);
    Field_unk_0200b544(11, 4);
    Field_unk_02003cc4(10);
    Field_unk_02003d96(11, 0, 10);
    Field_unk_02003daa(13, 0x5000, 20);
    Field_unk_02003dc0(13, 2);
    Field_unk_0200b4cc(20);
    Field_unk_02003dd8(10, 11, 0);
    Field_unk_02003d46(30);
    Field_unk_02003dbe(10, 3);
    Field_unk_02003dce(11, 3);
    Field_unk_02003e32(11, 0xd000, 0);
    Field_unk_02003e3e(10, 0xb000, 0);
    Field_unk_02003d74(40);
    Field_unk_02003e0c(13, 1);
    Field_unk_02003d82(30);
    Field_unk_02003e5e(13, 0x8000, 0);
    Field_unk_02003d94(40);
    Field_unk_02003e80(13, 0x102, 0);
    Field_unk_02003da6(60);
    Field_unk_0200b59c(14, 0xb000, 0);
    Field_unk_02003db8(30);
    Field_unk_02003e38(14, 4);
    Field_unk_02003e92(0x200e, 0, 30);
    Field_unk_02003ea6(9, 0xb000, 0);
    Field_unk_02003eb2(5, 0xb000, 0);
    Field_unk_02003ebe(13, 0x3000, 20);
    Field_unk_02003eda(5, 0x102, 40);
    Field_unk_02003ecc(0x2005, 0, 40);
    Field_unk_02003eee(13, 0x105, 60);
    Field_unk_02003e8e(13, 3);
    Field_unk_02003f00(10, 0x101, 0);
    Field_unk_02003f0a(11, 0x101, 60);
    Field_unk_02003ec2(14, 2);
    Field_unk_0200b594(14, 0, 60);
    Field_unk_0200b544(13, 3);
    Field_unk_02003e4a(40);
    Field_unk_02003f36(5, 0x102, 0);
    Field_unk_0200b5ac(9, 0x102, 40);
    Field_unk_0200b564(10, 11, 20);
    Field_unk_02003ee4(10, 3);
    Field_unk_02003ef4(11, 3);
    Field_unk_0200b4cc(20);
    Field_unk_0200b4fc(10, 0x9999, 0x4ccc);
    Field_unk_02003ec6(11, 0x9999, 0x4ccc);
    Field_unk_02003ef2(11, 0x1db, 0x15c);
    Field_unk_02003efe(10, 0x1eb, 0x15c);
    Field_unk_02003f14(11);
    Field_unk_02003f1a(10);
    Field_unk_02003f32(11, 1);
    Field_unk_02003f3a(10, 1);
    Field_unk_02003fa6(11, 0xd000, 0);
    Field_unk_02003fb2(10, 0xb000, 0);
    Field_unk_0200b59c(13, 0x3000, 10);
    Field_unk_02003fd8(11, 0x103, 0);
    Field_unk_02003f90(11, 2);
    Field_unk_02003fc8(0x200b, 0);
    Field_unk_0200b4fc(11, 0x19999, 0xcccc);
    Field_unk_02003f4c(5, 0x13333, 0x9999);
    Field_unk_02003f80(11, 0x1db, 0x152);
    record = Field_unk_02003f4e(11);
    ((struct FacingObject *)record)->facing_flags = (u8)(254 & ((struct FacingObject *)record)->facing_flags);
    Field_unk_02003fa0(11, 0x1db, 0x15c);
    Field_unk_02003fe0(5, 1);
    Field_unk_02003fda(5, 4, 0);
    Field_unk_0200b524(5, 0x1cb, 0x13c);
    Field_unk_02003fa4(5, 0x8000, 0x4000);
    Field_unk_0200405e(13, 0x103, 0);
    Field_unk_0200400e(13, 3);
    Field_unk_02004058(13, 0, 30);
    Field_unk_02004028(11, 1);
    Field_unk_02003f9e(20);
    ((struct FacingObject *)record)->facing_flags |= 1;
    Field_unk_02003fe4(11, 0x10000, 0x8000);
    Field_unk_02004018(11, 0x1db, 0x14c);
    Field_unk_0200409c(11, 0xb000, 20);
    Field_unk_02004094(0x200b, 0);
    Field_unk_020040b0(10, 0xb000, 0);
    Field_unk_02004070(10, 2);
    Field_unk_0200b594(10, 0, 20);
    Field_unk_020040de(11, 0x102, 20);
    Field_unk_0200b59c(5, 0x3000, 0);
    Field_unk_020040e6(11, 0x3000, 20);
    Field_unk_02004096(10, 4);
    Field_unk_0200409e(11, 3);
    Field_unk_02004102(11, 0xb000, 20);
    Field_unk_020040b2(11, 3);
    Field_unk_0200b544(13, 3);
    Field_unk_02004048(20);
    Field_unk_020040a6(5, 0x1b0, 0x13c);
    Field_unk_0200b524(13, 0x1a6, 0x137);
    Field_unk_0200b53c(5, 1);
    Field_unk_02004146(5, 0x3000, 0);
    Field_unk_02004152(13, 0x3000, 20);
    Field_unk_0200411a(10, 2);
    Field_unk_02004166(10, 0xd000, 20);
    Field_unk_02004168(0x100a, 0, 40);
    Field_unk_02004138(9, 2);
    Field_unk_020040ae(20);
    Field_unk_0200418a(9, 0xb000, 30);
    Field_unk_020040f8(9, 0x8000, 0x4000);
    Field_unk_0200b524(9, 0x1eb, 0x128);
    Field_unk_020041b0(9, 0x5000, 40);
    Field_unk_0200411e(10, 0x10000, 0x8000);
    Field_unk_0200b4fc(11, 0x10000, 0x8000);
    Field_unk_0200413a(13, 0x10000, 0x8000);
    Field_unk_02004148(14, 0x10000, 0x8000);
    Field_unk_0200417c(10, 0x1d7, 0x134);
    Field_unk_02004200(10, 0x5000, 0);
    Field_unk_02004194(11, 0x1c7, 0x134);
    Field_unk_02004218(11, 0x3000, 0);
    Field_unk_020041ac(14, 0x1e7, 0x134);
    Field_unk_02004230(10, 0xc000, 0);
    Field_unk_0200423c(11, 0xd000, 0);
    Field_unk_02004248(14, 0xb000, 0);
    Field_unk_02004254(5, 0xd000, 0);
    Field_unk_02004260(13, 0xd000, 0);
    Field_unk_0200428c(0x8000, 0x1000);
    Field_unk_020042a6(0x1d80000, -1, 0x12c0000, 1);
    Field_unk_020042b2();
    Field_unk_02004206(40);
    Field_TestFlag246(0x246);
    Field_unk_0200424a(10, 0x8000, 0x4000);
    Field_unk_02004258(11, 0x8000, 0x4000);
    Field_unk_02004266(14, 0x8000, 0x4000);
    Field_unk_020042b6(10, 3);
    Field_unk_02001ba8(10);
    Field_unk_020042dc(9, 2);
    Field_unk_02001bb6(9);
    Field_unk_0200b544(11, 3);
    Field_unk_02001bc4(11);
    Field_unk_0200433c(5, 0x9000, 40);
    Field_unk_0200b554(5, 2);
    Field_unk_02004346(0x2005, 0, 40);
    Field_unk_0200435a(13, 0x3000, 0);
    Field_unk_02004366(5, 0xb000, 30);
    Field_unk_02004372(13, 0xe000, 0);
    Field_unk_02001c0c(5);
    Field_unk_02001c12(13);
    Field_unk_0200438a(14, 0x7000, 40);
    Field_unk_0200438c(14, 0, 30);
    Field_unk_02004344(14, 4);
    Field_unk_0200b4cc(20);
    Field_unk_020043a4(14, 0, 30);
    Field_unk_02001c46(14);
    Field_unk_0200222a();
    Field_unk_0200b4dc(5);
}

void Scene_RunScene381(s32 a0)
{
    s32 p10;
    u8 *rec7;
    s32 recA;
    s32 rec2;
    s32 base6_0;

    p10 = a0;
    recA = Field_unk_0200432a(8);
    *(s32 *)(recA + 24) = 0x10000;
    *(s32 *)(recA + 28) = 0x10000;
    Field_unk_02004378(p10, 0x1d7, 0x122);
    Field_unk_020043fc(p10, 0xc000, 0);
    Field_unk_02004332(10);
    Field_unk_020043a6(8, 0x1d70000, 0x1220000);
    rec7 = Field_unk_02004364(p10);
    rec2 = Field_unk_0200436c(p10);
    Field_unk_0200430a(rec2, 0);
    Field_unk_020043fc_a(p10, 0x100);
    rec7[85] = 0;
    Field_unk_020044c2(201);
    base6_0 = 0;
    do {
        *(volatile s32 *)((s32)rec7 + 12) += 0x8000;
        Field_unk_0200437c(1);
        base6_0 = ((u32)((base6_0 + 1) << 24) >> 24);
    } while (base6_0 != 60);
    Field_unk_020044e4(190);
    base6_0 = 0;
    do {
        *(volatile s32 *)((s32)rec7 + 12) += 0x1999;
        *(volatile s32 *)((s32)rec7 + 24) += -0x28f;
        *(volatile s32 *)((s32)rec7 + 28) += -0x28f;
        *(volatile s32 *)(recA + 24) += -0x28f;
        *(volatile s32 *)(recA + 28) += -0x28f;
        Field_unk_020043b8(1);
        base6_0 = ((u32)((base6_0 + 1) << 24) >> 24);
    } while (base6_0 != 90);
    Field_unk_02004434(p10, 0, 0);
    Field_unk_0200443e(8, 0, 0);
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
        Field_unk_02004528();
        Field_unk_02000f9c();
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
        gOv3 = (u32)(Field_unk_02004778() << 2) >> 16;
    }
    v = gOv3;
    switch (v) {
    case 3:
        gOv = v;
        gOv4 = ((u32)(Field_unk_0200479e() * 20) >> 16) + 40;
        break;
    case 2:
        gOv = 15;
        gOv4 = ((u32)(Field_unk_020047b6() * 40) >> 16) + 80;
        break;
    case 1:
        gOv = 63;
        gOv4 = ((u32)(Field_unk_020047ce() * 80) >> 16) + 160;
        break;
    default:
        gOv = 127;
        gOv4 = ((u32)(Field_unk_020047e6() * 160) >> 16) + 320;
        break;
    }
}

void Actor_MoveTo232_125AndFace4000(s32 no)
{
    Ent *rec;

    rec = Field_unk_02005d12(no);
    Field_unk_02005d6a(no, 0xe80000, 0x7d0000);
    rec->unk6 = 0x4000;
    Field_unk_02005de8(no, 3);
}

void State_ApplyRectsByCondition(s32 a)
{
    if (a != 0) {
        s32 x;
        s32 y;
        x = 1;
        Field_SetRect(8, 47, 64, 7, x, x);
        y = 2;
        Field_unk_02005cea(7, 48, 63, 8, y, x);
        Field_unk_02005cfa(7, 49, 63, 9, y, x);
    } else {
        s32 x;
        x = 1;
        Field_unk_02005d0e(56, 0, 64, 7, x, x);
        Field_unk_02005d1e(56, 0, 63, 8, x, x);
        Field_unk_02005d30(56, 0, 63, 9, 2, x);
        Field_unk_02005d40(58, 25, 64, 8, x, x);
    }
    Field_unk_02005d34();
}

void State_ApplyRectPairByFlag(s32 a)
{
    if (a != 0) {
        s32 n;
        n = 2;
        Field_unk_02005d68(9, 45, 65, 5, n, n);
        Field_unk_02005d7a(11, 46, 67, 6, 1, n);
    } else {
        s32 n;
        n = 2;
        Field_unk_02005d8e(89, 2, 65, 5, n, n);
        Field_unk_02005da0(102, 32, 67, 6, 1, n);
    }
    Field_unk_02005d94();
}

void Scene_RunRandomHalfBranch(void)
{
    if ((gIw2 & 1) == 0) {
        if (Field_unk_02005d2e(Field_unk_02005d48(), 100) > 50) {
            Field_unk_020051a8(1);
        } else {
            Field_unk_020051b0(0);
        }
    }
}

void Scene_RunLateRandomHalfBranch(void)
{
    if ((gIw2 & 1) == 0) {
        if (Field_unk_02005d62(Field_unk_02005d7c(), 100) > 50) {
            Field_unk_0200526c(1);
        } else {
            Field_unk_02005274(0);
        }
    }
}

void Scene_RunFourWayEffectSequence(u32 mode)
{
    extern u8 *gOv8[];
    void Field_unk_0200b5ac(s32, s32, s32);
    void Field_unk_0200b5ec(s32, s32);
    void Field_unk_0200b5f4(s32);
    void Field_TestFlag121(s32);

    u32 i, zero;
    s32 x, y, z;
    s32 *pos;
    u8 *obj, *sprite;

    for (i = 0; i <= 15; i++)
        Field_unk_0200b4f4(i + 16);
    switch (mode) {
    case 0: Field_unk_0200b5ec(0x4039d2, 1); break;
    case 1: Field_unk_0200b5ec(0x4049d2, 1); break;
    case 2: Field_unk_0200b5ec(0x404a4e, 1); break;
    case 3: Field_unk_0200b5ec(0x403a52, 1); break;
    }
    Field_unk_0200b5f4(60);
    Field_TestFlag121(214);
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
        obj = Field_unk_0200b454(284, x, y, z);
        gOv8[i] = obj;
        obj[85] = zero;
        sprite = *(u8 **)(obj + 80);
        sprite[38] = zero;
        ((SpriteMode *)sprite)->mode = 1;
        Field_unk_0200b444(obj, 6);
        Field_unk_0200b3e4(6);
    }
    if (mode == 0) {
        Field_unk_0200b5ac(0, 256, 0);
        Field_unk_0200b5ac(1, 256, 0);
    }
    Field_unk_0200b3e4(20);
    Field_unk_0200b3ec(Effect_AdvanceTenEntryTimers, 3200);
    Field_TestFlag121(246);
    gOv7[0] = 1; Field_unk_0200b3e4(6);
    gOv7[1] = 1; Field_unk_0200b3e4(6);
    gOv7[2] = 1; Field_unk_0200b3e4(6);
    gOv7[3] = 1; Field_unk_0200b3e4(6);
    gOv7[4] = 1; Field_unk_0200b3e4(6);
    gOv7[5] = 1; Field_unk_0200b3e4(6);
    gOv7[6] = 1; Field_unk_0200b3e4(6);
    gOv7[7] = 1; Field_unk_0200b3e4(6);
    gOv7[8] = 1; Field_unk_0200b3e4(6);
    gOv7[9] = 1; Field_unk_0200b3e4(6);
    for (;;) {
        for (i = 0; i <= 9; i++) {
            if (gOv7[i] != 0) {
                i = 888;
                break;
            }
        }
        if (i != 888)
            break;
        Field_unk_0200b3e4(1);
    }
    Field_unk_0200b3e4(40);
    Field_unk_0200b3f4(Effect_AdvanceTenEntryTimers);
    Field_unk_0200b5ec(65536, 1);
    Field_unk_0200b5f4(40);
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

    p = Field_unk_020061fc(a);
    b = b << 16;
    a = a << 16;
    Field_SetMode(a, -1, b, 1);
    Field_unk_02006234(0, 0);
    Field_unk_02006242(20);
    Field_unk_02006038(40);
    p->unk10 = b;
    p->unk8 = a;
    p->unk38 = 0x80000000;
    p->unk40 = 0x80000000;
    p->unk24 = 0;
    p->unk2C = 0;
    Field_unk_02006052(5);
    Field_unk_020060d6();
    Field_unk_0200605c(5);
    Field_unk_0200626e(0x10000, 0);
    Field_unk_0200627c(20);
    Field_unk_02006072(30);
}

void State_ConfigureEightCornerRegions(void)
{
    s32 x;
    s32 y;
    s32 z;
    s32 w;
    s32 v;

    x = 0;
    Field_unk_02006132(14, 8, 1, 1, 10, x);
    Field_unk_02006144(14, 28, 1, 1, 11, x);
    Field_unk_02006156(44, 8, 1, 1, 12, x);
    Field_unk_02006168(44, 28, 1, 1, 13, x);
    z = 14;
    y = 8;
    Field_unk_0200617e(13, 8, 1, 1, z, y);
    w = 28;
    Field_unk_02006190(13, 28, 1, 1, z, w);
    v = 44;
    Field_unk_020061a4(43, 8, 1, 1, v, y);
    Field_unk_020061b4(43, 28, 1, 1, v, w);
}

void Scene_RunVariantStep(s32 a, s32 b, s32 c)
{
    if (a == 1) {
        Field_TestFlag134(0x134);
        Field_unk_0200634c(0x203a52, 1);
    } else {
        Field_unk_0200638c(0x121);
        Field_unk_0200635e(0x10000, 1);
    }
    Field_unk_0200636c(b);
    if (c != 0) {
        Field_unk_0200624e(c);
    }
}

void Scene_RunStepByRuntimeBits(s32 a)
{
    if ((gIw2 & 2) != 0) {
        Field_unk_0200624c(a, 7);
    } else {
        Field_unk_02006256(a, 0);
    }
    if ((gIw2 & 15) == 0) {
        Field_unk_02005c74(a);
    }
}

void State_ForwardByRuntimeWordBits(s32 a)
{
    volatile u32 *p = (u32 *)0x03001e40;

    if (*p & 1) {
        Field_unk_02006294(a, Field_unk_020061c4(*p >> 1, 6));
    }
    if ((*p & 15) == 0) {
        Field_unk_02005cb0(a);
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
        Field_unk_02006284(self);
        return;
    }

    amplitude = Field_unk_02006234_a(frame << 10);
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
        Field_unk_020062d4(obj);
        return;
    }

    amp = Field_unk_02006284_a(frame << 10);
    obj->amplitude_x = amp;
    obj->amplitude_y = -amp;
    obj->x = anchor->x;
    obj->y += 0x10000;
    obj->z = anchor->z - (0x10000 - amp) * 5 + 0x100000;
}

void State_SetValue140Mode0(void)
{
    Field_unk_0200660c(140, 0);
}

void Scene_CallHelper(void)
{
    Field_unk_02006620();
}

void Scene_RunActor15TwoStep(void)
{
    Field_unk_020064fe(15);
    Field_unk_02005de6();
}
