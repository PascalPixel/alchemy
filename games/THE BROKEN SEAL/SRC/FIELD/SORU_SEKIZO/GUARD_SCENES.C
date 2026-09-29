#include "STATUE_HALL.H"

/* The scrolling sprite rows: three rows of nine, after the seal scene. */
struct Ent SoruSekizo_SpriteRows[27];

void SceneEffect_UpdateScrollingSpriteRows(void)
{
    s32 *cp = &gMapWork->x;
    struct Ent *e = SoruSekizo_SpriteRows;
    s32 sx = cp[0] / 65536;
    s32 sy = 80 - cp[1] / 65536;
    s32 v;
    u32 i;

    if ((u32)(sy + 16) <= 175) {
        v = (SoruSekizo_ScrollPhase >> 10) - sx;
        v |= -32;
        {
            for (i = 0; i <= 8; i++) {
                e->f06 = v;
                e->f04 = sy;
                Runtime_PushSlotEntry(e, 0);
                v += 32;
                e++;
            }
        }
        v = (SoruSekizo_ScrollPhase >> 9) - sx;
        v |= -32;
        {
            for (i = 0; i <= 8; i++) {
                e->f06 = v;
                e->f04 = sy;
                Runtime_PushSlotEntry(e, 0);
                v += 32;
                e++;
            }
        }
        v = (SoruSekizo_ScrollPhase >> 8) - sx;
        v |= -32;
        {
            for (i = 0; i <= 8; i++) {
                e->f06 = v;
                e->f04 = sy + 8;
                Runtime_PushSlotEntry(e, 0);
                v += 32;
                e++;
            }
        }
    }
    SoruSekizo_ScrollPhase += 0x80;
}

void SceneState_RunWhenSlotZeroFacingC000(void)
{
    u16 *p = Engine_ActorGet(0);
    if (p[3] == 0xc000) {
        Leader_CheckAhead();
    }
}

void SceneState_RunWhenActorZeroFacing4000(void)
{
    u16 *p = Engine_ActorGet(0);
    if (p[3] == 0x4000) {
        Leader_CheckAhead();
    }
}

/* If the code-2059 check passes, runs a short setup/configuration sequence
 * for id 9: two no-argument calls bracket a select call and two calls each
 * taking a pair of numeric arguments. */
void FieldScene_RunPrimarySequenceHead(void)
{
    if (GameFlag_IsSet(GATE_CODE) == 0) {
        Event_Begin();
        Call1((void (*)())Engine_ActorGet, TARGET_ID);
        Actor_SetSpeed(TARGET_ID, 13107, 0x00001999); /* object_id, speed_limit, acceleration */
        Actor_WalkToAndWait(TARGET_ID, 504, 152); /* object_id, x=504, z=152 */
        Event_End();
    }
}

