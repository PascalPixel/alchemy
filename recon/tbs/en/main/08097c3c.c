/* Draft main:08097c3c. Whole extent 836 bytes, including 2-byte alignment
 * and twelve literal-pool words. Typed hypothesis 1: 852-byte candidate,
 * 389 differing halfwords / 276 edits, different block topology. Its frame
 * is 52 bytes, but pos/nearby occupy +28/+40 instead of +40/+28. The input
 * word load is absent at GetCycledTableWord; direction's narrow lifetime
 * retains an extra shifted value. Setup and movement semantics are recovered,
 * including the previously omitted x/z arguments of the terrain query.
 * The generic 940-byte draft (463 differing halfwords) remains in history.
 */
#include "TYPES.H"
#include "SYSTEM.H"
#include "OBJECT_RUNTIME.H"

union PreviewObject {
    struct ObjectRuntime object;
    struct {
        u8 unknown_00[108];
        void (*update)(void *);
    } effect;
};
struct PreviewWork {
    s32 direction;
    u8 unknown_04[12];
    union PreviewObject *parent;
    union PreviewObject *actor;
    s16 parent_id;
    u8 unknown_1a[30];
    void (*saved_update)(void *);
    const void *saved_script;
    u8 unknown_40[4];
    u8 saved_animation;
};
extern struct PreviewWork *Data_03001f30;
extern u32 Data_03001ae8;
extern u32 Data_03001e40;
extern u32 Data_03001c94;
extern const u8 Data_0809f0bc[];
extern const u8 Data_0809f118[];
extern const u8 Value_00006666;
extern const u8 Value_00003333;
extern const u8 Value_0000ffff;
extern const u8 Value_00000303;
void BattleEffect_InitializeSharedScene(void);
void ObjectDispatch_InitializeFar(void *, const void *);
void *BattleFx_StartItemBreak(void *);
void BattleFx_PrepareBufferInterpolation(void);
void Vector_AddPolarOffset(s32, s32, s32 *);
void Object_SetPosition(void *, s32, s32, s32);
void Object_CommitPosition(void *);
void Object_SetMode(void *, s32);
void BattleFx_SnapScaleToFull(void *);
void Animation_ApplyRandomChildValues(void *);
u16 BattleFx_GetCycledTableWord(void);
s32 Object_CheckMovementCollision(void *, s32 *);
void *Func_080092a0(void *, s32 *);
s32 Func_080092a8(s32, s32, s32);
void *Object_GetById(u32);
void Audio_PlayCue(s32);
void Animation_ApplyChildValuesFar(void *, s32);
void EffectRuntime_StopCurrentObject(void);
void Func_080981b0(void *);

