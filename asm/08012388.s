@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08012388
	.thumb_func
Func_08012388:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	ldr	r6, [pc, #72]
	mov	r8, r0
	mov	sl, r1
	ldr	r5, [pc, #72]
	movs	r0, #49
	adds	r1, r5, #0
	bl	Func_080048b0
	movs	r2, #132
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	adds	r1, r0, #0
	ldr	r3, [pc, #56]
	ldr	r0, [pc, #60]
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #56]
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r6, r6, r1
	adds	r3, #196
	ldr	r4, [r3, #0]
	ldr	r2, [pc, #48]
	mov	r0, r8
	mov	r1, sl
	adds	r3, r6, #0
	bl	Func_080072f4
	movs	r0, #49
	bl	Func_08002dd8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x0201c000
	.4byte 0x0000027c
	.4byte 0x040000d4
	.4byte 0x08009e7c
	.4byte 0x03001e50
	.4byte 0x0203c000
