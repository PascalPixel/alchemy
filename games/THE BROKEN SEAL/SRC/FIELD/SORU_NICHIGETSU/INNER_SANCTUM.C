#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

/*
 * The Luna and Sol rooms deep in Sol Sanctum. Sukureta wonders what the two
 * rooms mean and asks Robin to look farther up the passage. Lighting the four
 * statue lights turns the picture of Luna to Sol, and the trap that follows
 * shakes the statue hall.
 */

enum InnerSanctumActor {
    ACTOR_SUKURETA = ACTOR_FIRST_PLACED + 8
};

enum InnerSanctumMessage {
    MSG_WHAT_IS_THIS_ROOM = 0xff6,
    MSG_SUKURETA_THESE_ROOMS_HIDE_A_SECRET = 0xffa,
    MSG_SUKURETA_A_ROOM_FOR_LUNA_AND_SOL = 0xffb,
    MSG_SUKURETA_LET_ME_KNOW_WHAT_YOU_FIND = 0xffd,
    MSG_SUKURETA_JUST_WAIT_OVER_THERE = 0xfff,
    MSG_WHATS_HAPPENING = 0x1001,
    MSG_WHATS_HAPPENING_AT_THE_TRAP = 0x1018,
    MSG_YOU_FOUND_IT = 0x1025,
    MSG_THE_PICTURE_OF_LUNA_CHANGED_TO_SOL = 0x1027
};

enum InnerSanctumFlag {
    FLAG_INNER_SANCTUM_ENTERED = 0x80a,
    /* The four statue lights. */
    FLAG_STATUE_LIGHT_1 = 0x80b,
    FLAG_STATUE_LIGHT_2,
    FLAG_STATUE_LIGHT_3,
    FLAG_STATUE_LIGHT_4,
    FLAG_LUNA_PICTURE_CHANGED_TO_SOL = 0x810,
    FLAG_STATUE_TRAP_SPRUNG = 0x813,
    /* Robin agreed to look farther up the passage for Sukureta. */
    FLAG_ROBIN_SEARCHING_FOR_SUKURETA = 0x896
};

#if defined(TBS_EDITION_JA)
#define SCENE_STEP_VALUE Value_000011b4
#elif defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define SCENE_STEP_VALUE Value_0000100d
#else
#define SCENE_STEP_VALUE Value_0000102b
#endif

extern u8 Value_0000100d[];
extern u8 Value_0000102b[];
extern u8 Value_000011b4[];

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02004252_actor_step();
void Func_02004270();
void Func_02004606();
void Func_02004612();
void Func_02004626();
void Func_0200462c();
void Func_02004630();
void Func_02004636();
void Func_02004654();
void Func_0200465e();
void Func_02004662();
void Func_02004682();
void Func_0200468c();
void Func_0200469a();
void Func_020046a8();
void Func_020046aa();
void Func_020046e2();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

s32 CheckAllStatueLights(void);

void SetSolShindenActorStep(s32 actor_step, s32 wait_frames);

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/*
 * Withdrawn on 2026-09-10 as not reproducible, and reinstated the same night
 * once the cause was found and removed.
 *
 * The pair of equal 0x6480000 arguments in the last call below made this
 * owner's compile split: thirty raw cc1 runs on one fixed preprocessed input
 * produced 27 of one output and 3 of another, the minority recomputing the
 * shift instead of sharing it through mov r2, r1. Three sessions measured it
 * independently and agreed.
 *
 * The cause was not the source. GCC 2.96 hashes LABEL_REF by rtx address and
 * SYMBOL_REF by the address of its name, so macOS ASLR reordered equal-constant
 * discovery in CSE between runs. The toolchain now spawns compilers with
 * _POSIX_SPAWN_DISABLE_ASLR and the split collapses to a single output.
 *
 * Kept as a marker: raw runs spawned outside the toolchain still split 27/3,
 * so anyone measuring this owner by hand must disable ASLR or they will
 * rediscover the artefact rather than a property of this C.
 */

#include "TYPES.H"

/* Message ids handed to the Func_0808a170 service; see the header note. */
extern u8 Data_00000fe3[];
extern u8 Data_00000ff0[];
extern u8 Data_00000ff1[];
extern u8 Data_00000ff2[];

/*
 * resource_37a owner 0x02000488..0x020009f3 (0x56c = 1388 bytes).
 *
 * Reviewed scene script for the resource_37a sequence guarded by story flag
 * 0x809.  One straight command list: it opens the scene, places and drives
 * four actor slots (0, 1, 5 and 0x10) through a long beat-by-beat schedule of
 * moves and waits, plays two audio cues, branches once on a runtime query
 * near the end to choose between two message ids, then hands the three
 * principal slots back to their walking records, sets story flags 0x144 and
 * 0x809 and closes the scene.
 *
 * Complete owner.  Prologue `push {lr}` at 0x02000488; the single epilogue
 * `pop {r0} / bx r0` at 0x020009da pops the return address, so the owner is
 * void and takes no argument.  Code runs 0x02000488..0x020009d9 with two
 * interior literal pools (0x0200081c..0x0200084f and 0x020009de..0x020009f3);
 * the next prologue is at 0x020009f4.  All 140 `bl` sites are transcribed.
 *
 * Calls are spelled as the pre-relocation call word the image holds, the
 * convention the adopted sibling
 * games/THE BROKEN SEAL/src/overlays/scene_primary_script/run_scene_37a_sequence_a.c uses,
 * so one target reached from several sites appears under several spellings.
 *
 * RESIDUAL (all 32 remaining differing halfwords).  Eight of those spellings
 * collide: two sites whose different call words happen to decode to the same
 * address (Func_0200316c, Func_02003234, Func_0200326c, Func_02003330,
 * Engine_ActorSetDestination, Func_02003382, Func_020033b2 and Func_02003390).
 * Separating them needs an `absolute_symbols` entry in the translation-unit
 * manifest, which a single-file draft may not add, so all sixteen of those
 * sites are spelled with the main-image name of the service they reach.
 * Those calls are semantically right; they emit the direct long call instead
 * of this overlay's veneer word, and nothing else about them differs.
 *
 * The four ids handed to the Func_0808a170 message service are spelled as
 * `Data_00000<id>` externs, the convention the adopted overlay scene scripts
 * already use for this family (see
 * games/THE BROKEN SEAL/src/overlays/scene_primary_script/run_scene_372_sequence_a.c,
 * which declares `extern u8 Data_00000e67[]` and casts it to s32 at the call).
 * The reference forces that reading for 0xff0: every other pooled constant in
 * this owner (0x101, 0x105, 0x2005, 0x6666 ...) is one GCC cannot synthesize,
 * while 0xff0 is 0xff << 4 and from a plain integer literal would come out as
 * `movs r0,#255 / lsls r0,r0,#4`.  The reference loads it from the pool
 * instead, so the operand was address-shaped in the original source.  The
 * other three ids are byte-identical either way; they are spelled the same
 * way because they reach the same service.
 *
 * Uncertain: none of the main-image services is established beyond its
 * argument count and access widths, so all are declared old-style and their
 * constants are transcribed rather than interpreted.  The record returned by
 * the Func_0808a080 service is read at +8/+16 as words early in the scene and
 * at +10/+18 as signed halfwords at the end, so both stay raw offsets; no
 * shared type in games/THE BROKEN SEAL/include covers it.
 */

