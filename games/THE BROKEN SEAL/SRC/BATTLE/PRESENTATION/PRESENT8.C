#include "TYPES.H"
#include "OBJDISP.H"
#include "RAM_BUFFER.H"
#include "MOTION_OBJECT.H"
#include "BATTLE_WORK.H"
#include "BATTLE_RUNTIME.H"
#include "INVENTORY.H"


s32 Summon_IsEntryFlagged(s32 class_id);
s32 Summon_GetEntryValue(s32 class_id);
s32 Summon_GetEntryFlag1Field(s32 class_id);
s32 ResourceSlot_LoadFar(u32 slot, u32 *buffer, s32 value, u32 variant);
extern u16 Resource_SlotAssignments[];
s32 Inventory_FindEquippedFar(s32, s32);
extern u16 RomBytes_080c2a1c[];
extern u16 BattleUnit_WeaponAnimsClass1[];
extern u16 BattleUnit_WeaponAnimsClass2[];
extern u16 BattleUnit_WeaponAnimsClass3[];
extern u16 BattleUnit_WeaponAnimsClass5[];

struct MotionObject *Object_CreateFar(s32 kind, s32 x, s32 y, s32 z);
s32 Inventory_GetEquippedItemFar(struct BattleUnit *owner, s32 type);
s32 SummonSlot_RegisterActorSprites(s32);
s32 BattleUnit_LookupWeaponValueByClass(s32);
s32 ArcTan2(s32, s32);
extern const u8 BattlePres_ActorObjectScript[];

/* Step table for battle placement: signed bytes in (x, y) pairs. */
extern const s8 BattlePlacement_StepPairs[];
s32 Resource_FindFreeSlot(s32 key);

void Summon_LayoutPositions(u16 *unit_ids, s32 count, s32 *x, s32 *z);

s32 BattleMotion_GetSlotField14(s32 id)
{
    return GetBattleObjectSlot(id)->palette;
}

s32 Summon_ClassValid(s32 summon)
{
    struct BattleSession *ptr;
    s32 retval;
    s32 i;

    retval = Summon_IsEntryFlagged(summon);
    ptr = gBattleWork;
    for (i = 0; i <= 5; i++) {
        if (ptr->sprite_slots[i] != 0)
            continue;
        if (retval != 0)
            break;
        if (i <= 4 && ptr->sprite_slots[i + 1] == 0)
            break;
    }
    return i != 6;
}

s32 SummonSlot_RegisterActorSprites(s32 unit)
{
    s32 pass;
    struct BattleSession *table = gBattleWork;
    struct BattleUnit *actor = Owner_GetStateFar(unit);
    s32 single_slot = Summon_IsEntryFlagged(actor->class_id);
    s32 result = 0;
    s32 sprite_value = Summon_GetEntryValue(actor->class_id);

    /* FAKEMATCH: an opaque copy keeps cse from folding pass to the constant zero */
    asm("mov %0, %1" : "=r"(pass) : "r"(result));
    do {
        s32 slot;

        if (actor->class_index != 0)
            continue;

        for (slot = 0; slot <= 5; slot++) {
            if (table->sprite_slots[slot] != 0)
                continue;
            if (single_slot)
                break;
            if (slot <= 4 && table->sprite_slots[slot + 1] == 0)
                break;
        }

        if (slot == 6)
            break;

        {
            s32 flag = Summon_GetEntryFlag1Field(actor->class_id);
            s32 buffer_addr = (slot << 14) + (s32)Ram_ActorSpriteSlots;

            /* FAKEMATCH: an empty use of sprite_value steers it into r7 as the ROM allocates it */
            asm("" : "+r"(sprite_value));
            if (ResourceSlot_LoadFar(slot, (u32 *)buffer_addr, sprite_value + pass, flag) == 0)
                return 0;
        }

        if (pass == 0)
            result = (slot << 12) | sprite_value;

        table->sprite_slots[slot] = (s16)unit;
        if (!single_slot)
            table->sprite_slots[slot + 1] = (s16)unit;

        if (sprite_value != 476 && sprite_value != 483)
            break;
    } while (++pass <= 1);

    return result;
}

