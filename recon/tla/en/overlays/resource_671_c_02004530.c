/* Drafts of Func_02004530 and Func_02004620, the neighbours of s45d8.c and s46c8.c, differ 36 of
 * 72 and 36 of 74: everything matches but the twenty-step loop. The native builds the address of
 * the statue offset inside the loop on every pass (movs r3, #186; lsls r3, #1; adds r2, r5, r3;
 * ldr; add; str) and keeps only the scene state in r5 and the count in r6; this form builds it
 * once, in r6, before the loop. Measured (alchemy diff --rtl, the loop dump): the loop has 10
 * insns and the address (life 4, savings 1) passes loop.c's test, 15 * 1 * 4 >= 10. The scene
 * state read directly (gSceneState.field) is worse (differ 48); a loop built with goto still
 * has the address above it (gcse places it, since the store after the loop uses it again); the
 * wait as a function that returns twice puts every variable on the stack. Wanted: why the
 * native's loop keeps a twice-used address inside, here and in Func_02000550. */
#include "types.h"

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

void Func_0200567c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02005854(s32 sound);
void Func_020056b4(void);
void Func_020056bc(void);
void Func_0200580c(s32 value);
void Func_020044b4(s32 mode);
void Func_020044d0(s32 value);
void WaitFrames(s32 frames);

static __inline__ void Map_Copy(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_0200567c(a, b, c, d, e, f);
}

void Func_02004530(void)
{
    struct SceneState *state = &gSceneState;
    s32 i;

    if (state->redraw_a == 0) {
        Map_Copy(1, 13, 1, 1, state->origin_x + 3, state->origin_z - 2);
        if (state->statues_ready != 0) {
            Func_02005854(248);
            Func_020056b4();
            Func_0200580c(0);
            Func_020044b4(3);
            for (i = 19; i >= 0; i--) {
                state->statue_offset -= 0x333;
                WaitFrames(1);
            }
            Func_020044d0(0);
            Func_020056bc();
            state->statue_offset = 0;
        }
        state->redraw_a = 1;
    }
}

void Func_02004620(void)
{
    struct SceneState *state = &gSceneState;
    s32 i;

    if (state->redraw_b == 0) {
        Map_Copy(0, 13, 1, 1, state->origin_x + 7, state->origin_z - 2);
        if (state->statues_ready != 0) {
            Func_02005854(248);
            Func_020056b4();
            Func_0200580c(0);
            Func_020044b4(2);
            for (i = 19; i >= 0; i--) {
                state->statue_offset += 0x333;
                WaitFrames(1);
            }
            Func_020044d0(1);
            Func_020056bc();
            state->statue_offset = 0;
        }
        state->redraw_b = 1;
    }
}
