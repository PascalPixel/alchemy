.syntax unified
	.thumb
	.section .text.x020091d8,"ax",%progbits
	.balign 4
	.global Func_020011d8
	.thumb_func
Func_020011d8:
	push {r5, lr}
	ldr r0, [pc, #92]
	bl 0x0200df9c
	cmp r0, #0
	bne .L_020011d8_0
	bl 0x0200dfbc
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #0
	bl 0x0200dfec
	ldr r5, [pc, #68]
	adds r0, r5, #0
	bl 0x0200e084
	movs r0, #15
	movs r1, #0
	movs r2, #2
	bl 0x0200e09c
	adds r5, #2
	movs r2, #2
	movs r0, #16
	movs r1, #0
	bl 0x0200e09c
	movs r1, #1
	adds r0, r5, #0
	bl 0x0200df7c
	movs r0, #6
	bl 0x0200dfb4
	movs r0, #0
	movs r1, #69
	ldr r2, [pc, #24]
	bl 0x0200e024
	bl 0x0200dfc4
.L_020011d8_0:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000808
	.4byte 0x00000f4d
	.4byte 0x00000366
	.section .text.x0200acb0,"ax",%progbits
	.balign 4
	.global Func_02002cb0
	.thumb_func
Func_02002cb0:
	push {r5, lr}
	bl 0x0200dfbc
	movs r0, #0
	movs r1, #0
	bl 0x0200e07c
	movs r2, #20
	movs r1, #0
	movs r0, #8
	bl 0x0200e06c
	ldr r5, [pc, #548]
	adds r0, r5, #0
	bl 0x0200e084
	movs r0, #8
	movs r1, #2
	bl 0x0200e05c
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200e09c
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200e0dc
	movs r0, #199
	movs r1, #1
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	ldr r2, [pc, #504]
	bl 0x0200e0e4
	movs r0, #0
	ldr r1, [pc, #500]
	ldr r2, [pc, #504]
	bl 0x0200dfec
	movs r0, #1
	ldr r1, [pc, #492]
	ldr r2, [pc, #492]
	bl 0x0200dfec
	movs r1, #210
	movs r2, #152
	movs r0, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200e024
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200e0ac
	movs r0, #0
	bl 0x0200dfe4
	cmp r0, #0
	beq .L_02002cb0_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200e03c
.L_02002cb0_0:
	movs r1, #201
	movs r2, #152
	movs r0, #1
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200e024
	movs r1, #208
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200e0ac
	ldr r0, [pc, #408]
	movs r1, #0
	bl 0x0200e094
	movs r1, #160
	movs r2, #20
	movs r0, #8
	lsls r1, r1, #7
	bl 0x0200e0ac
	movs r0, #8
	movs r1, #3
	bl 0x0200e04c
	ldr r0, [pc, #384]
	movs r1, #0
	bl 0x0200e094
	movs r0, #0
	movs r1, #3
	bl 0x0200e044
	movs r1, #3
	movs r0, #1
	bl 0x0200e04c
	movs r0, #20
	bl 0x0200dfb4
	movs r0, #8
	movs r1, #2
	bl 0x0200e064
	movs r1, #0
	ldr r0, [pc, #344]
	bl 0x0200e08c
	movs r0, #0
	movs r1, #0
	bl 0x0200dfdc
	cmp r0, #1
	bne .L_02002cb0_1
	ldr r3, [pc, #328]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #8
	movs r1, #1
	bl 0x0200e05c
.L_02002cb0_1:
	ldr r0, [pc, #300]
	movs r1, #0
	movs r2, #40
	bl 0x0200e09c
	ldr r1, [pc, #300]
	movs r2, #60
	movs r0, #8
	bl 0x0200e0c4
	adds r0, r5, #6
	bl 0x0200e084
	movs r2, #20
	ldr r0, [pc, #272]
	movs r1, #0
	bl 0x0200e09c
	movs r1, #1
	movs r0, #1
	bl 0x0200e064
	movs r0, #40
	bl 0x0200dfb4
	movs r2, #40
	ldr r0, [pc, #244]
	movs r1, #0
	bl 0x0200e09c
	movs r0, #8
	movs r1, #1
	bl 0x0200e064
	movs r1, #208
	movs r2, #20
	movs r0, #8
	lsls r1, r1, #8
	bl 0x0200e0ac
	ldr r0, [pc, #220]
	movs r1, #0
	bl 0x0200e094
	movs r0, #1
	movs r1, #3
	bl 0x0200e04c
	ldr r0, [pc, #200]
	movs r1, #0
	movs r2, #120
	bl 0x0200e09c
	ldr r0, [pc, #196]
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r0, #1
	ldr r1, [pc, #192]
	movs r2, #40
	bl 0x0200e0c4
	movs r2, #40
	ldr r0, [pc, #168]
	movs r1, #0
	bl 0x0200e09c
	movs r0, #8
	movs r1, #4
	bl 0x0200e04c
	movs r2, #20
	ldr r0, [pc, #156]
	movs r1, #0
	bl 0x0200e09c
	movs r1, #3
	movs r0, #1
	bl 0x0200e04c
	movs r0, #40
	bl 0x0200dfb4
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200e0ac
	movs r2, #10
	ldr r0, [pc, #120]
	movs r1, #0
	bl 0x0200e09c
	movs r0, #0
	movs r1, #3
	bl 0x0200e044
	movs r1, #3
	movs r0, #1
	bl 0x0200e04c
	movs r0, #20
	bl 0x0200dfb4
	movs r0, #8
	movs r1, #3
	bl 0x0200e04c
	movs r0, #1
	movs r1, #2
	bl 0x0200e044
	movs r0, #0
	bl 0x0200dfe4
	cmp r0, #0
	beq .L_02002cb0_2
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x0200e00c
.L_02002cb0_2:
	movs r0, #1
	bl 0x0200e034
	movs r1, #0
	movs r2, #0
	movs r0, #1
	bl 0x0200e03c
	ldr r0, [pc, #44]
	bl 0x0200dfa4
	bl 0x0200dfc4
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00001c45
	.4byte 0x02460000
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x00001001
	.4byte 0x00004008
	.4byte 0x03001ebc
	.4byte 0x00000105
	.4byte 0x00000303
	.global Func_02002f14
	.thumb_func
Func_02002f14:
	push {lr}
	bl 0x0200dfbc
	movs r1, #1
	movs r3, #1
	ldr r0, [pc, #624]
	negs r1, r1
	ldr r2, [pc, #624]
	bl 0x0200e0e4
	movs r0, #0
	ldr r1, [pc, #620]
	ldr r2, [pc, #620]
	bl 0x0200e024
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r0, #0
	bl 0x0200dfe4
	cmp r0, #0
	beq .L_02002f14_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200e03c
.L_02002f14_0:
	movs r1, #173
	movs r0, #1
	lsls r1, r1, #1
	ldr r2, [pc, #576]
	bl 0x0200e024
	movs r1, #208
	movs r2, #20
	lsls r1, r1, #8
	movs r0, #1
	bl 0x0200e0ac
	ldr r0, [pc, #564]
	bl 0x0200e084
	movs r0, #1
	movs r1, #0
	bl 0x0200e094
	movs r0, #9
	movs r1, #2
	bl 0x0200e064
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200e0c4
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #10
	bl 0x0200e0ac
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #10
	bl 0x0200e0ac
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #40
	bl 0x0200e0ac
	movs r2, #20
	movs r0, #9
	movs r1, #0
	bl 0x0200e09c
	movs r0, #0
	movs r1, #3
	bl 0x0200e044
	movs r0, #1
	movs r1, #3
	bl 0x0200e04c
	movs r0, #9
	movs r1, #1
	bl 0x0200e064
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #10
	bl 0x0200e0ac
	movs r2, #20
	movs r0, #9
	movs r1, #0
	bl 0x0200e09c
	movs r0, #1
	movs r1, #1
	bl 0x0200e05c
	movs r0, #1
	ldr r1, [pc, #432]
	movs r2, #40
	bl 0x0200e0c4
	movs r2, #10
	movs r0, #1
	movs r1, #0
	bl 0x0200e09c
	movs r0, #9
	movs r1, #3
	bl 0x0200e04c
	movs r0, #9
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #128
	movs r2, #40
	movs r0, #1
	lsls r1, r1, #5
	bl 0x0200e0ac
	movs r0, #9
	movs r1, #4
	bl 0x0200e04c
	movs r0, #9
	movs r1, #0
	bl 0x0200e094
	movs r1, #176
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #208
	movs r2, #10
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r1, #2
	movs r0, #1
	bl 0x0200e064
	movs r0, #20
	bl 0x0200dfb4
	movs r2, #10
	movs r0, #1
	movs r1, #0
	bl 0x0200e09c
	movs r0, #9
	movs r1, #3
	bl 0x0200e04c
	movs r0, #9
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200e0c4
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #80
	bl 0x0200e0c4
	movs r2, #20
	movs r0, #1
	movs r1, #0
	bl 0x0200e09c
	movs r0, #9
	movs r1, #2
	bl 0x0200e064
	movs r0, #9
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #5
	movs r2, #20
	bl 0x0200e0ac
	movs r1, #0
	movs r0, #1
	bl 0x0200e08c
	movs r0, #0
	movs r1, #0
	bl 0x0200dfdc
	cmp r0, #0
	bne .L_02002f14_1
	movs r0, #1
	ldr r1, [pc, #208]
	movs r2, #60
	bl 0x0200e0c4
	b .L_02002f14_2
.L_02002f14_1:
	ldr r3, [pc, #200]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02002f14_2:
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #208
	movs r2, #10
	lsls r1, r1, #8
	movs r0, #1
	bl 0x0200e0ac
	ldr r0, [pc, #168]
	bl 0x0200e084
	movs r0, #1
	movs r1, #0
	bl 0x0200e094
	movs r0, #9
	movs r1, #3
	bl 0x0200e04c
	movs r2, #20
	movs r0, #9
	movs r1, #0
	bl 0x0200e09c
	movs r1, #3
	movs r0, #1
	bl 0x0200e04c
	movs r0, #20
	bl 0x0200dfb4
	movs r1, #128
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #5
	bl 0x0200e0ac
	movs r0, #1
	movs r1, #3
	bl 0x0200e04c
	movs r0, #0
	movs r1, #3
	bl 0x0200e04c
	movs r0, #1
	movs r1, #2
	bl 0x0200e044
	movs r0, #0
	bl 0x0200dfe4
	cmp r0, #0
	beq .L_02002f14_3
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x0200e00c
.L_02002f14_3:
	movs r0, #1
	bl 0x0200e034
	movs r1, #0
	movs r2, #0
	movs r0, #1
	bl 0x0200e03c
	movs r0, #193
	lsls r0, r0, #2
	bl 0x0200dfa4
	bl 0x0200dfc4
	pop {r0}
	bx r0
	.4byte 0x01650000
	.4byte 0x02e20000
	.4byte 0x0000016f
	.4byte 0x000002e9
	.4byte 0x00001c53
	.4byte 0x00000103
	.4byte 0x00000105
	.4byte 0x03001ebc
	.4byte 0x00001c60
	.section .text.x0200b4c8,"ax",%progbits
	.balign 4
	.global FieldScene_RunLargeStagingSequence
	.thumb_func
FieldScene_RunLargeStagingSequence:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #0
	sub sp, #28
	bl 0x0200dfe4
	mov r9, r0
	movs r0, #14
	bl 0x0200dfe4
	adds r6, r0, #0
	bl 0x0200dfbc
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	movs r3, #0
	negs r1, r1
	negs r0, r0
	bl 0x0200e0e4
	movs r0, #1
	bl 0x0200de8c
	bl 0x0200e0f4
	movs r7, #0
	adds r0, #85
	movs r1, #0
	strb r7, [r0]
	movs r0, #1
	mov r10, r1
	bl 0x0200de8c
	movs r3, #20
	movs r2, #50
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #49
	movs r1, #53
	movs r2, #8
	movs r3, #4
	bl 0x0200df3c
	movs r3, #2
	str r3, [sp, #0]
	movs r5, #1
	movs r0, #2
	movs r1, #102
	movs r2, #84
	movs r3, #41
	str r5, [sp, #4]
	bl 0x0200df34
	movs r0, #1
	movs r1, #102
	movs r2, #83
	movs r3, #41
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200df34
	movs r1, #103
	movs r2, #82
	movs r3, #42
	movs r0, #0
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200df34
	movs r0, #11
	bl 0x0200dfe4
	adds r7, r0, #0
	adds r3, r7, #0
	mov r2, r10
	adds r3, #85
	strb r2, [r3]
	movs r3, #160
	lsls r3, r3, #16
	mov r8, r3
	str r3, [r7, #12]
	movs r5, #194
	movs r3, #210
	lsls r5, r5, #17
	lsls r3, r3, #18
	str r3, [r7, #16]
	str r5, [r7, #8]
	movs r1, #0
	bl 0x0200df4c
	movs r0, #12
	bl 0x0200dfe4
	adds r7, r0, #0
	adds r3, r7, #0
	mov r1, r10
	adds r3, #85
	strb r1, [r3]
	movs r3, #211
	mov r2, r8
	lsls r3, r3, #18
	str r2, [r7, #12]
	str r3, [r7, #16]
	str r5, [r7, #8]
	movs r1, #0
	bl 0x0200df4c
	movs r0, #13
	bl 0x0200dfe4
	adds r7, r0, #0
	adds r3, r7, #0
	mov r1, r10
	adds r3, #85
	strb r1, [r3]
	movs r3, #212
	lsls r3, r3, #18
	mov r2, r8
	str r3, [r7, #16]
	str r2, [r7, #12]
	str r5, [r7, #8]
	movs r1, #0
	bl 0x0200df4c
	movs r0, #11
	bl 0x0200dfe4
	movs r1, #0
	bl 0x0200df4c
	movs r0, #12
	bl 0x0200dfe4
	movs r1, #0
	bl 0x0200df4c
	movs r0, #0
	movs r1, #11
	bl 0x0200e044
	ldr r7, [pc, #1016]
	movs r0, #0
	adds r1, r7, #0
	bl 0x0200dff4
	bl 0x0200df64
	ldr r5, [pc, #1004]
	movs r1, #0
	adds r0, r5, #0
	movs r2, #0
	bl 0x0200df84
	bl 0x0200df6c
	ldr r2, [pc, #992]
	movs r3, #0
	mov r1, r8
	ldr r0, [pc, #992]
	bl 0x0200e0e4
	bl 0x0200df14
	movs r0, #1
	bl 0x0200de8c
	ldr r0, [pc, #980]
	ldr r1, [pc, #980]
	bl 0x0200e0dc
	movs r0, #148
	movs r3, #1
	lsls r0, r0, #17
	mov r1, r8
	ldr r2, [pc, #972]
	bl 0x0200e0e4
	ldr r1, [pc, #968]
	ldr r2, [pc, #972]
	movs r0, #5
	bl 0x0200e03c
	movs r0, #1
	bl 0x0200de8c
	movs r0, #5
	ldr r1, [pc, #960]
	ldr r2, [pc, #960]
	bl 0x0200dfec
	movs r1, #210
	movs r0, #5
	lsls r1, r1, #1
	ldr r2, [pc, #952]
	bl 0x0200e01c
	bl 0x0200e134
	ldr r3, [pc, #948]
	ldr r2, [r3]
	movs r3, #228
	lsls r3, r3, #1
	mov r10, r3
	mov r1, r10
	movs r3, #60
	str r3, [r2, r1]
	bl 0x0200e104
	movs r0, #5
	bl 0x0200e034
	movs r1, #128
	movs r2, #128
	movs r0, #5
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200dfec
	movs r2, #133
	movs r0, #5
	ldr r1, [pc, #908]
	lsls r2, r2, #3
	bl 0x0200e024
	movs r0, #5
	ldr r1, [pc, #900]
	ldr r2, [pc, #904]
	bl 0x0200dfec
	movs r0, #5
	ldr r1, [pc, #900]
	ldr r2, [pc, #900]
	bl 0x0200e024
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200dfec
	movs r1, #159
	ldr r2, [pc, #884]
	movs r0, #8
	lsls r1, r1, #1
	bl 0x0200e01c
	movs r0, #8
	movs r1, #2
	bl 0x0200e044
	movs r1, #206
	movs r0, #5
	lsls r1, r1, #1
	ldr r2, [pc, #856]
	bl 0x0200e024
	movs r1, #206
	movs r0, #5
	lsls r1, r1, #1
	ldr r2, [pc, #852]
	bl 0x0200e024
	movs r1, #187
	movs r2, #252
	movs r0, #5
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200e024
	movs r0, #5
	ldr r1, [pc, #832]
	ldr r2, [pc, #836]
	bl 0x0200e024
	movs r1, #159
	movs r0, #8
	lsls r1, r1, #1
	ldr r2, [pc, #812]
	bl 0x0200e024
	movs r2, #40
	movs r0, #5
	movs r1, #8
	bl 0x0200e074
	movs r0, #8
	movs r1, #2
	bl 0x0200e064
	movs r1, #3
	movs r0, #5
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	ldr r2, [pc, #788]
	movs r0, #8
	ldr r1, [pc, #788]
	bl 0x0200e01c
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #8
	lsls r1, r1, #5
	bl 0x0200e0dc
	movs r2, #230
	movs r0, #5
	ldr r1, [pc, #772]
	lsls r2, r2, #2
	bl 0x0200e024
	movs r2, #231
	ldr r1, [pc, #764]
	lsls r2, r2, #2
	movs r0, #5
	bl 0x0200e024
	bl 0x0200e0ec
	movs r0, #10
	bl 0x0200dfb4
	movs r1, #240
	movs r2, #30
	movs r0, #5
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r1, #2
	movs r0, #5
	bl 0x0200e064
	movs r0, #20
	bl 0x0200dfb4
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200e0dc
	movs r3, #1
	ldr r0, [pc, #708]
	mov r1, r8
	ldr r2, [pc, #708]
	bl 0x0200e0e4
	adds r5, #1
	bl 0x0200e0ec
	movs r1, #2
	movs r2, #20
	movs r0, #10
	bl 0x0200e054
	adds r0, r5, #0
	bl 0x0200e084
	ldr r0, [pc, #684]
	movs r1, #0
	movs r2, #10
	bl 0x0200e09c
	movs r3, #128
	lsls r3, r3, #9
	mov r2, r9
	str r3, [r2, #24]
	str r3, [r2, #28]
	movs r0, #0
	movs r1, #1
	bl 0x0200dff4
	movs r2, #40
	movs r0, #10
	movs r1, #0
	bl 0x0200e074
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200e0cc
	movs r1, #2
	movs r0, #0
	bl 0x0200e064
	movs r0, #40
	bl 0x0200dfb4
	movs r1, #2
	movs r0, #10
	bl 0x0200e05c
	movs r0, #40
	bl 0x0200dfb4
	movs r2, #40
	ldr r0, [pc, #608]
	movs r1, #0
	bl 0x0200e09c
	movs r0, #0
	movs r1, #11
	bl 0x0200e044
	movs r0, #0
	adds r1, r7, #0
	bl 0x0200dff4
	movs r1, #1
	movs r0, #5
	bl 0x0200e0d4
	bl 0x0200e0ec
	movs r0, #40
	bl 0x0200dfb4
	movs r0, #5
	movs r1, #3
	bl 0x0200e04c
	movs r1, #208
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200e0ac
	movs r1, #156
	movs r0, #5
	lsls r1, r1, #1
	ldr r2, [pc, #544]
	bl 0x0200e024
	movs r2, #190
	ldr r1, [pc, #540]
	lsls r2, r2, #2
	movs r0, #5
	bl 0x0200e024
	movs r0, #20
	bl 0x0200dfb4
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200e0ac
	movs r0, #5
	movs r1, #0
	movs r2, #40
	bl 0x0200e0ac
	ldr r0, [pc, #504]
	movs r1, #0
	movs r2, #10
	bl 0x0200e09c
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200e0c4
	movs r0, #5
	movs r1, #4
	movs r2, #40
	bl 0x0200e054
	movs r1, #192
	movs r2, #30
	movs r0, #5
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r1, #2
	movs r0, #5
	bl 0x0200e064
	movs r0, #40
	bl 0x0200dfb4
	movs r2, #30
	movs r0, #5
	movs r1, #0
	bl 0x0200e09c
	movs r1, #3
	movs r0, #5
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl 0x0200e0dc
	movs r0, #198
	movs r1, #1
	movs r2, #147
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #18
	bl 0x0200e0e4
	mov r1, r10
	ldr r2, [pc, #396]
	movs r0, #5
	bl 0x0200e024
	bl 0x0200e0ec
	movs r0, #10
	bl 0x0200dfb4
	movs r2, #40
	movs r0, #1
	ldr r1, [pc, #376]
	bl 0x0200e0c4
	movs r1, #2
	movs r0, #1
	bl 0x0200e064
	movs r0, #20
	bl 0x0200dfb4
	bl 0x0200d594
	movs r0, #1
	movs r1, #17
	bl 0x0200e044
	ldr r0, [pc, #348]
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r0, #131
	bl 0x0200e14c
	movs r5, #0
.L_020034c8_0:
	movs r0, #1
	bl 0x0200dfe4
	bl 0x0200dc20
	adds r5, #1
	movs r0, #1
	bl 0x0200de8c
	cmp r5, #59
	bls .L_020034c8_0
	movs r0, #1
	movs r1, #1
	bl 0x0200e0bc
	ldr r3, [pc, #304]
	movs r1, #200
	mov r10, r3
	mov r0, r10
	lsls r1, r1, #4
	bl 0x0200de94
	ldr r1, [pc, #296]
	mov r8, r1
	movs r1, #200
	lsls r1, r1, #4
	mov r0, r8
	bl 0x0200de94
	movs r0, #14
	bl 0x0200dfe4
	movs r1, #0
	bl 0x0200df4c
	movs r2, #0
	mov r9, r2
	adds r3, r6, #0
	mov r1, r9
	adds r3, #85
	strb r1, [r3]
	movs r3, #214
	lsls r3, r3, #17
	str r3, [r6, #8]
	movs r3, #128
	lsls r3, r3, #8
	mov r11, r3
	movs r2, #208
	ldr r3, [pc, #248]
	movs r5, #146
	lsls r5, r5, #18
	mov r1, r11
	lsls r2, r2, #16
	str r3, [r6, #108]
	str r2, [r6, #12]
	strh r1, [r6, #6]
	str r5, [r6, #16]
	movs r0, #4
	bl 0x0200dfb4
	movs r1, #128
	movs r2, #128
	movs r0, #14
	lsls r1, r1, #10
	lsls r2, r2, #10
	bl 0x0200dfec
	movs r1, #204
	movs r2, #208
	adds r3, r5, #0
	adds r0, r6, #0
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x0200df1c
	movs r0, #40
	bl 0x0200dfb4
	movs r0, #9
	ldr r1, [pc, #188]
	ldr r2, [pc, #192]
	bl 0x0200dfec
	ldr r1, [pc, #180]
	ldr r2, [pc, #184]
.L_020039cc:
	movs r0, #14
	bl 0x0200dfec
	movs r0, #9
	bl 0x0200dfe4
	movs r1, #196
	movs r2, #208
	adds r3, r5, #0
	adds r0, r6, #0
	lsls r1, r1, #17
	lsls r2, r2, #16
	b .L_020039cc_0
	.2byte 0x0000
	.2byte 0xe590
	.2byte 0x0200
	.2byte 0x0ee8
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0495
	.2byte 0x0000
	.2byte 0x0153
	.2byte 0x547a
	.2byte 0x0000
	.2byte 0x0a8f
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0399
	.2byte 0x0000
	.2byte 0x0199
	.2byte 0x0000
	.2byte 0x046e
	.2byte 0xb333
	.2byte 0x0000
	.2byte 0x5999
	.2byte 0x0000
	.2byte 0x042c
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x0155
	.2byte 0x0000
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.2byte 0x0167
	.2byte 0x0000
	.2byte 0x0409
	.2byte 0x0000
	.2byte 0x03b3
	.2byte 0x0000
	.2byte 0x03fb
	.2byte 0x0000
	.2byte 0x015b
	.2byte 0x0000
	.2byte 0x03bb
	.2byte 0x0000
	.2byte 0x03f9
	.2byte 0x0000
	.2byte 0x017b
	.2byte 0x0000
	.2byte 0x014d
	.2byte 0x0000
	.2byte 0x012b
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0183
	.2byte 0x0000
	.2byte 0x0362
	.2byte 0x100a
	.2byte 0x0000
	.2byte 0x02f7
	.2byte 0x0000
	.2byte 0x0169
	.2byte 0x0000
	.2byte 0x6001
	.2byte 0x0000
	.2byte 0x02e3
	.2byte 0x0000
	.2byte 0x0105
	.2byte 0x0000
	.2byte 0x2001
	.2byte 0x0000
	.2byte 0xd5b1
	.2byte 0x0200
	.2byte 0xd5d1
	.2byte 0x0200
	.2byte 0xd75d
	.2byte 0x0200
	.2byte 0x2666
	.2byte 0x0000
	.2byte 0x1333
	.2byte 0x0000
.L_020039cc_0:
	bl 0x0200df1c
	movs r1, #189
	movs r2, #146
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #9
	bl 0x0200e014
	movs r0, #20
	bl 0x0200dfb4
	ldr r0, [pc, #1016]
	movs r1, #0
	movs r2, #10
	bl 0x0200e09c
	mov r2, r9
	movs r1, #2
	str r2, [r6, #108]
	movs r0, #1
	bl 0x0200e0bc
	movs r0, #1
	bl 0x0200dfe4
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	mov r0, r10
	bl 0x0200de9c
	mov r0, r8
	bl 0x0200de9c
	movs r0, #1
	bl 0x0200de8c
	movs r0, #1
	movs r1, #0
	bl 0x0200e07c
	movs r0, #9
	movs r1, #0
	bl 0x0200e07c
	movs r1, #192
	movs r2, #0
	movs r0, #1
	lsls r1, r1, #6
	bl 0x0200e0ac
	movs r1, #1
	movs r0, #1
	bl 0x0200e044
	adds r0, r6, #0
	bl 0x0200d7fc
	bl 0x0200d5a4
	movs r0, #10
	bl 0x0200dfb4
	movs r1, #212
	movs r2, #156
	movs r0, #5
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200e024
	movs r2, #60
	movs r0, #1
	movs r1, #5
	bl 0x0200e074
	movs r1, #2
	movs r0, #1
	bl 0x0200e064
	movs r0, #10
	bl 0x0200dfb4
	movs r2, #20
	ldr r0, [pc, #872]
	movs r1, #0
	bl 0x0200e09c
	movs r1, #3
	movs r0, #5
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r0, #5
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #129
	movs r2, #40
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200e0c4
	movs r1, #4
	movs r0, #5
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r0, #5
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r2, #80
	movs r0, #1
	ldr r1, [pc, #804]
	bl 0x0200e0c4
	movs r1, #1
	movs r0, #5
	bl 0x0200e064
	movs r0, #10
	bl 0x0200dfb4
	movs r0, #5
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #129
	movs r2, #80
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200e0c4
	movs r1, #4
	movs r0, #5
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r2, #20
	movs r0, #5
	movs r1, #0
	bl 0x0200e09c
	movs r1, #3
	movs r0, #1
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r1, #194
	movs r2, #151
	movs r0, #5
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200e024
	movs r1, #160
	movs r2, #20
	movs r0, #5
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r1, #2
	movs r0, #5
	bl 0x0200e064
	movs r0, #10
	bl 0x0200dfb4
	movs r2, #20
	ldr r0, [pc, #688]
	movs r1, #0
	bl 0x0200e09c
	movs r1, #4
	movs r0, #5
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r2, #40
	movs r0, #1
	ldr r1, [pc, #664]
	bl 0x0200e0c4
	movs r1, #2
	movs r0, #1
	bl 0x0200e064
	movs r0, #10
	bl 0x0200dfb4
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl 0x0200e0ac
	mov r1, r11
	movs r0, #1
	movs r2, #40
	bl 0x0200e0ac
	movs r3, #14
	str r3, [sp, #12]
	str r3, [sp, #20]
	movs r1, #5
	mov r3, r9
	movs r0, #10
	movs r4, #4
	str r1, [sp, #4]
	str r0, [sp, #8]
	str r3, [sp, #24]
	movs r2, #2
	movs r3, #25
	movs r1, #1
	movs r0, #1
	str r2, [sp, #0]
	str r4, [sp, #16]
	bl 0x0200e0b4
	movs r0, #40
	bl 0x0200dfb4
	movs r1, #129
	movs r0, #5
	lsls r1, r1, #1
	bl 0x0200e0cc
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200e0cc
	movs r0, #80
	bl 0x0200dfb4
	movs r0, #5
	ldr r1, [pc, #548]
	movs r2, #40
	bl 0x0200e0c4
	movs r2, #20
	ldr r0, [pc, #540]
	movs r1, #0
	bl 0x0200e09c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200e0cc
	movs r0, #80
	bl 0x0200dfb4
	movs r1, #4
	movs r0, #1
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r0, #5
	ldr r1, [pc, #496]
	movs r2, #40
	bl 0x0200e0c4
	movs r2, #20
	ldr r0, [pc, #492]
	movs r1, #0
	bl 0x0200e09c
	movs r1, #2
	movs r0, #1
	bl 0x0200e064
	movs r0, #40
	bl 0x0200dfb4
	movs r1, #4
	movs r0, #1
	bl 0x0200e04c
	movs r0, #20
	bl 0x0200dfb4
	movs r2, #80
	movs r0, #5
	ldr r1, [pc, #448]
	bl 0x0200e0c4
	movs r1, #4
	movs r0, #5
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	ldr r0, [pc, #432]
	movs r1, #0
	movs r2, #10
	bl 0x0200e09c
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #5
	movs r2, #40
	bl 0x0200e0ac
	movs r2, #40
	movs r0, #5
	movs r1, #1
	bl 0x0200e06c
	movs r1, #2
	movs r0, #1
	bl 0x0200e064
	movs r0, #10
	bl 0x0200dfb4
	movs r1, #190
	movs r2, #155
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #5
	bl 0x0200e024
	movs r0, #10
	bl 0x0200dfb4
	ldr r0, [pc, #364]
	movs r1, #0
	movs r2, #10
	bl 0x0200e09c
	movs r1, #160
	movs r2, #30
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200e0ac
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200e0cc
	movs r0, #80
	bl 0x0200dfb4
	movs r1, #2
	movs r0, #1
	bl 0x0200e064
	movs r0, #10
	bl 0x0200dfb4
	movs r2, #20
	ldr r0, [pc, #304]
	movs r1, #0
	bl 0x0200e09c
	movs r1, #4
	movs r0, #5
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r2, #80
	movs r0, #1
	ldr r1, [pc, #280]
	bl 0x0200e0c4
	movs r1, #4
	movs r0, #5
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r0, #1
	ldr r1, [pc, #272]
	ldr r2, [pc, #272]
	bl 0x0200dfec
	movs r1, #206
	movs r2, #151
	movs r0, #1
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200e024
	movs r1, #160
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200e0ac
	movs r1, #1
	movs r0, #5
	bl 0x0200e064
	movs r0, #10
	bl 0x0200dfb4
	ldr r0, [pc, #216]
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r0, #1
	ldr r1, [pc, #200]
	movs r2, #80
	bl 0x0200e0c4
	movs r0, #5
	movs r1, #4
	movs r2, #30
	bl 0x0200e054
	movs r2, #20
	ldr r0, [pc, #184]
	movs r1, #0
	bl 0x0200e09c
	movs r1, #3
	movs r0, #1
	bl 0x0200e064
	movs r0, #10
	bl 0x0200dfb4
	movs r1, #0
	movs r2, #20
	ldr r0, [pc, #148]
	bl 0x0200e09c
	movs r0, #30
	bl 0x0200dfb4
	movs r1, #224
	movs r2, #40
	movs r0, #5
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r0, #5
	movs r1, #3
	bl 0x0200e04c
	ldr r0, [pc, #124]
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r0, #1
	ldr r1, [pc, #108]
	movs r2, #80
	bl 0x0200e0c4
	movs r0, #5
	ldr r1, [pc, #120]
	movs r2, #40
	bl 0x0200e0c4
	movs r0, #5
	ldr r1, [pc, #100]
	ldr r2, [pc, #104]
	bl 0x0200dfec
	movs r1, #214
	movs r2, #157
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #5
	bl 0x0200e01c
	movs r0, #20
	bl 0x0200dfb4
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #1
	bl 0x0200e0ac
	movs r0, #5
	bl 0x0200e034
	ldr r0, [pc, #68]
	movs r1, #0
	bl 0x0200e094
	movs r1, #1
	movs r0, #5
	bl 0x0200e044
	movs r0, #60
	bl 0x0200dfb4
	movs r1, #176
	movs r2, #30
	movs r0, #5
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r0, #1
	b .L_020039cc_1
	.4byte 0x00002005
	.4byte 0x00006001
	.4byte 0x00000101
	.4byte 0x00001005
	.4byte 0x00000105
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x00000103
	.4byte 0x00005001
.L_020039cc_1:
	movs r1, #2
	bl 0x0200e064
	movs r1, #214
	movs r2, #157
	movs r0, #5
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200e024
	movs r1, #176
	movs r2, #20
	movs r0, #5
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r1, #3
	movs r0, #1
	bl 0x0200e064
	movs r0, #10
	bl 0x0200dfb4
	movs r1, #4
	movs r0, #5
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r2, #20
	ldr r0, [pc, #160]
	movs r1, #0
	bl 0x0200e09c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200e0cc
	movs r0, #40
	bl 0x0200dfb4
	movs r2, #20
	ldr r0, [pc, #136]
	movs r1, #0
	bl 0x0200e09c
	movs r0, #1
	movs r1, #3
	bl 0x0200e044
	movs r1, #3
	movs r0, #5
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r2, #128
	mov r1, r11
	movs r0, #5
	lsls r2, r2, #7
	bl 0x0200dfec
	movs r2, #128
	mov r1, r11
	movs r0, #1
	lsls r2, r2, #7
	bl 0x0200dfec
	movs r1, #225
	movs r0, #5
	lsls r1, r1, #1
	ldr r2, [pc, #80]
	bl 0x0200e01c
	movs r1, #225
	lsls r1, r1, #1
	ldr r2, [pc, #72]
	movs r0, #1
	bl 0x0200e01c
	movs r0, #60
	bl 0x0200dfb4
	ldr r3, [pc, #60]
	movs r1, #228
	ldr r3, [r3]
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #60
	str r2, [r3]
	bl 0x0200e10c
	bl 0x0200e114
	movs r0, #12
	bl 0x0200e0fc
	bl 0x0200dfc4
	sub sp, #-28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00002005
	.4byte 0x00005001
	.4byte 0x000002ee
	.4byte 0x03001ebc
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
	.global gHaidiaMuraActor22Actions
gHaidiaMuraActor22Actions:
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00004000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000002
	.4byte 0x80010000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01a40000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000f
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00d40000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00d40000
	.4byte 0x00000000
	.4byte 0x01840000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00d40000
	.4byte 0x00000000
	.4byte 0x01ad0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00d40000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.global gLeaderHammerAction
gLeaderHammerAction:
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x000007ae
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000010
	.global gGeraldAction
gGeraldAction:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x03700000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x03880000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000010
	.global gJasmineAction
gJasmineAction:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x03880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01410000
	.4byte 0x00000000
	.4byte 0x03880000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000010
	.global gVillagerAction
gVillagerAction:
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x010a0000
	.4byte 0x00000000
	.4byte 0x04ce0000
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
	.4byte 0x00000f5c
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x0000147a
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000018
	.4byte 0xc0010000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global gDustBurstScript
gDustBurstScript:
	.4byte 0x0000001b
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00001000
	.4byte 0x0000001c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x0000001b
	.global gHaidiaMuraEntrances
gHaidiaMuraEntrances:
	.4byte 0xffff0000
	.4byte 0x000000a7
	.4byte 0x40000501
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000100
	.4byte 0x400001b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000071
	.4byte 0x4000012f
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000026
	.4byte 0x0000027c
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x0000001d
	.4byte 0x00000318
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x000001ca
	.4byte 0x80000571
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x00000196
	.4byte 0x400002e7
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0007
	.4byte 0x00000106
	.4byte 0x40000335
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x00000154
	.4byte 0x40000388
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0009
	.4byte 0x00000146
	.4byte 0x40000476
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x00000176
	.4byte 0x400004e6
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x00000066
	.4byte 0x400004c6
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0010
	.4byte 0x00000190
	.4byte 0xc0000354
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0011
	.4byte 0x00000190
	.4byte 0xc0000354
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gHaidiaMuraExits
gHaidiaMuraExits:
	.4byte 0x00000004
	.4byte 0x00101013
	.4byte 0x00208005
	.4byte 0x00301006
	.4byte 0x00402006
	.4byte 0x00506007
	.4byte 0x00605007
	.4byte 0x00706008
	.4byte 0x00802007
	.4byte 0x00901007
	.4byte 0x00a02007
	.4byte 0x00b01009
	.4byte 0x00c11004
	.4byte 0x000001ff
	.global gHaidiaMuraPlacements
gHaidiaMuraPlacements:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x038c0000
	.4byte 0x0001c000
	.4byte 0xffff0073
	.4byte 0x00000003
	.4byte 0x005c0000
	.4byte 0x00000000
	.4byte 0x01fc0000
	.4byte 0x00004000
	.4byte 0xffff0066
	.4byte 0x00000001
	.4byte 0x00fc0000
	.4byte 0x00000000
	.4byte 0x026c0000
	.4byte 0x00008000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00a40000
	.4byte 0x00000000
	.4byte 0x04d40000
	.4byte 0x0001c000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x010c0000
	.4byte 0x00000000
	.4byte 0x04fc0000
	.4byte 0x00018000
	.4byte 0xffff0065
	.4byte 0x00000001
	.4byte 0x01140000
	.4byte 0x00000000
	.4byte 0x04140000
	.4byte 0x0000c000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00130000
	.4byte 0x00000000
	.4byte 0x02660000
	.4byte 0x00000000
	.4byte 0xffff0068
	.4byte 0x00000004
	.4byte 0x00320000
	.4byte 0x00000000
	.4byte 0x031b0000
	.4byte 0x00008000
	.4byte 0xffff0068
	.4byte 0x00000004
	.4byte 0x00410000
	.4byte 0x00000000
	.4byte 0x03200000
	.4byte 0x00008000
	.4byte 0xffff0068
	.4byte 0x00000004
	.4byte 0x00390000
	.4byte 0x00000000
	.4byte 0x03250000
	.4byte 0x00008000
	.4byte 0xffff002a
	.4byte 0x00000001
	.4byte 0x019c0000
	.4byte 0x00000000
	.4byte 0x024a0000
	.4byte 0x00018000
	.4byte 0xffff0025
	.4byte 0x00000001
	.4byte 0x018e0000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x0001c000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x017a0000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00020000
	.4byte 0xffff001f
	.4byte 0x00000001
	.4byte 0x01750000
	.4byte 0x00000000
	.4byte 0x03790000
	.4byte 0x00004000
	.4byte 0xffff007a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00ce
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00ce
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00ce
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0fd00016
	.4byte 0x00000007
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x03f80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gHaidiaMuraPlacements2
gHaidiaMuraPlacements2:
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x038c0000
	.4byte 0x0001c000
	.4byte 0xffff0073
	.4byte 0x00000003
	.4byte 0x005c0000
	.4byte 0x00000000
	.4byte 0x01fc0000
	.4byte 0x00004000
	.4byte 0xffff0066
	.4byte 0x00000001
	.4byte 0x00fc0000
	.4byte 0x00000000
	.4byte 0x026c0000
	.4byte 0x00008000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00a40000
	.4byte 0x00000000
	.4byte 0x04d40000
	.4byte 0x0001c000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x010c0000
	.4byte 0x00000000
	.4byte 0x04fc0000
	.4byte 0x00018000
	.4byte 0xffff0065
	.4byte 0x00000001
	.4byte 0x01140000
	.4byte 0x00000000
	.4byte 0x04140000
	.4byte 0x0000c000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00130000
	.4byte 0x00000000
	.4byte 0x02660000
	.4byte 0x00000000
	.4byte 0xffff0068
	.4byte 0x00000004
	.4byte 0x00320000
	.4byte 0x00000000
	.4byte 0x031b0000
	.4byte 0x00008000
	.4byte 0xffff0068
	.4byte 0x00000004
	.4byte 0x00410000
	.4byte 0x00000000
	.4byte 0x03200000
	.4byte 0x00008000
	.4byte 0xffff0068
	.4byte 0x00000004
	.4byte 0x00390000
	.4byte 0x00000000
	.4byte 0x03250000
	.4byte 0x00008000
	.4byte 0xffff002a
	.4byte 0x00000001
	.4byte 0x019c0000
	.4byte 0x00000000
	.4byte 0x024a0000
	.4byte 0x00038000
	.4byte 0xffff0025
	.4byte 0x00000001
	.4byte 0x018e0000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x0003c000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x017a0000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00020000
	.4byte 0xffff001f
	.4byte 0x00000001
	.4byte 0x014f0000
	.4byte 0x00000000
	.4byte 0x03650000
	.4byte 0x00004000
	.4byte 0xffff007a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00ce
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00ce
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00ce
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0fd00016
	.4byte 0x00000007
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x03f80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gHaidiaMuraPlacements3
gHaidiaMuraPlacements3:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff002a
	.4byte 0x00000001
	.4byte 0x019c0000
	.4byte 0x00000000
	.4byte 0x024a0000
	.4byte 0x0003d000
	.4byte 0xffff0025
	.4byte 0x00000001
	.4byte 0x01640000
	.4byte 0x00000000
	.4byte 0x02d70000
	.4byte 0x0000b000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00a40000
	.4byte 0x00000000
	.4byte 0x04d40000
	.4byte 0x0001c000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x010c0000
	.4byte 0x00000000
	.4byte 0x04fc0000
	.4byte 0x00028000
	.4byte 0xffff00d6
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00d6
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00d6
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x038c0000
	.4byte 0x0001c000
	.4byte 0xffff0073
	.4byte 0x00000003
	.4byte 0x005c0000
	.4byte 0x00000000
	.4byte 0x01fc0000
	.4byte 0x00024000
	.4byte 0xffff0066
	.4byte 0x00000001
	.4byte 0x00fc0000
	.4byte 0x00000000
	.4byte 0x026c0000
	.4byte 0x00018000
	.4byte 0xffff0065
	.4byte 0x00000001
	.4byte 0x01140000
	.4byte 0x00000000
	.4byte 0x04140000
	.4byte 0x0002c000
	.4byte 0x0fd00016
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x03f80000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x017a0000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gHaidiaMuraPlacements4
gHaidiaMuraPlacements4:
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x01990000
	.4byte 0x00000000
	.4byte 0x046e0000
	.4byte 0x0000c000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x01ac0000
	.4byte 0x00000000
	.4byte 0x024c0000
	.4byte 0x00008000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00cc0000
	.4byte 0x00000000
	.4byte 0x039c0000
	.4byte 0x00000000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x018a0000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00024000
	.4byte 0xffff001f
	.4byte 0x00000001
	.4byte 0x01750000
	.4byte 0x00000000
	.4byte 0x03790000
	.4byte 0x0000c000
	.4byte 0xffff00ce
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00ce
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00ce
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00d7
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gHaidiaMuraEvents
gHaidiaMuraEvents:
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00000f7f
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00000f80
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008add
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00000f79
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00000f7a
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00000f84
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02008b29
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x02008ba9
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00000f67
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x02008c61
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00000fd1
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008dc1
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02008df1
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x02008e19
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x02008e2d
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x02008e41
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008e55
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008e85
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x02008eb5
	.4byte 0x0000c602
	.4byte 0xffff0009
	.4byte 0x02008f5d
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte 0x02008f8d
	.4byte 0x0000c602
	.4byte 0xffff000b
	.4byte 0x02008fbd
	.4byte 0x00000002
	.4byte 0xffff000f
	.4byte 0x02008d71
	.4byte 0x00000002
	.4byte 0xffff000e
	.4byte 0x02008d99
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte 0x020090d9
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte 0x020091d9
	.4byte 0x00000002
	.4byte 0x08230032
	.4byte 0x020092bd
	.4byte 0x00000002
	.4byte 0xffff0033
	.4byte 0x02009455
	.4byte 0x00000002
	.4byte 0xffff0034
	.4byte 0x02009555
	.4byte 0x00000002
	.4byte 0xffff0035
	.4byte 0x02009591
	.4byte 0x0000c602
	.4byte 0xffff0064
	.4byte 0x02009245
	.4byte 0x00004602
	.4byte 0xffff0065
	.4byte 0x02009281
	.4byte 0x00009415
	.4byte 0x0fd0001a
	.4byte 0x02008a45
	.4byte 0x00008c15
	.4byte 0xffff0014
	.4byte 0x0200d951
	.4byte 0x00000602
	.4byte 0xffff001e
	.4byte 0x0200d929
	.4byte 0x00008602
	.4byte 0xffff001f
	.4byte 0x0200d929
	.4byte 0x00000003
	.4byte 0xffff0018
	.4byte 0x0200da25
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gHaidiaMuraEvents2
gHaidiaMuraEvents2:
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000011ca
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000011cb
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008add
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000011c5
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000011c6
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000011cd
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02008b29
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x000011b4
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x000011b5
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x0000111d
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x0000111e
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000011e7
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x000011e8
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000011f2
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000011f3
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000011f5
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000011f6
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000011f7
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000011f8
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000011f9
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008dc1
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02008df1
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x02008e19
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x02008e2d
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x02008e41
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008e55
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008e85
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x02008eb5
	.4byte 0x0000c602
	.4byte 0xffff0009
	.4byte 0x02008f5d
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte 0x02008f8d
	.4byte 0x0000c602
	.4byte 0xffff000b
	.4byte 0x02008fbd
	.4byte 0x00000002
	.4byte 0xffff000f
	.4byte 0x02008d71
	.4byte 0x00000002
	.4byte 0xffff000e
	.4byte 0x02008d99
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte 0x020090d9
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte 0x020091d9
	.4byte 0x00000002
	.4byte 0x08230032
	.4byte 0x020092bd
	.4byte 0x00000002
	.4byte 0xffff0033
	.4byte 0x02009455
	.4byte 0x00000002
	.4byte 0xffff0034
	.4byte 0x02009555
	.4byte 0x00000002
	.4byte 0xffff0035
	.4byte 0x02009591
	.4byte 0x0000c602
	.4byte 0xffff0064
	.4byte 0x02009245
	.4byte 0x00004602
	.4byte 0xffff0065
	.4byte 0x02009281
	.4byte 0x00009415
	.4byte 0x0fd0001a
	.4byte 0x02008a45
	.4byte 0x00008c15
	.4byte 0xffff0014
	.4byte 0x0200d951
	.4byte 0x00000602
	.4byte 0xffff001e
	.4byte 0x0200d929
	.4byte 0x00008602
	.4byte 0xffff001f
	.4byte 0x0200d929
	.4byte 0x00000003
	.4byte 0xffff0018
	.4byte 0x0200da25
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gHaidiaMuraEvents3
gHaidiaMuraEvents3:
	.4byte 0x00000000
	.4byte 0x03030008
	.4byte 0x0200acb1
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001c62
	.4byte 0x00000000
	.4byte 0x03040009
	.4byte 0x0200af15
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001c63
	.4byte 0x00000000
	.4byte 0x081f000a
	.4byte 0x02008c9d
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001c8e
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001c90
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001c99
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x02008cd1
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001c9c
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x02008d2d
	.4byte 0x00008d15
	.4byte 0x03030408
	.4byte 0x0200acb1
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001c64
	.4byte 0x00008d15
	.4byte 0x03040409
	.4byte 0x0200af15
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001c65
	.4byte 0x00008d15
	.4byte 0x081f040a
	.4byte 0x02008c9d
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001c8f
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001c91
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001ca7
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001ca8
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001ca9
	.4byte 0x00008d15
	.4byte 0x03070413
	.4byte 0x02008d2d
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001caa
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008dc1
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02008df1
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x02008e19
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x02008e2d
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x02008e41
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008e55
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008e85
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x02008eb5
	.4byte 0x0000c602
	.4byte 0xffff0009
	.4byte 0x02008f5d
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte 0x02008f8d
	.4byte 0x0000c602
	.4byte 0xffff000b
	.4byte 0x02008fbd
	.4byte 0x00000002
	.4byte 0xffff000f
	.4byte 0x02008d71
	.4byte 0x00000002
	.4byte 0xffff000e
	.4byte 0x02008d99
	.4byte 0x0000c602
	.4byte 0xffff0064
	.4byte 0x02009245
	.4byte 0x00004602
	.4byte 0xffff0065
	.4byte 0x02009281
	.4byte 0x00009415
	.4byte 0x0fd00014
	.4byte 0x02008a75
	.4byte 0x00008c15
	.4byte 0xffff0015
	.4byte 0x0200d951
	.4byte 0x00000602
	.4byte 0xffff001e
	.4byte 0x0200d929
	.4byte 0x00008602
	.4byte 0xffff001f
	.4byte 0x0200d929
	.4byte 0x00000003
	.4byte 0xffff0018
	.4byte 0x0200da25
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gHaidiaMuraCellAnimC
gHaidiaMuraCellAnimC:
	.4byte 0x00620000
	.4byte 0x00020002
	.4byte 0x00020002
	.4byte 0x00020062
	.4byte 0x00020002
	.2byte 0xffff
	.global gHaidiaMuraCellAnimA
gHaidiaMuraCellAnimA:
	.2byte 0x0000
	.4byte 0x00020060
	.4byte 0x00020002
	.4byte 0x00600002
	.4byte 0x00020002
	.4byte 0xffff0002
	.global gHaidiaMuraCellAnimB
gHaidiaMuraCellAnimB:
	.4byte 0x00620004
	.4byte 0x00020002
	.4byte 0x00040002
	.4byte 0x00020062
	.4byte 0x00020002
	.2byte 0xffff
	.global gHaidiaMuraCellAnimD
gHaidiaMuraCellAnimD:
	.2byte 0x0004
	.4byte 0x00020060
	.4byte 0x00020002
	.4byte 0x00600004
	.4byte 0x00020002
	.4byte 0xffff0002
	.global gHaidiaMuraLeaderWalkActions
gHaidiaMuraLeaderWalkActions:
	.4byte 0x0000001c
	.4byte 0x00000005
	.4byte 0x00000003
	.4byte 0x00c70000
	.4byte 0x00000000
	.4byte 0x01be0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00d70000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00ef0000
	.4byte 0x00000000
	.4byte 0x01cd0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global gHaidiaMuraActor22WalkActions
gHaidiaMuraActor22WalkActions:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00c70000
	.4byte 0x00000000
	.4byte 0x01be0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00d70000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00ef0000
	.4byte 0x00000000
	.4byte 0x01cd0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global gHaidiaMuraPairTableA
gHaidiaMuraPairTableA:
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00020000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00020000
	.4byte 0x0000001c
	.4byte 0x00000005
	.4byte 0x00000003
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x01ca0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01ca0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01da0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000010
	.global gHaidiaMuraPairTableB
gHaidiaMuraPairTableB:
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00010000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01ca0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000010
	.global gHaidiaMuraPairTableC
gHaidiaMuraPairTableC:
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00020000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00020000
	.4byte 0x0000001c
	.4byte 0x00000005
	.4byte 0x00000003
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01ca0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01da0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000010
	.global gHaidiaMuraPairTableD
gHaidiaMuraPairTableD:
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00010000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000010
