@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08028c04
	.thumb_func
Func_08028c04:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	sl, r0
	sub	sp, #4
	bl	Func_080284dc
	ldr	r3, [pc, #320]
	ldr	r7, [r3, #0]
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L0
	movs	r0, #44
	bl	Func_080287a8
	movs	r0, #45
	bl	Func_080287a8
	b.n	.L1
.L0:
	movs	r0, #46
	bl	Func_080287a8
	movs	r0, #47
	bl	Func_080287a8
	movs	r0, #48
	bl	Func_080287a8
.L1:
	movs	r0, #17
	movs	r1, #7
	movs	r2, #0
	bl	Func_08028808
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L2
	ldr	r1, [pc, #268]
	ldr	r0, [pc, #268]
	bl	Func_080041d8
	ldr	r3, [pc, #268]
	adds	r2, r7, #0
	adds	r2, #150
	strh	r3, [r2, #0]
	movs	r6, #2
	movs	r1, #0
	movs	r2, #17
	movs	r3, #4
	movs	r0, #7
	str	r6, [sp, #0]
	bl	Func_080162d4
	ldr	r5, [pc, #248]
	movs	r3, #128
	adds	r1, r0, #0
	adds	r3, r3, r7
	str	r1, [r3, #0]
	adds	r0, r5, #0
	movs	r2, #0
	mov	r8, r3
	movs	r3, #4
	bl	Func_0801e74c
	movs	r1, #4
	movs	r2, #25
	movs	r3, #12
	movs	r0, #3
	str	r6, [sp, #0]
	bl	Func_080162d4
	adds	r1, r0, #0
	str	r1, [r7, #124]
	adds	r0, r5, #1
	movs	r2, #8
	movs	r3, #0
	bl	Func_0801e74c
	adds	r0, r5, #2
	ldr	r1, [r7, #124]
	movs	r2, #8
	movs	r3, #11
	adds	r5, #3
	bl	Func_0801e74c
	ldr	r1, [r7, #124]
	adds	r0, r5, #0
	movs	r2, #8
	movs	r3, #22
	bl	Func_0801e74c
	b.n	.L3
.L2:
	ldr	r1, [pc, #156]
	ldr	r0, [pc, #172]
	bl	Func_080041d8
	ldr	r3, [pc, #156]
	adds	r2, r7, #0
	adds	r2, #150
	strh	r3, [r2, #0]
	movs	r5, #2
	movs	r1, #0
	movs	r2, #18
	movs	r3, #4
	movs	r0, #6
	str	r5, [sp, #0]
	bl	Func_080162d4
	movs	r3, #128
	adds	r1, r0, #0
	adds	r3, r3, r7
	str	r1, [r3, #0]
	ldr	r0, [pc, #136]
	movs	r2, #2
	mov	r8, r3
	movs	r3, #4
	bl	Func_0801e74c
	movs	r0, #1
	movs	r1, #5
	movs	r2, #28
	movs	r3, #7
	str	r5, [sp, #0]
	bl	Func_080162d4
	str	r0, [r7, #124]
.L3:
	movs	r0, #0
	bl	Func_08028574
	mov	r3, sl
	adds	r5, r0, #0
	cmp	r3, #0
	beq.n	.L4
	ldr	r0, [pc, #80]
	bl	Func_08004278
	b.n	.L5
.L4:
	ldr	r0, [pc, #84]
	bl	Func_08004278
.L5:
	mov	r3, r8
	ldr	r0, [r3, #0]
	bl	Func_08016478
	ldr	r0, [r7, #124]
	bl	Func_08016478
	mov	r3, r8
	ldr	r0, [r3, #0]
	movs	r1, #2
	bl	Func_08016418
	movs	r1, #2
	ldr	r0, [r7, #124]
	bl	Func_08016418
	movs	r0, #1
	bl	Func_080030f8
	bl	Func_0802851c
	adds	r0, r5, #0
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f38
	.4byte 0x00000c76
	.4byte 0x08028aa9
	.4byte 0x0000ffff
	.4byte 0x00000c77
	.4byte 0x08028b81
	.4byte 0x00000c76
