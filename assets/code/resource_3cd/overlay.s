.syntax unified
	.thumb
	.set sub_020008f2, 0x020008f2
	.set sub_020008f8, 0x020008f8
	.set sub_02000902, 0x02000902
	.set sub_0200090e, 0x0200090e
	.set sub_02000910, 0x02000910
	.set sub_02000920, 0x02000920
	.set sub_02000922, 0x02000922
	.set sub_0200092a, 0x0200092a
	.set sub_02000930, 0x02000930
	.set sub_02000932, 0x02000932
	.set sub_0200093e, 0x0200093e
	.set sub_02000942, 0x02000942
	.set sub_02000950, 0x02000950
	.set sub_02000964, 0x02000964
	.set sub_02000978, 0x02000978
	.set sub_0200099c, 0x0200099c
	.set sub_020009b4, 0x020009b4
	.set sub_020009cc, 0x020009cc
	.set sub_020009e6, 0x020009e6
	.set sub_020009f6, 0x020009f6
	.set sub_02000a28, 0x02000a28
	.set sub_02000a32, 0x02000a32
	.set sub_02000a38, 0x02000a38
	.set sub_02000a3e, 0x02000a3e
	.set sub_02000a44, 0x02000a44
	.set sub_02000a52, 0x02000a52
	.set sub_02000a5e, 0x02000a5e
	.set sub_02000a62, 0x02000a62
	.set sub_02000a76, 0x02000a76
	.set sub_02000a88, 0x02000a88
	.set sub_02000ab4, 0x02000ab4
	.set sub_02000aba, 0x02000aba
	.set sub_02000ac8, 0x02000ac8
	.set sub_02000acc, 0x02000acc
	.set sub_02000adc, 0x02000adc
	.set sub_02000ae0, 0x02000ae0
	.set sub_02000af0, 0x02000af0
	.set sub_02000b1c, 0x02000b1c
	.set sub_02000b1e, 0x02000b1e
	.set sub_02000b2a, 0x02000b2a
	.set sub_02000b38, 0x02000b38
	.set sub_02000b7e, 0x02000b7e
	.set sub_02000bb6, 0x02000bb6
	.set sub_02000bd6, 0x02000bd6
	.set sub_02000bec, 0x02000bec
	.set sub_02000bfc, 0x02000bfc
	.set sub_02000c04, 0x02000c04
	.set sub_02000c1a, 0x02000c1a
	.set sub_02000c26, 0x02000c26
	.set sub_02000c32, 0x02000c32
	.set sub_02000c34, 0x02000c34
	.set sub_02000c4a, 0x02000c4a
	.set sub_02000c4c, 0x02000c4c
	.set sub_02000c54, 0x02000c54
	.set sub_02000c60, 0x02000c60
	.set sub_02000c64, 0x02000c64
	.set sub_02000c76, 0x02000c76
	.set sub_02000c7e, 0x02000c7e
	.set sub_02000c8e, 0x02000c8e
	.set sub_02000c9a, 0x02000c9a
	.set sub_02000cc4, 0x02000cc4
	.set sub_02000cde, 0x02000cde
	.set sub_02000cee, 0x02000cee
	.set sub_02000d40, 0x02000d40
	.set sub_02000d46, 0x02000d46
	.set sub_02000d54, 0x02000d54
	.set sub_02000d58, 0x02000d58
	.set sub_02000d5e, 0x02000d5e
	.set sub_02000d6e, 0x02000d6e
	.set sub_02000d72, 0x02000d72
	.set sub_02000d78, 0x02000d78
	.set sub_02000d8c, 0x02000d8c
	.set sub_02000d90, 0x02000d90
	.set sub_02000d98, 0x02000d98
	.set sub_02000dba, 0x02000dba
	.set sub_02000e4c, 0x02000e4c
	.set sub_02000e50, 0x02000e50
	.set sub_02000e7e, 0x02000e7e
	.set sub_02000e94, 0x02000e94
	.set sub_02000ea0, 0x02000ea0
	.set sub_02000eac, 0x02000eac
	.set sub_02000ec4, 0x02000ec4
	.set sub_02000ede, 0x02000ede
	.set sub_02000eea, 0x02000eea
	.set sub_02000ef8, 0x02000ef8
	.set sub_02000f14, 0x02000f14
	.set sub_02000f18, 0x02000f18
	.set sub_02000f6a, 0x02000f6a
	.set sub_02000fa0, 0x02000fa0
	.set sub_02000fac, 0x02000fac
	.set sub_02000fb8, 0x02000fb8
	.set sub_02000fc8, 0x02000fc8
	.set sub_02000fca, 0x02000fca
	.set sub_02000fd2, 0x02000fd2
	.set sub_02000fe4, 0x02000fe4
	.set sub_02000fec, 0x02000fec
	.set sub_0200100c, 0x0200100c
	.set sub_02001020, 0x02001020
	.global Overlay_02000000
