#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/field/extended_effect.h"

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

u8 *Field_Run19(s32);

void *Field_Run20();

void *Field_Run21();

u8 *Field_Run22(s32);

u8 *Field_Run23(s32);

Ent *Field_Run24(s32);

u8 *Field_Run25(s32, s32, s32, s32);

Ent *Field_Run26(s32);

/*
 * Table getters published from the header words of overlay resource_381.
 * Only one overlay is resident at 0x02000000 at a time, so an address that
 * matches another overlay's names different bytes, not the same table.
 */

/* Set up the scene actors, run the two presentation phases, and dismiss the
 * actors through the shared exit sequence. Retained pending an exact build. */

static __inline__ s32 InstallTask(s32 (*f)(void (*)(), s32), void (*callback)(), s32 key)
{
    void Field_Run27();
    void Field_Run28();
    void Field_Run29();
    void Field_Run30();

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

s32 Field_Run31(void)
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
    void Field_Run27();
    void Field_Run28();
    void Field_Run29();
    void Field_Run30();

    u32 i;
    u8 *record;
    s32 base5_3001ec4;
    s32 base5_3001ebc;
    u8 *p7;

    base5_3001ec4 = (s32)gIw;
    p7 = *(u8 **)base5_3001ec4;
    Field_Run32();
    *(s32 *)(((s32)p7 + 0x40c)) = 0;
    Field_Run21(141);
    Field_Place(0x10000, 0x10000, 0x10000);
    Field_Place2(0, 0xe80000, 0x9c0000);
    Field_Place3(1, 0xda0000, 0xac0000);
    Field_Place4(0, 0xb000, 0);
    Field_Place5(1, 0xd000, 0);
    Field_Place6(5, 0x1db0000, 0x14c0000);
    Field_Place7(9, 0x1eb0000, 0x14c0000);
    Field_Place8(11, 0x1cb0000, 0x15c0000);
    Field_Place9(10, 0x1fb0000, 0x15c0000);
    Field_Place10(13, 0x1d70000, 0x1320000);
    Field_Place11(14, 0x1df0000, 0x16a0000);
    Field_Run33(15, 1);
    base5_3001ebc = base5_3001ec4 - 8;
    Field_Run(0xe80000, -1, 0x9c0000, 0);
    Field_Run34();
    *(s32 *)((*(s32 *)base5_3001ebc + 0x1c8)) = 8;
    Field_Run35();
    Field_Run36();
    Field_Run2(0x7fff, 0);
    Field_Run37(4);
    Field_Run38(4);
    Field_Run3(0x10000, 0);
    Field_Run39(4);
    Field_Run40(4);
    Field_Run20();
    Field_Run4(0x7fff, 0);
    Field_Run41(4);
    Field_Run42(16);
    Field_Run30(144);
    Field_Run5(0x10000, 0);
    Field_Run43(4);
    Field_Run44(4);
    Field_Run6(0x7fff, 0);
    Field_Run45(4);
    Field_Run46(4);
    Field_Run47(144);
    Field_Run7(0x10000, 0);
    Field_Run29(48);
    Field_Run48(48);
    Field_Run49(0, 6, 0);
    Field_Run50(1, 6, 20);
    Field_Run51(1, 20, 20);
    Field_Run52(0, 20, 40);
    Field_Do(0x10cd);
    Field_Place12(11, 0, 20);
    Field_Run8(10, 0);
    Field_Run53(1, 20, 0);
    Field_Run54(5, 0, 20);
    Field_Run55(0, 20, 0);
    Field_Run56(14, 0, 20);
    Field_Run57(9, 0, 20);
    Field_Run58(0, 1, 0);
    Field_Run59(40);
    Field_Run60(0, 2);
    Field_Run61(1, 2);
    Field_Run62(20);
    Field_Place13(0, 0xb000, 0);
    Field_Place14(1, 0xd000, 30);
    Field_Run63(1, 20, 0);
    Field_Run64(0, 2);
    Field_Run61(1, 2);
    Field_Run65(20);
    Field_Run66(0, 20, 20);
    record = Field_Run19(15);
    Field_Apply2(record, 0);
    Field_Place15(15, 0x1450000, 0x12e0000);
    record = Field_Run22(15);
    record[85] = 5;
    *(s32 *)(((s32)p7 + 0x40c)) = 1;
    Field_Do2(0x121);
    Field_Place16(-1, -1, 0xe666);
    Field_Run67();
    Field_Run68(150);
    Field_Run69(11, 0, 20);
    Field_Run70(5, 0);
    Field_Run71(10, 0, 10);
    Field_Run58(0, 1, 20);
    Field_Place17(0, 0x3000, 0);
    Field_Place18(1, 0x3000, 10);
    Field_Place19(5, 0x1db0000, 0x14c0000);
    Field_Place15(9, 0x1eb0000, 0x14c0000);
    Field_Place20(11, 0x1cb0000, 0x15c0000);
    Field_Place21(10, 0x1fb0000, 0x15c0000);
    Field_Place22(13, 0x1d70000, 0x1320000);
    Field_Place23(14, 0x1df0000, 0x16a0000);
    Field_Run9(0x66666, 0xcccc);
    Field_Run10(0x1480000, -1, 0x12b0000, 1);
    Field_Run72();
    Field_Run73(167);
    Field_Run11(0x205294, 2);
    Field_Run74(20);
    Field_Run75(20);
    Field_Run12(0x10000, 2);
    Field_Run76(20);
    Field_Run68(200);
    Field_Apply(0x1001, 0);
    if (Field_Check(0, 0) == 0) {
        Field_Do3(0x10d6);
    } else {
        Field_Do4(0x10d7);
    }
    Field_Place24(0x1001, 0, 80);
    Field_Do5(0x10d8);
    Field_Run77(9, 0, 20);
    Field_Run78(1, 20, 0);
    *(s32 *)(((s32)p7 + 0x40c)) = 0;
    Field_Run79(141);
    Field_Place25(0x10000, 0x20000, 0x10000);
    Field_Run80(80);
    *(s32 *)(((s32)p7 + 0x40c)) = 1;
    Field_Do6(0x121);
    Field_Place26(-1, -1, 0xe666);
    Field_Run81();
    Field_Run82(0, 20, 60);
    Field_Run83(14, 0, 30);
    Field_Place27(15, 0xa000, 40);
    Field_Run84(1, 20, 20);
    Field_Run85(0, 20, 20);
    Field_Place28(0x1001, 0, 30);
    Field_Place29(15, 0x1000, 40);
    Field_Run86(1, 20, 20);
    Field_Run87(0, 20, 20);
    Field_Run88(5, 0, 30);
    Field_Run89();
    InstallTask(Field_Apply3, (void (*)())0x020090c5, 0xc80);
    InstallTask(Field_Apply4, (void (*)())0x0200935d, 0xc80);
    Field_Run90(240);
    Field_Run91(10, 0, 30);
    Field_Place30(5, 0x1db0000, 0x14c0000);
    Field_Place31(9, 0x1eb0000, 0x14c0000);
    Field_Place32(11, 0x1cb0000, 0x15c0000);
    Field_Place33(10, 0x1fb0000, 0x15c0000);
    Field_Place34(13, 0x1d70000, 0x1320000);
    Field_Place35(14, 0x1df0000, 0x16a0000);
    Field_Place36(5, 0x8000, 0);
    Field_Place37(9, 0x8000, 0);
    Field_Place38(11, 0x8000, 0);
    Field_Place39(10, 0x8000, 0);
    Field_Place40(13, 0x8000, 0);
    Field_Place41(14, 0x8000, 0);
    record = Field_Run19(5);
    Field_Run13((*(s16 *)((s32)record + 10) << 16), -1, (*(s16 *)((s32)record + 18) << 16), 1);
    Field_Run92();
    Field_Run93(40);
    Field_Run94(13, 2);
    Field_Run95(20);
    Field_Run96(13, 0);
    Field_Run97(11, 2);
    Field_Run98(11, 0, 10);
    Field_Run99(14, 4);
    Field_Run100(14, 0, 20);
    Field_Run101(10, 11, 40);
    Field_Run102(10, 4);
    Field_Run103(10, 0, 10);
    Field_Place42(5, 0x100, 0);
    Field_Place43(5, 0x3000, 40);
    Field_Place44(0x4005, 0, 10);
    Field_Place45(10, 0xb000, 0);
    Field_Place46(11, 0xd000, 40);
    Field_Run104(10, 11, 40);
    Field_Place47(10, 0x105, 40);
    Field_Place48(10, 0xb000, 10);
    Field_Run105(10, 0, 10);
    Field_Place49(10, 0x8000, 10);
    Field_Run106(11, 2);
    Field_Run107(11, 0, 10);
    Field_Run108(10, 3);
    Field_Run109(10, 0, 40);
    Field_Place50(14, 0xb000, 60);
    Field_Run110(14, 3);
    Field_Run69(14, 0, 10);
    Field_Place51(13, 0x3000, 0);
    Field_Place52(5, 0x3000, 0);
    Field_Place53(9, 0x5000, 0);
    Field_Place54(11, 0x3000, 0);
    Field_Place55(10, 0x5000, 20);
    Field_Place56(14, 0xd000, 20);
    Field_Run111(14, 4);
    Field_Run112(14, 0, 10);
    Field_Place57(13, 0x103, 0);
    Field_Place58(13, 0x10000, 0x8000);
    Field_Place59(13, 0x1d7, 0x13a);
    Field_Run60(13, 3);
    Field_Run69(13, 0, 10);
    Field_Place60(14, 0xb000, 10);
    Field_Run113(14, 4);
    Field_Run114(14, 0, 10);
    Field_Run115(13, 3);
    Field_Run116(13, 0, 10);
    Field_Run61(9, 2);
    Field_Place61(0x4009, 0, 10);
    Field_Place62(10, 0xb000, 20);
    Field_Run117(10, 0, 30);
    Field_Run118(11, 0, 20);
    Field_Run119(11, 2);
    Field_Run68(30);
    Field_Run120(11, 3);
    Field_Run68(30);
    Field_Place63(11, 0xd000, 30);
    Field_Run111(11, 4);
    Field_Run121(10);
    Field_Run122(11, 0, 10);
    Field_Place64(13, 0x5000, 20);
    Field_Run123(13, 2);
    Field_Run68(20);
    Field_Run124(10, 11, 0);
    Field_Run125(30);
    Field_Run126(10, 3);
    Field_Run127(11, 3);
    Field_Place65(11, 0xd000, 0);
    Field_Place66(10, 0xb000, 0);
    Field_Run128(40);
    Field_Run129(13, 1);
    Field_Run130(30);
    Field_Place67(13, 0x8000, 0);
    Field_Run131(40);
    Field_Place68(13, 0x102, 0);
    Field_Run132(60);
    Field_Place54(14, 0xb000, 0);
    Field_Run133(30);
    Field_Run134(14, 4);
    Field_Place69(0x200e, 0, 30);
    Field_Place70(9, 0xb000, 0);
    Field_Place71(5, 0xb000, 0);
    Field_Place72(13, 0x3000, 20);
    Field_Place73(5, 0x102, 40);
    Field_Place74(0x2005, 0, 40);
    Field_Place75(13, 0x105, 60);
    Field_Run135(13, 3);
    Field_Place76(10, 0x101, 0);
    Field_Place77(11, 0x101, 60);
    Field_Run136(14, 2);
    Field_Run69(14, 0, 60);
    Field_Run111(13, 3);
    Field_Run137(40);
    Field_Place78(5, 0x102, 0);
    Field_Place79(9, 0x102, 40);
    Field_Run58(10, 11, 20);
    Field_Run138(10, 3);
    Field_Run139(11, 3);
    Field_Run68(20);
    Field_Place80(10, 0x9999, 0x4ccc);
    Field_Place81(11, 0x9999, 0x4ccc);
    Field_Place82(11, 0x1db, 0x15c);
    Field_Place83(10, 0x1eb, 0x15c);
    Field_Run140(11);
    Field_Run141(10);
    Field_Run142(11, 1);
    Field_Run143(10, 1);
    Field_Place84(11, 0xd000, 0);
    Field_Place85(10, 0xb000, 0);
    Field_Place54(13, 0x3000, 10);
    Field_Place86(11, 0x103, 0);
    Field_Run144(11, 2);
    Field_Run14(0x200b, 0);
    Field_Place80(11, 0x19999, 0xcccc);
    Field_Place87(5, 0x13333, 0x9999);
    Field_Place88(11, 0x1db, 0x152);
    record = Field_Run23(11);
    ((struct FacingObject *)record)->facing_flags = (u8)(254 & ((struct FacingObject *)record)->facing_flags);
    Field_Place89(11, 0x1db, 0x15c);
    Field_Run145(5, 1);
    Field_Run146(5, 4, 0);
    Field_Place90(5, 0x1cb, 0x13c);
    Field_Place91(5, 0x8000, 0x4000);
    Field_Place92(13, 0x103, 0);
    Field_Run147(13, 3);
    Field_Run148(13, 0, 30);
    Field_Run149(11, 1);
    Field_Run150(20);
    ((struct FacingObject *)record)->facing_flags |= 1;
    Field_Place93(11, 0x10000, 0x8000);
    Field_Place94(11, 0x1db, 0x14c);
    Field_Place95(11, 0xb000, 20);
    Field_Run15(0x200b, 0);
    Field_Place96(10, 0xb000, 0);
    Field_Run151(10, 2);
    Field_Run69(10, 0, 20);
    Field_Place97(11, 0x102, 20);
    Field_Place54(5, 0x3000, 0);
    Field_Place98(11, 0x3000, 20);
    Field_Run152(10, 4);
    Field_Run153(11, 3);
    Field_Place99(11, 0xb000, 20);
    Field_Run154(11, 3);
    Field_Run111(13, 3);
    Field_Run155(20);
    Field_Place100(5, 0x1b0, 0x13c);
    Field_Place90(13, 0x1a6, 0x137);
    Field_Run156(5, 1);
    Field_Place101(5, 0x3000, 0);
    Field_Place102(13, 0x3000, 20);
    Field_Run157(10, 2);
    Field_Place103(10, 0xd000, 20);
    Field_Place104(0x100a, 0, 40);
    Field_Run158(9, 2);
    Field_Run159(20);
    Field_Place105(9, 0xb000, 30);
    Field_Place106(9, 0x8000, 0x4000);
    Field_Place90(9, 0x1eb, 0x128);
    Field_Place107(9, 0x5000, 40);
    Field_Place108(10, 0x10000, 0x8000);
    Field_Place80(11, 0x10000, 0x8000);
    Field_Place109(13, 0x10000, 0x8000);
    Field_Place110(14, 0x10000, 0x8000);
    Field_Place111(10, 0x1d7, 0x134);
    Field_Place112(10, 0x5000, 0);
    Field_Place113(11, 0x1c7, 0x134);
    Field_Place114(11, 0x3000, 0);
    Field_Place115(14, 0x1e7, 0x134);
    Field_Place116(10, 0xc000, 0);
    Field_Place117(11, 0xd000, 0);
    Field_Place118(14, 0xb000, 0);
    Field_Place119(5, 0xd000, 0);
    Field_Place120(13, 0xd000, 0);
    Field_Run16(0x8000, 0x1000);
    Field_Run17(0x1d80000, -1, 0x12c0000, 1);
    Field_Run160();
    Field_Run161(40);
    Field_Do7(0x246);
    Field_Place121(10, 0x8000, 0x4000);
    Field_Place122(11, 0x8000, 0x4000);
    Field_Place123(14, 0x8000, 0x4000);
    Field_Run162(10, 3);
    Field_Run163(10);
    Field_Run164(9, 2);
    Field_Run165(9);
    Field_Run111(11, 3);
    Field_Run166(11);
    Field_Place124(5, 0x9000, 40);
    Field_Run60(5, 2);
    Field_Place125(0x2005, 0, 40);
    Field_Place126(13, 0x3000, 0);
    Field_Place127(5, 0xb000, 30);
    Field_Place128(13, 0xe000, 0);
    Field_Run167(5);
    Field_Run168(13);
    Field_Place129(14, 0x7000, 40);
    Field_Run169(14, 0, 30);
    Field_Run170(14, 4);
    Field_Run68(20);
    Field_Run171(14, 0, 30);
    Field_Run172(14);
    Field_Run173();
    Field_Run174(5);
}

