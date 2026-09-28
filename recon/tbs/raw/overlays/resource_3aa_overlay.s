.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/KAREI_KYUDEN/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	push {lr}
	ldr r3, [pc, #28]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_02000030_0
	ldr r0, [pc, #16]
	b .L_02000030_1
.L_02000030_0:
	ldr r0, [pc, #16]
.L_02000030_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000067
	.4byte 0x02009c04
	.4byte 0x02009bd4
	.global Func_02000060
	.thumb_func
Func_02000060:
	movs r0, #0
	bx lr
	.global Func_02000064
	.thumb_func
Func_02000064:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02009d9c
	.global Func_0200006c
	.thumb_func
Func_0200006c:
	push {r5, lr}
	ldr r3, [pc, #36]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #28]
	cmp r2, r3
	bne .L_0200006c_0
	ldr r5, [pc, #24]
	adds r0, r5, #0
	bl 0x02009a94
	adds r0, r5, #0
	b .L_0200006c_1
.L_0200006c_0:
	ldr r0, [pc, #16]
.L_0200006c_1:
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000067
	.4byte 0x02009df4
	.4byte 0x02009ddc
	.global Func_020000a4
	.thumb_func
Func_020000a4:
	push {lr}
	ldr r3, [pc, #28]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_020000a4_0
	ldr r0, [pc, #16]
	b .L_020000a4_1
.L_020000a4_0:
	ldr r0, [pc, #16]
.L_020000a4_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000067
	.4byte 0x02009f38
	.4byte 0x02009f2c
	.global Func_020000d4
	.thumb_func
Func_020000d4:
	push {lr}
	bl 0x02009a84
	ldr r0, [pc, #20]
	bl 0x02009b0c
	movs r1, #0
	movs r0, #13
	bl 0x02009b2c
	bl 0x02009a8c
	pop {r0}
	bx r0
	.4byte 0x00001b83
	.global Func_020000f4
	.thumb_func
Func_020000f4:
	push {lr}
	bl 0x02009a84
	ldr r0, [pc, #20]
	bl 0x02009b0c
	movs r1, #0
	movs r0, #16
	bl 0x02009b2c
	bl 0x02009a8c
	pop {r0}
	bx r0
	.4byte 0x00001b88
	.global Func_02000114
	.thumb_func
Func_02000114:
	push {lr}
	bl 0x02009a84
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #60
	movs r0, #8
	bl 0x02009b3c
	ldr r0, [pc, #84]
	bl 0x02009b0c
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r0, #8
	movs r1, #2
	bl 0x02009b04
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r0, #8
	movs r1, #4
	bl 0x02009af4
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r0, #8
	movs r1, #3
	bl 0x02009af4
	movs r1, #0
	movs r2, #10
	movs r0, #8
	bl 0x02009b24
	ldr r0, [pc, #16]
	bl 0x02009a6c
	bl 0x02009a8c
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001b91
	.4byte 0x00000913
	.global Func_02000184
	.thumb_func
Func_02000184:
	push {r5, lr}
	ldr r3, [pc, #164]
	ldr r5, [r3]
	bl 0x02009a84
	movs r0, #10
	bl 0x02009a7c
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #4
	bne .L_02000184_0
	movs r0, #188
	bl 0x02009b8c
	b .L_02000184_1
.L_02000184_0:
	movs r0, #158
	bl 0x02009b8c
.L_02000184_1:
	movs r0, #1
	bl 0x02009a4c
	movs r0, #2
	bl 0x02009a4c
	movs r0, #10
	bl 0x02009a7c
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #7
	movs r0, #0
	lsls r1, r1, #8
	bl 0x02009aac
	movs r0, #0
	movs r1, #2
	bl 0x02009aec
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #4
	bne .L_02000184_2
	movs r2, #16
	movs r0, #0
	movs r1, #0
	negs r2, r2
	bl 0x02009adc
	b .L_02000184_3
.L_02000184_2:
	movs r2, #16
	movs r0, #0
	movs r1, #3
	negs r2, r2
	bl 0x02009ad4
.L_02000184_3:
	movs r0, #16
	bl 0x02009a7c
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl 0x02009b5c
	movs r0, #1
	bl 0x02009a54
	movs r0, #2
	bl 0x02009a54
	bl 0x02009a8c
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.global Func_02000230
	.thumb_func
Func_02000230:
	push {lr}
	ldr r3, [pc, #32]
	movs r2, #224
	ldr r1, [r3]
	ldr r3, [pc, #28]
	lsls r2, r2, #1
	str r3, [r1, r2]
	ldr r3, [pc, #28]
	ldrsh r2, [r3, r2]
	ldr r3, [pc, #28]
	cmp r2, r3
	bne .L_02000230_0
	bl 0x02008264
.L_02000230_0:
	movs r0, #0
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00000209
	.4byte 0x02000240
	.4byte 0x00000067
	.global Func_02000264
	.thumb_func
Func_02000264:
	push {lr}
	movs r0, #1
	sub sp, #8
	bl 0x02009a54
	movs r0, #2
	bl 0x02009a54
	ldr r3, [pc, #204]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #11
	bgt .L_02000264_0
	cmp r3, #10
	bge .L_02000264_1
	cmp r3, #9
	beq .L_02000264_2
	b .L_02000264_3
.L_02000264_0:
	cmp r3, #20
	beq .L_02000264_4
	b .L_02000264_3
.L_02000264_2:
	ldr r0, [pc, #176]
	bl 0x02009a64
	cmp r0, #0
	beq .L_02000264_5
	movs r0, #8
	bl 0x02009aa4
	movs r3, #128
	lsls r3, r3, #5
	strh r3, [r0, #6]
	ldr r0, [pc, #160]
	bl 0x02009a64
	cmp r0, #0
	bne .L_02000264_3
	bl 0x02009494
	b .L_02000264_3
.L_02000264_5:
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x02009ae4
	ldr r0, [pc, #136]
	bl 0x02009a64
	cmp r0, #0
	beq .L_02000264_3
	movs r2, #211
	movs r0, #8
	ldr r1, [pc, #128]
	lsls r2, r2, #17
	bl 0x02009ae4
	movs r0, #8
	bl 0x02009aa4
	movs r3, #208
	lsls r3, r3, #8
	strh r3, [r0, #6]
	b .L_02000264_3
.L_02000264_1:
	ldr r0, [pc, #108]
	bl 0x02009a64
	cmp r0, #0
	beq .L_02000264_3
	movs r3, #4
	movs r2, #3
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #58
	movs r1, #70
	movs r2, #54
	movs r3, #70
	bl 0x02009a44
	movs r3, #55
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #55
	movs r1, #9
	movs r2, #2
	movs r3, #1
	bl 0x02009a5c
	bl 0x02009a3c
	movs r0, #1
	bl 0x02009a34
	b .L_02000264_3
.L_02000264_4:
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x02009ae4
	ldr r0, [pc, #40]
	bl 0x02009a64
	cmp r0, #0
	bne .L_02000264_3
	bl 0x02008360
.L_02000264_3:
	sub sp, #-8
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x00000941
	.4byte 0x00000914
	.4byte 0x00000321
	.4byte 0x038a0000
	.4byte 0x00000915
	.4byte 0x00000109
	.global Func_02000360
	.thumb_func
Func_02000360:
	push {r5, r6, lr}
	bl 0x02009a84
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	movs r3, #0
	negs r1, r1
	negs r0, r0
	bl 0x02009b54
	movs r0, #1
	bl 0x02009a34
	ldr r0, [pc, #1020]
	movs r1, #0
	bl 0x02009b6c
	movs r1, #0
	ldr r0, [pc, #1008]
	bl 0x02009b64
	movs r0, #1
	bl 0x02009b74
	movs r0, #1
	bl 0x02009a34
	ldr r6, [pc, #996]
	movs r3, #228
	ldr r1, [r6]
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #24
	str r3, [r2]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #65
	str r3, [r2]
	movs r1, #214
	movs r2, #220
	movs r0, #8
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl 0x02009ae4
	movs r1, #214
	movs r2, #243
	movs r0, #0
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl 0x02009ae4
	movs r1, #212
	movs r2, #251
	movs r0, #1
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl 0x02009ae4
	movs r1, #218
	movs r2, #243
	movs r0, #2
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl 0x02009ae4
	movs r1, #220
	movs r2, #251
	movs r0, #3
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl 0x02009ae4
	movs r1, #210
	movs r0, #10
	lsls r1, r1, #18
	ldr r2, [pc, #900]
	bl 0x02009ae4
	movs r1, #222
	movs r0, #11
	lsls r1, r1, #18
	ldr r2, [pc, #888]
	bl 0x02009ae4
	movs r0, #216
	movs r1, #1
	movs r2, #236
	movs r3, #0
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #18
	bl 0x02009b54
	bl 0x02009a3c
	movs r0, #1
	bl 0x02009a34
	bl 0x02009b7c
	bl 0x02009b84
	movs r0, #40
	bl 0x02009a7c
	movs r0, #8
	movs r1, #1
	bl 0x02009b04
	movs r1, #3
	movs r0, #8
	bl 0x02009af4
	movs r0, #10
	bl 0x02009a7c
	movs r0, #2
	movs r1, #1
	bl 0x02009b04
	movs r1, #4
	movs r0, #2
	bl 0x02009af4
	movs r0, #20
	bl 0x02009a7c
	movs r1, #128
	movs r2, #20
	movs r0, #2
	lsls r1, r1, #8
	bl 0x02009b34
	movs r0, #0
	movs r1, #2
	bl 0x02009b04
	movs r2, #10
	movs r0, #0
	movs r1, #0
	bl 0x02009b34
	movs r0, #0
	movs r1, #3
	bl 0x02009af4
	movs r0, #1
	movs r1, #1
	bl 0x02009afc
	movs r0, #3
	movs r1, #1
	bl 0x02009b04
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b34
	movs r1, #160
	movs r2, #10
	movs r0, #3
.L_020004ae:
	lsls r1, r1, #8
	bl 0x02009b34
	movs r0, #1
	movs r1, #3
	bl 0x02009aec
	movs r1, #3
	movs r0, #3
	bl 0x02009af4
	movs r0, #20
	bl 0x02009a7c
	movs r0, #20
	bl 0x02009450
	movs r1, #129
	movs r0, #8
	lsls r1, r1, #1
	bl 0x02009b44
	movs r1, #2
	movs r0, #8
	bl 0x02009afc
	movs r0, #60
	bl 0x02009a7c
	movs r1, #160
	movs r2, #10
	movs r0, #8
	lsls r1, r1, #7
	bl 0x02009b34
	movs r0, #8
	movs r1, #3
	bl 0x02009af4
	movs r1, #128
	movs r2, #0
	movs r0, #10
	lsls r1, r1, #1
	bl 0x02009b3c
	movs r1, #2
	movs r0, #10
	bl 0x02009afc
	movs r0, #60
	bl 0x02009a7c
	movs r1, #240
	movs r2, #20
	movs r0, #10
	lsls r1, r1, #8
	bl 0x02009b34
	movs r0, #11
	movs r1, #2
	bl 0x02009b04
	movs r1, #144
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #40
	bl 0x02009b34
	movs r1, #208
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b34
	movs r1, #176
	movs r2, #20
	movs r0, #11
	lsls r1, r1, #8
	bl 0x02009b34
	movs r1, #3
	movs r0, #11
	bl 0x02009af4
	movs r0, #10
	bl 0x02009a7c
	movs r0, #8
	movs r1, #2
	bl 0x02009b04
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #10
	bl 0x02009b34
	movs r2, #60
	movs r0, #8
	ldr r1, [pc, #528]
	bl 0x02009b3c
	movs r0, #11
	movs r1, #2
	bl 0x02009b04
	movs r0, #11
	movs r1, #4
	bl 0x02009af4
	movs r0, #11
	movs r1, #4
	bl 0x02009af4
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl 0x02009b64
	movs r0, #40
	bl 0x02009b74
	movs r0, #60
	bl 0x02009a34
	ldr r1, [pc, #476]
	movs r2, #60
	movs r0, #8
	bl 0x02009b3c
	ldr r0, [pc, #472]
	bl 0x02009b0c
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r0, #10
	movs r1, #2
	bl 0x02009b04
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl 0x02009b24
	movs r0, #11
	movs r1, #4
	bl 0x02009af4
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #10
	bl 0x02009b34
	movs r2, #10
	ldr r0, [pc, #408]
	movs r1, #0
	bl 0x02009b24
	movs r0, #11
	movs r1, #2
	bl 0x02009b04
	movs r2, #10
	movs r0, #11
	movs r1, #0
	bl 0x02009b24
	movs r0, #10
	movs r1, #4
	bl 0x02009aec
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #10
	bl 0x02009b34
	ldr r0, [pc, #356]
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
	movs r0, #8
	ldr r1, [pc, #348]
	movs r2, #60
	bl 0x02009b3c
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r0, #11
	movs r1, #2
	bl 0x02009b04
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
	movs r1, #128
	movs r2, #10
	movs r0, #2
	lsls r1, r1, #6
	bl 0x02009b34
	movs r0, #10
	movs r1, #3
	bl 0x02009af4
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
	movs r1, #192
	movs r2, #10
	movs r0, #2
	lsls r1, r1, #7
	bl 0x02009b34
	movs r0, #2
	movs r1, #1
	bl 0x02009afc
	movs r2, #10
	ldr r0, [pc, #252]
	movs r1, #0
	bl 0x02009b24
	movs r0, #10
	movs r1, #3
.L_0200069c:
	bl 0x02009af4
	movs r1, #128
	movs r2, #10
	movs r0, #2
	lsls r1, r1, #8
	bl 0x02009b34
	movs r1, #0
	ldr r0, [pc, #224]
	bl 0x02009b2c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #8
	bl 0x02009b44
	movs r0, #60
	bl 0x02009a7c
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r1, #2
	movs r0, #2
	bl 0x02009b04
	movs r0, #10
	bl 0x02009a7c
	movs r1, #192
	movs r2, #20
	movs r0, #2
	lsls r1, r1, #8
	bl 0x02009b34
	movs r1, #3
	movs r0, #2
	bl 0x02009af4
	movs r0, #20
	bl 0x02009a7c
	movs r0, #1
	movs r1, #3
	bl 0x02009af4
	movs r0, #1
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
	movs r2, #60
	movs r0, #8
	ldr r1, [pc, #120]
	bl 0x02009b3c
	movs r0, #3
	movs r1, #4
	bl 0x02009af4
	movs r0, #3
	movs r1, #0
	movs r2, #40
	bl 0x02009b24
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #5
	movs r2, #40
	bl 0x02009b34
	movs r0, #8
	ldr r1, [pc, #104]
	ldr r2, [pc, #104]
	bl 0x02009aac
	movs r1, #223
	movs r2, #220
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #8
	bl 0x02009acc
	movs r0, #40
	bl 0x02009a7c
	movs r1, #208
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #20
	bl 0x02009b34
	ldr r1, [pc, #44]
	movs r2, #60
	movs r0, #8
	bl 0x02009b3c
	movs r0, #60
	bl 0x0200940c
	movs r0, #40
	bl 0x02009450
	movs r1, #214
	movs r2, #220
	lsls r1, r1, #2
	lsls r2, r2, #1
	b .L_0200069c_0
	.2byte 0x0000
	.2byte 0x0002
	.2byte 0x0001
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x0000
	.2byte 0x0206
	.4byte 0x00000105
	.2byte 0x1b21
	.2byte 0x0000
	.4byte 0x00006002
	.2byte 0x2002
	.2byte 0x0000
	.2byte 0x0101
	.2byte 0x0000
	.4byte 0x00006666
	.4byte 0x00003333
.L_0200069c_0:
	movs r0, #8
	bl 0x02009acc
	movs r0, #40
	bl 0x02009a7c
	movs r1, #144
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #20
	bl 0x02009b34
	movs r0, #8
	ldr r1, [pc, #772]
	movs r2, #60
	bl 0x02009b3c
	movs r1, #240
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b34
	movs r1, #144
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #40
	bl 0x02009b34
	movs r1, #208
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b34
	movs r1, #176
	movs r2, #10
	movs r0, #11
	lsls r1, r1, #8
	bl 0x02009b34
	movs r0, #8
	movs r1, #1
	bl 0x02009b04
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r0, #8
	movs r1, #4
	bl 0x02009af4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r0, #10
	movs r1, #2
	bl 0x02009b04
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl 0x02009b24
	movs r0, #8
	movs r1, #2
	bl 0x02009b04
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #10
	bl 0x02009b34
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009b34
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009b34
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009b34
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009b34
	movs r1, #144
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #40
	bl 0x02009b34
	movs r1, #240
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #20
	bl 0x02009b34
	movs r2, #20
	movs r0, #10
	movs r1, #0
	bl 0x02009b24
	movs r1, #2
	movs r0, #11
	bl 0x02009b04
	movs r0, #20
	bl 0x02009a7c
	movs r1, #176
	movs r2, #20
	movs r0, #11
	lsls r1, r1, #8
	bl 0x02009b34
	movs r0, #11
	movs r1, #3
	bl 0x02009af4
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
	movs r1, #144
	movs r2, #40
	movs r0, #11
	lsls r1, r1, #8
	bl 0x02009b34
	movs r0, #10
	movs r1, #3
	bl 0x02009aec
	movs r0, #11
	movs r1, #3
	bl 0x02009af4
	movs r1, #208
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b34
	movs r1, #176
	movs r2, #10
	movs r0, #11
	lsls r1, r1, #8
	bl 0x02009b34
	movs r0, #10
	movs r1, #2
	bl 0x02009b04
	movs r2, #20
	movs r1, #0
	movs r0, #10
	bl 0x02009b24
	movs r0, #20
	bl 0x02009450
	movs r1, #2
	movs r0, #8
	bl 0x02009b04
	movs r0, #40
	bl 0x02009a7c
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #8
	bl 0x02009b44
	movs r0, #60
	bl 0x02009a7c
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r1, #4
	movs r0, #8
	bl 0x02009af4
	movs r0, #20
	bl 0x02009a7c
	movs r0, #8
	movs r1, #4
	bl 0x02009aec
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r0, #1
	movs r1, #2
	bl 0x02009b04
	movs r1, #224
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #8
	bl 0x02009b34
	movs r1, #0
	movs r0, #1
	bl 0x02009b14
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009b34
	movs r0, #0
	movs r1, #0
	movs r5, #1
	bl 0x02009a9c
	cmp r0, #1
	bne .L_0200069c_1
	ldr r2, [r6]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r5, #0
.L_0200069c_1:
	movs r0, #1
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
	cmp r5, #0
	beq .L_0200069c_2
	ldr r2, [r6]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_0200069c_2:
	movs r1, #128
	movs r2, #10
	movs r0, #2
	lsls r1, r1, #8
	bl 0x02009b34
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #2
	bl 0x02009b44
	movs r0, #60
	bl 0x02009a7c
	movs r0, #8
	ldr r1, [pc, #232]
	movs r2, #0
	bl 0x02009b3c
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #40
	bl 0x02009b34
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #20
	bl 0x02009b34
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #20
	bl 0x02009b34
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r0, #2
	movs r1, #1
	bl 0x02009b04
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #10
	bl 0x02009b34
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b34
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #10
	bl 0x02009b34
	ldr r0, [pc, #140]
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
	movs r0, #8
	ldr r1, [pc, #132]
	movs r2, #60
	bl 0x02009b3c
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r0, #8
	movs r1, #4
	bl 0x02009af4
	movs r1, #0
	movs r0, #8
	bl 0x02009b14
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b34
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b34
	movs r1, #160
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b34
	movs r0, #0
	movs r1, #0
	bl 0x02009a9c
	cmp r0, #0
	bne .L_0200069c_3
	movs r0, #20
	bl 0x02009a7c
	movs r0, #8
	movs r1, #3
	bl 0x02009af4
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	ldr r2, [r6]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200069c_4
	.4byte 0x00000105
	.4byte 0x00000101
	.4byte 0x00002002
	.4byte 0x00000107
.L_0200069c_3:
	movs r0, #20
	bl 0x02009a7c
	movs r0, #8
	movs r1, #4
	bl 0x02009af4
	ldr r2, [r6]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
.L_0200069c_4:
	movs r0, #20
	bl 0x02009450
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #6
	movs r2, #0
	bl 0x02009b34
	movs r1, #160
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #40
	bl 0x02009b34
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b34
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #10
	bl 0x02009b34
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x02009b24
	movs r0, #8
	ldr r1, [pc, #404]
	movs r2, #60
	bl 0x02009b3c
	movs r2, #40
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r0, #8
	movs r1, #2
	bl 0x02009b04
	movs r2, #40
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r1, #1
	movs r0, #2
	bl 0x02009b04
	movs r0, #20
	bl 0x02009a7c
	movs r0, #2
	ldr r1, [pc, #356]
	ldr r2, [pc, #360]
	bl 0x02009aac
	movs r1, #217
	movs r2, #236
	movs r0, #2
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x02009acc
	movs r2, #10
	ldr r0, [pc, #340]
	movs r1, #0
	bl 0x02009b24
	movs r0, #8
	movs r1, #4
	bl 0x02009af4
	movs r1, #0
	movs r2, #10
	movs r0, #8
	bl 0x02009b24
	movs r0, #40
	bl 0x0200940c
	movs r0, #20
	bl 0x02009450
	movs r0, #8
	ldr r1, [pc, #288]
	movs r2, #60
	bl 0x02009b3c
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r0, #1
	movs r1, #2
	bl 0x02009b04
	movs r2, #10
	movs r0, #1
	movs r1, #0
	bl 0x02009b24
	movs r0, #8
	movs r1, #1
	bl 0x02009b04
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #10
	bl 0x02009b34
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r0, #8
	movs r1, #3
	bl 0x02009af4
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #10
	bl 0x02009b34
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x02009b44
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	bl 0x02009b44
	movs r1, #129
	movs r0, #2
	lsls r1, r1, #1
	bl 0x02009b44
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #3
	bl 0x02009b44
	movs r0, #40
	bl 0x02009a7c
	movs r2, #10
	ldr r0, [pc, #156]
	movs r1, #0
	bl 0x02009b24
	movs r0, #8
	movs r1, #3
	bl 0x02009af4
	movs r1, #0
	movs r0, #8
	bl 0x02009b14
	movs r0, #1
	movs r1, #1
	bl 0x02009afc
	movs r0, #2
	movs r1, #1
	bl 0x02009afc
	movs r0, #3
	movs r1, #1
	bl 0x02009b04
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b34
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009b34
	movs r1, #160
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b34
	movs r0, #0
	movs r1, #0
	bl 0x02009a9c
	cmp r0, #0
	bne .L_0200069c_5
	movs r0, #20
	bl 0x02009a7c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #8
	bl 0x02009b44
	movs r0, #40
	bl 0x02009a7c
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	ldr r3, [pc, #32]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #2
	strh r3, [r2]
	b .L_0200069c_6
	.2byte 0x0000
	.4byte 0x00000105
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x00002002
	.4byte 0x03001ebc
.L_0200069c_5:
	movs r0, #20
	bl 0x02009a7c
	movs r0, #1
	movs r1, #2
	bl 0x02009b04
	ldr r3, [pc, #1016]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #1
	movs r2, #10
	movs r1, #0
	bl 0x02009b24
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #8
	bl 0x02009b44
	movs r0, #40
	bl 0x02009a7c
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
.L_0200069c_6:
	movs r0, #3
	ldr r1, [pc, #964]
	movs r2, #40
	bl 0x02009b3c
	movs r1, #192
	movs r2, #10
	movs r0, #3
	lsls r1, r1, #8
	bl 0x02009b34
	movs r0, #3
	movs r1, #0
	bl 0x02009b1c
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b34
	movs r1, #192
	movs r2, #20
	movs r0, #2
	lsls r1, r1, #8
	bl 0x02009b34
	movs r0, #8
	movs r1, #3
	bl 0x02009af4
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x02009b44
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	bl 0x02009b44
	movs r1, #129
	movs r0, #2
	lsls r1, r1, #1
	bl 0x02009b44
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #3
	bl 0x02009b44
	movs r0, #40
	bl 0x02009a7c
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b34
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b34
	movs r1, #160
	movs r2, #10
	movs r0, #3
	lsls r1, r1, #8
	bl 0x02009b34
	movs r0, #2
	movs r1, #4
	bl 0x02009af4
	movs r2, #10
	ldr r0, [pc, #796]
	movs r1, #0
	bl 0x02009b24
	movs r0, #8
	movs r1, #3
	bl 0x02009af4
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r0, #8
	movs r1, #1
	bl 0x02009b04
	movs r2, #10
	movs r1, #0
	movs r0, #8
	bl 0x02009b24
	movs r0, #10
	bl 0x02009450
	movs r0, #8
	movs r1, #4
	bl 0x02009af4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r0, #0
	movs r1, #4
	bl 0x02009aec
	movs r0, #1
	movs r1, #4
	bl 0x02009aec
	movs r0, #2
	movs r1, #4
	bl 0x02009aec
	movs r0, #3
	movs r1, #4
	bl 0x02009af4
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #8
	bl 0x02009b44
	movs r0, #60
	bl 0x02009a7c
	movs r2, #10
	movs r1, #0
	movs r0, #8
	bl 0x02009b24
	movs r0, #40
	bl 0x0200940c
	movs r0, #8
	movs r1, #4
	bl 0x02009af4
	movs r2, #10
	movs r1, #0
	movs r0, #8
	bl 0x02009b24
	movs r0, #20
	bl 0x02009450
	movs r0, #2
	movs r1, #2
	bl 0x02009b04
	movs r2, #10
	ldr r0, [pc, #632]
	movs r1, #0
	bl 0x02009b24
	movs r0, #8
	movs r1, #3
	bl 0x02009af4
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
	movs r0, #3
	ldr r1, [pc, #600]
	movs r2, #60
	bl 0x02009b3c
	movs r0, #3
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
	movs r1, #132
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #60
	bl 0x02009b3c
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r0, #3
	movs r1, #3
	bl 0x02009af4
	movs r2, #10
	movs r0, #3
	movs r1, #0
	bl 0x02009b24
	movs r0, #8
	movs r1, #3
	bl 0x02009af4
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
	movs r0, #1
	ldr r1, [pc, #520]
	movs r2, #40
	bl 0x02009b3c
	movs r0, #1
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
	movs r1, #160
	movs r2, #10
	movs r0, #8
	lsls r1, r1, #7
	bl 0x02009b34
	movs r0, #8
	movs r1, #3
	bl 0x02009af4
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r0, #10
	movs r1, #2
	bl 0x02009b04
	movs r0, #10
	movs r1, #4
	bl 0x02009af4
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
	movs r0, #8
	ldr r1, [pc, #452]
	movs r2, #40
	bl 0x02009b3c
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
	movs r1, #192
	movs r2, #10
	movs r0, #8
	lsls r1, r1, #6
	bl 0x02009b34
	movs r0, #8
	movs r1, #4
	bl 0x02009aec
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r0, #8
	movs r1, #1
	bl 0x02009b04
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #10
	bl 0x02009b34
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r0, #2
	movs r1, #1
	bl 0x02009b04
	ldr r0, [pc, #364]
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #40
	bl 0x02009b3c
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #10
	bl 0x02009b34
	movs r2, #10
	movs r1, #0
	movs r0, #8
	bl 0x02009b24
	movs r0, #40
	bl 0x0200940c
	movs r0, #20
	bl 0x02009450
	movs r0, #1
	movs r1, #1
	bl 0x02009b04
	movs r2, #10
	movs r0, #1
	movs r1, #0
	bl 0x02009b24
	movs r0, #8
	movs r1, #3
	bl 0x02009af4
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r0, #8
	movs r1, #4
	bl 0x02009aec
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r0, #0
	movs r1, #3
	bl 0x02009aec
	movs r0, #1
	movs r1, #3
	bl 0x02009aec
	movs r0, #2
	movs r1, #3
	bl 0x02009aec
	movs r0, #3
	movs r1, #3
	bl 0x02009af4
	movs r0, #8
	movs r1, #1
	bl 0x02009b04
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x02009b44
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #1
	bl 0x02009b44
	movs r0, #60
	bl 0x02009a7c
	movs r0, #8
	movs r1, #4
	bl 0x02009af4
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r0, #2
	movs r1, #1
	bl 0x02009afc
	movs r1, #1
	movs r0, #3
	bl 0x02009b04
	movs r0, #20
	bl 0x02009a7c
	movs r0, #8
	movs r1, #1
	bl 0x02009b04
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r0, #8
	movs r1, #3
	bl 0x02009aec
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
	movs r0, #2
	ldr r1, [pc, #104]
	movs r2, #60
	bl 0x02009b3c
	ldr r0, [pc, #92]
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
	movs r1, #160
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #10
	bl 0x02009b34
	movs r2, #10
	movs r0, #3
	movs r1, #0
	bl 0x02009b24
	movs r0, #2
	movs r1, #4
	bl 0x02009af4
	movs r0, #2
	movs r1, #4
	bl 0x02009af4
	movs r1, #128
	movs r2, #10
	movs r0, #2
	lsls r1, r1, #6
	bl 0x02009b34
	movs r0, #2
	movs r1, #4
	bl 0x02009aec
	movs r2, #10
	ldr r0, [pc, #20]
	movs r1, #0
	bl 0x02009b24
	movs r0, #8
	movs r1, #4
	b .L_0200069c_7
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00000101
	.4byte 0x00002002
	.4byte 0x00000105
.L_0200069c_7:
	bl 0x02009af4
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
	movs r1, #192
	movs r2, #0
	movs r0, #2
	lsls r1, r1, #8
	bl 0x02009b34
	movs r0, #3
	movs r1, #2
	bl 0x02009b04
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #10
	bl 0x02009b34
	movs r2, #10
	movs r0, #3
	movs r1, #0
	bl 0x02009b24
	movs r0, #8
	movs r1, #4
	bl 0x02009af4
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r0, #10
	movs r1, #2
	bl 0x02009afc
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl 0x02009b24
	movs r0, #11
	movs r1, #3
	bl 0x02009af4
	movs r2, #20
	movs r0, #11
	movs r1, #0
	bl 0x02009b24
	movs r0, #8
	movs r1, #3
	bl 0x02009af4
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r0, #2
	movs r1, #4
	bl 0x02009af4
	movs r0, #2
	movs r1, #4
	bl 0x02009aec
	movs r2, #10
	ldr r0, [pc, #612]
	movs r1, #0
	bl 0x02009b24
	movs r0, #1
	movs r1, #3
	bl 0x02009af4
	movs r2, #10
	movs r0, #1
	movs r1, #0
	bl 0x02009b24
	movs r0, #8
	movs r1, #1
	bl 0x02009b04
	movs r1, #0
	movs r0, #8
	bl 0x02009b14
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b34
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009b34
	movs r1, #160
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b34
	movs r0, #0
	movs r1, #0
	bl 0x02009a9c
	cmp r0, #0
	bne .L_0200069c_8
	movs r0, #20
	bl 0x02009a7c
	movs r0, #10
	bl 0x02009450
	movs r0, #1
	movs r1, #3
	bl 0x02009aec
	movs r0, #2
	movs r1, #3
	bl 0x02009aec
	movs r1, #3
	movs r0, #3
	bl 0x02009af4
	movs r0, #10
	bl 0x02009a7c
	movs r2, #10
	movs r0, #1
	movs r1, #0
	bl 0x02009b24
	ldr r3, [pc, #472]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #2
	strh r3, [r2]
	b .L_0200069c_9
.L_0200069c_8:
	movs r0, #10
	bl 0x02009a7c
	movs r0, #1
	movs r1, #2
	bl 0x02009b04
	ldr r3, [pc, #440]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #10
	movs r0, #1
	bl 0x02009b24
	movs r0, #10
	bl 0x02009450
	movs r0, #1
	movs r1, #3
	bl 0x02009aec
	movs r0, #2
	movs r1, #3
	bl 0x02009aec
	movs r1, #3
	movs r0, #3
	bl 0x02009af4
	movs r0, #10
	bl 0x02009a7c
	movs r0, #1
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
.L_0200069c_9:
	movs r0, #8
	movs r1, #3
	bl 0x02009af4
	movs r0, #8
	movs r1, #0
	movs r2, #40
	bl 0x02009b24
	movs r1, #160
	movs r2, #10
	movs r0, #8
	lsls r1, r1, #7
	bl 0x02009b34
	movs r1, #1
	movs r0, #8
	bl 0x02009b04
	movs r0, #10
	bl 0x02009a7c
	movs r1, #1
	movs r0, #10
	bl 0x02009b04
	movs r0, #20
	bl 0x02009a7c
	movs r1, #192
	movs r2, #10
	movs r0, #8
	lsls r1, r1, #6
	bl 0x02009b34
	movs r1, #1
	movs r0, #8
	bl 0x02009b04
	movs r0, #10
	bl 0x02009a7c
	movs r1, #1
	movs r0, #11
	bl 0x02009b04
	movs r0, #20
	bl 0x02009a7c
	movs r0, #8
	movs r1, #3
	bl 0x02009af4
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r0, #10
	movs r1, #3
	bl 0x02009aec
	movs r0, #11
	movs r1, #3
	bl 0x02009af4
	movs r1, #128
	movs r2, #128
	movs r0, #10
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x02009aac
	movs r1, #128
	movs r2, #128
	movs r0, #11
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x02009aac
	movs r1, #212
	movs r2, #135
	movs r0, #10
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl 0x02009ac4
	movs r1, #220
	movs r2, #135
	movs r0, #11
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl 0x02009acc
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x02009ae4
	movs r2, #0
	movs r0, #11
	movs r1, #0
	bl 0x02009ae4
	movs r0, #8
	movs r1, #3
	bl 0x02009af4
	movs r0, #0
	movs r1, #3
	bl 0x02009aec
	movs r0, #1
	movs r1, #3
	bl 0x02009aec
	movs r0, #2
	movs r1, #3
	bl 0x02009aec
	movs r1, #3
	movs r0, #3
	bl 0x02009af4
	movs r0, #20
	bl 0x02009a7c
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x02009aac
	movs r1, #128
	movs r2, #128
	movs r0, #2
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x02009aac
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #3
	lsls r1, r1, #9
	bl 0x02009aac
	ldr r5, [pc, #76]
	movs r0, #1
	adds r1, r5, #0
	bl 0x02009ab4
	adds r1, r5, #0
	movs r0, #2
	bl 0x02009ab4
	adds r1, r5, #0
	movs r0, #3
	bl 0x02009abc
	ldr r3, [pc, #48]
	ldr r1, [r3]
	movs r3, #228
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #16
	str r3, [r2]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #73
	str r3, [r2]
	ldr r0, [pc, #32]
	bl 0x02009a74
	ldr r0, [pc, #28]
	bl 0x02009a6c
	bl 0x02009a8c
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x00002002
	.4byte 0x03001ebc
	.4byte 0x02009b94
	.4byte 0x0000012f
	.4byte 0x00000912
	.global Func_0200140c
	.thumb_func
Func_0200140c:
	push {r5, lr}
	movs r1, #192
	adds r5, r0, #0
	lsls r1, r1, #7
	movs r0, #0
	movs r2, #0
	bl 0x02009b34
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b34
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #6
	movs r2, #0
	bl 0x02009b34
	movs r1, #160
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b34
	cmp r5, #0
	beq .L_0200140c_0
	adds r0, r5, #0
	bl 0x02009a7c
.L_0200140c_0:
	pop {r5}
	pop {r0}
	bx r0
	.global Func_02001450
	.thumb_func
Func_02001450:
	push {r5, lr}
	movs r1, #192
	adds r5, r0, #0
	lsls r1, r1, #8
	movs r0, #0
	movs r2, #0
	bl 0x02009b34
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b34
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b34
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b34
	cmp r5, #0
	beq .L_02001450_0
	adds r0, r5, #0
	bl 0x02009a7c
.L_02001450_0:
	pop {r5}
	pop {r0}
	bx r0
	.global Func_02001494
	.thumb_func
Func_02001494:
	push {r5, r6, lr}
	bl 0x02009a84
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl 0x02009b54
	movs r0, #1
	bl 0x02009a34
	movs r0, #216
	movs r1, #1
	movs r2, #134
	movs r3, #0
	negs r1, r1
	lsls r2, r2, #18
	lsls r0, r0, #18
	bl 0x02009b54
	bl 0x02009a3c
	movs r0, #1
	bl 0x02009a34
	movs r1, #216
	movs r0, #0
	lsls r1, r1, #18
	ldr r2, [pc, #956]
	bl 0x02009ae4
	ldr r6, [pc, #956]
	movs r1, #224
	ldr r2, [r6]
	lsls r1, r1, #1
	movs r5, #128
	adds r3, r2, r1
	lsls r5, r5, #1
	str r5, [r3]
	movs r3, #228
	lsls r3, r3, #1
	adds r2, r2, r3
	movs r3, #40
	str r3, [r2]
	bl 0x02009b7c
	ldr r0, [pc, #928]
	ldr r1, [pc, #932]
	bl 0x02009b4c
	movs r0, #216
	movs r1, #1
	movs r2, #236
	movs r3, #1
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #17
	bl 0x02009b54
	movs r0, #0
	ldr r1, [pc, #908]
	ldr r2, [pc, #900]
	bl 0x02009aac
	movs r0, #1
	ldr r1, [pc, #900]
	ldr r2, [pc, #888]
	bl 0x02009aac
	movs r0, #2
	ldr r1, [pc, #888]
	ldr r2, [pc, #880]
	bl 0x02009aac
	movs r0, #3
	ldr r1, [pc, #880]
	ldr r2, [pc, #868]
	bl 0x02009aac
	movs r1, #216
	movs r2, #249
	movs r0, #0
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x02009acc
	movs r0, #0
	bl 0x02009aa4
	cmp r0, #0
	beq .L_02001494_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x02009ae4
.L_02001494_0:
	movs r0, #0
	bl 0x02009aa4
	cmp r0, #0
	beq .L_02001494_1
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #2
	bl 0x02009ae4
.L_02001494_1:
	movs r0, #0
	bl 0x02009aa4
	cmp r0, #0
	beq .L_02001494_2
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #3
	bl 0x02009ae4
.L_02001494_2:
	movs r1, #214
	movs r2, #243
	movs r0, #0
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x02009ac4
	movs r1, #212
	movs r2, #251
	movs r0, #1
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x02009ac4
	movs r1, #218
	movs r2, #243
	movs r0, #2
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x02009ac4
	movs r1, #220
	movs r2, #251
	lsls r2, r2, #1
	movs r0, #3
	lsls r1, r1, #2
	bl 0x02009acc
	movs r0, #0
	movs r1, #1
	bl 0x02009aec
	movs r0, #1
	movs r1, #1
	bl 0x02009aec
	movs r1, #1
	movs r0, #2
	bl 0x02009aec
	movs r0, #10
	bl 0x02009a7c
	movs r0, #10
	bl 0x02009450
	adds r1, r5, #0
	movs r0, #9
	movs r2, #20
	bl 0x02009b3c
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #20
	movs r0, #9
	bl 0x02009b34
	ldr r0, [pc, #688]
	bl 0x02009b0c
	ldr r0, [pc, #684]
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
	adds r1, r5, #0
	movs r0, #8
	movs r2, #20
	bl 0x02009b3c
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #20
	bl 0x02009b34
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x02009b24
	movs r0, #8
	ldr r1, [pc, #644]
	movs r2, #60
	bl 0x02009b3c
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x02009b44
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	bl 0x02009b44
	movs r1, #129
	movs r0, #2
	lsls r1, r1, #1
	bl 0x02009b44
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #3
	bl 0x02009b44
	movs r0, #60
	bl 0x02009a7c
	movs r1, #129
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #60
	bl 0x02009b3c
	movs r1, #224
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #10
	bl 0x02009b34
	ldr r0, [pc, #552]
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #5
	movs r2, #10
	bl 0x02009b34
	movs r1, #132
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #20
	bl 0x02009b3c
	movs r2, #40
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r0, #8
	movs r1, #2
	bl 0x02009b04
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x02009b24
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #10
	bl 0x02009b34
	movs r1, #0
	movs r0, #8
	bl 0x02009b14
	movs r0, #0
	movs r1, #0
	movs r5, #1
	bl 0x02009a9c
	cmp r0, #0
	bne .L_02001494_3
	movs r0, #10
	bl 0x02009a7c
	movs r0, #8
	movs r1, #3
	bl 0x02009aec
	b .L_02001494_4
.L_02001494_3:
	movs r0, #10
	bl 0x02009a7c
	ldr r2, [r6]
	movs r1, #236
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #8
	movs r1, #4
	bl 0x02009aec
	movs r5, #0
.L_02001494_4:
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
	cmp r5, #0
	beq .L_02001494_5
	ldr r3, [pc, #376]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02001494_5:
	movs r0, #9
	movs r1, #2
	bl 0x02009afc
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #9
	bl 0x02009b44
	movs r0, #80
	bl 0x02009a7c
	ldr r0, [pc, #356]
	movs r1, #0
	movs r2, #10
.L_0200174a:
	bl 0x02009b24
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #5
	movs r2, #10
	bl 0x02009b34
	movs r2, #40
	movs r0, #8
	ldr r1, [pc, #336]
	bl 0x02009b3c
	movs r0, #2
	movs r1, #3
	bl 0x02009b04
	ldr r0, [pc, #324]
	movs r1, #0
	movs r2, #20
	bl 0x02009b24
	movs r0, #8
	ldr r1, [pc, #316]
	movs r2, #60
	bl 0x02009b3c
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #60
	bl 0x02009b34
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r0, #1
	movs r1, #2
	bl 0x02009afc
	movs r0, #1
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009b34
	movs r1, #192
	movs r2, #10
	movs r0, #2
	lsls r1, r1, #7
	bl 0x02009b34
	movs r0, #2
	movs r1, #2
	bl 0x02009afc
	ldr r0, [pc, #240]
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
	movs r1, #192
	movs r2, #10
	movs r0, #2
	lsls r1, r1, #8
	bl 0x02009b34
	movs r0, #2
	movs r1, #2
	bl 0x02009afc
	movs r2, #20
	ldr r0, [pc, #200]
	movs r1, #0
	bl 0x02009b24
	movs r0, #8
	movs r1, #3
	bl 0x02009af4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x02009b24
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #20
	bl 0x02009b34
	ldr r0, [pc, #172]
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
	movs r1, #160
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #10
	bl 0x02009b34
	movs r0, #3
	ldr r1, [pc, #144]
	movs r2, #40
	bl 0x02009b3c
	movs r2, #10
	movs r0, #3
	movs r1, #0
	bl 0x02009b24
	movs r1, #2
	movs r0, #2
	bl 0x02009b04
	movs r0, #80
	bl 0x02009a7c
	movs r1, #3
	movs r0, #2
	bl 0x02009af4
	movs r0, #20
	bl 0x02009a7c
	movs r1, #224
	movs r2, #10
	movs r0, #1
	lsls r1, r1, #8
	bl 0x02009b34
	movs r0, #1
	movs r1, #4
	bl 0x02009af4
	movs r1, #0
	movs r0, #1
	bl 0x02009b14
	movs r0, #0
	movs r1, #0
	bl 0x02009a9c
	cmp r0, #0
	bne .L_0200174a_0
	movs r0, #20
	bl 0x02009a7c
	ldr r3, [pc, #20]
	movs r1, #236
	ldr r2, [r3]
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200174a_1
	.2byte 0x0000
	.2byte 0x0276
	.4byte 0x03001ebc
	.2byte 0x6666
	.2byte 0x0000
	.2byte 0x0ccc
	.2byte 0x0000
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x2588
	.2byte 0x0000
	.2byte 0x2009
	.2byte 0x0000
	.4byte 0x00000107
	.4byte 0x00002002
	.4byte 0x00000105
	.4byte 0x00006002
.L_0200174a_0:
	movs r0, #20
	bl 0x02009a7c
	movs r0, #1
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
.L_0200174a_1:
	movs r0, #2
	movs r1, #3
	bl 0x02009aec
	movs r1, #3
	movs r0, #3
	bl 0x02009af4
	movs r0, #20
	bl 0x02009450
	movs r0, #9
	movs r1, #2
	bl 0x02009b04
	movs r2, #10
	ldr r0, [pc, #292]
	movs r1, #0
	bl 0x02009b24
	movs r0, #3
	movs r1, #2
	bl 0x02009b04
	movs r0, #3
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
	movs r1, #192
	movs r2, #10
	movs r0, #9
	lsls r1, r1, #6
	bl 0x02009b34
	movs r0, #9
	movs r1, #3
	bl 0x02009af4
.L_0200191e:
	ldr r0, [pc, #248]
	movs r1, #0
	movs r2, #10
	bl 0x02009b24
	movs r0, #2
	ldr r1, [pc, #240]
	movs r2, #60
	bl 0x02009b3c
	movs r2, #10
	ldr r0, [pc, #232]
	movs r1, #0
	bl 0x02009b24
	movs r0, #9
	movs r1, #1
	bl 0x02009b04
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #10
	bl 0x02009b34
	movs r2, #10
	ldr r0, [pc, #196]
	movs r1, #0
	bl 0x02009b24
	movs r0, #1
	movs r1, #2
	bl 0x02009b04
	movs r2, #10
	movs r0, #1
	movs r1, #0
	bl 0x02009b24
	movs r0, #9
	movs r1, #4
	bl 0x02009af4
	movs r2, #10
	ldr r0, [pc, #160]
	movs r1, #0
	bl 0x02009b24
	movs r0, #3
	movs r1, #1
	bl 0x02009b04
	movs r2, #10
	movs r0, #3
	movs r1, #0
	bl 0x02009b24
	movs r1, #1
	movs r0, #8
	bl 0x02009b04
	movs r0, #20
	bl 0x02009a7c
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02009b24
	movs r0, #0
	movs r1, #3
	bl 0x02009aec
	movs r0, #1
	movs r1, #3
	bl 0x02009aec
	movs r0, #2
	movs r1, #3
	bl 0x02009aec
	movs r1, #3
	movs r0, #3
	bl 0x02009af4
	movs r0, #20
	bl 0x02009a7c
	ldr r5, [pc, #84]
	movs r0, #1
	adds r1, r5, #0
	bl 0x02009ab4
	adds r1, r5, #0
	movs r0, #2
	bl 0x02009ab4
	adds r1, r5, #0
	movs r0, #3
	bl 0x02009abc
	ldr r3, [pc, #60]
	ldr r1, [r3]
	movs r3, #228
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #16
	str r3, [r2]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #73
	str r3, [r2]
	ldr r0, [pc, #40]
	bl 0x02009a74
	ldr r0, [pc, #40]
	bl 0x02009a6c
	bl 0x02009a8c
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00002009
	.4byte 0x00000101
	.4byte 0x00002002
	.4byte 0x02009b94
	.4byte 0x03001ebc
	.4byte 0x0000012f
	.4byte 0x00000914
	.include "games/THE BROKEN SEAL/SRC/FIELD/KAREI_KYUDEN/IMPORT.INC"
	.section .rodata,"a",%progbits
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x01e60000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0xffff0000
	.4byte 0x000000a8
	.4byte 0xc00000a8
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
	.4byte 0x000000a8
	.4byte 0xc00000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000f0
	.4byte 0xc0000288
	.4byte 0x002d0000
	.4byte 0x01b80168
	.4byte 0x000002a8
	.4byte 0xffff0002
	.4byte 0x00000058
	.4byte 0x40000228
	.4byte 0x002d0000
	.4byte 0x01b80168
	.4byte 0x000002a8
	.4byte 0xffff0003
	.4byte 0x00000188
	.4byte 0x40000228
	.4byte 0x002d0000
	.4byte 0x01b80168
	.4byte 0x000002a8
	.4byte 0xffff0004
	.4byte 0x000000f0
	.4byte 0x40000198
	.4byte 0x002d0000
	.4byte 0x01b80168
	.4byte 0x000002a8
	.4byte 0xffff0005
	.4byte 0x00000098
	.4byte 0x400001b0
	.4byte 0x002d0000
	.4byte 0x01b80168
	.4byte 0x000002a8
	.4byte 0xffff0006
	.4byte 0x00000108
	.4byte 0xc0000088
	.4byte 0x00280000
	.4byte 0x01310014
	.4byte 0x0000011d
	.4byte 0xffff0007
	.4byte 0x000001f8
	.4byte 0xc0000098
	.4byte 0x017c0000
	.4byte 0x02f30014
	.4byte 0x00000122
	.4byte 0xffff0008
	.4byte 0x000001c0
	.4byte 0x40000070
	.4byte 0x017c0000
	.4byte 0x02f30014
	.4byte 0x00000122
	.4byte 0xffff0009
	.4byte 0x00000360
	.4byte 0xc0000240
	.4byte 0x02ee0000
	.4byte 0x03d90172
	.4byte 0x0000027b
	.4byte 0xffff000a
	.4byte 0x000003a0
	.4byte 0x800000e0
	.4byte 0x03200000
	.4byte 0x04100014
	.4byte 0x000000fa
	.4byte 0xffff000b
	.4byte 0x00000388
	.4byte 0x40000048
	.4byte 0x03200000
	.4byte 0x04100014
	.4byte 0x000000fa
	.4byte 0xffff000c
	.4byte 0x00000268
	.4byte 0xc0000338
	.4byte 0x01d10000
	.4byte 0x02c10280
	.4byte 0x0000036b
	.4byte 0xffff000d
	.4byte 0x00000080
	.4byte 0x00000328
	.4byte 0x00140000
	.4byte 0x010402da
	.4byte 0x0000037a
	.4byte 0xffff000e
	.4byte 0x000000a8
	.4byte 0x40000328
	.4byte 0x00140000
	.4byte 0x010402da
	.4byte 0x0000037a
	.4byte 0xffff0014
	.4byte 0x00000360
	.4byte 0xc0000240
	.4byte 0x02ee0000
	.4byte 0x03d90172
	.4byte 0x0000027b
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000067
	.4byte 0x00102066
	.4byte 0x00206067
	.4byte 0x00307067
	.4byte 0x00409067
	.4byte 0x0050d067
	.4byte 0x00602067
	.4byte 0x00703067
	.4byte 0x0080a067
	.4byte 0x00904067
	.4byte 0x00a08067
	.4byte 0x00b0109c
	.4byte 0x00c0e067
	.4byte 0x00d05067
	.4byte 0x00e0c067
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
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
	.4byte 0x0000002d
	.4byte 0x00000001
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00003000
	.4byte 0x00000031
	.4byte 0x00000001
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00007000
	.4byte 0xffff0099
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002d000
	.4byte 0xffff0099
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002b000
	.4byte 0x0000006c
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x00710000
	.4byte 0x00000000
	.4byte 0x0000006a
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00f60000
	.4byte 0x0000d000
	.4byte 0x000000a7
	.4byte 0x00000001
	.4byte 0x00d40000
	.4byte 0x00000000
	.4byte 0x020a0000
	.4byte 0x00013000
	.4byte 0xffff007e
	.4byte 0x00000001
	.4byte 0x00a90000
	.4byte 0x00000000
	.4byte 0x00720000
	.4byte 0x00003000
	.4byte 0xffff007f
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00720000
	.4byte 0x00005000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008185
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008185
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02008185
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
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
	.4byte 0x00000031
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x0000c602
	.4byte 0xffff000e
	.4byte 0x02008185
	.4byte 0x00000000
	.4byte 0x03210008
	.4byte 0x00001b81
	.4byte 0x00000000
	.4byte 0x09130008
	.4byte 0x02008115
	.4byte 0x00000000
	.4byte 0x09410008
	.4byte 0x00001b95
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000025a6
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000025a5
	.4byte 0x00000000
	.4byte 0x0941000c
	.4byte 0x00001b82
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000025a7
	.4byte 0x00000000
	.4byte 0x0941000d
	.4byte 0x020080d5
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000025a8
	.4byte 0x00000000
	.4byte 0x0941000e
	.4byte 0x00001b86
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000025a9
	.4byte 0x00000000
	.4byte 0x0941000f
	.4byte 0x00001b87
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000025aa
	.4byte 0x00000000
	.4byte 0x09410010
	.4byte 0x020080f5
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x000025ab
	.4byte 0x00008d15
	.4byte 0x09130008
	.4byte 0x00001b8b
	.4byte 0x00008d15
	.4byte 0x09410008
	.4byte 0x00001b96
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000025ad
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000025ac
	.4byte 0x00008d15
	.4byte 0x0941000c
	.4byte 0x00001b8c
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000025ae
	.4byte 0x00008d15
	.4byte 0x0941000d
	.4byte 0x00001b8d
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000025af
	.4byte 0x00008d15
	.4byte 0x0941000e
	.4byte 0x00001b8e
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000025b0
	.4byte 0x00008d15
	.4byte 0x0941000f
	.4byte 0x00001b8f
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000025b1
	.4byte 0x00008d15
	.4byte 0x09410010
	.4byte 0x00001b90
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000025b2
	.4byte 0x00000013
	.4byte 0x0f860064
	.4byte 0x0010005f
	.4byte 0x00000033
	.4byte 0x0f870065
	.4byte 0x001000b5
	.4byte 0x000001c3
	.4byte 0xffff00c8
	.4byte 0x004029d9
	.4byte 0x000000f3
	.4byte 0xffff00c9
	.4byte 0x004029da
	.4byte 0x000000f3
	.4byte 0xffff00ca
	.4byte 0x004029db
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
