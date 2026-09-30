#include "YAMA.H"
#include "CALL.H"

void FieldScene_RunScene3a4_02002310(void)
{
    extern struct GameState Data_02000240;

    if (GameFlag_IsSet(0x8fe) != 0) {
        *(u16 *)(*(u8 **)&gMapWork + 20) &= ~0x200;
        Actor_SetPosition(9, 0, 0);
    } else {
        ArutinYama_StartPaletteAnim();
        if (GameFlag_IsSet(0x109) == 0 && Data_02000240.entrance == 99) {
            FieldScene_RunScene3a4SequenceF();
        } else {
            Map_CopyCellAttributes(38, 24, 1, 2, 37, 24);
            Map_CopyCellAttributes(44, 23, 1, 2, 45, 23);
            if (GameFlag_IsSet(0x8fe) == 0) {
                Actor_SetChildValue(9, 2);
                Actor_SetAnimation(9, 3);
                SceneState_StoreParamsAndInstallTask(0xee0000, 0, 0x1a20000, 0x8000);
            }
        }
    }
    if (GameFlag_IsSet(0x323) != 0) {
        Map_CopyCellAttributes(0, 0, 1, 1, 24, 80);
        Map_CopyCellsTo(0, 1, 24, 11, 1, 2);
    } else {
        Map_CopyCellAttributes(2, 0, 1, 1, 24, 80);
        Map_CopyCellsTo(2, 1, 24, 11, 1, 2);
    }
}

void FieldScene_RunScene3a4_02002428(void)
{
    extern u8 ArutinYama_RiseTimer[];

    extern u8 Data_02000240[];

    if (GameFlag_IsSet(0x8fe) != 0) {
        *(u16 *)(*(u8 **)&gMapWork + 20) &= ~0x200;
    } else {
        Map_CopyCellAttributes(52, 42, 1, 1, 53, 42);
    }
    {
        s32 index = 225;
        if ((u32)((((u16 *)Data_02000240)[index] - 6) << 16) <= 0x10000) {
            GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
        }
    }
}

void FieldScene_RunScene3a4_02002490(void)
{
    extern u8 ArutinYama_RiseTimer[];

    u32 i;
    u8 *record;

    if (GameFlag_IsSet(0x907) != 0) {
        *(u16 *)(*(u8 **)&gMapWork + 20) &= ~0x200;
        Actor_SetPosition(10, 0, 0);
    } else {
        if (GameFlag_IsSet(0x109) == 0) {
            if (gGameState.entrance == 99) {
                FieldScene_RunScene3a4SequenceE();
            }
        }
        ArutinYama_StartPaletteAnim();
        if (GameFlag_IsSet(0x907) == 0) {
            Actor_SetChildValue(10, 2);
            Actor_SetAnimation(10, 3);
            SceneState_StoreParamsAndInstallTask(0x2ec0000, 0x80000, 0x1180000, 0x8000);
        }
    }
    SceneActor_ClearCollisionFlagAndPlaceMarker(9);
    if (GameFlag_IsSet(0x200) != 0) {
        Actor_SetAnimation(9, 5);
        Map_CopyCellAttributes(23, 13, 1, 1, 25, 13);
        {
            u8 *record = Actor_Get(9);
            u8 flags = record[35] | 2;

            record[35] = flags;
        }
    }
    if (GameFlag_IsSet(0x325) != 0) {
        Map_CopyCellAttributes(10, 72, 1, 1, 11, 73);
        Map_CopyCellsTo(49, 32, 11, 4, 1, 2);
    } else {
        Map_CopyCellAttributes(12, 72, 1, 1, 11, 73);
        Map_CopyCellsTo(48, 32, 11, 4, 1, 2);
    }
}

void FieldScene_RunScene3a4_020025c0(void)
{
    extern u8 ArutinYama_RiseTimer[];

    u32 i;
    u8 *rec7;
    s32 record;

    if (gGameState.entrance == 2) {
        if (GameFlag_IsSet(0x109) == 0) {
            Actor_SetPosition(8, 0x1660000, 0x680000);
        }
    }
    SceneActor_ClearCollisionFlagAndPlaceMarker(9);
    if (GameFlag_IsSet(0x200) != 0) {
        rec7 = Actor_Get(9);
        Actor_SetAnimation(9, 5);
        Map_CopyCellAttributes(45, 41, 1, 1, 43, 41);
        {
            u8 flags = rec7[35] | 2;

            rec7[35] = flags;
        }
    }
    if (GameFlag_IsSet(0x907) != 0) {
        *(u16 *)(*(u8 **)&gMapWork + 20) &= ~0x200;
    }
    if (GameFlag_IsSet(0x326) != 0) {
        Map_CopyCellAttributes(17, 93, 1, 1, 16, 92);
        Map_CopyCellsTo(46, 29, 16, 28, 1, 2);
    } else {
        Map_CopyCellAttributes(15, 93, 1, 1, 16, 92);
        Map_CopyCellsTo(47, 29, 16, 28, 1, 2);
    }
}

void FieldScene_RunScene3a4_020026c0(void)
{
    extern u8 ArutinYama_RiseTimer[];

    u32 i;
    u8 *record;

    record = Actor_Get(9);
    Actor_SetSpriteFlags((s32)record, 0);
    SceneActor_UpdateSlot10ByTileX();
    SceneActor_ClearCollisionFlagAndPlaceMarker(9);
    if (GameFlag_IsSet(0x200) != 0) {
        Actor_SetAnimation(9, 5);
        Map_CopyCellAttributes(0, 0, 1, 1, 26, 26);
        {
            u8 *record = Actor_Get(9);
            u8 flags = record[35] | 2;

            record[35] = flags;
        }
    }
    SceneActor_ClearCollisionFlagAndPlaceMarker(11);
    if (GameFlag_IsSet(0x201) != 0) {
        ((void (*)())Engine_ActorSetAnimation)(11, 5);
        Map_CopyCellAttributes(1, 0, 1, 1, 17, 10);
        {
            u8 *record = Actor_Get(11);
            u8 flags = record[35] | 2;

            record[35] = flags;
        }
    }
    SceneActor_ClearCollisionFlagAndPlaceMarker(12);
    if (GameFlag_IsSet(0x204) != 0) {
        Actor_SetAnimation(12, 5);
        Map_CopyCellAttributes(1, 0, 1, 1, 26, 15);
        {
            u8 *record = Actor_Get(12);
            u8 flags = record[35] | 2;

            record[35] = flags;
        }
    }
    Engine_TaskAddCallback(SceneActor_SetActor12ModeByActorZeroHeight, 0xc80);
    if (GameFlag_IsSet(0x327) != 0) {
        Call6((void (*)())Engine_MapCopyCellAttributes, 30, 82, 1, 1, 29, 81);
        Map_CopyCellsTo(46, 28, 29, 17, 1, 2);
    } else {
        Map_CopyCellAttributes(28, 82, 1, 1, 29, 81);
        Map_CopyCellsTo(47, 28, 29, 17, 1, 2);
    }
}
