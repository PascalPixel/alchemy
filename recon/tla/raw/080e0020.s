.syntax unified
	.thumb
	.global Func_080e0020
	.thumb_func
Func_080e0020:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	adds r7, r0, #0
	movs r2, #64
	adds r2, r2, r7
	sub sp, #12
	mov r8, r3
	mov r10, r2
.L_080e003c:
	mov r3, r10
	movs r6, #0
	ldrsb r6, [r3, r6]
	cmp r6, #0
	bne .L_080e0094
	ldr r3, [r7, #20]
	mov r8, sp
	str r3, [sp, #0]
	ldr r3, [r7, #24]
	str r3, [sp, #8]
	bl Random16
	adds r5, r0, #0
	bl Random16
	lsls r5, r5, #16
	adds r3, r0, #0
	lsls r0, r3, #4
	asrs r5, r5, #16
	subs r0, r0, r3
	movs r2, #160
	lsls r5, r5, #16
	lsls r2, r2, #14
	lsls r0, r0, #1
	lsrs r5, r5, #16
	adds r0, r0, r2
	adds r1, r5, #0
	mov r2, r8
	bl Vector_AddPolarOffset
	mov r2, r8
	ldr r3, [r2]
	str r3, [r7, #12]
	ldr r3, [r2, #8]
	mov r2, r10
	str r3, [r7, #16]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r7, #36]
	str r3, [r7, #32]
	adds r3, r7, #0
	adds r3, #66
	strb r6, [r3]
	b .L_080e00f6
.L_080e0094:
	cmp r6, #1
	bne .L_080e00ac
	adds r0, r7, #0
	bl BattleFx_HasReachedTarget
	cmp r0, #0
	bne .L_080e0112
	mov r2, r10
	ldrb r3, [r2]
	adds r3, #1
	strb r3, [r2]
	b .L_080e003c
.L_080e00ac:
	cmp r6, #2
	bne .L_080e00fe
	mov r2, r8
	ldr r3, [r2, #4]
	mov r5, sp
	str r3, [r5]
	adds r0, r5, #0
	ldr r3, [r2, #8]
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r5, #4]
	mov r2, r8
	ldr r3, [r2, #12]
	str r3, [r5, #8]
	bl Camera_WorldToScreen
	bl Random16
	adds r1, r0, #0
	movs r0, #128
	adds r2, r5, #0
	lsls r0, r0, #11
	bl Vector_AddPolarOffset
	ldr r3, [r5]
	adds r2, r7, #0
	str r3, [r7, #12]
	adds r2, #66
	ldr r3, [r5, #8]
	str r3, [r7, #16]
	movs r3, #128
	lsls r3, r3, #5
	strh r3, [r7, #50]
	movs r3, #1
	strb r3, [r2]
	mov r2, r10
.L_080e00f6:
	ldrb r3, [r2]
	adds r3, #1
	strb r3, [r2]
	b .L_080e0112
.L_080e00fe:
	cmp r6, #3
	bne .L_080e0112
	adds r0, r7, #0
	bl BattleFx_HasReachedTarget
	cmp r0, #0
	bne .L_080e0112
	adds r0, r7, #0
	bl BattleFx_ClearOwnedSlot
.L_080e0112:
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
