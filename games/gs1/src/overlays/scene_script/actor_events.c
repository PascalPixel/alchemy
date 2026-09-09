#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define Scene_RunEventTransition Func_02000924
#define Scene_RunActorCue Func_02000ac0
#define Scene_RunActorExchange Func_02000b2c
#define Scene_RunActorSequence Func_02000c30
#define EventScript_PrepareActorRenderFlags Func_02000030
#define OverlayObject_SetFacingTowardObject10 Func_02000054
#define SceneData_SelectTable97b4ByState Func_0200007c
#define SceneData_ReturnZero Func_020000ac
#define SceneData_GetTable98ec Func_020000b0
#define SceneData_SelectTable9918ByState Func_020000b8
#define SceneDialogue_RunMessage1958Step Func_020000e8
#define SceneDialogue_RunActor11Message195d Func_02000140
#define SceneDialogue_RunActor13Message1961 Func_02000160
#define FieldScene_RunPrimaryScript Func_02000180
#define FieldScene_RunScene3a2SequenceA Func_020001ec
#define SceneDialogue_RunLine1956 Func_0200084c
#define ConfigureAndPlaceActorFourteen Func_02000870
#define FieldScene_RunScene3a2_020008a8 Func_020008a8
#define SceneDialogue_RunActorFifteenByLeaderHeading Func_020008e0
#define FieldScene_RunScriptedSteps0And1A12 Func_02000fcc
#define FieldScene_RunPairedLayoutStepsThenSetOne Func_02000ff0
#define SceneState_RunRect6x28Step Func_02001028
#define SceneActor_SetFlagBitByRankAgainstActorZero Func_02001060
#define SceneActor_UpdateActorFourteenByDepth Func_02001090
#define ActorPresentation_PrepareActorFourteenWithCallback Func_020010b8
#define FieldScene_SetSlot15Byte89AndRunStep Func_02001144
#define SceneData_SelectTableByWord224 Func_02001180

struct EventActor {
    u8 reserved_00[0x23];
    u8 flags;
    u8 reserved_24[0x2c];
    u8 *render_state;
};

struct Slot020008e0 {
    u8 head[6];
    u16 heading;
};

struct Actor02001060 {
    u8 head[12];
    s32 rank;
    u8 body[19];
    u8 flags;
};

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern s16 Data_02000240[];
extern u8 Value_0000004a;
extern u8 Data_02009844[];
extern u8 Data_020097b4[];
extern u8 Data_02009a38[];
extern u8 Data_02009918[];
extern u8 Data_02009c9c[];
extern u8 Data_02009b10[];

