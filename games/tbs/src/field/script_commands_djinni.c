// A Djinni's hop before its battle, a Djinni joining the party, and the field effect sprites the
// joining lights are drawn with. Part of script_commands.c.

#include "script_commands_internal.h"

// Update function shared by the Djinni battle-start and Mercury afterimages.
void fdActorMoveHalfwayTowardTarget(ACTOR *actor)
{
    fdEffectMoveAfterimageHalfway(actor);
}

#define DJINNI_AFTERIMAGES 8 // afterimages of the leaping Djinni

// A Djinni's battle start: the Djinni (actor id) bounces twice, turns to face the leader, hops back
// 16 pixels three times, then leaps 64 pixels at the party trailing eight afterimages tinted for
// its element.
void fdDjinnRunBattleStartHop(Sint32 djinniId)
{
    ACTOR        *djinni;
    ACTOR        *copy;
    ACTOR        *prev;
    void         *sharedSprite;
    Sint32        direction;
    Sint32        back;
    FIXED         distance;
    Sint32        sprite;
    Sint32        tint;
    Sint32        i;
    UNUSED ACTOR *copies[DJINNI_AFTERIMAGES];

    djinni    = fdActorGetById(djinniId);
    direction = FACING_NEAREST_CARDINAL(fdActorGetById(MainRoutineStateWork.leaderActorId)->facing);
    fdScriptStart();
    sySynch(10);
    sySoundPlayStub(SE_NOTE_LOW);
    fdActorRunRepeatedMotion(djinniId, 1);
    sySoundPlayStub(SE_NOTE_HIGH);
    fdActorRunRepeatedMotion(djinniId, 1);
    back = direction + DEGtoANG(180);
    sySynch(20);
    fdActorFaceDirectionById(djinniId, back, 0);
    sySynch(10);
    // Sprite priority 0, over every background, until the end.
    djinni->sprite->image.priority           = 0;
    djinni->facing                           = back;
    fdActorGetById(djinniId)->steeringFlags &= ~ACTOR_FACES_MOTION;
    djinni->motionFlags                      = ACTOR_FALLS;

    // Hop 16 pixels away from the leader, three times.
    distance                                 = toFIXED(16);
    fdActorSetTargetPosFromMagnitudeAngle(djinni, distance, direction);
    sySoundPlayStub(SE_JUMP);
    fdActorJump(djinniId, 4, 0);
    fdActorWaitForMoveStub(djinni);
    fdActorSetTargetPosFromMagnitudeAngle(djinni, distance, direction);
    sySoundPlayStub(SE_JUMP);
    fdActorJump(djinniId, 4, 0);
    fdActorWaitForMoveStub(djinni);
    fdActorSetTargetPosFromMagnitudeAngle(djinni, distance, direction);
    sySoundPlayStub(SE_JUMP);
    fdActorJump(djinniId, 4, 0);
    fdActorWaitForMoveStub(djinni);
    sySynch(20);

    // Afterimages of the same sprite at AFTERIMAGE_SCALE, each closing half the distance to the one
    // before it every tick, tinted by the Djinni's sprite.
    sprite = djinni->sprite->entries[0]->animationId;
    tint   = SPRITE_COLOR_BLUE_TINT;
    if (sprite == SPRITE_VENUS_DJINNI)
        tint = SPRITE_COLOR_RBG;
    if (sprite == SPRITE_MARS_DJINNI)
        tint = SPRITE_COLOR_RED_TINT;
    // DEAD: tint is already SPRITE_COLOR_BLUE_TINT for SPRITE_MERCURY_DJINNI; the ROM keeps the
    // test and the store.
    if (sprite == SPRITE_MERCURY_DJINNI)
        tint = SPRITE_COLOR_BLUE_TINT;
    sharedSprite = NULL;
    prev         = djinni;
    for (i = 0; i < DJINNI_AFTERIMAGES; i++)
    {
        copy      = fdActorCreateStub(sprite, djinni->x.fixed, djinni->y.fixed, djinni->z.fixed);
        // DEAD: nothing reads copies; the ROM keeps the store and its slot.
        copies[i] = copy;
        if (copy != NULL)
        {
            copy->scaleY                  = AFTERIMAGE_SCALE;
            copy->scaleX                  = AFTERIMAGE_SCALE;
            copy->motionFlags             = 0;
            copy->priorityFlags           = ACTOR_PRIORITY_UNDERFOOT;
            copy->steeringFlags          |= ACTOR_FACES_MOTION;
            copy->update                  = fdActorMoveHalfwayTowardTarget;
            copy->facing                  = djinni->facing;
            copy->sprite->image.priority  = 0;
            fdActorSetSpriteColorModeStub(copy, tint);
            fdActorSelectAnimationStub(copy, ANIM_STILL);
            fdActorSetSpriteFlagsStub(copy, 0);
            sharedSprite       = fdSpriteShareVRAMCacheEntry(copy->sprite, sharedSprite);
            copy->watchedActor = (union ACTOR_OR_EFFECT *)prev;
            prev               = copy;
        }
    }

    // Leap 64 pixels at the leader, 12 pixels a tick upward, and after 24 ticks freeze wherever it
    // is, landed or not.
    fdActorSetTargetPosFromMagnitudeAngle(djinni, toFIXED(64), direction + DEGtoANG(180));
    sySoundPlayStub(SE_WHOOSH);
    fdActorJump(djinniId, 12, 0);
    sySynch(24);
    djinni->motionFlags = 0;
    djinni->velocityX   = 0;
    djinni->velocityZ   = 0;
    djinni->velocityY   = 0;
    djinni->targetX     = ACTOR_NO_TARGET;
    djinni->targetZ     = ACTOR_NO_TARGET;
    djinni->targetY     = ACTOR_NO_TARGET;
    fdActorSelectAnimationStub(djinni, ANIM_STILL);
    djinni->sprite->image.priority = 2;
    fdScriptEnd();
}

