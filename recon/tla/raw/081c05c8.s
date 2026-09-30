.syntax unified
	.thumb
	.global Func_081c05c8
	.thumb_func
Func_081c05c8:
	push {lr}
	lsls r0, r0, #16
	ldr r2, [pc, #36]
	ldr r1, [pc, #40]
	lsrs r0, r0, #13
	adds r0, r0, r1
	ldrh r3, [r0, #4]
	lsls r1, r3, #1
	adds r1, r1, r3
	lsls r1, r1, #2
	bx pc
	.2byte 0x46c0
	.4byte 0xe0803091
	.4byte 0xe12fff1e
