.syntax unified
	.thumb
	.section .text.x020080dc,"ax",%progbits
	.balign 4
	.global Func_020000dc
	.thumb_func
Func_020000dc:
	push {r5, lr}
	bl 0x02009a84
	ldr r0, [pc, #128]
	bl 0x02009a64
	cmp r0, #0
	beq .L_020000dc_0
	ldr r0, [pc, #120]
	bl 0x02009b2c
	movs r0, #12
	movs r1, #0
	bl 0x02009b3c
	b .L_020000dc_1
.L_020000dc_0:
	ldr r5, [pc, #108]
	adds r0, r5, #0
	bl 0x02009b2c
	movs r2, #10
	movs r0, #12
	movs r1, #0
	bl 0x02009b14
	movs r1, #2
	movs r0, #12
	bl 0x02009b0c
	movs r0, #6
	bl 0x02009a7c
	movs r1, #0
	movs r0, #12
	bl 0x02009b34
	movs r0, #0
	movs r1, #0
	bl 0x02009a94
	cmp r0, #0
	bne .L_020000dc_2
	adds r0, r5, #1
	bl 0x02009b2c
	b .L_020000dc_3
.L_020000dc_2:
	adds r0, r5, #2
	bl 0x02009b2c
.L_020000dc_3:
	movs r0, #12
	movs r1, #3
	bl 0x02009b04
	movs r0, #12
	movs r1, #0
	bl 0x02009b3c
	movs r1, #192
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #10
	bl 0x02009b54
.L_020000dc_1:
	bl 0x02009a8c
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00000815
	.4byte 0x000011c4
	.4byte 0x00000f76
	.global Func_02000170
	.thumb_func
Func_02000170:
	push {r5, r6, lr}
	ldr r0, [pc, #656]
	bl 0x02009a64
	cmp r0, #0
	beq .L_02000170_0
	b .L_02000170_1
.L_02000170_0:
	bl 0x02009a84
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	lsls r1, r1, #9
	movs r0, #0
	bl 0x02009aa4
	ldr r0, [pc, #628]
	bl 0x02009b2c
	movs r0, #13
	movs r1, #1
	bl 0x02009b0c
	movs r2, #132
	lsls r2, r2, #1
	movs r0, #0
	movs r1, #232
	bl 0x02009ad4
	movs r0, #0
	movs r1, #0
	bl 0x02009aec
	movs r2, #20
	movs r0, #0
	movs r1, #13
	bl 0x02009b1c
	movs r0, #13
	movs r1, #2
	bl 0x02009b0c
	movs r2, #10
	movs r1, #0
	movs r0, #13
	bl 0x02009b44
	movs r0, #0
	bl 0x02009a9c
	movs r2, #10
	ldrsh r5, [r0, r2]
	movs r3, #18
	ldrsh r6, [r0, r3]
	lsls r5, r5, #16
	lsls r6, r6, #16
	movs r0, #5
	adds r1, r5, #0
	adds r2, r6, #0
	bl 0x02009ae4
	movs r0, #1
	adds r1, r5, #0
	adds r2, r6, #0
	bl 0x02009ae4
	movs r1, #128
	movs r2, #128
	movs r0, #5
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x02009aa4
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x02009aa4
	movs r2, #132
	movs r0, #5
	movs r1, #248
	lsls r2, r2, #1
	bl 0x02009acc
	movs r2, #132
	lsls r2, r2, #1
	movs r0, #1
	movs r1, #216
	bl 0x02009ad4
	movs r0, #0
	movs r1, #1
	bl 0x02009aec
	movs r0, #5
	movs r1, #1
	bl 0x02009aec
	movs r1, #1
	movs r0, #1
	bl 0x02009aec
	movs r0, #4
	bl 0x02009a7c
	movs r1, #176
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b54
	movs r1, #208
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #8
	bl 0x02009b54
	movs r1, #4
	movs r0, #5
	bl 0x02009af4
	movs r0, #10
	bl 0x02009a7c
	movs r2, #20
	movs r0, #5
	movs r1, #0
	bl 0x02009b44
	movs r1, #1
	movs r0, #13
	bl 0x02009b0c
	movs r0, #10
	bl 0x02009a7c
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #6
	movs r2, #10
	bl 0x02009b54
	movs r0, #13
	movs r1, #0
	movs r2, #10
	bl 0x02009b44
	movs r1, #192
	movs r2, #40
	movs r0, #1
	lsls r1, r1, #6
	bl 0x02009b54
	movs r0, #1
	movs r1, #2
	bl 0x02009b0c
	movs r2, #10
	movs r0, #1
	movs r1, #0
	bl 0x02009b44
	movs r0, #13
	movs r1, #2
	bl 0x02009b0c
	movs r1, #160
	movs r2, #10
	movs r0, #13
	lsls r1, r1, #7
	bl 0x02009b54
	movs r0, #13
	movs r1, #3
	bl 0x02009aec
	movs r0, #13
	movs r1, #0
	movs r2, #8
	bl 0x02009b44
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #20
	bl 0x02009b54
	movs r0, #5
	movs r1, #0
	movs r2, #10
	bl 0x02009b44
	movs r1, #192
	movs r2, #10
	movs r0, #13
	lsls r1, r1, #6
	bl 0x02009b54
	movs r1, #3
	movs r0, #13
	bl 0x02009af4
	movs r0, #10
	bl 0x02009a7c
	movs r0, #13
	movs r1, #0
	movs r2, #6
	bl 0x02009b44
	movs r0, #0
	ldr r1, [pc, #240]
	movs r2, #0
	bl 0x02009b5c
	movs r0, #1
	ldr r1, [pc, #228]
	movs r2, #0
	bl 0x02009b5c
	movs r0, #5
	ldr r1, [pc, #220]
	movs r2, #60
	bl 0x02009b5c
	movs r1, #208
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b54
	movs r1, #176
	movs r2, #20
	movs r0, #5
	lsls r1, r1, #8
	bl 0x02009b54
	movs r0, #5
	movs r1, #2
	bl 0x02009b0c
	movs r0, #5
	movs r1, #0
	movs r2, #10
	bl 0x02009b44
	movs r1, #129
	movs r2, #60
	movs r0, #13
	lsls r1, r1, #1
	bl 0x02009b5c
	movs r0, #13
	movs r1, #4
	bl 0x02009af4
	movs r0, #13
	movs r1, #0
	movs r2, #10
	bl 0x02009b44
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x02009b54
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #20
	bl 0x02009b54
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #60
	bl 0x02009b5c
	movs r1, #208
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b54
	movs r1, #176
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b54
	movs r1, #160
	movs r0, #13
	lsls r1, r1, #7
	movs r2, #20
	bl 0x02009b54
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #6
	movs r2, #40
	bl 0x02009b54
	movs r1, #160
	movs r0, #13
	lsls r1, r1, #7
	movs r2, #40
	bl 0x02009b54
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #6
	movs r2, #20
	bl 0x02009b54
	movs r1, #0
	movs r0, #13
	bl 0x02009b34
	movs r0, #0
	movs r1, #0
	bl 0x02009a94
	cmp r0, #0
	bne .L_02000170_2
	ldr r0, [pc, #16]
	bl 0x02009b2c
	b .L_02000170_3
	.4byte 0x00000801
	.4byte 0x00000fa6
	.4byte 0x00000101
	.4byte 0x00000fb0
.L_02000170_2:
	ldr r0, [pc, #800]
	bl 0x02009b2c
.L_02000170_3:
	movs r0, #20
	bl 0x02009a7c
	movs r1, #3
	movs r0, #13
	bl 0x02009af4
	movs r0, #10
	bl 0x02009a7c
	movs r2, #10
	movs r1, #0
	movs r0, #13
	bl 0x02009b44
	ldr r0, [pc, #768]
	bl 0x02009b2c
	movs r0, #5
	movs r1, #2
	bl 0x02009b0c
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #10
	bl 0x02009b54
	movs r0, #5
	movs r1, #0
	movs r2, #6
	bl 0x02009b44
	movs r0, #1
	ldr r1, [pc, #736]
	movs r2, #30
	bl 0x02009b5c
	movs r0, #1
	movs r1, #4
	movs r2, #30
	bl 0x02009afc
	movs r0, #1
	movs r1, #0
	movs r2, #10
	bl 0x02009b54
	movs r0, #1
	movs r1, #0
	movs r2, #6
	bl 0x02009b44
	movs r0, #0
	movs r1, #1
	movs r2, #10
	bl 0x02009b1c
	movs r0, #0
	movs r1, #5
	movs r2, #0
	bl 0x02009b1c
	movs r0, #13
	movs r1, #1
	movs r2, #10
	bl 0x02009b14
	movs r2, #10
	movs r0, #13
	movs r1, #5
	bl 0x02009b14
	movs r0, #0
	movs r1, #3
	bl 0x02009aec
	movs r0, #1
	movs r1, #3
	bl 0x02009aec
	movs r1, #3
	movs r0, #5
	bl 0x02009af4
	movs r0, #10
	bl 0x02009a7c
	movs r0, #5
	movs r1, #1
	bl 0x02009aec
	movs r0, #1
	movs r1, #1
	bl 0x02009aec
	movs r0, #0
	movs r1, #0
	bl 0x02009aec
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009b54
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009b54
	movs r1, #128
	movs r2, #16
	movs r0, #5
	lsls r1, r1, #7
	bl 0x02009b54
	movs r1, #129
	movs r0, #13
	lsls r1, r1, #1
	bl 0x02009b64
	movs r1, #3
	movs r0, #13
	bl 0x02009b0c
	movs r0, #10
	bl 0x02009a7c
	movs r0, #13
	movs r1, #0
	movs r2, #6
	bl 0x02009b44
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x02009b5c
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x02009b5c
	movs r1, #128
	movs r2, #40
	movs r0, #5
	lsls r1, r1, #1
	bl 0x02009b5c
	movs r1, #4
	movs r0, #13
	bl 0x02009af4
	movs r0, #10
	bl 0x02009a7c
	movs r2, #6
	movs r0, #13
	movs r1, #0
	bl 0x02009b44
	movs r0, #13
	movs r1, #1
	bl 0x02009b0c
	movs r0, #13
	movs r1, #0
	movs r2, #6
	bl 0x02009b44
	movs r1, #176
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #10
	bl 0x02009b54
	movs r0, #5
	movs r1, #0
	movs r2, #6
	bl 0x02009b44
	movs r1, #192
	movs r2, #10
	movs r0, #13
	lsls r1, r1, #6
	bl 0x02009b54
	movs r1, #3
	movs r0, #13
	bl 0x02009af4
	movs r0, #6
	bl 0x02009a7c
	movs r0, #13
	movs r1, #0
	movs r2, #10
	bl 0x02009b44
	movs r0, #0
	movs r1, #2
	movs r2, #0
	bl 0x02009afc
	movs r0, #1
	movs r1, #2
	movs r2, #0
	bl 0x02009afc
	movs r0, #5
	movs r1, #2
	movs r2, #10
	bl 0x02009afc
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b54
	movs r1, #208
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #20
	bl 0x02009b54
	movs r0, #1
	movs r1, #0
	movs r2, #6
	bl 0x02009b44
	movs r1, #160
	movs r2, #10
	movs r0, #13
	lsls r1, r1, #7
	bl 0x02009b54
	movs r1, #3
	movs r0, #13
	bl 0x02009af4
	movs r0, #16
	bl 0x02009a7c
	movs r0, #0
	movs r1, #5
	movs r2, #40
	bl 0x02009b1c
	movs r2, #10
	movs r0, #5
	movs r1, #0
	bl 0x02009b44
	movs r0, #13
	movs r1, #2
	bl 0x02009b0c
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #6
	movs r2, #10
	bl 0x02009b54
	movs r0, #13
	movs r1, #0
	movs r2, #6
	bl 0x02009b44
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b54
	movs r1, #176
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #30
	bl 0x02009b54
	movs r0, #0
	ldr r1, [pc, #236]
	movs r2, #0
	bl 0x02009b5c
	movs r0, #1
	ldr r1, [pc, #224]
	movs r2, #0
	bl 0x02009b5c
	movs r2, #80
	movs r0, #5
	ldr r1, [pc, #212]
	bl 0x02009b5c
	movs r0, #13
	movs r1, #4
	bl 0x02009af4
	movs r1, #0
	movs r0, #13
	bl 0x02009b34
	movs r0, #0
	movs r1, #0
	bl 0x02009a94
	cmp r0, #0
	bne .L_02000170_4
	ldr r0, [pc, #184]
	bl 0x02009b2c
	b .L_02000170_5
.L_02000170_4:
	ldr r0, [pc, #180]
	bl 0x02009b2c
.L_02000170_5:
	movs r1, #0
	movs r2, #20
	movs r0, #13
	bl 0x02009b44
	ldr r5, [pc, #168]
	adds r0, r5, #0
	bl 0x02009b2c
	movs r2, #10
	movs r0, #1
	movs r1, #0
	bl 0x02009b54
	movs r0, #1
	movs r1, #2
	bl 0x02009b0c
	movs r1, #0
	movs r0, #1
	bl 0x02009b34
	movs r0, #0
	movs r1, #0
	bl 0x02009a94
	cmp r0, #0
	bne .L_02000170_6
	adds r0, r5, #1
	bl 0x02009b2c
	b .L_02000170_7
.L_02000170_6:
	adds r0, r5, #2
	bl 0x02009b2c
.L_02000170_7:
	movs r1, #0
	movs r2, #6
	movs r0, #1
	bl 0x02009b44
	ldr r0, [pc, #100]
	bl 0x02009b2c
	movs r1, #128
	movs r2, #10
	movs r0, #5
	lsls r1, r1, #8
	bl 0x02009b54
	movs r0, #5
	movs r1, #1
	bl 0x02009b0c
	movs r1, #0
	movs r0, #5
	bl 0x02009b34
	movs r0, #4
	bl 0x02009a7c
	movs r0, #0
	movs r1, #0
	bl 0x02009a94
	cmp r0, #1
	bne .L_02000170_8
	movs r0, #5
	movs r1, #2
	movs r2, #20
	bl 0x02009afc
	movs r0, #5
	movs r1, #0
	movs r2, #10
	bl 0x02009b44
	b .L_02000170_9
	.2byte 0x0000
	.4byte 0x00000fb1
	.4byte 0x00000fb2
	.4byte 0x00000103
	.4byte 0x00000105
	.4byte 0x00000fbd
	.4byte 0x00000fbe
	.4byte 0x00000fbf
	.4byte 0x00000fc2
.L_02000170_8:
	movs r0, #0
	movs r1, #3
	bl 0x02009aec
	movs r0, #1
	movs r1, #3
	bl 0x02009aec
	movs r1, #3
	movs r0, #5
	bl 0x02009af4
	movs r0, #8
	bl 0x02009a7c
	movs r0, #0
	movs r1, #0
	bl 0x02009aec
	ldr r3, [pc, #460]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02000170_9:
	movs r1, #3
	movs r0, #13
	bl 0x02009af4
	movs r0, #10
	bl 0x02009a7c
	movs r0, #13
	movs r1, #0
	movs r2, #10
	bl 0x02009b44
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b54
	movs r1, #208
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b54
	movs r1, #176
	movs r2, #10
	movs r0, #5
	lsls r1, r1, #8
	bl 0x02009b54
	movs r0, #0
	movs r1, #3
	bl 0x02009aec
	movs r0, #1
	movs r1, #3
	bl 0x02009aec
	movs r1, #3
	movs r0, #5
	bl 0x02009af4
	movs r0, #10
	bl 0x02009a7c
	movs r1, #0
	movs r0, #0
	bl 0x02009aec
	movs r0, #20
	bl 0x02009a7c
	movs r0, #13
	movs r1, #2
	bl 0x02009b0c
	movs r1, #0
	movs r0, #13
	bl 0x02009b34
	movs r0, #4
	bl 0x02009a7c
	movs r0, #0
	movs r1, #0
	bl 0x02009a94
	cmp r0, #0
	bne .L_02000170_10
	ldr r0, [pc, #308]
	bl 0x02009b2c
	b .L_02000170_11
.L_02000170_10:
	ldr r0, [pc, #304]
	bl 0x02009b2c
.L_02000170_11:
	movs r0, #10
	bl 0x02009a7c
	movs r0, #1
	movs r1, #2
	bl 0x02009b0c
	movs r0, #1
	movs r1, #0
	movs r2, #10
	bl 0x02009b54
	movs r0, #1
	movs r1, #0
	movs r2, #6
	bl 0x02009b44
	movs r1, #128
	movs r2, #10
	movs r0, #5
	lsls r1, r1, #8
	bl 0x02009b54
	movs r0, #5
	movs r1, #4
	bl 0x02009af4
	movs r2, #6
	movs r0, #5
	movs r1, #0
	bl 0x02009b44
	movs r0, #1
	movs r1, #2
	bl 0x02009b0c
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #10
	bl 0x02009b54
	movs r0, #1
	ldr r1, [pc, #216]
	movs r2, #30
	bl 0x02009b5c
	movs r0, #1
	movs r1, #0
	movs r2, #10
	bl 0x02009b44
	movs r1, #128
	movs r0, #13
	lsls r1, r1, #1
	movs r2, #40
	bl 0x02009b5c
	movs r0, #13
	movs r1, #4
	movs r2, #40
	bl 0x02009afc
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b54
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b54
	movs r1, #176
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #20
	bl 0x02009b54
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #10
	movs r0, #13
	bl 0x02009b54
	movs r0, #158
	bl 0x02009bd4
	ldr r0, [pc, #128]
	movs r1, #43
	movs r2, #8
	bl 0x02009a54
	movs r1, #128
	movs r2, #128
	movs r0, #13
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x02009aa4
	movs r0, #13
	movs r1, #232
	movs r2, #218
	bl 0x02009ad4
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl 0x02009ae4
	movs r0, #0
	ldr r1, [pc, #84]
	movs r2, #0
	bl 0x02009b5c
	movs r0, #1
	ldr r1, [pc, #76]
	movs r2, #0
	bl 0x02009b5c
	movs r0, #5
	ldr r1, [pc, #64]
	movs r2, #60
	bl 0x02009b5c
	ldr r3, [pc, #36]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #66
	str r2, [r3]
	bl 0x02009bbc
	bl 0x02009bc4
	movs r0, #13
	bl 0x02009b8c
	bl 0x02009a8c
.L_02000170_1:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00000fc6
	.4byte 0x00000fc9
	.4byte 0x00000103
	.4byte 0x0200a0ac
	.4byte 0x00000101
	.section .text.x02008be0,"ax",%progbits
	.balign 4
	.global Func_02000be0
	.thumb_func
Func_02000be0:
	push {r5, r6, r7, lr}
	ldr r0, [pc, #824]
	bl 0x02009a64
	cmp r0, #0
	beq .L_02000be0_0
	b .L_02000be0_1
.L_02000be0_0:
	bl 0x02009a84
	movs r0, #17
	bl 0x02009bd4
	ldr r0, [pc, #800]
	bl 0x02009a6c
	ldr r7, [pc, #800]
	adds r0, r7, #0
	bl 0x02009b2c
	movs r0, #14
	movs r1, #0
	movs r2, #10
	bl 0x02009b44
	movs r0, #0
	ldr r1, [pc, #784]
	ldr r2, [pc, #784]
	bl 0x02009aa4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #30
	bl 0x02009b5c
	movs r1, #196
	movs r2, #164
	movs r0, #0
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x02009ad4
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #10
	movs r0, #0
	bl 0x02009b54
	movs r0, #0
	bl 0x02009a9c
	movs r3, #10
	ldrsh r5, [r0, r3]
	movs r3, #18
	ldrsh r6, [r0, r3]
	lsls r5, r5, #16
	lsls r6, r6, #16
	movs r0, #5
	adds r1, r5, #0
	adds r2, r6, #0
	bl 0x02009ae4
	movs r0, #1
	adds r1, r5, #0
	adds r2, r6, #0
	bl 0x02009ae4
	movs r0, #5
	ldr r1, [pc, #696]
	ldr r2, [pc, #700]
	bl 0x02009aa4
	movs r0, #1
	ldr r1, [pc, #688]
	ldr r2, [pc, #688]
	bl 0x02009aa4
	movs r1, #188
	movs r2, #164
	movs r0, #5
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x02009acc
	movs r1, #204
	movs r2, #164
	lsls r2, r2, #1
	movs r0, #1
	lsls r1, r1, #1
	bl 0x02009ad4
	movs r0, #0
	movs r1, #0
	bl 0x02009aec
	movs r0, #5
	movs r1, #0
	bl 0x02009aec
	movs r0, #1
	movs r1, #0
	bl 0x02009aec
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b54
	movs r0, #5
	movs r1, #0
	movs r2, #20
	bl 0x02009b54
	movs r0, #0
	ldr r1, [pc, #612]
	movs r2, #0
	bl 0x02009b5c
	movs r0, #1
	ldr r1, [pc, #600]
	movs r2, #0
	bl 0x02009b5c
	movs r0, #5
	ldr r1, [pc, #592]
	movs r2, #30
	bl 0x02009b5c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009b54
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009b54
	movs r1, #128
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #7
	bl 0x02009b54
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl 0x02009b74
	movs r0, #215
	movs r1, #1
	movs r3, #1
	ldr r2, [pc, #532]
	negs r1, r1
	lsls r0, r0, #16
	bl 0x02009b7c
	bl 0x02009b84
	movs r0, #20
	bl 0x02009a7c
	movs r0, #61
	bl 0x02009bd4
	movs r0, #14
	movs r1, #4
	bl 0x02009af4
	movs r0, #14
	movs r1, #4
	bl 0x02009aec
	movs r0, #14
	movs r1, #0
	movs r2, #20
	bl 0x02009b44
	movs r0, #15
	movs r1, #0
	movs r2, #10
	bl 0x02009b54
	movs r2, #10
	movs r0, #15
	movs r1, #0
	bl 0x02009b44
	movs r0, #14
	movs r1, #3
	bl 0x02009af4
	movs r0, #14
	movs r1, #0
	movs r2, #10
	bl 0x02009b44
	movs r1, #128
	movs r2, #60
	movs r0, #15
	lsls r1, r1, #7
	bl 0x02009b54
	movs r0, #15
	movs r1, #1
	bl 0x02009b0c
	movs r2, #20
	movs r0, #15
	movs r1, #0
	bl 0x02009b44
	movs r0, #14
	movs r1, #3
	bl 0x02009af4
	movs r2, #10
	movs r0, #14
	movs r1, #0
	bl 0x02009b44
	movs r0, #15
	movs r1, #4
	bl 0x02009af4
	movs r0, #15
	movs r1, #0
	movs r2, #6
	bl 0x02009b44
	movs r1, #128
	movs r2, #0
	movs r0, #14
	lsls r1, r1, #1
	bl 0x02009b5c
	movs r1, #2
	movs r0, #14
	bl 0x02009b0c
	movs r0, #20
	bl 0x02009a7c
	movs r0, #15
	movs r1, #0
	movs r2, #10
	bl 0x02009b54
	movs r0, #15
	ldr r1, [pc, #336]
	movs r2, #40
	bl 0x02009b5c
	movs r0, #14
	movs r1, #0
	movs r2, #60
	bl 0x02009b54
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #40
	bl 0x02009b54
	movs r0, #14
	movs r1, #0
	movs r2, #40
	bl 0x02009b54
	movs r2, #180
	movs r0, #14
	movs r1, #232
	lsls r2, r2, #1
	bl 0x02009ad4
	movs r2, #10
	movs r0, #14
	movs r1, #0
	bl 0x02009b54
	movs r1, #3
	movs r0, #15
	bl 0x02009af4
	movs r0, #10
	bl 0x02009a7c
	movs r1, #196
	movs r2, #180
	movs r0, #14
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x02009acc
	movs r2, #180
	movs r0, #15
	movs r1, #216
	lsls r2, r2, #1
	bl 0x02009ad4
	movs r1, #188
	movs r2, #180
	movs r0, #15
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x02009acc
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009b54
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009b54
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009b54
	movs r1, #1
	movs r3, #1
	ldr r0, [pc, #188]
	negs r1, r1
	ldr r2, [pc, #188]
	bl 0x02009b7c
	movs r1, #196
	movs r2, #180
	movs r0, #14
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x02009acc
	movs r1, #188
	movs r2, #180
	lsls r2, r2, #1
	movs r0, #15
	lsls r1, r1, #1
	bl 0x02009ad4
	movs r0, #14
	movs r1, #0
	bl 0x02009aec
	movs r0, #15
	movs r1, #0
	bl 0x02009aec
	movs r1, #208
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b54
	movs r1, #208
	movs r2, #30
	movs r0, #15
	lsls r1, r1, #8
	bl 0x02009b54
	movs r0, #14
	movs r1, #2
	bl 0x02009b0c
	movs r0, #14
	movs r1, #0
	movs r2, #10
	bl 0x02009b44
	movs r1, #129
	movs r2, #60
	movs r0, #1
	lsls r1, r1, #1
	bl 0x02009b5c
	movs r0, #1
	movs r1, #1
	bl 0x02009b0c
	movs r2, #10
	movs r0, #1
	movs r1, #0
	bl 0x02009b44
	movs r0, #15
	movs r1, #4
	bl 0x02009af4
	movs r1, #0
	ldr r0, [pc, #60]
	bl 0x02009b34
	movs r0, #0
	movs r1, #0
	bl 0x02009a94
	cmp r0, #0
	bne .L_02000be0_2
	adds r0, r7, #0
	adds r0, #10
	bl 0x02009b2c
	b .L_02000be0_3
	.2byte 0x0000
	.4byte 0x00000808
	.4byte 0x00000f85
	.4byte 0x00013333
	.4byte 0x00009999
	.4byte 0x00000101
	.4byte 0x01590000
	.4byte 0x01890000
	.4byte 0x01530000
	.4byte 0x0000100f
.L_02000be0_2:
	adds r0, r7, #0
	adds r0, #11
	bl 0x02009b2c
.L_02000be0_3:
	movs r2, #10
	ldr r0, [pc, #828]
	movs r1, #0
	bl 0x02009b44
	movs r1, #2
	movs r0, #1
	bl 0x02009b0c
	ldr r5, [pc, #816]
	adds r0, r5, #0
	bl 0x02009b2c
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x02009b44
	movs r0, #14
	movs r1, #15
	movs r2, #40
	bl 0x02009b1c
	movs r1, #208
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b54
	movs r1, #208
	movs r0, #15
	lsls r1, r1, #8
	movs r2, #20
	bl 0x02009b54
	movs r2, #60
	movs r0, #14
	movs r1, #0
	bl 0x02009b44
	movs r1, #1
	movs r0, #15
	bl 0x02009b0c
	movs r0, #10
	bl 0x02009a7c
	movs r0, #15
	movs r1, #3
	bl 0x02009af4
	movs r2, #10
	ldr r0, [pc, #724]
	movs r1, #0
	bl 0x02009b44
	movs r0, #5
	movs r1, #2
	bl 0x02009b0c
	movs r0, #5
	movs r1, #3
	bl 0x02009af4
	movs r2, #20
	ldr r0, [pc, #708]
	movs r1, #0
	bl 0x02009b44
	movs r0, #14
	movs r1, #2
	bl 0x02009b0c
	movs r1, #160
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #20
	bl 0x02009b54
	movs r1, #0
	movs r0, #14
	bl 0x02009b34
	movs r0, #0
	movs r1, #0
	bl 0x02009a94
	cmp r0, #0
	bne .L_02000be0_4
	adds r0, r5, #5
	bl 0x02009b2c
	b .L_02000be0_5
.L_02000be0_4:
	adds r0, r5, #6
	bl 0x02009b2c
.L_02000be0_5:
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl 0x02009b54
	movs r1, #128
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #8
	bl 0x02009b54
	movs r0, #14
	movs r1, #2
	bl 0x02009b0c
	movs r0, #14
	movs r1, #0
	movs r2, #10
	bl 0x02009b44
	movs r0, #14
	movs r1, #1
	movs r2, #30
	bl 0x02009b14
	movs r0, #14
	movs r1, #5
	movs r2, #30
	bl 0x02009b14
	movs r2, #80
	movs r0, #14
	ldr r1, [pc, #584]
	bl 0x02009b5c
	movs r1, #4
	movs r0, #14
	bl 0x02009af4
	ldr r0, [pc, #576]
	bl 0x02009b2c
	movs r0, #14
	movs r1, #0
	movs r2, #6
	bl 0x02009b44
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x02009b5c
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x02009b5c
	movs r1, #128
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #1
	bl 0x02009b5c
	movs r0, #1
	movs r1, #1
	bl 0x02009b04
	movs r0, #5
	movs r1, #1
	bl 0x02009b04
	movs r1, #1
	movs r0, #0
	bl 0x02009b0c
	movs r0, #40
	bl 0x02009a7c
	movs r0, #5
	movs r1, #2
	bl 0x02009b0c
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009b54
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #10
	bl 0x02009b54
	movs r2, #10
	ldr r0, [pc, #452]
	movs r1, #0
	bl 0x02009b44
	movs r0, #15
	movs r1, #2
	bl 0x02009b0c
	movs r0, #15
	movs r1, #0
	movs r2, #10
	bl 0x02009b54
	ldr r0, [pc, #416]
	movs r1, #0
	movs r2, #10
	bl 0x02009b44
	movs r1, #128
	movs r2, #20
	movs r0, #14
	lsls r1, r1, #8
	bl 0x02009b54
	movs r0, #14
	movs r1, #4
	bl 0x02009af4
	movs r2, #10
	movs r0, #14
	movs r1, #0
	bl 0x02009b44
	movs r1, #2
	movs r0, #15
	bl 0x02009b0c
	movs r0, #10
	bl 0x02009a7c
	movs r1, #176
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b54
	movs r1, #208
	movs r0, #15
	lsls r1, r1, #8
	movs r2, #20
	bl 0x02009b54
	movs r1, #128
	movs r2, #128
	movs r0, #15
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x02009aa4
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #14
	bl 0x02009aa4
	movs r0, #14
	bl 0x02009a9c
	adds r1, r0, #0
	adds r1, #90
	ldrb r2, [r1]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r1]
	movs r0, #15
	bl 0x02009a9c
	adds r2, r0, #0
	adds r2, #90
	ldrb r3, [r2]
	ands r5, r3
	strb r5, [r2]
	movs r1, #196
	movs r2, #188
	movs r0, #14
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x02009acc
	movs r1, #188
	movs r2, #188
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #15
	bl 0x02009ad4
	movs r0, #6
	bl 0x02009a7c
	movs r0, #14
	bl 0x02009a9c
	adds r2, r0, #0
	adds r2, #90
	ldrb r3, [r2]
	movs r5, #1
	orrs r3, r5
	strb r3, [r2]
	movs r0, #15
	bl 0x02009a9c
	adds r2, r0, #0
	adds r2, #90
	ldrb r3, [r2]
	orrs r5, r3
	strb r5, [r2]
	movs r0, #14
	movs r1, #0
	bl 0x02009aec
	movs r1, #0
	movs r0, #15
	bl 0x02009aec
	movs r0, #20
	bl 0x02009a7c
	movs r0, #1
	movs r1, #2
	bl 0x02009b0c
	movs r0, #1
	movs r1, #0
	movs r2, #10
	bl 0x02009b44
	movs r2, #20
	movs r0, #0
	movs r1, #1
	bl 0x02009b54
	movs r0, #0
	movs r1, #3
	bl 0x02009aec
	movs r1, #3
	movs r0, #1
	bl 0x02009af4
	movs r0, #17
	bl 0x02009bd4
	movs r0, #1
	movs r1, #2
	bl 0x02009aec
	movs r0, #0
	bl 0x02009a9c
	cmp r0, #0
	beq .L_02000be0_6
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x02009ac4
.L_02000be0_6:
	movs r0, #1
	bl 0x02009adc
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x02009ae4
	movs r0, #5
	movs r1, #2
	bl 0x02009aec
	movs r0, #0
	bl 0x02009a9c
	cmp r0, #0
	beq .L_02000be0_7
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl 0x02009ac4
.L_02000be0_7:
	movs r0, #5
	bl 0x02009adc
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl 0x02009ae4
	ldr r5, [pc, #60]
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #9
	adds r2, r5, #0
	bl 0x02009b24
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #9
	adds r2, r5, #0
	bl 0x02009b24
	bl 0x02009bcc
	bl 0x02009a8c
.L_02000be0_1:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000100f
	.4byte 0x00000f91
	.4byte 0x00001005
	.4byte 0x00000105
	.4byte 0x00000f98
	.4byte 0x02009ce0
	.section .text.x02009760,"ax",%progbits
	.balign 4
	.global Func_02001760
	.thumb_func
Func_02001760:
	push {r5, lr}
	ldr r0, [pc, #528]
	bl 0x02009a64
	cmp r0, #0
	beq .L_02001760_0
	b .L_02001760_1
.L_02001760_0:
	ldr r0, [pc, #520]
	bl 0x02009a64
	cmp r0, #0
	beq .L_02001760_2
	bl 0x02009a84
	movs r1, #2
	movs r0, #11
	bl 0x02009b0c
	ldr r0, [pc, #500]
	bl 0x02009b2c
	movs r0, #11
	movs r1, #0
	bl 0x02009b3c
	bl 0x02009a8c
	b .L_02001760_1
.L_02001760_2:
	bl 0x02009a84
	movs r0, #11
	bl 0x02009ab4
	movs r1, #1
	movs r0, #11
	bl 0x02009b0c
	ldr r5, [pc, #468]
	adds r0, r5, #0
	bl 0x02009b2c
	movs r0, #11
	movs r1, #0
	movs r2, #20
	bl 0x02009b44
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #30
	bl 0x02009b5c
	movs r0, #196
	movs r1, #1
	movs r3, #1
	lsls r0, r0, #15
	negs r1, r1
	ldr r2, [pc, #432]
	bl 0x02009b7c
	movs r0, #0
	movs r1, #94
	ldr r2, [pc, #424]
	bl 0x02009ad4
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b54
	movs r0, #0
	bl 0x02009a9c
	cmp r0, #0
	beq .L_02001760_3
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x02009ae4
.L_02001760_3:
	movs r0, #1
	movs r1, #110
	ldr r2, [pc, #388]
	bl 0x02009ad4
	movs r1, #160
	movs r2, #40
	movs r0, #1
	lsls r1, r1, #8
	bl 0x02009b54
	movs r1, #2
	movs r0, #11
	bl 0x02009b0c
	movs r0, #40
	bl 0x02009a7c
	movs r1, #0
	movs r0, #11
	bl 0x02009b34
	movs r0, #0
	movs r1, #0
	bl 0x02009a94
	cmp r0, #0
	bne .L_02001760_4
	movs r1, #2
	movs r0, #11
	bl 0x02009b0c
	movs r0, #20
	bl 0x02009a7c
	adds r0, r5, #2
	bl 0x02009b2c
	movs r0, #11
	movs r1, #0
	bl 0x02009b3c
	ldr r0, [pc, #288]
	bl 0x02009a6c
	b .L_02001760_5
.L_02001760_4:
	movs r1, #2
	movs r0, #11
	bl 0x02009b0c
	movs r0, #20
	bl 0x02009a7c
	adds r0, r5, #3
	bl 0x02009b2c
	movs r0, #11
	movs r1, #0
	movs r2, #40
	bl 0x02009b44
	movs r2, #0
	movs r0, #11
	movs r1, #0
	bl 0x02009b14
	movs r0, #11
	movs r1, #1
	bl 0x02009aec
	movs r2, #40
	movs r0, #11
	movs r1, #4
	bl 0x02009afc
	movs r0, #11
	movs r1, #6
	bl 0x02009aec
	movs r0, #11
	ldr r1, [pc, #236]
	movs r2, #40
	bl 0x02009b5c
	movs r2, #10
	movs r0, #11
	movs r1, #0
	bl 0x02009b44
	movs r1, #1
	movs r0, #11
	bl 0x02009aec
	movs r0, #10
	bl 0x02009a7c
	movs r0, #11
	movs r1, #3
	bl 0x02009af4
	movs r2, #10
	movs r0, #11
	movs r1, #0
	bl 0x02009b44
	movs r0, #11
	movs r1, #3
	bl 0x02009af4
	ldr r5, [pc, #180]
	movs r0, #0
	ldr r1, [pc, #180]
	adds r2, r5, #0
	bl 0x02009b24
	adds r2, r5, #0
	movs r0, #1
	ldr r1, [pc, #168]
	bl 0x02009b24
	ldr r1, [pc, #168]
	movs r0, #11
	bl 0x02009abc
	movs r0, #0
	bl 0x02009ab4
	movs r0, #1
	bl 0x02009ab4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009b54
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #60
	bl 0x02009b54
	movs r0, #0
	ldr r1, [pc, #124]
	movs r2, #0
	bl 0x02009b5c
	movs r0, #1
	ldr r1, [pc, #116]
	movs r2, #120
	bl 0x02009b5c
	ldr r0, [pc, #64]
	bl 0x02009a6c
.L_02001760_5:
	movs r0, #1
	movs r1, #2
	bl 0x02009aec
	movs r0, #0
	bl 0x02009a9c
	cmp r0, #0
	beq .L_02001760_6
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x02009ac4
.L_02001760_6:
	movs r0, #1
	bl 0x02009adc
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x02009ae4
	bl 0x02009a8c
.L_02001760_1:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000839
	.4byte 0x0000082f
	.4byte 0x00000e8b
	.4byte 0x00000e85
	.4byte 0x011b0000
	.4byte 0x00000125
	.4byte 0x00000117
	.4byte 0x00000101
	.4byte 0x02009ce0
	.4byte 0x0001000b
	.4byte 0x02009bdc
	.4byte 0x00000105
	.section .rodata,"a",%progbits
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01260000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x004d0000
	.4byte 0x00000000
	.4byte 0x01490000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00460000
	.4byte 0x00000000
	.4byte 0x019f0000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.global Sukureta_Actor11Actions
Sukureta_Actor11Actions:
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000800
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000400
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xfffffe00
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000400
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xfffffe00
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Sukureta_StrangerActions
Sukureta_StrangerActions:
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Placement_Scripts
Placement_Scripts:
	.4byte 0xffff0000
	.4byte 0x000000e4
	.4byte 0x400000ac
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000001c6
	.4byte 0x80000122
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000001c6
	.4byte 0x800001bb
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x000000e5
	.4byte 0x400000e9
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x00000048
	.4byte 0x40000050
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x00000068
	.4byte 0x40000068
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x000000e8
	.4byte 0xc0000108
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Placement_Messages
Placement_Messages:
	.4byte 0x00000006
	.4byte 0x00103004
	.4byte 0x00204004
	.4byte 0x00302008
	.4byte 0x0040105d
	.4byte 0x0060305d
	.4byte 0x00b03003
	.4byte 0x00c04003
	.4byte 0x00d16008
	.4byte 0x000001ff
	.global Placement_Actors
Placement_Actors:
	.4byte 0xffff0127
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00024000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x00f00000
	.4byte 0x00000000
	.4byte 0x01610000
	.4byte 0x00024000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x01800000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00024000
	.4byte 0xffff006c
	.4byte 0x02009c34
	.4byte 0x00530000
	.4byte 0x00000000
	.4byte 0x01110000
	.4byte 0x00004000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x005c0000
	.4byte 0x00000000
	.4byte 0x010c0000
	.4byte 0x0001c000
	.4byte 0xffff001e
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00004000
	.4byte 0xffff0001
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
	.4byte 0xffff0020
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00035000
	.4byte 0xffff0021
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00033000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0001d000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Placement_Effects
Placement_Effects:
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x02008171
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020080dd
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00000f9d
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00000f9e
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000011f1
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008ba9
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x020092a1
	.4byte 0x00000002
	.4byte 0xffff0007
	.4byte 0x02008be1
	.4byte 0x00008f15
	.4byte 0xffff0008
	.4byte 0x00000000
	.4byte 0x00000023
	.4byte 0x0f410064
	.4byte 0x001000bc
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Placement_Effects834
Placement_Effects834:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001121
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001121
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x02009761
	.4byte 0x00000002
	.4byte 0x082f0014
	.4byte 0x02009761
	.4byte 0x00000002
	.4byte 0x087d000a
	.4byte 0x020099a5
	.4byte 0x00000002
	.4byte 0x087e000b
	.4byte 0x020099e9
	.4byte 0x00000023
	.4byte 0xffff0064
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Placement_Effects87a
Placement_Effects87a:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008ba9
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001c8c
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x02009a2d
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001ca0
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001ca6
	.4byte 0x00008f15
	.4byte 0xffff0008
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Sukureta_GateCells
Sukureta_GateCells:
	.4byte 0x00240002
	.4byte 0x00020002
	.4byte 0x00040001
	.4byte 0x00020024
	.4byte 0x00010002
	.2byte 0xffff
