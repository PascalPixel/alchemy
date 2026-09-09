/*
 * A run of unnamed leaf owners in resource_376: scene data getters, actor
 * dialogue beats, numbered scene steps, and the long presentation
 * sequence.
 */

#include "types.h"

#define NULL ((void *)0)

/*
 * Each Func_ symbol names the pre-relocation call word the image holds,
 * not a runtime address.
 */
#define CalculateFacingAngle Func_020011b6
#define Scene_UpdateTimedActor Func_020010c0
#define OverlayObject_UpdateFacingTowardTarget Func_02000030
#define AdvancePositionScaleAndVelocity Func_02000088
#define SceneData_GetTable9478 Func_020000cc
#define SceneData_ReturnZero Func_020000d4
#define SceneData_SelectTable9568ByFlag Func_020000d8
#define SceneData_SelectFlaggedTable Func_020000fc
#define SceneData_SelectTable9c00ByFlags Func_02000140
#define SceneDialogue_RunActor15Message0f6d Func_02000190
#define SceneDialogue_RunActor19Message0f73 Func_020001bc
#define FieldScene_RunScene376_020001e8 Func_020001e8
#define SceneDialogue_RunActor16Message11be Func_02000258
#define SceneDialogue_RunActor10Message1c3d Func_02000278
#define FieldScene_RunScene376_02000298 Func_02000298
#define SceneState_SetRuntimeWord448To521AndRun Func_020002bc
#define FieldScene_RunIndexedStep1 Func_02000308
#define FieldScene_RunIndexedStep2 Func_02000314
#define FieldScene_RunIndexedStep3 Func_02000320
#define FieldScene_RunIndexedStep4 Func_0200032c
#define FieldScene_RunIndexedStep5 Func_02000338
#define FieldScene_RunIndexedStep6 Func_02000344
#define FieldScene_RunIndexedStep7 Func_02000350
#define FieldScene_RunIndexedStep8 Func_0200035c
#define FieldScene_RunByActorDirectionAndFlags Func_020004dc
#define FieldScene_RunScene376_0200055c Func_0200055c
#define FieldScene_RunScene376_020005d4 Func_020005d4
#define FieldScene_RunLongPresentationSequence Func_02000658

#include "facing_object.h"

typedef struct {
    u8 pad_to_position[8];
    s32 x;
    s32 y;
    u8 pad_to_scale[8];
    s32 scale_x;
    s32 scale_y;
    u8 pad_to_velocity[0x44];
    s16 velocity_x;
    s16 velocity_y;
} ScaledMotion;

union SceneActor {
    s32 words[26];
    u16 halfwords[52];
};

/*
 * The fields at 100 and 102 are consecutive halfwords, so this byte and
 * halfword view needs two-byte layout rather than the default four.
 */
union SceneField {
    u16 value;
    u8 bytes[2];
} __attribute__((packed, aligned(2)));

extern u8 Data_020098b8[];
extern u8 Data_02009738[];
extern u8 Data_020095b8[];
extern u8 Data_000011a4[];
extern u8 Data_00001c40[];
extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];
extern u8 Data_020092fc[];
extern u8 Data_02009400[];
extern u32 Data_03001e40;
extern u8 Data_02009440[];

