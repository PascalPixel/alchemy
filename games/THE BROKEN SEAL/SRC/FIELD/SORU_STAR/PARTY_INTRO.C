#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
/*
 * The Elemental Star Room of Sol Sanctum. Sukureta brings Robin, Gerald and
 * Jasmine in, explains the four Elemental Stars and hands out Mythril Bags,
 * and the party bags the Mercury, Mars and Jupiter Stars. Saturos and
 * Menardi then hold Sukureta and Jasmine hostage, Garcia takes off his mask
 * as their guarantee, and Alex takes the three Stars, leaving the party to
 * fetch the last one.
 *
 * The placed actors are named from what they say: Sukureta explains the
 * Stars, Garcia's masked actor is replaced by his unmasked one, Alex is the
 * one the party hands the Stars to, and in the collapse that follows Menardi
 * answers Saturos by name.
 */

enum StarRoomActor {
    ACTOR_JASMINE = 5,
    ACTOR_SUKURETA = ACTOR_FIRST_PLACED + 1,
    ACTOR_SATUROS,
    ACTOR_MENARDI,
    ACTOR_GARCIA_MASKED,
    ACTOR_GARCIA,
    ACTOR_ALEX
};

enum StarRoomMessage {
    MSG_SUKURETA_WHERE_ARE_WE = 0x103c,
    MSG_SUKURETA_THE_ELEMENTS_ARE_THE_SOURCE = 0x1048,
    MSG_SUKURETA_THE_WISDOM_STONE = 0x1056,
    MSG_THE_STARS_HAVE_ENORMOUS_POWER = 0x105b,
    MSG_JASMINE_WONT_YOU_PLEASE = 0x1068,
    MSG_JASMINE_THANK_YOU = 0x1069,
    MSG_THE_BAGS_ARE_FROM_SUKURETAS_COTTAGE = 0x106d,
    MSG_NO_STARS_OUT_THERE = 0x1072,
    MSG_JUPITER_STAR_BAGGED = 0x1077,
    MSG_MARS_STAR_BAGGED = 0x1078,
    MSG_SUKURETA_BRING_THE_STARS_HERE = 0x1079,
    MSG_JASMINE_BE_CAREFUL = 0x107a,
    MSG_SUKURETA_FETCH_THE_OTHERS = 0x107b,
    MSG_JASMINE_SUKURETA_IS_GIDDY = 0x107c,
    MSG_MENARDI_DONT_YOU_WANT_THEM_SAFE = 0x109b,
    MSG_GERALD_ILL_TAKE_THEM = 0x10b4,
    MSG_GERALD_THEY_WONT_LET_JASMINE_GO = 0x10b5,
    MSG_ALEX_YOUR_FRIENDS = 0x10c3,
    MSG_ALEX_THANK_YOU = 0x10c4,
    MSG_ALEX_I_ASK_AGAIN = 0x10c6,
    MSG_MENARDI_BRING_THE_FINAL_GEM = 0x10c7,
    MSG_GARCIA_SILENCE = 0x10c8,
    MSG_JASMINE_MY_BROTHER_IS_ALIVE = 0x10c9,
    MSG_SATUROS_BRING_THE_FINAL_STAR = 0x10ca,
    MSG_SUKURETA_I_AM_RESPONSIBLE = 0x10cb,
    MSG_ALEX_ONLY_ONE_LEFT = 0x10cc,
    MSG_GOT_FOUR_MYTHRIL_BAGS = 0x111b,
    MSG_TOO_MANY_ITEMS = 0x111c
};

enum StarRoomFlag {
    FLAG_STATUE_HALL_TRAP_ARMED = 0x12f,
    FLAG_STAR_ROOM_EXPLAINED = 0x83b,
    FLAG_FIRST_STAR_BAGGED = 0x83c,
    FLAG_MERCURY_STAR_BAGGED = 0x83d,
    FLAG_STARS_GIVEN_TO_ALEX = 0x83e
};

enum {
    ITEM_MYTHRIL_BAG = 224
};

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define OverlayObject_PrepareObject      Func_02000048
#define CreateOverlayObject Func_02004976
#define SetOverlayObjectMode Func_020049d8
#define SetOverlayObjectSlot Func_02004b30
#define CalculateAngleFromCoordinateDelta Func_02004bb2
void Effect_Move(void *object);

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
extern u8 LinkedMessage_MercuryStarBagged[];
extern u8 Data_03001ebc[];
extern u8 LinkedMessage_TakeTheStarsToThem[];
extern u8 LinkedMessage_TheyWontLetJasmineGo[];
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

u16 Func_02004bb2(s32, s32);
s32 Func_02004bc6();
void Func_02004c2c();
void Func_0200529a();
void Func_02001504(void);
void Func_02001a00(void);
void Func_02002208(void);
void Func_02002d0a(void);
s32 Func_02004f5c();
void Func_02005630_b();
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
void Func_02005d62();
void Func_02005e0e();
s32 Func_020063a2();
void Func_02006410();
void Func_020064b8();
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
void Func_02006742();
void Func_020069a6();
void Func_02006bd2();
void Func_02006bd8();
void Func_02006bde();
s32 Func_020066f4();
void Func_02006dc8_a();
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
void Func_020073a6();
void Func_020073bc();
void Func_020073ee();
void Func_02007496();
void Func_020074a6();
void Func_020074b6();
void Func_020074ce();
void Func_02007834_b();
void Func_02007902();
void Func_02007938_b();
void Func_020079c4_b();
void Func_020079da();
void Func_02007a74();
void Func_02007c24();
void Func_02007ca8();
void Func_02007d5a();
void Func_02007da0();
void Func_02007f12();
void Func_02007f78();
void Func_02007fb4();
void Func_02007fe0();
void Func_02007ffc();
void Func_02008036_b();
void Func_02008064();
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

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
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

    obj = Object_Create(fourth, first, second, third);
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

void Scene_BagMercuryStar(void)
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
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
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
    mes = (s32)LinkedMessage_MercuryStarBagged;
    Value2(Engine_MessageShowCentered, mes, 1);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xe000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xe000, 20);
    Event_CloseScreen();
    Event_WaitForScreen();
    Camera_MoveTo(0x1ce0000, -1, 0x15e0000, 0);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_Jump(ACTOR_SUKURETA, 4, 30);
    Event_SetMessage(mes - 2);
    Func_02004c2c(9, 20);
    Event_CloseScreen();
    Event_WaitForScreen();
    Camera_MoveTo(0x2c80000, -1, 0x980000, 0);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    GameFlag_Set(FLAG_MERCURY_STAR_BAGGED);
    Event_End();
}

