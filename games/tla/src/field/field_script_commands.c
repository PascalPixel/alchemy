// Actor script commands: waits, jumps, flags, moves and work values.
//
// Each `SC_` handler here is reached through ActorScriptCommandTable (map_grid_data.c), which
// fdActorUpdateAllOnField calls for each live actor; each returns 1 to run on and 0 to end the
// actor's tick (the protocol is in this folder's README). Part of field.c.

#include "sounds.h"

// The actor script commands' limits, angles and distances.
enum
{
    SCRIPT_LABEL_SEARCH_WORDS    = 1024, // script words a jump searches
    SCRIPT_BLOCKED_MOVE_TICKS    = 60,   // refused ticks SC_SYNC waits through
    SCRIPT_WANDER_TRIES          = 7,    // random points a wander tries a tick
    // The wander probes' distance, 8 pixels in 16.16: past the point for ground, and along the
    // heading for solid actors (SC_WANDER_HOME).
    SCRIPT_WANDER_PROBE_DISTANCE = toFIXED(8),
    SCRIPT_WANDER_DIAGONAL_ANGLE = DEGtoANG(45), // one eighth of a turn
    SCRIPT_WANDER_SIDE_ANGLE     = DEGtoANG(90), // one quarter of a turn
    SCRIPT_HALF_TURN             = DEGtoANG(180),
    // Whole pixels SC_APPROACH_WATCHED stays short of its watched actor.
    SCRIPT_APPROACH_DISTANCE     = 16,
    // Under 64 pixels (16.16) SC_CAMERA measures its distance in 16.16.
    CAMERA_FOLLOW_FINE_DISTANCE  = toFIXED(64),
    // Within a quarter pixel (16.16) SC_CAMERA lands on its point.
    CAMERA_FOLLOW_SNAP_DISTANCE  = toFIXED(1) / 4,
    // Within half a pixel (16.16) SC_CAMERA takes the leader's height at once.
    CAMERA_FOLLOW_HEIGHT_SNAP    = toFIXED(1) / 2
};

// SC_LABEL and SC_TO differ only in bit 30; a jump searches for the label.
#define SCRIPT_LABEL_KEY_MASK 0xbfffffff

// Advances the motion of those of the first RUNNING_LIST_SLOTS actors whose script is running by
// one tick, without running their scripts: steering towards a target or braking, gravity and
// bouncing, arrival, and turning to face the direction of travel.
void fdActorUpdateAllMotion(void)
{
    ACTOR  *list[RUNNING_LIST_SLOTS + 1];
    ACTOR **next;
    ACTOR  *actor;
    FIXED   y;
    Sint32  arrivedAtTarget;
    FIXED   x;
    FIXED   z;
    Sint32  dx;
    Sint32  dz;
    Sint32  distance;
    FIXED   ratio;
    Sint32  facingStep;

    // Those of the first RUNNING_LIST_SLOTS actors whose script is running.
    actor = RamHeapSlots->actors;
    fdActorListRunning(actor, list);
    actor = list[0];
    next  = &list[1];
    while (actor != NULL)
    {
        x = actor->x.fixed;
        y = actor->y.fixed;
        z = actor->z.fixed;
        // UB: a frozen actor otherwise reads an uninitialized arrival flag, or inherits the
        // preceding actor's flag.
#if FIX_UNINITIALIZED_READ
        arrivedAtTarget = 0;
#endif
        if (actor->frozen == 0)
        {
#if !FIX_UNINITIALIZED_READ
            arrivedAtTarget = 0;
#endif
            // Steer in the XZ plane towards the target, or brake.
            if (actor->targetX != ACTOR_NO_TARGET)
            {
                dx       = (actor->targetX - x) / toFIXED(1);
                dz       = (actor->targetZ - z) / toFIXED(1);
                distance = toFIXED(sySquartIWRAM(dx * dx + dz * dz));
                // Under 256 pixels, measure again in 16.16: the root of a 16.16 square is 8.8, and
                // << 8 makes it 16.16 again.
                if (distance <= toFIXED(256) - 1)
                {
                    dx       = actor->targetX - x;
                    dz       = actor->targetZ - z;
                    distance = sySquartIWRAM(syMulFX(dx, dx) + syMulFX(dz, dz)) << 8;
                }
                if (distance == 0)
                {
                    x = actor->targetX;
                    z = actor->targetZ;
                }
                else
                {
                    ratio             = syDivFX(distance, actor->acceleration);
                    actor->velocityX += syMulFX(dx, ratio);
                    actor->velocityZ += syMulFX(dz, ratio);
                    dx                = actor->velocityX;
                    dz                = actor->velocityZ;
                    distance          = sySquartIWRAM(syMulFX(dx, dx) + syMulFX(dz, dz)) << 8;
                    if (distance > actor->speedLimit)
                    {
                        ratio            = syDivFX(distance, actor->speedLimit);
                        actor->velocityX = syMulFX(dx, ratio);
                        actor->velocityZ = syMulFX(dz, ratio);
                    }
                }
            }
            else
            {
                dx = actor->velocityX;
                dz = actor->velocityZ;
                if ((dx | dz) != 0)
                {
                    distance = sySquartIWRAM(syMulFX(dx, dx) + syMulFX(dz, dz)) << 8;
                    if (distance != 0)
                    {
                        if (distance - actor->acceleration < 0)
                        {
                            actor->velocityX = 0;
                            actor->velocityZ = 0;
                        }
                        else
                        {
                            ratio            = syDivFX(distance, distance - actor->acceleration);
                            actor->velocityX = syMulFX(dx, ratio);
                            actor->velocityZ = syMulFX(dz, ratio);
                        }
                    }
                    else
                    {
                        actor->velocityX = 0;
                        actor->velocityZ = 0;
                    }
                }
            }
            // Gravity, and a bounce that loses energy on landing.
            if (actor->motionFlags & ACTOR_FALLS)
            {
                if (y > actor->groundHeight)
                {
                    actor->velocityY -= actor->gravity;
                }
                else if (actor->velocityY < 0)
                {
                    y                = actor->groundHeight;
                    actor->velocityY = -syMulFX(actor->velocityY, actor->bounce);
                    if ((actor->velocityY < 0 ? -actor->velocityY : actor->velocityY)
                        <= actor->gravity)
                        actor->velocityY = 0;
                }
            }
        }
        x += actor->velocityX;
        y += actor->velocityY;
        z += actor->velocityZ;
        // Arrived: the watched axis reached the target, or crossed it.
        if (actor->arrivalAxis != 0)
            switch (actor->arrivalAxis)
            {
                case ARRIVAL_AXIS_X:
                    if (x == actor->targetX
                        || ((actor->x.fixed - actor->targetX) ^ (x - actor->targetX)) < 0)
                        arrivedAtTarget = 1;
                    break;
                case ARRIVAL_AXIS_Y:
                    if (y == actor->targetY
                        || ((actor->y.fixed - actor->targetY) ^ (y - actor->targetY)) < 0)
                        arrivedAtTarget = 1;
                    break;
                case ARRIVAL_AXIS_Z:
                    if (z == actor->targetZ
                        || ((actor->z.fixed - actor->targetZ) ^ (z - actor->targetZ)) < 0)
                        arrivedAtTarget = 1;
                    break;
            }
        if (arrivedAtTarget)
        {
            if (actor->snapToTarget)
            {
                actor->velocityX = 0;
                actor->velocityZ = 0;
                x                = actor->targetX;
                z                = actor->targetZ;
                if (actor->motionFlags == 0)
                {
                    y                = actor->targetY;
                    actor->velocityY = 0;
                }
            }
            actor->targetX     = ACTOR_NO_TARGET;
            actor->targetY     = ACTOR_NO_TARGET;
            actor->targetZ     = ACTOR_NO_TARGET;
            actor->arrivalAxis = 0;
        }
        SET_ACTOR_POSITION(actor, x, y, z);
        // Turn to face the direction of travel, at most a sixteenth of a turn a tick, the shorter
        // way round.
        if (actor->steeringFlags & ACTOR_FACES_MOTION)
        {
            x = actor->velocityX;
            z = actor->velocityZ;
            if (x != 0 || z != 0)
            {
                facingStep = (Sint16)(syAtan(z, x) - actor->facing);
                if (facingStep > FACING_STEP)
                    facingStep = FACING_STEP;
                if (facingStep < -FACING_STEP)
                    facingStep = -FACING_STEP;
                actor->facing += facingStep;
            }
        }
        actor = *next++;
    }
}