/* Each declaration's comment names the owner or main-image service the call
 * word reaches. */

/* The sixteen colliding sites reach these main-image services directly. */

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Ordinary C field access retained from the reviewed disassembly dataflow. */
#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))
s32  Func_02002e42();  /* Func_080770c0 */
s32  Func_02002ede();  /* Func_0808a080 */
s32  Func_020031f2();  /* Func_0808a070 */
s32  Func_02003262();  /* Func_0808a178 */
void Func_020004ea();  /* FieldScene_RunScene37aSequenceA (0x02000054) */
void Func_02002b36();  /* SetSolShindenActorStep (0x020025fc) */
void Func_02002e7a();  /* Func_0808a018 */
void Func_02002ea6();  /* Func_0808a010 */
void Func_02002eb4();  /* Func_0808a090 */
void Func_02002ed8();  /* Func_0808a0d0 */
void Func_02002ef8();  /* Func_0808a100 */
void Func_02002f00();  /* Func_0808a170 */
void Func_02002f1c();  /* Func_0808a0f0 */
void Func_02002f60();  /* Func_0808a1b8 */
void Func_02002f66();  /* Func_0808a128 */
void Func_02002f8e();  /* Func_0808a0c8 */
void Func_02002fa4();  /* Func_0808a1e8 */
void Func_02002fa6();  /* Func_080f9010 */
void Func_02003030();  /* Func_0808a138 */
void Func_0200304e();  /* Func_0808a110 */
void Func_02003052();  /* Func_0808a208 */
void Func_02003108();  /* Func_0808a1f0 */
void Func_0200316e();  /* Func_0808a180 */
void Func_02003370();  /* Func_0808a0e8 */
void Func_0200338a();  /* Func_080770c8 */
void Func_020033b4();  /* Func_0808a020 */

/*
 * Inner sanctum data getters, published in the descriptor table at the head
 * of overlay resource_37a. Sibling overlays share this layout but not the
 * addresses returned.
 */

#include "TYPES.H"

/*
 * Returns 0x0200aafc, an address inside this overlay's image; its contents
 * are not established. The eight-byte owner includes its one pool word,
 * which sits past the bx lr. The address is published in the descriptor
 * table, not reached by any call.
 */

/*
 * Returns zero. The four-byte owner carries no pool word, since a constant
 * that fits in an immediate needs none, unlike its eight-byte neighbours in
 * the same table. Whether the caller reads the zero as a null pointer or as
 * an integer is not established.
 */

/*
 * Returns 0x0200abec, an address inside this overlay's image; its contents
 * are not established. The eight-byte owner includes its one pool word,
 * which sits past the bx lr. The address is published in the descriptor
 * table, not reached by any call.
 */

/*
 * Returns 0x0200ac14, an address inside this overlay's image; its contents
 * are not established. The eight-byte owner includes its one pool word,
 * which sits past the bx lr. The address is published in the descriptor
 * table, not reached by any call.
 */

/*
 * Returns 0x0200ad34, an address inside this overlay's image; its contents
 * are not established. The eight-byte owner includes its one pool word,
 * which sits past the bx lr. The address is published in the descriptor
 * table, not reached by any call.
 */

#include "TYPES.H"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
extern u8 LinkedMessage_LookFartherUpThePassage[];
extern u8 Data_00004010[];

void Func_0200306c();
void Func_020030c8();
void Func_0200311e();
s32 Func_0200340e();
void Func_020040ee();
void Func_0200410c();
void Func_02004132();
s32 Func_0200458e();

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

/* The scene step counter at 0x1d8 of the shared scene work record. */

static __inline__ void Call1_02000108(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call1_020001ec(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call1_020009f4(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call1_02002924(void (*f)(), s32 a0)
{
    f(a0);
}

/*
 * Inner sanctum scene state: clear the backdrop colour, light the statue
 * groups whose progress flags are set, and step an actor. Overlay
 * resource_37a.
 */

#include "TYPES.H"

/* Declared old-style: this parameter list is not established here. */

s32 Func_02002bf4_scene_state(s32);
/* Declared old-style: this parameter list is not established here. */

/*
 * Passes the step to one routine with a zero and the wait to another. Both
 * arguments are read before either incoming register is overwritten, so the
 * order of these two calls is what the reference requires. The zero is
 * transcribed, not interpreted.
 */

#include "TYPES.H"

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} EntA;

typedef struct {
    u8 filler0[10];
    s16 unkA;
    u8 fillerC[6];
    s16 unk12;
} EntB;

extern u8 LinkedMessage_WayLeadsOutSanctumShould;
extern u16 Data_0200ade4;
extern s32 Data_0200ade8;
extern u16 Data_0200addc;
extern u16 Data_0200ade0;
extern u16 Data_0200adec;

EntA *Func_020035f2(s32);
EntA *Func_0200364a(s32);
EntB *Func_0200372a(s32);

#include "TYPES.H"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];

void Func_02003fe0();
void Func_02003ff0();
void Func_020044f6_field_scene();
void Func_02003b8e();
void Func_02003bbe_field_scene();
u32 Func_02003f4c();
u32 Func_02003f5e();
u32 Func_02003f70_a();
u32 Func_02003f82_a();
s32 Func_02003f8e();
s32 Func_02003f98();
s32 Func_02003fa2();
s32 Func_02003fac();
void Func_02003cfa();
void Func_0200409c();
void Func_020040a2();
void Func_020040a8();
void Func_020040ae();
void UpdateStatueLight1(void);
void UpdateStatueLight2(void);
void UpdateStatueLight3(void);
void UpdateStatueLight4(void);

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */

#include "TYPES.H"

void Func_020039ce();
void Func_02003aa8();
void Func_02003f56_sol_shinden();
/* One import, four call sites, four distinct veneer addresses. */

static __inline__ void SetInitialScale(s32 actor, s32 scale, s32 duration)
{
    Actor_FaceDirection(actor, scale, duration);
}

static __inline__ void SetInitialDirection(s32 actor, s32 direction, s32 duration)
{
    Actor_ShowEmote(actor, direction, duration);
}

static __inline__ void SetFinalScale(s32 actor, s32 horizontal, s32 vertical)
{
    Actor_SetSpeed(actor, horizontal, vertical);
}

u8 *GetSolShindenScriptData(void)
{
    return (u8 *)0x0200aafc;
}

s32 GetSolShindenInitialState(void)
{
    return 0;
}

u8 *GetSolShindenMessageData(void)
{
    return (u8 *)0x0200abec;
}

u8 *GetSolShindenActorData(void)
{
    return (u8 *)0x0200ac14;
}

u8 *GetSolShindenEffectData(void)
{
    return (u8 *)0x0200ad34;
}

void FieldScene_RunScene37aSequenceA(void)
{
    u32 i;
    s32 record;

    if (Value0(CheckAllStatueLights)!= 0) {
        record = GameFlag_IsSet(0x201);
        if (record != 0) {
            goto L_020000f0;
        }
        Event_Begin();
        ColorBuffer_ApplyTarget(0x2051cc, 1);
        ColorBuffer_Interpolate(20);
        GameFlag_Set(0x201);
        GameFlag_Clear(0x200);
        GameFlag_Clear(0x202);
        if (GameFlag_IsSet(FLAG_INNER_SANCTUM_ENTERED) == 0) {
            Scene_EnterInnerSanctum();
        }
        if (Value0(CheckAllStatueLights)!= 0) {
            if (GameFlag_IsSet(0x811) == 0) {
                FieldScene_RunActorPositionTransition();
            }
        }
        Event_End();
    } else {
        if (GameFlag_IsSet(0x200) == 0) {
            Event_Begin();
            ColorBuffer_ApplyTarget(0x10000, 1);
            ColorBuffer_Interpolate(20);
            GameFlag_Set(0x200);
            GameFlag_Clear(0x201);
            GameFlag_Clear(0x202);
            Event_End();
        }
    }
    L_020000f0:;
}

void FieldScene_RunScene37aSequenceB(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x200) == 0) {
        Event_Begin();
        ColorBuffer_ApplyTarget(0x10000, 1);
        ColorBuffer_Interpolate(20);
        GameFlag_Set(0x200);
        GameFlag_Clear(0x201);
        GameFlag_Clear(0x202);
        Event_End();
    }
}