void Scene_HandOverStars(void)
{
    Event_Begin();
    Func_02001504();
    FieldScene_StagePairedActors();
    Func_02001a00();
    Scene_OfferGuarantee();
    Func_02002208();
    Scene_AlexTakesStars();
    GameFlag_Set(FLAG_STARS_GIVEN_TO_ALEX);
    Event_End();
    Func_02002d0a();
}

void Scene_BagJupiterStar(void)
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
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
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
    Message_ShowCentered(MSG_JUPITER_STAR_BAGGED, 1);
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
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 231, 0x1ea);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 30);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(180);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(80);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 60);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 246, 0x1df);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 10);
    /* Copy actor 0's stored fields at +8 and +16 onto actor 1, if a record
     * for actor 0 exists. */
    record = Actor_Get(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x101, 0x1eb);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 40);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 80);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x13333, 0x9999);
    Actor_SetSpeed(ACTOR_GERALD, 0x13333, 0x9999);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x109, 0x1c5);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x11a, 0x1d5);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 40);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
    Actor_Jump(ACTOR_GERALD, 6, 60);
    Actor_SetPosition(ACTOR_JASMINE, 0x1db0000, 0x14c0000);
    Actor_SetPosition(ACTOR_SUKURETA, 0x1eb0000, 0x14c0000);
    Actor_SetPosition(ACTOR_MENARDI, 0x1cb0000, 0x15c0000);
    Actor_SetPosition(ACTOR_SATUROS, 0x1fb0000, 0x15c0000);
    Camera_SetSpeed(0x73333, 0xe666);
    Camera_MoveTo(0x1e50000, -1, 0x1590000, 1);
    Actor_FaceDirection(ACTOR_JASMINE, 0x6000, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 0);
    Actor_FaceDirection(ACTOR_MENARDI, 0x5000, 0);
    Actor_FaceDirection(ACTOR_SATUROS, 0x5000, 0);
    Camera_WaitForMove();
    Event_Wait(40);
}

void Scene_OfferGuarantee(void)
{
    u32 i;
    s32 record;

    Actor_FaceDirection(ACTOR_JASMINE, 0x6000, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 0);
    Actor_FaceDirection(ACTOR_SATUROS, 0x8000, 0);
    Actor_FaceDirection(ACTOR_GARCIA_MASKED, 0x5000, 40);
    Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 40);
    Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
    Func_020056e4(11, 20);
    Actor_RunRepeatedMotion(ACTOR_GARCIA_MASKED, 2);
    Actor_SetAttachedEffect(ACTOR_GARCIA_MASKED, 0x102);
    Event_Wait(60);
    Func_02005704(12, 10);
    Actor_FaceActor(ACTOR_SATUROS, ACTOR_GARCIA_MASKED, 0);
    Actor_FaceActor(ACTOR_JASMINE, ACTOR_GARCIA_MASKED, 0);
    Actor_FaceActor(ACTOR_SUKURETA, ACTOR_GARCIA_MASKED, 0);
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
    Actor_FaceDirection(ACTOR_SATUROS, 0x8000, 10);
    Actor_SetAnimation(ACTOR_SATUROS, 3);
    Func_0200574c(10, 10);
    Actor_FaceDirection(ACTOR_MENARDI, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_MENARDI, 3);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_JASMINE, 0, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x8000, 20);
    Actor_StartRepeatedMotion(ACTOR_JASMINE, 2);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_JASMINE, 0xc000, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 10);
    Actor_FaceDirection(ACTOR_SATUROS, 0xb000, 40);
    Actor_SetAnimationAndWait(ACTOR_SATUROS, 3);
    Func_020057c4(10, 10);
    Actor_ShowEmote(ACTOR_JASMINE, 0x100, 20);
    Actor_SetAttachedEffect(ACTOR_GARCIA_MASKED, 0x102);
    Actor_RunRepeatedMotion(ACTOR_GARCIA_MASKED, 3);
    Event_Wait(40);
    Actor_SetPosition(ACTOR_GERALD, 0x15a0000, 0x1b80000);
    Task_Wait(1);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 20);
    Event_CloseScreen();
    Event_WaitForScreen();
    Actor_SetPosition(ACTOR_GERALD, 0x1180000, 0x1d60000);
    Camera_MoveTo(0x1050000, -1, 0x1d20000, 0);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x3000, 10);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 3);
    Event_AskYesNo(ACTOR_GERALD, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
    Event_Wait(10);
    Event_AskYesNo(ACTOR_GERALD, 0);
    Event_Wait(10);
    Event_SetMessage(MSG_MENARDI_DONT_YOU_WANT_THEM_SAFE);
    Event_ShowMessage(ACTOR_MENARDI, 0);
    Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 10);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 20);
    Event_CloseScreen();
    Event_WaitForScreen();
    Camera_MoveTo(0x1dd0000, -1, 0x14e0000, 0);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x8000, 30);
    Func_0200593e_a(9, 20);
    Actor_FaceDirection(ACTOR_JASMINE, 0, 40);
    Actor_SetAnimationAndWait(ACTOR_JASMINE, 4);
    Event_Wait(20);
    Func_0200595e(5, 10);
    Actor_RunRepeatedMotion(ACTOR_GARCIA_MASKED, 2);
    Event_Wait(80);
    Func_02005974(12, 20);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Actor_FaceDirection(ACTOR_JASMINE, 0xc000, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 20);
    Actor_SetAnimationAndWait(ACTOR_GARCIA_MASKED, 3);
    Func_020059a4(12, 20);
    Actor_StartRepeatedMotion(ACTOR_JASMINE, 2);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Actor_FaceDirection(ACTOR_GARCIA_MASKED, 0xb000, 40);
}

