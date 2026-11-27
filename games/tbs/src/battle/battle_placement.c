// Battle objects made and placed on the battlefield. Part of battle.c.

#include "battle_internal.h"

// The fdActorCreate kind a battle object's actor is made with: one that makes no sprite, its battle
// sprites being attached afterwards.
#define BATTLE_OBJECT_ACTOR_KIND 15

// Creates the battle object of a unit at (x, y) on the battlefield: its sprite, palette, size and
// weapon, turned to face the centre.
void btObjectMake(BATTLE_OBJECT *battleObject, Sint32 unitId, Sint32 x, Sint32 y)
{
    FIXED  fixedX;
    FIXED  fixedZ;
    Sint32 palette;
    Sint32 sprite;
    Sint32 registeredSprite;
    UNIT  *unit;
    ACTOR *actor;
    Sint32 facingAngle;
    Sint32 weaponSprite;
    Uint8  characterId;

    // Positions are 16.16 fixed point; y is the battlefield's depth.
    fixedX  = toFIXED(x);
    fixedZ  = toFIXED(y);
    actor   = fdActorCreateStub(ACTOR_CREATE_KIND(BATTLE_OBJECT_ACTOR_KIND), fixedX, 0, fixedZ);
    unit    = gmUnitGetStub(unitId);
    palette = 0;
    registeredSprite = btEnemySpriteLoadIntoSlots(unitId);

    // The sprite: an enemy's from its battle sprite row, a party member's by character; a character
    // on the enemy side, a link opponent, takes palette 1.
    if (unit->classId == CLASS_NONE)
    {
        sprite = btSpriteGetSprite(unit->id);
        if (registeredSprite == 0)
            palette = btSpriteGetPalette(unit->id);
        else
            sprite = registeredSprite;
    }
    else
    {
        switch (unit->id)
        {
            case GARET: sprite = SPRITE_BATTLE_GARET; break;
            case MIA  : sprite = SPRITE_BATTLE_MIA; break;
            case IVAN : sprite = SPRITE_BATTLE_IVAN; break;
            case JENNA: sprite = SPRITE_BATTLE_JENNA; break;
            case ISAAC:
            default   : sprite = SPRITE_BATTLE_ISAAC; break;
        }
        if (unitId < 0 || unitId > ALLY_MAX)
            palette = 1;
    }

    // The enemies drawn larger or smaller than their sprites, in 16.16. Bandit and Orc Captain are
    // set to 1.0 again.
    battleObject->scale = toFIXED(1);
    switch (unit->id)
    {
        case ENEMY_KILLER_APE                : battleObject->scale = toFIXED(8) / 5; break;
        case ENEMY_MANTICORE                 : battleObject->scale = toFIXED(31) / 20; break;
        case ENEMY_LIVING_STATUE             : battleObject->scale = toFIXED(6) / 5; break;
        case ENEMY_HYDROS_STATUE             : battleObject->scale = toFIXED(8) / 5; break;
        case ENEMY_ANT_LION                  : battleObject->scale = toFIXED(3) / 2; break;
        case ENEMY_TRET                      : battleObject->scale = toFIXED(9) / 5; break;
        case ENEMY_DEADBEARD                 : battleObject->scale = toFIXED(9) / 5; break;
        case ENEMY_BANDIT                    : battleObject->scale = toFIXED(1); break;
        case ENEMY_KRAKEN                    : battleObject->scale = toFIXED(17) / 10; break;
        case ENEMY_FUSION_DRAGON             : battleObject->scale = toFIXED(3) / 2; break;
        case ENEMY_MIMIC_MERCURY_LIGHTHOUSE  : battleObject->scale = toFIXED(3) / 2; break;
        case ENEMY_MIMIC_FUCHIN_FALLS_CAVE   : battleObject->scale = toFIXED(3) / 2; break;
        case ENEMY_MIMIC_ALTIN_PEAK          : battleObject->scale = toFIXED(3) / 2; break;
        case ENEMY_MIMIC_VAULT_CAVE          : battleObject->scale = toFIXED(3) / 2; break;
        case ENEMY_MIMIC_CROSSBONE_ISLE      : battleObject->scale = toFIXED(3) / 2; break;
        case ENEMY_MIMIC_ALTMILLER_CAVE      : battleObject->scale = toFIXED(3) / 2; break;
        case ENEMY_MIMIC_SUHALLA_DESERT      : battleObject->scale = toFIXED(3) / 2; break;
        case ENEMY_MIMIC_PIRATE_SHIP         : battleObject->scale = toFIXED(3) / 2; break;
        case ENEMY_TROLL                     : battleObject->scale = toFIXED(6) / 5; break;
        case ENEMY_CAVE_TROLL                : battleObject->scale = toFIXED(6) / 5; break;
        case ENEMY_BRUTAL_TROLL              : battleObject->scale = toFIXED(7) / 5; break;
        case ENEMY_ORC_CAPTAIN               : battleObject->scale = toFIXED(1); break;
        case ENEMY_ORC_LORD                  : battleObject->scale = toFIXED(5) / 4; break;
        case ENEMY_LIZARD_FIGHTER            : battleObject->scale = toFIXED(7) / 5; break;
        case ENEMY_LIZARD_KING               : battleObject->scale = toFIXED(7) / 5; break;
        case ENEMY_HOBGOBLIN                 : battleObject->scale = toFIXED(3) / 2; break;
        case ENEMY_GRYPHON                   : battleObject->scale = toFIXED(13) / 10; break;
        case ENEMY_CHIMERA                   : battleObject->scale = toFIXED(6) / 5; break;
        case ENEMY_AZART_IRON_SHIELD         : battleObject->scale = toFIXED(21) / 20; break;
        case ENEMY_AZART_PADDED_GLOVES       : battleObject->scale = toFIXED(21) / 20; break;
        case ENEMY_SATRAGE_CHAIN_MAIL        : battleObject->scale = toFIXED(11) / 10; break;
        case ENEMY_SATRAGE_COTTON_SHIRT      : battleObject->scale = toFIXED(11) / 10; break;
        case ENEMY_NAVAMPA_CLAYMORE          : battleObject->scale = toFIXED(6) / 5; break;
        case ENEMY_NAVAMPA_HUNTERS_SWORD     : battleObject->scale = toFIXED(6) / 5; break;
        case ENEMY_TOADONPA                  : battleObject->scale = toFIXED(3) / 2; break;
        case ENEMY_TORNADO_LIZARD            : battleObject->scale = toFIXED(6) / 5; break;
        case ENEMY_STORM_LIZARD              : battleObject->scale = toFIXED(3) / 2; break;
        case ENEMY_TEMPEST_LIZARD            : battleObject->scale = toFIXED(31) / 20; break;
        case ENEMY_RECLUSE                   : battleObject->scale = toFIXED(5) / 4; break;
        case ENEMY_CEREBUS                   : battleObject->scale = toFIXED(5) / 4; break;
        case ENEMY_SLIME_SOL_SANCTUM         :
        case ENEMY_SLIME_WORLD_MAP           :
        case ENEMY_OOZE                      :
        case ENEMY_SLIME_BEAST               :
        case ENEMY_THIEF                     :
        case ENEMY_SIREN                     :
        case ENEMY_SUCCUBUS                  :
        case ENEMY_NIGHTMARE                 :
        case ENEMY_KOBOLD                    :
        case ENEMY_MYSTERY_MAN               :
        case ENEMY_SATUROS_MERCURY_LIGHTHOUSE:
        case ENEMY_SATUROS_VENUS_LIGHTHOUSE  :
        case ENEMY_MYSTERY_WOMAN             :
        case ENEMY_MENARDI                   : battleObject->scale = toFIXED(9) / 10; break;
    }

    battleObject->actor          = actor;
    battleObject->anchorX        = fixedX;
    battleObject->anchorZ        = fixedZ;
    battleObject->palette        = palette;
    battleObject->resource       = sprite;
    weaponSprite                 = btUnitGetWeaponSprite(unitId);
    battleObject->animation      = 0;
    battleObject->animationEntry = 0;
    battleObject->shadowEntry    = 0;
    battleObject->active         = 0;
    battleObject->fading         = 0;
    battleObject->shadow         = SPRITE_BATTLE_SHADOW;
    characterId                  = unit->id;
    battleObject->overlay        = weaponSprite;

    // Isaac and Garet draw a Machete as part of their own sprite, with no weapon overlay.
    if (characterId <= GARET && gmInventoryGetEquippedItemStub(unit, EQUIP_WEAPON) == ITEM_MACHETE)
    {
        if (unit->id == ISAAC)
        {
            sprite                 = SPRITE_ISAAC_MACHETE;
            battleObject->resource = sprite;
        }
        else
        {
            sprite                 = SPRITE_GARET_MACHETE;
            battleObject->resource = sprite;
        }
        battleObject->overlay = 0;
    }

    // Face the centre of the battlefield; enemies are drawn at 1.3 times their size.
    facingAngle           = syAtan(y / 8, x) + DEGtoANG(180);
    actor->facing         = facingAngle;
    actor->collisionFlags = ACTOR_SOLID | ACTOR_PUSHABLE;
    actor->motionFlags    = ACTOR_FALLS;
    if (unit->classId == CLASS_NONE)
    {
        actor->scaleX = toFIXED(13) / 10;
        actor->scaleY = toFIXED(13) / 10;
    }
    else
    {
        actor->scaleX = toFIXED(1);
        actor->scaleY = toFIXED(1);
    }
    fdActorStartScriptStub(actor, BattleObjectEmptyScript);
}