// A Djinni joins the party: djinn (0 to 19) of element goes to the party member with the fewest
// Djinn, or to no one and only its flag while FLAG_DJINN_LESSON is set. Its actor, actorId (-1 for
// none), plays its element's joining and leaves the scene.
void fdDjinnJoinParty(Sint32 actorId, Sint32 element, Sint32 djinn)
{
    FIELD_STATE *state;
    Sint32       owner;

    state = FieldStateWork;
    if (gmFlagGetStub(FLAG_DJINN_LESSON) != 0)
    {
        owner = 0;
        gmFlagSetStub(djinn + element * DJINN_PER_ELEMENT + FLAG_DJINN_FIRST);
    }
    else
    {
        owner = gmDjinnGiveToPartyStub(element, djinn);
    }

    if (owner >= 0)
    {
        fdScriptStart();
        fdWorldMapFreeze();
        if (actorId != -1)
        {
            // On the World Map the actors far from the view are removed while the Djinni's joining
            // plays and placed again after it.
            if (state->mode == ACTOR_SYSTEM_WORLD_MAP)
                fdActorClearOutOfBounds();

            if (element == ELEMENT_VENUS)
                fdDjinnRunVenusJoinEffect(actorId);
            else if (element == ELEMENT_MERCURY)
                fdDjinnRunMercuryJoinEffect(actorId);
            else if (element == ELEMENT_MARS)
                fdDjinnRunMarsJoinEffect(actorId);
            else if (element == ELEMENT_JUPITER)
                fdDjinnRunJupiterJoinEffect(actorId);

            state->actors[actorId] = 0;
            if (state->mode == ACTOR_SYSTEM_WORLD_MAP)
                fdActorRespawn();
        }

        mnDjinnShowJoinedMessageStub(owner, element, djinn);
        fdWorldMapUnfreeze();
        fdScriptEnd();
    }
}

// Puts a Djinni's actor back where its battle started from: for a pending Djinni battle
// (pendingDjinni DJINNI_BATTLE_SCRIPT_FIRST plus the Djinni's number, FLAG_KEEP_PARTY_POSITION set,
// the battle over) the scene's actor placed for that Djinni (condition = its flag) goes back to its
// placement's position, or 32 pixels north of the leader when the script has DJINN_NO_HOP set, and
// stands.
void fdDjinnRestoreActorAfterBattle(void)
{
    Sint32 flags = MainRoutineStateWork.pendingDjinni & DJINN_PENDING_KIND_MASK;
    Uint32 event = MainRoutineStateWork.pendingDjinni & DJINN_PENDING_SCRIPT_MASK;
    Sint32 id;

    // Only a Djinni battle; a mimic chest's is skipped.
    if (gmFlagGetStub(FLAG_KEEP_PARTY_POSITION) != 0 && flags == PENDING_DJINNI_BATTLE)
    {
        flags  = event & DJINN_NO_HOP;
        event &= DJINN_PENDING_BATTLE_SCRIPT_MASK;
        // script is unsigned: scripts below DJINNI_BATTLE_SCRIPT_FIRST fail the range test too.
        if (event - DJINNI_BATTLE_SCRIPT_FIRST <= ELEMENT_COUNT * DJINN_PER_ELEMENT
            && MainRoutineStateWork.modeResult > 0)
        {
            for (id = ACTOR_FIRST_PLACED; id <= ACTOR_LAST_ID; id++)
            {
                ACTOR_PLACE *descriptor = fdFindActorPlacement(id);
                ACTOR       *actor;

                if (descriptor == NULL
                    || descriptor->condition - FLAG_DJINN_FIRST
                           != event - DJINNI_BATTLE_SCRIPT_FIRST)
                    continue;
                actor = fdActorLookup(id);
                if (actor == NULL)
                    continue;
                // Without DJINN_NO_HOP, back at its placement's position in a new actor's ground
                // state: no ground height yet, following the ground and falling.
                if (flags == 0)
                {
                    actor->groundHeight = flags;
                    actor->motionFlags  = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
                    fdActorSetPosAndResetMotionStub(
                        actor, descriptor->x, descriptor->y, descriptor->z);
                }
                else
                {
                    ACTOR *leader = fdActorLookup(MainRoutineStateWork.leaderActorId);

                    fdActorSetPosAndResetMotionStub(
                        actor, leader->x.fixed, leader->y.fixed, leader->z.fixed - toFIXED(32));
                }
                fdActorSelectAnimationStub(actor, ANIM_STAND);
            }
        }
    }
}

