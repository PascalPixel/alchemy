#include "TYPES.H"

void BattleFx_IntegrateVector2(s32 *vector, s32 damping, s32 gravity);
void BattleFx_IntegrateVector3(s32 *vector, s32 damping, s32 gravity);

void BattleFx_StepVector2(s32 *vector, s32 damping, s32 gravity)
{
    BattleFx_IntegrateVector2(vector, damping, gravity);
}

void BattleFx_StepVector3(s32 *vector, s32 damping, s32 gravity)
{
    BattleFx_IntegrateVector3(vector, damping, gravity);
}
