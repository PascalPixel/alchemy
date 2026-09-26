/* Draft main:0809b698, RunBattleEffect16. Complete extent is 364 bytes:
 * 332 bytes of code plus the 32-byte pool formerly labelled 0809b7e4.
 * Hypothesis 2: 368 bytes versus 364, 169 differing halfwords (75 edits).
 * Signed child bytes do not restore the loop's fp-held mode 7. The unsigned
 * resource member adds a return-value copy; word/byte views do restore the
 * work-array address additions. Saved-angle scheduling and loop-zero sharing
 * remain. Hypothesis 1 (356 / 163 halfwords) is preserved in history.
 * Previous 364-byte / 32-halfword byte-view draft remains in origin/main.
 */
#include "TYPES.H"
#include "SYSTEM.H"
#include "MOTION_OBJECT.H"
#include "CALLBACK_SCHEDULER.H"

struct EffectChildEntry {
    u8 unknown_00[5];
    s8 mode;
};
struct EffectRecord {
    u8 unknown_00[37];
    s8 reload;
    s8 value;
    u8 unknown_27;
    struct EffectChildEntry *entry;
};
union EffectObject {
    struct MotionObject motion;
    struct {
        u8 unknown_00[100];
        s16 active;
        u16 frame;
        u8 unknown_68[4];
        void (*callback)(void *);
    } effect;
};
struct EffectScene {
    u8 unknown_000[16];
    union EffectObject *object;
    u8 unknown_014[0x71a - 20];
    u16 resource;
};
extern struct EffectScene *Data_03001f30;
extern u8 Data_02000240[];
extern const u8 Data_0809c510[];
extern const u8 Value_00000145;
extern const u8 Value_00000922;
s16 Resource_FindFreeEntry(void);
s32 Resource_ResetEntry(s32);
s32 VramBlock_LoadCached(u32, u32, const void *);
void Animation_ApplyChildValuesFar(void *, s32);
void ObjectDispatch_SetSingleChildField26Far(void *, s32);
void BattleFx_UpdatePairedArcSpawner(void *);
void BattleFx_UpdateEffect16State(void);
s32 GameFlag_TestFar(s32);
void Audio_PlayCue(s32);
void UiText_ShowPositionedMessageAndWaitFar(s32, s32);

void RunBattleEffect16(void)
{
    struct EffectScene *scene;
    union EffectObject *obj;
    struct EffectRecord *record;
    struct EffectChildEntry *entry;
    u32 angle;
    u16 resource;
    s32 index;
    s32 mode;
    s32 cnt;

    scene = Data_03001f30;
    obj = scene->object;
    record = obj->motion.records;
    angle = obj->motion.angle;
    entry = record->entry;
    resource = Resource_FindFreeEntry();
    {
        s32 zero = 0;
        scene->resource = resource;
        VramBlock_LoadCached((s16)resource, 0x100, Data_0809c510);
        index = 145;
        ((s32 *)Data_02000240)[index] = 0x09600000;
        index = 146;
        *(s8 *)&((s32 *)Data_02000240)[index] = GameFlag_TestFar((s32)&Value_00000145);
        Animation_ApplyChildValuesFar(obj, 0);
        obj->effect.callback = BattleFx_UpdatePairedArcSpawner;
        obj->effect.active = zero;
        obj->effect.frame = zero;
    }
    Audio_PlayCue(0x8c);
    WaitFrames(15);
    obj->effect.active = 1;
    WaitFrames(10);
    mode = 7;
    cnt = 19;
    do {
        entry->mode = mode;
        record->reload = 1;
        WaitFrames(2);
        record->reload = 1;
        entry->mode = 0;
        record->value = 1;
        cnt--;
        WaitFrames(3);
    } while (cnt >= 0);
    obj->effect.callback = 0;
    obj->motion.angle = angle;
    Scheduler_AddOrUpdateCallback((s32)BattleFx_UpdateEffect16State, 0xc80);
    WaitFrames(15);
    Audio_PlayCue(0xae);
    WaitFrames(55);
    Scheduler_RemoveCallback((u32)BattleFx_UpdateEffect16State);
    index = 147;
    if (((s16 *)Data_02000240)[index * 2] != 0)
        ObjectDispatch_SetSingleChildField26Far(obj, 2);
    else
        ObjectDispatch_SetSingleChildField26Far(obj, 1);
    Animation_ApplyChildValuesFar(obj, 0);
    Resource_ResetEntry((s16)scene->resource);
    UiText_ShowPositionedMessageAndWaitFar((s32)&Value_00000922, 1);
}
