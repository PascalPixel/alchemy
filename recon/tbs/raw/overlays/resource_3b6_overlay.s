.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/TOREBI_HEYA/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02008bb4
	.global Func_02000038
	.thumb_func
Func_02000038:
	movs r0, #0
	bx lr
	.global Func_0200003c
	.thumb_func
Func_0200003c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02008dac
	.global Func_02000044
	.thumb_func
Func_02000044:
	push {lr}
	movs r0, #149
	lsls r0, r0, #4
	bl 0x020089c8
	cmp r0, #0
	beq .L_02000044_0
	ldr r0, [pc, #8]
	b .L_02000044_1
.L_02000044_0:
	ldr r0, [pc, #8]
.L_02000044_1:
	pop {r1}
	bx r1
	.4byte 0x02009040
	.4byte 0x02008e00
	.global Func_02000064
	.thumb_func
Func_02000064:
	push {lr}
	movs r0, #149
	lsls r0, r0, #4
	bl 0x020089c8
	cmp r0, #0
	beq .L_02000064_0
	ldr r0, [pc, #24]
	b .L_02000064_1
.L_02000064_0:
	ldr r0, [pc, #24]
	bl 0x020089c8
	cmp r0, #0
	beq .L_02000064_2
	ldr r0, [pc, #16]
	b .L_02000064_1
.L_02000064_2:
	ldr r0, [pc, #16]
.L_02000064_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x020099d0
	.4byte 0x00000962
	.4byte 0x02009670
	.4byte 0x02009310
	.global Func_0200009c
	.thumb_func
Func_0200009c:
	push {lr}
	ldr r3, [pc, #28]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #65
	str r3, [r2]
	subs r3, #57
	adds r2, r1, r3
	movs r3, #24
	str r3, [r2]
	bl 0x02008a60
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.global Func_020000c0
	.thumb_func
Func_020000c0:
	push {r5, r6, r7, lr}
	ldr r3, [pc, #112]
	ldr r6, [r3]
	movs r5, #8
	movs r7, #0
.L_020000c0_1:
	adds r0, r5, #0
	bl 0x020089f8
	cmp r0, #0
	beq .L_020000c0_0
	adds r3, r0, #0
	adds r3, #85
	strb r7, [r3]
.L_020000c0_0:
	adds r5, #1
	cmp r5, #65
	bls .L_020000c0_1
	movs r3, #182
	lsls r3, r3, #1
	adds r6, r6, r3
	movs r3, #0
	ldrsh r5, [r6, r3]
	movs r0, #158
	subs r5, #14
	bl 0x02008a88
	lsls r5, r5, #3
	ldr r0, [pc, #64]
	adds r3, r5, #4
	ldrh r1, [r0, r3]
	adds r3, r3, r0
	ldrh r2, [r3, #2]
	ldr r0, [r0, r5]
	bl 0x020089b0
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #7
	lsls r1, r1, #8
	movs r0, #0
	bl 0x02008a00
	movs r0, #0
	bl 0x020089f8
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #2
	movs r0, #0
	bl 0x02008a18
	movs r3, #0
	ldrsh r0, [r6, r3]
	bl 0x02008a60
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x02009dcc
	.global Func_0200013c
	.thumb_func
Func_0200013c:
	push {r5, lr}
	bl 0x020089e0
	ldr r0, [pc, #460]
	bl 0x02008a38
	movs r0, #40
	bl 0x020089d8
	movs r0, #142
	movs r1, #150
	movs r3, #206
	lsls r3, r3, #18
	movs r2, #0
	lsls r1, r1, #18
	lsls r0, r0, #1
	bl 0x020089a0
	movs r1, #0
	adds r5, r0, #0
	bl 0x020089c0
	adds r0, r5, #0
	movs r1, #6
	bl 0x02008998
	movs r0, #10
	bl 0x020089d8
	movs r1, #1
	adds r0, r5, #0
	bl 0x02008998
	movs r0, #40
	bl 0x020089d8
	adds r0, r5, #0
	bl 0x020089a8
	movs r0, #2
	bl 0x020089d8
	movs r1, #128
	movs r0, #25
	lsls r1, r1, #1
	movs r2, #50
	bl 0x02008a58
	movs r1, #128
	movs r2, #128
	movs r0, #25
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x02008a00
	movs r1, #150
	movs r2, #212
	movs r0, #25
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl 0x02008a08
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #25
	bl 0x02008a50
	movs r0, #40
	bl 0x020089d8
	movs r0, #25
	movs r1, #0
	bl 0x02008a48
	movs r1, #2
	movs r0, #25
	bl 0x02008a30
	movs r0, #30
	bl 0x020089d8
	movs r1, #142
	movs r2, #212
	movs r0, #25
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl 0x02008a08
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #25
	bl 0x02008a50
	movs r0, #30
	bl 0x020089d8
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #50
	movs r0, #25
	bl 0x02008a58
	movs r0, #20
	bl 0x020089d8
	movs r2, #16
	movs r1, #0
	negs r2, r2
	movs r0, #0
	bl 0x02008a70
	movs r0, #20
	bl 0x020089d8
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #25
	bl 0x02008a50
	movs r0, #30
	bl 0x020089d8
	movs r1, #2
	movs r0, #25
	bl 0x02008a30
	movs r0, #20
	bl 0x020089d8
	movs r1, #0
	movs r0, #25
	bl 0x02008a48
	movs r0, #20
	bl 0x020089d8
	movs r2, #50
	ldr r1, [pc, #188]
	movs r0, #0
	bl 0x02008a58
	movs r0, #20
	bl 0x020089d8
	movs r1, #4
	movs r0, #25
	bl 0x02008a20
	movs r0, #20
	bl 0x020089d8
	movs r1, #0
	movs r0, #25
	bl 0x02008a48
	movs r0, #30
	bl 0x020089d8
	movs r1, #129
	movs r2, #50
	movs r0, #25
	lsls r1, r1, #1
	bl 0x02008a58
	movs r0, #25
	movs r1, #0
	bl 0x02008a48
	movs r0, #25
	ldr r1, [pc, #128]
	ldr r2, [pc, #132]
	bl 0x02008a00
	movs r0, #25
	movs r1, #16
	movs r2, #0
	bl 0x02008a70
	movs r2, #32
	movs r1, #0
	movs r0, #25
	bl 0x02008a70
	movs r0, #20
	bl 0x020089d8
	movs r1, #3
	movs r0, #25
	bl 0x02008a20
	movs r0, #20
	bl 0x020089d8
	movs r0, #25
	movs r1, #0
	bl 0x02008a48
	movs r0, #0
	movs r1, #16
	movs r2, #0
	bl 0x02008a70
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #0
	bl 0x02008a50
	movs r0, #20
	bl 0x020089d8
	movs r0, #25
	ldr r1, [pc, #52]
	ldr r2, [pc, #52]
	bl 0x02008a00
	movs r0, #25
	movs r1, #0
	movs r2, #48
	bl 0x02008a70
	movs r0, #25
	movs r1, #0
	movs r2, #0
	bl 0x02008a10
	bl 0x020089e8
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00002394
	.4byte 0x00000101
	.4byte 0x00016666
	.4byte 0x0000b333
	.4byte 0x0001cccc
	.4byte 0x0000e666
	.global Func_02000328
	.thumb_func
Func_02000328:
	push {lr}
	bl 0x020089e0
	ldr r0, [pc, #168]
	bl 0x02008a38
	movs r0, #30
	bl 0x020089d8
	movs r0, #31
	movs r1, #4
	movs r2, #13
	bl 0x02008a28
	movs r2, #30
	movs r0, #31
	movs r1, #4
	bl 0x02008a28
	movs r1, #0
	movs r0, #31
	bl 0x02008a48
	movs r0, #10
	bl 0x020089d8
	movs r1, #129
	movs r2, #50
	lsls r1, r1, #1
	movs r0, #32
	bl 0x02008a58
	movs r0, #10
	bl 0x020089d8
	movs r1, #3
	movs r0, #32
	bl 0x02008a20
	movs r0, #30
	bl 0x020089d8
	movs r1, #0
	movs r0, #32
	bl 0x02008a48
	movs r0, #10
	bl 0x020089d8
	movs r1, #4
	movs r0, #33
	bl 0x02008a20
	movs r0, #20
	bl 0x020089d8
	movs r1, #0
	movs r0, #33
	bl 0x02008a48
	movs r0, #10
	bl 0x020089d8
	movs r1, #2
	movs r0, #31
	bl 0x02008a30
	movs r0, #20
	bl 0x020089d8
	movs r1, #0
	movs r0, #31
	bl 0x02008a48
	movs r0, #10
	bl 0x020089d8
	movs r1, #3
	movs r0, #32
	bl 0x02008a20
	movs r0, #30
	bl 0x020089d8
	bl 0x020089e8
	pop {r0}
	bx r0
	.4byte 0x000023a4
	.global Func_020003dc
	.thumb_func
Func_020003dc:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	ldr r3, [pc, #268]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #149
	adds r2, #73
	str r2, [r3]
	lsls r0, r0, #4
	sub sp, #8
	bl 0x020089c8
	cmp r0, #0
	beq .L_020003dc_0
	movs r3, #51
	movs r2, #45
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #47
	movs r2, #3
	movs r3, #1
	movs r0, #51
	bl 0x020089b8
	movs r0, #31
	bl 0x020089f8
	movs r2, #0
	adds r3, r0, #0
	mov r8, r2
	adds r3, #35
	mov r2, r8
	strb r2, [r3]
	ldr r1, [r0, #80]
	movs r5, #13
	ldrb r2, [r1, #9]
	negs r5, r5
	adds r3, r5, #0
	ands r3, r2
	movs r6, #8
	orrs r3, r6
	strb r3, [r1, #9]
	movs r0, #32
	bl 0x020089f8
	adds r3, r0, #0
	adds r3, #35
	mov r2, r8
	strb r2, [r3]
	ldr r2, [r0, #80]
	ldrb r3, [r2, #9]
	ands r5, r3
	orrs r5, r6
	strb r5, [r2, #9]
	ldr r0, [pc, #164]
	bl 0x020089c8
	cmp r0, #0
	beq .L_020003dc_1
	movs r1, #140
	movs r2, #170
	movs r0, #25
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl 0x02008a10
	movs r1, #128
	movs r0, #25
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02008a50
.L_020003dc_1:
	ldr r5, [pc, #132]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #19
	bne .L_020003dc_2
	ldr r0, [pc, #112]
	bl 0x020089c8
	cmp r0, #0
	bne .L_020003dc_2
	ldr r0, [pc, #100]
	bl 0x020089d0
	bl 0x02008a68
	bl 0x0200813c
.L_020003dc_2:
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #16
	bne .L_020003dc_3
	movs r0, #192
	lsls r0, r0, #2
	bl 0x020089c8
	cmp r0, #0
	bne .L_020003dc_3
	movs r0, #192
	lsls r0, r0, #2
	bl 0x020089d0
	bl 0x02008a68
	bl 0x02008328
.L_020003dc_3:
	ldr r0, [pc, #52]
	bl 0x020089c8
	cmp r0, #0
	beq .L_020003dc_0
	movs r0, #35
	movs r1, #0
	movs r2, #0
	bl 0x02008a10
	movs r0, #36
	movs r1, #0
	movs r2, #0
	bl 0x02008a10
.L_020003dc_0:
	movs r0, #0
	sub sp, #-8
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r1}
	bx r1
	.4byte 0x03001ebc
	.4byte 0x000008bc
	.4byte 0x02000240
	.4byte 0x000008ab
	.global Func_02000500
	.thumb_func
Func_02000500:
	push {r5, r6, lr}
	adds r6, r0, #0
	movs r0, #0
	bl 0x020089f8
	movs r2, #128
	ldrh r3, [r0, #6]
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, [pc, #40]
	ands r3, r2
	movs r2, #128
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_02000500_0
	movs r0, #28
	adds r1, r6, #0
	bl 0x02008a78
	b .L_02000500_1
.L_02000500_0:
	movs r0, #149
	lsls r0, r0, #4
	bl 0x020089c8
	cmp r0, #0
	beq .L_02000500_2
	ldr r0, [pc, #8]
	b .L_02000500_3
	.2byte 0x0000
	.4byte 0xffffc000
	.4byte 0x0000238d
.L_02000500_2:
	ldr r0, [pc, #84]
	bl 0x020089c8
	cmp r0, #0
	beq .L_02000500_4
	ldr r0, [pc, #80]
.L_02000500_3:
	bl 0x02008a38
	adds r0, r6, #0
	movs r1, #0
	bl 0x02008a48
	b .L_02000500_1
.L_02000500_4:
	ldr r5, [pc, #68]
	adds r0, r5, #0
	bl 0x02008a38
	movs r1, #0
	adds r0, r6, #0
	bl 0x02008a40
	movs r0, #0
	movs r1, #0
	bl 0x020089f0
	cmp r0, #0
	bne .L_02000500_5
	movs r0, #10
	bl 0x020089d8
	adds r0, r5, #1
	bl 0x02008a38
	b .L_02000500_6
.L_02000500_5:
	adds r0, r5, #2
	bl 0x02008a38
.L_02000500_6:
	adds r0, r6, #0
	movs r1, #0
	bl 0x02008a48
.L_02000500_1:
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x00000962
	.4byte 0x0000221b
	.4byte 0x00001fd5
	.global Func_020005a8
	.thumb_func
Func_020005a8:
	push {r5, r6, lr}
	adds r5, r0, #0
	movs r0, #0
	bl 0x020089f8
	movs r2, #128
	ldrh r3, [r0, #6]
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, [pc, #24]
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_020005a8_0
	movs r0, #26
	adds r1, r5, #0
	bl 0x02008a78
	b .L_020005a8_1
	.2byte 0x0000
	.4byte 0xffffc000
.L_020005a8_0:
	movs r0, #149
	lsls r0, r0, #4
	bl 0x020089c8
	cmp r0, #0
	beq .L_020005a8_2
	ldr r6, [pc, #116]
	adds r0, r6, #0
	bl 0x02008a38
	movs r1, #0
	adds r0, r5, #0
	bl 0x02008a40
	movs r0, #0
	movs r1, #0
	bl 0x020089f0
	cmp r0, #0
	bne .L_020005a8_3
	movs r0, #10
	bl 0x020089d8
	adds r0, r6, #1
	b .L_020005a8_4
.L_020005a8_3:
	adds r0, r6, #2
	bl 0x02008a38
	b .L_020005a8_5
.L_020005a8_2:
	ldr r0, [pc, #76]
	bl 0x020089c8
	cmp r0, #0
	beq .L_020005a8_6
	ldr r0, [pc, #68]
.L_020005a8_4:
	bl 0x02008a38
.L_020005a8_5:
	adds r0, r5, #0
	movs r1, #0
	bl 0x02008a48
	b .L_020005a8_1
.L_020005a8_6:
	ldr r0, [pc, #56]
	bl 0x02008a38
	adds r0, r5, #0
	movs r1, #0
	bl 0x02008a48
	movs r1, #131
	lsls r1, r1, #1
	adds r0, r5, #0
	movs r2, #0
	bl 0x02008a58
	movs r0, #40
	bl 0x020089d8
	adds r0, r5, #0
	movs r1, #0
	bl 0x02008a48
.L_020005a8_1:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00002389
	.4byte 0x00000962
	.4byte 0x00002219
	.4byte 0x00001fd2
	.global Func_0200066c
	.thumb_func
Func_0200066c:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #0
	bl 0x020089f8
	movs r2, #128
	ldrh r3, [r0, #6]
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, [pc, #40]
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_0200066c_0
	movs r0, #27
	adds r1, r5, #0
	bl 0x02008a78
	b .L_0200066c_1
.L_0200066c_0:
	movs r0, #149
	lsls r0, r0, #4
	bl 0x020089c8
	cmp r0, #0
	beq .L_0200066c_2
	ldr r0, [pc, #8]
	b .L_0200066c_3
	.2byte 0x0000
	.4byte 0xffffc000
	.4byte 0x0000238f
.L_0200066c_2:
	ldr r0, [pc, #44]
	bl 0x020089c8
	cmp r0, #0
	beq .L_0200066c_4
	ldr r0, [pc, #40]
.L_0200066c_3:
	bl 0x02008a38
	adds r0, r5, #0
	movs r1, #0
	bl 0x02008a48
	b .L_0200066c_1
.L_0200066c_4:
	ldr r0, [pc, #28]
	bl 0x02008a38
	adds r0, r5, #0
	movs r1, #0
	bl 0x02008a48
.L_0200066c_1:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000962
	.4byte 0x0000221d
	.4byte 0x00001fd9
	.global Func_020006ec
	.thumb_func
Func_020006ec:
	push {r5, r6, lr}
	adds r6, r0, #0
	bl 0x020089e0
	ldr r5, [pc, #64]
	adds r0, r5, #0
	bl 0x02008a38
	movs r1, #0
	adds r0, r6, #0
	bl 0x02008a40
	movs r0, #0
	movs r1, #0
	bl 0x020089f0
	cmp r0, #0
	bne .L_020006ec_0
	movs r0, #10
	bl 0x020089d8
	adds r0, r5, #1
	bl 0x02008a38
	b .L_020006ec_1
.L_020006ec_0:
	adds r0, r5, #2
	bl 0x02008a38
.L_020006ec_1:
	adds r0, r6, #0
	movs r1, #0
	bl 0x02008a48
	bl 0x020089e8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000239e
	.global Func_0200073c
	.thumb_func
Func_0200073c:
	push {r5, lr}
	adds r5, r0, #0
	bl 0x020089e0
	ldr r0, [pc, #20]
	bl 0x02008a38
	adds r0, r5, #0
	movs r1, #0
	bl 0x02008a48
	bl 0x020089e8
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x000023a1
	.global Func_02000760
	.thumb_func
Func_02000760:
	push {r5, r6, lr}
	adds r6, r0, #0
	bl 0x020089e0
	ldr r5, [pc, #64]
	adds r0, r5, #0
	bl 0x02008a38
	movs r1, #0
	adds r0, r6, #0
	bl 0x02008a40
	movs r0, #0
	movs r1, #0
	bl 0x020089f0
	cmp r0, #0
	bne .L_02000760_0
	movs r0, #10
	bl 0x020089d8
	adds r0, r5, #1
	bl 0x02008a38
	b .L_02000760_1
.L_02000760_0:
	adds r0, r5, #2
	bl 0x02008a38
.L_02000760_1:
	adds r0, r6, #0
	movs r1, #0
	bl 0x02008a48
	bl 0x020089e8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001fbb
	.global Func_020007b0
	.thumb_func
Func_020007b0:
	push {r5, r6, lr}
	adds r6, r0, #0
	bl 0x020089e0
	ldr r0, [pc, #140]
	bl 0x020089c8
	cmp r0, #0
	bne 0x020087fc
	ldr r5, [pc, #136]
	adds r0, r5, #0
	bl 0x02008a38
	movs r1, #0
	adds r0, r6, #0
	bl 0x02008a40
	movs r0, #0
	movs r1, #0
	bl 0x020089f0
	cmp r0, #0
	bne 0x020087ec
	movs r0, #10
	bl 0x020089d8
	adds r0, r5, #1
	bl 0x02008a38
.L_020007ea:
	b .L_020007ea_0
	.2byte 0x1ca8
	.2byte 0xf000
	.2byte 0xf923
.L_020007ea_0:
	adds r0, r6, #0
	movs r1, #0
	bl 0x02008a48
	b .L_020007ea_1
	.2byte 0x4814
	.2byte 0xf000
	.2byte 0xf8e3
	.2byte 0x2800
	.2byte 0xd113
	.2byte 0x4812
	.2byte 0xf000
	.2byte 0xf8e2
	.2byte 0x4811
	.2byte 0xf000
	.2byte 0xf913
	.2byte 0x2100
	.2byte 0x1c30
	.2byte 0xf000
	.2byte 0xf917
	.2byte 0x200a
	.2byte 0xf000
	.2byte 0xf8dc
	.2byte 0x1c30
	.2byte 0x2102
	.2byte 0xf000
	.2byte 0xf904
	.2byte 0x2014
	.2byte 0xf000
	.2byte 0xf8d5
	.2byte 0x480a
	.2byte 0xf000
	.2byte 0xf902
	.2byte 0x1c30
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xf906
.L_020007ea_1:
	bl 0x020089e8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x08bd
	.2byte 0x0000
	.2byte 0x2399
	.2byte 0x0000
	.2byte 0x08be
	.2byte 0x0000
	.2byte 0x239c
	.2byte 0x0000
	.2byte 0x239d
	.2byte 0x0000
	.global Func_0200085c
	.thumb_func
Func_0200085c:
	push {lr}
	bl 0x020089e0
	ldr r0, [pc, #40]
	bl 0x020089c8
	cmp r0, #0
	bne .L_0200085c_0
	ldr r0, [pc, #32]
	bl 0x02008a38
	b .L_0200085c_1
.L_0200085c_0:
	ldr r0, [pc, #28]
	bl 0x02008a38
.L_0200085c_1:
	movs r0, #25
	movs r1, #0
	bl 0x02008a48
	bl 0x020089e8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000008be
	.4byte 0x000023b3
	.4byte 0x000023b4
	.global Func_02000898
	.thumb_func
Func_02000898:
	push {r5, lr}
	adds r5, r0, #0
	bl 0x020089e0
	ldr r0, [pc, #32]
	bl 0x02008a38
	movs r2, #40
	movs r0, #31
	ldr r1, [pc, #28]
	bl 0x02008a58
	adds r0, r5, #0
	movs r1, #0
	bl 0x02008a48
	bl 0x020089e8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000023a8
	.4byte 0x00000103
	.global Func_020008cc
	.thumb_func
Func_020008cc:
	push {r5, r6, lr}
	adds r6, r0, #0
	bl 0x020089e0
	ldr r5, [pc, #64]
	adds r0, r5, #0
	bl 0x02008a38
	movs r1, #0
	adds r0, r6, #0
	bl 0x02008a40
	movs r0, #0
	movs r1, #0
	bl 0x020089f0
	cmp r0, #0
	bne .L_020008cc_0
	movs r0, #10
	bl 0x020089d8
	adds r0, r5, #1
	bl 0x02008a38
	b .L_020008cc_1
.L_020008cc_0:
	adds r0, r5, #2
	bl 0x02008a38
.L_020008cc_1:
	adds r0, r6, #0
	movs r1, #0
	bl 0x02008a48
	bl 0x020089e8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000023ac
	.global Func_0200091c
	.thumb_func
Func_0200091c:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #0
	bl 0x020089f8
	movs r2, #128
	ldrh r3, [r0, #6]
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, [pc, #36]
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_0200091c_0
	adds r0, r5, #0
	bl 0x02008a80
	b .L_0200091c_1
.L_0200091c_0:
	movs r0, #149
	lsls r0, r0, #4
	bl 0x020089c8
	cmp r0, #0
	beq .L_0200091c_2
	ldr r0, [pc, #4]
	b .L_0200091c_3
	.4byte 0xffffc000
	.4byte 0x000023bf
.L_0200091c_2:
	ldr r0, [pc, #44]
	bl 0x020089c8
	cmp r0, #0
	beq .L_0200091c_4
	ldr r0, [pc, #40]
.L_0200091c_3:
	bl 0x02008a38
	adds r0, r5, #0
	movs r1, #0
	bl 0x02008a48
	b .L_0200091c_1
.L_0200091c_4:
	ldr r0, [pc, #28]
	bl 0x02008a38
	adds r0, r5, #0
	movs r1, #0
	bl 0x02008a48
.L_0200091c_1:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000962
	.4byte 0x00002231
	.4byte 0x00001feb
	.include "games/THE BROKEN SEAL/SRC/FIELD/TOREBI_HEYA/IMPORT.INC"
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
	.4byte 0x00000028
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000a0
	.4byte 0xc00000d8
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00000100
	.4byte 0xffff0002
	.4byte 0x00000150
	.4byte 0xc00000c8
	.4byte 0x00f80000
	.4byte 0x01e80040
	.4byte 0x000000d8
	.4byte 0xffff0003
	.4byte 0x00000220
	.4byte 0xc00000c8
	.4byte 0x02080000
	.4byte 0x02b80030
	.4byte 0x000000d8
	.4byte 0xffff0004
	.4byte 0x00000318
	.4byte 0xc00000d8
	.4byte 0x02b00000
	.4byte 0x03c00000
	.4byte 0x000000f0
	.4byte 0xffff0005
	.4byte 0x00000220
	.4byte 0xc00001a8
	.4byte 0x01e80000
	.4byte 0x02b80108
	.4byte 0x000001c0
	.4byte 0xffff0006
	.4byte 0x00000138
	.4byte 0xc00001d8
	.4byte 0x00d00000
	.4byte 0x01c80110
	.4byte 0x000001e8
	.4byte 0xffff0008
	.4byte 0x00000338
	.4byte 0xc0000210
	.4byte 0x02b00000
	.4byte 0x03b000f8
	.4byte 0x00000238
	.4byte 0xffff0009
	.4byte 0x00000080
	.4byte 0xc0000300
	.4byte 0x00000000
	.4byte 0x01300218
	.4byte 0x00000320
	.4byte 0xffff000a
	.4byte 0x00000038
	.4byte 0xc0000260
	.4byte 0x00000000
	.4byte 0x01300218
	.4byte 0x00000320
	.4byte 0xffff000b
	.4byte 0x000001d0
	.4byte 0x40000270
	.4byte 0x01400000
	.4byte 0x02680238
	.4byte 0x000002e8
	.4byte 0xffff000c
	.4byte 0x00000188
	.4byte 0xc00002d8
	.4byte 0x01400000
	.4byte 0x02680238
	.4byte 0x000002e8
	.4byte 0xffff000d
	.4byte 0x00000218
	.4byte 0xc00002d8
	.4byte 0x01400000
	.4byte 0x02680238
	.4byte 0x000002e8
	.4byte 0xffff000e
	.4byte 0x00000188
	.4byte 0x400002a8
	.4byte 0x01400000
	.4byte 0x02680238
	.4byte 0x000002e8
	.4byte 0xffff000f
	.4byte 0x00000218
	.4byte 0x400002a8
	.4byte 0x01400000
	.4byte 0x02680238
	.4byte 0x000002e8
	.4byte 0xffff0010
	.4byte 0x00000328
	.4byte 0x40000298
	.4byte 0x02c00000
	.4byte 0x03b80260
	.4byte 0x00000300
	.4byte 0xffff0011
	.4byte 0x00000348
	.4byte 0x40000338
	.4byte 0x02c00000
	.4byte 0x03b80300
	.4byte 0x000003a0
	.4byte 0xffff0012
	.4byte 0x000001b8
	.4byte 0xc0000398
	.4byte 0x01200000
	.4byte 0x02100300
	.4byte 0x000003a8
	.4byte 0xffff0013
	.4byte 0x00000248
	.4byte 0xc0000398
	.4byte 0x01f00000
	.4byte 0x02e00300
	.4byte 0x000003a8
	.4byte 0xffff001e
	.4byte 0x00000338
	.4byte 0xc0000160
	.4byte 0x02b00000
	.4byte 0x03b000f8
	.4byte 0x00000238
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000088
	.4byte 0x00102087
	.4byte 0x00203087
	.4byte 0x00304087
	.4byte 0x00405087
	.4byte 0x00506087
	.4byte 0x00607087
	.4byte 0x00708087
	.4byte 0x00809087
	.4byte 0x0090a087
	.4byte 0x00a0b088
	.4byte 0x00b0a088
	.4byte 0x00c11088
	.4byte 0x00d10088
	.4byte 0x00e12088
	.4byte 0x00f13088
	.4byte 0x0100d088
	.4byte 0x0110c088
	.4byte 0x0120e088
	.4byte 0x0130f088
	.4byte 0x000001ff
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x0001d000
	.4byte 0xffff006a
	.4byte 0x00000002
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00013000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00013000
	.4byte 0xffff006d
	.4byte 0x00000002
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00015000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0001d000
	.4byte 0xffff0065
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00013000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x00c60000
	.4byte 0x00015000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x0001d000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x006a0000
	.4byte 0x00015000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x02600000
	.4byte 0x00000000
	.4byte 0x015c0000
	.4byte 0x00015000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00010000
	.4byte 0xffff007c
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff007d
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff0076
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0xffff0077
	.4byte 0x00000001
	.4byte 0x006a0000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00002000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x03400000
	.4byte 0x00015000
	.4byte 0xffff006c
	.4byte 0x00000002
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x02b00000
	.4byte 0x0001d000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x03500000
	.4byte 0x00013000
	.4byte 0xffff0084
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02500000
	.4byte 0x0001d000
	.4byte 0xffff0094
	.4byte 0x00000001
	.4byte 0x00e60000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00014000
	.4byte 0xffff0042
	.4byte 0x00000001
	.4byte 0x03600000
	.4byte 0x00000000
	.4byte 0x03500000
	.4byte 0x00015000
	.4byte 0xffff0043
	.4byte 0x00000001
	.4byte 0x03500000
	.4byte 0x00000000
	.4byte 0x03600000
	.4byte 0x0001d000
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x0001d000
	.4byte 0xffff006a
	.4byte 0x00000002
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00013000
	.4byte 0xffff0065
	.4byte 0x00000002
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00005000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0001d000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x0001d000
	.4byte 0xffff0065
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00013000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x00c60000
	.4byte 0x00015000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x0001d000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x006a0000
	.4byte 0x00015000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x02600000
	.4byte 0x00000000
	.4byte 0x015c0000
	.4byte 0x00015000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00010000
	.4byte 0xffff007c
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff007d
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff0076
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0xffff0077
	.4byte 0x00000001
	.4byte 0x006a0000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00002000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x0001c000
	.4byte 0xffff006c
	.4byte 0x00000002
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x02b00000
	.4byte 0x0001d000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x03500000
	.4byte 0x0001c000
	.4byte 0xffff0084
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02500000
	.4byte 0x0001d000
	.4byte 0xffff0094
	.4byte 0x02008a90
	.4byte 0x00e60000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x0002c000
	.4byte 0xffff0042
	.4byte 0x00000001
	.4byte 0x03600000
	.4byte 0x00000000
	.4byte 0x03500000
	.4byte 0x00015000
	.4byte 0xffff0043
	.4byte 0x00000001
	.4byte 0x03500000
	.4byte 0x00000000
	.4byte 0x03600000
	.4byte 0x0001d000
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00004000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x03500000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00008000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x03300000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x03500000
	.4byte 0x00000000
	.4byte 0x02b00000
	.4byte 0x00004000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x02a00000
	.4byte 0x00005000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x03600000
	.4byte 0x0001b000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x00013000
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
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x0000c602
	.4byte 0xffff000e
	.4byte 0x020080c1
	.4byte 0x0000c602
	.4byte 0xffff000f
	.4byte 0x020080c1
	.4byte 0x0000c602
	.4byte 0xffff0010
	.4byte 0x020080c1
	.4byte 0x0000c602
	.4byte 0xffff0011
	.4byte 0x020080c1
	.4byte 0x00000001
	.4byte 0xffff0012
	.4byte 0x00000012
	.4byte 0x00000001
	.4byte 0xffff0013
	.4byte 0x00000013
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001fba
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008761
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001fbe
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001fbf
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001fc4
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001fc6
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001fc7
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001fca
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001fcb
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001fce
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001fcf
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x020085a9
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x02008501
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x0200866d
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00001fdb
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00001fdc
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00001fdd
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x00001fde
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x00001fdf
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x00001fe0
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x00001fe1
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte 0x00001fe2
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001fc0
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001fc1
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001fc2
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001fc3
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001fc5
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001fc8
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001fc9
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001fcc
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001fcd
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001fd0
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001fd1
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001fd4
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001fd8
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001fda
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001fe3
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001fe4
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00001fe5
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x00001fe6
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x00001fe7
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte 0x00001fe8
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte 0x00001fe9
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte 0x00001fea
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte 0x0200891d
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte 0x00001fec
	.4byte 0x00000033
	.4byte 0x0f9a0064
	.4byte 0x001000b6
	.4byte 0x00000033
	.4byte 0x0f9b0065
	.4byte 0x001000e5
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x004029ad
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x004029ae
	.4byte 0x00000173
	.4byte 0xffff00ca
	.4byte 0x004029af
	.4byte 0x00000003
	.4byte 0xffff0028
	.4byte 0x00402075
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
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
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x0000c602
	.4byte 0xffff000e
	.4byte 0x020080c1
	.4byte 0x0000c602
	.4byte 0xffff000f
	.4byte 0x020080c1
	.4byte 0x0000c602
	.4byte 0xffff0010
	.4byte 0x020080c1
	.4byte 0x0000c602
	.4byte 0xffff0011
	.4byte 0x020080c1
	.4byte 0x00000001
	.4byte 0xffff0012
	.4byte 0x00000012
	.4byte 0x00000001
	.4byte 0xffff0013
	.4byte 0x00000013
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002201
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002202
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002203
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002204
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00002209
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000220b
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0000220c
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00002210
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002211
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00002215
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00002216
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x020085a9
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x02008501
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x0200866d
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x0000221f
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00002220
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00002221
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x00002224
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x00002225
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x00002226
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x00002227
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte 0x00002228
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002205
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002206
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002207
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002208
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000220a
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000220d
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000220e
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002213
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002214
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002217
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00002218
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x0000221a
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x0000221c
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x0000221e
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00002229
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x0000222a
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x0000222b
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x0000222c
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x0000222d
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte 0x0000222e
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte 0x0000222f
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte 0x00002230
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte 0x0200891d
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte 0x00002232
	.4byte 0x00000033
	.4byte 0x0f9a0064
	.4byte 0x001000b6
	.4byte 0x00000033
	.4byte 0x0f9b0065
	.4byte 0x001000e5
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x004029ad
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x004029ae
	.4byte 0x00000173
	.4byte 0xffff00ca
	.4byte 0x004029af
	.4byte 0x00000003
	.4byte 0xffff0028
	.4byte 0x00402075
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
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
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x0000c602
	.4byte 0xffff000e
	.4byte 0x020080c1
	.4byte 0x0000c602
	.4byte 0xffff000f
	.4byte 0x020080c1
	.4byte 0x0000c602
	.4byte 0xffff0010
	.4byte 0x020080c1
	.4byte 0x0000c602
	.4byte 0xffff0011
	.4byte 0x020080c1
	.4byte 0x00000001
	.4byte 0xffff0012
	.4byte 0x00000012
	.4byte 0x00000001
	.4byte 0xffff0013
	.4byte 0x00000013
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002375
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002376
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002379
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0000237a
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000237d
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0000237e
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00002381
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002382
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00002385
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00002386
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x020085a9
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x02008501
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x0200866d
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00002391
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00002392
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00002393
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x020087b1
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x020086ed
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x0200873d
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x000023a2
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte 0x000023a3
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002377
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002378
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x0000237b
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000237c
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000237f
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002380
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002383
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002384
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002387
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00002388
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x0000238c
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x0000238e
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00002390
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x000023b0
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x000023b1
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x000023b2
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x0200885d
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x000023b5
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte 0x000023b6
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte 0x000023b7
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte 0x000023b8
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte 0x0200891d
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte 0x000023c0
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte 0x02008899
	.4byte 0x00000000
	.4byte 0xffff0020
	.4byte 0x000023a9
	.4byte 0x00000000
	.4byte 0xffff0021
	.4byte 0x000023aa
	.4byte 0x00000000
	.4byte 0xffff0022
	.4byte 0x000023ab
	.4byte 0x00008d15
	.4byte 0xffff001f
	.4byte 0x000023b9
	.4byte 0x00008d15
	.4byte 0xffff0020
	.4byte 0x000023ba
	.4byte 0x00008d15
	.4byte 0xffff0021
	.4byte 0x000023bb
	.4byte 0x00008d15
	.4byte 0xffff0022
	.4byte 0x000023bc
	.4byte 0x00000000
	.4byte 0xffff0023
	.4byte 0x020088cd
	.4byte 0x00000000
	.4byte 0xffff0024
	.4byte 0x000023af
	.4byte 0x00008d15
	.4byte 0xffff0023
	.4byte 0x000023bd
	.4byte 0x00008d15
	.4byte 0xffff0024
	.4byte 0x000023be
	.4byte 0x00000033
	.4byte 0x0f9a0064
	.4byte 0x001000b6
	.4byte 0x00000033
	.4byte 0x0f9b0065
	.4byte 0x001000e5
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x004029ad
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x004029ae
	.4byte 0x00000173
	.4byte 0xffff00ca
	.4byte 0x004029af
	.4byte 0x00000003
	.4byte 0xffff0028
	.4byte 0x00402075
	.4byte 0x00000033
	.4byte 0x08bd0066
	.4byte 0x001000e5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x007b0002
	.4byte 0x00020001
	.4byte 0x00040006
	.4byte 0x0001007b
	.4byte 0x00060002
	.4byte 0x0000ffff
	.4byte 0x02009db4
	.4byte 0x00640018
	.4byte 0x02009db4
	.4byte 0x00640021
	.4byte 0x02009db4
	.4byte 0x00630032
	.4byte 0x02009db4
	.4byte 0x006d0034
