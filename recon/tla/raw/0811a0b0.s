.syntax unified
	.thumb
	.global Func_0811a0b0
	.thumb_func
Func_0811a0b0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	adds r6, r0, #0
	bl Func_080ad0f0
	adds r7, r0, #0
	cmp r7, #4
	ble .L_0811a0c8
	subs r7, #4
	b .L_0811a0ca
.L_0811a0c8:
	movs r7, #0
.L_0811a0ca:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	adds r3, #68
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0811a0da
	movs r7, #0
.L_0811a0da:
	cmp r7, #0
	ble .L_0811a110
	ldr r3, .L_0811a128
	movs r1, #135
	lsls r1, r1, #2
	adds r2, r3, r1
	movs r3, #2
	mov r8, r3
	adds r5, r7, #0
.L_0811a0ec:
	ldrb r0, [r2]
	adds r2, #1
	cmp r6, #0
	beq .L_0811a0f8
	strh r0, [r6]
	adds r6, #2
.L_0811a0f8:
	str r2, [sp, #0]
	bl Owner_GetState
	movs r1, #149
	lsls r1, r1, #1
	adds r3, r0, r1
	subs r5, #1
	mov r1, r8
	strb r1, [r3]
	ldr r2, [sp, #0]
	cmp r5, #0
	bne .L_0811a0ec
.L_0811a110:
	cmp r6, #0
	beq .L_0811a118
	ldr r3, .L_0811a124
	strh r3, [r6]
.L_0811a118:
	adds r0, r7, #0
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0811a124:
	.4byte 0x000000ff
.L_0811a128:
	.4byte gPartyState
