.syntax unified
	.thumb
	.global Func_080d3940
	.thumb_func
Func_080d3940:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #12
	adds r7, r1, #0
	bl Object_GetById
	mov r8, r0
	ldr r6, [r0, #80]
	cmp r0, #0
	beq .L_080d3a04
	cmp r6, #0
	beq .L_080d3a04
	movs r1, #0
	bl Object_SetMode
	movs r2, #128
	lsls r2, r2, #6
	adds r7, r7, r2
	mov r2, r8
	ldr r1, [r2, #8]
	mov r5, sp
	str r1, [r5]
	movs r3, #192
	ldr r2, [r2, #16]
	lsls r3, r3, #8
	str r2, [r5, #8]
	ands r7, r3
	mov r9, r3
	movs r3, #34
	add r3, r8
	ldrb r0, [r3]
	mov r10, r3
	movs r3, #255
	bl Func_080202c0
	movs r0, #128
	lsls r0, r0, #13
	adds r1, r7, #0
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	mov r2, r10
	ldrb r0, [r2]
	movs r3, #255
	ldr r1, [r5]
	ldr r2, [r5, #8]
	bl Func_080202c0
	movs r3, #128
	lsls r3, r3, #7
	cmp r7, r3
	beq .L_080d39e0
	cmp r7, r3
	bhi .L_080d39b8
	cmp r7, #0
	beq .L_080d39c6
	b .L_080d39e6
.L_080d39b8:
	movs r3, #128
	lsls r3, r3, #8
	cmp r7, r3
	beq .L_080d39ce
	cmp r7, r9
	beq .L_080d39da
	b .L_080d39e6
.L_080d39c6:
	ldrb r3, [r6, #22]
	subs r3, #3
	strb r3, [r6, #22]
	b .L_080d39e0
.L_080d39ce:
	ldrb r3, [r6, #22]
	adds r3, #2
	strb r3, [r6, #22]
	ldrb r3, [r6, #23]
	adds r3, #4
	b .L_080d39e4
.L_080d39da:
	ldrb r3, [r6, #23]
	adds r3, #1
	b .L_080d39e4
.L_080d39e0:
	ldrb r3, [r6, #23]
	adds r3, #5
.L_080d39e4:
	strb r3, [r6, #23]
.L_080d39e6:
	mov r3, r8
	adds r3, #89
	movs r0, #0
	strb r0, [r3]
	mov r1, r8
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	movs r2, #128
	lsls r2, r2, #7
	strb r3, [r1]
	adds r3, r7, r2
	strb r0, [r6, #26]
	strh r3, [r6, #18]
.L_080d3a04:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
