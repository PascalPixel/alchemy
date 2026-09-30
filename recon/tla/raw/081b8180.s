.syntax unified
	.thumb
	.global Func_081b8180
	.thumb_func
Func_081b8180:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #24
	str r3, [sp, #16]
	str r0, [sp, #20]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #180
	ldr r3, [r3]
	ldr r4, [sp, #60]
	str r3, [sp, #12]
	movs r3, #0
	str r3, [sp, #8]
	lsls r3, r4, #2
	adds r3, r3, r4
	lsls r3, r3, #1
	subs r1, r1, r3
	movs r6, #0
	cmp r4, #0
	beq .L_081b8262
	movs r3, #128
	lsls r3, r3, #1
	adds r3, r2, r3
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #10
	str r3, [sp, #4]
	adds r2, r2, r1
	subs r3, r4, #1
	mov r11, r2
	ldr r2, [sp, #4]
	mov r9, r3
	movs r1, #200
	lsls r3, r0, #3
	adds r1, r1, r3
	adds r7, r3, #0
	movs r3, #255
	ands r2, r3
	str r2, [sp, #4]
	mov r10, r1
	adds r7, #196
.L_081b81dc:
	subs r3, r4, r6
	ldr r0, [sp, #16]
	movs r5, #0
	cmp r3, #1
	beq .L_081b81f8
	mov r8, r9
.L_081b81e8:
	movs r1, #10
	str r4, [sp, #0]
	bl Math_Div
	adds r5, #1
	ldr r4, [sp, #0]
	cmp r5, r8
	bne .L_081b81e8
.L_081b81f8:
	movs r1, #10
	str r4, [sp, #0]
	bl __modsi3
	ldr r4, [sp, #0]
	cmp r0, #0
	beq .L_081b820c
	movs r3, #1
	str r3, [sp, #8]
	b .L_081b8218
.L_081b820c:
	ldr r1, [sp, #8]
	cmp r1, #0
	bne .L_081b8218
	subs r3, r4, #1
	cmp r6, r3
	bne .L_081b8252
.L_081b8218:
	movs r3, #128
	lsls r3, r3, #1
	ldr r1, [sp, #4]
	mov r2, r11
	adds r3, #255
	ands r3, r2
	lsls r3, r3, #16
	movs r2, #128
	orrs r3, r1
	lsls r2, r2, #23
	orrs r3, r2
	ldr r2, [sp, #12]
	movs r1, #204
	str r3, [r2, r7]
	lsls r1, r1, #2
	lsls r3, r0, #2
	adds r3, r3, r1
	ldr r1, [sp, #56]
	adds r7, #8
	lsls r2, r1, #12
	orrs r3, r2
	ldr r2, [sp, #12]
	mov r1, r10
	str r3, [r2, r1]
	ldr r3, [sp, #20]
	movs r2, #8
	adds r3, #1
	str r3, [sp, #20]
	add r10, r2
.L_081b8252:
	movs r2, #1
	movs r1, #10
	negs r2, r2
	adds r6, #1
	add r11, r1
	add r9, r2
	cmp r6, r4
	bne .L_081b81dc
.L_081b8262:
	ldr r0, [sp, #20]
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
