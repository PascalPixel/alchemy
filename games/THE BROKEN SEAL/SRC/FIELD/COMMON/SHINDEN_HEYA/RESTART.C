/* The shrine's question after the party falls: restart, or the file screen. */
#include "TYPES.H"
#include "IO_REG.H"
#include "SAVE_STATE.H"
#include "EDITION.H"
#include "FIELD_EVENT.H"
#include "FIELD_SERVICE.H"

extern u8 MsgShindenGreatHealer[];

/* A text cursor record the text-resource routines fill and move. */
struct TextCursor {
    u8 storage[12];
};

extern volatile u32 gKeyState;
extern volatile u32 gKeysRepeat;
extern volatile u32 gFrameTick;
/* How far the cursor bobs right over sixteen two-frame steps. */
extern const s32 ShindenHeya_CursorBob[16];

s32 UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiWork_Finalize(s32 window, s32 mode);
void UiText_DrawResource(s32 message, s32 window, s32 x, s32 y);
void UiTextResource_Initialize(struct TextCursor *cursor, s32 *slot);
void UiTextResource_SetPosition(struct TextCursor *cursor, s32 x, s32 y);
void UiTextResource_Release(s32 slot);

static inline void TextCursor_Initialize(struct TextCursor *cursor, s32 *slot)
{
    /* FAKEMATCH: a direct call lets GCC 2.96 keep the cursor's address in a register across the calls. */
    UiTextResource_Initialize(cursor, slot);
}

static inline void TextCursor_SetPosition(struct TextCursor *cursor, s32 x, s32 y)
{
    /* FAKEMATCH: a direct call lets GCC 2.96 keep the cursor's address in a register across the calls. */
    UiTextResource_SetPosition(cursor, x, y);
}

#if EDITION_INTERNATIONAL
#define QUESTION_X 2
#define QUESTION_WIDTH 25
#define CURSOR_X 24
#else
#define QUESTION_X 7
#define QUESTION_WIDTH 17
#define CURSOR_X 64
#endif

/*
 * Hides the party, fades the screen and asks whether to restart, offering
 * the file screen when no save records exist. Up and down move the cursor
 * between the two answers; A returns the one chosen, 0 or 1.
 */
s32 ShindenHeya_ChooseRestartOption(void)
{
    s32 slot;
    struct TextCursor cursor;
    s32 window;
    s32 text;
    s32 choice;

    Actor_SetPosition(8, 0, 0);
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(10, 0, 0);
    Actor_SetPosition(1, 0, 0);
    Actor_SetPosition(11, 0, 0);
    Actor_SetPosition(12, 0, 0);
    Actor_SetPosition(0, 0, 0);
    ColorBuffer_ApplyTarget(0x10000, 2);
    ColorBuffer_Interpolate(1);
    Event_Wait(1);
    window = UiWindow_Create(QUESTION_X, 7, QUESTION_WIDTH, 5, 1);
    text = (s32)MsgShindenGreatHealer;
    UiText_DrawResource(text, window, 16, 0);
    if (SaveState_CountRecordsExcludingFlagged(1) == 0) {
        UiText_DrawResource(text + 2, window, 16, 16);
    } else {
        UiText_DrawResource(text + 1, window, 16, 16);
    }
    TextCursor_Initialize(&cursor, &slot);
    TextCursor_SetPosition(&cursor, 72, 60);
    choice = 0;
    while ((gKeyState & KEY_A) == 0) {
        s32 x;

        if (gKeysRepeat & KEYS_VERTICAL) {
            choice ^= 1;
        }
        x = ShindenHeya_CursorBob[(gFrameTick >> 1) & 15] + CURSOR_X;
        TextCursor_SetPosition(&cursor, x, (choice << 4) + 60);
        Event_Wait(1);
    }
    UiTextResource_Release(slot);
    UiWork_Finalize(window, 1);
    return choice;
}
