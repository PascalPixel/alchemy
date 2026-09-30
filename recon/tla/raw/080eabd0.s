.syntax unified
	.thumb
	.global Func_080eabd0
	.thumb_func
Func_080eabd0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	sub sp, #12
	str r3, [sp, #8]
	movs r1, #212
	lsls r1, r1, #1
	adds r3, r3, r1
	ldr r3, [r3]
	movs r2, #255
	str r3, [sp, #4]
	lsls r2, r2, #8
	ldrh r3, [r0]
	adds r2, #255
	mov r11, r0
	cmp r3, r2
	beq .L_080eaca4
.L_080eac00:
	mov r2, r11
	ldrh r0, [r2]
	bl Object_GetById
	adds r6, r0, #0
	adds r7, r6, #0
	adds r7, #34
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	ldrb r0, [r7]
	bl Func_080202f0
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	mov r9, r0
	ldrb r0, [r7]
	bl Func_080201c0
	adds r4, r0, #0
	asrs r4, r4, #19
	adds r0, r6, #0
	str r4, [sp, #0]
	bl Func_080eab70
	ldrb r2, [r7]
	movs r1, #156
	lsls r3, r2, #3
	subs r3, r3, r2
	ldr r2, [sp, #8]
	lsls r1, r1, #1
	lsls r3, r3, #3
	adds r3, r3, r1
	ldr r3, [r2, r3]
	ldr r5, .L_080eacdc
	mov r10, r3
	ldr r3, .L_080eace0
	add r5, r10
	asrs r5, r5, #2
	adds r2, r6, #0
	adds r5, r5, r3
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r2, #4
	mov r8, r0
	strb r3, [r2]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26Far
	ldr r4, [sp, #0]
	mov r1, r9
	adds r4, #4
	adds r2, r4, #0
	ldrb r0, [r7]
	bl Func_080eab98
	mov r1, r8
	lsls r1, r1, #2
	add r5, r8
	mov r8, r1
	mov r1, r10
	add r1, r8
	ldrb r2, [r1, #3]
	movs r3, #128
	str r2, [r6, #76]
	orrs r2, r3
	strb r2, [r1, #3]
	strb r0, [r5]
	ldr r1, [sp, #4]
	movs r2, #2
	add r1, r8
	movs r3, #255
	add r11, r2
	strb r3, [r1, #2]
	mov r1, r11
	movs r2, #255
	ldrh r3, [r1]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_080eac00
.L_080eaca4:
	ldr r3, .L_080eace4
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	adds r5, r0, #0
	adds r3, r5, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	bl Func_080201c0
	ldr r3, [r5, #12]
	cmp r3, r0
	bge .L_080eaccc
	str r0, [r5, #20]
	str r0, [r5, #12]
.L_080eaccc:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080eacdc:
	.4byte 0xfdff0000
.L_080eace0:
	.4byte Data_02024000
.L_080eace4:
	.4byte gPartyState
