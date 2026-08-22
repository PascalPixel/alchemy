@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080f6440
	.thumb_func
Func_080f6440:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #156]
	ldr	r7, [r3, #0]
	subs	r3, #24
	ldr	r3, [r3, #0]
	movs	r1, #128
	sub	sp, #40
	lsls	r1, r1, #3
	movs	r0, #0
	str	r3, [sp, #36]
	str	r1, [sp, #28]
	str	r0, [sp, #32]
	bl	Func_08004458
	ldr	r3, [pc, #132]
	ldr	r2, [pc, #136]
	ldrh	r1, [r3, #10]
	ands	r2, r1
	strh	r2, [r3, #10]
	ldr	r2, [pc, #132]
	ldrh	r1, [r3, #10]
	ands	r2, r1
	strh	r2, [r3, #10]
	ldrh	r2, [r3, #10]
	movs	r2, #155
	lsls	r2, r2, #3
	adds	r0, r7, r2
	ldr	r1, [pc, #120]
	ldr	r2, [pc, #120]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #120]
	adds	r5, r7, #0
	ldr	r4, [r3, #0]
	adds	r5, #156
	ldrh	r0, [r5, #0]
	lsls	r4, r4, #16
	ldr	r1, [r3, #0]
	adds	r6, r7, #0
	lsrs	r3, r4, #16
	bics	r3, r0
	adds	r6, #160
	strh	r3, [r6, #0]
	movs	r3, #158
	adds	r3, r3, r7
	movs	r2, #240
	mov	r8, r3
	ands	r1, r2
	mov	r0, r8
	strh	r1, [r0, #0]
	ldrh	r3, [r5, #0]
	ands	r2, r3
	cmp	r2, r1
	bne.n	.L0
	adds	r1, r7, #0
	adds	r1, #162
	ldrh	r2, [r1, #0]
	adds	r3, r2, #0
	cmp	r3, #12
	bls.n	.L1
	movs	r3, #12
	strh	r3, [r1, #0]
	ldr	r2, [pc, #28]
.L1:
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L2
	movs	r3, #4
	strh	r3, [r1, #0]
	b.n	.L3
.L2:
	ldr	r0, [pc, #48]
	adds	r3, r2, r0
	strh	r3, [r1, #0]
	add	r1, sp, #32
	ldrh	r1, [r1, #0]
	mov	r2, r8
	strh	r1, [r2, #0]
	b.n	.L3
	movs	r0, r0
	.4byte 0x0000000c
	.4byte 0x03001f04
	.4byte 0x040000b0
	.4byte 0x0000c5ff
	.4byte 0x00007fff
	.4byte 0x04000054
	.4byte 0xa2600001
	.4byte 0x03001ae8
	.4byte 0x0000ffff
.L0:
	adds	r2, r7, #0
	adds	r2, #162
	movs	r3, #12
	strh	r3, [r2, #0]
.L3:
	lsrs	r3, r4, #16
	strh	r3, [r5, #0]
	ldr	r3, [pc, #204]
	ldrb	r3, [r3, #0]
	mov	sl, r3
	cmp	r3, #0
	beq.n	.L4
	bl	.L5
.L4:
	adds	r2, r7, #0
	adds	r2, #140
	str	r2, [sp, #12]
	str	r2, [sp, #24]
	ldr	r5, [r2, #0]
	cmp	r5, #0
	beq.n	.L6
	b.n	.L7
.L6:
	movs	r0, #228
	bl	Func_080772e0
	movs	r3, #152
	adds	r3, r3, r7
	ldr	r1, [pc, #168]
	mov	r9, r0
	ldr	r0, [r3, #0]
	mov	fp, r3
	mov	r4, r9
	adds	r5, r7, r1
	mov	r3, sl
	subs	r0, r4, r0
	ldr	r2, [r5, #0]
	movs	r1, #2
	str	r3, [sp, #0]
	movs	r3, #64
	bl	Func_080150b0
	mov	r4, fp
	movs	r3, #8
	ldr	r2, [r5, #0]
	ldr	r0, [r4, #0]
	movs	r1, #2
	str	r3, [sp, #0]
	movs	r3, #64
	bl	Func_080150b0
	ldrh	r2, [r6, #0]
	movs	r3, #2
	ands	r3, r2
	mov	r8, fp
	cmp	r3, #0
	beq.n	.L8
	ldr	r0, [sp, #12]
	ldr	r2, [pc, #112]
	movs	r1, #144
	movs	r3, #10
	lsls	r1, r1, #1
	str	r3, [r0, #0]
	adds	r2, r2, r1
	movs	r3, #254
	strb	r3, [r2, #0]
	movs	r2, #153
	lsls	r2, r2, #3
	adds	r3, r7, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl	Func_08015018
	movs	r3, #148
	adds	r3, r3, r7
	mov	r8, r3
	bl	.L9
.L8:
	movs	r3, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L10
	mov	r4, fp
	ldr	r3, [r4, #0]
	cmp	r3, #3
	bgt.n	.L11
	cmp	r9, r3
	ble.n	.L11
	adds	r3, #1
	str	r3, [r4, #0]
	movs	r0, #111
	bl	Func_080f9010
	b.n	.L10
.L11:
	movs	r0, #113
	bl	Func_080f9010
.L10:
	ldrh	r2, [r6, #0]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L12
	mov	r0, r8
	ldr	r3, [r0, #0]
	cmp	r3, #1
	ble.n	.L13
	subs	r3, #1
	str	r3, [r0, #0]
	movs	r0, #111
	bl	Func_080f9010
	b.n	.L12
	.4byte 0x03001d20
	.4byte 0x000004cc
	.4byte 0x0200024c
.L13:
	movs	r0, #113
	bl	Func_080f9010
.L12:
	ldr	r2, [pc, #64]
	ldr	r3, [pc, #52]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	adds	r2, #2
	strh	r3, [r2, #0]
	movs	r1, #1
	ldrh	r2, [r6, #0]
	adds	r3, r1, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L14
	b.n	.L15
.L14:
	ldr	r2, [sp, #24]
	str	r1, [r2, #0]
	ldr	r3, [sp, #36]
	ldr	r4, [pc, #36]
	movs	r0, #153
	adds	r2, r3, r4
	lsls	r0, r0, #3
	movs	r3, #0
	str	r3, [r2, #0]
	adds	r3, r7, r0
	ldr	r0, [r3, #0]
	bl	Func_08015018
	mov	r1, r8
	ldr	r3, [r1, #0]
	b.n	.L16
	.4byte 0x00003fd0
	.4byte 0x00000010
	.4byte 0x04000050
	.4byte 0x0000778c
.L16:
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L17
.L18:
	movs	r0, #228
	bl	Func_08077048
	mov	r2, r8
	ldr	r3, [r2, #0]
	adds	r5, #1
	cmp	r5, r3
	bne.n	.L18
.L17:
	ldr	r4, [pc, #788]
	adds	r3, r7, r4
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl	Func_08015018
	movs	r0, #152
	lsls	r0, r0, #1
	bl	Func_080f9010
	b.n	.L15
.L7:
	cmp	r5, #5
	beq.n	.L19
	b.n	.L20
.L19:
	adds	r4, r7, #0
	movs	r1, #0
	adds	r4, #168
	mov	r9, r1
	adds	r1, r4, #0
	ldr	r3, [r1, #0]
	adds	r3, #1
	str	r3, [r1, #0]
	ldrb	r3, [r7, #25]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L21
	adds	r2, r7, #0
	adds	r2, #25
.L23:
	adds	r5, #1
	cmp	r5, #5
	beq.n	.L22
	adds	r2, #28
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L23
.L21:
	cmp	r5, #5
	bne.n	.L24
.L22:
	movs	r2, #1
	mov	r9, r2
.L24:
	ldrh	r2, [r6, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L25
	movs	r2, #0
	str	r2, [r1, #0]
	ldr	r4, [sp, #36]
	ldr	r0, [pc, #696]
	adds	r3, r4, r0
	str	r2, [r3, #0]
	movs	r1, #148
	adds	r1, r1, r7
	ldr	r3, [r1, #0]
	mov	r8, r1
	mov	sl, r8
	cmp	r3, #4
	bne.n	.L26
	adds	r3, r7, #0
	adds	r3, #144
	str	r2, [r1, #0]
	str	r2, [r3, #0]
	ldr	r3, [sp, #24]
	str	r2, [r3, #0]
	adds	r2, r7, #0
	movs	r6, #0
	movs	r0, #0
	movs	r1, #255
	adds	r2, #24
.L27:
	ldrb	r3, [r2, #2]
	adds	r6, #1
	orrs	r3, r1
	strb	r0, [r2, #1]
	strb	r3, [r2, #2]
	adds	r2, #28
	cmp	r6, #5
	bne.n	.L27
	b.n	.L28
.L26:
	adds	r5, r7, #0
	adds	r5, #144
	ldr	r3, [r5, #0]
	cmp	r3, #4
	bgt.n	.L29
	ldr	r0, [pc, #632]
	bl	Func_080f9010
	ldr	r2, [r5, #0]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, #24
	adds	r3, r7, r3
	ldrb	r2, [r3, #1]
	movs	r1, #1
	eors	r2, r1
	strb	r2, [r3, #1]
	b.n	.L28
.L29:
	mov	r4, r9
	cmp	r4, #0
	bne.n	.L30
	movs	r0, #152
	lsls	r0, r0, #1
	bl	Func_080f9010
	ldr	r0, [sp, #24]
	mov	r1, r9
	movs	r3, #1
	adds	r2, r7, #0
	str	r3, [r0, #0]
	movs	r6, #0
	str	r1, [r5, #0]
	adds	r2, #24
	movs	r1, #255
.L31:
	ldrb	r3, [r2, #2]
	adds	r6, #1
	orrs	r3, r1
	strb	r3, [r2, #2]
	adds	r2, #28
	cmp	r6, #5
	bne.n	.L31
	mov	r2, sl
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	b.n	.L28
.L30:
	movs	r0, #113
	bl	Func_080f9010
	b.n	.L28
.L25:
	mov	r3, r8
	ldrh	r2, [r3, #0]
	movs	r3, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L32
	adds	r5, r7, #0
	adds	r5, #144
	ldr	r0, [r5, #0]
	movs	r1, #6
	adds	r0, #1
	bl	Func_080022fc
	str	r0, [r5, #0]
	movs	r0, #111
	bl	Func_080f9010
	mov	r4, r8
	ldrh	r2, [r4, #0]
.L32:
	movs	r3, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L33
	adds	r5, r7, #0
	adds	r5, #144
	ldr	r0, [r5, #0]
	movs	r1, #6
	adds	r0, #5
	bl	Func_080022fc
	str	r0, [r5, #0]
	movs	r0, #111
	bl	Func_080f9010
	movs	r0, #148
	adds	r0, r0, r7
	mov	r8, r0
	b.n	.L28
.L33:
	movs	r1, #148
	adds	r1, r1, r7
	mov	r8, r1
.L28:
	ldr	r2, [sp, #24]
	ldr	r2, [r2, #0]
	mov	sl, r2
	cmp	r2, #5
	beq.n	.L34
	b.n	.L35
.L34:
	adds	r3, r7, #0
	adds	r3, #144
	ldr	r2, [r3, #0]
	cmp	r2, #5
	bne.n	.L36
	mov	r3, r9
	cmp	r3, #0
	beq.n	.L37
	movs	r4, #195
	lsls	r4, r4, #3
	adds	r6, r7, r4
	ldr	r2, [r6, #0]
	subs	r3, r2, #1
	cmp	r3, #1
	bls.n	.L38
	movs	r0, #153
	lsls	r0, r0, #3
	adds	r5, r7, r0
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl	Func_08015018
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #19
	movs	r3, #4
	movs	r0, #11
	bl	Func_08015010
	adds	r1, r0, #0
	str	r1, [r5, #0]
	movs	r3, #0
	ldr	r0, [pc, #384]
	movs	r2, #0
	bl	Func_08015080
	movs	r1, #152
	movs	r3, #1
	b.n	.L39
.L38:
	cmp	r2, #1
	beq.n	.L40
	b.n	.L41
.L40:
	movs	r2, #153
	lsls	r2, r2, #3
	adds	r3, r7, r2
	ldr	r1, [r3, #0]
	ldr	r0, [pc, #360]
	movs	r3, #8
	movs	r2, #0
	bl	Func_08015080
	movs	r3, #2
	str	r3, [r6, #0]
	movs	r3, #152
	adds	r3, r3, r7
	mov	fp, r3
	b.n	.L9
.L37:
	movs	r4, #195
	lsls	r4, r4, #3
	adds	r6, r7, r4
	ldr	r3, [r6, #0]
	cmp	r3, #3
	beq.n	.L42
	movs	r0, #153
	lsls	r0, r0, #3
	adds	r5, r7, r0
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl	Func_08015018
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #14
	movs	r3, #3
	movs	r0, #16
	bl	Func_08015010
	adds	r1, r0, #0
	str	r1, [r5, #0]
	ldr	r0, [pc, #296]
	movs	r2, #0
	movs	r3, #0
	bl	Func_08015080
.L42:
	movs	r1, #152
	movs	r3, #3
.L39:
	adds	r1, r1, r7
	str	r3, [r6, #0]
	mov	fp, r1
	b.n	.L9
.L36:
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r7
	ldrb	r3, [r3, #25]
	cmp	r3, #0
	bne.n	.L43
	movs	r2, #195
	lsls	r2, r2, #3
	adds	r6, r7, r2
	ldr	r3, [r6, #0]
	cmp	r3, #4
	beq.n	.L44
	movs	r3, #153
	lsls	r3, r3, #3
	adds	r5, r7, r3
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl	Func_08015018
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #7
	movs	r3, #3
	movs	r0, #23
	bl	Func_08015010
	adds	r1, r0, #0
	str	r1, [r5, #0]
	ldr	r0, [pc, #216]
	movs	r2, #0
	movs	r3, #0
	bl	Func_08015080
.L44:
	movs	r4, #152
	movs	r3, #4
	adds	r4, r4, r7
	str	r3, [r6, #0]
	mov	fp, r4
	b.n	.L9
.L43:
	movs	r0, #195
	lsls	r0, r0, #3
	adds	r6, r7, r0
	ldr	r3, [r6, #0]
	cmp	r3, #5
	beq.n	.L45
	movs	r1, #153
	lsls	r1, r1, #3
	adds	r5, r7, r1
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl	Func_08015018
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #7
	movs	r3, #3
	movs	r0, #23
	bl	Func_08015010
	adds	r1, r0, #0
	str	r1, [r5, #0]
	ldr	r0, [pc, #152]
	movs	r2, #0
	movs	r3, #0
	bl	Func_08015080
.L45:
	movs	r3, #152
	mov	r2, sl
	adds	r3, r3, r7
	str	r2, [r6, #0]
	mov	fp, r3
	b.n	.L9
.L35:
	movs	r4, #153
	lsls	r4, r4, #3
	adds	r3, r7, r4
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl	Func_08015018
	movs	r0, #152
	adds	r0, r0, r7
	mov	fp, r0
	b.n	.L9
.L20:
	cmp	r5, #2
	bne.n	.L46
	adds	r4, r7, #0
	adds	r4, #168
	ldr	r3, [r4, #0]
	movs	r1, #0
	adds	r3, #1
	str	r3, [r4, #0]
	str	r1, [sp, #28]
	cmp	r3, #60
	beq.n	.L47
	b.n	.L48
.L47:
	ldr	r2, [sp, #12]
	movs	r3, #3
	str	r3, [r2, #0]
	movs	r0, #93
	str	r4, [sp, #8]
	bl	Func_080f9010
	ldr	r3, [sp, #28]
	ldr	r4, [sp, #8]
	ldr	r2, [pc, #72]
	str	r3, [r4, #0]
	ldr	r3, [pc, #28]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #28]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r4, [sp, #36]
	movs	r0, #239
	lsls	r0, r0, #7
	ldr	r1, [pc, #56]
	adds	r3, r4, r0
	str	r5, [r3, #0]
	adds	r2, r4, r1
	movs	r3, #75
	str	r3, [r2, #0]
	b.n	.L48
	.4byte 0x00003f44
	.4byte 0x00001010
	.4byte 0x000004cc
	.4byte 0x0000778c
	.4byte 0x00000131
	.4byte 0x00000912
	.4byte 0x00000913
	.4byte 0x0000090f
	.4byte 0x0000090d
	.4byte 0x0000090e
	.4byte 0x04000050
	.4byte 0x00007784
.L46:
	cmp	r5, #3
	bne.n	.L49
	adds	r4, r7, #0
	adds	r4, #168
	ldr	r3, [r4, #0]
	adds	r3, #1
	str	r3, [r4, #0]
	movs	r4, #0
	str	r4, [sp, #28]
	ldrh	r2, [r6, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L50
	b.n	.L51
.L50:
	ldr	r0, [sp, #12]
	movs	r3, #10
	str	r3, [r0, #0]
	movs	r0, #112
	bl	Func_080f9010
	b.n	.L52
.L49:
	cmp	r5, #11
	bne.n	.L53
	movs	r3, #195
	lsls	r3, r3, #3
	adds	r5, r7, r3
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L54
	movs	r4, #153
	movs	r3, #1
	lsls	r4, r4, #3
	str	r3, [r5, #0]
	adds	r3, r7, r4
	ldr	r1, [r3, #0]
	ldr	r0, [pc, #844]
	movs	r2, #0
	movs	r3, #8
	bl	Func_08015080
.L54:
	ldrh	r2, [r6, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L55
	b.n	.L52
.L55:
	ldr	r0, [sp, #12]
	movs	r3, #5
	mov	r1, sl
	str	r3, [r0, #0]
	str	r1, [r5, #0]
	movs	r0, #112
	bl	Func_080f9010
	movs	r2, #153
	lsls	r2, r2, #3
	adds	r3, r7, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl	Func_08015018
	b.n	.L56
.L53:
	cmp	r5, #20
	bne.n	.L57
	adds	r4, r7, #0
	adds	r4, #168
	ldr	r3, [r4, #0]
	adds	r3, #1
	str	r3, [r4, #0]
	cmp	r3, #45
	beq.n	.L58
	b.n	.L56
.L58:
	ldr	r0, [sp, #12]
	movs	r1, #152
	movs	r2, #148
	movs	r3, #10
	adds	r1, r1, r7
	adds	r2, r2, r7
	str	r3, [r0, #0]
	b.n	.L59
.L67:
	bl	Func_08004458
	movs	r3, #3
	ands	r0, r3
	adds	r0, #4
	strb	r0, [r5, #2]
	ldr	r0, [pc, #748]
	bl	Func_080f9010
	b.n	.L60
.L57:
	cmp	r5, #10
	bne.n	.L61
	b.n	.L62
.L61:
	adds	r4, r7, #0
	adds	r4, #168
	str	r4, [sp, #20]
	ldr	r3, [r4, #0]
	cmp	r3, #4
	bne.n	.L63
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #17
	movs	r2, #12
	movs	r3, #3
	movs	r0, #18
	str	r4, [sp, #8]
	bl	Func_08015010
	adds	r1, r0, #0
	movs	r0, #153
	lsls	r0, r0, #3
	adds	r3, r7, r0
	str	r1, [r3, #0]
	ldr	r0, [pc, #696]
	movs	r3, #0
	movs	r2, #0
	bl	Func_08015080
	ldr	r4, [sp, #8]
	ldr	r3, [r4, #0]
.L63:
	cmp	r3, #16
	bne.n	.L64
	movs	r0, #153
	lsls	r0, r0, #1
	str	r4, [sp, #8]
	bl	Func_080f9010
	ldr	r4, [sp, #8]
	ldr	r3, [r4, #0]
.L64:
	cmp	r3, #56
	ble.n	.L60
	ldr	r3, [pc, #664]
	ldr	r2, [sp, #36]
	adds	r1, r2, r3
	ldr	r3, [r1, #0]
	cmp	r3, #31
	bgt.n	.L65
	ldrh	r2, [r6, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L60
.L65:
	mov	r4, sl
	movs	r2, #1
	adds	r5, r7, #0
	str	r4, [r1, #0]
	movs	r6, #0
	negs	r2, r2
	adds	r5, #24
.L68:
	ldrb	r3, [r5, #1]
	cmp	r3, #0
	bne.n	.L66
	movs	r3, #2
	ldrsb	r3, [r5, r3]
	cmp	r3, r2
	beq.n	.L67
.L66:
	adds	r6, #1
	adds	r5, #28
	cmp	r6, #5
	bne.n	.L68
.L60:
	adds	r2, r7, #0
	movs	r6, #0
	adds	r2, #24
.L70:
	movs	r3, #2
	ldrsb	r3, [r2, r3]
	ldrb	r1, [r2, #2]
	cmp	r3, #0
	ble.n	.L69
	subs	r3, r1, #1
	strb	r3, [r2, #2]
.L69:
	adds	r6, #1
	adds	r2, #28
	cmp	r6, #5
	bne.n	.L70
	adds	r2, r7, #0
	movs	r1, #0
	movs	r6, #0
	movs	r4, #15
	adds	r2, #24
	movs	r0, #0
.L73:
	ldrb	r3, [r2, #1]
	cmp	r3, #1
	beq.n	.L71
	movs	r3, #2
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	bne.n	.L72
	ldr	r3, [r0, r7]
	ands	r3, r4
	cmp	r3, #8
	bne.n	.L72
.L71:
	adds	r1, #1
.L72:
	adds	r6, #1
	adds	r2, #28
	adds	r0, #28
	cmp	r6, #5
	bne.n	.L73
	cmp	r1, #5
	beq.n	.L74
	b.n	.L75
.L74:
	movs	r0, #0
	movs	r1, #152
	movs	r2, #172
	adds	r1, r1, r7
	adds	r2, r2, r7
	str	r0, [sp, #16]
	mov	sl, r0
	mov	fp, r1
	mov	r8, r2
.L86:
	movs	r3, #0
	mov	r0, r8
	str	r3, [r0, #0]
	mov	r1, fp
	ldr	r2, [r1, #0]
	mov	r9, r3
	movs	r3, #3
	movs	r4, #1
	subs	r3, r3, r2
	negs	r4, r4
	cmp	sl, r3
	ble.n	.L76
	adds	r3, r2, #3
	cmp	sl, r3
	bge.n	.L76
	movs	r6, #0
	movs	r5, #0
.L85:
	mov	r2, sl
	cmp	r2, #0
	bne.n	.L77
	ldr	r0, [r5, r7]
	cmp	r0, #0
	bge.n	.L78
	adds	r0, #15
.L78:
	asrs	r0, r0, #4
	subs	r0, r6, r0
	adds	r0, #22
	b.n	.L79
.L77:
	mov	r3, sl
	cmp	r3, #6
	bne.n	.L80
	ldr	r0, [r5, r7]
	negs	r3, r6
	cmp	r0, #0
	bge.n	.L81
	adds	r0, #15
.L81:
	asrs	r0, r0, #4
	subs	r0, r3, r0
	adds	r0, #26
	b.n	.L79
.L80:
	ldr	r0, [r5, r7]
	cmp	r0, #0
	bge.n	.L82
	adds	r0, #15
.L82:
	mov	r1, sl
	asrs	r0, r0, #4
	subs	r0, r1, r0
	adds	r0, #21
.L79:
	movs	r1, #21
	str	r4, [sp, #8]
	bl	Func_080022fc
	adds	r0, r0, r5
	adds	r0, #4
	ldrb	r3, [r7, r0]
	ldr	r4, [sp, #8]
	cmp	r3, #5
	beq.n	.L83
	movs	r2, #1
	negs	r2, r2
	cmp	r4, r2
	bne.n	.L84
	adds	r4, r3, #0
	b.n	.L83
.L84:
	cmp	r4, r3
	beq.n	.L83
	movs	r3, #1
	mov	r9, r3
.L83:
	adds	r6, #1
	adds	r5, #28
	cmp	r6, #5
	bne.n	.L85
	mov	r0, r9
	cmp	r0, #0
	bne.n	.L76
	movs	r3, #1
	mov	r1, r8
	str	r3, [r1, #0]
	ldr	r3, [sp, #16]
	movs	r0, #144
	lsls	r0, r0, #1
	adds	r2, r3, r0
	ldr	r3, [pc, #348]
	ldr	r1, [pc, #348]
	ldrb	r3, [r3, r4]
	strb	r3, [r1, r2]
	ldr	r2, [sp, #16]
	adds	r2, #1
	str	r2, [sp, #16]
.L76:
	movs	r4, #1
	add	sl, r4
	movs	r3, #4
	mov	r0, sl
	add	r8, r3
	cmp	r0, #7
	bne.n	.L86
	ldr	r1, [sp, #20]
	movs	r5, #0
	str	r5, [r1, #0]
	ldr	r2, [sp, #16]
	cmp	r2, #0
	beq.n	.L87
	movs	r3, #144
	subs	r4, #2
	ldr	r0, [pc, #308]
	lsls	r3, r3, #1
	adds	r2, r2, r3
	adds	r3, r4, #0
	strb	r3, [r0, r2]
	ldr	r1, [sp, #24]
	movs	r3, #2
	str	r3, [r1, #0]
	movs	r0, #171
	bl	Func_080f9010
	movs	r4, #239
	ldr	r3, [sp, #36]
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r0, [sp, #36]
	ldr	r1, [pc, #276]
	adds	r3, r0, r1
	str	r5, [r3, #0]
	movs	r2, #153
	ldr	r3, [pc, #272]
	lsls	r2, r2, #3
	strh	r5, [r3, #0]
	adds	r3, r7, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl	Func_08015018
	movs	r3, #148
	adds	r3, r3, r7
	mov	r8, r3
	b.n	.L88
.L87:
	ldr	r4, [sp, #24]
	movs	r3, #11
	str	r3, [r4, #0]
	movs	r0, #195
	movs	r2, #153
	ldr	r1, [sp, #16]
	lsls	r2, r2, #3
	lsls	r0, r0, #3
	adds	r5, r7, r2
	adds	r3, r7, r0
	str	r1, [r3, #0]
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl	Func_08015018
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #16
	movs	r2, #24
	movs	r3, #4
	movs	r0, #3
	bl	Func_08015010
	adds	r1, r0, #0
	str	r1, [r5, #0]
	movs	r3, #0
	ldr	r0, [pc, #200]
	movs	r2, #0
	bl	Func_08015080
	movs	r3, #148
	adds	r3, r3, r7
	mov	r8, r3
	mov	r5, r8
	ldr	r3, [r5, #0]
	cmp	r3, #4
	bne.n	.L88
	movs	r0, #228
	bl	Func_080772e0
	cmp	r0, #0
	ble.n	.L89
	ldr	r4, [sp, #24]
	movs	r3, #20
	str	r3, [r4, #0]
	b.n	.L90
.L89:
	ldr	r1, [sp, #24]
	movs	r3, #20
	str	r3, [r1, #0]
.L90:
	mov	r2, fp
	ldr	r3, [r2, #0]
	cmp	r3, r0
	ble.n	.L91
	str	r0, [r2, #0]
.L91:
	adds	r3, r7, #0
	movs	r2, #0
	adds	r3, #144
	str	r2, [r5, #0]
	str	r2, [r3, #0]
	adds	r2, r7, #0
	movs	r6, #0
	movs	r0, #0
	movs	r1, #255
	adds	r2, #24
.L92:
	ldrb	r3, [r2, #2]
	adds	r6, #1
	orrs	r3, r1
	strb	r0, [r2, #1]
	strb	r3, [r2, #2]
	adds	r2, #28
	cmp	r6, #5
	bne.n	.L92
	ldr	r3, [sp, #36]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r0, [sp, #36]
	ldr	r1, [pc, #84]
	movs	r2, #0
	adds	r3, r0, r1
	str	r2, [r3, #0]
	ldr	r3, [pc, #80]
	strh	r2, [r3, #0]
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #12
	movs	r3, #4
	movs	r0, #18
	bl	Func_08015010
	ldr	r2, [pc, #68]
	ldr	r5, [pc, #72]
	adds	r1, r0, #0
	adds	r6, r7, r2
	adds	r0, r5, #0
	str	r1, [r6, #0]
	movs	r2, #0
	movs	r3, #8
	subs	r5, #1
	bl	Func_08015080
	ldr	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	Func_08015080
	b.n	.L88
	.4byte 0x0000090c
	.4byte 0x00000133
	.4byte 0x0000090a
	.4byte 0x0000778c
	.4byte 0x080f870c
	.4byte 0x0200024c
	.4byte 0x00007784
	.4byte 0x04000050
	.4byte 0x0000090b
	.4byte 0x000004cc
	.4byte 0x00000905
.L75:
	movs	r3, #152
	movs	r4, #148
	adds	r3, r3, r7
	adds	r4, r4, r7
	mov	fp, r3
	mov	r8, r4
.L88:
	ldr	r0, [sp, #24]
	ldr	r3, [r0, #0]
	cmp	r3, #1
	bne.n	.L93
	movs	r6, #0
	adds	r1, r7, #0
.L98:
	ldrb	r3, [r1, #25]
	cmp	r3, #0
	bne.n	.L94
	movs	r3, #26
	ldrsb	r3, [r1, r3]
	cmp	r3, #0
	beq.n	.L95
	ldr	r3, [r1, #0]
	b.n	.L96
.L95:
	ldr	r2, [r1, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #8
	beq.n	.L97
	adds	r3, r2, #0
.L96:
	adds	r3, #8
	str	r3, [r1, #0]
	adds	r2, r3, #0
.L97:
	movs	r3, #168
	lsls	r3, r3, #1
	cmp	r2, r3
	bne.n	.L94
	movs	r3, #0
	str	r3, [r1, #0]
.L94:
	adds	r6, #1
	adds	r1, #28
	cmp	r6, #5
	bne.n	.L98
.L93:
	ldr	r4, [sp, #36]
	ldr	r0, [pc, #856]
	adds	r3, r4, r0
	ldr	r2, [r3, #0]
	adds	r2, #1
	str	r2, [r3, #0]
	ldr	r1, [sp, #20]
	ldr	r3, [r1, #0]
	adds	r3, #1
	str	r3, [r1, #0]
	b.n	.L9
.L5:
	adds	r2, r7, #0
	movs	r3, #152
	movs	r4, #148
	adds	r2, #140
	adds	r3, r3, r7
	adds	r4, r4, r7
	str	r2, [sp, #12]
	b.n	.L99
.L15:
	movs	r0, #148
	adds	r0, r0, r7
	mov	r8, r0
	b.n	.L9
.L41:
	movs	r1, #152
	adds	r1, r1, r7
	mov	fp, r1
	b.n	.L9
.L48:
	movs	r2, #152
	movs	r3, #148
	adds	r2, r2, r7
	adds	r3, r3, r7
	mov	fp, r2
	mov	r8, r3
	b.n	.L9
.L51:
	movs	r4, #152
	movs	r0, #148
	adds	r4, r4, r7
	adds	r0, r0, r7
	mov	fp, r4
	mov	r8, r0
	b.n	.L9
.L52:
	movs	r1, #152
	movs	r2, #148
	adds	r1, r1, r7
	adds	r2, r2, r7
.L59:
	mov	fp, r1
	mov	r8, r2
	b.n	.L9
.L56:
	movs	r3, #152
	movs	r4, #148
	adds	r3, r3, r7
	adds	r4, r4, r7
.L99:
	mov	fp, r3
	mov	r8, r4
	b.n	.L9
.L62:
	movs	r0, #152
	movs	r1, #148
	adds	r0, r0, r7
	adds	r1, r1, r7
	mov	fp, r0
	mov	r8, r1
.L9:
	ldr	r2, [sp, #12]
	ldr	r3, [r2, #0]
	cmp	r3, #5
	bne.n	.L100
	adds	r3, r7, #0
	adds	r3, #144
	ldr	r1, [r3, #0]
	lsls	r3, r1, #3
	adds	r3, r3, r1
	adds	r4, r7, #0
	lsls	r3, r3, #2
	adds	r4, #168
	adds	r5, r3, #0
	ldr	r3, [r4, #0]
	movs	r2, #15
	ands	r3, r2
	adds	r5, #36
	movs	r6, #128
	movs	r0, #0
	cmp	r3, #7
	bgt.n	.L101
	movs	r0, #1
.L101:
	cmp	r1, #5
	bne.n	.L102
	movs	r5, #208
	movs	r6, #32
.L102:
	adds	r3, r5, #0
	ldr	r4, [sp, #28]
	subs	r3, #12
	lsls	r3, r3, #16
	orrs	r3, r4
	adds	r4, r6, #0
	adds	r4, #8
	ldr	r2, [pc, #672]
	orrs	r3, r4
	orrs	r3, r2
	movs	r1, #200
	str	r3, [r7, r1]
	lsls	r1, r0, #4
	movs	r0, #172
	lsls	r0, r0, #2
	adds	r1, r1, r0
	movs	r3, #204
	str	r1, [r7, r3]
	adds	r3, r5, #0
	ldr	r2, [sp, #28]
	adds	r3, #12
	lsls	r3, r3, #16
	orrs	r3, r2
	ldr	r2, [pc, #644]
	orrs	r3, r4
	orrs	r3, r2
	movs	r0, #208
	str	r3, [r7, r0]
	movs	r3, #212
	str	r1, [r7, r3]
	ldr	r4, [sp, #28]
	lsls	r3, r5, #16
	ldr	r2, [pc, #632]
	orrs	r3, r4
	orrs	r3, r6
	orrs	r3, r2
	movs	r1, #216
	str	r3, [r7, r1]
	movs	r3, #248
	movs	r2, #220
	lsls	r3, r3, #1
	movs	r0, #3
	str	r3, [r7, r2]
	str	r0, [sp, #32]
.L100:
	ldr	r1, [sp, #12]
	ldr	r3, [r1, #0]
	cmp	r3, #3
	bne.n	.L103
	ldr	r3, [sp, #36]
	ldr	r0, [sp, #32]
	ldr	r2, [pc, #600]
	mov	lr, r3
	movs	r1, #204
	lsls	r3, r0, #3
	adds	r4, r7, #0
	adds	r1, r1, r3
	adds	r5, r3, #0
	movs	r6, #0
	adds	r4, #168
	mov	sl, r2
	mov	ip, r1
	adds	r5, #200
.L108:
	movs	r0, #225
	lsls	r0, r0, #7
	add	r0, lr
	movs	r3, #2
	ldrsh	r2, [r0, r3]
	ldr	r1, [sp, #28]
	lsls	r2, r2, #16
	orrs	r2, r1
	movs	r1, #6
	ldrsh	r3, [r0, r1]
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #255
	ands	r3, r1
	orrs	r2, r3
	movs	r3, #128
	lsls	r3, r3, #24
	orrs	r2, r3
	str	r2, [r7, r5]
	mov	r2, sl
	ldrb	r3, [r2, r6]
	movs	r1, #220
	lsls	r3, r3, #4
	lsls	r1, r1, #2
	movs	r2, #240
	lsls	r2, r2, #8
	adds	r3, r3, r1
	orrs	r3, r2
	mov	r2, ip
	str	r3, [r7, r2]
	ldr	r2, [r0, #16]
	ldr	r3, [r0, #4]
	adds	r3, r3, r2
	str	r3, [r0, #4]
	movs	r3, #128
	lsls	r3, r3, #7
	adds	r2, r2, r3
	str	r2, [r0, #16]
	ldr	r3, [r4, #0]
	adds	r2, r3, #0
	cmp	r3, #0
	bge.n	.L104
	adds	r2, #255
.L104:
	asrs	r2, r2, #8
	lsls	r2, r2, #8
	subs	r2, r3, r2
	lsls	r3, r6, #2
	adds	r3, #200
	cmp	r2, r3
	bne.n	.L105
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r0, #16]
	movs	r3, #0
	str	r3, [r0, #24]
.L105:
	movs	r2, #128
	ldr	r3, [r0, #4]
	lsls	r2, r2, #15
	cmp	r3, r2
	ble.n	.L106
	ldr	r1, [r0, #24]
	str	r2, [r0, #4]
	cmp	r1, #1
	bgt.n	.L107
	ldr	r3, [r0, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r0, #16]
.L107:
	adds	r3, r1, #1
	str	r3, [r0, #24]
.L106:
	ldr	r1, [sp, #32]
	movs	r0, #8
	adds	r1, #1
	movs	r2, #28
	adds	r6, #1
	add	ip, r0
	adds	r5, #8
	str	r1, [sp, #32]
	add	lr, r2
	cmp	r6, #8
	bne.n	.L108
.L103:
	ldr	r3, [pc, #396]
	ldr	r0, [sp, #32]
	mov	r9, r3
	lsls	r3, r0, #3
	ldr	r4, [pc, #404]
	adds	r1, r3, #0
	adds	r0, r3, #0
	movs	r2, #156
	movs	r3, #157
	adds	r1, #204
	lsls	r2, r2, #3
	lsls	r3, r3, #3
	ldr	r6, [pc, #392]
	mov	ip, r4
	movs	r5, #0
	adds	r4, r1, #0
	adds	r0, #200
	mov	sl, r2
	mov	lr, r3
.L111:
	ldrb	r3, [r6, #0]
	ldr	r2, [sp, #28]
	lsls	r3, r3, #16
	orrs	r3, r2
	mov	r2, ip
	ldrb	r2, [r2, #0]
	str	r2, [sp, #4]
	movs	r2, #1
	add	ip, r2
	ldr	r2, [sp, #4]
	orrs	r3, r2
	mov	r2, r9
	orrs	r3, r2
	adds	r6, #1
	str	r3, [r7, r0]
	cmp	r5, #3
	bgt.n	.L109
	mov	r3, sl
	str	r3, [r7, r4]
	b.n	.L110
.L109:
	mov	r2, lr
	str	r2, [r7, r1]
.L110:
	ldr	r3, [sp, #32]
	adds	r5, #1
	adds	r3, #1
	adds	r1, #8
	adds	r4, #8
	adds	r0, #8
	str	r3, [sp, #32]
	cmp	r5, #14
	bne.n	.L111
	lsls	r3, r3, #3
	ldr	r4, [pc, #316]
	movs	r0, #25
	adds	r1, r3, #0
	adds	r2, r3, #0
	movs	r5, #128
	adds	r0, r0, r7
	adds	r1, #204
	adds	r2, #200
	mov	lr, r4
	movs	r6, #0
	lsls	r5, r5, #14
	mov	ip, r0
	adds	r4, r1, #0
	mov	r9, r2
.L114:
	mov	r3, ip
	ldrb	r3, [r3, #0]
	mov	sl, r3
	movs	r0, #28
	mov	r3, sl
	add	ip, r0
	cmp	r3, #0
	bne.n	.L112
	ldr	r3, [sp, #28]
	mov	r0, lr
	orrs	r3, r5
	orrs	r3, r0
	mov	r0, r9
	str	r3, [r7, r0]
	movs	r3, #140
	lsls	r3, r3, #3
	str	r3, [r7, r4]
	b.n	.L113
.L112:
	ldr	r3, [sp, #28]
	mov	r0, lr
	orrs	r3, r5
	orrs	r3, r0
	str	r3, [r7, r2]
	movs	r3, #144
	lsls	r3, r3, #3
	str	r3, [r7, r1]
.L113:
	movs	r3, #8
	ldr	r0, [sp, #32]
	add	r9, r3
	movs	r3, #144
	adds	r0, #1
	lsls	r3, r3, #14
	adds	r6, #1
	adds	r1, #8
	adds	r2, #8
	adds	r4, #8
	str	r0, [sp, #32]
	adds	r5, r5, r3
	cmp	r6, #5
	bne.n	.L114
	ldr	r1, [sp, #32]
	ldr	r4, [pc, #212]
	lsls	r3, r1, #3
	adds	r2, r3, #0
	movs	r0, #128
	lsls	r0, r0, #3
	mov	sl, r4
	movs	r5, #128
	adds	r2, #204
	adds	r3, #200
	movs	r4, #132
	mov	ip, r0
	movs	r6, #0
	lsls	r5, r5, #14
	adds	r0, r2, #0
	mov	r9, r3
	mov	lr, r8
	lsls	r4, r4, #2
.L117:
	ldr	r3, [sp, #28]
	mov	r1, sl
	orrs	r3, r5
	orrs	r3, r1
	mov	r1, r9
	str	r3, [r7, r1]
	mov	r1, lr
	ldr	r3, [r1, #0]
	cmp	r6, r3
	bne.n	.L115
	adds	r3, r4, #0
	mov	r1, ip
	orrs	r3, r1
	str	r3, [r7, r0]
	b.n	.L116
.L115:
	movs	r1, #136
	lsls	r1, r1, #2
	lsls	r3, r6, #5
	adds	r3, r3, r1
	mov	r1, ip
	orrs	r3, r1
	str	r3, [r7, r2]
.L116:
	movs	r3, #8
	ldr	r1, [sp, #32]
	add	r9, r3
	movs	r3, #128
	adds	r1, #1
	lsls	r3, r3, #13
	adds	r6, #1
	adds	r2, #8
	adds	r0, #8
	str	r1, [sp, #32]
	adds	r5, r5, r3
	adds	r4, #32
	cmp	r6, #5
	bne.n	.L117
	lsls	r3, r1, #3
	movs	r4, #0
	mov	sl, r4
	adds	r0, r3, #0
	adds	r4, r3, #0
	movs	r5, #5
	adds	r4, #204
	adds	r0, #200
	adds	r1, r3, #0
.L120:
	mov	r3, sl
	movs	r2, #1
	ands	r2, r3
	movs	r3, #129
	lsls	r2, r2, #3
	lsls	r3, r3, #2
	subs	r3, r3, r2
	ldr	r2, [pc, #76]
	ands	r3, r2
	ldr	r2, [sp, #28]
	lsls	r3, r3, #16
	orrs	r3, r2
	ldr	r2, [pc, #44]
	orrs	r3, r5
	orrs	r3, r2
	str	r3, [r7, r0]
	mov	r3, fp
	ldr	r2, [r3, #0]
	movs	r3, #3
	subs	r3, r3, r2
	cmp	sl, r3
	ble.n	.L118
	adds	r3, r2, #3
	cmp	sl, r3
	bge.n	.L118
	movs	r3, #186
	lsls	r3, r3, #3
	str	r3, [r7, r4]
	b.n	.L119
	.4byte 0x0000778c
	.4byte 0x80006000
	.4byte 0x90006000
	.4byte 0x80002000
	.4byte 0x080f8712
	.4byte 0x080f8728
	.4byte 0x080f871a
	.4byte 0x8000207c
	.4byte 0x80006003
	.4byte 0x000001ff
.L118:
	adds	r2, r1, #0
	movs	r3, #162
	adds	r2, #204
	lsls	r3, r3, #3
	str	r3, [r7, r2]
.L119:
	ldr	r2, [sp, #32]
	movs	r3, #1
	adds	r2, #1
	add	sl, r3
	str	r2, [sp, #32]
	mov	r2, sl
	adds	r4, #8
	adds	r0, #8
	adds	r1, #8
	adds	r5, #16
	cmp	r2, #7
	bne.n	.L120
	movs	r3, #160
	lsls	r3, r3, #14
	movs	r6, #0
	mov	fp, r3
	adds	r5, r7, #0
.L124:
	ldr	r0, [sp, #32]
	movs	r4, #0
	lsls	r3, r0, #3
	movs	r1, #204
	mov	sl, r4
	adds	r1, r1, r3
	adds	r4, r3, #0
	mov	r9, fp
	mov	r8, r1
	adds	r4, #200
.L123:
	mov	r2, sl
	lsls	r1, r2, #4
	ldr	r2, [r5, #0]
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L121
	adds	r3, #15
.L121:
	asrs	r3, r3, #4
	lsls	r3, r3, #4
	subs	r3, r2, r3
	adds	r3, r1, r3
	mov	r0, r9
	adds	r3, #4
	ldr	r2, [pc, #372]
	orrs	r3, r0
	orrs	r3, r2
	str	r3, [r7, r4]
	ldr	r0, [r5, #0]
	cmp	r0, #0
	bge.n	.L122
	adds	r0, #15
.L122:
	mov	r1, sl
	asrs	r0, r0, #4
	subs	r0, r1, r0
	adds	r0, #21
	movs	r1, #21
	str	r4, [sp, #8]
	bl	Func_080022fc
	adds	r0, #4
	ldrb	r3, [r5, r0]
	movs	r2, #128
	lsls	r2, r2, #4
	lsls	r3, r3, #4
	orrs	r3, r2
	mov	r2, r8
	str	r3, [r7, r2]
	ldr	r0, [sp, #32]
	movs	r1, #1
	ldr	r4, [sp, #8]
	add	sl, r1
	movs	r3, #8
	adds	r0, #1
	mov	r2, sl
	add	r8, r3
	adds	r4, #8
	str	r0, [sp, #32]
	cmp	r2, #7
	bne.n	.L123
	movs	r3, #128
	lsls	r3, r3, #14
	adds	r6, #1
	add	fp, r3
	adds	r5, #28
	cmp	r6, #5
	bne.n	.L124
	ldr	r0, [sp, #12]
	ldr	r3, [r0, #0]
	movs	r4, #40
	cmp	r3, #1
	bne.n	.L125
	adds	r3, r7, #0
	adds	r3, #168
	ldr	r0, [r3, #0]
	cmp	r0, #47
	bgt.n	.L126
	ldr	r3, [pc, #268]
	muls	r0, r3
	bl	Func_08002322
	lsls	r0, r0, #6
	b.n	.L127
.L126:
	cmp	r0, #55
	bgt.n	.L125
	ldr	r1, [pc, #256]
	lsls	r0, r0, #12
	adds	r0, r0, r1
	bl	Func_08002322
	lsls	r0, r0, #2
.L127:
	asrs	r0, r0, #16
	adds	r4, r0, #0
	adds	r4, #40
.L125:
	ldr	r3, [sp, #32]
	ldr	r0, [sp, #28]
	lsls	r2, r3, #3
	ldr	r3, [pc, #236]
	orrs	r4, r0
	orrs	r4, r3
	adds	r1, r2, #0
	movs	r3, #160
	adds	r1, #200
	adds	r2, #204
	lsls	r3, r3, #3
	str	r4, [r7, r1]
	str	r3, [r7, r2]
	ldr	r1, [sp, #32]
	adds	r1, #1
	lsls	r3, r1, #3
	adds	r6, r3, #0
	adds	r6, #204
	adds	r4, r6, #0
	adds	r0, r4, #0
	str	r1, [sp, #32]
	movs	r2, #12
	adds	r1, r0, #0
	adds	r3, #200
	movs	r5, #0
	mov	r8, r2
	mov	lr, r1
	mov	ip, r3
.L133:
	ldr	r3, [sp, #28]
	mov	r2, r8
	orrs	r3, r2
	ldr	r2, [pc, #184]
	orrs	r3, r2
	mov	r2, ip
	str	r3, [r7, r2]
	cmp	r5, #0
	bne.n	.L128
	movs	r3, #168
	lsls	r3, r3, #3
	mov	r2, lr
	str	r3, [r7, r2]
	b.n	.L129
.L128:
	cmp	r5, #1
	bne.n	.L130
	movs	r3, #170
	lsls	r3, r3, #3
	str	r3, [r7, r1]
	b.n	.L129
.L130:
	cmp	r5, #6
	bne.n	.L131
	movs	r3, #174
	lsls	r3, r3, #3
	str	r3, [r7, r0]
	b.n	.L129
.L131:
	cmp	r5, #7
	bne.n	.L132
	movs	r3, #176
	lsls	r3, r3, #3
	str	r3, [r7, r4]
	b.n	.L129
.L132:
	movs	r3, #172
	lsls	r3, r3, #3
	str	r3, [r7, r6]
.L129:
	ldr	r2, [sp, #32]
	movs	r3, #8
	add	lr, r3
	add	ip, r3
	adds	r2, #1
	movs	r3, #16
	adds	r5, #1
	adds	r6, #8
	adds	r4, #8
	adds	r0, #8
	adds	r1, #8
	str	r2, [sp, #32]
	add	r8, r3
	cmp	r5, #8
	bne.n	.L133
	cmp	r2, #128
	beq.n	.L134
	lsls	r3, r2, #3
	adds	r2, r3, #0
	ldr	r0, [pc, #88]
	movs	r1, #0
	adds	r2, #204
	adds	r3, #200
.L135:
	str	r0, [r7, r3]
	str	r1, [r7, r2]
	ldr	r4, [sp, #32]
	adds	r4, #1
	adds	r2, #8
	adds	r3, #8
	str	r4, [sp, #32]
	cmp	r4, #128
	bne.n	.L135
.L134:
	ldr	r0, [sp, #32]
	movs	r4, #132
	lsls	r2, r0, #1
	lsls	r4, r4, #24
	adds	r0, r7, #0
	movs	r1, #224
	ldr	r3, [pc, #52]
	adds	r0, #200
	lsls	r1, r1, #19
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x80006000
	.4byte 0x000002aa
	.4byte 0xfffd0000
	.4byte 0x80d06000
	.4byte 0x80ce6000
	.4byte 0x40f02000
	.4byte 0x040000d4
