#include "TYPES.H"

/* Whole leaf owner [080b2720, 080b2764), 68 bytes, including both pool
   words. Shop_Run passes a 16-bit stock list and consumes the returned
   count. The adjacent exact row functions establish signed 33-halfword
   table rows: entries 0..23 are copied until zero, entry 32 is its type.
   2026-09-26 H1: make only the copy load volatile, not the source pointer.
   Candidate 68/68 bytes, 25 differing halfwords, 21 aligned edits.
   This admits the required LDRH copy / LDRSH sentinel invariant, which
   the old all-volatile pointer could not emit. The shared named sentinel
   now occupies r4, displacing count/destination to r0/r1; pool order is
   reversed and the return move is missing. No credit or registration.
   H2: give the entry and loop sentinel tests separate expression
   lifetimes instead of one named value.  68/68 bytes, six differing
   halfwords (six aligned edits): all instructions/register roles now
   match except the two pool offsets and reversed pool words.  The
   signed-read invariant remains exact; the final zero is still an
   SI link-address value rather than the reference's short-reach value.
   Earlier bounded work follows; do not repeat its all-volatile axis.
   Draft: a separate signed sentinel value removes the extra saved register,
   but the loop still uses ldrh and a left shift instead of ldrsh; counter
   and pointer registers differ. Value_00000000 restores the word pool load.
   A volatile source suppresses the carried read, but keeps ldrh/lsl, exchanges
   the counter and destination registers, and reverses the two pool words.
   Volatile candidate: 68 bytes, 19 differing halfwords; three hypotheses stop. */

extern s16 EventTable_AbilityLoadouts[][33];
extern u8 Value_00000000;

s32 EventTable_CopyRowHeader(s32 row_no, s16 *output)
{
    s16 *src;
    s16 *dst;
    s32 count;
    count = 0;
    if (EventTable_AbilityLoadouts[row_no][0] != 0) {
        dst = output;
        src = EventTable_AbilityLoadouts[row_no];
        do {
            /* FAKEMATCH: only the copy read is volatile, so its lifetime
               stays separate from the signed sentinel read. */
            *dst = *(volatile s16 *)src;
            count++;
            src++;
            dst++;
            if (count > 23)
                break;
        } while (*src != 0);
    }
    output[count] = (s32)&Value_00000000;
    return count;
}
