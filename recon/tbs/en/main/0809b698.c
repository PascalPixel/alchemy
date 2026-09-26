/* Draft: complete 364-byte effect (332-byte body plus 32-byte pool).
 * Candidate 368 bytes, 161 differing halfwords. Typed actor and visual
 * views leave resource conversion, load scheduling and loop allocation.
 * Prior byte-view draft was 364 bytes, 32 halfwords; retained in history.
 */
#include "TYPES.H"

struct EffectVisualEntry {
    u8 unknown_00[5];
    u8 mode;
};
struct EffectVisualGroup {
    u8 unknown_00[37];
    u8 changed;
    u8 refresh;
    u8 unknown_27;
    struct EffectVisualEntry *entry;
};
struct EffectActor {
    u8 unknown_00[6];
    u16 facing;
    u8 unknown_08[72];
    struct EffectVisualGroup *visuals;
    u8 unknown_54[16];
    s16 active;
    s16 frame;
    u8 unknown_68[4];
    void (*callback)(void);
};
struct EffectWork {
    u8 unknown_00[16];
    struct EffectActor *actor;
    u8 unknown_14[0x71a - 0x14];
    s16 resource;
};
struct EffectParty {
    u8 unknown_000[580];
    s32 control;
    u8 flag;
    u8 unknown_249[3];
    s16 variant;
};

extern struct EffectWork *Data_03001f30;
extern struct EffectParty Data_02000240;
extern const u8 Data_0809c510[];
s32 Resource_FindFreeEntry(void);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *src);
void Resource_ResetEntry(s32 slot);
s32 GameFlag_TestFar(s32 flag);
void Animation_ApplyChildValuesFar(struct EffectActor *actor, s32 value);
void ObjectDispatch_SetSingleChildField26Far(struct EffectActor *actor, s32 value);
void Audio_PlayCue(s32 cue);
void WaitFrames(s32 frames);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Scheduler_RemoveCallback(void (*callback)(void));
void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 arg);
void BattleFx_UpdatePairedArcSpawner(void);
void BattleFx_UpdateEffect16State(void);

void RunBattleEffect16(void)
{
    struct EffectWork *work;
    struct EffectActor *actor;
    struct EffectVisualGroup *group;
    struct EffectVisualEntry *entry;
    u32 facing;
    s16 resource;
    s32 count;

    work = Data_03001f30;
    actor = work->actor;
    group = actor->visuals;
    facing = actor->facing;
    entry = group->entry;
    resource = Resource_FindFreeEntry();
    work->resource = resource;
    VramBlock_LoadCached(resource, 256, Data_0809c510);
    Data_02000240.control = 0x09600000;
    Data_02000240.flag = GameFlag_TestFar(0x145);
    Animation_ApplyChildValuesFar(actor, 0);
    actor->callback = BattleFx_UpdatePairedArcSpawner;
    actor->active = 0;
    actor->frame = 0;
    Audio_PlayCue(140);
    WaitFrames(15);
    actor->active = 1;
    WaitFrames(10);
    count = 19;
    do {
        entry->mode = 7;
        group->changed = 1;
        WaitFrames(2);
        group->changed = 1;
        entry->mode = 0;
        group->refresh = 1;
        count--;
        WaitFrames(3);
    } while (count >= 0);
    actor->callback = 0;
    actor->facing = facing;
    Scheduler_AddOrUpdateCallback(BattleFx_UpdateEffect16State, 0xc80);
    WaitFrames(15);
    Audio_PlayCue(174);
    WaitFrames(55);
    Scheduler_RemoveCallback(BattleFx_UpdateEffect16State);
    if (Data_02000240.variant != 0)
        ObjectDispatch_SetSingleChildField26Far(actor, 2);
    else
        ObjectDispatch_SetSingleChildField26Far(actor, 1);
    Animation_ApplyChildValuesFar(actor, 0);
    Resource_ResetEntry(work->resource);
    UiText_ShowPositionedMessageAndWaitFar(0x922, 1);
}
