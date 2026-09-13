#include "types.h"

#if defined(GS1_EDITION_JA)
#define SCENE_STEP_VALUE Value_000011b4
#elif defined(GS1_EDITION_DE) || defined(GS1_EDITION_ES) || defined(GS1_EDITION_FR) || defined(GS1_EDITION_IT)
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
void Func_02004698_a();
void Func_02004698_b();
void Func_0200469a();
void Func_020046a8();
void Func_020046aa();
void Func_020046e2();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

#include "types.h"

/* Message ids handed to the Func_0808a170 service; see the header note. */
extern u8 Data_00000fe3[];
extern u8 Data_00000ff0[];
extern u8 Data_00000ff1[];
extern u8 Data_00000ff2[];

#define FieldScene_RunScene37aSequenceF Func_02000488

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
 * games/gs1/src/overlays/scene_primary_script/run_scene_37a_sequence_a.c uses,
 * so one target reached from several sites appears under several spellings.
 *
 * RESIDUAL (all 32 remaining differing halfwords).  Eight of those spellings
 * collide: two sites whose different call words happen to decode to the same
 * address (Func_0200316c, Func_02003234, Func_0200326c, Func_02003330,
 * Func_02003352, Func_02003382, Func_020033b2 and Func_02003390).
 * Separating them needs an `absolute_symbols` entry in the translation-unit
 * manifest, which a single-file draft may not add, so all sixteen of those
 * sites are spelled with the main-image name of the service they reach.
 * Those calls are semantically right; they emit the direct long call instead
 * of this overlay's veneer word, and nothing else about them differs.
 *
 * The four ids handed to the Func_0808a170 message service are spelled as
 * `Data_00000<id>` externs, the convention the adopted overlay scene scripts
 * already use for this family (see
 * games/gs1/src/overlays/scene_primary_script/run_scene_372_sequence_a.c,
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
 * shared type in games/gs1/include covers it.
 */

/* Each declaration's comment names the owner or main-image service the call
 * word reaches. */

/* The sixteen colliding sites reach these main-image services directly. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block,
 * which the reference's bare `push {lr}` frame shows did not happen.
 * A value-returning call also sets r0 last of its arguments. */



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
void Func_0200306c_sequence();  /* Func_0808a210 */
void Func_02003108();  /* Func_0808a1f0 */
void Func_0200316e();  /* Func_0808a180 */
void Func_02003352();  /* main-image name; collision fallback, see the RESIDUAL note */
void Func_02003370();  /* Func_0808a0e8 */
void Func_0200338a();  /* Func_080770c8 */
void Func_020033b4();  /* Func_0808a020 */

/*
 * Inner sanctum data getters, published in the descriptor table at the head
 * of overlay resource_37a. Sibling overlays share this layout but not the
 * addresses returned.
 */

#include "types.h"

#define GetSolShindenScriptData Func_02000030
#define GetSolShindenInitialState Func_02000038
#define GetSolShindenMessageData Func_0200003c
#define GetSolShindenActorData Func_02000044
#define GetSolShindenEffectData Func_0200004c
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

#include "types.h"

#define FieldScene_RunScene37aSequenceA Func_02000054
#define FieldScene_RunScene37aSequenceB Func_02000108
#define FieldScene_RunScene37aSequenceC Func_02000150
#define FieldScene_RunScene37aSequenceD Func_020001ec
#define FieldScene_RunScene37a_020009f4 Func_020009f4
#define FieldScene_RunScene37aSequenceE Func_02001a58
#define FieldScene_RunScene37a_02002924 Func_02002924
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000ffc[];
extern u8 Data_00004010[];

void Func_02000a8e();
void Func_02001c98();
s32 Func_02002608();
s32 Func_0200264e();
s32 Func_02002a16();
void Func_02002a3a();
void Func_02002a3e();
s32 Func_02002a46();
void Func_02002a4a();
void Func_02002a50();
s32 Func_02002a5c();
s32 Func_02002a72();
void Func_02002a90();
void Func_02002a9a_a();
void Func_02002a9a_b();
void Func_02002aa8();
void Func_02002aae();
void Func_02002aca();
void Func_02002b2e();
void Func_02002b44();
void Func_02002b8c();
void Func_02002ba2();
s32 Func_02002ac4();
void Func_02002aec_a();
void Func_02002aec_b();
void Func_02002afa();
void Func_02002b00();
void Func_02002b1c();
void Func_02002bde();
void Func_02002bf4();
void Func_02000bc4();
s32 Func_02002704();
s32 Func_02002b14();
void Func_02002b3c_a();
void Func_02002b3c_b();
void Func_02002b4a();
s32 Func_02002b4c();
void Func_02002b50();
void Func_02002b6c();
void Func_02002b70();
void Func_02002b74();
s32 Func_02002b7c();
void Func_02002b80();
void Func_02002b86();
void Func_02002bb0();
void Func_02002c2e();
void Func_02002c44();
void Func_02002c64();
void Func_02002c7a();
s32 Func_02002ba6();
void Func_02002bc6();
void Func_02002bd6();
void Func_02002bdc();
void Func_02002cba();
void Func_02002cd0();
void Func_0200306c();
void Func_02003096();
void Func_020030c8();
void Func_0200311e();
void Func_02003186();
void Func_020033fc();
s32 Func_0200340e();
void Func_02003420();
void Func_0200343c();
void Func_02003440();
void Func_0200344c();
void Func_0200344e();
void Func_0200345e();
void Func_02003460();
void Func_02003482();
void Func_0200349c();
void Func_020034ae();
void Func_020034d2();
s32 Func_020034de();
void Func_0200350c();
void Func_0200351a();
void Func_02003524();
void Func_02003530();
void Func_02003536();
void Func_02003538();
void Func_02003548();
void Func_0200354a();
s32 Func_0200354e_a();
s32 Func_0200354e_b();
void Func_02003554();
void Func_02003558();
void Func_0200355e();
void Func_02003560();
void Func_02003572();
void Func_0200357e_a();
void Func_0200357e_b();
void Func_02003584();
void Func_0200358e();
void Func_020035a8();
void Func_020035aa();
void Func_020035b6();
void Func_020035ba();
s32 Func_020035be();
void Func_020035c6();
void Func_020035c8();
void Func_020035d6();
void Func_0200361e();
void Func_0200362a();
void Func_02003656();
s32 Func_0200401a();
void Func_020040ee();
void Func_0200410c();
void Func_02004132();
void Func_02004162();
s32 Func_02004414();
void Func_02004448();
void Func_0200448c();
void Func_0200449c();
void Func_020044ac();
void Func_020044ba();
void Func_020044c2();
void Func_020044cc();
void Func_020044da();
void Func_020044e6();
void Func_020044ec();
void Func_020044f6();
void Func_020044fa();
void Func_02004500();
void Func_0200451c();
void Func_0200451e();
void Func_02004526();
void Func_0200452a();
void Func_02004534();
void Func_02004538();
void Func_02004544();
void Func_02004552();
void Func_02004556();
void Func_0200455e();
void Func_02004564();
void Func_0200456a();
void Func_0200457e();
void Func_02004588_a();
void Func_02004588_b();
s32 Func_0200458e();
void Func_02004594();
void Func_0200459e();
void Func_020045ac_a();
void Func_020045ac_b();
void Func_020045b0();
void Func_020045c0();
void Func_020045ce();
void Func_020045e0();
s32 Func_020052e2();
void Func_020052fc();
void Func_02005336();
void Func_0200538c();
void Func_02005394();
void Func_020053b6();
void Func_020053ca();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}







