#include "types.h"

/* overlays/scene/actor/transition/actor_depth_effect.c */
#define Scene_RunActorTenDepthSequence Func_02000568
void Func_02002982(s32);
void Func_020006fc(s32, s32, s32);
void Func_02000706(s32, s32, s32);
void Func_02000712(s32, s32, s32);
void Func_020029a8(s32);
void Func_020028be(s32);
void Func_02002804(s32);
void Func_020016fc(void);

/* Three sites of one import, so three names. */

void Scene_RunActorTenDepthSequence(void)
{
    /* r5 = -96 (movs #0x60 / negs), live across the first import call. */
    s32 depth = -96;

    Func_02002982((s32) 0xF1);
    Func_020006fc(10, 0, depth);
    Func_02000706(10, 0, depth);
    Func_02000712(10, 0, -80);
    Func_020029a8((s32) 0x121);
    Func_020028be((s32) 0x307);
    Func_02002804(2);
    Func_020016fc();
}
#undef Scene_RunActorTenDepthSequence

/* overlays/scene/actor/transition/actor_facing.c */
#define Actor_TurnTowardTableAngle Func_02002014
typedef struct {
    u8 filler0[6];
    u16 unk6;
    u8 filler8[82];
    u8 unk5A;
    u8 filler5B[9];
    u16 unk64;
} T;

extern s16 Data_0200a424[];

void Func_020042f0(T *, s32);
void Func_02004318(T *, s32);
void Func_02004328(T *, s32);

void Actor_TurnTowardTableAngle(s32 z)
{
    T *o;
    s32 t;
    s32 d;
    u16 prev;
    s32 n;

    o = (T *)z;
    n = o->unk64;
    z = 0;
    t = ((s16 *)&o->unk64)[z];
    if (t != 0) {
        o->unk64 = n - 1;
        return;
    }
    o->unk5A = t;
    z = 1;
    d = Data_0200a424[(*(u32 *)0x03001ae8 >> 4) & 0xF];
    z = -z;
    if (d == z) {
        Func_020042f0(o, 9);
        return;
    }
    prev = o->unk6;
    d = (s16)(d - prev);
    if (d > 0x1000)
        d = 0x1000;
    if (d < -0x1000)
        d = -0x1000;
    o->unk6 = prev + d;
    Func_02004318(o, 2);
    Func_02004328(o, 0x30);
}
#undef Actor_TurnTowardTableAngle

/* overlays/scene/actor/transition/actor_motion.c */
#define State_RunSlot8OffsetStep Func_0200029c
#define OvObj_AdvancePositionByDelta Func_02001bdc
/* Advance both coordinate triplets by their corresponding deltas. */
struct MovingObject_02001bdc {
    u8 unknown00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown14[4];
    s32 sub_x;
    s32 sub_y;
    u8 unknown20[0x10];
    s32 sub_dx;
    s32 sub_dy;
    u8 unknown38[0x0c];
    s32 dx;
    s32 dy;
    s32 dz;
};

void Func_020026b6(s32);
void Func_02000430(s32, s32, s32);
void Func_0200043a(s32, s32, s32);
void Func_020026d0(s32);
void Func_020025ee(s32);
void Func_0200252c(s32);
void Func_02001320(void);

void State_RunSlot8OffsetStep(void)
{
    s32 offset = 112;

    offset = -offset;
    Func_020026b6(0xF1);
    Func_02000430(8, offset, 0);
    Func_0200043a(8, offset, 0);
    Func_020026d0(0x121);
    Func_020025ee(0x301);
    Func_0200252c(2);
    Func_02001320();
}

void OvObj_AdvancePositionByDelta(struct MovingObject_02001bdc *object)
{
    object->x += object->dx;
    object->y += object->dy;
    object->z += object->dz;
    object->sub_x += object->sub_dx;
    object->sub_y += object->sub_dy;
}
#undef State_RunSlot8OffsetStep
#undef OvObj_AdvancePositionByDelta

/* overlays/scene/actor/transition/actor_position.c */
#define Actor_PlacePairAtOffset Func_02000180
#define Actor_PlaceAtTileAndMark Func_02001004
typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[28];
    s32 f30;
    s32 f34;
} Obj;

/* resource_39a owner at 0x02001004, 76 bytes.
 * Fetches a record for the caller's id and, when it exists, marks it and
 * writes two 20.12 fixed-point coordinates biased by half a unit. */
struct Rec_39a {
    u8 pad00[8];
    s32 f8;                     /* +8  */
    u8 pad0c[4];
    s32 f16;                    /* +16 */
    u8 pad14[14];
    u8 f34;                     /* +34 */
    u8 f35;                     /* +35 */
};

extern s32 Data_02000240[];

Obj *Func_020024f2(s32);
Obj *Func_020024fa(s32);
void Func_020024e8(void);
void Func_020024b2(Obj *, s32, s32, s32);
void Func_02002492(Obj *, s32);
void Func_020024de(Obj *, s32, s32, s32);
void Func_020024ca(Obj *, s32);
void Func_020024d4(Obj *, s32);
void Func_0200250a(Obj *);
void Func_0200257e(void);
struct Rec_39a *Func_02003362();
void Func_020033b8();

