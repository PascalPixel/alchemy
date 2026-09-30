#include "GOMA.H"
#include "CALL.H"

void SceneEffect_RotateRecord(EffectWork_020004c4 *work)
{
    EffectRecord_020004c4 *record = work->record;

    record->angle -= 0x800;
}

void SceneActor_WaitObjectBelowHeight(u8 *object, s32 height)
{
    s32 frames = 60;

    while (frames != 0) {
        WaitFrames(1);
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

union SceneActor *Engine_ActorGet(s32);
void WaitFrames(s32);
s32 Trig_Cos(s32);
s32 Trig_Sin(s32);
void ObjectMotion_SetSpeedParameters();
void ObjectMotion_ResetAndSetPosition();
void ObjectMotion_CommitCurrentPositionAndActivate(s32);
void Audio_PlayCue(s32);
void Runtime_SetWorkTripleIfNonNegative();
void Effect_Spawn();
void Object_SetModeById(s32, s32);
void Map_WaitWorkValuesBelow256(void);

/* Mixed object and option views preserve the reference's alias ordering. */
void SceneEffect_RunActorBurst(s32 no)
{
    union SceneActor *work;
    u32 cnt;
    struct Vector vec;
    union {
        struct ConfiguredEffectOptions fields;
        u8 bytes[sizeof(struct ConfiguredEffectOptions)];
    } opt;

    work = Engine_ActorGet(no);
    work->fields.mode = 0;
    for (cnt = 0; cnt < 18; cnt++) {
        WaitFrames(1);
        work->fields.sprite->angle -= 256;
        work->fields.x -= Trig_Cos(work->fields.sprite->angle) / 2;
        work->fields.field_38 = 0x80000000;
    }
    work->fields.callback = (u32)SceneEffect_RotateRecord;
    Call3(ObjectMotion_SetSpeedParameters, no, 0x30000, 0x18000);
    Call3(ObjectMotion_ResetAndSetPosition, no, 376, 288);
    work->fields.velocity_y = 0xcccc;
    work->fields.mode = 3;
    work->fields.field_22 = 0;
    ObjectMotion_CommitCurrentPositionAndActivate(no);
    SceneActor_WaitObjectBelowHeight(work, 0);
    Audio_PlayCue(188);
    Call3(Runtime_SetWorkTripleIfNonNegative, 0x50000, 0x50000, 0x10000);
    Audio_PlayCue(141);
    Call3(Runtime_SetWorkTripleIfNonNegative, -1, -1, 0xe666);
    for (cnt = 0; cnt < 17; cnt++) {
        vec.x = Trig_Cos(cnt << 12);
        vec.y = 0;
        vec.z = Trig_Sin(cnt << 12);
        vec.x -= vec.x / 4;
        vec.z -= vec.z / 2;
        Effect_Spawn(work->fields.x, work->fields.y, work->fields.z,
                     vec.x, vec.y, vec.z, 0, NULL);
    }
    work->fields.field_28 = 0x50000;
    Call3(ObjectMotion_ResetAndSetPosition, no, 346, 292);
    ObjectMotion_CommitCurrentPositionAndActivate(no);
    SceneActor_WaitObjectBelowHeight(work, 0);
    work->fields.callback = 0;
    work->fields.sprite->angle = 0x1000;
    opt.fields.kind = 214;
    opt.fields.accum18 = 0x8000;
    opt.fields.accum1c = 0xcccc;
    opt.fields.target30 = 0x18000;
    opt.fields.target34 = 0x13333;
    Effect_Spawn(work->fields.x, work->fields.y, work->fields.z, 0, 0, 0, 0x1c0000, &opt.fields);
    Audio_PlayCue(154);
    Object_SetModeById(no, 3);
    Map_WaitWorkValuesBelow256();
}
