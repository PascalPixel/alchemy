#include "TASK.H"

extern u8 MsgKorosseoRobinGotItem[];

/* The game state's cells, read here as words. */
extern s32 gCell[];

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

/* Complete two-byte empty hook plus its alignment halfword. */
void SceneState_EmptyHook(void)
{
}

/* Mark the stage's control record finished. */
void SceneState_SetHalfword1000To9(void)
{
    ((struct StageControl *)gSceneState)->status = 9;
}

/* Wait until the stage's control record is marked finished. */
void SceneState_WaitUntilStatusNine(void)
{
    s16 *status = &((struct StageControl *)gSceneState)->status;

    while (*status != 9) {
        Task_Wait(1);
    }
}

void KorosseoKabe_SpawnRandomSceneEffect(SparkSource *a)
{
    s32 t[3];
    u32 n;

    if (a->f28 >= -255 && a->f28 <= 255) {
        a->f55 = 0;
    }
    n = Random_Next();
    if (n * 100 >> 16 <= 9) {
        SparkSource *o;
        s32 u;
        s32 w;

        t[0] = a->f08;
        t[1] = a->f0c;
        t[2] = a->f10;
        u = Random_Next();
        w = Random_Next();
        Vector_AddPolarOffset(u << 4, w, t);
        {
            s32 x = t[0];
            s32 y = t[1];
            s32 z = t[2];

            o = (SparkSource *)Engine_ObjectCreate(285, x, y, z);
        }
        if (o != 0) {
            o->f55 = 0;
            Actor_SetSpriteFlags(o, 0);
            Object_SetScript(o, KorosseoKabe_SparkScript);
            Object_SetAnimation(o, 1);
            Object_SetAnimation(o, 0);
        }
    }
}

s32 KorosseoKabe_RaiseLinkedSceneEffect(RaisedEffect *a)
{
    RaisedEffect *o = (RaisedEffect *)Engine_ActorGet(a->f64);

    Engine_ObjectSetPosition(o, a->f08, a->f0c + 0x240000, a->f10);
    o->f55 = 0;
    Object_SetScript(o, KorosseoKabe_RaiseScript);
    Audio_PlayCue(83);
    a->f64 = 0;
    return 0;
}

s32 FieldScene_RunFlag211ApproachScene(s32 handleA, s32 handleB)
{
    u8 *workspace = *(u8 **)(gWorkSlot + WORK_SLOT_STAGE * 4);
    u8 *shared;
    u8 *record;
    s32 flag;
    s32 x;
    s32 z;
    u16 *cuep;
    s16 *waitp;

    flag = GameFlag_IsSet(0x211);

    shared = (u8 *)gCell;
    record = (u8 *)Engine_ActorGet(*(s32 *)(shared + 500));

    if (*(s32 *)(workspace + 232) < *(s32 *)(record + 8)) {
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

    waitp = (s16 *)(record + 100);
    *waitp = *cuep;
    *(s32 *)(record + 52) = 0x4000;
    *(s32 *)(record + 48) = 0x10000;

    Engine_ObjectSetPosition(record, x, 0, z);
    GameFlag_Set(0x211);
    Object_SetScript(record, KorosseoKabe_ApproachScript);

    while (*waitp != 0) {
        Task_Wait(1);
    }

    if (flag == 0) {
        ((s32 (*)())OverlayObject_NotifyMatchingEntries)(0, handleA);
        UiWork_PushValueSlot(handleA, 2);
    } else {
        ((s32 (*)())OverlayObject_NotifyMatchingEntries)(0, handleB);
        UiWork_PushValueSlot(handleB, 2);
    }

    shared = (u8 *)gCell;
    UiWork_PushValueSlot(*(s32 *)(shared + 500), 1);
    Message_ShowCentered((s32)MsgKorosseoRobinGotItem, 3);
    ObjectDispatch_WaitForValue16(record);

    return flag;
}
