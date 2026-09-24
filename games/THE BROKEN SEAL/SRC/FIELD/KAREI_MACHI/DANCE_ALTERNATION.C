#include "TYPES.H"

extern s32 KareiMachi_DanceStep;
extern u8 KareiMachi_DanceScriptA[];
extern u8 KareiMachi_DanceScriptB[];

u8 * Engine_ActorGet();
void Engine_ActorEnableActionCallback();

/* Each clear builds its own zero; a shared zero variable would live in r5
 * across the calls. */
static __inline__ void ClearStep(u8 *a)
{
    s32 z = 0;

    *(u16 *)(a + 100) = z;
}

void KareiMachi_AlternateDance(void)
{
    switch (KareiMachi_DanceStep) {
    case 0:
        ClearStep(Engine_ActorGet(21));
        Engine_ActorEnableActionCallback(21, KareiMachi_DanceScriptA);
        KareiMachi_DanceStep++;
        break;
    case 1:
        if (*(s16 *)(Engine_ActorGet(21) + 100) != 0) {
            ClearStep(Engine_ActorGet(20));
            Engine_ActorEnableActionCallback(20, KareiMachi_DanceScriptB);
            KareiMachi_DanceStep++;
        }
        break;
    case 2:
        if (*(s16 *)(Engine_ActorGet(20) + 100) != 0) {
            ClearStep(Engine_ActorGet(20));
            Engine_ActorEnableActionCallback(20, KareiMachi_DanceScriptA);
            KareiMachi_DanceStep++;
        }
        break;
    case 3:
        if (*(s16 *)(Engine_ActorGet(20) + 100) != 0) {
            ClearStep(Engine_ActorGet(21));
            Engine_ActorEnableActionCallback(21, KareiMachi_DanceScriptB);
            KareiMachi_DanceStep++;
        }
        break;
    case 4:
        if (*(s16 *)(Engine_ActorGet(21) + 100) != 0) {
            KareiMachi_DanceStep = 0;
        }
        break;
    }
}