Overlay_02000000:
	.4byte 0x47204c00
	.4byte 0x0200871d
	.4byte 0x47204c00
	.4byte 0x02008031
	.4byte 0x47204c00
	.4byte 0x0200803d
	.4byte 0x47204c00
	.4byte 0x02008045
	.4byte 0x47204c00
	.4byte 0x02008099
	.4byte 0x47204c00
	.4byte 0x02008039
	.4byte 0x47704800
	.4byte 0x02008964
	.4byte 0x47702000
	.4byte 0x47704800
	.4byte 0x02008994
	.4byte 0x47704800
	.2byte 0x8998
	.2byte 0x0200
	push	{lr}
	ldr	r0, [pc, #28]
	bl	sub_020008f2
	movs	r1, #129
	movs	r0, #13
	lsls	r1, r1, #1
	movs	r2, #0
	bl	sub_0200090e
	movs	r0, #13
	movs	r1, #0
	bl	sub_0200090e
	pop	{r0}
	bx	r0
	.2byte 0x23cd
	.2byte 0x0000
	push	{lr}
	ldr	r1, [pc, #28]
	movs	r2, #0
	movs	r0, #13
	bl	sub_0200092a
	ldr	r0, [pc, #20]
	bl	sub_02000920
	movs	r0, #13
	movs	r1, #0
	bl	sub_02000930
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x00000105
	.4byte 0x000023cd
	.4byte 0x47704800
	.2byte 0x8a58
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r6, r1, #0
	bl	sub_020008f8
	ldrb	r1, [r0, #15]
	adds	r0, r5, #0
	adds	r1, r1, r6
	bl	sub_02000932
	adds	r0, r5, #0
	bl	sub_02000910
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	push	{r5, r6, r7, lr}
	sub	sp, #32
	adds	r7, r0, #0
	mov	r0, sp
	bl	sub_02000942
	cmp	r0, #0
	ble.n	.L_020000e4
	mov	r6, sp
	adds	r5, r0, #0
.L_020000d4:
	ldrh	r0, [r6, #0]
	adds	r1, r7, #0
	subs	r5, #1
	adds	r6, #2
	bl	.L_0200017e
	cmp	r5, #0
	bne.n	.L_020000d4
.L_020000e4:
	add	sp, #32
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #272]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #4
	bl	sub_02000950
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #30
	movs	r3, #9
	mov	r8, r0
	movs	r0, #0
	bl	sub_02000902
	ldr	r5, [pc, #240]
	adds	r6, r0, #0
	adds	r1, r6, #0
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_02000922
	adds	r0, r5, #1
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #16
	adds	r5, #2
	bl	sub_02000930
	adds	r0, r5, #0
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #32
	movs	r7, #1
	bl	sub_0200093e
.L_02000140:
	cmp	r7, #0
	beq.n	.L_02000176
	adds	r0, r6, #0
	bl	sub_02000978
	mov	r0, r8
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #48
	bl	sub_02000964
	ldr	r0, [pc, #180]
	adds	r1, r6, #0
	movs	r2, #48
	movs	r3, #48
	bl	sub_02000978
	mov	r3, r8
	ldrb	r0, [r3, #15]
	movs	r3, #48
	str	r3, [sp, #0]
	movs	r1, #0
	adds	r2, r6, #0
	movs	r3, #72
	movs	r7, #0
	bl	sub_0200099c
.L_02000176:
	ldr	r5, [pc, #152]
	ldr	r3, [r5, #0]
	movs	r2, #8
	ands	r3, r2
.L_0200017e:
	cmp	r3, #0
	bne.n	.L_0200018c
	ldr	r3, [r5, #0]
	movs	r2, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200019a
.L_0200018c:
	movs	r0, #5
	bl	.L_02000250
	movs	r0, #93
	bl	sub_02000a5e
	movs	r7, #1
.L_0200019a:
	ldr	r3, [r5, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020001b2
	movs	r0, #1
	bl	.L_02000268
	movs	r0, #91
	bl	sub_02000a76
	movs	r7, #1
.L_020001b2:
	ldr	r3, [r5, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020001fa
	movs	r0, #113
	bl	sub_02000a88
	adds	r0, r6, #0
	bl	sub_020009f6
	movs	r0, #1
	bl	sub_020009b4
	movs	r1, #1
	adds	r0, r6, #0
	bl	sub_020009cc
	movs	r0, #0
	bl	sub_02000a32
	movs	r0, #1
	bl	sub_02000a38
	movs	r0, #3
	bl	sub_02000a3e
	movs	r0, #2
	bl	sub_02000a44
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
.L_020001fa:
	movs	r0, #1
	bl	sub_020009e6
	b.n	.L_02000140
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000c20
	.4byte 0x020088d0
	.2byte 0x1c94
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r2, #0
	movs	r0, #112
	sub	sp, #4
	mov	r8, r2
	bl	sub_02000af0
	movs	r5, #2
	movs	r1, #0
	movs	r2, #30
	movs	r3, #7
	movs	r0, #0
	str	r5, [sp, #0]
	bl	sub_02000a28
	movs	r1, #8
	adds	r7, r0, #0
	movs	r2, #28
	movs	r3, #10
	movs	r0, #0
	str	r5, [sp, #0]
	bl	sub_02000a38
	movs	r6, #1
	mov	r9, r0
	mov	sl, r6
.L_02000250:
	ldr	r3, [pc, #540]
	ldr	r0, [pc, #544]
	ldr	r1, [pc, #544]
	ldr	r2, [pc, #548]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r1, #28
	ldr	r0, [pc, #544]
	ldr	r2, [pc, #544]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #1
.L_02000268:
	bl	sub_02000a52
.L_0200026c:
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L_02000306
	movs	r3, #135
	lsls	r3, r3, #1
	movs	r1, #135
	movs	r2, #0
	adds	r0, r6, r3
	lsls	r1, r1, #1
	mov	sl, r2
	bl	sub_02000a62
	adds	r6, r0, #0
	adds	r0, r7, #0
	bl	sub_02000aba
	adds	r0, r7, #0
	bl	sub_02000ac8
	ldr	r0, [pc, #500]
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_02000ab4
	mov	r2, sl
	str	r2, [sp, #0]
	adds	r0, r6, #0
	movs	r1, #0
	adds	r2, r7, #0
	movs	r3, #80
	bl	sub_02000acc
	bl	sub_02000b38
	cmp	r0, #0
	beq.n	.L_020002fa
	ldr	r5, [pc, #468]
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #32
	ands	r5, r6
	ldr	r0, [pc, #460]
	bl	sub_02000adc
	adds	r0, r5, #0
	bl	sub_02000b2a
	ldr	r0, [pc, #452]
	adds	r1, r7, #0
	adds	r0, r5, r0
	movs	r2, #120
	movs	r3, #0
	bl	sub_02000ae0
	ldr	r3, [pc, #444]
	adds	r5, r5, r3
	adds	r1, r7, #0
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #16
	bl	sub_02000af0
	mov	r0, r9
	bl	sub_02000b1e
	mov	r0, r9
	adds	r1, r6, #0
	bl	sub_02000bb6
	b.n	.L_02000306
.L_020002fa:
	ldr	r0, [pc, #416]
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #32
	bl	sub_02000b1c
.L_02000306:
	ldr	r5, [pc, #408]
	ldr	r3, [r5, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000326
	adds	r0, r6, #0
	bl	sub_02000b7e
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_02000330
	movs	r0, #175
	bl	sub_02000bec
.L_02000326:
	ldr	r3, [r5, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000338
.L_02000330:
	movs	r0, #113
	bl	sub_02000bfc
	b.n	.L_02000442
.L_02000338:
	ldr	r5, [pc, #360]
	ldr	r3, [r5, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000354
	movs	r2, #255
	movs	r3, #1
	movs	r0, #111
	mov	r8, r2
	subs	r6, #1
	mov	sl, r3
	bl	sub_02000c1a
.L_02000354:
	ldr	r3, [r5, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200036c
	movs	r2, #1
	movs	r0, #111
	mov	r8, r2
	adds	r6, #1
	mov	sl, r2
	bl	sub_02000c32
.L_0200036c:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000384
	movs	r3, #1
	movs	r0, #111
	mov	r8, r3
	adds	r6, #10
	mov	sl, r3
	bl	sub_02000c4a
.L_02000384:
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200039e
	movs	r2, #255
	movs	r3, #1
	movs	r0, #111
	mov	r8, r2
	subs	r6, #10
	mov	sl, r3
	bl	sub_02000c64
.L_0200039e:
	ldr	r3, [r5, #0]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020003b8
	movs	r2, #1
	movs	r0, #111
	mov	r8, r2
	adds	r6, #30
	mov	sl, r2
	bl	sub_02000c7e
.L_020003b8:
	ldr	r3, [r5, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020003d4
	movs	r3, #255
	movs	r2, #1
	movs	r0, #111
	mov	r8, r3
	subs	r6, #30
	mov	sl, r2
	bl	sub_02000c9a
.L_020003d4:
	mov	r3, r8
	lsls	r5, r3, #24
	movs	r2, #1
	asrs	r3, r5, #24
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_02000408
	movs	r3, #135
	lsls	r3, r3, #1
	movs	r1, #135
	adds	r0, r6, r3
	b.n	.L_020003f2
.L_020003ec:
	ldr	r2, [pc, #184]
	movs	r1, #135
	adds	r0, r6, r2
.L_020003f2:
	lsls	r1, r1, #1
	bl	sub_02000bd6
	adds	r6, r0, #0
	ldr	r0, [pc, #144]
	ands	r0, r6
	bl	sub_02000c60
	ldrh	r3, [r0, #6]
	cmp	r3, #0
	beq.n	.L_020003ec
.L_02000408:
	movs	r3, #128
	lsls	r3, r3, #17
	cmp	r5, r3
	bne.n	.L_02000436
	movs	r2, #135
	lsls	r2, r2, #1
	movs	r1, #135
	adds	r0, r6, r2
	b.n	.L_02000420
.L_0200041a:
	ldr	r3, [pc, #144]
	movs	r1, #135
	adds	r0, r6, r3
.L_02000420:
	lsls	r1, r1, #1
	bl	sub_02000c04
	adds	r6, r0, #0
	ldr	r0, [pc, #96]
	ands	r0, r6
	bl	sub_02000c8e
	ldrh	r3, [r0, #6]
	cmp	r3, #0
	beq.n	.L_0200041a
.L_02000436:
	movs	r2, #0
	movs	r0, #1
	mov	r8, r2
	bl	sub_02000c26
	b.n	.L_0200026c
.L_02000442:
	adds	r0, r7, #0
	bl	sub_02000c76
	movs	r0, #1
	bl	sub_02000c34
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_02000c4c
	mov	r0, r9
	movs	r1, #1
	bl	sub_02000c54
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x040000d4
	.4byte 0x05000200
	.4byte 0x050001c0
	.4byte 0x80000010
	.4byte 0x050001e8
	.4byte 0x80000001
	.4byte 0x020088d4
	.4byte 0x000001ff
	.4byte 0x020088e0
	.4byte 0x00000182
	.4byte 0x00000075
	.4byte 0x020088f8
	.4byte 0x03001c94
	.4byte 0x03001b04
	.4byte 0x0000010d
	.2byte 0x010f
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r2, #0
	movs	r0, #112
	sub	sp, #4
	mov	r8, r2
	bl	sub_02000d8c
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #30
	movs	r3, #12
	movs	r0, #0
	bl	sub_02000cc4
	movs	r6, #1
	adds	r7, r0, #0
	mov	sl, r6
	ldr	r3, [pc, #476]
	ldr	r0, [pc, #480]
	ldr	r1, [pc, #480]
	ldr	r2, [pc, #484]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r1, #28
	ldr	r0, [pc, #480]
	ldr	r2, [pc, #480]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #1
	bl	sub_02000cde
.L_020004f8:
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L_02000574
	movs	r3, #135
	lsls	r3, r3, #1
	movs	r1, #135
	movs	r2, #0
	adds	r0, r6, r3
	lsls	r1, r1, #1
	mov	sl, r2
	bl	sub_02000cee
	adds	r6, r0, #0
	adds	r0, r7, #0
	bl	sub_02000d46
	adds	r0, r7, #0
	bl	sub_02000d54
	ldr	r0, [pc, #436]
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_02000d40
	mov	r2, sl
	str	r2, [sp, #0]
	adds	r0, r6, #0
	movs	r1, #0
	adds	r2, r7, #0
	movs	r3, #80
	bl	sub_02000d58
	ldr	r5, [pc, #412]
	ldr	r0, [pc, #412]
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #72
	bl	sub_02000d5e
	ldr	r0, [pc, #404]
	ands	r5, r6
	adds	r0, r5, r0
	adds	r1, r7, #0
	movs	r2, #120
	movs	r3, #0
	bl	sub_02000d5e
	ldr	r3, [pc, #392]
	adds	r5, r5, r3
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #24
	bl	sub_02000d6e
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #48
	bl	sub_02000d72
.L_02000574:
	ldr	r3, [pc, #368]
	ldr	r3, [r3, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020005b2
	movs	r0, #113
	bl	sub_02000e4c
	adds	r0, r7, #0
	bl	sub_02000dba
	movs	r0, #1
	bl	sub_02000d78
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_02000d90
	mov	r0, r9
	movs	r1, #1
	bl	sub_02000d98
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
.L_020005b2:
	ldr	r5, [pc, #312]
	ldr	r3, [r5, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020005ce
	movs	r3, #255
	movs	r2, #1
	movs	r0, #111
	mov	r8, r3
	subs	r6, #1
	mov	sl, r2
	bl	sub_02000e94
.L_020005ce:
	ldr	r3, [r5, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020005e6
	movs	r3, #1
	movs	r0, #111
	mov	r8, r3
	adds	r6, #1
	mov	sl, r3
	bl	sub_02000eac
.L_020005e6:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020005fe
	movs	r2, #1
	movs	r0, #111
	mov	r8, r2
	adds	r6, #10
	mov	sl, r2
	bl	sub_02000ec4
.L_020005fe:
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000618
	movs	r3, #255
	movs	r2, #1
	movs	r0, #111
	mov	r8, r3
	subs	r6, #10
	mov	sl, r2
	bl	sub_02000ede
.L_02000618:
	ldr	r3, [r5, #0]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000632
	movs	r3, #1
	movs	r0, #111
	mov	r8, r3
	adds	r6, #30
	mov	sl, r3
	bl	sub_02000ef8
.L_02000632:
	ldr	r3, [r5, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200064e
	movs	r2, #255
	movs	r3, #1
	movs	r0, #111
	mov	r8, r2
	subs	r6, #30
	mov	sl, r3
	bl	sub_02000f14
.L_0200064e:
	mov	r2, r8
	lsls	r5, r2, #24
	movs	r2, #1
	asrs	r3, r5, #24
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_02000682
	movs	r3, #135
	lsls	r3, r3, #1
	movs	r1, #135
	adds	r0, r6, r3
	b.n	.L_0200066c
.L_02000666:
	ldr	r2, [pc, #136]
	movs	r1, #135
	adds	r0, r6, r2
.L_0200066c:
	lsls	r1, r1, #1
	bl	sub_02000e50
	adds	r6, r0, #0
	ldr	r0, [pc, #96]
	ands	r0, r6
	bl	sub_02000eea
	ldrb	r3, [r0, #4]
	cmp	r3, #0
	beq.n	.L_02000666
.L_02000682:
	movs	r3, #128
	lsls	r3, r3, #17
	cmp	r5, r3
	bne.n	.L_020006b0
	movs	r2, #135
	lsls	r2, r2, #1
	movs	r1, #135
	adds	r0, r6, r2
	b.n	.L_0200069a
.L_02000694:
	ldr	r3, [pc, #92]
	movs	r1, #135
	adds	r0, r6, r3
.L_0200069a:
	lsls	r1, r1, #1
	bl	sub_02000e7e
	adds	r6, r0, #0
	ldr	r0, [pc, #52]
	ands	r0, r6
	bl	sub_02000f18
	ldrb	r3, [r0, #4]
	cmp	r3, #0
	beq.n	.L_02000694
.L_020006b0:
	movs	r2, #0
	movs	r0, #1
	mov	r8, r2
	bl	sub_02000ea0
	b.n	.L_020004f8
	.4byte 0x040000d4
	.4byte 0x05000200
	.4byte 0x050001c0
	.4byte 0x80000010
	.4byte 0x050001e8
	.4byte 0x80000001
	.4byte 0x0200890c
	.4byte 0x00003fff
	.4byte 0x02008914
	.4byte 0x00000333
	.4byte 0x0000053a
	.4byte 0x03001c94
	.4byte 0x03001b04
	.4byte 0x0000010d
	.2byte 0x010f
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #28]
	ldr	r1, [r3, #0]
	movs	r3, #224
	lsls	r3, r3, #1
	adds	r2, r1, r3
	adds	r3, #65
	str	r3, [r2, #0]
	subs	r3, #57
	adds	r2, r1, r3
	movs	r3, #24
	str	r3, [r2, #0]
	bl	sub_02000fca
	pop	{r0}
	bx	r0
	.2byte 0x1ebc
	.2byte 0x0300
	push	{r5, lr}
	ldr	r3, [pc, #64]
	ldr	r1, [r3, #0]
	movs	r3, #224
	lsls	r3, r3, #1
	adds	r2, r1, r3
	adds	r3, #68
	str	r3, [r2, #0]
	subs	r3, #60
	adds	r2, r1, r3
	movs	r3, #24
	str	r3, [r2, #0]
	movs	r0, #11
	bl	sub_02000fc8
	ldr	r5, [pc, #40]
	str	r5, [r0, #28]
	movs	r0, #11
	bl	sub_02000fd2
	movs	r1, #5
	str	r5, [r0, #24]
	movs	r0, #13
	bl	sub_02000fe4
	movs	r0, #14
	movs	r1, #2
	bl	sub_02000fec
	movs	r0, #0
	pop	{r5}
	pop	{r1}
	bx	r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.2byte 0x9999
	.2byte 0x0001
	push	{r5, lr}
	sub	sp, #36
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #13
	movs	r2, #30
	movs	r3, #6
	movs	r0, #0
	bl	sub_02000f6a
	adds	r5, r0, #0
	adds	r1, r5, #0
	ldr	r0, [pc, #40]
	movs	r2, #0
	movs	r3, #0
	bl	sub_02000fa0
	adds	r1, r5, #0
	ldr	r0, [pc, #32]
	movs	r2, #0
	movs	r3, #8
	bl	sub_02000fac
	ldr	r0, [pc, #28]
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #16
	bl	sub_02000fb8
	add	sp, #36
	pop	{r5}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x02008920
	.4byte 0x0200893c
	.4byte 0x02008958
	.4byte 0x681b4b02
	.4byte 0x33352201
	.4byte 0x4770701a
	.2byte 0x1f30
	.2byte 0x0300
	push	{lr}
	bl	sub_0200100c
	pop	{r1}
	bx	r1
	.2byte 0x0000
	push	{lr}
	bl	sub_02001020
	pop	{r1}
	bx	r1
	.2byte 0x0000
	.4byte 0x47204c00
	.4byte 0x030003ac
	.4byte 0x47204c00
	.4byte 0x080000c1
	.4byte 0x47204c00
	.4byte 0x08015011
	.4byte 0x47204c00
	.4byte 0x08015019
	.4byte 0x47204c00
	.4byte 0x08015079
	.4byte 0x47204c00
	.4byte 0x08015081
	.4byte 0x47204c00
	.4byte 0x08015091
	.4byte 0x47204c00
	.4byte 0x08015099
	.4byte 0x47204c00
	.4byte 0x080150a9
	.4byte 0x47204c00
	.4byte 0x080150b1
	.4byte 0x47204c00
	.4byte 0x08015271
	.4byte 0x47204c00
	.4byte 0x08015279
	.4byte 0x47204c00
	.4byte 0x08015341
	.4byte 0x47204c00
	.4byte 0x08015349
	.4byte 0x47204c00
	.4byte 0x08077009
	.4byte 0x47204c00
	.4byte 0x08077011
	.4byte 0x47204c00
	.4byte 0x08077019
	.4byte 0x47204c00
	.4byte 0x08077031
	.4byte 0x47204c00
	.4byte 0x08077081
	.4byte 0x47204c00
	.4byte 0x08077159
	.4byte 0x47204c00
	.4byte 0x080771f1
	.4byte 0x47204c00
	.4byte 0x08077251
	.4byte 0x47204c00
	.4byte 0x0808a081
	.4byte 0x47204c00
	.4byte 0x0808a101
	.4byte 0x47204c00
	.4byte 0x0808a171
	.4byte 0x47204c00
	.4byte 0x0808a181
	.4byte 0x47204c00
	.4byte 0x0808a1e9
	.4byte 0x47204c00
	.4byte 0x0808a249
	.4byte 0x47204c00
	.4byte 0x080a1049
	.4byte 0x47204c00
	.4byte 0x080f9011
	.4byte 0x0000764c
	.4byte 0x6d657449
	.4byte 0x3a6f4e20
	.4byte 0x00000000
	.4byte 0x65473a41
	.4byte 0x74492074
	.4byte 0x20206d65
	.4byte 0x65523a42
	.4byte 0x6e727574
	.4byte 0x00000000
	.4byte 0x4d455449
	.4byte 0x4c554620
	.4byte 0x2e2e2e4c
	.4byte 0x2e2e2e2e
	.4byte 0x0000002e
	.4byte 0x20797350
	.4byte 0x003a6f4e
	.4byte 0x65523a42
	.4byte 0x6e727574
	.4byte 0x00000000
	.4byte 0x20422041
	.4byte 0x20442043
	.4byte 0x20462045
	.4byte 0x20482047
	.4byte 0x204b204a
	.4byte 0x204d204c
	.4byte 0x004f204e
	.4byte 0x20512050
	.4byte 0x20532052
	.4byte 0x20552054
	.4byte 0x20572056
	.4byte 0x20592058
	.4byte 0x2061205a
	.4byte 0x00632062
	.4byte 0x203f2021
	.4byte 0x20242023
	.4byte 0x00000025
	.4byte 0xffff0000
	.4byte 0x00000338
	.4byte 0xc0000360
	.4byte 0x01180000
	.4byte 0x039001f8
	.4byte 0x00000380
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000001ff
	.4byte 0xffff01f4
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x03600000
	.4byte 0x00002000
	.4byte 0xffff0020
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x01e00000
	.4byte 0x00002000
	.4byte 0xffff0021
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00002000
	.4byte 0xffff0088
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x03200000
	.4byte 0x00000000
	.4byte 0xffff008f
	.4byte 0x00000001
	.4byte 0x03500000
	.4byte 0x00000000
	.4byte 0x03600000
	.4byte 0x00015000
	.4byte 0xffff0015
	.4byte 0x00000001
	.4byte 0x03500000
	.4byte 0x00000000
	.4byte 0x02b00000
	.4byte 0x00025000
	.4byte 0xffff0015
	.4byte 0x00000001
	.4byte 0x03300000
	.4byte 0x00000000
	.4byte 0x02b00000
	.4byte 0x00025000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008215
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x020087c9
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x020087d5
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x020080ed
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020084b1
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0200804d
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02008071
	.4byte 0x00008e15
	.4byte 0xffff000a
	.4byte 0x02008769
	.4byte 0x10008e15
	.4byte 0xffff000a
	.4byte 0x020087b9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
