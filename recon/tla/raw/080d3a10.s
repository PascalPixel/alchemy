.syntax unified
	.thumb
	.global Func_080d3a10
	.thumb_func
Func_080d3a10:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #12
	mov r8, r1
	adds r5, r2, #0
	bl Object_GetById
	adds r6, r0, #0
	ldr r7, [r6, #80]
	cmp r6, #0
	beq .L_080d3b16
	cmp r7, #0
	beq .L_080d3b16
	movs r2, #1
	negs r2, r2
	cmp r5, r2
	beq .L_080d3a3e
	adds r1, r5, #0
	bl Object_SetMode
.L_080d3a3e:
	ldr r1, [r6, #8]
	mov r5, sp
	movs r3, #128
	str r1, [r5]
	lsls r3, r3, #6
	add r8, r3
	movs r2, #192
	lsls r2, r2, #8
	mov r3, r8
	ands r3, r2
	mov r9, r2
	ldr r2, [r6, #16]
	mov r8, r3
	str r2, [r5, #8]
	movs r3, #34
	adds r3, r3, r6
	ldrb r0, [r3]
	mov r10, r3
	movs r3, #255
	bl Func_080202c0
	movs r0, #128
	lsls r0, r0, #13
	mov r1, r8
	adds r2, r5, #0
	bl Func_0801489c
	mov r2, r10
	ldrb r0, [r2]
	movs r3, #255
	ldr r1, [r5]
	ldr r2, [r5, #8]
	bl Func_080202c0
	movs r3, #128
	lsls r3, r3, #7
	cmp r8, r3
	beq .L_080d3ae8
	cmp r8, r3
	bhi .L_080d3a96
	mov r3, r8
	cmp r3, #0
	beq .L_080d3aa4
	b .L_080d3af8
.L_080d3a96:
	movs r2, #128
	lsls r2, r2, #8
	cmp r8, r2
	beq .L_080d3ac4
	cmp r8, r9
	beq .L_080d3ae2
	b .L_080d3af8
.L_080d3aa4:
	ldr r3, [r6, #8]
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r6, #8]
	ldr r3, [r6, #16]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r6, #16]
	ldrb r3, [r7, #22]
	subs r3, #4
	strb r3, [r7, #22]
	ldrb r3, [r7, #23]
	adds r3, #15
	b .L_080d3af6
.L_080d3ac4:
	ldr r3, [r6, #8]
	ldr r2, .L_080d3b24
	adds r3, r3, r2
	str r3, [r6, #8]
	ldr r3, [r6, #16]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r6, #16]
	ldrb r3, [r7, #22]
	adds r3, #3
	strb r3, [r7, #22]
	ldrb r3, [r7, #23]
	adds r3, #16
	b .L_080d3af6
.L_080d3ae2:
	ldrb r3, [r7, #23]
	adds r3, #1
	b .L_080d3af6
.L_080d3ae8:
	ldr r3, [r6, #16]
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r6, #16]
	ldrb r3, [r7, #23]
	adds r3, #13
.L_080d3af6:
	strb r3, [r7, #23]
.L_080d3af8:
	adds r3, r6, #0
	adds r3, #89
	movs r0, #0
	strb r0, [r3]
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #7
	add r3, r8
	strb r0, [r7, #26]
	strh r3, [r7, #18]
.L_080d3b16:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080d3b24:
	.4byte 0xfff80000
