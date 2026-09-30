.syntax unified
	.thumb
	.global Func_0804babc
	.thumb_func
Func_0804babc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #20
	str r0, [sp, #16]
	movs r0, #9
	str r1, [sp, #12]
	str r2, [sp, #8]
	bl Runtime_BumpAllocateAlternatePool
	ldr r2, [sp, #12]
	adds r5, r0, #0
	ldr r2, [r2]
	movs r0, #1
	adds r1, r5, #0
	mov r11, r2
	bl Func_08118088 + 0x38
	movs r3, #0
	mov r10, r3
	movs r7, #0
	cmp r10, r0
	bge .L_0804bb88
	mov r8, r0
.L_0804baf4:
	ldrh r0, [r5]
	bl Owner_GetState
	movs r4, #67
	adds r4, r4, r0
	ldrb r3, [r4]
	movs r6, #0
	mov r12, r4
	cmp r6, r3
	bge .L_0804bb7a
	movs r2, #158
	lsls r2, r2, #1
	adds r2, r2, r0
	movs r3, #156
	mov r9, r2
	lsls r3, r3, #1
	ldr r2, [sp, #16]
	adds r3, r3, r0
	mov r4, r10
	mov lr, r3
	str r5, [sp, #0]
	lsls r3, r4, #1
	adds r1, r3, r2
	lsls r2, r7, #4
	add r2, r11
.L_0804bb26:
	mov r3, r9
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_0804bb3a
	mov r4, lr
	ldr r3, [r4]
	ldr r4, .L_0804bb60
	ands r3, r4
	cmp r3, #0
	beq .L_0804bb64
.L_0804bb3a:
	ldr r4, [sp, #0]
	adds r7, #1
	ldrh r3, [r4]
	strh r3, [r2]
	adds r3, r0, #0
	adds r3, #64
	ldrh r3, [r3]
	strh r3, [r2, #4]
	movs r3, #8
	strh r3, [r2, #6]
	ldr r3, .L_0804bb5c
	strh r3, [r2, #8]
	movs r3, #192
	lsls r3, r3, #1
	strh r3, [r2, #10]
	adds r2, #16
	b .L_0804bb70
.L_0804bb5c:
	.4byte 0x00000000
.L_0804bb60:
	.4byte 0xffffff00
.L_0804bb64:
	ldr r4, [sp, #0]
	ldrh r3, [r4]
	strh r3, [r1]
	movs r3, #1
	adds r1, #2
	add r10, r3
.L_0804bb70:
	mov r4, r12
	ldrb r3, [r4]
	adds r6, #1
	cmp r6, r3
	blt .L_0804bb26
.L_0804bb7a:
	movs r2, #1
	negs r2, r2
	add r8, r2
	mov r3, r8
	adds r5, #2
	cmp r3, #0
	bne .L_0804baf4
.L_0804bb88:
	ldr r4, [sp, #8]
	lsls r3, r7, #4
	str r7, [r4]
	add r11, r3
	ldr r3, [sp, #12]
	mov r2, r11
	mov r0, r10
	str r2, [r3]
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
