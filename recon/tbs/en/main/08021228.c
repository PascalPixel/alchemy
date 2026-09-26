/* Whole 312-byte Djinn join announcement. Candidate 316 bytes, 81 differing
 * halfwords / 31 aligned edits. Initializing the window before the element
 * lookup reproduces its zero-copy chain and r9 lifetime; the 0x980 message
 * base must be a Value_ symbol. Corrected glyph return and six-argument
 * text-queue interfaces from their complete callees.
 * Remaining: zero/element use r8/sl instead of sl/r8; the record's last two
 * stores use r7 instead of sp; the queue emits a sixth-argument zero store
 * absent in the reference (the prior glyph call left that outgoing slot zero).
 * A typed three-word record compiles identically to the word array, so that
 * representation axis is closed. Keep the 32-byte frame and third-arg spill.
 * FAKEMATCH: the initial null window also supplies the shared integer zero. */
#include "TYPES.H"
struct MessageWindow;
struct SpriteRecord {
    u32 flags;
    u32 position;
    u32 tile;
};

extern u8 *Data_03001e8c;
extern s16 Data_080371fe[];
extern u8 Value_00000980[];

struct MessageWindow *UiWindow_Create(s32, s32, s32, s32, s32);
void UiWindow_DrawDividerLine(struct MessageWindow *, s32, s32, s32, s32);
s32 Localization_LookupEntryId(s32);
void UiGlyph_LoadEntryWithPalette(s32, s32, s32 *, s32 *, s32, s32);
void UiWork_PushValueSlot(s32, s32);
s32 UiText_BuildRenderEntriesMode1(s32);
s32 UiText_QueueRenderEntries(struct MessageWindow *, s32, s32, s32, s32, s32);
void Audio_PlayCue(s32);
void Runtime_PushSlotEntry(s32 *, s32);
void WaitFrames(s32);
s32 AudioCommand_GetStateByteFar(void);
void UiWork_Finalize(struct MessageWindow *, s32);
void Resource_ResetEntry(s32);

extern volatile u32 Data_03001c94;

void Djinn_ShowJoinedMessage(s32 p1, s32 p2, s32 p3)
{
    u8 *base = Data_03001e8c;
    s32 tableVal;
    struct MessageWindow *obj;
    struct SpriteRecord buf;
    s32 *display = (s32 *)&buf.flags;
    s32 sp12, sp16;
    s32 d1;
    s32 d2;
    u32 zero;

    obj = NULL;
    zero = (u32)obj;
    tableVal = Data_080371fe[p2 & 3];
    obj = UiWindow_Create(2, 1, 26, 5, (s32)obj);

    if (obj != 0) {
        UiWindow_DrawDividerLine(obj, 4, 0, 4, 4);

        base[0xea3] = 1;

        d1 = Localization_LookupEntryId(tableVal);
        UiGlyph_LoadEntryWithPalette(d1, zero, &sp16, &sp12, 14, zero);

        *display = zero;
        buf.position = 0x8014000c;
        buf.tile = sp12 | 0xe000;

        *(u16 *)(base + 0x12f4) = zero;
        *(u16 *)(base + 0x12f6) = zero;

        UiWork_PushValueSlot(p1, 1);
        UiWork_PushValueSlot(p2 * 20 + p3 + 300, 4);

        d2 = UiText_BuildRenderEntriesMode1(p2 + (s32)Value_00000980);

        UiText_QueueRenderEntries(obj, d2, 36, 2, zero, zero);

        Audio_PlayCue(81);

        do {
            Runtime_PushSlotEntry(display, 250);
            WaitFrames(1);
        } while (AudioCommand_GetStateByteFar() != 0 && (Data_03001c94 & 0x303) == 0);

        UiWork_Finalize(obj, 2);
        WaitFrames(1);
        Resource_ResetEntry(sp16);
    }
}
