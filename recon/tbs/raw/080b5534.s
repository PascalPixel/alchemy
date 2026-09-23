.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_08015000, 0x08015000
	.set sub_08015018, 0x08015018
	.set sub_08015020, 0x08015020
	.set sub_08015038, 0x08015038
	.set sub_08015048, 0x08015048
	.set sub_08015118, 0x08015118
	.set sub_08015120, 0x08015120
	.set sub_08015148, 0x08015148
	.set sub_08077008, 0x08077008
	.set sub_080f9010, 0x080f9010
	.global Unnamed_080b5534
	.global Func_080b5534
	.thumb_func
Unnamed_080b5534:
Func_080b5534:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r2, #0
	movs	r0, #0
	sub	sp, #128
	mov	sl, r2
	bl	sub_08077008
	mov	r5, sp
	adds	r6, r0, #0
	adds	r1, r5, #0
	ldr	r0, [pc, #356]
	bl	sub_08015020
	mov	r2, sl
	ldrh	r3, [r5, r2]
	strb	r3, [r6, #0]
	lsls	r3, r3, #16
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_080b557a
	adds	r2, r6, #0
	adds	r1, r5, #0
.L_080b5566:
	adds	r0, #1
	cmp	r0, #13
	bgt.n	.L_080b557a
	adds	r1, #2
	ldrh	r3, [r1, #0]
	adds	r2, #1
	strb	r3, [r2, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_080b5566
.L_080b557a:
	movs	r3, #0
	strb	r3, [r6, #14]
	bl	sub_08015000
	movs	r0, #71
	bl	sub_080f9010
	movs	r5, #0
	ldr	r2, [pc, #300]
	movs	r3, #128
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	ldr	r3, [pc, #296]
	movs	r2, #131
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r2, #2
.L_080b559c:
	strb	r2, [r3, #0]
	bl	sub_08015118
	ldr	r0, [pc, #284]
	movs	r1, #5
	bl	sub_08015120
	movs	r0, #0
	movs	r1, #3
	bl	sub_08015120
	movs	r0, #1
	movs	r1, #1
	bl	sub_08015120
	movs	r0, #1
	movs	r1, #2
	bl	sub_08015120
	movs	r0, #2
	movs	r1, #4
	bl	sub_08015120
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_080b55da
	ldr	r3, [pc, #240]
	movs	r1, #2
	adds	r0, r5, r3
	movs	r2, #10
	b.n	.L_080b55e2
.L_080b55da:
	ldr	r3, [pc, #236]
	movs	r1, #2
	adds	r0, r5, r3
	movs	r2, #2
.L_080b55e2:
	movs	r3, #4
	bl	sub_08015038
	adds	r7, r0, #0
	movs	r0, #10
	bl	sub_080030f8
	ldr	r2, [pc, #216]
	ldr	r3, [pc, #208]
	ldr	r6, [pc, #216]
	subs	r2, r2, r3
	mov	r8, r2
	b.n	.L_080b5614
.L_080b55fc:
	bl	sub_08015048
	cmp	r0, #0
	beq.n	.L_080b560e
	ldr	r3, [r6, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080b569a
.L_080b560e:
	movs	r0, #1
	bl	sub_080030f8
.L_080b5614:
	ldr	r3, [r6, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b5630
	mov	r2, sl
	cmp	r2, #0
	beq.n	.L_080b562a
	movs	r3, #0
	mov	sl, r3
	b.n	.L_080b5630
.L_080b562a:
	movs	r2, #1
	adds	r5, #1
	mov	sl, r2
.L_080b5630:
	ldr	r3, [r6, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b563c
	adds	r5, #1
.L_080b563c:
	ldr	r3, [r6, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b5648
	subs	r5, #2
.L_080b5648:
	ldr	r3, [r6, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b5656
	movs	r3, #1
	mov	sl, r3
.L_080b5656:
	ldr	r3, [r6, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b5664
	movs	r2, #0
	mov	sl, r2
.L_080b5664:
	ldr	r3, [r6, #0]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b5672
	adds	r5, #10
.L_080b5672:
	ldr	r3, [r6, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b5680
	subs	r5, #10
.L_080b5680:
	cmp	r5, #0
	bge.n	.L_080b5686
	movs	r5, #0
.L_080b5686:
	mov	r3, r8
	adds	r3, #5
	cmp	r5, r3
	bcc.n	.L_080b5690
	adds	r5, r3, #0
.L_080b5690:
	ldr	r3, [r6, #0]
	ldr	r2, [pc, #64]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b55fc
.L_080b569a:
	movs	r0, #1
	bl	sub_08015148
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_08015018
	ldr	r3, [pc, #44]
	ldr	r2, [pc, #48]
	ldr	r3, [r3, #0]
	adds	r3, r3, r2
	movs	r2, #0
	b.n	.L_080b559c
	.4byte 0x00000903
	.4byte 0x00001341
	.4byte 0x02000240
	.4byte 0x000003e7
	.4byte 0x000026fa
	.4byte 0x00000ad0
	.4byte 0x00002850
	.4byte 0x03001b04
	.4byte 0x000003f2
	.4byte 0x03001e8c
	.4byte 0x000012f8
