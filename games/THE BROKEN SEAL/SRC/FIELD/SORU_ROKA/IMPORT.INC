	.global SetCameraZoom
	.thumb_func
SetCameraZoom:
	overlay_veneer 0x080091f1
	.global TestSceneFlag
	.thumb_func
TestSceneFlag:
	overlay_veneer 0x080770c1
	.global InitializeSceneRecordBuffer
	.thumb_func
InitializeSceneRecordBuffer:
	overlay_veneer 0x0808a2f9
	.global QueueSceneSound
	.thumb_func
QueueSceneSound:
	overlay_veneer 0x0808a5e1
