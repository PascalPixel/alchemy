#include "TYPES.H"
#include "FIELD_EVENT.H"

/* The rival's path recorder in the scene state, as COMMON/KOROSSEO/PATH_RIVAL.C
   reads it. */
struct PathRecorder {
    s16 mode;
    s16 mirror;
    s16 actor;
    u16 pos;
    s16 still;
};

void *Resource_GetTableEntry(s32 resource);
void Resource_DecodeType01(const void *source, void *destination);
void Korosseo_UpdatePathRival(void);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);

/* Decode the rival's recorded course into the stage work and, until flag
   0x109 is set, start the recorder replaying it for the actor the work
   names; then schedule the rival's update. */
void SceneState_InitControlWhenFlag109Clear(s32 resource)
{
    u8 *work = gKorosseoWork;
    struct PathRecorder *recorder = (struct PathRecorder *)gSceneState;

    Resource_DecodeType01(Resource_GetTableEntry(resource), work + 240);
    if (GameFlag_IsSet(0x109) == 0) {
        recorder->mode = 1;
        recorder->mirror = 1;
        recorder->actor = *(u16 *)(work + 224);
        recorder->still = 0;
        recorder->pos = 0;
    }
    Scheduler_AddOrUpdateCallback(Korosseo_UpdatePathRival, 0xc85);
}