void Actor_PlacePairAtOffset(s32 a0, s32 a1, s32 a2)
{
    Obj *p;
    Obj *q;
    s32 x;
    s32 y;

    p = Func_020024f2(Data_02000240[125]);
    q = Func_020024fa(a0);
    Func_020024e8();
    {
        x = ((p->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        Func_020024b2(p, x, p->f0c, y);
    }
    Func_02002492(p, 27);
    {
        x = ((q->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((q->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        q->f30 = 0x10000;
        q->f34 = 0x8000;
        Func_020024de(q, x, q->f0c, y);
    }
    if (a1 < 0 || a2 < 0) {
        Func_020024ca(q, 4);
    } else {
        Func_020024d4(q, 3);
    }
    Func_0200250a(p);
    Func_0200257e();
}

void Actor_PlaceAtTileAndMark(s32 id, s32 x, s32 y)
{
    struct Rec_39a *rec = Func_02003362(id);

    if (rec != 0) {
        Func_020033b8(id, 3);
        rec->f34 = 2;
        rec->f35 |= 2;
        rec->f8 = (x << 20) + 0x80000;
        rec->f16 = (y << 20) + 0x80000;
    }
}
#undef Actor_PlacePairAtOffset
#undef Actor_PlaceAtTileAndMark

/* overlays/scene/actor/transition/actor_presentation.c */
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define ActorDraw_SetupActorEightForFlag301 Func_0200025c
#define ActorDraw_SetupActorNineForFlag302 Func_02000314
#define ActorDraw_SetupActorTenForFlag303 Func_0200034c
#define ActorDraw_SetupActorNineForFlag306 Func_020004bc
#define ActorDraw_SetupActorTenForFlag307 Func_02000528
#define ActorDraw_SetupActorEightForFlags308And309Guarded Func_02000668
#define ActorDraw_SetupActorEightForFlags308And309 Func_020006c0
#define ActorDraw_SetupActorNineForFlag30a Func_020006fc
#define ActorDraw_SetupActorTenForFlags30bAnd30d Func_020009d0
#define ActorDraw_SetupActorElevenAt0_112 Func_02000c78
#define ActorDraw_SetupActorElevenAt0_64 Func_02000ca8
#define ActorDraw_SetupActorElevenAt0_80 Func_02000cd8
#define ActorDraw_SetupActorEightForFlag313 Func_02000d38
#define ActorDraw_SetupActorNineForFlag314 Func_02000dd8
#define ActorDraw_SetupActorTenForFlag315 Func_02000e0c
#define ActorDraw_AdvanceActorEightStates Func_02000e78
#define ActorDraw_AdvanceActorTenStates Func_02000ea8
#define Actor_SetActor11Values1And2 Func_02000ec0

void Func_02002672(s32);
void Func_020003ec(s32, s32, s32);
s32 Func_020003f6(s32, s32, s32);
void Func_0200268c(s32);
void Func_020025a2(s32);
void Func_020024e8_a(s32);
void Func_020012dc(void);
void Func_0200272a(s32);
s32 Func_020004a6(s32, s32, s32);
void Func_0200273c(s32);
void Func_02002652(s32);
void Func_02002598(s32);
void Func_0200138c(void);
void Func_02002762(s32);
s32 Func_020004dc(s32, s32, s32);
void Func_02002772(s32);
void Func_02002690(s32);
void Func_020025ce(s32);
void Func_020013c2(void);
void Func_020028d2(s32);
s32 Func_0200064c(s32, s32, s32);
void Func_020028e2(s32);
void Func_020027f8(s32);
void Func_0200273e(s32);
void Func_02001636(void);
void Func_0200293e(s32);
void Func_020006b8(s32, s32, s32);
void Func_020006c2(s32, s32, s32);
void Func_02002958(s32);
void Func_02002876(s32);
void Func_020027b4(s32);
void Func_020016ac(void);
s32 Func_02002988(s32);
s32 Func_02002992(s32);
void Func_02002a94(s32);
void Func_020029ac(s32);
void Func_020029ba(s32);
void Func_0200081e(s32, s32, s32);
void Func_02002ab4(s32);
void Func_0200290a(s32);
void Func_0200197a(void);
void Func_0200085e(s32, s32, s32);
void Func_020019ba();
void Func_0200294a(s32);
void Func_020029f6(s32);
void Func_020029fc(s32);
void Func_02002ad6(s32);
void Func_02002af4(s32);
void Func_02002b12(s32);
s32 Func_0200088e(s32, s32, s32);
void Func_02002b24(s32);
void Func_02002a3a(s32);
void Func_02002980(s32);
void Func_020019f0(void);
void Func_02002de6(s32);
s32 Func_02000b62(s32, s32, s32);
void Func_02002df8(s32);
void Func_02002d0e(s32);
void Func_02002d1c(s32);
void Func_02002c5a(s32);
void Func_02001cca(void);
void Func_0200308e(s32);
s32 Func_02000e08(s32, s32, s32);
void Func_0200309e(s32);
void Func_0200185a(void);
void Func_02002ef8(s32);
void Func_02001f68(void);
void Func_020030be(s32);
s32 Func_02000e38(s32, s32, s32);
void Func_020030ce(s32);
void Func_0200188a(void);
void Func_02002f28(s32);
void Func_02001f98(void);
void Func_020030ee(s32);
s32 Func_02000e68(s32, s32, s32);
void Func_020030fe(s32);
void Func_020018ba(void);
void Func_02002f58(s32);
void Func_02001fc8(void);
void Func_0200314e(s32);
s32 Func_02000ec8(s32, s32, s32);
void Func_0200315e(s32);
void Func_02003074(s32);
void Func_02002fba(s32);
void Func_0200233a(void);
void Func_020031ee(s32);
s32 Func_02000f68(s32, s32, s32);
void Func_020031fe(s32);
void Func_0200311e(s32);
void Func_0200305c(s32);
void Func_020023dc(void);
void Func_02003222(s32);
void Func_02000f9c(s32, s32, s32);
void Func_02003232(s32);
void Func_02003148(s32);
void Func_0200308e_a(s32);
void Func_0200240e(void);
void Func_020031f8(s32, s32);
void Func_02003200(s32, s32);
void Func_02003228(s32, s32);
void Func_02003230(s32, s32);
void Func_02003240(s32, s32);
void Func_02003248(s32, s32);

void ActorDraw_SetupActorEightForFlag301(void)
{
    Func_02002672(0xF1);
    Func_020003ec(8, 0x70, 0);
    Func_020003f6(8, 0x70, 0);
    Func_0200268c(0x121);
    Func_020025a2(0x301);
    Func_020024e8_a(2);
    Func_020012dc();
}

void ActorDraw_SetupActorNineForFlag302(void)
{
    Func_0200272a(0xF1);
    Func_020004a6(9, 0, -64);
    Func_0200273c(0x121);
    Func_02002652(0x302);
    Func_02002598(2);
    Func_0200138c();
}

void ActorDraw_SetupActorTenForFlag303(void)
{
    Func_02002762(0xF1);
    Func_020004dc(0xA, 0, 0x40);
    Func_02002772(0x121);
    Func_02002690(0x303);
    Func_020025ce(2);
    Func_020013c2();
}

void ActorDraw_SetupActorNineForFlag306(void)
{
    Func_020028d2(0xF1);
    Func_0200064c(9, 0, 0x40);
    Func_020028e2(0x121);
    Func_020027f8(0x306);
    Func_0200273e(2);
    Func_02001636();
}

void ActorDraw_SetupActorTenForFlag307(void)
{
    Func_0200293e(0xF1);
    Func_020006b8(10, 0, 144);
    Func_020006c2(10, 0, 128);
    Func_02002958(0x121);
    Func_02002876(0x307);
    Func_020027b4(2);
    Func_020016ac();
}

void ActorDraw_SetupActorEightForFlags308And309Guarded(void)
{
    /* movs r0,#0xc4 / lsls r0,#2 builds 0x310. */
    if (Func_02002988((s32) 0x310) != 0) {
        return;
    }
    if (Func_02002992((s32) 0x30D) != 0) {
        return;
    }

    Func_02002a94((s32) 0xF1);
    /* movs r0,#0xc2 / lsls r0,#2 builds 0x308. */
    Func_020029ac((s32) 0x308);
    Func_020029ba((s32) 0x309);
    /* movs r1,#0x30 / negs r1,r1 */
    Func_0200081e(8, -48, 0);
    Func_02002ab4((s32) 0x121);
    Func_0200290a(2);
    Func_0200197a();
}

void ActorDraw_SetupActorEightForFlags308And309(void)
{
    Func_02002ad6(0xF1);
    Func_020029f6(0x308);
    Func_020029fc(0x309);
    Func_0200085e(8, 0x60, 0);
    Func_02002af4(0x121);
    Func_0200294a(2);
    Func_020019ba();
}

void ActorDraw_SetupActorNineForFlag30a(void)
{
    Func_02002b12(0xF1);
    Func_0200088e(9, -32, 0);
    Func_02002b24(0x121);
    Func_02002a3a(0x30A);
    Func_02002980(2);
    Func_020019f0();
}

void ActorDraw_SetupActorTenForFlags30bAnd30d(void)
{
    Func_02002de6(0xF1);
    Func_02000b62(0xA, 0, -64);
    Func_02002df8(0x121);
    Func_02002d0e(0x30B);
    Func_02002d1c(0x30D);
    Func_02002c5a(2);
    Func_02001cca();
}

void ActorDraw_SetupActorElevenAt0_112(void)
{
    Func_0200308e(241);
    Func_02000e08(11, 0, 112);
    Func_0200309e(0x121);
    Func_0200185a();
    Func_02002ef8(2);
    Func_02001f68();
}

void ActorDraw_SetupActorElevenAt0_64(void)
{
    Func_020030be(0xF1);
    Func_02000e38(0xB, 0, 0x40);
    Func_020030ce(0x121);
    Func_0200188a();
    Func_02002f28(2);
    Func_02001f98();
}

void ActorDraw_SetupActorElevenAt0_80(void)
{
    Func_020030ee(0xF1);
    Func_02000e68(0xB, 0, 0x50);
    Func_020030fe(0x121);
    Func_020018ba();
    Func_02002f58(2);
    Func_02001fc8();
}

void ActorDraw_SetupActorEightForFlag313(void)
{
    Func_0200314e(0xF1);
    Func_02000ec8(8, 0, 0x70);
    Func_0200315e(0x121);
    Func_02003074(0x313);
    Func_02002fba(2);
    Func_0200233a();
}

void ActorDraw_SetupActorNineForFlag314(void)
{
    Func_020031ee(0xF1);
    Func_02000f68(9, 0x80, 0);
    Func_020031fe(0x121);
    Func_0200311e(0x314);
    Func_0200305c(2);
    Func_020023dc();
}

void ActorDraw_SetupActorTenForFlag315(void)
{
    Func_02003222(241);
    Func_02000f9c(10, 160, 0);
    Func_02003232(0x121);
    Func_02003148(0x315);
    Func_0200308e_a(2);
    Func_0200240e();
}

void ActorDraw_AdvanceActorEightStates(void)
{
    Func_020031f8(8, 1);
    Func_02003200(8, 2);
}

void ActorDraw_AdvanceActorTenStates(void)
{
    Func_02003228(10, 1);
    Func_02003230(10, 2);
}

void Actor_SetActor11Values1And2(void)
{
    Func_02003240(11, 1);
    Func_02003248(11, 2);
}
#undef NULL
#undef FIELD_AT_OFFSET
#undef ActorDraw_SetupActorEightForFlag301
#undef ActorDraw_SetupActorNineForFlag302
#undef ActorDraw_SetupActorTenForFlag303
#undef ActorDraw_SetupActorNineForFlag306
#undef ActorDraw_SetupActorTenForFlag307
#undef ActorDraw_SetupActorEightForFlags308And309Guarded
#undef ActorDraw_SetupActorEightForFlags308And309
#undef ActorDraw_SetupActorNineForFlag30a
#undef ActorDraw_SetupActorTenForFlags30bAnd30d
#undef ActorDraw_SetupActorElevenAt0_112
#undef ActorDraw_SetupActorElevenAt0_64
#undef ActorDraw_SetupActorElevenAt0_80
#undef ActorDraw_SetupActorEightForFlag313
#undef ActorDraw_SetupActorNineForFlag314
#undef ActorDraw_SetupActorTenForFlag315
#undef ActorDraw_AdvanceActorEightStates
#undef ActorDraw_AdvanceActorTenStates
#undef Actor_SetActor11Values1And2

/* overlays/scene/actor/transition/conditional_scene_setup.c */
#define Scene_PlaceActorEightByFlags Func_020005b8
#define Scene_PlaceActorTenByFlags Func_02000768
void Func_020029ce(s32);
s32 Func_020028de(s32);
s32 Func_020028e8(s32);
void Func_02000760(s32, s32, s32);
void Func_02002910(s32);
void Func_0200290e(s32);
void Func_0200077c(s32, s32, s32);
void Func_02002924(s32);
void Func_02002932(s32);
void Func_02002a20(s32);
void Func_02002876_a(s32);
void Func_020018e6(void);
void Func_02002b7e(s32);
s32 Func_02002a8e(s32);
void Func_02000906(s32, s32, s32);
void Func_02002ab4_b(s32);
void Func_02002ab4_a(s32);
void Func_02002ac2(s32);
void Func_02002ac8(s32);
void Func_0200092e(s32, s32, s32);
void Func_02002ad4(s32);
void Func_02002ae4(s32);
void Func_02002aea(s32);
void Func_02002af0(s32);
void Func_02002bde(s32);
void Func_02002a34(s32);
void Func_02001aa4(void);

void Scene_PlaceActorEightByFlags(void)
{
    Func_020029ce((s32) 0xF1);

    /* movs r0,#0xc4 / lsls r0,#2 builds 0x310. The second test is only reached
     * when the first fails, and both truths take the same path. */
    if (Func_020028de((s32) 0x310) != 0 || Func_020028e8((s32) 0x30D) != 0) {
        Func_02000760(8, -48, 0);
        Func_02002910((s32) 0x308);
        Func_0200290e((s32) 0x309);
    } else {
        Func_0200077c(8, -96, 0);
        Func_02002924((s32) 0x308);
        Func_02002932((s32) 0x309);
    }

    Func_02002a20((s32) 0x121);
    Func_02002876_a(2);
    Func_020018e6();
}

void Scene_PlaceActorTenByFlags(void)
{
    Func_02002b7e(241);
    if (Func_02002a8e(0x308) != 0) {
        Func_02000906(10, 0, -64);
        Func_02002ab4_b(0x30b);
        Func_02002ab4_a(0x30c);
        Func_02002ac2(0x30d);
        Func_02002ac8(0x30e);
    } else {
        Func_0200092e(10, 0, -128);
        Func_02002ad4(0x30b);
        Func_02002ae4(0x30c);
        Func_02002aea(0x30d);
        Func_02002af0(0x30e);
    }
    Func_02002bde(0x121);
    Func_02002a34(2);
    Func_02001aa4();
}
#undef Scene_PlaceActorEightByFlags
#undef Scene_PlaceActorTenByFlags

/* overlays/scene/actor/transition/dialogue_layout.c */
#define DialogueLayout_ConfigureGroupOne Func_02001050
#define DialogueLayout_ConfigureGroupTwo Func_02001154
#define DialogueLayout_ConfigureGroupThree Func_020012cc
#define Scene_RunFlagBranchedLayoutSteps Func_020015dc
extern u16 Data_02000240[];

void Func_02003356();
s32 Func_02003384();
void Func_0200207e();
void Func_0200337c();
void Func_0200209c();
void Func_0200339c();
s32 Func_020033ca();
void Func_020020c4();
void Func_020033c4();
void Func_020020e4();
void Func_020033e4();
s32 Func_02003412();
void Func_0200210c();
void Func_0200340c();
void Func_0200212c();
void Func_0200342c();
void Func_0200345a();
s32 Func_0200348a();
void Func_02002184();
void Func_02003484();
void Func_020021a4();
void Func_020034a4();
s32 Func_020034d2();
void Func_020021cc();
void Func_020034cc();
void Func_020034de();
s32 Func_0200350e();
void Func_02002208();
void Func_02003504();
void Func_02003514();
s32 Func_02003542();
void Func_0200223c();
void Func_0200353c();
void Func_0200225c();
void Func_0200355c();
s32 Func_0200358a();
void Func_02002284();
void Func_02003584();
void Func_020022a4();
void Func_020035a4();
void Func_020022fc();
void Func_02002326();
void Func_0200236a();
void Func_02002392();
void Func_020023b2();
void Func_020023da();
void Func_0200240e_a();
void Func_02002450();
void Func_02002494();
void Func_020024d8();
void Func_020024ec();
void Func_02002520();
void Func_02002552();
void Func_02002584();
void Func_020025b6();
void Func_020035d2();
void Func_020035fc();
s32 Func_02003602();
void Func_02003626();
s32 Func_0200362c();
void Func_02003638();
void Func_0200364a();
void Func_0200366a();
void Func_02003692();
s32 Func_02003698();
void Func_020036b2();
void Func_020036da();
s32 Func_020036e0();
void Func_0200370e();
s32 Func_02003714();
void Func_02003720();
void Func_02003750();
s32 Func_02003756();
void Func_02003762();
void Func_02003794();
s32 Func_0200379a();
void Func_020037a6();
void Func_020037b8();
void Func_020037ec();
s32 Func_020037f2();
void Func_02003820();
s32 Func_02003826();
void Func_02003852();
s32 Func_02003858();
void Func_02003884();
s32 Func_0200388a();
void Func_02003896();
void Func_020038f8();
void Func_0200390e();
s32 Func_0200393c();
void Func_02002636();
void Func_02003936();
void Func_02002656();
void Func_02003956();
s32 Func_02003986();
void Func_02002680();
void Func_02003980();
void Func_020026a0();
void Func_020039a0();
s32 Func_020039ce();
void Func_020026c8();
void Func_020039c8();
void Func_020026e8();
void Func_020039e8();

/* Imports; the three queried ones are typed for their return value. */

/* Dialogue layout for resource_39a. */

/*
 * 0x02000240 is below the link base, so it is a resident table; entry 225 is
 * read here as an unsigned halfword.
 */

/* Imports; the queried ones are typed for their return value. */

void DialogueLayout_ConfigureGroupOne(void)
{
    { s32 f1 = 8; s32 g1 = 29; Func_02003356(8, 42, 15, 5,  f1, g1); }

    if (Func_02003384((s32)0x301) != 0) {
        Func_0200207e(8, 22, 31);
        { s32 f2 = 8; s32 g2 = 30; Func_0200337c(9, 30, 1, 3,  f2, g2); }
    } else {
        Func_0200209c(8, 8, 31);
        { s32 f3 = 22; s32 g3 = 30; Func_0200339c(9, 30, 1, 3,  f3, g3); }
    }

    if (Func_020033ca((s32)0x302) != 0) {
        Func_020020c4(9, 12, 29);
        { s32 f4 = 11; s32 g4 = 33; Func_020033c4(14, 33, 3, 1,  f4, g4); }
    } else {
        Func_020020e4(9, 12, 33);
        { s32 f5 = 11; s32 g5 = 29; Func_020033e4(14, 29, 3, 1,  f5, g5); }
    }

    if (Func_02003412((s32)0x303) != 0) {
        Func_0200210c(10, 18, 29);
        { s32 f6 = 17; s32 g6 = 33; Func_0200340c(14, 33, 3, 1,  f6, g6); }
    } else {
        Func_0200212c(10, 18, 33);
        { s32 f7 = 17; s32 g7 = 29; Func_0200342c(14, 29, 3, 1,  f7, g7); }
    }
}

void DialogueLayout_ConfigureGroupTwo(void)
{
    { s32 f1 = 12; s32 g1 = 8; Func_0200345a(0, 28, 10, 18,  f1, g1); }

    if (Func_0200348a((s32)0x304) != 0) {
        Func_02002184(8, 21, 20);
        { s32 f2 = 13; s32 g2 = 19; Func_02003484(20, 19, 1, 3,  f2, g2); }
    } else {
        Func_020021a4(8, 13, 20);
        { s32 f3 = 21; s32 g3 = 19; Func_020034a4(20, 19, 1, 3,  f3, g3); }
    }

    if (Func_020034d2((s32)0x305) != 0) {
        Func_020021cc(8, 12, 20);
        { s32 f4 = 12; s32 g4 = 19; Func_020034cc(5, 19, 1, 3,  f4, g4); }
        { s32 f5 = 13; s32 g5 = 19; Func_020034de(20, 19, 1, 3,  f5, g5); }
        if (Func_0200350e((s32)0x304) != 0) {
            Func_02002208(8, 21, 20);
            { s32 f6 = 13; s32 g6 = 19; Func_02003504(20, 19, 1, 3,  f6, g6); }
            { s32 f7 = 12; s32 g7 = 19; Func_02003514(20, 19, 1, 3,  f7, g7); }
        }
    }

    if (Func_02003542((s32)0x306) != 0) {
        Func_0200223c(9, 15, 21);
        { s32 f8 = 14; s32 g8 = 17; Func_0200353c(14, 18, 3, 1,  f8, g8); }
    } else {
        Func_0200225c(9, 15, 17);
        { s32 f9 = 14; s32 g9 = 21; Func_0200355c(14, 18, 3, 1,  f9, g9); }
    }

    if (Func_0200358a((s32)0x307) != 0) {
        Func_02002284(10, 19, 8);
        { s32 f10 = 18; s32 g10 = 25; Func_02003584(14, 18, 3, 1,  f10, g10); }
    } else {
        Func_020022a4(10, 19, 25);
        { s32 f11 = 18; s32 g11 = 8; Func_020035a4(14, 18, 3, 1,  f11, g11); }
    }
}

void DialogueLayout_ConfigureGroupThree(void)
{
    { s32 k5 = 12, k6 = 21; Func_020035d2(12, 3, 9, 16, k5, k6); }

    if (Func_02003602((s32)0x308) != 0) {
        Func_020022fc(8, 14, 25);
        { s32 k5 = 20, k6 = 24; Func_020035fc(16, 24, 1, 3, k5, k6); }
    } else if (Func_0200362c((s32)0x309) != 0) {
        Func_02002326(8, 17, 25);
        { s32 k6 = 24;
          Func_02003626(18, 24, 1, 3, 20, k6);
          Func_02003638(18, 24, 1, 3, 14, k6);
          Func_0200364a(8, 41, 1, 3, 17, k6);
        }
    } else {
        Func_0200236a(8, 20, 25);
        { s32 k5 = 14, k6 = 24; Func_0200366a(16, 24, 1, 3, k5, k6); }
    }

    if (Func_02003698((s32)0x30a) != 0) {
        Func_02002392(9, 13, 35);
        { s32 k5 = 15, k6 = 34; Func_02003692(14, 34, 1, 3, k5, k6); }
    } else {
        Func_020023b2(9, 15, 35);
        { s32 k5 = 13, k6 = 34; Func_020036b2(14, 34, 1, 3, k5, k6); }
    }

    if (Func_020036e0((s32)0x30b) != 0) {
        Func_020023da(10, 15, 22);
        { s32 k5 = 14;
          Func_020036da(14, 29, 3, 1, k5, 30);
          Func_020037b8(5, 41, 3, 1, k5, 22);
        }
    } else if (Func_02003714((s32)0x30c) != 0) {
        Func_0200240e_a(10, 15, 23);
        { s32 k5 = 14;
          Func_0200370e(5, 42, 3, 1, k5, 23);
          Func_02003720(14, 29, 3, 1, k5, 30);
          Func_020037b8(10, 44, 3, 1, k5, 21);
        }
    } else if (Func_02003756((s32)0x30d) != 0) {
        Func_02002450(10, 15, 26);
        { s32 k5 = 14;
          Func_02003750(14, 29, 3, 1, k5, 22);
          Func_02003762(5, 43, 3, 1, k5, 26);
          Func_020037b8(14, 29, 3, 1, k5, 30);
        }
    } else if (Func_0200379a((s32)0x30e) != 0) {
        Func_02002494(10, 15, 27);
        { s32 k5 = 14;
          Func_02003794(14, 29, 3, 1, k5, 22);
          Func_020037a6(14, 29, 3, 1, k5, 30);
          Func_020037b8(5, 44, 3, 1, k5, 27);
        }
    } else {
        Func_020024d8(10, 15, 30);
    }

    if (Func_020037f2((s32)0x30f) != 0) {
        Func_020024ec(11, 15, 23);
        { s32 k5 = 14;
          Func_020037ec(14, 29, 3, 1, k5, 31);
          Func_02003896(10, 40, 3, 1, k5, 23);
        }
    } else if (Func_02003826((s32)0x310) != 0) {
        Func_02002520(11, 15, 24);
        { s32 k5 = 14;
          Func_02003820(14, 29, 3, 1, k5, 31);
          Func_02003896(10, 41, 3, 1, k5, 24);
        }
    } else if (Func_02003858((s32)0x311) != 0) {
        Func_02002552(11, 15, 27);
        { s32 k5 = 14;
          Func_02003852(14, 29, 3, 1, k5, 31);
          Func_02003896(10, 42, 3, 1, k5, 27);
        }
    } else if (Func_0200388a((s32)0x312) != 0) {
        Func_02002584(11, 15, 28);
        { s32 k5 = 14;
          Func_02003884(14, 29, 3, 1, k5, 31);
          Func_02003896(10, 43, 3, 1, k5, 28);
        }
    } else {
        Func_020025b6(11, 15, 31);
    }
}

/*
 * Four flag-branched layout steps.  Nothing is returned; the three pool words
 * after the return belong to the owner.  The eight bytes of frame are the
 * fifth and sixth arguments of the six-argument layout calls.  Imports are
 * named by the address their call site computes, and are old-style because
 * arity varies between sites.
 */
void Scene_RunFlagBranchedLayoutSteps(void)
{
    /*
     * The byte offset 450 is built by shifting, giving entry 225.  The test
     * is (entry - 1) << 16 against 0x10000 with an unsigned compare, which
     * selects exactly entries 1 and 2.
     */
    if ((u32)((u32)(Data_02000240[225] - 1) << 16) <= (u32)0x10000) {
        { s32 f1 = 14; s32 g1 = 10; Func_020038f8(22, 20, 9, 8,  f1, g1); }
    } else {
        { s32 f2 = 7; s32 g2 = 45; Func_0200390e(20, 45, 11, 4,  f2, g2); }
    }

    if (Func_0200393c((s32)0x313) != 0) {
        Func_02002636(8, 20, 17);
        { s32 f3 = 19; s32 g3 = 10; Func_02003936(19, 11, 3, 1,  f3, g3); }
    } else {
        Func_02002656(8, 20, 10);
        { s32 f4 = 19; s32 g4 = 17; Func_02003956(19, 11, 3, 1,  f4, g4); }
    }

    /* 0x314 is built by shifting. */
    if (Func_02003986((s32)0x314) != 0) {
        Func_02002680(9, 14, 16);
        { s32 f5 = 22; s32 g5 = 15; Func_02003980(16, 15, 1, 3,  f5, g5); }
    } else {
        Func_020026a0(9, 22, 16);
        { s32 f6 = 14; s32 g6 = 15; Func_020039a0(16, 15, 1, 3,  f6, g6); }
    }

    if (Func_020039ce((s32)0x315) != 0) {
        Func_020026c8(10, 17, 46);
        { s32 f7 = 7; s32 g7 = 45; Func_020039c8(15, 15, 1, 3,  f7, g7); }
    } else {
        Func_020026e8(10, 7, 46);
        { s32 f8 = 17; s32 g8 = 45; Func_020039e8(15, 15, 1, 3,  f8, g8); }
    }
}
#undef DialogueLayout_ConfigureGroupOne
#undef DialogueLayout_ConfigureGroupTwo
#undef DialogueLayout_ConfigureGroupThree
#undef Scene_RunFlagBranchedLayoutSteps

/* overlays/scene/actor/transition/field_scene_head.c */
#define Scene_RunActor9Transition302 Func_020002e0
#define Scene_RunActor10Transition303 Func_02000380
#define Scene_RunActor8Transition304 Func_020003b8
#define Scene_RunActor8Transition304And305 Func_020003ec
#define Scene_RunActor8Transition305 Func_0200042c
#define Scene_RunActor8FlaggedSequence Func_02000464
#define Scene_RunActor9Flag306Sequence Func_020004f0
#define Scene_RunActor8Transition308And309 Func_0200062c
#define Scene_RunActor9Transition30A Func_02000734
#define Scene_RunFlag308DialogueBranch Func_020007f0
#define Scene_RunActor10Transition30BTo30E Func_020008cc
#define Scene_RunFlag311DialogueBranch Func_02000920
#define Scene_RunActor10Flags30bTo30eSequenceA Func_02000a10
#define Scene_RunActor10Flags30bTo30eSequenceB Func_02000a60
#define Scene_RunActor10Flags30bTo30eSequenceC Func_02000ab4
void Func_020026f6(s32);
s32 Func_02000470(s32, s32, s32);
void Func_02002706(s32);
void Func_02002624(s32);
void Func_02002562(s32);
void Func_02001356(void);
void Func_02002796(s32);
s32 Func_02000512(s32, s32, s32);
void Func_020027a8(s32);
void Func_020026be(s32);
void Func_02002604(s32);
void Func_020013f8(void);
void Func_020027ce(s32);
s32 Func_02000548(s32, s32, s32);
void Func_020027de(s32);
void Func_020026f6_a(s32);
void Func_0200263c(s32);
void Func_02001534(void);
void Func_02002802(s32);
s32 Func_0200057e(s32, s32, s32);
void Func_02002814(s32);
void Func_02002734(s32);
void Func_02002732(s32);
void Func_02002678(s32);
void Func_02001570(void);
void Func_02002842(s32);
s32 Func_020005be(s32, s32, s32);
void Func_02002854(s32);
void Func_0200276a(s32);
void Func_020026b0(s32);
void Func_020015a8(void);
void Func_0200287a(s32);
s32 Func_02002788(s32);
void Func_020005fe(s32, s32, s32);
void Func_020027ac(s32);
void Func_02000610(s32, s32, s32);
void Func_020027b8(s32);
void Func_020028ae(s32);
void Func_02002704(s32);
void Func_020015fc(void);
void Func_02002906(s32);
s32 Func_02000682(s32, s32, s32);
void Func_02002918(s32);
void Func_02002836(s32);
void Func_02002774(s32);
void Func_0200166c(void);
void Func_02002a42(s32);
void Func_02002962(s32);
void Func_02002968(s32);
s32 Func_020007ca(s32, s32, s32);
void Func_02002a60(s32);
void Func_020028b6(s32);
void Func_02001926(void);
void Func_02002b4a(s32);
s32 Func_020008c4(s32, s32, s32);
void Func_02002b5a(s32);
void Func_02002a78(s32);
void Func_020029b6(s32);
void Func_02001a26(void);
void Func_0200098c();
void Func_020009b8();
void Func_020009e2();
void Func_02000a08();
void Func_02001b7e();
void Func_02002b0e();
s32 Func_02002b16();
void Func_02002b3a();
void Func_02002b3a_a();
s32 Func_02002b42();
void Func_02002b48();
void Func_02002b66();
void Func_02002b66_a();
s32 Func_02002b6c();
void Func_02002b74();
void Func_02002b90();
void Func_02002b96();
void Func_02002b98();
void Func_02002ba4();
void Func_02002bb6();
void Func_02002bbe();
void Func_02002bc4();
void Func_02002bca();
void Func_02002c06();
void Func_02002cb8();
void Func_02002ce2(s32);
void Func_02000a5e(s32, s32, s32);
void Func_02002cf4(s32);
void Func_02002c0a(s32);
void Func_02002c1a(s32);
void Func_02002c20(s32);
void Func_02002c26(s32);
void Func_02002b64(s32);
void Func_02001bd4(void);
void Func_02000aba();
void Func_02000ae6();
void Func_02000b0c();
void Func_02001c82();
void Func_02002c12();
s32 Func_02002c44();
void Func_02002c68();
void Func_02002c6e();
s32 Func_02002c70();
s32 Func_02002c70_a();
void Func_02002c94();
void Func_02002c94_a();
void Func_02002ca2();
void Func_02002ca8();
void Func_02002cba();
void Func_02002cc2();
void Func_02002cc8();
void Func_02002cce();
void Func_02002d36();
void Func_02002dbc();
void Func_02002e36(s32);
void Func_02000ba0(s32, s32, s32);
void Func_02002d68(s32);
void Func_02002ca6(s32);
void Func_02001d16(void);
void Func_02002e76(s32);
void Func_02000bf2(s32, s32, s32);
void Func_02002e88(s32);
void Func_02002d9e(s32);
void Func_02002dae(s32);
void Func_02002db4(s32);
void Func_02002dba(s32);
void Func_02002cf8(s32);
void Func_02001d68(void);
void Func_02002eca(s32);
s32 Func_02000c44(s32, s32, s32);
void Func_02002eda(s32);
void Func_02002df8_a(s32);
void Func_02002e00(s32);
void Func_02002e06(s32);
void Func_02002e0c(s32);
void Func_02002d4a(s32);
void Func_02001dba(void);

/* Imports; the queried ones are typed for their return value. */

void Scene_RunActor9Transition302(void)
{
    Func_020026f6(0xF1);
    Func_02000470(9, 0, 0x40);
    Func_02002706(0x121);
    Func_02002624(0x302);
    Func_02002562(2);
    Func_02001356();
}

void Scene_RunActor10Transition303(void)
{
    Func_02002796(0xF1);
    Func_02000512(0xA, 0, -64);
    Func_020027a8(0x121);
    Func_020026be(0x303);
    Func_02002604(2);
    Func_020013f8();
}

void Scene_RunActor8Transition304(void)
{
    Func_020027ce(0xF1);
    Func_02000548(8, 0x90, 0);
    Func_020027de(0x121);
    Func_020026f6_a(0x304);
    Func_0200263c(2);
    Func_02001534();
}

void Scene_RunActor8Transition304And305(void)
{
    Func_02002802(0xF1);
    Func_0200057e(8, -144, 0);
    Func_02002814(0x121);
    Func_02002734(0x304);
    Func_02002732(0x305);
    Func_02002678(2);
    Func_02001570();
}

void Scene_RunActor8Transition305(void)
{
    Func_02002842(0xF1);
    Func_020005be(8, -14, 0);
    Func_02002854(0x121);
    Func_0200276a(0x305);
    Func_020026b0(2);
    Func_020015a8();
}

void Scene_RunActor8FlaggedSequence(void)
{
    Func_0200287a((s32) 0xF1);

    if (Func_02002788((s32) 0x306) != 0) {
        Func_020005fe(8, 16, 0);
        Func_020027ac((s32) 0x305);
    } else {
        Func_02000610(8, 144, 0);
        /* movs r0,#0xc1 / lsls r0,#2 builds 0x304. */
        Func_020027b8((s32) 0x304);
    }

    Func_020028ae((s32) 0x121);
    Func_02002704(2);
    Func_020015fc();
}

void Scene_RunActor9Flag306Sequence(void)
{
    Func_02002906(0xF1);
    Func_02000682(9, 0, -64);
    Func_02002918(0x121);
    Func_02002836(0x306);
    Func_02002774(2);
    Func_0200166c();
}

void Scene_RunActor8Transition308And309(void)
{
    Func_02002a42(0xF1);
    Func_02002962(0x308);
    Func_02002968(0x309);
    Func_020007ca(8, 0x30, 0);
    Func_02002a60(0x121);
    Func_020028b6(2);
    Func_02001926();
}

void Scene_RunActor9Transition30A(void)
{
    Func_02002b4a(0xF1);
    Func_020008c4(9, 0x20, 0);
    Func_02002b5a(0x121);
    Func_02002a78(0x30A);
    Func_020029b6(2);
    Func_02001a26();
}

/*
 * Scene step for resource_39a.  Nothing is returned; the five pool words
 * after the return belong to the owner.  The tail call shared by the first
 * three arms is written out in each arm rather than adding control flow the
 * reference does not have.  Imports are named by the address their call site
 * computes, and are old-style because arity varies between sites.
 */
void Scene_RunFlag308DialogueBranch(void)
{
    Func_02002c06((s32)0xf1);
    /* 0x308 is built by shifting. */
    if (Func_02002b16((s32)0x308) != 0) {
        Func_0200098c(10, 0, 16);
        Func_02002b3a((s32)0x30b);
        Func_02002b3a_a((s32)0x30c);
        Func_02002b48((s32)0x30d);
        Func_02002ba4((s32)0x30e);
        /* 0x310 is built by shifting. */
    } else if (Func_02002b42((s32)0x310) != 0) {
        Func_020009b8(10, 0, 16);
        Func_02002b66((s32)0x30b);
        Func_02002b66_a((s32)0x30c);
        Func_02002b74((s32)0x30d);
        Func_02002ba4((s32)0x30e);
    } else if (Func_02002b6c((s32)0x311) != 0) {
        Func_020009e2(10, 0, 64);
        Func_02002b90((s32)0x30b);
        Func_02002b98((s32)0x30c);
        Func_02002b96((s32)0x30d);
        Func_02002ba4((s32)0x30e);
    } else {
        Func_02000a08(10, 0, 128);
        Func_02002bb6((s32)0x30b);
        Func_02002bbe((s32)0x30c);
        Func_02002bc4((s32)0x30d);
        Func_02002bca((s32)0x30e);
    }
    Func_02002cb8((s32)0x121);
    Func_02002b0e(2);
    Func_02001b7e();
}

void Scene_RunActor10Transition30BTo30E(void)
{
    Func_02002ce2(241);
    Func_02000a5e(10, 0, -16);
    Func_02002cf4(0x121);
    Func_02002c0a(0x30b);
    Func_02002c1a(0x30c);
    Func_02002c20(0x30d);
    Func_02002c26(0x30e);
    Func_02002b64(2);
    Func_02001bd4();
}

/*
 * Scene step for resource_39a.  Nothing is returned; the five pool words
 * after the return belong to the owner.  Func_02002c70 serves both a query
 * and a setter, so its result is dropped at the setter site.  The tail call
 * shared by the first two arms is written out in each arm rather than adding
 * a flag the reference does not have.  Imports are named by the address their
 * call site computes, and are old-style because arity varies between sites.
 */
void Scene_RunFlag311DialogueBranch(void)
{
    Func_02002d36((s32)0xf1);
    if (Func_02002c44((s32)0x311) != 0) {
        Func_02000aba(10, 0, 48);
        Func_02002c68((s32)0x30b);
        /* 0x30c is built by shifting; the result is unused. */
        Func_02002c70((s32)0x30c);
        Func_02002c6e((s32)0x30d);
        Func_02002ca8((s32)0x30e);
        /* 0x310 is built by shifting. */
    } else if (Func_02002c70_a((s32)0x310) != 0) {
        Func_02000ae6(10, 0, 32);
        Func_02002c94((s32)0x30b);
        Func_02002c94_a((s32)0x30c);
        Func_02002ca2((s32)0x30d);
        Func_02002ca8((s32)0x30e);
    } else {
        Func_02000b0c(10, 0, 112);
        Func_02002cba((s32)0x30b);
        Func_02002cc2((s32)0x30c);
        Func_02002cc8((s32)0x30d);
        Func_02002cce((s32)0x30e);
    }
    Func_02002dbc((s32)0x121);
    Func_02002c12(2);
    Func_02001c82();
}

void Scene_RunActor10Flags30bTo30eSequenceA(void)
{
    Func_02002e26(241);
    Func_02000ba0(10, 0, 64);
    Func_02002e36(0x121);
    Func_02002d54(0x30b);
    Func_02002d5c(0x30c);
    Func_02002d62(0x30d);
    Func_02002d68(0x30e);
    Func_02002ca6(2);
    Func_02001d16();
}

void Scene_RunActor10Flags30bTo30eSequenceB(void)
{
    Func_02002e76(241);
    Func_02000bf2(10, 0, -80);
    Func_02002e88(0x121);
    Func_02002d9e(0x30b);
    Func_02002dae(0x30c);
    Func_02002db4(0x30d);
    Func_02002dba(0x30e);
    Func_02002cf8(2);
    Func_02001d68();
}

void Scene_RunActor10Flags30bTo30eSequenceC(void)
{
    Func_02002eca(0xF1);
    Func_02000c44(0xA, 0, 0x40);
    Func_02002eda(0x121);
    Func_02002df8_a(0x30B);
    Func_02002e00(0x30C);
    Func_02002e06(0x30D);
    Func_02002e0c(0x30E);
    Func_02002d4a(2);
    Func_02001dba();
}
#undef Scene_RunActor9Transition302
#undef Scene_RunActor10Transition303
#undef Scene_RunActor8Transition304
#undef Scene_RunActor8Transition304And305
#undef Scene_RunActor8Transition305
#undef Scene_RunActor8FlaggedSequence
#undef Scene_RunActor9Flag306Sequence
#undef Scene_RunActor8Transition308And309
#undef Scene_RunActor9Transition30A
#undef Scene_RunFlag308DialogueBranch
#undef Scene_RunActor10Transition30BTo30E
#undef Scene_RunFlag311DialogueBranch
#undef Scene_RunActor10Flags30bTo30eSequenceA
#undef Scene_RunActor10Flags30bTo30eSequenceB
#undef Scene_RunActor10Flags30bTo30eSequenceC

/* overlays/scene/actor/transition/overlay_object.c */
#define OvObj_CreateAndInitialize Func_02000ed8
#define OvObj_ApplyValue15 Func_02001c08
/*
 * resource_39a owner at 0x02000ed8, 88 bytes.
 *
 * Creates an actor and, when creation succeeded, initialises it: mode field of
 * the linked record to 1, state byte at +85 cleared, two setup calls, and bit 1
 * of the flag byte at +35 raised. Returns the actor, or null.
 *
 * The mode write is a bitfield assignment; as explicit mask arithmetic the
 * compiler narrows -13 to a byte and the `movs r3,#13 / negs r3,r3` pair is
 * lost.
 */
struct Mode_39a {
    u8 pad00[9];
    u8 lo : 2;
    u8 mode : 2;                /* +9, bits 2..3 */
    u8 hi : 4;
};

struct Actor_39a {
    u8 pad00[35];
    u8 f35;                     /* +35 */
    u8 pad24[44];
    struct Mode_39a *f80;       /* +80 */
    u8 pad51[1];
    u8 f85;                     /* +85 */
};

struct Actor_39a *Func_020031a2();
void Func_02003206();
void Func_020032a6();
void Func_02003f9e(s32, s32);

struct Actor_39a *OvObj_CreateAndInitialize(s32 a, s32 b, s32 c, s32 d)
{
    struct Actor_39a *actor = Func_020031a2(d, a, b, c);

    if (actor != 0) {
        actor->f80->mode = 1;
        actor->f85 = 0;
        Func_02003206(actor, 0);
        Func_020032a6(actor, 15);
        actor->f35 |= 2;
        return actor;
    }
    return 0;
}

s32 OvObj_ApplyValue15(s32 obj)
{
    Func_02003f9e(obj, 15);
    return 0;
}
#undef OvObj_CreateAndInitialize
#undef OvObj_ApplyValue15

/* overlays/scene/actor/transition/run_actor_eleven_flagged_steps.c */
/* Imports; the queried ones are typed for their return value. */
void Func_02000cac();
void Func_02000cda();
void Func_02000d02();
void Func_02001e78();
s32 Func_02002e2a();
s32 Func_02002e34();
void Func_02002e5a();
void Func_02002e60();
s32 Func_02002e62();
s32 Func_02002e62_a();
void Func_02002e88_b();
void Func_02002e88_a();
void Func_02002e96();
void Func_02002e9c();
void Func_02002ea8();
void Func_02002eb8();
void Func_02002ebe();
void Func_02002ec4();
void Func_02002e08();
void Func_02002f1a();
void Func_02002fb2();

/*
 * Field scene step for overlay resource_39a.  Imports are named by the address
 * their call site computes, not by a location in this image, and their
 * interfaces are left open.  Func_02002e62 is reached both as a setter and as
 * a query, so its result is dropped at the setter site.  The first two arms
 * share one tail call, which is why Func_02002e9c is spelled out in each arm.
 */
void Scene_RunActorElevenFlaggedSteps(void)
{
    Func_02002f1a((s32)0xf1);
    /* 0x308 is built by shifting a small immediate, not loaded whole. */
    if (Func_02002e2a((s32)0x308) != 0 || Func_02002e34((s32)0x30d) != 0) {
        Func_02000cac(11, 0, -64);
        Func_02002e5a((s32)0x30f);
        Func_02002e62((s32)0x310);
        Func_02002e60((s32)0x311);
        Func_02002e9c((s32)0x312);
        /* 0x30c is built by shifting a small immediate, not loaded whole. */
    } else if (Func_02002e62_a((s32)0x30c) != 0) {
        Func_02000cda(11, 0, -112);
        Func_02002e88_b((s32)0x30f);
        Func_02002e88_a((s32)0x310);
        Func_02002e96((s32)0x311);
        Func_02002e9c((s32)0x312);
    } else {
        Func_02000d02(11, 0, -128);
        Func_02002ea8((s32)0x30f);
        Func_02002eb8((s32)0x310);
        Func_02002ebe((s32)0x311);
        Func_02002ec4((s32)0x312);
    }
    Func_02002fb2((s32)0x121);
    Func_02002e08(2);
    Func_02001e78();
}

/* overlays/scene/actor/transition/run_scene_four_pass_callback_sequence.c */
/* Imports, named by the address each call site computes. */
void Func_02004224();
void Func_02004224_a();
void Func_0200422a();
void Func_02004156();
void Func_0200421a();
void Func_020041e4();
void Func_020041fa();
void Func_020040a0();
void Func_0200412e();
void Func_0200412e_a();
void Func_0200413e();
void Func_0200421e();
void Func_02004234();
void Func_020040dc();
void Func_020040e6();
void Func_020040fa();
void Func_020040f8();
void Func_02004250();
void Func_02004266();
void Func_0200410c();
void Func_02004122();
void Func_02004118();
void Func_02004272();
void Func_02004288();
void Func_020041f6();
void Func_0200420c();
void Func_020042e0();

extern void Func_02009d78();
extern

/*
 * Field scene sequence for overlay resource_39a.  Imports are named by the
 * address their call site computes, not by a location in this image, and
 * their interfaces are left open.  The declarations are old-style because
 * Func_02004224 and Func_0200412e are each reached with two different
 * argument counts.  Func_02009d78 names a loader-relocated call word handed
 * to two imports as a callback, not a runtime address.
 */
void Scene_RunFourPassCallbackSequence(void)
{
    s32 pass;
    s32 step;
    s32 span;
    s32 one;

    Func_02004224(19);
    Func_0200422a(182);
    Func_02004156();
    Func_0200421a();

    /* 8, 7 and 1 are locals held across the loop, not literals: the first
     * call takes 8 as an immediate for argument 4 and from a register for
     * argument 5, which a literal cannot produce. */
    pass = 0;
    step = 8;
    span = 7;
    one = 1;
    do {
        Func_020041e4((s32)0x204318, 1);
        Func_020041fa(1);
        Func_020040a0(2);
        if (pass == 0) {
            Func_0200412e(30, 8, 12, 8, step, span);
            Func_0200413e(30, 57, 19, 57, one, one);
        }
        Func_0200421e((s32)0x203108, 1);
        Func_02004234(1);
        Func_020040dc(2);
        /* The increment belongs to the loop test, not the body: `pass++;` as
         * a statement would not place it after the last call.  The compare is
         * unsigned against 3, so the body runs for pass 0 to 3. */
    } while ((unsigned int)++pass <= 3);

    Func_020040e6(30);
    /* 0xc80 is built by shifting a small immediate, not loaded whole. */
    Func_020040fa((void *)Func_02009d78, (s32)0xc80);
    Func_020040f8(40);
    Func_02004250((s32)0x201090, 1);
    Func_02004266(40);
    Func_0200410c(80);
    Func_02004122((void *)Func_02009d78);
    Func_02004118(20);
    /* 0x10000 is built by shifting a small immediate, not loaded whole. */
    Func_02004272((s32)0x10000, 1);
    Func_02004288(80);
    /* Same import as in the loop, one argument here. */
    Func_0200412e_a(80);
    /* 0x820 is built by shifting a small immediate, not loaded whole. */
    Func_020041f6((s32)0x820);
    Func_0200420c(230);
    Func_020042e0();
    /* Same import as the first call, no argument register written here. */
    Func_02004224_a();
}

/* overlays/scene/actor/transition/scene_data.c */
#define SceneData_SelectTableBySceneId Func_02000050
#define SceneData_ReturnZero Func_020000ec
#define SceneData_GetTableA8f4 Func_020000f0
#define SceneData_SelectDataByRuntimeSelector Func_020000f8
#define SceneData_SelectDataByRuntimeSelectorB Func_02000f30
extern s16 Data_02000240[];
extern u8 Value_00000034;
extern u8 Value_0000003e;
extern u8 Value_0000003f;
extern u8 Value_00000040;
extern u8 Value_00000041;
extern u8 Value_00000042;
extern u8 Value_00000043;
extern u8 Data_0200a4bc[];
extern u8 Data_0200a504[];
extern u8 Data_0200a5f4[];
extern u8 Data_0200a63c[];
extern u8 Data_0200a6cc[];
extern u8 Data_0200a744[];
extern u8 Data_0200a7bc[];
extern u8 Data_0200a48c[];
extern u8 Data_0200a8f4;
extern u8 Data_0200a9bc[];
extern u8 Data_0200a9ec[];
extern u8 Data_0200aa4c[];
extern u8 Data_0200aac4[];
extern u8 Data_0200ab3c[];
extern u8 Data_0200ab9c[];
extern u8 Data_0200a9a4[];
extern u8 Data_0200abd8[];
extern u8 Data_0200ac08[];
extern u8 Data_0200ad1c[];
extern u8 Data_0200ae24[];
extern u8 Data_0200b058[];
extern u8 Data_0200b130[];
extern u8 Data_0200b184[];
extern u8 Data_0200abcc[];

s32 SceneData_SelectTableBySceneId(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000034) {
        return (s32)Data_0200a4bc;
    }
    if (v == (s32)&Value_0000003e) {
        return (s32)Data_0200a504;
    }
    if (v == (s32)&Value_0000003f) {
        return (s32)Data_0200a5f4;
    }
    if (v == (s32)&Value_00000040) {
        return (s32)Data_0200a63c;
    }
    if (v == (s32)&Value_00000041) {
        return (s32)Data_0200a6cc;
    }
    if (v == (s32)&Value_00000042) {
        return (s32)Data_0200a744;
    }
    if (v == (s32)&Value_00000043) {
        return (s32)Data_0200a7bc;
    }
    return (s32)Data_0200a48c;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

void *SceneData_GetTableA8f4(void)
{
    return &Data_0200a8f4;
}

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000034) {
        return (s32)Data_0200a9bc;
    }
    if (v == (s32)&Value_0000003e) {
        return (s32)Data_0200a9ec;
    }
    if (v == (s32)&Value_0000003f) {
        return (s32)Data_0200aa4c;
    }
    if (v == (s32)&Value_00000040) {
        return (s32)Data_0200aac4;
    }
    if (v == (s32)&Value_00000041) {
        return (s32)Data_0200ab3c;
    }
    if (v == (s32)&Value_00000043) {
        return (s32)Data_0200ab9c;
    }
    return (s32)Data_0200a9a4;
}

s32 SceneData_SelectDataByRuntimeSelectorB(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000034) {
        return (s32)Data_0200abd8;
    }
    if (v == (s32)&Value_0000003e) {
        return (s32)Data_0200ac08;
    }
    if (v == (s32)&Value_0000003f) {
        return (s32)Data_0200ad1c;
    }
    if (v == (s32)&Value_00000040) {
        return (s32)Data_0200ae24;
    }
    if (v == (s32)&Value_00000041) {
        return (s32)Data_0200b058;
    }
    if (v == (s32)&Value_00000042) {
        return (s32)Data_0200b130;
    }
    if (v == (s32)&Value_00000043) {
        return (s32)Data_0200b184;
    }
    return (s32)Data_0200abcc;
}
#undef SceneData_SelectTableBySceneId
#undef SceneData_ReturnZero
#undef SceneData_GetTableA8f4
#undef SceneData_SelectDataByRuntimeSelector
#undef SceneData_SelectDataByRuntimeSelectorB

/* overlays/scene/actor/transition/scene_dialogue.c */
#define Dialogue_ShowLineF13WithWorkA488 Func_02001b9c
#define Dialogue_RunLine17e2 Func_02001f20
extern s32 Data_0200a488;

void Func_02003ed8(void);
void Func_02003f8e(s32, s32);
s32 Func_02003efe(s32, s32);
void Func_02003edc(s32);
void Func_02003e86(s32);
void Func_02003f0a(void);
void Func_0200425c(void);
void Func_0200422c(s32, s32);
void Func_02004270(void);

/* Returns a value: the reference sets r1 before r0 at this site. */

void Dialogue_ShowLineF13WithWorkA488(void)
{
    Func_02003ed8();

    /* r5 holds &Data_0200a488 across the calls; the word is reloaded before
     * the second test. */
    if (Data_0200a488 != 0) {
        Func_02003f8e(Data_0200a488, 3);
    }

    Func_02003efe((s32) 0xE6, 0);
    Func_02003edc((s32) 0xF13);

    if (Data_0200a488 != 0) {
        Func_02003e86(Data_0200a488);
    }

    Func_02003f0a();
}

void Dialogue_RunLine17e2(void)
{
    Func_0200425c();
    Func_0200422c(0x17E2, 1);
    Func_02004270();
}
#undef Dialogue_ShowLineF13WithWorkA488
#undef Dialogue_RunLine17e2

/* overlays/scene/actor/transition/shared.c */
#define Scene_RunSingleStep Func_02000ff8
void Func_02003090(void);

void Scene_RunSingleStep(void)
{
    Func_02003090();
}
#undef Scene_RunSingleStep

/* overlays/scene/actor/transition/staged_actor_navigation.c */
#define NULL ((void *)0)
#define Actor_StepSubjectAlongHeading Func_02002094

struct SharedData_02000240 {
    u8 pad_000[500];
    s32 selected_subject;
};

struct Subject_02002094 {
    u8 pad_000[6];
    u16 heading;
    s32 x;
    s32 y;
    s32 z;
    u8 pad_020[14];
    u8 id;
    u8 pad_035[13];
    s32 state_048;
    s32 state_052;
    u8 pad_056[34];
    u8 flags_090;
    u8 pad_091[9];
    u16 state_100;
    u8 pad_102[6];
    void *callback;
};

extern s16 Data_0200a464[];
extern struct SharedData_02000240 Data_02000240;
extern u32 Data_03001ae8;

void Func_0200a014();
struct Subject_02002094 *Func_02004498();
void Func_0200440a();
s32 Func_020043ec();
void Func_02004392();
s32 Func_02004406();
s32 Func_02004410();
void Func_02004426();
void Func_02004406_a();
void Func_02004416();
void Func_02004444();
s32 Func_0200446e();
void Func_02004480();
void Func_0200448e();
void Func_02004450();
s32 Func_020044c4_b();
void Func_020044c2();
void Func_020044d0();
void Func_02004466();
void Func_02004484();
void Func_02004568();

/*
 * In-image heading table at 0x0200a464.  0x02000240 is below the link base,
 * so it is a resident table; its word at byte offset 500 selects the subject.
 */

/* Installed callback, named by the linked address of its call word. */

/*
 * Imports named by the address their call site computes, not by a runtime
 * address.  Declarations are old-style because arity varies between sites.
 */

static __inline__ void AdvanceProbe_02002094(s32 heading, s32 *probe)
{
    /*
     * Keep this call behind an inline boundary: it is what makes sp+8 be
     * rematerialized for argument 2 before the split 0x100000 constant is
     * completed.
     */
    Func_02004450((s32)0x100000, heading, probe);
}

/*
 * Pathing step for resource_39a.  r0 holds the popped return address, so
 * nothing is returned, and the seven pool words after the return belong to
 * the owner.  Frame: sp+0 is the goal marker, sp+4 the heading, and
 * sp+8..sp+19 the three-word probe position handed to the stepping imports by
 * address.  The x and z assignment order and the inline stepping wrapper are
 * what reproduce the reference; do not reorder or respell them.
 */
void Actor_StepSubjectAlongHeading(void)
{
    struct Subject_02002094 *subject;
    s32 probe[3];
    s32 heading;
    s32 goal;
    s32 marker;
    s32 z;
    s32 x;
    u8 *subject_id;

    subject = Func_02004498(Data_02000240.selected_subject);

    for (;;) {
        heading = Data_0200a464[(Data_03001ae8 >> 4) & 15];
        /*
         * The test is on heading << 16 against 0xffff0000, the signed
         * halfword -1 meaning "no heading".
         */
        if ((heading << 16) == (s32)0xffff0000) {
            return;
        }
        /* No argument register is written before this branch. */
        Func_0200440a();

        /* The 0x80000 bias is built by shifting, not loaded as a constant. */
        probe[0] = (subject->x & (s32)0xfff00000) + 0x80000;
        probe[1] = subject->y;
        probe[2] = (subject->z & (s32)0xfff00000) + 0x80000;
        z = probe[2];
        x = probe[0];
        subject_id = (u8 *)subject;
        subject_id += 34;
        goal = Func_020043ec((s32)*subject_id, x, z);
        /*
         * 0x100000 is built by shifting, not loaded as a constant.  The probe
         * block is passed by address and is advanced by the callee.
         */
        Func_02004392((s32)0x100000, heading, probe);

        marker = Func_02004406((s32)*subject_id, probe[0], probe[2]);
        if (marker == 255
                || Func_02004410((s32)*subject_id, probe[0], probe[2])
                    - subject->y > 0x80000) {
            subject->heading = (u16)heading;
            goto tail;
        }

        /* Rewind the probe to the position it held before 0x02004392. */
        probe[0] = x;
        probe[2] = z;
        subject->state_048 = 0x20000;
        subject->state_052 = 0x1999;
        subject->state_100 = 0;
        Func_02004426(subject, x, subject->y, z);
        /*
         * Same call word as the marker lookup, but a two-argument command, so
         * it keeps its own declaration.
         */
        Func_02004406_a(subject, 2);
        Func_02004416(subject, 48);
        Func_02004444(subject);
        subject->callback = (void *)Func_0200a014;

        goto advance_probe;
continue_probe:
        if (Func_0200446e((s32)*subject_id, probe[0], probe[2])
                - subject->y > 0x80000) {
            goto finish_probe;
        }
        x = probe[0];
        z = probe[2];
        subject->state_048 = 0x20000;
        subject->state_052 = 0x1999;
        Func_02004480(subject, probe[0], probe[1], probe[2]);
        Func_0200448e(subject);
        if (marker != goal) {
            goto blocked;
        }

advance_probe:
        AdvanceProbe_02002094(heading, probe);
        marker = Func_020044c4_b((s32)*subject_id, probe[0], probe[2]);
        if (marker != 255) {
            goto continue_probe;
        }

finish_probe:
        subject->state_048 = 0x20000;
        subject->state_052 = 0x10000;
        Func_020044c2(subject, x, subject->y, z);
        Func_020044d0(subject);
        Func_02004466(2);
        /* The back edge re-reads the heading table and starts again. */
    }

blocked:
    subject->callback = NULL;
    subject->flags_090 |= 1;
    /* 0x4000 is built by shifting, not loaded as a constant. */
    subject->state_052 = 0x4000;

tail:
    Func_02004484(10);
    Func_02004568();
}
#undef NULL
#undef Actor_StepSubjectAlongHeading

/* overlays/scene/actor/transition/state_update.c */
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define State_ApplyValues8And2And1 Func_02000030
#define State_ApplyValues11And62 Func_02000040
#define State_SetServiceZeroValue06 Func_02000fcc
#define State_StoreValueToWorkspaceWord24WhenFlagged Func_0200170c
#define State_ClearWorkWord24 Func_02001730
#define Scene_ApplyOffset0Neg32 Func_02001ab0
#define State_ApplyOffsetMinus32 Func_02001ac0
#define State_SetValue17e1 Func_02001f04
#define State_SetWorkspace370ByFlag820 Func_02001f58

/* Prepare service zero, set its +6 halfword, and run the follow-up sequence. */
struct SceneService_02000fcc {
    u16 unknown00[3];
    u16 value06;
};

extern u8 *Data_03001ee0;
extern u8 *Data_0200b328;
extern u8 *Data_03001ebc;

void Func_02002442(s32, s32, s32);
void Func_020023f0(s32, s32);
void Func_02003308(void);
struct SceneService_02000fcc *Func_02003326(s32 index);
void Func_020033f2(s32 value);
void Func_020033b6(void);
void Func_020033c2(void);
void Func_02003390(s32 value);
s32 Func_02003a6e(s32);
void Func_0200358a_a(s32, s32);
void Func_0200359a(s32, s32);
void Func_02004240(void);
void Func_02004210(s32, s32);
void Func_02004254(void);
void Func_02004294();
s32 Func_0200427c();
void Func_02004270_a();
void Func_0200427a();
s32 Func_02004290();
void Func_020042da();

/* 0x03001ebc is the overlay's workspace pointer. */

/*
 * Imports. Each alias names the call word its site encodes, not a runtime
 * address. Only those used for their return value are typed, and the
 * declarations are old-style because one name is reached with different
 * argument counts.
 */

void State_ApplyValues8And2And1(void)
{
    Func_02002442(8, 2, 1);
}

void State_ApplyValues11And62(void)
{
    Func_020023f0(0xB, 0x3E);
}

void State_SetServiceZeroValue06(void)
{
    struct SceneService_02000fcc *work;

    Func_02003308();
    work = Func_02003326(0);
    work->value06 = 0x4000;
    Func_020033f2(123);
    Func_020033b6();
    Func_020033c2();
    Func_02003390(1);
}

void State_StoreValueToWorkspaceWord24WhenFlagged(void)
{
    s32 *flag = (s32 *)0x0200B328;

    if (*flag != 0) {
        u8 *state = Data_03001ee0;

        *(s32 *)(state + 24) = Func_02003a6e(0);
    }
}

void State_ClearWorkWord24(void)
{
    if (Data_0200b328 != 0) {
        *(s32 *)(Data_03001ee0 + 24) = 0;
    }
}

void Scene_ApplyOffset0Neg32(void)
{
    Func_0200358a_a(0, -32);
}

void State_ApplyOffsetMinus32(void)
{
    Func_0200359a(-32, 0);
}

void State_SetValue17e1(void)
{
    Func_02004240();
    Func_02004210(0x17E1, 1);
    Func_02004254();
}

/*
 * Branch on flag 0x820 -- resource_39a. One arm sets a record flag; the other
 * sets a different flag and writes workspace halfword 370. Nothing is
 * returned, and the owner extends through the three pool words that follow
 * the epilogue.
 */
void State_SetWorkspace370ByFlag820(void)
{
    Func_02004294();
    /* movs r0,#0x82 / lsls r0,#4 builds 0x820. */
    if (Func_0200427c((s32)0x820) != 0) {
        Func_02004270_a((s32)0x17e5, 1);
    } else {
        Func_0200427a((s32)0x17e4, 1);
        if (Func_02004290((s32)0xe6) != -1) {
            u8 *workspace = Data_03001ebc;

            /* movs r1,#0xb9 / lsls r1,#1 gives the byte offset 370. */
            /*
             * The store goes through a pointer local and an s32 value local,
             * in that order. Storing the literal directly builds the constant
             * in HImode and loads it from the literal pool, costing a pool
             * word; splitting the address out first also fixes which register
             * holds it.
             */
            {
                u16 *slot = (u16 *)(workspace + 370);
                s32 one = 1;

                *slot = (u16)one;
            }
        }
    }
    Func_020042da();
}
#undef NULL
#undef FIELD_AT_OFFSET
#undef State_ApplyValues8And2And1
#undef State_ApplyValues11And62
#undef State_SetServiceZeroValue06
#undef State_StoreValueToWorkspaceWord24WhenFlagged
#undef State_ClearWorkWord24
#undef Scene_ApplyOffset0Neg32
#undef State_ApplyOffsetMinus32
#undef State_SetValue17e1
#undef State_SetWorkspace370ByFlag820
