#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "vram_block.h"
#include "select.h"
#include "types.h"
#include "resource.h"
#include "text/msg_ids.h"



s32 Resource_LoadByMode(s32 mode, s32 value)
{
    s32 output;
    s32 result = -1;

    switch (mode) {
    case 1:
    case 6:
        Ui_BuildPairedPatternsToSlot(value, 0, &result, &output, 0);
        break;
    case 2:
        result = Resource_FindFreeEntry();
        if (result == VRAM_CACHE_ENTRY_COUNT)
            return -1;
        UiIcon_CopyResourceToSlot(value, 0x1a, result);
        break;
    case 9:
        result = Resource_FindFreeEntry();
        if (result == VRAM_CACHE_ENTRY_COUNT)
            return -1;
        RenderResource_LoadTableEntry(value, 0, result);
        break;
    case 4:
        Ability_LoadGlyph(value, 1, &result, &output, 0);
        break;
    }
    return result;
}

s32 Resource_LoadByModeIntoSlot(s32 mode, s32 value, s32 result, s32 option)
{
    s32 output;
    s32 original = result;

    if (result == -1) {
        result = Resource_FindFreeEntry();
        if (result == VRAM_CACHE_ENTRY_COUNT)
            return original;
    }

    switch (mode) {
    case 1:
    case 6:
        Ui_BuildPairedPatternsToSlot(value, option, &result, &output, 1);
        break;
    case 2:
        UiIcon_CopyResourceToSlot(value, 58, result);
        break;
    case 7:
        UiIcon_CopyResourceToSlot(value, 42, result);
        break;
    case 4:
        Ability_LoadGlyph(value, option, &result, &output, 1);
        break;
    case 8:
        Ui_BuildPatternToSlot(value, 0, result);
        break;
    case 9:
        RenderResource_LoadTableEntry(value, 0, result);
        break;
    }

    return result;
}

void MenuSelection_SetupEntry(u32 kind, s32 base, struct SelectionNode *node, s32 reuse)
{
    s32 src;
    s32 tile;
    struct SelectionSprite *obj;

    switch (kind) {
    case 1:
    case 6:
        if (reuse != 0)
            src = node->slot;
        Ui_BuildPairedPatternsToSlot(base, 0, &src, &tile, reuse);
        node->message = base + (s32)MsgCommandName;
        break;
    case 2:
        if (reuse != 0)
            src = node->slot;
        UiIcon_BuildItemIconTiles(base, 1, &src, &tile, reuse);
        node->message = base + (s32)MsgItemName;
        break;
    case 4:
        if (reuse != 0)
            src = node->slot;
        Ability_LoadGlyph(base, 1, &src, &tile, reuse);
        node->message = base + (s32)MsgAbilityName;
        break;
    }

    node->base = base;
    node->slot = src;
    node->tile = tile;
    node->kind = kind;
    node->scale = 0x100;
    node->scale_end = 0x100;

    obj = &node->sprite;
    obj->mode = 0;
    obj->colors = 0;
    obj->mosaic = 0;

    obj->size = 1;
    obj->shape = 0;
    obj->palette = 0;
    obj->tile = tile;
    obj->priority = 0;
}
