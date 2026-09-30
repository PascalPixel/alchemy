.syntax unified
	.thumb
	.global Func_081080a8
	.thumb_func
Func_081080a8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	movs r7, #156
	mov r8, r3
	movs r1, #0
	lsls r7, r7, #2
	mov r10, r1
	movs r6, #0
	add r7, r8
.L_081080c6:
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #6
	add r3, r8
	movs r5, #0
	ldrsb r5, [r3, r5]
	cmp r5, #3
	beq .L_081080e4
	cmp r5, #4
	beq .L_081080e4
	adds r0, r6, #0
	bl Func_080ad1d8 + 0x8
	cmp r5, r0
	bne .L_081080f8
.L_081080e4:
	adds r0, r6, #0
	movs r1, #0
	bl Item_AdjustCounterFar
	cmp r0, #0
	beq .L_081080f8
	movs r2, #1
	strh r6, [r7]
	add r10, r2
	adds r7, #2
.L_081080f8:
	movs r3, #128
	lsls r3, r3, #1
	adds r6, #1
	adds r3, #255
	cmp r6, r3
	ble .L_081080c6
	mov r1, r10
	movs r2, #156
	lsls r3, r1, #1
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r2, .L_08108124
	mov r1, r8
	strh r2, [r1, r3]
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #2
	add r3, r8
	mov r2, r10
	strh r2, [r3]
	mov r0, r10
	b .L_08108128
.L_08108124:
	.4byte 0x00000000
.L_08108128:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
