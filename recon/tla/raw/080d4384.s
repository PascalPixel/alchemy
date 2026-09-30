.syntax unified
	.thumb
	.global Object_AttachWorkTargetToObject
	.thumb_func
Object_AttachWorkTargetToObject:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	mov r10, r1
	bl ObjectTable_Get
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #108]
	adds r5, r0, #0
	mov r8, r3
	movs r3, #230
	lsls r3, r3, #1
	add r3, r8
	ldr r6, [r3]
	ldr r3, [r2, #32]
	cmp r5, #0
	beq .L_080d43e4
	adds r7, r6, #0
	adds r7, #8
	str r7, [r3]
	adds r0, r6, #0
	adds r1, r5, #0
	bl ObjectDispatch_InitFromTable4WithArgumentFar
	mov r3, r10
	cmp r3, #0
	bne .L_080d43e4
	ldr r3, [r5, #8]
	movs r0, #1
	str r3, [r7]
	ldr r3, [r5, #12]
	str r3, [r6, #12]
	ldr r3, [r5, #16]
	str r3, [r6, #16]
	bl WaitFrames
	movs r3, #197
	lsls r3, r3, #1
	add r3, r8
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	beq .L_080d43e4
	bl Func_08020120
.L_080d43e4:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
