#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define OverlayObject_PrepareSceneObject        Func_02000048
#define AcquireOverlayObject      Func_02001992
#define RunOverlayObjectCommand0  Func_020019d8
#define RunOverlayObjectCommand1  Func_020019f0
#define RunOverlayObjectCommand14 Func_02001ad0
#define CreateOverlayObject Func_020019ea
#define SetOverlayObjectMode Func_02001a34
#define SetOverlayObjectSlot Func_02001b2c
#define EffectDescriptorTable Data_02009bb4
#define SceneDialogue_RunActor13FlaggedLine      Func_02000cc4
#define GetPartyEffect Func_02001b3e
#define SpawnEffect Func_02001abc
#define SetEffectVariant Func_02001ac6
#define SetEffectDescriptor Func_02001ae0
#define SetEffectMode Func_02001c62
#define ScaleEffectDeltaFromAccumulated Func_02001b7c
#define ScaleEffectDeltaFromOrigin Func_02001b94
#define ScaleEffectVerticalDelta Func_02001ba2
#define SetEffectCallbackMode Func_02001be0
#define SetEffectCallbackArgument Func_02001bf0
#define BeginSceneStep    Func_02002680
#define SceneFlagIsClear  Func_02002656
#define PlaySceneCueA     Func_02002728
#define PlaySceneCueB     Func_02002730
#define SetSceneActor     Func_0200161e
#define EndSceneStep      Func_020026aa
#define EffectMotion_GetHorizontalDecay Func_02003092
#define EffectMotion_GetVerticalDecay Func_020030a0

#include "configured_effect_spawn.h"

struct EffectRecord {
    u8 pad[9];
    u8 flags_lo : 2;
    u8 mode : 2;
    u8 flags_hi : 4;
};

struct EffectWork {
    u8 pad[80];
    struct EffectRecord *record;
};

struct SceneActor {
    u8 reserved_00[6];
    s16 temporary_state;
    u8 reserved_08[92];
    u16 presentation_flags;
};

struct SceneActor_02000cfc {
    u8 reserved_00[100];
    u16 presentation_flags;
};

struct SceneActor_02000d78 { u8 reserved_00[100]; u16 presentation_flags; };

struct Presentation {
    u8 reserved_00[9];
    u8 flags;
};

struct SceneActor_02000fb4 {
    u8 reserved_00[35];
    u8 state_23;
    u8 reserved_24[44];
    struct Presentation *presentation;
};

struct SceneActor_02001010 {
    u8 reserved_00[35];
    u8 state_23;
    u8 reserved_24[44];
    struct Presentation *presentation;
};

struct SceneActor_0200113c {
    u8 reserved_00[35];
    u8 state_23;
    u8 reserved_24[44];
    struct Presentation *presentation;
};

struct OverlayEffectMotion {
    u8 pad00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[28];
    s32 horizontal_rate;
    s32 vertical_rate;
    s32 shadow_x;
    s32 shadow_y;
    s32 shadow_z;
    u8 pad44[32];
    s16 mode;
};

extern u8 *Data_03001e8c[];
extern u8 Data_000012c0[];

