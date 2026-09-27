/* FAKEMATCH: preserve five reconstructed per-module return-type boundaries
 * while consolidating the exact owners; their names bind to the same own-ROM
 * import slots. Canonical FIELD_EVENT.H declarations remain unchanged.
 * H1 predicts identical complete owner bytes, not a new ABI or credit.
 * Rejected scene H2 remains in 3db9eafb8: 2820 bytes/1081 halfwords/293 edits. */
#include "TYPES.H"

#define NULL ((void *)0)
#define CalculateFacingAngle Func_02002656

#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "FACING_OBJECT.H"
#include "FIELD_EFFECT.H"

struct Obj {
    u8 pad00[6];
    u16 f06;
    u8 pad08[0x30];
    s32 f38;
    s32 f3c;
    s32 f40;
};

struct VerticalEffectAnchor {
    u8 pad00[8];
    s32 x;
    u8 pad0c[4];
    s32 z;
};

struct SceneVerticalEffect {
    u8 pad00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[4];
    s32 amplitude_x;
    s32 amplitude_y;
    u8 pad20[0x44];
    u16 frame;
    u8 pad66[2];
    struct VerticalEffectAnchor *anchor;
};

/*
 * Exact 2026-09-23 (832 bytes), a tagged fake match. The callback passed
 * to the target-and-callback call (main Object_SetTargetAndCallback) is the
 * data symbol Data_0200ac00, as in HAIDIA_IE/ELDER_AID_EVENT.C; as a plain
 * integer its pool load was scheduled before the zero actor argument.
 * The do/while around the 0x22b area-state store holds the linked scene
 * load after the store, as in the reference.
 */
struct SceneWork {
    u8 pad000[0x22b];
    u8 area_state;
};

struct PairDetail {
    u8 unknown_00[22];
    u8 field_16;
};

struct PairSprite {
    struct FieldSprite sprite;
    struct PairDetail *detail;
};

union PairObject {
    union FieldObject object;
    s32 words[28];
    struct {
        u8 unknown_00[0x68];
        union PairObject *parent;
    } link;
};

struct PairWork {
    u8 unknown_00[70];
    u16 vram_block;
};

struct WorldMapVramBlock {
    u16 base;
    u16 offset;
};

/* The OAM view with attribute 1 ending in the two-bit size field. */
struct WorldMapOam {
    u8 unknown_00[4];
    u16 attr0;
    u16 x : 9;
    u16 affine_index : 5;
    u16 size : 2;
};

extern u8 Data_0200b144[];
extern u8 Data_0200b108[];
extern u8 Data_0200b380[];
extern u8 Data_0200b560[];
extern u8 Data_0200b7d0[];
extern u8 Data_0200b170[];
extern u8 Data_0200bcec[];
extern u8 Data_0200bb3c[];
extern u8 Data_0200bb30[];
extern u8 Data_0200ba64[];
extern u8 Data_0200b938[];
extern u8 LinkedMessage_ArentYouWorriedAboutCrossing[];
extern u8 Value_0200beb4;
extern u8 LinkedMessage_JasmineWentOffWay;
extern u8 Data_0200ae34[];
extern s16 Data_02000240_t[][1];
extern u8 Data_0200aef0[];
extern u8 Data_0200af50[];
extern u8 Data_0200af78[];
extern u8 Data_0200a874[];
extern const u8 Data_00000005[];
extern const u8 Data_0200ac90[];
extern const u8 Data_0200adf0[];
extern u8 Data_03001ebc[];
extern struct PairWork *Data_03001f30;
extern struct WorldMapVramBlock Data_03001b10[];

