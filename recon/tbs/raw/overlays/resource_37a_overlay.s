.syntax unified
	.thumb
	.section .text.x02008488,"ax",%progbits
	.balign 4
	.global FieldScene_RunScene37aSequenceF
	.thumb_func
FieldScene_RunScene37aSequenceF:
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
	.global Scene_EnterInnerSanctum
	.thumb_func
Scene_EnterInnerSanctum:
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
	.global UpdateStatueTrapActor
	.thumb_func
UpdateStatueTrapActor:
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
	.section .rodata,"a",%progbits
	.global gSoruNichigetsuEntrances
gSoruNichigetsuEntrances:
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
	.global gSoruNichigetsuExits
gSoruNichigetsuExits:
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
	.global gSoruNichigetsuPlacements
gSoruNichigetsuPlacements:
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
	.global gSoruNichigetsuEvents
gSoruNichigetsuEvents:
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
	.section .bss,"aw",%nobits
	.global SoruNichigetsu_Light2Timer
SoruNichigetsu_Light2Timer:
	.space 4
	.global SoruNichigetsu_Light3Timer
SoruNichigetsu_Light3Timer:
	.space 4
	.global SoruNichigetsu_Light1Timer
SoruNichigetsu_Light1Timer:
	.space 4
	.global SoruNichigetsu_FlashState
SoruNichigetsu_FlashState:
	.space 4
	.global SoruNichigetsu_Light4Timer
SoruNichigetsu_Light4Timer:
	.space 4
