#include "TYPES.H"
#include "SCENE.H"

struct State_0801a4c0 {
    u8 filler0[0x600];
    u16 first;
    u16 second;
    u32 value;
};

extern struct State_0801a4c0 *gGlyphWork;
extern u32 UiIcon_ItemIconPointers[];
void *Item_Get(s32);

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