s32 Func_02002656(s32, s32);
void Func_02002962(s32, s32);
void Func_020027a4(void);
s32 Func_0200290c();
s32 Func_0200290c_a();
void Func_0200295a();
void Func_020029c2_a();
void Func_0200273a(s32, s32);
void Func_02002778(s32, s32);
void Func_020027c8();
s32 Func_02002b36();
s32 Func_02002b40();
void Func_02002b8a();
void Func_02002b90();
s32 Func_02002bec_a();
s32 Func_02002c22();
void Func_02004c4a(s32);
void Func_020048be(void);
void Func_02004c5a(s32);
void Func_020048ce(void);
void Func_02004c6a(s32);
void Func_0200491a(void);
struct Obj *Func_02002d3e(s32);
void Func_02002f20(void);
void Func_02004dac(void);
void Func_02004768(s32);
s32 Func_02004930(s32, s32);
void Func_020047a4(s32);
s32 Func_0200496c(s32, s32);
void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c);
s32 Engine_GameFlagIsSet();
void Scene_SetFlag();
void Engine_EventRequestExit();
s32 Scene_GetActorAddress();
void Engine_MapCopyCellsTo();
void Engine_MapRedraw();
void Engine_TaskWait();
void Main_0808a2c0();
void Main_0808a2c8();
s32 Scene_CopyCellAttributes();
void Main_0808a168();
void Engine_ActorSetSpriteFlags();
void Engine_ActorSetAnimation();
void HaidiaIe_RunScriptScene();
void Engine_EventOpenScreen();
void Engine_EventWaitForScreen();
void Main_0808a2e0();
void Func_02003048();
s32 Func_02003050();
void Func_0200310c();
void Func_02003176();
void Func_020031bc();
s32 Func_020031c4();
void Func_020031cc();
void Func_020031ee();
void Func_020031fe();
void Func_02003212_a();
void Func_0200321e();
void Func_02003226();
s32 Func_02003238();
s32 Func_02003246();
void Func_02003254();
void Func_02003256();
void Func_0200325e();
void Func_02003278();
s32 Func_0200327a();
void Func_020032ec();
void Func_020032fa();
s32 Func_02003302();
void Func_0200332a();
void Func_0200333c();
void Func_02003344();
void Func_0200335c();
void Func_02003360();
void Func_02003364();
void Func_0200338e();
void Func_02003390();
void Func_020033ae();
void Func_020033b6();
void Func_020033b8();
void Func_020033c2();
void Func_020033cc();
void Func_020033d0();
void Func_020033d2();
void Func_020033d4();
void Func_020033dc();
void Func_020033e2();
void Func_020033f8();
void Func_020033fc();
void Func_02003422();
void Func_02003424();
void Func_0200342a();
void Func_02003436();
void Func_02003450();
void Func_0200345c();
void Func_0200345e();
s32 Func_0200346a();
void Func_020034a4();
void Func_020034a6();
void Func_020034b2();
void Func_020034cc();
void Func_020034d2();
void Func_020034d8();
void Func_020034da();
void Func_020034dc();
void Func_020034f0();
void Func_0200350e();
s32 Func_02003512();
void Func_0200357e();
void Func_020035a8();
void Func_020035b4();
void Func_020035dc();
void Func_020035e8();
void Func_020035f4();
void Func_020035fc();
void Func_02003600();
void Func_02003608();
void Func_02003618();
void Func_02003620();
void Func_02003628();
void Func_02003630();
void Func_0200364a();
void Func_02003660();
void Func_02003662();
void Func_02003666();
void Func_0200366e();
s32 Func_0200367c();
void Func_0200367e();
void Func_0200368a();
void Func_020036aa();
s32 Func_020036ac_a();
void Func_020036be();
void Func_020036c6();
void Func_020036ca();
s32 Func_020036dc_a();
void Func_020036fe();
void Func_0200370c();
void Func_0200372e();
void Func_0200373c();
void Func_0200374e();
void Func_020037a2();
void Func_020024ae();
s32 Func_02003472();
void Func_02003488();
void Func_0200349e();
void Func_020034aa();
s32 Func_020034c0();
s32 Func_020034e4();
void Func_020034fe();
s32 Func_02003500();
void Func_02003696();
void Func_020036b2();
void Func_020036da();
void Func_020036f4();
void Func_0200372e_a();
void Func_0200377a();
void Func_020037c4();
void Func_020037d6();
void Func_020037fc();
void Func_02003800();
void Func_02003806();
void Func_02003826();
s32 Func_02003850();
void Func_02003858();
void Func_02003862();
void Func_02003864();
void Func_0200386c();
void Func_02003870();
void Func_02003884();
void Func_0200388a();
void Func_020038a2();
void Func_020038be();
void Func_020038c2();
void Func_020038c6();
void Func_020038d4();
void Func_020038e2();
void Func_020038e6();
void Func_020038e8();
void Func_020038ec();
void Func_020038f2();
void Func_020038f8();
void Func_02003904();
void Func_02003914();
void Func_02003920();
void Func_02003924();
void Func_02003948();
void Func_0200394c();
void Func_0200395c();
void Func_02003962();
void Func_0200396c();
void Func_02003988();
void Func_020035c2();
s32 Func_02003600_a();
s32 Func_020036de();
void Func_020036fc();
s32 Func_02003704();
void Func_02003722();
s32 Func_02003732();
s32 Func_02003754();
s32 Func_02003778();
void Func_02003792();
void Func_02003888();
void Func_020038e0();
s32 Func_0200394a();
void Func_0200399c();
void Func_020039aa();
void Func_020039d4();
void Func_020039da();
void Func_020039f8();
void Func_02003a06();
void Func_02003a12();
void Func_02003a18();
void Func_02003a20();
void Func_02003a4c();
void Func_02003a58();
void Func_02003a5a();
void Func_02003a5e();
s32 Func_02003a70();
void Func_02003a80();
void Func_02003a92();
void Func_02003a9a();
void Func_02003aa0();
void Func_02003aa2();
void Func_02003ab0();
void Func_02003ab2();
void Func_02003abe();
void Func_02003ac2();
void Func_02003ad0();
void Func_02003aec();
void Func_02003afa();
void Func_02003b02();
void Func_02003b12();
s32 Func_02003b2a();
void Func_02003b44();
void Func_02003b4c();
s32 Func_02003b54();
void Func_02003b68();
void Func_02003b84();
void Func_02003b90();
void Func_02003b92();
void Func_02003b98();
void Func_02003b9a(s32, s32, s32);
void Func_02003ba0();
void Func_02003ba6();
void Func_02003ba8();
void Func_02003bae();
void Func_02003bc4();
void Func_02003bca();
void Func_02003bcc();
void Func_02003bda();
void Func_02003be8();
void Func_02003bf2();
void Func_02003bf6();
void Func_02003c02();
s32 Func_02003c0e();
void Func_02003c1a();
void Func_02003c1c();
void Func_02003c84();
void Func_02003c8e();
s32 Func_02003d12(const void *, s32);
void Func_02003d1a();
void Func_02003d22(const void *, s32);
void Event_SayThenWait(s32 speaker, s32 frames);
void Engine_AudioPlayCue();
void Engine_EventBegin();
void Engine_CameraMoveTo();
void Engine_ActorFaceDirection();
void Engine_EventWait();
s32 Scene_SetAnimationAndWait();
void Engine_ActorSetSpritePriority();
void Engine_ActorSetSpeed();
void Engine_ActorEnableActionCallback();
void Main_0808a0b0();
void Main_0808a2d0();
void Main_0808a2d8();
void Engine_EventCloseScreen();
void Engine_GameFlagClear();
void Engine_EventEnd();
void Func_02003b88();
void Func_02003b92_a();
void Func_02003bac();
void Func_02003bbc();
void Func_02003bca_a();
void Func_02003c00();
void Func_02003c10();
void Func_02003c34();
void Func_02003c3c();
void Func_02003c56();
void Func_02003cf6();
void Func_02003d06();
void Func_02003d10();
void Func_02003d28();
void Func_02003d32();
void Func_02003d3c();
void Func_02003d6a_a();
void Func_02003d74();
void Func_02003d7e();
void Func_02003dac();
void Func_02003db6();
void Func_02003dc0();
void Func_02003dc4();
void Func_02003dd4();
void Func_02003dd4_a();
void Func_02003dfa_a();
void Func_02003e02();
void Func_02003e0c();
void Func_02003e16();
void Func_02003e18();
void Func_02003e20();
void Func_02003e38();
void Func_02003e3c_a();
void Func_02003e68();
void Func_02003e6e();
void Func_02003e7a();
void Func_02003e7c();
void Func_02003e86();
void Func_02003e92();
void Func_02003e9e();
void Func_02003eaa();
void Func_02003ece();
u8 *Func_02003ed8();
u8 *Func_02003ee4();
u8 *Func_02003ef0();
u8 *Func_02003efc();
u8 *Func_02003f08();
void Func_02003f08_a();
u8 *Func_02003f14();
void Func_02003f2c();
void Func_02003f30();
void Func_02003f38();
void Func_02003f86();
void Func_02003f96();
void Func_02003f9e();
void Func_02003fb8();
void Func_02003fc6();
void Func_02003fc6_a();
void Func_02003fe8();
void Func_02003ff2();
void Func_02003ff4();
void Func_02003ffa();
void Func_02004002();
void Func_02004004();
void Func_0200400a();
void Func_02004010();
void Func_02004012();
void Func_02004012_a();
void Func_02004014();
void Func_0200401c();
void Func_02004020();
void Func_0200402a();
void Func_0200402e();
void Func_02004036();
void Func_0200403e();
void Func_02004054();
void Func_02004054_a();
void Func_02004062();
void Func_02004068();
void Func_02004080();
void Func_02004082();
void Func_0200408e();
void Func_02004096();
void Func_020040a2();
void Func_020040a6();
void Func_020040a6_a();
void Func_020040aa();
void Func_020040b6();
void Func_020040b8();
void Func_020040b8_a();
void Func_020040ba();
void Func_020040c6();
void Func_020040cc();
void Func_020040ce();
void Func_020040d2();
void Func_020040e8();
void Func_020040f8();
void Func_020040fa();
void Func_02004114();
void Func_02004128(s32 actor, s32 target, const u8 *table);
void Func_02004132(s32 actor, s32 target, const u8 *table);
void Func_02004138();
void Func_02004156();
void Func_02004156_a();
void Func_0200415c();
void Func_02004168();
void Func_0200417e();
void Func_02004194();
void Func_02004198();
void Func_0200419e();
void Func_020041b0();
void Func_020041c0();
void Func_020041c0_a();
void Func_020041dc();
void Func_020041de();
void Func_020041fc();
void Func_02004202();
void Func_02004202_a();
void Func_02004204();
struct FieldActor *Func_0200421a(s32 actor);
void Func_02004228();
struct FieldActor *Func_02004234(s32 actor);
void Func_02004244();
struct FieldActor *Func_02004246(s32 actor);
void Func_0200424e();
void Func_02004250();
void Func_0200426c();
void Func_02004270();
void Func_02004270_a();
void Func_02004278();
void Func_02004286();
void Func_0200428a(s32 actor, const u8 *table);
void Func_0200429a();
void Func_020042b4();
void Func_020042bc();
void Func_020042c2(s32 actor, const u8 *table);
void Func_020042c8();
void Func_020042e0();
void Func_020042e2();
void Func_020042e2_a();
void Func_020042e8();
void Func_020042f2();
void Func_02004304();
void Func_0200430a();
void Func_0200430e();
void Func_02004318();
void Func_02004320();
void Func_02004326();
void Func_0200432e();
void Func_0200433a();
void Func_0200435a();
void Func_02004362();
void Func_02004364();
void Func_02004374();
void Func_02004384();
void Func_02004384_a();
void Func_02004388();
void Func_0200438c();
void Func_020043a2();
void Func_020043aa();
void Func_020043b8();
void Func_020043d6();
void Func_020043e4();
void Func_020043f2();
void Func_0200442a();
void Func_02004440();
void Func_02004458();
void Func_0200445a();
void Func_0200446e();
struct FieldActor *Func_02004476(void);
void Func_0200447c();
void Func_02004484();
void Func_02004494();
void Func_02004496();
void Func_020044a2();
void Func_020044a6();
void Func_020044ea();
void Func_020044f4();
void Func_020044fa();
void Func_02004518();
void Func_02004528();
void Func_0200453a();
void Func_02004542();
void Func_02004556();
void Func_02004574();
void Func_0200458e();
void Func_02004590();
void Func_02004592();
void Func_02004598();
void Func_020045c6();
void Func_020045c8();
void Func_020045d4();
void Func_020045e0();
void Func_020045ec();
void Func_020045f8();
void Func_02004604();
void Func_02004612();
void Func_02004624();
void Func_02004628();
void Func_02004646();
void Func_0200464a();
void Func_02004654();
void Func_02004658();
void Func_0200465a();
void Func_02004668();
void Func_0200467a();
void Func_0200467e();
void Func_020046a2();
void Func_020046a2_a();
void Func_020046a2_b();
void Func_020046a2_c();
void Func_020046aa();
void Func_020046b8();
void Func_020046bc();
void Func_020046c0();
void Func_020046c0_a();
void Func_020046c4();
void Func_020046c8();
void Func_020046c8_a();
void Func_020046d6();
void Func_020046da();
void Func_020046e2();
void Func_020046e4();
void Func_020046ea();
void Func_020046ec();
void Func_020046fc();
void Func_02004700();
void Func_0200470e();
void Func_02004718();
void Func_02004752();
void Func_0200475a();
void Func_0200475a_a();
void Func_02004762();
void Func_0200476a();
void Func_02004772();
void Func_0200477a();
void Func_02004782();
void Func_0200478a();
void Func_02004790();
void Func_02004790_a();
void Func_02004798();
void Func_020047c4();
void Func_020047cc();
void Func_020047d4();
void Func_020047dc();
void Func_020047ea();
void Func_020047f2();
void Func_020047fa();
void Func_02004802();
void Func_0200480a();
void Func_0200481a();
void Func_02004826();
void Func_02004832();
void Func_0200483c();
void Func_02004846();
void Func_02004850();
void Func_0200485a();
void Func_0200485c();
void Func_02004864();
void Func_0200486e();
void Func_02004878();
void Func_02004882();
void Func_0200488c();
struct FieldActor *Func_02004892(s32 actor);
void Func_02004896();
void Func_020048a0();
struct FieldActor *Func_020048a2(s32 actor);
void Func_020048aa();
void Func_020048b4();
void Func_020048be_a();
void Func_020048c2();
void Func_020048c8();
void Func_020048d2();
void Func_020048dc();
void Func_020048e0();
void Func_020048e6();
void Func_020048ea();
void Func_020048f8();
void Func_020048fc(s32 actor, const u8 *table);
void Func_02004922();
void Func_02004932(s32 actor, const u8 *table);
void Func_0200493c();
void Func_0200496c_a();
void Func_02004972_a();
void Func_0200498e();
void Func_0200499a();
void Func_020049e2();
void Func_020049fa();
void Func_02004a60();
void Func_02004a9a();
void Func_02004aa6();
s32 Main_08009020(struct FieldSprite *sprite, s32 animation);
void Main_080001b8(s32 block);

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* Exact complete owner [020017c8,020022c8): 2816 bytes, pools included.
 * 2026-09-27 Sol H1: FieldActor lookup results own x/z, priority_flags and
 * motion_flags. The nullable lookup has a separate actor lifetime from the
 * earlier child records. This typed record model resolves all 639 differing
 * halfwords / 185 edits, preserving callback target reloads and table r5.
 * Complete normalized diff and literal pools read; candidate/reference cmp
 * is exact. This is not a constant spelling or register-allocation sweep.
 * Reuse sun-west's canonical s32 Party_GiveItem interface for both rewards;
 * that independently proven ABI correction is not the matching lever.
 * Historical baseline and closed axes:
 * Typed callback inline scopes reproduce the prior baseline bytes exactly;
 * they restore target-ID reloads and the r5 table but lose the direct-call
 * trial's shared-constant lifetimes (16 wrong instructions in 671ed82f4).
 * The compiler's cross-call target pseudo, not pointer qualifiers alone,
 * changes the saved-register interference. Inline scopes are ruled out. */

