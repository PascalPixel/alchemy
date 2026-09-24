#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define CreateOverlayObject Func_02001b22
#define SetOverlayObjectMode Func_02001b7c
#define SetOverlayObjectSlot Func_02001c2c

#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"
#include "CONFIGURED_EFFECT_SPAWN.H"

/* Shared 22-byte head leaf proved identical for this overlay family. */
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

/*
 * Per-frame integrator for an effect record -- resource_396. It adds the
 * velocity at +68/+72/+76 into the position at +8/+12/+16, decays two of the
 * three velocities, accumulates the rates at +48/+52, and turns the sprite.
 */

extern s16 *Data_0200add0;
extern u8 Data_02009ec8[];
extern u8 Data_0200adc0[];
extern u8 Data_0200adcc[];

void Func_020007d6(int, int, int, int);
void Func_020007e8(int, int, int, int);
void Func_02000800(int, int, int, int);
void Func_02000818(int, int, int, int);
void Func_02000832(int, int, int, int);
void Func_02000844(int, int, int, int);
void Func_0200085c(int, int, int, int);
void Func_02000874(int, int, int, int);
void Func_0200088e(int, int, int, int);
void Func_020008a0(int, int, int, int);
void Func_020008b8(int, int, int, int);
void Func_020008d0(int, int, int, int);
void Func_020008ea(int, int, int, int);
void Func_020008fc(int, int, int, int);
void Func_02000914(int, int, int, int);
void Func_0200092c(int, int, int, int);
void Func_02000946(int, int, int, int);
void Func_02000958(int, int, int, int);
void Func_02000970(int, int, int, int);
void Func_02000988(int, int, int, int);
void Func_020009a2(int, int, int, int);
void Func_02001e14();
void Func_02001f34();
void Func_02001f46();
void Func_02001fc6();
void Func_0200205e();
void Func_020020b2();
void Func_02002124();
void Func_02002144();
void Func_0200222a();
void Func_02002240();
void Func_02002418();
s32 Func_02002420();
s32 Func_0200243a();
void Func_02002806();
void Func_0200281a();
void Func_0200293c();
s32 Func_02002944();
s32 Func_020029fa();
void Func_02002a06();
void Func_02002a20_scene_primary_script();
void Func_0200962d();
void Func_02003282(void (*callback)(void));
void Func_02009244(void);

/* Contiguous unnamed leaf-owner run for resource_396. */

/* Return the overlay's first persistent workspace block. */

/* Return the overlay's second persistent workspace block. */

/* Return the overlay's third persistent workspace block. */

/* Return the overlay's fourth persistent workspace block. */

/* Return the overlay's presentation workspace block. */

/* Clear the active story variant once the scene controller is idle. */

/*
 * A loader-relocated call word rather than a runtime address; it turns a value
 * and a duration into a per-frame step. Declared without a prototype, and the
 * call site passes two arguments.
 */

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

void SetEffectRecordMode(struct EffectWork *work, s32 mode)
{
    work->record->mode = mode;
}

/*
 * Register this overlay's palette-fade callback.
 *
 * Both addresses are named at their decoded sites: the reference bl's raw
 * decode is 0x02003282 (the registration import as this stream reaches it,
 * semantically Func_080000d8), and the pool word is 0x02009245 -- the
 * callback lives at 0x02009244 in a later-loaded RAM segment, not at this
 * stream's own same-shaped 0x02001244.
 */
