.syntax unified
	.thumb
	.set sub_02000342, 0x02000342
	.set sub_02000bf0, 0x02000bf0
	.set sub_02000e7a, 0x02000e7a
	.set sub_02000e90, 0x02000e90
	.set sub_02000f40, 0x02000f40
	.set sub_02000f58, 0x02000f58
	.set sub_02000fe4, 0x02000fe4
	.set sub_02000fec, 0x02000fec
	.set sub_0200100a, 0x0200100a
	.set sub_0200101a, 0x0200101a
	.set sub_02001026, 0x02001026
	.set sub_02001028, 0x02001028
	.set sub_02001050, 0x02001050
	.set sub_02001092, 0x02001092
	.set sub_020010b4, 0x020010b4
	.set sub_0200116e, 0x0200116e
	.set sub_020011ee, 0x020011ee
	.set sub_020013a8, 0x020013a8
	.set sub_020014b0, 0x020014b0
	.set sub_020014bc, 0x020014bc
	.set sub_020014ce, 0x020014ce
	.set sub_0200151e, 0x0200151e
	.set sub_02001524, 0x02001524
	.set sub_02001542, 0x02001542
	.set sub_0200154c, 0x0200154c
	.set sub_02001554, 0x02001554
	.set sub_02001556, 0x02001556
	.set sub_02001570, 0x02001570
	.set sub_0200157c, 0x0200157c
	.set sub_0200158a, 0x0200158a
	.set sub_0200159c, 0x0200159c
	.set sub_020015dc, 0x020015dc
	.set sub_020015e0, 0x020015e0
	.set sub_020015e6, 0x020015e6
	.set sub_020015f0, 0x020015f0
	.set sub_020015f6, 0x020015f6
	.set sub_020015fc, 0x020015fc
	.set sub_02001614, 0x02001614
	.set sub_0200161a, 0x0200161a
	.set sub_0200162c, 0x0200162c
	.set sub_0200163c, 0x0200163c
	.set sub_0200165a, 0x0200165a
	.set sub_02001662, 0x02001662
	.set sub_02001674, 0x02001674
	.set sub_0200167a, 0x0200167a
	.set sub_0200168a, 0x0200168a
	.set sub_020016b6, 0x020016b6
	.set sub_02001750, 0x02001750
	.set sub_02001756, 0x02001756
	.set sub_020017dc, 0x020017dc
	.set sub_02001872, 0x02001872
	.set sub_020018b2, 0x020018b2
	.set sub_020018b8, 0x020018b8
	.set sub_020018dc, 0x020018dc
	.set sub_020018de, 0x020018de
	.set sub_020018e0, 0x020018e0
	.set sub_020018ee, 0x020018ee
	.set sub_020018f2, 0x020018f2
	.set sub_02001900, 0x02001900
	.set sub_02001906, 0x02001906
	.set sub_02001908, 0x02001908
	.set sub_02001916, 0x02001916
	.set sub_02001918, 0x02001918
	.set sub_0200191a, 0x0200191a
	.set sub_0200191c, 0x0200191c
	.set sub_0200193e, 0x0200193e
	.set sub_02001948, 0x02001948
	.set sub_02001954, 0x02001954
	.set sub_02001966, 0x02001966
	.set sub_02001974, 0x02001974
	.set sub_02001984, 0x02001984
	.set sub_02001986, 0x02001986
	.set sub_0200199c, 0x0200199c
	.set sub_020019b4, 0x020019b4
	.set sub_020019e8, 0x020019e8
	.set sub_020019ee, 0x020019ee
	.set sub_02001a08, 0x02001a08
	.set sub_02001a38, 0x02001a38
	.set sub_02001a56, 0x02001a56
	.set sub_02001a70, 0x02001a70
	.set sub_02001a72, 0x02001a72
	.set sub_02001a7c, 0x02001a7c
	.set sub_02001aa2, 0x02001aa2
	.set sub_02001aac, 0x02001aac
	.set sub_02001ac6, 0x02001ac6
	.set sub_02001ad6, 0x02001ad6
	.set sub_02001b32, 0x02001b32
	.set sub_02001b40, 0x02001b40
	.set sub_02001b4e, 0x02001b4e
	.set sub_02001b98, 0x02001b98
	.set sub_02001b9e, 0x02001b9e
	.set sub_02001c82, 0x02001c82
	.set sub_02001cb2, 0x02001cb2
	.global Overlay_02000000
