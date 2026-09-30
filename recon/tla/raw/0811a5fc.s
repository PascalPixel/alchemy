.syntax unified
	.thumb
	.global Func_0811a5fc
	.thumb_func
Func_0811a5fc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, .L_0811a62c
	ldr r7, [r3, #36]
	mov r8, r0
	movs r6, #0
	mov r10, r2
	movs r5, #4
.L_0811a614:
	ldrsh r3, [r5, r7]
	cmp r3, r8
	bne .L_0811a630
	movs r3, #0
	adds r0, r6, #0
	movs r1, #0
	movs r2, #0
	bl Func_080202b8
	mov r3, r10
	strh r3, [r5, r7]
	b .L_0811a630
.L_0811a62c:
	.4byte 0x00000000
.L_0811a630:
	adds r6, #1
	adds r5, #2
	cmp r6, #5
	ble .L_0811a614
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
