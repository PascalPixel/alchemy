/* 080ad6d4 OwnerAction_RunCompareLoop: structural draft. differ 30 of 1276 (1276 draft instructions); 62 at session 8's start, 62 at session 7's start and end, 64 at session 6's start, 93 at session 5's start, 105 at session 4's start, 289 at session 3's start, 709, and 1136 before shared work structures and real IWRAM calls. Frame, spill slots, mode in fp (as the plain parameter), render in sl, the row computation, the swap preview, the whole help line, the gMenuWork load, the tutorial cases 10 and 11 and the post-loop phase calls now match. Remaining: the prologue's zeros (native gives frame and cursor[] their own 0 where this shares anim's), the auto-advance case's timer store, and the tile bracket calls' argument order (see Open). No function counts yet.
 * Stubborn (2026-10-06): re-scored differ 927; not first-try. Score+parked from sun100 queue-3.
 * Trial (2026-10-06): the page dispatch as a switch with the default (paired entry action) first, then case 1, then case 0, matches the native block order (default, page 1, page 0): 927 -> 892.
 * Trial (2026-10-06): dirty as u32 gives the native lsrs for dirty >> 1; differ stays 892.
 * Trial (2026-10-06): swap = 1 as the first statement of the dirty block, where native sets r8 before the mode test: 892 -> 814.
 * Trial (2026-10-06): attr as u32 loads pair_entries with ldrh where u16 gave ldrsh and a zero-extend: 814 -> 709 (s32 gives 721).
 * Trial (2026-10-06): input_edge and input_held as u32 leave 709 unchanged.
 * Trial (2026-10-06): mode fixed in fp (FAKEMATCH) gives the native 136-byte frame and array offsets: 709 -> 716; with anim, dirty, frame, page declared in that order (spill slots 100, 96, 92, 88 as native): 662.
 * Trial (2026-10-06): input_edge and input_held declared in the loop block take the lowest spill slots (36, 32) as native: 662 -> 608.
 * Trial (2026-10-06): first_flag declared after second_index (native slots 80, 76, 72, 68): 608 -> 590.
 * Trial (2026-10-06): a work local in each tutorial case instead of one shared: scene takes r6 as native: 590 -> 588.
 * Trial (2026-10-06): row for the help line, apart from the loop's step: row takes r5 as native: 588 -> 568.
 * Trial (2026-10-06): the two stores to menu_busy after the loop through gWindowWork[0] directly, leaving 0 in sl as native: 568 -> 549.
 * Trial (2026-10-06): y = row * 8 computed before the help-line draw (permuter hint, as a temporary): the loop allocates as native: 549 -> 458.
 * Trial (2026-10-06): j for the owner_y loop apart from the tile loop's i: the counter takes r2 as native: 458 -> 446. Cursor zeroed before anim, dirty, page and frame shares one zero everywhere (444), unlike native; page, frame, dirty, anim order gives 453.
 * Trial (2026-10-06): one id for both owners in the swap preview, where first_id and second_id were two: id keeps r6 and unit r7 as native: 446 -> 408. Reloading attr once after the first 0x8000 test instead of inside it gives 411; row and y declared last leave 408.
 * Trial (2026-10-06, session 2): the swap preview's last three Iwram_CopyWords through a local function pointer: gcse propagates the constant back and nothing changes (408); the same pointer fixed in r5 (FAKEMATCH, the 332/address tie below) matches the whole swap preview: 408 -> 293.
 * Trial (2026-10-06, session 2): 0 for the mode-0 tilemap entry instead of mode: 295. 0x8000 fixed in r8 and the shared 0 in sl (two FAKEMATCH variables): 224 is rebuilt at each use as native, but attr moves to r1 and the copies around each and change: 369 (mask & attr: 369; zero assigned inside the argument: 376). The row computation differs because mode, a hard register here, is copied where native ties it: native eors r2 (mode copy) with 1, this copies 1 and eors with a copy of fp.
 * Trial (2026-10-06, session 2): y = row * 8 before the attr load: 546. row = mode, then row = (row == 1): 426. row = (1 == mode): 293, folded to the same tree. A 5-minute permuter run (2 jobs, 5,445 candidates) randomizing only the help line from row to the 0xf296 entry found nothing below 293.
 * Trial (2026-10-06, session 2): with the copy routine in r5, mode as the plain parameter wins fp by itself, so its fixed-fp FAKEMATCH is gone, and the row computation ties mode as native: 293 -> 289. On top of that, 0x8000 fixed in r8 and 0 in sl give 285 (the pools then line up) but attr still takes r1 and the copies stay wrong; not kept for 4. cursor[0] = cursor[1] = 0 leaves 289 and still reloads anim's 0 where native has its own zero in r7.
 * Trial (2026-10-06, session 3): the help line's ands copied attr where native copies the mask (mov r3, r8; movs r3, #224; movs r3, #31). In regmove the first commutative operand that stays live is copied into the result, so the copy goes to the constant only when attr is a hard register: attr fixed in r2 (FAKEMATCH) gives 289 -> 187. Constant-first source order (0xe0 & attr) folds back: 289. The tilemap entries read state->pair_entries[x] directly, where native loads it into r3 and not attr's r2: 175. row * 8 as the argument, not a y temporary: 172. 224 and the shared 0 then tie at priority 769 (7 refs over 182 insns, 4 over 104) and 224 wins by pseudo number; 0 fixed in sl (FAKEMATCH) and set inside the first tilemap call's argument as native: 127 (zero = 0 as a statement before the call: 139, before the draw: 154, at the row computation: 172). The 0x8000 test through state->pair_entries[0] directly, without attr: 259 against 289 (the no-call path then reloads).
 * Trial (2026-10-06, session 3): the prologue assignments as anim, dirty, frame, cursor[0], page, then state->cursor->active = 13 and cursor[1] (permuter hint, 5 minutes over the prologue): gMenuWork is loaded into r3 before the stack adjustment and state->cursor is reached through mov r4, r9 as native, and cursor[0] takes a fresh 0: 127 -> 105. All 24 orders of anim, dirty, page and frame alone: 127 to 135; frame = anim and state loaded last: 127; all 240 orders of the five with cursor[1] last or right after cursor[0]: 105 at best (also anim, frame, dirty, cursor[0], page); cursor[0] = cursor[1] = 0 or cursor[1] = cursor[0] = 0 in any of 120 orders each: 119 at best.
 * Trial (2026-10-06, session 4): expand_call loads a call argument into its pseudo before the function address when the argument is a constant costing more than 2 (0x060052c0, 0x44444444, 0x200), but loads a variable's register after it; native loads all three after the IWRAM address. A tile variable for 0x060052c0 shared by the copy and the fill: 105 -> 103; a fill variable for 0x44444444 as well: both calls match, 97. A size variable for the tutorial buffer's 0x200 in both copies of each case: 93 (the allocation through size or with 0x200, and size set before or after it: 93). Cursor[] zeroed through an empty asm output (none) after the four scalars: CSE gives the asm anim's register as input, 127.
 * Trial (2026-10-06, session 4): the cursor[] zero through a variable fixed in r7 (as native) after frame, or after active = 13: both stores take r7 as native, but gMenuWork is no longer loaded before the stack adjustment and frame still inherits anim's r0: 109 either way. A bracket variable for 0xf128 and 0xf129 after the tile loop: 93, unchanged. Reload's find_equiv_reg (reload.c) gives frame anim's r0 because anim's own insn, rewritten in place to (set r0 0), is the nearest earlier set of 0; native's frame would need that set hidden (an intervening r0 set or a label).
 * Trial (2026-10-06, session 4): the same cause explains the swap preview's 332: a size local (sizeof(*unit_copy)) for the allocation and the four copies, called directly, compiles identically to the copy routine fixed in r5, so that FAKEMATCH is gone (93). A 5-minute permuter run (2 jobs, 253 candidates on a shared machine) randomizing tutorial case 10 found nothing below 93; one work local shared by both tutorial cases, or size set right before each first copy: 93. A top local for the tile calls' -1 row: 119.
 * Trial (2026-10-07, session 5): message ids are linker symbols (W8), and a symbol's value always comes from the pool where a K constant is split into a shifted byte; the R-button hints 2976 and 2977 (DE, ES, FR, IT 2899 and 2900, JA 3371 and 3372) are named MsgDjinnPsyHelp and MsgDjinnStatusHelp in all six PO files and drawn through them: the 0xba0 pool load matches and the pool and branch offsets after it line up: 93 -> 64. The other messages here (0xba4 to 0xba7, 0xbc1, 0xc42, 0xc43) are still numbers and differ by edition, so they need names before adoption in other editions.
 * Trial (2026-10-07, session 5): the auto-advance case with input_edge set before input_held, or input_held = input_edge = 1: 66; timer and step before the inputs: 70. frame or anim as u32: 64; page as u32: 70. After the loop, a phase local (0) for both SetSlotPhase calls: 80 (it takes sl); step reused for it: 315. The bracket column as width - 3 - count[page] or width - (count[page] + 3): 64; the loop's tile variable at block scope carrying 0xf128 and 0xf129: 64 (CSE propagates the constant back). render->dirty = (2 << (list->y >> 2)) | render->dirty: 76; list->y / 4: 64. A 5-minute heat-aimed permuter run (2 jobs, 2,515 candidates) lowered the weighted score but not the count: best differ 75.
 * Diagnosis (2026-10-07, session 5, sched2 dump): the post-loop SetSlotPhase(0, 0) sets tie in the scheduler (priority 186, three dependents each), so the earlier insn wins: native's r1 first means its r1 set came first in the RTL. In tutorial case 10 everything after UiText_OpenMessageWindowFar is ready at once; the active = 1 chain (mov r4, r9; ldr; strb) outranks the work copy, so the copy lands after the store, and movs r0 then waits for it (anti dependence), which puts movs r1 first; native copies first. In the prologue frame's set gets r0 as its output reload register and the redundant set is deleted; cursor[0]'s constant reload takes r2 (native: frame r2, cursor[] r7).
 * Trial (2026-10-07, session 6): the bracket calls' -1 row through a block-local top: a constant costing more than 2 is precomputed into a pseudo ahead of the argument loads (so the second bracket call copied r3 before r0), while a variable loads in argument order: r0 is copied before r3 as native: 64 -> 62. row reused for it instead: 116 (it joins the help line's row and swaps r5/r7 there). A bracket variable for 0xf128 and 0xf129 on top: 62. Iwram_CopyWords typed void (an experiment only, a local redefinition): 74. A 5-minute heat-aimed permuter run (2 jobs) on 62: weighted 1570 at best (against the draft's 1860) at differ 75; the lowest differ was 71.
 * Diagnosis (2026-10-07, session 6, sched2 dump): the tutorial copies are a scheduler tie: the IWRAM address load, the r0 copy and the r1 copy all have priority 68 (case 10's first copy) and 3, 2 and 3 dependents, so LUID decides and the address (loaded by prepare_call_address before the argument registers) goes first; case 11's second copy matches only because its r1 copy has a fourth dependent (the WaitFrames(1) that follows). The auto-advance case: r3 = 0 (priority 7, 3 dependents) beats the 0x212c load (7, 2); native schedules it after both input stores. Prologue: native's registers are reload rotation over a spill set that holds r7: anim r0, dirty r1, frame r2, page r3, state's reload r4, cursor's 0 r7 (cursor[1] inherits it); here frame inherits anim's r0 and the spill set is r0 to r4 only.
 * Trial (2026-10-07, session 7): the bracket column through a block local set before the call (before top = -1: 660; after it: 658; the loop's allocation changes). 0xf128 through a block local set after top: 62, unchanged (CSE propagates it back into the argument load). The gMenuWork load moved after anim, after dirty or after frame: 72, 70, 70. anim = frame = 0 in place of the two statements: 85; frame = anim = 0: 62 (frame still inherits anim's r0).
 * Diagnosis (2026-10-07, session 7, greg and sched2 dumps): bracket call: its five argument insns (0xf128 pool load, r2 -= 3, r5 = 0, r0 = list, r3 = top) all have priority 41 and two dependents, so LUID decides; expand_call precomputes the costly 0xf128 ahead of every other argument, so it goes first; native's order (r5, r0, r3, r1, r2 -= 3) means its 0xf128 load and final subtract came after the r0 and r3 loads in its RTL. Post-loop: one block runs from the loop exit to the epilogue; r0 = 0 and r1 = 0 each have three dependents (the call, the next set of the same register, the epilogue's use of sp), so LUID puts r0 first. Prologue: frame = 0 is an output reload from a simple constant move, so choose_reload_regs searches find_equiv_reg for the constant (reload1.c, search_equiv from SET_SRC) and inherits anim's r0; native's registers are plain rotation (anim r0, dirty r1, frame r2, page r3, r4 for state, r7 for cursor's 0), so its frame found no earlier set of 0 and its spill set includes r7.
 * Trial (2026-10-07, session 8): after the loop, the phase argument held in r1 (FAKEMATCH, set to 0 before each call): both calls set r1 first as native: 62 -> 58. An empty asm (a full sched2 barrier: haifa-sched gives a traditional asm every register and memory dependence) right after each tutorial UiText_OpenMessageWindowFar (FAKEMATCH): the work copy stays right after the call and the tutorial copies and SlideCursor arguments follow in native order: 58 -> 50 (case 10) -> 40 (both cases). The same barrier between each case's tutorial_step store and active = 13 keeps the store before the mov from r9 as native, and the finalize call's r0 copy then also comes first: 40 -> 30. A barrier after input_edge = 1 in the auto-advance case: 28, but the 0x212c pool load then follows the input stores where native loads it before them (r3 = 0 now matches), so not kept. top = -1 before the tile loop instead of after it: 86. The bracket column as list->width - (count[page] + 3) or -3 - count[page] + list->width: 58, unchanged. frame = 0 moved after active = 13, after page = 2, or after cursor[1]: 80, 80, 81.
 * Open (2026-10-07, session 8, 30 remain): the prologue zeros (16); the auto-advance case schedules r3 = 0 before the input stores (native: the 0x212c load first, r3 = 0 after both stores; 4); the bracket calls' 0xf128 load and r2 -= 3 come before the r0 and r3 copies, the second call copies r3 before r0, and the dirty offset's pool load sits after ldrh (10). All are register choice or order.
 * Trial (2026-10-06, parallel session, merged 2026-10-07): the routine address held in r5 from the second copy on (fixed-register pointer, FAKEMATCH): the swap preview allocates as native, 332 rematerialized at each call: 408 -> 293. A plain function-pointer local set before the call (or inside the call expression) is set before the 332 and lives 92 insns against 88, so 332 still wins: 408. (Superseded by session 4's size local.)
 * Trial (2026-10-06, parallel session): 0x8000 fixed in r8 for the help line: 293 -> 291 (not kept: 224 then takes sl and the shape is unchanged); with the shared 0 also fixed in sl, 224 is rematerialized as native but the pools move: 374. An empty volatile asm between the two help-line statements (flushing CSE): 494. frame = 0 before page = 2: 293, identical code. ((attr >> 5) & 7) for the element: 272, but only because the pools shift; native ands 224 before the shift, so not kept.
 * Trial (2026-10-06, parallel session): the zeros. Native stores anim, dirty, page, frame from r0, r1, r3, r2 (frame does not reuse anim's 0) and the cursor pair from a fresh r7; here frame reuses r0 and the cursor pair reloads anim's slot. cursor[1] = cursor[0] = 0: 293; anim = 0 last: 297; s32 cursor[2] = { 0, 0 }: 593. A 4-minute permuter run over the order of the eight entry statements: best 286 (cursor[0] first, anim after the cursor write), whose store order is further from native's; not kept.
 * Trial (2026-10-06, parallel session): the page label 0xba0 (message 2976) as a linker symbol (named MsgDjinnPsynergyPage there, MsgDjinnPsyHelp in session 5, which this draft keeps): native's pool load of a shiftable constant is a symbol address, not 186<<4: 293 -> 276.
 * Trial (2026-10-06, parallel session): help line. 0x8000 fixed in r8 and the 0 in sl again: 376; 224 then goes unallocated, but regmove has already copied attr at every AND because the shared 224 never dies there, so it is still not native. Regmove's backward pass ties the operand that dies after scanning back to its set; native's tie of 224 and 31 needs both to die at their ANDs, so they were never shared when regmove ran. Passing 0 for mode in block 2: 295. (attr >> 5) & 7: 272 again, the 7 shared in r8 instead. The tile statements reading pair_entries directly, without attr: 347. Block 3 testing the attr of block 1 without its reload: 422. attr declared in each block: 293, identical code. Within block 1 both 224s sit in one basic block, so CSE always shares them unless the constant differs in mode or value; none of the source forms tried changes that. (Session 3's attr fixed in r2 solved the help line.)
 * Open (2026-10-06, session 4; 0xba0 solved in session 5, 64 remain): 93 instructions remained at session 4's end, all register choice or instruction order. Prologue: native gives anim, dirty, page and frame the registers r0, r1, r3, r2 and zeroes both cursor[] entries from its own 0 in r7; here frame's reload inherits anim's 0 in r0 and cursor[1] stores anim's 0 reloaded from its slot (CSE puts anim's register in for the constant). 0xba0: native loads it from the pool, which a K constant reaches only when the movsi split does not run on it; this splits it to 186<<4. Order only: the tutorial's work = UiText_OpenMessageWindowFar copy (native right after the call), the tutorial copies' argument moves (native mov r1 before the address load and the size's shift last), the tile loop's list->width - count[page] - 3 and the two SetSlotPhase(x, 0) after the loop (native sets r1 first: arguments load in order, so r1 first means its argument was precomputed, a value costing more than 2 that later folds to 0, or the scheduler had a reason this source does not give it). */
