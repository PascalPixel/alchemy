.syntax unified
	.thumb
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
	bl 0x02008a5c
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
	.4byte 0x02008c08
	.global Func_02000090
	.thumb_func
Func_02000090:
	movs r0, #0
	bx lr
	.global Func_02000094
	.thumb_func
Func_02000094:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02008c50
	.global Func_0200009c
	.thumb_func
Func_0200009c:
	push {lr}
	ldr r3, [pc, #64]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_0200009c_0
	ldr r0, [pc, #52]
	bl 0x02008ab4
	cmp r0, #0
	beq .L_0200009c_1
	ldr r3, [pc, #48]
	movs r2, #1
	adds r3, #118
	strb r2, [r3]
.L_0200009c_1:
	ldr r0, [pc, #44]
	bl 0x02008ab4
	cmp r0, #0
	beq .L_0200009c_2
	ldr r3, [pc, #28]
	movs r2, #0
	adds r3, #70
	strb r2, [r3]
.L_0200009c_2:
	ldr r0, [pc, #20]
	b .L_0200009c_3
.L_0200009c_0:
	ldr r0, [pc, #24]
.L_0200009c_3:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000022
	.4byte 0x0000084f
	.4byte 0x02008c7c
	.4byte 0x00000845
	.4byte 0x02008c64
	.global Func_020000f8
	.thumb_func
Func_020000f8:
	push {lr}
	bl 0x02008ad4
	movs r2, #0
	movs r1, #0
	movs r0, #13
	bl 0x02008b24
	ldr r0, [pc, #28]
	bl 0x02008abc
	movs r0, #181
	movs r1, #3
	bl 0x02008bc4
	movs r1, #0
	movs r0, #181
	bl 0x02008ae4
	bl 0x02008adc
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000fd2
	.global Func_0200012c
	.thumb_func
Func_0200012c:
	push {lr}
	bl 0x02008ad4
	ldr r0, [pc, #24]
	movs r1, #1
	bl 0x02008aa4
	ldr r0, [pc, #20]
	movs r1, #1
	bl 0x02008aa4
	bl 0x02008adc
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000947
	.4byte 0x000029de
	.global Func_02000154
	.thumb_func
Func_02000154:
	push {lr}
	ldr r3, [pc, #28]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_02000154_0
	ldr r0, [pc, #16]
	b .L_02000154_1
.L_02000154_0:
	ldr r0, [pc, #16]
.L_02000154_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000022
	.4byte 0x02008d30
	.4byte 0x02008d24
	.global Func_02000184
	.thumb_func
Func_02000184:
	push {lr}
	bl 0x02008ad4
	ldr r0, [pc, #20]
	bl 0x02008b54
	movs r1, #0
	movs r0, #9
	bl 0x02008b74
	bl 0x02008adc
	pop {r0}
	bx r0
	.4byte 0x000013c0
	.global Func_020001a4
	.thumb_func
Func_020001a4:
	push {lr}
	bl 0x02008ad4
	ldr r0, [pc, #20]
	bl 0x02008b54
	movs r1, #0
	movs r0, #10
	bl 0x02008b74
	bl 0x02008adc
	pop {r0}
	bx r0
	.4byte 0x000013c3
	.global Func_020001c4
	.thumb_func
Func_020001c4:
	push {lr}
	bl 0x02008ad4
	ldr r0, [pc, #20]
	bl 0x02008b54
	movs r1, #0
	movs r0, #11
	bl 0x02008b74
	bl 0x02008adc
	pop {r0}
	bx r0
	.4byte 0x00001751
	.global Func_020001e4
	.thumb_func
Func_020001e4:
	push {lr}
	bl 0x02008ad4
	ldr r0, [pc, #568]
	bl 0x02008ab4
	cmp r0, #0
	beq .L_020001e4_0
	movs r0, #193
	lsls r0, r0, #2
	bl 0x02008ab4
	cmp r0, #0
	beq .L_020001e4_1
	ldr r0, [pc, #548]
	bl 0x02008ab4
	cmp r0, #0
	bne .L_020001e4_2
	ldr r0, [pc, #544]
	bl 0x02008b54
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl 0x02008b6c
	movs r0, #12
	ldr r1, [pc, #528]
	movs r2, #40
	bl 0x02008b84
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl 0x02008b6c
	movs r0, #12
	movs r1, #2
	bl 0x02008b44
	ldr r0, [pc, #496]
	bl 0x02008abc
.L_020001e4_2:
	ldr r0, [pc, #500]
	bl 0x02008b54
	movs r0, #12
	movs r1, #0
	bl 0x02008b64
	b .L_020001e4_3
.L_020001e4_1:
	ldr r0, [pc, #488]
	bl 0x02008b54
	movs r0, #12
	movs r1, #0
	bl 0x02008b64
	b .L_020001e4_4
.L_020001e4_0:
	ldr r0, [pc, #476]
	bl 0x02008b54
	movs r1, #0
	movs r0, #12
	bl 0x02008b5c
	movs r0, #0
	movs r1, #0
	bl 0x02008aec
	cmp r0, #0
	beq .L_020001e4_5
	b .L_020001e4_6
.L_020001e4_5:
	ldr r3, [pc, #452]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #12
	movs r2, #10
	movs r1, #0
	bl 0x02008b6c
	movs r0, #0
	bl 0x02008af4
	ldr r2, [pc, #424]
	ldr r3, [r0, #16]
	cmp r3, r2
	bgt .L_020001e4_7
	movs r0, #12
	ldr r1, [pc, #420]
	ldr r2, [pc, #420]
	bl 0x02008afc
	movs r1, #173
	movs r2, #137
	movs r0, #0
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x02008b0c
	movs r1, #164
	movs r2, #141
	movs r0, #0
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x02008b0c
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02008b7c
.L_020001e4_7:
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #5
	movs r2, #0
	bl 0x02008b7c
	movs r1, #224
	movs r0, #12
	lsls r1, r1, #7
	movs r2, #20
	bl 0x02008b7c
	movs r1, #129
	movs r2, #20
	movs r0, #11
	lsls r1, r1, #1
	bl 0x02008b84
	movs r0, #11
	movs r1, #1
	bl 0x02008b3c
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl 0x02008b6c
	movs r1, #132
	movs r2, #60
	movs r0, #12
	lsls r1, r1, #1
	bl 0x02008b84
	movs r0, #12
	movs r1, #1
	bl 0x02008b3c
	movs r2, #20
	movs r0, #12
	movs r1, #0
	bl 0x02008b6c
	movs r0, #11
	movs r1, #3
	bl 0x02008b2c
	movs r0, #12
	movs r1, #3
	bl 0x02008b34
	movs r1, #192
	movs r0, #11
	lsls r1, r1, #6
	movs r2, #0
	bl 0x02008b7c
	movs r1, #160
	movs r2, #10
	movs r0, #12
	lsls r1, r1, #7
	bl 0x02008b7c
	movs r0, #11
	movs r1, #1
	bl 0x02008b3c
	movs r0, #11
	movs r1, #0
	movs r2, #20
	bl 0x02008b6c
	movs r1, #240
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02008b7c
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #12
	bl 0x02008afc
	movs r0, #12
	bl 0x02008af4
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #173
	strb r3, [r0]
	lsls r1, r1, #1
	ldr r2, [pc, #160]
	movs r0, #12
	bl 0x02008b0c
	movs r0, #1
	bl 0x02008acc
	movs r0, #12
	bl 0x02008af4
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	ldr r1, [pc, #160]
	movs r0, #11
	ldr r2, [pc, #160]
	bl 0x02008afc
	movs r1, #164
	movs r0, #11
	lsls r1, r1, #1
	ldr r2, [pc, #112]
	bl 0x02008b0c
	movs r1, #164
	movs r0, #11
	lsls r1, r1, #1
	movs r2, #252
	bl 0x02008b0c
	movs r1, #192
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #10
	bl 0x02008b7c
	bl 0x020088e8
	movs r1, #164
	movs r0, #11
	lsls r1, r1, #1
	movs r2, #246
	bl 0x02008b0c
	movs r1, #0
	movs r2, #0
	movs r0, #11
	bl 0x02008b24
	ldr r0, [pc, #44]
	bl 0x02008abc
.L_020001e4_4:
	movs r1, #128
	ldr r2, [pc, #88]
	movs r0, #12
	lsls r1, r1, #9
	bl 0x02008b4c
	b .L_020001e4_3
.L_020001e4_6:
	movs r0, #12
	movs r1, #0
	bl 0x02008b64
	movs r1, #192
	movs r0, #12
	lsls r1, r1, #6
	movs r2, #10
	bl 0x02008b7c
.L_020001e4_3:
	bl 0x02008adc
	pop {r0}
	bx r0
	.4byte 0x0000084a
	.4byte 0x00000201
	.4byte 0x00001414
	.4byte 0x00000107
	.4byte 0x00001416
	.4byte 0x00001413
	.4byte 0x0000140d
	.4byte 0x03001ebc
	.4byte 0x010dffff
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x00009999
	.4byte 0x00004ccc
	.4byte 0x02008bf4
	.global Func_0200045c
	.thumb_func
Func_0200045c:
	push {lr}
	bl 0x02008ad4
	movs r0, #128
	lsls r0, r0, #2
	bl 0x02008ab4
	cmp r0, #0
	bne .L_0200045c_0
	bl 0x020088e8
.L_0200045c_0:
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x02008afc
	ldr r3, [pc, #52]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #192
	str r2, [r3]
	movs r0, #0
	movs r1, #2
	bl 0x02008b2c
	movs r2, #16
	movs r1, #2
	negs r2, r2
	movs r0, #0
	bl 0x02008b14
	movs r0, #16
	bl 0x02008acc
	movs r0, #2
	bl 0x02008ba4
	bl 0x02008adc
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.global Func_020004bc
	.thumb_func
Func_020004bc:
	push {r5, lr}
	ldr r3, [pc, #64]
	movs r5, #224
	ldr r2, [r3]
	movs r3, #128
	lsls r3, r3, #1
	lsls r5, r5, #1
	str r3, [r2, r5]
	movs r0, #8
	bl 0x02008af4
	adds r2, r0, #0
	adds r2, #35
	movs r3, #0
	strb r3, [r2]
	ldr r1, [r0, #80]
	ldrb r2, [r1, #9]
	subs r3, #13
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	ldr r3, [pc, #24]
	ldrsh r2, [r3, r5]
	ldr r3, [pc, #24]
	cmp r2, r3
	bne .L_020004bc_0
	bl 0x0200850c
.L_020004bc_0:
	movs r0, #0
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x00000022
	.global Func_0200050c
	.thumb_func
Func_0200050c:
	push {lr}
	ldr r0, [pc, #172]
	bl 0x02008ab4
	cmp r0, #0
	beq .L_0200050c_0
	movs r0, #128
	lsls r0, r0, #2
	bl 0x02008ac4
.L_0200050c_0:
	ldr r0, [pc, #156]
	bl 0x02008ab4
	cmp r0, #0
	bne .L_0200050c_1
	movs r0, #13
	bl 0x02008974
.L_0200050c_1:
	ldr r0, [pc, #144]
	bl 0x02008ab4
	cmp r0, #0
	beq .L_0200050c_2
	movs r1, #154
	movs r0, #11
	lsls r1, r1, #17
	ldr r2, [pc, #132]
	bl 0x02008b24
	movs r1, #173
	movs r0, #12
	lsls r1, r1, #17
	ldr r2, [pc, #120]
	bl 0x02008b24
	ldr r0, [pc, #120]
	bl 0x02008ab4
	cmp r0, #0
	bne .L_0200050c_2
	ldr r0, [pc, #112]
	bl 0x02008ab4
	cmp r0, #0
	bne .L_0200050c_2
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl 0x02008b24
	movs r1, #128
	ldr r2, [pc, #96]
	movs r0, #12
	lsls r1, r1, #9
	bl 0x02008b4c
.L_0200050c_2:
	ldr r0, [pc, #80]
	bl 0x02008ab4
	cmp r0, #0
	beq .L_0200050c_3
	movs r1, #224
	movs r2, #146
	movs r0, #10
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl 0x02008b24
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02008b7c
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x02008b7c
	ldr r0, [pc, #44]
	bl 0x02008ab4
	cmp r0, #0
	bne .L_0200050c_3
	bl 0x020085dc
.L_0200050c_3:
	pop {r0}
	bx r0
	.4byte 0x00000109
	.4byte 0x00000fd2
	.4byte 0x0000084a
	.4byte 0x01070000
	.4byte 0x0000084f
	.4byte 0x00000845
	.4byte 0x02008bf4
	.4byte 0x0000085e
	.global Func_020005dc
	.thumb_func
Func_020005dc:
	push {lr}
	bl 0x02008ad4
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r0, r0
	negs r1, r1
	negs r2, r2
	movs r3, #0
	bl 0x02008b9c
	movs r0, #160
	movs r1, #1
	movs r2, #160
	movs r3, #0
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl 0x02008b9c
	bl 0x02008a8c
	movs r0, #1
	bl 0x02008a4c
	movs r1, #160
	movs r2, #186
	lsls r2, r2, #17
	movs r0, #0
	lsls r1, r1, #17
	bl 0x02008b24
	bl 0x02008bac
	ldr r0, [pc, #660]
	ldr r1, [pc, #660]
	bl 0x02008b94
	movs r0, #160
	movs r1, #1
	movs r2, #145
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	bl 0x02008b9c
	movs r0, #0
	ldr r1, [pc, #640]
	ldr r2, [pc, #640]
	bl 0x02008afc
	movs r1, #160
	movs r2, #155
	movs r0, #0
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x02008b0c
	movs r1, #192
	movs r2, #10
	movs r0, #11
	lsls r1, r1, #6
	bl 0x02008b7c
	movs r0, #11
	movs r1, #2
	bl 0x02008b3c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #60
	movs r0, #11
	bl 0x02008b84
	ldr r0, [pc, #592]
	bl 0x02008b54
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl 0x02008b6c
	movs r1, #160
	movs r2, #10
	movs r0, #12
	lsls r1, r1, #7
	bl 0x02008b7c
	movs r0, #12
	movs r1, #2
	bl 0x02008b3c
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #60
	bl 0x02008b84
	movs r0, #12
	movs r1, #0
	movs r2, #20
	bl 0x02008b6c
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #5
	movs r2, #0
	bl 0x02008b7c
	movs r1, #224
	movs r0, #12
	lsls r1, r1, #7
	movs r2, #40
	bl 0x02008b7c
	movs r1, #192
	movs r0, #11
	lsls r1, r1, #6
	movs r2, #0
	bl 0x02008b7c
	movs r1, #160
	movs r2, #10
	movs r0, #12
	lsls r1, r1, #7
	bl 0x02008b7c
	movs r0, #11
	movs r1, #1
	bl 0x02008b44
	movs r2, #10
	movs r0, #11
	movs r1, #0
	bl 0x02008b6c
	movs r0, #12
	movs r1, #1
	bl 0x02008b44
	movs r1, #0
	movs r0, #12
	bl 0x02008b5c
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02008b7c
	b .L_020005dc_0
.L_020005dc_1:
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #60
	movs r0, #12
	bl 0x02008b84
	ldr r0, [pc, #432]
	bl 0x02008b54
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl 0x02008b6c
	movs r0, #12
	movs r1, #2
	bl 0x02008b3c
	movs r0, #12
	movs r1, #0
	bl 0x02008b5c
.L_020005dc_0:
	movs r0, #0
	movs r1, #0
	bl 0x02008aec
	cmp r0, #0
	bne .L_020005dc_1
	movs r0, #10
	bl 0x02008acc
	movs r1, #192
	movs r0, #11
	lsls r1, r1, #6
	movs r2, #0
	bl 0x02008b7c
	movs r1, #160
	movs r2, #20
	movs r0, #12
	lsls r1, r1, #7
	bl 0x02008b7c
	movs r0, #11
	movs r1, #3
	bl 0x02008b2c
	movs r1, #3
	movs r0, #12
	bl 0x02008b34
	movs r0, #20
	bl 0x02008acc
	movs r1, #1
	movs r0, #11
	bl 0x02008b44
	ldr r0, [pc, #332]
	bl 0x02008b54
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl 0x02008b6c
	movs r1, #128
	movs r2, #128
	movs r0, #11
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x02008afc
	movs r1, #157
	movs r2, #140
	movs r0, #11
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x02008b0c
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #20
	bl 0x02008b7c
	movs r2, #40
	movs r0, #11
	movs r1, #0
	bl 0x02008b6c
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x02008b8c
	movs r0, #60
	bl 0x02008acc
	ldr r0, [pc, #252]
	bl 0x02008ab4
	cmp r0, #0
	bne .L_020005dc_2
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #12
	bl 0x02008afc
	movs r0, #12
	bl 0x02008af4
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #173
	strb r3, [r0]
	ldr r2, [pc, #216]
	lsls r1, r1, #1
	movs r0, #12
	bl 0x02008b0c
	movs r0, #1
	bl 0x02008acc
	movs r0, #12
	bl 0x02008af4
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
.L_020005dc_2:
	movs r0, #11
	ldr r1, [pc, #184]
	ldr r2, [pc, #188]
	bl 0x02008afc
	movs r0, #0
	ldr r1, [pc, #176]
	ldr r2, [pc, #176]
	bl 0x02008afc
	movs r1, #164
	movs r2, #131
	movs r0, #11
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x02008b04
	movs r1, #164
	movs r2, #139
	lsls r2, r2, #1
	movs r0, #0
	lsls r1, r1, #1
	bl 0x02008b0c
	movs r1, #1
	movs r0, #11
	bl 0x02008b2c
	bl 0x020088e8
	movs r0, #40
	bl 0x02008acc
	movs r1, #164
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #242
	bl 0x02008b04
	movs r1, #164
	movs r0, #11
	lsls r1, r1, #1
	movs r2, #242
	bl 0x02008b0c
	movs r1, #0
	movs r2, #0
	movs r0, #11
	bl 0x02008b24
	movs r0, #0
	bl 0x02008b1c
	movs r1, #0
	movs r0, #0
	movs r2, #0
	bl 0x02008b24
	ldr r3, [pc, #80]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #65
	str r2, [r3]
	bl 0x02008bb4
	bl 0x02008bbc
	movs r0, #10
	bl 0x02008ba4
	bl 0x02008adc
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00003333
	.4byte 0x00000666
	.4byte 0x00009999
	.4byte 0x00004ccc
	.4byte 0x00001720
	.4byte 0x00001724
	.4byte 0x00001726
	.4byte 0x0000084a
	.4byte 0x00000107
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x03001ebc
	.global Func_020008e8
	.thumb_func
Func_020008e8:
	push {lr}
	movs r0, #188
	bl 0x02008bcc
	ldr r0, [pc, #20]
	movs r1, #52
	movs r2, #11
	bl 0x02008a94
	movs r0, #128
	lsls r0, r0, #2
	bl 0x02008abc
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02008bd4
	.global Func_0200090c
	.thumb_func
Func_0200090c:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	ldr r0, [r6, #48]
	ldr r7, [r6, #80]
	bl 0x02008a64
	lsls r5, r0, #1
	cmp r5, #0
	ble .L_0200090c_0
	negs r5, r5
.L_0200090c_0:
	ldr r0, [r6, #48]
	bl 0x02008a6c
	ldr r3, [r6, #56]
	lsls r0, r0, #1
	adds r3, r3, r0
	str r3, [r6, #8]
	ldr r0, [r6, #48]
	ldr r3, [r6, #60]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r5
	adds r0, r0, r2
	str r3, [r6, #12]
	bl 0x02008a6c
	cmp r0, #0
	bge .L_0200090c_1
	adds r0, #7
.L_0200090c_1:
	asrs r3, r0, #3
	strh r3, [r7, #30]
	bl 0x02008a54
	adds r5, r0, #0
	bl 0x02008a54
	lsls r5, r5, #9
	lsls r0, r0, #9
	ldr r3, [r6, #48]
	lsrs r0, r0, #16
	lsrs r5, r5, #16
	adds r5, r5, r0
	movs r2, #128
	adds r3, r3, r5
	lsls r2, r2, #3
	adds r3, r3, r2
	str r3, [r6, #48]
	movs r0, #0
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_02000974
	.thumb_func
Func_02000974:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	bl 0x02008af4
	adds r7, r0, #0
	ldr r6, [r7, #80]
	movs r2, #13
	ldrb r3, [r6, #9]
	negs r2, r2
	ands r2, r3
	movs r3, #4
	ldrb r1, [r6, #5]
	orrs r2, r3
	movs r3, #33
	negs r3, r3
	ands r3, r1
	strb r3, [r6, #5]
	movs r3, #15
	ands r2, r3
	strb r2, [r6, #9]
	movs r2, #0
	mov r8, r2
	adds r3, r6, #0
	adds r3, #39
	mov r2, r8
	strb r2, [r3]
	movs r1, #0
	bl 0x02008a9c
	movs r3, #92
	adds r3, r3, r7
	mov r2, r8
	strb r2, [r3]
	mov r10, r3
	adds r3, r7, #0
	adds r3, #85
	strb r2, [r3]
	ldr r0, [pc, #124]
	bl 0x02008ab4
	cmp r0, #0
	bne .L_02000974_0
	ldr r3, [r7, #12]
	movs r2, #128
	lsls r2, r2, #14
	adds r3, r3, r2
	str r3, [r7, #12]
.L_02000974_0:
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	movs r2, #1
	strb r3, [r1]
	mov r9, r2
	adds r3, r7, #0
	mov r2, r9
	adds r3, #97
	movs r1, #193
	strb r2, [r3]
	lsls r1, r1, #3
	movs r0, #17
	bl 0x02008a74
	adds r5, r0, #0
	movs r0, #181
	bl 0x02008aac
	movs r3, #128
	lsls r3, r3, #3
	adds r5, r5, r3
	adds r2, r5, #0
	movs r1, #128
	ldrb r0, [r6, #28]
	bl 0x02008a84
	movs r0, #17
	bl 0x02008a7c
	ldr r3, [r7, #8]
	str r3, [r7, #56]
	ldr r3, [r7, #12]
	mov r2, r8
	str r2, [r7, #48]
	str r3, [r7, #60]
	mov r2, r10
	mov r3, r9
	strb r3, [r2]
	ldr r3, [pc, #28]
	str r3, [r7, #108]
	adds r3, r7, #0
	adds r3, #86
	mov r2, r8
	strb r2, [r3]
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x00000109
	.4byte 0x0200890d
	.section .rodata,"a",%progbits
	.4byte 0x00190022
	.4byte 0x00030001
	.4byte 0x00230005
	.4byte 0x00010019
	.4byte 0x00050003
	.4byte 0x00190024
	.4byte 0x00030001
	.4byte 0xffff0005
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000140
	.4byte 0xc0000148
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000148
	.4byte 0x40000110
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x0010201e
	.4byte 0x00201021
	.4byte 0x00a1d021
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0087
	.4byte 0x00000001
	.4byte 0x00670000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00018000
	.4byte 0xffff0087
	.4byte 0x00000002
	.4byte 0x00870000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00004000
	.4byte 0xffff0045
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x01470000
	.4byte 0x0001c000
	.4byte 0xffff0096
	.4byte 0x00000001
	.4byte 0x01400000
	.4byte 0x00000000
	.4byte 0x01070000
	.4byte 0x00015000
	.4byte 0xffff0096
	.4byte 0x00000001
	.4byte 0x014e0000
	.4byte 0x00000000
	.4byte 0x01070000
	.4byte 0x00003000
	.4byte 0x0fd20016
	.4byte 0x00000007
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x0200845d
	.4byte 0x00000000
	.4byte 0x08450008
	.4byte 0x000013bf
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000016d0
	.4byte 0x00000000
	.4byte 0x08450009
	.4byte 0x02008185
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000016d1
	.4byte 0x00000000
	.4byte 0x0845000a
	.4byte 0x020081a5
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000016d2
	.4byte 0x00000000
	.4byte 0x084f000b
	.4byte 0x0000140c
	.4byte 0x00000000
	.4byte 0x084e000b
	.4byte 0x00001468
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x020081c5
	.4byte 0x00000000
	.4byte 0x084f000c
	.4byte 0x020081e5
	.4byte 0x00000000
	.4byte 0x084e000c
	.4byte 0x00001469
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001754
	.4byte 0x00008d15
	.4byte 0x08450008
	.4byte 0x000013d0
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000016de
	.4byte 0x00008d15
	.4byte 0x08450009
	.4byte 0x000013d1
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000016df
	.4byte 0x00008d15
	.4byte 0x0845000a
	.4byte 0x000013d2
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000016e0
	.4byte 0x00008d15
	.4byte 0x084a000b
	.4byte 0x00001417
	.4byte 0x00008d15
	.4byte 0x084e000b
	.4byte 0x0000146c
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001757
	.4byte 0x00008d15
	.4byte 0x084a000c
	.4byte 0x00001418
	.4byte 0x00008d15
	.4byte 0x084f000c
	.4byte 0x00001419
	.4byte 0x00008d15
	.4byte 0x084e000c
	.4byte 0x0000146d
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001758
	.4byte 0x00000003
	.4byte 0xffff005a
	.4byte 0x0200812d
	.4byte 0x00009415
	.4byte 0x0fd2000d
	.4byte 0x020080f9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
