/* 2026-09-29 alchemy permute: score 9221 to 6210 on the permuter's scorer
   (0 is exact); remaining 92 register-only, 5 stack-only, 41 operand, 16
   reordered, 24 inserted, 15 deleted. Kept rewrites: 18x reorder
   independent statements, 17x introduce a temporary, 16x swap commutative
   operands, 14x reorder local declarations, 14x pointer arithmetic or
   indexing, 12x move an assignment into or out of a condition, 12x test
   truth or compare with zero, 10x split or join a compound assignment, 9x
   remove a temporary, 8x toggle register, 7x drop a same-width cast, 6x
   add a same-width cast. FAKEMATCH: the permuter's temporaries, register
   hints and swapped operand orders below only steer allocation and
   scheduling; no programmer would write them, so they stay tagged until a
   natural spelling replaces them. */
/* Draft main:08097c3c, whole 836 bytes including alignment and 12 pool words.
 * Hypothesis 3: paired coordinate buffers, independent scalar ownership,
 * shared lift magnitude, and linked-parent-success fallthrough. Candidate
 * 852 bytes versus 836, 405 differing halfwords / 298 edits; topology equal.
 * Both coordinate slots are correct, but the compiler keeps their aggregate
 * base at nearby rather than the reference's pos pointer. Scalar spill slots
 * still differ, actor is held high with extra moves, and -lift is negated from
 * an immediate magnitude instead of loading the twelfth pool word fff00000.
 * The corrected terrain call has explicit x/z arguments. Three structural
 * hypotheses exhausted; this remains not-yet-C, with no adoption claimed.
 * Hypotheses 1 and 2 and the original draft remain in branch history.
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

struct PreviewPositions {
    s32 nearby[3];
    s32 pos[3];
};

void Func_08097c3c(void)
{
    struct PreviewPositions positions;
    s32 lift;
    struct PreviewWork *work;
    union PreviewObject *actor;
    union PreviewObject *parent;
    union PreviewObject *preview;
    s32 moved_parent;
    u8 *flags;
    union PreviewObject *hit;
    s32 saved_x;
    s32 angle;
    register s32 keys;
    s32 parent_x;
    u32 direction;
    s32 rate;
    register s32 saved_z;
    s32 x;
    s32 parent_z;
    register s32 z;
    u8 *tmp5;
    s32 tmp7;
    s32 *tmp4;
    s32 *tmp6;
    struct ObjectRuntime *tmp9;
    register u8 *tmp;
    s32 *tmp2;

    work = Data_03001f30;
    parent = work->parent;
    actor = work->actor;
    lift = 0x100000;
    angle = work->direction + 0x8000;
    moved_parent = 0;
    if (actor == 0)
        return;
    BattleEffect_InitializeSharedScene();
    parent->object.linked_object = &actor->object;
    ObjectDispatch_InitializeFar(parent, Data_0809f0bc);
    preview = BattleFx_StartItemBreak(parent);
    if (!(preview != 0)) {
        BattleFx_PrepareBufferInterpolation();
        return;
    }
    tmp9 = &actor->object;
    preview->object.linked_object = tmp9;
    positions.pos[0] = actor->object.x;
    positions.pos[1] = actor->object.y + lift;
    positions.pos[2] = actor->object.z;
    Vector_AddPolarOffset(lift, angle, positions.pos);
    tmp4 = positions.pos;
    tmp6 = positions.pos;
    Object_SetPosition(preview, *positions.pos, tmp6[1], *(tmp4 + 2));
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
    /* FAKEMATCH: retain the observed input word ignored by the callee. */
    direction = BattleFx_GetCycledTableWord(Data_03001ae8);
    if ((s32)&Value_0000ffff == direction) {
        s32 *tmp8;
        positions.pos[0] = actor->object.x;
        *(positions.pos + 1) = actor->object.y + lift;
        positions.pos[2] = actor->object.z;
        Vector_AddPolarOffset(lift, angle, positions.pos);
        tmp8 = positions.pos;
        Object_SetPosition(preview, tmp8[0], positions.pos[1], positions.pos[2]);
        Object_SetMode(preview, 1);
        preview->object.velocity_x = keys;
        preview->object.velocity_y = keys;
        preview->object.velocity_z = keys;
        goto wait;
    }
    positions.pos[0] = actor->object.x;
    positions.pos[1] = actor->object.y + lift;
    positions.pos[2] = actor->object.z;
    Vector_AddPolarOffset(lift, angle, positions.pos);
    Vector_AddPolarOffset(0x20000, direction, positions.pos);
    Object_SetPosition(preview, positions.pos[0], positions.pos[1], positions.pos[2]);
    Object_CommitPosition(preview);
    *((u32)0 + positions.pos) = actor->object.x;
    ((s32 *)positions.pos)[1] = (*actor).object.y;
    ((s32 *)positions.pos)[2] = actor->object.z;
    Vector_AddPolarOffset(lift, direction, positions.pos);
    positions.nearby[0] = actor->object.x;
    positions.nearby[1] = actor->object.y;
    positions.nearby[2] = actor->object.z;
    Vector_AddPolarOffset(0x200000, direction, positions.nearby);
    if (Object_CheckMovementCollision(actor, positions.pos) > 0)
        goto blocked;
    if ((hit = Func_080092a0(actor, positions.pos)) == 0)
        goto move;
    tmp7 = hit != parent;
    if (tmp7)
        goto blocked;
    parent_x = parent->object.x & -lift;
    parent_z = -lift & parent->object.z;
    if (parent_x == (positions.pos[0] & -lift) && parent_z == (*(positions.pos + 2) & -lift))
        goto blocked;
    z = positions.nearby[2];
    if (parent_x != (-lift & (x = *positions.nearby)) || parent_z != (z & -lift))
        goto move;
    if (Func_080092a8(parent->object.terrain_id, x, z) == 0)
        goto move_parent;
