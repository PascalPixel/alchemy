#include "layout_guard.h"
#include "types.h"

struct FocusObject_080a8914 {
    u8 padding00[5];
    u8 dirty;
};

struct BattleUiState_080a8914 {
    u8 padding000[0x17c];
    struct FocusObject_080a8914 *focus;
};

struct Combatant_080a8914 {
    u8 padding000[0x0f];
    u8 level;
    u8 padding010[0x24];
    s16 paired_values[4];
    u16 summary_values[3];
    u8 summary_value_3;
    u8 padding043[0x0e1];
    s32 experience;
    u8 padding128;
    u8 class_id;
};

LAYOUT_OFFSET_GUARD(
    BattleUi080a8914_Focus,
    struct BattleUiState_080a8914,
    focus,
    0x17c);
LAYOUT_OFFSET_GUARD(
    Combatant080a8914_PairedValues,
    struct Combatant_080a8914,
    paired_values,
    0x34);
LAYOUT_OFFSET_GUARD(
    Combatant080a8914_SummaryValues,
    struct Combatant_080a8914,
    summary_values,
    0x3c);
LAYOUT_OFFSET_GUARD(
    Combatant080a8914_Experience,
    struct Combatant_080a8914,
    experience,
    0x124);
LAYOUT_OFFSET_GUARD(
    Combatant080a8914_ClassId,
    struct Combatant_080a8914,
    class_id,
    0x129);

extern struct BattleUiState_080a8914 *Data_03001f2c;

void Func_08015068(s32, s32, s32, s32, s32);
void Func_08015080(s32, s32, s32, s32);
void Func_08015090(const void *, s32, s32, s32);
s32 Func_08015098(const void *, s32, s32, s32);
void Func_080150b0(s32, s32, s32, s32, s32);
void Func_080150b8(s32);
void Func_080030f8(u32);
struct Combatant_080a8914 *Func_08077008(s32);

/*
 * Draw the stable identity/statistics portion of a combatant summary panel.
 * Mode zero clears both panel bands around the draw; other modes update the
 * existing window in place.  The surrounding selector owns the focus object,
 * which is marked dirty before any text is emitted.
 */
void Func_080a8914(s32 window, s32 combatant_id, s32 mode)
{
    struct Combatant_080a8914 *combatant =
        Func_08077008(combatant_id);
    s32 compact_mode = mode & 0xff;

    Data_03001f2c->focus->dirty = 1;
    if (compact_mode == 0)
        Func_08015068(window, 0, 0, 0x80, 0x28);

    Func_08015090(combatant, window, 0x28, 0);
    Func_08015080(combatant->class_id + 0x741, window, 0, 0x20);
    Func_08015090((const void *)0x080af22c, window, 0x68, 0);
    Func_080150b8(15);
    Func_080150b0(combatant->level, 2, window, 0x80, 0);

    Func_08015090((const void *)0x080af234, window, 0x28, 0x10);
    Func_080150b0(combatant->paired_values[2], 4, window, 0x48, 0x10);
    Func_080150b0(combatant->paired_values[0], 4, window, 0x70, 0x10);
    Func_08015098((const void *)0x080af230, window, 0x68, 0x10);

    Func_08015090((const void *)0x080af238, window, 0x28, 0x18);
    Func_080150b0(combatant->paired_values[3], 4, window, 0x48, 0x18);
    Func_080150b0(combatant->paired_values[1], 4, window, 0x70, 0x18);
    Func_08015098((const void *)0x080af230, window, 0x68, 0x18);

    Func_08015080(0x0b0e, window, 0x28, 8);
    Func_080150b0(combatant->experience, 7, window, 0x58, 8);

    if (compact_mode == 0) {
        Func_080030f8(1);
        Func_08015068(window, 0x90, 0, 0xe0, 0x28);
    }

    Func_08015080(0x0af7, window, 0x98, 0);
    Func_08015080(0x0af8, window, 0x98, 8);
    Func_08015080(0x0af9, window, 0x98, 0x10);
    Func_08015080(0x0afa, window, 0x98, 0x18);
    Func_080150b0(combatant->summary_values[0], 3, window, 0xc8, 0);
    Func_080150b0(combatant->summary_values[1], 3, window, 0xc8, 8);
    Func_080150b0(combatant->summary_values[2], 3, window, 0xc8, 0x10);
    Func_080150b0(combatant->summary_value_3, 3, window, 0xc8, 0x18);
}
