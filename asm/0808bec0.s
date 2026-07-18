@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808bec0
	.thumb_func
Func_0808bec0:
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
	bne.n	.L0
	b.n	.L1
.L0:
	bl	Func_08077148
	movs	r6, #0
	str	r0, [sp, #8]
	cmp	r6, r0
	bcs.n	.L2
	movs	r3, #252
	lsls	r3, r3, #1
	add	r5, sp, #28
	adds	r7, r7, r3
.L3:
	ldrb	r0, [r7, #0]
	bl	Func_08077008
	ldrh	r3, [r0, #56]
	strh	r3, [r5, #0]
	ldr	r4, [sp, #8]
	adds	r6, #1
	adds	r7, #1
	adds	r5, #2
	cmp	r6, r4
	bcc.n	.L3
.L2:
	movs	r3, #207
	lsls	r3, r3, #1
	add	r3, r8
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #3
	bne.n	.L4
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L5
	ldr	r3, [pc, #448]
	add	r3, sl
.L5:
	asrs	r2, r3, #21
	movs	r1, #31
	mov	r3, r9
	ands	r2, r1
	cmp	r3, #0
	bge.n	.L6
	ldr	r3, [pc, #432]
	add	r3, r9
.L6:
	asrs	r3, r3, #21
	ands	r3, r1
	lsls	r3, r3, #5
	adds	r3, r2, r3
	ldr	r1, [pc, #424]
	lsls	r3, r3, #2
	adds	r0, r3, r1
	b.n	.L7
.L4:
	mov	r2, fp
	cmp	r2, #2
	bhi.n	.L8
	lsls	r3, r2, #1
	add	r3, fp
	movs	r4, #152
	lsls	r3, r3, #4
	lsls	r4, r4, #1
	ldr	r1, [sp, #20]
	adds	r3, r3, r4
	ldr	r0, [r1, r3]
	b.n	.L9
.L8:
	ldr	r0, [pc, #396]
.L9:
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L10
	ldr	r3, [pc, #392]
	add	r3, sl
.L10:
	asrs	r2, r3, #20
	mov	r3, r9
	cmp	r3, #0
	bge.n	.L11
	ldr	r3, [pc, #380]
	add	r3, r9
.L11:
	asrs	r3, r3, #20
	lsls	r3, r3, #7
	adds	r3, r2, r3
	lsls	r3, r3, #2
	adds	r0, r0, r3
.L7:
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
	beq.n	.L12
	mov	r0, sl
	ldr	r1, [sp, #24]
	mov	r2, r9
	bl	Func_0808bde0
.L12:
	subs	r3, r6, #1
	cmp	r3, #238
	bhi.n	.L13
	movs	r3, #182
	lsls	r3, r3, #1
	add	r3, r8
	strh	r6, [r3, #0]
.L13:
	adds	r3, r6, #0
	subs	r3, #252
	cmp	r3, #2
	bhi.n	.L14
	movs	r3, #183
	lsls	r3, r3, #1
	add	r3, r8
	strh	r6, [r3, #0]
.L14:
	ldr	r4, [pc, #288]
	movs	r2, #249
	lsls	r2, r2, #1
	adds	r3, r4, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L15
	b.n	.L16
.L15:
	ldr	r3, [sp, #12]
	cmp	r3, #0
	bne.n	.L17
	b.n	.L16
.L17:
	movs	r0, #128
	ldr	r3, [r3, #56]
	lsls	r0, r0, #24
	cmp	r3, r0
	bne.n	.L18
	b.n	.L16
.L18:
	ldr	r1, [sp, #12]
	ldr	r0, [pc, #268]
	ldr	r5, [r1, #48]
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L19
	lsls	r5, r5, #1
.L19:
	movs	r3, #207
	lsls	r3, r3, #1
	add	r3, r8
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #3
	bne.n	.L20
	ldr	r0, [sp, #12]
	adds	r1, r5, #0
	adds	r0, #8
	bl	Func_0808b048
	movs	r3, #190
	lsls	r3, r3, #1
	add	r3, r8
	movs	r7, #1
	b.n	.L21
.L20:
	adds	r3, r6, #0
	subs	r3, #240
	cmp	r3, #1
	bhi.n	.L22
	adds	r3, r6, #0
	adds	r0, r6, #0
	adds	r3, #177
	mov	r4, r8
	subs	r0, #239
	adds	r1, r5, #0
	ldrb	r7, [r4, r3]
	b.n	.L23
.L22:
	movs	r3, #208
	lsls	r3, r3, #1
	add	r3, r8
	movs	r0, #0
	adds	r1, r5, #0
	ldrb	r7, [r3, #0]
.L23:
	bl	Func_0808b02c
	movs	r3, #190
	lsls	r3, r3, #1
	add	r3, r8
.L21:
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
	bne.n	.L24
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r0, r3, #1
.L24:
	movs	r2, #218
	lsls	r2, r2, #1
	add	r2, r8
	ldr	r3, [r2, #0]
	adds	r0, r3, r0
	ldr	r3, [pc, #140]
	str	r0, [r2, #0]
	cmp	r0, r3
	ble.n	.L25
	adds	r1, r0, #0
	cmp	r0, #0
	bge.n	.L26
	adds	r1, r0, r3
.L26:
	ands	r0, r3
	asrs	r1, r1, #16
	str	r0, [r2, #0]
	adds	r0, r1, #0
	movs	r1, #0
	bl	Func_0808c2dc
	bl	Func_080b50f8
	cmp	r0, #0
	beq.n	.L27
	movs	r0, #139
	bl	Func_080f9010
.L27:
	bl	Func_08091858
	bl	Func_0808c3a4
	str	r0, [sp, #4]
.L25:
	ldr	r4, [pc, #60]
	ldr	r0, [pc, #88]
	adds	r3, r4, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L28
	cmp	r6, #250
	bne.n	.L28
	movs	r3, #222
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r3, [r3, #0]
	ldrb	r3, [r3, #2]
	cmp	r3, #250
	bne.n	.L29
	ldr	r2, [pc, #64]
	ldr	r3, [sp, #12]
	adds	r1, r4, r2
	ldr	r2, [r3, #48]
	cmp	r2, #0
	bge.n	.L30
	ldr	r0, [pc, #44]
	adds	r2, r2, r0
.L30:
	ldrh	r3, [r1, #0]
	asrs	r2, r2, #16
	adds	r3, r3, r2
	strh	r3, [r1, #0]
	b.n	.L28
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
	.4byte 0x00000232
.L29:
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
.L28:
	movs	r2, #145
	lsls	r2, r2, #2
	adds	r1, r4, r2
	ldr	r2, [r1, #0]
	cmp	r2, #0
	beq.n	.L16
	ldr	r0, [pc, #372]
	adds	r3, r4, r0
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #2
	beq.n	.L16
	ldr	r0, [sp, #12]
	ldr	r3, [r0, #48]
	subs	r3, r2, r3
	str	r3, [r1, #0]
	cmp	r3, #0
	bgt.n	.L16
	movs	r3, #1
	movs	r2, #191
	str	r3, [r1, #0]
	lsls	r2, r2, #1
	add	r2, r8
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	cmp	r3, #0
	bne.n	.L16
	ldr	r3, [pc, #336]
	strh	r3, [r2, #0]
.L16:
	ldr	r2, [pc, #336]
	adds	r3, r4, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #1
	bne.n	.L31
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
	bne.n	.L32
	ldr	r0, [sp, #16]
	ldr	r1, [pc, #292]
	str	r4, [sp, #0]
	bl	Func_08093874
	ldrh	r0, [r6, #0]
	ldr	r4, [sp, #0]
.L32:
	movs	r3, #0
	ldrsh	r2, [r5, r3]
	lsls	r3, r0, #16
	asrs	r3, r3, #16
	cmp	r2, r3
	bne.n	.L31
	movs	r1, #128
	lsls	r1, r1, #1
	ldr	r0, [sp, #16]
	str	r4, [sp, #0]
	bl	Func_08093874
	ldr	r4, [sp, #0]
.L31:
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
	blt.n	.L33
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
	bl	Func_0808c30c
	ldr	r3, [sp, #4]
	adds	r3, #1
	str	r3, [sp, #4]
	ldr	r4, [sp, #0]
.L33:
	ldr	r0, [sp, #4]
	cmp	r0, #0
	beq.n	.L1
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
	bl	Func_08093874
	ldr	r2, [sp, #8]
	movs	r6, #0
	ldr	r4, [sp, #0]
	cmp	r6, r2
	bcs.n	.L1
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
.L36:
	ldrb	r0, [r5, #0]
	bl	Func_08077008
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	cmp	r3, #0
	ble.n	.L34
	ldrh	r3, [r7, #0]
	adds	r3, #1
	strh	r3, [r7, #0]
	b.n	.L35
.L34:
	add	r3, sp, #28
	lsls	r2, r6, #1
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L35
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
.L35:
	ldr	r4, [sp, #8]
	adds	r6, #1
	adds	r5, #1
	cmp	r6, r4
	bcc.n	.L36
.L1:
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
