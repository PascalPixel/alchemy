#include "TYPES.H"
#include "RUNTIME_MEM.H"
#include "SCENE.H"
#include "OWNER_STATE.H"
#include "CHARACTER.H"
#include "PARTY_STATE.H"
#include "RUNTIME_INTERFACES.H"
#include "ITEM.H"
#include "GAME_FLAGS.H"
#include "INVENTORY.H"
#include "RESOURCE.H"
#include "RESOURCE_IDS.H"

s32 GameState_InitDefaults();
s32 Game_ResetForNewGameFar(s32);


extern struct BattleUnit *gBattleOwnerStates;
extern const u8 Data_08080ec8[];

/* The existing byte-only multiplier prefix is used only at the stat store
   boundary below; ClassDefinition remains the full table-row owner. */
struct ClassRecord {
    u8 unknown_00[8];
    u8 hp;
    u8 pp;
    u8 attack;
    u8 defense;
    u8 agility;
    u8 luck;
};

struct StatWork {
    s32 hp;                     /* 0x00 */
    s32 pp;                     /* 0x04 */
    s32 attack;                 /* 0x08 */
    s32 defense;                /* 0x0c */
    s32 agility;                /* 0x10 */
    s32 unknown_14;
    s32 luck;                   /* 0x18 */
    s32 turns;                  /* 0x1c */
    s32 hp_regen;                /* 0x20 */
    s32 pp_regen;                /* 0x24 */
    s32 element[4][2];          /* 0x28 */
    s32 kind;                   /* 0x48 */
    s32 unknown_4c[2];
    s32 amount;                 /* 0x54 */
    struct ItemDefinition *item; /* 0x58 */
    s32 unknown_5c;
};

struct DjinnDefinition *Djinn_GetDefinition(s32 element, s32 djinn);

/* Distance between a stored value and one recomputed from its ratio. */
#define STAT_DIFF(a, b) ((a) - (b) < 0 ? (b) - (a) : (a) - (b))

void GameFlag_ClearBit(s32 flag);
s32 GameFlag_SetBit(s32 flag);

extern u8 gDebugMode;

void *Owner_GetState(u32 owner);

void Owner_RefreshAndResetZero(void)
{
    GameState_InitDefaults();
    Game_ResetForNewGameFar(0);
}

void *Trade_GetOfferState(s32 arg0)
{
    if (arg0 != 0) {
        return Owner_GetState(0x83);
    }
    return &gGameState.unknown_008[4];
}

u32 Party_GetAverageLevel(void)
{
    s32 count;
    s32 total;
    s32 i;

    total = 0;
    count = Party_CountActiveOwners();
    if (count == 0) {
        return 0;
    }
    for (i = 0; i < count; i++) {
        total += ((struct BattleUnit *)Owner_GetState(
            gGameState.active_owners[i]))->level;
    }
    total = total / count;
    return total;
}

void *Owner_GetState(u32 owner)
{
    struct BattleUnit *states;
    register u32 offset asm("r3"); /* FAKEMATCH: the scaled index in r3 */

    asm volatile("mov r3, lr" ::: "r3"); /* FAKEMATCH: the entry copy of lr */
    states = gGameState.owners;
    if (owner < 8)
        {
        register u8 *r asm("r0"); /* FAKEMATCH: the result in r0 */
        offset = sizeof(struct BattleUnit);
        offset *= owner;
        r = (u8 *)states + offset;
        return r;
        }
    if (owner - 0x80 < 6 && gBattleOwnerStates != NULL)
        {
        register u8 *r asm("r0"); /* FAKEMATCH: the result in r0 */
        u8 *t;
        offset = sizeof(struct BattleUnit);
        offset *= owner;
        t = (u8 *)gBattleOwnerStates + offset;
        r = t - 0xa600;
        return r;
        }
    return NULL;
}

const u8 *Owner_GetRecord(s32 selector)
{
    u32 record_index;

    record_index = selector - 8;
    if (record_index > 0xF9U) {
        record_index = 0;
    }
    return (const u8 *)&((const struct EnemyDefinition *)Data_08080ec8)[record_index];
}

