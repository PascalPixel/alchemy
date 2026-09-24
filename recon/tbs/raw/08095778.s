.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022fc, 0x080022fc
	.set sub_080030f8, 0x080030f8
	.set sub_080770c8, 0x080770c8
	.set sub_0808d394, 0x0808d394
	.set sub_080955b0, 0x080955b0
	.global Unnamed_08095778
	.global Func_08095778
	.thumb_func
Unnamed_08095778:
Func_08095778:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	mov	r8, r0
	ldr	r1, [pc, #232]
	movs	r0, #141
	lsls	r0, r0, #2
	adds	r7, r1, r0
	movs	r3, #0
	ldrsh	r5, [r7, r3]
	ldrh	r2, [r7, #0]
	movs	r3, #240
	ldr	r6, [pc, #220]
	lsls	r3, r3, #8
	mov	r0, r8
	ands	r5, r3
	ands	r6, r2
	cmp	r0, #0
	bne.n	.L_080957ec
	cmp	r5, #0
	bne.n	.L_080957cc
	ldr	r3, [pc, #208]
	ldr	r2, [pc, #208]
	ands	r6, r3
	adds	r3, r6, r2
	cmp	r3, #80
	bhi.n	.L_08095860
	ldr	r0, [pc, #204]
	adds	r3, r1, r0
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	cmp	r2, #0
	ble.n	.L_080957c0
	ldr	r3, [pc, #196]
	cmp	r2, r3
	bne.n	.L_08095860
.L_080957c0:
	adds	r0, r6, #0
	subs	r0, #172
	bl	sub_080770c8
	strh	r5, [r7, #0]
	b.n	.L_08095860
.L_080957cc:
	movs	r0, #128
	lsls	r0, r0, #5
	cmp	r5, r0
	bne.n	.L_08095860
	ldr	r2, [pc, #164]
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #1
	bne.n	.L_080957e6
	adds	r0, r6, #0
	bl	sub_080770c8
.L_080957e6:
	mov	r1, r8
	strh	r1, [r7, #0]
	b.n	.L_08095860
.L_080957ec:
	cmp	r5, #0
	bne.n	.L_08095856
	ldr	r2, [pc, #128]
	ldr	r0, [pc, #132]
	ands	r6, r2
	adds	r3, r6, r0
	cmp	r3, #80
	bhi.n	.L_08095856
	ands	r6, r2
	ldr	r2, [pc, #124]
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	ble.n	.L_08095856
	ldr	r1, [pc, #108]
	adds	r5, r6, r1
	adds	r0, r5, #0
	movs	r1, #20
	bl	sub_080022ec
	movs	r1, #20
	mov	r8, r0
	adds	r0, r5, #0
	bl	sub_080022fc
	movs	r5, #8
	adds	r7, r0, #0
	b.n	.L_08095828
.L_08095826:
	adds	r5, #1
.L_08095828:
	cmp	r5, #65
	bgt.n	.L_08095854
	adds	r0, r5, #0
	bl	sub_0808d394
	cmp	r0, #0
	beq.n	.L_08095826
	movs	r2, #2
	ldrsh	r3, [r0, r2]
	ldr	r0, [pc, #60]
	subs	r3, #48
	adds	r2, r6, r0
	cmp	r3, r2
	bne.n	.L_08095826
	movs	r0, #40
	bl	sub_080030f8
	mov	r1, r8
	adds	r0, r5, #0
	adds	r2, r7, #0
	bl	sub_080955b0
.L_08095854:
	ldr	r1, [pc, #20]
.L_08095856:
	movs	r3, #141
	lsls	r3, r3, #2
	adds	r2, r1, r3
	movs	r3, #0
	strh	r3, [r2, #0]
.L_08095860:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x00000fff
	.4byte 0x000007ff
	.4byte 0xfffffed4
	.4byte 0x00000236
	.4byte 0x000003e7
