#include "scene.h"
#include "flash.h"

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
extern u8 *gOv;

/*
 * Count a run of consecutive 0xff bytes starting at ptr, capped by the budget
 * read from the global, and return the budget left when the scan stopped.
 * A leaf with no prologue: the owner runs to its bx lr plus one alignment
 * halfword, and its single pool word sits inline mid-body, skipped by the
 * branch that enters the loop at its test.
 */
s32 CountRemainingErasedFlashBytes(u8 *ptr)
{
    s32 remaining = *(s32 *)(gOv + 4);

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
