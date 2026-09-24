#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

enum TaskMessage {
    MSG_WOULD_LIKE_FRIEND_CHEER_FOR = 0x207d,
    MSG_IF_KNOW_WHO_WANT_CHEER = 0x207e,
    MSG_ROBIN_WILL_CHEER_FOR_WAY = 0x207f,
    MSG_DO_YOUR_BEST = 0x2083,
    MSG_UNFORTUNATELY_WE_HAVE_FULL_HOUSE = 0x2084,
    MSG_MATCH_ABOUT_BEGIN_PLEASE_TAKE = 0x2085,
    MSG_OPERATOR_LIFTS_WILL_CHEER_FOR = 0x20a1,
    MSG_SHIFTING_FLOOR_STAGE = 0x20a2,
    MSG_HERE_YOUR_OBJECTIVE_RIDE_LOGS = 0x20a5,
    MSG_LOG_ROLLING_STAGE = 0x20a6,
    MSG_ROBIN_DID_GET_GOOD_LOOK = 0x20e5,
    MSG_WAIT_SHOULDNT_DECIDE_WHERE_BEST = 0x20e8
};


#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define HexDigits Data_0200c250
#define GetPartyInteractionRecord Func_02005e6a
#define GetPartyMemberCount Func_02005e3a_a

typedef struct Ctl {
    s16 f0;
    s16 f2;
    s16 f4;
    s16 f6;
    s16 f8;
} Ctl;

typedef struct Sub {
    u8 pad00[0x28];
    s16 *f28;
} Sub;

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[0x3c];
    Sub *f50;
    u8 f54;
} Obj;

typedef struct Obj_02000310 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[28];
    s32 f30;
    s32 f34;
} Obj_02000310;

typedef struct PartyInteractionRecord {
    u8 padding_00[10];
    s16 x;
    u8 padding_0c[6];
    s16 y;
} PartyInteractionRecord;

/* The two mode records the entry point seeds; the halfword at +26 holds the
 * per-mode span in sixtieths. */
struct ModeRecord {
    u8 pad[26];
    u16 span;
};

typedef struct Obj_0200325c {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[20];
    s32 f28;
    u8 f2c[41];
    u8 f55;
} Obj_0200325c;

typedef struct Obj_020032f0 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[65];
    u8 f55;
    u8 f56[14];
    s16 f64;
} Obj_020032f0;

/* The active subject's handle sits 500 bytes into the shared table. */
typedef struct ActiveSubjectSlot {
    u8 pad[500];
    void *handle;
} ActiveSubjectSlot;

typedef struct SceneRecord {
    u8 pad_00[6];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    u8 pad_14[14];
    u8 state;
    u8 pad_23;
    s32 motion_24;
    u8 pad_28[4];
    s32 motion_2c;
    s32 rate_x;
    s32 rate_z;
    u8 pad_38[33];
    u8 flags;
} SceneRecord;

typedef struct Position3 {
    s32 x;
    s32 y;
    s32 z;
} Position3;

extern u8 Data_0200c250[];
extern u8 Data_0200c194[];
extern Ctl Data_02001000;
extern u8 *Data_03001f3c;
extern u8 Data_0200b459[];
extern s16 Data_0200cb46;
extern u8 Value_0000008f;
extern u8 Value_00000090;
extern u8 Value_00002076;
extern u8 Value_00002078;
extern u8 Value_0000207a;
extern u8 LinkedMessage_WouldYouLikeHearDescription;
extern u16 Data_0200cc30;
extern u16 Data_0200cc04;
extern u16 Data_0200cc3c;
extern s32 Data_0200cc40;
extern u16 Data_0200cc98;
extern u16 Data_0200cc0c;
extern s32 Data_0200cc10;
extern s32 Data_0200ca1e;   /* Default handler. */
extern s32 Data_0200c0ce;   /* Handler for mode 2. */
extern s32 Data_0200ca4a;   /* Handler for mode 4. */
extern void Func_02004db0(s32 mode);          /* Func_02002e54 veneer #1 */
extern void Func_0200533c(s32 style, s32 variant); /* Func_020033d8 veneer #1 */
extern void Func_02004df4(s32 mode);          /* Func_02002e54 veneer #2 */
extern void Func_02005380(s32 style, s32 variant); /* Func_020033d8 veneer #2 */
extern s32 Func_02006b0a(void);               /* Func_080f9048 veneer (loop check) */
extern void Func_02004e1e(s32 mode);          /* Func_02002e54 veneer #3 */
extern void Func_020053aa(s32 style, s32 variant); /* Func_020033d8 veneer #3 */
extern void Func_020053be(s32 style, s32 variant); /* Func_020033d8 veneer #4 */
extern void Func_02004e4c(s32 mode);          /* Func_02002e54 veneer #4 */
extern void Func_020053d8(s32 style, s32 variant); /* Func_020033d8 veneer #5 */
extern void Func_02004e66(s32 mode);          /* Func_02002e54 veneer #5 */
extern void Func_020053f2(s32 style, s32 variant); /* Func_020033d8 veneer #6 */
extern void Func_02006b44(void);              /* Func_0808a4f0 veneer */
extern struct ModeRecord Data_0200cac8;
extern struct ModeRecord Data_0200c0f6;
extern u16 Data_0200cc94;
extern u16 Data_0200cc20;
extern u16 Data_0200cbf8;
extern u16 Data_0200cc14;
extern u16 Data_0200cc2c;
extern u16 Data_0200cc00;
extern u16 Data_0200cca0;
extern u16 Data_0200cc44;
extern u16 Data_0200cc5c;
extern u16 Data_0200cbf0;
extern u8 Data_0200c264[];
extern u8 Data_0200c288[];
extern u32 Data_0200c3d4[];

