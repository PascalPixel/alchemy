#include "scene.h"
#include "flash.h"

/* save/flash_timer_interrupt.c */
/* The flash driver's timer tick handler and the installer that arms it. */

extern volatile u16 gOv;
extern volatile u8 gOv2;
extern volatile u8 gOv3;
extern volatile u8 gOv4;

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
