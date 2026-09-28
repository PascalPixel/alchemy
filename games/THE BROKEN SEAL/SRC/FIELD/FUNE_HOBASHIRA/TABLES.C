#include "FUNE.H"

s32 OverlayObject_ShrinkScaleThenStop(u8 *o)
{
    u8 *t = *(u8 **)(o + 0x50);

    t[9] |= 12;
    *(s32 *)(o + 48) = 0x20000;
    *(s32 *)(o + 52) = 0x10000;
    if (*(s32 *)(o + 24) > 0x1000) {
        *(s32 *)(o + 24) += 0xFFFFFC00;
        *(s32 *)(o + 28) += 0xFFFFFC00;
    } else {
        *(s32 *)(o + 8) = 0;
        *(s32 *)(o + 12) = 0;
        *(s32 *)(o + 16) = 0;
        *(s32 *)(o + 36) = 0;
        *(s32 *)(o + 40) = 0;
        *(s32 *)(o + 44) = 0;
    }
    return 1;
}

u8 *SceneData_GetTable95c0(void)
{
    return FuneHobashira_SceneTableA;
}

u8 *SceneData_GetTable9680(void)
{
    return FuneHobashira_SceneTableB;
}

u8 *SceneData_GetTable96a0(void)
{
    return FuneHobashira_SceneTableC;
}

u8 *SceneData_GetTable96C4(void)
{
    return FuneHobashira_SceneTableD;
}

u8 *SceneData_GetTable988c(void)
{
    return FuneHobashira_SceneTableE;
}

void FieldScene_CallHelper14d0(void)
{
    FieldEffect_UpdateGridPlacement();
}

void FieldScene_RunActor232SceneWhenFlag923Or922(void)
{
    if (GameFlag_IsSet(FLAG_MAST_923) != 0 || GameFlag_IsSet(FLAG_MAST_922) != 0) {
        Event_Begin();
        Item_ShowFound(ITEM_ANCHOR_CHARM, 3);
        Party_GiveItem(ITEM_ANCHOR_CHARM, 0);
        GameFlag_Set(FLAG_MAST_924);
        Event_End();
    }
}
