#include "types.h"
#include "facing_object.h"

/* overlays/scene/story/set_piece/actor_facing.c */
#define NULL ((void *)0)
#define CalculateFacingAngle Func_02003afa
#define OvObj_UpdateFacingTowardTarget Func_02000040


s32 Func_02003afa(s32, s32);

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
#undef NULL
#undef CalculateFacingAngle
#undef OvObj_UpdateFacingTowardTarget

/* overlays/scene/story/set_piece/actor_motion.c */
#define Scene_RunActorTenFourStepSequence Func_02002d08
void Func_020069e8();          /* site veneer -> Func_0808a428 */

void Func_020069f8();          /* site veneer -> Func_0808a430 */

void Func_02006a14();          /* site veneer -> Func_0808a448 */

void Func_0200693c();          /* site veneer -> Func_0808a158 (1st) */

void Func_020069fa();          /* site veneer -> Func_0808a420 */

void Func_0200694a();          /* site veneer -> Func_0808a158 (2nd) */

void Func_02006a1e();          /* site veneer -> Func_0808a438 */

void Func_02006956();          /* site veneer -> Func_0808a158 (3rd) */

void Func_02006a32();          /* site veneer -> Func_0808a440 */

void Func_02006a6a();          /* site veneer -> Audio_PlayCue */

void Func_0200696a();          /* site veneer -> Func_0808a158 (4th) */

void Scene_RunActorTenFourStepSequence(void)
{
    Func_020069e8(24, 1);
    Func_020069f8(10, 9);
    Func_02006a14();
    Func_0200693c(10, 2);
    Func_020069fa(1);
    Func_0200694a(10, 2);
    Func_02006a1e();
    Func_02006956(10, 2);
    Func_02006a32();
    Func_02006a6a(288);
    Func_0200696a(10, 2);
}
#undef Scene_RunActorTenFourStepSequence

