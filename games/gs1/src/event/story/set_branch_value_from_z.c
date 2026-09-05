#include "types.h"

struct Object {
    u8 filler00[8];
    s32 x;
    u8 filler0C[4];
    s32 z;
};

extern s16 Data_02000240[];

extern struct Object *Func_020047f6(u8 *);
extern struct Object *Func_02004806(s32);
extern void Func_020049d2(s32);

void StoryScene_SetBranchValueFromZ(
    u8 *actor_object, s32 val_lower,
    s32 val_other)
{
    u8 *scene_state;
    s16 *scene_table;
    struct Object *subject_actor;
    struct Object *other_actor;

    subject_actor = Func_020047f6(actor_object - 0x64);
    scene_table = Data_02000240;
    other_actor = Func_02004806(*(s32 *)&scene_table[250]);
    scene_state = *(u8 **)0x03001ebc;
    if (other_actor->z < subject_actor->z) {
        *(u16 *)(scene_state + 0x170) = val_lower;
    } else {
        *(u16 *)(scene_state + 0x170) = val_other;
    }
    Func_020049d2(0x7B);
}
