@ Retired from C: the registered source did not reproduce these bytes
@ under the family's flags. Disassembled from the audited extent at
@ 0x08019d2c (124 bytes); the draft C stays in games/gs1/recon/en/main/.
.syntax unified
	.thumb
	.global Func_08019d2c
	.thumb_func
Func_08019d2c:
	push	{r5, lr}
	movs	r1, #1
	negs	r1, r1
	movs	r2, #0
	cmp	r0, #19
	bhi.n	.L_08019d66
	ldr	r4, [pc, #100]	@ (0x8019da0)
	movs	r5, #0
	ldrsh	r3, [r4, r5]
	cmp	r3, r1
	beq.n	.L_08019d98
	cmp	r3, r0
	bne.n	.L_08019d4c
	movs	r2, #2
	ldrsh	r1, [r4, r2]
	b.n	.L_08019d98
.L_08019d4c:
	adds	r2, #2
	lsls	r3, r2, #1
	ldrsh	r3, [r4, r3]
	movs	r5, #1
	negs	r5, r5
	cmp	r3, r5
	beq.n	.L_08019d98
	cmp	r3, r0
	bne.n	.L_08019d4c
	adds	r2, #1
	lsls	r3, r2, #1
	ldrsh	r1, [r4, r3]
	b.n	.L_08019d98
.L_08019d66:
	ldr	r4, [pc, #60]	@ (0x8019da4)
	movs	r5, #0
	ldrsh	r3, [r4, r5]
	movs	r5, #1
	negs	r5, r5
	cmp	r3, r5
	beq.n	.L_08019d98
	cmp	r3, r0
	bne.n	.L_08019d7e
	movs	r2, #2
	ldrsh	r1, [r4, r2]
	b.n	.L_08019d96
.L_08019d7e:
	adds	r2, #2
	lsls	r3, r2, #1
	ldrsh	r3, [r4, r3]
	movs	r5, #1
	negs	r5, r5
	cmp	r3, r5
	beq.n	.L_08019d98
	cmp	r3, r0
	bne.n	.L_08019d7e
	adds	r2, #1
	lsls	r3, r2, #1
	ldrsh	r1, [r4, r3]
.L_08019d96:
	adds	r1, #128	@ 0x80
.L_08019d98:
	adds	r0, r1, #0
	pop	{r5}
	pop	{r1}
	bx	r1
	.4byte 0x080367e4
	.4byte 0x0803680c
