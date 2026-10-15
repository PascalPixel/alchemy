/* Near miss: differ 6 of 106, two places. The native builds -1 fully before it copies the window out of r0
 * (movs r7, #1; negs r7, r7; mov r8, r0), and it finishes x (subs r1, r3, #4) before it starts the load of
 * the window row, which then reuses r3 (movs r3, #14; ldrsh r2, [r0, r3]); the draft loads the row first
 * through r4. An empty asm barrier after x orders the two but moves mov r0, r8 below it and keeps r4.
 * The exits must be gotos (as breaks the loop is rotated around its first test), and the pressed keys are
 * read again for each test, here through a volatile view. */
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "uiwindow.h"

struct ScreenWork {
    u8 unk000[0x5a4];
    u8 panel[0x84];
};

void Func_08040c78(void);
void Func_08040cb0(void);
struct UiWindow *Func_08040cc4(void);
void Func_08108048(void *panel, s32 x, s32 y);
void Func_08108040(void *panel, s32 x, s32 y, s32 frames);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
#define PRESSED (((volatile struct InputState *)&gInput)->pressed)

void UiWork_Finalize(struct UiWindow *window, s32 release);

s32 Func_08040d70(s32 index)
{
    struct ScreenWork *work;
    struct UiWindow *window;
    s32 moved;

    Func_08040c78();
    work = Ram_HeapSlots->unkd0[0];
    WaitFrames(1);
    window = Func_08040cc4();
    moved = -1;
    do {
        if (moved != 0) {
            s32 x;
            s32 y;

            index = (index + 5) % 5;
            x = (s16)window->x * 8 - 4;
            y = (index * 3 + (s16)window->y) * 8 + 16;
            if (moved == -1)
                Func_08108048(work->panel, x, y);
            else
                Func_08108040(work->panel, x, y, 3);
            moved = 0;
        }
        WaitFrames(1);
        if (PRESSED & 2) {
            Audio_PlayCue(113);
            index = -1;
            goto done;
        }
        if (PRESSED & 1) {
            Audio_PlayCue(112);
            goto done;
        }
        if (gInput.repeat & 64) {
            index--;
            moved = 1;
            Audio_PlayCue(111);
        }
        if (gInput.repeat & 128) {
            index++;
            moved = 1;
            Audio_PlayCue(111);
        }
    } while (1);
done:
    UiWork_Finalize(window, 2);
    Func_08040cb0();
    WaitFrames(1);
    return index;
}
