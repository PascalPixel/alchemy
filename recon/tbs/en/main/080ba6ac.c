/* Draft, complete main:080ba6ac [080ba6ac,080ba918), 620 bytes.
 * Baseline: 600/620 bytes, 290 differing halfwords, 201 aligned edits.
 * H1 transfers the signed-member/cached-child-count model from 080ba978,
 * exact BattlePlan/CommandRequest/Unit/Item interfaces, and canonical return
 * types. Own-ROM audit also corrects the scheduler callback, reloads an item
 * after the RNG call, and snapshots the removed index after Inventory_Remove.
 * H1 result: 604/620 bytes, 246 differing halfwords, 127 aligned edits.
 * Frame 88 and input sl/selection r8/object r9 now match. First instruction
 * difference is value-call argument setup; child count is r4 instead of ip.
 * Command loop strength-reduces a row pointer, unlike the indexed reference;
 * its command read uses row+0/field+6 instead of row+4/field+2. The u16 item
 * local also causes an unwanted signed load/extension and removes mask pool.
 * No matching-C credit claimed.
 */
#include "TYPES.H"
#include "BATTLE_COMMAND.H"
#include "MOTION_OBJECT.H"
#include "SYSTEM.H"
#include "CALLBACK_SCHEDULER.H"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

struct PresentationWork {
    s32 field_00;
    s32 field_04;
    s32 field_08;
    u8 reserved_0c[8];
    s32 count;
    u8 reserved_18[12];
    s16 table[8];
    u8 values[32];
};
struct MotionEntry { u8 reserved_00[39]; u8 count; void *children[1]; };
struct MotionChild { u8 reserved_00[5]; u8 value; };
struct QueuedItemAction {
    struct BattleCommandRequest command;
    u8 reserved_0c[4];
};
struct PresentationBattleWork {
    u8 reserved_000[0x2ec];
    struct QueuedItemAction actions[20];
    u8 reserved_42c[0x118];
    u16 palette[128];
    s32 palette_scale;
};

extern s32 *Data_03001f00;
extern struct PresentationBattleWork *Data_03001e74;
void BattleEvent_Playback(void);
void Func_08009080(struct MotionObject *, s32);
void Func_08009088(struct MotionObject *, s32);
s32 Func_08077058(s32, s32);
s32 Func_08077060(s32, s32);
void Func_080b8000(s32);
s32 Func_080b9d34(void *, struct PresentationWork *);
u32 Func_080bb938(void);
u32 Func_080bbabc(u32, u32);
s32 Func_080be02c(void);
void Func_080c10e8(u16 *, s32);
s32 Func_080c1724(u16 *, u16 *, s32, s32);
void Func_080c9008(struct PresentationWork *);
void Func_080c9018(struct PresentationWork *);

s32 Func_080ba6ac(struct BattlePlan *input, s32 unused,
                  struct BattleCommandRequest *selection)
{
    register struct BattlePlan *saved_input = input;
    register struct BattleCommandRequest *saved_selection = selection;
    struct PresentationWork work;
    struct MotionObject *object;
    s32 i;
    u16 ability;
    struct BattleUnit *unit;
    s32 kind;

    s32 *transition = Data_03001f00;
    s32 facing = -0x2000;
    if (saved_input->actor_id <= 4)
        facing = 0x2000;
    if (*transition != facing)
        *transition = facing;
    Func_080b9d34(saved_input, &work);
    Func_080c10e8(0, 0);
    object = GetBattleObjectSlot(work.field_08)->object;
    Func_08009080(object, 3);
    Func_08009088(object, 16);
    if (saved_input->target_ids[0] <= 7)
        work.field_04 = 1;
    else
        work.field_04 = 0;

    {
        s32 i1;
        for (i1 = 0; i1 != work.count; i1++) {
            struct MotionEntry *entry = GetMotionRecord(
                GetBattleObjectSlot(work.table[i1])->object, 0);
            s32 count = entry->count - 1;
            s32 j;
            for (j = 0; j != count; j++)
                work.values[i1 * 4 + j] =
                    ((struct MotionChild *)entry->children[j])->value;
        }
    }

    Scheduler_AddOrUpdateCallback((s32)BattleEvent_Playback, 0xc80);
    if (work.field_00 != 0) {
        s32 fade = 0;
        for (i = 0; i <= 19; i++, fade += 0x444) {
            struct PresentationBattleWork *battle = Data_03001e74;
            if (i <= 19) {
                s32 value = 0x10000 - fade;
                battle->palette_scale = value;
                Func_080c1724(battle->palette, (u16 *)0x050000c0, value, 0x80);
            }
            WaitFrames(1);
        }
        if (saved_input->presentation_flags & 0x4000)
            Func_080c9008(&work);
        else
            Func_080c9018(&work);
    } else {
        WaitFrames(60);
    }
    Func_080be02c();
    Func_08009080(object, 1);
    for (i = 0; i != work.count; i++)
        Func_080b8000(work.table[i]);

    unit = BattleUnit_Get(saved_selection->actor_id);
    ability = unit->inventory[saved_selection->parameter];
    kind = Item_Get(ability)->use_type;
    if (kind == 1) {
        s32 result = Func_08077058(saved_selection->actor_id, saved_selection->parameter);
        s32 index = saved_selection->parameter;
        if (result == 2) {
            struct PresentationBattleWork *battle = Data_03001e74;
            for (i = 0; i <= 19; i++) {
                struct BattleCommandRequest *command = &battle->actions[i].command;
                if (command->command == 2 &&
                    command->actor_id == saved_selection->actor_id) {
                    s16 current = command->parameter;
                    if (current == index)
                        command->parameter = 0xffff;
                    else if (current > index)
                        command->parameter--;
                }
            }
        }
    } else if ((u8)kind == 2) {
        if ((Func_080771a0() & 7) == 0) {
            Func_080bbabc(2, unit->inventory[saved_selection->parameter]);
            Func_080bbabc(4, 0x81c);
            Func_08077060(saved_selection->actor_id, saved_selection->parameter);
            Func_080bb938();
        }
    } else if ((u8)kind == 4) {
        if ((ability & 0x1ff) == 0xb8)
            ability = 0xb9;
        unit->inventory[saved_selection->parameter] = ability;
    }
    return 0;
}