void FieldScene_RunScene37aSequenceC(void)
{
    u32 i;
    s32 record;

    if (Value0(CheckAllStatueLights)!= 0) {
        record = GameFlag_IsSet(0x200);
        if (record != 0) {
            goto L_020001d6;
        }
        Event_Begin();
        ColorBuffer_ApplyTarget(0x10000, 1);
        ColorBuffer_Interpolate(20);
        GameFlag_Set(0x200);
        GameFlag_Clear(0x201);
        GameFlag_Clear(0x202);
        Event_End();
    } else {
        if (GameFlag_IsSet(0x201) == 0) {
            Event_Begin();
            ColorBuffer_ApplyTarget(0x2051cc, 1);
            ColorBuffer_Interpolate(20);
            GameFlag_Set(0x201);
            GameFlag_Clear(0x200);
            GameFlag_Clear(0x202);
            if (GameFlag_IsSet(FLAG_INNER_SANCTUM_ENTERED) == 0) {
                Scene_EnterInnerSanctum();
            }
            Event_End();
        }
    }
    L_020001d6:;
}

void FieldScene_RunScene37aSequenceD(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x202) == 0) {
        ColorBuffer_ApplyTarget(0x202db1, 1);
        ColorBuffer_Interpolate(20);
        GameFlag_Set(0x202);
        GameFlag_Clear(0x200);
        GameFlag_Clear(0x201);
    }
}

void ClearSolShindenBackdrop(void)
{
    s32 black = 0;
    u16 *backdrop_color = (u16 *)0x5000000;
    *backdrop_color = black;
}

void SetStatueLightGroup1(void)
{
    if (Func_02002bf4_scene_state(FLAG_STATUE_LIGHT_1) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2d, 28, 0x22, 10, a, b);
    }
    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_2) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2f, 28, 0x24, 10, a, b);
    }
    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_3) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2d, 29, 0x22, 11, a, b);
    }
    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_4) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2f, 29, 0x24, 11, a, b);
    }
}

void SetStatueLightGroup2(void)
{
    if (GameFlag_IsSet(0x826) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2d, 28, 0x22, 10, a, b);
    }
    if (GameFlag_IsSet(0x827) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2f, 28, 0x24, 10, a, b);
    }
    if (GameFlag_IsSet(0x828) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2d, 29, 0x22, 11, a, b);
    }
    if (GameFlag_IsSet(0x829) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2f, 29, 0x24, 11, a, b);
    }
}

void SetStatueLightGroup3(void)
{
    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_1) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2d, 30, 0x22, 10, a, b);
    }
    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_2) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2f, 30, 0x24, 10, a, b);
    }
    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_3) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2d, 31, 0x22, 11, a, b);
    }
    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_4) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2f, 31, 0x24, 11, a, b);
    }
}

void SetStatueLightGroup4(void)
{
    if (GameFlag_IsSet(0x826) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2d, 30, 0x22, 10, a, b);
    }
    if (GameFlag_IsSet(0x827) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2f, 30, 0x24, 10, a, b);
    }
    if (GameFlag_IsSet(0x828) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2d, 31, 0x22, 11, a, b);
    }
    if (GameFlag_IsSet(0x829) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2f, 31, 0x24, 11, a, b);
    }
}

