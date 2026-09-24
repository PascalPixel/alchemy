#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

/*
 * The Mars Star chamber shakes apart after Robin and Gerald take the Star.
 * The Wise One lifts a boulder over the chamber; Saturos, Menardi, Garcia
 * and Alex give up the Mars Star and leave, taking Sukureta and Jasmine.
 * The placed actors keep the Elemental Star Room's numbering: Menardi calls
 * Saturos by name here, and Alex agrees with Garcia.
 */

enum CollapseActor {
    ACTOR_SUKURETA = ACTOR_FIRST_PLACED + 1,
    ACTOR_SATUROS,
    ACTOR_MENARDI,
    ACTOR_GARCIA = ACTOR_FIRST_PLACED + 5,
    ACTOR_ALEX
};

#include "FACING_OBJECT.H"

enum CollapseMessage {
    MSG_JASMINE_WHAT_HAPPENED = 0x10cd,
    MSG_I_KNOW_ITS_A_ROCK = 0x10d6,
    MSG_SOMEONE_IS_LIFTING_IT = 0x10d7,
    MSG_SUKURETA_COULD_THAT_BE_THE_WISE_ONE = 0x10d8
};


typedef struct {
    u8 filler0[6];
    u16 unk6;
    u8 filler8[72];
    u8 *unk50;
} Ent;

typedef struct {
    u8 filler0[6];
    u16 unk6;
} Ent_02002820;

typedef struct {
    u8 unk[9];
    u8 unk0 : 2;
    u8 mode : 2;
    u8 unk4 : 4;
} SpriteMode;

typedef struct {
    u8 filler0[12];
    s32 unkC;
    u8 filler10[8];
    s32 unk18;
    s32 unk1C;
    u8 filler20[28];
    s32 unk3C;
} Ent_02002ba0;

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
    u8 filler14[16];
    s32 unk24;
    u8 filler28[4];
    s32 unk2C;
    u8 filler30[8];
    s32 unk38;
    u8 filler3C[4];
    s32 unk40;
} Ent_02002c1c;

/* resource_381 owner at 0x02002e0c (80 bytes), the twin of resource_371:3fb4.
 * Ported from that byte-exact owner: same field layout, same two calls. */
struct Actor_02002e0c {
    u8 filler00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c, only ever advanced by 0x10000 */
    s32 z;                          /* 0x10 */
    u8 filler14[4];
    s32 amplitude_x;                /* 0x18 */
    s32 amplitude_y;                /* 0x1c */
    u8 filler20[0x44];
    u16 frame;                      /* 0x64 */
    u8 filler66[2];
    struct Actor_02002e0c *anchor;  /* 0x68 */
};

/* resource_381 owner at 0x02002e5c (82 bytes), the twin of resource_371:4004.
 * Ported from that byte-exact owner: same field layout, same two calls. */
struct Actor_02002e5c {
    u8 filler00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c, only ever advanced by 0x10000 */
    s32 z;                          /* 0x10 */
    u8 filler14[4];
    s32 amplitude_x;                /* 0x18 */
    s32 amplitude_y;                /* 0x1c */
    u8 filler20[0x44];
    u16 frame;                      /* 0x64 */
    u8 filler66[2];
    struct Actor_02002e5c *anchor;  /* 0x68 */
};

extern u8 *Data_03001ec4[];
extern s32 Data_0200bb68;
extern s32 Data_0200bb00;
extern s32 Data_0200bb6c;
extern s32 Data_0200bb70;
extern s32 Data_0200bac0[];
extern s32 Data_03001e40;
extern s32 Data_0200b684[][2];
extern s32 Data_0200bb40[];

void Func_0200b4dc();
u8 *Func_0200b4ec(s32);
void *Func_020010bc();
void Func_02001570();
void Func_02002f1e();
void Func_02002f4a();
void Func_02002f5e();
void Func_02002fba();
void Func_020031d2();
void Func_020031f2();
void Func_020031fc();
void Func_0200321c();
void Func_02003226();
u8 *Func_0200379c(s32);
s32 Func_020038e2(void (*)(), s32);
s32 Func_020038ec(void (*)(), s32);
u8 *Func_02003f4e(s32);
s32 Func_0200432a();
s32 Func_02004364();
s32 Func_0200436c();
void Func_02004528(void);
void Func_020064fe(s32);
void Func_02005de6(void);
u32 Func_02005d2e(s32, s32);
u32 Func_02005d62(s32, s32);
void Func_02005c74(s32);
s32 Func_020061c4(u32, s32);
void Func_02005cb0(s32);
Ent *Func_02005d12(s32);
u8 *Func_0200b454(s32, s32, s32, s32);
s32 Func_0200b3ec(void (*)(), s32);
void Func_0200b3f4(void (*)());
Ent *Func_020061fc(s32);
void Func_02006620(void);

/*
 * Table getters published from the header words of overlay resource_381.
 * Only one overlay is resident at 0x02000000 at a time, so an address that
 * matches another overlay's names different bytes, not the same table.
 */

