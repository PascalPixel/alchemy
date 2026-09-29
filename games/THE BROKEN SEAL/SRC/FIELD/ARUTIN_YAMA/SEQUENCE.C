#include "YAMA.H"

void SceneEffect_SpawnObject222(void)
{
    extern s32 Data_03001e40;

    u8 *obj;

    if ((Data_03001e40 & 3) != 0) {
        return;
    }
    obj = (u8 *)Engine_ObjectCreate(222, ArutinYama_LeafOrigin[0], ArutinYama_LeafOrigin[1], ArutinYama_LeafOrigin[2]);
    if (obj == 0) {
        return;
    }
    {
        u16 *p = (u16 *)(obj + 100);
        u16 v = 30;
        *p = v;
    }
    {
        u16 *q = (u16 *)(obj + 102);
        u16 w = 1;
        *q = w;
    }
    *(s32 *)(obj + 104) = 20;
    SceneActor_SetMode3AndRate4ccc(obj);
    *(s32 *)(obj + 108) = (s32)ArutinYama_UpdateDriftingLeaf;
    Object_SetAnimation(obj, 1);
}

void SceneState_StoreParamsAndInstallTask(s32 v0, s32 v1, s32 v2, s32 v3)
{
    ArutinYama_LeafOrigin[0] = v0;
    ArutinYama_LeafOrigin[1] = v1;
    ArutinYama_LeafOrigin[2] = v2;
    ArutinYama_LeafMode = v3;

    BattleFx_SetQueuedSoundAndPlay(170);
    Engine_TaskAddCallback(SceneEffect_SpawnObject222, 0xc80);
}

void FieldScene_RunScene3a4SequenceG(void)
{
    extern u8 ArutinYama_RiseTimer[];

    u32 i;
    u8 *rec7;
    s32 record;
    s32 value;
    s32 base6_3001e40;

    base6_3001e40 = (u32)&gFrameCount;
    if (Value2(IwramUnsignedRemainderEntry, *(volatile s32 *)base6_3001e40, 3) == 0) {
        value = Value0(Engine_RandomNext);
        rec7 = Value4(Engine_ObjectCreate, 200, ((((u32)(((value << 1) + value) << 4) >> 16) << 16) + 0x2fd0000), -0x400000, 0x2600000);
        if ((s32)rec7 != 0) {
            if (Value2(IwramUnsignedRemainderEntry, *(volatile s32 *)base6_3001e40, 9) == 0) {
                {
                    s32 v2 = Random_Next();
                    if (((u32)(v2 << 1) >> 16) != 0) {
                        Audio_PlayCue(145);
                    } else {
                        Audio_PlayCue(144);
                    }
                }
            }
            rec7[85] = 0;
            {
                s32 v3 = Random_Next();
                s32 tmp2824 = (((u32)(v3 << 15) >> 16) + 0x4ccc);
                *(s32 *)(rec7 + 72) = 0x6666;
                *(s32 *)((s32)rec7 + 28) = tmp2824;
                *(s32 *)((s32)rec7 + 24) = tmp2824;
            }
            rec7[97] = 1;
            Actor_SetSpriteFlags((s32)rec7, 0);
            rec7[35] &= 254;
            {
                u8 *p80 = *(u8 **)(rec7 + 80);
                s32 mask9 = -13;
                p80[9] = (mask9 & p80[9]) | 4;
            }
            Object_SetAnimation((s32)rec7, 1);
            Object_SetScript((s32)rec7, ArutinYama_SparkScript);
            value = Value0(Engine_RandomNext);
            *(s32 *)(rec7 + 36) = ((((u32)(((value << 1) + value) << 1) >> 16) - 3) << 16);
            *(s32 *)(rec7 + 40) = 0x80000;
            value = Value0(Engine_RandomNext);
            *(s32 *)(rec7 + 44) = (((u32)(((value << 1) + value) << 9) >> 16) + -0x300);
        }
    }
}

void SceneState_ForwardByRuntimeSelector(s32 arg)
{
    extern s32 Data_03001e40;

    s32 sel = Data_03001e40 & 7;

    if (sel == 0) {
        Object_SetPalette(arg, 2);
    } else if (sel == 2) {
        Object_SetPalette(arg, 0);
    }
}

void FieldScene_RunActorTenFourStepSequence(void)
{
    Psynergy_Begin(24, 1);
    Psynergy_SetTarget(10, 9);
    Psynergy_RaiseHands();
    Actor_SetChildValue(10, 2);
    Psynergy_PlayEffect(1);
    Actor_SetChildValue(10, 2);
    Psynergy_LowerHands();
    Actor_SetChildValue(10, 2);
    BattleEffect_CleanupSceneObjects();
    Audio_PlayCue(288);
    Actor_SetChildValue(10, 2);
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    extern u8 ArutinYama_RiseTimer[];

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
    Event_Begin();
    Actor_SetPosition(10, 0, 0);
    Actor_SetPosition(8, 0, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Item_ShowFound(ITEM_FROST_JEWEL, 3);
    Party_GiveItem(ITEM_FROST_JEWEL, 0);
    *(u8 *)(Battle_GetWorkObject1e0() + 85) = 0;
    Camera_SetSpeed(0x19999, 0x3333);
    Camera_MoveTo(0x640000, 0, 0xf90000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    FieldScene_RunEarlySequence();
    Camera_MoveTo(*(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12), *(s32 *)(rec7 + 16), 1);
    Camera_WaitForMove();
    Call1(Event_LoadAreaScript, ArutinYama_OpenedAreaScript);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    Event_End();
}

void FieldScene_RunScene3a4SequenceF(void)
{
    extern u8 ArutinYama_RiseTimer[];

    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(8, 0x1480000, 0x1a80000);
    GameFlag_Set(0x323);
    FieldScene_RunScene3a4SequenceB();
    Map_Redraw();
    Task_Wait(1);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    Event_OpenScreen();
    Event_WaitForScreen();
    FieldScene_RunScene3a4SequenceH();
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    Event_End();
}

void FieldScene_RunScene3a4SequenceE(void)
{
    extern u8 ArutinYama_RiseTimer[];

    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetPosition(10, 0, 0);
    Actor_SetPosition(8, 0x1e80000, 0x8a0000);
    GameFlag_Set(0x325);
    FieldScene_RunScene3a4SequenceA();
    Map_Redraw();
    Task_Wait(1);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    Event_OpenScreen();
    Event_WaitForScreen();
    FieldScene_RunScene3a4SequenceI();
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    Event_End();
}

/*
 * Overlay resource_3a4. Per-frame tick that counts to sixty, fires one
 * audio cue and rewinds itself. The address of this routine is stored in
 * a record as a plain word, so it runs as a published callback.
 */

/* Plays a sound cue; the name is this site's own call word, not a runtime
 * address. */

/*
 * The owner spans the code, one alignment halfword and its one pool word,
 * thirty-six bytes in all. Engine_AudioPlayCue names the loader-relocated call
 * word for the cue call, not a runtime address. The limit of sixty reads
 * as one second of frames, but nothing here fixes a frame rate.
 */
void SceneAudio_PlayCue183EverySixtyTicks(void)
{
    ArutinYama_CueTicks = ArutinYama_CueTicks + 1;
    if (ArutinYama_CueTicks == 60) {
        Audio_PlayCue(183);
        ArutinYama_CueTicks = 0;
    }
}