void FieldScene_RunScene37aSequenceF(void)
{
    u8 *rec;

    if (Value1(Func_02002e42, 0x814) != 0) {
        Func_020004ea();
    }
    if (Value1(Func_02002e42, 0x809) == 0) {
        Func_02002e7a();
        Call1(Func_02002f00, (s32)Data_00000fe3);
        Func_02002fa6(17);
        Call3(Func_02002eb4, 0, 0x10000, 0x8000);
        Call3(Func_02002ed8, 0, 0x120, 232);
        Func_02002ef8(0, 0);
        Func_02002ea6(20);
        Func_02002fa6(21);
        Call3(Func_02002f60, 0, 0xc000, 0);
        rec = (u8 *)Value1(Func_02002ede, 0);
        if (rec != 0) {
            Func_02002f1c(16, FIELD(rec, s32 *, 8), FIELD(rec, s32 *, 16));
        }
        Call3(Func_02002eb4, 16, 0x16666, 0xb333);
        Call3(Func_02002ed8, 16, 0x120, 206);
        Func_02002ea6(40);
        Call3(Func_02002fa4, 16, 0x100, 0);
        Func_02002f66(16, 4, 60);
        Func_02002b36(16, 20);
        rec = (u8 *)Value1(Func_02002ede, 0);
        if (rec != 0) {
            Func_02002f1c(1, FIELD(rec, s32 *, 8), FIELD(rec, s32 *, 16));
        }
        rec = (u8 *)Value1(Func_02002ede, 0);
        if (rec != 0) {
            Func_02002f1c(5, FIELD(rec, s32 *, 8), FIELD(rec, s32 *, 16));
        }
        Call3(Func_02002eb4, 1, 0x8000, 0x4000);
        Call3(Func_02002eb4, 5, 0x8000, 0x4000);
        Call3(Func_02002f8e, 1, 0x118, 248);
        Call3(Func_02002ed8, 5, 0x128, 248);
        Func_02002ef8(1, 1);
        Call3(Func_02002f60, 1, 0xd000, 0);
        Call3(Func_02002f60, 5, 0xb000, 30);
        Call2(Func_02003052, 0x9999, 0x1333);
        Camera_MoveTo(0x1200000, -1, 0xd50000, 1);
        Call3(Func_02002eb4, 16, 0x6666, 0x3333);
        Call3(Func_02002ed8, 16, 0x120, 176);
        Func_02002ea6(40);
        Func_02003030(16, 2);
        Func_02002b36(16, 6);
        Call3(Func_02002f60, 16, 0x4000, 60);
        Func_02002b36(16, 20);
        Func_02002f60(16, 0, 40);
        Func_0200304e(16, 3);
        Func_02002ea6(10);
        Call3(Func_02002f60, 16, 0x8000, 40);
        Func_0200304e(16, 3);
        Func_02002ea6(20);
        Func_02003030(5, 2);
        Call3(Func_02002f60, 5, 0x9000, 10);
        Func_02002b36(5, 10);
        Func_02003030(1, 2);
        Call3(Func_02002f60, 1, 0xf000, 10);
        Func_02002b36(1, 6);
        Call2(Func_02003108, 5, 0x102);
        Func_02002ea6(40);
        Call3(Func_02002f60, 5, 0xa000, 10);
        Call2(Func_02002b36, 0x2005, 10);
        Func_02003030(16, 2);
        Func_02002ea6(10);
        Call3(Func_02002f60, 16, 0xa000, 20);
        Call2(Func_02003108, 16, 0x102);
        Func_02002ea6(20);
        Call3(Func_02002f60, 0, 0x5000, 40);
        Call3(Func_02002f60, 0, 0xc000, 0);
        Call3(Func_02002f60, 1, 0xe000, 0);
        Call3(Func_02002f60, 5, 0xa000, 40);
        Call3(Func_02002fa4, 1, 0x101, 20);
        Func_0200316e(1, 0);
        Func_02002ea6(60);
        Func_0200304e(16, 4);
        Func_02002ea6(40);
        Func_02002b36(16, 20);
        Call3(Func_02002fa4, 5, 0x101, 40);
        Func_02002b36(5, 60);
        Func_0200304e(16, 3);
        Func_02002b36(16, 10);
        Call3(Func_02002fa4, 0, 0x105, 0);
        Call3(Func_02002fa4, 1, 0x105, 0);
        Call3(Func_02002fa4, 5, 0x105, 60);
        Func_02003030(1, 2);
        Func_02002ea6(20);
        Func_02002b36(1, 10);
        Func_0200304e(16, 3);
        Func_02002ea6(20);
        Call3(Func_02002fa4, 0, 0x102, 0);
        Call3(Func_02002fa4, 1, 0x102, 0);
        Call3(Func_02002fa4, 5, 0x102, 80);
        Call3(Func_02002fa4, 16, 0x105, 80);
        Func_02002b36(16, 6);
        Call3(Func_02002f60, 0, 0x4000, 0);
        Call3(Func_02002f60, 1, 0xf000, 0);
        Call3(Func_02002f60, 5, 0x9000, 60);
        Call3(Func_02002f60, 16, 0x4000, 10);
        Func_02003030(16, 3);
        Func_02002ea6(6);
        Value2(Func_02003262, 16, 0);
        if (Value2(Func_020031f2, 0, 0) == 0) {
            Func_02002f00((s32)Data_00000ff0);
        } else {
            Call1(Func_02002f00, (s32)Data_00000ff1);
            Call3(Func_02002fa4, 16, 0x107, 20);
        }
        Func_02002f66(16, 4, 20);
        Call3(Func_02002f60, 0, 0xc000, 0);
        Call3(Func_02002f60, 1, 0xe000, 0);
        Call3(Func_02002f60, 5, 0xa000, 0);
        Func_02002b36(16, 6);
        Call1(Func_02002f00, (s32)Data_00000ff2);
        Func_02002ea6(30);
        Func_0200304e(5, 4);
        Call2(Func_02002b36, 0x2005, 6);
        Func_0200304e(1, 3);
        Func_02002b36(1, 20);
        Func_02002f66(16, 6, 20);
        Call3(Func_02002fa4, 16, 0x104, 20);
        Func_02002b36(16, 30);
        Func_02002ef8(0, 3);
        Func_02002ef8(1, 3);
        Func_0200304e(5, 3);
        Func_02002ea6(20);
        Func_0200304e(16, 3);
        Func_02002b36(16, 6);
        Call3(Func_02002eb4, 1, 0x10000, 0x8000);
        Call3(Func_02002eb4, 5, 0x10000, 0x8000);
        Call3(Func_02002eb4, 16, 0x20000, 0x10000);
        Func_02002ef8(16, 2);
        rec = (u8 *)Value1(Func_02002ede, 0);
        if (rec != 0) {
            Actor_SetDestination(ACTOR_SUKURETA, FIELD(rec, s16 *, 10), FIELD(rec, s16 *, 18));
        }
        Func_02003370(16);
        Func_02002f1c(16, 0, 0);
        Func_02002ef8(1, 2);
        rec = (u8 *)Value1(Func_02002ede, 0);
        if (rec != 0) {
            Actor_SetDestination(ACTOR_GERALD, FIELD(rec, s16 *, 10), FIELD(rec, s16 *, 18));
        }
        Func_02003370(1);
        Func_02002f1c(1, 0, 0);
        Func_02002ef8(5, 2);
        rec = (u8 *)Value1(Func_02002ede, 0);
        if (rec != 0) {
            Actor_SetDestination(ACTOR_JASMINE, FIELD(rec, s16 *, 10), FIELD(rec, s16 *, 18));
        }
        Func_02003370(5);
        Func_02002f1c(5, 0, 0);
        Call1(Func_0200338a, 0x144);
        Call1(Func_0200338a, 0x809);
        Func_020033b4();
    }
}

