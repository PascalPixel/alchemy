.syntax unified
	.thumb
	.global Func_0811a188
	.thumb_func
Func_0811a188:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r10, r0
	movs r0, #182
	adds r6, r1, #0
	movs r2, #6
	movs r1, #0
	lsls r0, r0, #1
	sub sp, #20
	mov r8, r1
	mov r9, r2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0811a1b0
	movs r3, #3
	mov r9, r3
.L_0811a1b0:
	movs r3, #1
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_0811a1f2
	add r5, sp, #4
	adds r0, r5, #0
	bl Func_0811a038
	cmp r8, r0
	bge .L_0811a1f2
	adds r2, r5, #0
	adds r5, r0, #0
.L_0811a1ca:
	ldrh r7, [r2]
	adds r2, #2
	adds r0, r7, #0
	str r2, [sp, #0]
	bl Owner_GetState
	movs r1, #56
	ldrsh r3, [r0, r1]
	ldr r2, [sp, #0]
	cmp r3, #0
	ble .L_0811a1ec
	cmp r6, #0
	beq .L_0811a1e8
	strh r7, [r6]
	adds r6, #2
.L_0811a1e8:
	movs r3, #1
	add r8, r3
.L_0811a1ec:
	subs r5, #1
	cmp r5, #0
	bne .L_0811a1ca
.L_0811a1f2:
	movs r3, #2
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_0811a232
	mov r7, r9
	movs r5, #128
	adds r7, #128
	cmp r5, r7
	bge .L_0811a232
.L_0811a206:
	adds r0, r5, #0
	bl Owner_GetState
	movs r2, #149
	lsls r2, r2, #1
	adds r3, r0, r2
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0811a22c
	movs r1, #56
	ldrsh r3, [r0, r1]
	cmp r3, #0
	ble .L_0811a22c
	cmp r6, #0
	beq .L_0811a228
	strh r5, [r6]
	adds r6, #2
.L_0811a228:
	movs r2, #1
	add r8, r2
.L_0811a22c:
	adds r5, #1
	cmp r5, r7
	blt .L_0811a206
.L_0811a232:
	cmp r6, #0
	beq .L_0811a23a
	ldr r3, .L_0811a248
	strh r3, [r6]
.L_0811a23a:
	mov r0, r8
	add sp, #20
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0811a248:
	.4byte 0x000000ff