/* Set up the scene actors, run the two presentation phases, and dismiss the
 * actors through the shared exit sequence. Retained pending an exact build. */

void SceneEffect_AdvanceTenEntryTimers(void);

static __inline__ void Call1(void (*f)(), s32 a0)
{

    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    void Actor_ShowEmote();
    void ColorBuffer_ApplyTarget();
    void ColorBuffer_Interpolate();
    void Audio_PlayCue();

    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    void Actor_ShowEmote();
    void ColorBuffer_ApplyTarget();
    void ColorBuffer_Interpolate();
    void Audio_PlayCue();

    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    void Actor_ShowEmote();
    void ColorBuffer_ApplyTarget();
    void ColorBuffer_Interpolate();
    void Audio_PlayCue();

    f(a0, a1, a2, a3);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    void Actor_ShowEmote();
    void ColorBuffer_ApplyTarget();
    void ColorBuffer_Interpolate();
    void Audio_PlayCue();

    return f(a0, a1);
}

static __inline__ s32 InstallTask(s32 (*f)(void (*)(), s32), void (*callback)(), s32 key)
{
    void Actor_ShowEmote();
    void ColorBuffer_ApplyTarget();
    void ColorBuffer_Interpolate();
    void Audio_PlayCue();

    return f(callback, key);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call3_020029a4(void (*f)(s32, s32, s32), s32 a, s32 b, s32 c)
{
    void Actor_ShowEmote(s32, s32, s32);
    void ColorBuffer_ApplyTarget(s32, s32);
    void ColorBuffer_Interpolate(s32);
    void Audio_PlayCue(s32);

    f(a, b, c);
}

/*
 * Returns the in-image table address 0x0200b6d4, loaded and returned
 * without being dereferenced. The eight-byte owner includes its one pool
 * word, which sits past the bx lr. Published from the overlay header, not
 * reached by any call.
 */
u8 *SceneData_GetTableB6D4(void)
{
    return (u8 *)0x0200b6d4;
}

s32 Func_02000038(void)
{
    return 0;
}

/*
 * Returns the in-image table address 0x0200b704, loaded and returned
 * without being dereferenced. The eight-byte owner includes its one pool
 * word, which sits past the bx lr. Published from the overlay header, not
 * reached by any call.
 */
u8 *SceneData_GetTableB704(void)
{
    return (u8 *)0x0200b704;
}

/*
 * Returns the in-image table address 0x0200b710, loaded and returned
 * without being dereferenced. The eight-byte owner includes its one pool
 * word, which sits past the bx lr. Published from the overlay header, not
 * reached by any call.
 */
u8 *SceneData_GetTableB710(void)
{
    return (u8 *)0x0200b710;
}

/*
 * Returns the in-image table address 0x0200b998, loaded and returned
 * without being dereferenced. The eight-byte owner includes its one pool
 * word, which sits past the bx lr. Published from the overlay header, not
 * reached by any call.
 */
u8 *SceneData_GetTableB998(void)
{
    return (u8 *)0x0200b998;
}

void Scene_SaturosTakesHostages(void)
{
    void Actor_ShowEmote();
    void ColorBuffer_ApplyTarget();
    void ColorBuffer_Interpolate();
    void Audio_PlayCue();

    u32 i;
    u8 *record;
    s32 base5_3001ec4;
    s32 base5_3001ebc;
    u8 *p7;

    base5_3001ec4 = (s32)Data_03001ec4;
    p7 = *(u8 **)base5_3001ec4;
    Event_Begin();
    *(s32 *)(((s32)p7 + 0x40c)) = 0;
    Audio_PlayCue(141);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0xe80000, 0x9c0000);
    Actor_SetPosition(ACTOR_GERALD, 0xda0000, 0xac0000);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xb000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 0);
    Actor_SetPosition(ACTOR_JASMINE, 0x1db0000, 0x14c0000);
    Actor_SetPosition(ACTOR_SUKURETA, 0x1eb0000, 0x14c0000);
    Actor_SetPosition(ACTOR_MENARDI, 0x1cb0000, 0x15c0000);
    Actor_SetPosition(ACTOR_SATUROS, 0x1fb0000, 0x15c0000);
    Actor_SetPosition(ACTOR_GARCIA, 0x1d70000, 0x1320000);
    Actor_SetPosition(ACTOR_ALEX, 0x1df0000, 0x16a0000);
    Actor_SetSpritePriority(15, 1);
    base5_3001ebc = base5_3001ec4 - 8;
    Camera_MoveTo(0xe80000, -1, 0x9c0000, 0);
    Map_Redraw();
    *(s32 *)((*(s32 *)base5_3001ebc + 0x1c8)) = 8;
    Event_OpenScreen();
    Event_WaitForScreen();
    ColorBuffer_ApplyTarget(0x7fff, 0);
    ColorBuffer_Interpolate(4);
    Event_Wait(4);
    ColorBuffer_ApplyTarget(0x10000, 0);
    ColorBuffer_Interpolate(4);
    Event_Wait(4);
    Func_020010bc();
    ColorBuffer_ApplyTarget(0x7fff, 0);
    ColorBuffer_Interpolate(4);
    Event_Wait(16);
    Audio_PlayCue(144);
    ColorBuffer_ApplyTarget(0x10000, 0);
    ColorBuffer_Interpolate(4);
    Event_Wait(4);
    ColorBuffer_ApplyTarget(0x7fff, 0);
    ColorBuffer_Interpolate(4);
    Event_Wait(4);
    Audio_PlayCue(144);
    ColorBuffer_ApplyTarget(0x10000, 0);
    ColorBuffer_Interpolate(48);
    Event_Wait(48);
    Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
    Actor_Jump(ACTOR_GERALD, 6, 20);
    Func_02002f1e(1, 20, 20);
    FieldScene_RunVariantStep(0, 20, 40);
    Event_SetMessage(MSG_JASMINE_WHAT_HAPPENED);
    Call3((void (*)())Engine_EventShowMessageAndWait, 11, 0, 20);
    Call2((void (*)())Engine_EventShowMessage, 10, 0);
    Func_02002f4a(1, 20, 0);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 20);
    Func_02002f5e(0, 20, 0);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 20);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    Event_Wait(40);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xb000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 30);
    Func_02002fba(1, 20, 0);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    FieldScene_RunVariantStep(0, 20, 20);
    record = Func_0200b4ec(15);
    Actor_SetSpriteFlags(record, 0);
    Actor_SetPosition(15, 0x1450000, 0x12e0000);
    record = Func_0200379c(15);
    record[85] = 5;
    *(s32 *)(((s32)p7 + 0x40c)) = 1;
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    MapRender_WaitForValues();
    Event_Wait(150);
    Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 20);
    Event_ShowMessage(ACTOR_JASMINE, 0);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x3000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x3000, 10);
    Actor_SetPosition(ACTOR_JASMINE, 0x1db0000, 0x14c0000);
    Actor_SetPosition(ACTOR_SUKURETA, 0x1eb0000, 0x14c0000);
    Actor_SetPosition(ACTOR_MENARDI, 0x1cb0000, 0x15c0000);
    Actor_SetPosition(ACTOR_SATUROS, 0x1fb0000, 0x15c0000);
    Actor_SetPosition(ACTOR_GARCIA, 0x1d70000, 0x1320000);
    Actor_SetPosition(ACTOR_ALEX, 0x1df0000, 0x16a0000);
    Camera_SetSpeed(0x66666, 0xcccc);
    Camera_MoveTo(0x1480000, -1, 0x12b0000, 1);
    Camera_WaitForMove();
    Audio_PlayCue(167);
    ColorBuffer_ApplyTarget(0x205294, 2);
    ColorBuffer_Interpolate(20);
    Task_Wait(20);
    ColorBuffer_ApplyTarget(0x10000, 2);
    ColorBuffer_Interpolate(20);
    Event_Wait(200);
    Event_OpenMessage(0x1001, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_SetMessage(MSG_I_KNOW_ITS_A_ROCK);
    } else {
        Event_SetMessage(MSG_SOMEONE_IS_LIFTING_IT);
    }
    Event_ShowMessageAndWait(0x1001, 0, 80);
    Event_SetMessage(MSG_SUKURETA_COULD_THAT_BE_THE_WISE_ONE);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 20);
    FieldScene_RunVariantStep(1, 20, 0);
    *(s32 *)(((s32)p7 + 0x40c)) = 0;
    Audio_PlayCue(141);
    Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
    Event_Wait(80);
    *(s32 *)(((s32)p7 + 0x40c)) = 1;
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    MapRender_WaitForValues();
    Func_020031d2(0, 20, 60);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 30);
    Actor_FaceDirection(15, 0xa000, 40);
    Func_020031f2(1, 20, 20);
    Func_020031fc(0, 20, 20);
    Event_ShowMessageAndWait(0x1001, 0, 30);
    Actor_FaceDirection(15, 0x1000, 40);
    Func_0200321c(1, 20, 20);
    Func_02003226(0, 20, 20);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 30);
    Func_02001570();
    InstallTask(Func_020038e2, (void (*)())0x020090c5, 0xc80);
    InstallTask(Func_020038ec, (void (*)())0x0200935d, 0xc80);
    Event_Wait(240);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 30);
    Actor_SetPosition(ACTOR_JASMINE, 0x1db0000, 0x14c0000);
    Actor_SetPosition(ACTOR_SUKURETA, 0x1eb0000, 0x14c0000);
    Actor_SetPosition(ACTOR_MENARDI, 0x1cb0000, 0x15c0000);
    Actor_SetPosition(ACTOR_SATUROS, 0x1fb0000, 0x15c0000);
    Actor_SetPosition(ACTOR_GARCIA, 0x1d70000, 0x1320000);
    Actor_SetPosition(ACTOR_ALEX, 0x1df0000, 0x16a0000);
    Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x8000, 0);
    Actor_FaceDirection(ACTOR_MENARDI, 0x8000, 0);
    Actor_FaceDirection(ACTOR_SATUROS, 0x8000, 0);
    Actor_FaceDirection(ACTOR_GARCIA, 0x8000, 0);
    Actor_FaceDirection(ACTOR_ALEX, 0x8000, 0);
    record = Func_0200b4ec(5);
    Camera_MoveTo((*(s16 *)((s32)record + 10) << 16), -1, (*(s16 *)((s32)record + 18) << 16), 1);
    Camera_WaitForMove();
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_GARCIA, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_GARCIA, 0);
    Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
    Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 10);
    Actor_SetAnimation(ACTOR_ALEX, 4);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
    Actor_FaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 40);
    Actor_SetAnimation(ACTOR_SATUROS, 4);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
    Actor_ShowEmote(ACTOR_JASMINE, 0x100, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x3000, 40);
    Event_ShowMessageAndWait(0x4005, 0, 10);
    Actor_FaceDirection(ACTOR_SATUROS, 0xb000, 0);
    Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 40);
    Actor_FaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 40);
    Actor_ShowEmote(ACTOR_SATUROS, 0x105, 40);
    Actor_FaceDirection(ACTOR_SATUROS, 0xb000, 10);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
    Actor_FaceDirection(ACTOR_SATUROS, 0x8000, 10);
    Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
    Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 10);
    Actor_SetAnimation(ACTOR_SATUROS, 3);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 40);
    Actor_FaceDirection(ACTOR_ALEX, 0xb000, 60);
    Actor_SetAnimationAndWait(ACTOR_ALEX, 3);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 10);
    Actor_FaceDirection(ACTOR_GARCIA, 0x3000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x3000, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 0);
    Actor_FaceDirection(ACTOR_MENARDI, 0x3000, 0);
    Actor_FaceDirection(ACTOR_SATUROS, 0x5000, 20);
    Actor_FaceDirection(ACTOR_ALEX, 0xd000, 20);
    Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 10);
    Actor_ShowEmote(ACTOR_GARCIA, 0x103, 0);
    Actor_SetSpeed(ACTOR_GARCIA, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_GARCIA, 0x1d7, 0x13a);
    Actor_StartRepeatedMotion(ACTOR_GARCIA, 3);
    Event_ShowMessageAndWait(ACTOR_GARCIA, 0, 10);
    Actor_FaceDirection(ACTOR_ALEX, 0xb000, 10);
    Actor_SetAnimation(ACTOR_ALEX, 4);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 10);
    Actor_StartRepeatedMotion(ACTOR_GARCIA, 3);
    Event_ShowMessageAndWait(ACTOR_GARCIA, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_ShowMessageAndWait(0x4009, 0, 10);
    Actor_FaceDirection(ACTOR_SATUROS, 0xb000, 20);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 30);
    Actor_FaceDirection(ACTOR_MENARDI, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_MENARDI, 3);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 30);
    Actor_SetAnimationAndWait(ACTOR_MENARDI, 4);
    Event_Wait(10);
    Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 10);
    Actor_FaceDirection(ACTOR_GARCIA, 0x5000, 20);
    Actor_RunRepeatedMotion(ACTOR_GARCIA, 2);
    Event_Wait(20);
    Actor_FaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 0);
    Event_Wait(30);
    Actor_SetAnimation(ACTOR_SATUROS, 3);
    Actor_SetAnimationAndWait(ACTOR_MENARDI, 3);
    Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 0);
    Actor_FaceDirection(ACTOR_SATUROS, 0xb000, 0);
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_GARCIA, 1);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_GARCIA, 0x8000, 0);
    Event_Wait(40);
    Actor_ShowEmote(ACTOR_GARCIA, 0x102, 0);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_ALEX, 0xb000, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
    Event_ShowMessageAndWait(0x200e, 0, 30);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xb000, 0);
    Actor_FaceDirection(ACTOR_GARCIA, 0x3000, 20);
    Actor_ShowEmote(ACTOR_JASMINE, 0x102, 40);
    Event_ShowMessageAndWait(0x2005, 0, 40);
    Actor_ShowEmote(ACTOR_GARCIA, 0x105, 60);
    Actor_SetAnimationAndWait(ACTOR_GARCIA, 3);
    Actor_ShowEmote(ACTOR_SATUROS, 0x101, 0);
    Actor_ShowEmote(ACTOR_MENARDI, 0x101, 60);
    Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 60);
    Actor_SetAnimationAndWait(ACTOR_GARCIA, 3);
    Event_Wait(40);
    Actor_ShowEmote(ACTOR_JASMINE, 0x102, 0);
    Actor_ShowEmote(ACTOR_SUKURETA, 0x102, 40);
    Actor_FaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 20);
    Actor_SetAnimation(ACTOR_SATUROS, 3);
    Actor_SetAnimationAndWait(ACTOR_MENARDI, 3);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_SATUROS, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_MENARDI, 0x9999, 0x4ccc);
    Actor_WalkTo(ACTOR_MENARDI, 0x1db, 0x15c);
    Actor_WalkTo(ACTOR_SATUROS, 0x1eb, 0x15c);
    Actor_WaitForMove(ACTOR_MENARDI);
    Actor_WaitForMove(ACTOR_SATUROS);
    Actor_SetAnimation(ACTOR_MENARDI, 1);
    Actor_SetAnimation(ACTOR_SATUROS, 1);
    Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 0);
    Actor_FaceDirection(ACTOR_SATUROS, 0xb000, 0);
    Actor_FaceDirection(ACTOR_GARCIA, 0x3000, 10);
    Actor_ShowEmote(ACTOR_MENARDI, 0x103, 0);
    Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
    Event_ShowMessage(0x200b, 0);
    Actor_SetSpeed(ACTOR_MENARDI, 0x19999, 0xcccc);
    Actor_SetSpeed(ACTOR_JASMINE, 0x13333, 0x9999);
    Actor_WalkToAndWait(ACTOR_MENARDI, 0x1db, 0x152);
    record = Func_02003f4e(11);
    ((struct FacingObject *)record)->facing_flags = (u8)(254 & ((struct FacingObject *)record)->facing_flags);
    Actor_WalkToAndWait(ACTOR_MENARDI, 0x1db, 0x15c);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 1);
    Actor_Jump(ACTOR_JASMINE, 4, 0);
    Actor_WalkToAndWait(ACTOR_JASMINE, 0x1cb, 0x13c);
    Actor_SetSpeed(ACTOR_JASMINE, 0x8000, 0x4000);
    Actor_ShowEmote(ACTOR_GARCIA, 0x103, 0);
    Actor_StartRepeatedMotion(ACTOR_GARCIA, 3);
    Event_ShowMessageAndWait(ACTOR_GARCIA, 0, 30);
    Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
    Event_Wait(20);
    ((struct FacingObject *)record)->facing_flags |= 1;
    Actor_SetSpeed(ACTOR_MENARDI, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_MENARDI, 0x1db, 0x14c);
    Actor_FaceDirection(ACTOR_MENARDI, 0xb000, 20);
    Event_ShowMessage(0x200b, 0);
    Actor_FaceDirection(ACTOR_SATUROS, 0xb000, 0);
    Actor_StartRepeatedMotion(ACTOR_SATUROS, 2);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 20);
    Actor_ShowEmote(ACTOR_MENARDI, 0x102, 20);
    Actor_FaceDirection(ACTOR_JASMINE, 0x3000, 0);
    Actor_FaceDirection(ACTOR_MENARDI, 0x3000, 20);
    Actor_SetAnimationAndWait(ACTOR_SATUROS, 4);
    Actor_SetAnimationAndWait(ACTOR_MENARDI, 3);
    Actor_FaceDirection(ACTOR_MENARDI, 0xb000, 20);
    Actor_SetAnimationAndWait(ACTOR_MENARDI, 3);
    Actor_SetAnimationAndWait(ACTOR_GARCIA, 3);
    Event_Wait(20);
    Actor_WalkTo(ACTOR_JASMINE, 0x1b0, 0x13c);
    Actor_WalkToAndWait(ACTOR_GARCIA, 0x1a6, 0x137);
    Actor_SetAnimation(ACTOR_JASMINE, 1);
    Actor_FaceDirection(ACTOR_JASMINE, 0x3000, 0);
    Actor_FaceDirection(ACTOR_GARCIA, 0x3000, 20);
    Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
    Actor_FaceDirection(ACTOR_SATUROS, 0xd000, 20);
    Event_ShowMessageAndWait(0x100a, 0, 40);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 30);
    Actor_SetSpeed(ACTOR_SUKURETA, 0x8000, 0x4000);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1eb, 0x128);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 40);
    Actor_SetSpeed(ACTOR_SATUROS, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_MENARDI, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_GARCIA, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_ALEX, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_SATUROS, 0x1d7, 0x134);
    Actor_FaceDirection(ACTOR_SATUROS, 0x5000, 0);
    Actor_WalkToAndWait(ACTOR_MENARDI, 0x1c7, 0x134);
    Actor_FaceDirection(ACTOR_MENARDI, 0x3000, 0);
    Actor_WalkToAndWait(ACTOR_ALEX, 0x1e7, 0x134);
    Actor_FaceDirection(ACTOR_SATUROS, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 0);
    Actor_FaceDirection(ACTOR_ALEX, 0xb000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xd000, 0);
    Actor_FaceDirection(ACTOR_GARCIA, 0xd000, 0);
    Camera_SetSpeed(0x8000, 0x1000);
    Camera_MoveTo(0x1d80000, -1, 0x12c0000, 1);
    Camera_WaitForMove();
    Event_Wait(40);
    GameFlag_Set(0x246);
    Actor_SetSpeed(ACTOR_SATUROS, 0x8000, 0x4000);
    Actor_SetSpeed(ACTOR_MENARDI, 0x8000, 0x4000);
    Actor_SetSpeed(ACTOR_ALEX, 0x8000, 0x4000);
    Actor_SetAnimationAndWait(ACTOR_SATUROS, 3);
    FieldScene_RunScene381_02000e30(10);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    FieldScene_RunScene381_02000e30(9);
    Actor_SetAnimationAndWait(ACTOR_MENARDI, 3);
    FieldScene_RunScene381_02000e30(11);
    Actor_FaceDirection(ACTOR_JASMINE, 0x9000, 40);
    Actor_StartRepeatedMotion(ACTOR_JASMINE, 2);
    Event_ShowMessageAndWait(0x2005, 0, 40);
    Actor_FaceDirection(ACTOR_GARCIA, 0x3000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xb000, 30);
    Actor_FaceDirection(ACTOR_GARCIA, 0xe000, 0);
    FieldScene_RunScene381_02000e30(5);
    FieldScene_RunScene381_02000e30(13);
    Actor_FaceDirection(ACTOR_ALEX, 0x7000, 40);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 30);
    Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 30);
    FieldScene_RunScene381_02000e30(14);
    Scene_RunExtendedEffectPresentation();
    Func_0200b4dc(5);
}

