// Running a field Psynergy by its kind, and helpers its kinds share. Part of script_commands.c.

#include "script_commands_internal.h"
#include "scene/actor_polar_target.h"

// Runs one step of the current field Psynergy: an FIELD_PSYNERGY_HANDLER.
void fdPsynergyRunHandler(Sint32 kind)
{
    if (kind == FIELD_PSYNERGY_HANDLER_UNTARGETED)
        fdPsynergyRun();
    else if (kind == FIELD_PSYNERGY_HANDLER_ON_TARGET)
        fdPsynergyDispatch();
    else if (kind == FIELD_PSYNERGY_HANDLER_FINISH)
        fdPsynergyFinishMindRead();
    else if (kind == FIELD_PSYNERGY_HANDLER_EXTRA)
        fdPsynergyRunExtraHandler();
}

// Casts the current field Psynergy with no target: runs the body of its kind
// (effectState->fieldPsynergy).
void fdPsynergyRun(void)
{
    FIELD_EFFECT_STATE *effectState;
    FIELD_STATE        *fieldState;
    Sint32              kind;
    Sint32              targetId;
    Sint32              nearestTargetId;

    effectState = FieldEffectStateWork;
    fieldState  = FieldStateWork;
    kind        = effectState->fieldPsynergy;
    targetId    = effectState->targetId;

    switch (kind)
    {
        case FIELD_PSYNERGY_MOVE     : fdPsynergyMove(); return;
        case FIELD_PSYNERGY_LIFT     : fdPsynergyLift(); return;
        case FIELD_PSYNERGY_CARRY    : fdPsynergyCarry(); return;
        case FIELD_PSYNERGY_FORCE    : fdPsynergyForce(); return;
        case FIELD_PSYNERGY_DOUSE    : fdPsynergyDouse(); return;
        case FIELD_PSYNERGY_WHIRLWIND: fdPsynergyWhirlwind(); return;
        case FIELD_PSYNERGY_FROST    : fdPsynergyFrost(); return;
        case FIELD_PSYNERGY_PLY      : fdPsynergyPly(); return;
        case FIELD_PSYNERGY_GROWTH   : fdPsynergyGrowth(); return;
        case FIELD_PSYNERGY_CATCH    : fdPsynergyCatch(); return;
        case FIELD_PSYNERGY_HALT:
            // Resume the actor stopped before, then stop the nearest one.
            if (MainRoutineStateWork.haltedActor != -1)
            {
                fdPsynergyResumeHaltedActor(MainRoutineStateWork.haltedActor);
                MainRoutineStateWork.haltedActor = -1;
            }

            nearestTargetId = fdFindNearbyPsynergyTarget(MainRoutineStateWork.leaderActorId, kind);
            nearestTargetId = fdPsynergyFilterHaltTarget(nearestTargetId);
            if (fdFindActorTalkCue(nearestTargetId) != 0)
            {
                fdPsynergySetupActorPair(MainRoutineStateWork.leaderActorId, nearestTargetId);
                fdPsynergyHaltOnTarget(nearestTargetId);
                fdPsynergyHaltActor(nearestTargetId);
                MainRoutineStateWork.haltedActor = nearestTargetId;
            }
            else
            {
                fdPsynergyHalt();
            }
            return;
        case FIELD_PSYNERGY_MIND_READ:
            // The wave and colour effect, stopping a running Reveal first.
            if (fieldState->revealActive != 0)
                fdPsynergyEndReveal();
            fdPsynergyStartMindReadWave(effectState->casterId, targetId);
            return;
        case FIELD_PSYNERGY_REVEAL : fdPsynergyReveal(); return;
        case FIELD_PSYNERGY_CLOAK  : fdPsynergyStartCloak(); return;
        case FIELD_PSYNERGY_RETREAT: fdPsynergyRetreat(); return;
        case FIELD_PSYNERGY_AVOID  : fdPsynergyAvoid(); return;
    }
}

