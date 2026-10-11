#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "callback_scheduler.h"
#include "types.h"
#include "resource.h"
#include "menu_list.h"
#include "mem.h"
#include "glyph.h"
#include "select.h"
#include "resource_ids.h"
#include "scene.h"
#include "global_cells.h"
#include "tbs_edition.h"
#include "node_chain.h"
#include "lib.h"
#include "window.h"
#include "field_event.h"
#include "game_state.h"
#include "b5_context.h"
#include "keystate.h"
#include "io_reg.h"
#include "render_input.h"
#include "btltype.h"
#include "uiflow.h"
#include "text/msg_ids.h"
#include "owner_state.h"
#include "btlpty.h"
#include "workspace_options.h"
#include "iwram_call.h"
#include "fixed_math.h"
#include "dma.h"
#include "edition.h"
#include "text_render_runtime.h"
#include "sound_ids.h"
#include "btlsys.h"
#include "ram_buffer.h"
#include "vram_block.h"
#include "runtime_interfaces.h"
#include "btlwork.h"
#include "ramcode.h"
#include "party_state.h"
#include "save_state.h"
#include "battle_unit.h"
#include "irq.h"

void UiText_DrawPrefixedNumberAtOffset( s32 value, struct RenderInput *work, s32 offset_x, s32 offset_y, s32 variant);
void UiText_DrawResource(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void UiWindow_DrawDividerLine(struct RenderInput *win, u32 x1, u32 y1, u32 x2, u32 y2);

void UiTextResource_NoOpCallback(void)
{
}

void UiTextResource_NoOpCallbackBank(void)
{
}

void UiTextResource_NoOpCallback1(void)
{
}

void UiTextResource_NoOpCallback2(void)
{
}

void UiTextResource_NoOpCallback3(void)
{
}

void UiTextResource_NoOpCallback4(void)
{
}

void UiTextResource_Initialize(struct MenuSprite *object, s32 *slot)
{
    const void *data = Menu_CursorObjectTiles;
    s32 value = Resource_FindFreeEntry();

    *slot = value;
    object->oam.f.tile = VramBlock_LoadCached(value, 0x80, data);
    object->oam.f.mode = 0;
    object->oam.f.mosaic = 0;
    object->oam.f.colors = 1;
    object->oam.f.affine = 0;
    object->oam.f.affine_index = 0;
    object->oam.f.size = 0;
    object->oam.f.shape = 2;
    object->oam.f.priority = 0;
}

void UiTextResource_SetPosition(struct MenuSprite *obj, s32 x, s32 y)
{
    obj->oam.f.x = x;
    obj->oam.f.y = y;
    Runtime_PushSlotEntry((s32 *)obj, 0xFC);
}

void UiTextResource_Release(s32 value)
{
    Resource_ResetEntry(value);
}

#define UiWork_CloseAndRelease ((s32 (*)(void))UiWork_CloseAndRelease)

void Resource_DecodeByteLz(void *source, void *destination);

s32 ItemMenu_RunFar(void);
#define gEventWork (*(u8 *(*))&gEventWork)

void Menu_LoadSelectedResource(void)
{
    struct SelectionScreen *state = gResQueueWork;
    struct SelectionNode *selection = NodeChain_GetNodeAtCount(state);
    struct SelectionNode *transfer;
    GlyphTransfer *buffer;
    u8 *tbl;
    void *resource;
    s32 no;

    if (selection->kind != 1 && selection->kind != 6)
        return;

    buffer = (GlyphTransfer *)Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
    transfer = &state->records[15];
    no = selection->base;
    tbl = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);
    {
        u8 **destination = &buffer->encoded;
        resource = tbl
            + *(u16 *)(tbl + selection->base * 2);
        *destination = resource;
    }
    Resource_DecodeByteLz(resource, buffer);

    if (transfer->kind == 0)
        transfer->slot = Resource_FindFreeEntry();
    transfer->tile =
        VramBlock_LoadCached(transfer->slot, 0x400, buffer);
    transfer->kind = 1;
    transfer->base = no;
    transfer->scale = 40;
    transfer->scale_step = 40;
    transfer->scale_end = 240;
    Runtime_ReleaseHeapBlock(17);
}

void Resource_ResetPendingTransfer(void)
{
    struct SelectionNode *transfer = &gResQueueWork->records[15];

    if (transfer->kind != 0) {
        Resource_ResetEntry(transfer->slot);
        transfer->kind = 0;
    }
}

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)

#define HAS_LOCALIZED_MENU_GUARD 1
#endif

s32 Menu_RunTopSelection(void)
{
    s32 ret;
    s32 sel;
    u8 *state;

    state = gEventWork;
    sel = 0;

loop:
    UiWindow_OpenMode1AndWaitFrame();
    sel = Menu_SelectTopEntry(sel);
#if defined(HAS_LOCALIZED_MENU_GUARD)
    state[0xcca] = 1;
    if (*(s16 *)(state + 0xcb8) != 0) {
        BattleFx_ClearRandomParticlesFar();
        WaitFrames(1);
    }
#endif
    ret = UiWork_CloseAndRelease();

    switch (sel) {
    case 0:
        ret = Object_GetTriggerTileAheadOfCurrentFar();
        if (ret == 0)
            ret = 0xff;
        *(u16 *)(state + 0x17a) = ret;
        break;
    case 1:
        ret = Menu_OpenConfirmPromptFar();
        if (ret == -1)
            goto loop;
        break;
    case 2:
        ret = ItemMenu_RunFar();
        if (ret != 0)
            goto loop;
        break;
    case 3:
        ret = RunAssetSelectionScreenFar();
        if (ret == -1)
            goto loop;
        break;
    case 4:
        ret = Menu_OpenActionFlow();
        if (ret == -1)
            goto loop;
        break;
    default:
        break;
    }

#if defined(HAS_LOCALIZED_MENU_GUARD)
    state[0xcca] = 0;
#endif
    return ret;
}

void UiWindow_OpenMode1AndWaitFrame(void)
{
    UiWindow_CreateWithLayoutBounds(1);
    WaitFrames(1);
}

#undef gEventWork

//--------------------------------------------------------------------------

// runtime_close_and_release.c

// ウィンドウ：ヒープ 16 番のウィンドウを閉じてブロックを返す

//--------------------------------------------------------------------------

#undef UiWork_CloseAndRelease

//--------------------------------------------------------------------------
// ヒープ 16 番のウィンドウを閉じ、ブロックを返す
//--------------------------------------------------------------------------
void UiWork_CloseAndRelease(void)
{
	UiWork_FinalizeAndReleaseBlock16();
}

#define MenuSelection_BuildEntries ((void (*)(u32))MenuSelection_BuildEntries)
#define Menu_OpenSelectionWindow ((void (*)(u32,u32))Menu_OpenSelectionWindow)
#define UiText_GetResourceDimensions ((s32 (*)(s32,s32*,s32*,s32*,s32*))UiText_GetResourceDimensions)

s32 UiGlyph_ResetWorkState();

void UiText_DrawResource(s32 resource, s32 window, s32 x, s32 y);
s32 PartyInventory_RemoveFar(s32);

struct TimedNoticeWork {
    u8 unknown_000[0x230];
    struct UiWindow *window;
    u16 countdown;
};

void Ui_ClearWorkStateAndWaitFrame(void)
{
    UiGlyph_ResetWorkState();
    Resource_ClearOwnerListAndCounters();
    WaitFrames(1);
}

u32 Menu_RunSelectionForValue(u32 value)
{
    struct SelectionScreen *state = gResQueueWork;
    u32 result;

    state->cursor_index = value;
    state->locked = 1;
    MenuSelection_BuildEntries(value);
    Menu_SetupSelectionBothSides();
    Menu_OpenSelectionWindow(0, 5);
    Resource_ScheduleOwnerResetDelayed();
    result = Menu_WaitForSelectionInput(1);
    Resource_ResetOwnerEntries();
    return result;
}

void UiTimedNotice_Create(void)
{
    struct TimedNoticeWork *work;
    s32 height;
    s32 width;
    s32 y;
    s32 x;
    s32 resource;
    struct UiWindow *window;

    work = (struct TimedNoticeWork *)gEventWork;
    x = 8;
    y = 8;
    resource = Scene_GetAreaNameIndexFar(gGameState.scene, gGameState.entrance) + RENDER_RESOURCE_BASE;
    UiText_GetResourceDimensions(resource, &x, &y, &width, &height);
    x = (30 - width) >> 1;
    y = (10 - height) >> 1;
    window = UiWindow_Create(x, y, width, height, 2);
    work->window = window;
    UiText_DrawResource(resource, (s32)window, 0, 0);
    work->countdown = 90;
    Scheduler_AddOrUpdateCallback(UiTimedNotice_Tick, 0xc80);
}

void UiTimedNotice_Tick(void)
{
    struct TimedNoticeWork *work;
    u16 count;

    work = (struct TimedNoticeWork *)gEventWork;
    work->countdown = (count = work->countdown + 0xffff);
    if ((count << 16) == 0) {
        UiWork_Finalize(work->window, 2);
        Scheduler_RemoveCallback(UiTimedNotice_Tick);
    }
}

void UiTimedNotice_CloseIfActive(void)
{
    struct UiWindow *window;

    window = ((struct TimedNoticeWork *)gEventWork)->window;
    if (window != NULL && window->flags != 0) {
        UiWork_Finalize(window, 2);
        Scheduler_RemoveCallback(UiTimedNotice_Tick);
    }
}

s32 Item_CallHandler48(s32 arg0, s32 arg1)
{
    PartyInventory_RemoveFar(arg1);
    return 0;
}

s32 Item_ReturnTrue(void)
{
    return 1;
}

void Party_AdjustPaletteGlowPhase(s32 arg0)
{
    u8 value = gGameState.palette_glow[0];
    if (arg0 & 0x20)
        value += 0xff;
    else
        value += 1;
    gGameState.palette_glow[0] = value;
}

struct BtlAct * BtlActGet(s32 action_id);

#define Resource_FixedBlockBTiles ((const u8 *)&Resource_FixedBlockBTiles)
#define UiWindow_Create ((struct RenderInput * (*)(s32, s32, s32, s32, s32))UiWindow_Create)
#define UiText_DrawCharacterAtOffset ((void (*)(s32, struct RenderInput *, s32, s32))UiText_DrawCharacterAtOffset)
#define RenderOutput_RedrawSavedRect ((void (*)(struct RenderInput *))RenderOutput_RedrawSavedRect)

#define Menu_FindShortcutEntries ((void (*)(u32*,u32*,const struct ShortcutListEntry*))Menu_FindShortcutEntries)

#define Object_CollectResources ((s32 (*)(struct ShortcutListEntry*))Object_CollectResources)

struct ShortcutListEntry {
    u16 owner;
    u16 ability;
};

void UiText_DrawNumberAtOffset(s32 value, s32 digits, struct RenderInput *window, s32 x, s32 y);
#define UiWindow_DrawDividerLine ((void (*)(struct RenderInput *window, s32 x, s32 y, s32 width, s32 style))UiWindow_DrawDividerLine)
void WaitFrames(s32 frames);
void AudioCommand_PlayFar(s32 cue);

