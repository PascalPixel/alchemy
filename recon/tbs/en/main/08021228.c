/* DRAFT: whole 312-byte Djinn join announcement, now 312 bytes and six
 * differing halfwords/aligned edits (2026-09-27). The five-argument old-style
 * text-queue call preserves the sixth outgoing zero left by the glyph call.
 * It removes the redundant stack store and fixes the sl/r8 zero/element
 * allocation as well; all other instructions, calls and pools now match.
 * Remaining: the display record's position/tile stores use r7+4/r7+8 instead
 * of sp+24/sp+28, with two nearby scheduling differences. Volatile record
 * fields compile identically. Separate volatile tail scalars give the desired
 * stack stores but move the escaped link and zero to different registers
 * (308 bytes / 54 edits); a one-element link array changes nothing there.
 * Stop the storage axis before more layout permutations.
 * FAKEMATCH: the initial null window also supplies the shared integer zero.
 * 2026-09-29 alchemy permute (seed 1, 4 jobs, 10 minutes): 59,349
 * candidates, none below the draft's score 200 (4 operand, 2 reordered),
 * 36,959 level with it. The 0x980 pool word is message 2432 (the Venus
 * Djinni joining line) plus the element, so it is a message symbol that the
 * catalogs have not named yet; a plain 0x980 becomes movs/lsls (score 555).
 * Data_080371fe needs its own ROM label. Setting the display pointer from
 * &buf, writing the flags through buf, a record pointer, display[0] or a
 * pointer set inside the if all keep the r7+4/r7+8 stores.
 */
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
/* FAKEMATCH: preserve the sixth outgoing zero left by the glyph call. */
s32 UiText_QueueRenderEntries();
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

        UiText_QueueRenderEntries(obj, d2, 36, 2, zero);

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
