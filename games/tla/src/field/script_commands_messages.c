// Messages shown at an actor, and the speaker's voice. Part of script_commands.c.

#include "window/stubs.h"
#include "field/field_mode.h"
#include "field/object.h"
#include "field/script.h"

// fdMessageSetVoiceFromActor's key for no voice: it clears the voice.
#define MESSAGE_VOICE_NONE 0x80000000

// The script command "open message": opens the script's current message in a speech window next to
// the speaking actor, with the speaker's portrait beside it when it has one, and moves on to the
// next message.
Sint32 fdMessageShowAtActor(Sint32 speaker, Sint32 unused)
{
    WIN_STATE   *window;
    FIELD_STATE *state;
    Sint32       sprite;
    Sint32       extra;
    Sint32       portraitTop;
    Sint32       margin;
    Sint32       height;
    Sint32       width;
    Sint32       top;
    Sint32       left;
    Sint32       pos[3];
    WINDOW      *handle;
    Sint32       portrait;
    Sint32       flags;
    Sint32       msg;
    ACTOR       *actor;
    Sint32       x;
    Sint32       y;
    Sint32       found;
    Sint32       column;

    window          = (WIN_STATE *)RamHeapSlots->windowState;
    state           = FieldStateWork;
    handle          = 0;
    portrait        = 0;
    sprite          = fdActorGetSpriteById(speaker);
    extra           = 0;
    portraitTop     = 0;
    margin          = 4;
    flags           = speaker & SPEAKER_FLAGS;
    msg             = state->msg;
    speaker        &= SPEAKER_ACTOR_MASK;
    x               = 0;
    actor           = fdActorLookup(speaker);
    state->speaker  = speaker;
    y               = 0;
    found           = 0;
    if (state->fastForward == 0)
    {
        if (actor != 0)
        {
            if (state->mode == ACTOR_SYSTEM_WORLD_MAP)
            {
                syConvert3Dto2D(&actor->x.fixed, pos);
                x      = pos[0] >> 3;
                y      = pos[1] >> 3;
                found  = 1;
                y     -= 2;
            }
            else
            {
                if (fdActorGetScreenPos(speaker, pos) != -1)
                    found = 1;
                else
                    found = 0;
                x = pos[0] >> 3;
                y = pos[1] >> 3;
            }
        }
        else if (speaker <= ACTOR_PIERS)
        {
            sprite = speaker;
            actor  = fdActorLookup(fdPartyGetLeaderActor());
            if (state->mode == ACTOR_SYSTEM_WORLD_MAP)
            {
                syConvert3Dto2D(&actor->x.fixed, pos);
                x     = pos[0] >> 3;
                y     = pos[1] >> 3;
                found = 1;
            }
            else
            {
                if (fdActorGetScreenPos(fdPartyGetLeaderActor(), pos) != -1)
                    found = 1;
                else
                    found = 0;
                x = pos[0] >> 3;
                y = pos[1] >> 3;
            }
        }

        if (!found)
        {
            left = DISPLAY_TILE_WIDTH / 2;
            top  = DISPLAY_TILE_HEIGHT / 2;
        }
        else
        {
            left = 0;
            top  = 0;
            mnTextGetMessageSizeAllPagesStub(msg, &left, &top, &width, &height);
            left = x - (Sint32)width / 2;
            if (flags & SPEAKER_WINDOW_ABOVE)
                top = y - height - 1;
            else if (!(flags & SPEAKER_WINDOW_BELOW) && y <= 8)
                top = y - height - 1;
            else
                top = y + 4;
        }

        // Place the portrait beside the speaker, on the side with more room.
        if (window->mindRead != 0)
            margin = 5;
        column = x;
        if (flags & SPEAKER_PORTRAIT_LEFT)
        {
            column = x - margin - 2;
            if (column < 0)
                column = 0;
        }
        else if (flags & SPEAKER_PORTRAIT_RIGHT)
        {
            column += 2;
            if (column + margin > DISPLAY_TILE_WIDTH - 1)
                column = DISPLAY_TILE_WIDTH - 1 - margin;
        }
        else if (column <= DISPLAY_TILE_WIDTH / 2)
        {
            column = column - margin - 2;
            if (column < 0)
                column = x + 2;
        }
        else
        {
            column += 2;
            if (column + margin > DISPLAY_TILE_WIDTH - 1)
                column = x - margin - 2;
        }

        // A speaker with a portrait: put the portrait just above or below the message, within the
        // screen's rows, and grow the message to the portrait's height.
        found = mnPortraitGetForSpeakerStub(sprite);
        if (found != -1)
        {
            mnTextGetMessageSizeAllPagesStub(msg, &left, &top, &width, &height);
            msg = -1;
            if (top > y)
                portraitTop = top - PORTRAIT_WINDOW_TILES;
            else
                portraitTop = top + height;
            if (portraitTop < 0)
                portraitTop = top + height;
            else if (portraitTop + PORTRAIT_WINDOW_TILES > DISPLAY_TILE_HEIGHT - 1)
                portraitTop = top - PORTRAIT_WINDOW_TILES;
            if (top < portraitTop)
            {
                Sint32 lines = height;

                mnTextGetMessageSizeStub(msg, &left, &top, &width, &height);
                msg   = -1;
                extra = lines - height + 1;
            }
        }
        else if (top < y)
        {
            Sint32 lines = height;

            mnTextGetMessageSizeStub(msg, &left, &top, &width, &height);
            extra = lines - height + 1;
            msg   = found;
        }
        if (column < 0)
            column = 0;
        else if (column + margin > DISPLAY_TILE_WIDTH - 1)
            column = DISPLAY_TILE_WIDTH - 1 - margin;

        // Open the message and the portrait, and wait until the text is typed out.
        if (window->mindRead != 0)
        {
            sySynch(8);
            if (extra != 0)
                handle =
                    mnMessageOpenWindowStub(msg,
                                            left,
                                            top + extra - 1,
                                            MESSAGE_WINDOW_ANCHOR_BOTTOM | MESSAGE_WINDOW_UNREAD);
            else
                handle = mnMessageOpenWindowStub(msg, left, top, MESSAGE_WINDOW_UNREAD);
        }
        else
        {
            Sint32 voice = fdSpeakerGetVoice(sprite);

            if (extra != 0)
                handle = mnMessageOpenWindowStub(msg,
                                                 left,
                                                 top + extra - 1,
                                                 MESSAGE_WINDOW_VOICE(voice)
                                                     | MESSAGE_WINDOW_ANCHOR_BOTTOM
                                                     | MESSAGE_WINDOW_ZOOM);
            else
                handle = mnMessageOpenWindowStub(
                    msg, left, top, MESSAGE_WINDOW_VOICE(voice) | MESSAGE_WINDOW_ZOOM);
        }
        if (window->mindRead != 0)
            portrait = (Sint32)mnPortraitOpenWindowStub(sprite, 0, column, portraitTop);
        else
            portrait = (Sint32)mnPortraitOpenWindowStub(sprite, 0, column, portraitTop);
        while (!mnMessageAllTypedStub())
            sySynch(1);
    }
    state->msgWindow  = (Sint32)handle;
    state->sideWindow = (WINDOW *)portrait;
    state->msg++;
    return handle;
}

