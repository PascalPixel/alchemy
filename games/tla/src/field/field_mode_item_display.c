// Showing a found item, and Reveal's sparkles on hidden things. Part of field_mode.c.

#include "graphics/glyph.h"
#include "field/screen_transition.h"
#include "field/ground_mask.h"

// Unused: only the stub table refers to it. Keeps the party leader off the treasure spots: a leader
// within 16 pixels of a treasure spot's actor is moved 20 pixels from it, in the direction it
// stood, and loses its move target.
void fdTreasureSpotMoveLeaderAway(void)
{
    ACTOR *leader;
    ACTOR *other;
    FIXED  dx;
    FIXED  dz;
    Sint32 i;

    leader = fdActorLookup(fdPartyGetLeaderActor());
    for (i = 0; i <= SCOOPED_ITEM_ACTOR - FIELD_TREASURE_ACTOR; i++)
    {
        other = fdActorLookup(i + FIELD_TREASURE_ACTOR);
        dx    = leader->x.fixed - other->x.fixed;
        dz    = leader->z.fixed - other->z.fixed;
        if (dx > -toFIXED(16) && dx < toFIXED(16))
            if (dz > -toFIXED(16) && dz < toFIXED(16))
            {
                leader->x.fixed = other->x.fixed;
                leader->z.fixed = other->z.fixed;
                syVectorAddPolarOffset(toFIXED(20), (Uint16)syAtan(dz, dx), &leader->x.fixed);
                leader->targetX = ACTOR_NO_TARGET;
                leader->targetY = ACTOR_NO_TARGET;
                leader->targetZ = ACTOR_NO_TARGET;
                break;
            }
    }
}

// Starts the particle script and its first animation at priority 1, if the spawn succeeded.
// Allocation failure leaves the sparkle absent.
static inline void fdActorInitSparkle(ACTOR *actor)
{
    if (actor != 0)
    {
        fdActorStartScriptStub(actor, (const Sint32 *)ItemDisplaySparkleScript);
        fdActorSelectAnimationStub(actor, 0);
        ((SPR *)actor->sprite)->image.priority = 1;
    }
}

// Spawns a sparkle within 16 pixels of source, in a random direction: the sparkling a found item's
// picture and a Reveal marker share.
#define SPAWN_SPARKLE_NEAR(source)                                                                 \
    {                                                                                              \
        POLAR_OFFSET_POS pos;                                                                      \
        ACTOR           *sparkle;                                                                  \
        Uint32           randomRadius;                                                             \
                                                                                                   \
        pos.vector.x = (source)->x.fixed;                                                          \
        pos.vector.y = (source)->y.fixed;                                                          \
        pos.vector.z = (source)->z.fixed;                                                          \
        randomRadius = syRandom();                                                                 \
        syVectorAddPolarOffset(randomRadius << 4, syRandom(), pos.coordinates);                    \
        sparkle = fdActorSpawn(SPRITE_SPARKLE, pos.vector.x, pos.vector.y, pos.vector.z);          \
        fdActorInitSparkle(sparkle);                                                               \
    }

// Each tick of a found item's picture: one time in ten, a sparkle appears within 16 pixels of it.
void fdItemDisplayEmitSparkles(ACTOR *emitter)
{
    // Once the picture's rise has all but stopped (under 1/256 pixel a tick), clearing its motion
    // flags (ACTOR_FALLS and ACTOR_FOLLOWS_TERRAIN) ends gravity, so it hangs at the top of its
    // rise.
    if (emitter->velocityY > -(toFIXED(1) / 256) && emitter->velocityY < toFIXED(1) / 256)
        emitter->motionFlags = 0;

    // Test the chance before drawing the radius and angle from the RNG.
    if ((100 * syRandom() >> 16) > 9)
        return;

    SPAWN_SPARKLE_NEAR(emitter);
}

// Returns 1 while Reveal is active, else 0.
Bool fdIsRevealActive(ACTOR *interpreter)
{
    FIELD_STATE *state = FieldStateWork;

    if (state == NULL)
        return FALSE;
    if (state->revealActive != 0)
        return TRUE;
    else
        return FALSE;
}

void   fdRevealEmitSparkles(ACTOR *src);

