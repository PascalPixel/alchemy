.syntax unified
	.thumb
	.set sub_08004458, 0x08004458
	.set sub_080072f0, 0x080072f0
	.set sub_08077008, 0x08077008
	.set sub_080770c0, 0x080770c0
	.set sub_080772c8, 0x080772c8
	.set sub_0808b320, 0x0808b320
	.global Func_0808ae74
	.thumb_func
Func_0808ae74:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #8
	str	r0, [sp, #4]
	str	r1, [sp, #0]
	ldr	r3, [pc, #388]
	ldr	r3, [r3, #0]
	ldr	r0, [pc, #388]
	mov	fp, r3
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_0808ae9a
	b.n	.L_0808afa0
.L_0808ae9a:
	movs	r0, #176
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_0808af26
	ldr	r0, [pc, #368]
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_0808af26
	ldr	r2, [sp, #4]
	movs	r0, #0
	cmp	r2, #0
	bne.n	.L_0808aeba
	b.n	.L_0808affc
.L_0808aeba:
	ldr	r3, [pc, #352]
	movs	r2, #147
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_0808aecc
	b.n	.L_0808affc
.L_0808aecc:
	ldr	r2, [sp, #4]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldr	r2, [pc, #332]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r7, [r3, #0]
	mov	r9, r3
	cmp	r7, #0
	bne.n	.L_0808aee2
	b.n	.L_0808affc
.L_0808aee2:
	movs	r0, #5
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_0808aefe
	movs	r0, #5
	bl	sub_08077008
	movs	r3, #146
	lsls	r3, r3, #1
	adds	r0, r0, r3
	ldr	r0, [r0, #0]
	cmp	r0, #130
	bgt.n	.L_0808af26
.L_0808aefe:
	bl	sub_080772c8
	mov	r2, r9
	ldrh	r3, [r2, #2]
	subs	r0, r0, r3
	cmp	r0, #0
	bge.n	.L_0808af0e
	movs	r0, #0
.L_0808af0e:
	cmp	r0, #5
	ble.n	.L_0808af14
	movs	r0, #5
.L_0808af14:
	cmp	r0, #0
	ble.n	.L_0808af2a
	ldr	r3, [pc, #256]
	movs	r2, #145
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0808af2a
.L_0808af26:
	movs	r0, #0
	b.n	.L_0808affc
.L_0808af2a:
	lsls	r3, r0, #2
	adds	r3, r3, r0
	adds	r7, r7, r3
	movs	r3, #212
	lsls	r3, r3, #1
	add	r3, fp
	ldr	r5, [r3, #0]
	mov	sl, r3
	cmp	r5, #0
	bne.n	.L_0808af66
	bl	sub_08004458
	adds	r5, r0, #0
	bl	sub_08004458
	mov	r8, r0
	bl	sub_08004458
	adds	r6, r0, #0
	bl	sub_08004458
	mov	r2, r8
	subs	r5, r5, r2
	adds	r5, r5, r6
	subs	r5, r5, r0
	lsrs	r3, r5, #31
	adds	r5, r5, r3
	asrs	r5, r5, #1
	mov	r3, sl
	str	r5, [r3, #0]
.L_0808af66:
	lsls	r3, r7, #4
	subs	r3, #16
	muls	r3, r5
	lsls	r0, r7, #20
	movs	r1, #128
	adds	r0, r0, r3
	lsls	r1, r1, #13
	ldr	r3, [pc, #172]
	bl	sub_080072f0
	ldr	r3, [pc, #172]
	ldr	r1, [sp, #0]
	movs	r0, r0
	mov	ip, pc
	bx	r3
	ldr	r3, [pc, #148]
	movs	r2, #142
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r2, [r3, #0]
	adds	r2, r2, r0
	str	r2, [r3, #0]
	movs	r3, #214
	lsls	r3, r3, #1
	add	r3, fp
	ldr	r3, [r3, #0]
	movs	r0, #0
	cmp	r2, r3
	blt.n	.L_0808affc
.L_0808afa0:
	movs	r2, #212
	lsls	r2, r2, #1
	add	r2, fp
	movs	r3, #0
	str	r3, [r2, #0]
	mov	r2, r9
	movs	r5, #0
	adds	r2, #20
	movs	r1, #7
.L_0808afb2:
	ldrb	r3, [r2, #0]
	subs	r1, #1
	adds	r2, #1
	adds	r5, r5, r3
	cmp	r1, #0
	bge.n	.L_0808afb2
	movs	r0, #0
	cmp	r5, #0
	beq.n	.L_0808affc
	bl	sub_08004458
	adds	r3, r5, #0
	muls	r3, r0
	mov	r2, r9
	lsrs	r0, r3, #16
	ldrb	r3, [r2, #20]
	subs	r0, r0, r3
	movs	r1, #0
	cmp	r0, #0
	blt.n	.L_0808afec
	adds	r2, #20
.L_0808afdc:
	adds	r1, #1
	cmp	r1, #7
	bgt.n	.L_0808afec
	adds	r2, #1
	ldrb	r3, [r2, #0]
	subs	r0, r0, r3
	cmp	r0, #0
	bge.n	.L_0808afdc
.L_0808afec:
	lsls	r3, r1, #1
	adds	r3, #4
	mov	r2, r9
	ldrh	r5, [r2, r3]
	ldr	r0, [sp, #4]
	bl	sub_0808b320
	adds	r0, r5, #0
.L_0808affc:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001ebc
	.4byte 0x0000015f
	.4byte 0x00000161
	.4byte 0x02000240
	.4byte 0x0809c610
	.4byte 0x0300013c
	.4byte 0x03000118
