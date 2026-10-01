/* DRAFT: complete main:080aa768 [080aa768,080aac84), 1308 bytes.
 * Current: 1324/1308 bytes, 590 differing halfwords, 306 aligned edits.
 * All attempts remain committed; no exact DONE credit.
 * H1 (2026-09-27): replace the old 840-byte model's four-times-scaled
 * fields, missing transfer/recalculation calls and incorrect state exits.
 * Whole listing, switch table, pools, caller 080aa56c and menu-family
 * callees audited. Transfer the Djinn list/owner/cursor record boundaries
 * from 080ab5e4 and exact CORE_COMPUTE_ENTRY_VALUES.C. The cursor column
 * is __umodsi3(cursor,10), not division; packed IDs have a low-byte view.
 * Prediction: all 16 states, shared tails and call arguments are present,
 * with the reference eight-byte outgoing/local frame. Gate: exact whole
 * extent plus compare/test/coverage/verify. Read full normalized diff.
 * One model plus at most two justified follow-ups, stop by 00:30 Lisbon.
 * Record every score/residual here and preserve each attempt in Git.
 * H1: 1340/1308 bytes, 596 differing halfwords, 351 aligned edits. Full
 * diff read. All state/call bodies now exist, but this compiler rounds the
 * ID union to four bytes: table counts move to +0x140 instead of +0xa0,
 * and fields after selected IDs shift by four. This disproves the union
 * record boundary; correct the byte-pair representation before allocation.
 * Frame is 4/8 bytes and result occupies r8 instead of r4; not adopted.
 * H2: use the exact family's u16 arrays with an explicit low-byte view,
 * not a rounded aggregate per ID. Predict 20-byte rows, counts at +0xa0,
 * lists at +0x184, owners at +0x208 and flags at +0x220. Also correct the
 * slot-position prototype against its exact value-returning definition;
 * this fixes the first call's proven r0-last argument ordering.
 * H2: 1332/1308 bytes, 592 differing halfwords, 328 aligned edits; full
 * normalized diff read. Record boundaries and first position-call order
 * now match. Remaining: result is r8 instead of r4/sp+0, extra saved r9,
 * and both cursor searches hoist list/count and use pointer induction,
 * whereas reference reloads the list/count at its test label each time.
 * H3: transfer the documented goto-loop recipe to the cursor search, whose
 * reference enters at a test label before walking the candidates. Predict
 * the repeated list/count reads and indexed low-byte address, eliminating
 * the hoisted pointer walk. Final follow-up; no declaration permutations.
 * H3 result: 1324/1308, 590 differing halfwords, 306 aligned edits; full
 * diff read. Both searches now reload list/count and use the reference's
 * indexed low-byte address and ip found-index role. Row conversion and
 * pointer-cell address lifetimes still differ. Main result remains in r8
 * instead of r4/sp+0; frame is 4/8 and extra r9 is saved. STOP after the
 * corrected model and two follow-ups. Preserve the recovered semantics,
 * record boundaries and goto-loop fact; no blind allocation sweep.
 * 2026-09-29 (alchemy permute scorer): the draft scored 9395 (114
 * register-only, 5 stack-only, 59 operand, 12 reordered, 40 inserted, 29
 * deleted). This body
 * is the permuter's best after a 300-second search (about 40,000 candidates):
 * 7080 (148 register-only, 5 stack-only, 33 operand, 11 reordered,
 * 27 inserted, 23 deleted). Its rewrites are search output, not a
 * reading of the ROM.
 */
#include "TYPES.H"

struct DjinnListTable {
    u16 ids[8][10];
    s8 counts[8];
};

struct DjinnMenuIcon {
    u8 unknown_00[5];
    u8 state;
    u8 unknown_06[6];
    u16 timer;
};