/* The scene step counter at 0x1d8 of the shared scene work record. */


/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02000108(void (*f)(), s32 a0)
{
    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_020001ec(void (*f)(), s32 a0)
{
    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_020009f4(void (*f)(), s32 a0)
{
    f(a0);
}







/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02002924(void (*f)(), s32 a0)
{
    f(a0);
}

/*
 * Inner sanctum scene state: clear the backdrop colour, light the statue
 * groups whose progress flags are set, and step an actor. Overlay
 * resource_37a.
 */

#include "types.h"

#define ClearSolShindenBackdrop Func_0200022c
#define SetStatueLightGroup1 Func_02000238
#define SetStatueLightGroup2 Func_020002cc
#define SetStatueLightGroup3 Func_02000360
#define SetStatueLightGroup4 Func_020003f4
#define SetSolShindenActorStep Func_020025fc
/* Declared old-style: this parameter list is not established here. */
extern void Func_02005068();

s32 Func_02002bf4_scene_state(s32);
void Func_02002bec(s32, s32, s32, s32, s32, s32);
s32 Func_02002c12(s32);
void Func_02002c0a(s32, s32, s32, s32, s32, s32);
s32 Func_02002c30(s32);
void Func_02002c28(s32, s32, s32, s32, s32, s32);
s32 Func_02002c4e(s32);
void Func_02002c46(s32, s32, s32, s32, s32, s32);
s32 Func_02002c88(s32);
void Func_02002c80(s32, s32, s32, s32, s32, s32);
s32 Func_02002ca6(s32);
void Func_02002c9e(s32, s32, s32, s32, s32, s32);
s32 Func_02002cc4(s32);
void Func_02002cbc(s32, s32, s32, s32, s32, s32);
s32 Func_02002ce2(s32);
void Func_02002cda(s32, s32, s32, s32, s32, s32);
s32 Func_02002d1c(s32);
void Func_02002d14(s32, s32, s32, s32, s32, s32);
s32 Func_02002d3a(s32);
void Func_02002d32(s32, s32, s32, s32, s32, s32);
s32 Func_02002d58(s32);
void Func_02002d50(s32, s32, s32, s32, s32, s32);
s32 Func_02002d76(s32);
void Func_02002d6e(s32, s32, s32, s32, s32, s32);
s32 Func_02002db0(s32);
void Func_02002da8(s32, s32, s32, s32, s32, s32);
s32 Func_02002dce(s32);
void Func_02002dc6(s32, s32, s32, s32, s32, s32);
s32 Func_02002dec(s32);
void Func_02002de4(s32, s32, s32, s32, s32, s32);
s32 Func_02002e0a(s32);
void Func_02002e02(s32, s32, s32, s32, s32, s32);
/* Declared old-style: this parameter list is not established here. */
void Func_02004fd6();











/*
 * Passes the step to one routine with a zero and the wait to another. Both
 * arguments are read before either incoming register is overwritten, so the
 * order of these two calls is what the reference requires. The zero is
 * transcribed, not interpreted.
 */

#include "types.h"

#define UpdateStatueTrapActor Func_02000c00
#define UpdateStatueLight1 Func_02000d9c
#define UpdateStatueLight2 Func_02000ef8
#define UpdateStatueLight3 Func_0200101c
#define UpdateStatueLight4 Func_02001140
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

extern u8 Value_00001000;
extern u16 Data_0200ade4;
/* 手番カウンタ。他のオーバーレイからも書き換わるため volatile。
 * The reference reloads this cell on the path where the compiler can prove the
 * value is unchanged, which in ordinary C only a volatile object produces. */
extern volatile s32 Data_0200ade8;
extern u16 Data_0200addc;
extern u16 Data_0200ade0;
extern u16 Data_0200adec;

EntA *Func_020035f2(s32);
s32 Func_020035c2(s32);
s32 Func_020035ce(s32);
void Func_02000d2a(void);
s32 Func_020035de(s32);
s32 Func_02003608_a();
s32 Func_02003608_b();
void Func_02003660(s32, s32);
void Func_02003696(s32);
s32 Func_020035fe(s32);
EntA *Func_0200364a(s32);
void Func_02003688(s32, s32, s32);
s32 Func_0200363e_a();
s32 Func_0200363e_b();
void Func_02003674(s32, s32, s32);
void Func_020036c2(s32, s32, s32);
void Func_02003678(s32);
void Func_020036ae(s32, s32, s32);
s32 Func_02003676(s32);
void Func_020036e8(s32, s32, s32);
void Func_02003698(s32);
void Func_020036fc(s32, s32, s32);
void Func_02003770(s32, s32, s32);
void Func_0200377c_a();
void Func_0200377c_b();
void Func_0200377e(s32, s32, s32);
void Func_02003746(s32, s32);
s32 Func_020036d6(s32);
s32 Func_020036e0(s32);
void Func_0200375c(s32, s32);
EntB *Func_0200372a(s32);
void Func_0200374c(s32, s32, s32);
void Func_0200376a(s32);
void Func_02003778(s32, s32, s32);
void Func_02003786(s32, s32, s32);
void Func_0200375a(void);
s32 Func_02003726(void);
void Func_020038ca(s32);
void Func_0200377e_a(s32, s32, s32, s32, s32, s32);
void Func_02003790(s32, s32, s32, s32, s32, s32);
void Func_02003808(s32, s32, s32, s32, s32, s32);
void Func_020037b0(s32, s32, s32, s32, s32, s32);
void Func_020037d0(s32, s32, s32, s32, s32, s32);
void Func_020037f6(s32, s32, s32, s32, s32, s32);
void Func_0200381e(s32, s32, s32, s32, s32, s32);
s32 Func_0200381a(void);
void Func_02003870(s32, s32, s32);
void Func_02003890(s32, s32, s32);
s32 Func_02003882(void);
void Func_02003a26(s32);
void Func_020038da(s32, s32, s32, s32, s32, s32);
void Func_020038ec(s32, s32, s32, s32, s32, s32);
void Func_02003964(s32, s32, s32, s32, s32, s32);
void Func_0200390c(s32, s32, s32, s32, s32, s32);
void Func_0200392c(s32, s32, s32, s32, s32, s32);
void Func_02003952(s32, s32, s32, s32, s32, s32);
void Func_0200397a(s32, s32, s32, s32, s32, s32);
s32 Func_02003976(void);
s32 Func_020039a6(void);
void Func_02003b4a(s32);
void Func_020039fe(s32, s32, s32, s32, s32, s32);
void Func_02003a10(s32, s32, s32, s32, s32, s32);
void Func_02003a88(s32, s32, s32, s32, s32, s32);
void Func_02003a30(s32, s32, s32, s32, s32, s32);
void Func_02003a50(s32, s32, s32, s32, s32, s32);
void Func_02003a76(s32, s32, s32, s32, s32, s32);
void Func_02003a9e(s32, s32, s32, s32, s32, s32);
s32 Func_02003a9a(void);
s32 Func_02003aca(void);
void Func_02003c6e(s32);
void Func_02003b22(s32, s32, s32, s32, s32, s32);
void Func_02003b34(s32, s32, s32, s32, s32, s32);
void Func_02003bac(s32, s32, s32, s32, s32, s32);
void Func_02003b54(s32, s32, s32, s32, s32, s32);
void Func_02003b74(s32, s32, s32, s32, s32, s32);
void Func_02003b9a(s32, s32, s32, s32, s32, s32);
void Func_02003bc2(s32, s32, s32, s32, s32, s32);
s32 Func_02003bbe(void);

#include "types.h"

#define FieldScene_PrepareStatueTransition Func_02001264
#define FieldScene_RunClosingSequence Func_02001510
#define FieldScene_RunFlaggedSequence Func_02001790
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];

void Func_02003c20();
void Func_02003c32();
void Func_02003c42();
void Func_02003c54();
void Func_02003c5c();
void Func_02003c60();
void Func_02003cc0();
void Func_02003cd0();
void Func_02003cd6();
void Func_02003cfc();
void Func_02003d02();
void Func_02003d06();
void Func_02003d14();
void Func_02003d1c();
void Func_02003d6c();
void Func_02003db4();
void Func_02003dca();
void Func_02003e00_a();
void Func_02003e00_b();
s32 Func_02003e14_a();
s32 Func_02003e14_b();
void Func_02003e16();
void Func_02003e2a();
void Func_02003e30();
void Func_0200416a();
s32 Func_02004150();
s32 Func_0200415a();
void Func_02004174();
void Func_02004158();
s32 Func_02004180();
s32 Func_0200418a();
void Func_0200419c();
s32 Func_0200419a();
s32 Func_020041a4();
void Func_020041be();
void Func_020041a2();
s32 Func_020041ca();
s32 Func_020041d4();
void Func_020041e6();
s32 Func_020041e4();
s32 Func_020041ee();
void Func_02004208();
void Func_020041ec();
s32 Func_02004214();
s32 Func_0200421e();
void Func_02004230();
s32 Func_0200422e();
s32 Func_02004238();
void Func_02004252();
void Func_02004236();
s32 Func_0200425e();
s32 Func_02004268();
void Func_0200427a();
void Func_02002b26();
void Func_02004342();
void Func_0200430c();
void Func_02004384();
void Func_02004390();
void Func_020042be();
void Func_020043ee();
void Func_02001bca();
void Func_020042d0();
void Func_020043fe();
void Func_02001d04();
void Func_020042e2();
void Func_02004416();
void Func_02001bf2();
void Func_020042f8();
void Func_02004426();
void Func_02001d2c();
void Func_0200430a();
void Func_0200443e();
void Func_02001c1a();
void Func_02004320();
void Func_0200444e();
void Func_02001d54();
void Func_02004332();
void Func_02004466();
void Func_02001c42();
void Func_02004348();
void Func_02004476();
void Func_02001d7c();
void Func_0200435a();
void Func_0200448e();
void Func_02001c6a();
void Func_02004370();
void Func_0200449e();
void Func_02001da4();
void Func_02004382();
void Func_020044b4();
void Func_02001c90();
void Func_02004396();
s32 Func_02004384_a();
void Func_02004430();
void Func_02003fe0();
void Func_02004418();
void Func_02003ff0();
void Func_020044ea();
void Func_020044f6_field_scene();
void Func_020044c4();
void Func_02003eea();
void Func_0200277e();
void Func_02003f86();
void Func_02003fb2();
void Func_02003fc6();
void Func_02003f88();
void Func_0200401c();
void Func_02004028();
void Func_02003f56();
void Func_02003b8e();
void Func_0200408e();
void Func_020017d6();
void Func_02003f70();
void Func_0200409e();
void Func_02001910();
void Func_02003f82();
void Func_02003bbe_field_scene();
u32 Func_02003f4c();
u32 Func_02003f5e();
u32 Func_02003f70_a();
u32 Func_02003f82_a();
s32 Func_02003f8e();
s32 Func_02003f98();
s32 Func_02003fa2();
s32 Func_02003fac();
void Func_02004134();
void Func_0200187c();
void Func_02004016();
void Func_02004144();
void Func_020019b6();
void Func_02004028_a();
void Func_0200415c();
void Func_020018a4();
void Func_0200403e();
void Func_0200416c();
void Func_020019de();
void Func_02004050();
void Func_02004184();
void Func_020018cc();
void Func_02004066();
void Func_02004194();
void Func_02001a06();
void Func_02004078();
void Func_020041ac();
void Func_020018f4();
void Func_0200408e_a();
void Func_020041bc();
void Func_02001a2e();
void Func_020040a0();
void Func_02004080();
void Func_0200412a();
void Func_02003cfa();
void Func_02004100();
void Func_02004126();
void Func_0200409c();
void Func_020040a2();
void Func_020040a8();
void Func_020040ae();
void Func_02004228();
void Func_02004234();
void Func_02004202();
void UpdateStatueLight1(void);
void UpdateStatueLight2(void);
void UpdateStatueLight3(void);
void UpdateStatueLight4(void);

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */










static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */

#include "types.h"

#define StartSolShindenTrapEvent Func_02001380
#define CheckAllStatueLights Func_020025b0

void Func_02001616();
void Func_0200163e();
void Func_02001666();
void Func_0200168e();
void Func_020016b6();
void Func_020016d6();
void Func_02001750();
void Func_02001778();
void Func_020017a0();
void Func_020017c8();
void Func_020017f0();
void Func_020025ec();
void Func_020039ce();
void Func_02003aa8();
void Func_02003d58();
void Func_02003d96();
void Func_02003db0();
void Func_02003dc2();
void Func_02003dd8();
void Func_02003de2();
void Func_02003de4();
void Func_02003dea();
void Func_02003e00();
void Func_02003e0e(s32, s32, s32);
void Func_02003e12();
void Func_02003e22(s32, s32, s32);
void Func_02003e28();
void Func_02003e3a();
void Func_02003e50();
void Func_02003e5c();
void Func_02003e62();
void Func_02003e68();
void Func_02003e70();
void Func_02003ea8();
void Func_02003eae(s32, s32, s32);
void Func_02003ece();
void Func_02003ed4();
void Func_02003ede();
void Func_02003ef6();
void Func_02003f06();
void Func_02003f1e();
void Func_02003f2e();
void Func_02003f46();
void Func_02003f56_sol_shinden();
void Func_02003f6e();
void Func_02003f7e();
void Func_02003f9e();
void Func_02003fbe();
void Func_02003fca();
/* One import, four call sites, four distinct veneer addresses. */
s32 Func_02004f6c();
s32 Func_02004f78();
s32 Func_02004f84();
s32 Func_02004f90();

static __inline__ void SetInitialScale(s32 actor, s32 scale, s32 duration)
{
    Func_02003e0e(actor, scale, duration);
}

static __inline__ void SetInitialDirection(s32 actor, s32 direction, s32 duration)
{
    Func_02003e22(actor, direction, duration);
}

static __inline__ void SetFinalScale(s32 actor, s32 horizontal, s32 vertical)
{
    Func_02003eae(actor, horizontal, vertical);
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

    if (Value0(Func_02002608)!= 0) {
        record = Value1(Func_02002a16, 0x201);
        if (record != 0) {
            goto L_020000f0;
        }
        Func_02002a3e();
        Call2(Func_02002b2e, 0x2051cc, 1);
        Func_02002b44(20);
        Call1(Func_02002a3a, 0x201);
        Call1(Func_02002a4a, 0x200);
        Call1(Func_02002a50, 0x202);
        if (Value1(Func_02002a46, 0x80a) == 0) {
            Func_02000a8e();
        }
        if (Value0(Func_0200264e)!= 0) {
            if (Value1(Func_02002a5c, 0x811) == 0) {
                Func_02001c98();
            }
        }
        Func_02002a90();
    } else {
        if (Value1(Func_02002a72, 0x200) == 0) {
            Func_02002a9a_a();
            Call2(Func_02002b8c, 0x10000, 1);
            Func_02002ba2(20);
            Call1(Func_02002a9a_b, 0x200);
            Call1(Func_02002aa8, 0x201);
            Call1(Func_02002aae, 0x202);
            Func_02002aca();
        }
    }
    L_020000f0:;
}

void FieldScene_RunScene37aSequenceB(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_02002ac4, 0x200) == 0) {
        Func_02002aec_a();
        Call2(Func_02002bde, 0x10000, 1);
        Func_02002bf4(20);
        Call1_02000108(Func_02002aec_b, 0x200);
        Call1_02000108(Func_02002afa, 0x201);
        Call1_02000108(Func_02002b00, 0x202);
        Func_02002b1c();
    }
}