// Opens the script's current message at the speaker and waits for its window; after ten seconds, or
// with Select, R, L and A held, the text is finished at once.
void fdMessageShowAtActorAndWait(Sint32 speaker, Sint32 flags)
{
    Sint32       masked;
    FIELD_STATE *state  = RamHeapSlots->fieldState;
    void        *window = fdMessageShowAtActor(speaker, flags);
    Sint32       id;
    Uint32       ticks;

    ticks = 0;
    sySynch(1);
    id = fdActorGetSpriteById(speaker);
    if (speaker <= ACTOR_PIERS)
    {
        masked = speaker & SPEAKER_ACTOR_MASK;
        if (fdFindActorPlacement(masked) == 0)
            id = masked;
    }
    mnPortraitCloseWindowStub(id);

    if (state->fastForward == 0)
    {
        while (mnWindowIsClosedStub((Sint32)window) == 0)
        {
            sySynch(1);
            ticks++;
            if (ticks > 600
                || ((PerWork.data & KEY_SELECT) && (PerWork.data & KEY_R) && (PerWork.data & KEY_L)
                    && (PerWork.data & KEY_A)))
                mnMessageCloseFinishedWindowsStub();
        }
    }

    fdMessageSetVoiceFromActor(MESSAGE_VOICE_NONE);
    sySynch(1);
}