blocked:
    Object_SetMode(preview, 4);
    if ((Data_03001e40 & 15) == 0)
        Audio_PlayCue(114);
    goto wait;
move_parent:
    moved_parent = 1;
move:
    Audio_PlayCue(175);
    saved_z = ((s32 *)positions.pos)[2];
    tmp = &Value_00003333;
    tmp5 = tmp;
    saved_x = positions.pos[0];
    Object_SetMode(preview, Data_0809f118[(u16)(angle - direction) >> 14]);
    WaitFrames(15);
    rate = (s32)tmp5;
    actor->object.movement_state = 0;
    actor->object.speed_limit = rate;
    actor[0].object.acceleration = rate;
    Object_SetPosition(actor, positions.pos[0], positions.pos[1], ((s32 *)positions.pos)[2]);
    *flags = 0;
    preview->object.speed_limit = rate;
    preview->object.acceleration = rate;
    tmp2 = positions.pos;
    Vector_AddPolarOffset(lift, direction, tmp2);
    Object_SetPosition(preview, positions.pos[0], positions.pos[1] + lift, positions.pos[2]);
    if (1 == moved_parent) {
        s32 tmp3;
        hit = Object_GetById(work->parent_id);
        hit->object.action_flags &= 254;
        parent->object.speed_limit = rate;
        parent->object.acceleration = rate;
        tmp3 = *(positions.nearby + 1);
        Object_SetPosition(parent, positions.nearby[0], tmp3, positions.nearby[2]);
    }
    Object_CommitPosition(actor);
    actor->object.x = saved_x;
    actor->object.z = saved_z;
    actor->object.velocity_x = 0;
    actor->object.velocity_z = 0;
    goto finish;
wait:
    WaitFrames(1);
    keys = (s32)&Value_00000303 & Data_03001c94;
    if (0 == keys)
        goto select;
finish:
    Animation_ApplyChildValuesFar(actor, work->saved_animation);
    ObjectDispatch_InitializeFar(actor, work->saved_script);
    actor[0].effect.update = work->saved_update;
    EffectRuntime_StopCurrentObject();
    if (moved_parent == 1) {
        hit = Object_GetById(work->parent_id);
        hit->object.action_flags |= 1;
    }
    BattleFx_PrepareBufferInterpolation();
    Func_080981b0(preview);
}
