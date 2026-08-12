#include "types.h"

/*
 * resource_3ba owner at 0x020031c0, 480 bytes.  The owner opens with the
 * r5-r8 save, contains a branch-skipped literal run at 0x02003312-0x0200332f,
 * and ends at the sole unwind return at 0x02003394-0x0200339f.  The next
 * semantic owner starts at 0x020033a0, so there is no trailing pool.
 *
 * This is a three-mode movement/state driver.  Mode 1 consumes signed X/Z
 * waypoints from the workspace table at +0xf0.  A zero/zero pair changes the
 * actor to its terminal animation and positions it beside workspace anchors;
 * otherwise the actor turns toward the waypoint with a clamped angular step.
 * Mode 2 writes the actor's current s16 samples back into that same table and,
 * at cursor 0x383e, terminates the capture and restores mode 1.
 *
 * The same control-flow shape occurs independently in resource_3ba:31c0 and
 * resource_3bc:3ef0.  Their script pointers differ, so each is admitted and
 * audited in its own overlay namespace rather than credited as a byte twin.
 *
 * Call accounting: nine reachable BL sites, all resolved and represented in
 * source/machine order.  The halfwords at 0x0200332c are an unreachable BL
 * shape inside the skipped literal run; the reachable-instruction audit and
 * multiset check both exclude that data rather than inventing a call.
 */

typedef struct {
    s16 mode;
    s16 mirror;
    s16 actor_id;
    u16 cursor;
    s16 settled_frames;
} State3ba;

typedef struct {
    u8 pad_e0[0xe0];
    u16 next_actor_id;
    u16 heading_negative;
    u16 heading_positive;
    u16 pad_e6;
    s32 x;
    s32 z;
    s16 waypoints[0x3840];
} Workspace3ba;

#define STATE_3BA ((State3ba *)0x02001000)
#define WORKSPACE_3BA (*(Workspace3ba *volatile *)0x03001f3c)

s32 Func_08000100();
void Func_08009080();
void Func_08009098();
void Func_08009150();
s32 Func_080770c0();
void Func_080770c8();
u8 *Func_0808a080();

void Func_020031c0(void)
{
    State3ba *state = STATE_3BA;
    Workspace3ba *workspace = WORKSPACE_3BA;
    u8 *actor = Func_0808a080(state->actor_id);
    s32 target_x;
    s32 target_z;

    if (actor == 0)
        return;

    if (state->mode == 1) {
        u16 cursor;
        s16 first;
        s16 second;

        cursor = state->cursor;
        first = workspace->waypoints[(s16)cursor];
        cursor++;
        second = workspace->waypoints[(s16)cursor];
        state->cursor = (u16)(cursor + 1);

        if (first == 0 && second == 0) {
            state->mode = 9;
            Func_08009080(actor, 1);

            target_x = workspace->x + 0x000c0000;
            if (workspace->x < *(s32 *)(actor + 8))
                target_x = workspace->x + (s32)0xfff40000;

            if (Func_080770c0(0x211) != 0) {
                target_z = workspace->z + 0x00100000;
                *(u16 *)(actor + 0x64) = workspace->heading_positive;
            } else {
                target_z = workspace->z + (s32)0xfff00000;
                *(u16 *)(actor + 0x64) = workspace->heading_negative;
            }

            *(s32 *)(actor + 0x34) = 0x4000;
            *(s32 *)(actor + 0x30) = 0x10000;
            Func_08009150(actor, target_x, 0, target_z);
            Func_080770c8(0x211);
            Func_08009098(actor, (const void *)0x0200c6a8);
        } else {
            s32 turn;

            target_x = (s32)first << 16;
            target_z = (s32)second << 16;
            if (state->mirror != 0)
                target_x = workspace->x * 2 - target_x;

            if (*(s32 *)(actor + 8) != target_x || *(s32 *)(actor + 16) != target_z) {
                turn = (s16)((u16)Func_08000100(
                    target_z - *(s32 *)(actor + 16),
                    target_x - *(s32 *)(actor + 8)) - *(u16 *)(actor + 6));
                if (turn > 0x1000)
                    turn = 0x1000;
                if (turn < -0x1000)
                    turn = -0x1000;
                *(u16 *)(actor + 6) = (u16)(*(u16 *)(actor + 6) + turn);
                *(s32 *)(actor + 8) = target_x;
                *(s32 *)(actor + 16) = target_z;
                state->settled_frames = 0;
            } else {
                state->settled_frames++;
            }

            if (state->settled_frames > 2)
                Func_08009080(actor, 1);
            else
                Func_08009080(actor, 5);
        }
    } else if (state->mode == 2) {
        u16 cursor;
        s16 next_cursor;

        cursor = state->cursor;
        workspace->waypoints[(s16)cursor] = *(s16 *)(actor + 0x0a);
        cursor++;
        workspace->waypoints[(s16)cursor] = *(s16 *)(actor + 0x12);
        cursor++;
        next_cursor = (s16)cursor;
        state->cursor = (u16)next_cursor;

        if (next_cursor == 0x383e) {
            workspace->waypoints[next_cursor] = 0;
            workspace->waypoints[next_cursor + 1] = 0;
            state->actor_id = workspace->next_actor_id;
            state->cursor = 0;
            state->mode = 1;
        }
    }
}
