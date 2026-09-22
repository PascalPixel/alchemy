.syntax unified
	.thumb
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08016ce4, 0x08016ce4
	.set sub_08016cfc, 0x08016cfc
	.set sub_08016d18, 0x08016d18
	.set sub_080f93f4, 0x080f93f4
	.set sub_081c0010, 0x081c0010
	.global Overlay_080f93c4
Overlay_080f93c4:
	push	{lr}
	ldr	r3, [pc, #96]
	ldr	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #0
	bne.n	sub_080f93f4
	movs	r3, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080f93e8
	ldr	r1, [pc, #80]
	ldr	r3, [pc, #80]
	ldr	r0, [pc, #84]
	movs	r2, #32
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe005
.L_080f93e8:
	ldr	r3, [pc, #76]
	ldr	r0, [pc, #72]
	movs	r1, #32
	ldr	r2, [pc, #76]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b12
	movs	r2, #8
	ldr	r3, [r3, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080f9414
	movs	r0, #113
	bl	sub_081c0010
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016cfc
	ldr	r0, [pc, #52]
	bl	sub_08014644
.L_080f9414:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	movs	r2, #227
	lsls	r2, r2, #4
	adds	r3, r3, r2
.L_080f9420:
	ldr	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080f9420
	pop	{pc}
	.4byte 0x0300122c
	.4byte 0x08105948
	.4byte 0x03000730
	.4byte 0x06002540
	.4byte 0x03000260
	.4byte 0x44444444
	.4byte 0x03001150
	.2byte 0x93c5
	.2byte 0x080f
	push	{lr}
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016d18
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #8]
	bl	sub_080145a8
	pop	{pc}
	movs	r0, r0
	.2byte 0x93c5
	.2byte 0x080f
	push	{lr}
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080f9478
	ldr	r0, [pc, #8]
	bl	sub_08014644
.L_080f9478:
	pop	{pc}
	movs	r0, r0
	.4byte 0x080f93c5
