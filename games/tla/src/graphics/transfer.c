#include "types.h"
#include "iwram_call.h"
#include "transform.h"

void Graphics_PrepareTransferInIwramWork(s32 *eye, s32 *target)
{
    Graphics_PrepareTransfer(eye, target, gTransform);
}

void Graphics_PrepareTransferAndRun(s32 *eye, s32 *target)
{
    s32 work[12];

    Graphics_PrepareTransfer(eye, target, work);
    Iwram_TransformMatrix(work);
}
