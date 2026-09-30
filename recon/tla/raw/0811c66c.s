.syntax unified
	.thumb
	.global Func_0811c66c
	.thumb_func
Func_0811c66c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #28
	bl Random16
	lsls r0, r0, #4
	lsrs r0, r0, #16
	cmp r0, #0
	beq .L_0811c6b6
	mov r5, sp
	movs r0, #1
	adds r1, r5, #0
	bl Func_0811a188
	adds r6, r0, #0
	movs r7, #0
	cmp r6, #0
	beq .L_0811c6ac
	mov r8, r5
	movs r5, #0
.L_0811c696:
	mov r2, r8
	ldrsh r0, [r5, r2]
	bl Func_0811bec8
	adds r7, #1
	movs r0, #8
	bl WaitFrames
	adds r5, #2
	cmp r7, r6
	bne .L_0811c696
.L_0811c6ac:
	movs r0, #22
	bl WaitFrames
	movs r0, #1
	b .L_0811c6be
.L_0811c6b6:
	ldr r0, .L_0811c6c8
	bl Func_080381c0 + 0x8
	movs r0, #0
.L_0811c6be:
	add sp, #28
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0811c6c8:
	.4byte 0x00000c99
