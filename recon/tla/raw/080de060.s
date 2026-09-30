.syntax unified
	.thumb
	.global Func_080de060
	.thumb_func
Func_080de060:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	adds r7, r0, #0
	movs r1, #64
	adds r1, r1, r7
	sub sp, #12
	mov r10, r3
	mov r8, r1
.L_080de07c:
	mov r2, r8
	movs r6, #0
	ldrsb r6, [r2, r6]
	cmp r6, #0
	bne .L_080de0be
	ldr r3, [r7, #20]
	mov r5, sp
	str r3, [r5]
	ldr r3, [r7, #24]
	str r3, [r5, #8]
	bl Random16
	adds r1, r0, #0
	lsls r1, r1, #16
	movs r0, #200
	lsrs r1, r1, #16
	lsls r0, r0, #13
	adds r2, r5, #0
	bl Func_0801489c
	ldr r3, [r5]
	mov r1, r8
	str r3, [r7, #12]
	ldr r3, [r5, #8]
	str r3, [r7, #16]
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r7, #36]
	str r3, [r7, #32]
	adds r3, r7, #0
	adds r3, #66
	strb r6, [r3]
	b .L_080de12e
.L_080de0be:
	cmp r6, #1
	bne .L_080de0d6
	adds r0, r7, #0
	bl Func_080ebe70
	cmp r0, #0
	bne .L_080de14a
	mov r2, r8
	ldrb r3, [r2]
	adds r3, #1
	strb r3, [r2]
	b .L_080de07c
.L_080de0d6:
	cmp r6, #2
	bne .L_080de136
	mov r3, r10
	ldr r2, [r3, #16]
	mov r5, sp
	ldr r3, [r2, #8]
	movs r1, #128
	str r3, [r5]
	lsls r1, r1, #13
	ldr r3, [r2, #12]
	movs r0, #128
	adds r3, r3, r1
	str r3, [r5, #4]
	lsls r0, r0, #12
	ldr r3, [r2, #16]
	mov r2, r10
	str r3, [r5, #8]
	ldrh r1, [r2]
	adds r2, r5, #0
	bl Func_0801489c
	adds r0, r5, #0
	bl Func_080dc390
	bl Random16
	adds r1, r0, #0
	movs r0, #128
	adds r2, r5, #0
	lsls r0, r0, #11
	bl Func_0801489c
	ldr r3, [r5]
	adds r2, r7, #0
	str r3, [r7, #12]
	adds r2, #66
	ldr r3, [r5, #8]
	mov r1, r8
	str r3, [r7, #16]
	movs r3, #128
	lsls r3, r3, #4
	strh r3, [r7, #50]
	movs r3, #1
	strb r3, [r2]
.L_080de12e:
	ldrb r3, [r1]
	adds r3, #1
	strb r3, [r1]
	b .L_080de14a
.L_080de136:
	cmp r6, #3
	bne .L_080de14a
	adds r0, r7, #0
	bl Func_080ebe70
	cmp r0, #0
	bne .L_080de14a
	adds r0, r7, #0
	bl Func_080ebf68
.L_080de14a:
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
