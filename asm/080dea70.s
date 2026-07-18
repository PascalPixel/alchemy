@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080dea70
	.thumb_func
Func_080dea70:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #408]
	adds	r2, r3, #0
	adds	r5, r0, #0
	ldmia	r2!, {r0}
	ldr	r2, [r2, #0]
	sub	sp, #156
	str	r2, [sp, #132]
	adds	r2, r3, #0
	subs	r2, #108
	ldr	r2, [r2, #0]
	str	r2, [sp, #108]
	ldr	r3, [r3, #8]
	mov	r8, r1
	movs	r1, #0
	str	r3, [sp, #104]
	str	r1, [sp, #100]
	ldr	r3, [pc, #380]
	mov	fp, r0
	add	r3, fp
	ldr	r2, [r5, #24]
	str	r5, [r3, #0]
	mov	r3, r8
	str	r2, [sp, #92]
	cmp	r3, #10
	bne.n	.L0
	movs	r0, #0
	bl	Func_080cd594
	b.n	.L1
.L0:
	movs	r0, #1
	bl	Func_080cd594
.L1:
	ldr	r6, [pc, #348]
	add	r6, fp
	ldr	r2, [r6, #0]
	ldr	r3, [r2, #28]
	cmp	r3, #1
	bne.n	.L2
	add	r3, sp, #140
	ldr	r2, [r2, #4]
	str	r3, [sp, #0]
	add	r3, sp, #136
	str	r3, [sp, #4]
	adds	r0, r5, #0
	movs	r1, #1
	movs	r3, #2
	bl	Func_080de2f8
	ldr	r2, [r6, #0]
.L2:
	mov	r4, r8
	cmp	r4, #5
	bne.n	.L3
	ldr	r3, [r2, #4]
	cmp	r3, #1
	bne.n	.L4
	movs	r3, #2
	b.n	.L5
.L4:
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r0, #46
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	bl	Func_080ed408
	b.n	.L6
.L3:
	ldr	r3, [r2, #4]
	cmp	r3, #1
	bne.n	.L7
	movs	r3, #3
.L5:
	str	r3, [sp, #0]
	movs	r0, #46
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	bl	Func_080ed408
	b.n	.L6
.L7:
	movs	r3, #3
	movs	r0, #46
	movs	r1, #7
	movs	r2, #7
	str	r3, [sp, #0]
	bl	Func_080ed408
.L6:
	ldr	r3, [pc, #244]
	adds	r3, #184
	ldr	r3, [r3, #0]
	ldr	r0, [pc, #244]
	str	r3, [sp, #112]
	ldr	r1, [sp, #104]
	movs	r2, #0
	movs	r3, #0
	mov	r6, r8
	bl	Func_080e0524
	cmp	r6, #0
	beq.n	.L8
	cmp	r6, #5
	beq.n	.L9
	cmp	r6, #8
	bne.n	.L10
.L8:
	mov	r7, r8
	cmp	r7, #5
	bne.n	.L11
.L9:
	movs	r0, #2
	str	r0, [sp, #92]
.L11:
	mov	r1, r8
	cmp	r1, #8
	bne.n	.L12
	movs	r2, #0
	str	r2, [sp, #92]
.L12:
	ldr	r3, [sp, #92]
	cmp	r3, #0
	bne.n	.L13
	movs	r1, #128
	lsls	r1, r1, #5
	ldr	r0, [pc, #188]
	b.n	.L14
.L13:
	ldr	r4, [sp, #92]
	cmp	r4, #1
	bne.n	.L15
	movs	r1, #128
	lsls	r1, r1, #5
	ldr	r0, [pc, #180]
.L14:
	add	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	b.n	.L16
.L15:
	movs	r1, #128
	lsls	r1, r1, #5
	ldr	r0, [pc, #164]
	add	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
.L16:
	mov	r6, r8
	cmp	r6, #5
	bne.n	.L17
	ldr	r0, [pc, #152]
	bl	Func_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #144]
	lsls	r0, r0, #19
	movs	r2, #128
	bl	Func_080072f0
.L17:
	movs	r1, #128
	lsls	r1, r1, #6
	ldr	r0, [pc, #136]
	add	r1, fp
	movs	r2, #1
	movs	r3, #0
	mov	r7, r8
	bl	Func_080e0524
	cmp	r7, #5
	bne.n	.L18
	ldr	r0, [pc, #108]
	bl	Func_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #104]
	lsls	r0, r0, #19
	movs	r2, #128
	bl	Func_080072f0
.L18:
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #92]
	movs	r3, #75
	b.n	.L19
.L10:
	mov	r0, r8
	cmp	r0, #1
	bne.n	.L20
	mov	r1, fp
	movs	r2, #1
	movs	r3, #1
	ldr	r0, [pc, #76]
	bl	Func_080e0524
	ldr	r2, [pc, #76]
	ldr	r3, [pc, #20]
	strh	r3, [r2, #0]
	movs	r3, #239
	lsls	r3, r3, #7
	ldr	r2, [pc, #56]
	add	r3, fp
	mov	r1, r8
	str	r1, [r3, #0]
	add	r2, fp
	movs	r3, #0
	b.n	.L21
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x03001e50
	.4byte 0x00000073
	.4byte 0x0000007f
	.4byte 0x00000080
	.4byte 0x00000081
	.4byte 0x000000b9
	.4byte 0x03001388
	.4byte 0x000000c7
	.4byte 0x00007784
	.4byte 0x0000005d
	.4byte 0x04000050
.L20:
	mov	r2, r8
	cmp	r2, #2
	bne.n	.L22
	ldr	r0, [pc, #816]
	bl	Func_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #808]
	movs	r2, #128
	lsls	r0, r0, #19
	bl	Func_080072f0
	movs	r2, #0
	movs	r3, #0
	ldr	r0, [pc, #800]
	mov	r1, fp
	bl	Func_080e0524
	movs	r3, #239
	lsls	r3, r3, #7
	ldr	r2, [pc, #792]
	add	r3, fp
	mov	r4, r8
	str	r4, [r3, #0]
	add	r2, fp
	movs	r3, #50
	b.n	.L21
.L22:
	mov	r3, r8
	subs	r3, #3
	cmp	r3, #1
	bls.n	.L23
	mov	r6, r8
	cmp	r6, #11
	bne.n	.L24
.L23:
	movs	r2, #1
	movs	r3, #1
	ldr	r0, [pc, #764]
	mov	r1, fp
	b.n	.L25
.L24:
	mov	r7, r8
	cmp	r7, #6
	bne.n	.L26
	movs	r2, #1
	movs	r3, #1
	ldr	r0, [pc, #752]
	mov	r1, fp
	b.n	.L25
.L26:
	ldr	r0, [pc, #748]
	mov	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	ldr	r1, [pc, #740]
	movs	r2, #1
	movs	r3, #0
	ldr	r0, [pc, #740]
	add	r1, fp
.L25:
	bl	Func_080e0524
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #700]
	movs	r3, #50
.L19:
	add	r2, fp
.L21:
	str	r3, [r2, #0]
	ldr	r5, [pc, #716]
	movs	r1, #144
	lsls	r1, r1, #3
	add	r5, fp
	ldr	r0, [pc, #712]
	bl	Func_080041d8
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #8]
	bl	Func_080b5098
	ldr	r6, [r0, #0]
	mov	r0, r8
	lsls	r0, r0, #2
	str	r0, [sp, #84]
	ldr	r1, [pc, #696]
	adds	r3, r0, #0
	add	r3, r8
	ldrb	r2, [r1, r3]
	str	r2, [sp, #88]
	adds	r2, r3, #1
	ldrb	r2, [r1, r2]
	mov	sl, r2
	adds	r2, r3, #2
	ldrb	r2, [r1, r2]
	str	r2, [sp, #80]
	adds	r3, #3
	ldrb	r1, [r1, r3]
	str	r1, [sp, #76]
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #8]
	bl	Func_080b5070
	str	r0, [sp, #72]
	ldr	r5, [r5, #0]
	ldr	r4, [sp, #88]
	ldr	r3, [r5, #20]
	muls	r3, r4
	cmp	r3, #63
	ble.n	.L27
	movs	r3, #1
	str	r3, [r5, #20]
.L27:
	movs	r7, #0
	str	r7, [sp, #128]
	ldr	r3, [r5, #20]
	cmp	r3, #0
	bne.n	.L28
	b.n	.L29
.L28:
	ldr	r0, [pc, #620]
	movs	r1, #0
	add	r0, fp
	movs	r2, #36
	str	r0, [sp, #68]
	str	r1, [sp, #24]
	str	r2, [sp, #20]
.L53:
	ldr	r4, [sp, #68]
	ldr	r7, [sp, #20]
	ldr	r3, [r4, #0]
	ldrsh	r0, [r3, r7]
	bl	Func_080b5098
	ldr	r2, [sp, #68]
	ldr	r4, [sp, #20]
	ldr	r3, [r2, #0]
	ldr	r7, [r0, #0]
	ldrsh	r0, [r3, r4]
	bl	Func_080b5070
	ldr	r3, [sp, #88]
	movs	r2, #0
	str	r0, [sp, #64]
	str	r2, [sp, #124]
	cmp	r3, #0
	bne.n	.L30
	b.n	.L31
.L30:
	ldr	r4, [sp, #72]
	lsrs	r3, r4, #31
	ldr	r0, [sp, #24]
	adds	r3, r4, r3
	asrs	r3, r3, #1
	mov	r9, r3
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsls	r3, r3, #2
	movs	r1, #225
	add	r3, fp
	lsls	r1, r1, #7
	adds	r5, r3, r1
.L52:
	ldr	r3, [r6, #8]
	mov	r2, r8
	str	r3, [r5, #0]
	cmp	r2, #7
	bne.n	.L32
	bl	Func_08004458
	movs	r2, #15
	ldr	r3, [r6, #12]
	ands	r2, r0
	lsls	r2, r2, #16
	movs	r4, #232
	adds	r3, r3, r2
	lsls	r4, r4, #14
	adds	r3, r3, r4
	b.n	.L33
.L32:
	mov	r0, r8
	cmp	r0, #10
	bne.n	.L34
	ldr	r3, [r6, #12]
	add	r3, r9
	b.n	.L33
.L34:
	mov	r1, r8
	cmp	r1, #6
	bne.n	.L35
	ldr	r3, [r6, #12]
	add	r3, r9
	b.n	.L33
.L35:
	mov	r2, r8
	cmp	r2, #9
	bne.n	.L36
	bl	Func_08004458
	movs	r2, #31
	ldr	r1, [r6, #12]
	ands	r2, r0
	movs	r3, #16
	b.n	.L37
.L36:
	mov	r3, r8
	subs	r3, #3
	cmp	r3, #1
	bhi.n	.L38
	bl	Func_08004458
	movs	r2, #31
	ldr	r1, [r6, #12]
	ands	r2, r0
	movs	r3, #16
	b.n	.L37
.L38:
	mov	r3, r8
	cmp	r3, #11
	bne.n	.L39
	bl	Func_08004458
	movs	r2, #63
	ldr	r1, [r6, #12]
	ands	r2, r0
	movs	r3, #32
.L37:
	subs	r3, r3, r2
	add	r1, r9
	lsls	r3, r3, #16
	adds	r1, r1, r3
	str	r1, [r5, #4]
	b.n	.L40
.L39:
	mov	r4, r8
	cmp	r4, #5
	bne.n	.L41
	ldr	r3, [r6, #12]
	add	r3, r9
	b.n	.L33
.L41:
	ldr	r3, [r6, #12]
	ldr	r0, [sp, #72]
	adds	r3, r3, r0
.L33:
	str	r3, [r5, #4]
.L40:
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	ldr	r0, [r7, #8]
	ldr	r3, [r5, #0]
	mov	r1, sl
	subs	r0, r0, r3
	bl	Func_080022ec
	mov	r1, r8
	str	r0, [r5, #12]
	cmp	r1, #7
	bne.n	.L42
	bl	Func_08004458
	movs	r3, #63
	ands	r3, r0
	ldr	r0, [r7, #12]
	lsls	r3, r3, #16
	adds	r0, r0, r3
	ldr	r3, [r5, #4]
	ldr	r2, [pc, #364]
	subs	r0, r0, r3
	adds	r0, r0, r2
	mov	r1, sl
	b.n	.L43
.L42:
	mov	r3, r8
	cmp	r3, #8
	bne.n	.L44
	bl	Func_08004458
	movs	r3, #7
	ands	r3, r0
	ldr	r0, [r7, #12]
	lsls	r3, r3, #16
	adds	r0, r0, r3
	ldr	r3, [r5, #4]
	movs	r4, #176
	subs	r0, r0, r3
	lsls	r4, r4, #13
	adds	r0, r0, r4
	mov	r1, sl
	b.n	.L43
.L44:
	mov	r0, r8
	cmp	r0, #9
	bne.n	.L45
	bl	Func_08004458
	movs	r2, #63
	ands	r2, r0
	movs	r3, #64
	subs	r3, r3, r2
	ldr	r0, [r7, #12]
	lsls	r3, r3, #16
	adds	r0, r0, r3
	b.n	.L46
.L45:
	mov	r1, r8
	cmp	r1, #10
	bne.n	.L47
	bl	Func_08004458
	movs	r3, #31
	ands	r3, r0
	ldr	r0, [r7, #12]
	lsls	r3, r3, #16
	adds	r0, r0, r3
	ldr	r3, [r5, #4]
	movs	r2, #128
	subs	r0, r0, r3
	lsls	r2, r2, #11
	adds	r0, r0, r2
	mov	r1, sl
	b.n	.L43
.L47:
	mov	r3, r8
	subs	r3, #3
	cmp	r3, #1
	bls.n	.L48
	mov	r3, r8
	cmp	r3, #11
	beq.n	.L48
	cmp	r3, #5
	bne.n	.L49
.L48:
	movs	r3, #0
	str	r3, [r5, #16]
	b.n	.L50
.L49:
	mov	r4, r8
	cmp	r4, #6
	bne.n	.L51
	bl	Func_08004458
	adds	r2, r0, #0
	ldr	r0, [sp, #64]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	ldr	r0, [r7, #12]
	asrs	r3, r3, #1
	adds	r0, r0, r3
	movs	r3, #15
	ands	r3, r2
	lsls	r3, r3, #16
	adds	r0, r0, r3
	b.n	.L46
.L51:
	bl	Func_08004458
	movs	r3, #15
	ldr	r1, [sp, #64]
	adds	r2, r0, #0
	ldr	r0, [r7, #12]
	ands	r3, r2
	adds	r0, r0, r1
	lsls	r3, r3, #16
	subs	r0, r0, r3
.L46:
	ldr	r3, [r5, #4]
	mov	r1, sl
	subs	r0, r0, r3
.L43:
	bl	Func_080022ec
	str	r0, [r5, #16]
.L50:
	ldr	r3, [r5, #8]
	ldr	r0, [r7, #16]
	mov	r1, sl
	subs	r0, r0, r3
	bl	Func_080022ec
	movs	r3, #0
	str	r0, [r5, #20]
	str	r3, [r5, #24]
	ldr	r2, [sp, #124]
	ldr	r3, [sp, #88]
	adds	r2, #1
	adds	r5, #28
	str	r2, [sp, #124]
	cmp	r2, r3
	beq.n	.L31
	b.n	.L52
.L31:
	ldr	r4, [sp, #24]
	ldr	r0, [sp, #20]
	ldr	r1, [sp, #128]
	ldr	r7, [sp, #88]
	adds	r0, #2
	adds	r4, r4, r7
	adds	r1, #1
	str	r4, [sp, #24]
	str	r0, [sp, #20]
	str	r1, [sp, #128]
	ldr	r2, [sp, #68]
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #20]
	cmp	r1, r3
	beq.n	.L29
	b.n	.L53
.L29:
	movs	r3, #0
	str	r3, [sp, #128]
	movs	r2, #128
	ldr	r3, [pc, #104]
	movs	r1, #0
	lsls	r2, r2, #3
.L54:
	str	r1, [r3, #0]
	ldr	r4, [sp, #128]
	adds	r4, #1
	adds	r3, #28
	str	r4, [sp, #128]
	cmp	r4, r2
	bne.n	.L54
	mov	r6, r8
	cmp	r6, #6
	bne.n	.L55
	ldr	r3, [pc, #60]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r7, [sp, #76]
	ldr	r3, [r3, #20]
	ldr	r0, [sp, #80]
	ldr	r1, [sp, #88]
	muls	r3, r7
	adds	r2, r0, #0
	muls	r2, r1
	adds	r3, r3, r2
	adds	r3, #32
	b.n	.L56
	movs	r0, r0
	.4byte 0x0000007f
	.4byte 0x03001388
	.4byte 0x0000005c
	.4byte 0x00007784
	.4byte 0x0000005b
	.4byte 0x00000068
	.4byte 0x000000b8
	.4byte 0x000065c0
	.4byte 0x00000092
	.4byte 0x00007828
	.4byte 0x080cd261
	.4byte 0x080eebec
	.4byte 0xfff40000
	.4byte 0x02010018
.L55:
	ldr	r3, [pc, #824]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r2, [sp, #76]
	ldr	r3, [r3, #20]
	ldr	r4, [sp, #80]
	ldr	r6, [sp, #88]
	muls	r3, r2
	adds	r2, r4, #0
	muls	r2, r6
	adds	r3, r3, r2
	adds	r3, #16
.L56:
	str	r3, [sp, #96]
	ldr	r0, [sp, #96]
	movs	r7, #0
	str	r7, [sp, #120]
	cmp	r0, #0
	bne.n	.L57
	bl	.L58
.L57:
	ldr	r1, [sp, #108]
	adds	r1, #12
	str	r1, [sp, #48]
.L128:
	ldr	r2, [sp, #100]
	cmp	r2, #0
	ble.n	.L59
	subs	r2, #1
	str	r2, [sp, #100]
.L59:
	mov	r3, r8
	cmp	r3, #6
	bne.n	.L60
	ldr	r4, [sp, #120]
	cmp	r4, #4
	bne.n	.L61
	movs	r0, #136
	bl	Func_080f9010
.L61:
	ldr	r6, [sp, #120]
	cmp	r6, #32
	bne.n	.L62
	movs	r0, #134
	bl	Func_080b50e8
	b.n	.L62
.L60:
	mov	r7, r8
	cmp	r7, #7
	bne.n	.L63
	ldr	r0, [sp, #120]
	cmp	r0, #48
	bne.n	.L62
	movs	r0, #133
	bl	Func_080b50e8
	b.n	.L62
.L63:
	mov	r1, r8
	cmp	r1, #5
	beq.n	.L62
	ldr	r2, [sp, #120]
	cmp	r2, #16
	bne.n	.L62
	movs	r0, #133
	bl	Func_080b50e8
.L62:
	ldr	r7, [pc, #700]
	add	r7, fp
	ldr	r3, [r7, #0]
	ldr	r3, [r3, #28]
	cmp	r3, #1
	bne.n	.L64
	ldr	r3, [sp, #120]
	lsls	r5, r3, #11
	adds	r0, r5, #0
	bl	Func_08002322
	ldr	r3, [sp, #140]
	negs	r0, r0
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	lsls	r0, r0, #2
	asrs	r3, r3, #1
	asrs	r0, r0, #16
	adds	r0, r0, r3
	subs	r0, #10
	mov	sl, r0
	adds	r0, r5, #0
	bl	Func_0800231c
	lsls	r0, r0, #1
	ldr	r3, [sp, #136]
	asrs	r0, r0, #16
	adds	r0, r0, r3
	ldr	r4, [sp, #120]
	adds	r6, r0, #0
	subs	r6, #24
	cmp	r4, #69
	ble.n	.L65
	lsls	r3, r4, #1
	subs	r3, r6, r3
	adds	r6, r3, #0
	adds	r6, #138
.L65:
	ldr	r3, [r7, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L66
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	bl	Func_080ed408
	b.n	.L67
.L66:
	movs	r3, #3
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	str	r3, [sp, #0]
	bl	Func_080ed408
.L67:
	ldr	r3, [pc, #588]
	adds	r3, #188
	ldr	r5, [r3, #0]
	ldr	r7, [sp, #120]
	str	r5, [sp, #116]
	cmp	r7, #3
	bgt.n	.L68
	movs	r3, #20
	ldr	r1, [pc, #576]
	str	r3, [sp, #0]
	movs	r3, #40
	str	r3, [sp, #4]
	ldr	r0, [sp, #132]
	add	r1, fp
	mov	r2, sl
	adds	r3, r6, #0
	bl	Func_080072f8
.L68:
	movs	r3, #20
	ldr	r1, [pc, #556]
	str	r3, [sp, #0]
	movs	r3, #40
	str	r3, [sp, #4]
	ldr	r0, [sp, #132]
	add	r1, fp
	mov	r2, sl
	adds	r3, r6, #0
	bl	Func_080072f8
	movs	r0, #47
	bl	Func_08002dd8
.L64:
	bl	Func_080049ac
	ldr	r0, [sp, #108]
	ldr	r1, [sp, #48]
	bl	Func_080051d8
	movs	r0, #0
	str	r0, [sp, #60]
	str	r0, [sp, #128]
	ldr	r2, [pc, #504]
	mov	r1, fp
	ldr	r3, [r1, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	bne.n	.L69
	b.n	.L70
.L69:
	movs	r3, #36
	str	r3, [sp, #32]
	str	r0, [sp, #28]
.L114:
	mov	r4, fp
	ldr	r3, [r4, r2]
	ldr	r6, [sp, #32]
	ldrsh	r0, [r3, r6]
	bl	Func_080b5098
	ldr	r0, [r0, #0]
	ldr	r1, [sp, #88]
	str	r0, [sp, #56]
	movs	r0, #0
	str	r0, [sp, #124]
	cmp	r1, #0
	bne.n	.L71
	b.n	.L72
.L71:
	ldr	r4, [sp, #60]
	lsls	r3, r4, #3
	subs	r3, r3, r4
	ldr	r6, [pc, #460]
	ldr	r2, [sp, #28]
	lsls	r3, r3, #2
	adds	r6, r3, r6
	str	r2, [sp, #44]
	str	r6, [sp, #40]
	str	r0, [sp, #36]
.L113:
	ldr	r7, [sp, #36]
	ldr	r0, [sp, #44]
	ldr	r1, [sp, #120]
	adds	r3, r7, r0
	cmp	r3, r1
	blt.n	.L73
	b.n	.L74
.L73:
	ldr	r3, [sp, #88]
	ldr	r4, [sp, #128]
	adds	r2, r3, #0
	muls	r2, r4
	ldr	r6, [sp, #124]
	adds	r2, r2, r6
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	movs	r7, #225
	add	r3, fp
	lsls	r7, r7, #7
	adds	r6, r3, r7
	add	r7, sp, #144
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl	Func_080e3944
	ldr	r3, [r7, #0]
	asrs	r3, r3, #1
	str	r3, [r7, #0]
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #12]
	adds	r2, r2, r3
	str	r2, [r6, #0]
	ldr	r3, [r6, #16]
	ldr	r2, [r6, #4]
	adds	r2, r2, r3
	str	r2, [r6, #4]
	ldr	r3, [r6, #20]
	ldr	r2, [r6, #8]
	mov	r0, r8
	adds	r2, r2, r3
	str	r2, [r6, #8]
	cmp	r0, #6
	bne.n	.L75
	movs	r1, #128
	ldr	r5, [pc, #364]
	movs	r2, #0
	lsls	r1, r1, #2
	mov	sl, r7
	movs	r4, #255
.L77:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	bne.n	.L76
	mov	r0, sl
	ldr	r3, [r0, #0]
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	ldr	r3, [r0, #4]
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	str	r4, [sp, #8]
	bl	Func_08004458
	ldr	r4, [sp, #8]
	ands	r0, r4
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	Func_08004458
	ldr	r4, [sp, #8]
	ands	r0, r4
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r5, #16]
	bl	Func_08004458
	movs	r3, #7
	ldr	r2, [sp, #12]
	ands	r3, r0
	adds	r3, #32
	adds	r2, #1
	str	r3, [r5, #24]
	ldr	r1, [sp, #16]
	ldr	r4, [sp, #8]
	cmp	r2, #2
	beq.n	.L75
.L76:
	movs	r3, #128
	adds	r1, #1
	lsls	r3, r3, #3
	adds	r5, #28
	cmp	r1, r3
	bne.n	.L77
.L75:
	ldr	r3, [r6, #24]
	cmp	r3, #0
	beq.n	.L78
	b.n	.L79
.L78:
	ldr	r4, [sp, #56]
	ldr	r1, [r4, #8]
	ldr	r2, [r6, #0]
	cmp	r1, #0
	bge.n	.L80
	lsrs	r3, r2, #31
	cmp	r3, #0
	bne.n	.L81
	b.n	.L79
.L80:
	mvns	r3, r2
	lsrs	r3, r3, #31
	cmp	r3, #0
	bne.n	.L81
	b.n	.L79
.L81:
	adds	r3, r2, #0
	cmp	r3, #0
	bge.n	.L82
	negs	r3, r3
.L82:
	adds	r2, r1, #0
	cmp	r2, #0
	bge.n	.L83
	negs	r2, r2
.L83:
	cmp	r3, r2
	bge.n	.L84
	b.n	.L79
.L84:
	movs	r0, #0
	movs	r3, #1
	mov	r1, r8
	str	r0, [sp, #52]
	ldr	r2, [sp, #40]
	str	r3, [r6, #24]
	cmp	r1, #5
	bne.n	.L85
	movs	r0, #134
	str	r2, [sp, #12]
	bl	Func_080b50e8
	b.n	.L86
.L85:
	mov	r3, r8
	cmp	r3, #6
	beq.n	.L87
	ldr	r4, [sp, #100]
	cmp	r4, #0
	bne.n	.L87
	movs	r0, #8
	str	r0, [sp, #100]
	movs	r0, #132
	str	r2, [sp, #12]
	bl	Func_080f9010
.L86:
	ldr	r2, [sp, #12]
.L87:
	mov	r1, r8
	cmp	r1, #2
	bne.n	.L88
	str	r2, [sp, #12]
	bl	Func_08004458
	movs	r3, #31
	ands	r3, r0
	adds	r3, #32
	lsls	r5, r3, #12
	bl	Func_08004458
	movs	r3, #1
	ands	r0, r3
	ldr	r2, [sp, #12]
	cmp	r0, #0
	beq.n	.L89
	ldr	r3, [r6, #16]
	adds	r3, r3, r5
	b.n	.L90
.L89:
	ldr	r3, [r6, #16]
	subs	r3, r3, r5
.L90:
	str	r3, [r6, #16]
	ldr	r3, [r6, #12]
	negs	r3, r3
	str	r3, [r6, #12]
.L88:
	movs	r3, #1
	str	r3, [r2, #24]
	ldr	r3, [r7, #0]
	str	r3, [r2, #0]
	ldr	r3, [r7, #4]
	str	r3, [r2, #4]
	movs	r3, #0
	str	r3, [r2, #8]
	mov	r2, r8
	cmp	r2, #7
	beq.n	.L91
	ldr	r2, [pc, #84]
	movs	r3, #2
	add	r2, fp
	str	r3, [r2, #0]
.L91:
	ldr	r5, [pc, #56]
	add	r5, fp
	ldr	r3, [r5, #0]
	ldr	r4, [sp, #32]
	ldrsh	r0, [r3, r4]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r1, #7
	ldr	r3, [sp, #128]
	bl	Func_080d6888
	mov	r2, r8
	cmp	r2, #7
	beq.n	.L92
	cmp	r2, #9
	beq.n	.L92
	cmp	r2, #10
	beq.n	.L92
	cmp	r2, #5
	bne.n	.L93
	ldr	r3, [r5, #0]
	ldr	r4, [sp, #32]
	ldrsh	r0, [r3, r4]
	movs	r1, #4
	bl	Func_080b5088
	b.n	.L92
	movs	r0, r0
	.4byte 0x00007828
	.4byte 0x03001e50
	.4byte 0x000065c0
	.4byte 0x02010000
	.4byte 0x02013800
	.4byte 0x000077a8
.L93:
	ldr	r3, [r5, #0]
	ldr	r2, [sp, #32]
	movs	r1, #5
	ldrsh	r0, [r3, r2]
	bl	Func_080b5088
.L92:
	movs	r2, #5
	mov	r0, r8
	eors	r2, r0
	negs	r3, r2
	orrs	r3, r2
	lsrs	r2, r3, #31
	movs	r3, #12
	subs	r2, r3, r2
	movs	r3, #255
	mov	r9, r3
	ldr	r3, [sp, #84]
	add	r3, r8
	adds	r3, #4
	ldr	r5, [pc, #852]
	movs	r1, #100
	adds	r4, r7, #0
	mov	sl, r3
.L95:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	bne.n	.L94
	ldr	r3, [r4, #0]
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	ldr	r3, [r4, #4]
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	str	r4, [sp, #8]
	bl	Func_08004458
	mov	r3, r9
	ldr	r2, [sp, #12]
	ands	r0, r3
	subs	r0, #128
	lsls	r0, r2
	str	r0, [r5, #12]
	bl	Func_08004458
	mov	r3, r9
	ands	r0, r3
	ldr	r2, [sp, #12]
	subs	r0, #128
	lsls	r0, r2
	str	r0, [r5, #16]
	bl	Func_08004458
	movs	r3, #7
	ands	r3, r0
	adds	r3, #16
	str	r3, [r5, #24]
	ldr	r0, [sp, #52]
	adds	r0, #1
	str	r0, [sp, #52]
	ldr	r3, [pc, #772]
	mov	r0, sl
	ldrb	r3, [r3, r0]
	mov	ip, r3
	ldr	r3, [sp, #52]
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #12]
	ldr	r4, [sp, #8]
	cmp	r3, ip
	beq.n	.L79
.L94:
	movs	r0, #128
	adds	r1, #1
	lsls	r0, r0, #2
	adds	r5, #28
	cmp	r1, r0
	bne.n	.L95
.L79:
	mov	r1, r8
	cmp	r1, #0
	beq.n	.L96
	cmp	r1, #5
	beq.n	.L96
	cmp	r1, #8
	bne.n	.L97
.L96:
	ldr	r3, [pc, #732]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L98
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	bl	Func_080ed408
	b.n	.L99
.L98:
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	bl	Func_080ed408
.L99:
	ldr	r4, [sp, #92]
	ldr	r3, [pc, #688]
	ldrb	r0, [r3, r4]
	movs	r1, #32
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	ldr	r6, [pc, #680]
	str	r1, [sp, #0]
	str	r0, [sp, #4]
	movs	r1, #128
	lsls	r1, r1, #5
	subs	r3, r3, r0
	subs	r2, #16
	ldr	r4, [r6, #0]
	ldr	r0, [sp, #132]
	add	r1, fp
	bl	Func_080072f4
	movs	r0, #47
	bl	Func_08002dd8
	ldr	r3, [pc, #644]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L100
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	movs	r3, #15
	bl	Func_080ed408
	b.n	.L101
.L100:
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	movs	r3, #11
	bl	Func_080ed408
.L101:
	movs	r1, #32
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	str	r1, [sp, #0]
	ldr	r7, [sp, #92]
	ldr	r1, [pc, #592]
	ldrb	r1, [r1, r7]
	ldr	r0, [pc, #592]
	str	r1, [sp, #4]
	movs	r1, #128
	lsls	r1, r1, #5
	ldr	r4, [r0, #0]
	subs	r2, #16
	ldr	r0, [sp, #132]
	add	r1, fp
	bl	Func_080072f4
	movs	r0, #47
	bl	Func_08002dd8
	b.n	.L74
.L97:
	mov	r1, r8
	cmp	r1, #1
	bne.n	.L102
	ldr	r0, [r6, #0]
	cmp	r0, #0
	bge.n	.L103
	negs	r0, r0
.L103:
	ldr	r2, [sp, #56]
	ldr	r1, [r2, #8]
	cmp	r1, #0
	bge.n	.L104
	negs	r1, r1
.L104:
	cmp	r0, r1
	bgt.n	.L74
	movs	r1, #6
	ldr	r0, [sp, #120]
	bl	Func_080022fc
	lsls	r1, r0, #1
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	adds	r1, r1, r0
	movs	r0, #32
	lsls	r1, r1, #8
	str	r0, [sp, #0]
	movs	r0, #24
	str	r0, [sp, #4]
	add	r1, fp
	subs	r2, #16
	subs	r3, #12
	ldr	r0, [sp, #132]
	b.n	.L105
.L102:
	mov	r6, r8
	cmp	r6, #7
	beq.n	.L106
	cmp	r6, #9
	beq.n	.L106
	cmp	r6, #10
	bne.n	.L107
.L106:
	ldr	r0, [sp, #124]
	movs	r3, #3
	ands	r3, r0
	ldr	r2, [pc, #484]
	lsls	r3, r3, #1
	ldrh	r1, [r2, r3]
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	movs	r0, #8
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	add	r1, fp
	subs	r2, #4
	subs	r3, #4
	ldr	r0, [sp, #132]
	b.n	.L105
.L107:
	mov	r6, r8
	cmp	r6, #2
	bne.n	.L108
	movs	r1, #6
	ldr	r0, [sp, #124]
	bl	Func_080022fc
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	adds	r1, r0, #0
	movs	r0, #8
	lsls	r1, r1, #7
	str	r0, [sp, #0]
	movs	r0, #16
	str	r0, [sp, #4]
	add	r1, fp
	subs	r2, #4
	subs	r3, #8
	ldr	r0, [sp, #132]
	b.n	.L109
.L108:
	mov	r0, r8
	cmp	r0, #3
	bne.n	.L110
	movs	r1, #18
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	str	r1, [sp, #0]
	movs	r1, #13
	str	r1, [sp, #4]
	mov	r1, fp
	subs	r2, #9
	subs	r3, #7
	ldr	r0, [sp, #132]
	adds	r1, #96
	b.n	.L105
.L110:
	mov	r6, r8
	cmp	r6, #4
	bne.n	.L111
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	movs	r1, #12
	str	r1, [sp, #0]
	movs	r1, #8
	str	r1, [sp, #4]
	subs	r2, #6
	subs	r3, #4
	ldr	r0, [sp, #132]
	mov	r1, fp
.L109:
	ldr	r7, [sp, #112]
	bl	Func_08007300
	b.n	.L74
.L111:
	mov	r0, r8
	cmp	r0, #11
	bne.n	.L112
	movs	r1, #29
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	str	r1, [sp, #0]
	movs	r1, #23
	str	r1, [sp, #4]
	movs	r1, #165
	lsls	r1, r1, #1
	subs	r2, #15
	subs	r3, #12
	ldr	r0, [sp, #132]
	add	r1, fp
.L105:
	ldr	r4, [sp, #112]
	bl	Func_080072f4
	b.n	.L74
.L112:
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	movs	r1, #40
	str	r1, [sp, #0]
	movs	r1, #64
	str	r1, [sp, #4]
	subs	r2, #20
	subs	r3, #32
	ldr	r0, [sp, #132]
	mov	r1, fp
	ldr	r6, [sp, #112]
	bl	Func_080072fc
.L74:
	ldr	r7, [sp, #36]
	ldr	r1, [sp, #124]
	ldr	r2, [sp, #40]
	ldr	r3, [sp, #60]
	ldr	r0, [sp, #80]
	ldr	r4, [sp, #88]
	adds	r7, r7, r0
	adds	r1, #1
	adds	r2, #28
	adds	r3, #1
	str	r7, [sp, #36]
	str	r1, [sp, #124]
	str	r2, [sp, #40]
	str	r3, [sp, #60]
	cmp	r1, r4
	beq.n	.L72
	b.n	.L113
.L72:
	ldr	r6, [sp, #32]
	ldr	r7, [sp, #28]
	ldr	r1, [sp, #128]
	ldr	r0, [sp, #76]
	adds	r6, #2
	adds	r7, r7, r0
	adds	r1, #1
	str	r6, [sp, #32]
	str	r7, [sp, #28]
	str	r1, [sp, #128]
	ldr	r2, [pc, #224]
	mov	r4, fp
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #20]
	cmp	r1, r3
	beq.n	.L70
	b.n	.L114
.L70:
	mov	r6, r8
	cmp	r6, #0
	beq.n	.L115
	cmp	r6, #5
	beq.n	.L115
	cmp	r6, #8
	beq.n	.L115
	b.n	.L116
.L115:
	movs	r7, #0
	ldr	r3, [pc, #192]
	str	r7, [sp, #128]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r0, [sp, #88]
	ldr	r3, [r3, #20]
	muls	r3, r0
	cmp	r3, #0
	bne.n	.L117
	b.n	.L116
.L117:
	ldr	r1, [pc, #188]
	ldr	r2, [pc, #188]
	ldr	r7, [pc, #192]
	mov	r9, r1
	mov	sl, r2
.L122:
	ldr	r1, [r7, #24]
	cmp	r1, #1
	beq.n	.L118
	b.n	.L119
.L118:
	ldr	r3, [r7, #8]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r6, r3, #1
	ldr	r3, [sp, #128]
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L120
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #47
	bl	Func_080ed408
	lsls	r5, r6, #1
	mov	r4, r9
	mov	r0, sl
	ldrh	r1, [r4, r5]
	ldr	r2, [r7, #0]
	ldrb	r4, [r0, r6]
	ldr	r3, [pc, #136]
	subs	r2, r2, r4
	ldrb	r0, [r3, r6]
	ldr	r3, [r7, #4]
	str	r4, [sp, #0]
	ldr	r4, [pc, #132]
	subs	r3, r3, r0
	ldrb	r0, [r4, r6]
	str	r0, [sp, #4]
	ldr	r0, [pc, #100]
	add	r1, fp
	ldr	r4, [r0, #0]
	ldr	r0, [sp, #132]
	bl	Func_080072f4
	movs	r0, #47
	bl	Func_08002dd8
	movs	r1, #2
	str	r1, [sp, #0]
	movs	r2, #7
	movs	r1, #7
	movs	r3, #15
	movs	r0, #47
	bl	Func_080ed408
	ldr	r3, [pc, #92]
	ldr	r4, [pc, #84]
	ldrb	r0, [r3, r6]
	ldr	r3, [r7, #4]
	adds	r3, r3, r0
	ldrb	r0, [r4, r6]
	mov	r4, sl
	subs	r3, r3, r0
	ldrb	r0, [r4, r6]
	mov	r2, r9
	ldrh	r1, [r2, r5]
	ldr	r4, [pc, #68]
	ldr	r2, [r7, #0]
	str	r0, [sp, #0]
	ldrb	r0, [r4, r6]
	ldr	r6, [pc, #36]
	str	r0, [sp, #4]
	add	r1, fp
	ldr	r4, [r6, #0]
	ldr	r0, [sp, #132]
	bl	Func_080072f4
	movs	r0, #47
	bl	Func_08002dd8
	b.n	.L121
	.4byte 0x02010af0
	.4byte 0x080eebec
	.4byte 0x00007828
	.4byte 0x080eebe9
	.4byte 0x03001f0c
	.4byte 0x080eec52
	.4byte 0x080eec44
	.4byte 0x080eec28
	.4byte 0x02010000
	.4byte 0x080eec3d
	.4byte 0x080eec2f
	.4byte 0x080eec36
.L120:
	movs	r0, #2
	str	r0, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #47
	bl	Func_080ed408
	lsls	r5, r6, #1
	mov	r2, r9
	ldrh	r1, [r2, r5]
	ldr	r4, [pc, #436]
	ldr	r3, [pc, #436]
	add	r1, fp
	adds	r1, r1, r3
	ldr	r2, [r7, #0]
	ldrb	r3, [r4, r6]
	mov	r0, sl
	ldrb	r4, [r0, r6]
	subs	r2, r2, r3
	ldr	r3, [r7, #4]
	subs	r3, r3, r4
	mov	ip, r3
	ldr	r3, [pc, #420]
	ldrb	r0, [r3, r6]
	str	r4, [sp, #4]
	str	r0, [sp, #0]
	ldr	r0, [pc, #416]
	mov	r3, ip
	ldr	r4, [r0, #0]
	ldr	r0, [sp, #132]
	bl	Func_080072f4
	movs	r0, #47
	bl	Func_08002dd8
	movs	r1, #2
	str	r1, [sp, #0]
	movs	r2, #7
	movs	r1, #7
	movs	r3, #15
	movs	r0, #47
	bl	Func_080ed408
	mov	r2, r9
	ldrh	r1, [r2, r5]
	ldr	r3, [pc, #368]
	add	r1, fp
	adds	r1, r1, r3
	ldr	r3, [pc, #376]
	ldr	r2, [r7, #0]
	ldrb	r3, [r3, r6]
	ldr	r4, [pc, #352]
	adds	r2, r2, r3
	ldrb	r3, [r4, r6]
	ldr	r4, [pc, #356]
	ldrb	r0, [r4, r6]
	subs	r2, r2, r3
	ldr	r3, [r7, #4]
	str	r0, [sp, #0]
	mov	r4, sl
	ldrb	r0, [r4, r6]
	ldr	r6, [pc, #344]
	str	r0, [sp, #4]
	ldr	r0, [sp, #132]
	ldr	r4, [r6, #0]
	bl	Func_080072f4
	movs	r0, #47
	bl	Func_08002dd8
.L121:
	ldr	r3, [r7, #8]
	adds	r3, #1
	str	r3, [r7, #8]
	cmp	r3, #12
	bne.n	.L119
	movs	r3, #0
	str	r3, [r7, #24]
.L119:
	ldr	r0, [sp, #128]
	ldr	r3, [pc, #320]
	adds	r0, #1
	str	r0, [sp, #128]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r1, [sp, #88]
	ldr	r3, [r3, #20]
	muls	r3, r1
	adds	r7, #28
	cmp	r0, r3
	beq.n	.L116
	b.n	.L122
.L116:
	movs	r2, #100
	str	r2, [sp, #128]
	ldr	r5, [pc, #296]
.L125:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	ble.n	.L123
	asrs	r0, r0, #3
	adds	r0, #1
	lsls	r4, r0, #1
	ldr	r2, [pc, #284]
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #104]
	adds	r1, r3, r1
	lsrs	r3, r0, #31
	movs	r6, #2
	ldrsh	r2, [r5, r6]
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #132]
	ldr	r4, [sp, #112]
	bl	Func_080072f4
	movs	r2, #128
	adds	r0, r5, #0
	movs	r1, #60
	lsls	r2, r2, #5
	bl	Func_080e3908
	movs	r6, #224
	ldr	r3, [r5, #4]
	lsls	r6, r6, #15
	cmp	r3, r6
	ble.n	.L124
	ldr	r3, [r5, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r5, #16]
.L124:
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L123:
	ldr	r7, [sp, #128]
	movs	r0, #128
	adds	r7, #1
	lsls	r0, r0, #2
	adds	r5, #28
	str	r7, [sp, #128]
	cmp	r7, r0
	bne.n	.L125
	ldr	r6, [pc, #188]
	ldr	r5, [pc, #192]
.L127:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	ble.n	.L126
	asrs	r0, r0, #4
	adds	r0, #1
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r6, r3]
	ldr	r2, [sp, #104]
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #132]
	ldr	r4, [sp, #112]
	bl	Func_080072f4
	adds	r0, r5, #0
	movs	r1, #60
	ldr	r2, [pc, #140]
	bl	Func_080e3908
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L126:
	ldr	r7, [sp, #128]
	movs	r0, #128
	adds	r7, #1
	lsls	r0, r0, #3
	adds	r5, #28
	str	r7, [sp, #128]
	cmp	r7, r0
	bne.n	.L127
	movs	r1, #4
	movs	r0, #4
	bl	Func_080e155c
	bl	Func_080cd52c
	ldr	r2, [pc, #104]
	movs	r3, #1
	add	r2, fp
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r1, [sp, #120]
	ldr	r2, [sp, #96]
	adds	r1, #1
	str	r1, [sp, #120]
	cmp	r1, r2
	beq.n	.L58
	bl	.L128
.L58:
	ldr	r0, [pc, #76]
	bl	Func_08004278
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #156
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x080eec3d
	.4byte 0x0000128a
	.4byte 0x080eec2f
	.4byte 0x03001f0c
	.4byte 0x080eec36
	.4byte 0x00007828
	.4byte 0x02010af0
	.4byte 0x080ede48
	.4byte 0x02013800
	.4byte 0xffffc000
	.4byte 0x00007824
	.4byte 0x080cd261