void Debug_SelectAbilityPair(void)
{
    struct ShortcutListEntry *list;
    struct RenderInput *win;
    struct RenderInput *title;
    struct RenderInput *info;
    struct RenderOutput *cursor;
    u32 first;
    u32 second;
    u32 count;
    s32 done = 0;
    s32 row = 0;
    s32 redraw;
    s32 slot;
    s32 y;
    s32 cost;
    s32 ability;

    cursor = NULL;
    redraw = 1;
    list = MemAlloc(0x700);
    OwnerAction_AddFar(0, 140);
    OwnerAction_AddFar(1, 140);
    OwnerAction_AddFar(2, 140);
    OwnerAction_AddFar(2, 141);
    OwnerAction_AddFar(2, 78);
    OwnerAction_AddFar(3, 93);
    OwnerAction_AddFar(5, 140);
    second = 0;
    first = 0;
    count = Object_CollectResources(list);
    if (count != 0) {
        Menu_FindShortcutEntries(&first, &second, list);
        win = UiWindow_Create(4, 6, 20, 7, 2);
        title = UiWindow_Create(4, 3, 20, 3, 2);
        info = UiWindow_Create(4, 14, 20, 5, 2);
        slot = Resource_FindFreeEntry();
        if (slot != 0) {
            VramBlock_LoadCached(slot, 128, Resource_FixedBlockBTiles);
            cursor = RenderOutput_Create(slot, 0x40000000, win, 0, 0);
        }
        /* FAKEMATCH: needed to match: the title message reuses the slot variable */
        slot = (s32)MsgPanelHpMaxLabel;
        UiText_DrawCharacterAtOffset(slot, title, 16, 0);
        for (;;) {
            if (redraw != 0) {
                redraw = 0;
                first = (first + count) % count;
                second = (second + count) % count;
                row = (row + 2) % 2;
                y = (row << 4) + (win->y << 3) + 28;
                cursor->y = y;
                cursor->oam.y = y;
                RenderOutput_RedrawSavedRect(win);
                UiWindow_DrawDividerLine(win, 1, 2, 17, 2);
                UiText_DrawCharacterAtOffset((s32)MsgPanelStatusLabel, win, 48, 0);
                UiText_DrawCharacterAtOffset(list[first].ability + (s32)MsgAbilityName, win, 56, 16);
                UiText_DrawCharacterAtOffset(list[second].ability + (s32)MsgAbilityName, win, 56, 32);
                UiText_DrawCharacterAtOffset((s32)MsgPanelStatusLabel - 2, win, 16, 16);
                UiText_DrawCharacterAtOffset((s32)MsgPanelStatusLabel - 1, win, 16, 32);
                UiText_DrawCharacterAtOffset(list[first].owner + (s32)MsgCharacterName, win, 104, 16);
                UiText_DrawCharacterAtOffset(list[second].owner + (s32)MsgCharacterName, win, 104, 32);
                RenderOutput_RedrawSavedRect(info);
                UiText_DrawCharacterAtOffset((s32)MsgPsynergyNeedMorePp, info, 0, 16);
                if (row != 0) {
                    cost = BtlActGet(list[second].ability)->pp_cost;
                    ability = list[second].ability;
                } else {
                    cost = BtlActGet(list[first].ability)->pp_cost;
                    ability = list[first].ability;
                }
                UiText_DrawNumberAtOffset(cost, 2, info, 64, 16);
                UiText_DrawCharacterAtOffset(ability + (s32)MsgAbilityDescription, info, 0, 0);
            }
            WaitFrames(1);
            if (gKeysRepeat & KEY_LEFT) {
                AudioCommand_PlayFar(111);
                if (row != 0)
                    second--;
                else
                    first--;
                redraw = 1;
            }
            if (gKeysRepeat & KEY_RIGHT) {
                AudioCommand_PlayFar(111);
                if (row != 0)
                    second++;
                else
                    first++;
                redraw = 1;
            }
            if (gKeysRepeat & KEY_UP) {
                AudioCommand_PlayFar(111);
                row--;
                redraw = 1;
            }
            if (gKeysRepeat & KEY_DOWN) {
                AudioCommand_PlayFar(111);
                row++;
                redraw = 1;
            }
            if (gKeyState & KEY_A)
                AudioCommand_PlayFar(112);
            else if (gKeyState & KEY_B)
                AudioCommand_PlayFar(113);
            else if (gKeyState & KEY_START)
                AudioCommand_PlayFar(113);
            else
                continue;
            /* FAKEMATCH: the unread flag forces fresh zeros for both entries and the cursor argument instead of reusing the row register. */
            done = 1;
            break;
        }
        gGameState.first_shortcut = (list[first].owner << 10) | list[first].ability;
        gGameState.second_shortcut = (list[second].owner << 10) | list[second].ability;
        UiWork_Finalize(win, 1);
        UiWork_Finalize(title, 1);
        UiWork_Finalize(info, 1);
        WaitFrames(1);
    }
    MemFree(list);
}

#undef Menu_FindShortcutEntries
#undef Object_CollectResources

struct BtlUnit *UnitGetFar(s32 object_id);

#define BtlActGet ((void * (*)(s32))BtlActGet)

#define ACTION_ID_MASK 0x3FFF

struct AbilityListEntry {
    u16 owner;
    u16 ability;
};

s32 Object_CollectResources(struct AbilityListEntry *output)
{
    u16 object_ids[14];
    s32 output_count = 0;
    s32 object_count = Party_ListActiveOwnersFar(object_ids);
    s32 i;

    for (i = 0; i < object_count; i++) {
        struct BtlUnit *object;
        u32 id;
        s32 index;
        u32 resource_id;

        id = object_ids[i];
        object = UnitGetFar(id);
        for (index = 0; index < (s32)(sizeof(object->action_slots) / sizeof(object->action_slots[0])); index++) {
            resource_id = object->action_slots[index].encoded_action & ACTION_ID_MASK;
            if (resource_id == 0)
                break;
            BtlActGet(resource_id);
            output[output_count].owner = id;
            output[output_count].ability = resource_id;
            output_count++;
        }
    }

    return output_count;
}

void Menu_FindShortcutEntries(u32 *first_index, u32 *second_index,
                              const struct AbilityListEntry *entries)
{
    s32 i;

    *first_index = 0;
    *second_index = 0;

    for (i = 0; i <= 447; i++) {
        if (entries[i].ability == (gGameState.first_shortcut & 0x3ff) &&
            entries[i].owner == (gGameState.first_shortcut >> 10)) {
            *first_index = i;
            break;
        }
    }

    for (i = 0; i <= 447; i++) {
        if (entries[i].ability == (gGameState.second_shortcut & 0x3ff) &&
            entries[i].owner == (gGameState.second_shortcut >> 10)) {
            *second_index = i;
            break;
        }
    }
}

void Menu_RunSelectionHook(void)
{
    Debug_SelectAbilityPair();
}

struct SelectionWorkspace {
    u8 unknown_000[0x46];
    u16 first_active;
    u16 first_resource;
    u8 unknown_04a[0x352 - 0x4a];
    u16 second_active;
    u16 second_resource;
    u8 unknown_356[0xff4 - 0x356];
    struct UiWindow *window;
    u8 unknown_ff8[12];
};

void Menu_AllocateSelectionWorkspace(void)
{
    struct SelectionWorkspace *work;
    u32 zero;

    work = Runtime_AllocateBlock(0x13, 0x1004);
    zero = 0;
    work->first_active = zero;
    work->second_active = zero;
}

void Ui_FinalizeWorkAndReleaseHeap13(void)
{
    struct SelectionWorkspace *state;
    u16 *p;

    state = *(struct SelectionWorkspace **)gSelectionWorkspace;
    UiWork_Finalize(state->window, 0);
    while (UiWork_IsIdle(state->window) == 0) {
        WaitFrames(1);
    }
    if (state->first_active != 0) {
        Resource_ResetEntry(state->first_resource);
    }
    p = &state->second_active;
    if (*p != 0) {
        p = (u16 *)((u8 *)p + 2);
        Resource_ResetEntry(*p);
    }
    Runtime_ReleaseHeapBlock(0x13);
}

void GraphicsPalette_ReservedNoOpC9BC(void)
{
}

void GraphicsPalette_ReservedNoOpC9C0(void)
{
}

void GraphicsPalette_ReservedNoOpC9C4(void)
{
}

#undef UiWindow_DrawDividerLine

struct MenuWorkspaceEntry {
    u8 unknown_00[10];
    u16 state;
    u8 unknown_0c[40];
};

void Menu_ResetWorkspaceSelection(struct WorkspaceWork *work)
{
    struct MenuWorkspaceEntry *entry = (struct MenuWorkspaceEntry *)((u8 *)work + 0x400);
    u16 zero = 0;

    entry->state = zero;
    entry++;
    entry->state = zero;
    entry++;
    entry->state = zero;
    entry++;
    entry->state = zero;
    entry++;
    entry->state = zero;
    entry++;
    entry->state = zero;
    entry++;
    entry->state = zero;
    work->page = zero;
    work->tint[0] = zero;
    work->tint[1] = zero;
    work->tint[2] = zero;
    work->cursor_frame = zero;
}

void GraphicsPalette_SetTintChannelsFromCounters(struct WorkspaceWork *work)
{
    s16 phase;
    s32 bias;
    s32 blue;
    s32 red;
    s32 green;

    phase = (gGameState.palette_glow[0] + 12) % 24 * 4;
    bias = gGameState.palette_glow[1] - 7;
    red = PaletteGlow_WaveTable[(phase % 96)];
    green = PaletteGlow_WaveTable[(phase + 32) % 96];
    blue = PaletteGlow_WaveTable[(phase + 64) % 96];
    red += bias;
    green += bias;
    blue += bias;
    if (red < 0)
        red = 0;
    if (green < 0)
        green = 0;
    if (blue < 0)
        blue = 0;
    if (red > 31)
        red = 31;
    if (green > 31)
        green = 31;
    if (blue > 31)
        blue = 31;
    work->tint[0] = red;
    work->tint[1] = green;
    work->tint[2] = blue;
}

void Palette_WriteBlendedBank15Entries(struct WorkspaceWork *work)
{
    *(s16 *)0x050001E8 = Palette_ScaleTintChannels(work, 0xEEEE, 0xCCCC, 0x11110);
    *(s16 *)0x050001EA = Palette_ScaleTintChannels(work, 0xD555, 0xBBBB, 0xEEEE);
    *(s16 *)0x050001EC = Palette_ScaleTintChannels(work, 0xBBBB, 0xAAAA, 0xCCCC);
    *(s16 *)0x050001EE = Palette_ScaleTintChannels(work, 0xA221, 0x9999, 0xAAAA);
    *(s16 *)0x050001F0 = Palette_ScaleTintChannels(work, 0x10888, 0xDDDD, 0x13333);
    *(s16 *)0x050001F2 = Palette_ScaleTintChannels(work, 0x12221, 0xEEEE, 0x15555);
    *(s16 *)0x050001F4 = Palette_ScaleTintChannels(work, 0x13BBB, 0x10000, 0x17777);
}

u16 Palette_ScaleTintChannels(struct WorkspaceWork *work, s32 scale_r, s32 scale_g, s32 scale_b)
{
    s32 r;
    s32 g;
    s32 b;

    r = Iwram_MulQ16(work->tint[0] << 16, scale_r) >> 16;
    g = Iwram_MulQ16(work->tint[1] << 16, scale_g) >> 16;
    b = Iwram_MulQ16(work->tint[2] << 16, scale_b) >> 16;
    if (r < 0)
        r = 0;
    if (g < 0)
        g = 0;
    if (b < 0)
        b = 0;
    if (r > 31)
        r = 31;
    if (g > 31)
        g = 31;
    if (b > 31)
        b = 31;
    return r + ((b << 10) + (g << 5));
}

u16 Color_ScaleComponents(s16 *rgb, s32 red_scale, s32 green_scale, s32 blue_scale)
{
    s32 red;
    s32 green;
    s32 blue;

    red = Iwram_MulQ16(rgb[0] << 16, red_scale) >> 16;
    green = Iwram_MulQ16(rgb[1] << 16, green_scale) >> 16;
    blue = Iwram_MulQ16(rgb[2] << 16, blue_scale) >> 16;
    if (red < 0)
        red = 0;
    if (red > 31)
        red = 31;
    if (green < 0)
        green = 0;
    if (green > 31)
        green = 31;
    if (blue < 0)
        blue = 0;
    if (blue > 31)
        blue = 31;
    return red + ((blue << 10) + (green << 5));
}

void PaletteGlow_Update(s32 phase, s32 brightness)
{
    s16 rgb[3];
    s16 step;
    s16 red;
    s16 green;
    s16 blue;
    s16 offset;

    step = ((phase + 12) % 24) * 4;
    offset = brightness - 7;
    red = PaletteGlow_WaveTable[(step % 96)] + offset;
    green = PaletteGlow_WaveTable[(step + 32) % 96] + offset;
    blue = PaletteGlow_WaveTable[(step + 64) % 96] + offset;
    if (red < 0)
        red = 0;
    if (red > 31)
        red = 31;
    if (green < 0)
        green = 0;
    if (green > 31)
        green = 31;
    if (blue < 0)
        blue = 0;
    if (blue > 31)
        blue = 31;
    rgb[0] = red;
    rgb[1] = green;
    rgb[2] = blue;
    BG_PLTT_COLOR(15, 4) = Color_ScaleComponents(rgb, 0xeeee, 0xcccc, 0x11110);
    BG_PLTT_COLOR(15, 5) = Color_ScaleComponents(rgb, 0xd555, 0xbbbb, 0xeeee);
    BG_PLTT_COLOR(15, 6) = Color_ScaleComponents(rgb, 0xbbbb, 0xaaaa, 0xcccc);
    BG_PLTT_COLOR(15, 7) = Color_ScaleComponents(rgb, 0xa221, 0x9999, 0xaaaa);
    BG_PLTT_COLOR(15, 8) = Color_ScaleComponents(rgb, 0x10888, 0xdddd, 0x13333);
    BG_PLTT_COLOR(15, 9) = Color_ScaleComponents(rgb, 0x12221, 0xeeee, 0x15555);
    BG_PLTT_COLOR(15, 10) = Color_ScaleComponents(rgb, 0x13bbb, 0x10000, 0x17777);
}

