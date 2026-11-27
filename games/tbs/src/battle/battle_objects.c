// Each tick's sprite priorities, the status icons, the poses and the sprites of the battle objects.
// Part of battle.c.

#include "battle_internal.h"

// The order a battle unit's sprite animations are drawn into its picture (SPR_ANIMATION.priority),
// the lowest first: the shadow under the body and its weapon, the status icon over them. Any order
// above 3 is not drawn.
#define ANIMATION_PRIORITY_SHADOW      0
#define ANIMATION_PRIORITY_BODY        1
#define ANIMATION_PRIORITY_STATUS_ICON 3
#define ANIMATION_PRIORITY_HIDDEN      9

// Runs each tick of a battle: advances the status icons, hides an airborne unit's shadow, and draws
// whichever side is nearer the camera in front.
void btObjectUpdateSpritePriorities(void)
{
    Uint16 unitIds[UNIT_MAX];
    // The party's, then the enemies': sprite priority 1 for the side in front, 2 behind (the lower
    // number draws over the higher).
    Sint32 sidePriorities[2];
    Sint32 unitIndex;
    Sint32 spriteIndex;
    Sint32 count;
    ACTOR *actor;
    SPR   *sprite;
    SPR  **sprites;

    // Every unit's status icon.
    btUnitListIds(BATTLE_SIDE_MASK_BOTH, unitIds);
    unitIndex = 0;
    if (unitIds[unitIndex] != BATTLE_UNIT_LIST_END)
    {
        for (;;)
        {
            BATTLE_OBJECT *slot = btObjectGet(unitIds[unitIndex]);

            if (slot != 0)
            {
                actor = slot->actor;

                btStatusIconCycle(slot);
                if (slot->shadowEntry != 0)
                {
                    sprite = btActorGetSprite(actor, 0);

                    if (sprite != 0)
                    {
                        SPR_ANIMATION *shadow;
                        Sint32         shadowPriority = ANIMATION_PRIORITY_SHADOW;

                        // Off the ground the shadow would rise with the picture.
                        if (actor->y.fixed != 0)
                            shadowPriority = ANIMATION_PRIORITY_HIDDEN;
                        shadow = slot->shadowEntry;
                        if (shadow->priority != shadowPriority)
                        {
                            shadow->priority = shadowPriority;
                            sprite->dirty    = 1;
                        }
                    }
                }
            }
            unitIndex++;
            if (unitIndex >= UNIT_MAX || unitIds[unitIndex] == BATTLE_UNIT_LIST_END)
                break;
        }
    }

    // The camera's turn decides which side is in front: with the yaw in the first half turn, not
    // negative, the party.
    if (BattleCameraWork->yaw >= 0)
    {
        sidePriorities[0] = 1;
        sidePriorities[1] = 2;
    }
    else
    {
        sidePriorities[0] = 2;
        sidePriorities[1] = 1;
    }

    // The party.
    {
        count = btUnitListIds(BATTLE_SIDE_MASK_PARTY, unitIds);
        for (unitIndex = 0; unitIndex < count; unitIndex++)
        {
            BATTLE_OBJECT *slot = btObjectGet(unitIds[unitIndex]);

            if (slot != 0)
            {
                actor = slot->actor;

                switch (actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK)
                {
                    case ACTOR_SPRITE_ONE:
                    {
                        sprite                 = actor->sprite;
                        sprite->image.priority = sidePriorities[0];
                        break;
                    }
                    case ACTOR_SPRITE_GROUP:
                    {
                        sprites = (SPR **)actor->sprite;
                        for (spriteIndex = 0; spriteIndex < ACTOR_SPRITE_GROUP_SIZE; spriteIndex++)
                        {
                            sprite = *sprites++;
                            if (sprite != 0)
                                sprite->image.priority = sidePriorities[0];
                        }
                        break;
                    }
                }
            }
        }
    }

    // The enemies.
    {
        count = btUnitListIds(BATTLE_SIDE_MASK_ENEMIES, unitIds);
        for (unitIndex = 0; unitIndex < count; unitIndex++)
        {
            BATTLE_OBJECT *slot = btObjectGet(unitIds[unitIndex]);

            if (slot != 0)
            {
                actor = slot->actor;

                switch (actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK)
                {
                    case ACTOR_SPRITE_ONE:
                    {
                        sprite                 = actor->sprite;
                        sprite->image.priority = sidePriorities[1];
                        break;
                    }
                    case ACTOR_SPRITE_GROUP:
                    {
                        sprites = (SPR **)actor->sprite;
                        for (spriteIndex = 0; spriteIndex < ACTOR_SPRITE_GROUP_SIZE; spriteIndex++)
                        {
                            sprite = *sprites++;
                            if (sprite != 0)
                                sprite->image.priority = sidePriorities[1];
                        }
                        break;
                    }
                }
            }
        }
    }
}