void *Func_02001992(s32, s32, s32, s32);
void Func_020019d8(void *, s32);
void Func_020019f0(void *, s32);
void Func_02001ad0(void *, s32);
void *Func_020019ea(s32, s32, s32, s32);
void Func_02001a34(void *, s32);
void Func_02001b2c(void *, s32);
u8 *Func_02001d26();
s32 Func_020019c6();
u8 *Func_02001d3c();
void Func_02001a06();
u8 *Func_02001db2(s32);
s32 Func_02001a72(u8 *, const u8 *, s32, s32);
s32 Func_02001dba(s32);
u8 *Func_02001dc6(int);
void Func_02001f28(void);
u8 *Func_02001dde(int);
void Func_02001e26(int,int,int,int);
void Func_02001f54(int);
void Func_02001e0a(int);
s32 Func_02001e10(int);
void Func_02001f84(void);
void Func_02001f3c(int,int);
void Func_02001f44(int,int);
void Func_02001fa0(void);
int Func_02001f7e(int);
void Func_02001d64();
void Func_02001fd0();
s32 Func_02002010();
void Func_02002024();
void Func_0200206e();
s32 Func_02002088();
void Func_020020b4();
void Func_020020d0();
void Func_02002030(void);
void Func_020020ce(int);
void Func_020020a6(int,int);
void Func_02001dcc(int,int,int);
int Func_020020f0(int,int);
int Func_02002078(int,int);
void Func_0200211c(int,int);
void Func_02002080(void);
void Func_0200208c(void);
void Func_0200212a(int);
void Func_02001e20(int,int,int);
int Func_02002144(int,int);
void Func_02002170(int,int);
void Func_020020d4(void);
void Func_020020e0(void);
void Func_0200217e(int);
void Func_02001e74(int,int,int);
int Func_02002198(int,int);
int Func_02002120(int,int);
void Func_020021c4(int,int);
void Func_02002128(void);
void Func_02002134(void);
void Func_020021d2(int);
void Func_02001094(int);
void Func_0200214c(void);
void Func_02002154(void);
void Func_020021f2(int);
void Func_020010b4(int);
void Func_0200216c(void);
void Func_02002174(void);
void Func_02002212(int);
void Func_020021ea(int,int);
void Func_02001f10(int,int,int);
void Func_020021a0(void);
void Func_02001f62(s32, s32, s32);
void Func_02002148(s32);
void Func_020021c6(void);
struct SceneActor *Func_020021d6(s32);
void Func_020021fc(void);
void Func_02002264(s32);
void Func_02001fc2(s32, s32);
void Func_02001fd2(s32, s32, s32);
void Func_020021b8(s32);
void Func_02002236(void);
struct SceneActor *Func_02002246(s32);
void Func_0200226c(void);
void Func_020022ac(s32, s32);
void Func_020022d4(s32);
void Func_0200201c();
void Func_02002288();
void Func_020022a0();
void Func_020022bc();
void Func_02002326();
s32 Func_02002350();
void Func_02002362();
void Func_020022ca(void);
void Func_0200233a(int,int);
void Func_02002060(int,int,int);
void Func_020022f0(void);
s32 Func_02002322(void);
void Func_0200230a(void);
void Func_0200237a(s32, s32);
void Func_02002310(s32);
void Func_0200232c(void);
void Func_02002338(void);
void Func_020023d6(int);
int Func_020023f0(int,int);
int Func_02002378(int,int);
void Func_0200241c(int,int);
void Func_02002380(void);
void Func_0200242a(int);
void Func_020012ec(int);
void Func_020023ac(void);
void Func_0200244a(int);
int Func_02002388(int);
void Func_02001326(int);
void Func_020023de(void);
void Func_020023e8(void);
void Func_02002486(int);
void Func_02001348(int);
void Func_02002400(void);
void Func_020021d4(s32, s32);
void Func_020021e4(s32, s32, s32);
s32 Func_020023fc(s32);
void Func_02002420(void);
struct SceneActor *Func_02002432(s32);
void Func_0200247c(void);
void Func_020024be();
void Func_02002232(s32, s32);
void Func_02002242(s32, s32, s32);
void Func_02002428(s32);
void Func_020024a6(void);
struct SceneActor *Func_020024b6(s32);
void Func_020024dc(void);
void Func_0200251c(s32, s32);
void Func_02002544(s32);
void Func_020024f8(void);
void Func_02002596(int);
void Func_0200256e(int,int);
void Func_02002294(int,int,int);
int Func_020025b8(int,int);
int Func_02002540(int,int);
void Func_020025e4(int,int);
void Func_02002548(void);
void Func_02002554(void);
void Func_020025f2(int);
void Func_020022e8(int,int,int);
int Func_0200260c(int,int);
int Func_02002594(int,int);
void Func_0200259c(void);
void Func_02002344(s32, s32, s32);
s32 Func_0200259e(s32);
void Func_020025a8(void);
s32 Func_020025b4(s32);
void Func_02002602(void);
void Func_02002646(s32);
void Func_02002670(s32, s32);
void Func_02002610(void);
void Func_020026ae(int);
void Func_02002698(int,int);
void Func_02002626(int);
void Func_020026de(int,int);
void Func_02002642(void);
void Func_02002648(void);
void Func_020026f0(int);
void Func_020026f8(int);
void Func_020015e6(int);
void Func_02002672(void);
void Func_02002680(void);
s32 Func_02002656(s32);
void Func_02002728(s32);
void Func_02002730(s32);
void Func_0200161e(s32);
void Func_020026aa(void);
void Func_0200168c(s32);
s32 Func_0200269e(s32);
s32 Func_020026c2(s32);
void Func_020026c8(void);
struct SceneActor *Func_020026e2(s32);
void Func_02002718(void);
struct SceneActor *Func_0200273e(s32);
void Func_02002770(s32);
void Func_02002784(s32);
void Func_020016ee(s32);
s32 Func_0200271a(s32);
void Func_02002744(void);
struct SceneActor *Func_0200275e(s32);
void Func_0200277a(void);
struct SceneActor *Func_020027a0(s32);
void Func_02002800(s32);
void Func_02002790(void);
int Func_02002766(int);
void Func_02002838(int);
void Func_0200172e(int);
void Func_020027ba(void);
u8 *Func_020027f2(int);
void Func_020027d6(void);
int Func_020027cc(int);
void Func_020028a6(int);
void Func_02002856(int,int);
void Func_020028be(int,int);
void Func_02002822(void);
void Func_020028de(int);
void Func_020017cc(int);
void Func_02002858(void);
void Func_0200283e(int);
void Func_02002812(int,int,int,int,int,int);
void Func_0200286e(int);
void Func_0200283a(int,int,int,int,int,int);
void Func_020028f4(s32, s32, s32);
void Func_02002916(s32, s32, s32);
void Func_020029ca(s32);
void Func_02002a0a(int);
void Func_0200288c(void *,int,int);
void Func_02001e4e(int,int,int);
void Func_02002a32(int);
void Func_020028b4(void *,int,int);
void Func_02001e76(int,int,int);
void Func_02002a5a(int);
void Func_020028dc(void *,int,int);
void Func_02001e9c(int,int,int);
void Func_02001ef6(s32, s32, s32);
void Func_02002910(const void *, s32, s32);
void Func_02002936(s32, s32, s32, s32, s32, s32);
struct SceneActor *Func_0200299c(s32);
void Func_02002a8e(s32);
void Func_0200296c(const void *, s32, s32);
void Func_02002992(s32, s32, s32, s32, s32, s32);
struct SceneActor *Func_020029f8(s32);
void Func_02002aea(s32);
void Func_02002b3a(int);
void Func_020029bc(void *,int,int);
void Func_02001f7a(int,int,int);
void Func_0200207c(s32, s32, s32);
void Func_02002a98(const void *, s32, s32);
void Func_02002abe(s32, s32, s32, s32, s32, s32);
void Func_02002c16(s32);
void Func_02002c66(int);
void Func_0200209e(int,int,int);
u8 *Func_02002ba2(int);
void Func_02002bba(int,int,int);
void Func_02002b40(u8 *,int);
void Func_02002bea(int,int,int);
void Func_02002b7c(u8 *,int);
void Func_0200243c();
s32 Func_02002bd6();
void Func_02002bf4();
void Func_02002bf8();
void Func_02002c2c();
void Func_02002c40();
void Func_02002c42();
void Func_02002c74();
void Func_02002c96();
void Func_02002ca6();
void Func_02002cb2();
void Func_02002cdc();
void Func_02002d0a();
void Func_02002c6e();
void Func_02002c80();
void Func_02002c82();
void Func_02002ca4();
void Func_02002cc0();
void Func_02002cca();
void Func_02002cd0();
void Func_02002ce0();
void Func_02002ce4();
void Func_02002cf2();
void Func_02002cf6();
void Func_02002d10();
void Func_02002d18();
void Func_02002d3a();
void Func_02002d44();
void Func_02002d4a();
void Func_02002d5a();
void Func_02002d70();
void Func_02002dbe();
typedef s32(*IwramSqrt02001638)(s32);
void Func_0200318c(s32,s32);
void Func_020030e2(s32);
void Func_02003184(s32,s32,s32);
void Func_020030fa(s32);
s32 Func_02003092();
s32 Func_020030a0();

