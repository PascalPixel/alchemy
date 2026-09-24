.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_08015120, 0x08015120
	.set sub_08015140, 0x08015140
	.set sub_08077008, 0x08077008
	.set sub_08077018, 0x08077018
	.set sub_08077068, 0x08077068
	.set sub_08077228, 0x08077228
	.set sub_08077230, 0x08077230
	.set sub_080b0574, 0x080b0574
	.set sub_080b0634, 0x080b0634
	.set sub_080b10cc, 0x080b10cc
	.set sub_080b1868, 0x080b1868
	.set sub_080b196c, 0x080b196c
	.set sub_080b1dec, 0x080b1dec
	.set sub_080b20e8, 0x080b20e8
	.set sub_080f9010, 0x080f9010
	.global Shop_RepairItem
	.global Func_080b2328
	.thumb_func
Shop_RepairItem:
Func_080b2328:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	str	r1, [sp, #12]
	ldr	r3, [pc, #392]
	ldr	r3, [r3, #0]
	adds	r7, r0, #0
	str	r3, [sp, #8]
	bl	sub_08077008
	ldr	r2, [sp, #12]
	lsls	r2, r2, #1
	str	r2, [sp, #4]
	adds	r6, r2, #0
	adds	r6, #216
	ldrh	r3, [r0, r6]
	ldr	r2, [pc, #372]
	ands	r2, r3
	mov	sl, r2
	mov	r8, r0
	mov	r0, sl
	bl	sub_08077018
	adds	r5, r0, #0
	ldrb	r1, [r5, #2]
	adds	r0, r7, #0
	bl	sub_08077228
	str	r0, [sp, #0]
	mov	r3, r8
	ldrh	r0, [r3, r6]
	bl	sub_080b20e8
	ldrb	r1, [r5, #12]
	mov	r9, r0
	cmp	r1, #2
	beq.n	.L_080b238c
	mov	r0, sl
	movs	r1, #2
	bl	sub_08015120
	ldr	r0, [pc, #324]
	bl	sub_080b0574
	b.n	.L_080b24b0
.L_080b238c:
	mov	r3, r8
	ldrh	r2, [r3, r6]
	movs	r3, #128
	lsls	r3, r3, #3
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080b23aa
	mov	r0, sl
	movs	r1, #2
	bl	sub_08015120
	ldr	r0, [pc, #300]
	bl	sub_080b0574
	b.n	.L_080b24b0
.L_080b23aa:
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b23cc
	ldrb	r3, [r5, #3]
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080b23cc
	mov	r0, sl
	movs	r1, #2
	bl	sub_08015120
	ldr	r0, [pc, #268]
	bl	sub_080b0574
	b.n	.L_080b24b0
.L_080b23cc:
	ldr	r3, [pc, #264]
	ldr	r3, [r3, #16]
	cmp	r9, r3
	bls.n	.L_080b23dc
	ldr	r0, [pc, #260]
	bl	sub_080b0574
	b.n	.L_080b24b0
.L_080b23dc:
	mov	r0, sl
	movs	r1, #2
	bl	sub_08015120
	mov	r0, r9
	movs	r1, #5
	bl	sub_08015120
	ldr	r2, [pc, #240]
	mov	fp, r2
	mov	r0, fp
	bl	sub_080b0574
	movs	r0, #0
	bl	sub_080b0634
	cmp	r0, #0
	beq.n	.L_080b240a
	mov	r0, fp
	adds	r0, #1
	bl	sub_080b0574
	b.n	.L_080b24b0
.L_080b240a:
	ldr	r5, [sp, #4]
	mov	r2, r8
	adds	r5, #216
	mov	r3, r8
	ldrh	r6, [r3, r5]
	strh	r0, [r2, r5]
	ldr	r3, [sp, #8]
	adds	r1, r7, #0
	ldr	r0, [r3, #32]
	bl	sub_080b1dec
	movs	r1, #2
	mov	r0, sl
	bl	sub_08015120
	mov	r0, fp
	adds	r0, #2
	bl	sub_080b0574
	bl	sub_08015140
	movs	r0, #10
	bl	sub_080030f8
	movs	r0, #100
	bl	sub_080f9010
	movs	r0, #110
	bl	sub_080030f8
	movs	r0, #100
	bl	sub_080f9010
	movs	r0, #110
	bl	sub_080030f8
	movs	r0, #100
	bl	sub_080f9010
	movs	r0, #110
	bl	sub_080030f8
	movs	r0, #112
	bl	sub_080f9010
	movs	r0, #20
	bl	sub_080030f8
	mov	r2, r8
	strh	r6, [r2, r5]
	ldr	r1, [sp, #12]
	adds	r0, r7, #0
	bl	sub_08077068
	mov	r3, r9
	negs	r0, r3
	bl	sub_08077230
	bl	sub_080b10cc
	ldr	r2, [sp, #8]
	adds	r1, r7, #0
	ldr	r0, [r2, #32]
	bl	sub_080b1dec
	movs	r1, #2
	mov	r0, sl
	bl	sub_08015120
	mov	r0, fp
	adds	r0, #3
	bl	sub_080b0574
	adds	r0, r7, #0
	ldr	r1, [sp, #12]
	bl	sub_080b1868
	cmp	r0, #0
	beq.n	.L_080b24b0
	adds	r0, r7, #0
	ldr	r1, [sp, #0]
	bl	sub_080b196c
.L_080b24b0:
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x000001ff
	.4byte 0x00000cba
	.4byte 0x00000cbb
	.4byte 0x00000cbc
	.4byte 0x02000240
	.4byte 0x00000cbd
	.4byte 0x00000cbe
