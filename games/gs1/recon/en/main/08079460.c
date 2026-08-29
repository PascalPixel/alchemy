#include "types.h"

struct BattleUnitDefinition {
    u8 reserved_00[15]; u8 level; u16 hp; u16 pp; u16 attack; u16 defense;
    u16 agility; u8 luck; u8 turns; u8 ai_group; u8 ai_flags; u32 rewards;
    u8 reserved_24[4]; u16 ability_groups[4]; u8 ability_counts[4];
    u8 reserved_34[32];
};

struct BattleUnitRecord {
    u8 name[14]; u8 name_flags; u8 level; u16 hp; u16 pp; s16 hp_ratio;
    s16 pp_ratio; u16 attack; u16 defense; u16 agility; u8 luck; u8 turns;
    u8 ai_group; u8 ai_flags; u8 reserved_22[2]; u8 equipment_and_stats[180];
    u16 abilities[15]; u8 reserved_f6[42]; u32 rewards; u8 reserved_124[4];
    u8 definition_id; u8 status; u8 side;
};

void Func_08004970(s32);
void *Func_08077394(s32);
void Func_080072f0(void *, s32, s32, s32);
void Func_08015028(s32, u16 *, s32);
u32 Func_080798e0(s32, s16 (*)[2]);
void Func_08077428(s32);

#define BattleUnit_Assign Func_08079460
s32 BattleUnit_Assign(s32 unitId, s32 definitionId, s32 nameSuffix)
{
    struct BattleUnitRecord *unit;
    const struct BattleUnitDefinition *definition;
    u16 nameBuffer[15];
    u32 definitionIndex;
    s32 nameLength;
    s32 group;
    s32 count;
    s32 abilitySlot;

    definitionIndex = definitionId - 8;
    if (unitId < 128 || unitId > 134 || definitionIndex > 242)
        return 0;
    Func_08004970(96);
    unit = (struct BattleUnitRecord *)Func_08077394(unitId);
    Func_080072f0(unit, 332, 0, 0x03000164);
    if (definitionIndex > 164)
        definitionIndex = 0;
    definition = (const struct BattleUnitDefinition *)0x08080ec8 + definitionIndex;

    unit->level = definition->level;
    unit->hp = definition->hp;
    unit->pp = definition->pp;
    unit->hp_ratio = 0x4000;
    unit->pp_ratio = 0x4000;
    unit->attack = definition->attack;
    unit->defense = definition->defense;
    unit->agility = definition->agility;
    unit->luck = definition->luck;
    unit->turns = definition->turns;
    unit->ai_group = definition->ai_group;
    unit->ai_flags = definition->ai_flags;

    Func_08015028(definitionIndex + 655, nameBuffer, 15);
    nameLength = 0;
    while (nameLength < 14 && nameBuffer[nameLength] != 0) {
        unit->name[nameLength] = (u8)nameBuffer[nameLength];
        nameLength++;
    }
    if (nameSuffix <= 8)
        unit->name[nameLength++] = (u8)(nameSuffix + '1');
    unit->name[nameLength] = 0;
    unit->name_flags = 0;

    abilitySlot = 0;
    for (group = 0; group < 4; group++) {
        if (definition->ability_groups[group] == 0)
            continue;
        count = definition->ability_counts[group];
        while (count-- != 0 && abilitySlot <= 14)
            unit->abilities[abilitySlot++] = definition->ability_groups[group];
    }

    unit->rewards = definition->rewards;
    unit->status = 0;
    unit->definition_id = (u8)definitionId;
    Func_080798e0(unitId, (s16 (*)[2])unit->equipment_and_stats);
    Func_08077428(unitId);
    unit->side = 1;
    if (unit->definition_id >= 158 && unit->definition_id <= 171)
        unit->side = 2;
    return 1;
}
