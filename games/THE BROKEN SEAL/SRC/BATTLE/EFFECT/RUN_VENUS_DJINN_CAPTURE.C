#include "TYPES.H"

/* The Venus Djinni capture: the Djinni hops twice and leaps, a ring of
   particles rises from it and falls back, then a burst above the leader
   and eight sparks scattered around it close the scene. */

struct CaptureObject {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 base_y;
    s32 scale_x;
    s32 scale_y;
    u8 unknown_20[8];
    s32 gravity;
    u8 unknown_2c[4];
    s32 speed;
    u8 unknown_34[33];
    u8 mode;
    u8 unknown_56[5];
    u8 visible;
    u8 unknown_5c[2];
    u16 lifetime;
    u8 unknown_60[4];
    u16 timer;
    u16 angle;
    u8 unknown_68[4];
    void *callback;
};

extern s32 gGameState[];

struct CaptureObject *Object_GetById(s32 id);
void BattleFx_InitializeSlots(void);
void Unnamed_080b0840Far(s32 value);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
void ObjectMotion_Launch(s32 id, s32 height, s32 frames);
void Func_080091f0(s32 x, s32 y, s32 z);
void Object_SetMode(struct CaptureObject *object, s32 mode);
void ObjectMotion_ArmCallback(s32 id, s32 value, s32 flags);
void Camera_WorldToScreen(s32 *position);
void EffectSlot_Initialize(void *slot, s32 kind, s32 x, s32 y);
void EffectSlot_SetCallback(void *slot, void *callback);
void EffectSlot_SetObjectMode(void *slot, s32 mode);
void ObjectGroup_SetChildValueUnlessFifteenFar(s32 object, s32 value);
s32 Random16(void);
s32 Math_Div(s32 numerator, s32 denominator);
s32 Math_DivU(s32 numerator, s32 denominator);
struct CaptureObject *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Animation_ApplyChildValuesFar(struct CaptureObject *object, s32 value);
void Motion_SetTargetPositionFromMagnitudeAngle(struct CaptureObject *object, s32 magnitude, s32 angle);
void Object_SetCallback(struct CaptureObject *object, const void *script);
void Shop_InitEffectFar(void);
void BattleFx_ClearActiveSlotsAndScheduleUpdates(void);

void BattleFx_RunVenusDjinnCapture(s32 arg)
{
    struct CaptureObject *leader;
    struct CaptureObject *djinni;
    void *effect_slots;
    u8 *slot;
    s8 *slot_state;
    s32 remaining;
    s32 scale;
    s32 position[3];

    leader = Object_GetById(gGameState[125]);
    djinni = Object_GetById(arg);
    if (djinni == NULL)
        return;

    BattleFx_InitializeSlots();
    effect_slots = *(void **)0x03001F30;
    Unnamed_080b0840Far(0x201204);
    WaitFrames(30);
    djinni->visible = 0;
    Audio_PlayCue(152);
    ObjectMotion_Launch(arg, 4, 15);
    Audio_PlayCue(152);
    ObjectMotion_Launch(arg, 4, 15);
    WaitFrames(30);
    djinni->callback = (void *)0x0809592d;
    Audio_PlayCue(153);
    ObjectMotion_Launch(arg, 8, 22);
    Audio_PlayCue(140);
    Func_080091f0(0x14ccc, 0x14ccc, 0x10000);
    djinni->callback = (void *)0x08095f9d;
    Object_SetMode(djinni, 3);
    WaitFrames(90);
    ObjectMotion_ArmCallback(gGameState[125], 0x4000, 0);
    WaitFrames(20);
    Object_SetMode(Object_GetById(gGameState[125]), 28);
    WaitFrames(30);
    Func_080091f0(0x19999, 0x19999, 0x10000);

    position[0] = djinni->x;
    position[1] = djinni->y;
    position[2] = djinni->z;
    Camera_WorldToScreen(position);

    slot = (u8 *)effect_slots + 88;
    remaining = 23;
    do {
        EffectSlot_Initialize(slot, 284, position[0], position[2]);
        EffectSlot_SetCallback(slot, (void *)0x08096049);
        EffectSlot_SetObjectMode(slot, 7);
        ObjectGroup_SetChildValueUnlessFifteenFar(*(s32 *)slot, 11);
        *(s32 *)(slot + 40) = 0x8000;
        *(s32 *)(slot + 44) = Random16() + 0x18000;
        remaining--;
        WaitFrames(1);
        slot += 72;
    } while (remaining >= 0);

    WaitFrames(140);
    {
        s32 next_state;

        next_state = 2;
        slot_state = effect_slots;
        slot_state += 152;
        remaining = 23;
        do {
            if (slot_state[5] != 0)
                slot_state[0] = next_state;
            remaining--;
            slot_state += 72;
        } while (remaining >= 0);
    }

    WaitFrames(20);
    Func_080091f0(1, 1, 1);
    WaitFrames(30);

    for (remaining = 0; remaining <= 23; remaining++) {
        position[0] = leader->x;
        position[1] = leader->y + 0x780000;
        position[2] = leader->z;
        djinni = Object_Spawn(284, position[0], position[1], position[2]);
        if (djinni != NULL) {
            scale = Math_DivU(Random16(), 3) + 0x10000;
            djinni->scale_y = scale;
            djinni->scale_x = scale;
            djinni->timer = 100;
            djinni->angle = Math_Div(remaining << 16, 24);
            djinni->callback = (void *)0x08095fcd;
            djinni->mode = 0;
            Object_SetMode(djinni, 7);
            Animation_ApplyChildValuesFar(djinni, 11);
        }
    }

    WaitFrames(100);
    Audio_PlayCue(288);
    WaitFrames(1);
    Audio_PlayCue(151);
    position[0] = leader->x;
    position[1] = leader->y + 0x120000;
    position[2] = leader->z;
    for (remaining = 0; remaining <= 7; remaining++) {
        djinni = Object_Spawn(284, position[0], position[1], position[2]);
        if (djinni == NULL)
            break;
        djinni->scale_y = 0x9999;
        djinni->scale_x = 0x9999;
        djinni->mode = 2;
        djinni->gravity = 0x50000;
        djinni->base_y = djinni->y;
        djinni->speed = Random16() + 0x16666;
        Motion_SetTargetPositionFromMagnitudeAngle(djinni, 0x200000, Random16());
        Animation_ApplyChildValuesFar(djinni, 11);
        djinni->lifetime = 8;
        Object_SetCallback(djinni, (void *)0x0809f0b0);
    }

    WaitFrames(15);
    Shop_InitEffectFar();
    BattleFx_ClearActiveSlotsAndScheduleUpdates();
}