// Unused: reads the battlefield position of a party place.
void btPlacementGetStepPair(Sint32 partyPlace, Sint32 *x, Sint32 *y)
{
    partyPlace *= 2;
    *x          = BattlePlacementSpots[partyPlace];
    *y          = BattlePlacementSpots[partyPlace + 1];
}

// BATTLE_STATE.enemyIds of an enemy that left the battle, and of a place held for one: unit ids 254
// and 255 in the list.
#define IS_PLACEHOLDER_ID(id)   ((Uint16)((id) - BATTLE_UNIT_REMOVED) <= 1)

// The enemies' column, in pixels from the battlefield's centre, and the nearer place the Kraken and
// the Fusion Dragon would take.
#define ENEMY_COLUMN_X          (-80)
#define ENEMY_COLUMN_NEAR_X     (-50)
// Pixels of the column a large enemy, a small one and an empty place take.
#define ENEMY_SPACING_LARGE     38
#define ENEMY_SPACING_SMALL     27
#define ENEMY_SPACING_EMPTY     25
// The spacing the column's length is first estimated from for up to four enemies; five or six are
// estimated at a small enemy's.
#define ESTIMATED_ENEMY_SPACING 30

// Lays out count enemies in a column left of the centre, spaced by their size, from a rough
// estimate of the column's length.
void btPlacementLayoutEnemies(Uint16 *unitIds, Sint32 count, Sint32 *xPositions, Sint32 *zPositions)
{
    Sint32 totalSpacing = count <= 4 ? ESTIMATED_ENEMY_SPACING : ENEMY_SPACING_SMALL;
    Sint32 z            = ((count - 1) * totalSpacing) / 2;
    Sint32 index;

    // z starts half a line's estimated length up and steps down by half of each enemy's spacing
    // before it (none for the first) and half after it.
    for (index = 0; index != count; index++)
    {
        Sint32 spacing    = 0;

        xPositions[index] = ENEMY_COLUMN_X;
        if (index != 0)
        {
            UNIT *unit;

            spacing = ENEMY_SPACING_EMPTY;
            if (!IS_PLACEHOLDER_ID(unitIds[index]))
            {
                unit    = gmUnitGetStub(unitIds[index]);
                spacing = btSpriteIsSmall(unit->id) ? ENEMY_SPACING_SMALL : ENEMY_SPACING_LARGE;
                // The first place keeps ENEMY_COLUMN_X whatever its size.
                if (unit->id == ENEMY_FUSION_DRAGON || unit->id == ENEMY_KRAKEN)
                {
                    xPositions[index] = ENEMY_COLUMN_NEAR_X;
                }
            }
        }
        z                 -= spacing / 2;
        zPositions[index]  = z;
        spacing            = ENEMY_SPACING_EMPTY;
        if (!IS_PLACEHOLDER_ID(unitIds[index]))
        {
            UNIT *unit = gmUnitGetStub(unitIds[index]);

            spacing    = btSpriteIsSmall(unit->id) ? ENEMY_SPACING_SMALL : ENEMY_SPACING_LARGE;
        }
        z -= spacing / 2;
    }
}

