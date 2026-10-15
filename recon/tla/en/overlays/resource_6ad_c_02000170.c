/* Near misses of overlay 6ad, measured with alchemy diff --symbol:
 * Func_02000170 differ 6: three loads sit one row from the listing (object->y twice, the position argument).
 * (Func_02000280 is C in s01f4.c; Func_020002b8 has its own draft.)
 * Func_020002ec differ 16: the listing keeps z in r0 where this draft uses r3. */
#include "../../../../games/tla/src/field/overlay_6ad/room.h"

extern char SceneId_Ov6adRoom1;
extern char SceneId_Ov6adRoom2;
extern char SceneId_Ov6adRoom3;
extern char SceneId_Ov6adRoom4;
extern const u8 Data_02006518[];
extern const u8 Data_02006530[];
extern const u8 Data_02006710[];
extern const u8 Data_02006890[];
extern const u8 Data_02006a0c[];

void Func_02000170(void)
{
    s32 position[3];
    struct ObjectRuntime *object = Object_GetById(gPartyState.current_owner);
    s32 angle = object->angle >> 12;

    angle = ((angle + 2) & 12) << 12;
    position[0] = object->x;
    position[1] = (object->y + 0x40000) & 0xfff80000;
    position[2] = object->z;
    Vector_AddPolar(0x100000, angle, position);
    object = Func_02005680(position, 1);
    if (object != NULL) {
        position[0] = object->x;
        position[1] = object->y;
        position[2] = object->z;
        Vector_AddPolar(0x100000, angle, position);
        if (Func_02005680(position, 1) != NULL)
            return;
    }
    Func_02005688();
}

void Func_020002ec(void)
{
    s32 id;

    Func_020054e8(53, 72, 1, 1, 38, 72);
    Func_020054e8(53, 72, 1, 1, 43, 72);
    Func_020054e8_(53, 8, 1, 1, 47, 8);
    for (id = 10; id <= 12; id++) {
        struct ObjectRuntime *object = Object_GetById(id);
        s32 x = object->x >> 20;
        s32 z = object->z >> 20;

        if (x == 38 && z == 8)
            Func_020054e8(54, 72, 1, 1, x, 72);
    }
    for (id = 10; id <= 12; id++) {
        struct ObjectRuntime *object = Object_GetById(id);
        s32 x = object->x >> 20;
        s32 z = object->z >> 20;

        if (x == 43 && z == 8)
            Func_020054e8(54, 72, 1, 1, x, 72);
    }
    {
        struct ObjectRuntime *object = Object_GetById(10);
        s32 x = object->x >> 20;
        s32 z = object->z >> 20;

        if (x == 47 && z == 8)
            Func_020054e8(54, 8, 1, 1, x, z);
    }
}

