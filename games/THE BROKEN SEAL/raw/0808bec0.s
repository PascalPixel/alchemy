.syntax unified
	.thumb
	.set sub_08077008, 0x08077008
	.set sub_080770c0, 0x080770c0
	.set sub_08077148, 0x08077148
	.set sub_0808b02c, 0x0808b02c
	.set sub_0808b048, 0x0808b048
	.set sub_0808bde0, 0x0808bde0
	.set sub_0808c2dc, 0x0808c2dc
	.set sub_0808c30c, 0x0808c30c
	.set sub_0808c3a4, 0x0808c3a4
	.set sub_08091858, 0x08091858
	.set sub_08093874, 0x08093874
	.set sub_080b50f8, 0x080b50f8
	.set sub_080f9010, 0x080f9010
	.global Overlay_0808bec0
Overlay_0808bec0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #44
	str	r2, [sp, #24]
	mov	r9, r3
	ldr	r3, [pc, #548]
	mov	fp, r0
	ldr	r0, [r3, #0]
	subs	r3, #76
	ldr	r3, [r3, #0]
	mov	sl, r1
	ldr	r7, [pc, #540]
	movs	r1, #250
	str	r3, [sp, #20]
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r3, [r3, #0]
	str	r3, [sp, #16]
	lsls	r3, r3, #2
	adds	r3, #20
	ldr	r3, [r0, r3]
	movs	r2, #0
	mov	r8, r0
	str	r3, [sp, #12]
	str	r2, [sp, #4]
	cmp	r0, #0
	bne.n	.L_0808bf02
	b.n	.L_0808c2ac
.L_0808bf02:
	bl	sub_08077148
	movs	r6, #0
	str	r0, [sp, #8]
	cmp	r6, r0
	bcs.n	.L_0808bf2c
	movs	r3, #252
	lsls	r3, r3, #1
	add	r5, sp, #28
	adds	r7, r7, r3
.L_0808bf16:
	ldrb	r0, [r7, #0]
	bl	sub_08077008
	ldrh	r3, [r0, #56]
	strh	r3, [r5, #0]
	ldr	r4, [sp, #8]
	adds	r6, #1
	adds	r7, #1
	adds	r5, #2
	cmp	r6, r4
	bcc.n	.L_0808bf16
.L_0808bf2c:
	movs	r3, #207
	lsls	r3, r3, #1
	add	r3, r8
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #3
	bne.n	.L_0808bf64
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L_0808bf44
	ldr	r3, [pc, #448]
	add	r3, sl
.L_0808bf44:
	asrs	r2, r3, #21
	movs	r1, #31
	mov	r3, r9
	ands	r2, r1
	cmp	r3, #0
	bge.n	.L_0808bf54
	ldr	r3, [pc, #432]
	add	r3, r9
.L_0808bf54:
	asrs	r3, r3, #21
	ands	r3, r1
	lsls	r3, r3, #5
	adds	r3, r2, r3
	ldr	r1, [pc, #424]
	lsls	r3, r3, #2
	adds	r0, r3, r1
	b.n	.L_0808bf9e
.L_0808bf64:
	mov	r2, fp
	cmp	r2, #2
	bhi.n	.L_0808bf7c
	lsls	r3, r2, #1
	add	r3, fp
	movs	r4, #152
	lsls	r3, r3, #4
	lsls	r4, r4, #1
	ldr	r1, [sp, #20]
	adds	r3, r3, r4
	ldr	r0, [r1, r3]
	b.n	.L_0808bf7e
.L_0808bf7c:
	ldr	r0, [pc, #396]
.L_0808bf7e:
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L_0808bf88
	ldr	r3, [pc, #392]
	add	r3, sl
.L_0808bf88:
	asrs	r2, r3, #20
	mov	r3, r9
	cmp	r3, #0
	bge.n	.L_0808bf94
	ldr	r3, [pc, #380]
	add	r3, r9
.L_0808bf94:
	asrs	r3, r3, #20
	lsls	r3, r3, #7
	adds	r3, r2, r3
	lsls	r3, r3, #2
	adds	r0, r0, r3
.L_0808bf9e:
	movs	r2, #220
	lsls	r2, r2, #1
	add	r2, r8
	movs	r1, #222
	ldr	r3, [r2, #0]
	lsls	r1, r1, #1
	ldrb	r6, [r0, #2]
	add	r1, r8
	str	r3, [r1, #0]
	str	r0, [r2, #0]
	cmp	r6, #0
	beq.n	.L_0808bfc0
	mov	r0, sl
	ldr	r1, [sp, #24]
	mov	r2, r9
	bl	sub_0808bde0
.L_0808bfc0:
	subs	r3, r6, #1
	cmp	r3, #238
	bhi.n	.L_0808bfce
	movs	r3, #182
	lsls	r3, r3, #1
	add	r3, r8
	strh	r6, [r3, #0]
.L_0808bfce:
	adds	r3, r6, #0
	subs	r3, #252
	cmp	r3, #2
	bhi.n	.L_0808bfde
	movs	r3, #183
	lsls	r3, r3, #1
	add	r3, r8
	strh	r6, [r3, #0]
.L_0808bfde:
	ldr	r4, [pc, #288]
	movs	r2, #249
	lsls	r2, r2, #1
	adds	r3, r4, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0808bfee
	b.n	.L_0808c17a
.L_0808bfee:
	ldr	r3, [sp, #12]
	cmp	r3, #0
	bne.n	.L_0808bff6
	b.n	.L_0808c17a
.L_0808bff6:
	movs	r0, #128
	ldr	r3, [r3, #56]
	lsls	r0, r0, #24
	cmp	r3, r0
	bne.n	.L_0808c002
	b.n	.L_0808c17a
.L_0808c002:
	ldr	r1, [sp, #12]
	ldr	r0, [pc, #268]
	ldr	r5, [r1, #48]
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_0808c012
	lsls	r5, r5, #1
.L_0808c012:
	movs	r3, #207
	lsls	r3, r3, #1
	add	r3, r8
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #3
	bne.n	.L_0808c034
	ldr	r0, [sp, #12]
	adds	r1, r5, #0
	adds	r0, #8
	bl	sub_0808b048
	movs	r3, #190
	lsls	r3, r3, #1
	add	r3, r8
	movs	r7, #1
	b.n	.L_0808c062
.L_0808c034:
	adds	r3, r6, #0
	subs	r3, #240
	cmp	r3, #1
	bhi.n	.L_0808c04c
	adds	r3, r6, #0
	adds	r0, r6, #0
	adds	r3, #177
	mov	r4, r8
	subs	r0, #239
	adds	r1, r5, #0
	ldrb	r7, [r4, r3]
	b.n	.L_0808c058
.L_0808c04c:
	movs	r3, #208
	lsls	r3, r3, #1
	add	r3, r8
	movs	r0, #0
	adds	r1, r5, #0
	ldrb	r7, [r3, #0]
.L_0808c058:
	bl	sub_0808b02c
	movs	r3, #190
	lsls	r3, r3, #1
	add	r3, r8
.L_0808c062:
	strh	r0, [r3, #0]
	movs	r3, #216
	ldr	r2, [sp, #12]
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r4, [pc, #168]
	ldr	r0, [r3, #0]
	ldr	r1, [r2, #48]
	movs	r0, r0
	mov	ip, pc
	bx	r4
	cmp	r7, #0
	bne.n	.L_0808c082
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r0, r3, #1
.L_0808c082:
	movs	r2, #218
	lsls	r2, r2, #1
	add	r2, r8
	ldr	r3, [r2, #0]
	adds	r0, r3, r0
	ldr	r3, [pc, #140]
	str	r0, [r2, #0]
	cmp	r0, r3
	ble.n	.L_0808c0c2
	adds	r1, r0, #0
	cmp	r0, #0
	bge.n	.L_0808c09c
	adds	r1, r0, r3
.L_0808c09c:
	ands	r0, r3
	asrs	r1, r1, #16
	str	r0, [r2, #0]
	adds	r0, r1, #0
	movs	r1, #0
	bl	sub_0808c2dc
	bl	sub_080b50f8
	cmp	r0, #0
	beq.n	.L_0808c0b8
	movs	r0, #139
	bl	sub_080f9010
.L_0808c0b8:
	bl	sub_08091858
	bl	sub_0808c3a4
	str	r0, [sp, #4]
.L_0808c0c2:
	ldr	r4, [pc, #60]
	ldr	r0, [pc, #88]
	adds	r3, r4, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_0808c140
	cmp	r6, #250
	bne.n	.L_0808c140
	movs	r3, #222
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r3, [r3, #0]
	ldrb	r3, [r3, #2]
	cmp	r3, #250
	bne.n	.L_0808c128
	ldr	r2, [pc, #64]
	ldr	r3, [sp, #12]
	adds	r1, r4, r2
	ldr	r2, [r3, #48]
	cmp	r2, #0
	bge.n	.L_0808c0f2
	ldr	r0, [pc, #44]
	adds	r2, r2, r0
.L_0808c0f2:
	ldrh	r3, [r1, #0]
	asrs	r2, r2, #16
	adds	r3, r3, r2
	strh	r3, [r1, #0]
	b.n	.L_0808c140
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x001fffff
	.4byte 0x02020000
	.4byte 0x02010000
	.4byte 0x000fffff
	.4byte 0x00000167
	.4byte 0x03000118
	.4byte 0x0000ffff
	.4byte 0x0000022e
	.2byte 0x0232
	.2byte 0x0000
.L_0808c128:
	movs	r1, #139
	lsls	r1, r1, #2
	adds	r3, r4, r1
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	ldr	r0, [pc, #392]
	lsrs	r3, r3, #31
	adds	r2, r2, r3
	asrs	r2, r2, #1
	adds	r3, r4, r0
	strh	r2, [r3, #0]
.L_0808c140:
	movs	r2, #145
	lsls	r2, r2, #2
	adds	r1, r4, r2
	ldr	r2, [r1, #0]
	cmp	r2, #0
	beq.n	.L_0808c17a
	ldr	r0, [pc, #372]
	adds	r3, r4, r0
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #2
	beq.n	.L_0808c17a
	ldr	r0, [sp, #12]
	ldr	r3, [r0, #48]
	subs	r3, r2, r3
	str	r3, [r1, #0]
	cmp	r3, #0
	bgt.n	.L_0808c17a
	movs	r3, #1
	movs	r2, #191
	str	r3, [r1, #0]
	lsls	r2, r2, #1
	add	r2, r8
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	cmp	r3, #0
	bne.n	.L_0808c17a
	ldr	r3, [pc, #336]
	strh	r3, [r2, #0]
.L_0808c17a:
	ldr	r2, [pc, #336]
	adds	r3, r4, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #1
	bne.n	.L_0808c1d0
	ldr	r1, [pc, #312]
	adds	r5, r4, r1
	ldrh	r3, [r5, #0]
	adds	r3, #1
	strh	r3, [r5, #0]
	subs	r2, #2
	adds	r6, r4, r2
	ldrh	r0, [r6, #0]
	lsls	r2, r0, #16
	asrs	r1, r2, #16
	lsrs	r2, r2, #31
	adds	r1, r1, r2
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	asrs	r1, r1, #1
	cmp	r3, r1
	bne.n	.L_0808c1b6
	ldr	r0, [sp, #16]
	ldr	r1, [pc, #292]
	str	r4, [sp, #0]
	bl	sub_08093874
	ldrh	r0, [r6, #0]
	ldr	r4, [sp, #0]
.L_0808c1b6:
	movs	r3, #0
	ldrsh	r2, [r5, r3]
	lsls	r3, r0, #16
	asrs	r3, r3, #16
	cmp	r2, r3
	bne.n	.L_0808c1d0
	movs	r1, #128
	lsls	r1, r1, #1
	ldr	r0, [sp, #16]
	str	r4, [sp, #0]
	bl	sub_08093874
	ldr	r4, [sp, #0]
.L_0808c1d0:
	ldr	r1, [pc, #236]
	movs	r2, #139
	lsls	r2, r2, #2
	adds	r0, r4, r1
	adds	r3, r4, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	movs	r1, #0
	ldrsh	r2, [r0, r1]
	cmp	r2, r3
	blt.n	.L_0808c20e
	movs	r2, #140
	lsls	r2, r2, #2
	adds	r3, r4, r2
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	movs	r3, #0
	strh	r3, [r0, #0]
	movs	r0, #255
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r0, r1
	negs	r0, r0
	ands	r1, r3
	str	r4, [sp, #0]
	bl	sub_0808c30c
	ldr	r3, [sp, #4]
	adds	r3, #1
	str	r3, [sp, #4]
	ldr	r4, [sp, #0]
.L_0808c20e:
	ldr	r0, [sp, #4]
	cmp	r0, #0
	beq.n	.L_0808c2ac
	movs	r3, #194
	lsls	r3, r3, #1
	movs	r2, #0
	add	r3, r8
	strh	r2, [r3, #0]
	movs	r3, #195
	lsls	r3, r3, #1
	add	r3, r8
	strh	r2, [r3, #0]
	ldr	r1, [sp, #12]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r1, #40]
	movs	r1, #129
	lsls	r1, r1, #1
	ldr	r0, [sp, #16]
	str	r4, [sp, #0]
	bl	sub_08093874
	ldr	r2, [sp, #8]
	movs	r6, #0
	ldr	r4, [sp, #0]
	cmp	r6, r2
	bcs.n	.L_0808c2ac
	movs	r3, #193
	lsls	r3, r3, #1
	movs	r7, #195
	movs	r0, #252
	add	r3, r8
	lsls	r7, r7, #1
	lsls	r0, r0, #1
	mov	sl, r3
	add	r7, r8
	adds	r5, r4, r0
.L_0808c258:
	ldrb	r0, [r5, #0]
	bl	sub_08077008
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	cmp	r3, #0
	ble.n	.L_0808c26e
	ldrh	r3, [r7, #0]
	adds	r3, #1
	strh	r3, [r7, #0]
	b.n	.L_0808c2a2
.L_0808c26e:
	add	r3, sp, #28
	lsls	r2, r6, #1
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_0808c2a2
	movs	r1, #194
	lsls	r1, r1, #1
	add	r1, r8
	ldrh	r3, [r1, #0]
	adds	r2, r3, #1
	strh	r2, [r1, #0]
	lsls	r3, r3, #16
	movs	r1, #196
	lsls	r1, r1, #1
	ldrb	r2, [r5, #0]
	asrs	r3, r3, #15
	adds	r3, r3, r1
	mov	r4, r8
	strh	r2, [r4, r3]
	ldr	r3, [pc, #60]
	mov	r1, sl
	strh	r3, [r1, #0]
	ldr	r3, [pc, #60]
	adds	r2, r0, r3
	movs	r3, #0
	strb	r3, [r2, #0]
.L_0808c2a2:
	ldr	r4, [sp, #8]
	adds	r6, #1
	adds	r5, #1
	cmp	r6, r4
	bcc.n	.L_0808c258
.L_0808c2ac:
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00000232
	.4byte 0x0000023e
	.4byte 0x00002096
	.4byte 0x0000022e
	.4byte 0x00000101
	.4byte 0x0000ffff
	.4byte 0x00000131
