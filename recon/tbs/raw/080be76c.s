.syntax unified
	.thumb
	.set sub_08015120, 0x08015120
	.set sub_080151c8, 0x080151c8
	.set sub_08077018, 0x08077018
	.set sub_08077078, 0x08077078
	.set sub_08077080, 0x08077080
	.set sub_08077128, 0x08077128
	.set sub_08077160, 0x08077160
	.set sub_080bb8d8, 0x080bb8d8
	.set sub_080be18c, 0x080be18c
	.set sub_080bec8a, 0x080bec8a
	.set sub_080bee00, 0x080bee00
	.set sub_080bee08, 0x080bee08
	.set sub_080bf1d6, 0x080bf1d6
	.global Func_080be76c
	.thumb_func
Func_080be76c:
	ldr	r4, [sp, #12]
	ldr	r0, [r4, #0]
	bl	sub_08077160
	mov	fp, r0
	add	r0, sp, #48
	mov	r9, r0
	mov	r0, fp
	bl	sub_080be18c
	movs	r1, #1
	negs	r1, r1
	cmp	r0, r1
	bne.n	.L_080be78c
	bl	sub_080bf1d6
.L_080be78c:
	mov	r2, fp
	cmp	r2, #1
	bne.n	.L_080be794
	b.n	sub_080bee08
.L_080be794:
	mov	r4, sl
	ldr	r3, [r4, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r1, #1
	bl	sub_08015120
	ldr	r2, [sp, #12]
	movs	r1, #1
	ldr	r0, [r2, #0]
	bl	sub_08077078
	movs	r1, #2
	bl	sub_08015120
	ldr	r5, [pc, #744]
	adds	r0, r5, #0
	bl	sub_080151c8
	adds	r5, #1
	bl	sub_080bb8d8
	mov	r0, fp
	movs	r1, #4
	bl	sub_08015120
	adds	r0, r5, #0
.L_080be7ca:
	bl	sub_080151c8
	b.n	sub_080bee00
	mov	r4, sl
	ldr	r3, [r4, #0]
	movs	r1, #8
	ldrsh	r0, [r3, r1]
	mov	fp, r0
	bl	sub_08077080
	add	r2, sp, #48
	adds	r6, r0, #0
	mov	r9, r2
	mov	r0, fp
	bl	sub_080be18c
	movs	r3, #1
	negs	r3, r3
	movs	r5, #1
	cmp	r0, r3
	bne.n	.L_080be7f8
	bl	sub_080bf1d6
.L_080be7f8:
	mov	r4, sl
	ldr	r3, [r4, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r1, #1
	bl	sub_08015120
	movs	r1, #4
	mov	r0, fp
	bl	sub_08015120
	ldr	r0, [pc, #656]
	bl	sub_080151c8
	ldr	r2, [sp, #12]
	ldr	r1, [r2, #0]
	movs	r3, #58
	ldrsh	r2, [r1, r3]
	ldrb	r3, [r6, #9]
	cmp	r2, r3
	bge.n	.L_080be82c
	ldr	r4, [sp, #8]
	ldr	r2, [r4, #0]
	movs	r3, #2
	str	r3, [r2, #92]
	movs	r5, #0
.L_080be82c:
	ldr	r0, [pc, #628]
	adds	r3, r1, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080be840
	ldr	r1, [sp, #8]
	ldr	r2, [r1, #0]
	movs	r3, #1
	str	r3, [r2, #92]
	movs	r5, #0
.L_080be840:
	cmp	r5, #0
	bne.n	.L_080be846
	b.n	sub_080bee00
.L_080be846:
	ldr	r2, [sp, #8]
	ldr	r3, [r2, #0]
	movs	r5, #0
	str	r5, [r3, #92]
	ldr	r3, [sp, #12]
	ldr	r1, [r3, #0]
	ldrb	r2, [r6, #9]
	ldrh	r3, [r1, #58]
	mov	r4, sl
	subs	r3, r3, r2
	strh	r3, [r1, #58]
	ldr	r3, [r4, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	bl	sub_08077128
	ldr	r2, [sp, #12]
	ldr	r1, [r2, #0]
	movs	r4, #58
	ldrsh	r3, [r1, r4]
	cmp	r3, #0
	bge.n	.L_080be874
	strh	r5, [r1, #58]
.L_080be874:
	movs	r0, #58
	ldrsh	r2, [r1, r0]
	movs	r4, #54
	ldrsh	r3, [r1, r4]
	ldrh	r0, [r1, #54]
	cmp	r2, r3
	bgt.n	.L_080be884
	b.n	sub_080bee00
.L_080be884:
	strh	r0, [r1, #58]
	b.n	sub_080bee00
	mov	r0, sl
	ldr	r3, [r0, #0]
	movs	r1, #8
	ldrsh	r2, [r3, r1]
	cmp	r2, #0
	bge.n	.L_080be8a6
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	movs	r1, #1
	bl	sub_08015120
	ldr	r0, [pc, #520]
	bl	sub_080151c8
	b.n	sub_080bec8a
.L_080be8a6:
	ldr	r4, [sp, #12]
	lsls	r2, r2, #1
	ldr	r3, [r4, #0]
	adds	r2, #216
	ldrh	r0, [r3, r2]
	bl	sub_08077018
	adds	r5, r0, #0
	ldrh	r0, [r5, #40]
	mov	fp, r0
	cmp	r0, #0
	beq.n	.L_080be8dc
	ldr	r1, [sp, #12]
	mov	r3, sl
	ldr	r2, [r1, #0]
	ldr	r1, [r3, #0]
	movs	r4, #8
	ldrsh	r3, [r1, r4]
	lsls	r3, r3, #1
	adds	r3, #216
	ldrh	r2, [r2, r3]
	movs	r3, #128
	lsls	r3, r3, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080be908
	b.n	.L_080be8e0
.L_080be8dc:
	mov	r0, sl
	ldr	r1, [r0, #0]
.L_080be8e0:
	movs	r2, #0
	ldrsh	r0, [r1, r2]
	movs	r1, #1
	bl	sub_08015120
	ldr	r0, [pc, #448]
	bl	sub_080151c8
	ldr	r4, [sp, #12]
	ldr	r0, [pc, #444]
	ldr	r3, [r4, #0]
	adds	r2, r3, r0
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	beq.n	.L_080be902
	b.n	sub_080bec8a
.L_080be902:
	movs	r3, #1
	strb	r3, [r2, #0]
	b.n	sub_080bec8a
.L_080be908:
	add	r1, sp, #48
	mov	r9, r1
	mov	r0, fp
	bl	sub_080be18c
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L_080be91e
	bl	sub_080bf1d6
.L_080be91e:
	mov	r4, sl
	ldr	r3, [r4, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r1, #1
	bl	sub_08015120
	ldr	r3, [sp, #12]
	mov	r4, sl
	ldr	r2, [r3, #0]
	ldr	r3, [r4, #0]
	movs	r0, #8
	ldrsh	r3, [r3, r0]
	lsls	r3, r3, #1
	adds	r3, #216
	ldrh	r0, [r2, r3]
	movs	r1, #2
	bl	sub_08015120
	ldrb	r3, [r5, #12]
	cmp	r3, #2
	beq.n	.L_080be94e
	cmp	r3, #0
	bne.n	.L_080be96a
.L_080be94e:
	ldrb	r0, [r5, #2]
	cmp	r0, #3
	beq.n	.L_080be966
	cmp	r0, #3
	bgt.n	.L_080be95e
	cmp	r0, #1
	beq.n	.L_080be966
	b.n	.L_080be96a
.L_080be95e:
	cmp	r0, #8
	bgt.n	.L_080be96a
	cmp	r0, #6
	blt.n	.L_080be96a
.L_080be966:
	ldr	r0, [pc, #332]
	b.n	.L_080be7ca
.L_080be96a:
	ldr	r0, [pc, #332]
	b.n	.L_080be7ca
	mov	r1, sl
	ldr	r3, [r1, #0]
	movs	r1, #1
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl	sub_08015120
	ldr	r0, [pc, #300]
	bl	sub_080151c8
	b.n	sub_080bec8a
	mov	r4, sl
	ldr	r3, [r4, #0]
	add	r2, sp, #48
	movs	r1, #8
	ldrsh	r0, [r3, r1]
	mov	r9, r2
	mov	fp, r0
	bl	sub_080be18c
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L_080be9a2
	bl	sub_080bf1d6
.L_080be9a2:
	mov	r4, sl
	ldr	r3, [r4, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r1, #1
	bl	sub_08015120
	mov	r0, fp
	movs	r1, #4
	bl	sub_08015120
	mov	r0, fp
	bl	sub_08077080
	ldrb	r2, [r0, #1]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #6
	bne.n	.L_080be9cc
	ldr	r0, [pc, #240]
	b.n	.L_080be9ce
.L_080be9cc:
	ldr	r0, [pc, #240]
.L_080be9ce:
	movs	r3, #244
	lsls	r3, r3, #1
	cmp	fp, r3
	beq.n	.L_080bea84
	cmp	fp, r3
	bgt.n	.L_080bea12
	ldr	r2, [pc, #232]
	cmp	fp, r2
	bgt.n	.L_080bea00
	subs	r3, #52
	cmp	fp, r3
	bgt.n	.L_080bea78
	mov	r4, fp
	cmp	r4, #224
	beq.n	.L_080bea64
	cmp	r4, #224
	bge.n	.L_080be9f2
	b.n	.L_080be7ca
.L_080be9f2:
	movs	r1, #217
	lsls	r1, r1, #1
	cmp	fp, r1
	bgt.n	.L_080be9fc
	b.n	.L_080be7ca
.L_080be9fc:
	ldr	r0, [pc, #200]
	b.n	.L_080be7ca
.L_080bea00:
	movs	r2, #222
	lsls	r2, r2, #1
	cmp	fp, r2
	ble.n	.L_080bea7c
	movs	r3, #236
	lsls	r3, r3, #1
	cmp	fp, r3
	beq.n	.L_080bea80
	b.n	.L_080be7ca
.L_080bea12:
	movs	r3, #250
	lsls	r3, r3, #1
	cmp	fp, r3
	beq.n	.L_080bea68
	cmp	fp, r3
	bgt.n	.L_080bea40
	subs	r3, #6
	cmp	fp, r3
	beq.n	.L_080bea74
	cmp	fp, r3
	bgt.n	.L_080bea32
	movs	r4, #246
	lsls	r4, r4, #1
	cmp	fp, r4
	beq.n	.L_080bea88
	b.n	.L_080be7ca
.L_080bea32:
	ldr	r1, [pc, #152]
	cmp	fp, r1
	beq.n	.L_080bea8c
	ldr	r2, [pc, #148]
	cmp	fp, r2
	beq.n	.L_080bea70
	b.n	.L_080be7ca
.L_080bea40:
	ldr	r3, [pc, #144]
	cmp	fp, r3
	beq.n	.L_080bea90
	cmp	fp, r3
	bgt.n	.L_080bea52
	subs	r3, #2
	cmp	fp, r3
	beq.n	.L_080bea6c
	b.n	.L_080be7ca
.L_080bea52:
	movs	r4, #252
	lsls	r4, r4, #1
	cmp	fp, r4
	beq.n	.L_080bea94
	movs	r1, #254
	lsls	r1, r1, #1
	cmp	fp, r1
	beq.n	.L_080bea98
	b.n	.L_080be7ca
.L_080bea64:
	ldr	r0, [pc, #56]
	b.n	.L_080be7ca
.L_080bea68:
	ldr	r0, [pc, #108]
	b.n	.L_080be7ca
.L_080bea6c:
	ldr	r0, [pc, #108]
	b.n	.L_080be7ca
.L_080bea70:
	ldr	r0, [pc, #108]
	b.n	.L_080be7ca
.L_080bea74:
	ldr	r0, [pc, #108]
	b.n	.L_080be7ca
.L_080bea78:
	ldr	r0, [pc, #108]
	b.n	.L_080be7ca
.L_080bea7c:
	ldr	r0, [pc, #64]
	b.n	.L_080be7ca
.L_080bea80:
	ldr	r0, [pc, #104]
	b.n	.L_080be7ca
.L_080bea84:
	ldr	r0, [pc, #104]
	b.n	.L_080be7ca
.L_080bea88:
	ldr	r0, [pc, #104]
	b.n	.L_080be7ca
.L_080bea8c:
	ldr	r0, [pc, #104]
	b.n	.L_080be7ca
.L_080bea90:
	ldr	r0, [pc, #104]
	b.n	.L_080be7ca
.L_080bea94:
	ldr	r0, [pc, #104]
	b.n	.L_080be7ca
.L_080bea98:
	ldr	r0, [pc, #104]
	b.n	.L_080be7ca
