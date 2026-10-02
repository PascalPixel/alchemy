#include "TYPES.H"
#include "RUNTIME_MEM.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "UI.H"
#include "TBS_EDITION.H"
#include "IWRAM_CALL.H"
#include "BATTLE_RUNTIME.H"
#include "FAR_RUNTIME.H"
#include "BATTLE_CALC.H"

#define FIELD(ptr, type, offset) (*(type *)((u8 *)(ptr) + (offset)))

struct MenuObjectControl {
    u8 padding00[4];
    u16 suspended;
};

extern struct MenuObjectControl *gMenuCtrlWork;

s32 Runtime_AllocateHeapBlock(s32 kind, s32 size);
void UiWindow_DrawFrameFar(s32 x, s32 y, s32 width, s32 height);
void UiWindow_InitializeWork(s32 unused);
s32 Party_ListActiveOwnersFar(const u16 *ids);
void ItemMenu_Init(s32, s32, s32, s32);
void Palette_LightenBankHighlight(s32 index);
void Link_DrawShiftedTilePairFar(s32 addr);
void Menu_CancelSoundReset(void);
s32 Menu_ResolveSelectedAction(s32 *, s32 *, s32 *);
void Menu_EnsureCancelSound(void);
void RenderOutput_ClearListFar(s32 screen_handle);
void ItemMenu_Close(void);
void UiWindow_EraseBorderRectFar(s32 x, s32 y, s32 width, s32 height);
void Event_ClearInvalidPackedValuesFar(void);

/* The European editions keep the 8 KB of background tiles at 0x06004000
   aside while the prompt is open and fill that area with colour 3. */
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define PROMPT_SAVES_TILES 1
#define PROMPT_TILES ((void *)0x06004000)
#define PROMPT_TILES_SIZE 0x2000
void Runtime_BumpFree(void *buffer);
void UiWork_SetAltFlagAndClearTableFar(s32 enable);
void UiWindow_MarkVisibleTileAttributesFar(void);

typedef s32 (*WordCopyFn)(void *dst, const void *src, s32 size);
typedef s32 (*WordFillFn)(void *dst, s32 size, u32 value);

static __inline__ s32 CopyWords(WordCopyFn copy, void *dst, const void *src, s32 size)
{
    /* FAKEMATCH: direct calls move the saved tile size from r9 to fp in the European editions. */
    return copy(dst, src, size);
}

static __inline__ s32 FillWords(WordFillFn fill, void *dst, s32 size, u32 value)
{
    /* FAKEMATCH: a direct fill call changes the preceding copy's fp call register and adds moves. */
    return fill(dst, size, value);
}
#endif

/*
 * Open a modal menu screen and run its blocking interaction body.
 *
 * The address of gMenuCtrlWork, not the pointer it holds, is the base for the
 * two fixed-address fields at +0x24 and +0x54; only "suspended" is reached
 * through the pointer itself.  The field read at +0x178 is named by position
 * only and is not otherwise confirmed.
 */