#include "MEM.H"
#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "DJINN_MENU.H"
#include "BATTLE_UNIT.H"
#include "GAME_STATE.H"
#include "WINDOW.H"
#include "CALLBACK_SCHEDULER.H"
#include "IWRAM_CALL.H"
extern struct DjinnMenuWork *gMenuWork;
extern u8 MsgDjinnPsyHelp[];
extern u8 MsgDjinnStatusHelp[];

extern u8 Menu_BackdropFrameTile[];
extern volatile u32 gKeyState;
extern volatile u32 gKeysRepeat;

struct BtlUnit *UnitGetFar(s32);
void WaitFrames(s32);
void AudioCommand_PlayFar(s32);
s32 Menu_GetModuloOfSum(s32, s32);
void UnitCalcFar(s32);

void RenderOutput_RedrawSavedRectFar(s32);
void RenderOutput_PrepareForRedrawFar(s32);
s32 UiWork_FinalizeFar(s32, s32);
void UiWindow_CloseIfOpen(s32 *, s32);
s32 UiWindow_UpdateOrCreate(s32 *, s32, s32, s32, s32, s32);
void UiWindow_SetTilemapEntryFar(s32, s32, s32, s32, s32);
void UiText_DrawCharacterAtOffsetFar(s32, s32, s32, s32);
void UiText_DrawQuantity(s32, s32);
s32 UiWork_SetParamNibbleFar(s32);
s32 UiText_OpenMessageWindowFar(s32, s32, s32, s32);
s32 UiWork_IsCompleteFar(void);

