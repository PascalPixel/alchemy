#include "TYPES.H"

struct EffectState {
    u8 unknown_000[0x52a];
    u16 frame;
    s32 x;
    s32 height;
    u8 unknown_534[0xcb8 - 0x534];
    u16 active;
    u16 duration;
    s16 center_x;
    s16 center_y;
    u16 step;
};
struct EffectGlobals {
    struct EffectState *scene;
    u8 unknown_04[12];
    struct EffectState *work;
};
struct EffectTarget {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};
struct PartyTargetState {
    u8 unknown_000[500];
    s32 target;
};
extern struct EffectGlobals Data_03001ebc;
extern struct PartyTargetState Data_02000240;
extern const u8 Value_00010001;
void ObjectDispatch_ApplyValueToKind200Children(s32);
void DisplayTransition_InitializeBattleEffectState(s32);
struct EffectTarget *ObjectTable_Get(s32);
void BattleFx_ApplyColorToSourceBuffer(s32, s32);
void BattleFx_ApplyColorToTargetBuffer(s32, s32);
void BattleFx_StartBufferInterpolation(s32);
void WaitFrames(s32);
s32 BattleFx_FindMatchingEvent(s32, s32, s32 *);
void BattleFx_RunEventAction(s32, s32, s32);
void Audio_PlayCue(s32);
void FieldEffect_SpawnNearbyMarkers(void);
s32 Scheduler_AddOrUpdateCallback(void (*)(void), s32);
void FieldEffect_WatchLeaderDistance(void);

static __inline__ s32 *PartyTarget_Slot(s32 base)
{
    /* FAKEMATCH: keep base-plus-offset formation through an integer argument. */
    return (s32 *)(base + 500);
}

void RunBattleEffect08(void)
{
    struct EffectState *scene;
    struct EffectState *work;
    struct EffectTarget *target;
    s32 *target_no;
    s32 resource;
    s32 result;
    s32 frame;

    scene = Data_03001ebc.scene;
    ObjectDispatch_ApplyValueToKind200Children(6);
    DisplayTransition_InitializeBattleEffectState(8);
    work = Data_03001ebc.work;
    target_no = PartyTarget_Slot((s32)&Data_02000240);
    target = ObjectTable_Get(*target_no);
    work->x = target->x;
    work->height = target->z - target->y;
    BattleFx_ApplyColorToSourceBuffer(0x10000, 0);
    BattleFx_ApplyColorToTargetBuffer((s32)&Value_00010001, 1);
    BattleFx_StartBufferInterpolation(1);
    WaitFrames(1);
    result = BattleFx_FindMatchingEvent(0x50000005, 8, &resource);
    if (result != 0)
        BattleFx_RunEventAction(result, *target_no, resource);
    Audio_PlayCue(131);
    scene->active = 1;
    scene->center_x = work->x / 0x10000;
    scene->center_y = work->height / 0x10000;
    scene->duration = 600;
    scene->step = 1;
    FieldEffect_SpawnNearbyMarkers();
    frame = 0;
    do {
        WaitFrames(1);
        work->frame = frame;
        frame++;
    } while (frame <= 18);
    Scheduler_AddOrUpdateCallback(FieldEffect_WatchLeaderDistance, 0xc80);
}