s32 Scene_RunGuardSequenceB(void)
{
    u8 *record;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    Scene_RunGuardSequenceC();
    GameFlag_Set(0x144);
    record = (u8 *)Value1(Engine_ActorGet, 18);
    record[89] = 0;
    {
        u8 flags = record[35] | 2;

        record[35] = flags;
    }
    Actor_SetSpriteFlags((s32)Engine_ActorGet(18), 0);
    *(u8 *)((s32)Engine_ActorGet(18) + 35) &= 254;
    Actor_SetSpritePriority(18, 1);
    if ((u32)((gCell[225][0] - 3) << 16) > 0x10000) {
        Actor_SetPosition(ACTOR_JASMINE, 0, 0);
        Actor_SetPosition(ACTOR_GERALD, 0, 0);
    }
    if (GameFlag_IsSet(0x818) != 0) {
        Actor_SetPosition(18, 0x1200000, 0xb20000);
        Actor_SetPosition(17, 0x6480000, 0x6480000);
        Actor_SetPosition(10, 0xe80000, 0x780000);
        Actor_SetPosition(12, 0x1580000, 0x780000);
        Actor_SetPosition(10, 0xe80000, 0x780000);
        Map_CopyCellsTo(0, 59, 15, 38, 4, 3);
        Actor_SetPosition(12, 0x1580000, 0x780000);
        Map_CopyCellsTo(4, 59, 17, 38, 4, 3);
        Map_CopyCellsTo(8, 60, 17, 39, 2, 2);
        Map_CopyCellAttributes(0, 1, 2, 1, 17, 7);
    } else if (GameFlag_IsSet(FLAG_LEFT_BEAM_SHINING) != 0
                && GameFlag_IsSet(FLAG_RIGHT_BEAM_SHINING) != 0) {
        Actor_SetPosition(10, 0xe80000, 0x780000);
        Actor_SetPosition(12, 0x1580000, 0x780000);
        Map_CopyCellsTo(0, 28, 17, 8, 2, 1);
        Actor_SetPosition(10, 0xe80000, 0x780000);
        Map_CopyCellsTo(0, 59, 15, 38, 4, 3);
        Actor_SetPosition(12, 0x1580000, 0x780000);
        Map_CopyCellsTo(4, 59, 17, 38, 4, 3);
        Map_CopyCellsTo(8, 60, 17, 39, 2, 2);
        Map_CopyCellAttributes(0, 0, 2, 1, 17, 8);
    } else {
        if (GameFlag_IsSet(FLAG_LEFT_BEAM_SHINING) != 0) {
            Actor_SetPosition(10, 0xe80000, 0x780000);
            Map_CopyCellsTo(0, 59, 15, 38, 4, 3);
        }
        if (GameFlag_IsSet(FLAG_RIGHT_BEAM_SHINING) != 0) {
            Actor_SetPosition(12, 0x1580000, 0x780000);
            Map_CopyCellsTo(4, 59, 17, 38, 4, 3);
        }
    }
    if (GameFlag_IsSet(0x80b) != 0) {
        Actor_SetPosition(9, 0x1f80000, 0x980000);
        Map_CopyCellsTo(2, 28, 34, 10, 2, 1);
        Map_CopyCellsTo(2, 30, 16, 10, 2, 1);
        Map_CopyCellsTo(0, 55, 32, 40, 4, 3);
    }
    if (GameFlag_IsSet(0x80c) != 0) {
        Actor_SetPosition(11, 0x2880000, 0x980000);
        Map_CopyCellsTo(4, 28, 36, 10, 2, 1);
        Map_CopyCellsTo(4, 30, 18, 10, 2, 1);
        Map_CopyCellsTo(4, 55, 36, 40, 4, 3);
    }
    if (GameFlag_IsSet(0x80d) != 0) {
        Actor_SetPosition(13, 0x1f80000, 0xc80000);
        Map_CopyCellsTo(2, 29, 34, 11, 2, 1);
        Map_CopyCellsTo(2, 31, 16, 11, 2, 1);
        Map_CopyCellsTo(0, 58, 32, 43, 4, 1);
    }
    if (GameFlag_IsSet(0x80e) != 0) {
        Actor_SetPosition(15, 0x2880000, 0xc80000);
        Map_CopyCellsTo(4, 29, 36, 11, 2, 1);
        Map_CopyCellsTo(4, 31, 18, 11, 2, 1);
        Map_CopyCellsTo(4, 58, 36, 43, 4, 1);
    }
    {
    s16 *state = (s16 *)gCell;

    if (state[225] == 3) {
        if (GameFlag_IsSet(0x30a) != 0) {
            Actor_SetPosition(ACTOR_GERALD, 0, 0);
            Actor_SetPosition(ACTOR_JASMINE, 0, 0);
        } else if (GameFlag_IsSet(0x109) == 0) {
            FieldScene_SetupStagedActors();
            GameFlag_Set(0x30a);
        }
    }
    if (state[225] == 4) {
        if (GameFlag_IsSet(0x30b) != 0) {
            Actor_SetPosition(ACTOR_GERALD, 0, 0);
            Actor_SetPosition(ACTOR_JASMINE, 0, 0);
        } else if (GameFlag_IsSet(0x109) == 0) {
            FieldScene_RunStagedActorScene();
            GameFlag_Set(0x30b);
        }
    }
    }
    if (GameFlag_IsSet(0x814) != 0) {
        BattleFx_SetQueuedSoundAndPlay(141);
        Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
        InitializeSceneRecordBuffer();
    }
    return 0;
}

