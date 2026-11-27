// The effect scene a field Psynergy runs in: fading to and from its cast colour, its effect slots
// and resources, the actor pair it works on, and Mind Read's wave and palettes. Part of
// script_commands.c.

#include "battle/battle_system.h"
#include "text/msg_ids.h"

// How long fdPsynergyEnd waits at most for the World Map's palette fade.
#define END_FADE_WAIT_TICKS 90

// Fades the palettes back to the saved ones over the given ticks.
void fdBattleEffectFadeToSavedPalettes(Sint32 ticks)
{
    fdStartPaletteTransition(ticks);
}

// Fades the palettes back from a cast's colours to the saved ones over 8 ticks; while Reveal is
// active the BG half fades to Reveal's grey instead (PALETTE_TINT_GRAYSCALE).
void fdPsynergyFadeFromCastColor(void)
{
    fdStartPaletteTransition(8);
}

// Converts a world position (16.16 pixels) to the screen: x and z receive the screen's x and y and
// y becomes 0.
void fdCameraWorldToScreen(VECTOR3 *pos)
{
    HEAP_SLOTS *heap = RamHeapSlots;

    if (((FIELD_STATE *)heap->fieldState)->mode == ACTOR_SYSTEM_WORLD_MAP)
    {
        Sint32 result[3];

        syConvert3Dto2D((const FIXED *)pos, result);
        pos->x = toFIXED(result[0]);
        pos->z = toFIXED(result[1]);
        pos->y = 0;
    }
    else
    {
        MAP_STATE *map  = heap->mapGrid;
        Sint32     x    = map->view[0] & FIELD_PIXEL_MASK;
        Sint32     z    = map->view[1] & FIELD_PIXEL_MASK;

        // Relative to the view's whole-pixel corner; height lifts a point up the screen.
        pos->x         -= x;
        pos->z         -= pos->y;
        pos->z         -= z;
        pos->y          = 0;
    }
}

// The same function as fdEffectUpdateAllSlots (script_commands_djinni.c), under another name. An
// event: updates the scene's 24 effect slots.
void fdBattleEffectUpdateAllEffectSlots(void)
{
    FIELD_EFFECT_STATE *scene;
    Sint32              i;

    scene = RamHeapSlots->fieldEffectState;
    for (i = 0; i < (Sint32)ARRAY_COUNT(scene->particles); i++)
        fdParticleUpdate(&scene->particles[i]);
}

// Prepares the state of the field Psynergy for ability (heap slot fieldEffectState): its kind, the
// free VRAM it starts with, the map's shake to restore, and whether the place is lit. While another
// field Psynergy is resolving, only the ability and kind are set.
void fdPsynergyLoadResources(Sint32 abilityId, Sint32 mode)
{
    FIELD_STATE        *event = RamHeapSlots->fieldState;
    MAP_STATE          *map   = RamHeapSlots->mapGrid;
    FIELD_EFFECT_STATE *state;
    Sint32              running;

    if (event->psynergyActing == 0)
    {
        state = syHeapSlotAllocate(HEAP_SLOT_FIELD_EFFECT, sizeof(*state));
        IWRAM_FILL_WORDS(state, sizeof(*state), 0);
    }
    else
    {
        state = RamHeapSlots->fieldEffectState;
    }
    state->ability       = abilityId;
    state->fieldPsynergy = gmAbilityGetDirectStub(abilityId)->fieldPsynergy;
    running              = event->psynergyActing;
    if (running != 0)
        return;
    state->freeBlocks      = VRAM_BLOCK_COUNT - fdResourceTableCountFreeBlocks();
    state->mode            = mode;
    state->flashCaster     = 1;
    state->aimAhead        = 1;
    state->announceName    = 1;
    state->msgMode         = 1;
    state->msgShown        = 0;
    state->stage           = running;
    state->frame           = -1;
    state->spriteActive    = running;
    state->savedShakeX     = map->shakeX;
    state->savedShakeY     = map->shakeY;
    state->savedShakeDecay = map->shakeDecay;
    if (MainRoutineStateWork.mapId == SCENE_GABOMBA_CATACOMBS_STREAM)
        state->lit = 1;
    if (MainRoutineStateWork.mapId == SCENE_TUNDARIA_TOWER_ENTRANCE)
        state->lit = 1;
    if (MainRoutineStateWork.mapId == SCENE_TUNDARIA_TOWER_CENTER_PRONG)
        state->lit = 1;
    if (MainRoutineStateWork.mapId == SCENE_TUNDARIA_TOWER_BURST_BROOCH)
        state->lit = 1;
    if (MainRoutineStateWork.mapId == SCENE_JUPITER_LIGHTHOUSE_AERIE)
        state->lit = 1;
    if (MainRoutineStateWork.mapId == (Sint32)&SceneIdJupiterLighthouseAerieBeacon)
        state->lit = 1;
    if (MainRoutineStateWork.mapId == SCENE_LOHO)
        state->lit = 1;
    fdPsynergySetupActorPair(MainRoutineStateWork.leaderActorId, -1);
    if (state->fieldPsynergy != FIELD_PSYNERGY_REVEAL)
        event->effectTimerRunning = 0;
    if (state->fieldPsynergy != FIELD_PSYNERGY_RING)
        event->ringTimerRunning = 0;
    event->targetX           = 0;
    event->targetY           = 0;
    event->targetZ           = 0;
    event->blazeLongSpray    = 0;
    event->psynergyHitObject = -1;
    fdBattleEffectStartArcEffect();
    sySetEvent(fdBattleEffectUpdateAllEffectSlots, EVENT_PASS_TICK);
}

