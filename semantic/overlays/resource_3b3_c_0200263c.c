typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

/*
 * Resource 3b3 unindexed helper at 0x0200263c (88 bytes incl. pool,
 * 3 calls).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,r6,lr}` + 8-byte stack buffer at 0x0200263c, epilogue `add sp,#8
 * / pop {r5,r6} / pop {r0} / bx r0` at 0x02002680-0x02002686. The
 * trailing pool at 0x02002688-0x02002690 (0x000001f5, 0x0200a0b9,
 * 0x0200ad64 -- same three constants as 0x020020f0 earlier in this
 * overlay) is included per the usual pool rule, immediately followed
 * by the next owner's push {lr} at 0x02002694, already this overlay's
 * row `0x02002694 | 4 calls`, so the span is 0x0200263c-0x02002694,
 * 88 bytes.
 *
 * A close sibling of 0x020020f0: guarded by a check at `a0 + 0x1f5`,
 * install a handler pointer (`0x0200a0b9`) into the fetched object's
 * `+0x6c` field, run a setup call with grid-coordinate conversion, and
 * a final call with a second handler address (`0x0200ad64`). No-op if
 * the guard fails.
 *
 * Raw callee naming.
 */

u8 *Func_02005160();
s32 Func_0200513a();
void Func_020050be();
void Func_020051aa();

void Func_0200263c(u8 *a0)
{
    u8 *obj = Func_02005160(a0);

    if (Func_0200513a(a0 + 0x1f5) != 0) {
        Func_020050be(obj, 5);

        *(u32 *)(obj + 0x6c) = 0x0200a0b9;

        Func_0200513a(20, 14, 1, 1, *(s32 *)(obj + 8) >> 20, *(s32 *)(obj + 16) >> 20);
        Func_020051aa(a0, 0x0200ad64);
    }
}