void FieldScene_RunScene37aSequenceC(void)
{
    u32 i;
    s32 record;

    if (Value0(Func_02002704)!= 0) {
        record = Value1(Func_02002b14, 0x200);
        if (record != 0) {
            goto L_020001d6;
        }
        Func_02002b3c_a();
        Call2(Func_02002c2e, 0x10000, 1);
        Func_02002c44(20);
        Call1(Func_02002b3c_b, 0x200);
        Call1(Func_02002b4a, 0x201);
        Call1(Func_02002b50, 0x202);
        Func_02002b6c();
    } else {
        if (Value1(Func_02002b4c, 0x201) == 0) {
            Func_02002b74();
            Call2(Func_02002c64, 0x2051cc, 1);
            Func_02002c7a(20);
            Call1(Func_02002b70, 0x201);
            Call1(Func_02002b80, 0x200);
            Call1(Func_02002b86, 0x202);
            if (Value1(Func_02002b7c, 0x80a) == 0) {
                Func_02000bc4();
            }
            Func_02002bb0();
        }
    }
    L_020001d6:;
}

void FieldScene_RunScene37aSequenceD(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_02002ba6, 0x202) == 0) {
        Call2(Func_02002cba, 0x202db1, 1);
        Func_02002cd0(20);
        Call1_020001ec(Func_02002bc6, 0x202);
        Call1_020001ec(Func_02002bd6, 0x200);
        Call1_020001ec(Func_02002bdc, 0x201);
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
    if (Func_02002bf4_scene_state(0x80b) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002bec(0x2d, 28, 0x22, 10, a, b);
    }
    if (Func_02002c12(0x80c) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002c0a(0x2f, 28, 0x24, 10, a, b);
    }
    if (Func_02002c30(0x80d) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002c28(0x2d, 29, 0x22, 11, a, b);
    }
    if (Func_02002c4e(0x80e) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002c46(0x2f, 29, 0x24, 11, a, b);
    }
}