void Scene_EnterInnerSanctum(void)
{
    u32 i;
    s32 record;
    s32 base6_ffc;
    s32 base5_4010;
    s32 base5_4010_2;

    Event_SetMessage(MSG_WHAT_IS_THIS_ROOM);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1e8, 176);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 0);
    record = Value1(Func_0200340e, 0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_SUKURETA, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 1);
    Actor_SetSpeed(ACTOR_SUKURETA, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1d8, 168);
    Actor_FaceDirection(ACTOR_SUKURETA, 0, 60);
    Actor_Jump(ACTOR_SUKURETA, 4, 40);
    Func_0200306c(16, 6);
    Camera_SetSpeed(0x26666, 0x4ccc);
    Camera_MoveTo(0x23f0000, -1, 0xb50000, 1);
    Camera_WaitForMove();
    Event_Wait(120);
    Call2(SetSolShindenActorStep, 0x1010, 80);
    Camera_MoveTo(0x1ec0000, -1, 0xa80000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    base5_4010 = (s32)Data_00004010;
    Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 20);
    Func_020030c8(base5_4010, 6);
    Actor_FaceDirection(ACTOR_SUKURETA, 0, 60);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 10);
    Event_OpenMessage(base5_4010, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_SetMessage(MSG_SUKURETA_THESE_ROOMS_HIDE_A_SECRET);
    } else {
        Event_SetMessage(MSG_SUKURETA_A_ROOM_FOR_LUNA_AND_SOL);
    }
    base5_4010_2 = (s32)Data_00004010;
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 10);
    Func_0200311e(base5_4010_2, 10);
    base6_ffc = (s32)LinkedMessage_LookFartherUpThePassage;
    Event_SetMessage(base6_ffc);
    Actor_FaceDirection(ACTOR_SUKURETA, 0, 40);
    Actor_ShowEmote(ACTOR_SUKURETA, 0x105, 40);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 4);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 10);
    Actor_SetAnimation(ACTOR_SUKURETA, 4);
    Event_OpenMessage(base5_4010_2, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_SetMessage((base6_ffc + 1));
        GameFlag_Set(FLAG_ROBIN_SEARCHING_FOR_SUKURETA);
    } else {
        Event_SetMessage((base6_ffc + 2));
    }
    Call2(SetSolShindenActorStep, (s32)Data_00004010, 4);
    Camera_FollowActor(ACTOR_SUKURETA, 1);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1e6, 131);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x240, 120);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 2);
    Camera_SetSpeed(0x40000, 0x8000);
    GameFlag_Set(FLAG_INNER_SANCTUM_ENTERED);
}

void UpdateStatueTrapActor(void)
{
    EntA *scene_actor;
    EntA *target_actor;
    EntB *target_position;
    s32 g1 = 0x810;
    s32 g2 = 0x810;
    s32 g3 = 0x810;
    s32 g4 = 0x810;
    s32 s1 = 0x10000;
    s32 s2 = 0x8000;
    s32 s3 = 0x20000;
    s32 s4 = 0x10000;
    s32 s5 = 0x4000;
    s32 d1 = 0x120;
    s32 d2 = 0x120;
    s32 d3 = 0x120;
    s32 d4 = 0x120;
    s32 d5 = 0xc000;
    s32 h1;
    s32 h2;

    scene_actor = Func_020035f2(16);
    if (GameFlag_IsSet(0x809) == 0) {
        return;
    }
    if (GameFlag_IsSet(0x814) != 0) {
        FieldScene_RunScene37aSequenceB();
        return;
    }
    if (GameFlag_IsSet(0x819) != 0) {
        return;
    }
    Event_Begin();
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 0);
    Event_SetMessage((s32)&LinkedMessage_WayLeadsOutSanctumShould);
    if (GameFlag_IsSet(g1)!= 0 || GameFlag_IsSet(FLAG_INNER_SANCTUM_ENTERED) == 0) {
        target_actor = Func_0200364a(0);
        if (target_actor != 0) {
            Actor_SetPosition(ACTOR_SUKURETA, target_actor->unk8, target_actor->unk10);
        }
        Event_Wait(4);
        Actor_SetSpeed(ACTOR_SUKURETA, s1, s2);
    } else {
        if (GameFlag_IsSet(g2)!= 0) goto do1;
        h1 = 0x1540000;
        if (scene_actor->unk8 > h1) {
do1:
            Actor_SetPosition(ACTOR_SUKURETA, 0x1880000, 0xa80000);
            Event_Wait(4);
            Actor_SetSpeed(ACTOR_SUKURETA, s3, s4);
        }
    }
    if (GameFlag_IsSet(g3)!= 0) goto do2;
    h2 = 0x1540000;
    if (scene_actor->unk8 > h2) {
do2:
        Actor_WalkToAndWait(ACTOR_SUKURETA, d1, 0xe8);
    } else {
        GameFlag_IsSet(FLAG_INNER_SANCTUM_ENTERED);
    }
    Actor_WalkToAndWait(ACTOR_SUKURETA, d2, 0xe8);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, d5, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, s5, 10);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    if (GameFlag_IsSet(g4)!= 0 || GameFlag_IsSet(FLAG_INNER_SANCTUM_ENTERED) == 0) {
        Actor_SetAnimation(ACTOR_SUKURETA, 2);
        target_position = Func_0200372a(0);
        if (target_position != 0) {
            Actor_SetDestination(ACTOR_SUKURETA, target_position->unkA, target_position->unk12);
        }
        Actor_WaitForMove(ACTOR_SUKURETA);
        Actor_SetPosition(ACTOR_SUKURETA, 0, 0);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, d3, 0xe8);
    } else {
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, d4, 0xf8);
    }
    Event_End();
}

void UpdateStatueLight1(void)
{
    if ((Random_Next() & 3) != 0) {
        switch (Data_0200ade4) {
        case 0:
            Audio_PlayCue(0xbb);
            Map_CopyCellsTo(0x2e, 0x3b, 30, 0x21, 1, 5);
            break;
        case 1:
            Map_CopyCellsTo(0x2e, 0x3b, 30, 0x21, 1, 1);
            Map_CopyCellsTo(0x2e, 0x3b, 30, 0x22, 1, 5);
            break;
        case 2:
            Map_CopyCellsTo(0x2e, 0x3b, 30, 0x22, 1, 1);
            Map_CopyCellsTo(0x2e, 0x3b, 30, 0x23, 1, 5);
            break;
        case 3:
            Map_CopyCellsTo(0x2e, 0x3b, 30, 0x23, 1, 1);
            Map_CopyCellsTo(0x2e, 0x3b, 30, 0x24, 1, 5);
            break;
        case 4:
            Data_0200ade8 = 2;
            Map_CopyCellsTo(0x2e, 0x3b, 30, 0x24, 1, 1);
            Map_CopyCellsTo(0x2e, 0x3b, 30, 0x25, 1, 5);
            break;
        case 0x50:
            Map_CopyCellsTo(0x2e, 0x31, 30, 0x21, 1, 10);
            break;
        }
        Data_0200ade4++;
        if (Data_0200ade4 > ((u32)(Random_Next() * 40) >> 16) + 90) {
            Data_0200ade4 = 0;
        }
    }
    if (Data_0200ade8 != 0) {
        if (Data_0200ade8 == 2) {
            Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
        } else if (Data_0200ade8 == 1) {
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        }
        Data_0200ade8--;
    }
}

