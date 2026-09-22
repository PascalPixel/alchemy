.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_080148e8, 0x080148e8
	.set sub_080d8d40, 0x080d8d40
	.global Overlay_080da22c
Overlay_080da22c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	str	r1, [sp, #12]
	str	r2, [sp, #8]
	mov	fp, r3
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #160
	ldr	r3, [r3, #0]
	ldr	r1, [sp, #48]
	mov	sl, r3
	movs	r3, #0
	str	r3, [sp, #4]
	ldr	r3, [sp, #12]
	ldr	r6, [sp, #52]
	subs	r1, r1, r3
	ldr	r3, [sp, #8]
	ldr	r2, [sp, #56]
	subs	r6, r6, r3
	mov	r3, fp
	mov	r9, r0
	subs	r0, r2, r3
	ldr	r2, [pc, #196]
	asrs	r1, r1, #8
	mov	r8, r2
	asrs	r7, r0, #8
	adds	r0, r1, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1236
	adds	r1, r6, #0
	adds	r5, r0, #0
	adds	r0, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1c39
	adds	r6, r0, #0
	adds	r0, r7, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x19ad
	adds	r5, r5, r0
	ldr	r3, [pc, #160]
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2106
	bl	sub_08002054
	mov	r2, r9
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	add	sl, r3
	movs	r6, #128
	movs	r3, #36
	movs	r2, #0
	adds	r0, #1
	add	sl, r3
	movs	r7, #0
	lsls	r6, r6, #11
	mov	r9, r2
	str	r0, [sp, #0]
	b.n	.L_080da300
.L_080da2b8:
	adds	r0, r7, #0
	bl	sub_08002090
	adds	r1, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x9b03
	adds	r0, r3, r0
	str	r0, [r5, #4]
	ldr	r2, [sp, #8]
	adds	r0, r7, #0
	str	r2, [r5, #8]
	bl	sub_08002096
	adds	r1, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x4458
	movs	r3, #2
	mov	r2, sl
	str	r0, [r5, #12]
	movs	r0, #192
	strb	r3, [r5, #16]
	adds	r1, r6, #0
	str	r5, [r2, #0]
	lsls	r0, r0, #11
	bl	sub_080148e8
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	adds	r7, r7, r0
	movs	r3, #1
	lsls	r0, r0, #2
	mov	sl, r5
	adds	r6, r6, r0
	add	r9, r3
.L_080da300:
	ldr	r2, [sp, #0]
	cmp	r9, r2
	bgt.n	.L_080da316
	bl	sub_080d8d40
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_080da2b8
	movs	r3, #1
	negs	r3, r3
	str	r3, [sp, #4]
.L_080da316:
	movs	r3, #0
	mov	r2, sl
	str	r3, [r2, #0]
	ldr	r0, [sp, #4]
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0300021c
	.4byte 0x030002d4
