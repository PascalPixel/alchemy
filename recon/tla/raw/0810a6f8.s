.syntax unified
	.thumb
	.global Func_0810a6f8
	.thumb_func
Func_0810a6f8:
	lsls r3, r0, #5
	ldr r2, .L_0810a708
	adds r3, r3, r0
	lsls r3, r3, #1
	adds r3, #64
	ldrsh r0, [r2, r3]
	bx lr
	.2byte 0x0000
.L_0810a708:
	.4byte Data_0810c3f4