// SC_WAIT(n): waits n ticks; this tick counts as the first.
Sint32 fdActorScriptWait(ACTOR *interpreter)
{
    Sint16 commandCursor       = interpreter->scriptCursor;

    interpreter->wait          = interpreter->script[commandCursor + 1] - 1;
    interpreter->scriptCursor += 2;
    return 0;
}

// SC_SYNC: waits until the actor arrives at its target, or until a solid actor has blocked it for
// SCRIPT_BLOCKED_MOVE_TICKS ticks (refused is the blocked count, which fdActorUpdateAllOnField
// raises), when its target is cleared.
Bool fdActorScriptWaitForArrival(ACTOR *interpreter)
{
    WAITING_ACTOR *actor = (WAITING_ACTOR *)interpreter;

    if (actor->refused > SCRIPT_BLOCKED_MOVE_TICKS - 1)
    {
        actor->refused     = 0;
        actor->targetX     = ACTOR_NO_TARGET;
        actor->targetY     = ACTOR_NO_TARGET;
        actor->targetZ     = ACTOR_NO_TARGET;
        actor->arrivalAxis = 0;
        interpreter->scriptCursor++;
        return TRUE;
    }
    if (fdActorIsTargetUnset(interpreter) != 0)
    {
        interpreter->scriptCursor++;
        return TRUE;
    }
    return FALSE;
}

// SC_CALL(fn): calls fn with the actor.
Bool fdActorScriptInvokeCallback(ACTOR *interpreter)
{
    Sint16         initCursor = interpreter->scriptCursor;
    SCRIPT_COMMAND callback   = (SCRIPT_COMMAND)interpreter->script[initCursor + 1];

    if (callback(interpreter) != 0)
        return FALSE;
    if (interpreter->scriptCursor == initCursor)
        interpreter->scriptCursor = interpreter->scriptCursor + 2;
    return TRUE;
}

// Returns the cursor just past the script's label key (SC_LABEL or SC_TO, matched without the bit
// that tells them apart), searching its first SCRIPT_LABEL_SEARCH_WORDS words, or 0 (the start)
// when key is 0 or not found, so a missing label restarts the script. Clears the wait.
Sint32 fdActorScriptFindLabel(ACTOR *interpreter, Uint32 labelKey)
{
    Uint32 *scriptWords;
    Sint32  wordIndex;

    interpreter->wait = 0;
    if (labelKey == 0)
    {
        return 0;
    }

    labelKey    &= SCRIPT_LABEL_KEY_MASK;
    scriptWords  = (Uint32 *)interpreter->script;
    for (wordIndex = 0; wordIndex < SCRIPT_LABEL_SEARCH_WORDS; wordIndex++)
    {
        if (*scriptWords++ == labelKey)
        {
            return wordIndex + 1;
        }
    }
    return 0;
}

// SC_LOOP(n, to): jumps to label to until it has done so n - 1 times, then moves on; with n =
// SC_FOREVER it always jumps.
Sint32 fdActorScriptRepeatOrJump(ACTOR *interpreter)
{
    const Sint32 *arguments;
    Sint32        repeatLimit;
    Sint32        jumpKey;
    Uint8        *repeatCount;
    Sint32        nextCount;

    arguments   = &interpreter->script[interpreter->scriptCursor + 1];
    repeatLimit = *arguments++;
    jumpKey     = *arguments;
    if (repeatLimit == SC_FOREVER)
    {
        interpreter->scriptCursor = fdActorScriptFindLabel(interpreter, jumpKey);
    }
    else
    {
        repeatCount  = &interpreter->repeatCount;
        nextCount    = *repeatCount + 1;
        *repeatCount = nextCount;
        if ((Sint32)(Uint8)nextCount < (Sint32)(Sint16)repeatLimit)
        {
            interpreter->scriptCursor = fdActorScriptFindLabel(interpreter, jumpKey);
        }
        else
        {
            *repeatCount               = 0;
            interpreter->scriptCursor += 3;
        }
    }
    return 1;
}

// SC_JUMP(to): jumps to label to.
Sint32 fdActorScriptJumpToLabel(ACTOR *interpreter)
{
    interpreter->scriptCursor =
        fdActorScriptFindLabel(interpreter, interpreter->script[interpreter->scriptCursor + 1]);
    return 1;
}

// SC_IF(to): jumps to label to when the last test was true.
Sint32 fdActorScriptJumpIfTrue(ACTOR *interpreter)
{
    Sint32        commandCursor = interpreter->scriptCursor;
    const Sint32 *script        = interpreter->script;
    Uint32        labelKey      = script[commandCursor + 1];

    if (interpreter->conditionResult != 0)
    {
        interpreter->scriptCursor = fdActorScriptFindLabel(interpreter, labelKey);
    }
    else
    {
        interpreter->scriptCursor += 2;
    }
    return 1;
}

// SC_IFNOT(to): jumps to label to when the last test was false.
Sint32 fdActorScriptJumpIfFalse(ACTOR *interpreter)
{
    Sint32        commandCursor = interpreter->scriptCursor;
    const Sint32 *script        = interpreter->script;
    Uint32        labelKey      = script[commandCursor + 1];

    if (interpreter->conditionResult == 0)
    {
        interpreter->scriptCursor = fdActorScriptFindLabel(interpreter, labelKey);
    }
    else
    {
        interpreter->scriptCursor += 2;
    }
    return 1;
}

// SC_END: hands the actor back to the idle script, ActorScriptIdleScript, whose lone SC_END runs
// again each tick. Returns 0.
Sint32 fdActorScriptEnd(ACTOR *interpreter)
{
    Sint32 result;

    interpreter->script       = ActorScriptIdleScript;
    interpreter->scriptCursor = (result = 0);
    return result;
}

// SC_FLAG_CHECK(f): tests game flag f.
Sint32 fdActorScriptTestFlag(ACTOR *interpreter)
{
    Sint32 done;

    interpreter->conditionResult =
        syGameFlagGet(interpreter->script[interpreter->scriptCursor + 1]);
    done                       = 1;
    interpreter->scriptCursor += 2;
    return done;
}

// SC_FLAG_ON(f): tests game flag f, then sets it.
Sint32 fdActorScriptSetFlagAndTest(ACTOR *interpreter)
{
    Sint32 value;

    value                        = interpreter->script[interpreter->scriptCursor + 1];
    interpreter->conditionResult = syGameFlagGet(value);
    syGameFlagSet(value);
    interpreter->scriptCursor += 2;
    return 1;
}

// SC_FLAG_OFF(f): tests game flag f, then clears it.
Sint32 fdActorScriptClearFlagAndTest(ACTOR *interpreter)
{
    Sint32 value;

    value                        = interpreter->script[interpreter->scriptCursor + 1];
    interpreter->conditionResult = syGameFlagGet(value);
    syGameFlagClear(value);
    interpreter->scriptCursor += 2;
    return 1;
}

