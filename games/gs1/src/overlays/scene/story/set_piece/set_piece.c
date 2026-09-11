#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/story/set_piece/set_piece.h"
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
void Story_Run26();          /* site veneer -> Story_Run27 */

void Story_Run28();          /* site veneer -> Story_Run29 */

void Story_Run30();          /* site veneer -> Story_Run31 */

void Story_Run32();          /* site veneer -> Story_Run33 (1st) */

void Story_Run34();          /* site veneer -> Story_Run35 */

void Story_Run36();          /* site veneer -> Story_Run33 (2nd) */

void Story_Run37();          /* site veneer -> Story_Run38 */

void Story_Run39();          /* site veneer -> Story_Run33 (3rd) */

void Story_Run40();          /* site veneer -> Story_Run41 */

void Story_Run42();          /* site veneer -> Audio_PlayCue */

void Story_Run43();          /* site veneer -> Story_Run33 (4th) */

void Scene_RunActorTenFourStepSequence(void)
{
    Story_Run26(24, 1);
    Story_Run28(10, 9);
    Story_Run30();
    Story_Run32(10, 2);
    Story_Run34(1);
    Story_Run36(10, 2);
    Story_Run37();
    Story_Run39(10, 2);
    Story_Run40();
    Story_Run42(288);
    Story_Run43(10, 2);
}

