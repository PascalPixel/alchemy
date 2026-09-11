#include "types.h"
#include "scene.h"

#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

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
} EffectRecord;

typedef struct {
    u8 pad_to_record[80];
    EffectRecord *record;
} EffectWork;

extern u8 Value_0000001d;
extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gOv6[];
extern u8 gOv7[];
extern u8 gVal[];
extern u8 gWork[];

void *AcquireOverlayObject(s32, s32, s32, s32);

void *CreateOverlayObject(s32, s32, s32, s32);

u8 *Actor_Run();

u8 *Actor_unk2_4();

u8 *Actor_unk3_4();

u8 *Actor_unk4_4();

/* Shared 22-byte head leaf proved identical for this overlay family. */

/* Contiguous unnamed leaf-owner run for resource_38a. */

/* Complete four-byte leaf: movs r0,#0 followed by bx lr. */

/* Complete eight-byte literal-address getter, including its sole pool word. */

/* Wait at most sixty frames for the object to reach the requested height. */

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
    extern s16 gCell[];

    if (gCell[224] == (s32)&Value_0000001d) {
        return (s32)gOv;
    }
    return (s32)gOv2;
}

s32 SceneData_ReturnZero(void)
{
    extern s16 gCell[];

    return 0;
}

u8 *SceneData_GetTable8920(void)
{
    extern s16 gCell[];

    return gOv3;
}

s32 SceneData_GetTable8948OrTable8978(void)
{
    extern s16 gCell[];

    if (gCell[224] == (s32)&Value_0000001d) {
        return (s32)gOv4;
    }
    return (s32)gOv5;
}

void Scene_RunActor8AtCell24Sequence(void)
{
    s32 *record;
    u8 *target;
    s32 value;

    record = (s32 *)Actor_Check(8);
    value = record[2] / 0x100000;
    if (value == 24) {
        Actor_unk5_4(8);
        {
            u8 *record = Actor_Run(8);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | 2);
        }
        Actor_SetRect(19, 74, 9, 3, 19, 17);
        target = Actor_unk2_4(8);
        Actor_unk6_4((s32)target, 0);
        Actor_Do(0x864);
    }
}

s32 SceneData_GetTable8990OrTable89f0(void)
{
    extern s16 gCell[];

    if (gCell[224] == (s32)&Value_0000001d) {
        return (s32)gOv6;
    }
    return (s32)gOv7;
}

s32 Scene_PlaceActor8OnEntry(void)
{
    extern u8 gCell[];

    u8 *record;
    u8 *base;

    *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x204;
    base = gCell;
    if (*(s16 *)(base + 0x1c0) == (s32)gVal) {
        if (*(s16 *)(base + 0x1c2) == 5) {
            Actor_unk2_2(0x12f);
        } else {
            {
                u8 *record = Actor_unk3_4(8);
                u8 value = *(volatile u8 *)&record[89];

                record[89] = (u8)(value | 16);
            }
            if (Actor_unk2(0x864) != 0) {
                Actor_Place(8, 0x15a0000, 0x1240000);
                record = Actor_unk4_4(8);
                Actor_unk7_4((s32)record, 0);
                *(u8 *)(Actor_unk3(8) + 35) |= 2;
                Actor_unk8_4(8, 2);
                Actor_unk2_5(19, 74, 9, 3, 19, 17);
            }
        }
    }
    return 0;
}

void Effect_RotateRecord(EffectWork *work)
{
    EffectRecord *record = work->record;

    record->angle -= 0x800;
}

void Actor_WaitObjectBelowHeight(u8 *object, s32 height)
{
    s32 frames = 60;

    while (frames != 0) {
        Actor_unk3_2(1);
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

union SceneActor *Actor_unk9_4(s32);

void Actor_SetSpeed();

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

    work = Actor_unk9_4(no);
    work->fields.mode = 0;
    for (cnt = 0; cnt < 18; cnt++) {
        Actor_unk4_2(1);
        work->fields.sprite->angle -= 256;
        work->fields.x -= Actor_unk4(work->fields.sprite->angle) / 2;
        work->fields.field_38 = 0x80000000;
    }
    work->fields.callback = 0x020084c5;
    Actor_unk2_3(no, 0x30000, 0x18000);
    Actor_unk3_3(no, 376, 288);
    work->fields.velocity_y = 0xcccc;
    work->fields.mode = 3;
    work->fields.field_22 = 0;
    Actor_unk5_2(no);
    Actor_Apply(work, 0);
    Actor_unk6_2(188);
    Actor_unk4_3(0x50000, 0x50000, 0x10000);
    Actor_unk7_2(141);
    Actor_unk5_3(-1, -1, 0xe666);
    for (cnt = 0; cnt < 17; cnt++) {
        vec.x = Actor_unk4(cnt << 12);
        vec.y = 0;
        vec.z = Actor_unk5(cnt << 12);
        vec.x -= vec.x / 4;
        vec.z -= vec.z / 2;
        Actor_unk10_4(work->fields.x, work->fields.y, work->fields.z,
                     vec.x, vec.y, vec.z, 0, NULL);
    }
    work->fields.field_28 = 0x50000;
    Actor_unk6_3(no, 346, 292);
    Actor_unk8_2(no);
    Actor_Apply2(work, 0);
    work->fields.callback = 0;
    work->fields.sprite->angle = 0x1000;
    opt.fields.kind = 214;
    opt.fields.accum18 = 0x8000;
    opt.fields.accum1c = 0xcccc;
    opt.fields.target30 = 0x18000;
    opt.fields.target34 = 0x13333;
    Actor_unk11_4(work->fields.x, work->fields.y, work->fields.z, 0, 0, 0, 0x1c0000, &opt.fields);
    Actor_unk9_2(154);
    Actor_Apply3(no, 3);
    Actor_unk12_4();
}
