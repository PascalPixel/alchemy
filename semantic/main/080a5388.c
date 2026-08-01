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
#include "layout_guard.h"
#include "types.h"

typedef void *(*WordCopy)(void *destination, const void *source, s32 size);
typedef struct ConfirmMenuState_080a5388 {
    u8 padding000[0x10c];
    s32 window;
    u8 padding110[0x64];
    u16 item_id;
    u16 subject_id;
    u8 padding178[0xa3];
    u8 selected_character;
} ConfirmMenuState_080a5388;

LAYOUT_OFFSET_GUARD(
    ConfirmMenuState080a5388_Window,
    ConfirmMenuState_080a5388,
    window,
    0x10c);
LAYOUT_OFFSET_GUARD(
    ConfirmMenuState080a5388_ItemId,
    ConfirmMenuState_080a5388,
    item_id,
    0x174);
LAYOUT_OFFSET_GUARD(
    ConfirmMenuState080a5388_SubjectId,
    ConfirmMenuState_080a5388,
    subject_id,
    0x176);
LAYOUT_OFFSET_GUARD(
    ConfirmMenuState080a5388_SelectedCharacter,
    ConfirmMenuState_080a5388,
    selected_character,
    0x21b);

extern ConfirmMenuState_080a5388 *Data_03001f2c;
extern volatile u32 Data_03001b04;
extern volatile u32 Data_03001c94;

s32 Func_080022fc(s32, s32);
void Func_08002df0(void *);
void Func_080030f8(s32);
void *Func_08004938(s32);
void Func_08015068(s32, s32, s32, s32, s32);
void Func_08015080(s32, s32, s32, s32);
void *Func_08077008(u8);
void Func_08077010(u8);
s32 Func_08077050(u8, u16);
s32 Func_080770c0(s32);
void Func_080772c0(u8);
void Func_080a1a40(s32, s32);
void Func_080a1ac0(s32, s32);
void Func_080a3ef0(u8, u16, s32);
void Func_080f9010(s32);

/*
 * Present the two-choice confirmation window for the selected character and
 * item.  The character state is snapshotted before the preview is installed;
 * choosing the second option, or losing the owning menu, restores that state.
 */
s32 Func_080a5388(void)
{
    ConfirmMenuState_080a5388 *state = Data_03001f2c;
    u8 character = state->selected_character;
    void *character_state = Func_08077008(character);
    void *snapshot;
    s32 selection = 0;
    s32 redraw = 1;

    Func_080a3ef0(character, state->subject_id, 0);
    snapshot = Func_08004938(0x14c);
    ((WordCopy)0x03001388)(snapshot, character_state, 0x14c);

    if ((u32)(Func_08077050(character, state->subject_id) + 2) <= 1) {
        selection = 1;
    } else {
        Func_08015080(0x0b2c, state->window, 24, 24);
        Func_08015080(0x0b2d, state->window, 72, 24);
        Func_08015068(state->window, 16, 16, 96, 24);
        Func_08015080(0x0ad6, state->window, 0, 16);
        Func_080a1ac0(110, 32);

        while (Func_080770c0(0x150) == 0) {
            if (redraw != 0) {
                redraw = 0;
                selection = Func_080022fc(selection + 2, 2);
            }

            if ((Data_03001b04 & 1) != 0) {
                Func_080f9010(0xaf);
                break;
            }
            if ((Data_03001b04 & 2) != 0) {
                Func_080f9010(0x71);
                break;
            }

            Func_080a1a40(selection * 48 + 110, 32);

            if ((Data_03001c94 & 0x20) != 0) {
                selection--;
                redraw = 1;
                Func_080f9010(0x6f);
            }
            if ((Data_03001c94 & 0x10) != 0) {
                selection++;
                redraw = 1;
                Func_080f9010(0x6f);
            }
            Func_080030f8(1);
        }
    }

    if (Func_080770c0(0x150) != 0)
        selection = 1;

    if (selection == 1)
        ((WordCopy)0x03001388)(character_state, snapshot, 0x14c);

    Func_08002df0(snapshot);
    Func_08077010(character);
    Func_080772c0(character);
    return selection;
}