void GraphicsPalette_DecrementSelectionWrap(struct WorkspaceWork *work)
{
    if (work->page == 0)
        work->page = 2;
    else
        work->page--;
}

void Menu_AdvanceWorkspaceIndexModulo3(struct WorkspaceWork *work)
{
    if (++work->page > 2)
        work->page = 0;
}

void GraphicsPalette_DecrementSelectedCounter(struct WorkspaceWork *work)
{
    switch (work->page) {
    case 0:
        if (gGameState.message_speed != 0)
            gGameState.message_speed--;
        break;
    case 1:
        if (gGameState.palette_glow[0] != 0)
            gGameState.palette_glow[0]--;
        break;
    case 2:
        if (gGameState.palette_glow[1] != 0)
            gGameState.palette_glow[1]--;
        break;
    }
}

void GraphicsPalette_AdjustSelectionCounter(struct WorkspaceWork *work)
{
    switch (work->page) {
    case 0:
        if (gGameState.message_speed <= 1)
            gGameState.message_speed++;
        break;
    case 1:
        if (gGameState.palette_glow[0] <= 23)
            gGameState.palette_glow[0]++;
        break;
    case 2:
        if (gGameState.palette_glow[1] <= 14)
            gGameState.palette_glow[1]++;
        break;
    }
}

void GraphicsPalette_SelectionNoOp(void)
{
}

#define ShopCursor_AdvanceFar ((void (*)(void*))ShopCursor_AdvanceFar)
#define ShopCursor_MoveTowardTargetFar ((void (*)(void*))ShopCursor_MoveTowardTargetFar)
#define Ui_ApplyTableScaleToObject ((void (*)(void*))Ui_ApplyTableScaleToObject)

void GraphicsPalette_LoadSelectionResourcesAndAdvance(void)
{
    s32 src0;
    s32 src1;
    s32 sel;
    struct WorkspaceWork *work;

    work = gSelectionWork;
    sel = work->page;
    ShopCursor_AdvanceFar(&work->cursor);
    ShopCursor_MoveTowardTargetFar(&work->marker[0]);
    ShopCursor_MoveTowardTargetFar(&work->marker[1]);

    if (sel == 0) {
        src0 = (work->cursor_frame & 7) +
            (s32)&ResourceId_ShopCursorFrames;
    } else {
        src0 = (s32)&ResourceId_ShopCursorFrames;
    }
    VramBlock_LoadCached(
        (u8)work->marker[0].output->index,
        0x100,
        Resource_GetTableEntry(src0));

    if (sel == 1) {
        src1 = (work->cursor_frame & 7) +
            (s32)&ResourceId_ShopCursorFrames;
    } else {
        src1 = (s32)&ResourceId_ShopCursorFrames;
    }
    VramBlock_LoadCached(
        (u8)work->marker[1].output->index,
        0x100,
        Resource_GetTableEntry(src1));

    if (sel > 1)
        Ui_ApplyTableScaleToObject(work->frame[sel - 2][work->option[sel]]);
    work->cursor_frame++;
}

void OptionMenu_InitializeWork(void)
{
    struct WorkspaceWork *work;
    volatile u32 zero;

    work = (struct WorkspaceWork *)Runtime_AllocateBlock(20, 0x628);
    zero = 0;
    Dma_Set((const void *)&zero, work, 0x8500018a, REG_DMA3);
    work->option[0] = gGameState.palette_glow[0];
    work->option_count[0] = 24;
    work->option[1] = gGameState.palette_glow[1];
    work->option_count[1] = 15;
    work->option[2] = gGameState.message_speed;
    work->option_count[2] = 3;
    work->option[3] = gGameState.speech;
    work->option_count[3] = 2;
    work->option[4] = gGameState.saved_options;
    work->option_count[4] = 2;
    Scheduler_AddOrUpdateCallback(GraphicsPalette_LoadSelectionResourcesAndAdvance, 0xc80);
}

void Runtime_ScheduleCallbackAndReleaseBlock20B(void)
{
    Scheduler_RemoveCallback(GraphicsPalette_LoadSelectionResourcesAndAdvance);
    Runtime_ReleaseHeapBlock(0x14);
}

#define RenderResource_CreateFrame ((void * (*)(s32,s32,struct RenderInput*,s32,s32))RenderResource_CreateFrame)
#define ShopCursor_SetPositionImmediateFar ((void (*)(void*,s32,s32))ShopCursor_SetPositionImmediateFar)
#define Shop_SetCursorFar ((void (*)(void*,s32,s32,s32))Shop_SetCursorFar)
#define UiWindow_ClearInteriorTiles ((void (*)(struct RenderInput*,s32,s32,s32,s32))UiWindow_ClearInteriorTiles)
#define Ui_ApplyTableOffsetToPair ((void (*)(void*))Ui_ApplyTableOffsetToPair)

#define SLIDER_PALETTE 14
#define RESOURCE_SLOTS 96
#define UiWindow_DrawDividerLine ((void (*)(struct RenderInput *, s32, s32, s32, s32))UiWindow_DrawDividerLine)

extern u8 gSleepDisabled;
extern u8 gOptionMirror;
void WaitFrames(s32);
void PaletteGlow_Update(s32, s32);
#define UiText_DrawResource ((void (*)(s32, struct RenderInput *, s32, s32))UiText_DrawResource)
void AudioCommand_PlayFar(s32);

struct RenderInput *Menu_OpenWorkspaceOptions(void)
{
    /* FAKEMATCH: the shared cursor union keeps pointer stores before window reads; an equal-size struct reorders these moves in all six editions. */
    struct WorkspaceWork *work;
    struct RenderInput *win;
    struct RenderOutput *out;
    s32 x;
    s32 y;
    s32 msg;

    work = gSelectionWork;
    win = UiWindow_Create(OPTION_WINDOW_X, 5, OPTION_WINDOW_WIDTH, 14, 2);
    UiWindow_DrawDividerLine(win, 0, 2, OPTION_WINDOW_WIDTH - 1, 2);
    UiWindow_DrawDividerLine(win, 0, 4, OPTION_WINDOW_WIDTH - 1, 4);
    UiWindow_DrawDividerLine(win, 0, 7, OPTION_WINDOW_WIDTH - 1, 7);
    UiWindow_DrawDividerLine(win, 0, 10, OPTION_WINDOW_WIDTH - 1, 10);
    msg = (s32)MsgWindowColorLabel;
    UiText_DrawCharacterAtOffset(msg, win, 8, 0);
    msg++;
    UiText_DrawCharacterAtOffset(msg, win, 8, 16);
#if EDITION_INTERNATIONAL
    msg = (s32)MsgMessageSpeedLabel;
    UiText_DrawCharacterAtOffset(msg, win, 8, 32);
    msg++;
    UiText_DrawCharacterAtOffset(msg, win, 32, 40);
#else
    UiText_DrawCharacterAtOffset((s32)MsgMessageSpeedLabel, win, 8, 40);
#endif
    UiText_DrawCharacterAtOffset((s32)MsgSpeechLabel, win, 8, 64);
    UiText_DrawCharacterAtOffset((s32)MsgAutoSleepLabel, win, 8, 88);

    x = Resource_FindFreeEntry();
    if (x < RESOURCE_SLOTS) {
        VramBlock_LoadCached(x, 128, (const void *)Resource_FixedBlockBTiles);
        out = RenderOutput_Create(x, 0x40000000, win, 0, 0);
        work->cursor.output = out;
        x = win->x * 8;
        y = win->y * 8 + 12;
        ShopCursor_SetPositionImmediateFar(&work->cursor, x, y);
    }

    x = Resource_FindFreeEntry();
    if (x < RESOURCE_SLOTS) {
        Dma_Set(WorkspaceOptions_SliderPalette, (void *)&OBJ_PLTT_COLOR(SLIDER_PALETTE, 0), 0x80000020, REG_DMA3);
        VramBlock_LoadCached(x, 256, WorkspaceOptions_SliderTiles);
        y = 0;
        out = RenderOutput_Create(x, 0x40004000, win, OPTION_SLIDER_X - 6, y);
        out->oam.palette = SLIDER_PALETTE;
        out = RenderOutput_Create(x, 0x40004000, win, OPTION_SLIDER_X + 26, y);
        out->oam.index += 4;
        out->oam.palette = SLIDER_PALETTE;
        y = 16;
        out = RenderOutput_Create(x, 0x40004000, win, OPTION_SLIDER_X - 6, y);
        out->oam.palette = 15;
        out = RenderOutput_Create(x, 0x40004000, win, OPTION_SLIDER_X + 26, y);
        out->oam.index += 4;
        out->oam.palette = 15;
    }

    x = Resource_FindFreeEntry();
    if (x < RESOURCE_SLOTS) {
        VramBlock_LoadCached(x, 256, 0);
        out = RenderOutput_Create(x, 0x40000000, win, 0, 0);
        out->oam.palette_256 = 1;
        work->marker[0].output = out;
        x = win->x * 8 + OPTION_SLIDER_X;
        x += work->option[0] * 60 / work->option_count[0];
        y = win->y * 8 + 4;
        ShopCursor_SetPositionImmediateFar(&work->marker[0], x, y);
    }

    x = Resource_FindFreeEntry();
    if (x < RESOURCE_SLOTS) {
        VramBlock_LoadCached(x, 256, 0);
        out = RenderOutput_Create(x, 0x40000000, win, 0, 0);
        out->oam.palette_256 = 1;
        work->marker[1].output = out;
        x = win->x * 8 + OPTION_SLIDER_X;
        x += work->option[1] * 60 / work->option_count[1];
        y = win->y * 8 + 20;
        ShopCursor_SetPositionImmediateFar(&work->marker[1], x, y);
    }

    y = 28;
    work->frame[0][0] = RenderResource_CreateFrame(OptionMenu_SpeedFrames[0], 0, win, OPTION_FRAME_X, y);
    work->frame[0][1] = RenderResource_CreateFrame(OptionMenu_SpeedFrames[1], 0, win, OPTION_FRAME_X + 24, y);
    work->frame[0][2] = RenderResource_CreateFrame(OptionMenu_SpeedFrames[2], 0, win, OPTION_FRAME_X + 48, y);
    y = 52;
    work->frame[1][0] = RenderResource_CreateFrame(OptionMenu_SpeechFrames[0], 0, win, 100, y);
    work->frame[1][1] = RenderResource_CreateFrame(OptionMenu_SpeechFrames[1], 0, win, 124, y);
    y = 76;
    work->frame[2][0] = RenderResource_CreateFrame(OptionMenu_SleepFrames[0], 0, win, 100, y);
    work->frame[2][1] = RenderResource_CreateFrame(OptionMenu_SleepFrames[1], 0, win, 124, y);
    return win;
}

