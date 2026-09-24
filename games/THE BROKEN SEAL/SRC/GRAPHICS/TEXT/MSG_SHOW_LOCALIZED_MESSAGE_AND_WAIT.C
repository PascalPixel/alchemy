#include "TYPES.H"
#include "SCENE.H"
#include "TBS_EDITION.H"
#include "SYSTEM.H"

#if defined(TBS_EDITION_JA)
#define TEXT_COUNT 32
#else
#define TEXT_COUNT 52
#endif

s32 UiWindow_Create(s32, s32, s32, s32, s32);
void UiWork_Finalize(struct Work *work, s32 release);

#if defined(TBS_EDITION_JA)
void Ui_Place(s32, s16 *, s32);
#else
s32 Ui_Place(s32, s16 *, s32);
#endif

extern u8 gGameState[];
extern void *gBattleWork;
extern volatile s32 gKeyState;
extern void *gLinkCountdownWork;
extern char Value_00000845;

s32 UiText_ShowLocalizedMessageAndWait(void)
{
    s16 buffer[64];
    s32 work;
    s32 result;
    void *state;
    s32 mode;

    state = gBattleWork;
    mode = gGameState[0x22B];
    if (gGameState[0x22B] == 2 || mode <= 2 || (result = 1, mode > 4)) {
        result = 0;
    }
    if (result == 0) {
        if (*(s8 *)((u8 *)state + 0x43) != 0) {
            result = 1;
        }
        if (result != 0) {
            goto active;
        }
    } else {
active:
        work = UiWindow_Create(0, 7, 30, 4, 42);
        Ui_FillVramBlockPattern();
        Ui_Place((s32)&Value_00000845, buffer, TEXT_COUNT);
        UiText_RenderWideStringAtOffset(buffer, work, 0, 4);
        do {
            WaitFrames(1);
        } while ((gKeyState & 3) == 0 &&
                 *(s32 *)((u8 *)gLinkCountdownWork + 0x4C) != 0);
        UiWork_Finalize(work, 1);
    }
    return result;
}
