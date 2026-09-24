#include "TYPES.H"

void SceneActor_UpdateAnimationOnStateMatch(s32 actor, s32 state, s32 anim, s32 script);

/* Pose actors 24 and 25 for dialogue steps 11 to 18. */
void KuupuappuHeya_Func02002564(void)
{
    switch (*(s16 *)(*(u8 **)0x03001ebc + 0x16c)) {
    case 11:
        SceneActor_UpdateAnimationOnStateMatch(24, 1, 2, 0x200d5b0);
        SceneActor_UpdateAnimationOnStateMatch(25, 3, 4, 0x200d8bc);
        break;
    case 12:
        SceneActor_UpdateAnimationOnStateMatch(24, 1, 4, 0x200d678);
        SceneActor_UpdateAnimationOnStateMatch(24, 2, 3, 0x200d5d8);
        SceneActor_UpdateAnimationOnStateMatch(25, 1, 3, 0x200d830);
        break;
    case 13:
        SceneActor_UpdateAnimationOnStateMatch(24, 2, 1, 0x200d538);
        SceneActor_UpdateAnimationOnStateMatch(24, 3, 6, 0x200d718);
        SceneActor_UpdateAnimationOnStateMatch(25, 2, 4, 0x200d894);
        break;
    case 14:
        SceneActor_UpdateAnimationOnStateMatch(24, 3, 2, 0x200d5b0);
        SceneActor_UpdateAnimationOnStateMatch(25, 4, 3, 0x200d858);
        break;
    case 15:
        SceneActor_UpdateAnimationOnStateMatch(24, 4, 5, 0x200d6c8);
        SceneActor_UpdateAnimationOnStateMatch(25, 1, 2, 0x200d7cc);
        break;
    case 16:
        SceneActor_UpdateAnimationOnStateMatch(24, 4, 1, 0x200d560);
        SceneActor_UpdateAnimationOnStateMatch(24, 5, 6, 0x200d6f0);
        SceneActor_UpdateAnimationOnStateMatch(25, 3, 1, 0x200d7a4);
        break;
    case 17:
        SceneActor_UpdateAnimationOnStateMatch(24, 5, 4, 0x200d650);
        SceneActor_UpdateAnimationOnStateMatch(24, 6, 3, 0x200d600);
        SceneActor_UpdateAnimationOnStateMatch(25, 4, 2, 0x200d808);
        break;
    case 18:
        SceneActor_UpdateAnimationOnStateMatch(24, 6, 5, 0x200d6c8);
        SceneActor_UpdateAnimationOnStateMatch(25, 2, 1, 0x200d768);
        break;
    }
}
