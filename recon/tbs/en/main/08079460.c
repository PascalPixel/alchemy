/* Draft, not exact (2026-09-24): 408 bytes for the 412-byte owner, 142
   halfwords differ (was 192). Unit setup, the definition copy and the
   ownership tail match. Residuals: the ROM keeps the name buffer's address
   in a stack-spilled local (add r4, sp, #4; str r4, [sp]) with a 36-byte
   frame and walks the name by index with the test at the loop bottom, and
   its equipment loop keeps the count in r8 with item offsets in sl and ip;
   a named buffer pointer local alone makes it worse (184).
   Func_08015028 copies a message into a u16 buffer. */

#include "TYPES.H"

/* An 84-byte enemy definition. */
struct EnemyDefinition {
    u8 unknown_00[15];
    u8 level;                   /* 0x0f */
    u16 hp;                     /* 0x10 */
    u16 pp;                     /* 0x12 */
    u16 attack;                 /* 0x14 */
    u16 defense;                /* 0x16 */
    u16 agility;                /* 0x18 */
    u8 luck;                    /* 0x1a */
    u8 turns;                   /* 0x1b */
    u8 stat_1c;                 /* 0x1c */
    u8 stat_1d;                 /* 0x1d */
    u8 unknown_1e[2];
    u32 rewards;                /* 0x20 */
    u8 unknown_24[4];
    u16 items[4];               /* 0x28 */
    u8 counts[4];               /* 0x30 */
    u8 unknown_34[0x20];
};

struct BattleUnitRecord {
    u8 name[14];                /* 0x00 */
    u8 name_flags;              /* 0x0e */
    u8 level;                   /* 0x0f */
    u16 base_hp;                /* 0x10 */
    u16 base_pp;                /* 0x12 */
    u16 hp_ratio;               /* 0x14 */
    u16 pp_ratio;               /* 0x16 */
    u16 attack;                 /* 0x18 */
    u16 defense;                /* 0x1a */
    u16 agility;                /* 0x1c */
    u8 luck;                    /* 0x1e */
    u8 turns;                   /* 0x1f */
    u8 stat_20;                 /* 0x20 */
    u8 stat_21;                 /* 0x21 */
    u8 unknown_22[2];
    u8 digits[16];              /* 0x24 */
    u16 max_hp;                 /* 0x34 */
    u16 max_pp;                 /* 0x36 */
    u16 hp;                     /* 0x38 */
    u16 pp;                     /* 0x3a */
    u8 unknown_3c[0x9c];
    u16 equipment[15];          /* 0xd8 */
    u8 unknown_0f6[0x2a];
    u32 rewards;                /* 0x120 */
    u8 unknown_124[4];
    u8 enemy;                   /* 0x128 */
    u8 class_id;                /* 0x129 */
    u8 side;                    /* 0x12a */
};

extern struct EnemyDefinition Data_08080ec8[];

struct BattleUnitRecord *Owner_GetState(s32 owner);
void Func_08015028(s32 message, u16 *buffer, s32 length);
u32 Owner_BuildDigitTiles(s32 owner, void *digits);
void Owner_RecalculateStats(s32 owner);

#define IwramClear ((void (*)(void *, s32))0x03000164)

/* Fills battle unit 128..134 from enemy definition 8..250 and names it,
   with a letter-free number suffix for repeated enemies. */
s32 BattleUnit_Assign(s32 unit_id, s32 enemy_id, s32 suffix)
{
    struct BattleUnitRecord *unit;
    struct EnemyDefinition *enemy;
    u16 name[15];
    u32 index;
    s32 n;
    s32 count;
    s32 k;
    s32 q;

    index = enemy_id - 8;
    if (unit_id <= 127)
        return 0;
    if (unit_id > 134)
        return 0;
    if (index > 242)
        return 0;
    unit = Owner_GetState(unit_id);
    IwramClear(unit, 332);
    if (index > 164)
        index = 0;
    enemy = &Data_08080ec8[index];
    unit->level = enemy->level;
    unit->base_hp = enemy->hp;
    unit->hp = enemy->hp;
    unit->max_hp = enemy->hp;
    unit->base_pp = enemy->pp;
    unit->pp = enemy->pp;
    unit->max_pp = enemy->pp;
    unit->hp_ratio = 0x4000;
    unit->pp_ratio = 0x4000;
    unit->attack = enemy->attack;
    unit->defense = enemy->defense;
    unit->agility = enemy->agility;
    unit->luck = enemy->luck;
    unit->turns = enemy->turns;
    unit->stat_20 = enemy->stat_1c;
    unit->stat_21 = enemy->stat_1d;
    Func_08015028(index + 0x28f, name, 15);
    n = 0;
    while (name[n] != 0) {
        unit->name[n] = name[n];
        n++;
        if (n > 13)
            break;
    }
    if (suffix <= 8) {
        unit->name[n] = suffix + '1';
        n++;
    }
    unit->name[n] = 0;
    unit->name_flags = 0;
    count = 0;
    for (k = 0; k < 4; k++) {
        if (enemy->items[k] == 0 || enemy->counts[k] == 0)
            continue;
        for (q = enemy->counts[k]; q != 0; q--) {
            if (count <= 14)
                unit->equipment[count++] = enemy->items[k];
        }
    }
    unit->rewards = enemy->rewards;
    unit->class_id = 0;
    unit->enemy = enemy_id;
    Owner_BuildDigitTiles(unit_id, unit->digits);
    Owner_RecalculateStats(unit_id);
    unit->side = 1;
    if (unit->enemy <= 171 && unit->enemy >= 158)
        unit->side = 2;
    return 1;
}
