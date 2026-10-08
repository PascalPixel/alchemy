#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "callback_scheduler.h"
#include "edition.h"
#include "types.h"
#include "io_reg.h"
#include "scene.h"
#include "sound_ids.h"
#include "fixed_math.h"
#include "tbs_edition.h"
#include "system.h"
#include "keystate.h"
#include "game_state.h"
#include "btlwork.h"
#include "uiflow.h"
#include "text/msg_ids.h"

#define gLinkCountdownWork (*(struct LinkCountdownState * *)&gLinkCountdownWork)
#define Ui_FillVramBlockPattern ((unsigned char (*)(void))Ui_FillVramBlockPattern)
#define UiWindow_Create ((s32 (*)(s32, s32, s32, s32, s32))UiWindow_Create)
#define UiText_CopyMessageString ((s32 (*)(s32, s16 *, s32))UiText_CopyMessageString)
#define UiText_DecodeMessage ((void (*)(s32, s16 *, s32))UiText_DecodeMessage)

#define UiText_RenderWideStringAtOffset ((void (*)(s16*,s32,s32,s32))UiText_RenderWideStringAtOffset)

#define COUNTDOWN_START_FRAMES 900
#define FRAMES_PER_SECOND 60

struct CountdownDisplayEntry {
    u8 data[12];
};

struct LinkSignature {
    u8 padding_000[8];
    u16 e;
    u16 d;
    u16 v;
    u16 s;
    u8 padding_010[8];
};

struct LinkCountdownState {
    struct CountdownDisplayEntry entries[3];
    u8 entryActive[3];
    u8 padding_027;
    s32 targetOffset;
    s32 currentOffset;
    u8 padding_030[20];
    s32 displayHandle;
    s32 secondaryHandle;
    s32 timer;
    s32 enabled;
};

extern struct LinkSignature gLinkPeerSignatures[];
void UiText_DrawNumberInWindow(s32 value, s32 width, s32 handle, s32 arg3, s32 arg4);
void AudioCommand_PlayFar(s32 soundId);


void UpdateLinkSessionCountdown(void)
{
    struct LinkCountdownState *state = gLinkCountdownWork;
    struct BtlWork *runtime;
    struct CountdownDisplayEntry *entry;
    u8 *active;
    struct LinkSignature *signature;
    s32 target;
    s32 current;
    s32 difference;
    s32 step;
    s32 newOffset;
    s32 timer;
    s32 nextTimer;
    s32 seconds;
    s32 i;

    if (state != 0) {
        target = state->targetOffset;
        current = state->currentOffset;
        if (target != current) {
            difference = target - current;
            step = difference / 3;
            if (step == 0) {
                step--;
                if (difference >= 0)
                    step = 1;
            }
            newOffset = current + step;
            state->currentOffset = newOffset;
            Camera_ConfigureSceneFar(newOffset);
        }

        entry = state->entries;
        active = state->entryActive;
        i = 2;
        do {
            if (*active++ != 0)
                Runtime_PushSlotEntry(entry, 240);
            i--;
            entry++;
        } while (i >= 0);
        Link_DrawShiftedTilePair(0x06006680);

        if (state->enabled == 0)
            goto done;

        runtime = BtlWk;
        if (runtime->link_paused == 0)
            goto load_timer;
        state->timer = 0;
        goto done;

signature_tail:
        if (signature->v != 'V')
            goto signature_done;
        if (signature->s != 'S')
            goto signature_done;
        state->timer = COUNTDOWN_START_FRAMES;
        timer = state->timer;
        goto signature_done;

load_timer:
        timer = state->timer;
        if (timer >= 0)
            goto timer_ready;
        signature = &gLinkPeerSignatures[runtime->link_side ^ 1];
        if (signature->e != 'E')
            goto signature_done;
        if (signature->d == 'D')
            goto signature_tail;

signature_done:
        if (timer < 0)
            goto done;

timer_ready:
        if (state->displayHandle == 0 && state->secondaryHandle == 0) {
            state->displayHandle = Link_CreateCountdownLabelWindow();
            timer = state->timer;
        }

        if (timer > 0) {
            nextTimer = timer - 1;
            state->timer = nextTimer;
            timer = nextTimer;
        }
        if (timer < 0)
            goto done;

        seconds = (timer + FRAMES_PER_SECOND - 1) / FRAMES_PER_SECOND;
        if (seconds != 0 && seconds *FRAMES_PER_SECOND == timer)
            AudioCommand_PlayFar(SOUND_TRIPLE_TONE_LOW);

        if (state->displayHandle != 0)
            UiText_DrawNumberInWindow(seconds, 2, state->displayHandle, 16, 8);
    }

done:
    return;
}

#if EDITION_INTERNATIONAL
#define TEXT_COUNT 52
#else
#define TEXT_COUNT 32
#endif

#if EDITION_INTERNATIONAL


#else


#endif

s32 UiText_ShowLocalizedMessageAndWait(void)
{
    s16 buffer[64];
    s32 work;
    s32 result;
    struct BtlWork *battle;
    s32 mode;

    battle = BtlWk;
    mode = gGameState.battle_start;
    if (gGameState.battle_start == 2 || mode <= 2 || (result = 1, mode > 4)) {
        result = 0;
    }
    if (result == 0) {
        if ((s8)battle->chouhatsu_flg != 0) {
            result = 1;
        }
        if (result != 0) {
            goto active;
        }
    } else {
active:
        work = UiWindow_Create(0, 7, 30, 4, 42);
        Ui_FillVramBlockPattern();
#if EDITION_INTERNATIONAL
        UiText_CopyMessageString((s32)MsgNoTimeToRun, buffer, TEXT_COUNT);
#else
        UiText_DecodeMessage((s32)MsgNoTimeToRun, buffer, TEXT_COUNT);
#endif
        UiText_RenderWideStringAtOffset(buffer, work, 0, 4);
        do {
            WaitFrames(1);
        } while ((*(volatile u32 *)&gKeyState & (KEY_A | KEY_B)) == 0 &&
                 gLinkCountdownWork->timer != 0);
        UiWork_Finalize(work, 1);
    }
    return result;
}
