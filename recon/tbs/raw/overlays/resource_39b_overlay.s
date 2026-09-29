.syntax unified
	.thumb
	.section .text.x02008d98,"ax",%progbits
	.p2align 2
	.global Makyuri_CyclePalette
	.thumb_func
Makyuri_CyclePalette:
	push {r5, r6, r7, lr}
	sub sp, #4
	ldr r3, [pc, #36]
	mov r5, sp
	adds r5, #2
	strh r3, [r5]
	ldr r3, [pc, #32]
	movs r1, #5
	ldr r0, [r3]
	bl 0x0200a3e4
	cmp r0, #0
	bne .L_02000d98_0
	ldr r3, [pc, #24]
	ldr r2, [r3]
	movs r1, #31
	adds r2, #4
	ands r2, r1
	str r2, [r3]
	movs r6, #0
	adds r7, r5, #0
	b .L_02000d98_1
	.4byte 0x00000000
	.4byte 0x03001e40
	.4byte 0x0200a91c
.L_02000d98_1:
	movs r3, #110
	subs r3, r3, r6
	movs r2, #160
	lsls r2, r2, #19
	lsls r3, r3, #1
	adds r3, r3, r2
	ldrh r2, [r3]
	movs r3, #31
	ands r3, r2
	strh r3, [r7]
	ldrh r5, [r7]
	cmp r6, #2
	bhi .L_02000d98_2
	lsls r0, r5, #2
	movs r1, #10
	bl 0x0200a3dc
	subs r5, r5, r0
.L_02000d98_2:
	movs r2, #111
	subs r2, r2, r6
	movs r3, #160
	lsls r3, r3, #19
	lsls r2, r2, #1
	adds r2, r2, r3
	ldr r3, [pc, #40]
	ldr r3, [r3]
	lsls r1, r3, #10
	ldr r3, [pc, #40]
	ldr r3, [r3]
	lsls r3, r3, #5
	orrs r1, r3
	orrs r5, r1
	adds r6, #1
	strh r5, [r2]
	cmp r6, #5
	bls .L_02000d98_1
	ldr r3, [pc, #24]
	ldr r3, [r3]
	ldr r2, [pc, #24]
	orrs r3, r1
	strh r3, [r2]
.L_02000d98_0:
	sub sp, #-4
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200a924
	.4byte 0x0200a920
	.4byte 0x0200a91c
	.4byte 0x050000d2
	.global Func_02000e3c
	.thumb_func
Func_02000e3c:
	push {lr}
	ldr r1, [pc, #8]
	ldr r3, [pc, #8]
	movs r2, #0
	b .L_02000e3c_0
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x050000de
.L_02000e3c_0:
	adds r2, #1
	strh r1, [r3]
	subs r3, #2
	cmp r2, #6
	bls .L_02000e3c_0
	pop {r0}
	bx r0
	.2byte 0x0000
	.section .text.x020091b4,"ax",%progbits
	.p2align 2
	.global MakyuriIriguchi_OpenEntrance
	.thumb_func
MakyuriIriguchi_OpenEntrance:
	push {lr}
	bl 0x0200a4dc
	movs r1, #2
	movs r0, #8
	bl 0x0200a544
	movs r0, #20
	bl 0x0200a4d4
	ldr r3, [pc, #44]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #64
	str r2, [r3]
	ldr r0, [pc, #36]
	movs r1, #31
	bl 0x0200a5b4
	ldr r3, [pc, #32]
	ldr r2, [pc, #32]
	adds r3, r3, r2
	movs r2, #3
	strb r2, [r3]
	movs r0, #36
	movs r1, #1
	bl 0x0200a5ac
	bl 0x0200a4e4
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x00000035
	.4byte 0x02000240
	.4byte 0x0000022b
	.section .text.x020092e0,"ax",%progbits
	.p2align 2
	.global Func_020012e0
	.thumb_func
Func_020012e0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, [pc, #952]
	movs r2, #253
	lsls r2, r2, #6
	sub sp, #68
	strh r2, [r3]
	ldr r2, [pc, #944]
	adds r3, #2
	strh r2, [r3]
	ldr r1, [pc, #944]
	movs r0, #21
	bl 0x02009a3c
	ldr r0, [pc, #940]
	bl 0x0200a4c4
	ldr r2, [pc, #936]
	ldr r3, [pc, #940]
	movs r4, #144
	adds r1, r2, r3
	movs r3, #11
	strh r3, [r1]
	lsls r4, r4, #2
	ldr r3, [pc, #932]
	adds r2, r2, r4
	movs r0, #0
	strh r3, [r2]
	bl 0x0200a5f4
	ldr r0, [pc, #924]
	bl 0x0200a4bc
	cmp r0, #0
	beq .L_020012e0_0
	movs r1, #200
	ldr r0, [pc, #916]
	lsls r1, r1, #4
	bl 0x0200a3f4
	b .L_020012e0_1
.L_020012e0_0:
	bl 0x02008e3c
.L_020012e0_1:
	ldr r3, [pc, #904]
	movs r1, #224
	ldr r3, [r3]
	lsls r1, r1, #1
	movs r2, #129
	adds r3, r3, r1
	lsls r2, r2, #2
	str r2, [r3]
	ldr r3, [pc, #868]
	subs r2, #66
	adds r3, r3, r2
	movs r4, #0
	ldrsh r3, [r3, r4]
	subs r3, #1
	cmp r3, #30
	bls .L_020012e0_2
	b .L_020012e0_3
.L_020012e0_2:
	ldr r2, [pc, #876]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	movs r0, r0
	str r3, [sp, #912]
	lsls r0, r0, #8
	str r3, [sp, #976]
	lsls r0, r0, #8
	str r4, [sp, #296]
	lsls r0, r0, #8
	str r4, [sp, #296]
	lsls r0, r0, #8
	str r4, [sp, #296]
	lsls r0, r0, #8
	str r4, [sp, #296]
	lsls r0, r0, #8
	str r4, [sp, #88]
	lsls r0, r0, #8
	str r4, [sp, #88]
	lsls r0, r0, #8
	str r4, [sp, #88]
	lsls r0, r0, #8
	str r4, [sp, #88]
	lsls r0, r0, #8
	str r4, [sp, #88]
	lsls r0, r0, #8
	str r4, [sp, #88]
	lsls r0, r0, #8
	str r7, [sp, #96]
	lsls r0, r0, #8
	str r7, [sp, #96]
	lsls r0, r0, #8
	str r7, [sp, #96]
	lsls r0, r0, #8
	str r7, [sp, #96]
	lsls r0, r0, #8
	str r6, [sp, #256]
	lsls r0, r0, #8
	str r6, [sp, #240]
	lsls r0, r0, #8
	str r6, [sp, #240]
	lsls r0, r0, #8
	str r6, [sp, #240]
	lsls r0, r0, #8
	str r7, [sp, #96]
	lsls r0, r0, #8
	str r7, [sp, #96]
	lsls r0, r0, #8
	str r7, [sp, #96]
	lsls r0, r0, #8
	str r7, [sp, #96]
	lsls r0, r0, #8
	str r6, [sp, #288]
	lsls r0, r0, #8
	str r7, [sp, #96]
	lsls r0, r0, #8
	str r7, [sp, #96]
	lsls r0, r0, #8
	str r7, [sp, #96]
	lsls r0, r0, #8
	str r7, [sp, #96]
	lsls r0, r0, #8
	str r6, [sp, #592]
	lsls r0, r0, #8
	str r7, [sp, #40]
	lsls r0, r0, #8
	ldr r0, [pc, #744]
	bl 0x0200a4bc
	cmp r0, #0
	bne .L_020012e0_4
	movs r0, #20
	bl 0x0200a5a4
.L_020012e0_4:
	movs r0, #12
	bl 0x0200a4ec
	ldr r5, [pc, #728]
	str r5, [r0, #24]
	movs r0, #13
	bl 0x0200a4ec
	str r5, [r0, #24]
	movs r0, #14
	bl 0x0200a4ec
	str r5, [r0, #24]
	movs r0, #1
	bl 0x0200a3ec
	b .L_020012e0_3
	.2byte 0x48aa
	.2byte 0xf001
	.2byte 0xf850
	.2byte 0x2800
	.2byte 0xd100
	.2byte 0xe17a
	.2byte 0x2314
	.2byte 0x2505
	.2byte 0x9300
	.2byte 0x2054
	.2byte 0x2105
	.2byte 0x220a
	.2byte 0x2307
	.2byte 0x9501
	.2byte 0xf001
	.2byte 0xf823
	.2byte 0x2325
	.2byte 0x9300
	.2byte 0x2065
	.2byte 0x2105
	.2byte 0x220c
	.2byte 0x2307
	.2byte 0x9501
	.2byte 0xf001
	.2byte 0xf81a
	.2byte 0xe166
	.2byte 0x21c8
	.2byte 0x489d
	.2byte 0x0109
	.2byte 0xf000
	.2byte 0xffd0
	.2byte 0x489a
	.2byte 0xf001
	.2byte 0xf831
	.2byte 0x2800
	.2byte 0xd018
	.2byte 0x2305
	.2byte 0x2203
	.2byte 0x9300
	.2byte 0x9201
	.2byte 0x2162
	.2byte 0x220a
	.2byte 0x2361
	.2byte 0x2025
	.2byte 0xf001
	.2byte 0xf801
	.2byte 0xf000
	.2byte 0xfff3
	.2byte 0x2001
	.2byte 0xf000
	.2byte 0xffb8
	.2byte 0x2306
	.2byte 0x2220
	.2byte 0x9300
	.2byte 0x9201
	.2byte 0x2046
	.2byte 0x2120
	.2byte 0x220d
	.2byte 0x2307
	.2byte 0xf000
	.2byte 0xfff6
	.2byte 0x4b88
	.2byte 0x21e1
	.2byte 0x0049
	.2byte 0x185b
	.2byte 0x2200
	.2byte 0x5e9b
	.2byte 0x2b06
	.2byte 0xd000
	.2byte 0xe13a
	.2byte 0x488d
	.2byte 0xf001
	.2byte 0xf80a
	.2byte 0x1c05
	.2byte 0x2d00
	.2byte 0xd000
	.2byte 0xe133
	.2byte 0x4889
	.2byte 0xf001
	.2byte 0xf807
	.2byte 0xf001
	.2byte 0xf811
	.2byte 0x2001
	.2byte 0x2101
	.2byte 0x2201
	.2byte 0x4249
	.2byte 0x4252
	.2byte 0x2300
	.2byte 0x4240
	.2byte 0xf001
	.2byte 0xf860
	.2byte 0xf000
	.2byte 0xffc6
	.2byte 0x2001
	.2byte 0xf000
	.2byte 0xff8b
	.2byte 0x2000
	.2byte 0xf001
	.2byte 0xf808
	.2byte 0x2382
	.2byte 0x041b
	.2byte 0x60c3
	.2byte 0x2000
	.2byte 0xf001
	.2byte 0xf802
	.2byte 0x2380
	.2byte 0x021b
	.2byte 0x6483
	.2byte 0x2000
	.2byte 0xf000
	.2byte 0xfffc
	.2byte 0x6445
	.2byte 0x2000
	.2byte 0xf000
	.2byte 0xfff8
	.2byte 0x3055
	.2byte 0x7005
	.2byte 0xf001
	.2byte 0xf868
	.2byte 0xf001
	.2byte 0xf86e
	.2byte 0x201e
	.2byte 0xf000
	.2byte 0xffe3
	.2byte 0x2000
	.2byte 0xf000
	.2byte 0xffec
	.2byte 0x2303
	.2byte 0x3055
	.2byte 0x7003
	.2byte 0x20cc
	.2byte 0xf001
	.2byte 0xf87a
	.2byte 0x2018
	.2byte 0xf000
	.2byte 0xffd7
	.2byte 0x2000
	.2byte 0xf000
	.2byte 0xffe0
	.2byte 0xab07
	.2byte 0x4699
	.2byte 0x464c
	.2byte 0x2307
	.2byte 0x2100
	.2byte 0xae04
	.2byte 0x6063
	.2byte 0x4682
	.2byte 0x4688
	.2byte 0x1c37
	.2byte 0x4642
	.2byte 0x0315
	.2byte 0x1c28
	.2byte 0xf000
	.2byte 0xff69
	.2byte 0x2300
	.2byte 0x6038
	.2byte 0x607b
	.2byte 0x1c28
	.2byte 0xf000
	.2byte 0xff5f
	.2byte 0x683a
	.2byte 0x60b8
	.2byte 0x1c13
	.2byte 0x2a00
	.2byte 0xda00
	.2byte 0x1cd3
	.2byte 0x0fc5
	.2byte 0x1945
	.2byte 0x109b
	.2byte 0x106d
	.2byte 0x1ad3
	.2byte 0x1b45
	.2byte 0x6033
	.2byte 0x60b5
	.2byte 0x4654
	.2byte 0x68e1
	.2byte 0x6922
	.2byte 0x68a0
	.2byte 0x6874
	.2byte 0x9400
	.2byte 0x4c57
	.2byte 0x9402
	.2byte 0x464c
	.2byte 0x9501
	.2byte 0x9403
	.2byte 0xf7fe
	.2byte 0xfdd8
	.2byte 0x2101
	.2byte 0x4488
	.2byte 0x4642
	.2byte 0x2a10
	.2byte 0xd9d4
	.2byte 0x20bc
	.2byte 0xf001
	.2byte 0xf83c
	.2byte 0x2000
	.2byte 0x4950
	.2byte 0xf000
	.2byte 0xffec
	.2byte 0x2000
	.2byte 0x2116
	.2byte 0xf000
	.2byte 0xffc4
	.2byte 0x20a0
	.2byte 0x21a0
	.2byte 0x2280
	.2byte 0x02c0
	.2byte 0x02c9
	.2byte 0x0252
	.2byte 0xf000
	.2byte 0xff70
	.2byte 0x2001
	.2byte 0x2101
	.2byte 0x4a48
	.2byte 0x4240
	.2byte 0x4249
	.2byte 0xf000
	.2byte 0xff69
	.2byte 0xf000
	.2byte 0xff6b
	.2byte 0x2180
	.2byte 0x0049
	.2byte 0x2000
	.2byte 0xf000
	.2byte 0xffd2
	.2byte 0x2000
	.2byte 0xf000
	.2byte 0xff87
	.2byte 0x2580
	.2byte 0x026d
	.2byte 0x6485
	.2byte 0x2000
	.2byte 0xf000
	.2byte 0xff81
	.2byte 0x2380
	.2byte 0x01db
	.2byte 0x6443
	.2byte 0x4833
	.2byte 0xf000
	.2byte 0xff63
	.2byte 0x2800
	.2byte 0xd149
	.2byte 0x1c28
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xffe1
	.2byte 0x2101
	.2byte 0x4838
	.2byte 0xf000
	.2byte 0xffd9
	.2byte 0x201e
	.2byte 0xf000
	.2byte 0xffde
	.2byte 0xf000
	.2byte 0xffe8
	.2byte 0x2101
	.2byte 0x2000
	.2byte 0xf000
	.2byte 0xff8c
	.2byte 0x201e
	.2byte 0xf000
	.2byte 0xff59
	.2byte 0x4832
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0xf000
	.2byte 0xff44
	.2byte 0x1c28
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xffc4
	.2byte 0x201e
	.2byte 0xf000
	.2byte 0xffc9
	.2byte 0xe028
	.2byte 0xf7ff
	.2byte 0xfde4
	.2byte 0x20aa
	.2byte 0xf000
	.2byte 0xffdf
	.2byte 0xe067
	.2byte 0x210f
	.2byte 0x2000
	.2byte 0xf000
	.2byte 0xff82
	.2byte 0x2000
	.2byte 0xf000
	.2byte 0xff4b
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xff18
	.2byte 0xf000
	.2byte 0xff3e
	.2byte 0xf000
	.2byte 0xfefc
	.2byte 0x2001
	.2byte 0xf000
	.2byte 0xfec1
	.2byte 0x4b17
	.2byte 0x24e0
	.2byte 0x681b
	.2byte 0x0064
	.2byte 0x2280
	.2byte 0x191b
	.2byte 0x0052
	.2byte 0x601a
	.2byte 0xf000
	.2byte 0xffab
	.2byte 0xf000
	.2byte 0xffb1
	.2byte 0x2078
	.2byte 0xf000
	.2byte 0xff26
	.2byte 0x2032
	.2byte 0xf000
	.2byte 0xff8b
	.2byte 0xf000
	.2byte 0xff29
	.2byte 0xe041
	.2byte 0x4816
	.2byte 0xf000
	.2byte 0xff11
	.2byte 0x2800
	.2byte 0xd12a
	.2byte 0xf000
	.2byte 0xf847
	.2byte 0xe039
	.4byte 0x04000050
	.4byte 0x00001010
	.4byte 0x02001000
	.4byte 0x00000111
	.4byte 0x02000240
	.4byte 0x00000242
	.4byte 0x00000039
	.4byte 0x00000875
	.4byte 0x02008d99
	.4byte 0x03001ebc
	.4byte 0x02009368
	.4byte 0x00000872
	.4byte 0xffff0000
	.2byte 0x0251
	.2byte 0x0000
	.2byte 0x0001
	.2byte 0x0001
	.2byte 0x0101
	.2byte 0x0000
	.2byte 0xe666
	.2byte 0x0000
	.2byte 0x0003
	.2byte 0x0001
	.2byte 0x1632
	.2byte 0x0000
	.2byte 0x0109
	.2byte 0x0000
	.2byte 0x2307
	.2byte 0x2209
	.2byte 0x9300
	.2byte 0x9201
	.2byte 0x2000
	.2byte 0x2100
	.2byte 0x2203
	.2byte 0x2303
	.2byte 0xf000
	.2byte 0xfeba
	.2byte 0xe006
	.2byte 0x4808
	.2byte 0xf000
	.2byte 0xfed6
	.2byte 0x2800
	.2byte 0xd101
	.2byte 0xf000
	.2byte 0xf92a
.L_020012e0_3:
	movs r0, #0
	sub sp, #-68
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.2byte 0x0109
	.2byte 0x0000
	.section .text.x02009a3c,"ax",%progbits
	.p2align 2
	.global Func_02001a3c
	.thumb_func
Func_02001a3c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r1
	mov r9, r0
	movs r1, #4
	movs r0, #35
	sub sp, #4
	bl 0x0200a42c
	mov r2, r8
	str r2, [r0]
	ldr r0, [pc, #304]
	bl 0x0200a4bc
	adds r3, r0, #0
	cmp r3, #0
	bne .L_02001a3c_0
	mov r0, sp
	str r3, [r0]
	mov r1, r8
	ldr r3, [pc, #292]
	ldr r2, [pc, #292]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r3, r9
	mov r0, r8
	str r3, [r0, #4]
	b .L_02001a3c_1
.L_02001a3c_0:
	ldr r3, [pc, #284]
	movs r2, #250
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r0, [r3]
	bl 0x0200a5ec
	adds r7, r0, #0
	ldr r4, [r7, #16]
	adds r3, r4, #0
	cmp r4, #0
	bge .L_02001a3c_2
	ldr r0, [pc, #264]
	adds r3, r4, r0
.L_02001a3c_2:
	ldr r1, [r7, #8]
	asrs r3, r3, #20
	lsls r2, r3, #7
	adds r3, r1, #0
	cmp r1, #0
	bge .L_02001a3c_3
	ldr r0, [pc, #248]
	adds r3, r1, r0
.L_02001a3c_3:
	asrs r3, r3, #20
	adds r3, r2, r3
	ldr r2, [pc, #244]
	lsls r3, r3, #2
	mov r0, r8
	adds r2, r2, r3
	ldr r3, [r0]
	mov r10, r2
	cmp r3, #0
	beq .L_02001a3c_4
	ldr r3, [r0, #20]
	cmp r3, #0
	beq .L_02001a3c_4
	ldr r2, [r7, #12]
	movs r3, #192
	lsls r3, r3, #13
	adds r2, r2, r3
	movs r0, #26
	adds r3, r4, #0
	bl 0x0200a44c
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02001a3c_5
	ldr r3, [r7, #20]
	ldr r1, [pc, #200]
	str r3, [r5, #20]
	ldr r6, [r5, #80]
	bl 0x0200a444
	adds r2, r5, #0
	movs r3, #4
	adds r2, #85
	str r7, [r5, #104]
	strb r3, [r2]
	ldr r0, [pc, #184]
	ldr r3, [r5, #12]
	adds r3, r3, r0
	str r3, [r5, #12]
	cmp r6, #0
	beq .L_02001a3c_6
	mov r2, r8
	ldr r3, [r2]
	movs r1, #6
	subs r1, r1, r3
	adds r0, r6, #0
	bl 0x0200a434
	adds r2, r6, #0
	adds r2, #38
	movs r3, #0
	strb r3, [r2]
	ldrb r2, [r6, #9]
	subs r3, #13
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r6, #9]
.L_02001a3c_6:
	mov r3, r8
	str r5, [r3, #20]
	b .L_02001a3c_5
.L_02001a3c_4:
	movs r3, #0
	mov r0, r8
	str r3, [r0, #20]
.L_02001a3c_5:
	mov r2, r10
	ldrb r3, [r2, #2]
	cmp r3, r9
	bne .L_02001a3c_7
	mov r0, r8
	ldr r3, [r0, #24]
	cmp r3, #0
	beq .L_02001a3c_7
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	movs r0, #26
	bl 0x0200a44c
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02001a3c_1
	ldr r3, [r7, #20]
	ldr r1, [pc, #96]
	str r3, [r5, #20]
	ldr r6, [r5, #80]
	bl 0x0200a444
	adds r3, r5, #0
	movs r2, #0
	adds r3, #85
	strb r2, [r3]
	adds r3, #15
	strh r2, [r3]
	adds r2, r5, #0
	movs r3, #2
	adds r2, #35
	strb r3, [r2]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r5, #48]
	cmp r6, #0
	beq .L_02001a3c_8
	adds r0, r6, #0
	movs r1, #6
	bl 0x0200a434
	adds r2, r6, #0
	ldr r3, [pc, #8]
	adds r2, #38
	strb r3, [r2]
.L_02001a3c_8:
	mov r2, r8
	str r5, [r2, #24]
	b .L_02001a3c_1
	.4byte 0x00000000
	.4byte 0x00000109
	.4byte 0x040000d4
	.4byte 0x85000007
	.4byte 0x02000240
	.4byte 0x000fffff
	.4byte 0x02010000
	.4byte 0x0200a7e8
	.4byte 0xffff8000
	.4byte 0x0200a7d0
.L_02001a3c_7:
	movs r3, #0
	mov r0, r8
	str r3, [r0, #24]
.L_02001a3c_1:
	sub sp, #-4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.section .text.x02009cb4,"ax",%progbits
	.p2align 2
	.global Func_02001cb4
	.thumb_func
Func_02001cb4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #128
	adds r6, r0, #0
	lsls r3, r3, #10
	str r3, [r6, #48]
	movs r3, #128
	lsls r3, r3, #9
	ldr r2, [r6, #104]
	str r3, [r6, #52]
	ldr r3, [r2, #8]
	mov r11, r3
	movs r3, #128
	ldr r2, [r2, #16]
	lsls r3, r3, #24
	str r3, [r6, #56]
	str r3, [r6, #60]
	str r3, [r6, #64]
	ldr r3, [r6, #8]
	mov r9, r2
	mov r2, r11
	subs r0, r2, r3
	sub sp, #4
	cmp r0, #0
	bge .L_02001cb4_0
	ldr r3, [pc, #240]
	adds r0, r0, r3
.L_02001cb4_0:
	ldr r3, [r6, #16]
	asrs r0, r0, #16
	mov r2, r9
	mov r10, r0
	subs r0, r2, r3
	cmp r0, #0
	bge .L_02001cb4_1
	ldr r3, [pc, #224]
	adds r0, r0, r3
.L_02001cb4_1:
	asrs r0, r0, #16
	mov r8, r0
	mov r2, r10
	mov r0, r10
	muls r0, r2
	mov r2, r8
	mov r3, r8
	muls r3, r2
	adds r0, r0, r3
	ldr r3, [pc, #204]
	bl 0x0200a628
	ldr r3, [r6, #8]
	mov r2, r11
	subs r2, r2, r3
	ldr r3, [r6, #16]
	mov r10, r2
	mov r2, r9
	subs r2, r2, r3
	movs r3, #128
	lsls r7, r0, #16
	lsls r3, r3, #15
	mov r8, r2
	cmp r7, r3
	bge .L_02001cb4_2
	ldr r4, [pc, #176]
	mov r0, r10
	mov r1, r10
	movs r0, r0
	mov r12, pc
	bx r4
	.2byte 0x1c03
	.2byte 0x4641
	.2byte 0x4640
	.2byte 0x0000
	.2byte 0x46fc
	.2byte 0x4720
	.2byte 0x181b
	.2byte 0x1c18
	.2byte 0xf000
	.2byte 0xfb5a
	.2byte 0x1c07
.L_02001cb4_2:
	adds r1, r7, #0
	cmp r7, #0
	bge .L_02001cb4_3
	adds r1, r7, #7
.L_02001cb4_3:
	ldr r3, [r6, #48]
	asrs r5, r1, #3
	cmp r5, r3
	ble .L_02001cb4_4
	adds r5, r3, #0
.L_02001cb4_4:
	movs r2, #128
	lsls r2, r2, #7
	cmp r7, r2
	bge .L_02001cb4_5
	mov r3, r11
	mov r2, r9
	str r3, [r6, #8]
	str r2, [r6, #16]
	b .L_02001cb4_6
.L_02001cb4_5:
	cmp r7, r5
	ble .L_02001cb4_7
	ldr r3, [pc, #108]
	mov r1, r10
	mov r9, r3
	adds r0, r7, #0
	bl 0x0200a640
	ldr r3, [pc, #92]
	adds r1, r5, #0
	movs r0, r0
	mov r12, pc
	bx r3
	.2byte 0x4641
	.2byte 0x9300
	.2byte 0x4682
	.2byte 0x1c38
	.2byte 0xf000
	.2byte 0xfc4e
	.2byte 0x1c29
	.2byte 0x9b00
	.2byte 0x46fc
	.2byte 0x4718
	.2byte 0x4680
.L_02001cb4_7:
	ldr r3, [r6, #8]
	add r3, r10
	str r3, [r6, #8]
	ldr r3, [r6, #16]
	add r3, r8
	str r3, [r6, #16]
.L_02001cb4_6:
	ldr r3, [pc, #56]
	ldr r2, [r3]
	movs r0, #1
	ldr r1, [r6, #80]
	lsrs r2, r2, #1
	ands r2, r0
	ldr r4, [r1, #40]
	lsls r3, r2, #3
	subs r3, r3, r2
	adds r1, #37
	strb r3, [r4, #5]
	strb r0, [r1]
	sub sp, #-4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.4byte 0x0000ffff
	.4byte 0x030001d8
	.4byte 0x03000118
	.4byte 0x0300013c
	.4byte 0x03001e40
	.section .text.x0200a030,"ax",%progbits
	.p2align 2
	.global Makyuri_RunActorMove
	.thumb_func
Makyuri_RunActorMove:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #440]
	ldr r2, [r3]
	subs r3, #32
	ldr r3, [r3]
	sub sp, #28
	ldr r2, [r2]
	movs r0, #250
	str r3, [sp, #12]
	ldr r3, [pc, #428]
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r3, [r3]
	ldr r1, [sp, #12]
	lsls r3, r3, #2
	adds r3, #20
	ldr r7, [r1, r3]
	mov r8, r2
	adds r2, r7, #0
	adds r2, #85
	str r2, [sp, #0]
	ldrb r3, [r2]
	str r3, [sp, #4]
	ldr r3, [pc, #404]
	ldr r3, [r3]
	movs r2, #15
	lsrs r3, r3, #4
	ldr r1, [pc, #400]
	ands r3, r2
	lsls r3, r3, #1
	ldrh r6, [r1, r3]
	ldrsh r3, [r1, r3]
	movs r1, #1
	negs r1, r1
	cmp r3, r1
	bne .L_02002030_0
	b .L_02002030_1
.L_02002030_0:
	movs r2, #16
	ldr r4, [r7, #8]
	ldr r1, [pc, #380]
	add r2, sp
	mov r11, r2
	movs r2, #128
	ands r4, r1
	lsls r2, r2, #12
	adds r5, r4, r2
	mov r3, r11
	str r5, [r3]
	ldr r3, [r7, #20]
	mov r0, r11
	str r3, [r0, #4]
	ldr r0, [r7, #16]
	ands r0, r1
	adds r2, r0, r2
	mov r1, r11
	str r2, [r1, #8]
	cmp r2, #0
	bge .L_02002030_2
	ldr r3, [pc, #344]
	adds r2, r0, r3
.L_02002030_2:
	asrs r3, r2, #20
	lsls r2, r3, #7
	adds r3, r5, #0
	cmp r3, #0
	bge .L_02002030_3
	ldr r0, [pc, #332]
	adds r3, r4, r0
.L_02002030_3:
	asrs r3, r3, #20
	adds r3, r2, r3
	ldr r1, [pc, #328]
	lsls r3, r3, #2
	movs r0, #128
	adds r5, r3, r1
	mov r2, r11
	lsls r0, r0, #14
	adds r1, r6, #0
	bl 0x0200a424
	mov r2, r11
	ldr r3, [r2, #8]
	cmp r3, #0
	bge .L_02002030_4
	ldr r0, [pc, #304]
	adds r3, r3, r0
.L_02002030_4:
	asrs r3, r3, #20
	mov r1, r11
	lsls r2, r3, #7
	ldr r3, [r1]
	cmp r3, #0
	bge .L_02002030_5
	ldr r0, [pc, #288]
	adds r3, r3, r0
.L_02002030_5:
	asrs r3, r3, #20
	adds r3, r2, r3
	ldr r1, [pc, #276]
	lsls r3, r3, #2
	adds r1, r3, r1
	str r1, [sp, #8]
	mov r2, r8
	ldrb r3, [r5, #2]
	ldr r1, [r2, #4]
	cmp r3, r1
	beq .L_02002030_6
	ldr r0, [sp, #8]
	ldrb r3, [r0, #2]
	cmp r3, r1
	bne .L_02002030_6
	ldr r3, [r2]
	cmp r3, #0
	bne .L_02002030_6
	b .L_02002030_1
.L_02002030_6:
	bl 0x0200a4dc
	adds r0, r7, #0
	add r1, sp, #16
	bl 0x0200a484
	mov r10, r0
	cmp r0, #0
	beq .L_02002030_7
	b .L_02002030_1
.L_02002030_7:
	mov r1, r8
	ldr r5, [r1, #24]
	cmp r5, #0
	beq .L_02002030_8
	adds r3, r5, #0
	adds r3, #100
	mov r2, r10
	strh r2, [r3]
	ldr r1, [pc, #216]
	adds r0, r5, #0
	bl 0x0200a444
	adds r0, r5, #0
	movs r1, #7
	bl 0x0200a43c
	mov r3, r10
	mov r0, r8
	str r3, [r0, #24]
.L_02002030_8:
	ldr r1, [sp, #8]
	mov r0, r8
	ldrb r2, [r1, #2]
	ldr r3, [r0, #4]
	cmp r2, r3
	bne .L_02002030_9
	ldr r3, [r0]
	cmp r3, #0
	beq .L_02002030_9
	ldr r6, [r0, #20]
	movs r0, #26
	ldr r1, [r6, #8]
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	bl 0x0200a44c
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02002030_10
	ldr r1, [r5, #80]
	ldr r3, [r6, #20]
	mov r9, r1
	str r3, [r5, #20]
	ldr r1, [pc, #152]
	bl 0x0200a444
	adds r3, r5, #0
	adds r3, #85
	mov r2, r10
	strb r2, [r3]
	mov r0, r10
	adds r3, #15
	adds r2, r5, #0
	strh r0, [r3]
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #52]
	mov r2, r11
	mov r0, r11
	ldr r1, [r2]
	ldr r3, [r0, #8]
	ldr r2, [r2, #4]
	adds r0, r5, #0
	bl 0x0200a464
	mov r1, r9
	cmp r1, #0
	beq .L_02002030_11
	mov r0, r9
	movs r1, #6
	bl 0x0200a434
	mov r2, r9
	ldr r3, [pc, #40]
	adds r2, #38
	strb r3, [r2]
.L_02002030_11:
	mov r2, r8
	str r5, [r2, #24]
.L_02002030_10:
	mov r0, r8
	ldr r3, [r0]
	subs r5, r3, #1
	str r5, [r0]
	cmp r5, #0
	bne .L_02002030_12
	ldr r0, [r0, #20]
	bl 0x0200a454
	mov r1, r8
	str r5, [r1, #20]
	ldr r0, [pc, #52]
	bl 0x0200a4cc
	b .L_02002030_9
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x03001edc
	.4byte 0x02000240
	.4byte 0x03001ae8
	.4byte 0x0200a7f4
	.4byte 0xfff00000
	.4byte 0x0017ffff
	.4byte 0x02010000
	.4byte 0x000fffff
	.4byte 0x0200a7dc
	.4byte 0x0200a7d0
	.4byte 0x00000161
.L_02002030_12:
	mov r2, r8
	ldr r0, [r2, #20]
	cmp r0, #0
	beq .L_02002030_9
	movs r1, #6
	subs r1, r1, r5
	bl 0x0200a43c
.L_02002030_9:
	movs r1, #6
	adds r0, r7, #0
	bl 0x0200a43c
	movs r0, #3
	bl 0x0200a3ec
	movs r0, #152
	bl 0x0200a614
	adds r0, r7, #0
	movs r1, #7
	bl 0x0200a43c
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r7, #40]
	ldr r3, [sp, #0]
	ldrb r2, [r3]
	ldr r0, [sp, #0]
	movs r3, #126
	ands r3, r2
	strb r3, [r0]
	movs r1, #0
	adds r0, r7, #0
	bl 0x0200a48c
	mov r3, r11
	movs r2, #2
	ldrsh r1, [r3, r2]
	movs r0, #10
	ldrsh r2, [r3, r0]
	movs r0, #0
	bl 0x0200a504
	movs r1, #6
	adds r0, r7, #0
	bl 0x0200a43c
	movs r0, #2
	bl 0x0200a3ec
	ldr r1, [sp, #8]
	mov r0, r8
	ldrb r2, [r1, #2]
	ldr r3, [r0, #4]
	cmp r2, r3
	beq .L_02002030_13
	adds r0, r7, #0
	movs r1, #1
	bl 0x0200a48c
	b .L_02002030_14
.L_02002030_13:
	movs r0, #215
	bl 0x0200a614
.L_02002030_14:
	movs r0, #1
	bl 0x0200a3ec
	add r1, sp, #4
	ldr r2, [sp, #0]
	ldrb r1, [r1]
	strb r1, [r2]
	ldr r3, [sp, #8]
	mov r0, r8
	ldrb r2, [r3, #2]
	ldr r3, [r0, #4]
	cmp r2, r3
	bne .L_02002030_15
	ldr r3, [r0, #24]
	cmp r3, #0
	bne .L_02002030_15
	movs r1, #18
	adds r0, r7, #0
	bl 0x0200a43c
	movs r0, #241
	bl 0x0200a614
	movs r1, #15
	ldr r6, [pc, #132]
	movs r5, #0
	mov r10, r1
	b .L_02002030_16
.L_02002030_18:
	movs r0, #1
	bl 0x0200a3ec
	adds r5, #1
.L_02002030_16:
	adds r3, r5, #0
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	bne .L_02002030_17
	adds r0, r7, #0
	bl 0x02009bc8
.L_02002030_17:
	cmp r5, #31
	ble .L_02002030_18
	ldr r3, [r6]
	cmp r3, #0
	beq .L_02002030_18
	movs r0, #144
	lsls r0, r0, #1
	bl 0x0200a614
	movs r0, #1
	bl 0x0200a3ec
	mov r0, r8
	ldr r3, [r0, #12]
	str r3, [r7, #8]
	ldr r3, [r0, #16]
	movs r1, #1
	str r3, [r7, #16]
	adds r0, r7, #0
	bl 0x0200a48c
.L_02002030_15:
	mov r1, r8
	movs r3, #0
	str r3, [r1, #8]
	bl 0x0200a4e4
	movs r0, #216
	ldr r2, [sp, #12]
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r1, #128
	ldr r4, [pc, #44]
	ldr r0, [r3]
	lsls r1, r1, #14
	mov r12, pc
	bx r4
	.2byte 0x9903
	.2byte 0x23da
	.2byte 0x005b
	.2byte 0x18ca
	.2byte 0x6813
	.2byte 0x181b
	.2byte 0x6013
.L_02002030_1:
	sub sp, #-28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x03001c94
	.4byte 0x03000118
	.section .rodata.part1,"a",%progbits
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.global StagedActor_DirectionSteps
StagedActor_DirectionSteps:
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
	.global StagedActor_FootprintKinds
StagedActor_FootprintKinds:
	.4byte 0x000000cf
	.4byte 0x000000cd
	.4byte 0x000000e4
	.4byte 0x000000e5
	.4byte 0x0000012a
	.4byte 0x00000129
	.global StagedActor_FootprintBounds
StagedActor_FootprintBounds:
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
	.global MakyuriIriguchi_RiseScript
MakyuriIriguchi_RiseScript:
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000001b
	.global MakyuriHeya_SparkScript
MakyuriHeya_SparkScript:
	.4byte 0x00000022
	.4byte 0x02009c21
	.4byte 0x0000001b
	.4byte 0x00000022
	.4byte 0x02009c61
	.4byte 0x00000010
	.global Makyuri_StartMoveScript
Makyuri_StartMoveScript:
	.4byte 0x00000022
	.4byte 0x02009c89
	.4byte 0x0000001b
	.global Makyuri_PillarScript
Makyuri_PillarScript:
	.4byte 0x00000022
	.4byte 0x02009cb5
	.4byte 0x00000010
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0xffffc000
	.4byte 0xc000ffff
	.4byte 0xffff4000
	.4byte 0x4000ffff
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.global gEffectScripts
gEffectScripts:
	.4byte 0x0200a658
	.4byte 0x0200a690
	.4byte 0x0200a6c8
	.global MakyuriIriguchi_Actor8Path1
MakyuriIriguchi_Actor8Path1:
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00020000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00020000
	.4byte 0x00000015
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00000022
	.4byte 0x02008d49
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global MakyuriIriguchi_Actor8Path2
MakyuriIriguchi_Actor8Path2:
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00020000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00020000
	.4byte 0x00000015
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00000022
	.4byte 0x02008d49
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global MakyuriIriguchi_Actor8Path3
MakyuriIriguchi_Actor8Path3:
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00020000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00020000
	.4byte 0x00000015
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00000022
	.4byte 0x02008d49
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global MakyuriIriguchi_SceneTableA
MakyuriIriguchi_SceneTableA:
	.4byte 0xffff0000
	.4byte 0x000000d1
	.4byte 0x40000117
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000088
	.4byte 0xc0000118
	.4byte 0x00100000
	.4byte 0x01000020
	.4byte 0x00000140
	.4byte 0xffff0002
	.4byte 0x00000088
	.4byte 0x40000078
	.4byte 0x00100000
	.4byte 0x01000020
	.4byte 0x00000140
	.4byte 0xffff0003
	.4byte 0x000000c8
	.4byte 0xc0000288
	.4byte 0x00500000
	.4byte 0x014001b0
	.4byte 0x000002b0
	.4byte 0xffff0004
	.4byte 0x00000078
	.4byte 0x40000208
	.4byte 0x00500000
	.4byte 0x014001b0
	.4byte 0x000002b0
	.4byte 0xffff0005
	.4byte 0x00000118
	.4byte 0x40000208
	.4byte 0x00500000
	.4byte 0x014001b0
	.4byte 0x000002b0
	.4byte 0xffff0006
	.4byte 0x000000c8
	.4byte 0x40000228
	.4byte 0x00500000
	.4byte 0x014001b0
	.4byte 0x000002b0
	.4byte 0xffff0007
	.4byte 0x00000168
	.4byte 0xc00000b8
	.4byte 0x01300000
	.4byte 0x03200020
	.4byte 0x000000d0
	.4byte 0xffff0008
	.4byte 0x000001c8
	.4byte 0xc00000b8
	.4byte 0x01300000
	.4byte 0x03200020
	.4byte 0x000000d0
	.4byte 0xffff0009
	.4byte 0x00000268
	.4byte 0xc00000b8
	.4byte 0x01300000
	.4byte 0x03200020
	.4byte 0x000000d0
	.4byte 0xffff000a
	.4byte 0x000002f8
	.4byte 0xc00000b8
	.4byte 0x01300000
	.4byte 0x03200020
	.4byte 0x000000d0
	.4byte 0xffff000b
	.4byte 0x00000158
	.4byte 0x40000058
	.4byte 0x01300000
	.4byte 0x03200020
	.4byte 0x000000d0
	.4byte 0xffff000c
	.4byte 0x00000218
	.4byte 0x40000058
	.4byte 0x01300000
	.4byte 0x03200020
	.4byte 0x000000d0
	.4byte 0x0876000d
	.4byte 0x000002e8
	.4byte 0xc00002a8
	.4byte 0x02200000
	.4byte 0x03100200
	.4byte 0x000002d0
	.4byte 0x0876000e
	.4byte 0x00000248
	.4byte 0x40000238
	.4byte 0x02200000
	.4byte 0x03100200
	.4byte 0x000002d0
	.4byte 0x0876000f
	.4byte 0x000002e8
	.4byte 0x40000238
	.4byte 0x02200000
	.4byte 0x03100200
	.4byte 0x000002d0
	.4byte 0xffff000d
	.4byte 0x00000248
	.4byte 0xc00001a8
	.4byte 0x01800000
	.4byte 0x02700100
	.4byte 0x000001c0
	.4byte 0xffff000e
	.4byte 0x000001a8
	.4byte 0x40000138
	.4byte 0x01800000
	.4byte 0x02700100
	.4byte 0x000001c0
	.4byte 0xffff000f
	.4byte 0x00000248
	.4byte 0x40000138
	.4byte 0x01800000
	.4byte 0x02700100
	.4byte 0x000001c0
	.4byte 0xffff0010
	.4byte 0x000002e8
	.4byte 0xc00001a8
	.4byte 0x02c00000
	.4byte 0x03b00100
	.4byte 0x000001c0
	.4byte 0xffff0011
	.4byte 0x00000158
	.4byte 0x40000368
	.4byte 0x00300000
	.4byte 0x01900300
	.4byte 0x000003c0
	.4byte 0xffff0012
	.4byte 0x000000e8
	.4byte 0x40000368
	.4byte 0x00300000
	.4byte 0x01900300
	.4byte 0x000003c0
	.4byte 0xffff0013
	.4byte 0x00000238
	.4byte 0x40000368
	.4byte 0x01d00000
	.4byte 0x03300300
	.4byte 0x000003c0
	.4byte 0xffff0014
	.4byte 0x000002b8
	.4byte 0x40000368
	.4byte 0x01d00000
	.4byte 0x03300300
	.4byte 0x000003c0
	.4byte 0xffff0019
	.4byte 0x000002e8
	.4byte 0x40000258
	.4byte 0x02200000
	.4byte 0x03100200
	.4byte 0x000002d0
	.4byte 0xffff001e
	.4byte 0x00000088
	.4byte 0xc0000128
	.4byte 0x00100000
	.4byte 0x01000020
	.4byte 0x00000140
	.4byte 0xffff001f
	.4byte 0x00000088
	.4byte 0xc00000a8
	.4byte 0x00100000
	.4byte 0x01000020
	.4byte 0x00000140
	.4byte 0xffff003c
	.4byte 0x00000088
	.4byte 0x40000098
	.4byte 0x00100000
	.4byte 0x01000020
	.4byte 0x00000140
	.4byte 0xffff0063
	.4byte 0x00000088
	.4byte 0x000000e8
	.4byte 0x00100000
	.4byte 0x01000020
	.4byte 0x00000140
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global MakyuriIriguchi_SceneTableB
MakyuriIriguchi_SceneTableB:
	.4byte 0x00000035
	.4byte 0x0010c039
	.4byte 0x00203035
	.4byte 0x00302035
	.4byte 0x00409035
	.4byte 0x00508035
	.4byte 0x00611035
	.4byte 0x00704035
	.4byte 0x00805035
	.4byte 0x00913035
	.4byte 0x00a01036
	.4byte 0x00b02036
	.4byte 0x00c07036
	.4byte 0x00d04036
	.4byte 0x00e12035
	.4byte 0x00f14035
	.4byte 0x01007035
	.4byte 0x0110d035
	.4byte 0x0120a035
	.4byte 0x01310035
	.4byte 0x0141e035
	.4byte 0x0320f036
	.4byte 0x03f1f035
	.4byte 0x000001ff
	.global MakyuriIriguchi_SceneTableC
MakyuriIriguchi_SceneTableC:
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff00c0
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff00e0
	.4byte 0x00000007
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00024000
	.4byte 0xffff00e0
	.4byte 0x00000007
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0xffff00e0
	.4byte 0x00000007
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff00e0
	.4byte 0x00000007
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x01024000
	.4byte 0xffff00e0
	.4byte 0x00000007
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0xffff00e0
	.4byte 0x00000007
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global MakyuriIriguchi_SceneTableD
MakyuriIriguchi_SceneTableD:
	.4byte 0x00000202
	.4byte 0x1875001f
	.4byte 0x02008e79
	.4byte 0x00000002
	.4byte 0x1875001f
	.4byte 0x02008e6d
	.4byte 0x00000a02
	.4byte 0x18750015
	.4byte 0x02008e79
	.4byte 0x00000002
	.4byte 0x18750015
	.4byte 0x02008e61
	.4byte 0x00000202
	.4byte 0x1875000b
	.4byte 0x02008e79
	.4byte 0x00000002
	.4byte 0x1875000b
	.4byte 0x02008e85
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x02009145
	.4byte 0x00000002
	.4byte 0x12050028
	.4byte 0x02009151
	.4byte 0x00000002
	.4byte 0x1205002a
	.4byte 0x02009151
	.4byte 0x00000002
	.4byte 0x02050029
	.4byte 0x0200916d
	.4byte 0x0000c602
	.4byte 0xffff0028
	.4byte 0x020091b5
	.4byte 0x0000c602
	.4byte 0xffff0029
	.4byte 0x020091b5
	.4byte 0x0000c602
	.4byte 0xffff0043
	.4byte 0x02008fad
	.4byte 0x0000c602
	.4byte 0xffff0044
	.4byte 0x02008fb9
	.4byte 0x0000c602
	.4byte 0xffff0045
	.4byte 0x02008fc5
	.4byte 0x00000001
	.4byte 0xffff0033
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0034
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0035
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0036
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0037
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0038
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0039
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff003a
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff003b
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff003c
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff003d
	.4byte 0x0000000b
	.4byte 0x00000021
	.4byte 0xffff003e
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff003f
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff0040
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff0041
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0042
	.4byte 0x00000010
	.4byte 0x00000000
	.4byte 0xffff0003
	.4byte 0x0000158d
	.4byte 0x00008d15
	.4byte 0xffff0003
	.4byte 0x0000158e
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x020091b5
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x020091b5
	.4byte 0x00000003
	.4byte 0xffff0001
	.4byte 0x02008ead
	.4byte 0x00005d15
	.4byte 0x08750010
	.4byte 0x02008fd9
	.4byte 0x10009585
	.4byte 0xffff0000
	.4byte 0x0200a3b9
	.4byte 0x00000013
	.4byte 0x0f650064
	.4byte 0x0010004c
	.4byte 0x00000013
	.4byte 0x0ef00065
	.4byte 0x00500000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global MakyuriIriguchi_SparkBurstScript
MakyuriIriguchi_SparkBurstScript:
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000a000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00004000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000a000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x0000000c
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
