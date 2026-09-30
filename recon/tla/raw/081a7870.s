.syntax unified
	.thumb
	.global Func_081a7870
	.thumb_func
Func_081a7870:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	adds r7, r0, #0
	adds r6, r1, #0
	adds r5, r2, #0
	mov r8, r3
	cmp r3, #0
	ble .L_081a78b2
	ldr r1, .L_081a78bc
	movs r2, #160
	lsls r2, r2, #3
	mov r10, r1
	adds r2, #255
.L_081a7890:
	movs r1, #0
	ldrsh r3, [r7, r1]
	movs r1, #0
	ldrsh r0, [r6, r1]
	str r2, [sp, #0]
	subs r0, r0, r3
	mov r1, r8
	mov lr, r10
	.2byte 0xf800
	ldr r2, [sp, #0]
	strh r0, [r5]
	subs r2, #1
	adds r7, #2
	adds r6, #2
	adds r5, #2
	cmp r2, #0
	bge .L_081a7890
.L_081a78b2:
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_081a78bc:
	.4byte IwramSignedDivide
