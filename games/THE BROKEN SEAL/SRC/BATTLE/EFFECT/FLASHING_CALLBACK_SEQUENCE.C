#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"

extern u8 *gEventWork;
extern s16 gGameState[];
#define PARTY_STATE gGameState

void *ObjectTable_Get(u32);
void Audio_PlayCue(s32);
void Object_SetMode(void *, s32);
void run_tile_trigger_sequence(void);
void check_object_tile(void);

typedef struct {
    u8 unknown_00[37];
    u8 flag_a;
    u8 flag_b;
} EffectSprite;

void BattleFx_RunFlashingCallbackSequence(void)
{
    u8 *state = gEventWork;
    s32 index = 250;
    u8 *object = ObjectTable_Get(*(u32 *)&PARTY_STATE[index]);
    EffectSprite *record = *(EffectSprite **)(object + 80);
    u8 *entry = *(u8 **)((u8 *)record + 40);
    u32 cycle;
    void (*callback)(void);

    Audio_PlayCue(0x82);
    Object_SetMode(object, 0);
    *(void **)(object + 108) = 0;
    cycle = 0;
    do {
        entry[5] = 7;
        record->flag_a = 1;
        record->flag_b = 2;
        WaitFrames(2);
        record->flag_a = 1;
        record->flag_b = 0;
        WaitFrames(2);
        cycle++;
    } while (cycle <= 9);
    cycle = 0;
    entry[5] = cycle;
    record->flag_b = 2;
    record->flag_a = 1;
    callback = check_object_tile;
    Scheduler_AddOrUpdateCallback((s32)callback, 0xc80);
    index = 147;
    *(s16 *)&((s32 *)PARTY_STATE)[index] = 1;
    callback();
    if (*(s16 *)(state + 382) == 0x2092) {
        run_tile_trigger_sequence();
        *(s16 *)(state + 382) = cycle;
    }
}
