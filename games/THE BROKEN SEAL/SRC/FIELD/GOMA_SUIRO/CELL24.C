#include "GOMA.H"

u8 *Engine_ActorGet();
void Engine_ActorSetSpriteFlags();
void Map_CopyCellAttributeRect();
void GameFlag_SetBit();
void SceneEffect_RunActorBurst(s32 no);

static __inline__ void SetFlagBits(u8 *flags, u8 bits)
{
    *flags |= bits;
}

void FieldScene_RunActor8AtCell24Sequence(void)
{
    s32 *record;
    u8 *target;
    s32 value;

    record = (s32 *)Value1(Engine_ActorGet, 8);
    value = record[2] / 0x100000;
    if (value == 24) {
        SceneEffect_RunActorBurst(8);
        SetFlagBits(Engine_ActorGet(8) + 35, 2);
        Call6(Map_CopyCellAttributeRect, 19, 74, 9, 3, 19, 17);
        target = Engine_ActorGet(8);
        Engine_ActorSetSpriteFlags((s32)target, 0);
        Call1(GameFlag_SetBit, 0x864);
    }
}