void Runtime_CopyBytesDirectional(u8 *first, u8 *second, s32 count, s32 direction)
{
    s32 index;

    if (direction != 0) {
        index = 0;
        if (index < count) {
            do {
                *second++ = *first++;
                index++;
            } while (index < count);
        }
    } else if (count > 0) {
        index = count;
        do {
            *first++ = *second++;
            index--;
        } while (index != 0);
    }
}

/* Rebuilds a party member's derived statistics from the base values,
   equipped items, set Djinn, class multipliers and levels, clamps them and
   rescales current HP and PP to the new maxima. */
void Owner_RecalculateStats(s32 owner)
{
    struct StatWork *work;
    struct BattleUnit *st;
    s32 i, j, el;
    s32 value;
    s32 flag;
    s32 kind;
    s32 amount;
    s32 cap;
    s16 old;

    work = (struct StatWork *)Runtime_BumpAllocateAlternatePool(sizeof(struct StatWork));
    st = Owner_GetState(owner);
    work->hp = st->base_hp;
    work->pp = st->base_pp;
    work->attack = st->base_attack;
    work->defense = st->base_defense;
    work->agility = st->base_agility;
    work->luck = st->base_luck;
    work->turns = st->base_action_count & 15;
    work->hp_regen = st->base_hp_regen;
    work->pp_regen = st->base_pp_regen;
    {
        s16 *src = &st->base_elements[0].power;
        s32 *dst = work->element[0];
        for (i = 0; i < 4; i++) {
            dst[0] = src[0];
            dst[1] = src[1];
            src += 2;
            dst += 2;
        }
    }

    if (STAT_DIFF(st->max_hp * st->hp_gauge / 0x4000, st->hp) > 1
        || STAT_DIFF(st->max_pp * st->pp_gauge / 0x4000, st->pp) > 1) {
        st->hp_gauge = 0x4000;
        st->pp_gauge = 0x4000;
        st->hp = st->max_hp;
        st->pp = st->max_pp;
    }

    st->restraint &= ~3;
    if (st->restraint & 4)
        st->restraint |= 1;
    if (st->ready_pose)
        work->turns++;
    st->unknown_142[0] = 0;
    st->unknown_142[1] = 0;

    if (st->class_index) {
        for (i = 0; i < 15; i++) {
            if (!(st->inventory[i] & INVENTORY_EQUIPPED))
                continue;
            work->item = Item_GetDirect(st->inventory[i]);
            if (work->item->flags & ITEM_CURSED)
                st->restraint |= 3;
            /* FAKEMATCH: a do-while barrier keeps the defense load after the
               item bonus load, as the ROM schedules it */
            do {
                work->attack += work->item->primary_bonus;
            } while (0);
            work->defense += work->item->secondary_bonus;
            for (j = 0; j < 4; j++) {
                kind = work->item->effects[j].kind;
                amount = work->item->effects[j].amount;
                work->kind = kind;
                work->amount = amount;
                switch (work->kind) {
                case ITEM_EFFECT_NONE:
                    break;
                case ITEM_EFFECT_ADD_HP:
                    work->hp += work->amount;
                    break;
                case ITEM_EFFECT_ADD_HP_REGEN:
                    work->hp_regen += work->amount;
                    break;
                case ITEM_EFFECT_ADD_PP:
                    work->pp += work->amount;
                    break;
                case ITEM_EFFECT_ADD_PP_REGEN:
                    work->pp_regen += work->amount;
                    break;
                case ITEM_EFFECT_ADD_AGILITY:
                    work->agility += work->amount;
                    break;
                case ITEM_EFFECT_ADD_LUCK:
                    work->luck += work->amount;
                    break;
                case 15:
                    work->element[0][0] += work->amount;
                    break;
                case 16:
                    work->element[1][0] += work->amount;
                    break;
                case 17:
                    work->element[2][0] += work->amount;
                    break;
                case 18:
                    work->element[3][0] += work->amount;
                    break;
                case 19:
                    work->element[0][1] += work->amount;
                    break;
                case 20:
                    work->element[1][1] += work->amount;
                    break;
                case 21:
                    work->element[2][1] += work->amount;
                    break;
                case 22:
                    work->element[3][1] += work->amount;
                    break;
                case 23:
                    st->unknown_142[0] += work->amount;
                    break;
                case 24:
                    st->unknown_142[1] += work->amount;
                    break;
                case 25:
                    st->restraint |= 8;
                    break;
                case 26:
                    work->turns += work->amount;
                    break;
                }
            }
        }
        if (st->restraint & 8)
            st->restraint &= ~9;

        for (el = 0; el < 4; el++) {
            u32 bits = st->djinn_active[el];

            for (i = 0; i < 20; i++) {
                if (bits & (1 << i)) {
                    struct DjinnDefinition *djinn = Djinn_GetDefinition(el, i);

                    work->hp += djinn->hp;
                    work->pp += djinn->pp;
                    work->attack += djinn->attack;
                    work->defense += djinn->defense;
                    work->agility += djinn->agility;
                    work->luck += djinn->luck;
                }
            }
        }

        {
            /* FAKEMATCH: the full ClassDefinition changes five multiplier-load
               and stat-store orders in the same 2584-byte native module.
               Keep the existing byte-only prefix at this alias boundary. */
            struct ClassRecord *class = (struct ClassRecord *)Owner_GetRecordStride84(st->class_index);

            work->hp = work->hp * class->hp / 10;
            work->pp = work->pp * class->pp / 10;
            work->attack = work->attack * class->attack / 10;
            work->defense = work->defense * class->defense / 10;
            work->agility = work->agility * class->agility / 10;
            work->luck = work->luck * class->luck / 10;
        }

        for (i = 0; i < 15; i++) {
            if (!(st->inventory[i] & INVENTORY_EQUIPPED))
                continue;
            work->item = Item_GetDirect(st->inventory[i]);
            for (j = 0; j < 4; j++) {
                kind = work->item->effects[j].kind;
                amount = work->item->effects[j].amount;
                work->kind = kind;
                kind -= ITEM_EFFECT_SCALE_HP; /* rate effects 7..14 scale a statistic in tenths */
                work->amount = amount;
                switch (kind) {
                case 0:
                    work->hp = work->hp * work->amount / 10;
                    break;
                case 1:
                    work->hp_regen = work->hp_regen * work->amount / 10;
                    break;
                case 2:
                    work->pp = work->pp * work->amount / 10;
                    break;
                case 3:
                    work->pp_regen = work->pp_regen * work->amount / 10;
                    break;
                case 4:
                    work->attack = work->attack * work->amount / 10;
                    break;
                case 5:
                    work->defense = work->defense * work->amount / 10;
                    break;
                case 6:
                    work->agility = work->agility * work->amount / 10;
                    break;
                case 7:
                    work->luck = work->luck * work->amount / 10;
                    break;
                }
            }
        }
    }

    work->attack = work->attack * (st->attack_modifier + 8) / 8;
    work->defense = work->defense * (st->defense_modifier + 8) / 8;
    work->agility = work->agility * (st->agility_modifier + 8) / 8;
    for (i = 0; i < 4; i++)
        work->element[i][0] += (st->element_modifier[i] * st->element_modifier[i] + st->element_modifier[i]) * 5;
    for (i = 0; i < 4; i++)
        work->element[i][1] += st->res_modifier * 20;

    if (st->class_index) {
        flag = 0;
        switch (st->class_id) {
        case 0:
            flag = GameFlag_Test(0x110);
            break;
        case 1:
            flag = GameFlag_Test(0x112);
            break;
        case 2:
            flag = GameFlag_Test(0x113);
            break;
        case 3:
            flag = GameFlag_Test(0x111);
            break;
        case 5:
            flag = GameFlag_Test(0x112);
            break;
        }
        if (flag)
            work->pp_regen += 4;
    }

    if (work->attack < 0)
        work->attack = 0;
    if (work->attack > 999)
        work->attack = 999;
    if (work->defense < 0)
        work->defense = 0;
    if (work->defense > 999)
        work->defense = 999;
    if (work->agility < 0)
        work->agility = 0;
    if (work->agility > 999)
        work->agility = 999;
    if (work->luck < 0)
        work->luck = 0;
    if (work->luck > 99)
        work->luck = 99;
    if (work->turns < 0)
        work->turns = 0;
    if (work->turns > 2)
        work->turns = 2;
    if (work->hp_regen < 0)
        work->hp_regen = 0;
    if (work->hp_regen > 10000)
        work->hp_regen = 10000;
    if (work->pp_regen < 0)
        work->pp_regen = 0;
    if (work->pp_regen > 200)
        work->pp_regen = 200;
    for (i = 0; i < 4; i++) {
        if (work->element[i][0] < 0)
            work->element[i][0] = 0;
        if (work->element[i][0] > 200)
            work->element[i][0] = 200;
        if (work->element[i][1] < 0)
            work->element[i][1] = 0;
        if (work->element[i][1] > 200)
            work->element[i][1] = 200;
    }

    st->attack = work->attack;
    st->defense = work->defense;
    st->agility = work->agility;
    st->luck = work->luck;
    st->action_entry_count = work->turns;
    st->hp_regen = work->hp_regen;
    st->pp_regen = work->pp_regen;
    for (i = 0; i < 4; i++) {
        st->elements[i].power = work->element[i][0];
        st->elements[i].resist = work->element[i][1];
    }

    cap = 9999;
    if (st->class_index)
        cap = 1999;

    old = st->max_hp;
    if (work->hp < 0)
        work->hp = 0;
    if (work->hp > cap)
        work->hp = cap;
    st->max_hp = work->hp;
    if (old != st->max_hp) {
        value = work->hp * st->hp_gauge / 0x4000;
        if (value < 0)
            value = 0;
        if (value > cap)
            value = cap;
        if (st->hp != 0 && value == 0)
            value = 1;
        st->hp = value;
    }

    old = st->max_pp;
    if (work->pp < 0)
        work->pp = 0;
    if (work->pp > cap)
        work->pp = cap;
    st->max_pp = work->pp;
    if (old != st->max_pp) {
        value = work->pp * st->pp_gauge / 0x4000;
        if (value < 0)
            value = 0;
        if (value > cap)
            value = cap;
        if (st->pp != 0 && value == 0)
            value = 1;
        st->pp = value;
    }

    Runtime_BumpFree(work);
}

