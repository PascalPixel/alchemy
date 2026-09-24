#include "TYPES.H"
#include "DMA.H"

/* menu/character_selector_run_rearrange.c */
struct CharacterSelectWork {
    u8 padding0[8];
    u32 owner;
    u8 padding0c[4];
    s32 object;
    u8 padding14[8];
    s8 cursor;
    u8 padding1d;
    s8 count;
    u8 padding1f[0x10c - 0x1f];
    void *window;
    u8 padding110[0x144 - 0x110];
    u16 frames[8];
    u8 padding154[0x208 - 0x154];
    u16 owners[9];
    u8 choice;
    u8 padding21b[0x220 - 0x21b];
    u16 page;
    u8 padding222[0x234 - 0x222];
    u16 slot_x[4];
    u16 slot_y[4];
};

/* The help lines are messages 0xB0D (Rearrange), 0xB16 (Djinn list) and
 * 0xB0A (Details); the first and last share one message-base symbol. */
extern u8 Value_00000b00[];
extern volatile u32 gKeyState;
extern volatile u32 gKeysRepeat;

u8 *Owner_GetStateFar(s32 owner);
void Palette_LightenBankHighlight(s32 bank);
void RenderOutput_RedrawSavedRectFar(void *window);
void UiText_DrawCharacterAtOffsetFar(s32 message, void *window, s32 x, s32 y);
s32 GameFlag_TestFar(s32 flag);
s32 Math_Mod(s32 value, s32 divisor);
void Menu_CreateWindowAndEntryObjects(s32 owner, s32 frame);
void PsynergyMenu_CallIconRoutineWithValue(void *work, s32 value);
void UiMenu_PositionCursor(s32 x, s32 y);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
s32 CharacterSelector_MoveEntry(s32 cursor, s32 forward);
void Menu_ReleaseEntryObjects(void);
s32 PsynergyMenu_InitializeEntryObjects(s32, s32, s32, s32, s32);
void Func_080a7850(void);

/* Pick a party member, rearranging the order with L and R; returns 1 when
 * one was chosen, -1 when cancelled. */
s32 CharacterSelector_RunRearrange(void)
{
    struct CharacterSelectWork *work;
    s32 cursor;
    s32 count;
    s32 page;
    s32 redraw;
    s32 result;
    s32 i;

    work = *(struct CharacterSelectWork **)0x03001f2c;
    cursor = work->cursor;
    count = work->count;
    redraw = 1;
    result = 0;
    page = work->page;
    Owner_GetStateFar(work->owners[cursor]);
    for (i = 0; i < 4; i++) {
        work->slot_x[i] = 130 + i * 32;
        work->slot_y[i] = 0x80;
    }
    Palette_LightenBankHighlight(14);
    Dma_Set((void *)0x05000200, (void *)0x05000000, 0x80000010, (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x050001c8, (void *)0x0500001c, 0x80000001, (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x05000200, (void *)0x05000020, 0x80000010, (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x050001e8, (void *)0x0500003c, 0x80000001, (volatile u32 *)0x040000d4);
    while (!GameFlag_TestFar(0x150)) {
        if (redraw) {
            redraw = 0;
            RenderOutput_RedrawSavedRectFar(work->window);
            UiText_DrawCharacterAtOffsetFar((s32)&Value_00000b00[0xd], work->window, 0, 0);
            if (GameFlag_TestFar(48))
                UiText_DrawCharacterAtOffsetFar(0xb16, work->window, 0, 16);
            UiText_DrawCharacterAtOffsetFar((s32)&Value_00000b00[0xa], work->window, 0, 8);
            cursor = Math_Mod(cursor + count, count);
            Owner_GetStateFar(work->owners[cursor]);
            page = Math_Mod(page + 3, 3);
            Menu_CreateWindowAndEntryObjects(work->owners[cursor], page);
            PsynergyMenu_CallIconRoutineWithValue(work, work->owners[cursor]);
            for (i = 7; i >= 0; i--)
                work->frames[i] = 30;
            work->frames[cursor] = 26;
        }
        UiMenu_PositionCursor(cursor * 24 - 10, 16);
        WaitFrames(1);
        if (gKeyState & 1) {
            Audio_PlayCue(0x70);
            result = 1;
            break;
        }
        if (gKeyState & 2) {
            Audio_PlayCue(0x71);
            result = -1;
            break;
        }
        if (gKeysRepeat & 0x100) {
            if (CharacterSelector_MoveEntry(cursor, 1)) {
                Audio_PlayCue(0x70);
                cursor++;
                Menu_ReleaseEntryObjects();
                PsynergyMenu_InitializeEntryObjects(work->object, 2, 2, 8, 0);
                for (i = 7; i >= 0; i--)
                    work->frames[i] = 30;
                work->frames[cursor] = 26;
            } else {
                Audio_PlayCue(0x72);
            }
            WaitFrames(1);
        } else if (gKeysRepeat & 0x200) {
            if (CharacterSelector_MoveEntry(cursor, 0)) {
                Audio_PlayCue(0x70);
                cursor--;
                Menu_ReleaseEntryObjects();
                PsynergyMenu_InitializeEntryObjects(work->object, 2, 2, 8, 0);
                for (i = 7; i >= 0; i--)
                    work->frames[i] = 30;
                work->frames[cursor] = 26;
            } else {
                Audio_PlayCue(0x72);
            }
            WaitFrames(1);
        } else if ((gKeyState & 4) && GameFlag_TestFar(48)) {
            Func_080a7850();
            redraw = 1;
        } else {
            if (gKeysRepeat & 0x20) {
                Audio_PlayCue(0x6f);
                if (count > 1) {
                    cursor--;
                    redraw = 1;
                }
            }
            if (gKeysRepeat & 0x10) {
                Audio_PlayCue(0x6f);
                if (count > 1) {
                    redraw = 1;
                    cursor++;
                }
            }
        }
    }
    work->cursor = cursor;
    work->owner = work->owners[cursor];
    work->choice = work->owners[cursor];
    return result;
}