struct DjinnCommandMenu {
    u8 unknown_000[8];
    s32 owner;
    u8 unknown_00c[8];
    struct DjinnMenuIcon *icon;
    u8 unknown_018[4];
    s8 column[2];
    u8 unknown_01e[0x12];
    s32 window;
    u8 unknown_034[0x110];
    u16 row_y[8];
    u8 unknown_154[0x20];
    u16 cursor[2];
    u16 selected[2];
    u8 unknown_17c[8];
    struct DjinnListTable *lists;
    u8 unknown_188[0x80];
    u16 owners[8];
    u8 count;
    u8 owner_count;
    u8 source_owner;
    u8 target_owner;
    u8 unknown_21c[4];
    u16 flags;
    u8 unknown_222[0x32];
    u8 number[2];
    u8 element[2];
};

extern struct DjinnCommandMenu *gMenuWork;
void WaitFrames(s32);
u32 __umodsi3(u32, u32);
void Audio_PlayCue(s32);
void RenderOutput_ClearListFar(s32);
void Owner_RecalculateStatsFar(s32);
s32 Djinn_ActivateFar(s32, s32, s32);
u32 Djinn_DeactivateFar(s32, s32, s32);
s32 Trade_RemoveOfferFar(s32, s32, s32);
u32 *Trade_AddOfferFar(u32, u32, u32);
s32 Djinn_TransferFar(s32, s32, s32, s32);
void Menu_SetFirstObjectRowCoordinates(s32);
s32 Func_080ab314(void);
s32 DjinnMenu_SelectDjinn(s32);
s32 Menu_OpenBackdropScreen(void);
s32 FourObjectMotion_SetSlotPosition(s32, s32, s32, s32);
s32 OwnerAction_RunCompareLoop(s32);
s32 Unnamed_080ae2f4(void);
void DjinnMenu_DrawElementList(struct DjinnListTable *);
s32 Menu_ComputeEntryValues(struct DjinnListTable *);

/* Recover the selected Djinn's position after the transfer changed a row.
 * Low-byte access is intentional: this compares the packed Djinn identity
 * independently of the owner/set bits in the upper byte. */
static __inline__ void RestoreDjinnCursor(struct DjinnCommandMenu *menu)
{
    u16 row;
    s32 found;
    s32 i;
    u8 selected;

    row = __umodsi3(menu->cursor[1], 10);
    found = 0;
    selected = *(u8 *)&menu->selected[0];
    i = 0;
    goto search_test;
search_next:
    i++;
search_test:
    if (i < menu->lists->counts[row]) {
        if (selected != *(u8 *)&menu->lists->ids[row][i])
            goto search_next;
        found = i;
    }
    menu->cursor[0] = row + found * 10;
    menu->icon->state = 1;
}

