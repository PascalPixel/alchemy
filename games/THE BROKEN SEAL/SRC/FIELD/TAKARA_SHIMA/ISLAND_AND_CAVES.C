#include "TYPES.H"
#include "FIELD_EVENT.H"

#define OverlayObject_IntegrateVelocities Func_02000ab0
#define GetOrbitingSceneObject Func_02005f6c
#define AllocateEffectTransfer Func_02005f3c
#define UpdateOrbitingSceneObject Value_0200ae71

#include "STAGED_ACTOR.H"
#include "FIELD_EFFECT.H"
#define OverlayObject_IntegrateVelocities Effect_Move

/* Integrate position, velocity, rate and sprite angle for one scene effect.
   Signed division preserves decay toward zero for negative Z velocity. */
struct Sprite {
    u8 pad00[9];
    u8 flags9;
    u8 pad0a[20];
    u16 angle;
    u8 pad20[6];
    u8 state26;
};

struct Effect {
    u8 pad00[8];
    s32 position[3];
    u8 pad14[4];
    s32 accum18;
    s32 accum1c;
    u8 pad20[3];
    u8 flatla3;
    u8 pad24[12];
    s32 rate30;
    s32 rate34;
    u8 pad38[12];
    s32 velocity[3];
    struct Sprite *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

struct T {
    u8 pad00[30];
    u16 f1e;
};

struct S {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    u8 pad10[32];
    s32 f30;
    s32 f34;
    s32 f38;
    s32 f3c;
    u8 pad40[16];
    struct T *f50;
};

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[28];
    s32 f30;
    s32 f34;
} Obj;

struct V6 {
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s32 e;
    s32 f;
};

struct S_02000474 {
    s32 f00;
    u16 f04;
    u16 f06;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[20];
    s32 f28;
    u8 pad2c[4];
    s32 f30;
    s32 f34;
    u8 pad38[29];
    u8 f55;
};

struct V {
    s32 a;
    s32 b;
    s32 c;
};

struct Owner {
    u8 unk0[9];
    u8 unk9_0 : 2;
    u8 mode : 2;
    u8 unk9_4 : 4;
};

struct S_020009dc {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[15];
    u8 f23;
};