// Casts the current field Psynergy on its target (secondActorId): runs the handler of its kind.
void fdPsynergyDispatch(void)
{
    FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
    FIELD_STATE        *fieldState  = FieldStateWork;
    Sint32              kind        = effectState->fieldPsynergy;
    Sint32              targetId    = effectState->targetId;

    effectState->aimAhead           = 0;
    switch (kind)
    {
        case FIELD_PSYNERGY_MIND_READ:
            if (fieldState->revealActive != 0)
                fdPsynergyEndReveal();
            if (MainRoutineStateWork.haltedActor != effectState->targetId)
                effectState->target->paused = 1;
            fdPsynergyStartMindReadWave(effectState->casterId, targetId);
            break;
        case FIELD_PSYNERGY_MOVE:
            // Push the target with the direction keys.
            fdPsynergySteerPushedTarget(targetId);
            break;
        case FIELD_PSYNERGY_LIFT  : fdPsynergyLiftOnTarget(targetId); break;
        case FIELD_PSYNERGY_CARRY : fdPsynergySteerLiftedTarget(targetId); break;
        case FIELD_PSYNERGY_FORCE : fdPsynergyForceOnTarget(targetId); break;
        case FIELD_PSYNERGY_DOUSE : fdPsynergyDouseOnTarget(targetId); break;
        case FIELD_PSYNERGY_FROST : fdPsynergyFrostOnTarget(targetId); break;
        case FIELD_PSYNERGY_GROWTH: fdPsynergyGrowthOnTarget(targetId); break;
        case FIELD_PSYNERGY_HALT:
            if (MainRoutineStateWork.haltedActor != -1)
            {
                fdPsynergyResumeHaltedActor(MainRoutineStateWork.haltedActor);
                MainRoutineStateWork.haltedActor = -1;
            }
            fdPsynergyHaltActor(targetId);
            MainRoutineStateWork.haltedActor = targetId;
            fdPsynergyHaltOnTarget(targetId);
            break;
        case FIELD_PSYNERGY_PLY      : fdPsynergyPlyOnTarget(targetId); break;
        case FIELD_PSYNERGY_WHIRLWIND: fdPsynergyWhirlwindOnTarget(targetId); break;
        case FIELD_PSYNERGY_CATCH    : fdPsynergyCatchOnTarget(targetId); break;
        case FIELD_PSYNERGY_REVEAL   : fdPsynergyReveal(); break;
        case FIELD_PSYNERGY_CLOAK    : fdPsynergyStartCloak(); break;
        case FIELD_PSYNERGY_RETREAT  : fdPsynergyRetreat(); break;
        case FIELD_PSYNERGY_AVOID    : fdPsynergyAvoid(); break;
    }
}

// Finishes the current field Psynergy when it is Mind Read: ends it and lets its target move again,
// unless the target is the actor kept stopped.
void fdPsynergyFinishMindRead(void)
{
    FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;

    if (effectState->fieldPsynergy == FIELD_PSYNERGY_MIND_READ)
    {
        fdPsynergyStopMindReadWave();
        if (MainRoutineStateWork.haltedActor != effectState->targetId)
        {
            effectState->target->paused = 0;
        }
    }
}

// The extra handler of Reveal, Cloak and Avoid: what each does when it ends.
void fdPsynergyRunExtraHandler(void)
{
    Uint32 kind;

    kind = ((FIELD_EFFECT_STATE *)FieldEffectStateWork)->fieldPsynergy;
    switch (kind)
    {
        case FIELD_PSYNERGY_REVEAL: fdPsynergyEndReveal(); return;
        case FIELD_PSYNERGY_CLOAK : fdPsynergyEndCloak(); return;
        case FIELD_PSYNERGY_AVOID : fdPsynergyShowAvoidWoreOff(); return;
    }
}

// Runs the script tied to a field Psynergy: a result below CUE_RESULT_MESSAGE_END is a message,
// shown at target; a larger one is a function, called with param and target.
Sint32 fdRunPsynergyScript(const void *event, Sint32 param, Sint32 target)
{
    Sint32 script;

    if (event != NULL)
    {
        // Every function's address is above CUE_RESULT_MESSAGE_END, so one word holds either.
        script = ((const CUE *)event)->value;
        if (script != 0)
        {
            if (script < CUE_RESULT_MESSAGE_END)
            {
                fdScriptStart();
                fdScriptSetMessageId(((const CUE *)event)->value);
                fdScriptShowMessageAndWait(target, 0);
                fdScriptEnd();
            }
            else
            {
                ((void (*)(Sint32, Sint32))script)(param, target);
            }
        }
        if (gmFlagGetStub(FLAG_NOTHING_HAPPENS) != 0)
        {
            fdScriptStart();
            mnFieldMessageShowStub(MsgNothingHappens, FIELD_MESSAGE_ZOOM);
            fdScriptEnd();
        }
    }
    return 0;
}

