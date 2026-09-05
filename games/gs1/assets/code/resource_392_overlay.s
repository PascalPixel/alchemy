.syntax unified
	.thumb
	.set sub_02000342, 0x02000342
	.set sub_02000c02, 0x02000c02
	.set sub_02000e7a, 0x02000e7a
	.set sub_02000ef8, 0x02000ef8
	.set sub_02000f08, 0x02000f08
	.set sub_02000f54, 0x02000f54
	.set sub_02000f84, 0x02000f84
	.set sub_02000f8c, 0x02000f8c
	.set sub_02000faa, 0x02000faa
	.set sub_02000fba, 0x02000fba
	.set sub_02000fc8, 0x02000fc8
	.set sub_02000ff0, 0x02000ff0
	.set sub_02001026, 0x02001026
	.set sub_0200104a, 0x0200104a
	.set sub_0200106c, 0x0200106c
	.set sub_0200110e, 0x0200110e
	.set sub_0200119e, 0x0200119e
	.set sub_02001348, 0x02001348
	.set sub_0200134c, 0x0200134c
	.set sub_02001352, 0x02001352
	.set sub_02001358, 0x02001358
	.set sub_02001460, 0x02001460
	.set sub_0200146c, 0x0200146c
	.set sub_020014ea, 0x020014ea
	.set sub_020014ec, 0x020014ec
	.set sub_020014f6, 0x020014f6
	.set sub_02001504, 0x02001504
	.set sub_02001510, 0x02001510
	.set sub_02001524, 0x02001524
	.set sub_0200152c, 0x0200152c
	.set sub_0200153a, 0x0200153a
	.set sub_02001544, 0x02001544
	.set sub_02001564, 0x02001564
	.set sub_0200157c, 0x0200157c
	.set sub_0200158e, 0x0200158e
	.set sub_02001590, 0x02001590
	.set sub_020015a6, 0x020015a6
	.set sub_020015b4, 0x020015b4
	.set sub_020015bc, 0x020015bc
	.set sub_020015ca, 0x020015ca
	.set sub_020015d4, 0x020015d4
	.set sub_020015ec, 0x020015ec
	.set sub_020015fa, 0x020015fa
	.set sub_02001614, 0x02001614
	.set sub_0200161a, 0x0200161a
	.set sub_0200162c, 0x0200162c
	.set sub_02001632, 0x02001632
	.set sub_0200164e, 0x0200164e
	.set sub_02001656, 0x02001656
	.set sub_020016b6, 0x020016b6
	.set sub_020016d2, 0x020016d2
	.set sub_02001706, 0x02001706
	.set sub_02001708, 0x02001708
	.set sub_0200173a, 0x0200173a
	.set sub_0200177c, 0x0200177c
	.set sub_020017fe, 0x020017fe
	.set sub_0200181a, 0x0200181a
	.set sub_02001846, 0x02001846
	.set sub_0200184c, 0x0200184c
	.set sub_02001878, 0x02001878
	.set sub_0200187c, 0x0200187c
	.set sub_02001880, 0x02001880
	.set sub_0200188e, 0x0200188e
	.set sub_020018a2, 0x020018a2
	.set sub_020018bc, 0x020018bc
	.set sub_020018d2, 0x020018d2
	.set sub_020018e0, 0x020018e0
	.set sub_020018ee, 0x020018ee
	.set sub_02001978, 0x02001978
	.set sub_020019fa, 0x020019fa
	.set sub_02001a10, 0x02001a10
	.set sub_02001a28, 0x02001a28
	.set sub_02001a2a, 0x02001a2a
	.set sub_02001a2e, 0x02001a2e
	.set sub_02001a62, 0x02001a62
	.set sub_02001a92, 0x02001a92
	.set sub_02001ae6, 0x02001ae6
	.set sub_02001aec, 0x02001aec
	.set sub_02001af4, 0x02001af4
	.set sub_02001b0c, 0x02001b0c
	.set sub_02001b12, 0x02001b12
	.set sub_02001b14, 0x02001b14
	.set sub_02001b3c, 0x02001b3c
	.global Overlay_02000000
Overlay_02000000:
	.4byte 0x47204c00
	.4byte 0x02008a69
	.4byte 0x47204c00
	.4byte 0x020089dd
	.4byte 0x47204c00
	.4byte 0x020089e9
	.4byte 0x47204c00
	.4byte 0x020089f1
	.4byte 0x47204c00
	.4byte 0x02008a61
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
	bl	sub_0200119e
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
	.4byte 0x02008f08
	.4byte 0x02008f20
	.2byte 0x8ec8
	.2byte 0x0200
AlchemyC_02000474:
	.space 0x194