s32 BattleMotion_ReleaseObjectSlotByValue(s32 value)
{
    u8 *base;
    s32 offset;
    s32 index;
    s16 item;

    base = (u8 *)gBattleWork;
    index = 0;
    do {
        offset = index * 2 + 4;
        item = *(s16 *)(offset + (u32)base);
        if (item == value) {
            ResourceSlot_LoadFar(index, 0, 0, 0);
            *(s16 *)(offset + (u32)base) = 0;
        }
        index++;
    } while (index <= 5);
}

s32 Resource_FindFreeSlot(s32 key)
{
    s32 index;
    s32 entry;
    u32 value;
    s32 result;

    for (index = 0;; index++) {
        entry = Resource_SlotAssignments[index];
        if (key == (entry & 0x1ff)) {
            /* FAKEMATCH: rereads the entry as [table, index]; cse otherwise swaps the operands */
            asm("ldrh %0, [%1, %2]" : "=r"(value) : "r"(Resource_SlotAssignments), "r"(index * 2));
            result = value >> 9;
            goto done;
        }
        if ((s16)entry == -1)
            break;
    }
    result = 6;
done:
    return result;
}

/* battle/unit/lookup_weapon_value_by_class.c */
s32 BattleUnit_LookupWeaponValueByClass(s32 id)
{
    struct BattleUnit *state;
    s32 entry;
    s32 result;

    state = Owner_GetStateFar(id);
    entry = Inventory_FindEquippedFar(id, 1);
    result = 0;
    if (entry >= 0) {
        s32 sel;

        sel = Resource_FindFreeSlot(state->inventory[entry] & 0x1FF);
        switch (state->class_id) {
        case 0:
            result = RomBytes_080c2a1c[sel];
            break;
        case 1:
            result = BattleUnit_WeaponAnimsClass1[sel];
            break;
        case 2:
            result = BattleUnit_WeaponAnimsClass2[sel];
            break;
        case 3:
            result = BattleUnit_WeaponAnimsClass3[sel];
            break;
        case 4:
            break;
        case 5:
            result = BattleUnit_WeaponAnimsClass5[sel];
            break;
        }
    }
    return result;
}

