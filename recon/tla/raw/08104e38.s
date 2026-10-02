.syntax unified
	.thumb
	.global Func_08104e38
	.thumb_func
Func_08104e38:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	str r0, [sp, #4]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	movs r2, #0
	ldr r6, [r3]
	mov r9, r2
	movs r2, #172
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r2, #1
	negs r2, r2
	mov r10, r2
	movs r2, #7
	mov r11, r1
	movs r7, #248
	mov r8, r2
.L_08104e6a:
	ldr r5, [r7, r6]
	cmp r5, #0
	beq .L_08104e82
	adds r0, r5, #0
	str r3, [sp, #0]
	bl ResourceObject_ReleaseFar
	ldr r3, [sp, #0]
	mov r2, r9
	str r2, [r7, r6]
	mov r2, r10
	strh r2, [r3]
.L_08104e82:
	movs r2, #1
	negs r2, r2
	add r8, r2
	mov r2, r8
	adds r3, #2
	adds r7, #4
	cmp r2, #0
	bge .L_08104e6a
	movs r3, #0
	movs r2, #172
	adds r7, r6, #0
	mov r8, r3
	lsls r2, r2, #1
	subs r3, #13
	adds r7, #248
	adds r6, r6, r2
	mov r10, r3
.L_08104ea4:
	mov r2, r8
	ldr r0, [sp, #4]
	cmp r2, #0
	beq .L_08104eae
	mov r0, r11
.L_08104eae:
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	beq .L_08104eda
	strh r0, [r6]
	bl Func_080c82b8
	bl ResourceObject_CreateFar
	adds r5, r0, #0
	cmp r5, #0
	beq .L_08104ed8
	movs r1, #1
	bl Animation_ApplyChildArgumentFar
	ldrb r3, [r5, #9]
	mov r2, r10
	ands r3, r2
	strb r3, [r5, #9]
	movs r3, #0
	strb r3, [r5, #26]
.L_08104ed8:
	str r5, [r7]
.L_08104eda:
	movs r3, #1
	add r8, r3
	mov r2, r8
	adds r7, #4
	adds r6, #2
	cmp r2, #1
	ble .L_08104ea4
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
