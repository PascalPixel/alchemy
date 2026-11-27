// An actor's screen position, the encounter transition, and the leader leaving through an exit and
// arriving. Part of script_commands.c.

#include "script_commands_internal.h"

// Finds where an actor is on screen: position[0] and position[1] receive its x and y in pixels, y
// at the top of its sprite.
Sint32 fdActorGetScreenPos(Sint32 actorId, Sint32 *pos)
{
    ACTOR                 *actor = fdActorLookup(actorId);
    MAP_GRID_SCROLL_STATE *grid;
    Sint32                 cameraX;
    Sint32                 cameraZ;
    FIXED                  x;
    FIXED                  z;

    if (actor == 0)
        return -1;
    grid    = MapGridState[0];
    cameraX = syMapGridScrollStateView(grid)[0] & FIELD_PIXEL_MASK;
    cameraZ = syMapGridScrollStateView(grid)[1] & FIELD_PIXEL_MASK;
    x       = actor->x.fixed - cameraX;
    z       = actor->z.fixed - cameraZ - actor->y.fixed;
    *pos++  = x / toFIXED(1);
    *pos    = z / toFIXED(1);
    if ((actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE)
        *pos -= syResourceGetMetadataRecordStub(actor->sprite->entries[0]->animationId)->spriteTop;
    return 0;
}

UNUSED void fdExitNoOperation(void)
{
}

// The encounter's screen closes over ENCOUNTER_CLOSE_TICKS, and its white flash then fades to black
// over ENCOUNTER_FADE_TICKS, ENCOUNTER_FADE_STEP levels a tick from just below RGB_LEVEL_MAX.
#define ENCOUNTER_CLOSE_TICKS 16
#define ENCOUNTER_FADE_TICKS  16
#define ENCOUNTER_FADE_STEP   2

// Starts the transition into a random battle: plays the scene's battle music and the encounter
// sounds, closes the screen and flashes the backdrop white, fading it to black.
UNREAD_RESULT fdStartEncounterTransition(Sint32 unused)
{
    FIELD_STATE *state = FieldStateWork;
    Sint32       i;
    Sint32       level;

    sySoundPlayStub(MainRoutineStateWork.battleMusic);
    sySoundPlayStub(SE_STOP_ACTION_SE);
    sySoundPlayStub(SE_BEAST_CRY);
    if (state->mode == ACTOR_SYSTEM_WORLD_MAP)
    {
        // On the World Map the flash colours the window palette's black instead, and the screen
        // closes toward the centre line from its middle (fdScreenTransitionUpdateFromCenter); a
        // field scene closes with WINDOW_TRANSITION_LEADER_INVERSE.
        IO_WRITE16(RGB_WHITE, (Uint16 *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BLACK_COLOR));
        fdScreenTransitionFinish(SCENE_TRANSITION(TRANSITION_CENTER_LINE, CENTER_LINE_FLICKER),
                                 ENCOUNTER_CLOSE_TICKS);
        state->transitionActive = 0;
        sySynch(ENCOUNTER_CLOSE_TICKS);
        for (i = 0; i < ENCOUNTER_FADE_TICKS; i++)
        {
            level = RGB_LEVEL_MAX - 1 - i * ENCOUNTER_FADE_STEP;
            IO_WRITE16(RGB(level, level, level),
                       (Uint16 *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BLACK_COLOR));
            sySynch(1);
        }
    }
    else
    {
        IO_WRITE16(RGB_WHITE, &PLTT_BACKDROP);
        fdScreenTransitionFinish(
            SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER_INVERSE),
            ENCOUNTER_CLOSE_TICKS);
        state->transitionActive = 0;
        sySynch(ENCOUNTER_CLOSE_TICKS);
        for (i = 0; i < ENCOUNTER_FADE_TICKS; i++)
        {
            level = RGB_LEVEL_MAX - 1 - i * ENCOUNTER_FADE_STEP;
            IO_WRITE16(RGB(level, level, level), &PLTT_BACKDROP);
            sySynch(1);
        }
    }
}

