@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08021be0
	.thumb_func
Func_08021be0:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	ldr	r5, [pc, #56]
	movs	r0, #49
	adds	r1, r5, #0
	bl	Func_080048b0
	movs	r2, #132
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	adds	r1, r0, #0
	ldr	r3, [pc, #44]
	ldr	r0, [pc, #44]
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #40]
	ldr	r1, [pc, #44]
	adds	r3, #196
	adds	r2, r6, r1
	ldr	r0, [r2, #0]
	ldr	r3, [r3, #0]
	adds	r1, r6, #0
	bl	Func_080072f0
	movs	r0, #49
	bl	Func_08002dd8
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00000278
	.4byte 0x040000d4
	.4byte 0x08015afc
	.4byte 0x03001e50
	.4byte 0x00000604