// The status icons' bits in BATTLE_OBJECT.activeStatuses.
#define ICON_POISON            0x01
#define ICON_VENOM             0x02 // deadly poison
#define ICON_STUN              0x04
#define ICON_PSY_SEAL          0x08
#define ICON_SLEEP             0x10
#define ICON_DELUSION          0x20
#define ICON_HAUNT             0x40
// The Grim Reaper's countdown, 1 to 7, as one of bits 7 to 13.
#define ICON_GRIM_REAPER_SHIFT 6

#define ICON_MAX               14 // the kinds of icon

// Builds the status icon bits of unit id's statuses into its battle object's activeStatuses, which
// btStatusIconCycle shows.
UNREAD_RESULT btUnitBuildStatusFlags(Sint32 unitId, BATTLE_OBJECT *battleObject)
{
    UNIT  *unit   = gmUnitGetStub(unitId);
    Sint8  poison = unit->poison;
    Uint32 flags  = 0;

    if (poison == POISON_NORMAL)
        flags = ICON_POISON;
    // Deadly poison: poison is ICON_VENOM.
    if (poison == POISON_VENOM)
        flags |= poison;
    if (unit->delusion != 0)
        flags |= ICON_DELUSION;
    if (unit->stun != 0)
    {
        Sint32 enemyId;
        flags   |= ICON_STUN;
        enemyId  = unit->id;
        // The two-part bosses show no stun icon.
        if (enemyId == ENEMY_KRAKEN || enemyId == ENEMY_FUSION_DRAGON)
            flags &= ~ICON_STUN;
    }
    if (unit->psySeal != 0)
        flags |= ICON_PSY_SEAL;
    if (unit->haunt != 0)
        flags |= ICON_HAUNT;
    if (unit->sleep != 0)
        flags |= ICON_SLEEP;
    if (unit->grimReaper != 0)
        flags |= 1 << (unit->grimReaper + ICON_GRIM_REAPER_SHIFT);
    battleObject->activeStatuses = flags;
}

// The ticks each status icon shows before the next one's turn.
#define ICON_CYCLE_TICKS      80

// The icons' first animations, for units 32 pixels wide and for the rest: each adds the icon's bit
// number (ICON_POISON's 0 up).
#define ICON_ANIMATION_NARROW SPRITE_STATUS_POISON
#define ICON_ANIMATION_WIDE   SPRITE_STATUS_WIDE_POISON

