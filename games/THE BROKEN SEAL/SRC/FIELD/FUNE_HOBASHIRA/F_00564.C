#include "TYPES.H"
#include "FIELD_EVENT.H"

void Main_0808a030(s32 value);

/* The IWRAM field globals: the map work first, the event work at +0x4c. */
struct FieldGlobals {
    s32 **map;
    u8 unknown_04[0x48];
    struct EventWork *event;
};

extern struct FieldGlobals Data_03001e70;
extern s32 Data_02009938[2];
extern s32 Data_02009930[2];
extern u8 Value_020096f4;
extern u8 Value_0200939c;
extern u8 Value_02009314;
extern u8 Value_020080a5;

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void FuneHobashira_Func02000564(void)
{
    s32 *placement = *Data_03001e70.map;
    struct FieldActor *actor;
    struct EventWork **event;
    s32 x;
    s32 zero = 0;
    s32 scale;
    s32 update;

    Engine_EventBegin();
    Main_0808a030((s32)&Value_020096f4);
    Engine_TaskWait(1);
    Engine_ActorSetChildValue(0, 15);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(0), 0);
    Engine_ActorEnableActionCallback(8, &Value_0200939c);
    event = &Data_03001e70.event;
    (*event)->start_transition = 0x203;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Data_02009938[0] = *placement++;
    Data_02009938[1] = *placement;
    Call3((void (*)())Engine_ActorSetPosition, 9, 0x500000, 0xd20000);
    x = 0x500000;
    Engine_ActorGet(9)->motion_flags = zero;
    Data_02009930[0] = x;
    Data_02009930[1] = zero;
    Engine_ActorEnableActionCallback(9, &Value_02009314);
    Engine_EventWait(20);
    Engine_AudioPlayCue(29);
    Engine_GameFlagSet(0x8f0);
    Engine_ActorStop(8);
    Engine_TaskWait(1);
    Call3((void (*)())Engine_ActorShowEmote, 8, 0x100, 0);
    Engine_ActorFaceDirection(8, 0xb000, 0);
    Engine_EventSetMessage(0x1e3e);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Call3((void (*)())Engine_ActorSetPosition, 10, x, 0xd20000);
    Call3((void (*)())Engine_ActorSetPosition, 11, x, 0xd20000);
    Call3((void (*)())Engine_ActorSetPosition, 12, x, 0xd20000);
    Engine_ActorSetSpritePriority(10, 3);
    Engine_ActorSetSpritePriority(11, 3);
    Engine_ActorSetSpritePriority(12, 3);
    Engine_ActorSetChildValue(10, 3);
    Engine_ActorSetChildValue(11, 3);
    Engine_ActorSetChildValue(12, 3);
    actor = Engine_ActorGet(10);
    scale = 0x8000;
    update = (s32)&Value_020080a5;
    actor->scale_y = scale;
    actor->scale_x = scale;
    actor->update = (void (*)(union FieldObject *))update;
    actor = Engine_ActorGet(11);
    actor->scale_y = scale;
    actor->scale_x = scale;
    actor->update = (void (*)(union FieldObject *))update;
    actor = Engine_ActorGet(12);
    actor->scale_y = scale;
    actor->scale_x = scale;
    actor->update = (void (*)(union FieldObject *))update;
    Engine_TaskWait(1);
    Call3((void (*)())Engine_ActorSetSpeed, 10, 0x851e, 0x428f);
    Call3((void (*)())Engine_ActorSetSpeed, 11, 0x7333, 0x3999);
    Call3((void (*)())Engine_ActorSetSpeed, 12, 0x9999, 0x4ccc);
    Call3((void (*)())Engine_ActorSetDestination, 10, 128, 345);
    Call3((void (*)())Engine_ActorSetDestination, 11, 136, 330);
    Call3((void (*)())Engine_ActorSetDestination, 12, 156, 340);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(8, 2);
    Call3((void (*)())Engine_ActorWalkToAndWait, 8, 164, 344);
    Engine_ActorJump(8, 4, 10);
    Engine_ActorJump(8, 6, 40);
    Engine_ActorStartRepeatedMotion(8, 3);
    Engine_EventShowMessageAndWait(8, 0, 20);
    (*event)->start_transition = 0x202;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(11);
    Engine_EventEnd();
}