void FieldScene_RunScene381_02000e30(s32 a0)
{
    u8 *rec7;
    s32 recA;
    s32 rec2;
    u8 i;

    recA = Func_0200432a(8);
    *(s32 *)(recA + 24) = 0x10000;
    *(s32 *)(recA + 28) = 0x10000;
    Value3(Engine_ActorWalkToAndWait, a0, 0x1d7, 0x122);
    Actor_FaceDirection(a0, 0xc000, 0);
    Event_Wait(10);
    Actor_SetPosition(8, 0x1d70000, 0x1220000);
    rec7 = Value1(Func_02004364, a0);
    rec2 = Func_0200436c(a0);
    Actor_SetSpriteFlags(rec2, 0);
    Actor_SetChildValue(a0, 0x100);
    rec7[85] = 0;
    Audio_PlayCue(201);
    i = 0;
    do {
        *(s32 *)(rec7 + 12) += 0x8000;
        Event_Wait(1);
        i++;
    } while (i != 60);
    Audio_PlayCue(190);
    i = 0;
    do {
        *(s32 *)(rec7 + 12) += 0x1999;
        *(s32 *)(rec7 + 24) -= 0x28f;
        *(s32 *)(rec7 + 28) -= 0x28f;
        *(s32 *)(recA + 24) -= 0x28f;
        *(s32 *)(recA + 28) -= 0x28f;
        Event_Wait(1);
        i++;
    } while (i != 90);
    Actor_SetPosition(a0, 0, 0);
    Actor_SetPosition(8, 0, 0);
}

