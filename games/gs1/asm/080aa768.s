@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080aa768
	.thumb_func
Func_080aa768:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #792]
	ldr	r7, [r3, #0]
	movs	r4, #0
	ldr	r2, [r7, #20]
	mov	sl, r4
	movs	r3, #13
	strb	r3, [r2, #5]
	mov	r3, sl
	sub	sp, #8
	movs	r1, #0
	strh	r3, [r2, #12]
	str	r4, [sp, #0]
	mov	r8, r1
	bl	Func_080aad10
	movs	r0, #1
	bl	Func_080030f8
	ldr	r4, [sp, #0]
	movs	r5, #2
.L45:
	cmp	r5, #15
	bls.n	.L0
	b.n	.L1
.L0:
	ldr	r2, [pc, #752]
	lsls	r3, r5, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080aa7e8
	.4byte 0x080aac56
	.4byte 0x080aa7fa
	.4byte 0x080aa8b0
	.4byte 0x080aabc0
	.4byte 0x080aabec
	.4byte 0x080aaaa4
	.4byte 0x080aa9be
	.4byte 0x080aa880
	.4byte 0x080aaaec
	.4byte 0x080aa83c
	.4byte 0x080aac06
	.4byte 0x080aaac0
	.4byte 0x080aa9da
	.4byte 0x080aab08
	.4byte 0x080aa86a
	cmp	r4, #0
	blt.n	.L2
	b.n	.L3
.L2:
	movs	r1, #1
	negs	r1, r1
	movs	r2, #1
	mov	sl, r1
	mov	r8, r2
	b.n	.L3
	movs	r0, #0
	bl	Func_080aa544
	movs	r1, #0
	movs	r2, #200
	movs	r3, #0
	movs	r0, #1
	bl	Func_080ad5b4
	movs	r0, #0
	bl	Func_080ab5e4
	adds	r4, r0, #0
	movs	r5, #15
	cmp	r4, #10
	bne.n	.L4
	b.n	.L5
.L4:
	movs	r5, #0
	cmp	r4, #0
	bge.n	.L6
	b.n	.L5
.L6:
	movs	r1, #187
	movs	r3, #28
	ldrsb	r3, [r7, r3]
	lsls	r1, r1, #1
	adds	r2, r7, r1
	strh	r3, [r2, #0]
	movs	r5, #10
	cmp	r4, #7
	bne.n	.L7
	b.n	.L5
.L7:
	movs	r5, #3
	b.n	.L5
	movs	r3, #28
	ldrsb	r3, [r7, r3]
	movs	r2, #130
	lsls	r2, r2, #2
	lsls	r3, r3, #1
	adds	r3, r3, r2
	ldrh	r2, [r7, r3]
	ldr	r1, [pc, #584]
	str	r2, [r7, #8]
	ldrh	r2, [r7, r3]
	adds	r3, r7, r1
	strb	r2, [r3, #0]
	bl	Func_080ae2f4
	movs	r2, #2
	adds	r4, r0, #0
	negs	r2, r2
	cmp	r4, r2
	beq.n	.L8
	b.n	.L3
.L8:
	movs	r3, #1
	mov	r8, r3
	b.n	.L3
	bl	Func_080ab314
	movs	r1, #2
	adds	r4, r0, #0
	negs	r1, r1
	cmp	r4, r1
	beq.n	.L9
	b.n	.L3
.L9:
	movs	r2, #1
	mov	r8, r2
	b.n	.L3
	movs	r1, #134
	lsls	r1, r1, #2
	adds	r3, r7, r1
	ldrb	r3, [r3, #0]
	movs	r5, #0
	cmp	r3, #0
	bne.n	.L10
	b.n	.L5
.L10:
	movs	r0, #1
	bl	Func_080ab5e4
	movs	r2, #2
	adds	r4, r0, #0
	negs	r2, r2
	cmp	r4, r2
	bne.n	.L11
	movs	r3, #1
	mov	r8, r3
.L11:
	movs	r5, #4
	cmp	r4, #0
	bge.n	.L12
	b.n	.L5
.L12:
	movs	r5, #9
	b.n	.L5
	movs	r1, #194
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r0, [r3, #0]
	bl	Func_080aafb8
	movs	r0, #8
	negs	r0, r0
	bl	Func_080aa544
	movs	r3, #28
	ldrsb	r3, [r7, r3]
	movs	r2, #130
	lsls	r2, r2, #2
	lsls	r3, r3, #1
	adds	r3, r3, r2
	ldrh	r2, [r7, r3]
	ldr	r1, [pc, #448]
	str	r2, [r7, #8]
	ldrh	r2, [r7, r3]
	adds	r3, r7, r1
	strb	r2, [r3, #0]
	movs	r3, #28
	ldrsb	r3, [r7, r3]
	lsls	r1, r3, #3
	subs	r1, r1, r3
	lsls	r1, r1, #3
	adds	r1, #48
	movs	r2, #54
	movs	r3, #0
	movs	r0, #0
	bl	Func_080ad5b4
	movs	r0, #1
	bl	Func_080ab5e4
	ldr	r2, [pc, #412]
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	movs	r1, #0
	adds	r4, r0, #0
	cmp	r1, r3
	bge.n	.L13
	adds	r0, r7, r2
	subs	r2, #213
.L14:
	ldrh	r3, [r2, r7]
	adds	r3, #8
	strh	r3, [r2, r7]
	ldrb	r3, [r0, #0]
	adds	r1, #1
	adds	r2, #2
	cmp	r1, r3
	blt.n	.L14
.L13:
	movs	r3, #2
	negs	r3, r3
	cmp	r4, r3
	bne.n	.L15
	movs	r1, #1
	mov	r8, r1
.L15:
	cmp	r4, #0
	bge.n	.L16
	b.n	.L3
.L16:
	subs	r3, r4, #3
	cmp	r3, #1
	bls.n	.L17
	cmp	r4, #8
	beq.n	.L17
	cmp	r4, #9
	bne.n	.L18
.L17:
	movs	r3, #29
	ldrsb	r3, [r7, r3]
	movs	r2, #130
	lsls	r2, r2, #2
	lsls	r3, r3, #1
	adds	r3, r3, r2
	ldr	r1, [pc, #340]
	ldrh	r2, [r7, r3]
	adds	r3, r7, r1
	strb	r2, [r3, #0]
.L18:
	cmp	r4, #0
	bge.n	.L19
	b.n	.L3
.L19:
	cmp	r4, #1
	bne.n	.L20
	movs	r5, #5
	b.n	.L5
.L20:
	cmp	r4, #2
	bne.n	.L21
	movs	r5, #6
	b.n	.L5
.L21:
	cmp	r4, #3
	bne.n	.L22
	movs	r3, #136
	lsls	r3, r3, #2
	adds	r2, r7, r3
	movs	r3, #2
	strh	r3, [r2, #0]
	movs	r5, #7
	b.n	.L5
.L22:
	cmp	r4, #4
	bne.n	.L23
	movs	r1, #136
	lsls	r1, r1, #2
	adds	r2, r7, r1
	movs	r3, #2
	strh	r3, [r2, #0]
	movs	r5, #9
	b.n	.L5
.L23:
	cmp	r4, #5
	bne.n	.L24
	movs	r5, #11
	b.n	.L5
.L24:
	cmp	r4, #6
	bne.n	.L25
	movs	r5, #12
	b.n	.L5
.L25:
	cmp	r4, #8
	bne.n	.L26
	movs	r3, #136
	lsls	r3, r3, #2
	adds	r2, r7, r3
	movs	r3, #2
	strh	r3, [r2, #0]
	movs	r5, #13
	b.n	.L5
.L26:
	cmp	r4, #9
	beq.n	.L27
	b.n	.L5
.L27:
	movs	r1, #136
	lsls	r1, r1, #2
	adds	r2, r7, r1
	movs	r3, #2
	strh	r3, [r2, #0]
	movs	r5, #14
	b.n	.L5
	movs	r0, #1
	bl	Func_080ad6d4
	movs	r2, #2
	adds	r4, r0, #0
	negs	r2, r2
	cmp	r4, r2
	bne.n	.L28
	movs	r3, #1
	mov	r8, r3
.L28:
	movs	r5, #3
	cmp	r4, #0
	bge.n	.L29
	b.n	.L5
.L29:
	movs	r0, #126
	bl	Func_080f9010
	ldr	r2, [pc, #188]
	ldr	r1, [pc, #176]
	adds	r3, r7, r2
	subs	r2, #2
	adds	r6, r7, r1
	ldrb	r1, [r3, #0]
	adds	r3, r7, r2
	ldrb	r2, [r3, #0]
	ldr	r3, [pc, #168]
	adds	r5, r7, r3
	ldrb	r3, [r5, #0]
	ldrb	r0, [r6, #0]
	bl	Func_080771d0
	adds	r4, r0, #0
	ldrb	r0, [r6, #0]
	str	r4, [sp, #0]
	bl	Func_08077010
	ldrb	r0, [r5, #0]
	bl	Func_08077010
	ldr	r2, [r7, #20]
	movs	r3, #13
	strb	r3, [r2, #5]
	ldr	r0, [r7, #48]
	bl	Func_08015278
	movs	r1, #194
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r0, [r3, #0]
	bl	Func_080aaf58
	movs	r2, #187
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldrh	r0, [r3, #0]
	movs	r1, #10
	bl	Func_08002304
	movs	r1, #188
	movs	r3, #0
	lsls	r1, r1, #1
	mov	ip, r3
	adds	r3, r7, r1
	lsls	r0, r0, #16
	ldrb	r6, [r3, #0]
	lsrs	r0, r0, #16
	movs	r1, #0
	adds	r5, r0, #0
	adds	r5, #160
	ldr	r4, [sp, #0]
	b.n	.L30
.L32:
	adds	r1, #1
.L30:
	movs	r2, #194
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r2, [r3, #0]
	ldrsb	r3, [r2, r5]
	cmp	r1, r3
	bge.n	.L31
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #1
	adds	r3, r3, r1
	lsls	r3, r3, #1
	ldrb	r3, [r2, r3]
	cmp	r6, r3
	bne.n	.L32
	mov	ip, r1
.L31:
	mov	r1, ip
	lsls	r3, r1, #2
	add	r3, ip
	movs	r1, #186
	lsls	r1, r1, #1
	lsls	r3, r3, #1
	adds	r3, r0, r3
	adds	r2, r7, r1
	strh	r3, [r2, #0]
	ldr	r2, [r7, #20]
	movs	r3, #1
	strb	r3, [r2, #5]
	movs	r5, #0
	b.n	.L5
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x080aa7a8
	.4byte 0x0000021a
	.4byte 0x00000219
	.4byte 0x0000021b
	.4byte 0x00000256
	movs	r0, #2
	bl	Func_080ad6d4
	movs	r2, #2
	adds	r4, r0, #0
	negs	r2, r2
	cmp	r4, r2
	bne.n	.L33
	movs	r3, #1
	mov	r8, r3
.L33:
	movs	r5, #3
	cmp	r4, #0
	bge.n	.L34
	b.n	.L5
.L34:
	movs	r0, #175
	bl	Func_080f9010
	ldr	r2, [pc, #424]
	ldr	r1, [pc, #424]
	adds	r3, r7, r2
	subs	r2, #2
	adds	r6, r7, r1
	adds	r5, r7, r2
	ldrb	r1, [r3, #0]
	ldrb	r2, [r5, #0]
	ldrb	r0, [r6, #0]
	str	r3, [sp, #4]
	bl	Func_080771b8
	ldr	r3, [sp, #4]
	ldrb	r2, [r5, #0]
	ldrb	r1, [r3, #0]
	ldrb	r0, [r6, #0]
	bl	Func_080771c8
	b.n	.L35
	movs	r0, #0
	bl	Func_080ad6d4
	movs	r3, #2
	adds	r4, r0, #0
	negs	r3, r3
	cmp	r4, r3
	bne.n	.L36
	movs	r1, #1
	mov	r8, r1
.L36:
	movs	r5, #3
	cmp	r4, #0
	bge.n	.L37
	b.n	.L5
.L37:
	movs	r0, #126
	bl	Func_080f9010
	ldr	r2, [pc, #356]
	ldr	r1, [pc, #348]
	adds	r6, r7, r2
	adds	r3, r7, r1
	adds	r2, #58
	ldrb	r1, [r3, #0]
	adds	r3, r7, r2
	ldrb	r2, [r3, #0]
	ldr	r3, [pc, #344]
	adds	r5, r7, r3
	ldrb	r3, [r5, #0]
	ldrb	r0, [r6, #0]
	bl	Func_080771d0
	ldr	r1, [pc, #336]
	ldr	r2, [pc, #336]
	adds	r3, r7, r1
	ldrb	r1, [r3, #0]
	adds	r3, r7, r2
	ldrb	r2, [r3, #0]
	ldrb	r0, [r5, #0]
	ldrb	r3, [r6, #0]
	bl	Func_080771d0
	adds	r4, r0, #0
	ldrb	r0, [r6, #0]
	str	r4, [sp, #0]
	bl	Func_08077010
	ldrb	r0, [r5, #0]
	bl	Func_08077010
	movs	r1, #194
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r0, [r3, #0]
	bl	Func_080aaf58
	movs	r2, #187
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldrh	r0, [r3, #0]
	movs	r1, #10
	bl	Func_08002304
	movs	r1, #188
	movs	r3, #0
	lsls	r1, r1, #1
	mov	ip, r3
	adds	r3, r7, r1
	lsls	r0, r0, #16
	ldrb	r6, [r3, #0]
	lsrs	r0, r0, #16
	movs	r1, #0
	adds	r5, r0, #0
	adds	r5, #160
	ldr	r4, [sp, #0]
	b.n	.L38
.L40:
	adds	r1, #1
.L38:
	movs	r2, #194
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r2, [r3, #0]
	ldrsb	r3, [r2, r5]
	cmp	r1, r3
	bge.n	.L39
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #1
	adds	r3, r3, r1
	lsls	r3, r3, #1
	ldrb	r3, [r2, r3]
	cmp	r6, r3
	bne.n	.L40
	mov	ip, r1
.L39:
	mov	r1, ip
	lsls	r3, r1, #2
	add	r3, ip
	movs	r1, #186
	lsls	r1, r1, #1
	lsls	r3, r3, #1
	adds	r3, r0, r3
	adds	r2, r7, r1
	strh	r3, [r2, #0]
	ldr	r2, [r7, #20]
	movs	r3, #1
	strb	r3, [r2, #5]
.L3:
	movs	r5, #2
	b.n	.L5
	movs	r2, #1
	negs	r2, r2
	cmp	r4, r2
	bne.n	.L41
	mov	sl, r4
	b.n	.L3
.L41:
	movs	r1, #136
	lsls	r1, r1, #2
	adds	r3, r7, r1
	ldrh	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L42
	movs	r5, #8
	b.n	.L5
.L42:
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L5
	movs	r5, #7
	b.n	.L5
	movs	r0, #3
	bl	Func_080ad6d4
	movs	r2, #2
	adds	r4, r0, #0
	negs	r2, r2
	cmp	r4, r2
	bne.n	.L43
	movs	r3, #1
	mov	r8, r3
.L43:
	movs	r5, #3
	cmp	r4, #0
	blt.n	.L5
	movs	r0, #139
	bl	Func_080f9010
	ldr	r2, [pc, #96]
	ldr	r1, [pc, #100]
	adds	r3, r7, r2
	subs	r2, #2
	adds	r6, r7, r1
	adds	r5, r7, r2
	ldrb	r1, [r3, #0]
	ldrb	r2, [r5, #0]
	ldrb	r0, [r6, #0]
	str	r3, [sp, #4]
	bl	Func_080771b0
	ldr	r3, [sp, #4]
	ldrb	r2, [r5, #0]
	ldrb	r1, [r3, #0]
	ldrb	r0, [r6, #0]
	bl	Func_080771c0
.L35:
	adds	r4, r0, #0
	ldrb	r0, [r6, #0]
	str	r4, [sp, #0]
	bl	Func_08077010
	ldr	r2, [r7, #20]
	movs	r3, #13
	strb	r3, [r2, #5]
	ldr	r0, [r7, #48]
	bl	Func_08015278
	ldr	r2, [r7, #20]
	movs	r3, #1
	strb	r3, [r2, #5]
	movs	r5, #2
	ldr	r4, [sp, #0]
	b.n	.L5
.L1:
	movs	r3, #1
	mov	r8, r3
.L5:
	mov	r1, r8
	cmp	r1, #0
	bne.n	.L44
	b.n	.L45
.L44:
	mov	r0, sl
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x00000256
	.4byte 0x0000021a
	.4byte 0x0000021b
	.4byte 0x00000257
	.4byte 0x00000255
