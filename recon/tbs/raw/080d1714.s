.syntax unified
	.thumb
	.set sub_08002f40, 0x08002f40
	.set sub_080041d8, 0x080041d8
	.set sub_08004458, 0x08004458
	.set sub_080044d0, 0x080044d0
	.set sub_08005340, 0x08005340
	.set sub_080072f0, 0x080072f0
	.set sub_080b5098, 0x080b5098
	.set sub_080cd594, 0x080cd594
	.set sub_080ed408, 0x080ed408
	.global Unnamed_080d1714
	.global Func_080d1714
	.thumb_func
Unnamed_080d1714:
Func_080d1714:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #672]
	ldr	r1, [r5, #0]
	sub	sp, #356
	str	r1, [sp, #100]
	adds	r3, r5, #0
	subs	r3, #112
	ldr	r3, [r3, #0]
	str	r3, [sp, #96]
	subs	r3, r5, #4
	ldr	r3, [r3, #0]
	str	r3, [sp, #92]
	ldr	r4, [pc, #652]
	ldr	r2, [r5, #4]
	adds	r3, r3, r4
	str	r2, [sp, #76]
	str	r0, [r3, #0]
	movs	r0, #0
	mov	r8, r3
	bl	sub_080cd594
	movs	r6, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r6, [sp, #0]
	bl	sub_080ed408
	ldr	r7, [r5, #24]
	movs	r3, #3
	movs	r1, #7
	movs	r2, #7
	movs	r0, #47
	str	r7, [sp, #84]
	str	r3, [sp, #0]
	bl	sub_080ed408
	ldr	r5, [r5, #28]
	ldr	r0, [pc, #604]
	str	r5, [sp, #88]
	bl	sub_08002f40
	adds	r5, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #596]
	movs	r2, #128
	adds	r1, r5, #0
	lsls	r0, r0, #19
	bl	sub_080072f0
	adds	r5, #128
	ldr	r1, [sp, #92]
	adds	r0, r5, #0
	bl	sub_08005340
	ldr	r0, [pc, #576]
	bl	sub_08002f40
	ldr	r1, [sp, #76]
	bl	sub_08005340
	movs	r1, #239
	ldr	r0, [sp, #92]
	lsls	r1, r1, #7
	adds	r3, r0, r1
	str	r6, [r3, #0]
	ldr	r3, [pc, #560]
	movs	r1, #144
	adds	r2, r0, r3
	movs	r3, #50
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #552]
	bl	sub_080041d8
	mov	r1, r8
	ldr	r3, [r1, #0]
	movs	r4, #128
	movs	r5, #160
	ldr	r3, [r3, #20]
	movs	r0, #0
	lsls	r4, r4, #16
	lsls	r5, r5, #14
	movs	r7, #0
	str	r4, [sp, #68]
	str	r5, [sp, #72]
	str	r7, [sp, #60]
	str	r0, [sp, #64]
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_080d186c
	mov	r2, sp
	movs	r3, #162
	movs	r4, #146
	ldr	r5, [sp, #92]
	ldr	r0, [pc, #484]
	adds	r2, #164
	lsls	r3, r3, #1
	lsls	r4, r4, #1
	add	r4, sp
	add	r3, sp
	adds	r5, r5, r0
	str	r2, [sp, #40]
	mov	r8, r4
	mov	fp, r3
	add	r7, sp, #260
	mov	sl, r5
	movs	r4, #0
	add	r6, sp, #196
.L_080d17fc:
	mov	r1, sl
	mov	r5, r9
	ldr	r2, [r1, #0]
	lsls	r3, r5, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	str	r4, [sp, #8]
	bl	sub_080b5098
	ldr	r5, [r0, #0]
	ldr	r3, [r5, #8]
	str	r3, [r6, #0]
	ldr	r3, [r5, #16]
	str	r3, [r6, #4]
	ldr	r4, [sp, #8]
	ldrh	r3, [r5, #6]
	ldr	r2, [sp, #40]
	str	r3, [r4, r2]
	ldr	r1, [r5, #16]
	ldr	r0, [r5, #8]
	bl	sub_080044d0
	ldr	r4, [sp, #8]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	mov	r3, fp
	str	r0, [r4, r3]
	ldr	r3, [r5, #8]
	asrs	r3, r3, #8
	adds	r0, r3, #0
	muls	r0, r3
	ldr	r3, [r5, #16]
	asrs	r3, r3, #8
	adds	r1, r3, #0
	muls	r1, r3
	adds	r3, r1, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #408]
	bl	sub_080072f0
	ldr	r4, [sp, #8]
	movs	r3, #0
	asrs	r0, r0, #7
	mov	r2, r8
	str	r0, [r4, r2]
	str	r3, [r4, r7]
	str	r3, [r5, #72]
	movs	r3, #1
	mov	r5, sl
	add	r9, r3
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #20]
	adds	r4, #4
	adds	r6, #8
	cmp	r9, r3
	bne.n	.L_080d17fc
.L_080d186c:
	ldr	r0, [sp, #92]
	movs	r1, #225
	movs	r7, #0
	lsls	r1, r1, #7
	mov	r9, r7
	movs	r6, #0
	adds	r5, r0, r1
.L_080d187a:
	movs	r3, #120
	str	r3, [r5, #8]
	str	r6, [r5, #4]
	bl	sub_08004458
	str	r6, [r5, #16]
	str	r6, [r5, #12]
	bl	sub_08004458
	movs	r3, #63
	movs	r2, #1
	ands	r3, r0
	add	r9, r2
	str	r3, [r5, #24]
	mov	r3, r9
	adds	r5, #28
	cmp	r3, #64
	bne.n	.L_080d187a
	ldr	r5, [sp, #96]
	movs	r4, #0
	adds	r5, #12
	str	r4, [sp, #80]
	str	r5, [sp, #36]