// SC_FLAG_REV(f): tests game flag f, then flips it.
Sint32 fdActorScriptToggleFlagAndTest(ACTOR *interpreter)
{
    Sint32 value;

    value                        = interpreter->script[interpreter->scriptCursor + 1];
    interpreter->conditionResult = syGameFlagGet(value);
    if (interpreter->conditionResult == 1)
        syGameFlagClear(value);
    else
        syGameFlagSet(value);
    interpreter->scriptCursor += 2;
    return 1;
}

// SC_ANIMATION(v): switches the actor's sprites to animation v.
Sint32 fdActorScriptSetAnimation(ACTOR *interpreter)
{
    fdActorSelectAnimation(interpreter, interpreter->script[interpreter->scriptCursor + 1]);
    interpreter->scriptCursor += 2;
    return 1;
}

// SC_RELEASE: releases the actor through fdActorRelease, which releases its sprites and zeroes its
// slot.
Sint32 fdActorScriptRelease(ACTOR *interpreter)
{
    fdActorRelease(interpreter);
    return 0;
}

// SC_NOP(v): does nothing with its operand.
Sint32 fdActorScriptNoOperation(ACTOR *interpreter)
{
    interpreter->scriptCursor += 2;
    return 1;
}

// SC_SE(n): sends sound command n.
Sint32 fdActorScriptPlaySound(ACTOR *interpreter)
{
    sySoundPlayStub(interpreter->script[interpreter->scriptCursor + 1]);
    interpreter->scriptCursor += 2;
    return 1;
}

// SC_SE_IN_VIEW(n): plays sound n when the actor is in view of the camera (fdActorPlaySoundInView).
Sint32 fdActorScriptPlaySoundInView(ACTOR *interpreter)
{
    fdActorPlaySoundInViewStub(interpreter, interpreter->script[interpreter->scriptCursor + 1]);
    interpreter->scriptCursor += 2;
    return 1;
}

// Returns -1 when an actor at position values would overlap a solid actor, each radius less 2
// pixels; otherwise 0.
Sint32 fdActorScriptCheckOverlap(ACTOR *actor, Sint32 *values)
{
    Sint32 actorIndex;
    ACTOR *candidate = ((ACTOR *)RamHeapSlots->actors);

    for (actorIndex = 0; actorIndex < ACTOR_COUNT; actorIndex++, candidate++)
    {
        if (candidate->script != NULL && (candidate->collisionFlags & ACTOR_SOLID)
            && candidate != actor)
        {
            if (fdCheckSpheresOverlap(
                    &candidate->x.fixed, candidate->radius - 2, values, actor->radius - 2)
                >= 0)
                return -1;
        }
    }
    return 0;
}

// Returns the first solid actor that an actor at position values would overlap, as
// fdActorScriptCheckOverlap tests, or NULL.
ACTOR *fdActorScriptFindOverlappingActor(ACTOR *actor, Sint32 *values)
{
    Sint32 actorIndex;
    ACTOR *candidate = ((ACTOR *)RamHeapSlots->actors);

    for (actorIndex = 0; actorIndex < ACTOR_COUNT; actorIndex++, candidate++)
    {
        if (candidate->script != NULL && (candidate->collisionFlags & ACTOR_SOLID)
            && candidate != actor)
        {
            if (fdCheckSpheresOverlap(
                    &candidate->x.fixed, candidate->radius - 2, values, actor->radius - 2)
                >= 0)
                return candidate;
        }
    }
    return NULL;
}

// SC_POSITION(x, y, z): places the actor at (x, y, z) and stops it; with y -1 an actor that follows
// the ground is raised by the ground height there.
Sint32 fdActorScriptSetPosAndResetMotion(ACTOR *actor)
{
    const Sint32 *argument = actor->script + actor->scriptCursor + 1;
    Sint32        x        = *argument++;
    Sint32        y        = *argument++;
    Sint32        z        = *argument;
    FIXED         height;

    fdActorSetPosAndResetMotion(actor, x, y, z);
    if (y == -1 && (actor->motionFlags & ACTOR_FOLLOWS_TERRAIN))
    {
        height         = fdMapGridGetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);
        actor->y.fixed = actor->y.fixed - actor->groundHeight + height;
        actor->groundHeight = height;
    }
    actor->scriptCursor += 4;
    return 1;
}

// SC_POSITION_ORIGIN: puts the actor at x and z 0, keeping its height, with no destination or
// speed; one that follows the ground then takes the ground height there.
Sint32 fdActorScriptResetToOrigin(ACTOR *actor)
{
    FIXED height;

    fdActorSetPosAndResetMotion(actor, 0, actor->y.fixed, 0);
    if (actor->motionFlags & ACTOR_FOLLOWS_TERRAIN)
    {
        height         = fdMapGridGetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);
        actor->y.fixed = actor->y.fixed - actor->groundHeight + height;
        actor->groundHeight = height;
    }
    actor->scriptCursor += 1;
    return 1;
}

// SC_MOVE(x, y, z): sends the actor towards (x, y, z).
Sint32 fdActorScriptApplyAbsolutePos(ACTOR *actor)
{
    const Sint32 *argument = actor->script + actor->scriptCursor + 1;
    Sint32        x        = *argument++;
    Sint32        y        = *argument++;
    Sint32        z        = *argument;

    fdActorSetMoveTarget(actor, x, y, z);
    actor->scriptCursor += 4;
    return 1;
}

// SC_MOVE_XZ(x, z): sends the actor towards (x, z), keeping its height.
Sint32 fdActorScriptSetMoveTargetXZ(ACTOR *actor)
{
    const Sint32 *argument = actor->script + actor->scriptCursor + 1;
    Sint32        x        = *argument++;
    Sint32        z        = *argument;

    fdActorSetMoveTarget(actor, x, actor->y.fixed, z);
    actor->scriptCursor += 3;
    return 1;
}

// SC_MOVE_BY(x, y, z): sends the actor towards its position plus (x, y, z).
Sint32 fdActorScriptApplyRelativePos(ACTOR *actor)
{

    Uint8  *commandBytes = (Uint8 *)(actor->script + actor->scriptCursor);
    Sint32 *arguments    = (Sint32 *)(commandBytes + 4);
    Sint32  deltaX       = *arguments++;
    Sint32  deltaY       = *arguments++;
    Sint32  deltaZ       = *arguments;

    fdActorSetMoveTarget(
        actor, actor->x.fixed + deltaX, actor->y.fixed + deltaY, actor->z.fixed + deltaZ);
    actor->scriptCursor += 4;
    return 1;
}

// SC_FACE_WATCHED: turns the actor to face its watched actor, on the ground plane (x and z).
Sint32 fdActorScriptStoreAngleToWatchedActor(ACTOR *actor)
{

    ACTOR *target;

    target        = (ACTOR *)actor->watchedActor;
    actor->facing = syAtan((target->z.fixed - actor->z.fixed), (target->x.fixed - actor->x.fixed));
    actor->scriptCursor++;
    return 1;
}

// SC_MOVE_TO_WATCHED: sends the actor towards its watched actor.
Sint32 fdActorScriptApplyWatchedActorPos(ACTOR *actor)
{

    ACTOR *target;

    target = (ACTOR *)actor->watchedActor;
    fdActorSetMoveTarget(actor, target->x.fixed, target->y.fixed, target->z.fixed);
    actor->scriptCursor++;
    return 1;
}