// The script command "show message and wait": shows the message, then waits the given number of
// ticks.
void fdScriptShowMessageThenWait(Sint32 speaker, Sint32 flags, Sint32 ticks)
{
    fdMessageShowAtActorAndWait(speaker, flags);
    fdScriptWait(ticks);
}

// The script command "show message": opens the current message at the speaker
// (fdMessageShowAtActor) and waits until it has been read and its window has closed; the speaker's
// portrait is closed once the text is typed out.
void fdScriptShowMessageAndWait(Sint32 speaker, Sint32 flags)
{
    fdMessageShowAtActorAndWait(speaker, flags);
}

// The script command "ask yes or no": opens the question at the speaker, lets the leader answer,
// then shows the next message for yes or the one after it for no.
Sint32 fdScriptAskYesNoAndShowResponse(Sint32 speaker, Sint32 flags)
{
    Sint32 result;

    fdMessageShowAtActor(speaker, flags);
    result = fdScriptAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0);
    if (result == 0)
    {
        fdMessageShowAtActorAndWait(speaker, flags);
        FieldStateWork->msg++;
    }
    else
    {
        FieldStateWork->msg++;
        fdMessageShowAtActorAndWait(speaker, flags);
    }
    return result;
}

// An empty hook; it does nothing in the released game.
void fdScriptNoOperationUpdate(void)
{
}

// fdScriptShowSpeakerMessage and fdScriptShowMessageAtPos move a position MESSAGE_POSITION_SHIFT
// away from the middle row of a MESSAGE_POSITION_WIDTH by MESSAGE_POSITION_HEIGHT area, larger than
// the 240 by 160 screen, and hold it MESSAGE_POSITION_MARGIN_X and MESSAGE_POSITION_MARGIN_Y inside
// the area.
#define MESSAGE_POSITION_WIDTH    320
#define MESSAGE_POSITION_HEIGHT   240
#define MESSAGE_POSITION_MARGIN_X 8
#define MESSAGE_POSITION_MARGIN_Y 20
#define MESSAGE_POSITION_SHIFT    32

// Unused: nothing calls it. It would record the speaker, open the message near it and wait unless
// fast-forwarding, then advance the message index. It places the message as
// fdScriptShowMessageAtPos does, before passing the position to a window function that takes tiles;
// with no caller, the units it expected can't be told.
void fdScriptShowSpeakerMessage(Sint32 speaker)
{
    // BUG: the lookup's result is discarded, and pos is left unset.
    struct
    {
        Sint32 x, y;
    } pos;

    FIELD_STATE *state  = RamHeapSlots->fieldState;

    speaker            &= SPEAKER_ACTOR_MASK;
    fdActorLookup(speaker);
    state->speaker = speaker;
    if (state->fastForward == 0)
    {
        Sint32 windowX = pos.x;
        Sint32 windowY = pos.y;
        Sint32 window;

        if (windowY > MESSAGE_POSITION_HEIGHT / 2 - 1)
            windowY += MESSAGE_POSITION_SHIFT;
        else
            windowY -= MESSAGE_POSITION_SHIFT;
        windowX = windowX < MESSAGE_POSITION_MARGIN_X ? MESSAGE_POSITION_MARGIN_X : windowX;
        if (windowX > MESSAGE_POSITION_WIDTH - MESSAGE_POSITION_MARGIN_X)
            windowX = MESSAGE_POSITION_WIDTH - MESSAGE_POSITION_MARGIN_X;
        windowY = windowY < MESSAGE_POSITION_MARGIN_Y ? MESSAGE_POSITION_MARGIN_Y : windowY;
        if (windowY > MESSAGE_POSITION_HEIGHT - MESSAGE_POSITION_MARGIN_Y)
            windowY = MESSAGE_POSITION_HEIGHT - MESSAGE_POSITION_MARGIN_Y;
        window = mnMessageOpenWindowStub((Sint16)state->msg, windowX, windowY, MESSAGE_WINDOW_ZOOM);
        state->msgWindow = window;
        while (mnWindowIsClosedStub(window) == 0)
            sySynch(1);
    }
    state->msg++;
}

