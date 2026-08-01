/*
 * Correctness fix, veneer audit (2026-08-01).
 *
 * `Func_080072f0` is not a function.  0x080072e4 begins the GCC
 * `__call_via_rN` veneer bank -- fifteen four-byte `bx rN; nop` entries,
 * r0..lr, ending at 0x08007320 -- so 0x080072f0 is `__call_via_r3` and
 * `bl 0x80072f0` calls whatever r3 holds.
 *
 * At every site in this file the ROM loads r3 from the literal pool with
 * the constant 0x03001388, so the callee is the relocated IWRAM word copy
 * at that address.  Its signature is not guessed: the EXACT source
 * src/080d40ec.c declares it as
 * `void *(*)(void *destination, const void *source, s32 size)` and
 * src/080e0524.c casts the same address to the same shape.
 *
 * Note what the previous draft had already half-seen: it passed
 * 0x03001388 as a fourth ARGUMENT.  That value was never an argument --
 * it is the callee, and the register load that produced it is the call
 * target, not a parameter.
 */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef void *(*WordCopy)(void *destination, const void *source, s32 size);

#define U8_AT(pointer, offset)  (*(u8 *)((u8 *)(pointer) + (offset)))
#define S8_AT(pointer, offset)  (*(s8 *)((u8 *)(pointer) + (offset)))
#define U16_AT(pointer, offset) (*(u16 *)((u8 *)(pointer) + (offset)))
#define S16_AT(pointer, offset) (*(s16 *)((u8 *)(pointer) + (offset)))

void *Func_08077008(s32);
void Func_08016418(void *, s32);
void *Func_080162d4(s32, s32, s32, s32, s32);
void *Func_08004938(s32);
void *Func_08004970(s32);
s32 Func_08077208(s32, s32, s32);
void Func_080771b8(s32, s32, s32);
void Func_080771b0(s32, s32, s32);
void Func_08077010(s32);
s32 Func_080228e4(const void *, const void *, u16 *, s32 *, s32 *);
s32 Func_080022ec(s32, s32);
void Func_0801e7c0(s32, void *, s32, s32);
void Func_080228bc(s16 *, s32);
void Func_08017c8c(s16 *, void *, s32, s32);
void Func_0801e41c(void *, s32, s32, s32, s32);
void Func_0801e71c(s32);
u8 *Func_08077080(s32);
void Func_08019000();
void Func_080218dc();
void Func_08022a38(void *, s32, s32, s32);
void Func_0801ea08();
void Func_08022a7c(s32, s32, s32);
void Func_08002df0(void *);

