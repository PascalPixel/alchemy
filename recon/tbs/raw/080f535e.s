.syntax unified
	.thumb
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_080f4100, 0x080f4100
	.global Func_080f535e
	.thumb_func
Func_080f535e:
	movs	r3, #0
	movs	r6, #128
	mov	fp, r3
	lsls	r6, r6, #1
.L_080f5366:
	mov	r4, fp
	movs	r5, #128
	lsls	r3, r4, #12
	lsls	r5, r5, #9
	subs	r5, r5, r3
	ldr	r0, [sp, #116]
	adds	r2, r5, #0
	ldr	r1, [pc, #96]
	adds	r3, r6, #0
	bl	sub_080f4100
	adds	r2, r5, #0
	movs	r1, #160
	movs	r5, #1
	add	r0, sp, #256
	lsls	r1, r1, #19
	adds	r3, r6, #0
	add	fp, r5
	bl	sub_080f4100
	mov	r7, fp
	movs	r0, #1
	bl	sub_080030f8
	cmp	r7, #17
	bne.n	.L_080f5366
	movs	r0, #12
	bl	sub_08002dd8
	movs	r0, #45
	bl	sub_08002dd8
	movs	r0, #40
	bl	sub_08002dd8
	movs	r0, #39
	bl	sub_08002dd8
	movs	r0, #41
	bl	sub_08002dd8
	movs	r3, #192
	lsls	r3, r3, #2
	add	sp, r3
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	lsls	r4, r2, #3
	lsls	r0, r0, #16
	lsls	r0, r0, #4
	strh	r0, [r0, #32]
	.4byte 0x05000200