s32 Func_020011b6(s32, s32);
s32 Func_02001262(s32 flag);
s32 Func_02001286(s32);
s32 Func_02001294(s32);
void Func_020012cc(void *);
s32 Func_020012e6(s32 flag);
void Func_02001330(void);
void Func_020013de(s32 msg);
void Func_020013d8(s32 actor, s32 target, s32 frames);
s32 Func_02001410(s32 actor, s32 mode);
void Func_02001354(void);
void Func_0200135c(void);
void Func_0200140a(s32 msg);
void Func_02001404(s32 actor, s32 target, s32 frames);
s32 Func_0200143c(s32 actor, s32 mode);
void Func_02001380(void);
s32 Func_02001376();
void Func_02001388();
void Func_020013aa();
void Func_020013c8();
void Func_020013e4();
void Func_020013ea();
void Func_02001440();
void Func_0200144c();
void Func_02001458();
void Func_0200145e();
void Func_02001482();
void Func_020013f8(void);
void Func_020014a6(s32 msg);
s32 Func_020014ce(s32 actor, s32 mode);
void Func_02001412(void);
void Func_02001418(void);
void Func_020014c6(s32 msg);
s32 Func_020014ee(s32 actor, s32 mode);
void Func_02001432(void);
void Func_02001438();
void Func_02001452();
void Func_020014e6();
void Func_020014fe();
s32 Func_02001448(s32 flagId);
void Func_02001588(void);
void Func_020015c6(s32 soundId);
void Func_020015c8(void);
void Func_020005ca(int value);
void Func_020005d6(int value);
void Func_020005e2(int value);
void Func_020005ee(int value);
void Func_020005fa(int value);
void Func_02000606(int value);
void Func_02000612(int value);
void Func_0200061e(int value);
void Func_02000aba();
s32 Func_02001500();
void Func_0200157a();
void Func_0200158e();
void Func_02001598();
void Func_020015ac();
void Func_020015b6();
void Func_020015c0();
s32 Func_020015c4();
void Func_020015ca();
s32 Func_020015de_a();
s32 Func_020015de_b();
void Func_020015e8();
void Func_020015f2();
void Func_020015fc();
void Func_02001606();
s32 Func_0200160c();
void Func_02001610();
void Func_0200161a_a();
void Func_0200161a_b();
void Func_02001620();
u8 *Func_0200162c();
u8 *Func_02001664();
void Func_0200169e();
void Func_020016ba();
void Func_020016ce();
void Func_020016ec();
void Func_02001706_a();
void Func_02001706_b();
void Func_0200171a();
u8 *Func_020016a6(s32 actor_id);
void Func_02001698(void);
s32 Func_020016a0(s32 flag_id);
void Func_02001772(s32 dialogue_id);
void Func_02001778(s32 actor_id, s32 mode);
void Func_0200178a(s32 actor_id, s32 mode);
void Func_020016de(void);
s32 Func_02001706();
s32 Func_02001718_a();
s32 Func_02001718_b();
s32 Func_02001726();
void Func_02001756();
void Func_020017d0();
void Func_020017ea();
void Func_02001802();
void Func_02001862();
void Func_02000808();
s32 Func_0200177a();
void Func_0200179a();
s32 Func_0200179c();
s32 Func_0200179e();
void Func_020017b4();
void Func_020017c2();
void Func_020017dc();
void Func_02001848();
void Func_02001860();
void Func_02001870();
void Func_02001888();
void Func_020018da();
void Func_020017c6();
void Func_020017fe();
void Func_02001840();
void Func_0200184c();
void Func_02001858_a();
void Func_02001858_b();
void Func_02001862_a();
void Func_02001862_b();
void Func_02001864();
void Func_0200186c();
void Func_02001876();
s32 Func_0200188a();
s32 Func_02001896();
s32 Func_020018a2();
void Func_020018ac();
s32 Func_020018ae();
s32 Func_020018ba();
void Func_020018be();
s32 Func_020018c4();
void Func_020018fe();
s32 Func_02001906();
void Func_02001908();
s32 Func_0200191a();
s32 Func_0200192e();
void Func_02001930();
void Func_02001948();
void Func_0200194a();
void Func_02001954();
void Func_02001968();
void Func_02001972();
void Func_0200197c_a();
void Func_0200197c_b();
void Func_02001980();
void Func_02001994();
void Func_020019a0();
void Func_020019b4();
void Func_020019bc();
void Func_020019c2();
void Func_020019d0();
void Func_020019ee();
void Func_020019fe_a();
void Func_020019fe_b();
void Func_02001a08();
void Func_02001a12_a();
void Func_02001a12_b();
void Func_02001a1c_a();
void Func_02001a1c_b();
void Func_02001a28();
void Func_02001a3a();
void Func_02001a40();
void Func_02001a46();
void Func_02001a4a();
void Func_02001a4c();
void Func_02001a56();
void Func_02001a84();
s32 Func_02001a90();
void Func_02001a92();
void Func_02001a9a();
void Func_02001aa2();
void Func_02001ab4_a();
void Func_02001ab4_b();
void Func_02001aba();
void Func_02001abe();
void Func_02001ae2();
void Func_02001ae8();
void Func_02001afa();
void Func_02001b00();
void Func_02001b06();
void Func_02001b10();
void Func_02001b16_a();
void Func_02001b16_b();
void Func_02001b1a();
void Func_02001b20();
void Func_02001b28();
void Func_02001b2e();
void Func_02001b38();
void Func_02001b40();
void Func_02001b4a();
void Func_02001b4e();
void Func_02001b56();
void Func_02001b5c();
void Func_02001b5e();
void Func_02001b66();
void Func_02001b68();
void Func_02001b6e();
void Func_02001b70();
void Func_02001b76();
void Func_02001b86();
void Func_02001b98();
void Func_02001bb2();
void Func_02001bb8();
void Func_02001bc0();
void Func_02001bda();
void Func_02001be0();
void Func_02001be8();
void Func_02001bea();
void Func_02001bfa();
void Func_02001c00();
void Func_02001c0c();
void Func_02001c1a();
void Func_02001c1e();
void Func_02001c34_a();
void Func_02001c34_b();
void Func_02001c3c();
void Func_02001c5c();
void Func_02001c5e();
void Func_02001c68();
void Func_02001c72();
void Func_02001c7a();
void Func_02001c82();
void Func_02001c8a();
void Func_02001c8e();
void Func_02001c92_a();
void Func_02001c92_b();
void Func_02001cb8();
void Func_02001cbe();
void Func_02001cc0();
void Func_02001cc6();
void Func_02001cca_a();
void Func_02001cca_b();
void Func_02001ce2();
void Func_02001d1a();
void Func_02001d40();
void Func_02001d44_a();
void Func_02001d44_b();
void Func_02001d4c();
void Func_02001d6c();
void Func_02001d72();
void Func_02001d88();
void Func_02001d94();
void Func_02001d98();
void Func_02001d9a();
void Func_02001dbe();
void Func_02001dc6();
void Func_02001dcc();
void Func_02001dd2();
void Func_02001dd6();
void Func_02001ddc();
void Func_02001dde();
void Func_02001df6();
void Func_02001e00();
void Func_02001e02_a();
void Func_02001e02_b();
void Func_02001e0e();
s32 Func_02001e12_a();
s32 Func_02001e12_b();
void Func_02001e1e();
void Func_02001e20();
void Func_02001e30();
void Func_02001e50();
void Func_02001e52_a();
void Func_02001e52_b();
s32 Func_02001e5e();
void Func_02001e6c();
void Func_02001e6e();
void Func_02001e8a();
void Func_02001e96();
void Func_02001ea2();
void Func_02001eaa();
void Func_02001eae();
void Func_02001eb2();
void Func_02001eba();
void Func_02001ede();
void Func_02001ee2();
void Func_02001ee6_a();
void Func_02001ee6_b();
void Func_02001ef4();
void Func_02001efe();
void Func_02001f0c();
void Func_02001f10();
void Func_02001f2c();
void Func_02001f36();
void Func_02001f38_a();
void Func_02001f38_b();
void Func_02001f40();
void Func_02001f48_a();
void Func_02001f48_b();
void Func_02001f4c();
void Func_02001f52();
void Func_02001f58_a();
void Func_02001f58_b();
void Func_02001f58_c();
void Func_02001f7c();
void Func_02001f90();
void Func_02001fa2();
void Func_02001fb0();
void Func_02001fb2();
void Func_02001fc2();
void Func_02001fcc();
void Func_02001fd4();
void Func_02001fda();
void Func_02001fe2();
void Func_02001fe6();
void Func_02001ff0();
void Func_02001ffa();
s32 Func_02001ffe();
void Func_0200201c();
void Func_02002026();
void Func_02002028_a();
void Func_02002028_b();
void Func_0200202e();
void Func_02002030();
void Func_02002036();
void Func_02002038();
void Func_02002042();
void Func_02002048();
void Func_0200205a();
s32 Func_02002062();
void Func_02002066();
void Func_02002068();
void Func_02002080();
void Func_020020b4();
void Func_020020c6();
void Func_020020c8_a();
void Func_020020c8_b();
void Func_020020ce();
void Func_020020d6();
void Func_020020de();
void Func_020020ea();
void Func_02002104();
void Func_0200210a();
void Func_0200210e();
void Func_02002114();
void Func_0200212e();
void Func_0200213e();
void Func_0200215a();
void Func_02002168();
void Func_020021c2_a();
void Func_020021c2_b();
void Func_020021c4();
void Func_020021c8();
void Func_020021d0();
void Func_020021d4();
void Func_020021e6();
void Func_020021e8();
void Func_020021f8();
void Func_020021fa();
void Func_02002204();
void Func_02002210();
void Func_02002216_a();
void Func_02002216_b();
void Func_0200221c();
void Func_02002228();
void Func_0200222a();
s32 Func_02002230();
void Func_02002232_a();
void Func_02002232_b();
void Func_0200223a();
s32 Func_02002240();
void Func_02002242();
s32 Func_02002248();
void Func_0200224a();
void Func_0200225e();
void Func_0200226a();
void Func_0200226c();
void Func_02002276();
void Func_02002282();
void Func_020022f0();
void Func_020090c1();
union SceneActor *Func_020022b0(s32);
s32 *Func_020022bc(s32);
void Func_0200230a(s32, s32, s32);
void Func_02002302(s32, void *);

