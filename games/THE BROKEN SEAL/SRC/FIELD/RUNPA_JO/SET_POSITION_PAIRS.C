#include "FORTRESS.H"

/*
 * The Lunpa Fortress bridge: pairs of position words for the movable
 * supports. Each indexed pair is a left and a right support that travel
 * together; the top pair stays level while the second is raised or lowered
 * in steps, and the lowest pair is only written once the pair index says it
 * is the bottom of the run.
 */

/* The support pairs, labelled where they lie among the overlay's data. */
extern s32 gRunpaJoSupportPairs[];

static __inline__ void FieldPair_Call6(
    void (*func)(s32, s32, s32, s32, s32, s32),
    s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    func(a0, a1, a2, a3, a4, a5);
}

void FieldScene_SetPositionPairs(s32 idx)
{
    s32 top_x;
    s32 top_y;
    s32 bottom_y;

    top_x = gRunpaJoSupportPairs[idx * 2];
    top_y = gRunpaJoSupportPairs[idx * 2 + 1];
    FieldPair_Call6(Engine_MapCopyCells, 0, 77, 1, 3, top_x, top_y);
    FieldPair_Call6(Engine_MapCopyCells, 1, 77, 1, 1, top_x + 1, top_y);
    bottom_y = top_y - 44;
    FieldPair_Call6(Map_CopyCellAttributeRect, top_x, top_y - 45, 1, 1, top_x, bottom_y);
    if (idx == 1)
        FieldPair_Call6(Map_CopyCellAttributeRect, top_x, bottom_y, 1, 1, top_x, top_y - 43);
}
