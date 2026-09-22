.syntax unified
	.thumb
	.set sub_08077008, 0x08077008
	.set sub_080c1ebc, 0x080c1ebc
	.global Overlay_080bac6c
Overlay_080bac6c:
	push	{r5, r6, lr}
	ldr	r3, [pc, #40]
	adds	r6, r0, #0
	ldr	r5, [r3, #0]
	bl	sub_08077008
	movs	r3, #149
	lsls	r3, r3, #1
	adds	r2, r0, r3
	ldr	r1, [pc, #20]
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r2, #88
	b.n	.L_080bac8a
.L_080bac88:
	adds	r2, #2
.L_080bac8a:
	ldrsh	r3, [r2, r5]
	cmp	r3, r6
	bne.n	.L_080bac9c
	strh	r1, [r2, r5]
	b.n	.L_080bacc4
	.4byte 0x000000fe
	.2byte 0x1e74
	.2byte 0x0300
.L_080bac9c:
	cmp	r3, #255
	bne.n	.L_080bac88
	movs	r1, #0
	adds	r0, r5, #2
.L_080baca4:
	lsls	r3, r1, #1
	adds	r2, r3, #0
	adds	r2, #100
	ldrsh	r3, [r0, r2]
	cmp	r3, r6
	bne.n	.L_080bacb6
	ldr	r3, [pc, #12]
	strh	r3, [r0, r2]
	b.n	.L_080bacc4
.L_080bacb6:
	adds	r1, #1
	cmp	r3, #255
	bne.n	.L_080baca4
	b.n	.L_080bace2
	movs	r0, r0
	.2byte 0x00fe
	.2byte 0x0000
.L_080bacc4:
	adds	r0, r6, #0
	bl	sub_080c1ebc
	movs	r2, #187
	movs	r1, #0
	movs	r0, #255
	lsls	r2, r2, #2
.L_080bacd2:
	ldrsh	r3, [r2, r5]
	cmp	r3, r6
	bne.n	.L_080bacda
	strh	r0, [r2, r5]
.L_080bacda:
	adds	r1, #1
	adds	r2, #16
	cmp	r1, #19
	bls.n	.L_080bacd2
.L_080bace2:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
