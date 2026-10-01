.syntax unified
	.thumb
	.global Func_0818de3c
	.thumb_func
Func_0818de3c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #44
	str r0, [sp, #40]
	movs r0, #0
	mov r9, r1
	str r2, [sp, #36]
	str r3, [sp, #32]
	str r0, [sp, #28]
	cmp r2, #0
	beq .L_0818df32
	ldr r1, [sp, #76]
	str r0, [sp, #12]
	subs r1, #1
	str r1, [sp, #16]
	str r0, [sp, #8]
.L_0818de66:
	movs r2, #0
	mov r3, r9
	mov r10, r2
	cmp r3, #0
	beq .L_0818df12
	ldr r0, [sp, #36]
	ldr r2, [sp, #12]
	subs r0, #1
	ldr r1, [sp, #8]
	ldr r3, [sp, #16]
	str r0, [sp, #20]
	str r2, [sp, #24]
	mov r8, r1
	mov r11, r3
.L_0818de82:
	ldr r0, [sp, #28]
	ldr r1, [sp, #20]
	cmp r0, r1
	bge .L_0818df08
	mov r3, r8
	add r3, r10
	lsls r5, r3, #1
	ldr r2, [sp, #40]
	add r0, sp, #24
	adds r5, r5, r3
	ldr r3, [sp, #32]
	ldrb r0, [r0]
	lsls r5, r5, #3
	adds r5, r5, r2
	mov r1, r10
	muls r1, r3
	strb r0, [r5, #5]
	add r0, sp, #24
	mov r2, r11
	ldrb r0, [r0]
	strb r2, [r5, #7]
	strb r2, [r5, #9]
	adds r3, r1, r3
	adds r2, r5, #0
	adds r2, #12
	subs r3, #1
	mov r7, r10
	strb r3, [r5, #4]
	strb r1, [r5, #6]
	strb r3, [r5, #8]
	adds r7, #1
	strb r1, [r2, #4]
	strb r1, [r2, #6]
	strb r3, [r2, #8]
	mov r1, r11
	adds r3, r0, #0
	strb r0, [r2, #5]
	strb r1, [r2, #7]
	strb r3, [r2, #9]
	mov r1, r9
	adds r0, r7, #0
	str r2, [sp, #4]
	bl __modsi3
	adds r6, r0, #0
	mov r0, r8
	adds r4, r6, r0
	strb r4, [r5]
	mov r1, r9
	mov r0, r10
	str r4, [sp, #0]
	bl __modsi3
	mov r1, r9
	ldr r2, [sp, #4]
	ldr r4, [sp, #0]
	adds r3, r0, r1
	add r6, r9
	add r3, r8
	add r6, r8
	add r0, r8
	strb r3, [r5, #1]
	strb r6, [r5, #2]
	strb r0, [r2]
	strb r3, [r2, #1]
	strb r4, [r2, #2]
	b .L_0818df0c
.L_0818df08:
	mov r7, r10
	adds r7, #1
.L_0818df0c:
	mov r10, r7
	cmp r10, r9
	bne .L_0818de82
.L_0818df12:
	ldr r3, [sp, #76]
	ldr r2, [sp, #16]
	ldr r0, [sp, #12]
	adds r2, r2, r3
	ldr r1, [sp, #8]
	str r2, [sp, #16]
	ldr r2, [sp, #28]
	adds r0, r0, r3
	ldr r3, [sp, #36]
	add r1, r9
	adds r2, #1
	str r0, [sp, #12]
	str r1, [sp, #8]
	str r2, [sp, #28]
	cmp r2, r3
	bne .L_0818de66
.L_0818df32:
	ldr r0, [sp, #36]
	movs r2, #0
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #3
	subs r3, #24
	mov r1, r9
	muls r1, r3
	ldr r0, [sp, #40]
	adds r3, r1, #0
	strb r2, [r0, r3]
	adds r3, r0, r3
	strb r2, [r3, #1]
	strb r2, [r3, #2]
	add sp, #44
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
