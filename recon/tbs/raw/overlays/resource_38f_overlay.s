.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MURA/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	push {r5, lr}
	ldr r5, [pc, #24]
	ldr r3, [r5]
	cmp r3, #0
	beq .L_02000030_0
	movs r1, #2
	bl 0x0200a9c4
	movs r3, #0
	str r3, [r5]
.L_02000030_0:
	movs r0, #1
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x0200b698
	.global Func_02000050
	.thumb_func
Func_02000050:
	push {r5, r6, lr}
	adds r5, r0, #0
	bl 0x0200a9b4
	movs r3, #100
	adds r2, r0, #0
	muls r2, r3
	adds r6, r5, #0
	adds r6, #100
	ldrh r3, [r6]
	lsrs r2, r2, #16
	adds r3, r3, r2
	movs r2, #250
	strh r3, [r6]
	lsls r2, r2, #18
	lsls r3, r3, #16
	cmp r3, r2
	ble .L_02000050_0
	adds r0, r5, #0
	movs r1, #7
	bl 0x0200aaf4
	b .L_02000050_1
.L_02000050_0:
	adds r0, r5, #0
	movs r1, #10
	bl 0x0200aaf4
.L_02000050_1:
	movs r2, #0
	ldrsh r3, [r6, r2]
	movs r2, #150
	lsls r2, r2, #3
	cmp r3, r2
	ble .L_02000050_2
	movs r3, #0
	strh r3, [r6]
.L_02000050_2:
	movs r0, #1
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_020000a0
	.thumb_func
Func_020000a0:
	push {r5, r6, lr}
	adds r0, #72
	movs r2, #0
	movs r6, #105
	movs r5, #110
	movs r4, #2
	movs r1, #1
.L_020000a0_1:
	subs r3, r2, #6
	strh r6, [r0]
	cmp r3, #1
	bhi .L_020000a0_0
	strh r5, [r0]
.L_020000a0_0:
	adds r2, #1
	strb r4, [r0, #22]
	str r1, [r0, #4]
	adds r0, #24
	cmp r2, #8
	bls .L_020000a0_1
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020000cc
	.thumb_func
Func_020000cc:
	push {lr}
	ldr r3, [r0, #8]
	ldr r2, [r0, #36]
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r2, [r0, #44]
	ldr r3, [r0, #16]
	adds r3, r3, r2
	str r3, [r0, #16]
	ldr r3, [pc, #48]
	adds r2, r2, r3
	str r2, [r0, #44]
	ldr r3, [r0, #24]
	movs r2, #192
	lsls r2, r2, #3
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r3, [r0, #28]
	adds r3, r3, r2
	str r3, [r0, #28]
	adds r2, r0, #0
	adds r2, #100
	ldrh r3, [r2]
	subs r3, #1
	strh r3, [r2]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_020000cc_0
	bl 0x0200a9dc
.L_020000cc_0:
	movs r0, #1
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0xfffff5c3
	.global Func_02000114
	.thumb_func
Func_02000114:
	push {r5, lr}
	adds r5, r0, #0
	ldr r1, [r5, #104]
	cmp r1, #0
	beq .L_02000114_0
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
	bl 0x0200a9bc
	ldrh r3, [r5, #6]
	lsls r0, r0, #16
	lsrs r0, r0, #16
	subs r0, r0, r3
	lsls r0, r0, #16
	asrs r0, r0, #16
	cmp r0, #0
	beq .L_02000114_0
	movs r2, #128
	lsls r2, r2, #5
	cmp r0, r2
	ble .L_02000114_1
	adds r0, r2, #0
.L_02000114_1:
	ldr r2, [pc, #20]
	cmp r0, r2
	bge .L_02000114_2
	adds r0, r2, #0
.L_02000114_2:
	adds r3, r3, r0
	strh r3, [r5, #6]
.L_02000114_0:
	movs r0, #1
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0xfffff000
	.global Func_0200016c
	.thumb_func
Func_0200016c:
	push {lr}
	ldr r3, [pc, #36]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #28]
	cmp r2, r3
	bne .L_0200016c_0
	ldr r0, [pc, #24]
	b .L_0200016c_1
.L_0200016c_0:
	ldr r3, [pc, #24]
	cmp r2, r3
	bne .L_0200016c_2
	ldr r0, [pc, #24]
	b .L_0200016c_1
.L_0200016c_2:
	ldr r0, [pc, #24]
.L_0200016c_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000027
	.4byte 0x0200af80
	.4byte 0x00000026
	.4byte 0x0200afc8
	.4byte 0x0200ae60
	.global Func_020001ac
	.thumb_func
Func_020001ac:
	push {lr}
	ldr r3, [pc, #24]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #16]
	movs r0, #0
	cmp r2, r3
	bne .L_020001ac_0
	ldr r0, [pc, #12]
.L_020001ac_0:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000026
	.4byte 0x0200b010
	.global Func_020001d4
	.thumb_func
Func_020001d4:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200b040
	.global Func_020001dc
	.thumb_func
Func_020001dc:
	push {lr}
	ldr r3, [pc, #52]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #44]
	cmp r2, r3
	bne .L_020001dc_0
	ldr r0, [pc, #40]
	bl 0x0200aa1c
	cmp r0, #0
	bne .L_020001dc_1
	ldr r0, [pc, #36]
	bl 0x020080a0
.L_020001dc_1:
	ldr r0, [pc, #28]
	b .L_020001dc_2
.L_020001dc_0:
	ldr r3, [pc, #28]
	cmp r2, r3
	bne .L_020001dc_3
	ldr r0, [pc, #28]
	b .L_020001dc_2
.L_020001dc_3:
	ldr r0, [pc, #28]
.L_020001dc_2:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000024
	.4byte 0x00000845
	.4byte 0x0200b098
	.4byte 0x00000027
	.4byte 0x0200b368
	.4byte 0x0200b080
	.global Func_02000230
	.thumb_func
Func_02000230:
	push {r5, lr}
	movs r0, #0
	bl 0x0200aa54
	ldrh r5, [r0, #6]
	bl 0x0200aa34
	ldr r3, [pc, #40]
	adds r5, r5, r3
	ldr r3, [pc, #40]
	cmp r5, r3
	bhi .L_02000230_0
	movs r0, #16
	bl 0x0200abc4
	b .L_02000230_1
.L_02000230_0:
	ldr r0, [pc, #28]
	bl 0x0200ab04
	movs r0, #16
	movs r1, #0
	bl 0x0200ab1c
.L_02000230_1:
	bl 0x0200aa3c
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0xffff5fff
	.4byte 0x00003ffe
	.4byte 0x000016b3
	.global Func_02000274
	.thumb_func
Func_02000274:
	push {lr}
	movs r0, #27
	movs r1, #0
	movs r2, #1
	bl 0x0200abbc
	pop {r0}
	bx r0
	.global Func_02000284
	.thumb_func
Func_02000284:
	push {lr}
	ldr r3, [pc, #28]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_02000284_0
	ldr r0, [pc, #16]
	b 0x0200829e
.L_02000284_0:
	ldr r0, [pc, #16]
.L_0200029e:
	pop {r1}
	bx r1
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	.2byte 0x0027
	.2byte 0x0000
	.2byte 0xb590
	.2byte 0x0200
	.2byte 0xb3b0
	.2byte 0x0200
	.global Func_020002b4
	.thumb_func
Func_020002b4:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #0
	bl 0x0200aa54
	movs r3, #0
	adds r0, #85
	movs r1, #128
	movs r2, #128
	strb r3, [r0]
	lsls r2, r2, #7
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200aa64
	movs r0, #0
	movs r1, #2
	bl 0x0200aabc
	movs r2, #8
	movs r0, #0
	movs r1, #0
	negs r2, r2
	bl 0x0200aaa4
	ldr r3, [pc, #24]
	movs r2, #228
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #16
	str r2, [r3]
	adds r0, r5, #0
	bl 0x0200ab5c
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.global Func_02000304
	.thumb_func
Func_02000304:
	push {r5, r6, r7, lr}
	ldr r3, [pc, #180]
	ldr r7, [r3]
	bl 0x0200aa34
	movs r0, #158
	bl 0x0200abcc
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #5
	movs r6, #0
	movs r5, #0
	cmp r3, #4
	bhi .L_02000304_0
	ldr r2, [pc, #148]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	strh r4, [r0, #26]
	lsls r0, r0, #8
	strh r2, [r1, #26]
	lsls r0, r0, #8
	strh r0, [r2, #26]
	lsls r0, r0, #8
	strh r6, [r2, #26]
	lsls r0, r0, #8
	strh r4, [r3, #26]
	lsls r0, r0, #8
	movs r6, #71
	movs r5, #9
	b .L_02000304_0
	.2byte 0x2649
	.2byte 0x2511
	.2byte 0xe024
	.2byte 0x2650
	.2byte 0x2515
	.2byte 0xe021
	.2byte 0x2654
	.2byte 0x250c
	.2byte 0xe01e
	.2byte 0x2000
	.2byte 0xf002
	.2byte 0xfb79
	.2byte 0x2300
	.2byte 0x3055
	.2byte 0x2180
	.2byte 0x2280
	.2byte 0x7003
	.2byte 0x0209
	.2byte 0x2000
	.2byte 0x01d2
	.2byte 0xf002
	.2byte 0xfb77
	.2byte 0x2000
	.2byte 0x2100
	.2byte 0x2208
	.2byte 0xf002
	.2byte 0xfb92
	.2byte 0x4b0e
	.2byte 0x22e4
	.2byte 0x681b
	.2byte 0x0052
	.2byte 0x189b
	.2byte 0x2210
	.2byte 0x601a
	.2byte 0x2009
	.2byte 0xf002
	.2byte 0xfbe4
	.2byte 0xf002
	.2byte 0xfb52
	.2byte 0xe00d
.L_02000304_0:
	ldr r0, [pc, #40]
	adds r1, r6, #0
	adds r2, r5, #0
	bl 0x0200a9f4
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl 0x020082b4
	bl 0x0200aa3c
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x02008330
	.4byte 0x0200ae48
	.global Func_020003c8
	.thumb_func
Func_020003c8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #8
	bl 0x0200aa34
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200ab44
	movs r0, #168
	movs r2, #246
	movs r3, #1
	movs r1, #0
	lsls r2, r2, #16
	lsls r0, r0, #16
	bl 0x0200ab4c
	bl 0x0200ab54
	movs r0, #20
	bl 0x0200aa2c
	movs r1, #128
	movs r2, #40
	movs r0, #8
	lsls r1, r1, #1
	bl 0x0200ab34
	movs r1, #2
	movs r0, #8
	bl 0x0200aadc
	ldr r0, [pc, #608]
	bl 0x0200ab04
	movs r0, #8
	movs r1, #0
	bl 0x0200ab14
	ldr r0, [pc, #596]
	ldr r1, [pc, #600]
	bl 0x0200ab44
	movs r0, #168
	movs r2, #234
	movs r3, #1
	lsls r0, r0, #16
	movs r1, #0
	lsls r2, r2, #16
	bl 0x0200ab4c
	movs r0, #0
	ldr r1, [pc, #580]
	ldr r2, [pc, #580]
	bl 0x0200aa64
	movs r2, #139
	movs r0, #0
	movs r1, #174
	lsls r2, r2, #1
	bl 0x0200aa9c
	movs r1, #224
	movs r2, #20
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200ab24
	movs r1, #3
	movs r0, #0
	bl 0x0200aac4
	movs r0, #20
	bl 0x0200aa2c
	movs r0, #8
	movs r1, #3
	bl 0x0200aabc
	movs r0, #8
	movs r1, #0
	bl 0x0200ab14
	movs r1, #144
	movs r2, #20
	movs r0, #8
	lsls r1, r1, #8
	bl 0x0200ab24
	movs r1, #1
	movs r0, #8
	bl 0x0200aadc
	movs r0, #20
	bl 0x0200aa2c
	movs r0, #8
	bl 0x0200aa54
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #128
	movs r2, #128
	strb r3, [r0]
	lsls r1, r1, #10
	movs r0, #8
	lsls r2, r2, #9
	bl 0x0200aa64
	movs r0, #8
	movs r1, #2
	movs r2, #0
	bl 0x0200aacc
	movs r1, #224
	movs r2, #197
	movs r0, #8
	bl 0x0200aa94
	movs r0, #176
	bl 0x0200abcc
	movs r0, #10
	bl 0x0200aa2c
	movs r1, #234
	movs r2, #200
	movs r0, #8
	bl 0x0200aa94
	movs r0, #10
	bl 0x0200aa2c
	movs r0, #198
	bl 0x0200abcc
	movs r0, #30
	bl 0x0200aa2c
	movs r5, #5
	movs r6, #4
	movs r1, #0
	movs r2, #72
	movs r3, #9
	movs r0, #91
	str r6, [sp, #4]
	str r5, [sp, #0]
	bl 0x0200a9fc
	movs r0, #12
	bl 0x0200aa2c
	movs r3, #9
	movs r1, #4
	movs r2, #72
	movs r0, #91
	str r6, [sp, #4]
	str r5, [sp, #0]
	bl 0x0200a9fc
	movs r0, #9
	bl 0x0200aa2c
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ab24
	movs r1, #8
	movs r2, #72
	movs r3, #9
	movs r0, #91
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a9fc
	movs r0, #6
	bl 0x0200aa2c
	movs r3, #6
	str r3, [sp, #4]
	movs r1, #13
	movs r2, #72
	movs r3, #9
	movs r0, #91
	str r5, [sp, #0]
	bl 0x0200a9fc
	movs r0, #3
	bl 0x0200aa2c
	movs r0, #188
	bl 0x0200abcc
	movs r0, #128
	lsls r0, r0, #9
	movs r7, #148
	movs r6, #0
	mov r8, r0
	lsls r7, r7, #16
.L_020003c8_2:
	movs r3, #129
	movs r0, #222
	adds r1, r7, #0
	movs r2, #0
	lsls r3, r3, #17
	bl 0x0200a9d4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020003c8_0
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r1, #0
	bl 0x0200aa0c
	ldr r1, [r5, #80]
	movs r0, #13
	ldrb r3, [r1, #9]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	strb r3, [r1, #9]
	bl 0x0200a9b4
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #3
	lsrs r3, r3, #16
	adds r2, r5, #0
	adds r3, #40
	adds r2, #100
	strh r3, [r2]
	movs r3, #3
	ands r3, r6
	lsls r3, r3, #16
	add r3, r8
	asrs r2, r3, #1
	mov r3, r8
	str r3, [r5, #44]
	movs r3, #1
	ands r3, r6
	str r2, [r5, #36]
	cmp r3, #0
	beq .L_020003c8_1
	negs r3, r2
	str r3, [r5, #36]
.L_020003c8_1:
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200a9c4
	adds r0, r5, #0
	ldr r1, [pc, #180]
	bl 0x0200a9cc
.L_020003c8_0:
	movs r0, #128
	lsls r0, r0, #11
	adds r6, #1
	adds r7, r7, r0
	cmp r6, #9
	bls .L_020003c8_2
	movs r3, #5
	movs r2, #7
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #91
	movs r1, #19
	movs r2, #72
	movs r3, #9
	bl 0x0200a9fc
	movs r3, #8
	movs r2, #11
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #7
	movs r0, #23
	movs r1, #11
	movs r2, #5
	bl 0x0200aa04
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	movs r0, #0
	bl 0x0200aa14
	movs r0, #10
	bl 0x0200aa2c
	movs r1, #6
	movs r2, #0
	movs r0, #0
	bl 0x0200aacc
	movs r0, #20
	bl 0x0200aa2c
	movs r0, #1
	movs r1, #1
	negs r0, r0
	negs r1, r1
	ldr r2, [pc, #80]
	bl 0x0200aa14
	movs r1, #128
	ldr r2, [pc, #76]
	lsls r1, r1, #9
	movs r0, #8
	bl 0x0200aafc
	movs r0, #60
	bl 0x0200aa2c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #60
	movs r0, #0
	bl 0x0200ab34
	ldr r0, [pc, #52]
	bl 0x0200aa24
	bl 0x0200aa3c
	sub sp, #-8
.L_02000664:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x1786
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.2byte 0x0ccc
	.2byte 0x0000
	.2byte 0x9999
	.2byte 0x0000
	.2byte 0x4ccc
	.2byte 0x0000
	.2byte 0xae20
	.2byte 0x0200
	.2byte 0xe666
	.2byte 0x0000
	.2byte 0xae34
	.2byte 0x0200
	.2byte 0x0847
	.2byte 0x0000
	.global Func_02000694
	.thumb_func
Func_02000694:
	push {r5, lr}
	ldr r3, [pc, #328]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #320]
	sub sp, #8
	cmp r2, r3
	bne .L_02000694_0
	bl 0x0200a910
	b .L_02000694_1
.L_02000694_0:
	ldr r3, [pc, #308]
	cmp r2, r3
	bne .L_02000694_2
	ldr r3, [pc, #308]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #68
	str r2, [r3]
	b .L_02000694_1
.L_02000694_2:
	movs r0, #23
	bl 0x0200aa54
	movs r1, #0
	bl 0x0200aa0c
	movs r0, #24
	bl 0x0200aa54
	movs r1, #0
	bl 0x0200aa0c
	movs r0, #25
	bl 0x0200aa54
	movs r1, #0
	bl 0x0200aa0c
	movs r0, #26
	bl 0x0200aa54
	movs r1, #0
	bl 0x0200aa0c
	ldr r5, [pc, #248]
	movs r0, #23
	adds r1, r5, #0
	bl 0x0200aa6c
	movs r0, #24
	adds r1, r5, #0
	bl 0x0200aa6c
	movs r0, #25
	adds r1, r5, #0
	bl 0x0200aa6c
	movs r0, #26
	adds r1, r5, #0
	bl 0x0200aa6c
	ldr r0, [pc, #216]
	bl 0x0200aa1c
	cmp r0, #0
	bne .L_02000694_3
	movs r5, #8
.L_02000694_4:
	adds r0, r5, #0
	bl 0x0200aa54
	adds r5, #1
	movs r1, #0
	bl 0x0200aa0c
	cmp r5, #16
	bls .L_02000694_4
	movs r3, #13
	str r3, [sp, #0]
	movs r5, #8
	movs r0, #13
	movs r1, #9
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200aa04
	movs r3, #15
	str r3, [sp, #0]
	movs r0, #13
	movs r1, #9
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200aa04
	movs r3, #14
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #13
	movs r1, #9
	movs r2, #1
	movs r3, #1
	bl 0x0200aa04
.L_02000694_3:
	ldr r0, [pc, #132]
	bl 0x0200aa1c
	cmp r0, #0
	bne .L_02000694_5
	ldr r3, [pc, #100]
	movs r1, #225
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	bne .L_02000694_5
	bl 0x020088ec
.L_02000694_5:
	ldr r0, [pc, #104]
	bl 0x0200aa1c
	cmp r0, #0
	beq .L_02000694_1
	movs r0, #1
	bl 0x0200aa5c
	movs r0, #2
	bl 0x0200aa5c
	movs r0, #3
	bl 0x0200aa5c
	movs r0, #17
	bl 0x0200aa5c
	movs r0, #18
	bl 0x0200aa5c
	movs r0, #19
	bl 0x0200aa5c
	movs r0, #20
	bl 0x0200aa5c
	movs r0, #21
	bl 0x0200aa5c
	movs r0, #22
	bl 0x0200aa5c
	ldr r0, [pc, #44]
	bl 0x0200aa44
.L_02000694_1:
	movs r0, #0
	sub sp, #-8
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000027
	.4byte 0x00000026
	.4byte 0x03001ebc
	.4byte 0x0200add8
	.4byte 0x00000845
	.4byte 0x00000843
	.4byte 0x0200b2d8
	.global Func_02000800
	.thumb_func
Func_02000800:
	push {lr}
	movs r0, #19
	bl 0x0200aa54
	cmp r0, #0
	beq .L_02000800_0
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	ldr r3, [pc, #144]
	ldr r2, [r3]
	movs r3, #1
	ands r2, r3
	cmp r2, #0
	bne .L_02000800_1
	str r2, [r0, #12]
	b .L_02000800_0
.L_02000800_1:
	movs r3, #250
	lsls r3, r3, #17
	str r3, [r0, #12]
.L_02000800_0:
	movs r0, #20
	bl 0x0200aa54
	cmp r0, #0
	beq .L_02000800_2
	adds r3, r0, #0
	adds r3, #85
	movs r1, #0
	strb r1, [r3]
	ldr r3, [pc, #104]
	ldr r3, [r3]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02000800_3
	str r1, [r0, #12]
	b .L_02000800_2
.L_02000800_3:
	movs r3, #250
	lsls r3, r3, #17
	str r3, [r0, #12]
.L_02000800_2:
	movs r0, #21
	bl 0x0200aa54
	cmp r0, #0
	beq .L_02000800_4
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	ldr r3, [pc, #64]
	ldr r2, [r3]
	movs r3, #1
	ands r2, r3
	cmp r2, #0
	bne .L_02000800_5
	str r2, [r0, #12]
	b .L_02000800_4
.L_02000800_5:
	movs r3, #250
	lsls r3, r3, #17
	str r3, [r0, #12]
.L_02000800_4:
	movs r0, #22
	bl 0x0200aa54
	cmp r0, #0
	beq .L_02000800_6
	adds r3, r0, #0
	adds r3, #85
	movs r1, #0
	strb r1, [r3]
	ldr r3, [pc, #24]
	ldr r3, [r3]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02000800_7
	str r1, [r0, #12]
	b .L_02000800_6
.L_02000800_7:
	movs r3, #250
	lsls r3, r3, #17
	str r3, [r0, #12]
.L_02000800_6:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001e40
	.global Func_020008ac
	.thumb_func
Func_020008ac:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	movs r0, #141
	movs r1, #1
	bl 0x0200ab8c
	adds r0, r5, #0
	adds r1, r6, #0
	bl 0x0200ab94
	bl 0x0200abac
	movs r0, #1
	bl 0x0200ab84
	movs r0, #1
	bl 0x0200a99c
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_020008d8
	.thumb_func
Func_020008d8:
	push {lr}
	movs r0, #2
	bl 0x0200ab84
	bl 0x0200ab9c
	bl 0x0200aba4
	pop {r0}
	bx r0
	.global Func_020008ec
	.thumb_func
Func_020008ec:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	bl 0x0200aa34
	movs r0, #3
	ldr r5, [pc, #960]
	bl 0x0200aa1c
	str r0, [r5]
	movs r0, #19
	bl 0x0200aa54
	movs r1, #0
	bl 0x0200aa0c
	movs r0, #20
	bl 0x0200aa54
	movs r1, #0
	bl 0x0200aa0c
	movs r0, #21
	bl 0x0200aa54
	movs r1, #0
	bl 0x0200aa0c
	movs r0, #22
	bl 0x0200aa54
	movs r1, #0
	bl 0x0200aa0c
	movs r0, #208
	movs r1, #1
	movs r2, #128
	movs r3, #0
	lsls r0, r0, #15
	negs r1, r1
	lsls r2, r2, #17
	bl 0x0200ab4c
	bl 0x0200a9e4
	movs r1, #184
	movs r2, #247
	lsls r1, r1, #13
	lsls r2, r2, #16
	movs r0, #0
	bl 0x0200aab4
	movs r0, #1
	bl 0x0200a99c
	bl 0x0200ab74
	bl 0x0200ab7c
	movs r0, #0
	ldr r1, [pc, #856]
	ldr r2, [pc, #856]
	bl 0x0200aa64
	movs r0, #0
	movs r1, #121
	movs r2, #238
	bl 0x0200aa9c
	movs r0, #1
	ldr r1, [pc, #844]
	ldr r2, [pc, #844]
	bl 0x0200aa64
	movs r0, #2
	ldr r1, [pc, #832]
	ldr r2, [pc, #836]
	bl 0x0200aa64
	movs r0, #0
	bl 0x0200aa54
	cmp r0, #0
	beq .L_020008ec_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200aab4
.L_020008ec_0:
	movs r0, #0
	bl 0x0200aa54
	cmp r0, #0
	beq .L_020008ec_1
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #2
	bl 0x0200aab4
.L_020008ec_1:
	ldr r1, [pc, #792]
	movs r0, #1
	bl 0x0200aa6c
	ldr r1, [pc, #788]
	movs r0, #2
	bl 0x0200aa6c
	ldr r3, [r5]
	cmp r3, #0
	beq .L_020008ec_2
	movs r0, #3
	ldr r1, [pc, #760]
	ldr r2, [pc, #764]
	bl 0x0200aa64
	movs r0, #0
	bl 0x0200aa54
	cmp r0, #0
	beq .L_020008ec_3
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #3
	bl 0x0200aab4
.L_020008ec_3:
	ldr r1, [pc, #748]
	movs r0, #3
	bl 0x0200aa6c
.L_020008ec_2:
	movs r0, #2
	bl 0x0200aa74
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200ab24
	movs r1, #224
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200ab24
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #6
	movs r2, #20
	bl 0x0200ab24
	ldr r5, [pc, #672]
	ldr r3, [r5]
	cmp r3, #0
	beq .L_020008ec_4
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200ab24
.L_020008ec_4:
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #20
	bl 0x0200ab24
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #6
	movs r2, #20
	bl 0x0200ab24
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #60
	bl 0x0200ab24
	ldr r3, [r5]
	cmp r3, #0
	beq .L_020008ec_5
	movs r1, #224
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ab24
.L_020008ec_5:
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ab24
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x0200aae4
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200aae4
	ldr r3, [r5]
	cmp r3, #0
	beq .L_020008ec_6
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl 0x0200aae4
.L_020008ec_6:
	movs r0, #40
	bl 0x0200aa2c
	movs r1, #1
	movs r0, #1
	bl 0x0200aad4
	ldr r0, [pc, #568]
	bl 0x0200ab04
	movs r0, #1
	movs r1, #20
	bl 0x0200a5a8
	movs r2, #0
	movs r0, #0
	movs r1, #1
	bl 0x0200aae4
	movs r1, #3
	movs r0, #0
	bl 0x0200aac4
	movs r0, #30
	bl 0x0200aa2c
	movs r2, #143
	movs r0, #2
	movs r1, #72
	lsls r2, r2, #1
	bl 0x0200aa9c
	movs r2, #151
	movs r0, #2
	movs r1, #72
	lsls r2, r2, #1
	bl 0x0200aa9c
	movs r2, #155
	lsls r2, r2, #1
	movs r0, #2
	movs r1, #88
	bl 0x0200aa9c
	movs r0, #2
	movs r1, #1
	bl 0x0200aadc
	movs r1, #0
	movs r2, #0
	movs r0, #2
	bl 0x0200aae4
	movs r0, #20
	bl 0x0200aa2c
	movs r0, #0
	movs r1, #2
	movs r2, #0
	bl 0x0200aae4
	movs r0, #1
	movs r1, #2
	movs r2, #0
	bl 0x0200aae4
	ldr r3, [r5]
	cmp r3, #0
	beq .L_020008ec_7
	movs r0, #3
	movs r1, #2
	movs r2, #0
	bl 0x0200aae4
.L_020008ec_7:
	movs r0, #30
	bl 0x0200aa2c
	movs r0, #1
	movs r1, #3
	bl 0x0200aabc
	ldr r3, [r5]
	cmp r3, #0
	beq .L_020008ec_8
	movs r0, #3
	movs r1, #3
	bl 0x0200aabc
.L_020008ec_8:
	movs r1, #3
	movs r0, #0
	bl 0x0200aac4
	movs r0, #20
	bl 0x0200aa2c
	movs r1, #3
	movs r0, #2
	bl 0x0200aac4
	movs r0, #30
	bl 0x0200aa2c
	movs r2, #0
	movs r1, #0
	movs r0, #2
	bl 0x0200ab24
	movs r0, #20
	bl 0x0200aa2c
	movs r1, #9
	movs r0, #2
	bl 0x020088ac
	movs r0, #40
	bl 0x0200aa2c
	bl 0x020088d8
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200ab34
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #2
	bl 0x0200aa64
	movs r0, #2
	bl 0x0200aa54
	adds r0, #90
	ldrb r2, [r0]
	movs r7, #254
	adds r3, r7, #0
	ands r3, r2
	movs r2, #155
	strb r3, [r0]
	movs r1, #80
	lsls r2, r2, #1
	movs r0, #2
	bl 0x0200aa9c
	movs r0, #1
	bl 0x0200aa2c
	movs r0, #2
	bl 0x0200aa54
	adds r0, #90
	ldrb r3, [r0]
	movs r6, #1
	orrs r3, r6
	movs r1, #129
	strb r3, [r0]
	movs r2, #40
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200ab34
	movs r0, #1
	movs r1, #20
	bl 0x0200a5a8
	movs r1, #129
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200ab34
	movs r0, #2
	movs r1, #20
	bl 0x0200a5a8
	ldr r3, [r5]
	cmp r3, #0
	beq .L_020008ec_9
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl 0x0200aae4
.L_020008ec_9:
	movs r0, #2
	movs r1, #1
	movs r2, #60
	bl 0x0200aaec
	ldr r3, [r5]
	cmp r3, #0
	beq .L_020008ec_10
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ab24
.L_020008ec_10:
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ab24
	movs r1, #224
	movs r2, #0
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200ab24
	movs r0, #1
	movs r1, #1
	bl 0x0200aad4
	movs r0, #1
	movs r1, #20
	bl 0x0200a5a8
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #2
	bl 0x0200aa64
	movs r0, #2
	bl 0x0200aa54
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r7, #0
	ands r3, r2
	movs r2, #143
	strb r3, [r0]
	movs r1, #72
	lsls r2, r2, #1
	movs r0, #2
	bl 0x0200aa9c
	movs r0, #1
	bl 0x0200aa2c
	movs r0, #2
	bl 0x0200aa54
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r6
	strb r3, [r0]
	ldr r1, [pc, #72]
	movs r0, #2
	bl 0x0200aa6c
	ldr r3, [r5]
	cmp r3, #0
	beq .L_020008ec_11
	ldr r1, [pc, #68]
	movs r2, #0
	movs r0, #3
	bl 0x0200ab34
	movs r0, #60
	bl 0x0200aa2c
	movs r0, #3
	movs r1, #20
	bl 0x0200a5a8
	movs r0, #3
	movs r1, #3
	bl 0x0200aabc
	b .L_020008ec_12
	.2byte 0x0000
	.4byte 0x0200b69c
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x00009999
	.4byte 0x00004ccc
	.4byte 0x0200abd4
	.4byte 0x0200ac08
	.4byte 0x0200ac3c
	.4byte 0x00001473
	.4byte 0x00000105
.L_020008ec_11:
	ldr r3, [pc, #676]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_020008ec_12:
	movs r2, #0
	movs r0, #2
	movs r1, #0
	bl 0x0200aae4
	movs r0, #0
	movs r1, #3
	bl 0x0200aabc
	movs r0, #1
	movs r1, #3
	bl 0x0200aabc
	movs r1, #3
	movs r0, #2
	bl 0x0200aac4
	movs r0, #20
	bl 0x0200aa2c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200ab3c
	movs r0, #10
	bl 0x0200aa2c
	movs r1, #128
	lsls r1, r1, #6
	movs r0, #1
	movs r2, #10
	bl 0x0200a5c0
	movs r1, #160
	lsls r1, r1, #8
	movs r0, #0
	movs r2, #10
	bl 0x0200a5c0
	movs r1, #0
	movs r0, #1
	bl 0x0200ab0c
	movs r0, #0
	movs r1, #0
	bl 0x0200aa4c
	cmp r0, #0
	bne .L_020008ec_13
	movs r0, #20
	bl 0x0200aa2c
	movs r0, #1
	movs r1, #3
	bl 0x0200aabc
	b .L_020008ec_14
.L_020008ec_13:
	movs r0, #1
	movs r1, #4
	bl 0x0200aac4
	ldr r3, [pc, #536]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_020008ec_14:
	movs r5, #128
	lsls r5, r5, #6
	movs r0, #1
	movs r1, #40
	bl 0x0200a5a8
	adds r1, r5, #0
	movs r0, #2
	movs r2, #40
	bl 0x0200a5c0
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #2
	movs r2, #20
	bl 0x0200a5c0
	movs r2, #128
	lsls r2, r2, #7
	mov r8, r2
	movs r0, #2
	mov r1, r8
	movs r2, #40
	bl 0x0200a5c0
	ldr r1, [pc, #476]
	movs r2, #0
	movs r0, #2
	bl 0x0200ab34
	movs r6, #192
	movs r0, #60
	bl 0x0200aa2c
	lsls r6, r6, #7
	movs r0, #1
	mov r1, r8
	movs r2, #0
	bl 0x0200ab24
	movs r0, #0
	adds r1, r6, #0
	movs r2, #60
	bl 0x0200a5c0
	adds r1, r5, #0
	movs r0, #3
	movs r2, #10
	bl 0x0200a5c0
	adds r1, r5, #0
	movs r5, #160
	lsls r5, r5, #8
	movs r0, #1
	movs r2, #0
	bl 0x0200ab24
	adds r1, r5, #0
	movs r0, #0
	movs r2, #10
	bl 0x0200a5c0
	movs r0, #1
	ldr r1, [pc, #400]
	movs r2, #0
	bl 0x0200ab34
	ldr r1, [pc, #392]
	movs r2, #0
	movs r0, #0
	bl 0x0200ab34
	movs r0, #40
	bl 0x0200aa2c
	movs r0, #1
	mov r1, r8
	movs r2, #0
	bl 0x0200ab24
	movs r2, #10
	movs r0, #0
	adds r1, r6, #0
	bl 0x0200a5c0
	movs r1, #2
	movs r0, #1
	bl 0x0200aadc
	ldr r0, [pc, #352]
	bl 0x0200ab04
	movs r0, #1
	movs r1, #10
	bl 0x0200a5a8
	movs r3, #192
	lsls r3, r3, #8
	mov r10, r3
	movs r2, #20
	movs r0, #2
	mov r1, r10
	bl 0x0200a5c0
	movs r0, #2
	movs r1, #3
	bl 0x0200aac4
	movs r0, #2
	movs r1, #20
	bl 0x0200a5a8
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200a5c0
	adds r1, r5, #0
	movs r0, #0
	movs r2, #40
	bl 0x0200a5c0
	movs r0, #1
	mov r1, r8
	movs r2, #20
	bl 0x0200a5c0
	movs r0, #0
	adds r1, r6, #0
	movs r2, #30
	bl 0x0200a5c0
	movs r0, #1
	adds r1, r6, #0
	movs r2, #20
	bl 0x0200a5c0
	movs r1, #224
	movs r2, #30
	lsls r1, r1, #8
	movs r0, #0
	bl 0x0200a5c0
	movs r0, #2
	movs r1, #2
	bl 0x0200aadc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #2
	bl 0x0200ab34
	movs r0, #40
	bl 0x0200aa2c
	movs r0, #1
	mov r1, r8
	movs r2, #0
	bl 0x0200ab24
	movs r0, #0
	adds r1, r6, #0
	movs r2, #20
	bl 0x0200a5c0
	movs r2, #10
	mov r1, r10
	movs r0, #2
	bl 0x0200a5c0
	movs r0, #17
	bl 0x0200abcc
	movs r0, #206
	bl 0x0200abcc
	movs r1, #0
	ldr r0, [pc, #176]
	bl 0x0200ab64
	movs r0, #1
	bl 0x0200ab6c
	movs r0, #1
	bl 0x0200a99c
	ldr r2, [pc, #164]
	movs r3, #1
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, [pc, #156]
	bl 0x0200a9a4
	movs r0, #20
	bl 0x0200a99c
	ldr r0, [pc, #148]
	movs r1, #1
	bl 0x0200ab64
	movs r0, #128
	movs r1, #2
	lsls r0, r0, #9
	bl 0x0200ab64
	movs r0, #120
	bl 0x0200ab6c
	movs r0, #60
	bl 0x0200a99c
	ldr r5, [pc, #124]
	movs r0, #0
	adds r1, r5, #0
	bl 0x0200aa6c
	movs r0, #1
	adds r1, r5, #0
	bl 0x0200aa6c
	movs r0, #2
	adds r1, r5, #0
	bl 0x0200aa6c
	adds r1, r5, #0
	movs r0, #3
	bl 0x0200aa6c
	movs r0, #100
	bl 0x0200aa2c
	movs r0, #1
	movs r1, #20
	bl 0x0200a5a8
	movs r0, #2
	movs r1, #40
	bl 0x0200a5a8
	ldr r3, [pc, #72]
	ldr r3, [r3]
	cmp r3, #0
	beq .L_020008ec_15
	movs r0, #40
	bl 0x0200aa2c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #3
	bl 0x0200ab3c
	movs r0, #40
	bl 0x0200aa2c
	movs r0, #3
	movs r1, #40
	bl 0x0200a5a8
	b .L_020008ec_16
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00000101
	.4byte 0x0000147b
	.4byte 0x00007fff
	.4byte 0x0200b6a0
	.4byte 0x0200a609
	.4byte 0x00405210
	.4byte 0x0200ac70
	.4byte 0x0200b69c
.L_020008ec_15:
	ldr r3, [pc, #568]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_020008ec_16:
	movs r0, #20
	bl 0x0200aa2c
	ldr r7, [pc, #552]
	ldr r3, [r7]
	cmp r3, #0
	beq .L_020008ec_17
	movs r0, #3
	bl 0x0200aa54
	movs r5, #128
	lsls r5, r5, #10
	str r5, [r0, #40]
	movs r0, #10
	bl 0x0200aa2c
	movs r0, #3
	adds r1, r5, #0
	adds r2, r5, #0
	bl 0x0200aa64
	movs r1, #2
	movs r2, #0
	movs r0, #3
	negs r1, r1
	bl 0x0200aaa4
	ldr r1, [pc, #508]
	movs r0, #3
	bl 0x0200aa6c
	movs r0, #3
	bl 0x0200aa54
	movs r1, #0
	bl 0x0200aa0c
	movs r0, #3
	movs r1, #19
	bl 0x0200aabc
	movs r0, #10
	bl 0x0200aa2c
.L_020008ec_17:
	movs r0, #0
	bl 0x0200aa54
	movs r5, #128
	lsls r5, r5, #10
	str r5, [r0, #40]
	movs r0, #10
	bl 0x0200aa2c
	adds r2, r5, #0
	adds r1, r5, #0
	movs r0, #0
	bl 0x0200aa64
	ldr r6, [pc, #444]
	movs r0, #0
	adds r1, r6, #0
	bl 0x0200aa6c
	movs r0, #0
	bl 0x0200aa54
	movs r1, #0
	bl 0x0200aa0c
	movs r1, #19
	movs r0, #0
	bl 0x0200aabc
	movs r0, #20
	bl 0x0200aa2c
	movs r0, #1
	bl 0x0200aa54
	str r5, [r0, #40]
	movs r0, #10
	bl 0x0200aa2c
	adds r2, r5, #0
	adds r1, r5, #0
	movs r0, #1
	bl 0x0200aa64
	adds r1, r6, #0
	movs r0, #1
	bl 0x0200aa6c
.L_02001078:
	movs r0, #1
	bl 0x0200aa54
	movs r1, #0
	bl 0x0200aa0c
	movs r1, #19
	movs r0, #1
	bl 0x0200aabc
	movs r0, #40
	bl 0x0200aa2c
	movs r0, #2
	bl 0x0200aa54
	str r5, [r0, #40]
	movs r0, #10
	bl 0x0200aa2c
	adds r1, r6, #0
	movs r0, #2
	bl 0x0200aa6c
	movs r0, #2
	bl 0x0200aa54
	movs r1, #0
	bl 0x0200aa0c
	movs r1, #19
	movs r0, #2
	bl 0x0200aabc
	ldr r3, [pc, #312]
	movs r5, #0
	str r5, [r3]
	movs r0, #160
	bl 0x0200aa2c
	ldr r0, [pc, #304]
	bl 0x0200a9ac
	movs r0, #120
	bl 0x0200aa2c
	movs r1, #1
	ldr r0, [pc, #296]
	bl 0x0200ab64
	movs r0, #60
	bl 0x0200ab6c
	movs r0, #60
	bl 0x0200a99c
	ldr r3, [pc, #280]
	ldr r2, [pc, #284]
	str r5, [r3]
	movs r3, #128
	ldr r5, [pc, #280]
	lsls r3, r3, #16
	str r3, [r2]
	movs r1, #200
	movs r3, #1
	str r3, [r5]
	lsls r1, r1, #4
	ldr r0, [pc, #272]
	bl 0x0200a9a4
	movs r0, #180
	bl 0x0200aa2c
	movs r0, #21
	bl 0x0200abcc
	movs r0, #1
	movs r1, #80
	bl 0x0200a5a8
	movs r0, #2
	movs r1, #40
	bl 0x0200a5a8
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200ab3c
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200ab3c
	movs r1, #129
	movs r0, #2
	lsls r1, r1, #1
	bl 0x0200ab3c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #3
	bl 0x0200ab3c
	movs r0, #60
	bl 0x0200aa2c
	movs r0, #2
	movs r1, #20
	bl 0x0200a5a8
	movs r3, #2
	str r3, [r5]
	movs r1, #2
	movs r0, #2
	bl 0x0200aad4
	movs r0, #20
	bl 0x0200aa2c
	movs r1, #1
	movs r0, #1
	bl 0x0200aad4
	movs r0, #40
	bl 0x0200aa2c
	movs r0, #0
	movs r1, #2
	bl 0x0200aad4
	movs r1, #1
	movs r0, #3
	bl 0x0200aad4
	movs r0, #20
	bl 0x0200aa2c
	movs r1, #3
	movs r0, #2
	bl 0x0200aad4
	movs r0, #40
	bl 0x0200aa2c
	movs r1, #1
	movs r0, #0
	bl 0x0200aad4
	movs r0, #20
	bl 0x0200aa2c
	movs r1, #2
	movs r0, #1
	bl 0x0200aad4
	movs r0, #20
	bl 0x0200aa2c
	movs r0, #3
	movs r1, #2
	bl 0x0200aad4
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200ab3c
	movs r0, #1
	movs r1, #20
	bl 0x0200a5a8
	ldr r3, [r7]
	cmp r3, #0
	beq .L_02001078_0
	movs r1, #129
	movs r0, #3
	lsls r1, r1, #1
	bl 0x0200ab3c
	movs r0, #3
	movs r1, #10
	bl 0x0200a5a8
	b .L_02001078_1
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0xb69c
	.2byte 0x0200
	.2byte 0xacfc
	.2byte 0x0200
	.4byte 0x0200b6a0
	.4byte 0x0200a609
	.4byte 0x00406218
	.4byte 0x0200b690
	.4byte 0x0200b68c
	.4byte 0x0200b694
	.4byte 0x0200a7ad
.L_02001078_0:
	ldr r3, [pc, #932]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02001078_1:
	ldr r7, [pc, #920]
	movs r3, #3
	str r3, [r7]
	movs r0, #0
	bl 0x0200aa54
	adds r0, #35
	movs r6, #254
	ldrb r2, [r0]
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #1
	bl 0x0200aa54
	adds r0, #35
	ldrb r2, [r0]
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #2
	bl 0x0200aa54
	adds r0, #35
	ldrb r2, [r0]
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #3
	bl 0x0200aa54
	adds r0, #35
	ldrb r2, [r0]
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #3
	movs r0, #0
	bl 0x0200ab2c
	movs r0, #1
	movs r1, #3
	bl 0x0200ab2c
	movs r0, #2
	movs r1, #3
	bl 0x0200ab2c
	movs r0, #3
	movs r1, #3
	bl 0x0200ab2c
	ldr r3, [pc, #820]
	movs r5, #0
	movs r1, #200
	str r5, [r3]
	lsls r1, r1, #4
	ldr r0, [pc, #816]
	bl 0x0200a9a4
	movs r0, #220
	bl 0x0200abcc
	movs r0, #19
	bl 0x0200aa54
	adds r0, #35
	ldrb r2, [r0]
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #2
	movs r0, #19
	bl 0x0200ab2c
	movs r1, #240
	movs r2, #248
	lsls r2, r2, #16
	movs r0, #19
	lsls r1, r1, #15
	bl 0x0200aab4
	ldr r5, [pc, #768]
	movs r0, #19
	adds r1, r5, #0
	bl 0x0200aa6c
	movs r0, #20
	bl 0x0200aa54
	adds r0, #35
	ldrb r2, [r0]
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #2
	movs r0, #20
	bl 0x0200ab2c
	movs r1, #200
	movs r2, #137
	movs r0, #20
	lsls r1, r1, #15
	lsls r2, r2, #17
	bl 0x0200aab4
	movs r0, #20
	adds r1, r5, #0
	bl 0x0200aa6c
	ldr r3, [pc, #716]
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02001078_2
	movs r0, #21
	bl 0x0200aa54
	adds r0, #35
	ldrb r2, [r0]
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #2
	movs r0, #21
	bl 0x0200ab2c
	movs r1, #148
	movs r2, #254
	movs r0, #21
	lsls r1, r1, #15
	lsls r2, r2, #16
	bl 0x0200aab4
	movs r0, #21
	adds r1, r5, #0
	bl 0x0200aa6c
.L_02001078_2:
	movs r0, #22
	bl 0x0200aa54
	adds r0, #35
	ldrb r2, [r0]
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #2
	movs r0, #22
	bl 0x0200ab2c
	movs r1, #188
	movs r2, #225
	movs r0, #22
	lsls r1, r1, #15
	lsls r2, r2, #16
	bl 0x0200aab4
	movs r0, #22
	adds r1, r5, #0
	bl 0x0200aa6c
	ldr r3, [r7]
	cmp r3, #0
	beq .L_02001078_3
	adds r5, r7, #0
.L_02001078_4:
	movs r0, #1
	bl 0x0200a99c
	ldr r3, [r5]
	cmp r3, #0
	bne .L_02001078_4
.L_02001078_3:
	movs r0, #150
	lsls r0, r0, #1
	bl 0x0200aa2c
	ldr r0, [pc, #592]
	bl 0x0200a9ac
	movs r0, #120
	bl 0x0200aa2c
	movs r0, #17
	bl 0x0200abcc
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl 0x0200ab64
	movs r0, #60
	bl 0x0200ab6c
	movs r0, #60
	bl 0x0200a99c
	movs r0, #19
	bl 0x0200aa7c
	movs r0, #20
	bl 0x0200aa7c
	ldr r7, [pc, #536]
	ldr r3, [r7]
	cmp r3, #0
	beq .L_02001078_5
	movs r0, #21
	bl 0x0200aa7c
.L_02001078_5:
	movs r0, #22
	bl 0x0200aa7c
	movs r0, #1
	bl 0x0200a99c
	ldr r5, [pc, #520]
	movs r0, #19
	adds r1, r5, #0
	bl 0x0200aa6c
	movs r0, #20
	adds r1, r5, #0
	bl 0x0200aa6c
	ldr r3, [r7]
	cmp r3, #0
	beq .L_02001078_6
	movs r0, #21
	adds r1, r5, #0
	bl 0x0200aa6c
.L_02001078_6:
	adds r1, r5, #0
	movs r0, #22
	bl 0x0200aa84
	movs r0, #80
	bl 0x0200aa2c
	movs r1, #2
	movs r0, #1
	bl 0x0200aadc
	movs r0, #40
	bl 0x0200aa2c
	movs r1, #0
	movs r0, #1
	bl 0x0200ab0c
	movs r1, #174
	movs r2, #139
	movs r0, #17
	lsls r1, r1, #15
	lsls r2, r2, #16
	bl 0x0200aab4
	movs r1, #174
	movs r2, #139
	lsls r1, r1, #15
	movs r0, #18
	lsls r2, r2, #16
	bl 0x0200aab4
	movs r0, #1
	bl 0x0200a99c
	movs r0, #17
	movs r1, #0
	bl 0x0200aa4c
	cmp r0, #1
	bne .L_02001078_7
	ldr r3, [pc, #376]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02001078_7:
	movs r1, #1
	movs r0, #0
	bl 0x0200aadc
	movs r0, #20
	bl 0x0200aa2c
	movs r0, #2
	movs r1, #2
	bl 0x0200aadc
	movs r0, #2
	movs r1, #20
	bl 0x0200a5a8
	ldr r3, [r7]
	cmp r3, #0
	beq .L_02001078_8
	movs r1, #2
	movs r0, #3
	bl 0x0200aadc
	movs r0, #10
	bl 0x0200aa2c
	ldr r0, [pc, #344]
	bl 0x0200ab04
	movs r0, #3
	movs r1, #40
	bl 0x0200a5a8
.L_02001078_8:
	movs r0, #1
	movs r1, #1
	bl 0x0200aad4
	movs r2, #0
	ldr r1, [pc, #324]
	movs r0, #1
	bl 0x0200ab34
	movs r0, #80
	bl 0x0200aa2c
	movs r1, #2
	movs r0, #2
	bl 0x0200aadc
	ldr r0, [pc, #304]
	bl 0x0200ab04
	movs r0, #2
	movs r1, #40
	bl 0x0200a5a8
	movs r1, #3
	movs r0, #1
	bl 0x0200aadc
	movs r0, #40
	bl 0x0200aa2c
	movs r1, #2
	movs r0, #1
	bl 0x0200ab2c
	movs r0, #1
	bl 0x0200aa54
	movs r2, #1
	adds r0, #35
	ldrb r3, [r0]
	mov r8, r2
	mov r2, r8
	orrs r3, r2
	strb r3, [r0]
	movs r0, #1
	bl 0x0200aa54
	movs r1, #1
	bl 0x0200aa0c
	movs r2, #0
	movs r0, #1
	movs r1, #6
	bl 0x0200aacc
	movs r0, #1
	movs r1, #1
	bl 0x0200aabc
	movs r1, #128
	movs r2, #60
	lsls r1, r1, #7
	movs r0, #1
	bl 0x0200a5c0
	movs r0, #1
	movs r1, #20
	bl 0x0200a5a8
	movs r0, #1
	movs r1, #2
	bl 0x0200aad4
	movs r5, #128
	movs r0, #1
	movs r1, #10
	bl 0x0200a5a8
	lsls r5, r5, #6
	movs r0, #0
	movs r1, #3
	bl 0x0200aadc
	movs r0, #1
	adds r1, r5, #0
	movs r2, #20
	bl 0x0200a5c0
	movs r6, #192
	ldr r1, [pc, #156]
	movs r2, #0
	movs r0, #1
	bl 0x0200ab34
	lsls r6, r6, #7
	movs r0, #40
	bl 0x0200aa2c
	movs r0, #1
	adds r1, r6, #0
	movs r2, #40
	bl 0x0200a5c0
	movs r0, #1
	adds r1, r5, #0
	movs r2, #20
	bl 0x0200a5c0
	movs r0, #1
	adds r1, r6, #0
	movs r2, #20
	bl 0x0200a5c0
	movs r0, #1
	adds r1, r5, #0
	movs r2, #10
	bl 0x0200a5c0
	movs r1, #2
	movs r2, #0
	movs r0, #1
	bl 0x0200aacc
	movs r0, #40
	bl 0x0200aa2c
	movs r1, #2
	movs r2, #0
	movs r0, #1
	bl 0x0200aacc
	movs r0, #10
	bl 0x0200aa2c
	movs r1, #4
	movs r2, #0
	movs r0, #1
	bl 0x0200aacc
	movs r0, #20
	bl 0x0200aa2c
	movs r0, #1
	movs r1, #20
	bl 0x0200a5a8
	ldr r3, [r7]
	cmp r3, #0
	beq .L_02001078_9
	b .L_02001078_10
	.4byte 0x03001ebc
	.4byte 0x0200b694
	.4byte 0x0200b698
	.4byte 0x02008801
	.4byte 0x0200ad20
	.4byte 0x0200b69c
	.4byte 0x0200a7ad
	.4byte 0x0200ad7c
	.4byte 0x00001488
	.4byte 0x00000101
	.4byte 0x00001489
.L_02001078_10:
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #3
	bl 0x0200ab34
	movs r0, #60
	bl 0x0200aa2c
	movs r1, #2
	movs r0, #3
	bl 0x0200aadc
	movs r0, #80
	bl 0x0200aa2c
	movs r1, #2
	movs r0, #3
	bl 0x0200ab2c
	movs r0, #3
	bl 0x0200aa54
	adds r0, #35
	ldrb r3, [r0]
	mov r2, r8
	orrs r3, r2
	strb r3, [r0]
	movs r0, #3
	bl 0x0200aa54
	movs r1, #1
	bl 0x0200aa0c
	movs r0, #3
	movs r1, #4
	movs r2, #0
	bl 0x0200aacc
	movs r1, #2
	movs r2, #0
	movs r0, #3
	negs r1, r1
	bl 0x0200aaa4
	movs r0, #3
	movs r1, #1
	bl 0x0200aabc
	movs r1, #224
	movs r2, #60
	lsls r1, r1, #8
	movs r0, #3
	bl 0x0200a5c0
	movs r1, #2
	movs r0, #3
	bl 0x0200aadc
	movs r0, #20
	bl 0x0200aa2c
	movs r0, #3
	movs r1, #20
	bl 0x0200a5a8
	b .L_02001078_11
.L_02001078_9:
	ldr r3, [pc, #632]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02001078_11:
	movs r6, #128
	movs r0, #1
	movs r1, #2
	movs r2, #0
	lsls r6, r6, #7
	bl 0x0200aacc
	movs r7, #128
	movs r2, #20
	movs r0, #1
	adds r1, r6, #0
	bl 0x0200a5c0
	lsls r7, r7, #6
	movs r0, #1
	movs r1, #3
	bl 0x0200aac4
	movs r2, #10
	movs r0, #1
	adds r1, r7, #0
	bl 0x0200a5c0
	movs r0, #1
	movs r1, #20
	bl 0x0200a5a8
	movs r1, #3
	movs r0, #1
	bl 0x0200aac4
	movs r0, #10
	bl 0x0200aa2c
	movs r1, #1
	movs r0, #2
	bl 0x0200aadc
	movs r0, #40
	bl 0x0200aa2c
	movs r1, #2
	movs r0, #2
	bl 0x0200aadc
	movs r0, #20
	bl 0x0200aa2c
	movs r1, #2
	movs r0, #2
	bl 0x0200ab2c
	movs r0, #2
	bl 0x0200aa54
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #2
	bl 0x0200aa54
	movs r1, #1
	bl 0x0200aa0c
	movs r2, #0
	movs r0, #2
	movs r1, #4
	bl 0x0200aacc
	movs r0, #2
	movs r1, #1
	bl 0x0200aabc
	movs r1, #192
	movs r2, #0
	movs r0, #2
	lsls r1, r1, #8
	bl 0x0200ab24
	movs r1, #2
	movs r0, #0
	bl 0x0200aadc
	movs r0, #10
	bl 0x0200aa2c
	movs r1, #2
	movs r0, #0
	bl 0x0200ab2c
	movs r0, #0
	bl 0x0200aa54
	adds r0, #35
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r0, #0
	bl 0x0200aa54
	movs r1, #1
	bl 0x0200aa0c
	movs r5, #192
	movs r2, #0
	movs r0, #0
	movs r1, #4
	bl 0x0200aacc
	lsls r5, r5, #7
	movs r0, #0
	movs r1, #1
	bl 0x0200aabc
	movs r0, #0
	adds r1, r5, #0
	movs r2, #60
	bl 0x0200a5c0
	movs r0, #0
	ldr r1, [pc, #376]
	movs r2, #0
	bl 0x0200ab34
	ldr r1, [pc, #368]
	movs r2, #0
	movs r0, #2
	bl 0x0200ab34
	movs r0, #60
	bl 0x0200aa2c
	movs r1, #160
	movs r2, #20
	lsls r1, r1, #8
	movs r0, #0
	bl 0x0200a5c0
	movs r0, #1
	movs r1, #3
	bl 0x0200aac4
	movs r0, #0
	movs r1, #3
	bl 0x0200aac4
	movs r0, #0
	adds r1, r5, #0
	movs r2, #10
	bl 0x0200a5c0
	movs r0, #1
	adds r1, r6, #0
	movs r2, #10
	bl 0x0200a5c0
	movs r1, #192
	movs r2, #0
	movs r0, #2
	lsls r1, r1, #8
	bl 0x0200ab24
	movs r0, #2
	movs r1, #3
	bl 0x0200aac4
	movs r0, #2
	movs r1, #20
	bl 0x0200a5a8
	movs r0, #0
	movs r1, #3
	bl 0x0200aabc
	movs r0, #3
	movs r1, #3
	bl 0x0200aabc
	movs r1, #3
	movs r0, #1
	bl 0x0200aac4
	movs r0, #20
	bl 0x0200aa2c
	movs r1, #1
	movs r0, #2
	bl 0x0200aadc
	movs r0, #20
	bl 0x0200aa2c
	movs r1, #0
	movs r0, #2
	bl 0x0200ab0c
	movs r0, #2
	movs r1, #3
	bl 0x0200aabc
	movs r1, #224
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ab24
	movs r0, #1
	adds r1, r7, #0
	movs r2, #0
	bl 0x0200ab24
	movs r0, #0
	movs r1, #0
	bl 0x0200aa4c
	cmp r0, #0
	bne .L_02001078_12
	movs r0, #2
	movs r1, #3
	bl 0x0200aabc
	movs r0, #1
	movs r1, #3
	bl 0x0200aac4
	ldr r3, [pc, #160]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02001078_13
.L_02001078_12:
	movs r0, #1
	movs r1, #2
	bl 0x0200aadc
	movs r0, #1
	adds r1, r7, #0
	movs r2, #10
	bl 0x0200a5c0
	movs r0, #1
	movs r1, #2
	bl 0x0200aad4
	movs r0, #1
	movs r1, #0
	bl 0x0200ab14
.L_02001078_13:
	movs r1, #128
	movs r2, #10
	lsls r1, r1, #7
	movs r0, #1
	bl 0x0200a5c0
	movs r0, #1
	movs r1, #4
	bl 0x0200aac4
	movs r0, #1
	movs r1, #20
	bl 0x0200a5a8
	movs r1, #192
	movs r2, #10
	lsls r1, r1, #8
	movs r0, #2
	bl 0x0200a5c0
	movs r0, #2
	movs r1, #3
	bl 0x0200aac4
	movs r0, #2
	movs r1, #10
	bl 0x0200a5a8
	ldr r3, [pc, #60]
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02001078_14
	movs r0, #3
	movs r1, #2
	bl 0x0200aadc
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200a5c0
	movs r1, #128
	lsls r1, r1, #6
	movs r0, #3
	movs r2, #10
	bl 0x0200a5c0
	movs r0, #3
	movs r1, #4
	bl 0x0200aabc
	movs r0, #3
	movs r1, #10
	bl 0x0200a5a8
	b .L_02001078_15
	.4byte 0x03001ebc
	.4byte 0x00000105
	.4byte 0x0200b69c
.L_02001078_14:
	ldr r3, [pc, #364]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02001078_15:
	movs r1, #128
	movs r6, #160
	lsls r6, r6, #8
	movs r0, #1
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200ab24
	movs r2, #10
	movs r0, #0
	adds r1, r6, #0
	bl 0x0200a5c0
	movs r0, #0
	movs r1, #3
	bl 0x0200aabc
	movs r1, #3
	movs r0, #1
	bl 0x0200aac4
	movs r0, #20
	bl 0x0200aa2c
	movs r5, #128
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	lsls r5, r5, #7
	bl 0x0200ab24
	movs r2, #10
	movs r0, #1
	adds r1, r5, #0
	bl 0x0200a5c0
	movs r1, #4
	movs r0, #2
	bl 0x0200aac4
	movs r0, #20
	bl 0x0200aa2c
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200ab34
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #80
	bl 0x0200ab34
	movs r1, #224
	movs r2, #10
	lsls r1, r1, #8
	movs r0, #2
	bl 0x0200a5c0
	movs r0, #2
	movs r1, #2
	bl 0x0200aad4
	movs r0, #2
	movs r1, #20
	bl 0x0200a5a8
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200ab24
	movs r0, #0
	adds r1, r6, #0
	movs r2, #40
	bl 0x0200a5c0
	movs r0, #1
	adds r1, r5, #0
	movs r2, #0
	bl 0x0200ab24
	movs r1, #192
	lsls r1, r1, #7
	movs r0, #0
	movs r2, #10
	bl 0x0200a5c0
	movs r1, #192
	movs r2, #10
	lsls r1, r1, #8
	movs r0, #2
	bl 0x0200a5c0
	movs r0, #2
	movs r1, #3
	bl 0x0200aac4
	movs r0, #2
	movs r1, #10
	bl 0x0200a5a8
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200ab3c
	movs r0, #40
	bl 0x0200aa2c
	movs r0, #1
	movs r1, #20
	bl 0x0200a5a8
	movs r1, #3
	movs r0, #2
	bl 0x0200aac4
	movs r0, #20
	bl 0x0200aa2c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200ab3c
	movs r0, #40
	bl 0x0200aa2c
	movs r0, #1
	movs r1, #20
	bl 0x0200a5a8
	movs r0, #2
	movs r1, #3
	bl 0x0200aabc
	movs r0, #2
	movs r1, #10
	bl 0x0200a5a8
	movs r0, #1
	movs r1, #2
	bl 0x0200aadc
	movs r1, #128
	movs r2, #10
	lsls r1, r1, #6
	movs r0, #1
	bl 0x0200a5c0
	movs r1, #0
	movs r0, #1
	bl 0x0200ab0c
	movs r0, #0
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200ab24
	movs r0, #0
	movs r1, #0
	bl 0x0200aa4c
	cmp r0, #0
	bne .L_02001078_16
	movs r0, #1
	movs r1, #3
	bl 0x0200aac4
	b .L_02001078_17
	.2byte 0x0000
	.4byte 0x03001ebc
.L_02001078_16:
	movs r0, #20
	bl 0x0200aa2c
	movs r0, #1
	movs r1, #2
	bl 0x0200aadc
	movs r0, #40
	bl 0x0200aa2c
	ldr r3, [pc, #1004]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02001078_17:
	movs r1, #0
	movs r0, #1
	bl 0x0200ab14
	movs r0, #21
	bl 0x0200abcc
	movs r1, #1
	ldr r0, [pc, #976]
	bl 0x0200ab64
	movs r0, #60
	bl 0x0200ab6c
	movs r0, #60
	bl 0x0200a99c
	ldr r2, [pc, #964]
	movs r3, #0
	str r3, [r2]
	ldr r2, [pc, #960]
	movs r3, #128
	lsls r3, r3, #16
	ldr r6, [pc, #960]
	str r3, [r2]
	movs r1, #200
	movs r3, #1
	str r3, [r6]
	lsls r1, r1, #4
	ldr r0, [pc, #952]
	bl 0x0200a9a4
	movs r0, #80
	bl 0x0200aa2c
	movs r0, #0
	movs r1, #2
	bl 0x0200aad4
	movs r0, #1
	movs r1, #2
	bl 0x0200aad4
	movs r0, #3
	movs r1, #2
	bl 0x0200aad4
	movs r5, #192
	movs r1, #2
	movs r0, #2
	bl 0x0200aadc
	lsls r5, r5, #8
	movs r0, #60
	bl 0x0200aa2c
	movs r2, #10
	adds r1, r5, #0
	movs r0, #2
	bl 0x0200a5c0
	ldr r0, [pc, #892]
	bl 0x0200ab04
	movs r0, #2
	movs r1, #10
	bl 0x0200a5a8
	movs r0, #1
	adds r1, r5, #0
	movs r2, #10
	bl 0x0200a5c0
	movs r0, #0
	adds r1, r5, #0
	movs r2, #10
	bl 0x0200a5c0
	ldr r7, [pc, #860]
	ldr r3, [r7]
	cmp r3, #0
	beq .L_02001078_18
	movs r0, #3
	adds r1, r5, #0
	movs r2, #10
	bl 0x0200a5c0
.L_02001078_18:
	movs r0, #0
	bl 0x0200aa54
	adds r0, #35
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #1
	bl 0x0200aa54
	adds r0, #35
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #2
	bl 0x0200aa54
	adds r0, #35
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #3
	bl 0x0200aa54
	adds r0, #35
	ldrb r3, [r0]
	ands r5, r3
	strb r5, [r0]
	movs r1, #3
	movs r0, #0
	bl 0x0200ab2c
	movs r0, #1
	movs r1, #3
	bl 0x0200ab2c
	movs r0, #2
	movs r1, #3
	bl 0x0200ab2c
	movs r1, #3
	movs r0, #3
	bl 0x0200ab2c
	movs r3, #2
	str r3, [r6]
	movs r0, #220
	bl 0x0200abcc
	movs r1, #240
	movs r2, #248
	lsls r2, r2, #16
	movs r0, #19
	lsls r1, r1, #15
	bl 0x0200aab4
	ldr r5, [pc, #728]
	movs r0, #19
	adds r1, r5, #0
	bl 0x0200aa6c
	movs r1, #200
	movs r2, #137
	movs r0, #20
	lsls r1, r1, #15
	lsls r2, r2, #17
	bl 0x0200aab4
	movs r0, #20
	adds r1, r5, #0
	bl 0x0200aa6c
	ldr r3, [r7]
	cmp r3, #0
	beq .L_02001078_19
	movs r1, #148
	movs r2, #254
	movs r0, #21
	lsls r1, r1, #15
	lsls r2, r2, #16
	bl 0x0200aab4
	movs r0, #21
	adds r1, r5, #0
	bl 0x0200aa6c
.L_02001078_19:
	movs r1, #188
	movs r2, #225
	lsls r2, r2, #16
	movs r0, #22
	lsls r1, r1, #15
	bl 0x0200aab4
	adds r1, r5, #0
	movs r0, #22
	bl 0x0200aa6c
	movs r0, #120
	bl 0x0200aa2c
	movs r3, #3
	str r3, [r6]
	adds r5, r6, #0
.L_02001078_20:
	movs r0, #1
	bl 0x0200a99c
	ldr r3, [r5]
	cmp r3, #0
	bne .L_02001078_20
	movs r0, #17
	movs r1, #80
	bl 0x0200a5a8
	movs r0, #18
	movs r1, #20
	bl 0x0200a5a8
	movs r0, #0
	ldr r1, [pc, #608]
	movs r2, #0
	bl 0x0200ab34
	movs r0, #1
	ldr r1, [pc, #596]
	movs r2, #0
	bl 0x0200ab34
	movs r0, #2
	ldr r1, [pc, #588]
	movs r2, #0
	bl 0x0200ab34
	movs r2, #0
	ldr r1, [pc, #576]
	movs r0, #3
	bl 0x0200ab34
	movs r0, #60
	bl 0x0200aa2c
	movs r0, #18
	movs r1, #20
	bl 0x0200a5a8
	movs r0, #0
	movs r1, #3
	bl 0x0200aabc
	movs r0, #1
	movs r1, #3
	bl 0x0200aabc
	movs r0, #3
	movs r1, #3
	bl 0x0200aabc
	movs r0, #2
	movs r1, #3
	bl 0x0200aac4
	movs r0, #18
	movs r1, #10
	bl 0x0200a5a8
	movs r0, #0
	movs r1, #3
	bl 0x0200aabc
	movs r0, #1
	movs r1, #3
	bl 0x0200aabc
	movs r0, #3
	movs r1, #3
	bl 0x0200aabc
	movs r0, #2
	movs r1, #3
	bl 0x0200aac4
	movs r0, #18
	movs r1, #10
	bl 0x0200a5a8
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200ab34
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200ab34
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200ab34
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #3
	bl 0x0200ab34
	movs r0, #40
	bl 0x0200aa2c
	movs r0, #17
	movs r1, #10
	bl 0x0200a5a8
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ab24
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	movs r6, #192
	lsls r6, r6, #8
	bl 0x0200ab24
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl 0x0200ab24
	movs r2, #40
	movs r0, #2
	adds r1, r6, #0
	bl 0x0200a5c0
	movs r0, #18
	movs r1, #10
	bl 0x0200a5a8
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200ab3c
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200ab3c
	movs r1, #129
	movs r0, #2
	lsls r1, r1, #1
	bl 0x0200ab3c
	movs r1, #129
	movs r0, #3
	lsls r1, r1, #1
	bl 0x0200ab3c
	movs r0, #0
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200ab24
	movs r0, #1
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200ab24
	movs r0, #2
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200ab24
	movs r2, #80
	movs r0, #3
	adds r1, r6, #0
	bl 0x0200a5c0
	movs r0, #18
	movs r1, #10
	bl 0x0200a5a8
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ab24
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200ab24
	movs r0, #2
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200ab24
	movs r2, #40
	movs r0, #3
	movs r1, #0
	bl 0x0200a5c0
	movs r0, #17
	movs r1, #10
	bl 0x0200a5a8
	movs r0, #0
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200ab24
	movs r0, #1
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200ab24
	movs r0, #2
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200ab24
	movs r2, #10
	movs r0, #3
	adds r1, r6, #0
	bl 0x0200a5c0
	movs r0, #0
	movs r1, #4
	bl 0x0200aabc
	movs r0, #1
	movs r1, #4
	bl 0x0200aabc
	movs r0, #3
	movs r1, #4
	bl 0x0200aabc
	movs r1, #4
	movs r0, #2
	bl 0x0200aac4
	movs r0, #60
	bl 0x0200aa2c
	movs r0, #18
	movs r1, #10
	bl 0x0200a5a8
	movs r0, #0
	movs r1, #3
	bl 0x0200aabc
	movs r0, #1
	movs r1, #3
	bl 0x0200aabc
	movs r0, #3
	movs r1, #3
	bl 0x0200aabc
	movs r0, #2
	movs r1, #3
	bl 0x0200aac4
	movs r0, #18
	movs r1, #20
	bl 0x0200a5a8
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ab24
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200ab24
	movs r0, #2
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200ab24
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x0200a5c0
	movs r0, #18
	movs r1, #10
	bl 0x0200a5a8
	movs r0, #0
	b .L_02001078_21
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00406218
	.4byte 0x0200b690
	.4byte 0x0200b68c
	.4byte 0x0200b694
	.4byte 0x0200a7ad
	.4byte 0x0000149d
	.4byte 0x0200b69c
	.4byte 0x0200ad20
	.4byte 0x00000101
.L_02001078_21:
	movs r1, #2
	bl 0x0200aad4
	movs r0, #1
	movs r1, #2
	bl 0x0200aad4
	movs r0, #3
	movs r1, #2
	bl 0x0200aad4
	movs r0, #2
	movs r1, #2
	bl 0x0200aadc
	movs r0, #0
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200ab24
	movs r0, #1
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200ab24
	movs r0, #2
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200ab24
	movs r2, #0
	movs r0, #3
	adds r1, r6, #0
	bl 0x0200ab24
	movs r0, #18
	movs r1, #20
	bl 0x0200a5a8
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ab24
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200ab24
	movs r0, #2
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200ab24
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x0200a5c0
	movs r0, #17
	movs r1, #20
	bl 0x0200a5a8
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200ab3c
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200ab3c
	movs r1, #129
	movs r0, #3
	lsls r1, r1, #1
	bl 0x0200ab3c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #2
	bl 0x0200ab3c
	movs r0, #40
	bl 0x0200aa2c
	movs r0, #18
	movs r1, #10
	bl 0x0200a5a8
	movs r0, #0
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200ab24
	movs r0, #1
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200ab24
	movs r0, #2
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200ab24
	movs r2, #10
	movs r0, #3
	adds r1, r6, #0
	bl 0x0200a5c0
	movs r0, #18
	movs r1, #10
	bl 0x0200a5a8
	movs r0, #0
	movs r1, #3
	bl 0x0200aabc
	movs r0, #1
	movs r1, #3
	bl 0x0200aabc
	movs r0, #3
	movs r1, #3
	bl 0x0200aabc
	movs r1, #3
	movs r0, #2
	bl 0x0200aac4
	movs r0, #60
	bl 0x0200aa2c
	movs r0, #18
	movs r1, #0
	bl 0x0200ab14
	movs r1, #0
	movs r0, #17
	bl 0x0200ab14
	ldr r0, [pc, #964]
	bl 0x0200a9ac
	movs r0, #80
	bl 0x0200aa2c
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl 0x0200ab64
	movs r0, #60
	bl 0x0200ab6c
	movs r0, #80
	bl 0x0200a99c
	movs r0, #19
	bl 0x0200aa7c
	movs r0, #20
	bl 0x0200aa7c
	movs r0, #21
	ldr r7, [pc, #920]
	bl 0x0200aa7c
	movs r0, #22
	bl 0x0200aa7c
	movs r0, #1
	bl 0x0200a99c
	ldr r5, [pc, #904]
	movs r0, #19
	adds r1, r5, #0
	bl 0x0200aa6c
	movs r0, #20
	adds r1, r5, #0
	bl 0x0200aa6c
	ldr r3, [r7]
	cmp r3, #0
	beq .L_02001078_22
	movs r0, #21
	adds r1, r5, #0
	bl 0x0200aa6c
.L_02001078_22:
	adds r1, r5, #0
	movs r0, #22
	bl 0x0200aa84
	movs r0, #20
	bl 0x0200aa2c
	movs r0, #0
	movs r1, #2
	bl 0x0200ab2c
	movs r0, #1
	movs r1, #2
	bl 0x0200ab2c
	movs r0, #2
	movs r1, #2
	bl 0x0200ab2c
	movs r1, #2
	movs r0, #3
	bl 0x0200ab2c
	movs r0, #0
	bl 0x0200aa54
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #1
	bl 0x0200aa54
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #2
	bl 0x0200aa54
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #3
	bl 0x0200aa54
	adds r0, #35
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r5, #224
	movs r0, #2
	movs r1, #2
	lsls r5, r5, #8
	bl 0x0200aadc
	movs r2, #10
	movs r0, #2
	adds r1, r5, #0
	bl 0x0200a5c0
	movs r1, #0
	movs r0, #2
	bl 0x0200ab0c
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200ab24
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl 0x0200ab24
	movs r0, #0
	movs r1, #0
	bl 0x0200aa4c
	cmp r0, #0
	bne .L_02001078_23
	movs r1, #2
	movs r0, #1
	bl 0x0200aadc
	movs r0, #10
	bl 0x0200aa2c
	movs r1, #0
	movs r0, #1
	bl 0x0200ab0c
	movs r0, #0
	movs r1, #0
	bl 0x0200aa4c
	cmp r0, #0
	bne .L_02001078_24
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200a5c0
	movs r0, #1
	ldr r1, [pc, #664]
	movs r2, #0
	bl 0x0200ab34
	movs r0, #2
	ldr r1, [pc, #652]
	movs r2, #0
	bl 0x0200ab34
	ldr r1, [pc, #644]
	movs r2, #0
	movs r0, #3
	bl 0x0200ab34
	movs r0, #40
	bl 0x0200aa2c
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #7
	movs r0, #1
	bl 0x0200a5c0
	movs r0, #1
	movs r1, #10
	bl 0x0200a5a8
	movs r0, #2
	adds r1, r6, #0
	movs r2, #20
	bl 0x0200a5c0
	movs r2, #20
	movs r0, #2
	adds r1, r5, #0
	bl 0x0200a5c0
	movs r0, #2
	movs r1, #3
	bl 0x0200aac4
	movs r0, #2
	movs r1, #20
	bl 0x0200a5a8
	movs r1, #128
	lsls r1, r1, #6
	movs r0, #1
	movs r2, #20
	bl 0x0200a5c0
	b .L_02001078_25
.L_02001078_24:
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200a5c0
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200ab34
	movs r1, #129
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200ab34
	movs r1, #129
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200ab34
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #7
	movs r0, #1
	bl 0x0200a5c0
	ldr r0, [pc, #504]
	bl 0x0200ab04
	movs r0, #1
	movs r1, #20
	bl 0x0200a5a8
	movs r0, #2
	movs r1, #3
	bl 0x0200aac4
	movs r0, #2
	movs r1, #20
	bl 0x0200a5a8
.L_02001078_25:
	movs r0, #3
	movs r1, #3
	bl 0x0200aabc
	movs r0, #1
	movs r1, #3
	bl 0x0200aac4
	b .L_02001078_26
.L_02001078_23:
	movs r0, #20
	bl 0x0200aa2c
	movs r1, #3
	movs r0, #1
	bl 0x0200aac4
	movs r0, #10
	bl 0x0200aa2c
	ldr r0, [pc, #440]
	bl 0x0200ab04
	movs r0, #1
	movs r1, #10
	bl 0x0200a5a8
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200ab24
	movs r1, #192
	movs r2, #20
	lsls r1, r1, #7
	movs r0, #0
	bl 0x0200a5c0
	movs r0, #1
	movs r1, #3
	bl 0x0200aabc
	movs r1, #3
	movs r0, #0
	bl 0x0200aac4
	movs r0, #10
	bl 0x0200aa2c
	movs r0, #2
	movs r1, #4
	bl 0x0200aac4
	movs r1, #0
	movs r0, #2
	bl 0x0200ab0c
	movs r0, #0
	movs r1, #0
	bl 0x0200aa4c
	cmp r0, #0
	beq .L_02001078_27
	b .L_02001078_28
.L_02001078_27:
	movs r0, #20
	bl 0x0200aa2c
	ldr r1, [pc, #348]
	movs r2, #0
	movs r0, #2
	bl 0x0200ab34
	movs r0, #40
	bl 0x0200aa2c
	movs r0, #2
	adds r1, r5, #0
	movs r2, #10
	bl 0x0200a5c0
	movs r0, #2
	movs r1, #10
	bl 0x0200a5a8
	ldr r3, [r7]
	cmp r3, #0
	beq .L_02001078_29
	movs r0, #3
	movs r1, #0
	movs r2, #10
	bl 0x0200a5c0
	movs r0, #3
	movs r1, #3
	bl 0x0200aad4
	movs r0, #3
	movs r1, #20
	bl 0x0200a5a8
	b .L_02001078_30
.L_02001078_29:
	ldr r3, [pc, #284]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02001078_30:
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200ab3c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200ab3c
	movs r0, #40
	bl 0x0200aa2c
	movs r0, #1
	movs r1, #2
	bl 0x0200aadc
	movs r0, #1
	movs r1, #20
	bl 0x0200a5a8
	ldr r1, [pc, #232]
	movs r2, #0
	movs r0, #1
	bl 0x0200ab34
	movs r0, #120
	bl 0x0200aa2c
	movs r0, #2
	movs r1, #40
	bl 0x0200a5a8
	ldr r3, [pc, #180]
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02001078_31
	movs r1, #128
	lsls r1, r1, #6
	movs r0, #3
	movs r2, #10
	bl 0x0200a5c0
	movs r0, #3
	movs r1, #4
	bl 0x0200aac4
	movs r0, #3
	movs r1, #10
	bl 0x0200a5a8
	b .L_02001078_32
.L_02001078_31:
	ldr r3, [pc, #164]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02001078_32:
	movs r0, #60
	bl 0x0200aa2c
	movs r0, #2
	movs r1, #2
	bl 0x0200aadc
	ldr r3, [pc, #112]
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02001078_33
	movs r1, #160
	lsls r1, r1, #8
	movs r0, #2
	movs r2, #40
	bl 0x0200a5c0
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #2
	movs r2, #20
	bl 0x0200a5c0
.L_02001078_33:
	movs r0, #2
	movs r1, #10
	bl 0x0200a5a8
	movs r0, #0
	movs r1, #2
	bl 0x0200aad4
	movs r1, #2
	movs r0, #1
	bl 0x0200aadc
	movs r0, #40
	bl 0x0200aa2c
	movs r0, #2
	movs r1, #20
	bl 0x0200a5a8
	movs r0, #0
	movs r1, #3
	bl 0x0200aac4
	movs r1, #3
	movs r0, #1
	bl 0x0200aac4
	movs r0, #20
	bl 0x0200aa2c
	movs r0, #3
	movs r1, #3
	bl 0x0200aabc
.L_02001078_26:
	movs r0, #2
	movs r1, #3
	bl 0x0200aac4
	b .L_02001078_34
	.4byte 0x0200a7ad
	.4byte 0x0200b69c
	.4byte 0x0200ad7c
	.4byte 0x00000101
	.4byte 0x000014b4
	.4byte 0x000014b6
	.4byte 0x00000103
	.4byte 0x03001ebc
	.4byte 0x00000105
.L_02001078_28:
	movs r2, #0
	ldr r1, [pc, #492]
	movs r0, #2
	bl 0x0200ab34
	movs r0, #40
	bl 0x0200aa2c
	movs r1, #3
	movs r0, #2
	bl 0x0200aac4
	ldr r0, [pc, #472]
	bl 0x0200ab04
	movs r0, #2
	movs r1, #20
	bl 0x0200a5a8
	ldr r3, [r7]
	cmp r3, #0
	beq .L_02001078_35
	movs r0, #3
	movs r1, #0
	movs r2, #10
	bl 0x0200a5c0
	movs r0, #3
	movs r1, #1
	bl 0x0200aad4
	movs r0, #3
	movs r1, #20
	bl 0x0200a5a8
	b .L_02001078_36
.L_02001078_35:
	ldr r3, [pc, #428]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02001078_36:
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200ab3c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #0
	bl 0x0200ab3c
	movs r0, #40
	bl 0x0200aa2c
	movs r0, #1
	movs r1, #2
	bl 0x0200aadc
	movs r0, #1
	movs r1, #20
	bl 0x0200a5a8
	ldr r1, [pc, #364]
	movs r2, #0
	movs r0, #2
	bl 0x0200ab34
	movs r0, #80
	bl 0x0200aa2c
	movs r0, #2
	movs r1, #40
	bl 0x0200a5a8
	ldr r3, [pc, #352]
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02001078_37
	movs r1, #128
	lsls r1, r1, #6
	movs r0, #3
	movs r2, #20
	bl 0x0200a5c0
	movs r0, #3
	movs r1, #4
	bl 0x0200aabc
	movs r0, #3
	movs r1, #40
	bl 0x0200a5a8
	b .L_02001078_38
.L_02001078_37:
	ldr r3, [pc, #308]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02001078_38:
	movs r1, #2
	movs r0, #2
	bl 0x0200aadc
	movs r0, #20
	bl 0x0200aa2c
	movs r0, #2
	movs r1, #20
	bl 0x0200a5a8
	movs r0, #1
	movs r1, #2
	bl 0x0200aad4
	movs r1, #2
	movs r0, #0
	bl 0x0200aadc
	movs r0, #40
	bl 0x0200aa2c
	movs r0, #2
	movs r1, #20
	bl 0x0200a5a8
.L_02001078_34:
	movs r0, #17
	bl 0x0200abcc
	movs r0, #1
	ldr r1, [pc, #240]
	ldr r2, [pc, #244]
	bl 0x0200aa64
	movs r0, #2
	ldr r1, [pc, #232]
	ldr r2, [pc, #232]
	bl 0x0200aa64
	movs r0, #3
	ldr r1, [pc, #220]
	ldr r2, [pc, #224]
	bl 0x0200aa64
	movs r0, #1
	movs r1, #2
	bl 0x0200aabc
	movs r0, #0
	bl 0x0200aa54
	cmp r0, #0
	beq .L_02001078_39
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x0200aa8c
.L_02001078_39:
	movs r0, #1
	bl 0x0200aaac
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200aab4
	movs r0, #2
	movs r1, #2
	bl 0x0200aabc
	movs r0, #0
	bl 0x0200aa54
	cmp r0, #0
	beq .L_02001078_40
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #2
	bl 0x0200aa8c
.L_02001078_40:
	movs r0, #2
	bl 0x0200aaac
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x0200aab4
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl 0x0200aab4
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl 0x0200aab4
	ldr r3, [pc, #92]
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02001078_41
	movs r0, #3
	movs r1, #2
	bl 0x0200aabc
	movs r0, #0
	bl 0x0200aa54
	cmp r0, #0
	beq .L_02001078_42
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #3
	bl 0x0200aa8c
.L_02001078_42:
	movs r0, #3
	bl 0x0200aaac
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl 0x0200aab4
.L_02001078_41:
	ldr r0, [pc, #48]
	bl 0x0200aa24
	bl 0x0200abb4
	bl 0x0200aa3c
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000105
	.4byte 0x000014bf
	.4byte 0x03001ebc
	.4byte 0x0200b69c
	.4byte 0x00013333
	.4byte 0x00009999
	.4byte 0x00000843
	.global Func_020025a8
	.thumb_func
Func_020025a8:
	push {r5, lr}
	adds r5, r1, #0
	movs r1, #0
	bl 0x0200ab14
	adds r0, r5, #0
	bl 0x0200aa2c
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020025c0
	.thumb_func
Func_020025c0:
	push {r5, lr}
	adds r5, r2, #0
	movs r2, #0
	bl 0x0200ab24
	adds r0, r5, #0
	bl 0x0200aa2c
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020025d8
	.thumb_func
Func_020025d8:
	push {lr}
	ldr r3, [r0, #24]
	ldr r2, [pc, #36]
	adds r3, r3, r2
	str r3, [r0, #24]
	movs r2, #128
	ldr r3, [r0, #56]
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_020025d8_0
	ldr r2, [r0, #60]
	cmp r2, r3
	bne .L_020025d8_0
	ldr r3, [r0, #64]
	cmp r3, r2
	bne .L_020025d8_0
	bl 0x0200a9dc
.L_020025d8_0:
	movs r0, #1
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x00001eb8
	.global Func_02002608
	.thumb_func
Func_02002608:
	push {r5, r6, lr}
	ldr r3, [pc, #132]
	ldr r6, [r3]
	movs r3, #7
	ands r6, r3
	cmp r6, #0
	bne .L_02002608_0
	ldr r3, [pc, #124]
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02002608_1
	movs r0, #200
	bl 0x0200abcc
.L_02002608_1:
	movs r1, #196
	movs r3, #210
	movs r0, #26
	lsls r1, r1, #15
	movs r2, #0
	lsls r3, r3, #15
	bl 0x0200a9d4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02002608_0
	ldr r1, [r5, #80]
	adds r0, #35
	adds r3, r1, #0
	ldrb r2, [r0]
	adds r3, #38
	strb r6, [r3]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	ldrb r2, [r1, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	ldr r3, [pc, #60]
	str r3, [r5, #24]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r5, #48]
	str r3, [r5, #52]
	adds r3, r5, #0
	adds r3, #85
	strb r6, [r3]
	adds r0, r5, #0
	movs r1, #2
	bl 0x0200a9c4
	movs r1, #196
	adds r0, r5, #0
	lsls r1, r1, #15
	movs r2, #0
	ldr r3, [pc, #28]
	bl 0x0200a9ec
	ldr r1, [pc, #28]
	adds r0, r5, #0
	bl 0x0200a9cc
.L_02002608_0:
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x03001e40
	.4byte 0x0200b6a0
	.4byte 0x00001999
	.4byte 0x010d0000
	.4byte 0x0200b5d8
	.global Func_020026a4
	.thumb_func
Func_020026a4:
	push {lr}
	ldr r3, [pc, #32]
	ldr r3, [r3]
	movs r2, #1
	lsrs r3, r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_020026a4_0
	movs r1, #10
	bl 0x0200aaf4
	b .L_020026a4_1
.L_020026a4_0:
	movs r1, #7
	bl 0x0200aaf4
.L_020026a4_1:
	movs r0, #0
	pop {r1}
	bx r1
	.4byte 0x03001e40
	.global Func_020026cc
	.thumb_func
Func_020026cc:
	push {r5, lr}
	ldr r3, [pc, #164]
	ldr r3, [r3]
	adds r5, r0, #0
	cmp r3, #0
	beq .L_020026cc_0
	ldr r1, [pc, #156]
	ldr r2, [r5, #8]
	adds r3, r2, r1
	ldr r1, [pc, #156]
	cmp r3, r1
	bhi .L_020026cc_1
	movs r1, #211
	ldr r3, [r5, #16]
	lsls r1, r1, #16
	cmp r3, r1
	ble .L_020026cc_1
	ldr r1, [pc, #144]
	cmp r3, r1
	ble .L_020026cc_2
.L_020026cc_1:
	ldr r1, [pc, #140]
	adds r3, r2, r1
	ldr r2, [pc, #140]
	cmp r3, r2
	bhi .L_020026cc_3
	movs r1, #194
	b .L_020026cc_4
.L_020026cc_0:
	ldr r1, [pc, #116]
	ldr r2, [r5, #8]
	adds r3, r2, r1
	ldr r1, [pc, #128]
	cmp r3, r1
	bhi .L_020026cc_5
	movs r1, #194
	ldr r3, [r5, #16]
	lsls r1, r1, #16
	cmp r3, r1
	ble .L_020026cc_5
	movs r1, #230
	lsls r1, r1, #16
	cmp r3, r1
	blt .L_020026cc_2
.L_020026cc_5:
	ldr r1, [pc, #108]
	adds r3, r2, r1
	ldr r1, [pc, #108]
	cmp r3, r1
	bhi .L_020026cc_6
	movs r1, #216
	ldr r3, [r5, #16]
	lsls r1, r1, #16
	cmp r3, r1
	ble .L_020026cc_6
	movs r1, #250
	lsls r1, r1, #16
	cmp r3, r1
	blt .L_020026cc_2
.L_020026cc_6:
	ldr r1, [pc, #88]
	adds r3, r2, r1
	ldr r2, [pc, #88]
	cmp r3, r2
	bhi .L_020026cc_3
	movs r1, #241
.L_020026cc_4:
	ldr r3, [r5, #16]
	lsls r1, r1, #16
	cmp r3, r1
	ble .L_020026cc_3
	ldr r2, [pc, #76]
	cmp r3, r2
	bgt .L_020026cc_3
.L_020026cc_2:
	movs r0, #106
	bl 0x0200abcc
	ldr r1, [pc, #68]
	adds r0, r5, #0
	bl 0x0200a9cc
	ldr r2, [pc, #64]
	movs r3, #1
	str r3, [r2]
.L_020026cc_3:
	movs r0, #0
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x0200b69c
	.4byte 0xffc4ffff
	.4byte 0x0051fffe
	.4byte 0x0100ffff
	.4byte 0xffbaffff
	.4byte 0x0034fffe
	.4byte 0x0033fffe
	.4byte 0xff90ffff
	.4byte 0x001dfffe
	.4byte 0xffb1ffff
	.4byte 0x002bfffe
	.4byte 0x0114ffff
	.4byte 0x0200b5ec
	.4byte 0x0200b698
	.global Func_020027ac
	.thumb_func
Func_020027ac:
	push {r5, r6, r7, lr}
	ldr r2, [pc, #304]
	ldr r3, [r2]
	movs r5, #0
	cmp r3, #2
	beq .L_020027ac_0
	cmp r3, #2
	bhi .L_020027ac_1
	cmp r3, #1
	beq .L_020027ac_2
	b .L_020027ac_3
.L_020027ac_1:
	cmp r3, #3
	beq .L_020027ac_4
	b .L_020027ac_3
.L_020027ac_2:
	ldr r2, [pc, #280]
	ldr r1, [pc, #284]
	ldr r3, [r2]
	cmp r3, r1
	bgt .L_020027ac_5
	adds r3, #50
	str r3, [r2]
.L_020027ac_5:
	ldr r2, [pc, #276]
	movs r1, #240
	ldr r3, [r2]
	lsls r1, r1, #14
	b .L_020027ac_6
.L_020027ac_0:
	ldr r2, [pc, #256]
	ldr r1, [pc, #268]
	ldr r3, [r2]
	cmp r3, r1
	bgt .L_020027ac_7
	adds r3, #50
	str r3, [r2]
.L_020027ac_7:
	ldr r2, [pc, #252]
	movs r1, #192
	ldr r3, [r2]
	lsls r1, r1, #13
.L_020027ac_6:
	cmp r3, r1
	ble .L_020027ac_3
	ldr r1, [pc, #248]
	adds r3, r3, r1
	str r3, [r2]
	b .L_020027ac_3
.L_020027ac_4:
	ldr r0, [pc, #232]
	ldr r3, [pc, #240]
	ldr r1, [r0]
	cmp r1, r3
	bge .L_020027ac_8
	str r5, [r2]
	b .L_020027ac_3
.L_020027ac_8:
	ldr r3, [pc, #208]
	ldr r2, [r3]
	adds r2, #50
	str r2, [r3]
	ldr r2, [pc, #216]
	adds r3, r1, r2
	str r3, [r0]
.L_020027ac_3:
	ldr r7, [pc, #220]
	ldr r3, [r7]
	movs r2, #7
	ands r3, r2
	cmp r3, #0
	bne .L_020027ac_9
	ldr r0, [pc, #212]
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl 0x0200a9d4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020027ac_9
	ldr r3, [pc, #196]
	ldr r3, [r3]
	ldr r6, [r3]
	ldr r3, [r7]
	movs r2, #63
	ands r3, r2
	cmp r3, #0
	bne .L_020027ac_10
	movs r0, #246
	bl 0x0200abcc
.L_020027ac_10:
	ldr r3, [pc, #140]
	ldr r3, [r3]
	cmp r3, #0
	beq .L_020027ac_11
	bl 0x0200a9b4
	ldr r3, [pc, #132]
	ldr r3, [r3]
	muls r3, r0
	ldr r2, [r6]
	lsrs r3, r3, #16
	lsls r3, r3, #8
	adds r2, r2, r3
	ldr r3, [pc, #124]
	ldr r3, [r3]
	adds r7, r2, r3
	b .L_020027ac_12
.L_020027ac_11:
	bl 0x0200a9b4
	ldr r3, [r6]
	lsls r0, r0, #8
	ldr r1, [pc, #120]
	adds r3, r3, r0
	adds r7, r3, r1
.L_020027ac_12:
	bl 0x0200a9b4
	ldr r2, [r6, #8]
	lsls r0, r0, #8
	ldr r3, [pc, #108]
	adds r2, r2, r0
	adds r2, r2, r3
	adds r3, r5, #0
	movs r0, #0
	adds r3, #85
	strb r0, [r3]
	movs r3, #160
	lsls r3, r3, #16
	str r3, [r5, #12]
	ldr r1, [r5, #80]
	ldr r3, [pc, #100]
	str r3, [r5, #24]
	str r3, [r5, #28]
	adds r3, r1, #0
	adds r3, #38
	str r7, [r5, #8]
	str r2, [r5, #16]
	strb r0, [r3]
	adds r0, r5, #0
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	ldrb r2, [r1, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200a9c4
	ldr r1, [pc, #56]
	adds r0, r5, #0
	bl 0x0200a9cc
.L_020027ac_9:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0200b694
	.4byte 0x0200b690
	.4byte 0x00003a97
	.4byte 0x0200b68c
	.4byte 0x0000752f
	.4byte 0xffffc000
	.4byte 0xff800000
	.4byte 0x03001e40
	.4byte 0x0000011d
	.4byte 0x03001e70
	.4byte 0x0000e666
	.4byte 0x0200b610
	.global Func_02002910
	.thumb_func
Func_02002910:
	push {lr}
	ldr r0, [pc, #128]
	sub sp, #8
	bl 0x0200aa1c
	cmp r0, #0
	bne .L_02002910_0
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x0200aab4
	movs r3, #9
	movs r2, #18
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #9
	movs r1, #17
	movs r2, #5
	movs r3, #1
	bl 0x0200aa04
	bl 0x0200a9e4
	movs r0, #1
	bl 0x0200a99c
	b .L_02002910_1
.L_02002910_0:
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200aab4
.L_02002910_1:
	ldr r0, [pc, #68]
	bl 0x0200aa1c
	cmp r0, #0
	beq .L_02002910_2
	movs r3, #5
	movs r2, #7
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #91
	movs r1, #19
	movs r2, #72
	movs r3, #9
	bl 0x0200a9fc
	movs r3, #8
	movs r2, #11
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #23
	movs r1, #11
	movs r2, #5
	movs r3, #7
	bl 0x0200aa04
	bl 0x0200a9e4
	movs r0, #1
	bl 0x0200a99c
.L_02002910_2:
	sub sp, #-8
	pop {r0}
	bx r0
	.4byte 0x00000845
	.4byte 0x00000847
	.include "games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MURA/IMPORT.INC"
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00610000
	.4byte 0x00000000
	.4byte 0x00dd0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000e000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00650000
	.4byte 0x00000000
	.4byte 0x010c0000
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
	.4byte 0x004e0000
	.4byte 0x00000000
	.4byte 0x00f70000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
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
	.4byte 0x00000002
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00001999
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00001999
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x0000051e
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x0000051e
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000046
	.4byte 0xc0010000
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000007
	.4byte 0xffffe667
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xfffffc29
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffffc29
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000005a
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000c000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000c000
	.4byte 0x00000015
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000022
	.4byte 0x02008051
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x020080cd
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x02008115
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00200042
	.4byte 0x00020001
	.4byte 0x00430004
	.4byte 0x00010020
	.4byte 0x00040002
	.4byte 0x0000ffff
	.4byte 0xffff0000
	.4byte 0x000000e8
	.4byte 0x40000078
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x0000002c
	.4byte 0x0000010b
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000001a2
	.4byte 0x8000011d
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000077
	.4byte 0x400000b6
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x00000097
	.4byte 0x40000136
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x00000108
	.4byte 0x40000175
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x00000149
	.4byte 0x400000e6
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0007
	.4byte 0x00000107
	.4byte 0xc0000146
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x00000168
	.4byte 0x4000015a
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x0000002c
	.4byte 0x0000010b
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000c
	.4byte 0x000000e8
	.4byte 0x400000b0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x000000aa
	.4byte 0xc0000182
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x000001a0
	.4byte 0xffff0002
	.4byte 0x000000a8
	.4byte 0x40000032
	.4byte 0x00300000
	.4byte 0x01200000
	.4byte 0x000001a0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x000000d8
	.4byte 0x40000140
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000138
	.4byte 0x40000138
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x001f00cf
	.4byte 0x00e10137
	.4byte 0x01490031
	.4byte 0x0001ffff
	.4byte 0x001f012f
	.4byte 0x0141012f
	.4byte 0x01410031
	.4byte 0x0002ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000024
	.4byte 0x00107002
	.4byte 0x00226002
	.4byte 0x00504025
	.4byte 0x00603025
	.4byte 0x00707025
	.4byte 0x0080a025
	.4byte 0x00908025
	.4byte 0x00a02026
	.4byte 0x00000027
	.4byte 0x0010b002
	.4byte 0x0022e002
	.4byte 0x00000026
	.4byte 0x00109025
	.4byte 0x00208024
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
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
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00004000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00008000
	.4byte 0xffff0065
	.4byte 0x00000002
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00008000
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00034000
	.4byte 0xffff006f
	.4byte 0x00000002
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00008000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00010000
	.4byte 0xffff0066
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00018000
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00014000
	.4byte 0x08430037
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x08430038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x084300dc
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x084300dc
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01004000
	.4byte 0x084300dc
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01004000
	.4byte 0x084300dc
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01004000
	.4byte 0x0845011d
	.4byte 0x00000001
	.4byte 0x00180000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00004000
	.4byte 0x0845011d
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x01004000
	.4byte 0x0845011d
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x01004000
	.4byte 0x0845011d
	.4byte 0x00000001
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01004000
	.4byte 0x0031005a
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0845011d
	.4byte 0x00000007
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00004000
	.4byte 0x0845011d
	.4byte 0x00000007
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x01004000
	.4byte 0x0845011d
	.4byte 0x00000007
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01004000
	.4byte 0x0845011d
	.4byte 0x00000007
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x01004000
	.4byte 0x0845011d
	.4byte 0x00000007
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x01004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0065
	.4byte 0x00000001
	.4byte 0x00ea0000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00035000
	.4byte 0xffff0069
	.4byte 0x00000001
	.4byte 0x00ea0000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0063
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008305
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008305
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008305
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x02008305
	.4byte 0x00004602
	.4byte 0xffff0009
	.4byte 0x02008305
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x18450008
	.4byte 0x0000168d
	.4byte 0x00000000
	.4byte 0x18450009
	.4byte 0x0000168e
	.4byte 0x00000000
	.4byte 0x1845000a
	.4byte 0x0000168f
	.4byte 0x00000000
	.4byte 0x1845000b
	.4byte 0x00001690
	.4byte 0x00000000
	.4byte 0x1845000c
	.4byte 0x00001691
	.4byte 0x00000000
	.4byte 0x1845000d
	.4byte 0x00001692
	.4byte 0x00000000
	.4byte 0x1845000e
	.4byte 0x00001693
	.4byte 0x00000000
	.4byte 0x1845000f
	.4byte 0x00001694
	.4byte 0x00000000
	.4byte 0x18450010
	.4byte 0x02008231
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x02008275
	.4byte 0x00008d15
	.4byte 0x18450008
	.4byte 0x00001695
	.4byte 0x00008d15
	.4byte 0x18450009
	.4byte 0x00001696
	.4byte 0x00008d15
	.4byte 0x1845000a
	.4byte 0x00001697
	.4byte 0x00008d15
	.4byte 0x1845000b
	.4byte 0x00001698
	.4byte 0x00008d15
	.4byte 0x1845000c
	.4byte 0x00001699
	.4byte 0x00008d15
	.4byte 0x1845000d
	.4byte 0x0000169a
	.4byte 0x00008d15
	.4byte 0x1845000e
	.4byte 0x0000169b
	.4byte 0x00008d15
	.4byte 0x1845000f
	.4byte 0x0000169c
	.4byte 0x00008d15
	.4byte 0x18450010
	.4byte 0x000016b6
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001675
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001676
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001677
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001678
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001679
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000167a
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000167b
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x0000167c
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001688
	.4byte 0x00000023
	.4byte 0x0f570064
	.4byte 0x001000c1
	.4byte 0x00000023
	.4byte 0x0f580065
	.4byte 0x001000b4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x0847000a
	.4byte 0x020083c9
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001788
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001789
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x0200a5d9
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00020000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00020000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x0000001b
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0xfffe0000
	.4byte 0x00000022
	.4byte 0x0200a6a5
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0xfffe0000
	.4byte 0x00000022
	.4byte 0x0200a6a5
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0xffff0000
	.4byte 0x00000022
	.4byte 0x0200a6a5
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000028
	.4byte 0xc0010000
	.4byte 0x00000022
	.4byte 0x0200a6cd
	.4byte 0x0000001b