void SetStatueLightGroup2(void)
{
    if (Func_02002c88(0x826) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002c80(0x2d, 28, 0x22, 10, a, b);
    }
    if (Func_02002ca6(0x827) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002c9e(0x2f, 28, 0x24, 10, a, b);
    }
    if (Func_02002cc4(0x828) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002cbc(0x2d, 29, 0x22, 11, a, b);
    }
    if (Func_02002ce2(0x829) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002cda(0x2f, 29, 0x24, 11, a, b);
    }
}

void SetStatueLightGroup3(void)
{
    if (Func_02002d1c(0x80b) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002d14(0x2d, 30, 0x22, 10, a, b);
    }
    if (Func_02002d3a(0x80c) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002d32(0x2f, 30, 0x24, 10, a, b);
    }
    if (Func_02002d58(0x80d) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002d50(0x2d, 31, 0x22, 11, a, b);
    }
    if (Func_02002d76(0x80e) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002d6e(0x2f, 31, 0x24, 11, a, b);
    }
}

void SetStatueLightGroup4(void)
{
    if (Func_02002db0(0x826) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002da8(0x2d, 30, 0x22, 10, a, b);
    }
    if (Func_02002dce(0x827) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002dc6(0x2f, 30, 0x24, 10, a, b);
    }
    if (Func_02002dec(0x828) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002de4(0x2d, 31, 0x22, 11, a, b);
    }
    if (Func_02002e0a(0x829) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002e02(0x2f, 31, 0x24, 11, a, b);
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
        Call4(Func_0200306c_sequence, 0x1200000, -1, 0xd50000, 1);
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
            Func_02003352(16, FIELD(rec, s16 *, 10), FIELD(rec, s16 *, 18));
        }
        Func_02003370(16);
        Func_02002f1c(16, 0, 0);
        Func_02002ef8(1, 2);
        rec = (u8 *)Value1(Func_02002ede, 0);
        if (rec != 0) {
            Func_02003352(1, FIELD(rec, s16 *, 10), FIELD(rec, s16 *, 18));
        }
        Func_02003370(1);
        Func_02002f1c(1, 0, 0);
        Func_02002ef8(5, 2);
        rec = (u8 *)Value1(Func_02002ede, 0);
        if (rec != 0) {
            Func_02003352(5, FIELD(rec, s16 *, 10), FIELD(rec, s16 *, 18));
        }
        Func_02003370(5);
        Func_02002f1c(5, 0, 0);
        Call1(Func_0200338a, 0x144);
        Call1(Func_0200338a, 0x809);
        Func_020033b4();
    }
}

