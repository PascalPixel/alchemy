#include "types.h"
#include "layout_guard.h"

struct Object_080a90bc {
    u8 padding[15];
    u8 state;
};

struct Slot_080a90bc {
    u8 padding[5];
    u8 dirty;
};

struct State_080a90bc {
    u8 unknown_00[8];
    s32 current;
    u8 unknown_0c[8];
    struct Slot_080a90bc *slot;
    u8 unknown_18[4];
    s8 index;
    u8 unknown_1d[7];
    void *display;
    u8 unknown_28[4];
    void *panel;
    u8 unknown_30[24];
    struct Object_080a90bc *objects[32];
    u8 unknown_c8[68];
    void *resource;
    u8 unknown_110[184];
    u8 layout[64];
    u16 entries[8];
    u8 selection;
    u8 count;
    u8 current_entry;
    u8 unknown_21b[33];
    s16 positions_y[4];
    u8 unknown_244[28];
    u8 marks[64];
};

LAYOUT_OFFSET_GUARD(
    Object_080a90bc_state_offset, struct Object_080a90bc, state, 15);
LAYOUT_OFFSET_GUARD(Slot_080a90bc_dirty_offset, struct Slot_080a90bc, dirty, 5);
LAYOUT_OFFSET_GUARD(State_080a90bc_current_offset, struct State_080a90bc,
    current, 8);
LAYOUT_OFFSET_GUARD(State_080a90bc_slot_offset, struct State_080a90bc, slot, 20);
LAYOUT_OFFSET_GUARD(State_080a90bc_index_offset, struct State_080a90bc, index,
    28);
LAYOUT_OFFSET_GUARD(State_080a90bc_display_offset, struct State_080a90bc,
    display, 36);
LAYOUT_OFFSET_GUARD(State_080a90bc_panel_offset, struct State_080a90bc, panel,
    44);
LAYOUT_OFFSET_GUARD(State_080a90bc_objects_offset, struct State_080a90bc,
    objects, 72);
LAYOUT_OFFSET_GUARD(State_080a90bc_resource_offset, struct State_080a90bc,
    resource, 268);
LAYOUT_OFFSET_GUARD(State_080a90bc_layout_offset, struct State_080a90bc, layout,
    456);
LAYOUT_OFFSET_GUARD(State_080a90bc_entries_offset, struct State_080a90bc,
    entries, 520);
LAYOUT_OFFSET_GUARD(State_080a90bc_selection_offset, struct State_080a90bc,
    selection, 536);
LAYOUT_OFFSET_GUARD(State_080a90bc_count_offset, struct State_080a90bc, count,
    537);
LAYOUT_OFFSET_GUARD(State_080a90bc_current_entry_offset,
    struct State_080a90bc, current_entry, 538);
LAYOUT_OFFSET_GUARD(State_080a90bc_positions_y_offset, struct State_080a90bc,
    positions_y, 572);
LAYOUT_OFFSET_GUARD(State_080a90bc_marks_offset, struct State_080a90bc, marks,
    608);

struct Session_080a90bc {
    u8 unknown_00[8];
    u8 scratch[8];
    s32 rows;
    void *cursor;
    s32 mark;
};

LAYOUT_OFFSET_GUARD(Session_080a90bc_rows_offset, struct Session_080a90bc, rows,
    16);
LAYOUT_OFFSET_GUARD(Session_080a90bc_cursor_offset, struct Session_080a90bc,
    cursor, 20);
LAYOUT_OFFSET_GUARD(Session_080a90bc_mark_offset, struct Session_080a90bc, mark,
    24);

extern struct State_080a90bc *Data_03001f2c;
extern volatile u32 Data_03001b04;
extern volatile u32 Data_03001c94;

void Func_080a8c2c(void);
void Func_08015270(void *arg0);
void Func_080a10d0(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4,
    s32 arg5);