void Resource381_NoOpCallbackA(void)
{
}

void Resource381_NoOpCallbackB(void)
{
}

s32 FieldScene_RunWhenWord225Is10(void)
{
    if (gGameState.entrance == 10) {
        Func_02004528();
        Scene_SaturosTakesHostages();
    }
    return 0;
}

s32 OverlayObject_SetRecordAngleFromHeading(Ent *p)
{
    *(u16 *)(p->unk50 + 30) = p->unk6 + 0x4000;
    return 1;
}

void SceneState_InitStateWordsAndSlots(void)
{
    s32 *p;
    u32 i;

    Data_0200bb68 = 63;
    Data_0200bb00 = 0;
    Data_0200bb6c = 0;
    Data_0200bb70 = 120;
    p = Data_0200bac0;
    for (i = 0; i < 16; i++) {
        *p++ = 0;
    }
}

void SceneState_UpdateRandomTimerLevel(void)
{
    u32 v;

    if (Data_0200bb70 != 0) {
        Data_0200bb70--;
        return;
    }
    if (Data_0200bb6c != 0) {
        Data_0200bb6c--;
    } else {
        Data_0200bb6c = (u32)(Random_Next() << 2) >> 16;
    }
    v = Data_0200bb6c;
    switch (v) {
    case 3:
        Data_0200bb68 = v;
        Data_0200bb70 = ((u32)(Random_Next() * 20) >> 16) + 40;
        break;
    case 2:
        Data_0200bb68 = 15;
        Data_0200bb70 = ((u32)(Random_Next() * 40) >> 16) + 80;
        break;
    case 1:
        Data_0200bb68 = 63;
        Data_0200bb70 = ((u32)(Random_Next() * 80) >> 16) + 160;
        break;
    default:
        Data_0200bb68 = 127;
        Data_0200bb70 = ((u32)(Random_Next() * 160) >> 16) + 320;
        break;
    }
}

