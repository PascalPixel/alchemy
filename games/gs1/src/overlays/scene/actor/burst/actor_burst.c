#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define OvObj_PrepareObject      Func_02000048
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

void Effect_SetRecordMode(struct EffectWork *work, s32 mode)
{
    work->record->mode = mode;
}

void *OvObj_PrepareObject(s32 first, s32 second, s32 third, s32 fourth)
{
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

void *OvObj_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
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

#define Effect_UpdateMotion Func_02000104

/* The object is accessed through word fields and a linked record. Keep the
 * shared storage view so the record load follows the position stores. */
union MotionWork {
    struct {
        u32 unk_00[2];
        s32 x, y, z;
        u32 unk_14;
        s32 accum_x, accum_y;
        u32 unk_20[4];
        s32 rate_x, rate_y;
        u32 unk_38[3];
        s32 velocity_x, velocity_y, velocity_z;
        u16 *record;
        u8 unk_54[16];
        u16 angle_step;
    } fields;
    u8 bytes[102];
};

void Effect_UpdateMotion(union MotionWork *work)
{
    u16 *record;

    work->fields.x += work->fields.velocity_x;
    work->fields.y += work->fields.velocity_y;
    work->fields.z += work->fields.velocity_z;
    work->fields.accum_x += work->fields.rate_x;
    work->fields.accum_y += work->fields.rate_y;
    record = work->fields.record;
    record[15] += work->fields.angle_step;
}

void Effect_SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "configured_effect_spawn_body.inc"
}

s32 SceneData_GetTable8818OrTable88d8(void)
{
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

s32 SceneData_GetTable8948OrTable8978(void)
{
    extern s16 Data_02000240[];

    if (Data_02000240[224] == (s32)&Value_0000001d) {
        return (s32)Data_02008978;
    }
    return (s32)Data_02008948;
}

void Scene_RunActor8AtCell24Sequence(void)
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

s32 SceneData_GetTable8990OrTable89f0(void)
{
    extern s16 Data_02000240[];

    if (Data_02000240[224] == (s32)&Value_0000001d) {
        return (s32)Data_020089f0;
    }
    return (s32)Data_02008990;
}

s32 Scene_PlaceActor8OnEntry(void)
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

void Effect_RotateRecord(EffectWork_020004c4 *work)
{
    EffectRecord_020004c4 *record = work->record;

    record->angle -= 0x800;
}

void Actor_WaitObjectBelowHeight(u8 *object, s32 height)
{
    s32 frames = 60;

    while (frames != 0) {
        Func_02000b98(1);
        frames--;
        if (*(s32 *)(object + 12) <= height)
            break;
    }
}

union SceneActor {
    struct {
        u32 unk_00[2];
        s32 x, y, z;
        u32 unk_14[3];
        u8 unk_20[2];
        u8 field_22;
        u8 unk_23[5];
        s32 field_28;
        u32 unk_2c[3];
        u32 field_38;
        u32 unk_3c[3];
        s32 velocity_y;
        u32 unk_4c;
        struct Sprite *sprite;
        u8 unk_54;
        u8 mode;
        u8 unk_56[22];
        u32 callback;
    } fields;
    u8 bytes[112];
};
struct Vector { s32 x, y, z; };

union SceneActor *Func_02000c2c(s32);
void Func_02000bce(s32);
s32 Func_02000bf0(s32);
s32 Func_02000c8c(s32);
void Actor_SetSpeed();
void Func_02000c9c();
void Func_02000cba(s32);
void Func_02000a5a(union SceneActor *, s32);
void Func_02000ce8(s32);
void Func_02000c96();
void Func_02000cfc(s32);
void Func_02000caa();
void Func_02000740();
void Func_02000d56();
void Func_02000d64(s32);
void Func_02000b04(union SceneActor *, s32);
void Func_020007ac();
void Func_02000dd2(s32);
void Func_02000dca(s32, s32);
void Func_02000d86(void);

#define Effect_RunActorBurst Func_020004f4

/* Mixed object and option views preserve the reference's alias ordering. */
void Effect_RunActorBurst(s32 no)
{
    union SceneActor *work;
    u32 cnt;
    struct Vector vec;
    union {
        struct ConfiguredEffectOptions fields;
        u8 bytes[sizeof(struct ConfiguredEffectOptions)];
    } opt;

    work = Func_02000c2c(no);
    work->fields.mode = 0;
    for (cnt = 0; cnt < 18; cnt++) {
        Func_02000bce(1);
        work->fields.sprite->angle -= 256;
        work->fields.x -= Func_02000bf0(work->fields.sprite->angle) / 2;
        work->fields.field_38 = 0x80000000;
    }
    work->fields.callback = 0x020084c5;
    Call3(Actor_SetSpeed, no, 0x30000, 0x18000);
    Call3(Func_02000c9c, no, 376, 288);
    work->fields.velocity_y = 0xcccc;
    work->fields.mode = 3;
    work->fields.field_22 = 0;
    Func_02000cba(no);
    Func_02000a5a(work, 0);
    Func_02000ce8(188);
    Call3(Func_02000c96, 0x50000, 0x50000, 0x10000);
    Func_02000cfc(141);
    Call3(Func_02000caa, -1, -1, 0xe666);
    for (cnt = 0; cnt < 17; cnt++) {
        vec.x = Func_02000bf0(cnt << 12);
        vec.y = 0;
        vec.z = Func_02000c8c(cnt << 12);
        vec.x -= vec.x / 4;
        vec.z -= vec.z / 2;
        Func_02000740(work->fields.x, work->fields.y, work->fields.z,
                     vec.x, vec.y, vec.z, 0, NULL);
    }
    work->fields.field_28 = 0x50000;
    Call3(Func_02000d56, no, 346, 292);
    Func_02000d64(no);
    Func_02000b04(work, 0);
    work->fields.callback = 0;
    work->fields.sprite->angle = 0x1000;
    opt.fields.kind = 214;
    opt.fields.accum18 = 0x8000;
    opt.fields.accum1c = 0xcccc;
    opt.fields.target30 = 0x18000;
    opt.fields.target34 = 0x13333;
    Func_020007ac(work->fields.x, work->fields.y, work->fields.z, 0, 0, 0, 0x1c0000, &opt.fields);
    Func_02000dd2(154);
    Func_02000dca(no, 3);
    Func_02000d86();
}