void *Func_02003fb4(s32);
s32 *Func_02004598();
s32 *Func_020045b2();
s32 *Func_020045cc();
void Func_02000b66(s32, s32, s32);
void Func_02000b7a(s32, s32, s32);
void Func_02000b8e(s32, s32, s32);
void Func_02000ba2(s32, s32, s32);
void Func_02000bb6(s32, s32, s32);
void Func_02000bca(s32, s32, s32);
void Func_02005eca(s32 arg0, s32 arg1);
void Func_02005ed4(s32 arg0, s32 arg1);
void Func_02005ede(s32 arg0, s32 arg1);
void Func_02005ee8(s32 arg0, s32 arg1);
void Func_02005ef2(s32 arg0, s32 arg1);
void Func_02005efc(s32 arg0, s32 arg1);
u8 *Func_020077f6();
s32 Func_02007810();
u8 *Func_020079a8();
u8 *Func_020079b0();
void Func_02007886();
s32 Func_020078aa();
void Func_02007856();
void Func_020078a4();
void Func_0200b638();
s32 Func_0200791e(void);
void Func_020078ee(s32, s32);
void Func_020078c2(s32, s32);
s32 *Func_0200402c(s32);
void Func_02003ff2(s32, s32);
s32 *Func_02004052(s32);
void Func_02004018(s32, s32);
s32 *Func_02004076(s32);
void Func_0200403c(s32, s32);
void Func_020042aa(s32, s32, s32);
s32 Func_02004700(s32, s32);
void Func_02004a58(s32);
void Func_02004790(s32);
void Func_02008714();
s32 Func_02006d3e(void);
u8 *Func_020041b6(s32);
u8 *Func_020041ea(s32);
s32 Func_02005a38(s32);
s32 Func_02005a42(s32);
s32 Func_02005a60(s32);
s32 Func_02005a6a(s32);
s32 Func_02005a84(s32);
s32 Func_02005a8e(s32);
Obj *Func_02005c38(s32);
Obj *Func_02005c46(s32);
void Func_02005cb0_a();
void Func_02005ca0(s32, s32);
void Func_0200396a(s32);
void Func_02006084(void);
void Func_02005ebc(s32, s32);
s32 Func_02005f1e(s32);
Obj_02000310 *Func_02004282(s32);
Obj_02000310 *Func_0200428a(s32);
void Func_020041f2(Obj_02000310 *, s32, s32, s32);
void Func_0200421e(Obj_02000310 *, s32, s32, s32);
void Func_02004250(Obj_02000310 *);
s32 Func_02004674();
s32 Func_0200467e();
void Func_02004692();
void Func_020046a6();
void Func_020046ba();
void Func_020046ce();
void Func_020046ec();
void Func_02004700_a();
void Func_02004714();
void Func_02004728();
s32 Func_0200475c();
s32 Func_02004770();
s32 Func_02004784();
s32 Func_02004798();
s32 Func_020047b6();
s32 Func_020047ca();
s32 Func_020047de();
s32 Func_020047f2();
void Func_020046ac();
void Func_02004a74();
void Func_0200322e();
s32 Func_02003444();
void Func_020035b8();
void Func_02004046();
void Func_02004274();
void Func_02003346();
s32 Func_0200355c();
void Func_02003750();
void Func_0200417a();
void Func_020043fe();
PartyInteractionRecord *Func_02005e6a(void);
s32 Func_02005e3a_a(void);
void Func_02005f72(s32, s32);
s32 Func_02006054();
s32 Func_0200607e();
void Func_020060e0();
s32 Func_020060ec_a();
void Func_020060f4();
void Func_0200611e();
void Func_02006130();
s32 Func_020061ca();
void Func_02006240();
void Func_0200624e_a();
void Func_0200625c();
s32 Func_02006282();
s32 Func_0200628e();
void Func_02006374();
u16 *Func_0200625c_a(s32 no);
void Func_0200626e_a(s32 no, s32 val);
void Func_0200628a(s32 no, s32 index);
void Func_02006700(s32 taskAddress, s32 frameBudget);
void Func_02004eda();
void Func_02004f06();
s32 Func_02005466();
s32 Func_02005492();
s32 Func_02006c28();
u8 *Func_02006d98();
void Func_02006cb6();
void Func_02005e32(u32 x, u32 y, u32 style);
void Func_02006d9c(s32 task_address, s32 budget);
void Func_02006df8(s32 task_address, s32 frame_budget);
void Func_02006e36(void *callback);
void Func_02006e90(s32 slot);
s32 *Func_02007128(s32 selector);
void Func_02006f34(s32 *actor);
void Func_02006f58(s32 *actor, s32 x, s32 y, s32 z);
u8 *Func_02007168(s32 no);
void Func_02006f74(void);
void Func_02006f98(u8 *obj, s32 x, s32 z, s32 arg3);
void Func_02006fa6(u8 *obj);
void Func_0200706e(s32, s32, s32 *);
Obj *Func_0200710a(s32, s32, s32, s32);
Obj *Func_02007254(s16);
void Func_0200718e(Obj *, s32, s32, s32);
u8 *Func_02007c5c();
void Func_02007b0e();
s32 *Func_02007896();
void Func_02007b40();
s32 *Func_020078c8();
SceneRecord *Func_02007726(Position3 *, SceneRecord *);
SceneRecord *Func_02007750(Position3 *, SceneRecord *);
SceneRecord *Func_0200777c(Position3 *, SceneRecord *);

/*
 * Mode task setup for resource_3bb.  It seeds a second per-instance task's
 * state and picks which of five addresses that task dispatches through.  The
 * 144-byte owner includes its 15-word literal pool.
 */

/*
 * resource_3bb scripted transition owner at 0x020029d0, 268 bytes including
 * alignment and its three-word pool.  Mode zero is the short opening; every
 * other mode runs the complete multi-stage transition and publishes flag
 * 0x123 when it closes.
 *
 * Call symbols are per-site (the raw disassembly shows a DIFFERENT veneer
 * target at every occurrence, including every repeated Func_0808a010,
 * Audio_PlayCue, Func_02002e54, Func_020033d8, Func_0808a018/360/370/020
 * call) -- declared/named as the literal per-site targets, not the shared
 * ultimate-destination symbol.
 */

/* Seed the marker cycle directly from caller coordinates and install it. */

/* Per-site veneers (raw sub_ symbols from the overlay .s). */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */

/* This overlay's own occupancy lookup for a cell. The record pointer the call
 * sites also load is spelled here, although the lookup itself uses only the
 * position. */

/* In-image direction table: sixteen packed steps, high half x, low half z. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
void FieldScene_RunMiddleSequence(s32 mode, s32 owner, s32 base);

void SceneActor_PlaceWithScale14000(s32 no, s32 x, s32 z);

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{

    extern s32 Data_02000240_t[][1];

    return f(a0);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{

    extern s32 Data_02000240_t[][1];

    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{

    extern s32 Data_02000240_t[][1];

    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{

    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{

    f(a0, a1, a2);
}

static __inline__ void Call3_02000b38(void (*f)(), s32 a0, s32 a1, s32 a2)
{

    f(a0, a1, a2);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{

    extern s16 Data_02000240_t[][1];

    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{

    extern s16 Data_02000240_t[][1];

    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{

    extern s16 Data_02000240_t[][1];

    return f(a0, a1, a2);
}

static __inline__ void Call2_02001538(void (*f)(), s32 a0, s32 a1)
{

    extern s16 Data_02000240_t[][1];
    f(a0, a1);
}

static __inline__ s32 Value2_02001538(s32 (*f)(), s32 a0, s32 a1)
{

    extern s16 Data_02000240_t[][1];
    return f(a0, a1);
}

static __inline__ s32 Value3_02001538(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{

    extern s16 Data_02000240_t[][1];
    return f(a0, a1, a2);
}

static __inline__ void Call4_02001538(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{

    extern s16 Data_02000240_t[][1];
    f(a0, a1, a2, a3);
}

static inline void InitializeActorZero(void)
{
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
}

static inline void InitializeSelectedActor(s32 actorId)
{
    Actor_SetSpeed(actorId, 0x10000, 0x8000);
}

static __inline__ s32 Value0(s32 (*f)())
{

    return f();
}

static __inline__ void Call1_02002114(void (*f)(), s32 a0)
{

    f(a0);
}

static __inline__ s32 Value1_02002114(s32 (*f)(), s32 a0)
{

    return f(a0);
}

static __inline__ s32 Value2_02002114(s32 (*f)(), s32 a0, s32 a1)
{

    return f(a0, a1);
}

static __inline__ s32 Value3_02002114(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{

    return f(a0, a1, a2);
}

static __inline__ void Call1_02002adc(void (*f)(), s32 a0)
{

    extern u8 Data_02000240_t[][2];
    void Event_OpenScreen();

    f(a0);
}

static __inline__ s32 Value2_02002adc(s32 (*f)(), s32 a0, s32 a1)
{

    extern u8 Data_02000240_t[][2];
    void Event_OpenScreen();

    return f(a0, a1);
}

#if defined(TBS_EDITION_JA)
#define SCENE_TEXT_VALUE 0x98f
#elif defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define SCENE_TEXT_VALUE 0xa5a
#else
#define SCENE_TEXT_VALUE 0x96a
#endif
#if defined(TBS_EDITION_DE)
#define SCENE_WORK_PTR 0x03001f4c
#else
#define SCENE_WORK_PTR 0x03001f3c
#endif
#if defined(TBS_EDITION_FR)
#define SCENE_CALLBACK 0x0200cbcc
#else
#define SCENE_CALLBACK 0x0200cb9c
#endif

extern s32 Data_02000240[];
extern s16 SceneStateHalfwords[];
extern u8 SceneStateBytes[];

u8 *Func_020072b8();           /* veneer to Scene_GetRecord */

void Func_0200723a();          /* veneer to Object_SetPosition */

s32 Func_02005772();           /* local thunk to Func_020020e8, site A */

s32 Func_02005784();           /* local thunk to Func_020020e8, site B */

void Func_020072aa();          /* veneer to UiText_DrawQuantity, site A */

void Func_020072bc();          /* veneer to UiText_DrawQuantity, site B */

void Func_020072cc();          /* shared veneer, selector refresh + 0x96a */

void Func_0200729a();          /* veneer to Func_08009148 */

SceneRecord *Func_02007afc();   /* scene record for a subject handle */

s32 Func_02007b0e_a(SceneRecord *, Position3 *);  /* terrain probe */

void Func_02007b12(SceneRecord *, s32, s32, s32);   /* place at (x, y, z) */

void Func_02007b22(SceneRecord *, s32, s32, s32);   /* place at (x, y, z) */