/*
 * Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the
 * block. A value-returning call sets r0 last of its arguments, so a callee
 * is declared to return a value here even where the result is unused.
 */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    f(a0, a1);
}

/* Advance the scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];
    void Func_0200220e();

    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_03001ebc[];
    void Func_0200220e();

    f(a0, a1, a2, a3);
}

static __inline__ void Call11(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9, s32 a10)
{
    extern u8 Data_03001ebc[];
    void Func_0200220e();

    f(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}

s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *obj)
{
    s32 delta;
    u16 old;
    s32 angle;
    struct FacingObject *target;

    target = obj->facing_target;
    if (target != NULL) {
        obj->facing_flags = (u8)(0xFE & obj->facing_flags);
        angle = (u16)CalculateFacingAngle(target->position_z - obj->position_z, target->position_x - obj->position_x);
        old = obj->facing;
        delta = (s16)(angle - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }
            /* The loader relocates the stored pool word to -0x1000. */
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            obj->facing = (u16)(old + delta);
        }
    }
    return 1;
}

s32 AdvancePositionScaleAndVelocity(ScaledMotion *motion)
{
    motion->x += motion->velocity_x << 8;
    motion->y += motion->velocity_y << 8;
    motion->scale_x += 0x666;
    motion->scale_y += 0x666;
    motion->velocity_x += 5;
    motion->velocity_y -= 1;
    return 0;
}

