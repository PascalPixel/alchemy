#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/transition/transition.h"

/* overlays/scene/actor/transition/actor_depth_effect.c */

/* Three sites of one import, so three names. */

void Scene_RunActorTenDepthSequence(void)
{
    /* r5 = -96 (movs #0x60 / negs), live across the first import call. */
    s32 depth = -96;

    Actor_Do((s32) 0xF1);
    ActorDraw_SetupActorNineForFlag30a(10, 0, depth);
    Actor_Place(10, 0, depth);
    Actor_Place2(10, 0, -80);
    Actor_Do2((s32) 0x121);
    Actor_Do3((s32) 0x307);
    Actor_Do4(2);
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
    Actor_Do5(0xF1);
    Actor_Place3(8, offset, 0);
    Actor_Place4(8, offset, 0);
    Actor_Do6(0x121);
    Actor_Do7(0x301);
    Actor_Do8(2);
    Actor_Run2();
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

Obj *Actor_Run3(s32);
Obj *Actor_Run4(s32);

struct Rec_39a *Actor_Run5();

void Actor_PlacePairAtOffset(s32 a0, s32 a1, s32 a2)
{
    Obj *p;
    Obj *q;
    s32 x;
    s32 y;

    p = Actor_Run3(gCell[125]);
    q = Actor_Run4(a0);
    Actor_Run6();
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
    Actor_Do9(p);
    Actor_Run7();
}

void Actor_PlaceAtTileAndMark(s32 id, s32 x, s32 y)
{
    struct Rec_39a *rec = Actor_Run5(id);

    if (rec != 0) {
        Actor_Run8(id, 3);
        rec->f34 = 2;
        rec->f35 |= 2;
        rec->f8 = (x << 20) + 0x80000;
        rec->f16 = (y << 20) + 0x80000;
    }
}

/* overlays/scene/actor/transition/actor_presentation.c */

void ActorDraw_SetupActorEightForFlag301(void)
{
    Actor_Do10(0xF1);
    Scene_RunActor8Transition304And305(8, 0x70, 0);
    Actor_Place5(8, 0x70, 0);
    Actor_Do11(0x121);
    Actor_Do12(0x301);
    Actor_Do13(2);
    Actor_Run9();
}

void ActorDraw_SetupActorNineForFlag302(void)
{
    Actor_Do14(0xF1);
    Actor_Place6(9, 0, -64);
    Actor_Do15(0x121);
    Actor_Do16(0x302);
    Actor_Do17(2);
    Actor_Run10();
}

void ActorDraw_SetupActorTenForFlag303(void)
{
    Actor_Do18(0xF1);
    Actor_Place7(0xA, 0, 0x40);
    Actor_Do19(0x121);
    Actor_Do20(0x303);
    Actor_Do21(2);
    Actor_Run11();
}

void ActorDraw_SetupActorNineForFlag306(void)
{
    Actor_Do22(0xF1);
    Actor_Place8(9, 0, 0x40);
    Actor_Do23(0x121);
    Actor_Do24(0x306);
    Actor_Do25(2);
    Actor_Run12();
}

void ActorDraw_SetupActorTenForFlag307(void)
{
    Actor_Do26(0xF1);
    Actor_Place9(10, 0, 144);
    Actor_Place10(10, 0, 128);
    Actor_Do27(0x121);
    Actor_Do28(0x307);
    Actor_Do29(2);
    Actor_Run13();
}

void ActorDraw_SetupActorEightForFlags308And309Guarded(void)
{
    /* movs r0,#0xc4 / lsls r0,#2 builds 0x310. */
    if (Actor_Check((s32) 0x310) != 0) {
        return;
    }
    if (Actor_Check2((s32) 0x30D) != 0) {
        return;
    }

    Actor_Do30((s32) 0xF1);
    /* movs r0,#0xc2 / lsls r0,#2 builds 0x308. */
    Actor_Do31((s32) 0x308);
    Actor_Do32((s32) 0x309);
    /* movs r1,#0x30 / negs r1,r1 */
    Actor_Place11(8, -48, 0);
    Actor_Do33((s32) 0x121);
    Actor_Do34(2);
    Actor_Run14();
}

void ActorDraw_SetupActorEightForFlags308And309(void)
{
    Actor_Do35(0xF1);
    Actor_Do36(0x308);
    Actor_Do37(0x309);
    Actor_Place12(8, 0x60, 0);
    Actor_Do38(0x121);
    Actor_Do39(2);
    Actor_Run15();
}

void ActorDraw_SetupActorNineForFlag30a(void)
{
    Actor_Do40(0xF1);
    Actor_Place13(9, -32, 0);
    Actor_Do41(0x121);
    Actor_Do42(0x30A);
    Actor_Do43(2);
    Actor_Run16();
}

void ActorDraw_SetupActorTenForFlags30bAnd30d(void)
{
    Actor_Do44(0xF1);
    Actor_Place14(0xA, 0, -64);
    Actor_Do45(0x121);
    Actor_Do46(0x30B);
    Actor_Do47(0x30D);
    Actor_Do48(2);
    Actor_Run17();
}

void ActorDraw_SetupActorElevenAt0_112(void)
{
    Actor_Do49(241);
    Actor_Place15(11, 0, 112);
    Actor_Do50(0x121);
    Actor_Run18();
    Actor_Do51(2);
    Actor_Run19();
}

void ActorDraw_SetupActorElevenAt0_64(void)
{
    Actor_Do52(0xF1);
    Actor_Place16(0xB, 0, 0x40);
    Actor_Do53(0x121);
    Actor_Run20();
    Actor_Do54(2);
    Actor_Run21();
}

void ActorDraw_SetupActorElevenAt0_80(void)
{
    Actor_Do55(0xF1);
    Actor_Place17(0xB, 0, 0x50);
    Actor_Do56(0x121);
    Actor_Run22();
    Actor_Do57(2);
    Actor_Run23();
}

void ActorDraw_SetupActorEightForFlag313(void)
{
    Actor_Do58(0xF1);
    Actor_Place18(8, 0, 0x70);
    Actor_Do59(0x121);
    Actor_Do60(0x313);
    Actor_Do61(2);
    Actor_Run24();
}

void ActorDraw_SetupActorNineForFlag314(void)
{
    Actor_Do62(0xF1);
    Actor_Place19(9, 0x80, 0);
    Actor_Do63(0x121);
    Actor_Do64(0x314);
    Actor_Do65(2);
    Actor_Run25();
}

void ActorDraw_SetupActorTenForFlag315(void)
{
    Actor_Do66(241);
    Actor_Place20(10, 160, 0);
    Actor_Do67(0x121);
    Actor_Do68(0x315);
    Actor_Do69(2);
    Actor_Run26();
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
    Actor_Do70((s32) 0xF1);

    /* movs r0,#0xc4 / lsls r0,#2 builds 0x310. The second test is only reached
     * when the first fails, and both truths take the same path. */
    if (Actor_Check3((s32) 0x310) != 0 || Actor_Check4((s32) 0x30D) != 0) {
        Actor_Place21(8, -48, 0);
        Actor_Do71((s32) 0x308);
        Actor_Do72((s32) 0x309);
    } else {
        Actor_Place22(8, -96, 0);
        Actor_Do73((s32) 0x308);
        Actor_Do74((s32) 0x309);
    }

    Actor_Do75((s32) 0x121);
    Actor_Do76(2);
    Actor_Run27();
}

void Scene_PlaceActorTenByFlags(void)
{
    Actor_Do77(241);
    if (Actor_Check5(0x308) != 0) {
        Actor_Place23(10, 0, -64);
        Actor_Do78(0x30b);
        Actor_Do79(0x30c);
        Actor_Do80(0x30d);
        Actor_Do81(0x30e);
    } else {
        Actor_Place24(10, 0, -128);
        Actor_Do82(0x30b);
        Actor_Do83(0x30c);
        Actor_Do84(0x30d);
        Actor_Do85(0x30e);
    }
    Actor_Do86(0x121);
    Actor_Do87(2);
    Actor_Run28();
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
    { s32 f1 = 8; s32 g1 = 29; Actor_Run29(8, 42, 15, 5,  f1, g1); }

    if (Actor_Check6((s32)0x301) != 0) {
        Actor_Run30(8, 22, 31);
        { s32 f2 = 8; s32 g2 = 30; Actor_Run31(9, 30, 1, 3,  f2, g2); }
    } else {
        Actor_Run32(8, 8, 31);
        { s32 f3 = 22; s32 g3 = 30; Actor_Run33(9, 30, 1, 3,  f3, g3); }
    }

    if (Actor_Check7((s32)0x302) != 0) {
        Actor_Run34(9, 12, 29);
        { s32 f4 = 11; s32 g4 = 33; Actor_Run35(14, 33, 3, 1,  f4, g4); }
    } else {
        Actor_Run36(9, 12, 33);
        { s32 f5 = 11; s32 g5 = 29; Actor_Run37(14, 29, 3, 1,  f5, g5); }
    }

    if (Actor_Check8((s32)0x303) != 0) {
        Actor_Run38(10, 18, 29);
        { s32 f6 = 17; s32 g6 = 33; Actor_Run39(14, 33, 3, 1,  f6, g6); }
    } else {
        Actor_Run40(10, 18, 33);
        { s32 f7 = 17; s32 g7 = 29; Actor_Run41(14, 29, 3, 1,  f7, g7); }
    }
}

void DialogueLayout_ConfigureGroupTwo(void)
{
    { s32 f1 = 12; s32 g1 = 8; Actor_Run42(0, 28, 10, 18,  f1, g1); }

    if (Actor_Check9((s32)0x304) != 0) {
        Actor_Run43(8, 21, 20);
        { s32 f2 = 13; s32 g2 = 19; Actor_Run44(20, 19, 1, 3,  f2, g2); }
    } else {
        Actor_Run45(8, 13, 20);
        { s32 f3 = 21; s32 g3 = 19; Actor_Run46(20, 19, 1, 3,  f3, g3); }
    }

    if (Actor_Check10((s32)0x305) != 0) {
        Actor_Run47(8, 12, 20);
        { s32 f4 = 12; s32 g4 = 19; Actor_Run48(5, 19, 1, 3,  f4, g4); }
        { s32 f5 = 13; s32 g5 = 19; Actor_Run49(20, 19, 1, 3,  f5, g5); }
        if (Actor_Check11((s32)0x304) != 0) {
            Actor_Run50(8, 21, 20);
            { s32 f6 = 13; s32 g6 = 19; Actor_Run51(20, 19, 1, 3,  f6, g6); }
            { s32 f7 = 12; s32 g7 = 19; Actor_Run52(20, 19, 1, 3,  f7, g7); }
        }
    }

    if (Actor_Check12((s32)0x306) != 0) {
        Actor_Run53(9, 15, 21);
        { s32 f8 = 14; s32 g8 = 17; Actor_Run54(14, 18, 3, 1,  f8, g8); }
    } else {
        Actor_Run55(9, 15, 17);
        { s32 f9 = 14; s32 g9 = 21; Actor_Run56(14, 18, 3, 1,  f9, g9); }
    }

    if (Actor_Check13((s32)0x307) != 0) {
        Actor_Run57(10, 19, 8);
        { s32 f10 = 18; s32 g10 = 25; Actor_Run58(14, 18, 3, 1,  f10, g10); }
    } else {
        Actor_Run59(10, 19, 25);
        { s32 f11 = 18; s32 g11 = 8; Actor_Run60(14, 18, 3, 1,  f11, g11); }
    }
}

void DialogueLayout_ConfigureGroupThree(void)
{
    { s32 k5 = 12, k6 = 21; Actor_Run61(12, 3, 9, 16, k5, k6); }

    if (Actor_Check14((s32)0x308) != 0) {
        Actor_Run62(8, 14, 25);
        { s32 k5 = 20, k6 = 24; Actor_Run63(16, 24, 1, 3, k5, k6); }
    } else if (Actor_Check15((s32)0x309) != 0) {
        Actor_Run64(8, 17, 25);
        { s32 k6 = 24;
          Actor_Run65(18, 24, 1, 3, 20, k6);
          Actor_Run66(18, 24, 1, 3, 14, k6);
          Actor_Run67(8, 41, 1, 3, 17, k6);
        }
    } else {
        Actor_Run68(8, 20, 25);
        { s32 k5 = 14, k6 = 24; Actor_Run69(16, 24, 1, 3, k5, k6); }
    }

    if (Actor_Check16((s32)0x30a) != 0) {
        Actor_Run70(9, 13, 35);
        { s32 k5 = 15, k6 = 34; Actor_Run71(14, 34, 1, 3, k5, k6); }
    } else {
        Actor_Run72(9, 15, 35);
        { s32 k5 = 13, k6 = 34; Actor_Run73(14, 34, 1, 3, k5, k6); }
    }

    if (Actor_Check17((s32)0x30b) != 0) {
        Actor_Run74(10, 15, 22);
        { s32 k5 = 14;
          Actor_Run75(14, 29, 3, 1, k5, 30);
          Actor_Run76(5, 41, 3, 1, k5, 22);
        }
    } else if (Actor_Check18((s32)0x30c) != 0) {
        Actor_Run77(10, 15, 23);
        { s32 k5 = 14;
          Actor_Run78(5, 42, 3, 1, k5, 23);
          Actor_Run79(14, 29, 3, 1, k5, 30);
          Actor_Run76(10, 44, 3, 1, k5, 21);
        }
    } else if (Actor_Check19((s32)0x30d) != 0) {
        Actor_Run80(10, 15, 26);
        { s32 k5 = 14;
          Actor_Run81(14, 29, 3, 1, k5, 22);
          Actor_Run82(5, 43, 3, 1, k5, 26);
          Actor_Run76(14, 29, 3, 1, k5, 30);
        }
    } else if (Actor_Check20((s32)0x30e) != 0) {
        Actor_Run83(10, 15, 27);
        { s32 k5 = 14;
          Actor_Run84(14, 29, 3, 1, k5, 22);
          Actor_Run85(14, 29, 3, 1, k5, 30);
          Actor_Run76(5, 44, 3, 1, k5, 27);
        }
    } else {
        Actor_Run86(10, 15, 30);
    }

    if (Actor_Check21((s32)0x30f) != 0) {
        Actor_Run87(11, 15, 23);
        { s32 k5 = 14;
          Actor_Run88(14, 29, 3, 1, k5, 31);
          Actor_Run89(10, 40, 3, 1, k5, 23);
        }
    } else if (Actor_Check22((s32)0x310) != 0) {
        Actor_Run90(11, 15, 24);
        { s32 k5 = 14;
          Actor_Run91(14, 29, 3, 1, k5, 31);
          Actor_Run89(10, 41, 3, 1, k5, 24);
        }
    } else if (Actor_Check23((s32)0x311) != 0) {
        Actor_Run92(11, 15, 27);
        { s32 k5 = 14;
          Actor_Run93(14, 29, 3, 1, k5, 31);
          Actor_Run89(10, 42, 3, 1, k5, 27);
        }
    } else if (Actor_Check24((s32)0x312) != 0) {
        Actor_Run94(11, 15, 28);
        { s32 k5 = 14;
          Actor_Run95(14, 29, 3, 1, k5, 31);
          Actor_Run89(10, 43, 3, 1, k5, 28);
        }
    } else {
        Actor_Run96(11, 15, 31);
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
        { s32 f1 = 14; s32 g1 = 10; Actor_Run97(22, 20, 9, 8,  f1, g1); }
    } else {
        { s32 f2 = 7; s32 g2 = 45; Actor_Run98(20, 45, 11, 4,  f2, g2); }
    }

    if (Actor_Check25((s32)0x313) != 0) {
        Actor_Run99(8, 20, 17);
        { s32 f3 = 19; s32 g3 = 10; Actor_Run100(19, 11, 3, 1,  f3, g3); }
    } else {
        Actor_Run101(8, 20, 10);
        { s32 f4 = 19; s32 g4 = 17; Actor_Run102(19, 11, 3, 1,  f4, g4); }
    }

    /* 0x314 is built by shifting. */
    if (Actor_Check26((s32)0x314) != 0) {
        Actor_Run103(9, 14, 16);
        { s32 f5 = 22; s32 g5 = 15; Actor_Run104(16, 15, 1, 3,  f5, g5); }
    } else {
        Actor_Run105(9, 22, 16);
        { s32 f6 = 14; s32 g6 = 15; Actor_Run106(16, 15, 1, 3,  f6, g6); }
    }

    if (Actor_Check27((s32)0x315) != 0) {
        Actor_Run107(10, 17, 46);
        { s32 f7 = 7; s32 g7 = 45; Actor_Run108(15, 15, 1, 3,  f7, g7); }
    } else {
        Actor_Run109(10, 7, 46);
        { s32 f8 = 17; s32 g8 = 45; Actor_Run110(15, 15, 1, 3,  f8, g8); }
    }
}

/* overlays/scene/actor/transition/field_scene_head.c */

/* Imports; the queried ones are typed for their return value. */

void Scene_RunActor9Transition302(void)
{
    Actor_Do88(0xF1);
    Actor_Place25(9, 0, 0x40);
    Actor_Do89(0x121);
    Actor_Do90(0x302);
    Actor_Do91(2);
    Actor_Run111();
}

void Scene_RunActor10Transition303(void)
{
    Actor_Do92(0xF1);
    Actor_Place26(0xA, 0, -64);
    Actor_Do93(0x121);
    Actor_Do94(0x303);
    Actor_Do95(2);
    Actor_Run112();
}

void Scene_RunActor8Transition304(void)
{
    Actor_Do96(0xF1);
    Actor_Place27(8, 0x90, 0);
    Actor_Do97(0x121);
    Actor_Do98(0x304);
    Actor_Do99(2);
    Actor_Run113();
}

void Scene_RunActor8Transition304And305(void)
{
    Actor_Do100(0xF1);
    Actor_Place28(8, -144, 0);
    Actor_Do101(0x121);
    Actor_Do102(0x304);
    Actor_Do103(0x305);
    Actor_Do104(2);
    Actor_Run114();
}

void Scene_RunActor8Transition305(void)
{
    Actor_Do105(0xF1);
    Actor_Place29(8, -14, 0);
    Actor_Do106(0x121);
    Actor_Do107(0x305);
    Actor_Do108(2);
    Actor_Run115();
}

void Scene_RunActor8FlaggedSequence(void)
{
    Actor_Do109((s32) 0xF1);

    if (Actor_Check28((s32) 0x306) != 0) {
        Actor_Place30(8, 16, 0);
        Actor_Do110((s32) 0x305);
    } else {
        Actor_Place31(8, 144, 0);
        /* movs r0,#0xc1 / lsls r0,#2 builds 0x304. */
        Actor_Do111((s32) 0x304);
    }

    Actor_Do112((s32) 0x121);
    Actor_Do113(2);
    Actor_Run116();
}

void Scene_RunActor9Flag306Sequence(void)
{
    Actor_Do114(0xF1);
    Actor_Place32(9, 0, -64);
    Actor_Do115(0x121);
    Actor_Do116(0x306);
    Actor_Do117(2);
    Actor_Run117();
}

void Scene_RunActor8Transition308And309(void)
{
    Actor_Do118(0xF1);
    Actor_Do119(0x308);
    Actor_Do120(0x309);
    Actor_Place33(8, 0x30, 0);
    Actor_Do121(0x121);
    Actor_Do122(2);
    Actor_Run118();
}

void Scene_RunActor9Transition30A(void)
{
    Actor_Do123(0xF1);
    Actor_Place34(9, 0x20, 0);
    Actor_Do124(0x121);
    Actor_Do125(0x30A);
    Actor_Do126(2);
    Actor_Run119();
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
    Actor_Run120((s32)0xf1);
    /* 0x308 is built by shifting. */
    if (Actor_Check29((s32)0x308) != 0) {
        Actor_Run121(10, 0, 16);
        Actor_Run122((s32)0x30b);
        Actor_Run123((s32)0x30c);
        Actor_Run124((s32)0x30d);
        Actor_Run125((s32)0x30e);
        /* 0x310 is built by shifting. */
    } else if (Actor_Check30((s32)0x310) != 0) {
        Actor_Run126(10, 0, 16);
        Actor_Run127((s32)0x30b);
        Actor_Run128((s32)0x30c);
        Actor_Run129((s32)0x30d);
        Actor_Run125((s32)0x30e);
    } else if (Actor_Check31((s32)0x311) != 0) {
        Actor_Run130(10, 0, 64);
        Actor_Run131((s32)0x30b);
        Actor_Run132((s32)0x30c);
        Actor_Run133((s32)0x30d);
        Actor_Run125((s32)0x30e);
    } else {
        Actor_Run134(10, 0, 128);
        Actor_Run135((s32)0x30b);
        Actor_Run136((s32)0x30c);
        Actor_Run137((s32)0x30d);
        Actor_Run138((s32)0x30e);
    }
    Actor_Run139((s32)0x121);
    Actor_Run140(2);
    Actor_Run141();
}

void Scene_RunActor10Transition30BTo30E(void)
{
    Actor_Do127(241);
    Actor_Place35(10, 0, -16);
    Actor_Do128(0x121);
    Actor_Do129(0x30b);
    Actor_Do130(0x30c);
    Actor_Do131(0x30d);
    Actor_Do132(0x30e);
    Actor_Do133(2);
    Actor_Run142();
}

/*
 * Scene step for resource_39a.  Nothing is returned; the five pool words
 * after the return belong to the owner.  Actor_Check32 serves both a query
 * and a setter, so its result is dropped at the setter site.  The tail call
 * shared by the first two arms is written out in each arm rather than adding
 * a flag the reference does not have.  Imports are named by the address their
 * call site computes, and are old-style because arity varies between sites.
 */
void Scene_RunFlag311DialogueBranch(void)
{
    Actor_Run143((s32)0xf1);
    if (Actor_Check33((s32)0x311) != 0) {
        Actor_Run144(10, 0, 48);
        Actor_Run145((s32)0x30b);
        /* 0x30c is built by shifting; the result is unused. */
        Actor_Check32((s32)0x30c);
        Actor_Run146((s32)0x30d);
        Actor_Run147((s32)0x30e);
        /* 0x310 is built by shifting. */
    } else if (Actor_Check34((s32)0x310) != 0) {
        Actor_Run148(10, 0, 32);
        Actor_Run149((s32)0x30b);
        Actor_Run150((s32)0x30c);
        Actor_Run151((s32)0x30d);
        Actor_Run147((s32)0x30e);
    } else {
        Actor_Run152(10, 0, 112);
        Actor_Run153((s32)0x30b);
        Actor_Run154((s32)0x30c);
        Actor_Run155((s32)0x30d);
        Actor_Run156((s32)0x30e);
    }
    Actor_Run157((s32)0x121);
    Actor_Run158(2);
    Actor_Run159();
}

void Scene_RunActor10Flags30bTo30eSequenceA(void)
{
    Actor_Run160(241);
    Actor_Place36(10, 0, 64);
    Actor_Do134(0x121);
    Actor_Run161(0x30b);
    Actor_Run162(0x30c);
    Actor_Run163(0x30d);
    Actor_Do135(0x30e);
    Actor_Do136(2);
    Actor_Run164();
}

void Scene_RunActor10Flags30bTo30eSequenceB(void)
{
    Actor_Do137(241);
    Actor_Place37(10, 0, -80);
    Actor_Do138(0x121);
    Actor_Do139(0x30b);
    Actor_Do140(0x30c);
    Actor_Do141(0x30d);
    Actor_Do142(0x30e);
    Actor_Do143(2);
    Actor_Run165();
}

void Scene_RunActor10Flags30bTo30eSequenceC(void)
{
    Actor_Do144(0xF1);
    Actor_Place38(0xA, 0, 0x40);
    Actor_Do145(0x121);
    Actor_Do146(0x30B);
    Actor_Do147(0x30C);
    Actor_Do148(0x30D);
    Actor_Do149(0x30E);
    Actor_Do150(2);
    Actor_Run166();
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

struct Actor_39a *Actor_Run167();

struct Actor_39a *OvObj_CreateAndInitialize(s32 a, s32 b, s32 c, s32 d)
{
    struct Actor_39a *actor = Actor_Run167(d, a, b, c);

    if (actor != 0) {
        actor->f80->mode = 1;
        actor->f85 = 0;
        Actor_Run168(actor, 0);
        Actor_Run169(actor, 15);
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
 * interfaces are left open.  Actor_Check35 is reached both as a setter and as
 * a query, so its result is dropped at the setter site.  The first two arms
 * share one tail call, which is why Actor_Run170 is spelled out in each arm.
 */
void Scene_RunActorElevenFlaggedSteps(void)
{
    Actor_Run171((s32)0xf1);
    /* 0x308 is built by shifting a small immediate, not loaded whole. */
    if (Actor_Check36((s32)0x308) != 0 || Actor_Check37((s32)0x30d) != 0) {
        Actor_Run172(11, 0, -64);
        Actor_Run173((s32)0x30f);
        Actor_Check35((s32)0x310);
        Actor_Run174((s32)0x311);
        Actor_Run170((s32)0x312);
        /* 0x30c is built by shifting a small immediate, not loaded whole. */
    } else if (Actor_Check38((s32)0x30c) != 0) {
        Actor_Run175(11, 0, -112);
        Actor_Run176((s32)0x30f);
        Actor_Run177((s32)0x310);
        Actor_Run178((s32)0x311);
        Actor_Run170((s32)0x312);
    } else {
        Actor_Run179(11, 0, -128);
        Actor_Run180((s32)0x30f);
        Actor_Run181((s32)0x310);
        Actor_Run182((s32)0x311);
        Actor_Run183((s32)0x312);
    }
    Actor_Run184((s32)0x121);
    Actor_Run185(2);
    Actor_Run186();
}

/* overlays/scene/actor/transition/run_scene_four_pass_callback_sequence.c */
/* Imports, named by the address each call site computes. */

extern

/*
 * Field scene sequence for overlay resource_39a.  Imports are named by the
 * address their call site computes, not by a location in this image, and
 * their interfaces are left open.  The declarations are old-style because
 * Actor_Run187 and Actor_Run188 are each reached with two different
 * argument counts.  Actor_Run189 names a loader-relocated call word handed
 * to two imports as a callback, not a runtime address.
 */
void Scene_RunFourPassCallbackSequence(void)
{
    s32 pass;
    s32 step;
    s32 span;
    s32 one;

    Actor_Run187(19);
    Actor_Run190(182);
    Actor_Run191();
    Actor_Run192();

    /* 8, 7 and 1 are locals held across the loop, not literals: the first
     * call takes 8 as an immediate for argument 4 and from a register for
     * argument 5, which a literal cannot produce. */
    pass = 0;
    step = 8;
    span = 7;
    one = 1;
    do {
        Actor_Run193((s32)0x204318, 1);
        Actor_Run194(1);
        Actor_Run195(2);
        if (pass == 0) {
            Actor_Run188(30, 8, 12, 8, step, span);
            Actor_Run196(30, 57, 19, 57, one, one);
        }
        Actor_Run197((s32)0x203108, 1);
        Actor_Run198(1);
        Actor_Run199(2);
        /* The increment belongs to the loop test, not the body: `pass++;` as
         * a statement would not place it after the last call.  The compare is
         * unsigned against 3, so the body runs for pass 0 to 3. */
    } while ((unsigned int)++pass <= 3);

    Actor_Run200(30);
    /* 0xc80 is built by shifting a small immediate, not loaded whole. */
    Actor_Run201((void *)Actor_Run189, (s32)0xc80);
    Actor_Run202(40);
    Actor_Run203((s32)0x201090, 1);
    Actor_Run204(40);
    Actor_Run205(80);
    Actor_Run206((void *)Actor_Run189);
    Actor_Run207(20);
    /* 0x10000 is built by shifting a small immediate, not loaded whole. */
    Actor_Run208((s32)0x10000, 1);
    Actor_Run209(80);
    /* Same import as in the loop, one argument here. */
    Actor_Run210(80);
    /* 0x820 is built by shifting a small immediate, not loaded whole. */
    Actor_Run211((s32)0x820);
    Actor_Run212(230);
    Actor_Run213();
    /* Same import as the first call, no argument register written here. */
    Actor_Run214();
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
    Actor_Run215();

    /* r5 holds &gOv26 across the calls; the word is reloaded before
     * the second test. */
    if (gOv26 != 0) {
        Actor_Apply14(gOv26, 3);
    }

    Actor_Apply15((s32) 0xE6, 0);
    Actor_Do151((s32) 0xF13);

    if (gOv26 != 0) {
        Actor_Do152(gOv26);
    }

    Actor_Run216();
}

void Dialogue_RunLine17e2(void)
{
    Actor_Run217();
    Actor_Apply16(0x17E2, 1);
    Actor_Run218();
}

/* overlays/scene/actor/transition/shared.c */

void Scene_RunSingleStep(void)
{
    Actor_Run219();
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

struct Subject *Actor_Run220();

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
    Actor_Run221((s32)0x100000, heading, probe);
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

    subject = Actor_Run220(gCell.selected_subject);

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
        Actor_Run222();

        /* The 0x80000 bias is built by shifting, not loaded as a constant. */
        probe[0] = (subject->x & (s32)0xfff00000) + 0x80000;
        probe[1] = subject->y;
        probe[2] = (subject->z & (s32)0xfff00000) + 0x80000;
        z = probe[2];
        x = probe[0];
        subject_id = (u8 *)subject;
        subject_id += 34;
        goal = Actor_Check39((s32)*subject_id, x, z);
        /*
         * 0x100000 is built by shifting, not loaded as a constant.  The probe
         * block is passed by address and is advanced by the callee.
         */
        Actor_Run223((s32)0x100000, heading, probe);

        marker = Actor_Check40((s32)*subject_id, probe[0], probe[2]);
        if (marker == 255
                || Actor_Check41((s32)*subject_id, probe[0], probe[2])
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
        Actor_Run224(subject, x, subject->y, z);
        /*
         * Same call word as the marker lookup, but a two-argument command, so
         * it keeps its own declaration.
         */
        Actor_Run225(subject, 2);
        Actor_Run226(subject, 48);
        Actor_Run227(subject);
        subject->callback = (void *)Actor_Run228;

        goto advance_probe;
continue_probe:
        if (Actor_Check42((s32)*subject_id, probe[0], probe[2])
                - subject->y > 0x80000) {
            goto finish_probe;
        }
        x = probe[0];
        z = probe[2];
        subject->state_048 = 0x20000;
        subject->state_052 = 0x1999;
        Actor_Run229(subject, probe[0], probe[1], probe[2]);
        Actor_Run230(subject);
        if (marker != goal) {
            goto blocked;
        }

advance_probe:
        AdvanceProbe(heading, probe);
        marker = Actor_Check43((s32)*subject_id, probe[0], probe[2]);
        if (marker != 255) {
            goto continue_probe;
        }

finish_probe:
        subject->state_048 = 0x20000;
        subject->state_052 = 0x10000;
        Actor_Run231(subject, x, subject->y, z);
        Actor_Run232(subject);
        Actor_Run233(2);
        /* The back edge re-reads the heading table and starts again. */
    }

blocked:
    subject->callback = NULL;
    subject->flags_090 |= 1;
    /* 0x4000 is built by shifting, not loaded as a constant. */
    subject->state_052 = 0x4000;

tail:
    Actor_Run234(10);
    Actor_Run235();
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

struct SceneService *Actor_Run236(s32 index);

/* 0x03001ebc is the overlay's workspace pointer. */

/*
 * Imports. Each alias names the call word its site encodes, not a runtime
 * address. Only those used for their return value are typed, and the
 * declarations are old-style because one name is reached with different
 * argument counts.
 */

void State_ApplyValues8And2And1(void)
{
    Actor_Place39(8, 2, 1);
}

void State_ApplyValues11And62(void)
{
    Actor_Apply17(0xB, 0x3E);
}

void State_SetServiceZeroValue06(void)
{
    struct SceneService *work;

    Actor_Run237();
    work = Actor_Run236(0);
    work->value06 = 0x4000;
    Actor_Do153(123);
    Actor_Run238();
    Actor_Run239();
    Actor_Do154(1);
}

void State_StoreValueToWorkspaceWord24WhenFlagged(void)
{
    s32 *flag = (s32 *)0x0200B328;

    if (*flag != 0) {
        u8 *state = gIw2;

        *(s32 *)(state + 24) = Actor_Check44(0);
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
    Actor_Run240();
    Actor_Apply20(0x17E1, 1);
    Actor_Run241();
}

/*
 * Branch on flag 0x820 -- resource_39a. One arm sets a record flag; the other
 * sets a different flag and writes workspace halfword 370. Nothing is
 * returned, and the owner extends through the three pool words that follow
 * the epilogue.
 */
void State_SetWorkspace370ByFlag820(void)
{
    Actor_Run242();
    /* movs r0,#0x82 / lsls r0,#4 builds 0x820. */
    if (Actor_Check45((s32)0x820) != 0) {
        Actor_Run243((s32)0x17e5, 1);
    } else {
        Actor_Run244((s32)0x17e4, 1);
        if (Actor_Check46((s32)0xe6) != -1) {
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
    Actor_Run245();
}
