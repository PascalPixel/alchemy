#include "types.h"
#include "scene.h"

/* overlays/scene/actor/transition/transition.c */
/* overlays/scene/actor/transition/transition.c */
/* overlays/scene/actor/transition/actor_depth_effect.c */

/* Three sites of one import, so three names. */

void Scene_RunActorTenDepthSequence(void)
{
    /* r5 = -96 (movs #0x60 / negs), live across the first import call. */
    s32 depth = -96;

    Actor_Do((s32) 0xF1);
    ActorDraw_SetupActorNineForFlag30a(10, 0, depth);
    Actor_Place(10, 0, depth);
    Actor_unk2_3(10, 0, -80);
    Actor_unk2_2((s32) 0x121);
    Actor_unk3_2((s32) 0x307);
    Actor_unk4_2(2);
    Actor_Run();
}

/* overlays/scene/actor/transition/actor_facing.c */
typedef struct {
    u8 filler0[6];
    u16 unk6;
    u8 filler8[82];
    u8 unk5A;
    u8 filler5B[9];
    u16 unk64;
} T;

extern s16 gOv[];

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
    d = gOv[(*(u32 *)0x03001ae8 >> 4) & 0xF];
    z = -z;
    if (d == z) {
        Actor_Apply(o, 9);
        return;
    }
    prev = o->unk6;
    d = (s16)(d - prev);
    if (d > 0x1000)
        d = 0x1000;
    if (d < -0x1000)
        d = -0x1000;
    o->unk6 = prev + d;
    Actor_Apply2(o, 2);
    Actor_Apply3(o, 0x30);
}