void UpdateStatueLight2(void)
{
    if ((Random_Next() & 3) != 0) {
        switch (Data_0200addc) {
        case 0:
            Audio_PlayCue(0xbb);
            Map_CopyCellsTo(0x2f, 0x3b, 42, 0x21, 1, 5);
            break;
        case 1:
            Map_CopyCellsTo(0x2f, 0x3b, 42, 0x21, 1, 1);
            Map_CopyCellsTo(0x2f, 0x3b, 42, 0x22, 1, 5);
            break;
        case 2:
            Map_CopyCellsTo(0x2f, 0x3b, 42, 0x22, 1, 1);
            Map_CopyCellsTo(0x2f, 0x3b, 42, 0x23, 1, 5);
            break;
        case 3:
            Map_CopyCellsTo(0x2f, 0x3b, 42, 0x23, 1, 1);
            Map_CopyCellsTo(0x2f, 0x3b, 42, 0x24, 1, 5);
            break;
        case 4:
            Data_0200ade8 = 2;
            Map_CopyCellsTo(0x2f, 0x3b, 42, 0x24, 1, 1);
            Map_CopyCellsTo(0x2f, 0x3b, 42, 0x25, 1, 5);
            break;
        case 0x5a:
            Map_CopyCellsTo(0x2f, 0x31, 42, 0x21, 1, 10);
            break;
        }
        Data_0200addc++;
        if (Data_0200addc > ((u32)(Random_Next() * 40) >> 16) + 100) {
            Data_0200addc = 0;
        }
    }
}

void UpdateStatueLight3(void)
{
    if ((Random_Next() & 3) != 0) {
        switch (Data_0200ade0) {
        case 0:
            Audio_PlayCue(0xbb);
            Map_CopyCellsTo(0x30, 0x3b, 31, 0x24, 1, 5);
            break;
        case 1:
            Map_CopyCellsTo(0x30, 0x3b, 31, 0x24, 1, 1);
            Map_CopyCellsTo(0x30, 0x3b, 31, 0x25, 1, 5);
            break;
        case 2:
            Map_CopyCellsTo(0x30, 0x3b, 31, 0x25, 1, 1);
            Map_CopyCellsTo(0x30, 0x3b, 31, 0x26, 1, 5);
            break;
        case 3:
            Map_CopyCellsTo(0x30, 0x3b, 31, 0x26, 1, 1);
            Map_CopyCellsTo(0x30, 0x3b, 31, 0x27, 1, 5);
            break;
        case 4:
            Data_0200ade8 = 2;
            Map_CopyCellsTo(0x30, 0x3b, 31, 0x27, 1, 1);
            Map_CopyCellsTo(0x30, 0x3b, 31, 0x28, 1, 5);
            break;
        case 0x5f:
            Map_CopyCellsTo(0x30, 0x31, 31, 0x24, 1, 10);
            break;
        }
        Data_0200ade0++;
        if (Data_0200ade0 > ((u32)(Random_Next() * 40) >> 16) + 105) {
            Data_0200ade0 = 0;
        }
    }
}

void UpdateStatueLight4(void)
{
    if ((Random_Next() & 3) != 0) {
        switch (Data_0200adec) {
        case 0:
            Audio_PlayCue(0xbb);
            Map_CopyCellsTo(0x2e, 0x3b, 41, 0x24, 1, 5);
            break;
        case 1:
            Map_CopyCellsTo(0x2e, 0x3b, 41, 0x24, 1, 1);
            Map_CopyCellsTo(0x2e, 0x3b, 41, 0x25, 1, 5);
            break;
        case 2:
            Map_CopyCellsTo(0x2e, 0x3b, 41, 0x25, 1, 1);
            Map_CopyCellsTo(0x2e, 0x3b, 41, 0x26, 1, 5);
            break;
        case 3:
            Map_CopyCellsTo(0x2e, 0x3b, 41, 0x26, 1, 1);
            Map_CopyCellsTo(0x2e, 0x3b, 41, 0x27, 1, 5);
            break;
        case 4:
            Data_0200ade8 = 2;
            Map_CopyCellsTo(0x2e, 0x3b, 41, 0x27, 1, 1);
            Map_CopyCellsTo(0x2e, 0x3b, 41, 0x28, 1, 5);
            break;
        case 0x55:
            Map_CopyCellsTo(0x2e, 0x31, 41, 0x24, 1, 10);
            break;
        }
        Data_0200adec++;
        if (Data_0200adec > ((u32)(Random_Next() * 40) >> 16) + 95) {
            Data_0200adec = 0;
        }
    }
}

void FieldScene_PrepareStatueTransition(void)
{
    u32 i;
    s32 record;

    Camera_MoveTo(-1, -1, -1, 0);
    Map_CopyCellsTo(30, 43, 32, 40, 8, 3);
    Map_CopyCellsTo(30, 43, 33, 39, 8, 1);
    Map_CopyCellsTo(30, 43, 36, 38, 3, 3);
    Map_CopyCellsTo(14, 41, 32, 41, 8, 4);
    Camera_MoveTo(0x23e0000, -1, 0x9e0000, 0);
    Map_Redraw();
    Actor_SetPosition(ACTOR_SUKURETA, 0x23e0000, 0x780000);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Task_Wait(1);
    ColorBuffer_ApplyTarget(0x2051cc, 1);
    ColorBuffer_Interpolate(20);
    GameFlag_Set(0x201);
    GameFlag_Clear(0x200);
    GameFlag_Clear(0x202);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 32;
    Event_OpenScreen();
    ((void (*)())Engine_EventWaitForScreen)();
    Event_Wait(40);
    Audio_PlayCue(171);
    ColorBuffer_ApplyTarget(0x10005, 1);
    ColorBuffer_Interpolate(8);
    Event_Wait(32);
    Value2(Engine_ColorBufferApplyTarget, 0x2051cc, 1);
    ColorBuffer_Interpolate(24);
}

