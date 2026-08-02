#include "layout_guard.h"
#include "types.h"

struct MenuState_080a8578 {
    u8 padding00[0x21a];
    u8 selected_character;
};

struct Character_080a8578 {
    u8 padding000[0x0f];
    u8 level;
    u8 padding010[0x114];
    s32 experience;
};

LAYOUT_OFFSET_GUARD(
    MenuState080a8578_SelectedCharacter,
    struct MenuState_080a8578,
    selected_character,
    0x21a);
LAYOUT_OFFSET_GUARD(
    Character080a8578_Level,
    struct Character_080a8578,
    level,
    0x0f);
LAYOUT_OFFSET_GUARD(
    Character080a8578_Experience,
    struct Character_080a8578,
    experience,
    0x124);

s16 *Func_08004938(s32);
s32 Func_08015030(s32, s16 *, s32);
s32 Func_08015258(s16 *, s32, s32, s32);
s32 Func_08077258(u8, s32);
struct Character_080a8578 *Func_08077008(u8);
void Func_08015120(s32, s32);
void Func_08002df0(void *);

/*
 * Render the help text for the alternate command pane.  The compact
 * seven-entry pane skips one message-table slot; the extended eight-entry
 * pane does not.  Entry 1 inserts the selected character's experience needed
 * for the next level, or switches to the max-level message at level 99.
 */
void Func_080a8578(s32 window, s32 selection, s32 extended_pane)
{
    struct MenuState_080a8578 *state =
        *(struct MenuState_080a8578 **)0x03001f2c;
    s16 *text;

    if (extended_pane == 0 && selection > 3)
        selection++;

    if (selection == 1) {
        u8 character_id = state->selected_character;
        struct Character_080a8578 *character =
            Func_08077008(character_id);

        if (character->level == 99) {
            selection = 8;
        } else {
            s32 next_level_experience =
                Func_08077258(character_id, character->level + 1);
            Func_08015120(
                next_level_experience - character->experience,
                5);
        }
    }

    text = Func_08004938(0x100);
    Func_08015030(0x0be6 + selection, text, 0x80);
    Func_08015258(text, window, 0, -1);
    Func_08002df0(text);
}
