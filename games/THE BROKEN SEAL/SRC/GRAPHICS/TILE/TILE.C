#include "RESOURCE.H"
#include "VRAM_BLOCK.H"
#include "TYPES.H"
#include "SCENE.H"
#include "GLYPH.H"
#include "ITEM.H"
#include "BATTLE_UNIT.H"


extern u8 RenderResource_PairSourceTable;

void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *, s32);
extern GlyphTransfer *gGlyphWork;
extern u8 *UiIcon_ItemIconPointers[];
void UiIcon_BuildAbilityIconTiles(u32, s32, s32 *, s32 *, s32);
struct BattleAction *BattleAction_Get(s32);

s32 RenderResource_LoadTableEntry(u32 value, s32 unused, u32 slot)
{
    void *source;
    switch (value) {
    case 1:
        /* FAKEMATCH: plain C folds the switch; retain each case's literal load. */
        asm volatile("ldr %0, .LPairSourceFirst" : "=r"(source));
        break;
    case 2:
        /* FAKEMATCH: retain the second case's separate literal load. */
        asm volatile("ldr %0, .LPairSourceSecond" : "=r"(source));
        break;
    case 3:
        /* FAKEMATCH: retain the third case's separate literal load. */
        asm volatile("ldr %0, .LPairSourceThird" : "=r"(source));
        break;
    case 0:
    default:
        /* FAKEMATCH: retain the default case's separate literal load. */
        asm volatile("ldr %0, .LPairSourceDefault" : "=r"(source));
        break;
    }
    VramBlock_LoadCached(slot, 32, source);
    return 1;
}

/* FAKEMATCH: four literal slots for the sole table name; include them in the function extent. */
asm(".align 2\n"
    ".LPairSourceFirst:\n.word RenderResource_PairSourceTable\n"
    ".LPairSourceSecond:\n.word RenderResource_PairSourceTable\n"
    ".LPairSourceThird:\n.word RenderResource_PairSourceTable\n"
    ".LPairSourceDefault:\n.word RenderResource_PairSourceTable\n"
    ".size RenderResource_LoadTableEntry, .-RenderResource_LoadTableEntry");

void Ui_PrepareTransferForItem(s32 code)
{
    GlyphTransfer *state = gGlyphWork;
    struct ItemDefinition *item = Item_Get(0x1ff & code);

    if (code != 0) {
        state->encoded = UiIcon_ItemIconPointers[item->icon];
    } else {
        state->encoded = UiIcon_ItemIconPointers[0];
    }
    state->width = 2;
    state->height = 2;
    UiGlyph_DecodeWithHeapRoutines(state, 0);
}

/* 取得項目の+4値を先頭引数として転送する。 */
void Ability_LoadGlyph(s32 action, s32 with_base, s32 *slot, s32 *tile, s32 reuse)
{
    UiIcon_BuildAbilityIconTiles(BattleAction_Get(action)->unknown_04[0],
        with_base, slot, tile, reuse);
}
