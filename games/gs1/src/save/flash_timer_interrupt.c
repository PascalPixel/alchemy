#include "flash.h"

/* The flash driver's timer tick handler and the installer that arms it. */

extern volatile u16 Data_02004c22;
extern volatile u8 Data_02004c24;
extern volatile u8 Data_02004c20;
extern volatile u32 Data_02004c28;

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
    if (Data_02004c22 != 0) {
        s32 v = Data_02004c22;

        v -= 1;
        Data_02004c22 = v;
        if ((v << 16) == 0) {
            Data_02004c24 = 1;
        }
    }
}

/*
 * Point one of the four hardware timers at the tick handler.  The
 * out-of-range arm is written first, and that source order is part of what
 * reproduces the layout.  Data_02004c20 is read back after its own store to
 * build the register address: 0x04000100 + index * 4 walks the four timer
 * count/reload registers, and Data_02004c28 caches the chosen one.
 */
s32 SetFlashTimerIntr(u8 timerIndex, void (**callback)(void))
{
    if (timerIndex > 3) {
        return 1;
    }
    Data_02004c20 = timerIndex;
    Data_02004c28 = 0x04000100 + (u32)Data_02004c20 * 4;
    *callback = FlashTimerIntr;
    return 0;
}