s16 Func_0200150e(s32, s32);
void *Func_02001564(s32);
void Func_020015dc(void);
void Func_02001672(s32);
s32 Func_02001682(s32, s32);
s32 Func_02001602(s32, s32);
void Func_020015f4(s32);
void Func_020016a4(s32, s32);
void Func_020016ce(s32, s32);
void Func_0200162a(void);
void Func_02001634(void);
void Func_020016ca(s32);
s32 Func_020016f2(s32, s32);
void Func_0200164e(void);
void Func_02001654(void);
void Func_020016ea(s32);
s32 Func_02001712(s32, s32);
void Func_0200166e(void);
void Func_02001638();
s32 Func_0200169e();
void Func_020016b6();
void Func_020016b6_a();
void Func_02001708();
void Func_02001714();
void Func_0200179e();
void Func_020017b4();
void Func_020016e0();
void Func_0200174e();
void Func_02001768();
void Func_02001776();
void Func_02001776_a();
void Func_02001784();
void Func_02001784_a();
void Func_02001792();
void Func_020017b2();
void Func_020017ba();
void Func_020017c8();
void Func_020017e0();
void Func_020017f6();
void Func_020017f6_a();
void Func_020017f8();
void Func_02001802();
void Func_0200180c();
void Func_0200180e();
void Func_02001814();
void Func_0200181a();
void Func_0200181c();
void Func_02001822();
void Func_0200182c();
void Func_02001832();
void Func_0200183a();
void Func_02001840();
void Func_02001844();
void Func_02001852();
void Func_02001866();
void Func_0200186c();
s32 Func_02001874();
void Func_02001878();
void Func_02001882();
void Func_02001882_a();
void Func_0200189a();
void Func_0200189a_a();
void Func_020018b2();
void Func_020018b4();
void Func_020018ca();
void Func_020018d0();
void Func_020018d6();
s32 Func_020018f4();
void Func_020018fa();
void Func_020018fc();
void Func_0200190c();
void Func_02001912();
void Func_0200192c();
void Func_02001930();
void Func_02001934();
void Func_0200194a();
void Func_02001950();
void Func_02001958();
void Func_0200195c();
void Func_0200197a();
void Func_02001980();
void Func_02001984();
void Func_02001986();
void Func_0200198e();
void Func_020019b2();
void Func_020019c0();
void Func_020019c4();
void Func_020019cc();
void Func_020019ce();
void Func_020019e2();
void Func_020019e6();
void Func_020019f4();
void Func_020019f8();
void Func_020019fa();
void Func_02001a00();
void Func_02001a00_a();
void Func_02001a16();
void Func_02001a24();
void Func_02001a24_a();
void Func_02001a2e();
void Func_02001a30();
void Func_02001a32();
void Func_02001a3c();
void Func_02001a3c_a();
void Func_02001a40();
void Func_02001a48();
void Func_02001a54();
void Func_02001a5a();
void Func_02001a68();
void Func_02001a70();
void Func_02001a70_a();
void Func_02001a72();
void Func_02001a74();
void Func_02001a78();
void Func_02001a86();
void Func_02001a88();
void Func_02001a88_a();
void Func_02001a96();
void Func_02001aa0();
void Func_02001aa4();
void Func_02001ab6();
void Func_02001ac6();
void Func_02001aee();
void Func_02001af0();
void Func_02001afc();
void Func_02001b10();
void Func_02001b30();
void Func_02001b3e();
void Func_02001b40();
void Func_02001b4c();
void Func_02001b52();
void Func_02001b56();
void Func_02001b64();
void Func_02001b66();
void Func_02001b68();
void Func_02001b7a();
void Func_02001b7c();
void Func_02001b82();
void Func_02001b86();
void Func_02001b88();
void Func_02001b94();
void Func_02001b98();
void Func_02001ba0();
void Func_02001ba0_a();
void Func_02001ba2();
void Func_02001baa();
void Func_02001bb0();
void Func_02001bba();
void Func_02001bc2();
void Func_02001bc2_a();
void Func_02001bc8();
void Func_02001be0();
void Func_02001bec();
void Func_02001bf8();
void Func_02001bfa();
void Func_02001c08();
void Func_02001c10();
void Func_02001c2c();
void Func_02001c30();
void Func_02001c34();
void Func_02001c42();
void Func_02001c58();
void Func_02001c66();
void Func_02001c6e();
void Func_02001c74();
void Func_02001c7a();
void Func_02001c7c();
void Func_02001c86();
void Func_02001c98();
void Func_02001c9e();
void Func_02001ca2();
void Func_02001ca8();
void Func_02001caa();
void Func_02001cae();
void Func_02001cb4();
void Func_02001cba();
void Func_02001cba_a();
void Func_02001cc2();
void Func_02001ce4();
void Func_02001cf2();
void Func_02001cf8();
void Func_02001d08();
void Func_02001d10();
void Func_02001d14();
void Func_02001d1c();
void Func_02001d20();
void Func_02001d20_a();
void Func_02001d26();
void Func_02001d2c();
void Func_02001d2c_a();
void Func_02001d30();
void Func_02001d38();
void Func_02001d4e();
void Func_02001d5e();
void Func_02001d60();
void Func_02001d6e();
void Func_02001d70();
void Func_02001d80();
void Func_02001db0(s32, s32);
void Func_02001d28(s32, s32);
void Func_02001d5c(void);
void Func_02001d3e();
void Func_02001e88();
void Func_02001d76();
void Func_02001e26();
void Func_02001ec4();
void Func_02001ddc(void);
void Func_02001f0c(s32);
void Func_02001e84(s32);
void Func_02001e9c(s32, s32);
void Func_02001e08(void);
s32 Func_02001dfa();
void Func_02001e24();
void Func_02001e48();
u8 *Func_02001e6e();
s32 Func_02001e78();
void Func_02001e92();
void Func_02001ea2();
void Func_02001ea4();
u8 *Func_02001eb4();
void Func_02001ebc();
void Func_02001ec8();
void Func_02001ed2_a();
void Func_02001ed2_b();
void Func_02001ede();
void Func_02001eea();
void Func_02001eee();
s32 Func_02001f14();
void Func_02001f2a();
void Func_02001f3a();
void Func_02001f3c();
void Func_02001f3e();
void Func_02001f48();
void Func_02001f4a_a();
void Func_02001f4a_b();
void Func_02001f4e();
u8 *Func_02001f50();
void Func_02001f5c();
void Func_02001f6a();
void Func_02001f70();
void Func_02001f76();
void Func_02001f8c();
void Func_02001f94();
void Func_02001fa4();
void Func_02001fd2();
void Func_02001fd6();
void Func_02001fe8();
void Func_02001ff0();
void Func_02002014();
void Func_0200202e();
void Func_02002030();
void Func_02002038();
void Func_02001fb4();
s32 Func_02001fe4();
void Func_02001fe6();
void Func_02001ff4();
void Func_02002012();
void Func_0200204a();
s32 Func_02002064();
void Func_02002066();
void Func_0200208c();
void Func_020020ae();
void Func_02002020();
void Func_02002026();
void Func_0200204c();
void Func_0200206a();
void Func_02002082();
void Func_020020b0();
void Func_020020ca();
void Func_020020d6();
void Func_020020ea();
void Func_020020ec();
void Func_020020f2();
void Func_020020f4_a();
void Func_020020f4_b();
void Func_020020fa();
void Func_0200210c();
void Func_02002114();
void Func_0200211c();
void Func_02002122();
void Func_02002124();
void Func_02002126();
void Func_02002128();
void Func_0200212a();
void Func_0200213c_a();
void Func_0200213c_b();
void Func_02002152();
void Func_02002158();
void Func_02002164();
void Func_02002170();
void Func_02002184();
void Func_0200218c();
void Func_020021b8();
void Func_020021be();
void Func_02002134();
void Func_0200215e();
void Func_02002176();
void Func_0200219a();
void Func_020021b6();
void Func_020021d2();
void Func_020021d6();
void Func_020021d8();
void Func_020021e4();
void Func_020021ee();
void Func_020021f4();
void Func_020021fe();
void Func_02002200();
void Func_02002202();
void Func_0200220c();
s32 Func_02002214();
void Func_02002218_a();
void Func_02002218_b();
void Func_02002226_a();
s32 Func_02002226_b();
void Func_02002230();
void Func_02002238();
void Func_02002240();
void Func_0200224a();
void Func_0200224c();
void Func_02002268();
void Func_0200226e();
void Func_0200227e();
void Func_02002284();
void Func_02002290();
void Func_0200229a();
void Func_0200229e();
void Func_020022a0();
void Func_020022a2();
void Func_020022aa();
void Func_020022b4();
void Func_020022c2();
void Func_020022c4();
void Func_020022d4();
void Func_020022e0();
void Func_020022e6();
void Func_020022e8();
void Func_020022ec();
void Func_020022ee();
void Func_020022f0();
void Func_020022fa();
void Func_02002308();
void Func_02002314();
void Func_02002326();
void Func_0200232e();
void Func_02002344();
void Func_0200234e();
void Func_0200235a();
void Func_0200235e();
void Func_0200236e();
void Func_02002376();
void Func_0200237c();
void Func_0200237e();
void Func_02002386();
void Func_0200238a();
void Func_0200238e();
void Func_02002390();
void Func_0200239e();
void Func_020023aa();
s32 Func_020023ac();
void Func_020023ae();
void Func_020023b4();
void Func_020023bc();
void Func_020023c2();
void Func_020023c4();
void Func_020023cc();
void Func_020023ce();
void Func_020023d6();
void Func_020023e4();
void Func_02002402();
u8 *Func_02002408();
void Func_0200240a();
void Func_02002424_a();
void Func_02002424_b();
void Func_0200242a();
void Func_02002440();
void Func_02002442();
void Func_0200244a();
void Func_02002458();
void Func_0200245a();
void Func_0200246c();
void Func_02002470();
void Func_02002472();
void Func_02002476();
void Func_02002486();
void Func_02002488();
void Func_0200248c();
void Func_02002490();
void Func_02002492();
void Func_0200249c();
void Func_020024b4();
void Func_020024cc();
void Func_020024d4();
void Func_020024e4_a();
void Func_020024e4_b();
void Func_020024fc();
void Func_02002514();
void Func_02002588();
void Func_020024c0(void);
void Func_02002530(s32, s32);
void Func_020024a8(s32, s32);
void Func_020024dc(void);
void Func_020024b6(s32, s32, s32, s32, s32, s32);
void Func_020024d2(s32, s32, s32, s32, s32, s32);
void Func_02002508(s32);
void Func_020024ee(s32, s32, s32, s32, s32, s32);
void Func_0200250a(s32, s32, s32, s32, s32, s32);
void Func_02002540(s32);
struct Actor02001060 *Func_02002570(s32);
struct Actor *Func_0200259e(s32);
struct Actor *Func_020025a6(s32);
void Func_0200265e(s32, s32);
void Func_020025ae();
u8 *Func_020025cc();
u8 *Func_020025de();
u8 *Func_020025ec();
u8 *Func_020025f8();
void Func_020025c0();
void Func_020025d2();
void Func_020025b8();
void Func_02002600();
void Func_020026e0();
void Func_0200262c();
void Func_0200263a(void);
void Func_02002616(s32, s32, s32, s32, s32, s32);
u8 *Func_0200266c();
void Func_02002648(s32);