/* Site-resolved aliases use this overlay's runtime veneers and local helpers.
 * The scene moves the actor groups before restoring the shared scene work. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02000940(s32 (*f)(), s32 a0)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200ac00[];
    void Scene_SetPosition();
    void HaidiaIe_RunScene015B4();
    void SceneState_SetValues352_365_2116_2117_40();
    void Scene_RunExtendedActorSequence();

    return f(a0);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_0200ac00[];

    f(a0, a1, a2, a3);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_020015b4(void (*f)(), s32 a0)
{
    s32 Scene_SetPosition();

    f(a0);
}

/* Call forms shared with the independently reconstructed scene scripts. */
static __inline__ void Call1_020017c8(void (*f)(), s32 a0)
{
    extern const u8 Data_0200ac00[];

    f(a0);
}

static __inline__ struct FieldActor *Pointer1(struct FieldActor *(*f)(s32), s32 a0)
{
    extern const u8 Data_0200ac00[];

    return f(a0);
}

/* FAKEMATCH: separate inline argument scopes preserve per-call target loads. */
static __inline__ void Callback3(void (*f)(s32, s32, const u8 *), s32 actor, s32 target, const u8 *table)
{
    extern const u8 Data_0200ac00[];

    f(actor, target, table);
}

enum ValeMessage {
    MSG_PARTY_PP_RESTORED = 0x974,
    MSG_CAN_I_USE_PSYNERGY = 0xea8,
    MSG_I_HAVE_SOME_PSYNERGY_LEFT = 0xeae,
    MSG_BE_SURE_TO_HELP_GARCIA = 0xeb1,
    MSG_MEDITATE_ON_MT_ALEPH_DAILY = 0xf3c,
    MSG_A_DIFFICULT_TIME_THREE_YEARS_AGO = 0xf3f,
    MSG_DID_THE_TRAVELERS_MEET_THE_MAYOR = 0xf44,
    MSG_HAVE_I_SHOWN_YOU_MY_ABILITY = 0xf48,
    MSG_BEHOLD_THE_POWER_OF_PSYNERGY = 0xf4b,
    MSG_CHECKED_THE_PSYNERGY_STONE = 0x111f,
    MSG_YOU_SAW_THE_WISE_ONE = 0x1191,
    MSG_THE_STONE_FELL_ON_THE_HUT = 0x11c7,
    MSG_YOU_CAME_BACK_HOME = 0x1be3,
    MSG_THIS_IS_VALE = 0x1be4,
    MSG_ANYTHING_INTERESTING_ON_YOUR_TRIP = 0x1be8,
    MSG_THE_PSYNERGY_STONE_IS_GONE = 0x1c94
};

LAYOUT_OFFSET_GUARD(PairSprite_Detail, struct PairSprite, detail, 0x28);

LAYOUT_OFFSET_GUARD(PairObject_Parent, union PairObject, link.parent, 0x68);

s32 Object_UpdateFacingTowardTarget(struct FacingObject *object)
{
    s32 facing_delta;
    u16 old_facing;
    s32 target_facing;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8)(0xFE & object->facing_flags);
        target_facing = (u16)CalculateFacingAngle(target->position_z - object->position_z, target->position_x - object->position_x);
        old_facing = object->facing;
        facing_delta = (s16)(target_facing - old_facing);
        if (facing_delta != 0) {
            if (facing_delta > 0x1000) {
                facing_delta = 0x1000;
            }
            /* The loader relocates the stored pool word to -0x1000. */
            if (facing_delta < -0x1000) {
                facing_delta = -0x1000;
            }
            object->facing = (u16)(old_facing + facing_delta);
        }
    }
    return 1;
}

/*
 * Table getter for resource_374. The owner at 0x02000088 is eight bytes and
 * includes its one pool word at 0x0200008c: the pc-relative load reads that
 * word, so the word belongs to this owner. The word is an address returned
 * without being dereferenced. Many getters share this body, but each returns
 * a different address.
 */
u8 *SceneData_GetTableAfa0(void)
{
    return (u8 *)0x0200afa0;
}

s32 Func_02000090(void)
{
    return 0;
}

void *SceneData_SelectTableByFlag834(void)
{
    if (GameFlag_IsSet(0x834) != 0) {
        return Data_0200b144;
    }
    return Data_0200b108;
}

void *SceneData_SelectTableByFlags834And87a(void)
{
    if (GameFlag_IsSet(0x834) != 0) {
        return Data_0200b380;
    }
    if (gGameState.entrance == 12) {
        return Data_0200b560;
    }
    if (GameFlag_IsSet(0x87a) != 0) {
        return Data_0200b7d0;
    }
    return Data_0200b170;
}

void Scene_CheckPsynergyStone(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_CHECKED_THE_PSYNERGY_STONE, 1);
    Audio_PlayCue(126);
    Func_02002962(0x3e7, 0);
    Event_Wait(10);
    Message_ShowCentered(MSG_PARTY_PP_RESTORED, 1);
    Func_020027a4();
    GameFlag_Clear(322);
    Event_End();
}

void *SceneData_SelectTableByFlags87a_815_834(void)
{
    if (GameFlag_IsSet(0x87a) != 0) {
        return Data_0200bcec;
    }
    if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        return Data_0200bb3c;
    }
    if (gGameState.entrance == 12) {
        return Data_0200bb30;
    }
    if (GameFlag_IsSet(0x834) != 0) {
        return Data_0200ba64;
    }
    return Data_0200b938;
}

void Villager_AskAboutMeditation(void)
{
    Event_Begin();
    Event_SetMessage(MSG_MEDITATE_ON_MT_ALEPH_DAILY);
    Actor_FaceEachOther(23, ACTOR_PARTY_LEADER, 2);
    Event_AskYesNo(23, 0);
    Event_End();
}

void Villager_RecallThreeYearsAgo(void)
{
    Event_Begin();
    Event_SetMessage(MSG_A_DIFFICULT_TIME_THREE_YEARS_AGO);
    Actor_FaceEachOther(24, ACTOR_PARTY_LEADER, 2);
    Event_AskYesNo(24, 0);
    Event_End();
}

void Villager_AskAboutTheTravelers(void)
{
    Event_Begin();
    Event_SetMessage(MSG_DID_THE_TRAVELERS_MEET_THE_MAYOR);
    Actor_FaceEachOther(15, ACTOR_PARTY_LEADER, 2);
    Event_AskYesNo(15, 0);
    Event_End();
}

void Villager_ShowOffPsynergy(void)
{
    u32 i;
    s32 base5_1197;
    s32 base7_0;
    u8 *p6;

    Event_Begin();
    if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        base5_1197 = (s32)LinkedMessage_ArentYouWorriedAboutCrossing;
        Event_SetMessage(base5_1197);
        if (GameFlag_IsSet(2) != 0) {
            bump_step(1);
        }
        if (GameFlag_IsSet(3) != 0) {
            bump_step(1);
        }
        Event_OpenMessage(17, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_SetMessage((base5_1197 + 3));
        } else {
            Event_SetMessage((base5_1197 + 4));
        }
        Event_ShowMessage(17, 0);
    } else {
        p6 = *(volatile s32 *)(*(volatile s32 *)0x03001e70);
        Event_SetMessage(MSG_HAVE_I_SHOWN_YOU_MY_ABILITY);
        Actor_FaceEachOther(17, ACTOR_PARTY_LEADER, 0);
        Event_AskYesNo(17, 0);
        Event_Wait(20);
        Actor_StartRepeatedMotion(17, 2);
        Event_Wait(15);
        SceneState_ApplyPair140And0();
        base7_0 = 0;
        for (i = 0; i < 40; i++) {
            OverlayObject_UpdateOnFrameBit1(((s32 (*)())Func_020029c2_a)(17));
            Task_Wait(1);
        }
        Value2(Func_0200290c, 0x200a591, 0xc80);
        Audio_PlayCue(107);
        for (i = 0; i != 180; i++) {
            if (Value2(Func_0200290c_a, i, 10) == 0) {
                if ((1 & base7_0) != 0) {
                    *(volatile s32 *)p6 = *(volatile s32 *)p6 - 0x10000;
                } else {
                    *(volatile s32 *)p6 = *(volatile s32 *)p6 + 0x10000;
                }
                base7_0 = (base7_0 + 1);
            }
            Event_Wait(1);
        }
        Audio_PlayCue(0x121);
        Call1(Func_0200295a, 0x200a591);
        Task_Wait(1);
        FieldScene_Forward4dac();
        Actor_SetChildValue(17, 0);
        Event_Wait(40);
        Event_SetMessage(MSG_BEHOLD_THE_POWER_OF_PSYNERGY);
        Event_ShowMessage(17, 0);
    }
    Event_End();
}

void SceneDialogue_RunFlagGatedMessageStep(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x87a) != 0) {
        Event_SetMessage(MSG_ANYTHING_INTERESTING_ON_YOUR_TRIP);
        Event_OpenMessage(15, 0);
        if (Event_ChooseYesNo(0, 0) == 1) {
            Event_ShowMessage(15, 0);
        } else {
            u8 *p = *(u8 **)0x03001ebc;
            *(u16 *)(p + 472) = *(u16 *)(p + 472) + 1;
            Event_AskYesNo(15, 0);
        }
    } else if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        Event_SetMessage(MSG_YOU_SAW_THE_WISE_ONE);
        Event_AskYesNo(11, 0);
    } else {
        Event_SetMessage(MSG_CAN_I_USE_PSYNERGY);
        Event_AskYesNo(11, 0);
    }
    Event_End();
}

void Scene_StoneFellOnTheHut(void)
{
    Event_Begin();
    Actor_SetAnimation(26, 1);
    Actor_FaceActor(26, ACTOR_PARTY_LEADER, 20);
    Actor_FaceActor(26, 21, 40);
    Event_SetMessage(MSG_THE_STONE_FELL_ON_THE_HUT);
    Func_0200273a(26, 20);
    Camera_SetSpeed(0x19999, 0x3333);
    Camera_MoveTo(0x1510000, -1, 0x1100000, 1);
    Event_Wait(20);
    Actor_RunRepeatedMotion(26, 2);
    Event_Wait(20);
    Actor_FaceActor(26, ACTOR_PARTY_LEADER, 10);
    Func_02002778(26, 40);
    Actor_EnableActionCallback(26, 2);
    Event_End();
}