void BattlePresentation_SpawnActorObject(struct BattleObjectSlot *actor, s32 unit, s32 x, s32 y)
{
    s32 fixed_x;
    s32 fixed_y;
    s32 actor_flag;
    s32 sprite;
    s32 existing_sprite;
    struct BattleUnit *unit_record;
    struct MotionObject *object;
    s32 position;
    s32 anim;
    u8 class_id;

    fixed_x = x << 16;
    fixed_y = y << 16;
    object = Object_CreateFar(0xf000, fixed_x, 0, fixed_y);
    unit_record = Owner_GetStateFar(unit);
    actor_flag = 0;
    existing_sprite = SummonSlot_RegisterActorSprites(unit);

    if (unit_record->class_index == 0) {
        sprite = Summon_GetEntryValue(unit_record->class_id);
        if (existing_sprite == 0)
            actor_flag = Summon_GetEntryFlag1Field(unit_record->class_id);
        else
            sprite = existing_sprite;
    } else {
        switch (unit_record->class_id) {
        case 1: sprite = 301; break;
        case 3: sprite = 303; break;
        case 2: sprite = 302; break;
        case 5: sprite = 305; break;
        case 0:
        default: sprite = 300; break;
        }
        if ((u32)unit > 7)
            actor_flag = 1;
    }

    actor->scale = 0x10000;
    switch (unit_record->class_id) {
    case 78: actor->scale = 0x19999; break;
    case 89: actor->scale = 0x18ccc; break;
    case 130: actor->scale = 0x13333; break;
    case 131: actor->scale = 0x19999; break;
    case 138: actor->scale = 0x18000; break;
    case 147: actor->scale = 0x1cccc; break;
    case 149: actor->scale = 0x1cccc; break;
    case 29: actor->scale = 0x10000; break;
    case 121: actor->scale = 0x1b333; break;
    case 148: actor->scale = 0x18000; break;
    case 150: actor->scale = 0x18000; break;
    case 151: actor->scale = 0x18000; break;
    case 152: actor->scale = 0x18000; break;
    case 153: actor->scale = 0x18000; break;
    case 154: actor->scale = 0x18000; break;
    case 155: actor->scale = 0x18000; break;
    case 156: actor->scale = 0x18000; break;
    case 157: actor->scale = 0x18000; break;
    case 47: actor->scale = 0x13333; break;
    case 48: actor->scale = 0x13333; break;
    case 49: actor->scale = 0x16666; break;
    case 84: actor->scale = 0x10000; break;
    case 85: actor->scale = 0x14000; break;
    case 128: actor->scale = 0x16666; break;
    case 129: actor->scale = 0x16666; break;
    case 94: actor->scale = 0x18000; break;
    case 98: actor->scale = 0x14ccc; break;
    case 110: actor->scale = 0x13333; break;
    case 132: actor->scale = 0x10ccc; break;
    case 133: actor->scale = 0x10ccc; break;
    case 134: actor->scale = 0x11999; break;
    case 135: actor->scale = 0x11999; break;
    case 136: actor->scale = 0x13333; break;
    case 137: actor->scale = 0x13333; break;
    case 141: actor->scale = 0x18000; break;
    case 144: actor->scale = 0x13333; break;
    case 145: actor->scale = 0x18000; break;
    case 146: actor->scale = 0x18ccc; break;
    case 52: actor->scale = 0x14000; break;
    case 105: actor->scale = 0x14000; break;
    case 18: case 19: case 20: case 21: case 30:
    case 68: case 69: case 70: case 92:
    case 122: case 123: case 124: case 125: case 126:
        actor->scale = 0xe666;
        break;
    }

    actor->object = object;
    actor->anchor_x = fixed_x;
    actor->anchor_z = fixed_y;
    actor->palette = actor_flag;
    actor->resource = sprite;
    anim = BattleUnit_LookupWeaponValueByClass(unit);
    actor->animation = 0;
    actor->animation_entry = 0;
    actor->effect_entry = 0;
    actor->active = 0;
    actor->fading = 0;
    actor->effect = 0x1fe;
    class_id = unit_record->class_id;
    actor->overlay = anim;

    if (class_id <= 1 && Inventory_GetEquippedItemFar((struct BattleUnit *)unit_record, 1) == 15) {
        if (unit_record->class_id == 0) {
            sprite = 480;
            actor->resource = sprite;
        } else {
            sprite = 482;
            actor->resource = sprite;
        }
        actor->overlay = 0;
    }

    position = ArcTan2(y / 8, x) + 0x8000;
    object->angle = position;
    object->unknown_59 = 3;
    object->motion_flags = 2;
    if (unit_record->class_index == 0) {
        object->scale_x = 0x14ccc;
        object->scale_y = 0x14ccc;
    } else {
        object->scale_x = 0x10000;
        object->scale_y = 0x10000;
    }
    ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)BattlePres_ActorObjectScript);
}

/*
 * Read one (x, y) pair from the step table.  Entries are pairs, so the index
 * is doubled and the second component read at index + 1.  A leaf with no
 * prologue; the owner includes the single pool word holding the table base.
 * No call site is known -- the address is reached by a computed value, or by
 * nothing at all.
 */
void BattlePlacement_GetStepPair(s32 index, s32 *x, s32 *y)
{
    index *= 2;
    *x = BattlePlacement_StepPairs[index];
    *y = BattlePlacement_StepPairs[index + 1];
}