/* Contiguous unnamed leaf-owner run for resource_3a2. */

/*
 * resource_3a2 owner at 0x020000b0, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x20000b4 holding 0x20098ec.
 *
 * LEAF RESIDUE. Published at image offset 0x14; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x020000b0 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x20098ec is image offset
 * 0x18ec under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/* Returns a value: the reference sets r1 before r0 at this site, which only a
 * value-returning callee does. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* The scene step counter at 0x1d8 of the shared scene work record. */

/*
 * resource_3a2 owner at 0x020010b8, 140 bytes.  Complete owner:
 * `push {r5, r6, lr}` / `sub sp,#8` prologue at 0x020010b8 and the interworking
 * return `add sp,#8 / pop {r5, r6} / pop {r0} / bx r0` at 0x02001136.  r0 holds
 * the popped return address, so the owner returns nothing; it takes no
 * arguments.  Straight-line body, no branches.
 *
 * A one-word literal pool sits at 0x02001140 (0x0200113e is the alignment
 * halfword after the `bx`), inside the row's 140-byte span but never reached by
 * a control-flow walk from the prologue:
 *   0x02001140 = 0x02009061
 *
 * LINK-BASE WITNESS: 0x02009061 is ODD, so by the parity rule it is a Thumb
 * function entry, and 0x02009061 - 0x8000 - 1 = 0x1060, which is exactly
 * Func_02001060 -- a function this overlay already has byte-exact in
 * games/gs1/asm/overlays/resource_3a2_c_02001060.c.  That confirms the 0x02008000 link
 * base for resource_3a2 against tracked material, and identifies the pool word
 * as an installed per-entity callback rather than data.  It is stored into the
 * entity record at +0x6c.
 *
 * The byte-exact games/gs1/asm/overlays/resource_3a2_c_02001060.c gives that callback's
 * own record layout: a `rank` s32 at +12 and a flags byte at +31 whose bit 1 it
 * sets or clears against the rank of Func_02002570(0) -- the printed name for
 * the same slot-record accessor this owner reaches as Scene_GetRecord.  So the
 * bit-1 clears below (`& 0xfd`) are the same flag that callback later drives.
 *
 * Calls resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3a2 10b8
 * --json` (eleven sites, seven distinct veneer targets; the inventory's
 * `calls=11` counts sites here):
 *   0x10bc -> veneer 0x14f0 -> Func_0808a018
 *   0x10c2 -> veneer 0x1508 -> Scene_GetRecord
 *   0x10d4 -> veneer 0x1508 -> Scene_GetRecord
 *   0x10e2 -> veneer 0x1508 -> Scene_GetRecord
 *   0x10ee -> veneer 0x1508 -> Scene_GetRecord
 *   0x1106 -> veneer 0x14b8 -> Func_080091c0
 *   0x1118 -> veneer 0x14b8 -> Func_080091c0
 *   0x111e -> veneer 0x1498 -> Func_080000c0
 *   0x1126 -> veneer 0x14d8 -> GameFlag_Set
 *   0x112e -> veneer 0x15b0 -> Func_0808a1e0
 *   0x1132 -> veneer 0x14f8 -> Func_0808a020
 *
 * Behaviour: prepare entity 14 -- clear bit 1 of its bytes at +35 and +89,
 * clear the byte at +85, install the Func_02001060 callback at +0x6c -- then
 * repaint two tile spans through the six-argument Func_080091c0, run one screen
 * step (Func_080000c0(1)), post cue 512 (built as `movs r0,#128 / lsls r0,#2`)
 * and hand entity 14 to Func_0808a1e0 with mode 2.
 *
 * Uncertainty: r5 holds the mask 253 across both bit clears and r6 holds 0 for
 * the +85 store; nothing else survives.  No register is set before
 * Func_0808a018, so it takes no asserted arguments.  Old-style declarations
 * keep every import's interface open.
 */

     /* installed callback, byte-exact in games/gs1/asm/overlays */

