	.global Resource388_QueueSlotCommand
	.thumb_func
Resource388_QueueSlotCommand:
	overlay_veneer 0x080091c1
	.global Resource388_SetSlotOption
	.thumb_func
Resource388_SetSlotOption:
	overlay_veneer 0x080091e1
	.global Resource388_TestSetupGate
	.thumb_func
Resource388_TestSetupGate:
	overlay_veneer 0x080770c1
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer 0x0808a081
	.global Resource388_SetSlotPosition
	.thumb_func
Resource388_SetSlotPosition:
	overlay_veneer 0x0808a0f1
	.global Resource388_SetSlotMode
	.thumb_func
Resource388_SetSlotMode:
	overlay_veneer 0x0808a101