void Scene_RunScene381(s32 a0)
{
    s32 p10;
    u8 *rec7;
    s32 recA;
    s32 rec2;
    s32 base6_0;

    p10 = a0;
    recA = Field_Check3(8);
    *(s32 *)(recA + 24) = 0x10000;
    *(s32 *)(recA + 28) = 0x10000;
    Field_Place130(p10, 0x1d7, 0x122);
    Field_Place131(p10, 0xc000, 0);
    Field_Run175(10);
    Field_Place132(8, 0x1d70000, 0x1220000);
    rec7 = Field_Check2(p10);
    rec2 = Field_Check4(p10);
    Field_Run176(rec2, 0);
    Field_Run18(p10, 0x100);
    rec7[85] = 0;
    Field_Run177(201);
    base6_0 = 0;
    do {
        *(volatile s32 *)((s32)rec7 + 12) += 0x8000;
        Field_Run178(1);
        base6_0 = ((u32)((base6_0 + 1) << 24) >> 24);
    } while (base6_0 != 60);
    Field_Run179(190);
    base6_0 = 0;
    do {
        *(volatile s32 *)((s32)rec7 + 12) += 0x1999;
        *(volatile s32 *)((s32)rec7 + 24) += -0x28f;
        *(volatile s32 *)((s32)rec7 + 28) += -0x28f;
        *(volatile s32 *)(recA + 24) += -0x28f;
        *(volatile s32 *)(recA + 28) += -0x28f;
        Field_Run180(1);
        base6_0 = ((u32)((base6_0 + 1) << 24) >> 24);
    } while (base6_0 != 90);
    Field_Run181(p10, 0, 0);
    Field_Run182(8, 0, 0);
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
        Field_Run183();
        Field_Run184();
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
        gOv3 = (u32)(Field_Check5() << 2) >> 16;
    }
    v = gOv3;
    switch (v) {
    case 3:
        gOv = v;
        gOv4 = ((u32)(Field_Check6() * 20) >> 16) + 40;
        break;
    case 2:
        gOv = 15;
        gOv4 = ((u32)(Field_Check7() * 40) >> 16) + 80;
        break;
    case 1:
        gOv = 63;
        gOv4 = ((u32)(Field_Check8() * 80) >> 16) + 160;
        break;
    default:
        gOv = 127;
        gOv4 = ((u32)(Field_Check9() * 160) >> 16) + 320;
        break;
    }
}

