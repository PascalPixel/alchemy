.syntax unified
	.thumb
	.set sub_08013164, 0x08013164
	.set sub_08013560, 0x08013560
	.set sub_08014dac, 0x08014dac
	.set sub_0801680c, 0x0801680c
	.set sub_08016854, 0x08016854
	.set sub_080168a0, 0x080168a0
	.set sub_080168cc, 0x080168cc
	.set sub_08016ca4, 0x08016ca4
	.set sub_08038020, 0x08038020
	.set sub_080ad000, 0x080ad000
	.set sub_0811a038, 0x0811a038
	.global Overlay_08119054
Overlay_08119054:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r0, #170
	lsls	r0, r0, #1
	sub	sp, #32
	bl	sub_08014dac
	movs	r2, #0
	mov	r8, r0
	mov	sl, r2
	movs	r7, #0
	b.n	.L_081190ea
.L_08119070:
	bl	sub_080168cc
	movs	r2, #149
	lsls	r2, r2, #1
	adds	r3, r6, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08119084
	movs	r3, #1
	add	sl, r3
.L_08119084:
	movs	r0, #2
	bl	sub_08013560
	mov	r5, sp
	ldr	r0, [pc, #180]
	adds	r1, r5, #0
	bl	sub_08038020
	movs	r0, #0
	ldrh	r3, [r5, r0]
	cmp	r3, #0
	beq.n	.L_081190ac
	adds	r2, r5, #0
.L_0811909e:
	adds	r0, #1
	cmp	r0, #4
	bgt.n	.L_081190ac
	adds	r2, #2
	ldrh	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_0811909e
.L_081190ac:
	adds	r4, r0, #0
	movs	r0, #14
	cmp	r0, r4
	blt.n	.L_081190cc
	subs	r3, r6, r4
	adds	r1, r6, #0
	adds	r2, r3, #0
	adds	r1, #14
	adds	r2, #14
.L_081190be:
	ldrb	r3, [r2, #0]
	subs	r0, #1
	strb	r3, [r1, #0]
	subs	r2, #1
	subs	r1, #1
	cmp	r0, r4
	bge.n	.L_081190be
.L_081190cc:
	cmp	r4, #0
	ble.n	.L_081190e4
	adds	r2, r6, #0
	adds	r1, r5, #0
	adds	r0, r4, #0
.L_081190d6:
	ldrh	r3, [r1, #0]
	subs	r0, #1
	strb	r3, [r2, #0]
	adds	r1, #2
	adds	r2, #1
	cmp	r0, #0
	bne.n	.L_081190d6
.L_081190e4:
	movs	r3, #0
	strb	r3, [r6, #14]
	adds	r7, #1
.L_081190ea:
	cmp	r7, #2
	bgt.n	.L_08119104
	adds	r0, r7, #0
	adds	r0, #128
	bl	sub_08016ca4
	adds	r6, r0, #0
	bl	sub_08016854
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L_08119070
.L_08119104:
	mov	r0, r8
	bl	sub_08013164
	movs	r0, #170
	lsls	r0, r0, #1
	bl	sub_08014dac
	mov	r8, r0
	movs	r0, #1
	bl	sub_080ad000
	bl	sub_08016854
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_08119130
	bl	sub_080168cc
	movs	r0, #2
	bl	sub_08013560
.L_08119130:
	mov	r0, r8
	bl	sub_08013164
	mov	r0, sl
	add	sp, #32
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0c58
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r5, #170
	lsls	r5, r5, #1
	adds	r0, r5, #0
	sub	sp, #16
	bl	sub_08014dac
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	adds	r6, r0, #0
	mov	r9, r3
	movs	r2, #255
	movs	r5, #7
	adds	r3, #79
.L_08119172:
	subs	r5, #1
	strb	r2, [r3, #0]
	subs	r3, #1
	cmp	r5, #0
	bge.n	.L_08119172
	mov	r7, sp
	adds	r0, r7, #0
	bl	sub_0811a038
	movs	r5, #0
	mov	r8, r0
	cmp	r5, r8
	bge.n	.L_081191e6
	movs	r1, #149
	lsls	r1, r1, #1
	adds	r1, r1, r6
	mov	sl, r7
	mov	fp, r1
	movs	r7, #0
.L_08119198:
	mov	r2, sl
	ldrh	r0, [r7, r2]
	bl	sub_08016ca4
	movs	r2, #170
	adds	r1, r0, #0
	lsls	r2, r2, #1
	ldr	r3, [pc, #244]
	adds	r0, r6, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2302
	mov	r4, fp
	mov	r1, sl
	strb	r3, [r4, #0]
	ldrh	r3, [r7, r1]
	adds	r2, r5, #0
	adds	r3, #72
	subs	r2, #128
	mov	r4, r9
	movs	r1, #170
	lsls	r1, r1, #1
	strb	r2, [r4, r3]
	adds	r0, r6, #0
	bl	sub_0801680c
	movs	r1, #1
	negs	r1, r1
	cmp	r0, r1
	beq.n	.L_081191e6
	bl	sub_080168a0
	adds	r5, #1
	movs	r0, #2
	bl	sub_08013560
	adds	r7, #2
	cmp	r5, r8
	blt.n	.L_08119198
.L_081191e6:
	movs	r2, #149
	lsls	r2, r2, #1
	movs	r3, #0
	adds	r7, r6, r2
	mov	r8, r3
	b.n	.L_081191fe
.L_081191f2:
	bl	sub_080168a0
	movs	r0, #2
	bl	sub_08013560
	adds	r5, #1
.L_081191fe:
	cmp	r5, #2
	bgt.n	.L_08119218
	mov	r4, r8
	movs	r1, #170
	lsls	r1, r1, #1
	strb	r4, [r7, #0]
	adds	r0, r6, #0
	bl	sub_0801680c
	movs	r1, #1
	negs	r1, r1
	cmp	r0, r1
	bne.n	.L_081191f2
.L_08119218:
	movs	r5, #170
	adds	r0, r6, #0
	lsls	r5, r5, #1
	bl	sub_08013164
	adds	r0, r5, #0
	bl	sub_08014dac
	adds	r6, r0, #0
	movs	r0, #0
	bl	sub_080ad000
	ldr	r3, [pc, #104]
	adds	r1, r0, #0
	adds	r2, r5, #0
	adds	r0, r6, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c34
	movs	r3, #148
	lsls	r3, r3, #1
	adds	r2, r6, r3
	ldr	r3, [r2, #0]
	movs	r1, #0
	adds	r4, #8
	cmp	r1, r3
	bge.n	.L_08119266
	adds	r0, r2, #0
	adds	r2, r4, #0
.L_08119252:
	ldrb	r3, [r2, #2]
	mov	r4, r9
	adds	r3, #72
	ldrb	r3, [r4, r3]
	adds	r1, #1
	strb	r3, [r2, #2]
	adds	r2, #4
	ldr	r3, [r0, #0]
	cmp	r1, r3
	blt.n	.L_08119252
.L_08119266:
	movs	r1, #170
	lsls	r1, r1, #1
	adds	r0, r6, #0
	bl	sub_0801680c
	movs	r1, #1
	negs	r1, r1
	cmp	r0, r1
	beq.n	.L_08119288
	bl	sub_080168a0
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #2
	bl	sub_08013560
.L_08119288:
	adds	r0, r6, #0
	bl	sub_08013164
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x03000730