/* The eight-byte owner includes its one pool word. */
void *SceneData_GetTable9478(void)
{
    return (void *)0x02009478;
}

/* A four-byte leaf that returns zero. */
int SceneData_ReturnZero(void)
{
    return 0;
}

/* The 36-byte owner includes its three pool words. */
void *SceneData_SelectTable9568ByFlag(void)
{
    if (Func_02001262(0x834) != 0)
        return (void *)0x02009590;
    return (void *)0x02009568;
}

void *SceneData_SelectFlaggedTable(void)
{
    void *tbl;

    if (Func_02001286(0x87a)) {
        tbl = Data_020098b8;
    } else if (Func_02001294(0x815)) {
        tbl = Data_02009738;
    } else {
        tbl = Data_020095b8;
    }
    Func_020012cc(tbl);
    return tbl;
}

/* The 80-byte owner includes its seven pool words. */
void *SceneData_SelectTable9c00ByFlags(void)
{
    if (Func_020012ca(0x834) != 0)
        return (void *)0x02009ac8;
    if (Func_020012d8(0x87a) != 0)
        return (void *)0x02009ffc;
    if (Func_020012e6(0x815) != 0)
        return (void *)0x02009da4;
    return (void *)0x02009c00;
}

/* The 44-byte actor-15 scene owner includes its one pool word. */
void SceneDialogue_RunActor15Message0f6d(void)
{
    Func_02001330();
    Func_020013de(0xf6d);
    Func_020013d8(0, 15, 6);
    Func_02001410(15, 0);
    Func_02001354();
}

/* The 44-byte actor-19 scene owner includes its one pool word. */
void SceneDialogue_RunActor19Message0f73(void)
{
    Func_0200135c();
    Func_0200140a(0xf73);
    Func_02001404(0, 19, 6);
    Func_0200143c(19, 0);
    Func_02001380();
}

void FieldScene_RunScene376_020001e8(void)
{
    s32 callback;
    s32 base5_11a4;

    Func_02001388();
    if (Value1(Func_02001376, 0x81b) != 0) {
        Call1(Func_02001440, 0x11a6);
        Func_02001458(20, 0);
        callback = 0x20092fc;
        Call3(Func_0200144c, 20, 0x10000, callback);
    } else {
        base5_11a4 = (s32)Data_000011a4;
        Func_0200145e(base5_11a4);
        Func_02001482(20, 0, 20);
        Func_020013aa((base5_11a4 + 1), 1);
        Func_020013ea(180, 0);
        Call1(Func_020013c8, 0x81b);
    }
    Func_020013e4();
}

/* The 32-byte actor-16 dialogue owner includes its one pool word. */
void SceneDialogue_RunActor16Message11be(void)
{
    Func_020013f8();
    Func_020014a6(0x11be);
    Func_020014ce(16, 0);
    Func_02001412();
}

/* The 32-byte actor-10 dialogue owner includes its one pool word. */
void SceneDialogue_RunActor10Message1c3d(void)
{
    Func_02001418();
    Func_020014c6(0x1c3d);
    Func_020014ee(10, 0);
    Func_02001432();
}

void FieldScene_RunScene376_02000298(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02001438();
    Func_020014e6((s32)Data_00001c40);
    Call2(Func_020014fe, 0x800b, 0);
    Func_02001452();
}

/* The 76-byte shared numbered-scene owner includes its two pool words. */
void SceneState_SetRuntimeWord448To521AndRun(s32 value)
{
    extern u8 *Data_03001ebc;
    void Func_020015a2_a(s32 value);
    void Func_020015d4_a(void);

    if (Func_02001448(0x834) != 0)
        Func_02001588();
    Func_020015c6(123);
    *(s32 *)(Data_03001ebc + 448) = 521;
    *(s32 *)(Data_03001ebc + 456) = 16;
    Func_020015c8();
    Func_020015d4_a();
    Func_020015a2_a(value);
}

/* Eight numbered-scene wrappers follow, each a twelve-byte owner. */
void FieldScene_RunIndexedStep1(void)
{
    Func_020005ca(1);
}

void FieldScene_RunIndexedStep2(void)
{
    Func_020005d6(2);
}

void FieldScene_RunIndexedStep3(void)
{
    Func_020005e2(3);
}

void FieldScene_RunIndexedStep4(void)
{
    Func_020005ee(4);
}

void FieldScene_RunIndexedStep5(void)
{
    Func_020005fa(5);
}

void FieldScene_RunIndexedStep6(void)
{
    Func_02000606(6);
}

void FieldScene_RunIndexedStep7(void)
{
    Func_02000612(7);
}

void FieldScene_RunIndexedStep8(void)
{
    Func_0200061e(8);
}