/* Two sites reach this one symbol with different arities; old-style so both
 * calls are legal. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];
    void Func_02001d40();

    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];
    void Func_02001d40();

    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];
    void Func_02001d40();

    return f(a0, a1);
}

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    void Func_02001dee(s32, s32, s32);

    Func_02001dee(actor, x, y);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Scene_AdvanceStep(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

s32 EventScript_PrepareActorRenderFlags(struct EventActor *actor)
{
    actor->flags &= ~1;
    actor->render_state[9] |= 0xc;
    actor->render_state[21] |= 0xc;
    return 0;
}

s32 OverlayObject_SetFacingTowardObject10(void *self)
{
    void *obj;

    obj = Func_02001564(0xA);
    FIELD_AT_OFFSET(self, s16 *, 6) = Func_0200150e(FIELD_AT_OFFSET(obj, s32 *, 0x10) - FIELD_AT_OFFSET(self, s32 *, 0x10), FIELD_AT_OFFSET(obj, s32 *, 8) - FIELD_AT_OFFSET(self, s32 *, 8));
    return 0;
}

s32 SceneData_SelectTable97b4ByState(void)
{
    if (Data_02000240[224] == (s32)&Value_0000004a) {
        return (s32)Data_02009844;
    }
    return (s32)Data_020097b4;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTable98ec(void)
{
    return (u8 *)0x020098ec;
}

s32 SceneData_SelectTable9918ByState(void)
{
    if (Data_02000240[224] == (s32)&Value_0000004a) {
        return (s32)Data_02009a38;
    }
    return (s32)Data_02009918;
}

void SceneDialogue_RunMessage1958Step(void)
{
    extern u8 *Data_03001ebc;

    u8 *work;

    Func_020015dc();
    Func_02001672(0x1958);
    Func_02001682(10, 0);

    if (Func_02001602(0, 0) == 1) {
        Func_020015f4(20);
        Func_020016a4(10, 0);
    } else {
        work = Data_03001ebc;
        *(u16 *)(work + 472) += 1;
        Func_020016ce(10, 0);
    }

    Func_0200162a();
}

void SceneDialogue_RunActor11Message195d(void)
{
    Func_02001634();
    Func_020016ca(0x195D);
    Func_020016f2(11, 0);
    Func_0200164e();
}

void SceneDialogue_RunActor13Message1961(void)
{
    Func_02001654();
    Func_020016ea(0x1961);
    Func_02001712(13, 0);
    Func_0200166e();
}

void FieldScene_RunPrimaryScript(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_0200179e(188);
    Call3(Func_02001638, 0x2009788, 67, 6);
    *(u8 *)(Func_0200169e(0) + 85) = 0;
    Call3(Func_020016b6, 0, 0xcccc, 0x6666);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x100;
    Func_02001714(0, 2);
    Call3(Func_02001708, 0, 0, -16);
    Func_020016b6_a(16);
    Func_020017b4(2);
}

void FieldScene_RunScene3a2SequenceA(void)
{
    extern u8 Data_03001ebc[];
    void Func_02001d40();

    u32 i;
    s32 record;

    Func_020016e0();
    Call3(Func_0200174e, 8, 0x880000, 0xa80000);
    Call3(Func_020017b2, 8, 0x5000, 0);
    Call3(Func_02001768, 0, 0x900000, 0xc80000);
    Call3(Func_02001776, 1, 0xa00000, 0xc00000);
    Call3(Func_02001784, 2, 0x800000, 0xc80000);
    Call3(Func_02001792, 3, 0x700000, 0xc00000);
    Call3(Func_020017f6, 0, 0xc000, 0);
    Call3(Func_02001802, 1, 0xa000, 0);
    Call3(Func_0200180e, 2, 0xc000, 0);
    Call3(Func_0200181a, 3, 0xe000, 0);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x201;
    Func_0200186c();
    Func_02001878();
    Func_02001776_a(60);
    Func_020017f6_a(8, 3);
    Func_02001784_a(20);
    Call1(Func_02001822, 0x19e9);
    Func_02001844(8, 0, 20);
    Func_0200180c(0, 3);
    Func_02001814(1, 3);
    Func_0200181c(2, 3);
    Func_0200182c(3, 3);
    Func_020017ba(20);
    Func_0200183a(8, 4);
    Func_020017c8(20);
    Func_02001882(8, 0, 20);
    Func_02001852(3, 4);
    Func_020017e0(20);
    Func_0200189a(3, 0, 20);
    Func_02001882_a(2, 2);
    Func_020017f8(20);
    Func_020018b2(2, 0, 20);
    Func_0200189a_a(8, 2);
    Call3(Func_020018d6, 8, 0x5000, 20);
    Func_020018d0(8, 0, 20);
    Call3(Func_020018fc, 2, 0x102, 60);
    Func_02001832(120);
    Func_020018ca(1, 2);
    Func_02001840(20);
    Call3(Func_0200190c, 1, 0x6000, 20);
    Value2(Func_020018f4, 1, 0);
    if (Value2(Func_02001874, 0, 0) == 0) {
        Func_02001866(20);
        Func_02001930(3, 0, 20);
        Call3(Func_0200194a, 3, 0x101, 60);
        Func_02001934(3, 0, 20);
        Call3(Func_02001950, 0, 0xe000, 0);
        Call3(Func_0200195c, 2, 0xe000, 0);
        Func_0200192c(1, 2);
        Call2(Func_02001986, 1, 0x102);
        Func_020018b4(60);
        Call3(Func_02001980, 1, 0x8000, 20);
        Func_0200197a(1, 0, 20);
        bump_step(2);
    } else {
        bump_step(2);
        Func_020018fa(20);
        Func_020019c4(3, 0, 20);
        Func_02001984(3, 3);
        Func_02001912(20);
        Func_020019cc(3, 0, 20);
        Call3(Func_020019f8, 1, 0x102, 60);
        Call3(Func_020019f4, 0, 0xe000, 0);
        Call3(Func_02001a00, 2, 0xe000, 0);
        Func_020019fa(1, 0, 20);
    }
    Func_020019e2(8, 2);
    Func_02001958(20);
    Call3(Func_02001a24, 0, 0xc000, 0);
    Call3(Func_02001a30, 1, 0xa000, 0);
    Call3(Func_02001a3c, 2, 0xc000, 0);
    Call3(Func_02001a48, 3, 0xe000, 0);
    Func_0200198e(20);
    Call3(Func_02001a5a, 8, 0x3000, 20);
    Func_02001a54(8, 0, 20);
    Func_02001a3c_a(1, 2);
    Func_020019b2(20);
    Func_02001a32(1, 3);
    Func_020019c0(30);
    Func_02001a40(8, 3);
    Func_020019ce(20);
    Func_02001a88(8, 0, 20);
    Func_02001a70(8, 2);
    Func_020019e6(20);
    Func_02001aa0(8, 0, 20);
    Func_02001a68(0, 3);
    Func_02001a70_a(1, 3);
    Func_02001a78(2, 3);
    Func_02001a88_a(3, 3);
    Func_02001a16(20);
    Func_02001a96(8, 3);
    Func_02001a24_a(20);
    Call3(Func_02001af0, 8, 0xc000, 30);
    Func_02001b66(188);
    Call3(Func_02001a00_a, 0x2009788, 67, 6);
    Call3(Func_02001a72, 8, 0xcccc, 0x6666);
    Func_02001aa4(8, 136, 136);
    Func_02001ac6(8, 0, 0);
    Func_02001b94(188);
    Call3(Func_02001a2e, 0x200979e, 67, 6);
    Func_02001a74(60);
    Func_02001b98();
    Func_02001b10(1, 2);
    Func_02001a86(20);
    Call3(Func_02001b52, 1, 0x6000, 20);
    Func_02001b4c(1, 0, 20);
    Call3(Func_02001b68, 0, 0x8000, 20);
    Func_02001b40(2, 2);
    Func_02001ab6(20);
    Call3(Func_02001b82, 3, 0x2000, 20);
    Func_02001b7c(3, 0, 20);
    Func_02001b64(1, 1);
    Call3(Func_02001ba0, 0, 0xe000, 20);
    Func_02001ba0_a(1, 0);
    Func_02001aee(20);
    Func_02001b86(2, 2);
    Func_02001afc(20);
    Call3(Func_02001bc8, 0, 0x8000, 20);
    Func_02001bc2(2, 0, 20);
    Func_02001ba2(0, 1);
    Func_02001baa(1, 1);
    Func_02001bba(3, 1);
    Func_02001b30(20);
    Func_02001bb0(2, 4);
    Func_02001b3e(20);
    Func_02001bf8(2, 0, 20);
    Func_02001be0(3, 2);
    Func_02001b56(20);
    Func_02001c10(3, 0, 20);
    Call3(Func_02001c2c, 2, 0xb000, 20);
    Func_02001bec(2, 3);
    Func_02001b7a(20);
    Func_02001bfa(2, 3);
    Func_02001b88(20);
    Func_02001c42(2, 0, 20);
    Call3(Func_02001c6e, 1, 0x102, 60);
    Func_02001c58(1, 0, 20);
    Call3(Func_02001c74, 2, 0xe000, 20);
    Func_02001c34(2, 4);
    Func_02001bc2_a(20);
    Func_02001c7c(2, 0, 20);
    Call3(Func_02001ca8, 0, 0x102, 0);
    Call3(Func_02001cb4, 3, 0x102, 60);
    Func_02001c9e(3, 0, 20);
    Call3(Func_02001cba, 2, 0xa000, 20);
    Func_02001c7a(2, 3);
    Func_02001c08(20);
    Func_02001cc2(2, 0, 20);
    Func_02001ca2(0, 1);
    Func_02001caa(1, 1);
    Func_02001cba_a(3, 1);
    Func_02001c30(20);
    Call3(Func_02001c66, 2, 0x8000, 0x4000);
    Func_02001c98(2, 128, 184);
    Call3(Func_02001d14, 2, 0x4000, 20);
    Call3(Func_02001d20, 0, 0xc000, 0);
    Call3(Func_02001d2c, 1, 0xa000, 0);
    Call3(Func_02001d38, 3, 0xe000, 20);
    Func_02001cf8(2, 3);
    Func_02001c86(20);
    Func_02001d40(2, 0, 20);
    Func_02001d08(0, 3);
    Func_02001d10(1, 3);
    Func_02001d20_a(3, 3);
    Func_02001cae(20);
    Call3(Func_02001ce4, 1, 0x8000, 0x4000);
    Call3(Func_02001cf2, 3, 0x8000, 0x4000);
    Func_02001d1c(1, 144, 200);
    Func_02001d26(2, 144, 200);
    Func_02001d30(3, 144, 200);
    Func_02001d4e(1);
    Func_02001d60(1, 0, 0);
    Func_02001d5e(2);
    Func_02001d70(2, 0, 0);
    Func_02001d6e(3);
    Func_02001d80(3, 0, 0);
    Func_02001d2c_a();
}

void SceneDialogue_RunLine1956(void)
{
    void Func_02001d40_a(void);

    Func_02001d40_a();
    Func_02001db0(0, 1);
    Func_02001d28(0x1956, 1);
    Func_02001d5c();
}

void ConfigureAndPlaceActorFourteen(void)
{
    void Func_02001dee(s32, s32, s32);

    s32 a = 21, b = 9;
    Func_02001d3e(85, 9, 1, 1, a, b);
    Func_02001e88(100, 0, 0);
    PlaceActor(14, 0x01580000, 0x00980000);
}

void FieldScene_RunScene3a2_020008a8(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Call6(Func_02001d76, 21, 73, 1, 1, 21, 9);
    Call3(Func_02001ec4, 100, -1, -1);
    Func_02001e26(14, 0, 0);
}

void SceneDialogue_RunActorFifteenByLeaderHeading(void)
{
    struct Slot020008e0 *Func_02001dee_a(s32);

    u32 heading = Func_02001dee_a(0)->heading;

    Func_02001ddc();
    if (heading - 0xA001 <= 0x3FFE) {
        Func_02001f0c(15);
    } else {
        Func_02001e84(0x1A1E);
        Func_02001e9c(15, 0);
    }
    Func_02001e08();
}

void Scene_RunEventTransition(void)
{
    u8 *record;
    s32 none;

    if (Value1(Func_02001dfa, 0x89a) == 0) {
    } else {
        Func_02001e24();
        Call3(Func_02001e92, 10, 0x2180000, 0xd80000);
        Call1(Func_02001ec8, 0x18b5);
        Func_02001eea(10, 0, 20);
        Func_02001ed2_a(0, 2);
        Func_02001e48(20);
        record = Func_02001e6e(0);
        *(s32 *)((s32)record + 108) = 0x2008055;
        record = Value1(Func_02001e78, 0);
        if ((*(s32 *)((s32)record + 16) >> 20) == 13) {
            Call3(Func_02001ebc, 0, 0x1b8, 200);
        }
        Call3(Func_02001ea2, 10, 0x20000, 0x10000);
        Func_02001f4a_a(10, 2);
        Call3(Func_02001ede, 10, 0x198, 216);
        {
            u8 *record = Func_02001eb4(10);
            u32 flag = 1;

            flag = flag | record[35];
            record[35] = (u8)flag;
        }
        Func_02001ea4(10);
        Call3(Func_02001f70, 10, 0x8000, 20);
        Func_02001f6a(10, 0, 20);
        Func_02001f4a_b(10, 2);
        Call2(Func_02001fa4, 10, 0x102);
        Func_02001ed2_b(60);
        Func_02001f8c(10, 0, 20);
        Value2(Func_02001f14, 10, 0x200962c);
        Call4(Func_02001fd6, 0x1280000, -1, 0x1580000, 1);
        Call1(Func_02001eee, 0x8b0);
        Func_02001f3c(10);
        Func_02001ff0();
        Call3(Func_02001f3e, 0, 0x10000, 0x8000);
        Call2(Func_02001f4e, 0, 0x20096b8);
        Func_02001f5c(0);
        Func_02001f2a(10);
        none = 0;
        record = Func_02001f50(0);
        *(s32 *)((s32)record + 108) = none;
        Func_02001f3a(30);
        Func_02001fd2(10, 2);
        Func_02001f48(20);
        Call3(Func_02002014, 10, 0x5000, 120);
        Call3(Func_0200202e, 10, 0x105, 60);
        Call3(Func_02002038, 0, 0x101, 60);
        Func_02001fe8(10, 4);
        Func_02001f76(20);
        Func_02002030(10, 0, 20);
        Func_02001f94();
    }
}

void Scene_RunActorCue(void)
{
    extern u8 Data_03001ebc[];

    Func_02001fb4();
    Call1(Func_0200204a, 0x18b9);
    Call3(Func_0200208c, 10, 0x105, 60);
    Value2(Func_02002064, 10, 0);
    if (Value2(Func_02001fe4, 0, 0) == 1) {
        Scene_AdvanceStep(1);
    }
    Func_02001fe6(20);
    Func_02002066(10, 4);
    Func_02001ff4(20);
    Func_020020ae(10, 0, 20);
    Func_02002012();
}

void Scene_RunActorExchange(void)
{
    Func_02002020();
    Func_0200211c();
    Func_02002128();
    Func_02002026(30);
    Call3(Func_020020f2, 0, 0x6000, 0);
    Func_0200212a(9, 1);
    Func_02002126();
    Func_020020d6(8, 2);
    Func_0200204c(20);
    Call1(Func_020020ea, 0x19cf);
    Func_0200210c(8, 0, 20);
    Func_020020f4_a(9, 1);
    Func_0200206a(20);
    Func_02002124(9, 0, 20);
    Func_020020f4_b(8, 3);
    Func_02002082(20);
    Func_0200213c_a(8, 0, 20);
    Call3(Func_02002158, 10, 0xd000, 20);
    Func_02002152(10, 0, 20);
    Func_02002122(8, 3);
    Func_020020b0(60);
    Call3(Func_0200218c, 8, 0x102, 60);
    Func_0200213c_b(8, 4);
    Func_020020ca(20);
    Func_02002184(8, 0, 20);
    Func_02002164(10, 2);
    Call2(Func_020021be, 10, 0x102);
    Func_020020ec(60);
    Call3(Func_020021b8, 10, 0xb000, 20);
    Func_02002170(9, 5);
    Func_02002114();
    Call1(Func_020020fa, 0x8b1);
}

void Scene_RunActorSequence(void)
{
    extern u8 Data_03001ebc[];
    void Func_02002124_a();

    s32 mask;

    Func_02002124_a();
    Call2(Func_020021fe, 8, 0x102);
    Func_020021b6(8, 2);
    Func_02002134(60);
    Call1(Func_020021d2, 0x19da);
    Func_020021f4(8, 0, 20);
    Call2(Func_02002226_a, 10, 0x102);
    Func_020021d8(10, 4, 0);
    Func_0200215e(60);
    Func_02002218_a(10, 0, 20);
    Func_02002200(8, 1);
    Func_02002176(20);
    Func_02002230(8, 0, 20);
    Call3(Func_0200224c, 10, 0xd000, 20);
    Func_0200220c(10, 3);
    Func_0200219a(20);
    Call3(Func_020021ee, 8, 178, 0x114);
    Call3(Func_02002202, 10, 172, 0x11c);
    Func_02002218_b(8);
    Call3(Func_02002284, 8, 0x5000, 0);
    Call3(Func_02002290, 10, 0xb000, 0);
    Func_020021d6(20);
    Func_0200226e(8, 2);
    Func_020021e4(20);
    mask = 254;
    Func_0200229e(8, 0, 20);
    *(u8 *)(Func_02002214(8) + 90) &= mask;
    *(u8 *)(Func_02002226_b(10) + 90) &= mask;
    Call3(Func_02002240, 8, 0x3333, 0x1999);
    Call3(Func_0200224a, 10, 0x3333, 0x1999);
    Func_0200229a(8, 5);
    Func_020022a2(10, 6);
    Func_02002238(20);
    Func_0200236e(125);
    Func_020022a0(8, 2, 0);
    Func_020022aa(9, 2, 0);
    Func_020022b4(10, 2, 0);
    Func_020022c2(10);
    Func_02002268(30);
    Func_020022e0(8, 5);
    Func_020022e8(10, 6);
    Func_0200227e(20);
    Func_020023b4(125);
    Func_020022e6(8, 4, 0);
    Func_020022f0(9, 4, 0);
    Func_020022fa(10, 4, 0);
    Func_02002308(10);
    Func_020022ee(9);
    Func_02002326(8, 1);
    Func_0200232e(10, 1);
    Func_020022c4(50);
    Func_0200234e(10, 2, 0);
    Func_020022d4(20);
    Func_0200238e(10, 0, 20);
    Func_0200235e(8, 3);
    Func_020022ec(20);
    Value2(Func_020023ac, 8, 0);
    Func_020023ae(8, 0, 20);
    Func_02002376(8, 5);
    Func_0200237e(10, 6);
    Func_02002314(20);
    Func_0200244a(125);
    Func_0200237c(8, 2, 0);
    Func_02002386(9, 2, 0);
    Func_02002390(10, 2, 0);
    Func_0200239e(10);
    Func_02002344(30);
    Func_020023bc(8, 5);
    Func_020023c4(10, 6);
    Func_0200235a(20);
    Func_02002490(125);
    Func_020023c2(8, 4, 0);
    Func_020023cc(9, 4, 0);
    Func_020023d6(10, 4, 0);
    Func_020023e4(10);
    Func_0200238a(40);
    Func_02002402(8, 1);
    Func_0200240a(10, 1);
    Func_02002424_a(10, 2, 0);
    Func_020023aa(20);
    Call3(Func_02002476, 10, 0xd000, 20);
    Func_02002470(10, 0, 20);
    Func_02002440(8, 3);
    Func_020023ce(20);
    Func_02002488(8, 0, 30);
    Func_02002492(8, 0, 20);
    {
        u8 *record = Func_02002408(10);
        u32 flag = 1;

        flag = flag | record[90];
        record[90] = (u8)flag;
    }
    Call3(Func_02002424_b, 10, 0xcccc, 0x6666);
    Call3(Func_02002458, 10, 168, 0x128);
    Call3(Func_020024d4, 10, 0xd000, 20);
    Func_0200248c(10, 5);
    Func_0200249c(8, 3);
    Func_0200242a(20);
    Func_020024e4_a(8, 0, 20);
    Func_020024b4(10, 3);
    Func_02002442(20);
    Func_020024fc(10, 0, 20);
    Func_020024cc(8, 3);
    Func_0200245a(20);
    Func_02002514(8, 0, 20);
    Func_020024e4_b(0, 3);
    Func_02002472(20);
    Func_02002486();
    Call1(Func_0200246c, 0x8b2);
    *(s32 *)((*(u8 **)Data_03001ebc + 0x1c0)) = 0x201;
    Func_02002588(6);
}

void FieldScene_RunScriptedSteps0And1A12(void)
{
    Func_020024c0();
    Func_02002530(0, 1);
    Func_020024a8(0x1A12, 1);
    Func_020024dc();
}

void FieldScene_RunPairedLayoutStepsThenSetOne(void)
{
    {
        s32 fifth = 1;
        s32 sixth = 2;

        Func_020024b6(5, 28, 5, 13, fifth, sixth);
    }
    {
        s32 fifth = 5;
        s32 sixth = 13;

        Func_020024d2(5, 28, 1, 2, fifth, sixth);
    }
    Func_02002508(1);
}

void SceneState_RunRect6x28Step(void)
{
    {
        s32 fifth = 1;
        s32 sixth = 2;

        Func_020024ee(6, 28, 5, 13, fifth, sixth);
    }
    {
        s32 fifth = 5;
        s32 sixth = 13;

        Func_0200250a(6, 28, 1, 2, fifth, sixth);
    }
    Func_02002540(1);
}

s32 SceneActor_SetFlagBitByRankAgainstActorZero(struct Actor02001060 *actor)
{
    if (Func_02002570(0)->rank > actor->rank) {
        actor->flags |= 2;
    } else {
        actor->flags &= 0xFD;
    }
}

void SceneActor_UpdateActorFourteenByDepth(void)
{
    struct Actor *current = Func_0200259e(0);
    struct Actor *other = Func_020025a6(14);

    if (current->f10 <= other->f10) {
        Func_0200265e(14, 1);
    }
}

void ActorPresentation_PrepareActorFourteenWithCallback(void)
{
    u8 zero;

    zero = 0;
    Func_020025ae();

    Func_020025cc(14)[35] &= 0xfd;
    Func_020025de(14)[89] &= 0xfd;
    Func_020025ec(14)[85] = zero;
    *(void **)(Func_020025f8(14) + 108) = (void *)0x02009061;

    Func_020025c0(55, 16, 1, 1, 56, 18);
    Func_020025d2(55, 16, 1, 1, 20, 18);

    Func_020025b8(1);
    Func_02002600(512);
    Func_020026e0(14, 2);
    Func_0200262c();
}

void FieldScene_SetSlot15Byte89AndRunStep(void)
{
    u8 *slot;

    Func_0200263a();
    {
        s32 fifth = 21;
        s32 sixth = 11;

        Func_02002616(14, 6, 1, 2, fifth, sixth);
    }
    slot = Func_0200266c(15) + 89;
    *slot = 254;
    Func_02002648(0x201);
    Func_0200266c_a();
}

s32 SceneData_SelectTableByWord224(void)
{
    if (Data_02000240[224] == (s32)&Value_0000004a) {
        return (s32)Data_02009c9c;
    }
    return (s32)Data_02009b10;
}
