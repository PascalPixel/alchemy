@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080936a0
	.thumb_func
Func_080936a0:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #88]
	adds	r6, r0, #0
	adds	r7, r1, #0
	movs	r0, #27
	ldr	r1, [pc, #84]
	ldr	r5, [r3, #0]
	bl	Func_080048f4
	movs	r1, #207
	lsls	r1, r1, #1
	adds	r3, r0, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #3
	bne.n	.L0
	movs	r1, #128
	ldr	r3, [pc, #64]
	lsls	r1, r1, #9
	adds	r0, r6, #0
	bl	Func_080072f0
	movs	r3, #212
	lsls	r3, r3, #2
	adds	r1, r5, r3
	adds	r3, #4
	adds	r2, r5, r3
	ldr	r3, [r2, #0]
	str	r3, [r1, #0]
	movs	r1, #214
	lsls	r1, r1, #2
	adds	r3, r5, r1
	adds	r1, #2
	str	r0, [r2, #0]
	strh	r7, [r3, #0]
	movs	r2, #0
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	ldr	r0, [pc, #24]
	ldr	r1, [pc, #28]
	bl	Func_080041d8
.L0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e70
	.4byte 0x00000ccc
	.4byte 0x0300013c
	.4byte 0x080935d5
	.4byte 0x00000c94
