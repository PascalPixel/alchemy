#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR_EFFECT.H"

struct Struct3848 {
    u8 pad00[8];
    u32 field08;
    s32 field0c;
    u32 field10;
};

s32 StagedActor_RunStepEffect(struct StagedActorEffectRequest *request);

void SceneActor_PassSubjectOffsetPosition(void)
{
    u32 buf[3];
    s32 off = 500;
    struct Struct3848 *p = (void *)Object_GetById(*(s32 *)((u8 *)&gGameState + off));
    u32 base = p->field08 & 0xfff00000;

    buf[0] = base + 0x80000;
    buf[1] = p->field0c;
    buf[2] = (p->field10 & 0xfff00000) + 0x80000;
    buf[0] = base + 0x280000;
    StagedActor_RunStepEffect((struct StagedActorEffectRequest *)buf);
}
