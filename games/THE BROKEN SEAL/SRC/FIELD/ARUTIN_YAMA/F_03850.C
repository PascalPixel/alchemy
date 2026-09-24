#include "TYPES.H"
#include "FIELD_EVENT.H"

void Main_08000128(s32 distance, s32 angle, union FieldCoordinate *pos);
s32 Main_080091a8(s32 layer, s32 x, s32 z);
s32 Main_080091b0(s32 layer, s32 x, s32 z);
void Main_08009048(struct FieldSprite *sprite, s32 mode);
struct FieldActor *Main_0808a400(s32 actor);
void Engine_ObjectCommitPosition(struct FieldActor *object);
void SceneState_SetRecordWord102AndPlayCue288();
void Local_020035ac(struct FieldActor *object);
void Local_02003668(struct FieldActor *object);
void ArutinYama_Func02003484(struct FieldActor *object);
void ArutinYama_Func02003738(struct FieldActor *object);

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

struct Byte {
    u8 v;
};

/* The rolling-object driver: finds a heading whose step keeps the object
 * level, starts the roll, then dispatches on the tile under it each frame
 * until tile 99. */
void ArutinYama_Func02003850(s32 id, s32 heading)
{
    struct FieldActor *object = Engine_ActorGet(id);
    struct FieldActor *leader = Main_0808a400(gGameState.selected_actor);
    union FieldCoordinate pos[3];
    union FieldCoordinate *p;
    struct Byte zero;
    s32 n;

    Engine_EventBegin();
    if (heading == -1) {
        heading = object->facing;
    }
    for (n = 0; n <= 3; n++) {
        pos[0].fixed = object->x.fixed;
        pos[1].fixed = object->y.fixed;
        pos[2].fixed = object->z.fixed;
        Main_08000128(0x100000, heading, pos);
        if (Main_080091a8(2, pos[0].fixed, pos[2].fixed) == object->y.fixed) {
            break;
        }
        heading += 0x4000;
    }
    if (n == 4) {
        return;
    }
    object->unknown_22 = 2;
    leader->x.fixed = 0;
    leader->z.fixed = 0;
    Main_08009048(object->sprite, 16);
    Engine_CameraFollowActor(id, 1);
    Engine_CameraWaitForMove();
    Call2((void (*)())Engine_CameraSetSpeed, 0x100000, 0x20000);
    object->facing = heading;
    object->speed = 0x20000;
    object->acceleration = 0xccc;
    zero.v = 0;
    object->unknown_5b = zero.v;
    object->unknown_64 = object->y.fixed / 0x10000;
    object->unknown_66 = 0;
    pos[0].fixed = object->x.fixed;
    pos[1].fixed = object->y.fixed;
    pos[2].fixed = object->z.fixed;
    Main_08000128(0x180000, heading, pos);
    Engine_ObjectSetPosition(object, pos[0].fixed, object->y.fixed, pos[2].fixed);
    Engine_ObjectCommitPosition(object);
    Engine_AudioPlayCue(233);
    for (;;) {
        switch (Main_080091b0(2, object->x.fixed, object->z.fixed)) {
        case 98:
            Local_020035ac(object);
            break;
        case 97:
            Local_02003668(object);
            break;
        case 96:
            SceneState_SetRecordWord102AndPlayCue288(object);
            break;
        case 99:
            goto arrived;
        }
        ArutinYama_Func02003738(object);
        Engine_TaskWait(1);
    }
arrived:
    ArutinYama_Func02003484(object);
    Engine_EventEnd();
}
