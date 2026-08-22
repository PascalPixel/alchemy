@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b6f44
	.thumb_func
Func_080b6f44:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #20
	str	r2, [sp, #16]
	adds	r6, r0, #0
	lsls	r2, r2, #16
	movs	r0, #240
	str	r3, [sp, #12]
	str	r2, [sp, #8]
	lsls	r3, r3, #16
	mov	r9, r1
	lsls	r0, r0, #8
	adds	r1, r2, #0
	movs	r2, #0
	str	r3, [sp, #4]
	bl	Func_080090c8
	mov	r8, r0
	mov	r0, r9
	bl	Func_08077008
	movs	r2, #0
	mov	sl, r0
	mov	r0, r9
	mov	fp, r2
	bl	Func_080b6d30
	ldr	r3, [pc, #904]
	add	r3, sl
	ldrb	r3, [r3, #0]
	adds	r7, r0, #0
	cmp	r3, #0
	bne.n	.L0
	movs	r5, #148
	lsls	r5, r5, #1
	add	r5, sl
	ldrb	r0, [r5, #0]
	bl	Func_080c2384
	adds	r1, r0, #0
	cmp	r7, #0
	bne.n	.L1
	ldrb	r0, [r5, #0]
	str	r1, [sp, #0]
	bl	Func_080c23a0
	mov	fp, r0
	ldr	r1, [sp, #0]
	b.n	.L2
.L1:
	adds	r1, r7, #0
	b.n	.L2
.L0:
	movs	r3, #148
	lsls	r3, r3, #1
	add	r3, sl
	ldrb	r3, [r3, #0]
	cmp	r3, #5
	bhi.n	.L3
	ldr	r2, [pc, #848]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080b6ff0
	.4byte 0x080b6fe0
	.4byte 0x080b6fe8
	.4byte 0x080b6fe4
	.4byte 0x080b6ff0
	.4byte 0x080b6fec
	ldr	r1, [pc, #820]
	b.n	.L4
	ldr	r1, [pc, #820]
	b.n	.L4
	movs	r1, #151
	b.n	.L5
	ldr	r1, [pc, #816]
	b.n	.L4
.L3:
	movs	r1, #150
.L5:
	lsls	r1, r1, #1
.L4:
	mov	r3, r9
	cmp	r3, #7
	bls.n	.L2
	movs	r2, #1
	mov	fp, r2
.L2:
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r6, #24]
	movs	r3, #148
	lsls	r3, r3, #1
	add	r3, sl
	ldrb	r3, [r3, #0]
	subs	r3, #18
	cmp	r3, #139
	bls.n	.L6
	b.n	.L7
.L6:
	ldr	r2, [pc, #780]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080b734c
	.4byte 0x080b734c
	.4byte 0x080b734c
	.4byte 0x080b734c
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b726a
	.4byte 0x080b734c
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b72aa
	.4byte 0x080b72ae
	.4byte 0x080b72b2
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7304
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b734c
	.4byte 0x080b734c
	.4byte 0x080b734c
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b724c
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b72b6
	.4byte 0x080b72bc
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7250
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b734c
	.4byte 0x080b7350
	.4byte 0x080b72ca
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b72d0
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b730a
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b72d4
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b7270
	.4byte 0x080b734c
	.4byte 0x080b734c
	.4byte 0x080b734c
	.4byte 0x080b734c
	.4byte 0x080b734c
	.4byte 0x080b7350
	.4byte 0x080b72c2
	.4byte 0x080b72c6
	.4byte 0x080b7254
	.4byte 0x080b7258
	.4byte 0x080b72d8
	.4byte 0x080b72dc
	.4byte 0x080b72e0
	.4byte 0x080b72e4
	.4byte 0x080b72e8
	.4byte 0x080b72ec
	.4byte 0x080b725c
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b72f0
	.4byte 0x080b7350
	.4byte 0x080b7350
	.4byte 0x080b72f6
	.4byte 0x080b72fa
	.4byte 0x080b7300
	.4byte 0x080b7262
	.4byte 0x080b7274
	.4byte 0x080b7266
	.4byte 0x080b727a
	.4byte 0x080b7280
	.4byte 0x080b7286
	.4byte 0x080b728c
	.4byte 0x080b7292
	.4byte 0x080b7298
	.4byte 0x080b729e
	.4byte 0x080b72a4
	ldr	r3, [pc, #216]
	b.n	.L8
	ldr	r3, [pc, #216]
	b.n	.L8
	ldr	r3, [pc, #216]
	b.n	.L8
	ldr	r3, [pc, #204]
	b.n	.L8
	movs	r3, #192
	lsls	r3, r3, #9
	b.n	.L8
	ldr	r3, [pc, #208]
	b.n	.L8
	ldr	r3, [pc, #204]
	b.n	.L8
	movs	r3, #128
	lsls	r3, r3, #9
	b.n	.L8
	ldr	r3, [pc, #196]
	b.n	.L8
	movs	r3, #192
	lsls	r3, r3, #9
	b.n	.L8
	movs	r3, #192
	lsls	r3, r3, #9
	b.n	.L8
	movs	r3, #192
	lsls	r3, r3, #9
	b.n	.L8
	movs	r3, #192
	lsls	r3, r3, #9
	b.n	.L8
	movs	r3, #192
	lsls	r3, r3, #9
	b.n	.L8
	movs	r3, #192
	lsls	r3, r3, #9
	b.n	.L8
	movs	r3, #192
	lsls	r3, r3, #9
	b.n	.L8
	movs	r3, #192
	lsls	r3, r3, #9
	b.n	.L8
	movs	r3, #192
	lsls	r3, r3, #9
	b.n	.L8
	ldr	r3, [pc, #132]
	b.n	.L8
	ldr	r3, [pc, #128]
	b.n	.L8
	ldr	r3, [pc, #136]
	b.n	.L8
	movs	r3, #128
	lsls	r3, r3, #9
	b.n	.L8
	movs	r3, #160
	lsls	r3, r3, #9
	b.n	.L8
	ldr	r3, [pc, #120]
	b.n	.L8
	ldr	r3, [pc, #116]
	b.n	.L8
	movs	r3, #192
	lsls	r3, r3, #9
	b.n	.L8
	ldr	r3, [pc, #108]
	b.n	.L8
	ldr	r3, [pc, #88]
	b.n	.L8
	ldr	r3, [pc, #104]
	b.n	.L8
	ldr	r3, [pc, #100]
	b.n	.L8
	ldr	r3, [pc, #100]
	b.n	.L8
	ldr	r3, [pc, #96]
	b.n	.L8
	ldr	r3, [pc, #68]
	b.n	.L8
	ldr	r3, [pc, #64]
	b.n	.L8
	movs	r3, #192
	lsls	r3, r3, #9
	b.n	.L8
	ldr	r3, [pc, #56]
	b.n	.L8
	movs	r3, #192
	lsls	r3, r3, #9
	b.n	.L8
	ldr	r3, [pc, #40]
	b.n	.L8
	movs	r3, #160
	lsls	r3, r3, #9
	b.n	.L8
	movs	r3, #160
	lsls	r3, r3, #9
	b.n	.L8
	.4byte 0x00000129
	.4byte 0x080b6fc8
	.4byte 0x0000012d
	.4byte 0x0000012f
	.4byte 0x00000131
	.4byte 0x080b701c
	.4byte 0x00019999
	.4byte 0x00018ccc
	.4byte 0x00013333
	.4byte 0x0001cccc
	.4byte 0x0001b333
	.4byte 0x00016666
	.4byte 0x00014ccc
	.4byte 0x00010ccc
	.4byte 0x00011999
	ldr	r3, [pc, #176]
.L8:
	str	r3, [r6, #24]
.L7:
	ldr	r2, [sp, #8]
	mov	r3, r8
	str	r3, [r6, #0]
	ldr	r3, [sp, #4]
	str	r2, [r6, #12]
	mov	r0, r9
	mov	r2, fp
	str	r3, [r6, #16]
	str	r2, [r6, #20]
	strh	r1, [r6, #4]
	bl	Func_080b6eb4
	movs	r5, #0
	strh	r5, [r6, #8]
	str	r5, [r6, #32]
	str	r5, [r6, #36]
	strh	r5, [r6, #40]
	strh	r5, [r6, #42]
	movs	r5, #148
	movs	r3, #255
	lsls	r5, r5, #1
	lsls	r3, r3, #1
	add	r5, sl
	strh	r3, [r6, #10]
	ldrb	r3, [r5, #0]
	strh	r0, [r6, #6]
	cmp	r3, #1
	bhi.n	.L9
	mov	r0, sl
	movs	r1, #1
	bl	Func_08077078
	cmp	r0, #15
	bne.n	.L9
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L10
	movs	r1, #240
	b.n	.L11
.L10:
	movs	r1, #241
.L11:
	lsls	r1, r1, #1
	strh	r1, [r6, #4]
	movs	r3, #0
	strh	r3, [r6, #6]
.L9:
	ldr	r3, [sp, #12]
	cmp	r3, #0
	bge.n	.L12
	adds	r3, #7
.L12:
	asrs	r0, r3, #3
	ldr	r1, [sp, #16]
	bl	Func_080044d0
	movs	r3, #128
	lsls	r3, r3, #8
	adds	r0, r0, r3
	mov	r2, r8
	movs	r3, #3
	strh	r0, [r2, #6]
	adds	r2, #89
	strb	r3, [r2, #0]
	movs	r3, #2
	subs	r2, #4
	strb	r3, [r2, #0]
	ldr	r3, [pc, #52]
	add	r3, sl
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L13
	ldr	r3, [pc, #44]
	b.n	.L14
.L13:
	movs	r3, #128
	lsls	r3, r3, #9
.L14:
	mov	r2, r8
	str	r3, [r2, #24]
	str	r3, [r2, #28]
	ldr	r1, [pc, #36]
	mov	r0, r8
	bl	Func_08009098
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x0000e666
	.4byte 0x00000129
	.4byte 0x00014ccc
	.4byte 0x080c5938