void Scene_UnmaskGarcia(void)
{
    u8 *obj;
    s32 other;
    s32 tbl;
    s32 left;
    s32 cnt;
    s32 mes_a;
    s32 mes_b;

    Audio_PlayCue(161);
    Actor_RunRepeatedMotion(ACTOR_GARCIA_MASKED, 3);
    Event_Wait(40);
    other = Value1(Engine_ActorGet, ACTOR_GARCIA_MASKED);
    if (other != 0) {
        Actor_SetPosition(ACTOR_GARCIA, *(s32 *)(other + 8), *(s32 *)(other + 16));
    }
    Actor_SetPosition(ACTOR_GARCIA_MASKED, 0, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GARCIA, 0x3000, 40);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 3);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 3);
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Func_02005a3e(5, 20);
    Actor_SetAnimationAndWait(ACTOR_GARCIA, 3);
    Event_Wait(10);
    Actor_StartRepeatedMotion(ACTOR_JASMINE, 3);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x8000, 10);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Func_02005a70(9, 40);
    Actor_SetAnimationAndWait(ACTOR_JASMINE, 3);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 40);
    Actor_RunRepeatedMotion(ACTOR_GARCIA, 2);
    Func_02005a9a(13, 20);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 1);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_GARCIA, 3);
    Event_Wait(10);
    Func_02005abe(13, 40);
    Actor_StartRepeatedMotion(ACTOR_SATUROS, 1);
    Actor_SetAnimationAndWait(ACTOR_SATUROS, 3);
    Event_Wait(10);
    Func_02005adc(10, 10);
    Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
    Actor_SetAnimation(ACTOR_MENARDI, 3);
    Func_02005af4(11, 80);
    Actor_RunRepeatedMotion(ACTOR_GARCIA, 2);
    Func_02005b04(13, 40);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Event_Wait(10);
    Func_02005b1a(5, 10);
    Actor_StartRepeatedMotion(ACTOR_GARCIA, 2);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_Wait(80);
    Actor_SetAnimationAndWait(ACTOR_JASMINE, 4);
    Event_Wait(20);
    Func_02005b46(5, 80);
    Actor_SetAnimationAndWait(ACTOR_GARCIA, 4);
    Func_02005b56(13, 80);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Event_Wait(4);
    Func_02005b6c(5, 20);
    Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
    Actor_SetAnimation(ACTOR_SATUROS, 3);
    Func_02005b84(10, 10);
    Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
    Func_02005b94(11, 10);
    Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
    Func_02005ba4(10, 10);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 80);
    Actor_ShowEmote(ACTOR_SUKURETA, 0x105, 80);
    Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
    Actor_FaceDirection(ACTOR_MENARDI, 0x5000, 40);
    Actor_StartRepeatedMotion(ACTOR_MENARDI, 2);
    Func_02005bde(11, 20);
    Event_CloseScreen();
    Event_WaitForScreen();
    Camera_MoveTo(0x1050000, -1, 0x1d20000, 0);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x3000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xb000, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_SetSpeed(ACTOR_GERALD, 0x8000, 0x4000);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 244, 0x1de);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x104, 0x1ea);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x3000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xb000, 20);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
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
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        mes_a = (s32)LinkedMessage_TakeTheStarsToThem;
        Event_SetMessage(mes_a);
        Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
        Event_Wait(10);
        if (cnt <= 2) {
            Func_02005d08(1, 30);
            Actor_WalkToAndWait(ACTOR_GERALD, 252, 0x1e6);
            Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
            Event_Wait(10);
            Func_02006410((mes_a + 1), 1, 0);
        } else {
            Event_SetMessage(MSG_GERALD_ILL_TAKE_THEM);
            Event_SayThenWait(ACTOR_GERALD, 30);
        }
    } else {
        if (cnt <= 2) {
            mes_b = (s32)LinkedMessage_TheyWontLetJasmineGo;
            Event_SetMessage(mes_b);
            Actor_RunRepeatedMotion(ACTOR_GERALD, 3);
            Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
            Func_02005d62(1, 10);
            Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
            Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
            Actor_SetSpeed(ACTOR_GERALD, 0x20000, 0x10000);
            obj = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
            obj[90] &= 254;
            Actor_WalkToAndWait(ACTOR_GERALD, 244, 0x1de);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
            Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
            Actor_SetDestination(ACTOR_PARTY_LEADER, 218, 0x1d7);
            Actor_WaitForMove(ACTOR_PARTY_LEADER);
            Func_020064b8((mes_b + 1), 1, 0);
            Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 30);
            {
                u8 flags = obj[90] | 1;

                obj[90] = flags;
            }
        } else {
            Event_SetMessage(MSG_GERALD_THEY_WONT_LET_JASMINE_GO);
            Actor_RunRepeatedMotion(ACTOR_GERALD, 3);
            Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
            Func_02005e0e(1, 10);
            Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 30);
        }
    }
    Camera_SetSpeed(0x8000, 0x1000);
    Camera_FollowActor(ACTOR_GERALD, 1);
    Camera_WaitForMove();
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 30);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_SetSpeed(ACTOR_GERALD, 0x8000, 0x4000);
    obj = Value1(Engine_ActorGet, ACTOR_GERALD);
    obj[90] &= 254;
    Actor_WalkToAndWait(ACTOR_GERALD, 0x108, 0x1e2);
    {
        u8 flags = obj[90] | 1;

        obj[90] = flags;
    }
    Actor_WalkToAndWait(ACTOR_GERALD, 0x116, 0x1e0);
    *(s32 *)(obj + 48) = 0x30000;
    *(s32 *)(obj + 52) = 0x20000;
    Audio_PlayCue(153);
    *(s32 *)(obj + 40) = 0x60000;
    Actor_SetAnimation(ACTOR_GERALD, 7);
    Actor_MoveToAndWait(ACTOR_GERALD, 0x138, 0x1d6);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Event_Wait(30);
    Audio_PlayCue(153);
    *(s32 *)(obj + 40) = 0x60000;
    Actor_SetAnimation(ACTOR_GERALD, 7);
    Actor_MoveToAndWait(ACTOR_GERALD, 0x156, 0x1d6);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Event_Wait(30);
    Audio_PlayCue(153);
    *(s32 *)(obj + 40) = 0x60000;
    Actor_SetAnimation(ACTOR_GERALD, 7);
    Actor_MoveToAndWait(ACTOR_GERALD, 0x178, 0x1d6);
    Actor_SetAnimation(ACTOR_GERALD, 1);
}

static __inline__ void SetFlagBits(u8 *flags, u8 bits)
{
    *flags |= bits;
}

