.syntax unified
	.thumb
	.global Func_0802b2d4
	.thumb_func
Func_0802b2d4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r7, r0, #0
	ldrh r0, [r7]
	movs r3, #255
	lsls r3, r3, #8
	mov r12, r0
	adds r3, #255
	sub sp, #8
	mov r10, r1
	mov r8, r2
	cmp r12, r3
	beq .L_0802b336
	mov r9, r3
	adds r6, r7, #2
.L_0802b2f8:
	movs r2, #0
	ldrsh r1, [r6, r2]
	movs r4, #2
	ldrsh r3, [r6, r4]
	movs r4, #4
	ldrsh r2, [r6, r4]
	movs r4, #6
	ldrsh r5, [r6, r4]
	lsls r3, r3, #16
	lsls r2, r2, #16
	lsrs r3, r3, #16
	lsrs r2, r2, #16
	lsls r1, r1, #16
	lsls r5, r5, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	lsrs r1, r1, #16
	mov r2, r10
	mov r3, r8
	lsrs r5, r5, #16
	bl Func_0802b1a0
	adds r7, #10
	adds r0, r5, #0
	bl WaitFrames
	ldrh r0, [r7]
	adds r6, #10
	mov r12, r0
	cmp r12, r9
	bne .L_0802b2f8
.L_0802b336:
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
