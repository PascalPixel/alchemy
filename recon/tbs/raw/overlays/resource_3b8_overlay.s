.syntax unified
	.thumb
	.section .text.x020080c8,"ax",%progbits
	.global Func_020000c8
	.thumb_func
Func_020000c8:
	push {r5, r6, lr}
	ldr r5, [pc, #56]
	adds r6, r0, #0
	adds r0, r5, #0
	bl 0x0200c498
	movs r1, #0
	adds r0, r6, #0
	bl 0x0200c4a0
	movs r0, #0
	movs r1, #0
	bl 0x0200c3f8
	cmp r0, #0
	bne .L_020000c8_0
	adds r0, r5, #1
	bl 0x0200c498
	b .L_020000c8_1
.L_020000c8_0:
	adds r0, r5, #2
	bl 0x0200c498
.L_020000c8_1:
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200c4a8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x00001ff1
	.global Func_02000108
	.thumb_func
Func_02000108:
	push {r5, r6, lr}
	adds r5, r0, #0
	movs r0, #0
	bl 0x0200c400
	movs r2, #128
	ldrh r3, [r0, #6]
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, [pc, #56]
	ands r3, r2
	lsls r3, r3, #16
	asrs r6, r3, #16
	bl 0x0200c3e0
	bl 0x0200c508
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200c3a0
	cmp r0, #0
	bne .L_02000108_0
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200c3a8
	ldr r0, [pc, #24]
	bl 0x0200c3b0
	ldr r0, [pc, #20]
	bl 0x0200c498
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200c4a8
	b .L_02000108_1
	.4byte 0xffffc000
	.4byte 0x00000969
	.4byte 0x00001ff7
.L_02000108_1:
	movs r0, #10
	bl 0x0200c3d8
	movs r2, #128
	lsls r3, r6, #16
	lsls r2, r2, #23
	movs r6, #128
	lsls r6, r6, #7
	cmp r3, r2
	bne .L_02000108_2
	movs r0, #0
	movs r1, #40
	movs r2, #104
	bl 0x0200c438
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200c4b0
.L_02000108_2:
	movs r1, #128
	movs r2, #128
	adds r0, r5, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200c408
	movs r2, #48
	adds r0, r5, #0
	movs r1, #0
	negs r2, r2
	bl 0x0200c530
	adds r0, r5, #0
	movs r1, #64
	movs r2, #0
	bl 0x0200c530
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200c4b0
	b .L_02000108_3
.L_02000108_0:
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200c3b0
	ldr r0, [pc, #152]
	bl 0x0200c3a8
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200c4b0
	movs r0, #0
	movs r1, #120
	movs r2, #96
	bl 0x0200c438
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #0
	bl 0x0200c4b0
	movs r0, #20
	bl 0x0200c3d8
	ldr r6, [pc, #112]
	adds r0, r6, #0
	bl 0x0200c498
	movs r1, #0
	adds r0, r5, #0
	bl 0x0200c4a0
	movs r0, #0
	movs r1, #0
	bl 0x0200c3f8
	cmp r0, #0
	bne .L_02000108_4
	adds r0, r6, #1
	bl 0x0200c498
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200c4a8
	b .L_02000108_5
.L_02000108_4:
	adds r0, r6, #2
	bl 0x0200c498
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200c4a8
.L_02000108_5:
	movs r0, #10
	bl 0x0200c3d8
	movs r1, #3
	adds r0, r5, #0
	bl 0x0200c468
	movs r0, #20
	bl 0x0200c3d8
	movs r1, #64
	adds r0, r5, #0
	negs r1, r1
	movs r2, #0
	bl 0x0200c530
	adds r0, r5, #0
	movs r1, #0
	movs r2, #48
	bl 0x0200c530
.L_02000108_3:
	bl 0x0200c3e8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x00000969
	.4byte 0x00001ff8
	.global Func_02000264
	.thumb_func
Func_02000264:
	push {r5, r6, lr}
	adds r5, r0, #0
	bl 0x0200c3e0
	bl 0x0200c508
	ldr r0, [pc, #196]
	bl 0x0200c3a0
	cmp r0, #0
	bne .L_02000264_0
	ldr r0, [pc, #188]
	bl 0x0200c3a8
	ldr r0, [pc, #184]
	bl 0x0200c3a8
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200c4b0
	movs r0, #0
	movs r1, #120
	movs r2, #96
	bl 0x0200c438
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #0
	bl 0x0200c4b0
	movs r0, #20
	bl 0x0200c3d8
	ldr r6, [pc, #144]
	adds r0, r6, #0
	bl 0x0200c498
	movs r1, #0
	adds r0, r5, #0
	bl 0x0200c4a0
	movs r0, #0
	movs r1, #0
	bl 0x0200c3f8
	cmp r0, #0
	bne .L_02000264_1
	movs r0, #10
	bl 0x0200c3d8
	adds r0, r6, #1
	bl 0x0200c498
	b .L_02000264_2
.L_02000264_1:
	adds r0, r6, #2
	bl 0x0200c498
.L_02000264_2:
	movs r1, #0
	adds r0, r5, #0
	bl 0x0200c4a8
	movs r0, #10
	bl 0x0200c3d8
	movs r1, #3
	adds r0, r5, #0
	bl 0x0200c468
	movs r0, #20
	bl 0x0200c3d8
	movs r1, #128
	movs r2, #128
	adds r0, r5, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200c408
	movs r1, #64
	adds r0, r5, #0
	negs r1, r1
	movs r2, #0
	bl 0x0200c530
	adds r0, r5, #0
	movs r1, #0
	movs r2, #48
	bl 0x0200c530
	b .L_02000264_3
.L_02000264_0:
	ldr r0, [pc, #32]
	bl 0x0200c498
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200c4a0
.L_02000264_3:
	bl 0x0200c3e8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x00000966
	.4byte 0x00000967
	.4byte 0x00002241
	.4byte 0x00002245
	.global Func_02000348
	.thumb_func
Func_02000348:
	push {r5, r6, lr}
	ldr r6, [pc, #92]
	adds r5, r0, #0
	adds r0, r6, #0
	bl 0x0200c498
	movs r1, #0
	adds r0, r5, #0
	bl 0x0200c4a0
	movs r0, #0
	movs r1, #0
	bl 0x0200c3f8
	cmp r0, #0
	bne .L_02000348_0
	movs r0, #10
	bl 0x0200c3d8
	movs r1, #129
	adds r0, r5, #0
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200c4b8
	adds r0, r6, #1
	bl 0x0200c498
	b .L_02000348_1
.L_02000348_0:
	movs r0, #10
	bl 0x0200c3d8
	adds r0, r5, #0
	ldr r1, [pc, #32]
	movs r2, #40
	bl 0x0200c4b8
	adds r0, r6, #2
	bl 0x0200c498
.L_02000348_1:
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200c4a8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00002006
	.4byte 0x00000105
	.section .text.x02008524,"ax",%progbits
	.global Func_02000524
	.thumb_func
Func_02000524:
	push {r5, r6, lr}
	ldr r5, [pc, #56]
	adds r6, r0, #0
	adds r0, r5, #0
	bl 0x0200c498
	movs r1, #0
	adds r0, r6, #0
	bl 0x0200c4a0
	movs r0, #0
	movs r1, #0
	bl 0x0200c3f8
	cmp r0, #0
	bne .L_02000524_0
	adds r0, r5, #1
	bl 0x0200c498
	b .L_02000524_1
.L_02000524_0:
	adds r0, r5, #2
	bl 0x0200c498
.L_02000524_1:
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200c4a8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x000022a8
	.global Func_02000564
	.thumb_func
Func_02000564:
	push {r5, r6, lr}
	ldr r5, [pc, #56]
	adds r6, r0, #0
	adds r0, r5, #0
	bl 0x0200c498
	movs r1, #0
	adds r0, r6, #0
	bl 0x0200c4a0
	movs r0, #0
	movs r1, #0
	bl 0x0200c3f8
	cmp r0, #0
	bne .L_02000564_0
	adds r0, r5, #1
	bl 0x0200c498
	b .L_02000564_1
.L_02000564_0:
	adds r0, r5, #2
	bl 0x0200c498
.L_02000564_1:
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200c4a8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x000022ab
	.global Func_020005a4
	.thumb_func
Func_020005a4:
	push {r5, lr}
	bl 0x0200c3e0
	bl 0x0200c508
	ldr r5, [pc, #188]
	adds r0, r5, #0
	bl 0x0200c498
	movs r0, #1
	movs r1, #0
	negs r0, r0
	bl 0x0200c4a8
	movs r0, #10
	bl 0x0200c3d8
	movs r1, #2
	movs r0, #14
	bl 0x0200c480
	movs r0, #30
	bl 0x0200c3d8
	movs r0, #0
	movs r1, #14
	movs r2, #30
	bl 0x0200c488
	movs r1, #0
	movs r0, #14
	bl 0x0200c4a0
	movs r0, #0
	movs r1, #0
	bl 0x0200c3f8
	cmp r0, #0
	beq .L_020005a4_0
	adds r0, r5, #2
	bl 0x0200c498
	movs r0, #14
	movs r1, #0
	bl 0x0200c4a8
	b .L_020005a4_1
.L_020005a4_0:
	movs r0, #20
	bl 0x0200c3d8
	adds r0, r5, #3
	bl 0x0200c498
	movs r1, #0
	movs r0, #14
	bl 0x0200c4a8
	movs r0, #10
	bl 0x0200c3d8
	movs r1, #3
	movs r0, #0
	bl 0x0200c468
	movs r0, #30
	bl 0x0200c3d8
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #0
	bl 0x0200c4b0
	movs r0, #30
	bl 0x0200c3d8
	movs r2, #0
	movs r0, #16
	movs r1, #0
	bl 0x0200c458
	movs r0, #205
	movs r1, #3
	bl 0x0200c500
	movs r0, #0
	movs r1, #1
	bl 0x0200c460
	movs r0, #205
	movs r1, #0
	bl 0x0200c3f0
	ldr r0, [pc, #16]
	bl 0x0200c3a8
.L_020005a4_1:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00002352
	.4byte 0x00000f31
	.global FieldScene_RunScene3b8SequenceB
	.thumb_func
FieldScene_RunScene3b8SequenceB:
	push {r5, r6, lr}
	bl 0x0200c3e0
	ldr r0, [pc, #104]
	bl 0x0200c498
	movs r0, #0
	bl 0x0200c400
	movs r5, #0
	adds r0, #84
	strb r5, [r0]
	movs r0, #10
	bl 0x0200c400
	adds r0, #84
	strb r5, [r0]
	movs r0, #1
	bl 0x0200c340
	movs r5, #128
	ldr r3, [pc, #60]
	lsls r5, r5, #19
	movs r0, #1
	movs r1, #0
	strh r3, [r5]
	negs r0, r0
	bl 0x0200c4a8
	ldr r3, [pc, #48]
	movs r0, #0
	strh r3, [r5]
	bl 0x0200c400
	movs r5, #1
	adds r0, #84
	strb r5, [r0]
	movs r0, #10
	bl 0x0200c400
	adds r0, #84
	strb r5, [r0]
	movs r1, #31
	movs r0, #0
	bl 0x0200c460
	movs r0, #0
	bl 0x0200c400
	movs r1, #0
	b .L_02000674_0
	.2byte 0x0000
	.4byte 0x00001140
	.4byte 0x00000140
	.4byte 0x00002280
.L_02000674_0:
	bl 0x0200c378
	movs r1, #240
	movs r2, #208
	movs r0, #1
	lsls r1, r1, #15
	lsls r2, r2, #15
	bl 0x0200c458
	movs r1, #208
	movs r2, #160
	movs r0, #3
	lsls r1, r1, #15
	lsls r2, r2, #15
	bl 0x0200c458
	movs r1, #240
	movs r2, #240
	movs r0, #2
	lsls r1, r1, #15
	lsls r2, r2, #15
	bl 0x0200c458
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200c4b0
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl 0x0200c4b0
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #2
	movs r2, #0
	bl 0x0200c4b0
	ldr r6, [pc, #932]
	movs r5, #228
	ldr r2, [r6]
	movs r3, #60
	lsls r5, r5, #1
	str r3, [r2, r5]
	bl 0x0200c4f0
	bl 0x0200c4f8
	movs r0, #20
	bl 0x0200c3d8
	ldr r2, [r6]
	movs r3, #24
	str r3, [r2, r5]
	movs r1, #128
	movs r2, #128
	movs r0, #3
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200c408
	movs r0, #3
	movs r1, #16
	movs r2, #0
	bl 0x0200c530
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #3
	bl 0x0200c4b0
	movs r0, #20
	bl 0x0200c3d8
	movs r1, #2
	movs r0, #3
	bl 0x0200c480
	movs r0, #30
	bl 0x0200c3d8
	movs r1, #0
	movs r0, #3
	bl 0x0200c4a8
	movs r0, #10
	bl 0x0200c3d8
	movs r0, #0
	bl 0x0200c400
	ldr r5, [pc, #828]
	ldr r3, [r0, #16]
	adds r3, r3, r5
	str r3, [r0, #16]
	movs r0, #0
	bl 0x0200c400
	ldr r3, [r0, #64]
	adds r3, r3, r5
	str r3, [r0, #64]
	movs r1, #32
	movs r0, #0
	bl 0x0200c460
	movs r0, #40
	bl 0x0200c3d8
	movs r1, #34
	movs r0, #0
	bl 0x0200c468
	movs r0, #30
	bl 0x0200c3d8
	movs r1, #33
	movs r0, #0
	bl 0x0200c460
	movs r0, #50
	bl 0x0200c3d8
	movs r1, #2
	movs r0, #1
	bl 0x0200c480
	movs r0, #30
	bl 0x0200c3d8
	movs r1, #0
	movs r0, #1
	bl 0x0200c4a8
	movs r0, #10
	bl 0x0200c3d8
	ldr r1, [pc, #740]
	movs r2, #60
	movs r0, #0
	bl 0x0200c4b8
	movs r0, #20
	bl 0x0200c3d8
	movs r1, #129
	movs r2, #60
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200c4b8
	movs r1, #0
	movs r0, #1
	bl 0x0200c4a8
	movs r0, #10
	bl 0x0200c3d8
	movs r1, #4
	movs r0, #1
	bl 0x0200c468
	movs r0, #20
	bl 0x0200c3d8
	movs r1, #0
	movs r0, #1
	bl 0x0200c4a8
	movs r0, #10
	bl 0x0200c3d8
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #80
	bl 0x0200c4b8
	movs r1, #131
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200c4b8
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #2
	bl 0x0200c4b0
	movs r0, #30
	bl 0x0200c3d8
	movs r1, #4
	movs r0, #2
	bl 0x0200c468
	movs r0, #20
	bl 0x0200c3d8
	movs r1, #0
	movs r0, #2
	bl 0x0200c4a8
	movs r0, #10
	bl 0x0200c3d8
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #1
	bl 0x0200c4b0
	movs r0, #30
	bl 0x0200c3d8
	movs r1, #2
	movs r0, #1
	bl 0x0200c480
	movs r0, #45
	bl 0x0200c3d8
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200c4b0
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #2
	bl 0x0200c4b0
	movs r0, #30
	bl 0x0200c3d8
	movs r1, #0
	movs r0, #1
	bl 0x0200c4a0
	movs r0, #1
	negs r0, r0
	movs r1, #0
	bl 0x0200c3f8
	cmp r0, #0
	beq .L_02000674_1
	movs r0, #10
	bl 0x0200c3d8
	movs r1, #34
	movs r0, #0
	bl 0x0200c468
	movs r0, #20
	bl 0x0200c3d8
	movs r1, #3
	movs r0, #1
	bl 0x0200c468
	movs r0, #20
	bl 0x0200c3d8
	movs r1, #0
	movs r0, #1
	bl 0x0200c4a8
	movs r0, #10
	bl 0x0200c3d8
	movs r1, #33
	movs r0, #0
	bl 0x0200c468
	movs r0, #30
	bl 0x0200c3d8
	movs r1, #3
	movs r0, #1
	bl 0x0200c468
	movs r0, #20
	bl 0x0200c3d8
	movs r0, #1
	movs r1, #0
	bl 0x0200c4a8
	ldr r2, [r6]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02000674_2
.L_02000674_1:
	movs r0, #10
	bl 0x0200c3d8
	movs r1, #33
	movs r0, #0
	bl 0x0200c468
	ldr r2, [r6]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #2
	strh r3, [r2]
	movs r0, #30
	bl 0x0200c3d8
	movs r1, #3
	movs r0, #1
	bl 0x0200c468
	movs r0, #20
	bl 0x0200c3d8
	movs r0, #1
	movs r1, #0
	bl 0x0200c4a8
.L_02000674_2:
	movs r0, #10
	bl 0x0200c3d8
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200c408
	movs r1, #16
	movs r0, #1
	negs r1, r1
	movs r2, #0
	bl 0x0200c530
	movs r1, #0
	movs r2, #0
	movs r0, #1
	bl 0x0200c4b0
	movs r0, #35
	bl 0x0200c3d8
	movs r0, #0
	movs r1, #6
	movs r2, #0
	bl 0x0200c470
	movs r0, #0
	ldr r1, [pc, #308]
	ldr r2, [pc, #308]
	bl 0x0200c408
	movs r1, #32
	movs r2, #0
	negs r1, r1
	movs r0, #0
	bl 0x0200c530
	movs r0, #0
	bl 0x0200c400
	movs r1, #1
	bl 0x0200c378
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200c4b0
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #2
	bl 0x0200c4b0
	movs r0, #40
	bl 0x0200c3d8
	movs r1, #3
	movs r0, #0
	bl 0x0200c460
	movs r0, #30
	bl 0x0200c3d8
	movs r0, #2
	movs r1, #3
	bl 0x0200c460
	movs r0, #1
	movs r1, #3
	bl 0x0200c460
	movs r1, #3
	movs r0, #3
	bl 0x0200c468
	movs r0, #30
	bl 0x0200c3d8
	movs r0, #1
	ldr r1, [pc, #208]
	ldr r2, [pc, #208]
	bl 0x0200c408
	movs r0, #3
	ldr r1, [pc, #196]
	ldr r2, [pc, #200]
	bl 0x0200c408
	movs r0, #2
	ldr r1, [pc, #188]
	ldr r2, [pc, #188]
	bl 0x0200c408
	movs r0, #1
	movs r1, #2
	bl 0x0200c460
	movs r0, #0
	bl 0x0200c400
	cmp r0, #0
	beq .L_02000674_3
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x0200c428
.L_02000674_3:
	movs r0, #1
	bl 0x0200c450
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200c458
	movs r0, #3
	movs r1, #2
	bl 0x0200c460
	movs r0, #0
	bl 0x0200c400
	cmp r0, #0
	beq .L_02000674_4
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #3
	bl 0x0200c428
.L_02000674_4:
	movs r0, #3
	bl 0x0200c450
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl 0x0200c458
	movs r0, #2
	movs r1, #2
	bl 0x0200c460
	movs r0, #0
	bl 0x0200c400
	cmp r0, #0
	beq .L_02000674_5
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #2
	bl 0x0200c428
.L_02000674_5:
	movs r0, #2
	bl 0x0200c450
	movs r1, #0
	movs r2, #0
	movs r0, #2
	bl 0x0200c458
	movs r0, #10
	bl 0x0200c3d8
	bl 0x0200c3e8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0xfffd0000
	.4byte 0x00000105
	.4byte 0x0001e666
	.4byte 0x0000f333
	.4byte 0x00013333
	.4byte 0x00009999
	.section .text.x0200be40,"ax",%progbits
	.global Func_02003e40
	.thumb_func
Func_02003e40:
	push {r5, lr}
	ldr r0, [pc, #312]
	bl 0x0200c3a8
	bl 0x0200c3e0
	bl 0x0200c508
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200c4b0
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200c4b0
	movs r2, #136
	movs r0, #0
	movs r1, #200
	lsls r2, r2, #1
	bl 0x0200c438
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #0
	bl 0x0200c4b0
	movs r0, #20
	bl 0x0200c3d8
	ldr r5, [pc, #248]
	adds r0, r5, #0
	bl 0x0200c498
	movs r1, #0
	movs r0, #8
	bl 0x0200c4a0
	movs r0, #0
	movs r1, #0
	bl 0x0200c3f8
	cmp r0, #0
	bne .L_02003e40_0
	movs r0, #20
	bl 0x0200c3d8
	adds r0, r5, #1
	bl 0x0200c498
	movs r0, #8
	movs r1, #0
	bl 0x0200c4a8
	b .L_02003e40_1
.L_02003e40_0:
	movs r0, #20
	bl 0x0200c3d8
	adds r0, r5, #2
	bl 0x0200c498
	movs r1, #0
	movs r0, #8
	bl 0x0200c4a8
	movs r0, #20
	bl 0x0200c3d8
	movs r0, #8
	movs r1, #9
	movs r2, #60
	bl 0x0200c490
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #9
	bl 0x0200c4b0
	movs r0, #40
	bl 0x0200c3d8
	movs r1, #2
	movs r0, #9
	bl 0x0200c480
	movs r0, #30
	bl 0x0200c3d8
	movs r2, #30
	movs r0, #8
	movs r1, #9
	bl 0x0200c490
	movs r1, #3
	movs r0, #9
	bl 0x0200c468
	movs r0, #30
	bl 0x0200c3d8
	movs r1, #129
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #50
	bl 0x0200c4b8
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200c4b0
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #9
	bl 0x0200c4b0
	movs r0, #20
	bl 0x0200c3d8
	movs r1, #4
	movs r0, #8
	bl 0x0200c468
	movs r0, #20
	bl 0x0200c3d8
	movs r1, #0
	movs r0, #8
	bl 0x0200c4a8
	movs r0, #10
	bl 0x0200c3d8
	movs r1, #2
	movs r0, #8
	bl 0x0200c480
	movs r0, #20
	bl 0x0200c3d8
	movs r0, #8
	movs r1, #0
	bl 0x0200c4a8
.L_02003e40_1:
	bl 0x0200c3e8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000096c
	.4byte 0x00002233
	.section .rodata,"a",%progbits
	.global TorebiKyuden_CellSteps
TorebiKyuden_CellSteps:
	.4byte 0x001c0019
	.4byte 0x00030001
	.4byte 0x001a0005
	.4byte 0x0001001c
	.4byte 0x00050003
	.4byte 0x001c001b
	.4byte 0x00030001
	.4byte 0xffff0005
	.global TorebiKyuden_MiddleActionScript
TorebiKyuden_MiddleActionScript:
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global gTorebiKyudenEntrancesOther
gTorebiKyudenEntrancesOther:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000300
	.4byte 0xc00000c8
	.4byte 0x02000000
	.4byte 0x04000000
	.4byte 0x000000e0
	.4byte 0xffff0002
	.4byte 0x000002d8
	.4byte 0x00000058
	.4byte 0x02000000
	.4byte 0x04000000
	.4byte 0x000000e0
	.4byte 0xffff0003
	.4byte 0x00000328
	.4byte 0x80000058
	.4byte 0x02000000
	.4byte 0x04000000
	.4byte 0x000000e0
	.4byte 0xffff0004
	.4byte 0x00000258
	.4byte 0x00000098
	.4byte 0x02000000
	.4byte 0x04000000
	.4byte 0x000000e0
	.4byte 0xffff0005
	.4byte 0x000003a8
	.4byte 0x80000098
	.4byte 0x02000000
	.4byte 0x04000000
	.4byte 0x000000e0
	.4byte 0xffff0006
	.4byte 0x00000248
	.4byte 0x40000038
	.4byte 0x02000000
	.4byte 0x04000000
	.4byte 0x000000e0
	.4byte 0xffff0007
	.4byte 0x000003b8
	.4byte 0x40000038
	.4byte 0x02000000
	.4byte 0x04000000
	.4byte 0x000000e0
	.4byte 0xffff0008
	.4byte 0x00000048
	.4byte 0xc00000d8
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x000000f0
	.4byte 0xffff0009
	.4byte 0x000001b8
	.4byte 0xc00000d8
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x000000f0
	.4byte 0xffff000a
	.4byte 0x00000048
	.4byte 0x000003c8
	.4byte 0x00000000
	.4byte 0x00f00360
	.4byte 0x00000400
	.4byte 0xffff000b
	.4byte 0x000000a8
	.4byte 0x400003a8
	.4byte 0x00000000
	.4byte 0x00f00360
	.4byte 0x00000400
	.4byte 0xffff000c
	.4byte 0x000001a8
	.4byte 0x800002a8
	.4byte 0x01480000
	.4byte 0x02380258
	.4byte 0x000002f8
	.4byte 0xffff000d
	.4byte 0x000001d8
	.4byte 0x400002b8
	.4byte 0x01480000
	.4byte 0x02380258
	.4byte 0x000002f8
	.4byte 0xffff0010
	.4byte 0x00000198
	.4byte 0x00000338
	.4byte 0x01400000
	.4byte 0x023002d8
	.4byte 0x00000378
	.4byte 0xffff0011
	.4byte 0x000001c8
	.4byte 0x40000328
	.4byte 0x01400000
	.4byte 0x023002d8
	.4byte 0x00000378
	.4byte 0xffff0012
	.4byte 0x00000338
	.4byte 0xc0000288
	.4byte 0x03000000
	.4byte 0x040001e0
	.4byte 0x000002a0
	.4byte 0xffff0014
	.4byte 0x00000368
	.4byte 0x800003a8
	.4byte 0x03100000
	.4byte 0x04000360
	.4byte 0x00000400
	.4byte 0xffff0015
	.4byte 0x00000378
	.4byte 0x800003c8
	.4byte 0x03100000
	.4byte 0x04000360
	.4byte 0x00000400
	.4byte 0xffff0016
	.4byte 0x000002f8
	.4byte 0x000003a8
	.4byte 0x02600000
	.4byte 0x03500360
	.4byte 0x00000400
	.4byte 0xffff0017
	.4byte 0x000002e8
	.4byte 0x000003c8
	.4byte 0x02600000
	.4byte 0x03500360
	.4byte 0x00000400
	.4byte 0xffff0018
	.4byte 0x000001b8
	.4byte 0x80000248
	.4byte 0x01100000
	.4byte 0x031000f0
	.4byte 0x00000260
	.4byte 0xffff0019
	.4byte 0x00000268
	.4byte 0x00000248
	.4byte 0x01100000
	.4byte 0x031000f0
	.4byte 0x00000260
	.4byte 0xffff001a
	.4byte 0x00000148
	.4byte 0x80000138
	.4byte 0x01100000
	.4byte 0x031000f0
	.4byte 0x00000260
	.4byte 0xffff001b
	.4byte 0x000002d8
	.4byte 0x00000138
	.4byte 0x01100000
	.4byte 0x031000f0
	.4byte 0x00000260
	.4byte 0xffff001c
	.4byte 0x00000138
	.4byte 0x80000128
	.4byte 0x01100000
	.4byte 0x031000f0
	.4byte 0x00000260
	.4byte 0xffff001d
	.4byte 0x000002e8
	.4byte 0x00000128
	.4byte 0x01100000
	.4byte 0x031000f0
	.4byte 0x00000260
	.4byte 0xffff001e
	.4byte 0x000001d8
	.4byte 0x400001f8
	.4byte 0x01100000
	.4byte 0x031000f0
	.4byte 0x00000260
	.4byte 0xffff001f
	.4byte 0x00000248
	.4byte 0x400001f8
	.4byte 0x01100000
	.4byte 0x031000f0
	.4byte 0x00000260
	.4byte 0xffff0020
	.4byte 0x00000210
	.4byte 0x40000128
	.4byte 0x01100000
	.4byte 0x031000f0
	.4byte 0x00000260
	.4byte 0xffff0021
	.4byte 0x000000d0
	.4byte 0xc0000348
	.4byte 0x00000000
	.4byte 0x01500270
	.4byte 0x00000360
	.4byte 0xffff0022
	.4byte 0x00000028
	.4byte 0x800002f8
	.4byte 0x00000000
	.4byte 0x01500270
	.4byte 0x00000360
	.4byte 0xffff0023
	.4byte 0x000000c8
	.4byte 0xc0000168
	.4byte 0x00000000
	.4byte 0x010000f0
	.4byte 0x000001e0
	.4byte 0xffff0024
	.4byte 0x00000338
	.4byte 0xc0000168
	.4byte 0x03000000
	.4byte 0x040000f0
	.4byte 0x000001e0
	.4byte 0xffff0028
	.4byte 0x00000178
	.4byte 0x000003a8
	.4byte 0x00e80000
	.4byte 0x01d80360
	.4byte 0x00000400
	.4byte 0xffff0029
	.4byte 0x00000208
	.4byte 0x800003a8
	.4byte 0x01a80000
	.4byte 0x02980360
	.4byte 0x00000400
	.4byte 0xffff002a
	.4byte 0x00000388
	.4byte 0x00000308
	.4byte 0x03080000
	.4byte 0x03f802c0
	.4byte 0x00000360
	.4byte 0xffff002b
	.4byte 0x000000d8
	.4byte 0x80000228
	.4byte 0x00000000
	.4byte 0x013001d8
	.4byte 0x00000278
	.4byte 0xffff002c
	.4byte 0x00000058
	.4byte 0x40000228
	.4byte 0x00000000
	.4byte 0x013001d8
	.4byte 0x00000278
	.4byte 0xffff002d
	.4byte 0x000002d8
	.4byte 0xc0000338
	.4byte 0x02280000
	.4byte 0x03180290
	.4byte 0x00000350
	.4byte 0xffff0062
	.4byte 0x00000098
	.4byte 0x40000068
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x000000f0
	.4byte 0xffff0063
	.4byte 0x00000098
	.4byte 0x40000068
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x000000f0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTorebiKyudenEntrances2
gTorebiKyudenEntrances2:
	.4byte 0xffff0000
	.4byte 0x00000108
	.4byte 0x400001b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000108
	.4byte 0xc00001a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000000c8
	.4byte 0x400000f8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global TorebiKyuden_EmptyTable
TorebiKyuden_EmptyTable:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global TorebiKyuden_MessageTable
TorebiKyuden_MessageTable:
	.4byte 0x0000008a
	.4byte 0x0010208b
	.4byte 0x0021808a
	.4byte 0x0031908a
	.4byte 0x0041408a
	.4byte 0x0051608a
	.4byte 0x0060808a
	.4byte 0x0070908a
	.4byte 0x0080608a
	.4byte 0x0090708a
	.4byte 0x00a1a08a
	.4byte 0x00b13008
	.4byte 0x00c1b08a
	.4byte 0x00d0108d
	.4byte 0x0102208a
	.4byte 0x0111208a
	.4byte 0x0121108a
	.4byte 0x0140408a
	.4byte 0x0152808a
	.4byte 0x0160508a
	.4byte 0x0172908a
	.4byte 0x0180208a
	.4byte 0x0190308a
	.4byte 0x01a0a08a
	.4byte 0x01b0c08a
	.4byte 0x01c2a08a
	.4byte 0x01d2b08a
	.4byte 0x01e2308a
	.4byte 0x01f2408a
	.4byte 0x0202108a
	.4byte 0x0212008a
	.4byte 0x0221008a
	.4byte 0x0231e08a
	.4byte 0x0241f08a
	.4byte 0x0281508a
	.4byte 0x0291708a
	.4byte 0x02a1c08a
	.4byte 0x02b1d08a
	.4byte 0x02c2d08a
	.4byte 0x02d2c08a
	.4byte 0x0000008b
	.4byte 0x0010b087
	.4byte 0x0020108a
	.4byte 0x000001ff
	.global gTorebiKyudenPlacements2
gTorebiKyudenPlacements2:
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00014000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTorebiKyudenPlacementsOther
gTorebiKyudenPlacementsOther:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x02f00000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00002000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00004000
	.4byte 0xffff003e
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00000000
	.4byte 0xffff003e
	.4byte 0x0200c570
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0000e000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00002000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x03780000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x03d80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0001e000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x03d80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00005000
	.4byte 0xffff0098
	.4byte 0x00000002
	.4byte 0x03b00000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00004000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x03980000
	.4byte 0x00000000
	.4byte 0x02300000
	.4byte 0x00018000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00004000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00018000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00016000
	.4byte 0x096b0039
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00020000
	.4byte 0x096b0098
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00020000
	.4byte 0x096b0098
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01700000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTorebiKyudenPlacementsColosso
gTorebiKyudenPlacementsColosso:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0067
	.4byte 0x00000002
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00004000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00004000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00004000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x03980000
	.4byte 0x00000000
	.4byte 0x02300000
	.4byte 0x00018000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00004000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00018000
	.4byte 0xffff0006
	.4byte 0x00000002
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00006000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTorebiKyudenPlacementsAfterColosso
gTorebiKyudenPlacementsAfterColosso:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0067
	.4byte 0x00000002
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00004000
	.4byte 0xffff006a
	.4byte 0x00000002
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00004000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00004000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00004000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x03d80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0000e000
	.4byte 0xffff0098
	.4byte 0x00000002
	.4byte 0x03b00000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00004000
	.4byte 0xffff0034
	.4byte 0x00000001
	.4byte 0x03780000
	.4byte 0x00000000
	.4byte 0x02290000
	.4byte 0x00014000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x03980000
	.4byte 0x00000000
	.4byte 0x02300000
	.4byte 0x00018000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x02140000
	.4byte 0x00014000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00004000
	.4byte 0xffff006f
	.4byte 0x00000002
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00004000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0x096f0039
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02c00000
	.4byte 0x00022000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTorebiKyudenEventsOther
gTorebiKyudenEventsOther:
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
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x0200bd41
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x0200bd41
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000031
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000031
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x0000c602
	.4byte 0xffff0011
	.4byte 0x0200bd41
	.4byte 0x00000001
	.4byte 0xffff0012
	.4byte 0x00000012
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
	.4byte 0x0000c602
	.4byte 0xffff001e
	.4byte 0x0200bd41
	.4byte 0x0000c602
	.4byte 0xffff001f
	.4byte 0x0200bd41
	.4byte 0x0000c602
	.4byte 0xffff0020
	.4byte 0x0200bd41
	.4byte 0x00000001
	.4byte 0xffff0021
	.4byte 0x00000021
	.4byte 0x00000001
	.4byte 0xffff0022
	.4byte 0x00000022
	.4byte 0x00000001
	.4byte 0xffff0023
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0xffff0024
	.4byte 0x00000024
	.4byte 0x00000001
	.4byte 0xffff0028
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0xffff0029
	.4byte 0x00000029
	.4byte 0x00000001
	.4byte 0xffff002a
	.4byte 0x0000002a
	.4byte 0x00000001
	.4byte 0xffff002b
	.4byte 0x0000002b
	.4byte 0x0000c602
	.4byte 0xffff002c
	.4byte 0x0200bd41
	.4byte 0x00000001
	.4byte 0xffff002d
	.4byte 0x0000002d
	.4byte 0x00000002
	.4byte 0x096b003c
	.4byte 0x02008ff9
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x020080c9
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001ff4
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001ff5
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001ff6
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008109
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008349
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x020083b1
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x0000200c
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x0000200d
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000200e
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000200f
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002010
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00002011
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002012
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002013
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002014
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002015
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002016
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x0000204b
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x0000204c
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x0000204d
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x0000204e
	.4byte 0x00000000
	.4byte 0xffff0006
	.4byte 0x0000204f
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00002050
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00002051
	.4byte 0x00008d15
	.4byte 0xffff0006
	.4byte 0x0200849d
	.4byte 0x00000003
	.4byte 0x19690046
	.4byte 0x02008af9
	.4byte 0x000001b3
	.4byte 0xffff00c8
	.4byte 0x004029b0
	.4byte 0x000001b3
	.4byte 0xffff00c9
	.4byte 0x004029b1
	.4byte 0x000001b3
	.4byte 0xffff00ca
	.4byte 0x004029b2
	.4byte 0x000000f3
	.4byte 0xffff00cb
	.4byte 0x004029b3
	.4byte 0x000000f3
	.4byte 0xffff00cc
	.4byte 0x004029b4
	.4byte 0x000000f3
	.4byte 0xffff00cd
	.4byte 0x004029b5
	.4byte 0x000000f3
	.4byte 0xffff00ce
	.4byte 0x004029b6
	.4byte 0x00000033
	.4byte 0x0f9f0064
	.4byte 0x001000e5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTorebiKyudenEventsColosso
gTorebiKyudenEventsColosso:
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
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x0200bd41
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x0200bd41
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000031
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000031
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x0000c602
	.4byte 0xffff0011
	.4byte 0x0200bd41
	.4byte 0x00000001
	.4byte 0xffff0012
	.4byte 0x00000012
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
	.4byte 0x0000c602
	.4byte 0xffff001e
	.4byte 0x0200bd41
	.4byte 0x0000c602
	.4byte 0xffff001f
	.4byte 0x0200bd41
	.4byte 0x0000c602
	.4byte 0xffff0020
	.4byte 0x0200bd41
	.4byte 0x00000001
	.4byte 0xffff0021
	.4byte 0x00000021
	.4byte 0x00000001
	.4byte 0xffff0022
	.4byte 0x00000022
	.4byte 0x00000001
	.4byte 0xffff0023
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0xffff0024
	.4byte 0x00000024
	.4byte 0x00000001
	.4byte 0xffff0028
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0xffff0029
	.4byte 0x00000029
	.4byte 0x00000001
	.4byte 0xffff002a
	.4byte 0x0000002a
	.4byte 0x00000001
	.4byte 0xffff002b
	.4byte 0x0000002b
	.4byte 0x0000c602
	.4byte 0xffff002c
	.4byte 0x0200bd41
	.4byte 0x00000001
	.4byte 0xffff002d
	.4byte 0x0000002d
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0000223d
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0000223e
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000223f
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002240
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008265
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002246
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002247
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002248
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00002249
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000224a
	.4byte 0x00000000
	.4byte 0xffff0006
	.4byte 0x0000224b
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000224c
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000224d
	.4byte 0x00008d15
	.4byte 0xffff0006
	.4byte 0x0000224e
	.4byte 0x00000003
	.4byte 0x19670046
	.4byte 0x02008af9
	.4byte 0x000001b3
	.4byte 0xffff00c8
	.4byte 0x004029b0
	.4byte 0x000001b3
	.4byte 0xffff00c9
	.4byte 0x004029b1
	.4byte 0x000001b3
	.4byte 0xffff00ca
	.4byte 0x004029b2
	.4byte 0x000000f3
	.4byte 0xffff00cb
	.4byte 0x004029b3
	.4byte 0x000000f3
	.4byte 0xffff00cc
	.4byte 0x004029b4
	.4byte 0x000000f3
	.4byte 0xffff00cd
	.4byte 0x004029b5
	.4byte 0x000000f3
	.4byte 0xffff00ce
	.4byte 0x004029b6
	.4byte 0x00000033
	.4byte 0x0f9f0064
	.4byte 0x001000e5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTorebiKyudenEventsAfterColosso
gTorebiKyudenEventsAfterColosso:
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
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x0200bd41
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x0200bd41
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000031
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000031
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x0000c602
	.4byte 0xffff0011
	.4byte 0x0200bd41
	.4byte 0x00000001
	.4byte 0xffff0012
	.4byte 0x00000012
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
	.4byte 0x0000c602
	.4byte 0xffff001e
	.4byte 0x0200bd41
	.4byte 0x0000c602
	.4byte 0xffff001f
	.4byte 0x0200bd41
	.4byte 0x0000c602
	.4byte 0xffff0020
	.4byte 0x0200bd41
	.4byte 0x00000001
	.4byte 0xffff0021
	.4byte 0x00000021
	.4byte 0x00000001
	.4byte 0xffff0022
	.4byte 0x00000022
	.4byte 0x00000001
	.4byte 0xffff0023
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0xffff0024
	.4byte 0x00000024
	.4byte 0x00000001
	.4byte 0xffff0028
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0xffff0029
	.4byte 0x00000029
	.4byte 0x00000001
	.4byte 0xffff002a
	.4byte 0x0000002a
	.4byte 0x00000001
	.4byte 0xffff002b
	.4byte 0x0000002b
	.4byte 0x0000c602
	.4byte 0xffff002c
	.4byte 0x0200bd41
	.4byte 0x00000001
	.4byte 0xffff002d
	.4byte 0x0000002d
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008525
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x02008565
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000022ae
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000022af
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000022b0
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000022b1
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000022b2
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000022b3
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000022b4
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000022b5
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000022b6
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000022b7
	.4byte 0x00000000
	.4byte 0x1f31000e
	.4byte 0x00002357
	.4byte 0x00008d15
	.4byte 0x1f31000e
	.4byte 0x00002358
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0000234e
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x0000234f
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002350
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002351
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x020085a5
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00002359
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x0000235a
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x0000235b
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x0000235c
	.4byte 0x00000003
	.4byte 0xffff0046
	.4byte 0x02008af9
	.4byte 0x000001b3
	.4byte 0xffff00c8
	.4byte 0x004029b0
	.4byte 0x000001b3
	.4byte 0xffff00c9
	.4byte 0x004029b1
	.4byte 0x000001b3
	.4byte 0xffff00ca
	.4byte 0x004029b2
	.4byte 0x000000f3
	.4byte 0xffff00cb
	.4byte 0x004029b3
	.4byte 0x000000f3
	.4byte 0xffff00cc
	.4byte 0x004029b4
	.4byte 0x000000f3
	.4byte 0xffff00cd
	.4byte 0x004029b5
	.4byte 0x000000f3
	.4byte 0xffff00ce
	.4byte 0x004029b6
	.4byte 0x00000033
	.4byte 0x0f9f0064
	.4byte 0x001000e5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTorebiKyudenEvents2
gTorebiKyudenEvents2:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x0200bdf9
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001fed
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001fee
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001fef
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001ff0
	.4byte 0x00000003
	.4byte 0xffff005a
	.4byte 0x0200c00d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTorebiKyudenEvents2Colosso
gTorebiKyudenEvents2Colosso:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x0200bdf9
	.4byte 0x00000000
	.4byte 0x096c0008
	.4byte 0x0200be41
	.4byte 0x00000000
	.4byte 0x096c0009
	.4byte 0x0200be41
	.4byte 0x00008d15
	.4byte 0x096c0408
	.4byte 0x0200be41
	.4byte 0x00008d15
	.4byte 0x096c0409
	.4byte 0x0200be41
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002238
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0200bf85
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000223b
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x0000223c
	.4byte 0x00000003
	.4byte 0xffff005a
	.4byte 0x0200c00d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTorebiKyudenEvents2AfterColosso
gTorebiKyudenEvents2AfterColosso:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x0200bdf9
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000022a2
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0200bfc5
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000022a6
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000022a7
	.4byte 0x00000003
	.4byte 0xffff005a
	.4byte 0x0200c00d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
