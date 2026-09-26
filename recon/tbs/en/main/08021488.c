/* Draft: complete 344-byte Party_ShowPairJoinedMessage, 64 differing
 * halfwords. Corrected value-flow prototypes and stack order close the
 * prior 304-byte draft. Remaining: zero lifetime, pointer-derived icon
 * stores instead of stack offsets, and setup scheduling. Pair/union views
 * did not close the residual; original attempt is retained in history.
 */
#include "TYPES.H"

struct PartyJoinWork {
    u8 unknown_0000[0xea3];
    u8 busy;
    u8 unknown_0ea4[0x12f4 - 0xea4];
    u16 cursor;
    u16 scroll;
};

extern struct PartyJoinWork *Data_03001e8c;
extern u32 gKeyState;
extern const u8 Value_0000001d;

void *UiWindow_Create(s32, s32, s32, s32, s32);
void UiWindow_DrawDividerLine(void *, s32, s32, s32, s32);
s32 Party_LookupCharacterValueByFlag32(u32);
s32 Localization_LookupEntryId(s32);
void UiGlyph_LoadEntryWithPalette(u32, s32, s32 *, s32 *, s32, s32);
void UiWork_PushValueSlot(s32, s32);
s32 UiText_BuildRenderEntriesMode1(s32);
void Func_080165d8(void *, s32, s32, s32, s32);
void Audio_PlayCue(s32);
void Runtime_PushSlotEntry(void *, s32);
void WaitFrames(s32);
s32 AudioCommand_GetStateByteFar(void);
void UiWork_Finalize(void *, s32);
void Resource_ResetEntry(u32);

union PartyJoinIcon {
    struct { u32 unused; u32 position; u32 tile; } fields;
    u32 words[3];
};

void Party_ShowPairJoinedMessage(s32 left, s32 right)
{
    struct PartyJoinWork *work;
    void *window;
    s32 right_resource;
    s32 tile;
    s32 left_resource;
    union PartyJoinIcon right_icon;
    union PartyJoinIcon left_icon;
    u32 *left_entry;
    u32 *right_entry;
    s32 zero;

    work = Data_03001e8c;
    left_entry = left_icon.words;
    window = UiWindow_Create(1, 1, 28, 5, 0);
    zero = 0;
    if (window != 0) {
        UiWindow_DrawDividerLine(window, 8, 0, 4, 4);
        work->busy = 1;
        UiGlyph_LoadEntryWithPalette(Localization_LookupEntryId(Party_LookupCharacterValueByFlag32(left)),
            0, &left_resource, &tile, 14, zero);
        left_entry[0] = zero;
        left_icon.fields.position = 0x800c000c;
        left_icon.fields.tile = tile | 0xe000;
        right_entry = right_icon.words;
        UiGlyph_LoadEntryWithPalette(Localization_LookupEntryId(Party_LookupCharacterValueByFlag32(right)),
            0, &right_resource, &tile, 15, zero);
        right_entry[0] = zero;
        right_icon.fields.position = 0x802c000c;
        right_icon.fields.tile = tile | 0xf000;
        work->cursor = zero;
        work->scroll = zero;
        UiWork_PushValueSlot(left, 1);
        UiWork_PushValueSlot(right, 1);
        Func_080165d8(window, UiText_BuildRenderEntriesMode1((s32)&Value_0000001d), 68, 2, zero);
        Audio_PlayCue(81);
        do {
            Runtime_PushSlotEntry(left_entry, 250);
            Runtime_PushSlotEntry(right_entry, 250);
            WaitFrames(1);
        } while (AudioCommand_GetStateByteFar() != 0 && (gKeyState & 0x303) == 0);
        UiWork_Finalize(window, 2);
        WaitFrames(1);
        Resource_ResetEntry(left_resource);
        Resource_ResetEntry(right_resource);
    }
}
