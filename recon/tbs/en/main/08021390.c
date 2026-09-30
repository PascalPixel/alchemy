/* Draft, not exact (2026-09-26): 256 of 248 bytes, 98 differing halfwords.
   Complete owner [0x08021390, 0x08021488), including its literal pool.
   Recovered from the validated split listing. Remaining: the zero is shared
   across window creation rather than initialized afterward; entry writes
   use the retained pointer instead of stack offsets. An explicit entry
   pointer moves its lifetime before creation but does not close the gap.
   A three-word named display record and a one-word clear-value aggregate
   each compile identically to this draft; neither changes zero sharing
   across the creation call or the later retained-pointer stores. Correcting
   the queue callee's value return and preserving its five explicit arguments
   also leaves this candidate unchanged (56 aligned halfword edits).
   2026-09-29 alchemy permute (seed 1, 3 jobs, 10 minutes): 32,655
   candidates; the best, 485 against 1530, writes icon[0] through an index
   variable holding 0, icon[1] through *(icon + 1) and icon[2] with a (u32)2
   index, which no programmer writes, so the draft keeps its spelling. The
   dump shows why: every array element store computes the record's base into
   a pseudo, and CSE replaces it with the older entry pointer, so all three
   stores go through r7; the reference stores only icon[0] through r7 and
   the other two through sp. Moving entry = icon after the window, after the
   stores or writing *entry for icon[0] scores 1530 to 1665. The 0x1b pool
   word is message 27, named MsgJoinedParty since 2026-09-30. */
#include "TYPES.H"

struct PartyJoinWork {
    u8 unknown_0000[0xea3];
    u8 busy;
    u8 unknown_0ea4[0x12f4 - 0xea4];
    u16 cursor;
    u16 scroll;
};

extern struct PartyJoinWork *gWindowWork;
extern u32 gKeyState;
extern const u8 MsgJoinedParty;

void *UiWindow_Create(s32, s32, s32, s32, s32);
void UiWindow_DrawDividerLine(void *, s32, s32, s32, s32);
s32 Party_LookupCharacterValueByFlag32(u32);
s32 Localization_LookupEntryId(s32);
void UiGlyph_LoadEntryWithPalette(u32, s32, s32 *, s32 *, s32, s32);
void UiWork_PushValueSlot(s32, s32);
s32 UiText_BuildRenderEntriesMode1(s32);
/* FAKEMATCH: the sixth outgoing zero remains from the glyph call. */
s32 UiText_QueueRenderEntries();
void Audio_PlayCue(s32);
void Runtime_PushSlotEntry(void *, s32);
void WaitFrames(s32);
s32 AudioCommand_GetStateByteFar(void);
void UiWork_Finalize(void *, s32);
void Resource_ResetEntry(u32);

void Party_ShowJoinedMessage(s32 member)
{
    struct PartyJoinWork *work;
    void *window;
    s32 tile;
    s32 resource;
    u32 icon[3];
    u32 *entry;
    s32 zero;

    work = gWindowWork;
    entry = icon;
    window = UiWindow_Create(2, 1, 26, 5, 0);
    zero = 0;
    if (window != 0) {
        UiWindow_DrawDividerLine(window, 4, 0, 4, 4);
        work->busy = 1;
        UiGlyph_LoadEntryWithPalette(Localization_LookupEntryId(Party_LookupCharacterValueByFlag32(member)),
            0, &resource, &tile, 14, zero);
        icon[0] = zero;
        icon[1] = 0x8014000c;
        icon[2] = tile | 0xe000;
        work->cursor = zero;
        work->scroll = zero;
        UiWork_PushValueSlot(member, 1);
        UiText_QueueRenderEntries(window, UiText_BuildRenderEntriesMode1((s32)&MsgJoinedParty), 36, 2, zero);
        Audio_PlayCue(81);
        do {
            Runtime_PushSlotEntry(entry, 250);
            WaitFrames(1);
        } while (AudioCommand_GetStateByteFar() != 0 && (gKeyState & 0x303) == 0);
        UiWork_Finalize(window, 2);
        WaitFrames(1);
        Resource_ResetEntry(resource);
    }
}