// Shows a unit's status icons one after another, and removes the icon when no status is left.
UNREAD_RESULT btStatusIconCycle(BATTLE_OBJECT *battleObject)
{
    SPR_ANIMATION *shownIcon;
    SPR_ANIMATION *icon;
    SPR           *sprite;
    ACTOR         *actor;
    Sint32         iconAnimation;
    Sint32         iconHandle;
    Sint32         prevCondition;
    Sint32         changed = 0;

    if (battleObject->cycleTimer >= 0)
        battleObject->cycleTimer--;

    // Move on when the time is up, when the shown status has ended, or when a status appears with
    // no icon shown.
    shownIcon = battleObject->animationEntry;
    if ((shownIcon == 0 ? battleObject->activeStatuses != 0
                        : ((battleObject->activeStatuses >> battleObject->shownStatus) & 1) == 0)
        || battleObject->cycleTimer == 0)
    {
        iconAnimation = -1;
        actor         = battleObject->actor;
        if (battleObject->activeStatuses != 0)
        {
            // The next status after the shown one, wrapping around.
            prevCondition = battleObject->shownStatus;
            for (iconAnimation = prevCondition + 1;; iconAnimation++)
            {
                if (iconAnimation > ICON_MAX - 1)
                    iconAnimation = 0;
                if (((battleObject->activeStatuses >> iconAnimation) & 1) != 0)
                    break;
            }

            if (prevCondition != iconAnimation || shownIcon == 0)
            {
                battleObject->shownStatus = iconAnimation;
                changed                   = 1;
            }
            battleObject->cycleTimer = ICON_CYCLE_TICKS;
        }
        else
        {
            changed = 1;
        }

        // Swap the icon on the unit's first sprite; iconAnimation is -1 when no status is left.
        sprite = btActorGetSprite(actor, 0);
        if (sprite != 0)
        {
            // Units 32 pixels wide take their own set of icon animations.
            if (iconAnimation >= 0)
            {
                if (sprite->width == 32)
                    iconAnimation += ICON_ANIMATION_NARROW;
                else
                    iconAnimation += ICON_ANIMATION_WIDE;
            }

            if (battleObject->animationEntry != 0 && changed != 0)
            {
                fdSpriteRemoveAnimationStub(sprite, (Sint32)battleObject->animationEntry);
                battleObject->animationEntry = 0;
            }

            if (iconAnimation >= 0 && changed != 0)
            {
                // The handle is stored first and cleared again when registration fails.
                iconHandle                   = fdSpriteAddAnimationStub(sprite, iconAnimation);
                battleObject->animationEntry = (SPR_ANIMATION *)iconHandle;
                if (iconHandle == -1)
                    battleObject->animationEntry = 0;
                icon = battleObject->animationEntry;
                if (icon != 0)
                {
                    icon->priority = ANIMATION_PRIORITY_STATUS_ICON;
                    fdSpriteAnimationStartStub(icon, 0);
                }
            }

            sprite->dirty = 1;
            if (iconAnimation >= 0)
                battleObject->animation = iconAnimation;
            else
                battleObject->animation = 0;
        }
    }
}

// Sets a unit's pose to match its state, held, down or standing, and sets its sway a little out of
// step with its neighbours'.
UNREAD_RESULT btObjectUpdatePose(Sint32 unitId)
{
    UNIT  *unit;
    Sint32 pose;

    unit = gmUnitGetStub(unitId);
    pose = POSE_STANDING;
    if (unit->hp != 0)
    {
        if (unit->sleep != 0 || unit->stun != 0 || unit->immobile != 0)
            pose = unit->side == BATTLE_SIDE_ENEMY ? POSE_STILL : POSE_PARTY_HELD;
    }
    else if (unit->side == BATTLE_SIDE_ENEMY)
    {
        pose = POSE_ENEMY_DOWN;
    }
    else
    {
        pose = POSE_PARTY_HELD;
    }

    fdActorSelectAnimationStub(btObjectGet(unitId)->actor, pose);
    fdActorSetAnimationSpeedStub(btObjectGet(unitId)->actor, ANIM_STEP_NORMAL - 2 + (unitId & 3));
}

// Releases every sprite unitId's actor holds and leaves it with none, clearing the battle object's
// animationEntry and shadowEntry.
void btObjectDestroySprites(Sint32 unitId)
{
    BATTLE_OBJECT *battleObject;
    ACTOR         *actor;
    Sint32         spriteIndex;
    SPR           *sprite;

    battleObject = btObjectGet(unitId);
    if (battleObject != NULL)
    {
        actor = battleObject->actor;
        if (actor != NULL)
        {
            battleObject->animationEntry = 0;
            battleObject->shadowEntry    = 0;
            for (spriteIndex = 0; (sprite = btActorGetSprite(actor, spriteIndex)) != NULL;
                 spriteIndex++)
            {
                fdSpriteDestroyStub(sprite);
            }
            actor->spriteStorageKind = ACTOR_SPRITE_NONE;
            actor->sprite            = NULL;
        }
    }
}

