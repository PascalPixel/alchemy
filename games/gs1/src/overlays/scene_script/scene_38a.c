#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define OverlayObject_PrepareObject      Func_02000048
#define AcquireOverlayObject      Func_02000736
#define RunOverlayObjectCommand0  Func_0200076c
#define RunOverlayObjectCommand1  Func_02000784
#define RunOverlayObjectCommand14 Func_020007dc
#define CreateOverlayObject Func_0200078e
#define SetOverlayObjectMode Func_020007c8
#define SetOverlayObjectSlot Func_02000838
#define EffectDescriptorTable Data_0200880c
#define GetPartyEffect Func_02000882
#define SpawnEffect Func_02000860
#define SetEffectVariant Func_0200086a
#define SetEffectDescriptor Func_02000884
#define SetEffectMode Func_0200096e
#define ScaleEffectDeltaFromAccumulated Func_02000928
#define ScaleEffectDeltaFromOrigin Func_02000940
#define ScaleEffectVerticalDelta Func_0200094e
#define SetEffectCallbackMode Func_02000984
#define SetEffectCallbackArgument Func_02000994

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

typedef struct {
    u8 pad_to_angle[30];
    u16 angle;
} EffectRecord_020004c4;

typedef struct {
    u8 pad_to_record[80];
    EffectRecord_020004c4 *record;
} EffectWork_020004c4;

extern u8 Value_0000001d;
extern u8 Data_020088d8[];
extern u8 Data_02008818[];
extern u8 Data_02008920[];
extern u8 Data_02008978[];
extern u8 Data_02008948[];
extern u8 Data_020089f0[];
extern u8 Data_02008990[];
extern u8 Data_0000001c[];
extern u8 Data_03001ebc[];

void *Func_02000736(s32, s32, s32, s32);
void Func_0200076c(void *, s32);
void Func_02000784(void *, s32);
void Func_020007dc(void *, s32);
void *Func_0200078e(s32, s32, s32, s32);
void Func_020007c8(void *, s32);
void Func_02000838(void *, s32);
void Func_02000892();
void Func_02000aa6();
s32 Func_02000aac();
void Func_02000aba();
u8 *Func_02000ac8();
void Func_02000ae8();
u8 *Func_02000aec();
void Func_02000b5e();
void Func_02000b64();
s32 Func_02000b66();
u8 *Func_02000b6e();
void Func_02000b88();
u8 *Func_02000b96();
s32 Func_02000ba2();
void Func_02000bb0();
void Func_02000bdc();
void Func_02000b98(s32 frames);

/* Shared 22-byte head leaf proved identical for this overlay family. */

/* Contiguous unnamed leaf-owner run for resource_38a. */

/* Complete four-byte leaf: movs r0,#0 followed by bx lr. */

/* Complete eight-byte literal-address getter, including its sole pool word. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* Wait at most sixty frames for the object to reach the requested height. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_02000240[];

    f(a0, a1, a2);
}

void SceneEffect_SetRecordMode(struct EffectWork *work, s32 mode)
{
    work->record->mode = mode;
}

void *OverlayObject_PrepareObject(s32 first, s32 second, s32 third, s32 fourth) {
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
    u8 *obj = CreateOverlayObject(arg3, arg0, arg1, arg2);

    if (obj != NULL) {
        u8 *rec = *(u8 **)(obj + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = rec[9];
        mask = -mask;
        mask &= flags;
        mask |= 4;
        rec[9] = mask;
        obj[0x55] = 0;
        obj[0x59] = 8;
        SetOverlayObjectMode(obj, 0);
        SetOverlayObjectSlot(obj, 15);
        obj[0x23] = (obj[0x23] & 0xfe) | 2;
        return obj;
    }
    return NULL;
}

void SceneEffect_SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "configured_effect_spawn_body.inc"
}

s32 SceneData_GetTable8818OrTable88d8(void) {
    extern s16 Data_02000240[];

    if (Data_02000240[224] == (s32)&Value_0000001d) {
        return (s32)Data_020088d8;
    }
    return (s32)Data_02008818;
}

s32 SceneData_ReturnZero(void)
{
    extern s16 Data_02000240[];

    return 0;
}

u8 *SceneData_GetTable8920(void)
{
    extern s16 Data_02000240[];

    return Data_02008920;
}

s32 SceneData_GetTable8948OrTable8978(void) {
    extern s16 Data_02000240[];

    if (Data_02000240[224] == (s32)&Value_0000001d) {
        return (s32)Data_02008978;
    }
    return (s32)Data_02008948;
}

void FieldScene_RunActor8AtCell24Sequence(void)
{
    s32 *record;
    u8 *target;
    s32 value;

    record = (s32 *)Value1(Func_02000aac, 8);
    value = record[2] / 0x100000;
    if (value == 24) {
        Func_02000892(8);
        {
            u8 *record = Func_02000ac8(8);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | 2);
        }
        Call6(Func_02000aa6, 19, 74, 9, 3, 19, 17);
        target = Func_02000aec(8);
        Func_02000aba((s32)target, 0);
        Call1(Func_02000ae8, 0x864);
    }
}

s32 SceneData_GetTable8990OrTable89f0(void) {
    extern s16 Data_02000240[];

    if (Data_02000240[224] == (s32)&Value_0000001d) {
        return (s32)Data_020089f0;
    }
    return (s32)Data_02008990;
}

s32 FieldScene_PlaceActor8OnEntry(void)
{
    extern u8 Data_02000240[];

    u8 *record;
    u8 *base;

    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x204;
    base = Data_02000240;
    if (*(s16 *)(base + 0x1c0) == (s32)Data_0000001c) {
        if (*(s16 *)(base + 0x1c2) == 5) {
            Call1(Func_02000b5e, 0x12f);
        } else {
            {
                u8 *record = Func_02000b6e(8);
                u8 value = *(volatile u8 *)&record[89];

                record[89] = (u8)(value | 16);
            }
            if (Value1(Func_02000b66, 0x864) != 0) {
                Call3(Func_02000bb0, 8, 0x15a0000, 0x1240000);
                record = Func_02000b96(8);
                Func_02000b64((s32)record, 0);
                *(u8 *)(Func_02000ba2(8) + 35) |= 2;
                Func_02000bdc(8, 2);
                Call6(Func_02000b88, 19, 74, 9, 3, 19, 17);
            }
        }
    }
    return 0;
}

void SceneEffect_RotateRecord(EffectWork_020004c4 *work)
{
    EffectRecord_020004c4 *record = work->record;

    record->angle -= 0x800;
}

void SceneActor_WaitObjectBelowHeight(u8 *object, s32 height)
{
    s32 frames = 60;

    while (frames != 0) {
        Func_02000b98(1);
        frames--;
        if (*(s32 *)(object + 12) <= height)
            break;
    }
}
