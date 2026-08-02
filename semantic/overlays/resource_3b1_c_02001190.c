typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

/*
 * Resource 3b1 unindexed helper at 0x02001190 (240 bytes incl. pool,
 * 9 calls).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,r6,r7,lr}` + high-reg (`r8`/`sl`) spill at 0x02001190, epilogue
 * `pop {r3,r5} / mov r8,r3 / mov sl,r5 / pop {r5,r6,r7} / pop {r0} / bx r0`
 * at 0x02001262-0x0200126c. The trailing pool (0x02001270-0x0200127c:
 * -0x4000, 0xcccc, 0x19999, and a table-base address 0x0200e668) ends
 * exactly where the next owner's `push {r5,r6,r7,lr}` begins
 * (0x02001280, already this overlay's row `0x02001280 | 2 calls`), so the
 * span is 0x02001190-0x02001280, 240 bytes.
 *
 * `sl` holds a pointer returned by the very first call
 * (`Func_02007686(0)`), read only at offset +6 (a `u16`) throughout — kept
 * as `subject` below, most plausibly a 16-bit facing angle. Three
 * candidate buckets are computed from `subject[+6]` biased by
 * `+0x4000`/`-0x4000`/`+0x8000` (quarter/half turns) and masked to the top
 * nibble of a 16-bit window (`& 0xF000`, then `>> 12`, giving a 0..15
 * direction bucket); the first two candidates are each validated by a
 * lookup call (`Func_02002454`/`Func_02002474`) and the first one that
 * returns non-zero wins, falling back to the third (unvalidated) bucket
 * if neither does. The table at 0x0200e668 is indexed by the winning
 * bucket and holds packed `{s16 hi; s16 lo;}` pairs read as one 32-bit
 * word — no existing symbol names this table elsewhere in the overlay,
 * so it is declared locally as `BucketTable_0200e668` here.
 *
 * Raw callee naming.
 */

extern u32 BucketTable_0200e668[];

u8 *Func_0808a080();
void Func_0808a1e0();
s32 Func_02001280();
void Func_0808a0f0();
void Func_0808a090();
void Func_0808a100();
void Func_0808a0e0();
void Func_0808a0e8();
void Func_02004880();

static s32 Bucket_02001190(u16 heading)
{
    /* Mask is entirely within the low 16 bits, so the result is always a
     * small non-negative value (0-0xF000) even though the reference does
     * this in a 32-bit register with `asrs`; unsigned shift matches it. */
    return (heading & 0xF000) >> 12;
}

void Func_02001190(u8 *obj)
{
    u8 *subject = Func_0808a080(0);
    u8 *p;
    u16 heading;
    s32 bucket;
    u32 packed;

    Func_0808a1e0(obj, 2, 1);

    p = Func_0808a080(obj);
    p[0x23] |= 1;

    heading = *(u16 *)(subject + 6);
    bucket = Bucket_02001190(heading + 0x4000);
    if (Func_02001280(bucket) == 0) {
        bucket = Bucket_02001190(heading - 0x4000);
        if (Func_02001280(bucket) == 0) {
            bucket = Bucket_02001190(heading + 0x8000);
        }
    }

    {
        u8 *hit = Func_0808a080(0);
        if (hit != 0) {
            Func_0808a0f0(obj, *(u32 *)(hit + 8), *(u32 *)(hit + 16));
        }
    }

    Func_0808a090(obj, 0x19999, 0xcccc);
    Func_0808a100(obj, 2);

    packed = BucketTable_0200e668[bucket];
    Func_0808a0e0(obj, (s16)(packed >> 16), (s16)packed);

    Func_0808a0e8(obj);
    Func_0808a100(obj, 1);
    Func_02004880(obj, heading);
}
