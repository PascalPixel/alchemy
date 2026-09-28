#include "MURA.H"

void OverlayObject_SpawnKind24AtActor(u8 *actor)
{
    u8 *obj;
    u8 *child;
    s32 mask;

    obj = (u8 *)Engine_ObjectCreate(24,
                            *(s32 *)(actor + 0x08),
                            *(s32 *)(actor + 0x0c),
                            *(s32 *)(actor + 0x10));
    if (obj == 0)
        return;

    child = *(u8 **)(obj + 0x50);
    Object_SetScript(obj, Mura_SpawnScript);
    *(u8 *)(obj + 0x55) = 0;
    *(u8 *)(obj + 0x22) = 1;
    *(u8 *)(obj + 0x23) = 2;
    if (child == 0)
        return;

    AnimationObjects_SelectAnimation(child, 2);
    *(u8 *)(child + 0x26) = 0;
    mask = 13;
    mask = -mask;
    *(u8 *)(child + 5) = (u8)((*(u8 *)(child + 5) & mask) | 4);
    *(u8 *)(child + 9) |= 12;
}

void FieldScene_DrawTilesByActor8Row(void)
{
    u8 *actor;
    actor = Actor_Get(8);
    if (actor == 0)
        return;

    {
        s32 tile_z = *(s32 *)(actor + 0x10) >> 20;
        if (tile_z == 6)
            Map_CopyCellAttributes(2, 0, 1, 1, 14, 6);
        else {
            s32 x = 14;
            s32 z = 6;
            Map_CopyCellAttributes(0, 0, 1, 1, x, z);
        }
    }

    {
        s32 tile_z = *(s32 *)(actor + 0x10) >> 20;
        if (tile_z == 9)
            Map_CopyCellAttributes(2, 0, 1, 1, 14, 9);
        else {
            s32 x = 14;
            s32 z = 9;
            Map_CopyCellAttributes(1, 0, 1, 1, x, z);
        }
    }
}
