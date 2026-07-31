typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 3b3 unindexed helper at 0x02000f58 (116 bytes, 7 calls).
 * Converted from a parked scope note (work/claude/notes/resource_3b3_f58.md)
 * per Vale's steer, drafted alongside its sibling 0x02000ec8 which shares
 * the same out-param call shape against a different helper.
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,r6,r7,lr}`, 48-byte stack frame at 0x02000f58, epilogue `add
 * sp,#48 / pop {r5,r6,r7} / pop {r1} / bx r1` at
 * 0x02000fc4-0x02000fca, immediately followed with no gap by the next
 * owner's push {r5,lr} at 0x02000fcc, already this overlay's row
 * `0x02000fcc` (drafted earlier this session), so the span is exactly
 * 0x02000f58-0x02000fcc, 116 bytes.
 *
 * `Func_02001d54` is an opaque out-param helper: besides its return
 * code it fills two scalar slots (`out20`, `out16`) and a 24-byte
 * record (only bytes +8/+16 of which -- `record[2]`/`record[4]` here
 * -- are ever read back), plus two more scalar out-params
 * (`out12`/`out8`) passed on the stack alongside it. Its exact field
 * semantics are not resolved here (not needed for a semantic draft --
 * every value is threaded through unchanged to the two finishing
 * calls in the same shape the reference uses); a `manual_regions`
 * caller only needs the argument wiring to be faithful, which this is.
 *
 * On success (`Func_02001d54(...) != 0`): combine `out12`/`out8` with
 * `record[2]`/`record[4]` into an x/z pair, run two finishing calls
 * with those plus the raw out-params, then poke the fetched object
 * (`Func_02003a18`, clear bit 1 of `+0x23`) and return 1. On failure,
 * return 0 without touching the object.
 *
 * Raw callee naming.
 */

u8 *Func_02003a7c();
s32 Func_02001d54();
void Func_02003a5c();
void Func_020014d4();
void Func_02003a18();

s32 Func_02000f58(u8 *a0)
{
    u8 *obj = Func_02003a7c(a0);
    s32 out20, out16, out12, out8;
    s32 record[6];

    if (Func_02001d54(a0, &out20, &out16, record, &out12, &out8) == 0) {
        return 0;
    }

    {
        s32 x = out12 + record[2];
        s32 z = out8 + record[4];

        Func_02003a5c(x, z, out20, out16, record[2], record[4]);
        Func_020014d4(0, record[2], record[4], out20, out16, 255);
    }

    Func_02003a18(obj, 1);
    obj[0x23] &= 0xfd;

    return 1;
}