/* overlays/scene/actor/transition/actor_motion.c */
/* Advance both coordinate triplets by their corresponding deltas. */
struct MovingObject {
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

void State_RunSlot8OffsetStep(void)
{
    s32 offset = 112;

    offset = -offset;
    Actor_unk5_2(0xF1);
    Actor_unk3_3(8, offset, 0);
    Actor_unk4_3(8, offset, 0);
    Actor_unk6_2(0x121);
    Actor_unk7_2(0x301);
    Actor_unk8_2(2);
    Actor_unk2_4();
}

void OvObj_AdvancePositionByDelta(struct MovingObject *object)
{
    object->x += object->dx;
    object->y += object->dy;
    object->z += object->dz;
    object->sub_x += object->sub_dx;
    object->sub_y += object->sub_dy;
}

/* overlays/scene/actor/transition/actor_position.c */
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

extern s32 gCell[];

Obj *Actor_unk3_4(s32);
Obj *Actor_unk4_4(s32);

struct Rec_39a *Actor_unk5_4();

void Actor_PlacePairAtOffset(s32 a0, s32 a1, s32 a2)
{
    Obj *p;
    Obj *q;
    s32 x;
    s32 y;

    p = Actor_unk3_4(gCell[125]);
    q = Actor_unk4_4(a0);
    Actor_unk6_4();
    {
        x = ((p->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        Actor_SetMode(p, x, p->f0c, y);
    }
    Actor_Apply4(p, 27);
    {
        x = ((q->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((q->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        q->f30 = 0x10000;
        q->f34 = 0x8000;
        Actor_SetMode2(q, x, q->f0c, y);
    }
    if (a1 < 0 || a2 < 0) {
        Actor_Apply5(q, 4);
    } else {
        Actor_Apply6(q, 3);
    }
    Actor_unk9_2(p);
    Actor_unk7_4();
}

void Actor_PlaceAtTileAndMark(s32 id, s32 x, s32 y)
{
    struct Rec_39a *rec = Actor_unk5_4(id);

    if (rec != 0) {
        Actor_unk8_4(id, 3);
        rec->f34 = 2;
        rec->f35 |= 2;
        rec->f8 = (x << 20) + 0x80000;
        rec->f16 = (y << 20) + 0x80000;
    }
}

/* overlays/scene/actor/transition/actor_presentation.c */

void ActorDraw_SetupActorEightForFlag301(void)
{
    Actor_unk10_2(0xF1);
    Scene_RunActor8Transition304And305(8, 0x70, 0);
    Actor_unk5_3(8, 0x70, 0);
    Actor_unk11_2(0x121);
    Actor_unk12_2(0x301);
    Actor_unk13_2(2);
    Actor_unk9_4();
}

void ActorDraw_SetupActorNineForFlag302(void)
{
    Actor_unk14_2(0xF1);
    Actor_unk6_3(9, 0, -64);
    Actor_unk15_2(0x121);
    Actor_unk16_2(0x302);
    Actor_unk17_2(2);
    Actor_unk10_4();
}

void ActorDraw_SetupActorTenForFlag303(void)
{
    Actor_unk18_2(0xF1);
    Actor_unk7_3(0xA, 0, 0x40);
    Actor_unk19_2(0x121);
    Actor_unk20_2(0x303);
    Actor_unk21_2(2);
    Actor_unk11_4();
}

void ActorDraw_SetupActorNineForFlag306(void)
{
    Actor_unk22_2(0xF1);
    Actor_unk8_3(9, 0, 0x40);
    Actor_unk23_2(0x121);
    Actor_unk24_2(0x306);
    Actor_unk25_2(2);
    Actor_unk12_4();
}

void ActorDraw_SetupActorTenForFlag307(void)
{
    Actor_unk26_2(0xF1);
    Actor_unk9_3(10, 0, 144);
    Actor_unk10_3(10, 0, 128);
    Actor_unk27_2(0x121);
    Actor_unk28_2(0x307);
    Actor_unk29_2(2);
    Actor_unk13_4();
}

void ActorDraw_SetupActorEightForFlags308And309Guarded(void)
{
    /* movs r0,#0xc4 / lsls r0,#2 builds 0x310. */
    if (Actor_Check((s32) 0x310) != 0) {
        return;
    }
    if (Actor_unk2((s32) 0x30D) != 0) {
        return;
    }

    Actor_unk30_2((s32) 0xF1);
    /* movs r0,#0xc2 / lsls r0,#2 builds 0x308. */
    Actor_unk31_2((s32) 0x308);
    Actor_unk32_2((s32) 0x309);
    /* movs r1,#0x30 / negs r1,r1 */
    Actor_unk11_3(8, -48, 0);
    Actor_unk33_2((s32) 0x121);
    Actor_unk34_2(2);
    Actor_unk14_4();
}

void ActorDraw_SetupActorEightForFlags308And309(void)
{
    Actor_unk35_2(0xF1);
    Actor_unk36_2(0x308);
    Actor_unk37_2(0x309);
    Actor_unk12_3(8, 0x60, 0);
    Actor_unk38_2(0x121);
    Actor_unk39_2(2);
    Actor_unk15_4();
}

void ActorDraw_SetupActorNineForFlag30a(void)
{
    Actor_unk40_2(0xF1);
    Actor_unk13_3(9, -32, 0);
    Actor_unk41_2(0x121);
    Actor_unk42_2(0x30A);
    Actor_unk43_2(2);
    Actor_unk16_4();
}

void ActorDraw_SetupActorTenForFlags30bAnd30d(void)
{
    Actor_unk44_2(0xF1);
    Actor_unk14_3(0xA, 0, -64);
    Actor_unk45_2(0x121);
    Actor_unk46_2(0x30B);
    Actor_unk47_2(0x30D);
    Actor_unk48_2(2);
    Actor_unk17_4();
}

void ActorDraw_SetupActorElevenAt0_112(void)
{
    Actor_unk49_2(241);
    Actor_unk15_3(11, 0, 112);
    Actor_unk50_2(0x121);
    Actor_unk18_4();
    Actor_unk51_2(2);
    Actor_unk19_4();
}

void ActorDraw_SetupActorElevenAt0_64(void)
{
    Actor_unk52_2(0xF1);
    Actor_unk16_3(0xB, 0, 0x40);
    Actor_unk53_2(0x121);
    Actor_unk20_4();
    Actor_unk54_2(2);
    Actor_unk21_4();
}

void ActorDraw_SetupActorElevenAt0_80(void)
{
    Actor_unk55_2(0xF1);
    Actor_unk17_3(0xB, 0, 0x50);
    Actor_unk56_2(0x121);
    Actor_unk22_4();
    Actor_unk57_2(2);
    Actor_unk23_4();
}

void ActorDraw_SetupActorEightForFlag313(void)
{
    Actor_unk58_2(0xF1);
    Actor_unk18_3(8, 0, 0x70);
    Actor_unk59_2(0x121);
    Actor_unk60_2(0x313);
    Actor_unk61_2(2);
    Actor_unk24_4();
}

void ActorDraw_SetupActorNineForFlag314(void)
{
    Actor_unk62_2(0xF1);
    Actor_unk19_3(9, 0x80, 0);
    Actor_unk63_2(0x121);
    Actor_unk64_2(0x314);
    Actor_unk65_2(2);
    Actor_unk25_4();
}

void ActorDraw_SetupActorTenForFlag315(void)
{
    Actor_unk66_2(241);
    Actor_unk20_3(10, 160, 0);
    Actor_unk67_2(0x121);
    Actor_unk68_2(0x315);
    Actor_unk69_2(2);
    Actor_unk26_4();
}

void ActorDraw_AdvanceActorEightStates(void)
{
    Actor_Apply7(8, 1);
    Actor_Apply8(8, 2);
}

void ActorDraw_AdvanceActorTenStates(void)
{
    Actor_Apply9(10, 1);
    Actor_Apply10(10, 2);
}

void Actor_SetActor11Values1And2(void)
{
    Actor_Apply11(11, 1);
    Actor_Apply12(11, 2);
}

/* overlays/scene/actor/transition/conditional_scene_setup.c */

void Scene_PlaceActorEightByFlags(void)
{
    Actor_unk70_2((s32) 0xF1);

    /* movs r0,#0xc4 / lsls r0,#2 builds 0x310. The second test is only reached
     * when the first fails, and both truths take the same path. */
    if (Actor_unk3((s32) 0x310) != 0 || Actor_unk4((s32) 0x30D) != 0) {
        Actor_unk21_3(8, -48, 0);
        Actor_unk71_2((s32) 0x308);
        Actor_unk72_2((s32) 0x309);
    } else {
        Actor_unk22_3(8, -96, 0);
        Actor_unk73_2((s32) 0x308);
        Actor_unk74_2((s32) 0x309);
    }

    Actor_unk75_2((s32) 0x121);
    Actor_unk76_2(2);
    Actor_unk27_4();
}

void Scene_PlaceActorTenByFlags(void)
{
    Actor_unk77_2(241);
    if (Actor_unk5(0x308) != 0) {
        Actor_unk23_3(10, 0, -64);
        Actor_unk78_2(0x30b);
        Actor_unk79_2(0x30c);
        Actor_unk80_2(0x30d);
        Actor_unk81_2(0x30e);
    } else {
        Actor_unk24_3(10, 0, -128);
        Actor_unk82_2(0x30b);
        Actor_unk83_2(0x30c);
        Actor_unk84_2(0x30d);
        Actor_unk85_2(0x30e);
    }
    Actor_unk86_2(0x121);
    Actor_unk87_2(2);
    Actor_unk28_4();
}

/* overlays/scene/actor/transition/dialogue_layout.c */
extern u16 gCell[];

/* Imports; the three queried ones are typed for their return value. */

/* Dialogue layout for resource_39a. */

/*
 * 0x02000240 is below the link base, so it is a resident table; entry 225 is
 * read here as an unsigned halfword.
 */

/* Imports; the queried ones are typed for their return value. */

void DialogueLayout_ConfigureGroupOne(void)
{
    { s32 f1 = 8; s32 g1 = 29; Actor_unk29_4(8, 42, 15, 5,  f1, g1); }

    if (Actor_unk6((s32)0x301) != 0) {
        Actor_unk30_4(8, 22, 31);
        { s32 f2 = 8; s32 g2 = 30; Actor_unk31_4(9, 30, 1, 3,  f2, g2); }
    } else {
        Actor_unk32_4(8, 8, 31);
        { s32 f3 = 22; s32 g3 = 30; Actor_unk33_4(9, 30, 1, 3,  f3, g3); }
    }

    if (Actor_unk7((s32)0x302) != 0) {
        Actor_unk34_4(9, 12, 29);
        { s32 f4 = 11; s32 g4 = 33; Actor_unk35_4(14, 33, 3, 1,  f4, g4); }
    } else {
        Actor_unk36_4(9, 12, 33);
        { s32 f5 = 11; s32 g5 = 29; Actor_unk37_4(14, 29, 3, 1,  f5, g5); }
    }

    if (Actor_unk8((s32)0x303) != 0) {
        Actor_unk38_4(10, 18, 29);
        { s32 f6 = 17; s32 g6 = 33; Actor_unk39_4(14, 33, 3, 1,  f6, g6); }
    } else {
        Actor_unk40_4(10, 18, 33);
        { s32 f7 = 17; s32 g7 = 29; Actor_unk41_4(14, 29, 3, 1,  f7, g7); }
    }
}

void DialogueLayout_ConfigureGroupTwo(void)
{
    { s32 f1 = 12; s32 g1 = 8; Actor_unk42_4(0, 28, 10, 18,  f1, g1); }

    if (Actor_unk9((s32)0x304) != 0) {
        Actor_unk43_4(8, 21, 20);
        { s32 f2 = 13; s32 g2 = 19; Actor_unk44_4(20, 19, 1, 3,  f2, g2); }
    } else {
        Actor_unk45_4(8, 13, 20);
        { s32 f3 = 21; s32 g3 = 19; Actor_unk46_4(20, 19, 1, 3,  f3, g3); }
    }

    if (Actor_unk10((s32)0x305) != 0) {
        Actor_unk47_4(8, 12, 20);
        { s32 f4 = 12; s32 g4 = 19; Actor_unk48_4(5, 19, 1, 3,  f4, g4); }
        { s32 f5 = 13; s32 g5 = 19; Actor_unk49_4(20, 19, 1, 3,  f5, g5); }
        if (Actor_unk11((s32)0x304) != 0) {
            Actor_unk50_4(8, 21, 20);
            { s32 f6 = 13; s32 g6 = 19; Actor_unk51_4(20, 19, 1, 3,  f6, g6); }
            { s32 f7 = 12; s32 g7 = 19; Actor_unk52_4(20, 19, 1, 3,  f7, g7); }
        }
    }

    if (Actor_unk12((s32)0x306) != 0) {
        Actor_unk53_4(9, 15, 21);
        { s32 f8 = 14; s32 g8 = 17; Actor_unk54_4(14, 18, 3, 1,  f8, g8); }
    } else {
        Actor_unk55_4(9, 15, 17);
        { s32 f9 = 14; s32 g9 = 21; Actor_unk56_4(14, 18, 3, 1,  f9, g9); }
    }

    if (Actor_unk13((s32)0x307) != 0) {
        Actor_unk57_4(10, 19, 8);
        { s32 f10 = 18; s32 g10 = 25; Actor_unk58_4(14, 18, 3, 1,  f10, g10); }
    } else {
        Actor_unk59_4(10, 19, 25);
        { s32 f11 = 18; s32 g11 = 8; Actor_unk60_4(14, 18, 3, 1,  f11, g11); }
    }
}

void DialogueLayout_ConfigureGroupThree(void)
{
    { s32 k5 = 12, k6 = 21; Actor_unk61_4(12, 3, 9, 16, k5, k6); }

    if (Actor_unk14((s32)0x308) != 0) {
        Actor_unk62_4(8, 14, 25);
        { s32 k5 = 20, k6 = 24; Actor_unk63_4(16, 24, 1, 3, k5, k6); }
    } else if (Actor_unk15((s32)0x309) != 0) {
        Actor_unk64_4(8, 17, 25);
        { s32 k6 = 24;
          Actor_unk65_4(18, 24, 1, 3, 20, k6);
          Actor_unk66_4(18, 24, 1, 3, 14, k6);
          Actor_unk67_4(8, 41, 1, 3, 17, k6);
        }
    } else {
        Actor_unk68_4(8, 20, 25);
        { s32 k5 = 14, k6 = 24; Actor_unk69_4(16, 24, 1, 3, k5, k6); }
    }

    if (Actor_unk16((s32)0x30a) != 0) {
        Actor_unk70_4(9, 13, 35);
        { s32 k5 = 15, k6 = 34; Actor_unk71_4(14, 34, 1, 3, k5, k6); }
    } else {
        Actor_unk72_4(9, 15, 35);
        { s32 k5 = 13, k6 = 34; Actor_unk73_4(14, 34, 1, 3, k5, k6); }
    }

    if (Actor_unk17((s32)0x30b) != 0) {
        Actor_unk74_4(10, 15, 22);
        { s32 k5 = 14;
          Actor_unk75_4(14, 29, 3, 1, k5, 30);
          Actor_unk76_4(5, 41, 3, 1, k5, 22);
        }
    } else if (Actor_unk18((s32)0x30c) != 0) {
        Actor_unk77_4(10, 15, 23);
        { s32 k5 = 14;
          Actor_unk78_4(5, 42, 3, 1, k5, 23);
          Actor_unk79_4(14, 29, 3, 1, k5, 30);
          Actor_unk76_4(10, 44, 3, 1, k5, 21);
        }
    } else if (Actor_unk19((s32)0x30d) != 0) {
        Actor_unk80_4(10, 15, 26);
        { s32 k5 = 14;
          Actor_unk81_4(14, 29, 3, 1, k5, 22);
          Actor_unk82_4(5, 43, 3, 1, k5, 26);
          Actor_unk76_4(14, 29, 3, 1, k5, 30);
        }
    } else if (Actor_unk20((s32)0x30e) != 0) {
        Actor_unk83_4(10, 15, 27);
        { s32 k5 = 14;
          Actor_unk84_4(14, 29, 3, 1, k5, 22);
          Actor_unk85_4(14, 29, 3, 1, k5, 30);
          Actor_unk76_4(5, 44, 3, 1, k5, 27);
        }
    } else {
        Actor_unk86_4(10, 15, 30);
    }

    if (Actor_unk21((s32)0x30f) != 0) {
        Actor_unk87_4(11, 15, 23);
        { s32 k5 = 14;
          Actor_unk88_4(14, 29, 3, 1, k5, 31);
          Actor_unk89_4(10, 40, 3, 1, k5, 23);
        }
    } else if (Actor_unk22((s32)0x310) != 0) {
        Actor_unk90_4(11, 15, 24);
        { s32 k5 = 14;
          Actor_unk91_4(14, 29, 3, 1, k5, 31);
          Actor_unk89_4(10, 41, 3, 1, k5, 24);
        }
    } else if (Actor_unk23((s32)0x311) != 0) {
        Actor_unk92_4(11, 15, 27);
        { s32 k5 = 14;
          Actor_unk93_4(14, 29, 3, 1, k5, 31);
          Actor_unk89_4(10, 42, 3, 1, k5, 27);
        }
    } else if (Actor_unk24((s32)0x312) != 0) {
        Actor_unk94_4(11, 15, 28);
        { s32 k5 = 14;
          Actor_unk95_4(14, 29, 3, 1, k5, 31);
          Actor_unk89_4(10, 43, 3, 1, k5, 28);
        }
    } else {
        Actor_unk96_4(11, 15, 31);
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
    if ((u32)((u32)(gCell[225] - 1) << 16) <= (u32)0x10000) {
        { s32 f1 = 14; s32 g1 = 10; Actor_unk97_4(22, 20, 9, 8,  f1, g1); }
    } else {
        { s32 f2 = 7; s32 g2 = 45; Actor_unk98_4(20, 45, 11, 4,  f2, g2); }
    }

    if (Actor_unk25((s32)0x313) != 0) {
        Actor_unk99_4(8, 20, 17);
        { s32 f3 = 19; s32 g3 = 10; Actor_unk100_4(19, 11, 3, 1,  f3, g3); }
    } else {
        Actor_unk101_4(8, 20, 10);
        { s32 f4 = 19; s32 g4 = 17; Actor_unk102_4(19, 11, 3, 1,  f4, g4); }
    }

    /* 0x314 is built by shifting. */
    if (Actor_unk26((s32)0x314) != 0) {
        Actor_unk103_4(9, 14, 16);
        { s32 f5 = 22; s32 g5 = 15; Actor_unk104_4(16, 15, 1, 3,  f5, g5); }
    } else {
        Actor_unk105_4(9, 22, 16);
        { s32 f6 = 14; s32 g6 = 15; Actor_unk106_4(16, 15, 1, 3,  f6, g6); }
    }

    if (Actor_unk27((s32)0x315) != 0) {
        Actor_unk107_4(10, 17, 46);
        { s32 f7 = 7; s32 g7 = 45; Actor_unk108_4(15, 15, 1, 3,  f7, g7); }
    } else {
        Actor_unk109_4(10, 7, 46);
        { s32 f8 = 17; s32 g8 = 45; Actor_unk110_4(15, 15, 1, 3,  f8, g8); }
    }
}

/* overlays/scene/actor/transition/field_scene_head.c */

/* Imports; the queried ones are typed for their return value. */

void Scene_RunActor9Transition302(void)
{
    Actor_unk88_2(0xF1);
    Actor_unk25_3(9, 0, 0x40);
    Actor_unk89_2(0x121);
    Actor_unk90_2(0x302);
    Actor_unk91_2(2);
    Actor_unk111_4();
}

void Scene_RunActor10Transition303(void)
{
    Actor_unk92_2(0xF1);
    Actor_unk26_3(0xA, 0, -64);
    Actor_unk93_2(0x121);
    Actor_unk94_2(0x303);
    Actor_unk95_2(2);
    Actor_unk112_4();
}

void Scene_RunActor8Transition304(void)
{
    Actor_unk96_2(0xF1);
    Actor_unk27_3(8, 0x90, 0);
    Actor_unk97_2(0x121);
    Actor_unk98_2(0x304);
    Actor_unk99_2(2);
    Actor_unk113_4();
}

void Scene_RunActor8Transition304And305(void)
{
    Actor_unk100_2(0xF1);
    Actor_unk28_3(8, -144, 0);
    Actor_unk101_2(0x121);
    Actor_unk102_2(0x304);
    Actor_unk103_2(0x305);
    Actor_unk104_2(2);
    Actor_unk114_4();
}

void Scene_RunActor8Transition305(void)
{
    Actor_unk105_2(0xF1);
    Actor_unk29_3(8, -14, 0);
    Actor_unk106_2(0x121);
    Actor_unk107_2(0x305);
    Actor_unk108_2(2);
    Actor_unk115_4();
}

void Scene_RunActor8FlaggedSequence(void)
{
    Actor_unk109_2((s32) 0xF1);

    if (Actor_unk28((s32) 0x306) != 0) {
        Actor_unk30_3(8, 16, 0);
        Actor_unk110_2((s32) 0x305);
    } else {
        Actor_unk31_3(8, 144, 0);
        /* movs r0,#0xc1 / lsls r0,#2 builds 0x304. */
        Actor_unk111_2((s32) 0x304);
    }

    Actor_unk112_2((s32) 0x121);
    Actor_unk113_2(2);
    Actor_unk116_4();
}

void Scene_RunActor9Flag306Sequence(void)
{
    Actor_unk114_2(0xF1);
    Actor_unk32_3(9, 0, -64);
    Actor_unk115_2(0x121);
    Actor_unk116_2(0x306);
    Actor_unk117_2(2);
    Actor_unk117_4();
}

void Scene_RunActor8Transition308And309(void)
{
    Actor_unk118_2(0xF1);
    Actor_unk119_2(0x308);
    Actor_unk120_2(0x309);
    Actor_unk33_3(8, 0x30, 0);
    Actor_unk121_2(0x121);
    Actor_unk122_2(2);
    Actor_unk118_4();
}

void Scene_RunActor9Transition30A(void)
{
    Actor_unk123_2(0xF1);
    Actor_unk34_3(9, 0x20, 0);
    Actor_unk124_2(0x121);
    Actor_unk125_2(0x30A);
    Actor_unk126_2(2);
    Actor_unk119_4();
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
    Actor_unk120_4((s32)0xf1);
    /* 0x308 is built by shifting. */
    if (Actor_unk29((s32)0x308) != 0) {
        Actor_unk121_4(10, 0, 16);
        Actor_unk122_4((s32)0x30b);
        Actor_unk123_4((s32)0x30c);
        Actor_unk124_4((s32)0x30d);
        Actor_unk125_4((s32)0x30e);
        /* 0x310 is built by shifting. */
    } else if (Actor_unk30((s32)0x310) != 0) {
        Actor_unk126_4(10, 0, 16);
        Actor_unk127_4((s32)0x30b);
        Actor_unk128_4((s32)0x30c);
        Actor_unk129_4((s32)0x30d);
        Actor_unk125_4((s32)0x30e);
    } else if (Actor_unk31((s32)0x311) != 0) {
        Actor_unk130_4(10, 0, 64);
        Actor_unk131_3((s32)0x30b);
        Actor_unk132_3((s32)0x30c);
        Actor_unk133_3((s32)0x30d);
        Actor_unk125_4((s32)0x30e);
    } else {
        Actor_unk134_3(10, 0, 128);
        Actor_unk135_3((s32)0x30b);
        Actor_unk136_3((s32)0x30c);
        Actor_unk137_3((s32)0x30d);
        Actor_unk138_3((s32)0x30e);
    }
    Actor_unk139_3((s32)0x121);
    Actor_unk140_3(2);
    Actor_unk141_3();
}

void Scene_RunActor10Transition30BTo30E(void)
{
    Actor_unk127_2(241);
    Actor_unk35_3(10, 0, -16);
    Actor_unk128_2(0x121);
    Actor_unk129_2(0x30b);
    Actor_unk130_2(0x30c);
    Actor_unk131(0x30d);
    Actor_unk132(0x30e);
    Actor_unk133(2);
    Actor_unk142_3();
}

/*
 * Scene step for resource_39a.  Nothing is returned; the five pool words
 * after the return belong to the owner.  Actor_unk32 serves both a query
 * and a setter, so its result is dropped at the setter site.  The tail call
 * shared by the first two arms is written out in each arm rather than adding
 * a flag the reference does not have.  Imports are named by the address their
 * call site computes, and are old-style because arity varies between sites.
 */
void Scene_RunFlag311DialogueBranch(void)
{
    Actor_unk143_3((s32)0xf1);
    if (Actor_unk33((s32)0x311) != 0) {
        Actor_unk144_3(10, 0, 48);
        Actor_unk145_3((s32)0x30b);
        /* 0x30c is built by shifting; the result is unused. */
        Actor_unk32((s32)0x30c);
        Actor_unk146_3((s32)0x30d);
        Actor_unk147_3((s32)0x30e);
        /* 0x310 is built by shifting. */
    } else if (Actor_unk34((s32)0x310) != 0) {
        Actor_unk148_3(10, 0, 32);
        Actor_unk149_3((s32)0x30b);
        Actor_unk150_3((s32)0x30c);
        Actor_unk151_3((s32)0x30d);
        Actor_unk147_3((s32)0x30e);
    } else {
        Actor_unk152_3(10, 0, 112);
        Actor_unk153_3((s32)0x30b);
        Actor_unk154_3((s32)0x30c);
        Actor_unk155_2((s32)0x30d);
        Actor_unk156_2((s32)0x30e);
    }
    Actor_unk157_2((s32)0x121);
    Actor_unk158_2(2);
    Actor_unk159_2();
}

void Scene_RunActor10Flags30bTo30eSequenceA(void)
{
    Actor_unk160_2(241);
    Actor_unk36_3(10, 0, 64);
    Actor_unk134(0x121);
    Actor_unk161_2(0x30b);
    Actor_unk162_2(0x30c);
    Actor_unk163_2(0x30d);
    Actor_unk135(0x30e);
    Actor_unk136(2);
    Actor_unk164_2();
}

void Scene_RunActor10Flags30bTo30eSequenceB(void)
{
    Actor_unk137(241);
    Actor_unk37_3(10, 0, -80);
    Actor_unk138(0x121);
    Actor_unk139(0x30b);
    Actor_unk140(0x30c);
    Actor_unk141(0x30d);
    Actor_unk142(0x30e);
    Actor_unk143(2);
    Actor_unk165_2();
}

void Scene_RunActor10Flags30bTo30eSequenceC(void)
{
    Actor_unk144(0xF1);
    Actor_unk38_3(0xA, 0, 0x40);
    Actor_unk145(0x121);
    Actor_unk146(0x30B);
    Actor_unk147(0x30C);
    Actor_unk148(0x30D);
    Actor_unk149(0x30E);
    Actor_unk150(2);
    Actor_unk166_2();
}

/* overlays/scene/actor/transition/overlay_object.c */
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

struct Actor_39a *Actor_unk167_2();

struct Actor_39a *OvObj_CreateAndInitialize(s32 a, s32 b, s32 c, s32 d)
{
    struct Actor_39a *actor = Actor_unk167_2(d, a, b, c);

    if (actor != 0) {
        actor->f80->mode = 1;
        actor->f85 = 0;
        Actor_unk168_2(actor, 0);
        Actor_unk169_2(actor, 15);
        actor->f35 |= 2;
        return actor;
    }
    return 0;
}

s32 OvObj_ApplyValue15(s32 obj)
{
    Actor_Apply13(obj, 15);
    return 0;
}

/* overlays/scene/actor/transition/run_actor_eleven_flagged_steps.c */
/* Imports; the queried ones are typed for their return value. */

/*
 * Field scene step for overlay resource_39a.  Imports are named by the address
 * their call site computes, not by a location in this image, and their
 * interfaces are left open.  Actor_unk35 is reached both as a setter and as
 * a query, so its result is dropped at the setter site.  The first two arms
 * share one tail call, which is why Actor_unk170_2 is spelled out in each arm.
 */
void Scene_RunActorElevenFlaggedSteps(void)
{
    Actor_unk171_2((s32)0xf1);
    /* 0x308 is built by shifting a small immediate, not loaded whole. */
    if (Actor_unk36((s32)0x308) != 0 || Actor_unk37((s32)0x30d) != 0) {
        Actor_unk172_2(11, 0, -64);
        Actor_unk173_2((s32)0x30f);
        Actor_unk35((s32)0x310);
        Actor_unk174_2((s32)0x311);
        Actor_unk170_2((s32)0x312);
        /* 0x30c is built by shifting a small immediate, not loaded whole. */
    } else if (Actor_unk38((s32)0x30c) != 0) {
        Actor_unk175_2(11, 0, -112);
        Actor_unk176_2((s32)0x30f);
        Actor_unk177_2((s32)0x310);
        Actor_unk178_2((s32)0x311);
        Actor_unk170_2((s32)0x312);
    } else {
        Actor_unk179_2(11, 0, -128);
        Actor_unk180_2((s32)0x30f);
        Actor_unk181_2((s32)0x310);
        Actor_unk182_2((s32)0x311);
        Actor_unk183_2((s32)0x312);
    }
    Actor_unk184_2((s32)0x121);
    Actor_unk185_2(2);
    Actor_unk186_2();
}

/* overlays/scene/actor/transition/run_scene_four_pass_callback_sequence.c */
/* Imports, named by the address each call site computes. */

extern

/*
 * Field scene sequence for overlay resource_39a.  Imports are named by the
 * address their call site computes, not by a location in this image, and
 * their interfaces are left open.  The declarations are old-style because
 * Actor_unk187_2 and Actor_unk188_2 are each reached with two different
 * argument counts.  Actor_unk189_2 names a loader-relocated call word handed
 * to two imports as a callback, not a runtime address.
 */
void Scene_RunFourPassCallbackSequence(void)
{
    s32 pass;
    s32 step;
    s32 span;
    s32 one;

    Actor_unk187_2(19);
    Actor_unk190_2(182);
    Actor_unk191_2();
    Actor_unk192_2();

    /* 8, 7 and 1 are locals held across the loop, not literals: the first
     * call takes 8 as an immediate for argument 4 and from a register for
     * argument 5, which a literal cannot produce. */
    pass = 0;
    step = 8;
    span = 7;
    one = 1;
    do {
        Actor_unk193_2((s32)0x204318, 1);
        Actor_unk194_2(1);
        Actor_unk195_2(2);
        if (pass == 0) {
            Actor_unk188_2(30, 8, 12, 8, step, span);
            Actor_unk196_2(30, 57, 19, 57, one, one);
        }
        Actor_unk197_2((s32)0x203108, 1);
        Actor_unk198_2(1);
        Actor_unk199_2(2);
        /* The increment belongs to the loop test, not the body: `pass++;` as
         * a statement would not place it after the last call.  The compare is
         * unsigned against 3, so the body runs for pass 0 to 3. */
    } while ((unsigned int)++pass <= 3);

    Actor_unk200_2(30);
    /* 0xc80 is built by shifting a small immediate, not loaded whole. */
    Actor_unk201_2((void *)Actor_unk189_2, (s32)0xc80);
    Actor_unk202_2(40);
    Actor_unk203_2((s32)0x201090, 1);
    Actor_unk204_2(40);
    Actor_unk205_2(80);
    Actor_unk206_2((void *)Actor_unk189_2);
    Actor_unk207_2(20);
    /* 0x10000 is built by shifting a small immediate, not loaded whole. */
    Actor_unk208_2((s32)0x10000, 1);
    Actor_unk209_2(80);
    /* Same import as in the loop, one argument here. */
    Actor_unk210_2(80);
    /* 0x820 is built by shifting a small immediate, not loaded whole. */
    Actor_unk211_2((s32)0x820);
    Actor_unk212_2(230);
    Actor_unk213_2();
    /* Same import as the first call, no argument register written here. */
    Actor_unk214_2();
}

/* overlays/scene/actor/transition/scene_data.c */
extern s16 gCell[];
extern u8 Value_00000034;
extern u8 Value_0000003e;
extern u8 Value_0000003f;
extern u8 Value_00000040;
extern u8 Value_00000041;
extern u8 Value_00000042;
extern u8 Value_00000043;
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gOv6[];
extern u8 gOv7[];
extern u8 gOv8[];
extern u8 gOv9[];
extern u8 gOv10;
extern u8 gOv11[];
extern u8 gOv12[];
extern u8 gOv13[];
extern u8 gOv14[];
extern u8 gOv15[];
extern u8 gOv16[];
extern u8 gOv17[];
extern u8 gOv18[];
extern u8 gOv19[];
extern u8 gOv20[];
extern u8 gOv21[];
extern u8 gOv22[];
extern u8 gOv23[];
extern u8 gOv24[];
extern u8 gOv25[];

s32 SceneData_SelectTableBySceneId(void)
{
    s16 v = gCell[224];

    if (v == (s32)&Value_00000034) {
        return (s32)gOv2;
    }
    if (v == (s32)&Value_0000003e) {
        return (s32)gOv3;
    }
    if (v == (s32)&Value_0000003f) {
        return (s32)gOv4;
    }
    if (v == (s32)&Value_00000040) {
        return (s32)gOv5;
    }
    if (v == (s32)&Value_00000041) {
        return (s32)gOv6;
    }
    if (v == (s32)&Value_00000042) {
        return (s32)gOv7;
    }
    if (v == (s32)&Value_00000043) {
        return (s32)gOv8;
    }
    return (s32)gOv9;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

void *SceneData_GetTableA8f4(void)
{
    return &gOv10;
}

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    s16 v = gCell[224];

    if (v == (s32)&Value_00000034) {
        return (s32)gOv11;
    }
    if (v == (s32)&Value_0000003e) {
        return (s32)gOv12;
    }
    if (v == (s32)&Value_0000003f) {
        return (s32)gOv13;
    }
    if (v == (s32)&Value_00000040) {
        return (s32)gOv14;
    }
    if (v == (s32)&Value_00000041) {
        return (s32)gOv15;
    }
    if (v == (s32)&Value_00000043) {
        return (s32)gOv16;
    }
    return (s32)gOv17;
}

s32 SceneData_SelectDataByRuntimeSelectorB(void)
{
    s16 v = gCell[224];

    if (v == (s32)&Value_00000034) {
        return (s32)gOv18;
    }
    if (v == (s32)&Value_0000003e) {
        return (s32)gOv19;
    }
    if (v == (s32)&Value_0000003f) {
        return (s32)gOv20;
    }
    if (v == (s32)&Value_00000040) {
        return (s32)gOv21;
    }
    if (v == (s32)&Value_00000041) {
        return (s32)gOv22;
    }
    if (v == (s32)&Value_00000042) {
        return (s32)gOv23;
    }
    if (v == (s32)&Value_00000043) {
        return (s32)gOv24;
    }
    return (s32)gOv25;
}

/* overlays/scene/actor/transition/scene_dialogue.c */
extern s32 gOv26;

/* Returns a value: the reference sets r1 before r0 at this site. */

void Dialogue_ShowLineF13WithWorkA488(void)
{
    Actor_unk215_2();

    /* r5 holds &gOv26 across the calls; the word is reloaded before
     * the second test. */
    if (gOv26 != 0) {
        Actor_Apply14(gOv26, 3);
    }

    Actor_Apply15((s32) 0xE6, 0);
    Actor_unk151((s32) 0xF13);

    if (gOv26 != 0) {
        Actor_unk152(gOv26);
    }

    Actor_unk216_2();
}

void Dialogue_RunLine17e2(void)
{
    Actor_unk217_2();
    Actor_Apply16(0x17E2, 1);
    Actor_unk218_2();
}

/* overlays/scene/actor/transition/shared.c */

void Scene_RunSingleStep(void)
{
    Actor_unk219_2();
}

/* overlays/scene/actor/transition/staged_actor_navigation.c */

struct SharedData {
    u8 pad_000[500];
    s32 selected_subject;
};

struct Subject {
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

extern s16 gOv27[];
extern struct SharedData gCell;
extern u32 gIw;

struct Subject *Actor_unk220_2();

/*
 * In-image heading table at 0x0200a464.  0x02000240 is below the link base,
 * so it is a resident table; its word at byte offset 500 selects the subject.
 */

/* Installed callback, named by the linked address of its call word. */

/*
 * Imports named by the address their call site computes, not by a runtime
 * address.  Declarations are old-style because arity varies between sites.
 */

static __inline__ void AdvanceProbe(s32 heading, s32 *probe)
{
    /*
     * Keep this call behind an inline boundary: it is what makes sp+8 be
     * rematerialized for argument 2 before the split 0x100000 constant is
     * completed.
     */
    Actor_unk221_2((s32)0x100000, heading, probe);
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
    struct Subject *subject;
    s32 probe[3];
    s32 heading;
    s32 goal;
    s32 marker;
    s32 z;
    s32 x;
    u8 *subject_id;

    subject = Actor_unk220_2(gCell.selected_subject);

    for (;;) {
        heading = gOv27[(gIw >> 4) & 15];
        /*
         * The test is on heading << 16 against 0xffff0000, the signed
         * halfword -1 meaning "no heading".
         */
        if ((heading << 16) == (s32)0xffff0000) {
            return;
        }
        /* No argument register is written before this branch. */
        Actor_unk222_2();

        /* The 0x80000 bias is built by shifting, not loaded as a constant. */
        probe[0] = (subject->x & (s32)0xfff00000) + 0x80000;
        probe[1] = subject->y;
        probe[2] = (subject->z & (s32)0xfff00000) + 0x80000;
        z = probe[2];
        x = probe[0];
        subject_id = (u8 *)subject;
        subject_id += 34;
        goal = Actor_unk39((s32)*subject_id, x, z);
        /*
         * 0x100000 is built by shifting, not loaded as a constant.  The probe
         * block is passed by address and is advanced by the callee.
         */
        Actor_unk223_2((s32)0x100000, heading, probe);

        marker = Actor_unk40((s32)*subject_id, probe[0], probe[2]);
        if (marker == 255
                || Actor_unk41((s32)*subject_id, probe[0], probe[2])
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
        Actor_unk224_2(subject, x, subject->y, z);
        /*
         * Same call word as the marker lookup, but a two-argument command, so
         * it keeps its own declaration.
         */
        Actor_unk225_2(subject, 2);
        Actor_unk226_2(subject, 48);
        Actor_unk227_2(subject);
        subject->callback = (void *)Actor_unk228_2;

        goto advance_probe;
continue_probe:
        if (Actor_unk42((s32)*subject_id, probe[0], probe[2])
                - subject->y > 0x80000) {
            goto finish_probe;
        }
        x = probe[0];
        z = probe[2];
        subject->state_048 = 0x20000;
        subject->state_052 = 0x1999;
        Actor_unk229_2(subject, probe[0], probe[1], probe[2]);
        Actor_unk230_2(subject);
        if (marker != goal) {
            goto blocked;
        }

advance_probe:
        AdvanceProbe(heading, probe);
        marker = Actor_unk43((s32)*subject_id, probe[0], probe[2]);
        if (marker != 255) {
            goto continue_probe;
        }

finish_probe:
        subject->state_048 = 0x20000;
        subject->state_052 = 0x10000;
        Actor_unk231_2(subject, x, subject->y, z);
        Actor_unk232_2(subject);
        Actor_unk233_2(2);
        /* The back edge re-reads the heading table and starts again. */
    }

blocked:
    subject->callback = NULL;
    subject->flags_090 |= 1;
    /* 0x4000 is built by shifting, not loaded as a constant. */
    subject->state_052 = 0x4000;

tail:
    Actor_unk234_2(10);
    Actor_unk235_2();
}

/* overlays/scene/actor/transition/state_update.c */

/* Prepare service zero, set its +6 halfword, and run the follow-up sequence. */
struct SceneService {
    u16 unknown00[3];
    u16 value06;
};

extern u8 *gIw2;
extern u8 *gOv28;
extern u8 *gWork;

struct SceneService *Actor_unk236_2(s32 index);

/* 0x03001ebc is the overlay's workspace pointer. */

/*
 * Imports. Each alias names the call word its site encodes, not a runtime
 * address. Only those used for their return value are typed, and the
 * declarations are old-style because one name is reached with different
 * argument counts.
 */

void State_ApplyValues8And2And1(void)
{
    Actor_unk39_3(8, 2, 1);
}

void State_ApplyValues11And62(void)
{
    Actor_Apply17(0xB, 0x3E);
}

void State_SetServiceZeroValue06(void)
{
    struct SceneService *work;

    Actor_unk237_2();
    work = Actor_unk236_2(0);
    work->value06 = 0x4000;
    Actor_unk153(123);
    Actor_unk238_2();
    Actor_unk239_2();
    Actor_unk154(1);
}

void State_StoreValueToWorkspaceWord24WhenFlagged(void)
{
    s32 *flag = (s32 *)0x0200B328;

    if (*flag != 0) {
        u8 *state = gIw2;

        *(s32 *)(state + 24) = Actor_unk44(0);
    }
}

void State_ClearWorkWord24(void)
{
    if (gOv28 != 0) {
        *(s32 *)(gIw2 + 24) = 0;
    }
}

void Scene_ApplyOffset0Neg32(void)
{
    Actor_Apply18(0, -32);
}

void State_ApplyOffsetMinus32(void)
{
    Actor_Apply19(-32, 0);
}

void State_SetValue17e1(void)
{
    Actor_unk240_2();
    Actor_Apply20(0x17E1, 1);
    Actor_unk241_2();
}

/*
 * Branch on flag 0x820 -- resource_39a. One arm sets a record flag; the other
 * sets a different flag and writes workspace halfword 370. Nothing is
 * returned, and the owner extends through the three pool words that follow
 * the epilogue.
 */
void State_SetWorkspace370ByFlag820(void)
{
    Actor_unk242_2();
    /* movs r0,#0x82 / lsls r0,#4 builds 0x820. */
    if (Actor_unk45((s32)0x820) != 0) {
        Actor_unk243_2((s32)0x17e5, 1);
    } else {
        Actor_unk244_2((s32)0x17e4, 1);
        if (Actor_unk46((s32)0xe6) != -1) {
            u8 *workspace = gWork;

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
    Actor_unk245_2();
}

/* overlays/scene/actor/transition/scene_run_steps30_f_to312.c */
/* overlays/scene/actor/transition/run_steps_to.c */
void Scene_RunSteps30FTo312(void)
{
    Actor_DoSteps30FTo312(0x30F);
    Actor_unk2_2Steps30FTo312(0x310);
    Actor_unk3_2Steps30FTo312(0x311);
    Actor_unk4_2Steps30FTo312(0x312);
}

/* overlays/scene/actor/transition/run_actor_11_offset_sequence.c */
/*
 * Run this scene transition's six setup services.  The literal word following
 * the return belongs to this owner and supplies 0x121 to Actor_RunSteps30FTo312.
 */

void Scene_RunActor11Offset128Sequence(void)
{
    Actor_DoSteps30FTo312(241);
    Actor_PlaceSteps30FTo312(11, 0, 128);
    Actor_unk2_4Steps30FTo312();
    Actor_RunSteps30FTo312(0x121);
    Actor_unk2_2Steps30FTo312(2);
    Actor_unk3_4Steps30FTo312();
}

/* overlays/scene/actor/transition/run_actor_11_flags_312_sequence.c */
void Scene_RunActor11Flags30fTo312Sequence(void)
{
    Actor_DoSteps30FTo312(0xF1);
    Actor_PlaceSteps30FTo312(0xB, 0, -16);
    Actor_unk2_2Steps30FTo312(0x121);
    Actor_unk3_2Steps30FTo312(0x30F);
    Actor_unk4_2Steps30FTo312(0x310);
    Actor_unk5_2Steps30FTo312(0x311);
    Actor_unk6_2Steps30FTo312(0x312);
    Actor_unk7_2Steps30FTo312(2);
    Actor_RunSteps30FTo312();
}

/* overlays/scene/actor/transition/run_actor_11_transition.c */
void Scene_RunActor11Transition(void)
{
    Actor_RunActor11Transition(241);
    Actor_PlaceActor11Transition(11, 0, 48);
    Actor_DoActor11Transition(0x121);
    Actor_unk2_4Actor11Transition();
    Actor_unk2_2Actor11Transition(2);
    Actor_unk3_4Actor11Transition();
}

/* overlays/scene/actor/transition/scene_run_actor8_transition313.c */
/* overlays/scene/actor/transition/run_actor_8_transition.c */
void Scene_RunActor8Transition313(void)
{
    Actor_DoActor8Transition313(0xF1);
    Actor_PlaceActor8Transition313(8, 0, -112);
    Actor_unk2_2Actor8Transition313(0x121);
    Actor_unk3_2Actor8Transition313(0x313);
    Actor_unk4_2Actor8Transition313(2);
    Actor_RunActor8Transition313();
}

/* overlays/scene/actor/transition/run_actor_9_flag_sequence.c */
void Scene_RunActor9Flag314Sequence(void)
{
    Actor_DoActor8Transition313(0xF1);
    Actor_PlaceActor8Transition313(9, -128, 0);
    Actor_unk2_2Actor8Transition313(0x121);
    Actor_unk3_2Actor8Transition313(0x314);
    Actor_unk4_2Actor8Transition313(2);
    Actor_RunActor8Transition313();
}

/* overlays/scene/actor/transition/run_actor_10_flag_sequence.c */
void Scene_RunActor10Flag315Sequence(void)
{
    Actor_Do10FlagSequence(241);
    Actor_Place10FlagSequence(10, -160, 0);
    Actor_unk2_210FlagSequence(0x121);
    Actor_unk3_210FlagSequence(0x315);
    Actor_unk4_210FlagSequence(2);
    Actor_Run10FlagSequence();
}

/* overlays/scene/actor/transition/scene_set_actor9_values1_and2.c */
void Scene_SetActor9Values1And2(void)
{
    Actor_ApplyActor9Values1And2(9, 1);
    Actor_Apply2Actor9Values1And2(9, 2);
}

/* overlays/scene/actor/transition/run_scene_primary_sequence.c */
extern u8 gVal[];
extern u8 gCell[];
extern u8 gWork[];
extern s16 gCell2[][1];

s32 Actor_RunPrimarySequence(void)
{
    u32 i;
    s32 record;

    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x204;
    if (Actor_Check(0x109) == 0) {
        if (gCell2[224][0] != (s32)gVal) {
            goto L_0200178c;
        }
        Actor_Do(0x144);
        Actor_unk2_4PrimarySequence();
    } else {
        L_0200178c:;
        Actor_unk3_4PrimarySequence();
    }
    return 0;
}

/* overlays/scene/actor/transition/run_scene_sequence_a.c */
extern u8 gWork[];

void Scene_RunScene39aSequenceA(void)
{
    u32 i;
    s32 record;

    Actor_RunSequence();
    record = Actor_CheckSequence(8);
    Actor_unk2_4Sequence(record, 0);
    Actor_unk3_4Sequence();
    Actor_Place(0, 0x20000, 0x1999);
    Actor_unk2_3(0, 0x108, 196);
    Actor_unk4_4Sequence();
}

/* overlays/scene/script/talk_pair_step.c */
/* overlays/scene/script/talk_pair_step.c */
extern u8 gWork[];

void Scene_RunTalk(s32 a0, s32 a1)
{
    u32 i;
    s32 record;

    Script_Run();
    Script_Place(0, 0x28000, 0x14000);
    Script_unk2_3(0, a0, a1);
    Script_unk2_4(0, 4, 0);
    Script_unk3_4(0, 7);
    Script_unk4_4(0);
    Script_unk5_3(0, 6);
    Script_unk6_3();
}

/* overlays/scene/script/talk_flag_step.c */
extern u8 gOvTalkPairStep[];

void Scene_RunTalkTalkFlagStep(void)
{
    u8 *rec;
    s32 rec7;
    s32 record;
    u8 *p6;

    record = 0;
    rec = Script_Check(22, 0xf80000, 0x80000, 0x980000);
    if ((s32)rec != 0) {
        p6 = *(volatile s32 *)((s32)rec + 80);
        p6[38] = record;
        *(u8 *)(((s32)p6 + 38) + 1) = record;
        *((s8 *)p6 + 5) &= -33;
        p6[9] &= 15;
        rec[85] = record;
        rec[92] = 1;
        rec7 = Script_unk2(17, 0x608);
        Script_RunTalkFlagStep(230);
        Script_Place(p6[28], 128, (rec7 + 0x400));
        Script_unk2_4TalkFlagStep(17);
        *(volatile s32 *)gOvTalkPairStep = (s32)rec;
    }
}

/* overlays/scene/actor/transition/run_scripted_step_17e3.c */
void Scene_RunScriptedStep17E3(void)
{
    Actor_RunTalkPairStep();
    Actor_ApplyTalkPairStep(0x17E3, 1);
    Actor_unk2_4TalkPairStep();
}
