#include "TYPES.H"
extern struct EventWork *gEventWork;
extern u8 KuupuappuHeya_PairScriptA[];
extern u8 KuupuappuHeya_PairScriptB[];
extern u8 KuupuappuHeya_PairScriptC[];
extern u8 KuupuappuHeya_PairScriptD[];
extern u8 KuupuappuHeya_PairScriptE[];
extern u8 KuupuappuHeya_PairScriptF[];
extern u8 KuupuappuHeya_PairScriptG[];
extern u8 KuupuappuHeya_PairScriptH[];
extern u8 KuupuappuHeya_PairScriptI[];
extern u8 KuupuappuHeya_PairScriptJ[];
extern u8 KuupuappuHeya_PairScriptK[];
extern u8 KuupuappuHeya_PairScriptL[];
extern u8 KuupuappuHeya_PairScriptM[];
extern u8 KuupuappuHeya_PairScriptN[];
extern u8 KuupuappuHeya_PairScriptO[];
extern u8 KuupuappuHeya_PairScriptP[];
extern u8 KuupuappuHeya_PairScriptQ[];
extern u8 KuupuappuHeya_PairScriptR[];

void SceneActor_UpdateAnimationOnStateMatch(s32 actor, s32 state, s32 anim, s32 script);

/* Pose actors 24 and 25 for dialogue steps 11 to 18. */
void KuupuappuHeya_PoseDialogueActors(void)
{
    switch (*(s16 *)(*(u8 **)&gEventWork + 0x16c)) {
    case 11:
        SceneActor_UpdateAnimationOnStateMatch(24, 1, 2, (s32)KuupuappuHeya_PairScriptC);
        SceneActor_UpdateAnimationOnStateMatch(25, 3, 4, (s32)KuupuappuHeya_PairScriptR);
        break;
    case 12:
        SceneActor_UpdateAnimationOnStateMatch(24, 1, 4, (s32)KuupuappuHeya_PairScriptG);
        SceneActor_UpdateAnimationOnStateMatch(24, 2, 3, (s32)KuupuappuHeya_PairScriptD);
        SceneActor_UpdateAnimationOnStateMatch(25, 1, 3, (s32)KuupuappuHeya_PairScriptO);
        break;
    case 13:
        SceneActor_UpdateAnimationOnStateMatch(24, 2, 1, (s32)KuupuappuHeya_PairScriptA);
        SceneActor_UpdateAnimationOnStateMatch(24, 3, 6, (s32)KuupuappuHeya_PairScriptJ);
        SceneActor_UpdateAnimationOnStateMatch(25, 2, 4, (s32)KuupuappuHeya_PairScriptQ);
        break;
    case 14:
        SceneActor_UpdateAnimationOnStateMatch(24, 3, 2, (s32)KuupuappuHeya_PairScriptC);
        SceneActor_UpdateAnimationOnStateMatch(25, 4, 3, (s32)KuupuappuHeya_PairScriptP);
        break;
    case 15:
        SceneActor_UpdateAnimationOnStateMatch(24, 4, 5, (s32)KuupuappuHeya_PairScriptH);
        SceneActor_UpdateAnimationOnStateMatch(25, 1, 2, (s32)KuupuappuHeya_PairScriptM);
        break;
    case 16:
        SceneActor_UpdateAnimationOnStateMatch(24, 4, 1, (s32)KuupuappuHeya_PairScriptB);
        SceneActor_UpdateAnimationOnStateMatch(24, 5, 6, (s32)KuupuappuHeya_PairScriptI);
        SceneActor_UpdateAnimationOnStateMatch(25, 3, 1, (s32)KuupuappuHeya_PairScriptL);
        break;
    case 17:
        SceneActor_UpdateAnimationOnStateMatch(24, 5, 4, (s32)KuupuappuHeya_PairScriptF);
        SceneActor_UpdateAnimationOnStateMatch(24, 6, 3, (s32)KuupuappuHeya_PairScriptE);
        SceneActor_UpdateAnimationOnStateMatch(25, 4, 2, (s32)KuupuappuHeya_PairScriptN);
        break;
    case 18:
        SceneActor_UpdateAnimationOnStateMatch(24, 6, 5, (s32)KuupuappuHeya_PairScriptH);
        SceneActor_UpdateAnimationOnStateMatch(25, 2, 1, (s32)KuupuappuHeya_PairScriptK);
        break;
    }
}
