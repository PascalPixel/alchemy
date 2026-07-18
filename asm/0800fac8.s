@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800fac8
	.thumb_func
Func_0800fac8:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r5, #128
	lsls	r5, r5, #8
	adds	r0, r5, #0
	bl	Func_08004970
	ldr	r3, [pc, #68]
	ldr	r1, [pc, #72]
	adds	r2, r5, #0
	mov	r8, r0
	bl	Func_080072f0
	ldr	r5, [pc, #64]
	adds	r0, r5, #0
	bl	Func_08004938
	movs	r2, #132
	adds	r6, r0, #0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	ldr	r3, [pc, #52]
	ldr	r0, [pc, #56]
	adds	r1, r6, #0
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #48]
	ldr	r1, [pc, #32]
	mov	r2, r8
	bl	Func_080072fc
	adds	r0, r6, #0
	bl	Func_08002df0
	mov	r0, r8
	bl	Func_08002df0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x03001388
	.4byte 0x02010000
	.4byte 0x0000009c
	.4byte 0x040000d4
	.4byte 0x0800a37c
	.4byte 0x02018000
