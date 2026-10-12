/* Draft, differ 171 of 119, a first attempt: the shape is the native's (two loops over the 8 by 11
 * cells, a statue placed or hidden for each occupied one), but the native keeps only the scene
 * state's base in r5 and rebuilds every field offset where it is used, inside the loops, while
 * this form loads the origin's addresses from the pool above them (r9, r11). The state pointer
 * that keeps Func_020044d0's offsets inside its loop does not hold them here. */
#include "types.h"
#include "object_runtime.h"

struct PuzzleCell {
    u8 kind;
    u8 blocked;
    u8 state;
    u8 occupied;
};

struct SceneState {
    struct PuzzleCell cells[88];
    u8 unknown_160[4];
    s16 first_id;
    s16 end_id;
    u16 tile_entry;
    s8 redraw_a;
    s8 redraw_b;
    s16 origin_x;
    s16 origin_z;
    s16 statues_ready;
    u8 unknown_172[2];
    s32 statue_offset;
};

extern struct SceneState gSceneState;

struct ObjectRuntime *Object_GetById(s32 id);
void Func_0200566c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

static __inline__ void Map_Put(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_0200566c(a, b, c, d, e, f);
}

void Func_02004288(s32 id, s32 show)
{
    struct SceneState *state = &gSceneState;
    s32 row;
    s32 column;

    state->statues_ready = show;
    state->first_id = id;
    for (row = 0; row <= 7; row++) {
        for (column = 0; column <= 10; column++) {
            struct PuzzleCell *cell = &state->cells[row * 11 + column];

            if (cell->occupied != 0) {
                struct ObjectRuntime *object = Object_GetById(id);

                if (show != 0) {
                    object->x = ((state->origin_x + column) << 20) + 0x80000;
                    object->z = ((state->origin_z + row) << 20) + 0x80000;
                    Map_Put(0, 11, 1, 1, state->origin_x + column, state->origin_z + row);
                    cell->occupied = 2;
                } else {
                    object->x = 0;
                    object->z = 0;
                    cell->occupied = 1;
                    Map_Put(1, cell->kind, 1, 1, state->origin_x + column, state->origin_z + row);
                }
                id++;
            }
        }
    }
    state->end_id = id;
}