// Unused: nothing calls it; forwards its arguments to fdScriptShowMessageAtPos.
void fdScriptForwardShowMessageAtPos(Sint32 unusedA, Sint32 unusedB, Sint32 x, Sint32 y)
{
    fdScriptShowMessageAtPos(unusedA, unusedB, x, y);
}

// Unused: nothing calls it. It would open the script's message at (x, y), wait for it and advance
// the message index; its units are as unknown as fdScriptShowSpeakerMessage's.
void fdScriptShowMessageAtPos(Sint32 unusedA, Sint32 unusedB, Sint32 x, Sint32 y)
{
    FIELD_STATE *state   = FieldStateWork;
    Sint32       windowY = y;
    Sint32       windowX = x;
    Sint32       minX    = MESSAGE_POSITION_MARGIN_X;
    Sint32       minY    = MESSAGE_POSITION_MARGIN_Y;
    Sint32       window;

    if (windowY > MESSAGE_POSITION_HEIGHT / 2 - 1)
        windowY += MESSAGE_POSITION_SHIFT;
    else
        windowY -= MESSAGE_POSITION_SHIFT;

    if (x < minX)
        windowX = minX;
    if (windowX > MESSAGE_POSITION_WIDTH - MESSAGE_POSITION_MARGIN_X)
        windowX = MESSAGE_POSITION_WIDTH - MESSAGE_POSITION_MARGIN_X;
    CLAMP(windowY, minY, MESSAGE_POSITION_HEIGHT - MESSAGE_POSITION_MARGIN_Y);

    window = (Sint32)mnMessageOpenWindowStub(state->msg, windowX, windowY, MESSAGE_WINDOW_ZOOM);
    while (mnWindowIsClosedStub((WINDOW *)window) == 0)
        sySynch(1);
    state->msg++;
}

// Closes the portrait of an actor's sprite, if it has one.
void fdActorClosePortrait(Sint32 actorId)
{
    Sint32 sprite;

    sprite = fdActorGetSpriteById(actorId);
    if (sprite != -1)
        mnPortraitCloseWindowStub(sprite);
}

