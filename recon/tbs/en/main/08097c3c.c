/* Draft main:08097c3c, whole 836 bytes including alignment and 12 pool words.
 * Hypothesis 2: aggregate local-work ownership and full-width direction.
 * Candidate 844 bytes versus 836, 387 differing halfwords / 294 edits.
 * Restores twelve pool values and full-width direction conversion, but the
 * escaped aggregate keeps a persistent frame-base register and accesses its
 * fields through that base, unlike the reference's independent stack slots.
 * Collision success also branches across the blocked path rather than
 * falling through from the linked-parent flag into movement. No exact match.
 * Hypothesis 1 and the original generic draft remain in history.
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
u16 BattleFx_GetCycledTableWord(u32 ignored_input);
s32 Object_CheckMovementCollision(void *, s32 *);
void *Func_080092a0(void *, s32 *);
s32 Func_080092a8(s32, s32, s32);
void *Object_GetById(u32);
void Audio_PlayCue(s32);
void Animation_ApplyChildValuesFar(void *, s32);
void EffectRuntime_StopCurrentObject(void);
void Func_080981b0(void *);

struct PreviewLocals {
    u8 *flags;
    s32 moved_parent;
    s32 angle;
    s32 saved_z;
    s32 saved_x;
    union PreviewObject *parent;
    struct PreviewWork *work;
    s32 nearby[3];
    s32 pos[3];
};

void Func_08097c3c(void)
{
    struct PreviewLocals frame;
    union PreviewObject *actor;
    union PreviewObject *preview;
    union PreviewObject *hit;
    u32 direction;
    s32 keys;
    s32 parent_x;
    s32 parent_z;
    s32 rate;
    s32 step;
    s32 x;
    s32 z;

    frame.work = Data_03001f30;
    frame.parent = frame.work->parent;
    actor = frame.work->actor;
    frame.angle = frame.work->direction + 0x8000;
    frame.moved_parent = 0;
    if (actor == 0)
        return;
    BattleEffect_InitializeSharedScene();
    frame.parent->object.linked_object = &actor->object;
    ObjectDispatch_InitializeFar(frame.parent, Data_0809f0bc);
    preview = BattleFx_StartItemBreak(frame.parent);
    if (preview == 0) {
        BattleFx_PrepareBufferInterpolation();
        return;
    }
    preview->object.linked_object = &actor->object;
    frame.pos[0] = actor->object.x;
    frame.pos[1] = actor->object.y + 0x100000;
    frame.pos[2] = actor->object.z;
    Vector_AddPolarOffset(0x100000, frame.angle, frame.pos);
    Object_SetPosition(preview, frame.pos[0], frame.pos[1], frame.pos[2]);
    BattleFx_SnapScaleToFull(preview);
    preview->object.speed_limit = 0x40000;
    preview->object.acceleration = 0x8000;
    frame.flags = &preview->object.flags;
    *frame.flags = 4;
    actor->effect.update = Animation_ApplyRandomChildValues;
    actor->object.speed_limit = (s32)&Value_00006666;
    actor->object.acceleration = (s32)&Value_00003333;
    actor->object.action_flags = *(u8 *)&frame.moved_parent;
    actor->object.terrain_id = 2;
    goto wait;

select:
    /* FAKEMATCH: retain the call site input word although the callee ignores it. */
    direction = (u16)BattleFx_GetCycledTableWord(Data_03001ae8);
    if (direction == (s32)&Value_0000ffff) {
        frame.pos[0] = actor->object.x;
        frame.pos[1] = actor->object.y + 0x100000;
        frame.pos[2] = actor->object.z;
        Vector_AddPolarOffset(0x100000, frame.angle, frame.pos);
        Object_SetPosition(preview, frame.pos[0], frame.pos[1], frame.pos[2]);
        Object_SetMode(preview, 1);
        preview->object.velocity_x = keys;
        preview->object.velocity_y = keys;
        preview->object.velocity_z = keys;
        goto wait;
    }
    frame.pos[0] = actor->object.x;
    frame.pos[1] = actor->object.y + 0x100000;
    frame.pos[2] = actor->object.z;
    Vector_AddPolarOffset(0x100000, frame.angle, frame.pos);
    Vector_AddPolarOffset(0x20000, direction, frame.pos);
    Object_SetPosition(preview, frame.pos[0], frame.pos[1], frame.pos[2]);
    Object_CommitPosition(preview);
    frame.pos[0] = actor->object.x;
    frame.pos[1] = actor->object.y;
    frame.pos[2] = actor->object.z;
    Vector_AddPolarOffset(0x100000, direction, frame.pos);
    frame.nearby[0] = actor->object.x;
    frame.nearby[1] = actor->object.y;
    frame.nearby[2] = actor->object.z;
    Vector_AddPolarOffset(0x200000, direction, frame.nearby);
    if (Object_CheckMovementCollision(actor, frame.pos) > 0)
        goto blocked;
    hit = Func_080092a0(actor, frame.pos);
    if (hit == 0)
        goto move;
    if (hit != frame.parent)
        goto blocked;
    parent_x = frame.parent->object.x & -0x100000;
    parent_z = frame.parent->object.z & -0x100000;
    if (parent_x == (frame.pos[0] & -0x100000)
            && parent_z == (frame.pos[2] & -0x100000))
        goto blocked;
    x = frame.nearby[0];
    z = frame.nearby[2];
    if (parent_x != (x & -0x100000) || parent_z != (z & -0x100000))
        goto move;
    if (Func_080092a8(frame.parent->object.terrain_id, x, z) != 0)
        goto blocked;
    frame.moved_parent = 1;
    goto move;

