.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08013b30, 0x08013b30
	.set sub_08013e70, 0x08013e70
	.set sub_08013fdc, 0x08013fdc
	.set sub_08014274, 0x08014274
	.set sub_080143e0, 0x080143e0
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014694, 0x08014694
	.set sub_08014d00, 0x08014d00
	.set sub_08014de4, 0x08014de4
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_08016cfc, 0x08016cfc
	.set sub_08016d5c, 0x08016d5c
	.set sub_08020088, 0x08020088
	.set sub_08038008, 0x08038008
	.set sub_08038018, 0x08038018
	.set sub_08038038, 0x08038038
	.set sub_08038048, 0x08038048
	.set sub_08038118, 0x08038118
	.set sub_08038120, 0x08038120
	.set sub_08038128, 0x08038128
	.set sub_08038130, 0x08038130
	.set sub_080381c8, 0x080381c8
	.set sub_080ad000, 0x080ad000
	.set sub_080ad0e8, 0x080ad0e8
	.set sub_080ad0f0, 0x080ad0f0
	.set sub_080ad148, 0x080ad148
	.set sub_0811843c, 0x0811843c
	.set sub_081185c4, 0x081185c4
	.set sub_08118c68, 0x08118c68
	.set sub_08118d6c, 0x08118d6c
	.set sub_08118d70, 0x08118d70
	.set sub_08118e64, 0x08118e64
	.set sub_08118eb0, 0x08118eb0
	.set sub_08118f6c, 0x08118f6c
	.set sub_08119374, 0x08119374
	.set sub_081195a0, 0x081195a0
	.set sub_081195d4, 0x081195d4
	.set sub_081195ec, 0x081195ec
	.set sub_081196fc, 0x081196fc
	.set sub_081197a8, 0x081197a8
	.set sub_0811b37c, 0x0811b37c
	.set sub_0811b9d4, 0x0811b9d4
	.set sub_0811bc98, 0x0811bc98
	.set sub_0811bddc, 0x0811bddc
	.set sub_0811c3bc, 0x0811c3bc
	.set sub_0811c594, 0x0811c594
	.set sub_0811d61c, 0x0811d61c
	.set sub_0811d748, 0x0811d748
	.set sub_0811d7e4, 0x0811d7e4
	.set sub_0811d9cc, 0x0811d9cc
	.set sub_0811ff08, 0x0811ff08
	.set sub_08120060, 0x08120060
	.set sub_08124bfc, 0x08124bfc
	.set sub_08124cc0, 0x08124cc0
	.set sub_08124e20, 0x08124e20
	.set sub_08125d74, 0x08125d74
	.set sub_081263c4, 0x081263c4
	.set sub_081263f0, 0x081263f0
	.set sub_081263fc, 0x081263fc
	.set sub_08126548, 0x08126548
	.set sub_08126804, 0x08126804
	.set sub_08127cd4, 0x08127cd4
	.set sub_081281ec, 0x081281ec
	.set sub_081284c0, 0x081284c0
	.set sub_081c0010, 0x081c0010
	.global Func_081197d0
	.thumb_func
