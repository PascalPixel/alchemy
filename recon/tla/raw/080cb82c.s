.syntax unified
	.thumb
	.global Func_080cb82c
	.thumb_func
Func_080cb82c:
	push {r5, r6, lr}
	movs r1, #213
	lsls r1, r1, #4
	movs r0, #108
	bl Runtime_AllocateBlock
	movs r1, #197
	adds r2, r0, #0
	lsls r1, r1, #1
	adds r3, r2, r1
	ldrh r3, [r3]
	cmp r3, #3
	bne .L_080cb8a0
	adds r1, #1
	adds r3, r2, r1
	movs r5, #1
	strb r5, [r3]
	adds r1, #65
	adds r3, r2, r1
	ldr r3, [r3]
	movs r1, #168
	adds r3, #91
	strb r5, [r3]
	lsls r1, r1, #3
	movs r0, #124
	movs r6, #0
	bl Runtime_AllocateBlock
	cmp r0, #0
	beq .L_080cb89c
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #61
	adds r1, r0, r2
	movs r3, #0
	ldrsb r3, [r1, r3]
	cmp r3, #0
	beq .L_080cb89c
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #58
	adds r2, r0, r3
	movs r3, #80
	strb r3, [r2]
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #59
	adds r3, r0, r2
	adds r2, #1
	strb r6, [r3]
	adds r3, r0, r2
	strb r5, [r3]
	movs r0, #2
	strb r6, [r1]
	bl WaitFrames
.L_080cb89c:
	bl Func_08020268
.L_080cb8a0:
	pop {r5, r6, pc}
	.2byte 0x0000