void *Func_08022b44(void *previous_window,
                    s32 combatant,
                    u32 encoded_selection,
                    s32 page,
                    s32 *page_count)
{
    void *current = Func_08077008(combatant);
    void *window;
    u8 *snapshot;
    s16 *scratch;
    u16 *entries;
    s32 entry_count;
    s32 has_first;
    s32 has_second;
    s32 pages;
    s32 old_signed_38 = 0;
    s32 old_signed_3a = 0;
    s32 old_unsigned_3c = 0;
    s32 old_unsigned_3e = 0;
    s32 old_unsigned_40 = 0;
    volatile s32 branch_barrier = 0;

    if (previous_window != 0)
        Func_08016418(previous_window, 1);

    if (page == 0) {
        window = Func_080162d4(0, 8, 0x15, 0x0b, 6);
        branch_barrier = 10;
    } else {
        window = Func_080162d4(0, 5, 0x15, 0x0e, 6);
        branch_barrier = 11;
    }
    if (window == 0)
        return 0;

    scratch = Func_08004938(0x80);
    snapshot = Func_08004938(0x14c);
    entries = Func_08004970(0x60);
    ((WordCopy)0x03001388)(snapshot, current, 0x14c);

    {
        s32 group = (encoded_selection >> 8) & 15;
        s32 slot = encoded_selection & 0xff;

        if (Func_08077208(combatant, group, slot) != 0)
            Func_080771b8(combatant, group, slot);
        else
            Func_080771b0(combatant, group, slot);
    }
    Func_08077010(combatant);

    entry_count = Func_080228e4(
        snapshot + 0x58, (u8 *)current + 0x58,
        entries, &has_first, &has_second);
    pages = Func_080022ec(entry_count - 1, 5) + 1;
    *page_count = pages;
    if (page * 5 - 5 >= entry_count)
        page = pages;

    if (page == 0) {
        Func_0801e7c0(0x8ae, window, 0, 8);
        Func_0801e7c0(0x8af, window, 0, 0x10);
        Func_0801e7c0(0x8b0, window, 0, 0x18);
        Func_0801e7c0(0x8b1, window, 0, 0x20);
        Func_0801e7c0(0x8b2, window, 0, 0x28);
        Func_0801e7c0(0x8b3, window, 0, 0x30);

        old_signed_38 = S16_AT(snapshot, 0x38);
        Func_080228bc(scratch, old_signed_38);
        Func_08017c8c(scratch + 7, window, 5, 1);

        old_signed_3a = S16_AT(snapshot, 0x3a);
        Func_080228bc(scratch, old_signed_3a);
        Func_08017c8c(scratch + 7, window, 5, 2);

        old_unsigned_3c = U16_AT(snapshot, 0x3c);
        Func_080228bc(scratch, old_unsigned_3c);
        Func_08017c8c(scratch + 8, window, 6, 3);

        old_unsigned_3e = U16_AT(snapshot, 0x3e);
        Func_080228bc(scratch, old_unsigned_3e);
        Func_08017c8c(scratch + 8, window, 6, 4);

        old_unsigned_40 = U16_AT(snapshot, 0x40);
        Func_080228bc(scratch, old_unsigned_40);
        Func_08017c8c(scratch + 8, window, 6, 5);

        Func_080228bc(scratch, U8_AT(snapshot, 0x42));
        Func_08017c8c(scratch + 8, window, 5, 6);
        Func_0801e41c(window, 0, 8, 0x13, 8);
        if (has_first || has_second)
            Func_0801e71c(2);
        Func_0801e7c0(0x8ad, window, 0x18, 0x40);
        Func_0801e71c(0x0f);
    }

    if (page > 0) {
        s32 shown = entry_count > 4 ? 5 : entry_count;
        s32 start = page * 5 - 5;
        s32 row = 0;
        s32 flags_shown = 0;

        if (start < entry_count) {
            u16 *entry = entries + start * 2;
            s32 x = -4;
            s32 y = 0;

            while (row < shown && start < entry_count) {
                u16 encoded = *entry;
                u8 *definition = Func_08077080(encoded);
                s32 category = definition[2];
                s32 secondary =
                    definition[8] == 0xff ? 0x0b : definition[8] - 1;

                if (category != 4)
                    Func_08019000(
                        window, category + 0x5001, 0x0f, y, 0);
                Func_080218dc(window, 0x10, y, secondary, 0);
                Func_08022a38(window, 0, x, encoded & 0x3fff);

                if (encoded & 0x8000)
                    Func_0801e71c(4);
                else if (encoded & 0x4000)
                    Func_0801e71c(2);
                else
                    Func_0801e71c(0x0f);

                Func_0801e7c0(
                    (encoded & 0x3fff) + 0x333, window, 0x10, y);
                Func_08019000(window, 0xf01f, 0x0b, y, 0);
                Func_08019000(window, 0xf01e, 0x0c, y, 0);
                Func_0801ea08(
                    Func_08077080(encoded)[9], 2, window, 0x68, y);

                row++;
                start++;
                entry += 2;
                x += 0x10;
                y += 2;
            }
        }

        if (has_first) {
            Func_0801e71c(4);
            Func_0801e7c0(0xba2, window, 0x20, 0x50);
            flags_shown = 1;
        }
        if (has_second) {
            Func_0801e71c(2);
            Func_0801e7c0(
                0xba3, window, 0x20, flags_shown * 8 + 0x50);
            flags_shown++;
        }
        if (flags_shown == 0)
            Func_0801e7c0(0xba8, window, 0x20, 0x50);
        Func_0801e71c(0x0f);
        Func_0801e71c(0x0f);
        Func_0801e41c(window, 0, 0x0a, 0x13, 0x0a);
    }

    if (page == 0) {
        s32 current_value;

        Func_0801e7c0(
            U8_AT(snapshot, 0x129) + 0x741, window, 0, 0);
        Func_0801e7c0(
            U8_AT(current, 0x129) + 0x741, window, 0x50, 0);
        if (U8_AT(snapshot, 0x129) != U8_AT(current, 0x129)) {
            Func_08019000(window, 0xf728, 9, 0, page);
            branch_barrier = 1;
        } else {
            Func_08019000(window, 0xf729, 9, 0, page);
            branch_barrier = 2;
        }

        current_value = S16_AT(current, 0x38);
        Func_080228bc(scratch, current_value);
        Func_08017c8c(scratch + 7, window, 0x0b, 1);
        if (current_value != old_signed_38)
            Func_08022a7c(0x50, 0x0e, current_value > old_signed_38);

        current_value = S16_AT(current, 0x3a);
        Func_080228bc(scratch, current_value);
        Func_08017c8c(scratch + 7, window, 0x0b, 2);
        if (current_value != old_signed_3a)
            Func_08022a7c(0x50, 0x16, current_value > old_signed_3a);

        current_value = U16_AT(current, 0x3c);
        Func_080228bc(scratch, current_value);
        Func_08017c8c(scratch + 7, window, 0x0b, 3);
        if (current_value != old_unsigned_3c)
            Func_08022a7c(
                0x50, 0x1e, (u32)current_value > (u32)old_unsigned_3c);

        current_value = U16_AT(current, 0x3e);
        Func_080228bc(scratch, current_value);
        Func_08017c8c(scratch + 7, window, 0x0b, 4);
        if (current_value != old_unsigned_3e)
            Func_08022a7c(
                0x50, 0x26, (u32)current_value > (u32)old_unsigned_3e);

        current_value = U16_AT(current, 0x40);
        Func_080228bc(scratch, current_value);
        Func_08017c8c(scratch + 7, window, 0x0b, 5);
        if (current_value != old_unsigned_40)
            Func_08022a7c(
                0x50, 0x2e, (u32)current_value > (u32)old_unsigned_40);

        current_value = U8_AT(current, 0x42);
        Func_080228bc(scratch, current_value);
        Func_08017c8c(scratch + 8, window, 0x0c, 6);
        if (current_value != U8_AT(snapshot, 0x42))
            Func_08022a7c(
                0x50, 0x36,
                (u32)current_value > (u32)U8_AT(snapshot, 0x42));
    }

    ((WordCopy)0x03001388)(current, snapshot, 0x14c);
    Func_08002df0(entries);
    Func_08002df0(snapshot);
    Func_08002df0(scratch);
    (void)branch_barrier;
    return window;
}
