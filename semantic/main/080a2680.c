/*
 * Correctness fix, veneer audit (2026-08-01).
 *
 * `Func_080072f8` and `Func_080072fc` are not functions.  0x080072e4 begins the GCC
 * `__call_via_rN` veneer bank -- fifteen four-byte `bx rN; nop` entries,
 * r0..lr, ending at 0x08007320 -- so a `bl` into that range calls
 * whatever the named register holds.
 *
 * 0x080072f8 is `__call_via_r5` and 0x080072fc is `__call_via_r6`.  All four
 * sites here were verified from the ROM: the two at 0x080a2e30 and
 * 0x080a2e3a dispatch r6, loaded from the pool at 0x080a2e54, and the two
 * at 0x080a2f8e and 0x080a2f9a dispatch r5, loaded from the pool at
 * 0x080a3294.  Both pools hold 0x03001388, so all four are the same
 * relocated IWRAM word copy reached through two different veneers.
 *
 * The callee's signature is not guessed: the EXACT source
 * src/080d40ec.c declares 0x03001388 as
 * `void *(*)(void *destination, const void *source, s32 size)`, and
 * src/080e0524.c casts the same address to the same shape.  The previous
 * draft already had the right arity and argument order -- only the callee
 * was wrong.
 */
#include "types.h"

typedef void *(*WordCopy)(void *destination, const void *source, s32 size);

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

extern u8 *Data_03001f2c;

void Func_08002df0(void *);
void Func_080030f8(s32);
void *Func_08004938(s32);
void Func_08015068();
void Func_08015080();
void Func_08015270();
void Func_08015278();
void Func_08015298();
u8 *Func_08077008(u16);
void Func_08077010(u16);
u8 *Func_08077018(u16);
s32 Func_08077028(u16, u16);
s32 Func_08077050(u16, u16);
s32 Func_08077058(u16, u16);
s32 Func_080770c0(s32);
void Func_08077240(u16, s32);
void Func_080772c0(u16);
s32 Func_0808a490(u16);
void Func_0808a548(void);
void Func_080a112c();
void Func_080a17c4();
void Func_080a1d08();
void Func_080a23c0();
s32 Func_080a32b8(void);
void Func_080a345c(void);
void Func_080a3480(void);
s32 Func_080a355c(s32);
s32 Func_080a38d0(s32);
s32 Func_080a3ce4(u16);
void Func_080a3cf8(s32, s32);
s32 Func_080a3d6c(u16);
s32 Func_080a3d9c(u16, u16);
s32 Func_080a3ddc();
void Func_080a3e28();
void Func_080a3e88(u16, s32);
void Func_080a3ef0(u16, u16, s32);
s32 Func_080a414c(void);
s32 Func_080a46b4(u16, u16);
void Func_080a4754(void);
void Func_080a4800(u16);
void Func_080a4e20(void);
void Func_080a4e44(void);
void Func_080a4e68(void);
void Func_080a4e90(void);
void Func_080a4ee0(void);
s32 Func_080a4f08(s32, s32, s32);
void Func_080a51d0(void);
s32 Func_080a524c(s32);
s32 Func_080a5388(void);
s32 Func_080a5788(s32);
void Func_080f9010(s32);

#define SET_PORTRAIT_PREVIEW(state_, character_)                         \
    do {                                                                \
        u8 *preview_scratch_ = (state_) + 0x1c8;                         \
        u8 *preview_record_ = Func_08077008(character_);                 \
        FIELD((state_), u8, 0x218) =                                    \
            (u8)Func_080a3ddc(preview_record_, preview_scratch_, 0);     \
        Func_080a3e28(preview_scratch_, 0);                              \
    } while (0)

/*
 * Run the character/item transfer menu.  The operation is a synchronous
 * state machine: each state owns one screen, and animation ownership can
 * temporarily suspend dispatch.  The three output words are committed only
 * after a selection has been accepted.
 */
