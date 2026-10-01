#include "TYPES.H"
#include "SCENE.H"

void VramBlock_LoadCached(void *, s32, void *);

extern u8 RenderResource_PairSourceTable;

void UiGlyph_DecodeWithHeapRoutines(struct State_0801a4c0 *, u32);

struct State_0801a4c0 {
    u8 filler0[0x600];
    u16 first;
    u16 second;
    u32 value;
};

extern struct State_0801a4c0 *gGlyphWork;
extern u32 UiIcon_ItemIconPointers[];
void *Item_Get(s32);
s32 UiIcon_BuildAbilityIconTiles(u8, s32, s32, s32, s32);
void *BattleAction_Get(s32);

s32 RenderResource_LoadTableEntry(u32 value, s32 unused, void *destination)
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
    VramBlock_LoadCached(destination, 32, source);
    return 1;
}

/* FAKEMATCH: four literal slots for the sole table name; include them in the function extent. */
asm(".align 2\n"
    ".LPairSourceFirst:\n.word RenderResource_PairSourceTable\n"
    ".LPairSourceSecond:\n.word RenderResource_PairSourceTable\n"
    ".LPairSourceThird:\n.word RenderResource_PairSourceTable\n"
    ".LPairSourceDefault:\n.word RenderResource_PairSourceTable\n"
    ".size RenderResource_LoadTableEntry, .-RenderResource_LoadTableEntry");

void Ui_PrepareTransferForItem(s32 arg0)
{
    struct State_0801a4c0 *state = gGlyphWork;
    void *result = Item_Get(0x1FF & arg0);

    if (arg0 != 0) {
        state->value = UiIcon_ItemIconPointers[*(u16 *)((u8 *)result + 6)];
    } else {
        state->value = UiIcon_ItemIconPointers[0];
    }
    state->first = 2;
    state->second = 2;
    UiGlyph_DecodeWithHeapRoutines(state, 0);
}

/* 取得項目の+4値を先頭引数として転送する。 */
void Ability_LoadGlyph(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    UiIcon_BuildAbilityIconTiles(
        FIELD_AT_OFFSET(BattleAction_Get(arg0), u8 *, 4),
        arg1,
        arg2,
        arg3,
        arg4);
}
