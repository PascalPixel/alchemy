@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080c08ec
	.thumb_func
Func_080c08ec:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r7, r2, #0
	ldr	r2, [pc, #212]
	ldr	r2, [r2, #0]
	mov	r9, r0
	adds	r0, r1, #0
	mov	sl, r2
	bl	Func_08002f40
	ldr	r3, [pc, #200]
	subs	r3, #140
	ldr	r6, [r3, #0]
	mov	r8, r0
	ldr	r5, [pc, #196]
	movs	r0, #49
	adds	r1, r5, #0
	bl	Func_080048b0
	movs	r2, #132
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	adds	r1, r0, #0
	ldr	r3, [pc, #180]
	ldr	r0, [pc, #184]
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #128
	ldr	r2, [pc, #160]
	lsls	r0, r0, #1
	ldr	r3, [r2, #20]
	ldr	r1, [pc, #172]
	add	r0, r8
	bl	Func_080072f0
	movs	r0, #49
	bl	Func_08002dd8
	ldr	r3, [pc, #160]
	adds	r4, r6, r3
	mov	r0, r8
	ldr	r3, [pc, #144]
	adds	r1, r4, #0
	ldr	r2, [pc, #156]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	cmp	r7, #0
	blt.n	.L0
	lsls	r3, r7, #4
	ldr	r2, [pc, #148]
	adds	r3, r3, r7
	lsls	r3, r3, #4
	adds	r0, r6, r2
	adds	r3, r3, r7
	movs	r2, #128
	lsls	r3, r3, #2
	lsls	r2, r2, #9
	subs	r2, r2, r3
	str	r2, [r0, #0]
	ldr	r1, [pc, #132]
	adds	r0, r4, #0
	movs	r3, #128
	bl	Func_080c1724
.L0:
	ldr	r3, [pc, #96]
	ldr	r0, [pc, #124]
	ldr	r1, [pc, #124]
	ldr	r2, [pc, #128]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #124]
	ldr	r2, [pc, #128]
	ldrh	r3, [r3, #0]
	ldr	r0, [pc, #128]
	strh	r3, [r2, #0]
	bl	Func_080c0098
	ldr	r0, [pc, #124]
	bl	Func_080c00d8
	ldr	r3, [pc, #120]
	ldr	r0, [pc, #124]
	movs	r1, #64
	bl	Func_080072f0
	mov	r2, sl
	ldr	r3, [r2, #8]
	cmp	r3, #0
	bne.n	.L1
	ldr	r0, [pc, #112]
	ldr	r1, [pc, #112]
	bl	Func_080041d8
.L1:
	mov	r3, r9
	mov	r2, sl
	str	r3, [r2, #8]
	cmp	r3, #1
	bne.n	.L2
	ldr	r2, [pc, #100]
	ldr	r3, [pc, #16]
	strh	r3, [r2, #0]
.L2:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00001f83
	.4byte 0x03001f00
	.4byte 0x00000230
	.4byte 0x040000d4
	.4byte 0x080b5138
	.4byte 0x06008000
	.4byte 0x00000544
	.4byte 0x84000040
	.4byte 0x00000644
	.4byte 0x050000c0
	.4byte 0x05000200
	.4byte 0x050000a0
	.4byte 0x80000010
	.4byte 0x050001e8
	.4byte 0x050000bc
	.4byte 0x06003800
	.4byte 0x0600f800
	.4byte 0x03000164
	.4byte 0x0600ffc0
	.4byte 0x080c0131
	.4byte 0x000004ff
	.4byte 0x0400000a
