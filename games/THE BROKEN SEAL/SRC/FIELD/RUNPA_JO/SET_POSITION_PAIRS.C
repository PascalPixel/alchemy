#include "FORTRESS.H"
#include "CALL.H"

/*
 * The Lunpa Fortress bridge: pairs of position words for the movable
 * supports. Each indexed pair is a left and a right support that travel
 * together; the top pair stays level while the second is raised or lowered
 * in steps, and the lowest pair is only written once the pair index says it
 * is the bottom of the run.
 */

/* The support pairs, labelled where they lie among the overlay's data. */
extern s32 gRunpaJoSupportPairs[];

void FieldScene_SetPositionPairs(s32 idx)
{
    s32 top_x;
    s32 top_y;
    s32 bottom_y;

    top_x = gRunpaJoSupportPairs[idx * 2];
    top_y = gRunpaJoSupportPairs[idx * 2 + 1];
    Engine_MapCopyCells(0, 77, 1, 3, top_x, top_y);
    Engine_MapCopyCells(1, 77, 1, 1, top_x + 1, top_y);
    bottom_y = top_y - 44;
    Map_CopyCellAttributeRect(top_x, top_y - 45, 1, 1, top_x, bottom_y);
    if (idx == 1)
        Call6(Map_CopyCellAttributeRect, top_x, bottom_y, 1, 1, top_x, top_y - 43);
}