// Records the caster and the target of a field Psynergy.
void fdPsynergySetupActorPair(Sint32 userId, Sint32 targetId)
{
    ACTOR              *user;
    ACTOR              *target;
    FIELD_EFFECT_STATE *scene;

    scene           = RamHeapSlots->fieldEffectState;
    scene->casterId = userId;
    user            = fdActorLookup((Sint16)userId);
    scene->targetId = targetId;
    scene->caster   = user;
    target          = fdActorLookup((Sint16)targetId);
    // The facing rounded to four directions, and to eight.
    scene->angle    = (user->facing + DEGtoANG(45)) & TRIG_TURN * 3 / 4;
    scene->angle8   = (user->facing + FACING_STEP) & TRIG_TURN * 7 / 8;
    scene->target   = target;
    if (user != NULL)
    {
        Uint8 priorityFlags;

        priorityFlags              = user->priorityFlags;

        scene->savedPriorityFlags  = priorityFlags;
        scene->savedSpritePriority = ((SPR *)user->sprite)->image.priority;
    }
    if (target != NULL)
    {
        // Save target motion and colors for later restoration.
        scene->savedCallback = target->update;
        scene->savedScript   = target->script;
        scene->savedPalette  = ((SPR *)target->sprite)->entries[0]->colorMode;
        scene->x             = target->x.fixed;
        scene->z             = target->z.fixed;
        scene->y             = target->y.fixed;
    }
    else
    {
        scene->x = user->x.fixed;
        scene->z = user->z.fixed;
        scene->y = user->y.fixed;
        syVectorAddPolarOffset(toFIXED(16), scene->angle, &scene->x);
    }
}