s32 Menu_RunWorkspaceOptions(void)
{
    struct WorkspaceWork *work;
    struct RenderInput *icon;
    struct RenderInput *win;
    struct RenderOutput *pair;
    s8 *pA;
    s8 *pB;
    s32 redraw;
    s32 page;
    s32 i;
    s32 dim;
    s32 index;
    s32 x;
    s32 y;
    s32 result;
    struct RenderOutput *frame;

    redraw = 1;
    page = 0;
    OptionMenu_InitializeWork();
    work = gSelectionWork;
    win = UiWindow_Create(OPTION_WINDOW_X, 2, OPTION_WINDOW_WIDTH, 3, 2);
    icon = Menu_OpenWorkspaceOptions();
    pair = RenderResource_CreatePair(7, icon, 64, -48);
    WaitFrames(1);
    pA = &work->option[0];
    pB = &work->option[1];

    for (;;) {
        if (redraw != 0) {
            redraw = 0;
            page = (page + 5) % 5;
            work->option[page] = (work->option[page] + work->option_count[page]) % work->option_count[page];
            work->page = page;
            if (gSleepDisabled != 0)
                work->option[4] = 0;

            for (i = 0; i <= 2; i++) {
                frame = work->frame[0][i];
                frame->sentinel = 0xfb;
                UiIcon_PrepareObjectFar(frame);
                index = (u8)work->frame[0][i]->index;
                dim = 0;
                if (i != work->option[2])
                    dim = 1;
                RenderResource_LoadFrame(OptionMenu_SpeedFrames[i], index, dim);
            }
            for (i = 0; i <= 1; i++) {
                frame = work->frame[1][i];
                frame->sentinel = 0xfb;
                UiIcon_PrepareObjectFar(frame);
                index = (u8)work->frame[1][i]->index;
                dim = 0;
                if (i != work->option[3])
                    dim = 1;
                RenderResource_LoadFrame(OptionMenu_SpeechFrames[i], index, dim);
            }
            for (i = 0; i <= 1; i++) {
                frame = work->frame[2][i];
                frame->sentinel = 0xfb;
                UiIcon_PrepareObjectFar(frame);
                index = (u8)work->frame[2][i]->index;
                dim = 0;
                if (i != work->option[4])
                    dim = 1;
                RenderResource_LoadFrame(OptionMenu_SleepFrames[i], index, dim);
            }

            x = icon->x * 8 + OPTION_SLIDER_X;
            x += (*pA * 60) / work->option_count[0];
            y = icon->y * 8 + 4;
            Shop_SetCursorFar(&work->marker[0], x, y, 1);
            x = icon->x * 8 + OPTION_SLIDER_X;
            x += (*pB * 60) / work->option_count[1];
            y = icon->y * 8 + 20;
            Shop_SetCursorFar(&work->marker[1], x, y, 1);

            x = work->option[2] + (s32)MsgMessageSpeedSetting;
            UiWindow_ClearInteriorTiles(icon, OPTION_SETTING_X, 40, OPTION_SPEED_END, 48);
            UiText_DrawCharacterAtOffset(x, icon, OPTION_SETTING_X, 40);
            x = work->option[3] + (s32)MsgSpeechSetting;
            UiWindow_ClearInteriorTiles(icon, OPTION_SETTING_X, 64, OPTION_SETTING_END, 72);
            UiText_DrawCharacterAtOffset(x, icon, OPTION_SETTING_X, 64);
            x = work->option[4] + (s32)MsgAutoSleepSetting;
            UiWindow_ClearInteriorTiles(icon, OPTION_SETTING_X, 88, OPTION_SETTING_END, 96);
            UiText_DrawCharacterAtOffset(x, icon, OPTION_SETTING_X, 88);
            PaletteGlow_Update(*pA, *pB);

            x = icon->x * 8;
            y = (page * 3 + icon->y) * 8 + 4;
            if (page == 0)
                y += 8;
            Shop_SetCursorFar(&work->cursor, x, y, 3);
            RenderOutput_ClearList(win);
            UiText_DrawResource(page + (s32)MsgOptionHelp, win, 0, 0);
        }
        Ui_ApplyTableOffsetToPair(pair);
        WaitFrames(1);

        if (gKeyState & KEY_SELECT) {
            AudioCommand_PlayFar(112);
            work->preset++;
            redraw = 1;
            if (work->preset > 5)
                work->preset = 0;
            *pA = OptionMenu_FirstColorPresets[work->preset];
            *pB = OptionMenu_SecondColorPresets[work->preset];
            continue;
        }
        if (gKeyState & (KEY_A | KEY_START)) {
            result = 0;
            AudioCommand_PlayFar(112);
            break;
        }
        if (gKeyState & KEY_B) {
            result = -1;
            AudioCommand_PlayFar(113);
            break;
        }
        if (gKeysRepeat & KEY_UP) {
            AudioCommand_PlayFar(111);
            page--;
            redraw = 1;
        } else if (gKeysRepeat & KEY_DOWN) {
            AudioCommand_PlayFar(111);
            page++;
            redraw = 1;
        } else {
            if (gKeysRepeat & KEY_LEFT) {
                AudioCommand_PlayFar(111);
                work->option[page]--;
                redraw = 1;
            }
            if (gKeysRepeat & KEY_RIGHT) {
                AudioCommand_PlayFar(111);
                work->option[page]++;
                redraw = 1;
            }
        }
    }

    UiWork_Finalize(win, 2);
    UiWork_Finalize(icon, 2);
    if (result == 0) {
        gGameState.palette_glow[0] = work->option[0];
        gGameState.palette_glow[1] = work->option[1];
        gGameState.message_speed = work->option[2];
        gGameState.speech = work->option[3];
        gGameState.saved_options = work->option[4];
        gOptionMirror = gGameState.saved_options;
    } else {
        PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
    }
    Runtime_ScheduleCallbackAndReleaseBlock20B();
    WaitFrames(1);
    return result;
}

s32 GameFlag_TestFar(s32 flag);

#define FLAG_WORKSPACE_SETTINGS_ONLY 0x17e
#define WORKSPACE_ROWS 3
#define WORKSPACE_DEBUG_ROWS 3
#define WORKSPACE_ROW_HEIGHT 24
#define WORKSPACE_TEXT_X 48

extern u8 gDebugMode;

void Menu_RunSelectedWorkspaceEntry(void)
{
    struct WorkspaceWork *work = gSelectionWork;
    u32 index;

    ShopCursor_AdvanceFar(&work->cursor);
    index = work->page;
    Ui_ApplyTableScaleToObject(work->icon[index]);
}

void Menu_InitializeSelectedWorkspace(void)
{
    void *work;
    volatile u32 zero;
    work = Runtime_AllocateBlock(20, 0x628);
    zero = 0;
    Dma_Set(&zero, work, 0x8500018a, REG_DMA3);
    Scheduler_AddOrUpdateCallback(Menu_RunSelectedWorkspaceEntry, 3200);
}

void Runtime_ScheduleCallbackAndReleaseBlock20A(void)
{
    Scheduler_RemoveCallback(&Menu_RunSelectedWorkspaceEntry);
    Runtime_ReleaseHeapBlock(0x14);
}

struct RenderInput *Menu_CreateWorkspaceWindows(void)
{
    struct WorkspaceWork *work;
    s32 rows;
    s32 first;
    s32 settings_only;
    s32 top;
    s32 height;
    struct RenderInput *win;
    s32 y;
    s32 slot;
    s32 msg;
    s32 i;
    s32 x;

    rows = WORKSPACE_ROWS;
    work = gSelectionWork;
    settings_only = GameFlag_TestFar(FLAG_WORKSPACE_SETTINGS_ONLY);
    first = 0;
    if (settings_only != 0) {
        rows = 1;
        first = 2;
    }
    if (gDebugMode != 0)
        rows += WORKSPACE_DEBUG_ROWS;
    top = 8 - rows;
    height = rows * 3 + 1;
    if (top + height > 19) {
        top = 1;
        height = 19;
    }
    win = UiWindow_Create(5, top, 20, height, 2);
    for (i = 1; i < rows; i++)
        UiWindow_DrawDividerLine(win, 0, i * 3, 19, i * 3);
    y = 4;
    if (settings_only == 0) {
        msg = (s32)MsgWorkspaceSaveQuest;
        UiText_DrawResource(msg, win, WORKSPACE_TEXT_X, 4);
        msg++;
        UiText_DrawResource(msg, win, WORKSPACE_TEXT_X, 4 + WORKSPACE_ROW_HEIGHT);
        y = 4 + 2 * WORKSPACE_ROW_HEIGHT;
    }
    UiText_DrawResource((s32)MsgWorkspaceChangeSettings, win, WORKSPACE_TEXT_X, y);
    y += WORKSPACE_ROW_HEIGHT;
    if (gDebugMode != 0) {
        msg = (s32)MsgWorkspaceRoughMenu;
        UiText_DrawResource(msg, win, WORKSPACE_TEXT_X, y);
        y += WORKSPACE_ROW_HEIGHT;
        UiText_DrawResource(msg + 1, win, WORKSPACE_TEXT_X, y);
        y += WORKSPACE_ROW_HEIGHT;
        msg += 2;
        UiText_DrawResource(msg, win, WORKSPACE_TEXT_X, y);
    }
    slot = Resource_FindFreeEntry();
    if (slot < RESOURCE_SLOTS) {
        VramBlock_LoadCached(slot, 128, Resource_FixedBlockBTiles);
        work->cursor.output = RenderOutput_Create(slot, 0x40000000, win, 0, 0);
        x = win->x * 8;
        y = win->y * 8 + 16;
        ShopCursor_SetPositionImmediateFar(&work->cursor, x, y);
    }
    y = -4;
    for (i = 0; i < rows; i++) {
        work->icon[i] = RenderResource_CreateFrame(Menu_WorkspaceIconFrames[first + i], 0, win, 12, y);
        y += WORKSPACE_ROW_HEIGHT;
    }
    return win;
}

#define gWindowWork (*(struct UiTileSlotWork * *)&gWindowWork)
#define UiIcon_PrepareObjectFar ((void (*)(void*))UiIcon_PrepareObjectFar)

s32 Menu_RunWorkspaceSelectionLoop(void)
{
    struct RenderInput *window;
    s32 redraw;
    struct RenderOutput *pair;
    s32 first;
    s32 i;
    s32 j;
    s32 rows;
    s32 settings_only;
    s32 dim;
    s32 y;
    s32 result;
    s32 page;
    s32 index;
    s32 x;
    s32 top;
    s8 *tbl;
    struct RenderOutput *icon;
    struct WorkspaceWork *work;

    redraw = 1;
    rows = 3;
    settings_only = GameFlag_TestFar(0x17E);
    first = 0;
    Menu_InitializeSelectedWorkspace();
    work = gSelectionWork;
    window = Menu_CreateWorkspaceWindows();
#if EDITION_INTERNATIONAL
    y = -0x18;
    if (gDebugMode != 0) {
        y = -0x10;
    }
    pair = RenderResource_CreatePair(6, window, 0x28, y);
#else
    pair = RenderResource_CreatePair(6, window, 0x28, -0x18);
#endif
    WaitFrames(1);
    page = work->page;
    if (settings_only != 0) {
        rows = 1;
        first = 2;
    }
    if (gDebugMode != 0) {
        rows += 3;
    }
redraw_menu:
    if (redraw != 0) {
        redraw = 0;
        page = (page + rows) % rows;
        work->page = page;
        i = 0;
        if (i < rows) {
            j = first;
            tbl = Menu_WorkspaceIconFrames;
            do {
                icon = work->icon[i];
                icon->sentinel = 0xFB;
                UiIcon_PrepareObjectFar(icon);
                index = (u8)icon->index;
                dim = 0;
                if (i != work->page) {
                    dim = 1;
                }
                RenderResource_LoadFrame(*(s8 *)(j + (s32)tbl), index, dim);
                i++;
                j++;
            } while (i < rows);
        }
        top = window->y;
        x = window->x * 8;
        y = (((page * 3) + top) * 8) + 0x10;
        Shop_SetCursorFar(
            &work->cursor,
            x,
            y,
            3);
    }
    Ui_ApplyTableOffsetToPair(pair);
    WaitFrames(1);
    if (gKeyState & KEY_A) {
        result = page;
        AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
    } else if (gKeyState & (KEY_B | KEY_START)) {
        result = -1;
        AudioCommand_PlayFar(SOUND_MENU_CANCEL);
    } else {
        if (gKeysRepeat & KEY_UP) {
            AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
            page -= 1;
            redraw = 1;
        } else if (gKeysRepeat & KEY_DOWN) {
            AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
            page += 1;
            redraw = 1;
        }
        goto redraw_menu;
    }
    UiWork_Finalize(window, 2);
    Runtime_ScheduleCallbackAndReleaseBlock20A();
    WaitFrames(1);
    if (result >= 0) {
        result += first;
    }
    return result;
}

struct UiTileSlotWork {
    u8 unknown_000[RENDER_TILE_ATTR_OFS];
    u8 used[0x100];
    u16 next;
};

