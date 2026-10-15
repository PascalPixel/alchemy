#include "types.h"
#include "field_obj.h"

s32 Scene_InitSceneRequestAndCameraZoom(void)
{
	struct Resource37deSceneRuntime *runtime;

	s32 zoomX = 0x10000;
	s32 zoomY = 0x10000;
	s32 zoomZ = 0x10000;

	runtime = (struct Resource37deSceneRuntime *)gEventWork;
	runtime->sceneRequest1c0 = 0x204;
	runtime->sceneSetupWord1c8 = 0x10;
	if (Engine_GameFlagIsSet(0x814) != 0)
	{
		QueueSceneSound(0x8D);
		Engine_WorkSetValuesIfNonNegative(zoomX, zoomY, zoomZ);
		InitializeSceneRecordBuffer();
	}
	return 0;
}