void SceneActor_MoveTo232_125AndFace4000(s32 no)
{
    Ent_02002820 *rec;

    rec = Func_02005d12(no);
    Actor_SetPosition(no, 0xe80000, 0x7d0000);
    rec->unk6 = 0x4000;
    Actor_SetSpritePriority(no, 3);
}

void SceneState_ApplyRectsByCondition(s32 a)
{
    if (a != 0) {
        s32 x;
        s32 y;
        x = 1;
        Map_CopyCellsTo(8, 47, 64, 7, x, x);
        y = 2;
        Map_CopyCellsTo(7, 48, 63, 8, y, x);
        Map_CopyCellsTo(7, 49, 63, 9, y, x);
    } else {
        s32 x;
        x = 1;
        Map_CopyCellsTo(56, 0, 64, 7, x, x);
        Map_CopyCellsTo(56, 0, 63, 8, x, x);
        Map_CopyCellsTo(56, 0, 63, 9, 2, x);
        Map_CopyCellsTo(58, 25, 64, 8, x, x);
    }
    Map_Redraw();
}

void SceneState_ApplyRectPairByFlag(s32 a)
{
    if (a != 0) {
        s32 n;
        n = 2;
        Map_CopyCellsTo(9, 45, 65, 5, n, n);
        Map_CopyCellsTo(11, 46, 67, 6, 1, n);
    } else {
        s32 n;
        n = 2;
        Map_CopyCellsTo(89, 2, 65, 5, n, n);
        Map_CopyCellsTo(102, 32, 67, 6, 1, n);
    }
    Map_Redraw();
}

