.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/KORIMA_KI/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02009ba4
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02009c04
	.global Func_02000040
	.thumb_func
Func_02000040:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02009c24
	.global Func_02000048
	.thumb_func
Func_02000048:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02009c34
	.global Func_02000050
	.thumb_func
Func_02000050:
	push {r5, lr}
	ldr r3, [pc, #40]
	ldr r5, [r3]
	bl 0x02009998
	movs r1, #0
	movs r2, #0
	movs r0, #0
	bl 0x02009aa8
	movs r3, #182
	lsls r3, r3, #1
	adds r5, r5, r3
	movs r3, #0
	ldrsh r0, [r5, r3]
	bl 0x02009a70
	bl 0x020099a0
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.global Func_02000080
	.thumb_func
Func_02000080:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02009d3c
	.global Func_02000088
	.thumb_func
Func_02000088:
	push {lr}
	bl 0x02009998
	movs r0, #11
	movs r1, #1
	bl 0x020092f4
	ldr r0, [pc, #172]
	bl 0x02009980
	cmp r0, #0
	beq .L_02000088_0
	ldr r0, [pc, #164]
	b .L_02000088_1
.L_02000088_0:
	ldr r0, [pc, #164]
	bl 0x02009980
	cmp r0, #0
	beq .L_02000088_2
	ldr r0, [pc, #160]
.L_02000088_1:
	bl 0x02009a20
	movs r0, #9
	movs r1, #0
	bl 0x02009a30
	b .L_02000088_3
.L_02000088_2:
	ldr r0, [pc, #148]
	bl 0x02009a20
	movs r2, #20
	movs r0, #9
	movs r1, #0
	bl 0x02009a38
	movs r1, #0
	movs r0, #11
	bl 0x020092f4
	movs r0, #60
	bl 0x02009990
	movs r0, #11
	movs r1, #1
	bl 0x020092f4
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl 0x02009a38
	movs r1, #3
	movs r0, #0
	bl 0x020099f8
	movs r0, #40
	bl 0x02009990
	movs r0, #9
	movs r1, #0
	bl 0x02009a30
	movs r1, #0
	movs r0, #11
	bl 0x020092f4
	movs r0, #80
	bl 0x02009990
	movs r2, #20
	movs r0, #9
	movs r1, #0
	bl 0x02009a38
	movs r0, #11
	movs r1, #1
	bl 0x020092f4
	movs r0, #9
	movs r1, #0
	movs r2, #20
	bl 0x02009a38
	ldr r0, [pc, #28]
	bl 0x02009988
.L_02000088_3:
	movs r0, #11
	movs r1, #0
	bl 0x020092f4
	bl 0x020099a0
	pop {r0}
	bx r0
	.4byte 0x00000845
	.4byte 0x0000151d
	.4byte 0x0000084c
	.4byte 0x00001525
	.4byte 0x00001520
	.global Func_02000158
	.thumb_func
Func_02000158:
	push {lr}
	bl 0x02009998
	ldr r0, [pc, #200]
	bl 0x02009980
	cmp r0, #0
	beq .L_02000158_0
	movs r1, #1
	movs r0, #10
	bl 0x020092f4
	ldr r0, [pc, #184]
	bl 0x02009a20
	movs r0, #8
	movs r1, #0
	bl 0x02009a30
	movs r0, #10
	movs r1, #0
	bl 0x020092f4
	b .L_02000158_1
.L_02000158_0:
	ldr r0, [pc, #164]
	bl 0x02009980
	cmp r0, #0
	beq .L_02000158_2
	movs r1, #1
	movs r0, #10
	bl 0x020092f4
	ldr r0, [pc, #152]
	bl 0x02009a20
	movs r0, #8
	movs r1, #0
	bl 0x02009a30
	movs r1, #0
	movs r0, #10
	bl 0x020092f4
	movs r0, #184
	bl 0x02009978
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	beq .L_02000158_1
	ldr r3, [pc, #120]
	movs r1, #185
	ldr r3, [r3]
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #1
	strh r3, [r2]
	b .L_02000158_1
.L_02000158_2:
	ldr r0, [pc, #108]
	bl 0x02009a20
	movs r0, #8
	movs r1, #0
	bl 0x02009a30
	movs r1, #1
	ldr r0, [pc, #96]
	bl 0x02009a78
	movs r0, #20
	bl 0x02009a88
	movs r0, #40
	bl 0x02009908
	movs r2, #10
	ldr r0, [pc, #80]
	movs r1, #0
	bl 0x02009a38
	movs r0, #0
	movs r1, #2
	bl 0x02009a10
	ldr r0, [pc, #64]
	movs r1, #0
	bl 0x02009a30
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	bl 0x02009a78
	movs r0, #20
	bl 0x02009a88
	movs r0, #40
	bl 0x02009908
.L_02000158_1:
	bl 0x020099a0
	pop {r0}
	bx r0
	.4byte 0x00000845
	.4byte 0x0000151c
	.4byte 0x00000844
	.4byte 0x000014eb
	.4byte 0x03001ebc
	.4byte 0x000014c9
	.4byte 0x00406218
	.4byte 0x0000200e
	.global Func_02000248
	.thumb_func
Func_02000248:
	push {lr}
	bl 0x02009998
	ldr r0, [pc, #40]
	bl 0x02009980
	cmp r0, #0
	beq .L_02000248_0
	ldr r0, [pc, #32]
	bl 0x02009a20
	b .L_02000248_1
.L_02000248_0:
	ldr r0, [pc, #28]
	bl 0x02009a20
.L_02000248_1:
	movs r0, #9
	movs r1, #0
	bl 0x02009a30
	bl 0x020099a0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000845
	.4byte 0x0000151f
	.4byte 0x000014c8
	.global Func_02000284
	.thumb_func
Func_02000284:
	push {lr}
	bl 0x02009998
	ldr r0, [pc, #40]
	bl 0x02009980
	cmp r0, #0
	beq .L_02000284_0
	ldr r0, [pc, #32]
	bl 0x02009a20
	b .L_02000284_1
.L_02000284_0:
	ldr r0, [pc, #28]
	bl 0x02009a20
.L_02000284_1:
	movs r0, #8
	movs r1, #0
	bl 0x02009a30
	bl 0x020099a0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000845
	.4byte 0x0000151e
	.4byte 0x000014ec
	.global Func_020002c0
	.thumb_func
Func_020002c0:
	push {lr}
	bl 0x02009998
	bl 0x02009a90
	ldr r0, [pc, #28]
	bl 0x02009980
	cmp r0, #0
	bne .L_020002c0_0
	bl 0x020082ec
	b .L_020002c0_1
.L_020002c0_0:
	bl 0x02008488
.L_020002c0_1:
	bl 0x020099a0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000844
	.global Func_020002ec
	.thumb_func
Func_020002ec:
	push {r5, r6, lr}
	movs r0, #0
	bl 0x020099b0
	movs r1, #192
	movs r2, #0
	adds r6, r0, #0
	lsls r1, r1, #8
	movs r0, #0
	bl 0x02009a40
	movs r1, #1
	ldr r0, [pc, #360]
	bl 0x02009a78
	movs r0, #20
	bl 0x02009a88
	movs r0, #40
	bl 0x02009908
	movs r0, #17
	bl 0x02009ab0
	ldr r5, [pc, #340]
	movs r3, #1
	movs r1, #200
	str r3, [r5]
	lsls r1, r1, #4
	ldr r0, [pc, #336]
	bl 0x02009910
	movs r0, #30
	bl 0x02009908
	movs r3, #0
	movs r0, #164
	movs r1, #1
	movs r2, #235
	lsls r2, r2, #16
	str r3, [r5]
	lsls r0, r0, #17
	movs r3, #1
	negs r1, r1
	bl 0x02009a60
	movs r1, #1
	movs r0, #0
	bl 0x02009a48
	movs r0, #0
	bl 0x020099b0
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #16
	movs r0, #0
	bl 0x020099f0
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #10
	movs r0, #0
	bl 0x020099b8
	movs r0, #133
	bl 0x02009ab0
	movs r3, #160
	lsls r3, r3, #11
	str r3, [r6, #40]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r6, #72]
	movs r3, #160
	lsls r3, r3, #8
	movs r2, #129
	str r3, [r6, #68]
	movs r0, #0
	ldr r1, [pc, #232]
	lsls r2, r2, #1
	bl 0x020099d8
	ldr r3, [r6, #40]
	cmp r3, #0
	blt .L_020002ec_0
.L_020002ec_1:
	movs r0, #1
	bl 0x02009908
	ldr r3, [r6, #40]
	cmp r3, #0
	bge .L_020002ec_1
.L_020002ec_0:
	movs r0, #1
	bl 0x02009908
	ldr r3, [r6, #40]
	cmp r3, #0
	ble .L_020002ec_0
	movs r0, #161
	bl 0x02009ab0
	movs r1, #19
	movs r0, #0
	bl 0x020099f0
	movs r0, #120
	bl 0x02009990
	movs r5, #128
	ldr r0, [pc, #168]
	bl 0x02009918
	lsls r5, r5, #7
	movs r0, #40
	bl 0x02009908
	str r5, [r6, #68]
	movs r0, #0
	bl 0x020099b0
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #80
	bl 0x02009990
	ldr r0, [pc, #136]
	bl 0x02009a20
	movs r2, #20
	ldr r0, [pc, #132]
	movs r1, #0
	bl 0x02009a38
	movs r1, #2
	movs r0, #0
	bl 0x02009a10
	movs r0, #20
	bl 0x02009990
	ldr r0, [pc, #112]
	movs r1, #0
	bl 0x02009a30
	bl 0x02009aa0
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl 0x02009a78
	movs r0, #20
	bl 0x02009a88
	movs r0, #40
	bl 0x02009908
	movs r3, #192
	lsls r3, r3, #8
	strh r3, [r6, #6]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #72]
	movs r1, #2
	str r5, [r6, #68]
	movs r0, #0
	bl 0x02009a10
	movs r0, #40
	bl 0x02009990
	movs r2, #0
	movs r0, #0
	movs r1, #4
	bl 0x02009a00
	movs r0, #0
	movs r1, #1
	bl 0x020099f0
	movs r0, #20
	bl 0x02009990
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00406218
	.4byte 0x02009dd0
	.4byte 0x02009219
	.4byte 0x0000014f
	.4byte 0x000014cc
	.4byte 0x0000200e
	.global Func_02000488
	.thumb_func
Func_02000488:
	push {r5, r6, lr}
	movs r0, #3
	bl 0x02009980
	movs r1, #164
	lsls r1, r1, #1
	movs r2, #212
	adds r6, r0, #0
	movs r0, #0
	bl 0x020099e0
	movs r1, #192
	movs r2, #20
	lsls r1, r1, #8
	movs r0, #0
	bl 0x02009a40
	movs r0, #17
	bl 0x02009ab0
	ldr r0, [pc, #952]
	movs r1, #1
	bl 0x02009960
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x020099b8
	movs r1, #128
	movs r2, #128
	movs r0, #2
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x020099b8
	movs r0, #0
	bl 0x020099b0
	cmp r0, #0
	beq .L_02000488_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x020099e8
.L_02000488_0:
	movs r0, #0
	bl 0x020099b0
	cmp r0, #0
	beq .L_02000488_1
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #2
	bl 0x020099e8
.L_02000488_1:
	ldr r1, [pc, #880]
	movs r0, #1
	bl 0x020099c0
	ldr r1, [pc, #876]
	movs r0, #2
	bl 0x020099c0
	cmp r6, #0
	beq .L_02000488_2
	movs r1, #128
	movs r2, #128
	movs r0, #3
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x020099b8
	movs r0, #0
	bl 0x020099b0
	cmp r0, #0
	beq .L_02000488_3
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #3
	bl 0x020099e8
.L_02000488_3:
	ldr r1, [pc, #836]
	movs r0, #3
	bl 0x020099c0
.L_02000488_2:
	movs r0, #2
	bl 0x020099c8
	movs r0, #40
	bl 0x02009990
	movs r0, #0
	bl 0x020098b8
	movs r0, #32
	bl 0x02009a88
	movs r0, #40
	bl 0x02009908
	ldr r5, [pc, #800]
	movs r3, #0
	movs r1, #200
	str r3, [r5]
	lsls r1, r1, #4
	ldr r0, [pc, #796]
	bl 0x02009910
	movs r0, #40
	bl 0x02009990
	movs r1, #192
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #7
	bl 0x02009a40
	ldr r0, [pc, #776]
	ldr r1, [pc, #776]
	bl 0x02009a58
	movs r0, #128
	movs r1, #1
	movs r2, #254
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #17
	bl 0x02009a60
	bl 0x02009a68
	movs r0, #246
	bl 0x02009ab0
	movs r0, #40
	bl 0x02009990
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #6
	movs r2, #20
	bl 0x02009a40
	movs r1, #1
	movs r3, #1
	negs r1, r1
	ldr r2, [pc, #724]
	ldr r0, [pc, #724]
	bl 0x02009a60
	bl 0x02009a68
.L_020005c2:
	movs r0, #246
	bl 0x02009ab0
	movs r0, #40
	bl 0x02009990
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009a40
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #7
	movs r2, #20
	bl 0x02009a40
	movs r0, #163
	movs r1, #1
	movs r2, #192
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	bl 0x02009a60
	bl 0x02009a68
	movs r0, #246
	bl 0x02009ab0
	ldr r3, [r5]
	cmp r3, #24
	beq .L_020005c2_0
.L_020005c2_1:
	movs r0, #1
	bl 0x02009908
	ldr r3, [r5]
	cmp r3, #24
	bne .L_020005c2_1
.L_020005c2_0:
	ldr r0, [pc, #616]
	bl 0x02009918
	movs r0, #10
	bl 0x02009908
	movs r5, #0
.L_020005c2_2:
	movs r0, #0
	bl 0x020098b8
	movs r0, #6
	bl 0x02009a88
	movs r0, #6
	bl 0x02009908
	movs r0, #1
	bl 0x020098b8
	movs r0, #6
	bl 0x02009a88
	adds r5, #1
	movs r0, #6
	bl 0x02009908
	cmp r5, #3
	bls .L_020005c2_2
	movs r0, #0
	bl 0x020098b8
	movs r0, #40
	bl 0x02009a88
	movs r0, #80
	bl 0x02009908
	movs r0, #164
	movs r1, #128
	movs r2, #212
	movs r3, #1
	lsls r2, r2, #16
	lsls r1, r1, #12
	lsls r0, r0, #17
	bl 0x02009a60
	bl 0x02009a68
	movs r0, #40
	bl 0x02009990
	movs r1, #1
	movs r0, #10
	bl 0x020092f4
	movs r0, #40
	bl 0x02009990
	movs r0, #7
	bl 0x02009ab0
	ldr r0, [pc, #516]
	bl 0x02009a20
	movs r0, #8
	movs r1, #0
	bl 0x02009a30
	movs r0, #0
	movs r1, #2
	bl 0x02009a08
	movs r0, #1
	movs r1, #2
	bl 0x02009a08
	movs r0, #3
	movs r1, #2
	bl 0x02009a08
	movs r0, #2
	movs r1, #2
	bl 0x02009a10
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009a40
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009a40
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009a40
	movs r1, #192
	movs r2, #20
	movs r0, #2
	lsls r1, r1, #8
	bl 0x02009a40
	movs r1, #2
	movs r0, #10
	bl 0x020092f4
	movs r0, #20
	bl 0x02009990
	movs r1, #3
	movs r0, #10
	bl 0x020092f4
	movs r0, #40
	bl 0x02009990
	movs r1, #1
	movs r0, #10
	bl 0x020092f4
	movs r0, #20
	bl 0x02009990
	movs r0, #8
	movs r1, #0
	bl 0x02009a30
	movs r0, #0
	ldr r1, [pc, #372]
	movs r2, #0
	bl 0x02009a50
	movs r0, #1
	ldr r1, [pc, #364]
	movs r2, #0
	bl 0x02009a50
	movs r0, #3
	ldr r1, [pc, #352]
	movs r2, #0
	bl 0x02009a50
	movs r0, #2
	ldr r1, [pc, #344]
	movs r2, #40
	bl 0x02009a50
	movs r0, #234
	movs r2, #232
	movs r3, #1
	lsls r2, r2, #16
	movs r1, #0
	lsls r0, r0, #16
	bl 0x02009a60
	bl 0x02009a68
	movs r0, #40
	bl 0x02009990
	movs r1, #1
	movs r0, #11
.L_02000764:
	bl 0x020092f4
	movs r0, #40
	bl 0x02009990
	movs r1, #3
	movs r0, #11
	bl 0x020092f4
	movs r0, #20
	bl 0x02009990
	movs r2, #20
	ldr r0, [pc, #284]
	movs r1, #0
	bl 0x02009a38
	movs r1, #2
	movs r0, #11
	bl 0x020092f4
	movs r0, #10
	bl 0x02009990
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009a40
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009a40
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009a40
	movs r1, #192
	movs r2, #20
	movs r0, #3
	lsls r1, r1, #7
	bl 0x02009a40
	movs r1, #3
	movs r0, #11
	bl 0x020092f4
	movs r0, #20
	bl 0x02009990
	movs r1, #2
	movs r0, #11
	bl 0x020092f4
	movs r0, #20
	bl 0x02009990
	movs r0, #11
	movs r1, #3
	bl 0x020092f4
	movs r2, #10
	ldr r0, [pc, #176]
	movs r1, #0
	bl 0x02009a38
	movs r1, #0
	movs r0, #10
	bl 0x020092f4
	movs r0, #20
	bl 0x02009990
	ldr r0, [pc, #156]
	movs r1, #0
	bl 0x02009a30
	movs r1, #1
	movs r0, #10
	bl 0x020092f4
	movs r0, #20
	bl 0x02009990
	movs r1, #0
	ldr r0, [pc, #132]
	bl 0x02009a28
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009a40
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009a40
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009a40
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009a40
	movs r0, #0
	movs r1, #0
	bl 0x020099a8
	cmp r0, #0
	bne .L_02000764_0
	ldr r0, [pc, #64]
	movs r1, #0
	bl 0x02009a30
	ldr r0, [pc, #60]
	movs r1, #0
	bl 0x02009a30
	b .L_02000764_1
	.2byte 0x14ed
	.2byte 0x0000
	.2byte 0x9ab8
	.2byte 0x0200
	.2byte 0x9af4
	.2byte 0x0200
	.2byte 0x9b30
	.2byte 0x0200
	.2byte 0x9dd4
	.2byte 0x0200
	.2byte 0x92b5
	.2byte 0x0200
	.2byte 0x3333
	.2byte 0x0003
	.2byte 0x6666
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0105
	.2byte 0x0000
	.2byte 0x019d
	.2byte 0x14ee
	.2byte 0x0000
	.2byte 0x0105
	.2byte 0x0000
	.4byte 0x00004009
	.4byte 0x00008008
.L_02000764_0:
	ldr r5, [pc, #496]
	movs r3, #236
	ldr r2, [r5]
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #2
	strh r3, [r2]
	movs r0, #3
	ldr r1, [pc, #484]
	movs r2, #0
	bl 0x02009a50
	movs r0, #1
	ldr r1, [pc, #472]
	movs r2, #0
	bl 0x02009a50
	movs r0, #2
	ldr r1, [pc, #464]
	movs r2, #40
	bl 0x02009a50
	movs r0, #1
	movs r1, #4
	bl 0x020099f0
	movs r0, #1
	movs r1, #0
	bl 0x02009a30
	cmp r6, #0
	beq .L_02000764_2
	movs r0, #3
	movs r1, #2
	bl 0x02009a10
	movs r0, #3
	movs r1, #0
	bl 0x02009a30
	b .L_02000764_3
.L_02000764_2:
	ldr r2, [r5]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02000764_3:
	movs r0, #2
	movs r1, #3
	bl 0x020099f8
	movs r0, #2
	movs r1, #0
	bl 0x02009a30
	ldr r0, [pc, #392]
	movs r1, #0
	bl 0x02009a30
	ldr r0, [pc, #388]
	movs r1, #0
	bl 0x02009a30
.L_02000764_1:
	movs r0, #0
	movs r1, #3
	bl 0x020099f0
	movs r0, #1
	movs r1, #3
	bl 0x020099f0
	movs r0, #3
	movs r1, #3
	bl 0x020099f0
	movs r0, #2
	movs r1, #3
	bl 0x020099f8
	movs r0, #164
	movs r1, #128
	movs r2, #212
	lsls r2, r2, #16
	movs r3, #1
	lsls r1, r1, #12
	lsls r0, r0, #17
	bl 0x02009a60
	bl 0x02009a68
	movs r0, #20
	bl 0x02009990
	movs r1, #0
	movs r0, #10
	bl 0x020092f4
	movs r0, #20
	bl 0x02009990
	movs r0, #0
	bl 0x020098b8
	movs r0, #1
	bl 0x02009a88
	movs r0, #1
	bl 0x02009908
	movs r1, #1
	ldr r0, [pc, #288]
	bl 0x02009a78
	movs r0, #40
	bl 0x02009a88
	movs r0, #60
	bl 0x02009990
	ldr r2, [pc, #276]
	movs r3, #0
	str r3, [r2]
	ldr r2, [pc, #272]
	movs r3, #164
	lsls r3, r3, #17
	str r3, [r2]
	movs r3, #192
	lsls r3, r3, #14
	str r3, [r2, #4]
	ldr r5, [pc, #264]
	movs r3, #205
	lsls r3, r3, #16
	movs r1, #200
	str r3, [r2, #8]
	lsls r1, r1, #4
	adds r0, r5, #0
	bl 0x02009910
	movs r0, #100
	bl 0x02009990
	adds r0, r5, #0
	bl 0x02009918
	movs r1, #0
	ldr r0, [pc, #236]
	bl 0x02009a78
	movs r0, #60
	bl 0x02009a88
	movs r0, #100
	bl 0x02009990
	movs r0, #0
	bl 0x020098b8
	movs r0, #20
	bl 0x02009a88
	movs r0, #40
	bl 0x02009990
	movs r1, #1
	movs r0, #10
	bl 0x020092f4
	movs r0, #10
	bl 0x02009990
	ldr r0, [pc, #188]
	bl 0x02009a20
	ldr r0, [pc, #160]
	movs r1, #0
	bl 0x02009a30
	movs r0, #0
	movs r1, #3
	bl 0x020099f0
	movs r0, #1
	movs r1, #3
	bl 0x020099f0
	movs r0, #3
	movs r1, #3
	bl 0x020099f0
	movs r0, #2
	movs r1, #3
	bl 0x020099f8
	movs r0, #234
	movs r2, #232
	movs r3, #1
	lsls r2, r2, #16
	movs r1, #0
	lsls r0, r0, #16
	bl 0x02009a60
	bl 0x02009a68
	movs r0, #20
	bl 0x02009990
	ldr r0, [pc, #88]
	movs r1, #0
	bl 0x02009a30
	movs r2, #10
	ldr r0, [pc, #84]
	movs r1, #0
	bl 0x02009a38
	movs r0, #1
	movs r1, #2
	bl 0x02009a10
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009a40
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #10
	bl 0x02009a40
	movs r1, #0
	movs r0, #1
	bl 0x02009a28
	movs r0, #0
	movs r1, #0
	bl 0x020099a8
	cmp r0, #0
	bne .L_02000764_4
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #40
	bl 0x02009a50
	b .L_02000764_5
	.4byte 0x03001ebc
	.4byte 0x00000103
	.4byte 0x00004009
	.4byte 0x00008008
	.4byte 0x00406218
	.4byte 0x02009dcc
	.4byte 0x02009dc0
	.4byte 0x020095a1
	.4byte 0x00007fff
	.4byte 0x000014fb
.L_02000764_4:
	movs r0, #1
	movs r1, #4
	bl 0x020099f8
	ldr r3, [pc, #1012]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02000764_5:
	movs r0, #1
	movs r1, #0
	bl 0x02009a30
	movs r1, #4
	movs r0, #10
	bl 0x020092f4
	movs r0, #20
	bl 0x02009990
	ldr r0, [pc, #980]
	bl 0x02009a20
	ldr r0, [pc, #976]
	movs r1, #0
	bl 0x02009a30
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009a40
	movs r1, #192
	movs r2, #0
	movs r0, #1
	lsls r1, r1, #8
	bl 0x02009a40
	ldr r0, [pc, #944]
	movs r1, #0
	bl 0x02009a30
	movs r0, #0
	movs r1, #3
	bl 0x020099f0
	movs r0, #1
	movs r1, #3
	bl 0x020099f0
	movs r0, #3
	movs r1, #3
	bl 0x020099f0
	movs r0, #2
	movs r1, #3
	bl 0x020099f8
	movs r1, #4
	movs r0, #10
	bl 0x020092f4
	movs r0, #20
	bl 0x02009990
	movs r2, #20
	ldr r0, [pc, #888]
	movs r1, #0
	bl 0x02009a38
	movs r0, #11
	movs r1, #0
	bl 0x020092f4
	movs r2, #20
	ldr r0, [pc, #876]
	movs r1, #0
	bl 0x02009a38
	movs r1, #3
	movs r0, #11
	bl 0x020092f4
	movs r0, #40
	bl 0x02009990
	movs r1, #1
	movs r0, #11
	bl 0x020092f4
	movs r0, #20
	bl 0x02009990
	movs r2, #20
	ldr r0, [pc, #836]
	movs r1, #0
	bl 0x02009a38
	movs r1, #2
	movs r0, #10
	bl 0x020092f4
	movs r0, #20
	bl 0x02009990
	ldr r0, [pc, #812]
	movs r1, #0
	bl 0x02009a30
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x02009a50
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x02009a50
	movs r1, #129
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #0
	bl 0x02009a50
	movs r1, #129
	movs r2, #80
	movs r0, #2
	lsls r1, r1, #1
	bl 0x02009a50
	movs r1, #5
	movs r0, #11
	bl 0x020092f4
	movs r0, #60
	bl 0x02009990
	movs r1, #3
	movs r0, #11
	bl 0x020092f4
	movs r0, #20
	bl 0x02009990
	movs r2, #20
	ldr r0, [pc, #728]
	movs r1, #0
	bl 0x02009a38
	movs r1, #5
	movs r0, #10
	bl 0x020092f4
	movs r0, #40
	bl 0x02009990
	movs r1, #2
	movs r0, #10
	bl 0x020092f4
	movs r0, #20
	bl 0x02009990
	movs r2, #20
	ldr r0, [pc, #696]
	movs r1, #0
	bl 0x02009a38
	movs r0, #1
	movs r1, #2
	bl 0x02009a10
	movs r1, #128
	movs r2, #10
	movs r0, #1
	lsls r1, r1, #8
	bl 0x02009a40
	movs r0, #1
	movs r1, #0
	bl 0x02009a30
	movs r1, #128
	movs r2, #20
	movs r0, #2
	lsls r1, r1, #8
	bl 0x02009a40
	ldr r0, [pc, #652]
	movs r1, #0
	bl 0x02009a30
	movs r1, #4
	movs r0, #11
	bl 0x020092f4
	movs r0, #20
	bl 0x02009990
	ldr r0, [pc, #620]
	movs r1, #0
	movs r2, #20
	bl 0x02009a38
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009a40
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #10
	bl 0x02009a40
	movs r2, #20
	movs r0, #1
	movs r1, #0
	bl 0x02009a38
	movs r0, #10
	movs r1, #1
	bl 0x020092f4
	movs r2, #10
	ldr r0, [pc, #564]
	movs r1, #0
	bl 0x02009a38
	movs r1, #2
	movs r0, #10
	bl 0x020092f4
	movs r0, #20
	bl 0x02009990
	movs r1, #3
	movs r0, #11
	bl 0x020092f4
	movs r0, #40
	bl 0x02009990
	movs r1, #0
	movs r0, #11
	bl 0x020092f4
	movs r0, #20
	bl 0x02009990
	movs r0, #0
	bl 0x020098b8
	movs r0, #1
	bl 0x02009a88
	movs r0, #1
	bl 0x02009908
	movs r1, #1
	ldr r0, [pc, #508]
	bl 0x02009a78
	movs r0, #40
	bl 0x02009a88
	movs r0, #60
	bl 0x02009990
	ldr r2, [pc, #496]
	movs r3, #0
	str r3, [r2]
	ldr r2, [pc, #492]
	movs r3, #136
	lsls r3, r3, #16
	str r3, [r2]
	movs r3, #160
	lsls r3, r3, #13
	str r3, [r2, #4]
	ldr r5, [pc, #484]
	movs r3, #129
	lsls r3, r3, #17
	movs r1, #200
	str r3, [r2, #8]
	lsls r1, r1, #4
	adds r0, r5, #0
	bl 0x02009910
	movs r0, #100
	bl 0x02009990
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009a40
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009a40
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009a40
	movs r1, #192
	movs r2, #40
	movs r0, #2
	lsls r1, r1, #7
	bl 0x02009a40
	movs r0, #2
	movs r1, #1
	bl 0x02009a08
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #20
	bl 0x02009a50
	movs r2, #10
	ldr r0, [pc, #372]
	movs r1, #0
	bl 0x02009a38
	movs r0, #0
	movs r1, #2
	bl 0x02009a10
	movs r1, #128
	movs r2, #10
	movs r0, #0
	lsls r1, r1, #6
	bl 0x02009a40
	movs r0, #0
	movs r1, #3
	bl 0x020099f8
	movs r1, #4
	movs r0, #10
	bl 0x020092f4
	movs r0, #20
	bl 0x02009990
	ldr r0, [pc, #312]
	movs r1, #0
	bl 0x02009a30
	movs r0, #2
	ldr r1, [pc, #332]
	movs r2, #60
	bl 0x02009a50
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #10
	bl 0x02009a40
	ldr r0, [pc, #292]
	movs r1, #0
	movs r2, #10
	bl 0x02009a38
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009a40
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009a40
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #20
	bl 0x02009a40
	movs r2, #10
	ldr r0, [pc, #232]
	movs r1, #0
	bl 0x02009a38
	movs r0, #0
	movs r1, #3
	bl 0x020099f0
	movs r0, #1
	movs r1, #3
	bl 0x020099f0
	movs r0, #3
	movs r1, #3
	bl 0x020099f0
	movs r1, #3
	movs r0, #2
	bl 0x020099f8
	movs r0, #10
	bl 0x02009990
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009a40
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009a40
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009a40
	movs r1, #192
	movs r2, #120
	lsls r1, r1, #7
	movs r0, #2
	bl 0x02009a40
	adds r0, r5, #0
	bl 0x02009918
	movs r0, #60
	bl 0x02009990
	movs r0, #0
	bl 0x020098b8
	movs r0, #40
	bl 0x02009a88
	movs r1, #2
	movs r0, #10
	bl 0x020092f4
	movs r0, #20
	bl 0x02009990
	movs r2, #20
	ldr r0, [pc, #100]
	movs r1, #0
	bl 0x02009a38
	movs r0, #11
	movs r1, #3
	bl 0x020092f4
	ldr r0, [pc, #88]
	movs r1, #0
	bl 0x02009a30
	ldr r0, [pc, #76]
	movs r1, #0
	bl 0x02009a30
	movs r1, #4
	movs r0, #11
	bl 0x020092f4
	movs r0, #20
	bl 0x02009990
	movs r2, #10
	ldr r0, [pc, #56]
	movs r1, #0
	bl 0x02009a38
	movs r0, #0
	movs r1, #2
	bl 0x02009a08
	movs r0, #1
	movs r1, #2
	bl 0x02009a08
	movs r0, #3
	movs r1, #2
	bl 0x02009a08
	movs r0, #2
	movs r1, #2
	bl 0x02009a10
	movs r0, #10
	movs r1, #1
	b .L_02000764_6
	.4byte 0x03001ebc
	.4byte 0x00001501
	.4byte 0x00008008
	.4byte 0x00004009
	.4byte 0x00004008
	.4byte 0x00008002
	.4byte 0x00406218
	.4byte 0x02009dcc
	.4byte 0x02009dc0
	.4byte 0x020095a1
	.4byte 0x00000101
.L_02000764_6:
	bl 0x020092f4
	movs r1, #0
	ldr r0, [pc, #356]
	bl 0x02009a28
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009a40
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009a40
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009a40
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009a40
	movs r0, #0
	movs r1, #0
	bl 0x020099a8
	cmp r0, #1
	bne .L_02000764_7
	ldr r3, [pc, #292]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02000764_7:
	movs r0, #10
	bl 0x02009990
	movs r1, #2
	movs r0, #10
	bl 0x020092f4
	movs r0, #20
	bl 0x02009990
	movs r1, #3
	movs r0, #11
	bl 0x020092f4
	movs r0, #40
	bl 0x02009990
	movs r1, #1
	movs r0, #10
	bl 0x020092f4
	movs r0, #20
	bl 0x02009990
	ldr r0, [pc, #224]
	movs r1, #0
	movs r2, #10
	bl 0x02009a38
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009a40
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x02009a40
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009a40
	movs r1, #128
	movs r2, #10
	movs r0, #2
	lsls r1, r1, #8
	bl 0x02009a40
	movs r0, #0
	movs r1, #3
	bl 0x020099f0
	movs r0, #1
	movs r1, #3
	bl 0x020099f0
	movs r0, #3
	movs r1, #3
	bl 0x020099f0
	movs r1, #3
	movs r0, #2
	bl 0x020099f8
	movs r0, #17
	bl 0x02009ab0
	ldr r5, [pc, #140]
	movs r0, #1
	adds r1, r5, #0
	bl 0x020099c0
	cmp r6, #0
	beq .L_02000764_8
	movs r0, #3
	adds r1, r5, #0
	bl 0x020099c0
.L_02000764_8:
	adds r1, r5, #0
	movs r0, #2
	bl 0x020099d0
	movs r0, #10
	movs r1, #4
	bl 0x020092f4
	movs r1, #4
	movs r0, #10
	bl 0x020092f4
	movs r0, #20
	bl 0x02009990
	ldr r0, [pc, #92]
	bl 0x02009a20
	ldr r0, [pc, #72]
	movs r1, #0
	bl 0x02009a30
	movs r0, #11
	movs r1, #4
	bl 0x020092f4
	movs r1, #4
	movs r0, #11
	bl 0x020092f4
	movs r0, #20
	bl 0x02009990
	movs r2, #10
	ldr r0, [pc, #56]
	movs r1, #0
	bl 0x02009a38
	movs r1, #3
	movs r0, #0
	bl 0x020099f8
	ldr r0, [pc, #44]
	bl 0x02009988
	movs r0, #1
	bl 0x02009ab0
	movs r0, #184
	movs r1, #185
	bl 0x0200972c
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00008008
	.4byte 0x03001ebc
	.4byte 0x02009b6c
	.4byte 0x00001519
	.4byte 0x00004009
	.4byte 0x00000845
	.global Func_02001070
	.thumb_func
Func_02001070:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #10
	bl 0x020099b0
	mov r8, r0
	movs r0, #14
	bl 0x020099b0
	mov r10, r0
	movs r0, #11
	bl 0x020099b0
	adds r7, r0, #0
	movs r0, #1
	bl 0x02009908
	movs r0, #14
	movs r1, #15
	bl 0x02009a18
	ldr r3, [pc, #56]
	movs r0, #224
	ldr r3, [r3]
	lsls r0, r0, #1
	movs r2, #129
	adds r3, r3, r0
	lsls r2, r2, #2
	str r2, [r3]
	ldr r2, [pc, #44]
	adds r0, #128
	ldr r1, [pc, #44]
	adds r3, r2, r0
	strh r1, [r3]
	ldr r3, [pc, #40]
	adds r2, r2, r3
	movs r3, #4
	strh r3, [r2]
	ldr r0, [pc, #36]
	ldr r6, [pc, #16]
	bl 0x02009980
	cmp r0, #0
	bne .L_02001070_0
	movs r0, #3
	bl 0x02009768
	b .L_02001070_0
	.4byte 0x00000000
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x00000028
	.4byte 0x00000242
	.4byte 0x00000845
.L_02001070_0:
	movs r0, #8
	bl 0x020099b0
	movs r5, #6
	strh r5, [r0, #32]
	movs r0, #9
	bl 0x020099b0
	strh r5, [r0, #32]
	movs r0, #12
	bl 0x020099b0
	strh r5, [r0, #32]
	movs r0, #13
	bl 0x020099b0
	strh r5, [r0, #32]
	movs r0, #14
	bl 0x020099b0
	movs r1, #0
	bl 0x02009958
	movs r0, #10
	bl 0x020099b0
	movs r1, #0
	bl 0x02009958
	movs r0, #11
	bl 0x020099b0
	movs r1, #0
	bl 0x02009958
	movs r0, #8
	bl 0x020099b0
	movs r1, #0
	bl 0x02009958
	movs r0, #9
	bl 0x020099b0
	movs r1, #0
	bl 0x02009958
	movs r0, #8
	movs r1, #2
	bl 0x02009a48
	movs r0, #14
	movs r1, #2
	bl 0x02009a48
	movs r0, #9
	movs r1, #2
	bl 0x02009a48
	mov r3, r8
	adds r3, #85
	strb r6, [r3]
	adds r2, r7, #0
	movs r3, #224
	lsls r3, r3, #13
	mov r0, r8
	adds r2, #85
	str r3, [r0, #12]
	strb r6, [r2]
	mov r2, r10
	adds r2, #85
	str r3, [r7, #12]
	strb r6, [r2]
	mov r2, r10
	str r3, [r2, #12]
	movs r0, #9
	movs r1, #3
	bl 0x020099f0
	movs r1, #3
	movs r0, #8
	bl 0x020099f0
	movs r0, #8
	bl 0x020099b0
	adds r0, #89
	ldrb r3, [r0]
	movs r5, #8
	orrs r3, r5
	strb r3, [r0]
	movs r0, #9
	bl 0x020099b0
	adds r0, #89
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #10
	bl 0x020099b0
	adds r0, #89
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #11
	bl 0x020099b0
	adds r0, #89
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #14
	bl 0x020099b0
	adds r0, #89
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r0, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.global Func_020011e8
	.thumb_func
Func_020011e8:
	push {lr}
	ldr r3, [r0, #24]
	ldr r2, [pc, #36]
	adds r3, r3, r2
	str r3, [r0, #24]
	movs r2, #128
	ldr r3, [r0, #56]
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_020011e8_0
	ldr r2, [r0, #60]
	cmp r2, r3
	bne .L_020011e8_0
	ldr r3, [r0, #64]
	cmp r3, r2
	bne .L_020011e8_0
	bl 0x02009948
.L_020011e8_0:
	movs r0, #1
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x00001eb8
	.global Func_02001218
	.thumb_func
Func_02001218:
	push {r5, r6, lr}
	ldr r3, [pc, #136]
	ldr r6, [r3]
	movs r3, #3
	ands r6, r3
	cmp r6, #0
	bne .L_02001218_0
	ldr r3, [pc, #128]
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02001218_1
	movs r0, #200
	bl 0x02009ab0
.L_02001218_1:
	movs r1, #163
	movs r2, #128
	movs r3, #192
	movs r0, #26
	lsls r1, r1, #17
	lsls r2, r2, #14
	lsls r3, r3, #16
	bl 0x02009940
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02001218_0
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
	ldr r3, [pc, #60]
	str r3, [r5, #24]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r5, #48]
	str r3, [r5, #52]
	adds r3, r5, #0
	adds r3, #85
	strb r6, [r3]
	adds r0, r5, #0
	movs r1, #2
	bl 0x02009930
	movs r1, #163
	movs r3, #240
	adds r0, r5, #0
	lsls r1, r1, #17
	movs r2, #0
	lsls r3, r3, #16
	bl 0x02009950
	ldr r1, [pc, #24]
	adds r0, r5, #0
	bl 0x02009938
.L_02001218_0:
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x03001e40
	.4byte 0x02009dd0
	.4byte 0x00001999
	.4byte 0x02009d9c
	.global Func_020012b4
	.thumb_func
Func_020012b4:
	push {r5, lr}
	ldr r5, [pc, #56]
	ldr r3, [r5]
	cmp r3, #0
	bne .L_020012b4_0
	movs r0, #0
	bl 0x020098b8
	movs r0, #20
	bl 0x02009a88
	b .L_020012b4_1
.L_020012b4_0:
	cmp r3, #20
	bne .L_020012b4_1
	movs r0, #1
	bl 0x020098b8
	movs r0, #8
	bl 0x02009a88
.L_020012b4_1:
	ldr r3, [r5]
	adds r3, #1
	str r3, [r5]
	cmp r3, #30
	bne .L_020012b4_2
	movs r3, #0
	str r3, [r5]
.L_020012b4_2:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x02009dd4
	.global Func_020012f4
	.thumb_func
Func_020012f4:
	push {lr}
	cmp r0, #10
	beq .L_020012f4_0
	b .L_020012f4_1
.L_020012f4_0:
	cmp r1, #11
	bls .L_020012f4_2
	b .L_020012f4_3
.L_020012f4_2:
	ldr r2, [pc, #544]
	lsls r3, r1, #2
	ldr r3, [r3, r2]
	mov pc, r3
	movs r0, r0
	str r3, [sp, #240]
	lsls r0, r0, #8
	str r3, [sp, #312]
	lsls r0, r0, #8
	str r3, [sp, #328]
	lsls r0, r0, #8
	str r3, [sp, #400]
	lsls r0, r0, #8
	str r3, [sp, #472]
	lsls r0, r0, #8
	str r3, [sp, #712]
	lsls r0, r0, #8
	str r3, [sp, #808]
	lsls r0, r0, #8
	str r4, [sp, #296]
	lsls r0, r0, #8
	str r3, [sp, #904]
	lsls r0, r0, #8
	str r3, [sp, #1000]
	lsls r0, r0, #8
	str r4, [sp, #72]
	lsls r0, r0, #8
	str r4, [sp, #296]
	lsls r0, r0, #8
	movs r1, #1
	movs r0, #8
	bl 0x020099f0
	movs r0, #6
	bl 0x02009908
	movs r0, #8
	b .L_020012f4_4
	.2byte 0x2008
	.2byte 0xe0d3
	.2byte 0x2101
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xfb4b
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfad4
	.2byte 0x2008
	.2byte 0xe09d
	.2byte 0x2101
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xfb42
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfacb
	.2byte 0x2008
	.2byte 0xe0a0
	.2byte 0x2101
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xfb39
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfac2
	.2byte 0x2103
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xfb32
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfabb
	.2byte 0x2101
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xfb2b
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfab4
	.2byte 0x2103
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xfb24
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfaad
	.2byte 0x2008
	.2byte 0xe0a3
	.2byte 0x2101
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xfb1b
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfaa4
	.2byte 0x2008
	.2byte 0x2102
	.2byte 0xf000
	.2byte 0xfb14
	.2byte 0xe0a6
	.2byte 0x2106
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xfb0f
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfa98
	.2byte 0x2008
	.2byte 0x2108
	.2byte 0xf000
	.2byte 0xfb08
	.2byte 0xe09a
	.2byte 0x2106
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xfb03
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfa8c
	.2byte 0x2008
	.2byte 0x2109
	.2byte 0xf000
	.2byte 0xfafc
	.2byte 0xe08e
	.2byte 0x2106
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xfaf7
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfa80
	.2byte 0x2008
	.2byte 0x210a
	.2byte 0xf000
	.2byte 0xfaf0
	.2byte 0xe082
	.2byte 0x2106
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xfaeb
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfa74
	.2byte 0x2108
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xfae4
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfa6d
	.2byte 0x2106
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xfadd
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfa66
	.2byte 0x2108
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xfad6
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfa5f
	.2byte 0x2008
	.2byte 0x2106
	.2byte 0xf000
	.2byte 0xfacf
	.2byte 0xe061
.L_020012f4_1:
	cmp r1, #5
	bhi .L_020012f4_3
	ldr r2, [pc, #204]
	lsls r3, r1, #2
	ldr r3, [r3, r2]
	mov pc, r3
	str r4, [sp, #480]
	lsls r0, r0, #8
	str r4, [sp, #992]
	lsls r0, r0, #8
	str r4, [sp, #576]
	lsls r0, r0, #8
	str r4, [sp, #672]
	lsls r0, r0, #8
	str r4, [sp, #768]
	lsls r0, r0, #8
	str r5, [sp, #8]
	lsls r0, r0, #8
	movs r1, #1
	movs r0, #9
	bl 0x020099f0
	movs r0, #6
	bl 0x02009908
	movs r0, #9
.L_020012f4_4:
	movs r1, #3
	bl 0x020099f0
	b .L_020012f4_3
	.2byte 0x2101
	.2byte 0x2009
	.2byte 0xf000
	.2byte 0xfaac
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfa35
	.2byte 0x2009
	.2byte 0x2105
	.2byte 0xf000
	.2byte 0xfaa5
	.2byte 0xe037
	.2byte 0x2101
	.2byte 0x2009
	.2byte 0xf000
	.2byte 0xfaa0
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfa29
	.2byte 0x2009
	.2byte 0x2104
	.2byte 0xf000
	.2byte 0xfa99
	.2byte 0xe02b
	.2byte 0x2101
	.2byte 0x2009
	.2byte 0xf000
	.2byte 0xfa94
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfa1d
	.2byte 0x2103
	.2byte 0x2009
	.2byte 0xf000
	.2byte 0xfa8d
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfa16
	.2byte 0x2101
	.2byte 0x2009
	.2byte 0xf000
	.2byte 0xfa86
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfa0f
	.2byte 0x2103
	.2byte 0x2009
	.2byte 0xf000
	.2byte 0xfa7f
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfa08
	.2byte 0x2009
	.2byte 0x2101
	.2byte 0xf000
	.2byte 0xfa78
	.2byte 0xe00a
	.2byte 0x2101
	.2byte 0x2009
	.2byte 0xf000
	.2byte 0xfa73
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xf9fc
	.2byte 0x2009
	.2byte 0x2102
	.2byte 0xf000
	.2byte 0xfa6c
.L_020012f4_3:
	movs r0, #12
	bl 0x02009908
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200930c
	.4byte 0x02009460
	.global Func_0200152c
	.thumb_func
Func_0200152c:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	adds r7, r6, #0
	adds r7, #100
	movs r1, #0
	ldrsh r2, [r7, r1]
	sub sp, #12
	cmp r2, #119
	bgt .L_0200152c_0
	ldr r3, [r6, #56]
	mov r5, sp
	str r3, [r5]
	ldr r3, [r6, #60]
	str r3, [r5, #4]
	ldr r3, [r6, #64]
	str r3, [r5, #8]
	adds r3, r6, #0
	adds r3, #102
	movs r1, #0
	ldrsh r3, [r3, r1]
	lsls r1, r2, #1
	adds r1, r1, r2
	lsls r1, r1, #8
	lsls r0, r2, #16
	adds r1, r1, r3
	adds r2, r5, #0
	bl 0x02009920
	ldr r3, [r5]
	str r3, [r6, #8]
	ldr r3, [r5, #4]
	str r3, [r6, #12]
	ldr r3, [r5, #8]
	ldr r2, [pc, #44]
	str r3, [r6, #16]
	ldr r3, [r6, #24]
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	adds r3, r3, r2
	str r3, [r6, #28]
	ldrh r3, [r7]
	adds r3, #1
	strh r3, [r7]
	b .L_0200152c_1
.L_0200152c_0:
	ldr r3, [r6, #80]
	ldrb r0, [r3, #28]
	bl 0x02009928
	adds r0, r6, #0
	bl 0x02009948
.L_0200152c_1:
	sub sp, #-12
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x00000147
	.global Func_020015a0
	.thumb_func
Func_020015a0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r1, [pc, #360]
	ldr r3, [r1]
	movs r0, #0
	mov r9, r0
	cmp r3, #40
	bls .L_020015a0_0
	b .L_020015a0_1
.L_020015a0_0:
	ldr r2, [pc, #348]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	str r6, [sp, #400]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #400]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #400]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #400]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #992]
	lsls r0, r0, #8
	str r6, [sp, #400]
	lsls r0, r0, #8
	movs r0, #220
	bl 0x02009ab0
	movs r2, #0
	ldr r6, [pc, #172]
	mov r8, r2
	mov r10, r2
	movs r7, #0
.L_020015a0_3:
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	ldr r0, [pc, #164]
	bl 0x02009940
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020015a0_2
	mov r1, r9
	ldr r0, [r5, #80]
	bl 0x02009a98
	adds r3, r5, #0
	adds r3, #85
	mov r9, r0
	mov r0, r10
	strb r0, [r3]
	ldr r1, [r5, #80]
	movs r0, #13
	ldrb r2, [r1, #9]
	negs r0, r0
	adds r3, r0, #0
	ands r2, r3
	movs r3, #4
	orrs r2, r3
	strb r2, [r1, #9]
	adds r0, r5, #0
	movs r1, #0
	bl 0x02009958
	adds r0, r5, #0
	movs r1, #1
	bl 0x02009930
	adds r3, r5, #0
	adds r3, #100
	mov r2, r10
	movs r1, #180
	strh r2, [r3]
	lsls r1, r1, #1
	adds r0, r7, #0
	bl 0x02009900
	adds r3, r5, #0
	adds r3, #102
	strh r0, [r3]
	ldr r3, [r6]
	str r3, [r5, #56]
	ldr r3, [r6, #4]
	str r3, [r5, #60]
	ldr r3, [r6, #8]
	str r3, [r5, #64]
	ldr r3, [pc, #68]
	str r3, [r5, #48]
	ldr r3, [pc, #68]
	str r3, [r5, #108]
.L_020015a0_2:
	movs r0, #1
	movs r3, #240
	add r8, r0
	lsls r3, r3, #14
	mov r2, r8
	adds r7, r7, r3
	cmp r2, #5
	bls .L_020015a0_3
	ldr r1, [pc, #28]
.L_020015a0_1:
	ldr r3, [r1]
	adds r3, #1
	str r3, [r1]
	cmp r3, #120
	ble .L_020015a0_4
	movs r3, #0
	str r3, [r1]
.L_020015a0_4:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x02009dcc
	.4byte 0x020095c0
	.4byte 0x02009dc0
	.4byte 0x0000011d
	.4byte 0x00019999
	.4byte 0x0200952d
	.global Func_0200172c
	.thumb_func
Func_0200172c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	mov r8, r1
	bl 0x02009978
	movs r7, #1
	adds r5, r0, #0
	negs r7, r7
	cmp r5, r7
	beq .L_0200172c_0
	adds r1, r6, #0
	bl 0x02009970
	adds r6, r0, #0
	cmp r6, r7
	beq .L_0200172c_0
	adds r0, r5, #0
	bl 0x02009968
	lsls r3, r6, #1
	adds r3, #216
	mov r2, r8
	strh r2, [r0, r3]
.L_0200172c_0:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_02001768
	.thumb_func
Func_02001768:
	push {r5, r6, r7, lr}
	adds r7, r0, #0
	bl 0x02009838
	movs r6, #0
.L_02001768_1:
	ldr r2, [pc, #84]
	adds r3, r6, r2
	movs r2, #192
	lsls r2, r2, #11
	lsrs r5, r6, #16
	cmp r3, r2
	bls .L_02001768_0
	ldr r2, [pc, #72]
	adds r3, r5, r2
	movs r2, #224
	lsls r3, r3, #16
	lsls r2, r2, #11
	cmp r3, r2
	bls .L_02001768_0
	movs r3, #160
	lsls r3, r3, #19
	lsls r5, r5, #1
	adds r5, r5, r3
	ldrh r0, [r5]
	adds r1, r7, #0
	bl 0x020097d0
	strh r0, [r5]
.L_02001768_0:
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r6, r2
	movs r2, #223
	lsls r2, r2, #16
	adds r6, r3, #0
	cmp r3, r2
	bls .L_02001768_1
	bl 0x02009878
	bl 0x02009858
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl 0x02009a78
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0xffef0000
	.4byte 0x0000ff3f
	.global Func_020017d0
	.thumb_func
Func_020017d0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #248
	lsls r0, r0, #16
	lsls r3, r3, #13
	ands r3, r0
	asrs r6, r3, #16
	ldr r2, [pc, #52]
	mov r8, r1
	lsrs r5, r0, #21
	lsrs r7, r0, #26
	lsls r1, r1, #2
	adds r0, r6, #0
	ands r5, r2
	ands r7, r2
	bl 0x020098f8
	adds r0, r6, r0
	lsls r0, r0, #16
	mov r1, r8
	asrs r6, r0, #16
	adds r0, r5, #0
	bl 0x020098f8
	subs r0, r5, r0
	lsls r0, r0, #16
	asrs r5, r0, #16
	mov r1, r8
	adds r0, r7, #0
	bl 0x020098f8
	subs r0, r7, r0
	lsls r0, r0, #16
	asrs r7, r0, #16
	b .L_020017d0_0
	.4byte 0x0000001f
.L_020017d0_0:
	cmp r6, #31
	ble .L_020017d0_1
	movs r6, #31
.L_020017d0_1:
	lsls r3, r7, #10
	lsls r2, r5, #5
	orrs r3, r2
	orrs r6, r3
	lsls r0, r6, #16
	lsrs r0, r0, #16
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.global Func_02001838
	.thumb_func
Func_02001838:
	ldr r2, [pc, #12]
	ldr r3, [pc, #16]
	ldr r0, [r2]
	ldr r1, [pc, #16]
	ldr r2, [pc, #16]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bx lr
	.4byte 0x03001ed0
	.4byte 0x040000d4
	.4byte 0x02009de0
	.4byte 0x840000e0
	.global Func_02001858
	.thumb_func
Func_02001858:
	ldr r2, [pc, #12]
	ldr r3, [pc, #16]
	ldr r0, [r2]
	ldr r1, [pc, #16]
	ldr r2, [pc, #16]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bx lr
	.4byte 0x03001ed0
	.4byte 0x040000d4
	.4byte 0x0200a4e0
	.4byte 0x840000e0
	.global Func_02001878
	.thumb_func
Func_02001878:
	push {lr}
	ldr r3, [pc, #44]
	ldr r4, [r3]
	movs r0, #160
	ldr r3, [pc, #40]
	lsls r0, r0, #19
	adds r1, r4, #0
	ldr r2, [pc, #40]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #224
	lsls r2, r2, #1
	adds r1, r4, r2
	ldr r0, [pc, #32]
	ldr r2, [pc, #24]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl 0x02009a80
	pop {r0}
	bx r0
	.4byte 0x03001ed0
	.4byte 0x040000d4
	.4byte 0x84000070
	.4byte 0x05000200
	.global Func_020018b8
	.thumb_func
Func_020018b8:
	push {lr}
	ldr r3, [pc, #40]
	ldr r1, [r3]
	cmp r0, #0
	beq .L_020018b8_0
	ldr r3, [pc, #36]
	ldr r0, [pc, #36]
	b .L_020018b8_1
.L_020018b8_0:
	ldr r3, [pc, #28]
	ldr r0, [pc, #36]
.L_020018b8_1:
	ldr r2, [pc, #36]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl 0x02009a78
	bl 0x02009878
	pop {r0}
	bx r0
	.4byte 0x03001ed0
	.4byte 0x040000d4
	.4byte 0x0200a4e0
	.4byte 0x02009de0
	.4byte 0x840000e0
	.include "games/THE BROKEN SEAL/SRC/FIELD/KORIMA_KI/IMPORT.INC"
	.section .rodata,"a",%progbits
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00e60000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x015a0000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x014d0000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00d40000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0xffff0000
	.4byte 0x000000d1
	.4byte 0x40000117
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x000001c0
	.4byte 0xffff0001
	.4byte 0x00000088
	.4byte 0xc00001a8
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x000001c0
	.4byte 0xffff0002
	.4byte 0x00100148
	.4byte 0x400000c8
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x000001c0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00580140
	.4byte 0x015000b0
	.4byte 0x00c00068
	.4byte 0x0002ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000002c
	.4byte 0x0010202b
	.4byte 0x0020102d
	.4byte 0x000001ff
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x01470000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00024000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x010e0000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x01470000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x010e0000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x01470000
	.4byte 0x00000000
	.4byte 0x00c20000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00024000
	.4byte 0xffff0037
	.4byte 0x00000001
	.4byte 0x01470000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00024000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000b814
	.4byte 0x0845000c
	.4byte 0x020082c1
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x02008089
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x02008159
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x02008249
	.4byte 0x00008d15
	.4byte 0x0844040c
	.4byte 0x02008159
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x02008285
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001999
	.4byte 0x00000022
	.4byte 0x020091e9
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