// SC_MOVE_LOCAL(a, b): sends the actor towards its position plus an offset that is never filled in;
// the operands are skipped unread.
Sint32 fdActorScriptApplyLocalOffsetPos(ACTOR *actor)
{

    Sint32 localOffset[3];

    // BUG: offset is never set, so the target is the position plus whatever the stack held.
#if FIX_UNINITIALIZED_READ
    localOffset[0] = localOffset[1] = localOffset[2] = 0;
#endif
    fdActorSetMoveTarget(actor,
                         actor->x.fixed + localOffset[0],
                         actor->y.fixed + localOffset[1],
                         actor->z.fixed + localOffset[2]);
    actor->scriptCursor += 3;
    return 1;
}

// How far apart a and b are.
#define DISTANCE(a, b) ((a) - (b) < 0 ? (b) - (a) : (a) - (b))

// SC_CAMERA: keeps the camera actor on its leader (watchedActor), clamped so the view stays on the
// map, and pushed out of the map's camera boxes, which the view may not show.
Sint32 fdCameraFollowLeaderInBounds(ACTOR *interpreter)
{
    ACTOR     *camera = interpreter;
    ACTOR     *leader;
    MAP_STATE *bounds;
    FIXED      minX;
    FIXED      minZ;
    FIXED      maxX;
    FIXED      maxZ;
    FIXED      x;
    FIXED      y;
    FIXED      z;
    Sint32     dz;
    Sint32     dy;
    Sint32     dx;
    FIXED      distance;
    FIXED      moveStep;
    Sint16    *region;

    bounds = RamHeapSlots->mapGrid;
    leader = (ACTOR *)camera->watchedActor;
    // UB: the bounds read the leader's height before the null check below.
#if FIX_NULL_DEREFERENCE
    if (leader == NULL)
    {
        camera->motionFlags = 0;
        camera->scriptCursor++;
        return 1;
    }
#endif
    // A layered map draws a point at screen row z - y, so the z bounds move with the leader's
    // height and the clamp keeps that row inside the map.
    minX                = bounds->minX + FIELD_VIEW_FOCUS_X;
    minZ                = bounds->minZ + leader->y.fixed + FIELD_VIEW_FOCUS_Y;
    maxX                = bounds->maxX - FIELD_VIEW_FOCUS_X;
    maxZ                = bounds->maxZ + leader->y.fixed - (FIELD_VIEW_HEIGHT - FIELD_VIEW_FOCUS_Y);
    camera->motionFlags = 0;
    if (leader != NULL && leader->script != NULL)
    {
        x               = leader->x.fixed;
        y               = leader->y.fixed;
        z               = leader->z.fixed;
        camera->targetX = ACTOR_NO_TARGET;
        camera->targetY = ACTOR_NO_TARGET;
        camera->targetZ = ACTOR_NO_TARGET;
        if (x < minX)
            x = minX;
        if (z < minZ)
            z = minZ;
        if (x > maxX)
            x = maxX;
        if (z > maxZ)
            z = maxZ;
        if (bounds->cameraBoxes != NULL)
        {
            region = bounds->cameraBoxes;
            while (region[0] != -1)
            {
                // The points from which the view would show some of the box.
                minX = toFIXED(region[0]) - FIELD_VIEW_FOCUS_X;
                minZ =
                    toFIXED(region[1]) + leader->y.fixed - (FIELD_VIEW_HEIGHT - FIELD_VIEW_FOCUS_Y);
                maxX = toFIXED(region[2]) + FIELD_VIEW_FOCUS_X;
                maxZ = toFIXED(region[3]) + leader->y.fixed + FIELD_VIEW_FOCUS_Y;
                if (x >= minX && z >= minZ && x <= maxX && z <= maxZ)
                {
                    dx = minX;
                    if (DISTANCE(minX, x) > DISTANCE(maxX, x))
                        dx = maxX;
                    dz = minZ;
                    if (DISTANCE(minZ, z) > DISTANCE(maxZ, z))
                        dz = maxZ;
                    if (DISTANCE(dx, x) <= DISTANCE(dz, z))
                        x = dx;
                    else
                        z = dz;
                    break;
                }
                region += 4;
            }
        }
        if (camera->timer != 0)
        {
            SET_ACTOR_POSITION(camera, x, y, z);
        }
        else
        {
            dx       = (x - camera->x.fixed) / toFIXED(1);
            // LEFTOVER(template): this whole-pixel dy, overwritten below and
            // * not used for the distance; Camelot's template: the sibling
            // * routine fdActorSetMoveTarget (field_actor_motion.c) computes the
            // * same three whole-pixel deltas and uses y's in its distance; as
            // * the line removed.
            dy       = (y - camera->y.fixed) / toFIXED(1);
            dz       = (z - camera->z.fixed) / toFIXED(1);
            distance = toFIXED(sySquartIWRAM(dx * dx + dz * dz));
            dx       = x - camera->x.fixed;
            dy       = y - camera->y.fixed;
            dz       = z - camera->z.fixed;
            if (distance < CAMERA_FOLLOW_FINE_DISTANCE)
                distance = sySquartFX(syIWRAMMulFX(dx, dx) + syIWRAMMulFX(dz, dz));
            if ((moveStep = distance / 8) > camera->speedLimit)
                moveStep = camera->speedLimit;
            if (distance < CAMERA_FOLLOW_SNAP_DISTANCE)
            {
                camera->x.fixed = x;
                camera->z.fixed = z;
            }
            else
            {
                if (distance > moveStep)
                {
                    FIXED  ratio                    = syDivFX(distance, dx);
                    Sint32 (*mulFX)(Sint32, Sint32) = syIWRAMMulFX;
                    dx                              = mulFX(ratio, moveStep);
                    dz                              = mulFX(syDivFX(distance, dz), moveStep);
                }
                camera->x.fixed += dx;
                camera->z.fixed += dz;
            }
            if ((dy < 0 ? -dy : dy) > CAMERA_FOLLOW_HEIGHT_SNAP)
                dy /= 4;
            camera->y.fixed += dy;
        }
    }
    camera->scriptCursor++;
    return 1;
}

