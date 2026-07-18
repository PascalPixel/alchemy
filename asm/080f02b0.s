@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080f02b0
	.thumb_func
Func_080f02b0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r1, #0
	cmp	r0, #0
	bne.n	.L0
	adds	r0, r5, #0
	bl	Func_080f0254
	b.n	.L1
.L0:
	bl	Func_08002f40
	adds	r6, r0, #0
	cmp	r5, #0
	bne.n	.L2
	movs	r3, #160
	movs	r2, #0
	movs	r7, #192
	lsls	r3, r3, #19
	mov	r8, r2
	lsls	r7, r7, #19
	b.n	.L3
.L2:
	ldr	r2, [pc, #116]
	ldr	r3, [pc, #116]
	ldr	r7, [pc, #120]
	mov	r8, r2
.L3:
	mov	sl, r3
	ldr	r5, [pc, #116]
	movs	r0, #49
	adds	r1, r5, #0
	bl	Func_080048b0
	movs	r2, #132
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	adds	r1, r0, #0
	ldr	r3, [pc, #104]
	ldr	r0, [pc, #104]
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #100]
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r3, #196
	adds	r1, r7, #0
	adds	r0, r6, r2
	ldr	r3, [r3, #0]
	mov	r2, r8
	bl	Func_080072f0
	movs	r0, #49
	bl	Func_08002dd8
	ldr	r1, [pc, #80]
	ldr	r0, [pc, #80]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L4
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r1
	adds	r3, #4
	adds	r2, #1
	stmia	r3!, {r6}
	strh	r2, [r1, #0]
	mov	r2, sl
	stmia	r3!, {r2}
	ldr	r2, [pc, #52]
	str	r2, [r3, #0]
.L4:
	strh	r4, [r0, #0]
.L1:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x80808080
	.4byte 0x05000100
	.4byte 0x06008000
	.4byte 0x00000230
	.4byte 0x040000d4
	.4byte 0x080f0024
	.4byte 0x03001e50
	.4byte 0x02002090
	.4byte 0x04000208
	.4byte 0x84000040
