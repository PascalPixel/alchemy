.syntax unified
	.thumb
	.global Func_08042ff0
	.thumb_func
Func_08042ff0:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	sub sp, #8
	adds r6, r0, #0
	movs r1, #4
	ldr r5, .L_080430b8
	add r1, sp
	ldrh r3, [r5]
	mov r10, r1
	str r3, [r1]
	strh r5, [r5]
	mov r9, sp
	ldr r2, .L_080430bc
	ldrh r3, [r2]
	mov r8, r2
	str r3, [sp, #0]
	movs r3, #0
	mov r1, r8
	strh r3, [r1]
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #176
	movs r4, #197
	ldrh r1, [r2, #10]
	lsls r4, r4, #8
	adds r4, #255
	adds r3, r4, #0
	ands r3, r1
	strh r3, [r2, #10]
	movs r0, #254
	ldrh r1, [r2, #10]
	lsls r0, r0, #7
	adds r0, #255
	adds r3, r0, #0
	ands r3, r1
	strh r3, [r2, #10]
	ldrh r3, [r2, #10]
	adds r2, #12
	ldrh r1, [r2, #10]
	adds r3, r4, #0
	ands r3, r1
	strh r3, [r2, #10]
	adds r3, r0, #0
	ldrh r1, [r2, #10]
	ands r3, r1
	strh r3, [r2, #10]
	ldrh r3, [r2, #10]
	adds r2, #12
	ldrh r1, [r2, #10]
	adds r3, r4, #0
	ands r3, r1
	strh r3, [r2, #10]
	adds r3, r0, #0
	ldrh r1, [r2, #10]
	ands r3, r1
	strh r3, [r2, #10]
	ldrh r3, [r2, #10]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldrh r2, [r3, #10]
	ands r4, r2
	strh r4, [r3, #10]
	ldrh r2, [r3, #10]
	ands r0, r2
	strh r0, [r3, #10]
	ldrh r3, [r3, #10]
	mov r2, r10
	ldr r3, [r2]
	strh r3, [r5]
	ldr r0, .L_080430c0
	bl SerialRuntime_QueueCommand
	adds r0, r6, #0
	bl Func_08015e8c
	adds r6, r0, #0
	movs r0, #0
	bl SerialRuntime_QueueCommand
	ldrh r2, [r5]
	mov r3, r10
	str r2, [r3]
	strh r5, [r5]
	mov r1, r9
	ldr r3, [r1]
	mov r1, r8
	strh r3, [r1]
	strh r2, [r5]
	adds r0, r6, #0
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, pc}
.L_080430b8:
	.4byte 0x04000208
.L_080430bc:
	.4byte 0x04000200
.L_080430c0:
	.4byte Func_08042dc4
