.syntax unified
	.thumb
	.global Func_080d784c
	.thumb_func
Func_080d784c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r5, .L_080d7930
	movs r0, #149
	lsls r0, r0, #2
	adds r3, r5, r0
	movs r1, #0
	ldrsh r7, [r3, r1]
	ldrh r2, [r3]
	movs r3, #240
	lsls r3, r3, #8
	ands r7, r3
	movs r3, #240
	lsls r3, r3, #4
	adds r3, #255
	mov r10, r3
	mov r0, r10
	ands r0, r2
	mov r10, r0
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080d7926
	cmp r7, #0
	bne .L_080d7926
	movs r3, #224
	movs r7, #128
	lsls r3, r3, #3
	mov r1, r10
	adds r3, #255
	lsls r7, r7, #4
	ands r7, r1
	ands r1, r3
	ldr r3, .L_080d7934
	mov r10, r1
	add r3, r10
	cmp r3, #80
	bhi .L_080d7926
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #86
	adds r3, r5, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	ble .L_080d7926
	subs r2, #66
	movs r1, #8
	adds r2, r2, r5
	mov r8, r1
	mov r9, r2
.L_080d78bc:
	mov r0, r8
	bl BattleAction_FindDescriptor
	adds r5, r0, #0
	cmp r5, #0
	beq .L_080d791c
	movs r0, #2
	ldrsh r3, [r5, r0]
	ldr r2, .L_080d7934
	subs r3, #48
	add r2, r10
	cmp r3, r2
	bne .L_080d791c
	mov r0, r8
	bl ObjectTable_Get
	adds r6, r0, #0
	cmp r6, #0
	beq .L_080d791c
	cmp r7, #0
	bne .L_080d78fc
	adds r2, r6, #0
	adds r2, #85
	movs r3, #3
	str r7, [r6, #20]
	strb r3, [r2]
	ldr r1, [r5, #8]
	ldr r2, [r5, #12]
	ldr r3, [r5, #16]
	bl Object_SetPositionAndResetMotionFar
	b .L_080d7914
.L_080d78fc:
	mov r1, r9
	ldr r0, [r1]
	bl ObjectTable_Get
	ldr r3, [r0, #16]
	ldr r1, [r0, #8]
	ldr r2, [r0, #12]
	ldr r0, .L_080d7938
	adds r3, r3, r0
	adds r0, r6, #0
	bl Object_SetPositionAndResetMotionFar
.L_080d7914:
	adds r0, r6, #0
	movs r1, #1
	bl Object_SetMode
.L_080d791c:
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #79
	ble .L_080d78bc
.L_080d7926:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_080d7930:
	.4byte gPartyState
.L_080d7934:
	.4byte 0xfffffed4
.L_080d7938:
	.4byte 0xffe00000