/* Shared 22-byte head leaf proved identical for this overlay family. */

/*
 * Resource 3b5, owner at 0x02000314 (152 bytes advertised; 144 bytes of code
 * plus an 8-byte literal pool at 0x02000200-0x02000207).
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the high-register save at
 * 0x02000314-0x02000178, and the matching interworking return at
 * 0x020001f2-0x020001fe.  It pops into r1, so r0 survives and is the result;
 * the only value ever left in r0 is the `movs r0, #0` at 0x020001f0, so this
 * returns a constant 0.
 *
 * Role: this is a per-frame actor callback.  The overlay initialiser
 * Func_02000728 stores the pool word 0x02008171 into field +0x6c of actors 14
 * and 15; under this overlay's proven 0x02008000 link base that word is
 * SceneActor_UpdateProximityToLeader + the Thumb bit, which names this row's role before it is
 * disassembled.  Its argument is therefore the owning actor record.
 *
 * 0x03001e8c is a table of pointers; entry 12 is 0x03001ebc, the overlay
 * workspace pointer the rest of this overlay loads directly (see
 * Func_02000644, Func_02000894, Func_02000980).  Modelled that way rather than
 * as two unrelated globals.
 *
 * Call targets resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`.
 */

/*
 * Old-style declarations: overlay import arities are not fixed per name.
 *
 * Each site names the veneer IT reaches, not the import behind it: the four
 * calls here are Scene_GetRecord, Func_0200007c, Scene_GetRecord, Func_0200007c,
 * and the reference reaches them through four DIFFERENT veneers.  Naming the
 * import instead made the linker synthesise fresh ones and every `bl` missed.
 * Same convention as 6e7d6a12e and games/gs1/src/resource_371_c_0200037c.c, where one
 * import reached from three sites carries three names.
 *
 * Func_02001a06 is `void` because its result is discarded (b7c1a35a0).
 *
 * Func_020019c6 returns s32: the reference tests its result with `cmp r0, #0`
 * and never truncates it, so a u8 return costs an `lsls r0, r0, #24` the ROM
 * does not have.  An earlier reading measured that widening as expensive, but
 * that was against the conditional-expression form of the partner select
 * below; once that is a branch, the wide return is what matches.
 */