void Func_08097c3c(void)
{
    s32 nearby[3];
    s32 pos[3];
    struct PreviewWork *work;
    union PreviewObject *parent;
    union PreviewObject *actor;
    union PreviewObject *preview;
    union PreviewObject *hit;
    u8 *flags;
    s32 moved_parent;
    s32 angle;
    u16 direction;
    s32 keys;
    s32 saved_x;
    s32 saved_z;
    s32 parent_x;
    s32 parent_z;
    s32 rate;
    s32 step;
    s32 x;
    s32 z;

    work = Data_03001f30;
    parent = work->parent;
    actor = work->actor;
    angle = work->direction + 0x8000;
    moved_parent = 0;
    if (actor == 0)
        return;
    BattleEffect_InitializeSharedScene();
    parent->object.linked_object = &actor->object;
    ObjectDispatch_InitializeFar(parent, Data_0809f0bc);
    preview = BattleFx_StartItemBreak(parent);
    if (preview == 0) {
        BattleFx_PrepareBufferInterpolation();
        return;
    }
    preview->object.linked_object = &actor->object;
    pos[0] = actor->object.x;
    pos[1] = actor->object.y + 0x100000;
    pos[2] = actor->object.z;
    Vector_AddPolarOffset(0x100000, angle, pos);
    Object_SetPosition(preview, pos[0], pos[1], pos[2]);
    BattleFx_SnapScaleToFull(preview);
    preview->object.speed_limit = 0x40000;
    preview->object.acceleration = 0x8000;
    flags = &preview->object.flags;
    *flags = 4;
    actor->effect.update = Animation_ApplyRandomChildValues;
    actor->object.speed_limit = (s32)&Value_00006666;
    actor->object.acceleration = (s32)&Value_00003333;
    actor->object.action_flags = *(u8 *)&moved_parent;
    actor->object.terrain_id = 2;
    goto wait;

select:
    direction = BattleFx_GetCycledTableWord();
    if (direction == (s32)&Value_0000ffff) {
        pos[0] = actor->object.x;
        pos[1] = actor->object.y + 0x100000;
        pos[2] = actor->object.z;
        Vector_AddPolarOffset(0x100000, angle, pos);
        Object_SetPosition(preview, pos[0], pos[1], pos[2]);
        Object_SetMode(preview, 1);
        preview->object.velocity_x = keys;
        preview->object.velocity_y = keys;
        preview->object.velocity_z = keys;
        goto wait;
    }
    pos[0] = actor->object.x;
    pos[1] = actor->object.y + 0x100000;
    pos[2] = actor->object.z;
    Vector_AddPolarOffset(0x100000, angle, pos);
    Vector_AddPolarOffset(0x20000, direction, pos);
    Object_SetPosition(preview, pos[0], pos[1], pos[2]);
    Object_CommitPosition(preview);
    pos[0] = actor->object.x;
    pos[1] = actor->object.y;
    pos[2] = actor->object.z;
    Vector_AddPolarOffset(0x100000, direction, pos);
    nearby[0] = actor->object.x;
    nearby[1] = actor->object.y;
    nearby[2] = actor->object.z;
    Vector_AddPolarOffset(0x200000, direction, nearby);
    if (Object_CheckMovementCollision(actor, pos) > 0)
        goto blocked;
    hit = Func_080092a0(actor, pos);
    if (hit == 0)
        goto move;
    if (hit != parent)
        goto blocked;
    parent_x = parent->object.x & -0x100000;
    parent_z = parent->object.z & -0x100000;
    if (parent_x == (pos[0] & -0x100000)
            && parent_z == (pos[2] & -0x100000))
        goto blocked;
    x = nearby[0];
    z = nearby[2];
    if (parent_x != (x & -0x100000) || parent_z != (z & -0x100000))
        goto move;
    if (Func_080092a8(parent->object.terrain_id, x, z) != 0)
        goto blocked;
    moved_parent = 1;
    goto move;

blocked:
    Object_SetMode(preview, 4);
    if ((Data_03001e40 & 15) == 0)
        Audio_PlayCue(114);
    goto wait;

move:
    Audio_PlayCue(175);
    saved_x = pos[0];
    saved_z = pos[2];
    step = (u16)(angle - direction) >> 14;
    Object_SetMode(preview, Data_0809f118[step]);
    WaitFrames(15);
    actor->object.movement_state = 0;
    rate = (s32)&Value_00003333;
    actor->object.speed_limit = rate;
    actor->object.acceleration = rate;
    Object_SetPosition(actor, pos[0], pos[1], pos[2]);
    *flags = 0;
    preview->object.speed_limit = rate;
    preview->object.acceleration = rate;
    Vector_AddPolarOffset(0x100000, direction, pos);
    Object_SetPosition(preview, pos[0], pos[1] + 0x100000, pos[2]);
    if (moved_parent == 1) {
        hit = Object_GetById(work->parent_id);
        hit->object.action_flags &= 254;
        parent->object.speed_limit = rate;
        parent->object.acceleration = rate;
        Object_SetPosition(parent, nearby[0], nearby[1], nearby[2]);
    }
    Object_CommitPosition(actor);
    actor->object.x = saved_x;
    actor->object.z = saved_z;
    actor->object.velocity_x = 0;
    actor->object.velocity_z = 0;
    goto finish;

wait:
    WaitFrames(1);
    keys = Data_03001c94 & (s32)&Value_00000303;
    if (keys == 0)
        goto select;
finish:
    Animation_ApplyChildValuesFar(actor, work->saved_animation);
    ObjectDispatch_InitializeFar(actor, work->saved_script);
    actor->effect.update = work->saved_update;
    EffectRuntime_StopCurrentObject();
    if (moved_parent == 1) {
        hit = Object_GetById(work->parent_id);
        hit->object.action_flags |= 1;
    }
    BattleFx_PrepareBufferInterpolation();
    Func_080981b0(preview);
}
