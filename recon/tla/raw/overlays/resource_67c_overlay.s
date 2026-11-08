.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x020080ce,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200815a,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200a340,"ax",%progbits
	.global Func_02002340
	.thumb_func
Func_02002340:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r6, .L_0200a644
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r2, #0
	ldrsh r5, [r3, r2]
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r2, #0
	ldrsh r7, [r3, r2]
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	mov r8, r3
	ldrb r2, [r1, #23]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #23]
	sub sp, #12
	bl Func_02003278
	ldr r3, .L_0200a648
	cmp r5, r3
	beq .L_0200a380
	b .L_0200a52a
.L_0200a380:
	subs r3, r7, #3
	cmp r3, #1
	bls .L_0200a38e
	cmp r7, #6
	beq .L_0200a38e
	cmp r7, #8
	bne .L_0200a39c
.L_0200a38e:
	movs r2, #2
	negs r2, r2
	movs r0, #33
	movs r1, #0
	bl Func_020032d4
	b .L_0200a4c8
.L_0200a39c:
	cmp r7, #5
	beq .L_0200a3a4
	cmp r7, #7
	bne .L_0200a462
.L_0200a3a4:
	movs r0, #12
	bl Actor_GetById
	adds r6, r0, #0
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	movs r1, #0
	str r5, [r6, #20]
	str r5, [r6, #12]
	bl Func_02003cc4
	movs r0, #13
	bl Actor_GetById
	adds r6, r0, #0
	adds r3, r6, #0
	adds r3, #85
	movs r0, #140
	strb r5, [r3]
	lsls r0, r0, #2
	str r5, [r6, #20]
	str r5, [r6, #12]
	bl GameFlag_Test
	adds r5, r0, #0
	cmp r5, #0
	bne .L_0200a422
	movs r3, #104
	movs r2, #40
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #104
	movs r1, #80
	movs r2, #16
	movs r3, #10
	bl Func_02003d24
	movs r3, #39
	movs r2, #105
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #110
	movs r1, #116
	movs r2, #18
	movs r3, #12
	bl Func_02003d24
	movs r3, #32
	movs r0, #64
	movs r1, #32
	movs r2, #64
	str r5, [sp, #0]
	str r3, [sp, #4]
	bl Func_02003d1c
	movs r0, #33
	movs r1, #0
	movs r2, #0
	bl Func_020032d4
	b .L_0200a440
.L_0200a422:
	movs r1, #4
	movs r0, #33
	movs r2, #0
	bl Func_020032d4
	movs r0, #12
	bl Actor_GetById
	adds r6, r0, #0
	ldr r0, [r6, #8]
	movs r1, #0
	asrs r0, r0, #20
	subs r0, #42
	bl Func_0200349c
.L_0200a440:
	movs r0, #148
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a4c8
	movs r3, #43
	movs r2, #44
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #41
	movs r1, #42
	movs r2, #1
	movs r3, #1
	bl Func_02003d1c
	b .L_0200a4c8
.L_0200a462:
	adds r3, r7, #0
	subs r3, #9
	cmp r3, #1
	bhi .L_0200a4c8
	movs r0, #153
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0200a4be
	movs r3, #77
	str r3, [sp, #0]
	movs r5, #32
	movs r0, #77
	movs r1, #80
	movs r2, #23
	movs r3, #20
	str r5, [sp, #4]
	bl Func_02003d24
	movs r3, #12
	movs r2, #98
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #112
	movs r1, #96
	movs r2, #16
	movs r3, #20
	bl Func_02003d24
	movs r0, #64
	movs r1, #32
	movs r2, #64
	movs r3, #32
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003d1c
	movs r0, #33
	movs r1, #0
	movs r2, #0
	bl Func_020032d4
	b .L_0200a4c8
.L_0200a4be:
	movs r0, #33
	movs r1, #4
	movs r2, #0
	bl Func_020032d4
.L_0200a4c8:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #90
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a50a
	movs r1, #184
	movs r2, #200
	movs r0, #10
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02003ddc
	movs r3, #9
	movs r5, #12
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #12
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02003d1c
	movs r3, #13
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #12
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02003d1c
.L_0200a50a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #91
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a51a
	b .L_0200abc4
.L_0200a51a:
	movs r1, #206
	movs r2, #216
	movs r0, #11
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02003ddc
	b .L_0200abc4
.L_0200a52a:
	ldr r3, .L_0200a64c
	cmp r5, r3
	bne .L_0200a558
	subs r3, r7, #1
	cmp r3, #1
	bhi .L_0200a542
	movs r2, #2
	negs r2, r2
	movs r0, #33
	movs r1, #0
	bl Func_020032d4
.L_0200a542:
	movs r0, #8
	bl Actor_GetById
	adds r6, r0, #0
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	str r3, [r6, #20]
	str r3, [r6, #12]
	b .L_0200abc4
.L_0200a558:
	ldr r3, .L_0200a650
	cmp r5, r3
	bne .L_0200a654
	movs r0, #0
	bl Func_02003ea4
	movs r0, #9
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #10
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r2, #2
	negs r2, r2
	movs r0, #33
	movs r1, #8
	bl Func_020032d4
	movs r0, #40
	movs r1, #8
	movs r2, #8
	bl Func_020032d4
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #102
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a5cc
	bl Func_02003628
	movs r1, #4
	movs r2, #16
	movs r3, #2
	movs r0, #16
	bl Func_02003688
	movs r0, #22
	movs r1, #4
	movs r2, #16
	movs r3, #0
	bl Func_02003688
	movs r5, #5
.L_0200a5be:
	movs r0, #33
	movs r1, #55
	subs r5, #1
	bl Func_020036b8
	cmp r5, #0
	bge .L_0200a5be
.L_0200a5cc:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #93
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a62c
	movs r3, #80
	movs r2, #27
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #75
	movs r1, #96
	movs r2, #7
	movs r3, #13
	bl Func_02003d24
	movs r5, #16
	movs r6, #31
	movs r0, #91
	movs r1, #96
	movs r2, #7
	movs r3, #10
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02003d24
	movs r3, #91
	str r3, [sp, #4]
	movs r0, #99
	movs r1, #96
	movs r2, #7
	movs r3, #14
	str r5, [sp, #0]
	bl Func_02003d24
	movs r3, #17
	str r3, [sp, #0]
	movs r0, #43
	movs r1, #0
	movs r2, #5
	movs r3, #9
	str r6, [sp, #4]
	bl Func_02003d1c
	bl Func_020007a8
	b .L_0200abc4
.L_0200a62c:
	movs r3, #17
	movs r2, #31
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #17
	movs r1, #32
	movs r2, #5
	movs r3, #1
	bl Func_02003d1c
	b .L_0200abc4
	.2byte 0x0000
.L_0200a644:
	.4byte gMainRoutineWork
.L_0200a648:
	.4byte 0x000000a0
.L_0200a64c:
	.4byte 0x000000a1
.L_0200a650:
	.4byte 0x000000a2
.L_0200a654:
	ldr r3, .L_0200a720
	cmp r5, r3
	beq .L_0200a65c
	b .L_0200a7cc
.L_0200a65c:
	movs r0, #0
	bl Func_02003ea4
	subs r3, r7, #6
	cmp r3, #1
	bls .L_0200a66a
	b .L_0200a7aa
.L_0200a66a:
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r6, r2
	ldr r0, [r3]
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r1, #5
	movs r2, #4
	movs r0, #12
	bl Func_02003420
	movs r1, #3
	movs r2, #4
	movs r0, #13
	bl Func_02003420
	movs r1, #4
	movs r2, #4
	movs r0, #14
	bl Func_02003420
	movs r0, #40
	movs r1, #4
	movs r2, #4
	bl Func_020032d4
	movs r0, #165
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a724
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #100
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a724
	mov r3, r8
	ldr r6, [r3, #32]
	movs r3, #71
	str r3, [sp, #0]
	movs r5, #39
	movs r0, #64
	movs r1, #64
	movs r2, #16
	movs r3, #13
	str r5, [sp, #4]
	bl Func_02003d24
	movs r3, #7
	str r3, [sp, #0]
	movs r0, #71
	movs r1, #39
	movs r2, #16
	movs r3, #15
	str r5, [sp, #4]
	bl Func_02003d1c
	bl Func_02000b84
	movs r3, #131
	lsls r3, r3, #1
	adds r2, r6, r3
	movs r3, #1
	strb r3, [r2]
	movs r0, #128
	lsls r0, r0, #19
	adds r0, #10
	ldrh r2, [r0]
	movs r3, #255
	lsls r3, r3, #8
	mov r1, sp
	adds r3, #252
	adds r1, #10
	ands r3, r2
	strh r3, [r1]
	ldr r2, .L_0200a71c
	orrs r3, r2
	strh r3, [r0]
	b .L_0200a78c
	.2byte 0x0000
.L_0200a71c:
	.4byte 0x00000001
.L_0200a720:
	.4byte 0x000000a3
.L_0200a724:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #100
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a73e
	movs r0, #146
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a78c
.L_0200a73e:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #100
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a75a
	movs r1, #140
	movs r2, #178
	movs r0, #12
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02003ddc
.L_0200a75a:
	movs r1, #38
	movs r0, #12
	bl Func_02000b24
	bl Func_02003628
	movs r0, #12
	movs r1, #2
	movs r2, #45
	movs r3, #1
	bl Func_02003688
	movs r5, #7
.L_0200a774:
	movs r0, #33
	movs r1, #55
	subs r5, #1
	bl Func_020036b8
	cmp r5, #0
	bge .L_0200a774
	movs r0, #165
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
.L_0200a78c:
	movs r5, #12
.L_0200a78e:
	adds r0, r5, #0
	bl Actor_GetById
	adds r6, r0, #0
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	adds r5, #1
	strb r3, [r2]
	str r3, [r6, #20]
	str r3, [r6, #12]
	cmp r5, #14
	ble .L_0200a78e
	b .L_0200a7b6
.L_0200a7aa:
	movs r2, #4
	negs r2, r2
	movs r0, #33
	movs r1, #0
	bl Func_020032d4
.L_0200a7b6:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #106
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a7c6
	b .L_0200abc4
.L_0200a7c6:
	bl Func_02000ad8
	b .L_0200abc4
.L_0200a7cc:
	ldr r3, .L_0200aa50
	cmp r5, r3
	bne .L_0200a82c
	movs r2, #2
	negs r2, r2
	movs r0, #33
	movs r1, #0
	bl Func_020032d4
	movs r0, #40
	movs r1, #0
	movs r2, #0
	bl Func_020032d4
	movs r0, #0
	bl Func_02003ea4
	subs r3, r7, #1
	cmp r3, #3
	bls .L_0200a7fa
	cmp r7, #7
	beq .L_0200a7fa
	b .L_0200abc4
.L_0200a7fa:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #103
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a80a
	b .L_0200abc4
.L_0200a80a:
	bl Func_02003628
	movs r0, #44
	movs r1, #0
	movs r2, #34
	movs r3, #2
	bl Func_02003688
	movs r5, #0
.L_0200a81c:
	movs r0, #33
	movs r1, #55
	adds r5, #1
	bl Func_020036b8
	cmp r5, #4
	ble .L_0200a81c
	b .L_0200abc4
.L_0200a82c:
	ldr r3, .L_0200aa54
	cmp r5, r3
	bne .L_0200a900
	movs r0, #0
	bl Func_02003ea4
	cmp r7, #1
	beq .L_0200a840
	cmp r7, #13
	bne .L_0200a84e
.L_0200a840:
	movs r2, #2
	negs r2, r2
	movs r0, #33
	movs r1, #0
	bl Func_020032d4
	b .L_0200a866
.L_0200a84e:
	cmp r7, #9
	beq .L_0200a85a
	cmp r7, #11
	beq .L_0200a85a
	cmp r7, #12
	bne .L_0200a866
.L_0200a85a:
	movs r2, #2
	negs r2, r2
	movs r0, #33
	movs r1, #0
	bl Func_020032d4
.L_0200a866:
	movs r0, #142
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a890
	movs r5, #13
.L_0200a874:
	adds r0, r5, #0
	bl Actor_GetById
	adds r6, r0, #0
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	adds r5, #1
	strb r3, [r2]
	str r3, [r6, #8]
	str r3, [r6, #16]
	cmp r5, #15
	ble .L_0200a874
	b .L_0200abc4
.L_0200a890:
	movs r3, #113
	movs r2, #39
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #114
	movs r1, #39
	movs r2, #1
	movs r3, #2
	bl Func_02003d24
	movs r5, #41
	movs r0, #105
	movs r1, #41
	movs r2, #12
	movs r3, #13
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003d1c
	movs r3, #105
	str r3, [sp, #0]
	movs r0, #105
	movs r1, #105
	movs r2, #12
	movs r3, #13
	str r5, [sp, #4]
	bl Func_02003d24
	movs r5, #13
.L_0200a8ca:
	adds r0, r5, #0
	bl Actor_GetById
	movs r2, #145
	lsls r2, r2, #2
	adds r6, r0, #0
	adds r0, r5, r2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a8f8
	ldr r2, [r6, #8]
	ldr r3, [r6, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #47
	movs r1, #45
	movs r2, #1
	movs r3, #1
	bl Func_02003d1c
.L_0200a8f8:
	adds r5, #1
	cmp r5, #15
	ble .L_0200a8ca
	b .L_0200abc4
.L_0200a900:
	ldr r3, .L_0200aa58
	cmp r5, r3
	bne .L_0200a914
	movs r2, #2
	negs r2, r2
	movs r0, #33
	movs r1, #0
	bl Func_020032d4
	b .L_0200abc4
.L_0200a914:
	ldr r3, .L_0200aa5c
	cmp r5, r3
	bne .L_0200a93c
	movs r0, #0
	bl Func_02003ea4
	movs r2, #2
	negs r2, r2
	movs r0, #33
	movs r1, #0
	bl Func_020032d4
	bl Func_02003edc
	movs r0, #0
	movs r1, #8
	movs r2, #9
	bl Func_02003ee4
	b .L_0200abc4
.L_0200a93c:
	ldr r3, .L_0200aa60
	cmp r5, r3
	beq .L_0200a944
	b .L_0200ab46
.L_0200a944:
	subs r3, r7, #3
	cmp r3, #1
	bls .L_0200a94c
	b .L_0200aafe
.L_0200a94c:
	ldr r3, .L_0200aa64
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r1, #6
	movs r2, #4
	movs r0, #10
	bl Func_02003420
	movs r1, #3
	movs r2, #4
	movs r0, #11
	bl Func_02003420
	movs r1, #5
	movs r2, #4
	movs r0, #12
	bl Func_02003420
	movs r1, #6
	movs r2, #4
	movs r0, #13
	bl Func_02003420
	movs r1, #5
	movs r2, #4
	movs r0, #14
	bl Func_02003420
	movs r1, #4
	movs r2, #4
	movs r0, #15
	bl Func_02003420
	movs r1, #4
	movs r2, #4
	movs r0, #16
	bl Func_02003420
	movs r1, #3
	movs r2, #4
	movs r0, #17
	bl Func_02003420
	movs r1, #6
	movs r2, #4
	movs r0, #18
	bl Func_02003420
	movs r0, #40
	movs r1, #4
	movs r2, #4
	bl Func_020032d4
	movs r0, #166
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200aa68
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #101
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200aa68
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #32]
	movs r3, #100
	str r3, [sp, #0]
	movs r5, #13
	movs r0, #64
	movs r1, #64
	movs r2, #19
	movs r3, #17
	str r5, [sp, #4]
	bl Func_02003d24
	movs r3, #35
	movs r2, #73
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #35
	movs r1, #105
	movs r2, #23
	movs r3, #24
	bl Func_02003d24
	movs r3, #36
	str r3, [sp, #0]
	movs r0, #100
	movs r1, #13
	movs r2, #19
	movs r3, #19
	str r5, [sp, #4]
	bl Func_02003d1c
	movs r3, #131
	lsls r3, r3, #1
	adds r2, r6, r3
	movs r3, #1
	strb r3, [r2]
	movs r0, #128
	lsls r0, r0, #19
	adds r0, #10
	ldrh r2, [r0]
	movs r3, #255
	lsls r3, r3, #8
	mov r1, sp
	adds r3, #252
	adds r1, #10
	ands r3, r2
	strh r3, [r1]
	ldr r2, .L_0200aa4c
	orrs r3, r2
	strh r3, [r0]
	b .L_0200aafe
.L_0200aa4c:
	.4byte 0x00000001
.L_0200aa50:
	.4byte 0x000000a4
.L_0200aa54:
	.4byte 0x000000a5
.L_0200aa58:
	.4byte 0x000000a6
.L_0200aa5c:
	.4byte 0x000000a7
.L_0200aa60:
	.4byte 0x000000a8
.L_0200aa64:
	.4byte gMainRoutineWork
.L_0200aa68:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #101
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200aa84
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #74
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200aafe
.L_0200aa84:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #101
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200aabc
	movs r1, #202
	movs r2, #148
	movs r0, #12
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02003ddc
	movs r1, #170
	movs r2, #180
	movs r0, #13
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02003ddc
	movs r1, #186
	movs r2, #212
	movs r0, #15
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02003ddc
.L_0200aabc:
	movs r0, #12
	movs r1, #38
	bl Func_02000b24
	movs r0, #13
	movs r1, #39
	bl Func_02000b24
	movs r1, #37
	movs r0, #15
	bl Func_02000b24
	bl Func_02003628
	movs r0, #38
	movs r1, #2
	movs r2, #19
	movs r3, #1
	bl Func_02003688
	movs r5, #7
.L_0200aae6:
	movs r0, #33
	movs r1, #55
	subs r5, #1
	bl Func_020036b8
	cmp r5, #0
	bge .L_0200aae6
	movs r0, #166
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
.L_0200aafe:
	movs r5, #10
.L_0200ab00:
	adds r0, r5, #0
	bl Actor_GetById
	adds r6, r0, #0
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	adds r5, #1
	strb r3, [r2]
	str r3, [r6, #20]
	str r3, [r6, #12]
	cmp r5, #20
	ble .L_0200ab00
	movs r0, #19
	bl Actor_GetById
	adds r6, r0, #0
	ldr r3, [r6, #8]
	movs r2, #39
	asrs r3, r3, #20
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #23
	movs r1, #39
	movs r2, #1
	bl Func_02003d1c
	subs r3, r7, #1
	cmp r3, #1
	bhi .L_0200abc4
	movs r0, #0
	bl Func_02003ea4
	b .L_0200abc4
.L_0200ab46:
	ldr r3, .L_0200abd0
	cmp r5, r3
	bne .L_0200abc4
	movs r0, #0
	bl Func_02003ea4
	movs r2, #2
	negs r2, r2
	movs r0, #33
	movs r1, #0
	bl Func_020032d4
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_SetBit
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #73
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200ab8a
	movs r3, #13
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #38
	movs r1, #9
	movs r2, #7
	movs r3, #6
	bl Func_02003d1c
	b .L_0200abc4
.L_0200ab8a:
	movs r3, #50
	str r3, [sp, #0]
	movs r5, #8
	movs r0, #77
	movs r1, #8
	movs r2, #7
	movs r3, #6
	str r5, [sp, #4]
	bl Func_02003d24
	movs r3, #77
	str r3, [sp, #0]
	movs r0, #70
	movs r1, #8
	movs r2, #7
	movs r3, #6
	str r5, [sp, #4]
	bl Func_02003d24
	movs r3, #13
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #38
	movs r1, #17
	movs r2, #7
	movs r3, #6
	bl Func_02003d1c
.L_0200abc4:
	movs r0, #0
	add sp, #12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200abd0:
	.4byte 0x000000a9
	.section .bss,"aw",%nobits
	.global gData_0200536c
gData_0200536c:
	.space 0x00000604
	.global gData_02005970
gData_02005970:
