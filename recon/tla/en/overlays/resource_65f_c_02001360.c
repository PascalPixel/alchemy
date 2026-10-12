/* Draft of overlay 65f Func_02001360, a twin of DriftScene_SpawnObject
 * (field/common/drift_spawn.c) with a fifth argument stored at 0x5e.
 * differ 2 of 32: the listing loads the stack argument before the pooled
 * zero for next_frame; this draft loads the zero first. Storing next_frame
 * before the argument differs 9. */
#include "../../../../games/tla/src/field/overlay_65f/ov65f.h"
#include "field_sprite.h"

struct SpawnObject {
    u8 unknown_00[0x50];
    struct FieldSprite *sprite;
    u8 unknown_54;
    u8 motion_flags;
    u8 unknown_56[8];
    u16 unknown_5e;
};

extern u8 Data_02006330[];

void Func_02005164(struct SpawnObject *object, s32 mode);
void Func_0200516c(struct SpawnObject *object, const void *script);
struct SpawnObject *Func_02005174(s32 type, s32 x, s32 y, s32 z);

void Func_02001360(s32 x, s32 y, s32 z, s16 value, s32 timer)
{
    struct SpawnObject *object;
    struct FieldSprite *sprite;

    object = Func_02005174(0x1e8, x, y, z);
    if (object != 0) {
        sprite = object->sprite;
        Func_02005164(object, 1);
        Func_0200516c(object, Data_02006330);
        object->motion_flags = 0;
        object->unknown_5e = timer;
        sprite->next_frame = 0;
        sprite->rotation = value;
    }
}