void Actor_MoveTo232_125AndFace4000(s32 no)
{
    Ent *rec;

    rec = Field_Run24(no);
    Field_Place134(no, 0xe80000, 0x7d0000);
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
        Field_SetRect2(7, 48, 63, 8, y, x);
        Field_SetRect3(7, 49, 63, 9, y, x);
    } else {
        s32 x;
        x = 1;
        Field_SetRect4(56, 0, 64, 7, x, x);
        Field_SetRect5(56, 0, 63, 8, x, x);
        Field_SetRect6(56, 0, 63, 9, 2, x);
        Field_SetRect7(58, 25, 64, 8, x, x);
    }
    Field_Run185();
}

void State_ApplyRectPairByFlag(s32 a)
{
    if (a != 0) {
        s32 n;
        n = 2;
        Field_SetRect8(9, 45, 65, 5, n, n);
        Field_SetRect9(11, 46, 67, 6, 1, n);
    } else {
        s32 n;
        n = 2;
        Field_SetRect10(89, 2, 65, 5, n, n);
        Field_SetRect11(102, 32, 67, 6, 1, n);
    }
    Field_Run186();
}

void Scene_RunRandomHalfBranch(void)
{
    if ((gIw2 & 1) == 0) {
        if (Field_Apply6(Field_Check10(), 100) > 50) {
            Field_Do8(1);
        } else {
            Field_Do9(0);
        }
    }
}

