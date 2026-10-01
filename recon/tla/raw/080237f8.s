.syntax unified
	.thumb
	.global Func_080237f8
	.thumb_func
Func_080237f8:
	push {lr}
	ldr r0, .L_08023838
	bl Scheduler_EnableCallbacks
	ldr r0, .L_0802383c
	bl Scheduler_EnableCallbacks
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl Func_080c8378
	movs r0, #1
	bl Func_080c8390
	movs r0, #1
	bl WaitFrames
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r1]
	movs r3, #241
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	ldr r2, .L_08023834
	orrs r3, r2
	strh r3, [r1]
	pop {pc}
	.2byte 0x0000
.L_08023834:
	.4byte 0x00001000
.L_08023838:
	.4byte Func_0802386c
.L_0802383c:
	.4byte Func_08023e18