/* overlays/scene/story/set_piece/actor_position.c */
struct Frame {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct Frame *Story_Run44(s32);
s32 *Story_Run45(s32);

s32 *Story_Run46(s32);

s32 *Story_Run47(s32);

s32 *Story_Run48(s32);

/*
 * resource_3a4 owner at 0x02003460, 34 bytes with no literal pool: clear
 * bit 0 of the scene record's flags byte at +89, then place a marker at the
 * record's x and z at level 255. Both callees are reached through veneers.
 */

u8 *Story_Run49();           /* Scene record accessor. */

void Story_Run50();          /* Place a marker: kind, x, z, level. */

s32 Actor_CopyActor8PositionWithFixedY(struct Frame *dst)
{
    struct Frame *src = Story_Run44(8);

    dst->f08 = src->f08;
    dst->f0c = 0xFFF40000;
    dst->f10 = src->f10;
    return 0;
}

void Actor_UpdateSlot10ByTileX(void)
{
    s32 *a = Story_Run45(10);

    if (a != 0) {
        s32 x = 24;
        s32 y = 26;
        s32 t;

        Story_SetRect51(x, 27, 2, 1, x, y);
        t = a[2] >> 20;
        if (t == 25) {
            Story_SetRect52(0, 0, 1, 1, t, y);
        } else {
            Story_SetRect53(0, 0, 1, 1, x, y);
        }
        Story_Apply(a, 0);
        ((u8 *)a)[0x55] = 0;
        Story_Run51();
        Story_Do22(1);
    }
}

void Actor_RaiseSlot9StepA(void)
{
    s32 *p;
    s32 s0;

    p = Story_Run46(9);
    s0 = 26;
    Story_SetRect54(0, 0, 1, 1, s0, s0);
    if (p != 0) {
        Story_Apply2(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    Story_Do23(0x200);
}

void Actor_RaiseSlot9StepB(void)
{
    s32 *p;
    s32 s0;
    s32 s1;

    p = Story_Run47(9);
    s0 = 25;
    s1 = 13;
    Story_SetRect55(23, 13, 1, 1, s0, s1);
    if (p != 0) {
        Story_Apply3(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    Story_Do24(0x200);
}

void Actor_RaiseSlot11AndSetFlag201(void)
{
    s32 *p;
    s32 s0;
    s32 s1;

    p = Story_Run48(11);
    s0 = 17;
    s1 = 10;
    Story_Check45(1, 0, 1, 1, s0, s1);
    if (p != 0) {
        Story_Apply4(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    Story_Do25(0x201);
}

void Actor_ClearCollisionFlagAndPlaceMarker(s32 no)
{
    u8 *record;

    record = Story_Run49(no);
    record[89] &= 0xfe;

    Story_Run50(0, *(s32 *)(record + 8), *(s32 *)(record + 16), 255);
}

/* overlays/scene/story/set_piece/actor_presentation.c */
extern unsigned char Value_00004ccc;
extern u32 gIw;

s32 *Story_Run52(s32);

u8 *Story_Run53(s32);
u8 *Story_Run54(s32);

u8 *Story_Run55(s32 id);
u8 *Story_Run56(s32 id);

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

    p = Story_Run52(8);
    s0 = 9;
    s1 = 13;
    Story_SetRect56(7, 13, 1, 1, s0, s1);
    if (p != 0) {
        Story_Apply5(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    Story_Do26(0x200);
}

void Actor_SetActor10Byte23To3(void)
{
    Story_Run53(10)[0x23] = 3;
}

void Actor_SetActor10Byte23To1(void)
{
    Story_Run54(10)[0x23] = 1;
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
    Story_Run57(rec, 3);
    Story_Run58(rec, 0);
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
        Story_Run59(8, 7);
    } else {
        Story_Run60(8, 6);
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
    if (*(s32 *)(Story_Run55(0) + 12) > 0x00300000) {
        {
            u8 *flag = Story_Run56(11) + 35;
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

    if ((u32)(Story_Check45() * 100) >> 16 >= gOv) {
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
extern u8 *Story_Run61(s32 arg0, s32 arg1, s32 arg2, s32 arg3);  /* site veneer -> Story_Run62 */
extern void Story_Run63(u8 *obj);                             /* site veneer -> Actor_SetMode3AndRate4ccc */
extern void Story_Run64(u8 *obj, s32 arg1);                   /* site veneer -> Object_SetMode */

void Effect_SpawnObject222(void)
{
    u8 *obj;

    if ((gIw & 3) != 0) {
        return;
    }
    obj = Story_Run61(222, gOv2[0], gOv2[1], gOv2[2]);
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
    Story_Run63(obj);
    *(s32 *)(obj + 108) = 0x0200aa49;
    Story_Run64(obj, 1);
}

/* overlays/scene/story/set_piece/field_scene.c */

extern u8 gVal[];
extern u8 gOv[];
extern u8 gWork[];
extern u8 gVal2[];
extern s16 gCell[][1];
extern u8 Value_00000056;

u8 *Story_Run65();

u8 *Story_Run66();

u8 *Story_Run67();

u8 *Story_Run68();

u8 *Story_Run69();

u8 *Story_Run70();

u8 *Story_Run71();

u8 *Story_Run72();
u8 *Story_Run73();

u8 *Story_Run74();
u8 *Story_Run75();

/* Byte at offset 0x22B of the shared work buffer. */

void Scene_RunEarlySequence(void)
{
    s32 i;
    s32 record;
    u8 *p5;
    u8 *rec;
    s32 v;

    p5 = *(u8 **)0x03001e70;
    Story_Run76(230);
    Story_Place(0x20000, 0x20000, 0x10000);
    Story_Run77(10);
    rec = p5 + 356;
    for (i = 0; i <= 23; i++) {
        *(volatile s32 *)(rec + 12) += -0x10000;
        Story_Run78(4);
        if (i == 8) {
            record = Story_Check(8);
            *(volatile s32 *)(record + 24) = 0x1999;
            record = Story_Check46(8);
            *(volatile s32 *)(record + 28) = 0x1999;
            Story_Place2(8, 0x980000, 0xd80000);
            Story_Run(8, 0x200bd48);
        }
    }
    Story_Run79(1, 0, 0x20086a1);
    *(u16 *)gOv = 0;
    do {
        Story_Run80(1);
        v = *(u16 *)gOv + 1;
        *(u16 *)gOv = (u16)v;
    } while ((u32)(v << 16) <= 0x640000);
    Story_Run81(1);
    Story_Run82(1, 0, 0);
    Story_Do(0x121);
    Story_Place3(-1, -1, 0xe666);
    Story_Run83(30);
    Story_SetRect(0, 0, 1, 2, 3, 14);
    Story_Do2(0x8fd);
}

void Story_Run84(void)
{
    s32 i;
    u8 *p5;
    u8 *rec;
    s32 v;

    p5 = *(u8 **)0x03001e70;
    Story_SetRect2(93, 41, 16, 4, 77, 28);
    Story_Run85(230);
    Story_Place4(0x20000, 0x20000, 0x10000);
    Story_Run86(10);
    rec = p5 + 356;
    for (i = 23; i >= 0; i--) {
        *(volatile s32 *)(rec + 12) += -0x10000;
        Story_Run87(4);
    }
    Story_Run88(1, 0, 0x20086a1);
    *(u16 *)gOv = 0;
    do {
        Story_Run89(1);
        v = *(u16 *)gOv + 1;
        *(u16 *)gOv = (u16)v;
    } while ((u32)(v << 16) <= 0x640000);
    Story_Run90(1);
    Story_Run91(1, 0, 0);
    Story_Do3(0x121);
    Story_Place5(-1, -1, 0xe666);
    Story_Run92(30);
    Story_SetRect3(77, 41, 16, 4, 77, 28);
    Story_Do4(0x8fe);
}

void Story_Run93(void)
{
    s32 i;
    u8 *p8;
    u8 *rec;
    s32 v;

    p8 = *(u8 **)0x03001e70;
    Story_SetRect4(113, 31, 103, 17, 1, 1);
    Story_SetRect5(111, 32, 104, 18, 3, 2);
    Story_SetRect6(64, 32, 103, 18, 1, 2);
    Story_Run94(230);
    Story_Place6(0x20000, 0x20000, 0x10000);
    Story_Run95(10);
    rec = p8 + 356;
    for (i = 23; i >= 0; i--) {
        *(volatile s32 *)(rec + 12) += -0x10000;
        Story_Run96(4);
    }
    Story_Run97(1, 0, 0x20086a1);
    *(u16 *)gOv = 0;
    do {
        Story_Run98(1);
        v = *(u16 *)gOv + 1;
        *(u16 *)gOv = (u16)v;
    } while ((u32)(v << 16) <= 0x640000);
    Story_Run99(1);
    Story_Run100(1, 0, 0);
    Story_Do5(0x121);
    Story_Place7(-1, -1, 0xe666);
    Story_Run101(30);
    Story_SetRect7(103, 14, 103, 17, 4, 3);
    Story_Do6(0x907);
}

void Scene_RunScene3a4SequenceB(void)
{
    u32 i;
    s32 record;

    if (Story_Check2(0x323) != 0) {
        Story_SetRect8(2, 0, 1, 1, 24, 80);
        Story_SetRect9(2, 1, 24, 11, 1, 2);
        Story_Do7(0x323);
    } else {
        Story_SetRect10(0, 0, 1, 1, 24, 80);
        Story_SetRect11(0, 1, 24, 11, 1, 2);
        Story_Do8(0x323);
    }
}

void Scene_RunScene3a4SequenceA(void)
{
    u32 i;
    s32 record;

    if (Story_Check3(0x325) != 0) {
        Story_SetRect12(12, 72, 1, 1, 11, 73);
        Story_SetRect13(48, 32, 11, 4, 1, 2);
        Story_Do9(0x325);
    } else {
        Story_SetRect14(10, 72, 1, 1, 11, 73);
        Story_SetRect15(49, 32, 11, 4, 1, 2);
        Story_Do10(0x325);
    }
}

void Scene_RunLine1528Sequence(void)
{
    Story_Run102();
    Story_Apply8(0x1528, 1);
    Story_Do27(125);
    Story_Run103();
    Story_Do28(20);
    Story_Run104();
    Story_Run105();
}

void Scene_RunScene3a4SequenceC(void)
{
    u32 i;
    s32 record;

    Story_Run106();
    Story_Run2(0x1528, 1);
    Story_Run107(125);
    if (Story_Check4(0x326) != 0) {
        Story_SetRect16(15, 93, 1, 1, 16, 92);
        Story_SetRect17(47, 29, 16, 28, 1, 2);
        Story_Do11(0x326);
    } else {
        Story_SetRect18(17, 93, 1, 1, 16, 92);
        Story_SetRect19(46, 29, 16, 28, 1, 2);
        Story_Do12(0x326);
    }
    Story_Run108(20);
    Story_Run109();
    Story_Run110();
}

void Scene_RunScene3a4SequenceD(void)
{
    u32 i;
    s32 record;

    Story_Run111();
    Story_Run3(0x1528, 1);
    Story_Run112(125);
    if (Story_Check5(0x327) != 0) {
        Story_SetRect20(28, 82, 1, 1, 29, 81);
        Story_SetRect21(47, 28, 29, 17, 1, 2);
        Story_Do13(0x327);
    } else {
        Story_SetRect22(30, 82, 1, 1, 29, 81);
        Story_SetRect23(46, 28, 29, 17, 1, 2);
        Story_Do14(0x327);
    }
    Story_Run113(20);
    Story_Run114();
    Story_Run115();
}

void Scene_RunScene3a4(void)
{
    u32 i;
    s32 record;

    Story_Run116();
    Story_SetRect24(24, 27, 2, 1, 24, 26);
    Story_Run117(185);
    Story_Place8(10, 0x3333, 0x1999);
    Story_Place9(0, 0x3333, 0x1999);
    *(u8 *)(Story_Check47(10) + 90) &= 254;
    Story_Run118(0, 8);
    Story_Place10(0, 0x190, 0x1a8);
    Story_Place11(10, 0x198, 0x1a8);
    Story_Run119(10);
    Story_Run120(0, 1);
    Story_Run121();
    Story_Run122();
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
    rec2 = Story_Check6(0);
    rec = Story_Check7(8);
    rec4 = Story_Check8(9);
    rec8 = Story_Check48(10);
    Story_Run4(0, 0x102);
    Story_Run123(40);
    Story_Run5(0x10000, 0x2000);
    Story_Run6(0x3100000, -1, 0x740000, 1);
    Story_Place12(0, 0x20000, 0x10000);
    Story_Run124(0, 6);
    Story_Place13(0, 0x318, 140);
    Story_Run125(0, 1);
    Story_Place14(0, 0xc000, 100);
    Story_Place15(0, 0x101, 60);
    Story_Run126(183);
    Story_Place16(0x30000, 0x30000, 0x10000);
    Story_Run127(20);
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
    Story_Run128(8, 4);
    *(volatile s32 *)(rec + 68) = 0x8000;
    *(volatile s32 *)(rec + 8) = 0x3120000;
    *(volatile s32 *)(rec + 12) = 0x200000;
    *(volatile s32 *)(rec + 16) = 0x5a0000;
    *(volatile s32 *)(rec + 24) = 0x20000;
    *(volatile s32 *)(rec + 28) = 0x20000;
    Story_Run129(10);
    Story_Run130(183);
    Story_Place17(0x40000, 0x20000, 0x10000);
    Story_Run131(20);
    *(s32 *)(rec8 + 8) += 0xe0000;
    *(s32 *)(rec8 + 12) += -0x80000;
    {
        s32 target = *(s32 *)(rec8 + 80);
        s32 shown = 0xc000;

        *(volatile u16 *)(target + 30) = shown;
    }
    Story_Run132(107);
    Story_Place18(0x10000, 0x10000, 0x10000);
    Story_Place19(0, 0x102, 80);
    Story_Run133(55);
    Story_Place20(0x10000, 0x30000, 0x10000);
    Story_Run134(8, 0);
    Story_Run135(0, 0);
    Story_Run7(0, 0x101);
    Story_Place21(0, 0x28000, 0x14000);
    *(volatile u16 *)(rec2 + 100) = none;
    Story_Run8(0, 0x200bdec);
    if (Story_Check9(0x205) != 0) {
        Story_Place22(1, 0x36e0000, 0x2100000);
        record = Story_Run65(1);
        {
            s32 shown = 0x5000;

            *(u16 *)((s32)record + 6) = shown;
        }
    }
    Story_Run9(0x14000, 0x2800);
    Story_Run10(0x3120000, -1, 0x22c0000, 1);
    Story_Run136(slot8);
    Story_Run137(8, 1);
    Story_Place23(8, 0x195c2, 0xcae1);
    *(volatile u16 *)(rec + 100) = none;
    Story_Run138(8, 0x200bd78);
    do {
        Story_Run139(1);
    } while (*(s16 *)(rec2 + 100) == 0);
    ((void (*)())Story_Check49)(0, 0);
    do {
        Story_Run140(1);
    } while (*(s16 *)(rec + 100) == 0);
    Story_Run141(0, 2);
    {
        u8 *record = Story_Run66(0);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | 1);
    }
    Story_Do15(0x121);
    zero = 0;
    Story_Place24(-1, -1, 0xe666);
    *(s32 *)((s32)rec4 + 8) = 0x3120000;
    base6_ffc00000 = -0x400000;
    *(s32 *)((s32)rec4 + 108) = zero;
    *(s32 *)((s32)rec4 + 16) = 0x26a0000;
    *(s32 *)((s32)rec4 + 12) = -0x400000;
    Story_Place25(8, 0x19999, 0xcccc);
    *(volatile s32 *)(rec + 68) = 0x1999;
    *(volatile s32 *)(rec + 72) = 0x3333;
    *(volatile s32 *)(rec + 40) = 0x40000;
    Story_Place26(8, 0x312, 0x25c);
    Story_Place27(8, 0x33333, 0x19999);
    Story_Place28(8, 0x312, 0x284);
    Story_Run142(15);
    Story_Place29(0x50000, 0x70000, 0x10000);
    Story_SetRect25(25, 36, 43, 36, 11, 9);
    Story_SetRect26(25, 35, 10, 5, 43, 35);
    Story_Run143(8, 0, 0);
    Story_Run144(9, 0, 0);
    base5_200abe1 = (s32)Story_Run145;
    Story_Run11(base5_200abe1, 0xc80);
    Story_Run146(80);
    Story_Run147(base5_200abe1);
    Story_Run148(60);
    Story_Run149(17);
    Story_Place30(-1, -1, 0xe666);
    Story_Run150(120);
    if (Story_Check10(0x205) != 0) {
        Story_Place31(1, 0x10000, 0x8000);
        Story_Place32(1, 0x338, 0x22e);
    }
    Story_Place33(0, 0x9999, 0x4ccc);
    Story_Place34(0, 0x356, 0x248);
    if (Story_Check11(0x205) != 0) {
        Story_Run151(1, 1);
        Story_Place35(1, 0x4000, 0);
    }
    Story_Place36(0, 0x6000, 40);
    Story_Place37(1, 0x102, 0);
    Story_Place38(0, 0x102, 60);
    Story_Run12(0x3140000, base6_ffc00000, 0x2620000, 1);
    Story_Run152();
    Story_Run153(148);
    Story_Run154(240);
    if (Story_Check12(0x205) != 0) {
        Story_Run13(0x40000, 0x8000);
        Story_Run14(0x3560000, 0, 0x2480000, 1);
        Story_Run155();
        Story_Place39(1, 0x348, 0x228);
        Story_Place40(1, 0x356, 0x232);
        Story_Run156(1, 2);
        record = Story_Check13(0);
        if ((s32)record != 0) {
            Story_Run157(1, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
        }
        Story_Run158(1);
        Story_Run159(1, 0, 0);
    }
    Story_Run160();
    Story_Do16(0x908);
}

void Scene_RunScene3a4(void)
{
    u32 i;
    u8 *record;
    s32 base5_1953;

    Story_Run161();
    base5_1953 = (s32)gVal2;
    Story_Check14(base5_1953, 1);
    if (Story_Check15(0x908) != 0) {
    } else {
        if (Story_Check16(0xf14) != 0) {
        } else {
            Story_Do17(0x205);
            Story_Place41(0, 0xcccc, 0x6666);
            Story_Place42(0, 0x316, 140);
            Story_Place43(0, 0x30c, 140);
            Story_Place44(0, 0xc000, 0);
            record = Story_Check17(0);
            if ((s32)record != 0) {
                Story_Run162(1, *(volatile s32 *)((s32)record + 8), *(volatile s32 *)((s32)record + 16));
            }
            Story_Place45(1, 0xcccc, 0x6666);
            Story_Place46(1, 0x320, 140);
            Story_Place47(1, 0xc000, 20);
            Story_Run163((base5_1953 + 1));
            Story_Run164(1, 4);
            Story_Run165(20);
            Story_Run166(1, 0, 10);
            Story_Run167(1, 6, 0);
            Story_Place48(1, 0x19999, 0xcccc);
            *(u8 *)(Story_Check50(1) + 90) &= 254;
            Story_Place49(1, 0x318, 110);
            Story_Run168(1);
            *(u8 *)(Story_Check51(1) + 90) |= 1;
            Story_Run169(161);
            Story_Place50(0x20000, 0x10000, 0x10000);
            *(u8 *)(Story_Check52(1) + 90) &= 254;
            Story_Place51(1, 0x318, 120);
            Story_Run170(1);
            {
                u8 *record = Story_Run67(1);
                u8 value = *(volatile u8 *)&record[90];

                record[90] = (u8)(value | 1);
            }
            Story_Place52(-1, -1, 0xe666);
            Story_Run171(80);
            Story_Run172(141);
            Story_Place53(0x10000, 0x10000, 0x10000);
            Story_Run173(40);
            Story_Place54(0, 0x101, 0);
            Story_Place55(1, 0x101, 60);
            Story_Place56(0, 0x8000, 0);
            Story_Run174(1, 0, 20);
            Story_Run175(0, 0, 0);
            Story_Place57(1, 0x8000, 40);
            Story_Place58(0, 0x8000, 0);
            Story_Run176(1, 0, 40);
            Story_Place59(0, 0xc000, 0);
            Story_Place60(1, 0xc000, 40);
            Story_Place61(1, 0x102, 60);
            Story_Place62(1, 0x4000, 20);
            Story_Run177(1, 2);
            Story_Run178(1, 0, 10);
            Story_Place63(1, 0x28000, 0x14000);
            Story_Run179(1, 5);
            Story_Place64(1, 0x31c, 138);
            Story_Run180(0, 0, 0);
            Story_Place65(1, 0x324, 140);
            Story_Place66(0, 0x4000, 0);
            Story_Place67(1, 0x324, 166);
            Story_Place68(1, 0x2fc, 166);
            Story_Place69(1, 0x2fc, 198);
            Story_Place70(1, 0x312, 198);
            Story_Place71(0, 0x102, 0);
            Story_Place72(1, 0x312, 246);
            Story_Run181(1, 1);
            Story_Run182(1, 0, 0);
            Story_Run183(40);
            Story_Run184(10);
        }
    }
    Story_Run185();
}

void Scene_RunScene3a4(void)
{
    extern u8 gCell2[];

    u32 i;
    s32 record;

    if (Story_Check18(0x8fe) != 0) {
        *(volatile u16 *)(*(volatile s32 *)0x03001e70 + 20) &= 0xfdff;
        Story_Run186(9, 0, 0);
    } else {
        Story_Run187();
        if (Story_Check19(0x109) == 0) {
            if (gCell[225][0] != 99) {
                goto L_0200235a;
            }
            Story_Run188();
        } else {
            L_0200235a:;
            Story_SetRect27(38, 24, 1, 2, 37, 24);
            Story_SetRect28(44, 23, 1, 2, 45, 23);
            if (Story_Check20(0x8fe) == 0) {
                Story_Run189(9, 2);
                Story_Run190(9, 3);
                Story_Run191(0xee0000, 0, 0x1a20000, 0x8000);
            }
        }
    }
    if (Story_Check21(0x323) != 0) {
        Story_SetRect29(0, 0, 1, 1, 24, 80);
        Story_SetRect30(0, 1, 24, 11, 1, 2);
    } else {
        Story_SetRect31(2, 0, 1, 1, 24, 80);
        Story_SetRect32(2, 1, 24, 11, 1, 2);
    }
}

void Scene_RunScene3a4(void)
{
    extern u8 gCell2[];

    if (Story_Check22(0x8fe) != 0) {
        *(volatile u16 *)(*(volatile s32 *)0x03001e70 + 20) &= 0xfdff;
    } else {
        Story_SetRect33(52, 42, 1, 1, 53, 42);
    }
    {
        s32 index = 225;
        if ((u32)((((u16 *)gCell2)[index] - 6) << 16) <= 0x10000) {
            Story_Do18(0x12f);
        }
    }
}

void Scene_RunScene3a4(void)
{
    extern u8 gCell2[];

    u32 i;
    u8 *record;

    if (Story_Check23(0x907) != 0) {
        *(volatile u16 *)(*(volatile s32 *)0x03001e70 + 20) &= 0xfdff;
        Story_Run192(10, 0, 0);
    } else {
        if (Story_Check24(0x109) == 0) {
            if (gCell[225][0] == 99) {
                Story_Run193();
            }
        }
        Story_Run194();
        if (Story_Check25(0x907) == 0) {
            Story_Run195(10, 2);
            Story_Run196(10, 3);
            Story_Run15(0x2ec0000, 0x80000, 0x1180000, 0x8000);
        }
    }
    Story_Run197(9);
    if (Story_Check26(0x200) != 0) {
        Story_Run198(9, 5);
        Story_SetRect34(23, 13, 1, 1, 25, 13);
        {
            u8 *record = Story_Run68(9);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | 2);
        }
    }
    if (Story_Check27(0x325) != 0) {
        Story_SetRect35(10, 72, 1, 1, 11, 73);
        Story_SetRect36(49, 32, 11, 4, 1, 2);
    } else {
        Story_SetRect37(12, 72, 1, 1, 11, 73);
        Story_SetRect38(48, 32, 11, 4, 1, 2);
    }
}

void Scene_RunScene3a4(void)
{
    extern u8 gCell2[];

    u32 i;
    u8 *rec7;
    s32 record;

    if (gCell[225][0] == 2) {
        if (Story_Check28(0x109) == 0) {
            Story_Place73(8, 0x1660000, 0x680000);
        }
    }
    Story_Run199(9);
    if (Story_Check29(0x200) != 0) {
        rec7 = Story_Run69(9);
        Story_Run200(9, 5);
        Story_SetRect39(45, 41, 1, 1, 43, 41);
        {
            u8 value = *(volatile u8 *)&rec7[35];

            rec7[35] = (u8)(value | 2);
        }
    }
    if (Story_Check30(0x907) != 0) {
        *(volatile u16 *)(*(volatile s32 *)0x03001e70 + 20) &= 0xfdff;
    }
    if (Story_Check31(0x326) != 0) {
        Story_SetRect40(17, 93, 1, 1, 16, 92);
        Story_SetRect41(46, 29, 16, 28, 1, 2);
    } else {
        Story_SetRect42(15, 93, 1, 1, 16, 92);
        Story_SetRect43(47, 29, 16, 28, 1, 2);
    }
}

void Scene_RunScene3a4(void)
{
    u32 i;
    u8 *record;

    record = Story_Run70(9);
    Story_Run201((s32)record, 0);
    Story_Run202();
    Story_Run203(9);
    if (Story_Check32(0x200) != 0) {
        Story_Run204(9, 5);
        Story_SetRect44(0, 0, 1, 1, 26, 26);
        {
            u8 *record = Story_Run71(9);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | 2);
        }
    }
    Story_Run205(11);
    if (Story_Check33(0x201) != 0) {
        Story_Run16(11, 5);
        Story_SetRect45(1, 0, 1, 1, 17, 10);
        {
            u8 *record = Story_Run72(11);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | 2);
        }
    }
    Story_Run206(12);
    if (Story_Check34(0x204) != 0) {
        Story_Run207(12, 5);
        Story_SetRect46(1, 0, 1, 1, 26, 15);
        {
            u8 *record = Story_Run75(12);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | 2);
        }
    }
    Story_Run17(0x200b429, 0xc80);
    if (Story_Check35(0x327) != 0) {
        Story_SetRect47(30, 82, 1, 1, 29, 81);
        Story_SetRect48(46, 28, 29, 17, 1, 2);
    } else {
        Story_SetRect49(28, 82, 1, 1, 29, 81);
        Story_SetRect50(47, 28, 29, 17, 1, 2);
    }
}

void Scene_RunScene3a4(void)
{
    extern s16 gCell2[];

    s32 rec7;
    s32 record;
    s16 flag;

    rec7 = Story_Check36(0x909);
    if (rec7 != 0) {
        Story_Run208(8, 0, 0);
        ((void (*)())Story_Check53)(9, 0, 0);
    } else {
        record = Story_Check54(8);
        Story_Run209(record, 0);
        Story_Run210(9, 3);
        record = Story_Check55(9);
        Story_Run211(record, 0);
        *(u8 *)(Story_Check56(9) + 89) = rec7;
    }
    flag = gCell2[225];
    if (flag == 1 || flag == 98) {
        if (Story_Check37(0x109) == 0) {
            rec7 = Story_Check38(0);
            Story_Run212();
            *(volatile s32 *)(rec7 + 12) = 0x100000;
            Story_Run213();
        }
    } else if (flag == 99) {
        if (Story_Check39(0x109) == 0) {
            Story_Run214();
        }
    }
    /* unlifted: 0x020029bc..0x020029c2 (2) */
}

void Scene_RunScene3a4(void)
{
    extern s16 gCell2[];

    s32 record;

    record = Story_Check57(9);
    Story_Run215(record, 0);
    if (gCell2[225] == 2) {
        Story_Place74(9, 0xb80000, 0x1480000);
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
    if (Story_Check40(*(volatile s32 *)base6_3001e40, 3) == 0) {
        value = Story_Run18();
        rec7 = Story_Check41(200, ((((u32)(((value << 1) + value) << 4) >> 16) << 16) + 0x2fd0000), -0x400000, 0x2600000);
        if ((s32)rec7 != 0) {
            if (Story_Check42(*(volatile s32 *)base6_3001e40, 9) == 0) {
                {
                    s32 v2 = Story_Check58();
                    if (((u32)(v2 << 1) >> 16) != 0) {
                        Story_Run216(145);
                    } else {
                        Story_Run217(144);
                    }
                }
            }
            rec7[85] = 0;
            {
                s32 v3 = Story_Check59();
                s32 tmp2824 = (((u32)(v3 << 15) >> 16) + 0x4ccc);
                *(volatile s32 *)((s32)rec7 + 72) = 0x6666;
                *(s32 *)((s32)rec7 + 28) = tmp2824;
                *(s32 *)((s32)rec7 + 24) = tmp2824;
            }
            rec7[97] = 1;
            Story_Run218((s32)rec7, 0);
            rec7[35] &= 254;
            {
                u8 *p80 = (u8 *)(*(volatile s32 *)((s32)rec7 + 80));
                s32 mask9 = -13;
                p80[9] = (mask9 & p80[9]) | 4;
            }
            Story_Run219((s32)rec7, 1);
            Story_Run19((s32)rec7, 0x200c01c);
            value = Story_Run20();
            *(volatile s32 *)((s32)rec7 + 36) = ((((u32)(((value << 1) + value) << 1) >> 16) - 3) << 16);
            *(volatile s32 *)((s32)rec7 + 40) = 0x80000;
            value = Story_Run21();
            *(volatile s32 *)((s32)rec7 + 44) = (((u32)(((value << 1) + value) << 9) >> 16) + -0x300);
        }
    }
}

void Scene_RunOpeningAuxiliarySequence(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Story_Check43(0);
    Story_Run220();
    Story_Run221(10, 0, 0);
    Story_Run222(8, 0, 0);
    *(s32 *)(*(u8 *volatile *)gWork + 0x1c0) = 0x201;
    Story_Run223();
    Story_Run224();
    Story_Run225(20);
    Story_Run226(202, 3);
    Story_Check44(202, 0);
    *(u8 *)(Story_Check60() + 85) = 0;
    Story_Run22(0x19999, 0x3333);
    Story_Run23(0x640000, 0, 0xf90000, 1);
    Story_Run227();
    Story_Run228(20);
    Story_Run229();
    Story_Run230(*(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12), *(s32 *)(rec7 + 16), 1);
    Story_Run231();
    Story_Do19(0x200cd6c);
    *(s32 *)(*(u8 *volatile *)gWork + 0x1c0) = 0x204;
    Story_Run232();
}

void Scene_RunScene3a4SequenceF(void)
{
    u32 i;
    s32 record;
    s32 v6;

    Story_Run233();
    Story_Run234(9, 0, 0);
    Story_Place75(8, 0x1480000, 0x1a80000);
    Story_Do20(0x323);
    Story_Run235();
    Story_Run236();
    v6 = 224;
    Story_Run237(1);
    *(volatile s32 *)(*(volatile s32 *)gWork + (v6 << 1)) = 0x201;
    Story_Run238();
    Story_Run239();
    Story_Run240();
    *(volatile s32 *)(*(volatile s32 *)gWork + (v6 << 1)) = 0x204;
    Story_Run241();
}

void Scene_RunScene3a4SequenceE(void)
{
    u32 i;
    s32 record;
    s32 v6;

    Story_Run242();
    Story_Run243(10, 0, 0);
    Story_Place76(8, 0x1e80000, 0x8a0000);
    Story_Do21(0x325);
    Story_Run244();
    Story_Run245();
    v6 = 224;
    Story_Run246(1);
    *(volatile s32 *)(*(volatile s32 *)gWork + (v6 << 1)) = 0x201;
    Story_Run247();
    Story_Run248();
    Story_Run249();
    *(volatile s32 *)(*(volatile s32 *)gWork + (v6 << 1)) = 0x204;
    Story_Run250();
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
    Story_Run24(33599213, 3200); /* main:080000d0 */
    Story_Place77(65536, 65536, 65536); /* main:080091f0 */
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
 * thirty-six bytes in all. Story_Run251 names the loader-relocated call
 * word for the cue call, not a runtime address. The limit of sixty reads
 * as one second of frames, but nothing here fixes a frame rate.
 */
void SceneAudio_PlayCue183EverySixtyTicks(void)
{
    gOv3 = gOv3 + 1;
    if (gOv3 == 60) {
        Story_Run251(183);
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
    Story_Run252(288);
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
    addr = (s32)Story_Run253;
    Story_Run25(addr, 0xc80); /* main:080000d0 */
    addr2 = (s32)gOv29;
    Motion_EnableActCb_3(9, addr2);
    Motion_MarkActiveAndSetActionCallback_2(8, addr2);
    Story_Run254(addr); /* main:080000d8 */
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

s32 Story_Run255();           /* per-site veneer -> Story_Run256 */

s32 Story_Run257();           /* per-site veneer -> Story_Run258 */

void Scene_RunValue1528Scene(void)
{
    Story_Run259();
    Story_Apply9(0x1528, 1);
    Story_Do29(125);
    Story_Run260();
    Story_Do30(20);
    Story_Run261();
    Story_Run262();
}

void State_StoreParamsAndInstallTask(s32 v0, s32 v1, s32 v2, s32 v3)
{
    gOv2[0] = v0;
    gOv2[1] = v1;
    gOv2[2] = v2;
    gOv31 = v3;

    Story_Run255(170);
    Story_Run257(0x0200ab59, 0xc80);
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

s32 *Story_Run263(s32);

s32 *Story_Run264(s32);

s32 *Story_Run265(s32);

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

    p = Story_Run263(9);
    s0 = 17;
    s1 = 13;
    Story_SetRect57(29, 1, 3, 1, s0, s1);
    if (p != 0) {
        ((u8 *)p)[0x55] = 2;
    }
    Story_Do31(0x201);
}

void State_ApplyRectAndLowerActor9(void)
{
    s32 *rec;
    s32 s0;
    s32 s1;

    rec = Story_Run264(9);
    s0 = 43;
    s1 = 41;
    Story_SetRect58(45, 41, 1, 1, s0, s1);
    if (rec != 0) {
        Story_Apply17(rec, 0);
        rec[3] += 0xffe00000;
        ((u8 *)rec)[0x23] = 2;
    }
    Story_Do32(0x200);
}

void Actor_AdjustSlot12AndSetFlag204(void)
{
    s32 *p;
    s32 s0;
    s32 s1;

    p = Story_Run265(12);
    s0 = 26;
    s1 = 15;
    Story_SetRect59(1, 0, 1, 1, s0, s1);
    if (p != 0) {
        Story_Apply18(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    Story_Do33(0x204);
}

void State_ForwardByRuntimeSelector(s32 arg)
{
    s32 sel = gIw & 7;

    if (sel == 0) {
        Story_Run266(arg, 2);
    } else if (sel == 2) {
        Story_Run267(arg, 0);
    }
}

void State_SetWorkspaceHalfword382To1018(void)
{
    *(u16 *)(gWork + (191 << 1)) = (int)&Value_00001018;
}
