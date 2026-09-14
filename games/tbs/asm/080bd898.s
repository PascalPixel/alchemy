@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080bd898
	.thumb_func
Func_080bd898:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #788]
	ldr	r3, [r3, #0]
	sub	sp, #44
	movs	r1, #215
	movs	r2, #128
	str	r3, [sp, #8]
	lsls	r1, r1, #3
	lsls	r2, r2, #4
	adds	r7, r3, r1
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L0
	b.n	.L1
.L0:
	movs	r3, #164
	lsls	r3, r3, #1
	adds	r5, r7, r3
	ldr	r3, [r5, #0]
	cmp	r3, #4
	bne.n	.L2
	b.n	.L1
.L2:
	cmp	r3, #1
	bne.n	.L3
	ldr	r2, [pc, #744]
	movs	r4, #160
	ldr	r1, [sp, #8]
	lsls	r4, r4, #1
	adds	r3, r1, r2
	adds	r6, r7, r4
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	ldr	r3, [r6, #0]
	cmp	r3, r2
	bge.n	.L4
	adds	r4, #4
	movs	r1, #166
	movs	r2, #0
	adds	r3, r7, r4
	lsls	r1, r1, #1
	str	r2, [r3, #0]
	adds	r4, #12
	adds	r3, r7, r1
	str	r2, [r3, #0]
	adds	r3, r7, r4
	str	r2, [r3, #0]
	ldr	r1, [sp, #8]
	ldr	r2, [pc, #704]
	adds	r0, r1, r2
	ldr	r1, [r6, #0]
	bl	Func_080bbb0c
	ldr	r3, [r6, #0]
	adds	r3, #1
	str	r3, [r6, #0]
	movs	r3, #2
	str	r3, [r5, #0]
	b.n	.L0
.L4:
	movs	r3, #4
	str	r3, [r5, #0]
	b.n	.L0
.L3:
	cmp	r3, #2
	beq.n	.L5
	b.n	.L6
.L5:
	movs	r4, #166
	movs	r1, #162
	lsls	r4, r4, #1
	lsls	r1, r1, #1
	adds	r3, r7, r4
	adds	r2, r7, r1
	ldr	r5, [r3, #0]
	ldr	r3, [r2, #0]
	cmp	r5, r3
	blt.n	.L7
	b.n	.L8
.L7:
	adds	r6, r5, #0
.L21:
	movs	r3, #168
	lsls	r3, r3, #1
	adds	r2, r7, r3
	ldr	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L9
	subs	r3, #1
	str	r3, [r2, #0]
	b.n	.L1
.L9:
	ldrb	r3, [r7, r6]
	cmp	r3, #14
	bls.n	.L10
	b.n	.L11
.L10:
	ldr	r2, [pc, #624]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080bd9b6
	.4byte 0x080bd9c4
	.4byte 0x080bd9d2
	.4byte 0x080bd9de
	.4byte 0x080bd9fa
	.4byte 0x080bda16
	.4byte 0x080bd9f0
	.4byte 0x080bda30
	.4byte 0x080bda42
	.4byte 0x080bda82
	.4byte 0x080bdb02
	.4byte 0x080bdb10
	.4byte 0x080bda36
	.4byte 0x080bd9a8
	.4byte 0x080bd99c
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r0, [r7, r3]
	bl	Func_080f9010
	b.n	.L11
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r1, [r7, r3]
	adds	r0, r7, #0
	bl	Func_080bb928
	b.n	.L11
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r0, [r7, r3]
	movs	r1, #1
	bl	Func_08015120
	b.n	.L11
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r0, [r7, r3]
	movs	r1, #5
	bl	Func_08015120
	b.n	.L11
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r0, [r7, r3]
	ldr	r3, [pc, #496]
	movs	r1, #2
	b.n	.L12
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r0, [r7, r3]
	ldr	r3, [pc, #488]
	movs	r1, #4
.L12:
	ands	r0, r3
	bl	Func_08015120
	b.n	.L11
	ldr	r3, [pc, #480]
	ldr	r2, [r3, #0]
	movs	r3, #1
	str	r3, [r2, #8]
	b.n	.L11
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r0, [r7, r3]
	cmp	r0, #0
	blt.n	.L13
	bl	Func_080151d0
.L13:
	movs	r4, #164
	lsls	r4, r4, #1
	adds	r2, r7, r4
	movs	r3, #3
	str	r3, [r2, #0]
	ldr	r2, [pc, #452]
	b.n	.L14
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r0, [r7, r3]
	cmp	r0, #0
	blt.n	.L15
	bl	Func_080151d0
.L15:
	movs	r1, #164
	lsls	r1, r1, #1
	adds	r2, r7, r1
	movs	r3, #13
	str	r3, [r2, #0]
	b.n	.L11
	bl	Func_08015118
	b.n	.L11
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r0, [r7, r3]
	bl	Func_080bb8e8
	b.n	.L11
	movs	r2, #180
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r0, [r3, #0]
	cmp	r0, #0
	ble.n	.L16
	bl	Func_080f9010
.L16:
	movs	r3, #178
	lsls	r3, r3, #1
	adds	r2, r7, r3
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r0, [r7, r3]
	str	r0, [r2, #0]
	bl	Func_080b7dd0
	movs	r1, #5
	ldr	r0, [r0, #0]
	bl	Func_08009080
	movs	r4, #164
	lsls	r4, r4, #1
	movs	r1, #168
	adds	r2, r7, r4
	movs	r3, #10
	lsls	r1, r1, #1
	str	r3, [r2, #0]
	adds	r2, r7, r1
.L14:
	movs	r3, #0
	str	r3, [r2, #0]
	b.n	.L11
	lsls	r3, r6, #2
	adds	r3, #64
	movs	r2, #178
	lsls	r2, r2, #1
	ldr	r0, [r7, r3]
	adds	r5, r7, r2
	movs	r4, #182
	lsls	r4, r4, #1
	str	r0, [r5, #0]
	adds	r3, r7, r4
	ldr	r1, [r3, #0]
	bl	Func_080c24f0
	ldr	r0, [r5, #0]
	bl	Func_080bb588
	ldr	r0, [r5, #0]
	bl	Func_08077008
	movs	r5, #0
	adds	r6, r0, #0
	b.n	.L17
.L20:
	movs	r1, #149
	lsls	r1, r1, #1
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	beq.n	.L18
	movs	r1, #4
	bl	Func_08009020
	b.n	.L19
.L18:
	movs	r1, #5
	bl	Func_08009020
.L19:
	adds	r5, #1
.L17:
	movs	r2, #178
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r0, [r3, #0]
	bl	Func_080b7dd0
	adds	r1, r5, #0
	ldr	r0, [r0, #0]
	bl	Func_080b7f70
	cmp	r0, #0
	bne.n	.L20
	movs	r4, #149
	lsls	r4, r4, #1
	adds	r3, r6, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L11
	movs	r1, #164
	lsls	r1, r1, #1
	adds	r3, r7, r1
	movs	r2, #11
	str	r2, [r3, #0]
	movs	r2, #168
	lsls	r2, r2, #1
	adds	r3, r7, r2
	str	r0, [r3, #0]
	b.n	.L11
	ldr	r3, [pc, #184]
	ldr	r3, [r3, #0]
	adds	r3, #65
	ldrb	r0, [r3, #0]
	bl	Func_08015130
	b.n	.L11
	lsls	r5, r6, #2
	adds	r5, #64
	ldr	r0, [r7, r5]
	bl	Func_080b7dd0
	adds	r1, r0, #0
	ldr	r0, [r7, r5]
	bl	Func_080b78e4
	ldr	r0, [r7, r5]
	bl	Func_080b7dd0
	adds	r6, r0, #0
	ldr	r0, [r7, r5]
	bl	Func_080b6cd0
	adds	r1, r0, #0
	ldr	r0, [r6, #0]
	bl	Func_080ba918
	ldr	r0, [r7, r5]
	bl	Func_080b7aac
.L11:
	movs	r3, #166
	lsls	r3, r3, #1
	adds	r2, r7, r3
	ldr	r3, [r2, #0]
	movs	r4, #162
	adds	r5, r3, #1
	str	r5, [r2, #0]
	lsls	r4, r4, #1
	adds	r3, r7, r4
	ldr	r3, [r3, #0]
	cmp	r5, r3
	bge.n	.L8
	movs	r1, #164
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r3, [r3, #0]
	adds	r6, r5, #0
	cmp	r3, #2
	bne.n	.L8
	b.n	.L21
.L8:
	movs	r3, #164
	lsls	r3, r3, #1
	adds	r2, r7, r3
	ldr	r3, [r2, #0]
	cmp	r3, #2
	beq.n	.L22
	b.n	.L0
.L22:
	movs	r3, #1
	str	r3, [r2, #0]
	b.n	.L0
.L6:
	cmp	r3, #3
	beq.n	.L23
	cmp	r3, #13
	bne.n	.L24
.L23:
	bl	Func_08015048
	cmp	r0, #0
	bne.n	.L25
	b.n	.L1
.L25:
	ldr	r3, [r5, #0]
	cmp	r3, #13
	bne.n	.L26
	movs	r4, #168
	movs	r3, #2
	lsls	r4, r4, #1
	str	r3, [r5, #0]
	adds	r2, r7, r4
	b.n	.L27
.L26:
	movs	r1, #176
	movs	r3, #5
	lsls	r1, r1, #1
	str	r3, [r5, #0]
	adds	r2, r7, r1
	subs	r3, #6
	str	r3, [r2, #0]
	movs	r3, #168
	lsls	r3, r3, #1
	adds	r2, r7, r3
	ldr	r3, [pc, #40]
	ldr	r3, [r3, #0]
	str	r3, [r2, #0]
	b.n	.L0
	movs	r0, r0
	.4byte 0x03001e74
	.4byte 0x00000655
	.4byte 0x00000654
	.4byte 0x080bd960
	.4byte 0x000001ff
	.4byte 0x00003fff
	.4byte 0x03001ee4
	.4byte 0x03001af8
	.4byte 0x03001800
.L24:
	cmp	r3, #5
	beq.n	.L28
	b.n	.L29
.L28:
	ldr	r2, [pc, #164]
	ldr	r3, [r2, #0]
	movs	r2, #7
	lsrs	r3, r3, #2
	ldr	r1, [pc, #160]
	ands	r3, r2
	lsls	r3, r3, #7
	adds	r3, r3, r1
	mov	sl, r3
	ldr	r3, [pc, #152]
	movs	r4, #170
	ldr	r3, [r3, #0]
	lsls	r4, r4, #1
	adds	r4, r4, r7
	mov	r9, r4
	ldr	r4, [r3, #0]
	ldr	r3, [r3, #4]
	movs	r2, #176
	str	r3, [sp, #4]
	lsls	r2, r2, #1
	adds	r6, r7, r2
	mov	fp, r4
	ldr	r3, [r6, #0]
	movs	r4, #1
	movs	r1, #0
	negs	r4, r4
	mov	r8, r1
	cmp	r3, r4
	bne.n	.L30
	ldr	r1, [sp, #8]
	ldr	r3, [r1, #84]
	str	r3, [r6, #0]
.L30:
	ldr	r5, [pc, #112]
	bl	Func_08015118
	adds	r0, r5, #0
	movs	r1, #4
	bl	Func_080039fc
	adds	r0, r5, #0
	movs	r1, #16
	bl	Func_0800393c
	movs	r3, #160
	mov	r2, r9
	lsls	r3, r3, #8
	str	r3, [r2, #4]
	mov	r3, r8
	str	r3, [r2, #8]
	mov	r1, sl
	ldr	r0, [r6, #0]
	bl	Func_080040d0
	ldr	r3, [pc, #48]
	mov	r4, r9
	ldrh	r2, [r4, #8]
	ands	r0, r3
	ldr	r3, [pc, #64]
	ands	r3, r2
	orrs	r3, r0
	mov	r1, r9
	strh	r3, [r1, #8]
	ldr	r4, [sp, #4]
	mov	r3, fp
	ldrh	r2, [r3, #12]
	ldrh	r3, [r4, #4]
	lsls	r2, r2, #3
	lsrs	r3, r3, #8
	adds	r2, r2, r3
	adds	r2, #4
	mov	r8, r2
	ldr	r3, [pc, #16]
	mov	r1, r8
	ands	r1, r3
	mov	r3, r9
	ldrh	r2, [r3, #6]
	ldr	r3, [pc, #32]
	ands	r3, r2
	b.n	.L31
	.4byte 0x000003ff
	.4byte 0x000001ff
	.4byte 0x03001e40
	.4byte 0x080c3734
	.4byte 0x03001ee4
	.4byte 0x0400004a
	.4byte 0xfffffc00
	.4byte 0xfffffe00
.L31:
	orrs	r3, r1
	ldr	r1, [pc, #800]
	ldr	r0, [r1, #0]
	mov	r4, r9
	strh	r3, [r4, #6]
	lsls	r0, r0, #12
	bl	Func_08002322
	cmp	r0, #0
	bge.n	.L32
	ldr	r2, [pc, #784]
	adds	r0, r0, r2
.L32:
	mov	r4, fp
	ldrh	r3, [r4, #14]
	ldr	r1, [sp, #4]
	asrs	r2, r0, #15
	lsls	r3, r3, #3
	adds	r2, r2, r3
	ldrh	r3, [r1, #6]
	lsrs	r3, r3, #8
	adds	r3, r3, r2
	ldr	r0, [pc, #768]
	adds	r3, #6
	mov	r2, r9
	strb	r3, [r2, #4]
	ldr	r3, [r0, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L33
	ldr	r3, [pc, #752]
	ldr	r1, [pc, #756]
	ldr	r3, [r3, #0]
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L33
	movs	r4, #168
	ldr	r3, [pc, #748]
	lsls	r4, r4, #1
	adds	r2, r7, r4
	ldr	r3, [r3, #0]
	ldr	r2, [r2, #0]
	subs	r3, r3, r2
	cmp	r3, #10
	bls.n	.L34
	ldr	r3, [r0, #0]
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L34
.L33:
	movs	r0, #111
	bl	Func_080f9010
	movs	r1, #164
	lsls	r1, r1, #1
	adds	r2, r7, r1
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r3, #168
	lsls	r3, r3, #1
	adds	r2, r7, r3
.L27:
	movs	r3, #0
	str	r3, [r2, #0]
	b.n	.L0
.L34:
	mov	r0, r9
	movs	r1, #240
	bl	Func_08003dec
	b.n	.L1
.L29:
	cmp	r3, #10
	bne.n	.L35
	movs	r4, #168
	lsls	r4, r4, #1
	adds	r3, r7, r4
	ldr	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L36
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L37
	add	r1, sp, #28
	mov	r9, r1
	mov	r2, r9
	movs	r3, #255
	strh	r3, [r2, #0]
	adds	r3, #101
	adds	r5, r7, r3
	ldr	r0, [r5, #0]
	bl	Func_080b7dd0
	adds	r6, r0, #0
	ldr	r0, [r5, #0]
	bl	Func_080b6cd0
	adds	r1, r0, #0
	ldr	r0, [r6, #0]
	bl	Func_080ba918
	b.n	.L38
.L37:
	movs	r1, #178
	lsls	r1, r1, #1
	movs	r4, #28
	adds	r3, r7, r1
	add	r4, sp
	ldr	r0, [r3, #0]
	mov	r9, r4
	mov	r2, r9
	movs	r3, #255
	strh	r0, [r2, #0]
	strh	r3, [r4, #2]
	bl	Func_080b7dd0
	movs	r1, #7
	ldr	r0, [r0, #0]
	bl	Func_080ba918
.L38:
	mov	r0, r9
	bl	Func_080152b8
.L36:
	movs	r2, #168
	lsls	r2, r2, #1
	adds	r1, r7, r2
	ldr	r3, [r1, #0]
	adds	r3, #1
	str	r3, [r1, #0]
	cmp	r3, #8
	bgt.n	.L39
	b.n	.L1
.L39:
	movs	r4, #164
	lsls	r4, r4, #1
	adds	r3, r7, r4
	movs	r2, #2
	str	r2, [r3, #0]
	movs	r3, #0
	str	r3, [r1, #0]
	b.n	.L0
.L35:
	cmp	r3, #11
	beq.n	.L40
	b.n	.L0
.L40:
	movs	r1, #168
	lsls	r1, r1, #1
	adds	r5, r7, r1
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L41
	movs	r2, #128
	lsls	r2, r2, #3
	cmp	r3, r2
	blt.n	.L42
.L41:
	movs	r4, #6
	mov	sl, r4
	cmp	r3, #0
	bne.n	.L43
	movs	r1, #182
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L43
	movs	r2, #178
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r0, [r3, #0]
	bl	Func_08077008
	movs	r3, #148
	lsls	r3, r3, #1
	adds	r0, r0, r3
	ldrb	r0, [r0, #0]
	bl	Func_080c2368
	cmp	r0, #0
	blt.n	.L44
	subs	r0, #1
	cmp	r0, #0
	bge.n	.L45
	movs	r0, #0
.L45:
	adds	r0, #146
	bl	Func_080f9010
.L44:
	movs	r4, #168
	lsls	r4, r4, #1
	movs	r3, #128
	adds	r2, r7, r4
	lsls	r3, r3, #3
	str	r3, [r2, #0]
.L43:
	movs	r1, #168
	lsls	r1, r1, #1
	adds	r2, r7, r1
	ldr	r3, [r2, #0]
	ldr	r4, [pc, #440]
	cmp	r3, r4
	ble.n	.L46
	movs	r3, #0
	str	r3, [r2, #0]
.L46:
	cmp	r3, #0
	bne.n	.L47
	movs	r1, #178
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r0, [r3, #0]
	bl	Func_08077008
	movs	r2, #148
	lsls	r2, r2, #1
	adds	r0, r0, r2
	ldrb	r0, [r0, #0]
	bl	Func_080c2368
	cmp	r0, #0
	blt.n	.L47
	adds	r0, #146
	bl	Func_080f9010
.L47:
	movs	r4, #168
	lsls	r4, r4, #1
	adds	r3, r7, r4
	movs	r1, #128
	ldr	r3, [r3, #0]
	lsls	r1, r1, #3
	cmp	r3, r1
	blt.n	.L48
	ldr	r2, [pc, #380]
	adds	r0, r3, r2
	cmp	r0, #0
	bge.n	.L49
	ldr	r4, [pc, #376]
	adds	r0, r3, r4
.L49:
	asrs	r0, r0, #3
	movs	r1, #5
	bl	Func_080022fc
	adds	r0, #1
	mov	sl, r0
.L48:
	mov	r1, sl
	cmp	r1, #6
	beq.n	.L50
	movs	r2, #168
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r3, [r3, #0]
	movs	r2, #7
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L50
	b.n	.L51
.L50:
	movs	r3, #255
	movs	r6, #0
	add	r5, sp, #12
	mov	r8, r3
	b.n	.L52
.L53:
	ldr	r2, [r0, #40]
	ldrb	r3, [r2, #22]
	mov	r1, r8
	mov	r4, sl
	orrs	r3, r1
	stmia	r5!, {r0}
	strb	r4, [r2, #5]
	strb	r3, [r2, #22]
	adds	r6, #1
.L52:
	movs	r2, #178
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r0, [r3, #0]
	bl	Func_080b7dd0
	adds	r1, r6, #0
	ldr	r0, [r0, #0]
	bl	Func_080b7f70
	cmp	r0, #0
	bne.n	.L53
	b.n	.L51
.L42:
	cmp	r3, #4
	bne.n	.L54
	movs	r4, #178
	lsls	r4, r4, #1
	adds	r3, r7, r4
	ldr	r0, [r3, #0]
	bl	Func_080bac6c
	ldr	r3, [r5, #0]
	b.n	.L55
.L54:
	cmp	r3, #4
	ble.n	.L55
	movs	r1, #178
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r0, [r3, #0]
	bl	Func_080b7dd0
	movs	r3, #1
	adds	r6, r0, #0
	movs	r2, #0
	add	r5, sp, #12
	strh	r3, [r6, #42]
	b.n	.L56
.L57:
	stmia	r5!, {r0}
	adds	r2, #1
.L56:
	adds	r1, r2, #0
	ldr	r0, [r6, #0]
	str	r2, [sp, #0]
	bl	Func_080b7f70
	ldr	r2, [sp, #0]
	cmp	r0, #0
	bne.n	.L57
	movs	r4, #168
	lsls	r4, r4, #1
	adds	r3, r7, r4
	ldr	r3, [r3, #0]
	movs	r1, #20
	lsls	r3, r3, #2
	negs	r1, r1
	adds	r1, r1, r3
	mov	r8, r1
	cmp	r1, #127
	ble.n	.L58
	cmp	r2, #0
	ble.n	.L59
	add	r6, sp, #12
	adds	r5, r2, #0
.L60:
	add	r2, sp, #44
	ldmia	r6!, {r0}
	mov	r9, r2
	movs	r1, #0
	subs	r5, #1
	bl	Func_080bd850
	cmp	r5, #0
	bne.n	.L60
.L59:
	movs	r4, #178
	lsls	r4, r4, #1
	adds	r3, r7, r4
	ldr	r0, [r3, #0]
	bl	Func_080b7e60
	movs	r1, #164
	lsls	r1, r1, #1
	adds	r2, r7, r1
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r3, #168
	lsls	r3, r3, #1
	adds	r2, r7, r3
	movs	r3, #0
	str	r3, [r2, #0]
	b.n	.L1
.L58:
	cmp	r2, #0
	ble.n	.L51
	movs	r4, #19
	movs	r1, #18
	negs	r4, r4
	negs	r1, r1
	adds	r4, r4, r3
	adds	r1, r1, r3
	subs	r3, #17
	mov	fp, r4
	mov	r9, r1
	mov	sl, r3
	adds	r6, r2, #0
	add	r5, sp, #12
.L61:
	ldr	r0, [r5, #0]
	mov	r1, r8
	bl	Func_080090f8
	ldr	r0, [r5, #0]
	mov	r1, fp
	bl	Func_080090f8
	ldr	r0, [r5, #0]
	mov	r1, r9
	bl	Func_080090f8
	subs	r6, #1
	ldmia	r5!, {r0}
	mov	r1, sl
	bl	Func_080090f8
	cmp	r6, #0
	bne.n	.L61
.L51:
	movs	r3, #168
	lsls	r3, r3, #1
	adds	r2, r7, r3
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	b.n	.L1
.L55:
	adds	r3, #1
	str	r3, [r5, #0]
.L1:
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e40
	.4byte 0x00007fff
	.4byte 0x03001ae8
	.4byte 0x03001af8
	.4byte 0x00000303
	.4byte 0x03001800
	.4byte 0x0000041d
	.4byte 0xfffffc00
	.4byte 0xfffffc07
