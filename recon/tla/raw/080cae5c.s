.syntax unified
	.thumb
	.global Func_080cae5c
	.thumb_func
Func_080cae5c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r2, .L_080caf68
	movs r3, #32
	mov r8, r2
	negs r3, r3
	add r3, r8
	movs r2, #128
	mov r10, r3
	lsls r2, r2, #5
	movs r3, #129
	sub sp, #8
	add r2, r8
	lsls r3, r3, #5
	str r2, [sp, #4]
	add r3, r8
	movs r2, #130
	str r3, [sp, #0]
	lsls r2, r2, #5
	add r2, r8
	mov r11, r2
	mov r2, r10
	movs r3, #0
	ldrb r7, [r2]
	mov r9, r3
	subs r3, #31
	add r3, r8
	mov r10, r3
	cmp r7, #255
	beq .L_080caf5a
.L_080caea2:
	adds r0, r7, #0
	bl ObjectTable_Get
	adds r6, r0, #0
	cmp r6, #0
	beq .L_080caf32
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	ldr r5, [r6, #80]
	adds r3, #212
	mov r0, r8
	adds r1, r6, #0
	adds r2, #32
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	str r5, [r6, #80]
	ldr r2, [sp, #4]
	adds r0, r6, #0
	ldrb r1, [r2]
	bl Object_SetMode
	ldr r3, [sp, #0]
	adds r0, r6, #0
	ldrb r1, [r3]
	bl ObjectDispatch_SetSingleChildField26Far
	mov r2, r11
	ldrb r1, [r2]
	movs r3, #3
	ldrb r2, [r5, #9]
	ands r1, r3
	movs r3, #13
	negs r3, r3
	lsls r1, r1, #2
	ands r3, r2
	orrs r3, r1
	strb r3, [r5, #9]
	adds r5, #37
	ldrb r2, [r5]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	orrs r3, r1
	strb r3, [r5]
	bl Func_080cdf5c
	cmp r7, r0
	bne .L_080caf32
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #108]
	movs r2, #230
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #192
	lsls r2, r2, #18
	ldr r1, [r3]
	ldr r3, [r2, #32]
	ldr r2, [r6, #12]
	ldr r0, [r3]
	ldr r3, [r6, #8]
	str r2, [r1, #20]
	str r3, [r1, #8]
	str r2, [r1, #12]
	ldr r3, [r6, #16]
	str r3, [r1, #16]
	str r2, [r0, #4]
	adds r0, r6, #0
	bl Object_ResetMotion
.L_080caf32:
	ldr r2, [sp, #4]
	movs r3, #128
	add r8, r3
	ldr r3, [sp, #0]
	adds r2, #1
	str r2, [sp, #4]
	movs r2, #1
	adds r3, #1
	add r9, r2
	str r3, [sp, #0]
	mov r3, r9
	add r11, r2
	cmp r3, #31
	bgt .L_080caf5a
	mov r2, r10
	ldrb r7, [r2]
	movs r3, #1
	add r10, r3
	cmp r7, #255
	bne .L_080caea2
.L_080caf5a:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080caf68:
	.4byte Data_02001024