// Settles a pending Djinni or mimic battle as a scene loads, from the battle's result in modeResult
// (the enemies defeated, BATTLE_OUTCOME_FLED when the party fled, BATTLE_OUTCOME_LOST when it
// lost): with shown 0, before the scene shows, a lost or fled Djinni hides again and a beaten
// mimic's flag is set; once it shows, a beaten Djinni joins the party.
void fdDjinnResolvePending(Sint32 shown)
{
    MAIN_ROUTINE_STATE *mainState = &MainRoutineStateWork;
    Sint32              kind      = mainState->pendingDjinni & DJINN_PENDING_KIND_MASK;
    Sint32              flag      = mainState->pendingDjinni & DJINN_PENDING_SCRIPT_MASK;

    if (shown == 0)
    {
        if (kind == PENDING_DJINNI_BATTLE)
        {
            flag &= DJINN_PENDING_BATTLE_SCRIPT_MASK;
            if (flag < DJINNI_BATTLE_SCRIPT_FIRST
                || flag > DJINNI_BATTLE_SCRIPT_FIRST + ELEMENT_COUNT * DJINN_PER_ELEMENT)
                return;
            // Won: the script waits for the call once the scene is shown.
            if (mainState->modeResult > 0 && mainState->modeResult != BATTLE_OUTCOME_FLED)
                return;
            gmFlagSetStub(flag - (DJINNI_BATTLE_SCRIPT_FIRST - FLAG_DJINN_HIDING_FIRST));
            mainState->pendingDjinni = kind;
        }
        else if (kind == PENDING_MIMIC_CHEST)
        {
            if (mainState->modeResult == 1)
                gmFlagSetStub(flag);
            mainState->pendingDjinni = shown;
        }
        return;
    }
    if (kind == PENDING_DJINNI_BATTLE)
    {
        flag &= DJINN_PENDING_BATTLE_SCRIPT_MASK;
        if (flag >= DJINNI_BATTLE_SCRIPT_FIRST
            && flag <= DJINNI_BATTLE_SCRIPT_FIRST + ELEMENT_COUNT * DJINN_PER_ELEMENT)
        {
            flag &= DJINN_PENDING_BATTLE_SCRIPT_MASK;
            if (mainState->modeResult > 0)
            {
                Sint32 djinni  = flag - DJINNI_BATTLE_SCRIPT_FIRST;
                Sint32 element = djinni / DJINN_PER_ELEMENT;
                Sint32 index   = djinni % DJINN_PER_ELEMENT;
                Sint32 actorId;

                for (actorId = ACTOR_FIRST_PLACED; actorId <= ACTOR_LAST_ID; actorId++)
                {
                    ACTOR_PLACE *found = fdFindActorPlacement(actorId);
                    if (found != 0
                        && found->condition - FLAG_DJINN_FIRST == flag - DJINNI_BATTLE_SCRIPT_FIRST)
                    {
                        sySynch(40);
                        fdDjinnJoinParty(actorId, element, index);
                        break;
                    }
                }
            }
        }
    }
    MainRoutineStateWork.pendingDjinni = 0;
}

// Event: updates the 24 field effect sprites.
void fdEffectUpdateAllSlots(void)
{
    fdEffectUpdateEffectSlots();
}

// Allocates and clears the field effect sprites (HEAP_SLOT_FIELD_EFFECT) and starts their update
// event.
void fdEffectInitSlots(void)
{
    void *state;

    state = syHeapSlotAllocate(HEAP_SLOT_FIELD_EFFECT, sizeof(FIELD_EFFECT_STATE));
    DMA_FILL32(0, state, sizeof(FIELD_EFFECT_STATE));
    sySetEvent(fdEffectUpdateAllSlots, EVENT_PASS_TICK);
}

