#include "TYPES.H"
#include "FIELD_EFFECT.H"

#define OverlayObject_CreateAndInitialize Func_020009f4
#define SceneEffect_SpawnNineRadialEffects Func_020011e0
#define OverlayObject_PrepareSpawnedObjectMode4 Func_02000a4c
#define OverlayObject_IntegrateVelocities Func_02000ab0
#define OverlayObject_SetYAboveLinkedActor Func_02001a98
#define SceneState_ApplyArgMode0AndReturnZero Func_02000cc0
#define SceneState_SwapSlotPairByRank Func_02000f10
#define SceneState_RunRect73x38Step Func_02001318
#define SceneState_ApplyTwoRectsAndRunThree Func_020013e0
#define SceneActor_MirrorFlag201IntoSlot14 Func_02001458
#define SceneState_ApplyFlag970 Func_02001500
#define SceneState_RunUnlessActorZeroAt30_52 Func_02001530
#define SceneState_SetValue202ThenCall Func_02001924
#define SceneActor_ConfigureSlot21AndSetFlag201 Func_02001938
#define SceneState_SetSlot17And18Selectors Func_02001f70
#define SceneState_SetValue268bInScene Func_02002330
#define SceneActor_PassRaisedPointOfActorZero Func_02000dc8
#define SceneActor_PassActorZeroOffsetPoint Func_02000df4
#define SceneActor_CopyActor8PositionWhenAtRow10 Func_02000fe8
#define SceneActor_ApplyPointLeftOfActorZero Func_02001348
#define SceneActor_PassPointTwoRightOfActorZero Func_020013b4
#define SceneActor_LandOnHighestPlatform Func_02001c2c
#define SceneActor_SetFlagBitByRelativeDepth Func_02000ec8
#define SceneData_SelectDataByRuntimeSelector Func_02001270
#define SceneData_SelectTableB81cByWord224 Func_020012b0
#define SceneData_GetTableB85c Func_020012d8
#define SceneData_SelectAndApplyTableBySceneId Func_020012e0
#define SceneData_SelectTableByWord224 Func_02002370
#define FieldScene_RunLayoutAt93By30 Func_02001374
#define FieldScene_RunLayoutAt83By45 Func_02002300
#define FieldScene_PlaceAndPinSlots10And11 Func_02002410
#define EffectRuntime_SetCurrentPosition_1(a0, a1, a2) Call3(Func_020051a2, a0, a1, a2)
#define EffectRuntime_SetCurrentPosition_2(a0, a1, a2) Call3(Func_020051ac, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_02005162, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(a0, a1, a2) Call3(Func_02005170, a0, a1, a2)
#define EffectRuntime_SetCurrentPosition_1_02002040(a0, a1, a2) Call3(Func_0200520a, a0, a1, a2)
#define EffectRuntime_SetCurrentPosition_2_02002040(a0, a1, a2) Call3(Func_02005218, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1_02002040(a0, a1, a2) Call3(Func_020051ca, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2_02002040(a0, a1, a2) Call3(Func_020051d4, a0, a1, a2)
#define FieldScene_RunStepWith6 Func_020013a4
#define FieldScene_RunSupplementalSequenceTwo Func_02001550
#define FieldScene_RunSupplementalSequenceOne Func_02001744
#define FieldScene_SetActor19TableB3B8 Func_02001910
#define FieldScene_RunMiddleSequence Func_02001d04
#define FieldScene_RunLateSequenceHead Func_02001fdc
#define FieldScene_RunLateSequenceSecond Func_02002040
#define FieldScene_RunScene3c4SequenceA Func_020020a4
#define FieldScene_RunScriptedStep953 Func_02002354
#define FieldScene_RunScene3c4_02002480 Func_02002480
#define SceneActor_CheckTwoUnitsAboveActorZero Func_02001424
#define SceneActor_SetActor14Field98ByFlag200 Func_020014ac
#define ActorPresentation_ConfigureActorTwentyAndFlag200 Func_0200170c
#define SceneState_RunUnlessActorZeroAtTile32x50 Func_02001510
#define FieldScene_RunFourCallSequenceB Func_020016f4
#define FieldScene_RunFourStepSequenceA Func_020018f8
#define FieldScene_RunTwoStepSequence Func_020019bc
#define FieldScene_RunFourStepSequenceB Func_020019cc
#define SceneActor_InstallSlotNineHandler Func_020019e4
#define SceneActor_SetupSlotNineAndInstallHandler Func_02001a10
#define FieldScene_RunThreeStepSequence Func_02001f5c
#define FieldScene_RunFourCallSequence Func_02001fc4
#define FieldScene_PlaceAndPinSlots8And9 Func_020023a0
#define SceneDialogue_RunFlag982Or983Dialogue Func_02001970
#define SceneActor_MoveActorZeroToTarget Func_02000cd0
#define SceneActor_WaitValueBelowLimit Func_02001038

#include "STAGED_ACTOR.H"
#include "SPAWN_CONFIGURED_EFFECT.H"


struct Owner {
    u8 unk0[9];
    u8 unk9_0 : 2;
    u8 mode : 2;
    u8 unk9_4 : 4;
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
    s32 unk00;
    s32 mode;
    u8 filler08[0x1c];
    s32 callback;
};

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
};

struct Slot02000f10 {
    u8 head[8];
    s32 x;
    s32 y;
    s32 rank;
};

struct Actor_02000cc0 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

typedef struct Slot_02001f70 {
    s32 pad0;
    s32 pad4;
    s32 w8;                /* +8, 12.20 fixed point */
} Slot_02001f70;

typedef struct Actor_02000cd0 {
    u8 pad0[6];
    u16 tag;               /* +6  */
    s32 x;                 /* +8,  12.20 fixed point */
    s32 y;                 /* +12 */
    s32 z;                 /* +16, 12.20 fixed point */
    u8 pad20[20];
    s32 speedZ;            /* +40 */
    s32 pad44;
    s32 speedX;            /* +48 */
    s32 speedY;            /* +52 */
    u8 pad56[29];
    u8 flags;              /* +85 */
} Actor_02000cd0;

typedef struct Target_02000cd0 {
    s32 x;                 /* +0, 12.20 fixed point */
    s32 pad4;
    s32 z;                 /* +8, 12.20 fixed point */
} Target_02000cd0;

struct Actor_02000dc8 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

typedef struct Record {
    s32 pad0;
    s32 pad4;
    s32 w8;
    s32 w12;
    s32 w16;
} Record;

/*
 * resource_3c4: land an actor on the highest platform sharing its tile.
 */
typedef struct Slot_02001c2c {
    u8 pad0[8];
    s32 x;                 /* +8,  12.20 fixed point */
    s32 y;                 /* +12 */
    s32 z;                 /* +16, 12.20 fixed point */
} Slot_02001c2c;

struct Actor_02000ec8 { u8 unknown_00[8]; s32 x; s32 y; s32 z; u8 unknown_14[0xf]; u8 flatla3; };

struct Track02001038 {
    u8 head[12];
    s32 value;
    u8 gap16[4];
    s32 limit;
    u8 gap24[16];
    s32 state;
    u8 gap44[16];
    s32 mark;
};

/*
 * Per-frame integrator for an effect record -- resource_3c4. It adds the
 * velocity at +68/+72/+76 into the position at +8/+12/+16, decays two of the
 * three velocities, accumulates the rates at +48/+52, and turns the sprite.
 */

