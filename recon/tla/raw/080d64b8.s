.syntax unified
	.thumb
	.global Func_080d64b8
	.thumb_func
Func_080d64b8:
	push {lr}
	ldr r0, .L_080d64e8
	bl Scheduler_RemoveCallback
	ldr r0, .L_080d64ec
	bl Scheduler_RemoveCallback
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #176
	ldrh r1, [r2, #10]
	movs r3, #197
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r1
	strh r3, [r2, #10]
	movs r3, #254
	ldrh r1, [r2, #10]
	lsls r3, r3, #7
	adds r3, #255
	ands r3, r1
	strh r3, [r2, #10]
	ldrh r3, [r2, #10]
	pop {pc}
.L_080d64e8:
	.4byte Func_080d5ff8
.L_080d64ec:
	.4byte Func_080d607c