/*
 * BYTE-EXACT and adopted 2026-08-07.  The last residual was the epilogue, which
 * the reference returns through r1 (`pop {r1} ; bx r1') rather than r0: the
 * owner passes the result of its final call back to its caller, so r0 has to
 * survive the pop.  The tail call is written as `return Func_02001e10(2);' and
 * the owner's type changed from void to s32 to say so; the fall-through path
 * returns nothing, exactly as the reference does.
 */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* The scene step counter at 0x1d8 of the shared scene work record. */

/* Returns a value: the reference sets r1 before r0 at this site, which
   only a value-returning callee does; the result is unused here. */

/*
 * resource_382 owner at 0x02001754, 88 bytes.
 * Per-frame step for a projectile: advances x by its rate and mirrors it into
 * the shadow copy, then either follows the y rate or falls at a fixed rate
 * depending on the mode word at +100, and finally decays both rates.
 */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1);
}

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

static __inline__ void SetScale(s32 actor, s32 horizontal, s32 vertical)
{
    extern u8 *Data_03001ebc;

    Func_020028f4(actor, horizontal, vertical);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];
    void Func_02002c1c();

    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];
    void Func_02002c1c();

    f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_03001ebc[];
    void Func_02002c1c();

    f(a0, a1, a2, a3);
}

void SceneEffect_SetEffectRecordMode(struct EffectWork *work, s32 mode)
{
    work->record->mode = mode;
}

void *OverlayObject_PrepareSceneObject(s32 first, s32 second, s32 third, s32 fourth) {
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
        RunOverlayObjectCommand0(obj, 0);
        RunOverlayObjectCommand14(obj, 0xE);
        RunOverlayObjectCommand1(obj, 1);
        return obj;
    }
    return NULL;
}

void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *result = CreateOverlayObject(arg3, arg0, arg1, arg2);

    if (result != NULL) {
        u8 *object = *(u8 **)(result + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = object[9];
        mask = -mask;
        mask &= flags;
        mask |= 4;
        object[9] = mask;
        result[0x55] = 0;
        result[0x59] = 8;
        SetOverlayObjectMode(result, 0);
        SetOverlayObjectSlot(result, 15);
        result[0x23] = (result[0x23] & 0xfe) | 2;
        return result;
    }
    return NULL;
}

void SceneEffect_SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "configured_effect_spawn_body.inc"
}

s32 SceneActor_UpdateProximityToLeader(u8 *self)
{
    u8 **globals = (u8 **)0x03001e8c;
    u8 *scene = globals[0];
    u8 *workspace = globals[12];        /* == *(u8 **)0x03001ebc */
    u16 *flags = (u16 *)(self + 100);
    s32 force = 0;
    s32 range = 18;
    u8 *partner;
    u8 *player;

    /*
     * Bit 0 of the actor's own flag halfword selects which partner to test.
     * Written as two calls, not `Func_02001d26(bit ? 15 : 14)`: the reference
     * branches and joins on one `bl`, which is what cross-jumping the two calls
     * produces.  As a conditional expression gcc folds it to `14 + (bit != 0)`
     * and emits the negs/orrs/lsrs boolean instead.
     */
    if ((*flags & 1) != 0) {
        partner = Func_02001d26(15);
    } else {
        partner = Func_02001d26(14);
    }
    if (Func_020019c6(self, partner, 32, 0) != 0) {
        return 0;
    }

    player = Func_02001d3c(0);

    /*
     * Widen the test when the scene counter at workspace + 376 is already
     * running, or when the scene byte at scene + 0x0ea4 is set.
     */
    if (*(s16 *)(workspace + 376) != 0 || scene[0x0ea4] != 0) {
        range = 26;
        if ((*flags & 2) != 0) {
            force = 1;
        }
    }

    Func_02001a06(self, player, range, force);
    return 0;
}

s32 ActorPresentation_UpdateEntityFromLeader(u8 *entity)
{
    u8 *base = Data_03001e8c[0];
    u8 *workspace = Data_03001e8c[12];
    s32 flag = 0;
    s32 selector = 18;
    u8 *leader;

    if (*(s32 *)(entity + 56) == (s32)0x80000000)
        return 0;

    leader = Func_02001db2(0);
    if (*(s16 *)(workspace + 376) != 0 || base[0x0ea4] != 0) {
        selector = 26;
        flag = 1;
    }
    Func_02001a72(entity, leader, selector, flag);
    return 0;
}

void *SceneData_GetScriptTable(void)
{
    return (void *)0x02009f5c;
}

int SceneData_ReturnZero(void)
{
    return 0;
}

void *SceneData_GetMessageTable(void)
{
    return (void *)0x0200a094;
}

void *SceneData_SelectActorTableByFlag855(void)
{
    if (Func_02001dba(0x855) != 0)
        return (void *)0x0200a27c;
    return (void *)0x0200a0cc;
}

int OverlayObject_GetObject2Byte280(void)
{
    return Func_02001dc6(2)[280];
}

