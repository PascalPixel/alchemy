.syntax unified
	.thumb
	.global Func_080d00f8
	.thumb_func
Func_080d00f8:
	push {r5, r6, lr}
	movs r1, #168
	adds r6, r0, #0
	lsls r1, r1, #3
	movs r0, #124
	sub sp, #4
	bl Runtime_AllocateBlock
	movs r3, #128
	adds r4, r0, #0
	movs r5, #0
	mov r0, sp
	lsls r3, r3, #19
	str r5, [r0]
	adds r3, #212
	adds r1, r4, #0
	ldr r2, .L_080d0174
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #165
	movs r2, #160
	lsls r1, r1, #3
	lsls r2, r2, #3
	adds r3, r4, r1
	adds r2, #42
	strh r6, [r3]
	adds r3, r4, r2
	strh r5, [r3]
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #52
	adds r2, r4, r3
	movs r3, #249
	lsls r3, r3, #6
	adds r3, #255
	strh r3, [r2]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #74
	ldrh r2, [r3]
	movs r3, #255
	lsls r3, r3, #8
	ands r3, r2
	ldr r2, .L_080d0170
	adds r1, #14
	adds r4, r4, r1
	orrs r3, r2
	strh r3, [r4]
	subs r1, #182
	ldr r0, .L_080d0178
	bl Scheduler_AddOrUpdateCallback
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #118
	ldr r0, .L_080d017c
	bl Scheduler_AddOrUpdateCallback
	add sp, #4
	b .L_080d0180
.L_080d0170:
	.4byte 0x00000001
.L_080d0174:
	.4byte 0x85000150
.L_080d0178:
	.4byte Func_080cf78c
.L_080d017c:
	.4byte Func_080cf6fc
.L_080d0180:
	pop {r5, r6, pc}
	.2byte 0x0000