void Runtime_SetMainState19(void);
void Menu_RunPairedEntryAction(s32, s32);
void UiMenu_PositionCursor(s32, s32);
s32 OwnerAction_DiffSlots(struct OwnerActionSlot *, struct OwnerActionSlot *, u16 *, s32 *, s32 *);
void FourObjectMotion_SetSlotPhase(s32, s32);
s32 FourObjectMotion_SetSlotPosition(s32, s32, s32, s32);

void Runtime_BumpFree(void *);
s32 Trig_Sin(s32);
void UiWindow_ClearInteriorTilesFar(s32, s32, s32, s32, s32);
void RenderOutput_ClearListFar(s32);
void UiWindow_MarkVisibleTileAttributesFar(void);
void Djinn_AddToOwnerFar(s32, s32, s32);
void Djinn_ActivateFar(s32, s32, s32);
void Djinn_DeactivateFar(s32, s32, s32);
void Menu_UpdateEntryObjectTransforms(void);
void UiMenu_SlideCursor(s32, s32);
s32 DjinnMenu_DrawStatPreview(s32, s32, s32, s32, s32, s32, s32, s32, s32);

s32 OwnerAction_RunCompareLoop(u32 mode)
{
    struct DjinnMenuWork *state;
    struct DjinnMenuBackdrop *scene;
    struct UiRenderWork *render;
    struct UiWindow *list;
    s32 count[2];
    s32 cursor[2];
    s32 anim;
    u32 dirty;
    s32 frame;
    s32 page;
    s32 entry;
    s32 swap;
    s32 step;
    s32 row;
    s32 result;
    /* FAKEMATCH: regmove copies a live pseudo operand of each help-line and into its result (adds r3, r1, #0); native copies the mask (mov r3, r8; movs r3, #224; movs r3, #31), which regmove leaves to reload only when the other operand is a hard register. attr in r2 as native. */
    register u32 attr asm("r2");
    /* FAKEMATCH: the shared 0 and the element mask 224 tie at allocation priority 769 (4 refs over 104 insns, 7 over 182) and 224 wins by pseudo number; native keeps 0 in sl and rebuilds 224 at each use. */
    register s32 zero asm("sl");
    s32 i;
    s32 j;

    state = gMenuWork;
    anim = 0;
    dirty = 1;
    frame = 0;
    cursor[0] = 0;
    page = 2;
    state->cursor->active = 13;
    cursor[1] = 0;

    for (j = 3; j >= 0; j--) {
        state->owner_y[j] = 0xc8;
    }
    state->owner_x[state->selected_column[1]] = 0x78;
    state->owner_y[state->selected_column[1]] = 0x20;
    state->owner_x[state->selected_column[0]] = 0x10;
    state->owner_y[state->selected_column[0]] = 0x20;

    RenderOutput_ClearListFar(state->list_window);
    WaitFrames(1);
    UiWindow_CloseIfOpen(&state->help_window, 1);
    UiWindow_UpdateOrCreate(&state->help_window, 0, 0, 30, 5, 2);

    count[0] = 1;
    count[1] = 1;

    if (mode <= 1) {
        u16 *diff_work;
        struct BtlUnit *unit_copy;
        struct BtlUnit *unit;
        s32 id;
        s32 first_element;
        s32 first_index;
        s32 second_element;
        s32 second_index;
        s32 first_flag;
        s32 second_flag;
        s32 out_a;
        s32 out_b;
        s32 size;

        diff_work = Runtime_BumpAllocateAlternatePool(96);
        size = sizeof(*unit_copy);
        unit_copy = Runtime_BumpAllocateAlternatePool(size);
        second_element = 0;
        second_index = 0;
        id = state->shown_owner[0];
        first_element = state->pair_element[0];
        first_index = state->pair_index[0] & 31;
        first_flag = (u16)(state->pair_entries[0] & 0x8000);

        unit = UnitGetFar(id);
        Iwram_CopyWords(unit_copy, unit, size);
        Djinn_DeactivateFar(id, first_element, first_index);
        if (mode == 0) {
            second_element = state->pair_element[1];
            second_index = state->pair_index[1] & 31;
            second_flag = (u16)(state->pair_entries[1] & 0x8000);
            Djinn_AddToOwnerFar(id, second_element, second_index);
            if (second_flag != 0) {
                Djinn_ActivateFar(id, second_element, second_index);
            }
        }
        UnitCalcFar(id);
        count[0] = OwnerAction_DiffSlots(
            unit_copy->action_slots, unit->action_slots, diff_work, &out_b, &out_a);
        Iwram_CopyWords(unit, unit_copy, size);

        id = state->shown_owner[1];
        unit = UnitGetFar(id);
        Iwram_CopyWords(unit_copy, unit, size);
        if (mode == 0) {
            Djinn_DeactivateFar(id, second_element, second_index);
        }
        Djinn_AddToOwnerFar(id, first_element, first_index);
        if (first_flag != 0) {
            Djinn_ActivateFar(id, first_element, first_index);
        }
        UnitCalcFar(id);
        count[1] = OwnerAction_DiffSlots(
            unit_copy->action_slots, unit->action_slots, diff_work, &out_b, &out_a);
        Iwram_CopyWords(unit, unit_copy, size);
        Runtime_BumpFree(unit_copy);
        Runtime_BumpFree(diff_work);
    } else if (mode - 2 <= 1) {
        u16 *diff_work;
        struct BtlUnit *unit_copy;
        struct BtlUnit *unit;
        s32 first_id;
        s32 first_element;
        s32 first_index;
        s32 apply_second;
        s32 out_a;
        s32 out_b;

        diff_work = Runtime_BumpAllocateAlternatePool(96);
        unit_copy = Runtime_BumpAllocateAlternatePool(sizeof(*unit_copy));
        first_id = state->shown_owner[0];
        first_element = state->pair_element[0];
        first_index = state->pair_index[0] & 31;
        apply_second = (mode == 3);

        unit = UnitGetFar(first_id);
        Iwram_CopyWords(unit_copy, unit, sizeof(*unit_copy));
        Djinn_DeactivateFar(first_id, first_element, first_index);
        if (apply_second != 0) {
            Djinn_ActivateFar(first_id, first_element, first_index);
        }
        UnitCalcFar(first_id);
        count[0] = OwnerAction_DiffSlots(
            unit_copy->action_slots, unit->action_slots, diff_work, &out_b, &out_a);
        Iwram_CopyWords(unit, unit_copy, sizeof(*unit_copy));
        count[1] = count[0];
        page = 0;
        Runtime_BumpFree(unit_copy);
        Runtime_BumpFree(diff_work);
    }

    count[0] = ((count[0] - 1) / 5) + 1;
    if (count[0] == 0) {
        count[0] = 1;
    }
    count[1] = ((count[1] - 1) / 5) + 1;
    if (count[1] == 0) {
        count[1] = 1;
    }

    UiWindow_UpdateOrCreate(&state->window, 0, 5, 15, 15, 2);
    UiWindow_UpdateOrCreate(&state->second_window, 15, 5, 15, 15, 2);
    RenderOutput_RedrawSavedRectFar(state->help_window);

    if (mode == 2) {
        UiText_DrawCharacterAtOffsetFar(0xba5, state->help_window, 96, 0);
        UiText_DrawCharacterAtOffsetFar(0xbc1, state->help_window, 96, 16);
    } else if (mode == 3) {
        UiText_DrawCharacterAtOffsetFar(0xba4, state->help_window, 96, 0);
        UiText_DrawCharacterAtOffsetFar(0xbc1, state->help_window, 96, 16);
    } else if (mode == 0) {
        UiText_DrawCharacterAtOffsetFar(0xba7, state->help_window, 128, 0);
        UiText_DrawCharacterAtOffsetFar(0xbc1, state->help_window, 128, 8);
    } else {
        UiText_DrawCharacterAtOffsetFar(0xba6, state->help_window, 128, 0);
        UiText_DrawCharacterAtOffsetFar(0xbc1, state->help_window, 128, 8);
    }

    row = (mode == 1);
    attr = state->pair_entries[0];
    if ((attr & 0x8000) == 0) {
        UiWork_SetParamNibbleFar(2);
        attr = state->pair_entries[0];
    }
    UiText_DrawCharacterAtOffsetFar(((attr & 0xe0) >> 5) * 20 + (attr & 31) + 0x45f,
        state->help_window, 48, row * 8);
    UiWindow_SetTilemapEntryFar(
        state->help_window, ((state->pair_entries[0] & 0xe0) >> 5) + 0x5001, 5, row, zero = 0);
    UiWork_SetParamNibbleFar(15);

    if (mode == 0) {
        attr = state->pair_entries[1];
        if ((attr & 0x8000) == 0) {
            UiWork_SetParamNibbleFar(2);
            attr = state->pair_entries[1];
        }
        UiText_DrawCharacterAtOffsetFar(((attr & 0xe0) >> 5) * 20 + (attr & 31) + 0x45f,
            state->help_window, 48, 16);
        UiWindow_SetTilemapEntryFar(
            state->help_window, ((state->pair_entries[1] & 0xe0) >> 5) + 0x5001, 5, 2, mode);
        UiWork_SetParamNibbleFar(15);
    } else if (mode - 2 <= 1) {
        attr = state->pair_entries[0];
        if ((attr & 0x8000) != 0) {
            UiWork_SetParamNibbleFar(2);
            attr = state->pair_entries[0];
        }
        UiText_DrawCharacterAtOffsetFar(((attr & 0xe0) >> 5) * 20 + (attr & 31) + 0x45f,
            state->help_window, 48, 16);
        UiWindow_SetTilemapEntryFar(
            state->help_window, ((state->pair_entries[0] & 0xe0) >> 5) + 0x5001, 5, 2, zero);
        UiWork_SetParamNibbleFar(15);
        UiWindow_SetTilemapEntryFar(state->help_window, 0xf296, 7, 1, zero);
    }

    for (;;) {
        s32 input_edge;
        s32 input_held;

        render = (struct UiRenderWork *)gWindowWork[0];
        scene = state->backdrop;
        input_edge = gKeyState;
        input_held = gKeysRepeat;

        if (scene->tutorial_step != 0) {
            s16 *saved;
            s32 size;

            size = 0x200;
            saved = Runtime_BumpAllocateAlternatePool(size);
            input_edge = 0;
            input_held = 0;
            scene->tutorial_timer += 1;

            switch (scene->tutorial_step) {
            case 5:
            case 12:
            case 23:
                if (scene->tutorial_timer == 100) {
                    input_held = 1;
                    input_edge = 1;
                    scene->tutorial_timer = 0;
                    scene->tutorial_step += 1;
                }
                break;
            case 10:
                if (scene->tutorial_timer == 60) {
                    s32 work;

                    Iwram_CopyWords(saved, render, size);
                    work = UiText_OpenMessageWindowFar(0xc43, 8, 0, 1);
                    /* FAKEMATCH: the active = 1 chain (mov r4, r9; ldr; strb) outranks the work copy in sched2, so the copy landed after the store; an empty asm is a scheduling barrier that keeps the copy right after the call as native. */
                    asm("");
                    state->cursor->active = 1;
                    UiMenu_SlideCursor(2, 96);
                    gGameState.message_speed = 1;
                    while (UiWork_IsCompleteFar() == 0) {
                        WaitFrames(1);
                    }
                    UiWork_FinalizeFar(work, 1);
                    Iwram_CopyWords(render, saved, size);
                    UiWindow_MarkVisibleTileAttributesFar();
                    render->dirty = 1;
                    dirty = 1;
                    scene->tutorial_timer = 0;
                    scene->tutorial_step = 11;
                    /* FAKEMATCH: the mov from r9 for active = 13 outranks the step store in sched2; an empty asm barrier keeps the store first as native. */
                    asm("");
                    state->cursor->active = 13;
                }
                break;
            case 11:
                if (scene->tutorial_timer == 60) {
                    s32 work;

                    Iwram_CopyWords(saved, render, size);
                    work = UiText_OpenMessageWindowFar(0xc42, 8, 0, 1);
                    /* FAKEMATCH: the active = 1 chain (mov r4, r9; ldr; strb) outranks the work copy in sched2, so the copy landed after the store; an empty asm is a scheduling barrier that keeps the copy right after the call as native. */
                    asm("");
                    state->cursor->active = 1;
                    UiMenu_SlideCursor(106, 56);
                    gGameState.message_speed = 1;
                    while (UiWork_IsCompleteFar() == 0) {
                        WaitFrames(1);
                    }
                    UiWork_FinalizeFar(work, 1);
                    Iwram_CopyWords(render, saved, size);
                    UiWindow_MarkVisibleTileAttributesFar();
                    render->dirty = 1;
                    WaitFrames(1);
                    dirty = 1;
                    scene->tutorial_timer = 0;
                    scene->tutorial_step = 12;
                    /* FAKEMATCH: the mov from r9 for active = 13 outranks the step store in sched2; an empty asm barrier keeps the store first as native. */
                    asm("");
                    state->cursor->active = 13;
                }
                break;
            default:
                break;
            }
            Runtime_BumpFree(saved);
        }

        if (dirty != 0) {
            swap = 1;
            if (mode <= 1) {
                UiWindow_ClearInteriorTilesFar(state->help_window, 128, 16, 224, 24);
                if (page == 1) {
                    UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnStatusHelp, state->help_window, 128, 16);
                } else {
                    UiText_DrawQuantity(
                        state->shown_owner[page != 2], 1);
                    UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnPsyHelp, state->help_window, 128, 16);
                }
            }
            render->menu_busy = 1;
            RenderOutput_PrepareForRedrawFar(state->window);
            RenderOutput_PrepareForRedrawFar(state->second_window);
            if ((dirty >> 1) != 0) {
                WaitFrames(1);
            }

            if (mode == 3) {
                entry = 0;
                swap = 0;
            } else if (mode == 2) {
                entry = 1;
                swap = 0;
            } else {
                entry = 2;
            }

            switch (page) {
            default:
                Menu_RunPairedEntryAction(mode, 0);
                break;
            case 1:
                if (mode == 1) {
                    entry = 4;
                }
                DjinnMenu_DrawStatPreview(state->window, 0, 0,
                    state->shown_owner[1], page, 0, entry, 0, 0);
                DjinnMenu_DrawStatPreview(state->second_window, 0, 0,
                    state->shown_owner[1], page, 0, entry,
                    cursor[1] + 1, 0);
                break;
            case 0:
                if (mode == 1) {
                    entry = 1;
                    swap = 0;
                }
                DjinnMenu_DrawStatPreview(state->window, 0, 0,
                    state->pair_owner[0], page, swap, entry, page, 1);
                DjinnMenu_DrawStatPreview(state->second_window, 0, 0,
                    state->pair_owner[0], page, swap, entry,
                    cursor[0] + 1, 1);
                break;
            }
            render->menu_busy = 0;
        }

        if (page <= 1) {
            list = (struct UiWindow *)state->second_window;
            if (count[page] > 1) {
                s32 top;

                for (i = 0; i < count[page]; i++) {
                    s32 tile;

                    tile = 0xf031 + i;
                    if (i > 9) {
                        tile = 0xf030;
                    }
                    if (i == cursor[page]) {
                        tile -= 0x1000;
                    }
                    UiWindow_SetTilemapEntryFar((s32)list, tile,
                        list->width - count[page] + i - 2,
                        -1, 0);
                }
                top = -1;
                UiWindow_SetTilemapEntryFar((s32)list, 0xf128,
                    list->width - count[page] - 3, top, 0);
                UiWindow_SetTilemapEntryFar((s32)list, 0xf129,
                    list->width - 2, top, 0);
                render->dirty |= 2 << (list->y >> 2);
            }
        }

        frame = frame + 1;
        step = (frame % 60) - 5;
        if (step < 0) {
            step = 0;
        }
        if (step > 29) {
            step = 29;
        }
        FourObjectMotion_SetSlotPhase(0, (s32)0xffff4000);
        FourObjectMotion_SetSlotPhase(1, (s32)0xffff4000);

        if (mode <= 1) {
            s32 rise;
            s32 slide;

            rise = -Iwram_MulQ16(16, Trig_Sin((step % 30) * 0x444));
            if (rise < -6) {
                rise = -6;
            }
            if (rise > 12) {
                rise = 12;
            }
            slide = (step % 35) * 2;
            FourObjectMotion_SetSlotPosition(0, slide + 34, rise + 20, 0);
            if (mode == 0) {
                FourObjectMotion_SetSlotPosition(1, 99 - slide, 36 - rise, 0);
            }
        } else {
            FourObjectMotion_SetSlotPosition(0, 32, 30, 0);
        }

        if (dirty != 0) {
            dirty = 0;
            anim = Menu_GetModuloOfSum(anim, 2);
        }
        UiMenu_PositionCursor(anim * 56 + 80, 16);

        if ((frame & 3) == 0) {
            void *tile;
            u32 fill;

            tile = (void *)0x060052c0;
            fill = 0x44444444;
            if ((frame & 4) != 0) {
                Iwram_CopyWords(tile, Menu_BackdropFrameTile, 32);
            } else {
                Iwram_FillWords(tile, 32, fill);
            }
        }

        if ((input_edge & 1) != 0) {
            if (anim == 0) {
                result = 1;
                break;
            }
            AudioCommand_PlayFar(113);
            result = -1;
            break;
        } else if ((input_edge & 8) != 0) {
            AudioCommand_PlayFar(113);
            result = -2;
            break;
        } else if ((input_edge & 2) != 0) {
            AudioCommand_PlayFar(113);
            result = -1;
            break;
        } else if ((input_edge & 0x100) != 0) {
            if (mode <= 1) {
                page = ((page + 1) % 3);
                dirty = 2;
                AudioCommand_PlayFar(111);
            }
        } else if ((input_held & 32) != 0) {
            if (page <= 1) {
                cursor[page] = cursor[page] - 1;
                cursor[page] = Menu_GetModuloOfSum(cursor[page], count[page]);
                AudioCommand_PlayFar(111);
                Runtime_SetMainState19();
                dirty = 1;
            }
        } else if ((input_held & 16) != 0) {
            if (page <= 1) {
                AudioCommand_PlayFar(111);
                Runtime_SetMainState19();
                cursor[page] = cursor[page] + 1;
                dirty = 1;
                cursor[page] = Menu_GetModuloOfSum(cursor[page], count[page]);
            }
        }
        WaitFrames(1);
    }

    {
        /* FAKEMATCH: the two sets of each call tie in sched2 (priority 186, three dependents each) and expand_call emits r0 first, so LUID puts r0 first; native sets r1 first. The phase held in r1 is set before each call. */
        register s32 phase asm("r1");

        phase = 0;
        FourObjectMotion_SetSlotPhase(0, phase);
        phase = 0;
        FourObjectMotion_SetSlotPhase(1, phase);
    }
    Scheduler_AddOrUpdateCallback((s32)Menu_UpdateEntryObjectTransforms, 0xc80);

    ((struct UiRenderWork *)gWindowWork[0])->menu_busy = 1;
    UiWindow_CloseIfOpen(&state->help_window, 1);
    WaitFrames(1);
    UiWindow_UpdateOrCreate(&state->help_window, 13, 0, 17, 5, 2);
    UiWindow_CloseIfOpen(&state->window, 1);
    UiWindow_CloseIfOpen(&state->second_window, 1);
    RenderOutput_RedrawSavedRectFar(state->list_window);
    RenderOutput_RedrawSavedRectFar(state->unknown_window_028);
    RenderOutput_RedrawSavedRectFar(state->status_window);
    ((struct UiRenderWork *)gWindowWork[0])->menu_busy = 0;
    WaitFrames(1);
    return result;
}
