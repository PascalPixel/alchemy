.syntax unified
	.thumb
	.set sub_08002dd8, 0x08002dd8
	.set sub_080048b0, 0x080048b0
	.set sub_080072ec, 0x080072ec
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_0800730c, 0x0800730c
	.global Overlay_08012388
Overlay_08012388:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	ldr	r6, [pc, #72]
	mov	r8, r0
	mov	sl, r1
	ldr	r5, [pc, #72]
	movs	r0, #49
	adds	r1, r5, #0
	bl	sub_080048b0
	movs	r2, #132
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	adds	r1, r0, #0
	ldr	r3, [pc, #56]
	ldr	r0, [pc, #60]
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #56]
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r6, r6, r1
	adds	r3, #196
	ldr	r4, [r3, #0]
	ldr	r2, [pc, #48]
	mov	r0, r8
	mov	r1, sl
	adds	r3, r6, #0
	bl	sub_080072f4
	movs	r0, #49
	bl	sub_08002dd8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x0201c000
	.4byte 0x0000027c
	.4byte 0x040000d4
	.4byte 0x08009e7c
	.4byte 0x03001e50
	.2byte 0xc000
	.2byte 0x0203
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #40
	str	r0, [sp, #12]
	ldr	r3, [r1, #0]
	add	r0, sp, #28
	adds	r7, r2, #0
	movs	r2, #0
	str	r2, [r0, #4]
	str	r3, [r0, #0]
	ldr	r3, [r1, #8]
	add	r1, sp, #16
	str	r3, [r0, #8]
	ldr	r3, [pc, #232]
	bl	sub_080072f0
	mov	r2, sp
	adds	r2, #16
	str	r2, [sp, #0]
	ldr	r3, [pc, #224]
	ldr	r0, [r2, #8]
	ldr	r1, [sp, #12]
	movs	r0, r0
	mov	ip, pc
	bx	r3
	ldr	r2, [sp, #0]
	ldr	r3, [r2, #4]
	subs	r0, r3, r0
	str	r0, [sp, #8]
	ldr	r3, [pc, #208]
	mov	r9, r3
	ldr	r3, [r3, #0]
	movs	r2, #0
	negs	r3, r3
	str	r3, [sp, #4]
	mov	fp, r2
	b.n	.L_0801244c
.L_08012448:
	ldr	r3, [pc, #192]
	mov	r9, r3
.L_0801244c:
	mov	r2, r9
	ldr	r1, [r2, #16]
	mov	r3, fp
	ldr	r2, [pc, #188]
	subs	r1, r1, r3
	lsls	r1, r1, #16
	mov	sl, r2
	ldr	r0, [sp, #4]
	bl	sub_0800730c
	ldr	r3, [sp, #12]
	mov	r8, r0
	subs	r0, r0, r3
	cmp	r0, #0
	bne.n	.L_0801246c
	movs	r0, #1
.L_0801246c:
	ldr	r1, [sp, #8]
	bl	sub_0800730c
	adds	r5, r0, #0
	cmp	r5, #0
	bge.n	.L_080124d8
	movs	r1, #128
	ldr	r6, [pc, #140]
	negs	r0, r5
	lsls	r1, r1, #8
	mov	ip, pc
	bx	r6
	mov	r2, r9
	adds	r1, r0, #0
	ldr	r0, [r2, #0]
	bl	sub_0800730c
	mov	r1, r8
	str	r0, [r7, #0]
	adds	r0, r5, #0
	mov	ip, pc
	bx	r6
	ldr	r3, [sp, #0]
	ldr	r1, [r3, #8]
	ldr	r3, [r3, #4]
	subs	r1, r1, r5
	asrs	r1, r1, #4
	subs	r5, r0, r3
	asrs	r5, r5, #4
	adds	r0, r1, #0
	mov	ip, pc
	bx	r6
	adds	r3, r0, #0
	adds	r1, r5, #0
	adds	r0, r5, #0
	movs	r0, r0
	mov	ip, pc
	bx	r6
	adds	r3, r3, r0
	ldr	r2, [pc, #88]
	adds	r0, r3, #0
	bl	sub_080072ec
	lsls	r0, r0, #12
	cmp	r5, #0
	bge.n	.L_080124ca
	negs	r0, r0
.L_080124ca:
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, r0
	mov	ip, pc
	bx	r6
	str	r0, [r7, #4]
	b.n	.L_080124de
.L_080124d8:
	movs	r3, #0
	str	r3, [r7, #0]
	str	r3, [r7, #4]
.L_080124de:
	movs	r2, #1
	movs	r3, #0
	add	fp, r2
	str	r3, [r7, #8]
	str	r3, [r7, #12]
	mov	r3, fp
	adds	r7, #20
	cmp	r3, #159
	ble.n	.L_08012448
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03000250
	.4byte 0x03000118