void Scene_AlexTakesStars(void)
{
    u32 i;
    u8 *rec;
    s32 record;
    s32 none;

    Actor_RunRepeatedMotion(ACTOR_GERALD, 3);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_GERALD, 0x3000, 0);
    record = Actor_Get(ACTOR_ALEX);
    Actor_SetSpriteFlags(record, 0);
    Actor_SetChildValue(ACTOR_ALEX, 15);
    Actor_SetPosition(ACTOR_ALEX, 0x1880000, 0x1c60000);
    Func_02006104();
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 10);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 40);
    Actor_FaceDirection(ACTOR_ALEX, 0x5000, 10);
    Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
    Event_Wait(20);
    Event_SetMessage((s32)&LinkedMessage_AlexAsksForStars);
    Event_ShowMessage(ACTOR_ALEX, 0);
    Actor_SetPosition(ACTOR_SATUROS, 0x1d50000, 0x15c0000);
    Event_Wait(20);
    Event_SayThenWait(0x200a, 10);
    Event_SayThenWait(0x200a, 40);
    Actor_SetPosition(ACTOR_SATUROS, 0x1fb0000, 0x15c0000);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_Wait(40);
    Actor_SetSpeed(ACTOR_GERALD, 0x8000, 0x4000);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x185, 0x1d4);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 60);
    Func_02006054(1, 20);
    Func_02006742((s32)&LinkedMessage_AlexAsksForStars + 4, 1, 10);
    Actor_SetSpeed(ACTOR_GERALD, 0x8000, 0x4000);
    rec = Value1(Engine_ActorGet, ACTOR_GERALD);
    rec[90] &= 254;
    none = 0;
    Actor_WalkToAndWait(ACTOR_GERALD, 0x178, 0x1d6);
    Event_Wait(30);
    SetFlagBits(&rec[90], 1);
    Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
    Event_Wait(10);
    Event_SetMessage((s32)&LinkedMessage_AlexAsksForStars + 5);
    Func_020060be(14, 20);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 60);
    Actor_SetAnimationAndWait(ACTOR_ALEX, 3);
    Func_020060d8(14, 20);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 60);
    Actor_SetAnimationAndWait(ACTOR_ALEX, 3);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_ALEX, 0xc000, 20);
    Actor_SetChildValue(ACTOR_ALEX, 0x100);
    record = Actor_Get(ACTOR_ALEX);
    Actor_SetSpriteFlags(record, 0);
    rec = Value1(Engine_ActorGet, ACTOR_ALEX);
    rec[85] = none;
    Audio_PlayCue(220);
    for (i = 0; i != 30; i++) {
        *(s32 *)(rec + 12) += 0x10000;
        Event_Wait(1);
    }
    rec[85] = 5;
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Func_02006156(1, 10);
    Actor_ShowEmote(ACTOR_ALEX, 0x101, 60);
    Actor_FaceDirection(ACTOR_ALEX, 0x5000, 10);
    Func_02006174(1, 20);
    Actor_RunRepeatedMotion(ACTOR_ALEX, 1);
    Func_02006184(14, 20);
    Actor_ShowEmote(ACTOR_GERALD, 0x103, 20);
    Func_02006196(1, 30);
    Actor_ShowEmote(ACTOR_ALEX, 0x105, 80);
    Actor_FaceDirection(ACTOR_ALEX, 0xd000, 40);
    Actor_FaceDirection(ACTOR_SATUROS, 0x5000, 10);
    Event_CloseScreen();
    Event_WaitForScreen();
    Camera_MoveTo(0x1dd0000, -1, 0x14e0000, 0);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_SATUROS, 4);
    Func_020061f8(10, 10);
    Event_OpenMessage(ACTOR_MENARDI, 0);
    Camera_SetSpeed(0x66666, 0xcccc);
    Camera_MoveTo(0x1760000, -1, 0x1d60000, 1);
    Camera_WaitForMove();
    Actor_FaceDirection(ACTOR_ALEX, 0x5000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    if (Event_ChooseYesNo(1, 0) != 0) {
        Event_Wait(10);
        Value2(Engine_ActorSetAnimationAndWait, 14, 4);
        Event_SetMessage(MSG_ALEX_YOUR_FRIENDS);
        Event_OpenMessage(ACTOR_ALEX, 0);
        if (Event_ChooseYesNo(1, 0) == 0) {
            do {
                Event_Wait(20);
                Value2(Engine_ActorSetAnimationAndWait, 14, 4);
                Event_Wait(10);
                Event_SetMessage(MSG_ALEX_I_ASK_AGAIN);
                Event_OpenMessage(ACTOR_ALEX, 0);
            } while (Event_ChooseYesNo(1, 0) == 0);
        }
    }
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_ALEX, 3);
    Event_Wait(20);
    Event_SetMessage(MSG_ALEX_THANK_YOU);
    Func_020062de(14, 30);
    Actor_SetAnimationAndWait(ACTOR_ALEX, 3);
    Event_Wait(10);
    Func_020062f4(14, 30);
    rec[85] = 0;
    Actor_SetSpeed(ACTOR_ALEX, 0x26666, 0x13333);
    Call4(Func_020069a6, (s32)rec, 0x1cc0000, 0, 0x1680000);
    Actor_WaitForMove(ACTOR_ALEX);
    Actor_SetChildValue(ACTOR_ALEX, 0);
    record = Actor_Get(ACTOR_ALEX);
    Actor_SetSpriteFlags(record, 1);
    Event_Wait(30);
    Camera_FollowActor(ACTOR_GERALD, 1);
    Camera_WaitForMove();
    Event_Wait(40);
    Actor_ShowEmote(ACTOR_GERALD, 0x103, 40);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 3);
    Event_Wait(20);
    rec = Value1(Engine_ActorGet, ACTOR_GERALD);
    SetFlagBits(&rec[90], 1);
    *(s32 *)(rec + 48) = 0x30000;
    *(s32 *)(rec + 52) = 0x20000;
    Audio_PlayCue(153);
    *(s32 *)(rec + 40) = 0x60000;
    Actor_SetAnimation(ACTOR_GERALD, 7);
    Actor_MoveToAndWait(ACTOR_GERALD, 0x156, 0x1d6);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Event_Wait(30);
    Audio_PlayCue(153);
    *(s32 *)(rec + 40) = 0x60000;
    Actor_SetAnimation(ACTOR_GERALD, 7);
    Actor_MoveToAndWait(ACTOR_GERALD, 0x138, 0x1d6);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Event_Wait(30);
    Audio_PlayCue(153);
    *(s32 *)(rec + 40) = 0x60000;
    Actor_SetAnimation(ACTOR_GERALD, 7);
    Actor_MoveToAndWait(ACTOR_GERALD, 0x116, 0x1e0);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Event_Wait(30);
    Camera_SetSpeed(0x8000, 0x1000);
    Camera_FollowActor(ACTOR_PARTY_LEADER, 1);
    Actor_SetSpeed(ACTOR_GERALD, 0x19999, 0xcccc);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 4);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    {
        s32 slot = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);

        if (slot != 0) {
            Actor_SetDestination(ACTOR_GERALD, *(s16 *)(slot + 10), *(s16 *)(slot + 18));
        }
    }
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Func_02006bd2(220);
    Func_02006bd8(221);
    Func_02006bde(223);
}

