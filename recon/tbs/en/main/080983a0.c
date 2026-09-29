#include "TYPES.H"

extern u8 *gEventWork;
extern u8 gGameState[];

void ObjectDispatch_ApplyValueToKind200Children(s32 mode);
void DisplayTransition_InitializeBattleEffectState(s32 mode);
void *ObjectTable_Get(void *entry);
void BattleFx_ApplyColorToSourceBuffer(s32 value, s32 mode);
void BattleFx_ApplyColorToTargetBuffer(s32 value, s32 mode);
void BattleFx_StartBufferInterpolation(s32 mode);
void WaitFrames(s32 frames);
s32 BattleFx_FindMatchingEvent(s32 value, s32 count, s32 *result);
void BattleFx_RunEventAction(s32 handle, void *entry, s32 result);
void Audio_PlayCue(s32 cue);
void FieldEffect_SpawnNearbyMarkers(void);
s32 Scheduler_AddOrUpdateCallback(const void *callback, s32 delay);
void FieldEffect_WatchLeaderDistance(void);

void RunBattleEffect08(void)
{
    u8 **state_slot = &gEventWork;
    u8 *state = *state_slot;
    u8 *scene;
    void **entry_slot;
    u8 *object;
    s32 result;
    s32 value;
    s16 frame;

    ObjectDispatch_ApplyValueToKind200Children(6);
    DisplayTransition_InitializeBattleEffectState(8);
    scene = state_slot[4];
    entry_slot = (void **)(gGameState + 500);
    object = ObjectTable_Get(*entry_slot);
    *(s32 *)(scene + 0x52c) = *(s32 *)(object + 8);
    *(s32 *)(scene + 0x530) = *(s32 *)(object + 16) - *(s32 *)(object + 12);
    BattleFx_ApplyColorToSourceBuffer(0x10000, 0);
    BattleFx_ApplyColorToTargetBuffer(0x10001, 1);
    BattleFx_StartBufferInterpolation(1);
    WaitFrames(1);
    value = BattleFx_FindMatchingEvent(0x50000005, 8, &result);
    if (value != 0) {
        BattleFx_RunEventAction(value, *entry_slot, result);
    }
    Audio_PlayCue(0x83);
    *(s16 *)(state + 0xcb8) = 1;
    value = *(s32 *)(scene + 0x52c);
    if (value < 0) {
        value += 0xffff;
    }
    *(s16 *)(state + 0xcbc) = value >> 16;
    value = *(s32 *)(scene + 0x530);
    if (value < 0) {
        value += 0xffff;
    }
    *(s16 *)(state + 0xcbe) = value >> 16;
    *(s16 *)(state + 0xcba) = 0x258;
    *(s16 *)(state + 0xcc0) = 1;
    FieldEffect_SpawnNearbyMarkers();
    frame = 0;
    do {
        WaitFrames(1);
        *(s16 *)(scene + 0x52a) = frame;
        frame++;
    } while (frame <= 18);
    Scheduler_AddOrUpdateCallback(FieldEffect_WatchLeaderDistance, 0xc80);
}
