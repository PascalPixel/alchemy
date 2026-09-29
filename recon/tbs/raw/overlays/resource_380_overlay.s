.syntax unified
	.thumb
	.section .text.x0200a27c,"ax",%progbits
	.balign 4
	.global Func_0200227c
	.thumb_func
Func_0200227c:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6}
	mov	r6, r8
	push	{r6}
	movs	r0, #0
	bl 0x0200c9bc
	mov	sl, r0
	bl 0x0200c99c
	movs	r0, #5
	movs	r1, #1
	bl 0x0200c9cc
	movs	r0, #9
	movs	r1, #1
	bl 0x0200c9cc
	movs	r0, #11
	movs	r1, #1
	bl 0x0200c9cc
	movs	r0, #10
	movs	r1, #1
	bl 0x0200c9cc
	movs	r0, #14
	movs	r1, #1
	bl 0x0200c9cc
	movs	r0, #13
	movs	r1, #1
	bl 0x0200c9cc
	movs	r2, #166
	movs	r0, #5
	ldr	r1, [pc, #284]
	lsls	r2, r2, #17
	bl 0x0200ca04
	movs	r2, #166
	movs	r0, #9
	ldr	r1, [pc, #276]
	lsls	r2, r2, #17
	bl 0x0200ca04
	movs	r2, #174
	movs	r0, #11
	ldr	r1, [pc, #268]
	lsls	r2, r2, #17
	bl 0x0200ca04
	movs	r2, #174
	movs	r0, #10
	ldr	r1, [pc, #260]
	lsls	r2, r2, #17
	bl 0x0200ca04
	movs	r1, #230
	movs	r2, #180
	movs	r0, #14
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200ca04
	movs	r2, #153
	ldr	r1, [pc, #240]
	lsls	r2, r2, #17
	movs	r0, #13
	bl 0x0200ca04
	movs	r0, #5
	bl 0x0200c9bc
	mov	r1, sl
	str	r1, [r0, #104]
	adds	r2, r0, #0
	adds	r2, #90
	ldrb	r3, [r2, #0]
	movs	r6, #1
	orrs	r3, r6
	strb	r3, [r2, #0]
	ldr	r3, [pc, #212]
	movs	r1, #0
	mov	r8, r3
	mov	r9, r1
	mov	r1, r8
	bl 0x0200c8bc
	movs	r0, #9
	bl 0x0200c9bc
	mov	r1, sl
	str	r1, [r0, #104]
	adds	r2, r0, #0
	adds	r2, #90
	ldrb	r3, [r2, #0]
	orrs	r3, r6
	strb	r3, [r2, #0]
	mov	r1, r8
	bl 0x0200c8bc
	movs	r0, #11
	bl 0x0200c9bc
	mov	r3, sl
	str	r3, [r0, #104]
	adds	r2, r0, #0
	adds	r2, #90
	ldrb	r3, [r2, #0]
	orrs	r3, r6
	strb	r3, [r2, #0]
	mov	r1, r8
	bl 0x0200c8bc
	movs	r0, #10
	bl 0x0200c9bc
	mov	r1, sl
	str	r1, [r0, #104]
	adds	r2, r0, #0
	adds	r2, #90
	ldrb	r3, [r2, #0]
	orrs	r3, r6
	mov	r1, r8
	strb	r3, [r2, #0]
	bl 0x0200c8bc
	movs	r0, #14
	bl 0x0200c9bc
	mov	r3, sl
	adds	r5, r0, #0
	str	r3, [r5, #104]
	adds	r2, r5, #0
	adds	r2, #90
	ldrb	r3, [r2, #0]
	orrs	r3, r6
	strb	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	movs	r0, #11
	bl 0x0200c9bc
	adds	r0, #85
	ldrb	r3, [r0, #0]
	adds	r2, r5, #0
	adds	r2, #85
	mov	r1, r9
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	str	r1, [r5, #12]
	mov	r1, r8
	bl 0x0200c8bc
	movs	r0, #13
	bl 0x0200c9bc
	mov	r3, sl
	str	r3, [r0, #104]
	adds	r2, r0, #0
	adds	r2, #90
	ldrb	r3, [r2, #0]
	orrs	r6, r3
	strb	r6, [r2, #0]
	mov	r1, r8
	bl 0x0200c8bc
	bl 0x0200c9a4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x01db0000
	.4byte 0x01eb0000
	.4byte 0x01cb0000
	.4byte 0x01fb0000
	.4byte 0x01d70000
	.2byte 0xcbd0
	.2byte 0x0200
	.section .text.x0200c25e,"ax",%progbits
	.2byte 0x0000
	.global Scene_PresentItem
	.thumb_func
Scene_PresentItem:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	sl, r0
	movs	r0, #0
	mov	r8, r0
	movs	r0, #22
	bl 0x0200c8c4
	adds	r6, r0, #0
	movs	r0, #224
	bl 0x0200c95c
	movs	r1, #224
	adds	r7, r0, #0
	bl 0x0200c954
	mov	r9, r0
	adds	r0, r7, #0
	cmp	r6, #0
	beq.n	.L_02004316
	ldr	r1, [pc, #148]
	adds	r0, r6, #0
	bl 0x0200c8bc
	ldr	r5, [r6, #80]
	adds	r3, r5, #0
	mov	r2, r8
	adds	r3, #38
	strb	r2, [r3, #0]
	adds	r3, #1
	strb	r2, [r3, #0]
	movs	r3, #33
	ldrb	r2, [r5, #5]
	negs	r3, r3
	ands	r3, r2
	ldrb	r2, [r5, #9]
	strb	r3, [r5, #5]
	movs	r3, #15
	ands	r3, r2
	strb	r3, [r5, #9]
	movs	r3, #160
	lsls	r3, r3, #10
	str	r3, [r6, #40]
	movs	r3, #128
	lsls	r3, r3, #7
	movs	r1, #193
	str	r3, [r6, #72]
	lsls	r1, r1, #3
	movs	r0, #17
	bl 0x0200c894
	mov	r8, r0
	mov	r0, sl
	bl 0x0200c934
	movs	r2, #128
	lsls	r2, r2, #3
	add	r2, r8
	movs	r1, #128
	ldrb	r0, [r5, #28]
	bl 0x0200c8ac
	movs	r0, #17
	bl 0x0200c8a4
	movs	r0, #83
	bl 0x0200cb14
	adds	r0, r6, #0
	movs	r1, #3
	bl 0x0200cafc
	mov	r1, r9
	adds	r0, r7, #0
	bl 0x0200c984
	mov	r1, sl
	adds	r0, r7, #0
	bl 0x0200c944
	adds	r0, r6, #0
	bl 0x0200c8cc
	movs	r0, #0
	movs	r1, #1
	bl 0x0200ca0c
	adds	r0, r7, #0
.L_02004316:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.2byte 0xcbe4
	.2byte 0x0200
	.section .text.x0200c49c,"ax",%progbits
	.global Soru_UpdateRing
	.thumb_func
Soru_UpdateRing:
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #202
	lsls	r1, r1, #1
	movs	r0, #33
	sub	sp, #68
	bl 0x0200c89c
	str	r0, [sp, #64]
	str	r0, [sp, #60]
	ldr	r1, [sp, #64]
	movs	r0, #0
	movs	r2, #200
	str	r0, [sp, #56]
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_020044ce
	b.n	.L_02004762
.L_020044ce:
	adds	r1, #8
	ldr	r3, [sp, #64]
	ldr	r4, [sp, #64]
	str	r0, [sp, #8]
	ldr	r0, [pc, #668]
	mov	sl, r1
	ldr	r1, [pc, #668]
	adds	r3, #36
	adds	r4, #37
	adds	r0, #1
	str	r3, [sp, #16]
	str	r4, [sp, #12]
	str	r0, [sp, #4]
	str	r1, [sp, #0]
.L_020044ea:
	mov	r3, sl
	ldr	r3, [r3, #8]
	ldr	r2, [sp, #60]
	ldr	r5, [r2, #0]
	str	r3, [sp, #52]
	mov	r4, sl
	ldr	r4, [r4, #12]
	str	r4, [sp, #48]
	mov	r0, sl
	ldr	r0, [r0, #16]
	str	r0, [sp, #44]
	mov	r1, sl
	ldr	r1, [r1, #20]
	str	r1, [sp, #40]
	mov	r2, sl
	ldr	r2, [r2, #24]
	ldr	r4, [sp, #60]
	str	r2, [sp, #36]
	ldr	r3, [sp, #12]
	ldr	r4, [r4, #4]
	ldrb	r3, [r3, #0]
	ldr	r0, [sp, #60]
	str	r4, [sp, #28]
	ldr	r0, [r0, #8]
	ldr	r2, [sp, #60]
	str	r0, [sp, #24]
	ldr	r2, [r2, #12]
	mov	fp, r3
	str	r2, [sp, #20]
	ldr	r3, [sp, #16]
	ldrb	r3, [r3, #0]
	str	r3, [sp, #32]
	adds	r3, #255
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	mov	r1, fp
	str	r3, [sp, #32]
	cmp	r3, #0
	beq.n	.L_0200453a
	b.n	.L_020046ee
.L_0200453a:
	movs	r4, #3
	str	r4, [sp, #32]
	cmp	r1, #0
	bne.n	.L_0200458e
	ldr	r0, [sp, #40]
	ldr	r2, [sp, #36]
	ldr	r4, [sp, #56]
	adds	r0, r0, r2
	str	r0, [sp, #40]
	ldr	r3, [pc, #556]
	lsls	r2, r4, #2
	ldr	r3, [r3, r2]
	cmp	r0, r3
	blt.n	.L_02004560
	ldr	r3, [pc, #552]
	ldr	r3, [r3, r2]
	negs	r3, r3
	str	r3, [sp, #36]
	b.n	.L_02004588
.L_02004560:
	ldr	r0, [sp, #40]
	ldr	r3, [pc, #544]
	cmp	r0, r3
	bgt.n	.L_02004588
	ldr	r3, [pc, #532]
	ldr	r4, [pc, #536]
	ldr	r3, [r3, r2]
	str	r4, [sp, #40]
	str	r3, [sp, #36]
	ldr	r2, [r5, #8]
	str	r2, [sp, #28]
	ldr	r3, [r5, #12]
	str	r3, [sp, #24]
	ldr	r4, [r5, #16]
	movs	r0, #24
	str	r4, [sp, #20]
	str	r1, [r5, #8]
	str	r1, [r5, #12]
	str	r1, [r5, #16]
	mov	fp, r0
.L_02004588:
	ldr	r0, [sp, #40]
	str	r0, [r5, #24]
	str	r0, [r5, #28]
.L_0200458e:
	bl 0x0200c874
	ldr	r2, [pc, #480]
	ldr	r1, [sp, #8]
	ldrb	r3, [r1, r2]
	muls	r3, r0
	lsrs	r6, r3, #16
	bl 0x0200c874
	ldr	r4, [sp, #4]
	ldrb	r3, [r4, #0]
	muls	r3, r0
	lsrs	r7, r3, #16
	bl 0x0200c874
	ldr	r1, [sp, #4]
	ldrb	r3, [r1, #1]
	muls	r3, r0
	lsrs	r3, r3, #16
	mov	r8, r3
	cmp	r6, #0
	beq.n	.L_020045c8
	movs	r1, #250
	lsls	r0, r6, #16
	lsls	r1, r1, #2
	bl 0x0200c85c
	adds	r6, r0, #0
	b.n	.L_020045ca
.L_020045c8:
	movs	r6, #0
.L_020045ca:
	cmp	r7, #0
	beq.n	.L_020045dc
	movs	r1, #250
	lsls	r0, r7, #16
	lsls	r1, r1, #2
	bl 0x0200c85c
	mov	r9, r0
	b.n	.L_020045e0
.L_020045dc:
	movs	r2, #0
	mov	r9, r2
.L_020045e0:
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_020045f2
	movs	r1, #250
	lsls	r0, r3, #16
	lsls	r1, r1, #2
	bl 0x0200c85c
	b.n	.L_020045f4
.L_020045f2:
	movs	r0, #0
.L_020045f4:
	ldr	r2, [pc, #400]
	ldr	r4, [sp, #8]
	ldrsb	r3, [r2, r4]
	cmp	r3, #1
	bne.n	.L_02004606
	ldr	r1, [sp, #52]
	adds	r1, r1, r6
	str	r1, [sp, #52]
	b.n	.L_02004618
.L_02004606:
	ldr	r4, [sp, #52]
	movs	r1, #1
	subs	r4, r4, r6
	negs	r1, r1
	str	r4, [sp, #52]
	cmp	r3, r1
	beq.n	.L_02004618
	movs	r3, #0
	str	r3, [sp, #52]
.L_02004618:
	ldr	r3, [sp, #8]
	adds	r3, #1
	ldrsb	r3, [r2, r3]
	cmp	r3, #1
	bne.n	.L_0200462a
	ldr	r4, [sp, #48]
	add	r4, r9
	str	r4, [sp, #48]
	b.n	.L_0200463e
.L_0200462a:
	ldr	r1, [sp, #48]
	mov	r4, r9
	subs	r1, r1, r4
	str	r1, [sp, #48]
	movs	r1, #1
	negs	r1, r1
	cmp	r3, r1
	beq.n	.L_0200463e
	movs	r3, #0
	str	r3, [sp, #48]
.L_0200463e:
	ldr	r3, [sp, #8]
	adds	r3, #2
	ldrsb	r3, [r2, r3]
	cmp	r3, #1
	bne.n	.L_02004650
	ldr	r4, [sp, #44]
	adds	r4, r4, r0
	str	r4, [sp, #44]
	b.n	.L_02004662
.L_02004650:
	ldr	r1, [sp, #44]
	movs	r2, #1
	subs	r1, r1, r0
	negs	r2, r2
	str	r1, [sp, #44]
	cmp	r3, r2
	beq.n	.L_02004662
	movs	r3, #0
	str	r3, [sp, #44]
.L_02004662:
	ldr	r4, [sp, #0]
	ldr	r1, [sp, #52]
	ldrb	r3, [r4, #0]
	adds	r0, r3, #0
	muls	r0, r1
	bl 0x0200c884
	ldr	r2, [sp, #0]
	ldr	r4, [sp, #48]
	ldrb	r3, [r2, #1]
	lsls	r6, r0, #1
	adds	r0, r3, #0
	muls	r0, r4
	bl 0x0200c884
	lsls	r7, r0, #1
	ldr	r0, [sp, #0]
	ldr	r1, [sp, #44]
	ldrb	r3, [r0, #2]
	adds	r0, r3, #0
	muls	r0, r1
	bl 0x0200c88c
	mov	r2, fp
	lsls	r0, r0, #1
	cmp	r2, #0
	beq.n	.L_020046d0
	ldr	r3, [sp, #28]
	adds	r3, r3, r6
	str	r3, [sp, #28]
	mov	r3, fp
	ldr	r4, [sp, #24]
	ldr	r1, [sp, #20]
	adds	r3, #255
	lsls	r3, r3, #24
	adds	r4, r4, r7
	adds	r1, r1, r0
	lsrs	r3, r3, #24
	str	r4, [sp, #24]
	str	r1, [sp, #20]
	mov	fp, r3
	cmp	r3, #0
	bne.n	.L_020046ee
	ldr	r2, [sp, #28]
	mov	r3, r9
	str	r2, [r5, #8]
	str	r2, [r5, #56]
	cmp	r3, #0
	beq.n	.L_020046c8
	str	r4, [r5, #12]
	str	r4, [r5, #60]
.L_020046c8:
	ldr	r4, [sp, #20]
	str	r4, [r5, #16]
	str	r4, [r5, #64]
	b.n	.L_020046ee
.L_020046d0:
	ldr	r3, [r5, #8]
	mov	r1, r9
	adds	r3, r3, r6
	str	r3, [r5, #8]
	str	r3, [r5, #56]
	cmp	r1, #0
	beq.n	.L_020046e6
	ldr	r3, [r5, #12]
	adds	r3, r3, r7
	str	r3, [r5, #12]
	str	r3, [r5, #60]
.L_020046e6:
	ldr	r3, [r5, #16]
	adds	r3, r3, r0
	str	r3, [r5, #16]
	str	r3, [r5, #64]
.L_020046ee:
	ldr	r2, [sp, #52]
	mov	r3, sl
	str	r2, [r3, #8]
	ldr	r4, [sp, #48]
	str	r4, [r3, #12]
	ldr	r0, [sp, #44]
	str	r0, [r3, #16]
	ldr	r1, [sp, #40]
	str	r1, [r3, #20]
	ldr	r2, [sp, #36]
	str	r2, [r3, #24]
	ldr	r4, [sp, #12]
	mov	r3, fp
	strb	r3, [r4, #0]
	ldr	r0, [sp, #28]
	ldr	r1, [sp, #60]
	str	r0, [r1, #4]
	ldr	r2, [sp, #24]
	mov	r3, sl
	str	r2, [r3, #0]
	ldr	r4, [sp, #20]
	add	r0, sp, #32
	str	r4, [r1, #12]
	ldrb	r0, [r0, #0]
	ldr	r1, [sp, #16]
	strb	r0, [r1, #0]
	ldr	r1, [sp, #0]
	ldr	r2, [sp, #8]
	adds	r1, #3
	adds	r2, #3
	ldr	r3, [sp, #4]
	ldr	r4, [sp, #56]
	str	r1, [sp, #0]
	str	r2, [sp, #8]
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #12]
	adds	r3, #3
	adds	r4, #1
	adds	r1, #40
	adds	r2, #40
	str	r3, [sp, #4]
	str	r4, [sp, #56]
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	ldr	r3, [sp, #60]
	movs	r0, #40
	adds	r3, #40
	add	sl, r0
	ldr	r4, [sp, #64]
	movs	r0, #200
	str	r3, [sp, #60]
	lsls	r0, r0, #1
	adds	r3, r4, r0
	ldrh	r3, [r3, #0]
	ldr	r1, [sp, #56]
	cmp	r1, r3
	beq.n	.L_02004762
	b.n	.L_020044ea
.L_02004762:
	add	sp, #68
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x0200d0e4
	.4byte 0x0200d102
	.4byte 0x0200d140
	.4byte 0x0200d168
	.4byte 0x00001999
	.2byte 0xd120
	.2byte 0x0200
	.section .rodata,"a",%progbits
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.global gEffectScripts
gEffectScripts:
	.4byte 0x0200cb1c
	.4byte 0x0200cb54
	.4byte 0x0200cb8c
	.global SoruStar_AngleScript
SoruStar_AngleScript:
	.4byte 0x00000022
	.4byte 0x02008315
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00008000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000c000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000e000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000e000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000f000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000f000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00016000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00016000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00012000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00012000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00016000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00016000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00012000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00012000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000010
	.global Placement_Scripts
Placement_Scripts:
	.4byte 0xffff0000
	.4byte 0x000001d8
	.4byte 0x40000142
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Placement_Messages
Placement_Messages:
	.4byte 0x00000011
	.4byte 0x0020a012
	.4byte 0x000001ff
	.global Placement_Actors
Placement_Actors:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00034000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00034000
	.4byte 0xffff0020
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0021
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0022
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0023
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff002b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x02690000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x01ad0000
	.4byte 0x00000000
	.4byte 0x00dd0000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x00710000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x00aa0000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x01340000
	.4byte 0x00000000
	.4byte 0x01ff0000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x018d0000
	.4byte 0x00000000
	.4byte 0x01a20000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x021e0000
	.4byte 0x00000000
	.4byte 0x010d0000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x01a50000
	.4byte 0x00000000
	.4byte 0x00750000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x01270000
	.4byte 0x00000000
	.4byte 0x00d20000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x02e60000
	.4byte 0x00000000
	.4byte 0x01360000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Placement_Effects
Placement_Effects:
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0200a675
	.4byte 0x00000000
	.4byte 0xffff0005
	.4byte 0x0200a6e1
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0200a74d
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0200a76d
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0200a78d
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0200a7ad
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000010cb
	.4byte 0x00008d15
	.4byte 0xffff0005
	.4byte 0x0200a6e1
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000010ca
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0200a76d
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000010cc
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x0200a7cd
	.4byte 0x00000602
	.4byte 0xffff000a
	.4byte 0x0200a7ed
	.4byte 0x00004602
	.4byte 0xffff000b
	.4byte 0x0200a7ed
	.4byte 0x00000003
	.4byte 0x083c0002
	.4byte 0x02008391
	.4byte 0x00000003
	.4byte 0x083d0003
	.4byte 0x020086f5
	.4byte 0x00000003
	.4byte 0x083e0004
	.4byte 0x02008a65
	.4byte 0x00000003
	.4byte 0x083f0005
	.4byte 0x0200a401
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global SoruStar_StarCells
SoruStar_StarCells:
	.4byte 0x0028003b
	.4byte 0x00040003
	.4byte 0x003e0006
	.4byte 0x00030028
	.4byte 0x00060004
	.4byte 0x00280041
	.4byte 0x00040003
	.4byte 0x00440006
	.4byte 0x00030028
	.4byte 0x00060004
	.4byte 0x00280047
	.4byte 0x00040003
	.4byte 0x004a0006
	.4byte 0x00030028
	.4byte 0x00060004
	.4byte 0x0028004d
	.4byte 0x00040003
	.4byte 0x00500006
	.4byte 0x00030028
	.4byte 0x00060004
	.4byte 0x00280053
	.4byte 0x00040003
	.4byte 0xffff0000
	.4byte 0x04040404
	.4byte 0x00040300
	.4byte 0x04000404
	.4byte 0x04040003
	.4byte 0x00060406
	.4byte 0x03000404
	.4byte 0x02010002
	.4byte 0x01010200
	.4byte 0x02000102
	.4byte 0x01020001
	.4byte 0x00010100
	.4byte 0x02010102
	.4byte 0x01020001
	.4byte 0x00010200
	.4byte 0x02000102
	.4byte 0x01010101
	.4byte 0x00010100
	.4byte 0x0100ff01
	.4byte 0x01ff00ff
	.4byte 0x00ff0101
	.4byte 0xff00ffff
	.4byte 0xffff00ff
	.4byte 0x0000ff00
	.global Soru_RingOffsetX
Soru_RingOffsetX:
	.4byte 0x00009999
	.4byte 0x0000cccc
	.4byte 0x0000b333
	.4byte 0x00009999
	.4byte 0x0000cccc
	.4byte 0x00009999
	.4byte 0x0000b333
	.4byte 0x00009999
	.4byte 0x00009999
	.4byte 0x0000b333
	.global Soru_RingOffsetZ
Soru_RingOffsetZ:
	.4byte 0x0000028f
	.4byte 0x000001ca
	.4byte 0x0000028f
	.4byte 0x000001ca
	.4byte 0x0000028f
	.4byte 0x000001ca
	.4byte 0x0000028f
	.4byte 0x0000028f
	.4byte 0x0000020c
	.4byte 0x0000028f
