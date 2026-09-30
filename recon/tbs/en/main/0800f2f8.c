/*
 * NONMATCHING: 1252 bytes, candidate 1244, 287 differing halfwords.
 * 2026-09-26: audited complete raw owner [0800f2f8,0800f7dc), including
 * its own pools; next whole owner is 0800f7dc, not 0800f7f4. The preceding
 * ebec movement owner returns before this entry. No direct caller was
 * found in the local symbolic listings; do not infer a caller from that.
 * Baseline: 1062 bytes, 616 differing halfwords, 477 aligned edits,
 * different conditional topology. Full normalized difference inspected.
 * H1 reconstructs missing semantics from the complete listing: each
 * alternate heading has five probes, the two IWRAM calls square a Q16
 * component with two operands, successful movement decrements the action
 * counter, and turn smoothing follows both movement arms. Also correct
 * decoded constants, signed narrowing, object +54 ownership and mask.
 * Predict ten static probe pairs and repaired branch content, not exact
 * registers. One whole-flow trial before 23:07; preserve full difference
 * here and commit. Only exact full bytes plus all gates permit adoption.
 * H1 result: 1244 bytes, 610 differing halfwords, 202 aligned edits
 * (baseline 477); full normalized difference inspected. Both five-probe
 * sequences now retain the reference register roles and call order.
 * Frame is 52 instead of 92: the two position records occupy sp+40/+28
 * rather than +80/+68. No unsupported padding added to conceal the gap.
 * Remaining: scalar stack-slot order, named table/global base ownership,
 * loop-end cmp 7/ble versus cmp 8/blt, chained-multiply local ownership,
 * and the byte-zero pool that anchors the tail's second literal pool.
 * The decoder still reports conditional topology different; do not infer
 * full CFG equivalence from the now-correct static probe count. Stop this
 * trial for the checkpoint; semantic correctness remains a draft claim
 * until full exact-owner verification. No new DONE bytes.
 * H2: give the game-state row and both direction tables independent
 * extern array ownership, as the audited neighbouring movement family
 * already does. Predict a 02000240 base plus row offset, and indexed
 * table loads without the literal-address add. Keep flow, frame and
 * all non-table globals unchanged. One score before the checkpoint;
 * preserve outcome even if the full owner remains nonmatching.
 * H2 result: 1244 bytes, 287 differing halfwords, 163 aligned edits;
 * topology still different. Full normalized difference inspected. The
 * game-state base is now 02000240 plus 540, both direction tables use
 * indexed loads, and probe-branch destinations align. This supports the
 * named-table ownership, not exactness: frame/slots, loop condition,
 * chained multiply and tail pool remain. H1 is committed at e2dc4bd31.
 * Two hypotheses used; preserve the third for genuinely new evidence.
 * 2026-09-29 alchemy permute (seed 1, 3 jobs, 10 minutes): 24,282
 * candidates; the best scored 1585 against 2854 (32 register-only, 20
 * stack-only, 6 operand, 6 reordered, 4 inserted, 5 deleted) after 43
 * rewrites (introduce a temporary, swap commutative operands, reorder local
 * declarations, reorder independent statements), none of them kept.
 * Temporaries and declaration moves make up most of its 43 rewrites;
 * stack-slot use (20 stack-only) is unchanged.
 */
#include "TYPES.H"
#include "OBJECT_RUNTIME.H"
#include "MAP.H"
#include "IWRAM_CALL.H"

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

struct GameFlagRow {
    u16 first;
    u16 second;
};

extern struct GameFlagRow gGameState[];
extern const s16 Data_08013254[16];
extern const s32 Data_0801328c[16];

void Vector_AddPolarOffset(s32 distance, s32 angle, struct Vec *position);
s32 CheckWorldMapCollisionRange(s32, struct WorldPosition *position);
void Object_SetMoveTarget(struct ObjectRuntime *, s32, s32, s32);
void ObjectDispatch_ApplyArgumentToChildren(void *, s32);
void ObjectDispatch_Initialize(void *, u32);
s32 AnimationObjects_SelectAnimation(void *, s32);
s32 Field_CheckConfiguredKeys(void);
s32 FixedSqrt(s32);
struct ObjectRuntime *FieldObject_Create(s32, s32, s32, s32);
s32 GetWorldMapCollision(struct WorldPosition *);

s32 Func_0800f2f8(struct ObjectRuntime *object)
{
    struct Vec position;
    struct Vec test_position;
    s32 angle;
    s32 mode;
    u16 direction;
    s16 angle_offsets[8];
    s32 angle_index;
    s32 collision;
    s32 difference;
    s32 collision_kind;
    s32 square;
    struct ObjectRuntime *effect;
    u8 *animation;
    s32 buttons;

    mode = 2;
    collision = 0;
    if (gGameState[135].first
        & *(u32 *)0x03001ae8) {
        object->speed_limit = 0x10000;
        object->acceleration = 0x14000;
        mode = 5;
    } else {
        object->speed_limit = 0x8000;
        object->acceleration = 0x4000;
    }
    buttons = *(u32 *)0x03001b04;
    if ((buttons & 0x200) != 0)
        object->speed_limit = 0x40000;

    angle = ((*(u32 *)0x03001ae8 >> 4) & 15);
    angle = Data_08013254[angle];
    direction = angle;
    if (direction == 0xffff) {
        collision |= 4;
        goto update_object;
    }

    collision = 0;
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(0x70000, direction, &position);
    if (*(u8 *)0x03001f54 != 0
        && (*(u32 *)0x03001ae8 & 0x200) != 0)
        goto update_object;
    if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&position) != 0)
        goto choose_direction;

    test_position.x = object->x;
    test_position.y = object->y;
    test_position.z = object->z;
    Vector_AddPolarOffset(0x70000, direction + 0x1000, &test_position);
    if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
        goto choose_direction;
    test_position.x = object->x;
    test_position.y = object->y;
    test_position.z = object->z;
    Vector_AddPolarOffset(0x70000, direction - 0x1000, &test_position);
    if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
        goto choose_direction;
    test_position.x = object->x;
    test_position.y = object->y;
    test_position.z = object->z;
    Vector_AddPolarOffset(0x70000, direction + 0x2000, &test_position);
    if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
        goto choose_direction;
    test_position.x = object->x;
    test_position.y = object->y;
    test_position.z = object->z;
    Vector_AddPolarOffset(0x70000, direction - 0x2000, &test_position);
    if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
        goto choose_direction;
    goto update_object;