// Returns the id of the first free enemy unit, or -1 when all six are taken.
Sint32 btEnemyGetFreeUnitId(void)
{
    Sint32 enemySlot;
    Sint32 unitId;

    for (enemySlot = 0; enemySlot < ENEMY_BATTLE_SIZE; enemySlot++)
    {
        unitId = enemySlot + FIRST_ENEMY_UNIT;
        if (gmUnitGetStub(unitId)->side == BATTLE_SIDE_NONE)
            break;
    }
    if (enemySlot == ENEMY_BATTLE_SIZE)
        return -1;
    return unitId;
}

// Lays the enemies out again and moves each one's battle object anchor to its new place.
UNREAD_RESULT btPlacementRelayoutEnemies(void)
{
    BATTLE_STATE *battle = BattleStateWork;
    Uint16        unitIds[UNIT_MAX];
    Sint32        xPositions[ENEMY_BATTLE_SIZE];
    Sint32        zPositions[ENEMY_BATTLE_SIZE];
    Sint32        enemyCount;
    Sint32        enemyIndex;

    for (enemyIndex = 0;
         enemyIndex < ENEMY_BATTLE_SIZE && battle->enemyIds[enemyIndex] != BATTLE_UNIT_LIST_END;
         enemyIndex++)
        unitIds[enemyIndex] = battle->enemyIds[enemyIndex];
    enemyCount = enemyIndex;
    btPlacementLayoutEnemies(unitIds, enemyCount, xPositions, zPositions);
    for (enemyIndex = 0; enemyIndex < enemyCount; enemyIndex++)
    {
        Sint32 unitId = battle->enemyIds[enemyIndex];

        if (unitId != BATTLE_UNIT_REMOVED)
        {
            BATTLE_OBJECT *battleObject = btObjectGet(unitId);

            battleObject->anchorX       = toFIXED(xPositions[enemyIndex]);
            battleObject->anchorZ       = toFIXED(zPositions[enemyIndex]);
        }
    }
}