// Shows the picture of item 16 pixels above actor actorId, or for the leader at the centre of the
// metatile 16 pixels in front of them: it rises a little and hangs there, sparkling. While Reveal
// is active it first releases Reveal's sparkles, and then its mark on the picture's metatile.
ACTOR *fdItemDisplayShowItemInFront(Sint32 actorId, Sint32 item)
{
    FIELD_STATE   *state   = RamHeapSlots->fieldState;
    ACTOR         *picture = state->actors[actorId];
    ACTOR         *actor;
    SPR           *sprite;
    FIXED          pos[3];
    ICON_TRANSFER *transfer;
    Sint32         tile;
    Sint32         i;

    if (picture == NULL)
        return NULL;
    pos[0] = picture->x.fixed;
    pos[1] = picture->y.fixed;
    pos[2] = picture->z.fixed;
    if (actorId == fdPartyGetLeaderActor())
    {
        syVectorAddPolarOffset(toFIXED(16), picture->facing, pos);
        pos[0] = (pos[0] & FIELD_METATILE_MASK) + toFIXED(8);
        pos[2] = (pos[2] & FIELD_METATILE_MASK) + toFIXED(8);
    }
    if (state->revealActive != 0)
    {
        // Stop both the marker emitters and the sparkle actors they already made.
        actor = RamHeapSlots->actors;
        for (i = 0; i < ACTOR_COUNT; i++)
        {
            if (actor->script != NULL)
            {
                if (actor->update == fdRevealEmitSparkles)
                    fdActorReleaseStub(actor);
                if (actor->script == (Sint32 *)ItemDisplaySparkleScript)
                    fdActorReleaseStub(actor);
            }
            actor++;
        }
    }
    sySynch(3);
    picture = fdActorCreateStub(SPRITE_UNSEEN_MARKER, pos[0], pos[1] + toFIXED(16), pos[2]);
    if (picture == NULL)
        return NULL;
    fdActorStartScriptStub(picture, ItemDisplayGrowInScript);
    sprite                  = picture->sprite;
    // No shadow, outline or sprite animations: a 16-colour sprite in OBJ palette 0 whose tiles are
    // the item's icon.
    sprite->flags           = 0;
    sprite->count           = 0;
    sprite->image.fullColor = 0;
    sprite->image.palette   = 0;
    sprite->image.priority  = 1;
    picture->motionFlags    = ACTOR_FALLS;
    picture->groundHeight   = pos[1];
    picture->velocityY      = toFIXED(2);
    picture->gravity        = toFIXED(1) / 4;
    transfer                = syHeapSlotAllocate(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
    mnIconDrawItemStub(item);
    tile               = syVRAMCacheLoad(sprite->slot, ICON_TILE_BYTES, transfer->tiles);
    sprite->image.tile = tile;
    syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
    picture->update = fdItemDisplayEmitSparkles;
    if (state->revealActive != 0)
    {
        ACTOR *marker = RamHeapSlots->actors;
        Sint32 j;

        for (j = 0; j < ACTOR_COUNT; j++, marker++)
        {
            if (marker->script != NULL && marker->script == (Sint32 *)RevealMarkerScript
                && FIELD_METATILE(marker->x.fixed) == FIELD_METATILE(picture->x.fixed)
                && FIELD_METATILE(marker->z.fixed) == FIELD_METATILE(picture->z.fixed))
            {
                fdActorReleaseStub(marker);
                break;
            }
        }
    }
    return picture;
}

// Releases actor unless it is NULL.
void fdActorReleaseIfPresent(ACTOR *actor)
{
    if (actor != 0)
        fdActorReleaseStub(actor);
}

// Unused: an empty function between the item display's own, which nothing calls; The Broken Seal
// has none here.
UNUSED void fdItemDisplayUnused(void)
{
}

// Raises a found item's picture to 36 pixels above the party leader, who holds it up
// (ANIM_LEADER_ARMS_RAISED) facing the screen.
void fdItemDisplayRaiseAboveLeader(ACTOR *actor)
{
    ACTOR *leader;

    if (actor == 0)
        return;

    {
        ACTOR *found        = fdActorLookup(fdPartyGetLeaderActor());
        // Up to 2 pixels a tick, gaining 1 a tick, in free flight: no motion flags, so it neither
        // falls nor follows the ground.
        actor->acceleration = toFIXED(1);
        actor->speedLimit   = toFIXED(2);
        actor->motionFlags  = 0;
        leader              = found;
    }
    fdActorSetMoveTargetStub(
        actor, leader->x.fixed, leader->y.fixed + toFIXED(36), leader->z.fixed);
    fdActorSetSpritePriorityDirectStub(actor, 1);
    sySynch(3);
    fdActorSelectAnimationStub(leader, ANIM_LEADER_ARMS_RAISED);
    fdActorStartScriptStub(actor, ItemDisplayPulseScript);
    // FACING_SOUTH, a quarter turn, faces the screen.
    leader->facing = FACING_SOUTH;
}

// Shows a found item's picture as flags choose (ITEM_DISPLAY_FLAG), with waits of 60 and 80 ticks
// around its raising when both are set.
void fdItemDisplayShowPicture(ACTOR *actor, Sint32 flags)
{
    ACTOR *leader;

    if (actor != NULL)
    {
        leader = fdActorLookup(fdPartyGetLeaderActor());
        if (flags & ITEM_DISPLAY_SPARKLES)
        {
            fdActorSetSpriteFlagsStub(actor, 0);
            fdActorStartScriptStub(actor, ItemDisplayGrowInScript);
            ((ACTOR *)actor)->velocityY = toFIXED(2);
            ((ACTOR *)actor)->gravity   = toFIXED(1) / 4;
            ((ACTOR *)actor)->update    = fdItemDisplayEmitSparkles;
        }
        if (flags == (ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED))
            sySynch(60);
        if (flags & ITEM_DISPLAY_RAISED)
            fdItemDisplayRaiseAboveLeader(actor);
        if (flags == (ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED))
            sySynch(80);
        fdActorSelectAnimationStub(leader, ANIM_STAND);
    }
}

// Shows the picture of item 36 pixels above the party leader for 80 ticks, as flags choose
// (ITEM_DISPLAY_FLAG).
void fdItemDisplayShowItem(Sint32 item, Sint32 flags)
{
    ACTOR         *leader   = fdActorLookup(fdPartyGetLeaderActor());
    ICON_TRANSFER *transfer = syHeapSlotAllocate(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
    ACTOR         *actor    = fdActorCreateStub(
        SPRITE_UNSEEN_MARKER, leader->x.fixed, leader->y.fixed + toFIXED(36), leader->z.fixed);

    if (actor != 0)
    {
        SPR *sprite             = actor->sprite;

        sprite->flags           = 0;
        sprite->count           = 0;
        sprite->image.fullColor = 0;
        sprite->image.palette   = 0;
        sprite->image.priority  = 1;

        mnIconDrawItemStub(item);
        syVRAMCacheLoad(sprite->slot, ICON_TILE_BYTES, transfer->tiles);
        syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);

        if (flags & ITEM_DISPLAY_SPARKLES)
            actor->update = fdItemDisplayEmitSparkles;
        if (flags & ITEM_DISPLAY_RAISED)
            fdItemDisplayRaiseAboveLeader(actor);

        sySynch(80);
        fdActorSelectAnimationStub(leader, ANIM_STAND);
        fdActorReleaseStub(actor);
    }
}

// Each tick of a Reveal marker: one time in ten, a sparkle appears within 16 pixels of it, as
// around a found item's picture (SPAWN_SPARKLE_NEAR).
void fdRevealEmitSparkles(ACTOR *actor)
{
    if ((100 * syRandom() >> 16) > 9)
        return;

    SPAWN_SPARKLE_NEAR(actor);
}

// Places Reveal's markers from the scene's item rows (a type, a script id, a flag and a pad): for
// each of the treasure spots' rows whose flag is clear and whose script has not run, a sparkle
// marker goes at its actor, or at the map metatile of a tile script, when that point is on screen.
void fdEffectSpawnRevealMarkers(void)
{
    Sint16    *entry   = fdSceneItemGetRows();
    MAP_STATE *map     = RamHeapSlots->mapGrid;
    Sint32     originX = syMapStateView(map)[0] & FIELD_PIXEL_MASK;
    Sint32     originZ = syMapStateView(map)[1] & FIELD_PIXEL_MASK;

    // A word of the map header, then the point fdSceneItemFindPos fills for each row.
    struct
    {
        Sint32 headerWord;
        Sint32 pos[3];
    } found;

    ACTOR *actor;
    Sint16 type;
    Sint32 id;
    Sint32 flag;
    Sint32 i;
    FIXED  x;
    Sint32 y;
    FIXED  z;
    Sint32 dx;
    Sint32 dz;

    // DEAD: the followed point's height is never read; the ROM keeps its load and store.
    found.headerWord = map->origin[1];
    if (entry == NULL)
        return;
    for (i = 0; i < TREASURE_SPOT_MAX && (type = *entry++) != 0 && !(type & SCENE_ITEMS_HEADER);
         i++)
    {
        id     = entry[0];
        flag   = entry[1];
        entry += 3;
        if (type != CONTAINER_CHEST && flag != -1 && syGameFlagGet(flag))
            continue;
        if (fdSceneItemFindPos(id, found.pos))
            continue;
        actor = fdTreasureSpotGetActor(i + FIELD_TREASURE_ACTOR);
        if (actor == NULL)
            continue;
        // A tile script's actor holds its map metatile, marked by this value.
        if (actor->sceneWord == TREASURE_SPOT_ON_METATILE)
        {
            x = toFIXED(actor->timer);
            z = toFIXED(actor->userValue);
            y = fdMapGridGetTerrainHeightStub(0, x, z);
        }
        else
        {
            if ((Uint16)(type - CONTAINER_CHEST) <= CONTAINER_MIMIC - CONTAINER_CHEST)
                continue;
            if (type == CONTAINER_PSYNERGY_STONE || type == CONTAINER_STONE_TABLET
                || type == CONTAINER_POND || type == CONTAINER_GROUND)
                continue;
            x = actor->x.fixed;
            y = actor->y.fixed;
            z = actor->z.fixed;
        }
        dx   = x - originX;
        dz   = z - originZ - y;
        dx >>= 16;
        dz >>= 16;
        // On the 240 x 160 screen, give or take 15 pixels at the sides, 47 above and 31 below.
        if ((Uint32)(dx + 15) > 270 || (Uint32)(dz + 47) > 238)
            continue;
        if (flag == -1 || syGameFlagGet(flag))
            continue;
        actor = fdActorCreateStub(SPRITE_SPARKLE, x, y, z);
        if (actor != NULL)
        {
            actor->motionFlags = 0;
            fdActorStartScriptStub((ACTOR *)actor, RevealMarkerScript);
            fdActorSetSpriteFlagsStub((ACTOR *)actor, 0);
            fdActorSelectAnimationStub(actor, ANIM_STAND);
            ((SPR *)actor->sprite)->image.priority = 1;
        }
    }
}
