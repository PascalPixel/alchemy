.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_IE/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	push {r5, lr}
	adds r5, r0, #0
	ldr r1, [r5, #104]
	cmp r1, #0
	beq .L_02000030_0
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
	bl 0x0200a604
	ldrh r3, [r5, #6]
	lsls r0, r0, #16
	lsrs r0, r0, #16
	subs r0, r0, r3
	lsls r0, r0, #16
	asrs r0, r0, #16
	cmp r0, #0
	beq .L_02000030_0
	movs r2, #128
	lsls r2, r2, #5
	cmp r0, r2
	ble .L_02000030_1
	adds r0, r2, #0
.L_02000030_1:
	ldr r2, [pc, #20]
	cmp r0, r2
	bge .L_02000030_2
	adds r0, r2, #0
.L_02000030_2:
	adds r3, r3, r0
	strh r3, [r5, #6]
.L_02000030_0:
	movs r0, #1
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0xfffff000
	.global Func_02000088
	.thumb_func
Func_02000088:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200afa0
	.global Func_02000090
	.thumb_func
Func_02000090:
	movs r0, #0
	bx lr
	.global Func_02000094
	.thumb_func
Func_02000094:
	push {lr}
	ldr r0, [pc, #20]
	bl 0x0200a674
	cmp r0, #0
	beq .L_02000094_0
	ldr r0, [pc, #12]
	b .L_02000094_1
.L_02000094_0:
	ldr r0, [pc, #12]
.L_02000094_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x00000834
	.4byte 0x0200b144
	.4byte 0x0200b108
	.global Func_020000b8
	.thumb_func
Func_020000b8:
	push {lr}
	ldr r0, [pc, #52]
	bl 0x0200a674
	cmp r0, #0
	beq .L_020000b8_0
	ldr r0, [pc, #44]
	b .L_020000b8_1
.L_020000b8_0:
	ldr r3, [pc, #44]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #12
	bne .L_020000b8_2
	ldr r0, [pc, #32]
	b .L_020000b8_1
.L_020000b8_2:
	ldr r0, [pc, #32]
	bl 0x0200a674
	cmp r0, #0
	beq .L_020000b8_3
	ldr r0, [pc, #28]
	b .L_020000b8_1
.L_020000b8_3:
	ldr r0, [pc, #28]
.L_020000b8_1:
	pop {r1}
	bx r1
	.4byte 0x00000834
	.4byte 0x0200b380
	.4byte 0x02000240
	.4byte 0x0200b560
	.4byte 0x0000087a
	.4byte 0x0200b7d0
	.4byte 0x0200b170
	.global Func_0200010c
	.thumb_func
Func_0200010c:
	push {lr}
	bl 0x0200a69c
	movs r1, #1
	ldr r0, [pc, #52]
	bl 0x0200a664
	movs r0, #126
	bl 0x0200a84c
	movs r1, #0
	ldr r0, [pc, #44]
	bl 0x0200a83c
	movs r0, #10
	bl 0x0200a694
	movs r1, #1
	ldr r0, [pc, #32]
	bl 0x0200a664
	bl 0x0200a66c
	movs r0, #161
	lsls r0, r0, #1
	bl 0x0200a684
	bl 0x0200a6a4
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000111f
	.4byte 0x000003e7
	.4byte 0x00000974
	.global Func_02000158
	.thumb_func
Func_02000158:
	push {lr}
	ldr r0, [pc, #68]
	bl 0x0200a674
	cmp r0, #0
	beq .L_02000158_0
	ldr r0, [pc, #60]
	b .L_02000158_1
.L_02000158_0:
	ldr r0, [pc, #60]
	bl 0x0200a674
	cmp r0, #0
	beq .L_02000158_2
	ldr r0, [pc, #56]
	b .L_02000158_1
.L_02000158_2:
	ldr r3, [pc, #56]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #12
	bne .L_02000158_3
	ldr r0, [pc, #44]
	b .L_02000158_1
.L_02000158_3:
	ldr r0, [pc, #44]
	bl 0x0200a674
	cmp r0, #0
	beq .L_02000158_4
	ldr r0, [pc, #36]
	b .L_02000158_1
.L_02000158_4:
	ldr r0, [pc, #36]
.L_02000158_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x0000087a
	.4byte 0x0200bcec
	.4byte 0x00000815
	.4byte 0x0200bb3c
	.4byte 0x02000240
	.4byte 0x0200bb30
	.4byte 0x00000834
	.4byte 0x0200ba64
	.4byte 0x0200b938
	.global Func_020001c4
	.thumb_func
Func_020001c4:
	push {lr}
	bl 0x0200a69c
	ldr r0, [pc, #32]
	bl 0x0200a764
	movs r2, #2
	movs r0, #23
	movs r1, #0
	bl 0x0200a74c
	movs r1, #0
	movs r0, #23
	bl 0x0200a784
	bl 0x0200a6a4
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000f3c
	.global Func_020001f0
	.thumb_func
Func_020001f0:
	push {lr}
	bl 0x0200a69c
	ldr r0, [pc, #32]
	bl 0x0200a764
	movs r2, #2
	movs r0, #24
	movs r1, #0
	bl 0x0200a74c
	movs r1, #0
	movs r0, #24
	bl 0x0200a784
	bl 0x0200a6a4
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000f3f
	.global Func_0200021c
	.thumb_func
Func_0200021c:
	push {lr}
	bl 0x0200a69c
	ldr r0, [pc, #32]
	bl 0x0200a764
	movs r2, #2
	movs r0, #15
	movs r1, #0
	bl 0x0200a74c
	movs r1, #0
	movs r0, #15
	bl 0x0200a784
	bl 0x0200a6a4
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000f44
	.global Func_02000248
	.thumb_func
Func_02000248:
	push {r5, r6, r7, lr}
	bl 0x0200a69c
	ldr r0, [pc, #320]
	bl 0x0200a674
	cmp r0, #0
	beq .L_02000248_0
	ldr r5, [pc, #312]
	adds r0, r5, #0
	bl 0x0200a764
	movs r0, #2
	bl 0x0200a674
	cmp r0, #0
	beq .L_02000248_1
	ldr r3, [pc, #300]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02000248_1:
	movs r0, #3
	bl 0x0200a674
	cmp r0, #0
	beq .L_02000248_2
	ldr r3, [pc, #272]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02000248_2:
	movs r1, #0
	movs r0, #17
	bl 0x0200a76c
	movs r0, #0
	movs r1, #0
	bl 0x0200a6bc
	cmp r0, #0
	bne .L_02000248_3
	adds r0, r5, #3
	bl 0x0200a764
	b .L_02000248_4
.L_02000248_3:
	adds r0, r5, #4
	bl 0x0200a764
.L_02000248_4:
	movs r0, #17
	movs r1, #0
	bl 0x0200a774
	b .L_02000248_5
.L_02000248_0:
	ldr r3, [pc, #216]
	ldr r3, [r3]
	ldr r0, [pc, #216]
	ldr r6, [r3]
	bl 0x0200a764
	movs r2, #0
	movs r0, #17
	movs r1, #0
	bl 0x0200a74c
	movs r1, #0
	movs r0, #17
	bl 0x0200a784
	movs r0, #20
	bl 0x0200a694
	movs r1, #2
	movs r0, #17
	bl 0x0200a734
	movs r0, #15
	bl 0x0200a694
	bl 0x0200a564
	movs r7, #0
	movs r5, #0
.L_02000248_6:
	movs r0, #17
	bl 0x0200a6c4
	bl 0x0200a2f8
	adds r5, #1
	movs r0, #1
	bl 0x0200a5ec
	cmp r5, #39
	bls .L_02000248_6
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #140]
	bl 0x0200a5f4
	movs r0, #107
	bl 0x0200a84c
	movs r5, #0
.L_02000248_10:
	adds r0, r5, #0
	movs r1, #10
	bl 0x0200a5e4
	cmp r0, #0
	bne .L_02000248_7
	movs r3, #1
	ands r3, r7
	cmp r3, #0
	beq .L_02000248_8
	ldr r3, [r6]
	ldr r2, [pc, #108]
	b .L_02000248_9
.L_02000248_8:
	ldr r3, [r6]
	movs r2, #128
	lsls r2, r2, #9
.L_02000248_9:
	adds r3, r3, r2
	str r3, [r6]
	adds r7, #1
.L_02000248_7:
	movs r0, #1
	adds r5, #1
	bl 0x0200a694
	cmp r5, #180
	bne .L_02000248_10
	ldr r0, [pc, #84]
	bl 0x0200a84c
	ldr r0, [pc, #72]
	bl 0x0200a5fc
	movs r0, #1
	bl 0x0200a5ec
	bl 0x0200a574
	movs r1, #0
	movs r0, #17
	bl 0x0200a754
	movs r0, #40
	bl 0x0200a694
	ldr r0, [pc, #52]
	bl 0x0200a764
	movs r0, #17
	movs r1, #0
	bl 0x0200a774
.L_02000248_5:
	bl 0x0200a6a4
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x00000815
	.4byte 0x00001197
	.4byte 0x03001ebc
	.4byte 0x03001e70
	.4byte 0x00000f48
	.4byte 0x0200a591
	.4byte 0xffff0000
	.4byte 0x00000121
	.4byte 0x00000f4b
	.global Func_020003b4
	.thumb_func
Func_020003b4:
	push {lr}
	bl 0x0200a69c
	ldr r0, [pc, #112]
	bl 0x0200a674
	cmp r0, #0
	beq .L_020003b4_0
	ldr r0, [pc, #104]
	bl 0x0200a764
	movs r1, #0
	movs r0, #15
	bl 0x0200a76c
	movs r0, #0
	movs r1, #0
	bl 0x0200a6bc
	cmp r0, #1
	bne .L_020003b4_1
	movs r0, #15
	movs r1, #0
	bl 0x0200a774
	b .L_020003b4_2
.L_020003b4_1:
	ldr r3, [pc, #72]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #15
	b .L_020003b4_3
.L_020003b4_0:
	ldr r0, [pc, #56]
	bl 0x0200a674
	cmp r0, #0
	beq .L_020003b4_4
	ldr r0, [pc, #52]
	bl 0x0200a764
	movs r0, #11
.L_020003b4_3:
	movs r1, #0
	bl 0x0200a784
	b .L_020003b4_2
.L_020003b4_4:
	ldr r0, [pc, #40]
	bl 0x0200a764
	movs r0, #11
	movs r1, #0
	bl 0x0200a784
.L_020003b4_2:
	bl 0x0200a6a4
	pop {r0}
	bx r0
	.4byte 0x0000087a
	.4byte 0x00001be8
	.4byte 0x03001ebc
	.4byte 0x00000815
	.4byte 0x00001191
	.4byte 0x00000ea8
	.global Func_02000444
	.thumb_func
Func_02000444:
	push {lr}
	bl 0x0200a69c
	movs r0, #26
	movs r1, #1
	bl 0x0200a71c
	movs r0, #26
	movs r1, #0
	movs r2, #20
	bl 0x0200a744
	movs r2, #40
	movs r1, #21
	movs r0, #26
	bl 0x0200a744
	ldr r0, [pc, #92]
	bl 0x0200a764
	movs r0, #26
	movs r1, #20
	bl 0x0200a2c8
	ldr r0, [pc, #80]
	ldr r1, [pc, #84]
	bl 0x0200a7ac
	movs r1, #1
	movs r2, #136
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	ldr r0, [pc, #72]
	bl 0x0200a7b4
	movs r0, #20
	bl 0x0200a694
	movs r1, #2
	movs r0, #26
	bl 0x0200a73c
	movs r0, #20
	bl 0x0200a694
	movs r2, #10
	movs r0, #26
	movs r1, #0
	bl 0x0200a744
	movs r0, #26
	movs r1, #40
	bl 0x0200a2c8
	movs r0, #26
	movs r1, #2
	bl 0x0200a6d4
	bl 0x0200a6a4
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000011c7
	.4byte 0x00019999
	.4byte 0x00003333
	.4byte 0x01510000
	.global Func_020004d4
	.thumb_func
Func_020004d4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	bl 0x0200a69c
	movs r2, #190
	movs r0, #0
	movs r1, #82
	lsls r2, r2, #2
	bl 0x0200a704
	movs r2, #30
	movs r1, #0
	movs r0, #15
	bl 0x0200a74c
	ldr r0, [pc, #228]
	bl 0x0200a764
	movs r0, #15
	movs r1, #20
	bl 0x0200a2c8
	movs r1, #160
	movs r2, #20
	lsls r1, r1, #8
	movs r0, #15
	bl 0x0200a2e0
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #15
	bl 0x0200a7a4
	movs r0, #20
	bl 0x0200a694
	bl 0x0200a564
	movs r5, #0
.L_020004d4_0:
	movs r0, #15
	bl 0x0200a6c4
	bl 0x0200a2f8
	adds r5, #1
	movs r0, #1
	bl 0x0200a5ec
	cmp r5, #39
	bls .L_020004d4_0
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #160]
	bl 0x0200a5f4
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #152]
	bl 0x0200a5f4
	movs r1, #160
	movs r2, #10
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200a78c
	movs r0, #20
	bl 0x0200a6c4
	adds r6, r0, #0
	adds r7, r6, #0
	adds r7, #85
	ldrb r2, [r7]
	movs r3, #0
	strb r3, [r7]
	mov r8, r2
	movs r5, #0
.L_020004d4_1:
	ldr r3, [r6, #12]
	movs r2, #192
	lsls r2, r2, #5
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r0, #1
	adds r5, #1
	bl 0x0200a5ec
	cmp r5, #39
	bls .L_020004d4_1
	mov r3, r8
	strb r3, [r7]
	ldr r0, [pc, #84]
	bl 0x0200a5fc
	ldr r0, [pc, #80]
	bl 0x0200a5fc
	movs r0, #1
	bl 0x0200a5ec
	movs r0, #161
	bl 0x0200a84c
	movs r0, #15
	movs r1, #0
	bl 0x0200a754
	movs r1, #0
	movs r0, #20
	bl 0x0200a754
	movs r0, #40
	bl 0x0200a694
	bl 0x0200a574
	movs r2, #30
	movs r0, #0
	movs r1, #15
	bl 0x0200a74c
	movs r0, #15
	movs r1, #0
	bl 0x0200a774
	bl 0x0200a6a4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x00000eae
	.4byte 0x0200a581
	.4byte 0x0200a5a1
	.global Func_020005e8
	.thumb_func
Func_020005e8:
	push {lr}
	bl 0x0200a69c
	movs r0, #16
	movs r1, #0
	movs r2, #10
	bl 0x0200a74c
	movs r0, #132
	lsls r0, r0, #4
	bl 0x0200a674
	cmp r0, #0
	beq .L_020005e8_0
	ldr r0, [pc, #36]
	bl 0x0200a764
	movs r0, #16
	movs r1, #0
	bl 0x0200a774
	b .L_020005e8_1
.L_020005e8_0:
	ldr r0, [pc, #24]
	bl 0x0200a764
	movs r0, #16
	movs r1, #0
	bl 0x0200a774
.L_020005e8_1:
	bl 0x0200a6a4
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000eb1
	.4byte 0x00000eb0
	.global Func_02000634
	.thumb_func
Func_02000634:
	push {lr}
	bl 0x0200a69c
	ldr r0, [pc, #44]
	bl 0x0200a674
	cmp r0, #0
	beq .L_02000634_0
	ldr r0, [pc, #36]
	bl 0x0200a764
	b .L_02000634_1
.L_02000634_0:
	ldr r0, [pc, #32]
	bl 0x0200a764
	ldr r0, [pc, #20]
	bl 0x0200a67c
.L_02000634_1:
	movs r0, #11
	movs r1, #0
	bl 0x0200a774
	bl 0x0200a6a4
	pop {r0}
	bx r0
	.4byte 0x00000302
	.4byte 0x00001be4
	.4byte 0x00001be3
	.global Func_02000674
	.thumb_func
Func_02000674:
	push {r5, lr}
	movs r0, #21
	bl 0x0200a6c4
	adds r5, r0, #0
	bl 0x0200a69c
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r5, #56]
	str r3, [r5, #60]
	str r3, [r5, #64]
	movs r1, #1
	movs r0, #21
	bl 0x0200a71c
	movs r0, #21
	bl 0x0200a6e4
	movs r1, #128
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #21
	bl 0x0200a79c
	movs r3, #176
	lsls r3, r3, #8
	strh r3, [r5, #6]
	movs r0, #20
	bl 0x0200a694
	movs r1, #2
	movs r0, #21
	bl 0x0200a734
	ldr r0, [pc, #76]
	bl 0x0200a764
	movs r0, #21
	movs r1, #0
	movs r2, #40
	bl 0x0200a77c
	movs r2, #20
	movs r0, #21
	movs r1, #0
	bl 0x0200a744
	movs r0, #21
	movs r1, #2
	bl 0x0200a734
	movs r1, #0
	movs r0, #21
	bl 0x0200a774
	ldr r0, [pc, #36]
	bl 0x0200a67c
	movs r0, #21
	bl 0x0200a6e4
	movs r0, #1
	bl 0x0200a5ec
	ldr r1, [pc, #24]
	movs r0, #21
	bl 0x0200a6d4
	bl 0x0200a6a4
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00001c94
	.4byte 0x00000306
	.4byte 0x0200ae34
	.global Func_02000714
	.thumb_func
Func_02000714:
	push {r5, lr}
	adds r5, r0, #0
	ldr r0, [pc, #44]
	bl 0x0200a674
	cmp r0, #0
	beq .L_02000714_0
	bl 0x0200a7fc
.L_02000714_0:
	ldr r3, [pc, #36]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	subs r3, #192
	str r3, [r2]
	adds r3, #200
	adds r2, r1, r3
	movs r3, #16
	str r3, [r2]
	adds r0, r5, #0
	bl 0x0200a7cc
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00000834
	.4byte 0x03001ebc
	.global Func_02000750
	.thumb_func
Func_02000750:
	push {lr}
	movs r0, #158
	bl 0x0200a84c
	ldr r0, [pc, #28]
	movs r1, #44
	movs r2, #7
	bl 0x0200a63c
	movs r0, #0
	movs r1, #248
	ldr r2, [pc, #20]
	bl 0x0200a6fc
	movs r0, #1
	bl 0x02008714
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200beb4
	.4byte 0x00000117
	.global Func_02000780
	.thumb_func
Func_02000780:
	push {r5, lr}
	movs r0, #188
	sub sp, #8
	bl 0x0200a84c
	movs r5, #2
	movs r1, #63
	movs r2, #51
	movs r3, #8
	movs r0, #0
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a644
	movs r0, #10
	bl 0x0200a5ec
	movs r3, #8
	movs r1, #63
	movs r2, #51
	movs r0, #2
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a644
	movs r0, #10
	bl 0x0200a5ec
	movs r1, #176
	movs r2, #153
	lsls r2, r2, #1
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200a704
	movs r0, #0
	movs r1, #3
	bl 0x0200a794
	movs r1, #176
	movs r2, #148
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #0
	bl 0x0200a704
	movs r0, #2
	bl 0x02008714
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020007ec
	.thumb_func
Func_020007ec:
	push {lr}
	movs r0, #158
	bl 0x0200a84c
	ldr r0, [pc, #28]
	movs r1, #43
	movs r2, #15
	bl 0x0200a63c
	movs r0, #0
	movs r1, #230
	ldr r2, [pc, #20]
	bl 0x0200a6fc
	movs r0, #3
	bl 0x02008714
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200beb4
	.4byte 0x00000197
	.global Func_0200081c
	.thumb_func
Func_0200081c:
	push {lr}
	movs r0, #158
	bl 0x0200a84c
	ldr r0, [pc, #28]
	movs r1, #52
	movs r2, #18
	bl 0x0200a63c
	movs r1, #187
	movs r0, #0
	lsls r1, r1, #1
	ldr r2, [pc, #16]
	bl 0x0200a6fc
	movs r0, #4
	bl 0x02008714
	pop {r0}
	bx r0
	.4byte 0x0200beb4
	.4byte 0x000001a3
	.global Func_0200084c
	.thumb_func
Func_0200084c:
	push {lr}
	movs r0, #158
	bl 0x0200a84c
	ldr r0, [pc, #28]
	movs r1, #41
	movs r2, #32
	bl 0x0200a63c
	movs r0, #0
	movs r1, #200
	ldr r2, [pc, #20]
	bl 0x0200a6fc
	movs r0, #5
	bl 0x02008714
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200beb4
	.4byte 0x00000222
	.global Func_0200087c
	.thumb_func
Func_0200087c:
	push {lr}
	movs r0, #158
	bl 0x0200a84c
	ldr r0, [pc, #28]
	movs r1, #35
	movs r2, #36
	bl 0x0200a63c
	movs r0, #0
	movs r1, #102
	ldr r2, [pc, #20]
	bl 0x0200a6fc
	movs r0, #6
	bl 0x02008714
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200beb4
	.4byte 0x00000263
	.global Func_020008ac
	.thumb_func
Func_020008ac:
	push {lr}
	movs r0, #158
	bl 0x0200a84c
	ldr r0, [pc, #28]
	movs r1, #51
	movs r2, #39
	bl 0x0200a63c
	movs r1, #179
	movs r0, #0
	lsls r1, r1, #1
	ldr r2, [pc, #16]
	bl 0x0200a6fc
	movs r0, #7
	bl 0x02008714
	pop {r0}
	bx r0
	.4byte 0x0200beb4
	.4byte 0x0000029e
	.global Func_020008dc
	.thumb_func
Func_020008dc:
	push {lr}
	movs r0, #123
	bl 0x0200a84c
	movs r0, #8
	bl 0x02008714
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020008f0
	.thumb_func
Func_020008f0:
	push {lr}
	ldr r0, [pc, #24]
	bl 0x0200a674
	cmp r0, #0
	beq .L_020008f0_0
	movs r0, #123
	bl 0x0200a84c
	movs r0, #10
	bl 0x02008714
.L_020008f0_0:
	pop {r0}
	bx r0
	.4byte 0x00000815
	.global Func_02000910
	.thumb_func
Func_02000910:
	push {lr}
	ldr r0, [pc, #8]
	bl 0x0200a67c
	pop {r0}
	bx r0
	.4byte 0x0000090b
	.global Func_02000920
	.thumb_func
Func_02000920:
	push {lr}
	ldr r0, [pc, #8]
	bl 0x0200a67c
	pop {r0}
	bx r0
	.4byte 0x0000090c
	.global Func_02000930
	.thumb_func
Func_02000930:
	push {lr}
	ldr r0, [pc, #8]
	bl 0x0200a67c
	pop {r0}
	bx r0
	.4byte 0x0000090d
	.global Func_02000940
	.thumb_func
Func_02000940:
	push {r5, lr}
	ldr r0, [pc, #584]
	sub sp, #8
	bl 0x0200a674
	cmp r0, #0
	beq .L_02000940_0
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x0200a714
.L_02000940_0:
	ldr r0, [pc, #564]
	bl 0x0200a674
	cmp r0, #0
	beq .L_02000940_1
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200a714
.L_02000940_1:
	ldr r0, [pc, #548]
	bl 0x0200a674
	cmp r0, #0
	beq .L_02000940_2
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x0200a714
.L_02000940_2:
	ldr r3, [pc, #532]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #98
	beq .L_02000940_3
	cmp r3, #98
	bgt .L_02000940_4
	cmp r3, #97
	bne .L_02000940_5
	b .L_02000940_6
.L_02000940_5:
	b .L_02000940_7
.L_02000940_4:
	cmp r3, #99
	beq .L_02000940_8
	b .L_02000940_7
.L_02000940_3:
	movs r0, #32
	bl 0x0200a67c
	movs r0, #50
	bl 0x0200a7cc
	b .L_02000940_9
.L_02000940_8:
	bl 0x0200a5b0
	b .L_02000940_9
.L_02000940_7:
	movs r0, #8
	bl 0x0200a6c4
	movs r5, #192
	lsls r5, r5, #9
	str r5, [r0, #28]
	movs r0, #9
	bl 0x0200a6c4
	str r5, [r0, #28]
	movs r0, #10
	bl 0x0200a6c4
	str r5, [r0, #28]
	ldr r0, [pc, #456]
	bl 0x0200a674
	cmp r0, #0
	beq .L_02000940_10
	movs r3, #2
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #97
	movs r1, #2
	movs r2, #80
	movs r3, #5
	bl 0x0200a644
	movs r3, #3
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #42
	movs r1, #53
	movs r2, #42
	movs r3, #54
	bl 0x0200a644
	bl 0x0200a634
	movs r0, #1
	bl 0x0200a5ec
	b .L_02000940_9
.L_02000940_10:
	ldr r0, [pc, #400]
	bl 0x0200a674
	cmp r0, #0
	beq .L_02000940_11
	bl 0x0200a7ec
	bl 0x0200a7f4
	movs r3, #18
	movs r2, #41
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #21
	movs r1, #38
	movs r2, #1
	movs r3, #1
	bl 0x0200a64c
	movs r0, #132
	lsls r0, r0, #4
	bl 0x0200a674
	cmp r0, #0
	beq .L_02000940_12
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl 0x0200a714
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl 0x0200a714
	movs r1, #128
	ldr r2, [pc, #332]
	movs r0, #19
	lsls r1, r1, #9
	bl 0x0200a75c
	b .L_02000940_12
.L_02000940_11:
	ldr r0, [pc, #324]
	bl 0x0200a674
	cmp r0, #0
	beq .L_02000940_12
	movs r1, #180
	movs r2, #142
	movs r0, #16
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x0200a714
	movs r3, #2
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #92
	movs r1, #2
	movs r2, #80
	movs r3, #5
	bl 0x0200a644
	movs r3, #3
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #42
	movs r1, #53
	movs r2, #42
	movs r3, #54
	bl 0x0200a644
	bl 0x0200a634
	movs r0, #1
	bl 0x0200a5ec
.L_02000940_12:
	ldr r3, [pc, #236]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #12
	bne .L_02000940_13
	bl 0x020097c8
	b .L_02000940_9
.L_02000940_13:
	ldr r0, [pc, #220]
	bl 0x0200a674
	cmp r0, #0
	beq .L_02000940_14
	movs r0, #20
	bl 0x0200a6c4
	ldr r3, [pc, #216]
	str r3, [r0, #24]
	str r3, [r0, #28]
	movs r0, #20
	bl 0x0200a6c4
	movs r1, #0
	bl 0x0200a654
	movs r0, #21
	bl 0x0200a6c4
	ldr r3, [pc, #196]
	movs r1, #5
	str r3, [r0, #24]
	str r3, [r0, #28]
	movs r0, #13
	bl 0x0200a71c
	b .L_02000940_15
.L_02000940_14:
	ldr r0, [pc, #172]
	bl 0x0200a674
	cmp r0, #0
	beq .L_02000940_15
	movs r2, #249
	ldr r1, [pc, #172]
	movs r0, #21
	lsls r2, r2, #16
	bl 0x0200a714
	movs r0, #21
	bl 0x0200a6c4
	movs r1, #0
	bl 0x0200a654
.L_02000940_15:
	movs r0, #132
	lsls r0, r0, #4
	bl 0x0200a674
	cmp r0, #0
	beq .L_02000940_16
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl 0x0200a714
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl 0x0200a714
.L_02000940_16:
	ldr r3, [pc, #92]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #19
	bne .L_02000940_17
.L_02000940_6:
	bl 0x020095b4
	b .L_02000940_9
.L_02000940_17:
	ldr r0, [pc, #76]
	bl 0x0200a674
	cmp r0, #0
	beq .L_02000940_18
	ldr r0, [pc, #92]
	bl 0x0200a674
	cmp r0, #0
	beq .L_02000940_18
	bl 0x02009084
	b .L_02000940_9
.L_02000940_18:
	ldr r0, [pc, #52]
	bl 0x0200a674
	cmp r0, #0
	beq .L_02000940_9
	bl 0x0200a814
	bl 0x0200a824
	bl 0x0200a80c
.L_02000940_9:
	movs r0, #0
	sub sp, #-8
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x0000090b
	.4byte 0x0000090c
	.4byte 0x0000090d
	.4byte 0x02000240
	.4byte 0x0000087a
	.4byte 0x00000834
	.4byte 0x0200ac00
	.4byte 0x00000815
	.4byte 0x00004ccc
	.4byte 0x00009999
	.4byte 0x014b0000
	.4byte 0x00000842
	.global Func_02000bbc
	.thumb_func
Func_02000bbc:
	push {r5, r6, lr}
	ldr r0, [pc, #512]
	movs r6, #0
	bl 0x0200a674
	cmp r0, #0
	bne .L_02000bbc_0
	b 0x02009054
.L_02000bbc_0:
	movs r0, #132
	lsls r0, r0, #4
	bl 0x0200a674
	cmp r0, #0
	beq .L_02000bbc_1
	b 0x02009054
.L_02000bbc_1:
	bl 0x0200a69c
	ldr r0, [pc, #484]
	ldr r1, [pc, #484]
	bl 0x0200a7ac
	movs r0, #197
	movs r1, #1
	movs r2, #192
	movs r3, #1
	lsls r2, r2, #18
	negs r1, r1
	lsls r0, r0, #16
	bl 0x0200a7b4
	bl 0x0200a7bc
	ldr r0, [pc, #460]
	bl 0x0200a764
	movs r0, #19
	movs r1, #2
	bl 0x0200a73c
	ldr r0, [pc, #452]
	movs r1, #0
	movs r2, #10
	bl 0x0200a77c
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200a6cc
	movs r1, #128
	movs r2, #128
	movs r0, #25
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200a6cc
	movs r0, #0
	movs r1, #179
	ldr r2, [pc, #412]
	bl 0x0200a704
	movs r0, #0
	bl 0x0200a6c4
	cmp r0, #0
	beq .L_02000bbc_2
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #25
	bl 0x0200a714
.L_02000bbc_2:
	movs r2, #201
	movs r0, #25
	movs r1, #179
	lsls r2, r2, #2
	bl 0x0200a704
	movs r0, #0
	movs r1, #25
	movs r2, #40
	bl 0x0200a74c
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200a78c
	movs r2, #0
	movs r0, #25
	movs r1, #0
	bl 0x0200a78c
	movs r0, #17
	movs r1, #3
	bl 0x0200a71c
	movs r0, #18
	movs r1, #3
	bl 0x0200a724
	movs r2, #0
	movs r1, #18
	movs r0, #17
	bl 0x0200a74c
	movs r0, #20
	bl 0x0200a694
	movs r0, #17
	movs r1, #1
	bl 0x0200a734
	movs r2, #10
	ldr r0, [pc, #308]
	movs r1, #0
	bl 0x0200a77c
	movs r0, #18
	movs r1, #3
	bl 0x0200a71c
	movs r0, #18
	movs r1, #0
	movs r2, #10
	bl 0x0200a77c
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl 0x0200a78c
	movs r1, #240
	movs r2, #10
	movs r0, #18
	lsls r1, r1, #8
	bl 0x0200a78c
	movs r0, #19
	movs r1, #3
	bl 0x0200a724
	ldr r0, [pc, #244]
	movs r1, #0
	movs r2, #10
	bl 0x0200a77c
	movs r0, #17
	ldr r1, [pc, #220]
	ldr r2, [pc, #240]
	bl 0x0200a6cc
	ldr r2, [pc, #236]
	movs r0, #18
	ldr r1, [pc, #208]
	bl 0x0200a6cc
	ldr r5, [pc, #228]
	movs r0, #17
	adds r1, r5, #0
	bl 0x0200a6d4
	movs r0, #20
	bl 0x0200a694
	movs r0, #18
	adds r1, r5, #0
	bl 0x0200a6d4
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a78c
	movs r1, #192
	movs r2, #60
	movs r0, #25
	lsls r1, r1, #8
	bl 0x0200a78c
	ldr r1, [pc, #184]
	movs r0, #0
	bl 0x0200a6d4
	ldr r1, [pc, #180]
	movs r0, #25
	bl 0x0200a6ec
	movs r0, #20
	bl 0x0200a694
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200a78c
	movs r2, #10
	movs r0, #25
	movs r1, #0
	bl 0x0200a78c
	movs r0, #25
	movs r1, #0
	bl 0x0200a774
	movs r1, #128
	movs r0, #19
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a78c
	movs r1, #192
	movs r2, #20
	lsls r1, r1, #7
	movs r0, #26
	bl 0x0200a2e0
	movs r0, #26
	movs r1, #2
	bl 0x0200a73c
	movs r0, #26
	movs r1, #10
	bl 0x0200a2c8
	movs r0, #0
	movs r1, #3
	bl 0x0200a71c
	movs r1, #3
	movs r0, #25
	bl 0x0200a724
	movs r0, #20
	bl 0x0200a694
	movs r0, #19
	movs r1, #2
	bl 0x0200a73c
	movs r1, #0
	ldr r0, [pc, #44]
	bl 0x0200a76c
	movs r0, #0
	movs r1, #0
	bl 0x0200a6bc
	cmp r0, #1
	bne .L_02000bbc_3
	movs r0, #19
	movs r1, #4
	movs r6, #1
	bl 0x0200a71c
	b .L_02000bbc_4
	.4byte 0x00000834
	.4byte 0x00019999
	.4byte 0x00003333
	.4byte 0x00000eb6
	.4byte 0x00004013
	.4byte 0x00000315
	.4byte 0x00004011
	.4byte 0x0000cccc
	.4byte 0x0200aef0
	.4byte 0x0200af50
	.4byte 0x0200af78
.L_02000bbc_3:
	movs r0, #19
	movs r1, #3
	bl 0x0200a71c
	ldr r3, [pc, #612]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02000bbc_4:
	ldr r0, [pc, #600]
	movs r1, #0
	bl 0x0200a774
	cmp r6, #0
	beq .L_02000bbc_5
	ldr r3, [pc, #584]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02000bbc_5:
	movs r5, #128
	lsls r5, r5, #7
	adds r1, r5, #0
	movs r2, #30
	movs r0, #22
	bl 0x0200a2e0
	movs r0, #22
	movs r1, #0
	bl 0x0200a774
	movs r1, #128
	movs r0, #19
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200a79c
	movs r1, #128
	movs r0, #26
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200a79c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200a79c
	movs r1, #128
	movs r0, #25
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200a79c
	movs r1, #160
	movs r0, #19
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a78c
	movs r1, #160
	movs r0, #26
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a78c
	movs r6, #224
	movs r1, #224
	lsls r6, r6, #8
.L_02000e84:
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a78c
	adds r1, r6, #0
	movs r2, #10
	movs r0, #25
	bl 0x0200a2e0
	ldr r0, [pc, #456]
	ldr r1, [pc, #460]
	bl 0x0200a7ac
	movs r0, #215
	movs r1, #1
	ldr r2, [pc, #452]
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	bl 0x0200a7b4
	bl 0x0200a7bc
	ldr r0, [pc, #440]
	ldr r1, [pc, #444]
	bl 0x0200a7ac
	movs r0, #205
	movs r1, #1
	movs r3, #1
	ldr r2, [pc, #436]
	lsls r0, r0, #16
	negs r1, r1
	bl 0x0200a7b4
	ldr r1, [pc, #428]
	movs r0, #22
	bl 0x0200a6d4
	movs r0, #22
	bl 0x0200a6dc
	movs r1, #128
	movs r2, #60
	lsls r1, r1, #6
	movs r0, #22
	bl 0x0200a2e0
	movs r0, #19
	movs r1, #2
	bl 0x0200a73c
	movs r0, #19
	movs r1, #10
	bl 0x0200a2c8
	movs r0, #22
	movs r1, #3
	bl 0x0200a724
	movs r0, #22
	movs r1, #20
	bl 0x0200a2c8
	movs r1, #3
	movs r0, #19
	bl 0x0200a724
	movs r0, #10
	bl 0x0200a694
	adds r1, r5, #0
	movs r2, #30
	movs r0, #19
	adds r5, #19
	bl 0x0200a2e0
.L_02000f20:
	adds r0, r5, #0
	movs r1, #10
	bl 0x0200a2c8
	adds r1, r6, #0
	movs r2, #30
	movs r0, #26
	movs r6, #128
	bl 0x0200a2e0
	lsls r6, r6, #8
	movs r0, #26
	movs r1, #3
	bl 0x0200a724
	movs r2, #30
	movs r0, #19
	adds r1, r6, #0
	bl 0x0200a2e0
	movs r0, #19
	movs r1, #2
	bl 0x0200a73c
	adds r0, r5, #0
.L_02000f52:
	movs r1, #10
	bl 0x0200a2c8
	movs r0, #0
	movs r1, #25
	movs r2, #40
	bl 0x0200a74c
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200a78c
	movs r0, #25
	movs r1, #0
	movs r2, #20
	bl 0x0200a2e0
	movs r2, #30
	movs r0, #26
	adds r1, r6, #0
	bl 0x0200a2e0
.L_02000f80:
	movs r0, #26
	movs r1, #3
	bl 0x0200a724
	movs r0, #26
	movs r1, #30
	bl 0x0200a2c8
	movs r1, #192
	movs r2, #30
	lsls r1, r1, #8
	movs r0, #26
	bl 0x0200a2e0
	movs r0, #26
	movs r1, #3
	bl 0x0200a724
	movs r0, #22
	movs r1, #3
	bl 0x0200a724
	movs r0, #25
	movs r1, #2
.L_02000fb0:
	bl 0x0200a71c
	movs r0, #0
	bl 0x0200a6c4
	cmp r0, #0
	beq .L_02000fb0_0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #25
	bl 0x0200a6f4
.L_02000fb0_0:
	movs r0, #25
	bl 0x0200a70c
	movs r0, #25
	movs r1, #0
	movs r2, #0
	bl 0x0200a714
	movs r0, #26
	movs r1, #2
	bl 0x0200a71c
	movs r0, #0
	bl 0x0200a6c4
	cmp r0, #0
	beq 0x02008ffc
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #26
	bl 0x0200a6f4
.L_02000ffc:
	movs r0, #26
	bl 0x0200a70c
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl 0x0200a714
	movs r0, #22
	movs r1, #2
	bl 0x0200a71c
	movs r0, #0
	bl 0x0200a6c4
.L_0200101a:
	cmp r0, #0
	beq .L_0200101a_0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #22
	bl 0x0200a6f4
.L_0200101a_0:
	movs r0, #22
	bl 0x0200a70c
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl 0x0200a714
	movs r1, #128
	ldr r2, [pc, #64]
	movs r0, #19
	lsls r1, r1, #9
	bl 0x0200a75c
	movs r0, #132
	lsls r0, r0, #4
	bl 0x0200a67c
	bl 0x0200a6a4
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x4013
	.2byte 0x0000
	.2byte 0x3333
	.2byte 0x0001
	.2byte 0x2666
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x02f6
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x1999
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x030a
	.2byte 0xa874
	.2byte 0x0200
	.4byte 0x0200ac00
	.global Func_02001084
	.thumb_func
Func_02001084:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	bl 0x0200a7c4
	adds r5, r0, #0
	bl 0x0200a69c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl 0x0200a7b4
	movs r0, #1
	bl 0x0200a5ec
	movs r0, #128
	movs r1, #144
	movs r2, #175
	movs r3, #0
	lsls r2, r2, #17
	lsls r1, r1, #16
	lsls r0, r0, #15
	bl 0x0200a7b4
	bl 0x0200a634
	movs r0, #1
	bl 0x0200a5ec
	movs r1, #0
	movs r0, #1
	bl 0x0200a6ac
	bl 0x0200a814
	movs r0, #17
	bl 0x0200a84c
	bl 0x0200a80c
	movs r1, #210
	lsls r1, r1, #15
	ldr r2, [pc, #360]
	movs r0, #23
	bl 0x0200a714
	movs r0, #1
	bl 0x0200a5ec
	movs r0, #0
	ldr r1, [pc, #348]
	ldr r2, [pc, #348]
	bl 0x0200a6cc
	ldr r2, [pc, #348]
	movs r1, #93
	movs r0, #0
	bl 0x0200a704
	ldr r0, [pc, #340]
	bl 0x0200a764
	movs r1, #0
	movs r0, #23
	bl 0x0200a774
	movs r0, #61
	bl 0x0200a84c
	adds r5, #85
	movs r3, #0
	movs r0, #192
	movs r1, #192
	strb r3, [r5]
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200a7ac
	movs r0, #218
	movs r1, #176
	movs r3, #1
	lsls r1, r1, #16
	ldr r2, [pc, #300]
	lsls r0, r0, #15
	bl 0x0200a7b4
	bl 0x0200a7bc
	movs r0, #40
	bl 0x0200a694
	movs r1, #135
	movs r2, #177
	movs r0, #24
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl 0x0200a714
	movs r0, #24
	ldr r1, [pc, #268]
	ldr r2, [pc, #272]
	bl 0x0200a6cc
	movs r2, #129
	movs r1, #126
	lsls r2, r2, #1
	movs r0, #24
	bl 0x0200a6fc
	movs r0, #40
	bl 0x0200a694
	movs r1, #208
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #23
	bl 0x0200a78c
	movs r0, #24
	bl 0x0200a70c
	movs r0, #24
	movs r1, #1
	bl 0x0200a71c
	movs r1, #224
	movs r2, #10
	lsls r1, r1, #7
	movs r0, #24
	bl 0x0200a2e0
	movs r0, #23
	movs r1, #3
	bl 0x0200a724
	movs r0, #24
	movs r1, #4
	bl 0x0200a724
	ldr r6, [pc, #196]
	ldr r0, [pc, #200]
	movs r1, #0
	bl 0x0200a774
	movs r5, #176
	movs r0, #23
	movs r1, #2
	bl 0x0200a73c
	lsls r5, r5, #8
	adds r0, r6, #0
	movs r1, #30
	bl 0x0200a2c8
	adds r1, r5, #0
	movs r2, #20
	movs r0, #24
	bl 0x0200a2e0
	ldr r3, [pc, #160]
	mov r8, r3
	mov r0, r8
	movs r1, #10
	bl 0x0200a2c8
	movs r2, #40
	adds r1, r5, #0
	movs r0, #23
	bl 0x0200a2e0
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200a774
	movs r0, #24
	movs r1, #4
	bl 0x0200a724
	mov r0, r8
	movs r1, #0
	bl 0x0200a774
	movs r1, #240
	movs r2, #10
	lsls r1, r1, #8
	movs r0, #23
	bl 0x0200a2e0
	movs r0, #23
	movs r1, #2
	bl 0x0200a73c
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200a774
	movs r1, #192
	movs r2, #20
	lsls r1, r1, #7
	movs r0, #24
	bl 0x0200a2e0
	movs r1, #3
	movs r0, #24
	bl 0x0200a724
	movs r0, #20
	bl 0x0200a694
	mov r0, r8
	movs r1, #20
	bl 0x0200a2c8
	bl 0x02009274
	bl 0x0200a6a4
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x010b0000
	.4byte 0x00013333
	.4byte 0x00009999
	.4byte 0x00000157
	.4byte 0x00000ed6
	.4byte 0x01190000
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x00008017
	.4byte 0x00002018
	.global Func_02001274
	.thumb_func
Func_02001274:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r1, #15
	movs r0, #25
	bl 0x0200a754
	movs r0, #25
	bl 0x0200a6c4
	movs r1, #0
	bl 0x0200a654
	ldr r2, [pc, #716]
	movs r1, #0
	movs r0, #25
	bl 0x0200a714
	movs r0, #1
	bl 0x0200a5ec
	ldr r0, [pc, #704]
	movs r1, #0
	bl 0x0200a774
	movs r1, #128
	movs r0, #23
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200a79c
	movs r1, #128
	movs r0, #24
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200a79c
	movs r0, #25
	movs r1, #0
	movs r2, #0
	bl 0x0200a714
	movs r1, #160
	movs r0, #23
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200a78c
	movs r2, #160
	lsls r2, r2, #7
	mov r8, r2
	mov r1, r8
	movs r2, #40
	movs r0, #24
	bl 0x0200a2e0
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200a7ac
	movs r0, #178
	movs r1, #176
	movs r3, #1
	lsls r1, r1, #16
	ldr r2, [pc, #616]
	lsls r0, r0, #15
	bl 0x0200a7b4
	bl 0x0200a7bc
	movs r0, #40
	bl 0x0200a694
	movs r1, #224
	movs r0, #23
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a78c
	movs r1, #224
	movs r2, #40
	lsls r1, r1, #7
	movs r0, #24
	bl 0x0200a2e0
	ldr r0, [pc, #580]
	ldr r1, [pc, #580]
	bl 0x0200a7ac
	movs r0, #200
	movs r1, #144
	movs r3, #1
	lsls r0, r0, #15
	lsls r1, r1, #16
	ldr r2, [pc, #568]
	bl 0x0200a7b4
	movs r1, #128
	movs r2, #128
	movs r0, #23
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200a6cc
	movs r1, #128
	movs r2, #128
	movs r0, #24
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200a6cc
	movs r1, #105
	ldr r2, [pc, #536]
	movs r0, #23
	bl 0x0200a6fc
	movs r0, #10
	bl 0x0200a694
	ldr r2, [pc, #524]
	movs r1, #124
	movs r0, #24
	bl 0x0200a6fc
	movs r0, #23
	bl 0x0200a70c
	movs r0, #23
	movs r1, #1
	bl 0x0200a71c
	mov r1, r8
	movs r2, #0
	movs r0, #23
	bl 0x0200a78c
	movs r0, #24
	bl 0x0200a70c
	movs r0, #24
	movs r1, #1
	bl 0x0200a71c
	movs r2, #0
	mov r1, r8
	movs r0, #24
	bl 0x0200a78c
	movs r1, #0
	movs r0, #25
	bl 0x0200a754
	movs r0, #25
	bl 0x0200a6c4
	movs r1, #1
	bl 0x0200a654
	movs r0, #25
	movs r1, #0
	ldr r2, [pc, #416]
	bl 0x0200a714
	movs r0, #25
	ldr r1, [pc, #436]
	ldr r2, [pc, #440]
	bl 0x0200a6cc
	ldr r2, [pc, #436]
	movs r1, #37
	movs r0, #25
	bl 0x0200a704
	movs r0, #20
	bl 0x0200a694
	movs r0, #23
	movs r1, #3
	bl 0x0200a724
	movs r5, #208
	movs r1, #0
	movs r0, #23
	bl 0x0200a76c
	lsls r5, r5, #8
	movs r0, #25
	ldr r1, [pc, #404]
	movs r2, #0
	bl 0x0200a79c
	movs r2, #10
	adds r1, r5, #0
	movs r0, #0
	bl 0x0200a2e0
	movs r1, #0
	movs r0, #0
	bl 0x0200a6bc
	movs r6, #128
	movs r0, #40
	bl 0x0200a694
	lsls r6, r6, #8
	movs r0, #23
	movs r1, #0
	movs r2, #0
	bl 0x0200a78c
	movs r2, #20
	adds r1, r6, #0
	movs r0, #24
	bl 0x0200a2e0
	movs r0, #25
	movs r1, #2
	bl 0x0200a73c
	ldr r0, [pc, #304]
	movs r1, #10
	bl 0x0200a2c8
	ldr r2, [pc, #336]
	movs r0, #0
	ldr r1, [pc, #336]
	bl 0x0200a75c
	movs r0, #25
	movs r1, #93
	ldr r2, [pc, #328]
	bl 0x0200a704
	movs r2, #40
	adds r1, r5, #0
	movs r0, #25
	bl 0x0200a2e0
	movs r1, #20
	movs r0, #25
	bl 0x0200a2c8
	movs r0, #0
	bl 0x0200a6e4
	movs r0, #23
	movs r1, #0
	movs r2, #0
	bl 0x0200a78c
	movs r2, #15
	adds r1, r6, #0
	movs r0, #24
	bl 0x0200a2e0
	movs r0, #23
	movs r1, #3
	bl 0x0200a71c
	movs r0, #24
	movs r1, #3
	bl 0x0200a724
	mov r1, r8
	movs r0, #23
	movs r2, #0
	bl 0x0200a78c
	movs r2, #30
	mov r1, r8
	movs r0, #24
	bl 0x0200a2e0
	movs r0, #24
	movs r1, #4
	bl 0x0200a724
	ldr r0, [pc, #240]
	movs r1, #0
	bl 0x0200a774
	adds r1, r5, #0
	movs r0, #0
	movs r2, #30
	bl 0x0200a2e0
	movs r1, #128
	movs r2, #40
	lsls r1, r1, #7
	movs r0, #0
	bl 0x0200a2e0
	movs r0, #23
	movs r1, #2
	bl 0x0200a73c
	movs r0, #23
	movs r1, #3
	bl 0x0200a724
	movs r0, #23
	movs r1, #20
	bl 0x0200a2c8
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200a7a4
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #25
	bl 0x0200a7a4
	movs r0, #80
	bl 0x0200a694
	ldr r1, [pc, #164]
	movs r0, #24
	bl 0x0200a6d4
	movs r0, #6
	bl 0x0200a694
	ldr r1, [pc, #152]
	movs r0, #23
	bl 0x0200a6d4
	movs r0, #20
	bl 0x0200a694
	ldr r1, [pc, #144]
	movs r0, #0
	bl 0x0200a6d4
	movs r0, #6
	bl 0x0200a694
	ldr r1, [pc, #132]
	movs r0, #25
	bl 0x0200a6ec
	ldr r3, [pc, #128]
	ldr r2, [pc, #132]
	adds r3, r3, r2
	movs r2, #2
	strb r2, [r3]
	ldr r5, [pc, #128]
	movs r1, #19
	adds r0, r5, #0
	bl 0x0200a7dc
	adds r0, r5, #0
	movs r1, #19
	bl 0x0200a7e4
	movs r0, #12
	movs r1, #4
	bl 0x0200a7d4
	movs r0, #141
	lsls r0, r0, #1
	bl 0x0200a67c
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x014b0000
	.4byte 0x00001019
	.4byte 0x01390000
	.4byte 0x0000cccc
	.4byte 0x00001999
	.4byte 0x014d0000
	.4byte 0x00000149
	.4byte 0x00013333
	.4byte 0x00009999
	.4byte 0x00000153
	.4byte 0x00000101
	.4byte 0x0200ac00
	.4byte 0x00010019
	.4byte 0x00000169
	.4byte 0x00002018
	.4byte 0x0200a8e8
	.4byte 0x0200a940
	.4byte 0x0200a998
	.4byte 0x0200a9f0
	.4byte 0x02000240
	.4byte 0x0000022b
	.4byte 0x00000005
	.global Func_020015b4
	.thumb_func
Func_020015b4:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r0, #17
	bl 0x0200a84c
	bl 0x0200a69c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl 0x0200a7b4
	movs r0, #1
	bl 0x0200a5ec
	movs r0, #128
	movs r1, #144
	movs r2, #175
	movs r3, #0
	lsls r1, r1, #16
	lsls r2, r2, #17
	lsls r0, r0, #15
	bl 0x0200a7b4
	bl 0x0200a634
	movs r0, #1
	bl 0x0200a5ec
	movs r1, #192
	movs r2, #173
	movs r0, #0
	lsls r1, r1, #14
	lsls r2, r2, #17
	bl 0x0200a714
	movs r1, #156
	movs r2, #179
	movs r0, #25
	lsls r1, r1, #15
	lsls r2, r2, #17
	bl 0x0200a714
	movs r1, #206
	movs r2, #171
	movs r0, #23
	lsls r1, r1, #15
	lsls r2, r2, #17
	bl 0x0200a714
	movs r1, #224
	movs r2, #180
	movs r0, #24
	lsls r1, r1, #15
	lsls r2, r2, #17
	bl 0x0200a714
	movs r1, #128
	movs r0, #23
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200a78c
	movs r1, #128
	movs r2, #0
	movs r0, #24
	lsls r1, r1, #8
	bl 0x0200a78c
	movs r1, #16
	movs r0, #0
	bl 0x0200a71c
	movs r0, #0
	bl 0x0200a6c4
	ldr r3, [pc, #332]
	str r3, [r0, #24]
	movs r0, #0
	bl 0x0200a6c4
	movs r1, #0
	bl 0x0200a654
	movs r1, #7
	movs r0, #25
	bl 0x0200a71c
	movs r0, #25
	bl 0x0200a6c4
	ldr r3, [pc, #304]
	ldr r2, [r0, #80]
	movs r0, #25
	strh r3, [r2, #30]
	bl 0x0200a6c4
	movs r1, #0
	bl 0x0200a654
	ldr r2, [pc, #292]
	mov r8, r2
	ldr r3, [r2]
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #192
	str r2, [r3]
	bl 0x0200a814
	bl 0x0200a80c
	bl 0x0200a824
	movs r0, #80
	bl 0x0200a694
	movs r1, #192
	movs r0, #23
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200a78c
	movs r1, #192
	movs r2, #40
	movs r0, #24
	lsls r1, r1, #8
	bl 0x0200a78c
	movs r1, #3
	movs r0, #23
	bl 0x0200a724
	movs r0, #20
	bl 0x0200a694
	movs r0, #24
	movs r1, #3
	bl 0x0200a724
	movs r1, #128
	movs r0, #23
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200a78c
	movs r1, #128
	movs r2, #10
	movs r0, #24
	lsls r1, r1, #8
	bl 0x0200a78c
	movs r0, #0
	movs r1, #3
	bl 0x0200a794
	movs r0, #25
	movs r1, #3
	bl 0x0200a794
	ldr r2, [pc, #176]
	ldr r1, [pc, #176]
	movs r0, #23
	bl 0x0200a6cc
	movs r0, #23
	bl 0x0200a6c4
	ldr r6, [pc, #168]
	movs r5, #128
	lsls r5, r5, #8
	str r6, [r0, #68]
	str r5, [r0, #72]
	ldr r1, [pc, #160]
	movs r0, #23
	bl 0x0200a6d4
	movs r0, #24
	bl 0x0200a694
	ldr r2, [pc, #136]
	ldr r1, [pc, #136]
	movs r0, #24
	bl 0x0200a6cc
	movs r0, #24
	bl 0x0200a6c4
	ldr r1, [pc, #136]
	str r6, [r0, #68]
	str r5, [r0, #72]
	movs r0, #24
	bl 0x0200a6ec
	movs r0, #40
	bl 0x0200a694
	bl 0x0200a7fc
	bl 0x0200a804
	movs r0, #20
	bl 0x0200a5ec
	bl 0x0200a804
	movs r0, #60
	bl 0x0200a5ec
	bl 0x0200a804
	movs r0, #20
	bl 0x0200a5ec
	bl 0x0200a7fc
	movs r0, #40
	bl 0x0200a694
	mov r2, r8
	ldr r3, [r2]
	movs r2, #228
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #120
	str r2, [r3]
	bl 0x0200a81c
	bl 0x0200a824
	ldr r0, [pc, #56]
	bl 0x0200a684
	movs r0, #9
	bl 0x0200a7cc
	bl 0x0200a6a4
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0xffff0000
	.4byte 0x00001555
	.4byte 0x03001ebc
	.4byte 0x00013333
	.4byte 0x00026666
	.4byte 0x0000028f
	.4byte 0x0200aa48
	.4byte 0x0200ab2c
	.4byte 0x00000834
	.global Func_020017c8
	.thumb_func
Func_020017c8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #1
	sub sp, #8
	bl 0x0200a68c
	bl 0x0200a69c
	movs r3, #3
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #42
	movs r1, #53
	movs r2, #42
	movs r3, #54
	bl 0x0200a644
	movs r0, #180
	movs r1, #128
	movs r3, #0
	ldr r2, [pc, #1012]
	lsls r1, r1, #13
	lsls r0, r0, #16
	bl 0x0200a7b4
	bl 0x0200a634
	movs r0, #1
	bl 0x0200a5ec
	movs r0, #22
	bl 0x0200a6c4
	movs r1, #0
	bl 0x0200a654
	movs r0, #23
	bl 0x0200a6c4
	movs r1, #0
	bl 0x0200a654
	movs r0, #24
	bl 0x0200a6c4
	movs r1, #0
	bl 0x0200a654
	movs r0, #25
	bl 0x0200a6c4
	movs r1, #0
	bl 0x0200a654
	movs r0, #26
	bl 0x0200a6c4
	movs r1, #0
	bl 0x0200a654
	movs r0, #29
	bl 0x0200a6c4
	movs r1, #0
	bl 0x0200a654
	movs r0, #0
	movs r1, #1
	bl 0x0200a794
	movs r0, #1
	movs r1, #1
	bl 0x0200a794
	movs r0, #17
	movs r1, #1
	bl 0x0200a794
	movs r0, #16
	movs r1, #1
	bl 0x0200a794
	movs r0, #15
	movs r1, #1
	bl 0x0200a794
	movs r1, #208
	lsls r1, r1, #16
	ldr r2, [pc, #880]
	movs r0, #0
	bl 0x0200a714
	bl 0x0200a814
	bl 0x0200a824
	movs r0, #80
	bl 0x0200a694
	movs r0, #12
	ldr r1, [pc, #860]
	movs r2, #40
	bl 0x0200a79c
	movs r3, #224
	lsls r3, r3, #7
	mov r8, r3
	movs r2, #20
	mov r1, r8
	movs r0, #12
	bl 0x0200a2e0
	ldr r0, [pc, #840]
	bl 0x0200a764
	movs r0, #12
	movs r1, #10
	bl 0x0200a2c8
	movs r1, #129
	movs r0, #11
	lsls r1, r1, #1
	movs r2, #20
	bl 0x0200a79c
	movs r3, #128
	lsls r3, r3, #5
	mov r11, r3
	movs r2, #10
	movs r0, #11
	mov r1, r11
	bl 0x0200a2e0
	movs r0, #11
	movs r1, #10
	bl 0x0200a2c8
	movs r1, #3
	movs r0, #12
	bl 0x0200a724
	movs r0, #10
	bl 0x0200a694
	movs r0, #11
	movs r1, #2
	bl 0x0200a73c
	movs r0, #11
	movs r1, #10
	bl 0x0200a2c8
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200a79c
	movs r7, #192
	movs r0, #12
	ldr r1, [pc, #748]
	ldr r2, [pc, #752]
	lsls r7, r7, #6
	bl 0x0200a6cc
	movs r0, #12
	movs r1, #184
	ldr r2, [pc, #744]
	bl 0x0200a704
	movs r2, #60
	movs r0, #12
	adds r1, r7, #0
	bl 0x0200a2e0
	movs r0, #12
	movs r1, #20
	bl 0x0200a2c8
	movs r1, #128
	movs r2, #128
	movs r0, #11
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200a6cc
	movs r0, #11
	movs r1, #168
	ldr r2, [pc, #700]
	bl 0x0200a704
	movs r1, #240
	movs r2, #10
	movs r0, #11
	lsls r1, r1, #8
	bl 0x0200a2e0
	movs r0, #11
	movs r1, #4
	bl 0x0200a71c
	movs r0, #11
	movs r1, #20
	bl 0x0200a2c8
	movs r2, #10
	movs r0, #12
	mov r1, r8
	bl 0x0200a2e0
	movs r0, #12
	movs r1, #1
	bl 0x0200a73c
	movs r1, #0
	movs r0, #12
	bl 0x0200a774
	movs r0, #10
	bl 0x0200a694
	movs r0, #30
	ldr r1, [pc, #636]
	ldr r2, [pc, #640]
	bl 0x0200a6cc
	movs r1, #220
	movs r2, #186
	lsls r2, r2, #18
	lsls r1, r1, #15
	movs r0, #30
	bl 0x0200a714
	movs r0, #2
	bl 0x0200a5ec
	movs r0, #30
	movs r1, #3
	bl 0x0200a71c
	ldr r1, [pc, #608]
	movs r0, #30
	bl 0x0200a6d4
	movs r0, #40
	bl 0x0200a694
	ldr r5, [pc, #600]
	movs r0, #11
	ldr r1, [pc, #600]
	adds r2, r5, #0
	bl 0x0200a75c
	ldr r1, [pc, #592]
	adds r2, r5, #0
	movs r0, #12
	bl 0x0200a75c
	movs r0, #30
	bl 0x0200a6dc
	movs r0, #11
	bl 0x0200a6e4
	movs r0, #12
	bl 0x0200a6e4
	movs r0, #60
	bl 0x0200a694
	movs r0, #11
	ldr r1, [pc, #560]
	movs r2, #0
	bl 0x0200a79c
	movs r0, #12
	ldr r1, [pc, #548]
	movs r2, #120
	bl 0x0200a79c
	movs r0, #11
	mov r1, r11
	movs r2, #0
	bl 0x0200a78c
	movs r0, #12
	mov r1, r8
	movs r2, #80
	bl 0x0200a2e0
	movs r3, #160
	lsls r3, r3, #7
	mov r9, r3
	movs r0, #11
	mov r1, r9
	movs r2, #40
	bl 0x0200a2e0
	movs r2, #20
	movs r0, #11
	mov r1, r11
	bl 0x0200a2e0
	movs r1, #3
	movs r0, #11
	bl 0x0200a724
	movs r0, #20
	bl 0x0200a694
	movs r0, #12
	mov r1, r9
	movs r2, #60
	bl 0x0200a2e0
	movs r0, #12
	adds r1, r7, #0
	movs r2, #40
	bl 0x0200a2e0
	movs r0, #12
	mov r1, r9
	movs r2, #60
	bl 0x0200a2e0
	movs r0, #12
	ldr r1, [pc, #408]
	movs r2, #80
	bl 0x0200a79c
	movs r0, #11
	adds r1, r7, #0
	movs r2, #0
	bl 0x0200a78c
	movs r1, #184
	ldr r2, [pc, #432]
	movs r0, #12
	bl 0x0200a704
	movs r0, #20
	bl 0x0200a694
	movs r0, #12
	adds r1, r7, #0
	movs r2, #20
	bl 0x0200a2e0
	movs r0, #12
	mov r1, r9
	movs r2, #20
	bl 0x0200a2e0
	movs r0, #12
	adds r1, r7, #0
	movs r2, #20
	bl 0x0200a2e0
	movs r0, #12
	ldr r1, [pc, #344]
	movs r2, #40
	bl 0x0200a79c
	movs r0, #11
	ldr r1, [pc, #332]
	movs r2, #40
	bl 0x0200a79c
	movs r1, #168
	ldr r2, [pc, #368]
	movs r0, #11
	bl 0x0200a704
	movs r0, #20
	bl 0x0200a694
	movs r0, #11
	adds r1, r7, #0
	movs r2, #40
	bl 0x0200a2e0
	movs r0, #11
	mov r1, r9
	movs r2, #40
	bl 0x0200a2e0
	movs r0, #11
	adds r1, r7, #0
	movs r2, #40
	bl 0x0200a2e0
	movs r0, #11
	ldr r1, [pc, #276]
	movs r2, #40
	bl 0x0200a79c
	movs r2, #10
	movs r0, #11
	mov r1, r11
	bl 0x0200a2e0
	movs r0, #11
	movs r1, #20
	bl 0x0200a2c8
	movs r0, #12
	movs r1, #3
	bl 0x0200a724
	movs r0, #12
	movs r1, #10
	bl 0x0200a2c8
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #1
	movs r2, #20
	bl 0x0200a79c
	movs r0, #11
	mov r1, r9
	movs r2, #20
	bl 0x0200a2e0
	movs r0, #11
	adds r1, r7, #0
	movs r2, #20
	bl 0x0200a2e0
	movs r0, #11
	mov r1, r9
	movs r2, #20
	bl 0x0200a2e0
	movs r2, #60
	movs r0, #11
	mov r1, r9
	bl 0x0200a2e0
	movs r0, #11
	movs r1, #3
	bl 0x0200a724
	movs r0, #11
	movs r1, #10
	bl 0x0200a2c8
	movs r0, #30
	bl 0x0200a6c4
	cmp r0, #0
	beq .L_020017c8_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #31
	bl 0x0200a714
.L_020017c8_0:
	movs r0, #2
	bl 0x0200a5ec
	movs r0, #30
	bl 0x0200a6c4
	adds r0, #35
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #31
	bl 0x0200a6c4
	adds r0, #35
	ldrb r3, [r0]
	ands r5, r3
	strb r5, [r0]
	movs r1, #2
	movs r0, #30
	bl 0x0200a794
	movs r0, #31
	movs r1, #2
	bl 0x0200a794
	ldr r2, [pc, #140]
	movs r0, #31
	ldr r1, [pc, #140]
	bl 0x0200a6cc
	movs r0, #31
	movs r1, #2
	bl 0x0200a71c
	ldr r5, [pc, #132]
	movs r0, #31
	adds r1, r5, #0
	bl 0x0200a6d4
	movs r0, #20
	bl 0x0200a694
	movs r0, #30
	movs r1, #3
	bl 0x0200a71c
	ldr r2, [pc, #72]
	movs r0, #30
	ldr r1, [pc, #108]
	bl 0x0200a6cc
	adds r1, r5, #0
	movs r0, #30
	bl 0x0200a6ec
	movs r0, #60
	bl 0x0200a694
	movs r0, #12
	movs r1, #2
	bl 0x0200a73c
	movs r2, #10
	mov r1, r8
	movs r0, #12
	bl 0x0200a2e0
	movs r0, #12
	b .L_020017c8_1
	.4byte 0x026a0000
	.4byte 0x032e0000
	.4byte 0x00000101
	.4byte 0x000011fa
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x0000026a
	.4byte 0x00026666
	.4byte 0x00013333
	.4byte 0x0200ac14
	.4byte 0x0200ac00
	.4byte 0x0001001e
	.4byte 0x00000105
	.4byte 0x00000276
	.4byte 0x0001cccc
	.4byte 0x00039999
	.4byte 0x0200ac90
	.4byte 0x0004cccc
.L_020017c8_1:
	movs r1, #10
	bl 0x0200a2c8
	movs r1, #1
	movs r0, #11
	bl 0x0200a73c
	movs r0, #20
	bl 0x0200a694
	movs r2, #10
	mov r1, r11
	movs r0, #11
	bl 0x0200a2e0
	movs r0, #11
	movs r1, #3
	bl 0x0200a724
	movs r0, #11
	movs r1, #20
	bl 0x0200a2c8
	movs r1, #3
	movs r0, #12
	bl 0x0200a71c
	movs r0, #10
	bl 0x0200a694
	movs r1, #3
	movs r0, #11
	bl 0x0200a724
	movs r0, #20
	bl 0x0200a694
	movs r0, #11
	ldr r1, [pc, #1016]
	ldr r2, [pc, #1020]
	bl 0x0200a6cc
	ldr r2, [pc, #1012]
	movs r0, #12
	ldr r1, [pc, #1004]
	bl 0x0200a6cc
	ldr r1, [pc, #1008]
	movs r0, #11
	bl 0x0200a6d4
	movs r0, #10
	bl 0x0200a694
	ldr r1, [pc, #996]
	ldr r0, [pc, #984]
	bl 0x0200a7ac
	bl 0x0200a7c4
	movs r6, #0
	adds r0, #85
	strb r6, [r0]
	movs r1, #128
	movs r0, #215
	movs r3, #1
	ldr r2, [pc, #976]
	lsls r1, r1, #13
	lsls r0, r0, #16
	bl 0x0200a7b4
	movs r0, #10
	bl 0x0200a694
	ldr r1, [pc, #964]
	movs r0, #12
	bl 0x0200a6d4
	movs r0, #12
	bl 0x0200a6dc
	movs r2, #120
	adds r1, r7, #0
	movs r0, #12
	bl 0x0200a2e0
	movs r1, #2
	movs r0, #13
	bl 0x0200a73c
	movs r0, #20
	bl 0x0200a694
	movs r6, #144
	movs r0, #13
	movs r1, #20
	bl 0x0200a2c8
	lsls r6, r6, #8
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200a78c
	adds r1, r6, #0
	movs r0, #1
	movs r2, #20
	bl 0x0200a2e0
	movs r3, #192
	lsls r3, r3, #8
	mov r10, r3
	mov r1, r10
	movs r0, #0
	movs r2, #10
	bl 0x0200a2e0
	movs r3, #176
	lsls r3, r3, #8
	mov r8, r3
	movs r2, #10
	mov r1, r8
	movs r0, #1
	bl 0x0200a2e0
	movs r1, #3
	movs r0, #0
	bl 0x0200a71c
	movs r0, #10
	bl 0x0200a694
	movs r1, #3
	movs r0, #1
	bl 0x0200a724
	movs r0, #40
	bl 0x0200a694
	movs r1, #2
	movs r0, #16
	bl 0x0200a73c
	movs r0, #20
	bl 0x0200a694
	movs r0, #16
	movs r1, #10
	bl 0x0200a2c8
	movs r1, #128
	movs r2, #128
	movs r0, #16
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200a6cc
	movs r2, #200
	movs r0, #16
	movs r1, #216
	lsls r2, r2, #2
	bl 0x0200a704
	movs r1, #128
	movs r2, #0
	movs r0, #16
	lsls r1, r1, #7
	bl 0x0200a78c
	movs r1, #0
	movs r0, #180
	bl 0x0200a6b4
	movs r1, #132
	movs r2, #200
	movs r0, #16
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200a704
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200a78c
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200a79c
	movs r1, #240
	movs r2, #10
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200a2e0
	movs r0, #1
	movs r1, #10
	bl 0x0200a2c8
	movs r0, #17
	movs r1, #4
	bl 0x0200a724
	movs r0, #17
	movs r1, #10
	bl 0x0200a2c8
	mov r1, r11
	movs r0, #1
	movs r2, #0
	bl 0x0200a78c
	movs r0, #1
	ldr r1, [pc, #684]
	movs r2, #20
	bl 0x0200a79c
	movs r2, #60
	movs r0, #1
	movs r1, #4
	bl 0x0200a72c
	movs r5, #208
	movs r1, #2
	movs r0, #14
	bl 0x0200a73c
	lsls r5, r5, #8
	movs r0, #20
	bl 0x0200a694
	adds r1, r5, #0
	movs r2, #10
	movs r0, #14
	bl 0x0200a2e0
	movs r0, #14
	movs r1, #60
	bl 0x0200a2c8
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200a79c
	movs r1, #129
	movs r0, #16
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200a79c
	movs r1, #129
	movs r0, #17
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200a79c
	movs r1, #129
	movs r0, #18
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200a79c
	movs r1, #129
	movs r0, #19
	lsls r1, r1, #1
	movs r2, #80
	bl 0x0200a79c
	movs r1, #128
	movs r2, #0
	movs r0, #17
	lsls r1, r1, #1
	bl 0x0200a79c
	movs r0, #17
	movs r1, #60
	bl 0x0200a2c8
	movs r0, #0
	movs r1, #17
	bl 0x0200a844
	movs r0, #1
	movs r1, #17
	bl 0x0200a844
	movs r2, #200
	movs r0, #17
	movs r1, #216
	lsls r2, r2, #2
	bl 0x0200a704
	movs r1, #128
	movs r2, #0
	movs r0, #17
	lsls r1, r1, #7
	bl 0x0200a78c
	movs r0, #17
	movs r1, #60
	bl 0x0200a2c8
	movs r1, #0
	movs r0, #207
	bl 0x0200a6b4
	movs r0, #0
	bl 0x0200a6e4
	movs r0, #1
	bl 0x0200a6e4
	movs r1, #136
	movs r2, #204
	movs r0, #17
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200a704
	movs r1, #128
	movs r2, #0
	movs r0, #17
	lsls r1, r1, #8
	bl 0x0200a78c
	movs r0, #1
	movs r1, #2
	bl 0x0200a73c
	adds r1, r6, #0
	movs r2, #10
	movs r0, #1
	bl 0x0200a2e0
	movs r0, #1
	movs r1, #10
	bl 0x0200a2c8
	adds r1, r7, #0
	movs r0, #14
	movs r2, #0
	bl 0x0200a78c
	movs r0, #0
	movs r1, #0
	movs r2, #10
	bl 0x0200a2e0
	movs r2, #60
	movs r0, #0
	ldr r1, [pc, #412]
	bl 0x0200a79c
	movs r0, #16
	movs r1, #1
	bl 0x0200a73c
	movs r0, #16
	movs r1, #10
	bl 0x0200a2c8
	movs r1, #240
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200a2e0
	movs r2, #20
	movs r0, #1
	ldr r1, [pc, #372]
	bl 0x0200a79c
	movs r0, #16
	movs r1, #4
	bl 0x0200a724
	movs r0, #16
	movs r1, #10
	bl 0x0200a2c8
	movs r0, #18
	movs r1, #3
	bl 0x0200a724
	movs r0, #18
	movs r1, #0
	bl 0x0200a774
	movs r2, #0
	adds r1, r5, #0
	movs r0, #1
	bl 0x0200a78c
	movs r0, #18
	movs r1, #4
	bl 0x0200a71c
	movs r0, #18
	movs r1, #0
	bl 0x0200a774
	movs r0, #18
	movs r1, #3
	bl 0x0200a734
	movs r0, #18
	movs r1, #0
	bl 0x0200a774
	movs r0, #16
	movs r1, #3
	bl 0x0200a724
	movs r0, #19
	movs r1, #3
	bl 0x0200a71c
	movs r1, #3
	movs r0, #17
	bl 0x0200a71c
	movs r0, #10
	bl 0x0200a694
	movs r0, #24
	movs r1, #3
	bl 0x0200a71c
	movs r0, #18
	movs r1, #3
	bl 0x0200a71c
	movs r1, #3
	movs r0, #27
	bl 0x0200a71c
	movs r0, #10
	bl 0x0200a694
	movs r1, #3
	movs r0, #28
	bl 0x0200a71c
	movs r0, #10
	bl 0x0200a694
	movs r0, #25
	movs r1, #3
	bl 0x0200a71c
	movs r0, #20
	movs r1, #3
	bl 0x0200a71c
	movs r0, #21
	movs r1, #3
	bl 0x0200a724
	movs r0, #15
	movs r1, #2
	movs r2, #10
	bl 0x0200a72c
	movs r2, #40
	movs r0, #15
	movs r1, #4
	bl 0x0200a72c
	movs r0, #15
	movs r1, #10
	bl 0x0200a2c8
	mov r1, r8
	movs r0, #1
	movs r2, #0
	bl 0x0200a78c
	mov r1, r10
	movs r0, #0
	movs r2, #20
	bl 0x0200a2e0
	adds r1, r5, #0
	movs r2, #10
	movs r0, #15
	bl 0x0200a2e0
	movs r0, #15
	movs r1, #10
	bl 0x0200a2c8
	adds r1, r6, #0
	movs r0, #15
	movs r2, #20
	bl 0x0200a2e0
	movs r2, #10
	mov r1, r9
	movs r0, #15
	bl 0x0200a2e0
	movs r0, #11
	movs r1, #3
	bl 0x0200a71c
	movs r0, #14
	movs r1, #3
	bl 0x0200a71c
	movs r0, #17
	movs r1, #3
	bl 0x0200a71c
	movs r0, #20
	movs r1, #3
	bl 0x0200a71c
	movs r0, #23
	movs r1, #3
	bl 0x0200a71c
	movs r0, #26
	movs r1, #3
	bl 0x0200a71c
	movs r1, #3
	movs r0, #29
	bl 0x0200a71c
	movs r0, #10
	bl 0x0200a694
	movs r0, #12
	movs r1, #3
	bl 0x0200a71c
	movs r0, #15
	movs r1, #3
	bl 0x0200a71c
	movs r0, #18
	b .L_020017c8_2
	.2byte 0x0000
	.4byte 0x00026666
	.4byte 0x00013333
	.4byte 0x0200acf8
	.4byte 0x00004ccc
	.4byte 0x03210000
	.4byte 0x0200ad74
	.4byte 0x00000103
	.4byte 0x00000101
.L_020017c8_2:
	movs r1, #3
	bl 0x0200a71c
	movs r0, #21
	movs r1, #3
	bl 0x0200a71c
	movs r0, #24
	movs r1, #3
	bl 0x0200a71c
	movs r1, #3
	movs r0, #27
	bl 0x0200a71c
	movs r0, #10
	bl 0x0200a694
	movs r0, #13
	movs r1, #3
	bl 0x0200a71c
	movs r0, #16
	movs r1, #3
	bl 0x0200a71c
	movs r0, #19
	movs r1, #3
	bl 0x0200a71c
	movs r0, #22
	movs r1, #3
	bl 0x0200a71c
	movs r0, #25
	movs r1, #3
	bl 0x0200a71c
	movs r1, #3
	movs r0, #28
	bl 0x0200a724
	movs r0, #80
	bl 0x0200a694
	movs r0, #11
	movs r1, #4
	movs r2, #0
	bl 0x0200a72c
	movs r0, #14
	movs r1, #4
	movs r2, #0
	bl 0x0200a72c
	movs r0, #17
	movs r1, #4
	movs r2, #0
	bl 0x0200a72c
	movs r0, #20
	movs r1, #4
	movs r2, #0
	bl 0x0200a72c
	movs r0, #23
	movs r1, #4
	movs r2, #0
	bl 0x0200a72c
	movs r0, #26
	movs r1, #4
	movs r2, #0
	bl 0x0200a72c
	movs r0, #29
	movs r1, #4
	movs r2, #0
	bl 0x0200a72c
	movs r0, #12
	movs r1, #4
	movs r2, #0
	bl 0x0200a72c
	movs r0, #15
	movs r1, #4
	movs r2, #0
	bl 0x0200a72c
	movs r0, #18
	movs r1, #4
	movs r2, #0
	bl 0x0200a72c
	movs r0, #21
	movs r1, #4
	movs r2, #0
	bl 0x0200a72c
	movs r0, #24
	movs r1, #4
	movs r2, #0
	bl 0x0200a72c
	movs r0, #27
	movs r1, #4
	movs r2, #0
	bl 0x0200a72c
	movs r0, #13
	movs r1, #4
	movs r2, #0
	bl 0x0200a72c
	movs r0, #16
	movs r1, #4
	movs r2, #0
	bl 0x0200a72c
	movs r0, #19
	movs r1, #4
	movs r2, #0
	bl 0x0200a72c
	movs r0, #22
	movs r1, #4
	movs r2, #0
	bl 0x0200a72c
	movs r0, #25
	movs r1, #4
	movs r2, #0
	bl 0x0200a72c
	movs r2, #0
	movs r0, #28
	movs r1, #4
	bl 0x0200a72c
	movs r1, #1
	ldr r0, [pc, #236]
	bl 0x0200a664
	movs r0, #80
	bl 0x0200a694
	movs r0, #0
	bl 0x0200a6c4
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #1
	bl 0x0200a6c4
	adds r0, #35
	ldrb r3, [r0]
	movs r1, #129
	orrs r5, r3
	strb r5, [r0]
	lsls r1, r1, #1
	movs r0, #0
	movs r2, #0
	bl 0x0200a79c
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #80
	bl 0x0200a79c
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #0
	movs r2, #10
	bl 0x0200a2e0
	mov r1, r9
	movs r0, #1
	movs r2, #20
	bl 0x0200a2e0
	ldr r2, [pc, #152]
	movs r0, #0
	ldr r1, [pc, #152]
	bl 0x0200a6cc
	ldr r5, [pc, #148]
	movs r0, #0
	adds r1, r5, #0
	bl 0x0200a6d4
	movs r0, #20
	bl 0x0200a694
	ldr r0, [pc, #124]
	ldr r1, [pc, #136]
	bl 0x0200a7ac
	movs r0, #216
	movs r1, #128
	movs r3, #1
	lsls r1, r1, #13
	ldr r2, [pc, #124]
	lsls r0, r0, #16
	bl 0x0200a7b4
	movs r0, #20
	bl 0x0200a694
	ldr r2, [pc, #96]
	movs r0, #1
	ldr r1, [pc, #96]
	bl 0x0200a6cc
	adds r1, r5, #0
	movs r0, #1
	bl 0x0200a6d4
	movs r0, #60
	bl 0x0200a694
	ldr r3, [pc, #92]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	subs r3, #192
	str r3, [r2]
	adds r3, #200
	adds r2, r1, r3
	movs r3, #60
	str r3, [r2]
	bl 0x0200a81c
	bl 0x0200a824
	movs r0, #0
	bl 0x0200a6e4
	movs r0, #1
	bl 0x0200a6e4
	movs r0, #10
	bl 0x0200a7cc
	bl 0x0200a6a4
	sub sp, #-8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x00001214
	.4byte 0x00006666
	.4byte 0x0000cccc
	.4byte 0x0200adf0
	.4byte 0x00000ccc
	.4byte 0x03890000
	.4byte 0x03001ebc
	.global Func_020022c8
	.thumb_func
Func_020022c8:
	push {r5, lr}
	adds r5, r1, #0
	movs r1, #0
	bl 0x0200a774
	adds r0, r5, #0
	bl 0x0200a694
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020022e0
	.thumb_func
Func_020022e0:
	push {r5, lr}
	adds r5, r2, #0
	movs r2, #0
	bl 0x0200a78c
	adds r0, r5, #0
	bl 0x0200a694
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020022f8
	.thumb_func
Func_020022f8:
	push {r5, lr}
	ldr r3, [pc, #52]
	ldr r3, [r3]
	movs r2, #2
	ands r3, r2
	adds r5, r0, #0
	cmp r3, #0
	beq .L_020022f8_0
	movs r1, #7
	bl 0x0200a65c
	b .L_020022f8_1
.L_020022f8_0:
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200a65c
.L_020022f8_1:
	ldr r3, [pc, #20]
	ldr r3, [r3]
	movs r2, #15
	ands r3, r2
	cmp r3, #0
	bne .L_020022f8_2
	adds r0, r5, #0
	bl 0x0200a440
.L_020022f8_2:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001e40
	.global Func_02002334
	.thumb_func
Func_02002334:
	push {r5, r6, lr}
	ldr r5, [pc, #52]
	ldr r3, [r5]
	movs r2, #1
	ands r3, r2
	adds r6, r0, #0
	cmp r3, #0
	beq .L_02002334_0
	ldr r0, [r5]
	movs r1, #6
	lsrs r0, r0, #1
	bl 0x0200a5e4
	adds r1, r0, #0
	adds r0, r6, #0
	bl 0x0200a65c
.L_02002334_0:
	ldr r3, [r5]
	movs r2, #15
	ands r3, r2
	cmp r3, #0
	bne .L_02002334_1
	adds r0, r6, #0
	bl 0x0200a440
.L_02002334_1:
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x03001e40
	.global Func_02002370
	.thumb_func
Func_02002370:
	push {r5, lr}
	adds r5, r0, #0
	ldr r0, [pc, #32]
	ldr r3, [r0]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02002370_0
	ldr r0, [r0]
	movs r1, #6
	lsrs r0, r0, #1
	bl 0x0200a5e4
	adds r1, r0, #0
	adds r0, r5, #0
	bl 0x0200a65c
.L_02002370_0:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001e40
	.global Func_0200239c
	.thumb_func
Func_0200239c:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #100
	ldrh r3, [r2]
	adds r3, #1
	ldr r6, [r5, #104]
	strh r3, [r2]
	lsls r3, r3, #16
	asrs r0, r3, #16
	cmp r0, #31
	ble .L_0200239c_0
	adds r0, r5, #0
	bl 0x0200a62c
	b .L_0200239c_1
.L_0200239c_0:
	lsls r0, r0, #10
	bl 0x0200a60c
	str r0, [r5, #24]
	str r0, [r5, #28]
	ldr r3, [r6, #8]
	movs r1, #128
	str r3, [r5, #8]
	ldr r3, [r5, #12]
	lsls r1, r1, #9
	adds r3, r3, r1
	str r3, [r5, #12]
	subs r1, r1, r0
	ldr r3, [r6, #16]
	lsls r2, r1, #2
	adds r2, r2, r1
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r5, #16]
.L_0200239c_1:
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_020023ec
	.thumb_func
Func_020023ec:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #100
	ldrh r3, [r2]
	adds r3, #1
	ldr r6, [r5, #104]
	strh r3, [r2]
	lsls r3, r3, #16
	asrs r0, r3, #16
	cmp r0, #31
	ble .L_020023ec_0
	adds r0, r5, #0
	bl 0x0200a62c
	b .L_020023ec_1
.L_020023ec_0:
	lsls r0, r0, #10
	bl 0x0200a60c
	negs r3, r0
	str r0, [r5, #24]
	str r3, [r5, #28]
	ldr r3, [r6, #8]
	movs r1, #128
	str r3, [r5, #8]
	ldr r3, [r5, #12]
	lsls r1, r1, #9
	adds r3, r3, r1
	str r3, [r5, #12]
	subs r1, r1, r0
	ldr r3, [r6, #16]
	lsls r2, r1, #2
	adds r2, r2, r1
	subs r3, r3, r2
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r5, #16]
.L_020023ec_1:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02002440
	.thumb_func
Func_02002440:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #80]
	ldr r3, [r3]
	adds r6, r0, #0
	movs r0, #131
	sub sp, #8
	mov r11, r3
	bl 0x0200a84c
	movs r1, #63
	movs r7, #0
	mov r10, sp
	mov r9, r1
.L_02002440_2:
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	ldr r1, [r6, #8]
	movs r0, #26
	bl 0x0200a624
	lsls r3, r7, #2
	mov r2, r10
	str r0, [r3, r2]
	cmp r0, #0
	beq .L_02002440_0
	ldr r3, [r6, #20]
	str r3, [r0, #20]
	adds r3, r0, #0
	ldr r5, [r0, #80]
	adds r3, #85
	movs r2, #0
	ldr r1, [pc, #16]
	strb r2, [r3]
	adds r3, #15
	strh r2, [r3]
	mov r8, r1
	str r6, [r0, #104]
	cmp r5, #0
	beq .L_02002440_0
	b .L_02002440_1
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x03001f30
.L_02002440_1:
	movs r1, #0
	adds r0, r5, #0
	bl 0x0200a61c
	adds r3, r5, #0
	adds r3, #38
	mov r2, r8
	strb r2, [r3]
	ldrb r0, [r5, #28]
	bl 0x0200a614
	mov r3, r11
	adds r3, #70
	ldrh r3, [r3]
	strb r3, [r5, #28]
	ldrb r3, [r5, #29]
	movs r2, #1
	orrs r3, r2
	strb r3, [r5, #29]
	ldrb r3, [r5, #28]
	ldr r2, [pc, #64]
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r1, [r3, #2]
	ldr r2, [pc, #52]
	ldrh r3, [r5, #8]
	lsls r1, r1, #17
	lsrs r1, r1, #22
	ands r3, r2
	orrs r3, r1
	movs r1, #33
	negs r1, r1
	strh r3, [r5, #8]
	ldrb r3, [r5, #5]
	adds r2, r1, #0
	ands r3, r2
	mov r2, r9
	ands r3, r2
	movs r2, #64
	orrs r3, r2
	ldrb r2, [r5, #7]
	strb r3, [r5, #5]
	mov r3, r9
	ands r3, r2
	movs r2, #128
	orrs r3, r2
	strb r3, [r5, #7]
	ldr r3, [r5, #40]
	mov r1, r8
	strb r1, [r3, #22]
	b .L_02002440_0
	.2byte 0x0000
	.4byte 0xfffffc00
	.4byte 0x03001b10
.L_02002440_0:
	adds r7, #1
	cmp r7, #1
	ble .L_02002440_2
	ldr r2, [sp, #0]
	ldr r3, [pc, #60]
	ldr r0, [r2, #80]
	str r3, [r2, #108]
	movs r2, #13
	ldrb r1, [r0, #9]
	negs r2, r2
	adds r3, r2, #0
	movs r4, #8
	ands r3, r1
	orrs r3, r4
	strb r3, [r0, #9]
	mov r3, r10
	ldr r1, [r3, #4]
	ldr r0, [r1, #80]
	ldrb r3, [r0, #9]
	ands r2, r3
	ldr r3, [pc, #32]
	orrs r2, r4
	str r3, [r1, #108]
	adds r1, #35
	movs r3, #2
	strb r2, [r0, #9]
	strb r3, [r1]
	sub sp, #-8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0200a3ed
	.4byte 0x0200a39d
	.global Func_02002564
	.thumb_func
Func_02002564:
	push {lr}
	movs r0, #140
	movs r1, #0
	bl 0x0200a82c
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02002574
	.thumb_func
Func_02002574:
	push {lr}
	bl 0x0200a834
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02002580
	.thumb_func
Func_02002580:
	push {lr}
	movs r0, #15
	bl 0x0200a6c4
	bl 0x0200a334
	pop {r0}
	bx r0
	.global Func_02002590
	.thumb_func
Func_02002590:
	push {lr}
	movs r0, #17
	bl 0x0200a6c4
	bl 0x0200a334
	pop {r0}
	bx r0
	.global Func_020025a0
	.thumb_func
Func_020025a0:
	push {lr}
	movs r0, #20
	bl 0x0200a6c4
	bl 0x0200a370
	pop {r0}
	bx r0
	.global Func_020025b0
	.thumb_func
Func_020025b0:
	push {lr}
	movs r0, #176
	lsls r0, r0, #1
	bl 0x0200a67c
	ldr r0, [pc, #28]
	bl 0x0200a67c
	ldr r0, [pc, #24]
	bl 0x0200a67c
	ldr r0, [pc, #24]
	bl 0x0200a67c
	movs r0, #40
	bl 0x0200a7cc
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000016d
	.4byte 0x00000844
	.4byte 0x00000845
	.include "games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_IE/IMPORT.INC"
	.4byte 0x01000000
	.4byte 0x02020101
	.4byte 0x03030302
	.4byte 0x05040404
	.4byte 0x06060505
	.4byte 0x01000006
	.4byte 0x03020201
	.4byte 0x05040403
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00d60000
	.4byte 0x00000000
	.4byte 0x02d10000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000001e
	.4byte 0x00000099
	.4byte 0x00000015
	.4byte 0x0000001a
	.4byte 0x00000003
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00080000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00d60000
	.4byte 0x00000000
	.4byte 0x02fb0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00020000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00020000
	.4byte 0x0000001e
	.4byte 0x00000099
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x000c0000
	.4byte 0x00000003
	.4byte 0x00720000
	.4byte 0x00900000
	.4byte 0x01680000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00020000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00020000
	.4byte 0x0000001e
	.4byte 0x00000099
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x000c0000
	.4byte 0x00000003
	.4byte 0x00720000
	.4byte 0x00900000
	.4byte 0x01570000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00020000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00020000
	.4byte 0x0000001e
	.4byte 0x00000098
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00040000
	.4byte 0x00000003
	.4byte 0x00520000
	.4byte 0x00900000
	.4byte 0x01570000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00020000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00020000
	.4byte 0x0000001e
	.4byte 0x00000098
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00040000
	.4byte 0x00000003
	.4byte 0x00520000
	.4byte 0x00900000
	.4byte 0x01680000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x0000001c
	.4byte 0x00000006
	.4byte 0x0000001e
	.4byte 0x00000099
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00080000
	.4byte 0x00000003
	.4byte 0x00410000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x0000001c
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x0000001c
	.4byte 0x00000006
	.4byte 0x0000001e
	.4byte 0x00000099
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00060000
	.4byte 0x00000003
	.4byte 0x001f0000
	.4byte 0x00000000
	.4byte 0x01520000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x0000001c
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x0000001c
	.4byte 0x00000006
	.4byte 0x0000001e
	.4byte 0x00000099
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00040000
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01520000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x0000001c
	.4byte 0x00000006
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00060000
	.4byte 0x00000003
	.4byte 0x003f0000
	.4byte 0x00000000
	.4byte 0x01760000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x0000001c
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x0000001c
	.4byte 0x00000006
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00070000
	.4byte 0x00000003
	.4byte 0x00110000
	.4byte 0x00000000
	.4byte 0x01520000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x0000001c
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x0000001c
	.4byte 0x00000006
	.4byte 0x0000001e
	.4byte 0x00000099
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00040000
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01520000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x027c0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00ab0000
	.4byte 0x00000000
	.4byte 0x02750000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00cf0000
	.4byte 0x00000000
	.4byte 0x02730000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00eb0000
	.4byte 0x00000000
	.4byte 0x02710000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01050000
	.4byte 0x00000000
	.4byte 0x023e0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01470000
	.4byte 0x00000000
	.4byte 0x02230000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x010f0000
	.4byte 0x00000000
	.4byte 0x023b0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00bb0000
	.4byte 0x00000000
	.4byte 0x02410000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x009a0000
	.4byte 0x00000000
	.4byte 0x02440000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x007f0000
	.4byte 0x00000000
	.4byte 0x021f0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x008b0000
	.4byte 0x00000000
	.4byte 0x01f20000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00890000
	.4byte 0x00000000
	.4byte 0x027f0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x007a0000
	.4byte 0x00000000
	.4byte 0x02b40000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x008d0000
	.4byte 0x00000000
	.4byte 0x02ea0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00013333
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00009999
	.4byte 0x00000003
	.4byte 0x00940000
	.4byte 0x00000000
	.4byte 0x03020000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00890000
	.4byte 0x00000000
	.4byte 0x027f0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x007a0000
	.4byte 0x00000000
	.4byte 0x02b40000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x008d0000
	.4byte 0x00000000
	.4byte 0x02ea0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00013333
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00009999
	.4byte 0x00000003
	.4byte 0x00a40000
	.4byte 0x00000000
	.4byte 0x02f60000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x03560000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00d70000
	.4byte 0x00000000
	.4byte 0x03990000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00000ccc
	.4byte 0x00000015
	.4byte 0x0000001e
	.4byte 0x00000003
	.4byte 0x80010000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000009
	.4byte 0x00040000
	.4byte 0x00080000
	.4byte 0x00100000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000009
	.4byte 0x00040000
	.4byte 0x00080000
	.4byte 0x00100000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000009
	.4byte 0x00040000
	.4byte 0x00080000
	.4byte 0x00100000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00940000
	.4byte 0x00000000
	.4byte 0x02f60000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x02c50000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00810000
	.4byte 0x00000000
	.4byte 0x02870000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00c40000
	.4byte 0x00000000
	.4byte 0x03060000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00c40000
	.4byte 0x00000000
	.4byte 0x03160000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0xffff0000
	.4byte 0x000000f8
	.4byte 0x40000127
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000f8
	.4byte 0x40000127
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000160
	.4byte 0x40000134
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x000000e6
	.4byte 0x400001a7
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x00000176
	.4byte 0x400001b3
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x000000c8
	.4byte 0x40000232
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x00000066
	.4byte 0x40000273
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0007
	.4byte 0x00000166
	.4byte 0x400002a6
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x0000001b
	.4byte 0x00000152
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x000000d9
	.4byte 0xc000034b
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x000000ba
	.4byte 0x400001da
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000c
	.4byte 0x000000b4
	.4byte 0xc000026a
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000f
	.4byte 0x000000b8
	.4byte 0xc00001d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0013
	.4byte 0x0000001b
	.4byte 0x00000152
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00108007
	.4byte 0x00203008
	.4byte 0x00303007
	.4byte 0x00407007
	.4byte 0x00504007
	.4byte 0x00601008
	.4byte 0x00704008
	.4byte 0x00805004
	.4byte 0x00901000
	.4byte 0x00a01002
	.4byte 0x00b0501c
	.4byte 0x03214008
	.4byte 0x0280101e
	.4byte 0x000001ff
	.4byte 0x00000005
	.4byte 0x00108007
	.4byte 0x00203008
	.4byte 0x00303007
	.4byte 0x00407007
	.4byte 0x00504007
	.4byte 0x00601008
	.4byte 0x00704008
	.4byte 0x00805003
	.4byte 0x00901000
	.4byte 0x000001ff
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00ba0000
	.4byte 0x00000000
	.4byte 0x01cc0000
	.4byte 0x00024000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00fc0000
	.4byte 0x00000000
	.4byte 0x01e00000
	.4byte 0x00024000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02300000
	.4byte 0x00024000
	.4byte 0xffff0067
	.4byte 0x00000003
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x0000c000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x0001c000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x0000b000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00003000
	.4byte 0xffff006b
	.4byte 0x00000002
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00004000
	.4byte 0xffff0066
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00005000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x00012000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0xffff0075
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00036000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff0065
	.4byte 0x00000001
	.4byte 0x00f00000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x0000d000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00003000
	.4byte 0xffff001e
	.4byte 0x00000001
	.4byte 0x06480000
	.4byte 0x00000000
	.4byte 0x06480000
	.4byte 0x00004000
	.4byte 0xffff0067
	.4byte 0x00000002
	.4byte 0x01720000
	.4byte 0x00000000
	.4byte 0x013f0000
	.4byte 0x00008000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0030
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000e000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00ba0000
	.4byte 0x00000000
	.4byte 0x01cc0000
	.4byte 0x00024000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00fc0000
	.4byte 0x00000000
	.4byte 0x01e00000
	.4byte 0x00024000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02300000
	.4byte 0x00024000
	.4byte 0xffff0067
	.4byte 0x00000003
	.4byte 0x00f90000
	.4byte 0x00000000
	.4byte 0x02740000
	.4byte 0x00000000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00d60000
	.4byte 0x00000000
	.4byte 0x027b0000
	.4byte 0x00004000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x01260000
	.4byte 0x00000000
	.4byte 0x029e0000
	.4byte 0x00024000
	.4byte 0xffff006a
	.4byte 0x00000003
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x02a60000
	.4byte 0x00004000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x00570000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00004000
	.4byte 0xffff0066
	.4byte 0x00000001
	.4byte 0x00ad0000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00004000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00d50000
	.4byte 0x00000000
	.4byte 0x03030000
	.4byte 0x00010000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00cc0000
	.4byte 0x00000000
	.4byte 0x030f0000
	.4byte 0x0001f000
	.4byte 0xffff0075
	.4byte 0x00000001
	.4byte 0x00fa0000
	.4byte 0x00000000
	.4byte 0x03050000
	.4byte 0x00036000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x02d90000
	.4byte 0x00004000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x02d90000
	.4byte 0x00004000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00d70000
	.4byte 0x00000000
	.4byte 0x02c50000
	.4byte 0x0000c000
	.4byte 0xffff0020
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0021
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0013
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0030
	.4byte 0x00000001
	.4byte 0x00ea0000
	.4byte 0x00000000
	.4byte 0x03120000
	.4byte 0x0000e000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00ba0000
	.4byte 0x00000000
	.4byte 0x01cc0000
	.4byte 0x00024000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00fc0000
	.4byte 0x00000000
	.4byte 0x01e00000
	.4byte 0x00024000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02300000
	.4byte 0x00024000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00e10000
	.4byte 0x00000000
	.4byte 0x032e0000
	.4byte 0x0000d000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x025f0000
	.4byte 0x00003000
	.4byte 0xffff0066
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x025f0000
	.4byte 0x00005000
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x00c70000
	.4byte 0x00000000
	.4byte 0x030d0000
	.4byte 0x00003000
	.4byte 0xffff007a
	.4byte 0x00000001
	.4byte 0x00b40000
	.4byte 0x00000000
	.4byte 0x03160000
	.4byte 0x00003000
	.4byte 0xffff0075
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x030a0000
	.4byte 0x00005000
	.4byte 0xffff002a
	.4byte 0x00000001
	.4byte 0x01060000
	.4byte 0x00000000
	.4byte 0x031f0000
	.4byte 0x00007000
	.4byte 0xffff0025
	.4byte 0x00000001
	.4byte 0x010e0000
	.4byte 0x00000000
	.4byte 0x032e0000
	.4byte 0x00007000
	.4byte 0xffff0026
	.4byte 0x00000001
	.4byte 0x00fc0000
	.4byte 0x00000000
	.4byte 0x03130000
	.4byte 0x00005000
	.4byte 0xffff0036
	.4byte 0x00000001
	.4byte 0x00ea0000
	.4byte 0x00000000
	.4byte 0x030d0000
	.4byte 0x00005000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x010c0000
	.4byte 0x00000000
	.4byte 0x02e60000
	.4byte 0x00005000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x011e0000
	.4byte 0x00000000
	.4byte 0x02f90000
	.4byte 0x00007000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x010f0000
	.4byte 0x00000000
	.4byte 0x030a0000
	.4byte 0x00007000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00f60000
	.4byte 0x00000000
	.4byte 0x02fa0000
	.4byte 0x00005000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00e30000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00005000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00d10000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00003000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01200000
	.4byte 0x00000000
	.4byte 0x031b0000
	.4byte 0x00007000
	.4byte 0xffff0065
	.4byte 0x00000001
	.4byte 0x00a10000
	.4byte 0x00000000
	.4byte 0x03160000
	.4byte 0x00001000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00ab0000
	.4byte 0x00000000
	.4byte 0x032f0000
	.4byte 0x00001000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x00bf0000
	.4byte 0x00000000
	.4byte 0x02f60000
	.4byte 0x00003000
	.4byte 0xffff0068
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000d000
	.4byte 0xffff0088
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00009000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00ba0000
	.4byte 0x00000000
	.4byte 0x01cc0000
	.4byte 0x00024000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00fc0000
	.4byte 0x00000000
	.4byte 0x01e00000
	.4byte 0x00024000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02300000
	.4byte 0x00024000
	.4byte 0xffff006c
	.4byte 0x00000002
	.4byte 0x00d90000
	.4byte 0x00000000
	.4byte 0x03240000
	.4byte 0x00008000
	.4byte 0xffff006a
	.4byte 0x00000002
	.4byte 0x006b0000
	.4byte 0x00000000
	.4byte 0x02fd0000
	.4byte 0x00000000
	.4byte 0xffff0065
	.4byte 0x00000001
	.4byte 0x010b0000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x0000b000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x02a90000
	.4byte 0x00003000
	.4byte 0xffff0067
	.4byte 0x00000003
	.4byte 0x01870000
	.4byte 0x00000000
	.4byte 0x02cc0000
	.4byte 0x0000c000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x02690000
	.4byte 0x0003d000
	.4byte 0xffff006b
	.4byte 0x00000002
	.4byte 0x00a60000
	.4byte 0x00000000
	.4byte 0x025b0000
	.4byte 0x00004000
	.4byte 0xffff0066
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00005000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x013b0000
	.4byte 0x00000000
	.4byte 0x03370000
	.4byte 0x00005000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x011f0000
	.4byte 0x00000000
	.4byte 0x023b0000
	.4byte 0x0000c000
	.4byte 0xffff0067
	.4byte 0x00000002
	.4byte 0x01720000
	.4byte 0x00000000
	.4byte 0x013f0000
	.4byte 0x00028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00000f3a
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00000f3b
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x020081c5
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x020081f1
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00000f42
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00000f43
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x0200821d
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00000f47
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x02008249
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00000f4c
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x00000f7b
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008751
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x020087ed
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x0200881d
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x0200884d
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x0200887d
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x020088ad
	.4byte 0x00000002
	.4byte 0xffff0008
	.4byte 0x020088f1
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00004e15
	.4byte 0x090b0008
	.4byte 0x02008911
	.4byte 0x00004e15
	.4byte 0x090c0009
	.4byte 0x02008921
	.4byte 0x00004e15
	.4byte 0x090d000a
	.4byte 0x02008931
	.4byte 0x00000003
	.4byte 0xffff0064
	.4byte 0x0200810d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x020083b5
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00000eab
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00000eac
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00000ead
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x020084d5
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x020085e9
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00000ec5
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008751
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x020087ed
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x0200881d
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x0200884d
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x0200887d
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x020088ad
	.4byte 0x00000002
	.4byte 0xffff0009
	.4byte 0x020088dd
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte 0x02008bbd
	.4byte 0x00000003
	.4byte 0xffff0064
	.4byte 0x0200810d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000118d
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0000118e
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x0000118f
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00001190
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x020083b5
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001194
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001195
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001196
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x02008249
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x0000119c
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x02008445
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000011ce
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000011cf
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x000011d0
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x000011d1
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000011d2
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000011d3
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000011d4
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000011d5
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x000011d6
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000011d7
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x000011f4
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008751
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x020087ed
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x0200881d
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x0200884d
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x0200887d
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x020088ad
	.4byte 0x00000002
	.4byte 0xffff0008
	.4byte 0x020088f1
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00004e15
	.4byte 0x090b0008
	.4byte 0x02008911
	.4byte 0x00004e15
	.4byte 0x090c0009
	.4byte 0x02008921
	.4byte 0x00004e15
	.4byte 0x090d000a
	.4byte 0x02008931
	.4byte 0x00000003
	.4byte 0xffff0064
	.4byte 0x0200810d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x02008635
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001be5
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001be6
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001be7
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x020083b5
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001bed
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001bee
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001bef
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001bf0
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001bf1
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x02008675
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001bf2
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001bf3
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001bf4
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001bf5
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001bf6
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001bf7
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001bf8
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001bf9
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001bfa
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001bfb
	.4byte 0x00008d15
	.4byte 0x03060415
	.4byte 0x02008675
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001ca5
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008751
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008781
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x020087ed
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x0200881d
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x0200884d
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x0200887d
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x020088ad
	.4byte 0x00000002
	.4byte 0xffff0008
	.4byte 0x020088f1
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00004e15
	.4byte 0x090b0008
	.4byte 0x02008911
	.4byte 0x00004e15
	.4byte 0x090c0009
	.4byte 0x02008921
	.4byte 0x00004e15
	.4byte 0x090d000a
	.4byte 0x02008931
	.4byte 0x00000003
	.4byte 0xffff0064
	.4byte 0x0200810d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00390000
	.4byte 0x00020002
	.4byte 0x00000001
	.4byte 0x0002003b
	.4byte 0x00010002
	.4byte 0x003b0002
	.4byte 0x00020002
	.4byte 0xffff0001
