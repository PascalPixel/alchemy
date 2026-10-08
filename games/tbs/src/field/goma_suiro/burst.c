#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "goma.h"
#include "call.h"
#include "fixed_math.h"

void SceneEffect_RotateRecord(EffectWork_020004c4 *work)
{
    EffectRecord_020004c4 *record = work->record;

    record->angle -= 0x800;
}

void SceneActor_WaitObjectBelowHeight(u8 *object, s32 height)
{
    s32 frames = 60;

    while (frames != 0) {
        Engine_TaskWait(1);
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

union SceneActor *Object_GetById(s32);
void Engine_ActorSetSpeed();
void Engine_ActorSetDestination();
void Engine_ActorWaitForMove(s32);
void Engine_AudioPlayCue(s32);
void Engine_WorkSetValuesIfNonNegative();
void Effect_Spawn();
void Engine_ActorSetAnimation(s32, s32);

void SceneEffect_RunActorBurst(s32 no)
{
    union SceneActor *work;
    u32 cnt;
    struct Vector vec;
    union {
        struct ConfiguredEffectOptions fields;
        u8 bytes[sizeof(struct ConfiguredEffectOptions)];
    } opt;

    work = Object_GetById(no);
    work->fields.mode = 0;
    for (cnt = 0; cnt < 18; cnt++) {
        Engine_TaskWait(1);
        work->fields.sprite->angle -= 256;
        work->fields.x -= Trig_Cos(work->fields.sprite->angle) / 2;
        work->fields.field_38 = 0x80000000;
    }
    work->fields.callback = (u32)SceneEffect_RotateRecord;
    Call3(Engine_ActorSetSpeed, no, 0x30000, 0x18000);
    Call3(Engine_ActorSetDestination, no, 376, 288);
    work->fields.velocity_y = 0xcccc;
    work->fields.mode = 3;
    work->fields.field_22 = 0;
    Engine_ActorWaitForMove(no);
    SceneActor_WaitObjectBelowHeight(work, 0);
    Engine_AudioPlayCue(188);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x50000, 0x50000, 0x10000);
    Engine_AudioPlayCue(141);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
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
    Call3(Engine_ActorSetDestination, no, 346, 292);
    Engine_ActorWaitForMove(no);
    SceneActor_WaitObjectBelowHeight(work, 0);
    work->fields.callback = 0;
    work->fields.sprite->angle = 0x1000;
    opt.fields.kind = 214;
    opt.fields.accum18 = 0x8000;
    opt.fields.accum1c = 0xcccc;
    opt.fields.target30 = 0x18000;
    opt.fields.target34 = 0x13333;
    Effect_Spawn(work->fields.x, work->fields.y, work->fields.z, 0, 0, 0, 0x1c0000, &opt.fields);
    Engine_AudioPlayCue(154);
    Engine_ActorSetAnimation(no, 3);
    Map_WaitWorkValuesBelow256();
}