s32 Unnamed_080aa768(void)
{
    struct DjinnCommandMenu *menu;
    s32 result;
    s32 state;
    s32 ret;
    s32 done;
    s32 i;

    result = 0;
    ret = 0;
    done = 0;
    menu = gMenuWork;
    menu->icon->state = 13;
    menu->icon->timer = 0;
    Menu_OpenBackdropScreen();
    WaitFrames(1);
    state = 2;
    while (1) {
        switch (state) {
            s32 tmp;
            s32 tmp2;
        case 0:
            if (0 > result) {
                ret = -1;
                done = 1;
            }
            state = 2;
            break;
        case 1:
            break;
        case 2:
            Menu_SetFirstObjectRowCoordinates(0);
            FourObjectMotion_SetSlotPosition(1, 0, 200, 0);
            result = DjinnMenu_SelectDjinn(0);
            state = 15;
            if (result == 10)
                break;
            state = 0;
            if (result < 0)
                break;
            menu->cursor[1] = menu->column[0];
            state = 10;
            if (result == 7)
                break;
            state = 3;
            break;
        case 10:
            menu->owner = menu->owners[menu->column[0]];
            menu->source_owner = menu->owners[menu->column[0]];
            result = Unnamed_080ae2f4();
            if (result == -2)
                done = 1;
            state = 2;
            break;
        case 15:
            result = Func_080ab314();
            if (result == -2)
                done = 1;
            state = 2;
            break;
        case 8:
            state = 0;
            if (menu->count == 0)
                break;
            result = DjinnMenu_SelectDjinn(1);
            if (result == -2)
                done = 1;
            state = 4;
            if (result < 0)
                break;
            state = 9;
            break;
        case 3:
            DjinnMenu_DrawElementList(menu->lists);
            Menu_SetFirstObjectRowCoordinates(-8);
            menu->owner = menu->owners[menu->column[0]];
            menu->source_owner = menu->owners[menu->column[0]];
            FourObjectMotion_SetSlotPosition(0, menu->column[0] * 56 + 48, 54, 0);
            tmp2 = DjinnMenu_SelectDjinn(1);
            tmp = tmp2;
            result = tmp;
            i = 0;
            while (i < menu->owner_count) {
                menu->row_y[i] += 8;
                i += 1;
            }
            if (result == -2)
                done = 1;
            if (result < 0) {
                state = 2;
                break;
            }
            if (result == 3 || result == 4 || result == 8 || result == 9)
                menu->target_owner = menu->owners[menu->column[1]];
            if (result < 0)
                state = 2;
            else if (result == 1)
                state = 5;
            else if (result == 2)
                state = 6;
            else if (result == 3) {
                menu->flags = 2;
                state = 7;
            } else if (result == 4) {
                menu->flags = 2;
                state = 9;
            } else if (result == 5)
                state = 11;
            else if (result == 6)
                state = 12;
            else if (result == 8) {
                menu->flags = 2;
                state = 13;
            } else if (result == 9) {
                menu->flags = 2;
                state = 14;
            }
            break;
        case 7:
            result = OwnerAction_RunCompareLoop(1);
            if (result == -2)
                done = 1;
            state = 3;
            if (result < 0)
                break;
            /* fall through */
        case 13:
            Audio_PlayCue(126);
            result = Djinn_TransferFar(menu->source_owner, menu->element[0], menu->number[0], menu->target_owner);
            Owner_RecalculateStatsFar(menu->source_owner);
            Owner_RecalculateStatsFar(menu->target_owner);
            menu->icon->state = 13;
            RenderOutput_ClearListFar(menu->window);
            Menu_ComputeEntryValues(menu->lists);
            RestoreDjinnCursor(menu);
            state = 0;
            break;
        case 6:
            result = OwnerAction_RunCompareLoop(2);
            if (result == -2)
                done = 1;
            state = 3;
            if (result < 0)
                break;
            /* fall through */
        case 12:
            Audio_PlayCue(175);
            Djinn_DeactivateFar(menu->source_owner, menu->element[0], menu->number[0]);
            result = (s32)Trade_AddOfferFar(menu->source_owner, menu->element[0], menu->number[0]);
            goto refresh_owner;
        case 9:
            result = OwnerAction_RunCompareLoop(0);
            if (result == -2)
                done = 1;
            state = 3;
            if (result < 0)
                break;
            /* fall through */
        case 14:
            Audio_PlayCue(126);
            Djinn_TransferFar(menu->source_owner, menu->element[0], menu->number[0], menu->target_owner);
            result = Djinn_TransferFar(menu->target_owner, menu->element[1], menu->number[1], menu->source_owner);
            Owner_RecalculateStatsFar(menu->source_owner);
            Owner_RecalculateStatsFar(menu->target_owner);
            Menu_ComputeEntryValues(menu->lists);
            RestoreDjinnCursor(menu);
            state = 2;
            break;
        case 4:
            if (result == -1) {
                ret = result;
                state = 2;
                break;
            }
            if ((menu->flags & 1) != 0)
                state = 8;
            else if (menu->flags & 2)
                state = 7;
            break;
        case 5:
            result = OwnerAction_RunCompareLoop(3);
            if (result == -2)
                done = 1;
            state = 3;
            if (result < 0)
                break;
            /* fall through */
        case 11:
            Audio_PlayCue(139);
            Djinn_ActivateFar(menu->source_owner, menu->element[0], menu->number[0]);
            result = Trade_RemoveOfferFar(menu->source_owner, menu->element[0], menu->number[0]);
        refresh_owner:
            state = 2;
            Owner_RecalculateStatsFar(menu->source_owner);
            menu->icon->state = 13;
            RenderOutput_ClearListFar(menu->window);
            menu->icon->state = 1;
            break;
        default:
            done = 1;
            break;
        }
        if (done != 0)
            break;
    }
    return ret;
}