// Readies the sprites of the units in the list and releases everyone else's; with refreshPoses,
// sets every listed unit's pose again.
void btObjectSpawnObjectsForList(Sint16 *unitIds, Sint32 refreshPoses)
{
    Sint32         unitIndex;
    Sint32         unitId;
    BATTLE_OBJECT *battleObject;
    BATTLE_ACTOR  *actor;
    SPR           *sprite;
    SPR_ANIMATION *entry;
    SPR          **sprites;
    Sint32         spriteId;
    Sint32         releasedUnitId;

    // Release the sprites of the places no listed unit holds.
    for (unitIndex = 0; unitIndex < UNIT_MAX; unitIndex++)
    {
        if (!btPlacementContainsId(unitIds, unitIndex))
        {
            releasedUnitId = unitIndex + FIRST_ENEMY_UNIT - PARTY_UNIT_COUNT;
            if (unitIndex <= ALLY_MAX)
                releasedUnitId = unitIndex;
            btObjectDestroySprites(releasedUnitId);
        }
    }
    // A tick passes between releasing and creating sprites, except inside a event, which cannot
    // wait.
    if (EventsRunning == 0)
        sySynch(1);

    for (unitIndex = 0;
         unitIndex < UNIT_MAX && (unitId = unitIds[unitIndex]) != BATTLE_UNIT_LIST_END;
         unitIndex++)
    {
        if (unitId == BATTLE_UNIT_REMOVED)
            continue;
        battleObject = btObjectGet(unitId);
        if (battleObject == 0)
            continue;
        btUnitBuildStatusFlags(unitId, battleObject);
        actor = (BATTLE_ACTOR *)battleObject->actor;
        if (actor == 0)
            continue;
        if (actor->spriteStorageKind != ACTOR_SPRITE_NONE)
            continue;
        if ((battleObject->resource & SPRITE_ID_MASK) == SPRITE_BATTLE_KRAKEN
            || (battleObject->resource & SPRITE_ID_MASK) == SPRITE_BATTLE_FUSION_DRAGON)
        {
            // A two-part enemy: its two sprites in the actor system's next group table. BUG: a
            // failed create leaves sprite null, and its flags are still written.
            sprites  = ActorSystemStateWork->spriteGroups + ActorSystemStateWork->count;
            spriteId = battleObject->resource;
            actor->spriteStorageKind = ACTOR_SPRITE_GROUP;
            actor->sprite            = sprites;
            syWordsClear(sprites, 16);
            sprite = fdSpriteCreateStub(spriteId);
            if (sprite != 0)
            {
                sprite->scale = syMulFX(sprite->scale, battleObject->scale);
                actor->radius = syResourceGetMetadataRecordStub(spriteId)->footprint >> 1;
                *sprites      = sprite;
                sprites++;
            }
            sprite->flags = 0;
            sprite        = fdSpriteCreateStub(spriteId + SPRITE_SECOND_PART);
            if (sprite != 0)
            {
                sprite->scale = syMulFX(sprite->scale, battleObject->scale);
                *sprites      = sprite;
            }
            sprite->flags = 0;
        }
        else
        {
            // The body, then the weapon, the status icon and the shadow.
            sprite = fdSpriteCreateStub(battleObject->resource);
            if (sprite != 0)
            {
                actor->spriteStorageKind = ACTOR_SPRITE_ONE;
                actor->sprite            = sprite;
                sprite->scale            = syMulFX(sprite->scale, battleObject->scale);
                entry                    = sprite->entries[0];
                entry->priority          = ANIMATION_PRIORITY_BODY;
                entry->colorMode         = battleObject->palette;
                if ((spriteId = battleObject->overlay) != 0)
                {
                    entry           = (SPR_ANIMATION *)fdSpriteAddAnimationStub(sprite, spriteId);
                    entry->priority = ANIMATION_PRIORITY_BODY;
                }
                if ((spriteId = battleObject->animation) != 0)
                {
                    entry = (SPR_ANIMATION *)fdSpriteAddAnimationStub(sprite, spriteId);
                    battleObject->animationEntry = entry;
                    fdSpriteAnimationStartStub(entry, 0);
                    entry->priority = ANIMATION_PRIORITY_STATUS_ICON;
                }
                if ((spriteId = battleObject->shadow) != 0)
                {
                    if (sprite->width == 32)
                        spriteId = SPRITE_BATTLE_SHADOW_NARROW;
                    entry = (SPR_ANIMATION *)fdSpriteAddAnimationStub(sprite, spriteId);
                    battleObject->shadowEntry = entry;
                    entry->priority           = ANIMATION_PRIORITY_SHADOW;
                    sprite->flags             = 0;
                }
            }
        }
        btObjectUpdatePose(unitId);
    }

    if (refreshPoses)
    {
        unitIndex = 0;
        if ((spriteId = unitIds[0]) != BATTLE_UNIT_LIST_END)
        {
            for (;;)
            {
                // DEAD: actor = battleObject->actor in the test, never read after it; the ROM keeps
                // the store.
                if (unitIds[unitIndex] != BATTLE_UNIT_REMOVED
                    && (battleObject = btObjectGet(spriteId)) != 0
                    && (actor = (BATTLE_ACTOR *)battleObject->actor) != 0)
                    btObjectUpdatePose(spriteId);
                unitIndex++;
                if (unitIndex >= UNIT_MAX
                    || (spriteId = unitIds[unitIndex]) == BATTLE_UNIT_LIST_END)
                    break;
            }
        }
    }
}

