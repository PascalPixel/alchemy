.syntax unified
	.thumb
	.global Func_080e45a8
	.thumb_func
Func_080e45a8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r9, r3
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	sub sp, #16
	ldr r3, [r3, #16]
	add r6, sp, #4
	mov r10, r3
	ldr r3, [r3, #8]
	mov r8, r1
	str r3, [r6]
	mov r1, r10
	ldr r2, [r1, #12]
	adds r7, r0, #0
	str r2, [r6, #4]
	ldr r3, [r1, #16]
	str r3, [r6, #8]
	ldr r3, [sp, #44]
	cmp r3, #0
	beq .L_080e45f4
	movs r5, #128
	lsls r5, r5, #10
	adds r3, r2, r5
	str r3, [r6, #4]
	bl Random16
	adds r2, r6, #0
	adds r1, r0, #0
	adds r0, r5, #0
	bl Func_0801489c
	b .L_080e4604
.L_080e45f4:
	bl Random16
	adds r1, r0, #0
	movs r0, #128
	lsls r0, r0, #11
	adds r2, r6, #0
	bl Func_0801489c
.L_080e4604:
	ldr r3, [r6]
	mov r1, r8
	str r3, [r1]
	ldr r3, [r6, #4]
	str r3, [r1, #4]
	ldr r3, [r6, #8]
	str r3, [r1, #8]
	movs r3, #0
	str r3, [r6, #8]
	str r3, [r6]
	ldr r2, [sp, #44]
	cmp r2, #0
	beq .L_080e4632
	bl Random16
	adds r1, r0, #0
	movs r0, #204
	lsls r0, r0, #6
	adds r0, #51
	adds r2, r6, #0
	bl Func_0801489c
	b .L_080e4644
.L_080e4632:
	bl Random16
	adds r1, r0, #0
	movs r0, #204
	lsls r0, r0, #8
	adds r0, #204
	adds r2, r6, #0
	bl Func_0801489c
.L_080e4644:
	ldr r3, [r6]
	mov r1, r8
	str r3, [r1, #12]
	movs r3, #0
	str r3, [r1, #16]
	mov r2, r9
	ldr r3, [r6, #8]
	adds r0, r7, #0
	str r3, [r1, #20]
	movs r3, #128
	str r2, [sp, #0]
	movs r1, #8
	movs r2, #8
	lsls r3, r3, #23
	bl Func_080eaf98
	movs r3, #204
	lsls r3, r3, #6
	adds r3, #51
	str r3, [r7, #20]
	str r3, [r7, #24]
	mov r0, r10
	bl Func_080db9c0
	ldrb r2, [r7, #9]
	movs r3, #3
	ands r0, r3
	movs r3, #13
	negs r3, r3
	ands r3, r2
	lsls r0, r0, #2
	orrs r3, r0
	strb r3, [r7, #9]
	mov r0, r10
	bl Func_080db9cc
	ldrb r3, [r7, #5]
	movs r2, #32
	orrs r3, r2
	ldrb r2, [r7, #9]
	strb r3, [r7, #5]
	movs r3, #15
	adds r0, #2
	ands r3, r2
	strh r0, [r7, #30]
	strb r3, [r7, #9]
	add sp, #16
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
