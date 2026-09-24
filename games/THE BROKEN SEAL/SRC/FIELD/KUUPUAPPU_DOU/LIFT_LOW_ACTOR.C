#include "TYPES.H"

struct SceneActorRecord {
    u8 reserved_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 reserved_14[20];
    s32 field_28;
    u8 reserved_2c[28];
    s32 field_48;
};

struct SceneActorRecord *Func_02001a78(s32 actor);
struct SceneActorRecord *Func_02001a84(s32 actor);

#define TILE(position) ((position) / 0x100000)


/* Checks actors 11 to 14 in turn against the subject actor. The first one
 * whose y lies above 0 and below one tile, and that stands on the subject's
 * tile, is moved up to y 255 with fields 0x28 and 0x48 cleared, and the check
 * returns 1; otherwise it returns 0. */
s32 SceneActor_LiftLowActorOnSubjectTile(s32 subject_actor)
{
    struct SceneActorRecord *subject = Func_02001a78(subject_actor);
    s32 index = 0;

    do {
        struct SceneActorRecord *actor = Func_02001a84(index + 11);

        if ((u32)(actor->y - 1) <= 0x000ffffe) {
            s32 actor_z = TILE(actor->z);
            s32 actor_x = TILE(actor->x);
            s32 subject_z = TILE(subject->z);
            s32 subject_x = TILE(subject->x);
            s32 z_delta = subject_z - actor_z;
            s32 x_delta = subject_x - actor_x;

            if (x_delta == 0 && z_delta == 0) {
                actor->y = 0x00ff0000;
                actor->field_48 = 0;
                actor->field_28 = 0;
                return 1;
            }
        }

        index++;
    } while (index <= 3);

    return 0;
}