void UiWindow_OverlayTilePattern(u16 *entry, u16 *mirror, s32 pattern, u8 *remap)
{
    struct UiTileSlotWork *work = gWindowWork;
    u8 pixels[128];
    u8 *sheet = Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
    u32 slot = *(u8 *)entry;
    u8 *src;
    u8 *dst;
    u32 n;
    u32 i;

    dst = pixels;
    src = (u8 *)(0x06000000 + 32 * slot);
    for (i = 0; i < 32; i++) {
        n = *src++;
        *dst++ = n & 15;
        *dst++ = n >> 4;
    }

    dst = pixels;
    src = sheet + pattern * 32;
    for (i = 0; i < 32; i++) {
        u32 value = *src++;
        u32 color = remap[value & 15];

        if (color != 0)
            *dst = color;
        dst++;
        color = remap[value >> 4];
        if (color != 0)
            *dst = color;
        dst++;
    }

    {
        u8 *p;

        dst = pixels;
        for (i = 0, p = dst; i < 32; i++) {
            u32 value = p[0] | p[1] << 4;

            p += 2;
            *dst++ = value;
        }
    }

    if ((s8)slot >= 0) {
        for (n = 0; n < 128; n++) {
            u32 value = work->next;
            u32 following = (value + 1) % 128;

            slot = (u8)value;
            work->next = following;
            if (work->used[slot] == 0)
                break;
        }
        work->used[slot] = 1;
        slot |= 0x80;
        *entry = slot | 0xf000;
        *mirror = slot | 0xf000;
    }
    Dma_Set(pixels, (void *)(0x06000000 + slot * 32), 0x84000008, REG_DMA3);
}

#undef UiWindow_DrawDividerLine
#undef UiText_DrawResource

#undef UiText_DrawCharacterAtOffset
#undef gWindowWork

s32 UiText_RenderStringTiles(u16 *script, u16 *tiles, u16 *mirror, s32 x)
{
    /* FAKEMATCH: needed to match: an unread 32-byte reserve, the native frame's extra bytes (the frame is 36 bytes without it). */
    u32 reserve[8];
    u8 pens[16];
    struct UiRenderWork *work;
    u8 *canvas;
    u8 *glyphs;
    u32 attr;
    u32 *src;
    u8 *dst;
    u32 c;
    u32 code;
    s32 count;
    u32 w;
    s32 i;
    s32 j;
    u32 bits;

    work = (struct UiRenderWork *)gWindowWork[0];
    canvas = MemAlloc(0x800);
    glyphs = Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
    attr = work->level << 12;
    Iwram_ClearWords(pens, 16);
    if (attr == 0xF000) {
        pens[1] = UiText_ColourPens[work->colour & 15];
        pens[3] = 3;
        Iwram_FillWords(canvas, 0x800, 0x04040404);
    } else {
        pens[1] = (u8)work->colour & 15;
        pens[3] = 1;
        Iwram_FillWords(canvas, 0x800, 0x0E0E0E0E);
    }
    if (script != 0) {
        for (;;) {
            c = *script++;
            code = c;
            if (c == 0)
                break;
            if (c <= 30) {
                switch (c) {
                case 8:
                    work->colour = *script++;
                    pens[1] = UiText_ColourPens[work->colour & 15];
                    break;
                case 7:
                case 9:
                case 10:
                    work->colour = 15;
                    pens[1] = UiText_ColourPens[work->colour];
                    break;
                case 3:
#if EDITION_INTERNATIONAL
                    x += UiText_GlyphWidths[0];
#else
                    x += 8;
#endif
                    break;
                case 14:
                case 15:
                case 28:
                    script++;
                case 11:
                case 12:
                case 17:
#if EDITION_INTERNATIONAL
                case 29:
#endif
                    script++;
                    break;
                }
            } else {
                c &= 0xFF;
#if !EDITION_INTERNATIONAL
                if (c == 222 || c == 223) {
                    x -= 3;
                    c += 32;
                }
#endif
                src = (u32 *)(glyphs + c * 32);
                dst = canvas + x;
                for (i = 0; i < 8; i++) {
                    bits = *src++;
                    for (j = 0; j < 4; j++) {
                        u8 pen = pens[bits & 15];

                        if (pen != 0)
                            *dst = pen;
                        dst++;
                        bits >>= 4;
                        pen = pens[bits & 15];
                        if (pen != 0)
                            *dst = pen;
                        dst++;
                        bits >>= 4;
                    }
                    dst += 248;
                }
                if (code == 0xF01D || code == 0xF01F)
                    x += 8;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
                else if (code == 0xF01E || code == 0xF005)
#else
                else if (code == 0xF01E)
#endif
                    x += 3;
                else {
#if !EDITION_INTERNATIONAL
                    if (c > 128) {
                        if (c == 254 || c == 255)
                            w = 3;
                        else
                            w = 8;
                    } else
#endif
                    if (c > 31)
                        w = UiText_GlyphWidths[c - 32];
                    else
                        w = 1;
                    x += w;
                }
            }
        }
    }

    count = (u32)(x + 7) >> 3;
    src = (u32 *)canvas;
    dst = canvas;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < count; j++) {
            c = src[0];
            bits = src[1];
            c |= c << 4;
            bits |= bits >> 4;
            c &= 0x0FF00FF0;
            bits &= 0x00FF00FF;
            c |= c << 8;
            bits |= bits >> 8;
            src += 2;
            *(u32 *)dst = (c << 4) >> 16 | bits << 16;
            dst += 4;
        }
        dst = dst - count * 4 + 0x100;
        src = (u32 *)((u8 *)src - count * 8 + 0x100);
    }

    for (i = 0; i < count; i++) {
        u32 mask = 127;
        u32 tile;
        u32 *p;

        if (work->alt != 0)
            mask = 255;
        tile = *tiles & 0x3FF;
        if ((tile < 0x80 || tile > 0xFF) &&
            (work->alt == 0 || tile < 0x200 || tile > 0x27F)) {
            tile = work->tile_cursor & mask;
            for (j = 0; j <= mask && work->tile_attributes[tile] != 0; j++)
                tile = (tile + 1) & mask;
            work->tile_cursor = (tile + 1) & mask;
            work->tile_attributes[tile] = 1;
            if (tile > 127)
                tile += 0x180;
            else
                tile |= 0x80;
            *tiles = attr | tile;
            *mirror = attr | tile;
        }
        p = (u32 *)(0x06000000 + tile * 32);
        *p++ = ((u32 *)canvas)[i];
        *p++ = ((u32 *)canvas)[i + 0x40];
        *p++ = ((u32 *)canvas)[i + 0x80];
        *p++ = ((u32 *)canvas)[i + 0xC0];
        *p++ = ((u32 *)canvas)[i + 0x100];
        *p++ = ((u32 *)canvas)[i + 0x140];
        *p++ = ((u32 *)canvas)[i + 0x180];
        *p++ = ((u32 *)canvas)[i + 0x1C0];
        tiles++;
        mirror++;
    }
    MemFree(canvas);
    return count;
}

void UiWindow_ClearTileAttributesInRect(s32 x, s32 y, u32 width, u32 height)
{
    u8 *base = gWindowWork[0];
    u16 *cursor = (u16 *)((y * 32 + x) * 2 + (u32)base);
    u32 alternate = ((struct UiRenderWork *)base)->alt;
    u32 row;

    for (row = 0; row < height; row++) {
        u32 column;

        for (column = 0; column < width; column++) {
            u32 tile = *cursor++ & 0x3FF;

            if ((tile - 0x80) <= 0x7F ||
                (alternate != 0 &&
                 tile > 0x1FF &&
                 tile <= 0x27F)) {
                u32 index = ((tile & 0xFF) ^ 0x80);
                ((struct UiRenderWork *)base)->tile_attributes[index] &= 0xFC;
            }
        }
        cursor += 32 - width;
    }
}

void UiWindow_MarkVisibleTileAttributes(void)
{
    u8 *base = gWindowWork[0];
    u16 *cursor = (u16 *)base;
    s32 width = 30;
    u32 alternate = ((struct UiRenderWork *)base)->alt;
    s32 row;
    s32 x;

    for (row = 20; row != 0; row--) {
        for (x = 0; x < width; x++) {
            u32 tile = *cursor++ & 0x3ff;

            if ((tile - 0x80) <= 0x7f ||
                (alternate != 0 && tile > 0x1ff && tile <= 0x27f)) {
                u32 index = ((tile & 0xff) ^ 0x80);

                ((struct UiRenderWork *)base)->tile_attributes[index] |= 2;
            }
        }
    }

    for (row = 0; row < 256; row++, base++) {
        if (((struct UiRenderWork *)base)->tile_attributes[0] == 1)
            ((struct UiRenderWork *)base)->tile_attributes[0] = 0;
    }
}

void UiWork_SetAltFlagAndClearTable(s32 flag)
{
    s32 i;
    s32 j;
    s8 *p;
    s8 *q;
    u8 *work;

    work = gWindowWork[0];
    if (flag != 0) {
        ((struct UiRenderWork *)work)->alt = 1;
        flag = 0;
        for (i = 0x80, p = (s8 *)((struct UiRenderWork *)work)->tile_attributes + 0x80; i <= 0xFF; i += 1) {
            *p = flag;
            p += 1;
        }
        return;
    }
    ((struct UiRenderWork *)work)->alt = 0;
    flag = 0;
    q = (s8 *)((struct UiRenderWork *)work)->tile_attributes + 0x80;
    j = 0x7F;
    do {
        j -= 1;
        *q = flag;
        q += 1;
    } while (j >= 0);
}

void UiRender_ReservedNoOp(void)
{
}

void UiWindow_DrawDividerLine(struct RenderInput *win, u32 x1, u32 y1, u32 x2, u32 y2)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
    u16 *cursor;
    u32 pos;
    u32 tile;

    if (x1 == x2) {
        if (y1 == y2)
            return;
        if (y1 > y2) {
            pos = y1;
            y1 = y2;
            y2 = pos;
        }
        UiWindow_ClearTileAttributesInRect(win->x + x2, win->y + y1, 1, y2 - y1);
        cursor = (u16 *)((((win->y + y1) << 5) + win->x + x2) * 2 + (u32)work->tilemap);
        for (pos = y1; pos <= y2; pos++) {
            tile = *cursor;
            if (pos == y1) {
                switch (tile) {
                case 0xf009:
                case 0xf00b:
                case 0xf00c:
                case 0xf00d:
                case 0xf018:
                    break;
                case 0xf011:
                    tile = 0xf018;
                    break;
                case 0xf00a:
                    tile = 0xf00d;
                    break;
                case 0xf00e:
                    tile = 0xf009;
                    break;
                default:
                    tile = 0xf00f;
                    break;
                }
            } else if (pos == y2) {
                switch (tile) {
                case 0xf00a:
                case 0xf00b:
                case 0xf00c:
                case 0xf00d:
                case 0xf019:
                    break;
                case 0xf014:
                    tile = 0xf019;
                    break;
                case 0xf009:
                    tile = 0xf00d;
                    break;
                case 0xf00e:
                    tile = 0xf00a;
                    break;
                default:
                    tile = 0xf00f;
                    break;
                }
            } else {
                switch (tile) {
                case 0xf00b:
                case 0xf00c:
                case 0xf00d:
                    break;
                case 0xf009:
                case 0xf00a:
                case 0xf00e:
                    tile = 0xf00d;
                    break;
                default:
                    tile = 0xf00f;
                    break;
                }
            }
            *cursor = tile;
            cursor += 32;
        }
    } else if (y1 == y2) {
        if (x1 == x2)
            return;
        if (x1 > x2) {
            pos = x1;
            x1 = x2;
            x2 = pos;
        }
        UiWindow_ClearTileAttributesInRect(win->x + x1, win->y + y1, x2 - x1, 1);
        cursor = (u16 *)((((win->y + y1) << 5) + win->x + x1) * 2 + (u32)work->tilemap);
        for (pos = x1; pos <= x2; pos++) {
            tile = *cursor;
            if (pos == x1) {
                switch (tile) {
                case 0xf009:
                case 0xf00a:
                case 0xf00b:
                case 0xf00d:
                case 0xf01a:
                    break;
                case 0xf016:
                    tile = 0xf01a;
                    break;
                case 0xf00c:
                    tile = 0xf00d;
                    break;
                case 0xf00f:
                    tile = 0xf00b;
                    break;
                default:
                    tile = 0xf00e;
                    break;
                }
            } else if (pos == x2) {
                switch (tile) {
                case 0xf009:
                case 0xf00a:
                case 0xf00c:
                case 0xf00d:
                case 0xf01b:
                    break;
                case 0xf017:
                    tile = 0xf01b;
                    break;
                case 0xf00b:
                    tile = 0xf00d;
                    break;
                case 0xf00f:
                    tile = 0xf00c;
                    break;
                default:
                    tile = 0xf00e;
                    break;
                }
            } else {
                switch (tile) {
                case 0xf009:
                case 0xf00a:
                case 0xf00d:
                    break;
                case 0xf00b:
                case 0xf00c:
                case 0xf00f:
                    tile = 0xf00d;
                    break;
                default:
                    tile = 0xf00e;
                    break;
                }
            }
            *cursor = tile;
            cursor++;
        }
    }
}

