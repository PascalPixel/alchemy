@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080dab74
	.thumb_func
Func_080dab74:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #92]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	sub	sp, #96
	str	r1, [sp, #48]
	ldr	r3, [r3, #0]
	str	r3, [sp, #44]
	ldr	r2, [r2, #8]
	str	r2, [sp, #36]
	ldr	r2, [pc, #76]
	adds	r3, r1, r2
	str	r0, [r3, #0]
	movs	r0, #1
	bl	Func_080cd594
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r1, [sp, #48]
	ldr	r0, [pc, #64]
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	ldr	r0, [pc, #60]
	ldr	r1, [sp, #36]
	movs	r2, #0
	movs	r3, #0
	bl	Func_080e0524
	mov	r3, sp
	adds	r3, #52
	movs	r0, #0
	adds	r1, r3, #0
	str	r3, [sp, #32]
	bl	Func_080cef64
	movs	r1, #225
	ldr	r0, [sp, #48]
	movs	r6, #0
	lsls	r1, r1, #7
	mov	sl, r6
	adds	r5, r0, r1
	b.n	.L0
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000020
	.4byte 0x000000b8
	.4byte 0x000000ba
.L0:
	bl	Func_08004458
	ldr	r3, [pc, #808]
	ands	r3, r0
	str	r3, [r5, #0]
	bl	Func_08004458
	movs	r3, #63
	ands	r3, r0
	adds	r3, #56
	str	r3, [r5, #8]
	bl	Func_08004458
	movs	r3, #31
	ands	r3, r0
	subs	r3, #64
	movs	r2, #1
	lsls	r3, r3, #16
	add	sl, r2
	str	r3, [r5, #4]
	mov	r3, sl
	adds	r5, #28
	cmp	r3, #64
	bne.n	.L0
	ldr	r6, [sp, #48]
	movs	r0, #239
	lsls	r0, r0, #7
	ldr	r1, [pc, #764]
	adds	r2, r6, r0
	movs	r3, #2
	str	r3, [r2, #0]
	adds	r2, r6, r1
	movs	r3, #50
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #748]
	bl	Func_080041d8
	ldr	r2, [pc, #748]
	adds	r3, r6, r2
	ldr	r1, [r3, #0]
	ldr	r3, [r1, #4]
	cmp	r3, #1
	bne.n	.L1
	ldr	r2, [pc, #740]
	ldr	r3, [pc, #740]
	str	r3, [r2, #0]
.L1:
	movs	r3, #0
	str	r3, [sp, #40]
	ldr	r3, [r1, #24]
	lsls	r2, r3, #1
	adds	r2, r2, r3
	ldr	r0, [pc, #732]
	adds	r2, #2
	movs	r6, #75
	ldrb	r3, [r0, r2]
	negs	r6, r6
	cmp	r3, r6
	bne.n	.L2
	b.n	.L3
.L2:
	ldr	r3, [pc, #704]
	ldr	r2, [sp, #48]
	mov	r1, sp
	adds	r1, #60
	adds	r3, r2, r3
	str	r1, [sp, #8]
	str	r3, [sp, #16]
.L45:
	movs	r6, #240
	lsls	r6, r6, #15
	movs	r1, #0
	str	r6, [sp, #24]
	str	r1, [sp, #20]
	ldr	r2, [sp, #16]
	ldr	r3, [r2, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r3, #2
	ldrb	r3, [r0, r3]
	ldr	r6, [sp, #40]
	adds	r3, #11
	cmp	r6, r3
	bne.n	.L4
	movs	r0, #132
	bl	Func_080b50e8
.L4:
	ldr	r0, [sp, #20]
	ldr	r1, [sp, #8]
	movs	r3, #128
	lsls	r3, r3, #18
	str	r0, [r1, #0]
	str	r0, [r1, #4]
	str	r3, [r1, #8]
	bl	Func_080049ac
	ldr	r0, [sp, #8]
	bl	Func_08004cb4
	ldr	r2, [sp, #40]
	subs	r2, #36
	str	r2, [sp, #12]
	cmp	r2, #27
	bhi.n	.L5
	ldr	r6, [sp, #40]
	movs	r3, #3
	ands	r3, r6
	cmp	r3, #0
	bne.n	.L5
	movs	r0, #115
	bl	Func_080f9010
.L5:
	ldr	r0, [sp, #40]
	cmp	r0, #85
	bne.n	.L6
	movs	r0, #136
	bl	Func_080f9010
.L6:
	ldr	r2, [pc, #588]
	ldr	r6, [sp, #48]
	ldr	r3, [r6, r2]
	ldr	r3, [r3, #20]
	movs	r1, #0
	mov	sl, r1
	cmp	r3, #0
	beq.n	.L7
	movs	r6, #36
	movs	r5, #40
.L9:
	ldr	r0, [sp, #40]
	cmp	r0, r5
	bne.n	.L8
	ldr	r1, [sp, #48]
	ldr	r3, [r1, r2]
	ldrsh	r0, [r3, r6]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #9
	movs	r2, #5
	subs	r3, #1
	bl	Func_080d6888
.L8:
	ldr	r2, [pc, #544]
	ldr	r0, [sp, #48]
	movs	r3, #1
	add	sl, r3
	ldr	r3, [r0, r2]
	ldr	r3, [r3, #20]
	adds	r6, #2
	adds	r5, #4
	cmp	sl, r3
	bne.n	.L9
.L7:
	ldr	r2, [sp, #16]
	ldr	r3, [r2, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	ldr	r1, [pc, #528]
	adds	r2, r3, r2
	adds	r3, r2, #2
	ldrb	r3, [r1, r3]
	ldr	r0, [sp, #40]
	movs	r6, #16
	mov	r9, r1
	str	r6, [sp, #28]
	cmp	r0, r3
	bge.n	.L10
	ldrb	r2, [r1, r2]
	str	r2, [sp, #28]
.L10:
	ldr	r2, [sp, #16]
	ldr	r3, [r2, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r3, #2
	ldrb	r3, [r1, r3]
	ldr	r6, [sp, #40]
	adds	r3, #35
	cmp	r6, r3
	blt.n	.L11
	b.n	.L12
.L11:
	ldr	r1, [sp, #28]
	movs	r0, #0
	mov	sl, r0
	cmp	r1, #0
	bne.n	.L13
	b.n	.L12
.L13:
	ldr	r6, [sp, #48]
	movs	r0, #225
	movs	r2, #84
	movs	r3, #72
	lsls	r0, r0, #7
	add	r2, sp
	add	r3, sp
	adds	r6, r6, r0
	mov	fp, r2
	mov	r9, r3
	mov	r8, r6
.L22:
	ldr	r1, [sp, #40]
	cmp	r1, sl
	ble.n	.L14
	mov	r1, sl
	cmp	r1, #0
	bge.n	.L15
	adds	r1, #7
.L15:
	asrs	r7, r1, #3
	mov	r2, sl
	lsls	r3, r7, #3
	subs	r7, r2, r3
	lsrs	r3, r2, #31
	add	r3, sl
	asrs	r3, r3, #1
	mov	r6, r8
	movs	r2, #48
	subs	r2, r2, r3
	ldr	r3, [r6, #4]
	lsls	r2, r2, #16
	cmp	r3, r2
	bge.n	.L16
	ldr	r0, [pc, #408]
	cmp	r3, r0
	ble.n	.L16
	ldr	r0, [r6, #0]
	bl	Func_08002322
	ldr	r3, [r6, #8]
	muls	r3, r0
	mov	r1, fp
	str	r3, [r1, #0]
	ldr	r3, [r6, #4]
	str	r3, [r1, #4]
	ldr	r0, [r6, #0]
	bl	Func_0800231c
	ldr	r3, [r6, #8]
	muls	r3, r0
	mov	r2, fp
	str	r3, [r2, #8]
	mov	r1, r9
	mov	r0, fp
	bl	Func_080e3944
	mov	r3, r9
	ldr	r2, [r3, #0]
	asrs	r2, r2, #17
	adds	r2, #64
	str	r2, [r3, #0]
	movs	r0, #6
	ldrsh	r3, [r3, r0]
	mov	r1, r9
	adds	r3, #60
	ldr	r0, [pc, #348]
	str	r3, [r1, #4]
	lsls	r1, r7, #1
	ldrh	r1, [r0, r1]
	ldr	r0, [sp, #48]
	adds	r1, r0, r1
	ldr	r0, [pc, #340]
	ldrb	r5, [r0, r7]
	lsrs	r0, r5, #1
	subs	r2, r2, r0
	ldr	r0, [pc, #336]
	ldrb	r4, [r0, r7]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	str	r5, [sp, #0]
	ldr	r0, [sp, #32]
	str	r4, [sp, #4]
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #44]
	bl	Func_080072f4
.L16:
	ldr	r1, [sp, #16]
	ldr	r3, [r1, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r2, [pc, #288]
	adds	r3, #2
	ldrb	r3, [r2, r3]
	ldr	r0, [sp, #40]
	cmp	r0, r3
	bge.n	.L17
	mov	r3, sl
	adds	r3, #16
	cmp	r0, r3
	ble.n	.L14
	ldr	r3, [r6, #8]
	cmp	r3, #4
	ble.n	.L18
	subs	r3, #2
	str	r3, [r6, #8]
.L18:
	ldr	r3, [r6, #4]
	ldr	r1, [pc, #276]
	cmp	r3, r1
	bgt.n	.L19
	movs	r2, #160
	lsls	r2, r2, #11
	adds	r3, r3, r2
	str	r3, [r6, #4]
.L19:
	ldr	r3, [r6, #0]
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r3, r3, r0
	str	r3, [r6, #0]
	b.n	.L14
.L17:
	ldr	r3, [r6, #8]
	adds	r3, #8
	str	r3, [r6, #8]
	movs	r1, #5
	mov	r0, sl
	bl	Func_080022fc
	ldr	r3, [r6, #4]
	adds	r0, #2
	lsls	r0, r0, #16
	subs	r3, r3, r0
	str	r3, [r6, #4]
	ldr	r1, [sp, #24]
	cmp	r1, r3
	ble.n	.L20
	str	r3, [sp, #24]
.L20:
	ldr	r2, [sp, #20]
	cmp	r2, r3
	bge.n	.L14
	str	r3, [sp, #20]
.L14:
	movs	r6, #1
	ldr	r0, [sp, #28]
	movs	r3, #28
	add	sl, r6
	add	r8, r3
	cmp	sl, r0
	beq.n	.L21
	b.n	.L22
.L21:
	ldr	r1, [pc, #172]
	mov	r9, r1
.L12:
	ldr	r2, [sp, #24]
	ldr	r6, [sp, #20]
	movs	r3, #128
	lsls	r3, r3, #15
	adds	r2, r2, r3
	adds	r6, r6, r3
	str	r6, [sp, #20]
	str	r2, [sp, #24]
	ldr	r0, [sp, #16]
	ldr	r3, [r0, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r2, r3, r2
	adds	r3, r2, #2
	mov	r1, r9
	ldrb	r3, [r1, r3]
	ldr	r6, [sp, #40]
	cmp	r6, r3
	bge.n	.L23
	adds	r3, r2, #1
	ldrb	r3, [r1, r3]
	movs	r0, #0
	mov	sl, r0
	cmp	r3, #0
	beq.n	.L23
	ldr	r5, [pc, #144]
	ldr	r7, [pc, #144]
	mov	r8, r5
	movs	r6, #0
.L26:
	ldr	r0, [sp, #12]
	movs	r1, #3
	bl	Func_080022ec
	cmp	sl, r0
	bge.n	.L24
	movs	r1, #3
	mov	r0, sl
	bl	Func_080022fc
	ldr	r1, [sp, #16]
	ldr	r3, [r1, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r3, #2
	mov	r2, r9
	ldrb	r3, [r2, r3]
	adds	r4, r0, #0
	ldr	r0, [sp, #40]
	subs	r3, #7
	cmp	r0, r3
	blt.n	.L25
	ldr	r0, [pc, #96]
	lsls	r3, r4, #1
	ldrh	r1, [r7, r3]
	ldrb	r4, [r0, r4]
	ldr	r2, [sp, #48]
	mov	r3, r8
	movs	r0, #32
	adds	r1, r2, r1
	ldrb	r2, [r6, r3]
	ldrb	r3, [r5, #1]
	str	r0, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #32]
	subs	r3, r3, r4
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #44]
	bl	Func_080072f4
	ldr	r1, [pc, #28]
	mov	r9, r1
	b.n	.L24
	movs	r0, r0
	.4byte 0x0000ffff
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x00007828
	.4byte 0x04000028
	.4byte 0xffff9000
	.4byte 0x080eea88
	.4byte 0xffd00000
	.4byte 0x080eeaa2
	.4byte 0x080eea91
	.4byte 0x080eea99
	.4byte 0x002fffff
	.4byte 0x080eea62
	.4byte 0x080eeab2
	.4byte 0x080eeab8
.L25:
	ldr	r0, [pc, #728]
	lsls	r3, r4, #1
	ldrh	r1, [r7, r3]
	ldrb	r4, [r0, r4]
	ldrb	r3, [r5, #1]
	ldr	r2, [sp, #48]
	movs	r0, #32
	subs	r3, r3, r4
	adds	r1, r2, r1
	ldrb	r2, [r5, #0]
	str	r0, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #44]
	ldr	r4, [sp, #52]
	bl	Func_080072f4
	ldr	r3, [pc, #700]
	mov	r9, r3
.L24:
	ldr	r1, [sp, #16]
	ldr	r3, [r1, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r3, #1
	mov	r2, r9
	movs	r0, #1
	ldrb	r3, [r2, r3]
	add	sl, r0
	adds	r5, #2
	adds	r6, #2
	cmp	sl, r3
	bne.n	.L26
.L23:
	ldr	r6, [sp, #16]
	ldr	r3, [r6, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r3, #2
	mov	r0, r9
	ldrb	r3, [r0, r3]
	ldr	r1, [sp, #40]
	cmp	r1, r3
	bne.n	.L27
	movs	r2, #0
	ldr	r5, [pc, #648]
	mov	sl, r2
	movs	r6, #15
.L28:
	bl	Func_08004458
	movs	r3, #127
	ands	r3, r0
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	bl	Func_08004458
	ands	r0, r6
	adds	r0, #80
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	bl	Func_08004458
	movs	r3, #63
	ands	r3, r0
	subs	r3, #32
	lsls	r3, r3, #12
	str	r3, [r5, #8]
	bl	Func_08004458
	negs	r0, r0
	ands	r0, r6
	subs	r0, #16
	lsls	r0, r0, #13
	str	r0, [r5, #16]
	bl	Func_08004458
	movs	r3, #1
	ands	r0, r6
	adds	r0, #16
	add	sl, r3
	str	r0, [r5, #24]
	mov	r0, sl
	adds	r5, #28
	cmp	r0, #32
	bne.n	.L28
	ldr	r1, [pc, #564]
	mov	r9, r1
.L27:
	ldr	r2, [sp, #16]
	ldr	r3, [r2, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r3, #2
	mov	r6, r9
	ldrb	r3, [r6, r3]
	ldr	r0, [sp, #40]
	cmp	r0, r3
	blt.n	.L29
	movs	r1, #0
	ldr	r5, [pc, #540]
	mov	sl, r1
.L34:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L30
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L31
	adds	r3, #7
.L31:
	asrs	r4, r3, #3
	lsls	r3, r4, #3
	mov	r2, sl
	subs	r4, r2, r3
	ldr	r2, [pc, #520]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #48]
	adds	r1, r3, r1
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	ldr	r0, [pc, #508]
	ldrb	r0, [r0, r4]
	movs	r6, #2
	ldrsh	r2, [r5, r6]
	str	r0, [sp, #0]
	ldr	r0, [pc, #504]
	ldrb	r0, [r0, r4]
	ldr	r6, [sp, #32]
	str	r0, [sp, #4]
	ldr	r0, [sp, #44]
	ldr	r4, [r6, #4]
	bl	Func_080072f4
	ldr	r3, [r5, #0]
	ldr	r2, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L30:
	ldr	r3, [r5, #4]
	ldr	r0, [sp, #24]
	cmp	r0, r3
	ble.n	.L32
	str	r3, [sp, #24]
.L32:
	ldr	r1, [sp, #20]
	cmp	r1, r3
	bge.n	.L33
	str	r3, [sp, #20]
.L33:
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	adds	r5, #28
	cmp	r3, #24
	bne.n	.L34
	ldr	r6, [pc, #420]
	mov	r9, r6
.L29:
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #20]
	asrs	r0, r0, #16
	asrs	r1, r1, #16
	str	r0, [sp, #24]
	str	r1, [sp, #20]
	cmp	r1, r0
	bgt.n	.L35
	adds	r0, #1
	str	r0, [sp, #20]
.L35:
	ldr	r2, [sp, #16]
	ldr	r3, [r2, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r3, #2
	mov	r6, r9
	ldrb	r3, [r6, r3]
	ldr	r0, [sp, #40]
	cmp	r0, r3
	bne.n	.L36
	ldr	r2, [sp, #48]
	movs	r3, #225
	movs	r1, #0
	lsls	r3, r3, #7
	mov	sl, r1
	adds	r5, r2, r3
.L39:
	bl	Func_08004458
	movs	r3, #127
	ands	r3, r0
	lsls	r3, r3, #16
	str	r3, [r5, #12]
	ldr	r6, [sp, #20]
	ldr	r0, [sp, #24]
	cmp	r6, r0
	bne.n	.L37
	lsls	r3, r0, #16
	str	r3, [r5, #16]
	b.n	.L38
.L37:
	bl	Func_08004458
	ldr	r2, [sp, #20]
	ldr	r3, [sp, #24]
	subs	r1, r2, r3
	bl	Func_08002304
	ldr	r6, [sp, #24]
	adds	r0, r0, r6
	lsls	r0, r0, #16
	str	r0, [r5, #16]
.L38:
	bl	Func_08004458
	movs	r3, #15
	ands	r3, r0
	movs	r0, #1
	add	sl, r0
	adds	r3, #20
	mov	r1, sl
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r1, #32
	bne.n	.L39
	ldr	r2, [pc, #288]
	mov	r9, r2
.L36:
	ldr	r6, [sp, #16]
	ldr	r3, [r6, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r3, #2
	mov	r0, r9
	ldrb	r3, [r0, r3]
	ldr	r1, [sp, #40]
	cmp	r1, r3
	blt.n	.L40
	subs	r3, r1, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	mov	r8, r3
	movs	r6, #225
	ldr	r3, [sp, #48]
	movs	r2, #0
	lsls	r6, r6, #7
	ldr	r7, [pc, #244]
	mov	sl, r2
	adds	r5, r3, r6
.L44:
	ldr	r3, [r5, #24]
	cmp	r3, #17
	bhi.n	.L41
	movs	r0, #17
	subs	r0, r0, r3
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	ldr	r2, [pc, #244]
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #36]
	adds	r1, r2, r1
	movs	r3, #14
	ldrsh	r2, [r5, r3]
	ldr	r3, [pc, #232]
	ldrb	r4, [r3, r0]
	movs	r6, #18
	ldrsh	r3, [r5, r6]
	lsrs	r0, r4, #1
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	ldr	r6, [sp, #32]
	subs	r2, r2, r0
	subs	r3, r3, r0
	mov	r0, r8
	subs	r3, r3, r0
	ldr	r4, [r6, #4]
	ldr	r0, [sp, #44]
	bl	Func_080072f4
	ldr	r3, [r5, #24]
.L41:
	movs	r0, #1
	subs	r3, #1
	negs	r0, r0
	str	r3, [r5, #24]
	cmp	r3, r0
	beq.n	.L42
	cmp	r3, #17
	bne.n	.L43
.L42:
	ldr	r1, [sp, #16]
	ldr	r3, [r1, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r3, #2
	ldrb	r3, [r7, r3]
	ldr	r2, [sp, #40]
	adds	r3, #35
	cmp	r2, r3
	bge.n	.L43
	movs	r3, #17
	str	r3, [r5, #24]
	bl	Func_08004458
	movs	r3, #127
	ands	r3, r0
	lsls	r3, r3, #16
	str	r3, [r5, #12]
	bl	Func_08004458
	ldr	r6, [sp, #24]
	ldr	r3, [sp, #20]
	subs	r1, r3, r6
	bl	Func_08002304
	adds	r0, r0, r6
	lsls	r0, r0, #16
	str	r0, [r5, #16]
.L43:
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	adds	r5, #28
	cmp	r1, #32
	bne.n	.L44
.L40:
	ldr	r3, [sp, #48]
	ldr	r6, [pc, #116]
	adds	r2, r3, r6
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r0, [sp, #40]
	adds	r0, #1
	str	r0, [sp, #40]
	ldr	r1, [sp, #16]
	ldr	r3, [r1, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r0, [pc, #56]
	adds	r3, #2
	ldrb	r3, [r0, r3]
	ldr	r2, [sp, #40]
	adds	r3, #75
	cmp	r2, r3
	beq.n	.L3
	b.n	.L45
.L3:
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	ldr	r0, [pc, #60]
	bl	Func_08004278
	bl	Func_080cdbc0
	add	sp, #96
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x080eeab8
	.4byte 0x080eea88
	.4byte 0x02010000
	.4byte 0x080eeacc
	.4byte 0x080eeabb
	.4byte 0x080eeac3
	.4byte 0x080ede84
	.4byte 0x080ede96
	.4byte 0x00007824
	.4byte 0x080cd261