void FieldScene_RunRandomHalfBranch(void)
{
    if ((Data_03001e40 & 1) == 0) {
        if (Func_02005d2e(Random_Next(), 100) > 50) {
            SceneState_ApplyRectsByCondition(1);
        } else {
            SceneState_ApplyRectsByCondition(0);
        }
    }
}

void FieldScene_RunLateRandomHalfBranch(void)
{
    if ((Data_03001e40 & 1) == 0) {
        if (Func_02005d62(Random_Next(), 100) > 50) {
            SceneState_ApplyRectPairByFlag(1);
        } else {
            SceneState_ApplyRectPairByFlag(0);
        }
    }
}

void FieldScene_RunFourWayEffectSequence(u32 mode)
{
    extern u8 *Data_0200bb10[];
    void Actor_ShowEmote(s32, s32, s32);
    void ColorBuffer_ApplyTarget(s32, s32);
    void ColorBuffer_Interpolate(s32);
    void Audio_PlayCue(s32);

    u32 i, zero;
    s32 x, y, z;
    s32 *pos;
    u8 *obj, *sprite;

    for (i = 0; i <= 15; i++)
        Actor_Destroy(i + 16);
    switch (mode) {
    case 0: ColorBuffer_ApplyTarget(0x4039d2, 1); break;
    case 1: ColorBuffer_ApplyTarget(0x4049d2, 1); break;
    case 2: ColorBuffer_ApplyTarget(0x404a4e, 1); break;
    case 3: ColorBuffer_ApplyTarget(0x403a52, 1); break;
    }
    ColorBuffer_Interpolate(60);
    Audio_PlayCue(214);
    i = 0;
    zero = i;
    for (pos = &Data_0200b684[0][0]; i <= 9; i++, pos += 2) {
        x = pos[0];
        y = pos[1];
        z = 0;
        switch (mode) {
        case 0: x += 0xe80000; z = 0x900000; break;
        case 1: x += 0xe80000; z = 0x1d00000; break;
        case 2: x += 0x2c70000; z = 0x900000; break;
        case 3: x += 0x2c70000; z = 0x1d00000; break;
        }
        Data_0200bb40[i] = zero;
        obj = Func_0200b454(284, x, y, z);
        Data_0200bb10[i] = obj;
        obj[85] = zero;
        sprite = *(u8 **)(obj + 80);
        sprite[38] = zero;
        ((SpriteMode *)sprite)->mode = 1;
        Object_SetAnimation(obj, 6);
        Task_Wait(6);
    }
    if (mode == 0) {
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 256, 0);
        Actor_ShowEmote(ACTOR_GERALD, 256, 0);
    }
    Task_Wait(20);
    Func_0200b3ec(SceneEffect_AdvanceTenEntryTimers, 3200);
    Audio_PlayCue(246);
    Data_0200bb40[0] = 1; Task_Wait(6);
    Data_0200bb40[1] = 1; Task_Wait(6);
    Data_0200bb40[2] = 1; Task_Wait(6);
    Data_0200bb40[3] = 1; Task_Wait(6);
    Data_0200bb40[4] = 1; Task_Wait(6);
    Data_0200bb40[5] = 1; Task_Wait(6);
    Data_0200bb40[6] = 1; Task_Wait(6);
    Data_0200bb40[7] = 1; Task_Wait(6);
    Data_0200bb40[8] = 1; Task_Wait(6);
    Data_0200bb40[9] = 1; Task_Wait(6);
    for (;;) {
        for (i = 0; i <= 9; i++) {
            if (Data_0200bb40[i] != 0) {
                i = 888;
                break;
            }
        }
        if (i != 888)
            break;
        Task_Wait(1);
    }
    Task_Wait(40);
    Func_0200b3f4(SceneEffect_AdvanceTenEntryTimers);
    ColorBuffer_ApplyTarget(65536, 1);
    ColorBuffer_Interpolate(40);
}