// Starts casting: the caster raises its hand (ANIM_CAST_PSYNERGY_START) and stops, flashes with a
// sound, and the ability's name is announced; with FLAG_PLAY_PSYNERGY_CAST set the caster holds out
// its hand (ANIM_CAST_PSYNERGY_END) and keeps its glow.
void fdPsynergyStartCast(void)
{
    FIELD_EFFECT_STATE *state   = RamHeapSlots->fieldEffectState;
    FIELD_STATE        *event   = RamHeapSlots->fieldState;
    ACTOR              *actor   = state->caster;
    Sint32              ability = state->ability;

    fdActorSelectAnimationStub(actor, ANIM_CAST_PSYNERGY_START);
    actor->targetX   = actor->x.fixed;
    actor->velocityX = 0;
    actor->targetY   = actor->y.fixed;
    actor->velocityY = 0;
    actor->targetZ   = actor->z.fixed;
    actor->velocityZ = 0;
    if (state->flashCaster != 0)
    {
        sySoundPlayStub(SE_POWER_UP);
        fdBattleEffectSetObjectUpdateMode(actor, CASTER_LOOK_FLASHING_WHITE);
    }
    if (state->announceName != 0)
    {
        state->msgShown = 1;
        fdActorSetOthersPaused(actor, 1, 0);
        mnMessageArgumentPushStub(ability, MESSAGE_ARGUMENT_ABILITY);
        mnFieldMessageShowStub((Sint32)&MsgCastAbilityName, state->msgMode);
        fdActorSetOthersPaused(actor, 0, ANIM_STEP_NORMAL);
        state->msgShown = 0;
    }
    if (syGameFlagGet(FLAG_PLAY_PSYNERGY_CAST))
    {
        if (state->flashCaster != 0)
            fdBattleEffectSetObjectUpdateMode(actor, CASTER_LOOK_CYCLING_COLORS);
        fdActorSelectAnimationStub(actor, ANIM_CAST_PSYNERGY_END);
    }
    else
    {
        fdBattleEffectStopMainActor();
    }
    event->castStarted = 1;
}

// Takes the caster out of its casting update and waits a tick.
void fdBattleEffectStopMainActor(void)
{
    FIELD_EFFECT_STATE *state = RamHeapSlots->fieldEffectState;

    fdBattleEffectSetObjectUpdateMode(state->caster, CASTER_LOOK_NORMAL);
    sySynch(1);
}

// Ends a field Psynergy: frees the field effect sprites still in use and, unless another is being
// resolved, waits for the swelling arcs to end, restores the map's shake and releases the state.
void fdPsynergyEnd(void)
{
    FIELD_EFFECT_STATE *state = RamHeapSlots->fieldEffectState;
    FIELD_STATE        *event = RamHeapSlots->fieldState;
    PALETTE_FADE       *fade  = RamHeapSlots->paletteFade;
    MAP_STATE          *map   = RamHeapSlots->mapGrid;
    ACTOR              *owner = state->caster;
    Sint32              i;
    Sint16              first;

    for (i = 0; i < (Sint32)ARRAY_COUNT(state->particles); i++)
    {
        FIELD_PARTICLE *slot = &state->particles[i];
        if (slot->active)
            fdEffectClearOwnedSlot(slot);
    }
    first = FIELD_PSYNERGY_LASH;
    if (state->fieldPsynergy >= first
        && (state->fieldPsynergy <= FIELD_PSYNERGY_GRIND
            || state->fieldPsynergy == FIELD_PSYNERGY_TELEPORT))
        fdPsynergyMarkWorldMapTilesUsed();
    if (event->psynergyActing != 0)
        return;
    fdBattleEffectWaitForArcGroups();
    fdBattleEffectEndArcEffect();
    if (event->mode == ACTOR_SYSTEM_WORLD_MAP && fade != NULL)
    {
        i = 0;
        if (fade->duration != 0)
        {
            do
            {
                sySynch(1);
                if (++i >= END_FADE_WAIT_TICKS)
                    break;
            } while (fade->duration != 0);
        }
    }
    event->castStarted = 0;
    fdPsynergyReleaseSceneSprite();
    syCloseEvent(fdBattleEffectUpdateAllEffectSlots);
    map->shakeX     = state->savedShakeX;
    map->shakeY     = state->savedShakeY;
    map->shakeDecay = state->savedShakeDecay;
    if (owner != NULL)
    {
        owner->priorityFlags                   = state->savedPriorityFlags;
        ((SPR *)owner->sprite)->image.priority = state->savedSpritePriority;
    }
    if (state->fieldPsynergy != FIELD_PSYNERGY_REVEAL)
        event->effectTimerRunning = 1;
    if (state->fieldPsynergy != FIELD_PSYNERGY_RING)
        event->ringTimerRunning = 1;
    fdScriptNoOperation();
    syHeapSlotRelease(HEAP_SLOT_FIELD_EFFECT);
}

