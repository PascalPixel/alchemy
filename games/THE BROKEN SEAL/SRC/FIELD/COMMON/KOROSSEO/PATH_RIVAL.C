#include "TYPES.H"
#include "FIELD_EVENT.H"

u16 Math_Atan2(s32 dy, s32 dx);

struct PathRecorder {
    s16 mode;
    s16 mirror;
    s16 actor;
    u16 pos;
    s16 still;
};

struct PathWork {
    u8 unknown_00[224];
    u16 actor;
    u16 finish_facing_a;
    u16 finish_facing_b;
    u16 unknown_e6;
    s32 finish_x;
    s32 finish_z;
    s16 path[0x3840];
};

#define PATH_RECORDER ((struct PathRecorder *)0x02001000)
#define PATH_WORK (*(struct PathWork **)0x03001f3c)

extern const s32 Korosseo_RivalFinishScript[];

/* The rival's recorded course: mode 2 records the actor's pixel position each
 * frame; mode 1 replays it, turning at most 0x1000 a frame, and at the end
 * walks the rival beside the finish and hands it to the finish script. */
void Korosseo_UpdatePathRival(void)
{
    struct PathWork *work = PATH_WORK;
    struct PathRecorder *rec = PATH_RECORDER;
    struct FieldActor *obj;
    s32 x, z, d;
    u16 *facing;

    obj = Engine_ActorGet(rec->actor);
    if (obj == NULL) {
        return;
    }
    if (rec->mode == 1) {
        x = work->path[(s16)rec->pos++];
        z = work->path[(s16)rec->pos++];
        if (x == 0 && z == 0) {
            s32 fz;
            rec->mode = 9;
            Engine_ObjectSetAnimation(obj, 1);
            if (work->finish_x < obj->x.fixed) {
                x = work->finish_x + 0xc0000;
            } else {
                x = work->finish_x - 0xc0000;
            }
            if (Engine_GameFlagIsSet(0x211)) {
                fz = work->finish_z + 0x100000;
                facing = &work->finish_facing_b;
            } else {
                fz = work->finish_z - 0x100000;
                facing = &work->finish_facing_a;
            }
            obj->unknown_64 = *facing;
            obj->acceleration = 0x4000;
            obj->speed = 0x10000;
            Engine_ObjectSetPosition(obj, x, 0, fz);
            Engine_GameFlagSet(0x211);
            Engine_ObjectSetScript(obj, Korosseo_RivalFinishScript);
            return;
        }
        x <<= 16;
        z <<= 16;
        if (rec->mirror != 0) {
            x = work->finish_x * 2 - x;
        }
        if (obj->x.fixed != x || obj->z.fixed != z) {
            {
            u16 angle = Math_Atan2(z - obj->z.fixed, x - obj->x.fixed);
            d = (s16)(angle - obj->facing);
            }
            if (d > 0x1000) {
                d = 0x1000;
            }
            if (d < -0x1000) {
                d = -0x1000;
            }
            obj->facing += d;
            obj->x.fixed = x;
            obj->z.fixed = z;
            rec->still = 0;
        } else {
            rec->still++;
        }
        if (rec->still > 2) {
            Engine_ObjectSetAnimation(obj, 1);
        } else {
            Engine_ObjectSetAnimation(obj, 5);
        }
    } else if (rec->mode == 2) {
        x = obj->x.part.pixel;
        z = obj->z.part.pixel;
        work->path[(s16)rec->pos++] = x;
        work->path[(s16)rec->pos++] = z;
        if ((s16)rec->pos == 0x383e) {
            work->path[(s16)rec->pos++] = 0;
            work->path[(s16)rec->pos] = 0;
            rec->actor = work->actor;
            rec->pos = 0;
            rec->mode = 1;
        }
    }
}