// Places every unit at the start of a battle and creates its battle object at its spot.
void btUnitRefreshPlacement(void)
{
    BATTLE_STATE *battle;
    Uint16        unitIds[UNIT_MAX];
    Sint32        xPositions[ENEMY_BATTLE_SIZE];
    Sint32        zPositions[ENEMY_BATTLE_SIZE];
    Sint32        count;
    Sint32        i;
    Sint32        partyPlace = 0;
    Sint32        enemyPlace = 0;
    Sint32        number;
    Sint32        enemy;
    Uint8        *place;

    battle = BattleStateWork;
    count  = btPartyPrepareMembers(unitIds);
    // objectIndex[0..7] is each party unit id's place and objectIndex[8..13] the enemy places,
    // numbered 8 to 13.
    number = BATTLE_OBJECT_NONE;
    for (i = 0; i < UNIT_MAX; i++)
        battle->objectIndex[i] = number;
    place = &battle->objectIndex[UNIT_MAX - 1];
    for (enemy = ENEMY_BATTLE_SIZE - 1, number = UNIT_MAX - 1; enemy >= 0; enemy--, number--)
        *place-- = number;

    // The party, at its fixed spots in pixels: x 80, z 48, 16, -16 and -48 (BattlePlacementSpots,
    // battle_tables.c).
    for (i = 0; i < count; partyPlace++, i++)
    {
        Sint32 unitId               = unitIds[i];

        battle->objectIndex[unitId] = partyPlace;
        btObjectMake(btObjectGet(unitId),
                     unitId,
                     BattlePlacementSpots[partyPlace * 2],
                     BattlePlacementSpots[partyPlace * 2 + 1]);
    }

    // The enemies, laid out by size.
    for (i = 0; i < ENEMY_BATTLE_SIZE && battle->enemyIds[i] != BATTLE_UNIT_LIST_END; i++)
        unitIds[i] = battle->enemyIds[i];
    count = i;
    btPlacementLayoutEnemies(unitIds, count, xPositions, zPositions);
    for (i = 0; i < count; i++, enemyPlace++)
    {
        Sint32 unitId = battle->enemyIds[i];

        if (unitId != BATTLE_UNIT_REMOVED)
            btObjectMake(
                btObjectGet(unitId), unitId, xPositions[enemyPlace], zPositions[enemyPlace]);
    }
}

// Returns 1 if the unit at place is in list.
Bool btPlacementContainsId(Sint16 *list, Sint32 place)
{
    Uint32 i;

    if (place > ALLY_MAX)
        place += FIRST_ENEMY_UNIT - PARTY_UNIT_COUNT;
    i = 0;
    do
    {
        Uint16 unitId = *list++;

        if (unitId == BATTLE_UNIT_LIST_END)
            break;
        if (unitId == place)
            return TRUE;
    } while (++i < UNIT_MAX);
    return FALSE;
}
