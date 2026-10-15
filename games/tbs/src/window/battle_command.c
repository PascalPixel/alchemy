#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/object/object.h"
#include "unit/window/glyph_arm.h"
#include "unit/runtime/runtime.h"
#include "types.h"
#include "edition.h"
#include "window.h"
#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "callback_scheduler.h"
#include "dma.h"
#include "tbs_edition.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "global_cells.h"
#include "menu_list.h"
#include "render_input.h"
#include "resource.h"
#include "output.h"
#include "glyph.h"
#include "game_state.h"
#include "battle_work.h"
#include "heap_state.h"
#include "text_font.h"
#include "text_render_runtime.h"
#include "fixed_math.h"
#include "runtime_interfaces.h"
#include "resource_ids.h"
#include "ramcode.h"
#include "affine.h"
#include "battle_party.h"
#include "scene.h"
#include "text_reader.h"
#include "lib.h"
#include "battle_unit.h"
#include "battle_system.h"
#include "item.h"
#include "vram_block.h"
#include "select.h"
#include "node_chain.h"
#include "text/msg_ids.h"
#include "sound_ids.h"
#include "layout_guard.h"
#include "field_event.h"
#include "b5_context.h"
#include "battle_type.h"
#include "uiflow.h"
#include "owner_state.h"
#include "workspace_options.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "save_state.h"
#include "irq.h"
#include "character_menu.h"
#include "animspr.h"
#include "four_object_motion.h"
#include "field_sprite.h"
#include "uiwindow.h"
#include "io_write_queue.h"
#include "map_scroll.h"
#include "vram_tab.h"
#include "djinn_preview.h"
#include "ui.h"
#include "motobj.h"
#include "effect_step.h"
#include "resource_menu.h"
#include "battle_summon.h"
#include "serial_runtime.h"
#include "battle_effect_work.h"
#include "shop.h"
#include "display_transition.h"
#include "frame.h"
#include "unit/object/psynergy_effects.h"
#include "unit/window/scales.h"

struct RenderOutput *RenderOutput_Create(
    s32 slot, s32 flags, struct RenderInput *input, s32 offsetX, s32 offsetY)
{
    s32                  x;
    struct RenderOutput *output;
    s32                  y;

    output = RenderOutput_AcquireFree();
    if (output == NULL)
    {
        Resource_ResetEntry(slot);
        return 0;
    }
    x = offsetX + (input->x * 8) + 8;
    y = offsetY + (input->y * 8) + 8;
    x &= 0x1ff;
    y &= 0xff;
    *(u32 *)&output->oam = (x << 16) | y | flags;
    ((u32 *)&output->oam)[1] = ((struct VramBlockCacheEntry *)Ram_VramBlockCache)[slot].offset >> 5;
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
    s32 mode, s32 resource, struct RenderInput *input, s32 x, s32 y)
{
    s32 no;

    no = Resource_LoadByMode(mode, resource);
    if (no < 0)
    {
        return NULL;
    }
    return RenderOutput_Create(no, 0x40000000, input, x, y);
}

struct RenderOutput *RenderOutput_CreateLoaded(
    s32 code, s32 layers, struct RenderInput *input, s32 x, s32 y)
{
    s32                  no;
    struct RenderOutput *result;

    no = Resource_FindFreeEntry();
    result = NULL;
    if (no != 0x60)
    {
        UiIcon_CopyResourceToSlot(code, layers, no);
        result = RenderOutput_Create(no, 0x40000000, input, x, y);
    }
    return result;
}

struct RenderOutput *RenderOutput_CreateWithTransform(
    s32 icon, struct RenderInput *input, s32 x, s32 y)
{
    s32                  count;
    s32                  unused;
    struct RenderOutput *result;

    count = Resource_FindFreeEntry();
    if (count == 0x60)
    {
        return NULL;
    }
    Ui_BuildPairedPatternsToSlot(icon, 1, &count, &unused, 1);
    result = RenderOutput_Create(count, 0x40000000, input, x, y);
    result->sentinel = 251;
    return result;
}

struct RenderOutput *RenderOutput_CreateFromTable(
    s32 table_entry, struct RenderInput *input, s32 x, s32 y)
{
    s32                  slot;
    struct RenderOutput *output;

    slot = Resource_FindFreeEntry();
    output = NULL;
    if (slot != 0x60)
    {
        RenderResource_LoadTableEntry(table_entry, 0, slot);
        output = RenderOutput_Create(slot, 0x40000000, input, x, y);
    }
    return output;
}

struct RenderOutput *CreateSideObject(
    s32 objectKind, s32 position, s32 side, s32 arg3, s32 arg4, s32 arg5)
{
    struct UiRenderWork *state = UI_WORK;
    struct RenderOutput *object = 0;
    s32                  first;
    s32                  second;
    s32                  id;
    s32                  slot;

    if (GameFlag_TestFar(32) != 0)
    {
        if (objectKind == 0)
            objectKind = 18;
        if (objectKind == 1)
            objectKind = 19;
    }

    id = Localization_LookupEntryId(objectKind);
    if (id == -1)
        return object;

    if ((u32)side > 1)
    {
        side = 1;
        if (state->resourceIds[1] != 999)
        {
            side = 0;
            if (state->resourceIds[0] != 999)
                return object;
        }
    }

    slot = 14 + side;
    UiGlyph_LoadEntryWithPalette(id, position, &first, &second, slot, 0);
    object = RenderOutput_Create(first, 0x80000000, (struct RenderInput *)arg3, arg4, arg5);
    if (object != 0)
    {
        object->oam.palette = slot;
        object->kind = 2;
    }

    state->resourceIds[side] = id;
    state->outputIds[side] = first;
    return object;
}

void Ui_LoadCharacterEntryForSlot(u32 slot, s32 character, s32 value)
{
    s32                  result;
    s32                  current;
    u32                  characterId;
    struct UiRenderWork *state;

    state = UI_WORK;

    if (GameFlag_TestFar(0x20) != 0)
    {
        if (character == 0)
            character = 0x12;
        if (character == 1)
            character = 0x13;
    }

    characterId = Localization_LookupEntryId(character);
    if (characterId != -1U)
    {
        if (slot > 1U)
        {
            if (state->resourceIds[1] == characterId)
            {
                slot = 1;
            }
            else if (state->resourceIds[0] == characterId)
            {
                slot = 0;
            }
            else
            {
                return;
            }
        }
        current = state->outputIds[slot];
        UiGlyph_LoadEntryWithPalette(characterId, value, &current, &result, slot + 0xe, 1);
    }
}

//--------------------------------------------------------------------------
// runtime_set_byte5_and_reset_halfword_c.c
// 表示：スプライト出力のアニメを切りかえる
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// active を mode にして、アニメの段（animStep）を 0 に戻す
// out : スプライト出力（NULL なら何もしない）
//--------------------------------------------------------------------------
void RenderOutput_SetAnimation(struct RenderOutput *out, s32 mode)
{
    if (out != NULL)
    {
        out->active = mode;
        out->animStep = 0;
    }
}

void RenderOutput_SetInvertedSentinel(void *arg0, s32 arg1)
{
    if (arg0 != 0)
        ((struct RenderOutput *)arg0)->sentinel = ~arg1;
}

typedef void (*RamRoutine)(void *dst, void *scene);

void UiWindow_FillFromScene(void *dst)
{
    void *scene = gWindowWork[0];

    if (scene == 0)
    {
        Dma_Fill16(0xe0e0, dst, 0x140);
    }
    else
    {
        RamRoutine code;

        RamCode_Load(code, Tile_BuildMetatiles, Tile_BuildMetatilesCodeSize);
        code(dst, scene);
        MemFree(code);
    }
}

