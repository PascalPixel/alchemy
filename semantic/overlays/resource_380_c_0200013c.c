typedef int s32;
typedef short s16;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

/*
 * resource_380 owner at 0x0200013c, 472 bytes: the overlay's general
 * object spawner.  Eight arguments (four in registers, four on the
 * stack); creates an object whose template id comes from the parameter
 * block when flag 0x100000 is set (s16 at params+24) and defaults to
 * 222 otherwise, wires its animation slot ((flags+1) & 15), binds the
 * per-slot descriptor from the table at 0x0200cbc4, installs the
 * resident step callback 0x02008105 at +108, seeds +68/+72/+76 from
 * the a3/a4/a5 arguments, and copies palette bits (record +9, mask 12)
 * from the Func_02004b1a(0) reference object.  The high half of flags
 * is then a field-by-field override mask against the parameter block:
 * 0x10000 link, 0x20000 orientation bits, 0x80000 position pair,
 * 0x40000 velocity pair (toward the position when 0x80000 is also
 * set, offset by -0x10000 otherwise, both scaled by the slot
 * descriptor's +12 rate), 0x200000 mode-1 pair, 0x400000 record
 * halfword +30, 0x800000 halfword +100, 0x1000000 callback override.
 *
 * Complete owner: full high-register prologue at 0x0200013c through
 * `pop {r0} / bx r0` at 0x02000306, then the literal pool
 * 0x02000308-0x02000313 (0x0200cbc4, 0x02008105, 0xffff0000); next
 * owner's prologue at 0x02000314 (assets/code exact owner).
 *
 * Uncertainty: callees unidentified beyond call shape; slot-descriptor
 * layout (+12 rate divisor) and object layout are named positionally.
 */

extern s32 Func_02004b1a(s32 arg0);
extern s32 Func_02004a48(s32 template_id, s32 a0, s32 a1, s32 a2);
extern void Func_02004a52(s32 object, s32 animation);
extern void Func_02004a6c(s32 object, s32 descriptor);
extern s32 Func_02004c66(s32 object, s32 link);
extern s32 Func_02004ad0(s32 delta, s32 rate);
extern s32 Func_02004ae8(s32 delta, s32 rate);
extern s32 Func_02004af6(s32 delta, s32 rate);
extern void Func_02004b6c(s32 object, s32 arg1);
extern void Func_02004b7c(s32 object, s32 value);

void Func_0200013c(
    s32 a0, s32 a1, s32 a2, s32 a3,
    s32 a4, s32 a5, u32 flags, u8 *params)
{
    s32 reference = Func_02004b1a(0);
    s32 template_id = 222;
    s32 object;
    s32 slot = (flags & 15) * 4;
    u8 *record;

    if ((flags & 0x100000) && params != 0)
        template_id = *(s16 *)(params + 24);
    object = Func_02004a48(template_id, a0, a1, a2);
    if (object == 0)
        return;

    record = *(u8 **)(object + 80);
    Func_02004a52(object, (flags + 1) & 15);
    Func_02004a6c(object, *(s32 *)(0x0200cbc4 + slot));
    *(u8 *)(object + 85) = 0;
    record[38] = 0;
    *(s32 *)(object + 108) = 0x02008105;
    *(s32 *)(object + 68) = a3;
    *(s32 *)(object + 72) = a4;
    *(s32 *)(object + 76) = a5;
    record[9] = (record[9] & ~13)
        | (*(u8 *)(*(s32 *)(reference + 80) + 9) & 12);
    *(s32 *)(object + 48) = 0;
    *(s32 *)(object + 52) = 0;
    *(u16 *)(object + 100) = 0;

    if ((flags & 0xffff0000) == 0)
        return;
    if (params == 0)
        return;

    if (flags & 0x10000)
        Func_02004c66(object, *(s32 *)(params + 4));
    if (flags & 0x20000) {
        *(u8 *)(object + 35) &= 0xfe;
        record[9] = (record[9] & ~13) | ((params[0] & 3) << 2);
    }
    if (flags & 0x80000) {
        *(s32 *)(object + 24) = *(s32 *)(params + 8);
        *(s32 *)(object + 28) = *(s32 *)(params + 12);
    }
    if (flags & 0x40000) {
        s32 rate = *(s32 *)(*(s32 *)(0x0200cbc4 + slot) + 12);

        if (flags & 0x80000) {
            *(s32 *)(object + 48) = Func_02004ad0(
                *(s32 *)(params + 16) - *(s32 *)(object + 24), rate);
            *(s32 *)(object + 52) = Func_02004af6(
                *(s32 *)(params + 20) - *(s32 *)(object + 28), rate);
        } else {
            *(s32 *)(object + 48) = Func_02004ae8(
                *(s32 *)(params + 16) + 0xffff0000, rate);
            *(s32 *)(object + 52) = Func_02004af6(
                *(s32 *)(params + 20) + 0xffff0000, rate);
        }
    }
    if (flags & 0x200000) {
        Func_02004b6c(object, 1);
        Func_02004b7c(object, *(s32 *)(params + 28));
    }
    if (flags & 0x400000)
        *(u16 *)(record + 30) = *(u16 *)(params + 32);
    if (flags & 0x800000)
        *(u16 *)(object + 100) = *(u16 *)(params + 34);
    if (flags & 0x1000000)
        *(s32 *)(object + 108) = *(s32 *)(params + 36);
}