void UiWork_SetParamNibble(s32 param)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];

    work->colour = param & 15;
}

void UiWork_SetRenderWord(u16 value)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];

    work->outline = value;
}

void UiText_DrawResource(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    struct UiRenderWork *canvas = (struct UiRenderWork *)gWindowWork[0];
    u16 *counter = &canvas->count;
    s32 zero = 0;

    *counter = zero;
    UiText_BuildRenderEntries(arg0, 1);
    canvas->entries[*counter] = zero;
    *counter = (*counter + 1) & RENDER_ENTRY_MASK;
    UiText_RenderWideStringAtOffset(canvas->entries, (struct UiWindow *)arg1, arg2, arg3);
}

void UiText_DrawCharacterAtOffset(
    s32 character,
    struct RenderInput *position,
    u32 offset_x,
    u32 offset_y)
{
    s32 byte_offset;
    s32 vram_address;
    u16 *text;
    s32 zero;
    u32 cell;
    struct UiRenderWork *canvas;
    u16 *counter;

    canvas = (struct UiRenderWork *)gWindowWork[0];
    counter = &canvas->count;
    zero = 0;
    *counter = zero;
    UiText_BuildRenderEntries(character, 1);

    canvas->entries[*counter] = zero;
    *counter = (*counter + 1) & RENDER_ENTRY_MASK;

    cell = ((position->y + (offset_y >> 3) + 1) << 5)
        + (position->x + (offset_x >> 3)) + 1;
    if (cell < 0x280U) {
        byte_offset = cell * 2;
        vram_address = byte_offset + 0x06002000;
        text = canvas->entries;
        UiText_RenderStringTiles(
            text,
            (u16 *)((u8 *)canvas->tilemap + byte_offset),
            (u16 *)vram_address,
            7 & offset_x);
    }
}

void UiText_DrawString(u8 *text, s32 arg1, s32 arg2, s32 arg3)
{
    s16 *buffer;
    s16 *output;
    u8 *input;

    input = text;
    buffer = Runtime_BumpAllocateAlternatePool(0x200);
    output = buffer;
    if (*input != 0) {
        do {
            *output = *input;
            input++;
            output++;
        } while (*input != 0);
    }
    *output = 0;
    UiText_RenderWideStringAtOffset((u16 *)buffer, (struct UiWindow *)arg1, arg2, arg3);
    MemFree(buffer);
}

void UiText_DrawStringAtOffset(
    u8 *text,
    struct RenderInput *work,
    s32 offset_x,
    s32 offset_y)
{
    s16 *buffer;
    s16 *output;
    struct UiRenderWork *canvas;
    u32 cell;
    s32 vram;
    s32 src;
    s32 dst;
    s32 phase;

    buffer = Runtime_BumpAllocateAlternatePool(0x200);
    canvas = (struct UiRenderWork *)gWindowWork[0];
    output = buffer;
    if (*text != 0) {
        do {
            *output = *text;
            text++;
            output++;
        } while (*text != 0);
    }
    *output = 0;

    cell = ((work->y + ((u32)offset_y >> 3) + 1) << 5)
        + (work->x + ((u32)offset_x >> 3)) + 1;
    if (cell < 0x280) {
        vram = 0x06002000;
        cell *= 2;
        dst = vram + cell;
        src = (s32)canvas->tilemap + cell;
        phase = offset_x & 7;
        UiText_RenderStringTiles(
            (u16 *)buffer,
            (u16 *)src,
            (u16 *)dst,
            phase);
        MemFree(buffer);
    }
}

void UiText_DrawStringInWindow(u8 *text, s32 arg1, u32 x, u32 y)
{
    u16 *buffer = Runtime_BumpAllocateAlternatePool(0x200);
    u16 *output = buffer;

    while (*text != 0) {
        *output = *text;
        text++;
        output++;
    }
    *output = 0;
    x >>= 3;
    y >>= 3;
    UiText_RenderWideStringInWindow((s16 *)buffer, (struct UiWindow *)arg1, x, y);
    MemFree(buffer);
}

void UiText_DrawNumber(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    u8 data[16];

    UiText_DrawString(UiText_FormatNumber(data, arg0, arg1), arg2, arg3, arg4);
}

void UiText_DrawNumberAtOffset(
    s32 arg0,
    s32 arg1,
    struct RenderInput *arg2,
    s32 arg3,
    s32 arg4)
{
    u8 data[16];

    UiText_DrawStringAtOffset(UiText_FormatNumber(data, arg0, arg1), arg2, arg3, arg4);
}

void UiText_DrawNumberInWindow(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    u8 data[16];

    UiText_DrawStringInWindow(UiText_FormatNumber(data, arg0, arg1), arg2, arg3, arg4);
}

void UiText_DrawPrefixedNumberAtOffset(
    s32 value,
    struct RenderInput *work,
    s32 offset_x,
    s32 offset_y,
    s32 variant)
{
    u8 formatted[16];
    s16 output[8];
    u8 *text;
    s32 index;
    u32 cell;
    struct UiRenderWork *canvas;
    s32 vram;
    s32 src;
    s32 dst;
    s32 phase;

    canvas = (struct UiRenderWork *)gWindowWork[0];
    text = UiText_FormatNumber(formatted, value, 4);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || \
    defined(TBS_EDITION_IT)
    if (variant == 0) {
        output[0] = 0xf01d;
        output[1] = 0xf01e;
    } else {
        output[0] = 0xf01f;
        output[1] = 0xf005;
    }
#else
    if (variant == 0) {
        output[0] = 0xf01d;
    } else {
        output[0] = 0xf01f;
    }
    output[1] = 0xf01e;
#endif
    for (index = 0; index <= 4; index++) {
        output[index + 2] = text[index];
    }
    output[6] = 0;

    cell = ((work->y + ((u32)offset_y >> 3) + 1) << 5)
        + (work->x + ((u32)offset_x >> 3)) + 1;
    if (cell < 0x280) {
        vram = 0x06002000;
        cell *= 2;
        dst = vram + cell;
        src = (s32)canvas->tilemap + cell;
        phase = offset_x & 7;
        UiText_RenderStringTiles(
            (u16 *)output,
            (u16 *)src,
            (u16 *)dst,
            phase);
    }
}

#define Resource_LoadByMode ((int (*)())Resource_LoadByMode)

struct RenderOutput *RenderOutput_AcquireFree(void);
#define RenderOutput_AppendToList ((s32 (*)( ))RenderOutput_AppendToList)

struct RenderOutput *RenderOutput_Create(
    s32 slot,
    s32 flags,
    struct RenderInput *input,
    s32 offset_x,
    s32 offset_y)
{
    s32 x;
    struct RenderOutput *output;
    s32 y;

    output = RenderOutput_AcquireFree();
    if (output == NULL) {
        Resource_ResetEntry(slot);
        return 0;
    }
    x = offset_x + (input->x * 8) + 8;
    y = offset_y + (input->y * 8) + 8;
    x &= 0x1ff;
    y &= 0xff;
    *(u32 *)&output->oam = (x << 16) | y | flags;
    ((u32 *)&output->oam)[1] =
        ((struct VramBlockCacheEntry *)Ram_VramBlockCache)[slot].offset >> 5;
    output->sentinel = 0xff;
    output->next = 0;
    output->x = x;
    output->y = y;
    output->index = slot;
    output->kind = 1;
    output->active = 1;
    RenderOutput_AppendToList(input, output);
    return output;
}

struct RenderOutput *RenderOutput_CreateFromResource(
    s32 mode,
    s32 resource,
    struct RenderInput *input,
    s32 x,
    s32 y)
{
    s32 no;

    no = Resource_LoadByMode(mode, resource);
    if (no < 0) {
        return NULL;
    }
    return RenderOutput_Create(no, 0x40000000, input, x, y);
}

struct RenderOutput *RenderOutput_CreateLoaded(
    s32 code,
    s32 layers,
    struct RenderInput *input,
    s32 x,
    s32 y)
{
    s32 no;
    struct RenderOutput *result;

    no = Resource_FindFreeEntry();
    result = NULL;
    if (no != 0x60) {
        UiIcon_CopyResourceToSlot(code, layers, no);
        result = RenderOutput_Create(no, 0x40000000, input, x, y);
    }
    return result;
}

struct RenderOutput *RenderOutput_CreateWithTransform(
    s32 icon,
    struct RenderInput *input,
    s32 x,
    s32 y)
{
    s32 count;
    s32 unused;
    struct RenderOutput *result;

    count = Resource_FindFreeEntry();
    if (count == 0x60) {
        return NULL;
    }
    Ui_BuildPairedPatternsToSlot(icon, 1, &count, &unused, 1);
    result = RenderOutput_Create(count, 0x40000000, input, x, y);
    result->sentinel = 251;
    return result;
}

struct RenderOutput *RenderOutput_CreateFromTable(
    s32 table_entry,
    struct RenderInput *input,
    s32 x,
    s32 y)
{
    s32 slot;
    struct RenderOutput *output;

    slot = Resource_FindFreeEntry();
    output = NULL;
    if (slot != 0x60) {
        RenderResource_LoadTableEntry(table_entry, 0, slot);
        output = RenderOutput_Create(slot, 0x40000000, input, x, y);
    }
    return output;
}

#undef Resource_LoadByMode
#undef RenderOutput_AppendToList

#define UiGlyph_LoadEntryWithPalette ((void (*)(s32,s32,s32*,s32*,s32,s32))UiGlyph_LoadEntryWithPalette)

struct SideObjectRegistry {
#if EDITION_INTERNATIONAL
    u8 pad_0000[0x12ec];
    u16 state_ids[2];
    u16 state_values[2];
#else
    u8 pad_0000[0x117c];
    u16 state_ids[2];
    u16 state_values[2];
#endif
};

struct RenderOutput *CreateSideObject(
    s32 object_kind, s32 position, s32 side, s32 arg3, s32 arg4, s32 arg5)
{
    struct SideObjectRegistry *state = (struct SideObjectRegistry *)gWindowWork[0];
    struct RenderOutput *object = 0;
    s32 first;
    s32 second;
    s32 id;
    s32 slot;

    if (GameFlag_TestFar(32) != 0) {
        if (object_kind == 0)
            object_kind = 18;
        if (object_kind == 1)
            object_kind = 19;
    }

    id = Localization_LookupEntryId(object_kind);
    if (id == -1)
        return object;

    if ((u32)side > 1) {
        side = 1;
        if (state->state_ids[1] != 999) {
            side = 0;
            if (state->state_ids[0] != 999)
                return object;
        }
    }

    slot = 14 + side;
    UiGlyph_LoadEntryWithPalette(id, position, &first, &second, slot, 0);
    object = RenderOutput_Create(first, 0x80000000, (struct RenderInput *)arg3, arg4, arg5);
    if (object != 0) {
        object->oam.palette = slot;
        object->kind = 2;
    }

    state->state_ids[side] = id;
    state->state_values[side] = first;
    return object;
}

void Ui_LoadCharacterEntryForSlot(u32 slot, s32 character, s32 value)
{
    s32 result;
    s32 current;
    u32 character_id;
    struct SideObjectRegistry *state;

    state = (struct SideObjectRegistry *)gWindowWork[0];

    if (GameFlag_TestFar(0x20) != 0) {
        if (character == 0)
            character = 0x12;
        if (character == 1)
            character = 0x13;
    }

    character_id = Localization_LookupEntryId(character);
    if (character_id != -1U) {
        if (slot > 1U) {
            if (state->state_ids[1] == character_id) {
                slot = 1;
            } else if (state->state_ids[0] == character_id) {
                slot = 0;
            } else {
                return;
            }
        }
        current = state->state_values[slot];
        UiGlyph_LoadEntryWithPalette(character_id, value, &current, &result, slot + 0xe, 1);
    }
}

//--------------------------------------------------------------------------

// runtime_set_byte5_and_reset_halfword_c.c

// 表示：スプライト出力のアニメを切りかえる

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// active を mode にして、アニメの段（anim_step）を 0 に戻す
// out : スプライト出力（NULL なら何もしない）
//--------------------------------------------------------------------------
void RenderOutput_SetAnimation(struct RenderOutput *out, s32 mode)
{
	if (out != NULL)
	{
		out->active = mode;
		out->anim_step = 0;
	}
}

#undef RenderOutput_RedrawSavedRect
#undef UiWindow_Create

void RenderOutput_SetInvertedSentinel(void *arg0, s32 arg1)
{
    if (arg0 != 0)
        ((struct RenderOutput *)arg0)->sentinel = ~arg1;
}