void *OverlayObject_PrepareSpawnedObject(s32 x, s32 y, s32 z, s32 kind)
{
    void *obj;
    void *rec;
    s32 mask;

    obj = Object_Create(kind, x, y, z);
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

void *OverlayObject_CreateConfigured(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "CREATE_CONFIGURED_OVERLAY_OBJECT_BODY.INC"
}

void *SceneData_GetTablea5cc(void)
{
    return (void *)0x0200a5cc;
}

void *SceneData_GetTablea9d4(void)
{
    return (void *)0x0200a9d4;
}

void *SceneData_GetTableaa14(void)
{
    return (void *)0x0200aa14;
}

void *SceneData_GetTableaa58(void)
{
    return (void *)0x0200aa58;
}

void FieldScene_RunStep200(void) { Func_020007d6(0x200, 64, 35, 21); }

void FieldScene_RunStep201(void) { Func_020007e8(0x201, 65, 35, 22); }

void FieldScene_RunStep202(void) { Func_02000800(0x202, 66, 35, 23); }

void FieldScene_RunStep203(void) { Func_02000818(0x203, 67, 35, 24); }

void FieldScene_RunStep204(void) { Func_02000832(0x204, 68, 35, 25); }

void FieldScene_RunStep205(void) { Func_02000844(0x205, 69, 35, 26); }

void FieldScene_RunStep206(void) { Func_0200085c(0x206, 70, 35, 27); }

void FieldScene_RunStep207(void) { Func_02000874(0x207, 71, 35, 28); }

void FieldScene_RunStep208(void) { Func_0200088e(0x208, 72, 35, 29); }

void FieldScene_RunStep209(void) { Func_020008a0(0x209, 73, 35, 31); }

void FieldScene_RunStep20a(void) { Func_020008b8(0x20a, 74, 35, 32); }

void FieldScene_RunStep20b(void) { Func_020008d0(0x20b, 79, 35, 50); }

void FieldScene_RunStep20c(void) { Func_020008ea(0x20c, 75, 35, 51); }

void FieldScene_RunStep20d(void) { Func_020008fc(0x20d, 76, 35, 52); }

void FieldScene_RunStep20e(void) { Func_02000914(0x20e, 77, 35, 53); }

void FieldScene_RunStep20f(void) { Func_0200092c(0x20f, 78, 35, 54); }

void FieldScene_RunStep210(void) { Func_02000946(0x210, 80, 35, 55); }

void FieldScene_RunStep211(void) { Func_02000958(0x211, 81, 35, 56); }

void FieldScene_RunStep212(void) { Func_02000970(0x212, 82, 35, 57); }

void FieldScene_RunStep213(void) { Func_02000988(0x213, 83, 35, 58); }

void FieldScene_RunStep214(void) { Func_020009a2(0x214, 84, 35, 59); }

void SceneState_ClearStoryVariantWhenIdle(void)
{
    if (Leader_CheckAhead() == 0)
        *Data_0200add0 = -1;
}

void *SceneData_GetTableAae8(void)
{
    return (void *)0x0200aae8;
}

void FieldScene_RunFourActorEncounter(void)
{
    u32 i;
    s32 rec;
    s32 record;
    s32 v6;
    s32 v5;
    s32 base5_200962d;
    s32 base5_2009ec8;

    rec = GameFlag_IsSet(3);
    *(u8 *)(Func_02002420(3) + 35) &= 254;
    Actor_SetSpritePriority(ACTOR_MIA, 2);
    *(u8 *)(Func_0200243a(0) + 35) &= 254;
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
    Func_02002418(184);
    Audio_PlayCue(17);
    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_MIA, 0xcccc, 0x6666);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0xa60000, 0x500000);
    v6 = 192;
    record = Actor_Get(ACTOR_PARTY_LEADER);
    *(u16 *)(record + 6) = (v6 << 8);
    Actor_SetPosition(ACTOR_GERALD, 0x940000, 0x5a0000);
    record = Actor_Get(ACTOR_GERALD);
    *(u16 *)(record + 6) = (v6 << 8);
    Actor_SetPosition(ACTOR_IVAN, 0xb60000, 0x5a0000);
    record = Value1(Engine_ActorGet, ACTOR_IVAN);
    *(u16 *)(record + 6) = (v6 << 8);
    if (rec != 0) {
        Actor_SetPosition(ACTOR_MIA, 0xa60000, 0x680000);
        record = Value1(Engine_ActorGet, ACTOR_MIA);
        *(u16 *)(record + 6) = (v6 << 8);
    }
    Func_02001e14(0);
    Task_Wait(10);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 48;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Camera_SetSpeed(0x13333, 0x2666);
    Camera_MoveTo(0xa80000, -1, 0x980000, 1);
    Camera_WaitForMove();
    Event_Wait(10);
    v5 = 10;
    Audio_PlayCue(123);
    Map_CopyCellAttributes(26, 3, 1, 2, v5, 8);
    Map_CopyCells(26, 38, 1, 1, v5, 43);
    Task_Wait(4);
    Map_CopyCells(26, 37, 1, 2, v5, 42);
    Task_Wait(4);
    Map_CopyCells(26, 36, 1, 3, v5, 41);
    Task_Wait(4);
    Map_CopyCells(26, 35, 1, 4, v5, 40);
    Task_Wait(80);
    Event_SetMessage(0x14d3);
    Event_ShowMessageAndWait(0x8009, 0, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_MIA, 2);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    Camera_MoveTo(0xa80000, -1, 0x5a0000, 1);
    Camera_WaitForMove();
    Event_Wait(40);
    Func_02001f34(1);
    Event_Wait(60);
    Audio_PlayCue(21);
    Func_02001f46(4);
    Event_ShowMessageAndWait(0x8009, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 80);
    Event_ShowMessage(0x8009, 0);
    Event_Wait(40);
    Event_ShowMessageAndWait(0x8009, 0, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x8009, 0, 20);
    Func_02001fc6(0);
    Event_Wait(40);
    Event_ShowMessageAndWait(0x8009, 0, 20);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    Event_Wait(60);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 10);
    Event_ShowMessageAndWait(0x8001, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 0);
    Event_ShowMessageAndWait(0x8002, 0, 20);
    Func_0200205e(0);
    Event_Wait(40);
    Event_ShowMessageAndWait(0x8009, 0, 10);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_MIA, 2);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, (v6 << 8), 0);
    ((void (*)())Engine_ActorFaceDirection)(1, (v6 << 8), 0);
    Actor_FaceDirection(ACTOR_IVAN, (v6 << 8), 40);
    Func_020020b2(4);
    Event_OpenMessage(0x8009, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        Actor_ShowEmote(ACTOR_GERALD, 0x103, 20);
        Actor_SetAnimation(ACTOR_GERALD, 4);
        Event_SetMessage(0x14dd);
        Event_ShowMessage(0x8001, 0);
        Actor_ShowEmote(ACTOR_IVAN, 0x103, 10);
        Actor_SetAnimation(ACTOR_IVAN, 3);
        Event_ShowMessage(0x8002, 0);
    }
    Event_Wait(20);
    Func_02002124(4);
    Event_SetMessage(0x14df);
    Event_ShowMessageAndWait(0x8009, 0, 20);
    Event_ShowMessageAndWait(0x8009, 0, 10);
    Func_02002144(0);
    Event_Wait(20);
    ColorBuffer_ApplySource(0x10000, 0);
    ColorBuffer_ApplyTarget(0x406218, 1);
    ColorBuffer_Interpolate(20);
    Task_Wait(40);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_MIA, 2);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Actor_FaceDirection(ACTOR_GERALD, (v6 << 8), 0);
    Actor_FaceDirection(ACTOR_IVAN, (v6 << 8), 20);
    Event_Wait(20);
    *(s32 *)Data_0200adcc = 0;
    {
        s32 *bank = (s32 *)Data_0200adc0;
        bank[0] = 0xa80000;
        bank[1] = 0x200000;
        base5_200962d = (s32)Func_0200962d;
        bank[2] = 0x340000;
    }
    Call2(Func_02002806, base5_200962d, 0xc80);
    Event_Wait(220);
    Func_0200281a(base5_200962d);
    ColorBuffer_ApplyTarget(0x10000, 1);
    ColorBuffer_Interpolate(20);
    Task_Wait(40);
    Func_0200222a(4);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x8009, 0, 10);
    Func_02002240(0);
    Event_ShowMessage(0x8009, 0);
    Call2(Func_0200293c, 8, 0x2009e54);
    Event_Wait(40);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 60);
    Event_ShowMessage(0x8001, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x102, 10);
    Event_ShowMessage(0x8002, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_ShowMessageAndWait(0x8001, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Event_ShowMessageAndWait(0x8002, 0, 10);
    if (rec != 0) {
        Actor_RunRepeatedMotion(ACTOR_MIA, 1);
        Call3(Func_02002a20_scene_primary_script, 0x8003, 0, 10);
    }
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    base5_2009ec8 = (s32)Data_02009ec8;
    Actor_EnableActionCallback(ACTOR_GERALD, base5_2009ec8);
    if (rec != 0) {
        Actor_EnableActionCallback(ACTOR_MIA, base5_2009ec8);
    }
    Func_02002a06(2, base5_2009ec8);
    Event_Wait(20);
    *(u8 *)(Func_020029fa(0) + 35) |= 1;
    GameFlag_Set(0x844);
    Value2(Func_02002944, 0x2009245, 0xc80);
    Event_End();
}

void SceneState_ApplyRectsByFlag844(s32 flag)
{
    if (flag != 0 && GameFlag_IsSet(0x109) == 0)
        FieldScene_RunFourActorEncounter();

    Task_Wait(1);
    if (GameFlag_IsSet(0x844) != 0) {
        s32 w1 = 10;
        Map_CopyCells(121, 34, 3, 1, 93, w1);
        {
            s32 w2 = 30;
            Map_CopyCells(46, 38, 1, 1, w2, 43);
            Map_CopyCellAttributes(0, 0, 1, 2, w2, 9);
        }
        Map_CopyCellAttributes(26, 3, 1, 2, w1, 8);
        Map_CopyCells(26, 35, 1, 4, w1, 40);
    } else {
        s32 w1 = 10;
        s32 w2 = 8;
        Map_CopyCellAttributes(11, 8, 1, 2, w1, w2);
    }
}

void SceneEffect_RegisterPaletteFadeCallback(void)
{
    Func_02003282(Func_02009244);
}

/*
 * The decay of the Z velocity stays a signed divide by sixteen: that shape is
 * what reproduces the negative bias and arithmetic shift in the reference.
 */
void Func_0200185c(struct Effect *effect)
{
    s32 velocity_z;
    struct Sprite *sprite;
    s32 velocity_x;

    /* This block orders the Z load after the Y store; do not flatten it. */
    do {
        velocity_x = effect->velocity_x;
        effect->position[0] += velocity_x;
        effect->position[1] += effect->velocity_y;
    } while (0);
    velocity_z = effect->velocity_z;
    effect->position[2] += velocity_z;

    effect->velocity_x = velocity_x - Math_Divide(velocity_x, 18);
    effect->velocity_z = velocity_z - velocity_z / 16;

    effect->accum18 += effect->rate30;
    effect->accum1c += effect->rate34;

    sprite = effect->sprite;
    sprite->angle += effect->step64;
}
