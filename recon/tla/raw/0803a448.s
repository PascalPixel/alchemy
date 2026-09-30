.syntax unified
	.thumb
	.global Func_0803a448
	.thumb_func
Func_0803a448:
	push {r5, lr}
	movs r3, #192
	movs r1, #128
	lsls r3, r3, #18
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #95
	ldr r5, [r3, #60]
	bl VramBlock_LoadCached
	movs r2, #152
	lsls r2, r2, #5
	adds r2, #72
	adds r3, r5, r2
	strh r0, [r3]
	movs r3, #154
	lsls r3, r3, #5
	adds r2, r5, r3
	movs r3, #9
	strh r3, [r2]
	movs r3, #240
	lsls r3, r3, #4
	adds r3, #56
	adds r2, r5, r3
	movs r3, #10
	strh r3, [r2]
	movs r2, #240
	lsls r2, r2, #4
	adds r2, #60
	adds r3, r5, r2
	movs r1, #0
	strh r1, [r3]
	movs r3, #240
	lsls r3, r3, #4
	adds r3, #62
	adds r2, r5, r3
	movs r3, #15
	strh r3, [r2]
	movs r2, #152
	lsls r2, r2, #5
	adds r2, #66
	adds r5, r5, r2
	strh r1, [r5]
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0803a4ac
	bl Func_080145a8
	pop {r5, pc}
	.2byte 0x0000
.L_0803a4ac:
	.4byte Func_0803a8c8