#define TILE_ROW_FROM_BOTTOM(tile, up) ((u32 *)((tile) * 32 - (up) * 4 + 0x0600001c))

typedef void (*RamRoutine)(void *dst, void *scene);

void UiWindow_FillFromScene(void *dst)
{
    void *scene = gWindowWork[0];

    if (scene == 0) {
        volatile u16 fill = 0xe0e0;

        Dma_Set((void *)&fill, dst, 0x810000a0, REG_DMA3);
    } else {
        RamRoutine code;

        RamCode_Load(code, Tile_BuildMetatiles, Tile_BuildMetatilesCodeSize);
        code(dst, scene);
        MemFree(code);
    }
}

void UiWindow_FillScreenBlockRect(s32 unused0, s32 unused1, u32 width, u32 height,
                   s32 value)
{
    u32 row = 0;
    s16 *dst = (s16 *)0x06002000;

    if (row < height) {
        do {
            u32 column = 0;
            if (column < width) {
                do {
                    column++;
                    *dst = value;
                    dst++;
                } while (column < width);
            }
            row++;
            dst += 32 - width;
        } while (row < height);
    }
}

void UiWindow_BuildLayoutBounds(s32 flags)
{
    void **slot = (void **)&gPartyStatusWork;
    struct PartyStatusWindow *state = *slot;
    struct UiRenderWork *work = slot[-1];
    s32 height = 4;
    s32 n;
    s32 right;
    s32 left;

    if (work->menu_state != 0) {
        n = BattleParty_PrepareActiveOwnersFar(0);
        height = 3;
    } else {
        n = Party_CountActiveOwnersFar();
    }
    if (flags & 1)
        height++;
    else
        flags &= -3;

    n *= 6;
    right = n + 1;
    if (flags & 2)
        right += 5;

    left = 30;
    left -= right;
    state->x = left;
    state->y = 0;
    state->width = right;
    state->height = height;
    state->flags = flags;
}

void UiWindow_CreateWithLayoutBounds(s32 flags)
{
    s32 zero;
    struct PartyStatusWindow *window;
    s8 *busy;

    window = Runtime_AllocateBlock(16, sizeof(*window));
    busy = (s8 *)&((struct UiRenderWork *)gWindowWork[0])->menu_busy;
    zero = 0;
    *busy = 1;
    UiWindow_BuildLayoutBounds(flags);
    window->window = UiWindow_Create(
        window->x, window->y, window->width, window->height, 6);
    PtyStDraw(flags);
    *busy = zero;
}

void UiWindow_DrawColumnBorders(struct RenderInput *window, u32 flags)
{
    struct UiRenderWork *canvas = (struct UiRenderWork *)gWindowWork[0];
    s32 first = 1;
    s32 bias = 0;
    u32 max = window->width - 1;
    s32 i;
    u32 rows = window->height;
    u32 col;
    u32 row;
    u16 *dest;

    if ((flags & 1) == 0)
        flags &= ~2;
    if (flags & 2) {
        bias = 5;
        first = 0;
    }
    i = first;
    while (UiWindow_PartyColumnOffsets[i] >= 0) {
        col = UiWindow_PartyColumnOffsets[i] + bias;
        if (col < max) {
            for (row = 0; row != rows; row++) {
                dest = (u16 *)(((window->y + row) * 32 + (window->x + col)) * 2 + (u32)canvas->tilemap);
                if (row == 0)
                    *dest = 0xf018;
                else if (row == rows - 1)
                    *dest = 0xf019;
                else
                    *dest = 0xf00f;
            }
        }
        i++;
    }
    if (canvas->menu_state) {
        dest = canvas->tilemap + (window->y + window->height - 1) * 32 + window->x;
        col = 1;
        *dest++ = 0xf080;
        while (col < max) {
            col++;
            *dest++ = 0xf081;
        }
        *dest = 0xf082;
    }
    canvas->dirty = 1;
}

s32 UiWindow_DrawStatusBarTiles(struct RenderInput *window, s32 x, s32 y, s32 value)
{
    // 戻り値の型は s32 だが return 文は無い（pop {r1}; bx r1）
    s32 i;
    struct UiRenderWork *canvas = (struct UiRenderWork *)gWindowWork[0];
    s32 filled = value;
    s32 row;
    s32 col;
    u32 id;
    u32 light;
    u32 dark;
    u32 pixels;
    u32 result;

    if (canvas->menu_state == 0) {
        Dma_Set((const void *)Runtime_GetLowTableAddress(), (void *)&BG_PLTT_COLOR(14, 0),
            0x80000010, REG_DMA3);
        BG_PLTT_COLOR(14, 14) = BG_PLTT_COLOR(15, 4);
    }
    x += window->x;
    y += window->y;
    for (i = 0; i < 5; i++, x++) {
        id = canvas->tilemap[y * 32 + x];
        light = 0x22222222;
        dark = 0xcccccccc;
        id &= 0x3ff;
        if (value > 7) {
            light = 0x88888888;
            dark = 0xdddddddd;
        } else if (value >= 0) {
            light <<= value * 4;
            light |= 0x88888888U >> (32 - value * 4);
            dark <<= value * 4;
            dark |= 0xddddddddU >> (32 - value * 4);
        }
        for (row = 0; row <= (filled != 0 ? 2 : 0); row++) {
            pixels = *TILE_ROW_FROM_BOTTOM(id, row);
            result = 0;
            for (col = 0; col <= 7; col++) {
                u32 color = pixels & 15;

                if (color == 14)
                    result |= light & (15U << (col * 4));
                else if (color == 1)
                    result |= dark & (15U << (col * 4));
                else
                    result |= color << (col * 4);
                pixels >>= 4;
            }
            *TILE_ROW_FROM_BOTTOM(id, row) = result;
        }
        value -= 8;
    }
}

void UiWindow_DrawFrame(s32, s32, u32, u32);
void UiText_DrawStringAtOffset(u8 *, struct RenderInput *, s32, s32);
s32 __divsi3(s32, s32);
#define UiText_DrawPrefixedNumberAtOffset ((s32 (*)())UiText_DrawPrefixedNumberAtOffset)
s32 UiWindow_PutGlyph();

void PtyStDraw(s32 flags)
{
    void **slot = (void **)gPartyStatusWork;
    struct PartyStatusWindow *layout = slot[0];
    struct BtlWork *party = slot[-7];
    struct RenderInput *window = (struct RenderInput *)layout->window;
    struct UiRenderWork *work = slot[-1];
    s32 x = 0;
    u32 count;
    s32 y = 0;
    u32 i;
    u16 owners[5];
    u8 djinn[4];

    if (work->menu_state != 0) {
        count = BattleParty_PrepareActiveOwnersFar(0);
        y = -1;
        for (i = 0; i < count; i++) {
            owners[i] = party->party_units[i];
            if (owners[i] == 255)
                break;
        }
    } else {
        count = Party_CountActiveOwnersFar();
        for (i = 0; i < count; i++)
            owners[i] = gGameState.active_owners[i];
        owners[i] = 255;
    }
    count = i;
    if (flags == -1)
        flags = layout->flags;
    if (!(flags & 1))
        flags &= ~2;
    if (work->menu_state == 0 || BattlePlacement_CountValidEntriesFar(0, 0) == 0)
        flags &= ~2;
    if (flags == 9) {
        UiWindow_EraseBorderRect(layout->x, layout->y, layout->width, layout->height);
        return;
    }
    work->menu_busy = 1;
    if (layout->flags == flags) {
        RenderOutput_RedrawSavedRect((struct UiWindow *)window);
        UiWindow_DrawColumnBorders(window, flags);
    } else {
        UiWindow_EraseBorderRect(layout->x, layout->y, layout->width, layout->height);
        UiWindow_BuildLayoutBounds(flags);
        window->width = layout->width;
        window->height = layout->height;
        window->x = layout->x;
        UiWindow_DrawFrame(layout->x, layout->y, layout->width, layout->height);
        UiWindow_DrawColumnBorders(window, flags);
    }
    if (flags & 2)
        x = 5;
    for (i = 0; i != count; i++) {
        struct BtlUnit *unit = UnitGetFar(owners[i]);
        s32 hp = unit->hp;
        s32 maximum = unit->max_hp;
        s32 value;
        s32 current;

        if (hp == 0)
            UiWork_SetParamNibble(2);
        else if (hp <= maximum / 4)
            UiWork_SetParamNibble(4);
        else
            UiWork_SetParamNibble(15);
        work->level = 14;
        if (work->menu_state != 0)
            work->level = 5;
        UiText_DrawPrefixedNumberAtOffset(hp, window, (x + i * 6) * 8, y * 8 + 8, 0);
        work->level = 15;
        UiText_DrawStringAtOffset(unit->name, window, (x + i * 6) * 8, y * 8);
        UiWork_SetParamNibble(15);
        if (unit->max_hp != 0) {
            current = unit->hp;
            value = __divsi3(current * 40, unit->max_hp);
            if (value == 0 && current != 0)
                value = 1;
            UiWindow_DrawStatusBarTiles(window, x + i * 6 + 1, y + 2, value);
        }
        if (flags & 1) {
            work->level = 14;
            if (work->menu_state != 0)
                work->level = 5;
            UiText_DrawPrefixedNumberAtOffset(unit->ep, window, (x + i * 6) * 8, y * 8 + 16, 1);
            if (unit->max_ep != 0) {
                current = unit->ep;
                value = __divsi3(current * 40, unit->max_ep);
                if (value == 0 && current != 0)
                    value = 1;
                UiWindow_DrawStatusBarTiles(window, x + i * 6 + 1, y + 3, value);
            }
        }
    }
    work->level = 15;
    if (work->menu_state != 0 && (flags & 2)) {
        s32 row = y;

        if (flags & 1)
            row++;
        BattlePlacement_CountValidEntriesFar(0, djinn);
        UiWindow_SetTilemapEntry((struct UiWindow *)window, 0x5001, 0, row, 0);
        UiWindow_SetTilemapEntry((struct UiWindow *)window, 0x5002, 2, row, 0);
        UiWindow_SetTilemapEntry((struct UiWindow *)window, 0x5003, 0, row + 1, 0);
        UiWindow_SetTilemapEntry((struct UiWindow *)window, 0x5004, 2, row + 1, 0);
        UiWindow_PutGlyph(window, djinn[0] + '0', 1, row);
        UiWindow_PutGlyph(window, djinn[1] + '0', 3, row);
        UiWindow_PutGlyph(window, djinn[2] + '0', 1, row + 1);
        UiWindow_PutGlyph(window, djinn[3] + '0', 3, row + 1);
    }
    work->menu_busy = 0;
}

#define Runtime_GetBuildStampTimeFar ((u32 (*)(s32))Runtime_GetBuildStampTimeFar)

#define gTitleExtraOptionEnabled (*(s16 *)&gTitleExtraOptionEnabled)

extern u8 gSceneState[];
extern u32 gLoadedStateWord;
extern u32 GameFlagBytes[];

void UiText_ShowPositionedMessageAndWait(s32, s32);

void UiWork_FinalizeAndReleaseBlock16(void)
{
    UiWork_Finalize((struct UiWindow *)**(s32 **)(u32)&gWindowWork[1], 1);
    Runtime_ReleaseHeapBlock(0x10);
}

void UiWindow_SetTileAttributeBitRect(
    const struct RenderInput *window, s32 x, s32 y, s32 width, s32 height, u32 field)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];

    x += window->x + 1;
    y += window->y + 1;
    field &= 1;
    field <<= 12;
    if (x < 0) {
        width += x;
        x = 0;
    }
    if (x + width > 29) {
        width = 30 - x;
    }
    if (y < 0) {
        height += y;
        y = 0;
    }
    if (y + height > 29) {
        height = 20 - y;
    }
    if (width > 0 && height > 0) {
        y <<= 6;
        x = y + (x << 1);
        do {
            u16 *cell = (u16 *)(x + (u32)work->tilemap);
            s32 remaining = width;
            while (remaining != 0) {
                u32 value = *cell;
                value &= 0xFFFFEFFF;
                value |= field;
                remaining--;
                *cell = value;
                cell++;
            }
            height--;
            x += 64;
        } while (height != 0);
        work->dirty = 1;
    }
}

