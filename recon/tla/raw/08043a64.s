.syntax unified
	.thumb
	.global Func_08043a64
	.thumb_func
Func_08043a64:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #12
	str r2, [sp, #4]
	str r1, [sp, #8]
	mov r8, r3
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	movs r2, #0
	ldr r1, [r3]
	mov r11, r2
	movs r3, #44
	mov r2, r8
	ldrsb r3, [r2, r3]
	movs r2, #1
	negs r2, r2
	mov r10, r0
	cmp r3, r2
	beq .L_08043b18
	movs r3, #140
	lsls r3, r3, #1
	movs r2, #0
	adds r6, r1, #0
	adds r7, r1, r3
	mov r9, r2
	adds r6, #248
	movs r4, #44
.L_08043aa6:
	mov r3, r8
	ldrsb r0, [r4, r3]
	str r4, [sp, #0]
	bl ResourceObject_CreateFar
	adds r5, r0, #0
	ldr r4, [sp, #0]
	cmp r5, #0
	beq .L_08043ad0
	movs r1, #1
	bl Animation_ApplyChildArgumentFar
	movs r3, #0
	strb r3, [r5, #26]
	movs r1, #13
	ldrb r3, [r5, #9]
	negs r1, r1
	adds r2, r1, #0
	ands r3, r2
	strb r3, [r5, #9]
	ldr r4, [sp, #0]
.L_08043ad0:
	str r5, [r6]
	mov r1, r10
	movs r2, #12
	ldrsh r3, [r1, r2]
	ldr r2, [sp, #8]
	adds r4, #1
	adds r3, r2, r3
	add r3, r9
	lsls r3, r3, #3
	adds r3, #16
	strh r3, [r7]
	movs r2, #14
	ldrsh r3, [r1, r2]
	ldr r1, [sp, #4]
	movs r2, #3
	adds r3, r1, r3
	lsls r3, r3, #3
	adds r3, #16
	strh r3, [r7, #16]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #64]
	movs r3, #1
	add r11, r3
	mov r1, r11
	adds r7, #2
	add r9, r2
	adds r6, #4
	cmp r1, #3
	bgt .L_08043b18
	mov r2, r8
	ldrsb r3, [r4, r2]
	movs r1, #1
	negs r1, r1
	cmp r3, r1
	bne .L_08043aa6
.L_08043b18:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_08043b30
	bl Scheduler_AddOrUpdateCallback
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08043b30:
	.4byte Func_08043b70
