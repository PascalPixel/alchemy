.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/SORU_NICHIGETSU/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200aafc
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
	.4byte 0x0200abec
	.global Func_02000044
	.thumb_func
Func_02000044:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200ac14
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200ad34
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {lr}
	bl 0x0200a5b0
	cmp r0, #0
	beq .L_02000054_0
	ldr r0, [pc, #148]
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_02000054_1
	bl 0x0200a9d4
	movs r1, #1
	ldr r0, [pc, #136]
	bl 0x0200aabc
	movs r0, #20
	bl 0x0200aacc
	ldr r0, [pc, #120]
	bl 0x0200a9bc
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200a9c4
	ldr r0, [pc, #112]
	bl 0x0200a9c4
	ldr r0, [pc, #112]
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_02000054_2
	bl 0x020089f4
.L_02000054_2:
	bl 0x0200a5b0
	cmp r0, #0
	beq .L_02000054_3
	ldr r0, [pc, #92]
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_02000054_3
	bl 0x02009be8
.L_02000054_3:
	bl 0x0200a9dc
	b .L_02000054_1
.L_02000054_0:
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_02000054_1
	bl 0x0200a9d4
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl 0x0200aabc
	movs r0, #20
	bl 0x0200aacc
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200a9bc
	ldr r0, [pc, #16]
	bl 0x0200a9c4
	ldr r0, [pc, #20]
	bl 0x0200a9c4
	bl 0x0200a9dc
.L_02000054_1:
	pop {r0}
	bx r0
	.4byte 0x00000201
	.4byte 0x002051cc
	.4byte 0x00000202
	.4byte 0x0000080a
	.4byte 0x00000811
	.global Func_02000108
	.thumb_func
Func_02000108:
	push {lr}
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_02000108_0
	bl 0x0200a9d4
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl 0x0200aabc
	movs r0, #20
	bl 0x0200aacc
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200a9bc
	ldr r0, [pc, #20]
	bl 0x0200a9c4
	ldr r0, [pc, #16]
	bl 0x0200a9c4
	bl 0x0200a9dc
.L_02000108_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000201
	.4byte 0x00000202
	.global Func_02000150
	.thumb_func
Func_02000150:
	push {lr}
	bl 0x0200a5b0
	cmp r0, #0
	beq .L_02000150_0
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_02000150_1
	bl 0x0200a9d4
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl 0x0200aabc
	movs r0, #20
	bl 0x0200aacc
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200a9bc
	ldr r0, [pc, #88]
	bl 0x0200a9c4
	ldr r0, [pc, #84]
	bl 0x0200a9c4
	bl 0x0200a9dc
	b .L_02000150_1
.L_02000150_0:
	ldr r0, [pc, #68]
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_02000150_1
	bl 0x0200a9d4
	movs r1, #1
	ldr r0, [pc, #60]
	bl 0x0200aabc
	movs r0, #20
	bl 0x0200aacc
	ldr r0, [pc, #40]
	bl 0x0200a9bc
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200a9c4
	ldr r0, [pc, #32]
	bl 0x0200a9c4
	ldr r0, [pc, #32]
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_02000150_2
	bl 0x020089f4
.L_02000150_2:
	bl 0x0200a9dc
.L_02000150_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000201
	.4byte 0x00000202
	.4byte 0x002051cc
	.4byte 0x0000080a
	.global Func_020001ec
	.thumb_func
Func_020001ec:
	push {lr}
	ldr r0, [pc, #48]
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_020001ec_0
	movs r1, #1
	ldr r0, [pc, #40]
	bl 0x0200aabc
	movs r0, #20
	bl 0x0200aacc
	ldr r0, [pc, #24]
	bl 0x0200a9bc
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200a9c4
	ldr r0, [pc, #16]
	bl 0x0200a9c4
.L_020001ec_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000202
	.4byte 0x00202db1
	.4byte 0x00000201
	.global Func_0200022c
	.thumb_func
Func_0200022c:
	movs r3, #160
	movs r2, #0
	lsls r3, r3, #19
	strh r2, [r3]
	bx lr
	.2byte 0x0000
	.global Func_02000238
	.thumb_func
Func_02000238:
	push {lr}
	ldr r0, [pc, #128]
	sub sp, #8
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02000238_0
	movs r3, #2
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #45
	movs r1, #28
	movs r2, #34
	movs r3, #10
	bl 0x0200a994
.L_02000238_0:
	ldr r0, [pc, #100]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02000238_1
	movs r3, #2
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #47
	movs r1, #28
	movs r2, #36
	movs r3, #10
	bl 0x0200a994
.L_02000238_1:
	ldr r0, [pc, #72]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02000238_2
	movs r3, #2
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #45
	movs r1, #29
	movs r2, #34
	movs r3, #11
	bl 0x0200a994
.L_02000238_2:
	ldr r0, [pc, #48]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02000238_3
	movs r3, #2
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #47
	movs r1, #29
	movs r2, #36
	movs r3, #11
	bl 0x0200a994
.L_02000238_3:
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000080b
	.4byte 0x0000080c
	.4byte 0x0000080d
	.4byte 0x0000080e
	.global Func_020002cc
	.thumb_func
Func_020002cc:
	push {lr}
	ldr r0, [pc, #128]
	sub sp, #8
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_020002cc_0
	movs r3, #2
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #45
	movs r1, #28
	movs r2, #34
	movs r3, #10
	bl 0x0200a994
.L_020002cc_0:
	ldr r0, [pc, #100]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_020002cc_1
	movs r3, #2
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #47
	movs r1, #28
	movs r2, #36
	movs r3, #10
	bl 0x0200a994
.L_020002cc_1:
	ldr r0, [pc, #72]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_020002cc_2
	movs r3, #2
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #45
	movs r1, #29
	movs r2, #34
	movs r3, #11
	bl 0x0200a994
.L_020002cc_2:
	ldr r0, [pc, #48]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_020002cc_3
	movs r3, #2
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #47
	movs r1, #29
	movs r2, #36
	movs r3, #11
	bl 0x0200a994
.L_020002cc_3:
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000826
	.4byte 0x00000827
	.4byte 0x00000828
	.4byte 0x00000829
	.global Func_02000360
	.thumb_func
Func_02000360:
	push {lr}
	ldr r0, [pc, #128]
	sub sp, #8
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02000360_0
	movs r3, #2
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #45
	movs r1, #30
	movs r2, #34
	movs r3, #10
	bl 0x0200a994
.L_02000360_0:
	ldr r0, [pc, #100]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02000360_1
	movs r3, #2
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #47
	movs r1, #30
	movs r2, #36
	movs r3, #10
	bl 0x0200a994
.L_02000360_1:
	ldr r0, [pc, #72]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02000360_2
	movs r3, #2
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #45
	movs r1, #31
	movs r2, #34
	movs r3, #11
	bl 0x0200a994
.L_02000360_2:
	ldr r0, [pc, #48]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02000360_3
	movs r3, #2
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #47
	movs r1, #31
	movs r2, #36
	movs r3, #11
	bl 0x0200a994
.L_02000360_3:
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000080b
	.4byte 0x0000080c
	.4byte 0x0000080d
	.4byte 0x0000080e
	.global Func_020003f4
	.thumb_func
Func_020003f4:
	push {lr}
	ldr r0, [pc, #128]
	sub sp, #8
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_020003f4_0
	movs r3, #2
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #45
	movs r1, #30
	movs r2, #34
	movs r3, #10
	bl 0x0200a994
.L_020003f4_0:
	ldr r0, [pc, #100]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_020003f4_1
	movs r3, #2
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #47
	movs r1, #30
	movs r2, #36
	movs r3, #10
	bl 0x0200a994
.L_020003f4_1:
	ldr r0, [pc, #72]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_020003f4_2
	movs r3, #2
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #45
	movs r1, #31
	movs r2, #34
	movs r3, #11
	bl 0x0200a994
.L_020003f4_2:
	ldr r0, [pc, #48]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_020003f4_3
	movs r3, #2
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #47
	movs r1, #31
	movs r2, #36
	movs r3, #11
	bl 0x0200a994
.L_020003f4_3:
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000826
	.4byte 0x00000827
	.4byte 0x00000828
	.4byte 0x00000829
	.global Func_02000488
	.thumb_func
Func_02000488:
	push {lr}
	ldr r0, [pc, #912]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02000488_0
	bl 0x02008054
.L_02000488_0:
	ldr r0, [pc, #900]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02000488_2
	b .L_02000488_3
.L_02000488_2:
	bl 0x0200a9d4
	ldr r0, [pc, #888]
	bl 0x0200aa54
	movs r0, #17
	bl 0x0200aaf4
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200a9f4
	movs r1, #144
	movs r2, #232
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200aa0c
	movs r1, #0
	movs r0, #0
	bl 0x0200aa24
	movs r0, #20
	bl 0x0200a9cc
	movs r0, #21
	bl 0x0200aaf4
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
.L_02000488_1:
	bl 0x0200aa74
	movs r0, #0
	bl 0x0200a9ec
	cmp r0, #0
	beq .L_02000488_4
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #16
	bl 0x0200aa1c
.L_02000488_4:
	movs r0, #16
	ldr r1, [pc, #800]
	ldr r2, [pc, #804]
	bl 0x0200a9f4
	movs r1, #144
	lsls r1, r1, #1
	movs r2, #206
	movs r0, #16
	bl 0x0200aa0c
	movs r0, #40
	bl 0x0200a9cc
	movs r1, #128
	movs r0, #16
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200aa7c
	movs r0, #16
	movs r1, #4
	movs r2, #60
	bl 0x0200aa34
	movs r0, #16
	movs r1, #20
	bl 0x0200a5fc
	movs r0, #0
	bl 0x0200a9ec
	cmp r0, #0
	beq .L_02000488_5
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200aa1c
.L_02000488_5:
	movs r0, #0
	bl 0x0200a9ec
	cmp r0, #0
	beq .L_02000488_6
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #5
	bl 0x0200aa1c
.L_02000488_6:
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200a9f4
	movs r1, #128
	movs r2, #128
	movs r0, #5
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200a9f4
	movs r1, #140
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #248
	bl 0x0200aa04
	movs r1, #148
	movs r2, #248
	movs r0, #5
	lsls r1, r1, #1
	bl 0x0200aa0c
	movs r0, #1
	movs r1, #1
	bl 0x0200aa24
	movs r1, #208
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aa74
	movs r1, #176
	movs r2, #30
	movs r0, #5
	lsls r1, r1, #8
	bl 0x0200aa74
	ldr r0, [pc, #628]
	ldr r1, [pc, #632]
	bl 0x0200aa94
	movs r0, #144
	movs r1, #1
	movs r2, #213
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	bl 0x0200aa9c
	movs r0, #16
	ldr r1, [pc, #608]
	ldr r2, [pc, #612]
	bl 0x0200a9f4
	movs r1, #144
	movs r2, #176
	lsls r1, r1, #1
	movs r0, #16
	bl 0x0200aa0c
	movs r0, #40
	bl 0x0200a9cc
	movs r0, #16
	movs r1, #2
	bl 0x0200aa3c
	movs r0, #16
	movs r1, #6
	bl 0x0200a5fc
	movs r1, #128
	movs r2, #60
	movs r0, #16
	lsls r1, r1, #7
	bl 0x0200aa74
	movs r0, #16
	movs r1, #20
	bl 0x0200a5fc
	movs r2, #40
	movs r0, #16
	movs r1, #0
	bl 0x0200aa74
	movs r1, #3
	movs r0, #16
	bl 0x0200aa2c
	movs r0, #10
	bl 0x0200a9cc
	movs r1, #128
	movs r2, #40
	movs r0, #16
	lsls r1, r1, #8
	bl 0x0200aa74
	movs r1, #3
	movs r0, #16
	bl 0x0200aa2c
	movs r0, #20
	bl 0x0200a9cc
	movs r0, #5
	movs r1, #2
	bl 0x0200aa3c
	movs r1, #144
	movs r2, #10
	movs r0, #5
	lsls r1, r1, #8
	bl 0x0200aa74
	movs r0, #5
	movs r1, #10
	bl 0x0200a5fc
	movs r0, #1
	movs r1, #2
	bl 0x0200aa3c
	movs r1, #240
	movs r2, #10
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200aa74
	movs r0, #1
	movs r1, #6
	bl 0x0200a5fc
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #5
	bl 0x0200aa84
	movs r0, #40
	bl 0x0200a9cc
	movs r1, #160
	movs r2, #10
	movs r0, #5
	lsls r1, r1, #8
	bl 0x0200aa74
	ldr r0, [pc, #420]
	movs r1, #10
	bl 0x0200a5fc
	movs r1, #2
	movs r0, #16
	bl 0x0200aa3c
	movs r0, #10
	bl 0x0200a9cc
	movs r1, #160
	movs r2, #20
	movs r0, #16
	lsls r1, r1, #8
	bl 0x0200aa74
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #16
	bl 0x0200aa84
	movs r0, #20
	bl 0x0200a9cc
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #40
	bl 0x0200aa74
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aa74
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aa74
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200aa74
	movs r2, #20
	movs r0, #1
	ldr r1, [pc, #324]
	bl 0x0200aa7c
	movs r1, #0
	movs r0, #1
	bl 0x0200aa64
	movs r0, #60
	bl 0x0200a9cc
	movs r1, #4
	movs r0, #16
	bl 0x0200aa2c
	movs r0, #40
	bl 0x0200a9cc
	movs r0, #16
	movs r1, #20
	bl 0x0200a5fc
	movs r2, #40
	movs r0, #5
	ldr r1, [pc, #276]
	bl 0x0200aa7c
	movs r0, #5
	movs r1, #60
	bl 0x0200a5fc
	movs r0, #16
	movs r1, #3
	bl 0x0200aa2c
	movs r0, #16
	movs r1, #10
	bl 0x0200a5fc
	movs r0, #0
	ldr r1, [pc, #248]
	movs r2, #0
	bl 0x0200aa7c
	movs r0, #1
	ldr r1, [pc, #240]
	movs r2, #0
	bl 0x0200aa7c
	movs r2, #60
	movs r0, #5
	ldr r1, [pc, #228]
	bl 0x0200aa7c
	movs r1, #2
	movs r0, #1
	bl 0x0200aa3c
	movs r0, #20
	bl 0x0200a9cc
	movs r0, #1
	movs r1, #10
	bl 0x0200a5fc
	movs r1, #3
	movs r0, #16
	bl 0x0200aa2c
	movs r0, #20
	bl 0x0200a9cc
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200aa7c
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200aa7c
	movs r1, #129
	movs r0, #5
	lsls r1, r1, #1
	movs r2, #80
	bl 0x0200aa7c
	movs r2, #80
	movs r0, #16
	ldr r1, [pc, #144]
	bl 0x0200aa7c
	movs r0, #16
	movs r1, #6
	bl 0x0200a5fc
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200aa74
	movs r1, #240
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aa74
	movs r1, #144
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #60
	bl 0x0200aa74
	movs r1, #128
	movs r2, #10
	movs r0, #16
	lsls r1, r1, #7
	bl 0x0200aa74
	movs r1, #3
	movs r0, #16
	bl 0x0200aa3c
	movs r0, #6
	bl 0x0200a9cc
	movs r1, #0
	movs r0, #16
	bl 0x0200aa5c
	movs r0, #0
	movs r1, #0
	bl 0x0200a9e4
	cmp r0, #0
	bne .L_02000488_7
	ldr r0, [pc, #52]
	bl 0x0200aa54
	b .L_02000488_8
	.4byte 0x00000814
	.4byte 0x00000809
	.4byte 0x00000fe3
	.4byte 0x00016666
	.4byte 0x0000b333
	.4byte 0x00009999
	.4byte 0x00001333
	.4byte 0x00006666
	.4byte 0x00003333
	.4byte 0x00002005
	.4byte 0x00000101
	.4byte 0x00000105
	.4byte 0x00000ff0
.L_02000488_7:
	ldr r0, [pc, #396]
	bl 0x0200aa54
	movs r0, #16
	ldr r1, [pc, #392]
	movs r2, #20
	bl 0x0200aa7c
.L_02000488_8:
	movs r0, #16
	movs r1, #4
	movs r2, #20
	bl 0x0200aa34
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aa74
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aa74
	movs r1, #160
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #8
	bl 0x0200aa74
	movs r1, #6
	movs r0, #16
	bl 0x0200a5fc
	ldr r0, [pc, #336]
	bl 0x0200aa54
	movs r0, #30
	bl 0x0200a9cc
	movs r0, #5
	movs r1, #4
	bl 0x0200aa2c
	ldr r0, [pc, #320]
	movs r1, #6
	bl 0x0200a5fc
	movs r0, #1
	movs r1, #3
	bl 0x0200aa2c
	movs r0, #1
	movs r1, #20
	bl 0x0200a5fc
	movs r0, #16
	movs r1, #6
	movs r2, #20
	bl 0x0200aa34
	movs r1, #130
	movs r2, #20
	movs r0, #16
	lsls r1, r1, #1
	bl 0x0200aa7c
	movs r0, #16
	movs r1, #30
	bl 0x0200a5fc
	movs r0, #0
	movs r1, #3
	bl 0x0200aa24
	movs r0, #1
	movs r1, #3
	bl 0x0200aa24
	movs r1, #3
	movs r0, #5
	bl 0x0200aa2c
	movs r0, #20
	bl 0x0200a9cc
	movs r0, #16
	movs r1, #3
	bl 0x0200aa2c
	movs r0, #16
	movs r1, #6
	bl 0x0200a5fc
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200a9f4
	movs r1, #128
	movs r2, #128
	movs r0, #5
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200a9f4
	movs r1, #128
	movs r2, #128
	movs r0, #16
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200a9f4
	movs r0, #16
	movs r1, #2
	bl 0x0200aa24
	movs r0, #0
	bl 0x0200a9ec
	cmp r0, #0
	beq .L_02000488_9
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #16
	bl 0x0200a9fc
.L_02000488_9:
	movs r0, #16
	bl 0x0200aa14
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl 0x0200aa1c
	movs r0, #1
	movs r1, #2
	bl 0x0200aa24
	movs r0, #0
	bl 0x0200a9ec
	cmp r0, #0
	beq .L_02000488_10
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x0200a9fc
.L_02000488_10:
	movs r0, #1
	bl 0x0200aa14
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200aa1c
	movs r0, #5
	movs r1, #2
	bl 0x0200aa24
	movs r0, #0
	bl 0x0200a9ec
	cmp r0, #0
	beq .L_02000488_11
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl 0x0200a9fc
.L_02000488_11:
	movs r0, #5
	bl 0x0200aa14
	movs r1, #0
	movs r2, #0
	movs r0, #5
	bl 0x0200aa1c
	movs r0, #162
	lsls r0, r0, #1
	bl 0x0200a9bc
	ldr r0, [pc, #28]
	bl 0x0200a9bc
	bl 0x0200a9dc
.L_02000488_3:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000ff1
	.4byte 0x00000107
	.4byte 0x00000ff2
	.4byte 0x00002005
	.4byte 0x00000809
	.global Func_020009f4
	.thumb_func
Func_020009f4:
	push {r5, r6, lr}
	ldr r0, [pc, #472]
	bl 0x0200aa54
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200a9f4
	movs r1, #244
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #176
	bl 0x0200aa0c
	movs r0, #0
	movs r1, #0
	bl 0x0200aa24
	movs r0, #0
	bl 0x0200a9ec
	cmp r0, #0
	beq .L_020009f4_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #16
	bl 0x0200aa1c
.L_020009f4_0:
	movs r0, #0
	movs r1, #0
	movs r2, #1
	bl 0x0200aa74
	movs r1, #128
	movs r2, #128
	movs r0, #16
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200a9f4
	movs r1, #236
	movs r0, #16
	lsls r1, r1, #1
	movs r2, #168
	bl 0x0200aa0c
	movs r0, #16
	movs r1, #0
	movs r2, #60
	bl 0x0200aa74
	movs r2, #40
	movs r0, #16
	movs r1, #4
	bl 0x0200aa34
	movs r0, #16
	movs r1, #6
	bl 0x0200a5fc
	ldr r0, [pc, #352]
	ldr r1, [pc, #352]
	bl 0x0200aa94
	movs r1, #1
	movs r2, #181
	lsls r2, r2, #16
	movs r3, #1
	negs r1, r1
	ldr r0, [pc, #340]
	bl 0x0200aa9c
	bl 0x0200aaa4
.L_02000a8e:
	movs r0, #120
	bl 0x0200a9cc
	ldr r0, [pc, #328]
	movs r1, #80
	bl 0x0200a5fc
	movs r0, #246
	movs r1, #1
	movs r2, #168
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #17
	bl 0x0200aa9c
	bl 0x0200aaa4
	movs r0, #20
	bl 0x0200a9cc
	ldr r5, [pc, #296]
	movs r1, #192
	movs r2, #20
	movs r0, #16
	lsls r1, r1, #6
	bl 0x0200aa74
	adds r0, r5, #0
	movs r1, #6
	bl 0x0200a5fc
	movs r2, #60
	movs r0, #16
	movs r1, #0
	bl 0x0200aa74
	movs r0, #16
	movs r1, #2
	bl 0x0200aa3c
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #6
	movs r2, #10
	bl 0x0200aa74
	movs r1, #0
	adds r0, r5, #0
	bl 0x0200aa5c
	movs r0, #0
	movs r1, #0
	bl 0x0200a9e4
	cmp r0, #0
	bne .L_02000a8e_0
	ldr r0, [pc, #228]
	bl 0x0200aa54
	b .L_02000a8e_1
.L_02000a8e_0:
	ldr r0, [pc, #224]
	bl 0x0200aa54
.L_02000a8e_1:
	ldr r5, [pc, #212]
	movs r1, #160
	movs r2, #10
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200aa74
	movs r1, #10
	adds r0, r5, #0
	bl 0x0200a5fc
	ldr r6, [pc, #200]
	adds r0, r6, #0
	bl 0x0200aa54
	movs r0, #16
	movs r1, #0
	movs r2, #40
	bl 0x0200aa74
	movs r2, #40
	movs r0, #16
	ldr r1, [pc, #184]
	bl 0x0200aa7c
	movs r0, #16
	movs r1, #4
	bl 0x0200aa2c
	movs r1, #192
	movs r2, #10
	movs r0, #16
	lsls r1, r1, #6
	bl 0x0200aa74
	movs r0, #16
	movs r1, #4
	bl 0x0200aa24
	movs r1, #0
	adds r0, r5, #0
	bl 0x0200aa5c
	movs r0, #0
	movs r1, #0
	bl 0x0200a9e4
	cmp r0, #0
	bne .L_02000a8e_2
	adds r0, r6, #1
	bl 0x0200aa54
	ldr r0, [pc, #128]
	bl 0x0200a9bc
	b .L_02000a8e_3
.L_02000a8e_2:
	adds r0, r6, #2
	bl 0x0200aa54
.L_02000a8e_3:
	ldr r0, [pc, #92]
	movs r1, #4
	bl 0x0200a5fc
	movs r0, #16
	movs r1, #1
	bl 0x0200aa8c
	movs r1, #243
	movs r0, #16
	lsls r1, r1, #1
	movs r2, #131
	bl 0x0200aa0c
	movs r1, #144
	movs r0, #16
	lsls r1, r1, #2
	movs r2, #120
	bl 0x0200aa0c
	movs r1, #192
	movs r2, #2
	movs r0, #16
	lsls r1, r1, #8
	bl 0x0200aa74
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl 0x0200aa94
.L_02000bc4:
	ldr r0, [pc, #52]
	bl 0x0200a9bc
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0ff6
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0002
	.2byte 0x4ccc
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x023f
	.2byte 0x1010
	.2byte 0x0000
	.2byte 0x4010
	.2byte 0x0000
	.2byte 0x0ffa
	.2byte 0x0000
	.2byte 0x0ffb
	.2byte 0x0000
	.2byte 0x0ffc
	.2byte 0x0000
	.2byte 0x0105
	.2byte 0x0000
	.2byte 0x0896
	.2byte 0x0000
	.4byte 0x0000080a
	.global Func_02000c00
	.thumb_func
Func_02000c00:
	push {r5, lr}
	movs r0, #16
	bl 0x0200a9ec
	adds r5, r0, #0
	ldr r0, [pc, #380]
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_02000c00_0
	b .L_02000c00_1
.L_02000c00_0:
	ldr r0, [pc, #372]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02000c00_2
	bl 0x02008108
	b .L_02000c00_1
.L_02000c00_2:
	ldr r0, [pc, #360]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02000c00_4
	b .L_02000c00_1
.L_02000c00_4:
	bl 0x0200a9d4
	movs r0, #0
	movs r1, #0
	bl 0x0200aa24
	ldr r0, [pc, #340]
	bl 0x0200aa54
	movs r0, #129
	lsls r0, r0, #4
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_02000c00_5
	ldr r0, [pc, #324]
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_02000c00_6
.L_02000c00_5:
	movs r0, #0
	bl 0x0200a9ec
	cmp r0, #0
	beq .L_02000c00_7
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #16
	bl 0x0200aa1c
.L_02000c00_7:
	movs r0, #4
	bl 0x0200a9cc
	movs r1, #128
	movs r2, #128
	movs r0, #16
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200a9f4
	b .L_02000c00_8
.L_02000c00_6:
	movs r0, #129
	lsls r0, r0, #4
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_02000c00_9
	movs r2, #170
	ldr r3, [r5, #8]
	lsls r2, r2, #17
	cmp r3, r2
	ble .L_02000c00_8
.L_02000c00_9:
	movs r1, #196
	movs r2, #168
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #16
	bl 0x0200aa1c
	movs r0, #4
	bl 0x0200a9cc
	movs r1, #128
	movs r2, #128
	movs r0, #16
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200a9f4
.L_02000c00_8:
	movs r0, #129
	lsls r0, r0, #4
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_02000c00_10
	movs r2, #170
	ldr r3, [r5, #8]
	lsls r2, r2, #17
	cmp r3, r2
	ble .L_02000c00_11
.L_02000c00_10:
	movs r1, #144
	movs r0, #16
	lsls r1, r1, #1
	movs r2, #232
	bl 0x0200aa0c
	b .L_02000c00_12
.L_02000c00_11:
	ldr r0, [pc, #180]
	bl 0x0200a9b4
.L_02000c00_12:
	movs r1, #144
	movs r0, #16
	lsls r1, r1, #1
	movs r2, #232
	bl 0x0200aa0c
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aa74
	movs r1, #128
	movs r0, #16
	lsls r1, r1, #7
	movs r2, #10
	bl 0x0200aa74
	movs r0, #16
	movs r1, #0
	movs r2, #10
	bl 0x0200aa6c
	movs r0, #0
	movs r1, #3
	bl 0x0200aa2c
	movs r0, #129
	lsls r0, r0, #4
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_02000c00_13
	ldr r0, [pc, #108]
.L_02000c00_3:
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_02000c00_14
.L_02000c00_13:
	movs r0, #16
	movs r1, #2
	bl 0x0200aa24
	movs r0, #0
	bl 0x0200a9ec
	cmp r0, #0
	beq .L_02000c00_15
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #16
	bl 0x0200a9fc
.L_02000c00_15:
	movs r0, #16
	bl 0x0200aa14
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl 0x0200aa1c
	movs r1, #144
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #232
	bl 0x0200aa0c
	b .L_02000c00_16
.L_02000c00_14:
	movs r1, #144
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #248
	bl 0x0200aa0c
.L_02000c00_16:
	bl 0x0200a9dc
.L_02000c00_1:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000809
	.4byte 0x00000814
	.4byte 0x00000819
	.4byte 0x00001000
	.4byte 0x0000080a
	.global Func_02000d9c
	.thumb_func
Func_02000d9c:
	push {r5, r6, lr}
	sub sp, #8
	bl 0x0200a984
	movs r3, #3
	ands r0, r3
	cmp r0, #0
	bne .L_02000d9c_0
	b .L_02000d9c_1
.L_02000d9c_0:
	ldr r6, [pc, #284]
	ldrh r5, [r6]
	cmp r5, #2
	beq .L_02000d9c_2
	cmp r5, #2
	bgt .L_02000d9c_3
	cmp r5, #0
	beq .L_02000d9c_4
	cmp r5, #1
	beq .L_02000d9c_5
	b .L_02000d9c_6
.L_02000d9c_3:
	cmp r5, #4
	beq .L_02000d9c_7
	cmp r5, #4
	blt .L_02000d9c_8
	cmp r5, #80
	beq .L_02000d9c_9
	b .L_02000d9c_6
.L_02000d9c_4:
	movs r0, #187
	bl 0x0200aaf4
	movs r3, #1
	movs r2, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #46
	movs r1, #59
	movs r2, #30
	movs r3, #33
	bl 0x0200a994
	b .L_02000d9c_6
.L_02000d9c_5:
	movs r0, #46
	movs r1, #59
	movs r2, #30
	movs r3, #33
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a994
	movs r3, #5
	str r3, [sp, #4]
	movs r0, #46
	movs r1, #59
	movs r2, #30
	movs r3, #34
	b .L_02000d9c_10
.L_02000d9c_2:
	movs r5, #1
	movs r0, #46
	movs r1, #59
	movs r2, #30
	movs r3, #34
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a994
	movs r3, #5
	str r3, [sp, #4]
	movs r0, #46
	movs r1, #59
	movs r2, #30
	movs r3, #35
	b .L_02000d9c_10
.L_02000d9c_8:
	movs r5, #1
	movs r0, #46
	movs r1, #59
	movs r2, #30
	movs r3, #35
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a994
	movs r3, #5
	str r3, [sp, #4]
	movs r0, #46
	movs r1, #59
	movs r2, #30
	movs r3, #36
	b .L_02000d9c_10
.L_02000d9c_7:
	ldr r2, [pc, #128]
	movs r3, #2
	str r3, [r2]
	movs r5, #1
	movs r0, #46
	movs r1, #59
	movs r2, #30
	movs r3, #36
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a994
	movs r3, #5
	str r3, [sp, #4]
	movs r0, #46
	movs r1, #59
	movs r2, #30
	movs r3, #37
.L_02000d9c_10:
	str r5, [sp, #0]
	bl 0x0200a994
	b .L_02000d9c_6
.L_02000d9c_9:
	movs r3, #1
	movs r2, #10
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #46
	movs r1, #49
	movs r2, #30
	movs r3, #33
	bl 0x0200a994
.L_02000d9c_6:
	ldrh r3, [r6]
	adds r3, #1
	strh r3, [r6]
	ldrh r5, [r6]
	bl 0x0200a984
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #3
	lsrs r3, r3, #16
	adds r3, #90
	cmp r5, r3
	bls .L_02000d9c_1
	ldr r3, [pc, #32]
	strh r3, [r6]
.L_02000d9c_1:
	ldr r5, [pc, #36]
	ldr r3, [r5]
	cmp r3, #0
	beq .L_02000d9c_11
	cmp r3, #2
	bne .L_02000d9c_12
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl 0x0200a9ac
	b .L_02000d9c_13
	.4byte 0x00000000
	.4byte 0x0200ade4
	.4byte 0x0200ade8
.L_02000d9c_12:
	cmp r3, #1
	bne .L_02000d9c_13
	movs r0, #1
	movs r1, #1
	negs r0, r0
	negs r1, r1
	ldr r2, [pc, #16]
	bl 0x0200a9ac
.L_02000d9c_13:
	ldr r3, [r5]
	subs r3, #1
	str r3, [r5]
.L_02000d9c_11:
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x0000e666
	.global Func_02000ef8
	.thumb_func
Func_02000ef8:
	push {r5, r6, lr}
	sub sp, #8
	bl 0x0200a984
	movs r3, #3
	ands r0, r3
	cmp r0, #0
	bne .L_02000ef8_0
	b .L_02000ef8_1
.L_02000ef8_0:
	ldr r6, [pc, #264]
	ldrh r5, [r6]
	cmp r5, #2
	beq .L_02000ef8_2
	cmp r5, #2
	bgt .L_02000ef8_3
	cmp r5, #0
	beq .L_02000ef8_4
	cmp r5, #1
	beq .L_02000ef8_5
	b .L_02000ef8_6
.L_02000ef8_3:
	cmp r5, #4
	beq .L_02000ef8_7
	cmp r5, #4
	blt .L_02000ef8_8
	cmp r5, #90
	beq .L_02000ef8_9
	b .L_02000ef8_6
.L_02000ef8_4:
	movs r0, #187
	bl 0x0200aaf4
	movs r3, #1
	movs r2, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #47
	movs r1, #59
	movs r2, #42
	movs r3, #33
	bl 0x0200a994
	b .L_02000ef8_6
.L_02000ef8_5:
	movs r0, #47
	movs r1, #59
	movs r2, #42
	movs r3, #33
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a994
	movs r3, #5
	str r3, [sp, #4]
	movs r0, #47
	movs r1, #59
	movs r2, #42
	movs r3, #34
	b .L_02000ef8_10
.L_02000ef8_2:
	movs r5, #1
	movs r0, #47
	movs r1, #59
	movs r2, #42
	movs r3, #34
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a994
	movs r3, #5
	str r3, [sp, #4]
	movs r0, #47
	movs r1, #59
	movs r2, #42
	movs r3, #35
	b .L_02000ef8_10
.L_02000ef8_8:
	movs r5, #1
	movs r0, #47
	movs r1, #59
	movs r2, #42
	movs r3, #35
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a994
	movs r3, #5
	str r3, [sp, #4]
	movs r0, #47
	movs r1, #59
	movs r2, #42
	movs r3, #36
	b .L_02000ef8_10
.L_02000ef8_7:
	ldr r2, [pc, #108]
	movs r3, #2
	str r3, [r2]
	movs r5, #1
	movs r0, #47
	movs r1, #59
	movs r2, #42
	movs r3, #36
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a994
	movs r3, #5
	str r3, [sp, #4]
	movs r0, #47
	movs r1, #59
	movs r2, #42
	movs r3, #37
.L_02000ef8_10:
	str r5, [sp, #0]
	bl 0x0200a994
	b .L_02000ef8_6
.L_02000ef8_9:
	movs r3, #1
	movs r2, #10
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #47
	movs r1, #49
	movs r2, #42
	movs r3, #33
	bl 0x0200a994
.L_02000ef8_6:
	ldrh r3, [r6]
	adds r3, #1
	strh r3, [r6]
	ldrh r5, [r6]
	bl 0x0200a984
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #3
	lsrs r3, r3, #16
	adds r3, #100
	cmp r5, r3
	bls .L_02000ef8_1
	ldr r3, [pc, #12]
	strh r3, [r6]
.L_02000ef8_1:
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0200addc
	.4byte 0x0200ade8
	.global Func_0200101c
	.thumb_func
Func_0200101c:
	push {r5, r6, lr}
	sub sp, #8
	bl 0x0200a984
	movs r3, #3
	ands r0, r3
	cmp r0, #0
	bne .L_0200101c_0
	b .L_0200101c_1
.L_0200101c_0:
	ldr r6, [pc, #264]
	ldrh r5, [r6]
	cmp r5, #2
	beq .L_0200101c_2
	cmp r5, #2
	bgt .L_0200101c_3
	cmp r5, #0
	beq .L_0200101c_4
	cmp r5, #1
	beq .L_0200101c_5
	b .L_0200101c_6
.L_0200101c_3:
	cmp r5, #4
	beq .L_0200101c_7
	cmp r5, #4
	blt .L_0200101c_8
	cmp r5, #95
	beq .L_0200101c_9
	b .L_0200101c_6
.L_0200101c_4:
	movs r0, #187
	bl 0x0200aaf4
	movs r3, #1
	movs r2, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #48
	movs r1, #59
	movs r2, #31
	movs r3, #36
	bl 0x0200a994
	b .L_0200101c_6
.L_0200101c_5:
	movs r0, #48
	movs r1, #59
	movs r2, #31
	movs r3, #36
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a994
	movs r3, #5
	str r3, [sp, #4]
	movs r0, #48
	movs r1, #59
	movs r2, #31
	movs r3, #37
	b .L_0200101c_10
.L_0200101c_2:
	movs r5, #1
	movs r0, #48
	movs r1, #59
	movs r2, #31
	movs r3, #37
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a994
	movs r3, #5
	str r3, [sp, #4]
	movs r0, #48
	movs r1, #59
	movs r2, #31
	movs r3, #38
	b .L_0200101c_10
.L_0200101c_8:
	movs r5, #1
	movs r0, #48
	movs r1, #59
	movs r2, #31
	movs r3, #38
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a994
	movs r3, #5
	str r3, [sp, #4]
	movs r0, #48
	movs r1, #59
	movs r2, #31
	movs r3, #39
	b .L_0200101c_10
.L_0200101c_7:
	ldr r2, [pc, #108]
	movs r3, #2
	str r3, [r2]
	movs r5, #1
	movs r0, #48
	movs r1, #59
	movs r2, #31
	movs r3, #39
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a994
	movs r3, #5
	str r3, [sp, #4]
	movs r0, #48
	movs r1, #59
	movs r2, #31
	movs r3, #40
.L_0200101c_10:
	str r5, [sp, #0]
	bl 0x0200a994
	b .L_0200101c_6
.L_0200101c_9:
	movs r3, #1
	movs r2, #10
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #48
	movs r1, #49
	movs r2, #31
	movs r3, #36
	bl 0x0200a994
.L_0200101c_6:
	ldrh r3, [r6]
	adds r3, #1
	strh r3, [r6]
	ldrh r5, [r6]
	bl 0x0200a984
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #3
	lsrs r3, r3, #16
	adds r3, #105
	cmp r5, r3
	bls .L_0200101c_1
	ldr r3, [pc, #12]
	strh r3, [r6]
.L_0200101c_1:
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0200ade0
	.4byte 0x0200ade8
	.global Func_02001140
	.thumb_func
Func_02001140:
	push {r5, r6, lr}
	sub sp, #8
	bl 0x0200a984
	movs r3, #3
	ands r0, r3
	cmp r0, #0
	bne .L_02001140_0
	b .L_02001140_1
.L_02001140_0:
	ldr r6, [pc, #264]
	ldrh r5, [r6]
	cmp r5, #2
	beq .L_02001140_2
	cmp r5, #2
	bgt .L_02001140_3
	cmp r5, #0
	beq .L_02001140_4
	cmp r5, #1
	beq .L_02001140_5
	b .L_02001140_6
.L_02001140_3:
	cmp r5, #4
	beq .L_02001140_7
	cmp r5, #4
	blt .L_02001140_8
	cmp r5, #85
	beq .L_02001140_9
	b .L_02001140_6
.L_02001140_4:
	movs r0, #187
	bl 0x0200aaf4
	movs r3, #1
	movs r2, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #46
	movs r1, #59
	movs r2, #41
	movs r3, #36
	bl 0x0200a994
	b .L_02001140_6
.L_02001140_5:
	movs r0, #46
	movs r1, #59
	movs r2, #41
	movs r3, #36
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a994
	movs r3, #5
	str r3, [sp, #4]
	movs r0, #46
	movs r1, #59
	movs r2, #41
	movs r3, #37
	b .L_02001140_10
.L_02001140_2:
	movs r5, #1
	movs r0, #46
	movs r1, #59
	movs r2, #41
	movs r3, #37
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a994
	movs r3, #5
	str r3, [sp, #4]
	movs r0, #46
	movs r1, #59
	movs r2, #41
	movs r3, #38
	b .L_02001140_10
.L_02001140_8:
	movs r5, #1
	movs r0, #46
	movs r1, #59
	movs r2, #41
	movs r3, #38
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a994
	movs r3, #5
	str r3, [sp, #4]
	movs r0, #46
	movs r1, #59
	movs r2, #41
	movs r3, #39
	b .L_02001140_10
.L_02001140_7:
	ldr r2, [pc, #108]
	movs r3, #2
	str r3, [r2]
	movs r5, #1
	movs r0, #46
	movs r1, #59
	movs r2, #41
	movs r3, #39
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a994
	movs r3, #5
	str r3, [sp, #4]
	movs r0, #46
	movs r1, #59
	movs r2, #41
	movs r3, #40
.L_02001140_10:
	str r5, [sp, #0]
	bl 0x0200a994
	b .L_02001140_6
.L_02001140_9:
	movs r3, #1
	movs r2, #10
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #46
	movs r1, #49
	movs r2, #41
	movs r3, #36
	bl 0x0200a994
.L_02001140_6:
	ldrh r3, [r6]
	adds r3, #1
	strh r3, [r6]
	ldrh r5, [r6]
	bl 0x0200a984
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #3
	lsrs r3, r3, #16
	adds r3, #95
	cmp r5, r3
	bls .L_02001140_1
	ldr r3, [pc, #12]
	strh r3, [r6]
.L_02001140_1:
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0200adec
	.4byte 0x0200ade8
	.global Func_02001264
	.thumb_func
Func_02001264:
	push {r5, r6, lr}
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r0, r0
	negs r1, r1
	negs r2, r2
	movs r3, #0
	sub sp, #8
	bl 0x0200aa9c
	movs r6, #8
	movs r5, #3
	movs r0, #30
	movs r1, #43
	movs r2, #32
	movs r3, #40
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a994
	movs r3, #1
	str r3, [sp, #4]
	movs r0, #30
	movs r1, #43
	movs r2, #33
	movs r3, #39
	str r6, [sp, #0]
	bl 0x0200a994
	movs r0, #30
	movs r1, #43
	movs r2, #36
	movs r3, #38
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a994
	movs r3, #4
	str r3, [sp, #4]
	movs r0, #14
	movs r1, #41
	movs r2, #32
	movs r3, #41
	str r6, [sp, #0]
	bl 0x0200a994
	movs r1, #1
	movs r2, #158
	movs r3, #0
	ldr r0, [pc, #156]
	negs r1, r1
	lsls r2, r2, #16
	bl 0x0200aa9c
	bl 0x0200a98c
	movs r2, #240
	movs r0, #16
	ldr r1, [pc, #140]
	lsls r2, r2, #15
	bl 0x0200aa1c
	movs r2, #0
	movs r1, #0
	movs r0, #0
	bl 0x0200aa1c
	movs r0, #1
	bl 0x0200a96c
	movs r1, #1
	ldr r0, [pc, #116]
	bl 0x0200aabc
	movs r0, #20
	bl 0x0200aacc
	ldr r0, [pc, #108]
	bl 0x0200a9bc
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200a9c4
	ldr r0, [pc, #100]
	bl 0x0200a9c4
	ldr r3, [pc, #96]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	subs r3, #192
	str r3, [r2]
	adds r3, #200
	adds r2, r1, r3
	movs r3, #32
	str r3, [r2]
	bl 0x0200aad4
	bl 0x0200aae4
	movs r0, #40
	bl 0x0200a9cc
	movs r0, #171
	bl 0x0200aaf4
	movs r1, #1
	ldr r0, [pc, #56]
	bl 0x0200aabc
	movs r0, #8
	bl 0x0200aacc
	movs r0, #32
	bl 0x0200a9cc
	movs r1, #1
	ldr r0, [pc, #20]
	bl 0x0200aabc
	movs r0, #24
	bl 0x0200aacc
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x023e0000
	.4byte 0x002051cc
	.4byte 0x00000201
	.4byte 0x00000202
	.4byte 0x03001ebc
	.4byte 0x00010005
	.global Func_02001380
	.thumb_func
Func_02001380:
	push {r5, lr}
	bl 0x0200a9d4
	bl 0x02009264
	ldr r0, [pc, #368]
	bl 0x0200aa54
	movs r1, #128
	movs r0, #16
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200aa74
	movs r1, #128
	movs r0, #16
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200aa7c
	movs r0, #16
	movs r1, #6
	movs r2, #30
	bl 0x0200aa34
	movs r1, #1
	movs r2, #174
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	ldr r0, [pc, #320]
	bl 0x0200aa9c
	bl 0x0200aaa4
	movs r0, #30
	bl 0x0200a9cc
	ldr r0, [pc, #308]
	movs r1, #20
	bl 0x0200a5fc
	movs r5, #0
.L_02001380_0:
	movs r0, #246
	bl 0x0200aaf4
	bl 0x02008238
	movs r0, #12
	bl 0x0200a9cc
	movs r0, #246
	bl 0x0200aaf4
	adds r5, #1
	bl 0x02008360
	movs r0, #12
	bl 0x0200a9cc
	cmp r5, #4
	bne .L_02001380_0
	movs r5, #0
.L_02001380_1:
	movs r0, #246
	bl 0x0200aaf4
	bl 0x02008238
	movs r0, #8
	bl 0x0200a9cc
	movs r0, #246
	bl 0x0200aaf4
	adds r5, #1
	bl 0x02008360
	movs r0, #8
	bl 0x0200a9cc
	cmp r5, #6
	bne .L_02001380_1
	movs r5, #0
.L_02001380_2:
	movs r0, #246
	bl 0x0200aaf4
	bl 0x02008238
	movs r0, #6
	bl 0x0200a9cc
	movs r0, #246
	bl 0x0200aaf4
	adds r5, #1
	bl 0x02008360
	movs r0, #6
	bl 0x0200a9cc
	cmp r5, #8
	bne .L_02001380_2
	movs r5, #0
.L_02001380_3:
	movs r0, #246
	bl 0x0200aaf4
	bl 0x02008238
	movs r0, #4
	bl 0x0200a9cc
	movs r0, #246
	bl 0x0200aaf4
	adds r5, #1
	bl 0x02008360
	movs r0, #4
	bl 0x0200a9cc
	cmp r5, #10
	bne .L_02001380_3
	movs r5, #0
.L_02001380_4:
	movs r0, #246
	bl 0x0200aaf4
	bl 0x02008238
	movs r0, #2
	bl 0x0200a9cc
	movs r0, #246
	bl 0x0200aaf4
	adds r5, #1
	bl 0x02008360
	movs r0, #2
	bl 0x0200a9cc
	cmp r5, #12
	bne .L_02001380_4
	bl 0x02008238
	movs r0, #6
	bl 0x0200a9cc
	ldr r0, [pc, #92]
	movs r1, #6
	bl 0x0200a5fc
	movs r1, #128
	movs r2, #128
	movs r0, #16
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200a9f4
	movs r1, #144
	movs r2, #140
	movs r0, #16
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x0200aa0c
	ldr r3, [pc, #60]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	subs r3, #192
	str r3, [r2]
	adds r3, #200
	adds r2, r1, r3
	movs r3, #32
	str r3, [r2]
	bl 0x0200aadc
	bl 0x0200aae4
	ldr r0, [pc, #32]
	bl 0x0200a9bc
	movs r0, #3
	bl 0x0200aaac
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001018
	.4byte 0x023e0000
	.4byte 0x00008010
	.4byte 0x03001ebc
	.4byte 0x00000813
	.global Func_02001510
	.thumb_func
Func_02001510:
	push {r5, lr}
	sub sp, #8
	bl 0x0200a9d4
	bl 0x02009264
	ldr r2, [pc, #60]
	ldr r3, [pc, #64]
	strh r2, [r3]
	ldr r3, [pc, #64]
	strh r2, [r3]
	ldr r3, [pc, #64]
	strh r2, [r3]
	ldr r3, [pc, #64]
	ldr r0, [pc, #64]
	strh r2, [r3]
	bl 0x0200aa54
	movs r1, #128
	movs r0, #16
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200aa74
	movs r1, #128
	movs r0, #16
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200aa7c
	movs r0, #16
	movs r1, #6
	movs r2, #30
	bl 0x0200aa34
	movs r1, #1
	b .L_02001510_0
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0200ade4
	.4byte 0x0200addc
	.4byte 0x0200ade0
	.4byte 0x0200adec
	.4byte 0x00001001
.L_02001510_0:
	movs r2, #174
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	ldr r0, [pc, #480]
	bl 0x0200aa9c
	bl 0x0200aaa4
	movs r0, #30
	bl 0x0200a9cc
	ldr r0, [pc, #468]
	movs r1, #20
	bl 0x0200a5fc
	movs r5, #0
.L_02001510_1:
	movs r0, #246
	bl 0x0200aaf4
	bl 0x02008238
	movs r0, #12
	bl 0x0200a9cc
	movs r0, #246
	bl 0x0200aaf4
	adds r5, #1
	bl 0x02008360
	movs r0, #12
	bl 0x0200a9cc
	cmp r5, #4
	bne .L_02001510_1
	movs r1, #6
	ldr r0, [pc, #420]
	bl 0x0200a5fc
	ldr r5, [pc, #416]
	bl 0x0200a984
	lsls r3, r0, #4
	subs r3, r3, r0
	lsls r3, r3, #2
	lsrs r3, r3, #16
	adds r3, #20
	strh r3, [r5]
	ldr r5, [pc, #404]
	bl 0x0200a984
	lsls r3, r0, #4
	subs r3, r3, r0
	lsls r3, r3, #2
	lsrs r3, r3, #16
	adds r3, #20
	strh r3, [r5]
	ldr r5, [pc, #388]
	bl 0x0200a984
	lsls r3, r0, #4
	subs r3, r3, r0
	lsls r3, r3, #2
	lsrs r3, r3, #16
	adds r3, #20
	strh r3, [r5]
	ldr r5, [pc, #376]
	bl 0x0200a984
	lsls r3, r0, #4
	subs r3, r3, r0
	lsls r3, r3, #2
	lsrs r3, r3, #16
	ldr r2, [pc, #364]
	adds r3, #20
	strh r3, [r5]
	movs r1, #200
	movs r3, #0
	str r3, [r2]
	lsls r1, r1, #4
.L_02001616:
	ldr r0, [pc, #356]
	bl 0x0200a974
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #348]
	bl 0x0200a974
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #344]
	bl 0x0200a974
	movs r1, #200
	ldr r0, [pc, #340]
	lsls r1, r1, #4
	bl 0x0200a974
	movs r5, #0
	movs r0, #246
.L_0200163e:
	bl 0x0200aaf4
	bl 0x02008238
	movs r0, #5
	bl 0x0200a9cc
	movs r0, #246
	bl 0x0200aaf4
	adds r5, #1
	bl 0x02008360
	movs r0, #5
	bl 0x0200a9cc
	cmp r5, #6
	bne 0x0200963c
	movs r5, #0
	movs r0, #246
.L_02001666:
	bl 0x0200aaf4
	bl 0x02008238
	movs r0, #4
	bl 0x0200a9cc
	movs r0, #246
	bl 0x0200aaf4
	adds r5, #1
	bl 0x02008360
	movs r0, #4
	bl 0x0200a9cc
	cmp r5, #8
	bne 0x02009664
	movs r5, #0
	movs r0, #246
.L_0200168e:
	bl 0x0200aaf4
	bl 0x02008238
	movs r0, #3
	bl 0x0200a9cc
	movs r0, #246
	bl 0x0200aaf4
	adds r5, #1
	bl 0x02008360
	movs r0, #3
	bl 0x0200a9cc
	cmp r5, #10
	bne 0x0200968c
	movs r5, #0
	movs r0, #246
.L_020016b6:
	bl 0x0200aaf4
	bl 0x02008238
	movs r0, #2
	bl 0x0200a9cc
	movs r0, #246
	bl 0x0200aaf4
	adds r5, #1
	bl 0x02008360
	movs r0, #2
	bl 0x0200a9cc
.L_020016d6:
	cmp r5, #12
	bne 0x020096b4
	movs r3, #4
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #10
	movs r0, #45
	movs r1, #30
	movs r2, #34
	bl 0x0200a994
	movs r2, #40
	movs r0, #16
	movs r1, #6
	bl 0x0200aa34
	ldr r0, [pc, #104]
	movs r1, #6
	bl 0x0200a5fc
	movs r1, #128
	movs r2, #128
	movs r0, #16
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200a9f4
	movs r1, #144
	movs r2, #140
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #16
	bl 0x0200aa0c
	ldr r0, [pc, #92]
	bl 0x0200a97c
	ldr r0, [pc, #92]
	bl 0x0200a97c
	ldr r0, [pc, #88]
	bl 0x0200a97c
	ldr r0, [pc, #88]
	bl 0x0200a97c
	ldr r3, [pc, #84]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	subs r3, #192
	str r3, [r2]
	adds r3, #200
	adds r2, r1, r3
	movs r3, #32
	str r3, [r2]
	bl 0x0200aadc
	bl 0x0200aae4
	movs r0, #4
	bl 0x0200aaac
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x023e
	.4byte 0x00008010
	.2byte 0xade4
	.2byte 0x0200
	.2byte 0xaddc
	.2byte 0x0200
	.2byte 0xade0
	.2byte 0x0200
	.2byte 0xadec
	.2byte 0x0200
	.2byte 0xade8
	.2byte 0x0200
	.4byte 0x02008d9d
	.4byte 0x02008ef9
	.4byte 0x0200901d
	.4byte 0x02009141
	.4byte 0x03001ebc
	.global Func_02001790
	.thumb_func
Func_02001790:
	push {r5, lr}
	sub sp, #8
	bl 0x0200a9d4
	ldr r0, [pc, #648]
	bl 0x0200a9b4
	cmp r0, #0
.L_020017a0:
	beq 0x020097c8
	ldr r0, [pc, #644]
	bl 0x0200a9b4
	cmp r0, #0
	beq 0x020097c8
	ldr r0, [pc, #632]
	bl 0x0200a9c4
	movs r3, #2
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #45
	movs r1, #28
	movs r2, #34
	movs r3, #10
	bl 0x0200a994
	b 0x020097e2
.L_020017c8:
	ldr r0, [pc, #600]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_020017c8_0
	ldr r0, [pc, #596]
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_020017c8_0
	ldr r0, [pc, #584]
	bl 0x0200a9bc
.L_020017c8_0:
	ldr r0, [pc, #584]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_020017c8_1
	ldr r0, [pc, #576]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_020017c8_1
	ldr r0, [pc, #568]
	bl 0x0200a9c4
	movs r3, #2
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #47
	movs r1, #28
	movs r2, #36
	movs r3, #10
	bl 0x0200a994
	b .L_020017c8_2
.L_020017c8_1:
	ldr r0, [pc, #536]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_020017c8_2
	ldr r0, [pc, #528]
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_020017c8_2
	ldr r0, [pc, #520]
	bl 0x0200a9bc
.L_020017c8_2:
	ldr r0, [pc, #516]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_020017c8_3
	ldr r0, [pc, #512]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_020017c8_3
	ldr r0, [pc, #500]
	bl 0x0200a9c4
	movs r3, #2
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #45
	movs r1, #29
	movs r2, #34
	movs r3, #11
	bl 0x0200a994
	b .L_020017c8_4
.L_020017c8_3:
	ldr r0, [pc, #468]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_020017c8_4
	ldr r0, [pc, #464]
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_020017c8_4
	ldr r0, [pc, #452]
	bl 0x0200a9bc
.L_020017c8_4:
	ldr r0, [pc, #452]
	bl 0x0200a9b4
.L_0200187c:
	cmp r0, #0
	beq 0x020098a6
	ldr r0, [pc, #444]
	bl 0x0200a9b4
	cmp r0, #0
	beq 0x020098a6
	ldr r0, [pc, #436]
	bl 0x0200a9c4
	movs r3, #2
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #47
	movs r1, #29
	movs r2, #36
	movs r3, #11
	bl 0x0200a994
.L_020018a4:
	b .L_020018a4_0
	.2byte 0x4865
	.2byte 0xf001
	.2byte 0xf884
	.2byte 0x2800
	.2byte 0xd007
	.2byte 0x4863
	.2byte 0xf001
	.2byte 0xf87f
	.2byte 0x2800
	.2byte 0xd102
	.2byte 0x4861
	.2byte 0xf001
	.2byte 0xf87e
.L_020018a4_0:
	bl 0x02009264
	movs r1, #128
	movs r0, #16
	lsls r1, r1, #7
	movs r2, #20
.L_020018cc:
	bl 0x0200aa74
	movs r0, #16
	movs r1, #6
	movs r2, #30
	bl 0x0200aa34
	movs r1, #1
	movs r2, #174
	ldr r0, [pc, #356]
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	bl 0x0200aa9c
	bl 0x0200aaa4
	movs r0, #30
	bl 0x0200a9cc
.L_020018f4:
	movs r5, #0
.L_020018f4_0:
	movs r0, #246
	bl 0x0200aaf4
	bl 0x020082cc
	movs r0, #12
	bl 0x0200a9cc
	movs r0, #246
	bl 0x0200aaf4
	adds r5, #1
	bl 0x020083f4
	movs r0, #12
	bl 0x0200a9cc
	cmp r5, #4
	bne .L_020018f4_0
	movs r5, #0
.L_020018f4_1:
	movs r0, #246
	bl 0x0200aaf4
	bl 0x020082cc
	movs r0, #8
	bl 0x0200a9cc
	movs r0, #246
	bl 0x0200aaf4
	adds r5, #1
	bl 0x020083f4
	movs r0, #8
	bl 0x0200a9cc
	cmp r5, #6
	bne .L_020018f4_1
	movs r5, #0
.L_020018f4_2:
	movs r0, #246
	bl 0x0200aaf4
	bl 0x020082cc
	movs r0, #6
	bl 0x0200a9cc
	movs r0, #246
	bl 0x0200aaf4
	adds r5, #1
	bl 0x020083f4
	movs r0, #6
	bl 0x0200a9cc
	cmp r5, #8
	bne .L_020018f4_2
	movs r5, #0
.L_020018f4_3:
	movs r0, #246
	bl 0x0200aaf4
	bl 0x020082cc
	movs r0, #4
	bl 0x0200a9cc
	movs r0, #246
	bl 0x0200aaf4
	adds r5, #1
	bl 0x020083f4
	movs r0, #4
	bl 0x0200a9cc
	cmp r5, #10
	bne .L_020018f4_3
	movs r5, #0
.L_020018f4_4:
	movs r0, #246
	bl 0x0200aaf4
	bl 0x020082cc
	movs r0, #2
	bl 0x0200a9cc
	movs r0, #246
	bl 0x0200aaf4
	adds r5, #1
	bl 0x020083f4
	movs r0, #2
	bl 0x0200a9cc
	cmp r5, #12
	bne .L_020018f4_4
	movs r0, #246
	bl 0x0200aaf4
	bl 0x020082cc
	movs r0, #6
	bl 0x0200a9cc
	ldr r0, [pc, #120]
	bl 0x0200a9b4
	cmp r0, #0
	bne 0x020099f6
	ldr r5, [pc, #116]
	ldr r0, [pc, #116]
	bl 0x0200aa54
.L_020019de:
	adds r0, r5, #0
	movs r1, #6
	bl 0x0200a5fc
	movs r0, #16
	movs r1, #3
	bl 0x0200aa2c
	adds r0, r5, #0
	movs r1, #6
	bl 0x0200a5fc
	ldr r3, [pc, #92]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	subs r3, #192
	str r3, [r2]
	adds r3, #200
.L_02001a06:
	adds r2, r1, r3
	movs r3, #32
	str r3, [r2]
	bl 0x0200aadc
	bl 0x0200aae4
	movs r0, #5
	bl 0x0200aaac
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x080b
	.2byte 0x0000
	.2byte 0x0826
	.2byte 0x0000
	.2byte 0x080c
	.2byte 0x0000
	.2byte 0x0827
	.2byte 0x0000
	.2byte 0x080d
	.2byte 0x0000
	.2byte 0x0828
	.2byte 0x0000
	.2byte 0x080e
	.2byte 0x0000
	.2byte 0x0829
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x023e
	.2byte 0x0822
	.2byte 0x0000
	.2byte 0x8010
	.2byte 0x0000
	.2byte 0x1025
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.global Func_02001a58
	.thumb_func
Func_02001a58:
	push {lr}
	movs r0, #129
	lsls r0, r0, #4
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02001a58_0
	b 0x02009bd2
.L_02001a58_0:
	bl 0x0200a5b0
	cmp r0, #0
	bne .L_02001a58_1
	b 0x02009bd2
.L_02001a58_1:
	bl 0x0200a9d4
	movs r2, #147
	movs r0, #16
	ldr r1, [pc, #348]
	lsls r2, r2, #16
	bl 0x0200aa1c
	movs r1, #128
	movs r0, #16
	lsls r1, r1, #7
	movs r2, #1
	bl 0x0200aa74
	movs r1, #1
	movs r2, #184
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #16
	ldr r0, [pc, #320]
	bl 0x0200aa9c
	ldr r0, [pc, #320]
	bl 0x0200aa54
	movs r1, #144
	movs r2, #232
	movs r0, #0
	lsls r1, r1, #2
	bl 0x0200aa0c
	movs r1, #0
	movs r0, #0
	bl 0x0200aa24
	bl 0x0200aaa4
	movs r0, #10
	bl 0x0200a9cc
	movs r1, #128
	movs r2, #128
	movs r0, #16
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200a9f4
	movs r1, #144
	lsls r1, r1, #2
	movs r2, #152
	movs r0, #16
	bl 0x0200aa0c
	movs r0, #6
	bl 0x0200a9cc
	movs r2, #30
	movs r0, #16
	movs r1, #6
	bl 0x0200aa34
	movs r0, #16
	movs r1, #6
	bl 0x0200a5fc
	movs r1, #3
	movs r0, #0
	bl 0x0200aa2c
	movs r0, #2
	bl 0x0200a9cc
	movs r0, #16
	movs r1, #4
	bl 0x0200aa2c
	movs r0, #16
	movs r1, #6
	bl 0x0200a5fc
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #0
	bl 0x0200aa84
	movs r0, #40
	bl 0x0200a9cc
	movs r1, #2
	movs r0, #16
	bl 0x0200aa3c
	movs r0, #30
	bl 0x0200a9cc
	movs r0, #16
	movs r1, #6
	bl 0x0200a5fc
	movs r0, #0
	movs r1, #3
	bl 0x0200aa2c
	movs r1, #144
	movs r2, #184
	lsls r1, r1, #2
	movs r0, #16
	bl 0x0200aa0c
	movs r0, #6
	bl 0x0200a9cc
	movs r1, #2
	movs r0, #16
	bl 0x0200aa3c
	movs r0, #40
	bl 0x0200a9cc
	ldr r0, [pc, #128]
	movs r1, #6
	bl 0x0200a5fc
	movs r1, #144
	movs r2, #208
	lsls r1, r1, #2
	movs r0, #16
	bl 0x0200aa0c
	movs r0, #40
	bl 0x0200a9cc
	movs r1, #3
	movs r0, #0
	bl 0x0200aa2c
	movs r0, #6
	bl 0x0200a9cc
	movs r1, #128
	movs r2, #128
	movs r0, #16
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200a9f4
	movs r0, #16
	movs r1, #2
	bl 0x0200aa24
	movs r0, #0
	bl 0x0200a9ec
	cmp r0, #0
	beq .L_02001a58_2
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #16
	bl 0x0200a9fc
.L_02001a58_2:
	movs r0, #16
	bl 0x0200aa14
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl 0x0200aa1c
	movs r0, #129
	lsls r0, r0, #4
.L_02001bca:
	bl 0x0200a9bc
	bl 0x0200a9dc
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0241
	.2byte 0x0000
	.2byte 0x023e
	.2byte 0x1027
	.2byte 0x0000
	.2byte 0x4010
	.2byte 0x0000
	.global Func_02001be8
	.thumb_func
Func_02001be8:
	push {lr}
	movs r0, #21
	bl 0x0200aaf4
	movs r1, #188
.L_02001bf2:
	movs r2, #184
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200aa0c
	movs r0, #0
	movs r1, #0
	bl 0x0200aa24
	movs r1, #188
	movs r2, #184
	movs r0, #16
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x0200aa1c
	movs r1, #128
	movs r2, #128
	movs r0, #16
	lsls r1, r1, #9
.L_02001c1a:
	lsls r2, r2, #8
	bl 0x0200a9f4
	movs r1, #196
	movs r0, #16
	lsls r1, r1, #1
	movs r2, #168
	bl 0x0200aa0c
	movs r1, #128
	movs r2, #30
	movs r0, #16
	lsls r1, r1, #8
	bl 0x0200aa74
	movs r1, #1
	movs r0, #16
	bl 0x0200aa24
	ldr r0, [pc, #92]
.L_02001c42:
	bl 0x0200aa54
	movs r2, #30
	movs r0, #16
	movs r1, #4
	bl 0x0200aa34
	movs r0, #16
	movs r1, #6
	bl 0x0200a5fc
	movs r1, #2
	movs r0, #0
	bl 0x0200aa3c
	movs r0, #6
	bl 0x0200a9cc
	movs r0, #16
	movs r1, #3
.L_02001c6a:
	bl 0x0200aa2c
	movs r0, #16
	movs r1, #6
	bl 0x0200a5fc
	movs r1, #188
	movs r0, #16
	lsls r1, r1, #1
	movs r2, #184
	bl 0x0200aa0c
	movs r1, #201
	lsls r1, r1, #19
	adds r2, r1, #0
	movs r0, #16
	bl 0x0200aa1c
	movs r0, #4
.L_02001c90:
	bl 0x0200a9cc
	ldr r0, [pc, #12]
	bl 0x0200a9bc
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x102b
	.2byte 0x0000
	.4byte 0x00000811
	.global Func_02001ca8
	.thumb_func
Func_02001ca8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r0, [pc, #996]
	sub sp, #8
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_02001ca8_0
	b .L_02001ca8_1
.L_02001ca8_0:
	bl 0x0200a9d4
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200aa94
	movs r1, #1
	movs r2, #148
	movs r3, #1
	ldr r0, [pc, #964]
	negs r1, r1
	lsls r2, r2, #16
	bl 0x0200aa9c
	movs r1, #144
	movs r2, #120
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200aa0c
	movs r0, #0
	movs r1, #0
	bl 0x0200aa24
	movs r0, #0
	movs r1, #4
	movs r2, #30
	bl 0x0200aa34
	movs r1, #144
	movs r2, #240
	movs r0, #16
	lsls r1, r1, #17
	lsls r2, r2, #15
	bl 0x0200aa1c
	movs r1, #128
	movs r2, #128
	movs r0, #16
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200a9f4
	movs r1, #138
	movs r0, #16
	lsls r1, r1, #1
	movs r2, #136
	bl 0x0200aa0c
	movs r1, #132
	movs r0, #16
	lsls r1, r1, #1
	movs r2, #136
	bl 0x0200aa04
	movs r1, #156
	movs r2, #136
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200aa0c
	movs r0, #0
	movs r1, #1
	bl 0x0200aa24
	movs r0, #16
	movs r1, #1
	bl 0x0200aa24
	movs r1, #176
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aa74
	movs r1, #208
	movs r0, #16
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200aa74
	ldr r0, [pc, #824]
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_02001ca8_2
	movs r0, #220
	bl 0x0200aaf4
.L_02001ca8_2:
	movs r0, #40
	bl 0x0200a9cc
	ldr r0, [pc, #800]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02001ca8_3
	b .L_02001ca8_4
.L_02001ca8_3:
	movs r2, #3
	str r2, [sp, #4]
	movs r5, #2
	mov r8, r2
	movs r0, #36
	movs r1, #62
	movs r2, #17
	movs r3, #36
	str r5, [sp, #0]
	bl 0x0200a994
	movs r6, #1
	movs r1, #59
	movs r2, #17
	movs r3, #38
	movs r0, #44
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200a994
	movs r0, #10
	bl 0x0200a9cc
	mov r3, r8
	str r3, [sp, #4]
	movs r0, #38
	movs r1, #62
	movs r2, #17
	movs r3, #36
	str r5, [sp, #0]
	bl 0x0200a994
	movs r1, #59
	movs r2, #17
	movs r3, #39
	movs r0, #44
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200a994
	movs r0, #10
	bl 0x0200a9cc
	mov r2, r8
	str r2, [sp, #4]
	movs r0, #40
	movs r1, #62
	movs r2, #17
	movs r3, #36
	str r5, [sp, #0]
	bl 0x0200a994
	movs r0, #0
	movs r1, #32
	movs r2, #17
	movs r3, #39
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200a994
	movs r3, #40
	movs r1, #59
	movs r2, #17
	movs r0, #44
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200a994
	movs r0, #10
	bl 0x0200a9cc
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200aa7c
	movs r1, #128
	movs r0, #16
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200aa7c
	mov r3, r8
	str r3, [sp, #4]
	movs r0, #42
	movs r1, #62
	movs r2, #17
	movs r3, #36
	str r5, [sp, #0]
	bl 0x0200a994
	movs r0, #0
	movs r1, #32
	movs r2, #17
	movs r3, #40
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200a994
	movs r1, #59
	movs r2, #17
	movs r3, #41
	movs r0, #44
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200a994
	movs r0, #10
	bl 0x0200a9cc
	movs r0, #0
	movs r1, #32
	movs r2, #17
	movs r3, #41
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200a994
	movs r1, #59
	movs r2, #17
	movs r3, #42
	movs r0, #44
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200a994
	movs r0, #10
	bl 0x0200a9cc
	mov r2, r8
	str r2, [sp, #4]
	movs r1, #32
	movs r2, #17
	movs r3, #42
	movs r0, #0
	str r5, [sp, #0]
	bl 0x0200a994
	movs r0, #80
	bl 0x0200a9cc
	movs r0, #9
	bl 0x0200a99c
	movs r0, #10
	bl 0x0200a99c
	ldr r0, [pc, #492]
	bl 0x0200a9bc
.L_02001ca8_4:
	movs r2, #30
	movs r0, #16
	movs r1, #0
	bl 0x0200aa44
	movs r0, #16
	movs r1, #3
	bl 0x0200aa24
	movs r0, #0
	movs r1, #3
	bl 0x0200aa2c
	movs r0, #16
	movs r1, #1
	bl 0x0200aa24
	ldr r5, [pc, #456]
	movs r1, #0
	movs r0, #0
	bl 0x0200aa24
	ldr r0, [pc, #452]
	bl 0x0200aa54
	adds r0, r5, #0
	movs r1, #6
	bl 0x0200a5fc
	movs r0, #16
	movs r1, #3
	bl 0x0200aa2c
	movs r0, #16
	movs r1, #1
	bl 0x0200aa24
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200aa64
	movs r1, #3
	movs r0, #0
	bl 0x0200aa24
	movs r0, #60
	bl 0x0200a9cc
	movs r0, #0
	bl 0x0200a9ec
	ldr r1, [pc, #396]
	adds r6, r0, #0
	ldr r0, [pc, #396]
	bl 0x0200aa94
	movs r1, #1
	movs r2, #228
	movs r3, #1
	ldr r0, [pc, #364]
	negs r1, r1
	lsls r2, r2, #15
	bl 0x0200aa9c
	movs r1, #144
	lsls r1, r1, #1
	movs r2, #120
	movs r0, #0
	bl 0x0200aa0c
	movs r0, #20
	bl 0x0200a9cc
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200aa74
	movs r0, #0
	ldr r1, [pc, #348]
	ldr r2, [pc, #348]
	bl 0x0200a9f4
	adds r1, r6, #0
	adds r1, #90
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	adds r3, r6, #0
	adds r3, #85
	movs r7, #0
	strb r7, [r3]
	movs r0, #201
	bl 0x0200aaf4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200aa4c
	ldr r7, [r6, #80]
	adds r3, r7, #0
	movs r5, #0
	adds r3, #38
	strb r5, [r3]
.L_02001ca8_5:
	ldr r3, [r6, #12]
	ldr r2, [pc, #300]
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r0, #1
	adds r5, #1
	bl 0x0200a96c
	cmp r5, #120
	bne .L_02001ca8_5
	movs r0, #190
	bl 0x0200aaf4
	movs r5, #0
.L_02001ca8_6:
	ldr r3, [r6, #12]
	ldr r2, [pc, #276]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r7, #24]
	ldr r2, [pc, #272]
	adds r3, r3, r2
	str r3, [r7, #24]
	movs r0, #1
	adds r5, #1
	bl 0x0200a96c
	cmp r5, #60
	bne .L_02001ca8_6
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200aa1c
	movs r2, #20
	movs r0, #16
	movs r1, #4
	bl 0x0200aa34
	movs r0, #16
	movs r1, #6
	bl 0x0200a5fc
	movs r1, #144
	movs r2, #120
	movs r0, #16
	lsls r1, r1, #1
	bl 0x0200aa0c
	movs r1, #2
	movs r0, #16
	bl 0x0200aa3c
	movs r0, #20
	bl 0x0200a9cc
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200aa74
	ldr r1, [pc, #172]
	ldr r2, [pc, #172]
	movs r0, #16
	bl 0x0200a9f4
	movs r0, #16
	bl 0x0200a9ec
	adds r6, r0, #0
	adds r1, r6, #0
	adds r1, #90
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	adds r3, r6, #0
	adds r3, #85
	movs r5, #0
	strb r2, [r1]
	movs r0, #201
	strb r5, [r3]
	bl 0x0200aaf4
	movs r1, #128
	movs r0, #16
	lsls r1, r1, #1
	bl 0x0200aa4c
	ldr r7, [r6, #80]
	adds r3, r7, #0
	adds r3, #38
	strb r5, [r3]
.L_02001ca8_7:
	ldr r3, [r6, #12]
	ldr r2, [pc, #116]
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r0, #1
	adds r5, #1
	bl 0x0200a96c
	cmp r5, #120
	bne .L_02001ca8_7
	movs r0, #190
	bl 0x0200aaf4
	movs r5, #0
.L_02001ca8_8:
	ldr r3, [r6, #12]
	ldr r2, [pc, #92]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r7, #24]
	ldr r2, [pc, #88]
	adds r3, r3, r2
	str r3, [r7, #24]
	movs r0, #1
	adds r5, #1
	bl 0x0200a96c
	cmp r5, #60
	bne .L_02001ca8_8
	movs r1, #0
	movs r2, #0
	movs r0, #16
	bl 0x0200aa1c
	movs r0, #80
	bl 0x0200a9cc
	ldr r3, [pc, #56]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	b .L_02001ca8_9
	.2byte 0x0000
	.4byte 0x00000811
	.4byte 0x011f0000
	.4byte 0x00000819
	.4byte 0x00008010
	.4byte 0x0000102e
	.4byte 0x00001333
	.4byte 0x00009999
	.4byte 0x00004ccc
	.4byte 0x00002666
	.4byte 0x00003333
	.4byte 0x00001999
	.4byte 0xfffffc00
	.4byte 0x03001ebc
.L_02001ca8_9:
	adds r2, r1, r3
	adds r3, #67
	str r3, [r2]
	subs r3, #59
	adds r2, r1, r3
	movs r3, #24
	str r3, [r2]
	bl 0x0200aadc
	bl 0x0200aae4
	movs r1, #0
	movs r0, #0
	bl 0x0200aabc
	movs r0, #1
	bl 0x0200aacc
	movs r0, #1
	bl 0x0200a96c
	movs r0, #7
	bl 0x0200aaac
	bl 0x0200a9dc
.L_02001ca8_1:
	sub sp, #-8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_02002108
	.thumb_func
Func_02002108:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #1
	sub sp, #8
	bl 0x0200a96c
	ldr r3, [pc, #628]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #68
	str r2, [r3]
	ldr r0, [pc, #616]
	movs r5, #0
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02002108_0
	ldr r0, [pc, #608]
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_02002108_0
	ldr r0, [pc, #604]
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_02002108_0
	movs r0, #162
	lsls r0, r0, #1
	bl 0x0200a9bc
.L_02002108_0:
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl 0x0200aac4
	ldr r0, [pc, #580]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02002108_1
	ldr r0, [pc, #572]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02002108_2
	ldr r0, [pc, #568]
	b .L_02002108_3
.L_02002108_2:
	ldr r0, [pc, #568]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02002108_4
	ldr r0, [pc, #560]
.L_02002108_3:
	movs r1, #1
	bl 0x0200aabc
	movs r0, #1
	bl 0x0200aacc
	movs r0, #1
	bl 0x0200a96c
	b .L_02002108_4
.L_02002108_1:
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200a9bc
	ldr r0, [pc, #536]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02002108_4
	movs r1, #144
	movs r2, #240
	movs r0, #16
	lsls r1, r1, #18
	lsls r2, r2, #15
	bl 0x0200aa1c
.L_02002108_4:
	ldr r3, [pc, #516]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #4
	bne .L_02002108_5
	ldr r0, [pc, #504]
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_02002108_6
	bl 0x02009380
	ldr r0, [pc, #488]
	bl 0x0200a9bc
	b .L_02002108_7
.L_02002108_5:
	cmp r3, #5
	bne .L_02002108_8
	ldr r0, [pc, #480]
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_02002108_6
	bl 0x02009510
	ldr r0, [pc, #468]
	bl 0x0200a9bc
	ldr r0, [pc, #464]
	bl 0x0200a9c4
	ldr r0, [pc, #464]
	bl 0x0200a9c4
	ldr r0, [pc, #460]
	bl 0x0200a9c4
	ldr r0, [pc, #460]
	bl 0x0200a9c4
	b .L_02002108_7
.L_02002108_8:
	cmp r3, #6
	bne .L_02002108_6
	ldr r0, [pc, #432]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02002108_7
	bl 0x02009790
	ldr r0, [pc, #436]
	bl 0x0200a9bc
.L_02002108_7:
	movs r5, #1
.L_02002108_6:
	ldr r0, [pc, #412]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02002108_9
	ldr r0, [pc, #424]
	bl 0x0200a9bc
.L_02002108_9:
	ldr r0, [pc, #400]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02002108_10
	ldr r0, [pc, #412]
	bl 0x0200a9bc
.L_02002108_10:
	ldr r0, [pc, #388]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02002108_11
	ldr r0, [pc, #400]
	bl 0x0200a9bc
.L_02002108_11:
	ldr r0, [pc, #376]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02002108_12
	ldr r0, [pc, #388]
	bl 0x0200a9bc
.L_02002108_12:
	movs r0, #4
	bl 0x0200a96c
	cmp r5, #0
	beq .L_02002108_13
	b .L_02002108_14
.L_02002108_13:
	bl 0x0200a5b0
	cmp r0, #0
	bne .L_02002108_15
	b .L_02002108_16
.L_02002108_15:
	movs r3, #12
	movs r2, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #30
	movs r1, #44
	movs r2, #30
	movs r3, #38
	bl 0x0200a994
	movs r3, #1
	str r3, [sp, #4]
	movs r5, #4
	movs r0, #30
	movs r1, #44
	movs r2, #34
	mov r8, r3
	movs r3, #37
	str r5, [sp, #0]
	bl 0x0200a994
	movs r6, #8
	movs r0, #14
	movs r1, #41
	movs r2, #32
	movs r3, #41
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a994
	movs r7, #2
	movs r0, #45
	movs r1, #28
	movs r2, #34
	movs r3, #10
	str r5, [sp, #0]
	str r7, [sp, #4]
	bl 0x0200a994
	movs r0, #45
	movs r1, #30
	movs r2, #16
	movs r3, #10
	str r5, [sp, #0]
	str r7, [sp, #4]
	bl 0x0200a994
	movs r2, #14
	movs r3, #41
	movs r0, #14
	movs r1, #45
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a994
	ldr r3, [pc, #204]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #8
	beq .L_02002108_17
	ldr r0, [pc, #156]
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_02002108_17
	ldr r0, [pc, #152]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02002108_18
	mov r3, r8
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #32
	movs r2, #17
	movs r3, #39
	str r7, [sp, #0]
	bl 0x0200a994
	movs r5, #3
	movs r0, #42
	movs r1, #62
	movs r2, #17
	movs r3, #36
	str r7, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a994
	mov r2, r8
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #32
	movs r2, #17
	movs r3, #40
	str r7, [sp, #0]
	bl 0x0200a994
	mov r3, r8
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #32
	movs r2, #17
	movs r3, #41
	str r7, [sp, #0]
	bl 0x0200a994
	movs r0, #0
	movs r1, #32
	movs r2, #17
	movs r3, #42
	str r7, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a994
	b .L_02002108_19
.L_02002108_18:
	movs r3, #6
	str r3, [sp, #4]
	movs r0, #44
	movs r1, #59
	movs r2, #17
	movs r3, #37
	str r7, [sp, #0]
	bl 0x0200a994
.L_02002108_19:
	movs r0, #9
	bl 0x0200a99c
	movs r0, #10
	bl 0x0200a99c
.L_02002108_17:
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl 0x0200aa1c
	b .L_02002108_14
	.4byte 0x03001ebc
	.4byte 0x00000809
	.4byte 0x00000814
	.4byte 0x00000819
	.4byte 0x00000109
	.4byte 0x00000201
	.4byte 0x002051cc
	.4byte 0x00000202
	.4byte 0x00202db1
	.4byte 0x0000080a
	.4byte 0x02000240
	.4byte 0x00000813
	.4byte 0x00000812
	.4byte 0x0000080b
	.4byte 0x0000080c
	.4byte 0x0000080d
	.4byte 0x0000080e
	.4byte 0x00000822
	.4byte 0x00000826
	.4byte 0x00000827
	.4byte 0x00000828
	.4byte 0x00000829
.L_02002108_16:
	ldr r0, [pc, #424]
	movs r5, #0
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02002108_20
	movs r5, #2
	movs r6, #1
	movs r0, #45
	movs r1, #28
	movs r2, #34
	movs r3, #10
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200a994
	movs r0, #45
	movs r1, #30
	movs r2, #16
	movs r3, #10
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200a994
	movs r5, #1
.L_02002108_20:
	ldr r0, [pc, #380]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02002108_21
	movs r5, #2
	movs r6, #1
	movs r0, #47
	movs r1, #28
	movs r2, #36
	movs r3, #10
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200a994
	movs r0, #47
	movs r1, #30
	movs r2, #18
	movs r3, #10
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200a994
	movs r5, #1
.L_02002108_21:
	ldr r0, [pc, #336]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02002108_22
	movs r5, #2
	movs r6, #1
	movs r0, #45
	movs r1, #29
	movs r2, #34
	movs r3, #11
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200a994
	movs r0, #45
	movs r1, #31
	movs r2, #16
	movs r3, #11
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200a994
	movs r5, #1
.L_02002108_22:
	ldr r0, [pc, #292]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02002108_23
	movs r5, #2
	movs r6, #1
	movs r0, #47
	movs r1, #29
	movs r2, #36
	movs r3, #11
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200a994
	movs r0, #47
	movs r1, #31
	movs r2, #18
	movs r3, #11
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200a994
	movs r5, #1
.L_02002108_23:
	ldr r0, [pc, #248]
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_02002108_24
	cmp r5, #0
	beq .L_02002108_25
.L_02002108_24:
	movs r6, #8
	movs r5, #3
	movs r0, #30
	movs r1, #43
	movs r2, #32
	movs r3, #40
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a994
	movs r3, #1
	str r3, [sp, #4]
	movs r0, #30
	movs r1, #43
	movs r2, #33
	movs r3, #39
	str r6, [sp, #0]
	bl 0x0200a994
	movs r0, #30
	movs r1, #43
	movs r2, #36
	movs r3, #38
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a994
	movs r3, #4
	str r3, [sp, #4]
	movs r0, #36
	movs r1, #58
	movs r2, #32
	movs r3, #41
	str r6, [sp, #0]
	bl 0x0200a994
.L_02002108_25:
	movs r3, #17
	movs r2, #6
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #15
	movs r1, #6
	movs r2, #2
	movs r3, #1
	bl 0x0200a9a4
.L_02002108_14:
	ldr r0, [pc, #144]
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_02002108_26
	ldr r3, [pc, #140]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #8
	bne .L_02002108_26
	bl 0x0200a614
	ldr r0, [pc, #116]
	bl 0x0200a9bc
	movs r3, #17
	movs r2, #6
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #15
	movs r1, #6
	movs r2, #2
	movs r3, #1
	bl 0x0200a9a4
	b .L_02002108_27
.L_02002108_26:
	ldr r0, [pc, #96]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02002108_27
	movs r0, #141
	bl 0x0200aaec
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl 0x0200a9ac
	bl 0x0200aab4
	movs r3, #17
	movs r2, #6
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #15
	movs r1, #6
	movs r2, #2
	movs r3, #1
	bl 0x0200a9a4
.L_02002108_27:
	movs r0, #0
	sub sp, #-8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.4byte 0x0000080b
	.4byte 0x0000080c
	.4byte 0x0000080d
	.4byte 0x0000080e
	.4byte 0x00000812
	.4byte 0x00000309
	.4byte 0x02000240
	.4byte 0x00000814
	.global Func_020025b0
	.thumb_func
Func_020025b0:
	push {r5, lr}
	ldr r0, [pc, #56]
	movs r5, #1
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_020025b0_0
	movs r5, #0
.L_020025b0_0:
	ldr r0, [pc, #44]
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_020025b0_1
	movs r5, #0
.L_020025b0_1:
	ldr r0, [pc, #36]
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_020025b0_2
	movs r5, #0
.L_020025b0_2:
	ldr r0, [pc, #28]
	bl 0x0200a9b4
	cmp r0, #0
	bne .L_020025b0_3
	movs r5, #0
.L_020025b0_3:
	adds r0, r5, #0
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x0000080b
	.4byte 0x0000080c
	.4byte 0x0000080d
	.4byte 0x0000080e
	.global Func_020025fc
	.thumb_func
Func_020025fc:
	push {r5, lr}
	adds r5, r1, #0
	movs r1, #0
	bl 0x0200aa64
	adds r0, r5, #0
.L_02002608:
	bl 0x0200a9cc
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02002614
	.thumb_func
Func_02002614:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	bl 0x0200a9d4
	movs r0, #143
	movs r1, #1
	movs r2, #134
	movs r3, #0
	lsls r2, r2, #16
	negs r1, r1
	lsls r0, r0, #17
	bl 0x0200aa9c
	bl 0x0200a98c
	movs r0, #1
	bl 0x0200a96c
	ldr r0, [pc, #692]
	movs r1, #0
	bl 0x0200aac4
	movs r1, #0
	ldr r0, [pc, #684]
	bl 0x0200aabc
	movs r0, #1
	bl 0x0200aacc
	movs r0, #1
	bl 0x0200a96c
	movs r1, #140
	movs r2, #134
	movs r0, #1
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x0200aa1c
	movs r1, #160
	movs r2, #0
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200aa74
	ldr r0, [pc, #644]
	movs r1, #228
	ldr r3, [r0]
	lsls r1, r1, #1
	movs r7, #1
	str r7, [r3, r1]
	mov r8, r0
	mov r10, r1
	bl 0x0200aad4
	bl 0x0200aae4
	movs r1, #1
	ldr r0, [pc, #620]
	bl 0x0200aabc
	movs r0, #120
	bl 0x0200aacc
	movs r0, #120
	bl 0x0200a96c
	ldr r0, [pc, #608]
	bl 0x0200a9bc
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200a9c4
	ldr r0, [pc, #596]
	bl 0x0200a9c4
	movs r0, #128
	movs r1, #2
	lsls r0, r0, #9
	bl 0x0200aabc
	movs r0, #60
	bl 0x0200aacc
	movs r0, #100
	bl 0x0200a96c
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200aa84
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200aa84
	movs r0, #60
	bl 0x0200a9cc
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #20
	bl 0x0200aa74
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200aa74
	movs r1, #144
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200aa74
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #80
	bl 0x0200aa74
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200aa74
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #5
	movs r2, #60
	bl 0x0200aa74
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl 0x0200a9ac
	movs r0, #20
	bl 0x0200a9cc
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200aa74
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200aa74
	movs r0, #1
	movs r1, #1
	ldr r2, [pc, #424]
	negs r1, r1
	negs r0, r0
	bl 0x0200a9ac
	movs r0, #20
	bl 0x0200a9cc
	movs r1, #2
	movs r0, #1
	bl 0x0200aa3c
	ldr r0, [pc, #404]
	bl 0x0200aa54
	movs r1, #0
	movs r0, #1
	bl 0x0200aa5c
	movs r1, #0
	movs r0, #0
	bl 0x0200a9e4
	movs r1, #1
	movs r0, #0
	bl 0x0200aa24
	movs r0, #40
	bl 0x0200a9cc
	movs r0, #107
	bl 0x0200aaf4
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #11
	bl 0x0200a9ac
	movs r0, #40
	bl 0x0200a9cc
	bl 0x0200aab4
	movs r0, #0
	bl 0x0200a9ec
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #1
	bl 0x0200a9ec
	adds r0, #90
	ldrb r3, [r0]
	ands r5, r3
	strb r5, [r0]
	movs r1, #4
	movs r0, #0
	movs r2, #0
	bl 0x0200aa34
	movs r0, #1
	movs r1, #4
	movs r2, #0
	bl 0x0200aa34
	movs r1, #150
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #130
	bl 0x0200a9fc
	movs r1, #133
	lsls r1, r1, #1
	movs r2, #144
	movs r0, #1
	bl 0x0200a9fc
	movs r0, #1
	bl 0x0200aa14
	movs r0, #40
	bl 0x0200a9cc
	movs r0, #0
	bl 0x0200a9ec
	adds r0, #90
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #1
	bl 0x0200a9ec
	adds r0, #90
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r0, #141
	bl 0x0200aaec
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #9
	lsls r0, r0, #9
	bl 0x0200a9ac
	movs r0, #40
	bl 0x0200a9cc
	movs r0, #1
	movs r1, #2
	bl 0x0200aa3c
	movs r0, #1
	movs r1, #10
	bl 0x0200a5fc
	movs r0, #0
	movs r1, #3
	bl 0x0200aa24
	movs r0, #1
	movs r1, #3
	bl 0x0200aa2c
	movs r0, #1
	movs r1, #2
	bl 0x0200aa24
	movs r0, #0
	movs r6, #0
	bl 0x0200a9ec
	cmp r0, #0
	beq .L_02002614_0
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x0200a9fc
.L_02002614_0:
	movs r0, #1
	bl 0x0200aa14
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200aa1c
	mov r0, r8
	ldr r3, [r0, #20]
	movs r1, #224
	lsls r1, r1, #4
	ldr r0, [pc, #104]
	adds r2, r3, r1
	strh r6, [r2]
	adds r2, r3, r0
	adds r0, #2
	strh r6, [r2]
	adds r2, r3, r0
	movs r0, #168
	ldr r1, [pc, #56]
	lsls r0, r0, #6
	strh r6, [r2]
	adds r2, r3, r0
	strb r1, [r2]
	ldr r1, [pc, #84]
	adds r0, #2
	adds r2, r3, r1
	adds r1, #2
	strb r7, [r2]
	adds r2, r3, r0
	adds r3, r3, r1
	strb r7, [r2]
	strb r7, [r3]
	movs r3, #160
	lsls r3, r3, #19
	strh r6, [r3]
	mov r2, r8
	ldr r1, [r2]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #68
	str r3, [r2]
	mov r0, r10
	movs r3, #16
	str r3, [r1, r0]
	bl 0x0200a9dc
	b .L_02002614_1
	.4byte 0x00000000
	.4byte 0x00007fff
	.4byte 0x03001ebc
	.4byte 0x002051cc
	.4byte 0x00000201
	.4byte 0x00000202
	.4byte 0x0000e666
	.4byte 0x00001119
	.4byte 0x00000e02
	.4byte 0x00002a01
.L_02002614_1:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_02002924
	.thumb_func
Func_02002924:
	push {lr}
	bl 0x0200a9d4
	ldr r0, [pc, #52]
	bl 0x0200a9b4
	cmp r0, #0
	beq .L_02002924_0
	ldr r0, [pc, #44]
	bl 0x0200aa54
	b .L_02002924_1
.L_02002924_0:
	ldr r0, [pc, #40]
	bl 0x0200aa54
.L_02002924_1:
	movs r0, #16
	movs r1, #0
	movs r2, #10
	bl 0x0200aa6c
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200aa74
	bl 0x0200a9dc
	pop {r0}
	bx r0
	.4byte 0x00000896
	.4byte 0x00000ffd
	.4byte 0x00000fff
	.include "games/THE BROKEN SEAL/SRC/FIELD/SORU_NICHIGETSU/IMPORT.INC"
	.section .rodata,"a",%progbits
	.4byte 0xffff0000
	.4byte 0x00000120
	.4byte 0x4000009d
	.4byte 0x00000000
	.4byte 0x02f00000
	.4byte 0x000001f0
	.4byte 0xffff0001
	.4byte 0x00000037
	.4byte 0xc00001dd
	.4byte 0x00000000
	.4byte 0x02f00000
	.4byte 0x000001f0
	.4byte 0xffff0002
	.4byte 0x00000287
	.4byte 0x40000147
	.4byte 0x00000000
	.4byte 0x02f00000
	.4byte 0x000001f0
	.4byte 0xffff0003
	.4byte 0x0000011f
	.4byte 0x400000da
	.4byte 0x00000000
	.4byte 0x02f00000
	.4byte 0x000001f0
	.4byte 0xffff0004
	.4byte 0x00000240
	.4byte 0x40000008
	.4byte 0x00000000
	.4byte 0x02f00000
	.4byte 0x000001f0
	.4byte 0xffff0005
	.4byte 0x00000240
	.4byte 0x40000008
	.4byte 0x00000000
	.4byte 0x02f00000
	.4byte 0x000001f0
	.4byte 0xffff0006
	.4byte 0x00000240
	.4byte 0x40000008
	.4byte 0x00000000
	.4byte 0x02f00000
	.4byte 0x000001f0
	.4byte 0xffff0007
	.4byte 0x00000240
	.4byte 0x40000008
	.4byte 0x00000000
	.4byte 0x02f00000
	.4byte 0x000001f0
	.4byte 0xffff0008
	.4byte 0x00000129
	.4byte 0xa0000077
	.4byte 0x00000000
	.4byte 0x02f00000
	.4byte 0x000001f0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000b
	.4byte 0x00110010
	.4byte 0x0020100c
	.4byte 0x0030300c
	.4byte 0x0040400c
	.4byte 0x0050500c
	.4byte 0x0060600c
	.4byte 0x0070a011
	.4byte 0x00a011fe
	.4byte 0x000001ff
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00003000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0000d000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0000b000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00005000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00003000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0000d000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0000b000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00005000
	.4byte 0xffff001e
	.4byte 0x00000001
	.4byte 0x06480000
	.4byte 0x00000000
	.4byte 0x06480000
	.4byte 0x0000c000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x06480000
	.4byte 0x00000000
	.4byte 0x06480000
	.4byte 0x0000c000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x06480000
	.4byte 0x00000000
	.4byte 0x06480000
	.4byte 0x0000c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x0200a925
	.4byte 0x00000002
	.4byte 0xffff000e
	.4byte 0x02008151
	.4byte 0x00000002
	.4byte 0xffff000d
	.4byte 0x02008055
	.4byte 0x00000002
	.4byte 0xffff000f
	.4byte 0x02008109
	.4byte 0x00000002
	.4byte 0xffff000c
	.4byte 0x020081ed
	.4byte 0x00000002
	.4byte 0xffff0013
	.4byte 0x0200822d
	.4byte 0x00000003
	.4byte 0xffff0014
	.4byte 0x02009ca9
	.4byte 0x00000002
	.4byte 0xffff0008
	.4byte 0x02009a59
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x02008489
	.4byte 0x00000002
	.4byte 0xffff0009
	.4byte 0x02008c01
	.4byte 0x00000013
	.4byte 0x0f400064
	.4byte 0x001000b4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
