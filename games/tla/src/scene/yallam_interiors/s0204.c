#include "yallam_interiors.h"

u8 *Func_02000204(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov677Room1)
        return gData_02000ea8;
    if (scene == (s32)&SceneId_Ov677Room2)
        return gData_02000fe0;
    return gData_02000e84;
}

void Func_02000244(void)
{
    s32 msg;

    Func_020009b0();
    Func_02000a50(0);
    msg = (s32)MsgYallamInteriors10;
    Func_02000a18(msg);
    UiText_OpenMessageAtObject(9, 0);
    if (Inventory_PromptAndSetObjectMode(Func_02000a58(), 0) == 0)
    {
        Battle_WaitMode0(10);
        Func_02000a18(msg + 1);
    }
    else
    {
        Battle_WaitMode0(20);
        Func_02000a18(msg + 2);
    }
    Func_02000a28(9, 0);
    Func_020009b8();
}

void Func_0200029c(s32 actor)
{
    u16 facing = Object_GetById(gPartyState.currentOwner)->facing;

    /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ loads the facing before it builds the quadrant offset (2 differ without it). */
    do
    {
    } while (0);
    if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000)
    {
        Func_02000a68(20, actor);
    }
    else
    {
        Func_020009b0();
        Func_02000a50(0);
        Func_02000a18((s32)MsgYallamInteriors21);
        Func_02000a28(actor, 0);
        Func_020009b8();
    }
}

void Func_020002fc(s32 actor)
{
    u16 facing = Object_GetById(gPartyState.currentOwner)->facing;

    /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ loads the facing before it builds the quadrant offset (2 differ without it). */
    do
    {
    } while (0);
    if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000)
    {
        Func_02000a68(21, actor);
    }
    else
    {
        Func_020009b0();
        Func_02000a50(0);
        Func_02000a18((s32)MsgYallamInteriors23);
        Func_02000a28(actor, 0);
        Func_020009b8();
    }
}

void Func_0200035c(s32 actor)
{
    u16 facing = Object_GetById(gPartyState.currentOwner)->facing;

    /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ loads the facing before it builds the quadrant offset (2 differ without it). */
    do
    {
    } while (0);
    if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000)
    {
        Func_02000a68(22, actor);
    }
    else
    {
        Func_020009b0();
        Func_02000a50(0);
        Func_02000a18((s32)MsgYallamInteriors25);
        Func_02000a28(actor, 0);
        Func_020009b8();
    }
}

void Func_020003bc(s32 actor)
{
    u16 facing = Object_GetById(gPartyState.currentOwner)->facing;

    /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ loads the facing before it builds the quadrant offset (2 differ without it). */
    do
    {
    } while (0);
    if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000)
    {
        Func_02000a78(7, actor);
        GameFlag_ClearBit(0x301);
    }
    else
    {
        Func_020009b0();
        Func_02000a50(0);
        Func_02000a18((s32)MsgYallamInteriors27);
        Func_02000a28(actor, 0);
        Func_020009b8();
    }
}

void Func_02000424(s32 actor)
{
    u16 facing = Object_GetById(gPartyState.currentOwner)->facing;

    /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ loads the facing before it builds the quadrant offset (2 differ without it). */
    do
    {
    } while (0);
    if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000)
    {
        Func_02000a70(actor);
    }
    else
    {
        Func_020009b0();
        Func_02000a50(0);
        Func_02000a18((s32)MsgMikasallaInteriors27);
        Func_02000a28(actor, 0);
        Func_020009b8();
    }
}

void Func_02000480(void)
{
    s32 msg;

    Func_020009b0();
    Func_02000a50(0);
    ObjectMotion_SetVariantCallback(14, 2);
    msg = (s32)MsgYallamInteriors31;
    Func_02000a18(msg);
    UiText_OpenMessageAtObject(14, 0);
    if (Inventory_PromptAndSetObjectMode(Func_02000a58(), 0) == 0)
    {
        Battle_WaitMode0(10);
        Func_02000a18(msg + 1);
    }
    else
    {
        Battle_WaitMode0(20);
        Func_02000a18(msg + 2);
    }
    Func_02000a28(14, 0);
    Func_020009b8();
}