void SceneEffect_AdvanceTenEntryTimers(void)
{
    extern Ent *Data_0200bb10[];

    u32 i;
    s32 v;
    Ent_02002ba0 *p;

    for (i = 0; i <= 9; i++) {
        v = Data_0200bb40[i];
        if (v != 0) {
            p = Data_0200bb10[i];
            if ((u32)v <= 8) {
                p->unk18 += -0x1ccc;
                p->unk1C += 0x8000;
                p->unkC += 0x4ccc;
                p->unk3C += 0x4ccc;
            } else {
                p->unkC += 0x140000;
                p->unk3C += 0x140000;
            }
            v = Data_0200bb40[i] + 1;
            Data_0200bb40[i] = v;
            if ((u32)v > 14) {
                Data_0200bb40[i] = 0;
            }
        }
    }
}

void SceneActor_PlaceAtTileAndRunSteps(s32 a, s32 b)
{
    Ent_02002c1c *p;

    p = Func_020061fc(a);
    b = b << 16;
    a = a << 16;
    Camera_MoveTo(a, -1, b, 1);
    ColorBuffer_ApplyTarget(0, 0);
    ColorBuffer_Interpolate(20);
    Task_Wait(40);
    p->unk10 = b;
    p->unk8 = a;
    p->unk38 = 0x80000000;
    p->unk40 = 0x80000000;
    p->unk24 = 0;
    p->unk2C = 0;
    Task_Wait(5);
    Map_Redraw();
    Task_Wait(5);
    ColorBuffer_ApplyTarget(0x10000, 0);
    ColorBuffer_Interpolate(20);
    Task_Wait(30);
}