void Scene_BagMarsStar(void)
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
    Message_ShowCentered(MSG_MARS_STAR_BAGGED, 1);
    Actor_SetPosition(ACTOR_JASMINE, 0x1330000, 0x1150000);
    Actor_SetPosition(ACTOR_SUKURETA, 0x1330000, 0x1150000);
    Actor_SetPosition(ACTOR_MENARDI, 0x1330000, 0x1150000);
    Actor_SetPosition(ACTOR_SATUROS, 0x1330000, 0x1150000);
    Actor_SetPosition(ACTOR_ALEX, 0x1330000, 0x1150000);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x13333, 0x9999);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 232, 156);
    Event_Wait(10);
    record = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(ACTOR_GERALD, 0x13333, 0x9999);
    Actor_WalkToAndWait(ACTOR_GERALD, 218, 172);
    Actor_FaceEachOther(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Event_Wait(20);
    Audio_PlayCue(145);
    Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
    Event_Wait(20);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x5000, 50);
    Audio_PlayCue(144);
    Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0, 50);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xb000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 0);
    Audio_PlayCue(144);
    Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Event_Wait(30);
    Actor_Jump(ACTOR_PARTY_LEADER, 2, 0);
    Actor_Jump(ACTOR_GERALD, 2, 20);
    Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
    Actor_Jump(ACTOR_GERALD, 6, 40);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Value0(Engine_EventCloseScreen);
    Event_WaitForScreen();
    Event_RequestExit(2);
}

/* Sukureta urges the party on, or apologizes once the Stars are gone. */
void Sukureta_Talk(void)
{
    Event_Begin();
    if (GameFlag_IsSet(FLAG_STARS_GIVEN_TO_ALEX)) {
        Event_SetMessage(MSG_SUKURETA_I_AM_RESPONSIBLE);
        Event_ShowMessage(ACTOR_SUKURETA, 0);
    } else {
        if (GameFlag_IsSet(FLAG_FIRST_STAR_BAGGED) == 0)
            Event_SetMessage(MSG_SUKURETA_BRING_THE_STARS_HERE);
        else
            Event_SetMessage(MSG_SUKURETA_FETCH_THE_OTHERS);
        Actor_FaceEachOther(ACTOR_SUKURETA, ACTOR_PARTY_LEADER, 0);
        Event_Wait(10);
        Event_ShowMessage(ACTOR_SUKURETA, 0);
    }
    Event_End();
}

/* Jasmine teases Sukureta, or wonders why her brother kept silent. */
void Jasmine_Talk(void)
{
    Event_Begin();
    if (GameFlag_IsSet(FLAG_STARS_GIVEN_TO_ALEX)) {
        Event_SetMessage(MSG_JASMINE_MY_BROTHER_IS_ALIVE);
        Event_ShowMessage(ACTOR_JASMINE, 0);
    } else {
        if (GameFlag_IsSet(FLAG_FIRST_STAR_BAGGED) == 0)
            Event_SetMessage(MSG_JASMINE_BE_CAREFUL);
        else
            Event_SetMessage(MSG_JASMINE_SUKURETA_IS_GIDDY);
        Actor_FaceEachOther(ACTOR_JASMINE, ACTOR_PARTY_LEADER, 0);
        Event_Wait(10);
        Event_ShowMessage(ACTOR_JASMINE, 0);
    }
    Event_End();
}

/* Saturos demands the last Elemental Star. */
void Saturos_Talk(void)
{
    Event_Begin();
    Event_SetMessage(MSG_SATUROS_BRING_THE_FINAL_STAR);
    Event_ShowMessage(ACTOR_SATUROS, 0);
    Event_End();
}

/* Menardi demands the last Elemental Star. */
void Menardi_Talk(void)
{
    Event_Begin();
    Event_SetMessage(MSG_MENARDI_BRING_THE_FINAL_GEM);
    Event_ShowMessage(ACTOR_MENARDI, 0);
    Event_End();
}

/* Garcia says nothing. */
void Garcia_Talk(void)
{
    Event_Begin();
    Event_SetMessage(MSG_GARCIA_SILENCE);
    Event_ShowMessage(ACTOR_GARCIA, 0);
    Event_End();
}

/* Alex counts the one Star left. */
void Alex_Talk(void)
{
    Event_Begin();
    Event_SetMessage(MSG_ALEX_ONLY_ONE_LEFT);
    Event_ShowMessage(ACTOR_ALEX, 0);
    Event_End();
}

/*
 * Same bracket and line call as the talk handlers, but the tail pairs actors
 * 9 and 10 through the overlay's own call.
 */
void SceneDialogue_RunLine1072WithPair9And10(void)
{
    Event_Begin();
    Event_SetMessage(MSG_NO_STARS_OUT_THERE);
    Func_02006a26(9, 10);
    Event_End();
}

void FieldScene_Forward72b4(void)
{
    Leader_CheckAhead();
}