struct SceneObject {
    u8 filler00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

struct EffectParams {
    u8 filler00[0x24];
    s32 callback;
};

typedef union {
    s32 w;
    s16 h[2];
} RecWord;

struct S_02001b14 { s32 pad[2]; s32 f08; s32 f0c; s32 f10; };

struct S_02001bbc {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001c84 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001d2c {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001de0 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001e5c {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001ef4 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001f78 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02002004 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02002080 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_0200216c {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02002200 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_020022c8 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_020023c4 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02002450 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

typedef struct OrbitingSceneObjectSprite {
    u8 padding_00[5];
    u8 flags_05_low : 5;
    u8 flags_05_bit_5 : 1;
    u8 flags_05_high : 2;
    u8 padding_06[3];
    u8 flags_09_low : 2;
    u8 flags_09_mode : 2;
    u8 flags_09_high : 4;
    u8 padding_0a[18];
    u8 palette;
    u8 padding_1d[10];
    u8 state;
} OrbitingSceneObjectSprite;

typedef struct OrbitingSceneObject {
    u8 padding_00[8];
    s32 x;
    s32 y;
    u8 padding_10[19];
    u8 flags_23;
    u8 padding_24[12];
    s32 orbit_angle;
    u8 padding_34[4];
    s32 orbit_center_x;
    s32 orbit_center_y;
    u8 padding_40[16];
    OrbitingSceneObjectSprite *sprite;
    u8 padding_54;
    u8 mode;
    u8 state;
    u8 padding_57[5];
    u8 active;
    u8 padding_5d[4];
    u8 visible;
    u8 padding_62[10];
    u32 callback;
} OrbitingSceneObject;

extern u8 Data_0200b2bc[];
extern u8 Value_00000071;
extern u8 Value_00000072;
extern u8 Value_0000007b;
extern u8 Value_0000007c;
extern u8 Value_0000007d;
extern u8 Data_0200b310[];
extern u8 Data_0200b358[];
extern u8 Data_0200b3a0[];
extern u8 Data_0200b400[];
extern u8 Data_0200b448[];
extern u8 Data_0200b478[];
extern u8 Value_00000086;
extern u8 Value_0000007e;
extern u8 Data_0200b610[];
extern u8 Data_0200b718[];
extern u8 Data_0200b850[];
extern u8 Data_0200b5f8[];
extern u8 Data_0200b904[];
extern u8 Data_0200b8e0[];
extern u8 Data_0200b9f4[];
extern u8 Data_0200bd48[];
extern u8 Data_0200bd6c[];
extern u8 Data_0200b880[];
extern u8 Data_00000071[];
extern u8 Data_0000007b[];
extern u8 Data_0000007d[];
extern u8 Data_0000007e[];
extern u8 Data_00000086[];
extern s16 Data_02000240_t[][1];
extern u8 Value_0200ae71;
Obj *Func_0200481e(s32);
Obj *Func_02004826(s32);
void Func_020047ee(Obj *, s32, s32, s32);
void Func_0200481a(Obj *, s32, s32, s32);
void Func_0200484c(Obj *);
void Func_020032be(int arg0);
void Func_020032ca(int arg0);
void Func_020032d6(int arg0);
void Func_020032e2(int arg0);
void Func_020032ee(int arg0);
void Func_020032fa(int arg0);
void Func_02003306(int arg0);
void Func_02003312(int arg0);
void Func_0200331e(int arg0);
void Func_0200332a(int arg0);
void Func_02003336(int arg0);
void Func_02003342(int arg0);
void Func_02001440(void);
void Func_02002638(void);
s32 Func_02001bc2(struct V6 *arg0);
void Func_02001d6e(struct V6 arg0);
void Func_02004a60(s32 arg0, s32 arg1, struct V *arg2);
s32 Func_02004ac0(struct S *arg0, struct V *arg1);
void Func_02003f86(s32 arg0, s32 arg1);
void Func_02003f8e(s32 arg0, s32 arg1);
struct S *Func_020048f4(void);
struct S *Func_02004948(void);
void Func_02003184(s32, s32, s32, s32);
void Func_02003190(s32, s32, s32, s32);
void Func_020031f0(s32, s32, s32, s32);
void Func_020031fc(s32, s32, s32, s32);
void Func_02003208(s32, s32, s32, s32);
void Func_02003214(s32, s32, s32, s32);
void Func_020031cc(s32, s32, s32, s32);
void Func_020031d8(s32, s32, s32, s32);
void Func_020031e4(s32, s32, s32, s32);
void Func_02003244(s32, s32, s32, s32);
void Func_02003250(s32, s32, s32, s32);
void Func_0200325c(s32, s32, s32, s32);
u8 *Func_02003a0e(s32 kind, s32 x, s32 y, s32 z);
u8 *Func_02003a66(s32 kind, s32 x, s32 y, s32 z);
struct SceneObject *Func_02003e96(void);
void Func_0200194a(s32, s32, s32, s32, s32, s32, s32, struct EffectParams *);
void Func_02003fb0_a(s32 arg0);
void Func_02001914();
void Func_0200191a();
void Func_020023cc();
void Func_0200265e();
void Func_020026be();
void Func_020028b4();
void Func_02004044_b();
s32 Func_020040d0();
void Func_020040e0();
s32 Func_020040f4();
void Func_0200411c();
s32 Func_02004130();
void Func_02004158();
s32 Func_0200416c();
void Func_02004194();
void Func_020041a4();
s32 Func_020041a8();
u8 *Func_020042a4();
s32 Func_020042da();
s32 Func_02004318();
s32 Func_0200473a();
s32 Func_0200475e();
s32 Func_02004782();
s32 Func_020047a6();
u8 *Func_02004598(s32 arg0);
s32 Func_020045aa(s32 arg0);
void Func_02004580(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
u8 *Func_020045d8(s32 arg0);
s32 Func_020045ea(s32 arg0);
void Func_020045c0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
u8 *Func_0200461c(s32 arg0);
s32 Func_0200462e(s32 arg0);
void Func_02004604(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
u8 *Func_02004660(s32 arg0);
s32 Func_02004672(s32 arg0);
void Func_02004648(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02004658(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
struct S_02001b14 *Func_02004ba4(s32 arg0);
s32 Func_02003576(struct S_02001b14 *arg0, struct V *arg1);
struct S_02001b14 *Func_02004bd0(s32 arg0);
s32 Func_020035a4(struct S_02001b14 *arg0, struct V *arg1);
struct S_02001b14 *Func_02004bf8(s32 arg0);
s32 Func_020035ca(struct S_02001b14 *arg0, struct V *arg1);
struct S_02001b14 *Func_02004c24(s32 arg0);
s32 Func_020035f8(struct S_02001b14 *arg0, struct V *arg1);
struct S_02001b14 *Func_02004c4c(s32 arg0);
struct S_02001b14 *Func_02004c56(s32 arg0);
struct S_02001b14 *Func_02004c60(s32 arg0);
struct S_02001b14 *Func_02004c6a(s32 arg0);
struct S_02001b14 *Func_02004cda(s32 arg0);
void Func_02003376(s32 arg0, s32 arg1, s32 arg2);
void Func_02003386(s32 arg0, s32 arg1, s32 arg2);
void Func_020033b2(s32 arg0, s32 arg1, s32 arg2);
void Func_02003758_a(void);
struct S_02001b14 *Func_02004d14(s32 arg0);
struct S_02001b14 *Func_02004d1e(s32 arg0);
struct S_02001b14 *Func_02004d28(s32 arg0);
struct S_02001b14 *Func_02004d84(s32 arg0);
void Func_0200342a(s32 arg0, s32 arg1, s32 arg2);
void Func_02003436(s32 arg0, s32 arg1, s32 arg2);
void Func_02003440(s32 arg0, s32 arg1, s32 arg2);
void Func_02003454(s32 arg0, s32 arg1, s32 arg2);
void Func_02003464(s32 arg0, s32 arg1, s32 arg2);
struct S_02001b14 *Func_02004dbc(s32 arg0);
struct S_02001b14 *Func_02004dc6(s32 arg0);
struct S_02001b14 *Func_02004dd0(s32 arg0);
struct S_02001b14 *Func_02004dda(s32 arg0);
struct S_02001b14 *Func_02004e38(s32 arg0);
void Func_020034f0(s32 arg0, s32 arg1, s32 arg2);
void Func_02003512(s32 arg0, s32 arg1, s32 arg2);
struct S_02001b14 *Func_02004e70(s32 arg0);
struct S_02001b14 *Func_02004e7a(s32 arg0);
struct S_02001b14 *Func_02004eb2(s32 arg0);
void Func_0200357c(s32 arg0, s32 arg1, s32 arg2);
void Func_02003588(s32 arg0, s32 arg1, s32 arg2);
void Func_02003592(s32 arg0, s32 arg1, s32 arg2);
struct S_02001b14 *Func_02004eec(s32 arg0);
struct S_02001b14 *Func_02004ef6(s32 arg0);
struct S_02001b14 *Func_02004f08(s32 arg0);
struct S_02001b14 *Func_02004f1e(s32 arg0);
struct S_02001b14 *Func_02004f4c(s32 arg0);
void Func_0200361e(s32 arg0, s32 arg1, s32 arg2);
void Func_0200362c(s32 arg0, s32 arg1, s32 arg2);
struct S_02001b14 *Func_02004f84_a(s32 arg0);
struct S_02001b14 *Func_02004f8e(s32 arg0);
struct S_02001b14 *Func_02004fa0(s32 arg0);
struct S_02001b14 *Func_02004fce(s32 arg0);
void Func_0200369e(s32 arg0, s32 arg1, s32 arg2);
void Func_020036ae(s32 arg0, s32 arg1, s32 arg2);
struct S_02001b14 *Func_02005008(s32 arg0);
struct S_02001b14 *Func_02005012_a(s32 arg0);
struct S_02001b14 *Func_0200505c(s32 arg0);
void Func_02003714(s32 arg0, s32 arg1, s32 arg2);
void Func_0200372a(s32 arg0, s32 arg1, s32 arg2);
void Func_02003736(s32 arg0, s32 arg1, s32 arg2);
struct S_02001b14 *Func_02005094(s32 arg0);
struct S_02001b14 *Func_0200509e(s32 arg0);
struct S_02001b14 *Func_020050d6(s32 arg0);
void Func_0200379c(s32 arg0, s32 arg1, s32 arg2);
void Func_020037b0(s32 arg0, s32 arg1, s32 arg2);
struct S_02001b14 *Func_02005110(s32 arg0);
struct S_02001b14 *Func_0200511a(s32 arg0);
struct S_02001b14 *Func_02005124(s32 arg0);
struct S_02001b14 *Func_0200512e(s32 arg0);
struct S_02001b14 *Func_020051c4(s32 arg0);
void Func_02003846(s32 arg0, s32 arg1, s32 arg2);
void Func_02003870(s32 arg0, s32 arg1, s32 arg2);
void Func_0200389e(s32 arg0, s32 arg1, s32 arg2);
struct S_02001b14 *Func_020051fc(s32 arg0);
struct S_02001b14 *Func_02005206(s32 arg0);
void Func_02005210(s32 arg0);
struct S_02001b14 *Func_02005258(s32 arg0);
void Func_0200390a(s32 arg0, s32 arg1, s32 arg2);
void Func_02003932(s32 arg0, s32 arg1, s32 arg2);
struct S_02001b14 *Func_02005290(s32 arg0);
struct S_02001b14 *Func_0200529a(s32 arg0);
struct S_02001b14 *Func_020052a4(s32 arg0);
struct S_02001b14 *Func_020052ae(s32 arg0);
struct S_02001b14 *Func_02005320(s32 arg0);
void Func_020039be(s32 arg0, s32 arg1, s32 arg2);
void Func_020039d6(s32 arg0, s32 arg1, s32 arg2);
void Func_020039fa(s32 arg0, s32 arg1, s32 arg2);
struct S_02001b14 *Func_02005358(s32 arg0);
struct S_02001b14 *Func_02005362(s32 arg0);
struct S_02001b14 *Func_0200536c(s32 arg0);
struct S_02001b14 *Func_02005376(s32 arg0);
struct S_02001b14 *Func_0200541c(s32 arg0);
void Func_02003a86(s32 arg0, s32 arg1, s32 arg2);
void Func_02003a9a(s32 arg0, s32 arg1, s32 arg2);
void Func_02003aaa(s32 arg0, s32 arg1, s32 arg2);
void Func_02003af6(s32 arg0, s32 arg1, s32 arg2);
struct S_02001b14 *Func_02005454(s32 arg0);
struct S_02001b14 *Func_0200545e(s32 arg0);
struct S_02001b14 *Func_02005468(s32 arg0);
struct S_02001b14 *Func_020054a6(s32 arg0);
void Func_02003b80(s32 arg0, s32 arg1, s32 arg2);
struct S_02001b14 *Func_020054e0(s32 arg0);
struct S_02001b14 *Func_020054ea(s32 arg0);
struct S_02001b14 *Func_0200551e(s32 arg0);
void Func_02003be8(s32 arg0, s32 arg1, s32 arg2);
void Func_02003bf8(s32 arg0, s32 arg1, s32 arg2);
u8 *Func_0200568a();
u8 *Func_02005694();
u8 *Func_0200569e();
u8 *Func_02005758();
void Func_02003da8();
void Func_02003db2();
void Func_02003dce();
void Func_02003dda();
void Func_02003df0();
void Func_02003dfc_a();
void Func_02003e12();
void Func_02003e22();
void Func_02003e32();
u8 *Func_020058e8();
u8 *Func_020058f2();
u8 *Func_020058fc();
u8 *Func_02005906();
u8 *Func_020059d4();
void Func_0200401a();
void Func_02004024();
void Func_02004044();
void Func_02004050();
void Func_02004070();
void Func_0200407c();
void Func_02004094();
void Func_020040a4();
void Func_020040b4_a();
u8 *Func_02005c1a();
u8 *Func_02005c24();
u8 *Func_02005c2e();
u8 *Func_02005ca4();
void Func_0200437e();
u8 *Func_02005ce6();
u8 *Func_02005cf0();
u8 *Func_02005cfa();
u8 *Func_02005d62();
void Func_0200443c();
u8 *Func_02005da6();
u8 *Func_02005db0();
u8 *Func_02005dba();
u8 *Func_02005e28();
void Func_02004502();
u8 *Func_02005e6a();
u8 *Func_02005e74();
u8 *Func_02005ec8();
void Func_020045a2_a();
OrbitingSceneObject *Func_02005f6c(void);
u8 *Func_02005f3c(s32, s32);

/*
 * Distance between two three-component 16.16 fixed-point positions.
 *
 * Each argument walks three consecutive 16.16 words in x, y, z order. The
 * per-axis deltas are taken in fixed point, shifted down to integers, squared,
 * and summed; the total is passed to the resident IWRAM integer square root.
 *
 * Expressions are preserved exactly as reconstructed: the walking-pointer form
 * is load-bearing for byte-identity and must not become struct field access.
 */

/* Advance actor 18 and its companions along the Z-axis escape route.
   Shared branches preserve the transition call sites used by multiple rows. */

/* Old-style declarations: overlay imports vary in arity between call sites.
   One import name per call site: bl displacements are per-site. */

/* This overlay's transition starter at 0x02001774, one name per site. */

/* Advance actor 9 and its companions along the Z-axis escape route.
   Shared branches preserve the transition call sites used by multiple rows. */

/* Return leg of the slot-14 transition beat: negated ids. */

/* This overlay's transition starter at 0x02001774. */

/* Outbound leg of the slot-14 transition beat. */

/* Return leg of the slot-16 transition beat: negated ids. */

/* Outbound leg of the slot-16 transition beat. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

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

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02001214(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02001494(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_0200167c(s32 (*f)(), s32 a0)
{
    return f(a0);
}

u8 *Func_02005680();                    /* scene entity by selector */

u8 *Func_020058dc();                    /* scene entity by selector */

u8 *Func_02005c10();                    /* scene entity by selector */

u8 *Func_02005cdc();                    /* scene entity by selector */

u8 *Func_02005d9c();                    /* scene entity by selector */

u8 *Func_02005e60();                    /* scene entity by selector */

/* Copy the low two mode bits into the object's owner record. */
void SetEffectOwnerMode(u8 *object, s32 mode)
{
    struct Owner *owner = *(struct Owner **)(object + 80);
    owner->mode = mode;
}

/* Spawn and configure the mode-14 object variant. */

/* Spawn and configure the mode-15 object variant. */

/* Returns the party record; only its presentation block at +80 is read. */
u8 *SpawnMode14Effect(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *object = Func_02003a0e(kind, x, y, z);

    if (object != 0) {
        u8 *owner = *(u8 **)(object + 80);
        s32 mask = ~12;
        mask &= owner[9];
        owner[9] = mask;
        object[85] = 0;
        object[89] = 8;
        Actor_SetSpriteFlags(object, 0);
        Object_SetPalette(object, 14);
        Object_SetBlendMode(object, 1);
        return object;
    }
    return 0;
}

u8 *SpawnMode15Effect(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *object = Func_02003a66(kind, x, y, z);

    if (object != 0) {
        u8 *owner = *(u8 **)(object + 80);
        s32 mask = ~12;
        mask &= owner[9];
        mask |= 4;
        owner[9] = mask;
        object[85] = 0;
        object[89] = 8;
        Actor_SetSpriteFlags(object, 0);
        Object_SetPalette(object, 15);
        {
            s32 mask2 = 254;
            mask2 &= object[35];
            mask2 |= 2;
            object[35] = mask2;
        }
        return object;
    }
    return 0;
}

/* Deliberate no-op callback. */
void NoOpEffectCallback(void) {}

s32 SelectPrimarySceneData(void)
{
    extern s16 Data_02000240[];

    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000071) {
        return (s32)Data_0200b310;
    }
    if (v == (s32)&Value_00000072) {
        return (s32)Data_0200b358;
    }
    if (v == (s32)&Value_0000007b) {
        return (s32)Data_0200b3a0;
    }
    if (v == (s32)&Value_0000007c) {
        return (s32)Data_0200b400;
    }
    if (v == (s32)&Value_0000007d) {
        return (s32)Data_0200b448;
    }
    return (s32)Data_0200b478;
}

/* Complete four-byte leaf: movs r0,#0 followed by bx lr. */
s32 GetEmptySceneData(void)
{
    return 0;
}

/* Complete eight-byte literal-address getter, including its sole pool word. */
u8 *GetDefaultSceneData(void)
{
    return (u8 *)0x0200b508;
}

s32 SelectSecondarySceneData(void)
{
    extern s16 Data_02000240[];

    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000071) {
        return (s32)Data_0200b610;
    }
    if (v == (s32)&Value_0000007b) {
        return (s32)Data_0200b718;
    }
    if (v <= (s32)&Value_00000086 && v >= (s32)&Value_0000007e) {
        return (s32)Data_0200b850;
    }
    return (s32)Data_0200b5f8;
}

void AdvanceEffectMotion(struct Effect *effect)
{

    s32 velocity_z;
    struct Sprite *sprite;
    s32 velocity_x;

    /* Macro-shaped block keeps the following Z load after the Y store. */
    do {
        velocity_x = effect->velocity[0];
        effect->position[0] += velocity_x;
        effect->position[1] += effect->velocity[1];
    } while (0);
    velocity_z = effect->velocity[2];
    effect->position[2] += velocity_z;

    effect->velocity[0] = velocity_x - Math_Divide(velocity_x, 18);
    effect->velocity[2] = velocity_z - velocity_z / 16;

    effect->accum18 += effect->rate30;
    effect->accum1c += effect->rate34;

    sprite = effect->sprite;
    sprite->angle += effect->step64;
}

void SpawnRadialEffectBurst(void)
{
    struct SceneObject *object;
    struct Vec vec;
    struct EffectParams params;
    u32 angle_step;
    s32 angle;
    s32 x;
    s32 z;

    object = Func_02003e96();
    params.callback = 0x02008da5;
    for (angle_step = 0; angle_step <= 16; angle_step += 2) {
        angle = angle_step << 12;
        vec.x = Math_Cos(angle);
        vec.y = 0;
        z = Math_Sin(angle);
        x = vec.x;
        vec.z = z;
        x = x + Math_Divide(x, 3);
        vec.x = x;
        Func_0200194a(object->x, object->y, object->z, x, vec.y, z, 0x01000001, &params);
    }
}

void InitializePrologueSceneState(void)
{
    extern u8 Data_02000240[];

    u8 *base;

    Event_Begin();
    base = Data_02000240;
    Func_02003f86(*(s16 *)(base + 448), 5);
    base[555] = 3;
    Func_02003f8e(84, 5);
    Event_End();
}

s32 SelectTertiarySceneData(void)
{
    extern s16 Data_02000240[];

    s16 scene_id = Data_02000240[224];

    if (scene_id == (s32)&Value_00000071) {
        return (s32)Data_0200b904;
    }
    if (scene_id == (s32)&Value_00000072) {
        return (s32)Data_0200b8e0;
    }
    if (scene_id == (s32)&Value_0000007b) {
        return (s32)Data_0200b9f4;
    }
    if (scene_id == (s32)&Value_0000007c) {
        return (s32)Data_0200bd48;
    }
    if (scene_id == (s32)&Value_0000007d) {
        return (s32)Data_0200bd6c;
    }
    return (s32)Data_0200b880;
}

void StartScriptedSceneMessage(s32 message_id)
{
    Func_02003fb0_a(message_id);
    Actor_SetPosition(8, 0, 0);
    GameFlag_Set(4055);
    Item_ShowFound(181, 3);
    Party_GiveItem(181, 0);
    Event_End();
}

s32 Func_02000f70(void)
{
    extern u8 Data_03001ebc[];

    u8 *rec7;
    s32 scene;
    s32 cur;
    s32 lo;

    ((RecWord *)(*(u8 *volatile *)Data_03001ebc + 0x1c0))->w = 0x204;
    scene = Data_02000240_t[224][0];
    if (scene == (s32)Data_0000007b) {
        Func_020028b4();
        return 0;
    }
    if (scene == (s32)Data_0000007d) {
        if (GameFlag_IsSet(0xef7) == 0) {
            Map_CopyCellAttributes(0, 3, 1, 1, 13, 40);
            Map_CopyCellAttributes(0, 2, 1, 1, 15, 40);
            MapObject_SetPosition(101, 0xd80000, 0x2880000);
        }
        if (Data_02000240_t[224][0] == scene) {
            if (Data_02000240_t[225][0] != 5) {
                if (GameFlag_IsSet(0x8d1) == 0) {
                    return 0;
                }
            }
            GameFlag_Set(0x8d1);
            Map_CopyCellAttributes(0, 1, 1, 1, 13, 30);
            MapObject_SetPosition(100, 0xd80000, 0x1e80000);
            return 0;
        }
    }
    cur = Data_02000240_t[224][0];
    if (cur == (s32)Data_00000071) {
        s32 rec;

        Func_020026be();
        rec = Func_020040d0(8);
        *(volatile s32 *)(rec + 56) = 0x810000;
        Func_02001914(9);
        Func_0200191a(10);
        if (GameFlag_IsSet(0x240) != 0) {
            rec7 = Value1(Func_020040f4, 11);
            if ((s32)rec7 != 0) {
                rec7[89] = 0;
                Object_SetAnimation((s32)rec7, 4);
                Actor_SetSpriteFlags((s32)rec7, 0);
            }
            Call4(Func_020040e0, 0, 0x1300000, 0x1700000, 253);
        }
        if (GameFlag_IsSet(0x241) != 0) {
            rec7 = Value1(Func_02004130, 12);
            if ((s32)rec7 != 0) {
                rec7[89] = 0;
                Object_SetAnimation((s32)rec7, 4);
                Actor_SetSpriteFlags((s32)rec7, 0);
            }
            Call4(Func_0200411c, 0, 0x500000, 0x1700000, 253);
        }
        if (GameFlag_IsSet(0x242) != 0) {
            rec7 = Value1(Func_0200416c, 13);
            if ((s32)rec7 != 0) {
                rec7[89] = 0;
                Object_SetAnimation((s32)rec7, 4);
                Actor_SetSpriteFlags((s32)rec7, 0);
            }
            Call4(Func_02004158, 0, 0x600000, 0x1500000, 253);
        }
        if (GameFlag_IsSet(0x243) != 0) {
            rec7 = Value1(Func_020041a8, 14);
            if ((s32)rec7 != 0) {
                rec7[89] = 0;
                Object_SetAnimation((s32)rec7, 4);
                Actor_SetSpriteFlags((s32)rec7, 0);
            }
            Call4(Func_02004194, 0, 0x900000, 0x1400000, 253);
            Call4(Func_020041a4, 0, 0x2f00000, 0x1400000, 253);
        }
        if (GameFlag_IsSet(0xfd7) != 0) {
            return 0;
        }
        Func_02004044_b(8);
        return 0;
    }
    lo = (s32)Data_0000007e;
    if (cur == lo) {
        if (GameFlag_IsSet(0xef4) == 0) {
            Map_CopyCellAttributes(0, 0, 1, 1, 37, 10);
            MapObject_SetPosition(100, 0x2580000, 0xa80000);
        }
    }
    cur = Data_02000240_t[224][0];
    if (cur >= lo) {
        if (cur <= (s32)Data_00000086) {
            Func_020023cc();
            if (Data_02000240_t[225][0] == 5) {
                Func_0200265e();
            }
        }
    }
    return 0;
}

void FieldScene_RunScene3b2SequenceA(void)
{
    u32 i;
    s32 rec7;
    u8 *rec8;
    s32 record;

    rec8 = Func_020042a4(8);
    rec7 = GameFlag_IsSet((Data_02000240_t[224][0] + (0x8d2 - (s32)Data_0000007e)));
    if (rec7 != 0) {
        Actor_SetPosition(8, 0x28a0000, 0xa80000);
        *(volatile s32 *)((s32)rec8 + 12) = -0x200000;
        record = Func_020042da(8);
        Actor_SetSpriteFlags(record, 0);
        Actor_SetSpritePriority(8, 3);
        rec8[85] = 0;
        {
            u8 value = *(volatile u8 *)&rec8[35];

            rec8[35] = (u8)(value | 2);
        }
        Map_CopyCellAttributes(42, 10, 1, 1, 40, 10);
    } else {
        *(u8 *)(Func_02004318(8) + 85) = rec7;
    }
}

void RunPrologueSceneSetup(void)
{
    Event_Begin();
    Func_02001440();
    Event_End();
    Func_02002638();
}

void StartSceneScript37(void)
{
    Map_AnimateCells(Data_0200b2bc, 37, 7);
    Audio_PlayCue(183);
    Event_RequestExit(4);
}

void FieldScene_RunScene3b2_02001494(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetSpeed(0, 0x6666, 0x3333);
    Event_OpenScreen();
    Event_WaitForScreen();
    GameFlag_Set((Data_02000240_t[224][0] + (0x8c8 - (s32)Data_0000007e)));
    Event_Wait(30);
    Map_AnimateCells(0x200b2bc, 44, 7);
    Actor_CenterAndWalk(0, 3, -16);
    Event_RequestExit(3);
    Event_End();
}

void ConfigureSceneActor11(s32 actor_id)
{
    s32 a = 0x1300000;
    s32 b = 0x1700000;
    u8 *p = Func_02004598(11);

    if (p != 0) {
        p[89] = 0;
    }
    Actor_SetSpriteFlags(Func_020045aa(actor_id), 0);
    Func_02004580(0, a, b, 253);
    GameFlag_Set(576);
}

void ConfigureSceneActor12(s32 actor_id)
{
    s32 a = 0x500000;
    s32 b = 0x1700000;
    u8 *p = Func_020045d8(12);

    if (p != 0) {
        p[89] = 0;
    }
    Actor_SetSpriteFlags(Func_020045ea(actor_id), 0);
    Func_020045c0(0, a, b, 253);
    GameFlag_Set(577);
}

void ConfigureSceneActor13(s32 actor_id)
{
    s32 a = 0x600000;
    s32 b = 0x1500000;
    u8 *p = Func_0200461c(13);

    if (p != 0) {
        p[89] = 0;
    }
    Actor_SetSpriteFlags(Func_0200462e(actor_id), 0);
    Func_02004604(0, a, b, 253);
    GameFlag_Set(578);
}

void ConfigureSceneActor14(s32 actor_id)
{
    s32 a = 0x900000;
    s32 b = 0x1400000;
    s32 c = 0x2f00000;
    s32 d = 0x1400000;
    u8 *p = Func_02004660(14);

    if (p != 0) {
        p[89] = 0;
    }
    Actor_SetSpriteFlags(Func_02004672(actor_id), 0);
    Func_02004648(0, a, b, 253);
    Func_02004658(0, c, d, 253);
    GameFlag_Set(579);
}

void ShowForgetEverythingMessage(void)
{
    GameFlag_Set(2244);
    {
        s32 k4 = 8, k5 = 21;

        Map_CopyCellAttributes(0, 0, 1, 1, k4, k5);
    }
}

void ShowHelpYouForgetMessage(void)
{
    GameFlag_Set(2245);
}

void ShowDamagedDoorMessage(void)
{
    GameFlag_Set(2246);
}

void ShowSaveMyLifeMessage(void)
{
    GameFlag_Set(2247);
}

void FieldScene_RunScene3b2_0200167c(void)
{
    s32 record;

    if (GameFlag_IsSet(0x8c4) != 0) {
        Map_CopyCellAttributes(0, 0, 1, 1, 8, 21);
        Actor_SetPosition(15, 0x3c80000, 0x3c80000);
    } else {
        record = Func_0200473a(15);
        *(s32 *)(record + 28) = 0x19999;
    }
    if (GameFlag_IsSet(0x8c5) != 0) {
        Actor_SetPosition(16, 0x3c80000, 0x3c80000);
    } else {
        record = Func_0200475e(16);
        *(s32 *)(record + 28) = 0x19999;
    }
    if (GameFlag_IsSet(0x8c6) != 0) {
        Actor_SetPosition(17, 0x3c80000, 0x3c80000);
    } else {
        record = Func_02004782(17);
        *(s32 *)(record + 28) = 0x19999;
    }
    if (GameFlag_IsSet(0x8c7) != 0) {
        Actor_SetPosition(18, 0x3c80000, 0x3c80000);
    } else {
        record = Func_020047a6(18);
        *(s32 *)(record + 28) = 0x19999;
    }
}

void RunSceneVectorTransition(void)
{
    struct V6 transition;

    Event_Begin();
    if (Func_02001bc2(&transition) != 0) {
        Func_02001d6e(transition);
    }
    Event_End();
}

void PositionSceneActorPair(s32 actor_id, s32 x_offset, s32 z_offset)
{
    extern s32 Data_02000240[];
    Obj *p;
    Obj *q;
    s32 x;
    s32 y;

    p = Func_0200481e(Data_02000240[125]);
    q = Func_02004826(actor_id);
    Event_Begin();
    {
        x = ((p->f08 + (x_offset << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (z_offset << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        Func_020047ee(p, x, p->f0c, y);
    }
    Object_SetAnimation(p, 27);
    {
        x = ((q->f08 + (x_offset << 16)) & 0xFFF00000) + 0x80000;
        y = ((q->f10 + (z_offset << 16)) & 0xFFF00000) + 0x80000;

        q->f30 = 0x10000;
        q->f34 = 0x8000;
        Func_0200481a(q, x, q->f0c, y);
    }
    if (x_offset < 0 || z_offset < 0) {
        Object_SetAnimation(q, 4);
    } else {
        Object_SetAnimation(q, 3);
    }
    Audio_PlayCue(226);
    Func_0200484c(p);
    Audio_PlayCue(288);
    Event_End();
}

void MarkGridLeftOfSceneActor(s32 actor_mode, s32 grid_value, s32 grid_attribute)
{
    struct S_020009dc *p = Func_020048f4();

    if (p != 0) {
        s32 v;

        Actor_SetSpritePriority(actor_mode, 3);
        v = 2;
        v |= p->f23;
        p->f23 = (u8)v;
        {
            s32 k5 = p->f10 >> 20;
            s32 k4 = (p->f08 >> 20) - 1;

            Map_CopyCellAttributes(grid_value, grid_attribute, 3, 1, k4, k5);
        }
    }
}

void MarkGridAboveSceneActor(s32 actor_mode, s32 grid_value, s32 grid_attribute)
{
    struct S_020009dc *p = Func_02004948();

    if (p != 0) {
        s32 v;

        Actor_SetSpritePriority(actor_mode, 3);
        v = 2;
        v |= p->f23;
        p->f23 = (u8)v;
        {
            s32 k4 = p->f08 >> 20;
            s32 k5 = (p->f10 >> 20) - 1;

            Map_CopyCellAttributes(grid_value, grid_attribute, 1, 3, k4, k5);
        }
    }
}

void SetSceneActorModes(int actor_id)
{
    Actor_SetAnimation(actor_id, 1);
    Actor_SetAnimation(actor_id, 2);
}

void InitializeEscapeSceneActors(void)
{
    Func_02003184(8, 0x11, 0x1E, 0x15);
    Func_02003190(0xA, 0x11, 0x1F, 0x16);
    Func_020031f0(0xB, 0x14, 0x1E, 0x17);
    Func_020031fc(0xC, 0x15, 0x1E, 0x18);
    Func_02003208(0xD, 0x16, 0x1E, 0x19);
    Func_02003214(0xF, 0x17, 0x1E, 0x1A);
    Func_020031cc(0x11, 0, 0x1E, 0x1F);
    Func_020031d8(0x12, 0, 0x1F, 0x20);
    Func_020031e4(9, 0, 0x20, 0x21);
    Func_02003244(0x13, 4, 0x1E, 0x22);
    Func_02003250(0xE, 5, 0x1E, 0x23);
    Func_0200325c(0x10, 6, 0x1E, 0x24);
}

void ActivateSceneActor8(void)
{
    Func_020032be(8);
}

void ActivateSceneActor10(void)
{
    Func_020032ca(10);
}

void ActivateSceneActor11(void)
{
    Func_020032d6(11);
}

void ActivateSceneActor12(void)
{
    Func_020032e2(12);
}

void ActivateSceneActor13(void)
{
    Func_020032ee(13);
}

void ActivateSceneActor15(void)
{
    Func_020032fa(15);
}

void ActivateSceneActor17(void)
{
    Func_02003306(17);
}

void ActivateSceneActor18(void)
{
    Func_02003312(18);
}

void ActivateSceneActor9(void)
{
    Func_0200331e(9);
}

void ActivateSceneActor19(void)
{
    Func_0200332a(19);
}

void ActivateSceneActor14(void)
{
    Func_02003336(14);
}

void ActivateSceneActor16(void)
{
    Func_02003342(16);
}

s32 TryPushBlockingSceneActor(struct S_02000474 *actor)
{
    u8 *state = &actor->f55;
    s32 saved_state = *state;
    struct V destination;

    destination.a = (actor->f08 & 0xfff00000) + 0x80000;
    destination.b = actor->f0c;
    destination.c = (actor->f10 & 0xfff00000) + 0x80000;
    {
        s32 direction = (actor->f06 + 0x2000) & 0xc000;

        Func_02004a60(0x200000, direction, &destination);
    }
    if (Func_02004ac0(actor, &destination) == 0) {
        s32 t;

        Event_Begin();
        Object_SetAnimation(actor, 6);
        Task_Wait(6);
        Audio_PlayCue(152);
        Object_SetAnimation(actor, 7);
        actor->f30 = 0x30000;
        actor->f34 = 0x20000;
        actor->f28 = 0x40000;
        t = 126;
        t &= *state;
        *state = (u8)t;
        Actor_SetSpriteFlags(actor, 0);
        {
            s16 *coordinates = (s16 *)&destination;

            Actor_MoveToAndWait(0, coordinates[1], coordinates[5]);
        }
        Object_SetAnimation(actor, 6);
        Actor_SetSpriteFlags(actor, 1);
        *state = (u8)saved_state;
        Event_End();
        return 1;
    }
    return 0;
}

s32 CheckActorPathSouth(void)
{
    struct S_02001b14 *actor = Func_02004ba4(0);
    struct V destination;

    destination.a = actor->f08;
    destination.b = actor->f0c;
    destination.c = actor->f10 + -0x200000;
    return Func_02003576(actor, &destination);
}

s32 CheckActorPathNorth(void)
{
    struct S_02001b14 *actor = Func_02004bd0(0);
    struct V destination;

    destination.a = actor->f08;
    destination.b = actor->f0c;
    destination.c = actor->f10 + 0x200000;
    return Func_020035a4(actor, &destination);
}

s32 CheckActorPathWest(void)
{
    struct S_02001b14 *actor = Func_02004bf8(0);
    struct V destination;

    destination.a = actor->f08 + -0x200000;
    destination.b = actor->f0c;
    destination.c = actor->f10;
    return Func_020035ca(actor, &destination);
}

s32 CheckActorPathEast(void)
{
    struct S_02001b14 *actor = Func_02004c24(0);
    struct V destination;

    destination.a = actor->f08 + 0x200000;
    destination.b = actor->f0c;
    destination.c = actor->f10;
    return Func_020035f8(actor, &destination);
}

void UpdateEscapeRouteForActorPositions(void)
{
    s32 actor_x = Func_02004c4c(8)->f08 >> 20;
    s32 actor_z = Func_02004c56(8)->f10 >> 20;
    s32 actor_12_x = Func_02004c60(12)->f08 >> 20;
    s32 actor_15_x = Func_02004c6a(15)->f08 >> 20;

    if (actor_z == 19) {
        if (actor_12_x == 24) {
            Func_020033b2(8, 0, -80);
        } else if (actor_15_x == 24) {
            Func_02003376(8, 0, -112);
            Func_020033b2(8, 0, -32);
        } else {
            Func_02003386(8, 0, -80);
            Func_020033b2(8, 0, -112);
        }
    } else if (actor_z == 14) {
        if (actor_12_x == 24) {
            return;
        }
        if (actor_15_x == 24) {
            Func_020033b2(8, 0, -64);
        } else {
            Func_020033b2(8, 0, -112);
        }
    } else if (actor_z == 10) {
        if (actor_15_x == 24) {
            return;
        }
        Func_020033b2(8, 0, -48);
    } else {
        Func_02003758_a();
        return;
    }
    Task_Wait(2);
    {
        s32 route_end_z = Func_02004cda(8)->f10 >> 20;
        s32 route_x = actor_x - 1;

        Map_CopyCellAttributes(route_x, actor_z, 3, 1, route_x, route_end_z);
    }
    Map_CopyCellAttributes(0, 0, 3, 1, actor_x - 1, actor_z);
}

void UpdateActor8ReturnRoute(void)
{
    s32 x = Func_02004d14(8)->f08 >> 20;
    s32 y = Func_02004d1e(8)->f10 >> 20;
    s32 z = Func_02004d28(12)->f08 >> 20;

    if (y == 7) {
        if (z == 24) {
            Func_0200342a(8, 0, 48);
        } else {
            Func_02003436(8, 0, 80);
            Func_02003440(8, 0, 112);
        }
    } else if (y == 10) {
        if (z == 24) {
            return;
        }
        Func_02003454(8, 0, 144);
    } else if (y == 14) {
        Func_02003464(8, 0, 80);
    } else {
        return;
    }
    Task_Wait(2);
    {
        s32 k = Func_02004d84(8)->f10 >> 20;
        s32 m = x - 1;

        Map_CopyCellAttributes(m, y, 3, 1, m, k);
    }
    Map_CopyCellAttributes(0, 0, 3, 1, x - 1, y);
}

void UpdateActor10RetreatRoute(void)
{
    s32 x = Func_02004dbc(10)->f08 >> 20;
    s32 y = Func_02004dc6(10)->f10 >> 20;
    s32 z = Func_02004dd0(13)->f08 >> 20;
    s32 w = Func_02004dda(15)->f08 >> 20;

    if (y == 18) {
        if (w >= 31 && w <= 33) {
            Func_02003512(10, 0, -128);
        } else if (z >= 31 && z <= 33) {
            Func_02003512(10, 0, -128);
        } else {
            Func_020034f0(10, 0, -112);
            Func_02003512(10, 0, -64);
        }
    } else if (y == 10) {
        if (w >= 31 && w <= 33) {
            return;
        }
        if (z >= 31 && z <= 33) {
            return;
        }
        Func_02003512(10, 0, -48);
    } else if (y == 7) {
        return;
    }
    Task_Wait(2);
    {
        s32 k = Func_02004e38(10)->f10 >> 20;
        s32 m = x - 1;

        Map_CopyCellAttributes(m, y, 3, 1, m, k);
    }
    Map_CopyCellAttributes(0, 0, 3, 1, x - 1, y);
}

void UpdateActor10AdvanceRoute(void)
{
    s32 x = Func_02004e70(10)->f08 >> 20;
    s32 y = Func_02004e7a(10)->f10 >> 20;

    if (y != 18) {
        if (y == 10) {
            Func_0200357c(10, 0, 128);
        } else {
            Func_02003588(10, 0, 112);
            Func_02003592(10, 0, 64);
        }
        Task_Wait(2);
        {
            s32 k = Func_02004eb2(10)->f10 >> 20;
            s32 m = x - 1;

            Map_CopyCellAttributes(m, y, 3, 1, m, k);
        }
        Map_CopyCellAttributes(0, 0, 3, 1, x - 1, y);
    }
}

void UpdateActor11WestRoute(void)
{
    s32 x = Func_02004eec(11)->f08 >> 20;
    s32 y = Func_02004ef6(11)->f10 >> 20;

    if (x != 30) {
        if (x == 34) {
            if ((Func_02004f08(10)->f10 >> 20) == 18) {
                return;
            }
            Func_0200361e(11, -64, 0);
        } else if (x == 36) {
            if ((Func_02004f1e(10)->f10 >> 20) == 18) {
                Func_0200361e(11, -32, 0);
            } else {
                Func_0200362c(11, -96, 0);
            }
        }
        Task_Wait(2);
        {
            s32 k = Func_02004f4c(11)->f08 >> 20;
            s32 m = y - 1;

            Map_CopyCellAttributes(x, m, 1, 3, k, m);
        }
        Map_CopyCellAttributes(0, 0, 1, 3, x, y - 1);
    }
}

void UpdateActor11EastRoute(void)
{
    s32 x = Func_02004f84_a(11)->f08 >> 20;
    s32 y = Func_02004f8e(11)->f10 >> 20;

    if (x != 36) {
        if (x == 30) {
            if ((Func_02004fa0(10)->f10 >> 20) == 18) {
                return;
            }
            Func_0200369e(11, 96, 0);
        } else if (x == 34) {
            Func_020036ae(11, 32, 0);
        }
        Task_Wait(2);
        {
            s32 k = Func_02004fce(11)->f08 >> 20;
            s32 m = y - 1;

            Map_CopyCellAttributes(x, m, 1, 3, k, m);
        }
        Map_CopyCellAttributes(0, 0, 1, 3, x, y - 1);
    }
}

void UpdateActor12WestRoute(void)
{
    s32 x = Func_02005008(12)->f08 >> 20;
    s32 y = Func_02005012_a(12)->f10 >> 20;

    if (x == 36) {
        Func_02003714(12, -96, 0);
        Func_02003736(12, -96, 0);
    } else if (x == 34) {
        Func_0200372a(12, -96, 0);
        Func_02003736(12, -64, 0);
    } else if (x == 24) {
        return;
    }
    Task_Wait(2);
    {
        s32 k = Func_0200505c(12)->f08 >> 20;
        s32 m = y - 1;

        Map_CopyCellAttributes(x, m, 1, 3, k, m);
    }
    Map_CopyCellAttributes(0, 0, 1, 3, x, y - 1);
}

void UpdateActor12EastRoute(void)
{
    s32 x = Func_02005094(12)->f08 >> 20;
    s32 y = Func_0200509e(12)->f10 >> 20;

    if (x == 24) {
        Func_0200379c(12, 96, 0);
        Func_020037b0(12, 96, 0);
    } else if (x == 34) {
        Func_020037b0(12, 32, 0);
    } else if (x == 36) {
        return;
    }
    Task_Wait(2);
    {
        s32 k = Func_020050d6(12)->f08 >> 20;
        s32 m = y - 1;

        Map_CopyCellAttributes(x, m, 1, 3, k, m);
    }
    Map_CopyCellAttributes(0, 0, 1, 3, x, y - 1);
}

void UpdateActor13WestRoute(void)
{
    s32 x = Func_02005110(13)->f08 >> 20;
    s32 y = Func_0200511a(13)->f10 >> 20;
    s32 z = Func_02005124(10)->f10 >> 20;
    s32 w = Func_0200512e(15)->f08 >> 20;

    if (x == 36) {
        if (w == 34) {
            Func_0200389e(13, -16, 0);
        } else if (z == 7) {
            Func_0200389e(13, -32, 0);
        } else if (w == 30) {
            Func_0200389e(13, -80, 0);
        } else {
            Func_02003846(13, -96, 0);
            Func_0200389e(13, -80, 0);
        }
    } else if (x == 35) {
        if (w == 34) {
            return;
        } else if (z == 7) {
            Func_0200389e(13, -16, 0);
        } else if (w == 30) {
            Func_0200389e(13, -64, 0);
        } else {
            Func_02003870(13, -80, 0);
            Func_0200389e(13, -80, 0);
        }
    } else if (x == 34) {
        if (z == 7) {
            return;
        }
        if (w == 30) {
            Func_0200389e(13, -48, 0);
        } else {
            Func_0200389e(13, -144, 0);
        }
    } else if (x == 31) {
        if (w == 30) {
            return;
        }
        Func_0200389e(13, -96, 0);
    } else if (x == 25) {
        return;
    }
    Task_Wait(2);
    {
        s32 k = Func_020051c4(13)->f08 >> 20;
        s32 m = y - 1;

        Map_CopyCellAttributes(x, m, 1, 3, k, m);
    }
    Map_CopyCellAttributes(0, 0, 1, 3, x, y - 1);
}

void UpdateActor13EastRoute(void)
{
    s32 x = Func_020051fc(13)->f08 >> 20;
    s32 y = Func_02005206(13)->f10 >> 20;

    Func_02005210(15);
    if (x == 25) {
        Func_0200390a(13, 96, 0);
        Func_02003932(13, 80, 0);
    } else if (x == 31) {
        Func_02003932(13, 80, 0);
    } else if (x == 34) {
        Func_02003932(13, 32, 0);
    } else if (x == 35) {
        Func_02003932(13, 16, 0);
    } else if (x == 36) {
        return;
    }
    Task_Wait(2);
    {
        s32 k = Func_02005258(13)->f08 >> 20;
        s32 m = y - 1;

        Map_CopyCellAttributes(x, m, 1, 3, k, m);
    }
    Map_CopyCellAttributes(0, 0, 1, 3, x, y - 1);
}

void UpdateActor15WestRoute(void)
{
    s32 x = Func_02005290(15)->f08 >> 20;
    s32 y = Func_0200529a(15)->f10 >> 20;
    s32 z = Func_020052a4(8)->f10 >> 20;
    s32 w = Func_020052ae(10)->f10 >> 20;

    if (x == 35) {
        if (w == 7) {
            Func_020039fa(15, -16, 0);
        } else if (z == 7) {
            Func_020039fa(15, -112, 0);
        } else {
            Func_020039be(15, -96, 0);
            Func_020039fa(15, -80, 0);
        }
    } else if (x == 34) {
        if (w == 7) {
            return;
        }
        Func_020039d6(15, -96, 0);
        Func_020039fa(15, -64, 0);
    } else if (x == 33) {
        Func_020039fa(15, -144, 0);
    } else if (x == 31) {
        Func_020039fa(15, -80, 0);
    } else if (x == 30) {
        Func_020039fa(15, -96, 0);
    } else if (x == 24) {
        return;
    }
    Task_Wait(2);
    {
        s32 k = Func_02005320(15)->f08 >> 20;
        s32 m = y - 1;

        Map_CopyCellAttributes(x, m, 1, 3, k, m);
    }
    Map_CopyCellAttributes(0, 0, 1, 3, x, y - 1);
}

void UpdateActor15EastRoute(void)
{
    s32 x = Func_02005358(15)->f08 >> 20;
    s32 y = Func_02005362(15)->f10 >> 20;
    s32 z = Func_0200536c(10)->f10 >> 20;
    s32 w = Func_02005376(13)->f08 >> 20;

    if (x == 24) {
        if (z == 7 || w == 31) {
            Func_02003af6(15, 96, 0);
        } else if (w == 34) {
            Func_02003a86(15, 64, 0);
            Func_02003af6(15, 80, 0);
        } else if (w == 35) {
            Func_02003a9a(15, 80, 0);
            Func_02003af6(15, 80, 0);
        } else {
            Func_02003aaa(15, 80, 0);
            Func_02003af6(15, 96, 0);
        }
    } else if (x == 30 || w == 31) {
        if (z == 7) {
            return;
        }
        if (w == 34) {
            Func_02003af6(15, 48, 0);
        } else if (w == 35) {
            Func_02003af6(15, 64, 0);
        } else {
            Func_02003af6(15, 80, 0);
        }
    } else if (x == 33) {
        if (w == 34) {
            return;
        }
        if (w == 35) {
            Func_02003af6(15, 16, 0);
        } else {
            Func_02003af6(15, 32, 0);
        }
    } else if (x == 34) {
        Func_02003af6(15, 16, 0);
    } else if (x == 35) {
        return;
    }
    Task_Wait(2);
    {
        s32 k = Func_0200541c(15)->f08 >> 20;
        s32 m = y - 1;

        Map_CopyCellAttributes(x, m, 1, 3, k, m);
    }
    Map_CopyCellAttributes(0, 0, 1, 3, x, y - 1);
}

void UpdateActor17SouthRoute(void)
{
    s32 x = Func_02005454(17)->f08 >> 20;
    s32 y = Func_0200545e(17)->f10 >> 20;
    s32 z = Func_02005468(19)->f08 >> 20;

    if (y == 19) {
        if (z >= 3 && z <= 5) {
            Func_02003b80(17, 0, -16);
        } else {
            Func_02003b80(17, 0, -64);
        }
    } else if (y == 18) {
        if (z >= 3 && z <= 5) {
            return;
        }
        Func_02003b80(17, 0, -48);
    } else if (y == 15) {
        return;
    }
    Task_Wait(2);
    {
        s32 k = Func_020054a6(17)->f10 >> 20;
        s32 m = x - 1;

        Map_CopyCellAttributes(m, y, 3, 1, m, k);
    }
    Map_CopyCellAttributes(0, 0, 3, 1, x - 1, y);
}

void UpdateActor17NorthRoute(void)
{
    s32 x = Func_020054e0(17)->f08 >> 20;
    s32 y = Func_020054ea(17)->f10 >> 20;

    if (y == 15) {
        Func_02003be8(17, 0, 64);
    } else if (y == 18) {
        Func_02003bf8(17, 0, 16);
    } else if (y == 19) {
        return;
    }
    Task_Wait(2);
    {
        s32 k = Func_0200551e(17)->f10 >> 20;

        s32 m = x - 1;

        Map_CopyCellAttributes(m, y, 3, 1, m, k);
    }
    Map_CopyCellAttributes(0, 0, 3, 1, x - 1, y);
}

void AdvanceActor18AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companion19Column;
    s32 companion14Column;

    s32 permuted_5;
    permuted_5 = *(s32 *)(Func_02005680(18) + 8) >> 20;
    row = *(s32 *)(Func_0200568a(18) + 16) >> 20;
    column  = permuted_5;
    companion19Column = *(s32 *)(Func_02005694(19) + 8) >> 20;
    companion14Column = *(s32 *)(Func_0200569e(14) + 8) >> 20;

    if (row == 9) {
        if ((u32)(companion14Column - 6) <= 2) {
            goto transition32;
        }
        if ((u32)(companion19Column - 6) <= 2) {
            goto transition80;
        }
        /* This arm runs two transitions back to back. */
        Func_02003da8(18, 0, 64);
        Func_02003db2(18, 0, 96);
    } else if (row == 11) {
        if ((u32)(companion14Column - 6) <= 2) {
            return;
        }
        if ((u32)(companion19Column - 6) <= 2) {
            Func_02003dce(18, 0, 48);
        } else {
            Func_02003dda(18, 0, 128);
        }
    } else if (row == 12) {
        if ((u32)(companion19Column - 6) <= 2) {
transition32:
            Func_02003df0(18, 0, 32);
        } else {
            Func_02003dfc_a(18, 0, 112);
        }
    } else if (row == 14) {
        if ((u32)(companion19Column - 6) <= 2) {
            return;
        }
transition80:
        Func_02003e12(18, 0, 80);
    } else if (row == 15) {
        Func_02003e22(18, 0, 64);
    } else if (row == 18) {
        Func_02003e32(18, 0, 16);
    } else if (row == 19) {
        return;
    }

    Task_Wait(2);

    column -= 1;
    Map_CopyCellAttributes(column, row, 3, 1,
                  column, *(s32 *)(Func_02005758(18) + 16) >> 20);
    Map_CopyCellAttributes(0, 0, 3, 1, column, row);
}

void AdvanceActor9AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companion19Column;
    s32 companion14Column;
    s32 companion16Column;

    s32 permuted_6;
    permuted_6 = *(s32 *)(Func_020058dc(9) + 8) >> 20;
    row = *(s32 *)(Func_020058e8(9) + 16) >> 20;
    column  = permuted_6;
    companion19Column = *(s32 *)(Func_020058f2(19) + 8) >> 20;
    companion14Column = *(s32 *)(Func_020058fc(14) + 8) >> 20;
    companion16Column = *(s32 *)(Func_02005906(16) + 8) >> 20;

    if (row == 8) {
        if ((u32)(companion16Column - 9) <= 2) {
            return;
        }
        if ((u32)(companion14Column - 9) <= 2) {
            goto transition48;
        }
        if ((u32)(companion19Column - 9) > 2) {
            Func_0200401a(9, 0, 80);
        }
        /* Falls through into the id-96 site from both paths. */
        Func_02004024(9, 0, 96);
    } else if (row == 11) {
        if ((u32)(companion14Column - 9) <= 2) {
            return;
        }
        if ((u32)(companion19Column - 9) <= 2) {
transition48:
            Func_02004044(9, 0, 48);
        } else {
            Func_02004050(9, 0, 128);
        }
    } else if (row == 12) {
        if ((u32)(companion14Column - 9) <= 2) {
            return;
        }
        if ((u32)(companion19Column - 9) <= 2) {
            Func_02004070(9, 0, 32);
        } else {
            Func_0200407c(9, 0, 112);
        }
    } else if (row == 14) {
        if ((u32)(companion19Column - 9) <= 2) {
            return;
        }
        Func_02004094(9, 0, 80);
    } else if (row == 15) {
        Func_020040a4(9, 0, 64);
    } else if (row == 18) {
        Func_020040b4_a(9, 0, 16);
    }

    Task_Wait(2);

    column -= 1;
    Map_CopyCellAttributes(column, row, 3, 1,
                  column, *(s32 *)(Func_020059d4(9) + 16) >> 20);
    Map_CopyCellAttributes(0, 0, 3, 1, column, row);
}

void RetreatActor14AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companion18Row;
    s32 companion9Row;
    s32 rowM1;

    s32 permuted_7;
    column = *(s32 *)(Func_02005c10(14) + 8) >> 20;
    permuted_7 = *(s32 *)(Func_02005c1a(14) + 16) >> 20;
    companion18Row = *(s32 *)(Func_02005c24(18) + 16) >> 20;
    row  = permuted_7;
    companion9Row = *(s32 *)(Func_02005c2e(9) + 16) >> 20;

    if (column == 13) {
        if ((u32)(companion9Row - 12) <= 2) {
            Func_0200437e(14, -16, 0);
        } else if ((u32)(companion18Row - 12) <= 2) {
            Func_0200437e(14, -64, 0);
        } else {
            Func_0200437e(14, -112, 0);
        }
    } else if (column == 12) {
        if ((u32)(companion9Row - 12) <= 2) {
            return;
        }
        if ((u32)(companion18Row - 12) <= 2) {
            Func_0200437e(14, -48, 0);
        } else {
            Func_0200437e(14, -96, 0);
        }
    } else if (column == 9) {
        if ((u32)(companion18Row - 12) <= 2) {
            return;
        }
        Func_0200437e(14, -48, 0);
    } else if (column == 8) {
        if ((u32)(companion18Row - 12) <= 2) {
            return;
        }
        Func_0200437e(14, -32, 0);
    } else if (column == 6) {
        return;
    }

    Task_Wait(2);

    rowM1 = row - 1;
    Map_CopyCellAttributes(column, rowM1, 1, 3,
                  *(s32 *)(Func_02005ca4(14) + 8) >> 20, rowM1);
    Map_CopyCellAttributes(0, 0, 1, 3, column, rowM1);
}

void AdvanceActor14AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companion18Row;
    s32 companion9Row;
    s32 rowM1;

    s32 permuted_7;
    column = *(s32 *)(Func_02005cdc(14) + 8) >> 20;
    permuted_7 = *(s32 *)(Func_02005ce6(14) + 16) >> 20;
    companion18Row = *(s32 *)(Func_02005cf0(18) + 16) >> 20;
    row  = permuted_7;
    companion9Row = *(s32 *)(Func_02005cfa(9) + 16) >> 20;

    if (column == 6) {
        if ((u32)(companion9Row - 12) <= 2) {
            Func_0200443c(14, 32, 0);
        } else if ((u32)(companion18Row - 12) <= 2) {
            Func_0200443c(14, 64, 0);
        } else {
            Func_0200443c(14, 112, 0);
        }
    } else if (column == 8) {
        if ((u32)(companion9Row - 12) <= 2) {
            return;
        }
        Func_0200443c(14, 80, 0);
    } else if (column == 9) {
        if ((u32)(companion9Row - 12) <= 2) {
            return;
        }
        Func_0200443c(14, 64, 0);
    } else if (column == 12) {
        Func_0200443c(14, 16, 0);
    } else if (column == 13) {
        return;
    }

    Task_Wait(2);

    rowM1 = row - 1;
    Map_CopyCellAttributes(column, rowM1, 1, 3,
                  *(s32 *)(Func_02005d62(14) + 8) >> 20, rowM1);
    Map_CopyCellAttributes(0, 0, 1, 3, column, rowM1);
}

void RetreatActor16AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companion18Row;
    s32 companion9Row;
    s32 rowM1;

    s32 permuted_7;
    column = *(s32 *)(Func_02005d9c(16) + 8) >> 20;
    permuted_7 = *(s32 *)(Func_02005da6(16) + 16) >> 20;
    companion18Row = *(s32 *)(Func_02005db0(18) + 16) >> 20;
    row  = permuted_7;
    companion9Row = *(s32 *)(Func_02005dba(9) + 16) >> 20;

    if (column == 13) {
        if ((u32)(companion9Row - 9) <= 2) {
            Func_02004502(16, -16, 0);
        } else if ((u32)(companion18Row - 9) <= 2) {
            Func_02004502(16, -64, 0);
        } else {
            Func_02004502(16, -112, 0);
        }
    } else if (column == 12) {
        if ((u32)(companion9Row - 9) <= 2) {
            return;
        }
        if ((u32)(companion18Row - 9) <= 2) {
            Func_02004502(16, -48, 0);
        } else {
            Func_02004502(16, -96, 0);
        }
    } else if (column == 9) {
        if ((u32)(companion18Row - 9) <= 2) {
            return;
        }
        Func_02004502(16, -48, 0);
    } else if (column == 8) {
        Func_02004502(16, -32, 0);
    } else if (column == 6) {
        return;
    }

    Task_Wait(2);

    rowM1 = row - 1;
    Map_CopyCellAttributes(column, rowM1, 1, 3,
                  *(s32 *)(Func_02005e28(16) + 8) >> 20, rowM1);
    Map_CopyCellAttributes(0, 0, 1, 3, column, rowM1);
}

void AdvanceActor16AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companionRow;

    s32 permuted_6;
    column = *(s32 *)(Func_02005e60(16) + 8) >> 20;
    permuted_6 = *(s32 *)(Func_02005e6a(16) + 16) >> 20;
    companionRow = *(s32 *)(Func_02005e74(9) + 16) >> 20;
    row  = permuted_6;

    if (column == 6) {
        if ((u32)(companionRow - 9) <= 2) {
            Func_020045a2_a(16, 32, 0);
        } else {
            Func_020045a2_a(16, 112, 0);
        }
    } else if (column == 8) {
        if ((u32)(companionRow - 9) <= 2) {
            return;
        }
        Func_020045a2_a(16, 80, 0);
    } else if (column == 9) {
        Func_020045a2_a(16, 64, 0);
    } else if (column == 12) {
        Func_020045a2_a(16, 16, 0);
    } else if (column == 13) {
        return;
    }

    Task_Wait(2);

    row -= 1;
    Map_CopyCellAttributes(column, row, 1, 3,
                  *(s32 *)(Func_02005ec8(16) + 8) >> 20, row);
    Map_CopyCellAttributes(0, 0, 1, 3, column, row);
}

s32 UpdateSwayingSceneObject(struct S *object)
{

    struct T *sprite = object->f50;
    s32 vertical_offset = Math_Sin(object->f30) * 2;
    s32 random_b;
    s32 random_a;

    if (vertical_offset > 0) {
        vertical_offset = -vertical_offset;
    }
    object->f08 = object->f38 + Math_Cos(object->f30) * 2;
    object->f0c = object->f3c + vertical_offset;
    sprite->f1e = (u16)(Math_Cos(object->f30 + 0x8000) / 8);
    random_a = Random_Next();
    random_b = Random_Next();
    object->f30 += (((u32)(random_a << 9)) >> 16) + (((u32)(random_b << 9)) >> 16) + 0x400;
    return 0;
}

void InitializeSwayingSceneObject(void)
{
    OrbitingSceneObject *actor;
    OrbitingSceneObjectSprite *sprite;
    u8 *transfer;
    s32 zero;

    actor = GetOrbitingSceneObject();
    sprite = actor->sprite;
    sprite->flags_09_mode = 1;
    sprite->flags_05_bit_5 = 0;
    sprite->flags_09_high = 0;

    zero = 0;
    sprite->state = zero;
    Actor_SetSpriteFlags(actor, zero);
    actor->active = zero;
    actor->mode = zero;

    if (GameFlag_IsSet(0x109) == 0)
        actor->y += 0x200000;

    actor->flags_23 &= 0xfe;
    actor->visible = 1;

    transfer = AllocateEffectTransfer(17, 0x608);
    Item_LoadIcon(181);
    transfer += 0x400;
    Vram_Load(sprite->palette, 128, transfer);
    Heap_Release(17);

    actor->orbit_center_x = actor->x;
    actor->orbit_angle = zero;
    actor->orbit_center_y = actor->y;
    actor->active = 1;
    actor->callback = (u32)&UpdateOrbitingSceneObject;
    actor->state = zero;
}
