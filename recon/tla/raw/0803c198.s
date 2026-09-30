.syntax unified
	.thumb
	.global Func_0803c198
	.thumb_func
Func_0803c198:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r10, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #60]
	mov r11, r2
	movs r2, #154
	lsls r2, r2, #5
	adds r3, r5, r2
	ldrh r3, [r3]
	movs r2, #240
	lsls r2, r2, #4
	adds r2, #56
	mov r9, r3
	adds r3, r5, r2
	ldrh r3, [r3]
	sub sp, #128
	adds r6, r0, #0
	mov r8, r3
	bl RenderOutput_AcquireFree
	adds r7, r0, #0
	movs r0, #0
	cmp r7, #0
	beq .L_0803c264
	ldr r2, .L_0803c258
	subs r3, r7, r5
	adds r3, r3, r2
	ldr r2, .L_0803c25c
	movs r4, #0
	adds r1, r3, #0
	muls r1, r2
	movs r3, #1
	mov r12, r3
	mov r2, r12
	movs r3, #152
	strb r2, [r7, #5]
	strb r4, [r7, #4]
	lsls r3, r3, #5
	adds r3, #72
	adds r0, r5, r3
	ldrh r5, [r0]
	ldr r2, .L_0803c260
	adds r5, r5, r1
	lsls r5, r5, #5
	adds r5, r5, r2
	mov r2, r8
	lsrs r3, r2, #1
	ldrh r2, [r6, #14]
	add r3, r10
	lsls r2, r2, #3
	adds r3, r3, r2
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #254
	adds r3, r3, r2
	strh r3, [r7, #20]
	mov r3, r9
	lsrs r2, r3, #1
	ldrh r3, [r6, #12]
	add r2, r11
	lsls r3, r3, #3
	adds r2, r2, r3
	ldr r3, .L_0803c254
	adds r2, #2
	orrs r2, r3
	strh r2, [r7, #22]
	adds r2, r7, #0
	ldrh r3, [r0]
	adds r2, #16
	adds r3, r3, r1
	strh r3, [r7, #24]
	movs r3, #253
	strb r3, [r7, #15]
	adds r0, r6, #0
	ldrh r3, [r2, #6]
	str r4, [r7]
	lsls r3, r3, #23
	lsrs r3, r3, #23
	strh r3, [r7, #6]
	ldrb r3, [r2, #4]
	strb r1, [r7, #14]
	strh r3, [r7, #8]
	adds r1, r7, #0
	bl RenderOutput_AppendToList
	adds r0, r5, #0
	b .L_0803c264
.L_0803c254:
	.4byte 0x00004000
.L_0803c258:
	.4byte 0xfffff8d0
.L_0803c25c:
	.4byte 0xb6db6db7
.L_0803c260:
	.4byte 0x06010000
.L_0803c264:
	add sp, #128
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