void Scene_EnterStarRoom(void)
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
    rec2 = (u8 *)Actor_Get(8);
    *(s32 *)(rec2 + 24) = 0x1999;
    *(s32 *)(rec2 + 28) = 0x1999;
    rec = (u8 *)Actor_Get(ACTOR_PARTY_LEADER);
    p9 = *(u8 **)(rec + 80) + 38;
    *p9 = 0;
    *(s32 *)(rec + 24) = 0x1999;
    *(s32 *)(rec + 28) = 0x1999;
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 0x100);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x1d70000, 0x1220000);
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
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
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
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 1);
    {
        u8 *record = Actor_Get(ACTOR_PARTY_LEADER);
        s32 flags = 1 | record[35];
        record[35] = flags;
    }
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
    {
        s32 shown = 1;

        *p9 = shown;
    }
    ((void (*)())Engine_ActorSetPosition)(8, 0, 0);
    Camera_WaitForMove();
    Event_Wait(20);
    Camera_FollowActor(ACTOR_PARTY_LEADER, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1d7, 0x136);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(20);
    zero = 0;
    record = (u8 *)Actor_Get(ACTOR_PARTY_LEADER);
    if ((s32)record != 0) {
        Actor_SetPosition(ACTOR_JASMINE, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
    }
    record = (u8 *)Actor_Get(ACTOR_PARTY_LEADER);
    if ((s32)record != 0) {
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
    }
    Actor_SetSpeed(ACTOR_GERALD, 0x8000, 0x4000);
    Actor_SetSpeed(ACTOR_JASMINE, 0x8000, 0x4000);
    Actor_WalkTo(ACTOR_JASMINE, 0x1c5, 0x12e);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x1e9, 0x12e);
    Actor_SetAnimation(ACTOR_JASMINE, 1);
    Actor_FaceDirection(ACTOR_GERALD, 0x3000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x6000, 10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(ACTOR_JASMINE, 0x100, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Actor_Jump(ACTOR_PARTY_LEADER, 2, 0);
    Actor_Jump(ACTOR_JASMINE, 2, 0);
    Actor_Jump(ACTOR_GERALD, 2, 40);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 3);
    Actor_StartRepeatedMotion(ACTOR_JASMINE, 3);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 3);
    Event_Wait(40);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_JASMINE, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Event_Wait(60);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x13333, 0x9999);
    Actor_SetSpeed(ACTOR_JASMINE, 0x13333, 0x9999);
    Actor_SetSpeed(ACTOR_GERALD, 0x13333, 0x9999);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x1d7, 0x15a);
    Actor_WalkTo(ACTOR_JASMINE, 0x1af, 0x152);
    Actor_WalkTo(ACTOR_GERALD, 0x1ff, 0x152);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_WaitForMove(ACTOR_JASMINE);
    Actor_SetAnimation(ACTOR_JASMINE, 1);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x4ccc, 0x2666);
    Actor_SetSpeed(ACTOR_JASMINE, 0x4ccc, 0x2666);
    Actor_SetSpeed(ACTOR_GERALD, 0x4ccc, 0x2666);
    Actor_FaceDirection(ACTOR_JASMINE, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 60);
    Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 60);
    Actor_FaceDirection(ACTOR_JASMINE, 0, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 40);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 40);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_JASMINE, 0x101, 60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x3000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x6000, 60);
    rec = (u8 *)Actor_Get(ACTOR_SUKURETA);
    p9b = *(u8 **)(rec + 80) + 38;
    *p9b = zero;
    *(s32 *)(rec + 24) = 0x1999;
    *(s32 *)(rec + 28) = 0x1999;
    *(s32 *)(rec2 + 24) = 0x1999;
    *(s32 *)(rec2 + 28) = 0x1999;
    Actor_SetChildValue(ACTOR_SUKURETA, 0x100);
    Actor_SetPosition(ACTOR_SUKURETA, 0x1d70000, 0x1220000);
    p10b = rec + 85;
    *p10b = zero;
    *(s32 *)(rec + 12) = 0x280000;
    Event_Wait(1);
    Event_SetMessage(MSG_SUKURETA_WHERE_ARE_WE);
    Event_ShowMessage(ACTOR_SUKURETA, 0);
    Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
    Actor_Jump(ACTOR_JASMINE, 4, 0);
    Actor_Jump(ACTOR_GERALD, 4, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Call3((void (*)())Engine_ActorFaceDirection, 5, 0xe000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xb000, 0);
    Camera_SetSpeed(0x20000, 0x4000);
    Camera_MoveTo(0x1d70000, -1, 0x1350000, 1);
    Camera_WaitForMove();
    Actor_SetPosition(8, 0x1d70000, 0x1220000);
    Audio_PlayCue(190);
    Actor_SetSpritePriority(ACTOR_SUKURETA, 2);
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
    Actor_SetSpritePriority(ACTOR_SUKURETA, 1);
    {
        u8 *record = Actor_Get(ACTOR_SUKURETA);
        s32 flags = 1 | record[35];
        record[35] = flags;
    }
    Actor_SetChildValue(ACTOR_SUKURETA, 0);
    {
        s32 shown = 1;

        *p9b = shown;
    }
    Actor_SetPosition(8, 0, 0);
    Call1_020027f8((void (*)())Engine_EventWait, 30);
    Actor_SetSpeed(ACTOR_SUKURETA, 0x13333, 0x9999);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1d7, 0x132);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_SUKURETA, 0x100, 0);
    Actor_Jump(ACTOR_SUKURETA, 2, 80);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 3);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x8000, 30);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0, 30);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x4000, 30);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 3);
    Event_Wait(30);
    Func_020070ae(9, 20);
    Actor_SetSpeed(ACTOR_SUKURETA, 0x26666, 0x13333);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1a7, 0x132);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xb000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xa000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xb000, 30);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 3);
    Event_Wait(40);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x207, 0x132);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xd000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xe000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 20);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_Wait(30);
    base5_4009 = (s32)Data_00004009;
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 4);
    Event_Wait(40);
    Func_02007172(base5_4009, 30);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_JASMINE, 0);
    Event_Wait(40);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 40);
    Func_020071a4(1, 40);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 20);
    Func_020071b8(base5_4009, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xe000, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_JASMINE, 0x101, 40);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 4);
    Func_020071f4(base5_4009, 10);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_JASMINE, 3);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 3);
    Event_Wait(40);
    Camera_SetSpeed(0x8000, 0x1000);
    Camera_MoveTo(0x2150000, -1, 0x1530000, 1);
    Actor_SetSpeed(ACTOR_SUKURETA, 0x8000, 0x4000);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x215, 0x153);
    Camera_WaitForMove();
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0, 20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xd000, 40);
    Actor_FaceDirection(ACTOR_SUKURETA, 0, 40);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 60);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 3);
    Event_Wait(30);
    Camera_FollowActor(ACTOR_SUKURETA, 1);
    Actor_SetSpeed(ACTOR_SUKURETA, 0x19999, 0xcccc);
    Actor_WalkTo(ACTOR_SUKURETA, 0x1c7, 0x168);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x2000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x5000, 0);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1c7, 0x168);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 30);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 3);
    Event_Wait(30);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1d7, 0x168);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 30);
    Actor_ShowEmote(ACTOR_SUKURETA, 0x100, 0);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_Wait(30);
    Func_020073a6(9, 30);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 3);
    Event_Wait(20);
    Func_020073bc(9, 30);
    Actor_SetSpeed(ACTOR_JASMINE, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_JASMINE, 0x1b8, 0x15a);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_JASMINE, 0x2000, 10);
    Func_020073ee(5, 20);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x1ef, 0x15a);
    Camera_WaitForMove();
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 10);
    Event_OpenMessage(ACTOR_GERALD, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 0);
    if (Event_ChooseYesNo(0, 0) == 1) {
        bump_step(1);
    }
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 30);
    Event_SayThenWait(ACTOR_SUKURETA, 30);
    Event_SetMessage(MSG_SUKURETA_THE_ELEMENTS_ARE_THE_SOURCE);
    Actor_FaceDirection(ACTOR_JASMINE, 0x2000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 30);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Func_02007496(9, 10);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 3);
    Func_020074a6(9, 20);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Func_020074b6(9, 10);
    Actor_SetAttachedEffect(ACTOR_SUKURETA, 0x102);
    Event_Wait(40);
    Func_020074ce(9, 10);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x2000, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_JASMINE, 2);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x2000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 20);
    Actor_Stop(ACTOR_SUKURETA);
    Actor_Stop(ACTOR_PARTY_LEADER);
    Actor_Stop(ACTOR_JASMINE);
    Actor_Stop(ACTOR_GERALD);
    Camera_SetSpeed(0x80000, 0x10000);
    Camera_MoveTo(0x2c70000, -1, 0x1d00000, 1);
    Camera_WaitForMove();
    Actor_SetPosition(ACTOR_SUKURETA, 0x24d0000, 0x1610000);
    Event_Wait(40);
    Event_ShowMessage(0x1009, 0);
    Actor_SetPosition(ACTOR_SUKURETA, 0x1d70000, 0x1680000);
    Event_Wait(80);
    Camera_MoveTo(0x1d70000, -1, 0x1720000, 1);
    Camera_WaitForMove();
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 3);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xd000, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xe000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 20);
    Camera_MoveTo(0x2c70000, -1, 0x930000, 1);
    Camera_WaitForMove();
    Actor_SetPosition(ACTOR_SUKURETA, 0x2540000, 0xee0000);
    Event_Wait(40);
    Event_ShowMessage(0x1009, 0);
    Actor_SetPosition(ACTOR_SUKURETA, 0x1d70000, 0x1680000);
    Event_Wait(80);
    Camera_MoveTo(0x1d70000, -1, 0x1720000, 1);
    Camera_WaitForMove();
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_Wait(10);
    Actor_Jump(ACTOR_SUKURETA, 4, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(ACTOR_JASMINE, 1);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xa000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 20);
    Camera_MoveTo(0xe70000, -1, 0x930000, 1);
    Camera_WaitForMove();
    Actor_SetPosition(ACTOR_SUKURETA, 0x1340000, 0xfa0000);
    Event_Wait(40);
    Event_ShowMessage(0x2009, 0);
    Actor_SetPosition(ACTOR_SUKURETA, 0x1d70000, 0x1680000);
    Event_Wait(80);
    Camera_MoveTo(0x1d70000, -1, 0x1720000, 1);
    Camera_WaitForMove();
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 3);
    Event_Wait(10);
    Actor_Jump(ACTOR_SUKURETA, 6, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_JASMINE, 2);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 20);
    Camera_MoveTo(0xe70000, -1, 0x1d00000, 1);
    Camera_WaitForMove();
    Actor_SetPosition(ACTOR_SUKURETA, 0x1320000, 0x16a0000);
    Event_Wait(40);
    Event_ShowMessage(0x2009, 0);
    Actor_SetPosition(ACTOR_SUKURETA, 0x1d70000, 0x1680000);
    Event_Wait(80);
    Camera_MoveTo(0x1d70000, -1, 0x1720000, 1);
    Camera_WaitForMove();
    Event_Wait(30);
    Actor_ShowEmote(ACTOR_SUKURETA, 0x104, 60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x2000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 40);
    Actor_ShowEmote(ACTOR_JASMINE, 0x102, 40);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 1);
    Func_02007834_b(5, 20);
    Actor_Jump(ACTOR_SUKURETA, 4, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 20);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Call2(Event_SayThenWait, 0xa009, 10);
    Actor_StartRepeatedMotion(ACTOR_SUKURETA, 3);
    Call2(Event_SayThenWait, 0xa009, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_JASMINE, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 60);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 20);
    Actor_SetAnimation(ACTOR_SUKURETA, 4);
    Event_OpenMessage(0x8009, 0);
    if (Event_ChooseYesNo(0, 0) == 1) {
        bump_step(1);
    }
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 10);
    Event_SayThenWait(ACTOR_GERALD, 10);
    Event_SetMessage(MSG_SUKURETA_THE_WISDOM_STONE);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 20);
    Actor_Jump(ACTOR_SUKURETA, 4, 40);
    Func_02007902(0x8009, 10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_JASMINE, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 80);
    Actor_SetAttachedEffect(ACTOR_SUKURETA, 0x102);
    Event_Wait(40);
    Func_02007938_b(0x8009, 40);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x106, 0);
    Actor_ShowEmote(ACTOR_JASMINE, 0x106, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x106, 60);
    Actor_FaceDirection(ACTOR_JASMINE, 0, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 20);
    Event_OpenMessage(0x8009, 0);
    if (Event_ChooseYesNo(0, 0) == 1) {
        bump_step(1);
    }
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 1);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_JASMINE, 0x2000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x5000, 20);
    Func_020079c4_b(0x8009, 40);
    Event_SetMessage(MSG_THE_STARS_HAVE_ENORMOUS_POWER);
    Actor_SetAnimation(ACTOR_JASMINE, 4);
    Func_020079da(5, 10);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 10);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 1);
    Actor_Jump(ACTOR_SUKURETA, 4, 40);
    Call2(Event_SayThenWait, 0xa009, 10);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(ACTOR_JASMINE, 1);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 10);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 1);
    Event_SayThenWait(0x8009, 40);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
    Actor_ShowEmote(ACTOR_JASMINE, 0x105, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x105, 120);
    Actor_ShowEmote(ACTOR_GERALD, 0x107, 0);
    Actor_Jump(ACTOR_GERALD, 4, 40);
    Func_02007a74(1, 10);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 1);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x4000, 80);
    Event_SayThenWait(0x8009, 10);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 30);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_JASMINE, 0, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 30);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x2000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x5000, 40);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_JASMINE, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Event_Wait(80);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 3);
    Event_Wait(20);
    Event_SayThenWait(0x8009, 40);
    rec = (u8 *)Actor_Get(ACTOR_SUKURETA);
    v6 = 192;
    Task_Wait(6);
    *(s32 *)(rec + 48) = 0x30000;
    *(s32 *)(rec + 52) = 0x20000;
    Audio_PlayCue(153);
    *(s32 *)(rec + 40) = (v6 << 11);
    Actor_MoveToAndWait(ACTOR_SUKURETA, 0x1d7, 0x18b);
    Task_Wait(6);
    Actor_SetSpeed(ACTOR_SUKURETA, 0x4ccc, 0x2666);
    p9c = rec + 90;
    *p9c &= 254;
    Actor_SetDestination(ACTOR_SUKURETA, 0x1d9, 0x18b);
    Actor_WaitForMove(ACTOR_SUKURETA);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Actor_SetDestination(ACTOR_SUKURETA, 0x1d5, 0x18b);
    Actor_WaitForMove(ACTOR_SUKURETA);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Actor_SetDestination(ACTOR_SUKURETA, 0x1d7, 0x18b);
    Actor_WaitForMove(ACTOR_SUKURETA);
    Func_02007c24(9, 10);
    Actor_SetSpeed(ACTOR_SUKURETA, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1d7, 0x19b);
    Actor_SetSpeed(ACTOR_SUKURETA, 0x4ccc, 0x2666);
    *p9c &= 254;
    Actor_SetDestination(ACTOR_SUKURETA, 0x1da, 0x19b);
    Actor_WaitForMove(ACTOR_SUKURETA);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 3);
    Actor_SetDestination(ACTOR_SUKURETA, 0x1d4, 0x19b);
    Actor_WaitForMove(ACTOR_SUKURETA);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 3);
    Actor_SetDestination(ACTOR_SUKURETA, 0x1d7, 0x19b);
    Actor_WaitForMove(ACTOR_SUKURETA);
    Actor_SetAttachedEffect(ACTOR_SUKURETA, 0x102);
    Actor_StartRepeatedMotion(ACTOR_SUKURETA, 3);
    Func_02007ca8(9, 10);
    Actor_SetSpeed(ACTOR_SUKURETA, 0x3333, 0x1999);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1d7, 0x18b);
    Actor_EnableActionCallback(ACTOR_SUKURETA, 1);
    Event_Wait(30);
    {
        s32 flags = 1 | *p9c;
        *p9c = flags;
    }
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 60);
    Task_Wait(6);
    *(s32 *)(rec + 48) = 0x30000;
    *(s32 *)(rec + 52) = 0x20000;
    Audio_PlayCue(153);
    *(s32 *)(rec + 40) = (v6 << 11);
    Actor_MoveToAndWait(ACTOR_SUKURETA, 0x1d7, 0x168);
    Task_Wait(6);
    Event_Wait(40);
    Actor_ShowEmote(ACTOR_SUKURETA, 0x102, 80);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(ACTOR_JASMINE, 0x102, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 80);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 1);
    Actor_ShowEmote(ACTOR_JASMINE, 0x107, 40);
    Func_02007d5a(5, 20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 40);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 4);
    Call2(Event_SayThenWait, 0xa009, 30);
    Actor_FaceDirection(ACTOR_JASMINE, 0, 30);
    Actor_ShowEmote(ACTOR_JASMINE, 0x106, 60);
    Actor_FaceDirection(ACTOR_JASMINE, 0x2000, 30);
    Func_02007da0(5, 20);
    Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
    Actor_Jump(ACTOR_GERALD, 4, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 40);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 30);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xd000, 30);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 1);
    Event_Wait(20);
    Event_SayThenWait(0x8009, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x5000, 30);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_JASMINE, 3);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_JASMINE, 0, 30);
    Event_OpenMessage(ACTOR_JASMINE, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    while (Event_ChooseYesNo(0, 0) != 0) {
        Event_SetMessage(MSG_JASMINE_WONT_YOU_PLEASE);
        Actor_ShowEmote(ACTOR_JASMINE, 0x107, 0);
        Actor_Jump(ACTOR_JASMINE, 4, 60);
        Event_OpenMessage(ACTOR_JASMINE, 0);
    }
    Event_SetMessage(MSG_JASMINE_THANK_YOU);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_JASMINE, 3);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_JASMINE, 0x2000, 30);
    Func_02007f12(5, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 10);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 30);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 3);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 30);
    Actor_SetSpeed(ACTOR_SUKURETA, 0x3333, 0x1999);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1d7, 0x160);
    Event_Wait(20);
    base5_8009 = (s32)Data_00008009;
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_Wait(20);
    Func_02007f78(base5_8009, 60);
    *p9c &= 254;
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1c8, 0x168);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Event_Wait(20);
    Event_SetMessage(MSG_THE_BAGS_ARE_FROM_SUKURETAS_COTTAGE);
    Func_02007fb4(5, 30);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 30);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 3);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 60);
    Func_02007fe0(1, 10);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xd000, 20);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 3);
    Func_02007ffc(base5_8009, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 40);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 40);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Func_02008036_b(base5_8009, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x5000, 30);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 3);
    Event_Wait(10);
    Func_02008064(base5_8009, 30);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 30);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(10);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    record = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
    if ((s32)record != 0) {
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
    }
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    GameFlag_Set(FLAG_STAR_ROOM_EXPLAINED);
    Func_0200885a(5);
    Scene_GiveMythrilBags();
    Map_CopyCellAttributes(8, 0, 5, 1, 27, 17);
    gEventWork->transition_frames = 16;
    GameFlag_Clear(FLAG_STATUE_HALL_TRAP_ARMED);
    Event_End();
}

