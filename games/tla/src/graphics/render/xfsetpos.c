#include "types.h"
#include "transform.h"

void SceneTransform_SetPosition(const s32 *pos)
{
	gTransform[9] = pos[0];
	gTransform[10] = pos[1];
	gTransform[11] = pos[2];
}
