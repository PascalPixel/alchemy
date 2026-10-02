/* 2026-10-02 conversion trial 6: read-write initial ip/r3 snapshots
 * recover their prologue scheduling; complete 116-byte extent scores 0 exact,
 * literal pool and alignment included. Awaiting contiguous module ownership. */
/* 2026-10-02 conversion trial 5: an early input barrier schedules the
 * entry load late too, score 180 (3 reordered); that barrier is removed. */
/* 2026-10-02 conversion trial 4: swapping the initial fixed-register
 * declarations changes their local order but still scores 120; the frame
 * and destination snapshots remain scheduled before both. */
/* 2026-10-02 conversion trial 3: a separate initial r3 sentinel and an
 * opaque independent cursor restore all operands and field-load order;
 * score 120, only the entry sentinel load and ip snapshot are scheduled late. */
/* 2026-10-02 conversion trial 2: ip entry snapshots and an r9 sentinel
 * recover the complete save set; score 335 (5 register, 3 operand,
 * 4 reordered). Residual is sentinel placement, two swapped field loads,
 * and the argument cursor reconstructed from the command cursor. */
/* 2026-10-02 conversion trial 1: a four-operand empty barrier plus a
 * word-width entry preserves all four ldrsh/unsigned-extension pairs; score
 * 805 (9 register, 5 operand, 1 reordered, 1 inserted, 5 deleted). */
/* 2026-10-02 (wave 1, slice 1): still 1530. Two readings of the reference
   for the next attempt. The entry is compared as a full word (ldrh, cmp
   against 0xffff), so it is an int-width variable: a u16 one narrows the
   test to lsls #16 against 0xffff0000. And combine folds a sign-extending
   load into its (u16) conversion unless a store or a call lies between
   the two; the reference keeps all four ldrsh and lsls/lsrs pairs, the
   delay's before the first call, so each conversion follows a store or is
   made where combine cannot reach the load. A volatile argument pointer,
   u16 locals filled from s32 ones, an inline step with u16 parameters and
   a record with s16 fields all score 32-39 rows off. */
/* Not-yet-C: complete 116-byte map-copy sequence, split from 08010000.
 * The original 120-byte candidate differs by 34 aligned halfwords.
 * Widening source to u32 recovers unsigned entry reads but removes the
 * sentinel register and argument extensions (96 bytes / 32 edits).
 * Narrowing all four argument locals gives the same 96-byte output.
 * Neither width model reproduces the signed loads followed by independent
 * unsigned conversions. A signed 16-bit-field argument record with a word
 * sentinel snapshot also gives 96 bytes / 32 edits: the first read is right,
 * but the sentinel copy and the source-y/width extensions still disappear.
 * Original model preserved; stop this record-width axis.
 * 2026-09-29: the permuter took the score from 1620 to 1530 (11
 * register-only, 2 operand, 10 reordered, 4 inserted, 4 deleted) with an
 * exit test at the bottom of an endless loop and the argument pointer
 * advanced before the wait. The reference compares a copy of the entry
 * in ip against 0xffff held in r9 and loads all four fields with ldrsh;
 * s32 or s16 locals, u16 or s32 parameters and an s32 entry all score
 * 1620-1790. */
#include "TYPES.H"

void Map_CopyMetatileIndicesRect(s32 source_x, s32 source_y, u32 destination_x,
                   u32 destination_y, u32 width, u32 height);
void WaitFrames(s32 delay);

void Map_PlayMetatileCopySequence(u16 *command, s32 destination_x, u32 destination_y)
{
    u32 source = *command;
    /* FAKEMATCH: pin the initial sentinel literal to r3. */
    register u32 end __asm__("r3") = 0xffff;
    /* FAKEMATCH: compare an ip snapshot and retain the entry in r0. */
    register u32 test __asm__("ip") = source;

    /* FAKEMATCH: retain the separate ip snapshot and r3 sentinel. */
    __asm__ ("" : "+r" (test), "+r" (end));

    if (test != end) {
        /* FAKEMATCH: retain the loop sentinel in r9. */
        register u32 stop __asm__("r9") = end;
        s16 *args = (s16 *)(command + 1);

        /* FAKEMATCH: keep the loop sentinel in the reference's r9. */
        __asm__ __volatile__ ("" : : "r" (stop));
        /* FAKEMATCH: retain an independent cursor instead of CSE reconstruction. */
        __asm__ ("" : "+r" (args));

        while (1) {
            s32 source_y = args[0];
            s32 width = args[1];
            s32 height = args[2];
            s32 delay = args[3];

            /* FAKEMATCH: combine otherwise folds the four signed loads. */
            __asm__ ("" : "+r" (source_y), "+r" (width),
                "+r" (height), "+r" (delay));
            Map_CopyMetatileIndicesRect(source, (u16)source_y,
                          destination_x, destination_y,
                          (u16)width, (u16)height);
            command += 5;
            WaitFrames((u16)delay);
            source = *command;
            test = source;
            /* FAKEMATCH: keep the sentinel snapshot in ip after the wait. */
            __asm__ ("" : : "r" (test));
            args += 5;
            if (test == stop)
                break;
        }
    }
}
