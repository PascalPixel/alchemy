.syntax unified
	.thumb
	.global Func_0803c068
	.thumb_func
Func_0803c068:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #140
	str r2, [sp, #8]
	str r3, [sp, #4]
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #60]
	mov r10, r1
	movs r1, #154
	lsls r1, r1, #5
	adds r3, r6, r1
	ldrh r3, [r3]
	movs r2, #240
	str r3, [sp, #0]
	lsls r2, r2, #4
	adds r2, #56
	adds r3, r6, r2
	ldrh r3, [r3]
	mov r11, r0
	mov r8, r3
	bl RenderOutput_AcquireFree
	adds r7, r0, #0
	movs r0, #0
	cmp r7, #0
	beq .L_0803c160
	ldr r1, .L_0803c154
	ldr r2, .L_0803c158
	subs r3, r7, r6
	adds r3, r3, r1
	adds r1, r3, #0
	muls r1, r2
	add r5, sp, #12
	movs r3, #1
	movs r2, #0
	mov r9, r1
	strb r3, [r7, #5]
	strb r2, [r7, #4]
	adds r1, r5, #0
	mov r0, r10
	bl Func_0803a8fc
	movs r3, #152
	lsls r3, r3, #5
	adds r3, #72
	adds r4, r6, r3
	ldrh r1, [r4]
	ldr r2, .L_0803c15c
	add r1, r9
	lsls r1, r1, #5
	adds r1, r1, r2
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	mov r10, r0
	adds r3, #212
	adds r0, r5, #0
	adds r2, #32
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, [sp, #4]
	mov r1, r8
	lsrs r3, r1, #1
	mov r1, r11
	adds r3, r2, r3
	ldrh r2, [r1, #14]
	mov r0, r11
	lsls r2, r2, #3
	adds r3, r3, r2
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #254
	adds r3, r3, r2
	strh r3, [r7, #20]
	ldr r3, [sp, #0]
	ldr r1, [sp, #8]
	lsrs r2, r3, #1
	adds r2, r1, r2
	mov r1, r11
	ldrh r3, [r1, #12]
	adds r1, r7, #0
	lsls r3, r3, #3
	adds r2, r2, r3
	ldr r3, .L_0803c150
	adds r2, #2
	orrs r2, r3
	strh r2, [r7, #22]
	adds r2, r7, #0
	ldrh r3, [r4]
	adds r2, #16
	add r3, r9
	strh r3, [r7, #24]
	movs r3, #253
	strb r3, [r7, #15]
	ldrh r3, [r2, #6]
	lsls r3, r3, #23
	lsrs r3, r3, #23
	strh r3, [r7, #6]
	ldrb r3, [r2, #4]
	mov r2, r9
	strh r3, [r7, #8]
	movs r3, #0
	strb r2, [r7, #14]
	str r3, [r7]
	bl RenderOutput_AppendToList
	mov r0, r10
	b .L_0803c160
	.2byte 0x0000
.L_0803c150:
	.4byte 0x00004000
.L_0803c154:
	.4byte 0xfffff8d0
.L_0803c158:
	.4byte 0xb6db6db7
.L_0803c15c:
	.4byte 0x06010000
.L_0803c160:
	add sp, #140
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
