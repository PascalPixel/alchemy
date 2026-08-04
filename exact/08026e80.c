#include "types.h"

#define COUNTDOWN_START_FRAMES 900
#define FRAMES_PER_SECOND 60

struct DisplayEntry {
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

struct CounterState {
    struct DisplayEntry entries[3];
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

struct RuntimeState {
    u8 padding_000[80];
    u8 side;
    u8 padding_051;
    u8 paused;
};

extern struct CounterState *Data_03001f34;
extern struct RuntimeState *Data_03001e74;
extern struct LinkSignature Data_02002024[];

s32 Func_080022ec(s32 dividend, s32 divisor);
void Func_08003dec(struct DisplayEntry *entry, s32 value);
void Func_080219c8(s32 destination);
s32 Func_08021c34(void);
void Func_0801ea08(s32 value, s32 width, s32 handle, s32 arg3, s32 arg4);
void Func_080b50d0(s32 offset);
void Func_080f9010(s32 soundId);

void Func_08026e80(void)
{
    struct CounterState *state = Data_03001f34;
    struct RuntimeState *runtime;
    struct DisplayEntry *entry;
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
            step = Func_080022ec(difference, 3);
            if (step == 0) {
                step--;
                if (difference >= 0)
                    step = 1;
            }
            newOffset = current + step;
            state->currentOffset = newOffset;
            Func_080b50d0(newOffset);
        }

        entry = state->entries;
        active = state->entryActive;
        i = 2;
        do {
            if (*active++ != 0)
                Func_08003dec(entry, 240);
            i--;
            entry++;
        } while (i >= 0);
        Func_080219c8(0x06006680);

        if (state->enabled == 0)
            goto done;

        runtime = Data_03001e74;
        if (runtime->paused == 0)
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
        signature = &Data_02002024[runtime->side ^ 1];
        if (signature->e != 'E')
            goto signature_done;
        if (signature->d == 'D')
            goto signature_tail;

signature_done:
        if (timer < 0)
            goto done;

timer_ready:
        if (state->displayHandle == 0 && state->secondaryHandle == 0) {
            state->displayHandle = Func_08021c34();
            timer = state->timer;
        }

        if (timer > 0) {
            nextTimer = timer - 1;
            state->timer = nextTimer;
            timer = nextTimer;
        }
        if (timer < 0)
            goto done;

        seconds = Func_080022ec(timer + FRAMES_PER_SECOND - 1, FRAMES_PER_SECOND);
        if (seconds != 0 && seconds * FRAMES_PER_SECOND == timer)
            Func_080f9010(108);

        if (state->displayHandle != 0)
            Func_0801ea08(seconds, 2, state->displayHandle, 16, 8);
    }

done:
    return;
}
