#include "PROTO.H"
#include "TYPES.H"
#include "MOTION_OBJECT.H"
#include "FX_SCENE.H"
#include "OBJDISP.H"
#include "SYSTEM.H"
#include "SCENE.H"
#include "SOUND_IDS.H"

struct ScatterPosition {
    s32 x;
    s32 y;
    s32 z;
};

extern struct BattleFxScene *gEffectWork;
struct MotionObject *BattleFx_SpawnItemBreakMode1(s32 x, s32 y, s32 z, s32 angle);
void Motion_SetTargetPositionFromMagnitudeAngle(
    void *object, s32 magnitude, s32 angle);
void Object_CommitPosition(void *object);
void Audio_PlayCue(s32 sound);
struct MotionObject *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
extern const s32 BattleFx_FragmentScript[];
void ObjectDispatch_ReleaseFar(void *object);

void Object_SetMode(void *, s32);

void RunBattleEffect11(void)
{
    struct BattleFxScene *scene;
    struct MotionObject *obj;
    struct MotionObject *anchors[2];
    struct ScatterPosition position;
    struct MotionObject **walk;
    s32 index;
    s32 magnitude;

    scene = gEffectWork;
    BattleEffect_InitializeSharedScene();
    position.x = scene->x;
    position.y = scene->y + 0x100000;
    position.z = scene->z;
    anchors[0] = BattleFx_SpawnItemBreakMode1(
        position.x + 0x200000, position.y, position.z, 0x8000);
    anchors[1] = BattleFx_SpawnItemBreakMode1(
        position.x - 0x200000, position.y, position.z, 0);

    WaitFrames(15);
    walk = anchors;
    for (index = 0; index < 2; index++) {
        obj = *walk++;
        if (obj != 0)
            Motion_SetTargetPositionFromMagnitudeAngle(
                obj, 0x180000, obj->angle);
    }

    Object_CommitPosition(anchors[0]);
    Audio_PlayCue(134);
    for (index = 0; index < 24; index++) {
        position.x = scene->x;
        position.y = scene->y + 0x100000;
        position.z = scene->z;
        obj = Object_Spawn(0x11d, position.x, position.y, position.z);
        if (obj != 0) {
            ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_FragmentScript);
            obj->speed_limit = Random16() + 0x20000;
            obj->acceleration = 0x20000;
            obj->motion_flags = 0;
            magnitude = Random16() * 24 + 0x80000;
            Motion_SetTargetPositionFromMagnitudeAngle(obj, magnitude, Random16());
        }
    }

    ObjectDispatch_ReleaseFar(anchors[0]);
    ObjectDispatch_ReleaseFar(anchors[1]);
    BattleFx_PrepareBufferInterpolation();
}

struct MotionObject *BattleFx_SpawnItemBreakMode1(s32 x, s32 y, s32 z, s32 angle)
{
    struct MotionObject *obj;
    s32 scale;

    Audio_PlayCue(SOUND_ITEM_BREAK);
    obj = Object_Spawn(215, x, y, z);
    if (obj != NULL) {
        obj->scale_y = 0x4000;
        obj->scale_x = 0x4000;
        obj->speed_limit = 0x30000;
        obj->acceleration = 0x30000;
        obj->auto_face_motion = 0;
        Object_SetMode(obj, 1);
        scale = obj->scale_x;
        if (scale < 0x10000) {
            do {
                scale += 0x800;
                obj->scale_y = scale;
                obj->scale_x = scale;
                obj->angle += 0x2000;
                WaitFrames(1);
                scale = obj->scale_x;
            } while (scale <= 0xffff);
        }
        obj->angle = angle;
    }
    return obj;
}
