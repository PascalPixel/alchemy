#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "RESOURCE.H"
#include "VRAM_BLOCK.H"
#include "TYPES.H"
#include "SCENE.H"
#include "GLYPH.H"
#include "ITEM.H"
#include "BATTLE_UNIT.H"
#include "BTLSYS.H"
struct ItemDefinition *Item_GetDirectFar(s32 item);

/* FAKEMATCH: three more names of the one table keep each case's own literal slot; one name folds the switch to a single load. */
extern u8 RenderResource_PairSourceTable[], PairSource1[] __asm__("RenderResource_PairSourceTable"), PairSource2[] __asm__("RenderResource_PairSourceTable"), PairSource3[] __asm__("RenderResource_PairSourceTable");

void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *, s32);
extern GlyphTransfer *gGlyphWork;
extern u8 *UiIcon_ItemIconPointers[];

s32 RenderResource_LoadTableEntry(u32 value, s32 unused, u32 slot)
{
    void *source;
    switch (value) {
    case 1:
        source = PairSource1;
        break;
    case 2:
        source = PairSource2;
        break;
    case 3:
        source = PairSource3;
        break;
    case 0:
    default:
        source = RenderResource_PairSourceTable;
        break;
    }
    VramBlock_LoadCached(slot, 32, source);
    return 1;
}

void Ui_PrepareTransferForItem(s32 code)
{
    GlyphTransfer *state = gGlyphWork;
    struct ItemDefinition *item = Item_GetDirectFar(0x1ff & code);

    if (code != 0) {
        state->encoded = UiIcon_ItemIconPointers[item->icon];
    } else {
        state->encoded = UiIcon_ItemIconPointers[0];
    }
    state->width = 2;
    state->height = 2;
    UiGlyph_DecodeWithHeapRoutines(state, 0);
}

void Ability_LoadGlyph(s32 action, s32 with_base, s32 *slot, s32 *tile, s32 reuse)
{
    UiIcon_BuildAbilityIconTiles(BtlActGet(action)->unknown_04[0],
        with_base, slot, tile, reuse);
}