Func_081197d0:
.L_081197d0:
	push	{lr}
	movs	r0, #118
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_081197e2
	ldr	r0, [pc, #8]
	b.n	.L_081197e4
.L_081197e2:
	ldr	r0, [pc, #8]
.L_081197e4:
	pop	{pc}
	movs	r0, r0
	.4byte 0x00003007
	.2byte 0x2fd2
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #44
	str	r0, [sp, #40]
	movs	r1, #76
	movs	r0, #48
	bl	sub_08014d00
	mov	sl, r0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #124
	mov	r8, r0
	mov	r1, r8
	movs	r0, #36
	bl	sub_08014d00
	movs	r5, #249
	lsls	r5, r5, #3
	adds	r1, r5, #0
	mov	r9, r0
	movs	r0, #216
	bl	sub_08014d00
	movs	r1, #32
	movs	r0, #176
	bl	sub_08014d00
	movs	r6, #192
	movs	r1, #160
	str	r0, [sp, #36]
	lsls	r1, r1, #2
	lsls	r6, r6, #18
	movs	r0, #44
	bl	sub_08014d00
	adds	r3, r6, #0
	adds	r3, #216
	adds	r1, r5, #0
	ldr	r0, [r3, #0]
	ldr	r3, [pc, #612]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xf6fa
	.2byte 0xfe36
	.2byte 0x9909
	movs	r3, #128
	lsls	r3, r3, #6
	str	r3, [r1, #0]
	movs	r3, #128
	movs	r2, #1
	lsls	r3, r3, #19
	movs	r7, #0
	movs	r0, #4
	str	r7, [r1, #4]
	str	r2, [r1, #20]
	str	r7, [r1, #24]
	str	r7, [r1, #28]
	strh	r2, [r3, #0]
	adds	r0, #255
	bl	sub_08016cfc
	movs	r0, #106
	adds	r0, #255
	bl	sub_08016cfc
	bl	sub_08014de4
	ldr	r5, [pc, #560]
	movs	r1, #76
	mov	r0, sl
	movs	r2, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4648
	mov	r1, r8
	movs	r2, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2301
	mov	r2, r9
	negs	r3, r3
	str	r3, [r2, #84]
	ldr	r3, [sp, #40]
	movs	r1, #12
	str	r3, [r2, #0]
	movs	r0, #148
	bl	sub_08014d00
	adds	r6, #148
	ldr	r0, [r6, #0]
	movs	r1, #12
	movs	r2, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0xf7ae
	.2byte 0xfe32
	.2byte 0x23ce
	lsls	r3, r3, #3
	add	r3, r9
	movs	r1, #224
	strh	r0, [r3, #0]
	lsls	r1, r1, #4
	movs	r0, #16
	bl	sub_08014d00
	movs	r1, #192
	lsls	r1, r1, #3
	movs	r0, #12
	bl	sub_08014d00
	movs	r0, #4
	bl	sub_08020088
	movs	r0, #183
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_081198f2
	movs	r0, #1
	bl	sub_08038008
	b.n	.L_081198f8
.L_081198f2:
	movs	r0, #0
	bl	sub_08038008
.L_081198f8:
	mov	r5, r9
	bl	sub_081196fc
	ldr	r0, [r5, #0]
	bl	sub_08127cd4
	adds	r6, r0, #0
	movs	r0, #182
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_0811992a
	adds	r5, #68
	movs	r3, #1
	str	r5, [sp, #16]
	strb	r3, [r5, #0]
	ldr	r3, [pc, #412]
	movs	r0, #166
	lsls	r0, r0, #1
	adds	r0, #255
	adds	r3, r3, r0
	movs	r2, #4
	strb	r2, [r3, #0]
	b.n	.L_08119930
.L_0811992a:
	mov	r1, r9
	adds	r1, #68
	str	r1, [sp, #16]
.L_08119930:
	ldr	r2, [sp, #16]
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_081199ae
	ldr	r2, [pc, #384]
	movs	r3, #0
	str	r3, [r2, #0]
	movs	r5, #0
	ldr	r3, [pc, #380]
	mov	r6, r9
	movs	r0, #1
	mov	r8, r3
	adds	r6, #82
	movs	r7, #3
	mov	sl, r0
.L_0811994e:
	mov	r1, r8
	ldrh	r2, [r1, #0]
	adds	r3, r7, #0
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L_0811996a
	movs	r0, #1
	adds	r5, #1
	bl	sub_08013560
	cmp	r5, #24
	ble.n	.L_0811994e
	mov	r2, sl
	strb	r2, [r6, #0]
.L_0811996a:
	ldr	r3, [pc, #344]
	mov	r2, r9
	ldr	r3, [r3, #0]
	adds	r2, #80
	lsls	r3, r3, #26
	lsrs	r3, r3, #30
	strb	r3, [r2, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #216
	movs	r4, #217
	ldr	r2, [r3, #0]
	ldr	r1, [pc, #324]
	lsls	r4, r4, #3
	movs	r0, #0
	adds	r4, #255
.L_0811998a:
	ldrb	r3, [r1, #0]
	adds	r0, #1
	strb	r3, [r2, #0]
	adds	r1, #1
	adds	r2, #1
	cmp	r0, r4
	bls.n	.L_0811998a
	movs	r0, #252
	lsls	r0, r0, #2
	bl	sub_08016d5c
	adds	r6, r0, #0
	bl	sub_081195a0
	mov	r2, r9
	adds	r2, #66
	movs	r3, #0
	strb	r3, [r2, #0]
.L_081199ae:
	movs	r1, #224
	lsls	r1, r1, #2
	adds	r1, #255
	ldr	r0, [pc, #276]
	bl	sub_080145a8
	movs	r5, #128
	ldr	r3, [pc, #248]
	lsls	r5, r5, #2
	adds	r5, #14
	adds	r3, r3, r5
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	cmp	r0, #0
	beq.n	.L_081199ea
	bl	sub_081c0010
	movs	r0, #182
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_081199f0
	movs	r0, #55
	bl	sub_081c0010
	movs	r0, #4
	bl	sub_08013b30
	b.n	.L_081199f0
.L_081199ea:
	movs	r0, #50
	bl	sub_081c0010
.L_081199f0:
	bl	sub_08118c68
	bl	sub_0811b37c
	bl	sub_08118eb0
	bl	sub_08118f6c
	movs	r0, #0
	bl	sub_080ad000
	ldrh	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L_08119a18
	mov	r2, r9
	adds	r2, #65
	movs	r3, #3
	str	r2, [sp, #20]
	strb	r3, [r2, #0]
	b.n	.L_08119a24
.L_08119a18:
	mov	r3, r9
	adds	r3, #65
	str	r3, [sp, #20]
	ldr	r5, [sp, #20]
	movs	r3, #1
	strb	r3, [r5, #0]
.L_08119a24:
	ldr	r3, [sp, #40]
	subs	r3, #75
	cmp	r3, #1
	bhi.n	.L_08119a30
	bl	sub_081185c4
.L_08119a30:
	movs	r0, #9
	bl	sub_08038128
	bl	sub_0811bddc
	bl	.L_0811a44c
	bl	sub_081263c4
	movs	r3, #206
	lsls	r3, r3, #3
	add	r3, r9
	ldrh	r1, [r3, #0]
	movs	r0, #1
	movs	r2, #0
	bl	sub_081263fc
	movs	r3, #128
	lsls	r3, r3, #10
	movs	r0, #160
	movs	r1, #160
	str	r3, [sp, #0]
	lsls	r0, r0, #16
	lsls	r1, r1, #15
	movs	r2, #0
	movs	r3, #0
	bl	sub_08126548
	movs	r1, #0
	movs	r2, #0
	movs	r3, #190
	movs	r0, #0
	bl	sub_08126804
	movs	r0, #1
	bl	sub_08118d6c
	ldr	r5, [pc, #48]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #80
	strh	r5, [r3, #0]
	bl	sub_081281ec
	movs	r0, #128
	bl	sub_080143e0
	mov	r1, r9
	str	r0, [r1, #84]
	adds	r1, #69
	movs	r0, #183
	str	r1, [sp, #24]
	lsls	r0, r0, #1
	strb	r5, [r1, #0]
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_08119ad0
	ldr	r2, [sp, #24]
	movs	r3, #1
	b.n	.L_08119b10
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x03000258
	.4byte 0x03000260
	.4byte 0x02000240
	.4byte 0x020054c8
	.4byte 0x0300124c
	.4byte 0x04000128
	.4byte 0x02018000
	.2byte 0x8add
	.2byte 0x0811
.L_08119ad0:
	ldr	r3, [pc, #500]
	movs	r5, #166
	lsls	r5, r5, #1
	adds	r5, #255
	adds	r3, r3, r5
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_08119b08
	bl	sub_080ad148
	movs	r3, #15
	ands	r0, r3
	cmp	r0, #0
	bne.n	.L_08119af4
	ldr	r0, [sp, #24]
	movs	r3, #1
	strb	r3, [r0, #0]
	b.n	.L_08119b12
.L_08119af4:
	bl	sub_080ad148
	movs	r3, #31
	ands	r0, r3
	cmp	r0, #0
	bne.n	.L_08119b12
	ldr	r1, [sp, #24]
	movs	r3, #2
	strb	r3, [r1, #0]
	b.n	.L_08119b12
.L_08119b08:
	cmp	r3, #6
	bne.n	.L_08119b12
	ldr	r2, [sp, #24]
	movs	r3, #2
.L_08119b10:
	strb	r3, [r2, #0]
.L_08119b12:
	ldr	r1, [sp, #40]
	adds	r0, r6, #0
	bl	sub_08125d74
	ldr	r3, [sp, #36]
	movs	r2, #0
	str	r2, [r3, #20]
	ldr	r3, [pc, #424]
	movs	r1, #144
	strb	r2, [r3, #0]
	ldr	r0, [pc, #424]
	lsls	r1, r1, #3
	bl	sub_080145a8
	movs	r5, #1
	str	r5, [sp, #32]
.L_08119b32:
	movs	r0, #1
	movs	r1, #0
	bl	.L_0811a39c
	cmp	r0, #0
	bne.n	.L_08119b42
	bl	sub_081195ec
.L_08119b42:
	bl	sub_0811d7e4
	bl	sub_08118f6c
	movs	r0, #0
	bl	sub_080ad000
	ldrh	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L_08119b5e
	ldr	r0, [sp, #20]
	movs	r3, #3
	strb	r3, [r0, #0]
	b.n	.L_08119b64
.L_08119b5e:
	ldr	r1, [sp, #20]
	movs	r3, #1
	strb	r3, [r1, #0]
.L_08119b64:
	ldr	r2, [sp, #36]
	movs	r3, #160
	lsls	r3, r3, #6
	str	r3, [r2, #0]
	movs	r3, #60
	str	r3, [r2, #4]
	ldr	r3, [sp, #20]
	movs	r5, #187
	ldrb	r0, [r3, #0]
	lsls	r5, r5, #2
	bl	sub_08038130
	add	r5, r9
	movs	r1, #160
	ldr	r3, [pc, #336]
	lsls	r1, r1, #1
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4649
	ldr	r0, [r1, #84]
	bl	sub_08014274
	movs	r0, #181
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_08119bb4
	adds	r0, r5, #0
	bl	sub_0811d61c
	b.n	.L_08119bba
.L_08119ba6:
	ldr	r0, [sp, #40]
	adds	r2, r0, #0
	adds	r2, #1
	str	r2, [sp, #40]
	bl	sub_081197a8
	b.n	.L_08119dde
.L_08119bb4:
	adds	r0, r5, #0
	bl	sub_0811c3bc
.L_08119bba:
	str	r0, [sp, #28]
	movs	r0, #128
	bl	sub_080143e0
	mov	r3, r9
	str	r0, [r3, #84]
	ldr	r5, [sp, #20]
	ldrb	r0, [r5, #0]
	bl	sub_08038130
	ldr	r0, [sp, #28]
	cmp	r0, #0
	bge.n	.L_08119bd6
	b.n	.L_08119f48
.L_08119bd6:
	movs	r1, #0
	mov	r8, r1
	cmp	r8, r0
	blt.n	.L_08119be0
	b.n	.L_08119dde
.L_08119be0:
	mov	r2, r9
	adds	r2, #71
	movs	r3, #189
	movs	r4, #187
	movs	r5, #187
	str	r2, [sp, #12]
	str	r1, [sp, #8]
	lsls	r3, r3, #2
	lsls	r4, r4, #2
	lsls	r5, r5, #2
	mov	sl, r3
	add	r4, r9
	mov	fp, r5
.L_08119bfa:
	movs	r3, #128
	mov	r0, fp
	mov	r2, r9
	lsls	r3, r3, #4
	ldrsh	r5, [r0, r2]
	adds	r3, #88
	movs	r2, #1
	add	r3, r9
	negs	r2, r2
	str	r2, [r3, #0]
	ldr	r0, [sp, #12]
	movs	r3, #0
	strb	r3, [r0, #0]
	movs	r0, #181
	lsls	r0, r0, #1
	str	r4, [sp, #4]
	bl	sub_08016ce4
	ldr	r4, [sp, #4]
	cmp	r0, #0
	bne.n	.L_08119c46
	ldr	r3, [sp, #8]
	movs	r1, #187
	lsls	r1, r1, #2
	add	r3, r9
	mov	r2, r8
	adds	r0, r3, r1
	movs	r1, #10
	cmp	r2, #0
	beq.n	.L_08119c38
	movs	r1, #0
.L_08119c38:
	str	r4, [sp, #4]
	bl	sub_0811d9cc
	ldr	r4, [sp, #4]
	cmp	r0, #1
	bne.n	.L_08119c5e
	b.n	.L_08119fd0
.L_08119c46:
	ldr	r0, [sp, #8]
	movs	r3, #187
	lsls	r3, r3, #2
	add	r0, r9
	adds	r0, r0, r3
	str	r4, [sp, #4]
	bl	sub_0811c594
	ldr	r4, [sp, #4]
	cmp	r0, #1
	bne.n	.L_08119c5e
	b.n	.L_08119fd0
.L_08119c5e:
	movs	r1, #128
	ldr	r0, [sp, #40]
	lsls	r1, r1, #2
	adds	r1, #126
	cmp	r0, r1
	bne.n	.L_08119c6c
	b.n	.L_08119f78
.L_08119c6c:
	movs	r0, #1
	movs	r1, #0
	str	r4, [sp, #4]
	bl	.L_0811a24c
	cmp	r0, #0
	bne.n	.L_08119c7c
	b.n	.L_08119f78
.L_08119c7c:
	movs	r0, #1
	movs	r1, #0
	bl	.L_0811a188
	cmp	r0, #0
	bne.n	.L_08119c8a
	b.n	.L_08119dde
.L_08119c8a:
	movs	r0, #2
	movs	r1, #0
	bl	.L_0811a188
	ldr	r4, [sp, #4]
	cmp	r0, #0
	bne.n	.L_08119cd8
	ldr	r3, [sp, #40]
	subs	r3, #78
	cmp	r3, #1
	bls.n	.L_08119ba6
	ldr	r2, [sp, #40]
	cmp	r2, #80
	bne.n	.L_08119cac
	movs	r0, #80
	bl	sub_081197a8
.L_08119cac:
	cmp	r5, #7
	bls.n	.L_08119cb2
	b.n	.L_08119eb8
.L_08119cb2:
	movs	r3, #171
	lsls	r3, r3, #3
	add	r3, r9
	ldr	r3, [r3, #0]
	cmp	r3, #1
	beq.n	.L_08119cc0
	b.n	.L_08119eb8
.L_08119cc0:
	mov	r5, r9
	movs	r3, #3
	strh	r3, [r5, #62]
	b.n	.L_08119eb8
	.4byte 0x02000240
	.4byte 0x0300123c
	.4byte 0x0811b9fd
	.2byte 0x0258
	.2byte 0x0300
.L_08119cd8:
	movs	r5, #128
	lsls	r5, r5, #4
	adds	r5, #88
	add	r5, r9
	ldr	r3, [r5, #0]
	movs	r7, #1
	negs	r7, r7
	cmp	r3, r7
	beq.n	.L_08119d40
	movs	r6, #128
	lsls	r6, r6, #4
	adds	r6, #92
	add	r6, r9
	ldr	r0, [r6, #0]
	str	r4, [sp, #4]
	bl	sub_0811d748
	ldr	r4, [sp, #4]
	cmp	r0, r7
	beq.n	.L_08119d40
	ldr	r3, [r5, #0]
	mov	r0, fp
	mov	r1, r9
	strh	r3, [r0, r1]
	movs	r3, #9
	strh	r3, [r4, #6]
	ldr	r3, [pc, #44]
	mov	r2, sl
	mov	r5, r9
	strh	r3, [r2, r5]
	ldr	r0, [r6, #0]
	bl	sub_0811d748
	ldr	r4, [sp, #4]
	movs	r3, #1
	strh	r0, [r4, #10]
	strh	r3, [r4, #12]
	ldr	r1, [sp, #8]
	movs	r0, #16
	subs	r1, #16
	movs	r2, #1
	str	r1, [sp, #8]
	negs	r0, r0
	negs	r2, r2
	add	sl, r0
	subs	r4, #16
	add	fp, r0
	add	r8, r2
	b.n	.L_08119d40
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0000
.L_08119d40:
	str	r4, [sp, #4]
	bl	sub_08119374
	ldr	r4, [sp, #4]
	cmp	r0, #0
	bge.n	.L_08119d4e
	b.n	.L_08119f48
.L_08119d4e:
	ldr	r3, [sp, #28]
	subs	r3, #1
	cmp	r8, r3
	bne.n	.L_08119dbc
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #100
	add	r0, r9
	ldrb	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L_08119dbc
	movs	r2, #134
	lsls	r2, r2, #4
	add	r2, r9
	ldrh	r1, [r2, #0]
	mov	r5, sl
	lsrs	r3, r1, #1
	strh	r3, [r2, #0]
	movs	r3, #1
	ands	r1, r3
	ldrb	r3, [r0, #0]
	mov	r2, r9
	adds	r3, #255
	strb	r3, [r0, #0]
	ldr	r0, [pc, #52]
	lsls	r3, r1, #7
	strh	r3, [r4, #0]
	movs	r3, #10
	strh	r3, [r4, #6]
	strh	r0, [r5, r2]
	mov	r3, sl
	ldr	r2, [pc, #40]
	cmp	r1, #0
	beq.n	.L_08119d94
	ldr	r2, [pc, #32]
.L_08119d94:
	add	r3, r9
	strh	r2, [r3, #2]
	movs	r3, #255
	strh	r3, [r4, #12]
	ldr	r5, [sp, #8]
	movs	r3, #16
	subs	r5, #16
	movs	r0, #1
	str	r5, [sp, #8]
	negs	r3, r3
	negs	r0, r0
	add	sl, r3
	subs	r4, #16
	add	fp, r3
	add	r8, r0
	b.n	.L_08119dbc
	.4byte 0x00000000
	.2byte 0x0080
	.2byte 0x0000
.L_08119dbc:
	ldr	r1, [sp, #12]
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	bne.n	.L_08119dde
	ldr	r3, [sp, #8]
	ldr	r0, [sp, #28]
	movs	r5, #1
	movs	r2, #16
	adds	r3, #16
	add	r8, r5
	add	sl, r2
	adds	r4, #16
	str	r3, [sp, #8]
	add	fp, r2
	cmp	r8, r0
	bge.n	.L_08119dde
	b.n	.L_08119bfa
.L_08119dde:
	bl	sub_08118e64
	movs	r1, #128
	lsls	r1, r1, #4
	adds	r1, #98
	add	r1, r9
	ldrh	r0, [r1, #0]
	movs	r2, #134
	lsls	r2, r2, #4
	add	r2, r9
	movs	r3, #0
	strh	r0, [r2, #0]
	strh	r3, [r1, #0]
	movs	r1, #128
	lsls	r1, r1, #4
	adds	r1, #101
	add	r1, r9
	movs	r3, #128
	ldrb	r2, [r1, #0]
	ldr	r4, [pc, #40]
	lsls	r3, r3, #4
	adds	r3, #100
	add	r3, r9
	strb	r2, [r3, #0]
	strb	r4, [r1, #0]
	ldr	r1, [sp, #24]
	strb	r4, [r1, #0]
	bl	sub_08124cc0
	bl	sub_08124e20
	bl	sub_0811bc98
	ldr	r5, [sp, #40]
	subs	r5, #74
	cmp	r5, #1
	bhi.n	.L_08119e34
	ldr	r2, [sp, #32]
	cmp	r2, #2
	beq.n	.L_08119ebc
	b.n	.L_08119e34
	.2byte 0x0000
	.2byte 0x0000
.L_08119e34:
	ldr	r0, [sp, #16]
	ldrb	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L_08119e46
	bl	sub_08119374
	cmp	r0, #0
	bge.n	.L_08119e4c
	b.n	.L_08119f4c
.L_08119e46:
	movs	r0, #20
	bl	sub_08013560
.L_08119e4c:
	movs	r0, #183
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_08119eb0
	bl	.L_081197d0
	movs	r1, #0
	adds	r0, #7
	movs	r2, #4
	movs	r3, #1
	bl	sub_08038038
	adds	r5, r0, #0
	b.n	.L_08119e72
.L_08119e6c:
	movs	r0, #1
	bl	sub_08013560
.L_08119e72:
	bl	sub_08038048
	cmp	r0, #0
	beq.n	.L_08119e6c
	movs	r1, #1
	adds	r0, r5, #0
	bl	sub_08038018
	movs	r0, #1
	bl	sub_08013560
	bl	.L_081197d0
	movs	r2, #4
	movs	r3, #1
	movs	r1, #10
	adds	r0, #8
	bl	sub_08038038
	movs	r1, #24
	adds	r5, r0, #0
	movs	r0, #44
	bl	sub_08120060
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08038018
	movs	r0, #1
	bl	sub_08013560
.L_08119eb0:
	ldr	r1, [sp, #32]
	adds	r1, #1
	str	r1, [sp, #32]
	b.n	.L_08119b32
.L_08119eb8:
	ldr	r5, [sp, #40]
	subs	r5, #74
.L_08119ebc:
	bl	sub_081195d4
	cmp	r5, #1
	bls.n	.L_08119f34
	ldr	r2, [sp, #40]
	cmp	r2, #80
	beq.n	.L_08119f2e
	movs	r0, #183
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_08119f2e
	ldr	r0, [sp, #16]
	ldrb	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L_08119ee4
	movs	r0, #58
	bl	sub_081c0010
.L_08119ee4:
	movs	r3, #171
	lsls	r3, r3, #3
	add	r3, r9
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08119f2a
	movs	r0, #58
	bl	sub_081c0010
	mov	r1, r9
	ldrh	r3, [r1, #62]
	cmp	r3, #1
	bhi.n	.L_08119f2a
	ldrh	r3, [r1, #60]
	movs	r2, #26
	lsls	r3, r3, #1
	adds	r3, #16
	ldrh	r1, [r1, r3]
	movs	r0, #128
	bl	sub_080ad0e8
	bl	sub_08038118
	movs	r0, #128
	movs	r1, #1
	bl	sub_08038120
	mov	r2, r9
	ldrh	r0, [r2, #62]
	ldr	r3, [pc, #84]
	adds	r0, r0, r3
	bl	sub_080381c8
	bl	sub_0811ff08
.L_08119f2a:
	bl	sub_081284c0
.L_08119f2e:
	movs	r0, #78
	bl	sub_081c0010
.L_08119f34:
	movs	r0, #30
	bl	sub_08013e70
	bl	sub_08013fdc
	movs	r3, #171
	lsls	r3, r3, #3
	add	r3, r9
	ldr	r7, [r3, #0]
	b.n	.L_08119fea
.L_08119f48:
	ldr	r5, [sp, #40]
	subs	r5, #74
.L_08119f4c:
	bl	sub_081195d4
	movs	r0, #0
	bl	sub_08014694
	ldr	r3, [pc, #24]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	movs	r3, #171
	lsls	r3, r3, #3
	movs	r0, #250
	add	r3, r9
	lsls	r0, r0, #2
	ldr	r7, [r3, #0]
	bl	sub_08016cfc
	b.n	.L_08119fea
	.4byte 0x00000001
	.2byte 0x0c84
	.2byte 0x0000
.L_08119f78:
	bl	sub_081195d4
	ldr	r5, [sp, #40]
	subs	r5, #74
	cmp	r5, #1
	bls.n	.L_08119fc0
	movs	r0, #128
	ldr	r3, [sp, #40]
	lsls	r0, r0, #2
	adds	r0, #126
	cmp	r3, r0
	beq.n	.L_08119fc0
	cmp	r3, #76
	beq.n	.L_08119fba
	movs	r0, #59
	bl	sub_081c0010
	bl	sub_08038118
	movs	r0, #0
	bl	.L_0811a038
	cmp	r0, #1
	bne.n	.L_08119fb0
	ldr	r0, [pc, #124]
	bl	sub_080381c8
	b.n	.L_08119fb6
.L_08119fb0:
	ldr	r0, [pc, #120]
	bl	sub_080381c8
.L_08119fb6:
	bl	sub_0811ff08
.L_08119fba:
	movs	r0, #78
	bl	sub_081c0010
.L_08119fc0:
	movs	r0, #30
	movs	r7, #1
	bl	sub_08013e70
	negs	r7, r7
	bl	sub_08013fdc
	b.n	.L_08119fea
.L_08119fd0:
	movs	r0, #78
	bl	sub_081c0010
	movs	r0, #30
	bl	sub_08013e70
	bl	sub_08013fdc
	ldr	r5, [sp, #40]
	movs	r7, #186
	lsls	r7, r7, #2
	adds	r7, #255
	subs	r5, #74
.L_08119fea:
	cmp	r5, #1
	bhi.n	.L_08119ff2
	bl	sub_0811843c
.L_08119ff2:
	bl	sub_08118d70
	bl	sub_08124cc0
	bl	sub_08124bfc
	ldr	r3, [pc, #48]
	movs	r1, #166
	lsls	r1, r1, #1
	adds	r1, #255
	adds	r3, r3, r1
	movs	r2, #0
	strb	r2, [r3, #0]
	ldr	r0, [pc, #36]
	bl	sub_08014644
	bl	sub_081263f0
	adds	r0, r7, #0
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x00000c89
	.4byte 0x00000c83
	.4byte 0x02000240
	.2byte 0xb9fd
	.2byte 0x0811
.L_0811a038:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	sub	sp, #4
	adds	r3, #68
	ldrb	r3, [r3, #0]
	adds	r5, r0, #0
	movs	r6, #4
	cmp	r3, #0
	beq.n	.L_0811a054
	movs	r6, #3
.L_0811a054:
	bl	sub_080ad0f0
	adds	r7, r0, #0
	cmp	r7, r6
	ble.n	.L_0811a060
	adds	r7, r6, #0
.L_0811a060:
	cmp	r7, #0
	ble.n	.L_0811a096
	ldr	r3, [pc, #68]
	movs	r1, #134
	lsls	r1, r1, #2
	adds	r2, r3, r1
	movs	r3, #2
	mov	r8, r3
	adds	r6, r7, #0
.L_0811a072:
	ldrb	r0, [r2, #0]
	adds	r2, #1
	cmp	r5, #0
	beq.n	.L_0811a07e
	strh	r0, [r5, #0]
	adds	r5, #2
.L_0811a07e:
	str	r2, [sp, #0]
	bl	sub_08016ca4
	movs	r1, #149
	lsls	r1, r1, #1
	adds	r3, r0, r1
	subs	r6, #1
	mov	r1, r8
	strb	r1, [r3, #0]
	ldr	r2, [sp, #0]
	cmp	r6, #0
	bne.n	.L_0811a072
.L_0811a096:
	cmp	r5, #0
	beq.n	.L_0811a09e
	ldr	r3, [pc, #12]
	strh	r3, [r5, #0]
.L_0811a09e:
	adds	r0, r7, #0
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x000000ff
	.2byte 0x0240
	.2byte 0x0200
.L_0811a0b0:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	adds	r6, r0, #0
	bl	sub_080ad0f0
	adds	r7, r0, #0
	cmp	r7, #4
	ble.n	.L_0811a0c8
	subs	r7, #4
	b.n	.L_0811a0ca
.L_0811a0c8:
	movs	r7, #0
.L_0811a0ca:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	adds	r3, #68
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0811a0da
	movs	r7, #0
.L_0811a0da:
	cmp	r7, #0
	ble.n	.L_0811a110
	ldr	r3, [pc, #72]
	movs	r1, #135
	lsls	r1, r1, #2
	adds	r2, r3, r1
	movs	r3, #2
	mov	r8, r3
	adds	r5, r7, #0
.L_0811a0ec:
	ldrb	r0, [r2, #0]
	adds	r2, #1
	cmp	r6, #0
	beq.n	.L_0811a0f8
	strh	r0, [r6, #0]
	adds	r6, #2
.L_0811a0f8:
	str	r2, [sp, #0]
	bl	sub_08016ca4
	movs	r1, #149
	lsls	r1, r1, #1
	adds	r3, r0, r1
	subs	r5, #1
	mov	r1, r8
	strb	r1, [r3, #0]
	ldr	r2, [sp, #0]
	cmp	r5, #0
	bne.n	.L_0811a0ec
.L_0811a110:
	cmp	r6, #0
	beq.n	.L_0811a118
	ldr	r3, [pc, #12]
	strh	r3, [r6, #0]
.L_0811a118:
	adds	r0, r7, #0
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x000000ff
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	movs	r2, #0
	mov	r8, r2
	movs	r7, #6
	movs	r0, #0
	cmp	r6, #0
	beq.n	.L_0811a17c
	movs	r0, #182
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_0811a14e
	movs	r7, #3
.L_0811a14e:
	movs	r5, #128
	adds	r7, #128
	cmp	r5, r7
	bge.n	.L_0811a176
.L_0811a156:
	adds	r0, r5, #0
	bl	sub_08016ca4
	movs	r2, #149
	lsls	r2, r2, #1
	adds	r3, r0, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0811a170
	movs	r3, #1
	strh	r5, [r6, #0]
	add	r8, r3
	adds	r6, #2
.L_0811a170:
	adds	r5, #1
	cmp	r5, r7
	blt.n	.L_0811a156
.L_0811a176:
	ldr	r3, [pc, #12]
	mov	r0, r8
	strh	r3, [r6, #0]
.L_0811a17c:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x00ff
	.2byte 0x0000
.L_0811a188:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	sl, r0
	movs	r0, #182
	adds	r6, r1, #0
	movs	r2, #6
	movs	r1, #0
	lsls	r0, r0, #1
	sub	sp, #20
	mov	r8, r1
	mov	r9, r2
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_0811a1b0
	movs	r3, #3
	mov	r9, r3
.L_0811a1b0:
	movs	r3, #1
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0811a1f2
	add	r5, sp, #4
	adds	r0, r5, #0
	bl	.L_0811a038
	cmp	r8, r0
	bge.n	.L_0811a1f2
	adds	r2, r5, #0
	adds	r5, r0, #0
.L_0811a1ca:
	ldrh	r7, [r2, #0]
	adds	r2, #2
	adds	r0, r7, #0
	str	r2, [sp, #0]
	bl	sub_08016ca4
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	ldr	r2, [sp, #0]
	cmp	r3, #0
	ble.n	.L_0811a1ec
	cmp	r6, #0
	beq.n	.L_0811a1e8
	strh	r7, [r6, #0]
	adds	r6, #2
.L_0811a1e8:
	movs	r3, #1
	add	r8, r3
.L_0811a1ec:
	subs	r5, #1
	cmp	r5, #0
	bne.n	.L_0811a1ca
.L_0811a1f2:
	movs	r3, #2
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0811a232
	mov	r7, r9
	movs	r5, #128
	adds	r7, #128
	cmp	r5, r7
	bge.n	.L_0811a232
.L_0811a206:
	adds	r0, r5, #0
	bl	sub_08016ca4
	movs	r2, #149
	lsls	r2, r2, #1
	adds	r3, r0, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0811a22c
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	cmp	r3, #0
	ble.n	.L_0811a22c
	cmp	r6, #0
	beq.n	.L_0811a228
	strh	r5, [r6, #0]
	adds	r6, #2
.L_0811a228:
	movs	r2, #1
	add	r8, r2
.L_0811a22c:
	adds	r5, #1
	cmp	r5, r7
	blt.n	.L_0811a206
.L_0811a232:
	cmp	r6, #0
	beq.n	.L_0811a23a
	ldr	r3, [pc, #16]
	strh	r3, [r6, #0]
.L_0811a23a:
	mov	r0, r8
	add	sp, #20
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x00ff
	.2byte 0x0000
.L_0811a24c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	sl, r0
	movs	r0, #182
	adds	r7, r1, #0
	movs	r2, #6
	movs	r1, #0
	lsls	r0, r0, #1
	sub	sp, #24
	mov	r8, r1
	mov	r9, r2
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_0811a274
	movs	r3, #3
	mov	r9, r3
.L_0811a274:
	movs	r3, #1
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0811a2c0
	add	r6, sp, #4
	adds	r0, r6, #0
	bl	.L_0811a038
	adds	r5, r0, #0
	lsls	r0, r5, #1
	adds	r0, r6, r0
	bl	.L_0811a0b0
	adds	r5, r5, r0
	cmp	r8, r5
	bge.n	.L_0811a2c0
	adds	r2, r6, #0
.L_0811a298:
	ldrh	r6, [r2, #0]
	adds	r2, #2
	adds	r0, r6, #0
	str	r2, [sp, #0]
	bl	sub_08016ca4
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	ldr	r2, [sp, #0]
	cmp	r3, #0
	ble.n	.L_0811a2ba
	cmp	r7, #0
	beq.n	.L_0811a2b6
	strh	r6, [r7, #0]
	adds	r7, #2
.L_0811a2b6:
	movs	r3, #1
	add	r8, r3
.L_0811a2ba:
	subs	r5, #1
	cmp	r5, #0
	bne.n	.L_0811a298
.L_0811a2c0:
	movs	r3, #2
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0811a300
	mov	r6, r9
	movs	r5, #128
	adds	r6, #128
	cmp	r5, r6
	bge.n	.L_0811a300
.L_0811a2d4:
	adds	r0, r5, #0
	bl	sub_08016ca4
	movs	r2, #149
	lsls	r2, r2, #1
	adds	r3, r0, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0811a2fa
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	cmp	r3, #0
	ble.n	.L_0811a2fa
	cmp	r7, #0
	beq.n	.L_0811a2f6
	strh	r5, [r7, #0]
	adds	r7, #2
.L_0811a2f6:
	movs	r2, #1
	add	r8, r2
.L_0811a2fa:
	adds	r5, #1
	cmp	r5, r6
	blt.n	.L_0811a2d4
.L_0811a300:
	cmp	r7, #0
	beq.n	.L_0811a308
	ldr	r3, [pc, #16]
	strh	r3, [r7, #0]
.L_0811a308:
	mov	r0, r8
	add	sp, #24
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x00ff
	.2byte 0x0000
.L_0811a31c:
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r6, r0, #0
	ldr	r0, [r3, #36]
	movs	r3, #1
	ands	r3, r6
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_0811a35a
	movs	r3, #88
	ldrsh	r3, [r0, r3]
	cmp	r3, #255
	beq.n	.L_0811a35a
	adds	r2, r0, #0
	adds	r2, #88
.L_0811a33c:
	movs	r7, #0
	ldrsh	r3, [r2, r7]
	ldrh	r4, [r2, #0]
	cmp	r3, #254
	beq.n	.L_0811a350
	cmp	r1, #0
	beq.n	.L_0811a34e
	strh	r4, [r1, #0]
	adds	r1, #2
.L_0811a34e:
	adds	r5, #1
.L_0811a350:
	adds	r2, #2
	movs	r4, #0
	ldrsh	r3, [r2, r4]
	cmp	r3, #255
	bne.n	.L_0811a33c
.L_0811a35a:
	movs	r3, #2
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_0811a38c
	adds	r2, r0, #2
	movs	r3, #100
	ldrsh	r3, [r2, r3]
	mov	ip, r2
	cmp	r3, #255
	beq.n	.L_0811a38c
	movs	r0, #100
.L_0811a370:
	ldrsh	r3, [r2, r0]
	ldrh	r4, [r2, r0]
	cmp	r3, #254
	beq.n	.L_0811a382
	cmp	r1, #0
	beq.n	.L_0811a380
	strh	r4, [r1, #0]
	adds	r1, #2
.L_0811a380:
	adds	r5, #1
.L_0811a382:
	adds	r0, #2
	mov	r2, ip
	ldrsh	r3, [r2, r0]
	cmp	r3, #255
	bne.n	.L_0811a370
.L_0811a38c:
	cmp	r1, #0
	beq.n	.L_0811a394
	ldr	r3, [pc, #4]
	strh	r3, [r1, #0]
.L_0811a394:
	adds	r0, r5, #0
	pop	{r5, r6, r7, pc}
	.2byte 0x00ff
	.2byte 0x0000
.L_0811a39c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #36]
	movs	r3, #1
	ands	r3, r0
	sub	sp, #4
	mov	r8, r0
	adds	r6, r1, #0
	movs	r2, #0
	cmp	r3, #0
	beq.n	.L_0811a3f2
	movs	r3, #88
	ldrsh	r3, [r7, r3]
	cmp	r3, #255
	beq.n	.L_0811a3f2
	adds	r5, r7, #0
	adds	r5, #88
.L_0811a3c4:
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	cmp	r0, #254
	beq.n	.L_0811a3e8
	str	r2, [sp, #0]
	bl	sub_08016ca4
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	ldr	r2, [sp, #0]
	cmp	r3, #0
	beq.n	.L_0811a3e8
	cmp	r6, #0
	beq.n	.L_0811a3e6
	ldrh	r3, [r5, #0]
	strh	r3, [r6, #0]
	adds	r6, #2
.L_0811a3e6:
	adds	r2, #1
.L_0811a3e8:
	adds	r5, #2
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, #255
	bne.n	.L_0811a3c4
.L_0811a3f2:
	movs	r3, #2
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0811a436
	adds	r5, r7, #2
	movs	r3, #100
	ldrsh	r3, [r5, r3]
	mov	r8, r5
	cmp	r3, #255
	beq.n	.L_0811a436
	movs	r7, #100
.L_0811a40a:
	ldrsh	r0, [r5, r7]
	cmp	r0, #254
	beq.n	.L_0811a42c
	str	r2, [sp, #0]
	bl	sub_08016ca4
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	ldr	r2, [sp, #0]
	cmp	r3, #0
	beq.n	.L_0811a42c
	cmp	r6, #0
	beq.n	.L_0811a42a
	ldrh	r3, [r5, r7]
	strh	r3, [r6, #0]
	adds	r6, #2
.L_0811a42a:
	adds	r2, #1
.L_0811a42c:
	adds	r7, #2
	mov	r5, r8
	ldrsh	r3, [r5, r7]
	cmp	r3, #255
	bne.n	.L_0811a40a
.L_0811a436:
	cmp	r6, #0
	beq.n	.L_0811a43e
	ldr	r3, [pc, #12]
	strh	r3, [r6, #0]
.L_0811a43e:
	adds	r0, r2, #0
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x00ff
	.2byte 0x0000
.L_0811a44c:
	push	{r5, lr}
	sub	sp, #28
	mov	r5, sp
	adds	r1, r5, #0
	movs	r0, #3
	bl	.L_0811a31c
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_0811b9d4
	add	sp, #28
	pop	{r5, pc}
	.2byte 0x0000