s32 Menu_OpenConfirmPrompt(void)
{
#if defined(PROMPT_SAVES_TILES)
    void *saved = Runtime_BumpAllocateAlternatePool(PROMPT_TILES_SIZE);
#endif
    void *state = (void *)Runtime_AllocateHeapBlock(0x37, 0xa70);
    s32 high;
    s32 unused;
    s32 low;
    s32 result;

    gMenuCtrlWork->suspended = 1;
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    WaitFrames(1);
    UiWindow_InitializeWork(0);
    FIELD(state, u8, 0x219) = (u8)Party_ListActiveOwnersFar((const u16 *)((u8 *)state + 0x208));
    ItemMenu_Init(0, 3, 0, 7);
    FIELD(state, s32, 0x10c) = UiWindow_CreateFar(13, 0, 17, 3, 2);
    Palette_LightenBankHighlight(14);
    Link_DrawShiftedTilePairFar(0x06002500);
#if defined(PROMPT_SAVES_TILES)
    Scheduler_EnableOverlayCallbacksWithFlags();
    CopyWords(Iwram_CopyWords, saved, PROMPT_TILES, PROMPT_TILES_SIZE);
    FillWords(Iwram_FillWords, PROMPT_TILES, PROMPT_TILES_SIZE, 0x33333333);
    UiWork_SetAltFlagAndClearTableFar(1);
#endif
    Menu_CancelSoundReset();
    result = Menu_ResolveSelectedAction(
        &high, &unused, &low);
    Menu_EnsureCancelSound();
    if (result == 1) {
        void *target = FIELD(&gMenuCtrlWork, void *, 0x54);
        u16 flags;
        BattleAction_Get(0x3fff & FIELD(state, u16, 0x178));
        flags = (u16)(low | (high << 10));
        FIELD(target, u16, 0x17e) = flags;
    }
    RenderOutput_ClearListFar(FIELD(state, s32, 0x24));
    FIELD(FIELD(&gMenuCtrlWork, void *, 0x24), u8, RENDER_MENU_BUSY_OFS) = 1;
    ItemMenu_Close();
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    Runtime_ReleaseHeapBlock(0x37);
    gMenuCtrlWork->suspended = 0;
#if defined(PROMPT_SAVES_TILES)
    UiWindow_MarkVisibleTileAttributesFar();
    UiWork_SetAltFlagAndClearTableFar(0);
    CopyWords(Iwram_CopyWords, PROMPT_TILES, saved, PROMPT_TILES_SIZE);
    FIELD(FIELD(&gMenuCtrlWork, void *, 0x24), u8, RENDER_MENU_BUSY_OFS) = 0;
    Runtime_BumpFree(saved);
    WaitFrames(1);
    Scheduler_DisableOverlayCallbacksWithFlags();
#endif
    WaitFrames(1);
    UiWindow_EraseBorderRectFar(0, 0, 30, 20);
    FIELD(FIELD(&gMenuCtrlWork, void *, 0x24), u8, RENDER_MENU_BUSY_OFS) = 0;
    Event_ClearInvalidPackedValuesFar();
    return result;
}

/*
 * gMenuWork is the polymorphic menu-runtime cell (see item_menu.h /
 * psynergy_menu.h). This owner reads and writes fields shared by both the
 * Inventory and Psynergy menu views (item_owner/target_owner at 0x21a/0x21b,
 * info_window at 0x2c, the selected id at 0x178, entry_count at 0x218), plus
 * two fields not yet named in either shared header (a byte "mode" at 0x268
 * that selects between three confirmation messages, and a u16 flags word at
 * 0x220). A local view is used here instead of extending the shared structs,
 * matching the project's convention for an owner-specific field range
 * (compare games/THE BROKEN SEAL/SRC/GAME/ITEM/USE.C's local ItemUseWork).
 */
struct MenuActionWork {
    u8 unknown_000[0x24];
    s32 field_024;             /* 0x024 */
    u8 unknown_028[4];
    s32 info_window;           /* 0x02c */
    u8 unknown_030[0x144];
    u16 field_174;             /* 0x174, cleared before party selection */
    u8 unknown_176[2];
    u16 selected_action;       /* 0x178 */
    u8 unknown_17a[0x9e];
    u8 entry_count;            /* 0x218 */
    u8 unknown_219;
    u8 item_owner;             /* 0x21a */
    u8 target_owner;           /* 0x21b */
    u8 unknown_21c[4];
    u16 flags_220;             /* 0x220 */
    s16 completion_flag;       /* 0x222 */
    u8 unknown_224[0x36];
    s16 message_offset;        /* 0x25a */
    u8 unknown_25c[0x0c];
    u8 mode;                   /* 0x268 */
};

extern struct MenuActionWork *gMenuWork;

