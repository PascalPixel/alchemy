@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08098cd8
	.thumb_func
Func_08098cd8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #192]
	ldr	r3, [r3, #0]
	ldr	r0, [r3, #20]
	sub	sp, #44
	mov	r9, r3
	str	r0, [sp, #8]
	bl	Func_08097384
	movs	r0, #130
	bl	Func_080f9010
	add	r1, sp, #16
	mov	r5, r9
	mov	sl, r1
	movs	r2, #11
	adds	r5, #88
	mov	r6, sl
	mov	r8, r2
.L0:
	mov	r3, r9
	ldr	r2, [r3, #16]
	ldr	r3, [r2, #8]
	str	r3, [r6, #0]
	movs	r0, #128
	ldr	r3, [r2, #12]
	lsls	r0, r0, #13
	adds	r3, r3, r0
	str	r3, [r6, #4]
	ldr	r3, [r2, #16]
	adds	r0, r6, #0
	str	r3, [r6, #8]
	bl	Func_080974d8
	movs	r1, #142
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #8]
	adds	r0, r5, #0
	lsls	r1, r1, #1
	bl	Func_0809ba90
	adds	r0, r5, #0
	ldr	r1, [pc, #116]
	bl	Func_0809ba7c
	adds	r0, r5, #0
	movs	r1, #7
	bl	Func_0809ba70
	ldr	r0, [r5, #0]
	movs	r1, #9
	bl	Func_08009248
	ldr	r3, [pc, #96]
	movs	r0, #2
	str	r3, [r5, #44]
	str	r3, [r5, #40]
	bl	Func_080030f8
	movs	r1, #1
	negs	r1, r1
	add	r8, r1
	mov	r2, r8
	adds	r5, #72
	cmp	r2, #0
	bge.n	.L0
	mov	r3, r9
	ldr	r2, [r3, #16]
	ldr	r3, [r2, #8]
	mov	r0, sl
	str	r3, [r0, #0]
	movs	r1, #128
	ldr	r3, [r2, #12]
	lsls	r1, r1, #13
	adds	r3, r3, r1
	str	r3, [r0, #4]
	ldr	r3, [r2, #16]
	str	r3, [r0, #8]
	mov	r2, r9
	movs	r0, #128
	ldr	r1, [r2, #0]
	lsls	r0, r0, #12
	mov	r2, sl
	bl	Func_0800447c
	mov	r3, sl
	ldr	r1, [r3, #0]
	ldr	r2, [r3, #4]
	movs	r0, #215
	ldr	r3, [r3, #8]
	bl	Func_08096c80
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L1
	bl	Func_0809748c
	b.n	.L2
	movs	r0, r0
	.4byte 0x03001f30
	.4byte 0x08098b11
	.4byte 0x0000b333
.L1:
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r6, #28]
	str	r3, [r6, #24]
	mov	r0, r9
	ldr	r3, [r0, #0]
	strh	r3, [r6, #6]
	movs	r3, #128
	lsls	r3, r3, #11
	ldr	r2, [pc, #40]
	str	r3, [r6, #48]
	str	r3, [r6, #52]
	adds	r3, r6, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #5
	bl	Func_08009080
	movs	r1, #3
	adds	r0, r6, #0
	bl	Func_08009240
	movs	r1, #128
	ldr	r3, [r6, #24]
	lsls	r1, r1, #9
	cmp	r3, r1
	bge.n	.L3
	b.n	.L4
	movs	r0, r0
	.4byte 0x00000000
.L4:
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r3, r3, r2
	str	r3, [r6, #28]
	str	r3, [r6, #24]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [r6, #24]
	ldr	r0, [pc, #508]
	cmp	r3, r0
	ble.n	.L4
.L3:
	movs	r0, #3
	bl	Func_080030f8
	mov	r3, sp
	adds	r3, #28
	movs	r1, #0
	movs	r2, #2
	str	r3, [sp, #4]
	mov	fp, r1
	mov	r8, r2
	add	r7, sp, #36
.L6:
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #16]
	movs	r0, #215
	bl	Func_08096c80
	adds	r5, r0, #0
	str	r0, [r7, #0]
	subs	r7, #4
	cmp	r5, #0
	beq.n	.L5
	movs	r3, #240
	lsls	r3, r3, #8
	str	r3, [r5, #28]
	str	r3, [r5, #24]
	mov	r0, r9
	ldr	r3, [r0, #0]
	strh	r3, [r5, #6]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r5, #48]
	str	r3, [r5, #52]
	adds	r3, r5, #0
	adds	r3, #85
	movs	r1, #0
	strb	r1, [r3, #0]
	adds	r0, r5, #0
	movs	r1, #5
	bl	Func_08009080
	adds	r0, r5, #0
	movs	r1, #2
	bl	Func_08009240
	mov	r1, fp
	ldr	r0, [r5, #80]
	bl	Func_08096c48
	mov	fp, r0
.L5:
	movs	r2, #1
	negs	r2, r2
	add	r8, r2
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L6
	mov	r3, r9
	mov	r0, fp
	adds	r3, #32
	ldrb	r0, [r0, #28]
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	mov	fp, r0
	cmp	r3, #0
	beq.n	.L7
	mov	r1, r9
	ldr	r2, [r1, #16]
	ldr	r3, [r2, #8]
	mov	r0, sl
	str	r3, [r0, #0]
	movs	r1, #128
	ldr	r3, [r2, #12]
	lsls	r1, r1, #13
	adds	r3, r3, r1
	str	r3, [r0, #4]
	ldr	r3, [r2, #16]
	str	r3, [r0, #8]
	mov	r2, r9
	movs	r0, #224
	ldr	r1, [r2, #0]
	lsls	r0, r0, #14
	mov	r2, sl
	bl	Func_0800447c
	b.n	.L8
.L7:
	mov	r0, r9
	ldr	r3, [r0, #4]
	mov	r1, sl
	str	r3, [r1, #0]
	movs	r2, #128
	ldr	r3, [r0, #8]
	lsls	r2, r2, #13
	adds	r3, r3, r2
	str	r3, [r1, #4]
	ldr	r3, [r0, #12]
	str	r3, [r1, #8]
.L8:
	mov	r3, sl
	ldr	r1, [r3, #0]
	ldr	r2, [r3, #4]
	adds	r0, r6, #0
	ldr	r3, [r3, #8]
	bl	Func_08009150
	ldr	r1, [pc, #296]
	adds	r0, r6, #0
	bl	Func_08009098
	ldr	r0, [sp, #4]
	movs	r1, #2
	str	r0, [sp, #0]
	mov	r7, sl
	mov	r8, r1
.L10:
	ldr	r3, [sp, #0]
	ldmia	r3!, {r5}
	adds	r2, r3, #0
	str	r2, [sp, #0]
	cmp	r5, #0
	beq.n	.L9
	movs	r0, #3
	bl	Func_080030f8
	ldr	r1, [r7, #0]
	adds	r0, r5, #0
	ldr	r2, [r7, #4]
	ldr	r3, [r7, #8]
	bl	Func_08009150
	adds	r0, r5, #0
	ldr	r1, [pc, #252]
	bl	Func_08009098
.L9:
	movs	r0, #1
	negs	r0, r0
	add	r8, r0
	mov	r1, r8
	cmp	r1, #0
	bge.n	.L10
	ldr	r3, [r6, #0]
	movs	r2, #0
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L11
.L12:
	movs	r0, #1
	bl	Func_080030f8
	movs	r3, #1
	add	r8, r3
	mov	r0, r8
	cmp	r0, #59
	bgt.n	.L11
	ldr	r3, [r6, #0]
	cmp	r3, #0
	bne.n	.L12
.L11:
	ldr	r1, [sp, #8]
	cmp	r1, #0
	beq.n	.L13
	mov	r3, r9
	adds	r3, #53
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L13
	mov	r3, r9
	adds	r3, #52
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L14
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r1, #40]
.L14:
	ldr	r2, [sp, #8]
	ldr	r3, [r2, #8]
	mov	r0, sl
	str	r3, [r0, #0]
	ldr	r3, [r2, #12]
	str	r3, [r0, #4]
	ldr	r3, [r2, #16]
	str	r3, [r0, #8]
	mov	r2, r9
	movs	r0, #128
	ldr	r1, [r2, #0]
	lsls	r0, r0, #13
	mov	r2, sl
	bl	Func_0800447c
	mov	r1, sl
	ldr	r0, [sp, #8]
	bl	Func_080091d8
	cmp	r0, #0
	bne.n	.L13
	ldr	r0, [sp, #8]
	mov	r1, sl
	bl	Func_08009250
	cmp	r0, #0
	bne.n	.L13
	ldr	r0, [sp, #8]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r0, #52]
	str	r3, [r0, #48]
	mov	r2, sl
	mov	r0, sl
	ldr	r1, [r2, #0]
	ldr	r3, [r0, #8]
	ldr	r2, [r2, #4]
	ldr	r0, [sp, #8]
	bl	Func_08009150
.L13:
	ldr	r0, [pc, #84]
	add	r2, sp, #12
	movs	r1, #4
	bl	Func_0808e4b4
	cmp	r0, #0
	beq.n	.L15
	ldr	r3, [pc, #76]
	movs	r1, #250
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldr	r1, [r3, #0]
	ldr	r2, [sp, #12]
	bl	Func_08096b28
.L15:
	movs	r0, #10
	bl	Func_080030f8
	bl	Func_0809748c
	movs	r0, #20
	bl	Func_080030f8
	mov	r2, fp
	cmp	r2, #96
	beq.n	.L2
	mov	r0, fp
	bl	Func_08003f3c
.L2:
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x0000ffff
	.4byte 0x0809f12c
	.4byte 0x0809f0b4
	.4byte 0x50000005
	.4byte 0x02000240