void Scene_SpringStatueTrap(void)
{
    s32 outer_pair;
    s32 second_pair;
    s32 middle_pair;
    s32 fourth_pair;
    s32 inner_pair;

    Event_Begin();
    FieldScene_PrepareStatueTransition();
    Event_SetMessage(MSG_WHATS_HAPPENING_AT_THE_TRAP);
    SetInitialScale(ACTOR_SUKURETA, 0x4000, 20);
    SetInitialDirection(ACTOR_SUKURETA, 256, 0);
    Actor_Jump(ACTOR_SUKURETA, 6, 30);
    Camera_MoveTo(37617664, -1, 11403264, 1);
    Camera_WaitForMove();
    Event_Wait(30);
    Func_020039ce(32784, 20);
    for (outer_pair = 0; outer_pair != 4; outer_pair++) {
        Audio_PlayCue(246);
        SetStatueLightGroup1();
        Event_Wait(12);
        Audio_PlayCue(246);
        SetStatueLightGroup3();
        Event_Wait(12);
    }
    for (second_pair = 0; second_pair != 6; second_pair++) {
        Audio_PlayCue(246);
        SetStatueLightGroup1();
        Event_Wait(8);
        Audio_PlayCue(246);
        SetStatueLightGroup3();
        Event_Wait(8);
    }
    for (middle_pair = 0; middle_pair != 8; middle_pair++) {
        Audio_PlayCue(246);
        SetStatueLightGroup1();
        Event_Wait(6);
        Audio_PlayCue(246);
        SetStatueLightGroup3();
        Event_Wait(6);
    }
    for (fourth_pair = 0; fourth_pair != 10; fourth_pair++) {
        Audio_PlayCue(246);
        SetStatueLightGroup1();
        Event_Wait(4);
        Func_02003f56_sol_shinden(246);
        SetStatueLightGroup3();
        Event_Wait(4);
    }
    for (inner_pair = 0; inner_pair != 12; inner_pair++) {
        Audio_PlayCue(246);
        SetStatueLightGroup1();
        Event_Wait(2);
        Audio_PlayCue(246);
        SetStatueLightGroup3();
        Event_Wait(2);
    }
    SetStatueLightGroup1();
    Event_Wait(6);
    Func_02003aa8(32784, 6);
    SetFinalScale(ACTOR_SUKURETA, 0x20000, 0x10000);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 576, 280);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 32;
    Event_CloseScreen();
    Event_WaitForScreen();
    GameFlag_Set(FLAG_STATUE_TRAP_SPRUNG);
    Event_RequestExit(3);
}

void FieldScene_RunClosingSequence(void)
{
    s32 i;
    Event_Begin();
    FieldScene_PrepareStatueTransition();
    Data_0200ade4 = 0;
    Data_0200addc = 0;
    Data_0200ade0 = 0;
    Data_0200adec = 0;
    Event_SetMessage(MSG_WHATS_HAPPENING);
    Actor_FaceDirection(ACTOR_SUKURETA, 16384, 20);
    Actor_ShowEmote(ACTOR_SUKURETA, 256, 0);
    Actor_Jump(ACTOR_SUKURETA, 6, 30);
    Camera_MoveTo(37617664, -1, 11403264, 1);
    Camera_WaitForMove();
    Event_Wait(30);
    Func_02003b8e(32784, 20);
    for (i = 0; i != 4; i++) {
        Audio_PlayCue(246);
        SetStatueLightGroup1();
        Event_Wait(12);
        Audio_PlayCue(246);
        SetStatueLightGroup3();
        Event_Wait(12);
    }
    Func_02003bbe_field_scene(32784, 6);
    Data_0200ade4 = ((Func_02003f4c() * 60) >> 16) + 20;
    Data_0200addc = ((Func_02003f5e() * 60) >> 16) + 20;
    Data_0200ade0 = ((Func_02003f70_a() * 60) >> 16) + 20;
    Data_0200adec = ((Func_02003f82_a() * 60) >> 16) + 20;
    Data_0200ade8 = 0;
    Value2(Func_02003f8e, (s32)UpdateStatueLight1, 3200);
    Value2(Func_02003f98, (s32)UpdateStatueLight2, 3200);
    Value2(Func_02003fa2, (s32)UpdateStatueLight3, 3200);
    Value2(Func_02003fac, (s32)UpdateStatueLight4, 3200);
    for (i = 0; i != 6; i++) {
        Audio_PlayCue(246);
        SetStatueLightGroup1();
        Event_Wait(5);
        Audio_PlayCue(246);
        SetStatueLightGroup3();
        Event_Wait(5);
    }
    for (i = 0; i != 8; i++) {
        Audio_PlayCue(246);
        SetStatueLightGroup1();
        Event_Wait(4);
        Audio_PlayCue(246);
        SetStatueLightGroup3();
        Event_Wait(4);
    }
    for (i = 0; i != 10; i++) {
        Audio_PlayCue(246);
        SetStatueLightGroup1();
        Event_Wait(3);
        Audio_PlayCue(246);
        SetStatueLightGroup3();
        Event_Wait(3);
    }
    for (i = 0; i != 12; i++) {
        Audio_PlayCue(246);
        SetStatueLightGroup1();
        Event_Wait(2);
        Audio_PlayCue(246);
        SetStatueLightGroup3();
        Event_Wait(2);
    }
    Map_CopyCellsTo(45, 30, 34, 10, 4, 2);
    Actor_Jump(ACTOR_SUKURETA, 6, 40);
    Func_02003cfa(32784, 6);
    Actor_SetSpeed(ACTOR_SUKURETA, 131072, 65536);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 576, 280);
    Func_0200409c((s32)UpdateStatueLight1);
    Func_020040a2((s32)UpdateStatueLight2);
    Func_020040a8((s32)UpdateStatueLight3);
    Func_020040ae((s32)UpdateStatueLight4);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 32;
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(4);
}

