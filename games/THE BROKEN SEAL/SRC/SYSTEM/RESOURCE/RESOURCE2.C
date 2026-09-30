#include "TYPES.H"
#include "RESOURCE.H"

extern void Ui_BuildPairedPatternsToSlot(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3, s32 arg4);
extern s32 UiIcon_CopyResourceToSlot(s32 arg0, s32 arg1, s32 arg2);
extern s32 RenderResource_LoadTableEntry(u32 value, s32 unused, void *destination);
extern void Ability_LoadGlyph(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern s32 Ui_BuildPatternToSlot(s32 arg0, s32 arg1, s32 arg2);

struct Object_0801c0dc {
    u8 filler0[5];
    u8 field_50 : 2;
    u8 field_52 : 2;
    u8 field_54 : 1;
    u8 field_55 : 1;
    u8 field_56 : 2;
    u8 filler6;
    u8 field_70 : 1;
    u8 field_71 : 5;
    u8 field_76 : 2;
    u16 field_80 : 10;
    u16 field_8a : 2;
    u16 field_8c : 4;
};

struct ListNode {
    u8 unk_00[8];
    s16 base;
    s16 kind;
    u16 src;
    s16 tile;
    u8 unk_10[16];
    s16 end;
    s16 field_22;
    u8 unk_24[2];
    s16 field_26;
    struct Object_0801c0dc obj;
};

void Ui_BuildPairedPatternsToSlot(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3, s32 arg4);
void UiIcon_BuildItemIconTiles(u32 glyph, s32 includeBase, s32 *sourceIndex,
                   s32 *result, s32 reuseSource);
void Ability_LoadGlyph(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern u8 MsgCommandName;
extern u8 MsgItemName;
extern u8 MsgAbilityName;

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
        if (result == 0x60)
            return -1;
        UiIcon_CopyResourceToSlot(value, 0x1a, result);
        break;
    case 9:
        result = Resource_FindFreeEntry();
        if (result == 0x60)
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
        if (result == 0x60)
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
        Ability_LoadGlyph(value, option, (s32)&result, (s32)&output, 1);
        break;
    case 8:
        Ui_BuildPatternToSlot(value, 0, result);
        break;
    case 9:
        RenderResource_LoadTableEntry(value, 0, (void *)result);
        break;
    }

    return result;
}

/* Sprite record setup for the menu selection list. */

/*
 * Load the graphic for kind through one of three tile loaders, record the
 * tile base, source slot and returned tile index in the node, then reset the
 * node's render object.  The object is reached through a pointer so that
 * every byte access stays within Thumb's 5-bit offset range from one base
 * register.  The per-kind tile counts are link-time constants; an integer
 * literal cannot produce 0x1f here.
 */
void MenuSelection_SetupEntry(u32 kind, s32 base, struct ListNode *node, s32 reuse)
{
    s32 src;
    s32 tile;
    struct Object_0801c0dc *obj;

    switch (kind) {
    case 1:
    case 6:
        if (reuse != 0)
            src = node->src;
        Ui_BuildPairedPatternsToSlot(base, 0, &src, &tile, reuse);
        node->end = base + (s32)&MsgCommandName;
        break;
    case 2:
        if (reuse != 0)
            src = node->src;
        UiIcon_BuildItemIconTiles(base, 1, &src, &tile, reuse);
        node->end = base + (s32)&MsgItemName;
        break;
    case 4:
        if (reuse != 0)
            src = node->src;
        Ability_LoadGlyph(base, 1, (s32)&src, (s32)&tile, reuse);
        node->end = base + (s32)&MsgAbilityName;
        break;
    }

    node->base = base;
    node->src = src;
    node->tile = tile;
    node->kind = kind;
    node->field_22 = 0x100;
    node->field_26 = 0x100;

    obj = &node->obj;
    obj->field_52 = 0;
    obj->field_55 = 0;
    obj->field_54 = 0;
    /*
     * field_76 = 1 stays before field_56 = 0: both mask with 0x3f, and the
     * mask is copied before the byte-7 store because the byte-5 store still
     * needs it.
     */
    obj->field_76 = 1;
    obj->field_56 = 0;
    obj->field_8c = 0;
    obj->field_80 = tile;
    obj->field_8a = 0;
}