void Func_02007b36(SceneRecord *);              /* re-attach the camera */

/* Return this overlay's state block. */

/*
 * Table getter for resource_3bb, published from the overlay's header.
 *
 * The eight-byte owner at 0x02000030 includes its one pool word at
 * 0x02000034; the load reads that word and returns it as an address,
 * without dereferencing it.
 */
u8 *SceneData_GetTableC414(void)
{
    return (u8 *)0x0200c414;
}

s32 Func_02000038(void)
{
    return 0;
}

/*
 * Overlay resource_3bb. Exported getter for an in-image table, published
 * from the overlay header.
 */

/*
 * The eight-byte owner at 0x0200003c includes its one pool word, which holds
 * the returned address; the table is returned without being dereferenced.
 */
u8 *SceneData_GetTableC474(void)
{
    return (u8 *)0x0200c474;
}

/*
 * Overlay resource_3bb. Exported getter for an in-image table, published
 * from the overlay header.
 */

/*
 * The eight-byte owner at 0x02000044 includes its one pool word, which holds
 * the returned address; the table is returned without being dereferenced.
 */
u8 *SceneData_GetTableC48c(void)
{
    return (u8 *)0x0200c48c;
}

/*
 * resource_3bb owner at 0x0200004c: two bytes, `bx lr', with no prologue and
 * no pool.  Two data-table slots install it as a handler, so the empty body
 * is deliberate rather than padding.  Whether those slots expect a void
 * handler or an identity is not settled here -- `bx lr' leaves r0 untouched,
 * so a caller reading a result gets back whatever it passed in.
 */
void Resource3bb_NoOpHandler(void)
{
}

void FieldScene_RunSingleStep(void)
{
    StagedActor_PushActorAhead();
}

/* Rect setup for resource_3bb.  Each call site is spelled with its own import
 * name even though several of them reach the same import. */

/* Contiguous unnamed leaf-owner run for resource_3bb. */

/*
 * Scene setup for resource_3bb: allocates a descriptor, stamps its parameter
 * block, binds the in-image data to a graphics handle, and installs the
 * per-frame task.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */

/* This overlay's own routine at file offset 0x3638, installed as a task. */

/* In-image descriptor at file offset 0x4194. */
void SceneState_ConfigureRegionByActorElevenColumn(void)
{
    u8 *work;
    s32 v0;
    s32 v1;

    work = Func_02003fb4(11);
    if ((*(s32 *)(work + 8) >> 20) == 36) {
        GameFlag_Set(0x335);
        v0 = 0x23;
        v1 = 0x4D;
        Map_CopyCellAttributes(0x23, 0x4E, 1, 1, v0, v1);
    } else {
        GameFlag_Clear(0x335);
        v0 = 0x23;
        v1 = 0x4D;
        Map_CopyCellAttributes(0x22, 0x4D, 1, 1, v0, v1);
    }
}

void FieldScene_RunTwoStepSequence(void)
{
    StagedActor_PushActorAhead();
    SceneState_ConfigureRegionByActorElevenColumn();
}

/* Per-site veneers: both reach the same main-image import, but each names its
 * own loader-relocated call word rather than a runtime address. */

/* In-image status words. The 64-byte owner includes its alignment bytes and
 * the two-word literal pool that holds these two addresses. */

/* Address-taken rather than called. Overlays are LINKED at 0x02008000 and run
 * at 0x02000000, so a pooled function address carries the link-base form: the
 * reference word is 0x02008715, which is this symbol plus the Thumb bit that
 * `.thumb_set` supplies. A called symbol uses the raw run-time annotation; an
 * address-taken one does not. */
void SceneState_StoreSlotTileXToWork832To848(void)
{
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 14;
    s32 sixth = 11;
    s32 *record;
    s32 value;

    Map_CopyCellAttributes(100, 11, 12, 4, fifth, sixth);

    record = Func_0200402c(12);
    value = record[2] >> 20;
    Func_02003ff2(832, value);
    Map_CopyCellAttributes(71, 16, 1, 1, value, 16);

    record = Func_02004052(13);
    value = record[2] >> 20;
    Func_02004018(840, value);
    Map_CopyCellAttributes(71, 16, 1, 1, value, 16);

    record = Func_02004076(14);
    value = record[2] >> 20;
    Func_0200403c(848, value);
    Map_CopyCellAttributes(71, 16, 1, 1, value, 16);
}

void SceneState_SetWorkByte35(void)
{
    u8 *record = *(u8 **)0x03001F30;

    record[53] = 1;
}

void SceneState_SetFlag331AndConfigureRegion46_17(void)
{
    u8 *p;

    GameFlag_Set(0x331);
    p = Func_020041b6(20) + 85;
    *p = 0;
    {
        s32 p5 = 44;
        s32 p6 = 17;

        Map_CopyCellAttributes(46, 17, 1, 1, p5, p6);
    }
}

void FieldScene_SetFlag332AndDrawTiles(void)
{
    u8 *slot;

    GameFlag_Set(0x332);
    slot = Func_020041ea(21) + 85;
    *slot = 0;
    {
        s32 v5 = 50;
        s32 v6 = 17;

        Map_CopyCellAttributes(46, 17, 1, 1, v5, v6);
    }
}

void FieldScene_SetFlag333AndDrawTiles(void)
{
    GameFlag_Set(0x333);
    {
        s32 width = 32;
        s32 height = 77;

        Map_CopyCellAttributes(32, 37, 1, 4, width, height);
    }
}

void SceneState_SendWord250With6(void)
{
    s16 *tbl = SceneStateHalfwords;

    Func_020042aa(*(s32 *)(tbl + 250), 6, 0);
}

void FieldScene_Forward4358(void)
{
    Leader_CheckAhead();
}