void Scene_RunLateRandomHalfBranch(void)
{
    if ((gIw2 & 1) == 0) {
        if (Field_Apply7(Field_Check11(), 100) > 50) {
            Field_Do10(1);
        } else {
            Field_Do11(0);
        }
    }
}

void Scene_RunFourWayEffectSequence(u32 mode)
{
    extern u8 *gOv8[];
    void Field_Run27(s32, s32, s32);
    void Field_Run28(s32, s32);
    void Field_Run29(s32);
    void Field_Run30(s32);

    u32 i, zero;
    s32 x, y, z;
    s32 *pos;
    u8 *obj, *sprite;

    for (i = 0; i <= 15; i++)
        Field_Do12(i + 16);
    switch (mode) {
    case 0: Field_Run28(0x4039d2, 1); break;
    case 1: Field_Run28(0x4049d2, 1); break;
    case 2: Field_Run28(0x404a4e, 1); break;
    case 3: Field_Run28(0x403a52, 1); break;
    }
    Field_Run29(60);
    Field_Run30(214);
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
        obj = Field_Run25(284, x, y, z);
        gOv8[i] = obj;
        obj[85] = zero;
        sprite = *(u8 **)(obj + 80);
        sprite[38] = zero;
        ((SpriteMode *)sprite)->mode = 1;
        Field_Apply8(obj, 6);
        Field_Do13(6);
    }
    if (mode == 0) {
        Field_Place133(0, 256, 0);
        Field_Place133(1, 256, 0);
    }
    Field_Do13(20);
    Field_Apply9(Effect_AdvanceTenEntryTimers, 3200);
    Field_Run30(246);
    gOv7[0] = 1; Field_Do13(6);
    gOv7[1] = 1; Field_Do13(6);
    gOv7[2] = 1; Field_Do13(6);
    gOv7[3] = 1; Field_Do13(6);
    gOv7[4] = 1; Field_Do13(6);
    gOv7[5] = 1; Field_Do13(6);
    gOv7[6] = 1; Field_Do13(6);
    gOv7[7] = 1; Field_Do13(6);
    gOv7[8] = 1; Field_Do13(6);
    gOv7[9] = 1; Field_Do13(6);
    for (;;) {
        for (i = 0; i <= 9; i++) {
            if (gOv7[i] != 0) {
                i = 888;
                break;
            }
        }
        if (i != 888)
            break;
        Field_Do13(1);
    }
    Field_Do13(40);
    Field_Do14(Effect_AdvanceTenEntryTimers);
    Field_Run28(65536, 1);
    Field_Run29(40);
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

    p = Field_Run26(a);
    b = b << 16;
    a = a << 16;
    Field_SetMode(a, -1, b, 1);
    Field_Apply10(0, 0);
    Field_Do15(20);
    Field_Do16(40);
    p->unk10 = b;
    p->unk8 = a;
    p->unk38 = 0x80000000;
    p->unk40 = 0x80000000;
    p->unk24 = 0;
    p->unk2C = 0;
    Field_Do17(5);
    Field_Run187();
    Field_Do18(5);
    Field_Apply11(0x10000, 0);
    Field_Do19(20);
    Field_Do20(30);
}

