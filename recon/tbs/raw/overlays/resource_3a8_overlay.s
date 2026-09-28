.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/KAREI_MACHI/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	push {lr}
	movs r0, #12
	movs r1, #2
	movs r2, #3
	bl 0x0200bd5c
	pop {r0}
	bx r0
	.global Func_02000040
	.thumb_func
Func_02000040:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r5, #0
	adds r6, #102
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #0
	bne .L_02000040_0
	bl 0x0200bb2c
	lsls r0, r0, #3
	lsrs r0, r0, #16
	cmp r0, #1
	beq .L_02000040_1
	cmp r0, #1
	bcc .L_02000040_2
	cmp r0, #4
	bhi .L_02000040_3
	cmp r0, #3
	bcc .L_02000040_3
	b .L_02000040_4
.L_02000040_2:
	adds r0, r5, #0
	movs r1, #3
	bl 0x0200bb5c
	b .L_02000040_3
.L_02000040_1:
	adds r0, r5, #0
	movs r1, #4
	bl 0x0200bb5c
	b .L_02000040_3
.L_02000040_4:
	bl 0x0200bb2c
	ldrh r3, [r5, #6]
	lsls r0, r0, #15
	lsrs r0, r0, #16
	adds r3, r3, r0
	strh r3, [r5, #6]
.L_02000040_3:
	bl 0x0200bb2c
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #4
	lsrs r3, r3, #16
	strh r3, [r6]
	cmp r3, #0
	beq .L_02000040_5
.L_02000040_0:
	ldrh r3, [r6]
	subs r3, #1
	strh r3, [r6]
.L_02000040_5:
	movs r0, #1
	pop {r5, r6}
	pop {r1}
	bx r1
	.global Func_020000ac
	.thumb_func
Func_020000ac:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r5, #0
	adds r6, #102
	movs r1, #0
	ldrsh r3, [r6, r1]
	ldrh r2, [r6]
	cmp r3, #0
	bne .L_020000ac_0
	bl 0x0200bb2c
	ldrh r3, [r5, #6]
	lsls r0, r0, #15
	lsrs r0, r0, #16
	adds r3, r3, r0
	strh r3, [r5, #6]
	bl 0x0200bb2c
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #4
	lsrs r3, r3, #16
	strh r3, [r6]
	cmp r3, #0
	beq .L_020000ac_1
	adds r2, r3, #0
.L_020000ac_0:
	subs r3, r2, #1
	strh r3, [r6]
.L_020000ac_1:
	movs r0, #1
	pop {r5, r6}
	pop {r1}
	bx r1
	.global Func_020000ec
	.thumb_func
Func_020000ec:
	push {lr}
	ldr r3, [pc, #76]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #68]
	cmp r2, r3
	bne .L_020000ec_0
	ldr r0, [pc, #64]
	b .L_020000ec_1
.L_020000ec_0:
	ldr r3, [pc, #64]
	cmp r2, r3
	bne .L_020000ec_2
	ldr r0, [pc, #64]
	b .L_020000ec_1
.L_020000ec_2:
	ldr r3, [pc, #64]
	cmp r2, r3
	bne .L_020000ec_3
	ldr r0, [pc, #60]
	b .L_020000ec_1
.L_020000ec_3:
	ldr r3, [pc, #60]
	cmp r2, r3
	bne .L_020000ec_4
	ldr r0, [pc, #60]
	b .L_020000ec_1
.L_020000ec_4:
	ldr r3, [pc, #60]
	cmp r2, r3
	bne .L_020000ec_5
	ldr r0, [pc, #56]
	b .L_020000ec_1
.L_020000ec_5:
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_020000ec_6
	ldr r0, [pc, #56]
	b .L_020000ec_1
.L_020000ec_6:
	ldr r0, [pc, #56]
.L_020000ec_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000063
	.4byte 0x0200c2c8
	.4byte 0x00000066
	.4byte 0x0200c448
	.4byte 0x00000099
	.4byte 0x0200c4a8
	.4byte 0x0000009a
	.4byte 0x0200c520
	.4byte 0x0000009b
	.4byte 0x0200c580
	.4byte 0x0000009c
	.4byte 0x0200c628
	.4byte 0x0200c298
	.global Func_02000174
	.thumb_func
Func_02000174:
	movs r0, #0
	bx lr
	.global Func_02000178
	.thumb_func
Func_02000178:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200c6b8
	.global Func_02000180
	.thumb_func
Func_02000180:
	push {lr}
	ldr r3, [pc, #56]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #48]
	cmp r2, r3
	bne .L_02000180_0
	ldr r0, [pc, #44]
	b .L_02000180_1
.L_02000180_0:
	ldr r3, [pc, #44]
	cmp r2, r3
	bne .L_02000180_2
	ldr r0, [pc, #44]
	b .L_02000180_1
.L_02000180_2:
	ldr r3, [pc, #44]
	cmp r2, r3
	bne .L_02000180_3
	ldr r0, [pc, #40]
	b .L_02000180_1
.L_02000180_3:
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_02000180_4
	ldr r0, [pc, #40]
	b .L_02000180_1
.L_02000180_4:
	ldr r0, [pc, #40]
.L_02000180_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000063
	.4byte 0x0200c768
	.4byte 0x00000066
	.4byte 0x0200ca20
	.4byte 0x00000099
	.4byte 0x0200ca80
	.4byte 0x0000009c
	.4byte 0x0200cb58
	.4byte 0x0200c750
	.global Func_020001e4
	.thumb_func
Func_020001e4:
	push {lr}
	bl 0x0200bbf4
	ldr r0, [pc, #24]
	movs r1, #1
	bl 0x0200bbbc
	ldr r0, [pc, #20]
	movs r1, #1
	bl 0x0200bbbc
	bl 0x0200bbfc
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000947
	.4byte 0x000029df
	.global Func_0200020c
	.thumb_func
Func_0200020c:
	push {lr}
	bl 0x0200bbf4
	movs r2, #0
	movs r1, #0
	movs r0, #12
	bl 0x0200bc74
	ldr r0, [pc, #28]
	bl 0x0200bbdc
	movs r0, #181
	movs r1, #3
	bl 0x0200bd44
	movs r1, #0
	movs r0, #181
	bl 0x0200bc0c
	bl 0x0200bbfc
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000fd6
	.global Func_02000240
	.thumb_func
Func_02000240:
	push {lr}
	ldr r3, [pc, #76]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #68]
	cmp r2, r3
	bne .L_02000240_0
	ldr r0, [pc, #64]
	b .L_02000240_1
.L_02000240_0:
	ldr r3, [pc, #64]
	cmp r2, r3
	bne .L_02000240_2
	ldr r0, [pc, #64]
	b .L_02000240_1
.L_02000240_2:
	ldr r3, [pc, #64]
	cmp r2, r3
	bne .L_02000240_3
	ldr r0, [pc, #60]
	b .L_02000240_1
.L_02000240_3:
	ldr r3, [pc, #60]
	cmp r2, r3
	bne .L_02000240_4
	ldr r0, [pc, #60]
	b .L_02000240_1
.L_02000240_4:
	ldr r3, [pc, #60]
	cmp r2, r3
	bne .L_02000240_5
	ldr r0, [pc, #56]
	b .L_02000240_1
.L_02000240_5:
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_02000240_6
	ldr r0, [pc, #56]
	b .L_02000240_1
.L_02000240_6:
	ldr r0, [pc, #56]
.L_02000240_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000063
	.4byte 0x0200cbf4
	.4byte 0x00000066
	.4byte 0x0200ce88
	.4byte 0x00000099
	.4byte 0x0200cedc
	.4byte 0x0000009a
	.4byte 0x0200cf24
	.4byte 0x0000009b
	.4byte 0x0200cf54
	.4byte 0x0000009c
	.4byte 0x0200cf9c
	.4byte 0x0200cbe8
	.global Func_020002c8
	.thumb_func
Func_020002c8:
	push {lr}
	bl 0x0200bbf4
	ldr r0, [pc, #20]
	bl 0x0200bcbc
	movs r1, #0
	movs r0, #19
	bl 0x0200bcdc
	bl 0x0200bbfc
	pop {r0}
	bx r0
	.4byte 0x00001a7c
	.global Func_020002e8
	.thumb_func
Func_020002e8:
	push {lr}
	movs r0, #0
	bl 0x0200bc1c
	ldr r2, [pc, #116]
	ldrh r3, [r0, #6]
	adds r3, r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #23
	cmp r3, r2
	bls .L_020002e8_0
	bl 0x0200bbf4
	movs r1, #8
	movs r2, #0
	movs r0, #0
	bl 0x0200bca4
	movs r0, #10
	bl 0x0200bbec
	ldr r0, [pc, #84]
	bl 0x0200bcbc
	movs r1, #0
	movs r0, #8
	bl 0x0200bcc4
	movs r0, #0
	movs r1, #0
	bl 0x0200bc14
	cmp r0, #0
	bne .L_020002e8_1
	movs r0, #8
	movs r1, #4
	bl 0x0200bc84
	movs r0, #8
	movs r1, #0
	bl 0x0200bccc
	b .L_020002e8_2
.L_020002e8_1:
	ldr r3, [pc, #44]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	movs r0, #8
	movs r1, #3
	strh r3, [r2]
	bl 0x0200bc84
	movs r0, #8
	movs r1, #0
	bl 0x0200bccc
.L_020002e8_2:
	bl 0x0200bbfc
.L_020002e8_0:
	pop {r0}
	bx r0
	.4byte 0xfffff000
	.4byte 0x00002584
	.4byte 0x03001ebc
	.global Func_02000374
	.thumb_func
Func_02000374:
	push {lr}
	bl 0x0200bbf4
	ldr r0, [pc, #20]
	bl 0x0200bcbc
	movs r1, #0
	movs r0, #10
	bl 0x0200bcdc
	bl 0x0200bbfc
	pop {r0}
	bx r0
	.4byte 0x000025b3
	.global Func_02000394
	.thumb_func
Func_02000394:
	push {lr}
	movs r0, #0
	bl 0x0200bc1c
	ldr r2, [pc, #48]
	ldrh r3, [r0, #6]
	adds r3, r3, r2
	movs r2, #128
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bls .L_02000394_0
	movs r0, #22
	movs r1, #22
	bl 0x0200bd74
	b .L_02000394_1
.L_02000394_0:
	bl 0x0200bbf4
	ldr r0, [pc, #24]
	bl 0x0200bcbc
	movs r0, #22
	movs r1, #0
	bl 0x0200bccc
	bl 0x0200bbfc
.L_02000394_1:
	pop {r0}
	bx r0
	.4byte 0xffffe000
	.4byte 0x00001acf
	.global Func_020003d8
	.thumb_func
Func_020003d8:
	push {lr}
	movs r0, #0
	bl 0x0200bc1c
	ldr r2, [pc, #48]
	ldrh r3, [r0, #6]
	adds r3, r3, r2
	ldr r2, [pc, #48]
	lsls r3, r3, #16
	cmp r3, r2
	bhi .L_020003d8_0
	movs r0, #23
	movs r1, #23
	bl 0x0200bd74
	b .L_020003d8_1
.L_020003d8_0:
	bl 0x0200bbf4
	ldr r0, [pc, #28]
	bl 0x0200bcbc
	movs r1, #0
	movs r0, #23
	bl 0x0200bcdc
	bl 0x0200bbfc
.L_020003d8_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0xffff9fff
	.4byte 0x7ffe0000
	.4byte 0x00001ad1
	.global Func_02000420
	.thumb_func
Func_02000420:
	push {lr}
	movs r0, #0
	bl 0x0200bc1c
	ldr r2, [pc, #48]
	ldrh r3, [r0, #6]
	adds r3, r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bls .L_02000420_0
	movs r0, #24
	movs r1, #24
	bl 0x0200bd74
	b .L_02000420_1
.L_02000420_0:
	bl 0x0200bbf4
	ldr r0, [pc, #24]
	bl 0x0200bcbc
	movs r0, #24
	movs r1, #0
	bl 0x0200bccc
	bl 0x0200bbfc
.L_02000420_1:
	pop {r0}
	bx r0
	.4byte 0xffffe000
	.4byte 0x00001ad5
	.global Func_02000464
	.thumb_func
Func_02000464:
	push {r5, r6, r7, lr}
	ldr r3, [pc, #148]
	ldr r7, [r3]
	bl 0x0200bbf4
	movs r5, #8
	movs r6, #0
.L_02000464_1:
	adds r0, r5, #0
	bl 0x0200bc1c
	cmp r0, #0
	beq .L_02000464_0
	adds r3, r0, #0
	adds r3, #85
	strb r6, [r3]
.L_02000464_0:
	adds r5, #1
	cmp r5, #65
	bls .L_02000464_1
	movs r3, #182
	lsls r3, r3, #1
	adds r6, r7, r3
	movs r3, #0
	ldrsh r5, [r6, r3]
	movs r0, #158
	subs r5, #1
	bl 0x0200bd7c
	lsls r5, r5, #3
	ldr r0, [pc, #96]
	adds r3, r5, #4
	ldrh r1, [r0, r3]
	adds r3, r3, r0
	ldrh r2, [r3, #2]
	ldr r0, [r0, r5]
	bl 0x0200bb94
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #7
	lsls r1, r1, #8
	movs r0, #0
	bl 0x0200bc2c
	movs r0, #0
	bl 0x0200bc1c
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #2
	movs r0, #0
	bl 0x0200bc7c
	movs r2, #8
	movs r1, #2
	negs r2, r2
	movs r0, #0
	bl 0x0200bc64
	movs r0, #10
	bl 0x0200bbec
	movs r3, #0
	ldrsh r0, [r6, r3]
	bl 0x0200bd24
	bl 0x0200bd34
	bl 0x0200bd3c
	bl 0x0200bbfc
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x0200d0e0
	.global Func_02000504
	.thumb_func
Func_02000504:
	push {r5, lr}
	sub sp, #8
	bl 0x0200bbf4
	movs r0, #188
	bl 0x0200bd7c
	movs r5, #2
	movs r1, #23
	movs r2, #43
	movs r3, #12
	movs r0, #36
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200bb9c
	movs r0, #5
	bl 0x0200bb14
	movs r3, #12
	movs r1, #23
	movs r2, #43
	movs r0, #39
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200bb9c
	movs r0, #5
	bl 0x0200bb14
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #7
	lsls r1, r1, #8
	movs r0, #0
	bl 0x0200bc2c
	movs r0, #0
	bl 0x0200bc1c
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #2
	movs r0, #0
	bl 0x0200bc7c
	movs r2, #8
	movs r1, #0
	negs r2, r2
	movs r0, #0
	bl 0x0200bc6c
	movs r0, #10
	bl 0x0200bbec
	movs r0, #2
	bl 0x0200bd24
	bl 0x0200bd34
	bl 0x0200bd3c
	bl 0x0200bbfc
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000590
	.thumb_func
Func_02000590:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #260]
	ldr r7, [r3]
	bl 0x0200bbf4
	movs r0, #145
	lsls r0, r0, #4
	bl 0x0200bbd4
	cmp r0, #0
	bne .L_02000590_0
	bl 0x0200951c
.L_02000590_0:
	ldr r0, [pc, #240]
	bl 0x0200bbd4
	cmp r0, #0
	beq .L_02000590_1
	bl 0x0200951c
.L_02000590_1:
	ldr r0, [pc, #228]
	bl 0x0200bc04
	movs r1, #252
	movs r2, #136
	movs r0, #20
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl 0x0200bc74
	movs r1, #142
	movs r2, #132
	movs r0, #27
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200bc74
	movs r1, #142
	movs r2, #140
	movs r0, #28
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200bc74
	movs r1, #150
	movs r2, #132
	movs r0, #29
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200bc74
	movs r1, #150
	movs r2, #140
	movs r0, #30
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200bc74
	movs r1, #158
	movs r2, #132
	movs r0, #32
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200bc74
	movs r1, #158
	movs r2, #140
	movs r0, #31
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200bc74
	movs r1, #166
	movs r2, #132
	movs r0, #33
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200bc74
	movs r1, #166
	movs r2, #140
	movs r0, #34
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200bc74
	movs r1, #182
	movs r2, #136
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r0, #21
	bl 0x0200bc74
	movs r0, #17
	bl 0x0200bd7c
	movs r0, #20
	bl 0x0200bcec
	movs r2, #0
	movs r1, #1
	ldr r0, [pc, #72]
	bl 0x0200bbc4
	movs r0, #9
	bl 0x0200bd7c
	movs r0, #10
	bl 0x0200bbec
	movs r0, #0
	movs r1, #2
	bl 0x0200bc9c
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #9
	bne .L_02000590_2
	ldr r0, [pc, #36]
	ldr r1, [pc, #36]
	bl 0x0200bd0c
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200bce4
	b .L_02000590_3
	.4byte 0x03001ebc
	.4byte 0x00000911
	.4byte 0x0200c948
	.4byte 0x00001a91
	.4byte 0x00026666
	.4byte 0x00004ccc
.L_02000590_2:
	ldr r0, [pc, #1012]
	ldr r1, [pc, #1016]
	bl 0x0200bd0c
	movs r0, #0
	movs r1, #0
	movs r2, #20
	bl 0x0200bce4
.L_02000590_3:
	movs r0, #20
	ldr r1, [pc, #1000]
	ldr r2, [pc, #1004]
	bl 0x0200bc2c
	movs r1, #128
	movs r2, #128
	movs r0, #27
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bc2c
	movs r1, #128
	movs r2, #128
	movs r0, #28
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bc2c
	movs r0, #29
	ldr r1, [pc, #972]
	ldr r2, [pc, #972]
	bl 0x0200bc2c
	movs r0, #30
	ldr r1, [pc, #960]
	ldr r2, [pc, #964]
	bl 0x0200bc2c
	movs r0, #32
	ldr r1, [pc, #960]
	ldr r2, [pc, #960]
	bl 0x0200bc2c
	movs r0, #31
	ldr r1, [pc, #948]
	ldr r2, [pc, #952]
	bl 0x0200bc2c
	movs r0, #33
	ldr r1, [pc, #948]
	ldr r2, [pc, #948]
	bl 0x0200bc2c
	movs r0, #34
	ldr r1, [pc, #936]
	ldr r2, [pc, #940]
	bl 0x0200bc2c
	ldr r2, [pc, #936]
	movs r0, #21
	ldr r1, [pc, #936]
	bl 0x0200bc2c
	ldr r5, [pc, #936]
	movs r0, #20
	adds r1, r5, #0
	bl 0x0200bc34
	movs r0, #27
	adds r1, r5, #0
	bl 0x0200bc34
	movs r0, #28
	adds r1, r5, #0
	bl 0x0200bc34
	movs r0, #29
	adds r1, r5, #0
	bl 0x0200bc34
	movs r0, #30
	adds r1, r5, #0
	bl 0x0200bc34
	movs r0, #32
	adds r1, r5, #0
	bl 0x0200bc34
	movs r0, #31
	adds r1, r5, #0
	bl 0x0200bc34
	movs r0, #33
	adds r1, r5, #0
	bl 0x0200bc34
	adds r1, r5, #0
	movs r0, #34
	bl 0x0200bc34
	movs r0, #21
	bl 0x0200bc1c
	adds r6, r0, #0
	movs r3, #0
	adds r6, #100
	strh r3, [r6]
	movs r0, #21
	adds r1, r5, #0
	bl 0x0200bc34
	movs r0, #186
	movs r1, #1
	movs r2, #136
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #16
	movs r3, #1
	bl 0x0200bd14
	movs r0, #20
	bl 0x0200bc3c
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl 0x0200bce4
.L_02000590_4:
	movs r0, #1
	bl 0x0200bb14
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #0
	beq .L_02000590_4
	movs r0, #40
	bl 0x0200bbec
	movs r0, #27
	movs r1, #2
	bl 0x0200bc9c
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #20
	movs r0, #27
	bl 0x0200bce4
	ldr r0, [pc, #768]
	bl 0x0200bcbc
	movs r2, #10
	movs r0, #27
	movs r1, #0
	bl 0x0200bcd4
	movs r0, #28
	movs r1, #2
	bl 0x0200bc9c
	movs r1, #176
	movs r2, #10
	movs r0, #28
	lsls r1, r1, #8
	bl 0x0200bce4
	movs r0, #28
	movs r1, #3
	bl 0x0200bc7c
	movs r2, #10
	movs r0, #28
	movs r1, #0
	bl 0x0200bcd4
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #32
	bl 0x0200bd04
	movs r0, #40
	bl 0x0200bbec
	movs r0, #32
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r1, #128
	movs r0, #31
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200bcfc
	movs r1, #176
	movs r0, #31
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200bce4
	movs r0, #31
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r1, #128
	movs r2, #10
	movs r0, #31
	lsls r1, r1, #8
	bl 0x0200bce4
	movs r0, #31
	movs r1, #2
	bl 0x0200bc9c
	movs r0, #31
	movs r1, #4
	bl 0x0200bc7c
	movs r0, #31
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r1, #176
	movs r0, #31
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #160
	movs r2, #20
	movs r0, #32
	lsls r1, r1, #7
	bl 0x0200bce4
	movs r0, #31
	movs r1, #3
	bl 0x0200bc7c
	movs r0, #32
	movs r1, #3
	bl 0x0200bc84
	movs r0, #20
	movs r1, #2
	bl 0x0200bc9c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #20
	bl 0x0200bd04
	movs r0, #40
	bl 0x0200bbec
	movs r2, #10
	movs r0, #20
	movs r1, #0
	bl 0x0200bcd4
	movs r1, #2
	movs r0, #20
	bl 0x0200bc9c
	movs r0, #20
	bl 0x0200bc1c
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	movs r2, #0
	mov r8, r2
	movs r2, #132
	strb r3, [r0]
	movs r1, #172
	lsls r2, r2, #1
	movs r0, #20
	bl 0x0200bc5c
	movs r0, #1
	bl 0x0200bbec
	movs r0, #20
	bl 0x0200bc1c
	adds r0, #90
	ldrb r3, [r0]
	movs r6, #1
	orrs r3, r6
	movs r1, #128
	strb r3, [r0]
	lsls r1, r1, #8
	movs r0, #27
	movs r2, #0
	bl 0x0200bce4
	movs r1, #128
	movs r0, #28
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #128
	movs r0, #32
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #128
	movs r2, #20
	movs r0, #31
	lsls r1, r1, #8
	bl 0x0200bce4
	movs r1, #3
	movs r0, #20
	bl 0x0200bc84
	movs r0, #20
	bl 0x0200bbec
	movs r0, #20
	bl 0x0200bc1c
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #136
	ands r5, r3
	movs r1, #172
	lsls r2, r2, #1
	strb r5, [r0]
	movs r0, #20
	bl 0x0200bc5c
	movs r0, #1
	bl 0x0200bbec
	movs r0, #20
	bl 0x0200bc1c
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #136
	orrs r6, r3
	strb r6, [r0]
	movs r1, #180
	movs r0, #20
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl 0x0200bce4
	movs r0, #20
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r2, #0
	movs r0, #34
	ldr r1, [pc, #340]
	bl 0x0200bcfc
	movs r0, #34
	movs r1, #1
	bl 0x0200bc9c
	movs r0, #34
	movs r1, #3
	bl 0x0200bc84
	movs r2, #10
	movs r0, #34
	movs r1, #0
	bl 0x0200bcd4
	movs r0, #33
	movs r1, #1
	bl 0x0200bc9c
	movs r2, #10
	movs r0, #33
	movs r1, #0
	bl 0x0200bcd4
	movs r0, #33
	movs r1, #4
	bl 0x0200bc7c
	movs r2, #10
	movs r0, #33
	movs r1, #0
	bl 0x0200bcd4
	movs r0, #21
	movs r1, #2
	bl 0x0200bc9c
	movs r1, #129
	movs r0, #21
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200bcfc
	movs r0, #21
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r0, #20
	movs r1, #2
	movs r2, #20
	bl 0x0200bc8c
	movs r2, #40
	movs r0, #20
	movs r1, #4
	bl 0x0200bc8c
	movs r0, #20
	movs r1, #2
	bl 0x0200bc9c
	movs r0, #20
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r0, #21
	ldr r1, [pc, #204]
	ldr r2, [pc, #168]
	bl 0x0200bc2c
	movs r2, #141
	movs r0, #21
	ldr r1, [pc, #196]
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r2, #142
	movs r0, #21
	movs r1, #251
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r2, #148
	movs r0, #21
	movs r1, #246
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #21
	bl 0x0200bce4
	bl 0x02009e6c
	movs r0, #40
	bl 0x0200bbec
	movs r0, #21
	ldr r1, [pc, #136]
	ldr r2, [pc, #100]
	bl 0x0200bc2c
	movs r2, #148
	movs r0, #21
	movs r1, #228
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #192
	movs r0, #21
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200bce4
	movs r2, #148
	movs r0, #21
	movs r1, #212
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #192
	movs r0, #21
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200bce4
	movs r2, #148
	movs r0, #21
	movs r1, #192
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #192
	movs r2, #40
	movs r0, #21
	lsls r1, r1, #8
	b .L_02000590_5
	.2byte 0x0000
	.4byte 0x00013333
	.4byte 0x00002666
	.4byte 0x00011999
	.4byte 0x00008ccc
	.4byte 0x0000e666
	.4byte 0x00007333
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x0000b333
	.4byte 0x00005999
	.4byte 0x00004ccc
	.4byte 0x00009999
	.4byte 0x0200bdc4
	.4byte 0x00001a92
	.4byte 0x00000105
	.4byte 0x00019999
	.4byte 0x00000109
.L_02000590_5:
	bl 0x0200bce4
	movs r0, #21
	movs r1, #2
	bl 0x0200bc94
	movs r1, #128
	movs r0, #21
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200bcfc
	movs r1, #192
	movs r0, #20
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200bce4
	movs r2, #143
	movs r0, #21
	movs r1, #184
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #176
	movs r2, #10
	movs r0, #21
	lsls r1, r1, #8
	bl 0x0200bce4
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #21
	bl 0x0200bd04
	movs r0, #40
	bl 0x0200bbec
	movs r0, #21
	movs r1, #4
	bl 0x0200bc84
	movs r2, #40
	movs r0, #20
	ldr r1, [pc, #1008]
	bl 0x0200bcfc
	movs r0, #20
	movs r1, #3
	bl 0x0200bc84
	movs r0, #20
	movs r1, #2
	bl 0x0200bc9c
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl 0x0200bce4
	movs r0, #21
	movs r1, #0
	movs r2, #60
	bl 0x0200bce4
	movs r1, #192
	movs r0, #20
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200bce4
	movs r1, #176
	movs r2, #10
	movs r0, #21
	lsls r1, r1, #8
	bl 0x0200bce4
	movs r0, #20
	movs r1, #3
	bl 0x0200bc84
	movs r0, #21
	movs r1, #3
	bl 0x0200bc84
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl 0x0200bce4
	ldr r2, [pc, #920]
	movs r0, #20
	ldr r1, [pc, #920]
	bl 0x0200bc2c
	ldr r1, [pc, #920]
	movs r0, #20
	bl 0x0200bc4c
	movs r2, #148
	movs r0, #20
	movs r1, #228
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #192
	movs r0, #20
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200bce4
	movs r2, #148
	movs r0, #20
	movs r1, #212
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #192
	movs r0, #20
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200bce4
	movs r2, #148
	movs r0, #20
	movs r1, #192
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #192
	movs r0, #20
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200bce4
	movs r1, #176
	movs r0, #20
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #192
	movs r2, #10
	movs r0, #21
	lsls r1, r1, #6
	bl 0x0200bce4
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #20
	bl 0x0200bd04
	movs r0, #60
	bl 0x0200bbec
	movs r1, #4
	movs r0, #20
	bl 0x0200bc84
	ldr r5, [pc, #796]
	adds r0, r5, #0
	bl 0x0200bcbc
	movs r0, #20
	movs r1, #0
	movs r2, #40
	bl 0x0200bcd4
	bl 0x02009ed8
	movs r2, #136
	movs r0, #20
	movs r1, #178
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #0
	movs r2, #0
	movs r0, #20
	bl 0x0200bce4
	movs r0, #240
	bl 0x0200bbec
	movs r0, #27
	bl 0x0200bc44
	movs r0, #1
	bl 0x0200bb14
	movs r1, #128
	movs r0, #27
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200bce4
	movs r0, #27
	ldr r1, [pc, #704]
	movs r2, #60
	bl 0x0200bcfc
	movs r1, #0
	movs r2, #10
	movs r0, #27
	bl 0x0200bcd4
	movs r0, #27
	bl 0x02009ea4
	movs r0, #80
	bl 0x0200bbec
	movs r0, #28
	bl 0x0200bc44
	movs r0, #1
	bl 0x0200bb14
	movs r1, #208
	movs r2, #20
	movs r0, #28
	lsls r1, r1, #8
	bl 0x0200bce4
	movs r0, #28
	movs r1, #2
	bl 0x0200bc9c
	movs r1, #0
	movs r2, #10
	movs r0, #28
	bl 0x0200bcd4
	movs r0, #28
	bl 0x02009ea4
	movs r0, #160
	bl 0x0200bbec
	movs r0, #32
	bl 0x0200bc44
	movs r0, #1
	bl 0x0200bb14
	movs r1, #160
	movs r0, #32
	lsls r1, r1, #7
	movs r2, #10
	bl 0x0200bce4
	movs r0, #32
	ldr r1, [pc, #592]
	movs r2, #60
	bl 0x0200bcfc
	movs r1, #0
	movs r2, #10
	movs r0, #32
	bl 0x0200bcd4
	movs r0, #32
	bl 0x02009ea4
	movs r0, #80
	bl 0x0200bbec
	movs r0, #30
	bl 0x0200bc44
	movs r0, #1
	bl 0x0200bb14
	movs r1, #176
	movs r2, #10
	movs r0, #30
	lsls r1, r1, #8
	bl 0x0200bce4
	movs r1, #1
	movs r0, #30
	bl 0x0200bc9c
	adds r0, r5, #6
	bl 0x0200bcbc
	movs r1, #0
	movs r2, #10
	movs r0, #30
	bl 0x0200bcd4
	ldr r0, [pc, #536]
	bl 0x0200bb24
	movs r0, #20
	bl 0x0200bc44
	movs r0, #21
	bl 0x0200bc44
	movs r0, #1
	bl 0x0200bb14
	movs r0, #20
	bl 0x0200bc1c
	mov r3, r8
	adds r0, #100
	strh r3, [r0]
	movs r0, #21
	bl 0x0200bc1c
	mov r2, r8
	adds r0, #100
	strh r2, [r0]
	ldr r1, [pc, #472]
	movs r0, #20
	ldr r2, [pc, #488]
	bl 0x0200bc2c
	ldr r2, [pc, #480]
	movs r0, #21
	ldr r1, [pc, #456]
	bl 0x0200bc2c
	ldr r1, [pc, #476]
	movs r0, #20
	bl 0x0200bc34
	ldr r1, [pc, #472]
	movs r0, #21
	bl 0x0200bc34
	movs r0, #29
	bl 0x0200bc44
	movs r0, #1
	bl 0x0200bb14
	movs r1, #160
	movs r2, #10
	movs r0, #29
	lsls r1, r1, #7
	bl 0x0200bce4
	adds r5, #5
	movs r1, #2
	movs r0, #29
	bl 0x0200bc9c
	adds r0, r5, #0
	bl 0x0200bcbc
	movs r0, #29
	movs r1, #0
	movs r2, #20
	bl 0x0200bcd4
	movs r0, #29
	bl 0x02009ea4
	movs r0, #30
	bl 0x02009ea4
.L_02000590_6:
	movs r0, #1
	bl 0x0200bb14
	movs r0, #20
	bl 0x0200bc1c
	adds r0, #100
	movs r2, #0
	ldrsh r3, [r0, r2]
	cmp r3, #0
	beq .L_02000590_6
	movs r0, #21
	bl 0x0200bc1c
	adds r0, #100
	movs r2, #0
	ldrsh r3, [r0, r2]
	cmp r3, #1
	bne .L_02000590_6
	ldr r1, [pc, #368]
	movs r0, #20
	bl 0x0200bc34
	ldr r1, [pc, #364]
	movs r0, #21
	bl 0x0200bc34
	movs r0, #31
	bl 0x0200bc44
	movs r0, #1
	bl 0x0200bb14
	movs r1, #160
	movs r2, #10
	movs r0, #31
	lsls r1, r1, #7
	bl 0x0200bce4
	movs r0, #31
	movs r1, #1
	bl 0x0200bc9c
	movs r1, #4
	movs r0, #31
	bl 0x0200bc84
	ldr r5, [pc, #320]
	adds r0, r5, #0
	bl 0x0200bcbc
	movs r1, #0
	movs r2, #10
	movs r0, #31
	bl 0x0200bcd4
	movs r0, #31
	bl 0x02009ea4
	movs r0, #34
	bl 0x0200bc44
	movs r0, #33
	bl 0x0200bc44
	movs r0, #1
	bl 0x0200bb14
	movs r0, #34
	ldr r1, [pc, #280]
	movs r2, #40
	bl 0x0200bcfc
	movs r0, #33
	ldr r1, [pc, #268]
	movs r2, #60
	bl 0x0200bcfc
	movs r1, #176
	movs r0, #34
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200bce4
	movs r1, #160
	movs r2, #10
	movs r0, #33
	lsls r1, r1, #7
	bl 0x0200bce4
	adds r5, #3
	movs r1, #4
	movs r0, #34
	bl 0x0200bc84
	adds r0, r5, #0
	bl 0x0200bcbc
	movs r2, #10
	movs r0, #34
	movs r1, #0
	bl 0x0200bcd4
	movs r0, #33
	movs r1, #1
	bl 0x0200bc9c
	movs r0, #33
	movs r1, #4
	bl 0x0200bc7c
	movs r0, #33
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r1, #129
	movs r0, #34
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200bcfc
	movs r2, #0
	movs r0, #20
	ldr r1, [pc, #172]
	bl 0x0200bcfc
	movs r1, #2
	movs r0, #20
	bl 0x0200bc9c
	ldr r0, [pc, #164]
	bl 0x0200bcbc
	movs r1, #0
	movs r2, #10
	movs r0, #20
	bl 0x0200bcd4
	movs r0, #27
	bl 0x0200bc44
	movs r0, #28
	bl 0x0200bc44
	movs r0, #29
	bl 0x0200bc44
	movs r0, #30
	bl 0x0200bc44
	movs r0, #32
	bl 0x0200bc44
	movs r0, #31
	bl 0x0200bc44
	movs r0, #33
	bl 0x0200bc44
	movs r0, #34
	bl 0x0200bc44
	movs r0, #20
	bl 0x0200bc44
	movs r0, #21
	bl 0x0200bc44
	movs r0, #1
	bl 0x0200bb14
	movs r0, #27
	movs r1, #2
	movs r2, #0
	bl 0x0200bc8c
	movs r0, #28
	movs r1, #2
	movs r2, #0
	bl 0x0200bc8c
	movs r0, #29
	movs r1, #2
	movs r2, #0
	b .L_02000590_7
	.4byte 0x00000101
	.4byte 0x0000cccc
	.4byte 0x00019999
	.4byte 0x0200bfb0
	.4byte 0x00001a9e
	.4byte 0x02009f15
	.4byte 0x00006666
	.4byte 0x0200c034
	.4byte 0x0200c0cc
	.4byte 0x0200c164
	.4byte 0x0200c1ac
	.4byte 0x00001aa2
	.4byte 0x00000105
	.4byte 0x00000103
	.4byte 0x00001ab2
.L_02000590_7:
	bl 0x0200bc8c
	movs r0, #30
	movs r1, #2
	movs r2, #0
	bl 0x0200bc8c
	movs r0, #32
	movs r1, #2
	movs r2, #0
	bl 0x0200bc8c
	movs r0, #31
	movs r1, #2
	movs r2, #0
	bl 0x0200bc8c
	movs r0, #33
	movs r1, #2
	movs r2, #0
	bl 0x0200bc8c
	movs r0, #34
	movs r1, #2
	movs r2, #0
	bl 0x0200bc8c
	movs r0, #21
	movs r1, #2
	movs r2, #40
	bl 0x0200bc8c
	movs r1, #128
	movs r0, #27
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #128
	movs r0, #28
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #128
	movs r0, #29
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #128
	movs r0, #30
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #128
	movs r0, #32
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #128
	movs r0, #31
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #128
	movs r0, #33
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #128
	movs r0, #34
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200bce4
	movs r0, #21
	movs r1, #4
	movs r2, #40
	bl 0x0200bc8c
	movs r2, #10
	movs r0, #21
	movs r1, #0
	bl 0x0200bcd4
	movs r0, #20
	movs r1, #1
	bl 0x0200bc9c
	movs r2, #10
	movs r0, #20
	movs r1, #0
	bl 0x0200bcd4
	movs r0, #21
	movs r1, #3
	bl 0x0200bc84
	movs r2, #10
	movs r0, #21
	movs r1, #0
	bl 0x0200bcd4
	movs r0, #20
	movs r1, #3
	bl 0x0200bc84
	movs r0, #20
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r1, #129
	movs r2, #40
	movs r0, #27
	lsls r1, r1, #1
	bl 0x0200bcfc
	movs r0, #27
	movs r1, #1
	bl 0x0200bc94
	movs r0, #27
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r1, #129
	movs r0, #28
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200bcfc
	movs r2, #10
	movs r0, #28
	movs r1, #0
	bl 0x0200bcd4
	movs r1, #4
	movs r0, #21
	bl 0x0200bc84
	movs r0, #40
	bl 0x0200bbec
	movs r0, #21
	movs r1, #3
	bl 0x0200bc84
	movs r2, #20
	movs r0, #21
	movs r1, #0
	bl 0x0200bcd4
	movs r0, #20
	movs r1, #3
	bl 0x0200bc84
	movs r0, #20
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r1, #160
	movs r0, #27
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200bce4
	movs r1, #176
	movs r0, #28
	lsls r1, r1, #8
	movs r2, #4
	bl 0x0200bce4
	movs r1, #160
	movs r0, #29
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200bce4
	movs r1, #176
	movs r0, #30
	lsls r1, r1, #8
	movs r2, #4
	bl 0x0200bce4
	movs r1, #160
	movs r0, #32
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200bce4
	movs r1, #176
	movs r0, #31
	lsls r1, r1, #8
	movs r2, #4
	bl 0x0200bce4
	movs r1, #160
	movs r0, #33
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200bce4
	movs r1, #176
	movs r2, #4
	movs r0, #34
	lsls r1, r1, #8
	bl 0x0200bce4
	movs r0, #27
	movs r1, #3
	bl 0x0200bc7c
	movs r0, #28
	movs r1, #3
	bl 0x0200bc84
	movs r0, #29
	movs r1, #3
	bl 0x0200bc7c
	movs r0, #30
	movs r1, #3
	bl 0x0200bc84
	movs r0, #32
	movs r1, #3
	bl 0x0200bc7c
	movs r0, #31
	movs r1, #3
	bl 0x0200bc84
	movs r0, #33
	movs r1, #3
	bl 0x0200bc7c
	movs r0, #34
	movs r1, #3
	bl 0x0200bc84
	movs r0, #20
	movs r1, #2
	movs r2, #40
	bl 0x0200bc8c
	movs r0, #20
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r1, #128
	movs r0, #27
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #128
	movs r0, #28
	lsls r1, r1, #8
	movs r2, #4
	bl 0x0200bce4
	movs r1, #128
	movs r0, #29
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #128
	movs r0, #30
	lsls r1, r1, #8
	movs r2, #4
	bl 0x0200bce4
	movs r1, #128
	movs r0, #32
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #128
	movs r0, #31
	lsls r1, r1, #8
	movs r2, #4
	bl 0x0200bce4
	movs r1, #128
	movs r0, #33
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #128
	movs r0, #34
	lsls r1, r1, #8
	movs r2, #4
	bl 0x0200bce4
	movs r0, #20
	ldr r1, [pc, #872]
	ldr r2, [pc, #872]
	bl 0x0200bc2c
	movs r0, #27
	ldr r1, [pc, #868]
	ldr r2, [pc, #872]
	bl 0x0200bc2c
	movs r0, #28
	ldr r1, [pc, #860]
	ldr r2, [pc, #860]
	bl 0x0200bc2c
	movs r1, #128
	movs r2, #128
	movs r0, #29
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bc2c
	movs r1, #128
	movs r2, #128
	movs r0, #30
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bc2c
	movs r0, #32
	ldr r1, [pc, #828]
	ldr r2, [pc, #832]
	bl 0x0200bc2c
	movs r0, #31
	ldr r1, [pc, #820]
	ldr r2, [pc, #820]
	bl 0x0200bc2c
	movs r0, #33
	ldr r1, [pc, #816]
	ldr r2, [pc, #820]
	bl 0x0200bc2c
	movs r0, #34
	ldr r1, [pc, #808]
	ldr r2, [pc, #808]
	bl 0x0200bc2c
	ldr r2, [pc, #808]
	movs r0, #21
	ldr r1, [pc, #808]
	bl 0x0200bc2c
	movs r0, #27
	movs r1, #1
	bl 0x0200bcf4
	movs r0, #28
	movs r1, #1
	bl 0x0200bcf4
	movs r0, #29
	movs r1, #1
	bl 0x0200bcf4
	movs r0, #30
	movs r1, #1
	bl 0x0200bcf4
	movs r0, #32
	movs r1, #1
	bl 0x0200bcf4
	movs r0, #31
	movs r1, #1
	bl 0x0200bcf4
	movs r0, #33
	movs r1, #1
	bl 0x0200bcf4
	movs r0, #34
	movs r1, #1
	bl 0x0200bcf4
	movs r0, #20
	movs r1, #1
	bl 0x0200bcf4
	movs r1, #1
	movs r0, #21
	bl 0x0200bcf4
	movs r0, #27
	bl 0x0200bc44
	movs r0, #28
	bl 0x0200bc44
	movs r0, #29
	bl 0x0200bc44
	movs r0, #30
	bl 0x0200bc44
	movs r0, #32
	bl 0x0200bc44
	movs r0, #31
	bl 0x0200bc44
	movs r0, #33
	bl 0x0200bc44
	movs r0, #34
	bl 0x0200bc44
	movs r0, #20
	bl 0x0200bc44
	movs r0, #21
	bl 0x0200bc44
	movs r0, #1
	bl 0x0200bb14
	ldr r5, [pc, #660]
	movs r0, #20
	adds r1, r5, #0
	bl 0x0200bc34
	movs r0, #27
	adds r1, r5, #0
	bl 0x0200bc34
	movs r0, #28
	adds r1, r5, #0
	bl 0x0200bc34
	movs r0, #29
	adds r1, r5, #0
	bl 0x0200bc34
	movs r0, #30
	adds r1, r5, #0
	bl 0x0200bc34
	movs r0, #32
	adds r1, r5, #0
	bl 0x0200bc34
	movs r0, #31
	adds r1, r5, #0
	bl 0x0200bc34
	movs r0, #33
	adds r1, r5, #0
	bl 0x0200bc34
	adds r1, r5, #0
	movs r0, #34
	bl 0x0200bc34
	movs r0, #21
	bl 0x0200bc1c
	adds r2, r0, #0
	adds r2, #100
	movs r3, #0
	strh r3, [r2]
	movs r0, #21
	adds r1, r5, #0
	bl 0x0200bc34
.L_02000590_8:
	movs r0, #1
	bl 0x0200bb14
	movs r0, #21
	bl 0x0200bc1c
	adds r0, #100
	movs r2, #0
	ldrsh r3, [r0, r2]
	cmp r3, #1
	bne .L_02000590_8
	movs r0, #80
	bl 0x0200bbec
	movs r1, #170
	movs r2, #137
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r0, #14
	bl 0x0200bc74
	movs r0, #1
	bl 0x0200bb14
	movs r1, #128
	movs r2, #128
	movs r0, #14
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bc2c
	movs r2, #137
	movs r0, #14
	movs r1, #224
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r0, #14
	movs r1, #0
	movs r2, #40
	bl 0x0200bce4
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200bce4
	movs r1, #192
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200bce4
	movs r1, #160
	movs r0, #14
	lsls r1, r1, #7
	movs r2, #40
	bl 0x0200bce4
	movs r0, #14
	ldr r1, [pc, #444]
	movs r2, #60
	bl 0x0200bcfc
	movs r0, #14
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r0, #14
	movs r1, #0
	movs r2, #40
	bl 0x0200bce4
	movs r1, #192
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200bce4
	movs r1, #128
	movs r2, #40
	movs r0, #14
	lsls r1, r1, #8
	bl 0x0200bce4
	movs r1, #129
	movs r0, #14
	lsls r1, r1, #1
	bl 0x0200bd04
	movs r0, #14
	movs r1, #4
	movs r2, #40
	bl 0x0200bc8c
	movs r2, #20
	movs r0, #14
	movs r1, #0
	bl 0x0200bcd4
	movs r0, #14
	movs r1, #2
	bl 0x0200bc9c
	movs r0, #14
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r0, #14
	movs r1, #4
	movs r2, #40
	bl 0x0200bc8c
	ldr r1, [pc, #340]
	ldr r2, [pc, #340]
	movs r0, #14
	bl 0x0200bc2c
	movs r0, #14
	bl 0x0200bc1c
	adds r3, r0, #0
	adds r3, #100
	movs r2, #0
	strh r2, [r3]
	ldr r1, [pc, #308]
	movs r0, #14
	bl 0x0200bc34
.L_02000590_9:
	movs r0, #1
	bl 0x0200bb14
	movs r0, #14
	bl 0x0200bc1c
	adds r0, #100
	movs r2, #0
	ldrsh r3, [r0, r2]
	cmp r3, #1
	bne .L_02000590_9
	movs r2, #157
	ldr r1, [pc, #292]
	lsls r2, r2, #17
	movs r0, #14
	bl 0x0200bc74
	movs r0, #14
	bl 0x0200bc1c
	movs r5, #208
	lsls r5, r5, #8
	movs r2, #217
	ldr r1, [pc, #272]
	lsls r2, r2, #17
	strh r5, [r0, #6]
	movs r0, #20
	bl 0x0200bc74
	movs r0, #20
	bl 0x0200bc1c
	movs r1, #232
	movs r2, #208
	lsls r2, r2, #17
	lsls r1, r1, #17
	strh r5, [r0, #6]
	movs r0, #21
	bl 0x0200bc74
	movs r0, #21
	bl 0x0200bc1c
	movs r3, #160
	lsls r3, r3, #7
	strh r3, [r0, #6]
	movs r0, #27
	bl 0x0200bc24
	movs r0, #28
	bl 0x0200bc24
	movs r0, #29
	bl 0x0200bc24
	movs r0, #30
	bl 0x0200bc24
	movs r0, #31
	bl 0x0200bc24
	movs r0, #32
	bl 0x0200bc24
	movs r0, #33
	bl 0x0200bc24
	movs r0, #34
	bl 0x0200bc24
	movs r0, #17
	bl 0x0200bd7c
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #9
	bne .L_02000590_10
	movs r2, #229
	movs r0, #0
	movs r1, #224
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r0, #0
	bl 0x0200bc1c
	movs r3, #192
	lsls r3, r3, #8
	b .L_02000590_11
.L_02000590_10:
	movs r0, #0
	movs r1, #40
	movs r2, #248
	bl 0x0200bc5c
	movs r0, #0
	bl 0x0200bc1c
	movs r3, #128
	lsls r3, r3, #7
.L_02000590_11:
	strh r3, [r0, #6]
	bl 0x0200bd54
	ldr r0, [pc, #112]
	bl 0x0200bbdc
	b .L_02000590_12
	.2byte 0x207b
	.2byte 0xf002
	.2byte 0xfc2d
	.2byte 0x22b6
	.2byte 0x0052
	.2byte 0x18bb
	.2byte 0x2200
	.2byte 0x5e98
	.2byte 0xf002
	.2byte 0xfbfa
	.2byte 0xf002
	.2byte 0xfc00
	.2byte 0xf002
	.2byte 0xfc02
.L_02000590_12:
	bl 0x0200bbfc
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00011999
	.4byte 0x00008ccc
	.4byte 0x00010ccc
	.4byte 0x00008666
	.4byte 0x0000f333
	.4byte 0x00007999
	.4byte 0x0000e666
	.4byte 0x00007333
	.4byte 0x00006ccc
	.4byte 0x0000d999
	.4byte 0x0200be00
	.4byte 0x00000101
	.4byte 0x00013333
	.4byte 0x00009999
	.4byte 0x01670000
	.4byte 0x01c70000
	.4byte 0x00000911
	.global Func_0200158c
	.thumb_func
Func_0200158c:
	push {lr}
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200bbd4
	cmp r0, #0
	beq .L_0200158c_0
	ldr r3, [pc, #20]
	ldr r2, [r3]
	movs r0, #128
	movs r3, #0
	str r3, [r2, #24]
	lsls r0, r0, #2
	bl 0x0200bbe4
.L_0200158c_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ee0
	.global Func_020015b4
	.thumb_func
Func_020015b4:
	push {r5, lr}
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200bbd4
	cmp r0, #0
	bne .L_020015b4_0
	ldr r3, [pc, #24]
	movs r0, #0
	ldr r5, [r3]
	bl 0x0200bc1c
	str r0, [r5, #24]
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200bbdc
.L_020015b4_0:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001ee0
	.global Func_020015e0
	.thumb_func
Func_020015e0:
	push {lr}
	movs r0, #0
	bl 0x0200bc1c
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020015f8
	.thumb_func
Func_020015f8:
	push {lr}
	movs r0, #0
	bl 0x0200bc1c
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001610
	.thumb_func
Func_02001610:
	push {lr}
	ldr r0, [pc, #48]
	bl 0x0200bb24
	movs r0, #1
	bl 0x0200bb14
	movs r2, #0
	movs r1, #0
	movs r0, #26
	bl 0x0200bc74
	ldr r0, [pc, #28]
	bl 0x0200bbdc
	movs r0, #181
	movs r1, #3
	bl 0x0200bd44
	movs r1, #0
	movs r0, #181
	bl 0x0200bc0c
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200b90d
	.4byte 0x00000916
	.global Func_0200164c
	.thumb_func
Func_0200164c:
	push {lr}
	ldr r0, [pc, #80]
	bl 0x0200bbdc
	ldr r3, [pc, #76]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #68]
	cmp r2, r3
	bne .L_0200164c_0
	bl 0x020096bc
	b .L_0200164c_1
.L_0200164c_0:
	ldr r3, [pc, #60]
	cmp r2, r3
	bne .L_0200164c_2
	bl 0x020097e8
	b .L_0200164c_1
.L_0200164c_2:
	ldr r3, [pc, #52]
	cmp r2, r3
	bne .L_0200164c_3
	bl 0x02009858
	b .L_0200164c_1
.L_0200164c_3:
	ldr r3, [pc, #44]
	cmp r2, r3
	bne .L_0200164c_4
	bl 0x020098a4
	b .L_0200164c_1
.L_0200164c_4:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_0200164c_1
	bl 0x02009930
.L_0200164c_1:
	movs r0, #0
	pop {r1}
	bx r1
	.4byte 0x0000087a
	.4byte 0x02000240
	.4byte 0x00000063
	.4byte 0x00000066
	.4byte 0x00000099
	.4byte 0x0000009b
	.4byte 0x0000009c
	.global Func_020016bc
	.thumb_func
Func_020016bc:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r0, [pc, #260]
	bl 0x0200bbd4
	cmp r0, #0
	beq .L_020016bc_0
	ldr r0, [pc, #252]
	bl 0x0200bbdc
	ldr r0, [pc, #252]
	bl 0x0200bbdc
	ldr r0, [pc, #248]
	bl 0x0200bbdc
	ldr r0, [pc, #248]
	bl 0x0200bbdc
.L_020016bc_0:
	movs r0, #148
	lsls r0, r0, #4
	bl 0x0200bbd4
	cmp r0, #0
	beq .L_020016bc_1
	ldr r0, [pc, #216]
	bl 0x0200bbdc
.L_020016bc_1:
	ldr r3, [pc, #228]
	movs r1, #225
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #14
	bne .L_020016bc_2
	movs r1, #212
	movs r2, #176
	movs r0, #25
	lsls r1, r1, #17
	lsls r2, r2, #15
	bl 0x0200bc74
.L_020016bc_2:
	movs r0, #21
	movs r1, #2
	bl 0x0200bcac
	ldr r0, [pc, #192]
	bl 0x0200bbd4
	mov r8, r0
	cmp r0, #0
	beq .L_020016bc_3
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl 0x0200bc74
	b .L_020016bc_4
.L_020016bc_3:
	movs r0, #26
	bl 0x0200bc1c
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
	adds r3, r6, #0
	mov r1, r8
	adds r3, #39
	strb r2, [r6, #9]
	strb r1, [r3]
	adds r3, r7, #0
	movs r2, #1
	adds r3, #92
	strb r2, [r3]
	subs r3, #7
	strb r1, [r3]
	movs r3, #160
	lsls r3, r3, #12
	str r3, [r7, #12]
	adds r3, r7, #0
	adds r3, #97
	movs r1, #193
	strb r2, [r3]
	lsls r1, r1, #3
	movs r0, #17
	bl 0x0200bb44
	adds r5, r0, #0
	movs r0, #181
	bl 0x0200bbcc
	movs r2, #128
	lsls r2, r2, #3
	adds r5, r5, r2
	movs r1, #128
	adds r2, r5, #0
	ldrb r0, [r6, #28]
	bl 0x0200bb54
	movs r0, #17
	bl 0x0200bb4c
	mov r3, r8
	str r3, [r7, #48]
	ldr r3, [r7, #8]
	str r3, [r7, #56]
	ldr r3, [r7, #12]
	str r3, [r7, #60]
	ldr r3, [r7, #16]
	movs r1, #200
	str r3, [r7, #64]
	ldr r0, [pc, #44]
	lsls r1, r1, #4
	bl 0x0200bb1c
.L_020016bc_4:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000941
	.4byte 0x00000321
	.4byte 0x00000913
	.4byte 0x00000912
	.4byte 0x00000915
	.4byte 0x02000240
	.4byte 0x00000916
	.4byte 0x0200b90d
	.global Func_020017e8
	.thumb_func
Func_020017e8:
	push {lr}
	ldr r0, [pc, #84]
	bl 0x0200bbd4
	cmp r0, #0
	beq .L_020017e8_0
	ldr r0, [pc, #76]
	bl 0x0200bbdc
	ldr r0, [pc, #76]
	bl 0x0200bbdc
	ldr r0, [pc, #72]
	bl 0x0200bbdc
	ldr r0, [pc, #72]
	bl 0x0200bbdc
.L_020017e8_0:
	movs r0, #148
	lsls r0, r0, #4
	bl 0x0200bbd4
	cmp r0, #0
	beq .L_020017e8_1
	ldr r0, [pc, #40]
	bl 0x0200bbdc
.L_020017e8_1:
	ldr r3, [pc, #52]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_020017e8_2
	ldr r0, [pc, #28]
	bl 0x0200bbd4
	cmp r0, #0
	bne .L_020017e8_2
	bl 0x0200a008
.L_020017e8_2:
	pop {r0}
	bx r0
	.4byte 0x00000941
	.4byte 0x00000321
	.4byte 0x00000913
	.4byte 0x00000912
	.4byte 0x00000915
	.4byte 0x02000240
	.global Func_02001858
	.thumb_func
Func_02001858:
	push {lr}
	ldr r0, [pc, #60]
	bl 0x0200bbd4
	cmp r0, #0
	bne .L_02001858_0
	movs r0, #12
	bl 0x0200ba3c
.L_02001858_0:
	ldr r0, [pc, #48]
	bl 0x0200bbd4
	cmp r0, #0
	beq .L_02001858_1
	movs r0, #8
	bl 0x0200bc1c
	movs r3, #0
	strh r3, [r0, #6]
.L_02001858_1:
	ldr r3, [pc, #32]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #10
	bne .L_02001858_2
	bl 0x0200a6c0
.L_02001858_2:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000fd6
	.4byte 0x00000915
	.4byte 0x02000240
	.global Func_020018a4
	.thumb_func
Func_020018a4:
	push {r5, lr}
	ldr r3, [pc, #124]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #68
	str r2, [r3]
	ldr r0, [pc, #112]
	sub sp, #8
	bl 0x0200bbd4
	cmp r0, #0
	beq .L_020018a4_0
	movs r3, #3
	str r3, [sp, #4]
	movs r5, #2
	movs r0, #58
	movs r1, #5
	movs r2, #58
	movs r3, #8
	str r5, [sp, #0]
	bl 0x0200bb9c
	movs r3, #8
	movs r2, #10
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #8
	movs r1, #11
	movs r2, #2
	movs r3, #1
	bl 0x0200bba4
	movs r3, #1
	str r3, [sp, #4]
	movs r0, #8
	movs r1, #12
	movs r2, #8
	movs r3, #11
	str r5, [sp, #0]
	bl 0x0200bb9c
	bl 0x0200bb7c
	movs r0, #1
	bl 0x0200bb14
.L_020018a4_0:
	ldr r3, [pc, #36]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	bgt .L_020018a4_1
	movs r0, #170
	bl 0x0200bd64
.L_020018a4_1:
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00000915
	.4byte 0x02000240
	.global Func_02001930
	.thumb_func
Func_02001930:
	push {r5, r6, lr}
	ldr r6, [pc, #852]
	movs r2, #224
	ldr r3, [r6]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #68
	str r2, [r3]
	movs r0, #0
	sub sp, #8
	bl 0x0200bd4c
	ldr r0, [pc, #832]
	bl 0x0200bbd4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02001930_0
	movs r0, #128
	lsls r0, r0, #2
	ldr r5, [r6, #36]
	bl 0x0200bbd4
	cmp r0, #0
	beq .L_02001930_1
	movs r0, #0
	bl 0x0200bc1c
.L_02001930_1:
	str r0, [r5, #24]
	b .L_02001930_2
.L_02001930_0:
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200bbdc
	ldr r3, [pc, #792]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #4
	bne .L_02001930_2
	ldr r3, [r6, #36]
	movs r0, #128
	str r5, [r3, #24]
	lsls r0, r0, #2
	bl 0x0200bbe4
.L_02001930_2:
	ldr r0, [pc, #768]
	bl 0x0200bbd4
	cmp r0, #0
	beq .L_02001930_3
	movs r1, #150
	movs r2, #182
	movs r0, #11
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x0200bc74
	ldr r0, [pc, #748]
	bl 0x0200bbd4
	cmp r0, #0
	beq .L_02001930_3
	movs r0, #11
	bl 0x0200bc1c
	movs r0, #11
	movs r1, #5
	bl 0x0200bc7c
	movs r3, #14
	str r3, [sp, #4]
	movs r5, #9
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200bba4
	movs r3, #45
	str r3, [sp, #4]
	movs r2, #1
	movs r3, #1
	movs r0, #0
	movs r1, #0
	str r5, [sp, #0]
	bl 0x0200bba4
	movs r0, #11
	bl 0x0200bc1c
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
.L_02001930_3:
	movs r0, #8
	bl 0x0200bc1c
	movs r1, #0
	bl 0x0200bbb4
	movs r0, #9
	bl 0x0200bc1c
	movs r1, #0
	bl 0x0200bbb4
	movs r1, #200
	ldr r0, [pc, #648]
	lsls r1, r1, #4
	bl 0x0200bb1c
	ldr r0, [pc, #644]
	bl 0x0200bbd4
	cmp r0, #0
	beq .L_02001930_4
	movs r1, #213
	lsls r1, r1, #17
	ldr r2, [pc, #636]
	movs r0, #10
	bl 0x0200bc74
	movs r0, #10
	bl 0x0200bc1c
	movs r3, #160
	lsls r3, r3, #7
	strh r3, [r0, #6]
	movs r3, #3
	str r3, [sp, #4]
	movs r6, #2
	movs r0, #88
	movs r1, #48
	movs r2, #88
	movs r3, #45
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r5, #1
	movs r0, #24
	movs r1, #49
	movs r2, #24
	movs r3, #48
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200bb9c
	movs r0, #25
	movs r1, #42
	movs r2, #25
	movs r3, #47
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200bb9c
	movs r3, #24
	movs r2, #49
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #22
	movs r1, #50
	movs r2, #2
	movs r3, #1
	bl 0x0200bba4
.L_02001930_4:
	ldr r0, [pc, #524]
	bl 0x0200bbd4
	cmp r0, #0
	bne .L_02001930_5
	b 0x02009ca8
.L_02001930_5:
	movs r1, #232
	movs r2, #183
	movs r0, #8
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x0200bc74
	movs r3, #0
	str r3, [sp, #0]
	movs r6, #1
	movs r0, #7
	movs r1, #44
	movs r2, #1
	movs r3, #1
	str r6, [sp, #4]
	bl 0x0200bba4
	movs r3, #5
	str r3, [sp, #4]
	movs r0, #74
	movs r1, #58
	movs r2, #78
	movs r3, #41
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r3, #3
	str r3, [sp, #0]
.L_02001ac8:
	movs r5, #2
	movs r0, #16
	movs r1, #109
	movs r2, #13
	movs r3, #109
	str r5, [sp, #4]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #64
	movs r2, #71
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #64
	movs r2, #72
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #68
	movs r2, #73
	movs r3, #43
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #68
	movs r2, #74
	movs r3, #43
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #64
	movs r2, #75
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #66
	movs r2, #76
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #64
	movs r2, #77
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #64
	movs r2, #78
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #64
	movs r2, #79
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #66
	movs r2, #80
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #2
	movs r1, #0
	movs r2, #9
	movs r3, #42
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #64
	movs r2, #71
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #64
	movs r2, #72
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #68
	movs r2, #73
	movs r3, #43
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #68
	movs r2, #74
	movs r3, #43
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #64
	movs r2, #75
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #66
	movs r2, #76
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #64
	movs r2, #77
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #64
	movs r2, #78
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #64
	movs r2, #79
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #66
	movs r2, #80
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #4
	movs r1, #0
	movs r2, #9
	movs r3, #42
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200bb9c
	movs r3, #8
	str r3, [sp, #4]
	movs r5, #10
	movs r0, #7
	movs r1, #11
	movs r2, #7
	movs r3, #42
	str r5, [sp, #0]
	bl 0x0200bb9c
	movs r3, #13
	str r3, [sp, #4]
	movs r0, #71
	movs r1, #12
	movs r2, #71
	movs r3, #43
	str r5, [sp, #0]
	bl 0x0200bb9c
	movs r3, #6
	movs r5, #44
	str r3, [sp, #0]
	movs r0, #6
	movs r1, #13
	movs r2, #12
	movs r3, #12
	str r5, [sp, #4]
	bl 0x0200bba4
	movs r3, #7
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #1
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200bba4
	b .L_02001ac8_0
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x0109
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	.2byte 0x0302
	.2byte 0x0000
	.2byte 0x0201
	.2byte 0x0000
	.2byte 0xb769
	.2byte 0x0200
	.2byte 0x0915
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x02da
	.2byte 0x4b6e
	.2byte 0x22e1
	.2byte 0x0052
	.2byte 0x189b
	.2byte 0x2200
	.2byte 0x5e9b
	.2byte 0x2b02
	.2byte 0xdc04
	.2byte 0x2b01
	.2byte 0xdb02
	.2byte 0x20aa
	.2byte 0xf002
	.2byte 0xf851
.L_02001ac8_0:
	ldr r0, [pc, #420]
	bl 0x0200bbd4
	cmp r0, #0
	bne .L_02001ac8_1
	b .L_02001ac8_2
.L_02001ac8_1:
	movs r1, #174
	movs r2, #183
	movs r0, #9
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl 0x0200bc74
	movs r3, #5
	str r3, [sp, #4]
	movs r6, #1
	movs r0, #74
	movs r1, #58
	movs r2, #107
	movs r3, #41
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r3, #3
	str r3, [sp, #0]
	movs r5, #2
	movs r0, #45
	movs r1, #109
	movs r2, #42
	movs r3, #109
	str r5, [sp, #4]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #64
	movs r2, #102
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #64
	movs r2, #103
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #64
	movs r2, #104
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #66
	movs r2, #105
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #64
	movs r2, #106
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #64
	movs r2, #107
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #64
	movs r2, #108
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #66
	movs r2, #109
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #64
	movs r2, #102
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #64
	movs r2, #103
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #64
	movs r2, #104
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #66
	movs r2, #105
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #64
	movs r2, #106
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #64
	movs r2, #107
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #64
	movs r2, #108
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #66
	movs r2, #109
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r3, #4
	str r3, [sp, #4]
	movs r5, #8
	movs r0, #38
	movs r1, #14
	movs r2, #38
	movs r3, #44
	str r5, [sp, #0]
	bl 0x0200bb9c
	movs r3, #12
	str r3, [sp, #4]
	movs r0, #102
	movs r1, #14
	movs r2, #102
	movs r3, #44
	str r5, [sp, #0]
	bl 0x0200bb9c
	movs r3, #37
	movs r2, #43
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #37
	movs r1, #13
	movs r2, #10
	movs r3, #12
	bl 0x0200bba4
	b .L_02001ac8_3
.L_02001ac8_2:
	ldr r3, [pc, #32]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #4
	bgt .L_02001ac8_3
	cmp r3, #3
	blt .L_02001ac8_3
	movs r0, #170
	bl 0x0200bd64
.L_02001ac8_3:
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000303
	.global Func_02001e6c
	.thumb_func
Func_02001e6c:
	push {lr}
	movs r0, #27
	bl 0x02009ea4
	movs r0, #28
	bl 0x02009ea4
	movs r0, #29
	bl 0x02009ea4
	movs r0, #30
	bl 0x02009ea4
	movs r0, #32
	bl 0x02009ea4
	movs r0, #31
	bl 0x02009ea4
	movs r0, #33
	bl 0x02009ea4
	movs r0, #34
	bl 0x02009ea4
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001ea4
	.thumb_func
Func_02001ea4:
	push {r5, r6, lr}
	adds r6, r0, #0
	bl 0x0200bc1c
	adds r5, r0, #0
	adds r3, r5, #0
	adds r3, #100
	strh r6, [r3]
	bl 0x0200bb2c
	lsls r3, r0, #2
	adds r3, r3, r0
	adds r2, r5, #0
	lsrs r3, r3, #12
	adds r2, #102
	strh r3, [r2]
	ldr r1, [pc, #12]
	adds r0, r5, #0
	bl 0x0200bb64
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200beac
	.global Func_02001ed8
	.thumb_func
Func_02001ed8:
	push {lr}
	ldr r3, [pc, #40]
	movs r2, #0
	str r2, [r3]
	movs r0, #20
	ldr r1, [pc, #36]
	ldr r2, [pc, #36]
	bl 0x0200bc2c
	movs r0, #21
	ldr r1, [pc, #24]
	ldr r2, [pc, #28]
	bl 0x0200bc2c
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #20]
	bl 0x0200bb1c
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200d144
	.4byte 0x00019999
	.4byte 0x0000cccc
	.4byte 0x02009f15
	.global Func_02001f14
	.thumb_func
Func_02001f14:
	push {lr}
	ldr r3, [pc, #224]
	ldr r3, [r3]
	cmp r3, #4
	bhi .L_02001f14_0
	ldr r2, [pc, #220]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	movs r0, r0
	ldr r7, [sp, #240]
	lsls r0, r0, #8
	ldr r7, [sp, #352]
	lsls r0, r0, #8
	ldr r7, [sp, #528]
	lsls r0, r0, #8
	ldr r7, [sp, #704]
	lsls r0, r0, #8
	ldr r7, [sp, #880]
	lsls r0, r0, #8
	movs r0, #21
	bl 0x0200bc1c
	movs r3, #0
	adds r0, #100
	strh r3, [r0]
	ldr r1, [pc, #180]
	movs r0, #21
	bl 0x0200bc34
	ldr r2, [pc, #164]
	ldr r3, [r2]
	adds r3, #1
	b .L_02001f14_1
	.2byte 0x2015
	.2byte 0xf001
	.2byte 0xfe5f
	.2byte 0x3064
	.2byte 0x2200
	.2byte 0x5e83
	.2byte 0x2b00
	.2byte 0xd044
	.2byte 0x2014
	.2byte 0xf001
	.2byte 0xfe57
	.2byte 0x2300
	.2byte 0x3064
	.2byte 0x8003
	.2byte 0x4923
	.2byte 0x2014
	.2byte 0xf001
	.2byte 0xfe5c
	.2byte 0x4a1e
	.2byte 0x6813
	.2byte 0x3301
	.2byte 0xe035
	.2byte 0x2014
	.2byte 0xf001
	.2byte 0xfe49
	.2byte 0x3064
	.2byte 0x2200
	.2byte 0x5e83
	.2byte 0x2b00
	.2byte 0xd02e
	.2byte 0x2014
	.2byte 0xf001
	.2byte 0xfe41
	.2byte 0x2300
	.2byte 0x3064
	.2byte 0x8003
	.2byte 0x4917
	.2byte 0x2014
	.2byte 0xf001
	.2byte 0xfe46
	.2byte 0x4a13
	.2byte 0x6813
	.2byte 0x3301
	.2byte 0xe01f
	.2byte 0x2014
	.2byte 0xf001
	.2byte 0xfe33
	.2byte 0x3064
	.2byte 0x2200
	.2byte 0x5e83
	.2byte 0x2b00
	.2byte 0xd018
	.2byte 0x2015
	.2byte 0xf001
	.2byte 0xfe2b
	.2byte 0x2300
	.2byte 0x3064
	.2byte 0x8003
	.2byte 0x490d
	.2byte 0x2015
	.2byte 0xf001
	.2byte 0xfe30
	.2byte 0x4a08
	.2byte 0x6813
	.2byte 0x3301
	.2byte 0xe009
	.2byte 0x2015
	.2byte 0xf001
	.2byte 0xfe1d
	.2byte 0x3064
	.2byte 0x2200
	.2byte 0x5e83
	.2byte 0x2b00
	.2byte 0xd002
	.2byte 0x4a02
	.2byte 0x2300
.L_02001f14_1:
	str r3, [r2]
.L_02001f14_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200d144
	.4byte 0x02009f28
	.4byte 0x0200bec0
	.2byte 0xbfb0
	.2byte 0x0200
	.global Func_02002008
	.thumb_func
Func_02002008:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #8
	bl 0x0200bbf4
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r1, r1
	negs r2, r2
	negs r0, r0
	bl 0x0200bd14
	movs r0, #1
	bl 0x0200bb14
	movs r1, #192
	movs r2, #171
	lsls r2, r2, #17
	lsls r1, r1, #16
	movs r0, #0
	bl 0x0200bc74
	movs r0, #1
	bl 0x0200bb14
	ldr r0, [pc, #856]
	ldr r1, [pc, #860]
	bl 0x0200bd0c
	movs r0, #192
	movs r1, #1
	movs r2, #252
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	bl 0x0200bd14
	ldr r2, [pc, #840]
	ldr r3, [r2]
	mov r8, r2
	movs r2, #228
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #40
	str r2, [r3]
	bl 0x0200bd2c
	movs r0, #0
	ldr r1, [pc, #820]
	ldr r2, [pc, #824]
	bl 0x0200bc2c
	movs r2, #139
	movs r0, #0
	movs r1, #192
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bc2c
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #9
	bl 0x0200bc2c
	movs r0, #8
	bl 0x0200bc1c
	movs r7, #192
	lsls r7, r7, #6
	strh r7, [r0, #6]
	movs r0, #9
	bl 0x0200bc1c
	movs r6, #160
	lsls r6, r6, #7
	strh r6, [r0, #6]
	movs r0, #1
	bl 0x0200bb14
	movs r0, #8
	bl 0x0200bc1c
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #9
	bl 0x0200bc1c
	adds r0, #90
	ldrb r3, [r0]
	ands r5, r3
	strb r5, [r0]
	movs r1, #184
	movs r0, #8
	movs r2, #232
	bl 0x0200bc54
	movs r2, #232
	movs r0, #9
	movs r1, #198
	bl 0x0200bc5c
	movs r1, #1
	movs r0, #8
	bl 0x0200bc7c
	movs r0, #20
	bl 0x0200bbec
	movs r0, #8
	bl 0x0200bc1c
	adds r0, #90
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #9
	bl 0x0200bc1c
	adds r0, #90
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r0, #20
	bl 0x0200bbec
	movs r1, #4
	movs r0, #8
	bl 0x0200bc84
	ldr r0, [pc, #640]
	bl 0x0200bcbc
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r2, #10
	movs r0, #9
	adds r1, r6, #0
	bl 0x0200bce4
	movs r0, #9
	movs r1, #3
	bl 0x0200bc84
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl 0x0200bcd4
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200bd04
	movs r0, #60
	bl 0x0200bbec
	movs r0, #0
	bl 0x0200bc1c
	cmp r0, #0
	beq .L_02002008_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #2
	bl 0x0200bc74
.L_02002008_0:
	movs r0, #2
	ldr r1, [pc, #552]
	ldr r2, [pc, #552]
	bl 0x0200bc2c
	movs r2, #134
	movs r0, #2
	movs r1, #212
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200bce4
	ldr r0, [pc, #520]
	movs r1, #0
	movs r2, #20
	bl 0x0200bcd4
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r2, #254
	movs r1, #202
	movs r0, #2
	bl 0x0200bc5c
	movs r0, #20
	bl 0x0200bbec
	movs r1, #3
	movs r0, #2
	bl 0x0200bc84
	movs r0, #10
	bl 0x0200bbec
	movs r0, #2
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r0, #8
	ldr r1, [pc, #460]
	movs r2, #0
	bl 0x0200bcfc
	movs r0, #9
	ldr r1, [pc, #448]
	movs r2, #40
	bl 0x0200bcfc
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x0200bce4
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200bce4
	movs r0, #8
	adds r1, r7, #0
	movs r2, #0
	bl 0x0200bce4
	movs r0, #9
	adds r1, r7, #0
	movs r2, #20
	bl 0x0200bce4
	movs r1, #129
	movs r2, #60
	movs r0, #2
	lsls r1, r1, #1
	bl 0x0200bcfc
	movs r0, #2
	movs r1, #1
	bl 0x0200bc9c
	movs r2, #10
	movs r0, #2
	movs r1, #0
	bl 0x0200bcd4
	movs r0, #8
	movs r1, #2
	bl 0x0200bc9c
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r2, #60
	movs r0, #8
	ldr r1, [pc, #352]
	bl 0x0200bcfc
	movs r0, #8
	movs r1, #2
	bl 0x0200bc94
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x0200bcd4
	movs r0, #9
	movs r1, #2
	bl 0x0200bc9c
	movs r2, #20
	movs r0, #9
	movs r1, #0
	bl 0x0200bcd4
	movs r1, #1
	movs r0, #2
	bl 0x0200bc9c
	movs r0, #10
	bl 0x0200bbec
	movs r1, #192
	movs r2, #10
	movs r0, #2
	lsls r1, r1, #7
	bl 0x0200bce4
	movs r1, #0
	ldr r0, [pc, #272]
	bl 0x0200bcdc
	movs r0, #10
	bl 0x0200bbec
	movs r0, #8
	movs r1, #1
	bl 0x0200bc9c
	movs r0, #8
	movs r1, #4
	bl 0x0200bc84
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r0, #2
	ldr r1, [pc, #236]
	movs r2, #40
	bl 0x0200bcfc
	movs r1, #160
	movs r2, #20
	movs r0, #2
	lsls r1, r1, #8
	bl 0x0200bce4
	movs r0, #9
	movs r1, #1
	bl 0x0200bc9c
	movs r0, #9
	movs r1, #3
	bl 0x0200bc84
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #60
	bl 0x0200bce4
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200bce4
	movs r0, #2
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x0200bce4
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200bce4
	movs r0, #8
	adds r1, r7, #0
	movs r2, #0
	bl 0x0200bce4
	movs r2, #10
	movs r0, #9
	adds r1, r7, #0
	bl 0x0200bce4
	movs r0, #8
	movs r1, #1
	bl 0x0200bc9c
	movs r1, #0
	movs r0, #8
	bl 0x0200bcc4
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200bce4
	movs r0, #0
	movs r1, #0
	bl 0x0200bc14
	cmp r0, #0
	bne .L_02002008_1
	movs r0, #2
	movs r1, #2
	bl 0x0200bc9c
	movs r2, #10
	ldr r0, [pc, #48]
	movs r1, #0
	bl 0x0200bcd4
	mov r3, r8
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02002008_2
	.2byte 0x0000
	.4byte 0x00003333
	.4byte 0x00000666
	.4byte 0x03001ebc
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x00001b05
	.4byte 0x00004002
	.4byte 0x00000101
	.4byte 0x00000105
.L_02002008_1:
	mov r3, r8
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #2
	movs r1, #4
	bl 0x0200bc84
	ldr r0, [pc, #716]
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
.L_02002008_2:
	movs r0, #9
	movs r1, #2
	bl 0x0200bc9c
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r1, #192
	movs r2, #10
	movs r0, #2
	lsls r1, r1, #8
	bl 0x0200bce4
	movs r0, #2
	movs r1, #4
	bl 0x0200bc84
	movs r2, #10
	movs r0, #2
	movs r1, #0
	bl 0x0200bcd4
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #8
	bl 0x0200bd04
	movs r0, #60
	bl 0x0200bbec
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r1, #160
	movs r2, #10
	movs r0, #2
	lsls r1, r1, #8
	bl 0x0200bce4
	movs r0, #2
	movs r1, #3
	bl 0x0200bc84
	movs r2, #10
	movs r0, #2
	movs r1, #0
	bl 0x0200bcd4
	movs r1, #129
	movs r0, #8
	lsls r1, r1, #1
	bl 0x0200bd04
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #9
	bl 0x0200bd04
	movs r0, #60
	bl 0x0200bbec
	movs r0, #8
	movs r1, #2
	bl 0x0200bc9c
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x0200bcd4
	movs r0, #9
	movs r1, #2
	bl 0x0200bc9c
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200bce4
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x0200bce4
	movs r0, #8
	movs r1, #3
	bl 0x0200bc84
	movs r1, #192
	movs r2, #10
	movs r0, #8
	lsls r1, r1, #6
	bl 0x0200bce4
	movs r0, #8
	movs r1, #0
	bl 0x0200bccc
	movs r1, #192
	movs r2, #10
	movs r0, #9
	lsls r1, r1, #6
	bl 0x0200bce4
	movs r0, #2
	movs r1, #3
	bl 0x0200bc84
	movs r1, #192
	movs r2, #10
	movs r0, #2
	lsls r1, r1, #7
	bl 0x0200bce4
	movs r1, #0
	ldr r0, [pc, #456]
	bl 0x0200bcdc
	movs r0, #10
	bl 0x0200bbec
	movs r0, #2
	movs r1, #1
	bl 0x0200bc9c
	movs r2, #10
	ldr r0, [pc, #436]
	movs r1, #0
	bl 0x0200bcd4
	movs r0, #0
	movs r1, #3
	bl 0x0200bc84
	movs r0, #2
	movs r1, #3
	bl 0x0200bc84
	movs r1, #192
	movs r2, #10
	movs r0, #2
	lsls r1, r1, #8
	bl 0x0200bce4
	movs r0, #2
	movs r1, #3
	bl 0x0200bc84
	movs r0, #8
	ldr r1, [pc, #392]
	ldr r2, [pc, #396]
	bl 0x0200bc2c
	ldr r1, [pc, #384]
	ldr r2, [pc, #388]
	movs r0, #9
	bl 0x0200bc2c
	movs r0, #8
	bl 0x0200bc1c
	movs r3, #0
	strh r3, [r0, #6]
	movs r0, #9
	bl 0x0200bc1c
	movs r3, #128
	lsls r3, r3, #8
	strh r3, [r0, #6]
	movs r0, #8
	bl 0x0200bc1c
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #9
	bl 0x0200bc1c
	adds r0, #90
	ldrb r3, [r0]
	ands r5, r3
	strb r5, [r0]
	movs r1, #168
	movs r0, #8
	movs r2, #232
	bl 0x0200bc54
	movs r2, #232
	movs r0, #9
	movs r1, #212
	bl 0x0200bc5c
	movs r1, #1
	movs r0, #8
	bl 0x0200bc7c
	movs r0, #20
	bl 0x0200bbec
	movs r0, #8
	bl 0x0200bc1c
	adds r0, #90
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #9
	bl 0x0200bc1c
	adds r0, #90
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r1, #192
	movs r0, #2
	movs r2, #232
	bl 0x0200bc5c
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #2
	bl 0x0200bce4
	movs r0, #188
	bl 0x0200bd7c
	movs r5, #2
	movs r1, #23
	movs r2, #43
	movs r3, #12
	movs r0, #36
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200bb9c
	movs r0, #5
	bl 0x0200bb14
	movs r3, #12
	movs r1, #23
	movs r2, #43
	movs r0, #39
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200bb9c
	movs r0, #5
	bl 0x0200bb14
	movs r0, #2
	movs r1, #192
	movs r2, #222
	bl 0x0200bc5c
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x0200bc74
	movs r0, #0
	ldr r1, [pc, #168]
	ldr r2, [pc, #172]
	bl 0x0200bc2c
	movs r0, #0
	movs r1, #192
	movs r2, #222
	bl 0x0200bc5c
	movs r2, #0
	movs r0, #0
	movs r1, #0
	bl 0x0200bc74
	movs r0, #8
	movs r1, #3
	bl 0x0200bc7c
	movs r0, #9
	movs r1, #3
	bl 0x0200bc84
	movs r0, #8
	movs r1, #184
	movs r2, #232
	bl 0x0200bc54
	movs r0, #9
	movs r1, #198
	movs r2, #232
	bl 0x0200bc5c
	movs r0, #8
	movs r1, #188
	movs r2, #212
	bl 0x0200bc54
	movs r0, #9
	movs r1, #194
	movs r2, #212
	bl 0x0200bc5c
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x0200bc74
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200bc74
	ldr r3, [pc, #72]
	ldr r1, [r3]
	movs r3, #228
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #24
	str r3, [r2]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #65
	str r3, [r2]
	bl 0x0200bd34
	bl 0x0200bd3c
	movs r0, #5
	bl 0x0200bd24
	bl 0x0200bbfc
	sub sp, #-8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00004002
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x00013333
	.4byte 0x00009999
	.4byte 0x03001ebc
	.global Func_020026c0
	.thumb_func
Func_020026c0:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	sub sp, #8
	bl 0x0200bbf4
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r1, r1
	negs r2, r2
	negs r0, r0
	bl 0x0200bd14
	movs r0, #1
	bl 0x0200bb14
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200bc74
	movs r1, #220
	ldr r2, [pc, #1004]
	lsls r1, r1, #17
	movs r0, #9
	bl 0x0200bc74
	movs r0, #9
	bl 0x0200bc1c
	movs r1, #0
	bl 0x0200bbb4
	movs r0, #220
	movs r1, #1
	ldr r2, [pc, #976]
	movs r3, #0
	negs r1, r1
	lsls r0, r0, #17
	bl 0x0200bd14
	bl 0x0200bb7c
	movs r0, #1
	bl 0x0200bb14
	bl 0x0200bd2c
	ldr r0, [pc, #956]
	ldr r1, [pc, #956]
	bl 0x0200bd0c
	movs r0, #220
	movs r1, #1
	movs r2, #200
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl 0x0200bd14
	movs r0, #141
	bl 0x0200bd7c
	movs r0, #9
	ldr r1, [pc, #920]
	ldr r2, [pc, #928]
	bl 0x0200bc2c
	movs r1, #220
	movs r2, #200
	lsls r2, r2, #1
	movs r0, #9
	lsls r1, r1, #1
	bl 0x0200bc5c
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #8
	lsls r1, r1, #5
	bl 0x0200bd0c
	movs r0, #220
	movs r1, #1
	movs r2, #150
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	bl 0x0200bd14
	movs r0, #9
	ldr r1, [pc, #876]
	ldr r2, [pc, #876]
	bl 0x0200bc2c
	movs r1, #220
	movs r2, #150
	lsls r2, r2, #1
	movs r0, #9
	lsls r1, r1, #1
	bl 0x0200bc5c
	movs r1, #0
	movs r0, #9
	bl 0x0200bc7c
	ldr r0, [pc, #852]
	bl 0x0200bd7c
	movs r0, #40
	bl 0x0200bbec
	movs r0, #11
	ldr r1, [pc, #832]
	ldr r2, [pc, #832]
	bl 0x0200bc2c
	movs r2, #153
	movs r0, #11
	ldr r1, [pc, #832]
	lsls r2, r2, #17
	bl 0x0200bc74
	movs r0, #11
	movs r1, #4
	movs r2, #0
	bl 0x0200bc8c
	movs r2, #156
	movs r0, #11
	ldr r1, [pc, #812]
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #208
	movs r2, #156
	movs r0, #11
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #200
	movs r2, #128
	movs r0, #11
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #192
	movs r0, #11
	lsls r1, r1, #6
	movs r2, #40
	bl 0x0200bce4
	movs r0, #205
	movs r1, #1
	movs r2, #140
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	bl 0x0200bd14
	movs r0, #10
	ldr r1, [pc, #748]
	ldr r2, [pc, #752]
	bl 0x0200bc2c
	movs r2, #153
	movs r0, #10
	ldr r1, [pc, #728]
	lsls r2, r2, #17
	bl 0x0200bc74
	movs r0, #10
	movs r1, #4
	movs r2, #0
	bl 0x0200bc8c
	movs r2, #156
	movs r0, #10
	ldr r1, [pc, #712]
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #208
	movs r2, #156
	movs r0, #10
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #194
	movs r2, #135
	movs r0, #10
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #208
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200bce4
	movs r1, #160
	movs r0, #11
	lsls r1, r1, #7
	movs r2, #10
	bl 0x0200bce4
	movs r0, #0
	ldr r1, [pc, #632]
	ldr r2, [pc, #636]
	bl 0x0200bc2c
	movs r2, #153
	movs r0, #0
	ldr r1, [pc, #632]
	lsls r2, r2, #17
	bl 0x0200bc74
	movs r0, #0
	movs r1, #4
	movs r2, #0
	bl 0x0200bc8c
	movs r2, #156
	movs r0, #0
	ldr r1, [pc, #616]
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #208
	movs r2, #156
	movs r0, #0
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #194
	movs r2, #150
	movs r0, #0
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #192
	movs r2, #40
	lsls r1, r1, #8
	movs r0, #0
.L_020028be:
	bl 0x0200bce4
	movs r0, #0
	bl 0x0200bc1c
	adds r5, r0, #0
	bl 0x0200bb2c
	lsls r3, r0, #2
	adds r3, r3, r0
	ldr r6, [pc, #564]
	lsrs r3, r3, #12
	adds r5, #102
	strh r3, [r5]
	movs r0, #0
	adds r1, r6, #0
	bl 0x0200bc34
	movs r2, #20
	movs r0, #11
	movs r1, #2
	bl 0x0200bc8c
	movs r1, #3
	movs r0, #11
	bl 0x0200bc84
	ldr r0, [pc, #532]
	bl 0x0200bcbc
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r0, #10
	movs r1, #3
	bl 0x0200bc84
	movs r0, #0
	bl 0x0200bc1c
	cmp r0, #0
	beq .L_020028be_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200bc74
.L_020028be_0:
	movs r0, #0
	bl 0x0200bc1c
	cmp r0, #0
	beq .L_020028be_1
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #2
	bl 0x0200bc74
.L_020028be_1:
	movs r0, #0
	bl 0x0200bc1c
	cmp r0, #0
	beq .L_020028be_2
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #3
	bl 0x0200bc74
.L_020028be_2:
	movs r0, #1
	ldr r1, [pc, #416]
	ldr r2, [pc, #416]
	bl 0x0200bc2c
	movs r0, #2
	ldr r1, [pc, #404]
	ldr r2, [pc, #408]
	bl 0x0200bc2c
	movs r0, #3
	ldr r1, [pc, #396]
	ldr r2, [pc, #396]
	bl 0x0200bc2c
	movs r1, #189
	movs r2, #155
	movs r0, #1
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x0200bc54
	movs r1, #200
	movs r2, #144
	movs r0, #2
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x0200bc54
	movs r1, #205
	movs r2, #154
	lsls r2, r2, #1
	movs r0, #3
	lsls r1, r1, #1
	bl 0x0200bc5c
	movs r0, #1
	movs r1, #1
	bl 0x0200bc7c
	movs r0, #2
	movs r1, #1
	bl 0x0200bc7c
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #192
	movs r2, #40
	lsls r1, r1, #8
	movs r0, #3
	bl 0x0200bce4
	movs r0, #1
	bl 0x0200bc1c
	adds r5, r0, #0
	bl 0x0200bb2c
	lsls r3, r0, #2
	adds r3, r3, r0
	lsrs r3, r3, #12
	adds r5, #102
	strh r3, [r5]
	movs r0, #2
	bl 0x0200bc1c
	adds r5, r0, #0
	bl 0x0200bb2c
	lsls r3, r0, #2
	adds r3, r3, r0
	lsrs r3, r3, #12
	adds r5, #102
	strh r3, [r5]
	movs r0, #3
	bl 0x0200bc1c
	adds r5, r0, #0
	bl 0x0200bb2c
	lsls r3, r0, #2
	adds r3, r3, r0
	lsrs r3, r3, #12
	adds r5, #102
	strh r3, [r5]
	movs r0, #1
	adds r1, r6, #0
	bl 0x0200bc34
	movs r0, #2
	adds r1, r6, #0
	bl 0x0200bc34
	movs r0, #3
	adds r1, r6, #0
	bl 0x0200bc34
	movs r0, #2
	ldr r1, [pc, #236]
	movs r2, #60
	bl 0x0200bcfc
	ldr r0, [pc, #232]
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200bcfc
	movs r1, #192
	movs r0, #10
	lsls r1, r1, #6
	movs r2, #20
	bl 0x0200bce4
	movs r1, #0
	movs r2, #10
	movs r0, #10
	bl 0x0200bcd4
	movs r0, #0
	bl 0x0200bc44
	movs r0, #1
	bl 0x0200bc44
	movs r0, #2
	bl 0x0200bc44
	movs r0, #3
	bl 0x0200bc44
	movs r0, #1
	bl 0x0200bb14
	movs r1, #192
	movs r2, #10
	movs r0, #2
	lsls r1, r1, #8
	bl 0x0200bce4
	movs r0, #2
	movs r1, #3
	bl 0x0200bc84
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #192
	movs r2, #20
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200bce4
	movs r0, #10
	movs r1, #3
	bl 0x0200bc84
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r0, #3
	ldr r1, [pc, #80]
	movs r2, #60
	bl 0x0200bcfc
	movs r2, #10
	ldr r0, [pc, #76]
	movs r1, #0
	bl 0x0200bcd4
	movs r0, #10
	movs r1, #3
	bl 0x0200bc84
	movs r0, #10
	movs r1, #0
	movs r2, #10
	b .L_020028be_3
	.2byte 0x0000
	.2byte 0x020a
	.2byte 0x9999
	.2byte 0x0001
	.2byte 0x3333
	.2byte 0x0000
	.4byte 0x0000cccc
	.4byte 0x00006666
	.2byte 0x0121
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x01b7
	.2byte 0x01b7
	.2byte 0x0000
	.2byte 0x9999
	.2byte 0x0000
	.2byte 0x4ccc
	.2byte 0x0000
	.4byte 0x0200c21c
	.4byte 0x0000256f
	.4byte 0x00000101
	.4byte 0x00002002
	.4byte 0x00002003
.L_020028be_3:
	bl 0x0200bcd4
	movs r0, #1
	ldr r1, [pc, #620]
	movs r2, #60
	bl 0x0200bcfc
	movs r0, #1
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
.L_02002b34:
	movs r1, #160
	movs r2, #0
	movs r0, #10
	lsls r1, r1, #7
	bl 0x0200bce4
	movs r0, #11
	movs r1, #2
	bl 0x0200bc9c
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200bce4
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
.L_02002b6a:
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200bce4
	movs r1, #160
	movs r2, #20
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200bce4
	movs r0, #0
	movs r1, #3
	bl 0x0200bc7c
	movs r0, #1
	movs r1, #3
	bl 0x0200bc7c
	movs r0, #2
	movs r1, #3
	bl 0x0200bc7c
	movs r1, #3
	movs r0, #3
	bl 0x0200bc84
.L_02002ba2:
	movs r0, #10
	bl 0x0200bbec
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200bce4
	movs r1, #208
	movs r2, #10
	movs r0, #10
	lsls r1, r1, #8
	bl 0x0200bce4
	movs r0, #10
	movs r1, #3
	bl 0x0200bc84
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl 0x0200bcd4
	movs r0, #11
	movs r1, #3
	bl 0x0200bc84
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #2
	bl 0x0200bd04
	movs r0, #40
	bl 0x0200bbec
	movs r0, #2
	movs r1, #2
	bl 0x0200bc94
	ldr r0, [pc, #380]
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r1, #192
	movs r2, #10
	movs r0, #11
	lsls r1, r1, #6
	bl 0x0200bce4
	movs r0, #11
	movs r1, #4
	bl 0x0200bc7c
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r0, #3
	ldr r1, [pc, #332]
	movs r2, #40
	bl 0x0200bcfc
	ldr r0, [pc, #332]
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r1, #192
	movs r0, #10
	lsls r1, r1, #6
	movs r2, #10
	bl 0x0200bce4
	movs r1, #132
	movs r0, #10
	lsls r1, r1, #1
.L_02002c64:
	movs r2, #20
	bl 0x0200bcfc
.L_02002c6a:
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl 0x0200bcd4
	movs r0, #1
	movs r1, #2
	bl 0x0200bc9c
	movs r0, #1
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r1, #160
	movs r0, #11
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200bce4
	movs r1, #160
	movs r2, #10
	movs r0, #10
	lsls r1, r1, #7
	bl 0x0200bce4
	movs r0, #11
	movs r1, #4
	bl 0x0200bc7c
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200bce4
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200bce4
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #192
	movs r2, #10
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200bce4
.L_02002ce0:
	movs r0, #11
	movs r1, #4
	bl 0x0200bc84
	movs r2, #10
	movs r0, #11
	movs r1, #0
	bl 0x0200bcd4
	movs r0, #10
	movs r1, #4
	bl 0x0200bc84
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r0, #0
	ldr r1, [pc, #148]
	movs r2, #0
	bl 0x0200bcfc
	movs r0, #1
	ldr r1, [pc, #136]
	movs r2, #0
	bl 0x0200bcfc
	movs r0, #2
	ldr r1, [pc, #128]
	movs r2, #0
	bl 0x0200bcfc
	movs r0, #3
.L_02002d24:
	ldr r1, [pc, #116]
	movs r2, #60
	bl 0x0200bcfc
	movs r1, #192
	movs r2, #10
	movs r0, #10
	lsls r1, r1, #6
	bl 0x0200bce4
	movs r0, #10
	movs r1, #3
	bl 0x0200bc7c
	movs r1, #0
	movs r0, #10
	bl 0x0200bcc4
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200bce4
	movs r1, #160
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r0, #0
	movs r1, #0
	bl 0x0200bc14
	cmp r0, #0
	bne .L_02002d24_0
	movs r0, #20
	bl 0x0200bbec
	ldr r3, [pc, #32]
	movs r1, #236
	ldr r2, [r3]
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	adds r3, #3
	strh r3, [r2]
	b .L_02002d24_1
	.2byte 0x0101
	.2byte 0x0000
	.2byte 0x2002
	.2byte 0x0000
	.2byte 0x2003
	.2byte 0x0000
	.4byte 0x00000105
	.4byte 0x03001ebc
.L_02002d24_0:
	movs r0, #20
	bl 0x0200bbec
	movs r0, #11
	movs r1, #2
	bl 0x0200bc9c
	movs r0, #11
	movs r1, #0
	movs r2, #40
	bl 0x0200bcd4
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r1, #131
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200bcfc
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200bce4
	ldr r0, [pc, #888]
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
.L_02002d24_1:
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #192
	movs r2, #10
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200bce4
	movs r0, #1
	movs r1, #3
	bl 0x0200bc7c
	movs r0, #2
	movs r1, #3
	bl 0x0200bc7c
	movs r1, #3
	movs r0, #3
	bl 0x0200bc84
	movs r0, #20
	bl 0x0200bbec
	movs r1, #192
	movs r2, #0
	movs r0, #8
	lsls r1, r1, #6
	bl 0x0200bce4
	movs r0, #10
	movs r1, #3
	bl 0x0200bc84
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200bd0c
	movs r0, #140
	movs r1, #1
	movs r2, #200
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	bl 0x0200bd14
.L_02002e5c:
	movs r0, #10
	ldr r1, [pc, #764]
	movs r2, #222
	bl 0x0200bc5c
	movs r1, #142
	movs r0, #10
	lsls r1, r1, #1
	movs r2, #198
	bl 0x0200bc5c
	movs r1, #128
	movs r2, #10
	movs r0, #10
	lsls r1, r1, #8
	bl 0x0200bce4
	movs r0, #10
	movs r1, #3
	bl 0x0200bc84
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x0200bce4
	movs r0, #8
	movs r1, #3
	bl 0x0200bc84
	movs r0, #8
	ldr r1, [pc, #708]
	ldr r2, [pc, #708]
	bl 0x0200bc2c
	movs r1, #134
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #198
	bl 0x0200bc5c
	movs r1, #192
	movs r2, #10
	movs r0, #8
	lsls r1, r1, #8
	bl 0x0200bce4
	movs r1, #2
	movs r0, #8
	bl 0x0200bc9c
	movs r0, #125
	bl 0x0200bd7c
	movs r3, #2
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #71
	movs r1, #60
	movs r2, #76
	movs r3, #11
.L_02002ed8:
	bl 0x0200bb9c
	movs r3, #16
	movs r2, #11
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r1, #60
	movs r2, #2
	movs r0, #71
	bl 0x0200bba4
	movs r0, #20
	bl 0x0200bbec
	movs r0, #8
	movs r1, #246
	movs r2, #198
	bl 0x0200bc5c
	movs r2, #20
	movs r1, #0
	movs r0, #8
	bl 0x0200bce4
	bl 0x0200bd1c
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	ldr r1, [pc, #592]
	ldr r0, [pc, #596]
	bl 0x0200bd0c
	movs r0, #248
	movs r1, #1
	movs r2, #170
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #16
	bl 0x0200bd14
	movs r1, #135
.L_02002f30:
	movs r0, #10
	lsls r1, r1, #1
	movs r2, #198
	bl 0x0200bc5c
	movs r1, #135
	movs r0, #10
	lsls r1, r1, #1
	movs r2, #174
	bl 0x0200bc5c
	movs r0, #10
	movs r1, #224
	movs r2, #170
	bl 0x0200bc5c
	movs r0, #10
	movs r1, #210
	movs r2, #158
	bl 0x0200bc5c
	movs r0, #10
	movs r1, #246
	movs r2, #148
	bl 0x0200bc5c
	movs r0, #10
	movs r1, #246
	movs r2, #142
	bl 0x0200bc5c
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x0200bc74
	ldr r2, [pc, #500]
	movs r3, #224
	lsls r3, r3, #1
	mov r10, r3
	mov r8, r2
	ldr r2, [r2]
	adds r3, #66
	mov r1, r10
	str r3, [r2, r1]
	bl 0x0200bd34
	bl 0x0200bd3c
	movs r1, #220
	movs r2, #170
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r0, #9
	bl 0x0200bc74
	movs r0, #9
	bl 0x0200bc1c
	movs r6, #128
	lsls r6, r6, #7
	strh r6, [r0, #6]
	movs r1, #1
	movs r0, #190
	movs r2, #140
	movs r3, #0
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #17
	bl 0x0200bd14
	bl 0x0200bb7c
	movs r0, #10
	bl 0x0200bb14
.L_02002fc8:
	bl 0x0200bd2c
	bl 0x0200bd3c
	movs r0, #40
	bl 0x0200bbec
	movs r1, #3
	movs r0, #11
	bl 0x0200bc84
	movs r0, #20
	bl 0x0200bbec
	movs r0, #0
	movs r1, #3
	bl 0x0200bc7c
	movs r0, #1
	movs r1, #3
	bl 0x0200bc7c
	movs r0, #2
	movs r1, #3
	bl 0x0200bc7c
	movs r0, #3
	movs r1, #3
	bl 0x0200bc84
	movs r1, #210
	movs r2, #141
	movs r0, #11
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #210
	movs r2, #156
	movs r0, #11
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r2, #156
	movs r0, #11
	ldr r1, [pc, #332]
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r2, #153
	movs r0, #11
	ldr r1, [pc, #320]
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r2, #0
	movs r0, #11
	movs r1, #0
	bl 0x0200bc74
	ldr r5, [pc, #308]
	movs r0, #1
	adds r1, r5, #0
	bl 0x0200bc34
	adds r1, r5, #0
	movs r0, #2
	bl 0x0200bc34
	adds r1, r5, #0
	movs r0, #3
	bl 0x0200bc4c
	movs r0, #205
	movs r1, #1
	movs r2, #150
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	bl 0x0200bd14
	movs r1, #208
	movs r2, #156
	movs r0, #0
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r2, #156
	movs r0, #0
	ldr r1, [pc, #240]
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r2, #153
	movs r0, #0
	ldr r1, [pc, #228]
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #0
	movs r2, #0
	movs r0, #0
	bl 0x0200bc74
	movs r0, #141
	bl 0x0200bd7c
	ldr r2, [pc, #188]
	movs r0, #9
	ldr r1, [pc, #180]
	bl 0x0200bc2c
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200bd0c
	movs r0, #220
	movs r1, #1
	movs r2, #210
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	bl 0x0200bd14
	movs r1, #220
	movs r2, #210
	lsls r2, r2, #1
	movs r0, #9
	lsls r1, r1, #1
	bl 0x0200bc5c
	movs r0, #128
	adds r1, r6, #0
	lsls r0, r0, #10
	bl 0x0200bd0c
	movs r0, #220
	movs r1, #1
	movs r2, #150
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #18
	bl 0x0200bd14
	movs r0, #9
	ldr r1, [pc, #128]
	ldr r2, [pc, #100]
	bl 0x0200bc2c
	movs r1, #220
	movs r2, #250
	movs r0, #9
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #220
	movs r2, #150
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #9
	bl 0x0200bc54
	ldr r0, [pc, #96]
	bl 0x0200bd7c
	mov r2, r8
	ldr r1, [r2]
	movs r3, #228
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #24
	str r3, [r2]
	adds r3, #232
	mov r2, r10
	str r3, [r1, r2]
	bl 0x0200bd34
	bl 0x0200bd3c
	movs r0, #10
	bl 0x0200bd24
	bl 0x0200bbfc
	sub sp, #-8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x2003
	.2byte 0x0000
	.2byte 0x014d
	.2byte 0x0000
	.4byte 0x0000cccc
	.4byte 0x00006666
	.2byte 0x1333
	.2byte 0x0000
	.2byte 0x9999
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.4byte 0x000001b7
	.4byte 0x0200c230
	.4byte 0x00019999
	.4byte 0x00000121
	.global Func_02003184
	.thumb_func
Func_02003184:
	push {r5, lr}
	ldr r3, [pc, #44]
	ldr r3, [r3]
	adds r2, r3, #0
	adds r5, r0, #0
	movs r4, #8
	adds r2, #52
.L_02003184_2:
	ldmia r2!, {r0}
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r5, r3
	bne .L_02003184_0
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r1, r3
	beq .L_02003184_1
.L_02003184_0:
	adds r4, #1
	cmp r4, #65
	bls .L_02003184_2
	movs r0, #0
.L_02003184_1:
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.global Func_020031b8
	.thumb_func
Func_020031b8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #0
	sub sp, #12
	bl 0x0200bc1c
	ldrh r3, [r0, #6]
	ldr r2, [pc, #200]
	lsrs r3, r3, #12
	lsls r5, r3, #2
	ldr r3, [r2, r5]
	mov r8, r0
	movs r1, #10
	ldrsh r0, [r0, r1]
	mov r10, r2
	asrs r2, r3, #16
	adds r0, r0, r2
	mov r2, r8
	movs r4, #18
	ldrsh r1, [r2, r4]
	lsls r3, r3, #16
	asrs r3, r3, #16
	adds r1, r1, r3
	asrs r0, r0, #4
	asrs r1, r1, #4
	bl 0x0200b184
	adds r7, r0, #0
	cmp r7, #0
	beq .L_020031b8_0
	movs r3, #0
	adds r2, r7, #0
	adds r2, #34
	mov r9, r3
	movs r3, #2
	strb r3, [r2]
	mov r4, r10
	ldr r1, [r4, r5]
	ldr r2, [pc, #144]
	ldr r3, [r7, #8]
	ands r2, r1
	mov r6, sp
	adds r3, r3, r2
	str r3, [r6]
	ldr r3, [r7, #12]
	str r3, [r6, #4]
	ldr r3, [r7, #16]
	lsls r1, r1, #16
	adds r3, r3, r1
	adds r1, r6, #0
	str r3, [r6, #8]
	bl 0x0200bbac
	cmp r0, #0
	bgt .L_020031b8_0
	movs r1, #8
	mov r0, r8
	bl 0x0200bb5c
	ldr r5, [pc, #104]
	movs r0, #15
	bl 0x0200bb14
	movs r0, #185
	bl 0x0200bd7c
	str r5, [r7, #48]
	str r5, [r7, #52]
	adds r0, r7, #0
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	bl 0x0200bb84
	mov r1, r8
	str r5, [r1, #48]
	str r5, [r1, #52]
	mov r0, r8
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	bl 0x0200bb84
	adds r0, r7, #0
	bl 0x0200bb8c
	bl 0x0200bd6c
	ldr r3, [r6]
	str r3, [r7, #8]
	ldr r3, [r6, #8]
	mov r2, r9
	str r3, [r7, #16]
	str r2, [r7, #36]
	str r2, [r7, #44]
	mov r0, r8
	movs r1, #1
	bl 0x0200bb5c
	bl 0x0200b2a4
.L_020031b8_0:
	sub sp, #-12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0200bd84
	.4byte 0xffff0000
	.4byte 0x00003333
	.global Func_020032a4
	.thumb_func
Func_020032a4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #8
	sub sp, #8
	bl 0x0200bc1c
	adds r6, r0, #0
	movs r0, #9
	bl 0x0200bc1c
	mov r10, r0
	ldr r0, [pc, #676]
	bl 0x0200bbd4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020032a4_0
	b .L_020032a4_1
.L_020032a4_0:
	ldr r3, [r6, #8]
	asrs r3, r3, #19
	cmp r3, #29
	ble .L_020032a4_2
	b .L_020032a4_1
.L_020032a4_2:
	movs r0, #11
	bl 0x0200bc1c
	mov r8, r0
	bl 0x0200bbf4
	movs r3, #1
	movs r0, #7
	movs r1, #44
	movs r2, #1
	str r5, [sp, #0]
	str r3, [sp, #4]
	bl 0x0200bba4
	movs r5, #67
	movs r7, #1
	movs r6, #5
.L_020032a4_4:
	adds r0, r5, #0
	movs r1, #58
	movs r2, #78
	movs r3, #41
	str r7, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200bb9c
	movs r0, #4
	bl 0x0200bbec
	cmp r5, #70
	bne .L_020032a4_3
	ldr r0, [pc, #592]
	bl 0x0200bbdc
.L_020032a4_3:
	adds r5, #1
	cmp r5, #74
	bls .L_020032a4_4
	movs r3, #3
	str r3, [sp, #0]
	movs r5, #2
	movs r1, #109
	movs r2, #13
	movs r3, #109
	movs r0, #16
	str r5, [sp, #4]
	bl 0x0200bb9c
	movs r0, #40
	bl 0x0200bbec
	ldr r3, [pc, #556]
	mov r2, r8
	str r3, [r2, #24]
	str r3, [r2, #28]
	movs r1, #150
	movs r2, #182
	lsls r2, r2, #18
	movs r0, #11
	lsls r1, r1, #16
	bl 0x0200bc74
	ldr r1, [pc, #540]
	movs r0, #11
	bl 0x0200bc34
	movs r6, #1
	movs r0, #67
	movs r1, #64
	movs r2, #71
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #64
	movs r2, #72
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #68
	movs r2, #73
	movs r3, #43
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #68
	movs r2, #74
	movs r3, #43
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #64
	movs r2, #75
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #66
	movs r2, #76
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #64
	movs r2, #77
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #64
	movs r2, #78
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #64
	movs r2, #79
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #66
	movs r2, #80
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r1, #0
	movs r2, #9
	movs r3, #42
	movs r0, #2
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200bb9c
	movs r0, #40
	bl 0x0200bbec
	movs r0, #68
	movs r1, #64
	movs r2, #71
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #64
	movs r2, #72
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #68
	movs r2, #73
	movs r3, #43
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #68
	movs r2, #74
	movs r3, #43
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #64
	movs r2, #75
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #66
	movs r2, #76
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #64
	movs r2, #77
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #64
	movs r2, #78
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #64
	movs r2, #79
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #66
	movs r2, #80
	movs r3, #44
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200bb9c
	movs r1, #0
	movs r2, #9
	movs r3, #42
	movs r0, #4
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200bb9c
	movs r0, #40
	bl 0x0200bbec
	movs r3, #8
	str r3, [sp, #4]
	movs r5, #10
	movs r0, #7
	movs r1, #11
	movs r2, #7
	movs r3, #42
	str r5, [sp, #0]
	bl 0x0200bb9c
	movs r3, #13
	str r3, [sp, #4]
	movs r0, #71
	movs r1, #12
	movs r2, #71
	movs r3, #43
	str r5, [sp, #0]
	bl 0x0200bb9c
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #13
	movs r2, #12
	movs r3, #12
	movs r5, #44
	movs r0, #6
	str r5, [sp, #4]
	bl 0x0200bba4
	movs r0, #40
	bl 0x0200bbec
	bl 0x020095b4
	movs r3, #7
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #1
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200bba4
	bl 0x0200bbfc
.L_020032a4_1:
	ldr r0, [pc, #80]
	bl 0x0200bbd4
	cmp r0, #0
	beq .L_020032a4_5
	b .L_020032a4_6
.L_020032a4_5:
	mov r2, r10
	ldr r3, [r2, #8]
	asrs r3, r3, #19
	cmp r3, #87
	ble .L_020032a4_7
	b .L_020032a4_6
.L_020032a4_7:
	bl 0x0200bbf4
	movs r5, #67
	movs r7, #1
	movs r6, #5
.L_020032a4_9:
	adds r0, r5, #0
	movs r1, #58
	movs r2, #107
	movs r3, #41
	str r7, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200bb9c
	movs r0, #4
	bl 0x0200bbec
	cmp r5, #70
	bne .L_020032a4_8
	ldr r0, [pc, #20]
	bl 0x0200bbdc
	b .L_020032a4_8
	.2byte 0x0000
	.4byte 0x00000302
	.4byte 0x00001999
	.4byte 0x0200c268
	.4byte 0x00000303
.L_020032a4_8:
	adds r5, #1
	cmp r5, #74
	bls .L_020032a4_9
	movs r3, #3
	str r3, [sp, #0]
	movs r6, #2
	movs r1, #109
	movs r2, #42
	movs r3, #109
	movs r0, #45
	str r6, [sp, #4]
	bl 0x0200bb9c
	movs r0, #40
	bl 0x0200bbec
	movs r5, #1
	movs r0, #67
	movs r1, #64
	movs r2, #102
	movs r3, #44
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #64
	movs r2, #103
	movs r3, #44
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #64
	movs r2, #104
	movs r3, #44
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #66
	movs r2, #105
	movs r3, #44
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #64
	movs r2, #106
	movs r3, #44
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #64
	movs r2, #107
	movs r3, #44
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200bb9c
	movs r0, #67
	movs r1, #64
	movs r2, #108
	movs r3, #44
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200bb9c
	movs r1, #66
	movs r2, #109
	movs r3, #44
	movs r0, #67
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200bb9c
	movs r0, #40
	bl 0x0200bbec
	movs r0, #68
	movs r1, #64
	movs r2, #102
	movs r3, #44
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #64
	movs r2, #103
	movs r3, #44
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #64
	movs r2, #104
	movs r3, #44
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #66
	movs r2, #105
	movs r3, #44
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #64
	movs r2, #106
	movs r3, #44
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #64
	movs r2, #107
	movs r3, #44
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200bb9c
	movs r0, #68
	movs r1, #64
	movs r2, #108
	movs r3, #44
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200bb9c
	movs r1, #66
	movs r2, #109
	movs r3, #44
	movs r0, #68
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200bb9c
	movs r0, #40
	bl 0x0200bbec
	movs r3, #4
	str r3, [sp, #4]
	movs r5, #8
	movs r0, #38
	movs r1, #14
	movs r2, #38
	movs r3, #44
	str r5, [sp, #0]
	bl 0x0200bb9c
	movs r3, #12
	str r3, [sp, #4]
	movs r0, #102
	movs r1, #14
	movs r2, #102
	movs r3, #44
	str r5, [sp, #0]
	bl 0x0200bb9c
	movs r3, #37
	movs r2, #43
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #13
	movs r2, #10
	movs r3, #12
	movs r0, #37
	bl 0x0200bba4
	movs r0, #40
	bl 0x0200bbec
	bl 0x020095b4
	bl 0x0200bbfc
.L_020032a4_6:
	sub sp, #-8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_020036f8
	.thumb_func
Func_020036f8:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r5, #0
	adds r6, #100
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #0
	bne .L_020036f8_0
	bl 0x0200bb74
	b .L_020036f8_1
.L_020036f8_0:
	cmp r3, #1
	bne .L_020036f8_2
	movs r3, #0
	str r3, [r5, #36]
	str r3, [r5, #40]
	str r3, [r5, #8]
	str r3, [r5, #12]
	b .L_020036f8_1
.L_020036f8_2:
	ldr r3, [r5, #24]
	movs r2, #128
	lsls r2, r2, #4
	adds r3, r3, r2
	str r3, [r5, #24]
	ldr r3, [r5, #28]
	adds r3, r3, r2
	str r3, [r5, #28]
.L_020036f8_1:
	ldr r3, [r5, #8]
	ldr r2, [r5, #36]
	adds r3, r3, r2
	str r3, [r5, #8]
	ldr r1, [r5, #40]
	ldr r3, [r5, #12]
	adds r3, r3, r1
	str r3, [r5, #12]
	adds r3, r2, #0
	cmp r2, #0
	bge .L_020036f8_3
	adds r3, #255
.L_020036f8_3:
	asrs r3, r3, #8
	subs r3, r2, r3
	str r3, [r5, #36]
	adds r3, r1, #0
	cmp r1, #0
	bge .L_020036f8_4
	adds r3, #15
.L_020036f8_4:
	asrs r3, r3, #4
	subs r3, r1, r3
	str r3, [r5, #40]
	ldrh r3, [r6]
	subs r3, #1
	strh r3, [r6]
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02003768
	.thumb_func
Func_02003768:
	push {r5, r6, lr}
	movs r0, #8
	bl 0x0200bc1c
	adds r5, r0, #0
	movs r0, #9
	bl 0x0200bc1c
	movs r2, #10
	ldrsh r3, [r5, r2]
	ldr r2, [pc, #196]
	adds r3, r3, r2
	adds r6, r0, #0
	cmp r3, #12
	bhi .L_02003768_0
	movs r2, #18
	ldrsh r3, [r5, r2]
	ldr r2, [pc, #188]
	cmp r3, r2
	ble .L_02003768_0
	movs r0, #0
	bl 0x0200bc1c
	ldr r3, [r0, #80]
	ldr r4, [r5, #80]
	ldrb r3, [r3, #9]
	movs r2, #12
	ands r2, r3
	ldrb r1, [r4, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r4, #9]
	b .L_02003768_1
.L_02003768_0:
	ldr r0, [pc, #156]
	bl 0x0200bbd4
	cmp r0, #0
	bne .L_02003768_1
	movs r2, #10
	ldrsh r3, [r5, r2]
	cmp r3, #245
	bgt .L_02003768_1
	ldr r3, [pc, #140]
	ldr r3, [r3]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	bne .L_02003768_1
	ldr r0, [pc, #132]
	bl 0x0200bbd4
	cmp r0, #0
	bne .L_02003768_2
	movs r0, #1
	negs r0, r0
	bl 0x0200bd64
	movs r0, #230
	bl 0x0200bd7c
	ldr r0, [pc, #108]
	bl 0x0200bbdc
.L_02003768_2:
	ldr r0, [r5, #8]
	ldr r1, [r5, #12]
	ldr r2, [r5, #16]
	bl 0x0200b864
.L_02003768_1:
	ldr r0, [pc, #96]
	bl 0x0200bbd4
	cmp r0, #0
	bne .L_02003768_3
	movs r2, #10
	ldrsh r3, [r6, r2]
	ldr r2, [pc, #88]
	cmp r3, r2
	bgt .L_02003768_3
	ldr r3, [pc, #68]
	ldr r3, [r3]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	bne .L_02003768_3
	ldr r0, [pc, #72]
	bl 0x0200bbd4
	cmp r0, #0
	bne .L_02003768_4
	movs r0, #1
	negs r0, r0
	bl 0x0200bd64
	movs r0, #230
	bl 0x0200bd7c
	ldr r0, [pc, #48]
	bl 0x0200bbdc
.L_02003768_4:
	ldr r0, [r6, #8]
	ldr r1, [r6, #12]
	ldr r2, [r6, #16]
	bl 0x0200b864
.L_02003768_3:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0xfffffe83
	.4byte 0x00000309
	.4byte 0x00000302
	.4byte 0x03001e40
	.4byte 0x00000202
	.4byte 0x00000303
	.4byte 0x000002c5
	.4byte 0x00000203
	.global Func_02003864
	.thumb_func
Func_02003864:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r6, r1, #0
	mov r8, r2
	adds r5, r0, #0
	bl 0x0200bb2c
	adds r2, r0, #0
	ldr r3, [pc, #140]
	lsls r2, r2, #3
	lsrs r2, r2, #16
	adds r5, r5, r3
	lsls r2, r2, #16
	movs r3, #128
	lsls r3, r3, #13
	adds r2, r2, r6
	adds r2, r2, r3
	adds r1, r5, #0
	movs r0, #222
	mov r3, r8
	bl 0x0200bb6c
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02003864_0
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	ldr r1, [r5, #80]
	ldrb r2, [r1, #9]
	subs r3, #13
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r1, #9]
	movs r1, #9
	bl 0x0200bcb4
	movs r1, #0
	adds r0, r5, #0
	bl 0x0200bbb4
	bl 0x0200bb2c
	lsls r0, r0, #1
	lsrs r0, r0, #16
	subs r0, #1
	lsls r0, r0, #16
	str r0, [r5, #36]
	bl 0x0200bb2c
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #1
	lsrs r3, r3, #16
	subs r3, #3
	lsls r3, r3, #16
	adds r2, r5, #0
	str r3, [r5, #40]
	adds r2, #100
	movs r3, #20
	strh r3, [r2]
	subs r2, #3
	movs r3, #1
	adds r0, r5, #0
	movs r1, #1
	strb r3, [r2]
	bl 0x0200bb5c
	ldr r1, [pc, #20]
	adds r0, r5, #0
	bl 0x0200bb64
.L_02003864_0:
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0xfff80000
	.4byte 0x0200d120
	.global Func_0200390c
	.thumb_func
Func_0200390c:
	push {r5, r6, r7, lr}
	movs r0, #26
	bl 0x0200bc1c
	adds r6, r0, #0
	ldr r0, [r6, #48]
	ldr r7, [r6, #80]
	bl 0x0200bb34
	lsls r5, r0, #1
	cmp r5, #0
	ble .L_0200390c_0
	negs r5, r5
.L_0200390c_0:
	ldr r0, [r6, #48]
	bl 0x0200bb3c
	ldr r3, [r6, #56]
	lsls r0, r0, #1
	adds r3, r3, r0
	str r3, [r6, #8]
	ldr r3, [r6, #60]
	ldr r0, [r6, #48]
	movs r2, #128
	adds r3, r3, r5
	lsls r2, r2, #8
	str r3, [r6, #12]
	adds r0, r0, r2
	bl 0x0200bb3c
	asrs r0, r0, #3
	strh r0, [r7, #30]
	bl 0x0200bb2c
	adds r5, r0, #0
	bl 0x0200bb2c
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
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_02003970
	.thumb_func
Func_02003970:
	push {r5, r6, lr}
	movs r0, #11
	sub sp, #8
	bl 0x0200bc1c
	movs r3, #14
	adds r6, r0, #0
	movs r5, #9
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200bba4
	movs r3, #45
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200bba4
	cmp r6, #0
	beq .L_02003970_0
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200bbb4
	ldr r3, [r6, #12]
	ldr r2, [pc, #24]
	adds r3, r3, r2
	adds r2, r6, #0
	str r3, [r6, #12]
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
.L_02003970_0:
	ldr r0, [pc, #16]
	bl 0x0200bbdc
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0xffe00000
	.4byte 0x00000201
	.global Func_020039d4
	.thumb_func
Func_020039d4:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	ldr r0, [r6, #48]
	ldr r7, [r6, #80]
	bl 0x0200bb34
	lsls r5, r0, #1
	cmp r5, #0
	ble .L_020039d4_0
	negs r5, r5
.L_020039d4_0:
	ldr r0, [r6, #48]
	bl 0x0200bb3c
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
	bl 0x0200bb3c
	cmp r0, #0
	bge .L_020039d4_1
	adds r0, #7
.L_020039d4_1:
	asrs r3, r0, #3
	strh r3, [r7, #30]
	bl 0x0200bb2c
	adds r5, r0, #0
	bl 0x0200bb2c
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
	.global Func_02003a3c
	.thumb_func
Func_02003a3c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	bl 0x0200bc1c
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
	bl 0x0200bbb4
	movs r3, #92
	adds r3, r3, r7
	mov r2, r8
	strb r2, [r3]
	mov r10, r3
	adds r3, r7, #0
	adds r3, #85
	strb r2, [r3]
	ldr r0, [pc, #124]
	bl 0x0200bbd4
	cmp r0, #0
	bne .L_02003a3c_0
	ldr r3, [r7, #12]
	movs r2, #128
	lsls r2, r2, #14
	adds r3, r3, r2
	str r3, [r7, #12]
.L_02003a3c_0:
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
	bl 0x0200bb44
	adds r5, r0, #0
	movs r0, #181
	bl 0x0200bbcc
	movs r3, #128
	lsls r3, r3, #3
	adds r5, r5, r3
	adds r2, r5, #0
	movs r1, #128
	ldrb r0, [r6, #28]
	bl 0x0200bb54
	movs r0, #17
	bl 0x0200bb4c
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
	.4byte 0x0200b9d5
	.include "games/THE BROKEN SEAL/SRC/FIELD/KAREI_MACHI/IMPORT.INC"
	.section .rodata,"a",%progbits
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
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xffb00000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xffb80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0xff9c0000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0xff9c0000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x00de0000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000022
	.4byte 0x02008041
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00e40000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00d40000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00b20000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00a40000
	.4byte 0x00000000
	.4byte 0x01160000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00b60000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00ee0000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00f60000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000011
	.4byte 0x80000000
	.4byte 0x00000015
	.4byte 0x00000012
	.4byte 0x80000000
	.4byte 0x00000015
	.4byte 0x00000013
	.4byte 0x80000000
	.4byte 0x00000015
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x0000000e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01260000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000011
	.4byte 0x80000000
	.4byte 0x00000015
	.4byte 0x00000012
	.4byte 0x80000000
	.4byte 0x00000015
	.4byte 0x00000013
	.4byte 0x80000000
	.4byte 0x00000015
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x0000000e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00c60000
	.4byte 0x00000000
	.4byte 0x012c0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00b20000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00ff0000
	.4byte 0x00000000
	.4byte 0x01210000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01090000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x011c0000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000022
	.4byte 0x020080ad
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01840000
	.4byte 0x00000000
	.4byte 0x012c0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000222
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000222
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000006c
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0xffff0000
	.4byte 0x000000a8
	.4byte 0x400000a8
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
	.4byte 0x000000e0
	.4byte 0x40000118
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000048
	.4byte 0x400000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000198
	.4byte 0x400000b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000198
	.4byte 0x400000d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x00000198
	.4byte 0x40000198
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x00000148
	.4byte 0x400001b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x00000078
	.4byte 0x40000108
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0007
	.4byte 0x00000138
	.4byte 0x40000108
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x00000098
	.4byte 0x400001b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0009
	.4byte 0x000000e0
	.4byte 0xc00001d0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x00000038
	.4byte 0x00000110
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x000000e0
	.4byte 0x40000058
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000c
	.4byte 0x00000128
	.4byte 0x40000198
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000d
	.4byte 0x00000098
	.4byte 0x400000e8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000e
	.4byte 0x000001b8
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
	.4byte 0x000000a8
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000c0
	.4byte 0xc0000118
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00000160
	.4byte 0xffff0002
	.4byte 0x000000c0
	.4byte 0x400000e8
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00000160
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a8
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000f8
	.4byte 0x40000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000001a8
	.4byte 0xc0000208
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x000001b8
	.4byte 0xc000021c
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
	.4byte 0x000000a8
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000128
	.4byte 0x40000068
	.4byte 0x00640000
	.4byte 0x01b80032
	.4byte 0x000001bd
	.4byte 0xffff0002
	.4byte 0x00000118
	.4byte 0xc0000188
	.4byte 0x00640000
	.4byte 0x01b80032
	.4byte 0x000001bd
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a8
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000128
	.4byte 0x400001c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000278
	.4byte 0x400001c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000098
	.4byte 0x40000048
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x00000238
	.4byte 0x40000098
	.4byte 0x01860000
	.4byte 0x02760000
	.4byte 0x000000dc
	.4byte 0xffff0005
	.4byte 0x000001d8
	.4byte 0x40000048
	.4byte 0x01860000
	.4byte 0x02760000
	.4byte 0x000000dc
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a8
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000198
	.4byte 0x40000278
	.4byte 0x00550000
	.4byte 0x01d10190
	.4byte 0x00000370
	.4byte 0xffff0002
	.4byte 0x00000128
	.4byte 0x400001e8
	.4byte 0x00550000
	.4byte 0x01d10190
	.4byte 0x00000370
	.4byte 0xffff0003
	.4byte 0x000002e8
	.4byte 0x400001e8
	.4byte 0x02530000
	.4byte 0x03a20190
	.4byte 0x00000370
	.4byte 0xffff0004
	.4byte 0x00000358
	.4byte 0xc0000358
	.4byte 0x02530000
	.4byte 0x03a20190
	.4byte 0x00000370
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000063
	.4byte 0x00101064
	.4byte 0x00202064
	.4byte 0x00303064
	.4byte 0x00404064
	.4byte 0x00506064
	.4byte 0x00607064
	.4byte 0x0070a064
	.4byte 0x00801065
	.4byte 0x00914002
	.4byte 0x00a13002
	.4byte 0x00b01066
	.4byte 0x00c05064
	.4byte 0x00d08064
	.4byte 0x00e0409c
	.4byte 0x00000066
	.4byte 0x0010b063
	.4byte 0x00201067
	.4byte 0x00514067
	.4byte 0x00000099
	.4byte 0x0010209a
	.4byte 0x0023c002
	.4byte 0x00a47002
	.4byte 0x0000009a
	.4byte 0x0010509b
	.4byte 0x00201099
	.4byte 0x0000009b
	.4byte 0x0010209c
	.4byte 0x0020309c
	.4byte 0x0030409b
	.4byte 0x0040309b
	.4byte 0x0050109a
	.4byte 0x0000009c
	.4byte 0x0010b067
	.4byte 0x0020109b
	.4byte 0x0030209b
	.4byte 0x0040e063
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0099
	.4byte 0x00000001
	.4byte 0x00bc0000
	.4byte 0x00000000
	.4byte 0x01dc0000
	.4byte 0x00013000
	.4byte 0xffff0099
	.4byte 0x00000001
	.4byte 0x01040000
	.4byte 0x00000000
	.4byte 0x01dc0000
	.4byte 0x00015000
	.4byte 0xffff006c
	.4byte 0x00000002
	.4byte 0x00de0000
	.4byte 0x00000000
	.4byte 0x017e0000
	.4byte 0x00004000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00860000
	.4byte 0x00000000
	.4byte 0xffff0065
	.4byte 0x00000002
	.4byte 0x00730000
	.4byte 0x00000000
	.4byte 0x01c50000
	.4byte 0x00000000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x01e40000
	.4byte 0x00000000
	.4byte 0x01020000
	.4byte 0x00005000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x01670000
	.4byte 0x00000000
	.4byte 0x013a0000
	.4byte 0x0001d000
	.4byte 0xffff006f
	.4byte 0x00000002
	.4byte 0x01940000
	.4byte 0x00000000
	.4byte 0x010c0000
	.4byte 0x00004000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x009a0000
	.4byte 0x00000000
	.4byte 0x00770000
	.4byte 0x00000000
	.4byte 0xffff0066
	.4byte 0x00000001
	.4byte 0x00c60000
	.4byte 0x00000000
	.4byte 0x00540000
	.4byte 0x00008000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00c70000
	.4byte 0x00000000
	.4byte 0x00e90000
	.4byte 0x00018000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x002e0000
	.4byte 0x00000000
	.4byte 0x00d90000
	.4byte 0x00003000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x01d40000
	.4byte 0x00000000
	.4byte 0x01b20000
	.4byte 0x0001b000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x01d00000
	.4byte 0x00000000
	.4byte 0x01a00000
	.4byte 0x00013000
	.4byte 0xffff007c
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00015000
	.4byte 0xffff007d
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00013000
	.4byte 0xffff0076
	.4byte 0x00000001
	.4byte 0x01160000
	.4byte 0x00000000
	.4byte 0x013e0000
	.4byte 0x00018000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x014c0000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0084
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00008000
	.4byte 0xffff0094
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00008000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00008000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00008000
	.4byte 0xffff009d
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00008000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00008000
	.4byte 0xffff0042
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00008000
	.4byte 0xffff0043
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00008000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0099
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00015000
	.4byte 0xffff0099
	.4byte 0x00000001
	.4byte 0x00d40000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00013000
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
	.4byte 0x0002c000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0099
	.4byte 0x00000001
	.4byte 0x00f60000
	.4byte 0x00000000
	.4byte 0x00c60000
	.4byte 0x00023000
	.4byte 0xffff0111
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0031
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0044
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0fd60016
	.4byte 0x00000007
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x033a0000
	.4byte 0x00024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x02dc0000
	.4byte 0x00024000
	.4byte 0xffff0044
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x005b005c
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008465
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008465
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008465
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02008465
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008465
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008465
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008465
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x02008465
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000002
	.4byte 0xffff0009
	.4byte 0x02008591
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x02008591
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001a71
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001a72
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001a73
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001a74
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001a75
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001a76
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001a77
	.4byte 0x00000000
	.4byte 0x0911000f
	.4byte 0x00001a78
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001b9b
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001a79
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001a7a
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001a7b
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x020082c9
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001a7f
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00001a80
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x02008395
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x020083d9
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x02008421
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001a81
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001a82
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001a83
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001a84
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001a85
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001a86
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001a87
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001a88
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001a89
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001a8a
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001a8b
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001a8c
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001a8d
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001a8e
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001ad0
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001ad4
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00001ad6
	.4byte 0x00008c15
	.4byte 0xffff0019
	.4byte 0x00000000
	.4byte 0x00009415
	.4byte 0x0916001a
	.4byte 0x02009611
	.4byte 0x00000023
	.4byte 0x0f810064
	.4byte 0x001000e3
	.4byte 0x00000023
	.4byte 0x0f820065
	.4byte 0x001000b6
	.4byte 0x00000003
	.4byte 0xffff005a
	.4byte 0x020081e5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008505
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001b97
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001b98
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001b99
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001b9a
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x020082e9
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002587
	.4byte 0x00009415
	.4byte 0x0fd6000c
	.4byte 0x0200820d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000013
	.4byte 0x0f220064
	.4byte 0x001000c1
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000021
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008375
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000025b6
	.4byte 0x00008c15
	.4byte 0x03020008
	.4byte 0x0200b2a5
	.4byte 0x00008c15
	.4byte 0x03030009
	.4byte 0x0200b2a5
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte 0x0200b1b9
	.4byte 0x00000202
	.4byte 0xffff000b
	.4byte 0x0200b1b9
	.4byte 0x00008602
	.4byte 0xffff000e
	.4byte 0x0200b1b9
	.4byte 0x00004602
	.4byte 0xffff000f
	.4byte 0x0200b1b9
	.4byte 0x00008602
	.4byte 0xffff000f
	.4byte 0x0200b1b9
	.4byte 0x00001815
	.4byte 0x0201000b
	.4byte 0x0200b971
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x0200958d
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x020095b5
	.4byte 0x00000002
	.4byte 0xffff0012
	.4byte 0x020095f9
	.4byte 0x00000002
	.4byte 0xffff0013
	.4byte 0x020095e1
	.4byte 0x00000013
	.4byte 0x0f1c0064
	.4byte 0x00100084
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x02008031
	.4byte 0x00000013
	.4byte 0x0f1e0066
	.4byte 0x001000e3
	.4byte 0x00000013
	.4byte 0x0f1f0067
	.4byte 0x001000e5
	.4byte 0x00000013
	.4byte 0x0f200068
	.4byte 0x002000c8
	.4byte 0x00000013
	.4byte 0x0f210069
	.4byte 0x001000b7
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00210023
	.4byte 0x00020001
	.4byte 0x00250005
	.4byte 0x00010021
	.4byte 0x00050002
	.4byte 0x0000ffff
	.4byte 0x0200d0c8
	.4byte 0x000a0024
	.4byte 0x0200d0c8
	.4byte 0x00070039
	.4byte 0x0200d0c8
	.4byte 0x000b0039
	.4byte 0x0200d0c8
	.4byte 0x00170039
	.4byte 0x0200d0c8
	.4byte 0x00190034
	.4byte 0x0200d0c8
	.4byte 0x000e0027
	.4byte 0x0200d0c8
	.4byte 0x000e0033
	.4byte 0x0200d0c8
	.4byte 0x00190029
	.4byte 0x00000022
	.4byte 0x0200b6f9
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