extern char MsgShortcutSetL;
extern char MsgShortcutSetR;
extern char MsgPsynergyChooseOwner;
extern char MsgPsynergyChooseAbility;
extern char MsgPsynergyChooseTarget;
extern char MsgPsynergyChooseForR;
extern char MsgPsynergyChooseForL;
extern char MsgItemUseResult;

void WaitFrames(s32 frames);
void ItemMenu_DrawMsg(s32 unused, s32 message);
s32 PsynergyMenu_SelectPartySlot(s32 unused);
void ItemMenu_PosCategory(void);
void Menu_DrawOwnerStatusPanel(s32 window, s32 owner, s32 unused0, s32 unused1);
s32 PsynergyMenu_RunList(s32 unused);
s32 PsynergyMenu_SetShortcut(s32 owner, s32 psynergy, s32 shortcut);
void InventoryMenu_ShowModalMessage(s32 message, s32 arg1, s32 arg2);
s32 PsynergyMenu_SelectTarget(s32 unused);
s32 PsynergyMenu_ClassifySelectedPsynergy(void);
s32 BattleEffect_ApplyToTargets(s32 action, s32 owner, s32 target, s32 flags);
void Ability_PlayUseAnimation(s32 action);
void Audio_PlayCue(s32 cue);

/*
 * State machine that resolves the currently selected item/Psynergy command:
 * state 0 primes the category window and waits for the underlying selection
 * loop to finish; state 1 waits for a target/shortcut pick; state 2
 * classifies the selected Psynergy (immediate vs needs-target vs
 * needs-confirm); state 3 asks for a final confirmation; state 4 actually
 * applies the action, pays its PP cost, and reports success or failure. On
 * a clean finish (state 2's default) the acting owner and the selected
 * action id are written back through the two out-parameters.
 */