s32 Func_02000368(void)
{
    extern u8 Data_03001ebc[];
    void Func_020015a2();
    void Func_020015d4();

    u32 i;
    u8 *record;
    u8 *volatile *scene = (u8 *volatile *)Data_03001ebc;

    *(s32 *)(scene[0] + 0x1c0) = 0x209;
    if (Value1(Func_02001500, 0x834) != 0) {
        Func_0200158e(8, 0, 0);
        Func_02001598(9, 0, 0);
        Func_020015a2(10, 0, 0);
        Func_020015ac(11, 0, 0);
        Func_020015b6(12, 0, 0);
        Func_020015c0(13, 0, 0);
        Func_020015ca(14, 0, 0);
        Func_020015d4(15, 0, 0);
        ((void (*)())Func_020015de_a)(16, 0, 0);
        Func_020015e8(17, 0, 0);
        Func_020015f2(18, 0, 0);
        Func_020015fc(19, 0, 0);
        Func_02001606(20, 0, 0);
        Func_02001610(21, 0, 0);
        Func_0200161a_a(22, 0, 0);
        Func_020016ce();
        {
            u16 *target = (u16 *)(scene[3] + 0x1f84);
            s32 shown = 1;

            *target = shown;
        }
        Func_020016ec();
        Func_0200157a(30);
        Func_02001706_a();
        Func_0200171a();
        Func_02001706_b();
    }
    if (Value1(Func_020015c4, 0x87a) != 0) {
        if (Data_02000240_t[225][0] == 6) {
            if (Value1(Func_020015de_b, 0x81d) == 0) {
                Func_02000aba();
            }
        }
        {
            u8 *record = Func_0200162c(10);
            u8 value = *(volatile u8 *)&record[89];

            record[89] = (u8)(value | 128);
        }
    }
    if (Data_02000240_t[225][0] == 2) {
        if (Value1(Func_0200160c, 0x815) != 0) {
            Call3(Func_0200169e, 13, 0x1c60000, 0x960000);
            record = Func_02001664(13);
            Func_0200161a_b((s32)record, 0);
            Func_020016ba(13, 5);
            Func_02001620(4);
        }
    }
    return 0;
}

/*
 * Choose what follows from actor zero's directional halfword at +6 and two
 * story flags. The halfword is reduced by 0xa001 modulo 2^32 and compared
 * unsigned with 0x3ffe, so the first arm covers 0xa001..0xdfff; what that
 * range means is not established. The record is fetched before either path
 * is chosen. The 128-byte owner includes its seven pool words.
 */
void FieldScene_RunByActorDirectionAndFlags(void)
{
    void Func_020017e2(s32 arg0, s32 actor_id);

    u8 *p;
    u32 dir;

    p = Func_020016a6(0);
    dir = *(u16 *)(p + 6);
    dir += 0xffff5fff;

    if (dir <= 0x3ffe) {
        Func_020017e2(1, 21);
        return;
    }

    Func_02001698();
    if (Func_02001686(0x87a) != 0) {
        Func_02001750(0x1c06);
        Func_02001778(21, 0);
    } else {
        if (Func_020016a0(0x815) != 0) {
            Func_0200176a(0x11a2);
        } else {
            Func_02001772(0x0f53);
        }
        Func_0200178a(21, 0);
    }
    Func_020016de();
}

void FieldScene_RunScene376_0200055c(void)
{
    extern u8 Data_03001ebc[];
    void Func_020017e2_a();

    u32 i;
    s32 record;

    record = Func_02001726(0);
    if ((u32)(*(volatile u16 *)(record + 6) + -0xa001) <= 0x3ffe) {
        Func_02001862(2, 22);
    } else {
        ((void (*)())Func_02001718_a)();
        if (Value1(Func_02001706, 0x87a) != 0) {
            Call1(Func_020017d0, 0x1c09);
        } else {
            if (Value1(Func_02001718_b, 0x815) != 0) {
                Call1(Func_020017e2_a, 0x11a3);
            } else {
                Call1(Func_020017ea, 0xf54);
            }
        }
        Func_02001802(22, 0);
        Func_02001756();
    }
}

void FieldScene_RunScene376_020005d4(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    record = Func_0200179e(0);
    if ((u32)(*(volatile u16 *)(record + 6) + -0xa001) <= 0x3ffe) {
        Func_020018da(3, 20);
    } else {
        if (Value1(Func_0200177a, 0x87a) != 0) {
            Func_0200179a();
            Call1(Func_02001848, 0x1c0a);
            Func_02001860(20, 0);
            Func_020017b4();
        } else {
            if (Value1(Func_0200179c, 0x815) != 0) {
                Func_02000808();
            } else {
                Func_020017c2();
                Call1(Func_02001870, 0xf55);
                Func_02001888(20, 0);
                Func_020017dc();
            }
        }
    }
}

