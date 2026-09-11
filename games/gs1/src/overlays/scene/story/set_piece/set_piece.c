#include "types.h"
#include "scene.h"
#include "facing_object.h"

/* overlays/scene/story/set_piece/actor_facing.c */

s32 OvObj_UpdateFacingTowardTarget(struct FacingObject *object)
{
    s32 delta;
    u16 old;
    s32 angle;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8)(0xFE & object->facing_flags);
        angle = (u16)CalculateFacingAngle(target->position_z - object->position_z, target->position_x - object->position_x);
        old = object->facing;
        delta = (s16)(angle - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }
            /* The loader relocates the stored pool word to -0x1000. */
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            object->facing = (u16)(old + delta);
        }
    }
    return 1;
}

/* overlays/scene/story/set_piece/actor_motion.c */
void Story_unk26_4();          /* site veneer -> Story_unk27_4 */

void Story_unk28_4();          /* site veneer -> Story_unk29_4 */

void Story_unk30_4();          /* site veneer -> Story_unk31_4 */

void Story_unk32_4();          /* site veneer -> Story_unk33_4 (1st) */

void Story_unk34_4();          /* site veneer -> Story_unk35_4 */

void Story_unk36_4();          /* site veneer -> Story_unk33_4 (2nd) */

void Story_unk37_4();          /* site veneer -> Story_unk38_4 */

void Story_unk39_4();          /* site veneer -> Story_unk33_4 (3rd) */

void Story_unk40_4();          /* site veneer -> Story_unk41_4 */

void Story_unk42_4();          /* site veneer -> Audio_PlayCue */

void Story_unk43_4();          /* site veneer -> Story_unk33_4 (4th) */

void Scene_RunActorTenFourStepSequence(void)
{
    Story_unk26_4(24, 1);
    Story_unk28_4(10, 9);
    Story_unk30_4();
    Story_unk32_4(10, 2);
    Story_unk34_4(1);
    Story_unk36_4(10, 2);
    Story_unk37_4();
    Story_unk39_4(10, 2);
    Story_unk40_4();
    Story_unk42_4(288);
    Story_unk43_4(10, 2);
}

