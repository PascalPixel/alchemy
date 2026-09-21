#include "CALLBACK_SCHEDULER.H"

extern u8 *Data_03001ebc;
extern s16 Data_02000240[];
#define PARTY_STATE Data_02000240

void *Func_0808ba1c(u32);
#define ObjectTable_Get Func_0808ba1c
void Func_080f9010(s32);
#define Audio_PlayCue Func_080f9010
void Func_08009080(void *, s32);
#define Object_SetMode Func_08009080
void Func_080030f8(s32);
#define WaitFrames Func_080030f8
void Func_08099738(void);
#define run_tile_trigger_sequence Func_08099738
void Func_08099678(void);
#define check_object_tile Func_08099678

#define BattleFx_RunFlashingCallbackSequence Func_08099838

typedef struct {
    u8 unknown_00[37];
    u8 flag_a;
    u8 flag_b;
} EffectSprite;

void BattleFx_RunFlashingCallbackSequence(void)
{
    u8 *state = Data_03001ebc;
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
    callback = Func_08099678;
    Scheduler_AddOrUpdateCallback((s32)callback, 0xc80);
    index = 147;
    *(s16 *)&((s32 *)PARTY_STATE)[index] = 1;
    callback();
    if (*(s16 *)(state + 382) == 0x2092) {
        run_tile_trigger_sequence();
        *(s16 *)(state + 382) = cycle;
    }
}
