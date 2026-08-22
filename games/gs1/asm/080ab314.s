@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080ab314
	.thumb_func
Func_080ab314:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #660]
	ldr	r1, [r3, #0]
	sub	sp, #32
	str	r1, [sp, #20]
	subs	r3, #160
	ldr	r3, [r3, #0]
	movs	r2, #0
	str	r3, [sp, #16]
	str	r2, [sp, #12]
	str	r2, [sp, #8]
	ldr	r0, [r1, #48]
	bl	Func_08015278
	movs	r0, #1
	bl	Func_080030f8
	movs	r1, #134
	ldr	r3, [sp, #20]
	lsls	r1, r1, #1
	adds	r6, r3, r1
	ldr	r0, [r6, #0]
	bl	Func_08015060
	ldr	r5, [pc, #616]
	ldr	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	adds	r5, #1
	bl	Func_08015080
	ldr	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #16
	bl	Func_08015080
	movs	r5, #6
	movs	r0, #1
	movs	r1, #1
	movs	r2, #11
	movs	r3, #3
	str	r5, [sp, #0]
	bl	Func_080ab21c
	ldr	r2, [sp, #20]
	movs	r3, #10
	ldr	r0, [r2, #48]
	movs	r1, #0
	str	r3, [sp, #0]
	movs	r2, #0
	movs	r3, #28
	str	r5, [sp, #4]
	bl	Func_080ab2ec
	movs	r1, #9
	movs	r2, #8
	movs	r3, #10
	movs	r0, #0
	str	r5, [sp, #0]
	bl	Func_08015010
	movs	r5, #2
	movs	r1, #12
	movs	r2, #22
	movs	r3, #7
	adds	r6, r0, #0
	movs	r0, #8
	str	r5, [sp, #0]
	bl	Func_08015010
	movs	r3, #3
	str	r0, [sp, #28]
	movs	r1, #9
	movs	r2, #22
	movs	r0, #8
	str	r5, [sp, #0]
	bl	Func_08015010
	str	r0, [sp, #24]
	bl	Func_080152a8
	ldr	r3, [pc, #504]
	movs	r7, #0
	movs	r5, #0
	mov	r8, r3
.L0:
	mov	r1, r8
	adds	r0, r5, r1
	lsls	r3, r5, #3
	adds	r1, r6, #0
	movs	r2, #0
	adds	r5, #1
	bl	Func_08015080
	cmp	r5, #6
	ble.n	.L0
	ldr	r3, [pc, #480]
	movs	r2, #1
	movs	r1, #0
	mov	fp, r2
	mov	r9, r3
	mov	r8, r1
.L10:
	ldr	r0, [sp, #24]
	bl	Func_08015060
	ldr	r0, [pc, #456]
	ldr	r1, [sp, #24]
	movs	r2, #0
	movs	r3, #0
	adds	r0, r7, r0
	bl	Func_08015078
	ldr	r1, [pc, #452]
	ldr	r0, [sp, #28]
	adds	r1, r7, r1
	bl	Func_080153f8
	mov	r2, fp
	movs	r3, #15
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	ldr	r2, [sp, #8]
	movs	r1, #0
	movs	r3, #6
	mov	sl, r0
	adds	r0, r6, #0
	bl	Func_080ab1f4
	mov	r3, fp
	str	r3, [sp, #0]
	movs	r3, #14
	str	r3, [sp, #4]
	adds	r0, r6, #0
	movs	r1, #0
	adds	r2, r7, #0
	movs	r3, #6
	bl	Func_080ab1f4
	str	r7, [sp, #8]
	b.n	.L1
.L7:
	mov	r1, r9
	ldr	r2, [r1, #0]
	movs	r3, #96
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L2
	subs	r7, #1
	adds	r0, r7, #0
	movs	r1, #7
	bl	Func_080aa538
	adds	r7, r0, #0
	movs	r0, #111
	bl	Func_080f9010
	b.n	.L3
.L2:
	ldr	r1, [pc, #368]
	ldr	r2, [r1, #0]
	movs	r3, #8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L4
	movs	r0, #113
	bl	Func_080f9010
	movs	r2, #2
	negs	r2, r2
	str	r2, [sp, #12]
	b.n	.L3
.L4:
	ldr	r2, [r1, #0]
	movs	r3, #6
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L5
	movs	r0, #113
	bl	Func_080f9010
	movs	r3, #1
	negs	r3, r3
	str	r3, [sp, #12]
	b.n	.L3
.L5:
	ldr	r3, [r1, #0]
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L1
	bl	Func_08015048
	cmp	r0, #0
	beq.n	.L6
	adds	r7, #1
	adds	r0, r7, #0
	movs	r1, #7
	bl	Func_080aa538
	adds	r7, r0, #0
	movs	r0, #112
	bl	Func_080f9010
	b.n	.L3
.L6:
	movs	r0, #111
	bl	Func_080f9010
.L1:
	ldrh	r1, [r6, #14]
	adds	r1, r1, r7
	lsls	r1, r1, #3
	movs	r0, #12
	adds	r1, #8
	negs	r0, r0
	bl	Func_080a1a40
	movs	r0, #1
	bl	Func_080030f8
	mov	r3, r9
	ldr	r2, [r3, #0]
	movs	r3, #144
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L7
	adds	r7, #1
	adds	r0, r7, #0
	movs	r1, #7
	bl	Func_080aa538
	adds	r7, r0, #0
	movs	r0, #111
	bl	Func_080f9010
.L3:
	ldr	r1, [sp, #16]
	ldr	r2, [pc, #224]
	adds	r5, r1, r2
	ldrh	r3, [r5, #0]
	cmp	r3, #99
	beq.n	.L8
	adds	r0, r3, #0
	bl	Func_08003f3c
	movs	r3, #99
	strh	r3, [r5, #0]
.L8:
	ldr	r5, [pc, #208]
	ldr	r1, [pc, #208]
	ldr	r3, [r5, #0]
	mov	r2, r8
	adds	r3, r3, r1
	strb	r2, [r3, #0]
	ldr	r0, [sp, #28]
	bl	Func_08015060
	mov	r1, sl
	ldr	r3, [r1, #0]
	mov	r2, r8
	mov	r1, r8
	strh	r1, [r3, #26]
	strh	r2, [r3, #24]
	strh	r2, [r3, #20]
	mov	r1, sl
	mov	r3, r8
	str	r3, [r1, #0]
	ldr	r2, [sp, #12]
	cmp	r2, #0
	bne.n	.L9
	b.n	.L10
.L9:
	ldr	r1, [pc, #168]
	ldr	r3, [r5, #0]
	movs	r2, #1
	adds	r3, r3, r1
	strb	r2, [r3, #0]
	ldr	r0, [sp, #24]
	bl	Func_08015278
	ldr	r0, [sp, #28]
	bl	Func_08015278
	movs	r0, #1
	bl	Func_080030f8
	movs	r1, #1
	ldr	r0, [sp, #24]
	bl	Func_08015018
	adds	r0, r6, #0
	movs	r1, #1
	bl	Func_08015018
	movs	r1, #1
	ldr	r0, [sp, #28]
	bl	Func_08015018
	bl	Func_080152a8
	movs	r3, #2
	ldr	r2, [sp, #12]
	negs	r3, r3
	cmp	r2, r3
	bne.n	.L11
	ldr	r1, [sp, #20]
	movs	r2, #134
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldr	r0, [r3, #0]
	bl	Func_08015060
	ldr	r3, [sp, #20]
	ldr	r0, [r3, #48]
	bl	Func_08015060
	ldr	r1, [sp, #20]
	ldr	r0, [r1, #16]
	bl	Func_08015060
	ldr	r2, [pc, #72]
	ldr	r3, [r5, #0]
	adds	r3, r3, r2
	movs	r2, #0
	strb	r2, [r3, #0]
.L11:
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #64]
	bl	Func_080041d8
	ldr	r0, [sp, #12]
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f2c
	.4byte 0x00000c30
	.4byte 0x00000c32
	.4byte 0x03001b04
	.4byte 0x00000c39
	.4byte 0x03001c94
	.4byte 0x000012b6
	.4byte 0x03001e8c
	.4byte 0x000012f8
	.4byte 0x00000ea6
	.4byte 0x080a19a1