// SC_CAMERA_IN_BOUNDS: as SC_CAMERA, clamped to the map alone, without its camera boxes.
Sint32 fdCameraFollowWatchedInBounds(ACTOR *camera)
{
    ACTOR     *leader;
    MAP_STATE *bounds;
    FIXED      minX;
    FIXED      minZ;
    FIXED      maxX;
    FIXED      maxZ;
    FIXED      x;
    FIXED      y;
    FIXED      z;
    Sint32     dz;
    Sint32     dy;
    Sint32     dx;
    FIXED      distance;
    FIXED      moveStep;

    bounds              = RamHeapSlots->mapGrid;
    leader              = (ACTOR *)camera->watchedActor;
    minX                = bounds->minX + FIELD_VIEW_FOCUS_X;
    minZ                = bounds->minZ + FIELD_VIEW_FOCUS_Y;
    maxX                = bounds->maxX - FIELD_VIEW_FOCUS_X;
    maxZ                = bounds->maxZ - (FIELD_VIEW_HEIGHT - FIELD_VIEW_FOCUS_Y);
    camera->motionFlags = 0;
    if (leader != NULL && leader->script != NULL)
    {
        x               = leader->x.fixed;
        y               = leader->y.fixed;
        z               = leader->z.fixed;
        camera->targetX = ACTOR_NO_TARGET;
        camera->targetY = ACTOR_NO_TARGET;
        camera->targetZ = ACTOR_NO_TARGET;
        if (x < minX)
            x = minX;
        if (z < minZ)
            z = minZ;
        if (x > maxX)
            x = maxX;
        if (z > maxZ)
            z = maxZ;
        if (camera->timer != 0)
        {
            SET_ACTOR_POSITION(camera, x, y, z);
        }
        else
        {
            dx       = (x - camera->x.fixed) / toFIXED(1);
            // LEFTOVER(template): this whole-pixel dy, overwritten below and
            // * not used for the distance; Camelot's template: the sibling
            // * routine fdActorSetMoveTarget (field_actor_motion.c) computes the
            // * same three whole-pixel deltas and uses y's in its distance; as
            // * the line removed.
            dy       = (y - camera->y.fixed) / toFIXED(1);
            dz       = (z - camera->z.fixed) / toFIXED(1);
            distance = toFIXED(sySquartIWRAM(dx * dx + dz * dz));
            dx       = x - camera->x.fixed;
            dy       = y - camera->y.fixed;
            dz       = z - camera->z.fixed;
            if (distance < CAMERA_FOLLOW_FINE_DISTANCE)
                distance = sySquartFX(syMulFX(dx, dx) + syMulFX(dz, dz));
            if ((moveStep = distance / 8) > camera->speedLimit)
                moveStep = camera->speedLimit;
            if (distance < CAMERA_FOLLOW_SNAP_DISTANCE)
            {
                camera->x.fixed = x;
                camera->z.fixed = z;
            }
            else
            {
                if (distance > moveStep)
                {
                    dx = syMulFX(syDivFX(distance, dx), moveStep);
                    dz = syMulFX(syDivFX(distance, dz), moveStep);
                }
                camera->x.fixed += dx;
                camera->z.fixed += dz;
            }
            if ((dy < 0 ? -dy : dy) > CAMERA_FOLLOW_HEIGHT_SNAP)
                dy /= 4;
            camera->y.fixed += dy;
        }
    }
    camera->scriptCursor++;
    return 1;
}

// SC_APPROACH_WATCHED: follows the watched actor at its speed. Further than
// SCRIPT_APPROACH_DISTANCE away it walks to that distance short of it and runs on; nearer, it
// stands and waits for the next tick.
Bool fdActorApproachWatchedActor(ACTOR *actor)
{
    Sint32 distancePixels;
    FIXED  moveZ;
    FIXED  dx;
    FIXED  dz;
    Sint32 deltaZPixels;
    Sint32 deltaXPixels;
    Sint32 remainingDistance;
    FIXED  moveX;
    ACTOR *watchedObject;

    watchedObject       = (ACTOR *)actor->watchedActor;
    actor->speedLimit   = watchedObject->speedLimit;
    actor->acceleration = watchedObject->acceleration;
    dx                  = watchedObject->x.fixed - actor->x.fixed;
    dz                  = watchedObject->z.fixed - actor->z.fixed;
    deltaXPixels        = FIXEDtoINT(dx);
    deltaZPixels        = FIXEDtoINT(dz);
    distancePixels = sySquartIWRAM((deltaXPixels * deltaXPixels) + (deltaZPixels * deltaZPixels));
    if (distancePixels > SCRIPT_APPROACH_DISTANCE)
    {
        remainingDistance = distancePixels - SCRIPT_APPROACH_DISTANCE;
        moveX             = dx * remainingDistance / distancePixels;
        moveZ             = dz * remainingDistance / distancePixels;
        fdActorSetMoveTarget(actor, actor->x.fixed + moveX, actor->y.fixed, actor->z.fixed + moveZ);
        fdActorSelectAnimation(actor, ANIM_WALK);
        actor->scriptCursor = (actor->scriptCursor + 1);
        return TRUE;
    }
    fdActorSelectAnimation(actor, ANIM_STAND);
    return FALSE;
}

// SC_WANDER(base, range, limit): walks to a random point base to base + range away, within
// SCRIPT_WANDER_SIDE_ANGLE either side of its facing, that no solid actor or wall blocks, with
// clear ground SCRIPT_WANDER_PROBE_DISTANCE further at SCRIPT_WANDER_DIAGONAL_ANGLE either side,
// and within limit pixels of home. After SCRIPT_WANDER_TRIES failed tries it turns around and waits
// for the next tick.
Bool fdActorWander(ACTOR *actor)
{
    POS           candidatePos;
    POS           probePos;
    Sint32        minimumDistance;
    const Sint32 *operands;
    Sint32        distanceRange;
    Sint32        homeRadiusSquared;
    FIXED         travelDistance;
    Sint32        heading;
    Sint32        attempts;
    Sint32        dx;
    Sint32        dz;

    operands          = &actor->script[actor->scriptCursor + 1];
    minimumDistance   = *operands++;
    distanceRange     = *operands++;
    homeRadiusSquared = *operands / toFIXED(1);
    attempts          = 0;
    homeRadiusSquared = homeRadiusSquared * homeRadiusSquared;
    for (;;)
    {
        attempts++;
        if (attempts <= SCRIPT_WANDER_TRIES)
        {
            SET_POSITION(&candidatePos, actor->x.fixed, actor->y.fixed, actor->z.fixed);
            travelDistance = minimumDistance + syMulFX(syRandom(), distanceRange);
            heading        = actor->facing + (syRandom() >> 2) - (syRandom() >> 2);
            syVectorAddPolarOffset(travelDistance, heading, &candidatePos);
            if (fdActorScriptCheckOverlap(actor, (Sint32 *)&candidatePos) != 0
                || fdMapGridCheckCollision(actor, &candidatePos) != 0)
                continue;
            travelDistance += SCRIPT_WANDER_PROBE_DISTANCE;
            SET_POSITION(&probePos, actor->x.fixed, actor->y.fixed, actor->z.fixed);
            // BUG: the probe straight ahead is placed but never tested, so only the two at
            // SCRIPT_WANDER_DIAGONAL_ANGLE need clear ground; SC_WANDER_HOME tests all three.
            syVectorAddPolarOffset(travelDistance, heading, &probePos);
            SET_POSITION(&probePos, actor->x.fixed, actor->y.fixed, actor->z.fixed);
            syVectorAddPolarOffset(
                travelDistance, heading + SCRIPT_WANDER_DIAGONAL_ANGLE, &probePos);
            if (fdMapGridCheckCollision(actor, &probePos) != 0)
                continue;
            SET_POSITION(&probePos, actor->x.fixed, actor->y.fixed, actor->z.fixed);
            syVectorAddPolarOffset(
                travelDistance, heading - SCRIPT_WANDER_DIAGONAL_ANGLE, &probePos);
            if (fdMapGridCheckCollision(actor, &probePos) != 0)
                continue;
            // FAKEMATCH(dead): the offsets in both arms of a test of actor; as
            // * the two lines alone.
            if (actor != 0)
            {
                dx = candidatePos.x / toFIXED(1) - actor->homeX;
                dz = candidatePos.z / toFIXED(1) - actor->homeZ;
            }
            else
            {
                dx = candidatePos.x / toFIXED(1) - actor->homeX;
                dz = candidatePos.z / toFIXED(1) - actor->homeZ;
            }
            if (dx * dx + dz * dz > homeRadiusSquared)
                continue;
        }
        else
        {
            // Out of tries: face the other way and stay on this command, to try again next tick.
            actor->facing += SCRIPT_HALF_TURN;
            actor->wait    = 1;
            return FALSE;
        }
        fdActorSetMoveTarget(actor, candidatePos.x, candidatePos.y, candidatePos.z);
        actor->scriptCursor += 4;
        return TRUE;
    }
}

