typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

/*
 * Resource 3b1 unindexed helper at 0x02001804 (144 bytes incl. one-word
 * pool, 14 calls).
 *
 * Derived span: no inventory row (item 28's unindexed population, found
 * via overlay_unindexed.ts's reference resolution). `push {r5,r6,r7,lr}`
 * at 0x02001804, epilogue `pop {r5,r6,r7} / pop {r0} / bx r0` at
 * 0x0200188a-0x0200188e. The trailing pool word at 0x02001890
 * (0x03001ebc, the workspace pointer used elsewhere in this overlay, see
 * assets/code/resource_3b1_c_02000b84.c / 02000cc8.c) is included per the
 * usual rule (last instruction's own pool load), immediately followed by
 * the next owner's `push {lr}` at 0x02001894.
 *
 * Raw callee naming. All three arguments pass straight through unmodified
 * to Func_02007cd6.
 */

extern u8 *Data_03001ebc;

void Func_0808a018();
void Func_0808a170();
void Func_0808a178();
s32 Func_0808a070();
void Func_0200486c();
void Func_0808a100();
u8 *Func_0808a080();
u8 *Func_0808a0b8();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_080770c8();
void Func_0808a020(void);

void Func_02001804(s32 a0, s32 a1, s32 a2)
{
    Func_0808a018(a0, a1, a2);
    Func_0808a170(a1);
    Func_0808a178(a0, 0);

    if (Func_0808a070(0, 0) != 0) {
        u16 *slot = (u16 *)(Data_03001ebc + 472);
        (*slot)++;
        Func_0200486c(a0);
    } else {
        u8 *obj;

        Func_0808a100(a0);
        Func_0808a080(a0, 2);
        obj = Func_0808a0b8(0);
        if (obj != 0) {
            s16 v1 = *(s16 *)(obj + 10);
            s16 v2 = *(s16 *)(obj + 18);
            Func_0808a0e8(a0, v1, v2);
        }
        Func_0808a0f0(a0);
        Func_080770c8(a0, 0, 0);
        Func_080770c8(0x300);
        Func_0200486c(a2);
    }

    Func_0808a020();
}
