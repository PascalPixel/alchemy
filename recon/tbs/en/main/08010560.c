/* 2026-10-04 retained candidate: complete116B score0 with real ten-byte
 * packed, two-byte-aligned command records and a consumed argument cursor.
 * Removed both rejected cursor opacity and redundant stop-input assembly.
 * Remaining steering is confined to sentinel register copies/order.
 * Independent S2 review confirmed all four conversions arise naturally;
 * remaining devices only choose sentinel registers/copy order. Approved
 * compiler/era assembler verify frame8, full116B, zero alignment and pool.
 * Still uncredited pending contiguous MAP.C ownership and all-edition tests.
 */
/* 2026-10-04 packed-record device removal audit:
 * args_from_halfwords: draft: score 0 (exact)
 * remove_entry_barrier: draft: score 105 (1 register-only, 1 deleted)
 * remove_stop_input: draft: score 0 (exact)
 * remove_tail_input: draft: score 105 (1 register-only, 1 deleted)
 * plain_end_test: draft: score 635 (11 register-only, 3 operand, 2 reordered, 2 inserted, 2 deleted)
 * plain_stop: draft: score 30 (6 register-only)
 * Retain exact consumed command-record source; independent review pending.
 */
/* 2026-10-04 explicit five-halfword command layout:
 * packed_aligned2: draft: score 0 (exact)
 * aligned2: draft: score 20 (1 operand)
 * packed: draft: score 1140 (3 register-only, 2 reordered, 8 inserted, 2 deleted)
 * halfword_stride: draft: score 0 (exact)
 * Retain packed_aligned2; independent device review pending.
 */
/* 2026-10-04 independent S2 review rejected the opaque args device:
 * its removal folds two signed-load/conversion chains into unsigned loads,
 * removing six instructions/12B, with no signed-value consumer. The
 * args_early/tail_readwrite score0 remains a rejected trial, never credit.
 */
/* 2026-10-04 source record/cursor trials without cursor opacity:
 * typed_record: draft: score 975 (7 register-only, 7 operand, 2 inserted, 6 deleted)
 * typed_record_late: draft: score 2015 (11 register-only, 5 operand, 1 reordered, 4 inserted, 14 deleted)
 * record_two_cursors: draft: score 20 (1 operand)
 * plain_postloads: draft: score 1150 (6 register-only, 3 operand, 1 reordered, 2 inserted, 8 deleted)
 * plain_local_postloads: draft: score 1005 (17 register-only, 6 operand, 7 inserted, 1 deleted)
 * single_read_cursor: draft: score 1470 (6 register-only, 4 operand, 1 reordered, 4 inserted, 9 deleted)
 * Retain record_two_cursors20, pending independent S2 review. The record
 * owns the source and all four consumed signed fields; no opaque cursor.
 */
/* 2026-10-04 sequence device audit trials:
 * remove_entry_barrier: draft: score 165 (1 register-only, 1 reordered, 1 deleted)
 * remove_stop_input: draft: score 120 (2 reordered)
 * remove_cursor_opacity: draft: score 1210 (6 register-only, 3 operand, 2 reordered, 2 inserted, 8 deleted)
 * remove_tail_input: draft: score 105 (1 register-only, 1 deleted)
 * plain_end_test: draft: score 615 (11 register-only, 2 operand, 2 reordered, 2 inserted, 2 deleted)
 * plain_stop: draft: score 90 (6 register-only, 1 reordered)
 * tail_readwrite: draft: score 0 (exact)
 * tail_load_test: draft: score 110 (2 register-only, 1 inserted)
 * plain_all: draft: score 1465 (9 register-only, 3 operand, 1 reordered, 3 inserted, 10 deleted)
 * Retain tail_readwrite; source-device review still pending.
 */
