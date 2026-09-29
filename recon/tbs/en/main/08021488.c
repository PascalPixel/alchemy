/* 2026-09-29 alchemy permute: score 2985 to 675 on the permuter's scorer
   (0 is exact); remaining 15 register-only, 5 operand, 8 reordered. Kept
   rewrites: 5x swap commutative operands, 4x reorder independent
   statements, 2x introduce a temporary, 2x add a same-width cast, 2x drop
   a same-width cast, 2x toggle register, 1x reorder local declarations, 1x
   move an assignment into or out of a condition. FAKEMATCH: the permuter's
   temporaries, register hints and swapped operand orders below only steer
   allocation and scheduling; no programmer would write them, so they stay
   tagged until a natural spelling replaces them. */
/* Not-yet-C, complete 344-byte pair-joined message and pool.
 * The lookup result is passed directly to Localization_LookupEntryId in both
 * call chains; the former no-argument declaration dropped this dependency.
 * Preserving it yields 352 bytes, a 44-byte frame and 118 aligned halfword
 * edits. The observed Value_0000001d pool load gives 356 bytes / 120 edits.
 * Both queue return types and a five-argument prototype leave this unchanged.
 * The ROM instead keeps zero in r6, spills the second member, uses a 48-byte
 * frame and direct stack stores for the two record tails. This model retains
 * the proven call dependencies; stop until that lifetime difference is known.
 * Earlier 340-byte / 62-edit code omitted those dependencies. Scalar record
 * tails were deleted as unreferenced; reversing records used the wrong slots.
 * No adoption or byte credit. */
#include "TYPES.H"

struct JoinedMessageBox {
    u32 flags;
    u32 position;
    u32 tiles;
};
struct JoinedMessageWork {
    u8 unknown_0000[0xea3];
    u8 message_mode;
    u8 unknown_0ea4[0x450];
    u16 first_count;
    u16 second_count;
};

extern struct JoinedMessageWork *gWindowWork;
extern const u8 Value_0000001d;

s32 UiWindow_Create(s32, s32, s32, s32, s32);
void UiWindow_DrawDividerLine(s32, s32, s32, s32, s32);
s32 Party_LookupCharacterValueByFlag32(u32);
s32 Localization_LookupEntryId(s32);
void UiGlyph_LoadEntryWithPalette(s32, s32, s32 *, s32 *, s32, s32);
void UiWork_PushValueSlot(u32, u32);
s32 UiText_BuildRenderEntriesMode1(s32);
/* FAKEMATCH: the sixth outgoing zero remains from the glyph call. */
s32 UiText_QueueRenderEntries();
void Audio_PlayCue(s32);
void Runtime_PushSlotEntry(void *, s32);
void WaitFrames(s32);
s32 AudioCommand_GetStateByteFar(void);
void UiWork_Finalize(s32, s32);
s32 Resource_ResetEntry(s32);

void Party_ShowPairJoinedMessage(s32 msg0, s32 msg1)
{
    s32 spC;
    register s32 sp10;
    s32 sp14;
    struct JoinedMessageBox second;
    s32 window;
    struct JoinedMessageBox first;
    struct JoinedMessageWork *base;
    struct JoinedMessageBox *box1;
    struct JoinedMessageBox *box2;
    u32 zero;
    s32 tmp;
    register s32 tmp2;

    box1 = &first;
    base = gWindowWork;
    window = 0;
    tmp2 = UiWindow_Create(1, 1, 0x1C, 5, window);
    tmp = tmp2;
    if ((window = tmp) != 0) {
        UiWindow_DrawDividerLine(window, 8, 0, 4, 4);
        zero = 0;
        base->message_mode = 1;
        UiGlyph_LoadEntryWithPalette(Localization_LookupEntryId(Party_LookupCharacterValueByFlag32(msg0)), 0, &sp14, &sp10, 0xE, zero);
        first.flags = zero;
        first.position = 0x800C000C;
        box2 = &second;
        first.tiles = sp10 | 0xE000;
        UiGlyph_LoadEntryWithPalette(Localization_LookupEntryId(Party_LookupCharacterValueByFlag32(msg1)), 0, &spC, &sp10, 0xF, zero);
        second.flags = zero;
        second.position = 0x802C000C;
        second.tiles = sp10 | 0xF000;
        base->first_count = zero;
        base->second_count = zero;
        UiWork_PushValueSlot(msg0, 1);
        UiWork_PushValueSlot(msg1, 1);
        UiText_QueueRenderEntries(window, UiText_BuildRenderEntriesMode1((s32)&Value_0000001d), 0x44, 2, zero);
        Audio_PlayCue(0x51);
    loop_2:
        Runtime_PushSlotEntry(box1, 0xFA);
        Runtime_PushSlotEntry(box2, 0xFA);
        WaitFrames(1);
        if (AudioCommand_GetStateByteFar() != 0) {
            if (!(0x303 & *(s32 *)0x03001C94)) {
                goto loop_2;
            }
        }
        UiWork_Finalize(window, 2);
        WaitFrames(1);
        Resource_ResetEntry(sp14);
        Resource_ResetEntry(spC);
    }
}
