#include "types.h"

struct Object {
    u8 filler00[8];
    s32 x;
    u8 filler0C[4];
    s32 z;
};

extern s16 Data_02000240[];

extern struct Object *Func_020047a6(u8 *);
extern struct Object *Func_020047b6(s32);
extern void Func_02004982(s32);

void StoryScene_SetBranchValueFromX(
    u8 *actor_object, s32 value_when_other_x_lower,
    s32 value_when_other_x_not_lower)
{
    u8 *scene_state;
    s16 *scene_table;
    struct Object *subject_actor;
    struct Object *other_actor;

    subject_actor = Func_020047a6(actor_object - 0x64);
    scene_table = Data_02000240;
    other_actor = Func_020047b6(*(s32 *)&scene_table[250]);
    scene_state = *(u8 **)0x03001ebc;
    if (other_actor->x < subject_actor->x) {
        *(u16 *)(scene_state + 0x170) = value_when_other_x_lower;
    } else {
        *(u16 *)(scene_state + 0x170) = value_when_other_x_not_lower;
    }
    Func_02004982(0x7B);
}
