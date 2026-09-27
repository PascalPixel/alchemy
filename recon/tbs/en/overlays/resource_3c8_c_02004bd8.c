/* NONMATCHING: 452 bytes, candidate 452, 172 differing halfwords, 129
 * aligned edits (2026-09-27). VinasuHeya_DispatchPushScript, meant for
 * FIELD/VINASU_HEYA/F_04BD8.C as a single-overlay unit binding its names at
 * their runtime addresses (an import veneer's listing offset plus 0x8000).
 * Remaining: hand-written push-script dispatch from velocity+switch cells;
 * shared fail paths and FindSlot Value2 in place
 * WALL: copy_versus_rematerialise: child/list/cell-index register sharing
 * and pool rematerialise of cursor-table bases resist further spelling.
 * Complete extent 02004bd8..02004d9c includes the final fifteen pool words.
 * H1: exact AnimationObjects_SelectAnimation and AnimationObject_Allocate
 * establish selected_animation +24 and animation-object resource id at
 * sprite +28 -> +0; this is not animation progress. Exact staged probes
 * independently confirm the nested view. Own 032c returns an actor and
 * ignores the caller's second argument. Retain that observed zero argument.
 * Canonical Engine_ObjectSetScript at overlay veneer 0200cddc fixes both
 * emitted script calls that the old main-address alias misbound. The typed
 * view otherwise leaves body allocation unchanged: 452/452, 172/129.
 * Full diff still has a merged success/failure call instead of three calls,
 * cached selected animation, and strength-reduced coordinate/cursor loops.
 * No DONE. No exact neighbour or shared header was edited. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIXED_POINT_POSITION.H"

extern u8 Value_0000ffff;
extern u8 Value_000000b9;
extern s16 RuntimeSelectorTable[];

extern u8 Data_0200d1a4[];
extern u8 Data_0200d1a8[];
extern u8 Data_0200d1ac[];
extern u8 Data_0200d1b0[];
extern const u32 Data_0200d564[];
extern struct { s32 x; s32 z; } Data_0200d128[];
extern struct { s32 x; s32 z; } Data_0200d164[];
extern u8 *Data_0200f72c[];
extern u8 *Data_0200f78c[];
extern u32 *Data_0200f77c[];
extern u32 *Data_0200f7ec[];

/* Animation selection's exact consumer establishes +24 as the selected
 * animation and +28 as the first animation object, whose first field is id. */
struct VinasuAnimationObject {
    s16 id;
};

struct VinasuSpriteAnimation {
    u8 unknown_00[0x24];
    u8 selected_animation;
    u8 unknown_25[3];
    struct VinasuAnimationObject *entries[4];
};

struct FieldActor *SceneData_FindSlotAtPosition(struct FixedPointPosition *pos, s32 unused);

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

/* Probe one unit below the actor for animation resource 0x100 and dispatch
 * a push script from its selected animation and the scene's switch cells. */
s32 VinasuHeya_DispatchPushScript(struct FieldActor *actor)
{
    struct FixedPointPosition pos;
    struct FieldActor *slot;
    struct VinasuSpriteAnimation *child;
    s32 vx;
    s32 vz;
    s32 abs_x;
    s32 abs_z;
    s32 axis;
    u8 *list;
    u8 byte;
    u8 *want_ptr;
    u32 cell;
    u32 skip;
    u8 *cursor;
    s32 x_cell;
    u32 *script_row;

    pos.x = actor->x.fixed;
    pos.y = actor->y.fixed + (s32)0xfff00000;
    pos.z = actor->z.fixed;
    slot = (struct FieldActor *)Value2((s32 (*)())SceneData_FindSlotAtPosition, (s32)&pos, 0);
    child = (struct VinasuSpriteAnimation *)slot->sprite;
    if (child->entries[0]->id != 0x100) {
        goto fail_tail;
    }

    vx = actor->velocity_x;
    abs_x = vx;
    if (vx < 0) {
        abs_x = -vx;
    }
    vz = actor->velocity_z;
    abs_z = vz;
    if (vz < 0) {
        abs_z = -vz;
    }

    if (abs_x > abs_z) {
        axis = vx;
        if (axis < 0) {
            axis += (s32)&Value_0000ffff;
        }
        if (axis < 0) {
            list = Data_0200d1a4;
        } else {
            list = Data_0200d1a8;
        }
    } else {
        axis = vz;
        if (axis < 0) {
            axis += (s32)&Value_0000ffff;
        }
        if (axis < 0) {
            list = Data_0200d1ac;
        } else {
            list = Data_0200d1b0;
        }
    }

    byte = list[0];
    if (byte == 0) {
        goto fail_mid;
    }
    want_ptr = &child->selected_animation;
    if (*want_ptr != byte) {
        do {
            list++;
            byte = list[0];
            if (byte == 0) {
                goto fail_mid;
            }
        } while (*want_ptr != byte);
    }
    if (byte == 0) {
        goto fail_mid;
    }

    if (RuntimeSelectorTable[224] == (s32)&Value_000000b9) {
        cell = 0;
        x_cell = actor->x.fixed;
        if ((x_cell >> 20) == Data_0200d128[0].x
            && (actor->z.fixed >> 20) == Data_0200d128[0].z) {
            goto found_b9;
        }
        do {
            cell++;
            if (cell > 3) {
                break;
            }
        } while ((x_cell >> 20) != Data_0200d128[cell].x
                 || (actor->z.fixed >> 20) != Data_0200d128[cell].z);
    found_b9:
        skip = 0;
        cell <<= 2;
        goto scan_b9;
    advance_b9:
        *(u8 **)((u8 *)Data_0200f72c + cell) = cursor + 1;
        skip++;
    scan_b9:
        cursor = *(u8 **)((u8 *)Data_0200f72c + cell);
        if (cursor[0] == 0) {
            goto fail_mid;
        }
        if (cursor[0] != ((struct VinasuSpriteAnimation *)slot->sprite)->selected_animation) {
            goto advance_b9;
        }
        script_row = *(u32 **)((u8 *)Data_0200f77c + cell);
        goto dispatch;
    }

    cell = 0;
    x_cell = actor->x.fixed;
    if ((x_cell >> 20) == Data_0200d164[0].x
        && (actor->z.fixed >> 20) == Data_0200d164[0].z) {
        goto found_other;
    }
    do {
        cell++;
        if (cell > 7) {
            break;
        }
    } while ((x_cell >> 20) != Data_0200d164[cell].x
             || (actor->z.fixed >> 20) != Data_0200d164[cell].z);
found_other:
    skip = 0;
    cell <<= 2;
    goto scan_other;
advance_other:
    *(u8 **)((u8 *)Data_0200f78c + cell) = cursor + 1;
    skip++;
scan_other:
    cursor = *(u8 **)((u8 *)Data_0200f78c + cell);
    if (cursor[0] == 0) {
        goto fail_mid;
    }
    if (cursor[0] != ((struct VinasuSpriteAnimation *)slot->sprite)->selected_animation) {
        goto advance_other;
    }
    script_row = *(u32 **)((u8 *)Data_0200f7ec + cell);

dispatch:
    Call2((void (*)())Engine_ObjectSetScript, (s32)actor, script_row[skip]);
    goto done;

fail_mid:
    Call2((void (*)())Engine_ObjectSetScript, (s32)actor, (s32)Data_0200d564);
    goto done;

fail_tail:
    Call2((void (*)())Engine_ObjectSetScript, (s32)actor, (s32)Data_0200d564);
done:
    return 0;
}
