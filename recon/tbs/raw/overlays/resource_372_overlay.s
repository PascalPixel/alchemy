.syntax unified
	.thumb
	.section .text.x0200a180,"ax",%progbits
	.balign 4
	.global Func_02002180
	.thumb_func
Func_02002180:
	.global FieldScene_RunFlagGatedActorSequence
	.thumb_func
FieldScene_RunFlagGatedActorSequence:
	push {r5, r6, lr}
	ldr r0, [pc, #1012]
	bl 0x0200c6dc
	cmp r0, #0
	beq .L_02002180_0
	b 0x0200a87e
.L_02002180_0:
	bl 0x0200c6fc
	movs r1, #192
	movs r0, #10
	lsls r1, r1, #16
	ldr r2, [pc, #992]
	bl 0x0200c78c
	movs r1, #128
	movs r2, #0
	movs r0, #10
	lsls r1, r1, #6
	bl 0x0200c7fc
	movs r1, #5
	movs r0, #10
	bl 0x0200c794
	movs r0, #10
	bl 0x0200c72c
	adds r5, r0, #0
	bl 0x0200c654
	movs r1, #90
	bl 0x0200c634
	ldr r6, [pc, #952]
	adds r0, #60
	adds r5, #100
	strh r0, [r5]
	adds r1, r6, #0
	movs r0, #10
	bl 0x0200c744
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #16
	ldr r2, [pc, #936]
	bl 0x0200c78c
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #227
	movs r0, #24
	lsls r1, r1, #16
	ldr r2, [pc, #904]
	bl 0x0200c78c
	movs r1, #128
	movs r2, #0
	movs r0, #24
	lsls r1, r1, #7
	bl 0x0200c7fc
	movs r1, #6
	movs r0, #24
	bl 0x0200c794
	movs r0, #24
	bl 0x0200c72c
	adds r5, r0, #0
	bl 0x0200c654
	movs r1, #90
	bl 0x0200c634
	adds r5, #100
	adds r0, #60
	strh r0, [r5]
	adds r1, r6, #0
	movs r0, #24
	bl 0x0200c744
	movs r1, #250
	movs r0, #25
	lsls r1, r1, #16
	ldr r2, [pc, #840]
	bl 0x0200c78c
	movs r1, #128
	movs r2, #0
	movs r0, #25
	lsls r1, r1, #7
	bl 0x0200c7fc
	movs r1, #6
	movs r0, #25
	bl 0x0200c794
	movs r0, #25
	bl 0x0200c72c
	adds r5, r0, #0
	bl 0x0200c654
	movs r1, #90
	bl 0x0200c634
	adds r5, #100
	adds r0, #60
	strh r0, [r5]
	adds r1, r6, #0
	movs r0, #25
	bl 0x0200c744
	movs r1, #227
	movs r0, #26
	lsls r1, r1, #16
	ldr r2, [pc, #784]
	bl 0x0200c78c
	movs r1, #192
	movs r0, #26
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #243
	movs r0, #23
	lsls r1, r1, #16
	ldr r2, [pc, #764]
	bl 0x0200c78c
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #23
	bl 0x0200c7fc
	movs r0, #23
	bl 0x0200c72c
	movs r1, #0
	bl 0x0200c6b4
	movs r0, #3
	bl 0x0200c63c
	ldr r0, [pc, #732]
	bl 0x0200c7d4
	ldr r0, [pc, #728]
	movs r1, #0
	bl 0x0200c7e4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #20
	bl 0x0200c80c
	movs r0, #0
	movs r1, #150
	ldr r2, [pc, #708]
	bl 0x0200c77c
	movs r0, #0
	bl 0x0200c72c
	cmp r0, #0
	beq .L_02002180_1
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #22
	bl 0x0200c78c
.L_02002180_1:
	movs r0, #22
	movs r1, #132
	ldr r2, [pc, #680]
	bl 0x0200c77c
	movs r1, #22
	movs r2, #0
	movs r0, #0
	bl 0x0200c7c4
	movs r0, #40
	bl 0x0200c6f4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #128
	movs r2, #20
	movs r0, #22
	lsls r1, r1, #7
	bl 0x0200c7fc
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl 0x0200c81c
	movs r0, #216
	movs r1, #1
	movs r2, #154
	movs r3, #1
	lsls r2, r2, #19
	negs r1, r1
	lsls r0, r0, #16
	bl 0x0200c824
	bl 0x0200c82c
	movs r0, #40
	bl 0x0200c6f4
	movs r0, #10
	movs r1, #2
	bl 0x0200c7b4
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl 0x0200c7ec
	movs r0, #23
	movs r1, #3
	bl 0x0200c7b4
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl 0x0200c7fc
	movs r0, #9
	movs r1, #3
	bl 0x0200c79c
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl 0x0200c7ec
	movs r1, #192
	movs r2, #10
	movs r0, #9
	lsls r1, r1, #6
	bl 0x0200c7fc
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200c81c
	movs r0, #232
	movs r1, #1
	movs r3, #1
	negs r1, r1
	ldr r2, [pc, #512]
	lsls r0, r0, #16
	bl 0x0200c824
	bl 0x0200c82c
	movs r0, #20
	bl 0x0200c6f4
	movs r0, #134
	bl 0x0200c8b4
	movs r2, #0
	movs r0, #23
	movs r1, #4
	bl 0x0200c7a4
	movs r1, #6
	movs r0, #23
	bl 0x0200c794
	movs r0, #10
	bl 0x0200c6f4
	movs r2, #0
	movs r1, #0
	movs r0, #23
	bl 0x0200c78c
	movs r0, #60
	bl 0x0200c6f4
	bl 0x0200c8ac
	movs r1, #1
	movs r0, #10
	bl 0x0200c794
	movs r0, #10
	bl 0x0200c72c
	movs r5, #128
	lsls r5, r5, #9
	str r5, [r0, #24]
	str r5, [r0, #28]
	movs r1, #1
	movs r0, #24
	bl 0x0200c794
	movs r0, #24
	bl 0x0200c72c
	movs r1, #1
	str r5, [r0, #24]
	str r5, [r0, #28]
	movs r0, #25
	bl 0x0200c794
	movs r0, #25
	bl 0x0200c72c
	movs r1, #2
	str r5, [r0, #24]
	str r5, [r0, #28]
	movs r0, #10
	bl 0x0200c7ac
	movs r0, #9
	movs r1, #2
	bl 0x0200c7ac
	movs r0, #24
	movs r1, #2
	bl 0x0200c7ac
	movs r0, #25
	movs r1, #2
	bl 0x0200c7ac
	movs r0, #26
	movs r1, #2
	bl 0x0200c7b4
	ldr r0, [pc, #348]
	ldr r1, [pc, #352]
	bl 0x0200c81c
	movs r0, #216
	movs r1, #1
	movs r2, #154
	movs r3, #1
	lsls r2, r2, #19
	lsls r0, r0, #16
	negs r1, r1
	bl 0x0200c824
	bl 0x0200c82c
	movs r1, #129
	movs r0, #26
	lsls r1, r1, #1
	bl 0x0200c814
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #9
	bl 0x0200c814
	movs r0, #60
	bl 0x0200c6f4
	movs r0, #26
	movs r1, #2
	bl 0x0200c7b4
	movs r0, #26
	movs r1, #3
	bl 0x0200c7ac
	movs r0, #26
	movs r1, #0
	bl 0x0200c7e4
	movs r0, #25
	movs r1, #2
	movs r2, #0
	bl 0x0200c7a4
	movs r0, #25
	movs r1, #234
	ldr r2, [pc, #264]
	bl 0x0200c764
	movs r0, #26
	movs r1, #2
	movs r2, #0
	bl 0x0200c7a4
	movs r1, #227
	ldr r2, [pc, #248]
	movs r0, #26
	bl 0x0200c764
	movs r0, #90
	bl 0x0200c6f4
	movs r0, #232
	movs r1, #1
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	ldr r2, [pc, #208]
	bl 0x0200c824
	bl 0x0200c82c
	movs r1, #243
	ldr r2, [pc, #180]
	lsls r1, r1, #16
	movs r0, #23
	bl 0x0200c78c
	movs r0, #1
	bl 0x0200c63c
	movs r0, #106
	bl 0x0200c8b4
	movs r0, #23
	bl 0x0200c72c
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r0, #40]
	movs r0, #6
	bl 0x0200c6f4
	movs r1, #7
	movs r0, #23
	bl 0x0200c794
	movs r0, #20
	bl 0x0200c6f4
	bl 0x0200c8ac
	movs r0, #20
	bl 0x0200c6f4
	ldr r0, [pc, #152]
	ldr r1, [pc, #156]
	bl 0x0200c81c
	movs r0, #216
	movs r1, #1
	movs r2, #154
	movs r3, #1
	lsls r2, r2, #19
	lsls r0, r0, #16
	negs r1, r1
	bl 0x0200c824
	bl 0x0200c82c
	movs r1, #2
	movs r0, #24
	bl 0x0200c7b4
	movs r0, #20
.L_02002538:
	bl 0x0200c6f4
	movs r0, #24
	ldr r1, [pc, #116]
	movs r2, #40
	bl 0x0200c80c
	movs r2, #0
	movs r1, #10
	movs r0, #24
	bl 0x0200c7c4
	movs r0, #10
	bl 0x0200c6f4
	movs r0, #10
	movs r1, #2
	bl 0x0200c7b4
	movs r1, #0
	ldr r0, [pc, #84]
	bl 0x0200c7e4
	movs r0, #25
	bl 0x0200c72c
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	b .L_02002538_0
	.2byte 0x083a
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x04be
	.2byte 0xcec8
	.2byte 0x0200
	.2byte 0x0000
	.2byte 0x04a5
	.2byte 0x0000
	.2byte 0x04fd
	.2byte 0x0e8c
	.2byte 0x0000
	.2byte 0x201a
	.2byte 0x0000
	.2byte 0x0446
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x04e5
	.2byte 0x9999
	.2byte 0x0000
	.2byte 0x1333
	.2byte 0x0000
	.2byte 0x04b5
	.2byte 0x0000
	.2byte 0x04b1
	.2byte 0x0000
	.2byte 0x9999
	.2byte 0x0001
	.2byte 0x3333
	.2byte 0x0000
	.4byte 0x00000105
	.4byte 0x0000800a
.L_02002538_0:
	strb r3, [r0]
	movs r0, #26
	bl 0x0200c72c
	adds r0, #90
	ldrb r3, [r0]
	ands r5, r3
	strb r5, [r0]
	ldr r1, [pc, #692]
	movs r0, #25
	ldr r2, [pc, #692]
	bl 0x0200c73c
	movs r0, #26
	ldr r1, [pc, #680]
	ldr r2, [pc, #684]
	bl 0x0200c73c
	movs r0, #25
	movs r1, #247
	ldr r2, [pc, #676]
	bl 0x0200c764
	movs r1, #227
	ldr r2, [pc, #672]
	movs r0, #26
	bl 0x0200c76c
	movs r0, #25
	bl 0x0200c72c
	adds r0, #90
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #26
	bl 0x0200c72c
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #192
	orrs r5, r3
	strb r5, [r0]
	lsls r1, r1, #7
	movs r0, #26
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #128
	movs r2, #10
	movs r0, #25
	lsls r1, r1, #8
	bl 0x0200c7fc
	movs r0, #24
	movs r1, #4
	bl 0x0200c794
	ldr r0, [pc, #608]
	movs r1, #0
	movs r2, #10
	bl 0x0200c7ec
	movs r1, #192
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200c7fc
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl 0x0200c7fc
	movs r0, #10
	movs r1, #4
	bl 0x0200c794
	ldr r0, [pc, #572]
	movs r1, #0
	movs r2, #10
	bl 0x0200c7ec
	movs r0, #24
	ldr r1, [pc, #564]
	movs r2, #0
	bl 0x0200c80c
	movs r0, #10
	ldr r1, [pc, #552]
	movs r2, #60
	bl 0x0200c80c
	movs r1, #131
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #20
	bl 0x0200c80c
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200c7fc
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200c7fc
	movs r0, #9
	movs r1, #0
	movs r2, #30
	bl 0x0200c7fc
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #10
	bl 0x0200c7fc
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl 0x0200c7ec
	movs r1, #192
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #144
	movs r0, #25
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #160
	movs r0, #24
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #128
	movs r2, #10
	movs r0, #26
	lsls r1, r1, #8
	bl 0x0200c7fc
	movs r0, #10
	movs r1, #1
	bl 0x0200c7b4
	movs r2, #10
	ldr r0, [pc, #416]
	movs r1, #0
	bl 0x0200c7ec
	movs r0, #9
	movs r1, #4
	bl 0x0200c794
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl 0x0200c7ec
	movs r0, #10
	ldr r1, [pc, #392]
	movs r2, #0
	bl 0x0200c80c
	movs r0, #24
	ldr r1, [pc, #380]
	movs r2, #0
	bl 0x0200c80c
	movs r0, #25
	ldr r1, [pc, #372]
	movs r2, #0
	bl 0x0200c80c
	movs r0, #26
	ldr r1, [pc, #360]
	movs r2, #40
	bl 0x0200c80c
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl 0x0200c7fc
	movs r1, #25
	movs r2, #0
	movs r0, #24
	bl 0x0200c7c4
	movs r0, #20
	bl 0x0200c6f4
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200c7fc
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl 0x0200c7fc
	movs r1, #128
	movs r0, #24
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #128
	movs r2, #10
	movs r0, #25
	lsls r1, r1, #8
	bl 0x0200c7fc
	movs r0, #24
	movs r1, #3
	bl 0x0200c794
	movs r0, #25
	movs r1, #3
	bl 0x0200c79c
	movs r2, #0
	movs r1, #9
	movs r0, #10
	bl 0x0200c7c4
	movs r0, #20
	bl 0x0200c6f4
	movs r0, #10
	movs r1, #1
	bl 0x0200c7b4
	movs r2, #10
	ldr r0, [pc, #236]
	movs r1, #0
	bl 0x0200c7ec
	movs r0, #9
	movs r1, #3
	bl 0x0200c79c
	movs r1, #208
	movs r2, #10
	movs r0, #24
	lsls r1, r1, #8
	bl 0x0200c7fc
	movs r0, #24
	movs r1, #1
	bl 0x0200c7ac
	movs r0, #24
	movs r1, #0
	movs r2, #10
	bl 0x0200c7ec
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x0200c7fc
	movs r2, #0
	movs r0, #9
	movs r1, #0
	bl 0x0200c7fc
	movs r0, #26
	movs r1, #1
	bl 0x0200c7b4
	movs r1, #128
	movs r0, #26
	lsls r1, r1, #6
	movs r2, #20
	bl 0x0200c7fc
	movs r1, #160
	movs r2, #20
	movs r0, #25
	lsls r1, r1, #8
	bl 0x0200c7fc
	movs r0, #25
	movs r1, #3
	bl 0x0200c79c
	movs r2, #10
	movs r0, #25
	movs r1, #0
	bl 0x0200c7ec
	movs r1, #3
	movs r0, #26
	bl 0x0200c79c
	movs r0, #20
	bl 0x0200c6f4
	movs r0, #9
	movs r1, #2
	bl 0x0200c7b4
	movs r0, #9
	movs r1, #3
	bl 0x0200c79c
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl 0x0200c7ec
	movs r1, #128
	movs r2, #10
	movs r0, #26
	lsls r1, r1, #8
	bl 0x0200c7fc
	movs r1, #3
	movs r0, #26
	bl 0x0200c79c
	movs r0, #20
	bl 0x0200c6f4
	movs r0, #9
	movs r1, #1
	bl 0x0200c7b4
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl 0x0200c7ec
	bl 0x0200a8a4
	ldr r0, [pc, #40]
	bl 0x0200c6e4
	bl 0x0200c704
	pop {r5, r6}
	pop {r0}
.L_02002882:
	bx r0
	.2byte 0x9999
	.2byte 0x0000
	.2byte 0x4ccc
	.2byte 0x0000
	.2byte 0x04ba
	.2byte 0x0000
	.2byte 0x04a5
	.2byte 0x0000
	.2byte 0x8018
	.2byte 0x0000
	.2byte 0x800a
	.2byte 0x0000
	.2byte 0x0105
	.2byte 0x0000
	.2byte 0x083a
	.2byte 0x0000
	.global Func_020028a4
	.thumb_func
Func_020028a4:
	.global RunEventScript01
	.thumb_func
RunEventScript01:
	push {r5, lr}
	movs r1, #192
	movs r0, #26
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #208
	movs r0, #24
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #176
	movs r0, #25
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #208
.L_020028d8:
	movs r2, #20
	movs r0, #10
	lsls r1, r1, #8
	bl 0x0200c7fc
	movs r0, #26
	movs r1, #3
	bl 0x0200c794
	movs r0, #24
.L_020028ec:
	movs r1, #3
	bl 0x0200c794
	movs r0, #25
	movs r1, #3
	bl 0x0200c794
	movs r0, #9
	movs r1, #3
	bl 0x0200c794
	movs r1, #3
	movs r0, #25
	bl 0x0200c79c
	movs r0, #20
	bl 0x0200c6f4
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200c81c
	movs r0, #134
	movs r1, #1
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	ldr r2, [pc, #920]
	bl 0x0200c824
	movs r0, #26
	ldr r1, [pc, #916]
	ldr r2, [pc, #916]
	bl 0x0200c73c
	ldr r2, [pc, #912]
	movs r0, #9
	ldr r1, [pc, #904]
	bl 0x0200c73c
	ldr r1, [pc, #904]
	movs r0, #26
	bl 0x0200c744
	ldr r1, [pc, #900]
	movs r0, #9
	bl 0x0200c75c
	movs r0, #158
	bl 0x0200c8b4
	movs r1, #38
	movs r2, #72
	ldr r0, [pc, #888]
	bl 0x0200c68c
	movs r0, #10
	bl 0x0200c6f4
	movs r0, #9
	movs r1, #149
	ldr r2, [pc, #876]
	bl 0x0200c77c
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200c78c
	movs r0, #25
	movs r1, #250
	ldr r2, [pc, #860]
	bl 0x0200c77c
	bl 0x0200c8ac
	movs r1, #192
	movs r0, #10
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #192
	movs r0, #24
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #192
	movs r2, #0
	movs r0, #25
	lsls r1, r1, #6
	bl 0x0200c7fc
	movs r0, #10
	movs r1, #5
	bl 0x0200c794
	movs r0, #24
	movs r1, #6
	bl 0x0200c794
	movs r1, #6
	movs r0, #25
	bl 0x0200c794
	movs r0, #10
	bl 0x0200c72c
	adds r5, r0, #0
	bl 0x0200c654
	movs r1, #90
	bl 0x0200c634
	adds r5, #100
	adds r0, #60
	strh r0, [r5]
	movs r0, #24
	bl 0x0200c72c
	adds r5, r0, #0
	bl 0x0200c654
	movs r1, #90
	bl 0x0200c634
	adds r5, #100
	adds r0, #60
	strh r0, [r5]
	movs r0, #25
	bl 0x0200c72c
	adds r5, r0, #0
	bl 0x0200c654
	movs r1, #90
	bl 0x0200c634
	adds r5, #100
	adds r0, #60
	strh r0, [r5]
	ldr r5, [pc, #720]
	movs r0, #10
	adds r1, r5, #0
	bl 0x0200c744
	adds r1, r5, #0
	movs r0, #24
	bl 0x0200c744
	adds r1, r5, #0
	movs r0, #25
	bl 0x0200c744
	movs r0, #26
	bl 0x0200c74c
	movs r0, #10
	bl 0x0200c6f4
	movs r0, #159
	bl 0x0200c8b4
	movs r1, #38
	movs r2, #72
	ldr r0, [pc, #676]
	bl 0x0200c68c
	movs r0, #30
	bl 0x0200c6f4
	bl 0x0200c8ac
	movs r0, #224
	movs r1, #1
	movs r3, #1
	negs r1, r1
	ldr r2, [pc, #656]
	lsls r0, r0, #15
	bl 0x0200c824
	movs r0, #158
	bl 0x0200c8b4
	movs r2, #73
	movs r1, #35
	ldr r0, [pc, #644]
	bl 0x0200c68c
	movs r0, #20
	bl 0x0200c6f4
	bl 0x0200c8ac
	ldr r1, [pc, #632]
	movs r0, #9
	bl 0x0200c744
	movs r0, #20
	bl 0x0200c6f4
	ldr r1, [pc, #620]
	movs r0, #26
	bl 0x0200c744
	movs r0, #40
	bl 0x0200c6f4
	movs r0, #159
	bl 0x0200c8b4
	movs r1, #35
	movs r2, #73
	ldr r0, [pc, #600]
	bl 0x0200c68c
	movs r0, #26
	bl 0x0200c74c
	bl 0x0200c8ac
	movs r0, #40
	bl 0x0200c6f4
	ldr r5, [pc, #584]
	adds r0, r5, #0
	bl 0x0200c7d4
	movs r2, #20
.L_02002abc:
	movs r0, #9
	movs r1, #0
	bl 0x0200c7ec
	movs r0, #26
	movs r1, #3
	bl 0x0200c79c
	movs r2, #40
	ldr r0, [pc, #560]
	movs r1, #0
	bl 0x0200c7ec
	movs r0, #9
	movs r1, #3
	bl 0x0200c794
	movs r1, #3
	movs r0, #26
	bl 0x0200c79c
	movs r0, #30
	bl 0x0200c6f4
	ldr r1, [pc, #532]
	movs r0, #9
	bl 0x0200c744
	ldr r1, [pc, #528]
	movs r0, #26
	bl 0x0200c744
	movs r0, #40
	bl 0x0200c6f4
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200c81c
	movs r0, #210
	movs r1, #1
	movs r3, #1
	negs r1, r1
	ldr r2, [pc, #500]
	lsls r0, r0, #15
	bl 0x0200c824
	movs r0, #9
	bl 0x0200c74c
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200c80c
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl 0x0200c7ec
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #128
	movs r0, #22
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200c7fc
	ldr r2, [pc, #432]
	movs r0, #9
	movs r1, #105
	bl 0x0200c77c
	movs r0, #9
	movs r1, #2
	bl 0x0200c7b4
	movs r1, #0
	ldr r0, [pc, #416]
	bl 0x0200c7dc
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl 0x0200c7fc
	movs r0, #0
	movs r1, #0
	bl 0x0200c724
	cmp r0, #0
	bne .L_02002abc_0
	movs r0, #9
	movs r1, #3
	bl 0x0200c79c
	adds r0, r5, #4
	bl 0x0200c7d4
	b .L_02002abc_1
.L_02002abc_0:
	movs r0, #9
	movs r1, #2
	bl 0x0200c7b4
	adds r0, r5, #5
	bl 0x0200c7d4
.L_02002abc_1:
	ldr r0, [pc, #360]
	movs r1, #0
	bl 0x0200c7e4
	movs r1, #128
	movs r0, #22
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200c7fc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl 0x0200c80c
	ldr r5, [pc, #332]
	adds r0, r5, #0
	bl 0x0200c7d4
	movs r1, #0
	ldr r0, [pc, #316]
	bl 0x0200c7dc
	movs r0, #0
	movs r1, #0
	bl 0x0200c724
	cmp r0, #0
	bne .L_02002abc_2
	movs r1, #3
	movs r0, #9
	bl 0x0200c79c
	adds r0, r5, #1
	bl 0x0200c7d4
	ldr r0, [pc, #284]
	movs r1, #0
	movs r2, #30
	bl 0x0200c7ec
	movs r1, #128
	movs r2, #20
	movs r0, #22
	lsls r1, r1, #8
	bl 0x0200c7fc
	movs r0, #0
	movs r1, #3
	bl 0x0200c794
	movs r0, #22
	movs r1, #3
	bl 0x0200c794
	movs r0, #9
	movs r1, #3
	bl 0x0200c79c
	movs r0, #40
	bl 0x0200c6f4
	b .L_02002abc_3
.L_02002abc_2:
	movs r0, #9
	ldr r1, [pc, #236]
	movs r2, #90
	bl 0x0200c80c
	movs r2, #40
	movs r0, #9
	ldr r1, [pc, #228]
	bl 0x0200c80c
	movs r1, #4
	movs r0, #9
	bl 0x0200c794
	adds r0, r5, #2
	bl 0x0200c7d4
	ldr r0, [pc, #196]
	movs r1, #0
	bl 0x0200c7e4
.L_02002abc_3:
	ldr r1, [pc, #204]
	movs r0, #9
	bl 0x0200c744
	movs r0, #90
	bl 0x0200c6f4
	movs r2, #0
	movs r1, #22
	movs r0, #0
	bl 0x0200c7c4
	movs r0, #40
	bl 0x0200c6f4
	movs r0, #0
	movs r1, #3
	bl 0x0200c794
	movs r1, #3
	movs r0, #22
	bl 0x0200c79c
	movs r0, #20
	bl 0x0200c6f4
	movs r0, #22
	movs r1, #2
	bl 0x0200c794
	movs r0, #0
	bl 0x0200c72c
	cmp r0, #0
	beq .L_02002abc_4
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #22
	bl 0x0200c764
.L_02002abc_4:
	movs r0, #22
	bl 0x0200c784
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl 0x0200c78c
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x04ab
	.2byte 0x9999
	.2byte 0x0001
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0xcab4
	.2byte 0x0200
	.2byte 0xca78
	.2byte 0x0200
	.2byte 0xd7a0
	.2byte 0x0200
	.2byte 0x0497
	.2byte 0x0000
	.2byte 0x04be
	.2byte 0x0000
	.2byte 0xcec8
	.2byte 0x0200
	.2byte 0xd7e2
	.2byte 0x0200
	.2byte 0x0000
	.2byte 0x04c9
	.2byte 0xd78a
	.2byte 0x0200
	.2byte 0xcb28
	.2byte 0x0200
	.2byte 0xcb9c
	.2byte 0x0200
	.2byte 0xd7cc
	.2byte 0x0200
	.2byte 0x0e9b
	.2byte 0x0000
	.4byte 0x0000201a
	.4byte 0x0200cc0c
	.4byte 0x0200cc5c
	.4byte 0x043e0000
	.4byte 0x0000043e
	.4byte 0x00008009
	.4byte 0x00000ea1
	.4byte 0x00000105
	.4byte 0x00000103
	.4byte 0x0200cca8
	.section .text.x0200b1ac,"ax",%progbits
	.global Scene_RunActorGroupDepartureSequence
	.thumb_func
Scene_RunActorGroupDepartureSequence:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #19
	sub	sp, #4
	bl 0x0200c72c
	adds	r7, r0, #0
	movs	r0, #27
	bl 0x0200c72c
	adds	r6, r0, #0
	ldr	r1, [r6, #80]
	ldr	r0, [r7, #80]
	mov	fp, r1
	mov	sl, r0
	movs	r1, #128
	movs	r0, #128
	lsls	r0, r0, #9
	lsls	r1, r1, #6
	bl 0x0200c81c
	movs	r0, #220
	movs	r1, #1
	movs	r3, #1
	lsls	r0, r0, #15
	negs	r1, r1
	ldr	r2, [pc, #428]
	bl 0x0200c824
	movs	r0, #8
	ldr	r1, [pc, #424]
	ldr	r2, [pc, #424]
	bl 0x0200c73c
	movs	r0, #26
	ldr	r1, [pc, #412]
	ldr	r2, [pc, #416]
	bl 0x0200c73c
	movs	r0, #0
	ldr	r1, [pc, #404]
	ldr	r2, [pc, #404]
	bl 0x0200c73c
	ldr	r2, [pc, #400]
	movs	r0, #22
	ldr	r1, [pc, #392]
	bl 0x0200c73c
	ldr	r5, [pc, #392]
	movs	r0, #8
	adds	r1, r5, #0
	bl 0x0200c744
	movs	r0, #10
	bl 0x0200c6f4
	adds	r1, r5, #0
	movs	r0, #26
	bl 0x0200c744
	bl 0x0200c864
	movs	r0, #10
	bl 0x0200c6f4
	adds	r1, r5, #0
	movs	r0, #0
	bl 0x0200c744
	movs	r0, #10
	bl 0x0200c6f4
	bl 0x0200c85c
	adds	r1, r5, #0
	movs	r0, #22
	bl 0x0200c744
	movs	r0, #128
	bl 0x0200c6f4
	bl 0x02008134
	movs	r0, #174
	movs	r1, #1
	negs	r1, r1
	ldr	r2, [pc, #320]
	movs	r3, #1
	lsls	r0, r0, #16
	bl 0x0200c824
	movs	r0, #104
	bl 0x0200c6f4
	movs	r0, #153
	movs	r1, #1
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	ldr	r2, [pc, #300]
	bl 0x0200c824
	movs	r2, #159
	movs	r0, #9
	movs	r1, #158
	lsls	r2, r2, #3
	bl 0x0200c77c
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #9
	bl 0x0200c7fc
	movs	r0, #8
	bl 0x0200c74c
	ldr	r1, [pc, #268]
	movs	r0, #8
	bl 0x0200c744
	ldr	r1, [pc, #264]
	movs	r0, #26
	bl 0x0200c744
	ldr	r1, [pc, #260]
	movs	r0, #0
	bl 0x0200c744
	ldr	r1, [pc, #256]
	movs	r0, #22
	bl 0x0200c75c
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	lsls	r0, r0, #11
	bl 0x0200c6bc
	movs	r0, #145
	bl 0x0200c8b4
	movs	r0, #20
	bl 0x0200c6f4
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	bl 0x0200c6bc
	movs	r0, #60
	bl 0x0200c6f4
	movs	r0, #0
	ldr	r1, [pc, #200]
	movs	r2, #0
	bl 0x0200c80c
	movs	r0, #26
	ldr	r1, [pc, #188]
	movs	r2, #0
	bl 0x0200c80c
	movs	r0, #22
	ldr	r1, [pc, #180]
	movs	r2, #0
	bl 0x0200c80c
	movs	r0, #8
	ldr	r1, [pc, #168]
	movs	r2, #0
	bl 0x0200c80c
	movs	r0, #9
	ldr	r1, [pc, #160]
	movs	r2, #60
	bl 0x0200c80c
	movs	r0, #26
	movs	r1, #8
	movs	r2, #0
	bl 0x0200c7c4
	movs	r2, #0
	movs	r1, #0
	movs	r0, #22
	bl 0x0200c7c4
	movs	r0, #20
	bl 0x0200c6f4
	movs	r0, #0
	bl 0x0200c72c
	adds	r5, r0, #0
	bl 0x0200c654
	movs	r1, #20
	bl 0x0200c634
	adds	r5, #100
	ldr	r2, [pc, #60]
	adds	r0, #20
	movs	r3, #0
	strh	r0, [r5, #0]
	movs	r0, #22
	mov	r9, r3
	mov	r8, r2
	bl 0x0200c72c
	adds	r5, r0, #0
	bl 0x0200c654
	movs	r1, #20
	bl 0x0200c634
	adds	r5, #100
	adds	r0, #20
	strh	r0, [r5, #0]
	movs	r0, #26
	bl 0x0200c72c
	adds	r5, r0, #0
	bl 0x0200c654
	movs	r1, #20
	bl 0x0200c634
	adds	r5, #100
	adds	r0, #20
	strh	r0, [r5, #0]
.L_02003390:
	b.n	.L_020033c4
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x058b0000
	.4byte 0x00013333
	.4byte 0x00009999
	.4byte 0x0200cd6c
	.4byte 0x05940000
	.4byte 0x052d0000
	.4byte 0x0200ce04
	.4byte 0x0200ce30
	.4byte 0x0200ce5c
	.4byte 0x0200ce88
	.2byte 0x0101
	.2byte 0x0000
.L_020033c4:
	movs	r0, #8
	bl 0x0200c72c
	adds	r5, r0, #0
	bl 0x0200c654
	movs	r1, #20
	bl 0x0200c634
	adds	r5, #100
	adds	r0, #20
	strh	r0, [r5, #0]
	movs	r0, #9
	bl 0x0200c72c
	adds	r5, r0, #0
	bl 0x0200c654
	movs	r1, #20
	bl 0x0200c634
	adds	r5, #100
	adds	r0, #20
	strh	r0, [r5, #0]
	ldr	r5, [pc, #1012]
	movs	r0, #9
	adds	r1, r5, #0
	bl 0x0200c744
	movs	r0, #30
	bl 0x0200c6f4
	adds	r1, r5, #0
	movs	r0, #0
	bl 0x0200c744
	adds	r1, r5, #0
	movs	r0, #26
	bl 0x0200c744
	adds	r1, r5, #0
	movs	r0, #22
	bl 0x0200c744
	adds	r1, r5, #0
	movs	r0, #8
	bl 0x0200c744
	movs	r0, #10
	bl 0x0200c6f4
	movs	r0, #17
	bl 0x0200c8b4
	movs	r0, #192
	movs	r1, #192
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200c6bc
	movs	r0, #145
	bl 0x0200c8b4
	movs	r0, #30
	bl 0x0200c6f4
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	bl 0x0200c6bc
	movs	r0, #120
	bl 0x0200c6f4
	movs	r0, #192
	movs	r1, #192
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200c6bc
	movs	r0, #145
	bl 0x0200c8b4
	movs	r0, #40
	bl 0x0200c6f4
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200c6bc
	movs	r0, #60
	bl 0x0200c6f4
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	lsls	r0, r0, #11
	bl 0x0200c6bc
	movs	r0, #145
	bl 0x0200c8b4
	movs	r0, #20
	bl 0x0200c6f4
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	bl 0x0200c6bc
	movs	r0, #60
	bl 0x0200c6f4
	movs	r0, #192
	movs	r1, #192
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200c6bc
	movs	r0, #145
	bl 0x0200c8b4
	movs	r0, #40
	bl 0x0200c6f4
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	bl 0x0200c6bc
	movs	r0, #60
	bl 0x0200c6f4
	bl 0x0200c85c
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	bl 0x0200c6bc
	movs	r0, #1
	bl 0x0200c6f4
	movs	r0, #1
	movs	r1, #1
	ldr	r2, [pc, #728]
	negs	r0, r0
	negs	r1, r1
	bl 0x0200c6bc
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #12
	lsls	r1, r1, #12
	bl 0x0200c81c
	movs	r0, #217
	movs	r1, #1
	ldr	r2, [pc, #704]
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	bl 0x0200c824
	movs	r1, #0
	movs	r0, #0
	bl 0x0200c874
	movs	r0, #40
	bl 0x0200c87c
	movs	r0, #40
	bl 0x0200c63c
	movs	r1, #0
	movs	r0, #19
	bl 0x0200c7cc
	movs	r0, #19
	bl 0x0200c72c
	movs	r1, #0
	bl 0x0200c6b4
	movs	r0, #27
	bl 0x0200c72c
	movs	r1, #0
	bl 0x0200c6b4
	ldr	r3, [pc, #644]
	adds	r1, r6, #0
	str	r3, [r6, #24]
	str	r3, [r6, #28]
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r0, #254
	adds	r3, r0, #0
	ands	r3, r2
	strb	r3, [r1, #0]
	mov	r1, fp
	ldrb	r2, [r1, #9]
	movs	r1, #13
	negs	r1, r1
	adds	r3, r1, #0
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	mov	r2, fp
	strb	r3, [r2, #9]
	movs	r3, #200
	lsls	r3, r3, #16
	ldr	r2, [pc, #604]
	str	r3, [r7, #8]
	str	r3, [r7, #12]
	str	r3, [r7, #56]
	str	r3, [r7, #60]
	adds	r3, r7, #0
	str	r2, [r7, #16]
	str	r2, [r7, #64]
	adds	r3, #85
	mov	r2, r8
	str	r3, [sp, #0]
	strb	r2, [r3, #0]
	adds	r2, r7, #0
	adds	r2, #35
	ldrb	r3, [r2, #0]
	ands	r0, r3
	strb	r0, [r2, #0]
	mov	r0, sl
	ldrb	r3, [r0, #9]
	ands	r1, r3
	strb	r1, [r0, #9]
	bl 0x0200c834
	movs	r5, #128
	lsls	r5, r5, #24
	str	r5, [r0, #56]
	bl 0x0200c834
	str	r5, [r0, #60]
	bl 0x0200c834
	str	r5, [r0, #64]
	bl 0x0200c834
	mov	r1, r9
	str	r1, [r0, #36]
	bl 0x0200c834
	mov	r2, r9
	str	r2, [r0, #40]
	bl 0x0200c834
	mov	r3, r9
	str	r3, [r0, #44]
	movs	r0, #1
	bl 0x0200c63c
	movs	r0, #247
	movs	r1, #128
	ldr	r2, [pc, #512]
	movs	r3, #0
	lsls	r1, r1, #16
	lsls	r0, r0, #16
	bl 0x0200c824
	bl 0x0200c684
	movs	r0, #1
	bl 0x0200c63c
	ldr	r0, [pc, #492]
	movs	r1, #1
	bl 0x0200c874
	movs	r0, #128
	movs	r1, #2
	lsls	r0, r0, #9
	bl 0x0200c874
	movs	r0, #30
	bl 0x0200c87c
	movs	r0, #30
	bl 0x0200c63c
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #464]
	bl 0x0200c644
	ldr	r1, [pc, #460]
	movs	r0, #19
	bl 0x0200c744
	movs	r0, #128
	lsls	r0, r0, #10
	ldr	r1, [pc, #452]
	bl 0x0200c81c
	movs	r0, #175
	movs	r1, #192
	lsls	r0, r0, #16
	lsls	r1, r1, #15
	ldr	r2, [pc, #444]
	movs	r3, #1
	bl 0x0200c824
	adds	r5, r7, #0
	adds	r5, #102
.L_02003662:
	movs	r0, #1
	bl 0x0200c63c
	movs	r0, #0
	ldrsh	r3, [r5, r0]
	cmp	r3, #8
	bne.n	.L_02003662
	movs	r1, #0
	movs	r0, #0
	bl 0x0200c874
	movs	r0, #60
	bl 0x0200c87c
	movs	r0, #60
	bl 0x0200c63c
	bl 0x0200c6c4
	bl 0x0200c834
	movs	r1, #128
	lsls	r1, r1, #24
	str	r1, [r0, #56]
	mov	r8, r1
	bl 0x0200c834
	mov	r2, r8
	str	r2, [r0, #60]
	bl 0x0200c834
	mov	r3, r8
	str	r3, [r0, #64]
	bl 0x0200c834
	movs	r5, #0
	str	r5, [r0, #36]
	bl 0x0200c834
	str	r5, [r0, #40]
	bl 0x0200c834
	str	r5, [r0, #44]
	ldr	r0, [pc, #332]
	bl 0x0200c64c
	movs	r0, #19
	bl 0x0200c754
	movs	r0, #1
	bl 0x0200c63c
	movs	r0, #19
	movs	r1, #0
	bl 0x0200c794
	mov	r2, fp
	movs	r3, #160
	lsls	r3, r3, #9
	adds	r2, #35
	movs	r0, #2
	str	r3, [r6, #24]
	str	r3, [r6, #28]
	strb	r0, [r2, #0]
	movs	r2, #128
	lsls	r2, r2, #10
	mov	r1, fp
	str	r3, [r1, #24]
	movs	r0, #1
	str	r2, [r7, #24]
	str	r2, [r7, #28]
	str	r5, [r7, #8]
	str	r5, [r7, #16]
	str	r5, [r7, #56]
	str	r5, [r7, #64]
	mov	sl, r2
	bl 0x0200c63c
	movs	r0, #23
	movs	r1, #8
	bl 0x0200c794
	movs	r1, #169
	movs	r2, #158
	movs	r0, #9
	lsls	r1, r1, #16
	lsls	r2, r2, #19
	bl 0x0200c78c
	movs	r1, #192
	movs	r2, #0
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x0200c7fc
	movs	r0, #9
	movs	r1, #9
	bl 0x0200c794
	movs	r1, #151
	movs	r0, #26
	lsls	r1, r1, #16
	ldr	r2, [pc, #232]
	bl 0x0200c78c
	movs	r1, #128
	movs	r2, #0
	movs	r0, #26
	lsls	r1, r1, #8
	bl 0x0200c7fc
	movs	r0, #26
	movs	r1, #5
	bl 0x0200c794
	movs	r1, #170
	movs	r0, #8
	lsls	r1, r1, #16
	ldr	r2, [pc, #204]
	bl 0x0200c78c
	movs	r1, #192
	movs	r2, #0
	movs	r0, #8
	lsls	r1, r1, #7
	bl 0x0200c7fc
	movs	r0, #8
	movs	r1, #5
	bl 0x0200c794
	movs	r1, #185
	movs	r0, #0
	lsls	r1, r1, #16
	ldr	r2, [pc, #176]
	bl 0x0200c78c
	movs	r1, #128
	movs	r2, #0
	movs	r0, #0
	lsls	r1, r1, #6
	bl 0x0200c7fc
	movs	r0, #0
	movs	r1, #17
	bl 0x0200c794
	movs	r1, #169
	movs	r2, #173
	movs	r0, #22
	lsls	r1, r1, #16
	lsls	r2, r2, #19
	bl 0x0200c78c
	movs	r1, #128
	movs	r2, #0
	movs	r0, #22
	lsls	r1, r1, #7
	bl 0x0200c7fc
	movs	r0, #22
	movs	r1, #0
	bl 0x0200c794
	movs	r0, #166
	movs	r1, #0
	ldr	r2, [pc, #116]
	lsls	r0, r0, #16
	movs	r3, #0
	bl 0x0200c824
	bl 0x0200c684
	ldr	r3, [sp, #0]
	mov	r0, r8
	strb	r5, [r3, #0]
	str	r0, [r7, #56]
	str	r0, [r7, #60]
	str	r0, [r7, #64]
	bl 0x0200bc48
	movs	r1, #218
	movs	r2, #147
	movs	r0, #27
	lsls	r1, r1, #16
	lsls	r2, r2, #19
	bl 0x0200c78c
	movs	r0, #210
	ldr	r2, [pc, #72]
	movs	r3, #0
	lsls	r0, r0, #16
	movs	r1, #0
	bl 0x0200c824
	b.n	.L_0200382c
	.2byte 0x0000
	.4byte 0x0200ceb4
	.4byte 0x0000e666
	.4byte 0x043c0000
	.4byte 0x0000cccc
	.4byte 0x03820000
	.4byte 0x03950000
	.4byte 0x00010003
	.4byte 0x0200bce5
	.4byte 0x0200cedc
	.4byte 0x000007ae
	.4byte 0x043e0000
	.4byte 0x050c0000
	.4byte 0x05210000
	.4byte 0x05350000
	.4byte 0x05390000
	.2byte 0x0000
	.2byte 0x04ac
.L_0200382c:
	bl 0x0200c684
	mov	r1, sl
	str	r1, [r6, #24]
	str	r1, [r6, #28]
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #644]
	bl 0x0200c644
	movs	r0, #10
	bl 0x0200c754
	movs	r0, #24
	bl 0x0200c754
	movs	r0, #25
	bl 0x0200c754
	movs	r0, #1
	bl 0x0200c63c
	movs	r0, #10
	bl 0x0200c72c
	adds	r6, r0, #0
	ldr	r2, [r6, #80]
	adds	r1, r6, #0
	adds	r1, #35
	mov	fp, r2
	ldrb	r2, [r1, #0]
	movs	r3, #254
	movs	r0, #128
	mov	sl, r3
	lsls	r0, r0, #9
	ands	r3, r2
	strb	r3, [r1, #0]
	str	r0, [r6, #24]
	str	r0, [r6, #28]
	mov	r1, fp
	movs	r3, #208
	ldrb	r2, [r1, #9]
	subs	r5, #13
	lsls	r3, r3, #8
	strh	r3, [r6, #6]
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r1, #9]
	movs	r0, #10
	movs	r1, #0
	bl 0x0200c794
	movs	r0, #24
	bl 0x0200c72c
	adds	r6, r0, #0
	ldr	r2, [r6, #80]
	adds	r1, r6, #0
	adds	r1, #35
	mov	fp, r2
	ldrb	r2, [r1, #0]
	mov	r3, sl
	ands	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r6, #24]
	str	r3, [r6, #28]
	movs	r0, #176
	mov	r3, fp
	ldrb	r2, [r3, #9]
	lsls	r0, r0, #8
	mov	r9, r0
	adds	r3, r5, #0
	ands	r3, r2
	mov	r1, r9
	mov	r0, fp
	strb	r3, [r0, #9]
	strh	r1, [r6, #6]
	movs	r0, #24
	movs	r1, #5
	bl 0x0200c794
	movs	r0, #25
	bl 0x0200c72c
	adds	r6, r0, #0
	ldr	r1, [r6, #80]
	mov	fp, r1
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	mov	r3, sl
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #9
	strb	r3, [r1, #0]
	str	r2, [r6, #24]
	str	r2, [r6, #28]
	mov	r0, fp
	ldrb	r2, [r0, #9]
	mov	r3, r9
	strh	r3, [r6, #6]
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r0, #9]
	movs	r1, #5
	movs	r0, #25
	bl 0x0200c794
	movs	r0, #27
	bl 0x0200c72c
	adds	r6, r0, #0
	ldr	r1, [r6, #80]
	mov	fp, r1
	bl 0x0200bc48
	movs	r3, #192
	lsls	r3, r3, #14
	movs	r1, #214
	movs	r2, #152
	str	r3, [r7, #12]
	lsls	r1, r1, #16
	mov	r3, r8
	lsls	r2, r2, #19
	str	r1, [r7, #8]
	str	r2, [r7, #16]
	str	r3, [r7, #56]
	str	r3, [r7, #60]
	str	r3, [r7, #64]
	mov	r0, fp
	ldrb	r3, [r0, #9]
	ands	r5, r3
	movs	r3, #4
	orrs	r5, r3
	strb	r5, [r0, #9]
	movs	r0, #27
	bl 0x0200c78c
	movs	r1, #192
	movs	r0, #24
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c7fc
	movs	r1, #192
	movs	r2, #20
	lsls	r1, r1, #8
	movs	r0, #25
	bl 0x0200c7fc
	movs	r0, #179
	lsls	r0, r0, #1
	bl 0x0200c6e4
	movs	r0, #0
	bl 0x0200c6a4
	movs	r0, #1
	bl 0x0200c6a4
	movs	r0, #2
	bl 0x0200c6a4
	movs	r0, #3
	bl 0x0200c6a4
	movs	r0, #4
	bl 0x0200c6a4
	movs	r0, #5
	bl 0x0200c6a4
	ldr	r0, [pc, #312]
	movs	r1, #1
	bl 0x0200c874
	movs	r0, #128
	movs	r1, #2
	lsls	r0, r0, #9
	bl 0x0200c874
	movs	r0, #120
	bl 0x0200c87c
	movs	r0, #160
	bl 0x0200c63c
	ldr	r0, [pc, #288]
	movs	r1, #1
	bl 0x0200c874
	movs	r1, #2
	ldr	r0, [pc, #276]
	bl 0x0200c874
	movs	r0, #80
	bl 0x0200c87c
	movs	r0, #80
	bl 0x0200c6f4
	movs	r0, #100
	bl 0x0200c6f4
	ldr	r0, [pc, #244]
	bl 0x0200c64c
	ldr	r3, [r6, #24]
	mov	r1, fp
	movs	r0, #179
	str	r3, [r1, #24]
	lsls	r0, r0, #1
	bl 0x0200c6ec
	movs	r0, #0
	bl 0x0200c69c
	movs	r0, #1
	bl 0x0200c69c
	movs	r0, #2
	bl 0x0200c69c
	movs	r0, #3
	bl 0x0200c69c
	movs	r0, #4
	bl 0x0200c69c
	movs	r0, #5
	bl 0x0200c69c
	bl 0x0200c0f0
	movs	r1, #165
	ldr	r2, [pc, #196]
	movs	r0, #9
	lsls	r1, r1, #16
	bl 0x0200c78c
	movs	r1, #1
	movs	r0, #9
	bl 0x0200c794
	movs	r0, #9
	bl 0x0200c72c
	movs	r2, #224
	lsls	r2, r2, #8
	mov	r8, r2
	adds	r7, r0, #0
	mov	r3, r8
	strh	r3, [r7, #6]
	bl 0x0200c654
	movs	r1, #90
	bl 0x0200c634
	adds	r3, r7, #0
	ldr	r5, [pc, #152]
	adds	r0, #60
	adds	r3, #100
	adds	r2, r7, #0
	strh	r0, [r3, #0]
	adds	r2, #102
	movs	r3, #1
	strh	r3, [r2, #0]
	adds	r1, r5, #0
	movs	r0, #9
	bl 0x0200c744
	movs	r1, #165
	ldr	r2, [pc, #128]
	movs	r0, #26
	lsls	r1, r1, #16
	bl 0x0200c78c
	movs	r1, #1
	movs	r0, #26
	bl 0x0200c794
	movs	r0, #26
	bl 0x0200c72c
	adds	r7, r0, #0
	mov	r0, r8
	strh	r0, [r7, #6]
	bl 0x0200c654
	movs	r1, #90
	bl 0x0200c634
	adds	r3, r7, #0
	adds	r0, #60
	adds	r3, #100
	ldr	r1, [pc, #60]
	strh	r0, [r3, #0]
	adds	r3, #2
	strh	r1, [r3, #0]
	movs	r0, #26
	adds	r1, r5, #0
	bl 0x0200c744
	movs	r1, #152
	ldr	r2, [pc, #68]
	movs	r0, #22
	lsls	r1, r1, #16
	bl 0x0200c78c
	movs	r1, #1
	movs	r0, #22
	bl 0x0200c794
	movs	r0, #22
	bl 0x0200c72c
	mov	r2, r8
	adds	r7, r0, #0
	strh	r2, [r7, #6]
	bl 0x0200c654
	movs	r1, #90
	bl 0x0200c634
	adds	r3, r7, #0
	b.n	.L_02003adc
	.4byte 0x00000002
	.4byte 0x0200be19
	.4byte 0x00010003
	.4byte 0x00007fff
	.4byte 0x04cd0000
	.4byte 0x0200cec8
	.4byte 0x04e60000
	.2byte 0x0000
	.2byte 0x0505
.L_02003adc:
	adds	r0, #60
	adds	r3, #100
	adds	r2, r7, #0
	strh	r0, [r3, #0]
	adds	r2, #102
	movs	r3, #3
	strh	r3, [r2, #0]
	adds	r1, r5, #0
	movs	r0, #22
	bl 0x0200c744
	movs	r1, #180
	ldr	r2, [pc, #312]
	lsls	r1, r1, #16
	movs	r0, #8
	bl 0x0200c78c
	movs	r0, #8
	bl 0x0200c72c
	mov	r3, r8
	adds	r7, r0, #0
	strh	r3, [r7, #6]
	bl 0x0200c654
	movs	r1, #90
	bl 0x0200c634
	adds	r3, r7, #0
	adds	r0, #60
	adds	r3, #100
	adds	r2, r7, #0
	adds	r2, #102
	strh	r0, [r3, #0]
	movs	r3, #4
	strh	r3, [r2, #0]
	adds	r1, r5, #0
	movs	r0, #8
	bl 0x0200c744
	movs	r1, #6
	movs	r0, #8
	bl 0x0200c794
	movs	r0, #22
	bl 0x0200c72c
	adds	r0, #35
	ldrb	r2, [r0, #0]
	mov	r3, sl
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #8
	bl 0x0200c72c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	mov	r1, sl
	ands	r1, r3
	strb	r1, [r0, #0]
	mov	sl, r1
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #216]
	bl 0x0200c644
	movs	r1, #181
	lsls	r1, r1, #16
	ldr	r2, [pc, #208]
	movs	r0, #0
	bl 0x0200c78c
	movs	r0, #0
	bl 0x0200c72c
	mov	r2, r8
	strh	r2, [r0, #6]
	movs	r1, #1
	movs	r0, #0
	bl 0x0200c794
	movs	r0, #181
	movs	r3, #0
	lsls	r0, r0, #16
	movs	r1, #0
	ldr	r2, [pc, #176]
	bl 0x0200c824
	bl 0x0200c684
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c78c
	movs	r0, #19
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c78c
	movs	r0, #24
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c78c
	movs	r0, #25
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c78c
	movs	r0, #23
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c78c
	movs	r0, #27
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c78c
	movs	r1, #144
	movs	r0, #17
	lsls	r1, r1, #16
	ldr	r2, [pc, #104]
	bl 0x0200c78c
	movs	r1, #138
	ldr	r2, [pc, #100]
	lsls	r1, r1, #17
	movs	r0, #18
	bl 0x0200c78c
	movs	r0, #60
	bl 0x0200c63c
	ldr	r0, [pc, #88]
	movs	r1, #1
	bl 0x0200c874
	movs	r0, #128
	movs	r1, #2
	lsls	r0, r0, #9
	bl 0x0200c874
	movs	r0, #80
	bl 0x0200c87c
	movs	r0, #60
	bl 0x0200c6f4
	bl 0x0200c8ac
	movs	r0, #60
	bl 0x0200c6f4
	movs	r0, #1
	bl 0x0200c71c
	bl 0x0200c86c
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x051f0000
	.4byte 0x0200c5b9
	.4byte 0x04f90000
	.4byte 0x042e0000
	.4byte 0x04f60000
	.2byte 0x0003
	.2byte 0x0001
	.section .rodata,"a",%progbits
	.global HaidiaArashi_FrameModes
HaidiaArashi_FrameModes:
	.4byte 0x00000007
	.global HaidiaArashi_ActorTwentyScript
HaidiaArashi_ActorTwentyScript:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00b90000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00b30000
	.4byte 0x00000000
	.4byte 0x03440000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00e20000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00e20000
	.4byte 0x00000000
	.4byte 0x03b00000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global HaidiaArashi_ActorTwentyTwoScriptA
HaidiaArashi_ActorTwentyTwoScriptA:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01590000
	.4byte 0x00000000
	.4byte 0x02640000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01370000
	.4byte 0x00000000
	.4byte 0x027e0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x02600000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global HaidiaArashi_ActorTwentyTwoScriptB
HaidiaArashi_ActorTwentyTwoScriptB:
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x000000c0
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000180
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000018
	.4byte 0xc0010000
	.4byte 0x80020000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xfffffe80
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffffd00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000000c
	.4byte 0xc0020000
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000010
	.global HaidiaArashi_ActorNineteenScript
HaidiaArashi_ActorNineteenScript:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000010
	.global HaidiaArashi_LeaderScript
HaidiaArashi_LeaderScript:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x005e0000
	.4byte 0x00000000
	.4byte 0x02340000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000010
	.global HaidiaArashi_ActorTwentyTwoScript
HaidiaArashi_ActorTwentyTwoScript:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x004c0000
	.4byte 0x00000000
	.4byte 0x02340000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00b20000
	.4byte 0x00000000
	.4byte 0x04a60000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00950000
	.4byte 0x00000000
	.4byte 0x04aa0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00c60000
	.4byte 0x00000000
	.4byte 0x04980000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00aa0000
	.4byte 0x00000000
	.4byte 0x04ab0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00950000
	.4byte 0x00000000
	.4byte 0x04aa0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00950000
	.4byte 0x00000000
	.4byte 0x04970000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000002
	.4byte 0x00660000
	.4byte 0x00000000
	.4byte 0x04a50000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00660000
	.4byte 0x00000000
	.4byte 0x04c20000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00540000
	.4byte 0x00000000
	.4byte 0x04cc0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000002
	.4byte 0x00660000
	.4byte 0x00000000
	.4byte 0x04a50000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00660000
	.4byte 0x00000000
	.4byte 0x04c20000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x04cc0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00320000
	.4byte 0x00000000
	.4byte 0x04be0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x002e0000
	.4byte 0x00000000
	.4byte 0x046a0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00430000
	.4byte 0x00000000
	.4byte 0x044c0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00990000
	.4byte 0x00000000
	.4byte 0x04e30000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00b10000
	.4byte 0x00000000
	.4byte 0x054d0000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x007b0000
	.4byte 0x00000000
	.4byte 0x04360000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00940000
	.4byte 0x00000000
	.4byte 0x04360000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00940000
	.4byte 0x00000000
	.4byte 0x04280000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00230000
	.4byte 0x00000000
	.4byte 0x04280000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global HaidiaArashi_StormRunActions
HaidiaArashi_StormRunActions:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00660000
	.4byte 0x00000000
	.4byte 0x04d40000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00940000
	.4byte 0x00000000
	.4byte 0x04e80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00b10000
	.4byte 0x00000000
	.4byte 0x05170000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00890000
	.4byte 0x00000000
	.4byte 0x05630000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00720000
	.4byte 0x00000000
	.4byte 0x05880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x005f0000
	.4byte 0x00000000
	.4byte 0x05880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00530000
	.4byte 0x00000000
	.4byte 0x05930000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x009a0000
	.4byte 0x00000000
	.4byte 0x05960000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00b30000
	.4byte 0x00000000
	.4byte 0x05760000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00b30000
	.4byte 0x00000000
	.4byte 0x05510000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x051d0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x00a50000
	.4byte 0x00000000
	.4byte 0x051a0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x00b70000
	.4byte 0x00000000
	.4byte 0x052d0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x00a60000
	.4byte 0x00000000
	.4byte 0x052d0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000010
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global HaidiaArashi_ActorEightScript
HaidiaArashi_ActorEightScript:
	.4byte 0x00000022
	.4byte 0x02008065
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00030000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00020000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00020000
	.4byte 0x00000003
	.4byte 0x01000000
	.4byte 0x00800000
	.4byte 0x03a30000
	.4byte 0x00000001
	.4byte 0x0000001e
	.4byte 0x00000091
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x0000003c
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00e40000
	.4byte 0x00950000
	.4byte 0x03bd0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00c90000
	.4byte 0x00a00000
	.4byte 0x03d80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00ad0000
	.4byte 0x008a0000
	.4byte 0x03f30000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000005
	.4byte 0x00000003
	.4byte 0x00920000
	.4byte 0x00600000
	.4byte 0x040e0000
	.4byte 0x00000001
	.4byte 0x0000001e
	.4byte 0x00000091
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00008000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x0000003c
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000006
	.4byte 0x00000003
	.4byte 0x00a10000
	.4byte 0x006b0000
	.4byte 0x041a0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00020000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00009999
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000007
	.4byte 0x00000003
	.4byte 0x00b10000
	.4byte 0x00600000
	.4byte 0x04260000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00028000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x0000cccc
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000008
	.4byte 0x00000003
	.4byte 0x00c90000
	.4byte 0x004a0000
	.4byte 0x04320000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00030000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000009
	.4byte 0x00000003
	.4byte 0x00e10000
	.4byte 0xffc00000
	.4byte 0x043e0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000000
	.4byte 0x00000010
	.global HaidiaArashi_SceneTable0
HaidiaArashi_SceneTable0:
	.4byte 0xffff0000
	.4byte 0x000000a7
	.4byte 0x40000501
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000101
	.4byte 0x400001c8
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
	.4byte 0x0000001b
	.4byte 0x0000026d
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
	.4byte 0xffff000c
	.4byte 0x00000065
	.4byte 0x400004c0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000d
	.4byte 0x00000092
	.4byte 0x400004a9
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000f
	.4byte 0x0000014f
	.4byte 0xc000038c
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0010
	.4byte 0x000000b5
	.4byte 0xe00004f9
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global HaidiaArashi_SceneTable1
HaidiaArashi_SceneTable1:
	.4byte 0x00000003
	.4byte 0x00208005
	.4byte 0x00301006
	.4byte 0x00402006
	.4byte 0x00506007
	.4byte 0x00605007
	.4byte 0x00706008
	.4byte 0x00802007
	.4byte 0x00901007
	.4byte 0x00a01007
	.4byte 0x00b01007
	.4byte 0x00c0b008
	.4byte 0x00d0c008
	.4byte 0x01410003
	.4byte 0x000001ff
	.global HaidiaArashi_SceneTable2
HaidiaArashi_SceneTable2:
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff001f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0035
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x01b50000
	.4byte 0x00000000
	.4byte 0x02fa0000
	.4byte 0x00024000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x01420000
	.4byte 0x00000000
	.4byte 0x05990000
	.4byte 0x01024000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x042e0000
	.4byte 0x01024000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x01140000
	.4byte 0x00000000
	.4byte 0x04f60000
	.4byte 0x01024000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00d2
	.4byte 0x00000001
	.4byte 0x01a40000
	.4byte 0x00000000
	.4byte 0x026b0000
	.4byte 0x00004000
	.4byte 0xffff0013
	.4byte 0x00000001
	.4byte 0x018c0000
	.4byte 0x00000000
	.4byte 0x026b0000
	.4byte 0x00000000
	.4byte 0xffff0024
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff003b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff003c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0030
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0120
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
	.global HaidiaArashi_LastObjectCall
HaidiaArashi_LastObjectCall:
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x007c0000
	.4byte 0x00000000
	.4byte 0x01770000
	.4byte 0x0002b000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00550000
	.4byte 0x00000000
	.4byte 0x01690000
	.4byte 0x0002d000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00870000
	.4byte 0x00000000
	.4byte 0x016b0000
	.4byte 0x0002b000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x00640000
	.4byte 0x00000000
	.4byte 0x01770000
	.4byte 0x0002d000
	.4byte 0xffff007a
	.4byte 0x00000001
	.4byte 0x007c0000
	.4byte 0x00000000
	.4byte 0x01770000
	.4byte 0x0002b000
	.4byte 0xffff007a
	.4byte 0x00000001
	.4byte 0x00640000
	.4byte 0x00000000
	.4byte 0x01770000
	.4byte 0x0002d000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global HaidiaArashi_SceneTable3
HaidiaArashi_SceneTable3:
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x020081d1
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x020081e5
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x020081f9
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x0200820d
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008241
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008279
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x020082e5
	.4byte 0x0000c602
	.4byte 0xffff0009
	.4byte 0x02008329
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte 0x02008361
	.4byte 0x0000c602
	.4byte 0xffff000b
	.4byte 0x02008399
	.4byte 0x0000c602
	.4byte 0xffff000c
	.4byte 0x020083cd
	.4byte 0x0000c602
	.4byte 0xffff000d
	.4byte 0x02008401
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x020096cd
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0200ad29
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00000ea6
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x00000ea7
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00000ece
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x00000ecf
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0200be49
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001121
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001121
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001121
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001121
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001121
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001121
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001121
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001121
	.4byte 0x00000002
	.4byte 0xffff000f
	.4byte 0x0200810d
	.4byte 0x00000002
	.4byte 0xffff000e
	.4byte 0x02008135
	.4byte 0x00000002
	.4byte 0xffff0029
	.4byte 0x02008d5d
	.4byte 0x00000002
	.4byte 0xffff002a
	.4byte 0x02008f39
	.4byte 0x00000002
	.4byte 0xffff002b
	.4byte 0x02009155
	.4byte 0x00000002
	.4byte 0xffff002c
	.4byte 0x02009349
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte 0x02009601
	.4byte 0x00000002
	.4byte 0xffff001f
	.4byte 0x02009685
	.4byte 0x00000002
	.4byte 0xffff0020
	.4byte 0x02009829
	.4byte 0x00000002
	.4byte 0xffff0021
	.4byte 0x0200998d
	.4byte 0x00000002
	.4byte 0xffff0022
	.4byte 0x02009b19
	.4byte 0x00000002
	.4byte 0xffff0023
	.4byte 0x0200a181
	.4byte 0x00000002
	.4byte 0xffff0024
	.4byte 0x0200aded
	.4byte 0x00000002
	.4byte 0x087c0010
	.4byte 0x0200c235
	.4byte 0x00000002
	.4byte 0x087f0011
	.4byte 0x0200c279
	.4byte 0x00000003
	.4byte 0xffff0018
	.4byte 0x0200c5fd
	.4byte 0x00000003
	.4byte 0xffff0013
	.4byte 0x0200c619
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global HaidiaArashi_CellSteps0
HaidiaArashi_CellSteps0:
	.4byte 0x00620000
	.4byte 0x00020002
	.4byte 0x00020002
	.4byte 0x00020062
	.4byte 0x00020002
	.2byte 0xffff
	.global HaidiaArashi_CellSteps1
HaidiaArashi_CellSteps1:
	.2byte 0x0000
	.4byte 0x00020060
	.4byte 0x00020002
	.4byte 0x00600002
	.4byte 0x00020002
	.4byte 0xffff0002
	.global HaidiaArashi_CellSteps2
HaidiaArashi_CellSteps2:
	.4byte 0x00620004
	.4byte 0x00020002
	.4byte 0x00060002
	.4byte 0x00020062
	.4byte 0x00020002
	.2byte 0xffff
	.global HaidiaArashi_CellSteps3
HaidiaArashi_CellSteps3:
	.2byte 0x0004
	.4byte 0x00020060
	.4byte 0x00020002
	.4byte 0x00600006
	.4byte 0x00020002
	.4byte 0xffff0002
	.4byte 0x00600000
	.4byte 0x00020002
	.4byte 0x00320002
	.4byte 0x0002002c
	.4byte 0x00020002
	.4byte 0x0004ffff
	.4byte 0x00020062
	.4byte 0x00020002
	.4byte 0x006c002c
	.4byte 0x00020002
	.4byte 0xffff0002