s32 Func_080a2680(s32 *source_out, s32 *target_out, s32 *item_out)
{
    u8 *state = Data_03001f2c;
    s32 phase = 0;
    s32 done = 0;
    s32 menu = 0;
    s32 result = 0;

    while (!done && Func_080770c0(0x150) == 0) {
        s32 command;

        switch (phase) {
        case 0:
            FIELD(state, u16, 0x174) = 0;
            Func_080a4ee0();
            Func_080a4e44();
            FIELD(FIELD(state, u8 *, 0x21c), u8, 5) = 13;
            Func_080a3cf8(0, 0xad8);
            Func_08015270(FIELD(state, void *, 44));
            Func_080a23c0(FIELD(state, void *, 44));
            command = Func_080a355c(0);
            if (command == -1) {
                result = -1;
                menu = 0;
                done = 1;
            }
            Func_08015270(FIELD(state, void *, 44));
            Func_080a345c();
            phase = 1;
            break;

        case 1:
            if (Func_080a3d6c(FIELD(state, u8, 0x21a)) == 0) {
                phase = 0;
                break;
            }
            Func_080a4ee0();
            Func_080a4e44();
            FIELD(FIELD(state, u8 *, 0x21c), u8, 5) = 13;
            FIELD(FIELD(state, u8 *, 20), u8, 5) = 1;
            Func_080a3cf8(0, 0xad9);
            menu = Func_080a5788(0);
            phase = 0;
            if (menu != -1) {
                FIELD(state, u8, 0x25d) = 0xff;
                phase = 9;
            }
            break;

        case 9:
            command = Func_080a414c();
            if (command == -1) {
                phase = 1;
                FIELD(state, u16, 0x25a) = 1;
            }
            if (command == 0) {
                u16 item = FIELD(state, u16, 0x178);
                u8 source = FIELD(state, u8, 0x21a);
                s32 action;

                if (Func_0808a490(item & 0x1ff) != 0) {
                    *source_out = source;
                    *target_out = 0;
                    *item_out = item & 0x1ff;
                    result = 1;
                    done = 1;
                } else {
                    action = Func_080a46b4(source, item);
                    if (action == 1)
                        phase = 2;
                    if (action == 2) {
                        Func_080a32b8();
                        Func_08015278(FIELD(state, void *, 44));
                        Func_080a1d08(
                            (s32)FIELD(state, s16, 0x222) + 0xbef,
                            0, -1);
                        FIELD(FIELD(state, u8 *, 20), u8, 5) = 13;
                        SET_PORTRAIT_PREVIEW(state, source);
                        phase = 0;
                    }
                    if (action == 0 || action == -1) {
                        *source_out = source;
                        *target_out = FIELD(state, u8, 0x21b);
                        *item_out = item & 0x1ff;
                        result = 1;
                        done = 1;
                    }
                }
            }
            if (command == 1)
                phase = 3;
            if (command == 3)
                phase = 6;
            if (command == 5)
                phase = 5;
            if (command == 4)
                phase = 11;
            if (command == 2)
                phase = 10;
            break;

        case 2:
            Func_080a345c();
            Func_080a4e68();
            Func_080a4e20();
            Func_08015270(FIELD(state, void *, 0x10c));
            Func_080a51d0();
            Func_08015080(0xadb, FIELD(state, void *, 0x10c), 16, 16);
            command = Func_080a38d0(0);
            if (command == -1) {
                phase = 9;
            } else {
                s32 detail_mode =
                    Func_080a3ce4(FIELD(state, u16, 0x178) & 0x1ff)
                        ? 8 : 0;
                s32 picked = Func_080a32b8();

                Func_080a112c(
                    FIELD(state, s32, 36), FIELD(state, u8, 0x21b),
                    0, detail_mode);
                if (picked != -1) {
                    Func_08015278(FIELD(state, void *, 44));
                    Func_080a1d08(
                        (s32)FIELD(state, s16, 0x25a) + 0xbef,
                        0, -1);
                    FIELD(FIELD(state, u8 *, 20), u8, 5) = 13;
                    Func_080a4754();
                    phase = 1;
                }
                SET_PORTRAIT_PREVIEW(state, FIELD(state, u8, 0x21b));
                FIELD(state, u16, 0x25a) = 1;
            }
            break;

        case 6:
            Func_080a4e68();
            Func_080a4e20();
            Func_08015270(FIELD(state, void *, 0x10c));
            Func_080a51d0();
            Func_08015080(0xadc, FIELD(state, void *, 0x10c), 16, 16);
            command = Func_080a38d0(1);
            phase = 4;
            if (command == -1) {
                Func_080a3ef0(
                    FIELD(state, u8, 0x21a), FIELD(state, u16, 0x174), 0);
                phase = 9;
            }
            break;

        case 5: {
            u16 selected = FIELD(state, u16, 0x178);
            s32 count = 0;
            u8 source;

            Func_080a345c();
            if ((FIELD(Func_08077018(selected & 0x1ff), u8, 3) & 0x10)
                != 0) {
                count = (selected >> 11) + 1;
                if (count > 1) {
                    Func_080a51d0();
                    count = Func_080a4f08(0, count, 1);
                } else {
                    count = 0;
                }
            }
            phase = 9;
            if (count == -1)
                break;

            FIELD(state, u8, 0x21a) = 0;
            selected = (selected & 0x5ff) | (count << 11);
            Func_08015298(
                2, selected, FIELD(FIELD(state, u8 *, 0x21c), u8, 14), 0);
            FIELD(FIELD(state, u8 *, 0x21c), u8, 5) = 1;
            FIELD(FIELD(state, u8 *, 0x21c), u16, 6) = 120;
            FIELD(FIELD(state, u8 *, 0x21c), u16, 8) = 28;
            Func_080a17c4(FIELD(state, void *, 0x21c));
            Func_08015068(FIELD(state, void *, 52), 0, 72, 120, 96);
            Func_08015270(FIELD(state, void *, 0x10c));

            if (Func_080a524c(menu) == 0) {
                s32 i;
                source = FIELD(state, u8, 0x21a);
                (void)Func_08077008(source);
                for (i = 0; i < count + 1; i++) {
                    s32 slot = Func_08077058(source, FIELD(state, u16, 0x174));
                    Func_08077240(
                        slot & FIELD(state, u16, 0x178), 1);
                }
                Func_08077010(source);
                Func_080a4e44();
                Func_080a3e88(FIELD(state, u8, 0x21a), 0);
                FIELD(FIELD(state, u8 *, 0x21c), u8, 5) = 13;
                FIELD(FIELD(state, u8 *, 20), u8, 5) = 13;
                Func_080030f8(1);
                Func_08015278(FIELD(state, void *, 44));
                Func_080a1d08(0xb7d, 14, 13);
                FIELD(state, u16, 0x25a) = 1;
            }
            Func_08077010(FIELD(state, u8, 0x21a));
            FIELD(FIELD(state, u8 *, 0x21c), u8, 5) = 13;
            Func_0808a548();
            break;
        }

        case 4: {
            u16 item = FIELD(state, u16, 0x178);
            s32 failed = 0;
            s32 chosen = 1;
            u8 first = FIELD(state, u8, 0x21a);
            u8 second = FIELD(state, u8, 0x21b);
            s32 i;

            if ((FIELD(Func_08077018(item & 0x1ff), u8, 3) & 0x10) != 0) {
                s32 start =
                    Func_080a3d9c(first, item & 0x1ff);
                if (start == 30)
                    failed = 1;
                if (Func_080a3d6c(first) == 15 && start == 0) {
                    phase = 7;
                    break;
                }
                if (!failed) {
                    s32 amount = (item >> 11) + 1;
                    if (start + amount > 30)
                        amount = 30 - start;
                    chosen = amount < 0
                        ? Func_080a4f08(0, amount, 0) : 0;
                    if (chosen == -1) {
                        phase = 6;
                        break;
                    }
                    for (i = 0; i < chosen + 1; i++) {
                        s32 slot =
                            Func_08077028(first, item & 0x5ff);
                        if (slot == -1) {
                            failed = 1;
                        } else {
                            Func_08077058(
                                first, FIELD(state, u16, 0x174));
                            FIELD(state, u16, 0x176) = (u16)slot;
                        }
                    }
                }
            } else {
                chosen = Func_08077028(first, item & 0x5ff);
                if (chosen == -1) {
                    phase = 7;
                    break;
                }
                FIELD(state, u16, 0x176) = (u16)chosen;
                if (Func_08077058(first, FIELD(state, u16, 0x174)) == -1)
                    failed = 1;
            }

            Func_08077010(first);
            Func_08077010(second);
            Func_080772c0(first);
            Func_080772c0(second);
            chosen = 1;
            if (!failed) {
                FIELD(state, u8, 0x21a) = second;
                FIELD(state, u16, 0x178) &= 0x5ff;
                Func_080a4e90();
                Func_08015270(FIELD(state, void *, 0x10c));
                Func_080a51d0();
                chosen = Func_080a5388();
            }
            if (Func_080770c0(0x150) != 0)
                break;

            Func_080a3e88(second, 1);
            FIELD(FIELD(state, u8 *, 20), u8, 5) = 13;
            Func_080030f8(1);
            Func_08015278(FIELD(state, void *, 44));
            if (failed) {
                Func_080a1d08(0xb85, 15, 14);
            } else if (chosen == 1) {
                Func_080a1d08(0xb7f, 15, 14);
            } else {
                Func_080a3ef0(second, FIELD(state, u16, 0x176), 0);
                Func_080a1d08(0xb7c, 15, 14);
                if ((FIELD(Func_08077018(FIELD(state, u16, 0x178)),
                           u8, 3) & 1) != 0) {
                    Func_080f9010(103);
                    Func_08015278(FIELD(state, void *, 44));
                    Func_080a1d08(0xb83, 14, 14);
                }
            }
            Func_0808a548();
            phase = 0;
            break;
        }

        case 7: {
            s32 failure = 0;
            s32 slot;
            s32 i;
            u8 *first_record;
            u8 *second_record;
            void *first_copy;
            void *second_copy;

            Func_080a4ee0();
            Func_080a4e44();
            Func_080a3cf8(0, 0xadd);
            menu = Func_080a5788(1);
            if (menu == -1) {
                phase = 6;
                break;
            }
            first_record = Func_08077008(FIELD(state, u8, 0x21a));
            second_record = Func_08077008(FIELD(state, u8, 0x21b));
            first_copy = Func_08004938(0x14c);
            second_copy = Func_08004938(0x14c);
            ((WordCopy)0x03001388)(first_copy, first_record, 0x14c);
            ((WordCopy)0x03001388)(second_copy, second_record, 0x14c);

            for (i = 0; i < 30; i++) {
                slot = Func_08077058(
                    FIELD(state, u8, 0x2a6 + i),
                    FIELD(state, u16, 0x174));
                if (slot == 2)
                    break;
                if (slot == -1)
                    failure = 1;
            }
            i++;
            for (slot = 0; slot < 30; slot++) {
                u16 item = FIELD(state, u16, 0x17a);
                if ((item & 0x200) &&
                    (FIELD(Func_08077018(item & 0x1ff), u8, 3) & 2))
                    failure = 1;
                command = Func_08077058(
                    FIELD(state, u8, 0x21b),
                    FIELD(state, u16, 0x176));
                if (command == 2)
                    break;
                if (command == -1) {
                    failure = 1;
                    break;
                }
            }
            slot++;
            while (i != 0) {
                command = Func_08077028(
                    FIELD(state, u8, 0x21a),
                    FIELD(state, u16, 0x178) & 0x5ff);
                if (command == -1) {
                    failure = 1;
                    break;
                }
                FIELD(state, u16, 0x176) = (u16)command;
                i--;
            }
            while (slot != 0) {
                command = Func_08077028(
                    FIELD(state, u8, 0x21b),
                    FIELD(state, u16, 0x17a) & 0x5ff);
                if (command == -1) {
                    failure = 1;
                    break;
                }
                FIELD(state, u16, 0x174) = (u16)command;
                slot--;
            }
            Func_080030f8(1);
            if (failure) {
                ((WordCopy)0x03001388)(first_record, first_copy, 0x14c);
                ((WordCopy)0x03001388)(second_record, second_copy, 0x14c);
                Func_08015278(FIELD(state, void *, 44));
                Func_080a1d08(0xb84, 15, 14);
            } else {
                s32 confirm;
                Func_08077010(FIELD(state, u8, 0x21a));
                Func_08077010(FIELD(state, u8, 0x21b));
                Func_080772c0(FIELD(state, u8, 0x21a));
                Func_080772c0(FIELD(state, u8, 0x21b));
                Func_080a4e68();
                Func_080a4e90();
                Func_080a3480();
                Func_08015270(FIELD(state, void *, 0x10c));
                FIELD(state, u8, 0x21a) = FIELD(state, u8, 0x21b);
                FIELD(state, u16, 0x178) &= 0x5ff;
                Func_080a51d0();
                confirm = Func_080a5388();
                if (Func_080770c0(0x150) == 0) {
                    Func_08015278(FIELD(state, void *, 44));
                    Func_080a4e20();
                    Func_080a3e88(FIELD(state, u8, 0x21b), 1);
                    if (confirm == 0) {
                        Func_080a3ef0(
                            FIELD(state, u8, 0x21b),
                            FIELD(state, u16, 0x176), 0);
                        Func_080a1d08(0xb7c, 15, 14);
                        if ((FIELD(Func_08077018(
                                     FIELD(state, u16, 0x178)),
                                   u8, 3) & 1) != 0) {
                            Func_080f9010(103);
                            Func_08015278(FIELD(state, void *, 44));
                            Func_080a1d08(0xb83, 14, 14);
                        }
                    } else {
                        Func_080a1d08(0xb80, 15, 14);
                    }
                }
            }
            Func_08002df0(second_copy);
            Func_08002df0(first_copy);
            Func_0808a548();
            phase = 0;
            break;
        }

        case 3:
            command = Func_08077050(
                FIELD(state, u8, 0x21b), FIELD(state, u16, 0x174));
            phase = 1;
            if (command == -1)
                break;
            if (command == -2) {
                Func_08015278(FIELD(state, void *, 44));
                Func_080a1d08(0xb81, 0, -1);
                break;
            }
            Func_08077010(FIELD(state, u8, 0x21b));
            Func_080772c0(FIELD(state, u8, 0x21b));
            FIELD(FIELD(state, u8 *, 20), u8, 5) = 13;
            SET_PORTRAIT_PREVIEW(state, FIELD(state, u8, 0x21b));
            Func_080030f8(1);
            Func_080a3ef0(
                FIELD(state, u8, 0x21b), FIELD(state, u16, 0x174), 0);
            Func_08015278(FIELD(state, void *, 44));
            Func_080a1d08(0xb7c, 15, 8);
            if ((FIELD(Func_08077018(FIELD(state, u16, 0x178)),
                       u8, 3) & 1) != 0) {
                Func_080f9010(103);
                Func_08015278(FIELD(state, void *, 44));
                Func_080a1d08(0xb83, 14, 8);
            }
            phase = 1;
            break;

        case 11: {
            u8 character = FIELD(state, u8, 0x21a);
            u8 *record = Func_08077008(character);
            s32 offset = 0xd8 + FIELD(state, u16, 0x174) * 2;

            FIELD(record, u16, offset) &= 0xfdff;
            Func_08077010(character);
            Func_080772c0(character);
            FIELD(FIELD(state, u8 *, 20), u8, 5) = 13;
            SET_PORTRAIT_PREVIEW(state, character);
            Func_080030f8(1);
            FIELD(state, u8, 0x25c) = 1;
            Func_080a3ef0(character, FIELD(state, u16, 0x174), 0);
            FIELD(state, u8, 0x25c) = 0;
            Func_08015278(FIELD(state, void *, 44));
            Func_080a1d08(0xb82, 14, 8);
            Func_0808a548();
            phase = 1;
            break;
        }

        case 10:
            FIELD(FIELD(state, u8 *, 20), u8, 5) = 13;
            Func_080a4800(FIELD(state, u16, 0x178));
            Func_08015270(FIELD(state, void *, 36));
            Func_080a3ef0(
                FIELD(state, u8, 0x21a), FIELD(state, u16, 0x174), 0);
            FIELD(FIELD(state, u8 *, 20), u8, 5) = 1;
            phase = 9;
            break;

        case 12:
            (void)Func_080a4f08(0, 30, 0);
            phase = 1;
            break;

        case 8:
            phase = 1;
            break;

        default:
            done = 1;
            break;
        }
    }

    if (Func_080770c0(0x150) != 0)
        result = -1;
    return result;
}
