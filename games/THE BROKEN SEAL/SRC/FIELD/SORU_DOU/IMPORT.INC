	.global Resource37c_SetViewScale
	.thumb_func
Resource37c_SetViewScale:
	overlay_veneer 0x080091f1
	.global Resource37c_TestSetupGate
	.thumb_func
Resource37c_TestSetupGate:
	overlay_veneer 0x080770c1
	.global Resource37c_SetSetupFlag
	.thumb_func
Resource37c_SetSetupFlag:
	overlay_veneer 0x080770c9
	.global Resource37c_FinalizeSetup
	.thumb_func
Resource37c_FinalizeSetup:
	overlay_veneer 0x0808a2f9
	.global Resource37c_QueueSoundCue
	.thumb_func
Resource37c_QueueSoundCue:
	overlay_veneer 0x0808a5e1