// Per-tick update: cycles an actor's colours through its own and the five with channels swapped, a
// new colour mode every tick.
void fdActorCycleSpriteColors(ACTOR *actor)
{
    SPR   *sprite;
    Sint32 count;
    Sint32 i;

    if (actor->spriteStorageKind == ACTOR_SPRITE_ONE)
    {
        sprite = actor->sprite;
        if (sprite != 0 && sprite->retainCacheEntry == 0)
        {
            count = sprite->count;
            for (i = 0; i < count; i++)
                sprite->entries[i]->colorMode = TickCount % 6;
            sprite->dirty = 1;
        }
    }
}

// Carry, Lift, Douse, Move and the Djinni's hop into battle throw things this way.
DEFINE_SET_TARGET_POSITION_FROM_MAGNITUDE_ANGLE(, fdActorSetMoveTargetStub)

// Counts the free blocks (VRAM_BLOCK_OWNER_FREE) of the sprite VRAM cache's VRAM_BLOCK_COUNT
// blocks.
Sint32 fdVRAMCacheCountFreeBlocks(void)
{
    Uint8 *marker    = VramCacheBlockOwners;
    Sint32 freeCount = 0;
    Sint32 remaining = VRAM_BLOCK_COUNT;

    do
    {
        if (*marker++ == VRAM_BLOCK_OWNER_FREE)
        {
            freeCount++;
        }
        remaining--;
    } while (remaining != 0);
    return freeCount;
}

// Makes srcSprite share sharedSprite's VRAM block, releasing its own; with no sharedSprite it keeps
// its block.
SPR *fdSpriteShareVRAMCacheEntry(SPR *srcSprite, SPR *sharedSprite)
{
    SPR *shared;
    SPR *sprite;

    sprite = srcSprite;
    shared = NULL;
    if (sprite != NULL)
    {
        if (sharedSprite == NULL)
        {
            sprite->retainCacheEntry = 1;
        }
        else
        {
            syVRAMCacheFreeEntry(sprite->slot);
            sprite->slot             = sharedSprite->slot;
            sprite->retainCacheEntry = 1;
            sprite                   = sharedSprite;
        }
        shared = sprite;
    }
    return shared;
}

// Creates an actor for a field Psynergy, at the caster's ground height, with sprite priority 0 and
// animation 1.
ACTOR *fdActorSpawn(Sint32 kind, Sint32 x, Sint32 y, Sint32 z)
{
    FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
    ACTOR              *actor;

    actor = fdActorCreateStub(kind, x, y, z);
    if (actor != NULL)
    {
        if (actor->spriteStorageKind == ACTOR_SPRITE_NONE)
        {
            fdActorReleaseStub(actor);
            return NULL;
        }
        actor->groundHeight           = effectState->caster->groundHeight;
        actor->motionFlags            = ACTOR_FLOATS;
        actor->priorityFlags          = ACTOR_PRIORITY_OVERHEAD;
        actor->sprite->image.priority = 0;
        fdActorSetSpriteFlagsStub(actor, 0);
        fdActorSelectAnimationStub(actor, 1);
    }
    return actor;
}

// Pauses or resumes every actor but the player's and except, and sets their animations' speed.
void fdActorSetOthersPaused(ACTOR *except, Sint32 paused, Sint32 animationSpeed)
{
    Sint16 *player;
    ACTOR  *actor;
    Sint32  actorId;

    actorId = 0;
    player  = (Sint16 *)PlayerActorId;
    do
    {
        actor = fdActorLookup(actorId);
        if (actorId != *player && actor != NULL && actor != except)
        {
            actor->paused = paused;
            fdActorSetAnimationSpeedStub(actor, animationSpeed);
        }
        actorId++;
        // BUG: one past the actor table: fdActorLookup(ACTOR_ID_COUNT) is the first treasure spot's
        // actor, so the map's first chest or Psynergy Stone is paused too.
#if FIX_FIRST_TREASURE_SPOT_IN_ACTOR_LOOPS
    } while (actorId < ACTOR_ID_COUNT);
#else
    } while (actorId <= ACTOR_ID_COUNT);
#endif
}
