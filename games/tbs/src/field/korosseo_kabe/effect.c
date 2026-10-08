#include "task.h"
#include "text/msg_ids.h"



void Text_WriteU32AsHex(u8 *buf, u32 value)
{
    s32 i;

    buf += 8;
    *buf = 0;
    buf--;
    for (i = 7; i >= 0; i--) {
        *buf = HexDigits[value & 15];
        value >>= 4;
        buf--;
    }
}

void SceneState_EmptyHook(void)
{
}

void KorosseoKabe_MarkStageReady(void)
{
    ((struct StageControl *)gSceneState)->status = 9;
}

void SceneState_WaitUntilStatusNine(void)
{
    s16 *status = &((struct StageControl *)gSceneState)->status;

    while (*status != 9) {
        Engine_TaskWait(1);
    }
}

void KorosseoKabe_SpawnRandomSceneEffect(SparkSource *a)
{
    s32 t[3];
    u32 n;

    if (a->f28 >= -255 && a->f28 <= 255) {
        a->f55 = 0;
    }
    n = Engine_RandomNext();
    if (n * 100 >> 16 <= 9) {
        SparkSource *o;
        s32 u;
        s32 w;

        t[0] = a->f08;
        t[1] = a->f0c;
        t[2] = a->f10;
        u = Engine_RandomNext();
        w = Engine_RandomNext();
        Vector_AddPolarOffset(u << 4, w, t);
        {
            s32 x = t[0];
            s32 y = t[1];
            s32 z = t[2];

            o = (SparkSource *)Engine_ObjectCreate(285, x, y, z);
        }
        if (o != 0) {
            o->f55 = 0;
            Engine_ActorSetSpriteFlags(o, 0);
            Engine_ObjectSetScript(o, KorosseoKabe_SparkScript);
            Object_SetMode(o, 1);
            Object_SetMode(o, 0);
        }
    }
}

s32 KorosseoKabe_RaiseLinkedSceneEffect(RaisedEffect *a)
{
    RaisedEffect *o = (RaisedEffect *)Object_GetById(a->f64);

    Engine_ObjectSetPosition(o, a->f08, a->f0c + 0x240000, a->f10);
    o->f55 = 0;
    Engine_ObjectSetScript(o, KorosseoKabe_RaiseScript);
    Engine_AudioPlayCue(83);
    a->f64 = 0;
    return 0;
}

s32 FieldScene_RunFlag211ApproachScene(s32 handleA, s32 handleB)
{
    u8 *workspace = *(u8 **)(gWorkSlot + WORK_SLOT_STAGE * 4);
    u8 *shared;
    struct FieldActor *record;
    s32 flag;
    s32 x;
    s32 z;
    u16 *cuep;
    s16 *waitp;

    flag = Engine_GameFlagIsSet(0x211);

    shared = (u8 *)&gGameState;
    record = Object_GetById(((struct GameState *)shared)->selected_actor);

    if (*(s32 *)(workspace + 232) < record->x.fixed) {
        x = *(s32 *)(workspace + 232) + 0xc0000;
    } else {
        x = *(s32 *)(workspace + 232) - 0xc0000;
    }

    if (flag != 0) {
        z = *(s32 *)(workspace + 236) + 0x100000;
        cuep = (u16 *)(workspace + 228);
    } else {
        z = *(s32 *)(workspace + 236) - 0x100000;
        cuep = (u16 *)(workspace + 226);
    }

    waitp = (s16 *)&record->timer;
    *waitp = *cuep;
    record->acceleration = 0x4000;
    record->speed = 0x10000;

    Engine_ObjectSetPosition(record, x, 0, z);
    Engine_GameFlagSet(0x211);
    Engine_ObjectSetScript(record, KorosseoKabe_ApproachScript);

    while (*waitp != 0) {
        Engine_TaskWait(1);
    }

    if (flag == 0) {
        ((s32 (*)())OverlayObject_NotifyMatchingEntries)(0, handleA);
        UiWork_PushValueSlot(handleA, 2);
    } else {
        ((s32 (*)())OverlayObject_NotifyMatchingEntries)(0, handleB);
        UiWork_PushValueSlot(handleB, 2);
    }

    shared = (u8 *)&gGameState;
    UiWork_PushValueSlot(((struct GameState *)shared)->selected_actor, 1);
    Engine_MessageShowCentered((s32)MsgKorosseoRobinGotItem, 3);
    ObjectDispatch_WaitForValue16(record);

    return flag;
}
