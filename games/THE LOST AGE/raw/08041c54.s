.syntax unified
	.thumb
	.set sub_08041abc, 0x08041abc
	.global Overlay_08041c54
Overlay_08041c54:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	sl, r3
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	adds	r7, r1, #0
	adds	r5, r0, #0
	mov	r8, r2
	ldr	r6, [sp, #28]
	mov	r9, r3
	cmp	r7, sl
	beq.n	.L_08041c76
	b.n	.L_08041dd4
.L_08041c76:
	cmp	r8, r6
	bne.n	.L_08041c7c
	b.n	.L_08041f4a
.L_08041c7c:
	cmp	r8, r6
	bls.n	.L_08041c86
	mov	r4, r8
	mov	r8, r6
	adds	r6, r4, #0
.L_08041c86:
	movs	r1, #12
	ldrsh	r0, [r5, r1]
	movs	r2, #14
	ldrsh	r1, [r5, r2]
	mov	r2, r8
	add	r1, r8
	subs	r3, r6, r2
	add	r0, sl
	movs	r2, #1
	bl	sub_08041abc
	movs	r1, #14
	ldrsh	r3, [r5, r1]
	movs	r1, #12
	ldrsh	r2, [r5, r1]
	add	r3, r8
	lsls	r3, r3, #6
	lsls	r2, r2, #1
	add	r3, r9
	mov	r1, sl
	adds	r3, r3, r2
	lsls	r2, r1, #1
	adds	r3, r3, r2
	adds	r0, r3, #0
	mov	r4, r8
	adds	r0, #8
	cmp	r4, r6
	bls.n	.L_08041cc0
	b.n	.L_08041f4a
.L_08041cc0:
	ldrh	r1, [r0, #0]
	cmp	r4, r8
	bne.n	.L_08041d28
	ldr	r2, [pc, #652]
	adds	r3, r1, r2
	cmp	r3, #15
	bhi.n	.L_08041dc0
	ldr	r2, [pc, #648]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x08041dc6
	.4byte 0x08041db8
	.4byte 0x08041dc6
	.4byte 0x08041dc6
	.4byte 0x08041dc6
	.4byte 0x08041d20
	.4byte 0x08041dc0
	.4byte 0x08041dc0
	.4byte 0x08041d18
	.4byte 0x08041dc0
	.4byte 0x08041dc0
	.4byte 0x08041dc0
	.4byte 0x08041dc0
	.4byte 0x08041dc0
	.4byte 0x08041dc0
	.2byte 0x1dc6
	.2byte 0x0804
	movs	r1, #240
	lsls	r1, r1, #8
	adds	r1, #24
	b.n	.L_08041dc6
	movs	r1, #240
	lsls	r1, r1, #8
	adds	r1, #9
	b.n	.L_08041dc6
.L_08041d28:
	cmp	r4, r6
	bne.n	.L_08041d90
	ldr	r2, [pc, #548]
	adds	r3, r1, r2
	cmp	r3, #16
	bhi.n	.L_08041dc0
	ldr	r2, [pc, #548]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x08041db8
	.4byte 0x08041dc6
	.4byte 0x08041dc6
	.4byte 0x08041dc6
	.4byte 0x08041dc6
	.4byte 0x08041d88
	.4byte 0x08041dc0
	.4byte 0x08041dc0
	.4byte 0x08041dc0
	.4byte 0x08041dc0
	.4byte 0x08041dc0
	.4byte 0x08041d80
	.4byte 0x08041dc0
	.4byte 0x08041dc0
	.4byte 0x08041dc0
	.4byte 0x08041dc0
	.2byte 0x1dc6
	.2byte 0x0804
	movs	r1, #240
	lsls	r1, r1, #8
	adds	r1, #25
	b.n	.L_08041dc6
	movs	r1, #240
	lsls	r1, r1, #8
	adds	r1, #10
	b.n	.L_08041dc6
.L_08041d90:
	ldr	r2, [pc, #448]
	adds	r3, r1, r2
	cmp	r3, #5
	bhi.n	.L_08041dc0
	ldr	r2, [pc, #452]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x08041db8
	.4byte 0x08041db8
	.4byte 0x08041dc6
	.4byte 0x08041dc6
	.4byte 0x08041dc6
	.2byte 0x1db8
	.2byte 0x0804
	movs	r1, #240
	lsls	r1, r1, #8
	adds	r1, #13
	b.n	.L_08041dc6
.L_08041dc0:
	movs	r1, #240
	lsls	r1, r1, #8
	adds	r1, #15
.L_08041dc6:
	adds	r4, #1
	strh	r1, [r0, #0]
	adds	r0, #64
	cmp	r4, r6
	bhi.n	.L_08041dd2
	b.n	.L_08041cc0
.L_08041dd2:
	b.n	.L_08041f4a
.L_08041dd4:
	cmp	r8, r6
	beq.n	.L_08041dda
	b.n	.L_08041f4a
.L_08041dda:
	cmp	r7, sl
	bne.n	.L_08041de0
	b.n	.L_08041f4a
.L_08041de0:
	cmp	r7, sl
	bls.n	.L_08041dea
	adds	r4, r7, #0
	mov	r7, sl
	mov	sl, r4
.L_08041dea:
	movs	r3, #12
	ldrsh	r0, [r5, r3]
	movs	r2, #14
	ldrsh	r1, [r5, r2]
	mov	r3, sl
	add	r1, r8
	subs	r2, r3, r7
	adds	r0, r0, r7
	movs	r3, #1
	bl	sub_08041abc
	movs	r1, #14
	ldrsh	r3, [r5, r1]
	movs	r1, #12
	ldrsh	r2, [r5, r1]
	add	r3, r8
	lsls	r3, r3, #6
	lsls	r2, r2, #1
	add	r3, r9
	adds	r3, r3, r2
	lsls	r2, r7, #1
	adds	r3, r3, r2
	adds	r0, r3, #0
	adds	r4, r7, #0
	adds	r0, #8
	cmp	r4, sl
	bls.n	.L_08041e22
	b.n	.L_08041f4a
.L_08041e22:
	ldrh	r1, [r0, #0]
	cmp	r4, r7
	bne.n	.L_08041e94
	ldr	r2, [pc, #296]
	adds	r3, r1, r2
	cmp	r3, #17
	bls.n	.L_08041e32
	b.n	.L_08041f38
.L_08041e32:
	ldr	r2, [pc, #304]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x08041f3e
	.4byte 0x08041f3e
	.4byte 0x08041f3e
	.4byte 0x08041f30
	.4byte 0x08041f3e
	.4byte 0x08041f38
	.4byte 0x08041e8c
	.4byte 0x08041f38
	.4byte 0x08041f38
	.4byte 0x08041f38
	.4byte 0x08041f38
	.4byte 0x08041f38
	.4byte 0x08041f38
	.4byte 0x08041e84
	.4byte 0x08041f38
	.4byte 0x08041f38
	.4byte 0x08041f38
	.2byte 0x1f3e
	.2byte 0x0804
	movs	r1, #240
	lsls	r1, r1, #8
	adds	r1, #26
	b.n	.L_08041f3e
	movs	r1, #240
	lsls	r1, r1, #8
	adds	r1, #11
	b.n	.L_08041f3e
.L_08041e94:
	cmp	r4, sl
	bne.n	.L_08041f04
	ldr	r2, [pc, #184]
	adds	r3, r1, r2
	cmp	r3, #18
	bhi.n	.L_08041f38
	ldr	r2, [pc, #196]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x08041f3e
	.4byte 0x08041f3e
	.4byte 0x08041f30
	.4byte 0x08041f3e
	.4byte 0x08041f3e
	.4byte 0x08041f38
	.4byte 0x08041efc
	.4byte 0x08041f38
	.4byte 0x08041f38
	.4byte 0x08041f38
	.4byte 0x08041f38
	.4byte 0x08041f38
	.4byte 0x08041f38
	.4byte 0x08041f38
	.4byte 0x08041ef4
	.4byte 0x08041f38
	.4byte 0x08041f38
	.4byte 0x08041f38
	.2byte 0x1f3e
	.2byte 0x0804
	movs	r1, #240
	lsls	r1, r1, #8
	adds	r1, #27
	b.n	.L_08041f3e
	movs	r1, #240
	lsls	r1, r1, #8
	adds	r1, #12
	b.n	.L_08041f3e
.L_08041f04:
	ldr	r2, [pc, #76]
	adds	r3, r1, r2
	cmp	r3, #6
	bhi.n	.L_08041f38
	ldr	r2, [pc, #92]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x08041f3e
	.4byte 0x08041f3e
	.4byte 0x08041f30
	.4byte 0x08041f30
	.4byte 0x08041f3e
	.4byte 0x08041f38
	.2byte 0x1f30
	.2byte 0x0804
	movs	r1, #240
	lsls	r1, r1, #8
	adds	r1, #13
	b.n	.L_08041f3e
.L_08041f38:
	movs	r1, #240
	lsls	r1, r1, #8
	adds	r1, #14
.L_08041f3e:
	adds	r4, #1
	strh	r1, [r0, #0]
	adds	r0, #2
	cmp	r4, sl
	bhi.n	.L_08041f4a
	b.n	.L_08041e22
.L_08041f4a:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0xffff0ff7
	.4byte 0x08041cd8
	.4byte 0x08041d3c
	.4byte 0x08041da0
	.4byte 0x08041e3c
	.4byte 0x08041ea8
	.2byte 0x1f14
	.2byte 0x0804
	ldr	r2, [pc, #20]
	movs	r3, #192
	lsls	r3, r3, #18
	ands	r0, r2
	ldr	r3, [r3, #60]
	movs	r2, #240
	lsls	r2, r2, #4
	adds	r2, #62
	adds	r3, r3, r2
	strh	r0, [r3, #0]
	b.n	.L_08041f8c
	movs	r0, r0
	.2byte 0x000f
	.2byte 0x0000
.L_08041f8c:
	bx	lr
