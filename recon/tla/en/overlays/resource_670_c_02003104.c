// near miss: differ 4 (listing 57 instructions, draft 57); scheduling of the timer's address and of the update callback's store
#include "../../../../games/tla/src/field/overlay_670/ov670.h"

void Func_020030b8(struct Ov670Object *object);

extern struct Ov670Object *Data_02007950;

struct Ov670EffectWork {
    u8 unknown_000[0x7c4];
    s16 field_7c4;
};

void Func_02000624(void);
void Func_02000668(void);
void Func_020006ac(void);
void Func_020006f0(void);
void Func_0200057c(void);
void Func_02002c34(void);
void Func_02002ccc(void);
void Func_02002d64(void);
void Func_02003504(void);
void Func_020031f0(s32 id);
void Func_02003220(void);
void Func_02003898(void);
void Func_020038ec(void);

struct Ov670Object *Func_02005a34(s32 type, s32 x, s32 y, s32 z);
void Func_02005a24(struct Ov670Object *object, s32 mode);
void Func_02005a3c(struct Ov670Object *object);
void ObjectDispatch_SetSingleChildFlags(struct Ov670Object *object, u32 value);
void Animation_SetStateFlags(struct Ov670Object *object, s32 flags);
void Object_SetModeById(s32 actor, s32 mode);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Field_DispatchTypeHandler(s32 type);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02005bb4(void);
void Func_02005bc4(s32 countdown);
void Func_02005c14(s32 a, s32 b);
s32 Func_02005c1c(s32 a, s32 b);
void Func_02005c24(void);
void Func_02005c2c(void);
void Func_02005c34(void);

void Func_02003104(s32 x, s32 y, s32 z)
{
    struct Ov670Object *object = Func_02005a34(0x106, x, y, z);
    struct FieldSprite *sprite;

    if (object != 0) {
        object->motion_flags = 0;
        do { } while (0);
        object->priority_flags &= ~1;
        object->scale_y = 0x20000;
        object->field_64 = ((Random16Far() * 10) >> 16) + 10;
        object->field_66 = 0;
        object->update = Func_020030b8;
        sprite = object->sprite;
        do { } while (0);
        sprite->priority = 1;
        ObjectDispatch_SetSingleChildFlags(object, 0);
        Func_02005a24(object, 5);
        Animation_SetStateFlags(object, 1);
    }
}
