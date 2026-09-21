#include "TYPES.H"

#define BattlePresentation_ApplyUnitDamage Func_080b8db8
#define Character_GetRuntimeRecord Func_08077008
#define GetBattleObjectSlot Func_080b7dd0

struct CharacterRuntimeRecord {
    u8 reserved_00[56];
    s16 hp;
};

struct BattleMotionSlot {
    void *actor;
};

struct CharacterRuntimeRecord *Func_08077008(s32 unit_id);
struct BattleMotionSlot *Func_080b7dd0(s32 unit_id);
void Func_08009080(void *actor, s32 mode);
#define Object_SetMode Func_08009080
void Func_08015130(s32 mode);
void Func_08015118(void);
void Func_08015120(s32 value, s32 mode);
void Func_080151c8(s32 message_id);
void Func_080b8ec4(s32 unit_id);
#define BattleMotion_SetMode5AndActivateSlot Func_080b8ec4

/* Takes damage from a unit's HP, stopping at zero, and reports it with the
 * unit posed in mode 5: an optional opening line (a bitter blow for units 0
 * to 7, the party, and a critical hit for the others), the damage message,
 * and a downed message once HP reaches zero. The unit returns to mode 1 at
 * the end. */
void BattlePresentation_ApplyUnitDamage(u32 unit_id, s32 damage, s32 show_message, u8 *context)
{
    u8 local_context[4];
    struct CharacterRuntimeRecord *character;
    struct BattleMotionSlot *slot;

    if (context == 0) {
        context = local_context;
        context[0] = 0;
        context[1] = 0;
        context[2] = 0;
        context[3] = 0;
    }

    character = Character_GetRuntimeRecord(unit_id);
    character->hp -= damage;
    if (character->hp < 0)
        character->hp = 0;

    slot = GetBattleObjectSlot(unit_id);
    Object_SetMode(slot->actor, 5);
    Func_08015130(0);
    Func_08015118();

    if (unit_id <= 7) {
        if (show_message != 0)
            Func_080151c8(0x823);
        Func_08015120(damage, 5);
        Func_08015120(unit_id, 1);
        Func_080151c8(0x827);
    } else {
        if (show_message != 0)
            Func_080151c8(0x822);
        Func_08015120(damage, 5);
        Func_08015120(unit_id, 1);
        Func_080151c8(0x826);
        Func_08015120(unit_id, 1);
    }

    BattleMotion_SetMode5AndActivateSlot(unit_id);
    if (unit_id <= 7) {
        if (character->hp <= 0) {
            Func_08015120(unit_id, 1);
            Func_080151c8(0x825);
        }
    } else if (character->hp <= 0) {
        Func_08015120(unit_id, 1);
        Func_080151c8(0x838);
    }

    slot = GetBattleObjectSlot(unit_id);
    Object_SetMode(slot->actor, 1);
}
