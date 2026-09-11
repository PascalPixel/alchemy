#include "scene.h"
#include "flash.h"

/* save/flash_timer_interrupt.c */
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

/* save/stop_flash_timer.c */
/* save/flash_read_routines.c */
/*
 * Retire the two halfwords at the cartridge-interface cursor, mask its
 * interrupt, and install the tiny relocated copy loop that follows it.
 * Clean-room reconstruction from games/gs1/asm/08006a78.s only.
 */
#include "flash.h"

void StopFlashTimer(void)
{
    u16 *volatile *cursor = (u16 *volatile *)0x02004c28;
    u16 *record = *cursor;

    *record = 0;
    record++;
    *cursor = record;
    *record = 0;
    record--;
    *cursor = record;

    *(volatile u16 *)0x04000208 = 0;
    *(volatile u16 *)0x04000200 &= ~(u16)(8 << *(volatile u8 *)0x02004c20);
    *(volatile u16 *)0x04000208 = *(volatile u16 *)0x02004c2c;
}

u8 ReadFlashByte(u8 *value)
{
    return *value;
}

void CopyFlashReadRoutineToRam(void *rawDestination)
{
    u8 *destination = rawDestination;
    u8 *source;
    u32 count;
    u32 begin;
    u16 value;

    *(u8 **)0x02004c1c = destination + 1;
    source = (u8 *)ReadFlashByte;
    source = (u8 *)((u32)source ^ (u32)1);
    begin = ((u32)(u8 *)CopyFlashReadRoutineToRam -
             (u32)(u8 *)ReadFlashByte) << 15;
    count = begin;
    goto check;

loop:
    *(u16 *)destination = *(u16 *)source;
    source += 2;
    destination += 2;
    count = (((count >> 16) - 1) << 16);
check:
    value = count >> 16;
    if (value != 0)
        goto loop;
}

/* save/wait_for_flash_write.c */
/* save/wait_for_flash_write.c */
/*
 * The callee stored at 0x02004c1c is invoked through the compiler's r1
 * indirect-call veneer, so it takes one real argument.  The explicit byte
 * narrowing mirrors the three byte-valued inputs at the call boundary.
 */
typedef s32 (*Callee)(s32 argument);

void StartFlashTimer(s32 timing_index);

s32 WaitForFlashWrite(u8 value, s32 argument, u8 expected)
{
    u32 packed = value;
    u32 narrowed = packed;
    s32 local_argument = argument;
    u32 local_expected = expected;
    s32 result;
    Callee *callee_slot;

    narrowed = (narrowed << 24) >> 24;
    local_expected = (local_expected << 24) >> 24;
    result = 0;
    StartFlashTimer(narrowed);
    callee_slot = (Callee *)0x02004c1c;
    packed = (narrowed | 0xc000) << 16;
    goto loop;

failure:
    if (*(u16 *)(*(u8 **)0x02004c08 + 20) == 0x1cc2)
        *(u8 *)0x0e005555 = 0xf0;
    result = packed >> 16;
    goto done;

loop:
    if ((u8)(*callee_slot)(local_argument) == local_expected)
        goto done;
    if (*(u8 *)0x02004c24 == 0)
        goto loop;
    if ((u8)(*callee_slot)(local_argument)!= local_expected)
        goto failure;

done:
    StopFlashTimer();
    return result;
}

/* save/verify_flash.c */
u8 *VerifyFlashCore(u8 *left, u8 *right, u16 size)
{
    u16 index = size - 1;

    while (index != 0xFFFF) {
        u8 rightByte = *right;
        u8 leftByte = *left;

        left++;
        right++;
        if (rightByte != leftByte) {
            return right - 1;
        }
        index--;
    }
    return 0;
}

/* save/program_flash_byte.c */
/*
 * Program one byte of flash: issue the unlock sequence, store the byte,
 * then call through the word at 0x02004C00.
 */

/*
 * Sys_SetMode names a `bx rN` slot: the call is indirect through the
 * register that slot selects, and the trailing argument is the callee
 * address rather than a parameter of the callee.
 */

/*
 * The word at 0x02004C00 is the address called, not data. The source byte
 * is read again for the call rather than reused from the store.
 */
u16 ProgramFlashByte(u8 *source, u8 *destination)
{
  s32 *status;
  *(volatile u8 *) 0x0E005555 = 0xAA;
  *(volatile u8 *) 0x0E002AAA = 0x55;
  *(volatile u8 *) 0x0E005555 = 0xA0;
  *destination = *source;
  status = (s32 *) 0x02004C00;
  return Sys_SetMode(1, destination, *source, *status);
}

/* save/count_remaining_erased_bytes.c */
/*
 * A global holding a pointer.  Only its field 4, the byte budget, is read
 * here; the object is not otherwise identified.
 */
extern u8 *gOvStopFlashTimer;

/*
 * Count a run of consecutive 0xff bytes starting at ptr, capped by the budget
 * read from the global, and return the budget left when the scan stopped.
 * A leaf with no prologue: the owner runs to its bx lr plus one alignment
 * halfword, and its single pool word sits inline mid-body, skipped by the
 * branch that enters the loop at its test.
 */
s32 CountRemainingErasedFlashBytes(u8 *ptr)
{
    s32 remaining = *(s32 *)(gOvStopFlashTimer + 4);

    while (remaining != 0) {
        u8 byte = *ptr;
        ptr++;
        if (byte != 0xff) {
            break;
        }
        remaining--;
    }

    return remaining;
}

/* save/run_erase_verifier.c */
/* Flash erase verification for the save driver. */

/*
 * Sys_Check is the `bx r1` slot of the __call_via_rN veneer bank at
 * 0x080072e4, so this call is an indirect call through r1 rather than a call
 * to a function at that address.  r1 is never written here: the callee is
 * the pointer the caller left in it, and r0 reaches that callee untouched.
 * The empty parameter list is unverified -- the real arity is not known.
 */
s32 RunFlashEraseVerifier(void)
{
    if (Sys_Check() == 0) {
        return 0;
    }
    return 0x8004;
}