void SceneActor_MovePairByTileOffset(s32 a0, s32 a1, s32 a2)
{

    Obj_02000310 *p;
    Obj_02000310 *q;
    s32 x;
    s32 y;

    p = Func_02004282(gGameState.selected_actor);
    q = Func_0200428a(a0);
    Event_Begin();
    {
        x = ((p->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        Func_020041f2(p, x, p->f0c, y);
    }
    Object_SetAnimation(p, 27);
    {
        x = ((q->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((q->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        q->f30 = 0x10000;
        q->f34 = 0x8000;
        Func_0200421e(q, x, q->f0c, y);
    }
    if (a1 < 0 || a2 < 0) {
        Object_SetAnimation(q, 4);
    } else {
        Object_SetAnimation(q, 3);
    }
    Audio_PlayCue(226);
    Func_02004250(p);
    Audio_PlayCue(288);
    Object_SetAnimation(q, 2);
    Event_End();
}

/*
 * One fixed line, then three whose fifth or sixth argument is a field of the
 * record fetched for participants 15, 16 and 17.  Records 15 and 16 contribute
 * their word at +8, record 17 its word at +16, which moves to the sixth
 * argument slot while a literal 18 takes the fifth.  The shift is arithmetic,
 * so the fields are signed fixed-point with 20 fractional bits.  Only those two
 * fields are asserted; what the six arguments mean is not established here.
 */
void SceneState_ApplyRectsForActors15To17(void)
{
    s32 field;

    Map_CopyCellAttributes(100, 11, 12, 4, 14, 11);

    field = Func_02004598(15)[2] >> 20;
    Map_CopyCellAttributes(13, 28, 1, 4, field, 11);

    field = Func_020045b2(16)[2] >> 20;
    Map_CopyCellAttributes(13, 28, 1, 4, field, 11);

    field = Func_020045cc(17)[4] >> 20;
    Map_CopyCellAttributes(13, 28, 4, 1, 18, field);
}

void FieldScene_RunStep15At29By26(void)
{
    Func_02000b66(15, 29, 26);
    SceneState_ApplyRectsForActors15To17();
}

void FieldScene_RunStep15At33By26(void)
{
    Func_02000b7a(15, 33, 26);
    SceneState_ApplyRectsForActors15To17();
}

void FieldScene_RunStep16At45By26(void)
{
    Func_02000b8e(16, 45, 26);
    SceneState_ApplyRectsForActors15To17();
}

void FieldScene_RunStep16At49By26(void)
{
    Func_02000ba2(16, 49, 26);
    SceneState_ApplyRectsForActors15To17();
}

void FieldScene_RunStep17At40By23(void)
{
    Func_02000bb6(17, 40, 23);
    SceneState_ApplyRectsForActors15To17();
}

void FieldScene_RunStep17At40By25(void)
{
    Func_02000bca(17, 40, 25);
    SceneState_ApplyRectsForActors15To17();
}

void FieldScene_RunSupplementalSequenceOne(void)
{

    extern s32 Data_0200c834;
    extern u32 Data_0200c838;

    s32 i;
    s32 rec8;
    s32 rec7;
    s32 xa;
    s32 xb;
    s32 xd;
    s32 ya;
    s32 yb;
    s32 yd;
    s32 record;

    rec8 = Func_02004674(gGameState.selected_actor);
    for (i = 22; i <= 25; i++) {
        rec7 = Value1(Func_0200467e, i);
        *(u8 *)(rec7 + 91) = 0;
        xa = *(s32 *)(rec7 + 8);
        xb = *(s32 *)(rec8 + 8);
        xd = xa - xb;
        if (xd >= 0) {
            if (xd > 0x9ffff) {
                continue;
            }
        } else {
            xb = xb - xa;
            if (xb > 0x9ffff) {
                continue;
            }
        }
        ya = *(s32 *)(rec7 + 16);
        yb = *(s32 *)(rec8 + 16);
        yd = ya - yb;
        if (yd >= 0) {
            if (yd > 0x9ffff) {
                continue;
            }
        } else {
            yb = yb - ya;
            if (yb > 0x9ffff) {
                continue;
            }
        }
        if (GameFlag_IsSet(0x104) != 0) {
            yb = *(s32 *)(rec7 + 16);
        } else {
            yb = *(s32 *)(rec8 + 16);
            yd = *(s32 *)(rec7 + 44);
            yb = yb + yd;
        }
        *(s32 *)(rec8 + 16) = yb;
    }
    if (Data_0200c838 != 0
        && *(s32 *)(rec7 + 56) == (s32)0x80000000) {
        if (Data_0200c834 == 0) {
            Map_CopyCellAttributes(58, 28, 7, 1, 58, 13);
        } else {
            Map_CopyCellAttributes(58, 10, 1, 1, 58, 11);
        }
    } else {
        Map_CopyCellAttributes(57, 11, 1, 1, 58, 11);
        Map_CopyCellAttributes(58, 14, 7, 1, 58, 13);
    }
    if (Data_0200c838 == 0) {
        Data_0200c834 ^= 1;
        if (Data_0200c834 != 0) {
            record = Func_0200475c(22);
            Call4(Func_02004692, record, 0x3a80000, 0, 0xb80000);
            record = Func_02004770(23);
            Call4(Func_020046a6, record, 0x3c80000, 0, 0xf80000);
            record = Func_02004784(24);
            Call4(Func_020046ba, record, 0x3e80000, 0, 0xb80000);
            record = Func_02004798(25);
            Call4(Func_020046ce, record, 0x4080000, 0, 0xf80000);
            Actor_SetAnimation(31, 11);
        } else {
            record = Func_020047b6(22);
            Call4(Func_020046ec, record, 0x3a80000, 0, 0xd80000);
            record = Func_020047ca(23);
            Call4(Func_02004700_a, record, 0x3c80000, 0, 0xd80000);
            record = Func_020047de(24);
            Call4(Func_02004714, record, 0x3e80000, 0, 0xd80000);
            record = Func_020047f2(25);
            Call4(Func_02004728, record, 0x4080000, 0, 0xd80000);
            Actor_SetAnimation(31, 10);
        }
    }
    Data_0200c838++;
    if (Data_0200c838 > 119) {
        if (GameFlag_IsSet(0x104) == 0) {
            Data_0200c838 = 0;
        }
    }
}

void FieldScene_RunScene3bbSequenceA(void)
{
    extern s32 Data_0200c834;
    extern u32 Data_0200c838;

    Data_0200c838 = 0;
    Data_0200c834 = 0;
    Call1(Func_020046ac, 0x2008715);
    Actor_SetPosition(22, 0x3a80000, 0xd80000);
    Actor_SetPosition(23, 0x3c80000, 0xd80000);
    Actor_SetPosition(24, 0x3e80000, 0xd80000);
    Actor_SetPosition(25, 0x4080000, 0xd80000);
    Actor_SetAnimation(31, 10);
}

void SceneState_ApplyTable8715AndValue104(void)
{
    Func_02004700(0x2008715, 0xC85);
    GameFlag_Clear(0x104);
}

/*
 * Spin until the first status word reaches zero with the second equal to 75,
 * giving up after 600 polls. Both words are re-read on every pass, because the
 * poll call lets the task that publishes them run.
 * The plain while loop is the spelling that reproduces the reference. What the
 * two words mean is not established here -- only that another task publishes
 * them while this owner spins.
 */
void SceneState_WaitForStatusWords(void)
{
    extern s32 Data_0200c834;
    extern u32 Data_0200c838;

    s32 cnt;

    /* The frame count is a literal ten. */
    Task_Wait(10);

    cnt = 0;
    while (Data_0200c834 != 0 || Data_0200c838 != 75) {
        Task_Wait(1);
        cnt++;
        if (cnt >= 600) {
            return;
        }
    }
}

void SceneState_InstallTask8714AndApplyTwoRects(void)
{
    extern s32 Data_0200c834;
    extern u32 Data_0200c838;

    Func_02004a58(31);
    GameFlag_Set(820);                 /* 205 << 2 */

    if (Data_0200c834 != 0) {
        Data_0200c838 = 0;
    }

    Task_Wait(30);
    Task_Wait(1);

    /* The task is published as its entry address with the Thumb bit set. The
     * `.thumb_set` alias the exact reconstruction emits for a Thumb symbol already carries
     * bit 0, so adding it again here overshoots by one. */
    Func_02004790((s32)Func_02008714);

    Map_CopyCellAttributes(58, 28, 7, 1, 58, 13);
    Map_CopyCellAttributes(57, 11, 1, 1, 58, 11);
}

u8 *SceneData_GetTableC83c(void) { return (u8 *)0x0200c83c; }

void FieldScene_RunScene3bb_02000b38(s32 a0)
{

    u32 i;
    s32 record;

    Actor_Destroy(40);
    Actor_Destroy(41);
    Func_02004a74(1);
    Event_Begin();
    Actor_SetPosition(8, 0x580000, 0x1000000);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x780000, 0x1000000);
    Actor_FaceActor(8, 0x4000, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 0x4000, 0);
    if (a0 < 0) {
        Actor_SetAnimation(8, 10);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 35);
    } else {
        Actor_SetAnimation(8, 8);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 28);
    }
    Task_Wait(1);
    Camera_MoveTo(0x680000, 0, 0xc00000, 0);
    FieldScene_RunLateSequence(a0);
    Event_End();
}

void FieldScene_RunSecondActorInteraction(s32 a0)
{

    extern s16 Data_02000240_t[][1];

    u32 i;
    s32 rec;
    s32 record;

    if (Data_02000240_t[225][0] == 2) {
        Func_0200322e();
    } else {
        Event_Begin();
        rec = Value2(Func_02003444, a0, 2);
        if (rec == 0) {
            Event_SetMessage(MSG_SHIFTING_FLOOR_STAGE);
            FieldScene_RunScene3bbSequenceA();
            Camera_SetSpeed(0x30000, 0x6000);
            Camera_MoveTo(0x3d80000, -1, 0xe80000, 1);
            Camera_WaitForMove();
            Value2(Engine_EventShowMessage, a0, 0);
            SceneState_ApplyTable8715AndValue104();
            Event_ShowMessage(a0, 0);
            Func_02004046(0, 0x438, 0x108);
            Event_Wait(15);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
            Value3(SceneActor_PlaceWithScale14000, 0, 0x438, 216);
            Value3(SceneActor_PlaceWithScale14000, 0, 0x428, 216);
            SceneState_WaitForStatusWords();
            Leader_CheckAhead();
            Camera_MoveTo(-1, -1, -1, 0);
            Event_ShowMessage(a0, 0);
            Func_02004274(0);
            Camera_FollowActor(ACTOR_PARTY_LEADER, 0);
            Func_020035b8(a0, 2);
        } else {
            if (rec == 1) {
                Event_SetMessage(MSG_OPERATOR_LIFTS_WILL_CHEER_FOR);
                Event_ShowMessage(a0, 0);
            }
        }
        Value3(FieldScene_RunMiddleSequence, rec, a0, 2);
        ((void (*)())Engine_EventEnd)();
    }
}

void FieldScene_RunSceneThreeCoordinator(s32 a0)
{

    extern s16 Data_02000240_t[][1];
    u32 i;
    s32 rec2;
    s32 record;

    if (Data_02000240_t[225][0] == 2) {
        Func_02003346();
    } else {
        Event_Begin();
        rec2 = Value2_02001538(Func_0200355c, a0, 3);
        if (rec2 != 0) {
        } else {
            Event_SetMessage(MSG_LOG_ROLLING_STAGE);
            Camera_SetSpeed(0x30000, 0x6000);
            Camera_MoveTo(0x2f00000, -1, 0xc00000, 1);
            Camera_WaitForMove();
            Event_Wait(60);
            Camera_SetSpeed(0x10000, 0x2000);
            Camera_MoveTo(0x2f00000, -1, 0xe00000, 1);
            Camera_WaitForMove();
            Event_ShowMessage(a0, 0);
            Func_0200417a(0, 0x358, 0x108);
            Event_Wait(10);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
            Value3_02001538(SceneActor_PlaceWithScale14000, 0, 0x358, 0x108);
            Value3_02001538(SceneActor_PlaceWithScale14000, 0, 0x358, 232);
            Event_ShowMessage(a0, 0);
            Call3(SceneActor_PlaceWithScale14000, 0, 0x348, 232);
            Event_Wait(10);
            Value3_02001538(SceneActor_MovePairByTileOffset, 33, -64, 0);
            Camera_MoveTo(0x2f00000, -1, 0xd80000, 1);
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
            Event_Wait(10);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x2f8, 232);
            Event_Wait(10);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 30);
            Event_ShowMessage(a0, 0);
            Func_020043fe(0);
            Camera_FollowActor(ACTOR_PARTY_LEADER, 0);
            Actor_SetPosition(33, 0x3480000, 0xe80000);
            Func_02003750(a0, 3);
            goto L_020016b0;
        }
        if (rec2 == 1) {
            Event_SetMessage(MSG_HERE_YOUR_OBJECTIVE_RIDE_LOGS);
            Event_ShowMessage(a0, 0);
        }
        L_020016b0:;
        Value3_02001538(FieldScene_RunMiddleSequence, rec2, a0, 3);
        Event_End();
    }
}

void Func_02001b30(void)
{
    {
        s32 x = Func_02005a38(896);
        s32 y = Func_02005a42(904);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Actor_SetPosition(ACTOR_GERALD, x, y);
    }
    {
        s32 x = Func_02005a60(912);
        s32 y = Func_02005a6a(920);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Actor_SetPosition(ACTOR_IVAN, x, y);
    }
    {
        s32 x = Func_02005a84(928);
        s32 y = Func_02005a8e(936);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Actor_SetPosition(ACTOR_MIA, x, y);
    }
}

void Func_02001c78(void)
{
    extern u8 *Data_03001ebc;

    u8 *state = Data_03001ebc;
    s32 v = gGameState.selected_actor;

    if (v != 0 && ((s32)(s16)*(u16 *)(state + 382) >> 10) == v
        && GameFlag_IsSet(321) != 0) {
        u16 *p = (u16 *)(state + 386);
        s32 t = 99;

        *p = t;
    }
}

void Func_02001cc0(void)
{
    extern u8 *Data_03001ebc;

    u8 *state = Data_03001ebc;
    s32 best = 8;
    s32 bestd = 0x100000;
    s32 n = gGameState.selected_actor;
    Obj *p = Func_02005c38(n);
    s32 i;
    s32 *q;
    s32 base;

    Event_Begin();
    for (i = 8; i <= 66; i++) {
        Obj *o = Func_02005c46(i);

        if (o != 0 && o->f54 == 1 && *o->f50->f28 == 165) {
            s32 dx = (p->f08 - o->f08) / 65536;
            s32 dy = (p->f10 - o->f10) / 65536;

            if (dy <= 0) {
                s32 a = dx;
                s32 d;

                if (a < 0) a = -a;
                if (dy < 0) dy = -dy;
                d = a + dy;
                if (d < bestd) {
                    best = i;
                    bestd = d;
                }
            }
        }
    }
    Event_SetMessage(MSG_MATCH_ABOUT_BEGIN_PLEASE_TAKE);
    Event_ShowMessage(best, 0);
    q = (s32 *)(state + 448);
    *q = 512;
    *(s32 *)(state + 456) = 15;
    Event_Wait(20);
    Event_CloseScreen();
    Event_WaitForScreen();
    base = n << 4;
    Func_02005ca0(base + 880, p->f08 >> 20);
    {
        s32 v = p->f10 >> 20;

        Func_02005cb0_a(base + 888, v);
    }
    n++;
    if (n > 3) {
        Event_RequestExit(10);
        GameFlag_Set(282);
    } else {
        Func_0200396a(n);
        Event_OpenScreen();
        Event_WaitForScreen();
        *q = 0;
    }
    Event_End();
}

void RunPartyCountInteractionCopyA(s32 actorId)
{
    PartyInteractionRecord *record;
    s32 x;
    s32 y;

    record = GetPartyInteractionRecord();
    x = record->x;
    y = record->y;
    Event_Begin();

    if (GetPartyMemberCount() <= 1) {
        Event_SetMessage(MSG_ROBIN_DID_GET_GOOD_LOOK);
        if (Event_AskYesNo(actorId, 0) == 0) {
            InitializeActorZero();
            InitializeSelectedActor(actorId);
            Actor_WalkTo(actorId, x, y + 0x40);
            Event_Wait(15);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, x, y);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, x, y + 0x20);
            Event_CloseScreen();
            Event_WaitForScreen();
            Event_RequestExit(11);
        }
    } else {
        Event_SetMessage(MSG_WAIT_SHOULDNT_DECIDE_WHERE_BEST);
        Event_ShowMessage(actorId, 0);
    }

    Event_End();
}

