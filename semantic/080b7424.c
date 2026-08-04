#include "types.h"

struct CharacterMetadata_080b7424 {
    u8 unknown_000[0x128];
    u8 type;
};

struct CharacterMetadata_080b7424 *Func_08077008(s32 character_id);
s32 Func_080c23c0(s32 type);

/*
 * Lay out up to six character markers around a centered vertical span.  Most
 * markers use x=-80; two wide portraits are moved right to x=-50.
 */
void Func_080b7424(
    u16 *character_ids,
    s32 count,
    s32 *x_positions,
    s32 *y_positions)
{
    s32 spacing = count > 4 ? 27 : 30;
    s32 cursor = ((count - 1) * spacing) / 2;
    s32 index;

    for (index = 0; index < count; index++) {
        u16 character_id = character_ids[index];
        s32 leading_width = 0;
        s32 trailing_width = 25;

        x_positions[index] = -80;

        if (index != 0) {
            leading_width = 25;
            if (character_id != 254 && character_id != 255) {
                struct CharacterMetadata_080b7424 *metadata =
                    Func_08077008(character_id);

                leading_width = 27;
                if (Func_080c23c0(metadata->type) == 0) {
                    leading_width = 38;
                }
                if (metadata->type == 0x94 || metadata->type == 0x79) {
                    x_positions[index] = -50;
                }
            }
        }

        y_positions[index] = cursor - (leading_width >> 1);

        if (character_id != 254 && character_id != 255) {
            struct CharacterMetadata_080b7424 *metadata =
                Func_08077008(character_id);

            trailing_width = 27;
            if (Func_080c23c0(metadata->type) == 0) {
                trailing_width = 38;
            }
        }
        cursor = y_positions[index] - (trailing_width >> 1);
    }
}
