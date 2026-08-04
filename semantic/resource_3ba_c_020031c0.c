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

#define STATE_3BA ((u8 *)0x02001000)
#define WORKSPACE_3BA (*(u8 *volatile *)0x03001f3c)

s32 Func_08000100();
void Func_08009080();
void Func_08009098();
void Func_08009150();
s32 Func_080770c0();
void Func_080770c8();
u8 *Func_0808a080();

void Func_020031c0(void)
{
    u8 *state = STATE_3BA;
    u8 *workspace = WORKSPACE_3BA;
    u8 *actor = Func_0808a080(*(s16 *)(state + 4));
    u16 cursor;
    s32 target_x;
    s32 target_z;

    if (actor == 0)
        return;

    if (*(s16 *)state == 1) {
        s16 first;
        s16 second;

        cursor = *(u16 *)(state + 6);
        first = *(s16 *)(workspace + 0xf0 + (s16)cursor * 2);
        second = *(s16 *)(workspace + 0xf0 + (s16)(cursor + 1) * 2);
        *(u16 *)(state + 6) = (u16)(cursor + 2);

        if (first == 0 && second == 0) {
            *(u16 *)state = 9;
            Func_08009080(actor, 1);

            target_x = *(s32 *)(workspace + 0xe8) + 0x000c0000;
            if (*(s32 *)(workspace + 0xe8) >= *(s32 *)(actor + 8))
                target_x = *(s32 *)(workspace + 0xe8) + (s32)0xfff40000;

            if (Func_080770c0(0x211) != 0) {
                target_z = *(s32 *)(workspace + 0xec) + 0x00100000;
                *(u16 *)(actor + 0x64) = *(u16 *)(workspace + 0xe4);
            } else {
                target_z = *(s32 *)(workspace + 0xec) + (s32)0xfff00000;
                *(u16 *)(actor + 0x64) = *(u16 *)(workspace + 0xe2);
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
            if (*(s16 *)(state + 2) != 0)
                target_x = *(s32 *)(workspace + 0xe8) * 2 - target_x;

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
                *(u16 *)(state + 8) = 0;
            } else {
                *(u16 *)(state + 8) = (u16)(*(u16 *)(state + 8) + 1);
            }

            if (*(s16 *)(state + 8) > 2)
                Func_08009080(actor, 1);
            else
                Func_08009080(actor, 5);
        }
    } else if (*(s16 *)state == 2) {
        s16 next_cursor;

        cursor = *(u16 *)(state + 6);
        *(s16 *)(workspace + 0xf0 + (s16)cursor * 2) = *(s16 *)(actor + 0x0a);
        *(s16 *)(workspace + 0xf0 + (s16)(cursor + 1) * 2) = *(s16 *)(actor + 0x12);
        next_cursor = (s16)(cursor + 2);
        *(u16 *)(state + 6) = (u16)next_cursor;

        if (next_cursor == 0x383e) {
            *(u16 *)(workspace + 0xf0 + next_cursor * 2) = 0;
            *(u16 *)(workspace + 0xf0 + (next_cursor + 1) * 2) = 0;
            *(u16 *)(state + 4) = *(u16 *)(workspace + 0xe0);
            *(u16 *)(state + 6) = 0;
            *(u16 *)state = 1;
        }
    }
}