/*
 * resource_3bb: six steps, each calling its own near-identical handler stub
 * with a fixed id -- 896 through 936, eight apart -- and a second argument of
 * zero.  The owner at 0x02001fb8 is 66 bytes: its code plus one trailing
 * alignment halfword.  It reads no incoming argument, so it takes none.  The
 * six callees are identified by call shape only.
 */
void FieldScene_RunSixSteps896To936(void)
{
    Func_02005eca(896, 0);
    Func_02005ed4(904, 0);
    Func_02005ede(912, 0);
    Func_02005ee8(920, 0);
    Func_02005ef2(928, 0);
    Func_02005efc(936, 0);
}

s32 Func_02001ffc(s32 a, s32 b)
{

    s32 v;
    s32 id;
    s32 r;

    Func_02006084();
    Func_02005ebc(b, 5);
    v = SceneStateHalfwords[224];
    if (v == (s32)&Value_0000008f) {
        id = (s32)&Value_00002076;
    } else if (v == (s32)&Value_00000090) {
        id = (s32)&Value_00002078;
    } else {
        id = (s32)&Value_0000207a;
    }
    Event_SetMessage(id);
    Event_ShowMessage(a, 0);
    if (GameFlag_IsSet(b + 512) != 0) {
        return 2;
    }
    if (GameFlag_IsSet(b + 520) != 0) {
        r = Func_02005f1e(0);
        if (r == 1) {
            return 2;
        }
        if (r == 2 || r == -1) {
            return 3;
        }
        return r;
    }
    GameFlag_Set(b + 520);
    Event_SetMessage((s32)&LinkedMessage_WouldYouLikeHearDescription);
    Event_OpenMessage(a, 0);
    return Event_ChooseYesNo(0, 0);
}

