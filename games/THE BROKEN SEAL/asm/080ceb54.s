@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080ceb54
	.thumb_func
Func_080ceb54:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #84
	ldr	r2, [pc, #520]
	str	r1, [sp, #60]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	str	r3, [sp, #56]
	subs	r2, #108
	ldr	r3, [pc, #512]
	ldr	r2, [r2, #0]
	mov	sl, r1
	add	r3, sl
	str	r2, [sp, #44]
	str	r0, [r3, #0]
	movs	r0, #1
	bl	Func_080cd594
	ldr	r0, [pc, #496]
	bl	Func_08002f40
	mov	r1, sl
	bl	Func_08005340
	ldr	r2, [sp, #60]
	cmp	r2, #0
	bne.n	.L0
	ldr	r0, [pc, #484]
	b.n	.L1
.L0:
	ldr	r3, [sp, #60]
	cmp	r3, #1
	bne.n	.L2
	ldr	r0, [pc, #476]
	b.n	.L1
.L2:
	ldr	r0, [pc, #476]
.L1:
	bl	Func_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	movs	r2, #128
	ldr	r3, [pc, #468]
	lsls	r0, r0, #19
	bl	Func_080072f0
	ldr	r3, [pc, #440]
	add	r3, sl
	ldr	r3, [r3, #0]
	add	r1, sp, #64
	ldr	r0, [r3, #4]
	bl	Func_080cef64
	movs	r1, #1
	movs	r2, #128
	ldr	r3, [pc, #448]
	movs	r7, #0
	negs	r1, r1
	lsls	r2, r2, #3
.L3:
	adds	r7, #1
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r7, r2
	bne.n	.L3
	movs	r0, #0
	str	r0, [sp, #48]
	ldr	r2, [pc, #400]
	mov	r1, sl
	ldr	r3, [r1, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L4
	movs	r3, #255
	mov	r9, r3
	mov	fp, r0
.L6:
	mov	r0, sl
	adds	r5, r0, r2
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #8]
	bl	Func_080b5098
	ldr	r3, [r5, #0]
	ldr	r6, [r0, #0]
	ldr	r0, [r3, #8]
	bl	Func_080b5070
	ldr	r5, [pc, #388]
	mov	r8, r0
	movs	r7, #0
	add	r5, fp
.L5:
	ldr	r3, [r6, #8]
	mov	r1, r8
	str	r1, [r5, #4]
	str	r3, [r5, #0]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	Func_08004458
	mov	r2, r9
	ands	r0, r2
	subs	r0, #128
	lsls	r0, r0, #10
	str	r0, [r5, #12]
	bl	Func_08004458
	mov	r3, r9
	ands	r0, r3
	subs	r0, #128
	lsls	r0, r0, #10
	str	r0, [r5, #16]
	bl	Func_08004458
	mov	r1, r9
	ands	r0, r1
	subs	r0, #128
	lsls	r0, r0, #10
	movs	r3, #0
	adds	r7, #1
	str	r0, [r5, #20]
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #128
	bne.n	.L5
	ldr	r3, [sp, #48]
	movs	r2, #224
	lsls	r2, r2, #4
	adds	r3, #1
	str	r3, [sp, #48]
	add	fp, r2
	ldr	r2, [pc, #276]
	mov	r0, sl
	ldr	r3, [r0, r2]
	ldr	r1, [sp, #48]
	ldr	r3, [r3, #20]
	cmp	r1, r3
	bne.n	.L6
.L4:
	movs	r5, #144
	lsls	r5, r5, #3
	adds	r1, r5, #0
	ldr	r0, [pc, #288]
	bl	Func_080041d8
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, sl
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #276]
	movs	r3, #75
	add	r2, sl
	str	r3, [r2, #0]
	adds	r1, r5, #0
	ldr	r0, [pc, #268]
	bl	Func_080041d8
	movs	r0, #146
	bl	Func_080f9010
	ldr	r3, [sp, #60]
	movs	r2, #0
	lsls	r3, r3, #1
	str	r2, [sp, #52]
	str	r3, [sp, #28]
	ldr	r2, [pc, #252]
	adds	r3, #1
	ldrb	r1, [r2, r3]
	ldr	r3, [pc, #200]
	add	r3, sl
	ldr	r3, [r3, #0]
	ldr	r2, [r3, #20]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #2
	cmn	r1, r3
	bne.n	.L7
	b.n	.L8
.L7:
	ldr	r0, [sp, #44]
	adds	r0, #12
	str	r0, [sp, #24]
.L22:
	ldr	r1, [sp, #52]
	cmp	r1, #80
	bne.n	.L9
	ldr	r2, [sp, #60]
	cmp	r2, #0
	bne.n	.L10
	movs	r0, #134
	bl	Func_080b50e8
	b.n	.L9
.L10:
	movs	r0, #133
	bl	Func_080b50e8
.L9:
	bl	Func_080049ac
	ldr	r0, [sp, #44]
	ldr	r1, [sp, #24]
	bl	Func_080051d8
	movs	r6, #211
	ldr	r0, [sp, #52]
	lsls	r6, r6, #7
	movs	r3, #128
	add	r6, sl
	movs	r7, #0
	lsls	r3, r3, #13
	lsls	r5, r0, #10
.L11:
	adds	r0, r5, #0
	str	r3, [sp, #8]
	bl	Func_08002322
	ldr	r3, [sp, #8]
	lsls	r0, r0, #4
	subs	r0, r3, r0
	movs	r1, #128
	asrs	r0, r0, #10
	lsls	r1, r1, #3
	adds	r7, #1
	stmia	r6!, {r0}
	adds	r5, r5, r1
	cmp	r7, #160
	bne.n	.L11
	movs	r2, #0
	str	r2, [sp, #48]
	ldr	r2, [pc, #88]
	mov	r0, sl
	ldr	r3, [r0, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	bne.n	.L12
	b.n	.L13
.L12:
	movs	r1, #0
	movs	r3, #36
	str	r1, [sp, #20]
	str	r3, [sp, #16]
	str	r1, [sp, #12]
	mov	fp, r1
.L21:
	mov	r0, sl
	adds	r5, r0, r2
	ldr	r1, [sp, #16]
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r1]
	bl	Func_080b5098
	ldr	r3, [r5, #0]
	ldr	r0, [r0, #0]
	ldr	r1, [sp, #16]
	mov	r9, r0
	ldrsh	r0, [r3, r1]
	bl	Func_080b5070
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	str	r0, [sp, #40]
	mov	r3, fp
	ldr	r0, [sp, #52]
	adds	r3, #71
	cmp	r0, r3
	bne.n	.L14
	ldr	r1, [sp, #60]
	cmp	r1, #0
	bne.n	.L15
	movs	r0, #134
	bl	Func_080f9010
	b.n	.L14
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x00000069
	.4byte 0x000000bb
	.4byte 0x0000008d
	.4byte 0x00000091
	.4byte 0x03001388
	.4byte 0x02010018
	.4byte 0x02010000
	.4byte 0x080dbb9d
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x080ee090
.L15:
	movs	r0, #133
	bl	Func_080f9010
.L14:
	mov	r3, fp
	ldr	r2, [sp, #52]
	adds	r3, #70
	cmp	r2, r3
	bne.n	.L16
	ldr	r3, [pc, #396]
	add	r3, sl
	ldr	r3, [r3, #0]
	ldr	r1, [sp, #16]
	ldrsh	r0, [r3, r1]
	movs	r3, #26
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	ldr	r3, [sp, #48]
	bl	Func_080d6888
.L16:
	ldr	r3, [sp, #52]
	cmp	r3, fp
	ble.n	.L17
	ldr	r2, [pc, #372]
	ldr	r0, [sp, #28]
	ldrb	r3, [r2, r0]
	movs	r7, #0
	cmp	r3, #0
	beq.n	.L17
	ldr	r1, [sp, #20]
	add	r3, sp, #72
	str	r1, [sp, #36]
	mov	r8, r3
	mov	r0, fp
	ldr	r1, [sp, #12]
	ldr	r3, [pc, #352]
	str	r0, [sp, #32]
	adds	r6, r1, r3
.L20:
	ldr	r0, [sp, #36]
	lsls	r3, r0, #1
	adds	r3, r3, r7
	ldr	r1, [sp, #52]
	lsls	r3, r3, #1
	cmp	r1, r3
	ble.n	.L18
	ldr	r3, [r6, #24]
	cmp	r3, #0
	blt.n	.L18
	mov	r1, r8
	adds	r0, r6, #0
	bl	Func_080e3944
	mov	r2, r8
	ldr	r5, [r2, #0]
	asrs	r5, r5, #1
	str	r5, [r2, #0]
	movs	r1, #3
	adds	r0, r7, #0
	bl	Func_080022fc
	lsls	r1, r0, #2
	adds	r1, r1, r0
	mov	r0, r8
	ldr	r3, [r0, #4]
	movs	r2, #20
	str	r2, [sp, #0]
	lsls	r1, r1, #7
	movs	r2, #32
	subs	r5, #10
	subs	r3, #16
	str	r2, [sp, #4]
	add	r1, sl
	ldr	r4, [sp, #64]
	ldr	r0, [sp, #56]
	adds	r2, r5, #0
	bl	Func_080072f4
	movs	r1, #62
	movs	r2, #0
	adds	r0, r6, #0
	bl	Func_080e38b8
	ldr	r1, [sp, #32]
	ldr	r2, [sp, #52]
	adds	r3, r1, r7
	adds	r3, #30
	cmp	r2, r3
	ble.n	.L19
	mov	r3, r9
	ldr	r0, [r3, #8]
	ldr	r3, [r6, #0]
	mov	r2, r9
	ldr	r1, [r2, #12]
	subs	r0, r0, r3
	ldr	r3, [sp, #40]
	adds	r1, r1, r3
	ldr	r3, [r6, #4]
	ldr	r2, [r2, #16]
	subs	r1, r1, r3
	ldr	r3, [r6, #8]
	subs	r2, r2, r3
	ldr	r3, [r6, #12]
	asrs	r0, r0, #9
	adds	r3, r3, r0
	str	r3, [r6, #12]
	ldr	r3, [r6, #16]
	asrs	r1, r1, #9
	adds	r3, r3, r1
	ldr	r1, [pc, #208]
	str	r3, [r6, #16]
	ldr	r3, [r6, #20]
	asrs	r2, r2, #9
	adds	r0, r0, r1
	ldr	r1, [pc, #204]
	adds	r3, r3, r2
	str	r3, [r6, #20]
	cmp	r0, r1
	bhi.n	.L19
	ldr	r0, [pc, #188]
	adds	r3, r2, r0
	cmp	r3, r1
	bhi.n	.L19
	movs	r3, #1
	negs	r3, r3
	str	r3, [r6, #24]
.L19:
	ldr	r2, [pc, #168]
.L18:
	ldr	r1, [sp, #28]
	ldrb	r3, [r2, r1]
	adds	r7, #1
	adds	r6, #28
	cmp	r7, r3
	bne.n	.L20
.L17:
	ldr	r2, [sp, #20]
	ldr	r3, [sp, #16]
	adds	r2, #5
	adds	r3, #2
	str	r2, [sp, #20]
	str	r3, [sp, #16]
	ldr	r1, [sp, #12]
	ldr	r3, [sp, #48]
	movs	r2, #224
	lsls	r2, r2, #4
	adds	r1, r1, r2
	adds	r3, #1
	str	r1, [sp, #12]
	str	r3, [sp, #48]
	movs	r0, #20
	ldr	r2, [pc, #120]
	add	fp, r0
	mov	r0, sl
	ldr	r3, [r0, r2]
	ldr	r1, [sp, #48]
	ldr	r3, [r3, #20]
	cmp	r1, r3
	beq.n	.L13
	b.n	.L21
.L13:
	bl	Func_080cd52c
	ldr	r2, [pc, #116]
	movs	r3, #1
	add	r2, sl
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r2, [sp, #52]
	ldr	r3, [sp, #28]
	adds	r2, #1
	str	r2, [sp, #52]
	ldr	r2, [pc, #80]
	adds	r3, #1
	ldrb	r1, [r2, r3]
	ldr	r3, [pc, #68]
	add	r3, sl
	ldr	r3, [r3, #0]
	ldr	r2, [r3, #20]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r1, r1, r3
	ldr	r3, [sp, #52]
	cmp	r3, r1
	beq.n	.L8
	b.n	.L22
.L8:
	ldr	r0, [pc, #68]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	ldr	r0, [pc, #56]
	bl	Func_08004278
	bl	Func_080cdbc0
	add	sp, #84
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00007828
	.4byte 0x080ee090
	.4byte 0x02010000
	.4byte 0x00000fff
	.4byte 0x00001ffe
	.4byte 0x00007824
	.4byte 0x080cd261
	.4byte 0x080dbb9d
