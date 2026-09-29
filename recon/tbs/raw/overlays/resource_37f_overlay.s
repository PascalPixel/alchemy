.syntax unified
	.thumb
	.section .text.x02008420,"ax",%progbits
	.balign 4
	.global Func_02000420
	.thumb_func
Func_02000420:
	push {r5, r6, lr}
	ldr r0, [pc, #308]
	sub sp, #8
	bl 0x02009b8c
	cmp r0, #0
	bne .L_02000420_0
	b .L_02000420_1
.L_02000420_0:
	ldr r0, [pc, #296]
	bl 0x02009b8c
	cmp r0, #0
	beq .L_02000420_2
	b .L_02000420_1
.L_02000420_2:
	bl 0x02009bac
	bl 0x02009cbc
	movs r0, #182
	bl 0x02009ccc
	movs r5, #1
	movs r2, #100
	movs r3, #71
	movs r1, #71
	movs r0, #0
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009b5c
	bl 0x02009b44
	movs r0, #40
	bl 0x02009ba4
	ldr r6, [pc, #248]
	movs r1, #1
	adds r0, r6, #0
	bl 0x02009b84
	movs r0, #20
	bl 0x02009ba4
	movs r0, #183
	bl 0x02009ccc
	movs r3, #2
	str r3, [sp, #4]
	movs r0, #122
	movs r1, #20
	movs r2, #120
	movs r3, #30
	str r5, [sp, #0]
	bl 0x02009b5c
	movs r3, #120
	movs r2, #30
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #2
	movs r0, #122
	movs r1, #20
	movs r2, #1
	bl 0x02009b64
	bl 0x02009b44
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x02009b7c
	movs r0, #20
	bl 0x02009ba4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x02009c6c
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl 0x02009b7c
	movs r0, #20
	bl 0x02009ba4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #40
	bl 0x02009c5c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #20
	bl 0x02009c5c
	movs r0, #0
	movs r1, #0
	movs r2, #20
	bl 0x02009c5c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #10
	bl 0x02009c5c
	movs r0, #0
	movs r1, #4
	movs r2, #20
	bl 0x02009c14
	movs r0, #0
	movs r1, #6
	movs r2, #40
	bl 0x02009c14
	movs r0, #1
	movs r1, #1
	ldr r2, [pc, #60]
	negs r1, r1
	negs r0, r0
	bl 0x02009b7c
	adds r6, #1
	movs r0, #40
	bl 0x02009ba4
	movs r1, #1
	adds r0, r6, #0
	bl 0x02009b84
	ldr r0, [pc, #40]
	bl 0x02009b94
	ldr r0, [pc, #20]
	bl 0x02009b94
	bl 0x02009bb4
.L_02000420_1:
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000f02
	.4byte 0x00000821
	.4byte 0x00001032
	.4byte 0x0000e666
	.4byte 0x00000143
	.section .text.x02008f8c,"ax",%progbits
	.balign 4
	.global SoruIriguchi_RunArrivalEvent
	.thumb_func
SoruIriguchi_RunArrivalEvent:
	push {lr}
.L_02000f8e:
	bl 0x02009bac
	bl 0x02009ca4
	bl 0x02009cb4
	movs r0, #0
	bl 0x02009bcc
	cmp r0, #0
	beq 0x02008fae
	ldr r1, [r0, #8]
.L_02000fa6:
	ldr r2, [r0, #16]
	movs r0, #8
	bl 0x02009bfc
	movs r0, #0
	bl 0x02009bcc
	cmp r0, #0
	beq .L_02000fa6_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #5
	bl 0x02009bfc
.L_02000fa6_0:
	movs r0, #0
	bl 0x02009bcc
	cmp r0, #0
	beq .L_02000fa6_1
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x02009bfc
.L_02000fa6_1:
	movs r0, #8
	ldr r1, [pc, #1016]
.L_02000fda:
	ldr r2, [pc, #1020]
	bl 0x02009bd4
	movs r0, #5
	ldr r1, [pc, #1008]
	ldr r2, [pc, #1008]
	bl 0x02009bd4
	ldr r2, [pc, #1004]
	movs r0, #1
	ldr r1, [pc, #996]
	bl 0x02009bd4
	movs r0, #1
	movs r1, #2
	bl 0x02009c04
	movs r0, #5
	movs r1, #2
	bl 0x02009c04
	movs r0, #8
	movs r1, #2
	bl 0x02009c04
	movs r1, #16
	movs r0, #1
	negs r1, r1
	movs r2, #0
	bl 0x02009bec
	movs r0, #5
	movs r1, #16
	movs r2, #0
	bl 0x02009bec
	movs r2, #16
	negs r2, r2
.L_02001026:
	movs r1, #0
	movs r0, #8
	bl 0x02009bec
	movs r0, #8
	bl 0x02009bf4
	movs r0, #8
	movs r1, #1
	bl 0x02009c04
	movs r0, #0
	movs r1, #0
	bl 0x02009c04
	movs r0, #1
	movs r1, #0
	bl 0x02009c04
	movs r0, #5
	movs r1, #0
	bl 0x02009c04
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009c5c
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009c5c
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #30
	bl 0x02009c5c
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009c5c
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl 0x02009c5c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009c5c
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #30
	bl 0x02009c5c
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009c5c
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009c5c
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x02009c5c
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #30
	bl 0x02009c5c
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009c5c
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009c5c
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009c5c
	movs r1, #192
	movs r2, #40
	movs r0, #8
	lsls r1, r1, #8
	bl 0x02009c5c
	movs r1, #2
	movs r0, #8
	bl 0x02009c24
	movs r0, #10
	bl 0x02009ba4
	movs r0, #8
	movs r1, #2
	bl 0x02009c04
	movs r2, #16
	negs r2, r2
	movs r1, #0
	movs r0, #8
	bl 0x02009bec
	movs r0, #8
	bl 0x02009bf4
	movs r1, #1
	movs r0, #8
	bl 0x02009c04
	movs r0, #6
	bl 0x02009ba4
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #20
	bl 0x02009c5c
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x02009c5c
	movs r1, #192
	movs r2, #40
	movs r0, #8
	lsls r1, r1, #8
	bl 0x02009c5c
	movs r1, #2
	movs r0, #8
	bl 0x02009c24
	movs r0, #20
	bl 0x02009ba4
	movs r0, #8
	movs r1, #2
	bl 0x02009c04
	movs r2, #32
	negs r2, r2
	movs r1, #0
	movs r0, #8
	bl 0x02009bec
	movs r0, #8
	bl 0x02009bf4
	movs r0, #8
	movs r1, #1
	bl 0x02009c04
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x02009c7c
	movs r1, #1
	movs r2, #150
	lsls r2, r2, #16
	movs r3, #1
	negs r1, r1
	ldr r0, [pc, #568]
	bl 0x02009c84
	bl 0x02009c8c
	movs r0, #10
	bl 0x02009ba4
	ldr r0, [pc, #556]
	ldr r1, [pc, #556]
	bl 0x02009c7c
	movs r1, #1
	movs r2, #200
	ldr r0, [pc, #552]
	negs r1, r1
	lsls r2, r2, #15
	movs r3, #1
	bl 0x02009c84
	bl 0x02009c8c
	movs r1, #1
	movs r2, #200
	lsls r2, r2, #15
	movs r3, #1
	ldr r0, [pc, #532]
	negs r1, r1
	bl 0x02009c84
	bl 0x02009c8c
	movs r0, #8
	movs r1, #1
	bl 0x02009c04
	movs r0, #219
	movs r1, #1
	movs r2, #150
	lsls r2, r2, #16
	movs r3, #1
	negs r1, r1
	lsls r0, r0, #19
	bl 0x02009c84
	bl 0x02009c8c
	movs r0, #40
	bl 0x02009ba4
	ldr r0, [pc, #488]
	ldr r1, [pc, #460]
	bl 0x02009c7c
	movs r1, #1
	movs r2, #128
	movs r3, #1
	lsls r2, r2, #17
	ldr r0, [pc, #476]
	negs r1, r1
	bl 0x02009c84
	bl 0x02009c8c
	movs r1, #3
	movs r0, #8
	bl 0x02009c0c
	movs r0, #10
	bl 0x02009ba4
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
.L_02001236:
	movs r2, #0
	bl 0x02009c5c
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #10
	bl 0x02009c5c
	ldr r1, [pc, #428]
	movs r2, #20
	movs r0, #1
	bl 0x02009c6c
	ldr r0, [pc, #424]
	bl 0x02009c34
	movs r0, #1
	movs r1, #0
	movs r2, #10
	bl 0x02009c4c
	movs r1, #129
	movs r2, #60
	movs r0, #8
	lsls r1, r1, #1
	bl 0x02009c6c
	movs r0, #8
	movs r1, #2
	bl 0x02009c1c
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009c4c
	movs r0, #0
	movs r1, #2
	bl 0x02009c1c
	movs r0, #1
	movs r1, #2
	bl 0x02009c1c
	movs r0, #5
	movs r1, #2
	bl 0x02009c1c
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x02009c74
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	bl 0x02009c74
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #5
	bl 0x02009c74
	movs r0, #40
	bl 0x02009ba4
	movs r1, #2
	movs r0, #8
	bl 0x02009c24
	movs r0, #20
	bl 0x02009ba4
	movs r0, #8
	movs r1, #0
	bl 0x02009c44
	movs r0, #8
	movs r1, #4
	bl 0x02009c0c
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl 0x02009c4c
	movs r2, #0
	movs r1, #5
	movs r0, #0
	bl 0x02009c2c
	movs r0, #40
	bl 0x02009ba4
	movs r0, #0
	movs r1, #1
	bl 0x02009c1c
	movs r1, #1
	movs r0, #5
	bl 0x02009c24
	movs r0, #10
	bl 0x02009ba4
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009c5c
	movs r1, #160
	movs r2, #20
	movs r0, #5
	lsls r1, r1, #8
	bl 0x02009c5c
	movs r0, #5
	movs r1, #2
	bl 0x02009c24
	movs r2, #10
	movs r0, #5
	movs r1, #0
	bl 0x02009c4c
	movs r0, #8
	movs r1, #4
	bl 0x02009c0c
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl 0x02009c4c
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #10
	bl 0x02009c5c
	movs r0, #8
	movs r1, #0
	movs r2, #40
	bl 0x02009c4c
	movs r1, #192
	movs r2, #20
	movs r0, #8
	lsls r1, r1, #8
	bl 0x02009c5c
	movs r1, #1
	movs r0, #8
	bl 0x02009c24
	movs r0, #10
	bl 0x02009ba4
	movs r1, #129
	movs r0, #8
	lsls r1, r1, #1
	bl 0x02009c74
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #20
	bl 0x02009c5c
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x02009c5c
	movs r1, #192
	movs r2, #60
	movs r0, #8
	lsls r1, r1, #8
	bl 0x02009c5c
	movs r1, #2
	movs r0, #8
	bl 0x02009c24
	movs r0, #10
	bl 0x02009ba4
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl 0x02009c4c
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #20
	bl 0x02009c5c
	movs r0, #8
	movs r1, #2
	movs r2, #20
	bl 0x02009c14
	b .L_02001236_0
	.2byte 0x0000
	.2byte 0x9999
	.2byte 0x0000
	.2byte 0x4ccc
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0631
	.2byte 0x3333
	.2byte 0x0001
	.2byte 0x2666
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0655
	.2byte 0x0000
	.2byte 0x06b6
	.2byte 0x6666
	.2byte 0x0002
	.2byte 0x0000
	.2byte 0x0684
	.4byte 0x00000101
	.4byte 0x00000fd6
.L_02001236_0:
	movs r0, #8
	movs r1, #0
	movs r2, #40
	bl 0x02009c4c
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x02009c5c
	movs r1, #0
	movs r0, #1
	bl 0x02009c3c
	movs r0, #0
	movs r1, #0
	bl 0x02009bc4
	cmp r0, #0
	bne .L_02001236_1
	ldr r0, [pc, #472]
	bl 0x02009c34
	movs r0, #1
	movs r1, #1
	bl 0x02009c24
	movs r0, #1
	movs r1, #0
	movs r2, #10
	bl 0x02009c4c
	b .L_02001236_2
.L_02001236_1:
	ldr r0, [pc, #452]
	bl 0x02009c34
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #20
	bl 0x02009c5c
	movs r0, #5
	movs r1, #0
	movs r2, #10
	bl 0x02009c4c
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009c5c
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009c5c
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #60
	bl 0x02009c5c
	movs r1, #129
	movs r2, #40
	movs r0, #0
	lsls r1, r1, #1
	bl 0x02009c6c
	movs r1, #1
	movs r0, #1
	bl 0x02009c24
	movs r0, #10
	bl 0x02009ba4
	movs r1, #3
	movs r0, #1
	bl 0x02009c0c
	movs r0, #10
	bl 0x02009ba4
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x02009c5c
	movs r1, #128
	movs r2, #30
	movs r0, #5
	lsls r1, r1, #8
	bl 0x02009c5c
	movs r1, #1
	movs r0, #1
	bl 0x02009c24
	movs r0, #10
	bl 0x02009ba4
	movs r0, #1
	movs r1, #0
	movs r2, #10
	bl 0x02009c4c
.L_02001236_2:
	ldr r2, [pc, #304]
	movs r0, #8
	ldr r1, [pc, #304]
	bl 0x02009bd4
	movs r0, #8
	movs r1, #2
	bl 0x02009c04
	movs r2, #48
	movs r1, #0
	movs r0, #8
	bl 0x02009bec
	movs r0, #8
	bl 0x02009bf4
	movs r1, #1
	movs r0, #8
	bl 0x02009c04
	movs r0, #6
	bl 0x02009ba4
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009c5c
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #5
	bl 0x02009c5c
	movs r0, #8
	bl 0x02009bf4
	movs r1, #1
	movs r0, #8
	bl 0x02009c04
	movs r0, #20
	bl 0x02009ba4
	movs r0, #1
	movs r1, #3
	bl 0x02009c04
	movs r0, #5
	movs r1, #3
	bl 0x02009c04
	movs r1, #3
	movs r0, #0
	bl 0x02009c0c
	movs r0, #6
	bl 0x02009ba4
	movs r0, #1
	movs r1, #2
	bl 0x02009c04
	movs r0, #0
	bl 0x02009bcc
	cmp r0, #0
	beq .L_02001236_3
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x02009bdc
.L_02001236_3:
	movs r0, #5
	movs r1, #2
	bl 0x02009c04
	movs r0, #0
	bl 0x02009bcc
	cmp r0, #0
	beq .L_02001236_4
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl 0x02009bdc
.L_02001236_4:
	movs r0, #8
	movs r1, #2
	bl 0x02009c04
	movs r0, #0
	bl 0x02009bcc
	cmp r0, #0
	beq .L_02001236_5
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #8
	bl 0x02009bdc
.L_02001236_5:
	movs r0, #8
	bl 0x02009bf4
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x02009bfc
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl 0x02009bfc
	movs r2, #0
	movs r0, #8
	movs r1, #0
	bl 0x02009bfc
	movs r0, #8
	movs r1, #1
	bl 0x02009c04
	movs r0, #1
	movs r1, #1
	bl 0x02009c04
	movs r1, #1
	movs r0, #5
	bl 0x02009c04
	ldr r0, [pc, #36]
	bl 0x02009b94
	ldr r0, [pc, #32]
	bl 0x02009b9c
	bl 0x02009bb4
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000fe0
	.4byte 0x00000fe1
	.4byte 0x00004ccc
	.4byte 0x00009999
	.4byte 0x00000804
	.4byte 0x0000012f
	.section .rodata.part1,"a",%progbits
	.global gSoruIriguchiEntrancesOther
gSoruIriguchiEntrancesOther:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSoruIriguchiEntrances2
gSoruIriguchiEntrances2:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000050
	.4byte 0xc0000120
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000078
	.4byte 0x40000070
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSoruIriguchiEntrances1
gSoruIriguchiEntrances1:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000118
	.4byte 0xc00001d8
	.4byte 0x00050000
	.4byte 0x022b0000
	.4byte 0x000001fe
	.4byte 0xffff0002
	.4byte 0x00000048
	.4byte 0x40000038
	.4byte 0x00050000
	.4byte 0x022b0000
	.4byte 0x000001fe
	.4byte 0xffff0003
	.4byte 0x000002e8
	.4byte 0xc0000098
	.4byte 0x026c0000
	.4byte 0x035c0000
	.4byte 0x000000c3
	.4byte 0xffff0004
	.4byte 0x000002b8
	.4byte 0xc0000238
	.4byte 0x023f0000
	.4byte 0x032f0109
	.4byte 0x00000262
	.4byte 0xffff0005
	.4byte 0x000002b8
	.4byte 0x40000148
	.4byte 0x023f0000
	.4byte 0x032f0109
	.4byte 0x00000262
	.4byte 0xffff0006
	.4byte 0x000003d8
	.4byte 0xc0000098
	.4byte 0x035c0000
	.4byte 0x044c0000
	.4byte 0x000000c3
	.4byte 0xffff0007
	.4byte 0x000003d8
	.4byte 0x40000038
	.4byte 0x035c0000
	.4byte 0x044c0000
	.4byte 0x000000c3
	.4byte 0xffff0008
	.4byte 0x000004c8
	.4byte 0xc00000b8
	.4byte 0x044c0000
	.4byte 0x053c0014
	.4byte 0x000000dc
	.4byte 0xffff0009
	.4byte 0x000004c8
	.4byte 0x40000068
	.4byte 0x044c0000
	.4byte 0x053c0014
	.4byte 0x000000dc
	.4byte 0xffff000a
	.4byte 0x00000528
	.4byte 0xc00001c8
	.4byte 0x03890000
	.4byte 0x0555011d
	.4byte 0x000001ea
	.4byte 0xffff000b
	.4byte 0x00000688
	.4byte 0xc0000118
	.4byte 0x05af0000
	.4byte 0x07620000
	.4byte 0x00000136
	.4byte 0xffff000c
	.4byte 0x000005e8
	.4byte 0x40000088
	.4byte 0x05af0000
	.4byte 0x07620000
	.4byte 0x00000136
	.4byte 0xffff000d
	.4byte 0x00000728
	.4byte 0x40000088
	.4byte 0x05af0000
	.4byte 0x07620000
	.4byte 0x00000136
	.4byte 0xffff000e
	.4byte 0x000006e8
	.4byte 0xc0000238
	.4byte 0x06040000
	.4byte 0x07cb01b8
	.4byte 0x00000258
	.4byte 0xffff000f
	.4byte 0x00000688
	.4byte 0x40000208
	.4byte 0x06040000
	.4byte 0x07cb01b8
	.4byte 0x00000258
	.4byte 0xffff0010
	.4byte 0x00000788
	.4byte 0x40000208
	.4byte 0x06040000
	.4byte 0x07cb01b8
	.4byte 0x00000258
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global SoruIriguchi_Exits
SoruIriguchi_Exits:
	.4byte 0x00000013
	.4byte 0x10102004
	.4byte 0x00000814
	.4byte 0x1010f00a
	.4byte 0x00000815
	.4byte 0x10102004
	.4byte 0xffffffff
	.4byte 0x10208010
	.4byte 0xffffffff
	.4byte 0x00000010
	.4byte 0x1010200f
	.4byte 0xffffffff
	.4byte 0x1020b010
	.4byte 0xffffffff
	.4byte 0x1030c010
	.4byte 0xffffffff
	.4byte 0x10409010
	.4byte 0xffffffff
	.4byte 0x1050100f
	.4byte 0xffffffff
	.4byte 0x1060d010
	.4byte 0xffffffff
	.4byte 0x1070100e
	.4byte 0xffffffff
	.4byte 0x10802013
	.4byte 0xffffffff
	.4byte 0x10904010
	.4byte 0xffffffff
	.4byte 0x10a0f010
	.4byte 0xffffffff
	.4byte 0x10b02010
	.4byte 0xffffffff
	.4byte 0x10c03010
	.4byte 0xffffffff
	.4byte 0x10d06010
	.4byte 0xffffffff
	.4byte 0x10e0200e
	.4byte 0xffffffff
	.4byte 0x10f0a010
	.4byte 0xffffffff
	.4byte 0x1100100b
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global gSoruIriguchiPlacementsOther
gSoruIriguchiPlacementsOther:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSoruIriguchiPlacements1
gSoruIriguchiPlacements1:
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
	.4byte 0xffff001e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0x000000cb
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x00380000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSoruIriguchiPlacements1Entrances11To13
gSoruIriguchiPlacements1Entrances11To13:
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
	.4byte 0xffff001e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x05e80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x07280000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x01024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x06180000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x01024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x06f80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x01024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x06680000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x06680000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x06680000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x01024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x06880000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x01024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x06a80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x01024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x06a80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x06a80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSoruIriguchiPlacements1Entrances14To16
gSoruIriguchiPlacements1Entrances14To16:
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
	.4byte 0xffff001e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x06480000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x06880000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x01024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x06c80000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x01024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x07080000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x01024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x07480000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x01024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x07880000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSoruIriguchiPlacements2
gSoruIriguchiPlacements2:
	.4byte 0xffff007a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00005000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSoruIriguchiEventsOther
gSoruIriguchiEventsOther:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSoruIriguchiEvents2
gSoruIriguchiEvents2:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008155
	.4byte 0x00000003
	.4byte 0xffff000a
	.4byte 0x02009aad
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0000111d
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000111e
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSoruIriguchiEvents1
gSoruIriguchiEvents1:
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
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000021
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000013
	.4byte 0x0f010064
	.4byte 0x001000e1
	.4byte 0x0000e104
	.4byte 0xffff0414
	.4byte 0x02008259
	.4byte 0x00000003
	.4byte 0xffff0014
	.4byte 0x02008201
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSoruIriguchiEvents1Entrances11To13
gSoruIriguchiEvents1Entrances11To13:
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
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000021
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00008c15
	.4byte 0x02340009
	.4byte 0x0200856d
	.4byte 0x00008c15
	.4byte 0x0234000a
	.4byte 0x020085ad
	.4byte 0x00000202
	.4byte 0xffff0016
	.4byte 0x020087d1
	.4byte 0x00000602
	.4byte 0xffff0017
	.4byte 0x020087d1
	.4byte 0x00008602
	.4byte 0xffff0019
	.4byte 0x020087d1
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSoruIriguchiEvents1Entrances14To16
gSoruIriguchiEvents1Entrances14To16:
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
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000021
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00008c15
	.4byte 0xffff0009
	.4byte 0x020085ed
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte 0x02008635
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte 0x0200867d
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte 0x020086c5
	.4byte 0x00008c15
	.4byte 0xffff000d
	.4byte 0x0200870d
	.4byte 0x00008c15
	.4byte 0xffff000e
	.4byte 0x02008755
	.4byte 0x00000202
	.4byte 0xffff0018
	.4byte 0x020087d1
	.4byte 0x0000e104
	.4byte 0xffff0415
	.4byte 0x02008421
	.4byte 0x00000003
	.4byte 0xffff0015
	.4byte 0x020083bd
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSoruPushSteps
gSoruPushSteps:
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
