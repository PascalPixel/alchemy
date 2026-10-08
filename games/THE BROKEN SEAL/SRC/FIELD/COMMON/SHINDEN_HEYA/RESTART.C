#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "SAVE_STATE.H"
#include "EDITION.H"
#include "FIELD_EVENT.H"
#include "FIELD_SERVICE.H"
#include "WINDOW.H"
#include "UIFLOW.H"
#include "text/MSG_IDS.H"

#define gKeyState (*(volatile u32 *)&gKeyState)
#define gKeysRepeat (*(volatile u32 *)&gKeysRepeat)


struct TextCursor {
    u8 storage[12];
};

extern volatile u32 gFrameTick;

void UiText_DrawResource(s32 message, s32 window, s32 x, s32 y);
void UiTextResource_Initialize(struct TextCursor *cursor, s32 *slot);
void UiTextResource_SetPosition(struct TextCursor *cursor, s32 x, s32 y);

static inline void TextCursor_Initialize(struct TextCursor *cursor, s32 *slot)
{
    /* FAKEMATCH: needed to match: a direct call lets GCC 2.96 keep the cursor's address in a register across the calls. */
    UiTextResource_Initialize(cursor, slot);
}

static inline void TextCursor_SetPosition(struct TextCursor *cursor, s32 x, s32 y)
{
    /* FAKEMATCH: needed to match: a direct call lets GCC 2.96 keep the cursor's address in a register across the calls. */
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

s32 ShindenHeya_ChooseRestartOption(void)
{
    s32 slot;
    struct TextCursor cursor;
    s32 window;
    s32 text;
    s32 choice;

    Engine_ActorSetPosition(8, 0, 0);
    Engine_ActorSetPosition(9, 0, 0);
    Engine_ActorSetPosition(10, 0, 0);
    Engine_ActorSetPosition(1, 0, 0);
    Engine_ActorSetPosition(11, 0, 0);
    Engine_ActorSetPosition(12, 0, 0);
    Engine_ActorSetPosition(0, 0, 0);
    Engine_ColorBufferApplyTarget(0x10000, 2);
    Engine_ColorBufferInterpolate(1);
    Engine_EventWait(1);
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
        Engine_EventWait(1);
    }
    UiTextResource_Release(slot);
    UiWork_Finalize(window, 1);
    return choice;
}
