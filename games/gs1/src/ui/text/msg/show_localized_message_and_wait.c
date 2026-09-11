#include "types.h"
#include "scene.h"
#include "abi/ui/text/msg/show_localized_message_and_wait.h"
#include "gs1_edition.h"

#if defined(GS1_EDITION_JA)
#define TEXT_COUNT 32
#else
#define TEXT_COUNT 52
#endif

void WaitFrames(s32);
s32 UiWindow_Create(s32, s32, s32, s32, s32);
void UiWork_Finalize(struct Work *work, s32 release);

#if defined(GS1_EDITION_JA)

#else

#endif

extern u8 gCell[];
extern void *volatile gBattleWork;
extern volatile s32 gIw;
extern void *gIw2;
extern char Value_00000845;

s32 UiText_ShowLocalizedMessageAndWait(void)
{
    s16 buffer[64];
    s32 work;
    s32 result;
    void *state;
    s32 mode;

    state = gBattleWork;
    mode = gCell[0x22B];
    if (gCell[0x22B] == 2 || mode <= 2 || (result = 1, mode > 4)) {
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
        Ui_Run();
        Ui_Place((s32)&Value_00000845, buffer, TEXT_COUNT);
        Ui_SetMode(buffer, work, 0, 4);
        do {
            WaitFrames(1);
        } while ((gIw & 3) == 0 &&
                 *(s32 *)((u8 *)gIw2 + 0x4C) != 0);
        UiWork_Finalize(work, 1);
    }
    return result;
}
