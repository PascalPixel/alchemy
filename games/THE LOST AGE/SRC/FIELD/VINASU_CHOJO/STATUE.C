#include "TYPES.H"
#include "FIELD_EVENT.H"

/* The statue's two map states on the lighthouse top: each copies the same
   six rectangles from a different source column. */
void SceneState_ApplySixRectsAfter161(void)
{
    s32 x;
    s32 y;
    s32 a;
    s32 b;

    x = 23;
    y = 8;
    Engine_MapCopyCellAttributes(35, 8, 1, 3, x, y);
    a = 3;
    b = 1;
    Engine_MapCopyCellsTo(35, 8, 23, 8, b, a);
    Engine_MapCopyCellsTo(99, 8, 87, 8, b, a);
    x = 46;
    y = 55;
    Engine_MapCopyCellAttributes(57, 55, 3, 3, x, y);
    Engine_MapCopyCellsTo(57, 55, 46, 55, a, a);
    Engine_MapCopyCellsTo(121, 55, 110, 55, a, a);
}

void SceneState_ApplySixRectsAfterFlag161(void)
{
    s32 x;
    s32 y;
    s32 a;
    s32 b;

    x = 23;
    y = 8;
    Engine_MapCopyCellAttributes(36, 8, 1, 3, x, y);
    a = 3;
    b = 1;
    Engine_MapCopyCellsTo(36, 8, 23, 8, b, a);
    Engine_MapCopyCellsTo(100, 8, 87, 8, b, a);
    x = 46;
    y = 55;
    Engine_MapCopyCellAttributes(53, 55, 3, 3, x, y);
    Engine_MapCopyCellsTo(53, 55, 46, 55, a, a);
    Engine_MapCopyCellsTo(117, 55, 110, 55, a, a);
}
