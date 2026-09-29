#include "HASHIRA.H"

s32 SceneActor_ApplyPlacementQueryAndTag(u8 *no)
{
    u8 *obj = Actor_Get(no);
    u32 out20, out16;
    s32 out12, out8;
    s32 rec[6];
    s32 r2, r4;
    u8 mask;

    if (FieldScene_QueryActorFootprint(no, &out20, &out16, rec, &out12, &out8) == 0) {
        return 0;
    }

    r2 = rec[2];
    r4 = rec[4];
    Map_CopyCellAttributes(2, 2, out20, out16, r2, r4);

    Stage_SetMode(obj, 4);
    mask = 2;
    obj[0x23] = obj[0x23] | mask;

    if (out20 > out16) {
        Map_CopyCellsTo(70, 40, rec[2] + 32, rec[4] + 2, out20, out16);
    } else {
        Map_CopyCellsTo(68, 40, rec[2] + 32, rec[4] + 2, out20, out16);
    }

    return 1;
}

s32 SceneActor_ApplyPlacementQuery(u8 *no)
{
    u8 *obj = Actor_Get(no);
    s32 out20, out16, out12, out8;
    s32 rec[6];

    if (FieldScene_QueryActorFootprint(no, &out20, &out16, rec, &out12, &out8) == 0) {
        return 0;
    }

    {
        s32 x = out12 + rec[2];
        s32 z = out8 + rec[4];

        Map_CopyCellAttributes(x, z, out20, out16, rec[2], rec[4]);
        StagedActor_FillGridAttributeRectangle(0, rec[2], rec[4], out20, out16, 255);
    }

    Stage_SetMode(obj, 1);
    obj[0x23] &= 0xfd;

    return 1;
}
