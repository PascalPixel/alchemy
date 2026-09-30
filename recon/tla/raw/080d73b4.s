.syntax unified
	.thumb
	.global Func_080d73b4
	.thumb_func
Func_080d73b4:
	push {lr}
	movs r1, #248
	lsls r1, r1, #5
	adds r1, #136
	movs r0, #120
	bl Runtime_AllocateBlock
	movs r3, #252
	lsls r3, r3, #5
	adds r2, r0, r3
	movs r3, #254
	lsls r3, r3, #7
	adds r3, #255
	strh r3, [r2]
	movs r3, #248
	lsls r3, r3, #5
	adds r3, #130
	movs r1, #0
	adds r0, r0, r3
	strh r1, [r0]
	pop {pc}
	.2byte 0x0000
