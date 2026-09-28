.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/IMIRU_MURA/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	push {r5, lr}
	adds r5, r0, #0
	adds r5, #100
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #2
	beq .L_02000030_0
	cmp r3, #2
	bgt .L_02000030_1
	cmp r3, #0
	beq .L_02000030_2
	b .L_02000030_3
.L_02000030_1:
	cmp r3, #4
	beq .L_02000030_4
	cmp r3, #6
	bne .L_02000030_3
	ldr r3, [r0, #24]
	ldr r2, [pc, #120]
	adds r3, r3, r2
	str r3, [r0, #24]
	movs r2, #128
	ldr r3, [r0, #28]
	lsls r2, r2, #6
	b .L_02000030_5
.L_02000030_4:
	ldr r3, [r0, #24]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r2, [pc, #100]
	b .L_02000030_6
.L_02000030_0:
	ldr r3, [r0, #24]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r2, [pc, #88]
.L_02000030_6:
	ldr r3, [r0, #28]
.L_02000030_5:
	adds r3, r3, r2
	str r3, [r0, #28]
	b .L_02000030_3
.L_02000030_2:
	ldr r3, [r0, #24]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r2, [pc, #68]
	ldr r3, [r0, #28]
	adds r3, r3, r2
	str r3, [r0, #28]
	adds r3, r0, #0
	adds r3, #102
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_02000030_7
	bl 0x0200a180
	movs r1, #40
	bl 0x0200a168
	adds r0, #40
	b .L_02000030_8
.L_02000030_7:
	bl 0x0200a180
	movs r1, #20
	bl 0x0200a168
	adds r0, #20
.L_02000030_8:
	strh r0, [r5]
.L_02000030_3:
	ldrh r3, [r5]
	subs r3, #1
	strh r3, [r5]
	movs r0, #1
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0xffffc000
	.4byte 0xfffff000
	.4byte 0xfffff800
	.global Func_020000d8
	.thumb_func
Func_020000d8:
	push {r5, lr}
	adds r5, r0, #0
	ldr r1, [r5, #104]
	cmp r1, #0
	beq .L_020000d8_0
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	ldr r0, [r1, #16]
	ldr r3, [r5, #16]
	ldr r1, [r1, #8]
	subs r0, r0, r3
	ldr r3, [r5, #8]
	subs r1, r1, r3
	bl 0x0200a188
	ldrh r3, [r5, #6]
	lsls r0, r0, #16
	lsrs r0, r0, #16
	subs r0, r0, r3
	lsls r0, r0, #16
	asrs r0, r0, #16
	cmp r0, #0
	beq .L_020000d8_0
	movs r2, #128
	lsls r2, r2, #5
	cmp r0, r2
	ble .L_020000d8_1
	adds r0, r2, #0
.L_020000d8_1:
	ldr r2, [pc, #20]
	cmp r0, r2
	bge .L_020000d8_2
	adds r0, r2, #0
.L_020000d8_2:
	adds r3, r3, r0
	strh r3, [r5, #6]
.L_020000d8_0:
	movs r0, #1
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0xfffff000
	.global Func_02000130
	.thumb_func
Func_02000130:
	push {lr}
	ldr r3, [pc, #28]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_02000130_0
	ldr r0, [pc, #16]
	b .L_02000130_1
.L_02000130_0:
	ldr r0, [pc, #16]
.L_02000130_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000033
	.4byte 0x0200a8a0
	.4byte 0x0200a798
	.global Func_02000160
	.thumb_func
Func_02000160:
	movs r0, #0
	bx lr
	.global Func_02000164
	.thumb_func
Func_02000164:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200a990
	.global Func_0200016c
	.thumb_func
Func_0200016c:
	push {r5, lr}
	ldr r3, [pc, #88]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #80]
	cmp r2, r3
	bne .L_0200016c_0
	ldr r5, [pc, #76]
	adds r0, r5, #0
	bl 0x0200a240
	ldr r0, [pc, #72]
	bl 0x0200a210
	cmp r0, #0
	beq .L_0200016c_1
	movs r3, #131
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #0
	strb r3, [r2]
	movs r3, #182
	lsls r3, r3, #16
	str r3, [r5, #80]
	movs r3, #141
	lsls r3, r3, #18
	str r3, [r5, #88]
	movs r3, #2
	str r3, [r5, #76]
.L_0200016c_1:
	adds r0, r5, #0
	b .L_0200016c_2
.L_0200016c_0:
	ldr r0, [pc, #32]
	bl 0x0200a210
	cmp r0, #0
	beq .L_0200016c_3
	ldr r0, [pc, #28]
	b .L_0200016c_2
.L_0200016c_3:
	ldr r0, [pc, #28]
.L_0200016c_2:
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000033
	.4byte 0x0200aad0
	.4byte 0x00000881
	.4byte 0x0200aa58
	.4byte 0x0200a9e0
	.global Func_020001e0
	.thumb_func
Func_020001e0:
	push {lr}
	bl 0x02009fa4
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020001ec
	.thumb_func
Func_020001ec:
	push {lr}
	ldr r3, [pc, #28]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_020001ec_0
	ldr r0, [pc, #16]
	b .L_020001ec_1
.L_020001ec_0:
	ldr r0, [pc, #16]
.L_020001ec_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000033
	.4byte 0x0200adb8
	.4byte 0x0200ac80
	.global Func_0200021c
	.thumb_func
Func_0200021c:
	push {lr}
	bl 0x0200a230
	movs r0, #3
	bl 0x0200a210
	cmp r0, #0
	beq .L_0200021c_0
	ldr r0, [pc, #28]
	bl 0x0200a300
	b .L_0200021c_1
.L_0200021c_0:
	ldr r0, [pc, #24]
	bl 0x0200a300
.L_0200021c_1:
	movs r0, #8
	movs r1, #0
	bl 0x0200a308
	bl 0x0200a238
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001570
	.4byte 0x00001529
	.global Func_02000254
	.thumb_func
Func_02000254:
	push {lr}
	bl 0x0200a230
	movs r0, #3
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000254_0
	ldr r0, [pc, #28]
	bl 0x0200a300
	b .L_02000254_1
.L_02000254_0:
	ldr r0, [pc, #24]
	bl 0x0200a300
.L_02000254_1:
	movs r0, #8
	movs r1, #0
	bl 0x0200a308
	bl 0x0200a238
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001571
	.4byte 0x0000152f
	.global Func_0200028c
	.thumb_func
Func_0200028c:
	push {lr}
	bl 0x0200a230
	movs r2, #10
	movs r1, #0
	movs r0, #9
	bl 0x0200a2e0
	ldr r0, [pc, #20]
	bl 0x0200a300
	movs r1, #0
	movs r0, #9
	bl 0x0200a318
	bl 0x0200a238
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000152a
	.global Func_020002b8
	.thumb_func
Func_020002b8:
	push {lr}
	bl 0x0200a230
	ldr r0, [pc, #184]
	bl 0x0200a210
	cmp r0, #0
	beq .L_020002b8_0
	ldr r0, [pc, #176]
	bl 0x0200a300
	movs r0, #10
	movs r1, #0
	bl 0x0200a308
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #10
	bl 0x0200a338
	movs r0, #40
	bl 0x0200a228
	movs r1, #1
	movs r0, #10
	bl 0x0200a2b8
	movs r0, #20
	bl 0x0200a228
	movs r2, #20
	movs r0, #10
	movs r1, #0
	bl 0x0200a2e0
	movs r1, #0
	movs r0, #10
	bl 0x0200a318
	movs r1, #192
	movs r0, #10
	lsls r1, r1, #6
	movs r2, #10
	bl 0x0200a320
	movs r0, #10
	movs r1, #9
	bl 0x0200a2b8
	b .L_020002b8_1
.L_020002b8_0:
	ldr r0, [pc, #96]
	bl 0x0200a300
	movs r0, #10
	movs r1, #0
	bl 0x0200a308
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #10
	bl 0x0200a338
	movs r0, #40
	bl 0x0200a228
	movs r1, #1
	movs r0, #10
	bl 0x0200a2b8
	movs r0, #20
	bl 0x0200a228
	movs r2, #20
	movs r0, #10
	movs r1, #0
	bl 0x0200a2e0
	movs r0, #10
	movs r1, #0
	bl 0x0200a308
	movs r1, #192
	movs r0, #10
	lsls r1, r1, #6
	movs r2, #10
	bl 0x0200a320
	movs r0, #10
	movs r1, #9
	bl 0x0200a2b8
.L_020002b8_1:
	bl 0x0200a238
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000881
	.4byte 0x0000163c
	.4byte 0x0000152d
	.global Func_02000384
	.thumb_func
Func_02000384:
	push {lr}
	ldr r0, [pc, #340]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000384_0
	bl 0x0200a230
	movs r1, #0
	movs r2, #0
	movs r0, #9
	bl 0x0200a2e0
	movs r0, #10
	bl 0x0200a228
	ldr r0, [pc, #312]
	bl 0x0200a300
	movs r1, #0
	movs r0, #9
	bl 0x0200a318
	bl 0x0200a238
	b .L_02000384_1
.L_02000384_0:
	ldr r0, [pc, #296]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000384_2
	bl 0x0200a230
	movs r0, #9
	movs r1, #7
	bl 0x0200a2b8
	movs r2, #69
	movs r1, #10
	ldr r0, [pc, #276]
	bl 0x0200a1e0
	ldr r0, [pc, #272]
	bl 0x0200a300
	movs r0, #9
	movs r1, #0
	bl 0x0200a308
	movs r0, #9
	movs r1, #8
	bl 0x0200a2b8
	ldr r0, [pc, #256]
	movs r1, #10
	movs r2, #69
	bl 0x0200a1e0
	bl 0x0200a238
	b .L_02000384_1
.L_02000384_2:
	bl 0x0200a230
	movs r0, #9
	bl 0x0200a248
	movs r3, #10
	adds r0, #100
	strh r3, [r0]
	ldr r1, [pc, #228]
	movs r0, #9
	bl 0x0200a258
	ldr r0, [pc, #224]
	bl 0x0200a300
	movs r1, #0
	movs r0, #9
	bl 0x0200a308
	movs r0, #8
	bl 0x0200a268
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200a330
	movs r1, #208
	movs r2, #10
	movs r0, #8
	lsls r1, r1, #8
	bl 0x0200a320
	movs r0, #8
	movs r1, #2
	bl 0x0200a2d0
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200a310
	ldr r1, [pc, #164]
	movs r0, #0
	bl 0x0200a258
	ldr r2, [pc, #160]
	movs r0, #8
	ldr r1, [pc, #160]
	bl 0x0200a250
	ldr r1, [pc, #160]
	movs r0, #8
	bl 0x0200a270
	movs r0, #40
	bl 0x0200a228
	movs r2, #0
	movs r0, #8
	movs r1, #2
	bl 0x0200a2c8
	movs r0, #8
	movs r1, #2
	bl 0x0200a2d0
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #8
	bl 0x0200a338
	movs r0, #60
	bl 0x0200a228
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl 0x0200a310
	movs r1, #192
	movs r2, #20
	movs r0, #8
	lsls r1, r1, #6
	bl 0x0200a320
	movs r0, #8
	movs r1, #2
	bl 0x0200a2d0
	movs r1, #0
	movs r0, #8
	bl 0x0200a308
	movs r0, #8
	bl 0x0200a248
	adds r0, #89
	ldrb r3, [r0]
	movs r2, #2
	eors r3, r2
	strb r3, [r0]
	ldr r0, [pc, #60]
	bl 0x0200a218
	bl 0x0200a238
.L_02000384_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000881
	.4byte 0x00001644
	.4byte 0x0000082b
	.4byte 0x0200b1c0
	.4byte 0x0000156c
	.4byte 0x0200b1d6
	.4byte 0x0200a4f4
	.4byte 0x00001534
	.4byte 0x0200a564
	.4byte 0x0000cccc
	.4byte 0x00019999
	.4byte 0x0200a508
	.4byte 0x0000082c
	.global Func_02000510
	.thumb_func
Func_02000510:
	push {lr}
	bl 0x0200a230
	ldr r0, [pc, #20]
	bl 0x0200a300
	movs r1, #0
	movs r0, #12
	bl 0x0200a318
	bl 0x0200a238
	pop {r0}
	bx r0
	.4byte 0x0000153f
	.global Func_02000530
	.thumb_func
Func_02000530:
	push {lr}
	bl 0x0200a230
	ldr r0, [pc, #20]
	bl 0x0200a300
	movs r1, #0
	movs r0, #18
	bl 0x0200a318
	bl 0x0200a238
	pop {r0}
	bx r0
	.4byte 0x0000154d
	.global Func_02000550
	.thumb_func
Func_02000550:
	push {lr}
	bl 0x0200a230
	movs r0, #3
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000550_0
	ldr r0, [pc, #48]
	bl 0x0200a300
	movs r0, #20
	movs r1, #0
	bl 0x0200a308
	b .L_02000550_1
.L_02000550_0:
	ldr r0, [pc, #36]
	bl 0x0200a300
	movs r1, #0
	movs r0, #20
	bl 0x0200a318
	ldr r0, [pc, #28]
	bl 0x0200a218
	ldr r0, [pc, #24]
	bl 0x0200a218
.L_02000550_1:
	bl 0x0200a238
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001574
	.4byte 0x00001557
	.4byte 0x0000082a
	.4byte 0x0000082c
	.global Func_020005a4
	.thumb_func
Func_020005a4:
	push {lr}
	bl 0x0200a230
	movs r0, #3
	bl 0x0200a210
	cmp r0, #0
	beq .L_020005a4_0
	ldr r0, [pc, #28]
	bl 0x0200a300
	b .L_020005a4_1
.L_020005a4_0:
	ldr r0, [pc, #24]
	bl 0x0200a300
.L_020005a4_1:
	movs r0, #20
	movs r1, #0
	bl 0x0200a308
	bl 0x0200a238
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001575
	.4byte 0x0000155b
	.global Func_020005dc
	.thumb_func
Func_020005dc:
	push {lr}
	bl 0x0200a230
	ldr r0, [pc, #32]
	bl 0x0200a300
	movs r0, #8
	movs r1, #0
	bl 0x0200a308
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #10
	bl 0x0200a320
	bl 0x0200a238
	pop {r0}
	bx r0
	.4byte 0x0000156d
	.global Func_02000608
	.thumb_func
Func_02000608:
	push {lr}
	bl 0x0200a230
	ldr r0, [pc, #56]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000608_0
	ldr r0, [pc, #48]
	bl 0x0200a300
	b .L_02000608_1
.L_02000608_0:
	ldr r0, [pc, #44]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000608_2
	ldr r0, [pc, #40]
	bl 0x0200a300
	b .L_02000608_1
.L_02000608_2:
	ldr r0, [pc, #36]
	bl 0x0200a300
.L_02000608_1:
	movs r0, #8
	movs r1, #0
	bl 0x0200a308
	bl 0x0200a238
	pop {r0}
	bx r0
	.4byte 0x0000082b
	.4byte 0x0000156f
	.4byte 0x0000082c
	.4byte 0x0000153b
	.4byte 0x00001533
	.global Func_0200065c
	.thumb_func
Func_0200065c:
	push {lr}
	bl 0x02008384
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000668
	.thumb_func
Func_02000668:
	push {lr}
	bl 0x0200a230
	ldr r0, [pc, #20]
	bl 0x0200a300
	movs r0, #10
	movs r1, #0
	bl 0x0200a308
	bl 0x0200a238
	pop {r0}
	bx r0
	.4byte 0x0000156e
	.global Func_02000688
	.thumb_func
Func_02000688:
	push {lr}
	bl 0x0200a230
	movs r0, #3
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000688_0
	ldr r0, [pc, #28]
	bl 0x0200a300
	b .L_02000688_1
.L_02000688_0:
	ldr r0, [pc, #24]
	bl 0x0200a300
.L_02000688_1:
	movs r0, #19
	movs r1, #0
	bl 0x0200a308
	bl 0x0200a238
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001573
	.4byte 0x0000155a
	.global Func_020006c0
	.thumb_func
Func_020006c0:
	push {r5, r6, r7, lr}
	ldr r3, [pc, #204]
	ldr r7, [r3]
	bl 0x0200a230
	movs r5, #8
	movs r6, #0
.L_020006c0_1:
	adds r0, r5, #0
	bl 0x0200a248
	cmp r0, #0
	beq .L_020006c0_0
	adds r3, r0, #0
	adds r3, #85
	strb r6, [r3]
.L_020006c0_0:
	adds r5, #1
	cmp r5, #65
	bls .L_020006c0_1
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r7, r2
	ldrh r3, [r3]
	subs r3, #50
	lsls r3, r3, #16
	asrs r5, r3, #16
	cmp r5, #6
	bne .L_020006c0_2
	movs r0, #188
	bl 0x0200a3e8
	b .L_020006c0_3
.L_020006c0_2:
	movs r0, #158
	bl 0x0200a3e8
.L_020006c0_3:
	ldr r2, [pc, #140]
	lsls r3, r5, #2
	subs r0, r3, #4
	subs r3, #2
	ldrsh r1, [r2, r0]
	ldrsh r2, [r2, r3]
	ldr r3, [pc, #132]
	ldr r0, [r3, r0]
	bl 0x0200a1e0
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #7
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200a250
	ldr r3, [pc, #104]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #192
	str r2, [r3]
	cmp r5, #6
	bne .L_020006c0_4
	ldr r2, [pc, #96]
	movs r0, #0
	ldr r1, [pc, #96]
	bl 0x0200a250
	movs r0, #0
	movs r1, #2
	bl 0x0200a2b8
	movs r0, #0
	movs r1, #3
	bl 0x0200a328
	movs r2, #8
	movs r0, #0
	movs r1, #0
	negs r2, r2
	bl 0x0200a2a0
	b .L_020006c0_5
.L_020006c0_4:
	movs r0, #0
	bl 0x0200a248
	movs r3, #0
	adds r0, #85
	movs r2, #16
	strb r3, [r0]
	movs r1, #3
	movs r0, #0
	negs r2, r2
	bl 0x0200a298
.L_020006c0_5:
	movs r0, #16
	bl 0x0200a228
	adds r0, r5, #0
	bl 0x0200a360
	bl 0x0200a238
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x0200b1a8
	.4byte 0x0200b190
	.4byte 0x00001999
	.4byte 0x00003333
	.global Func_020007a4
	.thumb_func
Func_020007a4:
	push {r5, lr}
	movs r0, #0
	bl 0x0200a248
	movs r2, #6
	ldrsh r5, [r0, r2]
	ldr r0, [pc, #192]
	bl 0x0200a210
	cmp r0, #0
	beq .L_020007a4_0
	ldr r2, [pc, #188]
	lsls r3, r5, #16
	adds r3, r3, r2
	ldr r2, [pc, #184]
	cmp r3, r2
	bhi .L_020007a4_1
	movs r0, #10
	movs r1, #12
	bl 0x0200a3d0
	b .L_020007a4_2
.L_020007a4_1:
	bl 0x0200a230
	movs r2, #0
	movs r1, #0
	movs r0, #12
	bl 0x0200a2e0
	movs r0, #10
	bl 0x0200a228
	ldr r0, [pc, #152]
	bl 0x0200a300
	movs r0, #12
	movs r1, #0
	bl 0x0200a308
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #7
	movs r2, #10
	bl 0x0200a320
	bl 0x0200a238
	b .L_020007a4_2
.L_020007a4_0:
	ldr r2, [pc, #112]
	lsls r3, r5, #16
	adds r3, r3, r2
	ldr r2, [pc, #112]
	cmp r3, r2
	bhi .L_020007a4_2
	bl 0x0200a230
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl 0x0200a340
	movs r0, #213
	movs r1, #1
	movs r2, #246
	lsls r2, r2, #17
	movs r3, #1
	negs r1, r1
	lsls r0, r0, #17
	bl 0x0200a348
	bl 0x0200a350
	movs r0, #20
	bl 0x0200a228
	ldr r1, [pc, #68]
	movs r0, #12
	bl 0x0200a270
	ldr r0, [pc, #64]
	bl 0x0200a300
	movs r0, #12
	movs r1, #0
	bl 0x0200a308
	movs r0, #213
	movs r1, #1
	movs r2, #154
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #18
	movs r3, #1
	bl 0x0200a348
	bl 0x0200a350
	bl 0x0200a238
.L_020007a4_2:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000881
	.4byte 0x5fff0000
	.4byte 0x3ffe0000
	.4byte 0x0000164b
	.4byte 0x0200a5ec
	.4byte 0x0000153e
	.global Func_0200088c
	.thumb_func
Func_0200088c:
	push {r5, lr}
	movs r0, #0
	bl 0x0200a248
	movs r2, #6
	ldrsh r5, [r0, r2]
	ldr r0, [pc, #192]
	bl 0x0200a210
	cmp r0, #0
	beq .L_0200088c_0
	ldr r2, [pc, #188]
	lsls r3, r5, #16
	adds r3, r3, r2
	ldr r2, [pc, #184]
	cmp r3, r2
	bhi .L_0200088c_1
	movs r0, #11
	movs r1, #13
	bl 0x0200a3d0
	b .L_0200088c_2
.L_0200088c_1:
	bl 0x0200a230
	movs r2, #0
	movs r1, #0
	movs r0, #13
	bl 0x0200a2e0
	movs r0, #10
	bl 0x0200a228
	ldr r0, [pc, #152]
	bl 0x0200a300
	movs r0, #13
	movs r1, #0
	bl 0x0200a308
	movs r1, #128
	movs r0, #13
	lsls r1, r1, #7
	movs r2, #10
	bl 0x0200a320
	bl 0x0200a238
	b .L_0200088c_2
.L_0200088c_0:
	ldr r2, [pc, #112]
	lsls r3, r5, #16
	adds r3, r3, r2
	ldr r2, [pc, #112]
	cmp r3, r2
	bhi .L_0200088c_2
	bl 0x0200a230
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl 0x0200a340
	movs r0, #213
	movs r1, #1
	movs r2, #246
	lsls r2, r2, #17
	movs r3, #1
	negs r1, r1
	lsls r0, r0, #17
	bl 0x0200a348
	bl 0x0200a350
	movs r0, #20
	bl 0x0200a228
	ldr r1, [pc, #68]
	movs r0, #13
	bl 0x0200a270
	ldr r0, [pc, #64]
	bl 0x0200a300
	movs r0, #13
	movs r1, #0
	bl 0x0200a308
	movs r0, #213
	movs r1, #1
	movs r2, #154
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #18
	movs r3, #1
	bl 0x0200a348
	bl 0x0200a350
	bl 0x0200a238
.L_0200088c_2:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000881
	.4byte 0x5fff0000
	.4byte 0x3ffe0000
	.4byte 0x0000164d
	.4byte 0x0200a5ec
	.4byte 0x00001543
	.global Func_02000974
	.thumb_func
Func_02000974:
	push {r5, lr}
	movs r0, #0
	bl 0x0200a248
	movs r2, #6
	ldrsh r5, [r0, r2]
	ldr r0, [pc, #160]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000974_0
	ldr r2, [pc, #156]
	lsls r3, r5, #16
	adds r3, r3, r2
	ldr r2, [pc, #152]
	cmp r3, r2
	bhi .L_02000974_1
	movs r0, #12
	movs r1, #15
	bl 0x0200a3d0
	b .L_02000974_2
.L_02000974_1:
	bl 0x0200a230
	movs r2, #0
	movs r1, #0
	movs r0, #15
	bl 0x0200a2e0
	ldr r0, [pc, #128]
	bl 0x0200a300
	movs r0, #15
	movs r1, #0
	bl 0x0200a308
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200a320
	bl 0x0200a238
	b .L_02000974_2
.L_02000974_0:
	ldr r2, [pc, #88]
	lsls r3, r5, #16
	adds r3, r3, r2
	ldr r2, [pc, #84]
	cmp r3, r2
	bhi .L_02000974_3
	bl 0x0200a230
	ldr r0, [pc, #84]
	bl 0x0200a300
	movs r0, #14
	movs r1, #0
	bl 0x0200a308
	movs r1, #14
	movs r0, #12
	bl 0x0200a3d0
	bl 0x0200a238
	b .L_02000974_2
.L_02000974_3:
	movs r2, #10
	movs r1, #0
	movs r0, #14
	bl 0x0200a2e0
	ldr r0, [pc, #48]
	bl 0x0200a300
	movs r0, #14
	movs r1, #0
	bl 0x0200a308
	movs r1, #160
	movs r0, #14
	lsls r1, r1, #7
	movs r2, #10
	bl 0x0200a320
.L_02000974_2:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00000881
	.4byte 0x5fff0000
	.4byte 0x3ffe0000
	.4byte 0x0000164f
	.4byte 0x00001546
	.4byte 0x00001547
	.global Func_02000a3c
	.thumb_func
Func_02000a3c:
	push {lr}
	movs r0, #0
	bl 0x0200a248
	ldr r2, [pc, #96]
	ldrh r3, [r0, #6]
	adds r3, r3, r2
	ldr r2, [pc, #96]
	lsls r3, r3, #16
	cmp r3, r2
	bhi .L_02000a3c_0
	movs r0, #4
	movs r1, #16
	bl 0x0200a3e0
	b .L_02000a3c_1
.L_02000a3c_0:
	bl 0x0200a230
	movs r0, #16
	movs r1, #0
	movs r2, #10
	bl 0x0200a2e0
	ldr r0, [pc, #68]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000a3c_2
	ldr r0, [pc, #60]
	bl 0x0200a300
	movs r0, #16
	movs r1, #0
	bl 0x0200a318
	b .L_02000a3c_3
.L_02000a3c_2:
	ldr r0, [pc, #48]
	bl 0x0200a300
	movs r0, #16
	movs r1, #0
	bl 0x0200a308
.L_02000a3c_3:
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #6
	movs r2, #10
	bl 0x0200a320
	bl 0x0200a238
.L_02000a3c_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00005fff
	.4byte 0x3ffe0000
	.4byte 0x00000881
	.4byte 0x00001653
	.4byte 0x0000154b
	.global Func_02000abc
	.thumb_func
Func_02000abc:
	push {lr}
	movs r0, #0
	bl 0x0200a248
	ldr r2, [pc, #136]
	ldrh r3, [r0, #6]
	adds r3, r3, r2
	ldr r2, [pc, #136]
	lsls r3, r3, #16
	cmp r3, r2
	bhi .L_02000abc_0
	bl 0x0200a230
	ldr r0, [pc, #128]
	bl 0x0200a210
	cmp r0, #0
	bne .L_02000abc_1
	ldr r0, [pc, #120]
	bl 0x0200a300
	movs r0, #19
	movs r1, #0
	bl 0x0200a308
	ldr r0, [pc, #104]
	bl 0x0200a218
.L_02000abc_1:
	bl 0x0200a238
	movs r0, #19
	bl 0x0200a3d8
	b .L_02000abc_2
.L_02000abc_0:
	bl 0x0200a230
	ldr r0, [pc, #88]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000abc_3
	ldr r0, [pc, #84]
	b .L_02000abc_4
.L_02000abc_3:
	movs r0, #3
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000abc_5
	ldr r0, [pc, #72]
.L_02000abc_4:
	bl 0x0200a300
	movs r0, #19
	movs r1, #0
	bl 0x0200a308
	b .L_02000abc_6
.L_02000abc_5:
	ldr r0, [pc, #60]
	bl 0x0200a300
	movs r1, #0
	movs r0, #19
	bl 0x0200a318
	movs r1, #192
	movs r0, #19
	lsls r1, r1, #6
	movs r2, #10
	bl 0x0200a320
.L_02000abc_6:
	bl 0x0200a238
.L_02000abc_2:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00005fff
	.4byte 0x3ffe0000
	.4byte 0x0000082d
	.4byte 0x00001553
	.4byte 0x00000881
	.4byte 0x00001671
	.4byte 0x00001572
	.4byte 0x00001554
	.global Func_02000b70
	.thumb_func
Func_02000b70:
	push {r5, r6, lr}
	ldr r6, [pc, #892]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r6, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #884]
	sub sp, #8
	cmp r2, r3
	bne .L_02000b70_0
	movs r0, #0
	bl 0x0200a248
	ldr r3, [pc, #872]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #192
	adds r5, r0, #0
	str r2, [r3]
	movs r0, #10
	movs r1, #9
	bl 0x0200a2b8
	ldr r0, [pc, #852]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000b70_1
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200a220
	ldr r0, [pc, #840]
	bl 0x0200a220
.L_02000b70_1:
	adds r3, r5, #0
	movs r2, #0
	adds r3, #100
	strh r2, [r3]
	movs r1, #200
	adds r3, #2
	strh r2, [r3]
	lsls r1, r1, #4
	ldr r0, [pc, #820]
	bl 0x0200a178
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #816]
	bl 0x0200a178
	movs r0, #11
	movs r1, #1
	bl 0x0200a328
	ldr r0, [pc, #804]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000b70_2
	bl 0x02009960
.L_02000b70_2:
	ldr r0, [pc, #776]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000b70_3
	b .L_02000b70_4
.L_02000b70_3:
	movs r1, #225
	lsls r1, r1, #1
	adds r3, r6, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #9
	beq .L_02000b70_5
	b .L_02000b70_4
.L_02000b70_5:
	bl 0x020099bc
	b .L_02000b70_4
.L_02000b70_0:
	ldr r3, [pc, #760]
	cmp r2, r3
	beq .L_02000b70_6
	b .L_02000b70_4
.L_02000b70_6:
	ldr r3, [pc, #728]
	movs r1, #224
	ldr r3, [r3]
	lsls r1, r1, #1
	ldr r2, [pc, #748]
	adds r3, r3, r1
	str r2, [r3]
	subs r2, #71
	adds r3, r6, r2
	movs r1, #0
	ldrsh r5, [r3, r1]
	cmp r5, #1
	beq .L_02000b70_7
	b .L_02000b70_8
.L_02000b70_7:
	movs r1, #15
	movs r0, #21
	bl 0x0200a2e8
	movs r0, #21
	bl 0x0200a248
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #8
	orrs r3, r2
	strb r3, [r0]
	movs r1, #1
	movs r0, #21
	bl 0x0200a328
	ldr r0, [pc, #700]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000b70_9
	movs r3, #10
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #10
	movs r1, #7
	movs r2, #1
	movs r3, #1
	bl 0x0200a200
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r2, #9
	movs r1, #125
	movs r3, #69
	movs r0, #3
	bl 0x0200a1e8
	bl 0x0200a1c8
	movs r0, #1
	bl 0x0200a170
	movs r0, #8
	movs r1, #2
	bl 0x0200a258
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x0200a2b0
	b .L_02000b70_4
.L_02000b70_9:
	ldr r0, [pc, #628]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000b70_10
	ldr r0, [pc, #620]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000b70_10
	movs r0, #10
	bl 0x0200a248
	movs r1, #0
	bl 0x0200a208
	movs r1, #174
	movs r2, #164
	lsls r2, r2, #16
	lsls r1, r1, #16
	movs r0, #9
	bl 0x0200a2b0
	movs r0, #9
	bl 0x0200a248
	movs r1, #0
	bl 0x0200a208
	movs r0, #9
	movs r1, #5
	bl 0x0200a2b8
	movs r1, #168
	movs r2, #152
	movs r0, #8
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl 0x0200a2b0
	movs r0, #8
	bl 0x0200a248
	movs r3, #192
	lsls r3, r3, #6
	strh r3, [r0, #6]
	ldr r0, [pc, #544]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000b70_11
	b .L_02000b70_4
.L_02000b70_11:
	bl 0x02008f90
	b .L_02000b70_4
.L_02000b70_10:
	movs r3, #10
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #10
	movs r1, #7
	movs r2, #1
	movs r3, #1
	bl 0x0200a200
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #3
	movs r1, #125
	movs r2, #9
	movs r3, #69
	bl 0x0200a1e8
	bl 0x0200a1c8
	movs r0, #1
	bl 0x0200a170
	ldr r0, [pc, #468]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000b70_12
	movs r1, #149
	movs r2, #232
	lsls r1, r1, #16
	lsls r2, r2, #15
	movs r0, #8
	bl 0x0200a2b0
	movs r0, #8
	bl 0x0200a248
	movs r5, #0
	strh r5, [r0, #6]
	movs r0, #9
	bl 0x0200a248
	adds r0, #102
	strh r5, [r0]
	ldr r1, [pc, #436]
	movs r0, #9
	bl 0x0200a258
	b .L_02000b70_4
.L_02000b70_12:
	movs r0, #8
	movs r1, #2
	bl 0x0200a258
	b .L_02000b70_4
.L_02000b70_8:
	cmp r5, #2
	bne .L_02000b70_13
	ldr r0, [pc, #396]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000b70_14
	b .L_02000b70_4
.L_02000b70_14:
	movs r0, #11
	bl 0x0200a248
	movs r3, #1
	adds r0, #102
	strh r3, [r0]
	ldr r1, [pc, #388]
	movs r0, #11
	bl 0x0200a258
	b .L_02000b70_4
.L_02000b70_13:
	cmp r5, #4
	bne .L_02000b70_15
	ldr r0, [pc, #360]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000b70_16
	movs r1, #182
	ldr r2, [pc, #368]
	movs r0, #12
	lsls r1, r1, #17
	bl 0x0200a2b0
	movs r1, #2
	movs r0, #12
	bl 0x0200a328
	movs r0, #12
	bl 0x0200a248
	adds r0, #89
	ldrb r3, [r0]
	movs r6, #4
	orrs r3, r6
	strb r3, [r0]
	movs r5, #3
	movs r3, #88
	movs r0, #6
	movs r1, #125
	movs r2, #22
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a1e8
	movs r1, #246
	ldr r2, [pc, #312]
	movs r0, #13
	lsls r1, r1, #17
	bl 0x0200a2b0
	movs r1, #2
	movs r0, #13
	bl 0x0200a328
	movs r0, #13
	bl 0x0200a248
	adds r0, #89
	ldrb r3, [r0]
	orrs r6, r3
	strb r6, [r0]
	movs r1, #125
	movs r0, #9
	movs r2, #28
	movs r3, #88
	b .L_02000b70_17
.L_02000b70_16:
	movs r0, #12
	bl 0x0200a248
	ldr r3, [pc, #268]
	str r3, [r0, #24]
	movs r0, #12
	bl 0x0200a248
	movs r1, #0
	bl 0x0200a208
	movs r1, #5
	movs r0, #12
	bl 0x0200a2b8
	movs r0, #13
	bl 0x0200a248
	movs r1, #0
	bl 0x0200a208
	movs r0, #13
	b .L_02000b70_18
.L_02000b70_15:
	cmp r5, #3
	bne .L_02000b70_19
	ldr r0, [pc, #200]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000b70_20
	movs r1, #230
	movs r2, #129
	lsls r2, r2, #17
	movs r0, #15
	lsls r1, r1, #17
	bl 0x0200a2b0
	movs r1, #2
	movs r0, #15
	bl 0x0200a328
	movs r0, #15
	bl 0x0200a248
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #4
	orrs r3, r2
	movs r1, #204
	movs r2, #132
	strb r3, [r0]
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r0, #14
	bl 0x0200a2b0
	movs r0, #14
	bl 0x0200a248
	movs r3, #128
	lsls r3, r3, #5
	strh r3, [r0, #6]
	movs r1, #125
	movs r0, #12
	movs r2, #26
	movs r3, #70
.L_02000b70_17:
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a1e8
	b .L_02000b70_4
.L_02000b70_20:
	movs r1, #230
	movs r2, #129
	lsls r2, r2, #17
	movs r0, #14
	lsls r1, r1, #17
	bl 0x0200a2b0
	movs r1, #2
	movs r0, #14
	bl 0x0200a328
	movs r0, #14
	bl 0x0200a248
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #4
	orrs r3, r2
	strb r3, [r0]
	movs r0, #15
	bl 0x0200a248
	ldr r3, [pc, #88]
	str r3, [r0, #24]
	movs r0, #15
	bl 0x0200a248
	movs r1, #0
	bl 0x0200a208
	movs r0, #15
.L_02000b70_18:
	movs r1, #5
	bl 0x0200a2b8
	b .L_02000b70_4
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000032
	.4byte 0x03001ebc
	.4byte 0x00000109
	.4byte 0x00000201
	.4byte 0x02009795
	.4byte 0x020098c5
	.4byte 0x00000203
	.4byte 0x00000033
	.4byte 0x00000209
	.4byte 0x00000881
	.4byte 0x0000082c
	.4byte 0x0000082a
	.4byte 0x0000082b
	.4byte 0x0200a4f4
	.4byte 0x02420000
	.4byte 0xffff0000
.L_02000b70_19:
	cmp r5, #7
	bne .L_02000b70_4
	ldr r0, [pc, #72]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000b70_4
	movs r0, #20
	bl 0x0200a248
	movs r3, #192
	lsls r3, r3, #6
	strh r3, [r0, #6]
	ldr r0, [pc, #56]
	bl 0x0200a210
	cmp r0, #0
	bne .L_02000b70_21
	movs r2, #161
	movs r0, #20
	ldr r1, [pc, #44]
	lsls r2, r2, #16
	bl 0x0200a2b0
	bl 0x020099e8
	b .L_02000b70_4
.L_02000b70_21:
	movs r1, #161
	movs r2, #166
	movs r0, #20
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl 0x0200a2b0
.L_02000b70_4:
	movs r0, #0
	sub sp, #-8
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x00000881
	.4byte 0x0000082e
	.4byte 0x028a0000
	.global Func_02000f90
	.thumb_func
Func_02000f90:
	push {r5, lr}
	bl 0x0200a230
	movs r1, #182
	movs r2, #150
	movs r0, #3
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl 0x0200a2b0
	movs r0, #141
	movs r1, #1
	movs r2, #221
	lsls r2, r2, #16
	movs r3, #0
	negs r1, r1
	lsls r0, r0, #16
	bl 0x0200a348
	movs r0, #1
	bl 0x0200a170
	ldr r0, [pc, #652]
	ldr r1, [pc, #656]
	bl 0x0200a340
	movs r0, #140
	movs r1, #1
	movs r2, #164
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	bl 0x0200a348
	ldr r3, [pc, #636]
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
	bl 0x0200a380
	movs r0, #0
	ldr r1, [pc, #612]
	ldr r2, [pc, #612]
	bl 0x0200a250
	movs r0, #1
	ldr r1, [pc, #600]
	ldr r2, [pc, #604]
	bl 0x0200a250
	movs r0, #2
	ldr r1, [pc, #592]
	ldr r2, [pc, #592]
	bl 0x0200a250
	movs r0, #0
	movs r1, #142
	movs r2, #221
	bl 0x0200a290
	movs r1, #208
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a320
	movs r0, #0
	bl 0x0200a248
	cmp r0, #0
	beq .L_02000f90_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200a2b0
.L_02000f90_0:
	movs r0, #0
	bl 0x0200a248
	cmp r0, #0
	beq .L_02000f90_1
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #2
	bl 0x0200a2b0
.L_02000f90_1:
	movs r0, #1
	movs r1, #150
	movs r2, #234
	bl 0x0200a288
	movs r2, #234
	movs r0, #2
	movs r1, #134
	bl 0x0200a290
	movs r0, #1
	movs r1, #1
	bl 0x0200a2b8
	ldr r5, [pc, #500]
	movs r0, #0
	adds r2, r5, #0
	ldr r1, [pc, #500]
	bl 0x0200a2f8
	adds r2, r5, #0
	movs r0, #1
	ldr r1, [pc, #488]
	bl 0x0200a2f8
	adds r2, r5, #0
	movs r0, #2
	ldr r1, [pc, #480]
	bl 0x0200a2f8
	bl 0x0200a350
	ldr r5, [pc, #472]
	movs r0, #9
	adds r1, r5, #0
	bl 0x0200a258
	movs r0, #40
	bl 0x0200a228
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #3
	bl 0x0200a338
	movs r0, #40
	bl 0x0200a228
	movs r1, #1
	movs r0, #3
	bl 0x0200a2d8
	ldr r0, [pc, #436]
	bl 0x0200a300
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x0200a310
	adds r1, r5, #0
	movs r0, #9
	bl 0x0200a258
	movs r0, #9
	movs r1, #0
	movs r2, #20
	bl 0x0200a310
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200a320
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x0200a320
	movs r0, #8
	movs r1, #4
	bl 0x0200a2c0
	movs r2, #40
	movs r0, #8
	movs r1, #0
	bl 0x0200a310
	movs r1, #3
	movs r0, #3
	bl 0x0200a2c0
	movs r0, #10
	bl 0x0200a228
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200a320
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #20
	bl 0x0200a320
	movs r2, #10
	movs r0, #3
	movs r1, #0
	bl 0x0200a310
	adds r1, r5, #0
	movs r0, #9
	bl 0x0200a258
	bl 0x020096c8
	movs r0, #3
	ldr r1, [pc, #304]
	movs r2, #60
	bl 0x0200a330
	movs r0, #3
	movs r1, #0
	movs r2, #40
	bl 0x0200a310
	movs r0, #9
	movs r1, #0
	movs r2, #20
	bl 0x0200a310
	movs r2, #60
	movs r0, #8
	ldr r1, [pc, #276]
	bl 0x0200a330
	movs r0, #9
	movs r1, #7
	bl 0x0200a2b8
	movs r2, #69
	movs r1, #10
	ldr r0, [pc, #264]
	bl 0x0200a1e0
	movs r0, #10
	bl 0x0200a228
	movs r0, #3
	movs r1, #2
	bl 0x0200a2d8
	movs r0, #3
	movs r1, #4
	bl 0x0200a2c0
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x0200a310
	movs r1, #1
	movs r0, #9
	bl 0x0200a2d8
	movs r0, #40
	bl 0x0200a228
	movs r0, #9
	movs r1, #8
	bl 0x0200a2b8
	movs r2, #69
	movs r1, #10
	ldr r0, [pc, #204]
	bl 0x0200a1e0
	movs r0, #40
	bl 0x0200a228
	movs r1, #3
	movs r0, #3
	bl 0x0200a2c0
	movs r0, #20
	bl 0x0200a228
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200a320
	movs r0, #8
	movs r1, #3
	bl 0x0200a2c0
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl 0x0200a310
	movs r0, #3
	ldr r1, [pc, #136]
	movs r2, #30
	bl 0x0200a330
	movs r1, #128
	movs r2, #10
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200a320
	movs r0, #3
	movs r1, #4
	bl 0x0200a2b8
	movs r2, #10
	movs r0, #3
	movs r1, #0
	bl 0x0200a310
	movs r1, #3
	movs r0, #8
	bl 0x0200a2c0
	movs r0, #20
	bl 0x0200a228
	movs r1, #3
	movs r0, #3
	bl 0x0200a2c0
	movs r0, #40
	bl 0x0200a228
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #3
	bl 0x0200a250
	movs r0, #3
	bl 0x0200a248
	movs r3, #0
	adds r0, #100
	strh r3, [r0]
	ldr r1, [pc, #60]
	movs r0, #3
	bl 0x0200a258
	b .L_02000f90_2
	.4byte 0x00004ccc
	.4byte 0x00000999
	.4byte 0x03001ebc
	.4byte 0x00006666
	.4byte 0x00003333
	.4byte 0x0200a74c
	.4byte 0x00010003
	.4byte 0x0200a5ec
	.4byte 0x0000155c
	.4byte 0x00000101
	.4byte 0x00000105
	.4byte 0x0200b1c0
	.4byte 0x0200b1d6
	.4byte 0x0200a670
.L_02000f90_3:
	movs r0, #1
	bl 0x0200a170
.L_02000f90_2:
	movs r0, #3
	bl 0x0200a248
	adds r0, #100
	movs r2, #0
	ldrsh r3, [r0, r2]
	cmp r3, #0
	beq .L_02000f90_3
	movs r0, #140
	movs r1, #1
	movs r2, #198
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #16
	bl 0x0200a348
	movs r0, #3
	bl 0x0200a260
	movs r0, #3
	ldr r1, [pc, #628]
	movs r2, #80
	bl 0x0200a330
	movs r2, #40
	movs r0, #3
	movs r1, #0
	bl 0x0200a310
	movs r1, #1
	movs r0, #3
	bl 0x0200a2d8
	movs r0, #10
	bl 0x0200a228
	movs r1, #0
	movs r0, #3
	bl 0x0200a308
	movs r0, #131
	bl 0x0200a3e8
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl 0x0200a370
	movs r1, #0
	ldr r0, [pc, #576]
	bl 0x0200a368
	movs r0, #10
	bl 0x0200a378
	movs r0, #1
	bl 0x0200a170
	movs r0, #220
	bl 0x0200a3e8
	movs r0, #40
	bl 0x0200a170
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl 0x0200a368
	movs r0, #60
	bl 0x0200a378
	movs r0, #60
	bl 0x0200a170
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #3
	bl 0x0200a338
	movs r0, #20
	bl 0x0200a228
	movs r0, #3
	movs r1, #0
	movs r2, #10
	bl 0x0200a320
	movs r1, #128
	movs r2, #128
	movs r0, #3
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200a250
	movs r2, #198
	movs r1, #202
	movs r0, #3
	bl 0x0200a290
	movs r0, #40
	bl 0x0200a228
	movs r0, #3
	movs r1, #2
	bl 0x0200a2d8
	movs r0, #3
	movs r1, #0
	bl 0x0200a308
	movs r0, #3
	movs r1, #4
	bl 0x0200a2c0
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x0200a310
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #3
	bl 0x0200a338
	movs r0, #40
	bl 0x0200a228
	movs r0, #3
	movs r1, #0
	movs r2, #40
	bl 0x0200a310
	movs r1, #128
	movs r2, #40
	movs r0, #3
	lsls r1, r1, #1
	bl 0x0200a330
	movs r1, #0
	movs r0, #3
	bl 0x0200a308
	movs r0, #0
	bl 0x0200a268
	movs r0, #1
	bl 0x0200a268
	movs r0, #2
	bl 0x0200a268
	movs r1, #192
	movs r2, #192
	lsls r1, r1, #10
	lsls r2, r2, #9
	movs r0, #3
	bl 0x0200a250
	movs r0, #3
	bl 0x0200a248
	movs r3, #0
	adds r0, #100
	strh r3, [r0]
	ldr r1, [pc, #348]
	movs r0, #3
	bl 0x0200a258
	b .L_02000f90_4
.L_02000f90_5:
	movs r0, #1
	bl 0x0200a170
.L_02000f90_4:
	movs r0, #3
	bl 0x0200a248
	adds r0, #100
	movs r2, #0
	ldrsh r3, [r0, r2]
	cmp r3, #0
	beq .L_02000f90_5
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200a320
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200a320
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #10
	bl 0x0200a320
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #11
	lsls r2, r2, #10
	bl 0x0200a250
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #11
	lsls r2, r2, #10
	bl 0x0200a250
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #10
	movs r0, #2
	bl 0x0200a250
	movs r0, #152
	bl 0x0200a3e8
	movs r0, #0
	bl 0x0200a248
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #1
	bl 0x0200a248
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #2
	bl 0x0200a248
	adds r0, #90
	ldrb r3, [r0]
	ands r5, r3
	strb r5, [r0]
	movs r1, #132
	movs r0, #0
	movs r2, #206
	bl 0x0200a278
	movs r0, #1
	movs r1, #136
	movs r2, #221
	bl 0x0200a278
	movs r1, #122
	movs r2, #238
	movs r0, #2
	bl 0x0200a278
	movs r0, #3
	bl 0x0200a260
	movs r0, #80
	bl 0x0200a228
	movs r0, #0
	bl 0x0200a248
	adds r0, #90
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #1
	bl 0x0200a248
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #2
	bl 0x0200a248
	adds r0, #90
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	ldr r1, [pc, #100]
	movs r0, #0
	ldr r2, [pc, #100]
	bl 0x0200a250
	movs r0, #1
	ldr r1, [pc, #88]
	ldr r2, [pc, #92]
	bl 0x0200a250
	ldr r2, [pc, #84]
	movs r0, #2
	ldr r1, [pc, #76]
	bl 0x0200a250
	ldr r5, [pc, #80]
	movs r0, #1
	adds r1, r5, #0
	bl 0x0200a258
	adds r1, r5, #0
	movs r0, #2
	bl 0x0200a270
	movs r0, #20
	bl 0x0200a228
	ldr r3, [pc, #60]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #73
	str r3, [r2]
	subs r3, #65
	adds r2, r1, r3
	movs r3, #24
	str r3, [r2]
	ldr r0, [pc, #40]
	bl 0x0200a218
	bl 0x0200a238
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00000101
	.4byte 0x00207e9f
	.4byte 0x0200a6e0
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x0200a760
	.4byte 0x03001ebc
	.4byte 0x0000082b
	.global Func_0200154c
	.thumb_func
Func_0200154c:
	push {r5, r6, lr}
	adds r6, r0, #0
	sub sp, #12
	cmp r6, #0
	beq .L_0200154c_0
	adds r2, r6, #0
	adds r2, #100
	ldrh r3, [r2]
	subs r3, #1
	strh r3, [r2]
	lsls r3, r3, #16
	asrs r1, r3, #16
	cmp r1, #0
	beq .L_0200154c_1
	ldr r2, [pc, #68]
	ldr r3, [r2]
	mov r5, sp
	str r3, [r5]
	movs r0, #128
	ldr r3, [r2, #4]
	lsls r0, r0, #12
	adds r3, r3, r0
	str r3, [r5, #4]
	ldr r3, [r2, #8]
	str r3, [r5, #8]
	adds r3, r6, #0
	adds r3, #102
	movs r2, #0
	ldrsh r3, [r3, r2]
	lsls r0, r1, #16
	lsls r1, r1, #11
	adds r1, r1, r3
	adds r2, r5, #0
	bl 0x0200a198
	ldr r3, [r5]
	str r3, [r6, #8]
	ldr r3, [r5, #4]
	str r3, [r6, #12]
	ldr r3, [r5, #8]
	str r3, [r6, #16]
	b .L_0200154c_0
.L_0200154c_1:
	adds r0, r6, #0
	bl 0x0200a1c0
.L_0200154c_0:
	sub sp, #-12
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200b1f0
	.global Func_020015b4
	.thumb_func
Func_020015b4:
	push {r5, r6, lr}
	adds r6, r0, #0
	adds r5, r6, #0
	adds r5, #100
	movs r2, #0
	ldrsh r0, [r5, r2]
	lsls r0, r0, #10
	sub sp, #12
	bl 0x0200a190
	adds r1, r0, #0
	movs r0, #192
	ldr r3, [pc, #188]
	lsls r0, r0, #11
	mov r12, pc
	bx r3
	.2byte 0x4b2e
	.2byte 0x681b
	.2byte 0x181b
	.2byte 0x60b3
	.2byte 0x882b
	.2byte 0x3301
	.2byte 0x802b
	.2byte 0x041b
	.2byte 0x1419
	.2byte 0x1c0a
	.2byte 0x3240
	.2byte 0x1c13
	.2byte 0x2a00
	.2byte 0xda01
	.2byte 0x1c0b
	.2byte 0x337f
	.2byte 0x119b
	.2byte 0x019b
	.2byte 0x1ad3
	.2byte 0x802b
	.2byte 0x4b25
	.2byte 0x2103
	.2byte 0x6818
	.2byte 0xf000
	.2byte 0xfdb1
	.2byte 0x2800
	.2byte 0xd14c
	.2byte 0x68b3
	.2byte 0x466d
	.2byte 0x602b
	.2byte 0x2280
	.2byte 0x68f3
	.2byte 0x0292
	.2byte 0x189b
	.2byte 0x606b
	.2byte 0x6933
	.2byte 0x60ab
	.2byte 0xf000
	.2byte 0xfdaf
	.2byte 0x1c06
	.2byte 0xf000
	.2byte 0xfdac
	.2byte 0x1c01
	.2byte 0x0070
	.2byte 0x1980
	.2byte 0x1c2a
	.2byte 0x0040
	.2byte 0xf000
	.2byte 0xfdb1
	.2byte 0x6829
	.2byte 0x686a
	.2byte 0x68ab
	.2byte 0x4816
	.2byte 0xf000
	.2byte 0xfdbb
	.2byte 0x1c05
	.2byte 0x2d00
	.2byte 0xd02d
	.2byte 0x6d29
	.2byte 0x230d
	.2byte 0x7a4a
	.2byte 0x425b
	.2byte 0x4013
	.2byte 0x724b
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xfdd7
	.2byte 0x1c28
	.2byte 0x2101
	.2byte 0xf000
	.2byte 0xfd9f
	.2byte 0x4b0e
	.2byte 0x1c2a
	.2byte 0x61ab
	.2byte 0x61eb
	.2byte 0x3223
	.2byte 0x2302
	.2byte 0x7013
	.2byte 0x4b05
	.2byte 0x3232
	.2byte 0x1c28
	.2byte 0x2109
	.2byte 0x7013
	.2byte 0xf000
	.2byte 0xfe39
	.2byte 0x4908
	.2byte 0x1c28
	.2byte 0xf000
	.2byte 0xfd95
	.2byte 0xe00d
	.2byte 0x0000
	.2byte 0x0000
	.4byte 0x03000118
	.2byte 0xb1f0
	.2byte 0x0200
	.2byte 0x1e40
	.2byte 0x0300
	.2byte 0x011d
	.2byte 0x0000
	.2byte 0x9999
	.2byte 0x0000
	.2byte 0xa64c
	.2byte 0x0200
	.2byte 0xb003
	.2byte 0xbc60
	.2byte 0xbc01
	.2byte 0x4700
	.global Func_020016ac
	.thumb_func
Func_020016ac:
	push {lr}
	ldr r1, [pc, #20]
	movs r0, #9
	bl 0x0200a258
	movs r0, #9
	movs r1, #0
	bl 0x0200a308
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200a5ec
	.global Func_020016c8
	.thumb_func
Func_020016c8:
	push {r5, lr}
	movs r0, #93
	movs r1, #1
	bl 0x0200a3a8
	ldr r3, [pc, #40]
	movs r1, #9
	movs r0, #3
	ldr r5, [r3]
	bl 0x0200a3b0
	ldr r3, [pc, #32]
	str r3, [r5, #36]
	bl 0x0200a3c8
	movs r0, #1
	bl 0x0200a3a0
	bl 0x0200a3b8
	bl 0x0200a3c0
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001f30
	.4byte 0x020096ad
	.global Func_02001704
	.thumb_func
Func_02001704:
	push {r5, lr}
	adds r5, r0, #0
	adds r3, r5, #0
	movs r2, #0
	adds r3, #85
	strb r2, [r3]
	adds r3, #15
	strh r2, [r3]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	ldr r1, [r5, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	movs r1, #9
	bl 0x0200a2f0
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200a208
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #24]
	str r3, [r5, #28]
	pop {r5}
	pop {r0}
	bx r0
	.global Func_0200174c
	.thumb_func
Func_0200174c:
	push {lr}
	adds r1, r0, #0
	adds r1, #100
	movs r3, #0
	ldrsh r2, [r1, r3]
	ldr r3, [r0, #8]
	lsls r2, r2, #8
	adds r3, r3, r2
	str r3, [r0, #8]
	movs r2, #128
	ldr r3, [r0, #12]
	lsls r2, r2, #8
	adds r3, r3, r2
	str r3, [r0, #12]
	ldr r2, [pc, #36]
	ldr r3, [r0, #24]
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r3, [r0, #28]
	adds r3, r3, r2
	str r3, [r0, #28]
	ldrh r3, [r1]
	adds r3, #2
	strh r3, [r1]
	ldr r3, [r0, #104]
	subs r3, #1
	str r3, [r0, #104]
	cmp r3, #0
	bne .L_0200174c_0
	bl 0x0200a1c0
.L_0200174c_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000007ae
	.global Func_02001794
	.thumb_func
Func_02001794:
	push {r5, r6, lr}
	ldr r6, [pc, #284]
	movs r1, #60
	ldr r0, [r6]
	bl 0x0200a168
	cmp r0, #0
	bne .L_02001794_0
	movs r3, #146
	movs r0, #222
	ldr r1, [pc, #268]
	movs r2, #0
	lsls r3, r3, #17
	bl 0x0200a1b8
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02001794_0
	bl 0x02009704
	movs r3, #60
	str r3, [r5, #104]
	ldr r3, [pc, #248]
	adds r0, r5, #0
	str r3, [r5, #108]
	movs r1, #5
	bl 0x0200a1a0
.L_02001794_0:
	ldr r0, [r6]
	movs r1, #60
	adds r0, #30
	bl 0x0200a168
	cmp r0, #0
	bne .L_02001794_1
	movs r1, #160
	movs r2, #128
	movs r3, #178
	movs r0, #222
	lsls r1, r1, #17
	lsls r2, r2, #14
	lsls r3, r3, #17
	bl 0x0200a1b8
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02001794_1
	bl 0x02009704
	movs r3, #60
	str r3, [r5, #104]
	ldr r3, [pc, #192]
	adds r0, r5, #0
	str r3, [r5, #108]
	movs r1, #5
	bl 0x0200a1a0
.L_02001794_1:
	ldr r0, [r6]
	movs r1, #60
	adds r0, #10
	bl 0x0200a168
	cmp r0, #0
	bne .L_02001794_2
	movs r1, #236
	movs r3, #140
	movs r0, #222
	lsls r1, r1, #15
	movs r2, #0
	lsls r3, r3, #15
	bl 0x0200a1b8
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02001794_2
	bl 0x02009704
	movs r3, #60
	str r3, [r5, #104]
	ldr r3, [pc, #136]
	adds r0, r5, #0
	str r3, [r5, #108]
	movs r1, #5
	bl 0x0200a1a0
.L_02001794_2:
	ldr r0, [r6]
	movs r1, #60
	adds r0, #50
	bl 0x0200a168
	cmp r0, #0
	bne .L_02001794_3
	movs r1, #171
	movs r3, #248
	movs r0, #222
	lsls r1, r1, #17
	movs r2, #0
	lsls r3, r3, #15
	bl 0x0200a1b8
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02001794_3
	bl 0x02009704
	movs r3, #60
	str r3, [r5, #104]
	ldr r3, [pc, #80]
	adds r0, r5, #0
	str r3, [r5, #108]
	movs r1, #5
	bl 0x0200a1a0
.L_02001794_3:
	ldr r0, [r6]
	movs r1, #60
	adds r0, #80
	bl 0x0200a168
	cmp r0, #0
	bne .L_02001794_4
	movs r3, #171
	movs r0, #222
	ldr r1, [pc, #52]
	movs r2, #0
	lsls r3, r3, #16
	bl 0x0200a1b8
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02001794_4
	bl 0x02009704
	movs r3, #60
	str r3, [r5, #104]
	ldr r3, [pc, #24]
	adds r0, r5, #0
	str r3, [r5, #108]
	movs r1, #5
	bl 0x0200a1a0
.L_02001794_4:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001e40
	.4byte 0x01cf0000
	.4byte 0x0200974d
	.4byte 0x01af0000
	.global Func_020018c4
	.thumb_func
Func_020018c4:
	push {lr}
	movs r0, #0
	bl 0x0200a248
	ldr r4, [r0, #8]
	asrs r2, r4, #19
	adds r3, r2, #0
	subs r3, #24
	cmp r3, #7
	bls .L_020018c4_0
	ldr r1, [r0, #16]
	asrs r3, r1, #19
	subs r3, #36
	cmp r3, #9
	bhi .L_020018c4_1
	adds r3, r2, #0
	subs r3, #22
	cmp r3, #9
	bhi .L_020018c4_1
.L_020018c4_0:
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200a210
	cmp r0, #0
	bne .L_020018c4_2
	ldr r3, [pc, #96]
	ldr r3, [r3]
	strb r0, [r3, #23]
	movs r0, #128
	lsls r0, r0, #2
	b .L_020018c4_3
.L_020018c4_1:
	movs r2, #232
	lsls r2, r2, #16
	cmp r4, r2
	ble .L_020018c4_4
	movs r2, #240
	ldr r3, [r0, #12]
	lsls r2, r2, #13
	cmp r3, r2
	ble .L_020018c4_4
	movs r3, #212
	lsls r3, r3, #16
	cmp r1, r3
	ble .L_020018c4_4
	ldr r3, [pc, #56]
	ldr r2, [r3]
	movs r0, #128
	movs r3, #0
	lsls r0, r0, #2
	strb r3, [r2, #23]
.L_020018c4_3:
	bl 0x0200a218
	ldr r0, [pc, #44]
	bl 0x0200a220
	b .L_020018c4_2
.L_020018c4_4:
	ldr r0, [pc, #36]
	bl 0x0200a210
	cmp r0, #0
	bne .L_020018c4_2
	ldr r3, [pc, #24]
	ldr r2, [r3]
	movs r3, #1
	ldr r0, [pc, #20]
	strb r3, [r2, #23]
	bl 0x0200a218
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200a220
.L_020018c4_2:
	pop {r0}
	bx r0
	.4byte 0x03001e70
	.4byte 0x00000201
	.global Func_02001960
	.thumb_func
Func_02001960:
	push {lr}
	ldr r0, [pc, #40]
	sub sp, #8
	bl 0x0200a218
	movs r0, #11
	movs r1, #3
	bl 0x0200a328
	movs r3, #15
	movs r2, #7
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #15
	movs r1, #6
	movs r2, #1
	movs r3, #1
	bl 0x0200a200
	sub sp, #-8
	pop {r0}
	bx r0
	.4byte 0x00000203
	.global Func_02001990
	.thumb_func
Func_02001990:
	push {lr}
	bl 0x0200a230
	movs r0, #0
	bl 0x0200a248
	movs r3, #192
	lsls r3, r3, #8
	strh r3, [r0, #6]
	movs r0, #123
	bl 0x0200a3e8
	bl 0x0200a388
	bl 0x0200a390
	movs r0, #8
	bl 0x0200a360
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020019bc
	.thumb_func
Func_020019bc:
	push {lr}
	bl 0x0200a230
	bl 0x0200a380
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #10
	ldr r2, [pc, #20]
	bl 0x0200a250
	movs r0, #0
	movs r1, #232
	movs r2, #204
	bl 0x0200a280
	bl 0x0200a238
	pop {r0}
	bx r0
	.4byte 0x00001999
	.global Func_020019e8
	.thumb_func
Func_020019e8:
	push {r5, r6, r7, lr}
	bl 0x0200a230
	movs r1, #160
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a320
	movs r0, #0
	ldr r1, [pc, #1012]
	ldr r2, [pc, #1016]
	bl 0x0200a250
	movs r2, #200
	ldr r1, [pc, #1012]
	movs r0, #0
	bl 0x0200a288
	bl 0x0200a358
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	ldr r1, [pc, #996]
	ldr r0, [pc, #1000]
	bl 0x0200a340
	movs r2, #164
	ldr r0, [pc, #996]
	movs r1, #0
	lsls r2, r2, #16
	movs r3, #1
	bl 0x0200a348
	ldr r7, [pc, #988]
	movs r3, #224
	ldr r1, [r7]
	lsls r3, r3, #1
	adds r2, r1, r3
	subs r3, #192
	str r3, [r2]
	adds r3, #200
	adds r2, r1, r3
	movs r3, #48
	str r3, [r2]
	bl 0x0200a380
	movs r0, #0
	bl 0x0200a2a8
	movs r0, #0
	movs r1, #1
	bl 0x0200a2b8
	movs r0, #3
	ldr r1, [pc, #920]
	ldr r2, [pc, #924]
	bl 0x0200a250
	movs r0, #0
	bl 0x0200a248
	cmp r0, #0
	beq .L_020019e8_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #3
	bl 0x0200a2b0
.L_020019e8_0:
	movs r0, #3
	ldr r1, [pc, #920]
	movs r2, #183
	bl 0x0200a290
	movs r1, #192
	movs r2, #0
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200a320
	movs r0, #19
	movs r1, #2
	bl 0x0200a2d0
	movs r1, #2
	movs r0, #20
	bl 0x0200a2d8
	movs r0, #40
	bl 0x0200a228
	ldr r0, [pc, #880]
	bl 0x0200a300
	movs r0, #19
	movs r1, #0
	movs r2, #10
	bl 0x0200a310
	movs r1, #224
	movs r2, #40
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200a320
	movs r0, #3
	movs r1, #3
	bl 0x0200a2c0
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #20
	bl 0x0200a338
	movs r0, #20
	bl 0x0200a228
	ldr r0, [pc, #832]
	movs r1, #0
	movs r2, #10
	bl 0x0200a310
	movs r1, #160
	movs r2, #40
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200a320
	movs r0, #3
	movs r1, #4
	bl 0x0200a2c0
	movs r2, #10
	ldr r0, [pc, #804]
	movs r1, #0
	bl 0x0200a310
	movs r1, #3
	movs r0, #20
	bl 0x0200a2c0
	movs r0, #20
	bl 0x0200a228
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #19
	bl 0x0200a338
	movs r0, #20
	bl 0x0200a228
	movs r0, #19
	movs r1, #0
	movs r2, #10
	bl 0x0200a310
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a320
	movs r1, #240
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200a320
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #6
	movs r2, #40
	bl 0x0200a320
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a320
	movs r1, #192
	movs r2, #40
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200a320
	movs r1, #2
	movs r0, #20
	bl 0x0200a2d8
	movs r0, #20
	bl 0x0200a228
	ldr r0, [pc, #680]
	movs r1, #0
	movs r2, #20
	bl 0x0200a310
	movs r1, #160
	movs r2, #20
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200a320
	movs r1, #3
	movs r0, #3
	bl 0x0200a2c0
	movs r0, #60
	bl 0x0200a228
	movs r0, #3
	ldr r1, [pc, #648]
	movs r2, #60
	bl 0x0200a330
	movs r0, #19
	ldr r1, [pc, #644]
	movs r2, #0
	bl 0x0200a330
	movs r2, #60
	movs r0, #20
	ldr r1, [pc, #632]
	bl 0x0200a330
	movs r1, #1
	movs r0, #19
	bl 0x0200a2d8
	movs r0, #20
	bl 0x0200a228
	movs r0, #19
	movs r1, #0
	movs r2, #10
	bl 0x0200a310
	movs r1, #224
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200a320
	movs r1, #160
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200a320
	movs r1, #224
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200a320
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #7
	movs r2, #80
	bl 0x0200a320
	ldr r0, [pc, #544]
	movs r1, #0
	movs r2, #20
	bl 0x0200a310
	movs r1, #240
	movs r0, #20
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a320
	movs r1, #224
	movs r0, #19
	lsls r1, r1, #7
	movs r2, #40
	bl 0x0200a320
	movs r1, #160
	movs r0, #19
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200a320
	movs r1, #192
	movs r0, #20
	lsls r1, r1, #6
	movs r2, #20
	bl 0x0200a320
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #20
	bl 0x0200a250
	movs r0, #20
	bl 0x0200a248
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	movs r1, #164
	strb r3, [r0]
	lsls r1, r1, #2
	movs r2, #166
	movs r0, #20
	bl 0x0200a290
	movs r0, #1
	bl 0x0200a228
	movs r0, #20
	bl 0x0200a248
	adds r0, #90
	ldrb r3, [r0]
	movs r6, #1
	orrs r3, r6
	strb r3, [r0]
	movs r0, #20
	bl 0x0200a228
	movs r2, #10
	ldr r0, [pc, #408]
	movs r1, #0
	bl 0x0200a310
	movs r1, #2
	movs r0, #3
	bl 0x0200a2d8
	movs r0, #40
	bl 0x0200a228
	movs r1, #160
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200a320
	ldr r0, [pc, #380]
.L_02001ca0:
	movs r1, #0
	movs r2, #40
	bl 0x0200a310
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #6
	movs r2, #20
	bl 0x0200a320
	movs r2, #10
	ldr r0, [pc, #368]
	movs r1, #0
	bl 0x0200a310
	movs r1, #129
	movs r0, #19
	lsls r1, r1, #1
	bl 0x0200a338
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #20
	bl 0x0200a338
	movs r0, #40
	bl 0x0200a228
	movs r1, #192
	movs r2, #20
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200a320
	movs r0, #3
	movs r1, #4
	bl 0x0200a2b8
	movs r2, #20
	ldr r0, [pc, #300]
	movs r1, #0
	bl 0x0200a310
	movs r0, #19
	movs r1, #1
	bl 0x0200a2d8
	movs r0, #19
	movs r1, #0
	movs r2, #10
	bl 0x0200a310
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #6
	movs r2, #40
	bl 0x0200a320
	movs r1, #192
	movs r2, #20
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200a320
	movs r1, #3
	movs r0, #3
	bl 0x0200a2c0
	movs r0, #20
	bl 0x0200a228
	movs r0, #20
	movs r1, #1
	bl 0x0200a2d8
	movs r2, #20
	ldr r0, [pc, #220]
	movs r1, #0
	bl 0x0200a310
	movs r1, #1
	movs r0, #3
	bl 0x0200a2d8
	movs r0, #20
	bl 0x0200a228
	movs r1, #160
	movs r2, #20
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200a320
	movs r0, #3
	movs r1, #3
	bl 0x0200a2c0
	ldr r0, [pc, #184]
	movs r1, #0
	movs r2, #80
	bl 0x0200a310
	movs r0, #19
	ldr r1, [pc, #176]
	movs r2, #0
	bl 0x0200a330
	movs r2, #60
	movs r0, #20
	ldr r1, [pc, #164]
	bl 0x0200a330
	movs r0, #19
	movs r1, #4
	bl 0x0200a2c0
	movs r2, #10
	movs r0, #19
	movs r1, #0
	bl 0x0200a310
	movs r0, #20
	movs r1, #4
	bl 0x0200a2b8
	ldr r0, [pc, #124]
	movs r1, #0
	movs r2, #20
	bl 0x0200a310
	movs r1, #129
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200a330
	movs r2, #40
	ldr r0, [pc, #104]
	movs r1, #0
	bl 0x0200a310
	movs r0, #19
	movs r1, #3
	bl 0x0200a2c0
	movs r0, #19
	movs r1, #0
	movs r2, #10
	bl 0x0200a310
	movs r1, #224
	movs r2, #20
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200a320
	movs r0, #20
	movs r1, #3
	bl 0x0200a2c0
	ldr r0, [pc, #52]
	movs r1, #0
	movs r2, #10
	bl 0x0200a310
	movs r1, #160
	movs r0, #3
	lsls r1, r1, #8
	b .L_02001ca0_0
	.2byte 0x0000
	.2byte 0x9999
	.2byte 0x0000
	.2byte 0x4ccc
	.2byte 0x0000
	.2byte 0x02b2
	.2byte 0x0000
	.2byte 0x1999
	.2byte 0x0000
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x02b2
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x02a1
	.2byte 0x0000
	.2byte 0x165b
	.2byte 0x0000
	.4byte 0x00004014
	.4byte 0x00002003
	.4byte 0x00000105
	.2byte 0x0101
	.2byte 0x0000
	.4byte 0x00004003
.L_02001ca0_0:
	movs r2, #60
	bl 0x0200a320
	movs r1, #224
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200a320
	movs r1, #160
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200a320
	movs r1, #192
	movs r2, #40
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200a320
	movs r0, #3
	movs r1, #3
	bl 0x0200a2c0
	movs r2, #10
	ldr r0, [pc, #176]
	movs r1, #0
	bl 0x0200a310
	movs r0, #19
	movs r1, #3
	bl 0x0200a2b8
	movs r1, #3
	movs r0, #20
	bl 0x0200a2c0
	movs r0, #40
	bl 0x0200a228
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #6
	movs r2, #20
	bl 0x0200a320
	ldr r0, [pc, #140]
	movs r1, #0
	movs r2, #20
	bl 0x0200a310
	movs r1, #160
	movs r2, #20
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200a320
	movs r1, #3
	movs r0, #0
	bl 0x0200a2c0
	movs r0, #20
	bl 0x0200a228
	movs r1, #172
	movs r0, #3
	lsls r1, r1, #2
	movs r2, #200
	bl 0x0200a290
	movs r1, #0
	movs r2, #0
	movs r0, #3
	bl 0x0200a2b0
	movs r0, #20
	bl 0x0200a248
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #161
	ands r5, r3
	lsls r1, r1, #2
	movs r2, #166
	strb r5, [r0]
	movs r0, #20
	bl 0x0200a290
	movs r0, #1
	bl 0x0200a228
	movs r0, #20
	bl 0x0200a248
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r6
	strb r3, [r0]
	ldr r3, [r7]
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #73
	str r2, [r3]
	ldr r0, [pc, #28]
	bl 0x0200a218
	ldr r0, [pc, #24]
	bl 0x0200a220
	bl 0x0200a238
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x00002003
	.4byte 0x00004003
	.4byte 0x0000082e
	.4byte 0x0000082d
	.global Func_02001f24
	.thumb_func
Func_02001f24:
	push {r5, lr}
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #100
	movs r0, #0
	ldrsh r1, [r2, r0]
	ldrh r3, [r2]
	cmp r1, #0
	beq .L_02001f24_0
	subs r3, #1
	strh r3, [r2]
	b .L_02001f24_1
.L_02001f24_0:
	adds r3, r5, #0
	adds r3, #90
	strb r1, [r3]
	ldr r3, [pc, #84]
	ldr r3, [r3]
	movs r2, #15
	lsrs r3, r3, #4
	ands r3, r2
	ldr r1, [pc, #76]
	lsls r3, r3, #1
	movs r0, #1
	ldrsh r3, [r1, r3]
	negs r0, r0
	cmp r3, r0
	bne .L_02001f24_2
	adds r0, r5, #0
	movs r1, #9
	bl 0x0200a1a0
	b .L_02001f24_1
.L_02001f24_2:
	ldrh r1, [r5, #6]
	subs r3, r3, r1
	lsls r3, r3, #16
	movs r2, #128
	asrs r3, r3, #16
	lsls r2, r2, #5
	cmp r3, r2
	ble .L_02001f24_3
	adds r3, r2, #0
.L_02001f24_3:
	ldr r2, [pc, #40]
	cmp r3, r2
	bge .L_02001f24_4
	adds r3, r2, #0
.L_02001f24_4:
	adds r3, r1, r3
	adds r0, r5, #0
	movs r1, #2
	strh r3, [r5, #6]
	bl 0x0200a1a0
	adds r0, r5, #0
	movs r1, #48
	bl 0x0200a1a8
.L_02001f24_1:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001ae8
	.4byte 0x0200a3f0
	.4byte 0xfffff000
	.global Func_02001fa4
	.thumb_func
Func_02001fa4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #408]
	movs r0, #250
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #20
	bl 0x0200a398
	adds r6, r0, #0
.L_02001fa4_8:
	ldr r3, [pc, #392]
	ldr r3, [r3]
	movs r2, #15
	lsrs r3, r3, #4
	ands r3, r2
	ldr r1, [pc, #388]
	lsls r3, r3, #1
	ldrsh r2, [r1, r3]
	str r2, [sp, #4]
	lsls r3, r2, #16
	ldr r2, [pc, #380]
	cmp r3, r2
	bne .L_02001fa4_0
	b .L_02001fa4_1
.L_02001fa4_0:
	bl 0x0200a230
	ldr r2, [r6, #8]
	ldr r1, [pc, #372]
	movs r3, #128
	lsls r3, r3, #12
	mov r11, r3
	ands r2, r1
	add r5, sp, #8
	add r2, r11
	str r2, [r5]
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	ands r3, r1
	add r3, r11
	str r3, [r5, #8]
	movs r0, #34
	mov r9, r3
	mov r10, r2
	adds r0, r0, r6
	mov r8, r0
	mov r1, r10
	mov r2, r9
	ldrb r0, [r0]
	bl 0x0200a1f8
	str r0, [sp, #0]
	movs r0, #128
	ldr r1, [sp, #4]
	lsls r0, r0, #13
	adds r2, r5, #0
	bl 0x0200a198
	mov r2, r8
	ldrb r0, [r2]
	ldr r1, [r5]
	ldr r2, [r5, #8]
	bl 0x0200a1f8
	adds r7, r0, #0
	cmp r7, #255
	beq .L_02001fa4_2
	mov r3, r8
	ldrb r0, [r3]
	ldr r1, [r5]
	ldr r2, [r5, #8]
	bl 0x0200a1f0
	ldr r3, [r6, #12]
	subs r0, r0, r3
	cmp r0, r11
	bgt .L_02001fa4_2
	movs r3, #128
	mov r0, r10
	mov r2, r9
	lsls r3, r3, #10
	str r0, [r5]
	str r2, [r5, #8]
	str r3, [r6, #48]
	ldr r3, [pc, #260]
	adds r2, r6, #0
	str r3, [r6, #52]
	adds r2, #100
	movs r3, #0
	strh r3, [r2]
	adds r0, r6, #0
	mov r3, r9
	ldr r2, [r6, #12]
	mov r1, r10
	bl 0x0200a1d0
	adds r0, r6, #0
	movs r1, #2
	bl 0x0200a1a0
	adds r0, r6, #0
	movs r1, #48
	bl 0x0200a1a8
	adds r0, r6, #0
	bl 0x0200a1d8
	ldr r3, [pc, #220]
	str r3, [r6, #108]
	b .L_02001fa4_3
.L_02001fa4_2:
	add r3, sp, #4
	ldrh r3, [r3]
	strh r3, [r6, #6]
	b .L_02001fa4_4
.L_02001fa4_7:
	mov r2, r8
	ldrb r0, [r2]
	ldr r1, [r5]
	ldr r2, [r5, #8]
	bl 0x0200a1f0
	ldr r3, [r6, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_02001fa4_5
	movs r3, #128
	lsls r3, r3, #10
	ldr r0, [r5]
	ldr r2, [r5, #8]
	str r3, [r6, #48]
	ldr r3, [pc, #168]
	str r3, [r6, #52]
	mov r10, r0
	ldr r3, [r5, #8]
	ldr r1, [r5]
	adds r0, r6, #0
	mov r9, r2
	ldr r2, [r5, #4]
	bl 0x0200a1d0
	adds r0, r6, #0
	bl 0x0200a1d8
	ldr r3, [sp, #0]
	cmp r7, r3
	bne .L_02001fa4_6
.L_02001fa4_3:
	movs r0, #128
	ldr r1, [sp, #4]
	add r2, sp, #8
	lsls r0, r0, #13
	bl 0x0200a198
	mov r2, r8
	ldrb r0, [r2]
	ldr r1, [r5]
	ldr r2, [r5, #8]
	bl 0x0200a1f8
	adds r7, r0, #0
	cmp r7, #255
	bne .L_02001fa4_7
.L_02001fa4_5:
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r6, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #52]
	ldr r2, [r6, #12]
	adds r0, r6, #0
	mov r1, r10
	mov r3, r9
	bl 0x0200a1d0
	adds r0, r6, #0
	bl 0x0200a1d8
	movs r0, #2
	bl 0x0200a170
	b .L_02001fa4_8
.L_02001fa4_6:
	movs r3, #0
	str r3, [r6, #108]
	adds r1, r6, #0
	adds r1, #90
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r6, #52]
.L_02001fa4_4:
	movs r0, #10
	bl 0x0200a170
	bl 0x0200a238
.L_02001fa4_1:
	sub sp, #-20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x03001ae8
	.4byte 0x0200a430
	.4byte 0xffff0000
	.4byte 0xfff00000
	.4byte 0x00001999
	.4byte 0x02009f25
	.include "games/THE BROKEN SEAL/SRC/FIELD/IMIRU_MURA/IMPORT.INC"
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0xe000c000
	.4byte 0xc000a000
	.4byte 0x20004000
	.4byte 0x40006000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0xffffc000
	.4byte 0xc000ffff
	.4byte 0xffff4000
	.4byte 0x4000ffff
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x0000c000
	.4byte 0xc0008000
	.4byte 0x00004000
	.4byte 0x40008000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffb000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00002800
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001400
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001400
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00950000
	.4byte 0x00000000
	.4byte 0x00740000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x009b0000
	.4byte 0x00000000
	.4byte 0x00870000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000010
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xffffd000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xffffe800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xffffe800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000010
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xffffd000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xffffe800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xffffe800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x0000000c
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0xffff999a
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000048
	.4byte 0x00000000
	.4byte 0x0000001b
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00b10000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00a10000
	.4byte 0x00000000
	.4byte 0x007c0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00860000
	.4byte 0x00000000
	.4byte 0x008b0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00870000
	.4byte 0x00000000
	.4byte 0x00bd0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00a50000
	.4byte 0x00000000
	.4byte 0x00cf0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00930000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00960000
	.4byte 0x00000000
	.4byte 0x010c0000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000022
	.4byte 0x020080d9
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x00ce0000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0xffff0000
	.4byte 0x0000017c
	.4byte 0x400001b1
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000120
	.4byte 0xc00001e8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000001e0
	.4byte 0x80000118
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000138
	.4byte 0x400001a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x000001a8
	.4byte 0x40000188
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x000001c8
	.4byte 0x400000f8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x00000178
	.4byte 0x400000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0007
	.4byte 0x00000078
	.4byte 0x400000d0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x00000040
	.4byte 0x40000168
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0009
	.4byte 0x000000e5
	.4byte 0x40000058
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x00000088
	.4byte 0xc00000f8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000090
	.4byte 0xc00000f2
	.4byte 0x00200000
	.4byte 0x01000034
	.4byte 0x00000118
	.4byte 0xffff0002
	.4byte 0x0000008f
	.4byte 0xc0000254
	.4byte 0x002a0000
	.4byte 0x010001a8
	.4byte 0x00000277
	.4byte 0xffff0003
	.4byte 0x0000018f
	.4byte 0xc0000142
	.4byte 0x01400000
	.4byte 0x0218003a
	.4byte 0x00000168
	.4byte 0xffff0004
	.4byte 0x00000190
	.4byte 0xc0000293
	.4byte 0x01330000
	.4byte 0x0221017e
	.4byte 0x000002b9
	.4byte 0xffff0005
	.4byte 0x00000081
	.4byte 0xc0000393
	.4byte 0x00320000
	.4byte 0x015e02c2
	.4byte 0x000003b5
	.4byte 0xffff0006
	.4byte 0x000001be
	.4byte 0x40000339
	.4byte 0x01920000
	.4byte 0x029e02e4
	.4byte 0x00000399
	.4byte 0xffff0007
	.4byte 0x000002af
	.4byte 0xc0000108
	.4byte 0x02520000
	.4byte 0x03260031
	.4byte 0x00000127
	.4byte 0xffff0008
	.4byte 0x000000b6
	.4byte 0x40000304
	.4byte 0x00320000
	.4byte 0x015e02c2
	.4byte 0x000003b5
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x00101033
	.4byte 0x00205033
	.4byte 0x00303033
	.4byte 0x00404033
	.4byte 0x00502033
	.4byte 0x00607033
	.4byte 0x00801034
	.4byte 0x00b09002
	.4byte 0x00c27002
	.4byte 0x00000033
	.4byte 0x00103032
	.4byte 0x00207032
	.4byte 0x00305032
	.4byte 0x00406032
	.4byte 0x00504032
	.4byte 0x00608033
	.4byte 0x00708032
	.4byte 0x00806033
	.4byte 0x000001ff
	.4byte 0xffff006c
	.4byte 0x0200a450
	.4byte 0x00630000
	.4byte 0x00000000
	.4byte 0x00ea0000
	.4byte 0x00003000
	.4byte 0xffff006b
	.4byte 0x00000002
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00003000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00023000
	.4byte 0xffff00ea
	.4byte 0x00000001
	.4byte 0x01060000
	.4byte 0x00000000
	.4byte 0x007c0000
	.4byte 0x00023000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00630000
	.4byte 0x00000000
	.4byte 0x00ea0000
	.4byte 0x0000f000
	.4byte 0xffff006b
	.4byte 0x00000002
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00003000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00023000
	.4byte 0xffff00ea
	.4byte 0x00000001
	.4byte 0x01060000
	.4byte 0x00000000
	.4byte 0x007c0000
	.4byte 0x00023000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000006f
	.4byte 0x00000001
	.4byte 0x006f0000
	.4byte 0x00000000
	.4byte 0x00d70000
	.4byte 0x00004000
	.4byte 0x00000067
	.4byte 0x00000001
	.4byte 0x00a60000
	.4byte 0x00000000
	.4byte 0x00740000
	.4byte 0x00024000
	.4byte 0x00000016
	.4byte 0x00000007
	.4byte 0x00ae0000
	.4byte 0x00000000
	.4byte 0x00ad0000
	.4byte 0x00024000
	.4byte 0x00000067
	.4byte 0x00000001
	.4byte 0x00760000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x0000c000
	.4byte 0x00000067
	.4byte 0x00000001
	.4byte 0x01720000
	.4byte 0x00000000
	.4byte 0x01d40000
	.4byte 0x00024000
	.4byte 0x00000067
	.4byte 0x00000001
	.4byte 0x01de0000
	.4byte 0x00000000
	.4byte 0x01d40000
	.4byte 0x00024000
	.4byte 0x00000066
	.4byte 0x00000001
	.4byte 0x01cc0000
	.4byte 0x00000000
	.4byte 0x010a0000
	.4byte 0x00005000
	.4byte 0x00000073
	.4byte 0x00000001
	.4byte 0x01b40000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0x00000077
	.4byte 0x00000001
	.4byte 0x00640000
	.4byte 0x00000000
	.4byte 0x033e0000
	.4byte 0x00014000
	.4byte 0x00000080
	.4byte 0x00000002
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x03700000
	.4byte 0x00004000
	.4byte 0x0000006c
	.4byte 0x00000001
	.4byte 0x00f90000
	.4byte 0x00000000
	.4byte 0x03400000
	.4byte 0x00010000
	.4byte 0x0000006b
	.4byte 0x00000001
	.4byte 0x02ae0000
	.4byte 0x00000000
	.4byte 0x00940000
	.4byte 0x00004000
	.4byte 0x00000066
	.4byte 0x00000001
	.4byte 0x02cd0000
	.4byte 0x00000000
	.4byte 0x00e50000
	.4byte 0x0000b000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x000000df
	.4byte 0x00000007
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x007c0000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff003c
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff003d
	.4byte 0x0000000c
	.4byte 0x00008c15
	.4byte 0x0200000b
	.4byte 0x02009961
	.4byte 0x0000c602
	.4byte 0xffff0033
	.4byte 0x020086c1
	.4byte 0x0000c602
	.4byte 0xffff0034
	.4byte 0x020086c1
	.4byte 0x0000c602
	.4byte 0xffff0035
	.4byte 0x020086c1
	.4byte 0x0000c602
	.4byte 0xffff0036
	.4byte 0x020086c1
	.4byte 0x0000c602
	.4byte 0xffff0037
	.4byte 0x020086c1
	.4byte 0x0000c602
	.4byte 0xffff0038
	.4byte 0x020086c1
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x02009991
	.4byte 0x00000002
	.4byte 0xffff005a
	.4byte 0x020081e1
	.4byte 0x00000000
	.4byte 0x08810008
	.4byte 0x0200821d
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0000163a
	.4byte 0x00000000
	.4byte 0x08810009
	.4byte 0x0200828d
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0000163b
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x020082b9
	.4byte 0x00008d15
	.4byte 0x08810008
	.4byte 0x02008255
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001640
	.4byte 0x00008d15
	.4byte 0x08810009
	.4byte 0x00001530
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001641
	.4byte 0x00008d15
	.4byte 0x0881000a
	.4byte 0x00001531
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001642
	.4byte 0x00000023
	.4byte 0x0f5f0064
	.4byte 0x00200009
	.4byte 0x00000013
	.4byte 0x0f600065
	.4byte 0x001000b6
	.4byte 0x00000083
	.4byte 0x0f610066
	.4byte 0x001000e5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x082c0008
	.4byte 0x00001532
	.4byte 0x00000000
	.4byte 0x082b0008
	.4byte 0x00001539
	.4byte 0x00000000
	.4byte 0x08810008
	.4byte 0x020085dd
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001643
	.4byte 0x00000000
	.4byte 0x082c0009
	.4byte 0x02008385
	.4byte 0x00000000
	.4byte 0x082b0009
	.4byte 0x00001538
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008385
	.4byte 0x00000000
	.4byte 0x182b000a
	.4byte 0x0200865d
	.4byte 0x00000000
	.4byte 0x0881000b
	.4byte 0x0000153c
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001649
	.4byte 0x00000000
	.4byte 0x0881000c
	.4byte 0x02008511
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020087a5
	.4byte 0x00000000
	.4byte 0x0881000d
	.4byte 0x00001544
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0200888d
	.4byte 0x00000000
	.4byte 0x0881000f
	.4byte 0x00001548
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02008975
	.4byte 0x00000000
	.4byte 0x0881000e
	.4byte 0x02008975
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001650
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x02008a3d
	.4byte 0x00000000
	.4byte 0x08810011
	.4byte 0x0000154c
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001656
	.4byte 0x00000000
	.4byte 0x08810012
	.4byte 0x02008531
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001657
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x02008abd
	.4byte 0x00000000
	.4byte 0x08810014
	.4byte 0x02008551
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001672
	.4byte 0x00008d15
	.4byte 0x08810008
	.4byte 0x02008609
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001647
	.4byte 0x00008d15
	.4byte 0x082c0409
	.4byte 0x02008385
	.4byte 0x00008d15
	.4byte 0x082b0009
	.4byte 0x0000153a
	.4byte 0x00008d15
	.4byte 0x08810009
	.4byte 0x0000156e
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001648
	.4byte 0x00008d15
	.4byte 0x182b000a
	.4byte 0x02008669
	.4byte 0x00008d15
	.4byte 0x0881000b
	.4byte 0x0000153d
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000164a
	.4byte 0x00008d15
	.4byte 0x0881000c
	.4byte 0x00001542
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000164c
	.4byte 0x00008d15
	.4byte 0x0881000d
	.4byte 0x00001545
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000164e
	.4byte 0x00008d15
	.4byte 0x0881000f
	.4byte 0x0000154a
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001651
	.4byte 0x00008d15
	.4byte 0x0881000e
	.4byte 0x00001549
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001652
	.4byte 0x00008d15
	.4byte 0x08810010
	.4byte 0x00001550
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001658
	.4byte 0x00008d15
	.4byte 0x08810011
	.4byte 0x00001551
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001659
	.4byte 0x00008d15
	.4byte 0x08810012
	.4byte 0x00001552
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x0000165a
	.4byte 0x00008d15
	.4byte 0x08810013
	.4byte 0x02008689
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001673
	.4byte 0x00008d15
	.4byte 0x08810014
	.4byte 0x020085a5
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001674
	.4byte 0x0000c403
	.4byte 0x0881000a
	.4byte 0x020087a5
	.4byte 0x0000c403
	.4byte 0x0881000b
	.4byte 0x0200888d
	.4byte 0x00000013
	.4byte 0x0f260064
	.4byte 0x001000b9
	.4byte 0x00000033
	.4byte 0x0f620065
	.4byte 0x001000e3
	.4byte 0x00000173
	.4byte 0x0f630066
	.4byte 0x001000c4
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x00402999
	.4byte 0x00000173
	.4byte 0xffff0066
	.4byte 0x0040299a
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
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00250028
	.4byte 0x00020004
	.4byte 0x00280005
	.4byte 0x00040027
	.4byte 0x00050002
	.4byte 0x0023ffff
	.4byte 0x00020023
	.4byte 0x00050002
	.4byte 0x00230021
	.4byte 0x00020002
	.4byte 0xffff0005
	.4byte 0x00250023
	.4byte 0x00020002
	.4byte 0x00210005
	.4byte 0x00020025
	.4byte 0x00050002
	.4byte 0x0023ffff
	.4byte 0x0002002b
	.4byte 0x00050002
	.4byte 0x002b0021
	.4byte 0x00020002
	.4byte 0xffff0005
	.4byte 0x00270023
	.4byte 0x00020002
	.4byte 0x00210005
	.4byte 0x00020027
	.4byte 0x00050002
	.4byte 0x002affff
	.4byte 0x00020030
	.4byte 0x00050002
	.4byte 0x00300028
	.4byte 0x00020002
	.4byte 0xffff0005
	.4byte 0x002e002a
	.4byte 0x00020002
	.4byte 0x00280005
	.4byte 0x0002002e
	.4byte 0x00050002
	.4byte 0x0000ffff
	.4byte 0x0200b14c
	.4byte 0x0200b136
	.4byte 0x0200b162
	.4byte 0x0200b178
	.4byte 0x0200b120
	.4byte 0x0200b0f4
	.4byte 0x00180032
	.4byte 0x00160039
	.4byte 0x000d003b
	.4byte 0x000a0036
	.4byte 0x00080026
	.4byte 0x00130022
	.4byte 0x007d0001
	.4byte 0x00020001
	.4byte 0x0000000a
	.4byte 0x0001007d
	.4byte 0x000a0002
	.4byte 0x0001ffff
	.4byte 0x0001007d
	.4byte 0x000a0002
	.4byte 0x007d0002
	.4byte 0x00020001
	.4byte 0xffff000a
