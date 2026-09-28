.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA_SAI/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	push {r5, lr}
	movs r0, #0
	bl 0x02008664
	ldr r5, [r0, #8]
	movs r0, #0
	bl 0x02008664
	asrs r5, r5, #20
	ldr r3, [r0, #16]
	subs r5, #34
	asrs r3, r3, #20
	cmp r5, #1
	bhi .L_02000030_0
	cmp r3, #40
	ble .L_02000030_0
	cmp r3, #42
	bgt .L_02000030_0
	movs r0, #148
	lsls r0, r0, #2
	bl 0x0200861c
	b .L_02000030_1
.L_02000030_0:
	movs r0, #148
	lsls r0, r0, #2
	bl 0x02008624
.L_02000030_1:
	pop {r5}
	pop {r0}
	bx r0
	.global Func_0200006c
	.thumb_func
Func_0200006c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x020086dc
	.global Func_02000074
	.thumb_func
Func_02000074:
	movs r0, #0
	bx lr
	.global Func_02000078
	.thumb_func
Func_02000078:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x020087cc
	.global Func_02000080
	.thumb_func
Func_02000080:
	push {r5, lr}
	ldr r5, [pc, #16]
	adds r0, r5, #0
	bl 0x0200864c
	adds r0, r5, #0
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x020087f4
	.global Func_02000098
	.thumb_func
Func_02000098:
	push {r5, lr}
	adds r5, r0, #0
	bl 0x0200863c
	adds r0, r5, #0
	movs r1, #1
	bl 0x02008674
	adds r0, r5, #0
	movs r1, #0
	bl 0x020086a4
	bl 0x02008644
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020000bc
	.thumb_func
Func_020000bc:
	push {lr}
	ldr r0, [pc, #24]
	bl 0x02008694
	movs r0, #9
	movs r1, #0
	movs r2, #2
	bl 0x0200868c
	movs r0, #9
	bl 0x02008098
	pop {r0}
	bx r0
	.4byte 0x00001cc9
	.global Func_020000dc
	.thumb_func
Func_020000dc:
	push {lr}
	ldr r0, [pc, #24]
	bl 0x02008694
	movs r0, #11
	movs r1, #0
	movs r2, #2
	bl 0x0200868c
	movs r0, #11
	bl 0x02008098
	pop {r0}
	bx r0
	.4byte 0x00001ccd
	.global Func_020000fc
	.thumb_func
Func_020000fc:
	push {lr}
	ldr r0, [pc, #24]
	bl 0x02008694
	movs r0, #12
	movs r1, #0
	movs r2, #2
	bl 0x0200868c
	movs r0, #12
	bl 0x02008098
	pop {r0}
	bx r0
	.4byte 0x00001cd0
	.global Func_0200011c
	.thumb_func
Func_0200011c:
	push {lr}
	bl 0x0200863c
	ldr r0, [pc, #148]
	bl 0x02008694
	movs r2, #2
	movs r0, #16
	movs r1, #0
	bl 0x0200868c
	movs r0, #16
	movs r1, #1
	bl 0x02008674
	movs r2, #20
	movs r0, #16
	movs r1, #0
	bl 0x020086ac
	movs r1, #4
	movs r0, #16
	bl 0x0200867c
	movs r0, #20
	bl 0x02008634
	movs r0, #16
	movs r1, #0
	movs r2, #20
	bl 0x020086ac
	movs r1, #129
	movs r0, #16
	lsls r1, r1, #1
	movs r2, #60
	bl 0x020086bc
	movs r0, #16
.L_0200016a:
	movs r1, #0
	movs r2, #30
	bl 0x020086ac
	movs r1, #0
	movs r0, #16
	bl 0x0200869c
	movs r0, #0
	movs r1, #0
	bl 0x0200865c
	cmp r0, #0
	beq 0x02008196
	ldr r3, [pc, #52]
	ldr r2, [r3]
.L_0200018a:
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #20
	movs r0, #16
	bl 0x020086ac
	movs r0, #192
	lsls r0, r0, #2
	bl 0x0200861c
	ldr r0, [pc, #20]
.L_020001aa:
	bl 0x0200861c
	bl 0x02008644
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x1cd4
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x0868
	.2byte 0x0000
	.global Func_020001c4
	.thumb_func
Func_020001c4:
	push {lr}
	ldr r0, [pc, #24]
	bl 0x02008694
	movs r0, #16
	movs r1, #0
	movs r2, #2
	bl 0x0200868c
	movs r0, #16
	bl 0x02008098
	pop {r0}
	bx r0
	.4byte 0x00001cda
	.global Func_020001e4
	.thumb_func
Func_020001e4:
	push {lr}
	ldr r0, [pc, #24]
	bl 0x02008694
	movs r0, #23
	movs r1, #0
	movs r2, #2
	bl 0x0200868c
	movs r0, #23
	bl 0x02008098
	pop {r0}
	bx r0
	.4byte 0x00001cee
	.global Func_02000204
	.thumb_func
Func_02000204:
	push {lr}
	bl 0x0200863c
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl 0x0200868c
	ldr r0, [pc, #208]
	bl 0x02008614
	cmp r0, #0
	bne .L_02000204_0
	ldr r0, [pc, #204]
	bl 0x02008694
	movs r0, #18
	movs r1, #0
	bl 0x0200869c
	b .L_02000204_1
.L_02000204_0:
	ldr r0, [pc, #192]
	bl 0x02008694
	movs r0, #18
	movs r1, #0
	bl 0x0200869c
.L_02000204_1:
	movs r0, #0
	movs r1, #0
	bl 0x0200865c
	cmp r0, #0
	bne 0x020082a8
	movs r0, #20
	bl 0x02008634
	movs r1, #0
	movs r0, #18
	bl 0x020086a4
	movs r0, #20
	bl 0x02008634
	movs r0, #18
	movs r1, #2
	bl 0x02008684
	movs r0, #20
	bl 0x02008634
	bl 0x0200862c
	cmp r0, #0
	bne 0x02008290
.L_02000272:
	movs r1, #4
	movs r0, #18
	bl 0x0200867c
	movs r0, #20
	bl 0x02008634
	ldr r0, [pc, #112]
	bl 0x02008694
	movs r0, #18
	movs r1, #0
	bl 0x020086a4
	b 0x020082d4
	.2byte 0x20e7
.L_02000292:
	movs r1, #3
	bl 0x020086c4
	movs r0, #231
	movs r1, #0
	bl 0x02008654
	ldr r0, [pc, #68]
	bl 0x0200861c
	b .L_02000292_0
	.2byte 0x4b13
	.2byte 0x681a
	.2byte 0x23ec
	.2byte 0x005b
	.2byte 0x18d2
	.2byte 0x8813
	.2byte 0x3301
	.2byte 0x8013
	.2byte 0x2014
	.2byte 0xf000
	.2byte 0xf9bb
	.2byte 0x2103
	.2byte 0x2012
	.2byte 0xf000
	.2byte 0xf9db
	.2byte 0x2014
	.2byte 0xf000
	.2byte 0xf9b4
	.2byte 0x2012
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xf9e8
.L_02000292_0:
	movs r1, #128
	movs r0, #18
	lsls r1, r1, #7
	movs r2, #0
	bl 0x020086b4
	bl 0x02008644
	pop {r0}
	bx r0
	.4byte 0x0000085b
	.2byte 0x137c
	.2byte 0x0000
	.2byte 0x1385
	.2byte 0x0000
	.2byte 0x1384
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.global Func_020002fc
	.thumb_func
Func_020002fc:
	push {r5, lr}
	bl 0x0200863c
	movs r1, #1
	movs r0, #16
	bl 0x02008684
	bl 0x02008644
	movs r0, #16
	bl 0x02008664
	movs r3, #1
	adds r0, #91
	strb r3, [r0]
	bl 0x0200811c
	movs r0, #16
	bl 0x02008664
	movs r5, #0
	adds r0, #91
	strb r5, [r0]
	movs r1, #2
	movs r0, #16
	bl 0x0200866c
	pop {r5}
	pop {r0}
	bx r0
	.global Func_02000338
	.thumb_func
Func_02000338:
	push {lr}
	bl 0x0200863c
	bl 0x0200862c
	cmp r0, #0
	bne .L_02000338_0
	movs r1, #4
	movs r0, #18
	bl 0x0200867c
	movs r0, #20
	bl 0x02008634
	ldr r0, [pc, #36]
	bl 0x02008694
	movs r0, #18
	movs r1, #0
	bl 0x020086a4
	b .L_02000338_1
.L_02000338_0:
	movs r0, #231
	movs r1, #3
	bl 0x020086c4
	movs r0, #231
	movs r1, #0
	bl 0x02008654
.L_02000338_1:
	bl 0x02008644
	pop {r0}
	bx r0
	.4byte 0x00001384
	.global Func_02000380
	.thumb_func
Func_02000380:
	push {r5, lr}
	movs r0, #0
	bl 0x02008664
	ldrh r5, [r0, #6]
	bl 0x0200863c
	ldr r3, [pc, #44]
	adds r5, r5, r3
	ldr r3, [pc, #44]
	cmp r5, r3
	bhi .L_02000380_0
	movs r0, #4
	movs r1, #19
	bl 0x020086cc
	b .L_02000380_1
.L_02000380_0:
	ldr r0, [pc, #32]
	bl 0x02008694
	movs r0, #19
	movs r1, #0
	bl 0x020086a4
.L_02000380_1:
	bl 0x02008644
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0xffff5fff
	.4byte 0x00003ffe
	.4byte 0x00001ce2
	.global Func_020003c8
	.thumb_func
Func_020003c8:
	push {r5, lr}
	movs r0, #0
	bl 0x02008664
	ldrh r5, [r0, #6]
	bl 0x0200863c
	ldr r3, [pc, #44]
	adds r5, r5, r3
	ldr r3, [pc, #44]
	cmp r5, r3
	bhi .L_020003c8_0
	movs r0, #5
	movs r1, #20
	bl 0x020086cc
	b .L_020003c8_1
.L_020003c8_0:
	ldr r0, [pc, #32]
	bl 0x02008694
	movs r0, #20
	movs r1, #0
	bl 0x020086a4
.L_020003c8_1:
	bl 0x02008644
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0xffff5fff
	.4byte 0x00003ffe
	.4byte 0x00001ce4
	.global Func_02000410
	.thumb_func
Func_02000410:
	push {r5, lr}
	movs r0, #0
	bl 0x02008664
	ldrh r5, [r0, #6]
	bl 0x0200863c
	ldr r3, [pc, #44]
	adds r5, r5, r3
	ldr r3, [pc, #44]
	cmp r5, r3
	bhi .L_02000410_0
	movs r0, #6
	movs r1, #21
	bl 0x020086cc
	b 0x02008440
.L_02000410_0:
	ldr r0, [pc, #32]
	bl 0x02008694
.L_02000438:
	movs r0, #21
	movs r1, #0
	bl 0x020086a4
	bl 0x02008644
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x5fff
	.2byte 0xffff
	.2byte 0x3ffe
	.2byte 0x0000
	.2byte 0x1ce6
	.2byte 0x0000
	.global Func_02000458
	.thumb_func
Func_02000458:
	push {r5, lr}
	movs r0, #0
	bl 0x02008664
	ldrh r5, [r0, #6]
	bl 0x0200863c
	ldr r3, [pc, #44]
	adds r5, r5, r3
	ldr r3, [pc, #44]
	cmp r5, r3
	bhi .L_02000458_0
	movs r0, #1
	movs r1, #22
	bl 0x020086d4
	b .L_02000458_1
.L_02000458_0:
	ldr r0, [pc, #32]
	bl 0x02008694
	movs r0, #22
	movs r1, #0
	bl 0x020086a4
.L_02000458_1:
	bl 0x02008644
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0xffff5fff
	.4byte 0x00003ffe
	.4byte 0x00001cec
	.global Func_020004a0
	.thumb_func
Func_020004a0:
	push {lr}
	bl 0x0200863c
	ldr r0, [pc, #40]
	bl 0x02008614
	cmp r0, #0
	bne .L_020004a0_0
	ldr r0, [pc, #32]
	bl 0x02008694
	b .L_020004a0_1
.L_020004a0_0:
	ldr r0, [pc, #28]
	bl 0x02008694
.L_020004a0_1:
	movs r0, #18
	movs r1, #0
	bl 0x020086a4
	bl 0x02008644
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000085b
	.4byte 0x00001382
	.4byte 0x00001cf4
	.global Func_020004dc
	.thumb_func
Func_020004dc:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200898c
	.global Func_020004e4
	.thumb_func
Func_020004e4:
	push {r5, lr}
	ldr r3, [pc, #124]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #73
	str r2, [r3]
	ldr r3, [pc, #112]
	subs r2, #71
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	sub sp, #8
	cmp r3, #5
	bne .L_020004e4_0
	movs r3, #4
	movs r2, #3
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #120
	movs r2, #8
	movs r3, #67
	movs r0, #0
	bl 0x02008604
	movs r0, #8
	bl 0x02008664
	movs r5, #0
	adds r0, #85
	strb r5, [r0]
	movs r0, #8
	bl 0x02008664
	str r5, [r0, #12]
	movs r0, #8
	bl 0x02008664
	str r5, [r0, #20]
	b .L_020004e4_1
.L_020004e4_0:
	cmp r3, #7
	beq .L_020004e4_2
	cmp r3, #11
	bne .L_020004e4_1
.L_020004e4_2:
	movs r1, #142
	movs r2, #128
	movs r3, #168
	lsls r1, r1, #18
	movs r0, #231
	lsls r2, r2, #13
	lsls r3, r3, #18
	bl 0x02008570
	movs r1, #200
	ldr r0, [pc, #24]
	lsls r1, r1, #4
	bl 0x020085dc
.L_020004e4_1:
	movs r0, #0
	sub sp, #-8
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x02008031
	.global Func_02000570
	.thumb_func
Func_02000570:
	push {r5, r6, r7, lr}
	adds r7, r0, #0
	movs r0, #22
	movs r5, #0
	bl 0x020085fc
	cmp r0, #0
	beq .L_02000570_0
	ldr r6, [r0, #80]
	adds r3, r6, #0
	adds r3, #38
	strb r5, [r3]
	adds r3, #1
	strb r5, [r3]
	movs r3, #33
	ldrb r2, [r6, #5]
	negs r3, r3
	ands r3, r2
	ldrb r2, [r6, #9]
	strb r3, [r6, #5]
	movs r3, #15
	ands r3, r2
	strb r3, [r6, #9]
	adds r3, r0, #0
	adds r3, #85
	adds r2, r0, #0
	strb r5, [r3]
	adds r2, #92
	movs r3, #1
	movs r1, #193
	strb r3, [r2]
	lsls r1, r1, #3
	movs r0, #17
	bl 0x020085e4
	adds r5, r0, #0
	adds r0, r7, #0
	bl 0x0200860c
	movs r3, #128
	lsls r3, r3, #3
	adds r5, r5, r3
	ldrb r0, [r6, #28]
	movs r1, #128
	adds r2, r5, #0
	bl 0x020085f4
	movs r0, #17
	bl 0x020085ec
.L_02000570_0:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.include "games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA_SAI/IMPORT.INC"
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x000000c0
	.4byte 0xc00000dc
	.4byte 0x00200000
	.4byte 0x01100000
	.4byte 0x000000f0
	.4byte 0xffff0006
	.4byte 0x000001b0
	.4byte 0xc00000fe
	.4byte 0x01200000
	.4byte 0x02100000
	.4byte 0x00000110
	.4byte 0xffff0007
	.4byte 0x000002d0
	.4byte 0xc000030e
	.4byte 0x01b00000
	.4byte 0x03200240
	.4byte 0x00000320
	.4byte 0xffff0008
	.4byte 0x00000270
	.4byte 0xc00000ec
	.4byte 0x02300000
	.4byte 0x03500020
	.4byte 0x00000100
	.4byte 0xffff0009
	.4byte 0x00000090
	.4byte 0xc00001ec
	.4byte 0x00100000
	.4byte 0x01000120
	.4byte 0x00000200
	.4byte 0xffff000a
	.4byte 0x000001a0
	.4byte 0xc000020e
	.4byte 0x01400000
	.4byte 0x02500120
	.4byte 0x00000220
	.4byte 0xffff000b
	.4byte 0x000002e8
	.4byte 0x40000280
	.4byte 0x01b00000
	.4byte 0x03200240
	.4byte 0x00000320
	.4byte 0xffff000c
	.4byte 0x00000166
	.4byte 0x40000278
	.4byte 0x00200000
	.4byte 0x01900240
	.4byte 0x00000308
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000018
	.4byte 0x00505017
	.4byte 0x00606017
	.4byte 0x00707017
	.4byte 0x00808017
	.4byte 0x00909017
	.4byte 0x00a0a017
	.4byte 0x00b0b018
	.4byte 0x00c0c018
	.4byte 0x000001ff
	.4byte 0x0000006c
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00013000
	.4byte 0x0000006a
	.4byte 0x00000003
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00008000
	.4byte 0x00000065
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x0000006b
	.4byte 0x00000002
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00004000
	.4byte 0x00000067
	.4byte 0x00000002
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00004000
	.4byte 0x0000006f
	.4byte 0x00000001
	.4byte 0x02f00000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00004000
	.4byte 0x0000006b
	.4byte 0x00000001
	.4byte 0x01f00000
	.4byte 0x00000000
	.4byte 0x01a00000
	.4byte 0x00010000
	.4byte 0x00000066
	.4byte 0x00000001
	.4byte 0x02000000
	.4byte 0x00000000
	.4byte 0x01a00000
	.4byte 0x00018000
	.4byte 0x00000075
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01c30000
	.4byte 0x00004000
	.4byte 0x00000073
	.4byte 0x00000001
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x0000c000
	.4byte 0x0000006a
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00004000
	.4byte 0x0000007c
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00014000
	.4byte 0x0000007d
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00014000
	.4byte 0x00000076
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00014000
	.4byte 0x00000077
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00014000
	.4byte 0x00000080
	.4byte 0x00000002
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001cc8
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x020080bd
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001ccc
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x020080dd
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020080fd
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001cd1
	.4byte 0x00000000
	.4byte 0x03000010
	.4byte 0x0200811d
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x020081c5
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001cdb
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001cdc
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001cdd
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x02008381
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x020083c9
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x02008411
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x02008459
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00001ced
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x020081e5
	.4byte 0x00000000
	.4byte 0x02500012
	.4byte 0x02008205
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001cca
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001ccb
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001cce
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001ccf
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001cd2
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001cd3
	.4byte 0x00008d15
	.4byte 0x03000410
	.4byte 0x020082fd
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001cde
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001cdf
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001ce0
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001ce1
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001ce3
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001ce5
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001ce7
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001cf2
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001cf3
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x020084a1
	.4byte 0x00000003
	.4byte 0xffff0029
	.4byte 0x02008339
	.4byte 0x00000023
	.4byte 0x0f4b0064
	.4byte 0x00200007
	.4byte 0x00000033
	.4byte 0x0f4c0065
	.4byte 0x00200004
	.4byte 0x00000033
	.4byte 0x0f4d0066
	.4byte 0x001000e3
	.4byte 0x000000d3
	.4byte 0x0f4e0067
	.4byte 0x001000c3
	.4byte 0x0000c4f3
	.4byte 0xffff00c8
	.4byte 0x004029d1
	.4byte 0x000000f3
	.4byte 0xffff00c9
	.4byte 0x004029d3
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
