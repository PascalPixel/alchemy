#include "TYPES.H"
#include "FIELD_EVENT.H"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define OverlayObject_PrepareObject      Func_02000048
#define AcquireOverlayObject      Func_0200491e
#define CreateOverlayObject Func_02004976
#define SetOverlayObjectMode Func_020049d8
#define SetOverlayObjectSlot Func_02004b30
#define CalculateAngleFromCoordinateDelta Func_02004bb2
void Effect_Move(void *object);
#define Scene_GetRecord_1(args...) Func_02005794(args)

#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"
#include "OVERLAY_OBJECT.H"

typedef struct {
    u8 pad[9];
    u8 f0 : 2;
    u8 f1 : 2;
    u8 f2 : 4;
} S;

union MotionWork {
    struct {
        u32 unk_00[2];
        s32 x, y, z;
        u32 unk_14;
        s32 accum_x, accum_y;
        u32 unk_20[4];
        s32 rate_x, rate_y;
        u32 unk_38[3];
        s32 velocity_x, velocity_y, velocity_z;
        u16 *record;
        u8 unk_54[16];
        u16 angle_step;
    } fields;
    u8 bytes[102];
};

extern s32 Data_0200cd88[];
extern s32 Data_0200cdb8[];
extern s32 Data_0200cdc4[];
extern s32 Data_0200cfa4[];
extern u8 Data_00001076[];
extern u8 Data_03001ebc[];
extern u8 Data_000010b0[];
extern u8 Data_000010b2[];
extern u8 LinkedMessage_AlexAsksForStars;
extern u8 Data_00004009[];
extern u8 Data_00008009[];
extern s32 Func_02008cd0(s32 arg0);                         /* 0x02004352 -> 08077248 */
extern s32 Func_02008cda(s32 arg0);                         /* 0x0200435c -> 08077248 */
extern s32 Func_02008e80(s32 *out_first, s32 *out_second);  /* 0x02004372 -> 080b0058 */
extern void Func_02008d08(s32 arg0, s32 arg1);              /* 0x02004382 -> 080772b0 */
extern void Func_02008cd8(s32 arg0);                        /* 0x0200438a -> 08077030 */
extern void Func_02008cde(s32 arg0);                        /* 0x02004390 -> 08077030 */
extern void Func_02008ce4(s32 arg0);                        /* 0x02004396 -> 08077030 */
extern void Func_02008cea(s32 arg0);                        /* 0x0200439c -> 08077030 */

void *Func_0200491e(s32, s32, s32, s32);
u16 Func_02004bb2(s32, s32);
s32 Func_02004bc6();
void Func_02004c2c();
void Func_0200529a();
void Func_02001504(void);
void Func_020017cc(void);
void Func_02001a00(void);
void Func_02001ec8(void);
void Func_02002208(void);
void Func_02002784(void);
void Func_02002d0a(void);
s32 Func_02004f5c();
void Func_02005630_b();
u8 *Func_02005794();
void Func_02005a3e();
void Func_02005a70();
void Func_02005a9a();
void Func_02005abe();
void Func_02005adc();
void Func_02005af4();
void Func_02005b04();
void Func_02005b1a();
void Func_02005b46();
void Func_02005b56();
void Func_02005b6c();
void Func_02005b84();
void Func_02005b94();
void Func_02005ba4();
void Func_02005bde();
void Func_02005d08();
void Func_02005d3c();
void Func_02005d62();
void Func_02005e0e();
s32 Func_02006162();
s32 Func_020063a2();
void Func_02006410();
void Func_020064b8();
s32 Func_020064fa_b();
s32 Func_020065d6();
void Func_02005ff6();
void Func_02005ffe();
void Func_02006054();
void Func_020060be();
void Func_020060d8();
void Func_02006104();
void Func_02006156();
void Func_02006174();
void Func_02006184();
void Func_02006196();
void Func_020061f8();
void Func_020062de();
void Func_020062f4();
s32 Func_020066e4();
void Func_02006742();
s32 Func_020067e6();
s32 Func_02006882();
s32 Func_0200688e();
void Func_020069a6();
s32 Func_02006a9a();
s32 Func_02006ad6();
void Func_02006bd2();
void Func_02006bd8();
s32 Func_02006bda();
void Func_02006bde();
s32 Func_020066f4();
void Func_02006dc8_a();
s32 Func_02006eba_b();
void Func_020056e4();
void Func_02005704();
void Func_0200574c();
void Func_020057c4();
void Func_0200593e_a();
void Func_0200595e();
void Func_02005974();
void Func_020059a4();
void Func_02006a26(s32 a, s32 b);
void Func_020070ae();
void Func_02007172();
void Func_020071a4();
void Func_020071b8();
void Func_020071f4();
struct ObjectRuntime *Func_020071f6(u32);
struct ObjectRuntime *Func_02007204(u32);
u8 *Func_02007308();
struct ObjectRuntime *Func_02007372_b(u32);
struct ObjectRuntime *Func_02007386(u32);
void Func_020073a6();
void Func_020073bc();
void Func_020073ee();
void Func_02007468();
void Func_02007496();
void Func_020074a6();
void Func_020074b6();
void Func_020074ce();
struct ObjectRuntime *Func_02007622(u32);
u8 *Func_02007766_a();
u8 *Func_02007766_b();
u8 *Func_02007766_c();
void Func_02007834_b();
void Func_0200785a();
void Func_0200786a_b();
void Func_020078da();
void Func_02007902();
void Func_02007938_b();
void Func_020079c4_b();
void Func_020079da();
void Func_02007a00();
void Func_02007a3a();
void Func_02007a74();
void Func_02007a96();
void Func_02007b4c();
void Func_02007c24();
void Func_02007ca8();
void Func_02007d5a();
void Func_02007d76();
void Func_02007da0();
void Func_02007e24();
void Func_02007f12();
void Func_02007f78();
void Func_02007fb4();
void Func_02007fe0();
void Func_02007ffc();
void Func_02008036_b();
void Func_02008064();
void Func_020081da();
struct ObjectRuntime *Func_020082c6(u32);
s32 Func_0200883c();
void Func_0200885a();

/* Overlay-local import veneers, retained per call site. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/*
 * resource_380: the talk handler for actor 9, one of a bank of per-actor
 * published callbacks. The handler is published through a script record
 * rather than called, so nothing here reaches it by name.
 */

/*
 * resource_380: the talk handler for actor 5, published through a script
 * record rather than called.
 */

/*
 * Overlay resource_380: the per-actor talk handlers of the party
 * introduction scene.
 */

/*
 * Callback for resource_380 that forwards to Engine_LeaderCheckAhead and nothing else.
 * A script record holds its Thumb pointer; nothing calls it directly. The
 * twelve-byte owner includes the zero alignment halfword after the return and
 * has no literal pool. Engine_LeaderCheckAhead takes no arguments -- nothing is loaded
 * into r0 before the call.
 */

/*
 * resource_380 owner at 0x02004248, 24 bytes including the trailing
 * alignment halfword: a two-call convenience wrapper.  Both Func_ names are
 * the overlay's import-veneer entries for main-ROM code, not runtime
 * addresses.  The callees' roles beyond call shape remain open.
 */

