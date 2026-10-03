/* 2026-10-03 typed-owner attempt; this supersedes the old matching scores.
 * Native EN extent [080b7738,080b78e4): 428 bytes including the pool.
 * Retained ordinary typed draft: 420 bytes, frame 36 versus native 44;
 * score 5737 (52 register, 7 stack, 14 operand, 6 reordered, 22 inserted,
 * 26 deleted). All eight call relocations and the gCameraWork pool word
 * resolve to the native targets; complete .text is not byte-identical.
 *
 * Replaced five private views with BattleObjectSlot, MotionObject,
 * AnimationObject and SpriteEntry. The old hidden word is MotionObject.y.
 * Effect-entry byte 6 and AnimationObject.dirty retain their byte widths;
 * camera yaw is still tested as signed s16. The hardware priority update
 * uses a byte view anchored in the existing first AnimationSpritePart.
 * Only its attribute-2 priority bits change; tile and palette bits survive.
 * Canonical getter, list and cycle declarations come from their owners.
 * Battle_RunEncounter schedules this void(void) callback and uses no result.
 *
 * Bounded ordinary forms, approved TBS compiler and era assembler, EN:
 * T0 typed six-bit attr, mask ~3: 484 bytes, score 6757, frame 36.
 * T1 finite six-bit mask 0x3c: 474 bytes, score 6792, frame 36.
 * T2 byte attribute view and slot-owned SpriteEntry: 416 bytes, score 5592.
 * T0-T2 carried the old icon register bindings; no new bindings were added.
 * T3 requested check without those bindings: 420 bytes, score 5737.
 * T3 is retained: old pin evidence does not establish a matching typed
 * model, and the new pins also remove an instruction. No pin remains.
 *
 * STOP: remaining differences include the smaller real frame, icon-context
 * dirty-store setup/order, priority-value lifetimes and side-loop allocation.
 * No unread storage, replacement object, shared-header edit, new compiler
 * option or routing was introduced. The older attempts below remain S4
 * evidence only; no adoption or all-edition matching credit is claimed.
 */
#include "BATTLE_PRESENTATION.H"
#include "BATTLE_PARTY.H"
#include "MOTION_OBJECT.H"
#include "ANIMSPR.H"
/* 2026-10-02 bounded register-lifetime experiment.
 * Baseline immutable score: 1175 (49 register-only, 2 operand,
 * 8 reordered, 2 inserted, 2 deleted); the older all-register summary
 * below is superseded by this normalized diagnostic.
 * H1: constrain each genuinely used object local to r5 at its definition.
 * Prediction: icon slot/object become r6/r5 and both priority objects r5;
 * disjoint later priority lifetimes can reuse r5 without extra instructions.
 * Accept only complete matching extent/pool/symbols in all six editions,
 * with the 44-byte frame unchanged and no instruction-presence device.
 * Budget: H1 plus two localized follow-ups, then record a precise stop.
 * H1 result: 5022 (41 register, 7 stack, 12 operand, 12 reordered,
 * 20 inserted, 18 deleted). Object roles improve, but inline constraints
 * disrupt priority loop optimization and shrink the frame to 36. Rejected.
 * H2: retain local fixed-register declarations without an inline asm node.
 * Prediction: preserve ascending/reversed priority loops and the 44-byte
 * frame while selecting r5 for each used object. Same acceptance gate.
 * H2 result: identical 5022 diagnostic and 36-byte frame. Fixed-register
 * object declarations themselves prevent the priority-loop optimization;
 * the inline asm node was not its cause. This priority pin axis is closed.
 * H3: restore ordinary priority objects; constrain only the icon object
 * to r5, motion context to r0 and icon effect to r2. Prediction: preserve
 * both optimized priority loops/frame and fix only the icon scan roles.
 * H3 result: 1030 (40 register, 2 operand, 8 reordered, 1 inserted,
 * 2 deleted); both priority loops and the 44-byte frame stay. The icon
 * roles now match, but its used context stays r0 for the dirty store,
 * removing the native r0-to-r2 copy. One new localized fact permits H4.
 * H4: after the effect store, transfer the genuinely used context into
 * a dirty-context local in r2. Prediction: restore that copy and the native
 * dirty store without disturbing the exact icon roles or priority loops.
 * This is the last trial; no broader lifetime search follows.
 * H4 first compile refused a declaration after a statement (C89);
 * the same transfer is corrected into its own declaration block.
 * H4 result: unchanged 1030 diagnostic. The dirty transfer is optimized
 * back into r0; rejected and removed. H3 remains the canonical near miss.
 * STOP: scoped priority pins alter loop shape/frame; icon-only pins fix
 * call-crossing roles but leave the dirty-store copy wrong. No dead code,
 * unread storage, instruction template, address alias or new routing used.
 * Final measurement: each TBS edition scores 1030 with all nine symbolic
 * relocations resolved (eight calls, one gCameraWork pool word). Ordinary
 * compiler plus compiler assembler emits complete .text/function 424 bytes
 * versus native 428, including the pool; missing pointer copy changes the
 * pool alignment as well. The frame remains 44 bytes. No all-edition byte
 * credit is earned. no-asm tbs-en: forbidden=0.
 * The draft remains uncredited; no neighbor files were changed.
 */
