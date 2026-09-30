.syntax unified
	.thumb
	.global Func_0811f3b8
	.thumb_func
Func_0811f3b8:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #36]
	adds r6, r0, #0
	bl Owner_GetState
	movs r3, #149
	lsls r3, r3, #1
	adds r2, r0, r3
	movs r3, #0
	strb r3, [r2]
	movs r2, #88
	ldrsh r3, [r2, r5]
	movs r1, #88
	cmp r3, r6
	beq .L_0811f414
	cmp r3, #255
	beq .L_0811f3ec
.L_0811f3de:
	adds r1, #2
	adds r2, r1, #0
	ldrsh r3, [r2, r5]
	cmp r3, r6
	beq .L_0811f414
	cmp r3, #255
	bne .L_0811f3de
.L_0811f3ec:
	movs r1, #0
	adds r0, r5, #2
.L_0811f3f0:
	lsls r3, r1, #1
	adds r2, r3, #0
	adds r2, #100
	ldrsh r3, [r0, r2]
	cmp r3, r6
	beq .L_0811f408
	adds r1, #1
	cmp r3, #255
	bne .L_0811f3f0
	movs r0, #1
	negs r0, r0
	b .L_0811f440
.L_0811f408:
	ldr r3, .L_0811f410
	strh r3, [r0, r2]
	b .L_0811f418
	.2byte 0x0000
.L_0811f410:
	.4byte 0x000000fe
.L_0811f414:
	ldr r3, .L_0811f43c
	strh r3, [r5, r2]
.L_0811f418:
	adds r0, r6, #0
	bl Func_08127ba0
	movs r2, #187
	movs r1, #0
	movs r0, #255
	lsls r2, r2, #2
.L_0811f426:
	ldrsh r3, [r2, r5]
	cmp r3, r6
	bne .L_0811f42e
	strh r0, [r2, r5]
.L_0811f42e:
	adds r1, #1
	adds r2, #16
	cmp r1, #19
	bls .L_0811f426
	movs r0, #0
	b .L_0811f440
	.2byte 0x0000
.L_0811f43c:
	.4byte 0x000000fe
.L_0811f440:
	pop {r5, r6, pc}
	.2byte 0x0000
