.syntax unified
	.thumb
	.section .text.x02008130,"ax",%progbits
	.global Func_02000130
	.thumb_func
Func_02000130:
	push {lr}
	bl 0x0200a470
	ldr r0, [pc, #20]
	bl 0x0200a538
	movs r1, #0
	movs r0, #10
	bl 0x0200a558
	bl 0x0200a478
	pop {r0}
	bx r0
	.4byte 0x00001420
	.section .text.x020088c0,"ax",%progbits
	.balign 4
	.global FieldScene_RunLongBranchingChoreography
	.thumb_func
FieldScene_RunLongBranchingChoreography:
	push {r5, lr}
	bl 0x0200a470
	movs r0, #1
.L_020008c8:
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl 0x0200a580
	movs r0, #1
	bl 0x0200a408
	bl 0x0200a588
	movs r3, #0
	adds r0, #85
	movs r1, #1
	movs r2, #166
	strb r3, [r0]
	negs r1, r1
	lsls r2, r2, #18
	ldr r0, [pc, #988]
	bl 0x0200a580
	movs r0, #1
	bl 0x0200a408
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200a4f8
	ldr r0, [pc, #972]
	bl 0x0200a450
	cmp r0, #0
	beq .L_020008c8_0
	movs r1, #1
	movs r3, #0
	ldr r0, [pc, #952]
	negs r1, r1
	ldr r2, [pc, #956]
	bl 0x0200a580
	movs r1, #219
	movs r0, #19
	lsls r1, r1, #18
	ldr r2, [pc, #948]
	bl 0x0200a4f8
	movs r1, #192
	movs r0, #19
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200a560
	movs r0, #0
	ldr r1, [pc, #916]
	ldr r2, [pc, #932]
	bl 0x0200a4f8
.L_020008c8_0:
	bl 0x0200a420
	movs r0, #1
	bl 0x0200a408
	ldr r3, [pc, #920]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	subs r3, #192
	str r3, [r2]
	adds r3, #200
	adds r2, r1, r3
	movs r3, #40
	str r3, [r2]
	bl 0x0200a598
	bl 0x0200a5a8
	ldr r0, [pc, #872]
	bl 0x0200a450
	cmp r0, #0
.L_02000970:
	beq .L_02000970_0
	b 0x02008e2c
.L_02000970_0:
	movs r0, #80
	bl 0x0200a468
	ldr r2, [pc, #868]
	movs r0, #19
	ldr r1, [pc, #848]
	bl 0x0200a4f8
	ldr r0, [pc, #864]
	ldr r1, [pc, #868]
	bl 0x0200a578
	movs r1, #1
	movs r3, #1
	ldr r0, [pc, #828]
	negs r1, r1
	ldr r2, [pc, #832]
	bl 0x0200a580
	movs r0, #19
.L_0200099c:
	ldr r1, [pc, #848]
	ldr r2, [pc, #852]
	bl 0x0200a4a0
	movs r2, #174
	ldr r1, [pc, #848]
	lsls r2, r2, #2
	movs r0, #19
	bl 0x0200a4d0
	movs r0, #80
	bl 0x0200a468
	movs r1, #1
	movs r2, #166
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #18
	ldr r0, [pc, #780]
	bl 0x0200a580
	movs r0, #19
.L_020009c8:
	bl 0x0200a4f0
	movs r2, #174
	movs r0, #19
	ldr r1, [pc, #808]
	lsls r2, r2, #2
	bl 0x0200a4d8
	movs r2, #159
	movs r0, #19
	ldr r1, [pc, #796]
	lsls r2, r2, #2
	bl 0x0200a4d8
	movs r1, #224
	movs r0, #18
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200a560
	movs r1, #219
	ldr r2, [pc, #780]
	movs r0, #19
	lsls r1, r1, #2
	bl 0x0200a4d8
	movs r1, #3
	movs r0, #19
	bl 0x0200a508
	movs r0, #20
	bl 0x0200a468
	movs r1, #3
	movs r0, #18
	bl 0x0200a508
	movs r0, #10
	bl 0x0200a468
	ldr r0, [pc, #744]
	bl 0x0200a538
	movs r2, #10
	ldr r0, [pc, #740]
	movs r1, #0
	bl 0x0200a550
	movs r0, #19
	movs r1, #2
	bl 0x0200a520
	movs r2, #20
	movs r0, #19
	movs r1, #0
	bl 0x0200a550
	movs r0, #18
	movs r1, #1
	bl 0x0200a520
	movs r2, #10
	ldr r0, [pc, #704]
	movs r1, #0
	bl 0x0200a550
	movs r1, #3
	movs r0, #19
	bl 0x0200a508
	movs r0, #40
	bl 0x0200a468
	movs r2, #60
	movs r0, #18
	ldr r1, [pc, #684]
	bl 0x0200a568
	ldr r0, [pc, #672]
	movs r1, #0
	bl 0x0200a548
	movs r0, #18
	movs r1, #1
	bl 0x0200a520
	movs r2, #10
	ldr r0, [pc, #656]
	movs r1, #0
	bl 0x0200a550
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #19
	bl 0x0200a570
	movs r0, #60
	bl 0x0200a468
	movs r1, #192
	movs r0, #19
	lsls r1, r1, #6
	movs r2, #10
	bl 0x0200a560
	movs r1, #160
	movs r0, #18
	lsls r1, r1, #7
	movs r2, #10
	bl 0x0200a560
	movs r1, #1
	movs r3, #1
	ldr r0, [pc, #548]
	negs r1, r1
	ldr r2, [pc, #552]
	bl 0x0200a580
	movs r0, #0
	ldr r1, [pc, #536]
	ldr r2, [pc, #548]
	bl 0x0200a4f8
	movs r0, #0
	ldr r1, [pc, #548]
	ldr r2, [pc, #588]
	bl 0x0200a4a0
	ldr r2, [pc, #584]
	ldr r1, [pc, #556]
	movs r0, #0
	bl 0x0200a4d8
	movs r0, #20
	bl 0x0200a468
	movs r1, #3
	movs r0, #0
	bl 0x0200a508
	movs r0, #10
	bl 0x0200a468
	movs r0, #18
	movs r1, #1
	bl 0x0200a520
	ldr r0, [pc, #536]
	movs r1, #0
	bl 0x0200a548
	movs r1, #1
	movs r2, #166
	ldr r0, [pc, #468]
	negs r1, r1
	lsls r2, r2, #18
	movs r3, #1
	bl 0x0200a580
	movs r2, #171
	movs r0, #0
	ldr r1, [pc, #492]
	lsls r2, r2, #2
	bl 0x0200a4d8
	movs r0, #0
	bl 0x0200a498
	cmp r0, #0
	beq .L_020009c8_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200a4f8
.L_020009c8_0:
	movs r0, #0
	bl 0x0200a498
	cmp r0, #0
	beq .L_020009c8_1
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #2
	bl 0x0200a4f8
.L_020009c8_1:
	movs r0, #3
	bl 0x0200a450
	cmp r0, #0
	beq .L_020009c8_2
	movs r0, #0
	bl 0x0200a498
	cmp r0, #0
	beq .L_020009c8_2
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #3
	bl 0x0200a4f8
.L_020009c8_2:
	movs r0, #1
	ldr r1, [pc, #396]
	ldr r2, [pc, #432]
	bl 0x0200a4a0
	movs r0, #2
	ldr r1, [pc, #384]
	ldr r2, [pc, #424]
	bl 0x0200a4a0
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #3
	lsls r1, r1, #9
	bl 0x0200a4a0
	movs r0, #1
	movs r1, #2
	bl 0x0200a500
	movs r0, #2
	movs r1, #2
	bl 0x0200a500
	movs r0, #3
	movs r1, #2
	bl 0x0200a500
	movs r1, #16
	movs r0, #1
	negs r1, r1
	movs r2, #16
	bl 0x0200a4e8
	movs r0, #2
	movs r1, #16
	movs r2, #16
	bl 0x0200a4e8
	movs r0, #3
	bl 0x0200a450
	cmp r0, #0
	beq .L_020009c8_3
	movs r0, #3
	movs r1, #32
	movs r2, #16
	bl 0x0200a4e8
.L_020009c8_3:
	movs r0, #2
	bl 0x0200a4f0
	movs r0, #1
	movs r1, #1
	bl 0x0200a500
	movs r0, #2
	movs r1, #1
	bl 0x0200a500
	movs r1, #1
	movs r0, #3
	bl 0x0200a500
	movs r0, #10
	bl 0x0200a468
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a560
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a560
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200a560
	movs r0, #18
	movs r1, #2
	movs r2, #20
	bl 0x0200a510
	movs r1, #224
	movs r0, #18
	lsls r1, r1, #7
	movs r2, #10
	bl 0x0200a560
	ldr r0, [pc, #236]
	movs r1, #0
	movs r2, #10
	bl 0x0200a550
	movs r1, #128
	movs r2, #10
	movs r0, #19
	lsls r1, r1, #5
	bl 0x0200a560
	movs r0, #19
	movs r1, #3
	bl 0x0200a508
	movs r1, #160
	movs r0, #18
	lsls r1, r1, #7
	movs r2, #40
	bl 0x0200a560
	movs r1, #224
	movs r2, #10
	movs r0, #18
	lsls r1, r1, #7
	bl 0x0200a560
	movs r0, #18
	movs r1, #4
	bl 0x0200a508
	movs r2, #10
	ldr r0, [pc, #172]
	movs r1, #0
	bl 0x0200a550
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #19
	bl 0x0200a570
	movs r0, #40
	bl 0x0200a468
	movs r1, #160
	movs r0, #18
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200a560
	movs r2, #40
	movs r0, #18
	ldr r1, [pc, #136]
	bl 0x0200a568
	movs r1, #0
	ldr r0, [pc, #124]
	bl 0x0200a540
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a560
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a560
	movs r0, #0
	movs r1, #0
	bl 0x0200a490
	cmp r0, #0
	bne .L_020009c8_4
	b .L_020009c8_5
.L_020009c8_4:
	ldr r0, [pc, #96]
	bl 0x0200a538
	ldr r0, [pc, #72]
	movs r1, #0
	movs r2, #10
	bl 0x0200a550
	movs r1, #192
	movs r2, #0
	movs r0, #19
	lsls r1, r1, #6
	b .L_020009c8_6
	.4byte 0x037e0000
	.2byte 0x085f
	.2byte 0x0000
	.4byte 0x02ba0000
	.2byte 0x0000
	.2byte 0x027a
	.4byte 0x031e0000
	.2byte 0x1ebc
	.2byte 0x0300
	.4byte 0x00009999
	.2byte 0x1333
	.2byte 0x0000
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.4byte 0x0000037e
	.4byte 0x0000034a
	.4byte 0x0000027a
	.4byte 0x00001437
	.4byte 0x00002012
	.4byte 0x00000105
	.4byte 0x00004ccc
	.4byte 0x000002d6
	.4byte 0x00001440
.L_020009c8_6:
	bl 0x0200a560
	movs r0, #18
	movs r1, #4
	bl 0x0200a508
	ldr r0, [pc, #240]
	movs r1, #0
	movs r2, #10
	bl 0x0200a550
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a560
	movs r1, #192
	movs r2, #0
	movs r0, #2
	lsls r1, r1, #8
	bl 0x0200a560
	movs r0, #0
	movs r1, #3
	bl 0x0200a508
	movs r0, #1
	movs r1, #2
	bl 0x0200a500
	movs r0, #0
	bl 0x0200a498
	cmp r0, #0
	beq .L_020009c8_7
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x0200a4c0
.L_020009c8_7:
	movs r0, #2
	movs r1, #2
	bl 0x0200a500
	movs r0, #0
	bl 0x0200a498
	cmp r0, #0
	beq .L_020009c8_8
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #2
	bl 0x0200a4c0
.L_020009c8_8:
	movs r0, #3
	bl 0x0200a450
	cmp r0, #0
	beq .L_020009c8_9
	movs r0, #3
	movs r1, #2
	bl 0x0200a500
	movs r0, #0
	bl 0x0200a498
	cmp r0, #0
	beq .L_020009c8_9
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #3
	bl 0x0200a4c0
.L_020009c8_9:
	movs r0, #2
	bl 0x0200a4f0
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200a4f8
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x0200a4f8
	movs r1, #0
	movs r2, #0
	movs r0, #3
	bl 0x0200a4f8
	ldr r0, [pc, #60]
	bl 0x0200a458
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200a4a0
	movs r2, #188
	movs r0, #0
	ldr r1, [pc, #40]
	lsls r2, r2, #2
	bl 0x0200a4d8
	ldr r3, [pc, #36]
	movs r2, #228
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #16
	str r2, [r3]
	bl 0x0200a5a0
	bl 0x0200a5a8
	bl 0x0200977e
	movs r0, r0
	.4byte 0x00002012
	.4byte 0x0000085f
	.4byte 0x0000037e
	.4byte 0x03001ebc
	movs r0, #0
	ldr r1, [pc, #984]
	ldr r2, [pc, #984]
	bl 0x0200a4a0
	movs r2, #171
	lsls r2, r2, #2
	ldr r1, [pc, #980]
	movs r0, #0
	bl 0x0200a4d0
	movs r0, #80
	bl 0x0200a468
	ldr r0, [pc, #956]
	ldr r1, [pc, #968]
	bl 0x0200a578
	movs r1, #1
	movs r2, #166
	negs r1, r1
	lsls r2, r2, #18
	movs r3, #1
	ldr r0, [pc, #956]
	bl 0x0200a580
	movs r0, #0
	bl 0x0200a4f0
	movs r0, #0
	movs r1, #1
	bl 0x0200a500
	movs r0, #0
	bl 0x0200a498
	cmp r0, #0
	beq .L_020009c8_10
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200a4f8
.L_020009c8_10:
	movs r0, #0
	bl 0x0200a498
	cmp r0, #0
	beq .L_020009c8_11
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #2
	bl 0x0200a4f8
.L_020009c8_11:
	movs r0, #3
	bl 0x0200a450
	cmp r0, #0
	beq .L_020009c8_12
	movs r0, #0
	bl 0x0200a498
	cmp r0, #0
	beq .L_020009c8_12
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #3
	bl 0x0200a4f8
.L_020009c8_12:
	movs r0, #1
	ldr r1, [pc, #848]
	ldr r2, [pc, #848]
	bl 0x0200a4a0
	movs r0, #2
	ldr r1, [pc, #836]
	ldr r2, [pc, #840]
	bl 0x0200a4a0
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #3
	lsls r1, r1, #9
	bl 0x0200a4a0
	movs r0, #1
	movs r1, #2
	bl 0x0200a500
	movs r0, #2
	movs r1, #2
	bl 0x0200a500
	movs r0, #3
	movs r1, #2
	bl 0x0200a500
	movs r1, #16
	movs r0, #1
	negs r1, r1
	movs r2, #16
	bl 0x0200a4e8
	movs r0, #2
	movs r1, #16
	movs r2, #16
	bl 0x0200a4e8
	movs r0, #3
	bl 0x0200a450
	cmp r0, #0
	beq .L_020009c8_13
	movs r0, #3
	movs r1, #32
	movs r2, #16
	bl 0x0200a4e8
.L_020009c8_13:
	movs r0, #2
	bl 0x0200a4f0
	movs r0, #1
	movs r1, #1
	bl 0x0200a500
	movs r0, #2
	movs r1, #1
	bl 0x0200a500
	movs r1, #1
	movs r0, #3
	bl 0x0200a500
	movs r0, #10
	bl 0x0200a468
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a560
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a560
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200a560
	ldr r1, [pc, #696]
	movs r2, #60
	movs r0, #18
	bl 0x0200a568
	ldr r0, [pc, #692]
	bl 0x0200a538
	movs r1, #0
	ldr r0, [pc, #688]
	bl 0x0200a540
	movs r0, #0
	movs r1, #0
	bl 0x0200a490
	cmp r0, #1
	bne .L_020009c8_5
	b .L_020009c8_4
.L_020009c8_5:
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a560
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a560
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a560
	movs r1, #192
	movs r2, #20
	movs r0, #2
	lsls r1, r1, #8
	bl 0x0200a560
	movs r0, #3
	movs r1, #3
	bl 0x0200a500
	movs r0, #0
	movs r1, #3
	bl 0x0200a500
	movs r0, #1
	movs r1, #3
	bl 0x0200a500
	movs r0, #2
	movs r1, #3
	bl 0x0200a508
	movs r2, #60
	ldr r1, [pc, #588]
	movs r0, #18
	bl 0x0200a568
	ldr r0, [pc, #584]
	bl 0x0200a538
	movs r1, #0
	ldr r0, [pc, #568]
	bl 0x0200a548
	movs r0, #20
	bl 0x0200a498
	movs r1, #0
	bl 0x0200a440
	movs r0, #20
.L_02000ffc:
	bl 0x0200a498
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #24]
	str r3, [r0, #28]
	movs r0, #18
	bl 0x0200a498
	cmp r0, #0
	beq .L_02000ffc_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #20
	bl 0x0200a4f8
.L_02000ffc_0:
	movs r0, #1
	bl 0x0200a408
	movs r0, #20
	movs r1, #6
	movs r2, #0
	bl 0x0200a510
	movs r1, #128
	movs r2, #128
	movs r0, #20
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200a4a0
	movs r2, #167
	ldr r1, [pc, #464]
	lsls r2, r2, #2
	movs r0, #20
	bl 0x0200a4c8
	movs r0, #40
	bl 0x0200a468
	ldr r0, [pc, #468]
	movs r1, #0
	movs r2, #10
	bl 0x0200a550
	movs r0, #3
	ldr r1, [pc, #448]
	movs r2, #0
	bl 0x0200a568
	movs r0, #0
	ldr r1, [pc, #440]
	movs r2, #0
	bl 0x0200a568
	movs r0, #1
	ldr r1, [pc, #428]
	movs r2, #0
	bl 0x0200a568
	movs r2, #60
	movs r0, #2
	ldr r1, [pc, #416]
	bl 0x0200a568
	movs r0, #18
	movs r1, #4
	bl 0x0200a508
	ldr r0, [pc, #412]
	movs r1, #0
	movs r2, #10
	bl 0x0200a550
	movs r0, #1
	ldr r1, [pc, #412]
	movs r2, #60
	bl 0x0200a568
	movs r1, #224
	movs r2, #10
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200a560
	movs r1, #0
	ldr r0, [pc, #392]
	bl 0x0200a540
	movs r1, #160
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a560
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a560
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200a560
	movs r0, #0
	movs r1, #0
	bl 0x0200a490
	cmp r0, #1
	bne .L_02000ffc_1
.L_02000ffc_2:
	movs r0, #1
	movs r1, #2
	bl 0x0200a518
	movs r1, #2
	movs r0, #2
	bl 0x0200a520
	ldr r0, [pc, #328]
	bl 0x0200a538
	movs r1, #0
	ldr r0, [pc, #316]
	bl 0x0200a540
	movs r0, #0
	movs r1, #0
	bl 0x0200a490
	cmp r0, #1
	bne .L_02000ffc_2
.L_02000ffc_1:
	movs r1, #3
	movs r0, #1
	bl 0x0200a508
	ldr r0, [pc, #296]
	bl 0x0200a538
	ldr r0, [pc, #284]
	movs r1, #0
	movs r2, #10
	bl 0x0200a550
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a560
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a560
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a560
	movs r1, #192
	movs r2, #10
	movs r0, #2
	lsls r1, r1, #8
	bl 0x0200a560
	movs r0, #0
	movs r1, #3
	bl 0x0200a500
	movs r1, #3
	movs r0, #0
	bl 0x0200a508
	movs r0, #20
	bl 0x0200a468
	movs r0, #18
	ldr r1, [pc, #188]
	movs r2, #60
	bl 0x0200a568
	ldr r0, [pc, #176]
	movs r1, #0
	movs r2, #20
	bl 0x0200a550
	movs r1, #129
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200a568
	movs r1, #192
	movs r0, #18
	lsls r1, r1, #6
	movs r2, #10
	bl 0x0200a560
	movs r0, #18
	ldr r1, [pc, #132]
	movs r2, #60
	bl 0x0200a568
	movs r0, #1
	ldr r1, [pc, #124]
	movs r2, #40
	bl 0x0200a568
	movs r2, #20
	movs r0, #1
	movs r1, #0
	bl 0x0200a560
	movs r0, #1
	movs r1, #1
	bl 0x0200a520
	ldr r0, [pc, #120]
	movs r1, #0
	movs r2, #10
	bl 0x0200a550
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a560
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #20
	bl 0x0200a560
	movs r1, #129
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200a568
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200a560
	movs r2, #10
	ldr r0, [pc, #72]
	movs r1, #0
	bl 0x0200a550
	movs r1, #1
	movs r0, #1
	bl 0x0200a520
	b .L_02000ffc_3
	.2byte 0x0000
	.2byte 0x9999
	.2byte 0x0000
	.2byte 0x4ccc
	.2byte 0x0000
	.4byte 0x0000037e
	.2byte 0x1333
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x037e
	.4byte 0x00000101
	.2byte 0x1442
	.2byte 0x0000
	.4byte 0x00002012
	.4byte 0x00000105
	.2byte 0x1443
	.2byte 0x0000
	.4byte 0x00000103
	.4byte 0x00004001
	.4byte 0x00001447
	.4byte 0x00001448
	.4byte 0x00004002
.L_02000ffc_3:
	movs r0, #40
	bl 0x0200a468
	movs r0, #1
	movs r1, #3
	bl 0x0200a508
	ldr r0, [pc, #76]
	movs r1, #0
	movs r2, #10
	bl 0x0200a550
	movs r1, #224
	movs r2, #10
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200a560
	movs r0, #1
	movs r1, #1
	bl 0x0200a518
	movs r1, #0
	ldr r0, [pc, #44]
	bl 0x0200a540
	movs r1, #160
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a560
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a560
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200a560
	b .L_02000ffc_4
	.2byte 0x0000
	.4byte 0x00004001
.L_02000ffc_5:
	ldr r0, [pc, #1000]
	bl 0x0200a538
	ldr r0, [pc, #1000]
	movs r1, #0
	bl 0x0200a540
.L_02000ffc_4:
	movs r0, #0
	movs r1, #0
	bl 0x0200a490
	cmp r0, #0
	bne .L_02000ffc_5
	movs r0, #10
	bl 0x0200a468
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a560
	movs r0, #1
	movs r1, #0
	movs r2, #10
	bl 0x0200a560
	movs r1, #128
	movs r2, #10
	movs r0, #0
	lsls r1, r1, #6
	bl 0x0200a560
	movs r0, #1
	movs r1, #3
	bl 0x0200a508
	movs r0, #2
	ldr r1, [pc, #932]
	movs r2, #60
	bl 0x0200a568
	movs r1, #128
	movs r2, #10
	movs r0, #2
	lsls r1, r1, #8
	bl 0x0200a560
	movs r1, #4
	movs r0, #2
	bl 0x0200a508
	ldr r0, [pc, #908]
	bl 0x0200a538
	movs r2, #20
	ldr r0, [pc, #904]
	movs r1, #0
	bl 0x0200a550
	movs r0, #18
	movs r1, #1
	bl 0x0200a520
	movs r1, #160
	movs r2, #10
	movs r0, #18
	lsls r1, r1, #7
	bl 0x0200a560
	movs r0, #18
	movs r1, #4
	bl 0x0200a508
	ldr r0, [pc, #872]
	movs r1, #0
	movs r2, #10
	bl 0x0200a550
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a560
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a560
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a560
	movs r1, #192
	movs r2, #20
	movs r0, #2
	lsls r1, r1, #8
	bl 0x0200a560
	movs r0, #3
	movs r1, #3
	bl 0x0200a500
	movs r0, #0
	movs r1, #3
	bl 0x0200a500
	movs r0, #1
	movs r1, #3
	bl 0x0200a500
	movs r1, #3
	movs r0, #2
	bl 0x0200a508
	movs r0, #40
	bl 0x0200a468
	movs r0, #18
	ldr r1, [pc, #764]
	movs r2, #80
	bl 0x0200a568
	movs r1, #128
	movs r2, #10
	movs r0, #19
	lsls r1, r1, #5
	bl 0x0200a560
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #19
	bl 0x0200a570
	movs r0, #40
	bl 0x0200a468
	movs r2, #10
	movs r0, #19
	movs r1, #0
	bl 0x0200a550
	movs r1, #1
	movs r0, #18
	bl 0x0200a520
	movs r0, #40
	bl 0x0200a468
	movs r1, #224
	movs r0, #18
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200a560
	movs r2, #10
	ldr r0, [pc, #700]
	movs r1, #0
	bl 0x0200a550
	movs r1, #3
	movs r0, #19
	bl 0x0200a508
	movs r0, #20
	bl 0x0200a468
	movs r0, #18
	movs r1, #4
	bl 0x0200a508
	movs r0, #18
	movs r1, #4
	bl 0x0200a500
	ldr r0, [pc, #664]
	movs r1, #0
	movs r2, #10
	bl 0x0200a550
	movs r0, #20
	movs r1, #6
	movs r2, #0
	bl 0x0200a510
	movs r0, #18
	bl 0x0200a498
	cmp r0, #0
	beq .L_02000ffc_6
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #20
	bl 0x0200a4c0
.L_02000ffc_6:
	movs r0, #20
	bl 0x0200a4f0
	movs r2, #0
	movs r1, #0
	movs r0, #20
	bl 0x0200a4f8
	movs r0, #20
	bl 0x0200a468
	movs r1, #129
	movs r0, #3
	lsls r1, r1, #1
	bl 0x0200a570
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200a570
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200a570
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #2
	bl 0x0200a570
	movs r0, #40
	bl 0x0200a468
	movs r0, #19
	movs r1, #2
	bl 0x0200a520
	movs r2, #10
	movs r0, #19
	movs r1, #0
	bl 0x0200a550
	movs r0, #18
	movs r1, #3
	bl 0x0200a508
	movs r2, #20
	ldr r0, [pc, #524]
	movs r1, #0
	bl 0x0200a550
	movs r0, #19
	movs r1, #3
	bl 0x0200a508
	movs r1, #2
	movs r0, #2
	bl 0x0200a520
	movs r0, #20
	bl 0x0200a468
	ldr r0, [pc, #488]
	movs r1, #0
	movs r2, #10
	bl 0x0200a550
	movs r1, #192
	movs r0, #19
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200a560
	movs r1, #192
	movs r2, #10
	movs r0, #18
	lsls r1, r1, #6
	bl 0x0200a560
	movs r0, #18
	movs r1, #4
	bl 0x0200a508
	ldr r0, [pc, #452]
	movs r1, #0
	movs r2, #10
	bl 0x0200a550
	movs r0, #1
	ldr r1, [pc, #444]
	movs r2, #60
	bl 0x0200a568
	ldr r0, [pc, #416]
	movs r1, #0
	movs r2, #10
	bl 0x0200a550
	movs r1, #160
	movs r2, #10
	movs r0, #18
	lsls r1, r1, #7
	bl 0x0200a560
	movs r0, #18
	movs r1, #4
	bl 0x0200a500
	movs r2, #10
	ldr r0, [pc, #400]
	movs r1, #0
	bl 0x0200a550
	movs r0, #2
	movs r1, #4
	bl 0x0200a500
	ldr r0, [pc, #380]
	movs r1, #0
	movs r2, #10
	bl 0x0200a550
	movs r1, #192
	movs r2, #10
	movs r0, #18
	lsls r1, r1, #6
	bl 0x0200a560
	movs r0, #18
	movs r1, #3
	bl 0x0200a508
	ldr r0, [pc, #352]
	movs r1, #0
	movs r2, #10
	bl 0x0200a550
	movs r0, #3
	ldr r1, [pc, #348]
	movs r2, #0
	bl 0x0200a568
	movs r0, #0
	ldr r1, [pc, #340]
	movs r2, #0
	bl 0x0200a568
	movs r0, #1
	ldr r1, [pc, #328]
	movs r2, #0
	bl 0x0200a568
	movs r0, #2
	ldr r1, [pc, #320]
	movs r2, #60
	bl 0x0200a568
	movs r1, #224
	movs r2, #10
	movs r0, #18
	lsls r1, r1, #7
	bl 0x0200a560
	movs r1, #3
	movs r0, #18
	bl 0x0200a508
	movs r0, #10
	bl 0x0200a468
	movs r2, #10
	ldr r0, [pc, #276]
	movs r1, #0
	bl 0x0200a550
	movs r0, #19
	movs r1, #2
	bl 0x0200a520
	movs r1, #128
	movs r2, #10
	movs r0, #19
	lsls r1, r1, #5
	bl 0x0200a560
	movs r1, #3
	movs r0, #19
	bl 0x0200a508
	movs r0, #20
	bl 0x0200a468
	ldr r5, [pc, #244]
	movs r0, #0
	ldr r1, [pc, #244]
	adds r2, r5, #0
	bl 0x0200a530
	movs r0, #1
	ldr r1, [pc, #236]
	adds r2, r5, #0
	bl 0x0200a530
	movs r0, #2
	ldr r1, [pc, #224]
	adds r2, r5, #0
	bl 0x0200a530
	movs r0, #3
	ldr r1, [pc, #216]
	adds r2, r5, #0
	bl 0x0200a530
	movs r0, #19
	ldr r1, [pc, #208]
	ldr r2, [pc, #212]
	bl 0x0200a4a0
	movs r1, #213
	movs r0, #19
	lsls r1, r1, #2
	ldr r2, [pc, #204]
	bl 0x0200a4d8
	movs r1, #213
	movs r0, #19
	lsls r1, r1, #2
	ldr r2, [pc, #196]
	bl 0x0200a4d8
	movs r1, #216
	movs r2, #168
	movs r0, #19
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl 0x0200a4d8
	movs r1, #128
	movs r0, #19
	lsls r1, r1, #5
	movs r2, #10
	bl 0x0200a560
	movs r1, #0
	movs r2, #20
	ldr r0, [pc, #164]
	bl 0x0200a550
	movs r0, #0
	bl 0x0200a4b8
	movs r0, #1
	bl 0x0200a4b8
	movs r0, #2
	bl 0x0200a4b8
	movs r0, #0
	ldr r1, [pc, #88]
	movs r2, #0
	bl 0x0200a568
	movs r0, #1
	ldr r1, [pc, #80]
	movs r2, #0
	bl 0x0200a568
	movs r2, #60
	movs r0, #2
	ldr r1, [pc, #68]
	bl 0x0200a568
	movs r0, #0
	movs r1, #3
	bl 0x0200a500
	movs r0, #1
	movs r1, #3
	bl 0x0200a500
	movs r0, #2
	movs r1, #3
	bl 0x0200a508
	movs r1, #128
	movs r0, #19
	lsls r1, r1, #9
	adds r2, r5, #0
	bl 0x0200a530
	movs r0, #1
	movs r1, #2
	bl 0x0200a500
	movs r0, #0
	bl 0x0200a498
	cmp r0, #0
	beq .L_02000ffc_7
	b .L_02000ffc_8
	.4byte 0x0000144e
	.4byte 0x00004001
	.4byte 0x00000105
	.4byte 0x0000144f
	.4byte 0x00004002
	.4byte 0x00002012
	.4byte 0x00000103
	.4byte 0x00000107
	.4byte 0x0200a5c0
	.4byte 0x00010013
	.4byte 0x00009999
	.4byte 0x00004ccc
	.4byte 0x00000286
	.4byte 0x0000029a
	.4byte 0x00004013
.L_02000ffc_8:
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x0200a4c0
.L_02000ffc_7:
	movs r0, #2
	movs r1, #2
	bl 0x0200a500
	movs r0, #0
	bl 0x0200a498
	cmp r0, #0
	beq .L_02000ffc_9
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #2
	bl 0x0200a4c0
.L_02000ffc_9:
	movs r0, #3
	bl 0x0200a450
	cmp r0, #0
	beq .L_02000ffc_10
	movs r0, #3
	movs r1, #2
	bl 0x0200a500
	movs r0, #0
	bl 0x0200a498
	cmp r0, #0
	beq .L_02000ffc_10
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #3
	bl 0x0200a4c0
.L_02000ffc_10:
	movs r0, #2
	bl 0x0200a4f0
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200a4f8
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x0200a4f8
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl 0x0200a4f8
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200a4a0
	movs r2, #188
	movs r0, #0
	ldr r1, [pc, #52]
	lsls r2, r2, #2
	bl 0x0200a4d8
	ldr r3, [pc, #48]
	movs r2, #228
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #16
	str r2, [r3]
	bl 0x0200a5a0
	bl 0x0200a5a8
	ldr r0, [pc, #28]
	bl 0x0200a458
	movs r0, #29
	bl 0x0200a590
	bl 0x0200a478
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000037e
	.4byte 0x03001ebc
	.4byte 0x00000321
	.section .rodata,"a",%progbits
	.global Kyuden_FacingActions
Kyuden_FacingActions:
	.4byte 0x00000022
	.4byte OverlayObject_UpdateFacingTowardTarget
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Kyuden_PacingActions
Kyuden_PacingActions:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x034a0000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x034a0000
	.4byte 0x00000000
	.4byte 0x027c0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x036c0000
	.4byte 0x00000000
	.4byte 0x027a0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global Placement_Scripts
Placement_Scripts:
	.4byte 0xffff0000
	.4byte 0x00000100
	.4byte 0xc0000328
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000100
	.4byte 0xc0000328
	.4byte 0x00200000
	.4byte 0x01e001f0
	.4byte 0x00000358
	.4byte 0xffff0002
	.4byte 0x00000078
	.4byte 0x00000318
	.4byte 0x00200000
	.4byte 0x01e001f0
	.4byte 0x00000358
	.4byte 0xffff0003
	.4byte 0x00000188
	.4byte 0x80000318
	.4byte 0x00200000
	.4byte 0x01e001f0
	.4byte 0x00000358
	.4byte 0xffff0004
	.4byte 0x000000e8
	.4byte 0x000002c8
	.4byte 0x00200000
	.4byte 0x01e001f0
	.4byte 0x00000358
	.4byte 0xffff0005
	.4byte 0x00000118
	.4byte 0x800002c8
	.4byte 0x00200000
	.4byte 0x01e001f0
	.4byte 0x00000358
	.4byte 0xffff0006
	.4byte 0x00000098
	.4byte 0x80000278
	.4byte 0x00200000
	.4byte 0x01e001f0
	.4byte 0x00000358
	.4byte 0xffff0007
	.4byte 0x00000068
	.4byte 0x00000228
	.4byte 0x00200000
	.4byte 0x01e001f0
	.4byte 0x00000358
	.4byte 0xffff0008
	.4byte 0x00000198
	.4byte 0x80000228
	.4byte 0x00200000
	.4byte 0x01e001f0
	.4byte 0x00000358
	.4byte 0xffff0009
	.4byte 0x00000100
	.4byte 0x40000278
	.4byte 0x00200000
	.4byte 0x01e001f0
	.4byte 0x00000358
	.4byte 0xffff000a
	.4byte 0x00000098
	.4byte 0x80000178
	.4byte 0x00380000
	.4byte 0x01680130
	.4byte 0x000001d0
	.4byte 0xffff000b
	.4byte 0x00000108
	.4byte 0x00000178
	.4byte 0x00380000
	.4byte 0x01680130
	.4byte 0x000001d0
	.4byte 0xffff000c
	.4byte 0x00000068
	.4byte 0x40000178
	.4byte 0x00380000
	.4byte 0x01680130
	.4byte 0x000001d0
	.4byte 0xffff000d
	.4byte 0x00000138
	.4byte 0x40000178
	.4byte 0x00380000
	.4byte 0x01680130
	.4byte 0x000001d0
	.4byte 0xffff000e
	.4byte 0x000002b8
	.4byte 0x80000060
	.4byte 0x02500000
	.4byte 0x03400000
	.4byte 0x000000a0
	.4byte 0xffff000f
	.4byte 0x000002a8
	.4byte 0x80000048
	.4byte 0x02500000
	.4byte 0x03400000
	.4byte 0x000000a0
	.4byte 0xffff0010
	.4byte 0x000003b8
	.4byte 0x00000048
	.4byte 0x03200000
	.4byte 0x04100000
	.4byte 0x000000a0
	.4byte 0xffff0011
	.4byte 0x000003a8
	.4byte 0x00000060
	.4byte 0x03200000
	.4byte 0x04100000
	.4byte 0x000000a0
	.4byte 0xffff0012
	.4byte 0x00000058
	.4byte 0xc00000c8
	.4byte 0x00000000
	.4byte 0x00f00010
	.4byte 0x000000f0
	.4byte 0xffff0013
	.4byte 0x00000168
	.4byte 0xc00000c8
	.4byte 0x00d00000
	.4byte 0x01c00010
	.4byte 0x000000f0
	.4byte 0xffff0014
	.4byte 0x00000218
	.4byte 0x00000070
	.4byte 0x01900000
	.4byte 0x02800000
	.4byte 0x000000a0
	.4byte 0xffff0015
	.4byte 0x00000218
	.4byte 0x00000058
	.4byte 0x01900000
	.4byte 0x02800000
	.4byte 0x000000a0
	.4byte 0xffff0016
	.4byte 0x00000208
	.4byte 0x80000138
	.4byte 0x01900000
	.4byte 0x028000d0
	.4byte 0x00000170
	.4byte 0xffff0017
	.4byte 0x00000208
	.4byte 0x80000128
	.4byte 0x01900000
	.4byte 0x028000d0
	.4byte 0x00000170
	.4byte 0xffff0018
	.4byte 0x000002d8
	.4byte 0x00000108
	.4byte 0x02500000
	.4byte 0x034000c0
	.4byte 0x00000160
	.4byte 0xffff0019
	.4byte 0x00000388
	.4byte 0x80000108
	.4byte 0x03180000
	.4byte 0x040800c0
	.4byte 0x00000160
	.4byte 0xffff001a
	.4byte 0x000002e8
	.4byte 0x000001a8
	.4byte 0x02500000
	.4byte 0x03400160
	.4byte 0x00000200
	.4byte 0xffff001b
	.4byte 0x00000378
	.4byte 0x800001a8
	.4byte 0x03180000
	.4byte 0x04080160
	.4byte 0x00000200
	.4byte 0xffff001c
	.4byte 0x00000258
	.4byte 0x000002f8
	.4byte 0x02000000
	.4byte 0x02f00230
	.4byte 0x00000330
	.4byte 0xffff001d
	.4byte 0x00000380
	.4byte 0xc00002d8
	.4byte 0x03080000
	.4byte 0x03f80230
	.4byte 0x00000300
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Placement_Messages
Placement_Messages:
	.4byte 0x00000021
	.4byte 0x00102022
	.4byte 0x0020e021
	.4byte 0x00311021
	.4byte 0x0040a021
	.4byte 0x0050b021
	.4byte 0x0061c021
	.4byte 0x00716021
	.4byte 0x00814021
	.4byte 0x0091d021
	.4byte 0x00a04021
	.4byte 0x00b05021
	.4byte 0x00c12021
	.4byte 0x00d13021
	.4byte 0x00e02021
	.4byte 0x00f1a021
	.4byte 0x0101b021
	.4byte 0x01103021
	.4byte 0x0120c021
	.4byte 0x0130d021
	.4byte 0x01408021
	.4byte 0x01519021
	.4byte 0x01607021
	.4byte 0x01718021
	.4byte 0x01817021
	.4byte 0x01915021
	.4byte 0x01a0f021
	.4byte 0x01b10021
	.4byte 0x01c06021
	.4byte 0x01d09021
	.4byte 0x000001ff
	.global gBiribinoKyudenPlacementsOther
gBiribinoKyudenPlacementsOther:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBiribinoKyudenPlacements
gBiribinoKyudenPlacements:
	.4byte 0x00000065
	.4byte 0x00000001
	.4byte 0x01090000
	.4byte 0x00000000
	.4byte 0x02be0000
	.4byte 0x00005000
	.4byte 0x00000073
	.4byte 0x00000001
	.4byte 0x01560000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x0001d000
	.4byte 0x0000006a
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00011000
	.4byte 0x0000006c
	.4byte 0x00000002
	.4byte 0x00a60000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00001000
	.4byte 0x0000006a
	.4byte 0x00000001
	.4byte 0x01460000
	.4byte 0x00000000
	.4byte 0x017a0000
	.4byte 0x00007000
	.4byte 0x0000006a
	.4byte 0x00000002
	.4byte 0x016a0000
	.4byte 0x00000000
	.4byte 0x008e0000
	.4byte 0x00005000
	.4byte 0x0000006a
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00570000
	.4byte 0x0002b000
	.4byte 0x00000067
	.4byte 0x00000001
	.4byte 0x03b80000
	.4byte 0x00000000
	.4byte 0x01210000
	.4byte 0x00011000
	.4byte 0x00000096
	.4byte 0x00000001
	.4byte 0x03b70000
	.4byte 0x00000000
	.4byte 0x01b60000
	.4byte 0x00025000
	.4byte 0xffff0096
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00003000
	.4byte 0x0000003a
	.4byte 0x00000001
	.4byte 0x037e0000
	.4byte 0x00000000
	.4byte 0x027a0000
	.4byte 0x00013000
	.4byte 0xffff0096
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00003000
	.4byte 0xffff00f5
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0014
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0014
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0014
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0014
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00026000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00022000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000007
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00024000
	.4byte 0x00000014
	.4byte 0x00000007
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x01024000
	.4byte 0x00000014
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x01024000
	.4byte 0x00000014
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x01024000
	.4byte 0x00000014
	.4byte 0x00000007
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00024000
	.4byte 0x00000014
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x01024000
	.4byte 0x00000014
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x01024000
	.4byte 0x00000014
	.4byte 0x00000007
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00024000
	.4byte 0x00000014
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01024000
	.4byte 0x00000014
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBiribinoKyudenEventsOther
gBiribinoKyudenEventsOther:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBiribinoKyudenEvents
gBiribinoKyudenEvents:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x0000c402
	.4byte 0xffff0009
	.4byte BiribinoKyuden_RunDoorExitScene
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x0000c402
	.4byte 0xffff000c
	.4byte BiribinoKyuden_RunDoorExitScene
	.4byte 0x0000c402
	.4byte 0xffff000d
	.4byte BiribinoKyuden_RunDoorExitScene
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0xffff0012
	.4byte 0x00000012
	.4byte 0x00000001
	.4byte 0xffff0013
	.4byte 0x00000013
	.4byte 0x00000001
	.4byte 0xffff0014
	.4byte 0x00000014
	.4byte 0x00000001
	.4byte 0xffff0015
	.4byte 0x00000015
	.4byte 0x00000001
	.4byte 0xffff0016
	.4byte 0x00000016
	.4byte 0x00000001
	.4byte 0xffff0017
	.4byte 0x00000017
	.4byte 0x00000001
	.4byte 0xffff0018
	.4byte 0x00000018
	.4byte 0x00000001
	.4byte 0xffff0019
	.4byte 0x00000019
	.4byte 0x00000001
	.4byte 0xffff001a
	.4byte 0x0000001a
	.4byte 0x00000001
	.4byte 0xffff001b
	.4byte 0x0000001b
	.4byte 0x00000001
	.4byte 0xffff001c
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0xffff001d
	.4byte 0x0000001d
	.4byte 0x00000000
	.4byte 0x084e0008
	.4byte 0x0000141e
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001759
	.4byte 0x00000000
	.4byte 0x084e0009
	.4byte 0x0000141f
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0000175a
	.4byte 0x00000000
	.4byte 0x084e000a
	.4byte 0x02008131
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0000175b
	.4byte 0x00000000
	.4byte 0x084e000b
	.4byte 0x00001423
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0000175c
	.4byte 0x00000000
	.4byte 0x084e000c
	.4byte 0x00001424
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0000175d
	.4byte 0x00000000
	.4byte 0x084e000d
	.4byte 0x0000142a
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001763
	.4byte 0x00000000
	.4byte 0x084e000e
	.4byte 0x0000142b
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte FieldScene_RunStartledGuard
	.4byte 0x00000000
	.4byte 0x084f000f
	.4byte 0x0000142e
	.4byte 0x00000000
	.4byte 0x084e000f
	.4byte 0x00001467
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte FieldScene_RunScene38dSequenceA
	.4byte 0x00000000
	.4byte 0x084e0010
	.4byte 0x0000142f
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte SceneDialogue_RunActor16Message1769
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001747
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte FieldScene_RunActorSeventeenFlaggedDialogue
	.4byte 0x00000000
	.4byte 0x084e0012
	.4byte 0x00001466
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x0000174f
	.4byte 0x00008d15
	.4byte 0x084e0008
	.4byte 0x00001425
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000175e
	.4byte 0x00008d15
	.4byte 0x084e0009
	.4byte 0x00001426
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x0000175f
	.4byte 0x00008d15
	.4byte 0x084e000a
	.4byte 0x00001427
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001760
	.4byte 0x00008d15
	.4byte 0x084e000b
	.4byte 0x00001428
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001761
	.4byte 0x00008d15
	.4byte 0x084e000c
	.4byte 0x00001429
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001762
	.4byte 0x00008d15
	.4byte 0x084e000d
	.4byte 0x0000142c
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001766
	.4byte 0x00008d15
	.4byte 0x084e000e
	.4byte 0x0000142d
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001767
	.4byte 0x00008d15
	.4byte 0x084f000f
	.4byte 0x00001430
	.4byte 0x00008d15
	.4byte 0x084e000f
	.4byte 0x0000146b
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte SceneDialogue_RunActor15Flag303Scene
	.4byte 0x00008d15
	.4byte 0x084e0010
	.4byte 0x00001431
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x0000176d
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x0000174a
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte FieldScene_RunActorSeventeenFlagDialogue
	.4byte 0x00008d15
	.4byte 0x084e0012
	.4byte 0x0000146a
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001755
	.4byte 0x00000003
	.4byte 0xffff0064
	.4byte FieldScene_RunStep210ByFlag84e
	.4byte 0x00000003
	.4byte 0xffff0065
	.4byte FieldScene_RunStep211ByFlag84e
	.4byte 0x00000003
	.4byte 0xffff0066
	.4byte FieldScene_RunStep212ByFlag84e
	.4byte 0x00000003
	.4byte 0xffff0067
	.4byte FieldScene_RunStep213ByFlag84e
	.4byte 0x00000002
	.4byte 0x03330032
	.4byte FieldScene_RunPalaceFarewell
	.4byte 0x00000002
	.4byte 0x03330033
	.4byte FieldScene_RunRewardReminder
	.4byte 0x00000033
	.4byte 0x0f550078
	.4byte 0x001000bc
	.4byte 0x00000033
	.4byte 0x0f560079
	.4byte 0x001000e2
	.4byte 0x000001c3
	.4byte 0xffff00c8
	.4byte 0x004029d7
	.4byte 0x000000f3
	.4byte 0xffff00c9
	.4byte 0x004029d8
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
