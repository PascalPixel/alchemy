@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080d59b0
	.thumb_func
Func_080d59b0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r6, [pc, #92]
	adds	r3, r6, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #40
	str	r3, [sp, #24]
	ldr	r3, [pc, #84]
	mov	r9, r1
	add	r3, r9
	str	r0, [r3, #0]
	movs	r0, #0
	bl	Func_080cd594
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	adds	r2, #48
	strh	r3, [r2, #0]
	ldr	r0, [pc, #64]
	mov	r1, r9
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	movs	r5, #1
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	Func_080ed408
	ldr	r2, [r6, #28]
	movs	r0, #47
	str	r2, [sp, #12]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #15
	str	r5, [sp, #0]
	bl	Func_080ed408
	b.n	.L0
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000020
	.4byte 0x000000a8
.L0:
	ldr	r6, [r6, #32]
	ldr	r5, [pc, #488]
	str	r6, [sp, #16]
	movs	r7, #0
.L1:
	bl	Func_08004458
	movs	r3, #63
	ands	r3, r0
	adds	r3, #32
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	ldr	r3, [pc, #472]
	movs	r6, #0
	str	r3, [r5, #4]
	bl	Func_08004458
	str	r6, [r5, #16]
	bl	Func_08004458
	movs	r3, #3
	ands	r3, r0
	str	r3, [r5, #8]
	bl	Func_08004458
	movs	r3, #255
	ands	r3, r0
	adds	r7, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #32
	bne.n	.L1
	ldr	r3, [pc, #436]
	add	r3, r9
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L2
	ldr	r2, [pc, #428]
	ldr	r3, [pc, #428]
	str	r3, [r2, #0]
.L2:
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r3, [pc, #420]
	movs	r1, #144
	add	r3, r9
	str	r6, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #412]
	bl	Func_080041d8
	movs	r0, #142
	bl	Func_080f9010
	movs	r3, #0
	str	r3, [sp, #20]
.L13:
	ldr	r3, [pc, #400]
	ldr	r4, [sp, #20]
	ldr	r3, [r3, #0]
	mov	fp, r3
	cmp	r4, #80
	bne.n	.L3
	movs	r0, #0
	bl	Func_080b50e8
.L3:
	ldr	r2, [pc, #364]
	mov	r1, r9
	ldr	r3, [r1, r2]
	ldr	r3, [r3, #20]
	movs	r0, #0
	mov	sl, r0
	cmp	r3, #0
	beq.n	.L4
	mov	r3, fp
	adds	r3, #12
	movs	r4, #36
	str	r3, [sp, #8]
	add	r6, sp, #28
	mov	r8, r4
	adds	r7, r1, r2
.L6:
	ldr	r3, [r7, #0]
	mov	r1, r8
	ldrsh	r0, [r3, r1]
	bl	Func_080b5098
	ldr	r5, [r0, #0]
	bl	Func_080049ac
	mov	r0, fp
	ldr	r1, [sp, #8]
	bl	Func_080051d8
	ldr	r3, [r5, #8]
	str	r3, [r6, #0]
	movs	r3, #160
	lsls	r3, r3, #14
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	adds	r0, r6, #0
	str	r3, [r6, #8]
	bl	Func_08004cb4
	mov	r4, sl
	lsls	r3, r4, #4
	ldr	r0, [sp, #20]
	adds	r3, #64
	cmp	r0, r3
	bne.n	.L5
	ldr	r3, [r7, #0]
	mov	r1, r8
	ldrsh	r0, [r3, r1]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #5
	subs	r3, #1
	bl	Func_080d6888
.L5:
	movs	r3, #2
	add	r8, r3
	ldr	r3, [r7, #0]
	movs	r4, #1
	ldr	r3, [r3, #20]
	add	sl, r4
	cmp	sl, r3
	bne.n	.L6
.L4:
	movs	r0, #32
	ldr	r6, [pc, #232]
	movs	r7, #0
	mov	r8, r0
.L11:
	ldr	r1, [sp, #20]
	lsls	r3, r7, #2
	cmp	r1, r3
	ble.n	.L7
	ldr	r0, [r6, #4]
	ldr	r2, [pc, #248]
	cmp	r0, r2
	bgt.n	.L7
	ldr	r1, [r6, #24]
	cmp	r1, #0
	bge.n	.L8
	adds	r1, #15
.L8:
	asrs	r1, r1, #4
	movs	r3, #7
	ands	r1, r3
	cmp	r1, #3
	bgt.n	.L9
	movs	r3, #2
	ldrsh	r2, [r6, r3]
	mov	r4, r8
	asrs	r3, r0, #16
	lsls	r1, r1, #10
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	add	r1, r9
	subs	r2, #16
	subs	r3, #16
	ldr	r0, [sp, #24]
	ldr	r4, [sp, #12]
	bl	Func_080072f4
	b.n	.L10
.L9:
	ldr	r2, [pc, #196]
	lsls	r1, r1, #10
	add	r1, r9
	adds	r1, r1, r2
	movs	r3, #2
	ldrsh	r2, [r6, r3]
	mov	r4, r8
	asrs	r3, r0, #16
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	subs	r2, #16
	subs	r3, #16
	ldr	r0, [sp, #24]
	ldr	r4, [sp, #16]
	bl	Func_080072f4
.L10:
	ldr	r4, [r6, #16]
	movs	r0, #128
	lsls	r0, r0, #6
	ldr	r2, [r6, #4]
	adds	r1, r4, r0
	ldr	r3, [r6, #24]
	ldr	r0, [r6, #8]
	movs	r5, #184
	adds	r2, r2, r4
	adds	r3, r3, r0
	lsls	r5, r5, #15
	str	r2, [r6, #4]
	str	r1, [r6, #16]
	str	r3, [r6, #24]
	cmp	r2, r5
	ble.n	.L7
	cmp	r1, #0
	bne.n	.L7
	ldr	r1, [pc, #132]
	adds	r3, r4, r1
	adds	r2, r0, #4
	negs	r3, r3
	str	r2, [r6, #8]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r5, [r6, #4]
	str	r3, [r6, #16]
.L7:
	adds	r7, #1
	adds	r6, #28
	cmp	r7, #12
	bne.n	.L11
	ldr	r2, [pc, #108]
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r2, [sp, #20]
	adds	r2, #1
	str	r2, [sp, #20]
	cmp	r2, #148
	beq.n	.L12
	b.n	.L13
.L12:
	ldr	r0, [pc, #64]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0xffe00000
	.4byte 0x00007828
	.4byte 0x04000028
	.4byte 0xffff9000
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x03001e80
	.4byte 0x007fffff
	.4byte 0xfffff000
	.4byte 0x00002001
	.4byte 0x00007824
