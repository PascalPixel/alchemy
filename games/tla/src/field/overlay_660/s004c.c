#include "ov660.h"

const u8 *Func_0200004c(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov660Room1)
        return Data_02000964;
    if (scene == (s32)&SceneId_Ov660Room2)
        return Data_02000ab4;
    if (scene == (s32)&SceneId_Ov660Room3)
        return Data_02000b2c;
    return Data_0200094c;
}

void Func_0200009c(s32 actor)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.current_owner);

    /* FAKEMATCH: the one-pass block keeps the angle load before the constant; as plain statements the constant is built first (differ 2) */
    do {
        u16 facing = (leader->angle + 0x2000) & ~0x3fff;

        if (facing == 0xc000 && GameFlag_Test(0x91b)) {
            Func_0200089c(10, actor);
        } else {
            if (GameFlag_Test(0x91b))
                Func_02000844((s32)MsgScene660No52);
            else
                Func_02000844((s32)MsgScene660No17);
            Func_0200085c(actor, 0);
        }
    } while (0);
}

void Func_02000114(s32 actor)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.current_owner);

    /* FAKEMATCH: the one-pass block keeps the angle load before the constant; as plain statements the constant is built first (differ 2) */
    do {
        u16 facing = (leader->angle + 0x2000) & ~0x3fff;

        if (facing == 0xc000 && GameFlag_Test(0x91b)) {
            Func_0200089c(11, actor);
        } else {
            if (GameFlag_Test(0x91b))
                Func_02000844((s32)MsgScene660No54);
            else
                Func_02000844((s32)MsgScene660No19);
            Func_0200085c(actor, 0);
        }
    } while (0);
}

void Func_0200018c(s32 actor)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.current_owner);

    /* FAKEMATCH: the one-pass block keeps the angle load before the constant; as plain statements the constant is built first (differ 2) */
    do {
        u16 facing = (leader->angle + 0x2000) & ~0x3fff;

        if (facing == 0xc000) {
            Func_0200089c(12, actor);
        } else if (GameFlag_Test(0x91b)) {
            Func_02000844((s32)MsgScene660No56);
            Func_0200085c(actor, 0);
        } else {
            s32 message = (s32)MsgScene660No21;

            Func_02000844(message);
            UiText_OpenMessageAtObject(actor, 0);
            if (Inventory_PromptAndSetObjectMode(Func_0200088c(), 0) == 0) {
                Battle_WaitMode0(10);
                Func_02000844(message + 1);
            } else {
                Battle_WaitMode0(20);
                Func_02000844(message + 2);
            }
            Func_0200085c(actor, 0);
        }
    } while (0);
}

void Func_02000230(s32 actor)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.current_owner);

    /* FAKEMATCH: the one-pass block keeps the angle load before the constant; as plain statements the constant is built first (differ 2) */
    do {
        u16 facing = (leader->angle + 0x2000) & ~0x3fff;

        if (facing == 0xc000) {
            Func_020008ac(3, actor);
        } else {
            if (GameFlag_Test(0x91b))
                Func_02000844((s32)MsgScene660No58);
            else
                Func_02000844((s32)MsgScene660No23);
            Func_0200085c(actor, 0);
        }
    } while (0);
}
