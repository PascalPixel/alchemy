#include "TYPES.H"
#include "FIELD_EVENT.H"

void Makyuri_SpawnLightObjects(s32 count, s32 base);
void Makyuri_ClearPalette(void);
void Makyuri_CyclePalette(void);
void Main_0808a408(s32 value);
void Main_0808a5e0(s32 value);
void Main_08015210(s32 message, s32 a1, s32 a2);
void Effect_Spawn(s32 x, s32 y, s32 z, s32 dx, s32 dy, s32 dz, s32 lift, void *params);
void FieldScene_RunScene39b_02001208(void);
void FieldScene_RunScene39b_0200196c(void);
void FieldScene_RunActorExchangeChoreography(void);

struct EffectParams {
    s32 count;
    s32 kind;
    u8 pad08[16];
    u16 tile;
    u16 pad1a;
    s32 script;
    u8 pad20[8];
};

struct Vec3 {
    s32 x;
    s32 y;
    s32 z;
};

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
};

extern union GameStateRows Data_02000240_t;
extern u8 Data_00000039[];

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

/* Mercury Lighthouse entrance: set the blend, lights and palette cycle, record the retreat point, then run the arrival for the entrance (a one-time burst around the leader through the sixth entrance). */
s32 MakyuriIriguchi_ApplyEntryState(void)
{
    struct EffectParams params;
    struct Vec3 vec;
    struct FieldActor *actor;
    u32 i;
    s32 angle;
    s32 flag;

    /* FAKEMATCH: the do/while and the held values keep the blend constants
     * in registers ahead of the frame setup. */
    do {
        s32 blend = 0x3f40;

        *(volatile u16 *)0x04000050 = blend;
    } while (0);
    {
        s32 alpha = 0x1010;

        *(volatile u16 *)0x04000052 = alpha;
    }
    Makyuri_SpawnLightObjects(21, 0x2001000);
    Engine_GameFlagSet(0x111);
    Data_02000240_t.halves[289][0] = 11;
    Data_02000240_t.halves[288][0] = (s32)Data_00000039;
    Main_0808a408(0);
    if (Engine_GameFlagIsSet(0x875)) {
        Engine_TaskAddCallback(Makyuri_CyclePalette, 0xc80);
    } else {
        Makyuri_ClearPalette();
    }
    *(s32 *)(*(s32 *)0x03001ebc + 0x1c0) = 0x204;
    switch (Data_02000240_t.halves[225][0]) {
    case 1:
        if (Engine_GameFlagIsSet(0x872) == 0) {
            Engine_EventRequestExit(20);
        }
    case 2:
        Engine_ActorGet(12)->scale_x = -0x10000;
        Engine_ActorGet(13)->scale_x = -0x10000;
        Engine_ActorGet(14)->scale_x = -0x10000;
        Engine_TaskWait(1);
        break;
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
        if (Engine_GameFlagIsSet(0x875)) {
            Call6(Engine_MapCopyCellAttributes, 84, 5, 10, 7, 20, 5);
            Call6(Engine_MapCopyCellAttributes, 101, 5, 12, 7, 37, 5);
        }
        break;
    case 3:
    case 4:
    case 5:
    case 6:
        Engine_TaskAddCallback(Makyuri_CyclePalette, 0xc80);
        if (Engine_GameFlagIsSet(0x875)) {
            Call6(Engine_MapCopyCellsTo, 37, 98, 10, 97, 5, 3);
            Engine_MapRedraw();
            Engine_TaskWait(1);
            Call6(Engine_MapCopyCellAttributes, 70, 32, 13, 7, 6, 32);
        }
        if (Data_02000240_t.halves[225][0] != 6) {
            break;
        }
        flag = Value1(Engine_GameFlagIsSet, 0x251);
        if (flag != 0) {
            break;
        }
        Call1(Engine_GameFlagSet, 0x251);
        Engine_EventBegin();
        Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
        Engine_MapRedraw();
        Engine_TaskWait(1);
        Engine_ActorGet(0)->y.fixed = 0x820000;
        *(s32 *)&Engine_ActorGet(0)->unknown_44[4] = 0x8000;
        *(s32 *)&Engine_ActorGet(0)->unknown_44[0] = flag;
        Engine_ActorGet(0)->motion_flags = flag;
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        Engine_EventWait(30);
        Engine_ActorGet(0)->motion_flags = 3;
        Engine_AudioPlayCue(204);
        Engine_EventWait(24);
        actor = Engine_ActorGet(0);
        params.kind = 7;
        for (i = 0; i <= 16; i++) {
            angle = i << 12;
            vec.x = Engine_MathCos(angle);
            vec.y = 0;
            vec.z = Engine_MathSin(angle);
            vec.x -= vec.x / 4;
            vec.z -= vec.z / 2;
            Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, vec.x, vec.y, vec.z, 0x10001, &params);
        }
        Engine_AudioPlayCue(188);
        Call2(Engine_ActorSetAttachedEffect, 0, 0x101);
        Engine_ActorSetAnimation(0, 22);
        Call3(Engine_WorkSetValuesIfNonNegative, 0x50000, 0x50000, 0x10000);
        Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
        Engine_MapWaitWorkValuesBelow256();
        Engine_ActorSetAttachedEffect(0, 0x100);
        *(s32 *)&Engine_ActorGet(0)->unknown_44[4] = 0x10000;
        *(s32 *)&Engine_ActorGet(0)->unknown_44[0] = 0x4000;
        if (Engine_GameFlagIsSet(0x875) == 0) {
            Engine_ColorBufferApplySource(0x10000, 0);
            Engine_ColorBufferApplyTarget(0x10003, 1);
            Engine_ColorBufferInterpolate(30);
            Engine_EventWaitForScreen();
            Engine_ActorSetAnimation(0, 1);
            Engine_EventWait(30);
            Main_08015210(0x1632, 0, 0);
            Engine_ColorBufferApplyTarget(0x10000, 0);
            Engine_ColorBufferInterpolate(30);
        }
        Engine_EventEnd();
        break;
    case 18:
    case 19:
    case 20:
        FieldScene_RunScene39b_02001208();
    case 17:
        Main_0808a5e0(170);
        break;
    case 25:
        Engine_ActorSetChildValue(0, 15);
        Engine_ActorSetSpriteFlags(Engine_ActorGet(0), 0);
        Engine_EventBegin();
        Engine_MapRedraw();
        Engine_TaskWait(1);
        *(s32 *)(*(s32 *)0x03001ebc + 0x1c0) = 0x100;
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        Engine_EventWait(120);
        Engine_EventRequestExit(50);
        Engine_EventEnd();
        break;
    case 30:
        if (Engine_GameFlagIsSet(0x109) == 0) {
            FieldScene_RunActorExchangeChoreography();
        } else {
            Call6(Engine_MapCopyCellAttributes, 0, 0, 3, 3, 7, 9);
        }
        break;
    case 31:
        if (Engine_GameFlagIsSet(0x109) == 0) {
            FieldScene_RunScene39b_0200196c();
        }
        break;
    }
    return 0;
}
