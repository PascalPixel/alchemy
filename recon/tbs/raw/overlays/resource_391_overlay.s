.syntax unified
	.thumb
	.section .text.x020089dc,"ax",%progbits
	.global Func_020009dc
	.thumb_func
Func_020009dc:
	push {r5, lr}
	ldr r5, [pc, #24]
	ldr r3, [r5]
	cmp r3, #0
	beq .L_020009dc_0
	movs r1, #2
	bl 0x0200aba4
	movs r3, #0
	str r3, [r5]
.L_020009dc_0:
	movs r0, #1
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x0200b390
	.global Func_020009fc
	.thumb_func
Func_020009fc:
	push {r5, r6, lr}
	adds r5, r0, #0
	bl 0x0200ab9c
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
	ble .L_020009fc_0
	adds r0, r5, #0
	movs r1, #7
	bl 0x0200acac
	b .L_020009fc_1
.L_020009fc_0:
	adds r0, r5, #0
	movs r1, #10
	bl 0x0200acac
.L_020009fc_1:
	movs r2, #0
	ldrsh r3, [r6, r2]
	movs r2, #150
	lsls r2, r2, #3
	cmp r3, r2
	ble .L_020009fc_2
	movs r3, #0
	strh r3, [r6]
.L_020009fc_2:
	movs r0, #1
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_02000a4c
	.thumb_func
Func_02000a4c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200b06c
	.global Func_02000a54
	.thumb_func
Func_02000a54:
	movs r0, #0
	bx lr
	.global Func_02000a58
	.thumb_func
Func_02000a58:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200b0cc
	.global Func_02000a60
	.thumb_func
Func_02000a60:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200b0e4
	.global Func_02000a68
	.thumb_func
Func_02000a68:
	push {r5, lr}
	sub sp, #32
	bl 0x0200ac0c
	add r5, sp, #8
	adds r0, r5, #0
	bl 0x02008474
	cmp r0, #0
	beq 0x02008b0e
	mov r3, sp
	add r2, sp, #24
.L_02000a80:
	ldmia r2!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r3, [r5, #12]
	ldr r0, [r5]
	ldr r1, [r5, #4]
	ldr r2, [r5, #8]
	bl 0x02008608
	ldr r3, [r5, #4]
	cmp r3, #10
	bne 0x02008b0e
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #20
	bne 0x02008b0e
	movs r0, #10
	movs r1, #3
	bl 0x0200ac84
	movs r1, #18
	movs r2, #6
	negs r1, r1
	movs r0, #10
	bl 0x0200ac6c
	movs r0, #30
	bl 0x0200ac04
	movs r0, #240
	bl 0x0200ad24
	movs r1, #8
	movs r0, #10
	bl 0x0200ac84
	movs r0, #10
	bl 0x0200ac24
.L_02000acc:
	movs r3, #2
	adds r0, #35
	strb r3, [r0]
	movs r2, #17
	movs r3, #19
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #17
	movs r2, #2
	movs r3, #4
	bl 0x0200abdc
	movs r3, #4
	str r3, [sp, #0]
	movs r1, #20
	movs r2, #17
	movs r3, #1
	movs r5, #0
	movs r0, #2
	str r5, [sp, #4]
	bl 0x02008244
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200abfc
	movs r0, #10
	bl 0x0200ac24
	movs r1, #0
	bl 0x0200abec
	bl 0x0200ac14
	sub sp, #-32
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000b1c
	.thumb_func
Func_02000b1c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r5, r0, #0
	movs r0, #0
	bl 0x0200ac24
	adds r6, r0, #0
	adds r7, r6, #0
	adds r7, #85
	ldrb r3, [r7]
	adds r1, r5, #0
	mov r10, r3
	bl 0x0200abe4
	mov r8, r0
	cmp r0, #0
	bne 0x02008c02
	bl 0x0200ac0c
	movs r1, #6
	adds r0, r6, #0
	bl 0x0200aba4
	movs r0, #6
	bl 0x0200ab84
	movs r0, #152
	bl 0x0200ad24
	adds r0, r6, #0
	movs r1, #7
	bl 0x0200aba4
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r6, #48]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r6, #52]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r6, #40]
	ldrb r2, [r7]
	movs r3, #126
	ands r3, r2
	strb r3, [r7]
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200abec
	movs r3, #10
	ldrsh r2, [r5, r3]
	movs r3, #2
	ldrsh r1, [r5, r3]
	movs r0, #0
	bl 0x0200ac5c
	adds r0, r6, #0
	movs r1, #6
	bl 0x0200aba4
	adds r0, r6, #0
	movs r1, #1
	bl 0x0200abec
	mov r3, r8
	movs r1, #7
	strb r3, [r7]
	movs r0, #10
	bl 0x0200ac84
	ldr r5, [pc, #96]
	ldr r3, [r6, #12]
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r6, #20]
	adds r3, r3, r5
	str r3, [r6, #20]
	movs r0, #2
	bl 0x0200ab84
	ldr r3, [r6, #12]
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r6, #20]
	adds r3, r3, r5
	str r3, [r6, #20]
	movs r0, #10
	bl 0x0200ab84
	movs r5, #128
	ldr r3, [r6, #12]
	lsls r5, r5, #9
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r6, #20]
	adds r3, r3, r5
	movs r0, #4
	str r3, [r6, #20]
	bl 0x0200ab84
	ldr r3, [r6, #12]
	adds r3, r3, r5
	str r3, [r6, #12]
.L_02000bf0:
	ldr r3, [r6, #20]
	adds r3, r3, r5
	str r3, [r6, #20]
	mov r3, r10
	strb r3, [r7]
	bl 0x0200ac14
	movs r0, #1
	b 0x02008c04
.L_02000c02:
	movs r0, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.2byte 0xffff
	.global Func_02000c14
	.thumb_func
Func_02000c14:
	push {r5, lr}
	ldr r3, [pc, #64]
	movs r2, #250
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #12
	bl 0x0200ac24
	ldr r5, [pc, #52]
	ldr r2, [r0, #8]
	movs r4, #128
	lsls r4, r4, #12
	ands r2, r5
	mov r1, sp
	adds r3, r2, r4
	str r3, [r1]
	ldr r3, [r0, #12]
	str r3, [r1, #4]
	ldr r3, [r0, #16]
	ands r3, r5
	adds r3, r3, r4
	str r3, [r1, #8]
	movs r3, #160
	lsls r3, r3, #14
	adds r2, r2, r3
	str r2, [r1]
	adds r0, r1, #0
	bl 0x02008b1c
	sub sp, #-12
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0xfff00000
	.global Func_02000c60
	.thumb_func
Func_02000c60:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200b294
	.global Func_02000c68
	.thumb_func
Func_02000c68:
	push {r5, lr}
	movs r0, #10
	sub sp, #8
	bl 0x020088c0
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200abf4
	cmp r0, #0
	beq .L_02000c68_0
	movs r0, #10
	bl 0x0200ac24
	movs r3, #2
	adds r0, #35
	strb r3, [r0]
	movs r2, #17
	movs r3, #19
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #17
	movs r2, #2
	movs r3, #4
	bl 0x0200abdc
	movs r3, #4
	str r3, [sp, #0]
	movs r1, #20
	movs r2, #17
	movs r3, #1
	movs r5, #0
	movs r0, #2
	str r5, [sp, #4]
	bl 0x02008244
	movs r0, #10
	bl 0x0200ac24
	movs r1, #0
	bl 0x0200abec
.L_02000c68_0:
	movs r0, #8
	bl 0x020088c0
	movs r0, #9
	bl 0x020088c0
	ldr r3, [pc, #100]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #4
	bne .L_02000c68_1
	ldr r0, [pc, #88]
	bl 0x0200abf4
	cmp r0, #0
	bne .L_02000c68_1
	bl 0x02008d3c
.L_02000c68_1:
	ldr r0, [pc, #76]
	bl 0x0200abf4
	cmp r0, #0
	beq .L_02000c68_2
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl 0x0200ac7c
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl 0x0200ac7c
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl 0x0200ac7c
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl 0x0200ac7c
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl 0x0200ac7c
.L_02000c68_2:
	movs r0, #0
	sub sp, #-8
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000843
	.4byte 0x00000845
.L_02000d3c:
	.global Func_02000d3c
	.thumb_func
Func_02000d3c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	bl 0x0200ac0c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl 0x0200acec
	movs r0, #1
	bl 0x0200ab84
	movs r0, #246
	movs r1, #1
	movs r2, #151
	movs r3, #0
	lsls r2, r2, #18
	negs r1, r1
	lsls r0, r0, #16
	bl 0x0200acec
	movs r0, #3
	ldr r6, [pc, #860]
	bl 0x0200abf4
	str r0, [r6]
	movs r0, #13
	bl 0x0200ac24
	movs r1, #0
	bl 0x0200abec
	movs r0, #14
	bl 0x0200ac24
	movs r1, #0
	bl 0x0200abec
	movs r0, #15
	bl 0x0200ac24
	movs r1, #0
	bl 0x0200abec
	movs r0, #16
	bl 0x0200ac24
	movs r1, #0
	bl 0x0200abec
	movs r0, #17
	bl 0x0200ac24
	movs r1, #0
	bl 0x0200abec
	movs r0, #18
	bl 0x0200ac24
	movs r1, #0
	bl 0x0200abec
	movs r0, #19
	bl 0x0200ac24
	movs r1, #0
	bl 0x0200abec
	movs r0, #20
	bl 0x0200ac24
	movs r1, #0
	bl 0x0200abec
	movs r0, #21
	bl 0x0200ac24
	movs r1, #0
	bl 0x0200abec
	ldr r5, [pc, #748]
	movs r0, #17
	adds r1, r5, #0
	bl 0x0200ac34
	movs r0, #18
	adds r1, r5, #0
	bl 0x0200ac34
	movs r0, #19
	adds r1, r5, #0
	bl 0x0200ac34
	movs r0, #20
	adds r1, r5, #0
	bl 0x0200ac34
	movs r0, #21
	adds r1, r5, #0
	bl 0x0200ac34
	movs r1, #232
	lsls r1, r1, #15
	ldr r2, [pc, #708]
	movs r0, #0
	bl 0x0200ac7c
	movs r0, #1
	bl 0x0200ab84
	bl 0x0200abc4
	movs r0, #1
	bl 0x0200ab84
	bl 0x0200ad04
	bl 0x0200ad0c
	movs r0, #0
	ldr r1, [pc, #676]
	ldr r2, [pc, #680]
	bl 0x0200ac2c
	movs r0, #0
	movs r1, #254
	ldr r2, [pc, #672]
	bl 0x0200ac64
	movs r0, #1
	ldr r1, [pc, #668]
	ldr r2, [pc, #672]
	bl 0x0200ac2c
	movs r0, #2
	ldr r1, [pc, #660]
	ldr r2, [pc, #660]
	bl 0x0200ac2c
	movs r0, #0
	bl 0x0200ac24
	cmp r0, #0
	beq .L_02000d3c_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200ac7c
.L_02000d3c_0:
	movs r0, #0
	bl 0x0200ac24
	cmp r0, #0
	beq .L_02000d3c_1
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #2
	bl 0x0200ac7c
.L_02000d3c_1:
	ldr r1, [pc, #620]
	movs r0, #1
	bl 0x0200ac34
	ldr r1, [pc, #616]
	movs r0, #2
	bl 0x0200ac34
	ldr r3, [r6]
	cmp r3, #0
	beq .L_02000d3c_2
	movs r0, #3
	ldr r1, [pc, #588]
	ldr r2, [pc, #588]
	bl 0x0200ac2c
	movs r0, #0
	bl 0x0200ac24
	cmp r0, #0
	beq .L_02000d3c_3
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #3
	bl 0x0200ac7c
.L_02000d3c_3:
	ldr r1, [pc, #576]
	movs r0, #3
	bl 0x0200ac34
.L_02000d3c_2:
	movs r5, #128
	lsls r5, r5, #6
	movs r0, #2
	bl 0x0200ac3c
	adds r1, r5, #0
	movs r0, #2
	movs r2, #40
	bl 0x0200a780
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #2
	movs r2, #20
	bl 0x0200a780
	movs r2, #128
	lsls r2, r2, #7
	mov r8, r2
	movs r0, #2
.L_02000eea:
	mov r1, r8
	movs r2, #40
	bl 0x0200a780
	ldr r1, [pc, #524]
.L_02000ef4:
	movs r2, #0
	movs r0, #2
	bl 0x0200acdc
	movs r6, #192
	movs r0, #60
	bl 0x0200ac04
	lsls r6, r6, #7
	movs r0, #1
	mov r1, r8
	movs r2, #0
	bl 0x0200accc
	movs r0, #0
	adds r1, r6, #0
	movs r2, #60
	bl 0x0200a780
	adds r1, r5, #0
	movs r0, #3
	movs r2, #10
	bl 0x0200a780
	adds r1, r5, #0
	movs r5, #160
	lsls r5, r5, #8
	movs r0, #1
	movs r2, #0
	bl 0x0200accc
	adds r1, r5, #0
	movs r0, #0
	movs r2, #10
	bl 0x0200a780
	movs r0, #1
	ldr r1, [pc, #448]
	movs r2, #0
	bl 0x0200acdc
	ldr r1, [pc, #440]
	movs r2, #0
	movs r0, #0
	bl 0x0200acdc
	movs r0, #40
	bl 0x0200ac04
	movs r0, #1
	mov r1, r8
	movs r2, #0
	bl 0x0200accc
	movs r2, #10
	movs r0, #0
	adds r1, r6, #0
	bl 0x0200a780
	movs r1, #2
	movs r0, #1
	bl 0x0200aca4
	ldr r0, [pc, #400]
	bl 0x0200acb4
	movs r1, #10
	movs r0, #1
	bl 0x0200a768
	ldr r0, [pc, #388]
	bl 0x0200acb4
	movs r3, #192
	lsls r3, r3, #8
	mov r10, r3
	movs r2, #20
	movs r0, #2
	mov r1, r10
	bl 0x0200a780
	movs r0, #2
	movs r1, #3
	bl 0x0200ac8c
	movs r0, #2
	movs r1, #20
	bl 0x0200a768
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200a780
	adds r1, r5, #0
	movs r0, #0
	movs r2, #40
	bl 0x0200a780
	movs r0, #1
	mov r1, r8
	movs r2, #20
	bl 0x0200a780
	movs r0, #0
	adds r1, r6, #0
	movs r2, #30
	bl 0x0200a780
	movs r0, #1
	adds r1, r6, #0
	movs r2, #20
	bl 0x0200a780
	movs r1, #224
	movs r2, #30
	lsls r1, r1, #8
	movs r0, #0
	bl 0x0200a780
	movs r0, #2
	movs r1, #2
	bl 0x0200aca4
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #2
	bl 0x0200acdc
	movs r0, #40
	bl 0x0200ac04
	movs r0, #1
	mov r1, r8
	movs r2, #0
	bl 0x0200accc
	movs r0, #0
	adds r1, r6, #0
	movs r2, #20
	bl 0x0200a780
	movs r2, #10
	mov r1, r10
	movs r0, #2
	bl 0x0200a780
	movs r0, #17
	bl 0x0200ad24
	movs r0, #206
	bl 0x0200ad24
	movs r1, #0
	ldr r0, [pc, #224]
	bl 0x0200acf4
	movs r0, #1
	bl 0x0200acfc
	movs r0, #1
	bl 0x0200ab84
	ldr r2, [pc, #208]
	movs r3, #1
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, [pc, #204]
	bl 0x0200ab8c
	movs r0, #20
	bl 0x0200ab84
	ldr r0, [pc, #196]
	movs r1, #1
	bl 0x0200acf4
	movs r0, #128
	movs r1, #2
	lsls r0, r0, #9
	bl 0x0200acf4
	movs r0, #120
	bl 0x0200acfc
	movs r0, #60
	bl 0x0200ab84
	ldr r5, [pc, #168]
	movs r0, #0
	adds r1, r5, #0
	bl 0x0200ac34
	movs r0, #1
	adds r1, r5, #0
	bl 0x0200ac34
	movs r0, #2
	adds r1, r5, #0
	bl 0x0200ac34
	adds r1, r5, #0
	movs r0, #3
	bl 0x0200ac34
	movs r0, #100
	bl 0x0200ac04
	movs r0, #1
	movs r1, #20
	bl 0x0200a768
	movs r0, #2
	movs r1, #40
	bl 0x0200a768
	ldr r3, [pc, #40]
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02000ef4_0
	movs r0, #40
	bl 0x0200ac04
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #3
	movs r2, #0
	bl 0x0200acdc
	movs r0, #40
	bl 0x0200ac04
	movs r0, #3
	movs r1, #40
	bl 0x0200a768
	b .L_02000ef4_1
	.2byte 0x0000
	.4byte 0x0200b394
	.2byte 0xb024
	.2byte 0x0200
	.2byte 0x0000
	.2byte 0x025a
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.2byte 0x0251
	.2byte 0x0000
	.2byte 0x9999
	.2byte 0x0000
	.2byte 0x4ccc
	.2byte 0x0000
	.2byte 0xae20
	.2byte 0x0200
	.2byte 0xae54
	.2byte 0x0200
	.2byte 0xae88
	.2byte 0x0200
	.4byte 0x00000101
	.4byte 0x00001474
	.4byte 0x0000147c
	.4byte 0x00007fff
	.4byte 0x0200b398
	.4byte 0x0200a7c9
	.4byte 0x00405210
	.4byte 0x0200aebc
.L_02000ef4_0:
	ldr r3, [pc, #580]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02000ef4_1:
	movs r0, #20
	bl 0x0200ac04
	ldr r7, [pc, #564]
	ldr r3, [r7]
	cmp r3, #0
	beq .L_02000ef4_2
	movs r0, #3
	bl 0x0200ac24
	movs r5, #128
	lsls r5, r5, #10
	str r5, [r0, #40]
	movs r0, #10
	bl 0x0200ac04
	movs r0, #3
	adds r1, r5, #0
	adds r2, r5, #0
	bl 0x0200ac2c
	movs r1, #2
	movs r2, #0
	movs r0, #3
	negs r1, r1
	bl 0x0200ac6c
	ldr r1, [pc, #520]
	movs r0, #3
	bl 0x0200ac34
	movs r0, #3
	bl 0x0200ac24
	movs r1, #0
	bl 0x0200abec
	movs r0, #3
	movs r1, #19
	bl 0x0200ac84
	movs r0, #10
	bl 0x0200ac04
.L_02000ef4_2:
	movs r0, #0
	bl 0x0200ac24
	movs r5, #128
	lsls r5, r5, #10
	str r5, [r0, #40]
	movs r0, #10
	bl 0x0200ac04
	adds r2, r5, #0
	adds r1, r5, #0
	movs r0, #0
	bl 0x0200ac2c
	ldr r6, [pc, #456]
	movs r0, #0
	adds r1, r6, #0
	bl 0x0200ac34
	movs r0, #0
	bl 0x0200ac24
	movs r1, #0
	bl 0x0200abec
	movs r1, #19
	movs r0, #0
	bl 0x0200ac84
	movs r0, #20
	bl 0x0200ac04
	movs r0, #1
	bl 0x0200ac24
	str r5, [r0, #40]
	movs r0, #10
	bl 0x0200ac04
	adds r2, r5, #0
	adds r1, r5, #0
	movs r0, #1
	bl 0x0200ac2c
	adds r1, r6, #0
	movs r0, #1
	bl 0x0200ac34
	movs r0, #1
	bl 0x0200ac24
	movs r1, #0
	bl 0x0200abec
	movs r1, #19
	movs r0, #1
	bl 0x0200ac84
	movs r0, #40
	bl 0x0200ac04
	movs r0, #2
	bl 0x0200ac24
	str r5, [r0, #40]
	movs r0, #10
	bl 0x0200ac04
	adds r1, r6, #0
	movs r0, #2
	bl 0x0200ac34
	movs r0, #2
	bl 0x0200ac24
	movs r1, #0
	bl 0x0200abec
	movs r1, #19
	movs r0, #2
	bl 0x0200ac84
	ldr r3, [pc, #324]
	movs r5, #0
	str r5, [r3]
	movs r0, #160
	bl 0x0200ac04
	ldr r0, [pc, #316]
	bl 0x0200ab94
	movs r0, #120
	bl 0x0200ac04
	movs r1, #1
	ldr r0, [pc, #308]
	bl 0x0200acf4
	movs r0, #60
	bl 0x0200acfc
	movs r0, #60
	bl 0x0200ab84
	ldr r3, [pc, #292]
	ldr r2, [pc, #296]
	str r5, [r3]
	movs r3, #128
	ldr r5, [pc, #292]
	lsls r3, r3, #16
	str r3, [r2]
	movs r1, #200
	movs r3, #1
	str r3, [r5]
	lsls r1, r1, #4
	ldr r0, [pc, #284]
	bl 0x0200ab8c
	movs r0, #180
	bl 0x0200ac04
	movs r0, #21
	bl 0x0200ad24
	movs r0, #1
	movs r1, #80
	bl 0x0200a768
	movs r0, #2
	movs r1, #40
	bl 0x0200a768
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200acdc
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200acdc
	movs r1, #129
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200acdc
	movs r1, #129
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #3
	bl 0x0200acdc
	movs r0, #60
	bl 0x0200ac04
	movs r0, #2
	movs r1, #20
	bl 0x0200a768
	movs r3, #2
	str r3, [r5]
	movs r1, #2
	movs r0, #2
	bl 0x0200ac9c
	movs r0, #20
	bl 0x0200ac04
	movs r1, #1
	movs r0, #1
	bl 0x0200ac9c
	movs r0, #40
	bl 0x0200ac04
	movs r0, #0
	movs r1, #2
	bl 0x0200ac9c
	movs r1, #1
	movs r0, #3
	bl 0x0200ac9c
	movs r0, #20
	bl 0x0200ac04
	movs r1, #3
	movs r0, #2
	bl 0x0200ac9c
	movs r0, #40
	bl 0x0200ac04
	movs r1, #1
	movs r0, #0
	bl 0x0200ac9c
	movs r0, #20
	bl 0x0200ac04
	movs r1, #2
	movs r0, #1
	bl 0x0200ac9c
	movs r0, #20
	bl 0x0200ac04
	movs r0, #3
	movs r1, #2
	bl 0x0200ac9c
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200acdc
	movs r0, #1
	movs r1, #20
	bl 0x0200a768
	ldr r3, [r7]
	cmp r3, #0
	beq .L_02000ef4_3
	movs r1, #129
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200acdc
	movs r0, #3
	movs r1, #10
	bl 0x0200a768
	b .L_02000ef4_4
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x0200b394
	.4byte 0x0200af48
	.4byte 0x0200b398
	.4byte 0x0200a7c9
	.4byte 0x00406218
	.4byte 0x0200b388
	.4byte 0x0200b384
	.4byte 0x0200b38c
	.4byte 0x0200a975
.L_02000ef4_3:
	ldr r3, [pc, #940]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02000ef4_4:
	ldr r7, [pc, #928]
	movs r3, #3
	str r3, [r7]
	movs r0, #0
	bl 0x0200ac24
	adds r0, #35
	movs r6, #254
	ldrb r2, [r0]
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #1
	bl 0x0200ac24
	adds r0, #35
	ldrb r2, [r0]
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #2
	bl 0x0200ac24
	adds r0, #35
	ldrb r2, [r0]
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #3
	bl 0x0200ac24
	adds r0, #35
	ldrb r2, [r0]
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #3
	movs r0, #0
	bl 0x0200acd4
	movs r0, #1
	movs r1, #3
	bl 0x0200acd4
	movs r0, #2
	movs r1, #3
	bl 0x0200acd4
	movs r0, #3
	movs r1, #3
	bl 0x0200acd4
	ldr r3, [pc, #828]
	movs r5, #0
	movs r1, #200
	str r5, [r3]
	lsls r1, r1, #4
	ldr r0, [pc, #824]
	bl 0x0200ab8c
	movs r0, #220
	bl 0x0200ad24
	movs r0, #13
	bl 0x0200ac24
	adds r0, #35
	ldrb r2, [r0]
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #2
	movs r0, #13
	bl 0x0200acd4
	movs r1, #253
	ldr r2, [pc, #788]
	movs r0, #13
	lsls r1, r1, #16
	bl 0x0200ac7c
	ldr r5, [pc, #784]
	movs r0, #13
	adds r1, r5, #0
	bl 0x0200ac34
	movs r0, #14
	bl 0x0200ac24
	adds r0, #35
	ldrb r2, [r0]
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #2
	movs r0, #14
	bl 0x0200acd4
	movs r1, #233
	movs r0, #14
	lsls r1, r1, #16
	ldr r2, [pc, #748]
	bl 0x0200ac7c
	movs r0, #14
	adds r1, r5, #0
	bl 0x0200ac34
	ldr r3, [pc, #736]
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02000ef4_5
	movs r0, #15
	bl 0x0200ac24
	adds r0, #35
	ldrb r2, [r0]
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #2
	movs r0, #15
	bl 0x0200acd4
	movs r1, #207
	movs r0, #15
	lsls r1, r1, #16
	ldr r2, [pc, #704]
	bl 0x0200ac7c
	movs r0, #15
	adds r1, r5, #0
	bl 0x0200ac34
.L_02000ef4_5:
	movs r0, #16
	bl 0x0200ac24
	adds r0, #35
	ldrb r2, [r0]
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #2
	movs r0, #16
	bl 0x0200acd4
	movs r1, #227
	movs r2, #145
	movs r0, #16
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x0200ac7c
	movs r0, #16
	adds r1, r5, #0
	bl 0x0200ac34
	ldr r3, [r7]
	cmp r3, #0
	beq .L_02000ef4_6
	adds r5, r7, #0
.L_02000ef4_7:
	movs r0, #1
	bl 0x0200ab84
	ldr r3, [r5]
	cmp r3, #0
	bne .L_02000ef4_7
.L_02000ef4_6:
	movs r0, #150
	lsls r0, r0, #1
	bl 0x0200ac04
	ldr r0, [pc, #620]
	bl 0x0200ab94
	movs r0, #120
	bl 0x0200ac04
	movs r0, #17
	bl 0x0200ad24
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl 0x0200acf4
	movs r0, #60
	bl 0x0200acfc
	movs r0, #60
	bl 0x0200ab84
	movs r0, #13
	bl 0x0200ac44
	movs r0, #14
	bl 0x0200ac44
	ldr r7, [pc, #560]
	ldr r3, [r7]
	cmp r3, #0
.L_02001530:
	beq .L_02001530_0
	movs r0, #15
	bl 0x0200ac44
.L_02001530_0:
	movs r0, #16
	bl 0x0200ac44
	movs r0, #1
	bl 0x0200ab84
	ldr r5, [pc, #544]
	movs r0, #13
	adds r1, r5, #0
	bl 0x0200ac34
	movs r0, #14
	adds r1, r5, #0
	bl 0x0200ac34
	ldr r3, [r7]
	cmp r3, #0
	beq .L_02001530_1
	movs r0, #15
	adds r1, r5, #0
	bl 0x0200ac34
.L_02001530_1:
	adds r1, r5, #0
	movs r0, #16
	bl 0x0200ac4c
	movs r0, #80
	bl 0x0200ac04
	movs r1, #2
	movs r0, #1
	bl 0x0200aca4
	movs r0, #40
	bl 0x0200ac04
	movs r1, #0
.L_02001582:
	movs r0, #1
	bl 0x0200acbc
.L_02001588:
	movs r1, #220
	movs r2, #247
	movs r0, #11
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl 0x0200ac7c
	movs r1, #220
	movs r2, #247
	lsls r1, r1, #16
	movs r0, #12
	lsls r2, r2, #17
	bl 0x0200ac7c
	movs r0, #1
	bl 0x0200ab84
	movs r0, #11
	movs r1, #0
	bl 0x0200ac1c
	cmp r0, #1
	bne .L_02001588_0
	ldr r3, [pc, #392]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02001588_0:
	movs r1, #1
	movs r0, #0
	bl 0x0200aca4
	movs r0, #20
	bl 0x0200ac04
	movs r0, #2
	movs r1, #2
	bl 0x0200aca4
	movs r0, #2
	movs r1, #20
	bl 0x0200a768
	ldr r3, [r7]
	cmp r3, #0
	beq .L_02001588_1
	movs r1, #2
	movs r0, #3
	bl 0x0200aca4
	movs r0, #10
	bl 0x0200ac04
	ldr r0, [pc, #368]
	bl 0x0200acb4
	movs r0, #3
	movs r1, #40
	bl 0x0200a768
.L_02001588_1:
	movs r0, #1
	movs r1, #1
	bl 0x0200ac9c
	movs r2, #0
	ldr r1, [pc, #348]
	movs r0, #1
	bl 0x0200acdc
	movs r0, #80
	bl 0x0200ac04
	movs r1, #2
	movs r0, #2
	bl 0x0200aca4
	ldr r0, [pc, #332]
	bl 0x0200acb4
	movs r0, #2
	movs r1, #40
	bl 0x0200a768
	movs r1, #3
	movs r0, #1
	bl 0x0200aca4
	movs r0, #40
	bl 0x0200ac04
	movs r1, #2
	movs r0, #1
	bl 0x0200acd4
	movs r0, #1
	bl 0x0200ac24
	movs r2, #1
	adds r0, #35
	ldrb r3, [r0]
	mov r8, r2
	mov r2, r8
	orrs r3, r2
	strb r3, [r0]
	movs r0, #1
	bl 0x0200ac24
	movs r1, #1
	bl 0x0200abec
	movs r0, #1
	movs r1, #6
	movs r2, #0
	bl 0x0200ac94
	movs r1, #3
	movs r2, #0
	movs r0, #1
	negs r1, r1
	bl 0x0200ac6c
	movs r0, #1
	movs r1, #1
	bl 0x0200ac84
	movs r1, #128
	movs r2, #60
	lsls r1, r1, #7
	movs r0, #1
	bl 0x0200a780
	movs r0, #1
	movs r1, #20
	bl 0x0200a768
	movs r0, #1
	movs r1, #2
	bl 0x0200ac9c
	movs r5, #128
	movs r0, #1
	movs r1, #10
	bl 0x0200a768
	lsls r5, r5, #6
	movs r0, #0
	movs r1, #3
	bl 0x0200aca4
	movs r0, #1
	adds r1, r5, #0
	movs r2, #20
	bl 0x0200a780
	movs r6, #192
	ldr r1, [pc, #168]
	movs r2, #0
	movs r0, #1
	bl 0x0200acdc
	lsls r6, r6, #7
	movs r0, #40
	bl 0x0200ac04
	movs r0, #1
	adds r1, r6, #0
	movs r2, #40
	bl 0x0200a780
	movs r0, #1
	adds r1, r5, #0
	movs r2, #20
	bl 0x0200a780
	movs r0, #1
	adds r1, r6, #0
	movs r2, #20
	bl 0x0200a780
	movs r0, #1
	adds r1, r5, #0
	movs r2, #10
	bl 0x0200a780
	movs r1, #2
	movs r2, #0
	movs r0, #1
	bl 0x0200ac94
	movs r0, #40
	bl 0x0200ac04
	movs r1, #2
	movs r2, #0
	movs r0, #1
	bl 0x0200ac94
	movs r0, #10
	bl 0x0200ac04
	movs r1, #4
	movs r2, #0
	movs r0, #1
	bl 0x0200ac94
	movs r0, #20
	bl 0x0200ac04
	movs r0, #1
	movs r1, #20
	bl 0x0200a768
	ldr r3, [r7]
	cmp r3, #0
	beq .L_02001588_2
	b .L_02001588_3
	.2byte 0x0000
	.4byte 0x03001ebc
	.2byte 0xb38c
	.2byte 0x0200
	.2byte 0xb390
	.2byte 0x0200
	.2byte 0xaad9
	.2byte 0x0200
	.2byte 0x0000
	.2byte 0x025b
	.2byte 0xaf6c
	.2byte 0x0200
	.2byte 0x0000
	.2byte 0x0275
	.2byte 0xb394
	.2byte 0x0200
	.2byte 0x0000
	.2byte 0x0261
	.2byte 0xa975
	.2byte 0x0200
	.2byte 0xafc8
	.2byte 0x0200
	.4byte 0x00001488
	.4byte 0x00000101
	.4byte 0x00001489
.L_02001588_3:
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #3
	bl 0x0200acdc
	movs r0, #60
	bl 0x0200ac04
	movs r1, #2
	movs r0, #3
	bl 0x0200aca4
	movs r0, #80
	bl 0x0200ac04
	movs r1, #2
	movs r0, #3
	bl 0x0200acd4
	movs r0, #3
	bl 0x0200ac24
	adds r0, #35
	ldrb r3, [r0]
	mov r2, r8
	orrs r3, r2
	strb r3, [r0]
	movs r0, #3
	bl 0x0200ac24
	movs r1, #1
	bl 0x0200abec
	movs r0, #3
	movs r1, #4
	movs r2, #0
	bl 0x0200ac94
	movs r1, #2
	movs r2, #0
	movs r0, #3
	negs r1, r1
	bl 0x0200ac6c
	movs r0, #3
	movs r1, #1
	bl 0x0200ac84
	movs r1, #224
	movs r2, #60
	lsls r1, r1, #8
	movs r0, #3
	bl 0x0200a780
	movs r1, #2
	movs r0, #3
	bl 0x0200aca4
	movs r0, #20
	bl 0x0200ac04
	movs r0, #3
	movs r1, #20
	bl 0x0200a768
	b .L_02001588_4
.L_02001588_2:
	ldr r3, [pc, #632]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02001588_4:
	movs r6, #128
	movs r0, #1
	movs r1, #2
	movs r2, #0
	lsls r6, r6, #7
	bl 0x0200ac94
	movs r7, #128
	movs r2, #20
	movs r0, #1
	adds r1, r6, #0
	bl 0x0200a780
	lsls r7, r7, #6
	movs r0, #1
	movs r1, #3
	bl 0x0200ac8c
	movs r2, #10
	movs r0, #1
	adds r1, r7, #0
	bl 0x0200a780
	movs r0, #1
	movs r1, #20
	bl 0x0200a768
	movs r1, #3
	movs r0, #1
	bl 0x0200ac8c
	movs r0, #10
	bl 0x0200ac04
	movs r1, #1
	movs r0, #2
	bl 0x0200aca4
	movs r0, #40
	bl 0x0200ac04
	movs r1, #2
	movs r0, #2
	bl 0x0200aca4
	movs r0, #20
	bl 0x0200ac04
	movs r1, #2
	movs r0, #2
	bl 0x0200acd4
	movs r0, #2
	bl 0x0200ac24
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #2
	bl 0x0200ac24
	movs r1, #1
	bl 0x0200abec
	movs r2, #0
	movs r0, #2
	movs r1, #4
	bl 0x0200ac94
	movs r0, #2
	movs r1, #1
	bl 0x0200ac84
	movs r1, #192
	movs r2, #0
	movs r0, #2
	lsls r1, r1, #8
	bl 0x0200accc
	movs r1, #2
	movs r0, #0
	bl 0x0200aca4
	movs r0, #10
	bl 0x0200ac04
	movs r1, #2
	movs r0, #0
	bl 0x0200acd4
	movs r0, #0
	bl 0x0200ac24
	adds r0, #35
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r0, #0
	bl 0x0200ac24
	movs r1, #1
	bl 0x0200abec
	movs r5, #192
	movs r2, #0
	movs r0, #0
	movs r1, #4
	bl 0x0200ac94
	lsls r5, r5, #7
	movs r0, #0
	movs r1, #1
	bl 0x0200ac84
	movs r0, #0
	adds r1, r5, #0
	movs r2, #60
	bl 0x0200a780
	movs r0, #0
	ldr r1, [pc, #376]
	movs r2, #0
	bl 0x0200acdc
	ldr r1, [pc, #368]
	movs r2, #0
	movs r0, #2
	bl 0x0200acdc
	movs r0, #60
	bl 0x0200ac04
	movs r1, #160
	movs r2, #20
	lsls r1, r1, #8
	movs r0, #0
	bl 0x0200a780
	movs r0, #1
	movs r1, #3
	bl 0x0200ac8c
	movs r0, #0
	movs r1, #3
	bl 0x0200ac8c
	movs r0, #0
	adds r1, r5, #0
	movs r2, #10
	bl 0x0200a780
	movs r0, #1
	adds r1, r6, #0
	movs r2, #10
	bl 0x0200a780
	movs r1, #192
	movs r2, #0
	movs r0, #2
	lsls r1, r1, #8
	bl 0x0200accc
	movs r0, #2
	movs r1, #3
	bl 0x0200ac8c
	movs r0, #2
	movs r1, #20
	bl 0x0200a768
	movs r0, #0
	movs r1, #3
	bl 0x0200ac84
	movs r0, #3
	movs r1, #3
	bl 0x0200ac84
	movs r1, #3
	movs r0, #1
	bl 0x0200ac8c
	movs r0, #20
	bl 0x0200ac04
	movs r1, #1
	movs r0, #2
	bl 0x0200aca4
	movs r0, #20
	bl 0x0200ac04
	movs r1, #0
	movs r0, #2
	bl 0x0200acbc
	movs r0, #2
	movs r1, #3
	bl 0x0200ac84
	movs r1, #224
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200accc
	movs r0, #1
	adds r1, r7, #0
	movs r2, #0
	bl 0x0200accc
	movs r0, #0
	movs r1, #0
	bl 0x0200ac1c
	cmp r0, #0
	bne .L_02001588_5
	movs r0, #2
	movs r1, #3
	bl 0x0200ac84
	movs r0, #1
	movs r1, #3
	bl 0x0200ac8c
	ldr r3, [pc, #160]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02001588_6
.L_02001588_5:
	movs r0, #1
	movs r1, #2
	bl 0x0200aca4
	movs r0, #1
	adds r1, r7, #0
	movs r2, #10
	bl 0x0200a780
	movs r0, #1
	movs r1, #2
	bl 0x0200ac9c
	movs r0, #1
	movs r1, #0
	bl 0x0200acc4
.L_02001588_6:
	movs r1, #128
	movs r2, #10
	lsls r1, r1, #7
	movs r0, #1
	bl 0x0200a780
	movs r0, #1
	movs r1, #4
	bl 0x0200ac8c
	movs r0, #1
	movs r1, #20
	bl 0x0200a768
	movs r1, #192
	movs r2, #10
	lsls r1, r1, #8
	movs r0, #2
	bl 0x0200a780
	movs r0, #2
	movs r1, #3
	bl 0x0200ac8c
	movs r0, #2
	movs r1, #10
	bl 0x0200a768
	ldr r3, [pc, #60]
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02001588_7
	movs r0, #3
	movs r1, #2
	bl 0x0200aca4
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200a780
	movs r1, #128
	lsls r1, r1, #6
	movs r0, #3
	movs r2, #10
	bl 0x0200a780
	movs r0, #3
	movs r1, #4
	bl 0x0200ac84
	movs r0, #3
	movs r1, #10
	bl 0x0200a768
	b .L_02001588_8
	.4byte 0x03001ebc
	.4byte 0x00000105
	.4byte 0x0200b394
.L_02001588_7:
	ldr r3, [pc, #372]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02001588_8:
	movs r1, #128
	movs r6, #160
	lsls r6, r6, #8
	movs r0, #1
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200accc
	movs r2, #10
	movs r0, #0
	adds r1, r6, #0
	bl 0x0200a780
	movs r0, #0
	movs r1, #3
	bl 0x0200ac84
	movs r1, #3
	movs r0, #1
	bl 0x0200ac8c
	movs r0, #20
	bl 0x0200ac04
	movs r5, #128
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	lsls r5, r5, #7
	bl 0x0200accc
	movs r2, #10
	movs r0, #1
	adds r1, r5, #0
	bl 0x0200a780
	movs r1, #4
	movs r0, #2
	bl 0x0200ac8c
	movs r0, #20
	bl 0x0200ac04
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200acdc
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #1
	bl 0x0200acdc
	movs r0, #80
	bl 0x0200ac04
	movs r1, #224
	movs r2, #10
	lsls r1, r1, #8
	movs r0, #2
	bl 0x0200a780
	movs r0, #2
	movs r1, #2
	bl 0x0200ac9c
	movs r0, #2
	movs r1, #20
	bl 0x0200a768
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200accc
	movs r0, #0
	adds r1, r6, #0
	movs r2, #40
	bl 0x0200a780
	movs r0, #1
	adds r1, r5, #0
	movs r2, #0
	bl 0x0200accc
	movs r1, #192
	lsls r1, r1, #7
	movs r0, #0
	movs r2, #10
	bl 0x0200a780
	movs r1, #192
	movs r2, #10
	lsls r1, r1, #8
	movs r0, #2
	bl 0x0200a780
	movs r0, #2
	movs r1, #3
	bl 0x0200ac8c
	movs r0, #2
	movs r1, #10
	bl 0x0200a768
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200ace4
	movs r0, #40
	bl 0x0200ac04
	movs r0, #1
	movs r1, #20
	bl 0x0200a768
	movs r1, #3
	movs r0, #2
	bl 0x0200ac8c
	movs r0, #20
	bl 0x0200ac04
	movs r1, #129
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200acdc
	movs r0, #40
	bl 0x0200ac04
	movs r0, #1
	movs r1, #20
	bl 0x0200a768
	movs r0, #2
	movs r1, #3
	bl 0x0200ac84
	movs r0, #2
	movs r1, #10
	bl 0x0200a768
	movs r0, #1
	movs r1, #2
	bl 0x0200aca4
	movs r1, #128
	movs r2, #10
	lsls r1, r1, #6
	movs r0, #1
	bl 0x0200a780
	movs r1, #0
	movs r0, #1
	bl 0x0200acbc
	movs r0, #0
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200accc
	movs r0, #0
	movs r1, #0
	bl 0x0200ac1c
	cmp r0, #0
	bne .L_02001588_9
	movs r0, #1
	movs r1, #3
	bl 0x0200ac8c
	b .L_02001588_10
	.2byte 0x0000
	.4byte 0x03001ebc
.L_02001588_9:
	movs r0, #20
	bl 0x0200ac04
	movs r0, #1
	movs r1, #2
	bl 0x0200aca4
	movs r0, #40
	bl 0x0200ac04
	ldr r3, [pc, #1004]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02001588_10:
	movs r1, #0
	movs r0, #1
	bl 0x0200acc4
	movs r0, #21
	bl 0x0200ad24
	movs r1, #1
	ldr r0, [pc, #976]
	bl 0x0200acf4
	movs r0, #60
	bl 0x0200acfc
	movs r0, #60
	bl 0x0200ab84
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
	bl 0x0200ab8c
	movs r0, #80
	bl 0x0200ac04
	movs r0, #0
	movs r1, #2
	bl 0x0200ac9c
	movs r0, #1
	movs r1, #2
	bl 0x0200ac9c
	movs r0, #3
	movs r1, #2
	bl 0x0200ac9c
	movs r5, #192
	movs r1, #2
	movs r0, #2
	bl 0x0200aca4
	lsls r5, r5, #8
	movs r0, #60
	bl 0x0200ac04
	movs r2, #10
	adds r1, r5, #0
	movs r0, #2
	bl 0x0200a780
	ldr r0, [pc, #892]
	bl 0x0200acb4
	movs r0, #2
	movs r1, #10
	bl 0x0200a768
	movs r0, #1
	adds r1, r5, #0
	movs r2, #10
	bl 0x0200a780
	movs r0, #0
	adds r1, r5, #0
	movs r2, #10
	bl 0x0200a780
	ldr r7, [pc, #860]
	ldr r3, [r7]
	cmp r3, #0
	beq .L_02001588_11
	movs r0, #3
	adds r1, r5, #0
	movs r2, #10
	bl 0x0200a780
.L_02001588_11:
	movs r0, #0
	bl 0x0200ac24
	adds r0, #35
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #1
	bl 0x0200ac24
	adds r0, #35
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #2
	bl 0x0200ac24
	adds r0, #35
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #3
	bl 0x0200ac24
	adds r0, #35
	ldrb r3, [r0]
	ands r5, r3
	strb r5, [r0]
	movs r1, #3
	movs r0, #0
	bl 0x0200acd4
	movs r0, #1
	movs r1, #3
	bl 0x0200acd4
	movs r0, #2
	movs r1, #3
	bl 0x0200acd4
	movs r1, #3
	movs r0, #3
	bl 0x0200acd4
	movs r3, #2
	str r3, [r6]
	movs r0, #220
	bl 0x0200ad24
	movs r1, #253
	ldr r2, [pc, #740]
	movs r0, #13
	lsls r1, r1, #16
	bl 0x0200ac7c
	ldr r5, [pc, #732]
	movs r0, #13
	adds r1, r5, #0
	bl 0x0200ac34
	movs r1, #233
	movs r0, #14
	lsls r1, r1, #16
	ldr r2, [pc, #720]
	bl 0x0200ac7c
	movs r0, #14
	adds r1, r5, #0
	bl 0x0200ac34
	ldr r3, [r7]
	cmp r3, #0
	beq .L_02001588_12
	movs r1, #207
	movs r0, #15
	lsls r1, r1, #16
	ldr r2, [pc, #700]
	bl 0x0200ac7c
	movs r0, #15
	adds r1, r5, #0
	bl 0x0200ac34
.L_02001588_12:
	movs r1, #227
	movs r2, #145
	lsls r2, r2, #18
	movs r0, #16
	lsls r1, r1, #16
	bl 0x0200ac7c
	adds r1, r5, #0
	movs r0, #16
	bl 0x0200ac34
	movs r0, #120
	bl 0x0200ac04
	movs r3, #3
	str r3, [r6]
	adds r5, r6, #0
.L_02001588_13:
	movs r0, #1
	bl 0x0200ab84
	ldr r3, [r5]
	cmp r3, #0
	bne .L_02001588_13
	movs r0, #11
	movs r1, #80
	bl 0x0200a768
	movs r0, #12
	movs r1, #20
	bl 0x0200a768
	movs r0, #0
	ldr r1, [pc, #624]
	movs r2, #0
	bl 0x0200acdc
	movs r0, #1
	ldr r1, [pc, #616]
	movs r2, #0
	bl 0x0200acdc
	movs r0, #2
	ldr r1, [pc, #604]
	movs r2, #0
	bl 0x0200acdc
	movs r2, #0
	ldr r1, [pc, #596]
	movs r0, #3
	bl 0x0200acdc
	movs r0, #60
	bl 0x0200ac04
	movs r0, #12
	movs r1, #20
	bl 0x0200a768
	movs r0, #0
	movs r1, #3
	bl 0x0200ac84
	movs r0, #1
	movs r1, #3
	bl 0x0200ac84
	movs r0, #3
	movs r1, #3
	bl 0x0200ac84
	movs r0, #2
	movs r1, #3
	bl 0x0200ac8c
	movs r0, #12
	movs r1, #10
	bl 0x0200a768
	movs r0, #0
	movs r1, #3
	bl 0x0200ac84
	movs r0, #1
	movs r1, #3
	bl 0x0200ac84
	movs r0, #3
	movs r1, #3
	bl 0x0200ac84
	movs r0, #2
	movs r1, #3
	bl 0x0200ac8c
	movs r0, #12
	movs r1, #10
	bl 0x0200a768
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200acdc
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200acdc
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200acdc
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #3
	bl 0x0200acdc
	movs r0, #40
	bl 0x0200ac04
	movs r0, #11
	movs r1, #10
	bl 0x0200a768
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200accc
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	movs r6, #192
	lsls r6, r6, #8
	bl 0x0200accc
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl 0x0200accc
	movs r2, #40
	movs r0, #2
	adds r1, r6, #0
	bl 0x0200a780
	movs r0, #12
	movs r1, #10
	bl 0x0200a768
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200acdc
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200acdc
	movs r1, #129
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200acdc
	movs r1, #129
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200acdc
	movs r0, #0
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200accc
	movs r0, #1
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200accc
	movs r0, #2
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200accc
	movs r2, #80
	movs r0, #3
	adds r1, r6, #0
	bl 0x0200a780
	movs r0, #12
	movs r1, #10
	bl 0x0200a768
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200accc
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200accc
	movs r0, #2
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200accc
	movs r2, #40
	movs r0, #3
	movs r1, #0
	bl 0x0200a780
	movs r0, #11
	movs r1, #10
	bl 0x0200a768
	movs r0, #0
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200accc
	movs r0, #1
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200accc
	movs r0, #2
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200accc
	movs r2, #10
	movs r0, #3
	adds r1, r6, #0
	bl 0x0200a780
	movs r0, #0
	movs r1, #4
	bl 0x0200ac84
	movs r0, #1
	movs r1, #4
	bl 0x0200ac84
	movs r0, #3
	movs r1, #4
	bl 0x0200ac84
	movs r1, #4
	movs r0, #2
	bl 0x0200ac8c
	movs r0, #60
	bl 0x0200ac04
	movs r0, #12
	movs r1, #10
	bl 0x0200a768
	movs r0, #0
	movs r1, #3
	bl 0x0200ac84
	movs r0, #1
	movs r1, #3
	bl 0x0200ac84
	movs r0, #3
	movs r1, #3
	bl 0x0200ac84
	movs r0, #2
	movs r1, #3
	bl 0x0200ac8c
	movs r0, #12
	movs r1, #20
	bl 0x0200a768
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200accc
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200accc
	movs r0, #2
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200accc
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x0200a780
	movs r0, #12
	movs r1, #10
	bl 0x0200a768
	b .L_02001588_14
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00406218
	.4byte 0x0200b388
	.4byte 0x0200b384
	.4byte 0x0200b38c
	.4byte 0x0200a975
	.4byte 0x0000149d
	.4byte 0x0200b394
	.4byte 0x025b0000
	.4byte 0x0200af6c
	.4byte 0x02750000
	.4byte 0x02610000
	.4byte 0x00000101
.L_02001588_14:
	movs r0, #0
	movs r1, #2
	bl 0x0200ac9c
	movs r0, #1
	movs r1, #2
	bl 0x0200ac9c
	movs r0, #3
	movs r1, #2
	bl 0x0200ac9c
	movs r0, #2
	movs r1, #2
	bl 0x0200aca4
	movs r0, #0
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200accc
	movs r0, #1
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200accc
	movs r0, #2
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200accc
	movs r2, #0
	movs r0, #3
	adds r1, r6, #0
	bl 0x0200accc
	movs r0, #12
	movs r1, #20
	bl 0x0200a768
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200accc
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200accc
	movs r0, #2
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200accc
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x0200a780
	movs r0, #11
	movs r1, #20
	bl 0x0200a768
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200acdc
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200acdc
	movs r1, #129
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200acdc
	movs r1, #129
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #2
	bl 0x0200acdc
	movs r0, #40
	bl 0x0200ac04
	movs r0, #12
	movs r1, #10
	bl 0x0200a768
	movs r0, #0
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200accc
	movs r0, #1
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200accc
	movs r0, #2
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200accc
	movs r2, #10
	movs r0, #3
	adds r1, r6, #0
	bl 0x0200a780
	movs r0, #12
	movs r1, #10
	bl 0x0200a768
	movs r0, #0
	movs r1, #3
	bl 0x0200ac84
	movs r0, #1
	movs r1, #3
	bl 0x0200ac84
	movs r0, #3
	movs r1, #3
	bl 0x0200ac84
	movs r1, #3
	movs r0, #2
	bl 0x0200ac8c
	movs r0, #60
	bl 0x0200ac04
	movs r0, #12
	movs r1, #0
	bl 0x0200acc4
	movs r1, #0
	movs r0, #11
	bl 0x0200acc4
	ldr r0, [pc, #976]
	bl 0x0200ab94
	movs r0, #80
	bl 0x0200ac04
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl 0x0200acf4
	movs r0, #60
	bl 0x0200acfc
	movs r0, #80
	bl 0x0200ab84
	movs r0, #13
	bl 0x0200ac44
	movs r0, #14
	bl 0x0200ac44
	movs r0, #15
	ldr r7, [pc, #932]
	bl 0x0200ac44
	movs r0, #16
	bl 0x0200ac44
	movs r0, #1
	bl 0x0200ab84
	ldr r5, [pc, #920]
	movs r0, #13
	adds r1, r5, #0
	bl 0x0200ac34
	movs r0, #14
	adds r1, r5, #0
	bl 0x0200ac34
	ldr r3, [r7]
	cmp r3, #0
	beq .L_02001588_15
	movs r0, #15
	adds r1, r5, #0
	bl 0x0200ac34
.L_02001588_15:
	adds r1, r5, #0
	movs r0, #16
	bl 0x0200ac4c
	movs r0, #20
	bl 0x0200ac04
	movs r0, #0
	movs r1, #2
	bl 0x0200acd4
	movs r0, #1
	movs r1, #2
	bl 0x0200acd4
	movs r0, #2
	movs r1, #2
	bl 0x0200acd4
	movs r1, #2
	movs r0, #3
	bl 0x0200acd4
	movs r0, #0
	bl 0x0200ac24
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #1
	bl 0x0200ac24
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #2
	bl 0x0200ac24
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #3
	bl 0x0200ac24
	adds r0, #35
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r5, #224
	movs r0, #2
	movs r1, #2
	lsls r5, r5, #8
	bl 0x0200aca4
	movs r2, #10
	movs r0, #2
	adds r1, r5, #0
	bl 0x0200a780
	movs r1, #0
	movs r0, #2
	bl 0x0200acbc
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200accc
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl 0x0200accc
	movs r0, #0
	movs r1, #0
	bl 0x0200ac1c
	cmp r0, #0
	beq .L_02001588_16
	b .L_02001588_17
.L_02001588_16:
	movs r1, #2
	movs r0, #1
	bl 0x0200aca4
	movs r0, #10
	bl 0x0200ac04
	movs r1, #0
	movs r0, #1
	bl 0x0200acbc
	movs r0, #0
	movs r1, #0
	bl 0x0200ac1c
	cmp r0, #0
	bne .L_02001588_18
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200a780
	movs r0, #1
	ldr r1, [pc, #676]
	movs r2, #0
	bl 0x0200acdc
	movs r0, #2
	ldr r1, [pc, #664]
	movs r2, #0
	bl 0x0200acdc
	ldr r1, [pc, #656]
	movs r2, #0
	movs r0, #3
	bl 0x0200acdc
	movs r0, #40
	bl 0x0200ac04
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #7
	movs r0, #1
	bl 0x0200a780
	movs r0, #1
	movs r1, #10
	bl 0x0200a768
	movs r0, #2
	adds r1, r6, #0
	movs r2, #20
	bl 0x0200a780
	movs r2, #20
	movs r0, #2
	adds r1, r5, #0
	bl 0x0200a780
	movs r0, #2
	movs r1, #3
	bl 0x0200ac8c
	movs r0, #2
	movs r1, #20
	bl 0x0200a768
	movs r1, #128
	lsls r1, r1, #6
	movs r0, #1
	movs r2, #20
	bl 0x0200a780
	b .L_02001588_19
.L_02001588_18:
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200a780
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200acdc
	movs r1, #129
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200acdc
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #3
	bl 0x0200acdc
	movs r0, #40
	bl 0x0200ac04
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #7
	movs r0, #1
	bl 0x0200a780
	ldr r0, [pc, #512]
	bl 0x0200acb4
	movs r0, #1
	movs r1, #20
	bl 0x0200a768
	movs r0, #2
	movs r1, #3
	bl 0x0200ac8c
	movs r0, #2
	movs r1, #20
	bl 0x0200a768
.L_02001588_19:
	movs r0, #3
	movs r1, #3
	bl 0x0200ac84
	movs r0, #1
	movs r1, #3
	bl 0x0200ac8c
	b .L_02001588_20
.L_02001588_17:
	movs r0, #20
	bl 0x0200ac04
	movs r1, #3
	movs r0, #1
	bl 0x0200ac8c
	movs r0, #10
	bl 0x0200ac04
	ldr r0, [pc, #448]
	bl 0x0200acb4
	movs r0, #1
	movs r1, #10
	bl 0x0200a768
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200accc
	movs r1, #192
	movs r2, #20
	lsls r1, r1, #7
	movs r0, #0
	bl 0x0200a780
	movs r0, #1
	movs r1, #3
	bl 0x0200ac84
	movs r1, #3
	movs r0, #0
	bl 0x0200ac8c
	movs r0, #10
	bl 0x0200ac04
	movs r0, #2
	movs r1, #4
	bl 0x0200ac8c
	movs r1, #0
	movs r0, #2
	bl 0x0200acbc
	movs r0, #0
	movs r1, #0
	bl 0x0200ac1c
	cmp r0, #0
	beq .L_02001588_21
	b .L_02001588_22
.L_02001588_21:
	movs r0, #20
	bl 0x0200ac04
	ldr r1, [pc, #356]
	movs r2, #0
	movs r0, #2
	bl 0x0200acdc
	movs r0, #40
	bl 0x0200ac04
	movs r0, #2
	adds r1, r5, #0
	movs r2, #10
	bl 0x0200a780
	movs r0, #2
	movs r1, #10
	bl 0x0200a768
	ldr r3, [r7]
	cmp r3, #0
	beq .L_02001588_23
	movs r0, #3
	movs r1, #0
	movs r2, #10
	bl 0x0200a780
	movs r0, #3
	movs r1, #3
	bl 0x0200ac9c
	movs r0, #3
	movs r1, #20
	bl 0x0200a768
	b .L_02001588_24
.L_02001588_23:
	ldr r3, [pc, #292]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02001588_24:
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200acdc
	movs r1, #129
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200acdc
	movs r0, #40
	bl 0x0200ac04
	movs r0, #1
	movs r1, #2
	bl 0x0200aca4
	movs r0, #1
	movs r1, #20
	bl 0x0200a768
	ldr r1, [pc, #232]
	movs r2, #0
	movs r0, #1
	bl 0x0200acdc
	movs r0, #120
	bl 0x0200ac04
	movs r0, #2
	movs r1, #40
	bl 0x0200a768
	ldr r3, [pc, #180]
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02001588_25
	movs r1, #128
	lsls r1, r1, #6
	movs r0, #3
	movs r2, #10
	bl 0x0200a780
	movs r0, #3
	movs r1, #4
	bl 0x0200ac8c
	movs r0, #3
	movs r1, #10
	bl 0x0200a768
	b .L_02001588_26
.L_02001588_25:
	ldr r3, [pc, #168]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02001588_26:
	movs r0, #60
	bl 0x0200ac04
	movs r0, #2
	movs r1, #2
	bl 0x0200aca4
	ldr r3, [pc, #112]
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02001588_27
	movs r1, #160
	lsls r1, r1, #8
	movs r0, #2
	movs r2, #40
	bl 0x0200a780
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #2
	movs r2, #20
	bl 0x0200a780
.L_02001588_27:
	movs r0, #2
	movs r1, #10
	bl 0x0200a768
	movs r0, #0
	movs r1, #2
	bl 0x0200ac9c
	movs r1, #2
	movs r0, #1
	bl 0x0200aca4
	movs r0, #40
	bl 0x0200ac04
	movs r0, #2
	movs r1, #20
	bl 0x0200a768
	movs r0, #0
	movs r1, #3
	bl 0x0200ac8c
	movs r1, #3
	movs r0, #1
	bl 0x0200ac8c
	movs r0, #20
	bl 0x0200ac04
	movs r0, #3
	movs r1, #3
	bl 0x0200ac84
.L_02001588_20:
	movs r0, #2
	movs r1, #3
	bl 0x0200ac8c
	b .L_02001588_28
	.2byte 0x0000
	.4byte 0x0200a975
	.4byte 0x0200b394
	.4byte 0x0200afc8
	.4byte 0x00000101
	.4byte 0x000014b4
	.4byte 0x000014b6
	.4byte 0x00000103
	.4byte 0x03001ebc
	.4byte 0x00000105
.L_02001588_22:
	movs r2, #0
	ldr r1, [pc, #496]
	movs r0, #2
	bl 0x0200acdc
	movs r0, #40
	bl 0x0200ac04
	movs r1, #3
	movs r0, #2
	bl 0x0200ac8c
	ldr r0, [pc, #476]
	bl 0x0200acb4
	movs r0, #2
	movs r1, #20
	bl 0x0200a768
	ldr r3, [r7]
	cmp r3, #0
	beq .L_02001588_29
	movs r0, #3
	movs r1, #0
	movs r2, #10
	bl 0x0200a780
	movs r0, #3
	movs r1, #1
	bl 0x0200ac9c
	movs r0, #3
	movs r1, #20
	bl 0x0200a768
	b .L_02001588_30
.L_02001588_29:
	ldr r3, [pc, #432]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02001588_30:
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200acdc
	movs r1, #129
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #0
	bl 0x0200acdc
	movs r0, #40
	bl 0x0200ac04
	movs r0, #1
	movs r1, #2
	bl 0x0200aca4
	movs r0, #1
	movs r1, #20
	bl 0x0200a768
	ldr r1, [pc, #364]
	movs r2, #0
	movs r0, #2
	bl 0x0200acdc
	movs r0, #80
	bl 0x0200ac04
	movs r0, #2
	movs r1, #40
	bl 0x0200a768
	ldr r3, [pc, #352]
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02001588_31
	movs r1, #128
	lsls r1, r1, #6
	movs r0, #3
	movs r2, #20
	bl 0x0200a780
	movs r0, #3
	movs r1, #4
	bl 0x0200ac84
	movs r0, #3
	movs r1, #40
	bl 0x0200a768
	b .L_02001588_32
.L_02001588_31:
	ldr r3, [pc, #308]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02001588_32:
	movs r1, #2
	movs r0, #2
	bl 0x0200aca4
	movs r0, #20
	bl 0x0200ac04
	movs r0, #2
	movs r1, #20
	bl 0x0200a768
	movs r0, #1
	movs r1, #2
	bl 0x0200ac9c
	movs r1, #2
	movs r0, #0
	bl 0x0200aca4
	movs r0, #40
	bl 0x0200ac04
	movs r0, #2
	movs r1, #20
	bl 0x0200a768
.L_02001588_28:
	movs r0, #17
	bl 0x0200ad24
	movs r0, #1
	ldr r1, [pc, #240]
	ldr r2, [pc, #244]
	bl 0x0200ac2c
	movs r0, #2
	ldr r1, [pc, #232]
	ldr r2, [pc, #232]
	bl 0x0200ac2c
	movs r0, #3
	ldr r1, [pc, #220]
	ldr r2, [pc, #224]
	bl 0x0200ac2c
	movs r0, #1
	movs r1, #2
	bl 0x0200ac84
	movs r0, #0
	bl 0x0200ac24
	cmp r0, #0
	beq .L_02001588_33
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x0200ac54
.L_02001588_33:
	movs r0, #1
	bl 0x0200ac74
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200ac7c
	movs r0, #2
	movs r1, #2
	bl 0x0200ac84
	movs r0, #0
	bl 0x0200ac24
	cmp r0, #0
	beq .L_02001588_34
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #2
	bl 0x0200ac54
.L_02001588_34:
	movs r0, #2
	bl 0x0200ac74
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x0200ac7c
	ldr r3, [pc, #112]
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02001588_35
	movs r0, #3
	movs r1, #2
	bl 0x0200ac84
	movs r0, #0
	bl 0x0200ac24
	cmp r0, #0
	beq .L_02001588_36
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #3
	bl 0x0200ac54
.L_02001588_36:
	movs r0, #3
	bl 0x0200ac74
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl 0x0200ac7c
.L_02001588_35:
	ldr r0, [pc, #68]
	bl 0x0200abfc
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl 0x0200ac7c
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl 0x0200ac7c
	bl 0x0200ad14
	bl 0x0200ac14
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
	.4byte 0x0200b394
	.4byte 0x00013333
	.4byte 0x00009999
	.4byte 0x00000843
	.global Func_02002768
	.thumb_func
Func_02002768:
	push {r5, lr}
	adds r5, r1, #0
	movs r1, #0
	bl 0x0200acc4
	adds r0, r5, #0
	bl 0x0200ac04
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02002780
	.thumb_func
Func_02002780:
	push {r5, lr}
	adds r5, r2, #0
	movs r2, #0
	bl 0x0200accc
	adds r0, r5, #0
	bl 0x0200ac04
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02002798
	.thumb_func
Func_02002798:
	push {lr}
	ldr r3, [r0, #24]
	ldr r2, [pc, #36]
	adds r3, r3, r2
	str r3, [r0, #24]
	movs r2, #128
	ldr r3, [r0, #56]
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_02002798_0
	ldr r2, [r0, #60]
	cmp r2, r3
	bne .L_02002798_0
	ldr r3, [r0, #64]
	cmp r3, r2
	bne .L_02002798_0
	bl 0x0200abbc
.L_02002798_0:
	movs r0, #1
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x00001eb8
	.global Func_020027c8
	.thumb_func
Func_020027c8:
	push {r5, r6, lr}
	ldr r3, [pc, #136]
	ldr r6, [r3]
	movs r3, #7
	ands r6, r3
	cmp r6, #0
	bne .L_020027c8_0
	ldr r3, [pc, #128]
	ldr r3, [r3]
	cmp r3, #0
	beq .L_020027c8_1
	movs r0, #200
	bl 0x0200ad24
.L_020027c8_1:
	movs r1, #231
	movs r3, #230
	movs r0, #26
	lsls r1, r1, #16
	movs r2, #0
	lsls r3, r3, #17
	bl 0x0200abb4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020027c8_0
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
	ldr r3, [pc, #64]
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
	bl 0x0200aba4
	movs r1, #231
	movs r3, #156
	adds r0, r5, #0
	lsls r1, r1, #16
	movs r2, #0
	lsls r3, r3, #18
	bl 0x0200abcc
	ldr r1, [pc, #24]
	adds r0, r5, #0
	bl 0x0200abac
.L_020027c8_0:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001e40
	.4byte 0x0200b398
	.4byte 0x00001999
	.4byte 0x0200b2d0
	.global Func_02002864
	.thumb_func
Func_02002864:
	push {lr}
	ldr r3, [pc, #32]
	ldr r3, [r3]
	movs r2, #1
	lsrs r3, r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02002864_0
	movs r1, #10
	bl 0x0200acac
	b .L_02002864_1
.L_02002864_0:
	movs r1, #7
	bl 0x0200acac
.L_02002864_1:
	movs r0, #0
	pop {r1}
	bx r1
	.4byte 0x03001e40
	.global Func_0200288c
	.thumb_func
Func_0200288c:
	push {r5, lr}
	ldr r3, [pc, #160]
	ldr r3, [r3]
	adds r5, r0, #0
	cmp r3, #0
	beq .L_0200288c_0
	ldr r1, [pc, #152]
	ldr r2, [r5, #8]
	adds r3, r2, r1
	ldr r1, [pc, #152]
	cmp r3, r1
	bhi .L_0200288c_1
	ldr r3, [r5, #16]
	ldr r1, [pc, #148]
	cmp r3, r1
	ble .L_0200288c_1
	movs r1, #153
	lsls r1, r1, #18
	cmp r3, r1
	blt .L_0200288c_2
.L_0200288c_1:
	ldr r1, [pc, #136]
	adds r3, r2, r1
	ldr r2, [pc, #136]
	cmp r3, r2
	bhi .L_0200288c_3
	ldr r3, [r5, #16]
	ldr r1, [pc, #132]
	b .L_0200288c_4
.L_0200288c_0:
	ldr r1, [pc, #108]
	ldr r2, [r5, #8]
	adds r3, r2, r1
	ldr r1, [pc, #128]
	cmp r3, r1
	bhi .L_0200288c_5
	ldr r3, [r5, #16]
	ldr r1, [pc, #116]
	cmp r3, r1
	ble .L_0200288c_5
	ldr r1, [pc, #116]
	cmp r3, r1
	ble .L_0200288c_2
.L_0200288c_5:
	ldr r1, [pc, #116]
	adds r3, r2, r1
	ldr r1, [pc, #116]
	cmp r3, r1
	bhi .L_0200288c_6
	ldr r3, [r5, #16]
	ldr r1, [pc, #112]
	cmp r3, r1
	ble .L_0200288c_6
	ldr r1, [pc, #108]
	cmp r3, r1
	ble .L_0200288c_2
.L_0200288c_6:
	ldr r1, [pc, #108]
	adds r3, r2, r1
	ldr r2, [pc, #108]
	cmp r3, r2
	bhi .L_0200288c_3
	movs r1, #149
	ldr r3, [r5, #16]
	lsls r1, r1, #18
.L_0200288c_4:
	cmp r3, r1
	ble .L_0200288c_3
	movs r2, #158
	lsls r2, r2, #18
	cmp r3, r2
	bge .L_0200288c_3
.L_0200288c_2:
	movs r0, #106
	bl 0x0200ad24
	ldr r1, [pc, #80]
	adds r0, r5, #0
	bl 0x0200abac
	ldr r2, [pc, #76]
	movs r3, #1
	str r3, [r2]
.L_0200288c_3:
	movs r0, #0
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x0200b394
	.4byte 0xff3fffff
	.4byte 0x0051fffe
	.4byte 0x02360000
	.4byte 0xff35ffff
	.4byte 0x0034fffe
	.4byte 0x02250000
	.4byte 0x0033fffe
	.4byte 0x0248ffff
	.4byte 0xff0bffff
	.4byte 0x001dfffe
	.4byte 0x023b0000
	.4byte 0x025cffff
	.4byte 0xff2cffff
	.4byte 0x002bfffe
	.4byte 0x0200b2e4
	.4byte 0x0200b390
	.global Func_02002974
	.thumb_func
Func_02002974:
	push {r5, r6, r7, lr}
	ldr r2, [pc, #304]
	ldr r3, [r2]
	movs r5, #0
	cmp r3, #2
	beq .L_02002974_0
	cmp r3, #2
	bhi .L_02002974_1
	cmp r3, #1
	beq .L_02002974_2
	b .L_02002974_3
.L_02002974_1:
	cmp r3, #3
	beq .L_02002974_4
	b .L_02002974_3
.L_02002974_2:
	ldr r2, [pc, #280]
	ldr r1, [pc, #284]
	ldr r3, [r2]
	cmp r3, r1
	bgt .L_02002974_5
	adds r3, #50
	str r3, [r2]
.L_02002974_5:
	ldr r2, [pc, #276]
	movs r1, #240
	ldr r3, [r2]
	lsls r1, r1, #14
	b .L_02002974_6
.L_02002974_0:
	ldr r2, [pc, #256]
	ldr r1, [pc, #268]
	ldr r3, [r2]
	cmp r3, r1
	bgt .L_02002974_7
	adds r3, #50
	str r3, [r2]
.L_02002974_7:
	ldr r2, [pc, #252]
	movs r1, #192
	ldr r3, [r2]
	lsls r1, r1, #13
.L_02002974_6:
	cmp r3, r1
	ble .L_02002974_3
	ldr r1, [pc, #248]
	adds r3, r3, r1
	str r3, [r2]
	b .L_02002974_3
.L_02002974_4:
	ldr r0, [pc, #232]
	ldr r3, [pc, #240]
	ldr r1, [r0]
	cmp r1, r3
	bge .L_02002974_8
	str r5, [r2]
	b .L_02002974_3
.L_02002974_8:
	ldr r3, [pc, #208]
	ldr r2, [r3]
	adds r2, #50
	str r2, [r3]
	ldr r2, [pc, #216]
	adds r3, r1, r2
	str r3, [r0]
.L_02002974_3:
	ldr r7, [pc, #220]
	ldr r3, [r7]
	movs r2, #7
	ands r3, r2
	cmp r3, #0
	bne .L_02002974_9
	ldr r0, [pc, #212]
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl 0x0200abb4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02002974_9
	ldr r3, [pc, #196]
	ldr r3, [r3]
	ldr r6, [r3]
	ldr r3, [r7]
	movs r2, #63
	ands r3, r2
	cmp r3, #0
	bne .L_02002974_10
	movs r0, #246
	bl 0x0200ad24
.L_02002974_10:
	ldr r3, [pc, #140]
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02002974_11
	bl 0x0200ab9c
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
	b .L_02002974_12
.L_02002974_11:
	bl 0x0200ab9c
	ldr r3, [r6]
	lsls r0, r0, #8
	ldr r1, [pc, #120]
	adds r3, r3, r0
	adds r7, r3, r1
.L_02002974_12:
	bl 0x0200ab9c
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
	bl 0x0200aba4
	ldr r1, [pc, #56]
	adds r0, r5, #0
	bl 0x0200abac
.L_02002974_9:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0200b38c
	.4byte 0x0200b388
	.4byte 0x00003a97
	.4byte 0x0200b384
	.4byte 0x0000752f
	.4byte 0xffffc000
	.4byte 0xff800000
	.4byte 0x03001e40
	.4byte 0x0000011d
	.4byte 0x03001e70
	.4byte 0x0000e666
	.4byte 0x0200b308
	.global Func_02002ad8
	.thumb_func
Func_02002ad8:
	push {lr}
	movs r0, #13
	bl 0x0200ac24
	cmp r0, #0
	beq .L_02002ad8_0
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	ldr r3, [pc, #144]
	ldr r2, [r3]
	movs r3, #1
	ands r2, r3
	cmp r2, #0
	bne .L_02002ad8_1
	str r2, [r0, #12]
	b .L_02002ad8_0
.L_02002ad8_1:
	movs r3, #250
	lsls r3, r3, #17
	str r3, [r0, #12]
.L_02002ad8_0:
	movs r0, #14
	bl 0x0200ac24
	cmp r0, #0
	beq .L_02002ad8_2
	adds r3, r0, #0
	adds r3, #85
	movs r1, #0
	strb r1, [r3]
	ldr r3, [pc, #104]
	ldr r3, [r3]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02002ad8_3
	str r1, [r0, #12]
	b .L_02002ad8_2
.L_02002ad8_3:
	movs r3, #250
	lsls r3, r3, #17
	str r3, [r0, #12]
.L_02002ad8_2:
	movs r0, #15
	bl 0x0200ac24
	cmp r0, #0
	beq .L_02002ad8_4
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	ldr r3, [pc, #64]
	ldr r2, [r3]
	movs r3, #1
	ands r2, r3
	cmp r2, #0
	bne .L_02002ad8_5
	str r2, [r0, #12]
	b .L_02002ad8_4
.L_02002ad8_5:
	movs r3, #250
	lsls r3, r3, #17
	str r3, [r0, #12]
.L_02002ad8_4:
	movs r0, #16
	bl 0x0200ac24
	cmp r0, #0
	beq .L_02002ad8_6
	adds r3, r0, #0
	adds r3, #85
	movs r1, #0
	strb r1, [r3]
	ldr r3, [pc, #24]
	ldr r3, [r3]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02002ad8_7
	str r1, [r0, #12]
	b .L_02002ad8_6
.L_02002ad8_7:
	movs r3, #250
	lsls r3, r3, #17
	str r3, [r0, #12]
.L_02002ad8_6:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001e40
@ The compiler library links here from its licensed container.
	.section .rodata.part1,"a",%progbits
	.global StagedActor_DirectionSteps
StagedActor_DirectionSteps:
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
	.global StagedActor_FootprintKinds
StagedActor_FootprintKinds:
	.4byte 0x000000cf
	.4byte 0x000000cd
	.4byte 0x000000e4
	.4byte 0x000000e5
	.4byte 0x0000012a
	.4byte 0x00000129
	.global StagedActor_FootprintBounds
StagedActor_FootprintBounds:
	.4byte 0xffffffe0
	.4byte 0xfffffff8
	.4byte 0x00000020
	.4byte 0x00000008
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.4byte 0xffffffe0
	.4byte 0xfffffff0
	.4byte 0x00000020
	.4byte 0x00000000
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.4byte 0xffffffe0
	.4byte 0xfffffff8
	.4byte 0x00000020
	.4byte 0x00000008
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00ea0000
	.4byte 0x00000000
	.4byte 0x02400000
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
	.4byte 0x00ea0000
	.4byte 0x00000000
	.4byte 0x026f0000
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
	.4byte 0x00d30000
	.4byte 0x00000000
	.4byte 0x025a0000
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
	.4byte 0x020089dd
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
	.4byte 0x020089fd
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x00000180
	.4byte 0xc0000138
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000018
	.4byte 0x00000080
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x000000ac
	.4byte 0x0000025a
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x00101029
	.4byte 0x00201028
	.4byte 0x00301028
	.4byte 0x00408002
	.4byte 0x000001ff
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
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00024000
	.4byte 0xffff0037
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00dc
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00dc
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01004000
	.4byte 0xffff00dc
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01004000
	.4byte 0xffff00dc
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01004000
	.4byte 0xffff011d
	.4byte 0x00000007
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x028f0000
	.4byte 0x00004000
	.4byte 0xffff011d
	.4byte 0x00000007
	.4byte 0x00a60000
	.4byte 0x00000000
	.4byte 0x02400000
	.4byte 0x01004000
	.4byte 0xffff011d
	.4byte 0x00000007
	.4byte 0x00bd0000
	.4byte 0x00000000
	.4byte 0x02140000
	.4byte 0x01004000
	.4byte 0xffff011d
	.4byte 0x00000007
	.4byte 0x01200000
	.4byte 0x00000000
	.4byte 0x023a0000
	.4byte 0x01004000
	.4byte 0xffff011d
	.4byte 0x00000007
	.4byte 0x014b0000
	.4byte 0x00000000
	.4byte 0x02930000
	.4byte 0x01004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte 0x02008a69
	.4byte 0x00000602
	.4byte 0xffff000b
	.4byte 0x02008c15
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x0200a799
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
	.4byte 0x0200a865
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0xfffe0000
	.4byte 0x00000022
	.4byte 0x0200a865
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
	.4byte 0x0200a865
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000028
	.4byte 0xc0010000
	.4byte 0x00000022
	.4byte 0x0200a88d
	.4byte 0x0000001b