void FieldScene_RunFlaggedSequence(void)
{
    s32 base;
    s32 i6;
    s32 i7;
    s32 i8;
    s32 i9;
    s32 i10;

    Event_Begin();
    if (GameFlag_IsSet(2059) != 0 && GameFlag_IsSet(2086) != 0) {
        GameFlag_Clear(2086);
        Map_CopyCellsTo(45, 28, 34, 10, 2, 1);
    } else if (GameFlag_IsSet(2059) != 0 && GameFlag_IsSet(2086) == 0) {
        GameFlag_Set(2086);
    }
    if (GameFlag_IsSet(2060) != 0 && GameFlag_IsSet(2087) != 0) {
        GameFlag_Clear(2087);
        Map_CopyCellsTo(47, 28, 36, 10, 2, 1);
    } else if (GameFlag_IsSet(2060) != 0 && GameFlag_IsSet(2087) == 0) {
        GameFlag_Set(2087);
    }
    if (GameFlag_IsSet(2061) != 0 && GameFlag_IsSet(2088) != 0) {
        GameFlag_Clear(2088);
        Map_CopyCellsTo(45, 29, 34, 11, 2, 1);
    } else if (GameFlag_IsSet(2061) != 0 && GameFlag_IsSet(2088) == 0) {
        GameFlag_Set(2088);
    }
    if (GameFlag_IsSet(2062) != 0 && GameFlag_IsSet(2089) != 0) {
        GameFlag_Clear(2089);
        Map_CopyCellsTo(47, 29, 36, 11, 2, 1);
    } else if (GameFlag_IsSet(2062) != 0 && GameFlag_IsSet(2089) == 0) {
        GameFlag_Set(2089);
    }
    FieldScene_PrepareStatueTransition();
    Actor_FaceDirection(ACTOR_SUKURETA, 16384, 20);
    Actor_Jump(ACTOR_SUKURETA, 6, 30);
    Camera_MoveTo(37617664, -1, 11403264, 1);
    Camera_WaitForMove();
    Event_Wait(30);
    for (i6 = 0; i6 != 4; i6++) {
        Audio_PlayCue(246);
        SetStatueLightGroup2();
        Event_Wait(12);
        Audio_PlayCue(246);
        SetStatueLightGroup4();
        Event_Wait(12);
    }
    for (i7 = 0; i7 != 6; i7++) {
        Audio_PlayCue(246);
        SetStatueLightGroup2();
        Event_Wait(8);
        Audio_PlayCue(246);
        SetStatueLightGroup4();
        Event_Wait(8);
    }
    for (i8 = 0; i8 != 8; i8++) {
        Audio_PlayCue(246);
        SetStatueLightGroup2();
        Event_Wait(6);
        Audio_PlayCue(246);
        SetStatueLightGroup4();
        Event_Wait(6);
    }
    for (i9 = 0; i9 != 10; i9++) {
        Audio_PlayCue(246);
        SetStatueLightGroup2();
        Event_Wait(4);
        Audio_PlayCue(246);
        SetStatueLightGroup4();
        Event_Wait(4);
    }
    for (i10 = 0; i10 != 12; i10++) {
        Audio_PlayCue(246);
        SetStatueLightGroup2();
        Event_Wait(2);
        Audio_PlayCue(246);
        SetStatueLightGroup4();
        Event_Wait(2);
    }
    Audio_PlayCue(246);
    SetStatueLightGroup2();
    Event_Wait(6);
    if (GameFlag_IsSet(2082) == 0) {
        base = 32784;
        Event_SetMessage(MSG_YOU_FOUND_IT);
        Func_02003fe0(base, 6);
        Actor_SetAnimationAndWait(ACTOR_SUKURETA, 3);
        Func_02003ff0(base, 6);
    }
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 32;
    Event_CloseScreen();
    Func_020044f6_field_scene();
    Event_RequestExit(5);
}

void Scene_ChangeLunaPictureToSol(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(FLAG_LUNA_PICTURE_CHANGED_TO_SOL) != 0) {
    } else {
        if (Value0(CheckAllStatueLights) == 0) {
        } else {
            Event_Begin();
            Actor_SetPosition(ACTOR_SUKURETA, 0x2410000, 0x930000);
            Actor_FaceDirection(ACTOR_SUKURETA, 0x4000, 1);
            Camera_MoveTo(0x23e0000, -1, 0xb80000, 1);
            Event_SetMessage(MSG_THE_PICTURE_OF_LUNA_CHANGED_TO_SOL);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x240, 232);
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 0);
            Camera_WaitForMove();
            Event_Wait(10);
            Actor_SetSpeed(ACTOR_SUKURETA, 0x10000, 0x8000);
            Actor_WalkToAndWait(ACTOR_SUKURETA, 0x240, 152);
            Event_Wait(6);
            Actor_Jump(ACTOR_SUKURETA, 6, 30);
            Func_020040ee(16, 6);
            Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
            Event_Wait(2);
            Actor_SetAnimationAndWait(ACTOR_SUKURETA, 4);
            Func_0200410c(16, 6);
            Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
            Event_Wait(40);
            Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
            Event_Wait(30);
            Func_02004132(16, 6);
            Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
            Actor_WalkToAndWait(ACTOR_SUKURETA, 0x240, 184);
            Event_Wait(6);
            Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
            Event_Wait(40);
            Call2(SetSolShindenActorStep, 0x4010, 6);
            Actor_WalkToAndWait(ACTOR_SUKURETA, 0x240, 208);
            Event_Wait(40);
            Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
            Event_Wait(6);
            Actor_SetSpeed(ACTOR_SUKURETA, 0x8000, 0x4000);
            Actor_SetAnimation(ACTOR_SUKURETA, 2);
            record = Value1(Func_0200458e, 0);
            if (record != 0) {
                Actor_SetDestination(ACTOR_SUKURETA, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Actor_WaitForMove(ACTOR_SUKURETA);
            Actor_SetPosition(ACTOR_SUKURETA, 0, 0);
            GameFlag_Set(FLAG_LUNA_PICTURE_CHANGED_TO_SOL);
            Event_End();
        }
    }
}

void FieldScene_RunActorPositionTransition(void)
{
    u32 i;
    s32 record;

    Func_020046e2(21);
    Call3(Func_02004606, 0, 0x178, 184);
    Func_02004626(0, 0);
    Call3(Func_0200462c, 16, 0x1780000, 0xb80000);
    Call3(Func_02004612, 16, 0x10000, 0x8000);
    Call3(Func_02004636, 16, 0x188, 168);
    Call3(Func_020046aa, 16, 0x8000, 30);
    Func_02004662(16, 1);
    Event_SetMessage((s32)SCENE_STEP_VALUE);
    Func_02004682(16, 4, 30);
    Func_02004252_actor_step(16, 6);
    Func_0200469a(0, 2);
    Func_02004630(6);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 3);
    Func_02004270(16, 6);
    Call3(Func_0200468c, 16, 0x178, 184);
    Call3(Func_020046a8, 16, 0x6480000, 0x6480000);
    Func_0200465e(4);
    Call1(Func_02004654, 0x811);
}

s32 CheckAllStatueLights(void)
{
    s32 all_set = 1;

    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_1) == 0)
        all_set = 0;
    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_2) == 0)
        all_set = 0;
    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_3) == 0)
        all_set = 0;
    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_4) == 0)
        all_set = 0;

    return all_set;
}

void SetSolShindenActorStep(s32 actor_step, s32 wait_frames)
{
    Event_ShowMessage(actor_step, 0);
    Event_Wait(wait_frames);
}

void Sukureta_Talk(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(FLAG_ROBIN_SEARCHING_FOR_SUKURETA) != 0) {
        Event_SetMessage(MSG_SUKURETA_LET_ME_KNOW_WHAT_YOU_FIND);
    } else {
        Event_SetMessage(MSG_SUKURETA_JUST_WAIT_OVER_THERE);
    }
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 10);
    Event_End();
}
