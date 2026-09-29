.syntax unified
	.thumb
	.section .text.x0200807c,"ax",%progbits
	.balign 4
	.global Func_0200007c
	.thumb_func
Func_0200007c:
	push {lr}
	ldr r3, [pc, #28]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_0200007c_0
	ldr r0, [pc, #16]
	b .L_0200007c_1
.L_0200007c_0:
	ldr r0, [pc, #16]
.L_0200007c_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000004a
	.4byte 0x02009844
	.4byte 0x020097b4
	.section .text.x020080b8,"ax",%progbits
	.balign 4
	.global Func_020000b8
	.thumb_func
Func_020000b8:
	push {lr}
	ldr r3, [pc, #28]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_020000b8_0
	ldr r0, [pc, #16]
	b .L_020000b8_1
.L_020000b8_0:
	ldr r0, [pc, #16]
.L_020000b8_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000004a
	.4byte 0x02009a38
	.4byte 0x02009918
	.section .text.x02009180,"ax",%progbits
	.balign 4
	.global Func_02001180
	.thumb_func
Func_02001180:
	push {lr}
	ldr r3, [pc, #28]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_02001180_0
	ldr r0, [pc, #16]
	b .L_02001180_1
.L_02001180_0:
	ldr r0, [pc, #16]
.L_02001180_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000004a
	.4byte 0x02009c9c
	.4byte 0x02009b10
	.global Func_020011b0
	.thumb_func
Func_020011b0:
	push {r5, r6, lr}
	ldr r3, [pc, #684]
	movs r2, #224
	ldr r1, [r3]
	movs r3, #128
	lsls r3, r3, #1
	lsls r2, r2, #1
	str r3, [r1, r2]
	ldr r5, [pc, #672]
	ldrsh r2, [r5, r2]
	ldr r3, [pc, #672]
	sub sp, #8
	cmp r2, r3
	bne .L_020011b0_0
	movs r0, #169
	bl 0x02009600
	movs r0, #11
	movs r1, #5
	bl 0x02009558
	movs r0, #12
	movs r1, #5
	bl 0x02009558
	movs r0, #14
	movs r1, #2
	bl 0x02009558
	movs r3, #21
	movs r2, #73
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #21
	movs r1, #9
	movs r2, #1
	movs r3, #1
	bl 0x020094b8
	bl 0x02008870
	ldr r0, [pc, #616]
	bl 0x020094d0
	cmp r0, #0
	beq .L_020011b0_1
	movs r1, #136
	movs r2, #128
	movs r0, #13
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl 0x02009550
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl 0x020095a8
.L_020011b0_1:
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #2
	bne .L_020011b0_2
	ldr r0, [pc, #572]
	bl 0x020094e0
	b .L_020011b0_3
.L_020011b0_2:
	cmp r3, #3
	beq .L_020011b0_4
	b .L_020011b0_3
.L_020011b0_4:
	ldr r0, [pc, #560]
	bl 0x020094d0
	cmp r0, #0
	beq .L_020011b0_5
	b .L_020011b0_3
.L_020011b0_5:
	bl 0x020081ec
	b .L_020011b0_3
.L_020011b0_0:
	ldr r3, [pc, #548]
	cmp r2, r3
	beq .L_020011b0_7
	b .L_020011b0_3
.L_020011b0_7:
	movs r0, #14
	bl 0x02009508
	movs r1, #0
	bl 0x020094c0
	movs r0, #14
	bl 0x02009508
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r0, #128
	lsls r0, r0, #2
	bl 0x020094d0
	cmp r0, #0
	beq .L_020011b0_8
	movs r0, #14
	movs r1, #5
	bl 0x02009558
	bl 0x020090b8
.L_020011b0_8:
	ldr r0, [pc, #492]
	bl 0x020094d0
	cmp r0, #0
	beq .L_020011b0_9
	movs r0, #15
	movs r1, #4
	bl 0x02009558
	bl 0x02009144
.L_020011b0_9:
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r5, r2
	ldrh r3, [r3]
	movs r2, #128
	subs r3, #4
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bhi .L_020011b0_10
	ldr r0, [pc, #436]
	bl 0x020094e0
.L_020011b0_10:
	ldr r0, [pc, #448]
	bl 0x020094d0
	cmp r0, #0
	bne .L_020011b0_11
	ldr r0, [pc, #440]
	bl 0x020094d0
	cmp r0, #0
	bne .L_020011b0_11
	ldr r0, [pc, #408]
	bl 0x020094d0
	cmp r0, #0
	bne .L_020011b0_11
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x02009550
.L_020011b0_11:
	ldr r0, [pc, #388]
	bl 0x020094d0
	cmp r0, #0
	bne .L_020011b0_12
	ldr r0, [pc, #400]
	bl 0x020094d0
	cmp r0, #0
	beq .L_020011b0_12
	ldr r3, [pc, #360]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #2
	bne .L_020011b0_12
	movs r1, #0
	movs r2, #0
	movs r0, #11
	bl 0x02009550
	ldr r0, [pc, #340]
	bl 0x020094d8
	ldr r0, [pc, #364]
	bl 0x020094d8
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x02009550
.L_020011b0_12:
	ldr r0, [pc, #320]
	bl 0x020094d0
	cmp r0, #0
	beq .L_020011b0_13
	movs r3, #2
	str r3, [sp, #4]
	movs r5, #1
	movs r0, #54
	movs r1, #21
	movs r2, #53
	movs r3, #21
	str r5, [sp, #0]
	bl 0x020094b0
	movs r3, #21
	str r3, [sp, #4]
	movs r6, #17
	movs r0, #18
	movs r1, #20
	movs r2, #1
	movs r3, #3
	str r6, [sp, #0]
	bl 0x020094b8
	movs r0, #44
	movs r1, #18
	movs r2, #43
	movs r3, #17
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x020094b0
	movs r3, #7
	str r3, [sp, #0]
	movs r0, #8
	movs r1, #17
	movs r2, #1
	movs r3, #1
	str r6, [sp, #4]
	bl 0x020094b8
.L_020011b0_13:
	ldr r0, [pc, #260]
	bl 0x020094d0
	cmp r0, #0
	beq .L_020011b0_14
	ldr r0, [pc, #224]
	bl 0x020094d0
	cmp r0, #0
	bne .L_020011b0_14
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl 0x02009550
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl 0x02009550
	movs r1, #192
	movs r2, #132
	movs r0, #8
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl 0x02009550
	movs r1, #164
	movs r2, #140
	movs r0, #9
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl 0x02009550
	movs r1, #184
	movs r2, #152
	movs r0, #10
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl 0x02009550
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #0
	bl 0x020095a8
	movs r1, #176
	movs r2, #0
	movs r0, #10
	lsls r1, r1, #8
	bl 0x020095a8
	ldr r1, [pc, #160]
	movs r0, #9
	bl 0x02009518
	movs r0, #9
	bl 0x02009508
	ldr r3, [pc, #152]
	str r3, [r0, #24]
.L_020011b0_14:
	ldr r0, [pc, #112]
	bl 0x020094d0
	cmp r0, #0
	bne .L_020011b0_15
	movs r1, #164
	movs r2, #140
	lsls r2, r2, #17
	movs r0, #9
	lsls r1, r1, #16
	bl 0x02009550
	ldr r1, [pc, #120]
	movs r0, #9
	bl 0x02009518
	movs r0, #9
	bl 0x02009508
	ldr r3, [pc, #108]
	str r3, [r0, #24]
.L_020011b0_15:
	ldr r3, [pc, #60]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #5
	bne .L_020011b0_3
	ldr r0, [pc, #92]
	bl 0x020094d0
.L_020011b0_6:
	cmp r0, #0
	bne .L_020011b0_3
	ldr r0, [pc, #52]
	bl 0x020094d0
	cmp r0, #0
	bne .L_020011b0_3
	ldr r0, [pc, #32]
	bl 0x020094d0
	cmp r0, #0
	bne .L_020011b0_3
	bl 0x02008b2c
.L_020011b0_3:
	movs r0, #0
	sub sp, #-8
	pop {r5, r6}
	pop {r1}
	bx r1
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x00000058
	.4byte 0x000008b2
	.4byte 0x0000012f
	.4byte 0x00000109
	.4byte 0x0000004a
	.4byte 0x00000201
	.4byte 0x0000089a
	.4byte 0x00000895
	.4byte 0x000008b3
	.4byte 0x02009730
	.4byte 0xffff0000
	.4byte 0x000008b1
	.section .rodata,"a",%progbits
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x000000f0
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000012c
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000010
	.global YamaRama_BoulderCells
YamaRama_BoulderCells:
	.4byte 0x002e0042
	.4byte 0x00020003
	.4byte 0x003f0005
	.4byte 0x0003002e
	.4byte 0x00050002
	.4byte 0x0042ffff
	.4byte 0x0003002e
	.4byte 0x00050002
	.4byte 0x002e0045
	.4byte 0x00020003
	.4byte 0xffff0005
	.4byte 0xffff0000
	.4byte 0x000001f0
	.4byte 0x800000d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000d8
	.4byte 0xc0000208
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000088
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000090
	.4byte 0xc00000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x000000e8
	.4byte 0x40000160
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
	.4byte 0x000001f0
	.4byte 0x800000d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000001f0
	.4byte 0x800000d8
	.4byte 0x00100000
	.4byte 0x02080020
	.4byte 0x000001a0
	.4byte 0xffff0002
	.4byte 0x00000180
	.4byte 0x40000048
	.4byte 0x00100000
	.4byte 0x02080020
	.4byte 0x000001a0
	.4byte 0xffff0003
	.4byte 0x00000028
	.4byte 0x00000118
	.4byte 0x00100000
	.4byte 0x02080020
	.4byte 0x000001a0
	.4byte 0xffff0004
	.4byte 0x00000058
	.4byte 0x400000f8
	.4byte 0x00100000
	.4byte 0x02080020
	.4byte 0x000001a0
	.4byte 0xffff0005
	.4byte 0x000000d8
	.4byte 0x400000f8
	.4byte 0x00100000
	.4byte 0x02080020
	.4byte 0x000001a0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global YamaRama_Exits
YamaRama_Exits:
	.4byte 0x00000058
	.4byte 0x00111002
	.4byte 0x0020303d
	.4byte 0x0000004a
	.4byte 0x0010f002
	.4byte 0x00232002
	.4byte 0x00333002
	.4byte 0x00406050
	.4byte 0x00507050
	.4byte 0x00603058
	.4byte 0x000001ff
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
	.4byte 0xffff003d
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0001c000
	.4byte 0xffff009e
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0xffff009e
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00004000
	.4byte 0xffff009e
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00024000
	.4byte 0xffff009e
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00024000
	.4byte 0xffff009e
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00014000
	.4byte 0xffff0125
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00024000
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff003d
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0001c000
	.4byte 0xffff0029
	.4byte 0x02009730
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0x08b20027
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00018000
	.4byte 0x08b20127
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01140000
	.4byte 0x00008000
	.4byte 0x18950082
	.4byte 0x02009620
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00004000
	.4byte 0x18950083
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00004000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00004000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00c20000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008181
	.4byte 0x00000000
	.4byte 0x08b20009
	.4byte 0x00001957
	.4byte 0x00000000
	.4byte 0x08b2000a
	.4byte 0x020080e9
	.4byte 0x00000000
	.4byte 0x08b2000b
	.4byte 0x02008141
	.4byte 0x00000000
	.4byte 0x08b2000c
	.4byte 0x00001960
	.4byte 0x00000000
	.4byte 0x08b2000d
	.4byte 0x02008161
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001a04
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001a05
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001a06
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001a07
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001a08
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x020088e1
	.4byte 0x00008d15
	.4byte 0x08b20009
	.4byte 0x00001964
	.4byte 0x00008d15
	.4byte 0x08b2000a
	.4byte 0x00001965
	.4byte 0x00008d15
	.4byte 0x08b2000b
	.4byte 0x00001966
	.4byte 0x00008d15
	.4byte 0x08b2000c
	.4byte 0x00001967
	.4byte 0x00008d15
	.4byte 0x08b2000d
	.4byte 0x00001968
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001a0b
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001a0c
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001a0d
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001a0e
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001a0f
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001a1f
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x020088a9
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02008871
	.4byte 0x00000003
	.4byte 0xffff0005
	.4byte 0x0200884d
	.4byte 0x0000c403
	.4byte 0x0350006e
	.4byte 0x00300000
	.4byte 0x0000e403
	.4byte 0x0350006e
	.4byte 0x00300000
	.4byte 0x0000a403
	.4byte 0x0350006e
	.4byte 0x00300000
	.4byte 0x00008403
	.4byte 0x0350006e
	.4byte 0x00300000
	.4byte 0x00000013
	.4byte 0x0f7a0064
	.4byte 0x001000bd
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
	.4byte 0xffff0005
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000005
	.4byte 0x00000002
	.4byte 0x08b0000a
	.4byte 0x02008925
	.4byte 0x00000000
	.4byte 0x0895000a
	.4byte 0x02008ac1
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000019d4
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000019d6
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000019d5
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001a20
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001a21
	.4byte 0x00008f15
	.4byte 0x08b2000b
	.4byte 0x02008c31
	.4byte 0x00008d15
	.4byte 0x0895000a
	.4byte 0x000018bc
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000019d7
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000019d9
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000019d8
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001a22
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001a23
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008ff1
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02009029
	.4byte 0x10001815
	.4byte 0x0200000e
	.4byte 0x02009091
	.4byte 0x00001815
	.4byte 0x0200000e
	.4byte 0x020090b9
	.4byte 0x00000c15
	.4byte 0x0201000f
	.4byte 0x02009145
	.4byte 0x00000003
	.4byte 0xffff0007
	.4byte 0x02008fcd
	.4byte 0x00000013
	.4byte 0x0f710064
	.4byte 0x001000bf
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
