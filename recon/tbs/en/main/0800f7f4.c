/* NONMATCHING: 472 / 472 bytes, 33 differing halfwords, 32 aligned edits.
 * 2026-09-26 H1: capture the displacement for both sign tests and give the
 * object-facing stores a byte/field union view. This recovers the exact
 * shared r5-relative position reload at 0800f882, including its incoming
 * branch, with no extra address, load or stack space. Full normalized diff
 * confirms that only that branch halfword differs from the prior baseline.
 * The recovered join is an admission invariant for subsequent experiments.
 * Whole owner [0800f7f4,0800f9cc) includes eight owned pool words. Callback
 * pointers occur at 08013688 and 08114db0; all five callees were audited.
 * gKeysHeld already binds to the named Data_03001ae8 linker symbol, so the
 * independently successful literal-key-address correction is inapplicable.
 * H1 is preserved at 6c1ca77f3 (472 bytes, 33 halfwords, 32 edits).
 * H2 applies explicit mutually exclusive action selection, unchanged case
 * first, instead of a default followed by conditional replacement. The mask
 * now precedes the first action assignment, but the initial default-value
 * lifetimes and reloads move away from the reference. The exact alias join,
 * frame and all pools survive. Full normalized difference read; this action
 * selection family does not close the remaining allocation. No adoption.
 * H2 is preserved at 5048a0132. H3 restores H1's action lifetime and moves
 * displacement initialization into a typed inline projection helper. Frame,
 * pools and the exact alias join remain fixed, but argument setup is still
 * after the zero stores. Only the order of three argument-setup instructions
 * changes from H1, with the same residual count. Full normalized diff read.
 * Stop after three hypotheses; the retained result is the store-side alias
 * invariant, not an exact owner. No further local-order sweep is justified.
 *
 * Historical draft (2026-09-25): 472 of 472 bytes, 34 differing halfwords.
   Every call, store and branch is in place and the frame matches (the
   position sits above 68 unused bytes). Remaining: allocation swaps r0-r3
   in the prologue, the zeroed position and both height tests, and the ROM
   reloads pos.z at the join after the two facing stores where this spelling
   skips the reload on the z <= 0 path (as if the ROM's facing store could
   alias the position). Struct, array, u16 and split spellings moved nothing.
   2026-09-26: a coordinate/halfword union also emits identical bytes.
   An explicit volatile z reload fixes the branch destination but creates
   a new sp+76 address, adds an instruction and disturbs the following
   load order (472 bytes / 48 aligned edits, baseline 33). Neither explains
   the reference's shared r5-relative reload; retain the typed baseline.
   The event-work pointer is bound directly to its audited pool address
   below so this draft scores independently without per-file bindings.
   2026-09-29 (alchemy permute scorer): 745 (17 register-only, 5 operand,
   9 reordered), including the unlabelled key-angle table at 08013254
   (inside the Script_MainScript scaffold block). Spelling MAP_CELLS as
   the linked gMapCellBuffer instead of 0x02010000 scores 1790: with a
   symbol, the two cell addresses share its register through CSE and the
   allocation cascades. The literal address keeps this a draft. */
#include "OBJECT_RUNTIME.H"
#include "MAP.H"

/* FAKEMATCH: a byte/field union view makes facing stores conservatively
 * alias the stack position, as the reference's shared join reload requires.
 * The two sign tests consume one captured displacement before either store. */
union KeyMoveObject {
    struct ObjectRuntime fields;
    u8 bytes[sizeof(struct ObjectRuntime)];
};

struct KeyMoveEventWork {
    u8 unknown_000[0x19c];
    u16 blocked_steps;
};

struct KeyMovePosition {
    s32 x;
    s32 y;
    s32 z;
};

extern volatile u32 gKeysHeld;
extern struct KeyMoveEventWork *Data_03001ebc;
extern const s16 Data_08013254[16];

void Vector_AddPolarOffset(s32 radius, s32 angle, struct KeyMovePosition *position);
s32 Func_08011f54(u32 layer, s32 x, s32 z);
void ObjectDispatch_ApplyArgumentToChildren(struct ObjectRuntime *object, s32 value);
void Object_SetMoveTarget(struct ObjectRuntime *object, s32 x, s32 y, s32 z);

#define MAP_CELLS ((struct MapCell *)0x02010000)

/* FAKEMATCH: give displacement initialization the projection-call lifetime
 * so its argument values can be established before the three zero stores. */
static __inline__ void MakeKeyStep(u16 angle, struct KeyMovePosition *pos)
{
    pos->x = 0;
    pos->y = 0;
    pos->z = 0;
    Vector_AddPolarOffset(0x80000, angle, pos);
}

/*
 * Walks an object half a tile in the direction the pad is held. The step is
 * refused where the next cell is occupied or changes height; with L or R
 * held it is refused where the ground rises or falls away too far. A step
 * the object cannot take counts as a blocked step in the event work.
 */
s32 Object_MoveByKeys(struct ObjectRuntime *object)
{
    struct KeyMovePosition pos;
    u8 unused[68];
    struct MapCell *from;
    struct MapCell *to;
    u16 angle;
    s32 motion;
    s32 blocked;
    s32 delta_z;

    object->speed_limit = 0x8000;
    object->acceleration = 0x4000;
    angle = Data_08013254[(gKeysHeld >> 4) & 15];
    motion = 12;
    blocked = 4;
    if (angle != 0xffff) {
        motion = 14;
        if ((angle & 0xf000) != 0) {
            motion = 15;
            if ((angle & 0xf000) != 0x8000)
                motion = 10;
        }
        blocked = 0;
        MakeKeyStep(angle, &pos);
        pos.x += object->x;
        delta_z = pos.z;
        if (delta_z < 0)
            ((union KeyMoveObject *)object)->fields.angle = 0xc000;
        if (delta_z > 0)
            ((union KeyMoveObject *)object)->fields.angle = 0x4000;
        pos.y = object->y - pos.z;
        pos.z = object->z;
        from = &MAP_CELLS[object->x / 0x100000 + (pos.z / 0x100000) * 128];
        to = &MAP_CELLS[pos.x / 0x100000 + (pos.z / 0x100000) * 128];
        if (CheckMapPositionCellOccupied((struct WorldPosition *)&pos) != 0
            || from->collision_code != to->collision_code) {
            blocked = 4;
            motion = 12;
        } else if (gKeysHeld & 0x40) {
            if (Func_08011f54(object->terrain_id, pos.x, pos.z - 0x100000) - object->y < 0x100000) {
                blocked = 1;
                motion = 12;
            }
        } else if (gKeysHeld & 0x80) {
            if (Func_08011f54(object->terrain_id, pos.x, pos.z) - object->y > -0x80000) {
                blocked = 1;
                motion = 12;
            }
        }
    }
    if (Data_03001ebc != NULL) {
        if (blocked & 3)
            Data_03001ebc->blocked_steps++;
        else
            Data_03001ebc->blocked_steps = 0;
    }
    ObjectDispatch_ApplyArgumentToChildren(object, motion);
    if (blocked != 0) {
        object->target_x = 0x80000000;
        object->target_y = 0x80000000;
        object->target_z = 0x80000000;
        object->velocity_x = 0;
        object->velocity_y = 0;
        object->velocity_z = 0;
    } else {
        Object_SetMoveTarget(object, pos.x, pos.y, pos.z);
    }
    object->step++;
    return 1;
}