/* One symbol per call site, named at that site's call word rather than at a
 * runtime address. A callee reached from several sites therefore appears
 * several times here under different names. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Inline calls keep argument constants local; direct calls permit sharing. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call2_02000d5c(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call1_02002400(void (*f)(), s32 a0)
{
    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02001450(void (*f)(), s32 a0)
{
    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_020027f8(void (*f)(), s32 a0)
{
    f(a0);
}

struct ObjectRuntime;

void OverlayObject_SetHandleFlagBits2And3(s32 obj, u32 v)
{
    S *h = *(S **)(obj + 0x50);
    h->f1 = v;
}

void *OverlayObject_PrepareObject(s32 first, s32 second, s32 third, s32 fourth)
{
    void *obj;
    void *rec;
    s32 mask;

    obj = AcquireOverlayObject(fourth, first, second, third);
    if (obj != NULL) {
        rec = FIELD_AT_OFFSET(obj, void *, 0x50);
        mask = -0xD;
        FIELD_AT_OFFSET(rec, u8, 9) = (u8)(mask & FIELD_AT_OFFSET(rec, u8, 9));
        FIELD_AT_OFFSET(obj, u8, 0x55) = 0;
        FIELD_AT_OFFSET(obj, u8, 0x59) = 8;
        Actor_SetSpriteFlags(obj, 0);
        Object_SetPalette(obj, 0xE);
        Object_SetBlendMode(obj, 1);
        return obj;
    }
    return NULL;
}

void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "CREATE_CONFIGURED_OVERLAY_OBJECT_BODY.INC"
}

s32 UpdateOverlayObjectAngle(struct OverlayObject *object)
{
    struct OverlayObject *linked_object = object->linked_object;
    if (linked_object != NULL) {
        s32 angle_delta;
        u16 angle;
        object->unknown_5a = object->unknown_5a & 0xFE;
        angle_delta = CalculateAngleFromCoordinateDelta(
            linked_object->coordinate_10 - object->coordinate_10,
            linked_object->coordinate_08 - object->coordinate_08);
        angle = object->angle;
        angle_delta -= angle;
        angle_delta <<= 16;
        angle_delta >>= 16;
        if (angle_delta != 0) {
            if (angle_delta > 0x1000) {
                angle_delta = 0x1000;
            }
            if (angle_delta < -0x1000) {
                angle_delta = -0x1000;
            }
            object->angle = angle + angle_delta;
        }
    }
    return 1;
}

s32 *SceneData_GetTableCd88(void)
{
    return Data_0200cd88;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

s32 *SceneData_GetTableCDB8(void)
{
    return Data_0200cdb8;
}

s32 *SceneData_GetTableCdc4(void)
{
    return Data_0200cdc4;
}

s32 *SceneData_GetTableCFA4(void)
{
    return Data_0200cfa4;
}

void Func_020006f4(void)
{
    u32 i;
    s32 obj;
    s32 mes;

    Event_Begin();
    Audio_PlayCue(141);
    for (i = 0; i != 6; i++) {
        ColorBuffer_ApplyTarget(0x404a4e, 1);
        ColorBuffer_Interpolate(8);
        Event_Wait(8);
        ColorBuffer_ApplyTarget(0x10000, 1);
        ColorBuffer_Interpolate(8);
        Event_Wait(8);
        if (i == 1) {
            Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
        }
    }
    Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
    Event_Wait(30);
    Work_SetValuesIfNonNegative(0, 0, 0);
    Camera_SetSpeed(0x59999, 0xb333);
    Camera_MoveTo(0x1d80000, -1, 0x620000, 1);
    Camera_WaitForMove();
    Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
    Event_Wait(20);
    Audio_PlayCue(144);
    Engine_MapAnimateCells(0x200d088, 84, 4);
    Map_CopyCellAttributes(0, 0, 1, 1, 29, 4);
    Map_CopyCellsTo(87, 42, 29, 6, 1, 2);
    Event_Wait(40);
    Work_SetValuesIfNonNegative(0, 0, 0);
    Camera_MoveTo(0x1570000, -1, 0x1710000, 1);
    Camera_WaitForMove();
    Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
    Event_Wait(20);
    Audio_PlayCue(144);
    Engine_MapAnimateCells(0x200d088, 76, 21);
    Map_CopyCellAttributes(0, 0, 1, 1, 21, 21);
    Map_CopyCellsTo(87, 42, 21, 23, 1, 2);
    Event_Wait(40);
    Work_SetValuesIfNonNegative(0, 0, 0);
    Camera_SetSpeed(0x33333, 0x6666);
    Camera_MoveTo(0x1570000, -1, 0x1f10000, 1);
    Camera_WaitForMove();
    Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
    Event_Wait(20);
    Audio_PlayCue(144);
    Engine_MapAnimateCells(0x200d088, 76, 29);
    Map_CopyCellAttributes(0, 0, 1, 1, 21, 29);
    Map_CopyCellsTo(87, 42, 21, 31, 1, 2);
    Event_Wait(40);
    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x202;
    Event_CloseScreen();
    Event_WaitForScreen();
    Camera_MoveTo(0x2c80000, -1, 0x980000, 0);
    Map_Redraw();
    Task_Wait(1);
    Work_SetValuesIfNonNegative(0x20000, 0x10000, 0x10000);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    MapRender_WaitForValues();
    Event_Wait(20);
    Map_CopyCellsTo(0, 40, 43, 46, 3, 3);
    Event_Wait(20);
    obj = Func_02004bc6(221, 0x2c80000, 0x100000, 0x900000);
    Event_Wait(40);
    Func_0200529a(obj, 1);
    mes = (s32)Data_00001076;
    Value2(Engine_MessageShowCentered, mes, 1);
    Actor_FaceDirection(9, 0xe000, 0);
    Actor_FaceDirection(5, 0xe000, 20);
    Event_CloseScreen();
    Event_WaitForScreen();
    Camera_MoveTo(0x1ce0000, -1, 0x15e0000, 0);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_Jump(9, 4, 30);
    Event_SetMessage(mes - 2);
    Func_02004c2c(9, 20);
    Event_CloseScreen();
    Event_WaitForScreen();
    Camera_MoveTo(0x2c80000, -1, 0x980000, 0);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    GameFlag_Set(0x83d);
    Event_End();
}

void FieldScene_RunSevenSceneChain(void)
{
    Event_Begin();
    Func_02001504();
    Func_020017cc();
    Func_02001a00();
    Func_02001ec8();
    Func_02002208();
    Func_02002784();
    GameFlag_Set(0x83E);
    Event_End();
    Func_02002d0a();
}

void Func_02000a98(void)
{
    u32 i;
    s32 obj;

    Audio_PlayCue(141);
    for (i = 0; i != 6; i++) {
        ColorBuffer_ApplyTarget(0x4049d2, 1);
        ColorBuffer_Interpolate(8);
        Event_Wait(8);
        ColorBuffer_ApplyTarget(0x10000, 1);
        ColorBuffer_Interpolate(8);
        Event_Wait(8);
        if (i == 1) {
            Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
        }
    }
    Work_SetValuesIfNonNegative(0x20000, 0x10000, 0x10000);
    Event_Wait(30);
    Camera_SetSpeed(0x26666, 0x4ccc);
    Camera_MoveTo(0xa70000, -1, 0x2110000, 1);
    Camera_WaitForMove();
    Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
    Event_Wait(20);
    Audio_PlayCue(144);
    Engine_MapAnimateCells(0x200d088, 65, 31);
    Map_CopyCellAttributes(0, 0, 1, 1, 10, 31);
    Map_CopyCellsTo(87, 42, 10, 33, 1, 2);
    Event_Wait(40);
    Work_SetValuesIfNonNegative(0, 0, 0);
    Camera_SetSpeed(0x66666, 0xcccc);
    Camera_MoveTo(0x1870000, -1, 0xb10000, 1);
    Camera_WaitForMove();
    Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
    Event_Wait(20);
    Audio_PlayCue(144);
    Engine_MapAnimateCells(0x200d088, 79, 9);
    Map_CopyCellAttributes(0, 0, 1, 1, 24, 9);
    Map_CopyCellsTo(87, 42, 24, 11, 1, 2);
    Event_Wait(40);
    Work_SetValuesIfNonNegative(0, 0, 0);
    Camera_SetSpeed(0x26666, 0x4ccc);
    Camera_MoveTo(0x2470000, -1, 0xc10000, 1);
    Camera_WaitForMove();
    Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
    Event_Wait(20);
    Audio_PlayCue(144);
    Engine_MapAnimateCells(0x200d088, 91, 10);
    Map_CopyCellAttributes(0, 0, 1, 1, 36, 10);
    Map_CopyCellsTo(87, 42, 36, 12, 1, 2);
    Event_Wait(40);
    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x202;
    Event_CloseScreen();
    Event_WaitForScreen();
    Camera_MoveTo(0xe80000, -1, 0x1dd0000, 0);
    Map_Redraw();
    Task_Wait(1);
    Work_SetValuesIfNonNegative(0x20000, 0x10000, 0x10000);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    MapRender_WaitForValues();
    Event_Wait(20);
    Map_CopyCellsTo(0, 40, 13, 66, 3, 3);
    Event_Wait(20);
    obj = Value4(Func_02004f5c, 223, 0xe80000, 0x100000, 0x1d00000);
    Event_Wait(40);
    Func_02005630_b(obj, 1);
    Message_ShowCentered(0x1077, 1);
}

/* Runs a sequence of position/scale/timing calls for actor pair 0 and 1,
 * copying a stored pair of 32-bit fields (offsets +8, +16) from actor 0's
 * record onto actor 1 partway through, then runs an analogous sequence for
 * actors 5, 9, 10 and 11. */