s32 Menu_ResolveSelectedAction(s32 *out_owner, s32 *unused, s32 *out_action)
{
    struct MenuActionWork *work;
    s32 result;
    s32 state;
    s32 done;
    s32 selection;
    s32 classification;
    s32 raw;
    s32 self_flag;
    struct BattleAction *action;

    state = 0;
    done = 0;
    work = gMenuWork;
    result = 0;

    while (done == 0 && GameFlag_TestFar(0x150) == 0) {
        switch (state) {
        case 0:
        {
            /* FAKEMATCH: the ordinary clear swaps address/zero r3/r2 in five instructions; the address belongs in r2. */
            register u16 *clear asm("r2") = &work->field_174;
            /* FAKEMATCH: constraining the address alone pools the zero; its full-width value belongs in r3. */
            register s32 zero asm("r3");
            /* FAKEMATCH: CSE copies the equal call result from r0; the reference keeps the cancellation constant in r3. */
            register s32 cancelled asm("r3");
            s32 chosen;

            /* FAKEMATCH: retain the clear address in r2 until the store, after the ordinary allocation swapped it with zero. */
            asm("" : : "r"(clear));
            zero = 0;
            /* FAKEMATCH: the full-width r3 zero keeps the reference immediate rather than an HImode pool load. */
            asm("" : : "r"(zero));
            *clear = zero;
            ItemMenu_DrawMsg(0, (s32)&MsgPsynergyChooseOwner);
            chosen = PsynergyMenu_SelectPartySlot(0);
            cancelled = -1;
            if (chosen == cancelled) {
                done = 1;
                /* FAKEMATCH: keep the cancellation copy in r3; CSE otherwise copies the equal call result in r0. */
                asm("" : "+r"(cancelled));
                result = cancelled;
            }
            RenderOutput_RedrawSavedRectFar(work->info_window);
            state = 1;
            break;
        }

        case 1:
            WaitFrames(1);
            Owner_GetStateFar(work->item_owner);
            state = 0;
            if (work->entry_count != 0) {
                /* FAKEMATCH: clobbering r2 hides the 0x218 it holds from reload_cse_move2add, so the 0x268 mode offset is built from its own constant (movs/lsls) instead of adds r2, #80. */
                asm("" : : : "r2");
                switch (work->mode) {
                case 0:
                    ItemMenu_DrawMsg(0, (s32)&MsgPsynergyChooseAbility);
                    break;
                case 1:
                    ItemMenu_DrawMsg(0, (s32)&MsgPsynergyChooseForL);
                    break;
                case 2:
                    ItemMenu_DrawMsg(0, (s32)&MsgPsynergyChooseForR);
                    break;
                }
                ItemMenu_PosCategory();
                Menu_DrawOwnerStatusPanel(work->field_024, work->item_owner, 0, 0);
                selection = PsynergyMenu_RunList(0);
                state = 0;
                if (selection != -1) {
                    state = 2;
                    if (work->mode != 0) {
                        if (work->mode == 1) {
                            PsynergyMenu_SetShortcut(
                                work->item_owner, selection, 0);
                            RenderOutput_ClearListFar(work->info_window);
                            InventoryMenu_ShowModalMessage(
                                (s32)&MsgShortcutSetL, -1, -1);
#if !EDITION_INTERNATIONAL
                            RenderOutput_RedrawSavedRectFar(work->info_window);
#endif
                        } else {
                            PsynergyMenu_SetShortcut(
                                work->item_owner, selection, 1);
                            RenderOutput_ClearListFar(work->info_window);
                            InventoryMenu_ShowModalMessage(
                                (s32)&MsgShortcutSetR, -1, -1);
#if !EDITION_INTERNATIONAL
                            RenderOutput_RedrawSavedRectFar(work->info_window);
#endif
                        }
                        state = 0;
                    }
                }
            }
            break;

        case 3:
            ItemMenu_DrawMsg(0, (s32)&MsgPsynergyChooseTarget);
            self_flag = PsynergyMenu_SelectTarget(0);
            state = 4;
            if (self_flag == -1) {
                work->flags_220 |= 1;
                state = 1;
            }
            break;

        case 2:
            classification = PsynergyMenu_ClassifySelectedPsynergy();
            if (classification == 1) {
                state = 3;
                break;
            }
            if (classification == 2) {
                work->target_owner = 9;
                state = 4;
                break;
            }
            done = 1;
            result = 1;
            *out_owner = work->item_owner;
            *out_action = work->selected_action & 0x3fff;
            break;

        case 4:
            self_flag = 0;
            raw = work->selected_action;
            result = BattleEffect_ApplyToTargets(
                raw, work->item_owner, work->target_owner, 0);
            if (work->target_owner == 9) {
                work->target_owner = work->item_owner;
                self_flag = 9;
            }
            if (result != -1) {
                action = BattleAction_Get(work->selected_action & 0x3fff);
                Owner_AdjustSecondValueFar(work->item_owner, -action->pp_cost);
            }
            BattleUnit_Recalculate(work->item_owner);
            if (result != -1) {
                Menu_DrawOwnerStatusPanel(work->field_024, work->target_owner, 0, 0);
                Ability_PlayUseAnimation(work->selected_action & 0x3fff);
                RenderOutput_ClearListFar(work->info_window);
                InventoryMenu_ShowModalMessage(
                    work->message_offset + (s32)&MsgItemUseResult, 0, -1);
            } else {
                Audio_PlayCue(114);
                RenderOutput_ClearListFar(work->info_window);
                InventoryMenu_ShowModalMessage(
                    work->message_offset + (s32)&MsgItemUseResult,
                    result,
                    result);
            }
            if (result != -1) {
                result = 1;
                work->flags_220 |= 1;
                state = 1;
            } else {
                work->completion_flag = 1;
                if (self_flag == 9) {
                    work->flags_220 |= 1;
                    state = 1;
                } else {
                    state = 3;
                }
            }
            break;

        default:
            done = 1;
            break;
        }
    }

    if (GameFlag_TestFar(0x150) != 0) {
        result = -1;
    }
    return result;
}