void UiWindow_FillScreenBlockRect(s32 unused0, s32 unused1, u32 width, u32 height, s32 value)
{
    u32  row = 0;
    s16 *dst = (s16 *)0x06002000;

    if (row < height)
    {
        do
        {
            u32 column = 0;
            if (column < width)
            {
                do
                {
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

#define TILE_ROW_FROM_BOTTOM(tile, up) ((u32 *)((tile) * 32 - (up) * 4 + 0x0600001c))

void UiWindow_BuildLayoutBounds(s32 flags)
{
    void                    **slot = (void **)&gPartyStatusWork;
    struct PartyStatusWindow *state = *slot;
    struct UiRenderWork      *work = slot[-1];
    s32                       height = 4;
    s32                       n;
    s32                       right;
    s32                       left;

    if (work->menuState != 0)
    {
        n = BattleParty_PrepareActiveOwnersFar(0);
        height = 3;
    }
    else
    {
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
    s32                       zero;
    struct PartyStatusWindow *window;
    s8                       *busy;

    window = Runtime_AllocateBlock(16, sizeof(*window));
    busy = (s8 *)&UI_WORK->menuBusy;
    zero = 0;
    *busy = 1;
    UiWindow_BuildLayoutBounds(flags);
    window->window = UiWindow_Create(window->x, window->y, window->width, window->height, 6);
    PartyStatusDraw(flags);
    *busy = zero;
}

void UiWindow_DrawColumnBorders(struct RenderInput *window, u32 flags)
{
    struct UiRenderWork *canvas = UI_WORK;
    s32                  first = 1;
    s32                  bias = 0;
    u32                  max = window->width - 1;
    s32                  i;
    u32                  rows = window->height;
    u32                  col;
    u32                  row;
    u16                 *dest;

    if ((flags & 1) == 0)
        flags &= ~2;
    if (flags & 2)
    {
        bias = 5;
        first = 0;
    }
    i = first;
    while (gUiWindow_PartyColumnOffsets[i] >= 0)
    {
        col = gUiWindow_PartyColumnOffsets[i] + bias;
        if (col < max)
        {
            for (row = 0; row != rows; row++)
            {
                dest = (u16 *)(((window->y + row) * 32 + (window->x + col)) * 2
                               + (u32)canvas->tilemap);
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
    if (canvas->menuState)
    {
        dest = canvas->tilemap + (window->y + window->height - 1) * 32 + window->x;
        col = 1;
        *dest++ = 0xf080;
        while (col < max)
        {
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
    s32                  i;
    struct UiRenderWork *canvas = UI_WORK;
    s32                  filled = value;
    s32                  row;
    s32                  col;
    u32                  id;
    u32                  light;
    u32                  dark;
    u32                  pixels;
    u32                  result;

    if (canvas->menuState == 0)
    {
        Dma_Set((const void *)Runtime_GetLowTableAddress(), (void *)&BG_PLTT_COLOR(14, 0),
            0x80000010, REG_DMA3);
        BG_PLTT_COLOR(14, 14) = BG_PLTT_COLOR(15, 4);
    }
    x += window->x;
    y += window->y;
    for (i = 0; i < 5; i++, x++)
    {
        id = canvas->tilemap[y * 32 + x];
        light = 0x22222222;
        dark = 0xcccccccc;
        id &= 0x3ff;
        if (value > 7)
        {
            light = 0x88888888;
            dark = 0xdddddddd;
        }
        else if (value >= 0)
        {
            light <<= value * 4;
            light |= 0x88888888U >> (32 - value * 4);
            dark <<= value * 4;
            dark |= 0xddddddddU >> (32 - value * 4);
        }
        for (row = 0; row <= (filled != 0 ? 2 : 0); row++)
        {
            pixels = *TILE_ROW_FROM_BOTTOM(id, row);
            result = 0;
            for (col = 0; col <= 7; col++)
            {
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

void PartyStatusDraw(s32 flags)
{
    void                    **slot = (void **)gPartyStatusWork;
    struct PartyStatusWindow *layout = slot[0];
    struct BattleWork        *party = slot[-7];
    struct RenderInput       *window = (struct RenderInput *)layout->window;
    struct UiRenderWork      *work = slot[-1];
    s32                       x = 0;
    u32                       count;
    s32                       y = 0;
    u32                       i;
    u16                       owners[5];
    u8                        djinn[4];

    if (work->menuState != 0)
    {
        count = BattleParty_PrepareActiveOwnersFar(0);
        y = -1;
        for (i = 0; i < count; i++)
        {
            owners[i] = party->partyUnits[i];
            if (owners[i] == 255)
                break;
        }
    }
    else
    {
        count = Party_CountActiveOwnersFar();
        for (i = 0; i < count; i++)
            owners[i] = gGameState.activeOwners[i];
        owners[i] = 255;
    }
    count = i;
    if (flags == -1)
        flags = layout->flags;
    if (!(flags & 1))
        flags &= ~2;
    if (work->menuState == 0 || BattlePlacement_CountValidEntriesFar(0, 0) == 0)
        flags &= ~2;
    if (flags == 9)
    {
        UiWindow_EraseBorderRect(layout->x, layout->y, layout->width, layout->height);
        return;
    }
    work->menuBusy = 1;
    if (layout->flags == flags)
    {
        RenderOutput_RedrawSavedRect((struct UiWindow *)window);
        UiWindow_DrawColumnBorders(window, flags);
    }
    else
    {
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
    for (i = 0; i != count; i++)
    {
        struct BattleUnit *unit = UnitGetFar(owners[i]);
        s32                hp = unit->hp;
        s32                maximum = unit->maxHp;
        s32                value;
        s32                current;

        if (hp == 0)
            UiWork_SetParamNibble(2);
        else if (hp <= maximum / 4)
            UiWork_SetParamNibble(4);
        else
            UiWork_SetParamNibble(15);
        work->level = 14;
        if (work->menuState != 0)
            work->level = 5;
        UiText_DrawPrefixedNumberAtOffset(hp, window, (x + i * 6) * 8, y * 8 + 8, 0);
        work->level = 15;
        UiText_DrawStringAtOffset(unit->name, window, (x + i * 6) * 8, y * 8);
        UiWork_SetParamNibble(15);
        if (unit->maxHp != 0)
        {
            current = unit->hp;
            value = ((current * 40) / unit->maxHp);
            if (value == 0 && current != 0)
                value = 1;
            UiWindow_DrawStatusBarTiles(window, x + i * 6 + 1, y + 2, value);
        }
        if (flags & 1)
        {
            work->level = 14;
            if (work->menuState != 0)
                work->level = 5;
            UiText_DrawPrefixedNumberAtOffset(unit->pp, window, (x + i * 6) * 8, y * 8 + 16, 1);
            if (unit->maxPp != 0)
            {
                current = unit->pp;
                value = ((current * 40) / unit->maxPp);
                if (value == 0 && current != 0)
                    value = 1;
                UiWindow_DrawStatusBarTiles(window, x + i * 6 + 1, y + 3, value);
            }
        }
    }
    work->level = 15;
    if (work->menuState != 0 && (flags & 2))
    {
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
    work->menuBusy = 0;
}

void UiWork_FinalizeAndReleaseBlock16(void)
{
    UiWork_Finalize((struct UiWindow *)**(s32 **)(u32)&gWindowWork[1], 1);
    Runtime_ReleaseHeapBlock(0x10);
}

void UiWindow_SetTileAttributeBitRect(
    const struct RenderInput *window, s32 x, s32 y, s32 width, s32 height, u32 field)
{
    struct UiRenderWork *work = UI_WORK;

    x += window->x + 1;
    y += window->y + 1;
    field &= 1;
    field <<= 12;
    if (x < 0)
    {
        width += x;
        x = 0;
    }
    if (x + width > 29)
    {
        width = 30 - x;
    }
    if (y < 0)
    {
        height += y;
        y = 0;
    }
    if (y + height > 29)
    {
        height = 20 - y;
    }
    if (width > 0 && height > 0)
    {
        y <<= 6;
        x = y + (x << 1);
        do
        {
            u16 *cell = (u16 *)(x + (u32)work->tilemap);
            s32  remaining = width;
            while (remaining != 0)
            {
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

extern const u8 *gRenderResource_PairSourceTable[];

void *Text_FormatPlayTime(s32 value, u8 *out)
{
    u8  buf[64];
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
    u32                 i;
    struct SaveSummary *p;

    p = gSaveWorkspace->summary;
    for (i = 0; i < 3; i++, p++)
    {
        if (p->level == 0)
            return i;
    }
    return 0x3E7;
}

s32 SaveState_CountRecordsExcludingFlagged(s32 flag)
{
    s32                 i;
    s32                 cnt;
    struct SaveSummary *summary;

    if (SaveState_InitializeWorkspace() != 0)
    {
        cnt = -9;
    }
    else
    {
        cnt = SaveState_LoadSummaryRecords();
        if (flag != 0)
        {
            summary = gSaveWorkspace->summary;
            for (i = 0; i < 3; i++)
            {
                if ((s8)summary[i].sendFlag != 0)
                    cnt--;
            }
        }
    }
    SaveState_ReleaseWorkspace();
    return cnt;
}

s32 SaveState_ScanRecordFlags(void)
{
    /* FAKEMATCH: the signed cursor from the party tail keeps the native
     * literal base 0x1070 and the flag offsets 1 and 2. Measured: the
     * fields read as summary[i].sendFlag and .flagCount, or through a
     * walking summary pointer, base the cursor on sendFlag (0x1071)
     * with offsets 0 and 1 (differ 4). */
    s32 err;
    s32 cnt;
    s32 ret;

    err = SaveState_InitializeWorkspace();
    cnt = 0;
    ret = -9;
    if (err == 0)
    {
        s32 i;
        s8 *flags;

        ret = SaveState_LoadSummaryRecords();
        flags = (s8 *)&gSaveWorkspace->summary[0].party[4];
        gTitleSendOptionEnabled = 0;
        (*(s16 *)&gTitleExtraOptionEnabled) = 0;
        for (i = 0; i < 3; i++)
        {
            if (flags[i * sizeof(struct SaveSummary)
                      + ((u32) & ((struct SaveSummary *)0)->sendFlag - (u32)
                          & ((struct SaveSummary *)0)->party[4])]
                != 0)
            {
                gTitleSendOptionEnabled = 1;
                cnt++;
            }
            if (flags[i * sizeof(struct SaveSummary)
                      + ((u32) & ((struct SaveSummary *)0)->flagCount - (u32)
                          & ((struct SaveSummary *)0)->party[4])]
                != 0)
            {
                (*(s16 *)&gTitleExtraOptionEnabled) = 1;
            }
        }

        if ((gKeysHeld & (KEY_R | KEY_LEFT)) != (KEY_R | KEY_LEFT))
        {
            gTitleSendOptionEnabled = 0;
        }
    }
    SaveState_ReleaseWorkspace();
    if (ret != 0 && cnt == ret)
    {
        return ret + 100;
    }
    return ret;
}

u32 SaveState_BuildSummaryHeader(void)
{
    u16                 owners[14];
    struct SaveSummary *summary;
    struct BattleUnit  *owner;
    u8                 *source;
    u8                 *destination;
    u32                *word;
    s32                 i;
    u32                 sum = 0;

    summary = (struct SaveSummary *)(gSaveBuffer - 16);
    gGameState.buildStamp = Runtime_GetBuildStampTimeFar(0);
    gGameState.playTime = gLoadedStateWord;
    {
        u32 *copy = (u32 *)gSceneState;
        copy[64] = gLoadedStateWord;
    }
    gGameState.savedOptions = gOptionMirror;
    owner = UnitGetFar(gGameState.selectedActor);
    destination = summary->name;
    source = owner->name;
    for (i = 11; i >= 0; i--)
        *destination++ = *source++;
    summary->level = owner->level;
    summary->playTime = gGameState.playTime;
    summary->area = Scene_GetAreaNameIndexFar(gGameState.scene, gGameState.entrance);
    summary->classId = owner->classIndex;
    summary->coins = gGameState.coins;
    summary->djinn[0] = Party_SumDjinnCountsFar(0);
    summary->djinn[1] = Party_SumDjinnCountsFar(1);
    summary->djinn[2] = Party_SumDjinnCountsFar(2);
    summary->djinn[3] = Party_SumDjinnCountsFar(3);
    Party_ListActiveOwnersFar(owners);
    for (i = 0; i <= 3 && owners[i] != 255; i++)
        summary->party[i] = owners[i];
    summary->party[i] = -1;
    summary->paletteGlow = gGameState.paletteGlow[0];
    summary->paletteGlowSub = gGameState.paletteGlow[1];
    summary->sendFlag = gGameState.unk20f;
    summary->flagCount = 0;
    for (i = 48; i <= 127; i++)
    {
        if (GameFlag_TestFar(i))
            summary->flagCount++;
    }
    summary->hasFlag20 = GameFlag_TestFar(32) != 0;
    {
        s32  n;
        u32 *frame = &gGameState.buildStamp;
        u32  frames;

        n = 242;
        /* FAKEMATCH: the one-pass null arm and duplicate frame read end
         * CSE with the bound constant ahead of the frame load. Folding
         * the arm reverses two rows; the conditional and duplicate read
         * disappear, preserving all 174 instructions in every edition. */
        if (!frame)
        {
            do
            {
                n = 242;
            } while (0);
        }
        else
        {
            n = 242;
            frames = *frame;
        }
        frames = *frame;
        word = (u32 *)gGameFlagBytes;
        i = 0;
        n <<= 2;
        summary->frames = frames;
        goto test;
        do
        {
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

    result = 0;
    value = gSaveSlot;
    if (value != -1)
    {
        found = SaveState_InitializeWorkspace();
        if (found != 0)
        {
            UiText_ShowPositionedMessageAndWait(MsgNoBackupMemory, 1);
            result = -9;
        }
        else
        {
            SaveState_BuildSummaryHeader();
            {
                void *base = &gSaveBuffer;
                s32   next;

                found = SaveState_WriteRecord(gSaveSlot, base);
                next = gSaveSlot;
                base = (char *)base + 0x1000;
                found |= SaveState_WriteRecord(next + 3, base);
                if (found != 0)
                {
                    UiText_ShowPositionedMessageAndWait(MsgSaveFailed, 1);
                    result = -3;
                }
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
    if (found != 0)
    {
        UiText_ShowPositionedMessageAndWait(MsgNoBackupMemory, 1);
        result = -9;
    }
    else
    {
        void *base = &gSaveBuffer;

        found = SaveState_WriteRecord(arg0, base);
        base = (char *)base + 0x1000;
        found |= SaveState_WriteRecord(arg0 + 3, base);
        if (found != 0)
        {
            UiText_ShowPositionedMessageAndWait(MsgSaveFailed, 1);
            result = -3;
        }
    }
    SaveState_ReleaseWorkspace();
    return result;
}

s32 SaveState_ProcessSelectedSlot(void)
{
    void *buffer;
    s32   result;
    s32   found;
    u8   *dst;

    buffer = Runtime_BumpAllocateAlternatePool(0x1000);
    result = 0;
    if (gSaveSlot == -1)
        return -1;
    found = SaveState_InitializeWorkspace();
    if (found != 0)
    {
        UiText_ShowPositionedMessageAndWait(MsgNoBackupMemory, 1);
        result = -9;
    }
    else
    {
        found = SaveState_ReadRecordPayload(gSaveSlot, buffer);
        if (found != 0)
        {
            UiText_ShowPositionedMessageAndWait(MsgSaveFailed, 1);
            result = -2;
        }
        dst = (u8 *)buffer + (s32)gSaveStamp;
        dst -= (s32)gSaveBuffer;
        Iwram_CopyWords(dst, gSaveStamp, 16);
        found = SaveState_WriteRecord(gSaveSlot, buffer);
        if (found != 0)
        {
            UiText_ShowPositionedMessageAndWait(MsgSaveFailed, 1);
            result = -3;
        }
    }
    SaveState_ReleaseWorkspace();
    MemFree(buffer);
    return result;
}

s32 SaveState_ConfirmAndProcessSelectedSlot(void)
{
    s32 result;

    UiText_OpenMessageWindow(MsgOverwriteConfirm, 8, 12, 2);
    while (UiWork_IsComplete() == 0)
    {
        WaitFrames(1);
    }
    if (Menu_RunConfirmSelection(1, 0, 0, 1))
    {
        UiWork_FinalizePendingCore();
    }
    else
    {
        UiWork_FinalizePendingCore();
        AudioCommand_PlayFar(85);
        result = SaveState_ProcessSelectedSlot();
        if (result >= 0)
        {
            UiText_ShowPositionedMessageAndWait(MsgGameSaved, 1);
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
    if (found != 0)
    {
        UiText_ShowPositionedMessageAndWait(MsgNoBackupMemory, 1);
        result = -9;
    }
    else
    {
        SaveState_LoadSummaryRecords();
        value = SaveMenu_SelectSlot(0, 2);
        if (value == -1)
        {
            result = value;
        }
        else
        {
            void *lower = &gSaveBuffer;
            void *upper;

            found = SaveState_ReadRecordPayload(value, lower);
            upper = (u8 *)lower + 0x1000;
            found |= SaveState_ReadRecordPayload(value + 3, upper);
            if (found != 0)
            {
                UiText_ShowPositionedMessageAndWait(MsgLoadFailed, 1);
                result = -2;
            }
            else
            {
                value = SaveState_FindFreeSummarySlot();
                if (value == 999)
                {
                    UiText_ShowPositionedMessageAndWait(MsgEraseFailed, 1);
                    result = -5;
                }
                else
                {
                    found = SaveState_WriteRecord(value, lower);
                    found |= SaveState_WriteRecord(value + 3, upper);
                    if (found != 0)
                    {
                        UiText_ShowPositionedMessageAndWait(MsgEraseFailed, 1);
                        result = -3;
                    }
                    else
                    {
                        UiText_ShowPositionedMessageAndWait(MsgFileCopied, 1);
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
    if (found != 0)
    {
        UiText_ShowPositionedMessageAndWait(MsgNoBackupMemory, 1);
        result = -9;
    }
    else
    {
        SaveState_LoadSummaryRecords();
        value = SaveMenu_SelectSlot(0, 3);
        if (value == -1)
        {
            result = value;
        }
        else
        {
            UiText_OpenMessageWindow(MsgEraseConfirm, 8, 1, 2);
            while (UiWork_IsComplete() == 0)
            {
                WaitFrames(1);
            }
            if (Menu_RunConfirmSelection(1, 0, 3, 1) != 0)
            {
                UiWork_FinalizePendingCore();
            }
            else
            {
                UiWork_FinalizePendingCore();
                found = SaveState_DeleteRecord(value);
                found |= SaveState_DeleteRecord(value + 3);
                if (found != 0)
                {
                    UiText_ShowPositionedMessageAndWait(MsgEraseFailed, 1);
                    result = -4;
                }
                else
                {
                    UiText_ShowPositionedMessageAndWait(MsgFileErased, 1);
                }
            }
        }
    }
    SaveState_ReleaseWorkspace();
    return result;
}

//--------------------------------------------------------------------------
// glow_update_sine.c
// パレット：BG パレット 14 の色 8〜11 を sin で明滅させる
//--------------------------------------------------------------------------

#if EDITION_INTERNATIONAL || defined(TLA_EDITION_JA)

//--------------------------------------------------------------------------
// スケジューラから呼ばれ、4 色を位相をずらした sin で明滅させる
// s = sin / 16384 として RGB555 で 赤 22 + 2s・緑 16 + s・青 20 + s
//--------------------------------------------------------------------------
void PaletteGlow_UpdateSine(void)
{
    s32          *tick;
    volatile u16 *pal;
    s32           col;
    s32           s;
    s32           i;

    tick = (s32 *)&gFrameTick;
    pal = (volatile u16 *)0x050001D0; // BG パレット 14 の色 8
    i = 0;
    do
    {
        s = Trig_Sin((*tick + i * 8) * 0x300); // 1 色ごとに 8 フレームずらす
        col = s / 16384;
        {
            u32 r = col << 1;
            u32 g = col;

            r += 22;
            g += 16;
            col += 20;
            /* FAKEMATCH: the one-pass block keeps the blue shift after the three
             * adds, in source order. Measured: without it the scheduler moves the
             * red add below the shifts (differ 6); the colour as one expression
             * differs 25, and r, g, b locals in place of col differ 6, as a do
             * or a for loop. */
            do
            {
                col <<= 10;
            } while (0);
            g <<= 5;
            col |= g;
            col |= r;
        }
        /* FAKEMATCH: the one-pass block keeps the store ahead of the loop
         * index's increment. Measured: without it, differ 2. */
        do
        {
            *pal++ = col;
        } while (0);
    } while (++i <= 3);
}

#endif

#if EDITION_INTERNATIONAL

void Scheduler_ScheduleCallbackAAfterFrames(void)
{
    Scheduler_AddOrUpdateCallback(PaletteGlow_UpdateSine, 0xC80);
}

void Scheduler_ScheduleCallbackA(void)
{
    Scheduler_RemoveCallback(PaletteGlow_UpdateSine);
}

void UiWindow_FillTilemapRect(struct UiWindow *window, s32 x, s32 y, s32 width, s32 height)
{
    u16 *map = (u16 *)gWindowWork[0];
    s32  sum;

    sum = x + window->x;
    x = sum + 1;
    sum = y + ((struct UiWindow *)window)->y;
    y = sum + 1;

    if (x < 0)
    {
        width += x;
        x = 0;
    }
    if (x + width > 29)
        width = 30 - x;

    if (y < 0)
    {
        height += y;
        y = 0;
    }
    if (y + height > 29)
        height = 20 - y;

    if (width > 0 && height > 0)
    {
        s32 yOffset = y << 6;
        s32 xOffset = x << 1;
        s32 offset = yOffset + xOffset;
        do
        {
            s32  remaining = width;
            u16 *cell = (u16 *)(offset + (s32)map);
            if (remaining != 0)
            {
                do
                {
                    remaining--;
                    *cell++ = 0xE006;
                } while (remaining != 0);
            }
            height--;
            offset += 64;
        } while (height != 0);
        ((struct UiRenderWork *)map)->dirty = 1;
    }
}

#endif

void ObjectPlacement_CreateGroup(
    struct RenderInput *origin, s32 x, s32 y, struct SaveSummary *descriptor)
{
    struct ShopRuntime *state = gMenuWork;
    s32                 i;
    s32                 duration;

    for (i = 0; i < 4 && descriptor->party[i] != -1; i++)
    {
        struct AnimationObject *object = ResourceObject_CreateFar(
            Party_RemapCharacterIdIfEnabledFar(descriptor->party[i], (s8)descriptor->hasFlag20));

        if (object != 0)
        {
            AnimationObjects_SelectAnimationFar(object, 1);
            ((struct FieldSprite *)object)->flags = 0;
            ((struct FieldSprite *)object)->priority = 0;
        }

        state->partyMemberIcons[i] = object;
        state->partyMemberX[i] = (x + origin->x + i * 3) * 8 + 0x10;
        state->partyMemberY[i] = (y + origin->y) * 8 + 0x10;
        state->partyMemberScale[i] = 0x10000;
    }

    duration = 200;
    duration <<= 4;
    Scheduler_AddOrUpdateCallback(Menu_UpdateFirstObjectRowPositions, duration);
}

void Menu_ClearFirstObjectRowAndScheduleUpdate(void)
{
    struct ShopRuntime *state = gMenuWork;
    s32                 i;

    Scheduler_RemoveCallback(Menu_UpdateFirstObjectRowPositions);
    for (i = 0; i < 4; i++)
    {
        if (state->partyMemberIcons[i] != 0)
        {
            ResourceObject_ReleaseFar(state->partyMemberIcons[i]);
            state->partyMemberIcons[i] = 0;
        }
    }
}

void Menu_UpdateFirstObjectRowPositions(void)
{
    struct ShopRuntime *state = gMenuWork;
    s32                 source[2];
    s32                 request[4];
    s32                 index = 0;
    s32                 handle;

    while (1)
    {
        handle = (s32)state->partyMemberIcons[index];
        if (handle != 0)
        {
            source[0] = state->partyMemberScale[index];
            source[1] = state->partyMemberScale[index];
            request[0] = state->partyMemberX[index] << 16;
            request[1] = 0x01F40000;
            request[2] = (state->partyMemberY[index] << 16) + 0x01F40000;
            request[3] = 0;
            Object_ApplyProjectedPlacementFar(handle, request, source, 0x4000);
        }
        index += 1;
        if (index > 3)
        {
            break;
        }
    }
}

void Menu_SpawnFourObjectsAtOrigin(struct RenderInput *origin, s32 x, s32 y)
{
    struct FourObjectMotionState *state = gMenuWork;
    s32                           i;

    if (origin != 0)
    {
        for (i = 0; i < 4; i++)
        {
            struct AnimationObject *object =
                (struct AnimationObject *)ResourceObject_CreateFar(gMenu_PartySpriteResourceIds[i]);

            if (object != 0)
            {
                AnimationObjects_SelectAnimationFar(object, 2);
                ((struct FieldSprite *)object)->flags = 0;
                ((struct FieldSprite *)object)->priority = 0;
            }

            state->objects[i] = object;
            state->positionsX[i] = (origin->x + x + i * 3) * 8 + 0x10;
            state->positionsY[i] = (origin->y + y) * 8 + 0x10;
        }

        Scheduler_AddOrUpdateCallback(Menu_UpdateSecondObjectRowPositions, 200 << 4);
    }
}

void Menu_ClearSecondObjectRowAndScheduleUpdate(void)
{
    struct FourObjectMotionState *state = gMenuWork;
    s32                           i;

    Scheduler_RemoveCallback(Menu_UpdateSecondObjectRowPositions);
    for (i = 0; i < 4; i++)
    {
        if (state->objects[i] != 0)
        {
            ResourceObject_ReleaseFar(state->objects[i]);
            state->objects[i] = 0;
        }
    }
}

void Menu_UpdateSecondObjectRowPositions(void)
{
    struct FourObjectMotionState *root = gMenuWork;
    s32                           scale[2];
    s32                           position[4];
    s32                           index;

    index = 0;
    do
    {
        void *object = root->objects[index];

        if (object != 0)
        {
            s32 unit = 0x10000;
            s32 bias = 0x1f40000;

            scale[0] = unit;
            scale[1] = unit;
            position[0] = root->positionsX[index] << 16;
            position[1] = bias;
            position[2] = (root->positionsY[index] << 16) + bias;
            position[3] = 0;
            Object_ApplyProjectedPlacementFar((s32)object, position, scale, 0x4000);
        }
        index++;
    } while (index <= 3);
}

//--------------------------------------------------------------------------
// draw_four_numbers_in_row.c
// セーブ：記録のジンの数を属性ごとに横に並べて書く
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 4 つの数を 2 けたで、24 ドットおきに y 16 に書く
// win : 書き込み先（0 なら何もしない）
//--------------------------------------------------------------------------
void UiText_DrawFourNumbersInRow(struct RenderInput *win, struct SaveSummary *row)
{
    s32 i;

    if (win != 0)
    {
        for (i = 0; i < 4; i++)
            UiText_DrawNumberAtOffset((s8)row->djinn[i], 2, win, i * 24, 16);
    }
}

//--------------------------------------------------------------------------
// draw_character_summary.c
// セーブ：記録の名前・レベル・クラス・コイン・プレイ時間を書く
//--------------------------------------------------------------------------
// 版ごとの「Lv」と「コイン」の x（ドット）
#if defined(TBS_EDITION_ES) || defined(TLA_EDITION_ES)
#define SUMMARY_LEVEL_X 64
#define SUMMARY_COINS_X 56
#elif defined(TBS_EDITION_FR) || defined(TLA_EDITION_FR)
#define SUMMARY_LEVEL_X 59
#define SUMMARY_COINS_X 56
#else
#define SUMMARY_COINS_X 48
#endif

//--------------------------------------------------------------------------
// win : 書き込み先（0 なら何もしない）
// rec : セーブの記録
//--------------------------------------------------------------------------
void StatusMenu_DrawCharacterSummary(s32 win, struct SaveSummary *rec)
{
    u32 buf[4]; // プレイ時間の文字列
    s32 y;

    if (win != 0)
    {
        RenderOutput_PrepareForRedraw((struct UiWindow *)win);
        UiWindow_DrawDividerLine(win, 0, 4, 13, 4);
        UiText_DrawStringAtOffset(rec->name, win, 0, 0);
        y = 0;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TLA_EDITION_ES) \
    || defined(TLA_EDITION_FR)
        UiText_DrawStringAtOffset(gStatusMenu_LevelLetterString, win, SUMMARY_LEVEL_X, 0);
#else
        UiText_DrawStringInWindow((s32)gStatusMenu_LevelLetterString, win, 72, 0);
#endif
        UiText_DrawNumberAtOffset(rec->level, 2, win, 80, y);
        UiText_DrawCharacterAtOffset(rec->classId + MsgClassName, win, 0, 16);
#if !EDITION_INTERNATIONAL
        y = 32;
        UiText_DrawNumberInWindow(rec->coins, 6, win, 24, y);
        UiText_DrawCharacterAtOffset(MsgCoins, win, 72, 32);
        UiText_DrawCharacterAtOffset(MsgStatusLabel, win, 0, 48);
        UiText_DrawStringInWindow((s32)Text_FormatPlayTime(rec->playTime, (u8 *)buf), win, 48, 48);
#else
        UiText_DrawCharacterAtOffset(MsgStatusLabel, win, 0, 32);
        UiText_DrawStringInWindow((s32)Text_FormatPlayTime(rec->playTime, (u8 *)buf), win, 48, 40);
        y = 48;
        UiText_DrawNumberInWindow(rec->coins, 6, win, 0, y);
#if defined(TBS_EDITION_FR) || defined(TLA_EDITION_FR)
        // フランス語版は 1 枚以下なら単数形
        if (rec->coins > 1)
            UiText_DrawCharacterAtOffset(MsgCoins, win, SUMMARY_COINS_X, 48);
        else
            UiText_DrawCharacterAtOffset(MsgPiece, win, SUMMARY_COINS_X, 48);
#else
        UiText_DrawCharacterAtOffset(MsgCoins, win, SUMMARY_COINS_X, 48);
#endif
#endif
    }
}

#if EDITION_INTERNATIONAL

s32 SaveMenu_SelectSlot(s32 slot, s32 mode)
{
    struct UiWindow      *list;
    struct SaveWorkspace *save;
    struct UiWindow      *status;
    struct UiWindow      *djinn;
    struct UiWindow      *counts;
    struct SaveSummary   *summary;
    struct RenderOutput  *cursor;
    s32                   redraw;
    struct UiRenderWork  *render;
    u32                   stamp;
    void                 *work;
    s32                   result;
    s32                   i;
    s32                   y;

    work = Runtime_AllocateHeapBlock(55, 0xa70);
    save = *(gWorkSlot + HEAP_SLOT_SAVE);
    status = NULL;
    djinn = NULL;
    counts = NULL;
    redraw = 1;
    render = *(gWorkSlot + HEAP_SLOT_WINDOW);
    stamp = Runtime_GetBuildStampTimeFar();
    if (slot < 0)
        slot = 0;
    if (mode == 1)
    {
        for (i = 0;
            i < 3 && (save->summary[slot].level == 0 || (s8)save->summary[slot].sendFlag != 0); i++)
        {
            slot++;
            if (slot == 3)
                slot = 0;
        }
        if (i == 3)
        {
            result = -2;
            goto done;
        }
    }
    else if (mode == 4)
    {
        for (i = 0;
            i < 3 && (save->summary[slot].level == 0 || (s8)save->summary[slot].flagCount == 0);
            i++)
        {
            slot++;
            if (slot == 3)
                slot = 0;
        }
        if (i == 3)
        {
            result = -2;
            goto done;
        }
    }
    else if (mode == 5)
    {
        for (i = 0;
            i < 3 && (save->summary[slot].level == 0 || (s8)save->summary[slot].sendFlag == 0); i++)
        {
            slot++;
            if (slot == 3)
                slot = 0;
        }
        if (i == 3)
        {
            result = -2;
            goto done;
        }
    }
    else if (mode != 0)
    {
        for (i = 0; i < 3 && (save->summary[slot].level == 0); i++)
        {
            slot++;
            if (slot == 3)
                slot = 0;
        }
        if (i == 3)
        {
            result = -2;
            goto done;
        }
    }

    Dma_Fill32(0, work, 0xa70);
    Scheduler_ScheduleCallbackAAfterFrames();
    list = UiWindow_Create(1, 2, 28, 7, 2);
    summary = save->summary;
    y = 0;
    for (i = 0; i < 3; i++, y += 16, summary++)
    {
        if (summary->level == 0)
        {
            UiText_DrawResource(MsgSaveNoData, list, 10, y);
        }
        else if (summary->frames < stamp)
        {
            UiText_DrawResource(MsgSaveContinue, list, 10, y);
        }
        else if (summary->playTime != *(u32 *)summary->unk38)
        {
            UiText_DrawResource(MsgSaveCorrupted, list, 10, y);
        }
        else if (mode == 5 && (s8)summary->sendFlag == 0)
        {
            UiText_DrawResource(MsgSaveNotCleared, list, 10, y);
        }
        else
        {
            UiText_DrawString(summary->name, list, SAVE_SLOT_NAME_X, y);
            UiText_DrawResource(summary->area + MsgDebugEntryName, list, 62, y);
            render->dirty = 1;
        }
    }
    UiWindow_DrawDividerLine(list, 0, 2, 27, 2);
    UiWindow_DrawDividerLine(list, 0, 4, 27, 4);
    cursor = RenderResource_CreatePair(mode, list, 72, -24);

    while (1)
    {
        if (redraw != 0)
        {
            redraw = 0;
            if (save->summary[slot].level != 0)
            {
                PaletteGlow_Update(
                    save->summary[slot].paletteGlow, save->summary[slot].paletteGlowSub);
                if (status == NULL)
                    status = UiWindow_Create(1, 10, 14, 9, 2);
                StatusMenu_DrawCharacterSummary(status, &save->summary[slot]);
                WaitFrames(1);
                if (djinn == NULL)
                    djinn = UiWindow_Create(16, 10, 13, 3, 2);
                Menu_ClearFirstObjectRowAndScheduleUpdate();
                ObjectPlacement_CreateGroup(djinn, 0, 0, &save->summary[slot]);
                WaitFrames(1);
                if ((s8)save->summary[slot].djinn[0] + (s8)save->summary[slot].djinn[1]
                        + (s8)save->summary[slot].djinn[2] + (s8)save->summary[slot].djinn[3]
                    != 0)
                {
                    if (counts == NULL)
                        counts = UiWindow_Create(16, 14, 13, 5, 2);
                    UiText_DrawFourNumbersInRow(counts, &save->summary[slot]);
                    Menu_ClearSecondObjectRowAndScheduleUpdate();
                    Menu_SpawnFourObjectsAtOrigin(counts, 0, 0);
                }
                else
                {
                    Menu_ClearSecondObjectRowAndScheduleUpdate();
                    UiWork_Finalize(counts, 2);
                    counts = NULL;
                }
            }
            else
            {
                PaletteGlow_Update(gGameState.paletteGlow[0], gGameState.paletteGlow[1]);
                Menu_ClearSecondObjectRowAndScheduleUpdate();
                Menu_ClearFirstObjectRowAndScheduleUpdate();
                UiWork_Finalize(counts, 2);
                UiWork_Finalize(djinn, 2);
                UiWork_Finalize(status, 2);
                counts = NULL;
                djinn = NULL;
                status = NULL;
            }
            RenderOutput_RedrawSavedRect(list);
            UiWindow_DrawDividerLine(list, 0, 2, 27, 2);
            UiWindow_DrawDividerLine(list, 0, 4, 27, 4);
            UiWindow_FillTilemapRect(list, 0, slot * 2, 26, 1);
        }
        Ui_ApplyTableOffsetToPair(cursor);
        WaitFrames(1);
        if (gKeysRepeat & 0x40)
        {
            AudioCommand_PlayFar(111);
            redraw = 1;
            do
            {
                slot = (slot + 2) % 3;
                if (mode == 0)
                    break;
            } while ((save->summary[slot].level == 0
                      || (mode == 1 && (s8)save->summary[slot].sendFlag != 0)
                      || (mode == 4 && (s8)save->summary[slot].flagCount == 0)
                      || (mode == 5 && (s8)save->summary[slot].sendFlag == 0)));
        }
        else if (gKeysRepeat & 0x80)
        {
            AudioCommand_PlayFar(111);
            redraw = 1;
            do
            {
                slot = (slot + 4) % 3;
                if (mode == 0)
                    break;
            } while ((save->summary[slot].level == 0
                      || (mode == 1 && (s8)save->summary[slot].sendFlag != 0)
                      || (mode == 4 && (s8)save->summary[slot].flagCount == 0)
                      || (mode == 5 && (s8)save->summary[slot].sendFlag == 0)));
        }
        else if (gKeyState & 2)
        {
            AudioCommand_PlayFar(113);
            result = -1;
            break;
        }
        else if (gKeyState & 1)
        {
            AudioCommand_PlayFar(112);
            result = slot;
            break;
        }
    }
    Menu_ClearSecondObjectRowAndScheduleUpdate();
    Menu_ClearFirstObjectRowAndScheduleUpdate();
    UiWork_Finalize(counts, 2);
    UiWork_Finalize(djinn, 2);
    UiWork_Finalize(status, 2);
    UiWork_Finalize(list, 2);
    Scheduler_ScheduleCallbackA();
    Runtime_ReleaseHeapBlock(55);
    PaletteGlow_Update(gGameState.paletteGlow[0], gGameState.paletteGlow[1]);
    WaitFrames(1);
done:
    return result;
}
#elif defined(TBS_EDITION_JA)

s32 SaveMenu_SelectSlot(s32 slot, s32 mode)
{
    struct UiWindow      *list;
    struct UiWindow      *status;
    struct SaveWorkspace *save;
    struct UiWindow      *djinn;
    struct UiWindow      *counts;
    struct SaveSummary   *summary;
    struct RenderOutput  *cursor;
    s32                   kind;
    s32                   redraw;
    struct UiRenderWork  *render;
    u32                   stamp;
    void                 *work;
    struct ShopCursor     marker;
    s32                   result;
    s32                   i;
    s32                   x;
    s32                   y;
    s32                   tile;
    struct RenderOutput  *anchor;

    work = Runtime_AllocateHeapBlock(55, 0xa70);
    save = *(gWorkSlot + HEAP_SLOT_SAVE);
    status = NULL;
    djinn = NULL;
    counts = NULL;
    redraw = 1;
    render = *(gWorkSlot + HEAP_SLOT_WINDOW);
    stamp = Runtime_GetBuildStampTimeFar();
    if (slot < 0)
        slot = 0;
    if (mode != 0)
    {
        for (i = 0; i < 3 && (save->summary[slot].level == 0); i++)
        {
            slot++;
            if (slot == 3)
                slot = 0;
        }
        if (i == 3)
        {
            result = -2;
            goto done;
        }
    }
    if (mode == 1)
    {
        for (i = 0; i < 3 && ((s8)save->summary[slot].sendFlag != 0); i++)
        {
            slot++;
            if (slot == 3)
                slot = 0;
        }
        if (i == 3)
        {
            result = -2;
            goto done;
        }
    }
    if (mode == 4)
    {
        for (i = 0; i < 3 && ((s8)save->summary[slot].flagCount == 0); i++)
        {
            slot++;
            if (slot == 3)
                slot = 0;
        }
        if (i == 3)
        {
            result = -2;
            goto done;
        }
    }
    if (mode == 5)
    {
        for (i = 0; i < 3 && ((s8)save->summary[slot].sendFlag == 0); i++)
        {
            slot++;
            if (slot == 3)
                slot = 0;
        }
        if (i == 3)
        {
            result = -2;
            goto done;
        }
    }

    Dma_Fill32(0, work, 0xa70);
    list = UiWindow_Create(1, 2, 28, 7, 2);

    for (i = 0, summary = save->summary; i < 3; i++, summary++)
    {
        if (save->summary[i].level == 0)
        {
            UiText_DrawResource(MsgSaveNoData, list, 10, i * 16);
        }
        else if (save->summary[i].frames < stamp)
        {
            UiText_DrawResource(MsgSaveContinue, list, 10, i * 16);
        }
        else if (save->summary[i].playTime != *(u32 *)save->summary[i].unk38)
        {
            UiText_DrawResource(MsgSaveCorrupted, list, 10, i * 16);
        }
        else if (mode == 5 && (s8)save->summary[i].sendFlag == 0)
        {
            UiText_DrawResource(MsgSaveNotCleared, list, 10, i * 16);
        }
        else
        {
            y = i * 16;
            UiText_DrawString(summary->name, list, SAVE_SLOT_NAME_X, y);
            UiText_DrawResource(save->summary[i].area + MsgDebugEntryName, list, 72, y);
            render->dirty = 1;
        }
    }
    UiWindow_DrawDividerLine(list, 0, 2, 27, 2);
    UiWindow_DrawDividerLine(list, 0, 4, 27, 4);
    tile = Resource_FindFreeEntry();
    if (tile < 96)
    {
        VramBlock_LoadCached(tile, 128, gResource_FixedBlockBTiles);
        x = -12;
        y = slot * 16 + 4;
        anchor = RenderOutput_Create(tile, 0x40000000, (struct RenderInput *)list, x, y);
        anchor->active = 1;
        marker.anchor = anchor;
    }
    kind = 1;
    cursor = RenderResource_CreatePair(mode, list, 72, -24);

    while (1)
    {
        if (redraw != 0)
        {
            redraw = 0;
            if (save->summary[slot].level != 0)
            {
                PaletteGlow_Update(
                    save->summary[slot].paletteGlow, save->summary[slot].paletteGlowSub);
                if (status == NULL)
                    status = UiWindow_Create(1, 10, 14, 9, 2);
                StatusMenu_DrawCharacterSummary(status, &save->summary[slot]);
                WaitFrames(1);
                if (djinn == NULL)
                    djinn = UiWindow_Create(16, 10, 13, 3, 2);
                Menu_ClearFirstObjectRowAndScheduleUpdate();
                ObjectPlacement_CreateGroup(djinn, 0, 0, &save->summary[slot]);
                WaitFrames(1);
                if ((s8)save->summary[slot].djinn[0] + (s8)save->summary[slot].djinn[1]
                        + (s8)save->summary[slot].djinn[2] + (s8)save->summary[slot].djinn[3]
                    != 0)
                {
                    if (counts == NULL)
                        counts = UiWindow_Create(16, 14, 13, 5, 2);
                    UiText_DrawFourNumbersInRow(counts, &save->summary[slot]);
                    Menu_ClearSecondObjectRowAndScheduleUpdate();
                    Menu_SpawnFourObjectsAtOrigin(counts, 0, 0);
                }
                else
                {
                    Menu_ClearSecondObjectRowAndScheduleUpdate();
                    UiWork_Finalize(counts, 2);
                    counts = NULL;
                }
            }
            else
            {
                PaletteGlow_Update(gGameState.paletteGlow[0], gGameState.paletteGlow[1]);
                Menu_ClearSecondObjectRowAndScheduleUpdate();
                Menu_ClearFirstObjectRowAndScheduleUpdate();
                UiWork_Finalize(counts, 2);
                UiWork_Finalize(djinn, 2);
                UiWork_Finalize(status, 2);
                counts = NULL;
                djinn = NULL;
                status = NULL;
            }
            x = list->x * 8 - 4;
            y = (list->y << 3) + slot * 16 + 12;
            Shop_SetCursorFar(&marker, x, y, kind);
            kind = 3;
        }
        ShopCursor_AdvanceFar(&marker);
        Ui_ApplyTableOffsetToPair(cursor);
        WaitFrames(1);
        if (gKeysRepeat & 0x40)
        {
            AudioCommand_PlayFar(111);
            redraw = 1;
            do
            {
                slot = (slot + 2) % 3;
                if (mode == 0)
                    break;
            } while ((save->summary[slot].level == 0
                      || (mode == 1 && (s8)save->summary[slot].sendFlag != 0)
                      || (mode == 4 && (s8)save->summary[slot].flagCount == 0)
                      || (mode == 5 && (s8)save->summary[slot].sendFlag == 0)));
        }
        else if (gKeysRepeat & 0x80)
        {
            AudioCommand_PlayFar(111);
            redraw = 1;
            do
            {
                slot = (slot + 4) % 3;
                if (mode == 0)
                    break;
            } while ((save->summary[slot].level == 0
                      || (mode == 1 && (s8)save->summary[slot].sendFlag != 0)
                      || (mode == 4 && (s8)save->summary[slot].flagCount == 0)
                      || (mode == 5 && (s8)save->summary[slot].sendFlag == 0)));
        }
        else if (gKeyState & 2)
        {
            AudioCommand_PlayFar(113);
            result = -1;
            break;
        }
        else if (gKeyState & 1)
        {
            AudioCommand_PlayFar(112);
            result = slot;
            break;
        }
    }
    Menu_ClearSecondObjectRowAndScheduleUpdate();
    Menu_ClearFirstObjectRowAndScheduleUpdate();
    UiWork_Finalize(counts, 2);
    UiWork_Finalize(djinn, 2);
    UiWork_Finalize(status, 2);
    UiWork_Finalize(list, 2);
    Runtime_ReleaseHeapBlock(55);
    PaletteGlow_Update(gGameState.paletteGlow[0], gGameState.paletteGlow[1]);
    WaitFrames(1);
done:
    return result;
}
#endif

s32 Save_WriteSelectedSlot(void)
{
    s32                   result;
    s32                   slot;
    s32                   flag;
    struct SaveWorkspace *work;

    result = 0;
    flag = SaveState_InitializeWorkspace();
    if (flag != 0)
    {
        UiText_ShowPositionedMessageAndWait(MsgNoBackupMemory, 1);
        result = -9;
    }
    else
    {
        SaveState_LoadSummaryRecords();
        work = gSaveWorkspace;
        slot = SaveMenu_SelectSlot(gSaveSlot, 0);
        if (slot == -1)
        {
            result = slot;
        }
        else
        {
            if (work->summary[slot].level != 0)
            {
                UiText_ShowPositionedMessageAndWait(MsgOverwriteConfirm, 13);
                while (UiWork_IsComplete() == 0)
                {
                    WaitFrames(1);
                }
                if (Menu_RunConfirmSelection(1, 0, 0, 1) != 0)
                {
                    UiWork_FinalizePendingCore();
                    goto skip;
                }
                UiWork_FinalizePendingCore();
            }
            gSaveSlot = slot;
            AudioCommand_PlayFar(85);
            UiText_ShowPositionedMessageAndWait(MsgSaving, 13);
            while (UiWork_IsComplete() == 0)
            {
                WaitFrames(1);
            }
            SaveState_BuildSummaryHeader();
            SaveState_CaptureObjectTableFar();
            flag = SaveState_WriteRecord(slot, gSaveBuffer);
            flag |= SaveState_WriteRecord(slot + 3, gSaveBuffer + 0x1000);
            UiWork_FinalizePendingCore();
            if (flag != 0)
            {
                UiText_ShowPositionedMessageAndWait(MsgSaveFailed, 1);
                result = -3;
            }
            else
            {
                UiText_ShowPositionedMessageAndWait(MsgGameSaved, 9);
            }
        }
    }
skip:
    SaveState_ReleaseWorkspace();
    return result;
}

s32 SaveState_LoadRecordIntoWork(s32 arg)
{
    s32 ret = 0;
    s32 err = SaveState_InitializeWorkspace();

    if (err != 0)
    {
        UiText_ShowPositionedMessageAndWait(MsgNoBackupMemory, 1);
        ret = -9;
    }
    else
    {
        s32 value;

        SaveState_LoadSummaryRecords();
        value = SaveMenu_SelectSlot(gSaveSlot, arg);
        if (value == -1)
        {
            ret = value;
        }
        else
        {
            void *base = &gSaveBuffer;

            err = SaveState_ReadRecordPayload(value, base);
            base = (char *)base + 0x1000;
            err |= SaveState_ReadRecordPayload(value + 3, base);
            if (err != 0)
            {
                UiText_ShowPositionedMessageAndWait(MsgLoadFailed, 1);
                ret = -2;
            }
            else
            {
                gLoadedStateWord = gGameState.playTime;
                gOptionMirror = gGameState.savedOptions;
                gPostLoadCounter = 0;
                gSaveSlot = value;
            }
        }
    }
    SaveState_ReleaseWorkspace();
    return ret;
}

s32 Resource_LoadFixedBlockBIntoFreeSlot(void)
{
    s32 slot;

    slot = Resource_FindFreeEntry();
    VramBlock_LoadCached(slot, 0x80, gResource_FixedBlockBTiles);
    return slot;
}

#if EDITION_INTERNATIONAL
s32 Resource_ReturnTrue(void)
{
    return 1;
}
#else

s32 NameEntry_AcceptsMark(s32 mark, s32 letter)
{
    const u8 *range;

    if (mark < 0xde || mark > 223)
        return 1;
    if (mark == 0xde)
        range = gNameEntry_VoicedMarkRanges;
    else
        range = gNameEntry_SemivoicedMarkRanges;
    for (; *range != 0; range += 2)
    {
        if (letter >= range[0] && letter <= range[1])
            return 1;
    }
    return 0;
}

s32 NameEntry_CountLetters(const u8 *name)
{
    s32 count;

    count = 0;
    for (; *name != 0; name++)
    {
        if ((u8)(*name - 0xde) > 1)
            count++;
    }
    return count;
}
#endif

void UiWindow_CopyTilemapRegion(const struct RenderInput *window, const void *source)
{
    struct UiRenderWork *work = UI_WORK;
    s16                 *mirror = (s16 *)work->tilemap;
    s16                 *buffer = Runtime_BumpAllocateAlternatePool(0x300);
    s16                 *input = buffer;
    u32                  cell;
    s16                 *vram;
    s32                  row;

    Resource_DecodeType01(source, buffer);
    cell = window->y * 32 + window->x;
    vram = (s16 *)0x06002000 + cell;
    mirror += cell;

    for (row = 0; row < window->height; row++)
    {
        s32 column;

        for (column = 0; column < window->width; column++)
        {
            s16 value = *input++;

            *vram++ = value;
            *mirror++ = value;
        }
        vram += 32 - window->width;
        mirror += 32 - window->width;
    }
    MemFree(buffer);
}

void UiWindow_SetTileAttributeRect(
    const struct RenderInput *window, s32 x, s32 y, s32 width, s32 height, u32 field)
{
    u8 *base = gWindowWork[0];

    x += window->x + 1;
    y += window->y + 1;
    field <<= 12;
    if (x < 0)
    {
        width += x;
        x = 0;
    }
    if (x + width > 29)
    {
        width = 30 - x;
    }
    if (y < 0)
    {
        height += y;
        y = 0;
    }
    if (y + height > 29)
    {
        height = 20 - y;
    }
    if (width > 0 && height > 0)
    {
        y <<= 6;
        x = y + (x << 1);
        do
        {
            u16 *cell = (u16 *)(x + (u32)base);
            s32  remaining = width;
            while (remaining != 0)
            {
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
        ((struct UiRenderWork *)base)->dirty = 1;
    }
}

void Resource_CopyFixedBlockB(s32 arg0)
{
    VramBlock_LoadCached(arg0, 0x80, gResource_FixedBlockBTiles);
}

void Resource_CopyFixedBlockA(s32 arg0)
{
    VramBlock_LoadCached(arg0, 0x80, gResource_FixedBlockATiles);
}

#if EDITION_INTERNATIONAL
s32 UiText_SetRenderString(const u8 *str)
{
    struct UiRenderWork *work;
    u16                 *dst;
    s32                  count;
    u32                  width;
    u32                  height;

    work = UI_WORK;
    count = 0;
    if (*str != 0)
    {
        dst = work->entries;
        do
        {
            *dst = *str;
            str++;
            dst++;
            count++;
        } while (*str != 0);
    }
    work->entries[count] = 0;
    UiText_MeasureEntryDimensions(0, &width, &height, 0);
    return width;
}

void UiText_DrawPaddedLabel(s32 output, u8 *input)
{
    u8  text[20];
    s32 length = 0;

    if (*input != 0)
    {
        do
        {
            text[length] = *input;
            input++;
            length++;
        } while (*input != 0);
    }

    text[length++] = 8;
    text[length++] = 2;

    while (length <= 6)
    {
        text[length++] = 95;
    }

    text[length++] = 8;
    text[length++] = 15;
    text[length] = 0;
    UiText_DrawString(text, output, 0, -2);
}

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
#define NAME_LABEL_WIDTH 9
#else
#define NAME_LABEL_WIDTH 8
#endif

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR)
#define NAME_KEY_LABELS 1
#else
#define NAME_KEY_LABELS 0
#endif
#if defined(TBS_EDITION_DE)
#define NAME_END_KEY_WIDTH 4
#else
#define NAME_END_KEY_WIDTH 3
#endif

s32 NameEntry_EditOwnerName(s32 entry)
{
    struct RenderInput  *label;
    s32                  result = 0;
    s32                  originalLength = 0;
    s32                  length = 0;
    u8                   text[16];
    u8                  *input = text + 1;
    u8                  *saved = (u8 *)UnitGetFar(entry);
    u8                  *work = gWindowWork[0];
    s32                  textDirty = 1;
    s32                  cursorDirty = 1;
    struct RenderInput  *window;
    union MenuCursor     cursor;
    union MenuCursor     caret;
    s32                  column, row, slot;
    struct RenderOutput *output;
    u8                  *end;
    u16                 *pal;

    Ui_LoadWindowGraphics();
    window = (struct RenderInput *)UiWindow_Create(3, 6, 24, 9, 2);
    label = (struct RenderInput *)UiWindow_Create(8, 3, NAME_LABEL_WIDTH, 3, 2);
    UiWindow_CreateWithSideObject(Localization_LookupEntryId(entry), 0, 3, 1);
    UiWindow_CopyTilemapRegion(window, gNameEntry_KeyboardTilemap);
#if NAME_KEY_LABELS
    UiText_DrawStringInWindow(gNameEntry_DeleteKeyLabel, window, 144, 32);
    UiText_DrawStringInWindow(gNameEntry_EndKeyLabel, window, 144, 40);
#endif
    UiWindow_DrawDividerLine(window, 18, 0, 18, 7);
    ((struct UiRenderWork *)work)->dirty = textDirty;
    text[0] = result;
    {
        s32 i;

        for (i = 0; i < 14; i++)
        {
            u32 c = saved[i];

            input[i] = c;
            if (c != 0)
            {
                originalLength++;
                length++;
            }
        }
    }
    input[14] = 0;
    UiText_DrawPaddedLabel((s32)label, saved);
    slot = Resource_FindFreeEntry();
    column = 18;
    row = 5;
    if (slot <= 95)
    {
        VramBlock_LoadCached(slot, 128, gResource_FixedBlockBTiles);
        output = RenderOutput_Create(slot, 0x40000000, window, 0, 0);
        cursor.output = output;
        ShopCursor_SetPositionImmediateFar(&cursor.cursor, window->x * 8 + 140, window->y * 8 + 52);
    }
    slot = Resource_FindFreeEntry();
    if (slot <= 95)
    {
        VramBlock_LoadCached(slot, 128, gResource_FixedBlockATiles);
        output = RenderOutput_Create(slot, 0x40000000, window, 0, 0);
        caret.output = output;
        output->sentinel = 255;
        ((struct FieldSprite *)output->unk10)->priority = 0;
        ShopCursor_SetPositionImmediateFar(&caret.cursor, UiText_SetRenderString(input) + 70, 22);
    }
    pal = (u16 *)0x050001c0;
    Dma_Set((void *)0x050001e0, pal, 0x84000008, REG_DMA3);
    pal[4] = 0x6318;
    end = (u8 *)(originalLength + (s32)input);
    while (1)
    {
        s32 width = 1;

        if (column == 18)
        {
            if (row == 4)
                width = 3;
            if (row == 5)
                width = NAME_END_KEY_WIDTH;
        }
        UiWindow_SetTileAttributeRect(window, column, row, width, 1, 14);
        WaitFrames(1);
        UiWindow_SetTileAttributeRect(window, column, row, width, 1, 15);
        if (cursorDirty)
        {
            cursorDirty = 0;
            Shop_SetCursorFar(
                &cursor.cursor, (window->x + column) * 8 - 7, (window->y + row) * 8 + 15, 3);
        }
        if (textDirty)
        {
            textDirty = 0;
            Shop_SetCursorFar(&caret.cursor, UiText_SetRenderString(input) + 70, 22, 3);
        }
        ShopCursor_AdvanceFar(&cursor.cursor);
        ShopCursor_MoveTowardTargetFar(&caret.cursor);
        {
            u32 frame = (gFrameTick >> 1) & 7;

            output = caret.output;
            ((struct FieldSprite *)output->unk10)->x = output->x + gNameEntry_CaretWave[frame];
            ((struct FieldSprite *)output->unk10)->y =
                output->y + gNameEntry_CaretWave[(frame + 5) & 7];
        }
        if (gKeysRepeat & KEY_UP)
        {
            AudioCommand_PlayFar(111);
            cursorDirty = 1;
            row--;
            if (column != 18)
            {
                if (row == -1)
                    row = 5;
            }
            else
            {
                row = (row != 3);
                row = 5 - row;
            }
        }
        if (gKeysRepeat & KEY_DOWN)
        {
            AudioCommand_PlayFar(111);
            cursorDirty = 1;
            row++;
            if (column != 18)
            {
                if (row == 6)
                    row = 0;
            }
            else
            {
                row = (row != 6);
                row += 4;
            }
        }
        if (gKeysRepeat & KEY_LEFT)
        {
            AudioCommand_PlayFar(111);
            cursorDirty = 1;
            column--;
            if (column == -1)
            {
                column = 18;
                if (row < 4 || row > 5)
                    column = 16;
            }
            else if (column == 5 || column == 11 || column == 17)
            {
                column--;
            }
        }
        if (gKeysRepeat & KEY_RIGHT)
        {
            AudioCommand_PlayFar(111);
            cursorDirty = 1;
            column++;
            if (column == 19)
                column = 0;
            else if (column == 5 || column == 11 || column == 17)
                column++;
            if (column == 18 && (row < 4 || row > 5))
                column = 0;
        }
        if (gKeyState & KEY_START)
        {
            AudioCommand_PlayFar(111);
            cursorDirty = 1;
            column = 18;
            row = 5;
        }
        if (gKeysRepeat & KEY_B)
        {
            AudioCommand_PlayFar(113);
        remove_character:
            if (length != 0)
            {
                length--;
                *--end = 0;
                RenderOutput_PrepareForRedraw(label);
                UiText_DrawPaddedLabel((s32)label, input);
                textDirty = 1;
                continue;
            }
            result = -1;
            break;
        }
        if (!(gKeysRepeat & KEY_A))
            continue;
        AudioCommand_PlayFar(112);
        if (column == 18)
        {
            if (row == 5)
            {
                if (length == 0)
                {
                    caret.output->active = 13;
                    RenderOutput_PrepareForRedraw(label);
                    UiText_DrawPaddedLabel((s32)label, saved);
                    WaitFrames(10);
                }
                else
                {
                    u8 *dst = saved;
                    u8 *src = input;
                    s32 i = 0;
                    u8 *dstSave = dst;
                    u8 *srcSave = src;

                    /* FAKEMATCH: the split restores end the pointer setup's
                     * CSE blocks. They disappear, preserving destination/
                     * source/index in r2/r1/r0; removing the barrier alone
                     * changes them to r1/r0/r2 (14 instructions differ). A for loop
                     * copying saved[i] = input[i], or walking the pointers,
                     * differs 14; the while form with i declared first, 12. */
                    if (saved)
                        src = NULL;
                    if (saved)
                        src = srcSave;
                    if (saved)
                        dst = NULL;
                    if (saved)
                        dst = dstSave;
                    while (i < 15)
                    {
                        *dst++ = *src++;
                        i++;
                    }
                }
                break;
            }
            if (row == 4)
                goto remove_character;
            continue;
        }
        {
            u32 tx = window->x + column + 1;
            u32 cell;
            u32 character;

            cell = ((window->y + row + 1) << 5) + tx;
            character = work[cell * 2];
            if (length == 5)
                continue;
            *end++ = character;
            *end = 0;
            length++;
            if (length == 5)
            {
                column = 18;
                row = 5;
                cursorDirty = 1;
            }
            RenderOutput_PrepareForRedraw(label);
            UiText_DrawPaddedLabel((s32)label, input);
            textDirty = 1;
        }
    }
    UiWork_Finalize((struct UiWindow *)window, 2);
    UiWork_Finalize((struct UiWindow *)label, 2);
    UiWork_FinalizeEntityMatchingLocalizedId(entry);
    WaitFrames(1);
    return result;
}
#else

s32 NameEntry_EditOwnerName(s32 entry)
{
    struct RenderInput  *label;
    s32                  result = 0;
    s32                  keyboard = 0;
    s32                  bytes = 0;
    s32                  length = 0;
    u8                   text[16];
    u8                  *input = text + 1;
    u8                  *saved = (u8 *)UnitGetFar(entry);
    u8                  *work = gWindowWork[0];
    s32                  textDirty = 1;
    s32                  cursorDirty = 1;
    struct RenderInput  *window;
    union MenuCursor     cursor;
    union MenuCursor     caret;
    s32                  column, row, slot, x;
    struct RenderOutput *output;
    u32                  character;
    u16                 *pal;

    Ui_LoadWindowGraphics();
    window = (struct RenderInput *)UiWindow_Create(3, 6, 24, 13, 2);
    label = (struct RenderInput *)UiWindow_Create(8, 3, 8, 3, 2);
    UiWindow_CreateWithSideObject(Localization_LookupEntryId(entry), 0, 3, 1);
    UiWindow_CopyTilemapRegion(window, gNameEntry_KeyboardTilemap);
    UiWindow_DrawDividerLine(window, 18, 0, 18, 12);
    ((struct UiRenderWork *)work)->dirty = textDirty;
    text[0] = result;
    {
        s32 i;

        for (i = 0; i < 14; i++)
        {
            character = saved[i];

            input[i] = character;
            if (character != 0)
            {
                bytes++;
                length++;
                if ((u8)(character - 0xde) <= 1)
                    length--;
            }
        }
    }
    input[14] = 0;
    UiText_DrawString(saved, (s32)label, 0, 0);
    column = 18;
    slot = Resource_FindFreeEntry();
    row = 10;
    if (slot <= 95)
    {
        VramBlock_LoadCached(slot, 128, gResource_FixedBlockBTiles);
        output = RenderOutput_Create(slot, 0x40000000, window, 0, 0);
        cursor.output = output;
        ShopCursor_SetPositionImmediateFar(&cursor.cursor, window->x * 8 + 140, window->y * 8 + 92);
    }
    slot = Resource_FindFreeEntry();
    if (slot <= 95)
    {
        VramBlock_LoadCached(slot, 128, gResource_FixedBlockATiles);
        output = RenderOutput_Create(slot, 0x40000000, window, 0, 0);
        caret.output = output;
        output->sentinel = 255;
        ((struct FieldSprite *)output->unk10)->priority = 0;
        x = 72 + NameEntry_CountLetters(input) * 10;
        ShopCursor_SetPositionImmediateFar(&caret.cursor, x, 22);
    }
    pal = (u16 *)0x050001c0;
    Dma_Set((void *)0x050001e0, pal, 0x84000008, REG_DMA3);
    pal[4] = 0x6318;
    while (1)
    {
        s32 width = 1;

        if (column == 18)
        {
            if (row == 0)
                width = 4;
            if (row == 8)
                width = 3;
            if (row == 10)
                width = 3;
        }
        UiWindow_SetTileAttributeRect(window, column, row, width, 1, 14);
        WaitFrames(1);
        UiWindow_SetTileAttributeRect(window, column, row, width, 1, 15);
        if (cursorDirty)
        {
            cursorDirty = 0;
            Shop_SetCursorFar(
                &cursor.cursor, (window->x + column) * 8 - 7, (window->y + row) * 8 + 15, 3);
        }
        if (textDirty)
        {
            textDirty = 0;
            x = 72 + NameEntry_CountLetters(input) * 10;
            Shop_SetCursorFar(&caret.cursor, x, 22, 3);
        }
        ShopCursor_AdvanceFar(&cursor.cursor);
        ShopCursor_MoveTowardTargetFar(&caret.cursor);
        {
            u32 frame = (gFrameTick >> 1) & 7;

            output = caret.output;
            ((struct FieldSprite *)output->unk10)->x = output->x + gNameEntry_CaretWave[frame];
            ((struct FieldSprite *)output->unk10)->y =
                output->y + gNameEntry_CaretWave[(frame + 5) & 7];
        }
        if (gKeysRepeat & KEY_UP)
        {
            AudioCommand_PlayFar(111);
            cursorDirty = 1;
            row--;
            if (column != 18)
            {
                if (row == -1)
                    row = 10;
                else if (row == 5)
                    row = 4;
            }
            else
            {
                if (row == -1)
                {
                    row = 10;
                }
                else
                {
                    row = (row != 7);
                    row *= 8;
                }
            }
        }
        if (gKeysRepeat & KEY_DOWN)
        {
            AudioCommand_PlayFar(111);
            cursorDirty = 1;
            row++;
            if (column != 18)
            {
                if (row == 11)
                    row = 0;
                if (row == 5)
                    row = 6;
            }
            else
            {
                if (row == 1)
                    row = 8;
                else if (row == 9)
                    row = 10;
                else
                    row = 0;
            }
        }
        if (gKeysRepeat & KEY_LEFT)
        {
            AudioCommand_PlayFar(111);
            cursorDirty = 1;
            column--;
            if (column == -1)
            {
                if (row == 0 || row == 8 || row == 10)
                    column = 18;
                else
                    column = 16;
            }
            else if (column == 5 || column == 11 || column == 17)
            {
                column--;
            }
        }
        if (gKeysRepeat & KEY_RIGHT)
        {
            AudioCommand_PlayFar(111);
            cursorDirty = 1;
            column++;
            if (column == 19)
                column = 0;
            else if (column == 5 || column == 11 || column == 17)
                column++;
            if (column == 18)
            {
                if (row == 0 || row == 8 || row == 10)
                    column = 18;
                else
                    column = 0;
            }
        }
        if (gKeyState & KEY_START)
        {
            AudioCommand_PlayFar(111);
            cursorDirty = 1;
            column = 18;
            row = 10;
        }
        if (gKeyState & KEY_SELECT)
        {
            AudioCommand_PlayFar(111);
        change_keyboard:
            keyboard ^= 1;
            if (keyboard)
                UiWindow_CopyTilemapRegion(window, gNameEntry_AlternateKeyboardTilemap);
            else
                UiWindow_CopyTilemapRegion(window, gNameEntry_KeyboardTilemap);
            UiWindow_DrawDividerLine(window, 18, 0, 18, 12);
            ((struct UiRenderWork *)work)->dirty = 1;
            continue;
        }
        if (gKeysRepeat & KEY_B)
        {
            AudioCommand_PlayFar(113);
        remove_character:
            if (length != 0)
            {
                length--;
                bytes--;
                character = input[bytes];
                input[bytes] = 0;
                if ((u8)(character - 0xde) <= 1)
                {
                    bytes--;
                    input[bytes] = 0;
                }
            }
            else
            {
                result = -1;
                break;
            }
            RenderOutput_PrepareForRedraw(label);
            UiText_DrawString(input, (s32)label, 0, 0);
            textDirty = 1;
            continue;
        }
        if (!(gKeysRepeat & KEY_A))
            continue;
        AudioCommand_PlayFar(112);
        if (column == 18)
        {
            if (row == 10)
            {
                if (length == 0)
                {
                    caret.output->active = 13;
                    RenderOutput_PrepareForRedraw(label);
                    UiText_DrawString(saved, (s32)label, 0, 0);
                    WaitFrames(10);
                }
                else
                {
                    u8 *dst;
                    u8 *src;
                    s32 i;
                    u8 *dstSave;
                    u8 *srcSave;

                    dst = saved;
                    src = input;
                    i = 0;
                    /* FAKEMATCH: the split restores end the pointer setup's
                     * CSE blocks. They disappear, preserving destination/
                     * source/index in r1/r2/r0; removing the barrier alone
                     * changes them to r2/r0/r1 (14 instructions differ). A for loop
                     * copying saved[i] = input[i] differs 18, one walking the
                     * pointers 14; the while form with i declared first, 12. */
                    dstSave = dst;
                    srcSave = src;
                    if (window->height)
                        dst = NULL;
                    if (window->height)
                        dst = dstSave;
                    if (window->height)
                        src = NULL;
                    if (window->height)
                        src = srcSave;
                    while (i < 15)
                    {
                        *dst++ = *src++;
                        i++;
                    }
                }
                break;
            }
            if (row == 8)
                goto remove_character;
            if (row == 0)
                goto change_keyboard;
            continue;
        }
        {
            u32 tx = window->x + column + 1;
            u32 cell;
            u8 *dst;

            cell = ((window->y + row + 1) << 5) + tx;
            character = work[cell * 2];
            if (character == 0xfe)
                character = 0xde;
            if (character == 0xff)
                character = 0xdf;
            if (length == 5 && (u8)(character - 0xde) > 1)
                continue;
            dst = input + bytes;
            if (!NameEntry_AcceptsMark(character, input[bytes - 1]))
            {
                AudioCommand_PlayFar(113);
                continue;
            }
            *dst = character;
            bytes++;
            input[bytes] = 0;
            length++;
            if ((u8)(character - 0xde) <= 1)
                length--;
            if (length == 5)
            {
                column = 18;
                row = 10;
                cursorDirty = 1;
            }
            RenderOutput_PrepareForRedraw(label);
            UiText_DrawString(input, (s32)label, 0, 0);
            textDirty = 1;
        }
    }
    UiWork_Finalize((struct UiWindow *)window, 2);
    UiWork_Finalize((struct UiWindow *)label, 2);
    UiWork_FinalizeEntityMatchingLocalizedId(entry);
    WaitFrames(1);
    return result;
}
#endif

#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR) \
    || defined(TBS_EDITION_IT)
#define TALK_WINDOW_X 7
#define TALK_WINDOW_WIDTH 18
#elif defined(TBS_EDITION_ES)
#define TALK_WINDOW_X 9
#define TALK_WINDOW_WIDTH 12
#else
#define TALK_WINDOW_X 8
#define TALK_WINDOW_WIDTH 16
#endif

s32 PartyTalkMenu_Choose(s32 unused)
{
    /* FAKEMATCH: the shared cursor union preserves pointer-store ordering; an equal-size struct changes stack-address construction in all six editions. */
    struct RenderInput *window;
    union MenuCursor    cursor;
    s32                 slot;
    s32                 row;
    s32                 moved;
    u16                *palette;

    moved = 1;
    window = UiWindow_Create(TALK_WINDOW_X, 13, TALK_WINDOW_WIDTH, 7, 2);
    UiText_DrawCharacterAtOffset(MsgTalkChoice, window, 8, 0);
    UiText_DrawCharacterAtOffset(MsgTalkChoice + 1, window, 8, 16);
    UiText_DrawCharacterAtOffset(MsgTalkChoice + 2, window, 8, 32);
    slot = Resource_FindFreeEntry();
    row = 0;
    if (slot < 96)
    {
        VramBlock_LoadCached(slot, 128, gResource_FixedBlockBTiles);
        cursor.output = RenderOutput_Create(slot, 0x40000000, window, 0, 0);
        ShopCursor_SetPositionImmediateFar(&cursor.cursor, window->x * 8 - 3, window->y * 8 + 9);
    }
    palette = (u16 *)0x050001c0;
    Dma_Set((const void *)0x050001e0, palette, 0x84000008, REG_DMA3);
    palette[4] = 0x6318;
    while (1)
    {
        UiWindow_SetTileAttributeRect(window, 1, row * 2, TALK_WINDOW_WIDTH - 4, 1, 14);
        WaitFrames(1);
        UiWindow_SetTileAttributeRect(window, 1, row * 2, TALK_WINDOW_WIDTH - 4, 1, 15);
        if (moved)
        {
            moved = 0;
            Shop_SetCursorFar(&cursor.cursor, window->x * 8 - 3, (window->y + row * 2) * 8 + 9, 3);
        }
        ShopCursor_AdvanceFar(&cursor.cursor);
        if (gKeysRepeat & KEY_UP)
        {
            AudioCommand_PlayFar(111);
            row--;
            moved = 1;
            if (row == -1)
                row = 2;
        }
        if (gKeysRepeat & KEY_DOWN)
        {
            AudioCommand_PlayFar(111);
            row++;
            moved = 1;
            if (row == 3)
                row = 0;
        }
        if (gKeysRepeat & KEY_B)
        {
            AudioCommand_PlayFar(113);
            row = -1;
            break;
        }
        if (gKeysRepeat & KEY_A)
        {
            AudioCommand_PlayFar(112);
            break;
        }
    }
    UiWork_Finalize(window, 2);
    WaitFrames(1);
    Resource_ResetEntry(slot);
    return row;
}

void Djinn_ShowJoinedMessage(s32 pc, s32 element, s32 djinn)
{
    struct UiRenderWork *work = (struct UiRenderWork *)*gWindowWork;
    s32                  name;
    struct UiWindow     *window;
    s32                  sprite[3];
    s32                 *entry;
    s32                  palette, handle;
    s32                  id;
    s32                  text;
    u32                  zero;
    s32                 *p;

    window = NULL;
    zero = 0;
    name = gDjinn_ElementSprites[element & 3];
    entry = sprite;
    window = UiWindow_Create(DJINN_JOIN_X, 1, DJINN_JOIN_WIDTH, 5, 0);

    if (window != NULL)
    {
        UiWindow_DrawDividerLine(window, 4, 0, 4, 4);

        work->dirty = 1;

        id = Localization_LookupEntryId(name);
        UiGlyph_LoadEntryWithPalette(id, zero, &handle, &palette, 14, zero);

        p = entry;
        *p++ = zero;
        *p++ = 0x8000000c | ((DJINN_JOIN_X * 8 + 4) << 16);
        *p = palette | 0xe000;

        work->result[0] = zero;
        work->result[1] = zero;

        UiWork_PushValueSlot(pc, 1);
        UiWork_PushValueSlot(element * 20 + djinn + 300, 4);

        text = UiText_BuildRenderEntriesMode1(element + MsgVenusDjinnJoined);

        UiText_QueueRenderEntries(window, text, 36, 2, zero);

        AudioCommand_PlayFar(81);

        do
        {
            Runtime_PushSlotEntry(entry, 250);
            WaitFrames(1);
        } while (AudioCommand_GetStateByteFar() != 0
                 && (gKeyState & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);

        UiWork_Finalize(window, 2);
        WaitFrames(1);
        Resource_ResetEntry(handle);
    }
}

s32 Party_LookupCharacterValueByFlag32(u32 index)
{
    if (index > 8)
    {
        return 0;
    }
    if (GameFlag_TestFar(32) == 0)
    {
        return gCharacter_SpriteTable[index];
    }
    return gCharacter_ChildSpriteTable[index];
}

void Party_ShowJoinedMessage(s32 member)
{
    struct UiRenderWork *work = (struct UiRenderWork *)*gWindowWork;
    struct UiWindow     *window = NULL;
    s32                  sprite[3];
    s32                 *entry = sprite;
    s32                  palette, handle;
    s32                 *p;

    window = UiWindow_Create(2, 1, 26, 5, 0);
    if (window != NULL)
    {
        UiWindow_DrawDividerLine(window, 4, 0, 4, 4);
        work->dirty = 1;
        UiGlyph_LoadEntryWithPalette(
            Localization_LookupEntryId(Party_LookupCharacterValueByFlag32(member)), 0, &handle,
            &palette, 14, 0);
        p = entry;
        *p++ = 0;
        *p++ = 0x8000000c | ((2 * 8 + 4) << 16);
        *p = palette | 0xe000;
        work->result[0] = 0;
        work->result[1] = 0;
        UiWork_PushValueSlot(member, 1);
        UiText_QueueRenderEntries(window, UiText_BuildRenderEntriesMode1(MsgJoinedParty), 36, 2, 0);
        AudioCommand_PlayFar(81);
        do
        {
            Runtime_PushSlotEntry(entry, 250);
            WaitFrames(1);
        } while (AudioCommand_GetStateByteFar() != 0
                 && (gKeyState & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);
        UiWork_Finalize(window, 2);
        WaitFrames(1);
        Resource_ResetEntry(handle);
    }
}

void Party_ShowPairJoinedMessage(s32 first, s32 second)
{
    u8              *work = gWindowWork[0];
    struct UiWindow *window;
    s32              sprite2[3];
    s32              sprite1[3];
    s32             *entry1;
    s32             *entry2;
    s32              handle1, palette, handle2;
    u32              zero;
    s32             *p;

    // the window starts as NULL, though it is assigned before it is read; the same zero is the style argument below
    window = NULL;
    entry1 = sprite1;
    window = UiWindow_Create(1, 1, 28, 5, 0);
    zero = 0;

    if (window != NULL)
    {
        UiWindow_DrawDividerLine(window, 8, 0, 4, 4);

        ((struct UiRenderWork *)work)->dirty = 1;

        UiGlyph_LoadEntryWithPalette(
            Localization_LookupEntryId(Party_LookupCharacterValueByFlag32(first)), 0, &handle1,
            &palette, 14, zero);
        p = entry1;
        *p++ = zero;
        *p++ = 0x800c000c;
        *p = palette | 0xe000;

        entry2 = sprite2;
        UiGlyph_LoadEntryWithPalette(
            Localization_LookupEntryId(Party_LookupCharacterValueByFlag32(second)), 0, &handle2,
            &palette, 15, zero);
        p = entry2;
        *p++ = zero;
        *p++ = 0x802c000c;
        *p = palette | 0xf000;

        ((struct UiRenderWork *)work)->result[0] = zero;
        ((struct UiRenderWork *)work)->result[1] = zero;

        UiWork_PushValueSlot(first, 1);
        UiWork_PushValueSlot(second, 1);

        UiText_QueueRenderEntries(
            window, UiText_BuildRenderEntriesMode1(MsgPairJoinedParty), 68, 2, zero);

        AudioCommand_PlayFar(81);

        do
        {
            Runtime_PushSlotEntry(entry1, 250);
            Runtime_PushSlotEntry(entry2, 250);
            WaitFrames(1);
        } while (AudioCommand_GetStateByteFar() != 0
                 && (gKeyState & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);

        UiWork_Finalize(window, 2);
        WaitFrames(1);
        Resource_ResetEntry(handle1);
        Resource_ResetEntry(handle2);
    }
}

void RenderResource_LoadPair(s32 groupIndex, s32 resourceIndex)
{
    void     *stagingBuffer = Runtime_AllocateBlock(14, 0x400);
    const u8 *resource = gRenderResource_PairSourceTable[groupIndex];

    if (resourceIndex <= 0x5F)
    {
        Resource_DecodeByteLz(resource, stagingBuffer);
        VramBlock_LoadCached(resourceIndex, 0x200, stagingBuffer);
        Runtime_ReleaseHeapBlock(14);
    }
}

struct RenderOutput *RenderResource_CreatePair(s32 group, struct RenderInput *input, s32 x, s32 y)
{
    s32                  index;
    struct RenderOutput *first;
    struct RenderOutput *second;

    index = Resource_FindFreeEntry();
    if (index > 95)
        return NULL;

    RenderResource_LoadPair(group, index);
    first = RenderOutput_Create(index, 0x80004000, input, x, y);
    first->sentinel = 0xFD;
    second = RenderOutput_Create(index, 0x80004000, input, x + 32, y);
    second->sentinel = 0xFD;
    second->oam.index += 8;
    return first;
}

void Ui_ApplyTableOffsetToPair(struct RenderOutput *obj)
{
    s32 value;

    value = (u8)obj->y + gUi_PairBobOffsets[(gFrameTick >> 2) & 7];
    obj->oam.y = value;
    obj = obj->next;
    /* FAKEMATCH: the volatile read loads gFrameTick again for the second sprite, as the bytes do; read plainly, CSE reuses the first index and the function differs 33, 15 instructions for 22. */
    value = (u8)obj->y + gUi_PairBobOffsets[(*(volatile u32 *)&gFrameTick >> 2) & 7];
    obj->oam.y = value;
}

void RenderResource_LoadFrame(s32 index, s32 value, s32 flag)
{
    s32   size = 1024;
    void *buffer = Runtime_AllocateBlock(14, size);
    u16  *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);

    if (value <= 95)
    {
        Resource_DecodeByteLz((void *)((u32)base + base[index]), buffer);
        if (flag != 0)
            Runtime_RemapBytesByTableFar(buffer, 768);
        VramBlock_LoadCached(value, size, buffer);
        Runtime_ReleaseHeapBlock(14);
    }
}

struct RenderOutput *RenderResource_CreateFrame(
    s32 arg0, s32 arg1, struct RenderInput *arg2, s32 arg3, s32 arg4)
{
    s32                  index;
    struct RenderOutput *entity;

    index = Resource_FindFreeEntry();
    entity = NULL;
    if (index != 0x60)
    {
        RenderResource_LoadFrame(arg0, index, arg1);
        entity = RenderOutput_Create(index, 0x80000000, arg2, arg3, arg4);
        entity->oam.palette256 = 1;
        entity->sentinel = 0xfb;
    }
    return entity;
}

void Ui_ApplyTableScaleToObject(struct RenderOutput *obj)
{
    s32                    v = gUi_ObjectPulseScales[(gFrameTick >> 1) & 7];
    struct AffineTransform effect;

    if (v < 0)
        v += 255;
    v >>= 8;

    if (obj != 0)
    {
        effect.scaleX = v;
        effect.scaleY = v;
        effect.angle = 0;
        obj->oam.matrix = AffineMatrix_BuildForEffect(&effect);
        obj->oam.affineMode = 3;
        obj->oam.x = obj->x + 0xfff0;
        obj->oam.y = (u8)obj->y + 0xf0;
        obj->sentinel = 0xfc;
    }
}

static __inline__ u32 XorWord(u32 word, u32 mask)
{
    return word ^ mask;
}

s32 Menu_BuildLocalizedPatternTiles(void)
{
    u32 *vram = (u32 *)0x06006280;
    s32  set;
    s32  n;

    for (set = 0; set < 2; set++)
    {
        for (n = 0; n < 6; n++)
        {
            u32 *tile = vram + set * 0x60 + n * 0x10;
            s32  x;

            Iwram_FillWords(tile, 64, 0x44444444);
            for (x = 1; x <= 7; x++)
            {
                s32 mi = n;

                if (set == 1 && x <= 1)
                {
                    continue;
                }
                if (set == 0 && n > x - 2)
                {
                    mi = x - 2;
                    if (mi < 0)
                    {
                        mi = 0;
                    }
                }
                tile[x] = XorWord(tile[x], gMenu_BarPatternRows[mi].word0);
                tile[x + 8] = XorWord(tile[x + 8], gMenu_BarPatternRows[mi].word1);
            }
        }
    }
}

int UiWindow_DrawThreeTileColumn(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 unused)
{
    // 値を返さない int 関数 (pop {r1}; bx r1)
    s32 tile_offset = arg3 * 2;
    s32 tile = tile_offset + 0xF315;

    UiWindow_SetTilemapEntry((struct UiWindow *)arg0, 0x400 | tile, arg1, arg2, 0);
    UiWindow_SetTilemapEntry((struct UiWindow *)arg0, tile_offset + 0xF314, arg1 + 1, arg2, 0);
    UiWindow_SetTilemapEntry((struct UiWindow *)arg0, tile, arg1 + 2, arg2, 0);
}

void Graphics_MergeShiftedTileRows(u32 *first, u32 *second, u32 *output, s32 shift)
{
    s32 neg;
    s32 rshift;
    s32 lshift;
    s32 i;

    neg = -shift;
    i = 0;
    rshift = neg * 4;
    lshift = shift * 4;

    do
    {
        u32 result;
        u32 shifted;
        u32 fallback;
        s32 j;

        shifted = *second++;
        result = 0;
        fallback = *first++;
        if (shift < 0)
            shifted >>= rshift;
        else
            shifted <<= lshift;

        j = 7;
        do
        {
            result <<= 4;
            if (shifted > 0x0FFFFFFF)
                result += shifted >> 28;
            else
                result += fallback >> 28;
            j--;
            shifted <<= 4;
            fallback <<= 4;
        } while (j >= 0);

        *output++ = result;
        i++;
    } while (i <= 7);
}

void Link_DrawShiftedTilePair(s32 offset)
{
    s32 phase = (gFrameCount >> 2) & 3;

    if (phase > 2)
    {
        phase = 2;
    }
    if (phase <= 0)
    {
        phase = 1;
    }
    phase++;
    Graphics_MergeShiftedTileRows(
        (u32 *)0x06000220, (u32 *)gRomShiftedTilePair, (u32 *)offset, -phase);
    Graphics_MergeShiftedTileRows(
        (u32 *)0x06000240, (u32 *)(gRomShiftedTilePair + 32), (u32 *)(offset + 32), phase);
}

void Graphics_ExpandVramTilesByColorTable(u16 *dst)
{
    s32 bank = 0;
    s32 palOffset = 0;
    s32 dstBank = 0;

    do
    {
        s32 row = 0;
        s32 dstOffset = (dstBank + bank) << 6;

        do
        {
            u16 *out = (u16 *)((u8 *)dst + dstOffset);
            u16 *src = (u16 *)((u8 *)0x06000600 + (row << 5));
            s32  col = 0;

            do
            {
                u32 packed = *src++;
                u32 dec = 0;
                s32 nibble = 0;

                do
                {
                    u32 color = gGraphics_ExpandNibbleTable[(packed & 15) + palOffset]
                                << (nibble * 4);
                    nibble++;
                    packed >>= 4;
                    dec |= color;
                } while (nibble <= 3);

                col++;
                *out++ = dec;
            } while (col <= 15);

            row++;
            dstOffset += 32;
        } while (row <= 9);

        palOffset += 16;
        dstBank += 4;
        bank++;
    } while (bank <= 1);
}

s32 Resource_LoadTableEntryToBuffer(s32 resource, s32 index)
{
    s32            result;
    GlyphTransfer *work;

    work = Runtime_AllocateBlock(17, sizeof(*work));
    Ui_PrepareTransferFromTableEntry(resource);
    result = Resource_GetBuffer(index, (s32)work->tiles);
    Runtime_ReleaseHeapBlock(0x11);
    return result;
}

s32 Resource_LoadKind26EntryToBuffer(s32 resource, s32 index)
{
    s32            result;
    GlyphTransfer *work;

    work = Runtime_AllocateBlock(17, sizeof(*work));
    ItemIcon_Compose(resource, 0x1a);
    result = Resource_GetBuffer(index, (s32)work->tiles);
    Runtime_ReleaseHeapBlock(0x11);
    return result;
}

s32 Resource_LoadIndexedEntryToBuffer(s32 resource, s32 index)
{
    s32            out;
    s32            cur;
    s32            ret;
    GlyphTransfer *work;

    work = Runtime_AllocateBlock(17, sizeof(*work));
    cur = index;
    Ability_LoadGlyph(resource, 0, &cur, &out, 1);
    ret = Resource_GetBuffer(index, (s32)work->tiles);
    Runtime_ReleaseHeapBlock(0x11);
    return ret;
}

s32 Ui_LoadEntryForKind(u32 kind, s32 value)
{
    s32 out;
    s32 cur;
    u32 no;

    no = kind;
    cur = value;
    if (no > 7U)
    {
        no = 0;
    }
    if (GameFlag_TestFar(0x20) != 0)
    {
        switch (no)
        {
        case 0:
            no = 0x38;
            break;
        case 1:
            no = 0x39;
            break;
        }
    }
    UiGlyph_LoadEntryWithPalette(no, 0, &cur, &out, 0xE, 1);
    return out;
}

//--------------------------------------------------------------------------
// get_table_word_zero.c
// ウィンドウ：固定のタイルの表の先頭を返す
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// gUi_FixedTileBlocks[0] を返す（0 以外の no も 0 として読む）
//--------------------------------------------------------------------------
s32 Ui_GetTableWordZero(s32 no)
{
    if (no != 0)
        no = 0;
    return gUi_FixedTileBlocks[no];
}

void Ui_RunCopiedRoutine(GlyphTransfer *work)
{

    RamCode_LoadSlot(49, Tile_Decompress4bpp, Tile_Decompress4bppCodeSize);
    ((void (*)(s32, u8 *))((union HeapState *)gWorkSlot)->slots[49])(
        (s32)work->encoded, (u8 *)work);
    Runtime_ReleaseHeapBlock(49);
}

//--------------------------------------------------------------------------
// create_countdown_label_window.c
// 通信：残り時間の見出しのウィンドウを開く
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 左上に 6x4 のウィンドウを開いて見出しを書き、そのハンドルを返す
//--------------------------------------------------------------------------
s32 Link_CreateCountdownLabelWindow(void)
{
    s32 win = UiWindow_Create(0, 0, 6, 4, 6);

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TLA_EDITION_ES) \
    || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    UiText_DrawStringAtOffset(gLink_TimeLabelString, win, 0, 0);
#else
    UiText_DrawStringInWindow(gLink_TimeLabelString, win, 0, 0);
#endif
    return win;
}

s32 Resource_LoadIndexedIntoBuffer(s32 arg0, s32 arg1)
{
    GlyphTransfer *buffer = Runtime_AllocateHeapBlock(0x11, sizeof(GlyphTransfer));
    u16           *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);
    u8           **slot = &buffer->encoded;
    void          *target = (void *)((u32)base + base[arg1]);
    s32            ret;

    *slot = target;
    Resource_DecodeByteLz(target, buffer);
    ret = Resource_GetBuffer(arg0, (s32)buffer);
    Runtime_ReleaseHeapBlock(0x11);
    return ret;
}

void UiText_LoadRemappedGlyph(struct GlyphPalette *palette, s32 glyph, s32 tile)
{
    GlyphTransfer *decoded;
    u8            *font;
    u8            *remapped;
    u8            *src;
    u8            *dst;
    s32            i;
    u32            index;

    decoded = Runtime_AllocateHeapBlock(17, sizeof(*decoded));
    font = Resource_GetTableEntry((u32)&ResourceId_CommandIcons);
    decoded->encoded = font + ((u16 *)font)[glyph];
    Resource_DecodeByteLz(decoded->encoded, decoded);
    remapped = MemAlloc(0x400);
    src = decoded->input;
    dst = remapped;
    for (i = 0; i < 0x400; i++)
    {
        index = *src++;
        if (palette->slot[index] == 255)
        {
            palette->slot[index] = palette->count;
            if (palette->count < 64)
            {
                ((u16 *)0x05000000)[palette->count] = ((u16 *)0x05000200)[index];
                palette->count++;
            }
        }
        *dst++ = palette->slot[index];
    }
    Dma_Set(remapped, (void *)(0x06004000 + tile * 64), 0x84000100, REG_DMA3);
    MemFree(remapped);
    Runtime_ReleaseHeapBlock(17);
}

struct SelectionEntry
{
    struct MenuSprite sprite;
    u16               resource, unused;
    s32               posX, posY;
    s32               kind;
};

struct SelectionWork
{
    struct GlyphPalette    palette;
    struct SelectionEntry  entries[6];
    struct UiWindow       *window;
    s32                    count;
    struct AffineTransform affine;
    s32                    matrix, selection, previous;
    s32                    kinds[6];
};

void UiText_DrawCharacter(struct SelectionWork *work, s32 index, u32 kind)
{
    struct SelectionEntry *entry = &work->entries[index];

    UiText_LoadRemappedGlyph(&work->palette, kind, index * 16);
    work->entries[index].kind = kind;
    MenuSprite_Init(entry->sprite, 0x80002000);
    entry->sprite.oam.f.tile = Resource_LoadIndexedIntoBuffer(work->entries[index].resource, kind);
}

void Graphics_SetBg1Priority3(void)
{
    u32 value = REG_BG1CNT;

    value &= ~3;
    value |= 3;
    REG_BG1CNT = value;
}

void Graphics_ClearBg1ControlBit2(void)
{
    u32 value = REG_BG1CNT;

    value &= ~3;
    REG_BG1CNT = value;
}

void Graphics_InstallBg1VCountHandler(void)
{
    u32 address = 0x04000012;
    s32 zero = 0;

    *(volatile u16 *)address = zero;
    /* FAKEMATCH: instruction order only: the do-while keeps the handler
       load ahead of the index. Plain forms measured (2026-10-13):
       REG_BG0VOFS = 0 or Io_Write16 with a direct call load the halfword
       zero from the pool (differ 6); through an s32 zero the store
       matches but the call sets r0 before loading the handler (differ 2). */
    do
    {
        address = (u32)Graphics_ClearBg1ControlBit2;
    } while (0);
    Runtime_SetIrqHandler(2, 136, (InterruptHandler)address);
}

struct UiWindow *UiText_ShowMessageAndWaitComplete(s32 message, s32 x, s32 y)
{
    struct UiWindow *result;

    result = UiText_OpenMessageWindow(message, x, y, 1);
    while (UiWork_IsComplete() == 0)
        WaitFrames(1);
    return result;
}

#if EDITION_INTERNATIONAL
#define WINDOW_X 22 /* 窓の X */
#else
#define WINDOW_X 23
#endif

s32 Ui_RunSelectionScreen(s32 mode)
{
    u8                    party[4];
    struct UiRenderWork  *render;
    struct SelectionWork *work;
    struct MenuCell      *runtime;
    s32                   rebuild, tradeCount, scale, result;
    s32                   cnt;
    s32                   keys, repeat;
    s32                   slot;
    s32                   i, num;

    render = UI_WORK;
    slot = Resource_LoadIntoFreeSlot(0x400);
    rebuild = 1;
    tradeCount = 0;
    work = Runtime_BumpAllocateAlternatePool(sizeof(*work));
    for (i = 255; i >= 0; i--)
        work->palette.slot[i] = 255;
    work->palette.count = 1;
    if (mode == 0)
        work->window = UiWindow_Create(20, 17, 10, 3, 6);
    else
        work->window = UiWindow_Create(WINDOW_X, 17, 30 - WINDOW_X, 3, 6);
    work->previous = -1;
    if (mode == 0)
    {
        num = 0;
        work->kinds[num++] = 14;
        work->kinds[num++] = 4;
        work->kinds[num++] = 7;
        work->kinds[num] = -1;
        work->selection = gLinkCountdownWork->commandSelection[0];
    }
    else
    {
        work->selection = gLinkCountdownWork->commandSelection[1];
        num = 0;
        work->kinds[num++] = 0;
        work->kinds[num++] = 1;
        if (*((s32 *)Trade_GetOfferStateFar(0)) != 0)
        {
            BattlePlacement_CountValidEntriesFar(0, party);
            for (i = 0; i <= 3; i++)
                tradeCount += party[i];
            work->kinds[num++] = 15;
            if (tradeCount != 0)
                work->kinds[num++] = 16;
        }
        work->kinds[num++] = 2;
        work->kinds[num++] = 3;
        work->kinds[num] = -1;
    }
    for (cnt = 0; cnt <= 5; cnt++)
    {
        work->entries[cnt].resource = slot;
        if (work->kinds[cnt] == -1)
            break;
        UiText_DrawCharacter(work, cnt, work->kinds[cnt]);
    }
    work->count = cnt;
    work->affine.scaleX = 320;
    work->affine.scaleY = 304;
    work->affine.angle = 0;
    cnt = 0;
    if (cnt < work->count)
    {
        do
        {
            s32 x = (cnt - work->count) * 24;
            work->entries[cnt].posX = x + 155;
            if (mode != 0)
                work->entries[cnt].posX = x + WINDOW_X * 8 - 5;
            work->entries[cnt].posY = 136;
            cnt++;
        } while (cnt < work->count);
    }
    Scheduler_AddOrUpdateCallback(Graphics_SetBg1Priority3, 0x480);
    Runtime_SetIrqHandler(2, 136, Graphics_ClearBg1ControlBit2);

    for (;;)
    {
        scale = gUi_BounceScales[(gFrameCount * 2) & 31];
        scale = (scale - 256) / 4 + 304;
        work->affine.scaleY = work->affine.scaleX = scale;
        gBgScroll[1].x = 0;
        gBgScroll[1].y = 32;
        if (rebuild != 0)
        {
            rebuild = 0;
            render->menuBusy = 1;
            if (mode)
            {
                s32 panel;
                s32 count;
                for (panel = 0; panel < 6 - work->count; panel++)
                {
                    for (cnt = 0; cnt <= 2; cnt++)
                    {
                        s32 col;
                        for (col = 0; col <= 2; col++)
                            *(render->tilemap + (cnt + 17) * 32 + panel * 3 + WINDOW_X - 18
                                + (col & 3)) = 0xf07f;
                    }
                }
                for (panel = 0; panel < work->count; panel++)
                {
                    count = work->count;
                    for (cnt = 0; cnt <= 2; cnt++)
                    {
                        s32 col;
                        for (col = 0; col <= 2; col++)
                        {
                            s32 tmp;
                            /* FAKEMATCH: vram, offset and mask restore two forms the bytes prove. Set ahead of the index, they are three more moves out of the column loop in loop.c's first pass, which drops its threshold so the tile pattern's row part stays in the column loop until the second pass: the row loop reduces the index giv first and copies it first (inits index r6, pattern r5; per row mov ip, r6 and str r5, [sp, #4]). volatile keeps the VRAM base add apart from the store it addresses. Without the three 24, without offset 564, without mask 470, without volatile 112. */
                            volatile u16 *vram = (volatile u16 *)0x0600f800 + 21 * 32 + WINDOW_X;
                            s32           offset = 0x100;
                            s32           mask = 3;
                            /* FAKEMATCH: needed to match: tmp, set twice (the tile, then the column's index), is no giv, so the tile is recomputed in the column loop as the bytes show (adds r3, r7, r0; adds r3, r3, r7); 267 differ without it, 846 in both modes. */
                            vram[panel * 3 + cnt * 32 + (col & mask) - count * 3] = tmp =
                                panel * 16 + cnt * 4 + col + offset;
                            tmp = col & mask;
                            *(render->tilemap + (cnt + 17) * 32 + panel * 3 + tmp - count * 3
                                + WINDOW_X) = 0;
                        }
                    }
                }
            }
            else
            {
                s32 panel;
                s32 count;
                for (panel = 0; panel < 6 - work->count; panel++)
                {
                    for (cnt = 0; cnt <= 2; cnt++)
                    {
                        s32 col;
                        for (col = 0; col <= 2; col++)
                            *(render->tilemap + (cnt + 17) * 32 + panel * 3 + 2 + (col & 3)) =
                                0xf07f;
                    }
                }
                for (panel = 0; panel < work->count; panel++)
                {
                    /* FAKEMATCH: needed to match: the empty loop restores the CSE block boundary at the panel body's head, so the exit test compares count instead of reloading work->count (223 differ without it). */
                    do
                    {
                    } while (0);
                    count = work->count;
                    for (cnt = 0; cnt <= 2; cnt++)
                    {
                        s32 col;
                        for (col = 0; col <= 2; col++)
                        {
                            s32 tmp;
                            /* FAKEMATCH: as in mode 1: vram, offset and mask keep the tile pattern in the column loop through loop.c's first pass, volatile keeps the VRAM add apart from its store. */
                            volatile u16 *vram = (volatile u16 *)0x0600f800 + 21 * 32 + 20;
                            s32           offset = 0x100;
                            s32           mask = 3;
                            /* FAKEMATCH: needed to match: tmp, as in mode 1 (271 differ without it, 846 in both modes). */
                            vram[panel * 3 + cnt * 32 + (col & mask) - count * 3] = tmp =
                                panel * 16 + cnt * 4 + col + offset;
                            tmp = col & mask;
                            *(render->tilemap + (cnt + 17) * 32 + panel * 3 + tmp - count * 3
                                + 20) = 0;
                        }
                    }
                }
            }
        }
        if (work->previous != work->selection)
        {
            RenderOutput_PrepareForRedraw(work->window);
            UiText_DrawCharacterAtOffset(work->entries[work->selection].kind + MsgCommandName,
                (struct RenderInput *)work->window, 0, 0);
            work->previous = work->selection;
            UiText_DrawCharacter(work, work->selection, work->kinds[work->selection]);
        }
        work->matrix = AffineMatrix_BuildForEffect(&work->affine);
        for (cnt = 0; cnt < work->count; cnt++)
        {
            struct SelectionEntry *entry = &work->entries[cnt];
            if (cnt == work->selection)
            {
                entry->sprite.oam.f.affineIndex = work->matrix;
                entry->sprite.oam.f.affine = 3;
                entry->sprite.oam.f.x = entry->posX + scale * 7 / 512 - 14;
                entry->sprite.oam.f.y = entry->posY + scale * 3 / 256 - 20;
                Runtime_PushSlotEntry(entry, 0xf1);
            }
            else
            {
                entry->sprite.oam.f.x = entry->posX;
                entry->sprite.oam.f.y = entry->posY;
                entry->sprite.oam.f.affineIndex = 0;
                entry->sprite.oam.f.affine = 0;
            }
        }
        keys = gKeyState;
        repeat = gKeysRepeat;
        runtime = gLinkCountdownWork;
        if (runtime->autoEnabled != 0)
        {
            keys = repeat = 0;
            if (runtime->autoDelay == 0)
            {
                if (runtime->autoMode == 1)
                {
                    if (work->kinds[work->selection] == 3)
                        keys = repeat = 1;
                    else
                        keys = repeat = 32;
                    runtime->autoDelay = 30;
                }
                else if (runtime->autoMode == 0)
                {
                    s32 kind = work->kinds[work->selection];
                    if (kind == 16 || (tradeCount == 0 && kind == 15))
                    {
                        if (runtime->autoEnabled == 1)
                        {
                            struct UiWindow *message;
                            if (kind == 15)
                                message = UiText_ShowMessageAndWaitComplete(
                                    MsgDjinnTutorialUnleashCommand, 15, 8);
                            else if (kind == 16)
                                message = UiText_ShowMessageAndWaitComplete(
                                    MsgDjinnTutorialSummon, 15, 8);
                            BattlePresentation_WaitForPromptAtFar(102, 155);
                            UiWork_Finalize(message, 1);
                            runtime->autoEnabled++;
                            runtime->autoDelay = 45;
                        }
                        else
                        {
                            keys = repeat = 1;
                            runtime->autoDelay = 200;
                        }
                    }
                    else
                    {
                        keys = repeat = 16;
                        runtime->autoDelay = 40;
                    }
                }
                else
                {
                    runtime->autoDelay = 60;
                    keys = repeat = 1;
                }
            }
            else
            {
                runtime->autoDelay--;
            }
        }
        if ((keys & 0x300) && mode != 0)
        {
            result = -2;
            break;
        }
        if (keys & 1)
        {
            result = work->kinds[work->selection];
            break;
        }
        if (mode != 0 && (keys & 2))
        {
            AudioCommand_PlayFar(0x71);
            result = -1;
            break;
        }
        if (repeat & 0x90)
        {
            AudioCommand_PlayFar(0x6f);
            work->selection = (work->selection + 1) % work->count;
        }
        else if (repeat & 0x60)
        {
            AudioCommand_PlayFar(0x6f);
            work->selection = (work->selection + work->count - 1) % work->count;
        }
        else if (gLinkCountdownWork->active == 0)
        {
            result = work->kinds[0];
            break;
        }
        QueueIoWriteDelay2(0x04000000, 0x1741);
        render->menuBusy = 0;
        WaitFrames(1);
    }
    if (mode != 0)
        gLinkCountdownWork->commandSelection[1] = work->selection;
    else
        gLinkCountdownWork->commandSelection[0] = work->selection;
    for (cnt = 0; cnt < work->count; cnt++)
        Resource_ResetEntry(work->entries[cnt].resource);
    render->menuBusy = 1;
    UiWork_Finalize(work->window, 1);
    {
        s32 panel;
        for (panel = 0; panel <= 6; panel++)
        {
            for (cnt = 0; cnt <= 2; cnt++)
            {
                s32 col;
                /* FAKEMATCH: the add of 0x44a stays apart from the store it addresses (adds r3, r7; strh r6, [r3]); as a plain store combine folds it into strh r6, [r3, r7], 0 -> 33 */
                for (col = 0; col <= 2; col++)
                    *(volatile u16 *)(render->tilemap + (cnt + 17) * 32 + panel * 3 + 5
                                      + (col & 3)) = 0xf07f;
            }
        }
    }
    render->dirty = 1;
    WaitFrames(1);
    Scheduler_RemoveCallback(Graphics_SetBg1Priority3);
    Runtime_SetIrqHandler(2, 0, 0);
    IO_WRITE_QUEUE_PUSH(0x04000000, 0x1541, 0x20000);
    MemFree(work);
    render->menuBusy = 0;
    WaitFrames(1);
    return result;
}

void Ui_SetRectHighlight(s32 x, s32 y, s32 width, s32 height, s32 alt)
{
    struct UiRenderWork *render = UI_WORK;

    alt &= 1;
    alt <<= 12;

    if (x < 0)
    {
        width += x;
        x = 0;
    }
    if (x + width > 29)
    {
        width = 30 - x;
    }
    if (y < 0)
    {
        height += y;
        y = 0;
    }
    if (y + height > 29)
    {
        height = 20 - y;
    }

    if (width > 0 && height > 0)
    {
        do
        {
            u16 *p = &((u16(*)[32])render->tilemap)[y][x];
            s32  col = width;

            if (col != 0)
            {
                do
                {
                    u32 tile = *p;
                    tile = (tile & ~0x1000) | alt;
                    col--;
                    *p = tile;
                    p++;
                } while (col != 0);
            }

            UiRender_MarkRow(render, y);
            height--;
            y++;
        } while (height != 0);
    }
}

s32 BattleLayout_HighlightPartyPanels(u16 *ids)
{
    struct BattleWork *battle = gBattleWork;
    s32                count;
    s32                i;
    s32                j;

    count = BattleParty_ListActorIdsFar(1, 0);
    Ui_SetRectHighlight(29 - count * 6, 0, 25, 5, 15);
    i = 0;
    if (ids[0] != 0xff)
    {
    next_id:
        for (j = 0; j < 4 && battle->partyUnits[j] != ids[i]; j++)
        {
            if (battle->partyUnits[j] == 0xff)
            {
                j = 4;
                break;
            }
        }
        if (j != 4)
            Ui_SetRectHighlight(29 - (count - j) * 6, 0, 7, 5, 14);
        if (++i < 4 && ids[i] != 0xff)
            goto next_id;
    }
    return 0;
}

s32 UiText_FormatNumberToHalfwords(s16 *out, s32 value)
{
    // 値を返さない int 関数 (pop {r1}; bx r1)
    s16 *dst;
    s32  n;
    u8  *src;

    dst = out;
    Text_FormatSignedDecimalToWork(value);
    src = (u8 *)((u32)&gNumberTextBuffer);
    n = 0xD;
    do
    {
        n -= 1;
        *dst = *src;
        src += 1;
        dst += 1;
    } while (n >= 0);
}

s32 DjinnMenu_ListChangedDjinn(const struct OwnerActionSlot *oldGrid,
    const struct OwnerActionSlot *newGrid, u16 *out, s32 *gained, s32 *lost)
{
    s32 i;
    s32 j;
    s32 count;
    s32 added;
    s32 removed;

    added = 0;
    removed = 0;
    count = 0;
    for (i = 0; i < 32 && newGrid[i].encodedAction != 0; i++)
    {
        if (BattleActGet(newGrid[i].encodedAction)->targetFlag & 0x80)
        {
            out[count] = newGrid[i].encodedAction & ACTION_ID_MASK;
            count++;
            for (j = 0; j < 32; j++)
            {
                if (((newGrid[i].encodedAction ^ oldGrid[j].encodedAction) & ACTION_ID_MASK) == 0)
                    break;
            }
            if (j == 32)
            {
                added++;
                out[count - 1] |= 0x8000;
            }
        }
    }
    for (i = 0; i < 32 && oldGrid[i].encodedAction != 0; i++)
    {
        if (BattleActGet(oldGrid[i].encodedAction)->targetFlag & 0x80)
        {
            for (j = 0; j < 32; j++)
            {
                if (((oldGrid[i].encodedAction ^ newGrid[j].encodedAction) & ACTION_ID_MASK) == 0)
                    break;
            }
            if (j == 32)
            {
                removed++;
                out[count] = (oldGrid[i].encodedAction & ACTION_ID_MASK) | 0x4000;
                count++;
            }
        }
    }
    *gained = added;
    *lost = removed;
    return count;
}

//--------------------------------------------------------------------------
// create_output_from_resource_slot.c
// 表示：わざのアイコンをスプライトとしてウィンドウに置く
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// あいている VRAM キャッシュにアイコンを読み、16x16 のスプライトで出す
// （あきがなければ何もしない）
// input : 置く先のウィンドウ
// x, y  : ウィンドウの中の位置（ドット）
// ability  : わざの番号
//--------------------------------------------------------------------------
s32 Ui_CreateOutputFromResourceSlot(struct RenderInput *input, s32 x, s32 y, s32 ability)
{
    s32 slot;

    slot = Resource_FindFreeEntry();
    if (slot != VRAM_CACHE_ENTRY_COUNT)
    {
        Resource_LoadIndexedEntryToBuffer(ability, slot);
        RenderOutput_Create(slot, 0x40000000, input, x, y); // OBJ サイズ 16x16
    }
}

struct RenderInput *DjinnMenu_ShowChangePreview(
    struct RenderInput *win, s32 owner, s32 code, s32 page, s32 *pageCount)
{
    struct BattleUnit *unit;
    struct BattleUnit *snap;
    struct BattleAct  *info;
    s16               *buf;
    s32                oldHp;
    s32                oldPp;
    s32                oldAtk;
    s32                oldDef;
    s32                oldAgi;
    u16               *list;
    s32                total;
    s32                n;
    s32                marks;
    s32                rows;
    s32                col;
    s32                row;
    s32                pages;
    s32                gained;
    s32                lost;
    s32                tile;
    s32                val;
    s32                i;

    void DjinnMenu_DrawStatArrow(s32 x, s32 y, s32 rising)
    {
        struct RenderOutput  *output = RenderOutput_AcquireFree();
        struct PreviewSprite *entry;
        u32                   zero;
        if (output)
        {
            output->active = 1;
            output->kind = 1;
            /* FAKEMATCH: instruction order only: a char store (alias set 0) keeps
               the index store ahead of the attribute word in sched2. Plain forms
               measured (2026-10-13) for this and the two typed stores below:
               output->index, entry->attributes.word or entry->tile.value alone
               differ 2, 2 and 4; all three plain differ 2 (the entry copy moves
               after the x and y stores), with entry set first or after the
               sentinel still 2, after x or y 38. */
            *(s8 *)&output->index = Resource_LoadIntoFreeSlot(128);
            entry = (struct PreviewSprite *)((u8 *)output + 16);
            output->sentinel = 240;
            output->x = 120;
            output->y = 120;
            /* FAKEMATCH: typed like the window pointer, the word store waits for the window load but not for the x and y stores, so sched2 places the entry copy before them */
            *(struct RenderInput **)&entry->attributes = (struct RenderInput *)0x40000400;
            zero = 0;
            /* FAKEMATCH: a pointer-typed clear does not wait for the x and y stores (sched2 order) */
            *(void **)&entry->tile = (void *)zero;
            entry->attributes.bits.x = win->x * 8 + x;
            entry->attributes.bits.y = win->y * 8 + y;
            /* FAKEMATCH: advancing and restoring the captured pointer keeps
             * the window chain in r9. Without the pair, r8/r9 allocation
             * changes and two extra instructions remain (80 -> 82, 22 rows). */
            win++;
            win--;
            entry->tile.bits.index = Resource_GetBuffer((u8)output->index,
                (s32)(rising ? gDjinnMenu_ArrowUpTiles : gDjinnMenu_ArrowDownTiles));
            RenderOutput_AppendToList((struct RenderOutputList *)win, output);
        }
    }

    unit = UnitGetFar(owner);
    if (win != 0)
        UiWork_Finalize(win, 1);

    if (page == 0)
        win = UiWindow_Create(0, 8, 21, 11, 6);
    else
        win = UiWindow_Create(0, 5, 21, 14, 6);
    if (win == 0)
        return 0;

    buf = MemAlloc(128);
    snap = MemAlloc(332);
    list = Runtime_BumpAllocateAlternatePool(96);
    Iwram_CopyWords(snap, unit, 332);

    col = (code >> 8) & 15;
    row = code & 255;
    if (Djinn_IsActiveFar(owner, col, row) != 0)
        Djinn_DeactivateFar(owner, col, row);
    else
        Djinn_ActivateFar(owner, col, row);
    UnitCalcFar(owner);

    total = DjinnMenu_ListChangedDjinn(snap->actionSlots, unit->actionSlots, list, &gained, &lost);
    pages = ((total - 1) / 5) + 1;
    *pageCount = pages;
    if (page * 5 - 5 >= total)
        page = pages;

    if (page == 0)
    {
        s32 label = MsgPreviewStatLabel;

        UiText_DrawCharacterAtOffset(label, win, 0, 8);
        UiText_DrawCharacterAtOffset(label + 1, win, 0, 16);
        UiText_DrawCharacterAtOffset(label + 2, win, 0, 24);
        UiText_DrawCharacterAtOffset(label + 3, win, 0, 32);
        UiText_DrawCharacterAtOffset(label + 4, win, 0, 40);
        UiText_DrawCharacterAtOffset(label + 5, win, 0, 48);

        oldHp = snap->hp;
        UiText_FormatNumberToHalfwords(buf, oldHp);
#if !EDITION_INTERNATIONAL
        UiText_RenderWideStringInWindow(buf + 7, win, 4, 1);
#else
        UiText_RenderWideStringInWindow(buf + 7, win, 5, 1);
#endif
        oldPp = snap->pp;
        UiText_FormatNumberToHalfwords(buf, oldPp);
#if !EDITION_INTERNATIONAL
        UiText_RenderWideStringInWindow(buf + 7, win, 4, 2);
#else
        UiText_RenderWideStringInWindow(buf + 7, win, 5, 2);
#endif
        oldAtk = snap->atk;
        UiText_FormatNumberToHalfwords(buf, oldAtk);
#if !EDITION_INTERNATIONAL
        UiText_RenderWideStringInWindow(buf + 7, win, 4, 3);
#else
        UiText_RenderWideStringInWindow(buf + 8, win, 6, 3);
#endif
        oldDef = snap->def;
        UiText_FormatNumberToHalfwords(buf, oldDef);
#if !EDITION_INTERNATIONAL
        UiText_RenderWideStringInWindow(buf + 7, win, 4, 4);
#else
        UiText_RenderWideStringInWindow(buf + 8, win, 6, 4);
#endif
        oldAgi = snap->agi;
        UiText_FormatNumberToHalfwords(buf, oldAgi);
#if !EDITION_INTERNATIONAL
        UiText_RenderWideStringInWindow(buf + 7, win, 4, 5);
#else
        UiText_RenderWideStringInWindow(buf + 8, win, 6, 5);
#endif
        UiText_FormatNumberToHalfwords(buf, snap->luck);
        UiText_RenderWideStringInWindow(buf + 8, win, 5, 6);

        UiWindow_DrawDividerLine(win, 0, 8, 19, 8);
        if (gained != 0 || lost != 0)
            UiWork_SetParamNibble(2);
#if !EDITION_INTERNATIONAL
        UiText_DrawCharacterAtOffset(MsgBattleAdditionalPsynergyHelp, win, 40, 64);
#else
        UiText_DrawCharacterAtOffset(MsgBattleAdditionalPsynergyHelp, win, 24, 64);
#endif
        UiWork_SetParamNibble(15);
    }

    if (page > 0)
    {
        marks = 0;
        rows = total;
        if (total > 4)
            rows = 5;
        i = page * 5 - 5;
        for (n = 0; n < rows && i < total; n++, i++)
        {
            info = BattleActGet(list[i]);
            if (info->damageClass != 4)
                UiWindow_SetTilemapEntry(win, 0x5001 + info->damageClass, 15, n * 2, 0);

            tile = info->range;
            if (tile == 255)
                tile = 11;
            else
                tile--;
            UiWindow_DrawThreeTileColumn((s32)win, 16, n * 2, tile, 0);

            Ui_CreateOutputFromResourceSlot(win, 0, n * 16 - 4, list[i] & 0x3fff);
            if ((list[i] & 0x8000) != 0)
                UiWork_SetParamNibble(4);
            else if ((list[i] & 0x4000) != 0)
                UiWork_SetParamNibble(2);
            else
                UiWork_SetParamNibble(15);

            UiText_DrawCharacterAtOffset((list[i] & 0x3fff) + MsgAbilityName, win, 16, n * 16);
            UiWindow_SetTilemapEntry(win, 0xf01f, 11, n * 2, 0);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
            UiWindow_SetTilemapEntry(win, 0xf005, 12, n * 2, 0);
#else
            UiWindow_SetTilemapEntry(win, 0xf01e, 12, n * 2, 0);
#endif
            UiText_DrawNumberInWindow(BattleActGet(list[i])->ppCost, 2, win, 104, n * 16);
        }

        if (gained != 0)
        {
            UiWork_SetParamNibble(4);
            UiText_DrawCharacterAtOffset(MsgPsynergyGained, win, 32, 80);
            marks = 1;
        }
        if (lost != 0)
        {
            UiWork_SetParamNibble(2);
            UiText_DrawCharacterAtOffset(MsgPsynergyLost, win, 32, marks * 8 + 80);
            marks++;
        }
        if (marks == 0)
            UiText_DrawCharacterAtOffset(MsgPsynergyUnchanged, win, 32, 80);
        UiWork_SetParamNibble(15);
        UiWork_SetParamNibble(15);
        UiWindow_DrawDividerLine(win, 0, 10, 19, 10);
    }

    if (page == 0)
    {
        UiText_DrawCharacterAtOffset(MsgClassName + snap->classIndex, win, 0, 0);
        UiText_DrawCharacterAtOffset(MsgClassName + unit->classIndex, win, 80, 0);
        if (snap->classIndex != unit->classIndex)
            UiWindow_SetTilemapEntry(win, 0xf728, 9, 0, page);
        else
            UiWindow_SetTilemapEntry(win, 0xf729, 9, 0, page);

        UiText_FormatNumberToHalfwords(buf, unit->hp);
        UiText_RenderWideStringInWindow(buf + 7, win, 11, 1);
        if (unit->hp != oldHp)
        {
            val = 0;
            if (unit->hp > oldHp)
                val = 1;
            DjinnMenu_DrawStatArrow(80, 14, val);
        }

        UiText_FormatNumberToHalfwords(buf, unit->pp);
        UiText_RenderWideStringInWindow(buf + 7, win, 11, 2);
        if (unit->pp != oldPp)
        {
            val = 0;
            if (unit->pp > oldPp)
                val = 1;
            DjinnMenu_DrawStatArrow(80, 22, val);
        }

        UiText_FormatNumberToHalfwords(buf, unit->atk);
        UiText_RenderWideStringInWindow(buf + 7, win, 11, 3);
        if (unit->atk != oldAtk)
        {
            val = 0;
            if (unit->atk > oldAtk)
                val = 1;
            DjinnMenu_DrawStatArrow(80, 30, val);
        }

        UiText_FormatNumberToHalfwords(buf, unit->def);
        UiText_RenderWideStringInWindow(buf + 7, win, 11, 4);
        if (unit->def != oldDef)
        {
            val = 0;
            if (unit->def > oldDef)
                val = 1;
            DjinnMenu_DrawStatArrow(80, 38, val);
        }

        UiText_FormatNumberToHalfwords(buf, unit->agi);
        UiText_RenderWideStringInWindow(buf + 7, win, 11, 5);
        if (unit->agi != oldAgi)
        {
            val = 0;
            if (unit->agi > oldAgi)
                val = 1;
            DjinnMenu_DrawStatArrow(80, 46, val);
        }

        UiText_FormatNumberToHalfwords(buf, unit->luck);
        UiText_RenderWideStringInWindow(buf + 8, win, 12, 6);
        if (unit->luck != snap->luck)
        {
            val = 0;
            if (unit->luck > snap->luck)
                val = 1;
            DjinnMenu_DrawStatArrow(80, 54, val);
        }
    }

    Iwram_CopyWords(unit, snap, 332);
    MemFree(list);
    MemFree(snap);
    MemFree(buf);
    return win;
}

#if EDITION_INTERNATIONAL
#define OWNER_STATUS_TEXT_MAX 0x80
#define OWNER_STATUS_VALUE_X 184
#else
#define OWNER_STATUS_TEXT_MAX 0x40
#define OWNER_STATUS_VALUE_X 176
#endif

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
#define OWNER_STATUS_LABEL(row) (MsgOwnerStatusAttack + (row))
#else
#define OWNER_STATUS_LABEL(row) (MsgOwnerStatusExp - 10 + (row))
#endif

struct StatusCursor
{
    s8  pos[5];
    s32 entry;
    s32 lastEntry;
    s32 mode;
    s32 limit;
    s32 col;
    s32 row;
};

s32 Ui_RunOwnerStatusScreen(u16 *list, s32 listCount, s32 owner)
{
    struct MenuSprite                   objFace;
    struct MenuSprite                   objCursor;
    s32                                 table[11];
    s8                                  selection[11];
    struct MenuSprite                   objIcon[11];
    struct MenuSprite                   objName;
    struct StatusCursor                 cur;
    struct UiRenderWork                *base;
    struct UiWindow                    *winMain;
    struct UiWindow                    *winDesc;
    s32                                 idx;
    s32                                 handleCursor;
    struct MenuPoint                    pix;
    s32                                 handleFrame;
    s32                                 cnt;
    s32                                 extended;
    s32                                 keys;
    u16                                *text;
    s32                                 iconIdx;
    s32                                *q;
    s32                                 defDelta;
    s32                                 i;
    s32                                 j;
    s32                                 atkDelta;
    s32                                 redraw;
    struct BattleUnit                  *object;
    struct BattleUnit                  *saved;
    const struct OwnerStatusStatCursor *stat;
    struct MenuSprite                  *p;
    s32                                 agiDelta;
    s32                                 code;
    s32                                 n;
    struct MenuCell                    *screen;
    s32                                 x;

    base = UI_WORK;
    redraw = 1;
    idx = -1;
    handleFrame = Resource_LoadIntoFreeSlot(0x200);
    cnt = 0;
    extended = Party_SumDjinnCountsFar(-1);

    screen = ((struct MenuCell *)gWindowWork[42]);
    screen->busy = 1;
    if (screen->work != NULL)
    {
        UiWork_Finalize(screen->work, 1);
        screen->work = NULL;
    }
    AudioCommand_PlayFar(0x70);

    for (i = 4; i >= 0; i--)
    {
        cur.pos[i] = 0;
    }
    cur.entry = 0;
    cur.lastEntry = 0;
    cur.mode = 0;

    handleCursor = Resource_LoadIntoFreeSlot(0x80);
    for (i = 0; i <= 10; i++)
    {
        table[i] = Resource_LoadIntoFreeSlot(0x80);
        selection[i] = -1;
    }

    if (listCount != 0)
    {
        for (i = 0; i <= 5; i++)
        {
            s32 kind = list[i];

            if (kind == 0xFF)
            {
                break;
            }
            if (kind != 0xFE)
            {
                if (kind == owner)
                {
                    idx = i;
                    break;
                }
            }
        }
    }

    pix.x = 0;
    pix.y = 0;
    winMain = UiWindow_Create(0, 0, 30, 20, 6);
    winDesc = UiWindow_Create(0, 14, 30, 6, 10);
    UiWindow_MarkVisibleTileAttributes();

    for (;;)
    {
        keys = gKeysRepeat;

        objName.oam.word.attr01 = 0x80000400;
        objName.oam.word.attr23 = 0;
        objName.oam.f.tile = Ui_LoadEntryForKind(owner, handleFrame);
        objName.oam.f.x = 8;
        objName.oam.f.y = 24;
        objName.oam.f.palette = 14;
        Runtime_PushSlotEntry((s32 *)&objName, 240);

        switch (cur.entry)
        {
        case 0:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        default:
            cur.mode = 0;
            if (extended != 0)
            {
                cur.limit = 9;
            }
            else
            {
                cur.limit = 7;
            }
            break;
        case 10:
        case 11:
        case 12:
        case 13:
            cur.mode = 1;
            cur.limit = 4;
            break;
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
            cur.mode = 2;
            cur.limit = cnt;
            break;
        }

        Ui_SetRectHighlight(winMain->x, winMain->y, winMain->width, winMain->height, 15);

        if (cnt != 0)
        {
            if ((u32)cur.mode <= 1)
            {
                n = cur.pos[cur.mode];
                if ((keys & 0x80) != 0)
                {
                    keys = 0;
                    AudioCommand_PlayFar(0x6F);
                    n++;
                    if (n >= cur.limit)
                    {
                        n = 0;
                        if (cur.mode == 1)
                        {
                            cur.mode = 2;
                            n = cur.pos[cur.mode];
                        }
                    }
                }
                else if ((keys & 0x40) != 0)
                {
                    keys = 0;
                    AudioCommand_PlayFar(0x6F);
                    n--;
                    if (n < 0)
                    {
                        n = cur.limit - 1;
                        if (cur.mode == 1)
                        {
                            cur.mode = 2;
                            n = cur.pos[cur.mode];
                        }
                    }
                }
                else if ((keys & 0x31) != 0)
                {
                    keys = 0;
                    AudioCommand_PlayFar(0x6F);
                    cur.mode ^= 2;
                    if ((gKeysRepeat & KEY_A) != 0)
                    {
                        n = cur.pos[cur.mode];
                    }
                    else if ((gKeysRepeat & KEY_LEFT) != 0)
                    {
                        n = cnt - 1;
                    }
                    else
                    {
                        n = 0;
                    }
                }
                cur.pos[cur.mode] = (s8)n;
            }
            else if (cur.mode == 2)
            {
                n = cur.pos[cur.mode];
                if (n >= cur.limit)
                {
                    n = cur.limit - 1;
                }
                if (n < 0)
                {
                    cur.mode = 0;
                    n = cur.pos[0];
                }
                else if ((keys & 0x10) != 0)
                {
                    keys = 0;
                    n++;
                    if (n >= cur.limit)
                    {
                        cur.mode = 0;
                        n = cur.pos[0];
                    }
                    AudioCommand_PlayFar(0x6F);
                }
                else if ((keys & 0x20) != 0)
                {
                    keys = 0;
                    n--;
                    if (n < 0)
                    {
                        cur.mode = 0;
                        n = cur.pos[0];
                    }
                    AudioCommand_PlayFar(0x6F);
                }
                else if ((keys & 0xC1) != 0)
                {
                    keys = 0;
                    cur.mode = 0;
                    n = cur.pos[0];
                    AudioCommand_PlayFar(0x6F);
                }
                cur.pos[cur.mode] = (s8)n;
            }
        }

        if (cur.mode == 0)
        {
            s32 k = cur.pos[cur.mode];

            if (extended == 0)
            {
                k += 9;
            }
            stat = &gOwnerStatus_StatCursors[k];
            cur.entry = stat->entry;
            cur.col = stat->column;
            cur.row = stat->row;
            Ui_SetRectHighlight(winMain->x + stat->highlightColumn + 1,
                winMain->y + stat->highlightRow + 1, stat->highlightWidth, 1, 14);
        }
        else if (cur.mode == 1)
        {
            cur.entry = gOwnerStatus_EquipmentCursors[cur.pos[cur.mode]].entry;
            cur.col = gOwnerStatus_EquipmentCursors[cur.pos[cur.mode]].column;
            cur.row = gOwnerStatus_EquipmentCursors[cur.pos[cur.mode]].row;
        }
        else if (cur.mode == 2)
        {
            cur.entry = gOwnerStatus_ConditionCursors[cur.pos[cur.mode]].entry;
            cur.col = gOwnerStatus_ConditionCursors[cur.pos[cur.mode]].column;
            cur.row = gOwnerStatus_ConditionCursors[cur.pos[cur.mode]].row;
        }

        if (cur.lastEntry != cur.entry)
        {
            cur.lastEntry = cur.entry;
            redraw = 2;
        }
        pix.x = cur.col * 8;
        pix.y = cur.row * 8;

        {
            struct MenuSprite *face = GetBattleObjectSlotFar(owner)->object->records;

            objFace.oam.word.attr01 = 0xC0002400;
            objFace.oam.word.attr23 = 0;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
            /* FAKEMATCH: the zero lives in sl here; sched2 puts the attr23 store before the move into sl and the 0xFC00 load before the face tile load, which may alias that store; native moves into sl between the two stores and loads the tile first (4 instructions) */
            asm("");
#endif
            objFace.oam.f.tile = face->oam.f.tile;
            objFace.oam.f.x = 0xAC;
            objFace.oam.f.y = 56;
            Runtime_PushSlotEntry((s32 *)&objFace, 240);
        }

        objCursor.oam.word.attr01 = 0x40000400;
        objCursor.oam.word.attr23 = 0;
        /* FAKEMATCH: arm_reorg counts this empty asm as 4 bytes, so the gFrameCount literal falls past the face x literal's 64-byte ldrh range and opens the next pool, as native; the pool and its branch move two instructions earlier, 85 -> 63. */
        /* FAKEMATCH: removing both barriers gives EN 1392/1392, common 1390, differ 4: gFrameCount joins the earlier pool and the branch follows two extra loads. ES 1458/1458, common 1455, differ 6: attr23/mov sl order and the 0x3ff load also move. 2026-10-11: arm_reorg runs before shorten_branches and counts the empty asm at its 4-byte default length while it emits nothing; the halfword constant fix needs that much more estimated length after it, and no C statement or single pattern-length change (strh, str, orr, and tested over all 413 English objects) supplies it. In ES and IT both asms are scheduling barriers only (differ 8 without them). */
        asm("");
        objCursor.oam.f.tile = Resource_GetBuffer(handleCursor, (s32)gResource_FixedBlockBTiles);
        objCursor.oam.f.x = (pix.x + winMain->x * 8 - ((gFrameCount & 4) >> 2)) + 16;
        objCursor.oam.f.y = (u8)((pix.y + winMain->y * 8 - ((gFrameCount & 4) >> 2)) + 16);
        objCursor.oam.f.affineIndex = 8;
        Runtime_PushSlotEntry((s32 *)&objCursor, 241);

        if (redraw != 0)
        {
            object = UnitGetFar(owner);
#if EDITION_INTERNATIONAL
            UiWindow_MarkVisibleTileAttributes();
#endif
            Ui_FillVramBlockPattern();

            if ((redraw & 1) != 0)
            {
                RenderOutput_RedrawSavedRect(winMain);
                UiWindow_MapTextCanvasTiles(
                    winDesc->x, winDesc->y, winDesc->width, winDesc->height, 0);
                UiWindow_DrawDividerLine((struct RenderInput *)winMain, 0, 14, 29, 14);

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
                switch (System_GetLanguage())
                {
                case 2:
                    UiText_DrawStringAtOffset(
                        (u8 *)object->name, (struct RenderInput *)winMain, 0, 0);
                    UiText_DrawStringAtOffset(
                        (u8 *)gOwnerStatus_NivString, (struct RenderInput *)winMain, 51, 0);
                    UiText_DrawNumberInWindow(
                        object->level, 2, (struct RenderInput *)winMain, 72, 0);
                    break;
                case 3:
                    UiText_DrawStringAtOffset(
                        (u8 *)object->name, (struct RenderInput *)winMain, 0, 0);
                    UiText_DrawStringAtOffset(
                        (u8 *)gOwnerStatus_NvString, (struct RenderInput *)winMain, 56, 0);
                    UiText_DrawNumberInWindow(
                        object->level, 2, (struct RenderInput *)winMain, 72, 0);
                    break;
                default:
                    UiText_DrawStringAtOffset(
                        (u8 *)object->name, (struct RenderInput *)winMain, 0, 0);
                    UiText_DrawStringAtOffset(
                        (u8 *)gOwnerStatus_LevelString, (struct RenderInput *)winMain, 56, 0);
                    UiText_DrawNumberInWindow(
                        object->level, 2, (struct RenderInput *)winMain, 72, 0);
                    break;
                }
#else
                UiText_DrawStringAtOffset((u8 *)object->name, (struct RenderInput *)winMain, 0, 0);
#if defined(TBS_EDITION_FR)
                UiText_DrawStringAtOffset(
                    (u8 *)gOwnerStatus_LevelString, (struct RenderInput *)winMain, 51, 0);
#else
                UiText_DrawStringAtOffset(
                    (u8 *)gOwnerStatus_LevelString, (struct RenderInput *)winMain, 56, 0);
#endif
                UiText_DrawNumberInWindow(object->level, 2, (struct RenderInput *)winMain, 72, 0);
#endif
                UiText_DrawCharacterAtOffset(
                    MsgOwnerStatusExp, (struct RenderInput *)winMain, 0, 8);
                UiText_DrawNumberInWindow(
                    object->experience, 8, (struct RenderInput *)winMain, 40, 8);

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
                switch (System_GetLanguage())
                {
                case 1:
                    UiText_DrawStringAtOffset(
                        (u8 *)gOwnerStatus_KpString, (struct RenderInput *)winMain, 40, 24);
                    break;
                case 2:
                    UiText_DrawStringAtOffset(
                        (u8 *)gOwnerStatus_PvString, (struct RenderInput *)winMain, 40, 24);
                    break;
                case 3:
                    UiText_DrawStringAtOffset(
                        (u8 *)gOwnerStatus_PvString, (struct RenderInput *)winMain, 40, 24);
                    break;
                case 4:
                    UiText_DrawStringAtOffset(
                        (u8 *)gOwnerStatus_HpString, (struct RenderInput *)winMain, 40, 24);
                    break;
                }
#else
                UiText_DrawStringAtOffset(
                    (u8 *)gOwnerStatus_HpString, (struct RenderInput *)winMain, 40, 24);
#endif
                UiText_DrawNumberInWindow(object->hp, 4, (struct RenderInput *)winMain, 56, 24);
                UiText_DrawStringAtOffset(
                    (u8 *)gOwnerStatus_SlashString, (struct RenderInput *)winMain, 88, 24);
                UiText_DrawNumberInWindow(object->maxHp, 4, (struct RenderInput *)winMain, 96, 24);
                UiText_DrawStringAtOffset(
                    (u8 *)gOwnerStatus_PpString, (struct RenderInput *)winMain, 40, 32);
                UiText_DrawNumberInWindow(object->pp, 4, (struct RenderInput *)winMain, 56, 32);
                UiText_DrawStringAtOffset(
                    (u8 *)gOwnerStatus_SlashString, (struct RenderInput *)winMain, 88, 32);
                UiText_DrawNumberInWindow(object->maxPp, 4, (struct RenderInput *)winMain, 96, 32);

                UiText_DrawCharacterAtOffset(
                    OWNER_STATUS_LABEL(0), (struct RenderInput *)winMain, 136, 16);
                UiText_DrawNumberInWindow(
                    object->atk, 3, (struct RenderInput *)winMain, OWNER_STATUS_VALUE_X, 16);
                UiText_DrawCharacterAtOffset(
                    OWNER_STATUS_LABEL(1), (struct RenderInput *)winMain, 136, 24);
                UiText_DrawNumberInWindow(
                    object->def, 3, (struct RenderInput *)winMain, OWNER_STATUS_VALUE_X, 24);
                UiText_DrawCharacterAtOffset(
                    OWNER_STATUS_LABEL(2), (struct RenderInput *)winMain, 136, 32);
                UiText_DrawNumberInWindow(
                    object->agi, 3, (struct RenderInput *)winMain, OWNER_STATUS_VALUE_X, 32);
                UiText_DrawCharacterAtOffset(
                    OWNER_STATUS_LABEL(3), (struct RenderInput *)winMain, 136, 40);
                UiText_DrawNumberInWindow(
                    object->luck, 3, (struct RenderInput *)winMain, OWNER_STATUS_VALUE_X, 40);

                UiText_DrawCharacterAtOffset(
                    MsgClassName + object->classIndex, (struct RenderInput *)winMain, 0, 48);
                if (extended != 0)
                {
                    UiText_DrawCharacterAtOffset(
                        OWNER_STATUS_LABEL(9), (struct RenderInput *)winMain, 0, 72);
                }
                UiText_DrawCharacterAtOffset(
                    OWNER_STATUS_LABEL(5), (struct RenderInput *)winMain, 0, 80);
                UiText_DrawCharacterAtOffset(
                    OWNER_STATUS_LABEL(6), (struct RenderInput *)winMain, 0, 88);
                UiText_DrawCharacterAtOffset(
                    OWNER_STATUS_LABEL(7), (struct RenderInput *)winMain, 0, 96);

                for (j = 0; j <= 3; j++)
                {
                    s32 pal;

                    pal = 1;
                    if (extended != 0)
                    {
                        pal = 0;
                    }
                    UiWindow_SetTilemapEntry(
                        (struct RenderInput *)winMain, 0x5001 + j, 7 + j * 4, pal + 8, 0);
                    if (extended != 0)
                    {
                        UiText_DrawNumberInWindow(object->djinnActiveCounts[j], 1,
                            (struct RenderInput *)winMain, 40 + j * 32, 72);
                        UiText_DrawStringAtOffset((u8 *)gOwnerStatus_SlashString,
                            (struct RenderInput *)winMain, 48 + j * 32, 72);
                        UiText_DrawNumberInWindow(object->djinnOwnedCounts[j], 1,
                            (struct RenderInput *)winMain, 48 + j * 32 + 8, 72);
                    }
                    UiText_DrawNumberInWindow(Owner_GetResistanceValueFar(owner, j), 2,
                        (struct RenderInput *)winMain, 48 + j * 32, 80);
                    UiText_DrawNumberInWindow(object->element[j].power, 3,
                        (struct RenderInput *)winMain, 40 + j * 32, 88);
                    UiText_DrawNumberInWindow(
                        object->element[j].res, 3, (struct RenderInput *)winMain, 40 + j * 32, 96);
                }

                i = 0;
                if (object->hp == 0)
                {
                    selection[0] = 16;
                    i = 1;
                }
                do
                {
                    if (object->restraint != 0)
                    {
                        selection[i++] = 15;
                    }
                    if (i > 7)
                    {
                        break;
                    }
                    if (object->grimReaper != 0)
                    {
                        selection[i++] = 8;
                    }
                    if (i > 7)
                    {
                        break;
                    }
                    if (object->poison == 1)
                    {
                        selection[i++] = 1;
                    }
                    if (i > 7)
                    {
                        break;
                    }
                    if (object->poison == 2)
                    {
                        selection[i++] = 2;
                    }
                    if (i > 7)
                    {
                        break;
                    }
                    if (object->psySeal != 0)
                    {
                        selection[i++] = 4;
                    }
                    if (i > 7)
                    {
                        break;
                    }
                    if (object->stun != 0)
                    {
                        selection[i++] = 3;
                    }
                    if (i > 7)
                    {
                        break;
                    }
                    if (object->sleep != 0)
                    {
                        selection[i++] = 5;
                    }
                    if (i > 7)
                    {
                        break;
                    }
                    if (object->haunt != 0)
                    {
                        selection[i++] = 7;
                    }
                    if (i > 7)
                    {
                        break;
                    }
                    if (object->delusion != 0)
                    {
                        selection[i++] = 6;
                    }
                    if (i > 7)
                    {
                        break;
                    }
                    if (object->atkCnt != 0)
                    {
                        if (object->atkLv > 0)
                        {
                            selection[i++] = 9;
                        }
                        if (object->atkLv < 0)
                        {
                            selection[i++] = 10;
                        }
                    }
                    if (i > 7)
                    {
                        break;
                    }
                    if (object->defCnt != 0)
                    {
                        if (object->defLv > 0)
                        {
                            selection[i++] = 11;
                        }
                        if (object->defLv < 0)
                        {
                            selection[i++] = 12;
                        }
                    }
                    if (i > 7)
                    {
                        break;
                    }
                    if (object->resCnt != 0)
                    {
                        if (object->resLv > 0)
                        {
                            selection[i++] = 13;
                        }
                        if (object->resLv < 0)
                        {
                            selection[i++] = 14;
                        }
                    }
                    if (i > 7)
                    {
                        break;
                    }
                    if (object->agiLv > 0)
                    {
                        selection[i++] = 17;
                    }
                    if (object->agiLv < 0)
                    {
                        selection[i++] = 18;
                    }
                } while (0);

                for (j = 0; j < i; j++)
                {
                    Resource_LoadTableEntryToBuffer(selection[j], table[j]);
                }
                if (i == 0)
                {
                    selection[0] = 0;
                    i = 1;
                }
                if (i <= 10)
                {
                    for (j = i; j < 11; j++)
                    {
                        selection[j] = -1;
                    }
                }
                cnt = i;
                if (selection[0] == 0)
                {
                    UiText_DrawCharacterAtOffset(
                        object->hp != 0 ? MsgOwnerStatusNormal : MsgOwnerStatusDowned,
                        (struct RenderInput *)winMain, 112, 0);
                }
            }

            text = (u16 *)MemAlloc(OWNER_STATUS_TEXT_MAX * sizeof(*text));
            if ((u32)cur.entry > 13)
            {
                iconIdx = selection[cur.entry - 14];
                if (iconIdx == 0 && object->hp == 0)
                {
                    iconIdx = 16;
                }

                saved = (struct BattleUnit *)MemAlloc(sizeof(*object));
                code = 0;
                Iwram_CopyWords(saved, object, sizeof(*object));

                atkDelta = object->atk;
                defDelta = object->def;
                agiDelta = object->agi;
                object->atkLv = 0;
                object->defLv = 0;
                object->agiLv = 0;
                UnitCalcFar(owner);
                atkDelta -= object->atk;
                defDelta -= object->def;
                agiDelta -= object->agi;
                Iwram_CopyWords(object, saved, sizeof(*object));
                MemFree(saved);

                switch (iconIdx)
                {
                case 8:
                    code = object->grimReaper;
                    break;
                case 9:
                    code = atkDelta;
                    break;
                case 10:
                    code = -atkDelta;
                    break;
                case 11:
                    code = defDelta;
                    break;
                case 12:
                    code = -defDelta;
                    break;
                case 17:
                    code = agiDelta;
                    break;
                case 18:
                    code = -agiDelta;
                    break;
                case 13:
                case 14:
                    code = object->resLv * 20;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
                    if (code < 0)
                    {
                        code = -code;
                    }
#endif
                    break;
                default:
                    break;
                }
                UiWork_PushValueSlot(code, 5);
                UiText_CopyMessageString(
                    iconIdx + MsgOwnerStatusConditionHelp, text, OWNER_STATUS_TEXT_MAX);
            }
            else if (cur.entry == 2 && object->level <= 98)
            {
                UiWork_PushValueSlot(
                    Owner_GetLevelThresholdFar(owner, object->level + 1) - object->experience, 5);
                UiText_CopyMessageString(MsgOwnerStatusNextLevel, text, OWNER_STATUS_TEXT_MAX);
            }
            else
            {
                UiText_CopyMessageString(
                    cur.entry + MsgOwnerStatusEntryHelp, text, OWNER_STATUS_TEXT_MAX);
            }

            UiText_RenderWideStringAtOffset(text, (struct UiWindow *)winDesc, 0, 4);
            MemFree(text);
            redraw = 0;
            base->dirty = 1;
        }

        p = objIcon;
        q = table;
        {
            struct VramBlockCacheEntry *vc;

            for (i = 0, vc = gVramBlockCache, x = 112; i <= 10; i++)
            {
                p->oam.word.attr01 = 0x40000400;
                p->oam.word.attr23 = 0;
                p->oam.f.tile = vc[*q++].offset >> 5;
                p->oam.f.x = x;
                p->oam.f.y = (u8)(winMain->y * 8 + 8);
                if (selection[i] > 0)
                {
                    Runtime_PushSlotEntry((s32 *)p, 240);
                }
                x += 15;
                p++;
            }
        }

        if (gLinkCountdownWork->active == 0)
        {
            break;
        }
        if ((gKeyState & KEY_B) != 0)
        {
            break;
        }

        if (listCount != 0)
        {
            if ((keys & 0x100) != 0)
            {
                idx++;
                if (idx >= listCount)
                {
                    idx = 0;
                }
                owner = list[idx];
                redraw = 1;
                AudioCommand_PlayFar(0x6F);
            }
            else if ((keys & 0x200) != 0)
            {
                idx--;
                if (idx < 0)
                {
                    idx = listCount - 1;
                }
                owner = list[idx];
                redraw = 1;
                AudioCommand_PlayFar(0x6F);
            }
        }
        WaitFrames(1);
    }

    {
        s32 *h = table;

        for (i = 10; i >= 0; i--)
        {
            Resource_ResetEntry(*h++);
        }
    }
    Resource_ResetEntry(handleFrame);
    Resource_ResetEntry(handleCursor);
    WaitFrames(1);
#if EDITION_INTERNATIONAL
    UiWindow_MarkVisibleTileAttributes();
#endif
    UiWork_Finalize(winMain, 1);
    UiWork_Finalize(winDesc, 1);
    PartyStatusDraw(gBattleWork->partyStatus);
    ((struct MenuCell **)&gBattleWork)[HEAP_SLOT_WINDOW + 42 - HEAP_SLOT_BATTLE]->busy = 0;
    WaitFrames(1);
    return 0;
}

#if EDITION_INTERNATIONAL
#define TEXT_COUNT 52
#define TEXT_LOAD UiText_CopyMessageString
#define TURNS_DIGITS 1
#define TURNS_X 48
#else
#define TEXT_COUNT 32
#define TEXT_LOAD UiText_DecodeMessage
#define TURNS_DIGITS 2
#define TURNS_X 40
#endif

// gLinkCountdownWork のブロック全体。0x28 から 0xe4 までは menu_list.h の MenuCell が読むところ
struct LinkRoundState
{
    struct MenuSprite entries[3];
    u8                entryActive[3];
    u8                unk27;
    s32               targetOffset;
    s32               currentOffset;
    s32               unk30;
    s32               unk34;
    s32               unk38;
    s32               unk3c;
    s32               unk40;
    s32               displayHandle;
    s32               secondaryHandle;
    s32               timer;
    s32               enabled;
    u8                cost[8][4];
    u8                pick[34];
    u16               list[33];
    s32               hintCount;
    s32               hintTimer;
    s32               actorId;
    u32               selected[8];
};

#define COUNTDOWN ((struct LinkRoundState *)gLinkCountdownWork)
#define CURSOR_OWNER (*(struct PartyStatusWindow **)gPartyStatusWork)

extern const u8 gDjinnMenu_BlinkTile[32];
extern const u8 gDjinnMenu_ElementXOffsets[4];
extern const u8 gDjinnMenu_ElementYOffsets[4];

s32 Battle_SelectAbility(s32 owner, s32 unused)
{
    struct CursorSprite       slot;
    u16                       buf[64];
    struct UiRenderWork      *ctx;
    struct UiWindow          *detailWin;
    s32                       prevRow;
    s32                       res;
    u32                      *table;
    s32                       cnt;
    s32                       prevPage;
    struct UiWindow          *msgWin;
    s32                       mask;
    s32                       savedRow;
    s32                       subSelection;
    s32                       redraw;
    s32                       isUsed;
    s32                       subCnt;
    struct MenuCell          *menu;
    struct BattleUnit        *unit;
    struct DjinnRecoveryList *list;
    struct UiWindow          *listWin;
    struct UiWindow          *iconWin;
    struct PartyStatusWindow *cursor;
    u32                      *pos;
    u32                       cur;
    u32                       blink;
    s32                       page;
    s32                       row;
    u32                       code;
    s32                       pages;
    s32                       pal;
    struct MenuPoint          cursorPos;
    s32                       trig;
    s32                       rep;
    s32                       opts;
    s32                       x;
    s32                       y;
    s32                       i;
    s32                       j;
    struct CursorSprite      *curSpr;

    ctx = UI_WORK;
    detailWin = 0;
    prevRow = -1;
    res = Resource_LoadIntoFreeSlot(0x80);
    table = Runtime_BumpAllocateAlternatePool(0xa8 * 2);
    cnt = 0;
    prevPage = -1;
    code = 0;
    mask = 0;
    subSelection = 0;
    subCnt = 0;
    redraw = 0;
    isUsed = 0;
    msgWin = UiWindow_Create(0, 4, 30, 4, 42);
    UiWork_SetAltFlagAndClearTable(1);
    listWin = UiWindow_Create(21, 9, 9, 11, 6);
    MenuCell_GetCursor(page, row, savedRow);
    unit = UnitGetFar(owner);

    for (x = 0; x <= 3; x++)
    {
        for (y = 0, pos = (u32 *)((cnt << 2) + (u32)table); y <= 19; y++)
        {
            if (unit->djinnActive[x] & (u32)(1 << y))
            {
                *pos = (u32)((x << 8) | y);
                pos++;
                cnt++;
            }
            else if (unit->djinnAvailable[x] & (u32)(1 << y))
            {
                u32 v;
                s32 k;
                list =
                    &((struct DjinnRecoveryTable *)Trade_GetOfferStateFar((u32)owner > 7 ? 1 : 0))
                         ->list;
                for (k = 0; k < list->count; k++)
                {
                    if (list->entries[k].unitId == owner && list->entries[k].element == x
                        && list->entries[k].index == y)
                    {
                        break;
                    }
                }
                v = (u32)((x << 8) | y | 0x10000);
                *pos = v;
                if (list->entries[k].turns > 0)
                {
                    v |= (u32)(list->entries[k].turns << 17);
                    *pos = v;
                }
                cnt++;
                pos++;
            }
        }
    }
    table[cnt] = 0x80000000;
    ctx->dirty = 1;

    /* FAKEMATCH: forced store: this dead store of curSpr before the loop keeps the native allocation; without it the function differs 394 (1193 instructions for 1201) in tbs-en and tbs-ja (measured 2026-10-14). */
    curSpr = &slot;
    for (;;)
    {
        if (page != prevPage || row != prevRow || redraw != 0)
        {
            cur = table[page + row];
            mask = 0;
            ctx->menuBusy = 1;
            Ui_SetRectHighlight(
                listWin->x + 1, listWin->y + prevRow * 2 + 1, listWin->width - 2, 1, 15);
            if (subSelection != 0)
            {
                UiWork_Finalize(msgWin, 1);
                msgWin = UiWindow_Create(0, 4, 30, 4, 42);
                Ui_FillVramBlockPattern();
            }
            redraw = 0;
            if (cnt != 0)
            {
                s32 k;
                u32 selected;
                UiWork_ClearValueNameTables();
                isUsed = 0;
                /* FAKEMATCH: this one-pass loop retains the native redraw
                 * reload after the selected-entry load. A plain assignment
                 * folds k to zero (1201 -> 1200 instructions, 27 rows differ);
                 * reading selected in the comparison moves the reload early
                 * (2 rows differ). No loop instructions survive. */
                selected = COUNTDOWN->selected[redraw];
                for (k = redraw; k == redraw;)
                {
                    k = redraw;
                    break;
                }
                if (selected == cur)
                {
                    isUsed = 1;
                }
                else
                {
                    for (;;)
                    {
                        /* FAKEMATCH: needed to match: isUsed is zero on every search-loop continuation. Its step dependency prevents loop.c from strength-reducing k into a pointer; CSE later removes the flag term. Plain k differs 35 instructions in EN; this form restores the native reload and indexing (differ 0). */
                        k += 1 - isUsed;
                        if (k > 7)
                            break;
                        if ((u32)COUNTDOWN->selected[k] == cur)
                        {
                            isUsed = 1;
                            break;
                        }
                    }
                }
                if (isUsed != 0)
                {
                    TEXT_LOAD(MsgBattleDjinnTwice, buf, TEXT_COUNT);
                    if (detailWin != 0)
                    {
                        UiWork_Finalize(detailWin, 1);
                        detailWin = 0;
                        subSelection = 0;
                    }
                }
                else if (cur & 0x10000)
                {
                    if (cur & 0x3e0000)
                    {
                        UiWork_PushValueSlot((s32)((cur & 0x3e0000) >> 17), 5);
                        TEXT_LOAD((s32)(((cur & 0xf00) >> 8) * 20 + (cur & 0xff)) + MsgDjinnEffect,
                            buf, TEXT_COUNT);
                        if (detailWin != 0)
                        {
                            UiWork_Finalize(detailWin, 1);
                            detailWin = 0;
                            subSelection = 0;
                        }
                    }
                    else
                    {
                        detailWin = (struct UiWindow *)DjinnMenu_ShowChangePreview(
                            (struct RenderInput *)detailWin, owner, (s32)cur, subSelection,
                            &subCnt);
                        UiText_CopyMessageString(MsgBattleSummonStandby, buf, TEXT_COUNT);
                        mask = 1 << ((cur & 0xf00) >> 8);
                    }
                }
                else
                {
                    detailWin = (struct UiWindow *)DjinnMenu_ShowChangePreview(
                        (struct RenderInput *)detailWin, owner, (s32)cur, subSelection, &subCnt);
                    TEXT_LOAD((s32)(((cur & 0xf00) >> 8) * 20 + (cur & 0xff)) + MsgDjinnEffect, buf,
                        TEXT_COUNT);
                }
            }
            else
            {
                TEXT_LOAD(MsgBattleNoDjinn, buf, TEXT_COUNT);
            }
            ctx->menuBusy = 0;
            if (subSelection == 0)
            {
                ctx->menuBusy = 1;
                UiWork_Finalize(msgWin, 1);
                msgWin = UiWindow_Create(0, 4, 30, 4, 42);
                ctx->menuBusy = (u8)subSelection;
            }
            UiText_RenderWideStringAtOffset(buf, msgWin, 0, 4);
            prevRow = row;
            if (page != prevPage)
            {
                RenderOutput_RedrawSavedRect(listWin);
                j = 0;
                while ((code = table[page + j]) != 0x80000000)
                {
                    UiWindow_SetTilemapEntry((struct RenderInput *)listWin,
                        (s32)((code & 0xf00) >> 8) + 0x5001, 0, j * 2, 0);
                    if (code & 0x3e0000)
                    {
                        UiWork_SetParamNibble(4);
                    }
                    else if (code & 0x10000)
                    {
                        UiWork_SetParamNibble(2);
                    }
                    UiText_DrawCharacterAtOffset(
                        (s32)(((code & 0xf00) >> 8) * 20 + (code & 0xff)) + MsgDjinnName,
                        (struct RenderInput *)listWin, 8, j * 16);
                    if (code & 0x3e0000)
                    {
                        UiText_DrawNumberAtOffset((s32)((code & 0x3e0000) >> 17), TURNS_DIGITS,
                            (struct RenderInput *)listWin, TURNS_X, j * 16);
                    }
                    UiWork_SetParamNibble(15);
                    j++;
                    if (j > 4)
                    {
                        break;
                    }
                }
                prevPage = page;
            }
            if (cnt > 5)
            {
                for (j = 0; j < (pages = ((cnt + 4) / 5)); j++)
                {
                    s32 tile = j + 0xf301;
                    if (j == (page / 5))
                    {
                        tile = j + 0xf30b;
                    }
                    UiWindow_SetTilemapEntry(
                        (struct RenderInput *)listWin, tile, listWin->width - pages + j - 2, -1, 0);
                }
            }
            Ui_SetRectHighlight(
                listWin->x + 1, listWin->y + row * 2 + 1, listWin->width - 2, 1, 14);
            ctx->dirty = 1;
            ctx->menuBusy = 0;
        }

        if (cnt > 5)
        {
            for (j = 0; j < (cnt + 4) / 5; j++)
            {
                s32 tile = j + 0xf301;
                if ((gKeysHeld & 0x100) || (gFrameCount & 15) <= 11)
                {
                    if (j == (page / 5))
                    {
                        tile = j + 0xf30b;
                    }
                }
                UiWindow_SetTilemapEntry((struct RenderInput *)listWin, tile,
                    listWin->width - ((cnt + 4) / 5) + j - 2, -1, 0);
            }
            if ((gKeysHeld & 0x100) == 0)
            {
                UiWindow_SetTilemapEntry((struct RenderInput *)listWin, 0xf334,
                    listWin->width - (cnt + 4) / 5 - 3, -1, 0);
                UiWindow_SetTilemapEntry(
                    (struct RenderInput *)listWin, 0xf335, listWin->width - 2, -1, 0);
            }
            else
            {
                UiWindow_SetTilemapEntry((struct RenderInput *)listWin, 0xf011,
                    listWin->width - (cnt + 4) / 5 - 3, -1, 0);
                UiWindow_SetTilemapEntry(
                    (struct RenderInput *)listWin, 0xf012, listWin->width - 2, -1, 0);
            }
            UiRender_MarkRow(ctx, listWin->y - 1);
        }

        trig = gKeyState;
        rep = gKeysRepeat;
        opts = gKeysHeld;
        menu = gLinkCountdownWork;
        if (menu->autoEnabled != 0)
        {
            rep = 0;
            trig = 0;
            opts = 0;
            if (menu->autoDelay == 0)
            {
                menu->autoDelay = 60;
                rep = 1;
                trig = 1;
            }
            else
            {
                menu->autoDelay--;
            }
        }
        menu = gLinkCountdownWork;
        if (menu->active == 0 || (trig & 2))
        {
            AudioCommand_PlayFar(0x71);
            code = -1;
            break;
        }
        if (trig & 1)
        {
            u32 selection;
            u32 turns;
            if (cnt != 0)
            {
                selection = table[page + row];
                turns = selection & 0x3e0000;
                if (turns == 0)
                {
                    if (isUsed == 0)
                    {
                        menu->page = page;
                        menu->row = row;
                        menu->preferredRow = savedRow;
                        code = selection;
                        break;
                    }
                }
                else if (isUsed == 0)
                {
                    Ui_FillVramBlockPattern();
                    UiWork_ClearValueNameTables();
                    UiWork_PushValueSlot((s32)((turns & 0x3e0000) >> 17), 5);
                    UiText_CopyMessageString(MsgBattleDjinnRecovering, buf, TEXT_COUNT);
                    UiText_RenderWideStringAtOffset(buf, msgWin, 0, 4);
                }
                AudioCommand_PlayFar(0x72);
            }
            else
            {
                code = -1;
                break;
            }
        }

        if (cnt != 0)
        {
            if (rep & 0x80)
            {
                AudioCommand_PlayFar(0x6f);
                row++;
                if (row == 5 || page + row == cnt)
                {
                    row = 0;
                }
                savedRow = row;
            }
            else if (rep & 0x40)
            {
                AudioCommand_PlayFar(0x6f);
                row--;
                if (row < 0)
                {
                    if (page == ((cnt - 1) / 5) * 5)
                    {
                        row = cnt - page - 1;
                    }
                    else
                    {
                        row = 4;
                    }
                }
                savedRow = row;
            }
            else if (opts & 0x100)
            {
                if (detailWin != 0)
                {
                    for (i = 0; i < subCnt; i++)
                    {
                        s32 tile = i + 0xf301;
                        if ((gFrameCount & 15) <= 11 && i == subSelection - 1)
                        {
                            tile = subSelection - 1 + 0xf30b;
                        }
                        UiWindow_SetTilemapEntry((struct RenderInput *)detailWin, tile,
                            detailWin->width - subCnt + i - 2, -1, 0);
                    }
                    UiWindow_SetTilemapEntry((struct RenderInput *)detailWin, 0xf334,
                        detailWin->width - subCnt - 3, -1, 0);
                    UiWindow_SetTilemapEntry(
                        (struct RenderInput *)detailWin, 0xf335, detailWin->width - 2, -1, 0);
                    UiRender_MarkRow(ctx, detailWin->y);
                }
                if (subSelection == 0 && subCnt != 0)
                {
                    if (detailWin != 0)
                    {
                        RenderOutput_ClearList(detailWin);
                    }
                    subSelection = 1;
                    redraw = 1;
                }
                else
                {
                    if (subSelection > subCnt)
                    {
                        subSelection = subCnt;
                    }
                    if (subSelection != 0)
                    {
                        if (rep & 16)
                        {
                            AudioCommand_PlayFar(0x6f);
                            subSelection++;
                            if (subSelection > subCnt)
                            {
                                subSelection = 1;
                            }
                            redraw = 1;
                        }
                        else if (rep & 32)
                        {
                            AudioCommand_PlayFar(0x6f);
                            subSelection--;
                            if (subSelection <= 0)
                            {
                                subSelection = subCnt;
                            }
                            redraw = 1;
                        }
                    }
                }
            }
            else if (subSelection != 0)
            {
                if (detailWin != 0)
                {
                    RenderOutput_ClearList(detailWin);
                }
                subSelection = 0;
                redraw = 1;
            }
            else if (rep & 16)
            {
                AudioCommand_PlayFar(0x6f);
                Runtime_SetMainState19();
                if (page + 5 >= cnt)
                {
                    if (page != 0)
                    {
                        page = 0;
                        row = savedRow;
                    }
                }
                else
                {
                    page += 5;
                    row = savedRow;
                    if (page == ((cnt - 1) / 5) * 5)
                    {
                        row = cnt - page - 1;
                        if (row > savedRow)
                        {
                            row = savedRow;
                        }
                    }
                }
            }
            else if (rep & 32)
            {
                AudioCommand_PlayFar(0x6f);
                Runtime_SetMainState19();
                if (page != 0)
                {
                    page -= 5;
                    row = savedRow;
                }
                else
                {
                    page = ((cnt - 1) / 5) * 5;
                    row = savedRow;
                    if (page != 0)
                    {
                        row = cnt - page - 1;
                        if (row > savedRow)
                        {
                            row = savedRow;
                        }
                    }
                }
            }
        }

        {
            s32 cx;

            /* FAKEMATCH: the one-pass calculation keeps row's reload after
             * the x shift. Without the block, mov r2, r10 fills the ldrh
             * delay slot before lsls (2 differing instruction rows). */
            do
            {
                cx = listWin->x * 8;
            } while (0);
            /* FAKEMATCH: instruction order: row read in a low register here is reloaded before the subtract, and row * 2 inherits that reload, so mov r2, r10 precedes subs r3, #2 as native (2 rows without; equal priority, and the reload otherwise has the later luid) */
            asm("" : : "l"(row));
            cursorPos.x = cx - 2;
        }
        cursorPos.y = (row * 2 + listWin->y) * 8 + 20;
        /* FAKEMATCH: forced re-point: curSpr already holds &slot; storing it again here keeps the native reload; without it the function differs 36 in tbs-en and tbs-ja (measured 2026-10-14). */
        curSpr = &slot;
        ((u32 *)curSpr)[1] = MENU_SPRITE_SIZE_16;
        ((u32 *)curSpr)[2] = 0;
        curSpr->tile = Resource_GetBuffer(res, (s32)gResource_FixedBlockBTiles);
        curSpr->x = cursorPos.x + ((gFrameCount & 4) >> 1) - 6;
        curSpr->y = cursorPos.y - ((gFrameCount & 4) >> 2) - 8;
        if (cnt != 0)
        {
            Runtime_PushSlotEntry(curSpr, 242);
        }
        cursor = CURSOR_OWNER;
        blink = gFrameCount & 4;
        iconWin = cursor->window;
        if (cursor->flags & 2)
        {
            for (i = 0; i <= 3; i++)
            {
                pal = 15 - (blink != 0);
                if (((1 << i) & mask) == 0)
                {
                    pal = 15;
                }
                Ui_SetRectHighlight(iconWin->x + gDjinnMenu_ElementXOffsets[i] + 1,
                    iconWin->y + gDjinnMenu_ElementYOffsets[i] + 1, 2, 2, pal);
            }
        }
        if (gFrameCount & 4)
        {
            Iwram_CopyWords((void *)0x06006500, gDjinnMenu_BlinkTile, 32);
            Iwram_CopyWords((void *)0x06006520, gDjinnMenu_BlinkTile, 32);
        }
        else
        {
            Iwram_FillWords((void *)0x06006500, 32, 0x44444444);
            Iwram_CopyWords((void *)0x06006520, gDjinnMenu_BlinkTile, 32);
        }
        WaitFrames(1);
    }

    if (CURSOR_OWNER->flags & 2)
    {
        Ui_SetRectHighlight(CURSOR_OWNER->window->x + 1, CURSOR_OWNER->window->y + 1, 4, 4, 15);
    }
    Resource_ResetEntry(res);
    UiWork_Finalize(msgWin, 1);
    UiWork_Finalize(detailWin, 1);
    UiWork_Finalize(listWin, 1);
    UiWindow_MarkVisibleTileAttributes();
    UiWork_SetAltFlagAndClearTable(0);
    MemFree(table);
    WaitFrames(1);
    return (s32)code;
}

//--------------------------------------------------------------------------
// classify_use_ability.c
// アイテム：召喚えらびのメニューと、戦闘でアイテムを使えるかの判定
//--------------------------------------------------------------------------

#define BATTLE_OK 0x80 // targetFlag：戦闘で使える

#if !GAME_TLA

#if EDITION_INTERNATIONAL
#define SUMMON_COST_X 13 // 必要ジン数の列
#else
#define SUMMON_COST_X 10
#endif

enum
{
    SUMMON_ELEMENT_TILE = 0x5001,       // 属性アイコン
    LIST_TILE_PAGE_NUMBER = 0xf301,     // ページ番号
    LIST_TILE_PAGE_NUMBER_LIT = 0xf30b, // 表示中のページ番号
    LIST_TILE_PAGES_LEFT = 0xf334,      // 左矢印
    LIST_TILE_PAGES_RIGHT = 0xf335      // 右矢印
};

#define SUMMON_END 32 // 召喚リストの終わり

//--------------------------------------------------------------------------
// 召喚えらび
// 覚えた召喚を 1 ページ 4 つずつ並べ、えらんだ召喚の番号を返す
// （キャンセルは -1）
// standby : 属性ごとのスタンバイ中のジンの数
//--------------------------------------------------------------------------
s32 SummonMenu_SelectSummon(s32 unused0, s32 unused1, const u8 *standby)
{
    struct CursorSprite            cursor;
    struct MenuSprite              sprites[4];
    u8                             available[33]; // 覚えた召喚
    u8                             ordered[33];   // 呼べるものを先にした並び
    u16                            text[64];
    s32                            handles[4];
    u8                             visible[4];
    s32                            glyph;
    s32                            count;
    struct UiWindow               *window;
    struct UiRenderWork           *render;
    s32                            drawnRow, drawnPage, row, page;
    s32                            cursorHandle;
    struct UiWindow               *description, *counts;
    s32                            mask, preferred;
    struct MenuPoint               cursorPosition;
    const struct SummonDefinition *summon;
    struct MenuCell               *nav;
    struct MenuSprite             *sprite;
    struct CursorSprite           *cur;
    s32                            used;
    s32                            id, tile, affordable;
    s32                            pressed, repeated, result;
    s32                            index;

    render = UI_WORK;
    drawnRow = -1;
    drawnPage = -1;
    cursorHandle = (s16)Resource_LoadIntoFreeSlot(128); // 16x16 スプライト 1 枚分
    description = UiWindow_Create(0, 4, 30, 4, 42);
    counts = UiWindow_Create(20, 8, 10, 3, 6);
    mask = 0;
    MenuCell_GetCursor(page, row, preferred);
#if EDITION_INTERNATIONAL
    window = UiWindow_Create(13, 11, 17, 9, 6);
#else
    window = UiWindow_Create(16, 11, 14, 9, 6);
#endif

    // 行ごとのアイコンのスプライト
    for (index = 0; index < 4; index++)
    {
        s32 y = index * 2;

        sprite = &sprites[index];
        MenuSprite_Init(sprites[index], MENU_SPRITE_SIZE_16);
        sprite->oam.f.x = window->x * 8 + 8;
        sprite->oam.f.y = (y + window->y) * 8 + 4;
    }
    for (index = 0; index < 4; index++)
    {
        s32 slot = Resource_LoadIntoFreeSlot(128);
        handles[index] = slot;
        sprites[index].oam.f.tile = Resource_GetBuffer(slot, -1);
    }

    // 今のスタンバイ数で呼べる召喚を後ろから順に先頭へ、残りをもとの順で後ろへ
    count = Trade_ListFlaggedEntriesFar(available);
    /* FAKEMATCH: instruction order only (block placement and schedule):
       with the first scan's exit inside this one-pass block, loop.c's
       find_and_verify_loops finds no barrier at depth 0 and leaves the
       main loop's select block in place, as the native has it (without
       the block it moves after the scan: differ 224, 6 branches fewer);
       inside it, count and used are stored before count - 1 is built
       (differ 2 with used = 0 before the block). */
    do
    {
        s32 i;

        used = 0;
        for (i = count - 1; i >= 0; i--)
        {
            const struct SummonDefinition *s;
            s32                            n, k;

            k = available[i];
            s = SummonDefinition_Get(k);
            for (n = 0; n <= 3; n++)
            {
                if (s->djinnRequired[n] > standby[n])
                    break;
            }
            if (n == 4)
            {
                ordered[used++] = k;
                available[i] = SUMMON_END;
            }
        }
    } while (0);
    {
        s32 i, k;

        for (i = 0; i < count; i++)
        {
            k = available[i];
            if (k != SUMMON_END)
                ordered[used++] = k;
        }
    }
    ordered[used] = SUMMON_END;

    for (;;)
    {
        cur = &cursor;

        //------------------------------------------------------------------
        // ページか行が変わったら描きなおす
        //------------------------------------------------------------------
        if (page != drawnPage || row != drawnRow)
        {
            render->menuBusy = 1;
            Ui_SetRectHighlight(
                window->x + 1, window->y + drawnRow * 2 + 1, window->width - 2, 1, 15);
            Ui_FillVramBlockPattern();

            // 説明文と、必要なジンの属性
            {
                const struct SummonDefinition *s;
                s32                            element;

                s = SummonDefinition_Get(ordered[page + row]);
#if EDITION_INTERNATIONAL
                UiText_CopyMessageString(s->nameMessageId + MsgAbilityDescription, text, 52);
#else
                UiText_DecodeMessage(s->nameMessageId + MsgAbilityDescription, text, 32);
#endif
                UiText_RenderWideStringAtOffset(text, (struct UiWindow *)description, 0, 4);
                drawnRow = row;
                mask = 0;
                for (element = 0; element <= 3; element++)
                {
                    if (s->djinnRequired[element] != 0)
                        mask |= 1 << element;
                }
            }

            // スタンバイ数と、ページの召喚 4 つ
            if (page != drawnPage)
            {
                RenderOutput_RedrawSavedRect(window);
                {
                    s32 element;

                    for (element = 0; element <= 3; element++)
                    {
                        UiWindow_SetTilemapEntry(
                            counts, element + SUMMON_ELEMENT_TILE, element * 2, 0, 0);
                        UiWindow_PutGlyph(counts, standby[element] + '0', element * 2 + 1, 0, 0);
                    }
                }
                {
                    s32 element, col, y;

                    index = 0;
                    if ((id = ordered[page + index]) != SUMMON_END)
                    {
                    next:
                        summon = SummonDefinition_Get(id);
                        col = 0;
                        for (element = 0; element <= 3; element++)
                        {
                            if (summon->djinnRequired[element] > standby[element])
                                break;
                        }
                        affordable = element == 4;
                        Ability_LoadGlyph(
                            summon->nameMessageId & 0x3fff, 0, &handles[index], &glyph, 1);
                        y = index * 2;
                        sprites[index].oam.f.tile = glyph;
                        if (!affordable)
                            UiWork_SetParamNibble(2); // 呼べない召喚は色 2
                        UiText_DrawCharacterAtOffset(
                            SummonDefinition_Get(id)->nameMessageId + MsgAbilityName,
                            (struct RenderInput *)window, 16, index * 16);
                        for (element = 0; element <= 3; element++)
                        {
                            if (summon->djinnRequired[element] != 0)
                            {
                                UiWindow_SetTilemapEntry(window, element + SUMMON_ELEMENT_TILE,
                                    col * 2 + SUMMON_COST_X, y, 0);
                                UiWindow_PutGlyph(window, summon->djinnRequired[element] + '0',
                                    col * 2 + SUMMON_COST_X + 1, y, 0);
                                col++;
                            }
                        }
                        UiWork_SetParamNibble(15);
                        visible[index++] = 1;
                        if (index <= 3 && (id = ordered[page + index]) != SUMMON_END)
                            goto next;
                    }
                    while (index <= 3)
                        visible[index++] = 0;
                }
                drawnPage = page;
            }

            // ページ番号
            if (count > 4)
            {
                for (index = 0; index < (count + 3) / 4; index++)
                {
                    tile = index + LIST_TILE_PAGE_NUMBER;
                    if (index == page / 4)
                        tile = index + LIST_TILE_PAGE_NUMBER_LIT;
                    UiWindow_SetTilemapEntry(
                        window, tile, window->width - (count + 3) / 4 + index - 2, -1, 0);
                }
            }
            Ui_SetRectHighlight(window->x + 1, window->y + row * 2 + 1, window->width - 2, 1, 14);
            render->dirty = 1;
            render->menuBusy = 0;
        }

        //------------------------------------------------------------------
        // 毎フレームの表示
        //------------------------------------------------------------------
        for (index = 0; index <= 3; index++)
        {
            if (visible[index] != 0)
                Runtime_PushSlotEntry((s32 *)&sprites[index], 240);
        }

        // カーソルは 4 フレームごとに揺れる
        cursorPosition.x = window->x * 8 - 2;
        cursorPosition.y = (row * 2 + window->y) * 8 + 20;
        ((u32 *)cur)[1] = MENU_SPRITE_SIZE_16;
        ((u32 *)cur)[2] = 0;
        cur->tile = Resource_GetBuffer((u16)cursorHandle, (s32)gResource_FixedBlockBTiles);
        cur->x = cursorPosition.x + ((gFrameCount & 4) >> 1) - 4;
        cur->y = cursorPosition.y - ((gFrameCount & 4) >> 2) - 8;
        /* FAKEMATCH: register choice and order: the cursor is pushed through
           sprite, re-pointed at it here as a struct MenuSprite *, which keeps
           the frame loop's registers as the native allocates them. Measured
           (2026-10-13): pushing &cursor or cur directly gives 947
           instructions for 946, differ 87 in all six editions. */
        sprite = (struct MenuSprite *)&cursor;
        Runtime_PushSlotEntry(sprite, 242);

        // 必要な属性のスタンバイ数を 8 フレームごとに点滅させる
        {
            s32 element, tile, blink;

            blink = gFrameCount & 8;
            for (element = 0; element <= 3; element++)
            {
                tile = 15 - (blink != 0);
                if ((1 << element & mask) == 0)
                    tile = 15;
                Ui_SetRectHighlight(counts->x + element * 2 + 1, counts->y + 1, 2, 1, tile);
            }
        }

        // ページ番号（表示中のものは点滅）と左右の矢印
        if (count > 4)
        {
            for (index = 0; index < (count + 3) / 4; index++)
            {
                tile = index + LIST_TILE_PAGE_NUMBER;
                if ((gFrameCount & 15) <= 11 && index == page / 4)
                    tile = index + LIST_TILE_PAGE_NUMBER_LIT;
                UiWindow_SetTilemapEntry(
                    window, tile, window->width - (count + 3) / 4 + index - 2, -1, 0);
            }
            UiWindow_SetTilemapEntry(
                window, LIST_TILE_PAGES_LEFT, window->width - (count + 3) / 4 - 3, -1, 0);
            UiWindow_SetTilemapEntry(window, LIST_TILE_PAGES_RIGHT, window->width - 2, -1, 0);
            UiRender_MarkRow(render, window->y - 1);
        }

        //------------------------------------------------------------------
        // 入力
        //------------------------------------------------------------------
        MenuCell_SetCursor(page, row, preferred);
        nav = gLinkCountdownWork;
        pressed = gKeyState;
        repeated = gKeysRepeat;

        // 自動入力中は 120 フレームごとに決定する
        if (nav->autoEnabled != 0)
        {
            repeated = 0;
            pressed = 0;
            if (nav->autoDelay == 0)
            {
                nav->autoDelay = 120;
                repeated = KEY_A;
                pressed = KEY_A;
            }
            else
            {
                nav->autoDelay--;
            }
        }
        if (pressed & KEY_A)
        {
            result = ordered[page + row];
            break;
        }
        if (gLinkCountdownWork->active == 0 || (pressed & KEY_B))
        {
            AudioCommand_PlayFar(SOUND_MENU_CANCEL);
            result = -1;
            break;
        }
        if (repeated & KEY_DOWN)
        {
            AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
            row++;
            if (row == 4 || page + row == count)
                row = 0;
            preferred = row;
        }
        else if (repeated & KEY_UP)
        {
            AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
            row--;
            if (row < 0)
            {
                if (page == ((count - 1) / 4) * 4)
                    row = count - page - 1; // 最後のページ
                else
                    row = 3;
            }
            preferred = row;
        }
        else if (repeated & KEY_RIGHT)
        {
            // 次のページ（最後からは先頭へ）
            AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
            Runtime_SetMainState19();
            if (page + 4 >= count)
            {
                if (page != 0)
                {
                    page = 0;
                    row = preferred;
                }
            }
            else
            {
                page += 4;
                row = preferred;
                if (page == ((count - 1) / 4) * 4)
                {
                    row = count - page - 1;
                    if (row > preferred)
                        row = preferred;
                }
            }
        }
        else if (repeated & KEY_LEFT)
        {
            // 前のページ（先頭からは最後へ）
            AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
            Runtime_SetMainState19();
            if (page != 0)
            {
                page -= 4;
                row = preferred;
            }
            else
            {
                page = ((count - 1) / 4) * 4;
                row = preferred;
                if (page != 0)
                {
                    row = count - page - 1;
                    if (row > preferred)
                        row = preferred;
                }
            }
        }
        WaitFrames(1);
    }

    // 後かたづけ
    WaitFrames(1);
    for (index = 0; index < 4; index++)
        Resource_ResetEntry(handles[index]);
    Resource_ResetEntry((u16)cursorHandle);
    UiWork_Finalize(counts, 1);
    UiWork_Finalize(description, 1);
    UiWork_Finalize(window, 1);
    WaitFrames(1);
    return result;
}
#endif

//--------------------------------------------------------------------------
// 戦闘でアイテムを使えるか
// 戻り値 : 0 使える / 1 使えない / 2 戦闘では使えない
// unit   : キャラクター番号
// item   : 持ち物の値
//--------------------------------------------------------------------------
s32 Item_ClassifyUseAbility(s32 unit, s32 item)
{
    s32                    ret;
    struct ItemDefinition *def;

    if (item == 0)
    {
        return 1;
    }
    def = Item_GetDirectFar(item);
    ret = 1;
    if (def->useType == 3) // エナジーを授ける装備
    {
        return ret;
    }
    if (def->actionId == 0)
    {
        return ret;
    }
    if ((def->type != 0) && (Item_CanOwnerEquip(unit, item) == 0))
    {
        return ret;
    }
    if ((BATTLE_OK & BattleActGet(def->actionId)->targetFlag) == 0)
    {
        return 2;
    }
    return 0;
}

void Vram_CopyTile(u32 source_tile, u32 destination_tile)
{
    Dma_Set((u8 *)0x06000000 + (source_tile & 0x3ff) * 32,
        (u8 *)0x06000000 + (destination_tile & 0x3ff) * 32, 0x84000008, REG_DMA3);
}

s32 ItemList_SelectEntry(s32 owner, u16 *entries, s32 count)
{
    struct UiRenderWork *render;
    s32                  drawnPage;
    s32                  drawnRow;
    s32                  cursorHandle;
    struct UiWindow     *messageWindow;
    s32                  iconCount;
    s32                  preferredRow;
    struct MenuPoint     pos;
    struct MenuCell     *cell;
    struct UiWindow     *window;
    s32                  page;
    s32                  row;
    s32                  entry;
    s32                  tile;
    s32                  edge;
    s32                  reason;
    s32                  result;
    s32                  i;
    struct CursorSprite  cursor;
    struct MenuSprite   *spr;
    struct CursorSprite *cur;
    u16                  buffer[64];
    s32                  handles[BATTLE_LIST_ROWS];
    struct MenuSprite    sprites[BATTLE_LIST_ROWS];

    render = UI_WORK;
    drawnPage = -1;
    drawnRow = -1;
    cursorHandle = Resource_LoadIntoFreeSlot(128);
    messageWindow = UiWindow_Create(0, 5, 30, 4, 42);
    iconCount = 0;
    MenuCell_GetCursor(page, row, preferredRow);
    window = UiWindow_Create(15, 9, 15, BATTLE_LIST_ROWS * 2 + 1, 6);

    for (i = 0; i < BATTLE_LIST_ROWS; i++)
    {
        s32 y = i * 2;

        spr = &sprites[i];
        MenuSprite_Init(sprites[i], MENU_SPRITE_SIZE_16);
        spr->oam.f.x = window->x * 8 + 8;
        spr->oam.f.y = (y + window->y) * 8 + 4;
    }

    for (i = 0; i < BATTLE_LIST_ROWS; i++)
    {
        handles[i] = Resource_LoadIntoFreeSlot(128);
        sprites[i].oam.f.tile = Resource_GetBuffer(handles[i], -1);
    }

    Vram_CopyTile(0xf018, 0x200);
    Vram_CopyTile(0xf018, 0x201);
    Vram_CopyTile(0xf019, 0x210);
    Vram_CopyTile(0xf019, 0x211);

    cur = &cursor;
    for (;;)
    {
        if (page != drawnPage || row != drawnRow)
        {
            render->menuBusy = 1;
            Ui_SetRectHighlight(
                window->x + 1, window->y + drawnRow * 2 + 1, window->width - 2, 1, 15);
            Ui_FillVramBlockPattern();
            if (count != 0)
            {
                if (Item_ClassifyUseAbility(owner, entries[page + row]) == 2)
                {
                    TEXT_LOAD(MsgBattleItemUnavailable, buffer, TEXT_COUNT);
                }
                else
                {
#if EDITION_INTERNATIONAL
                    UiText_CopyMessageString(
                        (entries[page + row] & ITEM_ID_MASK) + MsgItemPlainName, buffer, 52);
#else
                    UiText_DecodeMessage(
                        (entries[page + row] & ITEM_ID_MASK) + MsgItemPlainName, buffer, 32);
#endif
                }
            }
            else
            {
                TEXT_LOAD(MsgBattleNoItems, buffer, TEXT_COUNT);
            }
            UiText_RenderWideStringAtOffset(buffer, messageWindow, 0, 4);
            drawnRow = row;

            if (page != drawnPage)
            {
                RenderOutput_RedrawSavedRect(window);
                for (i = 0; i < BATTLE_LIST_ROWS; i++)
                {
                    entry = entries[page + i];
                    if (entry == 0)
                    {
                        break;
                    }
                    Item_GetDirectFar(entry);
                    UiWork_SetParamNibble(15);
                    if (Item_ClassifyUseAbility(owner, entry) != 0)
                    {
                        UiWork_SetParamNibble(4);
                    }
                    else if ((entry & INVENTORY_BROKEN) != 0)
                    {
                        UiWork_SetParamNibble(2);
                    }
                    UiText_DrawCharacterAtOffset((entry & ITEM_ID_MASK) + MsgItemName,
                        (struct RenderInput *)window, 16, i * 16);
                    UiWork_SetParamNibble(15);
                    sprites[i].oam.f.tile = Resource_LoadKind26EntryToBuffer(entry, handles[i]);
                }
                iconCount = i;
                drawnPage = page;
            }

            if (count > BATTLE_LIST_ROWS)
            {
                for (i = 0; i < (count + 4) / BATTLE_LIST_ROWS; i++)
                {
                    tile = i + LIST_TILE_PAGE_NUMBER;
                    if (i == page / BATTLE_LIST_ROWS)
                    {
                        tile = i + LIST_TILE_PAGE_NUMBER_LIT;
                    }
                    UiWindow_SetTilemapEntry(window, tile,
                        window->width - (count + 4) / BATTLE_LIST_ROWS + i - 2, -1, 0);
                }
            }

            Ui_SetRectHighlight(window->x + 1, window->y + row * 2 + 1, window->width - 2, 1, 14);
            render->dirty = 1;
            render->menuBusy = 0;
        }

        if (count > BATTLE_LIST_ROWS)
        {
            for (i = 0; i < (count + 4) / BATTLE_LIST_ROWS; i++)
            {
                tile = i + LIST_TILE_PAGE_NUMBER;
                if ((gFrameCount & 15) <= 11)
                {
                    if (i == page / BATTLE_LIST_ROWS)
                    {
                        tile = i + LIST_TILE_PAGE_NUMBER_LIT;
                    }
                }
                UiWindow_SetTilemapEntry(
                    window, tile, window->width - (count + 4) / BATTLE_LIST_ROWS + i - 2, -1, 0);
            }
            edge = -1;
            UiWindow_SetTilemapEntry(window, LIST_TILE_PAGES_LEFT,
                window->width - (count + 4) / BATTLE_LIST_ROWS - 3, edge, 0);
            UiWindow_SetTilemapEntry(window, LIST_TILE_PAGES_RIGHT, window->width - 2, edge, 0);
            UiRender_MarkRow(render, window->y - 1);
        }

        for (i = 0; i < iconCount; i++)
        {
            spr = &sprites[i];
            Runtime_PushSlotEntry(spr, 240);
        }

        pos.x = window->x * 8 - 2;
        pos.y = (row * 2 + window->y) * 8 + 20;
        /* FAKEMATCH: register choice (spill slots) only: re-pointing cur each
           frame orders the spill slots of the cursor address and row*2 as the
           native frame does. Measured (2026-10-13, after MenuCell_SetCursor):
           without it differ 56; with it but without the assignment before
           the loop, differ 14; the cursor written directly, with no cur,
           781 instructions for 770 (differ 209). Battle_SelectAbility sets
           curSpr the same two ways, untagged (394 and 36 without them). */
        cur = &cursor;
        ((u32 *)cur)[1] = MENU_SPRITE_SIZE_16;
        ((u32 *)cur)[2] = 0;
        cur->tile = Resource_GetBuffer(cursorHandle, (s32)gResource_FixedBlockBTiles);
        cur->x = pos.x + ((gFrameCount & 4) >> 1) - 4;
        cur->y = pos.y - ((gFrameCount & 4) >> 2) - 8;
        if (count != 0)
        {
            Runtime_PushSlotEntry(cur, 242);
        }

        MenuCell_SetCursor(page, row, preferredRow);
        cell = gLinkCountdownWork;

        if ((gKeyState & KEY_A) != 0)
        {
            if (count != 0)
            {
                result = page + row;
                Item_GetDirectFar(entries[result]);
                reason = 0;
                if ((entries[result] & INVENTORY_BROKEN) == 0)
                {
                    reason = Item_ClassifyUseAbility(owner, entries[result]);
                    if (reason == 0)
                    {
                        break;
                    }
                }
                AudioCommand_PlayFar(114);
                if (reason == 2)
                {
                    TEXT_LOAD(MsgBattleItemUnavailable, buffer, TEXT_COUNT);
                }
                else if ((entries[result] & INVENTORY_BROKEN) != 0)
                {
                    TEXT_LOAD(MsgBattleItemBroken, buffer, TEXT_COUNT);
                }
                else
                {
                    TEXT_LOAD(MsgBattleItemNoEffect, buffer, TEXT_COUNT);
                }
                Ui_FillVramBlockPattern();
                UiText_RenderWideStringAtOffset(buffer, messageWindow, 0, 4);
            }
            else
            {
                result = -1;
                break;
            }
        }
        else if (cell->active == 0 || (gKeyState & KEY_B) != 0)
        {
            AudioCommand_PlayFar(113);
            result = -1;
            break;
        }

        if (count != 0)
        {
            if ((gKeysRepeat & KEY_DOWN) != 0)
            {
                AudioCommand_PlayFar(111);
                row++;
                if (row == BATTLE_LIST_ROWS || page + row == count)
                {
                    row = 0;
                }
                preferredRow = row;
            }
            else if ((gKeysRepeat & KEY_UP) != 0)
            {
                AudioCommand_PlayFar(111);
                row--;
                if (row < 0)
                {
                    if (page == (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS)
                    {
                        row = count - page - 1;
                    }
                    else
                    {
                        row = BATTLE_LIST_ROWS - 1;
                    }
                }
                preferredRow = row;
            }
            else if ((gKeysRepeat & KEY_RIGHT) != 0)
            {
                AudioCommand_PlayFar(111);
                Runtime_SetMainState19();
                if (page + BATTLE_LIST_ROWS >= count)
                {
                    if (page != 0)
                    {
                        page = 0;
                        row = preferredRow;
                    }
                }
                else
                {
                    page += BATTLE_LIST_ROWS;
                    row = preferredRow;
                    if (page == (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS)
                    {
                        row = count - page - 1;
                        if (row > preferredRow)
                        {
                            row = preferredRow;
                        }
                    }
                }
            }
            else if ((gKeysRepeat & KEY_LEFT) != 0)
            {
                AudioCommand_PlayFar(111);
                Runtime_SetMainState19();
                if (page != 0)
                {
                    page -= BATTLE_LIST_ROWS;
                    row = preferredRow;
                }
                else
                {
                    page = (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS;
                    row = preferredRow;
                    if (page != 0)
                    {
                        row = count - page - 1;
                        if (row > preferredRow)
                        {
                            row = preferredRow;
                        }
                    }
                }
            }
        }
        WaitFrames(1);
    }

    UiWork_Finalize(messageWindow, 1);
    UiWork_Finalize(window, 1);
    WaitFrames(1);
    Resource_ResetEntry(cursorHandle);
    for (i = 0; i < BATTLE_LIST_ROWS; i++)
    {
        Resource_ResetEntry(handles[i]);
    }
    WaitFrames(1);
    return result;
}

s32 BattleMenu_RunActionSelection(s32 unitId, u16 *actions, s32 count)
{
    struct UiRenderWork *render;
    s32                  drawnPage;
    s32                  page;
    s32                  drawnRow;
    s32                  cursorHandle;
    struct BattleUnit   *unit;
    struct UiWindow     *messageWindow;
    s32                  iconCount;
    s32                  preferredRow;
    struct MenuPoint     pos;
    struct MenuCell     *nav;
    struct UiWindow     *window;
    s32                  row;
    s32                  entry;
    s32                  tile;
    s32                  edge;
    s32                  range;
    s32                  result;
    s32                  i;
    struct MenuSprite    cursor;
    struct MenuSprite   *spr;
    u16                  buffer[64];
    s32                  handles[BATTLE_LIST_ROWS];
    struct MenuSprite    sprites[BATTLE_LIST_ROWS];
    s32                  glyph;

    render = UI_WORK;
    drawnRow = -1;
    drawnPage = -1;
    cursorHandle = Resource_LoadIntoFreeSlot(128);
    unit = UnitGetFar(unitId);
    messageWindow = UiWindow_Create(0, 5, 30, 4, 42);
    iconCount = BATTLE_LIST_ROWS;
    MenuCell_GetCursor(page, row, preferredRow);
    window = UiWindow_Create(9, 9, 21, BATTLE_LIST_ROWS * 2 + 1, 6);

    for (i = 0; i < BATTLE_LIST_ROWS; i++)
    {
        s32 y = i * 2;

        spr = &sprites[i];
        MenuSprite_Init(sprites[i], MENU_SPRITE_SIZE_16);
        spr->oam.f.x = window->x * 8 + 8;
        spr->oam.f.y = (y + window->y) * 8 + 4;
    }

    for (i = 0; i < BATTLE_LIST_ROWS; i++)
    {
        handles[i] = Resource_LoadIntoFreeSlot(128);
        sprites[i].oam.f.tile = Resource_GetBuffer(handles[i], -1);
    }

    Vram_CopyTile(0xf018, 0x200);
    Vram_CopyTile(0xf018, 0x201);
    Vram_CopyTile(0xf019, 0x210);
    Vram_CopyTile(0xf019, 0x211);

    spr = &cursor;
    for (;;)
    {
        if (page != drawnPage || row != drawnRow)
        {
            render->menuBusy = 1;
            Ui_SetRectHighlight(
                window->x + 1, window->y + drawnRow * 2 + 1, window->width - 2, 1, 15);
            Ui_FillVramBlockPattern();
            if (count != 0)
            {
#if EDITION_INTERNATIONAL
                UiText_CopyMessageString(
                    (actions[page + row] & ACTION_ID_MASK) + MsgAbilityDescription, buffer, 52);
#else
                UiText_DecodeMessage(
                    (actions[page + row] & ACTION_ID_MASK) + MsgAbilityDescription, buffer, 32);
#endif
            }
            else
            {
                TEXT_LOAD(MSGNO(MsgBattleNoPsynergy), buffer, TEXT_COUNT);
            }
            UiText_RenderWideStringAtOffset(buffer, messageWindow, 0, 4);
            drawnRow = row;

            if (page != drawnPage)
            {
                RenderOutput_RedrawSavedRect(window);
                for (i = 0; i < BATTLE_LIST_ROWS; i++)
                {
                    struct BattleAct *action;

                    entry = actions[page + i];
                    if (entry == 0)
                    {
                        break;
                    }
                    action = BattleActGet(entry);
                    UiWindow_SetTilemapEntry(window, 0xf01f, 11, i * 2, 0);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
                    UiWindow_SetTilemapEntry(window, 0xf005, 12, i * 2, 0);
#else
                    UiWindow_SetTilemapEntry(window, 0xf01e, 12, i * 2, 0);
#endif
                    Ability_LoadGlyph(entry & ACTION_ID_MASK, 0, &handles[i], &glyph, 1);
                    sprites[i].oam.f.tile = glyph;

                    if ((action->targetFlag & ACTION_USABLE_IN_BATTLE) == 0)
                    {
                        UiWork_SetParamNibble(4);
                    }
                    else if (action->ppCost > unit->pp)
                    {
                        UiWork_SetParamNibble(2);
                    }
                    else if (unit->psySeal != 0)
                    {
                        UiWork_SetParamNibble(9);
                    }

                    render->level = 5;
                    UiText_DrawCharacterAtOffset(
                        entry + MsgAbilityName, (struct RenderInput *)window, 16, i * 16);
                    UiText_DrawNumberAtOffset(
                        action->ppCost, 2, (struct RenderInput *)window, 104, i * 16);
                    UiWork_SetParamNibble(15);
                    render->level = 15;

                    if (action->damageClass != 4)
                    {
                        UiWindow_SetTilemapEntry(
                            window, action->damageClass + 0x5001, 15, i * 2, 0);
                    }

                    range = action->range;
                    if (range == 255)
                    {
                        range = 11;
                    }
                    else
                    {
                        range--;
                    }
                    UiWindow_DrawThreeTileColumn((s32)window, 16, i * 2, range, 0);
                }
                iconCount = i;
                drawnPage = page;
            }

            if (count > BATTLE_LIST_ROWS)
            {
                for (i = 0; i < (count + 4) / BATTLE_LIST_ROWS; i++)
                {
                    tile = i + 0xf301;
                    if (i == page / BATTLE_LIST_ROWS)
                    {
                        tile = i + 0xf30b;
                    }
                    UiWindow_SetTilemapEntry(window, tile,
                        window->width - (count + 4) / BATTLE_LIST_ROWS + i - 2, -1, 0);
                }
            }

            Ui_SetRectHighlight(window->x + 1, window->y + row * 2 + 1, window->width - 2, 1, 14);
            render->dirty = 1;
            render->menuBusy = 0;
        }

        if (count > BATTLE_LIST_ROWS)
        {
            for (i = 0; i < (count + 4) / BATTLE_LIST_ROWS; i++)
            {
                tile = i + 0xf301;
                if ((gFrameCount & 15) <= 11)
                {
                    if (i == page / BATTLE_LIST_ROWS)
                    {
                        tile = i + 0xf30b;
                    }
                }
                UiWindow_SetTilemapEntry(
                    window, tile, window->width - (count + 4) / BATTLE_LIST_ROWS + i - 2, -1, 0);
            }
            edge = -1;
            UiWindow_SetTilemapEntry(
                window, 0xf334, window->width - (count + 4) / BATTLE_LIST_ROWS - 3, edge, 0);
            UiWindow_SetTilemapEntry(window, 0xf335, window->width - 2, edge, 0);
            UiRender_MarkRow(render, window->y - 1);
        }

        for (i = 0; i < iconCount; i++)
        {
            Runtime_PushSlotEntry(&sprites[i], 240);
        }

        pos.x = window->x * 8 - 4;
        pos.y = (row * 2 + window->y) * 8 + 20;
        ((u32 *)spr)[1] = 0x40000000;
        ((u32 *)spr)[2] = 0;
        spr->oam.f.tile = Resource_GetBuffer(cursorHandle, (s32)gResource_FixedBlockBTiles);
        spr->oam.f.x = pos.x + ((gFrameCount & 4) >> 1) - 4;
        spr->oam.f.y = pos.y - ((gFrameCount & 4) >> 2) - 8;
        if (count != 0)
        {
            Runtime_PushSlotEntry(spr, 242);
        }

        MenuCell_SetCursor(page, row, preferredRow);
        nav = gLinkCountdownWork;

        if ((gKeyState & KEY_A) != 0)
        {
            if (count != 0)
            {
                result = page + row;
                if ((BattleActGet(actions[result])->targetFlag & ACTION_USABLE_IN_BATTLE) != 0)
                {
                    break;
                }
                /* FAKEMATCH: a dead store here keeps the test above from being
                   threaded past the break. Measured (2026-10-13, after
                   MenuCell_SetCursor): without it the unusable branch skips the
                   native jump through the next block (793 instructions for 794,
                   differ 11 in every edition). */
                result = -1;
            }
            else
            {
                result = -1;
                break;
            }
        }
        else if (nav->active == 0 || (gKeyState & KEY_B) != 0)
        {
            AudioCommand_PlayFar(113);
            result = -1;
            break;
        }

        if (count != 0)
        {
            if ((gKeysRepeat & KEY_DOWN) != 0)
            {
                AudioCommand_PlayFar(111);
                row++;
                if (row == BATTLE_LIST_ROWS || page + row == count)
                {
                    row = 0;
                }
                preferredRow = row;
            }
            else if ((gKeysRepeat & KEY_UP) != 0)
            {
                AudioCommand_PlayFar(111);
                row--;
                if (row < 0)
                {
                    if (page == (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS)
                    {
                        row = count - page - 1;
                    }
                    else
                    {
                        row = BATTLE_LIST_ROWS - 1;
                    }
                }
                preferredRow = row;
            }
            else if ((gKeysRepeat & KEY_RIGHT) != 0)
            {
                AudioCommand_PlayFar(111);
                Runtime_SetMainState19();
                if (page + BATTLE_LIST_ROWS >= count)
                {
                    if (page != 0)
                    {
                        page = 0;
                        row = preferredRow;
                    }
                }
                else
                {
                    page += BATTLE_LIST_ROWS;
                    row = preferredRow;
                    if (page == (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS)
                    {
                        row = count - page - 1;
                        if (row > preferredRow)
                        {
                            row = preferredRow;
                        }
                    }
                }
            }
            else if ((gKeysRepeat & KEY_LEFT) != 0)
            {
                AudioCommand_PlayFar(111);
                Runtime_SetMainState19();
                if (page != 0)
                {
                    row = preferredRow;
                    page -= BATTLE_LIST_ROWS;
                }
                else
                {
                    page = (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS;
                    row = preferredRow;
                    if (page != 0)
                    {
                        row = count - page - 1;
                        if (row > preferredRow)
                        {
                            row = preferredRow;
                        }
                    }
                }
            }
        }
        WaitFrames(1);
    }

    UiWork_Finalize(messageWindow, 1);
    UiWork_Finalize(window, 1);
    WaitFrames(1);
    for (i = 0; i < BATTLE_LIST_ROWS; i++)
    {
        Resource_ResetEntry(handles[i]);
    }
    Resource_ResetEntry(cursorHandle);
    WaitFrames(1);
    return result;
}

struct CursorSlot
{
    u8 x;
    u8 y;
    u8 flags;
    s8 index;
};

s32 BattleTarget_RunSelection(s32 selection, s32 mode, u32 spread, u32 kind)
{
    u16                    ids[8];
    struct SelectionSprite entries[6];
    struct CursorSlot      table[6];
    struct EffectPosition  markPos;
    u16                    selectionIds[14];
    u8                     selectionSlot[8];
    struct EffectPosition  pos;
    u16                    name[15];
    struct EffectPosition  namePos;
    struct EffectPosition  targetPos;
    struct AffineTransform effect;
    struct BattleUnit     *unit;
    struct BattleWork     *runtime;
    struct UiWindow       *window;
    /* FAKEMATCH: the earlier combined pin/barrier removal gave EN 1585/1585, common 1343, differ 484: infoWin spills and the frame shrinks 324 -> 320. ES 1684/1684, common 1569, differ 230: infoWin/selection swap r10/r9 and spill slots permute. Both extents stayed unchanged. */
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
    struct UiWindow *infoWin;
#else
    /* FAKEMATCH: register choice only: as an ordinary local the info window is spilled and the frame shrinks to 320 bytes (differ 492); the native keeps it in r9. 2026-10-11: its live length is doubled by update_equiv_regs (the first set carries a constant REG_EQUAL), giving priority 2104 against &pos 2171 and the hoisted bit-field zero 2142; one duplicated no-effect block reaches 2143 against 2149 (differ 202), two push &pos below the zero (differ 449), and an initializer without the note sends it to r10 as in ES/IT (differ 96). */
    register struct UiWindow *infoWin asm("r9");
#endif
    s32                     cursor;
    s32                     cnt;
    s32                     total;
    s32                     redraw;
    s32                     slotId;
    s32                     pending;
    s32                     matrix;
    s32                     found;
    s32                     column;
    s32                     rows;
    s32                     count;
    s32                     i;
    s32                     j;
    s32                     n;
    s32                     nx;
    s32                     ny;
    s32                     pressed;
    s32                     y;
    s32                     width;
    s32                     repeat;
    s32                     result;
    struct CursorSlot      *slot;
    struct SelectionSprite *entry;
    struct SelectionSprite *head;
    struct BattleUnit      *who;

    runtime = gBattleWork;
    cnt = 0;
    redraw = 0xFFFF;
    slotId = Resource_LoadIntoFreeSlot(256);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
    /* FAKEMATCH: register choice only: this value-preserving null
     * initializer is needed to match infoWin in r10; a direct zero swaps
     * high registers and spill slots (2026-10-13: infoWin = 0 differs 94
     * in ES and IT). */
    infoWin = (struct UiWindow *)((u32)(u16)selection >> 16);
#else
    infoWin = 0;
#endif
    if (spread == 0)
        spread = 1;
    if (mode == 2 || mode == 4)
        ((struct MenuCell **)&gBattleWork)[48]->slideOffset = -2;
    else
        ((struct MenuCell **)&gBattleWork)[48]->slideOffset = 16;
    for (i = 0; i < 6; i++)
        table[i].flags = 0;
    cursor = -1;
    if (mode == 2)
    {
        for (i = 0; i <= 5 && runtime->partyUnits[i] != 0xFF; i++)
        {
            ids[cnt] = runtime->partyUnits[i];
            cnt++;
        }
    }
    else if (mode == 4)
    {
        ids[0] = (u16)selection;
        cnt = 1;
    }
    else
    {
        for (i = 0; i <= 5 && runtime->enemyIds[i] != 0xFF; i++)
        {
            ids[cnt] = runtime->enemyIds[i];
            cnt++;
        }
    }
    ids[cnt] = 0xFF;
    total = cnt;
    if (mode == 2)
    {
        if (spread != 0xFF && kind != 0)
        {
            found = 0;
            for (j = 0; j < cnt; j++)
            {
                struct BattleUnit *st;

                if (ids[j] == 0xFE)
                    continue;
                st = UnitGetFar(ids[j]);
                switch (kind)
                {
                case 5:
                    if (st->hp == 0)
                    {
                        selection = ids[j];
                        found = 1;
                    }
                    break;
                case 3:
                    if (st->poison != 0)
                        found = 1;
                    break;
                case 4:
                    if (st->delusion != 0 || st->stun != 0 || st->sleep != 0 || st->psySeal != 0
                        || st->grimReaper != 0)
                        found = 1;
                    break;
                case 6:
                    if (st->delusion != 0 || st->stun != 0 || st->sleep != 0 || st->psySeal != 0
                        || st->grimReaper != 0 || st->poison != 0 || st->haunt != 0)
                        found = 1;
                    break;
                }
                if (found != 0)
                {
                    selection = ids[j];
                    break;
                }
            }
        }
        for (j = 0; j < cnt; j++)
        {
            if (ids[j] == selection)
                break;
        }
        if (j != cnt)
            cursor = j;
    }
    if (cursor < 0)
        cursor = (cnt - 1) / 2;
    while (ids[cursor] == 0xFE
           || (GameFlag_TestFar(0x16C) && mode == 1 && UnitGetFar(ids[cursor])->hp == 0))
    {
        cursor = cursor + cnt - 1;
        cursor %= cnt;
    }
    if (mode != 2)
    {
        BattleMotion_ProjectConditionalPositionFar(selection, (s32 *)&markPos);
        table[0].flags = 8;
        table[0].x = (u8)markPos.x;
        table[0].y = 0x80;
    }
    window = UiWindow_Create(0, 12, 30, 4, 74);
    while (1)
    {
        head = entries;
        pending = 0;
        {
            s32 t = cursor * 2;

            /* FAKEMATCH: the separate switch arms end CSE before the
             * projection call, keeping the scaled offset reload ahead of
             * the ids load. Sharing their block reverses two rows; no
             * switch branch remains in any edition. Plain forms measured
             * (2026-10-13, EN and ES alike): ids[cursor] or a block-local
             * t = cursor * 2 reload ids before cursor * 2 (differ 2);
             * cursor[ids], *(cursor + ids) and (u8 *)ids + cursor * 2
             * differ 24; the call ahead of head and pending 12 to 24. */
            switch (t)
            {
            case 0:
                t = cursor * 2;
                break;
            default:
                t = cursor * 2;
                break;
            }
            BattleMotion_ProjectConditionalPositionFar(*(u16 *)((u8 *)ids + t), (s32 *)&pos);
        }
        ((s32 *)head)[1] = 0x40002000;
        ((s32 *)head)[2] = 0;
        head->tile = Resource_GetBuffer(
            slotId, (((gFrameCount >> 2) & 31) << 8) + (s32)gMenu_AnimatedCursorTiles);
        pos.y += Trig_Sin(gFrameCount << 12) / 32768;
        y = pos.y;
        if (table[0].flags & 1)
        {
            nx = (pos.x + table[0].x) / 2;
            ny = (y + table[0].y) / 2;
            if ((pos.x - nx < 0 ? -(pos.x - nx) : pos.x - nx) <= 7)
                pending = 1;
            pos.x = nx;
            table[0].x = (u8)nx;
            table[0].y = (u8)ny;
            pos.y = ny;
        }
        else if ((u8)table[0].flags <= 3)
        {
            table[0].x = (u8)pos.x;
            pos.y = y;
            table[0].y = (u8)y;
            table[0].flags = 1;
        }
        else
        {
            pos.x = table[0].x;
            pos.y = table[0].y;
            table[0].flags = (u8)(table[0].flags - 4);
            if (table[0].flags <= 3)
                table[0].flags = 1;
        }
        head->x = pos.x - 8;
        head->y = (u8)(pos.y - 16);
        Runtime_PushSlotEntry((s32 *)head, 240);
        if (spread == 0xFF)
        {
            effect.scaleX = 256;
            effect.scaleY = 256;
        }
        else
        {
            effect.scaleX = 176;
            effect.scaleY = 176;
        }
        effect.angle = 0;
#if !defined(TBS_EDITION_ES) && !defined(TBS_EDITION_IT)
        /* FAKEMATCH: seventeen dead stores restore the native frame's spill slots (table, entries, ids and cursor * 2 at sp+36 to sp+24, above &effect and selectionSlot); without them differ 102 in tbs-en, every row a spill offset. ES and IT match without them. */
        effect.angle = 0;
        effect.angle = 0;
        effect.angle = 0;
        effect.angle = 0;
        effect.angle = 0;
        effect.angle = 0;
        effect.angle = 0;
        effect.angle = 0;
        effect.angle = 0;
        effect.angle = 0;
        effect.angle = 0;
        effect.angle = 0;
        effect.angle = 0;
        effect.angle = 0;
        effect.angle = 0;
        effect.angle = 0;
        effect.angle = 0;
#endif
        matrix = AffineMatrix_BuildForEffect(&effect);
        if (redraw & 1)
        {
            cnt = 0;
            for (i = 0; i < 6; i++)
                table[i].flags &= (u8)~2;
            for (i = 0; (u32)i < spread; i++)
            {
                {
                    n = cursor + i;
                    if (n < total && ids[n] != 0xFE)
                    {
                        selectionIds[cnt] = ids[n];
                        table[i].flags |= 2;
                        if (table[i].index != i)
                        {
                            table[i].flags &= (u8)~1;
                            table[i].index = (s8)i;
                        }
                        selectionSlot[cnt] = (u8)i;
                        cnt++;
                    }
                    if (i != 0 && (n = cursor - i) >= 0 && ids[n] != 0xFE)
                    {
                        selectionIds[cnt] = ids[n];
                        table[6 - i].flags |= 2;
                        if (table[6 - i].index != -i)
                        {
                            table[6 - i].flags &= (u8)~1;
                            table[6 - i].index = (s8)-i;
                        }
                        selectionSlot[cnt] = (u8)(6 - i);
                        cnt++;
                    }
                }
            }
            for (i = 0; i < 6; i++)
            {
                if ((table[i].flags & 2) == 0)
                    table[i].index = 6;
            }
            selectionIds[cnt] = 0xFF;
            BattlePres_SetActorModesFar(selectionIds, 1);
            if (ids[cursor] <= 7)
            {
                if (spread != 0xFF && kind != 0)
                {
                    unit = UnitGetFar(ids[cursor]);
                    BattleMotion_ProjectConditionalPositionFar(ids[cursor], (s32 *)&pos);
                    if (infoWin != 0)
                        UiWork_Finalize(infoWin, 1);
                    switch (kind)
                    {
                    case 7:
                    {
                        s32 anchor;

                        anchor = pos.x / 8;
                        column = anchor - 4;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
                        if (System_GetLanguage() == 4)
                        {
                            if (anchor + 6 > 29)
                                column = 20;
                            infoWin = UiWindow_Create(column, 8, 10, 3, 6);
                        }
                        else if (System_GetLanguage() == 3)
                        {
                            if (anchor + 7 > 29)
                                column = 19;
                            infoWin = UiWindow_Create(column, 8, 11, 3, 6);
                        }
                        else
                        {
                            if (anchor + 5 > 29)
                                column = 21;
                            infoWin = UiWindow_Create(column, 8, 9, 3, 6);
                        }
#elif defined(TBS_EDITION_FR)
                        if (anchor + 5 > 29)
                            column = 21;
                        infoWin = UiWindow_Create(column, 8, 9, 3, 6);
#elif defined(TBS_EDITION_JA)
                        if (anchor + 4 > 29)
                            column = 22;
                        infoWin = UiWindow_Create(column, 8, 8, 3, 6);
#else
                        if (anchor + 4 > 29)
                            column = 22;
                        infoWin = UiWindow_Create(column, 8, 9, 3, 6);
#endif
                    }
                        UiWork_SetParamNibble(2);
                        UiText_DrawCharacterAtOffset(
                            MsgTargetSetDjinni, (struct RenderInput *)infoWin, 0, 0);
                        UiWork_SetParamNibble(15);
                        break;
                    case 1:
                    {
                        s32 anchor;

                        anchor = pos.x / 8;
                        column = anchor - 7;
                        if (anchor + 6 > 29)
                            column = 17;
                        infoWin = UiWindow_Create(column, 8, 13, 3, 6);
                    }
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
                        switch (System_GetLanguage())
                        {
                        case 1:
                            UiText_DrawStringAtOffset(
                                (u8 *)gOwnerStatus_KpString, (struct RenderInput *)infoWin, 0, 0);
                            break;
                        case 2:
                            UiText_DrawStringAtOffset(
                                (u8 *)gOwnerStatus_PvString, (struct RenderInput *)infoWin, 0, 0);
                            break;
                        case 3:
                            UiText_DrawStringAtOffset(
                                (u8 *)gOwnerStatus_PvString, (struct RenderInput *)infoWin, 0, 0);
                            break;
                        case 4:
                            UiText_DrawStringAtOffset(
                                (u8 *)gOwnerStatus_HpString, (struct RenderInput *)infoWin, 0, 0);
                            break;
                        }
#else
                        UiText_DrawStringAtOffset(
                            (u8 *)gOwnerStatus_HpString, (struct RenderInput *)infoWin, 0, 0);
#endif
                        UiText_DrawNumberInWindow(unit->hp, 4, (s32)infoWin, 16, 0);
                        UiText_DrawStringAtOffset(
                            (u8 *)gOwnerStatus_SlashString, (struct RenderInput *)infoWin, 48, 0);
                        UiText_DrawNumberInWindow(unit->maxHp, 4, (s32)infoWin, 56, 0);
                        break;
                    case 2:
                    {
                        s32 anchor;

                        anchor = pos.x / 8;
                        column = anchor - 7;
                        if (anchor + 6 > 29)
                            column = 17;
                        infoWin = UiWindow_Create(column, 8, 13, 3, 6);
                    }
                        UiText_DrawStringAtOffset(
                            (u8 *)gOwnerStatus_PpString, (struct RenderInput *)infoWin, 0, 0);
                        UiText_DrawNumberInWindow(unit->pp, 4, (s32)infoWin, 16, 0);
                        UiText_DrawStringAtOffset(
                            (u8 *)gOwnerStatus_SlashString, (struct RenderInput *)infoWin, 48, 0);
                        UiText_DrawNumberInWindow(unit->maxPp, 4, (s32)infoWin, 56, 0);
                        break;
                    case 5:
                    {
                        s32 anchor;

                        anchor = pos.x / 8;
                        column = anchor - 7;
                        if (anchor + 5 > 29)
                            column = 18;
                        infoWin = UiWindow_Create(column, 8, 12, 3, 6);
                    }
                        if (unit->hp != 0)
                            goto no_condition;
                        UiText_DrawCharacterAtOffset(
                            MsgTargetDowned, (struct RenderInput *)infoWin, 0, 0);
                        break;
                    case 3:
                    {
                        s32 anchor;

                        anchor = pos.x / 8;
                        column = anchor - 7;
                        if (anchor + 5 > 29)
                            column = 18;
                        infoWin = UiWindow_Create(column, 8, 12, 3, 6);
                    }
                        if (unit->poison == 0)
                            goto no_condition;
                        UiText_DrawCharacterAtOffset(
                            MsgTargetCurePoison, (struct RenderInput *)infoWin, 0, 0);
                        break;
                    case 4:
                        count = 0;
                        if (unit->delusion != 0)
                            count = 1;
                        if (unit->stun != 0)
                            count++;
                        if (unit->sleep != 0)
                            count++;
                        if (unit->psySeal != 0)
                            count++;
                        if (unit->grimReaper != 0)
                            count++;
                        if (count == 0)
                            count = 1;
                        rows = 9 - count;
                        if (rows <= 3)
                            rows = 4;
                        {
                            s32 anchor;

                            anchor = pos.x / 8;
                            column = anchor - 7;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
                            if (System_GetLanguage() == 3)
                            {
                                if (anchor + 10 > 29)
                                    column = 13;
                                infoWin = UiWindow_Create(column, rows, 17, count + 2, 6);
                            }
                            else
                            {
                                if (anchor + 9 > 29)
                                    column = 14;
                                infoWin = UiWindow_Create(column, rows, 16, count + 2, 6);
                            }
#elif defined(TBS_EDITION_JA)
                            if (anchor + 7 > 29)
                                column = 16;
                            infoWin = UiWindow_Create(column, rows, 14, count + 2, 6);
#else
                            if (anchor + 9 > 29)
                                column = 14;
                            infoWin = UiWindow_Create(column, rows, 16, count + 2, 6);
#endif
                        }
                        count = 0;
                        if (unit->delusion != 0)
                        {
                            UiText_DrawCharacterAtOffset(
                                MsgTargetCureDelusion, (struct RenderInput *)infoWin, 0, 0);
                            count = 1;
                        }
                        if (unit->stun != 0)
                        {
                            UiText_DrawCharacterAtOffset(
                                MsgTargetCureStun, (struct RenderInput *)infoWin, 0, count * 8);
                            count++;
                        }
                        if (unit->sleep != 0)
                        {
                            UiText_DrawCharacterAtOffset(
                                MsgTargetCureSleep, (struct RenderInput *)infoWin, 0, count * 8);
                            count++;
                        }
                        if (unit->psySeal != 0)
                        {
                            UiText_DrawCharacterAtOffset(
                                MsgTargetCureSeal, (struct RenderInput *)infoWin, 0, count * 8);
                            ++count;
                        }
                        if (unit->grimReaper != 0)
                        {
                            UiText_DrawCharacterAtOffset(
                                MsgTargetCureCurse, (struct RenderInput *)infoWin, 0, count * 8);
                            count++;
                        }
                        if (count == 0)
                            goto no_condition;
                        break;
                    default:
                        break;
                    case 6:
                        count = 0;
                        if (unit->poison != 0)
                            count = 1;
                        if (unit->delusion != 0)
                            count++;
                        if (unit->stun != 0)
                            count++;
                        if (unit->sleep != 0)
                            count++;
                        if (unit->psySeal != 0)
                            count++;
                        if (unit->grimReaper != 0)
                            count++;
                        if (unit->haunt != 0)
                            count++;
                        if (count == 0)
                            count = 1;
                        rows = 9 - count;
                        if (rows <= 3)
                            rows = 4;
                        {
                            s32 anchor;

                            anchor = pos.x / 8;
                            column = anchor - 7;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
                            if (System_GetLanguage() == 3)
                            {
                                if (anchor + 10 > 29)
                                    column = 13;
                                infoWin = UiWindow_Create(column, rows, 17, count + 2, 6);
                            }
                            else
                            {
                                if (anchor + 9 > 29)
                                    column = 14;
                                infoWin = UiWindow_Create(column, rows, 16, count + 2, 6);
                            }
#elif defined(TBS_EDITION_JA)
                            if (anchor + 7 > 29)
                                column = 16;
                            infoWin = UiWindow_Create(column, rows, 14, count + 2, 6);
#else
                            if (anchor + 9 > 29)
                                column = 14;
                            infoWin = UiWindow_Create(column, rows, 16, count + 2, 6);
#endif
                        }
                        count = 0;
                        if (unit->poison != 0)
                        {
                            UiText_DrawCharacterAtOffset(
                                MsgTargetCurePoison, (struct RenderInput *)infoWin, 0, 0);
                            count = 1;
                        }
                        if (unit->delusion != 0)
                        {
                            UiText_DrawCharacterAtOffset(
                                MsgTargetCureDelusion, (struct RenderInput *)infoWin, 0, count * 8);
                            count++;
                        }
                        if (unit->stun != 0)
                        {
                            UiText_DrawCharacterAtOffset(
                                MsgTargetCureStun, (struct RenderInput *)infoWin, 0, count * 8);
                            count++;
                        }
                        if (unit->sleep != 0)
                        {
                            UiText_DrawCharacterAtOffset(
                                MsgTargetCureSleep, (struct RenderInput *)infoWin, 0, count * 8);
                            count++;
                        }
                        if (unit->psySeal != 0)
                        {
                            UiText_DrawCharacterAtOffset(
                                MsgTargetCureSeal, (struct RenderInput *)infoWin, 0, count * 8);
                            count++;
                        }
                        if (unit->grimReaper != 0)
                        {
                            UiText_DrawCharacterAtOffset(
                                MsgTargetCureCurse, (struct RenderInput *)infoWin, 0, count * 8);
                            count++;
                        }
                        if (unit->haunt != 0)
                        {
                            UiText_DrawCharacterAtOffset(
                                MsgTargetCureHaunt, (struct RenderInput *)infoWin, 0, 8 * count);
                            count++;
                        }
                        if (count != 0)
                            break;
                    no_condition:
                        UiWork_SetParamNibble(2);
                        UiText_DrawCharacterAtOffset(
                            MsgTargetNoEffect, (struct RenderInput *)infoWin, 0, 0);
                        UiWork_SetParamNibble(15);
                        break;
                    }
                }
            }
            else if (spread != 0xFF)
            {
                who = UnitGetFar(ids[cursor]);
                BattleMotion_ProjectConditionalPositionFar(ids[cursor], (s32 *)&namePos);
                namePos.y += Trig_Sin(gFrameCount << 12) / 32768;
                if (who->classId == 125 || who->classId == 122)
                {
                    width = MsgMysteryPersonName;
                    if (who->classId == 125)
                        width++;
#if defined(TBS_EDITION_JA)
                    UiText_DecodeMessage(width, name, 10);
#else
                    UiText_CopyMessageString(width, name, 14);
#endif
                }
                else
                {
                    for (i = 0; i <= 13; i++)
                    {
                        name[i] = n = who->name[i];
                        if (!n)
                            break;
                    }
                    name[i] = 0;
                }
                width = UiText_GetWideStringWidth(name);
                namePos.x -= width / 2;
                namePos.x -= 8;
                if (namePos.x + width > 224)
                    namePos.x = 224 - width;
                if (namePos.x < 0)
                    namePos.x = 0;
                Ui_ClearVramBlock();
                UiText_RenderWideStringAtOffset(name, window, namePos.x, 4);
            }
            redraw &= ~1;
        }
        if (pending != 0)
        {
            for (i = 1; i < cnt; i++)
            {
                entry = &head[i];
                slot = &table[selectionSlot[i]];
                BattleMotion_ProjectConditionalPositionFar(selectionIds[i], (s32 *)&targetPos);
                targetPos.y = targetPos.y + Trig_Sin(gFrameCount << 12) / 32768;
                *entry = *head;
                if (slot->flags & 1)
                {
                    s32 x = (targetPos.x + slot->x) / 2;
                    s32 y = (targetPos.y + slot->y) / 2;

                    targetPos.x = x;
                    slot->x = x;
                    targetPos.y = y;
                    slot->y = y;
                }
                else
                {
                    s32 x = entry->x;
                    s32 y = entry->y + 8;

                    slot->flags = 1;
                    targetPos.x = x;
                    slot->x = x;
                    targetPos.y = y;
                    slot->y = y;
                }
                entry->mode = 1;
                entry->x = targetPos.x - 8;
                entry->y = targetPos.y - 12;
                if (spread == 0xFF)
                    entry->affine = 0;
                else
                    entry->affine = 1;
                entry->param = matrix;
                Runtime_PushSlotEntry((s32 *)entry, 240);
            }
        }
        pressed = gKeyState;
        repeat = gKeysRepeat;
        if (gLinkCountdownWork->autoEnabled)
        {
            repeat = 0;
            pressed = 0;
            if (gLinkCountdownWork->autoDelay == 0)
            {
                gLinkCountdownWork->autoDelay = 60;
                repeat = 1;
                pressed = 1;
            }
            else
            {
                gLinkCountdownWork->autoDelay--;
            }
        }
        if (pressed & 1)
        {
            s32 id;
            s32 k;

            id = ids[cursor];
            redraw = 0;
            result = -1;
            for (k = 0; k <= 5 && runtime->partyUnits[k] != 0xFF; k++)
            {
                if (runtime->partyUnits[k] == id)
                {
                    result = 0x100 | k;
                    break;
                }
            }
            if (result < 0)
            {
                for (k = 0; k <= 5 && runtime->enemyIds[k] != 0xFF; k++)
                {
                    if (runtime->enemyIds[k] == id)
                    {
                        result = 0x180 | k;
                        break;
                    }
                }
            }
            cursor = result;
        }
        else if (spread != 0xFF)
        {
            if (repeat & 0x90)
            {
                AudioCommand_PlayFar(111);
                do
                {
                    cursor++;
                    cursor %= total;
                } while (ids[cursor] == 0xFE);
                redraw |= 1;
            }
            if (repeat & 0x60)
            {
                AudioCommand_PlayFar(111);
                do
                {
                    cursor = cursor + total - 1;
                    cursor %= total;
                } while (ids[cursor] == 0xFE);
                redraw |= 1;
            }
        }
        if (gLinkCountdownWork->active == 0 || (pressed & 2))
        {
            AudioCommand_PlayFar(113);
            cursor = -1;
            break;
        }
        WaitFrames(1);
        if (redraw == 0)
            break;
    }
    WaitFrames(1);
    Resource_ResetEntry(slotId);
    if (infoWin != 0)
        UiWork_Finalize(infoWin, 1);
    UiWork_Finalize(window, 1);
    BattlePres_SetActorModesFar(ids, 0);
    gLinkCountdownWork->slideOffset = 0;
    WaitFrames(1);
    return cursor;
}

#define COUNTDOWN_START_FRAMES 900
#define FRAMES_PER_SECOND 60

struct LinkSignature
{
    u8  padding000[8];
    u16 e;
    u16 d;
    u16 v;
    u16 s;
    u8  padding010[8];
};

void UpdateLinkSessionCountdown(void)
{
    struct LinkRoundState *state = COUNTDOWN;
    struct BattleWork     *runtime;
    struct MenuSprite     *entry;
    u8                    *active;
    struct LinkSignature  *signature;
    s32                    target;
    s32                    current;
    s32                    difference;
    s32                    step;
    s32                    newOffset;
    s32                    timer;
    s32                    nextTimer;
    s32                    seconds;
    s32                    i;

    if (state != 0)
    {
        target = state->targetOffset;
        current = state->currentOffset;
        if (target != current)
        {
            difference = target - current;
            step = difference / 3;
            if (step == 0)
            {
                step--;
                if (difference >= 0)
                    step = 1;
            }
            newOffset = current + step;
            state->currentOffset = newOffset;
            Camera_ConfigureSceneFar(newOffset);
        }

        entry = state->entries;
        active = state->entryActive;
        i = 2;
        do
        {
            if (*active++ != 0)
                Runtime_PushSlotEntry(entry, 240);
            i--;
            entry++;
        } while (i >= 0);
        Link_DrawShiftedTilePair(0x06006680);

        if (state->enabled == 0)
            return;

        runtime = gBattleWork;
        if (runtime->linkPaused == 0)
            goto load_timer;
        state->timer = 0;
        return;

    signature_tail:
        if (signature->v != 'V')
            goto signature_done;
        if (signature->s != 'S')
            goto signature_done;
        state->timer = COUNTDOWN_START_FRAMES;
        timer = state->timer;
        goto signature_done;

    load_timer:
        timer = state->timer;
        if (timer >= 0)
            goto timer_ready;
        signature = (struct LinkSignature *)gLinkPeerSignatures[runtime->linkSide ^ 1];
        if (signature->e != 'E')
            goto signature_done;
        if (signature->d == 'D')
            goto signature_tail;

    signature_done:
        if (timer < 0)
            return;

    timer_ready:
        if (state->displayHandle == 0 && state->secondaryHandle == 0)
        {
            state->displayHandle = Link_CreateCountdownLabelWindow();
            timer = state->timer;
        }

        if (timer > 0)
        {
            nextTimer = timer - 1;
            state->timer = nextTimer;
            timer = nextTimer;
        }
        if (timer < 0)
            return;

        seconds = (timer + FRAMES_PER_SECOND - 1) / FRAMES_PER_SECOND;
        if (seconds != 0 && seconds * FRAMES_PER_SECOND == timer)
            AudioCommand_PlayFar(SOUND_TRIPLE_TONE_LOW);

        if (state->displayHandle != 0)
            UiText_DrawNumberInWindow(seconds, 2, state->displayHandle, 16, 8);
    }
}

s32 UiText_ShowLocalizedMessageAndWait(void)
{
    s16                buffer[64];
    s32                work;
    s32                result;
    struct BattleWork *battle;
    s32                mode;

    battle = gBattleWork;
    mode = gGameState.battleStart;
    if (gGameState.battleStart == 2 || mode <= 2 || (result = 1, mode > 4))
    {
        result = 0;
    }
    if (result == 0)
    {
        if ((s8)battle->challengeFlag != 0)
        {
            result = 1;
        }
    }
    if (result != 0)
    {
        work = UiWindow_Create(0, 7, 30, 4, 42);
        Ui_FillVramBlockPattern();
        TEXT_LOAD(MsgNoTimeToRun, buffer, TEXT_COUNT);
        UiText_RenderWideStringAtOffset(buffer, work, 0, 4);
        do
        {
            WaitFrames(1);
        } while ((gKeyState & (KEY_A | KEY_B)) == 0 && COUNTDOWN->timer != 0);
        UiWork_Finalize(work, 1);
    }
    return result;
}

//--------------------------------------------------------------------------
// classify_entry_kind.c
// バトル：わざの対象選びの種類
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// わざの効果から、対象選びで使う種類を返す
// 0 その他、1 HPかいふく、2 PPかいふく、3 毒がきえる、4 状態異常をなおす、
// 5 ふっき、6 状態異常をすべてなおす
// act : わざのデータ
//--------------------------------------------------------------------------
s32 Battle_ClassifyEntryKind(const struct BattleAct *act)
{
    s32 val;
    s32 type;
    s32 kind;
    u8  effect;

    kind = 0;
    type = act->targetFlag & 15;
    if (type == 1) // HPかいふく
        kind = 1;
    if (type == 11) // PPかいふく
        kind = 2;
    val = (s8)act->effect;
    effect = val;
    if (effect == 3) // 毒がきえる
        kind = 3;
    if (effect == 4) // 状態異常をなおす
        kind = 4;
    if (effect == 64) // 状態異常をすべてなおす
        kind = 6;
    if (BattleEffectRevCheckFar(act->effect) != 0) // ふっき
        kind = 5;
    return kind;
}

s32 Battle_CollectPartyCommands(struct BattleActionRecord *out, u16 *in, s32 count)
{
    struct LinkRoundState     *state;
    struct BattleWork         *link;
    struct BattleWork         *session;
    u16                       *peer;
    u16                       *sync;
    struct BattleActionRecord *ent;
    struct BattleUnit         *actor;
    struct UiWindow           *win;
    struct MenuSprite         *slot;
    u16                        hdr[2];
    s32                        aux;
    s32                        actorId;
    s32                        mode;
    s32                        kind;
    s32                        param;
    s32                        res1024;
    s32                        res512;
    s32                        miss;
    s32                        i;
    s32                        k;
    s32                        order;

    void DrawPartyPanels(void)
    {
        u16 data[2];
        data[0] = 0xff;
        BattleLayout_HighlightPartyPanels(data);
        BattlePres_SetActorModesFar(data, 1);
    }

    aux = 256;
    kind = 0;
    param = 0;
    res1024 = Resource_LoadIntoFreeSlot(1024);
    res512 = Resource_LoadIntoFreeSlot(512);
    state = Runtime_AllocateBlock(57, 260);
    Resource_LoadIndexedIntoBuffer(res1024, 0);
    Graphics_ExpandVramTilesByColorTable(0x06006000);
    Link_DrawShiftedTilePair(0x06006680);
    Menu_BuildLocalizedPatternTiles();
    link = gBattleWork;
    for (k = 0; k < 8; k++)
    {
        state->selected[k] = 0x80000000;
    }
    for (k = 0; k < 3; k++)
    {
        state->entryActive[k] = 0;
    }
    state->targetOffset = 0;
    state->currentOffset = 0;
    state->timer = -1;
    state->unk3c = 0;
    state->unk40 = 0;
    state->enabled = 0;
    state->secondaryHandle = 0;
    state->displayHandle = 0;
    if (link->twoSided != 0)
    {
        state->enabled = 1;
        sync = (u16 *)gSerialTransfer.payload;
        sync[4] = 'V';
        sync[5] = 'S';
        sync[6] = 'S';
        sync[7] = 'T';
        miss = 0;
        for (;;)
        {
            peer = (u16 *)&gLinkPeerSignatures[link->linkSide ^ 1];
            if (link->linkPaused != 0)
            {
                count = -1;
                state->enabled = 0;
                break;
            }
            if ((gLinkStatus & 3) != 3)
            {
                if (++miss > 24)
                {
                    count = -1;
                    state->enabled = 0;
                    break;
                }
            }
            else
            {
                miss = 0;
                if (peer[4] == 'V' && peer[5] == 'S' && peer[6] == 'S' && peer[7] == 'T')
                {
                    break;
                }
                if (peer[4] == 'E' && peer[5] == 'D' && peer[6] == 'V' && peer[7] == 'S')
                {
                    break;
                }
                if (!(peer[0] == 'E' && peer[1] == 'X' && peer[2] == 'E' && peer[3] == 'C'))
                {
                    count = -1;
                    state->enabled = 0;
                    break;
                }
            }
            WaitFrames(1);
        }
    }
    Scheduler_AddOrUpdateCallback(UpdateLinkSessionCountdown, 3200);

    while (1)
    {
        DrawPartyPanels();
        state->entryActive[2] = 0;
        state->actorId = -1;
        state->hintCount = 0;
        if (GameFlag_TestFar(366) != 0)
        {
            state->hintCount = 1;
            state->hintTimer = 60;
        }
        if (count > 0)
        {
            mode = Ui_RunSelectionScreen(0);
        }
        else
        {
            mode = 14;
        }
        if (mode == 7)
        {
            void *block;
            s32   selection;
            s32   handle;

            block = MemAlloc(12);
            if (gDebugMode != 0 && (gKeysHeld & KEY_START) != 0)
            {
                selection = 2;
            }
            else
            {
                selection = 1;
            }
            handle = BattleParty_ListActorIdsFar(selection, block);
            WaitFrames(1);
            Ui_RunOwnerStatusScreen(block, handle, *(u16 *)block);
            MemFree(block);
            continue;
        }
        if (mode == 4)
        {
            if (UiText_ShowLocalizedMessageAndWait() != 0)
            {
                continue;
            }
            count = 1;
            ent = out;
            ent->unitId = in[0];
            ent->priority = 0x7ffe;
            ent->command = 99;
            ent->parameter = 0;
            ent->target = 256;
            break;
        }
        if (mode != 14)
        {
            break;
        }
        AudioCommand_PlayFar(154);
        i = 0;
        if (i >= count)
        {
            break;
        }

        for (;;)
        {
            if (i == 0)
            {
                BattlePlacement_CountValidEntriesFar(0, state->cost[0]);
            }
            else
            {
                s32 k;

                for (k = 0; k < 4; k++)
                {
                    state->cost[i][k] = state->cost[i - 1][k];
                }
            }

            ent = &out[i];
            actorId = in[i];
            /* FAKEMATCH: instruction order only: called through a cast as
               returning a doubleword, the call also sets r1, so the r1
               reload for classId no longer depends on the in reload and
               sched2 loads i * 2 before in, as native (by its prototype,
               differ 2). */
            actor = (struct BattleUnit *)(s32)((s64 (*)(s32))UnitGetFar)(actorId);
            COUNTDOWN->actorId = actorId;
            COUNTDOWN->unk40 = 0;
            slot = &COUNTDOWN->entries[2];
            slot->oam.word.attr01 = 0x80000400;
            slot->oam.word.attr23 = 0;
            slot->oam.f.tile = Ui_LoadEntryForKind(actor->classId, res512);
#if EDITION_INTERNATIONAL
            slot->oam.f.x = 0;
#else
            slot->oam.f.x = 4;
#endif
            slot->oam.f.y = 0x80;
            slot->oam.f.palette = 14;
            state->entryActive[2] = 1;

        mark_visible:
            UiWindow_MarkVisibleTileAttributes();
            COUNTDOWN->entryActive[0] = 0;
            COUNTDOWN->selected[i] = 0x80000000;
            /* FAKEMATCH: twelve dead stores restore the native frame's spill slots (i << 1, i << 2 and i << 4 at sp+32 to sp+24, above &hdr at sp+20); without them differ 50 in every edition, every row a spill offset. */
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = (u16)actorId;
            hdr[1] = 255;
            BattlePres_SetActorModesFar(hdr, 1);
            BattleLayout_HighlightPartyPanels(hdr);
            WaitFrames(1);
            mode = Ui_RunSelectionScreen(1);
            WaitFrames(1);
            if (mode == -2)
            {
                void *block;
                s32   handle;

                block = MemAlloc(12);
                handle = BattleParty_ListActorIdsFar(1, block);
                state->entryActive[2] = 0;
                WaitFrames(1);
                Ui_RunOwnerStatusScreen(block, handle, actorId);
                state->entryActive[2] = 1;
                MemFree(block);
                continue;
            }
            BattlePres_SetActorModesFar(hdr, 0);
            if (mode == -1)
            {
                if (i == 0)
                {
                    break;
                }
                i--;
                continue;
            }

            if (COUNTDOWN->timer == 0)
            {
                mode = 3;
            }
            slot = &COUNTDOWN->entries[0];
            slot->oam.word.attr23 = 0;
            slot->oam.word.attr01 = 0x80002400;
            slot->oam.f.tile = Resource_LoadIndexedIntoBuffer(res1024, mode);
            if (mode == 15)
            {
                slot->oam.f.x = 128;
            }
            else
            {
                slot->oam.f.x = 96;
            }
            slot->oam.f.y = 136;
            state->entryActive[0] = 1;
            switch (mode)
            {
            case 0:
                win = UiWindow_Create(11, 17, 11, 3, 6);
#if defined(TBS_EDITION_FR)
                UiText_DrawCharacterAtOffset(MsgCommandName, (struct RenderInput *)win, 14, 0);
#else
                UiText_DrawCharacterAtOffset(MsgCommandName, (struct RenderInput *)win, 16, 0);
#endif
                state->entries[0].oam.f.x = 64;
                AudioCommand_PlayFar(112);
                mode = BattleTarget_RunSelection(actorId, 1, 1, 0);
                UiWork_Finalize(win, 1);
                if (mode == -1)
                {
                    goto mark_visible;
                }
                kind = 0;
                aux = mode;
                ent->range = 1;
                break;

            case 1:
            {
                struct MenuSprite   *slot;
                struct BattleAct    *ability;
                struct UiRenderWork *ui;
                u16                 *list;
                u8                  *pick;
                s32                  cost;
                s32                  handle;
                s32                  cnt;
                s32                  k;
                s32                  cur;

                AudioCommand_PlayFar(112);
                mode = 0;
                MenuCell_SetCursor(mode, mode, mode);
            psynergy_menu:
                list = state->list;
                pick = state->pick;
                state->entries[0].oam.f.x = 48;
                cnt = 0;
                k = 0;
                cur = actor->actionSlots[k].encodedAction & ACTION_ID_MASK;
                if (cur != 0)
                {
                    do
                    {
                        if ((BattleActGet(cur)->targetFlag & 0x80) != 0)
                        {
                            pick[cnt] = k;
                            list[cnt] = cur;
                            cnt++;
                        }
                        k++;
                        if (k == 32)
                        {
                            break;
                        }
                        cur = actor->actionSlots[k].encodedAction & ACTION_ID_MASK;
                    } while (cur != 0);
                }
                pick[cnt] = 0;
                list[cnt] = 0;
                mode = BattleMenu_RunActionSelection(actorId, list, cnt);
                if (mode == -1)
                {
                    goto mark_visible;
                }
                param = actor->actionSlots[pick[mode]].encodedAction & ACTION_ID_MASK;
                ability = BattleActGet(param);
                cost = ability->range;
                slot = &state->entries[1];
                handle = Resource_LoadIntoFreeSlot(128);
                ui = UI_WORK;
                win = UiWindow_Create(8, 17, 18, 3, 6);
                state->entries[0].oam.f.x = 40;
                slot->oam.word.attr01 = 0x40000400;
                slot->oam.word.attr23 = 0;
                slot->oam.f.x = win->x * 8 + 8;
                slot->oam.f.y = win->y * 8 + 4;
                /* FAKEMATCH: instruction order only (pool placement): arm_reorg counts this empty asm as a 4-byte insn, so the pool goes in after mov r1, r10 as native, whose constant 40 acts as a range-60 (zero_extendhisi2) pool load (differ 14). 2026-10-11: arm_reorg runs before shorten_branches and counts the empty asm at its 4-byte default length while it emits nothing; the halfword constant fix needs that much more estimated length after it, and no C statement or single pattern-length change (strh, str, orr, and tested over all 413 English objects) supplies it. */
                __asm__ volatile("");
                slot->oam.f.tile = Resource_LoadIndexedEntryToBuffer(param, handle);
                state->entryActive[1] = 1;
                ui->level = 5;
                if ((s32)ability->ppCost > (s32)actor->pp)
                {
                    UiWork_SetParamNibble(2);
                }
                else if (actor->psySeal != 0)
                {
                    UiWork_SetParamNibble(9);
                }
                UiText_DrawCharacterAtOffset(
                    param + MsgAbilityName, (struct RenderInput *)win, 16, 0);
                UiText_DrawNumberAtOffset(ability->ppCost, 2, (struct RenderInput *)win, 104, 0);
                ui->level = 15;
                UiWork_SetParamNibble(15);
                UiWindow_SetTilemapEntry(win, 61471, 11, 0, 0);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
                UiWindow_SetTilemapEntry(win, 61445, 12, 0, 0);
#else
                UiWindow_SetTilemapEntry(win, 61470, 12, 0, 0);
#endif
                if (ability->damageClass != 4)
                {
                    UiWindow_SetTilemapEntry(win, ability->damageClass + 20481, 15, 0, 0);
                }
                ent->range = cost;
                AudioCommand_PlayFar(112);
                mode = BattleTarget_RunSelection(actorId, ability->targetMode, cost,
                    Battle_ClassifyEntryKind((const struct BattleAct *)ability));
                state->entryActive[1] = 0;
                Resource_ResetEntry(handle);
                UiWork_Finalize(win, 1);
                if (mode == -1)
                {
                    goto psynergy_menu;
                }
                kind = 1;
                aux = mode;
                break;
            }

            case 16:
            {
                const struct SummonDefinition *djinn;
                struct BattleAct              *ability;
                struct MenuSprite             *slot;
                s32                            handle;
                s32                            ok;
                s32                            j;
                s32                            n;

                AudioCommand_PlayFar(112);
                MenuCell_SetCursor(0, 0, 0);
            djinn_menu:
#if EDITION_INTERNATIONAL
                state->entries[0].oam.f.x = 80;
#else
                state->entries[0].oam.f.x = 104;
#endif
                BattleLayout_HighlightPartyPanels(hdr);
                mode = SummonMenu_SelectSummon(0, 0, state->cost[i]);
                BattleLayout_HighlightPartyPanels(hdr);
                if (mode == -1)
                {
                    goto mark_visible;
                }
                kind = 6;
                param = mode;
                WaitFrames(1);
                slot = &state->entries[1];
                djinn = SummonDefinition_Get(mode);
                ability = BattleActGet(djinn->nameMessageId);
                handle = Resource_LoadIntoFreeSlot(128);
#if EDITION_INTERNATIONAL
                win = UiWindow_Create(10, 17, 17, 3, 6);
#else
                win = UiWindow_Create(10, 17, 14, 3, 6);
#endif
                n = 0;
                for (j = 0; j <= 3; j++)
                {
                    if (djinn->djinnRequired[j] > state->cost[i][j])
                    {
                        break;
                    }
                }
                ok = (j == 4);
                state->entries[0].oam.f.x = 56;
                slot->oam.word.attr01 = 0x40000400;
                slot->oam.word.attr23 = 0;
                /* FAKEMATCH: instruction order only (pool placement, schedule): counted as 4 bytes, the empty asm puts the pool after mov r2, r9 as native (constant 56) and keeps ok = 1 - (j != 4) among the slot stores (differ 12). 2026-10-11: arm_reorg runs before shorten_branches and counts the empty asm at its 4-byte default length while it emits nothing; the halfword constant fix needs that much more estimated length after it, and no C statement or single pattern-length change (strh, str, orr, and tested over all 413 English objects) supplies it. */
                __asm__ volatile("");
                slot->oam.f.x = win->x * 8 + 8;
                slot->oam.f.y = win->y * 8 + 4;
                slot->oam.f.tile =
                    Resource_LoadIndexedEntryToBuffer(djinn->nameMessageId & 0x3fff, handle);
                state->entryActive[1] = 1;
                if (ok == 0)
                {
                    UiWork_SetParamNibble(2);
                }
                UiText_DrawCharacterAtOffset(
                    SummonDefinition_Get(mode)->nameMessageId + MsgAbilityName, win, 16, 0);
                for (j = 0; j <= 3; j++)
                {
                    if (djinn->djinnRequired[j] != 0)
                    {
#if EDITION_INTERNATIONAL
                        UiWindow_SetTilemapEntry(win, j + 20481, n * 2 + 13, 0, 0);
                        UiWindow_PutGlyph(win, djinn->djinnRequired[j] + 48, n * 2 + 14, 0, 0);
#else
                        UiWindow_SetTilemapEntry(win, j + 20481, n * 2 + 10, 0, 0);
                        UiWindow_PutGlyph(win, djinn->djinnRequired[j] + 48, n * 2 + 11, 0, 0);
#endif
                        n++;
                    }
                }
                if (ok != 0)
                {
                    AudioCommand_PlayFar(112);
                }
                else
                {
                    AudioCommand_PlayFar(114);
                }
                mode = BattleTarget_RunSelection(actorId, ability->targetMode, ability->range,
                    Battle_ClassifyEntryKind((const struct BattleAct *)ability));
                ent->range = ability->range;
                state->entryActive[1] = 0;
                Resource_ResetEntry(handle);
                UiWork_Finalize(win, 1);
                if (mode == -1)
                {
                    goto djinn_menu;
                }
                for (j = 0; j <= 3; j++)
                {
                    if (djinn->djinnRequired[j] > state->cost[i][j])
                    {
                        state->cost[i][j] = 0;
                        break;
                    }
                    state->cost[i][j] -= djinn->djinnRequired[j];
                }
                aux = mode;
                break;
            }

            case 15:
            {
                struct BattleAct *ability;
                struct UiWindow  *hint;
                s32               cost;
                s32               element;
                s32               index;

                AudioCommand_PlayFar(112);
                MenuCell_SetCursor(0, 0, 0);
            summon_menu:
                state->entries[0].oam.f.x = 144;
                COUNTDOWN->selected[i] = 0x80000000;
                mode = Battle_SelectAbility(actorId, 1);
                ent->range = 0;
                if (mode == -1)
                {
                    goto mark_visible;
                }
                kind = 5;
                param = mode;
                COUNTDOWN->selected[i] = mode;
                element = (param >> 8) & 15;
                index = param & 255;
                if (Djinn_IsActiveFar(actorId, element, index) != 0)
                {
                    ability = BattleActGet(Djinn_GetDefinitionHeaderFar(element, index));
                    cost = ability->range;
                    win = UiWindow_Create(11, 17, 10, 3, 6);
                    state->entries[0].oam.f.x = 64;
                    UiWindow_SetTilemapEntry(win, element + 20481, 0, 0, 0);
                    UiText_DrawCharacterAtOffset(element * 20 + index + MsgDjinnName, win, 16, 0);
                    ent->range = cost;
                    WaitFrames(1);
                    AudioCommand_PlayFar(112);
                    mode = BattleTarget_RunSelection(actorId, ability->targetMode, cost,
                        Battle_ClassifyEntryKind((const struct BattleAct *)ability));
                    if (state->hintCount != 0)
                    {
                        hint = UiText_ShowMessageAndWaitComplete(MsgDjinnUnleashHint, 15, 8);
                        while (UiWork_IsComplete() == 0)
                        {
                            WaitFrames(1);
                        }
                        UiWork_Finalize(hint, 1);
                        state->hintCount++;
                        state->hintTimer = 45;
                    }
                    UiWork_Finalize(win, 1);
                    if (mode == -1)
                    {
                        goto summon_menu;
                    }
                    aux = mode;
                    break;
                }
                win = UiWindow_Create(11, 17, 10, 3, 6);
                state->entries[0].oam.f.x = 64;
                UiWork_SetParamNibble(2);
                UiWindow_SetTilemapEntry(win, element + 20481, 0, 0, 0);
                UiText_DrawCharacterAtOffset(element * 20 + index + MsgDjinnName, win, 16, 0);
                UiWork_SetParamNibble(15);
                ent->range = 1;
                WaitFrames(1);
                AudioCommand_PlayFar(112);
                mode = BattleTarget_RunSelection(actorId, 4, 0, 7);
                UiWork_Finalize(win, 1);
                if (mode == -1)
                {
                    goto summon_menu;
                }
                aux = mode;
                break;
            }

            case 2:
            {
                struct MenuSprite *slot;
                struct BattleAct  *ability;
                u16               *list;
                u8                *pick;
                s32                cost;
                s32                handle;
                s32                cnt;
                s32                k;
                s32                cur;
                s32                selection;

                AudioCommand_PlayFar(112);
                mode = 0;
                MenuCell_SetCursor(mode, mode, mode);
            item_menu:
                state->entries[0].oam.f.x = 96;
                pick = state->pick;
                list = state->list;
                cnt = 0;
                k = 0;
                cur = actor->inventory[k];
                if (cur != 0)
                {
                    do
                    {
                        Item_GetDirectFar(cur);
                        if (Item_ClassifyUseAbility(actorId, cur) == 0)
                        {
                            list[cnt] = cur;
                            pick[cnt++] = k;
                        }
                        k++;
                        if (k == 15)
                        {
                            break;
                        }
                        cur = actor->inventory[k];
                    } while (cur != 0);
                }
                k = 0;
                cur = actor->inventory[k];
                if (cur != 0)
                {
                    do
                    {
                        Item_GetDirectFar(cur);
                        if (Item_ClassifyUseAbility(actorId, cur) != 0)
                        {
                            list[cnt] = cur;
                            pick[cnt++] = k;
                        }
                        k++;
                        if (k == 15)
                        {
                            break;
                        }
                        cur = actor->inventory[k];
                    } while (cur != 0);
                }
                list[cnt] = 0;
                mode = ItemList_SelectEntry(actorId, list, cnt);
                if (mode == -1)
                {
                    goto mark_visible;
                }
                selection = pick[mode];
                param = selection;
                ability = BattleActGet(Item_GetDirectFar(actor->inventory[selection])->actionId);
                cost = ability->range;
                slot = &state->entries[1];
                handle = Resource_LoadIntoFreeSlot(128);
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
    || defined(TBS_EDITION_IT)
                win = UiWindow_Create(9, 17, 16, 3, 6);
#else
                win = UiWindow_Create(9, 17, 15, 3, 6);
#endif
                state->entries[0].oam.f.x = 48;
                slot->oam.word.attr01 = 0x40000400;
                slot->oam.word.attr23 = 0;
                /* FAKEMATCH: instruction order only (pool placement): counted as 4 bytes, the empty asm puts the pool after ldr r1, [sp, #72] as native (constant 48) (differ 10). 2026-10-11: arm_reorg runs before shorten_branches and counts the empty asm at its 4-byte default length while it emits nothing; the halfword constant fix needs that much more estimated length after it, and no C statement or single pattern-length change (strh, str, orr, and tested over all 413 English objects) supplies it. */
                __asm__ volatile("");
                slot->oam.f.x = win->x * 8 + 8;
                slot->oam.f.y = win->y * 8 + 4;
                slot->oam.f.tile =
                    Resource_LoadKind26EntryToBuffer(actor->inventory[selection], handle);
                state->entryActive[1] = 1;
                UiText_DrawCharacterAtOffset((actor->inventory[selection] & 0x1ff) + MsgItemName,
                    (struct RenderInput *)win, 24, 0);
                ent->range = cost;
                AudioCommand_PlayFar(112);
                mode = BattleTarget_RunSelection(actorId, ability->targetMode, cost,
                    Battle_ClassifyEntryKind((const struct BattleAct *)ability));
                state->entryActive[1] = 0;
                Resource_ResetEntry(handle);
                UiWork_Finalize(win, 1);
                if (mode == -1)
                {
                    goto item_menu;
                }
                kind = 2;
                aux = mode;
                break;
            }

            case 3:
                kind = 3;
                break;
            }

            AudioCommand_PlayFar(110);
            ent->unitId = (u16)actorId;
            order = actor->agi;
            if (order != 0)
            {
                order += (u32)(actor->agi * Random16()) >> 20;
            }
            ent->priority = order;
            if (i != 0 && in[i] == in[i - 1])
            {
                ent->priority = (s16)order / 2;
            }
            if ((s16)ent->priority < 0)
            {
                ent->priority = 2000;
            }
            ent->command = kind;
            ent->parameter = param;
            ent->target = aux;
            i++;
            if (i < count)
            {
                continue;
            }
            break;
        }

        if (i < count)
        {
            continue;
        }
        break;
    }

    if (state->enabled != 0)
    {
        sync = (u16 *)gSerialTransfer.payload;
        sync[4] = 'E';
        sync[5] = 'D';
        sync[6] = 'V';
        sync[7] = 'S';
    }
    if (state->displayHandle != 0)
    {
        UiWork_Finalize((struct UiWindow *)state->displayHandle, 1);
    }
    Resource_ResetEntry(res512);
    Resource_ResetEntry(res1024);
    Scheduler_RemoveCallback(UpdateLinkSessionCountdown);
    if (state->enabled != 0)
    {
        s32 DrawLocalizedResource(void)
        {
            u16 data[64];
            TEXT_LOAD(MsgWaitingForOpponent, data, TEXT_COUNT);
            UiText_RenderWideStringAtOffset(data, (struct UiWindow *)state->displayHandle, 0, 4);
        }

        session = gBattleWork;
        mode = 0;
        if (state->displayHandle == 0 && session->linkPaused == 0)
        {
            state->displayHandle = (s32)UiWindow_Create(0, 16, 30, 4, 42);
            Ui_FillVramBlockPattern();
            DrawLocalizedResource();
        }
        else
        {
            state->displayHandle = 0;
        }
        for (;;)
        {
            peer = (u16 *)&gLinkPeerSignatures[session->linkSide ^ 1];
            if (session->linkPaused != 0)
            {
                count = -1;
                break;
            }
            if ((gLinkStatus & 3) != 3)
            {
                mode++;
                if (mode > 24)
                {
                    count = -1;
                    break;
                }
            }
            else
            {
                mode = 0;
                if (peer[4] == 'E' && peer[5] == 'D' && peer[6] == 'V' && peer[7] == 'S')
                {
                    break;
                }
                if (!(peer[4] == 'V' && peer[5] == 'S' && peer[6] == 'S' && peer[7] == 'T'))
                {
                    mode = 1;
                }
            }
            WaitFrames(1);
        }
        if (state->displayHandle != 0)
        {
            UiWork_Finalize((struct UiWindow *)state->displayHandle, 1);
        }
    }
    Camera_ConfigureSceneFar(0);
    Runtime_ReleaseHeapBlock(57);
    return count;
}

struct WindowLine
{
    u16 left;
    u16 span;
};

struct WindowBuffer
{
    u8                unk000[4];
    struct WindowLine lines[160];
};

void UiWork_CloseAndRelease(void);

void AffineEffect_UpdateFrame(void)
{
    struct ResourceMenuWork  *work;
    struct ResourceMenuEntry *sprite;
    struct AffineTransform    effect;
    s32                       scale;
    s32                       matrix;
    s32                       edge;

    s32                            y;
    u32                            end;
    u32                            i;
    u16                           *line;
    struct DisplayTransitionState *buffers;

    work = gMenuSelectWork;
    sprite = work->entries;
    scale = gUi_BounceScales[(gFrameCount * 2) & 31];
    scale = (scale - 256) / 4 + 304;
    effect.scaleX = scale;
    effect.scaleY = scale;
    effect.angle = 0;
    matrix = AffineMatrix_BuildForEffect(&effect);
    for (i = 0; i < work->count; i++, sprite++)
    {
        u32 *attr;
        s32  slot;
        s32  px;

        px = sprite->x;
        if (px == 0)
            continue;
        if (i == work->selection)
        {
            s32 left = px + scale * 7 / 512 - 20;

            if (sprite->y != 0)
                y = sprite->y + scale * 3 / 256 - 20;
            else
                y = (scale * 15 / 256 - 30) & 255;
            attr = &sprite->link;
            *attr++ = 0;
            *attr++ = (matrix << 25) | y | (left << 16) | 0x80002300;
            *attr = gVramBlockCache[sprite->slot].offset >> 5;
            slot = 246;
        }
        else
        {
            u32 base;
            u32 value;

            attr = &sprite->link;
            /* FAKEMATCH: needed to match: reading the y halfword through attr
             * keeps the attribute-pointer copy before its load; sprite->y
             * moves the copy two instructions later. */
            y = ((s16 *)attr)[7];
            *attr++ = 0;
            value = y | (px << 16) | 0x80002000;
            base = gVramBlockCache[sprite->slot].offset;
            *attr++ = value;
            *attr = base >> 5;
            slot = 245;
        }
        Runtime_PushSlotEntry(sprite, slot);
    }
    if (work->row == 0)
    {
        struct WindowBuffer *window;

        s32 span;

        buffers = (struct DisplayTransitionState *)gDisplayTransition;
        if (buffers == 0 || work->count == 0)
            return;
        window = (struct WindowBuffer *)buffers->pages[buffers->page];
        edge = work->entries[work->selection].x;
        span = (scale * 12 - 2817) / 256;
        edge = ((edge - span) << 8) + (edge + span) + 23;
        end = work->entries[work->selection].y + 24;
        end += (scale * 32 - 7937) / 512 + 1;
        line = &window->lines[24].span;
        i = 24;
        if (i < end)
        {
            do
            {
                *line = (*line & 0xff) | edge;
                i++;
                line += 2;
            } while (i < end);
        }
        edge = work->entries[0].x;
        if (work->selection == 0)
            edge -= (scale * 12 - 2817) / 256;
        edge <<= 8;
        line = &window->lines[0].span;
        for (i = 0; i <= 23; i++)
        {
            *line = (*line & 0xff) | edge;
            line += 2;
        }
    }
    else
    {
        struct WindowBuffer *window;
        s32                  start;

        s32 span;

        buffers = (struct DisplayTransitionState *)gDisplayTransition;
        if (buffers == 0 || work->count == 0)
            return;
        window = (struct WindowBuffer *)buffers->pages[buffers->page];
        edge = work->entries[work->selection].x;
        span = (scale * 12 - 2817) / 256;
        edge = ((edge - span) << 8) + (edge + span) + 23;
        start = work->entries[work->selection].y;
        start -= (scale * 32 - 7937) / 512 + 1;
        line = &window->lines[start].span;
        i = start;
        if (i <= 135)
        {
            do
            {
                *line = (*line & 0xff) | edge;
                i++;
                line += 2;
            } while (i <= 135);
        }
        edge = work->entries[0].x;
        if (work->selection == 0)
            edge -= (scale * 12 - 2817) / 256;
        edge <<= 8;
        line = &window->lines[136].span;
        for (i = 136; i <= 159; i++)
        {
            *line = (*line & 0xff) | edge;
            line += 2;
        }
    }
}

void *AffineEffect_InitializeWork(void)
{
    void *work;
    work = Runtime_AllocateBlock(58, 152);
    Dma_Fill32(0, work, 152);
    Scheduler_AddOrUpdateCallback(AffineEffect_UpdateFrame, 0xc76);
    return work;
}

void Menu_EndResourceSelection(void)
{
    struct UiWindow         *child;
    s32                      i;
    struct ResourceMenuWork *work;

    work = gMenuSelectWork;
    Scheduler_RemoveCallback(AffineEffect_UpdateFrame);
    child = work->window;
    if (child != 0)
    {
        UiWork_Finalize(child, 2);
    }
    i = 0;
    while (i < work->count)
    {
        Resource_ResetEntry(work->entries[i].slot);
        i += 1;
    }
    Runtime_ReleaseHeapBlock(0x3A);
    WaitFrames(1U);
}

s32 Menu_RunResourceSelectionLoop(s32 initial)
{
    struct ResourceMenuWork *work = gMenuSelectWork;
    s32                      resource;

    work->selection = initial;
redraw:
    RenderOutput_PrepareForRedraw(work->window);
    if (work->resourceBase != 0)
        resource = work->resourceBase + work->selection;
    else
        resource = work->resourceIds[work->selection] + MsgCommandName;
    UiText_DrawCharacterAtOffset(resource, work->window, 0, 0);
    for (;;)
    {
        WaitFrames(1);
        if (gKeyState & KEY_A)
        {
            AudioCommand_PlayFar(112);
            return work->selection;
        }
        if (gKeyState & KEY_B)
        {
            AudioCommand_PlayFar(113);
            return -1;
        }
        if (gKeyState & KEY_START)
        {
            AudioCommand_PlayFar(113);
            return -1;
        }
        if ((gKeysRepeat & KEY_LEFT) || (gKeysRepeat & KEY_UP))
        {
            AudioCommand_PlayFar(111);
            work->selection--;
            if (work->selection < 0)
                work->selection = work->count - 1;
        }
        else if ((gKeysRepeat & KEY_RIGHT) || (gKeysRepeat & KEY_DOWN))
        {
            AudioCommand_PlayFar(111);
            work->selection++;
            if (work->selection >= work->count)
                work->selection = 0;
        }
        else
            continue;
        goto redraw;
    }
}

static inline s32 AbsoluteDifference(s32 diff, s32 lhs, s32 rhs)
{
    if (diff >= 0)
        return diff;
    return rhs - lhs;
}

s32 Menu_SelectResource(s32 start, s32 goal)
{
    s16                      resourceBase;
    s16                      cur;
    s32                      diff;
    s32                      resourceId;
    s32                      dist;
    s32                      pos;
    s32                      step;
    s32                      delay;
    const u8                *table;
    struct ResourceMenuWork *state;

    state = gMenuSelectWork;
    step = 1;
    delay = 12;
    state->selection = start;
    if (goal < start)
        step = -1;
    pos = start;

    for (;;)
    {
        RenderOutput_PrepareForRedraw(state->window);
        resourceBase = state->resourceBase;
        if (resourceBase != 0)
        {
            resourceId = resourceBase + state->selection;
        }
        else
        {
            resourceId = state->resourceIds[state->selection] + MsgCommandName;
        }
        UiText_DrawCharacterAtOffset(resourceId, state->window, 0, 0);

        cur = state->selection;
        table = gMenu_SelectionStepDelays;
        diff = cur - goal;
        dist = AbsoluteDifference(diff, cur, goal);
        WaitFrames(table[dist] + delay);

        if (pos == goal)
            break;

        state->selection += step;
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        delay = 0;
        pos += step;
    }

    WaitFrames(48);
    AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
    return goal;
}

void Menu_LoadResourceSlot(s32 slot, s32 index)
{
    s32   size = 1024;
    void *buffer = MemAlloc(size);
    u16  *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);

    Resource_DecodeByteLz((void *)((u32)base + base[index]), buffer);
    VramBlock_LoadCached(slot, size, buffer);
    MemFree(buffer);
}

void Menu_AppendResourceEntry(s32 no)
{
    struct ResourceMenuWork  *work;
    struct ResourceMenuEntry *entry;
    s16                       index;
    s32                       slot;
    s32                       flags;

    work = gMenuSelectWork;
    index = work->count;
    if (index <= 5)
    {
        work->count++;
        entry = &work->entries[index];
        slot = Resource_FindFreeEntry();
        Menu_LoadResourceSlot(slot, no);
        entry->x = index * 24 + 32;
        flags = 136;
        entry->y = flags;
        entry->slot = slot;
        work->resourceIds[index] = no;
    }
}

void Menu_CenterResourceEntries(s32 row, s32 width, s32 resourceBase)
{
    struct ResourceMenuWork *menu = gMenuSelectWork;
    s32                      x;
    s32                      i;
    s32                      count;

    menu->width = width + 2;
    menu->resourceBase = resourceBase;
    menu->row = row;
    count = menu->count;
    x = 15 - (count * 3 + menu->width * 2 / 3) / 2;
    for (i = 0; i < menu->count; i++)
    {
        struct ResourceMenuEntry *entry = &menu->entries[i];

        entry->x = x * 8;
        entry->y = row * 8;
        x += 3;
    }
    menu->window = UiWindow_Create(x, row, menu->width, 3, 2);
}

void Menu_LayoutResourceEntries(s32 x, s32 y, s32 width, s32 resourceBase)
{
    struct ResourceMenuWork  *work;
    s32                       i;
    struct ResourceMenuEntry *entry;

    work = gMenuSelectWork;
    work->width = width + 2;
    work->resourceBase = resourceBase;
    work->row = y;
    for (i = 0; i < work->count; i++)
    {
        entry = &work->entries[i];
        entry->y = y << 3;
        entry->x = x << 3;
        x += 3;
    }
    work->window = UiWindow_Create(x, y, work->width, 3, 2);
}

static __inline__ s32 TableGet(s8 *table, s32 index)
{
    return table[index];
}

s32 Menu_SelectTopEntry(s32 selection)
{
    s32 group = 0;
    s32 offset;
    s32 triple;
    s32 ret;
    s8 *table;

    if (Party_SumDjinnCountsFar(-1) == 0)
    {
        group = 1;
    }

    triple = group * 3;
    table = gMenu_TopEntryPositionByCommand;
    offset = triple << 1;
    selection = TableGet(table, selection + offset) - 1;
    if (selection < 0)
    {
        selection = 0;
    }

    AffineEffect_InitializeWork();
    Menu_AppendResourceEntry(1);
    if (group == 0)
    {
        Menu_AppendResourceEntry(15);
    }
    Menu_AppendResourceEntry(2);
    Menu_AppendResourceEntry(7);
    Menu_CenterResourceEntries(17, SELECT_MENU_WIDTH, 0);
    ret = Menu_RunResourceSelectionLoop(selection);
    Menu_EndResourceSelection();

    if (ret >= 0)
    {
        ret = gMenu_TopEntryCommandByPosition[ret + offset + 1];
    }

    return ret;
}

#if EDITION_INTERNATIONAL
#define ANIMATE_ENTRIES_ROW 7
#else
#define ANIMATE_ENTRIES_ROW 5
#endif

s32 Menu_AnimateSelectionToEntry(s32 arg0, s32 arg1)
{
    UiWindow_OpenMode1AndWaitFrame();
    AffineEffect_InitializeWork();
    Menu_AppendResourceEntry(1);
    Menu_AppendResourceEntry(0xF);
    Menu_AppendResourceEntry(2);
    Menu_AppendResourceEntry(7);
    Menu_CenterResourceEntries(0x11, ANIMATE_ENTRIES_ROW, 0);
    arg1 = Menu_SelectResource(arg0, arg1 - 1);
    Menu_EndResourceSelection();
    UiWork_CloseAndRelease();
    return arg1;
}

s32 Menu_SelectSaveSlotAction(void)
{
    s32 type;
    s32 ret;
    s32 initial;
    u32 group;

    group = 0;
    initial = 0;
    type = SaveState_ScanRecordFlags();
    if (type < 0)
    {
        return -1;
    }
    if (type == 0)
    {
        return 0;
    }
    if (type == 3)
    {
        group = 1;
    }
    else if (type == 0x67)
    {
        group = 2;
    }
    else if (type > 0x64)
    {
        group = 3;
    }
    else
    {
        initial = 1;
    }
    AffineEffect_InitializeWork();
    if ((group == 0) || (group == 3))
    {
        Menu_AppendResourceEntry(0x15);
    }
    if (group <= 1U)
    {
        Menu_AppendResourceEntry(0x16);
    }
    if ((group == 0) || (group == 3))
    {
        Menu_AppendResourceEntry(0x17);
    }
    Menu_AppendResourceEntry(0x18);
    if ((*(s16 *)gTitleExtraOptionEnabled) != 0)
    {
        Menu_AppendResourceEntry(0x1D);
    }
    if (gTitleSendOptionEnabled != 0)
    {
        Menu_AppendResourceEntry(0x1E);
    }
    Menu_CenterResourceEntries(0x11, TYPE_MENU_WIDTH, 0);
    ret = Menu_RunResourceSelectionLoop(initial);
    Menu_EndResourceSelection();
    if (ret >= 0)
    {
        ret = gMenu_SaveSlotActionByPosition[ret + (group * 6)];
    }
    return ret;
}

#if EDITION_INTERNATIONAL
#define MODE_LABEL_X 18
#define MODE_LABEL_RIGHT 144
#else
#define MODE_LABEL_X 8
#define MODE_LABEL_RIGHT 80
#endif

void Menu_DrawModeLabel(void)
{
    struct ResourceMenuWork *state = gMenuSelectWork;

    if (state->previousMode != state->selection)
    {
        state->previousMode = state->selection;
        UiWindow_ClearInteriorTiles(state->lowerWindow, 8, 40, MODE_LABEL_RIGHT, 80);

        switch (state->selection)
        {
        case 0:
        {
            s32 text = MsgPasswordLevel;

            UiText_DrawCharacterAtOffset(text, state->lowerWindow, MODE_LABEL_X, 40);
            UiText_DrawCharacterAtOffset(text + 1, state->lowerWindow, MODE_LABEL_X, 48);
            UiText_DrawCharacterAtOffset(text + 2, state->lowerWindow, MODE_LABEL_X, 56);
            UiText_DrawCharacterAtOffset(text + 3, state->lowerWindow, MODE_LABEL_X, 64);
            text += 4;
            UiText_DrawCharacterAtOffset(text, state->lowerWindow, MODE_LABEL_X, 72);
            break;
        }
        case 1:
        {
            s32 text = MsgPasswordLevel;

            UiText_DrawCharacterAtOffset(text, state->lowerWindow, MODE_LABEL_X, 40);
            UiText_DrawCharacterAtOffset(text + 1, state->lowerWindow, MODE_LABEL_X, 48);
            text += 2;
            UiText_DrawCharacterAtOffset(text, state->lowerWindow, MODE_LABEL_X, 56);
            break;
        }
        case 2:
        default:
        {
            s32 text = MsgPasswordLevel;

            UiText_DrawCharacterAtOffset(text++, state->lowerWindow, MODE_LABEL_X, 40);
            UiText_DrawCharacterAtOffset(text, state->lowerWindow, MODE_LABEL_X, 48);
            break;
        }
        }
    }
}

#if EDITION_INTERNATIONAL
#define PASSWORD_HELP_X 16
#else
#define PASSWORD_HELP_X 0
#endif

#if defined(TBS_EDITION_FR)

void Menu_DrawModeIndicator(void)
{
    struct ResourceMenuWork *state = gMenuSelectWork;

    if (state->previousMode != state->selection)
    {
        if (state->selection == 0)
        {
            RenderOutput_ClearList(state->lowerWindow);
            UiText_DrawResource(MsgPasswordTransferHelp, (s32)state->lowerWindow, 16, 4);
            UiText_DrawResource(MsgPasswordTransferHelp + 1, (s32)state->lowerWindow, 16, 16);
        }
        else if (state->previousMode == 0)
        {
            RenderOutput_ClearList(state->lowerWindow);
            UiText_DrawResource(MsgCableTransferHelp, (s32)state->lowerWindow, 0, 4);
            UiText_DrawResource(MsgCableTransferHelp + 1, (s32)state->lowerWindow, 0, 16);
            UiText_DrawResource(MsgCableTransferHelp + 2, (s32)state->lowerWindow, 0, 28);
        }
        state->previousMode = state->selection;
    }
}
#else
void Menu_DrawModeIndicator(void)
{
    struct ResourceMenuWork *state = gMenuSelectWork;
    s16                     *shown = &state->previousMode;
    s16                     *current = &state->selection;

    if (*shown != *current)
    {
        *shown = *current;
        RenderOutput_PrepareForRedraw(state->lowerWindow);
        if (*current == 0)
        {
#if defined(TBS_EDITION_DE)
            UiText_DrawCharacterAtOffset(MsgPasswordTransferHelp, state->lowerWindow, 32, 8);
            UiText_DrawCharacterAtOffset(MsgPasswordTransferHelp + 1, state->lowerWindow, 32, 24);
#else
            UiText_DrawResource(MsgPasswordTransferHelp, state->lowerWindow, PASSWORD_HELP_X, 4);
            UiText_DrawResource(
                MsgPasswordTransferHelp + 1, state->lowerWindow, PASSWORD_HELP_X, 16);
#endif
        }
        else
        {
#if defined(TBS_EDITION_DE)
            UiText_DrawCharacterAtOffset(MsgCableTransferHelp, state->lowerWindow, 0, 0);
            UiText_DrawCharacterAtOffset(MsgCableTransferHelp + 1, state->lowerWindow, 0, 16);
            UiText_DrawCharacterAtOffset(MsgCableTransferHelp + 2, state->lowerWindow, 0, 32);
#else
            UiText_DrawResource(MsgCableTransferHelp, state->lowerWindow, 0, 4);
            UiText_DrawResource(MsgCableTransferHelp + 1, state->lowerWindow, 0, 16);
            UiText_DrawResource(MsgCableTransferHelp + 2, state->lowerWindow, 0, 28);
#endif
        }
    }
}
#endif

s32 Menu_SelectResourceLayout(s32 mode)
{
    struct ResourceMenuWork *work;
    struct UiWindow         *window;
    s32                      msg;
    struct UiWindow        **upper;
    s32                      result;

    AffineEffect_InitializeWork();
    work = gMenuSelectWork;
    if (mode == 0)
    {
        Menu_AppendResourceEntry(44);
        Menu_AppendResourceEntry(45);
    }
    else
    {
        Menu_AppendResourceEntry(46);
        Menu_AppendResourceEntry(47);
        Menu_AppendResourceEntry(48);
#if defined(TBS_EDITION_ES)
        Menu_CenterResourceEntries(17, 5, 0);
#endif
    }
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) || defined(TBS_EDITION_IT)
    Menu_CenterResourceEntries(17, 7, 0);
#elif defined(TBS_EDITION_ES)
#elif defined(TBS_EDITION_FR)
    Menu_CenterResourceEntries(17, 9, 0);
#else
    Menu_CenterResourceEntries(17, 5, 0);
#endif
    if (mode != 0)
    {
        Scheduler_AddOrUpdateCallback(Menu_DrawModeLabel, 0xc76);
        work->previousMode = 0xffff;
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) || defined(TBS_EDITION_IT)
        window = UiWindow_Create(7, 0, 17, 4, 2);
#elif defined(TBS_EDITION_ES)
        window = UiWindow_Create(5, 0, 21, 4, 2);
#elif defined(TBS_EDITION_FR)
        window = UiWindow_Create(5, 0, 22, 4, 2);
#else
        window = UiWindow_Create(9, 0, 13, 4, 2);
#endif
        msg = MsgPasswordSelection;
        upper = &work->upperWindow;
        *upper = window;
        UiText_DrawResource(msg, (s32)window, 0, 4);
#if EDITION_INTERNATIONAL
#if defined(TBS_EDITION_DE)
        window = UiWindow_Create(1, 4, 28, 12, 2);
#else
        window = UiWindow_Create(3, 4, 25, 12, 2);
#endif
        work->lowerWindow = window;
        UiText_DrawResource(msg + 1, (s32)window, 8, 0);
        UiText_DrawResource(msg + 2, (s32)work->lowerWindow, 8, 11);
        msg += 3;
        UiText_DrawResource(msg, (s32)work->lowerWindow, 8, 22);
#else
        window = UiWindow_Create(5, 4, 21, 12, 2);
        work->lowerWindow = window;
        UiText_DrawResource(msg + 1, (s32)window, 0, 4);
        msg += 2;
        UiText_DrawResource(msg, (s32)work->lowerWindow, 0, 16);
#endif
    }
    else
    {
        Scheduler_AddOrUpdateCallback(Menu_DrawModeIndicator, 0xc76);
        work->previousMode = 0xffff;
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE)
        window = UiWindow_Create(6, 0, 18, 4, 2);
#elif defined(TBS_EDITION_ES)
        window = UiWindow_Create(4, 0, 21, 4, 2);
#elif defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
        window = UiWindow_Create(5, 0, 20, 4, 2);
#else
        window = UiWindow_Create(9, 0, 13, 4, 2);
#endif
        upper = &work->upperWindow;
        *upper = window;
#if EDITION_INTERNATIONAL
        UiText_DrawResource(MsgTransferMethod, (s32)window, 2, 4);
#else
        UiText_DrawResource(MsgTransferMethod, (s32)window, 0, 4);
#endif
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
    || defined(TBS_EDITION_IT)
        work->lowerWindow = UiWindow_Create(1, 5, 28, 7, 2);
#elif defined(TBS_EDITION_DE)
        work->lowerWindow = UiWindow_Create(0, 5, 30, 7, 2);
#else
        work->lowerWindow = UiWindow_Create(5, 5, 21, 7, 2);
#endif
    }
    result = Menu_RunResourceSelectionLoop(0);
    if (mode != 0)
        Scheduler_RemoveCallback(Menu_DrawModeLabel);
    else
        Scheduler_RemoveCallback(Menu_DrawModeIndicator);
    RenderOutput_PrepareForRedraw(*upper);
    RenderOutput_PrepareForRedraw(work->lowerWindow);
    UiWork_Finalize(*upper, 2);
    UiWork_Finalize(work->lowerWindow, 2);
    WaitFrames(1);
    Menu_EndResourceSelection();
    return result;
}

//--------------------------------------------------------------------------
// confirm_selection.c
// メニュー：コマンドのアイコンを並べてえらばせる（はい・いいえ など）
//--------------------------------------------------------------------------
// アイコンは ResourceId_CommandIcons の番号。
// えらんでいるコマンドの名前をウィンドウに出す
//--------------------------------------------------------------------------
// アイコン 0x11〜0x14 を画面の中央に並べてえらぶ（行 17）
// selection : はじめにえらんでいる位置。戻り値はえらんだ位置
//--------------------------------------------------------------------------
s32 Menu_SelectEntry11To14(s32 selection)
{
    s32 ret;

    AffineEffect_InitializeWork();
    Menu_AppendResourceEntry(0x11);
    Menu_AppendResourceEntry(0x12);
    Menu_AppendResourceEntry(0x13);
    Menu_AppendResourceEntry(0x14);
#if defined(TBS_EDITION_DE) || defined(TLA_EDITION_DE)
    Menu_CenterResourceEntries(17, 8, 0); // 名前のウィンドウの幅
#elif EDITION_INTERNATIONAL
    Menu_CenterResourceEntries(17, 7, 0);
#else
    Menu_CenterResourceEntries(17, 6, 0);
#endif
    ret = Menu_RunResourceSelectionLoop(selection);
    Menu_EndResourceSelection();
    return ret;
}

//--------------------------------------------------------------------------
// アイコン 0x19〜0x1c を画面の中央に並べてえらぶ（行 17）
//--------------------------------------------------------------------------
s32 Menu_SelectEntry19To1c(s32 selection)
{
    s32 ret;

    AffineEffect_InitializeWork();
    Menu_AppendResourceEntry(0x19);
    Menu_AppendResourceEntry(0x1A);
    Menu_AppendResourceEntry(0x1B);
    Menu_AppendResourceEntry(0x1C);
#if EDITION_INTERNATIONAL
    Menu_CenterResourceEntries(17, 10, 0);
#else
    Menu_CenterResourceEntries(17, 8, 0);
#endif
    ret = Menu_RunResourceSelectionLoop(selection);
    Menu_EndResourceSelection();
    return ret;
}

//--------------------------------------------------------------------------
// はい・いいえ（アイコン 5・6）をえらぶ。キャンセルは 1（いいえ）
// low  : 0 なら行 0、0 でなければ行 17 に出す
// message  : 名前のメッセージの先頭（0 ならコマンド名）
// w    : 名前のウィンドウの幅（0 なら 3）
// selection  : はじめにえらんでいる位置
//--------------------------------------------------------------------------
s32 Menu_RunConfirmSelection(s32 low, s32 message, s32 w, s32 selection)
{
    s32 row;

    row = 0;

    AffineEffect_InitializeWork();

    if (w == 0)
    {
        w = 3;
    }
    if (low != 0)
    {
        row = 17;
    }

    Menu_AppendResourceEntry(5);
    Menu_AppendResourceEntry(6);
    Menu_CenterResourceEntries(row, w, message);

    selection = Menu_RunResourceSelectionLoop(selection);
    Menu_EndResourceSelection();
    if (selection == -1)
    {
        selection = 1;
    }
    return selection;
}

//--------------------------------------------------------------------------
// はい・いいえを (x, y) に出してえらぶ。キャンセルは 1（いいえ）
//--------------------------------------------------------------------------
s32 Menu_RunConfirmSelectionAt(s32 x, s32 y, s32 selection)
{
    s32 message = MsgCommandYes;

    AffineEffect_InitializeWork();
    Menu_AppendResourceEntry(5);
    Menu_AppendResourceEntry(6);
    Menu_LayoutResourceEntries(x, y, 3, message);
    selection = Menu_RunResourceSelectionLoop(selection);
    Menu_EndResourceSelection();
    if (selection == -1)
    {
        selection = 1;
    }
    return selection;
}

//--------------------------------------------------------------------------
// select_entry_20_to_21.c
// メニュー：コマンドのアイコン 0x20・0x21 の 2 つからえらばせる
//--------------------------------------------------------------------------
// 版ごとの名前のウィンドウの幅
#if !EDITION_INTERNATIONAL
#define PAIR_WIDTH 7
#elif defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT) || defined(TLA_EDITION_FR) \
    || defined(TLA_EDITION_IT)
#define PAIR_WIDTH 10
#else
#define PAIR_WIDTH 9
#endif

//--------------------------------------------------------------------------
// アイコン 0x20・0x21 を画面の中央に並べてえらぶ（行 17）
// selection : はじめにえらんでいる位置。戻り値はえらんだ位置
//--------------------------------------------------------------------------
s32 Menu_SelectEntry20To21(s32 selection)
{
    s32 ret;

    AffineEffect_InitializeWork();
    Menu_AppendResourceEntry(0x20);
    Menu_AppendResourceEntry(0x21);
    Menu_CenterResourceEntries(17, PAIR_WIDTH, 0);
    ret = Menu_RunResourceSelectionLoop(selection);
    Menu_EndResourceSelection();
    return ret;
}

void Scheduler_ScheduleCallbackCAfterFrames(void)
{
    Scheduler_AddOrUpdateCallback((SchedulerCallback)Menu_RunSelection, 0xC80);
}

void Menu_DrawSelectionRow(struct UiWindow *work, s32 first, const s16 *second)
{
    s16 selected = first;
    s32 label = Scene_GetAreaNameIndexFar(selected, *second) + MsgDebugEntryName;
    RenderOutput_PrepareForRedraw(work);
    UiText_DrawNumber(selected, 3, (s32)work, 0, 14);
    UiText_DrawNumber(*second, 3, (s32)work, MENU_LABEL_X, 14);
    UiText_DrawString(gMenu_ColonString, (s32)work, MENU_TEXT_X, 0);
    UiText_DrawResource(selected + MsgDebugCategoryName, (s32)work, 0, 0);
    UiText_DrawString(gMenu_ColonString, (s32)work, MENU_TEXT_X, 14);
    UiText_DrawResource(label, (s32)work, MENU_LABEL_X, 0);
}

s16 Menu_RunSelection(void)
{
    s32               resource;
    s16               mode;
    s16               secondary;
    struct MenuSprite object;
    s16               primary;
    struct UiWindow  *work;
    s16               result;

    work = 0;
    mode = 0;
    primary = gGameState.defaultScene;
    secondary = gGameState.defaultEntrance;
    work = UiWindow_Create(0, 7, 30, 5, 2);
    Menu_DrawSelectionRow(work, primary, &secondary);
    UiTextResource_Initialize(&object, &resource);

    while (gKeysHeld != 0)
        WaitFrames(1);

    for (;;)
    {
        result = Menu_HandleSelectionRowInput(work, primary, &secondary, &mode);
        if (result == -1)
        {
            UiTextResource_Release(resource);
            UiWork_Finalize(work, 2);
            Event_SetDestinationFar(primary, secondary);
            return result;
        }
        if (result == -2)
        {
            UiTextResource_Release(resource);
            UiWork_Finalize(work, 2);
            return result;
        }

        UiTextResource_SetPosition(&object, MENU_TEXT_X, mode * 14 + MENU_TEXT_Y);
        primary = result;
        WaitFrames(1);
    }
}

s32 Menu_HandleSelectionRowInput(s32 arg, s16 value, s16 *sub, s16 *mode)
{
    if (gKeysRepeat & KEY_A)
        return -1;
    if (gKeysRepeat & KEY_B)
        return -2;
    if (gKeysRepeat & KEY_DOWN)
    {
        *mode ^= 1;
    }
    else if (gKeysRepeat & KEY_UP)
    {
        *mode ^= 1;
    }
    else if (gKeysRepeat & KEY_RIGHT)
    {
        if (*mode == 0)
        {
            value++;
        }
        else
        {
            (*sub)++;
            if (*sub > 99)
                *sub = 0;
        }
        if (value > 200)
            value = 0;
        Menu_DrawSelectionRow((struct UiWindow *)arg, value, sub);
    }
    else if (gKeysRepeat & KEY_LEFT)
    {
        if (*mode == 0)
        {
            value--;
        }
        else
        {
            (*sub)--;
            if (*sub < 0)
                *sub = 99;
        }
        if (value < 0)
            value = 200;
        Menu_DrawSelectionRow((struct UiWindow *)arg, value, sub);
    }
    else if (gKeysRepeat & KEY_R)
    {
        if (*mode == 0)
        {
            *sub = 0;
            value += 10;
        }
        else
        {
            *sub += 10;
            if (*sub > 99)
                *sub -= 99;
        }
        if (value > 200)
            value = 0;
        Menu_DrawSelectionRow((struct UiWindow *)arg, value, sub);
    }
    else if (gKeysRepeat & KEY_L)
    {
        if (*mode == 0)
        {
            *sub = 0;
            value -= 10;
        }
        else
        {
            *sub -= 10;
            if (*sub < 0)
                *sub += 99;
        }
        if (value < 0)
            value = 200;
        Menu_DrawSelectionRow((struct UiWindow *)arg, value, sub);
    }
    return value;
}

//--------------------------------------------------------------------------
// run_selection_with_cursor_object.c
// メニュー：フラグの一覧画面（カーソルで選んで A で切りかえる）
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 1 ページ 16x16 のフラグを表示し、閉じるまで入力を受ける
// ページ 8 から始める。戻り値はいつも 0
//--------------------------------------------------------------------------
s32 Menu_RunSelectionWithCursorObject(void)
{
    struct MenuSprite obj;    // カーソルのオブジェクト
    s32               cur[2]; // カーソルの列・行
    s32               res;
    s32               page;
    s32               win;
    s32               ret;

    page = 8;
    win = 0;
    cur[0] = win;
    cur[1] = win;
    win = UiWindow_Create(1, 0, 28, 20, 2);
    Menu_DrawFlagBitTable(win, page);
    UiTextResource_Initialize(&obj, &res);
    for (;;)
    {
        WaitFrames(1);
        ret = Menu_HandleFlagGridInput(win, &page, cur);
        if (ret == -1)
            break; // 閉じる
        if (ret == 1)
            Menu_DrawFlagBitTable(win, page); // フラグかページが変わった
        UiTextResource_SetPosition(&obj, cur[0] * 8 + 58, cur[1] * 8 + 20);
    }
    UiTextResource_Release(res);
    UiWork_Finalize(win, 2);
    return 0;
}

//--------------------------------------------------------------------------
// format_hex.c
// 文字：数を 16 進の文字列にする
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// value の下から count けた（5 けたまで）を、上のけたから大文字の
// 16 進で out に書く（終わりの 0 は書かない）
//--------------------------------------------------------------------------
void Text_FormatHex(u32 value, u32 count, u8 *out)
{
    u8  buf[8];
    u32 i;

    if (count > 5)
        count = 5;

    for (i = 0; i != count; i++)
    {
        u32 d = value & 15;
        if (d <= 9)
            buf[i] = d + '0';
        else
            buf[i] = d + 'A' - 10;
        value >>= 4;
    }

    for (i = count - 1; (s32)i >= 0; i--)
        *out++ = buf[i];
}

void Menu_DrawFlagBitTable(s32 window, s32 startFlag)
{
    s32  row;
    s32  y;
    s32  flag;
    char label[5];
    char bits[17];

    RenderOutput_PrepareForRedraw((struct UiWindow *)window);
    UiText_DrawStringInWindow((u8 *)gMenu_HexDigitsString, window, 48, 0);

    flag = startFlag << 8;
    for (row = 0; row != 16; row++)
    {
        s32 i;

        y = row * 8 + 16;

        for (i = 0; i != 5; i++)
        {
            label[i] = 0;
        }
        Text_FormatHex(flag, 3, (s32)label);
        UiText_DrawStringInWindow((u8 *)label, window, 0, y);
        UiText_DrawStringInWindow((u8 *)gMenu_ColonString, window, 32, y);

        for (i = 0; i < 16; i++)
        {
            s32 val = GameFlag_TestFar(flag);
            bits[i] = (val != 0) + 48;
            flag++;
        }
        bits[i] = 0;
        UiText_DrawStringInWindow((u8 *)bits, window, 48, y);
    }
}

s32 Menu_HandleFlagGridInput(s32 window, s32 *page, s32 *cursor)
{
    s32 *row = &cursor[1];

    if (gKeysRepeat & KEY_A)
    {
        s32 flag = ((*page << 4) + *row << 4) + cursor[0];

        if (GameFlag_TestFar(flag))
            GameFlag_ClearBitFar(flag);
        else
            GameFlag_SetBitFar(flag);
        return 1;
    }
    if ((gKeyState & KEY_B) || (gKeysRepeat & KEY_SELECT))
        return -1;
    if (gKeysRepeat & KEY_UP)
    {
        if (--*row < 0)
            *row = 15;
    }
    else if (gKeysRepeat & KEY_DOWN)
    {
        if (++*row > 15)
            *row = 0;
    }
    else if (gKeysRepeat & KEY_LEFT)
    {
        if (--cursor[0] < 0)
            cursor[0] = 15;
    }
    else if (gKeysRepeat & KEY_RIGHT)
    {
        if (++cursor[0] > 15)
            cursor[0] = 0;
    }
    else if ((gKeysRepeat & KEY_L) && (gKeysRepeat & KEY_START))
    {
        *page -= 10;
        if (*page < 0)
            *page = 15;
        return 1;
    }
    else if ((gKeysRepeat & KEY_R) && (gKeysRepeat & KEY_START))
    {
        *page += 10;
        if (*page > 15)
            *page = 0;
        return 1;
    }
    else if (gKeysRepeat & KEY_L)
    {
        if (--*page < 0)
            *page = 15;
        return 1;
    }
    else if (gKeysRepeat & KEY_R)
    {
        if (++*page > 15)
            *page = 0;
        return 1;
    }
    return 0;
}

void Menu_ReservedNoOp(void)
{
}

void Djinn_SetFoundFlagAndAdd(s32 owner, s32 element, s32 index)
{
    GameFlag_SetBitFar((element * 0x14) + index + 0x30);
    Djinn_AddToOwnerFar(owner, element, index);
}

s32 Menu_RunWorkspaceResultLoop(void)
{
    s32 result;

    result = Menu_RunWorkspaceSelectionLoop();
    if (result == -1)
    {
        return -1;
    }
    if (result == 0)
    {
        if (Save_WriteSelectedSlot() == -1)
        {
            return Menu_RunWorkspaceResultLoop();
        }
    }
    else if (result == 1)
    {
        UiText_ShowPositionedMessageAndWait(MsgSleepMode, 1);
        gSleepRequested = result;
    }
    else if (result == 2)
    {
        if (Menu_RunWorkspaceOptions() == -1)
        {
            return Menu_RunWorkspaceResultLoop();
        }
    }
    return 0;
}

s32 DebugMenu_BrowseIcons(void)
{
    s32                 redraw;
    s32                 tile;
    s32                 page;
    s32                 mode;
    struct RenderInput *window;
    s32                 slot;
    s32                 base;
    s32                 i;
    s32                 row;
    s32                 x;
    s32                 y;

    window = NULL;
    page = 0;
    mode = 0;
    redraw = 1;
    gMenuCtrlWork->suspended = redraw;
    WaitFrames(1);

    while (1)
    {
        if (gKeysRepeat & KEY_LEFT)
        {
            page--;
            redraw = 1;
        }
        if (gKeysRepeat & KEY_RIGHT)
        {
            redraw = 1;
            page++;
        }
        if (gKeysRepeat & KEY_L)
        {
            mode--;
            redraw = 1;
        }
        if (gKeysRepeat & KEY_R)
        {
            redraw = 1;
            mode++;
        }
        if (gKeysRepeat & KEY_A)
            break;
        if (gKeysRepeat & KEY_B)
            break;
        if (redraw != 0)
        {
            redraw = 0;
            page = (page + 8) % 8;
            mode = (mode + 3) % 3;
            UiWork_Finalize(window, 2);
            window = UiWindow_Create(10, 0, 18, 12, 2);
            if (mode == 0)
                UiText_DrawStringInWindow(gWorkspace_ItemTitle, window, 0, 0);
            else if (mode == 1)
                UiText_DrawStringInWindow(gWorkspace_EnergyTitle, window, 0, 0);
            else
                UiText_DrawStringInWindow(gWorkspace_StatusTitle, window, 0, 0);
            UiText_DrawStringInWindow(gWorkspace_PageLabel, window, 0, 8);
            UiText_DrawNumberInWindow(page, 0, window, 40, 8);
            base = page << 5;
            UiText_DrawNumberInWindow(base, 3, window, 64, 8);
            UiText_DrawStringInWindow(gWorkspace_RangeDash, window, 88, 8);
            UiText_DrawNumberInWindow(base + 31, 3, window, 96, 8);
            for (i = 0; i <= 31; i++)
            {
                slot = -1;
                row = i / 8;
                x = (i - row * 8) * 16;
                y = row * 16 + 16;
                if (mode == 0)
                {
                    UiIcon_BuildItemIconTiles(base + i, 1, &slot, &tile, mode);
                    RenderOutput_Create(slot, 0x40000000, window, x, y);
                }
                else if (mode == 1)
                {
                    UiIcon_BuildAbilityIconTiles(base + i, 1, &slot, &tile, 0);
                    RenderOutput_Create(slot, 0x40000000, window, x, y);
                }
                else
                {
                    slot = Resource_FindFreeEntry();
                    Ui_BuildPatternToSlot(i, 0, slot);
                    RenderOutput_Create(slot, 0x40000000, window, x, y);
                }
            }
        }
        WaitFrames(1);
    }
    UiWork_Finalize(window, 2);
    gMenuCtrlWork->suspended = 0;
    return 0;
}

#if !EDITION_INTERNATIONAL || defined(TBS_EDITION_EN)
#define DEBUG_GLYPH_NAME MsgDebugGlyphName
#elif defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR)
#define DEBUG_GLYPH_NAME MsgEnd
#else
#define DEBUG_GLYPH_NAME MsgDebugSecondForm
#endif

s32 DebugMenu_BrowseEntryGlyphs(void)
{
    struct UiRenderWork *work;
    struct RenderInput  *portrait;
    s32                  redraw;
    struct RenderInput  *window;
    s32                  count;
    s32                  total;
    s32                  index;
    s32                  i;
    s32                  glyph;
    s32                  tile;
    s32                  slot;
    s32                  cancel;
    u32                  slotOffset;

    work = UI_WORK;
    redraw = 1;
    portrait = UiWindow_CreateWithSideObject(0, 0, 10, 5);
    window = UiWindow_Create(10, 10, 14, 3, 2);
    index = 0;
    for (i = 0; gPortrait_CharacterMap[i * 2] != -1; i++)
    {
    }
    count = i;
    for (i = 0; gPortrait_SpriteMap[i * 2] != -1; i++)
    {
    }
    total = count + i;

    /* FAKEMATCH: the count-derived mask is needed to match the native mask
     * in r9 and work in fp; a direct constant swaps them (four instructions).
     * 2026-10-13: cancel = KEY_B differs 12 of 179, KEY_B tested directly
     * 20, cancel = KEY_B set first in the function 48. */
    cancel = ((u32)(u16)count >> 16) | KEY_B;
    /* FAKEMATCH: defining the offset before the loop keeps its load in r1; defining it inside redraw uses r3 (4 instructions differ in all six editions). */
    slotOffset = RENDER_SLOT_VALUE_OFFSET + 2;
    while (1)
    {
        if (gKeysRepeat & KEY_LEFT)
        {
            redraw = 1;
            index--;
        }
        if (gKeysRepeat & KEY_RIGHT)
        {
            redraw = 1;
            index++;
        }
        if (gKeysRepeat & KEY_L)
        {
            redraw = 1;
            index -= 10;
        }
        if (gKeysRepeat & KEY_R)
        {
            redraw = 1;
            index += 10;
        }
        if (gKeysRepeat & KEY_A)
            break;
        if (gKeysRepeat & cancel)
            break;
        if (redraw)
        {
            redraw = 0;
            index = (index + total) % total;
            RenderOutput_PrepareForRedraw(window);
            if (index < count)
                glyph = gPortrait_CharacterMap[index * 2 + 1];
            else
                glyph = gPortrait_SpriteMap[(index - count) * 2 + 1] + 128;
            {
                /* FAKEMATCH: the word temporary is needed to match palette's
                 * r2 through the slot store; direct assignment chooses r3.
                 * 2026-10-13: slot assigned directly with 15 passed as a
                 * literal differs 4 of 179 (the 15 takes r3 after the slot
                 * store); so do the loaded temporary alone, the load
                 * written as an index and the assignment inside the
                 * argument list; pal alone, without loaded, differs 69. */
                s32 pal;
                s32 loaded = *(u16 *)((u8 *)work + slotOffset);
                pal = 15;
                slot = loaded;
                UiGlyph_LoadEntryWithPalette(glyph, 0, &slot, &tile, pal, 1);
            }
            UiText_DrawNumberInWindow(index, 2, window, 0, 0);
            UiText_DrawCharacterAtOffset(index + DEBUG_GLYPH_NAME, window, 24, 0);
        }
        WaitFrames(1);
    }
    UiWork_Finalize(window, 2);
    UiWork_Finalize(portrait, 2);
    WaitFrames(1);
    return 0;
}
