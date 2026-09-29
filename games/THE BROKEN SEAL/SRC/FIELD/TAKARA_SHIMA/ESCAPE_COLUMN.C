#include "TYPES.H"
#include "SCENE_IDS.H"

extern s16 gGameState[];

u8 *Engine_ActorGet();
u8 *Engine_GameFlagIsSet();
void Engine_EventWait();
void SpawnRadialEffectBurst();
void Engine_AudioPlayCue();
void Engine_ActorSetSpriteFlags();
void ObjectMotion_SetActionVariant();
void Engine_MapCopyCellAttributes();
void Engine_GameFlagSet();

void FieldScene_HandleEscapeColumn(void)
{
    u8 *entity;
    s16 *slot;
    s32 column;

    entity = Engine_ActorGet(8);
    column = *(s32 *)(entity + 8) >> 20;    /* 16.16 -> 16-pixel tile grid */
    if (column != 40) {
        return;
    }

    {
        s32 off = 448;
        slot = (s16 *) ((u8 *) gGameState + off);
    }
    if (Engine_GameFlagIsSet(*slot + (0x8d2 - (s32)&SceneId_TakaraShima6)) != 0) {
        return;                             /* handled by 0x02001214 instead */
    }

    entity[85] = 3;

    Engine_EventWait(8);
    SpawnRadialEffectBurst(8);
    Engine_AudioPlayCue(136);
    Engine_EventWait(40);

    Engine_ActorSetSpriteFlags(Engine_ActorGet(8), 0);
    ObjectMotion_SetActionVariant(8, 3);

    entity[85] = 0;
    /* FAKEMATCH: a temporary holding the 2 picks the reference registers. */
    {
        s32 flags = 2;

        flags |= entity[35];
        entity[35] = flags;
    }

    Engine_MapCopyCellAttributes(42, 10, 1, 1, column, 10);

    Engine_GameFlagSet(*slot + (0x8d2 - (s32)&SceneId_TakaraShima6));
}