void FieldScene_RunScene37a_020009f4(void)
{
    u32 i;
    s32 record;
    s32 base6_ffc;
    s32 base5_4010;
    s32 base5_4010_2;

    Call1_020009f4(Func_0200344e, 0xff6);
    Call3(Func_020033fc, 0, 0x10000, 0x8000);
    Call3(Func_02003420, 0, 0x1e8, 176);
    Func_02003440(0, 0);
    record = Value1(Func_0200340e, 0);
    if (record != 0) {
        Func_0200344c(16, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Func_020034ae(0, 0, 1);
    Call3(Func_0200343c, 16, 0x10000, 0x8000);
    Call3(Func_02003460, 16, 0x1d8, 168);
    Func_020034d2(16, 0, 60);
    Func_0200349c(16, 4, 40);
    Func_0200306c(16, 6);
    Call2(Func_0200350c, 0x26666, 0x4ccc);
    Call4(Func_02003524, 0x23f0000, -1, 0xb50000, 1);
    Func_02003530();
    Func_0200345e(120);
    Call2(Func_02003096, 0x1010, 80);
    Call4(Func_02003548, 0x1ec0000, -1, 0xa80000, 1);
    Func_02003554();
    Func_02003482(20);
    base5_4010 = (s32)Data_00004010;
    Call3(Func_02003538, 16, 0x3000, 20);
    Func_020030c8(base5_4010, 6);
    Func_0200354a(16, 0, 60);
    Func_0200351a(16, 2);
    Call3(Func_0200355e, 16, 0x3000, 10);
    Value2(Func_0200354e_a, base5_4010, 0);
    if (Value2(Func_020034de, 0, 0) == 0) {
        Call1_020009f4(Func_02003558, 0xffa);
    } else {
        Call1_020009f4(Func_02003560, 0xffb);
    }
    base5_4010_2 = (s32)Data_00004010;
    Call3(Func_0200358e, 0, 0xa000, 10);
    Func_0200311e(base5_4010_2, 10);
    base6_ffc = (s32)Data_00000ffc;
    Func_0200357e_a(base6_ffc);
    Func_020035a8(16, 0, 40);
    Call3(Func_020035ba, 16, 0x105, 40);
    Func_02003572(16, 4);
    Call3(Func_020035c6, 16, 0x3000, 10);
    Func_0200357e_b(16, 4);
    Value2(Func_020035be, base5_4010_2, 0);
    if (Value2(Func_0200354e_b, 0, 0) == 0) {
        Func_020035c8((base6_ffc + 1));
        Call1_020009f4(Func_02003536, 0x896);
    } else {
        Func_020035d6((base6_ffc + 2));
    }
    Call2(Func_02003186, (s32)Data_00004010, 4);
    Func_0200361e(16, 1);
    Call3(Func_020035aa, 16, 0x1e6, 131);
    Call3(Func_020035b6, 16, 0x240, 120);
    Call3(Func_0200362a, 16, 0xc000, 2);
    Call2(Func_02003656, 0x40000, 0x8000);
    Call1_020009f4(Func_02003584, 0x80a);
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
    if (Func_020035c2(0x809) == 0) {
        return;
    }
    if (Func_020035ce(0x814) != 0) {
        Func_02000d2a();
        return;
    }
    if (Func_020035de(0x819) != 0) {
        return;
    }
    Func_02003608_a();
    Func_02003660(0, 0);
    Func_02003696((s32)&Value_00001000);
    if (Func_020035fe(g1)!= 0 || Func_02003608_b(0x80a) == 0) {
        target_actor = Func_0200364a(0);
        if (target_actor != 0) {
            Func_02003688(16, target_actor->unk8, target_actor->unk10);
        }
        Func_0200363e_a(4);
        Func_02003674(16, s1, s2);
    } else {
        if (Func_0200363e_b(g2)!= 0) goto do1;
        h1 = 0x1540000;
        if (scene_actor->unk8 > h1) {
do1:
            Func_020036c2(16, 0x1880000, 0xa80000);
            Func_02003678(4);
            Func_020036ae(16, s3, s4);
        }
    }
    if (Func_02003676(g3)!= 0) goto do2;
    h2 = 0x1540000;
    if (scene_actor->unk8 > h2) {
do2:
        Func_020036e8(16, d1, 0xe8);
    } else {
        Func_02003698(0x80a);
    }
    Func_020036fc(16, d2, 0xe8);
    Func_02003770(0, d5, 0);
    Func_0200377c_a(16, s5, 10);
    Func_0200377e(16, 0, 10);
    Func_02003746(0, 3);
    if (Func_020036d6(g4)!= 0 || Func_020036e0(0x80a) == 0) {
        Func_0200375c(16, 2);
        target_position = Func_0200372a(0);
        if (target_position != 0) {
            Func_0200374c(16, target_position->unkA, target_position->unk12);
        }
        Func_0200376a(16);
        Func_0200377c_b(16, 0, 0);
        Func_02003778(0, d3, 0xe8);
    } else {
        Func_02003786(0, d4, 0xf8);
    }
    Func_0200375a();
}

void UpdateStatueLight1(void)
{
    volatile s32 *st;
    s32 s;
    s32 t1 = 0x10000;
    s32 t2 = 0x10000;
    s32 t3 = 0x10000;
    s32 m1 = -1;
    s32 m2 = -1;
    s32 m3 = 0xe666;

    if ((Func_02003726() & 3) != 0) {
        u16 v = Data_0200ade4;
        switch (v) {
        case 0: {
            s32 a;
            s32 b;
            Func_020038ca(0xbb);
            a = 1;
            b = 5;
            Func_0200377e_a(0x2e, 0x3b, 30, 0x21, a, b);
            break;
        }
        case 1:
            Func_02003790(0x2e, 0x3b, 30, 0x21, v, v);
            Func_02003808(0x2e, 0x3b, 30, 0x22, v, 5);
            break;
        case 2:
            v = 1;
            Func_020037b0(0x2e, 0x3b, 30, 0x22, v, v);
            Func_02003808(0x2e, 0x3b, 30, 0x23, v, 5);
            break;
        case 3:
            v = 1;
            Func_020037d0(0x2e, 0x3b, 30, 0x23, v, v);
            Func_02003808(0x2e, 0x3b, 30, 0x24, v, 5);
            break;
        case 4: {
            s32 val = 2;
            volatile s32 *p = &Data_0200ade8;
            *p = val;
        }
            v = 1;
            Func_020037f6(0x2e, 0x3b, 30, 0x24, v, v);
            Func_02003808(0x2e, 0x3b, 30, 0x25, v, 5);
            break;
        case 0x50: {
            s32 a = 1;
            s32 b = 10;
            Func_0200381e(0x2e, 0x31, 30, 0x21, a, b);
            break;
        }
        }
        {
            u16 *c = &Data_0200ade4;
            u32 w;
            *c = *c + 1;
            w = *c;
            if (w > ((u32)(Func_0200381a() * 40) >> 16) + 90) {
                *c = 0;
            }
        }
    }
    st = &Data_0200ade8;
    s = *st;
    if (s != 0) {
        if (s == 2) {
            Func_02003870(t1, t2, t3);
        } else if (s == 1) {
            Func_02003890(m1, m2, m3);
        }
        *st = *st - 1;
    }
}

void UpdateStatueLight2(void)
{
    if ((Func_02003882() & 3) != 0) {
        u16 v = Data_0200addc;
        switch (v) {
        case 0: {
            s32 a;
            s32 b;
            Func_02003a26(0xbb);
            a = 1;
            b = 5;
            Func_020038da(0x2f, 0x3b, 42, 0x21, a, b);
            break;
        }
        case 1:
            Func_020038ec(0x2f, 0x3b, 42, 0x21, v, v);
            Func_02003964(0x2f, 0x3b, 42, 0x22, v, 5);
            break;
        case 2:
            v = 1;
            Func_0200390c(0x2f, 0x3b, 42, 0x22, v, v);
            Func_02003964(0x2f, 0x3b, 42, 0x23, v, 5);
            break;
        case 3:
            v = 1;
            Func_0200392c(0x2f, 0x3b, 42, 0x23, v, v);
            Func_02003964(0x2f, 0x3b, 42, 0x24, v, 5);
            break;
        case 4: {
            s32 val = 2;
            s32 *p = &Data_0200ade8;
            *p = val;
        }
            v = 1;
            Func_02003952(0x2f, 0x3b, 42, 0x24, v, v);
            Func_02003964(0x2f, 0x3b, 42, 0x25, v, 5);
            break;
        case 0x5a: {
            s32 a = 1;
            s32 b = 10;
            Func_0200397a(0x2f, 0x31, 42, 0x21, a, b);
            break;
        }
        }
        {
            u16 *c = &Data_0200addc;
            u32 w;
            *c = *c + 1;
            w = *c;
            if (w > ((u32)(Func_02003976() * 40) >> 16) + 100) {
                *c = 0;
            }
        }
    }
}

void UpdateStatueLight3(void)
{
    if ((Func_020039a6() & 3) != 0) {
        u16 v = Data_0200ade0;
        switch (v) {
        case 0: {
            s32 a;
            s32 b;
            Func_02003b4a(0xbb);
            a = 1;
            b = 5;
            Func_020039fe(0x30, 0x3b, 31, 0x24, a, b);
            break;
        }
        case 1:
            Func_02003a10(0x30, 0x3b, 31, 0x24, v, v);
            Func_02003a88(0x30, 0x3b, 31, 0x25, v, 5);
            break;
        case 2:
            v = 1;
            Func_02003a30(0x30, 0x3b, 31, 0x25, v, v);
            Func_02003a88(0x30, 0x3b, 31, 0x26, v, 5);
            break;
        case 3:
            v = 1;
            Func_02003a50(0x30, 0x3b, 31, 0x26, v, v);
            Func_02003a88(0x30, 0x3b, 31, 0x27, v, 5);
            break;
        case 4: {
            s32 val = 2;
            s32 *p = &Data_0200ade8;
            *p = val;
        }
            v = 1;
            Func_02003a76(0x30, 0x3b, 31, 0x27, v, v);
            Func_02003a88(0x30, 0x3b, 31, 0x28, v, 5);
            break;
        case 0x5f: {
            s32 a = 1;
            s32 b = 10;
            Func_02003a9e(0x30, 0x31, 31, 0x24, a, b);
            break;
        }
        }
        {
            u16 *c = &Data_0200ade0;
            u32 w;
            *c = *c + 1;
            w = *c;
            if (w > ((u32)(Func_02003a9a() * 40) >> 16) + 105) {
                *c = 0;
            }
        }
    }
}

void UpdateStatueLight4(void)
{
    if ((Func_02003aca() & 3) != 0) {
        u16 v = Data_0200adec;
        switch (v) {
        case 0: {
            s32 a;
            s32 b;
            Func_02003c6e(0xbb);
            a = 1;
            b = 5;
            Func_02003b22(0x2e, 0x3b, 41, 0x24, a, b);
            break;
        }
        case 1:
            Func_02003b34(0x2e, 0x3b, 41, 0x24, v, v);
            Func_02003bac(0x2e, 0x3b, 41, 0x25, v, 5);
            break;
        case 2:
            v = 1;
            Func_02003b54(0x2e, 0x3b, 41, 0x25, v, v);
            Func_02003bac(0x2e, 0x3b, 41, 0x26, v, 5);
            break;
        case 3:
            v = 1;
            Func_02003b74(0x2e, 0x3b, 41, 0x26, v, v);
            Func_02003bac(0x2e, 0x3b, 41, 0x27, v, 5);
            break;
        case 4: {
            s32 val = 2;
            s32 *p = &Data_0200ade8;
            *p = val;
        }
            v = 1;
            Func_02003b9a(0x2e, 0x3b, 41, 0x27, v, v);
            Func_02003bac(0x2e, 0x3b, 41, 0x28, v, 5);
            break;
        case 0x55: {
            s32 a = 1;
            s32 b = 10;
            Func_02003bc2(0x2e, 0x31, 41, 0x24, a, b);
            break;
        }
        }
        {
            u16 *c = &Data_0200adec;
            u32 w;
            *c = *c + 1;
            w = *c;
            if (w > ((u32)(Func_02003bbe() * 40) >> 16) + 95) {
                *c = 0;
            }
        }
    }
}

void FieldScene_PrepareStatueTransition(void)
{
    u32 i;
    s32 record;
    u8 *work;

    Call4(Func_02003d14, -1, -1, -1, 0);
    Call6(Func_02003c20, 30, 43, 32, 40, 8, 3);
    Call6(Func_02003c32, 30, 43, 33, 39, 8, 1);
    Call6(Func_02003c42, 30, 43, 36, 38, 3, 3);
    Call6(Func_02003c54, 14, 41, 32, 41, 8, 4);
    Call4(Func_02003d6c, 0x23e0000, -1, 0x9e0000, 0);
    Func_02003c60();
    Call3(Func_02003cfc, 16, 0x23e0000, 0x780000);
    Func_02003d06(0, 0, 0);
    Func_02003c5c(1);
    Call2(Func_02003db4, 0x2051cc, 1);
    Func_02003dca(20);
    Call1(Func_02003cc0, 0x201);
    Call1(Func_02003cd0, 0x200);
    Call1(Func_02003cd6, 0x202);
    work = *(u8 **)Data_03001ebc;
    *(s32 *)(((s32)work + 0x1c0)) = 0x100;
    *(s32 *)(((s32)work + 0x1c8)) = 32;
    Func_02003e00_a();
    ((void (*)())Func_02003e14_a)();
    Func_02003d02(40);
    Func_02003e30(171);
    Call2(Func_02003e00_b, 0x10005, 1);
    Func_02003e16(8);
    Func_02003d1c(32);
    Value2(Func_02003e14_b, 0x2051cc, 1);
    Func_02003e2a(24);
}

void StartSolShindenTrapEvent(void)
{
    u32 scene_state;
    s32 outer_pair;
    s32 second_pair;
    s32 middle_pair;
    s32 fourth_pair;
    s32 inner_pair;

    Func_02003d58();
    Func_020025ec();
    Func_02003de2(4120);
    SetInitialScale(16, 0x4000, 20);
    SetInitialDirection(16, 256, 0);
    Func_02003de4(16, 6, 30);
    Func_02003e5c(37617664, -1, 11403264, 1);
    Func_02003e68();
    Func_02003d96(30);
    Func_020039ce(32784, 20);
    for (outer_pair = 0; outer_pair != 4; outer_pair++) {
        Func_02003ece(246);
        Func_02001616();
        Func_02003db0(12);
        Func_02003ede(246);
        Func_02001750();
        Func_02003dc2(12);
    }
    for (second_pair = 0; second_pair != 6; second_pair++) {
        Func_02003ef6(246);
        Func_0200163e();
        Func_02003dd8(8);
        Func_02003f06(246);
        Func_02001778();
        Func_02003dea(8);
    }
    for (middle_pair = 0; middle_pair != 8; middle_pair++) {
        Func_02003f1e(246);
        Func_02001666();
        Func_02003e00(6);
        Func_02003f2e(246);
        Func_020017a0();
        Func_02003e12(6);
    }
    for (fourth_pair = 0; fourth_pair != 10; fourth_pair++) {
        Func_02003f46(246);
        Func_0200168e();
        Func_02003e28(4);
        Func_02003f56_sol_shinden(246);
        Func_020017c8();
        Func_02003e3a(4);
    }
    for (inner_pair = 0; inner_pair != 12; inner_pair++) {
        Func_02003f6e(246);
        Func_020016b6();
        Func_02003e50(2);
        Func_02003f7e(246);
        Func_020017f0();
        Func_02003e62(2);
    }
    Func_020016d6();
    Func_02003e70(6);
    Func_02003aa8(32784, 6);
    SetFinalScale(16, 0x20000, 0x10000);
    Func_02003ed4(16, 576, 280);
    scene_state = *(u32 *)Data_03001ebc;
    *(s32 *)(scene_state + 448) = 256;
    *(s32 *)(scene_state + 456) = 32;
    Func_02003fbe();
    Func_02003fca();
    Func_02003ea8(2067);
    Func_02003f9e(3);
}

void FieldScene_RunClosingSequence(void)
{
    s32 i;
    u8 *work;
    Func_02003eea();
    Func_0200277e();
    Data_0200ade4 = 0;
    Data_0200addc = 0;
    Data_0200ade0 = 0;
    Data_0200adec = 0;
    Func_02003f86(4097);
    Call3(Func_02003fb2, 16, 16384, 20);
    Call3(Func_02003fc6, 16, 256, 0);
    Func_02003f88(16, 6, 30);
    Func_0200401c(37617664, -1, 11403264, 1);
    Func_02004028();
    Func_02003f56(30);
    Func_02003b8e(32784, 20);
    for (i = 0; i != 4; i++) {
        Func_0200408e(246);
        Func_020017d6();
        Func_02003f70(12);
        Func_0200409e(246);
        Func_02001910();
        Func_02003f82(12);
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
        Func_02004134(246);
        Func_0200187c();
        Func_02004016(5);
        Func_02004144(246);
        Func_020019b6();
        Func_02004028_a(5);
    }
    for (i = 0; i != 8; i++) {
        Func_0200415c(246);
        Func_020018a4();
        Func_0200403e(4);
        Func_0200416c(246);
        Func_020019de();
        Func_02004050(4);
    }
    for (i = 0; i != 10; i++) {
        Func_02004184(246);
        Func_020018cc();
        Func_02004066(3);
        Func_02004194(246);
        Func_02001a06();
        Func_02004078(3);
    }
    for (i = 0; i != 12; i++) {
        Func_020041ac(246);
        Func_020018f4();
        Func_0200408e_a(2);
        Func_020041bc(246);
        Func_02001a2e();
        Func_020040a0(2);
    }
    Call6(Func_02004080, 45, 30, 34, 10, 4, 2);
    Func_0200412a(16, 6, 40);
    Func_02003cfa(32784, 6);
    Call3(Func_02004100, 16, 131072, 65536);
    Func_02004126(16, 576, 280);
    Func_0200409c((s32)UpdateStatueLight1);
    Func_020040a2((s32)UpdateStatueLight2);
    Func_020040a8((s32)UpdateStatueLight3);
    Func_020040ae((s32)UpdateStatueLight4);
    work = *(u8 **)Data_03001ebc;
    *(s32 *)(work + 0x1c0) = 0x100;
    *(s32 *)(work + 0x1c8) = 32;
    Func_02004228();
    Func_02004234();
    Func_02004202(4);
}

void FieldScene_RunFlaggedSequence(void)
{
    s32 base;
    u8 *work;
    s32 i6;
    s32 i7;
    s32 i8;
    s32 i9;
    s32 i10;

    Func_0200416a();
    if (Value1(Func_02004150, 2059) != 0 && Value1(Func_0200415a, 2086) != 0) {
        Call1(Func_02004174, 2086);
        Call6(Func_02004158, 45, 28, 34, 10, 2, 1);
    } else if (Value1(Func_02004180, 2059) != 0 && Value1(Func_0200418a, 2086) == 0) {
        Call1(Func_0200419c, 2086);
    }
    if (Value1(Func_0200419a, 2060) != 0 && Value1(Func_020041a4, 2087) != 0) {
        Call1(Func_020041be, 2087);
        Call6(Func_020041a2, 47, 28, 36, 10, 2, 1);
    } else if (Value1(Func_020041ca, 2060) != 0 && Value1(Func_020041d4, 2087) == 0) {
        Call1(Func_020041e6, 2087);
    }
    if (Value1(Func_020041e4, 2061) != 0 && Value1(Func_020041ee, 2088) != 0) {
        Call1(Func_02004208, 2088);
        Call6(Func_020041ec, 45, 29, 34, 11, 2, 1);
    } else if (Value1(Func_02004214, 2061) != 0 && Value1(Func_0200421e, 2088) == 0) {
        Call1(Func_02004230, 2088);
    }
    if (Value1(Func_0200422e, 2062) != 0 && Value1(Func_02004238, 2089) != 0) {
        Call1(Func_02004252, 2089);
        Call6(Func_02004236, 47, 29, 36, 11, 2, 1);
    } else if (Value1(Func_0200425e, 2062) != 0 && Value1(Func_02004268, 2089) == 0) {
        Call1(Func_0200427a, 2089);
    }
    Func_02002b26();
    Call3(Func_02004342, 16, 16384, 20);
    Func_0200430c(16, 6, 30);
    Func_02004384(37617664, -1, 11403264, 1);
    Func_02004390();
    Func_020042be(30);
    for (i6 = 0; i6 != 4; i6++) {
        Func_020043ee(246);
        Func_02001bca();
        Func_020042d0(12);
        Func_020043fe(246);
        Func_02001d04();
        Func_020042e2(12);
    }
    for (i7 = 0; i7 != 6; i7++) {
        Func_02004416(246);
        Func_02001bf2();
        Func_020042f8(8);
        Func_02004426(246);
        Func_02001d2c();
        Func_0200430a(8);
    }
    for (i8 = 0; i8 != 8; i8++) {
        Func_0200443e(246);
        Func_02001c1a();
        Func_02004320(6);
        Func_0200444e(246);
        Func_02001d54();
        Func_02004332(6);
    }
    for (i9 = 0; i9 != 10; i9++) {
        Func_02004466(246);
        Func_02001c42();
        Func_02004348(4);
        Func_02004476(246);
        Func_02001d7c();
        Func_0200435a(4);
    }
    for (i10 = 0; i10 != 12; i10++) {
        Func_0200448e(246);
        Func_02001c6a();
        Func_02004370(2);
        Func_0200449e(246);
        Func_02001da4();
        Func_02004382(2);
    }
    Func_020044b4(246);
    Func_02001c90();
    Func_02004396(6);
    if (Func_02004384_a(2082) == 0) {
        base = 32784;
        Func_02004430(4133);
        Func_02003fe0(base, 6);
        Func_02004418(16, 3);
        Func_02003ff0(base, 6);
    }
    work = *(u8 **)Data_03001ebc;
    *(s32 *)(work + 0x1c0) = 0x100;
    *(s32 *)(work + 0x1c8) = 32;
    Func_020044ea();
    Func_020044f6_field_scene();
    Func_020044c4(5);
}

void FieldScene_RunScene37aSequenceE(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_02004414, 0x810) != 0) {
    } else {
        if (Value0(Func_0200401a) == 0) {
        } else {
            Func_02004448();
            Call3(Func_0200449c, 16, 0x2410000, 0x930000);
            Call3(Func_02004500, 16, 0x4000, 1);
            Call4(Func_02004538, 0x23e0000, -1, 0xb80000, 1);
            Call1(Func_020044f6, 0x1027);
            Call3(Func_020044ba, 0, 0x240, 232);
            Func_020044da(0, 0);
            Func_0200455e();
            Func_0200448c(10);
            Call3(Func_020044c2, 16, 0x10000, 0x8000);
            Call3(Func_020044e6, 16, 0x240, 152);
            Func_020044ac(6);
            Func_0200451e(16, 6, 30);
            Func_020040ee(16, 6);
            Func_02004526(0, 3);
            Func_020044cc(2);
            Func_02004534(16, 4);
            Func_0200410c(16, 6);
            Call2(Func_0200459e, 0, 0x102);
            Func_020044ec(40);
            Func_02004564(16, 2);
            Func_020044fa(30);
            Func_02004132(16, 6);
            Func_0200456a(0, 3);
            Call3(Func_02004556, 16, 0x240, 184);
            Func_0200451c(6);
            Func_02004594(16, 2);
            Func_0200452a(40);
            Call2(Func_02004162, 0x4010, 6);
            Call3(Func_0200457e, 16, 0x240, 208);
            Func_02004544(40);
            Func_020045ac_a(0, 3);
            Func_02004552(6);
            Call3(Func_02004588_a, 16, 0x8000, 0x4000);
            Func_020045c0(16, 2);
            record = Value1(Func_0200458e, 0);
            if (record != 0) {
                Func_020045b0(16, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Func_020045ce(16);
            Func_020045e0(16, 0, 0);
            Call1(Func_02004588_b, 0x810);
            Func_020045ac_b();
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
    Call1(Func_02004698_a, (s32)SCENE_STEP_VALUE);
    Func_02004682(16, 4, 30);
    Func_02004252_actor_step(16, 6);
    Func_0200469a(0, 2);
    Func_02004630(6);
    Func_02004698_b(16, 3);
    Func_02004270(16, 6);
    Call3(Func_0200468c, 16, 0x178, 184);
    Call3(Func_020046a8, 16, 0x6480000, 0x6480000);
    Func_0200465e(4);
    Call1(Func_02004654, 0x811);
}

s32 CheckAllStatueLights(void)
{
    s32 all_set = 1;

    if (Func_02004f6c(0x80b) == 0)
        all_set = 0;
    if (Func_02004f78(0x80c) == 0)
        all_set = 0;
    if (Func_02004f84(0x80d) == 0)
        all_set = 0;
    if (Func_02004f90(0x80e) == 0)
        all_set = 0;

    return all_set;
}

void SetSolShindenActorStep(s32 actor_step, s32 wait_frames)
{
    Func_02005068(actor_step, 0);
    Func_02004fd6(wait_frames);
}

void FieldScene_RunScene37a_02002924(void)
{
    u32 i;
    s32 record;

    Func_020052fc();
    if (Value1(Func_020052e2, 0x896) != 0) {
        Call1_02002924(Func_0200538c, 0xffd);
    } else {
        Call1_02002924(Func_02005394, 0xfff);
    }
    Func_020053b6(16, 0, 10);
    Call3(Func_020053ca, 16, 0xc000, 10);
    Func_02005336();
}
