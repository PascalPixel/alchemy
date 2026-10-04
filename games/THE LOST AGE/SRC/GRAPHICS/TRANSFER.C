#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "TRANSFORM.H"

/* Builds the look-at transform from eye to target into the current one. */
void Graphics_PrepareTransferInIwramWork(s32 *eye, s32 *target)
{
    Graphics_PrepareTransfer(eye, target, gTransform);
}

/* Builds the look-at transform and applies it to the current one. */
void Graphics_PrepareTransferAndRun(s32 *eye, s32 *target)
{
    s32 work[12];

    Graphics_PrepareTransfer(eye, target, work);
    Iwram_TransformMatrix(work);
}
