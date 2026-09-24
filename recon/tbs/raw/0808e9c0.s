.syntax unified
	.thumb
	.set sub_080072e4, 0x080072e4
	.set sub_08009080, 0x08009080
	.set sub_080090c8, 0x080090c8
	.set sub_080090d0, 0x080090d0
	.set sub_08009140, 0x08009140
	.set sub_080091e0, 0x080091e0
	.set sub_080770c0, 0x080770c0
	.set sub_0808e9a8, 0x0808e9a8
	.global Battle_PlaceMapMarkers
	.global Func_0808e9c0
	.thumb_func
Battle_PlaceMapMarkers:
Func_0808e9c0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #548]
	sub	sp, #12
	movs	r1, #0
	ldr	r2, [r3, #0]
	ldr	r3, [r3, #76]
	str	r1, [sp, #0]
	ldr	r2, [r2, #16]
	mov	sl, r2
	movs	r2, #142
	lsls	r2, r2, #1
	adds	r2, r2, r3
	mov	r8, r2
	add	r0, sp, #8
	str	r1, [r0, #0]
	ldr	r3, [pc, #524]
	mov	r1, r8
	ldr	r2, [pc, #524]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_0808e9fc
	b.n	.L_0808ebe2
.L_0808e9fc:
	movs	r4, #1
	add	sl, r4
	ldrb	r7, [r3, #0]
	mov	r1, sl
	ldrb	r5, [r1, #0]
	add	sl, r4
	cmp	r7, #255
	bne.n	.L_0808ea12
	cmp	r5, #255
	bne.n	.L_0808ea12
	b.n	.L_0808ebe2
.L_0808ea12:
	mov	r2, sl
	ldrb	r2, [r2, #0]
	movs	r3, #1
	add	sl, r3
	adds	r3, r2, #0
	subs	r3, #100
	str	r2, [sp, #4]
	cmp	r3, #139
	bls.n	.L_0808ea26
	b.n	.L_0808ebc8
.L_0808ea26:
	ldr	r3, [pc, #472]
	ldr	r0, [r3, #36]
	bl	sub_080072e4
	adds	r6, r0, #0
	movs	r4, #1
	ldr	r2, [r6, #0]
	negs	r4, r4
	cmp	r2, r4
	bne.n	.L_0808ea3c
	b.n	.L_0808ebc8
.L_0808ea3c:
	movs	r1, #128
	lsls	r7, r7, #20
	lsls	r1, r1, #12
	mov	fp, r7
	mov	r9, r1
	lsls	r7, r5, #20
.L_0808ea48:
	movs	r4, #4
	ldrsh	r3, [r6, r4]
	ldr	r1, [sp, #4]
	cmp	r3, r1
	beq.n	.L_0808ea54
	b.n	.L_0808ebb8
.L_0808ea54:
	ldr	r3, [pc, #428]
	ands	r3, r2
	cmp	r3, #19
	bne.n	.L_0808eb02
	mov	r4, r9
	mov	r1, fp
	movs	r0, #20
	add	r1, r9
	movs	r2, #0
	adds	r3, r7, r4
	bl	sub_080090c8
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_0808ea74
	b.n	.L_0808ebb8
.L_0808ea74:
	bl	sub_0808e9a8
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_080091e0
	movs	r1, #6
	ldrsh	r0, [r6, r1]
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_0808eaaa
	ldr	r3, [pc, #376]
	ldr	r2, [r6, #8]
	ands	r2, r3
	movs	r3, #160
	lsls	r3, r3, #15
	cmp	r2, r3
	bne.n	.L_0808eaa2
	adds	r0, r5, #0
	bl	sub_080090d0
	b.n	.L_0808ebb8
.L_0808eaa2:
	adds	r0, r5, #0
	movs	r1, #2
	bl	sub_08009080
.L_0808eaaa:
	adds	r0, r5, #0
	bl	sub_08009140
	ldr	r2, [r5, #8]
	cmp	r2, #0
	bge.n	.L_0808eaba
	ldr	r4, [pc, #340]
	adds	r2, r2, r4
.L_0808eaba:
	adds	r3, r5, #0
	adds	r3, #100
	asrs	r2, r2, #16
	strh	r2, [r3, #0]
	ldr	r3, [r5, #16]
	cmp	r3, #0
	bge.n	.L_0808eacc
	ldr	r1, [pc, #320]
	adds	r3, r3, r1
.L_0808eacc:
	adds	r2, r5, #0
	asrs	r3, r3, #16
	adds	r2, #102
	strh	r3, [r2, #0]
	movs	r3, #1
	subs	r2, #67
	strb	r3, [r2, #0]
	adds	r2, #54
	strb	r3, [r2, #0]
	ldrh	r3, [r6, #4]
	mov	r2, r8
	strb	r3, [r2, #4]
	ldr	r3, [r5, #8]
	str	r5, [r2, #0]
	cmp	r3, #0
	bge.n	.L_0808eaf0
	ldr	r4, [pc, #288]
	adds	r3, r3, r4
.L_0808eaf0:
	ldr	r0, [r5, #16]
	asrs	r3, r3, #20
	mov	r1, r8
	strb	r3, [r1, #6]
	cmp	r0, #0
	bge.n	.L_0808eba2
	ldr	r2, [pc, #272]
	adds	r0, r0, r2
	b.n	.L_0808eba2
.L_0808eb02:
	cmp	r3, #3
	bne.n	.L_0808ebb8
	ldr	r3, [pc, #256]
	ldr	r2, [r6, #8]
	ands	r2, r3
	movs	r3, #192
	lsls	r3, r3, #14
	cmp	r2, r3
	bne.n	.L_0808ebb8
	movs	r4, #6
	ldrsh	r0, [r6, r4]
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_0808ebb8
	mov	r4, r9
	mov	r1, fp
	movs	r0, #28
	add	r1, r9
	movs	r2, #0
	adds	r3, r7, r4
	bl	sub_080090c8
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0808ebb8
	bl	sub_0808e9a8
	movs	r1, #0
	adds	r0, r5, #0
	bl	sub_080091e0
	adds	r0, r5, #0
	bl	sub_08009140
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08009080
	ldr	r2, [r5, #8]
	cmp	r2, #0
	bge.n	.L_0808eb5a
	ldr	r1, [pc, #180]
	adds	r2, r2, r1
.L_0808eb5a:
	adds	r3, r5, #0
	adds	r3, #100
	asrs	r2, r2, #16
	strh	r2, [r3, #0]
	ldr	r3, [r5, #16]
	cmp	r3, #0
	bge.n	.L_0808eb6c
	ldr	r2, [pc, #160]
	adds	r3, r3, r2
.L_0808eb6c:
	adds	r2, r5, #0
	asrs	r3, r3, #16
	adds	r2, #102
	strh	r3, [r2, #0]
	movs	r3, #1
	subs	r2, #13
	strb	r3, [r2, #0]
	subs	r2, #54
	strb	r3, [r2, #0]
	mov	r3, r8
	str	r5, [r3, #0]
	ldrh	r3, [r6, #4]
	mov	r4, r8
	strb	r3, [r4, #4]
	ldr	r3, [r5, #8]
	cmp	r3, #0
	bge.n	.L_0808eb92
	ldr	r1, [pc, #128]
	adds	r3, r3, r1
.L_0808eb92:
	ldr	r0, [r5, #16]
	asrs	r3, r3, #20
	mov	r2, r8
	strb	r3, [r2, #6]
	cmp	r0, #0
	bge.n	.L_0808eba2
	ldr	r3, [pc, #112]
	adds	r0, r0, r3
.L_0808eba2:
	mov	r4, r8
	asrs	r3, r0, #20
	strb	r3, [r4, #7]
	ldr	r2, [sp, #0]
	movs	r1, #8
	adds	r2, #1
	add	r8, r1
	str	r2, [sp, #0]
	cmp	r2, #9
	ble.n	.L_0808ebc8
	b.n	.L_0808ebe2
.L_0808ebb8:
	adds	r6, #12
	ldr	r3, [r6, #0]
	movs	r4, #1
	negs	r4, r4
	adds	r2, r3, #0
	cmp	r3, r4
	beq.n	.L_0808ebc8
	b.n	.L_0808ea48
.L_0808ebc8:
	mov	r1, sl
	movs	r2, #1
	add	sl, r2
	ldrb	r7, [r1, #0]
	mov	r3, sl
	ldrb	r5, [r3, #0]
	add	sl, r2
	cmp	r7, #255
	beq.n	.L_0808ebdc
	b.n	.L_0808ea12
.L_0808ebdc:
	cmp	r5, #255
	beq.n	.L_0808ebe2
	b.n	.L_0808ea12
.L_0808ebe2:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e70
	.4byte 0x040000d4
	.4byte 0x85000014
	.4byte 0x02008000
	.4byte 0x000001ff
	.4byte 0xfff00000
	.4byte 0x0000ffff
	.4byte 0x000fffff
