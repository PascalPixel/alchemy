@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808e680
	.thumb_func
Func_0808e680:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r8, r0
	ldr	r0, [pc, #672]
	ldr	r3, [pc, #676]
	mov	r2, r8
	ands	r2, r0
	mov	r9, r2
	ldr	r3, [r3, #0]
	mov	r0, r9
	sub	sp, #12
	mov	sl, r3
	bl	Func_08077080
	mov	r3, r8
	ldrb	r0, [r0, #12]
	lsrs	r6, r3, #10
	movs	r3, #15
	ands	r6, r3
	mov	fp, r0
	ldr	r3, [pc, #648]
	movs	r0, #250
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	bl	Func_0808ba1c
	movs	r2, #0
	str	r2, [sp, #0]
	bl	Func_08091660
	ldr	r0, [pc, #628]
	bl	Func_080770d0
	cmp	r6, #15
	bne.n	.L0
	movs	r6, #0
.L0:
	movs	r0, #191
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L1
	adds	r0, r6, #0
	movs	r1, #1
	bl	Func_08015120
	mov	r0, r9
.L4:
	movs	r1, #4
	bl	Func_08015120
	ldr	r0, [pc, #592]
.L7:
	movs	r1, #1
	bl	Func_08015040
	b.n	.L2
.L1:
	movs	r3, #207
	lsls	r3, r3, #1
	add	r3, sl
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #3
	bne.n	.L3
	mov	r2, r9
	cmp	r2, #144
	bne.n	.L3
	adds	r0, r6, #0
	movs	r1, #1
	bl	Func_08015120
	movs	r0, #144
	b.n	.L4
.L3:
	mov	r3, r9
	cmp	r3, #149
	bne.n	.L5
	movs	r0, #162
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L6
	adds	r0, r6, #0
	movs	r1, #1
	bl	Func_08015120
	movs	r0, #149
	movs	r1, #4
	bl	Func_08015120
	ldr	r0, [pc, #520]
	b.n	.L7
.L6:
	movs	r0, #149
	movs	r1, #4
	bl	Func_08015120
	movs	r1, #13
	ldr	r0, [pc, #512]
	bl	Func_08015040
	movs	r0, #1
	bl	Func_08091d84
	adds	r5, r0, #0
	bl	Func_08015140
	movs	r0, #0
	cmp	r5, #0
	beq.n	.L8
	b.n	.L9
.L8:
	ldr	r1, [pc, #468]
	movs	r0, #144
	lsls	r0, r0, #2
	adds	r3, r1, r0
	ldrh	r2, [r3, #0]
	subs	r0, #128
	adds	r3, r1, r0
	strh	r2, [r3, #0]
	ldr	r2, [pc, #472]
	adds	r3, r1, r2
	ldrh	r3, [r3, #0]
	adds	r0, #2
	adds	r2, r1, r0
	strh	r3, [r2, #0]
	movs	r2, #184
	lsls	r2, r2, #1
	ldr	r3, [pc, #460]
	add	r2, sl
	strh	r3, [r2, #0]
	movs	r2, #1
	str	r2, [sp, #0]
.L5:
	movs	r7, #128
	lsls	r7, r7, #6
	mov	r3, r8
	ands	r7, r3
	cmp	r7, #0
	beq.n	.L10
	mov	r0, r8
	bl	Func_0808e5d8
	b.n	.L9
.L10:
	cmp	r6, #7
	bgt.n	.L11
	mov	r0, r9
	bl	Func_08077080
	ldrb	r5, [r0, #9]
	adds	r0, r6, #0
	bl	Func_08077008
	movs	r2, #58
	ldrsh	r3, [r0, r2]
	cmp	r3, r5
	bge.n	.L12
	adds	r0, r6, #0
	movs	r1, #1
	bl	Func_08015120
	mov	r0, r9
	movs	r1, #4
	bl	Func_08015120
	ldr	r0, [pc, #392]
	movs	r1, #1
	bl	Func_08015040
	ldr	r3, [sp, #0]
	cmp	r3, #0
	bne.n	.L13
	b.n	.L2
.L13:
	movs	r3, #184
	lsls	r3, r3, #1
	add	r3, sl
	strh	r7, [r3, #0]
	b.n	.L2
.L12:
	negs	r1, r5
	adds	r0, r6, #0
	bl	Func_08077120
.L11:
	add	r5, sp, #8
	adds	r2, r5, #0
	mov	r1, fp
	ldr	r0, [pc, #356]
	bl	Func_0808e4b4
	adds	r2, r5, #0
	str	r0, [sp, #4]
	mov	r1, fp
	movs	r0, #5
	bl	Func_0808e4b4
	adds	r2, r5, #0
	mov	r1, fp
	mov	r8, r0
	ldr	r0, [pc, #336]
	bl	Func_0808e4b4
	ldr	r5, [pc, #336]
	adds	r7, r0, #0
	movs	r3, #1
	movs	r0, #160
	negs	r3, r3
	lsls	r0, r0, #1
	str	r3, [sp, #8]
	bl	Func_080770c8
	adds	r0, r5, #0
	bl	Func_080770c8
	ldr	r0, [sp, #4]
	cmp	r0, #0
	bne.n	.L14
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L14
	cmp	r7, #0
	beq.n	.L15
.L14:
	ldr	r3, [pc, #256]
	movs	r0, #250
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	mov	r1, fp
	bl	Func_0808df1c
	mov	r2, r8
	str	r0, [sp, #8]
	cmp	r2, #0
	beq.n	.L16
	ldrh	r2, [r2, #4]
	movs	r3, #128
	lsls	r3, r3, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L16
	movs	r0, #160
	lsls	r0, r0, #1
	bl	Func_080770d0
	ldr	r0, [pc, #252]
	bl	Func_080770d0
	b.n	.L16
.L15:
	adds	r0, r5, #0
	bl	Func_080770d0
.L16:
	movs	r3, #207
	lsls	r3, r3, #1
	add	r3, sl
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #3
	bne.n	.L17
	bl	Func_0808b8e8
.L17:
	mov	r0, r9
	movs	r1, #0
	bl	Func_08096fb0
	ldr	r2, [pc, #216]
	movs	r3, #1
	add	r2, sl
	strb	r3, [r2, #0]
	ldr	r3, [pc, #164]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	ldr	r1, [sp, #8]
	bl	Func_080970f8
	bl	Func_0809728c
	adds	r1, r6, #0
	ldr	r2, [sp, #8]
	ldr	r0, [sp, #4]
	bl	Func_08096b28
	movs	r0, #160
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L18
	ldr	r0, [pc, #160]
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L19
	bl	Func_08096960
	b.n	.L18
.L19:
	bl	Func_08096810
.L18:
	bl	Func_08097174
	movs	r5, #160
	ldr	r2, [sp, #8]
	mov	r0, r8
	adds	r1, r6, #0
	lsls	r5, r5, #1
	bl	Func_08096b28
	adds	r0, r5, #0
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L20
	bl	Func_08096ab0
.L20:
	adds	r0, r5, #0
	bl	Func_080770d0
	ldr	r0, [pc, #104]
	bl	Func_080770d0
	ldr	r2, [pc, #100]
	movs	r3, #0
	add	r2, sl
	strb	r3, [r2, #0]
	bl	Func_08097194
	movs	r3, #207
	lsls	r3, r3, #1
	add	r3, sl
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #3
	bne.n	.L2
	bl	Func_0808b98c
.L2:
	movs	r0, #0
.L9:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x00000145
	.4byte 0x0000091f
	.4byte 0x00000921
	.4byte 0x00000920
	.4byte 0x00000242
	.4byte 0x000003e7
	.4byte 0x0000091e
	.4byte 0x10000005
	.4byte 0x50000005
	.4byte 0x00000141
	.4byte 0x00000cc6
