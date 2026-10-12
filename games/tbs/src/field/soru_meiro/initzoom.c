s32 Scene_InitSceneRequestAndCameraZoom(void)
{
	struct Resource37deSceneRuntime *runtime;

	s32 zoom_x = 0x10000;
	s32 zoom_y = 0x10000;
	s32 zoom_z = 0x10000;

	runtime = gEventWork;
	runtime->scene_request_1c0 = 0x204;
	runtime->scene_setup_word_1c8 = 0x10;
	if (Engine_GameFlagIsSet(0x814) != 0)
	{
		QueueSceneSound(0x8D);
		Engine_WorkSetValuesIfNonNegative(zoom_x, zoom_y, zoom_z);
		InitializeSceneRecordBuffer();
	}
	return 0;
}