void FieldScene_StagePairedActors(void)
{
    u32 i;
    u8 *record;

    Audio_PlayCue(17);
    Actor_SetSpeed(0, 0x8000, 0x4000);
    Actor_WalkToAndWait(0, 231, 0x1ea);
    Actor_FaceDirection(0, 0xc000, 30);
    Actor_SetAnimationAndWait(0, 3);
    Event_Wait(180);
    Actor_RunRepeatedMotion(0, 2);
    Event_Wait(80);
    Actor_ShowEmote(0, 0x101, 60);
    Actor_SetSpeed(0, 0x8000, 0x4000);
    Actor_WalkToAndWait(0, 246, 0x1df);
    Actor_FaceDirection(0, 0xe000, 10);
    /* Copy actor 0's stored fields at +8 and +16 onto actor 1, if a record
     * for actor 0 exists. */
    record = Scene_GetRecord_1(0);
    if (record != 0) {
        Actor_SetPosition(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(1, 0x10000, 0x8000);
    Actor_WalkToAndWait(1, 0x101, 0x1eb);
    Actor_FaceDirection(0, 0x2000, 0);
    Actor_FaceDirection(1, 0xa000, 40);
    Actor_StartRepeatedMotion(0, 2);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(20);
    Actor_ShowEmote(0, 0x101, 0);
    Actor_ShowEmote(1, 0x101, 80);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimationAndWait(1, 4);
    Actor_SetSpeed(0, 0x13333, 0x9999);
    Actor_SetSpeed(1, 0x13333, 0x9999);
    Actor_WalkTo(0, 0x109, 0x1c5);
    Actor_WalkToAndWait(1, 0x11a, 0x1d5);
    Actor_SetAnimation(0, 1);
    Actor_FaceDirection(0, 0xe000, 0);
    Actor_FaceDirection(1, 0xe000, 40);
    Actor_ShowEmote(0, 0x100, 0);
    Actor_ShowEmote(1, 0x100, 0);
    Actor_Jump(0, 6, 0);
    Actor_Jump(1, 6, 60);
    Actor_SetPosition(5, 0x1db0000, 0x14c0000);
    Actor_SetPosition(9, 0x1eb0000, 0x14c0000);
    Actor_SetPosition(11, 0x1cb0000, 0x15c0000);
    Actor_SetPosition(10, 0x1fb0000, 0x15c0000);
    Camera_SetSpeed(0x73333, 0xe666);
    Camera_MoveTo(0x1e50000, -1, 0x1590000, 1);
    Actor_FaceDirection(5, 0x6000, 0);
    Actor_FaceDirection(9, 0x5000, 0);
    Actor_FaceDirection(11, 0x5000, 0);
    Actor_FaceDirection(10, 0x5000, 0);
    Camera_WaitForMove();
    Event_Wait(40);
}

void RunEventScript01(void)
{
    u32 i;
    s32 record;

    Actor_FaceDirection(5, 0x6000, 0);
    Actor_FaceDirection(9, 0x5000, 0);
    Actor_FaceDirection(10, 0x8000, 0);
    Actor_FaceDirection(12, 0x5000, 40);
    Actor_FaceDirection(11, 0xd000, 40);
    Actor_RunRepeatedMotion(11, 2);
    Func_020056e4(11, 20);
    Actor_RunRepeatedMotion(12, 2);
    Actor_SetAttachedEffect(12, 0x102);
    Event_Wait(60);
    Func_02005704(12, 10);
    Actor_FaceActor(10, 12, 0);
    Actor_FaceActor(5, 12, 0);
    Actor_FaceActor(9, 12, 0);
    Event_Wait(40);
    Actor_RunRepeatedMotion(10, 1);
    Actor_FaceDirection(10, 0x8000, 10);
    Actor_SetAnimation(10, 3);
    Func_0200574c(10, 10);
    Actor_FaceDirection(11, 0, 10);
    Actor_SetAnimationAndWait(11, 3);
    Event_Wait(10);
    Actor_FaceDirection(5, 0, 0);
    Actor_FaceDirection(9, 0x8000, 20);
    Actor_StartRepeatedMotion(5, 2);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(10);
    Actor_FaceDirection(5, 0xc000, 0);
    Actor_FaceDirection(9, 0xb000, 10);
    Actor_FaceDirection(10, 0xb000, 40);
    Actor_SetAnimationAndWait(10, 3);
    Func_020057c4(10, 10);
    Actor_ShowEmote(5, 0x100, 20);
    Actor_SetAttachedEffect(12, 0x102);
    Actor_RunRepeatedMotion(12, 3);
    Event_Wait(40);
    Actor_SetPosition(1, 0x15a0000, 0x1b80000);
    Task_Wait(1);
    Event_ShowMessage(1, 0);
    Actor_FaceDirection(0, 0x2000, 0);
    Actor_FaceDirection(1, 0xa000, 20);
    Event_CloseScreen();
    Event_WaitForScreen();
    Actor_SetPosition(1, 0x1180000, 0x1d60000);
    Camera_MoveTo(0x1050000, -1, 0x1d20000, 0);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_FaceDirection(0, 0x3000, 10);
    Actor_ShowEmote(1, 0x102, 0);
    Actor_RunRepeatedMotion(1, 3);
    Event_AskYesNo(1, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(1, 4);
    Event_Wait(10);
    Event_AskYesNo(1, 0);
    Event_Wait(10);
    Event_SetMessage(0x109b);
    Event_ShowMessage(11, 0);
    Actor_FaceDirection(11, 0xd000, 10);
    Actor_StartRepeatedMotion(0, 2);
    Actor_RunRepeatedMotion(0, 2);
    Event_Wait(10);
    Actor_FaceDirection(0, 0xe000, 0);
    Actor_FaceDirection(1, 0xe000, 20);
    Event_CloseScreen();
    Event_WaitForScreen();
    Camera_MoveTo(0x1dd0000, -1, 0x14e0000, 0);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(10);
    Actor_FaceDirection(9, 0x8000, 30);
    Func_0200593e_a(9, 20);
    Actor_FaceDirection(5, 0, 40);
    Actor_SetAnimationAndWait(5, 4);
    Event_Wait(20);
    Func_0200595e(5, 10);
    Actor_RunRepeatedMotion(12, 2);
    Event_Wait(80);
    Func_02005974(12, 20);
    Actor_RunRepeatedMotion(5, 2);
    Actor_FaceDirection(5, 0xc000, 0);
    Actor_FaceDirection(9, 0xb000, 20);
    Actor_SetAnimationAndWait(12, 3);
    Func_020059a4(12, 20);
    Actor_StartRepeatedMotion(5, 2);
    Actor_RunRepeatedMotion(9, 2);
    Actor_FaceDirection(12, 0xb000, 40);
}

void Func_0200178c(void)
{
    u8 *obj;
    s32 other;
    s32 tbl;
    s32 left;
    s32 cnt;
    s32 mes_a;
    s32 mes_b;

    Audio_PlayCue(161);
    Actor_RunRepeatedMotion(12, 3);
    Event_Wait(40);
    other = Value1(Func_02006162, 12);
    if (other != 0) {
        Actor_SetPosition(13, *(s32 *)(other + 8), *(s32 *)(other + 16));
    }
    Actor_SetPosition(12, 0, 0);
    Event_Wait(20);
    Actor_FaceDirection(13, 0x3000, 40);
    Actor_RunRepeatedMotion(5, 3);
    Actor_RunRepeatedMotion(5, 3);
    Event_Wait(40);
    Actor_RunRepeatedMotion(5, 2);
    Func_02005a3e(5, 20);
    Actor_SetAnimationAndWait(13, 3);
    Event_Wait(10);
    Actor_StartRepeatedMotion(5, 3);
    Actor_FaceDirection(9, 0x8000, 10);
    Actor_RunRepeatedMotion(9, 2);
    Func_02005a70(9, 40);
    Actor_SetAnimationAndWait(5, 3);
    Event_Wait(40);
    Actor_FaceDirection(9, 0xb000, 40);
    Actor_RunRepeatedMotion(13, 2);
    Func_02005a9a(13, 20);
    Actor_RunRepeatedMotion(5, 1);
    Event_Wait(40);
    Actor_SetAnimationAndWait(13, 3);
    Event_Wait(10);
    Func_02005abe(13, 40);
    Actor_StartRepeatedMotion(10, 1);
    Actor_SetAnimationAndWait(10, 3);
    Event_Wait(10);
    Func_02005adc(10, 10);
    Actor_RunRepeatedMotion(11, 1);
    Actor_SetAnimation(11, 3);
    Func_02005af4(11, 80);
    Actor_RunRepeatedMotion(13, 2);
    Func_02005b04(13, 40);
    Actor_RunRepeatedMotion(5, 2);
    Event_Wait(10);
    Func_02005b1a(5, 10);
    Actor_StartRepeatedMotion(13, 2);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(80);
    Actor_SetAnimationAndWait(5, 4);
    Event_Wait(20);
    Func_02005b46(5, 80);
    Actor_SetAnimationAndWait(13, 4);
    Func_02005b56(13, 80);
    Actor_RunRepeatedMotion(5, 2);
    Event_Wait(4);
    Func_02005b6c(5, 20);
    Actor_RunRepeatedMotion(10, 1);
    Actor_SetAnimation(10, 3);
    Func_02005b84(10, 10);
    Actor_RunRepeatedMotion(11, 1);
    Func_02005b94(11, 10);
    Actor_RunRepeatedMotion(10, 1);
    Func_02005ba4(10, 10);
    Actor_FaceDirection(9, 0x3000, 80);
    Actor_ShowEmote(9, 0x105, 80);
    Actor_RunRepeatedMotion(11, 1);
    Actor_FaceDirection(11, 0x5000, 40);
    Actor_StartRepeatedMotion(11, 2);
    Func_02005bde(11, 20);
    Event_CloseScreen();
    Event_WaitForScreen();
    Camera_MoveTo(0x1050000, -1, 0x1d20000, 0);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_FaceDirection(0, 0x3000, 0);
    Actor_FaceDirection(1, 0xb000, 20);
    Actor_StartRepeatedMotion(0, 2);
    Actor_RunRepeatedMotion(1, 2);
    Actor_SetSpeed(0, 0x8000, 0x4000);
    Actor_SetSpeed(1, 0x8000, 0x4000);
    Actor_WalkTo(0, 244, 0x1de);
    Actor_WalkToAndWait(1, 0x104, 0x1ea);
    Actor_WaitForMove(0);
    Actor_SetAnimation(0, 1);
    Actor_FaceDirection(0, 0x3000, 0);
    Actor_FaceDirection(1, 0xb000, 20);
    Actor_RunRepeatedMotion(0, 2);
    Actor_SetAnimationAndWait(1, 4);
    Event_Wait(20);
    tbl = Func_020063a2(1);
    cnt = 0;
    tbl += 216;
    left = 14;
    do {
        u32 id = *(u16 *)(tbl)& 0x1ff;
        tbl = (tbl + 2);
        if (id == 220 || id == 221 || id == 223)
            cnt++;
        left--;
    } while (left >= 0);
    Event_OpenMessage(1, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        mes_a = (s32)Data_000010b0;
        Event_SetMessage(mes_a);
        Actor_SetAnimationAndWait(1, 3);
        Event_Wait(10);
        if (cnt <= 2) {
            Func_02005d08(1, 30);
            Actor_WalkToAndWait(1, 252, 0x1e6);
            Actor_RunRepeatedMotion(1, 2);
            Event_Wait(10);
            Func_02006410((mes_a + 1), 1, 0);
        } else {
            Event_SetMessage(0x10b4);
            Func_02005d3c(1, 30);
        }
    } else {
        if (cnt <= 2) {
            mes_b = (s32)Data_000010b2;
            Event_SetMessage(mes_b);
            Actor_RunRepeatedMotion(1, 3);
            Actor_SetAnimationAndWait(1, 4);
            Func_02005d62(1, 10);
            Actor_SetAnimationAndWait(1, 4);
            Actor_RunRepeatedMotion(1, 1);
            Actor_SetSpeed(1, 0x20000, 0x10000);
            obj = Value1(Func_020064fa_b, 0);
            obj[90] &= 254;
            Actor_WalkToAndWait(1, 244, 0x1de);
            Actor_SetSpeed(0, 0x18000, 0xc000);
            Actor_Jump(0, 6, 0);
            Actor_SetDestination(0, 218, 0x1d7);
            Actor_WaitForMove(0);
            Func_020064b8((mes_b + 1), 1, 0);
            Actor_RunRepeatedMotion(0, 2);
            Actor_FaceDirection(0, 0, 30);
            {
                u8 flags = obj[90] | 1;

                obj[90] = flags;
            }
        } else {
            Event_SetMessage(0x10b5);
            Actor_RunRepeatedMotion(1, 3);
            Actor_SetAnimationAndWait(1, 4);
            Func_02005e0e(1, 10);
            Actor_SetAnimationAndWait(1, 4);
            Actor_FaceDirection(0, 0xe000, 30);
        }
    }
    Camera_SetSpeed(0x8000, 0x1000);
    Camera_FollowActor(1, 1);
    Camera_WaitForMove();
    Actor_FaceDirection(1, 0x8000, 30);
    Actor_RunRepeatedMotion(1, 2);
    Actor_SetSpeed(1, 0x8000, 0x4000);
    obj = Value1(Func_020065d6, 1);
    obj[90] &= 254;
    Actor_WalkToAndWait(1, 0x108, 0x1e2);
    {
        u8 flags = obj[90] | 1;

        obj[90] = flags;
    }
    Actor_WalkToAndWait(1, 0x116, 0x1e0);
    *(s32 *)(obj + 48) = 0x30000;
    *(s32 *)(obj + 52) = 0x20000;
    Audio_PlayCue(153);
    *(s32 *)(obj + 40) = 0x60000;
    Actor_SetAnimation(1, 7);
    Actor_MoveToAndWait(1, 0x138, 0x1d6);
    Actor_SetAnimation(1, 1);
    Event_Wait(30);
    Audio_PlayCue(153);
    *(s32 *)(obj + 40) = 0x60000;
    Actor_SetAnimation(1, 7);
    Actor_MoveToAndWait(1, 0x156, 0x1d6);
    Actor_SetAnimation(1, 1);
    Event_Wait(30);
    Audio_PlayCue(153);
    *(s32 *)(obj + 40) = 0x60000;
    Actor_SetAnimation(1, 7);
    Actor_MoveToAndWait(1, 0x178, 0x1d6);
    Actor_SetAnimation(1, 1);
}

static __inline__ void SetFlagBits(u8 *flags, u8 bits)
{
    *flags |= bits;
}

void FieldScene_RunActorFourteenGuestScene(void)
{
    u32 i;
    u8 *rec;
    s32 record;
    s32 none;

    Actor_RunRepeatedMotion(1, 3);
    Event_Wait(10);
    Actor_FaceDirection(1, 0x3000, 0);
    record = Func_020066e4(14);
    Actor_SetSpriteFlags(record, 0);
    Actor_SetChildValue(14, 15);
    Actor_SetPosition(14, 0x1880000, 0x1c60000);
    Func_02006104();
    Actor_FaceDirection(1, 0xd000, 10);
    Actor_StartRepeatedMotion(1, 2);
    Actor_ShowEmote(1, 0x100, 40);
    Actor_FaceDirection(14, 0x5000, 10);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(20);
    Event_SetMessage((s32)&LinkedMessage_AlexAsksForStars);
    Event_ShowMessage(14, 0);
    Actor_SetPosition(10, 0x1d50000, 0x15c0000);
    Event_Wait(20);
    Func_02005ff6(0x200a, 10);
    Func_02005ffe(0x200a, 40);
    Actor_SetPosition(10, 0x1fb0000, 0x15c0000);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(40);
    Actor_SetAnimationAndWait(1, 3);
    Event_Wait(40);
    Actor_SetSpeed(1, 0x8000, 0x4000);
    Actor_WalkToAndWait(1, 0x185, 0x1d4);
    Actor_FaceDirection(1, 0xd000, 60);
    Func_02006054(1, 20);
    Func_02006742((s32)&LinkedMessage_AlexAsksForStars + 4, 1, 10);
    Actor_SetSpeed(1, 0x8000, 0x4000);
    rec = Value1(Func_020067e6, 1);
    rec[90] &= 254;
    none = 0;
    Actor_WalkToAndWait(1, 0x178, 0x1d6);
    Event_Wait(30);
    SetFlagBits(&rec[90], 1);
    Actor_SetAnimationAndWait(14, 4);
    Event_Wait(10);
    Event_SetMessage((s32)&LinkedMessage_AlexAsksForStars + 5);
    Func_020060be(14, 20);
    Actor_ShowEmote(1, 0x101, 60);
    Actor_SetAnimationAndWait(14, 3);
    Func_020060d8(14, 20);
    Actor_ShowEmote(1, 0x102, 60);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(20);
    Actor_FaceDirection(14, 0xc000, 20);
    Actor_SetChildValue(14, 0x100);
    record = Func_02006882(14);
    Actor_SetSpriteFlags(record, 0);
    rec = Value1(Func_0200688e, 14);
    rec[85] = none;
    Audio_PlayCue(220);
    for (i = 0; i != 30; i++) {
        *(s32 *)(rec + 12) += 0x10000;
        Event_Wait(1);
    }
    rec[85] = 5;
    Actor_StartRepeatedMotion(1, 2);
    Func_02006156(1, 10);
    Actor_ShowEmote(14, 0x101, 60);
    Actor_FaceDirection(14, 0x5000, 10);
    Func_02006174(1, 20);
    Actor_RunRepeatedMotion(14, 1);
    Func_02006184(14, 20);
    Actor_ShowEmote(1, 0x103, 20);
    Func_02006196(1, 30);
    Actor_ShowEmote(14, 0x105, 80);
    Actor_FaceDirection(14, 0xd000, 40);
    Actor_FaceDirection(10, 0x5000, 10);
    Event_CloseScreen();
    Event_WaitForScreen();
    Camera_MoveTo(0x1dd0000, -1, 0x14e0000, 0);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_SetAnimationAndWait(10, 4);
    Func_020061f8(10, 10);
    Event_OpenMessage(11, 0);
    Camera_SetSpeed(0x66666, 0xcccc);
    Camera_MoveTo(0x1760000, -1, 0x1d60000, 1);
    Camera_WaitForMove();
    Actor_FaceDirection(14, 0x5000, 0);
    Actor_FaceDirection(1, 0xe000, 10);
    Actor_RunRepeatedMotion(1, 2);
    if (Event_ChooseYesNo(1, 0) != 0) {
        Event_Wait(10);
        Value2(Engine_ActorSetAnimationAndWait, 14, 4);
        Event_SetMessage(0x10c3);
        Event_OpenMessage(14, 0);
        if (Event_ChooseYesNo(1, 0) == 0) {
            do {
                Event_Wait(20);
                Value2(Engine_ActorSetAnimationAndWait, 14, 4);
                Event_Wait(10);
                Event_SetMessage(0x10c6);
                Event_OpenMessage(14, 0);
            } while (Event_ChooseYesNo(1, 0) == 0);
        }
    }
    Event_Wait(30);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(20);
    Event_SetMessage(0x10c4);
    Func_020062de(14, 30);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(10);
    Func_020062f4(14, 30);
    rec[85] = 0;
    Actor_SetSpeed(14, 0x26666, 0x13333);
    Call4(Func_020069a6, (s32)rec, 0x1cc0000, 0, 0x1680000);
    Actor_WaitForMove(14);
    Actor_SetChildValue(14, 0);
    record = Func_02006a9a(14);
    Actor_SetSpriteFlags(record, 1);
    Event_Wait(30);
    Camera_FollowActor(1, 1);
    Camera_WaitForMove();
    Event_Wait(40);
    Actor_ShowEmote(1, 0x103, 40);
    Actor_RunRepeatedMotion(1, 3);
    Event_Wait(20);
    rec = Value1(Func_02006ad6, 1);
    SetFlagBits(&rec[90], 1);
    *(s32 *)(rec + 48) = 0x30000;
    *(s32 *)(rec + 52) = 0x20000;
    Audio_PlayCue(153);
    *(s32 *)(rec + 40) = 0x60000;
    Actor_SetAnimation(1, 7);
    Actor_MoveToAndWait(1, 0x156, 0x1d6);
    Actor_SetAnimation(1, 1);
    Event_Wait(30);
    Audio_PlayCue(153);
    *(s32 *)(rec + 40) = 0x60000;
    Actor_SetAnimation(1, 7);
    Actor_MoveToAndWait(1, 0x138, 0x1d6);
    Actor_SetAnimation(1, 1);
    Event_Wait(30);
    Audio_PlayCue(153);
    *(s32 *)(rec + 40) = 0x60000;
    Actor_SetAnimation(1, 7);
    Actor_MoveToAndWait(1, 0x116, 0x1e0);
    Actor_SetAnimation(1, 1);
    Event_Wait(30);
    Camera_SetSpeed(0x8000, 0x1000);
    Camera_FollowActor(0, 1);
    Actor_SetSpeed(1, 0x19999, 0xcccc);
    Actor_FaceEachOther(0, 1, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(1, 3);
    Actor_SetAnimationAndWait(0, 4);
    Actor_RunRepeatedMotion(1, 2);
    Actor_SetAnimationAndWait(0, 3);
    Event_Wait(20);
    Actor_SetAnimation(1, 2);
    {
        s32 slot = Value1(Func_02006bda, 0);

        if (slot != 0) {
            Actor_SetDestination(1, *(s16 *)(slot + 10), *(s16 *)(slot + 18));
        }
    }
    Actor_WaitForMove(1);
    Actor_SetPosition(1, 0, 0);
    Func_02006bd2(220);
    Func_02006bd8(221);
    Func_02006bde(223);
}

void FieldScene_RunSixPassEffectSequence(void)
{
    u8 pass;
    s32 rec7;
    s32 record;

    Event_Begin();
    Audio_PlayCue(141);
    for (pass = 0; pass != 6; pass++) {
        ColorBuffer_ApplyTarget(0x4039d2, 1);
        ColorBuffer_Interpolate(8);
        Event_Wait(8);
        ColorBuffer_ApplyTarget(0x10000, 1);
        ColorBuffer_Interpolate(8);
        Event_Wait(8);
        if (pass == 1) {
            Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
        }
    }
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Map_CopyCellsTo(0, 40, 13, 46, 3, 3);
    Event_Wait(20);
    rec7 = Value4(Func_020066f4, 222, 0xe80000, 0x100000, 0x900000);
    Event_Wait(40);
    Func_02006dc8_a(rec7, 1);
    Message_ShowCentered(0x1078, 1);
    Actor_SetPosition(5, 0x1330000, 0x1150000);
    Actor_SetPosition(9, 0x1330000, 0x1150000);
    Actor_SetPosition(11, 0x1330000, 0x1150000);
    Actor_SetPosition(10, 0x1330000, 0x1150000);
    Actor_SetPosition(14, 0x1330000, 0x1150000);
    Actor_SetSpeed(0, 0x13333, 0x9999);
    Actor_WalkToAndWait(0, 232, 156);
    Event_Wait(10);
    record = Value1(Func_02006eba_b, 0);
    if (record != 0) {
        Actor_SetPosition(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(1, 0x13333, 0x9999);
    Actor_WalkToAndWait(1, 218, 172);
    Actor_FaceEachOther(1, 0, 0);
    Event_Wait(20);
    Audio_PlayCue(145);
    Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
    Event_Wait(20);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Event_Wait(40);
    Actor_FaceDirection(0, 0xd000, 0);
    Actor_FaceDirection(1, 0x5000, 50);
    Audio_PlayCue(144);
    Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Actor_FaceDirection(0, 0x8000, 0);
    Actor_FaceDirection(1, 0, 50);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Actor_FaceDirection(0, 0, 0);
    Actor_FaceDirection(1, 0x8000, 50);
    Actor_FaceDirection(0, 0xb000, 0);
    Actor_FaceDirection(1, 0xd000, 0);
    Audio_PlayCue(144);
    Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Event_Wait(30);
    Actor_Jump(0, 2, 0);
    Actor_Jump(1, 2, 20);
    Actor_Jump(0, 6, 0);
    Actor_Jump(1, 6, 40);
    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x100;
    Value0(Engine_EventCloseScreen);
    Event_WaitForScreen();
    Event_RequestExit(2);
}

/*
 * A scripted-scene bracket around a two-level story-flag test. When flag
 * 0x83e is set the actor speaks one line and the handler ends; otherwise flag
 * 0x83c chooses between two alternative lines and the longer path also plays
 * a motion, waits ten frames and closes. The two tests read with opposite
 * polarity. The trailing zero arguments have no established meaning. The
 * 108-byte owner includes its five-word literal pool.
 */
void FieldScene_RunActorNineDialogueByFlags(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x83e)) {
        Event_SetMessage(0x10cb);
        Event_ShowMessage(9, 0);
    } else {
        if (GameFlag_IsSet(0x83c) == 0)
            Event_SetMessage(0x1079);
        else
            Event_SetMessage(0x107b);
        Actor_FaceEachOther(9, 0, 0);
        Event_Wait(10);
        Event_ShowMessage(9, 0);
    }
    Event_End();
}

/*
 * A scripted-scene bracket around a two-level story-flag test. Flag 0x83e
 * selects a single line; otherwise flag 0x83c chooses between two lines and
 * the longer path plays a motion, waits ten frames and closes. The two tests
 * read with opposite polarity. The trailing zero arguments have no
 * established meaning. The 108-byte owner includes its five-word pool.
 */
void SceneDialogue_RunActorFiveTalk(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x83e)) {
        Event_SetMessage(0x10c9);
        Event_ShowMessage(5, 0);
    } else {
        if (GameFlag_IsSet(0x83c) == 0)
            Event_SetMessage(0x107a);
        else
            Event_SetMessage(0x107c);
        Actor_FaceEachOther(5, 0, 0);
        Event_Wait(10);
        Event_ShowMessage(5, 0);
    }
    Event_End();
}

/*
 * Talk handler for actor 10: the scripted-scene bracket around one line of
 * dialogue, with no branch and no state. It is published into a script record
 * rather than called from the image.
 *
 * The thirty-two-byte owner includes its one pool word, which carries the
 * line id, and the zero alignment halfword after it. The second argument of
 * the tail call is zero at every site in this bank and its role is not
 * established.
 */
void SceneDialogue_RunActorTenDialogue(void)
{

    Event_Begin();
    Event_SetMessage(0x10ca);
    Event_ShowMessage(10, 0);
    Event_End();
}

/*
 * Talk handler for actor 11, published into a script record rather than
 * called. The thirty-two-byte owner includes its one pool word, which carries
 * the line id, and the zero alignment halfword after it.
 */
void SceneDialogue_RunActor11TalkLine(void)
{

    Event_Begin();
    Event_SetMessage(0x10c7);
    Event_ShowMessage(11, 0);
    Event_End();
}

/*
 * Talk handler for actor 13, published into a script record rather than
 * called. The thirty-two-byte owner includes its one pool word, which carries
 * the line id, and the zero alignment halfword after it.
 */
void SceneDialogue_ShowLine10C8(void)
{

    Event_Begin();
    Event_SetMessage(0x10c8);
    Event_ShowMessage(13, 0);
    Event_End();
}

/*
 * Talk handler for actor 14, published into a script record rather than
 * called. The thirty-two-byte owner includes its one pool word, which carries
 * the line id, and the zero alignment halfword after it.
 */
void SceneDialogue_RunActor14TalkLine(void)
{

    Event_Begin();
    Event_SetMessage(0x10cc);
    Event_ShowMessage(14, 0);
    Event_End();
}

/*
 * Same bracket and line call as the talk handlers, but the tail pairs actors
 * 9 and 10 through the overlay's own call.
 */
void SceneDialogue_RunLine1072WithPair9And10(void)
{
    Event_Begin();
    Event_SetMessage(0x1072);
    Func_02006a26(9, 10);
    Event_End();
}

void FieldScene_Forward72b4(void)
{
    Leader_CheckAhead();
}

void Lifted_020027f8(void)
{
    u16 i;
    u8 *p10;
    u8 *p10b;
    u8 *p9;
    u8 *p9b;
    u8 *p9c;
    u8 *rec;
    u8 *rec2;
    u8 *record;
    u8 *work;
    s32 base5_4009;
    s32 v6;
    s32 base5_8009;
    s32 zero;

    Event_Begin();
    Map_CopyCellAttributes(27, 16, 5, 1, 27, 17);
    Camera_MoveTo(0x1d70000, -1, 0x1050000, 0);
    Camera_WaitForMove();
    Map_Redraw();
    rec2 = (u8 *)Func_020071f6(8);
    *(s32 *)(rec2 + 24) = 0x1999;
    *(s32 *)(rec2 + 28) = 0x1999;
    rec = (u8 *)Func_02007204(0);
    p9 = *(u8 **)(rec + 80) + 38;
    *p9 = 0;
    *(s32 *)(rec + 24) = 0x1999;
    *(s32 *)(rec + 28) = 0x1999;
    Actor_SetChildValue(0, 0x100);
    Actor_SetPosition(0, 0x1d70000, 0x1220000);
    p10 = rec + 85;
    *p10 = 0;
    *(s32 *)(rec + 12) = 0x280000;
    work = *(u8 **)Data_03001ebc;
    *(s32 *)(((s32)work + 0x1c0)) = 0x203;
    *(s32 *)(((s32)work + 0x1c8)) = 32;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_SetPosition(8, 0x1d70000, 0x1220000);
    Audio_PlayCue(190);
    Actor_SetSpritePriority(0, 2);
    for (i = 0; i != 90; i++) {
        *(s32 *)(rec + 12) += -0x1999;
        *(s32 *)(rec + 24) += 0x28f;
        *(s32 *)(rec + 28) += 0x28f;
        *(s32 *)(rec2 + 24) += 0x28f;
        *(s32 *)(rec2 + 28) += 0x28f;
        Event_Wait(1);
    }
    *p10 = 5;
    Event_Wait(80);
    Camera_SetSpeed(0x4ccc, 0x999);
    Camera_MoveTo(0x1d70000, -1, 0x1220000, 1);
    for (i = 0; i != 60; i++) {
        *(s32 *)(rec + 12) += -0x8000;
        Event_Wait(1);
    }
    *p10 = 3;
    Event_Wait(20);
    Actor_SetSpritePriority(0, 1);
    {
        u8 *record = Func_02007308(0);
        s32 flags = 1 | record[35];
        record[35] = flags;
    }
    Actor_SetChildValue(0, 0);
    {
        s32 shown = 1;

        *p9 = shown;
    }
    ((void (*)())Engine_ActorSetPosition)(8, 0, 0);
    Camera_WaitForMove();
    Event_Wait(20);
    Camera_FollowActor(0, 1);
    Actor_SetSpeed(0, 0x8000, 0x4000);
    Actor_WalkToAndWait(0, 0x1d7, 0x136);
    Event_Wait(20);
    Actor_RunRepeatedMotion(0, 2);
    Event_Wait(20);
    zero = 0;
    record = (u8 *)Func_02007372_b(0);
    if ((s32)record != 0) {
        Actor_SetPosition(5, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
    }
    record = (u8 *)Func_02007386(0);
    if ((s32)record != 0) {
        Actor_SetPosition(1, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
    }
    Actor_SetSpeed(1, 0x8000, 0x4000);
    Actor_SetSpeed(5, 0x8000, 0x4000);
    Actor_WalkTo(5, 0x1c5, 0x12e);
    Actor_WalkToAndWait(1, 0x1e9, 0x12e);
    Actor_SetAnimation(5, 1);
    Actor_FaceDirection(1, 0x3000, 0);
    Actor_FaceDirection(5, 0x6000, 10);
    Actor_ShowEmote(0, 0x100, 0);
    Actor_ShowEmote(5, 0x100, 0);
    Actor_ShowEmote(1, 0x100, 0);
    Actor_Jump(0, 2, 0);
    Actor_Jump(5, 2, 0);
    Actor_Jump(1, 2, 40);
    Actor_StartRepeatedMotion(0, 3);
    Actor_StartRepeatedMotion(5, 3);
    Actor_RunRepeatedMotion(1, 3);
    Event_Wait(40);
    Actor_SetAttachedEffect(0, 0x102);
    Actor_SetAttachedEffect(5, 0x102);
    Actor_SetAttachedEffect(1, 0x102);
    Event_Wait(60);
    Actor_SetSpeed(0, 0x13333, 0x9999);
    Actor_SetSpeed(5, 0x13333, 0x9999);
    Actor_SetSpeed(1, 0x13333, 0x9999);
    Actor_WalkTo(0, 0x1d7, 0x15a);
    Actor_WalkTo(5, 0x1af, 0x152);
    Actor_WalkTo(1, 0x1ff, 0x152);
    Actor_WaitForMove(0);
    Actor_SetAnimation(0, 1);
    Actor_WaitForMove(5);
    Actor_SetAnimation(5, 1);
    Actor_WaitForMove(1);
    Actor_SetAnimation(1, 1);
    Actor_SetSpeed(0, 0x4ccc, 0x2666);
    Actor_SetSpeed(5, 0x4ccc, 0x2666);
    Actor_SetSpeed(1, 0x4ccc, 0x2666);
    Actor_FaceDirection(5, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(0, 0xc000, 60);
    Actor_FaceDirection(5, 0x8000, 0);
    Actor_FaceDirection(1, 0, 0);
    Actor_FaceDirection(0, 0x4000, 60);
    Actor_FaceDirection(5, 0, 0);
    Actor_FaceDirection(1, 0x8000, 40);
    Actor_FaceDirection(0, 0, 40);
    Actor_SetAnimationAndWait(0, 3);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(1, 4);
    Event_Wait(20);
    Actor_FaceDirection(0, 0x8000, 40);
    Actor_SetAnimationAndWait(0, 3);
    Actor_RunRepeatedMotion(5, 2);
    Event_Wait(20);
    Actor_ShowEmote(5, 0x101, 60);
    Actor_FaceDirection(0, 0x4000, 0);
    Actor_FaceDirection(1, 0x3000, 0);
    Actor_FaceDirection(5, 0x6000, 60);
    rec = (u8 *)Func_02007622(9);
    p9b = *(u8 **)(rec + 80) + 38;
    *p9b = zero;
    *(s32 *)(rec + 24) = 0x1999;
    *(s32 *)(rec + 28) = 0x1999;
    *(s32 *)(rec2 + 24) = 0x1999;
    *(s32 *)(rec2 + 28) = 0x1999;
    Actor_SetChildValue(9, 0x100);
    Actor_SetPosition(9, 0x1d70000, 0x1220000);
    p10b = rec + 85;
    *p10b = zero;
    *(s32 *)(rec + 12) = 0x280000;
    Event_Wait(1);
    Event_SetMessage(0x103c);
    Event_ShowMessage(9, 0);
    Actor_Jump(0, 4, 0);
    Actor_Jump(5, 4, 0);
    Actor_Jump(1, 4, 40);
    Actor_FaceDirection(0, 0xc000, 0);
    Call3((void (*)())Func_02007766_a, 5, 0xe000, 0);
    Actor_FaceDirection(1, 0xb000, 0);
    Camera_SetSpeed(0x20000, 0x4000);
    Camera_MoveTo(0x1d70000, -1, 0x1350000, 1);
    Camera_WaitForMove();
    Actor_SetPosition(8, 0x1d70000, 0x1220000);
    Audio_PlayCue(190);
    Actor_SetSpritePriority(9, 2);
    for (i = 0; i != 90; i++) {
        *(s32 *)(rec + 12) += -0x1999;
        *(s32 *)(rec + 24) += 0x28f;
        *(s32 *)(rec + 28) += 0x28f;
        *(s32 *)(rec2 + 24) += 0x28f;
        *(s32 *)(rec2 + 28) += 0x28f;
        Event_Wait(1);
    }
    *p10b = 5;
    Event_Wait(80);
    for (i = 0; i != 60; i++) {
        *(s32 *)(rec + 12) += -0x8000;
        Event_Wait(1);
    }
    *p10b = 3;
    Event_Wait(30);
    Actor_SetSpritePriority(9, 1);
    {
        u8 *record = Func_02007766_b(9);
        s32 flags = 1 | record[35];
        record[35] = flags;
    }
    Actor_SetChildValue(9, 0);
    {
        s32 shown = 1;

        *p9b = shown;
    }
    Actor_SetPosition(8, 0, 0);
    Call1_020027f8((void (*)())Func_02007766_c, 30);
    Actor_SetSpeed(9, 0x13333, 0x9999);
    Actor_WalkToAndWait(9, 0x1d7, 0x132);
    Event_Wait(20);
    Actor_ShowEmote(9, 0x100, 0);
    Actor_Jump(9, 2, 80);
    Actor_RunRepeatedMotion(9, 3);
    Event_Wait(30);
    Actor_FaceDirection(9, 0x8000, 30);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(20);
    Actor_FaceDirection(9, 0, 30);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(20);
    Actor_FaceDirection(9, 0x4000, 30);
    Actor_RunRepeatedMotion(9, 3);
    Event_Wait(30);
    Func_020070ae(9, 20);
    Actor_SetSpeed(9, 0x26666, 0x13333);
    Actor_WalkToAndWait(9, 0x1a7, 0x132);
    Actor_FaceDirection(9, 0xb000, 0);
    Actor_FaceDirection(0, 0xb000, 0);
    Actor_FaceDirection(5, 0xa000, 0);
    Actor_FaceDirection(1, 0xb000, 30);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(30);
    Actor_SetAnimationAndWait(9, 3);
    Event_Wait(40);
    Actor_WalkToAndWait(9, 0x207, 0x132);
    Actor_FaceDirection(9, 0xd000, 0);
    Actor_FaceDirection(0, 0xd000, 0);
    Actor_FaceDirection(5, 0xe000, 0);
    Actor_FaceDirection(1, 0xd000, 20);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(30);
    base5_4009 = (s32)Data_00004009;
    Actor_SetAnimationAndWait(9, 4);
    Event_Wait(40);
    Func_02007172(base5_4009, 30);
    Actor_FaceEachOther(0, 5, 0);
    Event_Wait(40);
    Actor_StartRepeatedMotion(0, 2);
    Actor_RunRepeatedMotion(5, 2);
    Actor_ShowEmote(1, 0x101, 40);
    Func_020071a4(1, 40);
    Actor_FaceDirection(9, 0x5000, 20);
    Func_020071b8(base5_4009, 20);
    Actor_FaceDirection(0, 0xd000, 0);
    Actor_FaceDirection(5, 0xe000, 20);
    Actor_ShowEmote(0, 0x101, 0);
    Actor_ShowEmote(5, 0x101, 40);
    Actor_SetAnimationAndWait(9, 4);
    Func_020071f4(base5_4009, 10);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(5, 3);
    Actor_SetAnimationAndWait(0, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(9, 3);
    Event_Wait(40);
    Camera_SetSpeed(0x8000, 0x1000);
    Camera_MoveTo(0x2150000, -1, 0x1530000, 1);
    Actor_SetSpeed(9, 0x8000, 0x4000);
    Actor_WalkToAndWait(9, 0x215, 0x153);
    Camera_WaitForMove();
    Actor_FaceDirection(0, 0, 0);
    Actor_FaceDirection(5, 0, 0);
    Actor_FaceDirection(1, 0, 20);
    Actor_FaceDirection(9, 0xd000, 40);
    Actor_FaceDirection(9, 0, 40);
    Actor_FaceDirection(9, 0x3000, 60);
    Actor_RunRepeatedMotion(9, 3);
    Event_Wait(30);
    Camera_FollowActor(9, 1);
    Actor_SetSpeed(9, 0x19999, 0xcccc);
    Actor_WalkTo(9, 0x1c7, 0x168);
    Event_Wait(20);
    Actor_FaceDirection(0, 0x4000, 0);
    Actor_FaceDirection(5, 0x2000, 0);
    Actor_FaceDirection(1, 0x5000, 0);
    Actor_WalkToAndWait(9, 0x1c7, 0x168);
    Event_Wait(20);
    Actor_FaceDirection(9, 0x5000, 30);
    Actor_RunRepeatedMotion(9, 3);
    Event_Wait(30);
    Actor_WalkToAndWait(9, 0x1d7, 0x168);
    Event_Wait(20);
    Actor_FaceDirection(9, 0x3000, 30);
    Actor_ShowEmote(9, 0x100, 0);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(30);
    Func_020073a6(9, 30);
    Actor_RunRepeatedMotion(9, 3);
    Event_Wait(20);
    Func_020073bc(9, 30);
    Actor_SetSpeed(5, 0xcccc, 0x6666);
    Actor_WalkToAndWait(5, 0x1b8, 0x15a);
    Event_Wait(10);
    Actor_FaceDirection(5, 0x2000, 10);
    Func_020073ee(5, 20);
    Actor_SetSpeed(1, 0xcccc, 0x6666);
    Actor_WalkToAndWait(1, 0x1ef, 0x15a);
    Camera_WaitForMove();
    Event_Wait(10);
    Actor_FaceDirection(1, 0x6000, 10);
    Event_OpenMessage(1, 0);
    Actor_FaceDirection(5, 0, 0);
    Actor_FaceDirection(1, 0x8000, 0);
    if (Event_ChooseYesNo(0, 0) == 1) {
        bump_step(1);
    }
    Actor_FaceDirection(9, 0xc000, 30);
    Func_02007468(9, 30);
    Event_SetMessage(0x1048);
    Actor_FaceDirection(5, 0x2000, 0);
    Actor_FaceDirection(1, 0x6000, 30);
    Actor_RunRepeatedMotion(9, 2);
    Func_02007496(9, 10);
    Actor_SetAnimationAndWait(9, 3);
    Func_020074a6(9, 20);
    Actor_RunRepeatedMotion(9, 2);
    Func_020074b6(9, 10);
    Actor_SetAttachedEffect(9, 0x102);
    Event_Wait(40);
    Func_020074ce(9, 10);
    Actor_FaceDirection(9, 0x2000, 20);
    Actor_StartRepeatedMotion(0, 2);
    Actor_StartRepeatedMotion(5, 2);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(10);
    Actor_FaceDirection(0, 0x2000, 0);
    Actor_FaceDirection(5, 0x2000, 0);
    Actor_FaceDirection(1, 0x2000, 20);
    Actor_Stop(9);
    Actor_Stop(0);
    Actor_Stop(5);
    Actor_Stop(1);
    Camera_SetSpeed(0x80000, 0x10000);
    Camera_MoveTo(0x2c70000, -1, 0x1d00000, 1);
    Camera_WaitForMove();
    Actor_SetPosition(9, 0x24d0000, 0x1610000);
    Event_Wait(40);
    Event_ShowMessage(0x1009, 0);
    Actor_SetPosition(9, 0x1d70000, 0x1680000);
    Event_Wait(80);
    Camera_MoveTo(0x1d70000, -1, 0x1720000, 1);
    Camera_WaitForMove();
    Event_Wait(10);
    Actor_RunRepeatedMotion(9, 3);
    Event_Wait(10);
    Actor_FaceDirection(9, 0xd000, 20);
    Actor_FaceDirection(0, 0xe000, 0);
    Actor_FaceDirection(5, 0xe000, 0);
    Actor_FaceDirection(1, 0xe000, 20);
    Camera_MoveTo(0x2c70000, -1, 0x930000, 1);
    Camera_WaitForMove();
    Actor_SetPosition(9, 0x2540000, 0xee0000);
    Event_Wait(40);
    Event_ShowMessage(0x1009, 0);
    Actor_SetPosition(9, 0x1d70000, 0x1680000);
    Event_Wait(80);
    Camera_MoveTo(0x1d70000, -1, 0x1720000, 1);
    Camera_WaitForMove();
    Event_Wait(10);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(10);
    Actor_Jump(9, 4, 0);
    Actor_FaceDirection(9, 0xb000, 20);
    Actor_StartRepeatedMotion(0, 1);
    Actor_StartRepeatedMotion(5, 1);
    Actor_RunRepeatedMotion(1, 1);
    Actor_FaceDirection(0, 0xa000, 0);
    Actor_FaceDirection(5, 0xa000, 0);
    Actor_FaceDirection(1, 0xa000, 20);
    Camera_MoveTo(0xe70000, -1, 0x930000, 1);
    Camera_WaitForMove();
    Actor_SetPosition(9, 0x1340000, 0xfa0000);
    Event_Wait(40);
    Event_ShowMessage(0x2009, 0);
    Actor_SetPosition(9, 0x1d70000, 0x1680000);
    Event_Wait(80);
    Camera_MoveTo(0x1d70000, -1, 0x1720000, 1);
    Camera_WaitForMove();
    Event_Wait(10);
    Actor_RunRepeatedMotion(9, 3);
    Event_Wait(10);
    Actor_Jump(9, 6, 0);
    Actor_FaceDirection(9, 0x5000, 20);
    Actor_StartRepeatedMotion(0, 2);
    Actor_StartRepeatedMotion(5, 2);
    Actor_RunRepeatedMotion(1, 2);
    Actor_FaceDirection(0, 0x6000, 0);
    Actor_FaceDirection(5, 0x6000, 0);
    Actor_FaceDirection(1, 0x6000, 20);
    Camera_MoveTo(0xe70000, -1, 0x1d00000, 1);
    Camera_WaitForMove();
    Actor_SetPosition(9, 0x1320000, 0x16a0000);
    Event_Wait(40);
    Event_ShowMessage(0x2009, 0);
    Actor_SetPosition(9, 0x1d70000, 0x1680000);
    Event_Wait(80);
    Camera_MoveTo(0x1d70000, -1, 0x1720000, 1);
    Camera_WaitForMove();
    Event_Wait(30);
    Actor_ShowEmote(9, 0x104, 60);
    Actor_FaceDirection(0, 0x4000, 0);
    Actor_FaceDirection(5, 0x2000, 0);
    Actor_FaceDirection(1, 0x6000, 40);
    Actor_ShowEmote(5, 0x102, 40);
    Actor_RunRepeatedMotion(5, 1);
    Func_02007834_b(5, 20);
    Actor_Jump(9, 4, 0);
    Actor_FaceDirection(9, 0xb000, 20);
    Actor_RunRepeatedMotion(9, 2);
    Call2(Func_0200785a, 0xa009, 10);
    Actor_StartRepeatedMotion(9, 3);
    Call2(Func_0200786a_b, 0xa009, 20);
    Actor_ShowEmote(0, 0x101, 0);
    Actor_ShowEmote(5, 0x101, 0);
    Actor_ShowEmote(1, 0x101, 60);
    Actor_FaceDirection(9, 0xc000, 20);
    Actor_SetAnimation(9, 4);
    Event_OpenMessage(0x8009, 0);
    if (Event_ChooseYesNo(0, 0) == 1) {
        bump_step(1);
    }
    Event_Wait(20);
    Actor_FaceDirection(1, 0x8000, 10);
    Func_020078da(1, 10);
    Event_SetMessage(0x1056);
    Actor_FaceDirection(1, 0x6000, 20);
    Actor_Jump(9, 4, 40);
    Func_02007902(0x8009, 10);
    Actor_ShowEmote(0, 0x101, 0);
    Actor_ShowEmote(5, 0x101, 0);
    Actor_ShowEmote(1, 0x101, 80);
    Actor_SetAttachedEffect(9, 0x102);
    Event_Wait(40);
    Func_02007938_b(0x8009, 40);
    Actor_ShowEmote(0, 0x106, 0);
    Actor_ShowEmote(5, 0x106, 0);
    Actor_ShowEmote(1, 0x106, 60);
    Actor_FaceDirection(5, 0, 0);
    Actor_FaceDirection(1, 0x8000, 20);
    Event_OpenMessage(0x8009, 0);
    if (Event_ChooseYesNo(0, 0) == 1) {
        bump_step(1);
    }
    Actor_RunRepeatedMotion(9, 1);
    Event_Wait(20);
    Actor_FaceDirection(5, 0x2000, 0);
    Actor_FaceDirection(1, 0x5000, 20);
    Func_020079c4_b(0x8009, 40);
    Event_SetMessage(0x105b);
    Actor_SetAnimation(5, 4);
    Func_020079da(5, 10);
    Actor_FaceDirection(9, 0xb000, 10);
    Actor_RunRepeatedMotion(9, 1);
    Actor_Jump(9, 4, 40);
    Call2(Func_02007a00, 0xa009, 10);
    Actor_StartRepeatedMotion(0, 1);
    Actor_StartRepeatedMotion(5, 1);
    Actor_RunRepeatedMotion(1, 1);
    Event_Wait(40);
    Actor_FaceDirection(9, 0xc000, 10);
    Actor_RunRepeatedMotion(9, 1);
    Func_02007a3a(0x8009, 40);
    Actor_ShowEmote(0, 0x105, 0);
    Actor_ShowEmote(5, 0x105, 0);
    Actor_ShowEmote(1, 0x105, 120);
    Actor_ShowEmote(1, 0x107, 0);
    Actor_Jump(1, 4, 40);
    Func_02007a74(1, 10);
    Actor_RunRepeatedMotion(9, 1);
    Event_Wait(40);
    Actor_FaceDirection(9, 0x4000, 80);
    Func_02007a96(0x8009, 10);
    Actor_FaceDirection(1, 0x8000, 0);
    Actor_FaceDirection(0, 0, 30);
    Actor_StartRepeatedMotion(0, 2);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(20);
    Actor_FaceDirection(5, 0, 0);
    Actor_FaceDirection(0, 0x8000, 30);
    Actor_StartRepeatedMotion(0, 2);
    Actor_RunRepeatedMotion(5, 2);
    Event_Wait(20);
    Actor_FaceDirection(0, 0x4000, 0);
    Actor_FaceDirection(5, 0x2000, 0);
    Actor_FaceDirection(1, 0x5000, 40);
    Actor_SetAttachedEffect(0, 0x102);
    Actor_SetAttachedEffect(5, 0x102);
    Actor_SetAttachedEffect(1, 0x102);
    Event_Wait(80);
    Actor_SetAnimationAndWait(9, 3);
    Event_Wait(20);
    Func_02007b4c(0x8009, 40);
    rec = (u8 *)Func_020082c6(9);
    v6 = 192;
    Task_Wait(6);
    *(s32 *)(rec + 48) = 0x30000;
    *(s32 *)(rec + 52) = 0x20000;
    Audio_PlayCue(153);
    *(s32 *)(rec + 40) = (v6 << 11);
    Actor_MoveToAndWait(9, 0x1d7, 0x18b);
    Task_Wait(6);
    Actor_SetSpeed(9, 0x4ccc, 0x2666);
    p9c = rec + 90;
    *p9c &= 254;
    Actor_SetDestination(9, 0x1d9, 0x18b);
    Actor_WaitForMove(9);
    Actor_RunRepeatedMotion(9, 2);
    Actor_SetDestination(9, 0x1d5, 0x18b);
    Actor_WaitForMove(9);
    Actor_RunRepeatedMotion(9, 2);
    Actor_SetDestination(9, 0x1d7, 0x18b);
    Actor_WaitForMove(9);
    Func_02007c24(9, 10);
    Actor_SetSpeed(9, 0xcccc, 0x6666);
    Actor_WalkToAndWait(9, 0x1d7, 0x19b);
    Actor_SetSpeed(9, 0x4ccc, 0x2666);
    *p9c &= 254;
    Actor_SetDestination(9, 0x1da, 0x19b);
    Actor_WaitForMove(9);
    Actor_RunRepeatedMotion(9, 3);
    Actor_SetDestination(9, 0x1d4, 0x19b);
    Actor_WaitForMove(9);
    Actor_RunRepeatedMotion(9, 3);
    Actor_SetDestination(9, 0x1d7, 0x19b);
    Actor_WaitForMove(9);
    Actor_SetAttachedEffect(9, 0x102);
    Actor_StartRepeatedMotion(9, 3);
    Func_02007ca8(9, 10);
    Actor_SetSpeed(9, 0x3333, 0x1999);
    Actor_WalkToAndWait(9, 0x1d7, 0x18b);
    Actor_EnableActionCallback(9, 1);
    Event_Wait(30);
    {
        s32 flags = 1 | *p9c;
        *p9c = flags;
    }
    Actor_FaceDirection(9, 0xc000, 60);
    Task_Wait(6);
    *(s32 *)(rec + 48) = 0x30000;
    *(s32 *)(rec + 52) = 0x20000;
    Audio_PlayCue(153);
    *(s32 *)(rec + 40) = (v6 << 11);
    Actor_MoveToAndWait(9, 0x1d7, 0x168);
    Task_Wait(6);
    Event_Wait(40);
    Actor_ShowEmote(9, 0x102, 80);
    Actor_ShowEmote(0, 0x102, 0);
    Actor_ShowEmote(5, 0x102, 0);
    Actor_ShowEmote(1, 0x102, 80);
    Actor_RunRepeatedMotion(5, 1);
    Actor_ShowEmote(5, 0x107, 40);
    Func_02007d5a(5, 20);
    Actor_FaceDirection(9, 0xb000, 40);
    Actor_SetAnimationAndWait(9, 4);
    Call2(Func_02007d76, 0xa009, 30);
    Actor_FaceDirection(5, 0, 30);
    Actor_ShowEmote(5, 0x106, 60);
    Actor_FaceDirection(5, 0x2000, 30);
    Func_02007da0(5, 20);
    Actor_Jump(0, 4, 0);
    Actor_Jump(1, 4, 0);
    Actor_FaceDirection(0, 0x8000, 0);
    Actor_FaceDirection(1, 0x8000, 40);
    Actor_StartRepeatedMotion(0, 1);
    Actor_RunRepeatedMotion(1, 1);
    Event_Wait(40);
    Actor_FaceDirection(0, 0, 30);
    Actor_StartRepeatedMotion(0, 2);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(30);
    Actor_FaceDirection(9, 0xd000, 30);
    Actor_RunRepeatedMotion(9, 1);
    Event_Wait(20);
    Func_02007e24(0x8009, 20);
    Actor_FaceDirection(0, 0x4000, 0);
    Actor_FaceDirection(1, 0x5000, 30);
    Actor_StartRepeatedMotion(0, 2);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(5, 3);
    Event_Wait(20);
    Actor_FaceDirection(5, 0, 30);
    Event_OpenMessage(5, 0);
    Actor_FaceDirection(0, 0x8000, 0);
    while (Event_ChooseYesNo(0, 0) != 0) {
        Event_SetMessage(0x1068);
        Actor_ShowEmote(5, 0x107, 0);
        Actor_Jump(5, 4, 60);
        Event_OpenMessage(5, 0);
    }
    Event_SetMessage(0x1069);
    Event_Wait(20);
    Actor_SetAnimationAndWait(5, 3);
    Event_Wait(20);
    Actor_FaceDirection(5, 0x2000, 30);
    Func_02007f12(5, 10);
    Actor_FaceDirection(0, 0x4000, 10);
    Actor_FaceDirection(9, 0xb000, 30);
    Actor_SetAnimationAndWait(9, 3);
    Event_Wait(20);
    Actor_FaceDirection(9, 0xc000, 30);
    Actor_SetSpeed(9, 0x3333, 0x1999);
    Actor_WalkToAndWait(9, 0x1d7, 0x160);
    Event_Wait(20);
    base5_8009 = (s32)Data_00008009;
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(20);
    Func_02007f78(base5_8009, 60);
    *p9c &= 254;
    Actor_WalkToAndWait(9, 0x1c8, 0x168);
    Event_Wait(20);
    Actor_RunRepeatedMotion(5, 2);
    Event_Wait(20);
    Event_SetMessage(0x106d);
    Func_02007fb4(5, 30);
    Actor_FaceDirection(9, 0xb000, 30);
    Actor_SetAnimationAndWait(9, 3);
    Event_Wait(20);
    Actor_ShowEmote(1, 0x101, 60);
    Func_02007fe0(1, 10);
    Actor_FaceDirection(9, 0xd000, 20);
    Actor_SetAnimationAndWait(9, 3);
    Func_02007ffc(base5_8009, 20);
    Actor_FaceDirection(0, 0, 0);
    Actor_FaceDirection(1, 0x8000, 40);
    Actor_ShowEmote(0, 0x101, 0);
    Actor_ShowEmote(1, 0x101, 40);
    Actor_RunRepeatedMotion(9, 2);
    Func_02008036_b(base5_8009, 10);
    Actor_FaceDirection(0, 0x4000, 0);
    Actor_FaceDirection(1, 0x5000, 30);
    Actor_SetAnimationAndWait(9, 3);
    Event_Wait(10);
    Func_02008064(base5_8009, 30);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimationAndWait(0, 3);
    Event_Wait(20);
    Actor_FaceDirection(0, 0, 0);
    Actor_FaceDirection(1, 0x8000, 30);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(30);
    Actor_SetAnimationAndWait(0, 3);
    Event_Wait(10);
    Actor_SetSpeed(1, 0x10000, 0x8000);
    Actor_SetAnimation(1, 2);
    record = Value1(Func_0200883c, 0);
    if ((s32)record != 0) {
        Actor_SetDestination(1, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
    }
    Actor_WaitForMove(1);
    Actor_SetPosition(1, 0, 0);
    GameFlag_Set(0x83b);
    Func_0200885a(5);
    Func_020081da();
    Map_CopyCellAttributes(8, 0, 5, 1, 27, 17);
    *(s32 *)((*(u8 **)Data_03001ebc + 0x1c8)) = 16;
    GameFlag_Clear(0x12f);
    Event_End();
}

void FieldScene_RunSplitPairSteps(s32 a, s32 b)
{
    Event_ShowMessage(a, 0);
    Event_Wait(b);
}

/*
 * Drain until room: save the s16 counter at scene workspace + 472, prime two
 * channels, then loop while fewer than four of thirty slots are free,
 * requesting more and passing on any event pair that is not -1. On exit it
 * flushes four times with id 224 and restores the saved counter. The 148-byte
 * owner includes its three-word literal pool. Callee roles are not established.
 */
void SceneState_PumpUntilSlotsFree(void)
{
    u8 *work = *(u8 **)0x03001ebc;
    s16 saved = *(s16 *)(work + 472);
    s32 first;
    s32 second;
    s32 cnt;

    Audio_PlayCue(0x53);
    Item_ShowFound(224, 3);
    Message_ShowCentered(0x111b, 1);
    do {
        cnt = 30 - Func_02008cd0(0);
        cnt -= Func_02008cda(1);

        if (cnt <= 3) {
            Message_ShowCentered(0x111c, 1);
            if (Func_02008e80(&second, &first) != -1)
                Func_02008d08(second, first);
        }
    } while (cnt <= 3);
    Func_02008cd8(224);
    Func_02008cde(224);
    Func_02008ce4(224);
    Func_02008cea(224);
    *(s16 *)(work + 472) = saved;
}
