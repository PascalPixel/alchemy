.syntax unified
	.thumb
	.global SerialRuntime_CollectReceivedPayloads
	.thumb_func
SerialRuntime_CollectReceivedPayloads:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	movs r3, #0
	adds r4, r0, #0
	str r3, [sp, #4]
	ldr r2, .L_0800622c
	strh r3, [r2]
	ldr r3, .L_08006230
	adds r1, r3, #0
	adds r1, #64
	movs r7, #3
.L_0800617a:
	ldr r2, [r1, #16]
	ldr r3, [r1]
	subs r7, #1
	str r3, [r1, #16]
	stmia r1!, {r2}
	cmp r7, #0
	bge .L_0800617a
	ldr r1, .L_08006234
	ldr r3, [r1]
	movs r0, #0
	mov lr, sp
	str r3, [sp, #0]
	str r0, [r1]
	movs r2, #1
	ldr r3, .L_0800622c
	strh r2, [r3]
	subs r3, r1, #4
	mov r9, r3
	subs r2, #2
	adds r6, r1, #0
	strb r0, [r3, #3]
	movs r7, #0
	mov r10, r9
	mov r8, r2
	adds r6, #76
	mov r12, r4
.L_080061ae:
	ldr r2, [r6]
	movs r0, #0
	movs r1, #0
.L_080061b4:
	ldrh r3, [r2]
	adds r1, #1
	adds r2, #2
	adds r0, r0, r3
	cmp r1, #13
	bls .L_080061b4
	mov r3, lr
	ldrb r4, [r3, r7]
	cmp r4, #1
	bne .L_080061ee
	lsls r5, r0, #16
	asrs r3, r5, #16
	cmp r3, r8
	bne .L_08006202
	ldr r0, [r6]
	ldr r3, .L_08006238
	adds r0, #4
	mov r1, r12
	ldr r2, .L_0800623c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r1, r10
	ldrb r3, [r1, #3]
	lsls r4, r7
	ldr r2, .L_08006230
	orrs r4, r3
	strb r4, [r1, #3]
	mov r9, r2
	b .L_080061f0
.L_080061ee:
	lsls r5, r0, #16
.L_080061f0:
	asrs r3, r5, #16
	cmp r3, r8
	bne .L_08006202
	ldr r2, [r6]
	ldrh r3, [r2, #2]
	mvns r3, r3
	strh r3, [r2, #2]
	ldr r3, .L_08006230
	mov r9, r3
.L_08006202:
	movs r1, #24
	adds r7, #1
	adds r6, #4
	add r12, r1
	cmp r7, #1
	ble .L_080061ae
	mov r2, r9
	ldrb r3, [r2, #2]
	ldrb r2, [r2, #3]
	mov r1, r9
	ldrb r0, [r1, #3]
	orrs r3, r2
	strb r3, [r1, #2]
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_0800622c:
	.4byte 0x04000208
.L_08006230:
	.4byte gSerialRuntime
.L_08006234:
	.4byte gSerialRuntime + 0x4
.L_08006238:
	.4byte 0x040000d4
.L_0800623c:
	.4byte 0x84000006