choose_direction:
    angle_offsets[0] = direction + 0x1000;
    angle_offsets[1] = direction - 0x1000;
    angle_offsets[2] = direction + 0x2000;
    angle_offsets[3] = direction - 0x2000;
    angle_offsets[4] = direction + 0x3000;
    angle_offsets[5] = direction - 0x3000;
    angle_offsets[6] = direction + 0x4000;
    angle_offsets[7] = direction - 0x4000;
    for (angle_index = 0; angle_index < 8; angle_index++) {
        direction = angle_offsets[angle_index];
        position.x = object->x;
        position.y = object->y;
        position.z = object->z;
        Vector_AddPolarOffset(0x70000, direction, &position);
        if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&position) != 0)
            continue;
        test_position.x = object->x;
        test_position.y = object->y;
        test_position.z = object->z;
        Vector_AddPolarOffset(0x70000, direction + 0x1000, &test_position);
        if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
            continue;
        test_position.x = object->x;
        test_position.y = object->y;
        test_position.z = object->z;
        Vector_AddPolarOffset(0x70000, direction - 0x1000, &test_position);
        if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
            continue;
        test_position.x = object->x;
        test_position.y = object->y;
        test_position.z = object->z;
        Vector_AddPolarOffset(0x70000, direction + 0x2000, &test_position);
        if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
            continue;
        test_position.x = object->x;
        test_position.y = object->y;
        test_position.z = object->z;
        Vector_AddPolarOffset(0x70000, direction - 0x2000, &test_position);
        if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
            continue;
        angle = (s16)direction;
        goto update_object;
    }
    collision |= 1;

update_object:
    if (*(u8 **)0x03001ebc != 0) {
        if ((collision & 3) != 0)
            (*(u16 *)(*(u8 **)0x03001ebc + 0x19c))++;
        else
            *(u16 *)(*(u8 **)0x03001ebc + 0x19c) = 0;
    }
    if (collision != 0)
        ObjectDispatch_ApplyArgumentToChildren(object, 9);
    else
        ObjectDispatch_ApplyArgumentToChildren(object, mode);

    if (collision != 0) {
        object->target_x = -0x80000000;
        object->target_y = -0x80000000;
        object->target_z = -0x80000000;
        object->velocity_x = 0;
        object->velocity_z = 0;
        if ((collision & 3) != 0) {
            difference = (s16)((u16)angle - object->angle);
            if (difference > 0x1000)
                difference = 0x1000;
            if (difference < -0x1000)
                difference = -0x1000;
            object->angle += difference;
        }
        object->action = 0;
        object->unknown_66 = 2;
        goto movement_done;
    }

    Object_SetMoveTarget(object, position.x, position.y, position.z);
    square = Iwram_MulQ16(object->velocity_x, object->velocity_x);
    square += Iwram_MulQ16(object->velocity_z, object->velocity_z);
    square = FixedSqrt(square);
    object->velocity_x = collision;
    object->velocity_z = collision;
    Vector_AddPolarOffset(square, (u16)angle, (struct Vec *)&object->velocity_x);
    if (object->action != 0)
        object->action--;

movement_done:
    {
        u8 *work = *(u8 **)0x03001e70;
        u16 *turn = (u16 *)(work + 282);
        s32 rate = Data_0801328c[(*(u32 *)0x03001ae8 >> 4) & 15];
        difference = (s16)(rate - *turn);
        if (difference < 0)
            difference += 7;
        difference >>= 3;
        if (difference > 0x200)
            difference = 0x200;
        if (difference < -0x200)
            difference = -0x200;
        if ((u32)(difference + 15) <= 30)
            difference = rate - *turn;
        *turn += difference;
    }

    if (object->animation_kind == 1) {
        animation = object->animation;
        collision_kind = GetWorldMapCollision((struct WorldPosition *)&object->x);
        if (collision_kind == 9) {
            *(u8 *)(*(u8 **)(animation + 44) + 6) = 1;
            *(u8 *)(animation + 38) = 0;
        } else {
            *(u8 *)(*(u8 **)(animation + 44) + 6) = 9;
            *(u8 *)(animation + 38) = 1;
        }
        if (collision_kind == 6 && *(s16 *)&object->action == 0 && collision == 0) {
            effect = FieldObject_Create(24, object->x, object->y, object->z);
            if (effect != 0) {
                animation = effect->animation;
                ObjectDispatch_Initialize(effect, 0x08013280);
                *((u8 *)effect + 0x55) = collision;
                *((u8 *)effect + 0x22) = 1;
                if (animation != 0) {
                    AnimationObjects_SelectAnimation(animation, 1);
                    *(u8 *)(animation + 38) = collision;
                    animation[5] = (animation[5] & ~12) | 4;
                    animation[9] = (animation[9] & ~12) | 8;
                }
                object->action = 10;
            }
        }
    }
    Field_CheckConfiguredKeys();
    object->step++;
    return 1;
}
