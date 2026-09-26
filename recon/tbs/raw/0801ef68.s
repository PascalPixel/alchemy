.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080045e8, 0x080045e8
	.set sub_08016178, 0x08016178
	.set sub_08016498, 0x08016498
	.set sub_080170f8, 0x080170f8
	.set sub_08018efc, 0x08018efc
	.set sub_08019000, 0x08019000
	.set sub_0801e71c, 0x0801e71c
	.set sub_0801e8b0, 0x0801e8b0
	.set sub_0801ea3c, 0x0801ea3c
	.set sub_0801eea0, 0x0801eea0
	.set sub_08077008, 0x08077008
	.set sub_08077148, 0x08077148
	.set sub_080b50c8, 0x080b50c8
	.set sub_080b5130, 0x080b5130
	.global UiWindow_DrawColumnBorders
	.global Func_0801ef68
	.thumb_func
UiWindow_DrawColumnBorders:
Func_0801ef68:
.L_0801ef68:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #124]
	ldr	r3, [r3, #0]
	adds	r6, r0, #0
	sub	sp, #4
	mov	r9, r3
	movs	r3, #0
	str	r3, [sp, #0]
	ldrh	r3, [r6, #8]
	subs	r3, #1
	movs	r2, #1
	mov	fp, r3
	adds	r3, r1, #0
	ands	r3, r2
	ldrh	r7, [r6, #10]
	cmp	r3, #0
	bne.n	.L_0801ef9c
	movs	r3, #3
	negs	r3, r3
	ands	r1, r3
.L_0801ef9c:
	movs	r3, #2
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0801efaa
	movs	r2, #5
	str	r2, [sp, #0]
	movs	r2, #0
.L_0801efaa:
	ldr	r1, [pc, #76]
	adds	r5, r2, #0
	b.n	.L_0801f012
.L_0801efb0:
	ldrsb	r3, [r2, r5]
	ldr	r2, [sp, #0]
	adds	r0, r3, r2
	cmp	r0, fp
	bcs.n	.L_0801f010
	movs	r4, #0
	cmp	r7, #0
	beq.n	.L_0801f010
	ldr	r3, [pc, #56]
	subs	r2, r7, #1
	mov	ip, r2
	ldr	r2, [pc, #56]
	mov	sl, r3
	adds	r3, #1
	mov	r8, r3
	mov	lr, r2
.L_0801efd0:
	ldrh	r2, [r6, #14]
	ldrh	r3, [r6, #12]
	adds	r2, r2, r4
	adds	r3, r3, r0
	lsls	r2, r2, #5
	adds	r2, r2, r3
	lsls	r2, r2, #1
	mov	r3, r9
	adds	r1, r2, r3
	cmp	r4, #0
	bne.n	.L_0801efea
	mov	r2, sl
	b.n	.L_0801f006
.L_0801efea:
	cmp	r4, ip
	bne.n	.L_0801f004
	mov	r3, r8
	strh	r3, [r1, #0]
	b.n	.L_0801f008
	.4byte 0x03001e8c
	.4byte 0x080371c4
	.4byte 0x0000f018
	.2byte 0xf00f
	.2byte 0x0000
.L_0801f004:
	mov	r2, lr
.L_0801f006:
	strh	r2, [r1, #0]
.L_0801f008:
	adds	r4, #1
	cmp	r4, r7
	bne.n	.L_0801efd0
	ldr	r1, [pc, #92]
.L_0801f010:
	adds	r5, #1
.L_0801f012:
	adds	r2, r1, #0
	ldrsb	r3, [r2, r5]
	cmp	r3, #0
	bge.n	.L_0801efb0
	ldr	r3, [pc, #84]
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0801f054
	ldrh	r3, [r6, #10]
	ldrh	r2, [r6, #14]
	adds	r2, r2, r3
	ldrh	r3, [r6, #12]
	lsls	r2, r2, #6
	lsls	r3, r3, #1
	add	r2, r9
	adds	r2, r2, r3
	adds	r1, r2, #0
	ldr	r3, [pc, #40]
	subs	r1, #64
	movs	r0, #1
	strh	r3, [r1, #0]
	adds	r1, #2
	cmp	r0, fp
	bcs.n	.L_0801f050
	ldr	r3, [pc, #28]
.L_0801f046:
	adds	r0, #1
	strh	r3, [r1, #0]
	adds	r1, #2
	cmp	r0, fp
	bcc.n	.L_0801f046
.L_0801f050:
	ldr	r3, [pc, #20]
	strh	r3, [r1, #0]
.L_0801f054:
	ldr	r2, [pc, #28]
	movs	r3, #1
	add	r2, r9
	strb	r3, [r2, #0]
	add	sp, #4
	b.n	.L_0801f078
	.4byte 0x0000f080
	.4byte 0x0000f081
	.4byte 0x0000f082
	.4byte 0x080371c4
	.4byte 0x00000ea5
	.2byte 0x0ea3
	.2byte 0x0000
.L_0801f078:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
