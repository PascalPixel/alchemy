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

 * 2026-10-03 T0 natural typed baseline: replaces private storage views and
 * the addressing-only command wrapper with the maintained owners. The
 * child parameters use the byte view at actors[8], with count - 1 sampled
 * once per animation. Direct action rows retain signed fields and an
 * unsigned 20-row scan. The redundant inner fade guard, register aliases
 * and inventory cast are removed; there is no replacement steering device.
 * The earlier unresolved-veneer note is historical: both dispatch veneers
 * now have their physical names in SYSTEM/FAR_CALL/EFFECT.S.
 * Native zero return and the unused second argument are retained. The
 * production caller's discarded void declaration needs closure before
 * adoption, as does the EN message 0x81c's catalogue name and edition value.
 * T0 result, one approved TBS EN compile: 608/620 bytes including the full
 * 44-byte pool. Complete relocation-normalized comparison has 557 differing
 * bytes in the shared 608-byte span and a 12-byte shorter extent; the first
 * difference is +0x02. The current raw object normalizes exactly to own ROM.
 * Both objects have 27 relocations (24 calls, three absolute pointers); all
 * call targets and their order agree. All 11 pool values agree, but the
 * palette/brightness offsets exchange order and the pool starts at +0x234
 * instead of +0x240. No unresolved symbol or compilation blocker remains.
 * Local frame 88/work sp+4 and plan sl/selection r8/object r9 are retained.
 * The parameter-byte base uses fp, adding one saved register: 32/28 saved
 * bytes and 120/116 total stack. The child-count snapshot uses r4, not ip.
 * Produced/native instructions are 258/264, loads 50/50, stores 7/7 and
 * branches including calls/return 54/55. Direct action access becomes a row
 * pointer with command at +6, versus native indexed row+4/field+2. The fade
 * counter counts down and omits the redundant inner guard; the use-type
 * copy and second-branch byte extension are absent. Register and scheduling
 * differences remain elsewhere. No aligned instruction score is claimed.
 * Stopped after this single baseline, with no follow-up, padding, steering
 * device or matching-C credit. The adoption closures above remain open.
 */
#include "TYPES.H"
#include "BATTLE_EVENT.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_WORK.H"
#include "BATTLE_PRESENTATION.H"
#include "MOTION_OBJECT.H"
#include "ANIMSPR.H"
#include "OBJDISP.H"
#include "INVENTORY.H"
#include "SYSTEM.H"
#include "IO_REG.H"
#include "CALLBACK_SCHEDULER.H"

extern struct BattlePresentationTransition *gTransitionWork;
void BattleEvent_Playback(void);
void Object_SetMode(void *object, s32 mode);
void ObjectDispatch_ApplyValueToChildrenFar(struct DispatchObject *object, s32 value);
s32 Inventory_RemoveFar(s32 owner, s32 slot);
void Actor_ResetMotionAtAnchor(s32 actor);
void BattlePres_SetActorModes(u16 *actors, s32 mode);
s32 Graphics_ScaleRgb555Clamped(u16 *source, u16 *destination, s32 scale, s32 count);
void BattleFx_DispatchByIdRangeFar(s32 *work);
void BattleFx_DispatchModeFar(s32 *work);

s32 Func_080ba6ac(struct BattlePlan *plan, s32 unused,
                 struct BattleActionRecord *selection)
{
    struct BattlePresentationWork work;
    struct MotionObject *object;
    s32 i;
    s32 item;
    struct BattleUnit *unit;
    u8 kind;
    struct BattlePresentationTransition *transition = gTransitionWork;
    s32 facing = -0x2000;

    if (plan->actor_id <= 4)
        facing = 0x2000;
    if (transition->target_yaw != facing)
        transition->target_yaw = facing;
    BattlePres_BuildTargetList(plan, &work);
    BattlePres_SetActorModes(0, 0);
    object = GetBattleObjectSlot(work.actor)->object;
    Object_SetMode(object, 3);
    ObjectDispatch_ApplyValueToChildrenFar((struct DispatchObject *)object, 16);
    if (plan->target_ids[0] <= 7)
        work.side = 1;
    else
        work.side = 0;

    {
        /* Child parameters reuse the tail of the actor storage, starting
           at work + 0x34, with four bytes reserved per target. */
        u8 *params = (u8 *)&work.actors[8];

        for (i = 0; i != work.count; i++) {
            struct AnimationObject *animation = GetMotionRecord(
                GetBattleObjectSlot(work.actors[i])->object, 0);
            s32 count = animation->count - 1;
            s32 j;

            for (j = 0; j != count; j++)
                params[i * 4 + j] = animation->entries[j]->param;
        }
    }

    Scheduler_AddOrUpdateCallback((s32)BattleEvent_Playback, 0xc80);
    if (work.kind != 0) {
        s32 fade = 0;
        i = 0;
        while (i <= 19) {
            struct BattleSession *battle = gBattleWork;
            s32 value = 0x10000 - fade;

            battle->brightness = value;
            Graphics_ScaleRgb555Clamped(
                battle->palette, (u16 *)(BG_PLTT + 6 * 16), value, 128);
            WaitFrames(1);
            i++;
            fade += 0x444;
        }
        if (plan->presentation_flags & 0x4000)
            BattleFx_DispatchByIdRangeFar((s32 *)&work);
        else
            BattleFx_DispatchModeFar((s32 *)&work);
    } else {
        WaitFrames(60);
    }
    BattleEventRuntime_WaitForReady();
    Object_SetMode(object, 1);
    for (i = 0; i != work.count; i++)
        Actor_ResetMotionAtAnchor(work.actors[i]);

    unit = Owner_GetStateFar(selection->unit_id);
    item = unit->inventory[selection->parameter];
    kind = Item_Get(item)->use_type;
    if (kind == 1) {
        s32 result = Inventory_RemoveFar(selection->unit_id, selection->parameter);
        s32 index = selection->parameter;
        if (result == 2) {
            struct BattleSession *battle = gBattleWork;
            u32 row;

            for (row = 0; row < 20; row++) {
                struct BattleActionRecord *action = &battle->actions[row];

                if (action->command == 2 && action->unit_id == selection->unit_id) {
                    s16 current = action->parameter;
                    if (current == index)
                        action->parameter = -1;
                    else if (current > index)
                        action->parameter--;
                }
            }
        }
    } else if (kind == 2) {
        if ((BattleRandom16Far() & 7) == 0) {
            BattleEv_Push(BATTLE_EVENT_ITEM, unit->inventory[selection->parameter]);
            BattleEv_Push(BATTLE_EVENT_TEXT, 0x81c);
            Inventory_BreakFar(selection->unit_id, selection->parameter);
            BattleEv_DispatchQueued();
        }
    } else if (kind == 4) {
        if ((item & ITEM_ID_MASK) == 0xb8)
            item = 0xb9;
        unit->inventory[selection->parameter] = item;
    }
    return 0;
}