s32 OverlayObject_RunObject2WhenFlagged(void)
{
    Func_02001f28();
    if ((*(u32 *)(Func_02001dde(2) + 248) & 1) != 0) {
        Func_02001e26(2, 0, 0, 0);
        Func_02001f54(126);
        Func_02001e0a(0);
        return Func_02001e10(2);
    }
}

void SceneState_SetFlags947And29dc(void)
{
    Func_02001f84();
    Func_02001f3c(0x947, 1);
    Func_02001f44(0x29dc, 1);
    Func_02001fa0();
}

void *SceneData_SelectTableA414ByFlag855(void)
{
    if (Func_02001f7e(0x855) != 0)
        return (void *)0x0200a630;
    return (void *)0x0200a414;
}

void SceneDialogue_RunActor9LineAndAdvance(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02001fd0();
    Call1(Func_0200206e, 0x1223);
    Func_02001d64(9, 0, 2);
    Value2(Func_02002088, 9, 0);
    if (Value2(Func_02002010, 0, 0) != 0) {
        bump_step(1);
    }
    Func_020020b4(9, 0);
    Call3(Func_020020d0, 9, 0x5000, 0);
    Func_02002024();
}

void ActorPresentation_RunActorThirteenSceneSetup(void)
{
    u8 *workspace;

    Func_02002030();
    Func_020020ce(0x1229);
    Func_020020a6(13, 1);
    Func_02001dcc(13, 0, 2);
    Func_020020f0(13, 0);
    if (Func_02002078(0, 0) != 0) {
        workspace = *(u8 **)0x03001ebc;
        ++*(u16 *)(workspace + 472);
    }
    Func_0200211c(13, 0);
    Func_02002080();
}

void ActorPresentation_RunActorSeventeenSceneSetup(void)
{
    int Func_020020cc(int,int);

    u8 *workspace;

    Func_0200208c();
    Func_0200212a(0x122f);
    Func_02001e20(17, 0, 2);
    Func_02002144(17, 0);
    if (Func_020020cc(0, 0) != 0) {
        workspace = *(u8 **)0x03001ebc;
        ++*(u16 *)(workspace + 472);
    }
    Func_02002170(17, 0);
    Func_020020d4();
}

void ActorPresentation_RunActorEighteenSceneSetup(void)
{
    u8 *workspace;

    Func_020020e0();
    Func_0200217e(0x1232);
    Func_02001e74(18, 0, 2);
    Func_02002198(18, 0);
    if (Func_02002120(0, 0) != 0) {
        workspace = *(u8 **)0x03001ebc;
        ++*(u16 *)(workspace + 472);
    }
    Func_020021c4(18, 0);
    Func_02002128();
}

void SceneDialogue_RunActor11Line(void) { Func_02002134(); Func_020021d2(0x1227); Func_02001094(11); Func_0200214c(); }

void SceneDialogue_RunActor16Line(void) { Func_02002154(); Func_020021f2(0x122e); Func_020010b4(16); Func_0200216c(); }

void SceneDialogue_RunActor19Line(void)
{
    void Func_0200223c(int,int);

    Func_02002174(); Func_02002212(0x1235); Func_020021ea(19, 0);
    Func_02001f10(19, 0, 2); Func_0200223c(19, 0); Func_020021a0();
}

void ActorPresentation_RunActorFourteenDialogue(void)
{
    void Func_02001f52(s32, s32);
    void Func_0200223c(s32, s32);

    struct SceneActor *actor = Func_020021d6(14);
    s16 saved = actor->temporary_state;

    actor->presentation_flags |= 2;
    Func_020021c6();
    Func_02002264(0x122c);
    Func_0200223c(14, 0);
    Func_02001f62(14, 0, 2);
    Func_02001f52(14, 10);
    actor->temporary_state = saved;
    Func_02002148(1);
    Func_020021fc();
    actor->presentation_flags &= 1;
}

void ActorPresentation_RunActorFifteenDialogue(void)
{
    struct SceneActor *actor = Func_02002246(15);
    s16 saved = actor->temporary_state;

    actor->presentation_flags |= 2;
    Func_02002236();
    Func_020022d4(0x122d);
    Func_020022ac(15, 0);
    Func_02001fd2(15, 0, 2);
    Func_02001fc2(15, 10);
    actor->temporary_state = saved;
    Func_020021b8(1);
    Func_0200226c();
    actor->presentation_flags &= 1;
}

void FieldScene_RunActor21Sequence(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02002288();
    Func_02002326((s32)Data_000012c0);
    Func_0200201c(21, 0, 2);
    Call3(Func_02002362, 21, 0x103, 0);
    Func_020022a0(30);
    Value2(Func_02002350, 21, 0);
    Func_020022bc();
}

void SceneActor_RunActorStep(int actor)
{
    void Func_0200238c(int,int);

    Func_020022ca(); Func_0200233a(actor, 1); Func_02002060(actor, 0, 2);
    Func_0200238c(actor, 0); Func_020022f0();
}