void FieldScene_RunMiddleAuxiliarySequence(void)
{
    u32 i;
    s32 p8;
    u8 *rec8;
    s32 record;
    s32 v2;

    Event_Begin();
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 82, 0x2f8);
    Actor_FaceEachOther(15, ACTOR_PARTY_LEADER, 30);
    Event_SetMessage(MSG_I_HAVE_SOME_PSYNERGY_LEFT);
    Func_020027c8(15, 20);
    Value3(SceneActor_SetPairZeroAndValue, 15, 0xa000, 20);
    Actor_SetAttachedEffect(15, 0x102);
    Event_Wait(20);
    SceneState_ApplyPair140And0();
    for (i = 0; i < 40; i++) {
        OverlayObject_UpdateOnFrameBit1(Func_02002bec_a(15));
        Task_Wait(1);
    }
    Value2(Func_02002b36, 0x200a581, 0xc80);
    Value2(Func_02002b40, 0x200a5a1, 0xc80);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 10);
    rec8 = Value1(Func_02002c22, 20);
    v2 = rec8[85];
    rec8[85] = 0;
    p8 = v2;
    for (i = 0; i < 40; i++) {
        *(s32 *)(rec8 + 12) += 0x1800;
        Task_Wait(1);
    }
    rec8[85] = p8;
    Call1(Func_02002b8a, 0x200a581);
    Call1(Func_02002b90, 0x200a5a1);
    Task_Wait(1);
    Audio_PlayCue(161);
    Actor_SetChildValue(15, 0);
    Actor_SetChildValue(20, 0);
    Event_Wait(40);
    FieldScene_Forward4dac();
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 15, 30);
    Event_ShowMessage(15, 0);
    Event_End();
}

void SceneDialogue_ShowLineEB1OrEB0(void)
{
    Event_Begin();
    Actor_FaceEachOther(16, ACTOR_PARTY_LEADER, 10);
    if (GameFlag_IsSet(0x840) != 0) {
        Event_SetMessage(MSG_BE_SURE_TO_HELP_GARCIA);
        Event_ShowMessage(16, 0);
    } else {
        Event_SetMessage((s32)&LinkedMessage_JasmineWentOffWay);
        Event_ShowMessage(16, 0);
    }
    Event_End();
}

void Villager_WelcomeBack(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x302) != 0) {
        Event_SetMessage(MSG_THIS_IS_VALE);
    } else {
        Event_SetMessage(MSG_YOU_CAME_BACK_HOME);
        GameFlag_Set(0x302);
    }
    Event_ShowMessage(11, 0);
    Event_End();
}

void Scene_PsynergyStoneIsGone(void)
{
    struct Obj *p = Func_02002d3e(21);
    Event_Begin();
    p->f38 = 0x80000000;
    p->f3c = 0x80000000;
    p->f40 = 0x80000000;
    Actor_SetAnimation(21, 1);
    Actor_Stop(21);
    Actor_ShowEmote(21, 256, 40);
    p->f06 = 0xb000;
    Event_Wait(20);
    Actor_StartRepeatedMotion(21, 2);
    Event_SetMessage(MSG_THE_PSYNERGY_STONE_IS_GONE);
    Event_ShowMessageAndWait(21, 0, 40);
    Actor_FaceActor(21, ACTOR_PARTY_LEADER, 20);
    Actor_StartRepeatedMotion(21, 2);
    Event_ShowMessage(21, 0);
    GameFlag_Set(0x306);
    Actor_Stop(21);
    Task_Wait(1);
    Actor_EnableActionCallback(21, Data_0200ae34);
    Event_End();
}

void SceneState_SetWork1c0AndRun(s32 no)
{
    u8 *p;
    if (GameFlag_IsSet(0x834) != 0) {
        Func_02002f20();
    }
    p = *(u8 **)0x03001ebc;
    *(s32 *)(p + 0x1c0) = 0x100;
    *(s32 *)(p + 0x1c8) = 16;
    Event_RequestExit(no);
}

/* Runs four fixed scene-helper calls in sequence, one of them passed the
 * address of Value_0200beb4 as its first argument. */
void FieldScene_RunSupplementalSequenceOne(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200beb4, 44, 7);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 248, 0x117);
    Call1(SceneState_SetWork1c0AndRun, 1);
}

/* Sets step 188, then runs a pair of 6-argument setup calls for indices 0
 * and 2 sharing the same trailing four values, followed by a pair of
 * 3-argument calls sharing the same leading two arguments, and a closing
 * 1-argument call. */
void FieldScene_RunSupplementalSequenceTwo(void)
{
    Audio_PlayCue(188);
    Map_CopyCellsTo(0, 63, 51, 8, 2, 2);
    Task_Wait(10);
    Map_CopyCellsTo(2, 63, 51, 8, 2, 2);
    Task_Wait(10);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 352, 306);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 352, 296);
    Call1(SceneState_SetWork1c0AndRun, 2);
}

/* Runs four fixed scene-helper calls in sequence, one of them passed the
 * address of Value_0200beb4 as its first argument. */
void FieldScene_RunSupplementalSequenceThree(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200beb4, 43, 15); /* main:08009178 */
    Actor_WalkTo(ACTOR_PARTY_LEADER, 230, 0x197);
    Call1(SceneState_SetWork1c0AndRun, 3);
}

/* Runs four scene calls in sequence: a single-argument call, a call that
 * passes the address of Value_0200beb4 with two more values, a call that
 * passes 0, 374, and 0x1a3, and a final single-argument call. */
void FieldScene_RunSupplementalSequenceFour(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200beb4, 52, 18); /* main:08009178 */
    Actor_WalkTo(ACTOR_PARTY_LEADER, 374, 0x1a3); /* object_id 0, x 374, z 0x1a3 */
    Call1(SceneState_SetWork1c0AndRun, 4);
}

/* Runs a fixed sequence of four scripted calls: one keyed off Value_0200beb4
 * with two small numeric arguments, one with a 0x222 argument, and two plain
 * single-argument calls. */
void FieldScene_RunSupplementalSequenceFive(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200beb4, 41, 32); /* main:08009178 */
    Actor_WalkTo(ACTOR_PARTY_LEADER, 200, 0x222);
    Call1(SceneState_SetWork1c0AndRun, 5);
}

/* Runs four scripted calls with fixed literal arguments: a single-argument
 * call, a 3-argument call whose first argument is the address of
 * Value_0200beb4, another 3-argument call, and a closing single-argument
 * call. */
void FieldScene_RunSupplementalSequenceSix(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200beb4, 35, 36); /* main:08009178 */
    Actor_WalkTo(ACTOR_PARTY_LEADER, 102, 0x263); /* object_id 0, x 102, z 611 */
    Call1(SceneState_SetWork1c0AndRun, 6);
}

/* Runs four scripted scene calls in sequence, passing a byte's address and a
 * handful of small immediate constants to each. */
void FieldScene_RunSupplementalSequenceSeven(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200beb4, 51, 39); /* main:08009178 */
    Actor_WalkTo(ACTOR_PARTY_LEADER, 358, 0x29e);
    Call1(SceneState_SetWork1c0AndRun, 7);
}

void FieldScene_RunStep7BThen8(void)
{
    Audio_PlayCue(123);
    SceneState_SetWork1c0AndRun(8);
}

void SceneState_ApplyFlag815Branch(void)
{
    if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        Audio_PlayCue(123);
        SceneState_SetWork1c0AndRun(10);
    }
}

void SceneState_ApplyFlag90b(void)
{
    GameFlag_Set(0x90b);
}

void SceneState_ApplyFlag90c(void)
{
    GameFlag_Set(0x90c);
}

void SceneState_ApplyFlag90d(void)
{
    GameFlag_Set(0x90d);
}