// SC_WANDER_HOME(base, range, limit): wanders as SC_WANDER does, testing more ground around the
// point, while it is within limit pixels of home; outside that, or when no point works, it heads
// back towards home.
Sint32 fdActorScriptWanderNearHome(ACTOR *actor)
{
    POS           candidatePosition;
    POS           probePosition;
    Sint32        minimumDistance;
    const Sint32 *operands;
    Sint32        distanceRange;
    Sint32        homeRadiusSquared;
    FIXED         travelDistance;
    Uint16        heading;
    Uint16        angle;
    Sint32        attempts;
    Sint32        dx;
    Sint32        dz;

    operands           = &actor->script[actor->scriptCursor + 1];
    minimumDistance    = *operands++;
    distanceRange      = *operands++;
    homeRadiusSquared  = *operands / toFIXED(1);
    angle              = actor->facing;
    homeRadiusSquared *= homeRadiusSquared;
    attempts           = 0;
    dx                 = actor->x.fixed / toFIXED(1) - actor->homeX;
    dz                 = actor->z.fixed / toFIXED(1) - actor->homeZ;
    if (dx * dx + dz * dz <= homeRadiusSquared)
    {
        for (;;)
        {
            attempts++;
            if (attempts <= SCRIPT_WANDER_TRIES)
            {
                travelDistance = minimumDistance + syMulFX(syRandom(), distanceRange);
                heading        = angle + (syRandom() >> 2) - (syRandom() >> 2);
                SET_POSITION(&candidatePosition, actor->x.fixed, actor->y.fixed, actor->z.fixed);
                syVectorAddPolarOffset(SCRIPT_WANDER_PROBE_DISTANCE, heading, &candidatePosition);
                if (fdActorScriptCheckOverlap(actor, (Sint32 *)&candidatePosition) != 0)
                    continue;
                SET_POSITION(&candidatePosition, actor->x.fixed, actor->y.fixed, actor->z.fixed);
                syVectorAddPolarOffset(travelDistance, heading, &candidatePosition);
                if (fdMapGridCheckCollision(actor, &candidatePosition) != 0)
                    continue;
                probePosition.x  = actor->x.fixed;
                probePosition.y  = actor->y.fixed;
                travelDistance  += SCRIPT_WANDER_PROBE_DISTANCE;
                probePosition.z  = actor->z.fixed;
                syVectorAddPolarOffset(travelDistance, heading, &probePosition);
                if (fdMapGridCheckCollision(actor, &probePosition) != 0)
                    continue;
                SET_POSITION(&probePosition, actor->x.fixed, actor->y.fixed, actor->z.fixed);
                syVectorAddPolarOffset(
                    travelDistance, heading + SCRIPT_WANDER_DIAGONAL_ANGLE, &probePosition);
                if (fdMapGridCheckCollision(actor, &probePosition) != 0)
                    continue;
                SET_POSITION(&probePosition, actor->x.fixed, actor->y.fixed, actor->z.fixed);
                syVectorAddPolarOffset(
                    travelDistance, heading - SCRIPT_WANDER_DIAGONAL_ANGLE, &probePosition);
                if (fdMapGridCheckCollision(actor, &probePosition) != 0)
                    continue;
                SET_POSITION(&probePosition, actor->x.fixed, actor->y.fixed, actor->z.fixed);
                syVectorAddPolarOffset(
                    travelDistance, heading + SCRIPT_WANDER_SIDE_ANGLE, &probePosition);
                if (fdMapGridCheckCollision(actor, &probePosition) != 0)
                    continue;
                SET_POSITION(&probePosition, actor->x.fixed, actor->y.fixed, actor->z.fixed);
                syVectorAddPolarOffset(
                    travelDistance, heading - SCRIPT_WANDER_SIDE_ANGLE, &probePosition);
                if (fdMapGridCheckCollision(actor, &probePosition) != 0)
                    continue;
                dx = candidatePosition.x / toFIXED(1) - actor->homeX;
                dz = candidatePosition.z / toFIXED(1) - actor->homeZ;
                if (dx * dx + dz * dz > homeRadiusSquared)
                    continue;
                actor->collisionFlags |= ACTOR_PUSHABLE;
                fdActorSetMoveTarget(
                    actor, candidatePosition.x, candidatePosition.y, candidatePosition.z);
                goto done;
            }
            break;
        }
    }
    attempts = 0;
    angle    = (Uint16)syAtan(dz, dx) + SCRIPT_HALF_TURN;
    for (;;)
    {
        attempts++;
        if (attempts <= SCRIPT_WANDER_TRIES)
        {
            travelDistance = minimumDistance + syMulFX(syRandom(), distanceRange);
            heading        = angle + (syRandom() >> 2) - (syRandom() >> 2);
            SET_POSITION(&candidatePosition, actor->x.fixed, actor->y.fixed, actor->z.fixed);
            syVectorAddPolarOffset(SCRIPT_WANDER_PROBE_DISTANCE, heading, &candidatePosition);
            if (fdActorScriptCheckOverlap(actor, (Sint32 *)&candidatePosition) != 0)
                continue;
            SET_POSITION(&candidatePosition, actor->x.fixed, actor->y.fixed, actor->z.fixed);
            syVectorAddPolarOffset(travelDistance, heading, &candidatePosition);
            if (fdMapGridCheckCollision(actor, &candidatePosition) != 0)
                continue;
            actor->collisionFlags &= ~ACTOR_PUSHABLE;
            fdActorSetMoveTarget(
                actor, candidatePosition.x, candidatePosition.y, candidatePosition.z);
        }
        break;
    }
done:
    actor->scriptCursor += 4;
    return 1;
}

// What a work value handler does with its value, for SC_SET, SC_ADD and SC_CMP in turn.
typedef enum SCRIPT_OPERAND_OPERATION
{
    SCRIPT_OPERAND_SET,
    SCRIPT_OPERAND_ADD,
    SCRIPT_OPERAND_COMPARE // sets conditionResult to 1 when equal, else 0
} SCRIPT_OPERAND_OPERATION;

// Defines the handler for a 32-bit work value: set it, add to it, or compare it (into
// conditionResult).
#define SCRIPT_DEFINE_OPERAND_WORD(name, field)                                                    \
    void name(SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value)                           \
    {                                                                                              \
        if (operation == SCRIPT_OPERAND_SET)                                                       \
        {                                                                                          \
            operands->field = value;                                                               \
            return;                                                                                \
        }                                                                                          \
        if (operation == SCRIPT_OPERAND_ADD)                                                       \
        {                                                                                          \
            operands->field += value;                                                              \
            return;                                                                                \
        }                                                                                          \
        operands->conditionResult = operands->field == value;                                      \
    }

// The same for an address: an add steps by value words (value * 4 bytes).
#define SCRIPT_DEFINE_OPERAND_ADDRESS(name, field)                                                 \
    void name(SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value)                           \
    {                                                                                              \
        if (operation == SCRIPT_OPERAND_SET)                                                       \
        {                                                                                          \
            operands->field = value;                                                               \
            return;                                                                                \
        }                                                                                          \
        if (operation == SCRIPT_OPERAND_ADD)                                                       \
        {                                                                                          \
            operands->field += value * 4;                                                          \
            return;                                                                                \
        }                                                                                          \
        operands->conditionResult = operands->field == value;                                      \
    }

// The same for an 8- or 16-bit value: a compare narrows value to its type.
#define SCRIPT_DEFINE_OPERAND_NARROW(name, field, type)                                            \
    void name(SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value)                           \
    {                                                                                              \
        if (operation == SCRIPT_OPERAND_SET)                                                       \
        {                                                                                          \
            operands->field = value;                                                               \
        }                                                                                          \
        else if (operation == SCRIPT_OPERAND_ADD)                                                  \
        {                                                                                          \
            operands->field += value;                                                              \
        }                                                                                          \
        else                                                                                       \
        {                                                                                          \
            operands->conditionResult = operands->field == (type)value;                            \
        }                                                                                          \
    }

