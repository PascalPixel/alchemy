// near miss: differ 10 (listing 43 instructions, draft 43); scheduling: the native loads 0x0c08 before it builds the register address
#include "../../../../games/tla/src/scene/kibombo/kibombo.h"

extern struct KibomboObject *Data_02007950;

struct KibomboEffectWork {
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

struct KibomboObject *Func_02005a34(s32 type, s32 x, s32 y, s32 z);
void Func_02005a24(struct KibomboObject *object, s32 mode);
void Func_02005a3c(struct KibomboObject *object);
void ObjectDispatch_SetSingleChildFlags(struct KibomboObject *object, u32 value);
void Animation_SetStateFlags(struct KibomboObject *object, s32 flags);
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

void Func_02003184(void)
{
    do { } while (0);
    *(volatile u16 *)0x04000052 = 0x0c08;
    *(volatile u16 *)0x04000050 = 0x3f10;
    Func_02003104(0x1b80000, 0x280000, 0x1c20000);
    Func_02003104(0x1b80000, 0x2e0000, 0x1b60000);
    Func_02003104(0x1b80000, 0x340000, 0x1aa0000);
    Func_02003104(0x1b80000, 0x3a0000, 0x19e0000);
    Func_02003104(0x1b80000, 0x400000, 0x1920000);
}
