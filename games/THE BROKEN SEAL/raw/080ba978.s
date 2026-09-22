.syntax unified
	.thumb
	.set sub_080041d8, 0x080041d8
	.set sub_080044d0, 0x080044d0
	.set sub_08009080, 0x08009080
	.set sub_08009088, 0x08009088
	.set sub_08015130, 0x08015130
	.set sub_080b7dd0, 0x080b7dd0
	.set sub_080b7f70, 0x080b7f70
	.set sub_080b8000, 0x080b8000
	.set sub_080b9d34, 0x080b9d34
	.set sub_080bb938, 0x080bb938
	.set sub_080bbabc, 0x080bbabc
	.set sub_080be02c, 0x080be02c
	.set sub_080c10e8, 0x080c10e8
	.set sub_080c1798, 0x080c1798
	.set sub_080c1a14, 0x080c1a14
	.set sub_080c9008, 0x080c9008
	.set sub_080c9018, 0x080c9018
	.set sub_080f9010, 0x080f9010
	.global Overlay_080ba978
Overlay_080ba978:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #568]
	adds	r7, r0, #0
	ldr	r5, [r3, #0]
	movs	r2, #128
	ldr	r3, [r7, #88]
	lsls	r2, r2, #11
	ands	r3, r2
	sub	sp, #88
	mov	sl, r1
	cmp	r3, #0
	beq.n	.L_080ba9aa
	ldrb	r3, [r7, #0]
	ldr	r2, [pc, #548]
	cmp	r3, #7
	bls.n	.L_080ba9a2
	movs	r2, #160
	lsls	r2, r2, #7
.L_080ba9a2:
	movs	r3, #60
	str	r2, [r5, #0]
	str	r3, [r5, #4]
	b.n	.L_080baa28
.L_080ba9aa:
	ldrb	r0, [r7, #0]
	bl	sub_080b7dd0
	ldr	r3, [r0, #0]
	ldr	r1, [r3, #16]
	ldr	r0, [r3, #8]
	bl	sub_080044d0
	ldrb	r4, [r7, #0]
	lsls	r0, r0, #16
	ldr	r2, [pc, #516]
	lsrs	r0, r0, #16
	adds	r3, r4, #0
	adds	r1, r0, r2
	cmp	r3, #7
	bls.n	.L_080ba9d0
	movs	r3, #192
	lsls	r3, r3, #5
	adds	r1, r0, r3
.L_080ba9d0:
	lsls	r3, r1, #16
	asrs	r1, r3, #16
	adds	r3, r4, #0
	cmp	r3, #7
	bhi.n	.L_080ba9e0
	movs	r3, #128
	lsls	r3, r3, #6
	b.n	.L_080ba9e2
.L_080ba9e0:
	ldr	r3, [pc, #476]
.L_080ba9e2:
	subs	r3, r3, r1
	lsls	r2, r3, #1
	adds	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_080ba9ee
	adds	r2, #3
.L_080ba9ee:
	asrs	r3, r2, #2
	adds	r1, r1, r3
	ldrb	r3, [r7, #2]
	adds	r2, r4, #0
	cmp	r3, #7
	bhi.n	.L_080baa08
	movs	r3, #0
	cmp	r2, #7
	bhi.n	.L_080baa02
	movs	r3, #1
.L_080baa02:
	cmp	r3, #0
	bne.n	.L_080baa14
	b.n	.L_080baa20
.L_080baa08:
	movs	r3, #0
	cmp	r2, #7
	bls.n	.L_080baa10
	movs	r3, #1
.L_080baa10:
	cmp	r3, #0
	beq.n	.L_080baa20
.L_080baa14:
	movs	r1, #144
	adds	r3, r4, #0
	lsls	r1, r1, #6
	cmp	r3, #7
	bls.n	.L_080baa20
	ldr	r1, [pc, #424]
.L_080baa20:
	ldr	r3, [r5, #0]
	cmp	r3, r1
	beq.n	.L_080baa28
	str	r1, [r5, #0]
.L_080baa28:
	ldr	r3, [r7, #88]
	movs	r2, #128
	lsls	r2, r2, #12
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080baa46
	ldrb	r3, [r7, #0]
	ldr	r2, [pc, #392]
	cmp	r3, #7
	bls.n	.L_080baa40
	movs	r2, #128
	lsls	r2, r2, #6
.L_080baa40:
	movs	r3, #60
	str	r2, [r5, #0]
	str	r3, [r5, #4]
.L_080baa46:
	add	r5, sp, #4
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_080b9d34
	mov	r6, sl
	movs	r3, #1
	ands	r6, r3
	cmp	r6, #0
	beq.n	.L_080baa5c
	str	r3, [r5, #28]
.L_080baa5c:
	movs	r1, #0
	movs	r0, #0
	bl	sub_080c10e8
	ldr	r3, [pc, #356]
	ldr	r3, [r3, #0]
	adds	r3, #65
	ldrb	r0, [r3, #0]
	movs	r3, #2
	negs	r3, r3
	ands	r0, r3
	bl	sub_08015130
	ldr	r0, [r5, #8]
	bl	sub_080b7dd0
	ldr	r0, [r0, #0]
	movs	r1, #3
	mov	r8, r0
	bl	sub_08009080
	movs	r1, #16
	mov	r0, r8
	bl	sub_08009088
	movs	r0, #154
	bl	sub_080f9010
	movs	r3, #2
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080baaac
	ldr	r0, [r5, #8]
	ldr	r1, [r7, #80]
	movs	r2, #1
	movs	r3, #0
	bl	sub_080c1798
	b.n	.L_080baabc
.L_080baaac:
	cmp	r6, #0
	bne.n	.L_080baabc
	ldr	r0, [r5, #8]
	ldr	r1, [r7, #80]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080c1798
.L_080baabc:
	ldrb	r3, [r7, #2]
	cmp	r3, #7
	bhi.n	.L_080baac6
	movs	r3, #1
	b.n	.L_080baac8
.L_080baac6:
	movs	r3, #0
.L_080baac8:
	str	r3, [r5, #4]
	ldr	r3, [r5, #20]
	movs	r4, #0
	adds	r2, r5, #0
	cmp	r3, #0
	beq.n	.L_080bab1e
	movs	r6, #0
.L_080baad6:
	lsls	r3, r4, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	str	r4, [sp, #0]
	bl	sub_080b7dd0
	movs	r1, #0
	ldr	r0, [r0, #0]
	bl	sub_080b7f70
	adds	r3, r0, #0
	adds	r3, #39
	ldrb	r3, [r3, #0]
	subs	r3, #1
	movs	r1, #0
	ldr	r4, [sp, #0]
	cmp	r3, #0
	beq.n	.L_080bab12
	mov	ip, r3
	adds	r3, r6, r5
	adds	r2, r3, #0
	adds	r2, #52
	adds	r0, #40
.L_080bab04:
	ldmia	r0!, {r3}
	ldrb	r3, [r3, #5]
	adds	r1, #1
	strb	r3, [r2, #0]
	adds	r2, #1
	cmp	r1, ip
	bne.n	.L_080bab04
.L_080bab12:
	ldr	r3, [r5, #20]
	adds	r4, #1
	adds	r6, #4
	adds	r2, r5, #0
	cmp	r4, r3
	bne.n	.L_080baad6
.L_080bab1e:
	ldr	r3, [r7, #92]
	cmp	r3, #0
	beq.n	.L_080bab4c
	cmp	r3, #1
	bne.n	.L_080bab3a
	ldrb	r1, [r7, #0]
	movs	r0, #0
	bl	sub_080bbabc
	ldr	r1, [pc, #156]
	movs	r0, #4
	bl	sub_080bbabc
	b.n	.L_080bab42
.L_080bab3a:
	ldr	r1, [pc, #152]
	movs	r0, #4
	bl	sub_080bbabc
.L_080bab42:
	bl	sub_080bb938
	bl	sub_080c1a14
	b.n	.L_080babaa
.L_080bab4c:
	movs	r1, #200
	ldr	r0, [pc, #136]
	lsls	r1, r1, #4
	bl	sub_080041d8
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_080bab78
	ldr	r3, [r7, #88]
	movs	r2, #128
	lsls	r2, r2, #7
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080bab70
	adds	r0, r5, #0
	bl	sub_080c9008
	b.n	.L_080bab7c
.L_080bab70:
	adds	r0, r5, #0
	bl	sub_080c9018
	b.n	.L_080bab7c
.L_080bab78:
	bl	sub_080c1a14
.L_080bab7c:
	bl	sub_080be02c
	adds	r6, r5, #0
	mov	r0, r8
	movs	r1, #1
	bl	sub_08009080
	ldr	r3, [r6, #20]
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L_080babaa
	movs	r7, #36
.L_080bab94:
	ldrsh	r0, [r6, r7]
	str	r4, [sp, #0]
	bl	sub_080b8000
	adds	r5, r6, #0
	ldr	r4, [sp, #0]
	ldr	r3, [r5, #20]
	adds	r4, #1
	adds	r7, #2
	cmp	r4, r3
	bne.n	.L_080bab94
.L_080babaa:
	movs	r0, #0
	add	sp, #88
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f00
	.4byte 0xffffe000
	.4byte 0xffffe800
	.4byte 0xffffdc00
	.4byte 0x03001e74
	.4byte 0x00000856
	.4byte 0x00000855
	.4byte 0x080bd899