// Returns unitId's battle object, through its place, or null for a unit with no place.
BATTLE_OBJECT *btObjectGet(Sint32 unitId)
{
    BATTLE_STATE  *battle  = BattleStateWork;
    BATTLE_OBJECT *objects = battle->objects;
    Sint32         place   = unitId;

    // Enemy unit ids from FIRST_ENEMY_UNIT index the places from PARTY_UNIT_COUNT.
    if (unitId > ALLY_MAX)
        place -= FIRST_ENEMY_UNIT - PARTY_UNIT_COUNT;
    if (battle->objectIndex[place] == BATTLE_OBJECT_NONE)
        return NULL;
    return &objects[battle->objectIndex[place]];
}

// Detaches the scripts from the four sprite entries of one sprite.
void btSpriteDetachScripts(void *untypedSprite)
{
    SPR   *sprite = untypedSprite;
    Sint32 entryIndex;

    if (sprite != NULL)
    {
        for (entryIndex = 0; entryIndex < 4; entryIndex++)
        {
            if (sprite->entries[entryIndex] != NULL)
                sprite->entries[entryIndex]->script = NULL;
        }
    }
}

// Unused: detaches the scripts from every sprite of a unit's actor.
void btActorDetachSpriteScripts(ACTOR *actor)
{
    void **groups;
    Sint32 i;

    if (actor == NULL)
        return;
    switch (actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK)
    {
        case ACTOR_SPRITE_ONE: btSpriteDetachScripts(actor->sprite); break;
        case ACTOR_SPRITE_GROUP:
            groups = (void **)actor->sprite;
            for (i = 0; i < ACTOR_SPRITE_GROUP_SIZE; i++)
            {
                if (groups[i] != NULL)
                    btSpriteDetachScripts(groups[i]);
            }
            break;
    }
}

// Frees the enemy sprite slots a unit holds and marks its actor for release at the end of the
// action. Returns 0.
Sint32 btObjectMarkForRelease(Sint32 unitId)
{
    BATTLE_OBJECT *battleObject;
    btObjectReleaseSpriteSlots(unitId);
    battleObject         = btObjectGet(unitId);
    battleObject->active = 1;
    return 0;
}

