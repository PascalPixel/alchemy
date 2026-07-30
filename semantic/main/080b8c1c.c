#include "layout_guard.h"
#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

typedef struct Input_080b8c1c {
    s16 primary_id;
    u8 padding02[6];
    s16 work_flags;
    s16 secondary_id;
    s16 field_0c;
} Input_080b8c1c;

typedef struct Work_080b8c1c {
    s32 flags;
    s32 secondary_is_low_id;
    s32 primary_id;
    s32 secondary_id;
    s32 field_10;
    s32 count;
    s32 mode;
    s32 unknown_1c;
    u8 padding20[4];
    s16 members[24];
} Work_080b8c1c;

typedef struct Child_080b8c1c {
    u8 padding00[5];
    u8 value;
} Child_080b8c1c;

typedef struct Record_080b8c1c {
    u8 padding00[0x27];
    u8 child_count;
    Child_080b8c1c **children;
} Record_080b8c1c;

typedef struct Motion_080b8c1c {
    u8 padding00[0x50];
    Record_080b8c1c *record;
} Motion_080b8c1c;

typedef struct Slot_080b8c1c {
    Motion_080b8c1c *object;
} Slot_080b8c1c;

typedef struct Transition_080b8c1c {
    s32 facing;
    s32 timer;
} Transition_080b8c1c;

LAYOUT_OFFSET_GUARD(
    Input080b8c1c_WorkFlags,
    Input_080b8c1c,
    work_flags,
    8);
LAYOUT_OFFSET_GUARD(
    Input080b8c1c_Secondary,
    Input_080b8c1c,
    secondary_id,
    0x0a);
LAYOUT_OFFSET_GUARD(
    Work080b8c1c_Members,
    Work_080b8c1c,
    members,
    0x24);
LAYOUT_SIZE_GUARD(
    Work080b8c1c_Size,
    Work_080b8c1c,
    84);
LAYOUT_OFFSET_GUARD(
    Motion080b8c1c_Record,
    Motion_080b8c1c,
    record,
    0x50);
LAYOUT_OFFSET_GUARD(
    Record080b8c1c_Children,
    Record_080b8c1c,
    children,
    0x28);

void Func_080030f8(s32);
void Func_08009080(Motion_080b8c1c *, s32);
void Func_08009088(Motion_080b8c1c *, s32);
void Func_08015130(s32);
s32 Func_080b6b40(s32, s16 *);
Slot_080b8c1c *Func_080b7dd0(s32);
void Func_080b8000(s32);
s32 Func_080b8808(s32);
void Func_080c9018(Work_080b8c1c *);
void Func_080c9020(Work_080b8c1c *);

/*
 * Prepare a one-member battle work area, copy that member's child attributes,
 * run each preparation phase, then execute and release the participating
 * actors.
 */
s32 Func_080b8c1c(const Input_080b8c1c *input)
{
    Transition_080b8c1c *transition =
        *(Transition_080b8c1c **)0x03001f00;
    u8 *world = *(u8 **)0x03001e74;
    Work_080b8c1c work;
    Motion_080b8c1c *primary_object;
    s32 target;
    s32 initial_team;
    s32 i;

    if (input->primary_id <= 4)
        target = 0x2000;
    else
        target = -0x2000;

    if (transition->facing == target) {
        transition->timer = 40;
        Func_080030f8(40);
    } else {
        transition->facing = target;
        transition->timer = 40;
        Func_080030f8(40);
    }

    work.flags = input->work_flags;
    work.field_10 = input->field_0c;
    work.primary_id = input->primary_id;
    work.secondary_id = input->secondary_id;

    if (Func_080b8808(work.primary_id) < 0)
        return -1;

    if (work.secondary_id > 127)
        initial_team = 2;
    else
        initial_team = 1;
    work.count = Func_080b6b40(initial_team, work.members);

    Func_08015130(world[0x41] & ~1);
    primary_object = Func_080b7dd0(work.primary_id)->object;
    Func_08009080(primary_object, 3);
    Func_08009088(primary_object, 16);

    if ((u16)input->secondary_id <= 7) {
        work.secondary_is_low_id = 1;
        Func_080b6b40(1, work.members);
    } else {
        work.secondary_is_low_id = 0;
        Func_080b6b40(2, work.members);
    }
    work.count = 1;

    for (i = 0; i < work.count; i++) {
        Record_080b8c1c *record =
            Func_080b7dd0(work.members[i])->object->record;
        s32 children = record->child_count - 1;
        s32 child;

        for (child = 0; child < children; child++) {
            FIELD(&work, u8, 0x34 + i * 4 + child) =
                record->children[child]->value;
        }
    }

    work.flags = 0;
    work.mode = 0;
    Func_080c9020(&work);
    work.flags = 1;
    Func_080c9020(&work);
    work.flags = 2;
    Func_080c9020(&work);
    work.flags = 3;
    Func_080c9020(&work);

    work.flags = 0;
    Func_080c9018(&work);
    Func_08009080(primary_object, 1);

    for (i = 0; i < work.count; i++)
        Func_080b8000(work.members[i]);
    Func_080b8000(work.primary_id);
    return 0;
}