void Func_020020b8(s32 a, s32 b)
{

    s32 v;
    s32 id;

    Func_02005f72(b, 5);
    v = SceneStateHalfwords[224];
    if (v == (s32)&Value_0000008f) {
        id = (s32)&Value_00002076;
    } else if (v == (s32)&Value_00000090) {
        id = (s32)&Value_00002078;
    } else {
        id = (s32)&Value_0000207a;
    }
    Event_SetMessage(id + 1);
    Event_ShowMessage(a, 0);
}

void FieldScene_RunMiddleSequence(s32 mode, s32 owner, s32 base)
{
    s32 rec;
    s32 record;
    s32 p9;
    s32 p11;
    s32 count;
    s32 state;
    s32 obj;
    s32 hi;
    s32 lo;
    s32 tail;
    s32 sx;
    s32 sy;
    s32 i;
    u8 buf[8];

    rec = Value1_02002114(Func_0200607e, owner);
    p9 = *(s16 *)(rec + 10);
    p11 = *(s16 *)(rec + 18);
    if (mode != 3) {
        count = Value0(Func_02006054);
        for (i = 0; i < count; i++) {
            buf[i] = SceneStateBytes[504 + i];
        }
        if (count <= 1) {
            Event_SetMessage(MSG_DO_YOUR_BEST);
            Event_ShowMessage(owner, 0);
            return;
        }
        if (GameFlag_IsSet(base + 512) != 0) {
            Event_SetMessage(MSG_UNFORTUNATELY_WE_HAVE_FULL_HOUSE);
            Event_ShowMessage(owner, 0);
            return;
        }
        if (mode == 2) {
            state = 0;
            Task_Wait(6);
        } else {
            Event_SetMessage(MSG_WOULD_LIKE_FRIEND_CHEER_FOR);
            Event_OpenMessage(owner, 0);
            state = Event_ChooseYesNo(0, 0);
        }
        if (state == 0) {
            if (state < count) {
                for (i = 0; i < count; i++) {
                    Func_020060e0((s32)(s8)buf[i]);
                }
            }
            for (i = 0; i < count; i++) {
                if ((s32)(s8)buf[i] != 0) {
                    Func_020060f4((s32)(s8)buf[i]);
                }
            }
            obj = Value0(Func_0200628e);
            for (i = 0; i < count; i++) {
                Func_0200611e((s32)(s8)buf[i]);
            }
            for (i = 0; i < count; i++) {
                Func_02006130((s32)(s8)buf[i]);
            }
            if (obj != -1) {
                goto L_main;
            }
        }
    }
    Event_SetMessage(MSG_IF_KNOW_WHO_WANT_CHEER);
    Event_ShowMessage(owner, 0);
    return;
L_main:
    ((void (*)())Func_020060ec_a)(obj, 1);
    Event_SetMessage(MSG_ROBIN_WILL_CHEER_FOR_WAY);
    Event_ShowMessage(owner, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_SetSpeed(obj, 0x10000, 0x8000);
    Actor_SetSpeed(owner, 0x10000, 0x8000);
    record = Value1_02002114(Func_020061ca, 0);
    if (record != 0) {
        Actor_SetPosition(obj, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    hi = p11 + 16;
    Actor_WalkToAndWait(obj, p9, hi);
    lo = p9 + 16;
    Value3_02002114(Engine_ActorWalkToAndWait, 0, lo, hi);
    Actor_FaceEachOther(obj, ACTOR_PARTY_LEADER, 30);
    Actor_SetAnimation(obj, 3);
    tail = hi - 32;
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_WalkToAndWait(owner, p9, tail);
    Value3_02002114(Engine_ActorWalkTo, owner, lo, tail);
    Func_02006374(0, obj);
    Actor_WalkToAndWait(obj, p9, tail);
    Actor_SetAnimation(owner, 1);
    Actor_FaceDirection(owner, 0x8000, 0);
    Actor_WalkToAndWait(obj, p9, p11 - 48);
    Actor_WalkToAndWait(owner, p9, tail);
    Actor_WalkToAndWait(owner, p9, p11);
    Func_02006240(obj);
    GameFlag_Set(base + 512);
    rec = Value1_02002114(Func_02006282, obj);
    sx = *(s32 *)(rec + 8) >> 20;
    Func_0200624e_a((obj << 4) + 880, sx);
    sy = *(s32 *)(rec + 16) >> 20;
    Func_0200625c((obj << 4) + 888, sy);
}

/*
 * resource_3bb: look up an object by arg0, then scan the first 15 halfword
 * entries of its table at offset 0xd8 for one equal to arg1, calling a handler
 * with each matching index.  The owner at 0x02002380 is 60 bytes and carries
 * no literal pool.  The three callees are identified by call shape only, and
 * the table's role is inferred from this scan alone.
 */
void OverlayObject_NotifyMatchingEntries(s32 no, s32 val)
{
    u16 *tbl = Func_0200625c_a(no);
    s32 i;

    Func_0200626e_a(no, val);

    tbl = (u16 *)((char *)tbl + 0xd8);
    for (i = 0; i <= 14; i++) {
        if (tbl[i] == val) {
            Func_0200628a(no, i);
        }
    }
}

/*
 * 0x0200a451 is the installed callback, a routine in this overlay.  The branch
 * chain picks one of five addresses by mode, consulting param only when mode is
 * 3; whether those five are code or per-mode records is not resolved, so they
 * are typed as plain addresses.  The four stores that follow reset the rest of
 * the task's state block, three as halfwords and one as a word.
 */
void Func_02002940(u32 mode, u32 param)
{
    s32 handler;

    Data_0200cc30 = (u16)mode;
    Data_0200cc04 = (u16)(param << 4);

    {
        s32 budget = 0xc80;
        s32 task = 0x0200a451;
        Func_02006700(task, budget);
    }

    handler = (s32)&Data_0200ca1e;
    if (mode == 2) {
        handler = (s32)&Data_0200c0ce;
    }
    if (mode == 4) {
        handler = (s32)&Data_0200ca4a;
    }
    if (mode == 3) {
        if (param != 0) {
            handler = (s32)&Data_0200c0f6;
        } else {
            handler = (s32)&Data_0200cac8;
        }
    }

    Data_0200cc3c = 0;
    Data_0200cc40 = handler;
    Data_0200cc98 = 0;
    Data_0200cc0c = 0;
    Data_0200cc10 = 0;
}

void Func_020029d0(s32 mode)
{
    if (mode == 0) {
        Event_Begin();
        Event_OpenScreen();
        Event_WaitForScreen();
        Event_Wait(30);
        Audio_PlayCue(0x59);
        Func_02004db0(0);
        Func_0200533c(1, 0);
        Event_Wait(120);
        Event_End();
        return;
    }

    Audio_PlayCue(0xf7);
    Event_Begin();
    Event_OpenScreen();
    Event_WaitForScreen();
    {
        s16 *base = (s16 *)0x0200ca1e;
        *(s16 *)((u8 *)base + 30) = (s16)(mode * 60);
    }
    Event_Wait(30);
    Audio_PlayCue(mode + 0x5a);
    Func_02004df4(mode);
    Func_02005380(1, 0);
    Event_Wait(120);

    goto check_transition;
wait_transition:
    Task_Wait(1);
check_transition:
    if (Func_02006b0a() != 0)
        goto wait_transition;

    Audio_PlayCue(0x121);
    Func_02004e1e(5);
    Func_020053aa(2, 0);
    Audio_PlayCue(0xec);
    Event_Wait(60);
    Func_020053be(2, 1);
    Audio_PlayCue(0xec);
    Event_Wait(60);
    Func_02004e4c(6);
    Func_020053d8(2, 0);
    Audio_PlayCue(0xec);
    Event_Wait(60);
    Func_02004e66(7);
    Func_020053f2(4, 0);
    Audio_PlayCue(0xed);
    Func_02006b44();
    Event_End();
    GameFlag_Set(0x123);
}

void FieldScene_RunLateSequence(s32 a0)
{

    extern u8 Data_02000240_t[][2];
    void Event_OpenScreen();

    s32 kind;

    Audio_PlayCue(247);
    Event_OpenScreen();
    Event_WaitForScreen();
    Data_0200cac8.span = a0 * 60;
    Data_0200c0f6.span = (a0 < 0 ? -a0 : a0) * 60;
    if (a0 < 0) {
        Event_Wait(30);
        Audio_PlayCue(86);
        Func_02004eda(8);
        Value2_02002adc(Func_02005466, 3, 1);
        Event_Wait(-a0 * 60 + 60);
        kind = 0;
    } else {
        Event_Wait(30);
        Audio_PlayCue(a0 + 90);
        Func_02004f06(4);
        Value2_02002adc(Func_02005492, 3, 0);
        Event_Wait(a0 * 60 + 60);
        kind = 8;
    }
    Actor_ShowEmote(kind, 0x105, 0);
    while (Value0(Func_02006c28)!= 0) {
        Task_Wait(1);
    }
    Audio_PlayCue(19);
    Event_Wait(30);
    Audio_PlayCue(0x121);
    Event_CloseScreen();
    Event_WaitForScreen();
}

/* Reset the selected actor's motion fields after refreshing it. */
void StagedActor_ResetMotionAfterRefresh(s32 slot)
{
    u8 *actor = Func_02006d98(slot);
    Func_02006cb6(actor);
    *(s32 *)(actor + 36) = 0;
    *(s32 *)(actor + 44) = 0;
    *(s32 *)(actor + 56) = (s32)0x80000000;
    *(s32 *)(actor + 64) = (s32)0x80000000;
}

void SceneState_InitCursorWhenUnset(void)
{
    if (Data_0200cb46 == -1) {
        Data_0200cb46 = Func_02006d3e();
    }
}

void SceneEffect_SeedMarkerAndInstallTask(u32 x, u32 y, u32 style)
{
    Func_02005e32(x, y, style);

    Data_0200cc94 = (u16)x;
    Data_0200cc20 = (u16)y;
    Data_0200cbf8 = (u16)(style & 3);
    Data_0200cc14 = 0;
    Data_0200cc2c = 0;

    {
        s32 budget = 0xc80;
        Func_02006d9c(0x0200ae85, budget);
    }
}

/* Start an interpolated marker move from the current position. */

/* Per-site veneer (raw sub_ symbol from the overlay .s). */
void SceneState_StartMarkerMove(u32 x, u32 y, u32 duration)
{
    Data_0200cc00 = (u16)x;
    Data_0200cca0 = (u16)y;
    Data_0200cc44 = Data_0200cc94;
    Data_0200cc5c = Data_0200cc20;
    Data_0200cc2c = (u16)duration;
    Data_0200cbf0 = 0;

    {
        s32 frame_budget = 0xc80;
        Func_02006df8(0x0200ae85, frame_budget);
    }
}

/* Remove the marker task and release its selected display slot.
 * Per-site veneers (raw sub_ symbols from the overlay .s). */
void SceneEffect_RemoveMarkerTask(void)
{
    Func_02006e36((void *)0x0200ae85);
    Func_02006e90(Data_0200cb46);
    Data_0200cb46 = -1;
}

/* STILL-OPEN: adopt --where differing_bytes=11/64. alchemist.ts refused
 * (tiers: class, priority) -- matches the documented DEAD-END FINGERPRINT
 * (adjacent register swap in an asrs/str block), genuinely unfixed. */

/* Per-site veneers (raw sub_ symbols from the overlay .s). */

/*
 * One symbol per call site.  These names stand for loader-relocated call
 * words in the overlay's veneer table, not runtime addresses.
 */
void SceneActor_PlaceWithScale20000(s32 selector, s32 x, s32 z)
{
    s32 *actor = Func_02007128(selector);

    if (actor != 0) {
        s32 scale = 0x20000;
        u8 *p = (u8 *)actor;
        u8 zero = 0;
        *(s32 *)((u8 *)actor + 48) = scale;
        *(s32 *)((u8 *)actor + 52) = scale >> 1;
        p += 91;
        *p = zero;

        Func_02006f34(actor);
        Object_SetAnimation(actor, 5);
        Func_02006f58(actor, x << 16, actor[3], z << 16);
    }
}

/*
 * resource_3bb owner at 0x020030e8, 80 bytes plus one alignment halfword:
 * spawn an object and run a short fixed setup sequence on it.  `no' is
 * forwarded unchanged -- it is never freshly loaded before the first call.
 * The object's fields (0x30 and 0x34 taking the fixed 0x14000/0xa000 pair,
 * 0x5b cleared, 0xc read back for the position call) are named by position
 * from call shape alone and are not verified.
 */
void SceneActor_PlaceWithScale14000(s32 no, s32 x, s32 z)
{
    u8 *obj = Func_02007168(no);

    if (obj == 0) {
        return;
    }

    {
        s32 scale = 0x14000;
        u8 *p = obj;
        u8 zero = 0;
        *(s32 *)(obj + 0x30) = scale;
        *(s32 *)(obj + 0x34) = scale >> 1;
        p += 0x5b;
        *p = zero;
    }

    Func_02006f74();
    Object_SetAnimation(obj, 5);
    Func_02006f98(obj, x << 16, *(s32 *)(obj + 12), z << 16);
    Func_02006fa6(obj);
    Object_SetAnimation(obj, 1);
}

void Text_WriteU32AsHex(u8 *buf, u32 value)
{
    s32 i;

    buf += 8;
    *buf = 0;
    buf--;
    for (i = 7; i >= 0; i--) {
        *buf = HexDigits[value & 15];
        value >>= 4;
        buf--;
    }
}

/* Complete two-byte empty hook plus its alignment halfword. */
void Resource3bb_EmptyHook(void)
{
}

/* Complete eight-byte state setter plus its sole four-byte pool word. */
void SceneState_SetHalfword1000To9(void)
{
    Data_02001000.f0 = 9;
}

void SceneState_WaitUntilStatusNine(void)
{
    s16 *status = &Data_02001000.f0;

    while (*status != 9) {
        Task_Wait(1);
    }
}

void Func_0200325c(Obj_0200325c *a)
{
    s32 t[3];
    u32 n;

    if (a->f28 >= -255 && a->f28 <= 255) {
        a->f55 = 0;
    }
    n = Random_Next();
    if (n * 100 >> 16 <= 9) {
        Obj_0200325c *o;
        s32 u;
        s32 w;

        t[0] = a->f08;
        t[1] = a->f0c;
        t[2] = a->f10;
        u = Random_Next();
        w = Random_Next();
        Func_0200706e(u << 4, w, t);
        {
            s32 x = t[0];
            s32 y = t[1];
            s32 z = t[2];

            o = Func_0200710a(285, x, y, z);
        }
        if (o != 0) {
            o->f55 = 0;
            Actor_SetSpriteFlags(o, 0);
            Object_SetScript(o, (s32)Data_0200c264);
            Object_SetAnimation(o, 1);
            Object_SetAnimation(o, 0);
        }
    }
}

s32 Func_020032f0(Obj_020032f0 *a)
{
    Obj_020032f0 *o = Func_02007254(a->f64);

    Func_0200718e(o, a->f08, a->f0c + 0x240000, a->f10);
    o->f55 = 0;
    Object_SetScript(o, (s32)Data_0200c288);
    Audio_PlayCue(83);
    a->f64 = 0;
    return 0;
}

s32 Func_02003344(s32 handleA, s32 handleB)
{
    u8 *workspace = *(u8 **)SCENE_WORK_PTR;
    u8 *shared;
    u8 *record;
    s32 flag;
    s32 x;
    s32 z;
    u16 *cuep;
    s16 *waitp;

    flag = GameFlag_IsSet(0x211);

    shared = Data_02000240;
    record = Func_020072b8(*(s32 *)(shared + 500));

    if (*(s32 *)(workspace + 232) < *(s32 *)(record + 8)) {
        x = *(s32 *)(workspace + 232) + 0xc0000;
    } else {
        x = *(s32 *)(workspace + 232) - 0xc0000;
    }

    if (flag != 0) {
        z = *(s32 *)(workspace + 236) + 0x100000;
        cuep = (u16 *)(workspace + 228);
    } else {
        z = *(s32 *)(workspace + 236) - 0x100000;
        cuep = (u16 *)(workspace + 226);
    }

    waitp = (s16 *)(record + 100);
    *waitp = *cuep;
    *(s32 *)(record + 52) = 0x4000;
    *(s32 *)(record + 48) = 0x10000;

    Func_0200723a(record, x, 0, z);
    GameFlag_Set(0x211);
    Object_SetScript(record, (void *)SCENE_CALLBACK);

    while (*waitp != 0) {
        Task_Wait(1);
    }

    if (flag == 0) {
        Func_02005772(0, handleA);
        Func_020072aa(handleA, 2);
    } else {
        Func_02005784(0, handleB);
        Func_020072bc(handleB, 2);
    }

    shared = Data_02000240;
    Func_020072cc(*(s32 *)(shared + 500), 1);
    Message_ShowCentered(SCENE_TEXT_VALUE, 3);
    Func_0200729a(record);

    return flag;
}

/*
 * Seven arguments: four in registers, three from the caller's stack; five are
 * stored as halfwords and two as words, which is how they are typed here. The
 * 232-byte owner includes the seven-word literal pool the body branches over.
 * When the story flag is clear, `second` is mirrored about `centre`. The
 * extent stored at +216 is passed back sign-extended from sixteen bits.
 * Descriptor layout is asserted only for the fields written here.
 */
void FieldScene_BuildDescriptorAndInstallTask(s32 first, s32 second, s32 mode, s32 centre,
                   s32 extra, s32 third, s32 fourth)
{
    u8 *descriptor;
    u8 *first_record;
    u8 *second_record;
    s32 handle;
    s32 extent;

    descriptor = Func_020077f6(59, 0x7170);
    handle = Func_02007810(512);                /* 128 << 2 */

    *(u16 *)(descriptor + 222) = (u16)first;
    *(u16 *)(descriptor + 224) = (u16)second;
    *(u16 *)(descriptor + 226) = (u16)third;
    *(u16 *)(descriptor + 228) = (u16)fourth;
    *(u16 *)(descriptor + 230) = (u16)mode;
    *(s32 *)(descriptor + 232) = centre;
    *(s32 *)(descriptor + 236) = extra;

    first_record = Func_020079a8(first);
    second_record = Func_020079b0(second);

    if (GameFlag_IsSet(0x109) == 0) {
        *(s32 *)(second_record + 8) =
            (centre << 1) - *(s32 *)(first_record + 8);
        *(s32 *)(second_record + 16) = *(s32 *)(first_record + 16);
    }

    *(u16 *)(descriptor + 218) = 0;
    *(u16 *)(descriptor + 220) = 0;

    Func_02007886(Data_0200c194, handle);

    extent = Func_020078aa();
    *(u16 *)(descriptor + 216) = (u16)extent;
    Vram_Load((s16)extent, 512, handle);

    /* The task is published as its entry address plus the Thumb bit. */
    Func_02007856((s32)Func_0200b638 + 1, 0xc76);

    Func_020078a4(handle);
}

void SceneState_InitControlRecordAndStartTask(void)
{
    u8 *state = Data_03001f3c;
    Ctl *m = &Data_02001000;

    Func_020078ee(Func_0200791e(), (s32)(state + 240));
    if (GameFlag_IsSet(0x109) == 0) {
        m->f0 = 1;
        m->f2 = 1;
        m->f4 = *(u16 *)(state + 224);
        m->f8 = 0;
        m->f6 = 0;
    }
    {
        s32 e = 0xc85;

        Func_020078c2((s32)Data_0200b459, e);
    }
}

void SceneState_SetWorkHalfwordDc(s16 v)
{
    FIELD_AT_OFFSET(*(void **)0x03001F3C, s16 *, 0xDC) = v;
}

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay.
 * The occupancy lookups take the position block; the record pointer the call
 * sites also pass is not asserted as an argument. */
s32 *SceneData_FindSlotAtPosition(s32 *pos)
{

    extern u8 *Data_03001ebc;

    s32 **slots = (s32 **)(Data_03001ebc + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if ((pos[0] >> 20) == (p[2] >> 20)
            && (pos[1] >> 20) == (p[3] >> 20)
            && (pos[2] >> 20) == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

/*
 * The push interaction: probe the tile one step ahead of the subject and, if
 * something occupies it, slide it and the subject one step on. Only the
 * occupant's position is committed. The 360-byte owner includes its four-word
 * literal pool. Record fields are asserted only where written: facing at +6,
 * position at +8/+12/+16, state at +34, occupancy flag bit 0 at +89, speeds
 * at +48/+52, and the words cleared at +36/+44.
 */
void StagedActor_PushActorAhead(void)
{

    SceneRecord *subject;
    SceneRecord *target;
    SceneRecord *blocker;
    u32 step;
    u32 dir;
    Position3 pos;
    u32 idx = 250;
    s32 zero;
    s32 handle;

    handle = *(s32 *)((u8 *)SceneStateHalfwords + (idx << 1));
    subject = Func_02007afc(handle);

    dir = subject->facing >> 12;

    step = Data_0200c3d4[dir];
    pos.x = subject->x + (s32)(step & 0xffff0000);
    pos.y = subject->y;
    step <<= 16;
    pos.z = subject->z + (s32)step;

    target = Func_02007726(&pos, subject);
    if (target == 0) {
        return;
    }

    step = Data_0200c3d4[dir];
    pos.x = target->x + (s32)(step & 0xffff0000);
    pos.y = target->y;
    step <<= 16;
    pos.z = target->z + (s32)step;

    blocker = Func_02007750(&pos, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    pos.x = target->x;
    pos.y = target->y + 0x100000;
    pos.z = target->z;

    blocker = Func_0200777c(&pos, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    target->state = 2;
    zero = 0;

    step = Data_0200c3d4[dir];
    pos.x = target->x + (s32)(step & 0xffff0000);
    pos.y = target->y;
    step <<= 16;
    pos.z = target->z + (s32)step;

    /* Terrain probe: signed, so only a positive code refuses the move. */
    if (Func_02007b0e_a(target, &pos) > 0) {
        return;
    }

    Object_SetAnimation(subject, 8);
    Task_Wait(15);

    target->rate_x = 0x3333;
    target->rate_z = 0x3333;
    Func_02007b12(target, pos.x, pos.y, pos.z);

    /* The same destination block, moved onto the subject this time. */
    subject->rate_x = 0x3333;
    subject->rate_z = 0x3333;
    Func_02007b22(subject, pos.x, pos.y, pos.z);

    Audio_PlayCue(0xee);
    Func_02007b36(target);
    Audio_PlayCue(0x120);

    target->x = pos.x;
    target->z = pos.z;
    target->motion_24 = zero;
    target->motion_2c = zero;

    Object_SetAnimation(subject, 1);
}

/*
 * Probe the two cells ahead of the active subject and return what occupies the
 * nearer one, else the further one, else zero. The 160-byte owner includes its
 * alignment bytes and two-word literal pool. Facing is the biased quadrant of
 * the halfword at +6, with no sign extension; each probe rounds x and z down to
 * whole units and re-centres them by half a unit, carrying y unrounded. Only
 * the record fields at +6, +8, +12 and +16 are asserted.
 */
s32 *SceneActor_FindOccupantAheadOfSubject(void)
{

    u8 *record;
    s32 facing;
    s32 position[3];
    s32 *occupant;

    record = Func_02007c5c(((ActiveSubjectSlot *)SceneStateHalfwords)->handle);

    /* 128 << 6 = 0x2000 bias, then masked to bits 14-15 (192 << 8). */
    facing = (*(u16 *)(record + 6) + 0x2000) & 0xc000;

    position[0] = (*(s32 *)(record + 8) & 0xfff00000) + 0x80000;
    position[1] = *(s32 *)(record + 12);
    position[2] = (*(s32 *)(record + 16) & 0xfff00000) + 0x80000;
    Func_02007b0e(0x100000, facing, position);          /* 128 << 13 */

    occupant = Func_02007896(position, record);
    if (occupant == 0) {
        position[0] = (*(s32 *)(record + 8) & 0xfff00000) + 0x80000;
        position[1] = *(s32 *)(record + 12);
        position[2] = (*(s32 *)(record + 16) & 0xfff00000) + 0x80000;
        Func_02007b40(0x200000, facing, position);      /* 128 << 14 */

        occupant = Func_020078c8(position, record);
    }

    return occupant;
}