// Stops the field effect sprites: removes their event, frees every slot still in use and releases
// their heap block, FieldEffectStateWork with it.
void fdEffectReleaseSlots(void)
{
    FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
    FIELD_PARTICLE     *effect;
    Sint32              i;

    syCloseEvent(fdEffectUpdateAllSlots);
    for (i = 0; i < PSYNERGY_EFFECT_SLOT_COUNT; i++)
    {
        effect = &effectState->particles[i];
        if (effect->active != 0)
            fdEffectClearOwnedSlot(effect);
    }
    syHeapSlotRelease(HEAP_SLOT_FIELD_EFFECT);
    sySynch(1);
}

// Per-tick update: spins an actor 1/8 turn a tick.
void fdEffectAdvanceSpinAngle(ACTOR *actor)
{
    actor->facing += DEGtoANG(45);
}

// The ticks a Jupiter Djinni's light widens, and later closes in over, so that it ends where it
// began; and the ticks it circles and drifts.
#define DJINNI_LIGHT_RADIUS_TICKS   60
#define DJINNI_LIGHT_CIRCLING_TICKS 40
#define DJINNI_LIGHT_DRIFTING_TICKS 40

// The states of a Jupiter Djinni's light particle, in order.
typedef enum DJINNI_LIGHT_STATE
{
    DJINNI_LIGHT_WIDENING = 0, // circles as its radius grows
    DJINNI_LIGHT_CIRCLING = 1,
    DJINNI_LIGHT_DRIFTING = 2, // its centre drifts toward the leader
    DJINNI_LIGHT_CLOSING  = 3, // circles as its radius shrinks
    DJINNI_LIGHT_DONE     = 4  // frees its slot
} DJINNI_LIGHT_STATE;

// Field effect sprite callback of a Jupiter Djinni's light particles, the ring of coloured lights a
// joining Jupiter Djinni bursts into: each circles its centre while it widens, circles, drifts
// toward a point 20 pixels over the leader and closes in on it, and is freed. It turns 1/32 turn a
// tick throughout, and its radius grows, then shrinks, by 1 pixel a tick.
void fdEffectRunAngledApproachPhases(FIELD_PARTICLE *particle)
{
    ACTOR  *target;
    VECTOR3 pos;

    target = fdActorGetById(MainRoutineStateWork.leaderActorId);
    if (particle->state == DJINNI_LIGHT_WIDENING)
    {
        particle->orbitRadius += 1;
        particle->orbitAngle  += 1;
        if (particle->age == DJINNI_LIGHT_RADIUS_TICKS)
        {
            particle->age    = 0;
            particle->state += 1;
        }
    }
    else if (particle->state == DJINNI_LIGHT_CIRCLING)
    {
        particle->orbitAngle += 1;
        if (particle->age == DJINNI_LIGHT_CIRCLING_TICKS)
        {
            particle->age    = 0;
            particle->state += 1;
        }
    }
    else if (particle->state == DJINNI_LIGHT_DRIFTING)
    {
        particle->orbitAngle += 1;
        pos.x                 = target->x.fixed;
        pos.y                 = target->y.fixed + toFIXED(20);
        pos.z                 = target->z.fixed;
        // The centre moves 1/8 of the way to the leader's screen point a tick.
        fdCameraWorldToScreen(&pos);
        particle->originX += (pos.x - particle->originX) / 8;
        particle->originZ += (pos.z - particle->originZ) / 8;
        if (particle->age == DJINNI_LIGHT_DRIFTING_TICKS)
        {
            particle->age    = 0;
            particle->state += 1;
        }
    }
    else if (particle->state == DJINNI_LIGHT_CLOSING)
    {
        particle->orbitRadius -= 1;
        particle->orbitAngle  += 1;
        if (particle->age == DJINNI_LIGHT_RADIUS_TICKS)
        {
            particle->age    = 0;
            particle->state += 1;
        }
    }
    else if (particle->state == DJINNI_LIGHT_DONE)
    {
        fdEffectClearOwnedSlot(particle);
    }
    pos.x = particle->originX;
    pos.z = particle->originZ;
    syVectorAddPolarOffset(
        toFIXED(particle->orbitRadius), particle->orbitAngle * (TRIG_TURN / 32), (FIXED *)&pos);
    particle->x = pos.x;
    particle->z = pos.z;
}

#undef DJINNI_AFTERIMAGES
#undef DJINNI_LIGHT_RADIUS_TICKS
#undef DJINNI_LIGHT_CIRCLING_TICKS
#undef DJINNI_LIGHT_DRIFTING_TICKS