void GameFlag_RefreshLureCap(void)
{
    s32 count;
    s32 n;

    GameFlag_ClearBit(0x167);
    count = Party_CountActiveOwners();
    for (n = 0; n < count; n++) {
        struct BattleUnit *owner;
        s32 i;

        owner = Owner_GetState(gGameState.active_owners[n]);
        for (i = 0; i < 15; i++) {
            if (owner->inventory[i] & INVENTORY_EQUIPPED) {
                struct ItemEffect *effect;
                s32 j;

                effect = Item_GetDirect(owner->inventory[i])->effects;
                for (j = 0; j < 4; j++) {
                    u8 kind;

                    kind = effect->kind;
                    effect++;
                    if (kind == 27) {
                        GameFlag_SetBit(0x167);
                    }
                }
            }
        }
    }
}

u16 Runtime_GetBuildStampTime(void)
{
    u8 *digits;
    s32 hourTens;
    s32 hourUnits;
    s32 minuteTens;
    s32 minuteUnits;
    s32 secondTens;
    s32 secondUnits;
    s32 hours;
    s32 minutes;
    s32 seconds;
    s32 packed;
    s32 shifted;
    s32 result;

    digits = Resource_GetTableEntry((s32)&ResourceId_BuildStamp);
    hourTens = *digits;
    hours = (hourTens - '0') * 10;
    digits++;
    hourUnits = *digits;
    digits++;
    hours += hourUnits - '0';
    minuteTens = *digits;
    minutes = (minuteTens - '0') * 10;
    digits++;
    minuteUnits = *digits;
    digits++;
    minutes += minuteUnits - '0';
    secondTens = digits[0];
    seconds = (secondTens - '0') * 10;
    secondUnits = digits[1];
    seconds += secondUnits - '0';
    packed = (((hours << 4) + minutes) << 6) + seconds;
    shifted = 0x80 << 21;
    shifted |= packed << 16;
    result = shifted >> 16;
    if (gDebugMode != 0) {
        result |= (s32)0xffff8000;
    }
    return (u16)result;
}
