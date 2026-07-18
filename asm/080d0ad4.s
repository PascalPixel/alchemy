@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080d0ad4
	.thumb_func
Func_080d0ad4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r1, [pc, #104]
	mov	r8, r1
	mov	r3, r8
	ldmia	r3!, {r2}
	sub	sp, #120
	str	r2, [sp, #68]
	ldr	r3, [r3, #0]
	str	r3, [sp, #64]
	mov	r3, r8
	ldr	r4, [pc, #88]
	ldr	r6, [r3, #8]
	subs	r3, #108
	ldr	r3, [r3, #0]
	adds	r5, r2, r4
	str	r3, [sp, #44]
	str	r0, [r5, #0]
	movs	r0, #0
	bl	Func_080cd594
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #60]
	ldr	r0, [pc, #72]
	strh	r3, [r2, #0]
	bl	Func_08002f40
	adds	r7, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #64]
	movs	r2, #128
	adds	r1, r7, #0
	lsls	r0, r0, #19
	bl	Func_080072f0
	adds	r7, #128
	ldr	r1, [sp, #68]
	adds	r0, r7, #0
	bl	Func_08005340
	ldr	r0, [pc, #48]
	bl	Func_08002f40
	adds	r1, r6, #0
	bl	Func_08005340
	ldr	r0, [pc, #40]
	bl	Func_08002f40
	movs	r3, #128
	ldr	r2, [sp, #68]
	lsls	r3, r3, #5
	b.n	.L0
	.4byte 0x00000100
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000020
	.4byte 0x00000079
	.4byte 0x03001388
	.4byte 0x00000073
	.4byte 0x00000076
.L0:
	adds	r1, r2, r3
	adds	r7, r0, #0
	bl	Func_08005340
	movs	r1, #239
	ldr	r4, [sp, #68]
	lsls	r1, r1, #7
	adds	r2, r4, r1
	movs	r3, #3
	str	r3, [r2, #0]
	ldr	r3, [pc, #808]
	adds	r2, r4, r3
	ldr	r3, [pc, #808]
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #804]
	bl	Func_080041d8
	ldr	r3, [r5, #0]
	mov	r1, sp
	movs	r4, #36
	ldrsh	r0, [r3, r4]
	adds	r1, #108
	str	r1, [sp, #36]
	bl	Func_080e396c
	ldr	r3, [sp, #36]
	ldr	r2, [r3, #0]
	movs	r3, #64
	subs	r3, r3, r2
	ldr	r2, [pc, #780]
	str	r3, [sp, #40]
	lsls	r3, r3, #8
	str	r3, [r2, #0]
	movs	r0, #142
	bl	Func_080f9010
	movs	r4, #0
	str	r4, [sp, #60]
	ldr	r3, [r5, #0]
	ldr	r2, [r3, #20]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	movs	r5, #72
	lsls	r3, r3, #2
	negs	r5, r5
	cmp	r3, r5
	bne.n	.L1
	b.n	.L2
.L1:
	ldr	r1, [sp, #60]
	cmp	r1, #64
	bne.n	.L3
	movs	r0, #0
	bl	Func_080b50e8
.L3:
	ldr	r2, [sp, #60]
	cmp	r2, #46
	bne.n	.L4
	ldr	r4, [sp, #68]
	ldr	r5, [pc, #724]
	adds	r3, r4, r5
	ldr	r3, [r3, #0]
	movs	r2, #36
	ldrsh	r1, [r3, r2]
	ldr	r0, [r3, #8]
	movs	r2, #16
	movs	r3, #0
	bl	Func_080b5078
.L4:
	ldr	r0, [sp, #60]
	ldr	r1, [pc, #708]
	ldr	r2, [pc, #708]
	movs	r3, #0
	bl	Func_080d40ec
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	Func_080ed408
	ldr	r3, [pc, #688]
	adds	r3, #184
	ldr	r3, [r3, #0]
	movs	r0, #47
	str	r3, [sp, #52]
	movs	r1, #7
	movs	r3, #7
	movs	r2, #7
	str	r5, [sp, #0]
	bl	Func_080ed408
	ldr	r3, [pc, #672]
	ldr	r4, [sp, #60]
	ldr	r3, [r3, #0]
	str	r3, [sp, #56]
	cmp	r4, #16
	ble.n	.L5
	movs	r3, #15
	ands	r3, r4
	cmp	r3, #0
	bne.n	.L5
	ldr	r5, [sp, #68]
	ldr	r1, [pc, #616]
	adds	r2, r5, r1
	ldr	r3, [r2, #0]
	ldr	r4, [pc, #648]
	adds	r3, r3, r4
	str	r3, [r2, #0]
.L5:
	ldr	r1, [sp, #60]
	lsls	r3, r1, #1
	adds	r3, r3, r1
	movs	r5, #0
	lsls	r3, r3, #9
	str	r5, [sp, #48]
	str	r5, [sp, #16]
	str	r3, [sp, #12]
	mov	fp, r1
.L16:
	ldr	r2, [sp, #68]
	ldr	r4, [pc, #600]
	ldr	r5, [sp, #48]
	adds	r3, r2, r4
	ldr	r2, [r3, #0]
	lsls	r3, r5, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	bl	Func_080b5098
	mov	r2, fp
	ldr	r6, [r0, #0]
	cmp	r2, #95
	bls.n	.L6
	b.n	.L7
.L6:
	bl	Func_080049ac
	ldr	r0, [sp, #44]
	adds	r1, r0, #0
	adds	r1, #12
	bl	Func_080051d8
	ldr	r3, [r6, #8]
	add	r5, sp, #96
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	add	r3, sp, #84
	mov	sl, r3
	mov	r1, sl
	adds	r0, r5, #0
	bl	Func_080e3944
	ldr	r4, [sp, #36]
	ldr	r1, [sp, #40]
	ldr	r3, [r4, #0]
	mov	r2, sl
	adds	r3, r3, r1
	str	r3, [r2, #0]
	ldr	r3, [r2, #4]
	subs	r3, #24
	str	r3, [r2, #4]
	mov	r3, fp
	cmp	r3, #67
	ble.n	.L8
	b.n	.L9
.L8:
	ldr	r5, [sp, #16]
	lsls	r2, r5, #3
	subs	r2, r2, r5
	ldr	r1, [sp, #68]
	movs	r4, #0
	lsls	r2, r2, #2
	adds	r2, r2, r1
	movs	r3, #168
	str	r4, [sp, #20]
	ldr	r1, [sp, #12]
	movs	r4, #225
	lsls	r3, r3, #10
	lsls	r4, r4, #7
	subs	r1, r3, r1
	adds	r5, r2, r4
	movs	r3, #64
	mov	r2, fp
	subs	r3, r3, r2
	lsls	r3, r3, #9
	movs	r7, #0
	add	r6, sp, #72
	mov	r8, r1
	mov	r9, r3
.L11:
	bl	Func_080049ac
	mov	r3, fp
	cmp	r3, #63
	bgt.n	.L10
	mov	r4, r8
	str	r4, [r6, #0]
	str	r4, [r6, #4]
	str	r4, [r6, #8]
	adds	r0, r6, #0
	bl	Func_08004cf0
	mov	r0, r9
	bl	Func_08004c6c
	mov	r0, r9
	bl	Func_08004c1c
.L10:
	ldr	r0, [sp, #20]
	bl	Func_08004c6c
	add	r2, sp, #96
	adds	r1, r2, #0
	ldr	r0, [pc, #440]
	bl	Func_080e3944
	mov	r4, sl
	ldr	r3, [r4, #0]
	ldr	r2, [sp, #96]
	adds	r2, r2, r3
	str	r2, [r5, #12]
	ldr	r3, [sp, #100]
	ldr	r2, [r4, #4]
	adds	r3, r3, r2
	adds	r3, #16
	str	r3, [r5, #16]
	ldr	r1, [sp, #20]
	ldr	r2, [pc, #396]
	adds	r7, #1
	adds	r1, r1, r2
	str	r1, [sp, #20]
	adds	r5, #28
	cmp	r7, #3
	bne.n	.L11
	ldr	r3, [sp, #16]
	str	r3, [sp, #28]
	movs	r7, #0
.L14:
	ldr	r4, [sp, #28]
	adds	r2, r7, r4
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldr	r5, [sp, #68]
	lsls	r3, r3, #2
	movs	r1, #225
	lsls	r1, r1, #7
	adds	r7, #1
	adds	r3, r5, r3
	adds	r3, r3, r1
	adds	r0, r7, #0
	movs	r1, #3
	str	r3, [sp, #32]
	str	r7, [sp, #24]
	bl	Func_080022fc
	ldr	r2, [sp, #28]
	adds	r0, r0, r2
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsls	r3, r3, #2
	movs	r4, #225
	adds	r3, r5, r3
	lsls	r4, r4, #7
	adds	r4, r4, r3
	mov	r2, fp
	mov	r9, r4
	cmp	r2, #0
	bge.n	.L12
	adds	r2, #15
.L12:
	asrs	r2, r2, #4
	movs	r3, #5
	subs	r4, r3, r2
	movs	r5, #0
	mov	r8, r5
	lsls	r7, r4, #1
.L13:
	ldr	r2, [sp, #32]
	mov	r1, r9
	ldr	r6, [r2, #12]
	ldr	r3, [r1, #12]
	subs	r3, r3, r6
	mov	r0, r8
	muls	r0, r3
	movs	r1, #24
	str	r4, [sp, #8]
	bl	Func_080022ec
	ldr	r1, [sp, #32]
	mov	r5, r9
	ldr	r3, [r5, #16]
	ldr	r5, [r1, #16]
	subs	r3, r3, r5
	adds	r6, r6, r0
	movs	r1, #24
	mov	r0, r8
	muls	r0, r3
	bl	Func_080022ec
	ldr	r2, [pc, #276]
	subs	r3, r7, #2
	ldrh	r1, [r2, r3]
	ldr	r4, [sp, #8]
	ldr	r3, [sp, #68]
	adds	r5, r5, r0
	movs	r2, #128
	subs	r5, r5, r4
	subs	r6, r6, r4
	adds	r1, r3, r1
	lsls	r2, r2, #5
	adds	r1, r1, r2
	adds	r3, r5, #0
	adds	r2, r6, #0
	str	r7, [sp, #0]
	str	r7, [sp, #4]
	ldr	r0, [sp, #64]
	ldr	r5, [sp, #52]
	bl	Func_080072f8
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	ldr	r4, [sp, #8]
	cmp	r2, #24
	bne.n	.L13
	ldr	r7, [sp, #24]
	cmp	r7, #3
	bne.n	.L14
.L9:
	mov	r3, fp
	cmp	r3, #63
	ble.n	.L7
	mov	r4, sl
	ldr	r2, [r4, #0]
	ldr	r3, [r4, #4]
	movs	r5, #24
	str	r5, [sp, #0]
	movs	r5, #48
	subs	r2, #24
	subs	r3, #24
	str	r5, [sp, #4]
	ldr	r1, [sp, #68]
	ldr	r4, [sp, #52]
	ldr	r0, [sp, #64]
	bl	Func_080072f4
	mov	r1, sl
	ldr	r3, [r1, #4]
	movs	r4, #24
	ldr	r2, [r1, #0]
	subs	r3, #24
	str	r5, [sp, #4]
	str	r4, [sp, #0]
	ldr	r0, [sp, #64]
	ldr	r1, [sp, #68]
	ldr	r5, [sp, #56]
	bl	Func_080072f8
.L7:
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #12]
	ldr	r5, [sp, #48]
	ldr	r3, [pc, #160]
	movs	r4, #8
	adds	r1, #32
	adds	r2, r2, r3
	negs	r4, r4
	adds	r5, #1
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	add	fp, r4
	str	r5, [sp, #48]
	cmp	r5, #1
	beq.n	.L15
	b.n	.L16
.L15:
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	ldr	r2, [pc, #124]
	ldr	r1, [sp, #68]
	adds	r3, r1, r2
	str	r5, [r3, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [sp, #60]
	ldr	r4, [sp, #68]
	adds	r3, #1
	ldr	r5, [pc, #68]
	str	r3, [sp, #60]
	adds	r3, r4, r5
	ldr	r3, [r3, #0]
	ldr	r2, [r3, #20]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #2
	ldr	r1, [sp, #60]
	adds	r3, #72
	cmp	r1, r3
	beq.n	.L2
	b.n	.L1
.L2:
	ldr	r0, [pc, #36]
	bl	Func_08004278
	bl	Func_080cdbc0
	add	sp, #120
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00007784
	.4byte 0x04040404
	.4byte 0x080cd261
	.4byte 0x04000028
	.4byte 0x00007828
	.4byte 0x0000aaab
	.4byte 0x00005555
	.4byte 0x03001e50
	.4byte 0x03001f0c
	.4byte 0x01010101
	.4byte 0x080ee134
	.4byte 0x080ede5c
	.4byte 0xffffd000
	.4byte 0x00007824