Overlay_02000000:
	.4byte 0x47204c00
	.4byte 0x02008bf9
	.4byte 0x47204c00
	.4byte 0x020089dd
	.4byte 0x47204c00
	.4byte 0x020089e9
	.4byte 0x47204c00
	.4byte 0x020089f1
	.4byte 0x47204c00
	.4byte 0x02008bf1
	.4byte 0x47204c00
	.2byte 0x89e5
	.2byte 0x0200
AlchemyC_02000030:
	.space 0x3c
AlchemyC_0200006c:
	.space 0x58
AlchemyC_020000c4:
	.space 0x180
AlchemyC_02000244:
	.space 0x64
AlchemyC_020002a8:
	.space 0xa4
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #24
	str	r0, [sp, #20]
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	ldr	r3, [pc, #256]
	movs	r0, #0
	ldr	r5, [r3, #0]
	bl	sub_020011ee
	ldrh	r3, [r0, #6]
	ldr	r1, [sp, #20]
	lsrs	r3, r3, #12
	movs	r2, #8
	str	r3, [r1, #0]
	adds	r5, #52
	str	r2, [sp, #8]
	mov	r9, r0
	mov	fp, r5
.L_0200037e:
	mov	r3, fp
	ldr	r3, [r3, #0]
	mov	sl, r3
	ldr	r3, [r3, #80]
	ldr	r3, [r3, #40]
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	ldr	r3, [pc, #216]
	movs	r4, #0
	str	r1, [sp, #4]
	ldr	r0, [pc, #216]
	str	r3, [sp, #0]
	mov	r8, r4
.L_02000398:
	ldr	r1, [sp, #0]
	ldmia	r1!, {r3}
	ldr	r2, [sp, #4]
	adds	r4, r1, #0
	str	r4, [sp, #0]
	cmp	r2, r3
	bne.n	.L_02000436
	ldr	r4, [sp, #12]
	mov	r3, r8
	str	r3, [r4, #0]
	ldr	r2, [sp, #20]
	ldr	r3, [r2, #0]
	ldr	r4, [pc, #188]
	lsls	r3, r3, #2
	mov	r1, r9
	ldr	r2, [r4, r3]
	ldr	r1, [r1, #8]
	asrs	r3, r2, #16
	mov	lr, r1
	asrs	r1, r1, #16
	adds	r1, r1, r3
	asrs	r7, r1, #4
	mov	r1, r9
	ldr	r1, [r1, #16]
	lsls	r2, r2, #16
	asrs	r2, r2, #16
	asrs	r3, r1, #16
	adds	r3, r3, r2
	asrs	r5, r3, #4
	mov	r3, sl
	mov	ip, r1
	movs	r2, #10
	ldrsh	r1, [r3, r2]
	ldr	r3, [r0, #0]
	adds	r3, r1, r3
	asrs	r6, r3, #4
	mov	r3, sl
	movs	r4, #18
	ldrsh	r2, [r3, r4]
	ldr	r3, [r0, #4]
	adds	r3, r2, r3
	asrs	r4, r3, #4
	ldr	r3, [r0, #8]
	adds	r1, r1, r3
	ldr	r3, [r0, #12]
	adds	r2, r2, r3
	asrs	r1, r1, #4
	asrs	r2, r2, #4
	cmp	r6, r7
	bgt.n	.L_02000436
	cmp	r7, r1
	bge.n	.L_02000436
	cmp	r4, r5
	bgt.n	.L_02000436
	cmp	r5, r2
	bge.n	.L_02000436
	movs	r3, #1
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02000424
	mov	r2, lr
	asrs	r3, r2, #20
	cmp	r6, r3
	beq.n	.L_02000436
	ldr	r3, [sp, #8]
	ldr	r4, [sp, #16]
	mov	r0, sl
	str	r3, [r4, #0]
	b.n	.L_02000452
.L_02000424:
	mov	r1, ip
	asrs	r3, r1, #20
	cmp	r4, r3
	beq.n	.L_02000436
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #16]
	mov	r0, sl
	str	r2, [r3, #0]
	b.n	.L_02000452
.L_02000436:
	movs	r4, #1
	add	r8, r4
	mov	r1, r8
	adds	r0, #16
	cmp	r1, #5
	bls.n	.L_02000398
	ldr	r3, [sp, #8]
	movs	r2, #4
	adds	r3, #1
	add	fp, r2
	str	r3, [sp, #8]
	cmp	r3, #65
	bls.n	.L_0200037e
	movs	r0, #0
.L_02000452:
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001ebc
	.4byte 0x02008f50
	.4byte 0x02008f68
	.2byte 0x8f10
	.2byte 0x0200
AlchemyC_02000474:
	.space 0x194
	.2byte 0xb084
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #40
	str	r0, [sp, #72]
	str	r1, [sp, #76]
	str	r2, [sp, #80]
	str	r3, [sp, #84]
	ldr	r3, [pc, #640]
	ldr	r3, [r3, #0]
	movs	r0, #0
	str	r3, [sp, #12]
	bl	sub_020014b0
	ldrh	r3, [r0, #6]
	ldr	r0, [sp, #76]
	lsrs	r3, r3, #12
	mov	r8, r3
	bl	sub_020014bc
	ldr	r3, [sp, #72]
	ldr	r4, [pc, #616]
	lsls	r1, r3, #4
	adds	r3, r1, #4
	ldr	r2, [r4, r3]
	adds	r7, r0, #0
	cmp	r2, #0
	bge.n	.L_0200064c
	negs	r2, r2
.L_0200064c:
	adds	r3, r1, #0
	adds	r3, #12
	ldr	r3, [r4, r3]
	cmp	r3, #0
	bge.n	.L_02000658
	negs	r3, r3
.L_02000658:
	adds	r3, r2, r3
	asrs	r3, r3, #4
	str	r3, [sp, #8]
	ldr	r2, [r4, r1]
	cmp	r2, #0
	bge.n	.L_02000666
	negs	r2, r2
.L_02000666:
	adds	r3, r1, #0
	adds	r3, #8
	ldr	r3, [r4, r3]
	cmp	r3, #0
	bge.n	.L_02000672
	negs	r3, r3
.L_02000672:
	adds	r3, r2, r3
	asrs	r3, r3, #4
	mov	r9, r3
	ldr	r5, [pc, #560]
	ldr	r3, [r7, #8]
	movs	r6, #128
	add	r0, sp, #28
	lsls	r6, r6, #8
	str	r6, [r7, #48]
	str	r5, [r7, #52]
	str	r3, [r0, #0]
	ldr	r3, [r7, #16]
	str	r3, [r0, #8]
	ldr	r2, [sp, #72]
	lsls	r2, r2, #4
	ldr	r3, [r4, r2]
	ldr	r1, [r7, #8]
	lsls	r3, r3, #16
	adds	r1, r1, r3
	mov	fp, r0
	add	r0, sp, #16
	str	r1, [r0, #0]
	adds	r2, #4
	ldr	r3, [r4, r2]
	ldr	r2, [r7, #16]
	lsls	r3, r3, #16
	adds	r2, r2, r3
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	str	r1, [r0, #0]
	str	r2, [r0, #8]
	ldr	r3, [sp, #8]
	str	r3, [sp, #0]
	movs	r3, #0
	str	r3, [sp, #4]
	movs	r0, #0
	mov	r3, r9
	bl	.L_02000902
	adds	r2, r5, #0
	movs	r0, #0
	adds	r1, r6, #0
	bl	sub_02001554
	movs	r1, #8
	movs	r0, #0
	bl	sub_0200157c
	movs	r0, #15
	bl	sub_02001542
	mov	r4, fp
	ldr	r2, [sp, #80]
	ldr	r3, [r4, #0]
	subs	r1, r2, r3
	cmp	r1, #0
	bge.n	.L_020006e8
	ldr	r0, [pc, #456]
	adds	r1, r1, r0
.L_020006e8:
	mov	r4, fp
	ldr	r2, [sp, #88]
	ldr	r3, [r4, #8]
	subs	r2, r2, r3
	asrs	r1, r1, #17
	cmp	r2, #0
	bge.n	.L_020006fa
	ldr	r0, [pc, #440]
	adds	r2, r2, r0
.L_020006fa:
	asrs	r2, r2, #17
	movs	r0, #0
	bl	sub_0200159c
	movs	r0, #0
	bl	sub_0200158a
	ldr	r3, [pc, #424]
	str	r3, [r0, #108]
	movs	r0, #4
	bl	sub_0200157c
	mov	r3, r8
	subs	r3, #6
	cmp	r3, #7
	bhi.n	.L_02000724
	adds	r0, r7, #0
	movs	r1, #3
	bl	sub_0200154c
	b.n	.L_0200072c
.L_02000724:
	adds	r0, r7, #0
	movs	r1, #2
	bl	sub_02001556
.L_0200072c:
	movs	r0, #239
	bl	sub_020015fc
	adds	r0, r7, #0
	ldr	r1, [sp, #80]
	ldr	r2, [sp, #84]
	ldr	r3, [sp, #88]
	bl	sub_02001570
	movs	r0, #0
	bl	sub_020015e6
	movs	r0, #0
	movs	r1, #2
	bl	sub_020015f6
	movs	r0, #0
	ldr	r1, [pc, #360]
	ldr	r2, [pc, #344]
	bl	sub_020015e0
	ldr	r2, [pc, #356]
	mov	r1, r8
	lsls	r3, r1, #2
	ldr	r0, [r2, r3]
	asrs	r3, r0, #16
	lsls	r3, r3, #16
	lsls	r0, r0, #16
	asrs	r1, r3, #16
	asrs	r2, r0, #16
	lsrs	r3, r3, #31
	lsrs	r0, r0, #31
	adds	r1, r1, r3
	adds	r2, r2, r0
	asrs	r1, r1, #1
	asrs	r2, r2, #1
	movs	r0, #0
	bl	sub_02001614
	ldr	r3, [sp, #92]
	cmp	r3, #0
	beq.n	.L_02000784
	bl	sub_02001662
.L_02000784:
	movs	r0, #0
	bl	sub_0200162c
	movs	r1, #1
	movs	r0, #0
	bl	sub_0200163c
	movs	r0, #0
	bl	sub_0200161a
	movs	r2, #0
	str	r2, [r0, #108]
	adds	r0, r7, #0
	bl	sub_020015dc
	movs	r0, #144
	lsls	r0, r0, #1
	bl	sub_02001674
	movs	r0, #213
	bl	sub_0200167a
	ldr	r3, [sp, #80]
	str	r3, [r7, #8]
	ldr	r3, [sp, #88]
	str	r3, [r7, #16]
	movs	r3, #0
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_020015f0
	ldr	r2, [sp, #72]
	ldr	r4, [pc, #220]
	lsls	r2, r2, #4
	ldr	r3, [r4, r2]
	ldr	r0, [sp, #80]
	lsls	r3, r3, #16
	adds	r2, #4
	adds	r0, r0, r3
	ldr	r3, [r4, r2]
	ldr	r1, [sp, #88]
	lsls	r3, r3, #16
	adds	r1, r1, r3
.L_020007de:
	ldr	r2, [sp, #12]
	asrs	r0, r0, #20
	asrs	r1, r1, #20
	mov	sl, r4
	movs	r4, #158
	str	r0, [sp, #80]
	str	r1, [sp, #88]
	lsls	r4, r4, #1
	adds	r3, r2, r4
	ldr	r3, [r3, #0]
	mov	r8, r3
	mov	r2, r8
	asrs	r2, r2, #20
	ldr	r4, [sp, #12]
	mov	r8, r2
	movs	r2, #160
	lsls	r2, r2, #1
	adds	r3, r4, r2
	ldr	r6, [r3, #0]
	mov	r4, r8
	asrs	r6, r6, #20
	adds	r3, r4, r0
	adds	r2, r6, r1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	ldr	r3, [sp, #8]
	mov	r2, r9
	bl	sub_0200165a
	ldr	r0, [sp, #8]
	ldr	r1, [sp, #80]
	ldr	r2, [sp, #88]
	str	r0, [sp, #0]
	movs	r5, #255
	mov	r3, r9
	movs	r0, #0
	str	r5, [sp, #4]
	bl	.L_02000a6e
	ldr	r3, [sp, #8]
	ldr	r1, [sp, #80]
	ldr	r2, [sp, #88]
	str	r3, [sp, #0]
	movs	r0, #2
	mov	r3, r9
	str	r5, [sp, #4]
	bl	.L_02000a80
	ldr	r2, [sp, #72]
	mov	r4, sl
	lsls	r2, r2, #4
	ldr	r3, [r4, r2]
	mov	r0, fp
	ldr	r1, [r0, #0]
	lsls	r3, r3, #16
	adds	r2, #4
	adds	r1, r1, r3
	ldr	r3, [r4, r2]
	ldr	r2, [r0, #8]
	lsls	r3, r3, #16
	adds	r2, r2, r3
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	str	r1, [r0, #0]
	str	r2, [r0, #8]
	add	r8, r1
	adds	r6, r6, r2
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	ldr	r3, [sp, #8]
	mov	r0, r8
	adds	r1, r6, #0
	mov	r2, r9
	bl	sub_020016b6
	ldr	r3, [sp, #8]
	mov	r2, fp
	ldr	r1, [r2, #0]
	movs	r4, #0
	ldr	r2, [r2, #8]
	movs	r0, #2
	str	r3, [sp, #0]
	mov	r3, r9
	str	r4, [sp, #4]
	bl	.L_02000acc
	bl	sub_02001750
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r3}
	add	sp, #16
	bx	r3
	.2byte 0x0000
	.4byte 0x03001e70
	.4byte 0x02008f68
	.4byte 0x00001999
	.4byte 0x0001ffff
	.4byte 0x020082a9
	.4byte 0x00004ccc
	.2byte 0x8f10
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #260]
	ldr	r3, [r3, #0]
	sub	sp, #32
	mov	sl, r3
	bl	sub_02001756
	ldr	r3, [r0, #80]
	ldr	r3, [r3, #40]
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r1, [pc, #244]
	movs	r5, #0
	ldr	r3, [r1, r5]
	cmp	r2, r3
	bne.n	.L_020008ea
	add	r7, sp, #8
	b.n	.L_0200090c
.L_020008ea:
	add	r7, sp, #8
	mov	ip, r7
	movs	r6, #7
	adds	r4, r1, #0
.L_020008f2:
	mov	r3, ip
	adds	r5, #1
	str	r6, [r3, #0]
	cmp	r5, #5
	bhi.n	.L_0200090e
	ldr	r3, [r0, #80]
	ldr	r3, [r3, #40]
	adds	r4, #4
.L_02000902:
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [r4, #0]
	cmp	r2, r3
	bne.n	.L_020008f2
.L_0200090c:
	str	r5, [r7, #0]
.L_0200090e:
	ldr	r2, [r7, #0]
	cmp	r2, #6
	bls.n	.L_02000918
	movs	r0, #0
	b.n	.L_020009c2
.L_02000918:
	ldr	r3, [r0, #8]
	str	r3, [r7, #8]
	mov	ip, r3
	ldr	r3, [r0, #12]
	str	r3, [r7, #12]
	ldr	r0, [r0, #16]
	lsls	r1, r2, #4
	str	r0, [r7, #16]
	ldr	r4, [pc, #172]
	adds	r5, r1, #4
	ldr	r2, [r4, r5]
	mov	lr, r0
	cmp	r2, #0
	bge.n	.L_02000936
	negs	r2, r2
.L_02000936:
	adds	r3, r1, #0
	adds	r3, #12
	ldr	r3, [r4, r3]
	cmp	r3, #0
	bge.n	.L_02000942
	negs	r3, r3
.L_02000942:
	adds	r3, r2, r3
	ldr	r0, [r4, r1]
	asrs	r3, r3, #4
	mov	r8, r3
	adds	r6, r0, #0
	cmp	r0, #0
	bge.n	.L_02000952
	negs	r6, r0
.L_02000952:
	adds	r3, r1, #0
	adds	r3, #8
	ldr	r3, [r4, r3]
	cmp	r3, #0
	bge.n	.L_0200095e
	negs	r3, r3
.L_0200095e:
	lsls	r0, r0, #16
	add	r0, ip
	str	r0, [r7, #8]
	ldr	r1, [r4, r5]
	lsls	r1, r1, #16
	add	r1, lr
	asrs	r0, r0, #20
	asrs	r1, r1, #20
	adds	r6, r6, r3
	movs	r3, #158
	str	r0, [r7, #8]
	str	r1, [r7, #16]
	lsls	r3, r3, #1
	add	r3, sl
	ldr	r3, [r3, #0]
	asrs	r5, r3, #20
	movs	r3, #160
	lsls	r3, r3, #1
	add	r3, sl
	ldr	r3, [r3, #0]
	asrs	r3, r3, #20
	adds	r2, r5, r0
	adds	r3, r3, r1
	asrs	r6, r6, #4
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	adds	r2, r6, #0
	mov	r3, r8
	bl	sub_020017dc
	mov	r3, r8
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	movs	r5, #255
	str	r3, [sp, #0]
	movs	r0, #0
	adds	r3, r6, #0
	str	r5, [sp, #4]
	bl	sub_02000bf0
	mov	r3, r8
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	movs	r0, #2
	str	r3, [sp, #0]
	adds	r3, r6, #0
	str	r5, [sp, #4]
	bl	.L_02000c02
	movs	r0, #1
.L_020009c2:
	add	sp, #32
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001e70
	.4byte 0x02008f50
	.4byte 0x02008f68
AlchemyC_020009dc:
	.space 0x8
AlchemyC_020009e4:
	.space 0x4
AlchemyC_020009e8:
	.space 0x8
AlchemyC_020009f0:
	.space 0x8
	push	{r5, lr}
	sub	sp, #32
	bl	sub_02001872
	add	r5, sp, #8
	adds	r0, r5, #0
	bl	sub_02000e7a
	cmp	r0, #0
	beq.n	.L_02000a9c
	mov	r3, sp
	add	r2, sp, #24
	ldmia	r2!, {r0, r1}
	stmia	r3!, {r0, r1}
	ldr	r3, [r5, #12]
	ldr	r0, [r5, #0]
	ldr	r1, [r5, #4]
	ldr	r2, [r5, #8]
	bl	sub_02001026
	ldr	r3, [r5, #4]
	cmp	r3, #10
	bne.n	.L_02000a9c
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #12
	bne.n	.L_02000a9c
	movs	r0, #10
	movs	r1, #3
	bl	sub_020018e0
	movs	r1, #18
	movs	r2, #6
	negs	r1, r1
	movs	r0, #10
	bl	sub_020018dc
	movs	r0, #30
	bl	sub_020018b2
	movs	r0, #240
	bl	sub_02001918
	movs	r1, #8
	movs	r0, #10
	bl	sub_02001900
	movs	r0, #10
	bl	sub_020018de
	movs	r3, #2
	adds	r0, #35
	strb	r3, [r0, #0]
	movs	r2, #16
	movs	r3, #11
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #32
	movs	r1, #20
.L_02000a6e:
	movs	r2, #2
	movs	r3, #4
	bl	sub_020018b8
	movs	r3, #4
	str	r3, [sp, #0]
	movs	r1, #12
	movs	r2, #16
	movs	r3, #1
.L_02000a80:
	movs	r5, #0
	movs	r0, #2
	str	r5, [sp, #4]
	bl	.L_02000ccc
	ldr	r0, [pc, #28]
	bl	sub_020018f2
	movs	r0, #10
	bl	sub_02001918
	movs	r1, #0
	bl	sub_020018ee
.L_02000a9c:
	bl	sub_0200191a
	add	sp, #32
	pop	{r5}
	pop	{r0}
	bx	r0
	.2byte 0x0201
	.2byte 0x0000
AlchemyC_02000aac:
	.space 0x20
.L_02000acc:
	.space 0xd8
AlchemyC_02000ba4:
	.space 0x4c
AlchemyC_02000bf0:
	.space 0x8
	push	{r5, lr}
	ldr	r3, [pc, #132]
	movs	r2, #224
	ldr	r3, [r3, #0]
	lsls	r2, r2, #1
.L_02000c02:
	adds	r3, r3, r2
	adds	r2, #68
	str	r2, [r3, #0]
	movs	r0, #10
	sub	sp, #8
	bl	sub_020014ce
	ldr	r0, [pc, #112]
	bl	sub_02001a70
	cmp	r0, #0
	beq.n	.L_02000c5a
	movs	r0, #10
	bl	sub_02001aa2
	movs	r3, #2
	adds	r0, #35
	strb	r3, [r0, #0]
	movs	r2, #16
	movs	r3, #11
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #32
	movs	r1, #20
	movs	r2, #2
	movs	r3, #4
	bl	sub_02001a7c
	movs	r3, #4
	str	r3, [sp, #0]
	movs	r1, #12
	movs	r2, #16
	movs	r3, #1
	movs	r5, #0
	movs	r0, #2
	str	r5, [sp, #4]
	bl	sub_02000e90
	movs	r0, #10
	bl	sub_02001ad6
	movs	r1, #0
	bl	sub_02001aac
.L_02000c5a:
	movs	r0, #8
	bl	sub_0200151e
	movs	r0, #9
	bl	sub_02001524
	ldr	r0, [pc, #32]
	bl	sub_02001ac6
	cmp	r0, #0
	bne.n	.L_02000c76
	movs	r0, #6
	bl	sub_02001900
.L_02000c76:
	movs	r0, #0
	add	sp, #8
	pop	{r5}
	pop	{r1}
	bx	r1
	.4byte 0x03001ebc
	.4byte 0x00000201
	.2byte 0x0845
	.2byte 0x0000
AlchemyC_02000c8c:
	.space 0x40
.L_02000ccc:
	.space 0x28
AlchemyC_02000cf4:
	.space 0x68
AlchemyC_02000d5c:
	.space 0x20
AlchemyC_02000d7c:
	.space 0x20
	push	{lr}
	ldr	r3, [pc, #44]
	ldr	r4, [r3, #0]
	movs	r0, #160
	ldr	r3, [pc, #40]
	lsls	r0, r0, #19
	adds	r1, r4, #0
	ldr	r2, [pc, #40]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #224
	lsls	r2, r2, #1
	adds	r1, r4, r2
	ldr	r0, [pc, #32]
	ldr	r2, [pc, #24]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl	sub_02001c82
	pop	{r0}
	bx	r0
	.4byte 0x03001ed0
	.4byte 0x040000d4
	.4byte 0x84000070
	.2byte 0x0200
	.2byte 0x0500
AlchemyC_02000ddc:
	.space 0x40
	.4byte 0x47204c00
	.4byte 0x03000380
	.4byte 0x47204c00
	.4byte 0x080000c1
	.4byte 0x47204c00
	.4byte 0x08009081
	.4byte 0x47204c00
	.4byte 0x08009151
	.4byte 0x47204c00
	.4byte 0x08009159
	.4byte 0x47204c00
	.4byte 0x080091c1
	.4byte 0x47204c00
	.4byte 0x080091d9
	.4byte 0x47204c00
	.4byte 0x080091e1
	.4byte 0x47204c00
	.4byte 0x080770c1
	.4byte 0x47204c00
	.4byte 0x080770c9
	.4byte 0x47204c00
	.4byte 0x0808a011
	.4byte 0x47204c00
	.4byte 0x0808a019
	.4byte 0x47204c00
	.4byte 0x0808a021
	.4byte 0x47204c00
	.4byte 0x0808a081
	.4byte 0x47204c00
	.4byte 0x0808a091
	.4byte 0x47204c00
	.4byte 0x0808a0c1
	.4byte 0x47204c00
	.4byte 0x0808a0e1
	.4byte 0x47204c00
	.4byte 0x0808a0e9
	.4byte 0x47204c00
	.4byte 0x0808a101
	.4byte 0x47204c00
	.4byte 0x0808a331
	.4byte 0x47204c00
	.4byte 0x0808a339
	.4byte 0x47204c00
	.4byte 0x0808a5e9
	.4byte 0x47204c00
	.4byte 0x080f9011
	.4byte 0x46c04700
	.4byte 0x46c04708
	.4byte 0x46c04710
	.4byte 0x46c04718
	.4byte 0x46c04720
	.4byte 0x46c04728
	.4byte 0x46c04730
	.4byte 0x46c04738
	.4byte 0x46c04740
	.4byte 0x46c04748
	.4byte 0x46c04750
	.4byte 0x46c04758
	.4byte 0x46c04760
	.4byte 0x46c04768
	.4byte 0x46c04770
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x00100000
	.4byte 0x000000cf
	.4byte 0x000000cd
	.4byte 0x000000e4
	.4byte 0x000000e5
	.4byte 0x0000012a
	.4byte 0x00000129
	.4byte 0xffffffe0
	.4byte 0xfffffff8
	.4byte 0x00000020
	.4byte 0x00000008
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.4byte 0xffffffe0
	.4byte 0xfffffff0
	.4byte 0x00000020
	.4byte 0x00000000
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.4byte 0xffffffe0
	.4byte 0xfffffff8
	.4byte 0x00000020
	.4byte 0x00000008
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.4byte 0xffff0000
	.4byte 0x000000d1
	.4byte 0x40000117
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000140
	.4byte 0xc00001e8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000001e0
	.4byte 0x80000068
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000002a
	.4byte 0x0010102b
	.4byte 0x00202029
	.4byte 0x000001ff
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte 0x020089f9
	.4byte 0x00000602
	.4byte 0xffff000b
	.4byte 0x02008ba5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