// On the World Map, notes that the field Psynergy has used the second half of the BG tile buffer,
// so the next still image decodes it again.
void fdPsynergyMarkWorldMapTilesUsed(void)
{
    HEAP_SLOTS *heap = RamHeapSlots;
    Sint32      mode = ((FIELD_STATE *)heap->fieldState)->mode;

    if (mode == ACTOR_SYSTEM_WORLD_MAP)
        ((WORLD_MAP_VIEW *)heap->mapGrid)->tileBufUsed = mode;
}

// Starts Mind Read's wave between the caster and the one it reads (HEAP_SLOT_MIND_READ): a hue
// taken from the tick count, the field faded toward it, the message window greyed for thoughts, and
// fdPsynergyUpdateMindReadWave run each tick.
void fdPsynergyStartMindReadWave(Sint32 casterId, Sint32 targetId)
{
    MIND_READ_STATE *state;

    state = syHeapSlotAllocateEWRAM(HEAP_SLOT_MIND_READ, sizeof(*state));
    DMA_FILL32(0, state, sizeof(*state));
    state->hue      = (TickCount * 2) % HUE_DEGREES;
    state->srcId    = casterId;
    state->targetId = targetId;
    state->delay    = 8;
    state->count    = 0;
    fdPsynergyFadeToCastColor();
    fdMindReadAdvanceTint();
    fdPaletteFadeSetTarget(RGB(state->red, state->green, state->blue) | PALETTE_TINT_CUSTOM,
                           PALETTES_BG);
    fdPaletteFadeStartInterpolation(8);
    fdPsynergyGreyMindReadPalette();
    sySetEvent((EVENT_FUNC)fdPsynergyUpdateMindReadWave, EVENT_PASS_TICK);
}

// Stops the wave effect: restores the caster's colours and the palettes and releases the Psynergy's
// state.
void fdPsynergyStopMindReadWave(void)
{
    MIND_READ_STATE *state = RamHeapSlots->mindRead;

    fdPsynergyRestoreMindReadPalette();
    syCloseEvent(fdPsynergyUpdateMindReadWave);
    fdActorSetLayersColorMode(fdActorGetById(state->srcId), SPRITE_COLOR_NORMAL + 1);
    fdPsynergyFadeFromCastColor();
    syHeapSlotRelease(HEAP_SLOT_MIND_READ);
}

#define MIND_READ_GREY RGB(28, 28, 28) // a thought window's light grey

// Greys the message window's white and most of its other colours (MIND_READ_GREY), so messages read
// as thoughts, and starts the BG0 line scroll (fdMindReadStartRipple) at each VBlank.
void fdPsynergyGreyMindReadPalette(void)
{
    volatile Uint16 *p;

    ((WIN_STATE *)RamHeapSlots->windowState)->mindRead = 1;
    p   = &BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_COLOR_WHITE);
    *p  = MIND_READ_GREY;
    p  += 2;
    *p  = MIND_READ_GREY;
    p  += 8;
    *p  = MIND_READ_GREY;
    p  += 1;
    *p  = MIND_READ_GREY;
    p  -= 8;
    *p  = MIND_READ_GREY;
    p  += 1;
    *p  = MIND_READ_GREY;
    p  += 1;
    *p  = MIND_READ_GREY;
    p  += 1;
    *p  = MIND_READ_GREY;
    p  += 1;
    *p  = MIND_READ_GREY;
    p  += 1;
    *p  = MIND_READ_GREY;
    p  += 1;
    *p  = MIND_READ_GREY;
    sySetEvent((EVENT_FUNC)fdMindReadStartRipple, EVENT_PASS_VBLANK - 20);
}

// Stops the BG0 line scroll and restores WINDOW_PALETTE.
void fdPsynergyRestoreMindReadPalette(void)
{
    WIN_STATE *state = (WIN_STATE *)RamHeapSlots->windowState;

    syCloseEvent((EVENT_FUNC)fdMindReadStartRipple);
    BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_COLOR_WHITE)      = RGB_WHITE;
    BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_COLOR_BLACK)      = RGB(0, 0, 0);
    BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_COLOR_DARK_GREY)  = RGB(10, 10, 10);
    BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_COLOR_LIGHT_GREY) = RGB(20, 20, 20);
    mnWindowColorApplyStub(MainRoutineStateWork.windowHue, MainRoutineStateWork.windowBrightness);
    state->mindRead = 0;
}
