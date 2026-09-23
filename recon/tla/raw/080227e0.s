.syntax unified
	.thumb
	.set sub_08013300, 0x08013300
	.set sub_0801587c, 0x0801587c
	.set sub_08021918, 0x08021918
	.set sub_08022c98, 0x08022c98
	.global Func_080227e0
	.thumb_func
Func_080227e0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r6, r0, #0
	sub	sp, #4
	adds	r7, r1, #0
	adds	r4, r2, #0
	mov	r8, r3
	cmp	r6, #7
	bls.n	.L_080227fa
.L_080227f6:
	movs	r0, #0
	b.n	.L_080228a8
.L_080227fa:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #24]
	adds	r0, r4, #0
	lsls	r3, r6, #3
	adds	r5, r5, r3
	str	r4, [sp, #0]
	bl	sub_08021918
	ldr	r4, [sp, #0]
	ldr	r2, [pc, #164]
	lsls	r3, r6, #12
	adds	r5, #28
	orrs	r3, r4
	str	r3, [r5, #0]
	movs	r1, #2
	ldrsh	r3, [r2, r1]
	mov	sl, r0
	lsls	r3, r3, #16
	lsrs	r3, r3, #16
	str	r7, [r5, #4]
	ldrh	r0, [r2, #0]
	adds	r1, r2, #6
	movs	r5, #0
.L_0802282a:
	adds	r2, #4
	cmp	r3, #0
	beq.n	.L_080227f6
	cmp	r3, r4
	beq.n	.L_08022848
	adds	r5, #1
	cmp	r5, #255
	bhi.n	.L_08022848
	movs	r6, #0
	ldrsh	r3, [r1, r6]
	ldrh	r0, [r2, #0]
	lsls	r3, r3, #16
	lsrs	r3, r3, #16
	adds	r1, #4
	b.n	.L_0802282a
.L_08022848:
	bl	sub_08013300
	adds	r1, r7, #0
	bl	sub_0801587c
	ldr	r3, [r7, #0]
	adds	r4, r7, #0
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_08022870
	adds	r2, r3, #0
.L_0802285e:
	adds	r3, r2, r7
	adds	r5, #1
	stmia	r4!, {r3}
	cmp	r5, #255
	bhi.n	.L_08022870
	ldr	r3, [r4, #0]
	adds	r2, r3, #0
	cmp	r3, #0
	bne.n	.L_0802285e
.L_08022870:
	mov	r1, r8
	cmp	r1, #0
	beq.n	.L_0802289e
	mov	r2, r8
	subs	r2, #1
	adds	r5, r4, #4
	adds	r0, r7, r0
	cmp	r2, #4
	bls.n	.L_08022884
	movs	r2, #0
.L_08022884:
	ldr	r3, [pc, #48]
	lsls	r2, r2, #8
	adds	r2, r2, r3
	cmp	r5, r0
	bcs.n	.L_0802289e
.L_0802288e:
	ldrb	r4, [r5, #0]
	cmp	r4, #223
	bhi.n	.L_08022898
	ldrb	r4, [r2, r4]
	strb	r4, [r5, #0]
.L_08022898:
	adds	r5, #1
	cmp	r5, r0
	bcc.n	.L_0802288e
.L_0802289e:
	mov	r2, sl
	ldrb	r3, [r2, #0]
	ldrb	r2, [r2, #1]
	adds	r0, r2, #0
	muls	r0, r3
.L_080228a8:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0802e91c
	.2byte 0x03a8
	.2byte 0x0802
.L_080228bc:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #24]
	movs	r1, #0
	adds	r2, r3, #0
	adds	r2, #28
.L_080228ca:
	ldr	r3, [r2, #0]
	cmp	r3, r0
	bne.n	.L_080228d4
	ldr	r0, [r2, #4]
	b.n	.L_080228de
.L_080228d4:
	adds	r1, #1
	adds	r2, #8
	cmp	r1, #7
	bls.n	.L_080228ca
	movs	r0, #0
.L_080228de:
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	ldrb	r3, [r7, #27]
	movs	r1, #0
	sub	sp, #8
	cmp	r1, r3
	bge.n	.L_08022962
	adds	r2, r7, #0
	adds	r2, #40
	str	r2, [sp, #4]
	mov	r8, r1
.L_080228fa:
	ldr	r2, [sp, #4]
	str	r1, [sp, #0]
	ldmia	r2!, {r6}
	adds	r3, r2, #0
	str	r3, [sp, #4]
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	bl	sub_08021918
	adds	r5, r0, #0
	ldrb	r2, [r5, #0]
	ldr	r1, [sp, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_0802295a
	cmp	r1, #0
	bne.n	.L_08022930
	ldrb	r3, [r5, #1]
	strb	r2, [r7, #20]
	strb	r3, [r7, #21]
	ldrh	r3, [r5, #2]
	lsls	r3, r3, #8
	str	r3, [r7, #12]
	ldrb	r3, [r5, #7]
	strb	r3, [r7, #23]
	ldrb	r3, [r5, #6]
	strb	r3, [r7, #22]
.L_08022930:
	ldr	r0, [r5, #12]
	cmp	r0, #0
	bne.n	.L_08022942
	movs	r2, #0
	ldrsh	r0, [r6, r2]
	str	r1, [sp, #0]
	bl	.L_080228bc
	ldr	r1, [sp, #0]
.L_08022942:
	ldrb	r3, [r5, #4]
	strb	r3, [r6, #4]
	ldr	r3, [r5, #16]
	str	r0, [r6, #8]
	str	r3, [r6, #12]
	ldrb	r3, [r5, #10]
	strb	r3, [r6, #7]
	movs	r3, #255
	strb	r3, [r6, #22]
	mov	r3, r8
	str	r3, [r6, #16]
	strb	r3, [r6, #20]
.L_0802295a:
	ldrb	r3, [r7, #27]
	adds	r1, #1
	cmp	r1, r3
	blt.n	.L_080228fa
.L_08022962:
	movs	r0, #0
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, lr}
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080229aa
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	bl	sub_08021918
	adds	r6, r0, #0
	ldrb	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L_080229aa
	ldr	r0, [r6, #12]
	cmp	r0, #0
	bne.n	.L_08022992
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	bl	.L_080228bc
.L_08022992:
	ldrb	r3, [r6, #4]
	movs	r2, #0
	strb	r3, [r5, #4]
	str	r2, [r5, #16]
	ldr	r3, [r6, #16]
	str	r0, [r5, #8]
	str	r3, [r5, #12]
	strb	r2, [r5, #20]
	ldrb	r3, [r6, #10]
	strb	r3, [r5, #7]
	movs	r3, #255
	strb	r3, [r5, #22]
.L_080229aa:
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	ldr	r5, [r6, #40]
	mov	r8, r1
	movs	r7, #0
	cmp	r5, #0
	beq.n	.L_080229d0
	adds	r3, r6, #0
	adds	r3, #40
.L_080229c2:
	adds	r7, #1
	cmp	r7, #3
	bgt.n	.L_080229d0
	adds	r3, #4
	ldr	r5, [r3, #0]
	cmp	r5, #0
	bne.n	.L_080229c2
.L_080229d0:
	cmp	r7, #4
	bne.n	.L_080229da
	movs	r0, #1
	negs	r0, r0
	b.n	.L_08022a1c
.L_080229da:
	mov	r0, r8
	bl	sub_08022c98
	adds	r5, r0, #0
	movs	r0, #0
	cmp	r5, #0
	beq.n	.L_08022a1c
	lsls	r3, r7, #2
	adds	r3, #40
	mov	r0, r8
	str	r5, [r6, r3]
	bl	sub_08021918
	ldrb	r3, [r6, #27]
	cmp	r3, #0
	bne.n	.L_08022a10
	ldrb	r3, [r0, #0]
	strb	r3, [r6, #20]
	ldrb	r3, [r0, #1]
	strb	r3, [r6, #21]
	ldrh	r3, [r0, #2]
	lsls	r3, r3, #8
	str	r3, [r6, #12]
	ldrb	r3, [r0, #7]
	strb	r3, [r6, #23]
	ldrb	r3, [r0, #6]
	strb	r3, [r6, #22]
.L_08022a10:
	ldrb	r3, [r6, #27]
	cmp	r7, r3
	bne.n	.L_08022a1a
	adds	r3, r7, #1
	strb	r3, [r6, #27]
.L_08022a1a:
	adds	r0, r5, #0
.L_08022a1c:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