void SceneState_ConfigureEightCornerRegions(void)
{
    s32 x;
    s32 y;
    s32 z;
    s32 w;
    s32 v;

    x = 0;
    Map_CopyCellAttributes(14, 8, 1, 1, 10, x);
    Map_CopyCellAttributes(14, 28, 1, 1, 11, x);
    Map_CopyCellAttributes(44, 8, 1, 1, 12, x);
    Map_CopyCellAttributes(44, 28, 1, 1, 13, x);
    z = 14;
    y = 8;
    Map_CopyCellAttributes(13, 8, 1, 1, z, y);
    w = 28;
    Map_CopyCellAttributes(13, 28, 1, 1, z, w);
    v = 44;
    Map_CopyCellAttributes(43, 8, 1, 1, v, y);
    Map_CopyCellAttributes(43, 28, 1, 1, v, w);
}

void FieldScene_RunVariantStep(s32 a, s32 b, s32 c)
{
    if (a == 1) {
        Audio_PlayCue(0x134);
        ColorBuffer_ApplyTarget(0x203a52, 1);
    } else {
        Audio_PlayCue(0x121);
        ColorBuffer_ApplyTarget(0x10000, 1);
    }
    ColorBuffer_Interpolate(b);
    if (c != 0) {
        Event_Wait(c);
    }
}

void FieldScene_RunStepByRuntimeBits(s32 a)
{
    if ((Data_03001e40 & 2) != 0) {
        Object_SetPartPalettes(a, 7);
    } else {
        Object_SetPartPalettes(a, 0);
    }
    if ((Data_03001e40 & 15) == 0) {
        Func_02005c74(a);
    }
}

void SceneState_ForwardByRuntimeWordBits(s32 a)
{
    volatile u32 *p = (u32 *)0x03001e40;

    if (*p & 1) {
        Object_SetPartPalettes(a, Func_020061c4(*p >> 1, 6));
    }
    if ((*p & 15) == 0) {
        Func_02005cb0(a);
    }
}

void SceneEffect_UpdateArcOverAnchor(struct Actor_02002e0c *self)
{
    struct Actor_02002e0c *anchor;
    s32 frame;
    s32 amplitude;

    anchor = self->anchor;
    self->frame = (u16)(self->frame + 1);
    frame = (s16)self->frame;

    if (frame > 31) {
        Engine_ObjectDispatchRelease(self);
        return;
    }

    amplitude = Math_Sin(frame << 10);
    self->amplitude_x = amplitude;
    self->amplitude_y = amplitude;
    self->x = anchor->x;
    self->y += 0x10000;
    self->z = anchor->z + (0x10000 - amplitude) * 5 + 0x80000;
}

void SceneEffect_UpdateAnchoredRiseArc(struct Actor_02002e5c *obj)
{
    struct Actor_02002e5c *anchor;
    s32 frame;
    s32 amp;

    anchor = obj->anchor;
    obj->frame = (u16)(obj->frame + 1);
    frame = (s16)obj->frame;

    if (frame > 31) {
        Engine_ObjectDispatchRelease(obj);
        return;
    }

    amp = Math_Sin(frame << 10);
    obj->amplitude_x = amp;
    obj->amplitude_y = -amp;
    obj->x = anchor->x;
    obj->y += 0x10000;
    obj->z = anchor->z - (0x10000 - amp) * 5 + 0x100000;
}

void SceneState_SetValue140Mode0(void)
{
    Psynergy_Begin(140, 0);
}

void FieldScene_CallHelper6620(void)
{
    Func_02006620();
}

void FieldScene_RunActor15TwoStep(void)
{
    Func_020064fe(15);
    Func_02005de6();
}