/* DRAFT (score 1175), rewritten 2026-10-02: same frame and same shape as the
 * ROM, 63 instructions differ, all register choices.
 * 1. In the two priority loops the ROM holds the object in r5 and, inside the
 *    four-record loop, reuses r5 for the shifted priority; here the object is
 *    in r0. With one object variable for the whole function the object is in
 *    r5, but the shifted priority is then set before the record list is read,
 *    conflicts with it and pushes the loop counter out of r7.
 * 2. In the icon scan the ROM leaves the motion record in r0 and loads the icon
 *    effect into r2; here they are the other way round, and the slot and the
 *    object trade r5 and r6.
 * Settled: the priorities are a two-word array copied into a variable for
 * each loop (which is why the ROM masks them with 3 and shares the masked
 * value between the two cases); the icon scan is not strength-reduced (kept
 * as a goto loop, tagged); the slot belongs to each loop; the four-record
 * loops are ascending loops the compiler reverses. Walking the record list
 * with a pointer lets the compiler reverse the outer loop, which the ROM
 * does not. */
#include "TYPES.H"
#include "BATTLE_STATUS_ICON.H"

/* Cycle status icons, update the effect entry from a unit's vertical
 * position, and order the two sides around the camera. */
void Func_080b7738(void)
{
    u16 ids[14];
    s32 priority[2];
    s32 i;
    s32 j;
    s32 count;

    BattleParty_ListActorIds(BATTLE_SIDE_BOTH, ids);
    /* FAKEMATCH: the scan is rotated by hand with a goto, which keeps the loop
     * pass off it; as a for loop it becomes a pointer walk. */
    i = 0;
    if (ids[i] != BATTLE_UNIT_LIST_END) {
again:
        {
            struct BattleObjectSlot *slot = GetBattleObjectSlot(ids[i]);

            if (slot != 0) {
                struct MotionObject *object = slot->object;

                BattleStatusIcon_Cycle(slot);
                if (slot->effect_entry != 0) {
                    struct AnimationObject *context = GetMotionRecord(object, 0);

                    if (context != 0) {
                        struct SpriteEntry *effect;
                        s32 state = 0;

                        if (object->y != 0)
                            state = 9;
                        effect = slot->effect_entry;
                        if (effect->priority != state) {
                            effect->priority = state;
                            context->dirty = 1;
                        }
                    }
                }
            }
        }
        i++;
        if (i <= 13 && ids[i] != BATTLE_UNIT_LIST_END)
            goto again;
    }
    if ((s16)gCameraWork->yaw >= 0) {
        priority[0] = 1;
        priority[1] = 2;
    } else {
        priority[0] = 2;
        priority[1] = 1;
    }
    {
        s32 value;

        count = BattleParty_ListActorIds(BATTLE_SIDE_PARTY, ids);
        value = priority[0];
        for (i = 0; i < count; i++) {
            struct BattleObjectSlot *slot = GetBattleObjectSlot(ids[i]);

            if (slot != 0) {
                struct MotionObject *object = slot->object;

                switch (object->record_storage_kind & 15) {
                case 1:
                {
                    struct AnimationObject *record = object->records;
                    u8 *attributes = (u8 *)&record->part[0];

                    /* Attribute 2: priority is bits 2-3 of its high byte. */
                    attributes[9] = (attributes[9] & ~0x0c) | ((value & 3) << 2);
                    break;
                }
                case 2:
                    for (j = 0; j < 4; j++) {
                        struct AnimationObject *record = ((struct AnimationObject **)object->records)[j];

                        if (record != 0) {
                            u8 *attributes = (u8 *)&record->part[0];

                            attributes[9] = (attributes[9] & ~0x0c) | ((value & 3) << 2);
                        }
                    }
                    break;
                }
            }
        }
    }
    {
        s32 value;

        count = BattleParty_ListActorIds(BATTLE_SIDE_ENEMIES, ids);
        value = priority[1];
        for (i = 0; i < count; i++) {
            struct BattleObjectSlot *slot = GetBattleObjectSlot(ids[i]);

            if (slot != 0) {
                struct MotionObject *object = slot->object;

                switch (object->record_storage_kind & 15) {
                case 1:
                {
                    struct AnimationObject *record = object->records;
                    u8 *attributes = (u8 *)&record->part[0];

                    /* Attribute 2: priority is bits 2-3 of its high byte. */
                    attributes[9] = (attributes[9] & ~0x0c) | ((value & 3) << 2);
                    break;
                }
                case 2:
                    for (j = 0; j < 4; j++) {
                        struct AnimationObject *record = ((struct AnimationObject **)object->records)[j];

                        if (record != 0) {
                            u8 *attributes = (u8 *)&record->part[0];

                            attributes[9] = (attributes[9] & ~0x0c) | ((value & 3) << 2);
                        }
                    }
                    break;
                }
            }
        }
    }
}
