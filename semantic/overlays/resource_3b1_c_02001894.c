typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

/*
 * Apply the current world-event selection.
 *
 * The owner starts with `push {lr}` at 0x02001894 and returns through
 * `pop {r0} / bx r0` at 0x0200194e-0x02001950.  The halfword at 0x02001952 is
 * alignment, followed by the nine literal words at 0x02001954-0x02001977;
 * the next owner's `push {lr}` is 0x02001978.  The complete measured span is
 * therefore 0x02001894-0x02001977 (228 bytes).
 *
 * The first test is an unsigned fixed-point comparison: the record state is
 * adjusted by 0xffffe000, shifted into the high halfword, and compared with
 * 0xc0000000.  Keeping the arithmetic in u32 preserves the machine's wrap
 * and unsigned `bls` condition.
 */

extern u8 *Func_0808a080(s32 id);
extern s32 Func_080770c0(s32 flag);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a170(s32 value);
extern void Func_0808a180(s32 value, s32 mode);
extern void Func_080b0010(s32 value);

void Func_02001894(void)
{
    u8 *record = Func_0808a080(0);
    u32 adjustedState = ((u32)*(u16 *)(record + 6) + 0xffffe000u) << 16;

    if (adjustedState > 0xc0000000u) {
        if (Func_080770c0(0x928) != 0 &&
            Func_080770c0(0x93e) == 0) {
            Func_080b0010(17);
        } else {
            Func_080b0010(15);
        }
    } else {
        Func_0808a018();

        if (Func_080770c0(0x93e) != 0) {
            Func_0808a170(0x1f81);
        } else if (Func_080770c0(0x8a0) != 0) {
            Func_0808a170(0x1f48);
        } else if (Func_080770c0(0x925) != 0) {
            Func_0808a170(0x1f7f);
        } else if (Func_080770c0(0x1f7d) != 0) {
            Func_0808a170(0x1f7d);
        } else {
            Func_0808a170(0x1f7b);
        }

        if (Func_080770c0(0x928) != 0 &&
            Func_080770c0(0x93e) == 0) {
            Func_0808a180(17, 0);
        } else {
            Func_0808a180(15, 0);
        }
    }

    Func_0808a020();
}