s32 HaidiaIe_RestoreEntryState(void)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200ac00[];
    void Scene_SetPosition();
    void HaidiaIe_RunScene015B4();
    void SceneState_SetValues352_365_2116_2117_40();
    void Scene_RunExtendedActorSequence();

    u32 i;
    s32 record;
    s32 v5;

    if (Value1_02000940(Engine_GameFlagIsSet, 0x90b) != 0) {
        Scene_SetPosition(8, 0, 0);
    }
    if (Value1_02000940(Engine_GameFlagIsSet, 0x90c) != 0) {
        Call3(Scene_SetPosition, 9, 0, 0);
    }
    if (Value1_02000940(Engine_GameFlagIsSet, 0x90d) != 0) {
        Scene_SetPosition(10, 0, 0);
    }
    switch (Data_02000240_t[225][0]) {
    case 98:
        Scene_SetFlag(32);
        Engine_EventRequestExit(50);
        return 0;
    case 99:
        SceneState_SetValues352_365_2116_2117_40();
        return 0;
    case 97:
        HaidiaIe_RunScene015B4();
        return 0;
    }
    v5 = 192;
    record = Scene_GetActorAddress(8);
    *(s32 *)(record + 28) = (v5 << 9);
    record = Value1_02000940(Scene_GetActorAddress, 9);
    *(s32 *)(record + 28) = (v5 << 9);
    record = Value1_02000940(Scene_GetActorAddress, 10);
    *(s32 *)(record + 28) = (v5 << 9);
    if (Value1_02000940(Engine_GameFlagIsSet, 0x87a) != 0) {
        Call6(Engine_MapCopyCellsTo, 97, 2, 80, 5, 2, 2);
        Call6(Engine_MapCopyCellsTo, 42, 53, 42, 54, 3, 1);
        Engine_MapRedraw();
        Engine_TaskWait(1);
    } else {
        if (Value1_02000940(Engine_GameFlagIsSet, 0x834) != 0) {
            Main_0808a2c0();
            Main_0808a2c8();
            Call6(Scene_CopyCellAttributes, 21, 38, 1, 1, 18, 41);
            record = Value1_02000940(Engine_GameFlagIsSet, 0x840);
            if (record == 0) {
                goto L_02000aaa;
            }
            Scene_SetPosition(17, 0, 0);
            Scene_SetPosition(18, 0, 0);
            Call3(Main_0808a168, 19, 0x10000, (s32)Data_0200ac00);
        } else {
            if (Value1_02000940(Engine_GameFlagIsSet, 0x815) != 0) {
                Call3(Scene_SetPosition, 16, 0xb40000, 0x2380000);
                Call6(Engine_MapCopyCellsTo, 92, 2, 80, 5, 2, 2);
                Call6(Engine_MapCopyCellsTo, 42, 53, 42, 54, 3, 1);
                Engine_MapRedraw();
                Engine_TaskWait(1);
            }
        }
        L_02000aaa:;
        if (Data_02000240_t[225][0] == 12) {
            Scene_RunExtendedActorSequence();
        } else {
            if (Value1_02000940(Engine_GameFlagIsSet, 0x834) != 0) {
                record = Scene_GetActorAddress(20);
                *(s32 *)(record + 24) = 0x4ccc;
                *(s32 *)(record + 28) = 0x4ccc;
                record = Scene_GetActorAddress(20);
                Engine_ActorSetSpriteFlags(record, 0);
                record = Scene_GetActorAddress(21);
                *(s32 *)(record + 24) = 0x9999;
                *(s32 *)(record + 28) = 0x9999;
                Engine_ActorSetAnimation(13, 5);
            } else {
                if (Value1_02000940(Engine_GameFlagIsSet, 0x815) != 0) {
                    Call3(Scene_SetPosition, 21, 0x14b0000, 0xf90000);
                    record = Scene_GetActorAddress(21);
                    Engine_ActorSetSpriteFlags(record, 0);
                }
            }
            if (Value1_02000940(Engine_GameFlagIsSet, 0x840) != 0) {
                Scene_SetPosition(26, 0, 0);
                Scene_SetPosition(22, 0, 0);
            }
            if (Data_02000240_t[225][0] == 19) {
                HaidiaIe_RunScene015B4();
            } else {
                if (Value1_02000940(Engine_GameFlagIsSet, 0x834) != 0) {
                    record = Value1_02000940(Engine_GameFlagIsSet, 0x842);
                    if (record == 0) {
                        goto L_02000b6a;
                    }
                    HaidiaIe_RunScriptScene();
                } else {
                    L_02000b6a:;
                    if (Value1_02000940(Engine_GameFlagIsSet, 0x834) != 0) {
                        Engine_EventOpenScreen();
                        Engine_EventWaitForScreen();
                        Main_0808a2e0();
                    }
                }
            }
        }
    }
    L_02000b80:;
    return 0;
}

/* Runs once flag 0x834 is set, until this event sets flag 0x840 at its end.
 * The dialogue starts at message 0xeb6, urging the party to aid the elders,
 * while the actors move, turn and animate around it. After the prompt an
 * answer of 1 shows the next reply and skips the one after it; any other
 * answer skips straight to that second reply. */