// The script command "show two messages and wait": two actors speak at once, each message at its
// own (x, y) with its portrait at (portraitX, portraitY), all in tiles. When delay is nonzero, the
// second message waits for the first to finish, checking every delay ticks. Waits for A, B, L or R,
// then closes both.
void fdScriptShowTwoMessagesAndWait(Sint32 first,
                                    Sint32 firstX,
                                    Sint32 firstY,
                                    Sint32 firstPortraitX,
                                    Sint32 firstPortraitY,
                                    Sint32 second,
                                    Sint32 secondX,
                                    Sint32 secondY,
                                    Sint32 secondPortraitX,
                                    Sint32 secondPortraitY,
                                    Sint32 delay)
{
    FIELD_STATE *event        = RamHeapSlots->fieldState;
    Sint32       firstSprite  = fdActorGetSpriteById(first);
    Sint32       secondSprite = fdActorGetSpriteById(second);
    Sint32       firstWindow;
    Sint32       secondWindow;

    firstWindow = mnMessageOpenWindowStub(
        (Sint16)event->msg++, firstX, firstY, MESSAGE_WINDOW_VOICE(fdSpeakerGetVoice(firstSprite)));
    mnPortraitOpenWindowStub(firstSprite, 0, firstPortraitX, firstPortraitY);
    if (delay != 0)
    {
        while (!mnMessageAllTypedStub())
            sySynch(delay);
    }

    secondWindow = mnMessageOpenWindowStub((Sint16)event->msg++,
                                           secondX,
                                           secondY,
                                           MESSAGE_WINDOW_VOICE(fdSpeakerGetVoice(secondSprite)));
    mnPortraitOpenWindowStub(secondSprite, 0, secondPortraitX, secondPortraitY);

    while (mnMessageAllTypedStub() == 0)
        sySynch(1);

    sySynch(1);
    while ((PerWork.push & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0)
        sySynch(1);

    sySynch(1);
    mnPortraitCloseWindowStub(firstSprite);
    mnPortraitCloseWindowStub(secondSprite);
    mnMessageCloseFinishedWindowsStub();
    sySynch(1);

    while (mnWindowIsClosedStub((WINDOW *)firstWindow) == 0)
        sySynch(1);
    while (mnWindowIsClosedStub((WINDOW *)secondWindow) == 0)
        sySynch(1);

    sySynch(1);
}

extern const Uint8 MessageSpeedVoiceRates[];

// Sets the voice of the messages that follow to that of actor key's sprite, at the rate the message
// speed option picks (MessageSpeedVoiceRates); MESSAGE_VOICE_NONE clears it.
void               fdMessageSetVoiceFromActor(Sint32 key)
{
    WIN_STATE *state = (WIN_STATE *)RamHeapSlots->windowState;
    Uint8      value;
    Sint32     id;

    if (key == MESSAGE_VOICE_NONE)
    {
        *(Sint16 *)&state->speakerVoiceSound     = (value = 0);
        *(Sint16 *)&state->speakerVoiceCountdown = value;
    }
    else
    {
        id    = fdSpeakerGetVoice(fdActorGetSpriteById(key));
        value = MessageSpeedVoiceRates[MainRoutineStateWork.msgSpeed];
        *(Sint16 *)&state->speakerVoiceSound     = id;
        *(Sint16 *)&state->speakerVoiceCountdown = value;
    }
}

// The script command "camera follow actor": the camera follows the actor, put on it at once unless
// keepPos leaves it where it is to set out from.
void fdCameraFollowActor(Sint32 actorId, Sint32 keepPos)
{
    ACTOR       *actor;
    ACTOR       *target;
    FIELD_STATE *state;
    MAP_STATE   *map;

    actor  = fdActorLookup(actorId);
    state  = RamHeapSlots->fieldState;
    target = state->camera;
    map    = RamHeapSlots->mapGrid;
    if (actor != 0)
    {
        map->origin = &target->x.fixed;
        fdActorStartCameraStub(target, actor);
        if (keepPos == 0)
        {
            SET_ACTOR_POSITION(target, actor->x.fixed, actor->y.fixed, actor->z.fixed);
            sySynch(1);
            if (state->mode != ACTOR_SYSTEM_WORLD_MAP)
                fdMapGridCenterOnFollowedPointStub();
        }
    }
}

// The script command "camera set speed": the camera's top speed and acceleration, 16.16 pixels per
// tick.
void fdCameraSetSpeedParams(FIXED speed, FIXED acceleration)
{
    FIELD_STATE *state          = syHeapSlotAllocateEWRAM(HEAP_SLOT_FIELD, sizeof(FIELD_STATE));

    state->camera->speedLimit   = speed;
    state->camera->acceleration = acceleration;
}

#undef MESSAGE_VOICE_NONE
#undef MESSAGE_POSITION_WIDTH
#undef MESSAGE_POSITION_HEIGHT
#undef MESSAGE_POSITION_MARGIN_X
#undef MESSAGE_POSITION_MARGIN_Y
#undef MESSAGE_POSITION_SHIFT
