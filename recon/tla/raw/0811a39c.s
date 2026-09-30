.syntax unified
	.thumb
	.global Func_0811a39c
	.thumb_func
Func_0811a39c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #36]
	movs r3, #1
	ands r3, r0
	sub sp, #4
	mov r8, r0
	adds r6, r1, #0
	movs r2, #0
	cmp r3, #0
	beq .L_0811a3f2
	movs r3, #88
	ldrsh r3, [r7, r3]
	cmp r3, #255
	beq .L_0811a3f2
	adds r5, r7, #0
	adds r5, #88
.L_0811a3c4:
	movs r3, #0
	ldrsh r0, [r5, r3]
	cmp r0, #254
	beq .L_0811a3e8
	str r2, [sp, #0]
	bl Owner_GetState
	movs r1, #56
	ldrsh r3, [r0, r1]
	ldr r2, [sp, #0]
	cmp r3, #0
	beq .L_0811a3e8
	cmp r6, #0
	beq .L_0811a3e6
	ldrh r3, [r5]
	strh r3, [r6]
	adds r6, #2
.L_0811a3e6:
	adds r2, #1
.L_0811a3e8:
	adds r5, #2
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, #255
	bne .L_0811a3c4
.L_0811a3f2:
	movs r3, #2
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0811a436
	adds r5, r7, #2
	movs r3, #100
	ldrsh r3, [r5, r3]
	mov r8, r5
	cmp r3, #255
	beq .L_0811a436
	movs r7, #100
.L_0811a40a:
	ldrsh r0, [r5, r7]
	cmp r0, #254
	beq .L_0811a42c
	str r2, [sp, #0]
	bl Owner_GetState
	movs r1, #56
	ldrsh r3, [r0, r1]
	ldr r2, [sp, #0]
	cmp r3, #0
	beq .L_0811a42c
	cmp r6, #0
	beq .L_0811a42a
	ldrh r3, [r5, r7]
	strh r3, [r6]
	adds r6, #2
.L_0811a42a:
	adds r2, #1
.L_0811a42c:
	adds r7, #2
	mov r5, r8
	ldrsh r3, [r5, r7]
	cmp r3, #255
	bne .L_0811a40a
.L_0811a436:
	cmp r6, #0
	beq .L_0811a43e
	ldr r3, .L_0811a448
	strh r3, [r6]
.L_0811a43e:
	adds r0, r2, #0
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0811a448:
	.4byte 0x000000ff
