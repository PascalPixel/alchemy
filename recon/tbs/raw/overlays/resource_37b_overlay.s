.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200a5c0
	.global Func_02000038
	.thumb_func
Func_02000038:
	movs r0, #0
	bx lr
	.global Func_0200003c
	.thumb_func
Func_0200003c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200a698
	.global Func_02000044
	.thumb_func
Func_02000044:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200a6bc
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200a80c
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {lr}
	ldr r0, [pc, #52]
	bl 0x0200a434
	cmp r0, #0
	beq .L_02000054_0
	ldr r0, [pc, #44]
	bl 0x0200a434
	cmp r0, #0
	bne .L_02000054_1
	movs r0, #3
	b .L_02000054_2
.L_02000054_0:
	ldr r0, [pc, #36]
	bl 0x0200a434
	cmp r0, #0
	bne .L_02000054_1
	movs r0, #4
.L_02000054_2:
	bl 0x0200a52c
	movs r0, #1
	b .L_02000054_3
.L_02000054_1:
	movs r0, #1
	negs r0, r0
.L_02000054_3:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x00000818
	.4byte 0x00000813
	.4byte 0x00000812
	.global Func_02000098
	.thumb_func
Func_02000098:
	push {r5, r6, r7, lr}
	sub sp, #8
	movs r3, #2
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #28
	movs r2, #17
	movs r3, #8
	bl 0x0200a404
	movs r0, #200
	bl 0x0200a564
	movs r5, #0
	movs r7, #2
	movs r6, #1
.L_02000098_0:
	movs r1, #61
	movs r2, #17
	movs r3, #40
	movs r0, #10
	str r7, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200a404
	movs r0, #4
	bl 0x0200a44c
	movs r0, #8
	movs r1, #61
	movs r2, #17
	movs r3, #40
	str r7, [sp, #0]
	str r6, [sp, #4]
	adds r5, #1
	bl 0x0200a404
	movs r0, #4
	bl 0x0200a44c
	cmp r5, #22
	bne .L_02000098_0
	movs r5, #4
	movs r6, #3
	movs r0, #0
	movs r1, #59
	movs r2, #15
	movs r3, #38
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200a404
	movs r0, #4
	movs r1, #59
	movs r2, #17
	movs r3, #38
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200a404
	movs r3, #2
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #8
	movs r1, #60
	movs r2, #17
	movs r3, #39
	bl 0x0200a404
	movs r3, #17
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #0
	movs r2, #2
	movs r3, #1
	movs r0, #0
	bl 0x0200a40c
	ldr r0, [pc, #16]
	bl 0x0200a43c
	bl 0x020096dc
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000207
	.global Func_02000150
	.thumb_func
Func_02000150:
	push {r5, r6, r7, lr}
	bl 0x0200a454
	ldr r7, [pc, #1020]
	movs r6, #224
	ldr r2, [r7]
	movs r3, #128
	lsls r6, r6, #1
	lsls r3, r3, #1
	movs r5, #228
	str r3, [r2, r6]
	lsls r5, r5, #1
	movs r3, #32
	str r3, [r2, r5]
	bl 0x0200a544
	bl 0x0200a554
	movs r0, #20
	bl 0x0200a44c
	movs r1, #144
	movs r2, #232
	lsls r2, r2, #16
	lsls r1, r1, #18
	movs r0, #8
	bl 0x0200a4a4
	movs r0, #1
	bl 0x0200a44c
	ldr r0, [pc, #968]
	bl 0x0200a4dc
	movs r0, #8
	movs r1, #6
	bl 0x0200a3a4
	movs r1, #144
	movs r2, #140
	lsls r2, r2, #17
	movs r0, #8
	lsls r1, r1, #18
	bl 0x0200a4a4
	ldr r0, [pc, #944]
	ldr r1, [pc, #944]
	bl 0x0200a514
	movs r1, #1
	movs r2, #180
	movs r3, #1
	ldr r0, [pc, #936]
	negs r1, r1
	lsls r2, r2, #16
	bl 0x0200a51c
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200a474
	movs r1, #144
	lsls r1, r1, #2
	movs r2, #216
	movs r0, #8
	bl 0x0200a494
	movs r0, #20
	bl 0x0200a44c
	movs r2, #0
	movs r1, #2
	movs r0, #5
	bl 0x0200a4bc
	movs r0, #30
	bl 0x0200a44c
	movs r0, #5
	movs r1, #6
	bl 0x0200a3a4
	movs r1, #2
	movs r0, #8
	bl 0x0200a4c4
	movs r0, #6
	bl 0x0200a44c
	movs r1, #144
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #8
	bl 0x0200a4f4
	movs r0, #10
	bl 0x0200a44c
	ldr r0, [pc, #844]
	ldr r1, [pc, #844]
	bl 0x0200a514
	movs r1, #1
	movs r2, #176
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	ldr r0, [pc, #832]
	bl 0x0200a51c
	bl 0x0200a524
	movs r0, #60
	bl 0x0200a44c
	movs r1, #1
	movs r2, #180
	movs r3, #1
	lsls r2, r2, #16
	negs r1, r1
	ldr r0, [pc, #796]
	bl 0x0200a51c
	bl 0x0200a524
	movs r0, #20
	bl 0x0200a44c
	movs r1, #3
	movs r0, #8
	bl 0x0200a4b4
	movs r0, #10
	bl 0x0200a44c
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl 0x0200a4f4
	movs r0, #10
	bl 0x0200a44c
	movs r0, #8
	movs r1, #6
	movs r2, #0
	bl 0x0200a4bc
	movs r1, #192
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #10
	lsls r2, r2, #10
	bl 0x0200a474
	movs r1, #144
	movs r2, #184
	lsls r1, r1, #2
	movs r0, #8
	bl 0x0200a494
	movs r0, #40
	bl 0x0200a44c
	movs r0, #8
	movs r1, #6
	bl 0x0200a3a4
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl 0x0200a4f4
	movs r0, #40
	bl 0x0200a44c
	ldr r2, [r7]
	ldr r3, [pc, #692]
	str r3, [r2, r6]
	movs r6, #16
	str r6, [r2, r5]
	bl 0x0200a54c
	bl 0x0200a554
	movs r1, #1
	movs r2, #176
	movs r3, #0
	negs r1, r1
	lsls r2, r2, #16
	ldr r0, [pc, #664]
	bl 0x0200a51c
	bl 0x0200a3fc
	movs r0, #1
	bl 0x0200a3bc
	bl 0x0200a544
	bl 0x0200a554
	movs r0, #40
	bl 0x0200a44c
	movs r1, #212
	movs r2, #200
	lsls r2, r2, #16
	lsls r1, r1, #17
	movs r0, #8
	bl 0x0200a4a4
	movs r0, #1
	bl 0x0200a3bc
	movs r0, #8
	movs r1, #20
	bl 0x0200a3a4
	bl 0x0200a54c
	bl 0x0200a554
	movs r1, #144
	movs r2, #184
	movs r0, #8
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl 0x0200a4a4
	movs r1, #1
	movs r2, #180
	lsls r2, r2, #16
	movs r3, #0
	negs r1, r1
	ldr r0, [pc, #560]
	bl 0x0200a51c
	bl 0x0200a3fc
	movs r0, #1
	bl 0x0200a3bc
	bl 0x0200a544
	bl 0x0200a554
	movs r0, #20
	bl 0x0200a44c
	ldr r0, [pc, #552]
	ldr r1, [pc, #552]
	bl 0x0200a514
	movs r1, #1
	movs r2, #157
	movs r3, #1
	lsls r2, r2, #16
	negs r1, r1
	ldr r0, [pc, #512]
	bl 0x0200a51c
	bl 0x0200a524
	movs r0, #20
	bl 0x0200a44c
	movs r0, #1
	movs r1, #2
	bl 0x0200a4c4
	movs r0, #1
	movs r1, #20
	bl 0x0200a3a4
	movs r0, #5
	movs r1, #3
	bl 0x0200a4b4
	movs r0, #5
	movs r1, #6
	bl 0x0200a3a4
	movs r1, #2
	movs r0, #8
	bl 0x0200a4c4
	movs r0, #40
	bl 0x0200a44c
	movs r0, #8
	movs r1, #4
	bl 0x0200a4b4
	movs r0, #8
	movs r1, #80
	bl 0x0200a3a4
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #8
	bl 0x0200a504
	movs r0, #60
	bl 0x0200a44c
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #8
	bl 0x0200a4f4
	movs r0, #30
	bl 0x0200a44c
	movs r0, #8
	movs r1, #20
	bl 0x0200a3a4
	movs r0, #0
	ldr r1, [pc, #416]
	movs r2, #0
	bl 0x0200a504
	movs r0, #1
	ldr r1, [pc, #408]
	movs r2, #0
	bl 0x0200a504
	ldr r1, [pc, #400]
	movs r2, #0
	movs r0, #5
	bl 0x0200a504
	movs r0, #60
	bl 0x0200a44c
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #8
	bl 0x0200a4f4
	movs r0, #30
	bl 0x0200a44c
	movs r0, #8
	movs r1, #4
	bl 0x0200a4b4
	movs r0, #8
	movs r1, #6
	bl 0x0200a3a4
	movs r1, #1
	movs r2, #0
	movs r0, #0
	bl 0x0200a4d4
	movs r0, #40
	bl 0x0200a44c
	movs r2, #0
	movs r1, #5
	movs r0, #0
	bl 0x0200a4d4
	movs r0, #40
	bl 0x0200a44c
	movs r0, #8
	movs r1, #3
	bl 0x0200a4b4
	movs r0, #8
	movs r1, #6
	bl 0x0200a3a4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200a4f4
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200a4f4
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #1
	bl 0x0200a4f4
	movs r0, #40
	bl 0x0200a44c
	movs r0, #1
	movs r1, #3
	bl 0x0200a4ac
	movs r0, #5
	movs r1, #3
	bl 0x0200a4ac
	movs r1, #3
	movs r0, #0
	bl 0x0200a4b4
	movs r0, #20
	bl 0x0200a44c
	movs r0, #8
	movs r1, #3
	bl 0x0200a4b4
	movs r0, #8
	movs r1, #6
	bl 0x0200a3a4
	ldr r0, [pc, #216]
	ldr r1, [pc, #220]
	bl 0x0200a514
	movs r0, #144
	movs r1, #1
	movs r2, #215
	movs r3, #1
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #16
	bl 0x0200a51c
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #8
	bl 0x0200a4f4
	movs r0, #10
	bl 0x0200a44c
	movs r0, #8
	movs r1, #6
	movs r2, #0
	bl 0x0200a4bc
	movs r1, #144
	lsls r1, r1, #2
	movs r2, #217
	movs r0, #8
	bl 0x0200a494
	movs r0, #20
	bl 0x0200a44c
	movs r1, #144
	movs r0, #8
	lsls r1, r1, #2
	ldr r2, [pc, #148]
	bl 0x0200a494
	movs r2, #0
	movs r0, #8
	movs r1, #0
	bl 0x0200a4a4
	ldr r0, [pc, #136]
	ldr r1, [pc, #136]
	bl 0x0200a514
	movs r0, #144
	movs r1, #1
	movs r2, #136
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #18
	bl 0x0200a51c
	bl 0x0200a524
	movs r0, #20
	bl 0x0200a44c
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200a474
	movs r1, #128
	movs r2, #128
	movs r0, #5
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200a474
	movs r0, #1
	movs r1, #2
	bl 0x0200a4ac
	movs r0, #0
	b .L_02000150_0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x0000101a
	.4byte 0x0000cccc
	.4byte 0x00001999
	.4byte 0x023e0000
	.4byte 0x00059999
	.4byte 0x0000b333
	.4byte 0x011f0000
	.4byte 0x00000202
	.4byte 0x00013333
	.4byte 0x00002666
	.4byte 0x00000101
	.4byte 0x00019999
	.4byte 0x00003333
	.4byte 0x00000141
	.4byte 0x00039999
	.4byte 0x00007333
.L_02000150_0:
	bl 0x0200a46c
	cmp r0, #0
	beq .L_02000150_1
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x0200a47c
.L_02000150_1:
	movs r0, #1
	bl 0x0200a49c
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200a4a4
	movs r0, #5
	movs r1, #2
	bl 0x0200a4ac
	movs r0, #0
	bl 0x0200a46c
	cmp r0, #0
	beq .L_02000150_2
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl 0x0200a47c
.L_02000150_2:
	movs r0, #5
	bl 0x0200a49c
	movs r1, #0
	movs r2, #0
	movs r0, #5
	bl 0x0200a4a4
	ldr r0, [pc, #32]
	bl 0x0200a444
	ldr r1, [r7]
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r1, r2
	adds r2, #68
	str r2, [r3]
	subs r2, #60
	adds r3, r1, r2
	str r6, [r3]
	bl 0x0200a45c
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0000012f
	.global Func_02000614
	.thumb_func
Func_02000614:
	push {lr}
	bl 0x0200a454
	ldr r3, [pc, #1020]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	subs r3, #192
	str r3, [r2]
	adds r3, #200
	adds r2, r1, r3
	movs r3, #32
	str r3, [r2]
	bl 0x0200a544
	bl 0x0200a554
	movs r0, #20
	bl 0x0200a44c
	movs r1, #144
	movs r2, #148
	lsls r2, r2, #17
	lsls r1, r1, #18
	movs r0, #8
	bl 0x0200a4a4
	movs r0, #1
	bl 0x0200a44c
	ldr r0, [pc, #968]
	bl 0x0200a4dc
	movs r0, #8
	movs r1, #6
	bl 0x0200a3a4
	ldr r0, [pc, #956]
	ldr r1, [pc, #960]
	bl 0x0200a514
	movs r1, #1
	movs r2, #180
	movs r3, #1
	ldr r0, [pc, #952]
	negs r1, r1
	lsls r2, r2, #16
	bl 0x0200a51c
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200a474
	movs r1, #144
	lsls r1, r1, #2
	movs r2, #216
	movs r0, #8
	bl 0x0200a494
	movs r0, #20
	bl 0x0200a44c
	movs r2, #0
	movs r1, #2
	movs r0, #5
	bl 0x0200a4bc
	movs r0, #30
	bl 0x0200a44c
	movs r0, #5
	movs r1, #6
	bl 0x0200a3a4
	movs r1, #2
	movs r0, #8
	bl 0x0200a4c4
	movs r0, #6
	bl 0x0200a44c
	movs r1, #144
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #8
	bl 0x0200a4f4
	movs r0, #10
	bl 0x0200a44c
	ldr r0, [pc, #856]
	ldr r1, [pc, #860]
	bl 0x0200a514
	movs r1, #1
	movs r2, #176
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	ldr r0, [pc, #848]
	bl 0x0200a51c
	bl 0x0200a524
	movs r0, #60
	bl 0x0200a44c
	movs r1, #1
	movs r2, #180
	movs r3, #1
	lsls r2, r2, #16
	negs r1, r1
	ldr r0, [pc, #808]
	bl 0x0200a51c
	bl 0x0200a524
	movs r0, #20
	bl 0x0200a44c
	movs r1, #3
	movs r0, #8
	bl 0x0200a4b4
	movs r0, #10
	bl 0x0200a44c
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl 0x0200a4f4
	movs r0, #10
	bl 0x0200a44c
	movs r0, #8
	movs r1, #6
	movs r2, #0
	bl 0x0200a4bc
	movs r1, #192
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #10
	lsls r2, r2, #10
	bl 0x0200a474
	movs r1, #144
	movs r2, #184
	lsls r1, r1, #2
	movs r0, #8
	bl 0x0200a494
	movs r0, #40
	bl 0x0200a44c
	movs r0, #8
	movs r1, #3
	bl 0x0200a4b4
	movs r0, #8
	movs r1, #6
	bl 0x0200a3a4
	movs r0, #1
	movs r1, #3
	bl 0x0200a4ac
	movs r0, #5
	movs r1, #3
	bl 0x0200a4ac
	movs r1, #3
	movs r0, #0
	bl 0x0200a4b4
	movs r0, #40
	bl 0x0200a44c
	movs r0, #8
	movs r1, #3
	bl 0x0200a4c4
	movs r0, #8
	movs r1, #6
	bl 0x0200a3a4
	movs r0, #0
	movs r1, #2
	movs r2, #0
	bl 0x0200a4bc
	movs r0, #1
	movs r1, #2
	movs r2, #0
	bl 0x0200a4bc
	movs r2, #0
	movs r1, #2
	movs r0, #5
	bl 0x0200a4bc
	movs r0, #30
	bl 0x0200a44c
	movs r0, #1
	movs r1, #2
	bl 0x0200a4c4
	movs r0, #1
	movs r1, #6
	bl 0x0200a3a4
	movs r0, #8
	movs r1, #1
	bl 0x0200a4c4
	movs r0, #8
	movs r1, #4
	bl 0x0200a4b4
	movs r0, #8
	movs r1, #6
	bl 0x0200a3a4
	movs r1, #5
	movs r2, #0
	movs r0, #0
	bl 0x0200a4d4
	movs r0, #40
	bl 0x0200a44c
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x0200a4cc
	movs r1, #5
	movs r2, #0
	movs r0, #8
	bl 0x0200a4cc
	movs r0, #40
	bl 0x0200a44c
	movs r0, #8
	movs r1, #6
	movs r2, #0
	bl 0x0200a4bc
	movs r1, #144
	movs r0, #8
	lsls r1, r1, #2
	movs r2, #216
	bl 0x0200a494
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl 0x0200a4f4
	movs r0, #10
	bl 0x0200a44c
	movs r0, #8
	ldr r1, [pc, #512]
	ldr r2, [pc, #516]
	bl 0x0200a474
	movs r1, #216
	movs r2, #200
	lsls r1, r1, #1
	movs r0, #8
	bl 0x0200a48c
	movs r0, #20
	bl 0x0200a44c
	ldr r0, [pc, #496]
	ldr r1, [pc, #496]
	bl 0x0200a514
	movs r0, #144
	movs r1, #1
	movs r2, #171
	lsls r2, r2, #16
	movs r3, #1
	negs r1, r1
	lsls r0, r0, #17
	bl 0x0200a51c
	bl 0x0200a524
	movs r0, #80
	bl 0x0200a44c
	movs r0, #8
	movs r1, #1
	bl 0x0200a4ac
	movs r1, #1
	movs r2, #180
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #16
	ldr r0, [pc, #416]
	bl 0x0200a51c
	movs r0, #20
	bl 0x0200a44c
	movs r1, #0
	movs r2, #0
	movs r0, #8
	bl 0x0200a4f4
	movs r0, #30
	bl 0x0200a44c
	movs r1, #144
	movs r0, #8
	lsls r1, r1, #2
	movs r2, #216
	bl 0x0200a494
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl 0x0200a4f4
	movs r0, #10
	bl 0x0200a44c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200a4f4
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200a4f4
	movs r1, #128
	movs r2, #0
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200a4f4
	ldr r0, [pc, #316]
	ldr r1, [pc, #316]
	bl 0x0200a514
	movs r1, #1
	movs r2, #171
	movs r3, #1
	ldr r0, [pc, #308]
	negs r1, r1
	lsls r2, r2, #16
	bl 0x0200a51c
	movs r0, #8
	movs r1, #6
	movs r2, #0
	bl 0x0200a4bc
	movs r1, #192
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #10
	lsls r2, r2, #10
	bl 0x0200a474
	movs r1, #144
	movs r2, #184
	lsls r1, r1, #2
	movs r0, #8
	bl 0x0200a494
	movs r0, #80
	bl 0x0200a44c
	movs r0, #8
	movs r1, #6
	bl 0x0200a3a4
	movs r0, #8
	movs r1, #4
	bl 0x0200a4b4
	movs r0, #8
	movs r1, #20
	bl 0x0200a3a4
	movs r1, #129
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #5
	bl 0x0200a504
	movs r0, #40
	bl 0x0200a44c
	movs r0, #5
	movs r1, #2
	bl 0x0200a4c4
	movs r0, #5
	movs r1, #6
	bl 0x0200a3a4
	movs r0, #8
	movs r1, #3
	bl 0x0200a4b4
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl 0x0200a4f4
	movs r0, #40
	bl 0x0200a44c
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #8
	bl 0x0200a4f4
	movs r0, #30
	bl 0x0200a44c
	movs r0, #8
	movs r1, #6
	bl 0x0200a3a4
	movs r0, #0
	ldr r1, [pc, #176]
	movs r2, #0
	bl 0x0200a504
	movs r0, #1
	ldr r1, [pc, #168]
	movs r2, #0
	bl 0x0200a504
	movs r2, #0
	ldr r1, [pc, #156]
	movs r0, #5
	bl 0x0200a504
	movs r0, #60
	bl 0x0200a44c
	movs r0, #8
	movs r1, #4
	bl 0x0200a4b4
	movs r0, #8
	movs r1, #6
	bl 0x0200a3a4
	movs r1, #0
	movs r2, #0
	movs r0, #1
	bl 0x0200a4d4
	movs r0, #40
	bl 0x0200a44c
	movs r2, #0
	movs r1, #0
	movs r0, #5
	bl 0x0200a4d4
	movs r0, #40
	bl 0x0200a44c
	movs r1, #0
	movs r0, #8
	bl 0x0200a4e4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200a4f4
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200a4f4
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200a4f4
	movs r0, #0
	b .L_02000614_0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00001004
	.4byte 0x0000cccc
	.4byte 0x00001999
	.4byte 0x023e0000
	.4byte 0x00059999
	.4byte 0x0000b333
	.4byte 0x011f0000
	.4byte 0x00013333
	.4byte 0x00009999
	.4byte 0x00026666
	.4byte 0x00004ccc
	.4byte 0x00000101
.L_02000614_0:
	movs r1, #0
	bl 0x0200a464
	cmp r0, #0
	bne .L_02000614_1
	ldr r0, [pc, #516]
	bl 0x0200a4dc
	b .L_02000614_2
.L_02000614_1:
	ldr r0, [pc, #512]
	bl 0x0200a4dc
.L_02000614_2:
	movs r1, #6
	movs r0, #8
	bl 0x0200a3a4
	ldr r0, [pc, #500]
	bl 0x0200a4dc
	movs r0, #8
	movs r1, #2
	bl 0x0200a4c4
	movs r0, #8
	movs r1, #6
	bl 0x0200a3a4
	movs r1, #129
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200a504
	movs r0, #60
	bl 0x0200a44c
	movs r0, #1
	movs r1, #6
	bl 0x0200a3a4
	movs r0, #8
	movs r1, #3
	bl 0x0200a4b4
	movs r0, #8
	movs r1, #6
	bl 0x0200a3a4
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #8
	bl 0x0200a4f4
	movs r0, #20
	bl 0x0200a44c
	movs r0, #8
	movs r1, #6
	movs r2, #0
	bl 0x0200a4bc
	movs r1, #144
	movs r2, #216
	lsls r1, r1, #2
	movs r0, #8
	bl 0x0200a494
	movs r0, #40
	bl 0x0200a44c
	ldr r0, [pc, #396]
	ldr r1, [pc, #396]
	bl 0x0200a514
	movs r1, #1
	movs r2, #191
	movs r3, #1
	ldr r0, [pc, #388]
	negs r1, r1
	lsls r2, r2, #16
	bl 0x0200a51c
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200a474
	movs r1, #144
	movs r2, #232
	lsls r1, r1, #2
	movs r0, #8
	bl 0x0200a494
	movs r0, #40
	bl 0x0200a44c
	movs r1, #2
	movs r0, #8
	bl 0x0200a4c4
	movs r0, #40
	bl 0x0200a44c
	movs r0, #8
	movs r1, #6
	bl 0x0200a3a4
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #8
	bl 0x0200a4f4
	movs r0, #30
	bl 0x0200a44c
	movs r0, #8
	movs r1, #6
	bl 0x0200a3a4
	movs r0, #8
	movs r1, #3
	bl 0x0200a4b4
	movs r0, #144
	movs r1, #1
	movs r2, #215
	movs r3, #1
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #16
	bl 0x0200a51c
	movs r0, #8
	ldr r1, [pc, #276]
	ldr r2, [pc, #276]
	bl 0x0200a494
	movs r2, #0
	movs r0, #8
	movs r1, #0
	bl 0x0200a4a4
	ldr r0, [pc, #264]
	ldr r1, [pc, #268]
	bl 0x0200a514
	movs r0, #144
	movs r1, #1
	movs r2, #136
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #18
	bl 0x0200a51c
	bl 0x0200a524
	movs r0, #20
	bl 0x0200a44c
	movs r2, #0
	movs r1, #0
	movs r0, #5
	bl 0x0200a4f4
	movs r0, #10
	bl 0x0200a44c
	movs r0, #5
	movs r1, #6
	bl 0x0200a3a4
	movs r0, #1
	movs r1, #3
	bl 0x0200a4ac
	movs r0, #5
	movs r1, #3
	bl 0x0200a4b4
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200a474
	movs r1, #128
	movs r2, #128
	movs r0, #5
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200a474
	movs r0, #5
	movs r1, #2
	bl 0x0200a4ac
	movs r0, #0
	bl 0x0200a46c
	cmp r0, #0
	beq .L_02000614_3
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl 0x0200a47c
.L_02000614_3:
	movs r0, #5
	bl 0x0200a49c
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl 0x0200a4a4
	movs r0, #1
	movs r1, #2
	bl 0x0200a4ac
	movs r0, #0
	bl 0x0200a46c
	cmp r0, #0
	beq .L_02000614_4
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x0200a47c
.L_02000614_4:
	movs r0, #1
	bl 0x0200a49c
	movs r1, #0
	movs r2, #0
	movs r0, #1
	bl 0x0200a4a4
	ldr r0, [pc, #72]
	bl 0x0200a444
	ldr r3, [pc, #72]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #68
	str r3, [r2]
	subs r3, #60
	adds r2, r1, r3
	movs r3, #16
	str r3, [r2]
	bl 0x0200a45c
	pop {r0}
	bx r0
	.4byte 0x00001010
	.4byte 0x00001011
	.4byte 0x00001012
	.4byte 0x0000cccc
	.4byte 0x00001999
	.4byte 0x023e0000
	.4byte 0x0000023e
	.4byte 0x00000143
	.4byte 0x00039999
	.4byte 0x00007333
	.4byte 0x0000012f
	.4byte 0x03001ebc
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #516]
	ldr	r3, [r3, #0]
	movs	r0, #0
	mov	sl, r3
	sub	sp, #20
	bl 0x0200a46c
	adds	r5, r0, #0
	movs	r2, #179
	ldr	r3, [r5, #16]
	lsls	r2, r2, #16
	cmp	r3, r2
	bge.n	.L_02000d0c
	movs	r0, #0
	ldr	r1, [pc, #492]
	movs	r2, #132
	bl 0x0200a494
	movs	r1, #128
	movs	r2, #0
	movs	r0, #0
	lsls	r1, r1, #7
	bl 0x0200a4f4
	movs	r0, #30
	bl 0x0200a44c
	mov	r3, sl
	ldr	r3, [r3, #0]
	mov	fp, r3
	ldr	r3, [r5, #8]
	add	r7, sp, #8
	str	r3, [r7, #0]
	ldr	r3, [r5, #12]
	str	r3, [r7, #4]
	ldr	r3, [r5, #16]
	mov	r2, sl
	str	r3, [r7, #8]
	str	r7, [r2, #0]
	movs	r6, #0
	adds	r5, r7, #0
.L_02000cec:
	ldr	r3, [r5, #8]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r3, r2
	str	r3, [r5, #8]
	movs	r0, #1
	adds	r6, #1
	bl 0x0200a44c
	cmp	r6, #30
	bne.n	.L_02000cec
	movs	r0, #40
	bl 0x0200a44c
	movs	r3, #1
	b.n	.L_02000d60
.L_02000d0c:
	movs	r0, #0
	ldr	r1, [pc, #408]
	movs	r2, #222
	bl 0x0200a494
	movs	r1, #192
	movs	r2, #0
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200a4f4
	movs	r0, #30
	bl 0x0200a44c
	ldr	r3, [r5, #8]
	add	r7, sp, #8
	str	r3, [r7, #0]
	ldr	r3, [r5, #12]
	str	r3, [r7, #4]
	ldr	r3, [r5, #16]
	mov	r2, sl
	ldr	r2, [r2, #0]
	str	r3, [r7, #8]
	mov	r3, sl
	str	r7, [r3, #0]
	mov	fp, r2
	movs	r6, #0
	adds	r5, r7, #0
.L_02000d44:
	ldr	r3, [r5, #8]
	ldr	r2, [pc, #356]
	adds	r3, r3, r2
	str	r3, [r5, #8]
	movs	r0, #1
	adds	r6, #1
	bl 0x0200a44c
	cmp	r6, #30
	bne.n	.L_02000d44
	movs	r0, #40
	bl 0x0200a44c
	movs	r3, #2
.L_02000d60:
	mov	r9, r3
	movs	r2, #4
	movs	r6, #0
	mov	r8, r2
	movs	r5, #2
.L_02000d6a:
	mov	r3, r8
	str	r3, [sp, #0]
	movs	r1, #28
	movs	r2, #34
	movs	r3, #10
	movs	r0, #2
	str	r5, [sp, #4]
	bl 0x0200a404
	movs	r0, #8
	bl 0x0200a44c
	mov	r2, r8
	str	r2, [sp, #0]
	movs	r0, #2
	movs	r1, #30
	movs	r2, #34
	movs	r3, #10
	str	r5, [sp, #4]
	adds	r6, #1
	bl 0x0200a404
	movs	r0, #8
	bl 0x0200a44c
	cmp	r6, #6
	bne.n	.L_02000d6a
	movs	r3, #4
	movs	r6, #0
	mov	r8, r3
	movs	r5, #2
.L_02000da8:
	mov	r2, r8
	str	r2, [sp, #0]
	movs	r1, #28
	movs	r2, #34
	movs	r3, #10
	movs	r0, #2
	str	r5, [sp, #4]
	bl 0x0200a404
	movs	r0, #4
	bl 0x0200a44c
	mov	r3, r8
	str	r3, [sp, #0]
	movs	r0, #2
	movs	r1, #30
	movs	r2, #34
	movs	r3, #10
	str	r5, [sp, #4]
	adds	r6, #1
	bl 0x0200a404
	movs	r0, #4
	bl 0x0200a44c
	cmp	r6, #10
	bne.n	.L_02000da8
	movs	r2, #4
	movs	r6, #0
	mov	r8, r2
	movs	r5, #2
.L_02000de6:
	mov	r3, r8
	str	r3, [sp, #0]
	movs	r1, #28
	movs	r2, #34
	movs	r3, #10
	movs	r0, #2
	str	r5, [sp, #4]
	bl 0x0200a404
	movs	r0, #2
	bl 0x0200a44c
	mov	r2, r8
	str	r2, [sp, #0]
	movs	r0, #2
	movs	r1, #30
	movs	r2, #34
	movs	r3, #10
	str	r5, [sp, #4]
	adds	r6, #1
	bl 0x0200a404
	movs	r0, #2
	bl 0x0200a44c
	cmp	r6, #12
	bne.n	.L_02000de6
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r5, #4
	movs	r0, #2
	movs	r1, #28
	movs	r2, #34
	movs	r3, #10
	str	r5, [sp, #0]
	bl 0x0200a404
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r0, #8
	movs	r3, #40
	movs	r1, #55
	movs	r2, #32
	str	r5, [sp, #4]
	bl 0x0200a404
	movs	r0, #60
	bl 0x0200a44c
	mov	r3, r9
	cmp	r3, #1
	bne.n	.L_02000e68
	movs	r6, #0
	adds	r5, r7, #0
.L_02000e52:
	ldr	r3, [r5, #8]
	ldr	r2, [pc, #84]
	adds	r3, r3, r2
	str	r3, [r5, #8]
	movs	r0, #1
	adds	r6, #1
	bl 0x0200a44c
	cmp	r6, #30
	bne.n	.L_02000e52
	b.n	.L_02000e88
.L_02000e68:
	mov	r3, r9
	cmp	r3, #2
	bne.n	.L_02000e88
	movs	r6, #0
	adds	r5, r7, #0
.L_02000e72:
	ldr	r3, [r5, #8]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r3, r2
	str	r3, [r5, #8]
	movs	r0, #1
	adds	r6, #1
	bl 0x0200a44c
	cmp	r6, #30
	bne.n	.L_02000e72
.L_02000e88:
	mov	r3, fp
	mov	r2, sl
	str	r3, [r2, #0]
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e70
	.4byte 0x0000023f
	.4byte 0x00000241
	.2byte 0x0000
	.2byte 0xffff
	.global Func_02000eb0
	.thumb_func
Func_02000eb0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #268]
	ldr r3, [r3]
	adds r3, #228
	ldr r2, [r3]
	ldr r6, [pc, #264]
	cmp r2, #0
	bge .L_02000eb0_0
	ldr r1, [pc, #260]
	adds r2, r2, r1
.L_02000eb0_0:
	ldr r3, [r3, #4]
	asrs r2, r2, #16
	mov r10, r2
	cmp r3, #0
	bge .L_02000eb0_1
	ldr r2, [pc, #248]
	adds r3, r3, r2
.L_02000eb0_1:
	asrs r3, r3, #16
	movs r2, #80
	subs r2, r2, r3
	mov r8, r2
	mov r3, r8
	adds r3, #16
	cmp r3, #175
	bhi .L_02000eb0_2
	ldr r3, [pc, #232]
	ldr r3, [r3]
	mov r1, r10
	asrs r3, r3, #10
	subs r5, r3, r1
	movs r3, #32
	negs r3, r3
	orrs r5, r3
	ldr r2, [pc, #220]
	ldr r3, [pc, #220]
	movs r7, #0
	mov r11, r2
	mov r9, r3
.L_02000eb0_3:
	ldrh r3, [r6, #6]
	adds r2, r5, #0
	mov r1, r11
	ands r2, r1
	mov r1, r9
	ands r3, r1
	orrs r3, r2
	mov r2, r8
	strh r3, [r6, #6]
	strb r2, [r6, #4]
	adds r0, r6, #0
	movs r1, #0
	adds r7, #1
	bl 0x0200a3f4
	adds r5, #32
	adds r6, #12
	cmp r7, #8
	bls .L_02000eb0_3
	ldr r3, [pc, #168]
	ldr r3, [r3]
	mov r1, r10
	asrs r3, r3, #9
	subs r5, r3, r1
	movs r3, #32
	negs r3, r3
	orrs r5, r3
	ldr r2, [pc, #156]
	ldr r3, [pc, #156]
	movs r7, #0
	mov r11, r2
	mov r9, r3
.L_02000eb0_4:
	ldrh r3, [r6, #6]
	adds r2, r5, #0
	mov r1, r11
	ands r2, r1
	mov r1, r9
	ands r3, r1
	orrs r3, r2
	mov r2, r8
	strh r3, [r6, #6]
	strb r2, [r6, #4]
	adds r0, r6, #0
	movs r1, #0
	adds r7, #1
	bl 0x0200a3f4
	adds r5, #32
	adds r6, #12
	cmp r7, #8
	bls .L_02000eb0_4
	ldr r3, [pc, #104]
	ldr r3, [r3]
	mov r1, r10
	asrs r3, r3, #8
	subs r5, r3, r1
	movs r3, #32
	negs r3, r3
	orrs r5, r3
	ldr r1, [pc, #96]
	ldr r3, [pc, #88]
	movs r2, #8
	movs r7, #0
	add r8, r2
	mov r9, r3
	mov r10, r1
.L_02000eb0_5:
	adds r2, r5, #0
	mov r3, r9
	ands r2, r3
	ldrh r3, [r6, #6]
	mov r1, r10
	ands r3, r1
	orrs r3, r2
	mov r2, r8
	strh r3, [r6, #6]
	strb r2, [r6, #4]
	adds r0, r6, #0
	movs r1, #0
	adds r7, #1
	bl 0x0200a3f4
	adds r5, #32
	adds r6, #12
	cmp r7, #8
	bls .L_02000eb0_5
.L_02000eb0_2:
	ldr r2, [pc, #36]
	ldr r3, [r2]
	adds r3, #128
	str r3, [r2]
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001e70
	.4byte 0x0200aa50
	.4byte 0x0000ffff
	.4byte 0x0200a974
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.global Func_02000fe4
	.thumb_func
Func_02000fe4:
	push {lr}
	movs r0, #0
	bl 0x0200a46c
	movs r2, #192
	ldrh r3, [r0, #6]
	lsls r2, r2, #8
	cmp r3, r2
	bne .L_02000fe4_0
	bl 0x0200a534
.L_02000fe4_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001000
	.thumb_func
Func_02001000:
	push {lr}
	movs r0, #0
	bl 0x0200a46c
	movs r2, #128
	ldrh r3, [r0, #6]
	lsls r2, r2, #7
	cmp r3, r2
	bne .L_02001000_0
	bl 0x0200a534
.L_02001000_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_0200101c
	.thumb_func
Func_0200101c:
	push {lr}
	ldr r0, [pc, #48]
	bl 0x0200a434
	cmp r0, #0
	bne .L_0200101c_0
	bl 0x0200a454
	movs r0, #9
	bl 0x0200a46c
	movs r0, #9
	ldr r1, [pc, #28]
	ldr r2, [pc, #32]
	bl 0x0200a474
	movs r1, #252
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #152
	bl 0x0200a494
	bl 0x0200a45c
.L_0200101c_0:
	pop {r0}
	bx r0
	.4byte 0x0000080b
	.4byte 0x00003333
	.4byte 0x00001999
	.global Func_0200105c
	.thumb_func
Func_0200105c:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	ldr r3, [pc, #916]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #68
	str r2, [r3]
	sub sp, #8
	bl 0x020094b8
	movs r0, #162
	lsls r0, r0, #1
	bl 0x0200a43c
	movs r0, #18
	bl 0x0200a46c
	adds r2, r0, #0
	adds r2, #89
	movs r3, #0
	strb r3, [r2]
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r0, #18
	bl 0x0200a46c
	movs r1, #0
	bl 0x0200a414
	movs r0, #18
	bl 0x0200a46c
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #1
	movs r0, #18
	bl 0x0200a4fc
	ldr r3, [pc, #832]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r3, [r3]
	movs r2, #128
	subs r3, #3
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bls .L_0200105c_0
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl 0x0200a4a4
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200a4a4
.L_0200105c_0:
	ldr r0, [pc, #792]
	bl 0x0200a434
	cmp r0, #0
	beq .L_0200105c_1
	movs r1, #144
	movs r2, #178
	movs r0, #18
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x0200a4a4
	movs r1, #201
	movs r2, #201
	movs r0, #17
	lsls r1, r1, #19
	lsls r2, r2, #19
	bl 0x0200a4a4
	movs r1, #232
	movs r2, #240
	movs r0, #10
	lsls r1, r1, #16
	lsls r2, r2, #15
	bl 0x0200a4a4
	movs r1, #172
	movs r2, #240
	movs r0, #12
	lsls r1, r1, #17
	lsls r2, r2, #15
	bl 0x0200a4a4
	movs r1, #232
	movs r2, #240
	movs r0, #10
	lsls r1, r1, #16
	lsls r2, r2, #15
	bl 0x0200a4a4
	movs r5, #4
	movs r3, #38
	movs r6, #3
	movs r0, #0
	movs r1, #59
	movs r2, #15
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200a404
	movs r1, #172
	movs r2, #240
	movs r0, #12
	lsls r1, r1, #17
	lsls r2, r2, #15
	bl 0x0200a4a4
	movs r0, #4
	movs r1, #59
	movs r2, #17
	movs r3, #38
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200a404
	movs r3, #2
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #8
	movs r1, #60
	movs r2, #17
	movs r3, #39
	bl 0x0200a404
	movs r3, #17
	movs r2, #7
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #1
	b .L_0200105c_2
.L_0200105c_1:
	ldr r0, [pc, #636]
	bl 0x0200a434
	cmp r0, #0
	beq .L_0200105c_3
	ldr r0, [pc, #628]
	bl 0x0200a434
	cmp r0, #0
	beq .L_0200105c_3
	movs r1, #232
	movs r2, #240
	movs r0, #10
	lsls r1, r1, #16
	lsls r2, r2, #15
	bl 0x0200a4a4
	movs r1, #172
	movs r2, #240
	movs r0, #12
	lsls r1, r1, #17
	lsls r2, r2, #15
	bl 0x0200a4a4
	movs r3, #1
	str r3, [sp, #4]
	movs r5, #2
	movs r3, #8
	movs r0, #0
	movs r1, #28
	movs r2, #17
	str r5, [sp, #0]
	bl 0x0200a404
	movs r1, #232
	movs r2, #240
	movs r0, #10
	lsls r1, r1, #16
	lsls r2, r2, #15
	bl 0x0200a4a4
	movs r3, #3
	str r3, [sp, #4]
	movs r6, #4
	mov r8, r3
	movs r0, #0
	movs r3, #38
	movs r1, #59
	movs r2, #15
	str r6, [sp, #0]
	bl 0x0200a404
	movs r1, #172
	movs r2, #240
	movs r0, #12
	lsls r1, r1, #17
	lsls r2, r2, #15
	bl 0x0200a4a4
	mov r2, r8
	str r2, [sp, #4]
	movs r0, #4
	movs r1, #59
	movs r2, #17
	movs r3, #38
	str r6, [sp, #0]
	bl 0x0200a404
	movs r0, #8
	movs r1, #60
	movs r2, #17
	movs r3, #39
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a404
	movs r3, #17
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
.L_0200105c_2:
	movs r2, #2
	movs r3, #1
	bl 0x0200a40c
	b .L_0200105c_4
.L_0200105c_3:
	ldr r0, [pc, #460]
	bl 0x0200a434
	cmp r0, #0
	beq .L_0200105c_5
	movs r1, #232
	movs r2, #240
	movs r0, #10
	lsls r1, r1, #16
	lsls r2, r2, #15
	bl 0x0200a4a4
	movs r3, #4
	movs r2, #3
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #59
	movs r2, #15
	movs r3, #38
	bl 0x0200a404
.L_0200105c_5:
	ldr r0, [pc, #420]
	bl 0x0200a434
	cmp r0, #0
	beq .L_0200105c_4
	movs r1, #172
	movs r2, #240
	movs r0, #12
	lsls r1, r1, #17
	lsls r2, r2, #15
	bl 0x0200a4a4
	movs r3, #4
	movs r2, #3
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #4
	movs r1, #59
	movs r2, #17
	movs r3, #38
	bl 0x0200a404
.L_0200105c_4:
	ldr r0, [pc, #380]
	bl 0x0200a434
	cmp r0, #0
	beq .L_0200105c_6
	movs r1, #252
	movs r2, #152
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x0200a4a4
	movs r5, #2
	movs r6, #1
	movs r0, #2
	movs r1, #28
	movs r2, #34
	movs r3, #10
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200a404
	movs r0, #2
	movs r1, #30
	movs r2, #16
	movs r3, #10
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200a404
	movs r3, #4
	movs r2, #3
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #55
	movs r2, #32
	movs r3, #40
	bl 0x0200a404
.L_0200105c_6:
	ldr r0, [pc, #304]
	bl 0x0200a434
	cmp r0, #0
	beq .L_0200105c_7
	movs r1, #162
	movs r2, #152
	movs r0, #11
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl 0x0200a4a4
	movs r5, #2
	movs r6, #1
	movs r0, #4
	movs r1, #28
	movs r2, #36
	movs r3, #10
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200a404
	movs r0, #4
	movs r1, #30
	movs r2, #18
	movs r3, #10
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200a404
	movs r3, #4
	movs r2, #3
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #4
	movs r1, #55
	movs r2, #36
	movs r3, #40
	bl 0x0200a404
.L_0200105c_7:
	ldr r0, [pc, #228]
	bl 0x0200a434
	cmp r0, #0
	beq .L_0200105c_8
	movs r1, #252
	movs r2, #200
	movs r0, #13
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x0200a4a4
	movs r5, #1
	movs r6, #2
	movs r0, #2
	movs r1, #29
	movs r2, #34
	movs r3, #11
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a404
	movs r0, #2
	movs r1, #31
	movs r2, #16
	movs r3, #11
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a404
	movs r3, #4
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #58
	movs r2, #32
	movs r3, #43
	str r5, [sp, #4]
	bl 0x0200a404
.L_0200105c_8:
	ldr r0, [pc, #156]
	bl 0x0200a434
	cmp r0, #0
	beq .L_0200105c_9
	movs r1, #162
	movs r2, #200
	movs r0, #15
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl 0x0200a4a4
	movs r5, #1
	movs r6, #2
	movs r0, #4
	movs r1, #29
	movs r2, #36
	movs r3, #11
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a404
	movs r0, #4
	movs r1, #31
	movs r2, #18
	movs r3, #11
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a404
	movs r3, #4
	str r3, [sp, #0]
	movs r0, #4
	movs r1, #58
	movs r2, #36
	movs r3, #43
	str r5, [sp, #4]
	bl 0x0200a404
.L_0200105c_9:
	ldr r5, [pc, #48]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	bne .L_0200105c_10
	ldr r0, [pc, #64]
	bl 0x0200a434
	cmp r0, #0
	beq .L_0200105c_11
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200a4a4
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl 0x0200a4a4
	b .L_0200105c_10
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x00000818
	.4byte 0x00000816
	.4byte 0x00000817
	.4byte 0x0000080b
	.4byte 0x0000080c
	.4byte 0x0000080d
	.4byte 0x0000080e
	.4byte 0x0000030a
.L_0200105c_11:
	ldr r0, [pc, #132]
	bl 0x0200a434
	cmp r0, #0
	bne .L_0200105c_10
	bl 0x02008150
	ldr r0, [pc, #124]
	bl 0x0200a43c
.L_0200105c_10:
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #4
	bne .L_0200105c_12
	ldr r0, [pc, #108]
	bl 0x0200a434
	cmp r0, #0
	beq .L_0200105c_13
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200a4a4
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl 0x0200a4a4
	b .L_0200105c_12
.L_0200105c_13:
	ldr r0, [pc, #68]
	bl 0x0200a434
	cmp r0, #0
	bne .L_0200105c_12
	bl 0x02008614
	ldr r0, [pc, #60]
	bl 0x0200a43c
.L_0200105c_12:
	ldr r0, [pc, #60]
	bl 0x0200a434
	cmp r0, #0
	beq .L_0200105c_14
	movs r0, #141
	bl 0x0200a55c
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl 0x0200a41c
	bl 0x0200a53c
.L_0200105c_14:
	movs r0, #0
	sub sp, #-8
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r1}
	bx r1
	.4byte 0x00000109
	.4byte 0x0000030a
	.4byte 0x0000030b
	.4byte 0x00000814
	.global Func_020014b8
	.thumb_func
Func_020014b8:
	push {r5, r6, lr}
	movs r1, #128
	lsls r1, r1, #3
	movs r0, #14
	ldr r5, [pc, #160]
	bl 0x0200a3cc
	adds r6, r0, #0
	adds r1, r6, #0
	ldr r0, [pc, #156]
	bl 0x0200a3dc
	bl 0x0200a3ec
	movs r1, #128
	adds r2, r6, #0
	bl 0x0200a3e4
	movs r3, #172
	lsls r3, r3, #8
	ldr r1, [pc, #136]
	movs r2, #0
	movs r4, #0
	orrs r0, r3
.L_020014b8_0:
	adds r3, r5, #0
	stmia r3!, {r4}
	stmia r3!, {r1}
	adds r2, #1
	adds r5, #12
	str r0, [r3]
	cmp r2, #8
	bls .L_020014b8_0
	bl 0x0200a3ec
	adds r2, r6, #0
	adds r2, #128
	movs r1, #128
	bl 0x0200a3e4
	movs r3, #220
	lsls r3, r3, #8
	ldr r1, [pc, #96]
	movs r2, #0
	movs r4, #0
	orrs r0, r3
.L_020014b8_1:
	adds r3, r5, #0
	stmia r3!, {r4}
	stmia r3!, {r1}
	adds r2, #1
	adds r5, #12
	str r0, [r3]
	cmp r2, #8
	bls .L_020014b8_1
	bl 0x0200a3ec
	movs r3, #128
	lsls r3, r3, #1
	adds r2, r6, r3
	movs r1, #128
	bl 0x0200a3e4
	movs r3, #192
	lsls r3, r3, #4
	ldr r1, [pc, #52]
	movs r2, #0
	movs r4, #0
	orrs r0, r3
.L_020014b8_2:
	adds r3, r5, #0
	stmia r3!, {r4}
	stmia r3!, {r1}
	adds r2, #1
	adds r5, #12
	str r0, [r3]
	cmp r2, #8
	bls .L_020014b8_2
	movs r0, #14
	bl 0x0200a3d4
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #20]
	bl 0x0200a3c4
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x0200aa50
	.4byte 0x0200a56c
	.4byte 0x40004000
	.4byte 0x02008eb1
	.global Func_02001574
	.thumb_func
Func_02001574:
	push {lr}
	movs r0, #9
	movs r1, #31
	movs r2, #9
.L_0200157c:
	bl 0x02009be8
	cmp r0, #0
	beq .L_0200157c_0
	bl 0x02009c14
.L_0200157c_0:
	pop {r0}
	bx r0
	.global Func_0200158c
	.thumb_func
Func_0200158c:
	push {lr}
	movs r0, #11
	movs r1, #40
	movs r2, #9
	bl 0x02009be8
	cmp r0, #0
	beq .L_0200158c_0
	bl 0x02009d14
.L_0200158c_0:
	pop {r0}
	bx r0
	.global Func_020015a4
	.thumb_func
Func_020015a4:
	push {lr}
	movs r0, #13
	movs r1, #31
	movs r2, #12
	bl 0x02009be8
	cmp r0, #0
	beq .L_020015a4_0
	bl 0x02009e10
.L_020015a4_0:
	pop {r0}
	bx r0
	.global Func_020015bc
	.thumb_func
Func_020015bc:
	push {lr}
	movs r0, #15
	movs r1, #40
	movs r2, #12
	bl 0x02009be8
	cmp r0, #0
	beq .L_020015bc_0
	bl 0x02009f0c
.L_020015bc_0:
	pop {r0}
	bx r0
	.global Func_020015d4
	.thumb_func
Func_020015d4:
	push {lr}
	movs r1, #208
	movs r2, #224
	movs r0, #2
	lsls r1, r1, #16
	lsls r2, r2, #15
	movs r3, #0
	bl 0x0200a424
	movs r0, #10
	movs r1, #14
	movs r2, #7
	bl 0x02009be8
	cmp r0, #0
	beq .L_020015d4_0
	bl 0x0200a244
.L_020015d4_0:
	pop {r0}
	bx r0
	.global Func_020015fc
	.thumb_func
Func_020015fc:
	push {lr}
	movs r1, #176
	movs r2, #224
	movs r0, #2
	lsls r1, r1, #17
	lsls r2, r2, #15
	movs r3, #0
	bl 0x0200a424
	movs r0, #12
	movs r1, #21
	movs r2, #7
	bl 0x02009be8
	cmp r0, #0
	beq .L_020015fc_0
	bl 0x0200a2f4
.L_020015fc_0:
	pop {r0}
	bx r0
	.global Func_02001624
	.thumb_func
Func_02001624:
	push {r5, lr}
	movs r0, #0
	bl 0x0200a46c
	ldr r3, [r0, #8]
	movs r0, #0
	asrs r5, r3, #20
	bl 0x0200a46c
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, #8
	bne .L_02001624_0
	adds r3, r5, #0
	subs r3, #17
	cmp r3, #1
	bhi .L_02001624_0
	movs r1, #136
	movs r2, #128
	movs r0, #2
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r3, #255
	bl 0x0200a424
	movs r1, #144
	movs r2, #128
	movs r0, #2
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r3, #255
	bl 0x0200a424
.L_02001624_0:
	pop {r5}
	pop {r0}
	bx r0
	.global Func_0200166c
	.thumb_func
Func_0200166c:
	push {r5, lr}
	movs r0, #0
	bl 0x0200a46c
	ldr r3, [r0, #8]
	movs r0, #0
	asrs r5, r3, #20
	bl 0x0200a46c
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, #7
	bne .L_0200166c_0
	adds r3, r5, #0
	subs r3, #13
	cmp r3, #1
	bhi .L_0200166c_0
	movs r1, #208
	movs r2, #224
	movs r0, #2
	lsls r1, r1, #16
	lsls r2, r2, #15
	movs r3, #255
	bl 0x0200a424
.L_0200166c_0:
	pop {r5}
	pop {r0}
	bx r0
	.global Func_020016a4
	.thumb_func
Func_020016a4:
	push {r5, lr}
	movs r0, #0
	bl 0x0200a46c
	ldr r3, [r0, #8]
	movs r0, #0
	asrs r5, r3, #20
	bl 0x0200a46c
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, #7
	bne .L_020016a4_0
	adds r3, r5, #0
	subs r3, #21
	cmp r3, #1
	bhi .L_020016a4_0
	movs r1, #176
	movs r2, #224
	movs r0, #2
	lsls r1, r1, #17
	lsls r2, r2, #15
	movs r3, #255
	bl 0x0200a424
.L_020016a4_0:
	pop {r5}
	pop {r0}
	bx r0
	.global Func_020016dc
	.thumb_func
Func_020016dc:
	push {r5, r6, lr}
	movs r0, #17
	sub sp, #8
	bl 0x0200a46c
	movs r1, #136
	movs r2, #128
	adds r6, r0, #0
	lsls r1, r1, #17
	movs r0, #2
	lsls r2, r2, #16
	movs r3, #0
	bl 0x0200a424
	movs r1, #144
	movs r2, #128
	movs r0, #2
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r3, #0
	bl 0x0200a424
	cmp r6, #0
	bne .L_020016dc_0
	b .L_020016dc_1
.L_020016dc_0:
	ldr r5, [r6, #16]
	asrs r5, r5, #20
	bl 0x0200a454
	cmp r5, #8
	beq .L_020016dc_2
	b .L_020016dc_3
.L_020016dc_2:
	ldr r0, [pc, #524]
	bl 0x0200a434
	cmp r0, #0
	bne .L_020016dc_4
	movs r0, #0
	bl 0x0200a46c
	ldr r3, [r0, #16]
	asrs r3, r3, #19
	cmp r3, #17
	bhi .L_020016dc_4
	movs r0, #0
	ldr r1, [pc, #504]
	movs r2, #158
	bl 0x0200a494
	movs r0, #0
	bl 0x0200a46c
	movs r3, #192
	lsls r3, r3, #8
	strh r3, [r0, #6]
.L_020016dc_4:
	ldr r0, [pc, #488]
	bl 0x0200a434
	cmp r0, #0
	bne .L_020016dc_5
	b .L_020016dc_3
.L_020016dc_5:
	ldr r0, [pc, #480]
	bl 0x0200a434
	cmp r0, #0
	bne .L_020016dc_6
	b .L_020016dc_3
.L_020016dc_6:
	ldr r0, [pc, #472]
	bl 0x0200a43c
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200a514
	movs r0, #143
	movs r1, #1
	movs r2, #146
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #17
	bl 0x0200a51c
	bl 0x0200a524
	movs r0, #17
	bl 0x0200a46c
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #192
	movs r2, #128
	lsls r2, r2, #9
	strb r3, [r0]
	lsls r1, r1, #10
	movs r0, #17
	bl 0x0200a474
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	movs r1, #3
	movs r0, #17
	bl 0x0200a4fc
	movs r0, #189
	bl 0x0200a564
	movs r1, #144
	lsls r1, r1, #1
	movs r2, #178
	movs r0, #17
	bl 0x0200a47c
	movs r0, #8
	bl 0x0200a44c
	movs r1, #144
	movs r2, #178
	movs r0, #18
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x0200a4a4
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r6, #56]
	str r3, [r6, #60]
	str r3, [r6, #64]
	movs r0, #17
	movs r1, #0
	movs r2, #0
	str r5, [r6, #8]
	str r5, [r6, #12]
	str r5, [r6, #16]
	str r5, [r6, #36]
	str r5, [r6, #40]
	str r5, [r6, #44]
	bl 0x0200a4a4
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x0200a41c
	movs r0, #10
	bl 0x0200a44c
	movs r0, #141
	bl 0x0200a564
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl 0x0200a41c
	movs r0, #10
	bl 0x0200a44c
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #11
	bl 0x0200a41c
	movs r0, #35
	bl 0x0200a44c
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #11
	bl 0x0200a41c
	movs r0, #20
	bl 0x0200a44c
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl 0x0200a41c
	movs r0, #30
	bl 0x0200a44c
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl 0x0200a41c
	movs r0, #40
	bl 0x0200a44c
	ldr r0, [pc, #164]
	bl 0x0200a564
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x0200a41c
	movs r0, #10
	bl 0x0200a44c
	movs r0, #1
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #148]
	negs r0, r0
	bl 0x0200a41c
	movs r0, #60
	bl 0x0200a44c
	movs r0, #188
	bl 0x0200a564
	ldr r0, [pc, #132]
	bl 0x0200a434
	cmp r0, #0
	beq .L_020016dc_7
	ldr r0, [pc, #124]
	bl 0x0200a434
	cmp r0, #0
	beq .L_020016dc_7
	ldr r0, [pc, #120]
	bl 0x0200a434
	cmp r0, #0
	beq .L_020016dc_7
	ldr r0, [pc, #112]
	bl 0x0200a434
	cmp r0, #0
	beq .L_020016dc_7
	ldr r0, [pc, #108]
	bl 0x0200a43c
.L_020016dc_7:
	movs r0, #40
	bl 0x0200a44c
	ldr r0, [pc, #100]
	movs r1, #1
	bl 0x0200a42c
	movs r3, #8
	movs r5, #17
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #1
	movs r2, #2
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200a40c
	movs r3, #7
	str r3, [sp, #4]
	movs r0, #17
	movs r1, #9
	movs r2, #2
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200a40c
.L_020016dc_3:
	bl 0x0200a45c
.L_020016dc_1:
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x00000207
	.4byte 0x00000121
	.4byte 0x00000816
	.4byte 0x00000817
	.4byte 0x00000818
	.4byte 0x0000e666
	.4byte 0x0000080b
	.4byte 0x0000080c
	.4byte 0x0000080d
	.4byte 0x0000080e
	.4byte 0x0000080f
	.4byte 0x00001038
	.global Func_0200195c
	.thumb_func
Func_0200195c:
	push {lr}
	movs r0, #17
	bl 0x0200a46c
	cmp r0, #0
	beq .L_0200195c_0
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, #8
	bne .L_0200195c_0
	bl 0x0200a454
	movs r0, #185
	bl 0x0200a564
	movs r0, #17
	ldr r1, [pc, #92]
	ldr r2, [pc, #96]
	bl 0x0200a474
	ldr r1, [pc, #84]
	ldr r2, [pc, #88]
	movs r0, #0
	bl 0x0200a474
	movs r0, #17
	bl 0x0200a46c
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #8
	movs r0, #0
	bl 0x0200a4ac
	movs r0, #0
	bl 0x0200a46c
	movs r2, #136
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r0, #0
	bl 0x0200a47c
	movs r1, #144
	lsls r1, r1, #1
	movs r2, #120
	movs r0, #17
	bl 0x0200a47c
	movs r0, #17
	bl 0x0200a49c
	movs r0, #0
	movs r1, #1
	bl 0x0200a4ac
	bl 0x0200a45c
.L_0200195c_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00003333
	.4byte 0x00001999
	.global Func_020019e4
	.thumb_func
Func_020019e4:
	push {lr}
	sub sp, #4
	movs r3, #9
	str r3, [sp, #0]
	movs r0, #9
	movs r1, #31
	movs r2, #9
	movs r3, #30
	bl 0x02009b44
	bl 0x02009c14
	sub sp, #-4
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001a04
	.thumb_func
Func_02001a04:
	push {lr}
	sub sp, #4
	movs r3, #9
	str r3, [sp, #0]
	movs r0, #11
	movs r1, #40
	movs r2, #9
	movs r3, #41
	bl 0x02009b44
	bl 0x02009d14
	sub sp, #-4
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001a24
	.thumb_func
Func_02001a24:
	push {lr}
	sub sp, #4
	movs r3, #12
	str r3, [sp, #0]
	movs r0, #13
	movs r1, #31
	movs r2, #12
	movs r3, #30
	bl 0x02009b44
	bl 0x02009e10
	sub sp, #-4
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001a44
	.thumb_func
Func_02001a44:
	push {lr}
	sub sp, #4
	movs r3, #12
	str r3, [sp, #0]
	movs r0, #15
	movs r1, #40
	movs r2, #12
	movs r3, #41
	bl 0x02009b44
	bl 0x02009f0c
	sub sp, #-4
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001a64
	.thumb_func
Func_02001a64:
	push {lr}
	sub sp, #4
	movs r3, #7
	str r3, [sp, #0]
	movs r0, #10
	movs r1, #14
	movs r2, #7
	movs r3, #13
	bl 0x02009b44
	bl 0x0200a244
	sub sp, #-4
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001a84
	.thumb_func
Func_02001a84:
	push {lr}
	movs r0, #10
	sub sp, #4
	bl 0x0200a46c
	cmp r0, #0
	beq .L_02001a84_0
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	adds r2, r3, #1
	str r3, [sp, #0]
	movs r0, #10
	movs r1, #13
	movs r3, #13
	bl 0x02009b44
.L_02001a84_0:
	sub sp, #-4
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001aac
	.thumb_func
Func_02001aac:
	push {lr}
	movs r0, #10
	sub sp, #4
	bl 0x0200a46c
	cmp r0, #0
	beq .L_02001aac_0
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	subs r2, r3, #1
	str r3, [sp, #0]
	movs r0, #10
	movs r1, #13
	movs r3, #13
	bl 0x02009b44
.L_02001aac_0:
	sub sp, #-4
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001ad4
	.thumb_func
Func_02001ad4:
	push {lr}
	sub sp, #4
	movs r3, #7
	str r3, [sp, #0]
	movs r0, #12
	movs r1, #21
	movs r2, #7
	movs r3, #22
	bl 0x02009b44
	bl 0x0200a2f4
	sub sp, #-4
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001af4
	.thumb_func
Func_02001af4:
	push {lr}
	movs r0, #12
	sub sp, #4
	bl 0x0200a46c
	cmp r0, #0
	beq .L_02001af4_0
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	adds r2, r3, #1
	str r3, [sp, #0]
	movs r0, #12
	movs r1, #22
	movs r3, #22
	bl 0x02009b44
.L_02001af4_0:
	sub sp, #-4
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001b1c
	.thumb_func
Func_02001b1c:
	push {lr}
	movs r0, #12
	sub sp, #4
	bl 0x0200a46c
	cmp r0, #0
	beq .L_02001b1c_0
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	subs r2, r3, #1
	str r3, [sp, #0]
	movs r0, #12
	movs r1, #22
	movs r3, #22
	bl 0x02009b44
.L_02001b1c_0:
	sub sp, #-4
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001b44
	.thumb_func
Func_02001b44:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	ldr r6, [sp, #24]
	mov r9, r0
	adds r5, r3, #0
	mov r8, r1
	mov r10, r2
	bl 0x0200a454
	movs r0, #185
	bl 0x0200a564
	mov r0, r9
	ldr r1, [pc, #120]
	ldr r2, [pc, #120]
	bl 0x0200a474
	ldr r1, [pc, #112]
	ldr r2, [pc, #112]
	movs r0, #0
	bl 0x0200a474
	mov r0, r9
	bl 0x0200a46c
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	lsls r5, r5, #4
	lsls r6, r6, #4
	strb r3, [r0]
	movs r1, #8
	movs r0, #0
	adds r5, #8
	adds r6, #8
	bl 0x0200a4ac
	adds r1, r5, #0
	adds r2, r6, #0
	movs r0, #0
	bl 0x0200a47c
	mov r3, r8
	lsls r3, r3, #4
	mov r8, r3
	mov r3, r10
	lsls r3, r3, #4
	mov r10, r3
	movs r3, #8
	add r8, r3
	add r10, r3
	mov r1, r8
	mov r2, r10
	mov r0, r9
	bl 0x0200a47c
	mov r0, r9
	bl 0x0200a49c
	movs r0, #0
	movs r1, #1
	bl 0x0200a4ac
	bl 0x0200a45c
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00003333
	.4byte 0x00001999
	.global Func_02001be8
	.thumb_func
Func_02001be8:
	push {r5, r6, lr}
	adds r5, r1, #0
	adds r6, r2, #0
	bl 0x0200a46c
	cmp r0, #0
	beq .L_02001be8_0
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, r5
	beq .L_02001be8_1
.L_02001be8_0:
	movs r0, #0
	b .L_02001be8_2
.L_02001be8_1:
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	movs r0, #0
	cmp r3, r6
	bne .L_02001be8_2
	movs r0, #1
.L_02001be8_2:
	pop {r5, r6}
	pop {r1}
	bx r1
	.global Func_02001c14
	.thumb_func
Func_02001c14:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r7, [pc, #232]
	movs r3, #0
	str r3, [r7]
	movs r3, #55
	mov r9, r3
	str r3, [r7, #4]
	movs r3, #4
	mov lr, r3
	str r3, [r7, #16]
	movs r3, #16
	str r3, [r7, #80]
	ldr r3, [pc, #212]
	str r3, [r7, #96]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #100]
	movs r3, #250
	movs r6, #40
	lsls r3, r3, #1
	movs r4, #32
	mov r12, r6
	str r6, [r7, #12]
	str r3, [r7, #104]
	movs r6, #28
	movs r3, #132
	mov r10, r4
	mov r11, r6
	str r6, [r7, #52]
	str r3, [r7, #108]
	mov r6, r9
	movs r3, #8
	str r4, [r7, #8]
	str r3, [r7, #112]
	movs r4, #3
	str r6, [r7, #116]
	mov r3, r10
	mov r6, r12
	movs r2, #2
	movs r1, #10
	movs r0, #1
	movs r5, #34
	mov r8, r4
	str r4, [r7, #20]
	str r3, [r7, #120]
	movs r4, #30
	str r6, [r7, #124]
	movs r3, #128
	mov r6, lr
	str r2, [r7, #24]
	str r4, [r7, #28]
	str r2, [r7, #40]
	str r2, [r7, #48]
	str r2, [r7, #64]
	str r2, [r7, #72]
	str r4, [r7, #76]
	str r2, [r7, #88]
	str r5, [r7, #32]
	str r1, [r7, #36]
	str r0, [r7, #44]
	str r5, [r7, #56]
	str r1, [r7, #60]
	str r0, [r7, #68]
	str r1, [r7, #84]
	str r0, [r7, #92]
	str r6, [r3, r7]
	mov r6, r8
	movs r3, #132
	str r6, [r3, r7]
	adds r3, r7, #0
	adds r3, #136
	str r2, [r3]
	adds r3, #4
	str r4, [r3]
	adds r3, #4
	str r5, [r3]
	adds r3, #4
	str r1, [r3]
	adds r3, #4
	str r2, [r3]
	adds r3, #4
	str r0, [r3]
	adds r3, #4
	str r2, [r3]
	mov r4, r11
	adds r3, #4
	str r4, [r3]
	movs r6, #16
	adds r3, #4
	str r6, [r3]
	adds r3, #4
	str r1, [r3]
	adds r3, #4
	str r2, [r3]
	adds r3, #4
	adds r2, r7, #0
	str r0, [r3]
	adds r2, #184
	movs r3, #9
	str r3, [r2]
	movs r3, #244
	adds r2, #4
	lsls r3, r3, #1
	str r3, [r2]
	adds r2, #4
	movs r3, #152
	str r3, [r2]
	bl 0x02009ff4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200a980
	.4byte 0x0000080b
	.global Func_02001d14
	.thumb_func
Func_02001d14:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r7, [pc, #224]
	movs r3, #55
	mov r10, r3
	str r3, [r7, #4]
	movs r3, #3
	mov r8, r3
	str r3, [r7, #20]
	movs r3, #28
	mov r9, r3
	str r3, [r7, #52]
	ldr r3, [pc, #208]
	str r3, [r7, #96]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #100]
	ldr r3, [pc, #200]
	str r3, [r7, #104]
	movs r3, #132
	movs r6, #40
	str r3, [r7, #108]
	movs r3, #12
	mov lr, r6
	str r6, [r7, #12]
	str r3, [r7, #112]
	movs r6, #30
	mov r3, r10
	mov r11, r6
	str r6, [r7, #28]
	str r6, [r7, #76]
	str r3, [r7, #116]
	movs r6, #18
	adds r3, r7, #0
	movs r2, #4
	movs r1, #36
	movs r0, #10
	movs r4, #2
	movs r5, #1
	mov r12, r6
	str r6, [r7, #80]
	adds r3, #128
	mov r6, lr
	str r2, [r7]
	str r2, [r7, #16]
	str r2, [r7, #24]
	str r2, [r7, #48]
	str r2, [r7, #72]
	str r6, [r7, #124]
	str r1, [r7, #8]
	mov r6, r8
	str r1, [r7, #32]
	str r0, [r7, #36]
	str r4, [r7, #40]
	str r5, [r7, #44]
	str r1, [r7, #56]
	str r0, [r7, #60]
	str r4, [r7, #64]
	str r5, [r7, #68]
	str r0, [r7, #84]
	str r4, [r7, #88]
	str r5, [r7, #92]
	str r1, [r7, #120]
	str r2, [r3]
	movs r3, #132
	str r6, [r3, r7]
	adds r3, r7, #0
	adds r3, #136
	str r2, [r3]
	mov r6, r11
	adds r3, #4
	str r6, [r3]
	adds r3, #4
	str r1, [r3]
	adds r3, #4
	str r0, [r3]
	adds r3, #4
	str r4, [r3]
	adds r3, #4
	str r5, [r3]
	adds r3, #4
	str r2, [r3]
	mov r2, r9
	adds r3, #4
	str r2, [r3]
	mov r6, r12
	adds r3, #4
	str r6, [r3]
	adds r3, #4
	str r0, [r3]
	adds r3, #4
	str r4, [r3]
	adds r2, r7, #0
	adds r3, #4
	str r5, [r3]
	adds r2, #184
	movs r3, #11
	str r3, [r2]
	movs r3, #166
	adds r2, #4
	lsls r3, r3, #2
	str r3, [r2]
	adds r2, #4
	movs r3, #152
	str r3, [r2]
	bl 0x02009ff4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0200a980
	.4byte 0x0000080c
	.4byte 0x0000028e
	.global Func_02001e10
	.thumb_func
Func_02001e10:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r7, [pc, #228]
	movs r3, #0
	str r3, [r7]
	movs r3, #58
	mov r10, r3
	str r3, [r7, #4]
	movs r3, #4
	mov lr, r3
	str r3, [r7, #16]
	movs r3, #16
	mov r11, r3
	str r3, [r7, #80]
	ldr r3, [pc, #208]
	str r3, [r7, #96]
	movs r3, #192
	lsls r3, r3, #8
	str r3, [r7, #100]
	movs r3, #250
	movs r6, #43
	lsls r3, r3, #1
	movs r4, #32
	mov r12, r6
	str r6, [r7, #12]
	str r3, [r7, #104]
	movs r6, #29
	movs r3, #216
	mov r8, r4
	mov r9, r6
	str r6, [r7, #52]
	str r3, [r7, #108]
	mov r6, r10
	movs r3, #8
	str r3, [r7, #112]
	str r6, [r7, #116]
	mov r3, r8
	mov r6, r12
	movs r2, #2
	movs r1, #1
	movs r0, #11
	movs r5, #34
	str r4, [r7, #8]
	str r3, [r7, #120]
	movs r4, #31
	str r6, [r7, #124]
	movs r3, #128
	mov r6, lr
	str r2, [r7, #24]
	str r4, [r7, #28]
	str r2, [r7, #40]
	str r2, [r7, #48]
	str r2, [r7, #64]
	str r2, [r7, #72]
	str r4, [r7, #76]
	str r2, [r7, #88]
	str r1, [r7, #20]
	str r5, [r7, #32]
	str r0, [r7, #36]
	str r1, [r7, #44]
	str r5, [r7, #56]
	str r0, [r7, #60]
	str r1, [r7, #68]
	str r0, [r7, #84]
	str r1, [r7, #92]
	str r6, [r3, r7]
	adds r3, r7, #0
	adds r3, #132
	str r1, [r3]
	adds r3, #4
	str r2, [r3]
	adds r3, #4
	str r4, [r3]
	adds r3, #4
	str r5, [r3]
	adds r3, #4
	str r0, [r3]
	adds r3, #4
	str r2, [r3]
	adds r3, #4
	str r1, [r3]
	adds r3, #4
	str r2, [r3]
	mov r4, r9
	adds r3, #4
	str r4, [r3]
	mov r6, r11
	adds r3, #4
	str r6, [r3]
	adds r3, #4
	str r0, [r3]
	adds r3, #4
	str r2, [r3]
	adds r3, #4
	adds r2, r7, #0
	str r1, [r3]
	adds r2, #184
	movs r3, #13
	str r3, [r2]
	movs r3, #244
	adds r2, #4
	lsls r3, r3, #1
	str r3, [r2]
	adds r2, #4
	movs r3, #200
	str r3, [r2]
	bl 0x02009ff4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200a980
	.4byte 0x0000080d
	.global Func_02001f0c
	.thumb_func
Func_02001f0c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r7, [pc, #208]
	movs r3, #58
	mov r8, r3
	str r3, [r7, #4]
	movs r3, #43
	mov lr, r3
	str r3, [r7, #12]
	movs r3, #29
	mov r10, r3
	str r3, [r7, #52]
	movs r3, #18
	mov r12, r3
	str r3, [r7, #80]
	ldr r3, [pc, #188]
	str r3, [r7, #96]
	movs r3, #192
	lsls r3, r3, #8
	str r3, [r7, #100]
	ldr r3, [pc, #180]
	str r3, [r7, #104]
	movs r3, #216
	str r3, [r7, #108]
	movs r3, #12
	str r3, [r7, #112]
	mov r3, r8
	str r3, [r7, #116]
	mov r3, lr
	str r3, [r7, #124]
	adds r3, r7, #0
	movs r2, #4
	movs r1, #1
	movs r0, #36
	movs r4, #11
	movs r5, #2
	movs r6, #31
	adds r3, #128
	str r2, [r7]
	str r2, [r7, #16]
	str r2, [r7, #24]
	str r2, [r7, #48]
	str r2, [r7, #72]
	str r0, [r7, #8]
	str r1, [r7, #20]
	str r6, [r7, #28]
	str r0, [r7, #32]
	str r4, [r7, #36]
	str r5, [r7, #40]
	str r1, [r7, #44]
	str r0, [r7, #56]
	str r4, [r7, #60]
	str r5, [r7, #64]
	str r1, [r7, #68]
	str r6, [r7, #76]
	str r4, [r7, #84]
	str r5, [r7, #88]
	str r1, [r7, #92]
	str r0, [r7, #120]
	str r2, [r3]
	adds r3, #4
	str r1, [r3]
	adds r3, #4
	str r2, [r3]
	adds r3, #4
	str r6, [r3]
	adds r3, #4
	str r0, [r3]
	adds r3, #4
	str r4, [r3]
	adds r3, #4
	str r5, [r3]
	adds r3, #4
	str r1, [r3]
	adds r3, #4
	str r2, [r3]
	mov r2, r10
	adds r3, #4
	str r2, [r3]
	mov r2, r12
	adds r3, #4
	str r2, [r3]
	adds r3, #4
	str r4, [r3]
	adds r3, #4
	str r5, [r3]
	adds r2, r7, #0
	adds r3, #4
	str r1, [r3]
	adds r2, #184
	movs r3, #15
	str r3, [r2]
	movs r3, #166
	adds r2, #4
	lsls r3, r3, #2
	str r3, [r2]
	adds r2, #4
	movs r3, #200
	str r3, [r2]
	bl 0x02009ff4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200a980
	.4byte 0x0000080e
	.4byte 0x0000028e
	.global Func_02001ff4
	.thumb_func
Func_02001ff4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #0
	sub sp, #8
	mov r8, r3
	bl 0x0200a454
	ldr r0, [pc, #544]
	bl 0x0200a434
	cmp r0, #0
	beq .L_02001ff4_0
	b .L_02001ff4_1
.L_02001ff4_0:
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200a514
	movs r0, #144
	movs r1, #1
	movs r2, #172
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #18
	bl 0x0200a51c
	bl 0x0200a524
	movs r0, #186
	bl 0x0200a564
	ldr r5, [pc, #496]
	ldr r6, [r5, #20]
	ldr r4, [r5, #16]
	ldr r0, [r5]
	ldr r1, [r5, #4]
	ldr r2, [r5, #8]
	ldr r3, [r5, #12]
	str r6, [sp, #4]
	str r4, [sp, #0]
	bl 0x0200a404
	movs r7, #0
	adds r6, r5, #0
.L_02001ff4_2:
	movs r0, #246
	bl 0x0200a564
	ldr r4, [r6, #40]
	ldr r5, [r6, #44]
	ldr r1, [r6, #28]
	ldr r2, [r6, #32]
	ldr r3, [r6, #36]
	ldr r0, [r6, #24]
	str r4, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a404
	movs r0, #4
	bl 0x0200a44c
	movs r0, #246
	bl 0x0200a564
	ldr r4, [r6, #64]
	ldr r5, [r6, #68]
	ldr r0, [r6, #48]
	ldr r1, [r6, #52]
	ldr r2, [r6, #56]
	ldr r3, [r6, #60]
	adds r7, #1
	str r4, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a404
	movs r0, #4
	bl 0x0200a44c
	cmp r7, #20
	bne .L_02001ff4_2
	ldr r7, [pc, #400]
	ldr r4, [r7, #88]
	ldr r5, [r7, #92]
	ldr r3, [r7, #84]
	ldr r1, [r7, #76]
	ldr r2, [r7, #80]
	ldr r0, [r7, #72]
	str r4, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a404
	ldr r0, [r7, #96]
	bl 0x0200a43c
	bl 0x02008054
	movs r3, #1
	mov r8, r0
	negs r3, r3
	cmp r8, r3
	bne .L_02001ff4_4
	ldr r0, [pc, #364]
	bl 0x0200a434
	cmp r0, #0
	beq .L_02001ff4_5
	b .L_02001ff4_1
.L_02001ff4_5:
	movs r1, #1
	movs r0, #0
	bl 0x0200a50c
	movs r0, #0
	bl 0x0200a46c
	ldr r3, [r7, #100]
	movs r1, #128
	movs r2, #128
	strh r3, [r0, #6]
	lsls r1, r1, #10
	lsls r2, r2, #10
	movs r0, #0
	bl 0x0200a474
	movs r0, #0
	bl 0x0200a46c
	adds r0, #90
	ldrb r2, [r0]
	movs r6, #254
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #4
	movs r0, #0
	movs r2, #0
	bl 0x0200a4bc
.L_02001ff4_3:
	ldr r1, [r7, #104]
	ldr r2, [r7, #108]
	movs r0, #0
	bl 0x0200a47c
	adds r4, r7, #0
	adds r4, #128
	ldr r5, [r4]
	adds r4, #4
	ldr r4, [r4]
	ldr r0, [r7, #112]
	ldr r1, [r7, #116]
	ldr r2, [r7, #120]
	ldr r3, [r7, #124]
	str r5, [sp, #0]
	str r4, [sp, #4]
	bl 0x0200a404
	adds r3, r7, #0
	adds r3, #136
	adds r4, r7, #0
	ldr r0, [r3]
	adds r4, #152
	adds r3, #4
	ldr r1, [r3]
	ldr r5, [r4]
	adds r3, #4
	adds r4, #4
	ldr r2, [r3]
	ldr r4, [r4]
	adds r3, #4
	ldr r3, [r3]
	str r5, [sp, #0]
	str r4, [sp, #4]
	bl 0x0200a404
	adds r3, r7, #0
	adds r3, #160
	adds r4, r7, #0
	ldr r0, [r3]
	adds r4, #176
	adds r3, #4
	ldr r1, [r3]
	ldr r5, [r4]
	adds r3, #4
	ldr r2, [r3]
	adds r4, #4
	adds r3, #4
	ldr r4, [r4]
	ldr r3, [r3]
	str r5, [sp, #0]
	adds r5, r7, #0
	adds r5, #184
	str r4, [sp, #4]
	bl 0x0200a404
	ldr r0, [r5]
	bl 0x0200a46c
	adds r0, #90
	ldrb r3, [r0]
	ands r6, r3
	strb r6, [r0]
	adds r3, r7, #0
	adds r3, #188
	ldr r1, [r3]
	adds r3, #4
	ldr r2, [r3]
	ldr r0, [r5]
	bl 0x0200a484
	movs r0, #0
	bl 0x0200a46c
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	ldr r0, [r7, #96]
	bl 0x0200a444
	b .L_02001ff4_1
.L_02001ff4_4:
	mov r3, r8
	cmp r3, #0
	bne .L_02001ff4_1
	ldr r0, [pc, #120]
	bl 0x0200a434
	cmp r0, #0
	beq .L_02001ff4_1
	ldr r0, [pc, #112]
	bl 0x0200a434
	cmp r0, #0
	beq .L_02001ff4_6
	ldr r0, [pc, #108]
	bl 0x0200a434
	cmp r0, #0
	beq .L_02001ff4_6
	ldr r0, [pc, #100]
	bl 0x0200a434
	cmp r0, #0
	beq .L_02001ff4_6
	ldr r0, [pc, #72]
	bl 0x0200a434
	cmp r0, #0
	bne .L_02001ff4_1
	ldr r0, [pc, #60]
	bl 0x0200a43c
	bl 0x02008c8c
	b .L_02001ff4_1
.L_02001ff4_6:
	ldr r0, [pc, #72]
	bl 0x0200a434
	cmp r0, #0
	beq .L_02001ff4_1
	movs r0, #5
	bl 0x0200a52c
	movs r3, #1
	mov r8, r3
.L_02001ff4_1:
	mov r3, r8
	cmp r3, #1
	bne .L_02001ff4_7
	bl 0x0200a54c
	bl 0x0200a554
.L_02001ff4_7:
	bl 0x0200a45c
	sub sp, #-8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000080f
	.4byte 0x0200a980
	.4byte 0x00000818
	.4byte 0x0000080b
	.4byte 0x0000080d
	.4byte 0x0000080e
	.4byte 0x00000812
	.global Func_02002244
	.thumb_func
Func_02002244:
	push {lr}
	sub sp, #8
	bl 0x0200a454
	ldr r0, [pc, #152]
	bl 0x0200a434
	cmp r0, #0
	bne .L_02002244_0
	ldr r0, [pc, #148]
	bl 0x0200a434
	cmp r0, #0
	bne .L_02002244_0
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200a514
	movs r0, #143
	movs r1, #1
	movs r2, #146
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #17
	bl 0x0200a51c
	bl 0x0200a524
	movs r0, #186
	bl 0x0200a564
	movs r3, #4
	movs r2, #3
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #59
	movs r2, #15
	movs r3, #38
	bl 0x0200a404
	ldr r0, [pc, #80]
	bl 0x0200a434
	cmp r0, #0
	beq .L_02002244_1
	movs r3, #2
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #8
	movs r1, #60
	movs r2, #17
	movs r3, #39
	bl 0x0200a404
.L_02002244_1:
	movs r1, #0
	movs r2, #0
	movs r0, #0
	bl 0x0200a4f4
	movs r0, #30
	bl 0x0200a44c
	ldr r0, [pc, #32]
	bl 0x0200a43c
	ldr r0, [pc, #32]
	bl 0x0200a434
	cmp r0, #0
	beq .L_02002244_0
	bl 0x02008098
.L_02002244_0:
	bl 0x0200a45c
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000818
	.4byte 0x00000816
	.4byte 0x00000817
	.global Func_020022f4
	.thumb_func
Func_020022f4:
	push {lr}
	sub sp, #8
	bl 0x0200a454
	ldr r0, [pc, #152]
	bl 0x0200a434
	cmp r0, #0
	bne .L_020022f4_0
	ldr r0, [pc, #148]
	bl 0x0200a434
	cmp r0, #0
	bne .L_020022f4_0
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200a514
	movs r0, #143
	movs r1, #1
	movs r2, #146
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #17
	bl 0x0200a51c
	bl 0x0200a524
	movs r0, #186
	bl 0x0200a564
	movs r3, #4
	movs r2, #3
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #4
	movs r1, #59
	movs r2, #17
	movs r3, #38
	bl 0x0200a404
	ldr r0, [pc, #80]
	bl 0x0200a434
	cmp r0, #0
	beq .L_020022f4_1
	movs r3, #2
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #8
	movs r1, #60
	movs r2, #17
	movs r3, #39
	bl 0x0200a404
.L_020022f4_1:
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #0
	bl 0x0200a4f4
	movs r0, #30
	bl 0x0200a44c
	ldr r0, [pc, #32]
	bl 0x0200a43c
	ldr r0, [pc, #28]
	bl 0x0200a434
	cmp r0, #0
	beq .L_020022f4_0
	bl 0x02008098
.L_020022f4_0:
	bl 0x0200a45c
	sub sp, #-8
	pop {r0}
	bx r0
	.4byte 0x00000818
	.4byte 0x00000817
	.4byte 0x00000816
	.global Func_020023a4
	.thumb_func
Func_020023a4:
	push {r5, lr}
	adds r5, r1, #0
	movs r1, #0
	bl 0x0200a4ec
	adds r0, r5, #0
	bl 0x0200a44c
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.include "games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/IMPORT.INC"
	.section .rodata,"a",%progbits
	.4byte 0x01020041
	.4byte 0x000cccc0
	.4byte 0x060100cc
	.4byte 0x012005f8
	.4byte 0x0d200021
	.4byte 0x0c2d4000
	.4byte 0x01111101
	.4byte 0x010a1173
	.4byte 0x03002000
	.4byte 0x00000123
	.4byte 0x40000920
	.4byte 0xffff2324
	.4byte 0xf0ff0302
	.4byte 0x010100ff
	.4byte 0x05f71e04
	.4byte 0x00230322
	.4byte 0x01060420
	.4byte 0x050302f0
	.4byte 0x06200049
	.4byte 0x000d01e0
	.4byte 0x00000162
	.4byte 0xffff0000
	.4byte 0x0000011f
	.4byte 0x400000b0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000286
	.4byte 0x40000149
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000287
	.4byte 0x400001d6
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000240
	.4byte 0x40000088
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x00000240
	.4byte 0x40000088
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x00000240
	.4byte 0x40000088
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x00000240
	.4byte 0x40000088
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0063
	.4byte 0x00000240
	.4byte 0x40000088
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x0010200b
	.4byte 0x0020100d
	.4byte 0x0030400b
	.4byte 0x0040500b
	.4byte 0x0050600b
	.4byte 0x0060700b
	.4byte 0x00a011fe
	.4byte 0x000001ff
	.4byte 0xffff001e
	.4byte 0x00000001
	.4byte 0x06480000
	.4byte 0x00000000
	.4byte 0x06480000
	.4byte 0x00004000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00005000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x01005000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00003000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x01003000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0000b000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x0100b000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0000d000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x0100d000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x01200000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00004000
	.4byte 0xffff00cc
	.4byte 0x00000001
	.4byte 0x06480000
	.4byte 0x00000000
	.4byte 0x06480000
	.4byte 0x00004000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x02300000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00004000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x02500000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte 0x02008fe5
	.4byte 0x00000202
	.4byte 0xffff000b
	.4byte 0x02009001
	.4byte 0x00008c15
	.4byte 0x080b0009
	.4byte 0x02009575
	.4byte 0x00008c15
	.4byte 0x080c000b
	.4byte 0x0200958d
	.4byte 0x00008c15
	.4byte 0x080d000d
	.4byte 0x020095a5
	.4byte 0x00008c15
	.4byte 0x080e000f
	.4byte 0x020095bd
	.4byte 0x00000602
	.4byte 0x080b0005
	.4byte 0x020099e5
	.4byte 0x00008602
	.4byte 0x080c0006
	.4byte 0x02009a05
	.4byte 0x00000602
	.4byte 0x080d0007
	.4byte 0x02009a25
	.4byte 0x00008602
	.4byte 0x080e0008
	.4byte 0x02009a45
	.4byte 0x10008c15
	.4byte 0x0816000a
	.4byte 0x0200966d
	.4byte 0x00008c15
	.4byte 0x0816000a
	.4byte 0x020095d5
	.4byte 0x10008c15
	.4byte 0x0817000c
	.4byte 0x020096a5
	.4byte 0x00008c15
	.4byte 0x0817000c
	.4byte 0x020095fd
	.4byte 0x00000602
	.4byte 0x08160014
	.4byte 0x02009a65
	.4byte 0x00000602
	.4byte 0x08160015
	.4byte 0x02009a65
	.4byte 0x00004602
	.4byte 0x08160015
	.4byte 0x02009a85
	.4byte 0x0000c602
	.4byte 0x08160017
	.4byte 0x02009aad
	.4byte 0x0000c602
	.4byte 0x08160018
	.4byte 0x02009aad
	.4byte 0x00008602
	.4byte 0x08170019
	.4byte 0x02009ad5
	.4byte 0x00008602
	.4byte 0x0817001a
	.4byte 0x02009ad5
	.4byte 0x00004602
	.4byte 0x0817001a
	.4byte 0x02009af5
	.4byte 0x0000c602
	.4byte 0x0817001c
	.4byte 0x02009b1d
	.4byte 0x0000c602
	.4byte 0x0817001d
	.4byte 0x02009b1d
	.4byte 0x10008c15
	.4byte 0x08180011
	.4byte 0x02009625
	.4byte 0x00008c15
	.4byte 0x08180011
	.4byte 0x020096dd
	.4byte 0x0000c602
	.4byte 0x08180020
	.4byte 0x0200995d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
