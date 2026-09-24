#include "TYPES.H"
#include "FIELD_EVENT.H"

void SceneActor_TransformAndApplyRecordPosition(struct FieldActor *object, s32 scale, s32 angle);

extern const s32 KuupuappuDou_PuffScript[];

/* Spawn up to four small type-240 objects at the source, rising with a random speed and a random turn, each running the puff script. */
void KuupuappuDou_SpawnPuffs(struct FieldActor *source)
{
    struct FieldActor *puff;
    s32 i;

    for (i = 0; i < 4; i++) {
        puff = Engine_ObjectCreate(240, source->x.fixed, source->y.fixed, source->z.fixed);
        if (puff == 0)
            break;
        puff->scale_y = 0x8ccc;
        puff->scale_x = 0x8ccc;
        puff->motion_flags = 2;
        puff->velocity_y = -0x10000;
        puff->speed = Engine_RandomNext() + 0xcccc;
        puff->collision_flags = 1;
        SceneActor_TransformAndApplyRecordPosition(puff, 0x200000, Engine_RandomNext());
        {
            u16 *timer = (u16 *)&puff->unknown_5d[1];
            s32 frames = 8;

            *timer = frames;
        }
        Engine_ObjectSetScript(puff, KuupuappuDou_PuffScript);
    }
}
