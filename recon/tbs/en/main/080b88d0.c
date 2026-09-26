/* Draft, not exact (2026-09-26): 632 of 632 bytes, 44 differing halfwords,
   23 aligned edits, frame 128. Baseline was 614 bytes / 154 edits.
   ROM-backed corrections: reload action[0] after validation; target's later
   >127 test is signed, while its <=7 tests are unsigned; loops exit on equal
   counts; living_count is initialized before the validity calls. Those changes
   gave 636 / 96 / 50. A typed 84-byte context restores the base held across
   its actor-copy loop and shared tail stores, giving the retained 632 / 44 / 23.
   Remaining: transition-mode branch orientation, four commutative adds, side
   flag common tail, and final setup-call argument order. A combined explicit
   mode/side-tail, index-first adds, and confirmed void SetupTransitionScene
   prototype trial regressed to 636 / 253 / 96 by shifting reload assignments
   throughout the owner. It is not retained; isolate the declared return type
   before adoption (registered SetupTransitionScene returns void). */
#include "TYPES.H"

#define BattlePres_RunPairedUnitTransition Func_080b88d0

#define FIELD16(base, offset) (*(u16 *)((u8 *)(base) + (offset)))
#define FIELD32(base, offset) (*(u32 *)((u8 *)(base) + (offset)))

struct TransitionContext {
    s32 kind;
    s32 side;
    s32 actor;
    u8 unknown_0c[8];
    s32 count;
    u8 unknown_18[12];
    s16 actors[14];
    u8 unknown_40[20];
};

void Func_080c9010(struct TransitionContext *context);

s32 BattlePres_RunPairedUnitTransition(s16 *action)
{
    u16 visible_units[14];
    struct TransitionContext context;
    s32 actor_id;
    s32 target_id;
    u32 side_start;
    u32 side_count;
    u32 living_count;
    u32 actor_record;
    u32 index;

    living_count = 0;
    actor_id = action[0];
    if (Func_080b8808(actor_id) < 0) {
        return -1;
    }
    target_id = action[5];
    if (Func_080b8808(target_id) < 0) {
        return -1;
    }

    {
        u32 *transition = *(u32 **)0x03001f00;
        transition[0] = action[0] <= 4 ? 0x2000 : 0x5000;
        transition[1] = 60;
    }
    Func_080030f8(10);
    Func_08004458();
    actor_record = *(u32 *)Func_080b7dd0(actor_id);

    if ((u32)target_id <= 7) {
        side_count = Func_080b6b40(2, visible_units);
        side_start = 0x80;
    } else {
        side_count = Func_080b6b40(1, visible_units);
        side_start = 0;
    }

    for (index = 0; index != side_count; index++) {
        if (side_start + index == actor_id) {
            Func_08009080(actor_record, 3);
        }
    }

    Func_080030f8(30);
    *(u16 *)0x04000050 = 0x3f40;
    for (index = 0; index != side_count; index++) {
        Func_080c0f98(side_start + index, 1);
    }
    for (index = 0; index != 16; index++) {
        *(u16 *)0x04000052 = (16 - index) | 0x1000;
        Func_080030f8(1);
    }
    Func_08015130(9);

    if (target_id > 0x7f) {
        u32 count = Func_080b6b40(2, visible_units);
        for (index = 0; living_count != count; index++) {
            u32 unit = index + 0x80;
            if (*(s16 *)((u8 *)Func_08077008(unit) + 0x38) > 0) {
                visible_units[living_count++] = unit;
            }
        }
    } else {
        u32 count = Func_080b6b40(1, visible_units);
        for (index = 0; living_count != count; index++) {
            if (*(s16 *)((u8 *)Func_08077008(index) + 0x38) > 0) {
                visible_units[living_count++] = index;
            }
        }
    }
    visible_units[living_count] = 0xff;
    Func_080b7b6c(visible_units, 0);

    context.kind = action[4];
    context.actor = actor_id;
    for (index = 0; index != living_count; index++) {
        context.actors[index] = visible_units[index];
    }
    context.count = living_count;
    context.side = (u32)target_id <= 7;
    Func_080c9010(&context);

    Func_080030f8(10);
    Func_080b6c90();
    *(u16 *)0x04000050 = 0x3f40;
    for (index = 0; index != side_count; index++) {
        Func_080c0f98(side_start + index, 1);
    }
    for (index = 0; index != 16; index++) {
        *(u16 *)0x04000052 = index | 0x1000;
        Func_080030f8(1);
    }
    for (index = 0; index != side_count; index++) {
        Func_080c0f98(side_start + index, 0);
    }
    Func_080c0cec(0, 0, 0, 0x64);
    Func_080030f8(3);
    return 0;
}
