#include "layout_guard.h"
#include "types.h"

struct Template_08079460 {
    u8 unknown_00[0x0f];
    u8 field_0f;
    u16 field_10;
    u16 field_12;
    u16 field_14;
    u16 field_16;
    u16 field_18;
    u8 field_1a;
    u8 field_1b;
    u8 field_1c;
    u8 field_1d;
    u8 unknown_1e[2];
    s32 field_20;
    u8 unknown_24[4];
    u16 equipment[4];
    u8 equipment_count[4];
};

struct Record_08079460 {
    u8 name[14];
    u8 field_0e;
    u8 field_0f;
    u16 field_10;
    u16 field_12;
    u16 field_14;
    u16 field_16;
    u16 field_18;
    u16 field_1a;
    u16 field_1c;
    u8 field_1e;
    u8 field_1f;
    u8 field_20;
    u8 field_21;
    u8 unknown_22[0x12];
    u16 field_34;
    u16 field_36;
    u16 field_38;
    u16 field_3a;
    u8 unknown_3c[0x9c];
    u16 equipment[15];
    u8 unknown_f6[0x2a];
    s32 field_120;
    u8 unknown_124[4];
    u8 template_id;
    u8 field_129;
    u8 field_12a;
};

/* The call-via-r3 thunk preserves the slot in r2 while invoking the
 * relocated record initializer at 0x03000164.  Keeping all four ABI values
 * visible gives the semantic source the same call contract as the reference,
 * without embedding registers or assembly in C. */
void Func_080072f0(
    struct Record_08079460 *,
    s32 size,
    s32 slot,
    void *initialize);

LAYOUT_OFFSET_GUARD(
    Template08079460_Equipment,
    struct Template_08079460,
    equipment,
    0x28);
LAYOUT_OFFSET_GUARD(
    Record08079460_Equipment,
    struct Record_08079460,
    equipment,
    0xd8);
LAYOUT_OFFSET_GUARD(
    Record08079460_Field120,
    struct Record_08079460,
    field_120,
    0x120);
LAYOUT_OFFSET_GUARD(
    Record08079460_TemplateId,
    struct Record_08079460,
    template_id,
    0x128);

struct Record_08079460 *Func_08077394(s32 slot);
void Func_08077428(s32 slot);
void Func_080798e0(s32 slot, void *record_data);
void Func_08015028(s32 text_id, u16 *output, s32 limit);

s32 Func_08079460(s32 slot, u8 template_id, s32 suffix)
{
    static const struct Template_08079460 *const templates =
        (const struct Template_08079460 *)0x08080ec8;
    struct Record_08079460 *record;
    const struct Template_08079460 *template;
    u16 text[15];
    u32 template_index = (u32)template_id - 8;
    s32 name_length;
    s32 equipment_count;
    s32 group;

    if (slot < 128 || slot > 134 || template_index > 242)
        return 0;

    record = Func_08077394(slot);
    Func_080072f0(record, 0x14c, slot, (void *)0x03000164);
    if (template_index > 164)
        template_index = 0;
    template = &templates[template_index];

    record->field_0f = template->field_0f;
    record->field_10 = template->field_10;
    record->field_34 = template->field_10;
    record->field_38 = template->field_10;
    record->field_12 = template->field_12;
    record->field_36 = template->field_12;
    record->field_3a = template->field_12;
    record->field_14 = 0x4000;
    record->field_16 = 0x4000;
    record->field_18 = template->field_14;
    record->field_1a = template->field_16;
    record->field_1c = template->field_18;
    record->field_1e = template->field_1a;
    record->field_1f = template->field_1b;
    record->field_20 = template->field_1c;
    record->field_21 = template->field_1d;

    Func_08015028(template_index + 0x28f, text, 15);
    name_length = 0;
    while (name_length < 14 && text[name_length] != 0) {
        ((u8 *)record)[name_length] = text[name_length];
        name_length++;
    }
    if (suffix <= 8)
        ((u8 *)record)[name_length++] = suffix + '1';
    ((u8 *)record)[name_length] = 0;
    record->field_0e = 0;

    equipment_count = 0;
    for (group = 0; group < 4; group++) {
        s32 repeat;

        if (template->equipment[group] == 0 ||
            template->equipment_count[group] == 0)
            continue;
        for (repeat = 0; repeat < template->equipment_count[group];
             repeat++) {
            if (equipment_count < 15)
                record->equipment[equipment_count++] =
                    template->equipment[group];
        }
    }

    record->field_120 = template->field_20;
    record->field_129 = 0;
    record->template_id = template_id;
    Func_080798e0(slot, (u8 *)record + 0x24);
    Func_08077428(slot);
    record->field_12a = 1;
    if (record->template_id >= 158 && record->template_id <= 171)
        record->field_12a = 2;
    return 1;
}