/* 2026-10-04 consumed-record/cursor trials:
 * record_s32: draft: score 1532 (13 register-only, 2 stack-only, 7 operand, 2 reordered, 9 inserted, 3 deleted)
 * record_s16: draft: score 4947 (13 register-only, 2 stack-only, 7 operand, 7 reordered, 34 inserted, 9 deleted)
 * record_u32: draft: score 1532 (13 register-only, 2 stack-only, 7 operand, 2 reordered, 9 inserted, 3 deleted)
 * load_cursor: draft: score 480 (4 operand, 4 inserted)
 * local_cursor: draft: score 1105 (8 register-only, 5 operand, 1 reordered, 5 inserted, 4 deleted)
 * command_early: draft: score 1130 (6 register-only, 2 operand, 1 reordered, 2 inserted, 8 deleted)
 * args_early: draft: score 60 (1 reordered)
 * both_early: draft: score 60 (1 reordered)
 * Retain args_early at60: advance the consumed cursor after loading its
 * four fields. This naturally retains all signed loads/conversions; only
 * the placement of the final ip snapshot differs from the reference.
 */
/* 2026-10-04 ordinary conversion-preparation order (source_y,width,
 * height,delay), each real u16 conversion assigned before its consumer:
 * 0123:2030; 0132:2030; 0213:2030; 0231:2030; 0312:2030; 0321:2030; 1023:2030; 1032:2030; 1203:2030; 1230:2030; 1302:2030; 1320:2030;
 * 2013:2030; 2031:2030; 2103:2030; 2130:2030; 2301:2030; 2310:2030; 3012:2030; 3021:2030; 3102:2030; 3120:2030; 3201:2030; 3210:2030.
 * No barrier, volatile, discarded load or extra source consumer introduced.
 */
/* 2026-10-04 S2 audit: removing the four-output conversion barrier gives
 * score1130 (6 register,2 operand,1 reordered,2 inserted,8 deleted).
 * Independent source/assembly review confirms that barrier prevented two
 * ldrsh/extension chains from becoming ldrh, retaining six instructions
 * with no intervening source consumer. Its score0 was therefore outside
 * S2's register/order-only permission. Barrier removed; this owner stays
 * uncredited until genuine source lifetimes explain the signed loads.
 * The remaining sentinel/cursor devices require review before adoption. */
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

struct MapCopyCommand {
    u16 source;
    s16 y, width, height, delay;
} __attribute__((packed, aligned(2)));
typedef char MapCopyCommandSize[(sizeof(struct MapCopyCommand) == 10) ? 1 : -1];

void Map_PlayMetatileCopySequence(u16 *command, s32 destination_x, u32 destination_y)
{
    struct MapCopyCommand *record = (struct MapCopyCommand *)command;
    u32 source = record->source;
    /* FAKEMATCH: native r3/ip choice; ordinary register choices score635 after record/cursor reshaping. */
    register u32 end __asm__("r3") = 0xffff;
    /* FAKEMATCH: compare an ip snapshot and retain the entry in r0. */
    register u32 test __asm__("ip") = source;

    /* FAKEMATCH: removing ip snapshot compares in r0 and drops its copy (score105), without field-load changes. */
    __asm__ ("" : "+r" (test), "+r" (end));

    if (test != end) {
        /* FAKEMATCH: native r9 sentinel; ordinary choice scores30 with six register-only differences. */
        register u32 stop __asm__("r9") = end;
        s16 *args = (s16 *)(command + 1);

        while (1) {
            s32 source_y = args[0];
            s32 width = args[1];
            s32 height = args[2];
            s32 delay = args[3];
            args += 5;

            Map_CopyMetatileIndicesRect(source, (u16)source_y,
                          destination_x, destination_y,
                          (u16)width, (u16)height);
            record++;
            WaitFrames((u16)delay);
            source = record->source;
            test = source;
            /* FAKEMATCH: removing post-wait ip snapshot compares in r0 and drops its register copy (score105). */
            __asm__ ("" : "+r" (test));
            if (test == stop)
                break;
        }
    }
}