/* Shows the next line of dialogue, then holds the scene for a moment. */
void Event_SayThenWait(s32 speaker, s32 frames)
{
    Event_ShowMessage(speaker, 0);
    Event_Wait(frames);
}

/*
 * Drain until room: save the s16 counter at scene workspace + 472, prime two
 * channels, then loop while fewer than four of thirty slots are free,
 * requesting more and passing on any event pair that is not -1. On exit it
 * flushes four times with id 224 and restores the saved counter. The 148-byte
 * owner includes its three-word literal pool. Callee roles are not established.
 */
void Scene_GiveMythrilBags(void)
{
    u8 *work = *(u8 **)0x03001ebc;
    s16 saved = *(s16 *)(work + 472);
    s32 first;
    s32 second;
    s32 cnt;

    Audio_PlayCue(0x53);
    Item_ShowFound(ITEM_MYTHRIL_BAG, 3);
    Message_ShowCentered(MSG_GOT_FOUR_MYTHRIL_BAGS, 1);
    do {
        cnt = 30 - Func_02008cd0(0);
        cnt -= Func_02008cda(1);

        if (cnt <= 3) {
            Message_ShowCentered(MSG_TOO_MANY_ITEMS, 1);
            if (Func_02008e80(&second, &first) != -1)
                Func_02008d08(second, first);
        }
    } while (cnt <= 3);
    Func_02008cd8(ITEM_MYTHRIL_BAG);
    Func_02008cde(ITEM_MYTHRIL_BAG);
    Func_02008ce4(ITEM_MYTHRIL_BAG);
    Func_02008cea(ITEM_MYTHRIL_BAG);
    *(s16 *)(work + 472) = saved;
}
