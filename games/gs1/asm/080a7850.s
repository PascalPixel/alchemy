@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a7850
	.thumb_func
Func_080a7850:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #108]
	ldr	r3, [r3, #0]
	sub	sp, #28
	str	r3, [sp, #24]
	bl	Func_080a9d84
	movs	r0, #112
	bl	Func_080f9010
	movs	r1, #134
	ldr	r0, [sp, #24]
	lsls	r1, r1, #1
	adds	r5, r0, r1
	ldr	r0, [r5, #0]
	bl	Func_08015270
	ldr	r1, [r5, #0]
	ldr	r0, [pc, #76]
	movs	r2, #0
	movs	r3, #16
	bl	Func_08015080
	ldr	r2, [sp, #24]
	ldr	r3, [r2, #20]
	movs	r2, #13
	strb	r2, [r3, #5]
	ldr	r0, [sp, #24]
	movs	r1, #190
	lsls	r1, r1, #1
	adds	r3, r0, r1
	ldr	r3, [r3, #0]
	movs	r0, #1
	strb	r2, [r3, #5]
	bl	Func_080030f8
	ldr	r2, [sp, #24]
	ldr	r3, [sp, #24]
	ldr	r2, [r2, #36]
	movs	r0, #162
	movs	r1, #141
	lsls	r0, r0, #1
	lsls	r1, r1, #2
	ldr	r5, [pc, #16]
	ldr	r4, [pc, #16]
	mov	sl, r2
	adds	r2, r3, r0
	adds	r3, r3, r1
	movs	r0, #32
	movs	r1, #3
	b.n	.L0
	movs	r0, r0
	.4byte 0x00000046
	.4byte 0x0000001e
	.4byte 0x03001f2c
	.4byte 0x00000b17
.L0:
	subs	r1, #1
	strh	r0, [r3, #0]
	strh	r5, [r3, #8]
	adds	r0, #56
	strh	r4, [r2, #0]
	adds	r3, #2
	adds	r2, #2
	cmp	r1, #0
	bge.n	.L0
	mov	r0, sl
	bl	Func_08015270
	movs	r3, #11
	str	r3, [sp, #0]
	mov	r0, sl
	movs	r1, #0
	movs	r2, #11
	movs	r3, #28
	bl	Func_08015070
	movs	r1, #134
	ldr	r2, [sp, #24]
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r2, #96
	ldr	r1, [r3, #0]
	negs	r2, r2
	ldr	r0, [pc, #212]
	movs	r3, #132
	bl	Func_08015078
	movs	r2, #0
	str	r2, [sp, #12]
	str	r2, [sp, #8]
	str	r2, [sp, #4]
	mov	r9, r2
.L3:
	ldr	r3, [sp, #4]
	ldr	r0, [sp, #8]
	ldr	r1, [sp, #12]
	movs	r2, #24
	adds	r5, r3, #0
	str	r3, [sp, #20]
	str	r0, [sp, #16]
	movs	r6, #0
	mov	fp, r1
	mov	r8, r2
	adds	r5, #48
.L2:
	ldr	r3, [sp, #20]
	adds	r0, r5, #0
	adds	r7, r3, r6
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L1
	movs	r2, #0
	str	r2, [sp, #0]
	ldr	r1, [pc, #156]
	ldr	r2, [sp, #16]
	adds	r3, r6, #3
	mov	r0, sl
	add	r1, r9
	adds	r2, #1
	bl	Func_08015280
	ldr	r0, [pc, #144]
	mov	r2, fp
	adds	r0, r7, r0
	mov	r1, sl
	adds	r2, #16
	mov	r3, r8
	bl	Func_08015080
.L1:
	movs	r0, #8
	adds	r6, #1
	add	r8, r0
	adds	r5, #1
	cmp	r6, #6
	ble.n	.L2
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #4]
	movs	r0, #1
	adds	r1, #56
	add	r9, r0
	str	r1, [sp, #12]
	adds	r2, #7
	adds	r3, #20
	mov	r1, r9
	str	r2, [sp, #8]
	str	r3, [sp, #4]
	cmp	r1, #3
	ble.n	.L3
	ldr	r3, [pc, #92]
	ldr	r2, [pc, #96]
	ldr	r3, [r3, #0]
	adds	r3, r3, r2
	movs	r2, #1
	strb	r2, [r3, #0]
	ldr	r6, [pc, #88]
	movs	r5, #7
.L5:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L4
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [r6, #0]
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L5
.L4:
	ldr	r3, [sp, #24]
	ldr	r0, [r3, #36]
	bl	Func_08015270
	movs	r1, #134
	ldr	r0, [sp, #24]
	lsls	r1, r1, #1
	adds	r3, r0, r1
	ldr	r0, [r3, #0]
	bl	Func_08015278
	movs	r1, #141
	ldr	r2, [sp, #24]
	lsls	r1, r1, #2
	ldr	r0, [pc, #8]
	adds	r3, r2, r1
	movs	r1, #130
	movs	r2, #3
	b.n	.L6
	.4byte 0x00000080
	.4byte 0x00000b18
	.4byte 0x00001001
	.4byte 0x0000045f
	.4byte 0x03001e8c
	.4byte 0x00000ea3
	.4byte 0x03001c94
.L6:
	subs	r2, #1
	strh	r1, [r3, #0]
	strh	r0, [r3, #8]
	adds	r1, #32
	adds	r3, #2
	cmp	r2, #0
	bge.n	.L6
	ldr	r2, [sp, #24]
	ldr	r3, [r2, #20]
	movs	r2, #1
	strb	r2, [r3, #5]
	ldr	r0, [sp, #24]
	movs	r1, #190
	lsls	r1, r1, #1
	adds	r3, r0, r1
	ldr	r3, [r3, #0]
	movs	r0, #113
	strb	r2, [r3, #5]
	bl	Func_080f9010
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
