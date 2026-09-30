.syntax unified
	.thumb
	.global Graphics_PrepareTransferInIwramWork
	.thumb_func
Graphics_PrepareTransferInIwramWork:
	push {lr}
	ldr r2, .L_080156f4
	bl Func_08015510
	pop {pc}
	.2byte 0x0000
.L_080156f4:
	.4byte gTransform