void SceneActor_RunActorCommandWithFlag91(s32 x)
{
    void Func_020023c8(s32, s32);

    u8 *flag = (u8 *)Func_02002322() + 91;
    s32 zero = 0;

    *flag = 1;
    Func_0200230a();
    Func_0200237a(x, 1);
    Func_02002310(2);
    Func_020023c8(x, 0);
    Func_0200232c();
    *flag = zero;
}

void ActorPresentation_RunActorEightSceneSetup(void)
{
    void Func_020020cc(int,int,int);

    u8 *workspace;
    Func_02002338(); Func_020023d6(0x1330); Func_020020cc(8, 0, 2); Func_020023f0(8, 0);
    if (Func_02002378(0, 0) != 0) { workspace = *(u8 **)0x03001ebc; ++*(u16 *)(workspace + 472); }
    Func_0200241c(8, 0); Func_02002380();
}

void SceneDialogue_RunActor11SecondLine(void) { Func_0200238c(); Func_0200242a(0x1335); Func_020012ec(11); Func_020023a4(); }

void SceneDialogue_RunActor12LineAndAdvance(void)
{
    u8 *workspace;
    Func_020023ac(); Func_0200244a(0x1336);
    if (Func_02002388(2) != 0) { workspace = *(u8 **)0x03001ebc; ++*(u16 *)(workspace + 472); }
    Func_02001326(12); Func_020023de();
}

void SceneDialogue_RunActor13Line(void) { Func_020023e8(); Func_02002486(0x1338); Func_02001348(13); Func_02002400(); }

void ActorPresentation_RunActorFourteenDialogueAndAdvanceStory(void)
{
    extern u8 *Data_03001ebc;
    void Func_020023c8(s32);

    struct SceneActor *actor = Func_02002432(14);
    u16 *flags = &actor->presentation_flags;
    s16 saved = actor->temporary_state;
    /* Keeps the original compiler's flag-result register lifetime. */
    s32 tmp;

    *flags = (tmp = *flags | 2);
    Func_02002420();
    Func_020024be(0x1339);
    if (Func_020023fc(2) != 0)
        ++*(u16 *)(Data_03001ebc + 472);
    Func_020024be(14, 0);
    Func_020021e4(14, 0, 2);
    Func_020021d4(14, 10);
    actor->temporary_state = saved;
    Func_020023c8(1);
    Func_0200247c();
    *flags &= 1;
}

void ActorPresentation_RunActorFifteenFollowupDialogue(void)
{
    struct SceneActor *actor = Func_020024b6(15);
    s16 saved = actor->temporary_state;

    actor->presentation_flags |= 2;
    Func_020024a6();
    Func_02002544(0x133b);
    Func_0200251c(15, 0);
    Func_02002242(15, 0, 2);
    Func_02002232(15, 10);
    actor->temporary_state = saved;
    Func_02002428(1);
    Func_020024dc();
    actor->presentation_flags &= 1;
}

void ActorPresentation_RunActorSixteenSceneSetup(void)
{
    u8 *workspace;
    Func_020024f8(); Func_02002596(0x133c); Func_0200256e(16, 1); Func_02002294(16, 0, 2); Func_020025b8(16, 0);
    if (Func_02002540(0, 0) != 0) { workspace = *(u8 **)0x03001ebc; ++*(u16 *)(workspace + 472); }
    Func_020025e4(16, 0); Func_02002548();
}

void ActorPresentation_RunActorEighteenFollowupSceneSetup(void)
{
    void Func_02002638(int,int);

    u8 *workspace;
    Func_02002554(); Func_020025f2(0x133f); Func_020022e8(18, 0, 2); Func_0200260c(18, 0);
    if (Func_02002594(0, 0) != 0) { workspace = *(u8 **)0x03001ebc; ++*(u16 *)(workspace + 472); }
    Func_02002638(18, 0); Func_0200259c();
}

void ActorPresentation_RunActorNineteenDialogueAndSetSceneState(void)
{
    extern u8 *Data_03001ebc;
    void Func_0200261e(s32, s32);
    void Func_02002638(s32, s32);

    Func_020025a8();
    Func_02002646(0x1342);
    Func_0200261e(19, 0);
    Func_02002344(19, 0, 2);
    Func_02002670(19, 0);
    Func_02002638(19, 1);
    if (Func_0200259e(231) != -1 && Func_020025b4(0x858) == 0) {
        u16 *p = (u16 *)(Data_03001ebc + 370);
        u16 value = 1;

        *p = value;
    }
    Func_02002602();
}

void SceneDialogue_RunActor20Line(void)
{
    void Func_020023a4(int,int,int);

    Func_02002610(); Func_020026ae(0x137f); Func_020023a4(20, 0, 2); Func_02002698(20, 3);
    Func_02002626(20); Func_020026de(20, 0); Func_02002642();
}

