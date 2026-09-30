.syntax unified
	.thumb
	.global Func_080d4714
	.thumb_func
Func_080d4714:
	push {r5, r6, lr}
	movs r3, #192
	movs r1, #213
	lsls r3, r3, #18
	lsls r1, r1, #4
	movs r0, #108
	ldr r6, [r3, #32]
	bl Runtime_AllocateBlock
	movs r2, #197
	lsls r2, r2, #1
	adds r3, r0, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080d4768
	movs r2, #144
	lsls r2, r2, #4
	adds r2, #108
	adds r3, r6, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	movs r5, #0
	cmp r3, #0
	beq .L_080d4768
	movs r3, #144
	lsls r3, r3, #4
	adds r3, #108
	adds r6, r6, r3
.L_080d4750:
	movs r0, #1
	bl WaitFrames
	movs r2, #44
	adds r5, #1
	adds r2, #255
	cmp r5, r2
	bgt .L_080d4768
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #0
	bne .L_080d4750
.L_080d4768:
	pop {r5, r6, pc}
	.2byte 0x0000