/* overlays/scene/story/set_piece/actor_position.c */
#define Actor_CopyActor8PositionWithFixedY Func_02000098
#define Actor_UpdateSlot10ByTileX Func_0200040c
#define Actor_RaiseSlot9StepA Func_0200050c
#define Actor_RaiseSlot9StepB Func_0200055c
#define Actor_RaiseSlot11AndSetFlag201 Func_020005fc
#define Actor_ClearCollisionFlagAndPlaceMarker Func_02003460
struct Frame {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct Frame *Func_02003c38(s32);
s32 *Func_02003fac(s32);
void Func_02003f56(s32, s32, s32, s32, s32, s32);
void Func_02003f6e(s32, s32, s32, s32, s32, s32);
void Func_02003f80(s32, s32, s32, s32, s32, s32);
void Func_02003f90(s32 *, s32);
void Func_02003f5c(void);
void Func_02003eea(s32);
s32 *Func_020040ac(s32);
void Func_02004050(s32, s32, s32, s32, s32, s32);
void Func_02004064(s32 *, s32);
void Func_020040a4(s32);
s32 *Func_020040fc(s32);
void Func_020040a2(s32, s32, s32, s32, s32, s32);
void Func_020040b6(s32 *, s32);
void Func_020040f6(s32);
s32 *Func_0200419c(s32);
void Func_02004142(s32, s32, s32, s32, s32, s32);
void Func_02004156(s32 *, s32);
void Func_02004194(s32);

/*
 * resource_3a4 owner at 0x02003460, 34 bytes with no literal pool: clear
 * bit 0 of the scene record's flags byte at +89, then place a marker at the
 * record's x and z at level 255. Both callees are reached through veneers.
 */

u8 *Func_02006ffc();           /* Scene record accessor. */

void Func_02006fbc();          /* Place a marker: kind, x, z, level. */

s32 Actor_CopyActor8PositionWithFixedY(struct Frame *dst)
{
    struct Frame *src = Func_02003c38(8);

    dst->f08 = src->f08;
    dst->f0c = 0xFFF40000;
    dst->f10 = src->f10;
    return 0;
}

void Actor_UpdateSlot10ByTileX(void)
{
    s32 *a = Func_02003fac(10);

    if (a != 0) {
        s32 x = 24;
        s32 y = 26;
        s32 t;

        Func_02003f56(x, 27, 2, 1, x, y);
        t = a[2] >> 20;
        if (t == 25) {
            Func_02003f6e(0, 0, 1, 1, t, y);
        } else {
            Func_02003f80(0, 0, 1, 1, x, y);
        }
        Func_02003f90(a, 0);
        ((u8 *)a)[0x55] = 0;
        Func_02003f5c();
        Func_02003eea(1);
    }
}

void Actor_RaiseSlot9StepA(void)
{
    s32 *p;
    s32 s0;

    p = Func_020040ac(9);
    s0 = 26;
    Func_02004050(0, 0, 1, 1, s0, s0);
    if (p != 0) {
        Func_02004064(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    Func_020040a4(0x200);
}

void Actor_RaiseSlot9StepB(void)
{
    s32 *p;
    s32 s0;
    s32 s1;

    p = Func_020040fc(9);
    s0 = 25;
    s1 = 13;
    Func_020040a2(23, 13, 1, 1, s0, s1);
    if (p != 0) {
        Func_020040b6(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    Func_020040f6(0x200);
}

void Actor_RaiseSlot11AndSetFlag201(void)
{
    s32 *p;
    s32 s0;
    s32 s1;

    p = Func_0200419c(11);
    s0 = 17;
    s1 = 10;
    Func_02004142(1, 0, 1, 1, s0, s1);
    if (p != 0) {
        Func_02004156(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    Func_02004194(0x201);
}

void Actor_ClearCollisionFlagAndPlaceMarker(s32 no)
{
    u8 *record;

    record = Func_02006ffc(no);
    record[89] &= 0xfe;

    Func_02006fbc(0, *(s32 *)(record + 8), *(s32 *)(record + 16), 255);
}
#undef Actor_CopyActor8PositionWithFixedY
#undef Actor_UpdateSlot10ByTileX
#undef Actor_RaiseSlot9StepA
#undef Actor_RaiseSlot9StepB
#undef Actor_RaiseSlot11AndSetFlag201
#undef Actor_ClearCollisionFlagAndPlaceMarker

/* overlays/scene/story/set_piece/actor_presentation.c */
#define ActorDraw_SetCellAndLowerActorEight Func_0200047c
#define Actor_SetActor10Byte23To3 Func_02000c74
#define Actor_SetActor10Byte23To1 Func_02000c88
#define Actor_SetMode3AndRate4ccc Func_02002a10
#define Actor_SetActor8ModeByCounterBit Func_02002ffc
#define Actor_SetActor12ModeByActorZeroHeight Func_02003428
extern unsigned char Value_00004ccc;
extern u32 Data_03001e40;

s32 *Func_0200401c(s32);
void Func_02003fc2(s32, s32, s32, s32, s32, s32);
void Func_02003fd6(s32 *, s32);
void Func_02004016(s32);
u8 *Func_02004812(s32);
u8 *Func_02004826(s32);
void Func_0200664e();
void Func_02006566();
void Func_02006c2a();
void Func_02006c34();
u8 *Func_02006fc6(s32 id);
u8 *Func_02006fd6(s32 id);
void Func_020070a8(s32 id, s32 arg1);
void Func_020070b2(s32 id, s32 arg1);

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

    p = Func_0200401c(8);
    s0 = 9;
    s1 = 13;
    Func_02003fc2(7, 13, 1, 1, s0, s1);
    if (p != 0) {
        Func_02003fd6(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    Func_02004016(0x200);
}

void Actor_SetActor10Byte23To3(void)
{
    Func_02004812(10)[0x23] = 3;
}

void Actor_SetActor10Byte23To1(void)
{
    Func_02004826(10)[0x23] = 1;
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
    Func_0200664e(rec, 3);
    Func_02006566(rec, 0);
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
    if (((Data_03001e40 >> 1) & 1) != 0) {
        Func_02006c2a(8, 7);
    } else {
        Func_02006c34(8, 6);
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
    if (*(s32 *)(Func_02006fc6(0) + 12) > 0x00300000) {
        {
            u8 *flag = Func_02006fd6(11) + 35;
            s32 bit = 2;

            bit |= *flag;
            *flag = bit;
        }
        Func_020070a8(12, 3);
    } else {
        Func_020070b2(12, 2);
    }
}
#undef ActorDraw_SetCellAndLowerActorEight
#undef Actor_SetActor10Byte23To3
#undef Actor_SetActor10Byte23To1
#undef Actor_SetMode3AndRate4ccc
#undef Actor_SetActor8ModeByCounterBit
#undef Actor_SetActor12ModeByActorZeroHeight

/* overlays/scene/story/set_piece/display.c */
#define State_SetDispcntBit9ByThreshold Func_020006a0
extern u16 Data_0200d238;

s32 Func_02004142(void);

void State_SetDispcntBit9ByThreshold(void)
{
    volatile u16 *reg = (volatile u16 *)0x04000000;
    s16 v = *reg & 0xfdff;

    if ((u32)(Func_02004142() * 100) >> 16 >= Data_0200d238) {
        s32 k = 0x200;

        v |= k;
    }
    {
        u32 t = (u16)v;

        *reg = t;
    }
}
#undef State_SetDispcntBit9ByThreshold

/* overlays/scene/story/set_piece/effect.c */
#define Effect_SpawnObject222 Func_02002b58
extern s32 Data_03001e40;
extern s32 Data_0200d240[3];   /* image offset 0x5240 */
extern u8 *Func_02006652(s32 arg0, s32 arg1, s32 arg2, s32 arg3);  /* site veneer -> Func_080090c8 */
extern void Func_0200559e(u8 *obj);                             /* site veneer -> Func_02002a10 */
extern void Func_0200666a(u8 *obj, s32 arg1);                   /* site veneer -> Object_SetMode */

void Effect_SpawnObject222(void)
{
    u8 *obj;

    if ((Data_03001e40 & 3) != 0) {
        return;
    }
    obj = Func_02006652(222, Data_0200d240[0], Data_0200d240[1], Data_0200d240[2]);
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
    Func_0200559e(obj);
    *(s32 *)(obj + 108) = 0x0200aa49;
    Func_0200666a(obj, 1);
}
#undef Effect_SpawnObject222

/* overlays/scene/story/set_piece/field_scene.c */
#define Battle_Reset_1() Value0(Func_02006a84)
#define Motion_SetSpeedLim_1(a0, a1) Call2(Func_02006b94, a0, a1)
#define Motion_CamBounds_1(a0, a1, a2, a3) Call4(Func_02006bae, a0, a1, a2, a3)
#define Motion_SetSpeed_1(a0, a1, a2) Call3(Func_02006ad8, a0, a1, a2)
#define Motion_SetPosReset_1(a0, a1, a2) Value3(Func_02006b1c, a0, a1, a2)
#define Audio_PlayCue_1(a0) Value1(Func_02006c6a, a0)
#define Motion_SetSpeed_2(a0, a1, a2) Call3(Func_02006b0e, a0, a1, a2)
#define Motion_SetSpeed_3(a0, a1, a2) Call3(Func_02006b18_a, a0, a1, a2)
#define Object_SetModeById_1(a0, a1) Call2(Func_02006b70, a0, a1)
#define Motion_ResetAndSetPosition_1(a0, a1, a2) Call3(Func_02006b4c, a0, a1, a2)
#define Motion_ResetAndSetPosition_2(a0, a1, a2) Value3(Func_02006b58, a0, a1, a2)
#define Battle_WaitMode0_1(a0) Value1(Func_02006b06, a0)
#define BattleFx_SpawnLinked_1(a0, a1, a2) Call3(Func_02006c0a, a0, a1, a2)
#define Motion_SetVarCb_1(a0, a1) Value2(Func_02006bba, a0, a1)
#define Motion_CommitPos_1(a0) Value1(Func_02006b98, a0)
#define SharedWorkData_SetFirstAndSecondFields_1(a0, a1) Call2(Func_02006c72, a0, a1)
#define BattleFx_ComputeWeightedResultAndDispatch_1(a0, a1) Call2(Func_02006c6a_a, a0, a1)
#define WORK_BYTE_22B (Data_02000240[0x22B])
#define Scene_RunEarlySequence Func_020006dc
#define Scene_RunScene3a4SequenceB Func_020009ec
#define Scene_RunScene3a4SequenceA Func_02000a94
#define Scene_RunLine1528Sequence Func_02000b0c
#define Scene_RunScene3a4SequenceC Func_02000b3c
#define Scene_RunScene3a4SequenceD Func_02000bd8
#define FieldScene_RunScene3a4_02000c9c Func_02000c9c
#define Scene_RunSharedSetPiece Func_02000ec0
#define FieldScene_RunScene3a4_02001398 Func_02001398
#define FieldScene_RunScene3a4_02002310 Func_02002310
#define FieldScene_RunScene3a4_02002428 Func_02002428
#define FieldScene_RunScene3a4_02002490 Func_02002490
#define FieldScene_RunScene3a4_020025c0 Func_020025c0
#define FieldScene_RunScene3a4_020026c0 Func_020026c0
#define FieldScene_RunScene3a4_02002934 Func_02002934
#define FieldScene_RunScene3a4_020029dc Func_020029dc
#define Scene_RunScene3a4SequenceG Func_02002be0
#define Scene_RunOpeningAuxiliarySequence Func_02002d58
#define Scene_RunScene3a4SequenceF Func_02002e1c
#define Scene_RunScene3a4SequenceE Func_02002e84
#define Scene_RunLateAuxiliarySequence Func_02002f10

extern u8 Data_00000000[];
extern u8 Data_0200d238[];
extern u8 Data_03001ebc[];
extern u8 Data_00001953[];
extern s16 Data_02000240_t[][1];
extern u8 Value_00000056;

void Func_0200418e();
void Func_020041d6();
void Func_020041ec();
void Func_02004208();
void Func_02004230();
void Func_02004236();
void Func_02004266();
void Func_020042ae();
s32 Func_020042b8();
s32 Func_020042c0();
void Func_020042ca();
void Func_020042e0();
void Func_020042e8();
void Func_020042ec();
void Func_020042f6();
void Func_020042fc();
void Func_02004320();
void Func_02004322();
void Func_0200432a();
void Func_0200432c();
void Func_02004350();
void Func_02004386();
void Func_020043c2();
void Func_020043d8();
void Func_020043da();
void Func_020043de();
void Func_020043f4();
void Func_020043fa();
void Func_02004408();
void Func_0200440e();
void Func_02004410();
void Func_02004418();
void Func_0200441e();
void Func_0200443e();
void Func_02004464();
void Func_0200449a();
void Func_020044a4();
void Func_020044d6();
void Func_020044ec();
void Func_02004522();
void Func_02004528();
void Func_0200452c();
void Func_02004598();
void Func_0200463c();
void Func_020046ac();
void Func_02004322_a();
void Func_0200440e_a();
void Func_02004528_a();
void Func_02004534();
s32 Func_02004544();
void Func_02004558();
void Func_02004564();
void Func_02004586();
void Func_020045ae();
void Func_020045d0();
void Func_020045dc();
s32 Func_020045ec();
void Func_02004600();
void Func_0200460c();
void Func_0200462e();
void Func_02004656();
void Func_02004680(void);
void Func_02004660(s32, s32);
void Func_0200483e(s32);
void Func_020015b6(void);
void Func_020045a0(s32);
void Func_02004844(void);
void Func_020046a8(void);
void Func_02004636();
void Func_0200468a();
void Func_02004692();
void Func_02004696();
s32 Func_020046a6();
void Func_020046b2();
void Func_020046ba();
void Func_020046c6();
void Func_020046e8();
void Func_02004710();
void Func_0200473e();
void Func_02004870();
void Func_020048da();
void Func_020046d2();
void Func_02004726();
void Func_0200472e();
void Func_02004732();
s32 Func_02004742();
void Func_0200474e();
void Func_02004756();
void Func_02004762();
void Func_02004784();
void Func_020047ac();
void Func_020047da();
void Func_0200490c();
void Func_02004976();
void Func_02001122();
void Func_020047de();
void Func_02004812_a();
void Func_02004866();
s32 Func_0200486e();
void Func_02004870_a();
void Func_02004892();
void Func_020048b6();
void Func_020048c4();
void Func_020048d8();
void Func_020048ea();
void Func_02004902();
void Func_020049dc();
void Func_02004a68();
s32 Func_02004a6e();
s32 Func_02004a76();
s32 Func_02004a7e();
s32 Func_02004a86();
void Func_02004aac();
void Func_02004acc();
void Func_02004ae2();
void Func_02004b08();
void Func_02004b12();
void Func_02004b24();
void Func_02004b2e();
void Func_02004b38();
void Func_02004b48();
void Func_02004b48_a();
void Func_02004b4c();
void Func_02004b62();
void Func_02004b62_a();
void Func_02004b6a();
void Func_02004b84();
void Func_02004b90();
void Func_02004b9e();
void Func_02004ba4();
s32 Func_02004bbe();
s32 Func_02004bbe_a();
void Func_02004bfa();
void Func_02004c10();
void Func_02004c14();
u8 *Func_02004c1c();
void Func_02004c4a();
void Func_02004c56();
void Func_02004c5e();
void Func_02004c60();
void Func_02004c66();
void Func_02004c76();
void Func_02004c7c();
void Func_02004c84();
void Func_02004c92();
u8 *Func_02004c98();
void Func_02004c9a();
void Func_02004c9e();
void Func_02004cb2();
void Func_02004cba();
void Func_02004cd2();
void Func_02004cda();
void Func_02004cda_a();
void Func_02004cec();
void Func_02004d00();
void Func_02004d0c();
void Func_02004d0e();
void Func_02004d1e();
void Func_02004d20();
void Func_02004d26();
void Func_02004d28();
void Func_02004d2c();
void Func_02004d38();
void Func_02004d42();
void Func_02004d44();
s32 Func_02004d4c();
s32 Func_02004d4c_a();
void Func_02004d52();
void Func_02004d5e();
s32 Func_02004d86();
void Func_02004d9c();
void Func_02004da6();
void Func_02004dae();
void Func_02004dc4();
s32 Func_02004de6();
void Func_02004dea();
void Func_02004df8();
void Func_02004e08();
void Func_02004e30();
void Func_02004e32();
void Func_02004e60();
s32 Func_02004e74();
void Func_02004e9c();
void Func_02004e9e();
void Func_02004ea6();
void Func_02004eaa();
void Func_02004eae();
void Func_02004ec6();
void Func_02004ec6_a();
void Func_02004ed2();
void Func_02004ed4();
void Func_02004ee6();
void Func_02004f00();
void Func_02004f02();
void Func_02004f0c();
void Func_02004f1c();
void Func_02004f32();
void Func_02004f3e();
void Func_02004faa();
void Func_0200500a();
void Func_0200abe1();
void Func_020024fe();
s32 Func_02004eee();
s32 Func_02004efc();
s32 Func_02004f08();
void Func_02004f0c_a();
void Func_02004f1c_a();
void Func_02004f6e();
s32 Func_02004f8e();
void Func_02004fa2();
void Func_02004fae();
void Func_02004fb0();
void Func_02004fbc();
void Func_02004fda();
void Func_02004fe4();
void Func_02004fec();
void Func_02004ff2();
s32 Func_02004ff6();
void Func_02004ff8();
void Func_02005016();
s32 Func_0200501a();
void Func_02005024();
void Func_0200502a();
void Func_02005032();
s32 Func_02005040();
s32 Func_02005040_a();
void Func_0200504c();
void Func_0200504e();
void Func_0200504e_a();
void Func_0200505c();
u8 *Func_02005060();
void Func_02005068();
void Func_02005076();
void Func_0200508c();
void Func_02005094();
void Func_02005146();
void Func_0200516a();
void Func_02005170();
void Func_02005174();
void Func_0200517a();
void Func_02005182();
void Func_02005184();
void Func_02005190();
void Func_02005196();
void Func_02005198();
void Func_0200519c();
void Func_0200519e();
void Func_020051a0();
void Func_020051a6();
void Func_020051b0();
void Func_020051b2();
void Func_020051b2_a();
void Func_020051ba();
void Func_020051bc();
void Func_020051be();
void Func_020051c8();
void Func_020051d2();
void Func_020051d6();
void Func_020051da();
void Func_020051e0();
void Func_020051e8();
void Func_0200520a();
void Func_02005214();
void Func_02005218();
void Func_0200521a();
void Func_0200522c();
void Func_02005276();
void Func_02004f5c();
void Func_02005172();
void Func_02005d7c();
s32 Func_02005e68();
s32 Func_02005e8e();
void Func_02005e94();
void Func_02005ea8();
s32 Func_02005ed6();
void Func_02005ee6();
void Func_02005ef2();
s32 Func_02005f02();
void Func_02005f10();
void Func_02005f1a();
void Func_02005f1c();
void Func_02005f8a();
void Func_02005faa();
void Func_02005f7e();
s32 Func_02005f80();
void Func_02005fd2();
void Func_020050b4();
void Func_02005356();
void Func_0200596a();
void Func_02005f1a_a();
s32 Func_02005fe8();
s32 Func_0200600a();
s32 Func_0200602c();
void Func_0200605a();
s32 Func_02006062();
void Func_0200607c();
void Func_02006088();
s32 Func_02006098();
void Func_0200609a();
void Func_020060a6();
void Func_020060b2();
u8 *Func_020060d0();
void Func_020060e0();
void Func_02006100();
void Func_0200610e();
void Func_02005a50();
s32 Func_02006128();
s32 Func_02006148();
void Func_0200614a();
void Func_0200617a();
s32 Func_02006180();
void Func_02006186();
s32 Func_02006196();
u8 *Func_0200619a();
void Func_020061a4();
void Func_020061b0();
void Func_020061d2();
void Func_020061fc();
void Func_02002ade();
void Func_02005b38();
void Func_02005b74();
void Func_02005bb0();
void Func_020061fe();
void Func_02006212();
void Func_02006226();
s32 Func_02006230();
u8 *Func_02006260();
void Func_02006262();
s32 Func_0200626a();
u8 *Func_0200629c();
void Func_020062a0();
s32 Func_020062a8();
void Func_020062cc();
u8 *Func_020062d8();
u8 *Func_020062d8_a();
void Func_020062dc();
s32 Func_020062e8();
void Func_020062f6();
void Func_02006302();
u8 *Func_02006316();
u8 *Func_02006316_a();
void Func_02006354();
s32 Func_0200648a();
void Func_02006492();
void Func_020064a6();
s32 Func_020064ea();
s32 Func_020064f4();
s32 Func_02006508();
s32 Func_02006510();
s32 Func_02006514();
void Func_0200651a();
void Func_0200652c();
void Func_02006532();
s32 Func_0200653c();
s32 Func_0200653c_a();
void Func_020059f0();
void Func_020065c2();
void Func_02006518();
s32 Func_0200657a();
void Func_020065ee();
s32 Func_0200665a();
s32 Func_02006682();
s32 Func_0200668a();
s32 Func_020066b2();
s32 Func_020066d4();
s32 Func_020066ec();
s32 Func_02006724();
s32 Func_0200673c();
void Func_02006758();
void Func_02006768();
void Func_02006794();
void Func_02006950();
void Func_02006958();
void Func_020034b4();
void Func_020068d8();
s32 Func_020068fa();
void Func_02006900();
s32 Func_02006930();
void Func_0200693c_a();
void Func_0200695a();
void Func_02006964();
void Func_0200696e();
void Func_02006974();
void Func_02006a32_a();
s32 Func_02006a3c();
void Func_02006a3e();
void Func_02006a4a();
void Func_02006a4a_a();
void Func_02006a56();
void Func_02006a60();
void Func_02006a64();
void Func_02006a70();
void Func_0200364c();
void Func_0200382e();
void Func_020068c4();
void Func_02006936();
void Func_02006990();
void Func_02006996();
void Func_020069e8_a();
void Func_02006a12();
void Func_02006a20();
void Func_02006b0c();
void Func_02006b18();
void Func_020037a0();
void Func_0200393e();
void Func_0200692c();
void Func_0200699e();
void Func_020069f8_a();
void Func_020069fe();
void Func_02006a50();
void Func_02006a7a();
void Func_02006a88();
void Func_02006b74();
void Func_02006b80();
void Func_020069d4();
void Func_02006a84();
void Func_02006a9c();
void Func_02006ad8();
void Func_02006b06();
void Func_02006b0e();
void Func_02006b18_a();
void Func_02006b1c();
void Func_02006b4c();
void Func_02006b58();
void Func_02006b70();
void Func_02006b94();
void Func_02006b98();
void Func_02006bae();
void Func_02006bba();
void Func_02006c0a();
void Func_02006c6a();
void Func_02006c6a_a();
void Func_02006c72();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Byte at offset 0x22B of the shared work buffer. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02000c9c(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02002310(s32 (*f)(), s32 a0)
{
    extern u8 Data_02000240[];

    return f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02002490(s32 (*f)(), s32 a0)
{
    extern u8 Data_02000240[];

    return f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_020025c0(s32 (*f)(), s32 a0)
{
    extern u8 Data_02000240[];

    return f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_020026c0(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02002934(s32 (*f)(), s32 a0)
{
    extern s16 Data_02000240[];

    return f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_020029dc(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern s16 Data_02000240[];

    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

/* Loader-relocated ROM calls: each site names the pre-relocation call word the image holds. */
static __inline__ void Call0(void (*f)())
{
    extern u8 Data_02000240[];

    f();
}

static __inline__ s32 Value0_02002f10(s32 (*f)())
{
    extern u8 Data_02000240[];

    return f();
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_02000240[];

    return f(a0, a1, a2);
}

void Scene_RunEarlySequence(void)
{
    s32 i;
    s32 record;
    u8 *p5;
    u8 *rec;
    s32 v;

    p5 = *(u8 **)0x03001e70;
    Func_02004408(230);
    Call3(Func_02004230, 0x20000, 0x20000, 0x10000);
    Func_02004266(10);
    rec = p5 + 356;
    for (i = 0; i <= 23; i++) {
        *(volatile s32 *)(rec + 12) += -0x10000;
        Func_0200418e(4);
        if (i == 8) {
            record = Value1(Func_020042b8, 8);
            *(volatile s32 *)(record + 24) = 0x1999;
            record = Func_020042c0(8);
            *(volatile s32 *)(record + 28) = 0x1999;
            Call3(Func_02004320, 8, 0x980000, 0xd80000);
            Call2(Func_020042e8, 8, 0x200bd48);
        }
    }
    Func_02004208(1, 0, 0x20086a1);
    *(u16 *)Data_0200d238 = 0;
    do {
        Func_020041d6(1);
        v = *(u16 *)Data_0200d238 + 1;
        *(u16 *)Data_0200d238 = (u16)v;
    } while ((u32)(v << 16) <= 0x640000);
    Func_020041ec(1);
    Func_02004236(1, 0, 0);
    Call1(Func_020044a4, 0x121);
    Call3(Func_020042ec, -1, -1, 0xe666);
    Func_02004322(30);
    Call6(Func_020042f6, 0, 0, 1, 2, 3, 14);
    Call1(Func_0200432c, 0x8fd);
}

void Func_020007e8(void)
{
    s32 i;
    u8 *p5;
    u8 *rec;
    s32 v;

    p5 = *(u8 **)0x03001e70;
    Call6(Func_02004322_a, 93, 41, 16, 4, 77, 28);
    Func_02004528(230);
    Call3(Func_02004350, 0x20000, 0x20000, 0x10000);
    Func_02004386(10);
    rec = p5 + 356;
    for (i = 23; i >= 0; i--) {
        *(volatile s32 *)(rec + 12) += -0x10000;
        Func_020042ae(4);
    }
    Func_020042fc(1, 0, 0x20086a1);
    *(u16 *)Data_0200d238 = 0;
    do {
        Func_020042ca(1);
        v = *(u16 *)Data_0200d238 + 1;
        *(u16 *)Data_0200d238 = (u16)v;
    } while ((u32)(v << 16) <= 0x640000);
    Func_020042e0(1);
    Func_0200432a(1, 0, 0);
    Call1(Func_02004598, 0x121);
    Call3(Func_020043d8, -1, -1, 0xe666);
    Func_0200440e(30);
    Call6(Func_020043da, 77, 41, 16, 4, 77, 28);
    Call1(Func_02004418, 0x8fe);
}

void Func_020008d4(void)
{
    s32 i;
    u8 *p8;
    u8 *rec;
    s32 v;

    p8 = *(u8 **)0x03001e70;
    Call6(Func_020043fa, 113, 31, 103, 17, 1, 1);
    Call6(Func_0200440e_a, 111, 32, 104, 18, 3, 2);
    Call6(Func_0200441e, 64, 32, 103, 18, 1, 2);
    Func_0200463c(230);
    Call3(Func_02004464, 0x20000, 0x20000, 0x10000);
    Func_0200449a(10);
    rec = p8 + 356;
    for (i = 23; i >= 0; i--) {
        *(volatile s32 *)(rec + 12) += -0x10000;
        Func_020043c2(4);
    }
    Func_02004410(1, 0, 0x20086a1);
    *(u16 *)Data_0200d238 = 0;
    do {
        Func_020043de(1);
        v = *(u16 *)Data_0200d238 + 1;
        *(u16 *)Data_0200d238 = (u16)v;
    } while ((u32)(v << 16) <= 0x640000);
    Func_020043f4(1);
    Func_0200443e(1, 0, 0);
    Call1(Func_020046ac, 0x121);
    Call3(Func_020044ec, -1, -1, 0xe666);
    Func_02004522(30);
    Call6(Func_020044d6, 103, 14, 103, 17, 4, 3);
    Call1(Func_0200452c, 0x907);
}

void Scene_RunScene3a4SequenceB(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_02004544, 0x323) != 0) {
        Call6(Func_02004534, 2, 0, 1, 1, 24, 80);
        Call6(Func_02004528_a, 2, 1, 24, 11, 1, 2);
        Call1(Func_02004586, 0x323);
    } else {
        Call6(Func_02004564, 0, 0, 1, 1, 24, 80);
        Call6(Func_02004558, 0, 1, 24, 11, 1, 2);
        Call1(Func_020045ae, 0x323);
    }
}

void Scene_RunScene3a4SequenceA(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_020045ec, 0x325) != 0) {
        Call6(Func_020045dc, 12, 72, 1, 1, 11, 73);
        Call6(Func_020045d0, 48, 32, 11, 4, 1, 2);
        Call1(Func_0200462e, 0x325);
    } else {
        Call6(Func_0200460c, 10, 72, 1, 1, 11, 73);
        Call6(Func_02004600, 49, 32, 11, 4, 1, 2);
        Call1(Func_02004656, 0x325);
    }
}

void Scene_RunLine1528Sequence(void)
{
    Func_02004680();
    Func_02004660(0x1528, 1);
    Func_0200483e(125);
    Func_020015b6();
    Func_020045a0(20);
    Func_02004844();
    Func_020046a8();
}

void Scene_RunScene3a4SequenceC(void)
{
    u32 i;
    s32 record;

    Func_020046b2();
    Call2(Func_02004692, 0x1528, 1);
    Func_02004870(125);
    if (Value1(Func_020046a6, 0x326) != 0) {
        Call6(Func_02004696, 15, 93, 1, 1, 16, 92);
        Call6(Func_0200468a, 47, 29, 16, 28, 1, 2);
        Call1(Func_020046e8, 0x326);
    } else {
        Call6(Func_020046c6, 17, 93, 1, 1, 16, 92);
        Call6(Func_020046ba, 46, 29, 16, 28, 1, 2);
        Call1(Func_02004710, 0x326);
    }
    Func_02004636(20);
    Func_020048da();
    Func_0200473e();
}

void Scene_RunScene3a4SequenceD(void)
{
    u32 i;
    s32 record;

    Func_0200474e();
    Call2(Func_0200472e, 0x1528, 1);
    Func_0200490c(125);
    if (Value1(Func_02004742, 0x327) != 0) {
        Call6(Func_02004732, 28, 82, 1, 1, 29, 81);
        Call6(Func_02004726, 47, 28, 29, 17, 1, 2);
        Call1(Func_02004784, 0x327);
    } else {
        Call6(Func_02004762, 30, 82, 1, 1, 29, 81);
        Call6(Func_02004756, 46, 28, 29, 17, 1, 2);
        Call1(Func_020047ac, 0x327);
    }
    Func_020046d2(20);
    Func_02004976();
    Func_020047da();
}

void FieldScene_RunScene3a4_02000c9c(void)
{
    u32 i;
    s32 record;

    Func_02004812_a();
    Call6(Func_020047de, 24, 27, 2, 1, 24, 26);
    Func_020049dc(185);
    Call3_02000c9c(Func_02004866, 10, 0x3333, 0x1999);
    Call3_02000c9c(Func_02004870_a, 0, 0x3333, 0x1999);
    *(u8 *)(Func_0200486e(10) + 90) &= 254;
    Func_020048d8(0, 8);
    Call3_02000c9c(Func_020048b6, 0, 0x190, 0x1a8);
    Call3_02000c9c(Func_020048c4, 10, 0x198, 0x1a8);
    Func_020048ea(10);
    Func_02004902(0, 1);
    Func_02001122();
    Func_02004892();
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
    rec2 = Value1(Func_02004a6e, 0);
    rec = Value1(Func_02004a76, 8);
    rec4 = Value1(Func_02004a7e, 9);
    rec8 = Func_02004a86(10);
    Call2(Func_02004b62, 0, 0x102);
    Func_02004a68(40);
    Call2(Func_02004b84, 0x10000, 0x2000);
    Call4(Func_02004b9e, 0x3100000, -1, 0x740000, 1);
    Call3(Func_02004acc, 0, 0x20000, 0x10000);
    Func_02004b24(0, 6);
    Call3(Func_02004b08, 0, 0x318, 140);
    Func_02004b38(0, 1);
    Call3(Func_02004ba4, 0, 0xc000, 100);
    Call3(Func_02004bbe, 0, 0x101, 60);
    Func_02004c84(183);
    Call3(Func_02004aac, 0x30000, 0x30000, 0x10000);
    Func_02004ae2(20);
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
    Func_02004b90(8, 4);
    *(volatile s32 *)(rec + 68) = 0x8000;
    *(volatile s32 *)(rec + 8) = 0x3120000;
    *(volatile s32 *)(rec + 12) = 0x200000;
    *(volatile s32 *)(rec + 16) = 0x5a0000;
    *(volatile s32 *)(rec + 24) = 0x20000;
    *(volatile s32 *)(rec + 28) = 0x20000;
    Func_02004b2e(10);
    Func_02004cec(183);
    Call3(Func_02004b12, 0x40000, 0x20000, 0x10000);
    Func_02004b48(20);
    *(s32 *)(rec8 + 8) += 0xe0000;
    *(s32 *)(rec8 + 12) += -0x80000;
    {
        s32 target = *(s32 *)(rec8 + 80);
        s32 shown = 0xc000;

        *(volatile u16 *)(target + 30) = shown;
    }
    Func_02004d20(107);
    Call3(Func_02004b48_a, 0x10000, 0x10000, 0x10000);
    Call3(Func_02004c7c, 0, 0x102, 80);
    Func_02004d42(55);
    Call3(Func_02004b6a, 0x10000, 0x30000, 0x10000);
    Func_02004c92(8, 0);
    Func_02004c9a(0, 0);
    Call2(Func_02004cb2, 0, 0x101);
    Call3(Func_02004bfa, 0, 0x28000, 0x14000);
    *(volatile u16 *)(rec2 + 100) = none;
    Call2(Func_02004c10, 0, 0x200bdec);
    if (Value1(Func_02004bbe_a, 0x205) != 0) {
        Call3(Func_02004c66, 1, 0x36e0000, 0x2100000);
        record = Func_02004c1c(1);
        {
            s32 shown = 0x5000;

            *(u16 *)((s32)record + 6) = shown;
        }
    }
    Call2(Func_02004d0e, 0x14000, 0x2800);
    Call4(Func_02004d26, 0x3120000, -1, 0x22c0000, 1);
    Func_02004c14(slot8);
    Func_02004d0c(8, 1);
    Call3(Func_02004c60, 8, 0x195c2, 0xcae1);
    *(volatile u16 *)(rec + 100) = none;
    Func_02004c76(8, 0x200bd78);
    do {
        Func_02004b4c(1);
    } while (*(s16 *)(rec2 + 100) == 0);
    ((void (*)())Func_02004d4c)(0, 0);
    do {
        Func_02004b62_a(1);
    } while (*(s16 *)(rec + 100) == 0);
    Func_02004d52(0, 2);
    {
        u8 *record = Func_02004c98(0);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | 1);
    }
    Call1(Func_02004e30, 0x121);
    zero = 0;
    Call3(Func_02004c56, -1, -1, 0xe666);
    *(s32 *)((s32)rec4 + 8) = 0x3120000;
    base6_ffc00000 = -0x400000;
    *(s32 *)((s32)rec4 + 108) = zero;
    *(s32 *)((s32)rec4 + 16) = 0x26a0000;
    *(s32 *)((s32)rec4 + 12) = -0x400000;
    Call3(Func_02004cda, 8, 0x19999, 0xcccc);
    *(volatile s32 *)(rec + 68) = 0x1999;
    *(volatile s32 *)(rec + 72) = 0x3333;
    *(volatile s32 *)(rec + 40) = 0x40000;
    Call3(Func_02004d1e, 8, 0x312, 0x25c);
    Call3(Func_02004d00, 8, 0x33333, 0x19999);
    Call3(Func_02004d2c, 8, 0x312, 0x284);
    Func_02004cda_a(15);
    Call3(Func_02004cba, 0x50000, 0x70000, 0x10000);
    Call6(Func_02004c9e, 25, 36, 43, 36, 11, 9);
    Call6(Func_02004cd2, 25, 35, 10, 5, 43, 35);
    Func_02004d9c(8, 0, 0);
    Func_02004da6(9, 0, 0);
    base5_200abe1 = (s32)Func_0200abe1;
    Call2(Func_02004c4a, base5_200abe1, 0xc80);
    Func_02004d38(80);
    Func_02004c5e(base5_200abe1);
    Func_02004d44(60);
    Func_02004f02(17);
    Call3(Func_02004d28, -1, -1, 0xe666);
    Func_02004d5e(120);
    if (Value1(Func_02004d4c_a, 0x205) != 0) {
        Call3(Func_02004dae, 1, 0x10000, 0x8000);
        Call3(Func_02004dea, 1, 0x338, 0x22e);
    }
    Call3(Func_02004dc4, 0, 0x9999, 0x4ccc);
    Call3(Func_02004e08, 0, 0x356, 0x248);
    if (Value1(Func_02004d86, 0x205) != 0) {
        Func_02004e32(1, 1);
        Call3(Func_02004e9e, 1, 0x4000, 0);
    }
    Call3(Func_02004eaa, 0, 0x6000, 40);
    Call3(Func_02004ec6, 1, 0x102, 0);
    Call3(Func_02004ed2, 0, 0x102, 60);
    Call4(Func_02004f00, 0x3140000, base6_ffc00000, 0x2620000, 1);
    Func_02004f0c();
    Func_02004faa(148);
    Func_02004df8(240);
    if (Value1(Func_02004de6, 0x205) != 0) {
        Call2(Func_02004f1c, 0x40000, 0x8000);
        Call4(Func_02004f32, 0x3560000, 0, 0x2480000, 1);
        Func_02004f3e();
        Call3(Func_02004e9c, 1, 0x348, 0x228);
        Call3(Func_02004ea6, 1, 0x356, 0x232);
        Func_02004ec6_a(1, 2);
        record = Value1(Func_02004e74, 0);
        if ((s32)record != 0) {
            Func_02004eae(1, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
        }
        Func_02004ed4(1);
        Func_02004ee6(1, 0, 0);
    }
    Func_0200500a();
    Call1(Func_02004e60, 0x908);
}

void FieldScene_RunScene3a4_02001398(void)
{
    u32 i;
    u8 *record;
    s32 base5_1953;

    Func_02004f0c_a();
    base5_1953 = (s32)Data_00001953;
    Value2(Func_02004eee, base5_1953, 1);
    if (Value1(Func_02004efc, 0x908) != 0) {
    } else {
        if (Value1(Func_02004f08, 0xf14) != 0) {
        } else {
            Call1(Func_02004f1c_a, 0x205);
            Call3(Func_02004f6e, 0, 0xcccc, 0x6666);
            Call3(Func_02004fb0, 0, 0x316, 140);
            Call3(Func_02004fbc, 0, 0x30c, 140);
            Call3(Func_02005040, 0, 0xc000, 0);
            record = Value1(Func_02004f8e, 0);
            if ((s32)record != 0) {
                Func_02004fec(1, *(volatile s32 *)((s32)record + 8), *(volatile s32 *)((s32)record + 16));
            }
            Call3(Func_02004fae, 1, 0xcccc, 0x6666);
            Call3(Func_02004ff2, 1, 0x320, 140);
            Call3(Func_02005076, 1, 0xc000, 20);
            Func_0200505c((base5_1953 + 1));
            Func_02005024(1, 4);
            Func_02004fa2(20);
            Func_0200508c(1, 0, 10);
            Func_0200504e(1, 6, 0);
            Call3(Func_02004ff8, 1, 0x19999, 0xcccc);
            *(u8 *)(Func_02004ff6(1) + 90) &= 254;
            Call3(Func_0200504e_a, 1, 0x318, 110);
            Func_02004fe4(1);
            *(u8 *)(Func_0200501a(1) + 90) |= 1;
            Func_020051b2(161);
            Call3(Func_02004fda, 0x20000, 0x10000, 0x10000);
            *(u8 *)(Func_02005040_a(1) + 90) &= 254;
            Call3(Func_02005094, 1, 0x318, 120);
            Func_0200502a(1);
            {
                u8 *record = Func_02005060(1);
                u8 value = *(volatile u8 *)&record[90];

                record[90] = (u8)(value | 1);
            }
            Call3(Func_02005016, -1, -1, 0xe666);
            Func_0200504c(80);
            Func_0200520a(141);
            Call3(Func_02005032, 0x10000, 0x10000, 0x10000);
            Func_02005068(40);
            Call3(Func_0200516a, 0, 0x101, 0);
            Call3(Func_02005174, 1, 0x101, 60);
            Call3(Func_02005170, 0, 0x8000, 0);
            Func_0200517a(1, 0, 20);
            Func_02005184(0, 0, 0);
            Call3(Func_02005190, 1, 0x8000, 40);
            Call3(Func_0200519c, 0, 0x8000, 0);
            Func_020051a6(1, 0, 40);
            Call3(Func_020051b2_a, 0, 0xc000, 0);
            Call3(Func_020051be, 1, 0xc000, 40);
            Call3(Func_020051da, 1, 0x102, 60);
            Call3(Func_020051d6, 1, 0x4000, 20);
            Func_02005196(1, 2);
            Func_020051e0(1, 0, 10);
            Call3(Func_02005146, 1, 0x28000, 0x14000);
            Func_0200519e(1, 5);
            Call3(Func_02005182, 1, 0x31c, 138);
            Func_02005214(0, 0, 0);
            Call3(Func_02005198, 1, 0x324, 140);
            Call3(Func_0200522c, 0, 0x4000, 0);
            Call3(Func_020051b0, 1, 0x324, 166);
            Call3(Func_020051bc, 1, 0x2fc, 166);
            Call3(Func_020051c8, 1, 0x2fc, 198);
            Call3(Func_020051d2, 1, 0x312, 198);
            Call3(Func_02005276, 0, 0x102, 0);
            Call3(Func_020051e8, 1, 0x312, 246);
            Func_02005218(1, 1);
            Func_0200521a(1, 0, 0);
            Func_020051a0(40);
            Func_020024fe(10);
        }
    }
    Func_020051ba();
}

void FieldScene_RunScene3a4_02002310(void)
{
    extern u8 Data_02000240[];

    u32 i;
    s32 record;

    if (Value1_02002310(Func_02005e68, 0x8fe) != 0) {
        *(volatile u16 *)(*(volatile s32 *)0x03001e70 + 20) &= 0xfdff;
        Func_02005f1a(9, 0, 0);
    } else {
        Func_02005d7c();
        if (Value1_02002310(Func_02005e8e, 0x109) == 0) {
            if (Data_02000240_t[225][0] != 99) {
                goto L_0200235a;
            }
            Func_02005172();
        } else {
            L_0200235a:;
            Call6(Func_02005e94, 38, 24, 1, 2, 37, 24);
            Call6(Func_02005ea8, 44, 23, 1, 2, 45, 23);
            if (Value1_02002310(Func_02005ed6, 0x8fe) == 0) {
                Func_02005faa(9, 2);
                Func_02005f8a(9, 3);
                Func_02004f5c(0xee0000, 0, 0x1a20000, 0x8000);
            }
        }
    }
    if (Value1_02002310(Func_02005f02, 0x323) != 0) {
        Call6(Func_02005ef2, 0, 0, 1, 1, 24, 80);
        Call6(Func_02005ee6, 0, 1, 24, 11, 1, 2);
    } else {
        Call6(Func_02005f1c, 2, 0, 1, 1, 24, 80);
        Call6(Func_02005f10, 2, 1, 24, 11, 1, 2);
    }
}

void FieldScene_RunScene3a4_02002428(void)
{
    extern u8 Data_02000240[];

    if (Value1(Func_02005f80, 0x8fe) != 0) {
        *(volatile u16 *)(*(volatile s32 *)0x03001e70 + 20) &= 0xfdff;
    } else {
        Call6(Func_02005f7e, 52, 42, 1, 1, 53, 42);
    }
    {
        s32 index = 225;
        if ((u32)((((u16 *)Data_02000240)[index] - 6) << 16) <= 0x10000) {
            Call1(Func_02005fd2, 0x12f);
        }
    }
}

void FieldScene_RunScene3a4_02002490(void)
{
    extern u8 Data_02000240[];

    u32 i;
    u8 *record;

    if (Value1_02002490(Func_02005fe8, 0x907) != 0) {
        *(volatile u16 *)(*(volatile s32 *)0x03001e70 + 20) &= 0xfdff;
        Func_0200609a(10, 0, 0);
    } else {
        if (Value1_02002490(Func_0200600a, 0x109) == 0) {
            if (Data_02000240_t[225][0] == 99) {
                Func_02005356();
            }
        }
        Func_02005f1a_a();
        if (Value1_02002490(Func_0200602c, 0x907) == 0) {
            Func_02006100(10, 2);
            Func_020060e0(10, 3);
            Call4(Func_020050b4, 0x2ec0000, 0x80000, 0x1180000, 0x8000);
        }
    }
    Func_0200596a(9);
    if (Value1_02002490(Func_02006062, 0x200) != 0) {
        Func_0200610e(9, 5);
        Call6(Func_0200605a, 23, 13, 1, 1, 25, 13);
        {
            u8 *record = Func_020060d0(9);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | 2);
        }
    }
    if (Value1_02002490(Func_02006098, 0x325) != 0) {
        Call6(Func_02006088, 10, 72, 1, 1, 11, 73);
        Call6(Func_0200607c, 49, 32, 11, 4, 1, 2);
    } else {
        Call6(Func_020060b2, 12, 72, 1, 1, 11, 73);
        Call6(Func_020060a6, 48, 32, 11, 4, 1, 2);
    }
}

void FieldScene_RunScene3a4_020025c0(void)
{
    extern u8 Data_02000240[];

    u32 i;
    u8 *rec7;
    s32 record;

    if (Data_02000240_t[225][0] == 2) {
        if (Value1_020025c0(Func_02006128, 0x109) == 0) {
            Call3(Func_020061d2, 8, 0x1660000, 0x680000);
        }
    }
    Func_02005a50(9);
    if (Value1_020025c0(Func_02006148, 0x200) != 0) {
        rec7 = Func_0200619a(9);
        Func_020061fc(9, 5);
        Call6(Func_0200614a, 45, 41, 1, 1, 43, 41);
        {
            u8 value = *(volatile u8 *)&rec7[35];

            rec7[35] = (u8)(value | 2);
        }
    }
    if (Value1_020025c0(Func_02006180, 0x907) != 0) {
        *(volatile u16 *)(*(volatile s32 *)0x03001e70 + 20) &= 0xfdff;
    }
    if (Value1_020025c0(Func_02006196, 0x326) != 0) {
        Call6(Func_02006186, 17, 93, 1, 1, 16, 92);
        Call6(Func_0200617a, 46, 29, 16, 28, 1, 2);
    } else {
        Call6(Func_020061b0, 15, 93, 1, 1, 16, 92);
        Call6(Func_020061a4, 47, 29, 16, 28, 1, 2);
    }
}

void FieldScene_RunScene3a4_020026c0(void)
{
    u32 i;
    u8 *record;

    record = Func_02006260(9);
    Func_020061fe((s32)record, 0);
    Func_02002ade();
    Func_02005b38(9);
    if (Value1_020026c0(Func_02006230, 0x200) != 0) {
        Func_020062dc(9, 5);
        Call6(Func_02006226, 0, 0, 1, 1, 26, 26);
        {
            u8 *record = Func_0200629c(9);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | 2);
        }
    }
    Func_02005b74(11);
    if (Value1_020026c0(Func_0200626a, 0x201) != 0) {
        Call2((void (*)())Func_02006316, 11, 5);
        Call6(Func_02006262, 1, 0, 1, 1, 17, 10);
        {
            u8 *record = Func_020062d8(11);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | 2);
        }
    }
    Func_02005bb0(12);
    if (Value1_020026c0(Func_020062a8, 0x204) != 0) {
        Func_02006354(12, 5);
        Call6(Func_020062a0, 1, 0, 1, 1, 26, 15);
        {
            u8 *record = Func_02006316_a(12);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | 2);
        }
    }
    Call2(Func_02006212, 0x200b429, 0xc80);
    if (Value1_020026c0(Func_020062e8, 0x327) != 0) {
        Call6((void (*)())Func_020062d8_a, 30, 82, 1, 1, 29, 81);
        Call6(Func_020062cc, 46, 28, 29, 17, 1, 2);
    } else {
        Call6(Func_02006302, 28, 82, 1, 1, 29, 81);
        Call6(Func_020062f6, 47, 28, 29, 17, 1, 2);
    }
}

void FieldScene_RunScene3a4_02002934(void)
{
    extern s16 Data_02000240[];

    s32 rec7;
    s32 record;
    s16 flag;

    rec7 = Value1_02002934(Func_0200648a, 0x909);
    if (rec7 != 0) {
        Func_02006532(8, 0, 0);
        ((void (*)())Func_0200653c)(9, 0, 0);
    } else {
        record = Func_020064f4(8);
        Func_02006492(record, 0);
        Func_020065c2(9, 3);
        record = Func_02006508(9);
        Func_020064a6(record, 0);
        *(u8 *)(Func_02006514(9) + 89) = rec7;
    }
    flag = Data_02000240[225];
    if (flag == 1 || flag == 98) {
        if (Value1_02002934(Func_020064ea, 0x109) == 0) {
            rec7 = Value1_02002934(Func_0200653c_a, 0);
            Func_0200651a();
            *(volatile s32 *)(rec7 + 12) = 0x100000;
            Func_0200652c();
        }
    } else if (flag == 99) {
        if (Value1_02002934(Func_02006510, 0x109) == 0) {
            Func_020059f0();
        }
    }
    /* unlifted: 0x020029bc..0x020029c2 (2) */
}

void FieldScene_RunScene3a4_020029dc(void)
{
    extern s16 Data_02000240[];

    s32 record;

    record = Func_0200657a(9);
    Func_02006518(record, 0);
    if (Data_02000240[225] == 2) {
        Call3_020029dc(Func_020065ee, 9, 0xb80000, 0x1480000);
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
    if (Value2(Func_0200665a, *(volatile s32 *)base6_3001e40, 3) == 0) {
        value = Value0(Func_02006682);
        rec7 = Value4(Func_020066ec, 200, ((((u32)(((value << 1) + value) << 4) >> 16) << 16) + 0x2fd0000), -0x400000, 0x2600000);
        if ((s32)rec7 != 0) {
            if (Value2(Func_0200668a, *(volatile s32 *)base6_3001e40, 9) == 0) {
                {
                    s32 v2 = Func_020066b2();
                    if (((u32)(v2 << 1) >> 16) != 0) {
                        Func_02006950(145);
                    } else {
                        Func_02006958(144);
                    }
                }
            }
            rec7[85] = 0;
            {
                s32 v3 = Func_020066d4();
                s32 tmp2824 = (((u32)(v3 << 15) >> 16) + 0x4ccc);
                *(volatile s32 *)((s32)rec7 + 72) = 0x6666;
                *(s32 *)((s32)rec7 + 28) = tmp2824;
                *(s32 *)((s32)rec7 + 24) = tmp2824;
            }
            rec7[97] = 1;
            Func_02006794((s32)rec7, 0);
            rec7[35] &= 254;
            {
                u8 *p80 = (u8 *)(*(volatile s32 *)((s32)rec7 + 80));
                s32 mask9 = -13;
                p80[9] = (mask9 & p80[9]) | 4;
            }
            Func_02006758((s32)rec7, 1);
            Call2(Func_02006768, (s32)rec7, 0x200c01c);
            value = Value0(Func_02006724);
            *(volatile s32 *)((s32)rec7 + 36) = ((((u32)(((value << 1) + value) << 1) >> 16) - 3) << 16);
            *(volatile s32 *)((s32)rec7 + 40) = 0x80000;
            value = Value0(Func_0200673c);
            *(volatile s32 *)((s32)rec7 + 44) = (((u32)(((value << 1) + value) << 9) >> 16) + -0x300);
        }
    }
}

void Scene_RunOpeningAuxiliarySequence(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_020068fa, 0);
    Func_020068d8();
    Func_0200695a(10, 0, 0);
    Func_02006964(8, 0, 0);
    *(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0) = 0x201;
    Func_02006a3e();
    Func_02006a4a();
    Func_02006900(20);
    Func_02006a60(202, 3);
    Value2(Func_02006930, 202, 0);
    *(u8 *)(Func_02006a3c() + 85) = 0;
    Call2(Func_02006a32_a, 0x19999, 0x3333);
    Call4(Func_02006a4a_a, 0x640000, 0, 0xf90000, 1);
    Func_02006a56();
    Func_0200693c_a(20);
    Func_020034b4();
    Func_02006a64(*(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12), *(s32 *)(rec7 + 16), 1);
    Func_02006a70();
    Call1(Func_0200696e, 0x200cd6c);
    *(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0) = 0x204;
    Func_02006974();
}

void Scene_RunScene3a4SequenceF(void)
{
    u32 i;
    s32 record;
    s32 v6;

    Func_02006990();
    Func_02006a12(9, 0, 0);
    Call3(Func_02006a20, 8, 0x1480000, 0x1a80000);
    Call1(Func_02006996, 0x323);
    Func_0200382e();
    Func_02006936();
    v6 = 224;
    Func_020068c4(1);
    *(volatile s32 *)(*(volatile s32 *)Data_03001ebc + (v6 << 1)) = 0x201;
    Func_02006b0c();
    Func_02006b18();
    Func_0200364c();
    *(volatile s32 *)(*(volatile s32 *)Data_03001ebc + (v6 << 1)) = 0x204;
    Func_020069e8_a();
}

void Scene_RunScene3a4SequenceE(void)
{
    u32 i;
    s32 record;
    s32 v6;

    Func_020069f8_a();
    Func_02006a7a(10, 0, 0);
    Call3(Func_02006a88, 8, 0x1e80000, 0x8a0000);
    Call1(Func_020069fe, 0x325);
    Func_0200393e();
    Func_0200699e();
    v6 = 224;
    Func_0200692c(1);
    *(volatile s32 *)(*(volatile s32 *)Data_03001ebc + (v6 << 1)) = 0x201;
    Func_02006b74();
    Func_02006b80();
    Func_020037a0();
    *(volatile s32 *)(*(volatile s32 *)Data_03001ebc + (v6 << 1)) = 0x204;
    Func_02006a50();
}

/* Runs a fixed chain of 19 calls with literal arguments, sets byte 0x22B of
 * the shared work buffer to 3, then issues two more calls. */
void Scene_RunLateAuxiliarySequence(void)
{
    extern u8 Data_02000240[];

    Battle_Reset_1();
    Motion_SetSpeedLim_1(39321, 4915);
    Motion_CamBounds_1(21495808, -1, 5701632, 1);
    Motion_SetSpeed_1(0, 39321, 19660);
    Motion_SetPosReset_1(0, 328, 116);
    Audio_PlayCue_1(148);
    Value2(Func_020069d4, 33599213, 3200); /* main:080000d0 */
    Call3(Func_02006a9c, 65536, 65536, 65536); /* main:080091f0 */
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
#undef Battle_Reset_1
#undef Motion_SetSpeedLim_1
#undef Motion_CamBounds_1
#undef Motion_SetSpeed_1
#undef Motion_SetPosReset_1
#undef Audio_PlayCue_1
#undef Motion_SetSpeed_2
#undef Motion_SetSpeed_3
#undef Object_SetModeById_1
#undef Motion_ResetAndSetPosition_1
#undef Motion_ResetAndSetPosition_2
#undef Battle_WaitMode0_1
#undef BattleFx_SpawnLinked_1
#undef Motion_SetVarCb_1
#undef Motion_CommitPos_1
#undef SharedWorkData_SetFirstAndSecondFields_1
#undef BattleFx_ComputeWeightedResultAndDispatch_1
#undef WORK_BYTE_22B
#undef Scene_RunEarlySequence
#undef Scene_RunScene3a4SequenceB
#undef Scene_RunScene3a4SequenceA
#undef Scene_RunLine1528Sequence
#undef Scene_RunScene3a4SequenceC
#undef Scene_RunScene3a4SequenceD
#undef FieldScene_RunScene3a4_02000c9c
#undef Scene_RunSharedSetPiece
#undef FieldScene_RunScene3a4_02001398
#undef FieldScene_RunScene3a4_02002310
#undef FieldScene_RunScene3a4_02002428
#undef FieldScene_RunScene3a4_02002490
#undef FieldScene_RunScene3a4_020025c0
#undef FieldScene_RunScene3a4_020026c0
#undef FieldScene_RunScene3a4_02002934
#undef FieldScene_RunScene3a4_020029dc
#undef Scene_RunScene3a4SequenceG
#undef Scene_RunOpeningAuxiliarySequence
#undef Scene_RunScene3a4SequenceF
#undef Scene_RunScene3a4SequenceE
#undef Scene_RunLateAuxiliarySequence

/* overlays/scene/story/set_piece/overlay_object.c */
#define OvObj_IntegrateAndDamp Func_020000bc
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
#undef OvObj_IntegrateAndDamp

/* overlays/scene/story/set_piece/scene_audio.c */
#define SceneAudio_PlayCue183EverySixtyTicks Func_02002eec
#define State_SetRecordWord102AndPlayCue288 Func_02003724
extern s32 Data_0200d1b0;   /* In-image writable data at image offset 0x51b0. */

void Func_02006c1e();
void Func_02007452();

/*
 * Overlay resource_3a4. Per-frame tick that counts to sixty, fires one
 * audio cue and rewinds itself. The address of this routine is stored in
 * a record as a plain word, so it runs as a published callback.
 */

/* Plays a sound cue; the name is this site's own call word, not a runtime
 * address. */

/*
 * The owner spans the code, one alignment halfword and its one pool word,
 * thirty-six bytes in all. Func_02006c1e names the loader-relocated call
 * word for the cue call, not a runtime address. The limit of sixty reads
 * as one second of frames, but nothing here fixes a frame rate.
 */
void SceneAudio_PlayCue183EverySixtyTicks(void)
{
    Data_0200d1b0 = Data_0200d1b0 + 1;
    if (Data_0200d1b0 == 60) {
        Func_02006c1e(183);
        Data_0200d1b0 = 0;
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
    Func_02007452(288);
}
#undef SceneAudio_PlayCue183EverySixtyTicks
#undef State_SetRecordWord102AndPlayCue288

/* overlays/scene/story/set_piece/scene_data.c */
#define SceneData_SelectTableByWord224 Func_020000e4
#define SceneData_SelectTableC80cOrC83c Func_020001c8
#define SceneData_GetTableC85c Func_02000204
#define SceneData_SelectDataByRuntimeSelector Func_0200020c
extern s16 Data_02000240[];
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
extern u8 Data_0200c194[];
extern u8 Data_0200c20c[];
extern u8 Data_0200c26c[];
extern u8 Data_0200c314[];
extern u8 Data_0200c3ec[];
extern u8 Data_0200c464[];
extern u8 Data_0200c524[];
extern u8 Data_0200c59c[];
extern u8 Data_0200c644[];
extern u8 Data_0200c704[];
extern u8 Data_0200c77c[];
extern u8 Data_0200c164[];
extern u8 Data_0200c80c[];
extern u8 Data_0200c83c[];
extern u8 Data_0200c85c[];   /* Image offset 0x485c. */
extern u8 Data_0200c940[];
extern u8 Data_0200c9a0[];
extern u8 Data_0200ca00[];
extern u8 Data_0200ca60[];
extern u8 Data_0200caa8[];
extern u8 Data_0200cb68[];
extern u8 Data_0200cb98[];
extern u8 Data_0200cc40[];
extern u8 Data_0200ccd0[];
extern u8 Data_0200c928[];

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
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_0000004d) {
        return (s32)Data_0200c194;
    }
    if (v == (s32)&Value_0000004e) {
        return (s32)Data_0200c20c;
    }
    if (v == (s32)&Value_0000004f) {
        return (s32)Data_0200c26c;
    }
    if (v == (s32)&Value_00000050) {
        return (s32)Data_0200c314;
    }
    if (v == (s32)&Value_00000051) {
        return (s32)Data_0200c3ec;
    }
    if (v == (s32)&Value_00000052) {
        return (s32)Data_0200c464;
    }
    if (v == (s32)&Value_00000053) {
        return (s32)Data_0200c524;
    }
    if (v == (s32)&Value_00000054) {
        return (s32)Data_0200c59c;
    }
    if (v == (s32)&Value_00000055) {
        return (s32)Data_0200c644;
    }
    if (v == (s32)&Value_00000056) {
        return (s32)Data_0200c704;
    }
    if (v == (s32)&Value_00000057) {
        return (s32)Data_0200c77c;
    }
    return (s32)Data_0200c164;
}

s32 SceneData_SelectTableC80cOrC83c(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000055) {
        return (s32)Data_0200c80c;
    }
    if (v == (s32)&Value_00000056) {
        return (s32)Data_0200c83c;
    }
    return 0;
}

u8 *SceneData_GetTableC85c(void)
{
    return Data_0200c85c;
}

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_0000004d) {
        return (s32)Data_0200c940;
    }
    if (v == (s32)&Value_0000004f) {
        return (s32)Data_0200c9a0;
    }
    if (v == (s32)&Value_00000051) {
        return (s32)Data_0200ca00;
    }
    if (v == (s32)&Value_00000052) {
        return (s32)Data_0200ca60;
    }
    if (v == (s32)&Value_00000053) {
        return (s32)Data_0200caa8;
    }
    if (v == (s32)&Value_00000054) {
        return (s32)Data_0200cb68;
    }
    if (v == (s32)&Value_00000055) {
        return (s32)Data_0200cb98;
    }
    if (v == (s32)&Value_00000056) {
        return (s32)Data_0200cc40;
    }
    if (v == (s32)&Value_00000057) {
        return (s32)Data_0200ccd0;
    }
    return (s32)Data_0200c928;
}
#undef SceneData_SelectTableByWord224
#undef SceneData_SelectTableC80cOrC83c
#undef SceneData_GetTableC85c
#undef SceneData_SelectDataByRuntimeSelector

/* overlays/scene/story/set_piece/scene_primary_script.c */
#define Battle_Reset_1(args...) Func_02006b9c(args)
#define Motion_SetHPosTerrain_1(a0, a1, a2) Call3(Func_02006c22, a0, a1, a2)
#define Motion_SetHPosTerrain_2(a0, a1, a2) Call3(Func_02006c30, a0, a1, a2)
#define Object_SetModeById_1(args...) Func_02006c40(args)
#define Battle_WaitMode0_1(args...) Func_02006d1a(args)
#define Motion_SetSpeed_1(args...) Func_02006d26(args)
#define Battle_WaitMode0_2(args...) Func_02006bdc(args)
#define Scene_GetRecord_1(a0) Value1(Func_02006c12, a0)
#define Motion_SetHPosTerrain_3(args...) Func_02006c70(args)
#define Scene_GetRecord_2(a0) Value1(Func_02006c26, a0)
#define Motion_SetHPosTerrain_4(args...) Func_02006c84(args)
#define Scene_GetRecord_3(a0) Value1(Func_02006c3a, a0)
#define Motion_SetHPosTerrain_5(args...) Func_02006c98(args)
#define Motion_SetSpeed_2(a0, a1, a2) Call3(Func_02006c5a, a0, a1, a2)
#define Motion_SetSpeed_3(a0, a1, a2) Call3(Func_02006c64, a0, a1, a2)
#define Motion_SetSpeed_4(a0, a1, a2) Call3(Func_02006c6e, a0, a1, a2)
#define Motion_EnableActCb_1(a0, a1) Value2(Func_02006c7e, a0, a1)
#define Motion_EnableActCb_2(a0, a1) Value2(Func_02006c86, a0, a1)
#define Motion_MarkActiveAndSetActionCallback_1(a0, a1) Value2(Func_02006c9e, a0, a1)
#define Motion_ArmCb_1(a0, a1, a2) Call3(Func_02006d42, a0, a1, a2)
#define Motion_ArmCb_2(a0, a1, a2) Call3(Func_02006d4e, a0, a1, a2)
#define Motion_ArmCb_3(a0, a1, a2) Call3(Func_02006d5a, a0, a1, a2)
#define Motion_SetVarCbObj_1(args...) Func_02006d22(args)
#define Motion_ArmCb_4(a0, a1, a2) Call3(Func_02006d6e, a0, a1, a2)
#define SceneWork_SetStepValue_1(a0) Call1(Func_02006d54, a0)
#define Battle_RunThenWaitIfModeZero_1(args...) Func_02006d76(args)
#define Motion_SetVarCbObj_2(args...) Func_02006d46(args)
#define Motion_ArmCb_5(a0, a1, a2) Call3(Func_02006d92, a0, a1, a2)
#define Motion_ArmCb_6(a0, a1, a2) Call3(Func_02006d9e, a0, a1, a2)
#define Motion_SetSpeed_5(a0, a1) Value2(Func_02006d8e, a0, a1)
#define UiWork_WaitThenFinalizeCapacity_1(a0, a1) Value2(Func_02006cee, a0, a1)
#define Motion_CallWaitAnim_1(args...) Func_02006d62(args)
#define Motion_CallWaitAnim_2(args...) Func_02006d6c(args)
#define Battle_RunThenWaitIfModeZero_2(args...) Func_02006dd4(args)
#define Object_SetModeById_2(args...) Func_02006d84(args)
#define Object_SetModeById_3(args...) Func_02006d8c(args)
#define Object_SetModeById_4(args...) Func_02006d94(args)
#define Motion_CallWaitAnim_3(args...) Func_02006da4(args)
#define Battle_WaitMode0_3(args...) Func_02006d1a_a(args)
#define Motion_SetVarCbObj_3(args...) Func_02006dca(args)
#define SceneWork_SetStepValue_2(a0) Call1(Func_02006df0, a0)
#define BattleEv_RunWait_1(args...) Func_02006e08(args)
#define Motion_ArmCb_7(a0, a1, a2) Call3(Func_02006e24, a0, a1, a2)
#define BattleFx_SpawnLinked_1(a0, a1, a2) Call3(Func_02006e40, a0, a1, a2)
#define Motion_ArmCb_8(a0, a1, a2) Call3(Func_02006e3c, a0, a1, a2)
#define BattleEv_RunWait_2(args...) Func_02006e34(args)
#define Motion_ArmCb_9(a0, a1, a2) Call3(Func_02006e50, a0, a1, a2)
#define BattleFx_SpawnLinked_2(a0, a1, a2) Call3(Func_02006e6a, a0, a1, a2)
#define BattleFx_SpawnLinked_3(a0, a1, a2) Call3(Func_02006e74, a0, a1, a2)
#define BattleFx_SpawnLinked_4(a0, a1, a2) Call3(Func_02006e7e, a0, a1, a2)
#define BattleFx_SpawnLinked_5(a0, a1, a2) Call3(Func_02006e88, a0, a1, a2)
#define Audio_PlayCue_1(args...) Func_02006f4e(args)
#define ObjectGroup_ConfigureChildValue_1(args...) Func_02006e4e(args)
#define Battle_WaitMode0_4(args...) Func_02006da4_a(args)
#define Audio_PlayCue_2(a0) Call1(Func_02006f62, a0)
#define Motion_ArmCb_10(a0, a1, a2) Call3(Func_02006e9e, a0, a1, a2)
#define Motion_ArmCb_11(a0, a1, a2) Call3(Func_02006eaa, a0, a1, a2)
#define Motion_ArmCb_12(a0, a1, a2) Call3(Func_02006eb6, a0, a1, a2)
#define Motion_ArmCb_13(a0, a1, a2) Call3(Func_02006ec2, a0, a1, a2)
#define BattleFx_SpawnLinked_6(a0, a1, a2) Call3(Func_02006ede, a0, a1, a2)
#define BattleFx_SpawnLinked_7(a0, a1, a2) Call3(Func_02006eea, a0, a1, a2)
#define BattleFx_SpawnLinked_8(a0, a1, a2) Call3(Func_02006ef6, a0, a1, a2)
#define BattleFx_SpawnLinked_9(a0, a1, a2) Call3(Func_02006f02, a0, a1, a2)
#define Audio_PlayCue_3(args...) Func_02006fc8(args)
#define Motion_EnableActCb_3(args...) Func_02006e66(args)
#define Motion_MarkActiveAndSetActionCallback_2(args...) Func_02006e7e_a(args)
#define Battle_WaitMode0_5(args...) Func_02006e3a(args)
#define Motion_SetVarCbObj_4(args...) Func_02006eea_a(args)
#define Battle_WaitMode0_6(args...) Func_02006e48(args)
#define BattleEv_RunWait_3(args...) Func_02006f28(args)
#define Motion_ArmCb_14(a0, a1, a2) Call3(Func_02006f44, a0, a1, a2)
#define Motion_ArmCb_15(a0, a1, a2) Call3(Func_02006f50, a0, a1, a2)
#define Object_SetModeById_5(args...) Func_02006ef8(args)
#define Object_SetModeById_6(args...) Func_02006f00(args)
#define Motion_CallWaitAnim_4(args...) Func_02006f10(args)
#define Battle_WaitMode0_7(args...) Func_02006e86(args)
#define Motion_SetVarCbObj_5(args...) Func_02006f36(args)
#define Battle_RunThenWaitIfModeZero_3(args...) Func_02006f78(args)
#define Motion_ArmCb_16(a0, a1, a2) Call3(Func_02006f8c, a0, a1, a2)
#define Motion_ArmCb_17(a0, a1, a2) Call3(Func_02006f98, a0, a1, a2)
#define Motion_CallWaitAnim_5(args...) Func_02006f48(args)
#define Battle_RunThenWaitIfModeZero_4(args...) Func_02006fa2(args)
#define Motion_CallWaitAnim_6(args...) Func_02006f5a(args)
#define Object_SetModeById_7(args...) Func_02006f5a_a(args)
#define Object_SetModeById_8(args...) Func_02006f62_a(args)
#define Motion_CallWaitAnim_7(args...) Func_02006f72(args)
#define Battle_WaitMode0_8(args...) Func_02006ee8(args)
#define Motion_EnableActCb_4(args...) Func_02006f32(args)
#define Motion_EnableActCb_5(a0, a1) Value2(Func_02006f3a, a0, a1)
#define Motion_MarkActiveAndSetActionCallback_3(args...) Func_02006f52(args)
#define Battle_WaitMode0_9(args...) Func_02006f08(args)
#define GameFlag_Clear_1(a0) Call1(Func_02006f14, a0)
#define GameFlag_Set_1(a0) Call1(Func_02006f1e, a0)
#define Battle_SchedShoulder_1(args...) Func_02006f42(args)
#define SCENE_PHASE (*(volatile s32 *)(work + 0x1c0))
#define SCENE_FIELD_1C8 (*(volatile s32 *)(work + 0x1c8))
#define RunEventScript01 Func_02003028

extern u8 Data_0200c0e4[];
extern u8 Data_0200c12c[];
extern u8 Data_03001ebc[];

void Func_02006b9c();
void Func_02006bdc();
s32 Func_02006c12();
void Func_02006c22();
s32 Func_02006c26();
void Func_02006c30();
s32 Func_02006c3a();
void Func_02006c40();
void Func_02006c5a();
void Func_02006c64();
void Func_02006c6e();
void Func_02006c70();
s32 Func_02006c7e();
void Func_02006c84();
s32 Func_02006c86();
void Func_02006c98();
s32 Func_02006c9e();
s32 Func_02006cee();
void Func_02006d1a();
void Func_02006d1a_a();
void Func_02006d22();
void Func_02006d26();
void Func_02006d34();
void Func_02006d42();
void Func_02006d46();
void Func_02006d4e();
void Func_02006d54();
void Func_02006d54_a();
void Func_02006d5a();
void Func_02006d62();
void Func_02006d6c();
void Func_02006d6e();
void Func_02006d76();
void Func_02006d84();
void Func_02006d8c();
s32 Func_02006d8e();
void Func_02006d92();
void Func_02006d94();
void Func_02006d9e();
void Func_02006da4();
void Func_02006da4_a();
void Func_02006dca();
void Func_02006dd4();
void Func_02006df0();
void Func_02006e08();
void Func_02006e24();
void Func_02006e34();
void Func_02006e3a();
void Func_02006e3c();
void Func_02006e40();
void Func_02006e48();
void Func_02006e4e();
void Func_02006e50();
void Func_02006e66();
void Func_02006e6a();
void Func_02006e74();
void Func_02006e7e();
void Func_02006e7e_a();
void Func_02006e86();
void Func_02006e88();
void Func_02006e9e();
void Func_02006eaa();
void Func_02006eb6();
void Func_02006ec2();
void Func_02006ede();
void Func_02006ee8();
void Func_02006eea();
void Func_02006eea_a();
void Func_02006ef6();
void Func_02006ef8();
void Func_02006f00();
void Func_02006f02();
void Func_02006f08();
void Func_02006f10();
void Func_02006f14();
void Func_02006f1e();
void Func_02006f28();
void Func_02006f32();
void Func_02006f36();
s32 Func_02006f3a();
void Func_02006f42();
void Func_02006f44();
void Func_02006f48();
void Func_02006f4e();
void Func_02006f50();
void Func_02006f52();
void Func_02006f5a();
void Func_02006f5a_a();
void Func_02006f62();
void Func_02006f62_a();
void Func_02006f72();
void Func_02006f78();
void Func_02006f8c();
void Func_02006f98();
void Func_02006fa2();
void Func_02006fc8();
void Func_0200affd();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Phase/status word at 0x1c0, and a companion word at 0x1c8, of the shared
 * scene work record. */

/* Remaining raw call sites, named from the engine function's own source. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Record returned by Func_02006c12/26/3a: a pair of s32 fields at +8 and
 * +16 that get forwarded straight into the matching setup call. */
void RunEventScript01(void)
{
    u32 i;
    s32 record;
    u8 *work;
    s32 addr_0200affd;
    s32 addr_0200c0e4;
    s32 addr_0200c12c;

    Battle_Reset_1();
    Motion_SetHPosTerrain_1(8, 0x1480000, 0x580000);
    Motion_SetHPosTerrain_2(9, 0x1480000, 0x580000);
    Object_SetModeById_1(8, 0);
    work = *(u8 *volatile *)Data_03001ebc;
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
    addr_0200affd = (s32)Func_0200affd;
    Call2(Func_02006d34, addr_0200affd, 0xc80); /* main:080000d0 */
    addr_0200c0e4 = (s32)Data_0200c0e4;
    Motion_EnableActCb_3(9, addr_0200c0e4);
    Motion_MarkActiveAndSetActionCallback_2(8, addr_0200c0e4);
    Func_02006d54_a(addr_0200affd); /* main:080000d8 */
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
    addr_0200c12c = (s32)Data_0200c12c;
    Motion_EnableActCb_4(1, addr_0200c12c);
    Motion_EnableActCb_5(2, addr_0200c12c);
    Motion_MarkActiveAndSetActionCallback_3(3, addr_0200c12c);
    Battle_WaitMode0_9(20);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x204;
    GameFlag_Clear_1(0x12f);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c8)) = 16;
    GameFlag_Set_1(0x909);
    Battle_SchedShoulder_1();
}
#undef Battle_Reset_1
#undef Motion_SetHPosTerrain_1
#undef Motion_SetHPosTerrain_2
#undef Object_SetModeById_1
#undef Battle_WaitMode0_1
#undef Motion_SetSpeed_1
#undef Battle_WaitMode0_2
#undef Scene_GetRecord_1
#undef Motion_SetHPosTerrain_3
#undef Scene_GetRecord_2
#undef Motion_SetHPosTerrain_4
#undef Scene_GetRecord_3
#undef Motion_SetHPosTerrain_5
#undef Motion_SetSpeed_2
#undef Motion_SetSpeed_3
#undef Motion_SetSpeed_4
#undef Motion_EnableActCb_1
#undef Motion_EnableActCb_2
#undef Motion_MarkActiveAndSetActionCallback_1
#undef Motion_ArmCb_1
#undef Motion_ArmCb_2
#undef Motion_ArmCb_3
#undef Motion_SetVarCbObj_1
#undef Motion_ArmCb_4
#undef SceneWork_SetStepValue_1
#undef Battle_RunThenWaitIfModeZero_1
#undef Motion_SetVarCbObj_2
#undef Motion_ArmCb_5
#undef Motion_ArmCb_6
#undef Motion_SetSpeed_5
#undef UiWork_WaitThenFinalizeCapacity_1
#undef Motion_CallWaitAnim_1
#undef Motion_CallWaitAnim_2
#undef Battle_RunThenWaitIfModeZero_2
#undef Object_SetModeById_2
#undef Object_SetModeById_3
#undef Object_SetModeById_4
#undef Motion_CallWaitAnim_3
#undef Battle_WaitMode0_3
#undef Motion_SetVarCbObj_3
#undef SceneWork_SetStepValue_2
#undef BattleEv_RunWait_1
#undef Motion_ArmCb_7
#undef BattleFx_SpawnLinked_1
#undef Motion_ArmCb_8
#undef BattleEv_RunWait_2
#undef Motion_ArmCb_9
#undef BattleFx_SpawnLinked_2
#undef BattleFx_SpawnLinked_3
#undef BattleFx_SpawnLinked_4
#undef BattleFx_SpawnLinked_5
#undef Audio_PlayCue_1
#undef ObjectGroup_ConfigureChildValue_1
#undef Battle_WaitMode0_4
#undef Audio_PlayCue_2
#undef Motion_ArmCb_10
#undef Motion_ArmCb_11
#undef Motion_ArmCb_12
#undef Motion_ArmCb_13
#undef BattleFx_SpawnLinked_6
#undef BattleFx_SpawnLinked_7
#undef BattleFx_SpawnLinked_8
#undef BattleFx_SpawnLinked_9
#undef Audio_PlayCue_3
#undef Motion_EnableActCb_3
#undef Motion_MarkActiveAndSetActionCallback_2
#undef Battle_WaitMode0_5
#undef Motion_SetVarCbObj_4
#undef Battle_WaitMode0_6
#undef BattleEv_RunWait_3
#undef Motion_ArmCb_14
#undef Motion_ArmCb_15
#undef Object_SetModeById_5
#undef Object_SetModeById_6
#undef Motion_CallWaitAnim_4
#undef Battle_WaitMode0_7
#undef Motion_SetVarCbObj_5
#undef Battle_RunThenWaitIfModeZero_3
#undef Motion_ArmCb_16
#undef Motion_ArmCb_17
#undef Motion_CallWaitAnim_5
#undef Battle_RunThenWaitIfModeZero_4
#undef Motion_CallWaitAnim_6
#undef Object_SetModeById_7
#undef Object_SetModeById_8
#undef Motion_CallWaitAnim_7
#undef Battle_WaitMode0_8
#undef Motion_EnableActCb_4
#undef Motion_EnableActCb_5
#undef Motion_MarkActiveAndSetActionCallback_3
#undef Battle_WaitMode0_9
#undef GameFlag_Clear_1
#undef GameFlag_Set_1
#undef Battle_SchedShoulder_1
#undef SCENE_PHASE
#undef SCENE_FIELD_1C8
#undef RunEventScript01

/* overlays/scene/story/set_piece/scene_setup.c */
#define Scene_RunValue1528Scene Func_02000a64
#define State_StoreParamsAndInstallTask Func_02002bb0
extern s32 Data_0200d240[3];
extern s32 Data_0200d23c;

void Func_020045d8(void);
void Func_020045b8(s32, s32);
void Func_02004796(s32);
void Func_02001466(void);
void Func_020044f8(s32);
void Func_0200479c(void);
void Func_02004600_a(void);

s32 Func_020068d2();           /* per-site veneer -> Func_0808a5e0 */

s32 Func_0200664c();           /* per-site veneer -> Func_080000d0 */

void Scene_RunValue1528Scene(void)
{
    Func_020045d8();
    Func_020045b8(0x1528, 1);
    Func_02004796(125);
    Func_02001466();
    Func_020044f8(20);
    Func_0200479c();
    Func_02004600_a();
}

void State_StoreParamsAndInstallTask(s32 v0, s32 v1, s32 v2, s32 v3)
{
    Data_0200d240[0] = v0;
    Data_0200d240[1] = v1;
    Data_0200d240[2] = v2;
    Data_0200d23c = v3;

    Func_020068d2(170);
    Func_0200664c(0x0200ab59, 0xc80);
}
#undef Scene_RunValue1528Scene
#undef State_StoreParamsAndInstallTask

/* overlays/scene/story/set_piece/scene_state_interaction.c */
#define State_SetByte22bTo3AndSend51 Func_020003e0
extern u8 Data_02000240[];
extern u8 Value_00000051;

void Func_0200409a(s32, s32);
void Func_02004092(s32, s32);

void State_SetByte22bTo3AndSend51(void)
{
    Data_02000240[0x22b] = 3;
    Func_0200409a((s32)&Value_00000051, 99);
    Func_02004092(53, 2);
}
#undef State_SetByte22bTo3AndSend51

/* overlays/scene/story/set_piece/state_update.c */
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define State_SetValue14Mode23 Func_02000030
#define State_SetWorkByte22bTo3 Func_02000388
#define State_SetByte22bTo3 Func_020003b4
#define State_ApplyRectAndSetActor9Byte55 Func_020004cc
#define State_ApplyRectAndLowerActor9 Func_020005ac
#define Actor_AdjustSlot12AndSetFlag204 Func_02000650
#define State_ForwardByRuntimeSelector Func_02002ce0
#define State_SetWorkspaceHalfword382To1018 Func_02003410

extern u8 Data_02000240[];
extern u8 Value_0000004d;
extern u8 Value_0000004f;
extern s32 Data_03001e40;
extern u8 *Data_03001ebc;   /* The scene workspace pointer. */
extern u8 Value_00001018;   /* The pooled constant 0x1018. */

void Func_02003cd8(s32, s32);
void Func_02004042(s32, s32);
void Func_0200403a(s32, s32);
void Func_0200406e(s32, s32);
void Func_02004066(s32, s32);
s32 *Func_0200406c(s32);
void Func_02004012(s32, s32, s32, s32, s32, s32);
void Func_02004054(s32);
s32 *Func_0200414c(s32);
void Func_020040f2(s32, s32, s32, s32, s32, s32);
void Func_02004106(s32 *, s32);
void Func_02004146(s32);
s32 *Func_020041f0(s32);
void Func_02004196(s32, s32, s32, s32, s32, s32);
void Func_020041aa(s32 *, s32);
void Func_020041ea(s32);
void Func_02006912();
void Func_0200691e();

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
    Func_02003cd8(0xE, 0x17);
}

void State_SetWorkByte22bTo3(void)
{
    Data_02000240[0x22b] = 3;
    Func_02004042((s32)&Value_0000004d, 99);
    Func_0200403a(53, 2);
}

void State_SetByte22bTo3(void)
{
    Data_02000240[0x22b] = 3;
    Func_0200406e((s32)&Value_0000004f, 99);
    Func_02004066(53, 2);
}

void State_ApplyRectAndSetActor9Byte55(void)
{
    s32 *p;
    s32 s0;
    s32 s1;

    p = Func_0200406c(9);
    s0 = 17;
    s1 = 13;
    Func_02004012(29, 1, 3, 1, s0, s1);
    if (p != 0) {
        ((u8 *)p)[0x55] = 2;
    }
    Func_02004054(0x201);
}

void State_ApplyRectAndLowerActor9(void)
{
    s32 *rec;
    s32 s0;
    s32 s1;

    rec = Func_0200414c(9);
    s0 = 43;
    s1 = 41;
    Func_020040f2(45, 41, 1, 1, s0, s1);
    if (rec != 0) {
        Func_02004106(rec, 0);
        rec[3] += 0xffe00000;
        ((u8 *)rec)[0x23] = 2;
    }
    Func_02004146(0x200);
}

void Actor_AdjustSlot12AndSetFlag204(void)
{
    s32 *p;
    s32 s0;
    s32 s1;

    p = Func_020041f0(12);
    s0 = 26;
    s1 = 15;
    Func_02004196(1, 0, 1, 1, s0, s1);
    if (p != 0) {
        Func_020041aa(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    Func_020041ea(0x204);
}

void State_ForwardByRuntimeSelector(s32 arg)
{
    s32 sel = Data_03001e40 & 7;

    if (sel == 0) {
        Func_02006912(arg, 2);
    } else if (sel == 2) {
        Func_0200691e(arg, 0);
    }
}

void State_SetWorkspaceHalfword382To1018(void)
{
    *(u16 *)(Data_03001ebc + (191 << 1)) = (int)&Value_00001018;
}
#undef NULL
#undef FIELD_AT_OFFSET
#undef State_SetValue14Mode23
#undef State_SetWorkByte22bTo3
#undef State_SetByte22bTo3
#undef State_ApplyRectAndSetActor9Byte55
#undef State_ApplyRectAndLowerActor9
#undef Actor_AdjustSlot12AndSetFlag204
#undef State_ForwardByRuntimeSelector
#undef State_SetWorkspaceHalfword382To1018
