@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808d9a4
	.thumb_func
Func_0808d9a4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r8, r0
	ldr	r3, [pc, #444]
	movs	r0, #250
	lsls	r0, r0, #1
	mov	r5, r8
	adds	r3, r3, r0
	subs	r5, #242
	ldr	r6, [r3, #0]
	cmp	r5, #5
	bhi.n	.L0
	bl	Func_08091660
	ldr	r3, [pc, #428]
	ldrb	r3, [r3, r5]
	ldr	r0, [pc, #428]
	mov	r8, r3
	add	r0, r8
	movs	r1, #1
	bl	Func_08015040
	ldr	r0, [pc, #420]
	movs	r1, #1
	add	r0, r8
	bl	Func_08015040
	b.n	.L1
.L0:
	movs	r0, #3
	mov	r1, r8
	bl	Func_0808d48c
	adds	r7, r0, #0
	cmp	r7, #0
	bne.n	.L2
	b.n	.L3
.L2:
	ldr	r3, [r7, #0]
	asrs	r5, r3, #4
	movs	r3, #31
	movs	r2, #6
	ldrsh	r1, [r7, r2]
	ands	r5, r3
	ldrh	r2, [r7, #4]
	movs	r3, #128
	lsls	r3, r3, #3
	ands	r3, r2
	mov	sl, r1
	cmp	r3, #0
	bne.n	.L4
	cmp	r5, #0
	beq.n	.L4
	bl	Func_08091660
	ldr	r0, [pc, #352]
	movs	r1, #1
	adds	r0, r5, r0
	bl	Func_08015040
	movs	r0, #161
	lsls	r0, r0, #1
	bl	Func_080770c8
	b.n	.L5
.L4:
	movs	r0, #161
	lsls	r0, r0, #1
	bl	Func_080770d0
.L5:
	ldr	r2, [r7, #8]
	movs	r3, #240
	lsls	r3, r3, #20
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L6
	ldr	r3, [pc, #320]
	movs	r0, #128
	ands	r3, r2
	lsls	r0, r0, #15
	cmp	r3, r0
	bne.n	.L7
	b.n	.L8
.L6:
	mov	r0, sl
	bl	Func_0808d428
	cmp	r0, #0
	beq.n	.L9
	ldr	r3, [pc, #280]
	movs	r1, #250
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	ldr	r3, [r7, #8]
	bl	Func_080072f0
.L9:
	movs	r0, #161
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L10
	b.n	.L11
.L10:
	ldr	r0, [pc, #264]
	movs	r1, #1
	adds	r0, r5, r0
	bl	Func_08015040
	b.n	.L11
.L8:
	mov	r0, sl
	bl	Func_0808d428
	cmp	r0, #0
	beq.n	.L12
	ldrh	r0, [r7, #8]
	b.n	.L13
.L12:
	ldr	r0, [pc, #244]
.L13:
	movs	r1, #1
	bl	Func_08015040
	b.n	.L11
.L7:
	bl	Func_080916b0
	mov	r0, sl
	bl	Func_0808d428
	cmp	r0, #0
	bne.n	.L14
	b.n	.L15
.L14:
	ldr	r1, [r7, #8]
	movs	r3, #240
	lsls	r3, r3, #12
	movs	r0, #128
	ands	r3, r1
	lsls	r0, r0, #9
	movs	r2, #1
	cmp	r3, r0
	bne.n	.L16
	cmp	r6, #7
	bgt.n	.L16
	movs	r2, #0
.L16:
	cmp	r2, #0
	bne.n	.L17
	b.n	.L18
.L17:
	ldr	r2, [pc, #192]
	ldr	r3, [r7, #0]
	ands	r3, r2
	mov	fp, r2
	cmp	r3, #19
	bne.n	.L19
	mov	r0, r8
	bl	Func_0808ece0
	ldr	r1, [r7, #8]
.L19:
	ldr	r3, [pc, #164]
	movs	r0, #192
	ands	r3, r1
	lsls	r0, r0, #14
	cmp	r3, r0
	bne.n	.L20
	ldr	r3, [r7, #0]
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #19
	bne.n	.L21
	mov	r0, r8
	bl	Func_0808ed1c
.L21:
	mov	r0, r8
	bl	Func_0808ed4c
	adds	r6, r0, #0
	bl	Func_0808f0d8
	movs	r0, #83
	bl	Func_080f9010
	ldrh	r0, [r7, #8]
	movs	r1, #5
	bl	Func_08015120
	ldr	r5, [pc, #124]
	movs	r1, #3
	adds	r0, r5, #0
	bl	Func_08015040
	movs	r1, #0
	ldr	r0, [pc, #116]
	bl	Func_0808c2dc
	movs	r0, #1
	bl	Func_08015128
	movs	r0, #126
	bl	Func_080f9010
	adds	r0, r5, #1
	movs	r1, #1
	bl	Func_08015040
	bl	Func_08015138
	movs	r1, #2
	adds	r0, r6, #0
	bl	Func_08009080
	movs	r0, #246
	bl	Func_080f9010
	adds	r5, #2
	movs	r0, #30
	bl	Func_0809163c
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08015040
	mov	r0, r8
	bl	Func_0808ed78
	movs	r2, #1
	negs	r2, r2
	cmp	sl, r2
	bne.n	.L22
	b.n	.L23
.L22:
	mov	r0, sl
	bl	Func_080770c8
	b.n	.L23
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x0809e680
	.4byte 0x00000928
	.4byte 0x00000948
	.4byte 0xfff00000
	.4byte 0x00000976
	.4byte 0x000001ff
	.4byte 0x00000970
	.4byte 0x000003e7
.L20:
	movs	r0, #160
	lsls	r0, r0, #15
	cmp	r3, r0
	bne.n	.L24
	ldr	r3, [pc, #88]
	ldr	r5, [r3, #0]
	ldr	r3, [r7, #0]
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #19
	bne.n	.L25
	mov	r0, r8
	bl	Func_0808ec8c
.L25:
	movs	r2, #1
	negs	r2, r2
	cmp	sl, r2
	beq.n	.L26
	ldr	r2, [pc, #56]
	mov	r1, sl
	orrs	r1, r2
	ldr	r3, [pc, #60]
	movs	r0, #141
	lsls	r0, r0, #2
	mov	sl, r1
	adds	r3, r3, r0
	mov	r2, sl
	strh	r2, [r3, #0]
.L26:
	ldrh	r1, [r7, #8]
	movs	r0, #99
	bl	Func_0808b05c
	movs	r1, #190
	lsls	r1, r1, #1
	adds	r3, r5, r1
	strh	r0, [r3, #0]
	ldr	r5, [pc, #28]
	ldr	r3, [pc, #32]
	adds	r2, r5, r3
	movs	r3, #2
	strb	r3, [r2, #0]
	ldrh	r1, [r7, #8]
	movs	r0, #99
	bl	Func_0808b320
	movs	r0, #247
	b.n	.L27
	movs	r0, r0
	.4byte 0x00001000
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x0000022b
.L27:
	lsls	r0, r0, #1
	adds	r3, r5, r0
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	bl	Func_080f9010
	ldr	r0, [pc, #364]
	b.n	.L28
.L24:
	movs	r2, #128
	lsls	r2, r2, #14
	cmp	r3, r2
	bne.n	.L29
	ldr	r3, [pc, #356]
	movs	r0, #250
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl	Func_0808ef70
	adds	r5, r0, #0
	movs	r0, #30
	bl	Func_080030f8
	ldr	r3, [r7, #0]
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #19
	bne.n	.L30
	mov	r0, r8
	bl	Func_0808ed1c
.L30:
	adds	r0, r5, #0
	bl	Func_0808f0d8
	movs	r0, #83
	bl	Func_080f9010
	ldrh	r0, [r7, #8]
	movs	r1, #5
	bl	Func_08015120
	ldr	r0, [pc, #300]
	movs	r1, #3
	bl	Func_08015040
	ldrh	r0, [r7, #8]
	bl	Func_08077230
	movs	r2, #1
	negs	r2, r2
	cmp	sl, r2
	beq.n	.L31
	mov	r0, sl
	bl	Func_080770c8
.L31:
	adds	r0, r5, #0
	bl	Func_080090d0
	b.n	.L23
.L29:
	ldr	r3, [pc, #268]
	ldr	r2, [pc, #272]
	ldr	r0, [r3, #0]
	ands	r1, r2
	bl	Func_0808ef70
	mov	r9, r0
	movs	r0, #30
	bl	Func_080030f8
	ldrh	r0, [r7, #8]
	bl	Func_08077030
	movs	r3, #1
	adds	r6, r0, #0
	negs	r3, r3
	ldr	r5, [pc, #244]
	cmp	r6, r3
	bne.n	.L32
	ldr	r0, [r7, #8]
	ldr	r1, [pc, #232]
	ands	r0, r1
	movs	r1, #2
	bl	Func_08015120
	ldr	r5, [pc, #232]
	movs	r1, #1
	adds	r0, r5, #0
	adds	r5, #4
	bl	Func_08015040
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08015040
	mov	r0, r9
	bl	Func_0808f0c8
	ldr	r3, [r7, #0]
	mov	r2, fp
	ands	r3, r2
	cmp	r3, #19
	bne.n	.L23
	mov	r0, r8
	bl	Func_0808ec50
	b.n	.L23
.L32:
	ldr	r3, [r7, #0]
	mov	r0, fp
	ands	r3, r0
	cmp	r3, #19
	bne.n	.L33
	mov	r0, r8
	bl	Func_0808ed1c
.L33:
	mov	r0, r9
	bl	Func_0808f0d8
	movs	r0, #83
	bl	Func_080f9010
	ldr	r0, [r7, #8]
	movs	r1, #2
	ands	r0, r5
	bl	Func_08015120
	ldr	r1, [pc, #136]
	ldr	r3, [r1, #0]
	cmp	r6, r3
	bne.n	.L34
	ldr	r0, [pc, #144]
	movs	r1, #3
	bl	Func_08015040
	b.n	.L35
.L34:
	adds	r0, r6, #0
	movs	r1, #1
	bl	Func_08015120
	ldr	r0, [pc, #132]
	movs	r1, #3
	bl	Func_08015040
.L35:
	movs	r2, #1
	negs	r2, r2
	cmp	sl, r2
	beq.n	.L36
	mov	r0, sl
	bl	Func_080770c8
.L36:
	mov	r0, r9
	bl	Func_080090d0
	b.n	.L23
.L18:
	ldr	r0, [pc, #104]
.L28:
	movs	r1, #1
	bl	Func_08015040
	b.n	.L23
.L15:
	ldr	r0, [pc, #100]
	movs	r1, #1
	adds	r0, r5, r0
	bl	Func_08015040
.L23:
	bl	Func_08091750
	bl	Func_0809202c
	b.n	.L11
.L3:
	ldr	r0, [pc, #84]
	movs	r1, #1
	bl	Func_08015040
	ldr	r0, [pc, #80]
	movs	r1, #1
	bl	Func_08015040
.L11:
	movs	r0, #161
	lsls	r0, r0, #1
	bl	Func_080770d0
.L1:
	movs	r0, #0
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x00000973
	.4byte 0x02000240
	.4byte 0x00000969
	.4byte 0x02000434
	.4byte 0x00000fff
	.4byte 0x0000ffff
	.4byte 0x00000968
	.4byte 0x0000096a
	.4byte 0x0000096b
	.4byte 0x0000096f
	.4byte 0x00000948
	.4byte 0x0000092d
	.4byte 0x0000094d
