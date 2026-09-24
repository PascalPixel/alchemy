#include "TYPES.H"

struct State_0801a4c0 {
    u8 filler0[0x600];
    u16 first;
    u16 second;
    u32 value;
};

extern struct State_0801a4c0 *gGlyphWork;
extern u32 UiIcon_MiscIconPointers[];
void UiGlyph_DecodeWithHeapRoutines(struct State_0801a4c0 *, u32);

void Ui_PrepareTransferFromTableEntry(u32 index)
{
    struct State_0801a4c0 *state = gGlyphWork;

    state->value = UiIcon_MiscIconPointers[index];
    state->first = 2;
    state->second = 2;
    UiGlyph_DecodeWithHeapRoutines(state, 0);
}