void Summon_LayoutPositions(u16 *actor_ids, s32 count, s32 *x_positions, s32 *z_positions)
{
    s32 total_spacing = count <= 4 ? 30 : 27;
    s32 z = ((count - 1) * total_spacing) / 2;
    s32 index;

    for (index = 0; index != count; index++) {
        s32 spacing = 0;

        x_positions[index] = -80;
        if (index != 0) {
            struct BattleUnit *actor;

            spacing = 25;
            if ((u16)(actor_ids[index] - 254) > 1) {
                actor = Owner_GetStateFar(actor_ids[index]);
                spacing = Summon_IsEntryFlagged(actor->class_id) ? 27 : 38;
                if (actor->class_id == 148 || actor->class_id == 121) {
                    x_positions[index] = -50;
                }
            }
        }
        z -= spacing / 2;
        z_positions[index] = z;
        spacing = 25;
        if ((u16)(actor_ids[index] - 254) > 1) {
            struct BattleUnit *actor = Owner_GetStateFar(actor_ids[index]);

            spacing = Summon_IsEntryFlagged(actor->class_id) ? 27 : 38;
        }
        z -= spacing / 2;
    }
}

s32 Summon_FindSlot(void)
{
    s32 i;
    s32 id;

    for (i = 0; i <= 5; i++) {
        id = i + 0x80;
        if (Owner_GetStateFar(id)->status_12a == 0)
            break;
    }
    if (i == 6)
        return -1;
    return id;
}

/*
 * Lays out the battle's enemy list again: collects up to six ids,
 * computes their standing positions and moves every unit still present
 * there.
 */
s32 Summon_Refresh(void)
{
    struct BattleSession *work = gBattleWork;
    u16 unit_ids[14];
    s32 x[6];
    s32 z[6];
    s32 count;
    s32 i;

    for (i = 0; i < 6 && work->enemy_units[i] != 0xff; i++)
        unit_ids[i] = work->enemy_units[i];
    count = i;
    Summon_LayoutPositions(unit_ids, count, x, z);
    for (i = 0; i < count; i++) {
        s32 id = work->enemy_units[i];

        if (id != 0xfe) {
            struct BattleObjectSlot *object = GetBattleObjectSlot(id);

            object->anchor_x = x[i] << 16;
            object->anchor_z = z[i] << 16;
        }
    }
}

s32 BattleParty_PrepareActiveOwners(u16 *ids);

/*
 * Stands every unit at its place: the party in the order
 * BattleParty_PrepareActiveOwners lists it, each member's place recorded in
 * the session, then the enemy list at its computed positions.
 */
void BattleUnit_RefreshPlacement(void)
{
    struct BattleSession *work;
    u16 ids[14];
    s32 x[6];
    s32 z[6];
    s32 count;
    s32 i;
    s32 place;
    s32 stand;
    s32 n;
    s32 value;
    u8 *p;

    place = 0;
    stand = 0;
    work = gBattleWork;
    count = BattleParty_PrepareActiveOwners(ids);
    value = 0xff;
    for (i = 13; i >= 0; i--)
        work->placement[i] = value;
    p = &work->placement[13];
    for (n = 5, value = 13; n >= 0; n--, value--)
        *p-- = value;
    for (i = 0; i < count; place++, i++) {
        s32 id = ids[i];

        work->placement[id] = place;
        BattlePresentation_SpawnActorObject(GetBattleObjectSlot(id), id,
            BattlePlacement_StepPairs[place * 2], BattlePlacement_StepPairs[place * 2 + 1]);
    }
    for (i = 0; i < 6 && work->enemy_units[i] != 0xff; i++)
        ids[i] = work->enemy_units[i];
    count = i;
    Summon_LayoutPositions(ids, count, x, z);
    for (i = 0; i < count; i++, stand++) {
        s32 id = work->enemy_units[i];

        if (id != 0xfe)
            BattlePresentation_SpawnActorObject(GetBattleObjectSlot(id), id, x[stand], z[stand]);
    }
}