void SceneDialogue_RunActor11FlaggedLine(void)
{
    int Func_0200261e(int);

    Func_02002648();
    if (Func_0200261e(0x855) == 0) Func_020026f0(0x1239); else Func_020026f8(0x1346);
    Func_020015e6(11); Func_02002672();
}

void SceneDialogue_RunActor13FlaggedLine(void) {
    BeginSceneStep();
    if (SceneFlagIsClear(0x855) == 0) {
        PlaySceneCueA(0x123b);
    } else {
        PlaySceneCueB(0x1348);
    }
    SetSceneActor(13);
    EndSceneStep();
}

void ActorPresentation_RunActorFourteenFlaggedDialogue(void)
{
    extern u8 *Data_03001ebc;

    Func_020026e2(14)->presentation_flags |= 2;
    Func_020026c8();
    if (Func_0200269e(0x855) == 0) {
        Func_02002770(0x123c);
    } else {
        Func_02002784(0x1349);
        if (Func_020026c2(2) != 0)
            ++*(u16 *)(Data_03001ebc + 472);
    }
    Func_0200168c(14);
    Func_02002718();
    Func_0200273e(14)->presentation_flags &= 1;
}

void ActorPresentation_RunActorFifteenScriptBranch(void)
{
    void Func_020027ec(s32);

    Func_0200275e(15)->presentation_flags |= 2;
    Func_02002744();
    if (Func_0200271a(0x855) == 0)
        Func_020027ec(0x123d);
    else
        Func_02002800(0x134b);
    Func_020016ee(15);
    Func_0200277a();
    Func_020027a0(15)->presentation_flags &= 1;
}

void ActorPresentation_RunActorSixteenScriptBranch(void)
{
    void Func_02002840(int);

    Func_02002790();
    if (Func_02002766(0x855) == 0) Func_02002838(0x123e); else Func_02002840(0x134c);
    Func_0200172e(16); Func_020027ba();
}

void ActorPresentation_RunActorNineteenScriptBranch(void)
{
    void Func_020027ec(int);

    u8 *actor = Func_020027f2(19); actor[91] = 1; Func_020027d6();
    if (Func_020027ac(0x855) == 0) {
        Func_0200287e(0x1241); Func_02002856(19, 0); Func_020027ec(2);
    } else if (Func_020027cc(0x858) != 0) {
        Func_0200289e(0x13ab);
    } else {
        Func_020028a6(0x134e);
    }
    Func_020028be(19, 0); Func_02002822(); actor[91] = 0;
}

void SceneDialogue_RunActor21Line(void) { Func_02002840(); Func_020028de(0x12c1); Func_020017cc(21); Func_02002858(); }

void SceneState_Apply200ThenPlace55_26(void)
{
    Func_0200283e(0x200);
    {
        int v1 = 23;
        int v2 = 26;
        Func_02002812(55, 26, 4, 2, v1, v2);
    }
}

void SceneState_Apply200ThenPlace23_23(void)
{
    Func_0200286e(0x200);
    {
        int v1 = 23;
        int v2 = 26;
        Func_0200283a(23, 23, 4, 2, v1, v2);
    }
}

void SceneActor_PlaceActor0AndSetSceneDelay(s32 x, s32 y, s32 continuation)
{
    extern u8 *Data_03001ebc;

    SetScale(0, 0x8000, 0x4000);
    Func_02002916(0, x, y);
    *(s32 *)(Data_03001ebc + 456) = 16;
    Func_020029ca(continuation);
}

void FieldScene_SetupScene5At408_320(void)
{
    Func_02002a0a(158); Func_0200288c((void *)0x0200a828, 56, 19); Func_02001e4e(408, 320, 5);
}

void FieldScene_SetupScene6At312_304(void)
{ Func_02002a32(158); Func_020028b4((void *)0x0200a83e, 50, 18); Func_02001e76(312, 304, 6); }

void FieldScene_SetupScene7At216_288(void)
{ Func_02002a5a(158); Func_020028dc((void *)0x0200a854, 44, 17); Func_02001e9c(216, 288, 7); }

