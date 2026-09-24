#include "TYPES.H"
#include "FIELD_EVENT.H"

#define NULL ((void *)0)
#define CreateOverlayObject Func_020006de
void Effect_Move(void *object);

typedef struct {
    u8 pad[9];
    u8 lo:2;
    u8 field:2;
    u8 hi:4;
} Object_02000030;

union Slot {
    s32 w;
    u16 h[2];
    void *p;
};

extern u8 Data_02008778[];
extern u8 Data_02008868[];
extern u8 Data_020089c8[];
extern u8 Data_02008890[];
extern u8 Value_00001a40;
extern u8 Data_02008d4c[];
extern u8 Data_02008a28[];

void *Func_020006de(s32, s32, s32, s32);
void Func_020009b0(void *);
u16 *Func_020009d6(s32);
u16 *Func_02000a36(s32);
u16 *Func_02000a96(s32);
u16 *Func_02000afe(s32);
s32 Func_02000678(s32, s32, s32, s32);

void OverlayObject_SetEntryField(void *arg0, s32 arg1)
{
    Object_02000030 *obj = *(Object_02000030 **)((u8 *)arg0 + 0x50);

    obj->field = arg1;
}

void *OverlayObject_SpawnWithMode14(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    void *Func_02000686(s32, s32, s32, s32);

    u8 *ret = Func_02000686(arg3, arg0, arg1, arg2);

    if (ret != NULL) {
        u8 *obj = *(u8 **)(ret + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = obj[9];
        mask = -mask;
        mask &= flags;
        obj[9] = mask;
        ret[0x55] = 0;
        ret[0x59] = 8;
        Actor_SetSpriteFlags(ret, 0);
        Object_SetPalette(ret, 14);
        Object_SetBlendMode(ret, 1);
        return ret;
    }
    return NULL;
}

void *OverlayObject_CreateConfigured(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *ret = CreateOverlayObject(arg3, arg0, arg1, arg2);

    if (ret != NULL) {
        u8 *obj = *(u8 **)(ret + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = obj[9];
        mask = -mask;
        mask &= flags;
        mask |= 4;
        obj[9] = mask;
        ret[0x55] = 0;
        ret[0x59] = 8;
        Actor_SetSpriteFlags(ret, 0);
        Object_SetPalette(ret, 15);
        ret[0x23] = (ret[0x23] & 0xfe) | 2;
        return ret;
    }
    return NULL;
}

s32 SceneData_GetTable8778(void)
{
    return (s32)Data_02008778;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

s32 SceneData_GetTable8868(void)
{
    return (s32)Data_02008868;
}

s32 SceneData_SelectTable89c8Or8890(void)
{
    if (gGameState.entrance == 8) {
        return (s32)Data_020089c8;
    }
    Func_020009b0(Data_02008890);
    return (s32)Data_02008890;
}

void SceneDialogue_RunActor14FlaggedDialogue(void)
{
    u16 *state = Func_020009d6(0);
    u32 value = state[3];

    Event_Begin();
    if (value >= 0xa001 && value <= 0xdfff) {
        Shop_Open(16, 14);
    } else {
        if (GameFlag_IsSet(0x895) == 0) {
            Event_SetMessage(0x1817);
        } else {
            Event_SetMessage(0x1a46);
        }
        Event_ShowMessage(14, 0);
    }
    Event_End();
}

void SceneDialogue_RunActor15FlaggedDialogue(void)
{
    u16 *state = Func_02000a36(0);
    u32 value = state[3];

    Event_Begin();
    if (value >= 0xa001 && value <= 0xdfff) {
        Shop_Open(17, 15);
    } else {
        if (GameFlag_IsSet(0x895) == 0) {
            Event_SetMessage(0x1819);
        } else {
            Event_SetMessage(0x1a48);
        }
        Event_ShowMessage(15, 0);
    }
    Event_End();
}

void SceneDialogue_RunActor16FlaggedDialogue(void)
{
    void Event_ShowMessage();
    void Event_SetMessage(s32);

    u16 *state = Func_02000a96(0);
    u32 value = state[3];

    Event_Begin();
    if (value >= 0xa001 && value <= 0xdfff) {
        Shop_Open(18, 16);
    } else if (GameFlag_IsSet(0x895) == 0) {
        Event_SetMessage(0x181b);
        Event_ShowMessage(16, 0);
    } else {
        Event_SetMessage(0x1a4a);
        Event_AskYesNo(16, 0);
    }
    Event_End();
}

void SceneDialogue_RunActor17FlaggedDialogue(void)
{
    s32 GameFlag_IsSet(s32);

    u16 *state = Func_02000afe(0);
    u32 value = state[3];

    Event_Begin();
    if (value < 0x2000 || value > 0xe000) {
        Inn_Open(5, 17);
    } else {
        if (GameFlag_IsSet(0x895) == 0) {
            Event_SetMessage(0x181d);
        } else {
            Event_SetMessage(0x1a4e);
        }
        Event_ShowMessage(17, 0);
    }
    Event_End();
}

void SceneDialogue_RunActor10Dialogue(void)
{
    s32 Event_AskYesNo(s32, s32);

    Event_Begin();
    Event_SetMessage(0x1a3a);
    Event_AskYesNo(10, 0);
    Event_End();
}

void SceneDialogue_RunActor12Dialogue(void)
{
    void Event_SetMessage(s32);
    s32 Event_AskYesNo(s32, s32);

    Event_Begin();
    Event_SetMessage((s32)&Value_00001a40);
    Event_AskYesNo(12, 0);
    Event_End();
}

void SceneDialogue_RunActor9MotionDialogue(void)
{
    void Event_SetMessage(s32);

    Event_Begin();
    Event_SetMessage(0x1a64);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_FaceActor(9, 10, 0);
    Event_Wait(60);
    Actor_FaceActor(9, 0, 0);
    Event_Wait(20);
    Event_ShowMessage(9, 0);
    Event_End();
}

s32 SceneData_SelectTable8d4cOr8a28(void)
{
    if (gGameState.entrance == 8) {
        return (s32)Data_02008d4c;
    }
    return (s32)Data_02008a28;
}

s32 SceneState_SetRuntimeWord448To521(void)
{
    void Func_02000686_a(s32, s32, s32, s32);

    s16 scene;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    scene = gGameState.entrance;
    if (scene == 4 || scene == 7) {
        Func_02000686_a(0x00f80000, 0, 0x01a10000, 20);
    } else if (scene == 6) {
        Func_02000678(0x01cc0000, 0, 0x02380000, 20);
        Func_02000686_a(0x01e40000, 0, 0x02380000, 20);
    } else if (scene == 8) {
        GameFlag_Clear(0x12f);
        Actor_SetAnimation(10, 6);
    }
    return 0;
}