/* overlays/scene/story/set_piece/actor_position.c */
struct Frame {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct Frame *Story_unk44_4(s32);
s32 *Story_unk45_4(s32);

s32 *Story_unk46_4(s32);

s32 *Story_unk47_4(s32);

s32 *Story_unk48_4(s32);

/*
 * resource_3a4 owner at 0x02003460, 34 bytes with no literal pool: clear
 * bit 0 of the scene record's flags byte at +89, then place a marker at the
 * record's x and z at level 255. Both callees are reached through veneers.
 */

u8 *Story_unk49_4();           /* Scene record accessor. */

void Story_unk50_4();          /* Place a marker: kind, x, z, level. */

s32 Actor_CopyActor8PositionWithFixedY(struct Frame *dst)
{
    struct Frame *src = Story_unk44_4(8);

    dst->f08 = src->f08;
    dst->f0c = 0xFFF40000;
    dst->f10 = src->f10;
    return 0;
}

void Actor_UpdateSlot10ByTileX(void)
{
    s32 *a = Story_unk45_4(10);

    if (a != 0) {
        s32 x = 24;
        s32 y = 26;
        s32 t;

        Story_unk51_5(x, 27, 2, 1, x, y);
        t = a[2] >> 20;
        if (t == 25) {
            Story_unk52_4(0, 0, 1, 1, t, y);
        } else {
            Story_unk53_4(0, 0, 1, 1, x, y);
        }
        Story_Apply(a, 0);
        ((u8 *)a)[0x55] = 0;
        Story_unk51_4();
        Story_unk22_2(1);
    }
}

void Actor_RaiseSlot9StepA(void)
{
    s32 *p;
    s32 s0;

    p = Story_unk46_4(9);
    s0 = 26;
    Story_unk54_4(0, 0, 1, 1, s0, s0);
    if (p != 0) {
        Story_Apply2(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    Story_unk23_2(0x200);
}

void Actor_RaiseSlot9StepB(void)
{
    s32 *p;
    s32 s0;
    s32 s1;

    p = Story_unk47_4(9);
    s0 = 25;
    s1 = 13;
    Story_unk55_4(23, 13, 1, 1, s0, s1);
    if (p != 0) {
        Story_Apply3(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    Story_unk24_2(0x200);
}

void Actor_RaiseSlot11AndSetFlag201(void)
{
    s32 *p;
    s32 s0;
    s32 s1;

    p = Story_unk48_4(11);
    s0 = 17;
    s1 = 10;
    Story_unk45(1, 0, 1, 1, s0, s1);
    if (p != 0) {
        Story_Apply4(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    Story_unk25_2(0x201);
}

void Actor_ClearCollisionFlagAndPlaceMarker(s32 no)
{
    u8 *record;

    record = Story_unk49_4(no);
    record[89] &= 0xfe;

    Story_unk50_4(0, *(s32 *)(record + 8), *(s32 *)(record + 16), 255);
}

/* overlays/scene/story/set_piece/actor_presentation.c */
extern unsigned char Value_00004ccc;
extern u32 gIw;

s32 *Story_unk52_3(s32);

u8 *Story_unk53_3(s32);
u8 *Story_unk54_3(s32);

u8 *Story_unk55_3(s32 id);
u8 *Story_unk56_3(s32 id);

/*
 * Presentation setup in resource_3a4: clear a record byte, adjust two
 * handle flag bits, run two presentation primitives, then stamp a fixed
 * rate into the record.
 */

/* Declared without prototypes -- call sites vary in argument shape. */

/*
 * resource_3a4: a published callback that sets the mode of actor record 8.
 */

/*
 * Presentation callback for resource_3a4, published rather than called
 * directly from this overlay.
 */

void ActorDraw_SetCellAndLowerActorEight(void)
{
    s32 *p;
    s32 s0;
    s32 s1;

    p = Story_unk52_3(8);
    s0 = 9;
    s1 = 13;
    Story_unk56_4(7, 13, 1, 1, s0, s1);
    if (p != 0) {
        Story_Apply5(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    Story_unk26_2(0x200);
}

void Actor_SetActor10Byte23To3(void)
{
    Story_unk53_3(10)[0x23] = 3;
}

void Actor_SetActor10Byte23To1(void)
{
    Story_unk54_3(10)[0x23] = 1;
}

/*
 * Clear the record byte at +0x55, then rewrite the handle flags at +9 as
 * (flags & ~0x0c) | 0x04 -- the mask is built from the zero already in v,
 * not spelled as a constant. The rate address is held in a local and
 * stored to both +24 and +28. The 50-byte owner includes its one pool
 * word.
 */
void Actor_SetMode3AndRate4ccc(u8 *rec)
{
    u8 *p = rec + 0x55;
    s32 v = 0;
    u8 *h;

    *p = v;
    h = *(u8 **)(rec + 80);
    v -= 13;
    v &= h[9];
    v |= 4;
    h[9] = (u8)v;
    Story_unk57_3(rec, 3);
    Story_unk58_3(rec, 0);
    {
        s32 rate = (s32)&Value_00004ccc;

        *(s32 *)(rec + 24) = rate;
        *(s32 *)(rec + 28) = rate;
    }
}

/*
 * Bit 1 of the runtime status word at 0x03001e40 selects mode 7 or mode 6
 * for record 8. That bit's meaning is unverified: other callbacks here mask
 * different bits of the same word. Both branches reach the same veneer, and
 * the declarations carry no parameter list because the arguments are set up
 * in registers at the call site. The owner spans 44 bytes -- the body, one
 * alignment halfword and one literal pool word.
 */
void Actor_SetActor8ModeByCounterBit(void)
{
    if (((gIw >> 1) & 1) != 0) {
        Story_unk59_3(8, 7);
    } else {
        Story_unk60_3(8, 6);
    }
}

/*
 * Compare record 0's field at +12 against a signed threshold and set record
 * 12's mode accordingly; the taller branch also sets bit 1 of record 11's
 * byte at +35.  Both offsets are named by position only and their roles are
 * unverified; +35 is read-modify-written as a flags byte.  The threshold is
 * kept as the value 0x00300000 the code builds, in no assumed fixed-point
 * unit.
 */
void Actor_SetActor12ModeByActorZeroHeight(void)
{
    if (*(s32 *)(Story_unk55_3(0) + 12) > 0x00300000) {
        {
            u8 *flag = Story_unk56_3(11) + 35;
            s32 bit = 2;

            bit |= *flag;
            *flag = bit;
        }
        Story_Apply6(12, 3);
    } else {
        Story_Apply7(12, 2);
    }
}

/* overlays/scene/story/set_piece/display.c */
extern u16 gOv;

void State_SetDispcntBit9ByThreshold(void)
{
    volatile u16 *reg = (volatile u16 *)0x04000000;
    s16 v = *reg & 0xfdff;

    if ((u32)(Story_unk45() * 100) >> 16 >= gOv) {
        s32 k = 0x200;

        v |= k;
    }
    {
        u32 t = (u16)v;

        *reg = t;
    }
}

/* overlays/scene/story/set_piece/effect.c */
extern s32 gIw;
extern s32 gOv2[3];   /* image offset 0x5240 */
extern u8 *Story_unk61_3(s32 arg0, s32 arg1, s32 arg2, s32 arg3);  /* site veneer -> Story_unk62_3 */
extern void Story_unk63_3(u8 *obj);                             /* site veneer -> Actor_SetMode3AndRate4ccc */
extern void Story_unk64_3(u8 *obj, s32 arg1);                   /* site veneer -> Object_SetMode */

void Effect_SpawnObject222(void)
{
    u8 *obj;

    if ((gIw & 3) != 0) {
        return;
    }
    obj = Story_unk61_3(222, gOv2[0], gOv2[1], gOv2[2]);
    if (obj == 0) {
        return;
    }
    {
        u16 *p = (u16 *)(obj + 100);
        u16 v = 30;
        *p = v;
    }
    {
        u16 *q = (u16 *)(obj + 102);
        u16 w = 1;
        *q = w;
    }
    *(s32 *)(obj + 104) = 20;
    Story_unk63_3(obj);
    *(s32 *)(obj + 108) = 0x0200aa49;
    Story_unk64_3(obj, 1);
}

/* overlays/scene/story/set_piece/field_scene.c */

extern u8 gVal[];
extern u8 gOv[];
extern u8 gWork[];
extern u8 gVal2[];
extern s16 gCell[][1];
extern u8 Value_00000056;

u8 *Story_unk65_3();

u8 *Story_unk66_3();

u8 *Story_unk67_3();

u8 *Story_unk68_3();

u8 *Story_unk69_3();

u8 *Story_unk70_3();

u8 *Story_unk71_3();

u8 *Story_unk72_3();
u8 *Story_unk73_3();

u8 *Story_unk74_3();
u8 *Story_unk75_3();

/* Byte at offset 0x22B of the shared work buffer. */

void Scene_RunEarlySequence(void)
{
    s32 i;
    s32 record;
    u8 *p5;
    u8 *rec;
    s32 v;

    p5 = *(u8 **)0x03001e70;
    Story_unk76_3(230);
    Story_Place(0x20000, 0x20000, 0x10000);
    Story_unk77_3(10);
    rec = p5 + 356;
    for (i = 0; i <= 23; i++) {
        *(volatile s32 *)(rec + 12) += -0x10000;
        Story_unk78_3(4);
        if (i == 8) {
            record = Story_Check(8);
            *(volatile s32 *)(record + 24) = 0x1999;
            record = Story_unk46(8);
            *(volatile s32 *)(record + 28) = 0x1999;
            Story_unk2_3(8, 0x980000, 0xd80000);
            Story_Run(8, 0x200bd48);
        }
    }
    Story_unk79_3(1, 0, 0x20086a1);
    *(u16 *)gOv = 0;
    do {
        Story_unk80_3(1);
        v = *(u16 *)gOv + 1;
        *(u16 *)gOv = (u16)v;
    } while ((u32)(v << 16) <= 0x640000);
    Story_unk81_3(1);
    Story_unk82_3(1, 0, 0);
    Story_Do(0x121);
    Story_unk3_3(-1, -1, 0xe666);
    Story_unk83_3(30);
    Story_SetRect(0, 0, 1, 2, 3, 14);
    Story_unk2_2(0x8fd);
}

void Story_unk84_3(void)
{
    s32 i;
    u8 *p5;
    u8 *rec;
    s32 v;

    p5 = *(u8 **)0x03001e70;
    Story_unk2_5(93, 41, 16, 4, 77, 28);
    Story_unk85_3(230);
    Story_unk4_3(0x20000, 0x20000, 0x10000);
    Story_unk86_3(10);
    rec = p5 + 356;
    for (i = 23; i >= 0; i--) {
        *(volatile s32 *)(rec + 12) += -0x10000;
        Story_unk87_3(4);
    }
    Story_unk88_3(1, 0, 0x20086a1);
    *(u16 *)gOv = 0;
    do {
        Story_unk89_3(1);
        v = *(u16 *)gOv + 1;
        *(u16 *)gOv = (u16)v;
    } while ((u32)(v << 16) <= 0x640000);
    Story_unk90_3(1);
    Story_unk91_3(1, 0, 0);
    Story_unk3_2(0x121);
    Story_unk5_3(-1, -1, 0xe666);
    Story_unk92_3(30);
    Story_unk3_5(77, 41, 16, 4, 77, 28);
    Story_unk4_2(0x8fe);
}

void Story_unk93_3(void)
{
    s32 i;
    u8 *p8;
    u8 *rec;
    s32 v;

    p8 = *(u8 **)0x03001e70;
    Story_unk4_5(113, 31, 103, 17, 1, 1);
    Story_unk5_5(111, 32, 104, 18, 3, 2);
    Story_unk6_5(64, 32, 103, 18, 1, 2);
    Story_unk94_3(230);
    Story_unk6_3(0x20000, 0x20000, 0x10000);
    Story_unk95_3(10);
    rec = p8 + 356;
    for (i = 23; i >= 0; i--) {
        *(volatile s32 *)(rec + 12) += -0x10000;
        Story_unk96_3(4);
    }
    Story_unk97_3(1, 0, 0x20086a1);
    *(u16 *)gOv = 0;
    do {
        Story_unk98_3(1);
        v = *(u16 *)gOv + 1;
        *(u16 *)gOv = (u16)v;
    } while ((u32)(v << 16) <= 0x640000);
    Story_unk99_3(1);
    Story_unk100_3(1, 0, 0);
    Story_unk5_2(0x121);
    Story_unk7_3(-1, -1, 0xe666);
    Story_unk101_3(30);
    Story_unk7_5(103, 14, 103, 17, 4, 3);
    Story_unk6_2(0x907);
}

void Scene_RunScene3a4SequenceB(void)
{
    u32 i;
    s32 record;

    if (Story_unk2(0x323) != 0) {
        Story_unk8_5(2, 0, 1, 1, 24, 80);
        Story_unk9_5(2, 1, 24, 11, 1, 2);
        Story_unk7_2(0x323);
    } else {
        Story_unk10_5(0, 0, 1, 1, 24, 80);
        Story_unk11_5(0, 1, 24, 11, 1, 2);
        Story_unk8_2(0x323);
    }
}

void Scene_RunScene3a4SequenceA(void)
{
    u32 i;
    s32 record;

    if (Story_unk3(0x325) != 0) {
        Story_unk12_5(12, 72, 1, 1, 11, 73);
        Story_unk13_5(48, 32, 11, 4, 1, 2);
        Story_unk9_2(0x325);
    } else {
        Story_unk14_5(10, 72, 1, 1, 11, 73);
        Story_unk15_5(49, 32, 11, 4, 1, 2);
        Story_unk10_2(0x325);
    }
}

void Scene_RunLine1528Sequence(void)
{
    Story_unk102_3();
    Story_Apply8(0x1528, 1);
    Story_unk27_2(125);
    Story_unk103_3();
    Story_unk28_2(20);
    Story_unk104_3();
    Story_unk105_3();
}

void Scene_RunScene3a4SequenceC(void)
{
    u32 i;
    s32 record;

    Story_unk106_3();
    Story_unk2_4(0x1528, 1);
    Story_unk107_3(125);
    if (Story_unk4(0x326) != 0) {
        Story_unk16_5(15, 93, 1, 1, 16, 92);
        Story_unk17_5(47, 29, 16, 28, 1, 2);
        Story_unk11_2(0x326);
    } else {
        Story_unk18_5(17, 93, 1, 1, 16, 92);
        Story_unk19_5(46, 29, 16, 28, 1, 2);
        Story_unk12_2(0x326);
    }
    Story_unk108_3(20);
    Story_unk109_3();
    Story_unk110_3();
}

void Scene_RunScene3a4SequenceD(void)
{
    u32 i;
    s32 record;

    Story_unk111_3();
    Story_unk3_4(0x1528, 1);
    Story_unk112_3(125);
    if (Story_unk5(0x327) != 0) {
        Story_unk20_5(28, 82, 1, 1, 29, 81);
        Story_unk21_5(47, 28, 29, 17, 1, 2);
        Story_unk13_2(0x327);
    } else {
        Story_unk22_5(30, 82, 1, 1, 29, 81);
        Story_unk23_5(46, 28, 29, 17, 1, 2);
        Story_unk14_2(0x327);
    }
    Story_unk113_3(20);
    Story_unk114_3();
    Story_unk115_3();
}

void Scene_RunScene3a4(void)
{
    u32 i;
    s32 record;

    Story_unk116_3();
    Story_unk24_5(24, 27, 2, 1, 24, 26);
    Story_unk117_3(185);
    Story_unk8_3(10, 0x3333, 0x1999);
    Story_unk9_3(0, 0x3333, 0x1999);
    *(u8 *)(Story_unk47(10) + 90) &= 254;
    Story_unk118_3(0, 8);
    Story_unk10_3(0, 0x190, 0x1a8);
    Story_unk11_3(10, 0x198, 0x1a8);
    Story_unk119_3(10);
    Story_unk120_3(0, 1);
    Story_unk121_3();
    Story_unk122_3();
}

void Scene_RunSharedSetPiece(s32 a0)
{
    u32 i;
    s32 rec;
    s32 rec2;
    u8 *rec4;
    s32 rec8;
    u8 *record;
    s32 none;
    s32 base5_200abe1;
    s32 base6_ffc00000;
    s32 slot8;
    s32 zero;

    slot8 = a0;
    rec2 = Story_unk6(0);
    rec = Story_unk7(8);
    rec4 = Story_unk8(9);
    rec8 = Story_unk48(10);
    Story_unk4_4(0, 0x102);
    Story_unk123_3(40);
    Story_unk5_4(0x10000, 0x2000);
    Story_unk6_4(0x3100000, -1, 0x740000, 1);
    Story_unk12_3(0, 0x20000, 0x10000);
    Story_unk124_3(0, 6);
    Story_unk13_3(0, 0x318, 140);
    Story_unk125_3(0, 1);
    Story_unk14_3(0, 0xc000, 100);
    Story_unk15_3(0, 0x101, 60);
    Story_unk126_3(183);
    Story_unk16_3(0x30000, 0x30000, 0x10000);
    Story_unk127_3(20);
    *(s32 *)((s32)rec4 + 24) = 0x13333;
    *(s32 *)((s32)rec4 + 28) = 0x13333;
    {
        u8 *p = &rec4[35];
        u8 two = 2;
        u8 value = *(volatile u8 *)p;

        none = 0;
        *p = (u8)(value | two);
    }
    *(s32 *)((s32)rec4 + 108) = 0x2008099;
    Story_unk128_3(8, 4);
    *(volatile s32 *)(rec + 68) = 0x8000;
    *(volatile s32 *)(rec + 8) = 0x3120000;
    *(volatile s32 *)(rec + 12) = 0x200000;
    *(volatile s32 *)(rec + 16) = 0x5a0000;
    *(volatile s32 *)(rec + 24) = 0x20000;
    *(volatile s32 *)(rec + 28) = 0x20000;
    Story_unk129_3(10);
    Story_unk130_3(183);
    Story_unk17_3(0x40000, 0x20000, 0x10000);
    Story_unk131_3(20);
    *(s32 *)(rec8 + 8) += 0xe0000;
    *(s32 *)(rec8 + 12) += -0x80000;
    {
        s32 target = *(s32 *)(rec8 + 80);
        s32 shown = 0xc000;

        *(volatile u16 *)(target + 30) = shown;
    }
    Story_unk132_3(107);
    Story_unk18_3(0x10000, 0x10000, 0x10000);
    Story_unk19_3(0, 0x102, 80);
    Story_unk133_3(55);
    Story_unk20_3(0x10000, 0x30000, 0x10000);
    Story_unk134_3(8, 0);
    Story_unk135_3(0, 0);
    Story_unk7_4(0, 0x101);
    Story_unk21_3(0, 0x28000, 0x14000);
    *(volatile u16 *)(rec2 + 100) = none;
    Story_unk8_4(0, 0x200bdec);
    if (Story_unk9(0x205) != 0) {
        Story_unk22_3(1, 0x36e0000, 0x2100000);
        record = Story_unk65_3(1);
        {
            s32 shown = 0x5000;

            *(u16 *)((s32)record + 6) = shown;
        }
    }
    Story_unk9_4(0x14000, 0x2800);
    Story_unk10_4(0x3120000, -1, 0x22c0000, 1);
    Story_unk136_3(slot8);
    Story_unk137_3(8, 1);
    Story_unk23_3(8, 0x195c2, 0xcae1);
    *(volatile u16 *)(rec + 100) = none;
    Story_unk138_3(8, 0x200bd78);
    do {
        Story_unk139_3(1);
    } while (*(s16 *)(rec2 + 100) == 0);
    ((void (*)())Story_unk49)(0, 0);
    do {
        Story_unk140_3(1);
    } while (*(s16 *)(rec + 100) == 0);
    Story_unk141_3(0, 2);
    {
        u8 *record = Story_unk66_3(0);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | 1);
    }
    Story_unk15_2(0x121);
    zero = 0;
    Story_unk24_3(-1, -1, 0xe666);
    *(s32 *)((s32)rec4 + 8) = 0x3120000;
    base6_ffc00000 = -0x400000;
    *(s32 *)((s32)rec4 + 108) = zero;
    *(s32 *)((s32)rec4 + 16) = 0x26a0000;
    *(s32 *)((s32)rec4 + 12) = -0x400000;
    Story_unk25_3(8, 0x19999, 0xcccc);
    *(volatile s32 *)(rec + 68) = 0x1999;
    *(volatile s32 *)(rec + 72) = 0x3333;
    *(volatile s32 *)(rec + 40) = 0x40000;
    Story_unk26_3(8, 0x312, 0x25c);
    Story_unk27_3(8, 0x33333, 0x19999);
    Story_unk28_3(8, 0x312, 0x284);
    Story_unk142_3(15);
    Story_unk29_3(0x50000, 0x70000, 0x10000);
    Story_unk25_5(25, 36, 43, 36, 11, 9);
    Story_unk26_5(25, 35, 10, 5, 43, 35);
    Story_unk143_3(8, 0, 0);
    Story_unk144_3(9, 0, 0);
    base5_200abe1 = (s32)Story_unk145_3;
    Story_unk11_4(base5_200abe1, 0xc80);
    Story_unk146_3(80);
    Story_unk147_3(base5_200abe1);
    Story_unk148_3(60);
    Story_unk149_3(17);
    Story_unk30_3(-1, -1, 0xe666);
    Story_unk150_3(120);
    if (Story_unk10(0x205) != 0) {
        Story_unk31_3(1, 0x10000, 0x8000);
        Story_unk32_3(1, 0x338, 0x22e);
    }
    Story_unk33_3(0, 0x9999, 0x4ccc);
    Story_unk34_3(0, 0x356, 0x248);
    if (Story_unk11(0x205) != 0) {
        Story_unk151_3(1, 1);
        Story_unk35_3(1, 0x4000, 0);
    }
    Story_unk36_3(0, 0x6000, 40);
    Story_unk37_3(1, 0x102, 0);
    Story_unk38_3(0, 0x102, 60);
    Story_unk12_4(0x3140000, base6_ffc00000, 0x2620000, 1);
    Story_unk152_3();
    Story_unk153_3(148);
    Story_unk154_3(240);
    if (Story_unk12(0x205) != 0) {
        Story_unk13_4(0x40000, 0x8000);
        Story_unk14_4(0x3560000, 0, 0x2480000, 1);
        Story_unk155_3();
        Story_unk39_3(1, 0x348, 0x228);
        Story_unk40_3(1, 0x356, 0x232);
        Story_unk156_3(1, 2);
        record = Story_unk13(0);
        if ((s32)record != 0) {
            Story_unk157_3(1, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
        }
        Story_unk158_3(1);
        Story_unk159_3(1, 0, 0);
    }
    Story_unk160_3();
    Story_unk16_2(0x908);
}

void Scene_RunScene3a4(void)
{
    u32 i;
    u8 *record;
    s32 base5_1953;

    Story_unk161_3();
    base5_1953 = (s32)gVal2;
    Story_unk14(base5_1953, 1);
    if (Story_unk15(0x908) != 0) {
    } else {
        if (Story_unk16(0xf14) != 0) {
        } else {
            Story_unk17_2(0x205);
            Story_unk41_3(0, 0xcccc, 0x6666);
            Story_unk42_3(0, 0x316, 140);
            Story_unk43_3(0, 0x30c, 140);
            Story_unk44_3(0, 0xc000, 0);
            record = Story_unk17(0);
            if ((s32)record != 0) {
                Story_unk162_3(1, *(volatile s32 *)((s32)record + 8), *(volatile s32 *)((s32)record + 16));
            }
            Story_unk45_3(1, 0xcccc, 0x6666);
            Story_unk46_3(1, 0x320, 140);
            Story_unk47_3(1, 0xc000, 20);
            Story_unk163_3((base5_1953 + 1));
            Story_unk164_3(1, 4);
            Story_unk165_3(20);
            Story_unk166_3(1, 0, 10);
            Story_unk167_3(1, 6, 0);
            Story_unk48_3(1, 0x19999, 0xcccc);
            *(u8 *)(Story_unk50(1) + 90) &= 254;
            Story_unk49_3(1, 0x318, 110);
            Story_unk168_3(1);
            *(u8 *)(Story_unk51(1) + 90) |= 1;
            Story_unk169_3(161);
            Story_unk50_3(0x20000, 0x10000, 0x10000);
            *(u8 *)(Story_unk52(1) + 90) &= 254;
            Story_unk51_3(1, 0x318, 120);
            Story_unk170_3(1);
            {
                u8 *record = Story_unk67_3(1);
                u8 value = *(volatile u8 *)&record[90];

                record[90] = (u8)(value | 1);
            }
            Story_unk52_2(-1, -1, 0xe666);
            Story_unk171_3(80);
            Story_unk172_3(141);
            Story_unk53_2(0x10000, 0x10000, 0x10000);
            Story_unk173_3(40);
            Story_unk54_2(0, 0x101, 0);
            Story_unk55_2(1, 0x101, 60);
            Story_unk56_2(0, 0x8000, 0);
            Story_unk174_3(1, 0, 20);
            Story_unk175_3(0, 0, 0);
            Story_unk57_2(1, 0x8000, 40);
            Story_unk58_2(0, 0x8000, 0);
            Story_unk176_3(1, 0, 40);
            Story_unk59_2(0, 0xc000, 0);
            Story_unk60_2(1, 0xc000, 40);
            Story_unk61_2(1, 0x102, 60);
            Story_unk62_2(1, 0x4000, 20);
            Story_unk177_3(1, 2);
            Story_unk178_3(1, 0, 10);
            Story_unk63_2(1, 0x28000, 0x14000);
            Story_unk179_3(1, 5);
            Story_unk64_2(1, 0x31c, 138);
            Story_unk180_3(0, 0, 0);
            Story_unk65_2(1, 0x324, 140);
            Story_unk66_2(0, 0x4000, 0);
            Story_unk67_2(1, 0x324, 166);
            Story_unk68_2(1, 0x2fc, 166);
            Story_unk69_2(1, 0x2fc, 198);
            Story_unk70_2(1, 0x312, 198);
            Story_unk71_2(0, 0x102, 0);
            Story_unk72_2(1, 0x312, 246);
            Story_unk181_3(1, 1);
            Story_unk182_3(1, 0, 0);
            Story_unk183_3(40);
            Story_unk184_3(10);
        }
    }
    Story_unk185_3();
}

void Scene_RunScene3a4(void)
{
    extern u8 gCell2[];

    u32 i;
    s32 record;

    if (Story_unk18(0x8fe) != 0) {
        *(volatile u16 *)(*(volatile s32 *)0x03001e70 + 20) &= 0xfdff;
        Story_unk186_3(9, 0, 0);
    } else {
        Story_unk187_3();
        if (Story_unk19(0x109) == 0) {
            if (gCell[225][0] != 99) {
                goto L_0200235a;
            }
            Story_unk188_3();
        } else {
            L_0200235a:;
            Story_unk27_5(38, 24, 1, 2, 37, 24);
            Story_unk28_5(44, 23, 1, 2, 45, 23);
            if (Story_unk20(0x8fe) == 0) {
                Story_unk189_3(9, 2);
                Story_unk190_3(9, 3);
                Story_unk191_2(0xee0000, 0, 0x1a20000, 0x8000);
            }
        }
    }
    if (Story_unk21(0x323) != 0) {
        Story_unk29_5(0, 0, 1, 1, 24, 80);
        Story_unk30_5(0, 1, 24, 11, 1, 2);
    } else {
        Story_unk31_5(2, 0, 1, 1, 24, 80);
        Story_unk32_5(2, 1, 24, 11, 1, 2);
    }
}

void Scene_RunScene3a4(void)
{
    extern u8 gCell2[];

    if (Story_unk22(0x8fe) != 0) {
        *(volatile u16 *)(*(volatile s32 *)0x03001e70 + 20) &= 0xfdff;
    } else {
        Story_unk33_5(52, 42, 1, 1, 53, 42);
    }
    {
        s32 index = 225;
        if ((u32)((((u16 *)gCell2)[index] - 6) << 16) <= 0x10000) {
            Story_unk18_2(0x12f);
        }
    }
}

void Scene_RunScene3a4(void)
{
    extern u8 gCell2[];

    u32 i;
    u8 *record;

    if (Story_unk23(0x907) != 0) {
        *(volatile u16 *)(*(volatile s32 *)0x03001e70 + 20) &= 0xfdff;
        Story_unk192_2(10, 0, 0);
    } else {
        if (Story_unk24(0x109) == 0) {
            if (gCell[225][0] == 99) {
                Story_unk193_2();
            }
        }
        Story_unk194_2();
        if (Story_unk25(0x907) == 0) {
            Story_unk195_2(10, 2);
            Story_unk196_2(10, 3);
            Story_unk15_4(0x2ec0000, 0x80000, 0x1180000, 0x8000);
        }
    }
    Story_unk197_2(9);
    if (Story_unk26(0x200) != 0) {
        Story_unk198_2(9, 5);
        Story_unk34_5(23, 13, 1, 1, 25, 13);
        {
            u8 *record = Story_unk68_3(9);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | 2);
        }
    }
    if (Story_unk27(0x325) != 0) {
        Story_unk35_5(10, 72, 1, 1, 11, 73);
        Story_unk36_5(49, 32, 11, 4, 1, 2);
    } else {
        Story_unk37_5(12, 72, 1, 1, 11, 73);
        Story_unk38_5(48, 32, 11, 4, 1, 2);
    }
}

void Scene_RunScene3a4(void)
{
    extern u8 gCell2[];

    u32 i;
    u8 *rec7;
    s32 record;

    if (gCell[225][0] == 2) {
        if (Story_unk28(0x109) == 0) {
            Story_unk73_2(8, 0x1660000, 0x680000);
        }
    }
    Story_unk199_2(9);
    if (Story_unk29(0x200) != 0) {
        rec7 = Story_unk69_3(9);
        Story_unk200_2(9, 5);
        Story_unk39_5(45, 41, 1, 1, 43, 41);
        {
            u8 value = *(volatile u8 *)&rec7[35];

            rec7[35] = (u8)(value | 2);
        }
    }
    if (Story_unk30(0x907) != 0) {
        *(volatile u16 *)(*(volatile s32 *)0x03001e70 + 20) &= 0xfdff;
    }
    if (Story_unk31(0x326) != 0) {
        Story_unk40_5(17, 93, 1, 1, 16, 92);
        Story_unk41_5(46, 29, 16, 28, 1, 2);
    } else {
        Story_unk42_5(15, 93, 1, 1, 16, 92);
        Story_unk43_5(47, 29, 16, 28, 1, 2);
    }
}

void Scene_RunScene3a4(void)
{
    u32 i;
    u8 *record;

    record = Story_unk70_3(9);
    Story_unk201_2((s32)record, 0);
    Story_unk202_2();
    Story_unk203_2(9);
    if (Story_unk32(0x200) != 0) {
        Story_unk204_2(9, 5);
        Story_unk44_5(0, 0, 1, 1, 26, 26);
        {
            u8 *record = Story_unk71_3(9);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | 2);
        }
    }
    Story_unk205_2(11);
    if (Story_unk33(0x201) != 0) {
        Story_unk16_4(11, 5);
        Story_unk45_5(1, 0, 1, 1, 17, 10);
        {
            u8 *record = Story_unk72_3(11);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | 2);
        }
    }
    Story_unk206_2(12);
    if (Story_unk34(0x204) != 0) {
        Story_unk207_2(12, 5);
        Story_unk46_5(1, 0, 1, 1, 26, 15);
        {
            u8 *record = Story_unk75_3(12);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | 2);
        }
    }
    Story_unk17_4(0x200b429, 0xc80);
    if (Story_unk35(0x327) != 0) {
        Story_unk47_5(30, 82, 1, 1, 29, 81);
        Story_unk48_5(46, 28, 29, 17, 1, 2);
    } else {
        Story_unk49_5(28, 82, 1, 1, 29, 81);
        Story_unk50_5(47, 28, 29, 17, 1, 2);
    }
}

void Scene_RunScene3a4(void)
{
    extern s16 gCell2[];

    s32 rec7;
    s32 record;
    s16 flag;

    rec7 = Story_unk36(0x909);
    if (rec7 != 0) {
        Story_unk208_2(8, 0, 0);
        ((void (*)())Story_unk53)(9, 0, 0);
    } else {
        record = Story_unk54(8);
        Story_unk209_2(record, 0);
        Story_unk210_2(9, 3);
        record = Story_unk55(9);
        Story_unk211_2(record, 0);
        *(u8 *)(Story_unk56(9) + 89) = rec7;
    }
    flag = gCell2[225];
    if (flag == 1 || flag == 98) {
        if (Story_unk37(0x109) == 0) {
            rec7 = Story_unk38(0);
            Story_unk212_2();
            *(volatile s32 *)(rec7 + 12) = 0x100000;
            Story_unk213_2();
        }
    } else if (flag == 99) {
        if (Story_unk39(0x109) == 0) {
            Story_unk214_2();
        }
    }
    /* unlifted: 0x020029bc..0x020029c2 (2) */
}

void Scene_RunScene3a4(void)
{
    extern s16 gCell2[];

    s32 record;

    record = Story_unk57(9);
    Story_unk215_2(record, 0);
    if (gCell2[225] == 2) {
        Story_unk74_2(9, 0xb80000, 0x1480000);
    }
}

void Scene_RunScene3a4SequenceG(void)
{
    u32 i;
    u8 *rec7;
    s32 record;
    s32 value;
    s32 base6_3001e40;

    base6_3001e40 = 0x3001e40;
    if (Story_unk40(*(volatile s32 *)base6_3001e40, 3) == 0) {
        value = Story_unk18_4();
        rec7 = Story_unk41(200, ((((u32)(((value << 1) + value) << 4) >> 16) << 16) + 0x2fd0000), -0x400000, 0x2600000);
        if ((s32)rec7 != 0) {
            if (Story_unk42(*(volatile s32 *)base6_3001e40, 9) == 0) {
                {
                    s32 v2 = Story_unk58();
                    if (((u32)(v2 << 1) >> 16) != 0) {
                        Story_unk216_2(145);
                    } else {
                        Story_unk217_2(144);
                    }
                }
            }
            rec7[85] = 0;
            {
                s32 v3 = Story_unk59();
                s32 tmp2824 = (((u32)(v3 << 15) >> 16) + 0x4ccc);
                *(volatile s32 *)((s32)rec7 + 72) = 0x6666;
                *(s32 *)((s32)rec7 + 28) = tmp2824;
                *(s32 *)((s32)rec7 + 24) = tmp2824;
            }
            rec7[97] = 1;
            Story_unk218_2((s32)rec7, 0);
            rec7[35] &= 254;
            {
                u8 *p80 = (u8 *)(*(volatile s32 *)((s32)rec7 + 80));
                s32 mask9 = -13;
                p80[9] = (mask9 & p80[9]) | 4;
            }
            Story_unk219_2((s32)rec7, 1);
            Story_unk19_4((s32)rec7, 0x200c01c);
            value = Story_unk20_4();
            *(volatile s32 *)((s32)rec7 + 36) = ((((u32)(((value << 1) + value) << 1) >> 16) - 3) << 16);
            *(volatile s32 *)((s32)rec7 + 40) = 0x80000;
            value = Story_unk21_4();
            *(volatile s32 *)((s32)rec7 + 44) = (((u32)(((value << 1) + value) << 9) >> 16) + -0x300);
        }
    }
}

void Scene_RunOpeningAuxiliarySequence(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Story_unk43(0);
    Story_unk220_2();
    Story_unk221_2(10, 0, 0);
    Story_unk222_2(8, 0, 0);
    *(s32 *)(*(u8 *volatile *)gWork + 0x1c0) = 0x201;
    Story_unk223_2();
    Story_unk224_2();
    Story_unk225_2(20);
    Story_unk226_2(202, 3);
    Story_unk44(202, 0);
    *(u8 *)(Story_unk60() + 85) = 0;
    Story_unk22_4(0x19999, 0x3333);
    Story_unk23_4(0x640000, 0, 0xf90000, 1);
    Story_unk227_2();
    Story_unk228_2(20);
    Story_unk229_2();
    Story_unk230_2(*(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12), *(s32 *)(rec7 + 16), 1);
    Story_unk231_2();
    Story_unk19_2(0x200cd6c);
    *(s32 *)(*(u8 *volatile *)gWork + 0x1c0) = 0x204;
    Story_unk232_2();
}

void Scene_RunScene3a4SequenceF(void)
{
    u32 i;
    s32 record;
    s32 v6;

    Story_unk233_2();
    Story_unk234_2(9, 0, 0);
    Story_unk75_2(8, 0x1480000, 0x1a80000);
    Story_unk20_2(0x323);
    Story_unk235_2();
    Story_unk236_2();
    v6 = 224;
    Story_unk237_2(1);
    *(volatile s32 *)(*(volatile s32 *)gWork + (v6 << 1)) = 0x201;
    Story_unk238_2();
    Story_unk239_2();
    Story_unk240_2();
    *(volatile s32 *)(*(volatile s32 *)gWork + (v6 << 1)) = 0x204;
    Story_unk241_2();
}

void Scene_RunScene3a4SequenceE(void)
{
    u32 i;
    s32 record;
    s32 v6;

    Story_unk242_2();
    Story_unk243_2(10, 0, 0);
    Story_unk76_2(8, 0x1e80000, 0x8a0000);
    Story_unk21_2(0x325);
    Story_unk244_2();
    Story_unk245_2();
    v6 = 224;
    Story_unk246_2(1);
    *(volatile s32 *)(*(volatile s32 *)gWork + (v6 << 1)) = 0x201;
    Story_unk247_2();
    Story_unk248_2();
    Story_unk249_2();
    *(volatile s32 *)(*(volatile s32 *)gWork + (v6 << 1)) = 0x204;
    Story_unk250_2();
}

/* Runs a fixed chain of 19 calls with literal arguments, sets byte 0x22B of
 * the shared work buffer to 3, then issues two more calls. */
void Scene_RunLateAuxiliarySequence(void)
{
    extern u8 gCell2[];

    Battle_Reset_1();
    Motion_SetSpeedLim_1(39321, 4915);
    Motion_CamBounds_1(21495808, -1, 5701632, 1);
    Motion_SetSpeed_1(0, 39321, 19660);
    Motion_SetPosReset_1(0, 328, 116);
    Audio_PlayCue_1(148);
    Story_unk24_4(33599213, 3200); /* main:080000d0 */
    Story_unk77_2(65536, 65536, 65536); /* main:080091f0 */
    Motion_SetSpeed_2(8, 6553, 3276);
    Motion_SetSpeed_3(9, 6553, 3276);
    Object_SetModeById_1(8, 2);
    Motion_ResetAndSetPosition_1(8, 328, 104);
    Motion_ResetAndSetPosition_2(9, 328, 108);
    Battle_WaitMode0_1(60);
    BattleFx_SpawnLinked_1(0, 256, 0);
    Motion_SetVarCb_1(0, 2);
    Motion_CommitPos_1(8);
    do {
        WORK_BYTE_22B = 3;
    } while (0);
    SharedWorkData_SetFirstAndSecondFields_1((s32)&Value_00000056, 99);
    BattleFx_ComputeWeightedResultAndDispatch_1(53, 3);
}

/* overlays/scene/story/set_piece/overlay_object.c */
/*
 * Per-frame integrator for one actor record in resource_3a4. Advances the
 * position pair at +8 and +12, advances +24 and +28 by one shared velocity,
 * damps that velocity, and returns 0.
 *
 * The damping subtracts +72 from the value of +40 already held in a register,
 * not from a fresh load; v28 and v2c carry those earlier reads and must stay
 * locals rather than become repeated loads.
 */
s32 OvObj_IntegrateAndDamp(u8 *p)
{
    s32 v28;
    s32 v2c;

    *(s32 *)(p + 8) = *(s32 *)(p + 8) + *(s32 *)(p + 36);

    v28 = *(s32 *)(p + 40);
    *(s32 *)(p + 12) = *(s32 *)(p + 12) + v28;

    v2c = *(s32 *)(p + 44);
    *(s32 *)(p + 24) = *(s32 *)(p + 24) + v2c;
    *(s32 *)(p + 28) = *(s32 *)(p + 28) + v2c;

    *(s32 *)(p + 40) = v28 - *(s32 *)(p + 72);

    return 0;
}

/* overlays/scene/story/set_piece/scene_audio.c */
extern s32 gOv3;   /* In-image writable data at image offset 0x51b0. */

/*
 * Overlay resource_3a4. Per-frame tick that counts to sixty, fires one
 * audio cue and rewinds itself. The address of this routine is stored in
 * a record as a plain word, so it runs as a published callback.
 */

/* Plays a sound cue; the name is this site's own call word, not a runtime
 * address. */

/*
 * The owner spans the code, one alignment halfword and its one pool word,
 * thirty-six bytes in all. Story_unk251_2 names the loader-relocated call
 * word for the cue call, not a runtime address. The limit of sixty reads
 * as one second of frames, but nothing here fixes a frame rate.
 */
void SceneAudio_PlayCue183EverySixtyTicks(void)
{
    gOv3 = gOv3 + 1;
    if (gOv3 == 60) {
        Story_unk251_2(183);
        gOv3 = 0;
    }
}

/*
 * Stamp a fixed value into the caller's record at +102, then play sound cue
 * 288. The owner at 0x02003724 in resource_3a4 is 20 bytes with no literal
 * pool.
 */
void State_SetRecordWord102AndPlayCue288(u16 *record)
{
    record = (u16 *)((char *)record + 102);
    {
        s32 value = 0x21;

        *record = value;
    }
    Story_unk252_2(288);
}

/* overlays/scene/story/set_piece/scene_data.c */
extern s16 gCell2[];
extern u8 Value_0000004d;
extern u8 Value_0000004e;
extern u8 Value_0000004f;
extern u8 Value_00000050;
extern u8 Value_00000051;
extern u8 Value_00000052;
extern u8 Value_00000053;
extern u8 Value_00000054;
extern u8 Value_00000055;
extern u8 Value_00000056;
extern u8 Value_00000057;
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gOv6[];
extern u8 gOv7[];
extern u8 gOv8[];
extern u8 gOv9[];
extern u8 gOv10[];
extern u8 gOv11[];
extern u8 gOv12[];
extern u8 gOv13[];
extern u8 gOv14[];
extern u8 gOv15[];
extern u8 gOv16[];
extern u8 gOv17[];
extern u8 gOv18[];   /* Image offset 0x485c. */
extern u8 gOv19[];
extern u8 gOv20[];
extern u8 gOv21[];
extern u8 gOv22[];
extern u8 gOv23[];
extern u8 gOv24[];
extern u8 gOv25[];
extern u8 gOv26[];
extern u8 gOv27[];
extern u8 gOv28[];

/*
 * Table getter for resource_3a4, published from the overlay's header as an
 * entry point.
 *
 * The eight-byte owner at 0x02000204 includes its one pool word at
 * 0x02000208; the load reads that word and returns it as an address,
 * without dereferencing it.
 */

s32 SceneData_SelectTableByWord224(void)
{
    s16 v = gCell2[224];

    if (v == (s32)&Value_0000004d) {
        return (s32)gOv4;
    }
    if (v == (s32)&Value_0000004e) {
        return (s32)gOv5;
    }
    if (v == (s32)&Value_0000004f) {
        return (s32)gOv6;
    }
    if (v == (s32)&Value_00000050) {
        return (s32)gOv7;
    }
    if (v == (s32)&Value_00000051) {
        return (s32)gOv8;
    }
    if (v == (s32)&Value_00000052) {
        return (s32)gOv9;
    }
    if (v == (s32)&Value_00000053) {
        return (s32)gOv10;
    }
    if (v == (s32)&Value_00000054) {
        return (s32)gOv11;
    }
    if (v == (s32)&Value_00000055) {
        return (s32)gOv12;
    }
    if (v == (s32)&Value_00000056) {
        return (s32)gOv13;
    }
    if (v == (s32)&Value_00000057) {
        return (s32)gOv14;
    }
    return (s32)gOv15;
}

s32 SceneData_SelectTableC80cOrC83c(void)
{
    s16 v = gCell2[224];

    if (v == (s32)&Value_00000055) {
        return (s32)gOv16;
    }
    if (v == (s32)&Value_00000056) {
        return (s32)gOv17;
    }
    return 0;
}

u8 *SceneData_GetTableC85c(void)
{
    return gOv18;
}

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    s16 v = gCell2[224];

    if (v == (s32)&Value_0000004d) {
        return (s32)gOv19;
    }
    if (v == (s32)&Value_0000004f) {
        return (s32)gOv20;
    }
    if (v == (s32)&Value_00000051) {
        return (s32)gOv21;
    }
    if (v == (s32)&Value_00000052) {
        return (s32)gOv22;
    }
    if (v == (s32)&Value_00000053) {
        return (s32)gOv23;
    }
    if (v == (s32)&Value_00000054) {
        return (s32)gOv24;
    }
    if (v == (s32)&Value_00000055) {
        return (s32)gOv25;
    }
    if (v == (s32)&Value_00000056) {
        return (s32)gOv26;
    }
    if (v == (s32)&Value_00000057) {
        return (s32)gOv27;
    }
    return (s32)gOv28;
}

/* overlays/scene/story/set_piece/scene_primary_script.c */
#define SCENE_PHASE (*(volatile s32 *)(work + 0x1c0))
#define SCENE_FIELD_1C8 (*(volatile s32 *)(work + 0x1c8))

extern u8 gOv29[];
extern u8 gOv30[];
extern u8 gWork[];

/* Phase/status word at 0x1c0, and a companion word at 0x1c8, of the shared
 * scene work record. */

/* Remaining raw call sites, named from the engine function's own source. */

/* Record returned by Scene_GetRecord_1/26/3a: a pair of s32 fields at +8 and
 * +16 that get forwarded straight into the matching setup call. */
void RunEventScript01(void)
{
    u32 i;
    s32 record;
    u8 *work;
    s32 addr;
    s32 addr2;
    s32 addr3;

    Battle_Reset_1();
    Motion_SetHPosTerrain_1(8, 0x1480000, 0x580000);
    Motion_SetHPosTerrain_2(9, 0x1480000, 0x580000);
    Object_SetModeById_1(8, 0);
    work = *(u8 *volatile *)gWork;
    SCENE_PHASE = 0x100;
    SCENE_FIELD_1C8 = 40;
    Battle_WaitMode0_1();
    Motion_SetSpeed_1(); /* main:0808a370 */
    Battle_WaitMode0_2(20);
    record = Scene_GetRecord_1(0);
    if (record != 0) {
        Motion_SetHPosTerrain_3(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        Motion_SetHPosTerrain_4(2, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Scene_GetRecord_3(0);
    if (record != 0) {
        Motion_SetHPosTerrain_5(3, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Motion_SetSpeed_2(1, 0x9999, 0x4ccc);
    Motion_SetSpeed_3(2, 0x9999, 0x4ccc);
    Motion_SetSpeed_4(3, 0x9999, 0x4ccc);
    Motion_EnableActCb_1(1, 0x200c054);
    Motion_EnableActCb_2(2, 0x200c084);
    Motion_MarkActiveAndSetActionCallback_1(3, 0x200c0b4);
    Motion_ArmCb_1(1, 0xc000, 0);
    Motion_ArmCb_2(2, 0xc000, 0);
    Motion_ArmCb_3(3, 0xc000, 40);
    Motion_SetVarCbObj_1(1, 1);
    Motion_ArmCb_4(1, 0xe000, 10);
    SceneWork_SetStepValue_1(0x190c);
    Battle_RunThenWaitIfModeZero_1(1, 0, 10);
    Motion_SetVarCbObj_2(2, 1);
    Motion_ArmCb_5(2, 0xa000, 10);
    Motion_ArmCb_6(0, 0x2000, 0);
    Motion_SetSpeed_5(2, 0); /* main:0808a178 */
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        Motion_CallWaitAnim_1(2, 3);
    } else {
        Motion_CallWaitAnim_2(2, 4);
        bump_step(1);
    }
    Battle_RunThenWaitIfModeZero_2(2, 0, 20);
    Object_SetModeById_2(0, 3);
    Object_SetModeById_3(1, 3);
    Object_SetModeById_4(0, 3);
    Motion_CallWaitAnim_3(0, 3);
    Battle_WaitMode0_3(20);
    Motion_SetVarCbObj_3(3, 2);
    SceneWork_SetStepValue_2(0x1910);
    BattleEv_RunWait_1(3, 0);
    Motion_ArmCb_7(0, 0x4000, 10);
    BattleFx_SpawnLinked_1(1, 0x102, 60);
    Motion_ArmCb_8(1, 0x2000, 40);
    BattleEv_RunWait_2(1, 0);
    Motion_ArmCb_9(0, 0x6000, 10);
    BattleFx_SpawnLinked_2(0, 0x101, 0);
    BattleFx_SpawnLinked_3(1, 0x101, 0);
    BattleFx_SpawnLinked_4(2, 0x101, 0);
    BattleFx_SpawnLinked_5(3, 0x101, 40);
    Audio_PlayCue_1(190);
    ObjectGroup_ConfigureChildValue_1(8, 7);
    Battle_WaitMode0_4(10);
    Audio_PlayCue_2(0x121);
    Motion_ArmCb_10(0, 0xc000, 0);
    Motion_ArmCb_11(1, 0xc000, 0);
    Motion_ArmCb_12(2, 0xc000, 0);
    Motion_ArmCb_13(3, 0xc000, 0);
    BattleFx_SpawnLinked_6(0, 0x100, 0);
    BattleFx_SpawnLinked_7(1, 0x100, 0);
    BattleFx_SpawnLinked_8(2, 0x100, 0);
    BattleFx_SpawnLinked_9(3, 0x100, 40);
    Audio_PlayCue_3(103);
    addr = (s32)Story_unk253_2;
    Story_unk25_4(addr, 0xc80); /* main:080000d0 */
    addr2 = (s32)gOv29;
    Motion_EnableActCb_3(9, addr2);
    Motion_MarkActiveAndSetActionCallback_2(8, addr2);
    Story_unk254_2(addr); /* main:080000d8 */
    Battle_WaitMode0_5(60);
    Motion_SetVarCbObj_4(2, 2);
    Battle_WaitMode0_6(20);
    BattleEv_RunWait_3(2, 0);
    Motion_ArmCb_14(1, 0xe000, 0);
    Motion_ArmCb_15(2, 0xa000, 20);
    Object_SetModeById_5(1, 3);
    Object_SetModeById_6(2, 3);
    Motion_CallWaitAnim_4(3, 3);
    Battle_WaitMode0_7(20);
    Motion_SetVarCbObj_5(3, 1);
    Battle_RunThenWaitIfModeZero_3(3, 0, 20);
    Motion_ArmCb_16(1, 0xe000, 0);
    Motion_ArmCb_17(0, 0x6000, 40);
    Motion_CallWaitAnim_5(1, 3);
    Battle_RunThenWaitIfModeZero_4(1, 0, 10);
    Motion_CallWaitAnim_6(0, 3);
    Object_SetModeById_7(1, 3);
    Object_SetModeById_8(2, 3);
    Motion_CallWaitAnim_7(3, 3);
    Battle_WaitMode0_8(20);
    addr3 = (s32)gOv30;
    Motion_EnableActCb_4(1, addr3);
    Motion_EnableActCb_5(2, addr3);
    Motion_MarkActiveAndSetActionCallback_3(3, addr3);
    Battle_WaitMode0_9(20);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x204;
    GameFlag_Clear_1(0x12f);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c8)) = 16;
    GameFlag_Set_1(0x909);
    Battle_SchedShoulder_1();
}

/* overlays/scene/story/set_piece/scene_setup.c */
extern s32 gOv2[3];
extern s32 gOv31;

s32 Story_unk255_2();           /* per-site veneer -> Story_unk256_2 */

s32 Story_unk257_2();           /* per-site veneer -> Story_unk258_2 */

void Scene_RunValue1528Scene(void)
{
    Story_unk259_2();
    Story_Apply9(0x1528, 1);
    Story_unk29_2(125);
    Story_unk260_2();
    Story_unk30_2(20);
    Story_unk261_2();
    Story_unk262_2();
}

void State_StoreParamsAndInstallTask(s32 v0, s32 v1, s32 v2, s32 v3)
{
    gOv2[0] = v0;
    gOv2[1] = v1;
    gOv2[2] = v2;
    gOv31 = v3;

    Story_unk255_2(170);
    Story_unk257_2(0x0200ab59, 0xc80);
}

/* overlays/scene/story/set_piece/scene_state_interaction.c */
extern u8 gCell2[];
extern u8 Value_00000051;

void State_SetByte22bTo3AndSend51(void)
{
    gCell2[0x22b] = 3;
    Story_Apply10((s32)&Value_00000051, 99);
    Story_Apply11(53, 2);
}

/* overlays/scene/story/set_piece/state_update.c */

extern u8 gCell2[];
extern u8 Value_0000004d;
extern u8 Value_0000004f;
extern s32 gIw;
extern u8 *gWork;   /* The scene workspace pointer. */
extern u8 Value_00001018;   /* The pooled constant 0x1018. */

s32 *Story_unk263_2(s32);

s32 *Story_unk264_2(s32);

s32 *Story_unk265_2(s32);

/*
 * Read the low three bits of the runtime word at 0x03001e40 and, on two of the
 * eight values, forward the incoming argument. The 40-byte owner includes its
 * alignment halfword and one pool word. The argument is never materialised in
 * the body, yet it is live at both call sites, so the parameter must stay. The
 * meaning of the selector is not established; the other values fall through
 * deliberately.
 */

/* Two call words that reach the same target. */

/*
 * Write 0x1018 into the halfword at scene workspace + 382. The 24-byte owner
 * includes its two-word literal pool holding 0x03001ebc and 0x00001018, both
 * read by pc-relative loads in the row. The displacement is built as 191 << 1
 * and is a plain displacement, never used as a value. 0x1018 is loaded from
 * the pool rather than built. No arguments and no return value.
 */

void State_SetValue14Mode23(void)
{
    Story_Apply12(0xE, 0x17);
}

void State_SetWorkByte22bTo3(void)
{
    gCell2[0x22b] = 3;
    Story_Apply13((s32)&Value_0000004d, 99);
    Story_Apply14(53, 2);
}

void State_SetByte22bTo3(void)
{
    gCell2[0x22b] = 3;
    Story_Apply15((s32)&Value_0000004f, 99);
    Story_Apply16(53, 2);
}

void State_ApplyRectAndSetActor9Byte55(void)
{
    s32 *p;
    s32 s0;
    s32 s1;

    p = Story_unk263_2(9);
    s0 = 17;
    s1 = 13;
    Story_unk57_4(29, 1, 3, 1, s0, s1);
    if (p != 0) {
        ((u8 *)p)[0x55] = 2;
    }
    Story_unk31_2(0x201);
}

void State_ApplyRectAndLowerActor9(void)
{
    s32 *rec;
    s32 s0;
    s32 s1;

    rec = Story_unk264_2(9);
    s0 = 43;
    s1 = 41;
    Story_unk58_4(45, 41, 1, 1, s0, s1);
    if (rec != 0) {
        Story_Apply17(rec, 0);
        rec[3] += 0xffe00000;
        ((u8 *)rec)[0x23] = 2;
    }
    Story_unk32_2(0x200);
}

void Actor_AdjustSlot12AndSetFlag204(void)
{
    s32 *p;
    s32 s0;
    s32 s1;

    p = Story_unk265_2(12);
    s0 = 26;
    s1 = 15;
    Story_unk59_4(1, 0, 1, 1, s0, s1);
    if (p != 0) {
        Story_Apply18(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    Story_unk33_2(0x204);
}

void State_ForwardByRuntimeSelector(s32 arg)
{
    s32 sel = gIw & 7;

    if (sel == 0) {
        Story_unk266_2(arg, 2);
    } else if (sel == 2) {
        Story_unk267_2(arg, 0);
    }
}

void State_SetWorkspaceHalfword382To1018(void)
{
    *(u16 *)(gWork + (191 << 1)) = (int)&Value_00001018;
}
