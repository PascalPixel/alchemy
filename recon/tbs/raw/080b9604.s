.syntax unified
	.thumb
	.global Func_080b9604
	.thumb_func
Func_080b9604:
	push {r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	mov r1, r9
	sub sp, #4
	mov r8, r1
	mov r3, sp
	mov r7, r8
	str r1, [r3]
	subs r7, #4
	ldr r0, [r7]
	bl Party_Check
	movs r2, #1
	movs r5, #150
	negs r2, r2
	movs r6, #0
	lsls r5, r5, #1
	cmp r0, r2
	bne .L_080b965a
	b .L_080b970c
.L_080b9630:
	ldr r3, .L_080b971c
	ldrh r3, [r3]
	cmp r3, #20
	bhi .L_080b9704
	movs r0, #1
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	blt .L_080b9704
	ldr r3, .L_080b9720
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	beq .L_080b9658
	adds r6, #1
	cmp r6, #24
	ble .L_080b965a
	b .L_080b9704
.L_080b9658:
	movs r6, #0
.L_080b965a:
	bl SerialRuntime_GetActiveTransfers
	cmp r0, #0
	bne .L_080b9630
	ldr r3, .L_080b971c
	ldrh r3, [r3]
	cmp r3, #20
	bne .L_080b9704
	movs r3, #16
	negs r3, r3
	add r3, r8
	mov r9, r3
	ldr r3, [r7]
	ldr r2, [r3]
	mov r1, r9
	str r2, [r1]
	ldr r3, [r3]
	cmp r3, #0
	beq .L_080b970a
	mov r3, r8
	mov r2, r8
	subs r3, #20
	subs r2, #12
	ldr r3, [r3]
	ldr r0, [r2]
	lsls r3, r3, #4
	adds r0, r0, r3
	bl Party_Check
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	bne .L_080b96de
	b .L_080b970c
.L_080b969e:
	ldr r3, .L_080b971c
	ldrh r3, [r3]
	mov r8, r3
	mov r3, r9
	ldr r0, [r3]
	lsls r0, r0, #4
	adds r0, #19
	movs r1, #20
	bl __udivsi3
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #2
	cmp r8, r3
	bhi .L_080b9704
	movs r0, #1
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	blt .L_080b9704
	ldr r3, .L_080b9720
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	beq .L_080b96dc
	adds r6, #1
	cmp r6, #24
	ble .L_080b96de
	b .L_080b9704
.L_080b96dc:
	movs r6, #0
.L_080b96de:
	bl SerialRuntime_GetActiveTransfers
	cmp r0, #0
	bne .L_080b969e
	mov r1, r9
	ldr r0, [r1]
	ldr r3, .L_080b971c
	lsls r0, r0, #4
	ldrh r3, [r3]
	adds r0, #19
	movs r1, #20
	mov r8, r3
	bl __udivsi3
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #2
	cmp r8, r3
	beq .L_080b970a
.L_080b9704:
	movs r0, #1
	negs r0, r0
	b .L_080b970c
.L_080b970a:
	movs r0, #0
.L_080b970c:
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r9, r5
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
.L_080b971c:
	.4byte gSerialReceivedSize
.L_080b9720:
	.4byte gLinkStatus