// The work value handlers, one for each entry of ACTOR_SCRIPT_VALUE (see
// ActorScriptOperandHandlerTable in map_grid_data.c); each names the actor field it sets, adds to
// or compares.
SCRIPT_DEFINE_OPERAND_ADDRESS(fdActorScriptSetOrCompareScriptAddress, script)

// Work value SW_CURSOR: sets, adds to or compares the script cursor.
void fdActorScriptSetOrCompareCursor(SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value)
{
    if (operation == SCRIPT_OPERAND_SET)
    {
        operands->cursor = value;
    }
    else if (operation == SCRIPT_OPERAND_ADD)
    {
        operands->cursor = ((Sint16)operands->cursor + value);
    }
    else
    {
        operands->conditionResult = (Sint16)operands->cursor == (Sint16)value;
    }
}

SCRIPT_DEFINE_OPERAND_NARROW(fdActorScriptSetOrCompareFacing, facing, Uint16)
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareX, x)
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareY, y)
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareZ, z)

// Work value SW_RADIUS: sets, adds to or compares the actor's radius.
void fdActorScriptSetOrCompareRadius(SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value)
{
    if (operation == SCRIPT_OPERAND_SET)
    {
        state->radius = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD)
    {
        state->radius = (state->radius + value);
        return;
    }
    state->conditionResult = state->radius == (Sint16)value;
}

SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareScaleX, scaleX)
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareScaleY, scaleY)
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareVelocityX, velocityX)
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareVelocityY, velocityY)
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareVelocityZ, velocityZ)
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareSpeedLimit, speedLimit)
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareAcceleration, acceleration)
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareTargetX, targetX)
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareTargetY, targetY)
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareTargetZ, targetZ)
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareBounce, bounce)
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareGravity, gravity)
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareTerrainHeight, groundHeight)
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareSceneWord, sceneWord)
SCRIPT_DEFINE_OPERAND_ADDRESS(fdActorScriptSetOrCompareSprite, sprite)
SCRIPT_DEFINE_OPERAND_NARROW(fdActorScriptSetOrCompareSpriteStorageKind, spriteStorageKind, Uint8)
SCRIPT_DEFINE_OPERAND_NARROW(fdActorScriptSetOrCompareMotionFlags, motionFlags, Uint8)
SCRIPT_DEFINE_OPERAND_NARROW(fdActorScriptSetOrCompareArrivalAxis, arrivalAxis, Uint8)
SCRIPT_DEFINE_OPERAND_NARROW(fdActorScriptSetOrCompareComparisonResult, conditionResult, Uint8)
SCRIPT_DEFINE_OPERAND_NARROW(fdActorScriptSetOrCompareSnapToTarget, snapToTarget, Uint8)
SCRIPT_DEFINE_OPERAND_NARROW(fdActorScriptSetOrCompareCollisionFlags, collisionFlags, Uint8)
SCRIPT_DEFINE_OPERAND_NARROW(fdActorScriptSetOrCompareSteeringFlags, steeringFlags, Uint8)
SCRIPT_DEFINE_OPERAND_NARROW(fdActorScriptSetOrComparePaused, paused, Uint8)
SCRIPT_DEFINE_OPERAND_NARROW(fdActorScriptSetOrCompareRepeatCount, repeatCount, Uint8)

// Work value SW_WAIT: sets, adds to or compares the wait counter.
SCRIPT_DEFINE_OPERAND_NARROW(fdActorScriptSetOrCompareWait, wait, Sint16)

SCRIPT_DEFINE_OPERAND_NARROW(fdActorScriptSetOrCompareTimer, timer, Sint16)
SCRIPT_DEFINE_OPERAND_NARROW(fdActorScriptSetOrCompareUserValue, userValue, Sint16)
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareWatchedActor, watchedActor)
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareUpdateAddress, update)
SCRIPT_DEFINE_OPERAND_NARROW(fdActorScriptSetOrCompareSceneCounter, sceneCount, Uint8)
SCRIPT_DEFINE_OPERAND_NARROW(fdActorScriptSetOrCompareSceneByte, sceneByte, Uint8)

// Reads the two words after the command (a work value number and a value) and passes operation to
// that value's handler.
static __inline__ Sint32 fdActorScriptApplyOperandCommand(SCRIPT_OPERANDS *operands,
                                                          Sint32           operation)
{
    Sint16        commandCursor = operands->cursor;
    const Sint32 *arguments     = (const Sint32 *)operands->script + commandCursor + 1;
    OPERAND_FUNC  handler       = ActorScriptOperandHandlerTable[arguments[0]];

    if (handler != 0)
        handler(operands, operation, arguments[1]);
    operands->cursor += 3;
    return 1;
}

// SC_SET(w, v): sets work value w to v.
Sint32 fdActorScriptApplyOperandSet(ACTOR *interpreter)
{
    SCRIPT_OPERANDS *operands = (SCRIPT_OPERANDS *)interpreter;

    return fdActorScriptApplyOperandCommand(operands, SCRIPT_OPERAND_SET);
}

// SC_ADD(w, v): adds v to work value w.
Sint32 fdActorScriptApplyOperandAdd(ACTOR *interpreter)
{
    SCRIPT_OPERANDS *operands = (SCRIPT_OPERANDS *)interpreter;

    return fdActorScriptApplyOperandCommand(operands, SCRIPT_OPERAND_ADD);
}

// SC_CMP(w, v): tests whether work value w equals v.
Sint32 fdActorScriptApplyOperandCompare(ACTOR *interpreter)
{
    SCRIPT_OPERANDS *operands = (SCRIPT_OPERANDS *)interpreter;

    return fdActorScriptApplyOperandCommand(operands, SCRIPT_OPERAND_COMPARE);
}

// Unused: sets an actor's update function, when there is an actor.
UNUSED void fdActorSetUpdate(ACTOR *actor, void (*update)(void))
{
    if (actor != NULL)
        actor->update = update;
}

// The heading of each direction-pad combination, -1 for none.
extern const Sint16 WorldMapKeyHeadings[16];

