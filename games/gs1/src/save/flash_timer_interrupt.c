#include "scene.h"
#include "abi/save/flash_timer_interrupt.h"
#include "flash.h"

/* The flash driver's timer tick handler and the installer that arms it. */

extern volatile u16 gOv;
extern volatile u8 gOv2;
extern volatile u8 gOv3;
extern volatile u32 gOv4;

/*
 * Count one tick down and raise the expiry flag on the way through zero.
 * Every named cell is volatile and each read is a separate access: the
 * counter is read again inside the arm rather than reused from the test.
 * The decrement runs through an s32 local that narrows at the store, and the
 * reached-zero test is that local's (v << 16) == 0 rather than a fresh read;
 * a u16 local would not produce the same code.
 */
void FlashTimerIntr(void)
{
    if (gOv != 0) {
        s32 v = gOv;

        v -= 1;
        gOv = v;
        if ((v << 16) == 0) {
            gOv2 = 1;
        }
    }
}

/*
 * Point one of the four hardware timers at the tick handler.  The
 * out-of-range arm is written first, and that source order is part of what
 * reproduces the layout.  gOv3 is read back after its own store to
 * build the register address: 0x04000100 + index * 4 walks the four timer
 * count/reload registers, and gOv4 caches the chosen one.
 */
s32 SetFlashTimerIntr(u8 timerIndex, void (**callback)(void))
{
    if (timerIndex > 3) {
        return 1;
    }
    gOv3 = timerIndex;
    gOv4 = 0x04000100 + (u32)gOv3 * 4;
    *callback = FlashTimerIntr;
    return 0;
}