// Starts the leader's animation for leaving through an exit: centres it on its metatile, at the
// metatile's south edge, stops it and plays animation over 18 ticks.
void fdExitPrepareAnimation(Sint32 animation)
{
    ACTOR         *actor;
    SPR           *sprite;
    SPR_ANIMATION *doorway;

    actor   = fdActorLookup(MainRoutineStateWork.leaderActorId);
    sprite  = actor->sprite;
    doorway = (SPR_ANIMATION *)fdSpriteAddAnimationStub(sprite, SPRITE_DOORWAY);
    {
        Uint32 cleared     = 0;
        Uint8  hidden      = SPRITE_COLOR_HIDDEN;

        // No shadow as the leader leaves.
        sprite->flags      = cleared;
        doorway->colorMode = hidden;
        // Keep the metatile and put the leader at its middle in x and its south edge in z.
        actor->x.fixed     = (actor->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
        actor->z.fixed     = (actor->z.fixed & FIELD_METATILE_MASK) + toFIXED(16);
        actor->velocityX   = cleared;
        actor->velocityZ   = cleared;
        actor->targetX     = ACTOR_NO_TARGET;
        actor->targetZ     = ACTOR_NO_TARGET;
        fdActorSelectAnimationStub(actor, animation);
        sySynch(18);
    }
}

// Leaves through an EXIT_STYLE_STAIRS_UP exit (ANIM_LEADER_LEAVE); FLAG_ARRIVAL_STAIRS_UP makes the
// next scene play the matching arrival (fdEntrancePlayArrival).
void fdExitStartStairsUp(void)
{
    fdExitPrepareAnimation(ANIM_LEADER_LEAVE);
    gmFlagSetStub(FLAG_ARRIVAL_STAIRS_UP);
}

// Leaves through an EXIT_STYLE_STAIRS_DOWN exit (ANIM_LEADER_LEAVE_ALTERNATE), recorded in
// FLAG_ARRIVAL_STAIRS_DOWN.
void fdExitStartStairsDown(void)
{
    fdExitPrepareAnimation(ANIM_LEADER_LEAVE_ALTERNATE);
    gmFlagSetStub(FLAG_ARRIVAL_STAIRS_DOWN);
}

// Plays the leader's arrival animation through an exit: centred on its metatile at the metatile's
// north edge, the leader plays animation for 30 ticks, loses the hidden doorway part and walks 8
// pixels south.
void fdEntrancePlayArrivalAnimation(Sint32 animation)
{
    ACTOR         *leader   = fdActorLookup(MainRoutineStateWork.leaderActorId);
    SPR           *sprite   = leader->sprite;
    SPR_ANIMATION *doorway  = (SPR_ANIMATION *)fdSpriteAddAnimationStub(sprite, SPRITE_DOORWAY);
    sprite->flags           = 0;
    doorway->colorMode      = SPRITE_COLOR_HIDDEN;
    leader->x.fixed         = (leader->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
    leader->z.fixed        &= FIELD_METATILE_MASK;
    fdActorSelectAnimationStub(leader, animation);
    sySynch(30);
    // Back to one sprite part, with its shadow, walking 8 pixels south at 1 pixel a tick.
    sprite->count = 1;
    fdSpriteAnimationClearStub(sprite->entries[1]);
    sprite->entries[1]   = NULL;
    sprite->flags        = SPRITE_FLAG_SHADOW;
    leader->acceleration = toFIXED(1);
    leader->speedLimit   = toFIXED(1);
    fdActorSetMoveTargetStub(
        leader, leader->x.fixed, leader->y.fixed, leader->z.fixed + toFIXED(8));
    fdActorWaitForMoveStub(leader);
}

// Plays the leader's arrival for the way the last scene was left: through a EXIT_STYLE_STAIRS_UP
// exit (FLAG_ARRIVAL_STAIRS_UP, ANIM_LEADER_ARRIVE, returns 1) or a EXIT_STYLE_STAIRS_DOWN one
// (FLAG_ARRIVAL_STAIRS_DOWN, ANIM_LEADER_ARRIVE_ALTERNATE, returns 2), or falling in from 160
// pixels above (FLAG_ARRIVE_FALLING, returns 3).
Sint32 fdEntrancePlayArrival(void)
{
    Sint32 result = 0;
    Sint32 flag   = FLAG_ARRIVAL_STAIRS_UP;

    if (gmFlagGetStub(flag) != 0)
    {
        fdEntrancePlayArrivalAnimation(ANIM_LEADER_ARRIVE);
        gmFlagClearStub(flag);
        result = 1;
    }
    else
    {
        flag = FLAG_ARRIVAL_STAIRS_DOWN;
        if (gmFlagGetStub(flag) != 0)
        {
            fdEntrancePlayArrivalAnimation(ANIM_LEADER_ARRIVE_ALTERNATE);
            gmFlagClearStub(flag);
            result = 2;
        }
        else
        {
            flag = FLAG_ARRIVE_FALLING;
            if (gmFlagGetStub(flag) != 0)
            {
                Sint32 leaderId;
                ACTOR *leader;

                gmFlagClearStub(flag);
                leaderId         = MainRoutineStateWork.leaderActorId;
                leader           = fdActorLookup(leaderId);
                // FLAG_ARRIVE_FALLING is set by scenes the party drops out of (the Suhalla storm, a
                // floor giving way in the Tret tree): start 160 pixels up and let gravity bring the
                // leader down to the ground.
                leader->y.fixed += toFIXED(160);
                fdCameraMoveWithinMapBounds(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
                // Until the next tick's fall would reach the ground.
                while (leader->y.fixed + leader->velocityY > leader->groundHeight)
                {
                    sySynch(1);
                }
                sySoundPlayStub(SE_DOOR_CLOSE);
                leader->y.fixed = leader->groundHeight;
                fdActorSelectAnimationStub(leader, ANIM_LEADER_LEAN_BACK);
                fdScriptWait(15);
                fdCameraFollowActor(leaderId, 1);
                result = 3;
            }
        }
    }
    return result;
}

#undef ENCOUNTER_CLOSE_TICKS
#undef ENCOUNTER_FADE_TICKS
#undef ENCOUNTER_FADE_STEP