void FieldScene_RunElderAidEvent(void)
{
    extern u8 Data_0200ac00[];

    s32 record;
    s32 skip_reply = 0;
    s32 unk;

    if (Value1(Func_02003238, 0x834) != 0 && Value1(Func_02003246, 0x840) == 0) {
        Func_02003278();
        Call2(Func_02003390, 0x19999, 0x3333);
        Camera_MoveTo(0xc50000, -1, 0x3000000, 1);
        Func_020033b6();
        Call1(Func_02003364, 0xeb6);
        Func_02003344(19, 2);
        Call3(Func_0200338e, 0x4013, 0, 10);
        Call3(Func_020032ec, 0, 0x10000, 0x8000);
        Call3(Func_020032fa, 25, 0x10000, 0x8000);
        Call3(Func_0200333c, 0, 179, 0x315);
        record = Value1(Func_02003302, 0);
        if (record != 0) {
            Func_02003360(25, *(s32 *)(record + 8), *(s32 *)(record + 16));
        }
        Call3(Func_0200335c, 25, 179, 0x324);
        Func_020033ae(0, 25, 40);
        Func_020033f8(0, 0, 0);
        Actor_FaceDirection(25, 0, 0);
        Actor_SetAnimation(17, 3);
        Actor_SetAnimationAndWait(18, 3);
        Func_020033dc(17, 18, 0);
        Func_0200332a(20);
        Func_020033d2(17, 1);
        Call3(Func_02003424, 0x4011, 0, 10);
        Func_020033cc(18, 3);
        Func_02003436(18, 0, 10);
        Func_02003450(17, 0, 0);
        Call3(Func_0200345c, 18, 0xf000, 10);
        Func_020033fc(19, 3);
        Call3(Func_0200345e, 0x4013, 0, 10);
        Call3(Func_020033b8, 17, 0x19999, 0xcccc);
        Call3(Func_020033c2, 18, 0x19999, 0xcccc);
        Func_020033d4(17, (s32)Data_0200aef0);
        Event_Wait(20);
        Func_020033e2(18, (s32)Data_0200aef0);
        Call3(Func_020034a6, 0, 0xc000, 0);
        Call3(Func_020034b2, 25, 0xc000, 60);
        Value2(Engine_ActorEnableActionCallback, 0, (s32)Data_0200af50);
        Call2(Func_02003422, 25, (s32)Data_0200af78);
        Func_020033d0(20);
        Func_020034d2(0, 0, 0);
        Func_020034dc(25, 0, 10);
        Func_020034cc(25, 0);
        Call3(Func_020034f0, 19, 0x8000, 0);
        Value3(Func_02003050, 26, 0x6000, 20);
        Actor_RunRepeatedMotion(26, 2);
        Func_02003048(26, 10);
        Func_020034a4(0, 3);
        Actor_SetAnimationAndWait(25, 3);
        Func_0200342a(20);
        Func_020034da(19, 2);
        Value2(Func_02003512, 0x4013, 0);
        if (Value2(Func_0200346a, 0, 0) == 1) {
            skip_reply = 1;
            Func_020034d8(19, 4);
        } else {
            Func_0200350e(19, 3);
            *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        }
        Call2(Func_0200357e, 0x4013, 0);
        if (skip_reply != 0) {
            *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        }
        unk = 0x4000;
        Func_0200310c(22, 0x4000, 30);
        Func_020035a8(22, 0);
        Call3(Func_020035dc, 19, 0x100, 0);
        Call3(Func_020035e8, 26, 0x100, 0);
        Call3(Func_020035f4, 0, 0x100, 0);
        Call3(Func_02003600, 25, 0x100, 40);
        Call3(Func_020035fc, 19, 0xa000, 0);
        Call3(Func_02003608, 26, 0xa000, 0);
        Call3(Func_02003618, 0, 0xe000, 0);
        Func_02003176(25, 0xe000, 10);
        Call2(Func_0200364a, 0x13333, 0x2666);
        Call4(Func_02003662, 0xd70000, -1, 0x2f60000, 1);
        Func_0200366e();
        Call2(Func_02003666, 0xcccc, 0x1999);
        Call4(Func_0200367e, 0xcd0000, -1, 0x30a0000, 1);
        Actor_EnableActionCallback(22, (s32)Data_0200a874);
        Func_020035b4(22);
        Value3(Func_020031c4, 22, 0x2000, 60);
        Func_02003628(19, 2);
        Func_020031bc(19, 10);
        Func_02003620(22, 3);
        Func_020031cc(22, 20);
        Func_02003630(19, 3);
        Event_Wait(10);
        Func_020031fe(19, unk, 30);
        Func_020031ee(unk + 19, 10);
        Func_02003212_a(26, 0xe000, 30);
        Func_02003660(26, 3);
        Func_02003226(19, 0x8000, 30);
        Func_0200368a(19, 2);
        Func_0200321e(unk + 19, 10);
        Actor_FaceEachOther(ACTOR_PARTY_LEADER, 25, 40);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
        Func_02003254(25, 0, 20);
        Func_0200325e(26, 0x8000, 30);
        Func_020036aa(26, 3);
        Func_02003256(26, 30);
        Value3(Func_0200327a, 26, 0xc000, 30);
        Func_020036c6(26, 3);
        Actor_SetAnimationAndWait(22, 3);
        Actor_SetAnimation(25, 2);
        record = Value1(Func_0200367c, 0);
        if (record != 0) {
            Func_020036be(25, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove(25);
        Actor_SetPosition(25, 0, 0);
        Func_020036fe(26, 2);
        record = Value1(Func_020036ac_a, 0);
        if (record != 0) {
            Actor_SetDestination(26, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_0200370c(26);
        Actor_SetPosition(26, 0, 0);
        Func_0200372e(22, 2);
        record = Value1(Func_020036dc_a, 0);
        if (record != 0) {
            Actor_SetDestination(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_0200373c(22);
        Func_0200374e(22, 0, 0);
        Call3(Func_020037a2, 19, 0x10000, (s32)Data_0200ac00);
        Call1(Func_020036ca, 0x840);
        Event_End();
    }
}

/* NONMATCHING: 496 of 496 bytes, 11 halfword edits (2026-09-24). Script call run;
 * 0x8017 and 0x2018 are shared constants whose registers (r6/r8) and pool
 * order still differ from the reference. */
void HaidiaIe_RunScriptScene(void)
{
    u8 *rec7;

    rec7 = Value0(Func_02003850);
    Func_0200372e_a();
    Call4(Func_02003858, -1, -1, -1, 0);
    Func_02003696(1);
    Call4(Func_02003870, 0x400000, 0x900000, 0x15e0000, 0);
    Func_020036f4();
    Func_020036b2(1);
    Func_0200377a(1, 0);
    Func_020038e6();
    Func_02003924(17);
    Func_020038e8();
    Call3(Func_020037fc, 23, 0x690000, 0x10b0000);
    Func_020036da(1);
    Call3(Func_020037c4, 0, 0x13333, 0x9999);
    Call3(Func_02003806, 0, 93, 0x157);
    Call1(Func_0200386c, 0xed6);
    Func_02003884(23, 0);
    Func_02003962(61);
    rec7[85] = 0;
    Call2(Func_020038d4, 0x30000, 0x6000);
    Call4(Func_020038ec, 0x6d0000, 0xb00000, 0x1190000, 1);
    Func_020038f8();
    Func_020037d6(40);
    Call3(Func_02003864, 24, 0x870000, 0xb10000);
    Call3(Func_02003826, 24, 0xcccc, 0x6666);
    Call3(Func_02003862, 24, 126, 0x102);
    Func_02003800(40);
    Call3(Func_02003904, 23, 0xd000, 0);
    Func_0200388a(24);
    Func_020038a2(24, 1);
    Value3(Func_02003472, 24, 0x7000, 10);
    Func_020038be(23, 3);
    Func_020038c6(24, 4);
    Call2(Func_02003920, 0x2018, 0);
    Func_020038f2(23, 2);
    Func_02003488(0x8017, 30);
    Func_020034aa(24, 0xb000, 20);
    Func_0200349e(0x2018, 10);
    Value3(Func_020034c0, 23, 0xb000, 40);
    Func_0200395c(0x8017, 0);
    Func_02003914(24, 4);
    Func_0200396c(0x2018, 0);
    Value3(Func_020034e4, 23, 0xf000, 10);
    Func_02003948(23, 2);
    Func_02003988(0x8017, 0);
    Value3(Func_02003500, 24, 0x6000, 20);
    Func_0200394c(24, 3);
    Func_020038c2(20);
    Func_020034fe(0x2018, 20);
    Func_020024ae();
    Func_020038e2();
}

/* Stages the two moving actors around actor 25, then advances the area's
 * scene state after the final message and sound cue. */
void FieldScene_RunGroupChoreography(void)
{
    extern struct SceneWork Data_02000240;
    extern u8 Data_0200ac00[];

    s32 record;
    s32 walk_speed;
    s32 turn_speed;
    s32 approach_speed;

    Func_020039d4(25, 15);
    record = Func_0200394a(25);
    Func_020038e0(record, 0);
    Call3(Func_020039aa, 25, 0, 0x14b0000);
    Func_02003888(1);
    Call2(Func_02003a18, 0x1019, 0);
    Call3(Func_02003a4c, 23, 0x100, 0);
    Call3(Func_02003a58, 24, 0x100, 40);
    Func_020039da(25, 0, 0);
    Call3(Func_02003a5e, 23, 0x5000, 0);
    walk_speed = 0x5000;
    Func_020035c2(24, walk_speed, 40);
    Call2(Func_02003a9a, 0x18000, 0x3000);
    Call4(Func_02003ab2, 0x590000, 0xb00000, 0x1390000, 1);
    Func_02003abe();
    Func_0200399c(40);
    Call3(Func_02003aa0, 23, 0xe000, 0);
    Value3(Func_02003600_a, 24, 0x7000, 40);
    Camera_SetSpeed(0xcccc, 0x1999);
    Call4(Func_02003aec, 0x640000, 0x900000, 0x14d0000, 1);
    Call3(Func_02003a12, 23, 0x10000, 0x8000);
    Call3(Func_02003a20, 24, 0x10000, 0x8000);
    Call3(Func_02003a5a, 23, 105, 0x149);
    Func_020039f8(10);
    Actor_WalkTo(24, 124, 0x149);
    Func_02003a80(23);
    Actor_SetAnimation(23, 1);
    Func_02003b12(23, walk_speed, 0);
    Actor_WaitForMove(24);
    Func_02003ab0(24, 1);
    Value3(Func_02003b2a, 24, walk_speed, 0);
    Func_02003afa(25, 0);
    record = Func_02003a70(25);
    Func_02003a06(record, 1);
    Call3(Func_02003ad0, 25, 0, 0x14b0000);
    Call3(Func_02003a92, 25, 0x13333, 0x9999);
    Actor_WalkToAndWait(25, 37, 0x153);
    Event_Wait(20);
    Func_02003b02(23, 3);
    Value2(Func_02003b54, 23, 0);
    Call3(Func_02003b90, 25, 0x101, 0);
    approach_speed = 0xd000;
    Value3(Func_020036de, 0, approach_speed, 10);
    Func_02003ac2(0, 0);
    Func_02003aa2(40);
    Func_02003ba6(23, 0, 0);
    turn_speed = 0x8000;
    Value3(Func_02003704, 24, turn_speed, 20);
    Func_02003b68(25, 2);
    Call2(Func_020036fc, 0x1019, 10);
    Call3(Func_02003b9a, 0, 0x10019, (s32)Data_0200ac00);
    Call3(Func_02003b4c, 25, 93, 0x169);
    Value3(Func_02003732, 25, approach_speed, 40);
    Func_02003722(25, 20);
    Func_02003b44(0);
    Func_02003bf6(23, 0, 0);
    Value3(Func_02003754, 24, turn_speed, 15);
    Func_02003b98(23, 3);
    Func_02003ba8(24, 3);
    Func_02003c1a(23, walk_speed, 0);
    Value3(Func_02003778, 24, walk_speed, 30);
    Func_02003bc4(24, 4);
    Call2(Func_02003c1c, 0x2018, 0);
    Func_02003792(0, approach_speed, 30);
    Value3(SceneActor_SetPairZeroAndValue, 0, 0x4000, 40);
    Func_02003c02(23, 2);
    Func_02003bf2(23, 3);
    ((void (*)())Event_SayThenWait)(23, 20);
    Call2(Func_02003c84, 0, 0x102);
    Call2(Func_02003c8e, 25, 0x102);
    Func_02003b84(80);
    Call2(Func_02003bcc, 24, 0x200a8e8);
    Func_02003b92(6);
    Call2(Func_02003bda, 23, 0x200a940);
    Func_02003ba0(20);
    Call2(Func_02003be8, 0, 0x200a998);
    Func_02003bae(6);
    Value2(Func_02003c0e, 25, 0x200a9f0);
    /* FAKEMATCH: the do/while loads the linked scene after the store. */
    do {
        Data_02000240.area_state = 2;
    } while (0);
    {
        const void *message = Data_00000005;

        Func_02003d12(message, 19);
        Func_02003d22(message, 19);
    }
    Func_02003d1a(12, 4);
    Call1(Func_02003bca, 0x11a);
}

void HaidiaIe_RunScene015B4(void)
{
    s32 Scene_SetPosition();

    u32 i;
    s32 record;
    s32 v5;

    Engine_AudioPlayCue(17);
    Engine_EventBegin();
    Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
    Engine_TaskWait(1);
    Call4(Engine_CameraMoveTo, 0x400000, 0x900000, 0x15e0000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Call3(Scene_SetPosition, 0, 0x300000, 0x15a0000);
    Call3(Scene_SetPosition, 25, 0x4e0000, 0x1660000);
    Call3(Scene_SetPosition, 23, 0x670000, 0x1560000);
    Call3(Scene_SetPosition, 24, 0x700000, 0x1680000);
    Call3(Engine_ActorFaceDirection, 23, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 24, 0x8000, 0);
    Engine_ActorSetAnimation(0, 16);
    record = Scene_GetActorAddress(0);
    *(s32 *)(record + 24) = -0x10000;
    record = Scene_GetActorAddress(0);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_ActorSetAnimation(25, 7);
    record = Scene_GetActorAddress(25);
    {
        u8 *motion = *(u8 **)(record + 80);
        s32 shown = 0x1555;

        *(u16 *)(motion + 30) = shown;
    }
    record = Scene_GetActorAddress(25);
    Engine_ActorSetSpriteFlags(record, 0);
    {
        u8 *work = *(u8 **)0x03001ebc;

        *(s32 *)(work + 0x1c0) = 0x100;
    }
    Engine_EventOpenScreen();
    Main_0808a2e0();
    Engine_EventWaitForScreen();
    Engine_EventWait(80);
    Call3(Engine_ActorFaceDirection, 23, 0x3000, 0);
    Call3(Engine_ActorFaceDirection, 24, 0xc000, 40);
    Scene_SetAnimationAndWait(23, 3);
    Engine_EventWait(20);
    Call2((void (*)())Scene_SetAnimationAndWait, 24, 3);
    Call3(Engine_ActorFaceDirection, 23, 0x8000, 10);
    Call3(Engine_ActorFaceDirection, 24, 0x8000, 10);
    Engine_ActorSetSpritePriority(0, 3);
    Engine_ActorSetSpritePriority(25, 3);
    Call3(Engine_ActorSetSpeed, 23, 0x26666, 0x13333);
    v5 = 128;
    record = Scene_GetActorAddress(23);
    *(s32 *)(record + 68) = 0x28f;
    *(s32 *)(record + 72) = (v5 << 8);
    Call2(Engine_ActorEnableActionCallback, 23, 0x200aa48);
    Engine_EventWait(24);
    Call3(Engine_ActorSetSpeed, 24, 0x26666, 0x13333);
    record = Scene_GetActorAddress(24);
    *(s32 *)(record + 68) = 0x28f;
    *(s32 *)(record + 72) = (v5 << 8);
    Call2(Main_0808a0b0, 24, 0x200ab2c);
    Engine_EventWait(40);
    Main_0808a2d0();
    Main_0808a2d8();
    Engine_TaskWait(20);
    Main_0808a2d8();
    Engine_TaskWait(60);
    Main_0808a2d8();
    Engine_TaskWait(20);
    Main_0808a2d0();
    Engine_EventWait(40);
    *(s32 *)((*(s32 *)0x03001ebc + 0x1c8)) = 120;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Call1_020015b4(Engine_GameFlagClear, 0x834);
    Engine_EventRequestExit(9);
    Engine_EventEnd();
}

void Scene_RunExtendedActorSequence(void)
{
    extern const u8 Data_0200ac00[];

    u8 *record;
    struct EventWork *work;
    const u8 *base5_200ac00;
    s32 v5;
    const u8 *base5_200ac90;
    s32 v6;
    const u8 *base5_200adf0;

    Func_02003e68(1);
    Func_02003e7c();
    Call6(Func_02003e38, 42, 53, 42, 54, 3, 1);
    Call4(Func_02003fb8, 0xb40000, 0x100000, 0x26a0000, 0);
    Func_02003e3c_a();
    Func_02003dfa_a(1);
    record = Func_02003ed8(22);
    Func_02003e6e((s32)record, 0);
    record = Func_02003ee4(23);
    Func_02003e7a((s32)record, 0);
    record = Func_02003ef0(24);
    Func_02003e86((s32)record, 0);
    record = Func_02003efc(25);
    Func_02003e92((s32)record, 0);
    record = Func_02003f08(26);
    Func_02003e9e((s32)record, 0);
    record = Func_02003f14(29);
    Func_02003eaa((s32)record, 0);
    Func_02003ff2(0, 1);
    Func_02003ffa(1, 1);
    Func_02004002(17, 1);
    Func_0200400a(16, 1);
    Func_02004012(15, 1);
    Call3(Func_02003f9e, 0, 0xd00000, 0x32e0000);
    Func_020040a2();
    Func_020040b6();
    Func_02003f2c(80);
    Call3(Func_0200403e, 12, 0x101, 40);
    Func_02003b92_a(12, 0x7000, 20);
    Call1_020017c8(Func_0200401c, 0x11fa);
    Func_02003b88(12, 10);
    Call3(Func_02004068, 11, 0x102, 20);
    Func_02003bbc(11, 0x1000, 10);
    Func_02003bac(11, 10);
    Func_02004010(12, 3);
    Func_02003f86(10);
    Func_02004036(11, 2);
    Func_02003bca_a(11, 10);
    Call3(Func_020040aa, 12, 0x100, 40);
    Call3(Func_02003fe8, 12, 0xcccc, 0x6666);
    Call3(Func_0200402a, 12, 184, 0x26a);
    Func_02003c10(12, 0x3000, 60);
    Func_02003c00(12, 20);
    Call3(Func_02004012_a, 11, 0x10000, 0x8000);
    Call3(Func_02004054, 11, 168, 0x26a);
    Call3(Func_02003c3c, 11, 0xf000, 10);
    Func_02004080(11, 4);
    Func_02003c34(11, 20);
    Func_02003c56(12, 0x7000, 10);
    Func_020040ba(12, 1);
    Func_020040fa(12, 0);
    Func_02004020(10);
    Call3(Func_02004062, 30, 0x26666, 0x13333);
    Call3(Func_020040b8, 30, 0x6e0000, 0x2e80000);
    Func_02003f96(2);
    Func_020040ce(30, 3);
    Call2(Func_0200408e, 30, 0x200ac14);
    Func_02004054_a(40);
    base5_200ac00 = Data_0200ac00;
    Callback3(Func_02004128, 11, 0x1001e, base5_200ac00);
    Callback3(Func_02004132, 12, 0x1001e, base5_200ac00);
    Func_020040b8_a(30);
    Func_020040c6(11);
    Func_020040cc(12);
    Func_02004082(60);
    Call3(Func_02004194, 11, 0x105, 0);
    Call3(Func_0200419e, 12, 0x105, 120);
    Func_02004198(11, 0x1000, 0);
    Func_02003cf6(12, 0x7000, 80);
    Func_02003d06(11, 0x5000, 40);
    Func_02003d10(11, 0x1000, 20);
    Func_0200415c(11, 3);
    Func_020040d2(20);
    Func_02003d28(12, 0x5000, 60);
    Func_02003d32(12, 0x3000, 40);
    Func_02003d3c(12, 0x5000, 60);
    Call3(Func_02004202, 12, 0x101, 80);
    Func_020041fc(11, 0x3000, 0);
    Call3(Func_0200417e, 12, 184, 0x276);
    Func_02004114(20);
    Func_02003d6a_a(12, 0x3000, 20);
    Func_02003d74(12, 0x5000, 20);
    Func_02003d7e(12, 0x3000, 20);
    Call3(Func_02004244, 12, 0x101, 40);
    Call3(Func_0200424e, 11, 0x101, 40);
    Call3(Func_020041c0, 11, 168, 0x276);
    Func_02004156(20);
    Func_02003dac(11, 0x3000, 40);
    Func_02003db6(11, 0x5000, 40);
    Func_02003dc0(11, 0x3000, 40);
    Call3(Func_02004286, 11, 0x101, 40);
    Func_02003dd4(11, 0x1000, 10);
    Func_02003dc4(11, 20);
    Func_02004228(12, 3);
    Func_02003dd4_a(12, 10);
    Call3(Func_020042b4, 11, 0x100, 20);
    Func_02003e02(11, 0x5000, 20);
    Func_02003e0c(11, 0x3000, 20);
    Func_02003e16(11, 0x5000, 20);
    Func_02003e20(11, 0x5000, 60);
    Func_0200426c(11, 3);
    Func_02003e18(11, 10);
    {
        struct FieldActor *actor = Pointer1(Func_0200421a, 30);

        if (actor != NULL) {
            Func_02004278(31, actor->x.fixed, actor->z.fixed);
        }
    }
    v5 = 254;
    Func_02004156_a(2);
    Func_02004234(30)->priority_flags &= v5;
    Func_02004246(31)->priority_flags &= v5;
    Func_02004326(30, 2);
    Func_0200432e(31, 2);
    Call3(Func_02004270, 31, 0x39999, 0x1cccc);
    Func_020042c8(31, 2);
    base5_200ac90 = Data_0200ac90;
    Func_0200428a(31, base5_200ac90);
    Func_02004250(20);
    Func_020042e0(30, 3);
    Call3(Func_0200429a, 30, 0x4cccc, 0x26666);
    Func_020042c2(30, base5_200ac90);
    Func_02004270_a(60);
    Func_02004320(12, 2);
    Func_02003ece(12, 0x7000, 10);
    Call2(Func_02003f08_a, 12, 10);
    Func_02004384(11, 1);
    Func_020042e2(20);
    Func_02003f38(11, 0x1000, 10);
    Func_02004384_a(11, 3);
    Func_02003f30(11, 20);
    Func_0200438c(12, 3);
    Func_0200430a(10);
    Func_020043a2(11, 3);
    Func_02004318(20);
    Call3(Func_0200435a, 11, 0x26666, 0x13333);
    Call3(Func_02004364, 12, 0x26666, 0x13333);
    Call2(Func_02004374, 11, 0x200acf8);
    Func_0200433a(10);
    Call2(Func_0200445a, 0x26666, 0x4ccc);
    v6 = 0;
    Func_02004476()->motion_flags = v6;
    Call4(Func_0200447c, 0xd70000, 0x100000, 0x3210000, 1);
    Func_02004362(10);
    Call2(Func_020043aa, 12, 0x200ad74);
    Func_020043b8(12);
    Func_02003fc6(12, 0x3000, 120);
    Func_0200442a(13, 2);
    Func_02004388(20);
    Func_02003fc6_a(13, 20);
    Func_02004496(0, 0, 0);
    Func_02003ff4(1, 0x9000, 20);
    Func_02004004(0, 0xc000, 10);
    Func_02004014(1, 0xb000, 10);
    Func_02004458(0, 3);
    Func_020043d6(10);
    Func_0200446e(1, 3);
    Func_020043e4(40);
    Func_02004494(16, 2);
    Func_020043f2(20);
    Func_0200402e(16, 10);
    Call3(Func_02004440, 16, 0x10000, 0x8000);
    Call3(Func_02004484, 16, 216, 0x320);
    Call3(Func_02004518, 16, 0x4000, 0);
    Party_GiveItem(180, 0);
    Call3(Func_020044a6, 16, 0x108, 0x320);
    Call3(Func_0200453a, 16, 0x6000, 0);
    Call3(Func_02004556, 1, 0x102, 40);
    Call3(Func_020040a6, 1, 0xf000, 10);
    Func_02004096(1, 10);
    Func_020044fa(17, 4);
    Func_020040a6_a(17, 10);
    Func_02004574(1, 0x1000, 0);
    Call3(Func_0200458e, 1, 0x103, 20);
    Func_02004528(1, 4, 60);
    Func_02004542(14, 2);
    Func_020044a2(20);
    Func_020040f8(14, 0xd000, 10);
    Func_020040e8(14, 60);
    Call3(Func_020045c8, 1, 0x102, 0);
    Call3(Func_020045d4, 16, 0x102, 0);
    Call3(Func_020045e0, 17, 0x102, 0);
    Call3(Func_020045ec, 18, 0x102, 0);
    Call3(Func_020045f8, 19, 0x102, 80);
    Call3(Func_02004604, 17, 0x100, 0);
    Func_02004138(17, 60);
    Func_020046bc(0, 17);
    Func_020046c4(1, 17);
    Call3(Func_02004590, 17, 216, 0x320);
    Call3(Func_02004624, 17, 0x4000, 0);
    Func_02004168(17, 60);
    Party_GiveItem(207, 0);
    Func_02004592(0);
    Func_02004598(1);
    Call3(Func_020045c6, 17, 0x110, 0x330);
    Call3(Func_0200465a, 17, 0x8000, 0);
    Func_02004612(1, 2);
    Func_020041c0_a(1, 0x9000, 10);
    Func_020041b0(1, 10);
    Func_0200467e(14, 0x3000, 0);
    Func_020041dc(0, 0, 10);
    Call3(Func_020046a2, 0, 0x101, 60);
    Func_0200464a(16, 1);
    Func_020041de(16, 10);
    Call3(Func_02004202_a, 1, 0xf000, 10);
    Call3(Func_020046c8, 1, 0x101, 20);
    Func_02004658(16, 4);
    Func_02004204(16, 10);
    Func_02004668(18, 3);
    Func_020046c0(18, 0);
    Func_020046e2(1, 0xd000, 0);
    Func_0200467a(18, 4);
    Func_020046da(18, 0);
    Func_020046a2_a(18, 3);
    Func_020046ea(18, 0);
    Func_020046a2_b(16, 3);
    Func_020046a2_c(19, 3);
    Func_020046aa(17, 3);
    Func_02004628(10);
    Func_020046b8(24, 3);
    Func_020046c0_a(18, 3);
    Func_020046c8_a(27, 3);
    Func_02004646(10);
    Func_020046d6(28, 3);
    Func_02004654(10);
    Func_020046e4(25, 3);
    Func_020046ec(20, 3);
    Func_020046fc(21, 3);
    Func_0200470e(15, 2, 10);
    Func_02004718(15, 4, 40);
    Func_020042bc(15, 10);
    Func_0200478a(1, 0xb000, 0);
    Func_020042e8(0, 0xc000, 20);
    Func_020042f2(15, 0xd000, 10);
    Func_020042e2_a(15, 10);
    Func_02004304(15, 0x9000, 20);
    Func_0200430e(15, 0x5000, 10);
    Func_02004752(11, 3);
    Func_0200475a(14, 3);
    Func_02004762(17, 3);
    Func_0200476a(20, 3);
    Func_02004772(23, 3);
    Func_0200477a(26, 3);
    Func_02004782(29, 3);
    Func_02004700(10);
    Func_02004790(12, 3);
    Func_02004798(15, 3);
    Func_020047c4(18, 3);
    Func_020047cc(21, 3);
    Func_020047d4(24, 3);
    Func_020047dc(27, 3);
    Func_0200475a_a(10);
    Func_020047ea(13, 3);
    Func_020047f2(16, 3);
    Func_020047fa(19, 3);
    Func_02004802(22, 3);
    Func_0200480a(25, 3);
    Func_0200481a(28, 3);
    Func_02004790_a(80);
    Func_02004832(11, 4, 0);
    Func_0200483c(14, 4, 0);
    Func_02004846(17, 4, 0);
    Func_02004850(20, 4, 0);
    Func_0200485a(23, 4, 0);
    Func_02004864(26, 4, 0);
    Func_0200486e(29, 4, 0);
    Func_02004878(12, 4, 0);
    Func_02004882(15, 4, 0);
    Func_0200488c(18, 4, 0);
    Func_02004896(21, 4, 0);
    Func_020048a0(24, 4, 0);
    Func_020048aa(27, 4, 0);
    Func_020048b4(13, 4, 0);
    Func_020048be_a(16, 4, 0);
    Func_020048c8(19, 4, 0);
    Func_020048d2(22, 4, 0);
    Func_020048dc(25, 4, 0);
    Func_020048e6(28, 4, 0);
    Call2(Func_02004826, 0x1214, 1);
    v5 = 1;
    Func_0200485c(80);
    Func_02004892(0)->priority_flags |= v5;
    {
        struct FieldActor *actor = Func_020048a2(1);
        u8 value = (u8)(v5 | actor->priority_flags);

        actor->priority_flags = value;
    }
    Call3(Func_0200498e, 0, 0x102, 0);
    Call3(Func_0200499a, 1, 0x102, 80);
    Func_020044ea(0, 0x4000, 10);
    Func_020044f4(1, 0x5000, 20);
    Call3(Func_020048ea, 0, 0xcccc, 0x6666);
    base5_200adf0 = Data_0200adf0;
    Func_020048fc(0, base5_200adf0);
    Func_020048c2(20);
    Call2(Func_020049e2, 0x6666, 0xccc);
    Call4(Func_020049fa, 0xd80000, 0x100000, 0x3890000, 1);
    Func_020048e0(20);
    Call3(Func_02004922, 1, 0xcccc, 0x6666);
    Func_02004932(1, base5_200adf0);
    Func_020048f8(60);
    work = *(struct EventWork **)Data_03001ebc;
    work->start_transition = 0x100;
    work->transition_frames = 60;
    Func_02004a9a();
    Func_02004aa6();
    Func_0200496c_a(0);
    Func_02004972_a(1);
    Func_02004a60(10);
    Func_0200493c();
}

/* Shows the next line of dialogue, then holds the scene for a moment. */
void Event_SayThenWait(s32 speaker, s32 frames)
{
    Event_ShowMessage(speaker, 0);
    Event_Wait(frames);
}

void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c)
{
    Actor_FaceDirection(a, b, 0);
    Event_Wait(c);
}

void OverlayObject_UpdateOnFrameBit1(s32 obj)
{
    if ((*(volatile s32 *)0x03001e40 & 2) != 0) {
        Object_SetPartPalettes(obj, 7);
    } else {
        Object_SetPartPalettes(obj, 0);
    }
    if ((*(volatile s32 *)0x03001e40 & 15) == 0) {
        Func_02004768(obj);
    }
}

void SceneEffect_UpdateByFrameBits(s32 no)
{
    volatile s32 *p = (volatile s32 *)0x03001e40;
    if ((*p & 1) != 0) {
        s32 t = Func_02004930((u32)*p >> 1, 6);
        Object_SetPartPalettes(no, t);
    }
    if ((*p & 15) == 0) {
        Func_020047a4(no);
    }
}

void SceneEffect_UpdateByFrameBit(s32 no)
{
    volatile s32 *p = (volatile s32 *)0x03001e40;
    if ((*p & 1) != 0) {
        s32 t = Func_0200496c((u32)*p >> 1, 6);
        Object_SetPartPalettes(no, t);
    }
}

void SceneEffect_AnimateVerticalPositive(struct SceneVerticalEffect *effect)
{
    struct VerticalEffectAnchor *anchor;
    s32 frame;
    s32 amplitude;
    s32 offset;

    anchor = effect->anchor;
    effect->frame = effect->frame + 1;
    frame = (s16)effect->frame;
    if (frame > 31) {
        Engine_ObjectDispatchRelease(effect);
        return;
    }
    amplitude = Math_Sin(frame << 10);
    effect->amplitude_x = amplitude;
    effect->amplitude_y = amplitude;
    effect->x = anchor->x;
    offset = 0x10000;
    effect->y = effect->y + offset;
    offset = offset - amplitude;
    effect->z = anchor->z + (offset * 4 + offset) + 0x80000;
}

void SceneEffect_AnimateVerticalNegative(struct SceneVerticalEffect *effect)
{
    struct VerticalEffectAnchor *anchor;
    s32 frame;
    s32 amplitude;
    s32 offset;

    anchor = effect->anchor;
    effect->frame = effect->frame + 1;
    frame = (s16)effect->frame;
    if (frame > 31) {
        Engine_ObjectDispatchRelease(effect);
        return;
    }
    amplitude = Math_Sin(frame << 10);
    effect->amplitude_x = amplitude;
    effect->amplitude_y = -amplitude;
    effect->x = anchor->x;
    offset = 0x10000;
    effect->y = effect->y + offset;
    offset = offset - amplitude;
    effect->z = anchor->z - (offset * 4 + offset) + 0x100000;
}

/* Haidia house: spawns the linked pair of effect objects above the parent actor, with a cue, and gives the two their update routines and priorities. */
void HaidiaIe_SpawnEffectPair(union PairObject *parent)
{
    union PairObject *pair[2];
    union PairObject *child;
    struct PairSprite *part;
    struct FieldSprite *sprite;
    struct PairWork *work = Data_03001f30;
    s32 i;

    Engine_AudioPlayCue(131);
    for (i = 0; i < 2; ++i) {
        child = (union PairObject *)Engine_ObjectCreate(26,
            parent->object.actor.x.fixed, parent->object.actor.y.fixed,
            parent->object.actor.z.fixed);
        pair[i] = child;
        if (child != NULL) {
            child->words[5] = parent->words[5];
            part = (struct PairSprite *)child->object.actor.sprite;
            child->object.actor.motion_flags = 0;
            child->object.effect.spin = 0;
            child->link.parent = parent;
            if (part != NULL) {
                sprite = &part->sprite;
                Main_08009020(sprite, 0);
                sprite->flags = 0;
                Main_080001b8(sprite->vram_block);
                sprite->vram_block = work->vram_block;
                /* FAKEMATCH: a plain byte access; the struct field store
                 * leaves a dead QImode zero that takes r3 from the +85
                 * address. */
                *(u8 *)&sprite->unknown_1d |= 1;
                sprite->tile = (Data_03001b10[sprite->vram_block].offset >> 5) & 0x3ff;
                sprite->full_color = 0;
                sprite->shape = 1;
                ((struct WorldMapOam *)sprite)->size = 2;
                part->detail->field_16 = 0;
            }
        }
    }
    {
        union PairObject *p = pair[0];
        struct FieldSprite *sp = p->object.actor.sprite;

        p->object.actor.update = (void (*)(union FieldObject *))0x0200a3ed;
        sp->priority = 2;
    }
    {
        struct FieldActor *p = &pair[1]->object.actor;
        struct FieldSprite *sp = p->sprite;

        sp->priority = 2;
        p->update = (void (*)(union FieldObject *))0x0200a39d;
        p->priority_flags = 2;
    }
}

void SceneState_ApplyPair140And0(void)
{
    Psynergy_Begin(140, 0);
}

void FieldScene_Forward4dac(void)
{
    Func_02004dac();
}

void FieldScene_RunStep15(void)
{
    Func_02004c4a(15);
    Func_020048be();
}

void FieldScene_RunStep17(void)
{
    Func_02004c5a(17);
    Func_020048ce();
}

void FieldScene_RunStep20(void)
{
    Func_02004c6a(20);
    Func_0200491a();
}

void SceneState_SetValues352_365_2116_2117_40(void)
{
    GameFlag_Set(352);
    GameFlag_Set(0x16d);
    GameFlag_Set(0x844);
    GameFlag_Set(0x845);
    Event_RequestExit(40);
}
