#include "NIWA.H"

void BiribinoNiwa_ApplyEntryState(void);

/*
 * The garden's scene start: seats scene entity 8 in its idle presentation
 * and, in the garden itself, runs the scene body.
 */
s32 Scene_Initialize(void)
{
    u8 *work = (u8 *)gEventWork;
    struct SceneEntity *ent;
    struct SceneHandle *h;
    u8 *fp;
    s32 zero;

    *(s32 *)(work + 448) = 256;            /* 128 << 1 */

    ent = (struct SceneEntity *)Engine_ActorGet(8);
    fp = (u8 *)ent + 35;
    /* FAKEMATCH: one register carries the stored zero and then, decremented
       by 13, the ~0x0c handle mask, so the two are not folded into separate
       constants. */
    zero = 0;
    *fp = (u8)zero;

    h = ent->h;
    zero -= 13;
    h->flags09 = (u8)((h->flags09 & zero) | 0x04);

    if (gGameState.scene == (s32)&SceneId_BiribinoNiwa) {
        BiribinoNiwa_ApplyEntryState();
    }

    return 0;
}