AlchemyC_02000608:
	.space 0x2b8
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #260]
	ldr	r3, [r3, #0]
	sub	sp, #32
	mov	sl, r3
	bl	sub_02001706
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
	bl	sub_0200177c
	mov	r3, r8
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	movs	r5, #255
	str	r3, [sp, #0]
	movs	r0, #0
	adds	r3, r6, #0
	str	r5, [sp, #4]
	bl	.L_02000bf0
	mov	r3, r8
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	movs	r0, #2
	str	r3, [sp, #0]
	adds	r3, r6, #0
	str	r5, [sp, #4]
	bl	sub_02000c02
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
	.4byte 0x02008f08
	.4byte 0x02008f20
AlchemyC_020009dc:
	.space 0x8
AlchemyC_020009e4:
	.space 0x4
AlchemyC_020009e8:
	.space 0x8
AlchemyC_020009f0:
	.space 0x8
AlchemyC_020009f8:
	.space 0x34
AlchemyC_02000a2c:
	.space 0x34
AlchemyC_02000a60:
	.space 0x8
AlchemyC_02000a68:
	.space 0x6
.L_02000a6e:
	.space 0x12
.L_02000a80:
	.space 0x3c
AlchemyC_02000abc:
	.space 0x10
.L_02000acc:
	.space 0x58
AlchemyC_02000b24:
	.space 0x68
	.4byte 0x4b044a03
	.4byte 0x49046810
	.4byte 0xc3074a04
	.4byte 0x47703b0c
	.4byte 0x03001ed0
	.4byte 0x040000d4
	.4byte 0x020090b0
	.4byte 0x840000e0
	.4byte 0x4b044a03
	.4byte 0x49046810
	.4byte 0xc3074a04
	.4byte 0x47703b0c
	.4byte 0x03001ed0
	.4byte 0x040000d4
	.4byte 0x020097b0
	.2byte 0x00e0
	.2byte 0x8400
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
.L_02000bf0:
	lsls	r0, r0, #9
	movs	r1, #0
	bl	sub_02001a62
	pop	{r0}
	bx	r0
	.4byte 0x03001ed0
	.4byte 0x040000d4
	.4byte 0x84000070
	.2byte 0x0200
	.2byte 0x0500
	push	{lr}
	ldr	r3, [pc, #40]
	ldr	r1, [r3, #0]
	cmp	r0, #0
	beq.n	.L_02000c1c
	ldr	r3, [pc, #36]
	ldr	r0, [pc, #36]
	b.n	.L_02000c20
.L_02000c1c:
	ldr	r3, [pc, #28]
	ldr	r0, [pc, #36]
.L_02000c20:
	ldr	r2, [pc, #36]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl	sub_02001a92
	bl	sub_020017fe
	pop	{r0}
	bx	r0
	.4byte 0x03001ed0
	.4byte 0x040000d4
	.4byte 0x020097b0
	.4byte 0x020090b0
	.2byte 0x00e0
	.2byte 0x8400
AlchemyC_02000c4c:
	.space 0x66
	.2byte 0x0000
AlchemyC_02000cb4:
	.space 0xd8
	.4byte 0x47204c00
	.4byte 0x03000380
	.4byte 0x47204c00
	.4byte 0x080000c1
	.4byte 0x47204c00
	.4byte 0x080000f9
	.4byte 0x47204c00
	.4byte 0x08000119
	.4byte 0x47204c00
	.4byte 0x08000121
	.4byte 0x47204c00
	.4byte 0x08000141
	.4byte 0x47204c00
	.4byte 0x08000151
	.4byte 0x47204c00
	.4byte 0x080001c9
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
	.4byte 0x08015251
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
	.4byte 0x0808a061
	.4byte 0x47204c00
	.4byte 0x0808a081
	.4byte 0x47204c00
	.4byte 0x0808a091
	.4byte 0x47204c00
	.4byte 0x0808a0e1
	.4byte 0x47204c00
	.4byte 0x0808a0e9
	.4byte 0x47204c00
	.4byte 0x0808a0f1
	.4byte 0x47204c00
	.4byte 0x0808a101
	.4byte 0x47204c00
	.4byte 0x0808a331
	.4byte 0x47204c00
	.4byte 0x0808a339
	.4byte 0x47204c00
	.4byte 0x0808a399
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
	.4byte 0x000001e0
	.4byte 0x80000178
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000138
	.4byte 0x40000028
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000029
	.4byte 0x0010102a
	.4byte 0x00202028
	.4byte 0x000001ff
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01700000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00024000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00024000
	.4byte 0x0fd00016
	.4byte 0x00000007
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01180000
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
	.4byte 0x00009415
	.4byte 0x0fd3000b
	.4byte 0x02008a2d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