void ActorPresentation_SetupActorZeroForSceneEightAt376_224(void)
{
    struct SceneActor_02000fb4 *actor = Func_0200299c(0);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Func_02002a8e(158);
    Func_02002910((const void *)0x0200a86a, 54, 13);
    {
        s32 cell = 23;
        s32 row = 12;

        Func_02002936(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    Func_02001ef6(376, 224, 8);
}

void ActorPresentation_SetupActorZeroForSceneNineAt296_176(void)
{
    void Func_02001f52(s32, s32, s32);

    struct SceneActor_02001010 *actor = Func_020029f8(0);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Func_02002aea(158);
    Func_0200296c((const void *)0x0200a880, 49, 10);
    {
        s32 cell = 18;
        s32 row = 10;

        Func_02002992(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    Func_02001f52(296, 176, 9);
}

void FieldScene_SetupScene10At120_144(void)
{ Func_02002b3a(158); Func_020029bc((void *)0x0200a896, 38, 6); Func_02001f7a(120, 144, 10); }

void ActorPresentation_SetupActorZeroForSceneTwelveAt72_160(void)
{
    struct SceneActor *Func_02002b24(s32);

    struct SceneActor_0200113c *actor = Func_02002b24(0);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Func_02002c16(158);
    Func_02002a98((const void *)0x0200a8ac, 35, 9);
    {
        s32 cell = 4;
        s32 row = 10;

        Func_02002abe(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    Func_0200207c(72, 160, 12);
}

void FieldScene_SetupScene13At152_264(void) { Func_02002c66(123); Func_0200209e(152,264,13); }

void ActorPresentation_MoveActorToPositionAndWait(int actor, int x, int z, int field40)
{
    void Func_02002b24(int);
    void Func_02002c1c(int,int,int);

    u8 *record = Func_02002ba2(actor); int frames;
    Func_02002bba(actor, 0x30000, 0x18000); *(s32 *)(record + 72) = 0x8000;
    *(s32 *)(record + 68) = 0; *(s32 *)(record + 40) = field40; Func_02002b40(record, 0);
    Func_02002bea(actor, x, z); Func_02002c1c(actor, x << 16, z << 16);
    for (frames = 60; frames != 0; --frames) { Func_02002b24(1); if (*(s16 *)(record + 42) == 0) break; }
    Func_02002b7c(record, 1); *(s32 *)(record + 72) = 0x10000;
}

void FieldScene_RunActor23SequenceOnceByFlag867(void)
{
    extern u8 Data_03001ebc[];
    void Func_02002c1c();

    u32 i;
    s32 record;

    Func_02002bf4();
    Func_02002d0a(100);
    Func_02002bf8(40);
    if (Value1(Func_02002bd6, 0x867) == 0) {
        Call2(Func_02002cdc, 23, 0x102);
        Func_02002c96(23, 4, 0);
        Func_02002c1c(12);
        Func_02002ca6(23, 4, 0);
        Func_02002c2c(20);
        Call4(Func_0200243c, 23, 0x188, 104, 0x70000);
        Func_02002c42(20);
        Call3(Func_02002ca6, 23, 0x198, 104);
        Call3(Func_02002cb2, 23, 0x198, 120);
        Call1(Func_02002c40, 0x867);
    }
    Func_02002c74();
}

void FieldScene_RunActor19MotionSequence(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02002c6e(231);
    Func_02002c82();
    Func_02002c80(10);
    Func_02002d10(19, 2);
    Call3(Func_02002cca, 19, 0xcccc, 0x6666);
    Call3(Func_02002cf6, 19, 216, 0x198);
    Func_02002ca4(10);
    Call3(Func_02002d70, 19, 0x4000, 20);
    Func_02002d3a(19, 6, 0);
    Func_02002cc0(30);
    Func_02002d4a(19, 6, 0);
    Func_02002cd0(30);
    Func_02002d5a(19, 6, 0);
    Func_02002ce0(30);
    Call3(Func_02002d44, 19, 216, 0x188);
    Func_02002cf2(10);
    Call3(Func_02002dbe, 19, 0x4000, 20);
    Call1(Func_02002ce4, 0x858);
    Func_02002d18();
}

s32 SceneActor_GetPositionDistance(s32 *a, s32 *b)
{
    s32 dx = (*a++ - *b++) >> 16;
    s32 dy = (*a++ - *b++) >> 16;
    s32 dz = (*a - *b) >> 16;
    s32 dxsq = dx * dx;
    s32 dysq = dy * dy;
    s32 dzsq = dz * dz;

    return ((IwramSqrt02001638) 0x030001D8)(dxsq + dysq + dzsq);
}

void SceneActor_ApplyActorZeroThenWait(s32 actor, s32 delay)
{
    Func_0200318c(actor, 0);
    Func_020030e2(delay);
}

void SceneActor_ApplyActorCueThenWait(s32 actor, s32 cue, s32 delay)
{
    Func_02003184(actor, cue, 0);
    Func_020030fa(delay);
}

void SceneEffect_UpdateMotionWithDamping(struct OverlayEffectMotion *effect)
{
    s32 horizontal_rate;
    s32 vertical_rate;

    effect->x += effect->horizontal_rate;
    effect->shadow_x = effect->x;

    if (effect->mode != 0) {
        effect->y += effect->vertical_rate;
        effect->shadow_y = effect->y;
    } else {
        effect->z += effect->vertical_rate;
        effect->shadow_z = effect->z;
        effect->y += 1024;
        effect->shadow_y = effect->y;
    }

    horizontal_rate = effect->horizontal_rate;
    effect->horizontal_rate = horizontal_rate - EffectMotion_GetHorizontalDecay(horizontal_rate, 28);
    vertical_rate = effect->vertical_rate;
    effect->vertical_rate = vertical_rate - EffectMotion_GetVerticalDecay(vertical_rate, 28);
}
