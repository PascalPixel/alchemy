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

void Map_CopyMetatileIndicesRect(s32 source_x, s32 source_y, s32 destination_x,
                   u32 destination_y, s32 height, s32 width);
void WaitFrames(s32 delay);

void Map_PlayMetatileCopySequence(u16 *command, s32 destination_x, u32 destination_y)
{
    u16 source = *command;

    if (source != 0xffff) {
        s16 *args = (s16 *)(command + 1);

        while (1) {
            s32 source_y = args[0];
            s32 height = args[1];
            s32 width = args[2];
            s32 delay = args[3];

            Map_CopyMetatileIndicesRect(source, (u16)source_y,
                          destination_x, destination_y,
                          (u16)height, (u16)width);
            command += 5;
            args += 5;
            WaitFrames((u16)delay);
            source = *command;
            if (source == 0xffff)
                break;
        }
    }
}
