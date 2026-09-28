.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/SORU_IRIGUCHI/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	push {lr}
	ldr r3, [pc, #36]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #28]
	cmp r2, r3
	bne .L_02000030_0
	ldr r0, [pc, #24]
	b .L_02000030_1
.L_02000030_0:
	ldr r3, [pc, #24]
	cmp r2, r3
	bne .L_02000030_2
	ldr r0, [pc, #24]
	b .L_02000030_1
.L_02000030_2:
	ldr r0, [pc, #24]
.L_02000030_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000013
	.4byte 0x02009d04
	.4byte 0x00000010
	.4byte 0x02009d64
	.4byte 0x02009cd4
	.global Func_02000070
	.thumb_func
Func_02000070:
	movs r0, #0
	bx lr
	.global Func_02000074
	.thumb_func
Func_02000074:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02009f14
	.global Func_0200007c
	.thumb_func
Func_0200007c:
	push {r5, lr}
	ldr r1, [pc, #76]
	movs r0, #224
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, [pc, #68]
	cmp r2, r3
	bne .L_0200007c_0
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #11
	blt .L_0200007c_1
	cmp r3, #13
	ble .L_0200007c_2
	cmp r3, #16
	bgt .L_0200007c_1
	ldr r0, [pc, #44]
	b .L_0200007c_3
.L_0200007c_2:
	ldr r0, [pc, #44]
	b .L_0200007c_3
.L_0200007c_1:
	ldr r5, [pc, #44]
	adds r0, r5, #0
	bl 0x02009bbc
	adds r0, r5, #0
	b .L_0200007c_3
.L_0200007c_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_0200007c_4
	ldr r0, [pc, #32]
	b .L_0200007c_3
.L_0200007c_4:
	ldr r0, [pc, #32]
.L_0200007c_3:
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000010
	.4byte 0x0200a1b8
	.4byte 0x0200a050
	.4byte 0x02009fd8
	.4byte 0x00000013
	.4byte 0x0200a2a8
	.4byte 0x02009fc0
	.global Func_020000ec
	.thumb_func
Func_020000ec:
	push {lr}
	ldr r1, [pc, #68]
	movs r0, #224
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, [pc, #60]
	cmp r2, r3
	bne .L_020000ec_0
	ldr r0, [pc, #56]
	b .L_020000ec_1
.L_020000ec_0:
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_020000ec_2
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #11
	blt .L_020000ec_3
	cmp r3, #13
	ble .L_020000ec_4
	cmp r3, #16
	bgt .L_020000ec_3
	ldr r0, [pc, #32]
	b .L_020000ec_1
.L_020000ec_4:
	ldr r0, [pc, #32]
	b .L_020000ec_1
.L_020000ec_3:
	ldr r0, [pc, #32]
	b .L_020000ec_1
.L_020000ec_2:
	ldr r0, [pc, #32]
.L_020000ec_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000013
	.4byte 0x0200a2e4
	.4byte 0x00000010
	.4byte 0x0200a524
	.4byte 0x0200a41c
	.4byte 0x0200a32c
	.4byte 0x0200a2d8
	.global Func_02000154
	.thumb_func
Func_02000154:
	push {r5, r6, lr}
	sub sp, #8
	bl 0x02009bac
	movs r0, #181
	bl 0x02009ccc
	movs r5, #3
	movs r6, #2
	movs r1, #28
	movs r2, #21
	movs r3, #3
	movs r0, #16
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x02009b5c
	movs r0, #10
	bl 0x02009b24
	movs r1, #30
	movs r2, #21
	movs r3, #3
	movs r0, #16
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x02009b5c
	movs r0, #10
	bl 0x02009b24
	movs r3, #3
	movs r2, #21
	movs r1, #32
	movs r0, #16
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x02009b5c
	movs r0, #10
	bl 0x02009b24
	movs r0, #0
	movs r1, #2
	bl 0x02009c64
	movs r0, #0
	ldr r1, [pc, #68]
	ldr r2, [pc, #68]
	bl 0x02009bd4
	movs r2, #98
	movs r0, #0
	movs r1, #120
	bl 0x02009be4
	movs r0, #0
	movs r1, #2
	bl 0x02009c04
	movs r2, #8
	movs r1, #0
	negs r2, r2
	movs r0, #0
	bl 0x02009bec
	movs r0, #10
	bl 0x02009ba4
	bl 0x02009cac
	bl 0x02009cb4
	movs r0, #2
	bl 0x02009c94
	bl 0x02009bb4
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x00009999
	.4byte 0x00004ccc
	.global Func_02000200
	.thumb_func
Func_02000200:
	push {lr}
	bl 0x02009bac
	ldr r0, [pc, #60]
	bl 0x02009b8c
	cmp r0, #0
	beq .L_02000200_0
	ldr r0, [pc, #52]
	movs r1, #1
	bl 0x02009b84
	b .L_02000200_1
.L_02000200_0:
	ldr r0, [pc, #48]
	movs r1, #1
	bl 0x02009b84
	ldr r0, [pc, #44]
	bl 0x02009b8c
	cmp r0, #0
	beq .L_02000200_1
	ldr r3, [pc, #36]
	movs r1, #185
	ldr r3, [r3]
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #1
	strh r3, [r2]
.L_02000200_1:
	bl 0x02009bb4
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000081a
	.4byte 0x00001034
	.4byte 0x00001031
	.4byte 0x00000f01
	.4byte 0x03001ebc
	.global Func_02000258
	.thumb_func
Func_02000258:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	ldr r0, [pc, #328]
	sub sp, #8
	bl 0x02009b8c
	cmp r0, #0
	bne .L_02000258_0
	b .L_02000258_1
.L_02000258_0:
	ldr r0, [pc, #316]
	bl 0x02009b8c
	cmp r0, #0
	beq .L_02000258_2
	b .L_02000258_1
.L_02000258_2:
	bl 0x02009bac
	bl 0x02009cbc
	movs r0, #182
	bl 0x02009ccc
	movs r5, #1
	movs r2, #30
	movs r1, #70
	movs r3, #42
	movs r0, #0
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009b5c
	bl 0x02009b44
	movs r0, #40
	bl 0x02009ba4
	ldr r3, [pc, #268]
	mov r8, r3
	movs r1, #1
	mov r0, r8
	bl 0x02009b84
	movs r0, #20
	bl 0x02009ba4
	movs r0, #183
	bl 0x02009ccc
	movs r3, #2
	str r3, [sp, #4]
	movs r6, #3
	movs r0, #0
	movs r1, #29
	movs r2, #3
	movs r3, #1
	str r6, [sp, #0]
	bl 0x02009b5c
	movs r0, #0
	movs r1, #29
	movs r2, #3
	movs r3, #2
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009b64
	movs r3, #81
	movs r0, #1
	movs r1, #109
	movs r2, #4
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009b5c
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
	ldr r2, [pc, #64]
	negs r1, r1
	negs r0, r0
	bl 0x02009b7c
	movs r0, #40
	bl 0x02009ba4
	movs r3, #1
	add r8, r3
	movs r1, #1
	mov r0, r8
	bl 0x02009b84
	ldr r0, [pc, #40]
	bl 0x02009b94
	ldr r0, [pc, #24]
	bl 0x02009b94
	bl 0x02009bb4
.L_02000258_1:
	sub sp, #-8
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x00000f01
	.4byte 0x0000081a
	.4byte 0x00001032
	.4byte 0x0000e666
	.4byte 0x00000143
	.global Func_020003bc
	.thumb_func
Func_020003bc:
	push {r5, lr}
	bl 0x02009bac
	ldr r0, [pc, #72]
	bl 0x02009b8c
	cmp r0, #0
	beq .L_020003bc_0
	ldr r0, [pc, #64]
	movs r1, #1
	bl 0x02009b84
	b .L_020003bc_1
.L_020003bc_0:
	ldr r0, [pc, #60]
	bl 0x02009b8c
	cmp r0, #0
	beq .L_020003bc_2
	ldr r3, [pc, #52]
	ldr r0, [pc, #56]
	movs r1, #1
	ldr r5, [r3]
	bl 0x02009b84
	movs r3, #185
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #1
	strh r3, [r2]
	b .L_020003bc_1
.L_020003bc_2:
	ldr r0, [pc, #32]
	movs r1, #1
	bl 0x02009b84
.L_020003bc_1:
	bl 0x02009bb4
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000821
	.4byte 0x00001034
	.4byte 0x00000f02
	.4byte 0x03001ebc
	.4byte 0x00001031
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
	.global Scene_UpdateOuterActor9Flags
	.thumb_func
Scene_UpdateOuterActor9Flags:
	.global Func_0200056c
	.thumb_func
Func_0200056c:
	push {r5, lr}
	movs r0, #9
	bl 0x02009bcc
	cmp r0, #0
	beq .L_0200056c_0
	ldr r3, [r0, #8]
	ldr r0, [pc, #40]
	asrs r5, r3, #20
	bl 0x02009b9c
	ldr r0, [pc, #36]
	bl 0x02009b9c
	cmp r5, #93
	bne .L_0200056c_1
	ldr r0, [pc, #24]
	bl 0x02009b94
	b .L_0200056c_0
.L_0200056c_1:
	cmp r5, #95
	bne .L_0200056c_0
	ldr r0, [pc, #8]
	bl 0x02009b94
.L_0200056c_0:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00000302
	.4byte 0x00000303
	.global Scene_UpdateOuterActor10Flags
	.thumb_func
Scene_UpdateOuterActor10Flags:
	.global Func_020005ac
	.thumb_func
Func_020005ac:
	push {r5, lr}
	movs r0, #10
	bl 0x02009bcc
	cmp r0, #0
	beq .L_020005ac_0
	ldr r3, [r0, #8]
	movs r0, #192
	lsls r0, r0, #2
	asrs r5, r3, #20
	bl 0x02009b9c
	ldr r0, [pc, #32]
	bl 0x02009b9c
	cmp r5, #115
	bne .L_020005ac_1
	movs r0, #192
	lsls r0, r0, #2
	bl 0x02009b94
	b .L_020005ac_0
.L_020005ac_1:
	cmp r5, #113
	bne .L_020005ac_0
	ldr r0, [pc, #8]
	bl 0x02009b94
.L_020005ac_0:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00000301
	.global Scene_UpdateFormationActor9Flags
	.thumb_func
Scene_UpdateFormationActor9Flags:
	.global Func_020005ec
	.thumb_func
Func_020005ec:
	push {r5, lr}
	movs r0, #9
	bl 0x02009bcc
	cmp r0, #0
	beq .L_020005ec_0
	ldr r3, [r0, #8]
	movs r0, #196
	lsls r0, r0, #2
	asrs r5, r3, #20
	bl 0x02009b9c
	ldr r0, [pc, #40]
	bl 0x02009b9c
	cmp r5, #99
	bne .L_020005ec_1
	ldr r0, [pc, #32]
	bl 0x02009b94
	b .L_020005ec_2
.L_020005ec_1:
	cmp r5, #101
	bne .L_020005ec_2
	movs r0, #196
	lsls r0, r0, #2
	bl 0x02009b94
.L_020005ec_2:
	movs r0, #0
	bl 0x020097c0
.L_020005ec_0:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000311
	.global Scene_UpdateFormationActor10Flags
	.thumb_func
Scene_UpdateFormationActor10Flags:
	.global Func_02000634
	.thumb_func
Func_02000634:
	push {r5, lr}
	movs r0, #10
	bl 0x02009bcc
	cmp r0, #0
	beq .L_02000634_0
	ldr r3, [r0, #8]
	ldr r0, [pc, #48]
	asrs r5, r3, #20
	bl 0x02009b9c
	ldr r0, [pc, #44]
	bl 0x02009b9c
	cmp r5, #103
	bne .L_02000634_1
	ldr r0, [pc, #32]
	bl 0x02009b94
	b .L_02000634_2
.L_02000634_1:
	cmp r5, #105
	bne .L_02000634_2
	ldr r0, [pc, #16]
	bl 0x02009b94
.L_02000634_2:
	movs r0, #0
	bl 0x020097c0
.L_02000634_0:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000312
	.4byte 0x00000313
	.global Scene_UpdateFormationActor11Flags
	.thumb_func
Scene_UpdateFormationActor11Flags:
	.global Func_0200067c
	.thumb_func
Func_0200067c:
	push {r5, lr}
	movs r0, #11
	bl 0x02009bcc
	cmp r0, #0
	beq .L_0200067c_0
	ldr r3, [r0, #8]
	movs r0, #197
	lsls r0, r0, #2
	asrs r5, r3, #20
	bl 0x02009b9c
	ldr r0, [pc, #40]
	bl 0x02009b9c
	cmp r5, #107
	bne .L_0200067c_1
	ldr r0, [pc, #32]
	bl 0x02009b94
	b .L_0200067c_2
.L_0200067c_1:
	cmp r5, #109
	bne .L_0200067c_2
	movs r0, #197
	lsls r0, r0, #2
	bl 0x02009b94
.L_0200067c_2:
	movs r0, #0
	bl 0x020097c0
.L_0200067c_0:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000315
	.global Scene_UpdateFormationActor12Flags
	.thumb_func
Scene_UpdateFormationActor12Flags:
	.global Func_020006c4
	.thumb_func
Func_020006c4:
	push {r5, lr}
	movs r0, #12
	bl 0x02009bcc
	cmp r0, #0
	beq .L_020006c4_0
	ldr r3, [r0, #8]
	ldr r0, [pc, #48]
	asrs r5, r3, #20
	bl 0x02009b9c
	ldr r0, [pc, #44]
	bl 0x02009b9c
	cmp r5, #111
	bne .L_020006c4_1
	ldr r0, [pc, #32]
	bl 0x02009b94
	b .L_020006c4_2
.L_020006c4_1:
	cmp r5, #113
	bne .L_020006c4_2
	ldr r0, [pc, #16]
	bl 0x02009b94
.L_020006c4_2:
	movs r0, #0
	bl 0x020097c0
.L_020006c4_0:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000316
	.4byte 0x00000317
	.global Scene_UpdateFormationActor13Flags
	.thumb_func
Scene_UpdateFormationActor13Flags:
	.global Func_0200070c
	.thumb_func
Func_0200070c:
	push {r5, lr}
	movs r0, #13
	bl 0x02009bcc
	cmp r0, #0
	beq .L_0200070c_0
	ldr r3, [r0, #8]
	movs r0, #198
	lsls r0, r0, #2
	asrs r5, r3, #20
	bl 0x02009b9c
	ldr r0, [pc, #40]
	bl 0x02009b9c
	cmp r5, #115
	bne .L_0200070c_1
	ldr r0, [pc, #32]
	bl 0x02009b94
	b .L_0200070c_2
.L_0200070c_1:
	cmp r5, #117
	bne .L_0200070c_2
	movs r0, #198
	lsls r0, r0, #2
	bl 0x02009b94
.L_0200070c_2:
	movs r0, #0
	bl 0x020097c0
.L_0200070c_0:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000319
	.global Scene_UpdateFormationActor14Flags
	.thumb_func
Scene_UpdateFormationActor14Flags:
	.global Func_02000754
	.thumb_func
Func_02000754:
	push {r5, lr}
	movs r0, #14
	bl 0x02009bcc
	cmp r0, #0
	beq .L_02000754_0
	ldr r3, [r0, #8]
	ldr r0, [pc, #48]
	asrs r5, r3, #20
	bl 0x02009b9c
	ldr r0, [pc, #44]
	bl 0x02009b9c
	cmp r5, #119
	bne .L_02000754_1
	ldr r0, [pc, #32]
	bl 0x02009b94
	b .L_02000754_2
.L_02000754_1:
	cmp r5, #121
	bne .L_02000754_2
	ldr r0, [pc, #16]
	bl 0x02009b94
.L_02000754_2:
	movs r0, #0
	bl 0x020097c0
.L_02000754_0:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000031a
	.4byte 0x0000031b
	.global SceneActor_FindSlotByTilePosition
	.thumb_func
SceneActor_FindSlotByTilePosition:
	.global Func_0200079c
	.thumb_func
Func_0200079c:
	push {r5, lr}
	ldr r3, [pc, #44]
	ldr r3, [r3]
	adds r2, r3, #0
	adds r5, r0, #0
	movs r4, #8
	adds r2, #52
.L_0200079c_2:
	ldmia r2!, {r0}
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r5, r3
	bne .L_0200079c_0
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r1, r3
	beq .L_0200079c_1
.L_0200079c_0:
	adds r4, #1
	cmp r4, #65
	bls .L_0200079c_2
	movs r0, #0
.L_0200079c_1:
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.section .text.x020088f4,"ax",%progbits
	.align 2
	.global Func_020008f4
	.thumb_func
Func_020008f4:
	push {lr}
	ldr r3, [pc, #40]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #32]
	cmp r2, r3
	bne .L_020008f4_0
	bl 0x0200892c
	b .L_020008f4_1
.L_020008f4_0:
	ldr r3, [pc, #24]
	cmp r2, r3
	bne .L_020008f4_1
	bl 0x02008a24
.L_020008f4_1:
	movs r0, #0
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000013
	.4byte 0x00000010
	.global Func_0200092c
	.thumb_func
Func_0200092c:
	push {r5, lr}
	movs r0, #162
	lsls r0, r0, #1
	sub sp, #8
	bl 0x02009b94
	ldr r3, [pc, #208]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #192
	str r2, [r3]
	ldr r0, [pc, #200]
	bl 0x02009b8c
	cmp r0, #0
	beq .L_0200092c_0
	ldr r3, [pc, #192]
	movs r2, #0
	movs r1, #200
	str r2, [r3]
	ldr r0, [pc, #188]
	lsls r1, r1, #4
	bl 0x02009b2c
.L_0200092c_0:
	ldr r0, [pc, #184]
	bl 0x02009b8c
	cmp r0, #0
	beq .L_0200092c_1
	movs r5, #6
	movs r0, #5
	movs r1, #6
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009b64
	movs r3, #7
	str r3, [sp, #0]
	movs r0, #5
	movs r1, #6
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x02009b64
	movs r3, #8
	str r3, [sp, #0]
	movs r0, #5
	movs r1, #6
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x02009b64
	movs r3, #5
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #1
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl 0x02009b64
.L_0200092c_1:
	ldr r0, [pc, #108]
	bl 0x02009b8c
	cmp r0, #0
	beq .L_0200092c_2
	movs r1, #240
	movs r2, #232
	movs r0, #8
	lsls r1, r1, #15
	lsls r2, r2, #16
	bl 0x02009bfc
	movs r3, #6
	movs r5, #14
	str r3, [sp, #0]
	movs r0, #2
	movs r1, #10
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x02009b64
	movs r3, #7
	str r3, [sp, #0]
	movs r0, #2
	movs r1, #10
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x02009b64
	movs r3, #8
	str r3, [sp, #0]
	movs r0, #2
	movs r1, #10
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x02009b64
.L_0200092c_2:
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00000814
	.4byte 0x0200a69c
	.4byte 0x02009ac9
	.4byte 0x00000879
	.4byte 0x00000815
	.global Func_02000a24
	.thumb_func
Func_02000a24:
	push {r5, r6, lr}
	ldr r3, [pc, #700]
	movs r0, #224
	ldr r3, [r3]
	lsls r0, r0, #1
	movs r2, #129
	adds r3, r3, r0
	lsls r2, r2, #2
	str r2, [r3]
	ldr r0, [pc, #688]
	sub sp, #8
	bl 0x02009b8c
	cmp r0, #0
	beq .L_02000a24_0
	movs r0, #141
	bl 0x02009cc4
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl 0x02009b7c
	bl 0x02009c9c
.L_02000a24_0:
	ldr r1, [pc, #652]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	subs r3, #1
	cmp r3, #15
	bls .L_02000a24_1
	b .L_02000a24_2
.L_02000a24_1:
	ldr r2, [pc, #636]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	ldrh r0, [r7, #20]
	lsls r0, r0, #8
	ldrh r0, [r7, #20]
	lsls r0, r0, #8
	ldrh r0, [r2, #24]
	lsls r0, r0, #8
	ldrh r2, [r3, #38]
	lsls r0, r0, #8
	ldrh r2, [r3, #38]
	lsls r0, r0, #8
	ldrh r2, [r3, #38]
	lsls r0, r0, #8
	ldrh r2, [r3, #38]
	lsls r0, r0, #8
	ldrh r6, [r3, #24]
	lsls r0, r0, #8
	ldrh r2, [r3, #38]
	lsls r0, r0, #8
	ldrh r2, [r3, #38]
	lsls r0, r0, #8
	ldrh r2, [r0, #26]
	lsls r0, r0, #8
	ldrh r2, [r0, #26]
	lsls r0, r0, #8
	ldrh r2, [r0, #26]
	lsls r0, r0, #8
	ldrh r0, [r6, #32]
	lsls r0, r0, #8
	ldrh r0, [r6, #32]
	lsls r0, r0, #8
	ldrh r0, [r6, #32]
	lsls r0, r0, #8
	ldr r0, [pc, #568]
	bl 0x02009b8c
	cmp r0, #0
	bne .L_02000a24_3
	b .L_02000a24_2
.L_02000a24_3:
	movs r5, #1
	movs r0, #1
	movs r1, #109
	movs r2, #4
	movs r3, #81
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009b5c
	movs r0, #0
	movs r1, #70
	movs r2, #30
	movs r3, #42
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009b5c
	movs r3, #2
	str r3, [sp, #4]
	movs r6, #3
	movs r0, #0
	movs r1, #29
	movs r2, #3
	movs r3, #1
	str r6, [sp, #0]
	bl 0x02009b5c
	movs r0, #0
	movs r1, #29
	movs r2, #3
	movs r3, #2
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009b64
	bl 0x02009b44
	b .L_02000a24_2
	.2byte 0x2009
	.2byte 0xf001
	.2byte 0xf85b
	.2byte 0x2100
	.2byte 0xf001
	.2byte 0xf82c
	.2byte 0xe0dd
	.2byte 0x2090
	.2byte 0x4a75
	.2byte 0x0080
	.2byte 0x180b
	.2byte 0x801a
	.2byte 0x4b74
	.2byte 0x18ca
	.2byte 0x2308
	.2byte 0x8013
	.2byte 0x4873
	.2byte 0xf001
	.2byte 0xf82b
	.2byte 0x2800
	.2byte 0xd000
	.2byte 0xe0ce
	.2byte 0xf000
	.2byte 0xf8ee
	.2byte 0xe0cb
	.2byte 0x2009
	.2byte 0xf001
	.2byte 0xf842
	.2byte 0x2100
	.2byte 0xf001
	.2byte 0xf813
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xf83c
	.2byte 0x2100
	.2byte 0xf001
	.2byte 0xf80d
	.2byte 0x200b
	.2byte 0xf001
	.2byte 0xf836
	.2byte 0x2100
	.2byte 0xf001
	.2byte 0xf807
	.2byte 0x200c
	.2byte 0xf001
	.2byte 0xf830
	.2byte 0x2100
	.2byte 0xf001
	.2byte 0xf801
	.2byte 0x200d
	.2byte 0xf001
	.2byte 0xf82a
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xfffb
	.2byte 0x200e
	.2byte 0xf001
	.2byte 0xf824
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xfff5
	.2byte 0x200f
	.2byte 0xf001
	.2byte 0xf81e
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xffef
	.2byte 0x2010
	.2byte 0xf001
	.2byte 0xf818
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xffe9
	.2byte 0x2011
	.2byte 0xf001
	.2byte 0xf812
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xffe3
	.2byte 0x2012
	.2byte 0xf001
	.2byte 0xf80c
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xffdd
	.2byte 0x2013
	.2byte 0xf001
	.2byte 0xf806
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xffd7
	.2byte 0x484f
	.2byte 0xf000
	.2byte 0xffe0
	.2byte 0x2800
	.2byte 0xd101
	.2byte 0xf000
	.2byte 0xf9dc
	.2byte 0x484c
	.2byte 0xf000
	.2byte 0xffd9
	.2byte 0x2800
	.2byte 0xd007
	.2byte 0x21bb
	.2byte 0x2288
	.2byte 0x2009
	.2byte 0x04c9
	.2byte 0x0412
	.2byte 0xf001
	.2byte 0xf808
	.2byte 0xe00b
	.2byte 0x4847
	.2byte 0xf000
	.2byte 0xffcc
	.2byte 0x2800
	.2byte 0xd006
	.2byte 0x21bf
	.2byte 0x2288
	.2byte 0x2009
	.2byte 0x04c9
	.2byte 0x0412
	.2byte 0xf000
	.2byte 0xfffb
	.2byte 0x4842
	.2byte 0xf000
	.2byte 0xffc0
	.2byte 0x2800
	.2byte 0xd001
	.2byte 0x21e3
	.2byte 0xe006
	.2byte 0x20c0
	.2byte 0x0080
	.2byte 0xf000
	.2byte 0xffb8
	.2byte 0x2800
	.2byte 0xd05c
	.2byte 0x21e7
	.2byte 0x2288
	.2byte 0x200a
	.2byte 0x04c9
	.2byte 0x0412
	.2byte 0xf000
	.2byte 0xffe7
	.2byte 0xe054
	.2byte 0x2009
	.2byte 0xf000
	.2byte 0xffcb
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xff9c
	.2byte 0x200a
	.2byte 0xf000
	.2byte 0xffc5
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xff96
	.2byte 0x200b
	.2byte 0xf000
	.2byte 0xffbf
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xff90
	.2byte 0x200c
	.2byte 0xf000
	.2byte 0xffb9
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xff8a
	.2byte 0x200d
	.2byte 0xf000
	.2byte 0xffb3
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xff84
	.2byte 0x200e
	.2byte 0xf000
	.2byte 0xffad
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xff7e
	.2byte 0x4826
	.2byte 0xf000
	.2byte 0xff87
	.2byte 0x2800
	.2byte 0xd101
	.2byte 0xf000
	.2byte 0xfccb
	.2byte 0x2001
	.2byte 0xf000
	.2byte 0xfd9a
	.2byte 0x208d
	.2byte 0x0080
	.2byte 0xf000
	.2byte 0xff80
	.2byte 0x4820
	.2byte 0xf000
	.2byte 0xff79
	.2byte 0x2800
	.2byte 0xd01d
	.2byte 0x2501
	.2byte 0x2000
	.2byte 0x2147
	.2byte 0x2264
	.2byte 0x2347
	.2byte 0x9500
	.2byte 0x9501
	.2byte 0xf000
	.2byte 0xff56
	.2byte 0x2302
	.2byte 0x9301
	.2byte 0x207a
	.2byte 0x2114
	.2byte 0x2278
	.2byte 0x231e
	.2byte 0x9500
	.2byte 0xf000
	.2byte 0xff4d
	.2byte 0x2378
	.2byte 0x221e
	.2byte 0x9300
	.2byte 0x9201
	.2byte 0x207a
	.2byte 0x2114
	.2byte 0x2201
	.2byte 0x2302
	.2byte 0xf000
	.2byte 0xff47
	.2byte 0xf000
	.2byte 0xff35
.L_02000a24_2:
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00000814
	.4byte 0x02000240
	.4byte 0x02008a78
	.4byte 0x0000081a
	.2byte 0x0010
	.2byte 0x0000
	.2byte 0x0242
	.2byte 0x0000
	.2byte 0x0802
	.2byte 0x0000
	.2byte 0x0804
	.2byte 0x0000
	.2byte 0x0303
	.2byte 0x0000
	.2byte 0x0302
	.2byte 0x0000
	.2byte 0x0301
	.2byte 0x0000
	.2byte 0x0825
	.2byte 0x0000
	.2byte 0x0821
	.2byte 0x0000
	.global Func_02000d1c
	.thumb_func
Func_02000d1c:
	push {r5, lr}
	bl 0x02009bac
	ldr r5, [pc, #584]
	movs r2, #224
	ldr r3, [r5]
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #192
	str r2, [r3]
	bl 0x02009ca4
	movs r1, #0
	movs r0, #0
	bl 0x02009c04
	movs r0, #4
	bl 0x02009ba4
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	negs r1, r1
	bl 0x02009c84
	ldr r0, [pc, #536]
	ldr r1, [pc, #540]
	bl 0x02009c7c
	movs r0, #153
	movs r1, #1
	movs r2, #136
	lsls r0, r0, #19
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	bl 0x02009c84
	movs r0, #0
	bl 0x02009bcc
	cmp r0, #0
	beq .L_02000d1c_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #8
	bl 0x02009bfc
.L_02000d1c_0:
	movs r0, #0
	bl 0x02009bcc
	cmp r0, #0
	beq .L_02000d1c_1
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #5
	bl 0x02009bfc
.L_02000d1c_1:
	movs r0, #0
	bl 0x02009bcc
	cmp r0, #0
	beq .L_02000d1c_2
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x02009bfc
.L_02000d1c_2:
	movs r0, #8
	ldr r1, [pc, #448]
	ldr r2, [pc, #456]
	bl 0x02009bd4
	movs r0, #5
	ldr r1, [pc, #440]
	ldr r2, [pc, #444]
	bl 0x02009bd4
	ldr r2, [pc, #440]
	movs r0, #1
	ldr r1, [pc, #428]
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
	movs r2, #32
	negs r2, r2
	movs r1, #0
	movs r0, #8
	bl 0x02009bec
	movs r0, #1
	bl 0x02009bf4
	movs r0, #1
	movs r1, #0
	bl 0x02009c04
	movs r0, #5
	movs r1, #0
	bl 0x02009c04
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
.L_02000e20:
	bl 0x02009c5c
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #5
	bl 0x02009c5c
	movs r0, #8
	bl 0x02009bf4
	movs r1, #1
	movs r0, #8
	bl 0x02009c04
	movs r0, #40
	bl 0x02009ba4
	movs r1, #2
	movs r0, #8
	bl 0x02009c24
	movs r0, #20
	bl 0x02009ba4
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #40
	bl 0x02009c5c
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
.L_02000e64:
	movs r2, #40
	bl 0x02009c5c
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #20
	bl 0x02009c5c
	movs r2, #20
	movs r1, #4
	movs r0, #8
	bl 0x02009c14
	ldr r0, [pc, #248]
	bl 0x02009c34
	movs r1, #0
	ldr r0, [pc, #244]
	bl 0x02009c54
	movs r0, #20
	bl 0x02009ba4
	movs r0, #153
	movs r1, #1
	movs r2, #148
	lsls r0, r0, #19
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	bl 0x02009c84
	movs r0, #1
	movs r1, #2
.L_02000eaa:
	bl 0x02009c04
	movs r0, #0
	bl 0x02009bcc
	cmp r0, #0
	beq .L_02000eaa_0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x02009bdc
.L_02000eaa_0:
	movs r0, #5
	movs r1, #2
	bl 0x02009c04
	movs r0, #0
	bl 0x02009bcc
	cmp r0, #0
	beq .L_02000eaa_1
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl 0x02009bdc
.L_02000eaa_1:
	movs r0, #8
	movs r1, #2
	bl 0x02009c04
	movs r0, #0
	bl 0x02009bcc
	cmp r0, #0
.L_02000ef6:
	beq .L_02000ef6_0
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #8
	bl 0x02009bdc
.L_02000ef6_0:
	movs r0, #1
	bl 0x02009bf4
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x02009bfc
	movs r1, #0
	movs r2, #0
	movs r0, #5
	bl 0x02009bfc
	movs r0, #8
	bl 0x02009bf4
	movs r2, #0
	movs r0, #8
	movs r1, #0
	bl 0x02009bfc
	movs r0, #1
	movs r1, #1
	bl 0x02009c04
	movs r0, #5
	movs r1, #1
	bl 0x02009c04
	movs r1, #1
.L_02000f42:
	movs r0, #8
	bl 0x02009c04
	ldr r0, [pc, #56]
	bl 0x02009b94
	ldr r3, [r5]
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #68
	str r2, [r3]
	ldr r0, [pc, #44]
	bl 0x02009b9c
	bl 0x02009bb4
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x9999
	.2byte 0x0000
	.2byte 0x1333
	.2byte 0x0000
	.2byte 0x4ccc
	.2byte 0x0000
	.2byte 0x0fd3
	.2byte 0x0000
	.2byte 0x4008
	.2byte 0x0000
	.4byte 0x00000802
	.4byte 0x0000012f
	.global Func_02000f8c
	.thumb_func
Func_02000f8c:
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
	.global Func_0200161c
	.thumb_func
Func_0200161c:
	push {lr}
	bl 0x02009bac
	ldr r3, [pc, #368]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #68
	str r2, [r3]
	bl 0x02009ca4
	bl 0x02009cb4
	movs r0, #20
	bl 0x02009ba4
	movs r0, #0
	bl 0x02009bcc
	cmp r0, #0
	beq .L_0200161c_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #8
	bl 0x02009bfc
.L_0200161c_0:
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #8
	lsls r1, r1, #9
	bl 0x02009bd4
	movs r0, #8
	movs r1, #2
	bl 0x02009c04
	movs r2, #10
	negs r2, r2
	movs r1, #24
	movs r0, #8
	bl 0x02009bec
	movs r0, #8
	bl 0x02009bf4
	movs r1, #1
	movs r0, #8
	bl 0x02009c04
	movs r0, #6
	bl 0x02009ba4
	movs r1, #176
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009c5c
	movs r1, #192
	movs r2, #40
	movs r0, #0
	lsls r1, r1, #8
	bl 0x02009c5c
	ldr r0, [pc, #244]
	ldr r1, [pc, #248]
	bl 0x02009c7c
	movs r0, #209
	movs r1, #1
	movs r2, #131
	lsls r2, r2, #18
	movs r3, #1
	negs r1, r1
	lsls r0, r0, #19
	bl 0x02009c84
	bl 0x02009c8c
	movs r0, #20
	bl 0x02009ba4
	ldr r0, [pc, #216]
	ldr r1, [pc, #220]
	bl 0x02009c7c
	movs r0, #235
	movs r1, #1
	movs r2, #131
	lsls r2, r2, #18
	movs r3, #1
	negs r1, r1
	lsls r0, r0, #19
	bl 0x02009c84
	bl 0x02009c8c
	movs r0, #20
	bl 0x02009ba4
	ldr r0, [pc, #188]
	ldr r1, [pc, #192]
	bl 0x02009c7c
	movs r1, #1
	movs r2, #137
	movs r3, #1
	lsls r2, r2, #18
	negs r1, r1
	ldr r0, [pc, #180]
	bl 0x02009c84
	bl 0x02009c8c
	movs r0, #20
	bl 0x02009ba4
	movs r0, #8
	movs r1, #2
	bl 0x02009c24
	movs r1, #0
	movs r2, #30
	movs r0, #8
	bl 0x02009c5c
	ldr r0, [pc, #148]
	bl 0x02009c34
	ldr r0, [pc, #148]
	movs r1, #0
	movs r2, #10
	bl 0x02009c4c
	movs r1, #128
	movs r2, #40
	movs r0, #8
	lsls r1, r1, #1
	bl 0x02009c6c
	movs r0, #8
	movs r1, #1
	bl 0x02009c24
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #20
	bl 0x02009c5c
	ldr r0, [pc, #104]
	movs r1, #0
	movs r2, #10
	bl 0x02009c4c
	movs r0, #8
	movs r1, #2
	bl 0x02009c04
	movs r0, #0
	bl 0x02009bcc
	cmp r0, #0
	beq .L_0200161c_1
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #8
	bl 0x02009bdc
.L_0200161c_1:
	movs r0, #8
	bl 0x02009bf4
	movs r1, #0
	movs r2, #0
	movs r0, #8
	bl 0x02009bfc
	ldr r0, [pc, #52]
	bl 0x02009b94
	bl 0x02009bb4
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x00026666
	.4byte 0x00004ccc
	.4byte 0x00019999
	.4byte 0x00003333
	.4byte 0x00033333
	.4byte 0x00006666
	.4byte 0x06e90000
	.4byte 0x0000103a
	.4byte 0x00004008
	.4byte 0x00000825
	.global Func_020017c0
	.thumb_func
Func_020017c0:
	push {r5, r6, r7, lr}
	sub sp, #8
	movs r5, #32
	movs r1, #20
	movs r2, #1
	movs r3, #1
	adds r6, r0, #0
	movs r7, #100
	movs r0, #122
	str r7, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009b64
	movs r3, #104
	str r3, [sp, #0]
	movs r0, #122
	movs r1, #20
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x02009b64
	movs r3, #108
	str r3, [sp, #0]
	movs r0, #122
	movs r1, #20
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x02009b64
	movs r3, #112
	str r3, [sp, #0]
	movs r0, #122
	movs r1, #20
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x02009b64
	movs r3, #116
	str r3, [sp, #0]
	movs r0, #122
	movs r1, #20
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x02009b64
	movs r3, #120
	str r3, [sp, #0]
	movs r0, #122
	movs r1, #20
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x02009b64
	ldr r0, [pc, #592]
	bl 0x02009b8c
	cmp r0, #0
	beq .L_020017c0_0
	movs r0, #121
	movs r1, #20
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009b64
	cmp r6, #0
	beq .L_020017c0_1
	movs r1, #199
	movs r2, #130
	movs r0, #9
	lsls r1, r1, #19
	lsls r2, r2, #18
	bl 0x02009bfc
	b .L_020017c0_1
.L_020017c0_0:
	movs r0, #196
	lsls r0, r0, #2
	bl 0x02009b8c
	cmp r0, #0
	beq .L_020017c0_1
	movs r0, #121
	movs r1, #20
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009b64
	cmp r6, #0
	beq .L_020017c0_1
	movs r1, #203
	movs r2, #130
	movs r0, #9
	lsls r1, r1, #19
	lsls r2, r2, #18
	bl 0x02009bfc
.L_020017c0_1:
	ldr r0, [pc, #504]
	bl 0x02009b8c
	cmp r0, #0
	beq .L_020017c0_2
	movs r3, #104
	movs r2, #32
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #121
	movs r1, #20
	movs r2, #1
	movs r3, #1
	bl 0x02009b64
	cmp r6, #0
	beq .L_020017c0_3
	movs r1, #207
	movs r2, #130
	movs r0, #10
	lsls r1, r1, #19
	lsls r2, r2, #18
	bl 0x02009bfc
	b .L_020017c0_3
.L_020017c0_2:
	ldr r0, [pc, #460]
	bl 0x02009b8c
	cmp r0, #0
	beq .L_020017c0_3
	movs r3, #104
	movs r2, #32
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #121
	movs r1, #20
	movs r2, #1
	movs r3, #1
	bl 0x02009b64
	cmp r6, #0
	beq .L_020017c0_3
	movs r1, #211
	movs r2, #130
	movs r0, #10
	lsls r1, r1, #19
	lsls r2, r2, #18
	bl 0x02009bfc
.L_020017c0_3:
	ldr r0, [pc, #416]
	bl 0x02009b8c
	cmp r0, #0
	beq .L_020017c0_4
	movs r3, #108
	movs r2, #32
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #121
	movs r1, #20
	movs r2, #1
	movs r3, #1
	bl 0x02009b64
	cmp r6, #0
	beq .L_020017c0_5
	movs r1, #215
	movs r2, #130
	movs r0, #11
	lsls r1, r1, #19
	lsls r2, r2, #18
	bl 0x02009bfc
	b .L_020017c0_5
.L_020017c0_4:
	movs r0, #197
	lsls r0, r0, #2
	bl 0x02009b8c
	cmp r0, #0
	beq .L_020017c0_5
	movs r3, #108
	movs r2, #32
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #121
	movs r1, #20
	movs r2, #1
	movs r3, #1
	bl 0x02009b64
	cmp r6, #0
	beq .L_020017c0_5
	movs r1, #219
	movs r2, #130
	movs r0, #11
	lsls r1, r1, #19
	lsls r2, r2, #18
	bl 0x02009bfc
.L_020017c0_5:
	ldr r0, [pc, #320]
	bl 0x02009b8c
	cmp r0, #0
	beq .L_020017c0_6
	movs r3, #112
	movs r2, #32
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #121
	movs r1, #20
	movs r2, #1
	movs r3, #1
	bl 0x02009b64
	cmp r6, #0
	beq .L_020017c0_7
	movs r1, #223
	movs r2, #130
	movs r0, #12
	lsls r1, r1, #19
	lsls r2, r2, #18
	bl 0x02009bfc
	b .L_020017c0_7
.L_020017c0_6:
	ldr r0, [pc, #272]
	bl 0x02009b8c
	cmp r0, #0
	beq .L_020017c0_7
	movs r3, #112
	movs r2, #32
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #121
	movs r1, #20
	movs r2, #1
	movs r3, #1
	bl 0x02009b64
	cmp r6, #0
	beq .L_020017c0_7
	movs r1, #227
	movs r2, #130
	movs r0, #12
	lsls r1, r1, #19
	lsls r2, r2, #18
	bl 0x02009bfc
.L_020017c0_7:
	ldr r0, [pc, #228]
	bl 0x02009b8c
	cmp r0, #0
	beq .L_020017c0_8
	movs r3, #116
	movs r2, #32
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #121
	movs r1, #20
	movs r2, #1
	movs r3, #1
	bl 0x02009b64
	cmp r6, #0
	beq .L_020017c0_9
	movs r1, #231
	movs r2, #130
	movs r0, #13
	lsls r1, r1, #19
	lsls r2, r2, #18
	bl 0x02009bfc
	b .L_020017c0_9
.L_020017c0_8:
	movs r0, #198
	lsls r0, r0, #2
	bl 0x02009b8c
	cmp r0, #0
	beq .L_020017c0_9
	movs r3, #116
	movs r2, #32
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #121
	movs r1, #20
	movs r2, #1
	movs r3, #1
	bl 0x02009b64
	cmp r6, #0
	beq .L_020017c0_9
	movs r1, #235
	movs r2, #130
	movs r0, #13
	lsls r1, r1, #19
	lsls r2, r2, #18
	bl 0x02009bfc
.L_020017c0_9:
	ldr r0, [pc, #132]
	bl 0x02009b8c
	cmp r0, #0
	beq .L_020017c0_10
	movs r3, #120
	movs r2, #32
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #121
	movs r1, #20
	movs r2, #1
	movs r3, #1
	bl 0x02009b64
	cmp r6, #0
	beq .L_020017c0_11
	movs r1, #239
	movs r2, #130
	movs r0, #14
	lsls r1, r1, #19
	lsls r2, r2, #18
	bl 0x02009bfc
	b .L_020017c0_11
.L_020017c0_10:
	ldr r0, [pc, #88]
	bl 0x02009b8c
	cmp r0, #0
	beq .L_020017c0_11
	movs r3, #120
	movs r2, #32
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #121
	movs r1, #20
	movs r2, #1
	movs r3, #1
	bl 0x02009b64
	cmp r6, #0
	beq .L_020017c0_11
	movs r1, #243
	movs r2, #130
	movs r0, #14
	lsls r1, r1, #19
	lsls r2, r2, #18
	bl 0x02009bfc
.L_020017c0_11:
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000311
	.4byte 0x00000313
	.4byte 0x00000312
	.4byte 0x00000315
	.4byte 0x00000317
	.4byte 0x00000316
	.4byte 0x00000319
	.4byte 0x0000031b
	.4byte 0x0000031a
	.global Func_02001aac
	.thumb_func
Func_02001aac:
	push {lr}
	bl 0x02009bac
	ldr r0, [pc, #16]
	movs r1, #1
	bl 0x02009b84
	bl 0x02009bb4
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000953
	.global Func_02001ac8
	.thumb_func
Func_02001ac8:
	push {r5, lr}
	ldr r5, [pc, #80]
	ldr r3, [r5]
	cmp r3, #0
	beq .L_02001ac8_0
	subs r3, #1
	str r3, [r5]
	cmp r3, #40
	bne .L_02001ac8_1
	movs r0, #1
	movs r1, #1
	negs r0, r0
	negs r1, r1
	ldr r2, [pc, #60]
	bl 0x02009b7c
	b .L_02001ac8_1
.L_02001ac8_0:
	bl 0x02009b34
	lsls r3, r0, #4
	subs r3, r3, r0
	lsls r3, r3, #3
	lsrs r3, r3, #16
	cmp r3, #0
	bne .L_02001ac8_1
	movs r0, #138
	bl 0x02009ccc
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x02009b7c
	movs r3, #80
	str r3, [r5]
.L_02001ac8_1:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200a69c
	.4byte 0x0000e666
	.include "games/THE BROKEN SEAL/SRC/FIELD/SORU_IRIGUCHI/IMPORT.INC"
	.section .rodata.part1,"a",%progbits
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
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
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
