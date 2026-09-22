.syntax unified
	.thumb
	.set sub_08002df0, 0x08002df0
	.set sub_08004970, 0x08004970
	.set sub_080056cc, 0x080056cc
	.set sub_08005920, 0x08005920
	.set sub_08005a78, 0x08005a78
	.set sub_08005cf8, 0x08005cf8
	.set sub_080072f0, 0x080072f0
	.set sub_0801776c, 0x0801776c
	.global Overlay_0801faa8
Overlay_0801faa8:
	push	{r5, r6, r7, lr}
	movs	r0, #128
	lsls	r0, r0, #5
	bl	sub_08004970
	ldr	r6, [pc, #124]
	adds	r5, r0, #0
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	movs	r3, #1
	negs	r3, r3
	movs	r7, #0
	cmp	r0, r3
	beq.n	.L_0801fb28
	bl	sub_080056cc
	cmp	r0, #0
	beq.n	.L_0801fad8
	ldr	r0, [pc, #100]
	movs	r1, #1
	movs	r7, #9
	bl	sub_0801776c
	b.n	.L_0801fb1a
.L_0801fad8:
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	adds	r1, r5, #0
	bl	sub_08005a78
	cmp	r0, #0
	beq.n	.L_0801faf2
	ldr	r0, [pc, #80]
	movs	r1, #1
	bl	sub_0801776c
	movs	r7, #2
	negs	r7, r7
.L_0801faf2:
	ldr	r1, [pc, #72]
	ldr	r3, [pc, #72]
	adds	r0, r5, r1
	subs	r0, r0, r3
	movs	r2, #16
	ldr	r3, [pc, #68]
	bl	sub_080072f0
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	adds	r1, r5, #0
	bl	sub_08005920
	cmp	r0, #0
	beq.n	.L_0801fb1c
	ldr	r0, [pc, #36]
	movs	r1, #1
	bl	sub_0801776c
	movs	r7, #3
.L_0801fb1a:
	negs	r7, r7
.L_0801fb1c:
	bl	sub_08005cf8
	adds	r0, r5, #0
	bl	sub_08002df0
	adds	r0, r7, #0
.L_0801fb28:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x02002004
	.4byte 0x0000000a
	.4byte 0x0000000b
	.4byte 0x020004e4
	.4byte 0x02000000
	.4byte 0x03001388
