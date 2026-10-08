/* Near miss (4 of 151 instructions differ): in each terrain call the
 * native builds the last argument (6) before it loads the object's x and z,
 * the draft between the two loads; one load swapped against a constant. */
#include "types.h"

struct Ov6a4Object {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[0xf];
    u8 priority_flags;
    u8 unknown_24[0x31];
    u8 motion_flags;
    u8 unknown_56[0xc];
    u8 state;
};

struct Ov6a4Object *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void Scene_SetArrivalFlags(s32 flags);
void Func_02001cb4(void);
void Func_02001dbc(s32 actor, s32 mode);
void Func_02002a48(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void Func_02002a50(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void Func_02002a78(s32 x, s32 z, s32 a, s32 b);
void Func_02002ab8(s32 actor, s32 x, s32 z);

s32 Func_02000604(void)
{
    struct Ov6a4Object *object;

    Scene_SetArrivalFlags(0);
    Func_02001cb4();
    if (GameFlag_Test(0xa0a)) {
        Func_02002ab8(13, 0, 0);
        Func_02002a50(66, 39, 1, 1, 70, 10);
        Func_02002a50(5, 75, 1, 1, 6, 75);
        Func_02002a48(5, 12, 1, 1, 6, 12);
        Func_02002a50(70, 39, 1, 1, 71, 10);
    } else {
        Func_02001dbc(13, 3);
        Object_GetById(13)->priority_flags |= 32;
    }
    object = Object_GetById(13);
    object->motion_flags = 0;
    Func_02002a78(object->x, object->z, 0, 6);
    if (GameFlag_Test(0xa0b)) {
        Func_02002ab8(14, 0, 0);
        Func_02002a50(68, 39, 1, 1, 121, 10);
        Func_02002a50(58, 75, 1, 1, 57, 75);
        Func_02002a48(58, 12, 1, 1, 57, 12);
        Func_02002a50(72, 39, 1, 1, 120, 10);
    } else {
        Func_02001dbc(14, 5);
        Object_GetById(14)->priority_flags |= 32;
    }
    object = Object_GetById(14);
    object->motion_flags = 0;
    Func_02002a78(object->x, object->z, 0, 6);
    Object_GetById(11)->state = 1;
    Object_GetById(12)->state = 1;
    return 0;
}