blocked:
    Object_SetMode(preview, 4);
    if ((Data_03001e40 & 15) == 0)
        Audio_PlayCue(114);
    goto wait;

move:
    Audio_PlayCue(175);
    frame.saved_x = frame.pos[0];
    frame.saved_z = frame.pos[2];
    step = (u16)(frame.angle - direction) >> 14;
    Object_SetMode(preview, Data_0809f118[step]);
    WaitFrames(15);
    actor->object.movement_state = 0;
    rate = (s32)&Value_00003333;
    actor->object.speed_limit = rate;
    actor->object.acceleration = rate;
    Object_SetPosition(actor, frame.pos[0], frame.pos[1], frame.pos[2]);
    *frame.flags = 0;
    preview->object.speed_limit = rate;
    preview->object.acceleration = rate;
    Vector_AddPolarOffset(0x100000, direction, frame.pos);
    Object_SetPosition(preview, frame.pos[0], frame.pos[1] + 0x100000, frame.pos[2]);
    if (frame.moved_parent == 1) {
        hit = Object_GetById(frame.work->parent_id);
        hit->object.action_flags &= 254;
        frame.parent->object.speed_limit = rate;
        frame.parent->object.acceleration = rate;
        Object_SetPosition(frame.parent, frame.nearby[0], frame.nearby[1], frame.nearby[2]);
    }
    Object_CommitPosition(actor);
    actor->object.x = frame.saved_x;
    actor->object.z = frame.saved_z;
    actor->object.velocity_x = 0;
    actor->object.velocity_z = 0;
    goto finish;

wait:
    WaitFrames(1);
    keys = Data_03001c94 & (s32)&Value_00000303;
    if (keys == 0)
        goto select;
finish:
    Animation_ApplyChildValuesFar(actor, frame.work->saved_animation);
    ObjectDispatch_InitializeFar(actor, frame.work->saved_script);
    actor->effect.update = frame.work->saved_update;
    EffectRuntime_StopCurrentObject();
    if (frame.moved_parent == 1) {
        hit = Object_GetById(frame.work->parent_id);
        hit->object.action_flags |= 1;
    }
    BattleFx_PrepareBufferInterpolation();
    Func_080981b0(preview);
}
