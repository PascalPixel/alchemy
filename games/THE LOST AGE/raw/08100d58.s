.syntax unified
	.thumb
	.set sub_08002064, 0x08002064
	.set sub_080ad078, 0x080ad078
	.set sub_080f93b8, 0x080f93b8
	.global Overlay_08100d58
Overlay_08100d58:
	push	{lr}
	bl	sub_080ad078
	ldrb	r3, [r0, #1]
	movs	r2, #15
	ands	r2, r3
	cmp	r2, #1
	beq.n	.L_08100d6e
	cmp	r2, #11
	beq.n	.L_08100d74
	b.n	.L_08100d7c
.L_08100d6e:
	movs	r0, #126
	bl	sub_080f93b8
.L_08100d74:
	movs	r0, #126
	bl	sub_080f93b8
	b.n	.L_08100e22
.L_08100d7c:
	ldrb	r3, [r0, #3]
	subs	r0, r3, #1
	cmp	r0, #31
	bhi.n	.L_08100e1c
	ldr	r2, [pc, #156]
	lsls	r3, r0, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x08100e22
	.4byte 0x08100e22
	.4byte 0x08100e14
	.4byte 0x08100e1c
	.4byte 0x08100e0c
	.4byte 0x08100e1c
	.4byte 0x08100e1c
	.4byte 0x08100e1c
	.4byte 0x08100e1c
	.4byte 0x08100e1c
	.4byte 0x08100e1c
	.4byte 0x08100e1c
	.4byte 0x08100e1c
	.4byte 0x08100e1c
	.4byte 0x08100e1c
	.4byte 0x08100e1c
	.4byte 0x08100e1c
	.4byte 0x08100e1c
	.4byte 0x08100e1c
	.4byte 0x08100e1c
	.4byte 0x08100e1c
	.4byte 0x08100e1c
	.4byte 0x08100e1c
	.4byte 0x08100e1c
	.4byte 0x08100e1c
	.4byte 0x08100e1c
	.4byte 0x08100e1c
	.4byte 0x08100e1c
	.4byte 0x08100e1c
	.4byte 0x08100e1c
	.4byte 0x08100e22
	.2byte 0x0e22
	.2byte 0x0810
	movs	r0, #82
	bl	sub_080f93b8
	b.n	.L_08100e22
	movs	r0, #84
	bl	sub_080f93b8
	b.n	.L_08100e22
.L_08100e1c:
	movs	r0, #91
	bl	sub_080f93b8
.L_08100e22:
	pop	{pc}
	.2byte 0x0d8c
	.2byte 0x0810
	push	{lr}
	adds	r0, r0, r1
	bl	sub_08002064
	pop	{pc}