void Func_08004278(void *arg0);
void Func_080a33d4(void *arg0, void *arg1);
void Func_08015080(s32 arg0, void *arg1, s32 arg2, s32 arg3);
void Func_080f9010(s32 arg0);
void Func_080041d8(void *arg0, s32 arg1);
void Func_080a8f40(void *arg0, s32 arg1, void *arg2);
void Func_080a8d34(void *arg0, s32 arg1, void *arg2);
void Func_080030f8(s32 arg0);
s32 Func_080a1fd4(s32 arg0, void *arg1, s32 arg2, void *arg3, void *arg4);
void Func_080a1a40(s32 arg0, s32 arg1);
s32 Func_080022fc(s32 arg0, s32 arg1);
void Func_080a1804(void *arg0, s32 arg1);
s32 Func_080770c0(s32 arg0);
void Func_080a9cbc(void);
s32 Func_08077008(s32 arg0);
s32 Func_080a68ec(s32 arg0, void *arg1, s32 arg2);
void Func_080a8b8c(void *arg0, s32 arg1);
void Func_080a9374(void *arg0, s32 arg1);
void Func_08015278(void *arg0);
void Func_080a345c(void);

extern void Func_080a19a0(void);

s32 Func_080a90bc(void)
{
    struct State_080a90bc *state = Data_03001f2c;
    struct Session_080a90bc session;
    s16 *position;
    struct Object_080a90bc **entry;
    s32 remaining;
    s32 result = 0;
    s32 done = 0;
    s32 redraw;
    s32 reload;
    s32 status;
    s32 index;
    s32 id;
    s32 offset;
    s32 next;

    Func_080a8c2c();
    Func_08015270(state->resource);
    Func_080a10d0(&state->panel, 0, 0, 30, 5, 2);

    remaining = 3;
    position = &state->positions_y[3];
    do {
        remaining--;
        *position-- = -16;
    } while (remaining >= 0);

    entry = state->objects;
    remaining = 31;
    do {
        struct Object_080a90bc *object = *entry++;

        if (object != 0) {
            object->state = 245;
        }
        remaining--;
    } while (remaining >= 0);

    Func_08004278(Func_080a19a0);
    Func_080a33d4(state, state->resource);
    offset = -24;
    id = 2822;
    Func_08015080(id, state->display, 80, offset);
    id += 2;
    Func_08015080(id, state->display, 0, offset);

    while (done == 0 && Func_080770c0(336) == 0) {
        Func_080a9cbc();
        Func_08015270(state->display);
        state->selection =
            Func_080a68ec(Func_08077008(state->current_entry), state->layout, 0);
        Func_080030f8(1);
        Func_080a8b8c(&session, 0);
        Func_080a9374(state->display, state->current_entry);
        redraw = 1;
        reload = 1;

        while (Func_080770c0(336) == 0) {
            if (redraw != 0) {
                s32 pending = reload;

                redraw = 0;
                if (pending != 0) {
                    reload = redraw;
                    Func_080a8f40(state->display, 0, &session);
                }
                Func_080a8d34(state->display, 0, &session);
                Func_080030f8(1);
            }
            Func_080030f8(1);
            status = Func_080a1fd4(0, session.cursor, 5, &session.rows,
                &session.scratch);
            state->slot->dirty = 1;
            Func_080a1a40(55, session.rows * 16 + 60);
            if (status == 1) {
                reload = 1;
                redraw = 1;
            }
            if (status == 0) {
                redraw = 1;
            }
            if (status == -1) {
                redraw = 0;
            }
            if ((Data_03001c94 & 1) != 0) {
                Func_080f9010(112);
                result = 1;
                done = 1;
                break;
            }
            if ((Data_03001c94 & 2) != 0) {
                Func_080f9010(113);
                result = -1;
                done = 1;
                Func_080041d8(Func_080a19a0, 3200);
                break;
            }
            next = 0x100;
            if ((Data_03001b04 & next) != 0 || (Data_03001b04 & 0x200) != 0) {
                Func_080f9010(111);
                index = state->index;
                state->marks[state->entries[index]] = session.mark;
                if ((Data_03001b04 & next) != 0) {
                    index++;
                } else {
                    index--;
                }
                index = Func_080022fc(index + state->count, state->count);
                state->current = state->entries[index];
                state->current_entry = state->entries[index];
                state->index = index;
                Func_080a1804(state, state->entries[index]);
                break;
            }
        }
    }

    Func_08015278(state->panel);
    Func_08015270(state->panel);
    Func_080a345c();
    Func_08015278(state->resource);
    Func_08015270(state->display);
    return result;
}

s32 Func_080a9370(void)
{
    return 1;
}
