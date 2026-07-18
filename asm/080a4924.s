@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a4924
	.thumb_func
Func_080a4924:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r8, r0
	ldr	r0, [pc, #832]
	sub	sp, #12
	movs	r5, #0
	ands	r0, r1
	mov	fp, r1
	str	r5, [sp, #8]
	bl	Func_08077018
	ldrb	r3, [r0, #2]
	mov	sl, r0
	cmp	r3, #0
	beq.n	.L0
	ldr	r3, [r0, #8]
	ldr	r2, [pc, #812]
	ands	r3, r2
	movs	r1, #0
	cmp	r3, #0
	bne.n	.L1
	mov	r2, sl
	mov	r9, r1
	adds	r2, #24
.L3:
	mov	r0, r9
	cmp	r0, #3
	bgt.n	.L2
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L1
	mov	r0, sl
	ldrb	r3, [r0, #12]
	movs	r0, #1
	adds	r2, #4
	add	r9, r0
	cmp	r3, #3
	bne.n	.L3
.L1:
	movs	r1, #1
.L2:
	cmp	r1, #1
	bne.n	.L4
	ldr	r0, [pc, #768]
	mov	r1, r8
	movs	r2, #16
	movs	r3, #0
	bl	Func_08015080
	movs	r5, #1
.L4:
	mov	r2, sl
	movs	r1, #8
	ldrsh	r3, [r2, r1]
	cmp	r3, #0
	beq.n	.L5
	lsls	r5, r5, #24
	asrs	r5, r5, #24
	lsls	r6, r5, #3
	adds	r3, r6, #0
	ldr	r0, [pc, #740]
	mov	r1, r8
	movs	r2, #0
	bl	Func_08015080
	mov	r0, sl
	movs	r3, #8
	ldrsh	r7, [r0, r3]
	movs	r1, #3
	adds	r0, r7, #0
	mov	r2, r8
	movs	r3, #64
	str	r6, [sp, #0]
	bl	Func_080a4db4
	adds	r5, #1
	lsls	r5, r5, #24
	lsrs	r5, r5, #24
.L5:
	mov	r1, sl
	movs	r3, #10
	ldrsb	r3, [r1, r3]
	cmp	r3, #0
	beq.n	.L0
	lsls	r5, r5, #24
	asrs	r5, r5, #24
	lsls	r6, r5, #3
	ldr	r0, [pc, #692]
	mov	r1, r8
	movs	r2, #0
	adds	r3, r6, #0
	bl	Func_08015080
	mov	r2, sl
	movs	r7, #10
	ldrsb	r7, [r2, r7]
	movs	r1, #3
	adds	r0, r7, #0
	mov	r2, r8
	movs	r3, #64
	str	r6, [sp, #0]
	bl	Func_080a4db4
	adds	r5, #1
	lsls	r5, r5, #24
	lsrs	r5, r5, #24
.L0:
	movs	r3, #0
	mov	r9, r3
.L16:
	mov	r0, r9
	lsls	r1, r0, #2
	adds	r2, r1, #0
	adds	r2, #24
	mov	r0, sl
	ldrb	r3, [r0, r2]
	cmp	r3, #0
	bne.n	.L6
	b.n	.L7
.L6:
	adds	r3, r0, r2
	movs	r7, #1
	ldrsb	r7, [r3, r7]
	ldrb	r3, [r3, #0]
	cmp	r3, #27
	bls.n	.L8
	b.n	.L9
.L8:
	ldr	r2, [pc, #620]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080a4b94
	.4byte 0x080a4a94
	.4byte 0x080a4a94
	.4byte 0x080a4a94
	.4byte 0x080a4a94
	.4byte 0x080a4a94
	.4byte 0x080a4a94
	.4byte 0x080a4b06
	.4byte 0x080a4b06
	.4byte 0x080a4b06
	.4byte 0x080a4b06
	.4byte 0x080a4b06
	.4byte 0x080a4b06
	.4byte 0x080a4b06
	.4byte 0x080a4b06
	.4byte 0x080a4aaa
	.4byte 0x080a4aaa
	.4byte 0x080a4aaa
	.4byte 0x080a4aaa
	.4byte 0x080a4aaa
	.4byte 0x080a4aaa
	.4byte 0x080a4aaa
	.4byte 0x080a4aaa
	.4byte 0x080a4b7a
	.4byte 0x080a4b94
	.4byte 0x080a4b7a
	.4byte 0x080a4a94
	.4byte 0x080a4b7a
	adds	r3, r1, #0
	adds	r3, #24
	mov	r1, sl
	ldrb	r0, [r1, r3]
	lsls	r6, r5, #24
	ldr	r3, [pc, #496]
	asrs	r5, r6, #21
	adds	r0, r0, r3
	mov	r1, r8
	movs	r2, #0
	b.n	.L10
	adds	r4, r1, #0
	mov	r3, sl
	adds	r4, #24
	ldrb	r2, [r3, r4]
	adds	r3, r2, #0
	subs	r3, #15
	adds	r1, r3, #0
	cmp	r3, #0
	bge.n	.L11
	adds	r1, r2, #0
	subs	r1, #12
.L11:
	asrs	r1, r1, #2
	lsls	r1, r1, #2
	subs	r1, r3, r1
	lsls	r1, r1, #24
	lsls	r6, r5, #24
	asrs	r5, r6, #24
	movs	r3, #2
	lsrs	r1, r1, #24
	str	r3, [sp, #0]
	adds	r1, #1
	adds	r3, r5, #0
	mov	r0, r8
	movs	r2, #0
	str	r4, [sp, #4]
	bl	Func_08015280
	ldr	r4, [sp, #4]
	mov	r1, sl
	ldrb	r0, [r1, r4]
	ldr	r3, [pc, #424]
	lsls	r5, r5, #3
	adds	r0, r0, r3
	mov	r1, r8
	movs	r2, #8
.L10:
	adds	r3, r5, #0
	bl	Func_08015080
	adds	r0, r7, #0
	movs	r1, #3
	mov	r2, r8
	movs	r3, #64
	str	r5, [sp, #0]
	bl	Func_080a4db4
	b.n	.L12
	adds	r3, r1, #0
	adds	r3, #24
	mov	r2, sl
	ldrb	r0, [r2, r3]
	lsls	r6, r5, #24
	ldr	r3, [pc, #380]
	asrs	r5, r6, #21
	adds	r0, r0, r3
	mov	r1, r8
	movs	r2, #0
	adds	r3, r5, #0
	bl	Func_08015080
	ldr	r0, [pc, #368]
	mov	r1, r8
	movs	r2, #64
	adds	r3, r5, #0
	bl	Func_08015098
	cmp	r7, #9
	ble.n	.L13
	movs	r0, #1
	movs	r1, #1
	mov	r2, r8
	movs	r3, #72
	str	r5, [sp, #0]
	bl	Func_080150b0
	ldr	r0, [pc, #344]
	mov	r1, r8
	movs	r2, #80
	adds	r3, r5, #0
	bl	Func_08015098
	adds	r0, r7, #0
	subs	r0, #10
	b.n	.L14
.L13:
	movs	r0, #0
	movs	r1, #1
	mov	r2, r8
	movs	r3, #72
	str	r5, [sp, #0]
	bl	Func_080150b0
	ldr	r0, [pc, #312]
	mov	r1, r8
	movs	r2, #80
	adds	r3, r5, #0
	bl	Func_08015098
	adds	r0, r7, #0
.L14:
	movs	r1, #1
	mov	r2, r8
	movs	r3, #88
	str	r5, [sp, #0]
	bl	Func_080150b0
	b.n	.L12
	adds	r3, r1, #0
	adds	r3, #24
	mov	r1, sl
	ldrb	r0, [r1, r3]
	ldr	r3, [pc, #268]
	lsls	r6, r5, #24
	adds	r0, r0, r3
	mov	r1, r8
	asrs	r3, r6, #21
	movs	r2, #0
	bl	Func_08015080
	b.n	.L12
.L9:
	lsls	r6, r5, #24
.L12:
	movs	r2, #128
	lsls	r2, r2, #17
	adds	r3, r6, r2
	lsrs	r5, r3, #24
.L7:
	movs	r3, #1
	add	r9, r3
	mov	r0, r9
	cmp	r0, #3
	bgt.n	.L15
	b.n	.L16
.L15:
	mov	r1, sl
	ldrb	r2, [r1, #3]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L17
	lsls	r5, r5, #24
	asrs	r5, r5, #24
	lsls	r3, r5, #3
	ldr	r0, [pc, #224]
	mov	r1, r8
	movs	r2, #0
	bl	Func_08015080
	adds	r5, #1
	lsls	r5, r5, #24
	lsrs	r5, r5, #24
.L17:
	mov	r3, sl
	ldrb	r2, [r3, #12]
	adds	r3, r2, #0
	cmp	r3, #3
	bne.n	.L18
	lsls	r5, r5, #24
	asrs	r5, r5, #24
	ldr	r0, [pc, #196]
	lsls	r3, r5, #3
	mov	r1, r8
	movs	r2, #0
	bl	Func_08015080
	movs	r0, #1
	str	r0, [sp, #8]
	adds	r5, #1
	mov	r1, sl
	lsls	r5, r5, #24
	ldrb	r2, [r1, #12]
	lsrs	r5, r5, #24
.L18:
	adds	r3, r2, #0
	cmp	r3, #4
	beq.n	.L19
	cmp	r3, #0
	beq.n	.L19
	ldr	r2, [sp, #8]
	cmp	r2, #0
	bne.n	.L20
	lsls	r5, r5, #24
	asrs	r5, r5, #24
	lsls	r3, r5, #3
	ldr	r0, [pc, #152]
	mov	r1, r8
	movs	r2, #16
	bl	Func_08015080
	adds	r5, #1
	lsls	r5, r5, #24
	lsrs	r5, r5, #24
.L20:
	mov	r0, sl
	ldrb	r3, [r0, #12]
	cmp	r3, #1
	beq.n	.L21
	cmp	r3, #1
	ble.n	.L19
	cmp	r3, #2
	beq.n	.L22
	b.n	.L19
.L21:
	lsls	r5, r5, #24
	asrs	r5, r5, #24
	lsls	r3, r5, #3
	adds	r5, #1
	ldr	r0, [pc, #116]
	mov	r1, r8
	movs	r2, #0
	lsls	r5, r5, #24
	bl	Func_08015080
	b.n	.L23
.L22:
	movs	r3, #128
	lsls	r3, r3, #3
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L24
	lsls	r5, r5, #24
	asrs	r5, r5, #24
	ldr	r6, [pc, #88]
	lsls	r3, r5, #3
	adds	r5, #1
	lsls	r5, r5, #24
	adds	r0, r6, #0
	mov	r1, r8
	movs	r2, #0
	asrs	r5, r5, #24
	bl	Func_08015080
	adds	r6, #1
	lsls	r3, r5, #3
	adds	r5, #1
	adds	r0, r6, #0
	mov	r1, r8
	movs	r2, #0
	lsls	r5, r5, #24
	bl	Func_08015080
	b.n	.L23
	.4byte 0x000001ff
	.4byte 0x00ffffff
	.4byte 0x00000b6d
	.4byte 0x00000af7
	.4byte 0x00000af8
	.4byte 0x080a4a24
	.4byte 0x00000b3b
	.4byte 0x080af21c
	.4byte 0x080af220
	.4byte 0x00000b76
	.4byte 0x00000b65
	.4byte 0x00000b6e
	.4byte 0x00000b63
	.4byte 0x00000b73
.L24:
	lsls	r5, r5, #24
	asrs	r5, r5, #24
	ldr	r6, [pc, #228]
	lsls	r3, r5, #3
	adds	r5, #1
	lsls	r5, r5, #24
	adds	r0, r6, #0
	mov	r1, r8
	movs	r2, #0
	asrs	r5, r5, #24
	adds	r6, #1
	bl	Func_08015080
	lsls	r3, r5, #3
	adds	r0, r6, #0
	mov	r1, r8
	movs	r2, #0
	bl	Func_08015080
	adds	r5, #1
	lsls	r5, r5, #24
.L23:
	lsrs	r5, r5, #24
.L19:
	mov	r3, sl
	ldrb	r2, [r3, #3]
	movs	r3, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L25
	lsls	r6, r5, #24
	asrs	r3, r6, #24
	cmp	r3, #0
	beq.n	.L26
	adds	r3, #1
	lsls	r6, r3, #24
.L26:
	ldr	r0, [pc, #168]
	asrs	r5, r6, #24
	lsls	r3, r5, #3
	mov	r1, r8
	movs	r2, #16
	mov	r9, r0
	bl	Func_08015080
	movs	r0, #248
	adds	r5, #1
	lsls	r0, r0, #8
	mov	r1, fp
	lsls	r5, r5, #24
	ands	r0, r1
	lsrs	r5, r5, #24
	cmp	r0, #0
	bge.n	.L27
	ldr	r2, [pc, #140]
	adds	r0, r0, r2
.L27:
	asrs	r7, r0, #11
	adds	r0, r7, #1
	movs	r1, #5
	lsls	r5, r5, #24
	bl	Func_08015120
	asrs	r5, r5, #24
	mov	r0, r9
	lsls	r3, r5, #3
	adds	r0, #1
	mov	r1, r8
	movs	r2, #0
	bl	Func_08015080
	adds	r5, #1
	lsls	r5, r5, #24
	lsrs	r5, r5, #24
.L25:
	movs	r1, #0
	cmp	r5, #0
	bne.n	.L28
	mov	r3, sl
	ldrb	r2, [r3, #3]
	movs	r3, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L29
	mov	r1, r8
	ldr	r0, [pc, #84]
	movs	r2, #0
	movs	r3, #0
	bl	Func_08015080
	movs	r1, #1
.L29:
	cmp	r1, #0
	bne.n	.L28
	mov	r0, sl
	ldrb	r2, [r0, #3]
	movs	r3, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L30
	mov	r1, r8
	ldr	r0, [pc, #60]
	movs	r2, #0
	movs	r3, #0
	bl	Func_08015080
	movs	r1, #1
.L30:
	cmp	r1, #0
	bne.n	.L28
	ldr	r0, [pc, #48]
	mov	r1, r8
	movs	r2, #0
	movs	r3, #0
	bl	Func_08015080
.L28:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00000b71
	.4byte 0x00000b6f
	.4byte 0x000007ff
	.4byte 0x00000b69
	.4byte 0x00000b6a
	.4byte 0x00000b6c