/* Every field is named by offset; the layout is not verified. */
struct MotionEffect {
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

typedef struct Slot_02002410 {
    s32 pad0;
    s32 pad4;
    s32 column;            /* +8,  12.20 fixed point */
    s32 pad12;
    s32 row;               /* +16, 12.20 fixed point */
} Slot_02002410;

struct Actor02001424 {
    u8 head[8];
    s32 x;
    s32 y;
    s32 z;
};

struct Actor_02001510 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

typedef struct Slot_02001a10 {
    s32 pad0;
    s32 pad4;
    s32 col;            /* +8,  12.20 fixed point */
    s32 pad12;
    s32 row;               /* +16, 12.20 fixed point */
} Slot_02001a10;

typedef struct Slot_020023a0 {
    s32 pad0;
    s32 pad4;
    s32 column;            /* +8,  12.20 fixed point */
    s32 pad12;
    s32 row;               /* +16, 12.20 fixed point */
} Slot_020023a0;

extern s16 Data_02000240[];
extern u8 Value_000000ac;
extern u8 Value_000000ad;
extern u8 Data_0200b474[];
extern u8 Data_0200b654[];
extern u8 Data_0200b42c[];
extern u8 Data_0200b81c[];
extern u8 Data_0200b8f4[];
extern u8 Data_0200ba74[];
extern u8 Data_0200bc0c[];
extern u8 Data_0200bef4[];

u8 *Func_02003a6a(s32 kind, s32 x, s32 y, s32 z);
void Func_02003ad8(u8 *obj, s32 mode);
void Func_02003b98(u8 *obj, s32 mode);
void Func_02003af0(u8 *obj, s32 mode);
struct SceneObject *Func_020042fa(void);
s32 Func_02004254(s32);
s32 Func_02004258(s32);
s32 Func_0200423e(s32, s32);
void Func_02001d32(s32, s32, s32, s32, s32, s32, s32, struct EffectParams *);
u8 *Func_02003ac2(s32 kind, s32 x, s32 y, s32 z);
void Func_02003b34(u8 *obj, s32 mode);
void Func_02003bf4(u8 *obj, s32 mode);
struct Actor *Func_02004bb2(s32);
void Func_02003d72(s32, s32);
struct Slot02000f10 *Func_02004022(s32);
struct Slot02000f10 *Func_0200402a(s32);
void Func_02003f68(s32);
void Func_02004412(void);
void Func_020043ce(s32, s32, s32, s32, s32, s32);
void Func_020013fa(void);
void Func_020036da(void);
void Func_0200443a(void);
void Func_020044da(void);
void Func_02004496(s32, s32, s32, s32, s32, s32);
void Func_020044a8(s32, s32, s32, s32, s32, s32);
void Func_020014d4(void);
void Func_02003894(void);
void Func_02004514(void);
void Func_0200453c(s32);
s32 Func_0200453a(s32);
u8 *Func_0200457c(s32);
u8 *Func_02004588(s32);
u8 *Func_02004598(s32);
u8 *Func_020045a4(s32);
void Func_020045e4(s32);
struct Actor *Func_02004642(s32);
void Func_0200233e(void);
void Func_02004a06(s32);
void Func_02003072(void);
void Func_02004a84(s32, s32);
void Func_02004aa4(s32, s32);
void Func_02004a94(s32, s32);
u8 *Func_02004a62(s32);
void Func_02004a42(s32);
void Func_02005068(void);
Slot_02001f70 *Func_02005086(s32 slot);
Slot_02001f70 *Func_020050a2(s32 slot);
void Func_02005064(s32 selector);
void Func_02005074(s32 selector);
void Func_02005080(s32 selector);
void Func_02005090(s32 selector);
void Func_020044dc(void);
void Func_020050b0(void);
void Func_02005428(void);
void Func_02005480(s32, s32);
void Func_02005410(s32, s32);
void Func_02005444(void);
Actor_02000cd0 *Func_02003dec_a();
s32 Func_02003d6a();
s32 Func_02003dca_a();
s32 Func_02003dca_b();
s32 Func_02003dd6();
void Func_02003e2e();
void Func_02003d96();
void Func_02003d6c();
void Func_02003efa();
void Func_02003daa();
void Func_02003e24();
void Func_02003eb4();
void Func_02003dec_b();
void Func_02003e4c();
void Func_02003eaa();
struct Actor *Func_02003edc(s32);
void Func_02001aba(s32 *);
struct Actor *Func_02003f08(s32);
void Func_02001ae4(s32 *);
Record *Func_020040fc(s32);
Record *Func_0200410a(s32);
Record *Func_02004118(s32);
Record *Func_02004126(s32);
struct Actor *Func_0200445c(s32);
void Func_02002038(s32 *);
struct Actor *Func_020044c8(s32);
void Func_020020a6(s32 *);
Slot_02001c2c *Func_02004d56();
Slot_02001c2c *Func_02004d5e();
Slot_02001c2c *Func_02004d6e();
Slot_02001c2c *Func_02004d76();
Slot_02001c2c *Func_02004d86();
Slot_02001c2c *Func_02004d98();
Slot_02001c2c *Func_02004da8();
void Func_02004dcc();
Slot_02001c2c *Func_02004dca();
Slot_02001c2c *Func_02004dd2();
Slot_02001c2c *Func_02004dda();
void Func_02004d4e();
void Func_02004e14();
void Func_02004e92();
void Func_02002ecc();
void Func_02004dde();
struct Actor *Func_02003fdc();
void Func_0200406a(s32);
s32 Func_0200409e();
void Func_02004402(u8 *);
void Func_0200446e(void);
void Func_0200442a(s32, s32, s32, s32, s32, s32);
void Func_02001456(void);
void Func_020037a6(void);
void Func_02004496_a(void);
void Func_020053fa(void);
void Func_020053b6(s32, s32, s32, s32, s32, s32);
void Func_020023e2(void);
void Func_020043c6(void);
void Func_02005422(void);
void Func_020054c2();
void Func_0200333e();
Slot_02002410 *Func_02005540();
Slot_02002410 *Func_02005548();
void Func_020054ee();
Slot_02002410 *Func_02005564();
Slot_02002410 *Func_0200556c();
void Func_02005512();
void Func_02004502(s32, s32, s32);
s32 Func_02001f74();
void Func_020020de();
void Func_020045cc();
s32 Func_020045ee();
s32 Func_02004600();
void Func_0200464e();
void Func_0200465c();
s32 Func_0200466c();
void Func_0200469e();
struct FieldActor *Func_020046a0();
void Func_020046aa();
struct FieldActor *Func_020046ba();
void Func_020046e0();
void Func_020046f8();
void Func_02004700();
struct FieldActor *Func_02004726();
void Func_02004730();
void Func_02004736();
void Func_02004744();
void Func_0200474c();
void Func_0200476e();
void Func_02004770();
void Func_0200477a();
void Func_0200477c();
void Func_02004788();
void Func_0200478e();
void Func_020047ac();
void Func_020047b6();
void Func_020047d0();
void Func_02004804();
void Func_02004840();
s32 Func_02002172();
void Func_020022dc();
void Func_020047ca();
s32 Func_020047ec();
s32 Func_020047fe();
s32 Func_02004838();
void Func_02004842();
void Func_0200485c();
s32 Func_02004860();
void Func_0200489c();
struct FieldActor *Func_0200489e();
void Func_020048a8();
struct FieldActor *Func_020048b8();
void Func_020048de();
void Func_020048f6();
void Func_020048fe();
struct FieldActor *Func_02004924();
void Func_0200492e();
void Func_02004934();
void Func_02004942();
void Func_0200494a();
void Func_0200496c();
void Func_0200496e();
void Func_02004978();
void Func_0200497a();
void Func_02004986();
void Func_0200498c();
void Func_020049aa();
void Func_020049b4();
void Func_020049ce();
void Func_02004a02();
void Func_02004a3e();
s32 Func_02004a34(s32, s32);
void Func_02002d82();
void Func_020039d4();
void Func_02003aee();
void Func_02004e0a();
s32 Func_02004e10();
s32 Func_02004e30();
void Func_02004e36();
void Func_02004e52();
s32 Func_02004e6e();
s32 Func_02004e7a();
s32 Func_02004e86();
void Func_02004e90();
s32 Func_02004e92_a();
s32 Func_02004e9e();
s32 Func_02004ea8();
s32 Func_02004eb0();
s32 Func_02004eba();
s32 Func_02004ec2();
s32 Func_02004ecc();
s32 Func_02004ed0();
s32 Func_02004ed8();
s32 Func_02004ee4();
s32 Func_02004ef4();
s32 Func_02004f16();
s32 Func_02004f1c();
s32 Func_02004f1e();
s32 Func_02004f26();
s32 Func_02004f32();
s32 Func_02004f3c();
s32 Func_02004f6a();
s32 Func_02004f74();
s32 Func_02004f7e();
s32 Func_02004f8a();
s32 Func_02004f96();
s32 Func_02004fd4();
s32 Func_02004fe2();
s32 Func_02004ff0();
s32 Func_02004ffe();
s32 Func_0200500c();
void Func_0200500e();
s32 Func_02005010();
s32 Func_0200501a();
s32 Func_0200502a();
s32 Func_0200502c();
void Func_02005034();
void Func_0200503e();
void Func_02005040();
void Func_0200508e();
void Func_020050a0();
void Func_02005162();
void Func_02005170();
void Func_020051a2();
void Func_020051ac();
void Func_020050f2();
void Func_02005104();
void Func_020051ca();
void Func_020051d4();
void Func_0200520a();
void Func_02005218();
void Func_0200515a();
void Func_0200517e();
void Func_0200519e();
void Func_020051a2_a();
void Func_020051c6();
s32 Func_020051d0();
s32 Func_020051d8();
void Func_020051ea();
s32 Func_020051f4();
s32 Func_020051fc();
void Func_0200520e();
s32 Func_02005218_a();
s32 Func_02005220();
s32 Func_0200523c();
s32 Func_02005244();
s32 Func_02005260();
s32 Func_02005268();
s32 Func_02005284();
s32 Func_02005294();
s32 Func_020052a4();
s32 Func_020052b2();
s32 Func_020052c2();
s32 Func_020052d0();
s32 Func_020052e0();
void Func_020052e2();
s32 Func_020052e8();
s32 Func_020052ee();
s32 Func_020052fe();
s32 Func_0200530c();
void Func_0200530e();
void Func_02005310();
void Func_02005318();
void Func_0200532a();
void Func_02005334();
void Func_02005348();
s32 Func_02005354();
void Func_02005364();
void Func_02005374();
void Func_02005376();
void Func_02005398();
void Func_0200539a();
void Func_020053a0();
void Func_020053ba();
void Func_020053c6();
void Func_020053ca();
void Func_020053de();
void Func_02005400();
void Func_02005406();
void Func_02005420();
void Func_0200542c();
void Func_02005466();
void Func_0200544c(void);
void Func_0200542c_a(s32, s32);
void Func_02005460(void);
void Func_02005532();
void Func_02005544();
void Func_02005574();
void Func_02005598();
s32 Func_020055ba();
void Func_020055bc();
s32 Func_020055c6();
s32 Func_020055ce();
s32 Func_020055ea();
s32 Func_020055f2();
s32 Func_0200560e();
s32 Func_02005616();
struct Actor02001424 *Func_02004538(s32);
s32 Func_02002116(s32 *);
void Func_0200282e(void);
s32 Func_0200458e_a(s32);
s32 Func_0200458e_b(s32);
u8 *Func_020045d0(s32);
u8 *Func_020045dc(s32);
u8 *Func_020045ec(s32);
u8 *Func_020045f8(s32);
void Func_02004858(s32, s32);
void Func_02004878(s32, s32);
void Func_02004868(s32, s32);
u8 *Func_02004836(s32);
void Func_02004818(s32);
struct Actor *Func_02004622(s32);
void Func_02002872(void);
void Func_020047ec_a(void);
void Func_020017c0(void);
void Func_020047fc(void);
void Func_02002c54(void);
void Func_020049f0(void);
void Func_020019c4(void);
void Func_02004a00(void);
void Func_0200304c(void);
void Func_02004ab4(void);
void Func_02004ac0(void);
void Func_02004ac4(void);
void Func_02004ad4(void);
void Func_02003398(void);
s32 Func_02004b08(s32, s32);
void Func_02004ace(s32);
u8 *Func_02004b04(s32);
void Func_02004b0a();
void Func_02004b8a_a();
void Func_02004b8a_b();
void Func_02004b6a();
void Func_02004b7a();
u8 *Func_02004b48();
void Func_02004b2a();
Slot_02001a10 *Func_02004b60();
Slot_02001a10 *Func_02004b68();
void Func_02004b0e();
u8 *Func_02004b84_a();
u8 *Func_02004b84_b();
u8 *Func_02004b8e();
struct Record_02000ec8;
s32 Func_02008ec8(struct Record_02000ec8 *record);
void Func_02005054(void);
void Func_02002028(void);
void Func_02005064_a(void);
void Func_020050bc(void);
void Func_02002090(void);
void Func_02003f40(void);
void Func_020050d0(void);
void Func_02005452();
void Func_020032ce();
Slot_020023a0 *Func_020054d0();
Slot_020023a0 *Func_020054d8();
void Func_0200547e();
Slot_020023a0 *Func_020054f4();
Slot_020023a0 *Func_020054fc();
void Func_020054a2();
void Func_02004a68(void);
void Func_02004ac0_a(s32, s32);
s32 Func_02004a56(s32);
s32 Func_02004a60(s32);
void Func_02004a64(s32, s32);
void Func_02004a6e(s32, s32);
void Func_02004aa2(void);

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

/* Copy the low two mode bits into the object's owner record. */

/*
 * Actor movement for overlay resource_3c4.  Declarations are left without
 * prototypes because Func_02003dec and Func_02003dca are each called twice
 * with different argument shapes.
 */

/*
 * Slot nine setup for overlay resource_3c4.
 */

/*
 * Every call site branches through its own veneer, so the callees are named
 * per site. Func_02004b84 serves two sites with different argument shapes
 * and is declared without a prototype.
 */

/*
 * Setup step in resource_3c4. One six-argument placement, then two identical
 * follow-ups that pin an overlay at the 12.20 grid cell of slots 8 and 9: the
 * slot's +8 and +16 words are each shifted right by 20 and passed as the two
 * stack arguments of the placement call.
 *
 * Several call sites reach the same routine, but each keeps its own call
 * word; the sites must not be collapsed onto one alias.
 */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value0(s32 (*f)())
{
    extern u8 Data_03001ebc[];

    return f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02001d04(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_020020a4(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    f(a0, a1);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02002480(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

struct Record_02000ec8;


void Func_020009dc(u8 *object, s32 mode)
{
    struct Owner *owner = *(struct Owner **)(object + 80);
    owner->mode = mode;
}

/*
 * Create an object, clear the low bits of its owner record, and initialise it.
 * The four callees are declared at their in-overlay entry points, not at
 * veneers.  The +9 flag clear takes its mask from a local that is initialised
 * first and only afterwards ANDed with the loaded byte; written as one
 * expression it swaps which register holds the mask and which the byte.
 */
u8 *OverlayObject_CreateAndInitialize(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *ret;
    u8 *obj = Func_02003a6a(kind, x, y, z);

    if (obj != 0) {
        u8 *owner = *(u8 **)(obj + 80);
        u32 mask = ~12;
        mask = mask & owner[9];
        owner[9] = (u8)mask;
        obj[85] = 0;
        obj[89] = 8;
        Func_02003ad8(obj, 0);
        Func_02003b98(obj, 14);
        Func_02003af0(obj, 1);
        ret = obj;
    } else {
        ret = 0;
    }
    return ret;
}

/*
 * Spawn an object and prepare its owner record and flags.  The three callees
 * are declared at their in-overlay entry points, not at veneers.  Each
 * read-modify-write takes its mask from a local that is initialised first and
 * ANDed with the loaded byte afterwards, which is what puts the mask in r3 and
 * the byte in r2.  The +0x23 mask stays byte-wide as 0xfe rather than ~1,
 * which would widen it to a negated 2.
 */
u8 *OverlayObject_PrepareSpawnedObjectMode4(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *ret;
    u8 *obj = Func_02003ac2(kind, x, y, z);

    if (obj != 0) {
        u8 *rec = *(u8 **)(obj + 80);
        u32 mask = ~12;
        u32 lo;
        mask = mask & rec[9];
        rec[9] = (u8)(mask | 4);
        obj[85] = 0;
        obj[89] = 8;
        Func_02003b34(obj, 0);
        Func_02003bf4(obj, 15);
        lo = 0xfe;
        lo = lo & obj[35];
        obj[35] = (u8)(lo | 2);
        ret = obj;
    } else {
        ret = 0;
    }
    return ret;
}

void OverlayObject_IntegrateVelocities(union FieldObject *object)
{
    object->effect.x += object->effect.velocity_x;
    object->effect.y += object->effect.velocity_y;
    object->effect.z += object->effect.velocity_z;
    object->effect.scale_x += object->effect.scale_rate_x;
    object->effect.scale_y += object->effect.scale_rate_y;
    object->effect.sprite->rotation += object->effect.spin;
}

/*
 * resource_3c4 @ 0x02001f70 (84 bytes: 72 code + alignment + two pool words).
 *
 * Publishes selector 0x974 for slot 17 and 0x975 for slot 18, choosing a
 * different publisher for each depending on whether that slot's +8 word sits
 * at 12.20 row 45 and 46 respectively.  `asrs #20` makes both tests signed.
 * Both pool words are selectors, not addresses.
 *
 * `pop {r0} ; bx r0` return: void.
 */
s32 SceneState_ApplyArgMode0AndReturnZero(s32 no)
{
    Func_02003d72(no, 0);
    return 0;
}

/*
 * Tries to move actor 0 onto the caller's target.  It builds a three-word
 * 12.20 probe from the record's own position -- the horizontal words snapped
 * to their whole-unit grid and lifted by half a unit -- asks the collision
 * service about it, and refuses when either the probe or the target is
 * rejected.  Returns 1 when refused and 0 when the move ran.  The 248-byte
 * owner includes its one pool word.
 */
s32 SceneActor_MoveActorZeroToTarget(const Target_02000cd0 *target)
{
    Actor_02000cd0 *actor = Func_02003dec_a(0);
    u8 saved = actor->flags;
    s32 probe[3];

    probe[0] = (actor->x & (s32)0xfff00000) + 0x00080000;
    probe[1] = actor->y;
    probe[2] = (actor->z & (s32)0xfff00000) + 0x00080000;

    Func_02003d6a(0x00100000, (actor->tag + 0x2000) & 0xc000, probe);

    /* Both guards branch to one shared exit placed after the body.  Writing
     * `return 1` twice would put an inline copy near the top instead. */
    if (Func_02003dca_a(actor, probe) == 1) {
        goto refuse;
    }
    if (Func_02003dd6(actor, target) != 0) {
        goto refuse;
    }

    Func_02003e2e();
    Func_02003d96(actor, 6);
    Func_02003d6c(6);
    Func_02003efa(152);
    Func_02003daa(actor, 7);

    actor->speedX = 0x00030000;
    actor->speedY = 0x00020000;
    actor->speedZ = 0x00040000;
    actor->flags &= (u8)0x7e;   /* masks the byte re-read here, not `saved` */

    Func_02003e24(actor, 0);
    Func_02003eb4(0, ((target->x >> 20) << 4) + 8, ((target->z >> 20) << 4) + 8);
    Func_02003dec_b(actor, 6);
    Func_02003e4c(actor, 1);
    Func_02003dca_b(6);

    actor->flags = saved;
    Func_02003eaa();
    return 0;

refuse:
    return 1;
}

/*
 * Each site names the call word it holds, so the same slot accessor appears
 * here under several symbols. The declarations are old-style because the call
 * sites vary in arity.
 */
void SceneActor_PassRaisedPointOfActorZero(void)
{
    s32 pos[3];
    struct Actor_02000dc8 *p = Func_02003edc(0);

    pos[0] = p->f08;
    pos[1] = p->f0c;
    pos[2] = p->f10 + 0x200000;
    Func_02001aba(pos);
}

void SceneActor_PassActorZeroOffsetPoint(void)
{
    s32 pos[3];
    struct Actor_02000dc8 *actor = Func_02003f08(0);

    pos[0] = actor->f08;
    pos[1] = actor->f0c;
    pos[2] = actor->f10 + 0xFFE00000;
    Func_02001ae4(pos);
}

s32 SceneActor_SetFlagBitByRelativeDepth(struct Actor_02000ec8 *actor)
{
    struct Actor_02000ec8 *ref;
    u8 *fp;
    u8 flag;
    ref = Func_02003fdc(0);
    fp = &actor->flatla3;
    flag = *fp | 2;
    *fp = flag;
    if (ref->z < actor->z) {
        s32 lim = actor->z - ref->z;
        s32 ay;
        lim += 0x00040000;
        ay = actor->y;
        ay += lim;
        if (ref->y <= ay) {
            flag &= 0xfd;
            *fp = flag;
        }
    }
    return 0;
}

void SceneState_SwapSlotPairByRank(s32 first, s32 second)
{
    struct Slot02000f10 *a = Func_02004022(first);
    struct Slot02000f10 *b = Func_0200402a(second);

    if (a->rank <= b->rank) {
        s32 t;

        t = a->x;    a->x    = b->x;    b->x    = t;
        t = a->y;    a->y    = b->y;    b->y    = t;
        t = a->rank; a->rank = b->rank; b->rank = t;
        Func_02003f68(1);
    }
}

s32 SceneActor_CopyActor8PositionWhenAtRow10(Record *record)
{
    Record *ref = Func_020040fc(0);

    if (ref->w12 > (s32)0xffd00000
        && (Func_0200410a(8)->w16 >> 20) == 10) {
        record->w8 = Func_02004118(8)->w8;
        record->w12 = (s32)0xffe00000;
        record->w16 = Func_02004126(8)->w16;
    } else {
        record->w8 = 0;
        record->w12 = 0;
        record->w16 = 0;
    }
    return 0;
}

void SceneActor_WaitValueBelowLimit(struct Track02001038 *track)
{
    s32 cnt = 60;
    s32 limit;

    for (;;) {
        if (cnt != 0) {
            s32 value;

            Func_0200406a(1);
            value = track->value;
            limit = track->limit;
            cnt--;
            if (value <= limit) {
                break;
            }
            continue;
        }
        limit = track->limit;
        break;
    }
    track->state = 0;
    track->value = limit;
    track->mark = (s32) 0x80000000;
}

/*
 * A loader-relocated call word rather than a runtime address; it turns a value
 * and a duration into a per-frame step. Declared without a prototype, and the
 * call site passes two arguments.
 */

/*
 * The decay of the Z velocity stays a signed divide by sixteen: that shape is
 * what reproduces the negative bias and arithmetic shift in the reference.
 */
void Func_02001068(struct MotionEffect *effect)
{
    s32 velocity_z;
    struct Sprite *sprite;
    s32 velocity_x;

    /* This block orders the Z load after the Y store; do not flatten it. */
    do {
        velocity_x = effect->velocity[0];
        effect->position[0] += velocity_x;
        effect->position[1] += effect->velocity[1];
    } while (0);
    velocity_z = effect->velocity[2];
    effect->position[2] += velocity_z;

    effect->velocity[0] = velocity_x - Func_0200409e(velocity_x, 18);
    effect->velocity[2] = velocity_z - velocity_z / 16;

    effect->accum18 += effect->rate30;
    effect->accum1c += effect->rate34;

    sprite = effect->sprite;
    sprite->angle += effect->step64;
}

void SceneEffect_SpawnNineRadialEffects(void)
{
    struct SceneObject *object;
    struct Vec vec;
    struct EffectParams params;
    u32 i;
    s32 v;
    s32 x;
    s32 z;

    object = Func_020042fa();
    params.unk00 = 1;
    params.mode = 7;
    params.callback = 0x02009069;
    for (i = 0; i <= 16; i += 2) {
        v = i << 12;
        vec.x = Func_02004254(v);
        vec.y = 0;
        z = Func_02004258(v);
        x = vec.x;
        vec.z = z;
        x = x + Func_0200423e(x, 3);
        vec.x = x;
        Func_02001d32(object->x, object->y, object->z, x, vec.y, z, 0x01030001, &params);
    }
}

/* Return this overlay's state block. */
s32 SceneData_SelectDataByRuntimeSelector(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_000000ac) {
        return (s32)Data_0200b474;
    }
    if (v == (s32)&Value_000000ad) {
        return (s32)Data_0200b654;
    }
    return (s32)Data_0200b42c;
}

s32 SceneData_SelectTableB81cByWord224(void)
{
    if (Data_02000240[224] == (s32)&Value_000000ad) {
        return (s32)Data_0200b81c;
    }
    return 0;
}

u8 *SceneData_GetTableB85c(void) { return (u8 *)0x0200b85c; }

u8 *SceneData_SelectAndApplyTableBySceneId(void)
{
    u8 *tbl;

    if (Data_02000240[224] == (s32)&Value_000000ac) {
        tbl = Data_0200b8f4;
    } else {
        tbl = Data_0200ba74;
    }
    Func_02004402(tbl);
    return tbl;
}

void SceneState_RunRect73x38Step(void)
{
    Func_02004412();
    {
        s32 width = 9;
        s32 height = 38;

        Func_020043ce(73, 38, 5, 5, width, height);
    }
    Func_020013fa();
    Func_020036da();
    Func_0200443a();
}

void SceneActor_ApplyPointLeftOfActorZero(void)
{
    s32 point[3];
    struct Actor_02000dc8 *actor = Func_0200445c(0);

    point[0] = actor->f08 + 0xFFE00000;
    point[1] = actor->f0c;
    point[2] = actor->f10;
    Func_02002038(point);
}

/*
 * Layout step in resource_3c4 for slots 10 and 11. One six-argument
 * placement, then two grid-cell pins built from each slot's +8 and +16 words
 * shifted right by 20.
 *
 * Several call sites reach the same routine, but each keeps its own call
 * word; the sites must not be collapsed onto one alias.
 */
void FieldScene_RunLayoutAt93By30(void)
{
    Func_0200446e();
    {
        s32 width = 29;
        s32 height = 30;

        Func_0200442a(93, 30, 6, 5, width, height);
    }
    Func_02001456();
    Func_020037a6();
    Func_02004496_a();
}

void FieldScene_RunStepWith6(void)
{
    extern u8 Data_03001ebc[];

    Func_02004502(0, 6, 0);
}

void SceneActor_PassPointTwoRightOfActorZero(void)
{
    s32 pos[3];
    struct Actor_02000dc8 *actor = Func_020044c8(0);

    pos[0] = actor->f08 + 0x200000;
    pos[1] = actor->f0c;
    pos[2] = actor->f10;
    Func_020020a6(pos);
}

void SceneState_ApplyTwoRectsAndRunThree(void)
{
    s32 lead = 25;

    Func_020044da();
    Func_02004496(89, 49, 3, 2, lead, 49);
    Func_020044a8(89, 51, 8, 5, lead, 51);
    Func_020014d4();
    Func_02003894();
    Func_02004514();
}

void SceneActor_CheckTwoUnitsAboveActorZero(void)
{
    struct Actor02001424 *actor = Func_02004538(0);
    s32 target[3];

    target[0] = actor->x;
    target[1] = actor->y;
    target[2] = actor->z + 0x00200000;
    if (Func_02002116(target)!= 0) {
        Func_0200282e();
    }
}

/*
 * resource_3c4 @ 0x02001458 (84 bytes: 80 code and one pool word).
 *
 * This is the selector-reversed sibling immediately before 0x020014ac and is
 * written in that owner's proven shape.  It initializes query 0x200, tests
 * flag 0x201, then mirrors the queried state into slot 14's byte at +98 and
 * bit 3 of the byte at +89.  The zero halfword at 0x02001456 is alignment
 * after the preceding owner, not part of this one.
 *
 * Every call site bls its own veneer, so the callees are named at the decoded
 * per-site addresses.
 */
void SceneActor_MirrorFlag201IntoSlot14(void)
{
    u8 *flags;
    u8 value;

    Func_0200453c(0x200);
    if (Func_0200453a(0x201) != 0) {
        Func_0200457c(14)[98] = 0;
        Func_02004588(14)[89] &= (u8)0xf7;
    } else {
        Func_02004598(14)[98] = 1;
        flags = Func_020045a4(14);
        flags += 89;
        value = 8;
        value |= *flags;
        *flags = value;
    }
}

void SceneActor_SetActor14Field98ByFlag200(void)
{
    u8 *p;
    u8 val;

    Func_0200458e_a(0x201);
    if (Func_0200458e_b(0x200) != 0) {
        Func_020045d0(14)[98] = 0;
        Func_020045dc(14)[89] &= (u8)0xf7;
    } else {
        Func_020045ec(14)[98] = 1;
        p = Func_020045f8(14);
        p += 89;
        val = 8;
        val |= *p;
        *p = val;
    }
}

void SceneState_ApplyFlag970(void)
{
    Func_020045e4(0x970);
}

void SceneState_RunUnlessActorZeroAtTile32x50(void)
{
    struct Actor_02001510 *actor = Func_02004622(0);

    if ((actor->f08 >> 20) != 32 || (actor->f10 >> 20) != 50) {
        Func_02002872();
    }
}

void SceneState_RunUnlessActorZeroAt30_52(void)
{
    struct Actor_02000cc0 *actor = Func_02004642(0);

    if ((actor->f08 >> 20) != 30 || (actor->f10 >> 20) != 52) {
        Func_0200233e();
    }
}

void FieldScene_RunSupplementalSequenceTwo(void)
{
    u32 i;
    s32 rec2;
    s32 rec7;
    struct FieldActor *actor;
    s32 value;
    s32 v5;
    s32 v6;
    u8 slot16[40];
    u8 *frame;

    Func_0200464e();
    actor = (struct FieldActor *)Value1(Func_0200466c, 18);
    if ((actor->x.fixed >> 20) == 46) {
        Func_0200465c(30);
        rec2 = Func_02001f74(0x2e80000, 0, 0xb80000, 253);
        frame = slot16;
        *(s32 *)(frame + 8) = 0x9999;
        *(s32 *)(frame + 12) = 0x9999;
        *(s32 *)(frame + 4) = 7;
        Func_020046a0(18)->motion_flags = 0;
        Func_0200474c(185);
        for (i = 0; i < 16; i++) {
            Func_020045cc(3);
            Func_020046ba(18)->y.fixed -= 0x10000;
            rec7 = Value0(Func_020045ee);
            rec7 = ((((u32)(rec7 << 4) >> 16) << 16) + 0x2e00000);
            value = Value0(Func_02004600);
            Func_020020de(rec7, 0, ((((u32)(((value << 3) + value) << 1) >> 16) << 16) + 0x800000), 0, 0, 0, 0x90000, frame);
        }
        Call6(Func_020046aa, 51, 8, 1, 1, 49, 8);
        Func_02004700(30);
        Func_02004726(18)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
        Func_02004770(18, 3);
        Func_0200469e(rec2);
        Call6(Func_020046e0, 45, 4, 1, 1, 46, 8);
        Call3(Func_0200478e, 20, 0x2e80000, 0x880000);
        v5 = 1;
        v6 = 3;
        Func_02004804(188);
        Call6(Func_020046f8, 58, 8, 49, 8, v5, v6);
        Call3(Func_02004736, 0, 0x50000, 0x10000);
        Call3(Func_02004744, -1, -1, 0xe666);
        Func_0200477a(20);
        Func_02004840(188);
        Call6(Func_02004730, 59, 8, 49, 8, v5, v6);
        Call3(Func_0200476e, 0, 0x50000, 0x10000);
        Call3(Func_0200477c, -1, -1, 0xe666);
        Func_02004788();
        Func_020047b6(10);
        Call1(Func_020047ac, 0x971);
    }
    Func_020047d0();
}

void FieldScene_RunFourCallSequenceB(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    Func_020047ec_a();
    Func_020017c0();
    Func_020047fc();
    Func_02002c54();
}

void ActorPresentation_ConfigureActorTwentyAndFlag200(void)
{
    u8 *flags;

    Func_02004858(20, 1);
    Func_02004878(20, 0);
    Func_02004868(20, 2);
    flags = Func_02004836(20) + 35;
    *flags &= 0xFD;
    Func_02004818(0x200);
}

void FieldScene_RunSupplementalSequenceOne(s32 a0)
{
    u32 i;
    s32 rec2;
    s32 rec7;
    struct FieldActor *actor;
    s32 value;
    s32 arg0;
    s32 arg2;
    s32 v5;
    s32 v6;
    u8 slot16[40];
    u8 *slot;

    Func_02004842(a0);
    actor = (struct FieldActor *)Value1(Func_02004860, 19);
    if ((actor->x.fixed >> 20) == 48) {
        if (Value1(Func_02004838, 0x202) != 0) {
            Func_0200485c(30);
            rec2 = Func_02002172(0x3020000, 0, 0x1120000, 223);
            slot = slot16;
            *(s32 *)(slot + 8) = 0x9999;
            *(s32 *)(slot + 12) = 0x9999;
            *(s32 *)(slot + 4) = 7;
            Func_0200489e(19)->motion_flags = 0;
            Func_0200494a(185);
            for (i = 0; i < 16; i++) {
                Func_020047ca(3);
                Func_020048b8(19)->y.fixed -= 0x10000;
                rec7 = Value0(Func_020047ec);
                arg0 = ((((u32)(rec7 << 4) >> 16) << 16) + 0x3000000);
                value = Value0(Func_020047fe);
                arg2 = ((((u32)(((value << 3) + value) << 1) >> 16) << 16) + 0xe00000);
                Func_020022dc(arg0, 0, arg2, 0, 0, 0, 0x90000, slot);
            }
            Call6(Func_020048a8, 51, 8, 1, 1, 45, 14);
            Func_020048fe(30);
            Func_02004924(19)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
            Func_0200496e(19, 3);
            Func_0200489c(rec2);
            Call6(Func_020048de, 45, 4, 1, 1, 48, 14);
            Call3(Func_0200498c, 21, 0x3080000, 0xe80000);
            v5 = 1;
            v6 = 3;
            Func_02004a02(188);
            Call6(Func_020048f6, 58, 8, 45, 14, v5, v6);
            Call3(Func_02004934, 0, 0x50000, 0x10000);
            Call3(Func_02004942, -1, -1, 0xe666);
            Func_02004978(20);
            Func_02004a3e(188);
            Call6(Func_0200492e, 59, 8, 45, 14, v5, v6);
            Call3(Func_0200496c, 0, 0x50000, 0x10000);
            Call3(Func_0200497a, -1, -1, 0xe666);
            Func_02004986();
            Func_020049b4(10);
            Call1(Func_020049aa, 0x972);
        }
    }
    Func_020049ce();
}

void FieldScene_RunFourStepSequenceA(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    Func_020049f0();
    Func_020019c4();
    Func_02004a00();
    Func_0200304c();
}

void FieldScene_SetActor19TableB3B8(void)
{
    extern u8 Data_03001ebc[];

    Func_02004a34(19, 0x0200B3B8);
}

void SceneState_SetValue202ThenCall(void)
{
    Func_02004a06(0x202);
    Func_02003072();
}

void SceneActor_ConfigureSlot21AndSetFlag201(void)
{
    u8 *flags;

    Func_02004a84(21, 1);
    Func_02004aa4(21, 0);
    Func_02004a94(21, 2);
    flags = Func_02004a62(21) + 35;
    *flags &= 0xFD;
    Func_02004a42(0x201);
}

void SceneDialogue_RunFlag982Or983Dialogue(void)
{
    Func_02004a68();
    Func_02004ac0_a(0, 1);
    if (Func_02004a56(0x982) != 0 || Func_02004a60(0x983) != 0) {
        Func_02004a64(0x268A, 1);
    } else {
        Func_02004a6e(0x2689, 1);
    }
    Func_02004aa2();
}

void FieldScene_RunTwoStepSequence(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    Func_02004ab4();
    Func_02004ac0();
}

void FieldScene_RunFourStepSequenceB(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    Func_02004ac4();
    StagedActor_AdvancePair();
    Func_02004ad4();
    Func_02003398();
}

void SceneActor_InstallSlotNineHandler(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    u8 *owner;

    Func_02004b08(8, 0x0200B3B8);
    Func_02004ace(0x203);
    owner = Func_02004b04(9);
    *(s32 *)(owner + 108) = 0x02008FE9;
}

/*
 * Brings slot 9 up: four state writes, clear bit 1 of the byte at +35,
 * publish selector 0x204, pin an overlay at slot 9's 12.20 grid cell, then
 * install one handler on slots 9 and 8. The 136-byte owner at 0x02001a10
 * includes its alignment halfword and its one pool word; that word is an
 * odd Thumb pointer, so the handler is Func_02008ec8. The bit-clear folds
 * +35 into the returned pointer through the address local, not into a copy.
 */
void SceneActor_SetupSlotNineAndInstallHandler(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    u8 *desc;
    s32 col;
    s32 row;

    Func_02004b0a();
    Func_02004b8a_a(9, 1);
    Func_02004b6a(9, 1);
    Func_02004b8a_b(9, 0);
    Func_02004b7a(9, 2);

    {
        u8 *flag = Func_02004b48(9) + 35;
        *flag &= (u8)0xfd;
    }

    Func_02004b2a(0x204);

    col = Func_02004b60(9)->col;
    row = Func_02004b68(9)->row >> 20;
    Func_02004b0e(26, 8, 1, 1, col >> 20, row);

    desc = Func_02004b84_a(9);
    *(Handler_02001a10 *)(desc + 108) = Func_02008ec8;

    desc = Func_02004b8e(8);
    *(Handler_02001a10 *)(desc + 108) = Func_02008ec8;

    Func_02004b84_b(desc);
}

s32 OverlayObject_SetYAboveLinkedActor(u8 *owner)
{
    s16 *id = (s16 *)(owner + 100);
    struct Actor *actor = Func_02004bb2(*id);

    *(s32 *)(owner + 12) = actor->f0c + 0x100000;
    return 0;
}

/*
 * Scan slots 10 to 14, skipping the subject, keep those whose whole-tile x
 * and z match the subject's, and take the greatest height among them; the
 * winning slot index goes into the subject's tag at +100. The starting best
 * height is -5.0 in 12.20, held in the owner's one pool word, and it is what
 * the move receives when no slot matches -- the tag is then left untouched.
 * Every field read re-fetches its record, which is the shape to keep.
 */
void SceneActor_LandOnHighestPlatform(s32 subject)
{
    s32 best = (s32)0xffb00000;
    u32 i;

    for (i = 0; i <= 4; i++) {
        s32 slot = i + 10;

        if (slot == subject) continue;

        if ((Func_02004d56(slot)->x >> 20) != (Func_02004d5e(subject)->x >> 20)) continue;
        if ((Func_02004d6e(slot)->z >> 20) != (Func_02004d76(subject)->z >> 20)) continue;

        /*
         * 0x00100000 is one whole unit above the candidate's own height. The
         * comparison is signed, and a tie updates the best.
         */
        if (best > Func_02004d86(slot)->y + 0x100000) continue;

        best = Func_02004d98(slot)->y + 0x100000;
        *(u16 *)((u8 *)Func_02004da8(subject) + 100) = (u16)slot;
    }

    Func_02004dcc(subject, 0x40000, 0x20000);   /* 128 << 11, 128 << 10 */

    /*
     * Three separate lookups of the same record, in this order. The locals
     * fix the sequence, which argument evaluation order would not.
     */
    {
        Slot_02001c2c *target = Func_02004dca(subject);
        Slot_02001c2c *from = Func_02004dd2(subject);
        s32 z = Func_02004dda(subject)->z;

        Func_02004d4e(target, from->x, best, z);
    }

    Func_02004e14(subject);
    Func_02004e92(188);
    Func_02002ecc(subject);
    Func_02004dde(30);
}

void FieldScene_RunMiddleSequence(void)
{
    struct FieldActor saved;
    s32 i;
    s32 j;
    s32 found;

    Func_02004e0a();
    for (i = 0; i <= 2; i++) {
        if (((struct FieldActor *)Value1_02001d04(Func_02004e30, i + 12))->sprite->priority == 3
            && Value1_02001d04(Func_02004e10, i + 0x200) == 0) {
            Func_02004e52(i + 12);
            Func_02002d82();
            Func_02004e90(i + 12, 0, 0);
            Func_02004e36(i + 0x200);
            break;
        }
        if ((((struct FieldActor *)Value1_02001d04(Func_02004ef4, i + 12))->z.fixed >> 20) == 9
            && Value1_02001d04(Func_02004ed0, i + 0x200) == 0) {
            *(s32 *)(Value1_02001d04(Func_02004f16, i + 12) + 20) = 0;
            ((struct FieldActor *)Value1_02001d04(Func_02004f1e, i + 12))->velocity_y = 0;
            *(s32 *)(Func_02004f26(i + 12) + 60) = -0x80000000;
            ((struct FieldActor *)Func_02004f32(i + 12))->motion_flags = 0;
            *(u16 *)(Func_02004f3c(i + 12) + 100) = 0;
            found = i;
            for (j = 0; j < i; j++) {
                if (Value1_02001d04(Func_02004f1c, 0x200 + j) == 0) {
                    saved.x.fixed = ((struct FieldActor *)Value1_02001d04(Func_02004e6e, i + 12))->x.fixed;
                    saved.y.fixed = ((struct FieldActor *)Value1_02001d04(Func_02004e7a, i + 12))->y.fixed;
                    saved.z.fixed = ((struct FieldActor *)Value1_02001d04(Func_02004e86, i + 12))->z.fixed;
                    ((struct FieldActor *)Func_02004e92_a(i + 12))->x.fixed =
                        ((struct FieldActor *)Value1_02001d04(Func_02004e9e, j + 12))->x.fixed;
                    ((struct FieldActor *)Value1_02001d04(Func_02004ea8, i + 12))->y.fixed =
                        ((struct FieldActor *)Value1_02001d04(Func_02004eb0, j + 12))->y.fixed;
                    ((struct FieldActor *)Value1_02001d04(Func_02004eba, i + 12))->z.fixed =
                        ((struct FieldActor *)Value1_02001d04(Func_02004ec2, j + 12))->z.fixed;
                    ((struct FieldActor *)Func_02004ecc(j + 12))->x.fixed = saved.x.fixed;
                    ((struct FieldActor *)Func_02004ed8(j + 12))->y.fixed = saved.y.fixed;
                    ((struct FieldActor *)Func_02004ee4(j + 12))->z.fixed = saved.z.fixed;
                    found = j;
                    break;
                }
            }
            *(s32 *)(Func_02004f6a(found + 12) + 20) = 0;
            ((struct FieldActor *)Func_02004f74(found + 12))->velocity_y = 0;
            *(s32 *)(Func_02004f7e(found + 12) + 60) = -0x80000000;
            ((struct FieldActor *)Func_02004f8a(found + 12))->motion_flags = 0;
            *(u16 *)(Func_02004f96(found + 12) + 100) = 0;
            Value2(Func_02005010, 0x30000, 0x6000);
            ((struct FieldActor *)Func_0200502c())->motion_flags = 0;
            Call4(Func_02005034, 0xa80000, 0x80000, 0xb80000, 1);
            Func_02005040();
            Func_02003aee(found + 12);
            if ((((struct FieldActor *)Value1_02001d04(Func_02004fd4, found + 12))->x.fixed >> 20) == 8) {
                (*(s16 *)(Func_02004fe2(10) + 100))++;
                (*(s16 *)(Func_02004ff0(11) + 100))--;
            } else {
                (*(s16 *)(Func_02004ffe(10) + 100))--;
                (*(s16 *)(Func_0200500c(11) + 100))++;
            }
            ((struct FieldActor *)Func_0200501a(found + 12))->update = (void (*)(union FieldObject *))0x2009a99;
            Func_020039d4(40);
            ((struct FieldActor *)Func_0200502a(found + 12))->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
            Func_0200500e(0x200 + found);
            break;
        }
    }
    Func_0200503e();
}

void FieldScene_RunThreeStepSequence(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    Func_02005054();
    Func_02002028();
    Func_02005064_a();
}

void SceneState_SetSlot17And18Selectors(void)
{
    Func_02005068();

    if ((Func_02005086(17)->w8 >> 20) == 45) {
        Func_02005064(0x974);
    } else {
        Func_02005074(0x974);
    }

    if ((Func_020050a2(18)->w8 >> 20) == 46) {
        Func_02005080(0x975);
    } else {
        Func_02005090(0x975);
    }

    Func_020044dc();
    Func_020050b0();
}

void FieldScene_RunFourCallSequence(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    Func_020050bc();
    Func_02002090();
    Func_02003f40();
    Func_020050d0();
}

/* Runs six queued setup calls for this scene: two paired 6-argument calls
 * whose last two args repeat the first two (72,49 / 113,43), two 3-argument
 * id calls (100, 101) with zeroed remaining args, and two more 3-argument
 * calls (id 15, 16) passing a start/end pair where the second call's start
 * value (198<<18) equals the first call's end value. */
void FieldScene_RunLateSequenceHead(void)
{
    extern u8 Data_03001ebc[];

    Call6(Func_0200508e, 72, 49, 1, 1, 8, 49); /* main:080091c0 */
    Call6(Func_020050a0, 113, 43, 1, 1, 49, 43); /* main:080091c0 */
    EffectRuntime_SetCurrentPosition_1(100, 0, 0);
    EffectRuntime_SetCurrentPosition_2(101, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(15, 8912896, 51904512); /* 136<<16, 198<<18 */
    ObjectMotion_SetHorizontalPositionWithTerrain_2(16, 51904512, 45613056); /* 198<<18, 174<<18 */
}

/* Two six-argument calls whose first and fifth arguments repeat the same id
 * (8 and 49 respectively), followed by four three-argument calls each keyed
 * by an id with a trailing pair of values (-1, -1 or 0, 0). */
void FieldScene_RunLateSequenceSecond(void)
{
    extern u8 Data_03001ebc[];

    Call6(Func_020050f2, 8, 113, 1, 1, 8, 49);
    Call6(Func_02005104, 49, 107, 1, 1, 49, 43);
    EffectRuntime_SetCurrentPosition_1_02002040(100, -1, -1);
    EffectRuntime_SetCurrentPosition_2_02002040(101, -1, -1);
    ObjectMotion_SetHorizontalPositionWithTerrain_1_02002040(15, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_2_02002040(16, 0, 0);
}

void FieldScene_RunScene3c4SequenceA(s32 a0)
{
    extern u8 Data_03001ebc[];

    s32 record;
    s32 v6;
    s32 p5;
    s32 q;

    v6 = 0;
    Func_0200519e(a0);
    Call6(Func_0200515a, 83, 45, 11, 8, 19, 45);
    record = Value1(Func_020051d0, 19);
    p5 = *(s32 *)(record + 8);
    q = *(s32 *)(Value1(Func_020051d8, 19) + 16);
    q >>= 20;
    p5 >>= 20;
    Func_0200517e(20, 56, 1, 1, p5, q);
    record = Value1(Func_020051f4, 20);
    p5 = *(s32 *)(record + 8);
    q = *(s32 *)(Value1(Func_020051fc, 20) + 16);
    q >>= 20;
    p5 >>= 20;
    Func_020051a2_a(20, 56, 1, 1, p5, q);
    record = Value1(Func_02005218_a, 21);
    p5 = *(s32 *)(record + 8);
    q = *(s32 *)(Value1(Func_02005220, 21) + 16);
    q >>= 20;
    p5 >>= 20;
    Func_020051c6(20, 56, 1, 1, p5, q);
    record = Value1(Func_0200523c, 22);
    p5 = *(s32 *)(record + 8);
    q = *(s32 *)(Value1(Func_02005244, 22) + 16);
    q >>= 20;
    p5 >>= 20;
    Func_020051ea(20, 56, 1, 1, p5, q);
    record = Value1(Func_02005260, 23);
    p5 = *(s32 *)(record + 8);
    q = *(s32 *)(Value1(Func_02005268, 23) + 16);
    q >>= 20;
    p5 >>= 20;
    Func_0200520e(20, 56, 1, 1, p5, q);
    record = Value1(Func_02005284, 19);
    if ((*(s32 *)(record + 8) >> 20) == 25) {
        record = Value1(Func_02005294, 19);
        if ((*(s32 *)(record + 16) >> 20) == 49) {
            v6 = 1;
        }
    }
    record = Value1(Func_020052a4, 20);
    if ((*(s32 *)(record + 8) >> 20) == 23) {
        record = Value1(Func_020052b2, 20);
        if ((*(s32 *)(record + 16) >> 20) == 49) {
            v6 = (v6 + 1);
        }
    }
    record = Value1(Func_020052c2, 21);
    if ((*(s32 *)(record + 8) >> 20) == 25) {
        record = Value1(Func_020052d0, 21);
        if ((*(s32 *)(record + 16) >> 20) == 47) {
            v6 = (v6 + 1);
        }
    }
    record = Value1(Func_020052e0, 22);
    if ((*(s32 *)(record + 8) >> 20) == 23) {
        record = Value1(Func_020052ee, 22);
        if ((*(s32 *)(record + 16) >> 20) == 47) {
            v6 = (v6 + 1);
        }
    }
    record = Value1(Func_020052fe, 23);
    if ((*(s32 *)(record + 8) >> 20) == 24) {
        record = Value1(Func_0200530c, 23);
        if ((*(s32 *)(record + 16) >> 20) == 48) {
            v6 = (v6 + 1);
        }
    }
    if (v6 == 5) {
        if (Value1(Func_020052e8, 0x984) != 0) {
            Func_02005318();
            goto L_020022e4;
        }
        Func_02005310(20);
        Call2(Func_020053a0, 0xcccc, 0x1999);
        Call4(Func_020053ba, 0x1d80000, -1, 0x30c0000, 1);
        Func_020053c6();
        Func_02005334(30);
        Call1_020020a4(Func_0200532a, 0x984);
        Func_02005400(158);
        Call3(Func_020052e2, 0x200b3ec, 32, 46);
        Call6(Func_0200530e, 24, 60, 1, 1, 32, 47);
        Func_02005364(40);
    } else {
        if (Value1(Func_02005354, 0x984) != 0) {
            Func_02005376(20);
            Call2(Func_02005406, 0xcccc, 0x1999);
            Call4(Func_02005420, 0x1d80000, -1, 0x30c0000, 1);
            Func_0200542c();
            Func_0200539a(30);
            Call1_020020a4(Func_02005398, 0x984);
            Func_02005466(159);
            Call3(Func_02005348, 0x200b40c, 32, 46);
            Call6(Func_02005374, 31, 47, 1, 1, 32, 47);
            Func_020053ca(40);
        }
    }
    Func_020053de();
    L_020022e4:;
}

void FieldScene_RunLayoutAt83By45(void)
{
    Func_020053fa();
    {
        s32 width = 19;
        s32 height = 45;

        Func_020053b6(83, 45, 11, 8, width, height);
    }
    Func_020023e2();
    Func_020043c6();
    Func_02005422();
}

void SceneState_SetValue268bInScene(void)
{
    Func_02005428();
    Func_02005480(0, 1);
    Func_02005410(0x268B, 1);
    Func_02005444();
}

void FieldScene_RunScriptedStep953(void)
{
    extern u8 Data_03001ebc[];

    Func_0200544c();
    Func_0200542c_a(0x953, 1);
    Func_02005460();
}

s32 SceneData_SelectTableByWord224(void)
{
    if (Data_02000240[224] == (s32)&Value_000000ac) {
        return (s32)Data_0200bc0c;
    }
    return (s32)Data_0200bef4;
}

void FieldScene_PlaceAndPinSlots8And9(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    s32 row;

    {
        s32 p5 = 9, p6 = 38;
        Func_02005452(73, 38, 5, 5, p5, p6);
    }
    Func_020032ce(9, 8);

    {
        s32 col = Func_020054d0(8)->column >> 20;
        row = Func_020054d8(8)->row >> 20;
        Func_0200547e(2, 36, 1, 1, col, row);
    }

    {
        s32 col = Func_020054f4(9)->column >> 20;
        row = Func_020054fc(9)->row >> 20;
        Func_020054a2(2, 36, 1, 1, col, row);
    }
}

void FieldScene_PlaceAndPinSlots10And11(void)
{
    s32 row;

    {
        s32 k5 = 29, k6 = 30;
        Func_020054c2(93, 30, 6, 5, k5, k6);
    }
    Func_0200333e(11, 10);

    {
        s32 col20 = Func_02005540(10)->column >> 20;
        row = Func_02005548(10)->row >> 20;
        Func_020054ee(2, 36, 1, 1, col20, row);
    }

    {
        s32 col20 = Func_02005564(11)->column >> 20;
        row = Func_0200556c(11)->row >> 20;
        Func_02005512(2, 36, 1, 1, col20, row);
    }
}

void FieldScene_RunScene3c4_02002480(void)
{
    extern u8 Data_03001ebc[];

    s32 record;
    s32 p5;

    Call6(Func_02005532, 89, 49, 3, 2, 25, 49);
    Call6(Func_02005544, 89, 51, 8, 5, 25, 51);
    *(u8 *)(Func_020055ba(14) + 34) = 1;
    record = Value1_02002480(Func_020055c6, 12);
    p5 = *(s32 *)(record + 8);
    record = Value1_02002480(Func_020055ce, 12);
    p5 = p5 >> 20;
    Call6(Func_02005574, 22, 52, 1, 1, p5, (*(s32 *)(record + 16) >> 20));
    record = Value1_02002480(Func_020055ea, 13);
    p5 = *(s32 *)(record + 8);
    record = Value1_02002480(Func_020055f2, 13);
    p5 = p5 >> 20;
    Call6(Func_02005598, 22, 52, 1, 1, p5, (*(s32 *)(record + 16) >> 20));
    record = Value1_02002480(Func_0200560e, 14);
    p5 = *(s32 *)(record + 8);
    record = Value1_02002480(Func_02005616, 14);
    p5 = p5 >> 20;
    Call6(Func_020055bc, 22, 52, 1, 1, p5, (*(s32 *)(record + 16) >> 20));
}