void *Text_FormatPlayTime(s32 value, u8 *out)
{
    u8 buf[64];
    u32 time;
    u32 minutes;
    s32 seconds;
    u8 *s;
    u8 *p;

    time = (u32)value / 0xe10;
    if (time > 0xea5f)
        time = 0xea5f;

    minutes = time / 60;
    seconds = time % 60;

    s = UiText_FormatNumber(buf, minutes, 3);
    *out = *s;
    s++;
    p = out + 1;
    *p = *s;
    p++;
    *p = s[1];
    seconds += 100;
    p++;
    *p = ':';

    s = UiText_FormatNumber(buf, seconds, 2);
    p++;
    *p = s[0];
    p++;
    *p = s[1];
    p[1] = 0;

    return out;
}

u32 SaveState_FindFreeSummarySlot(void)
{
    u32 i;
    struct SaveSummary *p;

    p = gSaveWorkspace->summary;
    for (i = 0; i < 3; i++, p++) {
        if (p->level == 0)
            return i;
    }
    return 0x3E7;
}

s32 SaveState_CountRecordsExcludingFlagged(s32 flag)
{
    s32 i;
    s32 cnt;
    struct SaveSummary *summary;

    if (SaveState_InitializeWorkspace() != 0) {
        cnt = -9;
    } else {
        cnt = SaveState_LoadSummaryRecords();
        if (flag != 0) {
            summary = gSaveWorkspace->summary;
            for (i = 0; i < 3; i++) {
                if ((s8)summary[i].send_flag != 0)
                    cnt--;
            }
        }
    }
    SaveState_ReleaseWorkspace();
    return cnt;
}

s32 SaveState_ScanRecordFlags(void)
{
    /* FAKEMATCH: the signed cursor from the party tail preserves the literal base changed by direct flag fields. */
    s32 err;
    s32 cnt;
    s32 ret;

    err = SaveState_InitializeWorkspace();
    cnt = 0;
    ret = -9;
    if (err == 0) {
        s32 i;
        s8 *flags;

        ret = SaveState_LoadSummaryRecords();
        flags = (s8 *)&gSaveWorkspace->summary[0].party[4];
        gTitleSendOptionEnabled = 0;
        gTitleExtraOptionEnabled = 0;
        for (i = 0; i < 3; i++) {
            if (flags[i * sizeof(struct SaveSummary) + ((u32)&((struct SaveSummary *)0)->send_flag - (u32)&((struct SaveSummary *)0)->party[4])] != 0) {
                gTitleSendOptionEnabled = 1;
                cnt++;
            }
            if (flags[i * sizeof(struct SaveSummary) + ((u32)&((struct SaveSummary *)0)->flag_count - (u32)&((struct SaveSummary *)0)->party[4])] != 0) {
                gTitleExtraOptionEnabled = 1;
            }
        }

        if ((gKeysHeld & (KEY_R | KEY_LEFT)) != (KEY_R | KEY_LEFT)) {
            gTitleSendOptionEnabled = 0;
        }
    }
    SaveState_ReleaseWorkspace();
    if (ret != 0 && cnt == ret) {
        return ret + 100;
    }
    return ret;
}

u32 SaveState_BuildSummaryHeader(void)
{
    u16 owners[14];
    struct SaveSummary *summary;
    struct BtlUnit *owner;
    u8 *source;
    u8 *destination;
    u32 *word;
    s32 i;
    u32 sum = 0;

    summary = (struct SaveSummary *)(gSaveBuffer - 16);
    gGameState.build_stamp = Runtime_GetBuildStampTimeFar(0);
    gGameState.play_time = gLoadedStateWord;
    {
        u32 *copy = (u32 *)gSceneState;
        copy[64] = gLoadedStateWord;
    }
    gGameState.saved_options = gOptionMirror;
    owner = UnitGetFar(gGameState.selected_actor);
    destination = summary->name;
    source = owner->name;
    for (i = 11; i >= 0; i--)
        *destination++ = *source++;
    summary->level = owner->level;
    summary->play_time = gGameState.play_time;
    summary->area = Scene_GetAreaNameIndexFar(gGameState.scene, gGameState.entrance);
    summary->class_id = owner->class_index;
    summary->coins = gGameState.coins;
    summary->djinn[0] = Party_SumDjinnCountsFar(0);
    summary->djinn[1] = Party_SumDjinnCountsFar(1);
    summary->djinn[2] = Party_SumDjinnCountsFar(2);
    summary->djinn[3] = Party_SumDjinnCountsFar(3);
    Party_ListActiveOwnersFar(owners);
    for (i = 0; i <= 3 && owners[i] != 255; i++)
        summary->party[i] = owners[i];
    summary->party[i] = -1;
    summary->palette_glow = gGameState.palette_glow[0];
    summary->palette_glow_sub = gGameState.palette_glow[1];
    summary->send_flag = gGameState.unknown_20f;
    summary->flag_count = 0;
    for (i = 48; i <= 127; i++) {
        if (GameFlag_TestFar(i))
            summary->flag_count++;
    }
    summary->has_flag_20 = GameFlag_TestFar(32) != 0;
    {
        s32 n;
        u32 *frame = &gGameState.build_stamp;
        u32 frames;

        n = 242;
        /* FAKEMATCH: the one-pass null arm and duplicate frame read end
         * CSE with the bound constant ahead of the frame load. Folding
         * the arm reverses two rows; the conditional and duplicate read
         * disappear, preserving all 174 instructions in every edition. */
        if (!frame) {
            do {
                n = 242;
            } while (0);
        } else {
            n = 242;
            frames = *frame;
        }
        frames = *frame;
        word = GameFlagBytes;
        i = 0;
        n <<= 2;
        summary->frames = frames;
        goto test;
        do {
            sum += *word++;
            i++;
test:;
        } while (i < n);
    }
    summary->checksum = sum;
    return sum;
}

s16 SaveState_WriteCurrentSlotPair(void)
{
    s16 value;
    s16 result;
    s32 found;
    s32 error;

    result = 0;
    value = gSaveSlot;
    if (value != -1) {
        found = SaveState_InitializeWorkspace();
        if (found != 0) {
            UiText_ShowPositionedMessageAndWait((s32)MsgNoBackupMemory, 1);
            error = 9;
            goto set_error;
        }
        SaveState_BuildSummaryHeader();
        {
            void *base = &gSaveBuffer;
            s32 next;

            found = SaveState_WriteRecord(gSaveSlot, base);
            next = gSaveSlot;
            base = (char *)base + 0x1000;
            found |= SaveState_WriteRecord(next + 3, base);
            if (found != 0) {
                UiText_ShowPositionedMessageAndWait((s32)MsgSaveFailed, 1);
                error = 3;
set_error:
                result = 0 - error;
            }
        }
        SaveState_ReleaseWorkspace();
        value = result;
    }
    return value;
}

s32 SaveState_WriteSlotPair(s32 arg0)
{
    s32 found;
    s16 result = 0;

    found = SaveState_InitializeWorkspace();
    if (found != 0) {
        UiText_ShowPositionedMessageAndWait((s32)MsgNoBackupMemory, 1);
        result = -9;
    } else {
        void *base = &gSaveBuffer;

        found = SaveState_WriteRecord(arg0, base);
        base = (char *)base + 0x1000;
        found |= SaveState_WriteRecord(arg0 + 3, base);
        if (found != 0) {
            UiText_ShowPositionedMessageAndWait((s32)MsgSaveFailed, 1);
            result = -3;
        }
    }
    SaveState_ReleaseWorkspace();
    return result;
}

s32 UiText_OpenMessageWindow(s32, s32, s32, s32);

s32 UiWork_IsComplete(void);
void UiWork_FinalizePendingCore(void);

s32 SaveState_ProcessSelectedSlot(void)
{
    void *buffer;
    s32 value;
    s32 result;
    s32 found;
    s32 error;

    buffer = Runtime_BumpAllocateAlternatePool(0x1000);
    result = 0;
    value = gSaveSlot;
    if (value != -1) {
        found = SaveState_InitializeWorkspace();
        if (found != 0) {
            error = 9;
            UiText_ShowPositionedMessageAndWait((s32)MsgNoBackupMemory, 1);
            /* FAKEMATCH: this eliminated self-assignment ends CSE before
             * the shared negate block. Without it, the unbound error uses
             * r6 for both constants and needs a copy to r7 (differ 6).
             * Scoping, a slot pointer and an inline return leave that
             * allocation; duplicating the negate tail still differs 4. */
            gSaveSlot += 0;
            goto negate;

        } else {
            u8 *dst;

            found = SaveState_ReadRecordPayload(gSaveSlot, buffer);
            if (found != 0) {
                UiText_ShowPositionedMessageAndWait((s32)MsgSaveFailed, 1);
                result = -2;
            }
            dst = (u8 *)buffer + (s32)gSaveStamp;
            dst -= (s32)gSaveBuffer;
            Iwram_CopyWords(dst, gSaveStamp, 16);
            found = SaveState_WriteRecord(gSaveSlot, buffer);
            if (found != 0) {
                UiText_ShowPositionedMessageAndWait((s32)MsgSaveFailed, 1);
                error = 3;
negate:
                result = -error;
            }
        }
        SaveState_ReleaseWorkspace();
        MemFree(buffer);
        value = result;
    }
    return value;
}

s32 SaveState_ConfirmAndProcessSelectedSlot(void)
{
    s32 result;

    UiText_OpenMessageWindow((s32)MsgOverwriteConfirm, 8, 12, 2);
    while (UiWork_IsComplete() == 0) {
        WaitFrames(1);
    }
    if (Menu_RunConfirmSelection(1, 0, 0, 1)) {
        UiWork_FinalizePendingCore();
    } else {
        UiWork_FinalizePendingCore();
        AudioCommand_PlayFar(85);
        result = SaveState_ProcessSelectedSlot();
        if (result >= 0) {
            UiText_ShowPositionedMessageAndWait((s32)MsgGameSaved, 1);
        }
    }
    return result;
}

s32 SaveState_CopySlotPair(void)
{
    u32 found;
    s32 value;
    s32 result;

    result = 0;
    found = SaveState_InitializeWorkspace();
    if (found != 0) {
        UiText_ShowPositionedMessageAndWait((s32)MsgNoBackupMemory, 1);
        result = -9;
    } else {
        SaveState_LoadSummaryRecords();
        value = SaveMenu_SelectSlot(0, 2);
        if (value == -1) {
            result = value;
        } else {
            void *lower = &gSaveBuffer;
            void *upper;

            found = SaveState_ReadRecordPayload(value, lower);
            upper = (u8 *)lower + 0x1000;
            found |= SaveState_ReadRecordPayload(value + 3, upper);
            if (found != 0) {
                UiText_ShowPositionedMessageAndWait((s32)MsgLoadFailed, 1);
                result = -2;
            } else {
                value = SaveState_FindFreeSummarySlot();
                if (value == 999) {
                    UiText_ShowPositionedMessageAndWait((s32)MsgEraseFailed, 1);
                    result = -5;
                } else {
                    found = SaveState_WriteRecord(value, lower);
                    found |= SaveState_WriteRecord(value + 3, upper);
                    if (found != 0) {
                        UiText_ShowPositionedMessageAndWait((s32)MsgEraseFailed, 1);
                        result = -3;
                    } else {
                        UiText_ShowPositionedMessageAndWait((s32)MsgFileCopied, 1);
                    }
                }
            }
        }
    }

    SaveState_ReleaseWorkspace();
    return result;
}

s32 SaveState_DeleteSelectedSlot(void)
{
    s32 found;
    s32 value;
    s32 result = 0;

    found = SaveState_InitializeWorkspace();
    if (found != 0) {
        UiText_ShowPositionedMessageAndWait((s32)MsgNoBackupMemory, 1);
        result = -9;
    } else {
        SaveState_LoadSummaryRecords();
        value = SaveMenu_SelectSlot(0, 3);
        if (value == -1) {
            result = value;
        } else {
            UiText_OpenMessageWindow((s32)MsgEraseConfirm, 8, 1, 2);
            while (UiWork_IsComplete() == 0) {
                WaitFrames(1);
            }
            if (Menu_RunConfirmSelection(1, 0, 3, 1) != 0) {
                UiWork_FinalizePendingCore();
            } else {
                UiWork_FinalizePendingCore();
                found = SaveState_DeleteRecord(value);
                found |= SaveState_DeleteRecord(value + 3);
                if (found != 0) {
                    UiText_ShowPositionedMessageAndWait((s32)MsgEraseFailed, 1);
                    result = -4;
                } else {
                    UiText_ShowPositionedMessageAndWait((s32)MsgFileErased, 1);
                }
            }
        }
    }
    SaveState_ReleaseWorkspace();
    return result;
}

#undef UiText_DrawPrefixedNumberAtOffset