void FieldScene_RunLongPresentationSequence(void)
{
    extern u8 Data_03001ebc[];
    void Func_0200220e();

    u32 i;
    s32 record;
    s32 v6;
    s32 base7_20090c1;
    s32 base5_20092fc;
    s32 base5_2009400;

    Func_020017fe();
    Call4(Func_02001908, -1, -1, -1, 0);
    Func_020017c6(1);
    Func_020018fe(3, 1);
    Call3(Func_02001858_a, 0, 0x6666, 0x3333);
    Call3(Func_02001862_a, 1, 0x6666, 0x3333);
    Call3(Func_0200186c, 2, 0x6666, 0x3333);
    Call3(Func_02001876, 3, 0x6666, 0x3333);
    Func_020018be(8, 5);
    Call3(Func_020018ac, 0, 0x328, 0x1fc);
    record = Func_0200188a(23);
    Func_02001840(record, 0);
    record = Func_02001896(24);
    Func_0200184c(record, 0);
    record = Func_020018a2(25);
    Func_02001858_b(record, 0);
    v6 = 0;
    *(u8 *)(Func_020018ae(23) + 85) = v6;
    *(u8 *)(Func_020018ba(24) + 85) = v6;
    *(u8 *)(Func_020018c4(25) + 85) = v6;
    base7_20090c1 = (s32)Func_020090c1;
    Call2(Func_02001864, base7_20090c1, 0xc80);
    Func_02001862_b(1);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c8)) = 32;
    Func_020019fe_a();
    Func_02001a12_a();
    Func_02001930(0);
    Func_02001948(0, 1);
    record = Value1(Func_02001906, 0);
    if (record != 0) {
        Func_02001954(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Value1(Func_0200191a, 0);
    if (record != 0) {
        Func_02001968(2, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Value1(Func_0200192e, 0);
    if (record != 0) {
        Func_0200197c_a(3, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Call3(Func_02001972, 1, 0x318, 0x200);
    Call3(Func_02001980, 2, 0x338, 0x1f8);
    Call3(Func_02001994, 3, 0x332, 0x20c);
    Func_020019b4(1, 1);
    Func_020019bc(2, 1);
    Func_0200194a(10);
    base5_20092fc = (s32)Data_020092fc;
    Call3(Func_020019fe_b, 0, 0x1000a, base5_20092fc);
    Call3(Func_02001a08, 1, 0x1000a, base5_20092fc);
    Call3(Func_02001a12_b, 2, 0x1000a, base5_20092fc);
    Call3(Func_02001a1c_a, 3, 0x1000a, base5_20092fc);
    Call1(Func_0200197c_b, 0x12c);
    *(u8 *)(Func_02001a90() + 85) = v6;
    Call2(Func_02001a84, 0x1999, 0x333);
    Call4(Func_02001a9a, 0x3120000, 0, 0x1ae0000, 1);
    Func_020019a0(240);
    Func_020019ee(10);
    Call3(Func_02001aa2, 10, 0x102, 80);
    Call3(Func_02001a1c_b, 10, 0x333, 0x195);
    Func_020019c2(40);
    Func_02001a4a(10, 4);
    Func_020019d0(40);
    Call3(Func_02001ab4_a, 10, 0xd000, 20);
    Call1(Func_02001a92, 0x1c1e);
    Call3(Func_02001ab4_b, 0x900a, 0, 20);
    Func_02001a3a(0);
    Func_02001a40(1);
    Func_02001a46(2);
    Func_02001a4c(3);
    Call3(Func_02001b00, 11, 0x100, 40);
    Call3(Func_02001ae2, 0x200b, 0, 20);
    Func_02001aba(10, 2);
    Func_02001a28(40);
    Call3(Func_02001afa, 0x900a, 0, 10);
    Call3(Func_02001b16_a, 11, 0x5000, 10);
    Call3(Func_02001b10, 0x200b, 0, 40);
    Func_02001ae8(10, 2);
    Func_02001a56(20);
    Call3(Func_02001b28, 0x900a, 0, 20);
    Call3(Func_02001b5c, 0, 0x102, 80);
    Call3(Func_02001b68, 11, 0x106, 40);
    Call3(Func_02001b4a, 0x200b, 0, 40);
    Func_02001b1a(10, 2);
    Call3(Func_02001b86, 10, 0x102, 20);
    Func_02001b16_b(10, 4);
    Call3(Func_02001b70, 0x900a, 0, 10);
    Func_02001b40(11, 1);
    Func_02001b38(11, 3);
    Func_02001abe(20);
    Func_02001b56(10, 1);
    Func_02001b4e(10, 4);
    Func_02001b66(11, 1);
    Func_02001b5e(11, 3);
    Func_02001b76(10, 1);
    Func_02001b6e(10, 4);
    Call3(Func_02001be8, 9, 0x105, 0);
    Func_02001b98(9, 1);
    Func_02001b06(20);
    Call3(Func_02001bea, 9, 0x1000, 40);
    Func_02001bb2(9, 2);
    Func_02001b20(60);
    Func_02001bc0(9, 3);
    Func_02001b2e(40);
    Call3(Func_02001c00, 0x4009, 0, 40);
    Func_02001bb8(11, 0);
    Func_02001be0(11, 2);
    Call3(Func_02001c1a, 0x200b, 0, 10);
    Func_02001bda(9, 4);
    Func_02001bfa(9, 2);
    Call3(Func_02001c34_a, 0x4009, 0, 10);
    Call3(Func_02001c68, 10, 0x100, 20);
    Call3(Func_02001c5c, 10, 0x5000, 40);
    Func_02001c0c(10, 3);
    Call3(Func_02001c5e, 0x400a, 0, 10);
    Func_02001c1e(9, 4);
    Call3(Func_02001c82, 9, 0xd000, 10);
    Func_02001c3c(9, 2, 0);
    Func_02001c34_b(9, 4);
    Call3(Func_02001c8e, 0x4009, 0, 10);
    Call3(Func_02001cc0, 11, 0x101, 0);
    Call3(Func_02001cca_a, 10, 0x101, 40);
    Call3(Func_02001cbe, 10, 0xd000, 80);
    Call3(Func_02001cca_b, 10, 0x5000, 60);
    Func_02001c8a(10, 2);
    Func_02001c92_a(11, 2);
    Call11(Func_02001d4c, 10, 11, 6, 6, 6, 11, 12, 1, 7, 1, v6);
    Func_02001c72(20);
    Call2(Func_02001d72, 0x19999, 0x3333);
    Call4(Func_02001d88, 0x3090000, 0, 0x1d40000, 1);
    Func_02001d94();
    Func_02001c92_b(40);
    Func_02001d1a(1, 3);
    Call3(Func_02001d6c, 0x1001, 0, 20);
    Func_02001d44_a(8, 2);
    Func_02001c7a(base7_20090c1);
    Func_02001cb8(40);
    Func_02001d40(8, 6);
    Func_02001cc6(20);
    Call3(Func_02001d98, 0x4008, 0, 20);
    Call4(Func_02001ddc, 0x2ee0000, 0, 0x1c30000, 1);
    Func_02001ce2(20);
    Call3(Func_02001dc6, 11, 0x5000, 0);
    Call3(Func_02001dd2, 10, 0x5000, 10);
    Call3(Func_02001dde, 8, 0x1000, 40);
    Call3(Func_02001e02_a, 8, 0x100, 40);
    Call3(Func_02001df6, 8, 0x3000, 20);
    Call3(Func_02001e02_b, 8, 0x1000, 20);
    Call3(Func_02001e0e, 8, 0x3000, 40);
    Func_02001dbe(8, 6);
    Func_02001d44_b(60);
    Func_02001dd6(8, 6, 0);
    Call3(Func_02001e20, 0x4008, 0, 20);
    Call3(Func_02001d9a, 1, 0x19999, 0xcccc);
    Call3(Func_02001dcc, 1, 0x315, 0x1d9);
    Call3(Func_02001e50, 1, 0x7000, 20);
    Func_02001e00(1, 3);
    Call3(Func_02001e52_a, 0x4001, 0, 10);
    Call3(Func_02001e6e, 8, 0x1000, 20);
    Func_02001e1e(8, 3);
    Value2(Func_02001e5e, 0x4008, 0);
    Call3(Func_02001e8a, 10, 0x5000, 0);
    Call3(Func_02001e96, 9, 0x1000, 0);
    Call3(Func_02001ea2, 1, 0x3000, 0);
    Call3(Func_02001eae, 2, 0x7000, 0);
    Call3(Func_02001eba, 3, 0xb000, 0);
    if (Value2(Func_02001e12_a, 0, 0) == 1) {
        bump_step(1);
    }
    Call4(Func_02001f0c, 0x3090000, 0, 0x1ac0000, 1);
    ((void (*)())Func_02001e12_b)(20);
    Func_02001eb2(10, 2);
    Func_02001ee2(10, 0);
    Func_02001eaa(11, 4);
    Func_02001e30(20);
    Call1(Func_02001ee6_a, 0x1c33);
    Call2(Func_02001efe, 0x200b, 0);
    Call4(Func_02001f4c, 0x3090000, 0, 0x1d40000, 1);
    Func_02001e52_b(20);
    Call3(Func_02001f36, 1, 0xd000, 20);
    Func_02001ee6_b(1, 3);
    Func_02001e6c(20);
    Func_02001ef4(9, 4);
    Call3(Func_02001f58_a, 9, 0xd000, 10);
    Call2(Func_02001f48_a, 0x4009, 0);
    Func_02001f10(8, 3);
    Call2(Func_02001f58_b, 0x4008, 0);
    Call3(Func_02001f7c, 1, 0x7000, 10);
    Func_02001f2c(1, 3);
    Call3(Func_02001f90, 9, 0x1000, 10);
    Func_02001f38_a(11, 3);
    Func_02001f40(10, 3);
    Func_02001f48_b(9, 3);
    Func_02001f58_c(8, 3);
    Func_02001ede(20);
    Call3(Func_02001fc2, 1, 0x3000, 20);
    Call3(Func_02001fe6, 1, 0x102, 80);
    Call3(Func_02001fda, 1, 0x7000, 20);
    Call3(Func_02001fd4, 0x4001, 0, 20);
    Call3(Func_02001ff0, 1, 0x3000, 10);
    Func_02001ffa(0, 0, 40);
    Func_02001fa2(0, 3);
    Func_02001fb2(1, 3);
    Func_02001f38_b(20);
    Call3(Func_0200201c, 0, 0x4000, 20);
    Func_02001fcc(3, 3);
    Func_02001f52(20);
    Call3(Func_02002036, 1, 0x1000, 0);
    Call3(Func_02002042, 0, 0xe000, 0);
    Call3(Func_02001fb0, 2, 0x10000, 0x8000);
    Call3(Func_02001fe2, 2, 0x333, 0x1e9);
    Call3(Func_02002066, 2, 0xb000, 40);
    Func_0200202e(2, 2);
    Func_02002068(2, 0, 20);
    Func_02002028_a(2, 3);
    Func_02002028_b(8, 3);
    Func_02002030(9, 3);
    Func_02002038(10, 3);
    Func_02002048(9, 3);
    *(u8 *)(Func_02001ffe(3) + 35) &= 254;
    Func_020020c8_a(3, 1);
    Call3(Func_02002026, 3, 0x10000, 0x8000);
    Call3(Func_0200205a, 3, 0x31a, 0x208);
    Call3(Func_020020de, 1, 0x5000, 0);
    Call3(Func_020020ea, 0, 0xa000, 0);
    Call3(Func_02002080, 3, 0x310, 0x1f0);
    Call3(Func_02002104, 3, 0x9000, 10);
    *(u8 *)(Func_02002062(3) + 35) |= 1;
    Func_0200210e(3, 0, 20);
    Func_020020c6(8, 3);
    Func_020020ce(9, 3);
    Func_020020d6(10, 3);
    Func_0200212e(9, 3);
    Func_020020b4(20);
    Call4(Func_020021c2_a, 0x3090000, 0, 0x1ac0000, 1);
    Func_020020c8_b(20);
    Call3(Func_0200210a, 11, 0x6666, 0x3333);
    Call3(Func_0200213e, 11, 0x343, 0x184);
    Call3(Func_020021c2_b, 11, 0x5000, 0);
    Call3(Func_020021e6, 11, 0x108, 40);
    Call3(Func_020021c8, 0x200b, 0, 20);
    Call4(Func_0200220e, 0x3090000, 0, 0x1d40000, 1);
    Func_02002114(40);
    Call3(Func_020021f8, 2, 0x7000, 0);
    Call3(Func_02002204, 3, 0xf000, 40);
    Call3(Func_02002210, 2, 0x9000, 0);
    Call3(Func_0200221c, 3, 0xd000, 20);
    Func_020021c4(2, 3);
    Func_020021d4(3, 3);
    Func_0200215a(20);
    Func_020021fa(10, 1);
    Func_02002168(20);
    Func_020021e8(10, 3);
    Func_02002242(10, 0, 20);
    Call3(Func_0200225e, 0, 0xc000, 0);
    Call3(Func_0200226a, 1, 0xd000, 0);
    Call3(Func_02002276, 2, 0xb000, 0);
    Call3(Func_02002282, 3, 0xd000, 40);
    Func_0200222a(0, 3);
    Func_02002232_a(1, 3);
    Func_0200223a(2, 3);
    Func_0200224a(3, 3);
    Func_020021d0(20);
    Call3(Func_02002216_a, 2, 0x10000, 0x8000);
    base5_2009400 = (s32)Data_02009400;
    Func_02002228(1, base5_2009400);
    Value2(Func_02002230, 2, base5_2009400);
    Value2(Func_02002248, 3, base5_2009400);
    Value2(Func_02002240, 10, 0x2009310);
    Call3(Func_0200226c, 11, 0x345, 0x178);
    Call3(Func_020022f0, 11, 0xd000, 20);
    Call1(Func_02002216_b, 0x81d);
    Func_02002232_b();
}

void Scene_UpdateTimedActor(void)
{
    u32 Func_0200220e_a(u32, u32);

    s32 no;
    u32 phase;
    union SceneActor *actor;
    s32 *other;

    phase = Func_0200220e_a(Data_03001e40, 180);
    no = 23;
    switch (phase) {
    case 10:
        break;
    case 20:
        no = 24;
        break;
    case 30:
        no = 25;
        break;
    default:
        return;
    }
    actor = Func_020022b0(no);
    if (actor == NULL) {
        return;
    }
    other = Func_020022bc(8);
    if (other != NULL) {
        Func_0200230a(no, other[2], other[4]);
    }
    actor->words[6] = 0x6666;
    actor->words[7] = 0x6666;
    {
        s32 y = actor->words[3] + 0x180000;
        union SceneField *dst = (union SceneField *)(actor->halfwords + 50);
        s32 value;
        actor->words[3] = y;
        actor->words[15] = y;
        value = 25;
        dst->value = value;
        dst++;
        value = 128;
        dst->value = value;
    }
    Func_02002302(no, Data_02009440);
}
