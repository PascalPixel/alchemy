#include "types.h"

/*
 * resource_3bb owner at 0x020004c0, 364 bytes (0x020004c0-0x0200062b):
 * 342 bytes of code, two alignment bytes at 0x02000616, and the five-word
 * literal pool at 0x02000618.
 *
 * Prologue at 0x020004c0 saves r5, r6, r7, lr and then fp, sl, r9 and r8
 * before `sub sp, #4`; the matching epilogue at 0x02000604 restores all of
 * them and ends `pop {r0} / bx r0`, so r0 holds the popped return address and
 * the owner is void.  The single frame word and fp are the two components of
 * the subject's displacement, only one of which is ever non-zero.
 *
 * Three arguments: r0 is a participant id, r1 and r2 are coordinates.  Both
 * coordinates are used signed - r1 is halved with the `lsrs #31 / adds /
 * asrs #1` round-toward-zero idiom - and both are also scaled by << 19.
 *
 * All 23 `bl` sites are placed and reach 11 distinct callees.  The inventory
 * row records calls=22; that count predates the corrected branch decoding.
 * Targets come from cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- (target offset = stored
 * displacement + 2), never from the disassembler's annotations, which print
 * each repeated callee as a different function.  0x3f50 -> Func_0808a080
 * (twice), 0x3f38 -> Func_0808a018, 0x3f98 -> Func_0808a100 (twice), 0x3f30
 * -> Func_0808a010 (three), 0x3e48 -> Func_08009080 (four), 0x3e78 ->
 * Func_08009150 (twice), 0x3dd8 -> Func_08000148, 0x3e60 -> Func_080090e0,
 * 0x3f60 -> Func_0808a090, 0x40b0 -> Func_080f9010 (three), 0x3e80 ->
 * Func_08009158 (twice), 0x3f40 -> Func_0808a020.
 *
 * The pool word 0x02000240 is below the overlay's 0x02008000 link base (base
 * witnessed by 0x02008715 = Func_02000714 + 1 in the byte-exact sibling
 * assets/code/resource_3bb_c_02000950.c), so it is a RAM global holding the
 * active subject id at +500 (250 << 1).  0x0200c0c0 is inside the link band
 * and is therefore in-image data at file offset 0x40c0: a four-entry byte
 * table of poses, indexed by the subject's facing halfword at +6 biased by
 * 0x1000, masked to 0xe000 and shifted down 14.  That mask leaves the index
 * non-negative, so the `bge` sign correction by 0x3fff in front of the shift
 * can never fire; it is reproduced anyway because it is in the instruction
 * stream.
 *
 * Shape: a two-actor swap-in.  The named participant is moved to the argument
 * coordinates and posed from the facing table; the active subject then steps
 * to meet it, along x when the participant's x is not already at r1 / 2 and
 * along z otherwise, in each case by half the remaining gap.  The 0xccc
 * lookup through Func_08000148 fetches a pointer at +480 (240 << 1) of the
 * returned record and hands it to Func_080090e0 with the participant.
 *
 * Uncertainties: only the record fields at +6, +8, +16, +48 and +52 are
 * asserted.  Both Func_08009150 calls pass 0 as their third (y) argument,
 * unlike the sibling owner Func_02000310, which passes the record's own +12;
 * that is what the instructions do.  What Func_08000148's record is, and what
 * lives at its +480, is not established here.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
u8 *Func_02004436();
u8 *Func_0200443e();
void Func_0200447c();
void Func_020044e4();
void Func_02004482();
void Func_020043ba();
void Func_020043f6();
void Func_020044b4();
void Func_02004524();
u8 *Func_0200436c();
void Func_02004402();
void Func_0200450e();
void Func_020043fe();
void Func_02004440();
void Func_0200467e();
void Func_02004454();
void Func_02004424();
void Func_02004462();
void Func_0200469a();
void Func_020046a0();
void Func_0200452e();
void Func_02004542();

                     

                     

                     

                     

/* In-image pose table at file offset 0x40c0, four entries. */
extern u8 Data_0200c0c0[];

void Func_020004c0(s32 participant, s32 x, s32 z)
{
    u8 *subject;
    u8 *record;
    u8 *fetched;
    s32 subject_id;
    s32 facing;
    s32 target_x;
    s32 target_z;
    s32 step_x;
    s32 step_z;

    s32 permuted_0;
    subject = Func_02004436(subject_id);
    record = Func_0200443e(participant);
    subject_id = *(s32 *)(0x02000240 + 500);

    /* (facing + 0x1000) & 0xe000 - the biased octant, kept for the pose. */
    facing = ((*(u16 *)(subject + 6) + 0x1000) & 0xe000);

    target_x = x << 19;
    target_z = z << 19;

    if ((*(s32 *)(record + 8) >> 20) != (x / 2)) {
        permuted_0 = (target_x - *(s32 *)(record + 8)) / 2;
    } else {
        step_z = 0;
        step_x = 0;
        step_x  = permuted_0;
        step_z = (target_z - *(s32 *)(record + 16)) / 2;
    }

    Func_0200447c();
    Func_020044e4(subject_id, 8);
    Func_02004482(6);

    *(s32 *)(record + 52) = 0x3333;
    *(s32 *)(record + 48) = 0x8000;             /* 128 << 8 */

    Func_020043ba(record, Data_0200c0c0[facing >> 14]);
    Func_020043f6(record, target_x, 0, target_z);

    Func_020044b4(6);
    Func_02004524(subject_id, 2);

    fetched = Func_0200436c(27, 0xccc);
    Func_02004402(*(s32 *)(fetched + 480), record);     /* 240 << 1 */

    Func_0200450e(subject_id, 0x8000, 0x3333);
    Func_020043fe(subject, 2);
    Func_02004440(subject,
                  0,
                  *(s32 *)(subject + 8) + step_x,
                  *(s32 *)(subject + 16) + step_z);

    Func_0200467e(239);
    Func_02004454(subject);
    Func_02004424(subject, 1);
    Func_02004462(record);
    Func_0200469a(288);                         /* 144 << 1 */
    Func_020046a0(213);
    Func_02004440(record, 1);
    Func_0200452e(15);
    Func_02004542();
}
