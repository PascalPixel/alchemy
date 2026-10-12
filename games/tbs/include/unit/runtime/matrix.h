#ifndef ALCHEMY_UNIT_RUNTIME_MATRIX_H
#define ALCHEMY_UNIT_RUNTIME_MATRIX_H

#include "types.h"
#include "callback_scheduler.h"
#include "irq.h"

void Render_ResetTransformState(void);
void Graphics_SaveTransferWorkOnce(void);
void Graphics_SaveTransferWork(void *dst);
void Graphics_LoadTransferWork(const void *src);
void Graphics_RestoreTransferWork(void);
void SceneTransform_ResetMatrix(void);
void SceneTransform_ApplyRotation(s32 *ang);
void SceneTransform_ApplyPitch(s32 ang);
void SceneTransform_ApplyYaw(s32 ang);
void SceneTransform_ApplyRoll(s32 ang);
void SceneTransform_ApplyPosition(const s32 *pos);
void SceneTransform_ApplyScale(const s32 *scale);
void SceneTransform_ApplyRotationTranslation(s32 *ang, s32 *pos);
void SceneTransform_ApplyScaledRotation(s32 *ang, s32 *pos, s32 *scale);
void Graphics_PrepareTransfer(s32 *eye, s32 *target, s32 *out);
void Graphics_PrepareTransferInIwramWork(s32 *eye, s32 *target);
void Graphics_PrepareTransferAndRun(s32 *eye, s32 *target);
void Camera_SetAngleParameters(u32 value, s32 param1, s32 param2);
void Camera_StoreSceneParameters(u32 value0, u32 value1, u32 value2);
s32 Render_ProjectPoint(s32 *point, s32 *screen);

#endif
