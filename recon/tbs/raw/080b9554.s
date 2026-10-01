.syntax unified
	.thumb
	.global Func_080b9554
	.thumb_func
Func_080b9554:
	push {r5, r6, r7, lr}
	mov r7, r9
	push {r7}
	sub sp, #4
	mov r3, sp
	mov r2, r9
	str r2, [r3]
	adds r7, r2, #0
	subs r3, r7, #4
	ldr r0, [r3]
	movs r1, #20
	bl SerialRuntime_BeginTransferA
	movs r3, #1
	movs r5, #150
	negs r3, r3
	movs r6, #0
	lsls r5, r5, #1
	cmp r0, r3
	bne .L_080b95a0
	b .L_080b95f4
.L_080b957e:
	movs r0, #1
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	blt .L_080b95e2
	ldr r3, .L_080b9600
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	beq .L_080b959e
	adds r6, #1
	cmp r6, #24
	ble .L_080b95a0
	b .L_080b95e2
.L_080b959e:
	movs r6, #0
.L_080b95a0:
	bl SerialRuntime_GetActiveTransfers
	cmp r0, #0
	bne .L_080b957e
	adds r3, r7, #0
	subs r3, #8
	ldr r1, [r3]
	cmp r1, #0
	beq .L_080b95f2
	subs r3, #4
	ldr r0, [r3]
	bl SerialRuntime_BeginTransferA
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	bne .L_080b95ea
	b .L_080b95f4
.L_080b95c4:
	movs r0, #1
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	blt .L_080b95e2
	ldr r3, .L_080b9600
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	beq .L_080b95e8
	adds r6, #1
	cmp r6, #24
	ble .L_080b95ea
.L_080b95e2:
	movs r0, #1
	negs r0, r0
	b .L_080b95f4
.L_080b95e8:
	movs r6, #0
.L_080b95ea:
	bl SerialRuntime_GetActiveTransfers
	cmp r0, #0
	bne .L_080b95c4
.L_080b95f2:
	movs r0, #0
.L_080b95f4:
	add sp, #4
	pop {r3}
	mov r9, r3
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_080b9600:
	.4byte gLinkStatus
