.syntax unified
	.thumb
	.section .text.x02008248,"ax",%progbits
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
	.section .text.x020085e8,"ax",%progbits
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
	.section .text.x02009274,"ax",%progbits
	.global FieldScene_RunGroupChoreography
	.thumb_func
FieldScene_RunGroupChoreography:
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
	.section .rodata,"a",%progbits
	.4byte 0x01000000
	.4byte 0x02020101
	.4byte 0x03030302
	.4byte 0x05040404
	.4byte 0x06060505
	.4byte 0x01000006
	.4byte 0x03020201
	.4byte 0x05040403
	.global Data_0200a874
Data_0200a874:
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
	.global Data_0200aa48
Data_0200aa48:
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
	.global Data_0200ab2c
Data_0200ab2c:
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
	.global Data_0200ac00
Data_0200ac00:
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Data_0200ac14
Data_0200ac14:
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
	.global Data_0200ac90
Data_0200ac90:
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
	.global Data_0200acf8
Data_0200acf8:
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
	.global Data_0200ad74
Data_0200ad74:
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
	.global Data_0200adf0
Data_0200adf0:
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
	.global Data_0200ae34
Data_0200ae34:
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
	.global Data_0200aef0
Data_0200aef0:
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
	.global Data_0200af50
Data_0200af50:
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
	.global Data_0200af78
Data_0200af78:
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
	.global Data_0200afa0
Data_0200afa0:
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
	.global Data_0200b108
Data_0200b108:
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
	.global Data_0200b144
Data_0200b144:
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
	.global Data_0200b170
Data_0200b170:
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
	.global Data_0200b380
Data_0200b380:
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
	.global Data_0200b560
Data_0200b560:
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
	.global Data_0200b7d0
Data_0200b7d0:
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
	.global Data_0200b938
Data_0200b938:
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
	.global Data_0200ba64
Data_0200ba64:
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
	.global Data_0200bb30
Data_0200bb30:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200bb3c
Data_0200bb3c:
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
	.global Data_0200bcec
Data_0200bcec:
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
	.global Value_0200beb4
Value_0200beb4:
	.4byte 0x00390000
	.4byte 0x00020002
	.4byte 0x00000001
	.4byte 0x0002003b
	.4byte 0x00010002
	.4byte 0x003b0002
	.4byte 0x00020002
	.4byte 0xffff0001