// Releases the actor of every unit marked by btObjectMarkForRelease, leaving its battle object with
// no actor.
#if FIX_FUNCTION_SIGNATURES
void btObjectDestroyAll(void)
{
#else
void *btObjectDestroyAll(void)
{
#endif
    Sint32 place;

    for (place = 0; place < UNIT_MAX; place++)
    {
        Sint32         unitId = place + FIRST_ENEMY_UNIT - PARTY_UNIT_COUNT;
        BATTLE_OBJECT *battleObject;

        if (place <= ALLY_MAX)
            unitId = place;
        battleObject = btObjectGet(unitId);

        if (battleObject != NULL && battleObject->active != 0)
        {
            fdActorReleaseStub(battleObject->actor);
            battleObject->actor  = NULL;
            battleObject->active = 0;
        }
    }
}

// Unused: writes the battlefield spot of place (its anchor, on the ground) to position, and returns
// 0.
Sint32 btObjectGetAnchor(Sint32 place, POS *pos)
{
    BATTLE_STATE *battle = BattleStateWork;

    SET_POSITION(pos, battle->objects[place].anchorX, 0, battle->objects[place].anchorZ);
    return 0;
}

// Sets up the camera's view for projecting points, tilted further while commands are chosen.
UNREAD_RESULT btCameraApplyTransform(void)
{
    BATTLE_CAMERA *camera = BattleCameraWork;
    syInitMatrix();
    if (gmFlagGetStub(FLAG_CHOOSING_COMMANDS) != 0)
    {
        syMultiMatrix(BattleCameraCommandMatrix);
        syLookAt(camera->eye, camera->focus);
    }
    else
    {
        syLookAtLoad(camera->eye, camera->focus);
    }
}

// Projects unit id's position onto the screen, into projected (its pixel x and y, then its depth;
// syConvert3Dto2D). Returns 0.
Sint32 btMotionProjectPos(Sint32 unitId, Sint32 *projected)
{
    ACTOR *actor  = btObjectGet(unitId)->actor;
    SPR   *sprite = btActorGetSprite(actor, 0);
    Sint32 pos[3];
    Sint32 projectionScale;

    btCameraApplyTransform();
    SET_VECTOR(pos, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    projectionScale = syConvert3Dto2D(pos, projected);
    // The sprite's projected scale is worked out and thrown away; compare btMotionProjectScaledPos,
    // which keeps it.
    (void)syMulFX(projectionScale, sprite->scale);
    return 0;
}

// Returns sprite spriteIndex of an actor (0 only, for a single sprite), or null when it has no such
// sprite.
void *btActorGetSprite(ACTOR *actor, Sint32 spriteIndex)
{
    Sint32 storageKind = actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK;
    if (storageKind == ACTOR_SPRITE_ONE)
    {
        if (spriteIndex == 0)
        {
            return actor->sprite;
        }
    }
    else if (storageKind == ACTOR_SPRITE_GROUP)
    {
        return ((void **)actor->sprite)[spriteIndex];
    }
    return NULL;
}

// Puts the battle camera at its default view of the battlefield's centre.
void btCameraSetDefaultView(void)
{
    BATTLE_CAMERA *camera = BattleCameraWork;
    POINT          eyeOffset;

    // A pitch of -1/128 of a turn, 510 pixels away, the distance btCameraSetView uses.
    camera->yaw      = TRIG_TURN * 3 / 16;
    camera->pitch    = TRIG_TURN - TRIG_TURN / 128;
    camera->distance = BATTLE_CAMERA_DISTANCE;
    SET_VECTOR(camera->focus, 0, 0, 0);
    camera->followPos = 0;
    camera->followEye = 0;

    syInitMatrix();
    syMatrixTranslateVector(&camera->focus[0]);
    syRotY(camera->yaw);
    syRotX(camera->pitch);

    eyeOffset[X] = 0;
    eyeOffset[Y] = 0;
    eyeOffset[Z] = camera->distance;
    syCalcPoint(eyeOffset, (Sint32 *)camera);
}
