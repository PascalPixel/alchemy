.syntax unified
	.thumb
	.set sub_080022f4, 0x080022f4
	.set sub_08002304, 0x08002304
	.set sub_08015080, 0x08015080
	.set sub_08015270, 0x08015270
	.set sub_080aa538, 0x080aa538
	.set sub_080aafb8, 0x080aafb8
	.set sub_080ab1f4, 0x080ab1f4
	.set sub_080ab2ec, 0x080ab2ec
	.set sub_080ae714, 0x080ae714
	.global Unnamed_080ab5e4
	.global Func_080ab5e4
	.thumb_func
Unnamed_080ab5e4:
Func_080ab5e4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #108
	str	r0, [sp, #80]
	ldr	r3, [pc, #496]
	ldr	r3, [r3, #0]
	movs	r0, #194
	str	r3, [sp, #76]
	ldr	r2, [sp, #80]
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r3, [r3, #0]
	lsls	r2, r2, #1
	movs	r1, #1
	movs	r5, #186
	str	r1, [sp, #72]
	str	r2, [sp, #52]
	ldr	r7, [sp, #76]
	lsls	r5, r5, #1
	mov	r9, r3
	adds	r3, r2, r5
	ldrh	r5, [r7, r3]
	movs	r1, #10
	adds	r0, r5, #0
	bl	sub_08002304
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	str	r0, [sp, #56]
	movs	r1, #10
	adds	r0, r5, #0
	bl	sub_080022f4
	mov	r2, sp
	adds	r2, #100
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	str	r2, [sp, #28]
	movs	r1, #1
	str	r0, [sp, #48]
	negs	r1, r1
	movs	r0, #0
	ldr	r5, [sp, #28]
	mov	r3, sp
	str	r0, [sp, #44]
	str	r0, [sp, #40]
	str	r0, [sp, #36]
	str	r0, [sp, #32]
	str	r1, [sp, #60]
	movs	r2, #0
	adds	r3, #107
	mov	ip, r5
.L_080ab656:
	strb	r2, [r3, #0]
	subs	r3, #1
	cmp	r3, ip
	bge.n	.L_080ab656
	ldr	r7, [sp, #80]
	cmp	r7, #0
	bne.n	.L_080ab6d8
	mov	r0, r9
	bl	sub_080aafb8
	ldr	r3, [pc, #384]
	movs	r0, #0
	ldr	r1, [sp, #76]
	str	r0, [sp, #68]
	adds	r2, r1, r3
	ldrb	r3, [r2, #0]
	cmp	r7, r3
	bge.n	.L_080ab69e
	adds	r0, r2, #0
	ldr	r1, [sp, #28]
	mov	r2, r9
	movs	r4, #4
	adds	r2, #160
.L_080ab684:
	ldrb	r3, [r2, #0]
	lsls	r3, r3, #24
	adds	r2, #1
	cmp	r3, #0
	bne.n	.L_080ab690
	strb	r4, [r1, #0]
.L_080ab690:
	ldr	r5, [sp, #68]
	adds	r5, #1
	str	r5, [sp, #68]
	ldrb	r3, [r0, #0]
	adds	r1, #1
	cmp	r5, r3
	blt.n	.L_080ab684
.L_080ab69e:
	ldr	r7, [sp, #48]
	movs	r0, #0
	ldr	r1, [sp, #76]
	ldr	r2, [pc, #324]
	str	r7, [sp, #44]
	str	r0, [sp, #68]
	adds	r3, r1, r2
	ldrb	r3, [r3, #0]
	cmp	r0, r3
	bge.n	.L_080ab746
	adds	r5, r1, r2
.L_080ab6b4:
	ldr	r7, [sp, #28]
	ldr	r0, [sp, #56]
	ldrsb	r3, [r7, r0]
	cmp	r3, #4
	bne.n	.L_080ab6ca
	adds	r0, #1
	str	r0, [sp, #56]
	ldrb	r1, [r5, #0]
	bl	sub_080aa538
	str	r0, [sp, #56]
.L_080ab6ca:
	ldr	r1, [sp, #68]
	adds	r1, #1
	str	r1, [sp, #68]
	ldrb	r3, [r5, #0]
	cmp	r1, r3
	blt.n	.L_080ab6b4
	b.n	.L_080ab746
.L_080ab6d8:
	ldr	r2, [sp, #76]
	add	r5, sp, #84
	movs	r1, #28
	ldrsb	r1, [r2, r1]
	adds	r0, r5, #0
	bl	sub_080ae714
	movs	r3, #0
	ldr	r7, [sp, #76]
	ldr	r0, [pc, #256]
	str	r3, [sp, #68]
	adds	r2, r7, r0
	ldrb	r3, [r2, #0]
	movs	r1, #0
	cmp	r1, r3
	bge.n	.L_080ab746
	ldr	r0, [sp, #28]
	mov	r1, r9
	adds	r4, r2, #0
	adds	r1, #160
	adds	r2, r0, #0
	movs	r6, #7
.L_080ab704:
	ldr	r7, [sp, #76]
	movs	r3, #28
	ldrsb	r3, [r7, r3]
	ldr	r7, [sp, #68]
	cmp	r7, r3
	bne.n	.L_080ab714
	strb	r6, [r2, #0]
	b.n	.L_080ab732
.L_080ab714:
	ldr	r7, [sp, #68]
	ldrb	r3, [r5, r7]
	cmp	r3, #0
	beq.n	.L_080ab722
	movs	r3, #0
	strb	r3, [r2, #0]
	b.n	.L_080ab732
.L_080ab722:
	movs	r3, #3
	strb	r3, [r2, #0]
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	cmp	r3, #0
	bne.n	.L_080ab732
	movs	r3, #7
	strb	r3, [r0, #0]
.L_080ab732:
	ldr	r3, [sp, #68]
	adds	r3, #1
	str	r3, [sp, #68]
	ldr	r7, [sp, #68]
	ldrb	r3, [r4, #0]
	adds	r0, #1
	adds	r1, #1
	adds	r2, #1
	cmp	r7, r3
	blt.n	.L_080ab704
.L_080ab746:
	ldr	r0, [sp, #80]
	cmp	r0, #1
	bne.n	.L_080ab818
	ldr	r1, [sp, #76]
	movs	r2, #186
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldrh	r6, [r3, #0]
	movs	r1, #10
	adds	r0, r6, #0
	bl	sub_08002304
	movs	r1, #10
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	sub_080022f4
	lsls	r5, r5, #16
	lsrs	r5, r5, #16
	ldr	r3, [sp, #76]
	adds	r2, r0, #0
	lsls	r6, r5, #3
	subs	r6, r6, r5
	lsls	r2, r2, #16
	ldr	r0, [r3, #48]
	ldr	r5, [sp, #80]
	adds	r6, #1
	movs	r3, #14
	lsrs	r2, r2, #16
	str	r3, [sp, #4]
	adds	r2, #2
	adds	r1, r6, #0
	movs	r3, #6
	str	r5, [sp, #0]
	bl	sub_080ab1f4
	ldr	r7, [sp, #76]
	movs	r3, #7
	ldr	r0, [r7, #48]
	adds	r1, r6, #0
	str	r3, [sp, #0]
	movs	r2, #2
	movs	r3, #6
	str	r3, [sp, #4]
	bl	sub_080ab2ec
	ldr	r0, [pc, #72]
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	movs	r5, #0
	cmp	r5, r3
	bge.n	.L_080ab818
	movs	r6, #8
.L_080ab7b0:
	ldr	r1, [sp, #76]
	movs	r3, #28
	ldrsb	r3, [r1, r3]
	cmp	r5, r3
	bne.n	.L_080ab7d2
	movs	r2, #188
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldrh	r3, [r3, #0]
	ldr	r2, [pc, #32]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ab7ce
	ldr	r0, [pc, #36]
	b.n	.L_080ab7fe
.L_080ab7ce:
	ldr	r0, [pc, #36]
	b.n	.L_080ab7fe
.L_080ab7d2:
	ldr	r3, [sp, #28]
	ldrb	r2, [r3, r5]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ab7fc
	ldr	r0, [pc, #24]
	b.n	.L_080ab7fe
	movs	r0, r0
	.4byte 0x00008000
	.4byte 0x03001f2c
	.4byte 0x00000219
	.4byte 0x00000bb0
	.4byte 0x00000baf
	.2byte 0x0bae
	.2byte 0x0000
.L_080ab7fc:
	ldr	r0, [pc, #840]
.L_080ab7fe:
	ldr	r7, [sp, #76]
	adds	r2, r6, #0
	movs	r3, #8
	ldr	r1, [r7, #48]
	bl	sub_08015080
	ldr	r0, [pc, #832]
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	adds	r5, #1
	adds	r6, #56
	cmp	r5, r3
	blt.n	.L_080ab7b0
.L_080ab818:
	ldr	r1, [sp, #76]
	movs	r2, #134
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldr	r0, [r3, #0]
	bl	sub_08015270
	ldr	r3, [sp, #76]
	ldr	r2, [r3, #20]
	movs	r3, #1
	strb	r3, [r2, #5]
	ldr	r5, [sp, #56]
	lsls	r5, r5, #3
	str	r5, [sp, #24]
