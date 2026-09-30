.syntax unified
	.thumb
	.global Func_08122c88
	.thumb_func
Func_08122c88:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	movs r1, #128
	lsls r1, r1, #4
	adds r1, #40
	adds r2, r3, r1
	movs r4, #0
	adds r1, #8
	str r4, [r2]
	adds r2, r3, r1
	adds r1, #4
	str r4, [r2]
	adds r2, r3, r1
	str r0, [r2]
	movs r2, #128
	lsls r2, r2, #4
	adds r2, #44
	adds r1, r3, r2
	movs r2, #2
	str r2, [r1]
	movs r1, #192
	lsls r1, r1, #3
	adds r1, #125
	adds r3, r3, r1
	movs r1, #144
	strb r4, [r3]
	lsls r1, r1, #3
	ldr r0, .L_08122ccc
	bl Scheduler_AddOrUpdateCallback
	pop {pc}
	.2byte 0x0000
.L_08122ccc:
	.4byte Func_08122d10
