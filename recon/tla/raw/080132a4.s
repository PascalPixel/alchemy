.syntax unified
	.thumb
	.global Func_080132a4
	.thumb_func
Func_080132a4:
	push {r5, lr}
	ldr r5, .L_080132b4
.L_080132a8:
	movs r0, #1
	ldr r3, [r5, #4]
	bl WaitFrames
	b .L_080132a8
	.2byte 0x0000
.L_080132b4:
	.4byte gInput