void State_ConfigureEightCornerRegions(void)
{
    s32 x;
    s32 y;
    s32 z;
    s32 w;
    s32 v;

    x = 0;
    Field_SetRect12(14, 8, 1, 1, 10, x);
    Field_SetRect13(14, 28, 1, 1, 11, x);
    Field_SetRect14(44, 8, 1, 1, 12, x);
    Field_SetRect15(44, 28, 1, 1, 13, x);
    z = 14;
    y = 8;
    Field_SetRect16(13, 8, 1, 1, z, y);
    w = 28;
    Field_SetRect17(13, 28, 1, 1, z, w);
    v = 44;
    Field_SetRect18(43, 8, 1, 1, v, y);
    Field_SetRect19(43, 28, 1, 1, v, w);
}

void Scene_RunVariantStep(s32 a, s32 b, s32 c)
{
    if (a == 1) {
        Field_Do21(0x134);
        Field_Apply12(0x203a52, 1);
    } else {
        Field_Do22(0x121);
        Field_Apply13(0x10000, 1);
    }
    Field_Do23(b);
    if (c != 0) {
        Field_Do24(c);
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
        Field_Do25(a);
    }
}

void State_ForwardByRuntimeWordBits(s32 a)
{
    volatile u32 *p = (u32 *)0x03001e40;

    if (*p & 1) {
        Field_Apply16(a, Field_Apply17(*p >> 1, 6));
    }
    if ((*p & 15) == 0) {
        Field_Do26(a);
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
        Field_Run188(self);
        return;
    }

    amplitude = Field_Check12(frame << 10);
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
        Field_Run189(obj);
        return;
    }

    amp = Field_Check13(frame << 10);
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
    Field_Run190();
}

void Scene_RunActor15TwoStep(void)
{
    Field_Do27(15);
    Field_Run191();
}
