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
 * H2: transfer the proven typed-subrecord boundary to the queued command
 * halfword (row+4/field+2); give that fixed 20-row scan its unsigned counter,
 * matching the reference's BLS backedge. No declaration-order sweep.
 * H2 result: 616/620 bytes, 227 differing halfwords, 123 aligned edits.
 * The typed command read now uses field+2 and the unsigned backedge matches,
 * but strength reduction still advances row pointers instead of indexing.
 * Frame/save set remain correct. Stop at the bounded followup; this is not a
 * near match. Untried width evidence: ROM keeps the inventory item as a word,
 * and separately narrows the use-type at the second branch.
 * H3, resumed with explicit width evidence: keep the inventory item in an
 * s32 local after its unsigned halfword load; keep use_type in a distinct u8
 * local. Prediction: remove the item's signed-load/extension sequence and
 * recover the final 0x1ff mask pool without changing frame or call topology.
 * H3 result: 612/620 bytes, 248 differing halfwords, 114 aligned edits.
 * Both inventory predictions hold: LDRH into r5 and the 0x1ff pool/AND are
 * restored, with unit r6, frame 88 and the saved-role set unchanged. The u8
 * use-type still loses the reference copy and second-branch zero extension.
 * No matching-C credit claimed.
 * 2026-09-29: callees carry the build's names; alchemy permute (with
 * --function Func_080ba6ac) scores 2480, from 2700. BattleFx_DispatchModeFar stays
 * unresolved: the BattleFx_DispatchMode veneer in SYSTEM/FAR_CALL/EFFECT.S
 * has no label yet.
 * Eight minutes of permutation then found 2120: the queued-command scan as a
 * do-while that advances row before testing the command (2155 alone), the
 * fade loop as a while with its steps at the end, and the item read through
 * a u16 view with the use type assigned in the test.
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
struct QueuedCommandKind {
    s16 unknown_04;
    s16 kind;
};
struct QueuedItemAction {
    s16 actor_id;
    s16 unknown_02;
    struct QueuedCommandKind dispatch;
    s16 parameter;
    s16 unknown_0a;
    u8 reserved_0c[4];
};
static __inline__ s32 QueuedCommand_GetKind(struct QueuedCommandKind *command)
{
    return command->kind;
}
struct PresentationBattleWork {
    u8 reserved_000[0x2ec];
    struct QueuedItemAction actions[20];
    u8 reserved_42c[0x118];
    u16 palette[128];
    s32 palette_scale;
};

extern s32 *gTransitionWork;
extern struct PresentationBattleWork *gBattleWork;
void BattleEvent_Playback(void);
void Object_SetMode(struct MotionObject *, s32);
void ObjectDispatch_ApplyValueToChildrenFar(struct MotionObject *, s32);
s32 Inventory_RemoveFar(s32, s32);
s32 Inventory_BreakFar(s32, s32);
void Actor_ResetMotionAtAnchor(s32);
s32 BattlePres_BuildTargetList(void *, struct PresentationWork *);
u32 BattleEv_DispatchQueued(void);
u32 BattleEv_Push(u32, u32);
s32 BattleEventRuntime_WaitForReady(void);
void BattlePres_SetActorModes(u16 *, s32);
s32 Graphics_ScaleRgb555Clamped(u16 *, u16 *, s32, s32);
void BattleFx_DispatchByIdRangeFar(struct PresentationWork *);
void BattleFx_DispatchModeFar(struct PresentationWork *);

s32 Func_080ba6ac(struct BattlePlan *input, s32 unused,
                  struct BattleCommandRequest *selection)
{
    register struct BattlePlan *saved_input = input;
    register struct BattleCommandRequest *saved_selection = selection;
    struct PresentationWork work;
    struct MotionObject *object;
    s32 i;
    s32 ability;
    struct BattleUnit *unit;
    u8 kind;

    s32 *transition = gTransitionWork;
    s32 facing = -0x2000;
    if (saved_input->actor_id <= 4)
        facing = 0x2000;
    if (*transition != facing)
        *transition = facing;
    BattlePres_BuildTargetList(saved_input, &work);
    BattlePres_SetActorModes(0, 0);
    object = GetBattleObjectSlot(work.field_08)->object;
    Object_SetMode(object, 3);
    ObjectDispatch_ApplyValueToChildrenFar(object, 16);
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
        i = 0;
        while (i <= 19) {
            struct PresentationBattleWork *battle = gBattleWork;
            if (i <= 19) {
                s32 value = 0x10000 - fade;
                battle->palette_scale = value;
                Graphics_ScaleRgb555Clamped(battle->palette, (u16 *)0x050000c0, value, 0x80);
            }
            WaitFrames(1);
            i++;
            fade += 0x444;
        }
        if (saved_input->presentation_flags & 0x4000)
            BattleFx_DispatchByIdRangeFar(&work);
        else
            BattleFx_DispatchModeFar(&work);
    } else {
        WaitFrames(60);
    }
    BattleEventRuntime_WaitForReady();
    Object_SetMode(object, 1);
    for (i = 0; i != work.count; i++)
        Actor_ResetMotionAtAnchor(work.table[i]);

    unit = Owner_GetStateFar(saved_selection->actor_id);
    ability = ((u16 *)unit->inventory)[saved_selection->parameter];
    if ((kind = Item_Get(ability)->use_type) == 1) {
        s32 result = Inventory_RemoveFar(saved_selection->actor_id, saved_selection->parameter);
        s32 index = saved_selection->parameter;
        if (result == 2) {
            struct PresentationBattleWork *battle = gBattleWork;
            u32 row;
            row = 0;
            do {
                struct QueuedItemAction *command = &battle->actions[row];
                row++;
                if (QueuedCommand_GetKind(&command->dispatch) == 2 &&
                    command->actor_id == saved_selection->actor_id) {
                    s16 current = command->parameter;
                    if (current == index)
                        command->parameter = 0xffff;
                    else if (current > index)
                        command->parameter--;
                }
            } while (row <= 19);
        }
    } else if ((u8)kind == 2) {
        if ((BattleRandom16Far() & 7) == 0) {
            BattleEv_Push(2, unit->inventory[saved_selection->parameter]);
            BattleEv_Push(4, 0x81c);
            Inventory_BreakFar(saved_selection->actor_id, saved_selection->parameter);
            BattleEv_DispatchQueued();
        }
    } else if ((u8)kind == 4) {
        if ((ability & 0x1ff) == 0xb8)
            ability = 0xb9;
        unit->inventory[saved_selection->parameter] = ability;
    }
    return 0;
}