// SC_HOVER: the leader hovering (the Hover Psynergy), moved by the direction pad over what lies
// below.
Bool                fdActorUpdateHoverControl(ACTOR *actor)
{
    POS    pos;
    POS    testPos;
    POS    contact;
    Sint16 angleOffsets[22];
    Sint32 collision;
    Sint16 heading;
    Sint32 angle;
    Sint32 movementAnimation;
    Uint16 direction;
    Sint32 angleIndex;
    Sint32 headingCount;
    ACTOR *other;
    Bool   pressed;

    movementAnimation  = ANIM_WALK;
    collision          = 0;
    actor->motionFlags = ACTOR_FLOATS;
    // Hovering, the dash button changes only the animation, not the speed.
    if (PerWork.data & MainRoutineStateWork.dashKeys)
    {
        actor->speedLimit   = toFIXED(1) / 2;
        actor->acceleration = toFIXED(1) / 8;
        movementAnimation   = ANIM_LEADER_RUN;
    }
    else
    {
        actor->speedLimit   = toFIXED(1) / 2;
        actor->acceleration = toFIXED(1) / 8;
    }

    angle = WorldMapKeyHeadings[DPAD_INDEX(PerWork.data)];
    if ((Uint16)angle == WORLD_MAP_KEY_HEADING_NONE)
    {
        collision |= PLAYER_STEP_STOPPED;
    }
    else
    {
        collision = 0;
        SET_POSITION(&pos, actor->x.fixed, actor->y.fixed, actor->z.fixed);
        syVectorAddPolarOffset(toFIXED(8), (Uint16)angle, &pos);
        if (DebugMode != 0 && (PerWork.data & KEY_L))
        {
            heading = angle;
        }
        else
        {
            if (fdMapGridCheckCollisionAbove(actor, &pos) == 0)
            {
                SET_POSITION(&testPos, actor->x.fixed, actor->y.fixed, actor->z.fixed);
                syVectorAddPolarOffset(toFIXED(8), (Uint16)angle + FIELD_PROBE_ANGLE, &testPos);
                if (fdMapGridCheckCollisionAbove(actor, &testPos) == 0)
                {
                    SET_POSITION(&testPos, actor->x.fixed, actor->y.fixed, actor->z.fixed);
                    syVectorAddPolarOffset(toFIXED(8), (Uint16)angle - FIELD_PROBE_ANGLE, &testPos);
                    if (fdMapGridCheckCollisionAbove(actor, &testPos) == 0)
                    {
                        SET_POSITION(&testPos, actor->x.fixed, actor->y.fixed, actor->z.fixed);
                        syVectorAddPolarOffset(toFIXED(8), (Uint16)angle + DEGtoANG(45), &testPos);
                        if (fdMapGridCheckCollisionAbove(actor, &testPos) == 0)
                        {
                            SET_POSITION(&testPos, actor->x.fixed, actor->y.fixed, actor->z.fixed);
                            syVectorAddPolarOffset(
                                toFIXED(8), (Uint16)angle - DEGtoANG(45), &testPos);
                            if (fdMapGridCheckCollisionAbove(actor, &testPos) == 0)
                            {
                                heading = angle;
                                goto check_objects;
                            }
                        }
                    }
                }
            }
            direction                    = angle;
            headingCount                 = 0;
            angleOffsets[headingCount++] = direction + FIELD_PROBE_ANGLE;
            angleOffsets[headingCount++] = direction - FIELD_PROBE_ANGLE;
            angleOffsets[headingCount++] = direction + DEGtoANG(45);
            angleOffsets[headingCount++] = direction - DEGtoANG(45);
            angleOffsets[headingCount++] = direction + FIELD_PROBE_ANGLE * 3;
            angleOffsets[headingCount++] = direction - FIELD_PROBE_ANGLE * 3;
            for (angleIndex = 0; angleIndex < headingCount; angleIndex++)
            {
                heading = angleOffsets[angleIndex];
                SET_POSITION(&pos, actor->x.fixed, actor->y.fixed, actor->z.fixed);
                syVectorAddPolarOffset(toFIXED(8), (Uint16)heading, &pos);
                if (fdMapGridCheckCollisionAbove(actor, &pos) != 0)
                    continue;
                SET_POSITION(&testPos, actor->x.fixed, actor->y.fixed, actor->z.fixed);
                syVectorAddPolarOffset(toFIXED(8), (Uint16)heading + FIELD_PROBE_ANGLE, &testPos);
                if (fdMapGridCheckCollisionAbove(actor, &testPos) != 0)
                    continue;
                SET_POSITION(&testPos, actor->x.fixed, actor->y.fixed, actor->z.fixed);
                syVectorAddPolarOffset(toFIXED(8), (Uint16)heading - FIELD_PROBE_ANGLE, &testPos);
                if (fdMapGridCheckCollisionAbove(actor, &testPos) != 0)
                    continue;
                SET_POSITION(&testPos, actor->x.fixed, actor->y.fixed, actor->z.fixed);
                syVectorAddPolarOffset(toFIXED(8), (Uint16)heading + DEGtoANG(45), &testPos);
                if (fdMapGridCheckCollisionAbove(actor, &testPos) != 0)
                    continue;
                SET_POSITION(&testPos, actor->x.fixed, actor->y.fixed, actor->z.fixed);
                syVectorAddPolarOffset(toFIXED(8), (Uint16)heading - DEGtoANG(45), &testPos);
                if (fdMapGridCheckCollisionAbove(actor, &testPos) == 0)
                    goto check_objects;
            }
            SET_POSITION(&pos, actor->x.fixed, actor->y.fixed, actor->z.fixed);
            collision |= PLAYER_STEP_BLOCKED_BY_MAP;

        check_objects:
            SET_POSITION(&contact, actor->x.fixed, actor->y.fixed, actor->z.fixed);
            syVectorAddPolarOffset(toFIXED(4), (Uint16)heading, &contact);
            other = RamHeapSlots->actors;
            for (angleIndex = 0; angleIndex < ACTOR_COUNT; angleIndex++, other++)
            {
                Sint32 radius;

                radius = actor->radius - 2;
                if (other->script == 0)
                    continue;
                if ((other->collisionFlags & ACTOR_SOLID) == 0)
                    continue;
                if (other == actor)
                    continue;
                if (fdCheckSpheresOverlap(&other->x.fixed, radius, &contact.x, other->radius - 2)
                    < 0)
                    continue;
                collision |= PLAYER_STEP_BLOCKED_BY_ACTOR;
            }
        }
    }

    if (RamHeapSlots->fieldState != NULL)
    {
        FIELD_STATE *state = RamHeapSlots->fieldState;

        if (collision & PLAYER_STEP_BLOCKED)
            state->blockedSteps++;
        else
            state->blockedSteps = 0;
    }
    if (collision != 0)
    {
        Sint32 action;

        action = ANIM_LEADER_LOOK_AROUND;
        if (syUnitGet(MainRoutineStateWork.leaderActorId)->hp == 0)
            action = ANIM_LEADER_LEAN_BACK;
        fdActorSelectAnimation(actor, action);
    }
    else
    {
        fdActorSelectAnimation(actor, movementAnimation);
    }

    if (collision != 0)
    {
        actor->targetX = ACTOR_NO_TARGET;
        actor->targetY = ACTOR_NO_TARGET;
        actor->targetZ = ACTOR_NO_TARGET;
        if (collision & PLAYER_STEP_BLOCKED)
        {
            Sint32 difference;

            difference = (Uint16)angle - actor->facing;
            difference = (Sint16)difference;
            if (difference > FIELD_BLOCKED_TURN_MAX)
                difference = FIELD_BLOCKED_TURN_MAX;
            if (difference < -FIELD_BLOCKED_TURN_MAX)
                difference = -FIELD_BLOCKED_TURN_MAX;
            actor->facing += difference;
        }
        actor->timer     = 0;
        actor->userValue = FOOTPRINT_BOTH_FEET;
    }
    else
    {
        fdActorSetMoveTarget(actor, pos.x, pos.y, pos.z);
        if (actor->timer != 0)
            actor->timer--;
    }

    pressed = FALSE;
    if (RamHeapSlots->fieldState == NULL)
        return FALSE;
    if ((PerWork.push & MainRoutineStateWork.selectKeys)
        || (PerWork.push & MainRoutineStateWork.aKeys)
        || (PerWork.push & MainRoutineStateWork.startKeys)
        || (PerWork.push & MainRoutineStateWork.firstShortcutKeys)
        || (PerWork.push & MainRoutineStateWork.secondShortcutKeys))
        pressed = TRUE;
    // While hovering the menus and shortcuts are refused with a buzz.
    if (pressed)
    {
        sySoundPlayStub(SE_MENU_CANCEL);
    }
    else
    {
        actor->groundHeight =
            fdMapGridGetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);
        actor->scriptCursor++;
    }
    return TRUE;
}

#undef DISTANCE
#undef SCRIPT_LABEL_KEY_MASK