void Scene_RunGuardSequenceC(void)
{
    u32 i;
    s32 value;
    s32 *p;
    s32 buf;

    p = (s32 *)SoruSekizo_SpriteRows;
    buf = Value2(Runtime_AllocateBlock, 14, 0x400);
    Call2(Resource_DecodeByteLz, (s32)SoruSekizo_SpriteRowsGfx, buf);
    value = Vram_Load(Resource_FindFreeEntry(), 128, buf);
    for (i = 0; i < 9; i++) {
        s32 *q = p;

        *q++ = 0;
        *q++ = 0x40004000;
        p += 3;
        *q = value | 0xac00;
    }
    value = Vram_Load(Resource_FindFreeEntry(), 128, buf + 128);
    for (i = 0; i < 9; i++) {
        s32 *q = p;

        *q++ = 0;
        *q++ = 0x40004000;
        p += 3;
        *q = value | 0xdc00;
    }
    value = Vram_Load(Resource_FindFreeEntry(), 128, buf + 0x100);
    for (i = 0; i < 9; i++) {
        s32 *q = p;

        *q++ = 0;
        *q++ = 0x40004000;
        p += 3;
        *q = value | 0xc00;
    }
    Heap_Release(14);
    {
        s32 size = 0xc80;

        Engine_TaskAddCallback((s32)SceneEffect_UpdateScrollingSpriteRows, size);
    }
}

void FieldScene_CallWhenCheck9_31_9(void)
{
    if (SceneActor_IsActorAtTile(9, 31, 9) != 0) {
        SceneData_InitTableA980();
    }
}

void FieldScene_RunGuardedStep11(void)
{
    if (SceneActor_IsActorAtTile(11, 40, 9) != 0) {
        SceneData_FillTableA980();
    }
}

void FieldScene_RunGuardedStep13(void)
{
    if (SceneActor_IsActorAtTile(13, 31, 12) != 0) {
        SceneData_InitTableA980AndRunB();
    }
}

void FieldScene_RunGuardedStep15(void)
{
    if (SceneActor_IsActorAtTile(15, 40, 12) != 0) {
        SceneData_BuildTableA980();
    }
}

void ConfigureSceneAndCheckActors(void)
{
    ConfigureScene(2, 0x00d00000, 0x00700000, 0);
    if (SceneActor_IsActorAtTile(10, 14, 7) != 0) {
        Scene_ShineLeftBeam();
    }
}

void ConfigureAlternateSceneAndCheckActors(void)
{
    ConfigureScene_02003a30(2, 23068672, 7340032, 0);
    if (SceneActor_IsActorAtTile(12, 21, 7) != 0) {
        Scene_ShineRightBeam();
    }
}

void FieldScene_RunClosingSequence(void)
{
    s32 first;
    s32 kind;
    s32 second;

    first = Value1(Engine_ActorGet, 0);
    kind = *(s32 *)(first + 8) >> 20;
    second = Value1(Engine_ActorGet, 0);
    if ((*(s32 *)(second + 16) >> 20) == 8) {
        if ((u32)(kind - 17) <= 1) {
            Call4(SetMapCellCollision, 2, 0x1100000, 0x800000, 255);
            Call4(SetMapCellCollision, 2, 0x1200000, 0x800000, 255);
        }
    }
}
