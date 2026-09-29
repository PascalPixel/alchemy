.syntax unified
	.thumb
	.section .text.x0200835c,"ax",%progbits
	.balign 4
	.global Func_0200035c
	.thumb_func
Func_0200035c:
	push {r5, r6, lr}
	ldr r0, [pc, #484]
	sub sp, #8
	bl 0x0200b6b8
	ldr r3, [pc, #480]
	movs r2, #224
	ldr r1, [r3]
	movs r3, #129
	lsls r2, r2, #1
	lsls r3, r3, #2
	str r3, [r1, r2]
	ldr r3, [pc, #468]
	ldrsh r2, [r3, r2]
	ldr r3, [pc, #468]
	cmp r2, r3
	beq .L_0200035c_0
	b .L_0200035c_1
.L_0200035c_0:
	movs r0, #162
	lsls r0, r0, #1
	bl 0x0200b6b8
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #452]
	bl 0x0200b638
	movs r0, #0
	movs r1, #1
	bl 0x0200b790
	movs r0, #1
	movs r1, #1
	bl 0x0200b790
	movs r0, #2
	movs r1, #1
	bl 0x0200b790
	movs r0, #3
	movs r1, #1
	bl 0x0200b790
	movs r0, #5
	movs r1, #1
	bl 0x0200b790
	movs r0, #20
	movs r1, #1
	bl 0x0200b790
	movs r0, #21
	movs r1, #1
	bl 0x0200b790
	movs r0, #22
	movs r1, #1
	bl 0x0200b790
	movs r0, #23
	movs r1, #1
	bl 0x0200b790
	movs r0, #24
	movs r1, #1
	bl 0x0200b790
	movs r0, #8
	movs r1, #1
	bl 0x0200b790
	movs r0, #9
	movs r1, #1
	bl 0x0200b790
	movs r0, #10
	movs r1, #1
	bl 0x0200b790
	movs r0, #11
	movs r1, #1
	bl 0x0200b790
	movs r0, #12
	movs r1, #1
	bl 0x0200b790
	movs r0, #13
	movs r1, #1
	bl 0x0200b790
	movs r5, #14
	movs r6, #0
.L_0200035c_2:
	movs r1, #1
	adds r0, r5, #0
	bl 0x0200b790
	adds r0, r5, #0
	bl 0x0200b6f0
	movs r3, #4
	adds r0, #85
	strb r3, [r0]
	adds r0, r5, #0
	bl 0x0200b6f0
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #2
	orrs r3, r2
	strb r3, [r0]
	adds r0, r5, #0
	bl 0x0200b6f0
	ldr r3, [pc, #276]
	adds r5, #1
	str r3, [r0, #12]
	cmp r5, #19
	bls .L_0200035c_2
	ldr r0, [pc, #272]
	bl 0x0200b6b0
	cmp r0, #0
	beq .L_0200035c_3
	bl 0x020088cc
	cmp r0, #0
	beq .L_0200035c_3
	bl 0x0200b6f0
	cmp r0, #0
	beq .L_0200035c_3
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_0200035c_3:
	movs r0, #9
	bl 0x0200b6f0
	movs r1, #0
	bl 0x0200b690
	movs r0, #10
	bl 0x0200b6f0
	movs r1, #0
	bl 0x0200b690
	movs r0, #11
	bl 0x0200b6f0
	movs r1, #0
	bl 0x0200b690
	movs r0, #12
	bl 0x0200b6f0
	movs r1, #0
	bl 0x0200b690
	movs r0, #13
	bl 0x0200b6f0
	movs r1, #0
	bl 0x0200b690
	movs r0, #12
	bl 0x0200b6f0
	ldr r5, [pc, #176]
	str r5, [r0, #24]
	movs r0, #13
	bl 0x0200b6f0
	ldr r3, [pc, #144]
	movs r2, #225
	str r5, [r0, #24]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #1
	bne .L_0200035c_4
	ldr r0, [pc, #144]
	bl 0x0200b6b0
	cmp r0, #0
	bne .L_0200035c_1
	bl 0x0200856c
	b .L_0200035c_1
.L_0200035c_4:
	cmp r3, #2
	bne .L_0200035c_5
	ldr r0, [pc, #132]
	bl 0x0200b6b0
	cmp r0, #0
	bne .L_0200035c_1
	ldr r3, [pc, #124]
	movs r2, #178
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #19
	str r2, [r3, #12]
	bl 0x0200b678
	movs r0, #1
	bl 0x0200b630
	movs r3, #5
	movs r2, #4
	str r3, [sp, #0]
	movs r0, #4
	movs r1, #70
	movs r3, #74
	str r2, [sp, #4]
	bl 0x0200b680
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200b728
	ldr r0, [pc, #60]
	bl 0x0200b6b0
	cmp r0, #0
	bne .L_0200035c_1
	bl 0x02009af0
	b .L_0200035c_1
.L_0200035c_5:
	cmp r3, #5
	bne .L_0200035c_1
	ldr r0, [pc, #48]
	bl 0x0200b6b8
.L_0200035c_1:
	movs r0, #0
	sub sp, #-8
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x00000111
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x0000003a
	.4byte 0x0200b4bd
	.4byte 0xffcd8000
	.4byte 0x00000109
	.4byte 0xffff0000
	.4byte 0x00000251
	.4byte 0x03001e70
	.section .text.x02008b24,"ax",%progbits
	.balign 4
	.global Func_02000b24
	.thumb_func
Func_02000b24:
	push {r5, r6, lr}
	bl 0x0200b6d8
	movs r0, #17
	bl 0x0200b6f0
	adds r0, #85
	ldrb r2, [r0]
	movs r3, #250
	ands r3, r2
	strb r3, [r0]
	ldr r1, [pc, #672]
	movs r0, #0
	ldr r2, [pc, #672]
	bl 0x0200b6f8
	movs r0, #1
	ldr r1, [pc, #660]
	ldr r2, [pc, #660]
	bl 0x0200b6f8
	movs r0, #2
	ldr r1, [pc, #648]
	ldr r2, [pc, #652]
	bl 0x0200b6f8
	movs r0, #3
	ldr r1, [pc, #640]
	ldr r2, [pc, #640]
	bl 0x0200b6f8
	movs r0, #0
	bl 0x0200b6f0
	cmp r0, #0
	beq .L_02000b24_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200b728
.L_02000b24_0:
	movs r0, #0
	bl 0x0200b6f0
	cmp r0, #0
	beq .L_02000b24_1
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #2
	bl 0x0200b728
.L_02000b24_1:
	movs r0, #0
	bl 0x0200b6f0
	cmp r0, #0
	beq .L_02000b24_2
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #3
	bl 0x0200b728
.L_02000b24_2:
	movs r0, #1
	bl 0x0200b630
	movs r1, #172
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #232
	bl 0x0200b710
	movs r1, #164
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #232
	bl 0x0200b710
	movs r1, #172
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #248
	bl 0x0200b710
	movs r1, #164
	lsls r1, r1, #1
	movs r2, #248
	movs r0, #3
	bl 0x0200b710
	movs r0, #0
	bl 0x0200b720
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #0
	bl 0x0200b788
	movs r0, #1
	bl 0x0200b720
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200b788
	movs r0, #2
	bl 0x0200b720
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #2
	bl 0x0200b788
	movs r0, #3
	bl 0x0200b720
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #3
	bl 0x0200b788
	movs r0, #50
	bl 0x0200b6d0
	movs r1, #2
	movs r0, #1
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200b7a8
	movs r0, #164
	movs r1, #160
	movs r2, #176
	lsls r1, r1, #14
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #17
	bl 0x0200b7b0
	bl 0x0200b7c0
	movs r3, #0
	adds r0, #85
	movs r1, #192
	movs r2, #192
	strb r3, [r0]
	lsls r1, r1, #9
	movs r0, #1
	lsls r2, r2, #8
	bl 0x0200b6f8
	movs r1, #164
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #216
	bl 0x0200b718
	bl 0x0200b7b8
	ldr r1, [pc, #356]
	ldr r2, [pc, #356]
	movs r0, #1
	bl 0x0200b6f8
	movs r0, #60
	bl 0x0200b6d0
	movs r0, #172
	movs r1, #192
	movs r2, #232
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #13
	lsls r2, r2, #16
	bl 0x0200b7b0
	movs r1, #128
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #6
	bl 0x0200b788
	bl 0x0200b7b8
	movs r1, #4
	movs r0, #1
	bl 0x0200b738
	ldr r0, [pc, #304]
	bl 0x0200b768
	movs r2, #20
	movs r0, #1
	movs r1, #0
	bl 0x0200b780
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #3
	bl 0x0200b7a0
	movs r0, #60
	bl 0x0200b6d0
	movs r1, #192
	movs r2, #192
	movs r0, #3
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200b6f8
	movs r1, #164
	lsls r1, r1, #1
	movs r2, #232
	movs r0, #3
	bl 0x0200b718
	movs r0, #10
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x0200b780
	movs r1, #4
	movs r0, #3
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #3
	movs r1, #0
	movs r2, #30
	bl 0x0200b780
	movs r1, #0
	movs r2, #0
	movs r0, #2
	bl 0x0200b750
	movs r0, #60
	bl 0x0200b6d0
.L_02000d22:
	movs r1, #168
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #248
	bl 0x0200b718
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #0
	bl 0x0200b788
	movs r0, #30
	bl 0x0200b6d0
	movs r0, #2
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r0, #3
	movs r1, #0
	movs r2, #30
	bl 0x0200b788
	movs r1, #0
	movs r0, #3
	bl 0x0200b770
	movs r0, #0
	movs r1, #0
	bl 0x0200b6e8
	cmp r0, #0
	bne .L_02000d22_0
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200b798
	movs r1, #128
	movs r2, #10
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200b788
	movs r0, #1
	movs r1, #3
	bl 0x0200b738
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #192
	movs r2, #0
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200b788
	movs r1, #4
	movs r0, #1
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #1
	movs r1, #0
	bl 0x0200b780
	ldr r3, [pc, #28]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #2
	strh r3, [r2]
	b .L_02000d22_1
	.2byte 0x0000
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.2byte 0x159c
	.2byte 0x0000
	.4byte 0x03001ebc
.L_02000d22_0:
	ldr r3, [pc, #1016]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #2
	strh r3, [r2]
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #3
	ldr r1, [pc, #996]
	movs r2, #60
	bl 0x0200b798
	movs r1, #128
	movs r2, #10
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200b788
	movs r0, #1
	movs r1, #4
	bl 0x0200b738
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #192
	movs r2, #0
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200b788
	movs r1, #2
	movs r0, #1
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
.L_02000d22_1:
	movs r0, #3
	ldr r1, [pc, #920]
	movs r2, #60
	bl 0x0200b798
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #1
	movs r2, #0
	movs r0, #0
	bl 0x0200b750
	movs r0, #60
	bl 0x0200b6d0
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #128
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200b788
	movs r0, #0
	movs r1, #3
	bl 0x0200b730
	movs r1, #3
	movs r0, #1
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #1
	movs r1, #0
	bl 0x0200b780
	movs r0, #3
	movs r1, #2
	bl 0x0200b748
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x0200b780
	movs r0, #0
	movs r1, #3
	bl 0x0200b730
	movs r0, #1
	movs r1, #3
	bl 0x0200b730
	movs r1, #3
	movs r0, #2
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #4
	movs r0, #3
	bl 0x0200b738
	movs r0, #10
	bl 0x0200b6d0
	movs r2, #10
	movs r0, #3
	movs r1, #0
	bl 0x0200b788
	movs r0, #3
	movs r1, #16
	bl 0x0200b730
	movs r1, #0
	movs r2, #60
	movs r0, #3
	bl 0x0200b780
	movs r0, #17
	bl 0x0200b840
	movs r1, #216
	movs r2, #200
	movs r0, #5
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl 0x0200b728
	movs r0, #5
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #240
	movs r2, #160
	movs r0, #5
	lsls r1, r1, #15
	lsls r2, r2, #16
	bl 0x0200b728
	movs r2, #0
	movs r0, #5
	movs r1, #0
	bl 0x0200b788
	movs r0, #3
	movs r1, #1
	bl 0x0200b730
	movs r0, #0
	movs r1, #1
	bl 0x0200b740
	movs r0, #1
	movs r1, #1
	bl 0x0200b748
	movs r0, #0
	movs r1, #0
	movs r2, #10
	bl 0x0200b788
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200b788
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200b788
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200b788
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #5
	bl 0x0200b788
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200b788
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #5
	bl 0x0200b788
	movs r1, #128
	movs r2, #5
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200b788
	movs r0, #0
	movs r1, #2
	bl 0x0200b740
	movs r1, #128
	movs r2, #10
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200b788
	movs r1, #2
	movs r0, #1
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200b7a8
	movs r0, #240
	movs r2, #168
	movs r3, #1
	lsls r2, r2, #16
	ldr r1, [pc, #520]
	lsls r0, r0, #15
	bl 0x0200b7b0
	bl 0x0200b7b8
	movs r0, #40
	bl 0x0200b6d0
	movs r1, #1
	movs r0, #21
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #61
	bl 0x0200b840
	movs r2, #20
	movs r0, #21
	movs r1, #0
	bl 0x0200b780
	movs r1, #1
	movs r0, #23
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #23
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #192
	movs r0, #23
	lsls r1, r1, #8
	movs r2, #60
	bl 0x0200b788
	movs r0, #23
	movs r1, #0
	movs r2, #20
	bl 0x0200b788
	movs r0, #23
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #132
	movs r2, #144
	movs r0, #1
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200b728
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #164
	movs r2, #216
	lsls r2, r2, #16
	movs r0, #1
	lsls r1, r1, #17
	bl 0x0200b728
	movs r1, #4
	movs r0, #23
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #23
	movs r1, #0
	bl 0x0200b780
	movs r1, #3
	movs r0, #23
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #23
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #160
	movs r0, #23
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200b788
	movs r1, #128
	movs r2, #10
	movs r0, #21
	lsls r1, r1, #6
	bl 0x0200b788
	movs r1, #1
	movs r0, #21
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #128
	movs r0, #20
	lsls r1, r1, #8
	movs r2, #30
	bl 0x0200b788
	movs r2, #30
	movs r0, #21
	movs r1, #0
	bl 0x0200b788
	movs r0, #5
	movs r1, #2
	bl 0x0200b748
	movs r1, #4
	movs r0, #21
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #128
	movs r2, #10
	movs r0, #21
	lsls r1, r1, #6
	bl 0x0200b788
	movs r1, #3
	movs r0, #21
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #23
	movs r1, #3
	bl 0x0200b738
	movs r0, #21
	ldr r1, [pc, #204]
	ldr r2, [pc, #204]
	bl 0x0200b6f8
	movs r1, #104
	movs r2, #168
	movs r0, #21
	bl 0x0200b718
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #23
	movs r1, #0
	movs r2, #20
	bl 0x0200b788
	movs r0, #20
	movs r1, #0
	movs r2, #10
	bl 0x0200b788
	movs r1, #129
	movs r0, #5
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b798
	movs r1, #129
	movs r0, #20
	lsls r1, r1, #1
	movs r2, #70
	bl 0x0200b798
	movs r1, #132
	movs r2, #144
	movs r0, #22
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200b728
	movs r0, #22
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #148
	movs r2, #240
	movs r0, #22
	lsls r1, r1, #17
	lsls r2, r2, #15
	bl 0x0200b728
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
.L_02001196:
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl 0x0200b788
	movs r2, #0
	movs r0, #20
	movs r1, #0
	bl 0x0200b788
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200b7a8
	movs r0, #232
	movs r1, #160
	b .L_02001196_0
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x0101
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xffe8
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
.L_02001196_0:
	movs r2, #152
	movs r3, #1
	lsls r0, r0, #16
	lsls r1, r1, #14
	lsls r2, r2, #16
	bl 0x0200b7b0
	movs r0, #22
	ldr r1, [pc, #1016]
	ldr r2, [pc, #1020]
	bl 0x0200b6f8
	movs r1, #136
	movs r0, #22
	lsls r1, r1, #1
	movs r2, #128
	bl 0x0200b718
	movs r1, #132
	movs r0, #22
	lsls r1, r1, #1
	movs r2, #152
	bl 0x0200b718
	movs r1, #140
	movs r0, #22
	lsls r1, r1, #1
	movs r2, #168
	bl 0x0200b718
	bl 0x0200b7b8
	movs r1, #160
	movs r2, #20
	lsls r1, r1, #7
	movs r0, #22
	bl 0x0200b788
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #0
	movs r0, #23
	bl 0x0200b778
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #3
	movs r0, #22
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #22
	movs r1, #0
	bl 0x0200b778
	movs r0, #148
	movs r1, #160
	movs r2, #216
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #14
	lsls r2, r2, #16
	bl 0x0200b7b0
	movs r1, #152
	movs r0, #22
	lsls r1, r1, #1
	movs r2, #176
	bl 0x0200b718
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #22
	bl 0x0200b788
	bl 0x0200b7b8
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #0
	movs r1, #1
	bl 0x0200b740
	movs r0, #1
	movs r1, #1
	bl 0x0200b740
	movs r0, #2
	movs r1, #1
	bl 0x0200b740
	movs r1, #1
	movs r0, #3
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #3
	movs r0, #22
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #0
	movs r0, #22
	bl 0x0200b778
	movs r0, #10
	bl 0x0200b6d0
	movs r0, #0
	ldr r1, [pc, #796]
	movs r2, #0
	bl 0x0200b798
	movs r0, #1
	ldr r1, [pc, #788]
	movs r2, #0
	bl 0x0200b798
	movs r0, #3
	ldr r1, [pc, #776]
	movs r2, #0
	bl 0x0200b798
	movs r2, #70
	movs r0, #2
	ldr r1, [pc, #764]
	bl 0x0200b798
	movs r1, #1
	movs r0, #22
	bl 0x0200b748
	movs r0, #30
	bl 0x0200b6d0
	movs r1, #0
	movs r0, #22
	bl 0x0200b778
	movs r0, #10
	bl 0x0200b6d0
	movs r0, #0
	ldr r1, [pc, #732]
	movs r2, #0
	bl 0x0200b798
	movs r0, #1
	ldr r1, [pc, #724]
	movs r2, #0
	bl 0x0200b798
	movs r0, #2
	ldr r1, [pc, #712]
	movs r2, #0
	bl 0x0200b798
	movs r2, #70
	movs r0, #3
	ldr r1, [pc, #700]
	bl 0x0200b798
	movs r1, #3
	movs r0, #22
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #0
	movs r0, #22
	bl 0x0200b778
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #0
	movs r0, #23
	bl 0x0200b778
	movs r0, #10
	bl 0x0200b6d0
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #22
	bl 0x0200b788
	movs r0, #40
	bl 0x0200b6d0
	movs r1, #0
	movs r0, #22
	bl 0x0200b778
	movs r0, #30
	bl 0x0200b6d0
	movs r1, #0
	movs r0, #23
	bl 0x0200b778
	movs r0, #10
	bl 0x0200b6d0
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #22
	bl 0x0200b788
	movs r0, #30
	bl 0x0200b6d0
	movs r1, #0
	movs r0, #22
	bl 0x0200b778
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #1
	bl 0x0200b6f0
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	movs r1, #164
	strb r3, [r0]
	lsls r1, r1, #1
	movs r0, #1
	movs r2, #224
	bl 0x0200b710
	movs r1, #172
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #224
	bl 0x0200b710
	movs r1, #172
	lsls r1, r1, #1
	movs r2, #232
	movs r0, #2
	bl 0x0200b710
	movs r0, #1
	bl 0x0200b720
	movs r0, #1
	bl 0x0200b6f0
	adds r0, #90
	ldrb r3, [r0]
	movs r6, #1
	orrs r3, r6
	movs r1, #192
	strb r3, [r0]
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200b788
	movs r0, #0
	bl 0x0200b720
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #0
	bl 0x0200b788
	movs r0, #2
	bl 0x0200b720
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #2
	bl 0x0200b788
	movs r0, #30
	bl 0x0200b6d0
	movs r1, #168
	movs r2, #200
	movs r0, #23
.L_0200145a:
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl 0x0200b728
	movs r0, #23
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #208
	movs r2, #200
	lsls r2, r2, #16
	movs r0, #23
	lsls r1, r1, #15
	bl 0x0200b728
	movs r1, #1
	movs r0, #22
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #22
	movs r1, #0
	bl 0x0200b780
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200b7a8
	movs r0, #240
	movs r2, #168
	movs r3, #1
	lsls r2, r2, #16
	lsls r0, r0, #15
	ldr r1, [pc, #364]
	bl 0x0200b7b0
	bl 0x0200b7b8
	movs r1, #3
	movs r0, #23
	bl 0x0200b738
	movs r0, #10
	bl 0x0200b6d0
	movs r1, #208
	movs r2, #20
	movs r0, #23
	lsls r1, r1, #8
	bl 0x0200b788
	movs r1, #3
	movs r0, #23
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #23
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r0, #20
	lsls r1, r1, #6
	movs r2, #70
	bl 0x0200b788
	movs r1, #20
	movs r2, #0
	movs r0, #5
	bl 0x0200b750
	movs r0, #50
	bl 0x0200b6d0
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r2, #20
	movs r0, #20
	lsls r1, r1, #6
	bl 0x0200b788
	movs r1, #4
	movs r0, #5
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #128
	movs r0, #23
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200b798
	movs r0, #23
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r0, #5
	ldr r1, [pc, #200]
	movs r2, #60
	bl 0x0200b798
	movs r1, #208
	movs r2, #20
	movs r0, #21
	lsls r1, r1, #8
	bl 0x0200b788
	movs r0, #21
	movs r1, #1
	bl 0x0200b748
	movs r1, #3
	movs r0, #23
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #23
	movs r1, #0
	bl 0x0200b780
	movs r1, #4
	movs r0, #23
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #23
	movs r1, #0
	bl 0x0200b780
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #21
	bl 0x0200b7a0
	movs r0, #60
	bl 0x0200b6d0
	movs r1, #4
	movs r0, #21
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #21
	movs r1, #0
	bl 0x0200b780
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #5
	bl 0x0200b7a0
	movs r0, #60
	bl 0x0200b6d0
	movs r2, #40
	movs r0, #5
	movs r1, #0
	bl 0x0200b780
	movs r1, #3
	movs r0, #21
	bl 0x0200b738
	movs r0, #60
	bl 0x0200b6d0
	movs r1, #160
	movs r2, #10
	movs r0, #5
	lsls r1, r1, #8
	bl 0x0200b788
	movs r1, #3
	movs r0, #20
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #20
	movs r1, #0
	b .L_0200145a_0
	.2byte 0x0000
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.2byte 0x0107
	.2byte 0x0000
	.4byte 0x00000105
	.4byte 0xffe80000
.L_0200145a_0:
	movs r2, #30
	bl 0x0200b780
	movs r0, #5
	ldr r1, [pc, #908]
	ldr r2, [pc, #912]
	bl 0x0200b6f8
	movs r0, #20
	ldr r1, [pc, #900]
	ldr r2, [pc, #900]
	bl 0x0200b6f8
	movs r0, #5
	movs r1, #128
	movs r2, #144
	bl 0x0200b710
	movs r0, #20
	movs r1, #120
	movs r2, #136
	bl 0x0200b718
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #20
	bl 0x0200b788
	movs r0, #5
	bl 0x0200b720
	movs r0, #5
	movs r1, #0
	movs r2, #20
	bl 0x0200b788
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #20
	movs r0, #21
	bl 0x0200b788
	movs r0, #21
	bl 0x0200b6f0
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #88
	movs r2, #152
	movs r0, #21
	bl 0x0200b718
	movs r0, #21
	bl 0x0200b6f0
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #176
	orrs r3, r6
	strb r3, [r0]
	movs r2, #20
	movs r0, #23
	lsls r1, r1, #8
	bl 0x0200b788
	movs r0, #23
	movs r1, #3
	bl 0x0200b730
	movs r1, #3
	movs r0, #21
	bl 0x0200b738
	movs r0, #40
	bl 0x0200b6d0
	movs r1, #192
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #10
	movs r0, #23
	bl 0x0200b6f8
	movs r0, #23
	bl 0x0200b6f0
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r0, #40]
	movs r0, #152
	bl 0x0200b840
	movs r0, #23
	bl 0x0200b6f0
	adds r0, #85
	ldrb r2, [r0]
	movs r3, #126
	ands r3, r2
	strb r3, [r0]
	movs r0, #23
	bl 0x0200b6f0
	movs r1, #0
	bl 0x0200b690
	movs r0, #17
	bl 0x0200b6f0
	movs r3, #4
	adds r0, #85
	strb r3, [r0]
	movs r2, #168
	movs r1, #104
	movs r0, #23
	bl 0x0200b708
	movs r0, #23
	bl 0x0200b6f0
	movs r1, #1
	bl 0x0200b690
	movs r0, #23
	bl 0x0200b6f0
	movs r3, #3
	adds r0, #85
	strb r3, [r0]
	movs r1, #0
	movs r0, #23
	movs r2, #30
	bl 0x0200b788
	movs r0, #21
	movs r1, #0
	movs r2, #10
	bl 0x0200b788
	movs r2, #20
	movs r0, #21
	movs r1, #0
	bl 0x0200b780
	movs r0, #5
	movs r1, #2
	bl 0x0200b748
	movs r1, #4
	movs r0, #5
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #5
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r2, #20
	movs r0, #23
	movs r1, #0
	bl 0x0200b780
	movs r0, #17
	movs r1, #0
	bl 0x0200b790
	movs r0, #18
	movs r1, #0
	bl 0x0200b790
	bl 0x0200b208
	movs r0, #17
	movs r1, #1
	bl 0x0200b790
	movs r0, #18
	movs r1, #1
	bl 0x0200b790
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b788
	movs r0, #152
	movs r1, #128
	movs r2, #216
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #14
	lsls r2, r2, #16
	bl 0x0200b7b0
	bl 0x0200b7b8
	movs r1, #136
	movs r2, #140
	movs r0, #20
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200b728
	movs r0, #20
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r2, #0
	movs r0, #20
	movs r1, #0
	bl 0x0200b728
	movs r1, #1
	movs r0, #1
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #192
	movs r2, #192
	movs r0, #1
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200b6f8
	movs r1, #156
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #216
	bl 0x0200b718
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #10
	bl 0x0200b788
	movs r0, #22
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #156
	movs r0, #22
	lsls r1, r1, #1
	movs r2, #184
	bl 0x0200b718
	movs r1, #192
	movs r0, #22
	lsls r1, r1, #6
	movs r2, #20
	bl 0x0200b788
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200b798
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	movs r0, #1
	bl 0x0200b6f8
	movs r0, #1
	bl 0x0200b6f0
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #164
	ands r5, r3
	movs r2, #224
	lsls r1, r1, #1
	strb r5, [r0]
	movs r0, #1
	bl 0x0200b718
	movs r0, #1
	bl 0x0200b6d0
	movs r0, #1
	bl 0x0200b6f0
	adds r0, #90
	ldrb r3, [r0]
	orrs r6, r3
	strb r6, [r0]
	movs r1, #1
	movs r0, #0
	bl 0x0200b740
	movs r0, #1
	movs r1, #1
	bl 0x0200b740
	movs r0, #2
	movs r1, #1
	bl 0x0200b740
	movs r1, #1
	movs r0, #3
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #1
	movs r0, #22
	bl 0x0200b748
	movs r0, #10
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #22
	movs r1, #0
	bl 0x0200b780
	movs r1, #2
	movs r0, #1
	bl 0x0200b748
	movs r0, #10
	bl 0x0200b6d0
	movs r1, #4
	movs r0, #1
	bl 0x0200b738
	movs r0, #10
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #1
	movs r1, #0
	bl 0x0200b780
	movs r1, #1
	movs r0, #2
	bl 0x0200b748
	movs r0, #10
	bl 0x0200b6d0
	movs r1, #3
	movs r0, #2
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #2
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r0, #22
	ldr r1, [pc, #80]
	movs r2, #60
	bl 0x0200b798
	movs r1, #0
	movs r0, #22
	bl 0x0200b770
	movs r0, #0
	movs r1, #0
	bl 0x0200b6e8
	cmp r0, #0
	bne .L_0200145a_1
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #4
	movs r0, #22
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #22
	movs r1, #0
	bl 0x0200b780
	ldr r3, [pc, #28]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200145a_2
	.4byte 0x0000b333
	.4byte 0x00005999
	.4byte 0x00000101
	.4byte 0x03001ebc
.L_0200145a_1:
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #4
	movs r0, #22
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	ldr r3, [pc, #256]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #22
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
.L_0200145a_2:
	movs r1, #2
	movs r0, #22
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #22
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #164
	movs r2, #200
	movs r0, #22
	lsls r1, r1, #1
	bl 0x0200b718
	movs r0, #22
	movs r1, #2
	bl 0x0200b748
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #20
	movs r0, #22
	bl 0x0200b788
	movs r0, #22
	bl 0x0200b6f0
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #168
	strb r3, [r0]
	lsls r1, r1, #1
	movs r2, #208
	movs r0, #22
	bl 0x0200b718
	movs r0, #1
	bl 0x0200b6d0
	movs r0, #22
	bl 0x0200b6f0
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	movs r1, #129
	strb r3, [r0]
	lsls r1, r1, #1
	movs r0, #22
	movs r2, #60
	bl 0x0200b798
	movs r0, #3
	ldr r1, [pc, #116]
	movs r2, #60
	bl 0x0200b798
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r0, #22
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #160
	movs r2, #20
	movs r0, #22
	lsls r1, r1, #7
	bl 0x0200b788
	movs r1, #2
	movs r0, #22
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #22
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #168
	movs r2, #216
	movs r0, #22
	lsls r1, r1, #1
	bl 0x0200b718
	ldr r0, [pc, #44]
	movs r1, #2
	bl 0x0200b7d8
	ldr r3, [pc, #40]
	ldr r2, [pc, #40]
	adds r3, r3, r2
	movs r2, #3
	strb r2, [r3]
	movs r0, #36
	movs r1, #2
	bl 0x0200b7d0
	bl 0x0200b6e0
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00000101
	.4byte 0x0000003a
	.4byte 0x02000240
	.4byte 0x0000022b
	.section .text.x0200b4bc,"ax",%progbits
	.balign 4
	.global Func_020034bc
	.thumb_func
Func_020034bc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #8
	bl 0x0200b6f0
	ldr r3, [pc, #100]
	mov r10, r0
	ldr r5, [r3]
	bl 0x0200b640
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #4
	lsrs r3, r3, #16
	lsls r3, r3, #16
	adds r5, #232
	mov r8, r3
	movs r0, #2
	ldrsh r3, [r5, r0]
	cmp r3, #129
	bgt .L_020034bc_0
	ldr r3, [pc, #72]
	ldr r3, [r3]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_020034bc_1
	movs r1, #152
	movs r2, #144
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #8
	bl 0x0200b728
	movs r0, #8
	bl 0x0200b6f0
	movs r5, #128
	lsls r5, r5, #9
	b .L_020034bc_2
.L_020034bc_1:
	movs r1, #152
	movs r2, #151
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #8
	bl 0x0200b728
	movs r0, #8
	bl 0x0200b6f0
	ldr r5, [pc, #20]
.L_020034bc_2:
	str r5, [r0, #24]
	movs r0, #8
	bl 0x0200b6f0
	str r5, [r0, #28]
	b .L_020034bc_3
	.4byte 0x03001e70
	.4byte 0x03001e40
	.4byte 0x00014ccc
.L_020034bc_0:
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #12
	lsls r2, r2, #12
	bl 0x0200b728
.L_020034bc_3:
	mov r1, r10
	cmp r1, #0
	beq .L_020034bc_4
	ldr r3, [pc, #160]
	ldr r6, [r3]
	movs r3, #15
	ands r6, r3
	cmp r6, #0
	bne .L_020034bc_4
	mov r0, r10
	ldr r2, [r0, #12]
	ldr r1, [r1, #8]
	movs r3, #128
	lsls r3, r3, #12
	add r2, r8
	adds r1, r1, r3
	adds r2, r2, r3
	ldr r3, [r0, #16]
	movs r0, #142
	lsls r0, r0, #1
	bl 0x0200b670
	movs r1, #192
	lsls r1, r1, #11
	adds r7, r0, #0
	mov r0, r8
	bl 0x0200b628
	mov r8, r0
	mov r1, r8
	lsls r1, r1, #16
	mov r8, r1
	cmp r7, #0
	beq .L_020034bc_4
	ldr r1, [pc, #104]
	adds r0, r7, #0
	ldr r5, [r7, #80]
	bl 0x0200b668
	movs r1, #3
	adds r0, r7, #0
	bl 0x0200b760
	adds r3, r7, #0
	adds r3, #85
	strb r6, [r3]
	bl 0x0200b640
	ldr r3, [pc, #80]
	adds r2, r7, #0
	ands r3, r0
	adds r2, #100
	ldr r0, [pc, #60]
	strh r3, [r2]
	adds r3, r7, #0
	mov r9, r0
	adds r3, #102
	ldr r0, [pc, #64]
	strh r6, [r3]
	mov r2, r8
	ldr r3, [pc, #64]
	mov r1, r10
	ands r0, r2
	str r1, [r7, #104]
	str r3, [r7, #108]
	asrs r0, r0, #4
	bl 0x0200b648
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #3
	asrs r3, r3, #16
	str r3, [r7, #48]
	adds r3, r5, #0
	adds r3, #38
	mov r0, r9
	strb r0, [r3]
	mov r1, r10
	ldr r3, [r1, #80]
	ldrb r3, [r3, #9]
	movs r2, #12
	ands r2, r3
	b .L_020034bc_5
	.4byte 0x00000000
	.4byte 0x03001e40
	.4byte 0x0200bc54
	.4byte 0x0ffff000
	.4byte 0x000fffff
	.4byte 0x0200b461
.L_020034bc_5:
	ldrb r1, [r5, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r5, #9]
.L_020034bc_4:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
@ The compiler library links here from its licensed container.
	.section .text.x02008890,"ax",%progbits
	.balign 4
	.global MeasureFixedPointPositionDistance
	.thumb_func
MeasureFixedPointPositionDistance:
	push {r5, lr}
	ldmia r0!, {r5}
	ldmia r1!, {r3}
	ldmia r0!, {r4}
	subs r5, r5, r3
	ldmia r1!, {r3}
	ldr r2, [r1]
	subs r4, r4, r3
	ldr r3, [r0]
	subs r3, r3, r2
	asrs r5, r5, #16
	asrs r4, r4, #16
	asrs r3, r3, #16
	adds r0, r5, #0
	muls r0, r5
	adds r2, r4, #0
	muls r2, r4
	adds r1, r3, #0
	muls r1, r3
	adds r0, r0, r2
	adds r3, r1, #0
	adds r0, r0, r3
	ldr r3, [pc, #8]
	bl 0x0200b854
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x030001d8
	.section .rodata.part1,"a",%progbits
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.global gEffectScripts
gEffectScripts:
	.4byte 0x0200b884
	.4byte 0x0200b8bc
	.4byte 0x0200b8f4
	.global MakyuriChojo_ScriptTable
MakyuriChojo_ScriptTable:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000008
	.4byte 0x40000008
	.4byte 0x00000000
	.4byte 0x02600000
	.4byte 0x000001a8
	.4byte 0xffff0002
	.4byte 0x00000008
	.4byte 0x40000008
	.4byte 0x00000000
	.4byte 0x02600000
	.4byte 0x000001a8
	.4byte 0xffff0005
	.4byte 0x000001f8
	.4byte 0x40000098
	.4byte 0x00000000
	.4byte 0x02600000
	.4byte 0x000001a8
	.4byte 0xffff0033
	.4byte 0x000001f8
	.4byte 0x400000a8
	.4byte 0x00000000
	.4byte 0x02600000
	.4byte 0x000001a8
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global MakyuriChojo_MessageTable
MakyuriChojo_MessageTable:
	.4byte 0x0000003a
	.4byte 0x0010f039
	.4byte 0x000001ff
	.global MakyuriChojo_ActorTable
MakyuriChojo_ActorTable:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00026000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00022000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0x02530005
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00020000
	.4byte 0xffff00f4
	.4byte 0x00000007
	.4byte 0x01300000
	.4byte 0x00280000
	.4byte 0x00900000
	.4byte 0x00024000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00024000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x01024000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01024000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01024000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0x0253001e
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0x02530023
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00020000
	.4byte 0xffff0020
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0x02530021
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00020000
	.4byte 0xffff002b
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global MakyuriChojo_EventTable
MakyuriChojo_EventTable:
	.4byte 0x00000002
	.4byte 0x08800005
	.4byte 0x02008b25
	.4byte 0x00000002
	.4byte 0x02510006
	.4byte 0x0200addd
	.4byte 0x00000202
	.4byte 0xffff000b
	.4byte 0x02008929
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x02008ad1
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte 0x02008929
	.4byte 0x00000002
	.4byte 0x0250000a
	.4byte 0x020089fd
	.4byte 0x00000003
	.4byte 0x03500064
	.4byte 0x00300000
	.4byte 0x00000002
	.4byte 0xffff000c
	.4byte 0x0200aeb9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global MakyuriChojo_NearestActor
MakyuriChojo_NearestActor:
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00016666
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00016666
	.4byte 0x0000001c
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000001b
