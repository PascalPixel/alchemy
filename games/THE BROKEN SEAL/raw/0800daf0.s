.syntax unified
	.thumb
	.set sub_080045d4, 0x080045d4
	.set sub_080072f0, 0x080072f0
	.set sub_08007310, 0x08007310
	.global Overlay_0800daf0
Overlay_0800daf0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #448]
	ldr	r1, [r3, #0]
	adds	r3, r1, #0
	adds	r3, #236
	ldr	r3, [r3, #0]
	movs	r2, #240
	lsls	r2, r2, #15
	mov	r8, r0
	adds	r7, r3, r2
	ldr	r0, [r0, #104]
	adds	r3, r1, #0
	adds	r3, #240
	ldr	r2, [r0, #12]
	ldr	r3, [r3, #0]
	movs	r4, #192
	adds	r3, r3, r2
	lsls	r4, r4, #15
	adds	r6, r3, r4
	adds	r3, r1, #0
	adds	r3, #244
	ldr	r3, [r3, #0]
	ldr	r5, [pc, #408]
	adds	r4, r3, r5
	adds	r3, r1, #0
	adds	r3, #248
	ldr	r3, [r3, #0]
	adds	r3, r3, r2
	ldr	r2, [pc, #400]
	adds	r1, r3, r2
	mov	r2, r8
	adds	r2, #85
	movs	r3, #0
	sub	sp, #8
	strb	r3, [r2, #0]
	cmp	r0, #0
	bne.n	.L_0800db48
	b.n	.L_0800dca0
.L_0800db48:
	ldr	r3, [r0, #0]
	cmp	r3, #0
	bne.n	.L_0800db50
	b.n	.L_0800dca0
.L_0800db50:
	ldr	r3, [r0, #8]
	ldr	r5, [r0, #12]
	mov	fp, r3
	ldr	r0, [r0, #16]
	movs	r3, #128
	lsls	r3, r3, #24
	mov	r2, r8
	str	r0, [sp, #4]
	str	r3, [r2, #56]
	str	r3, [r2, #60]
	str	r3, [r2, #64]
	cmp	fp, r7
	bge.n	.L_0800db6c
	mov	fp, r7
.L_0800db6c:
	ldr	r3, [sp, #4]
	cmp	r3, r6
	bge.n	.L_0800db74
	str	r6, [sp, #4]
.L_0800db74:
	cmp	fp, r4
	ble.n	.L_0800db7a
	mov	fp, r4
.L_0800db7a:
	ldr	r4, [sp, #4]
	cmp	r4, r1
	ble.n	.L_0800db82
	str	r1, [sp, #4]
.L_0800db82:
	mov	r3, r8
	adds	r3, #100
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_0800db9c
	mov	r4, r8
	mov	r3, fp
	str	r3, [r4, #8]
	str	r5, [r4, #12]
	ldr	r5, [sp, #4]
	str	r5, [r4, #16]
	b.n	.L_0800dca0
.L_0800db9c:
	mov	r2, r8
	ldr	r3, [r2, #8]
	mov	r4, fp
	subs	r0, r4, r3
	cmp	r0, #0
	bge.n	.L_0800dbac
	ldr	r2, [pc, #288]
	adds	r0, r0, r2
.L_0800dbac:
	mov	r2, r8
	ldr	r3, [r2, #16]
	ldr	r4, [sp, #4]
	asrs	r0, r0, #16
	mov	sl, r0
	subs	r0, r4, r3
	cmp	r0, #0
	bge.n	.L_0800dbc0
	ldr	r2, [pc, #268]
	adds	r0, r0, r2
.L_0800dbc0:
	asrs	r6, r0, #16
	mov	r3, sl
	mov	r0, sl
	muls	r0, r3
	adds	r3, r6, #0
	muls	r3, r6
	adds	r0, r0, r3
	ldr	r3, [pc, #256]
	bl	sub_080072f0
	mov	r4, r8
	ldr	r3, [r4, #8]
	mov	r2, fp
	subs	r2, r2, r3
	ldr	r3, [r4, #12]
	subs	r5, r5, r3
	mov	r9, r5
	ldr	r3, [r4, #16]
	movs	r5, #128
	ldr	r4, [sp, #4]
	lsls	r7, r0, #16
	lsls	r5, r5, #15
	mov	sl, r2
	subs	r6, r4, r3
	cmp	r7, r5
	bge.n	.L_0800dc16
	ldr	r4, [pc, #220]
	mov	r0, sl
	mov	r1, sl
	movs	r0, r0
	mov	ip, pc
	bx	r4
	adds	r3, r0, #0
	adds	r1, r6, #0
	adds	r0, r6, #0
	movs	r0, r0
	mov	ip, pc
	bx	r4
	adds	r3, r3, r0
	adds	r0, r3, #0
	bl	sub_080045d4
	adds	r7, r0, #0
.L_0800dc16:
	adds	r1, r7, #0
	cmp	r7, #0
	bge.n	.L_0800dc1e
	adds	r1, r7, #7
.L_0800dc1e:
	mov	r2, r8
	ldr	r3, [r2, #48]
	asrs	r5, r1, #3
	cmp	r5, r3
	ble.n	.L_0800dc2a
	adds	r5, r3, #0
.L_0800dc2a:
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r7, r3
	bge.n	.L_0800dc3e
	mov	r5, r8
	mov	r4, fp
	str	r4, [r5, #8]
	ldr	r2, [sp, #4]
	str	r2, [r5, #16]
	b.n	.L_0800dc7c
.L_0800dc3e:
	cmp	r7, r5
	ble.n	.L_0800dc6e
	ldr	r3, [pc, #148]
	mov	r1, sl
	mov	fp, r3
	adds	r0, r7, #0
	bl	sub_08007310
	ldr	r3, [pc, #132]
	adds	r1, r5, #0
	movs	r0, r0
	mov	ip, pc
	bx	r3
	adds	r1, r6, #0
	str	r3, [sp, #0]
	mov	sl, r0
	adds	r0, r7, #0
	bl	sub_08007310
	adds	r1, r5, #0
	ldr	r3, [sp, #0]
	mov	ip, pc
	bx	r3
	adds	r6, r0, #0
.L_0800dc6e:
	mov	r4, r8
	ldr	r3, [r4, #8]
	add	r3, sl
	str	r3, [r4, #8]
	ldr	r3, [r4, #16]
	adds	r3, r3, r6
	str	r3, [r4, #16]
.L_0800dc7c:
	mov	r3, r9
	cmp	r3, #0
	bge.n	.L_0800dc84
	negs	r3, r3
.L_0800dc84:
	movs	r5, #128
	lsls	r5, r5, #8
	cmp	r3, r5
	ble.n	.L_0800dc98
	mov	r3, r9
	cmp	r3, #0
	bge.n	.L_0800dc94
	adds	r3, #3
.L_0800dc94:
	asrs	r3, r3, #2
	mov	r9, r3
.L_0800dc98:
	mov	r2, r8
	ldr	r3, [r2, #12]
	add	r3, r9
	str	r3, [r2, #12]
.L_0800dca0:
	mov	r4, r8
	ldrh	r3, [r4, #4]
	mov	r5, r8
	adds	r3, #1
	movs	r0, #1
	strh	r3, [r5, #4]
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e70
	.4byte 0xff880000
	.4byte 0xffc00000
	.4byte 0x0000ffff
	.4byte 0x030001d8
	.4byte 0x03000118
	.4byte 0x0300013c
