@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801a5a4
	.thumb_func
Func_0801a5a4:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	adds	r6, r1, #0
	ldr	r5, [pc, #152]
	movs	r0, #49
	adds	r1, r5, #0
	bl	Func_080048b0
	movs	r2, #132
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	adds	r1, r0, #0
	mov	r8, r2
	ldr	r3, [pc, #136]
	ldr	r0, [pc, #136]
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #132]
	ldr	r4, [pc, #136]
	adds	r3, #196
	adds	r2, r7, r4
	ldr	r0, [r2, #0]
	ldr	r3, [r3, #0]
	adds	r1, r7, #0
	bl	Func_080072f0
	movs	r0, #49
	bl	Func_08002dd8
	cmp	r6, #0
	beq.n	.L0
	ldr	r5, [pc, #112]
	movs	r0, #49
	adds	r1, r5, #0
	bl	Func_080048b0
	lsrs	r5, r5, #2
	mov	r2, r8
	adds	r1, r0, #0
	ldr	r3, [pc, #80]
	ldr	r0, [pc, #100]
	b.n	.L1
.L0:
	ldr	r5, [pc, #100]
	movs	r0, #49
	adds	r1, r5, #0
	bl	Func_080048b0
	lsrs	r5, r5, #2
	mov	r2, r8
	adds	r1, r0, #0
	ldr	r3, [pc, #60]
	ldr	r0, [pc, #84]
.L1:
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r4, #192
	ldr	r0, [pc, #56]
	lsls	r4, r4, #3
	movs	r2, #128
	adds	r3, r7, r4
	lsls	r2, r2, #3
	adds	r4, #2
	adds	r1, r7, r2
	adds	r0, #196
	ldrh	r2, [r3, #0]
	adds	r3, r7, r4
	ldrh	r3, [r3, #0]
	ldr	r4, [r0, #0]
	adds	r0, r7, #0
	bl	Func_080072f4
	movs	r0, #49
	bl	Func_08002dd8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00000278
	.4byte 0x040000d4
	.4byte 0x08015afc
	.4byte 0x03001e50
	.4byte 0x00000604
	.4byte 0x0000009c
	.4byte 0x08015d74
	.4byte 0x0000007c
	.4byte 0x08015e10
