#include "types.h"

#define BattlePresentation_BuildActions Func_080b9934

struct BattleActionRecord {
    s16 unit_id;
    u8 reserved_02[2];
    u16 value;
    s16 kind;
    u8 reserved_08[8];
};

struct BattlePresentationTransition {
    s32 angle;
};

struct CharacterRuntimeRecord {
    u8 reserved_00[0x12b];
    u8 presentation_side;
};

extern u8 *Data_03001e74;
extern struct BattlePresentationTransition *Data_03001f00;

void Func_080b90ac(void);
void Func_080b98b4(s32 value);
void Func_080770c8(s32 id);
void Func_080b8fd4(s32 value);
void Func_08015218(void);
s32 Func_080b920c(struct BattleActionRecord *actions);
s32 Func_080b90f8(void);
s32 Func_080b9724(struct BattleActionRecord *actions, s32 count);
s32 Func_080b60a0(void);
s32 Func_080b9324(struct BattleActionRecord *actions);
void Func_080b9470(struct BattleActionRecord *actions, s32 count);
struct CharacterRuntimeRecord *Func_08077008(s32 unit_id);
void Func_080770d0(s32 id);
void Func_080b7f9c(void);

s32 BattlePresentation_BuildActions(struct BattleActionRecord *actions)
{
    struct CharacterRuntimeRecord *character;
    u8 *battle;
    u8 *mode;
    s32 count;
    s32 added;
    s32 i;

    battle = Data_03001e74;
    {
        struct BattleActionRecord *reset_record =
            (struct BattleActionRecord *)(battle + 187 * 4);

        i = 0;
        do {
            i++;
            reset_record->unit_id = 0xff;
            reset_record->value = 0x8000;
            reset_record++;
        } while (i <= 19);
    }

    Func_080b90ac();
    Func_080b98b4(8);
    Func_080770c8(0x16b);
    mode = battle + 69;
    Func_080b8fd4(0);
    Func_08015218();

    if (*mode != 2) {
        count = Func_080b920c(actions);
        if (count < 0)
            goto finish;
        if (count != 0 && actions[0].kind == 99 && Func_080b90f8() == 0)
            *mode = 2;
    } else {
        count = 0;
    }

    if (Data_03001e74[68] != 0) {
        added = Func_080b9724(actions, count);
        if (Func_080b60a0() < 0 || added < 0) {
            count = -1;
            goto finish;
        }
        count += added;
    } else {
        count += Func_080b9324(actions + count);
    }

    Func_080b9470(actions, count);
    if (count > 0) {
        struct BattleActionRecord *action = actions;

        i = count;
        do {
            if (action->kind == 3 || action->kind == 7) {
                character = Func_08077008(action->unit_id);
                character->presentation_side = action->kind == 3 ? 1 : 2;
            }
            action++;
            i--;
        } while (i != 0);
    }

finish:
    Func_080770d0(0x16b);
    Func_080b7f9c();
    Data_03001f00->angle = 0x2000;
    return count;
}
