.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/ENTRY.INC"
	.section .text.x020089dc,"ax",%progbits
	.global Func_020009dc
	.thumb_func
Func_020009dc:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_020009dc_0
	ldr r0, [pc, #36]
	b .L_020009dc_1
.L_020009dc_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_020009dc_2
	ldr r0, [pc, #36]
	b .L_020009dc_1
.L_020009dc_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_020009dc_3
	ldr r0, [pc, #32]
	b .L_020009dc_1
.L_020009dc_3:
	ldr r0, [pc, #32]
.L_020009dc_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000a0
	.4byte 0x0200e2a4
	.4byte 0x000000a1
	.4byte 0x0200e4b4
	.4byte 0x000000a2
	.4byte 0x0200e754
	.4byte 0x0200e814
	.global Func_02000a30
	.thumb_func
Func_02000a30:
	movs r0, #0
	bx lr
	.global Func_02000a34
	.thumb_func
Func_02000a34:
	push {lr}
	ldr r3, [pc, #44]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000a34_0
	ldr r0, [pc, #32]
	b .L_02000a34_1
.L_02000a34_0:
	ldr r3, [pc, #32]
	cmp r2, r3
	beq .L_02000a34_2
	ldr r3, [pc, #32]
	cmp r2, r3
	bne .L_02000a34_3
.L_02000a34_2:
	ldr r0, [pc, #28]
	b .L_02000a34_1
.L_02000a34_3:
	ldr r0, [pc, #28]
.L_02000a34_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000a1
	.4byte 0x0200e910
	.4byte 0x000000a2
	.4byte 0x000000a3
	.4byte 0x0200e97c
	.4byte 0x0200e8a4
.L_02000a80:
	.global Func_02000a80
	.thumb_func
Func_02000a80:
	push {lr}
	ldr r3, [pc, #68]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #60]
	cmp r2, r3
	bne .L_02000a80_0
	ldr r0, [pc, #56]
	b .L_02000a80_1
.L_02000a80_0:
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_02000a80_2
	ldr r0, [pc, #56]
	b .L_02000a80_1
.L_02000a80_2:
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_02000a80_3
	ldr r0, [pc, #52]
	b .L_02000a80_1
.L_02000a80_3:
	ldr r3, [pc, #52]
	cmp r2, r3
	bne .L_02000a80_4
	ldr r0, [pc, #52]
	b .L_02000a80_1
.L_02000a80_4:
	ldr r3, [pc, #52]
	cmp r2, r3
	bne .L_02000a80_5
	ldr r0, [pc, #48]
	b .L_02000a80_1
.L_02000a80_5:
	ldr r0, [pc, #48]
.L_02000a80_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000006a
	.4byte 0x0200e9d0
	.4byte 0x000000a2
	.4byte 0x0200ee08
	.4byte 0x000000a1
	.4byte 0x0200ec28
	.4byte 0x000000a0
	.4byte 0x0200eac0
	.4byte 0x000000a3
	.4byte 0x0200ee98
	.4byte 0x0200e9b8
	.global Func_02000af8
	.thumb_func
Func_02000af8:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_02000af8_0
	ldr r0, [pc, #36]
	b .L_02000af8_1
.L_02000af8_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000af8_2
	ldr r0, [pc, #36]
	b .L_02000af8_1
.L_02000af8_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000af8_3
	ldr r0, [pc, #32]
	b .L_02000af8_1
.L_02000af8_3:
	ldr r0, [pc, #32]
.L_02000af8_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000a0
	.4byte 0x0200eff4
	.4byte 0x000000a1
	.4byte 0x0200f258
	.4byte 0x000000a2
	.4byte 0x0200f528
	.4byte 0x0200f63c
	.global Func_02000b4c
	.thumb_func
Func_02000b4c:
	push {r5, lr}
	sub sp, #8
	movs r3, #22
	str r3, [sp, #4]
	movs r5, #15
	movs r0, #15
	movs r1, #20
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200d5d8
	movs r3, #23
	str r3, [sp, #4]
	movs r0, #17
	movs r1, #23
	movs r2, #1
	movs r3, #3
	str r5, [sp, #0]
	bl 0x0200d5d8
	movs r0, #12
	bl 0x0200d650
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02000b4c_0
	movs r1, #0
	bl 0x0200d5e8
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	subs r2, #50
	movs r3, #2
	strb r3, [r2]
.L_02000b4c_0:
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000ba0
	.thumb_func
Func_02000ba0:
	push {lr}
	bl 0x020080c4
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000bac
	.thumb_func
Func_02000bac:
	push {lr}
	movs r1, #128
	movs r2, #128
	movs r0, #12
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200d658
	movs r2, #188
	movs r1, #248
	lsls r2, r2, #1
	movs r0, #12
	bl 0x0200d678
	movs r0, #12
	bl 0x0200d698
	movs r0, #215
	bl 0x0200d7a8
	movs r0, #60
	bl 0x0200d628
	bl 0x02008b4c
	ldr r0, [pc, #8]
	bl 0x0200d618
	pop {r0}
	bx r0
	.4byte 0x00000943
	.global Func_02000bec
	.thumb_func
Func_02000bec:
	push {r5, lr}
	movs r0, #12
.L_02000bf0:
	sub sp, #8
	bl 0x0200d650
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, #22
	ble 0x02008c68
	movs r0, #128
	movs r1, #128
.L_02000c02:
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl 0x0200d5f0
	movs r0, #1
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #88]
	negs r0, r0
	bl 0x0200d5f0
	movs r0, #144
	bl 0x0200d7a8
	movs r3, #22
	str r3, [sp, #4]
	movs r5, #15
	movs r0, #15
	movs r1, #20
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200d5d8
	movs r3, #23
	str r3, [sp, #4]
	movs r0, #17
	movs r1, #23
	movs r2, #1
	movs r3, #3
	str r5, [sp, #0]
	bl 0x0200d5d8
	movs r0, #12
	bl 0x0200d650
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02000c02_0
	movs r1, #0
	bl 0x0200d5e8
	adds r2, r5, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
.L_02000c02_0:
	ldr r0, [pc, #16]
	bl 0x0200d618
.L_02000c68:
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0xe666
	.2byte 0x0000
	.2byte 0x0943
	.2byte 0x0000
	.global Func_02000c78
	.thumb_func
Func_02000c78:
	push {lr}
	movs r1, #248
	movs r2, #188
	movs r0, #12
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl 0x0200d6a0
	bl 0x02008b4c
	pop {r0}
	bx r0
	.global Func_02000c90
	.thumb_func
Func_02000c90:
	push {r5, r6, lr}
	ldr r3, [pc, #72]
	lsls r0, r0, #3
	ldr r6, [r3, r0]
	adds r0, #4
	ldr r5, [r3, r0]
	sub sp, #8
	movs r0, #0
	movs r1, #77
	movs r2, #1
	movs r3, #3
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200d5d0
	adds r3, r6, #1
	str r3, [sp, #0]
	movs r0, #1
	movs r1, #77
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200d5d0
	adds r1, r5, #0
	subs r1, #48
	subs r5, #46
	adds r0, r6, #0
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200d5d0
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x0200f714
	.global Func_02000ce0
	.thumb_func
Func_02000ce0:
	push {r5, r6, r7, lr}
	ldr r3, [pc, #100]
	movs r0, #234
	ldr r5, [r3]
	bl 0x0200d608
	movs r6, #1
	negs r6, r6
	cmp r0, r6
	beq .L_02000ce0_0
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r7, [r3, r2]
	ldr r0, [pc, #76]
	adds r5, r7, #0
	subs r5, #40
	bl 0x0200d610
	cmp r0, #0
	beq .L_02000ce0_1
	cmp r5, #4
	beq .L_02000ce0_0
.L_02000ce0_1:
	adds r0, r5, #0
	bl 0x02008c90
	movs r0, #157
	bl 0x0200d7a8
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200d5f0
	adds r0, r6, #0
	adds r1, r6, #0
	ldr r2, [pc, #28]
	bl 0x0200d5f0
	movs r3, #202
	lsls r3, r3, #2
	adds r0, r7, r3
	bl 0x0200d618
.L_02000ce0_0:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00000941
	.4byte 0x0000e666
	.global Func_02000d54
	.thumb_func
Func_02000d54:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r2, [pc, #108]
	lsls r3, r0, #3
	ldr r7, [r2, r3]
	adds r3, #4
	ldr r6, [r2, r3]
	sub sp, #8
	mov r8, r0
	movs r1, #77
	movs r0, #0
	movs r2, #1
	movs r3, #3
	str r7, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200d5d0
	adds r3, r7, #1
	str r3, [sp, #0]
	movs r0, #1
	movs r1, #77
	movs r2, #1
	movs r3, #1
	str r6, [sp, #4]
	bl 0x0200d5d0
	adds r5, r6, #0
	adds r1, r6, #0
	subs r1, #45
	subs r5, #44
	adds r0, r7, #0
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200d5d8
	mov r1, r8
	cmp r1, #1
	bne .L_02000d54_0
	adds r3, r6, #0
	subs r3, #43
	str r3, [sp, #4]
	adds r0, r7, #0
	adds r1, r5, #0
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	bl 0x0200d5d8
.L_02000d54_0:
	sub sp, #-8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200f73c
	.global Func_02000dcc
	.thumb_func
Func_02000dcc:
	push {r5, r6, lr}
	ldr r3, [pc, #84]
	movs r0, #234
	ldr r5, [r3]
	bl 0x0200d608
	movs r6, #1
	negs r6, r6
	cmp r0, r6
	beq .L_02000dcc_0
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r5, [r3, r2]
	adds r0, r5, #0
	subs r0, #40
	bl 0x02008d54
	movs r0, #157
	bl 0x0200d7a8
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200d5f0
	adds r0, r6, #0
	adds r1, r6, #0
	ldr r2, [pc, #24]
	bl 0x0200d5f0
	ldr r3, [pc, #24]
	adds r5, r5, r3
	adds r0, r5, #0
	bl 0x0200d618
.L_02000dcc_0:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x0000e666
	.4byte 0x0000032d
	.global Func_02000e30
	.thumb_func
Func_02000e30:
	push {r5, r6, lr}
	ldr r3, [pc, #72]
	lsls r0, r0, #3
	ldr r6, [r3, r0]
	adds r0, #4
	ldr r5, [r3, r0]
	sub sp, #8
	movs r0, #55
	movs r1, #121
	movs r2, #1
	movs r3, #3
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200d5d0
	adds r3, r6, #1
	str r3, [sp, #0]
	movs r0, #56
	movs r1, #121
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200d5d0
	adds r1, r5, #0
	subs r1, #63
	subs r5, #62
	adds r0, r6, #0
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200d5d0
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x0200f754
	.global Func_02000e80
	.thumb_func
Func_02000e80:
	push {r5, r6, lr}
	ldr r3, [pc, #84]
	movs r0, #234
	ldr r5, [r3]
	bl 0x0200d608
	movs r6, #1
	negs r6, r6
	cmp r0, r6
	beq .L_02000e80_0
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r5, [r3, r2]
	adds r0, r5, #0
	subs r0, #40
	bl 0x02008e30
	movs r0, #157
	bl 0x0200d7a8
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200d5f0
	adds r0, r6, #0
	adds r1, r6, #0
	ldr r2, [pc, #24]
	bl 0x0200d5f0
	movs r3, #204
	lsls r3, r3, #2
	adds r0, r5, r3
	bl 0x0200d618
.L_02000e80_0:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x0000e666
	.global Func_02000ee0
	.thumb_func
Func_02000ee0:
	push {r5, r6, lr}
	ldr r3, [pc, #72]
	lsls r0, r0, #3
	ldr r6, [r3, r0]
	adds r0, #4
	ldr r5, [r3, r0]
	sub sp, #8
	movs r0, #1
	movs r1, #80
	movs r2, #1
	movs r3, #3
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200d5d0
	adds r3, r6, #1
	str r3, [sp, #0]
	movs r0, #2
	movs r1, #80
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200d5d0
	adds r1, r5, #0
	subs r1, #63
	subs r5, #62
	adds r0, r6, #0
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200d5d8
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x0200f764
	.global Func_02000f30
	.thumb_func
Func_02000f30:
	push {r5, r6, lr}
	ldr r3, [pc, #84]
	movs r0, #234
	ldr r5, [r3]
	bl 0x0200d608
	movs r6, #1
	negs r6, r6
	cmp r0, r6
	beq .L_02000f30_0
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r5, [r3, r2]
	adds r0, r5, #0
	subs r0, #40
	bl 0x02008ee0
	movs r0, #157
	bl 0x0200d7a8
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200d5f0
	adds r0, r6, #0
	adds r1, r6, #0
	ldr r2, [pc, #24]
	bl 0x0200d5f0
	ldr r3, [pc, #24]
	adds r5, r5, r3
	adds r0, r5, #0
	bl 0x0200d618
.L_02000f30_0:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x0000e666
	.4byte 0x00000332
	.global Func_02000f94
	.thumb_func
Func_02000f94:
	push {lr}
	movs r0, #234
	bl 0x0200d608
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	bne .L_02000f94_0
	ldr r0, [pc, #8]
	movs r1, #1
	bl 0x0200d600
.L_02000f94_0:
	pop {r0}
	bx r0
	.4byte 0x00000953
	.global Func_02000fb4
	.thumb_func
Func_02000fb4:
	push {lr}
	ldr r0, [pc, #12]
	movs r1, #1
	bl 0x0200d600
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000953
	.global Func_02000fc8
	.thumb_func
Func_02000fc8:
	push {lr}
	movs r0, #8
	movs r1, #8
	bl 0x02009038
	cmp r0, #0
	beq .L_02000fc8_0
	ldr r0, [pc, #8]
	bl 0x0200d618
.L_02000fc8_0:
	pop {r0}
	bx r0
	.4byte 0x00000f2a
	.global Func_02000fe4
	.thumb_func
Func_02000fe4:
	push {lr}
	movs r0, #9
	movs r1, #7
	bl 0x02009038
	cmp r0, #0
	beq .L_02000fe4_0
	ldr r0, [pc, #8]
	bl 0x0200d618
.L_02000fe4_0:
	pop {r0}
	bx r0
	.4byte 0x00000f2b
	.global Func_02001000
	.thumb_func
Func_02001000:
	push {lr}
	movs r0, #10
	movs r1, #6
	bl 0x02009038
	cmp r0, #0
	beq .L_02001000_0
	ldr r0, [pc, #8]
	bl 0x0200d618
.L_02001000_0:
	pop {r0}
	bx r0
	.4byte 0x00000f2c
	.global Func_0200101c
	.thumb_func
Func_0200101c:
	push {lr}
	movs r0, #11
	movs r1, #5
	bl 0x02009038
	cmp r0, #0
	beq .L_0200101c_0
	ldr r0, [pc, #8]
	bl 0x0200d618
.L_0200101c_0:
	pop {r0}
	bx r0
	.4byte 0x00000f2d
	.global Func_02001038
	.thumb_func
Func_02001038:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #0
	adds r5, r1, #0
	adds r6, r0, #0
	mov r8, r3
	bl 0x0200d630
	adds r1, r6, #0
	adds r2, r5, #0
	movs r0, #0
	bl 0x0200d780
	movs r1, #0
	adds r7, r0, #0
	adds r0, r5, #0
	bl 0x0200d640
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	beq .L_02001038_0
	adds r0, r6, #0
	movs r1, #2
	bl 0x0200d6a8
	movs r3, #1
	mov r8, r3
	b .L_02001038_1
.L_02001038_0:
	movs r0, #125
	bl 0x0200d7a8
	adds r0, r6, #0
	movs r1, #5
	bl 0x0200d6a8
.L_02001038_1:
	adds r0, r7, #0
	bl 0x0200d5b0
	bl 0x0200d638
	mov r0, r8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.global Func_02001098
	.thumb_func
Func_02001098:
	bx lr
	.2byte 0x0000
	.global Func_0200109c
	.thumb_func
Func_0200109c:
	bx lr
	.2byte 0x0000
	.global Func_020010a0
	.thumb_func
Func_020010a0:
	bx lr
	.2byte 0x0000
	.global Func_020010a4
	.thumb_func
Func_020010a4:
	bx lr
	.2byte 0x0000
	.global Func_020010a8
	.thumb_func
Func_020010a8:
	push {lr}
	movs r1, #132
	movs r2, #198
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #0
	bl 0x0200d680
	movs r0, #0
	bl 0x0200d698
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #0
	bl 0x0200d708
	movs r0, #10
	bl 0x0200d628
	movs r0, #0
	movs r1, #1
	bl 0x0200d6a8
	movs r0, #234
	movs r1, #3
	bl 0x0200d788
	movs r0, #0
	movs r1, #1
	bl 0x0200d6a8
	movs r1, #0
	movs r0, #234
	bl 0x0200d640
	ldr r0, [pc, #16]
	bl 0x0200d618
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x0200d6a0
	pop {r0}
	bx r0
	.4byte 0x00000f2e
	.global Func_02001108
	.thumb_func
Func_02001108:
	push {lr}
	movs r0, #0
	bl 0x0200d650
	ldr r3, [r0, #16]
	cmp r3, #0
	bge .L_02001108_0
	ldr r2, [pc, #52]
	adds r3, r3, r2
.L_02001108_0:
	ldr r0, [r0, #8]
	asrs r2, r3, #20
	cmp r0, #0
	bge .L_02001108_1
	ldr r3, [pc, #40]
	adds r0, r0, r3
.L_02001108_1:
	subs r3, r2, #5
	asrs r0, r0, #20
	cmp r3, #2
	bhi .L_02001108_2
	cmp r0, #10
	bgt .L_02001108_2
	movs r0, #1
	b .L_02001108_3
.L_02001108_2:
	adds r3, r0, #0
	subs r3, #8
	cmp r3, #1
	bhi .L_02001108_4
	movs r0, #1
	cmp r2, #22
	bgt .L_02001108_3
.L_02001108_4:
	movs r0, #0
.L_02001108_3:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x000fffff
	.global Func_02001150
	.thumb_func
Func_02001150:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #9
	bl 0x0200d650
	adds r7, r0, #0
	movs r0, #10
	bl 0x0200d650
	ldr r2, [pc, #348]
	movs r1, #178
	ldr r3, [r2]
	lsls r1, r1, #1
	adds r6, r3, r1
	ldr r3, [pc, #340]
	ldr r2, [r2, #76]
	ldr r3, [r3]
	mov r8, r2
	movs r2, #1
	ands r3, r2
	mov r10, r0
	cmp r3, #0
	beq .L_02001150_0
	str r2, [r6, #24]
	str r2, [r6, #28]
	b .L_02001150_1
.L_02001150_0:
	movs r3, #1
	negs r3, r3
	str r3, [r6, #24]
	str r3, [r6, #28]
.L_02001150_1:
	movs r0, #131
	lsls r0, r0, #1
	bl 0x0200d610
	cmp r0, #0
	bne .L_02001150_2
	movs r3, #191
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02001150_2
	movs r3, #192
	lsls r3, r3, #1
	add r3, r8
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_02001150_3
.L_02001150_2:
	adds r3, r7, #0
	adds r3, #91
	movs r2, #1
	strb r2, [r3]
	mov r3, r10
	adds r3, #91
	strb r2, [r3]
	b .L_02001150_4
.L_02001150_3:
	movs r0, #133
	lsls r0, r0, #2
	bl 0x0200d610
	cmp r0, #0
	bne .L_02001150_4
	adds r5, r7, #0
	mov r3, r10
	adds r5, #91
	adds r3, #91
	strb r0, [r5]
	strb r0, [r3]
	movs r0, #133
	lsls r0, r0, #2
	bl 0x0200d610
	cmp r0, #0
	bne .L_02001150_5
	ldrb r3, [r5]
	cmp r3, #0
	bne .L_02001150_5
	ldr r0, [r7, #8]
	bl 0x0200daf0
	adds r3, r1, #0
	adds r2, r0, #0
	ldr r0, [pc, #204]
	ldr r1, [pc, #208]
	bl 0x0200da78
	bl 0x0200db6c
	str r0, [r6, #32]
.L_02001150_5:
	bl 0x02009108
	cmp r0, #0
	bne .L_02001150_4
	ldr r3, [pc, #192]
	movs r2, #147
	lsls r2, r2, #2
	adds r5, r3, r2
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, #0
	beq .L_02001150_6
	movs r0, #9
	bl 0x02009918
	cmp r0, #0
	beq .L_02001150_7
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #0
	beq .L_02001150_7
	movs r2, #191
	lsls r2, r2, #1
	ldr r3, [pc, #156]
	add r2, r8
	b .L_02001150_8
.L_02001150_7:
	movs r0, #10
	bl 0x02009918
	cmp r0, #0
	beq .L_02001150_9
	ldr r3, [pc, #136]
	movs r1, #147
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_02001150_6
	movs r2, #191
	lsls r2, r2, #1
	ldr r3, [pc, #120]
	add r2, r8
	b .L_02001150_8
.L_02001150_9:
	ldr r3, [pc, #112]
	movs r1, #147
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02001150_10
.L_02001150_6:
	movs r0, #9
	bl 0x020098e4
	cmp r0, #0
	beq .L_02001150_11
	ldr r0, [pc, #92]
	bl 0x0200d618
	movs r0, #133
	lsls r0, r0, #2
	bl 0x0200d618
.L_02001150_11:
	movs r0, #10
	bl 0x020098e4
	cmp r0, #0
	beq .L_02001150_10
	ldr r0, [pc, #68]
	bl 0x0200d618
	movs r0, #133
	lsls r0, r0, #2
	bl 0x0200d618
.L_02001150_10:
	movs r0, #133
	lsls r0, r0, #2
	bl 0x0200d610
	cmp r0, #0
	beq .L_02001150_4
	movs r2, #193
	lsls r2, r2, #1
	add r2, r8
	movs r3, #91
.L_02001150_8:
	strh r3, [r2]
.L_02001150_4:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x03001e70
	.4byte 0x03001e40
	.4byte 0x41610000
	.4byte 0x00000000
	.4byte 0x02000240
	.4byte 0x00002092
	.4byte 0x00000215
	.global Func_020012e0
	.thumb_func
Func_020012e0:
	push {lr}
	bl 0x0200d630
	movs r0, #9
	bl 0x0200d670
	movs r2, #0
	movs r0, #9
	movs r1, #0
	bl 0x0200d690
	movs r0, #9
	movs r1, #0
	bl 0x0200d6a8
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200d6d0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #9
	bl 0x0200d718
	movs r0, #10
	bl 0x02009b24
	bl 0x0200d638
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001324
	.thumb_func
Func_02001324:
	push {r5, lr}
	movs r0, #0
	bl 0x0200d650
	adds r5, r0, #0
	movs r0, #17
	bl 0x0200d650
	ldr r3, [r5, #8]
	cmp r3, #0
	bge .L_02001324_0
	ldr r1, [pc, #76]
	adds r3, r3, r1
.L_02001324_0:
	asrs r2, r3, #20
	ldr r3, [r0, #16]
	cmp r3, #0
	bge .L_02001324_1
	ldr r1, [pc, #64]
	adds r3, r3, r1
.L_02001324_1:
	ldr r0, [r0, #8]
	asrs r3, r3, #20
	cmp r0, #0
	bge .L_02001324_2
	ldr r1, [pc, #52]
	adds r0, r0, r1
.L_02001324_2:
	asrs r0, r0, #20
	cmp r2, #52
	bne .L_02001324_3
	cmp r0, #57
	bne .L_02001324_3
	cmp r3, #34
	ble .L_02001324_3
	cmp r3, #40
	bgt .L_02001324_3
	movs r0, #1
	b .L_02001324_4
.L_02001324_3:
	cmp r2, #57
	bne .L_02001324_5
	cmp r0, #52
	bne .L_02001324_5
	cmp r3, #34
	ble .L_02001324_5
	movs r0, #1
	cmp r3, #40
	ble .L_02001324_4
.L_02001324_5:
	movs r0, #0
.L_02001324_4:
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x000fffff
	.global Func_0200138c
	.thumb_func
Func_0200138c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #17
	bl 0x0200d650
	ldr r2, [pc, #268]
	movs r1, #178
	ldr r3, [r2]
	ldr r2, [r2, #76]
	lsls r1, r1, #1
	adds r6, r3, r1
	mov r8, r2
	adds r7, r0, #0
	bl 0x0200d750
	ldr r3, [pc, #252]
	ldr r3, [r3]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200138c_0
	str r2, [r6, #24]
	str r2, [r6, #28]
	b .L_0200138c_1
.L_0200138c_0:
	movs r3, #1
	negs r3, r3
	str r3, [r6, #24]
	str r3, [r6, #28]
.L_0200138c_1:
	movs r0, #131
	lsls r0, r0, #1
	bl 0x0200d610
	cmp r0, #0
	bne .L_0200138c_2
	movs r3, #191
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_0200138c_2
	movs r3, #192
	lsls r3, r3, #1
	add r3, r8
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_0200138c_3
.L_0200138c_2:
	adds r2, r7, #0
	adds r2, #91
	movs r3, #1
	strb r3, [r2]
	b .L_0200138c_4
.L_0200138c_3:
	movs r0, #133
	lsls r0, r0, #2
	bl 0x0200d610
	cmp r0, #0
	bne .L_0200138c_4
	adds r5, r7, #0
	adds r5, #91
	strb r0, [r5]
	movs r0, #133
	lsls r0, r0, #2
	bl 0x0200d610
	cmp r0, #0
	bne .L_0200138c_5
	ldrb r3, [r5]
	cmp r3, #0
	bne .L_0200138c_5
	ldr r2, [r7, #8]
	movs r3, #208
	lsls r3, r3, #18
	subs r3, r3, r2
	str r3, [r6, #32]
	ldr r2, [r7, #16]
	movs r3, #144
	lsls r3, r3, #18
	subs r3, r3, r2
	str r3, [r6, #36]
.L_0200138c_5:
	bl 0x02009324
	cmp r0, #0
	bne .L_0200138c_4
	movs r0, #17
	bl 0x02009980
	movs r0, #17
	bl 0x02009918
	cmp r0, #0
	beq .L_0200138c_6
	ldr r3, [pc, #100]
	movs r2, #147
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_0200138c_7
	subs r2, #206
	ldr r3, [pc, #88]
	add r2, r8
	b .L_0200138c_8
.L_0200138c_6:
	ldr r3, [pc, #76]
	movs r2, #147
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200138c_9
.L_0200138c_7:
	movs r0, #17
	bl 0x020098e4
	cmp r0, #0
	beq .L_0200138c_9
	ldr r0, [pc, #60]
	bl 0x0200d618
	movs r0, #133
	lsls r0, r0, #2
	bl 0x0200d618
.L_0200138c_9:
	movs r0, #133
	lsls r0, r0, #2
	bl 0x0200d610
	cmp r0, #0
	beq .L_0200138c_4
	movs r2, #193
	lsls r2, r2, #1
	add r2, r8
	movs r3, #92
.L_0200138c_8:
	strh r3, [r2]
.L_0200138c_4:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x03001e70
	.4byte 0x03001e40
	.4byte 0x02000240
	.4byte 0x00002092
	.4byte 0x00000215
	.global Func_020014bc
	.thumb_func
Func_020014bc:
	push {lr}
	movs r0, #17
	bl 0x02009b24
	bl 0x0200d638
	pop {r0}
	bx r0
	.global Func_020014cc
	.thumb_func
Func_020014cc:
	push {lr}
	movs r0, #0
	bl 0x0200d650
	ldr r3, [r0, #16]
	cmp r3, #0
	bge .L_020014cc_0
	ldr r2, [pc, #72]
	adds r3, r3, r2
.L_020014cc_0:
	ldr r0, [r0, #8]
	asrs r2, r3, #20
	cmp r0, #0
	bge .L_020014cc_1
	ldr r3, [pc, #60]
	adds r0, r0, r3
.L_020014cc_1:
	asrs r0, r0, #20
	adds r3, r0, #0
	subs r3, #41
	cmp r3, #3
	bhi .L_020014cc_2
	cmp r2, #25
	ble .L_020014cc_2
	cmp r2, #28
	ble .L_020014cc_3
.L_020014cc_2:
	cmp r0, #41
	bne .L_020014cc_4
	cmp r2, #37
	ble .L_020014cc_4
	cmp r2, #41
	bgt .L_020014cc_4
.L_020014cc_3:
	movs r0, #1
	b .L_020014cc_5
.L_020014cc_4:
	adds r3, r0, #0
	subs r3, #54
	cmp r3, #2
	bhi .L_020014cc_6
	cmp r2, #30
	ble .L_020014cc_6
	movs r0, #1
	cmp r2, #40
	ble .L_020014cc_5
.L_020014cc_6:
	movs r0, #0
.L_020014cc_5:
	pop {r1}
	bx r1
	.4byte 0x000fffff
	.global Func_02001528
	.thumb_func
Func_02001528:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #18
	bl 0x0200d650
	ldr r2, [pc, #260]
	movs r1, #178
	ldr r3, [r2]
	lsls r1, r1, #1
	adds r6, r3, r1
	ldr r3, [pc, #256]
	ldr r2, [r2, #76]
	ldr r3, [r3]
	mov r8, r2
	movs r2, #1
	ands r3, r2
	adds r7, r0, #0
	cmp r3, #0
	beq .L_02001528_0
	str r2, [r6, #24]
	str r2, [r6, #28]
	b .L_02001528_1
.L_02001528_0:
	movs r3, #1
	negs r3, r3
	str r3, [r6, #24]
	str r3, [r6, #28]
.L_02001528_1:
	movs r0, #131
	lsls r0, r0, #1
	bl 0x0200d610
	cmp r0, #0
	bne .L_02001528_2
	movs r3, #191
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02001528_2
	movs r3, #192
	lsls r3, r3, #1
	add r3, r8
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_02001528_3
.L_02001528_2:
	adds r2, r7, #0
	adds r2, #91
	movs r3, #1
	strb r3, [r2]
	b .L_02001528_4
.L_02001528_3:
	movs r0, #133
	lsls r0, r0, #2
	bl 0x0200d610
	cmp r0, #0
	bne .L_02001528_4
	adds r5, r7, #0
	adds r5, #91
	strb r0, [r5]
	movs r0, #133
	lsls r0, r0, #2
	bl 0x0200d610
	cmp r0, #0
	bne .L_02001528_5
	ldrb r3, [r5]
	cmp r3, #0
	bne .L_02001528_5
	ldr r2, [r7, #8]
	movs r3, #188
	lsls r3, r3, #18
	subs r3, r3, r2
	str r3, [r6, #32]
	ldr r2, [r7, #16]
	movs r3, #248
	lsls r3, r3, #17
	subs r3, r3, r2
	str r3, [r6, #36]
.L_02001528_5:
	bl 0x020094cc
	cmp r0, #0
	bne .L_02001528_4
	movs r0, #18
	bl 0x02009918
	cmp r0, #0
	beq .L_02001528_6
	ldr r3, [pc, #104]
	movs r2, #147
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_02001528_7
	subs r2, #206
	ldr r3, [pc, #88]
	add r2, r8
	b .L_02001528_8
.L_02001528_6:
	ldr r3, [pc, #80]
	movs r2, #147
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_02001528_9
.L_02001528_7:
	movs r0, #18
	bl 0x020098e4
	cmp r0, #0
	beq .L_02001528_9
	ldr r0, [pc, #60]
	bl 0x0200d618
	movs r0, #133
	lsls r0, r0, #2
	bl 0x0200d618
.L_02001528_9:
	movs r0, #133
	lsls r0, r0, #2
	bl 0x0200d610
	cmp r0, #0
	beq .L_02001528_4
	movs r2, #193
	lsls r2, r2, #1
	add r2, r8
	movs r3, #93
.L_02001528_8:
	strh r3, [r2]
.L_02001528_4:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001e70
	.4byte 0x03001e40
	.4byte 0x02000240
	.4byte 0x00002092
	.4byte 0x00000215
	.global Func_02001650
	.thumb_func
Func_02001650:
	push {lr}
	movs r0, #18
	bl 0x02009b24
	bl 0x0200d638
	pop {r0}
	bx r0
	.global Func_02001660
	.thumb_func
Func_02001660:
	push {lr}
	movs r0, #0
	bl 0x0200d650
	ldr r3, [r0, #16]
	cmp r3, #0
	bge .L_02001660_0
	ldr r2, [pc, #40]
	adds r3, r3, r2
.L_02001660_0:
	ldr r0, [r0, #8]
	asrs r3, r3, #20
	cmp r0, #0
	bge .L_02001660_1
	ldr r2, [pc, #28]
	adds r0, r0, r2
.L_02001660_1:
	asrs r0, r0, #20
	cmp r0, #45
	ble .L_02001660_2
	cmp r3, #14
	ble .L_02001660_2
	cmp r0, #64
	bgt .L_02001660_2
	movs r0, #0
	cmp r3, #16
	ble .L_02001660_3
.L_02001660_2:
	movs r0, #1
.L_02001660_3:
	pop {r1}
	bx r1
	.4byte 0x000fffff
	.global Func_0200169c
	.thumb_func
Func_0200169c:
	push {r5, lr}
	ldr r3, [pc, #92]
	movs r0, #133
	lsls r0, r0, #2
	ldr r5, [r3]
	bl 0x0200d610
	cmp r0, #0
	bne .L_0200169c_0
	bl 0x02009660
	cmp r0, #0
	bne .L_0200169c_0
	ldr r3, [pc, #72]
	movs r2, #147
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_0200169c_1
	movs r0, #17
	bl 0x020098e4
	cmp r0, #0
	beq .L_0200169c_1
	ldr r0, [pc, #48]
	bl 0x0200d618
	movs r0, #133
	lsls r0, r0, #2
	bl 0x0200d618
.L_0200169c_1:
	movs r0, #133
	lsls r0, r0, #2
	bl 0x0200d610
	cmp r0, #0
	beq .L_0200169c_0
	movs r3, #193
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #94
	strh r3, [r2]
.L_0200169c_0:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x00000215
	.global Func_02001708
	.thumb_func
Func_02001708:
	push {lr}
	movs r0, #17
	bl 0x02009b24
	bl 0x0200d638
	pop {r0}
	bx r0
	.global Func_02001718
	.thumb_func
Func_02001718:
	push {r5, lr}
	ldr r3, [pc, #52]
	movs r0, #12
	ldr r5, [r3]
	bl 0x020098e4
	cmp r0, #0
	beq 0x02009748
.L_02001728:
	ldr r3, [pc, #40]
	movs r2, #147
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02001728_0
	ldr r0, [pc, #28]
	bl 0x0200d598
	movs r3, #193
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #95
	strh r3, [r2]
.L_02001728_0:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.4byte 0x02000240
	.4byte 0x02009719
	.global Func_0200175c
	.thumb_func
Func_0200175c:
	push {r5, lr}
	ldr r3, [pc, #72]
	ldr r0, [pc, #72]
	ldr r5, [r3]
	bl 0x0200d610
	cmp r0, #0
	bne .L_0200175c_0
	movs r0, #13
	bl 0x020098e4
	cmp r0, #0
	beq .L_0200175c_0
	ldr r3, [pc, #56]
	movs r2, #147
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_0200175c_0
	ldr r0, [pc, #36]
	bl 0x0200d618
	ldr r0, [pc, #36]
	bl 0x0200d598
	ldr r0, [pc, #36]
	bl 0x0200d598
	movs r3, #193
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #96
	strh r3, [r2]
.L_0200175c_0:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x00000225
	.4byte 0x02000240
	.4byte 0x0200975d
	.4byte 0x020097bd
	.global Func_020017bc
	.thumb_func
Func_020017bc:
	push {r5, lr}
	ldr r3, [pc, #72]
	ldr r0, [pc, #72]
	ldr r5, [r3]
	bl 0x0200d610
	cmp r0, #0
	bne 0x02009802
	movs r0, #21
	bl 0x020098e4
	cmp r0, #0
	beq 0x02009802
.L_020017d6:
	ldr r3, [pc, #56]
	movs r2, #147
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_020017d6_0
	ldr r0, [pc, #36]
	bl 0x0200d618
	ldr r0, [pc, #36]
	bl 0x0200d598
	ldr r0, [pc, #36]
	bl 0x0200d598
	movs r3, #193
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #96
	strh r3, [r2]
.L_020017d6_0:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x1ebc
	.2byte 0x0300
	.4byte 0x00000225
	.4byte 0x02000240
	.4byte 0x020097bd
	.4byte 0x0200975d
	.global Func_0200181c
	.thumb_func
Func_0200181c:
	push {r5, r6, lr}
	bl 0x0200d650
	adds r5, r0, #0
	movs r0, #0
	bl 0x0200d650
	ldr r3, [r5, #16]
	adds r1, r0, #0
	cmp r3, #0
	bge .L_0200181c_0
	ldr r2, [pc, #72]
	adds r3, r3, r2
.L_0200181c_0:
	ldr r0, [r5, #8]
	asrs r6, r3, #20
	cmp r0, #0
	bge .L_0200181c_1
	ldr r3, [pc, #60]
	adds r0, r0, r3
.L_0200181c_1:
	ldr r2, [r1, #16]
	asrs r4, r0, #20
	cmp r2, #0
	bge .L_0200181c_2
	ldr r3, [pc, #48]
	adds r2, r2, r3
.L_0200181c_2:
	ldr r0, [r1, #8]
	asrs r3, r2, #20
	cmp r0, #0
	bge .L_0200181c_3
	ldr r2, [pc, #36]
	adds r0, r0, r2
.L_0200181c_3:
	subs r3, r6, r3
	adds r3, #6
	asrs r0, r0, #20
	cmp r3, #12
	bhi .L_0200181c_4
	subs r3, r4, #1
	cmp r3, r0
	bge .L_0200181c_4
	adds r3, r4, #1
	cmp r3, r0
	ble .L_0200181c_4
	movs r0, #1
	b .L_0200181c_5
.L_0200181c_4:
	movs r0, #0
.L_0200181c_5:
	pop {r5, r6}
	pop {r1}
	bx r1
	.4byte 0x000fffff
	.global Func_02001880
	.thumb_func
Func_02001880:
	push {r5, r6, lr}
	bl 0x0200d650
	adds r5, r0, #0
	movs r0, #0
	bl 0x0200d650
	ldr r3, [r5, #16]
	adds r1, r0, #0
	cmp r3, #0
	bge .L_02001880_0
	ldr r2, [pc, #72]
	adds r3, r3, r2
.L_02001880_0:
	ldr r0, [r5, #8]
	asrs r6, r3, #20
	cmp r0, #0
	bge .L_02001880_1
	ldr r3, [pc, #60]
	adds r0, r0, r3
.L_02001880_1:
	ldr r2, [r1, #16]
	asrs r4, r0, #20
	cmp r2, #0
	bge .L_02001880_2
	ldr r3, [pc, #48]
	adds r2, r2, r3
.L_02001880_2:
	ldr r0, [r1, #8]
	asrs r2, r2, #20
	cmp r0, #0
	bge .L_02001880_3
	ldr r3, [pc, #36]
	adds r0, r0, r3
.L_02001880_3:
	asrs r3, r0, #20
	subs r3, r4, r3
	adds r3, #6
	movs r0, #0
	cmp r3, #12
	bhi .L_02001880_4
	subs r3, r6, #2
	cmp r3, r2
	bge .L_02001880_5
	adds r3, r6, #2
	movs r0, #1
	cmp r3, r2
	bgt .L_02001880_4
.L_02001880_5:
	movs r0, #0
.L_02001880_4:
	pop {r5, r6}
	pop {r1}
	bx r1
	.4byte 0x000fffff
	.global Func_020018e4
	.thumb_func
Func_020018e4:
	push {r5, lr}
	adds r5, r0, #0
	bl 0x02009980
	cmp r0, #0
	bne .L_020018e4_0
	movs r0, #0
	b .L_020018e4_1
.L_020018e4_0:
	adds r0, r5, #0
	bl 0x0200981c
	cmp r0, #0
	beq .L_020018e4_2
	movs r0, #1
	b .L_020018e4_1
.L_020018e4_2:
	adds r0, r5, #0
	bl 0x02009880
	adds r3, r0, #0
	negs r0, r3
	orrs r0, r3
	lsrs r0, r0, #31
.L_020018e4_1:
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_02001918
	.thumb_func
Func_02001918:
	push {r5, lr}
	bl 0x0200d650
	adds r5, r0, #0
	movs r0, #0
	bl 0x0200d650
	ldr r2, [r5, #16]
	adds r3, r0, #0
	cmp r2, #0
	bge .L_02001918_0
	ldr r0, [pc, #76]
	adds r2, r2, r0
.L_02001918_0:
	ldr r0, [r5, #8]
	asrs r1, r2, #20
	cmp r0, #0
	bge .L_02001918_1
	ldr r2, [pc, #64]
	adds r0, r0, r2
.L_02001918_1:
	ldr r2, [r3, #16]
	asrs r4, r0, #20
	cmp r2, #0
	bge .L_02001918_2
	ldr r0, [pc, #52]
	adds r2, r2, r0
.L_02001918_2:
	ldr r0, [r3, #8]
	asrs r2, r2, #20
	cmp r0, #0
	bge .L_02001918_3
	ldr r3, [pc, #40]
	adds r0, r0, r3
.L_02001918_3:
	asrs r3, r0, #20
	subs r3, r4, r3
	adds r1, #1
	cmp r3, #0
	bge .L_02001918_4
	negs r3, r3
.L_02001918_4:
	subs r0, r1, r2
	cmp r0, #0
	bge .L_02001918_5
	negs r0, r0
.L_02001918_5:
	adds r3, r3, r0
	movs r0, #1
	cmp r3, #4
	ble .L_02001918_6
	movs r0, #0
.L_02001918_6:
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x000fffff
	.global Func_02001980
	.thumb_func
Func_02001980:
	push {r5, lr}
	bl 0x0200d650
	adds r5, r0, #0
	bl 0x0200d750
	ldr r3, [r5, #8]
	adds r2, r0, #0
	cmp r3, #0
	bge .L_02001980_0
	ldr r0, [pc, #76]
	adds r3, r3, r0
.L_02001980_0:
	ldr r0, [r5, #16]
	asrs r4, r3, #20
	cmp r0, #0
	bge 0x020099a4
	ldr r1, [pc, #64]
	adds r0, r0, r1
.L_020019a4:
	ldr r3, [r2, #8]
	asrs r1, r0, #20
	cmp r3, #0
	bge .L_020019a4_0
	ldr r0, [pc, #52]
	adds r3, r3, r0
.L_020019a4_0:
	ldr r0, [r2, #16]
	asrs r3, r3, #20
	cmp r0, #0
	bge .L_020019a4_1
	ldr r2, [pc, #40]
	adds r0, r0, r2
.L_020019a4_1:
	subs r3, r4, r3
	asrs r0, r0, #20
	cmp r3, #0
	bge .L_020019a4_2
	negs r3, r3
.L_020019a4_2:
	subs r0, r1, r0
	cmp r0, #0
	bge .L_020019a4_3
	negs r0, r0
.L_020019a4_3:
	cmp r3, #7
	bgt .L_020019a4_4
	cmp r0, #5
	ble .L_020019a4_5
.L_020019a4_4:
	movs r0, #0
	b .L_020019a4_6
.L_020019a4_5:
	movs r0, #1
.L_020019a4_6:
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x000fffff
	.global Func_020019e8
	.thumb_func
Func_020019e8:
	push {r5, lr}
	movs r0, #0
	bl 0x0200d650
	adds r5, r0, #0
	ldr r0, [pc, #68]
	bl 0x0200d610
	cmp r0, #0
	bne .L_020019e8_0
	ldr r3, [r5, #8]
	cmp r3, #0
	bge .L_020019e8_1
	ldr r2, [pc, #56]
	adds r3, r3, r2
.L_020019e8_1:
	ldr r0, [r5, #16]
	asrs r3, r3, #20
	cmp r0, #0
	bge .L_020019e8_2
	ldr r2, [pc, #44]
	adds r0, r0, r2
.L_020019e8_2:
	asrs r0, r0, #20
	cmp r3, #43
	bne .L_020019e8_0
	cmp r0, #28
	ble .L_020019e8_0
	cmp r0, #31
	bgt .L_020019e8_0
	ldr r3, [pc, #28]
	movs r2, #182
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #41
	strh r2, [r3]
	bl 0x02008f30
.L_020019e8_0:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x0000035b
	.4byte 0x000fffff
	.4byte 0x03001ebc
	.global Func_02001a44
	.thumb_func
Func_02001a44:
	push {r5, lr}
	movs r0, #0
	bl 0x0200d650
	adds r5, r0, #0
	movs r0, #214
	lsls r0, r0, #2
	bl 0x0200d610
	cmp r0, #0
	bne .L_02001a44_0
	ldr r3, [r5, #8]
	cmp r3, #0
	bge .L_02001a44_1
	ldr r1, [pc, #68]
	adds r3, r3, r1
.L_02001a44_1:
	asrs r2, r3, #20
	ldr r3, [r5, #16]
	cmp r3, #0
	bge .L_02001a44_2
	ldr r1, [pc, #56]
	adds r3, r3, r1
.L_02001a44_2:
	asrs r3, r3, #20
	cmp r2, #16
	bne .L_02001a44_0
	cmp r3, #55
	ble .L_02001a44_0
	cmp r3, #58
	bgt .L_02001a44_0
	movs r2, #192
	ldrh r0, [r5, #6]
	lsls r2, r2, #8
	cmp r0, r2
	beq .L_02001a44_3
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bne .L_02001a44_0
.L_02001a44_3:
	ldr r3, [pc, #24]
	movs r1, #182
	ldr r3, [r3]
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #40
	strh r2, [r3]
	bl 0x02008e80
.L_02001a44_0:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x000fffff
	.4byte 0x03001ebc
	.global Func_02001ab0
	.thumb_func
Func_02001ab0:
	push {r5, lr}
	bl 0x0200d630
	movs r2, #0
	movs r0, #9
	movs r1, #0
	bl 0x0200d690
	movs r1, #1
	movs r0, #9
	bl 0x0200d660
	movs r0, #9
	bl 0x0200d670
	movs r0, #9
	movs r1, #0
	bl 0x0200d6a8
	movs r1, #1
	movs r0, #0
	bl 0x0200d660
	ldr r5, [pc, #64]
	adds r0, r5, #0
	bl 0x0200d6e8
	movs r0, #9
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #129
	movs r2, #60
	lsls r1, r1, #1
	movs r0, #0
	adds r5, #1
	bl 0x0200d718
	adds r0, r5, #0
	bl 0x0200d6e8
	movs r1, #0
	movs r0, #9
	bl 0x0200d6f8
	movs r0, #60
	bl 0x0200d758
	bl 0x0200d778
	bl 0x0200d638
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000240d
	.global Func_02001b24
	.thumb_func
Func_02001b24:
	push {r5, r6, lr}
	adds r5, r0, #0
	bl 0x0200d630
	bl 0x0200d630
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #1
	movs r2, #1
	bl 0x0200d718
	movs r2, #0
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200d690
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200d660
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200d6a8
	adds r0, r5, #0
	movs r2, #0
	movs r1, #0
	bl 0x0200d6d0
	movs r0, #0
	movs r1, #1
	bl 0x0200d6a8
	movs r2, #0
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200d690
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200d660
	adds r0, r5, #0
	bl 0x0200d670
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200d6a8
	movs r1, #1
	movs r0, #0
	bl 0x0200d660
	ldr r6, [pc, #76]
	adds r0, r6, #0
	bl 0x0200d6e8
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200d6f8
	adds r1, r5, #0
	movs r0, #0
	movs r2, #0
	bl 0x0200d6d0
	movs r1, #129
	movs r2, #60
	lsls r1, r1, #1
	movs r0, #0
	adds r6, #1
	bl 0x0200d718
	adds r0, r6, #0
	bl 0x0200d6e8
	movs r1, #0
	adds r0, r5, #0
	bl 0x0200d6f8
	bl 0x0200d778
	movs r0, #60
	bl 0x0200d628
	movs r0, #60
	bl 0x0200d758
	bl 0x0200d638
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x0000240d
	.global Func_02001be4
	.thumb_func
Func_02001be4:
	push {r5, lr}
	adds r5, r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200d6d0
	movs r2, #0
	movs r0, #0
	adds r1, r5, #0
	bl 0x0200d6d0
	ldr r3, [pc, #76]
	ldr r2, [r3]
	movs r3, #3
	ands r2, r3
	cmp r2, #1
	beq .L_02001be4_0
	cmp r2, #1
	bgt .L_02001be4_1
	cmp r2, #0
	beq .L_02001be4_2
	b .L_02001be4_3
.L_02001be4_1:
	cmp r2, #2
	beq .L_02001be4_4
	cmp r2, #3
	beq .L_02001be4_5
	b .L_02001be4_3
.L_02001be4_2:
	adds r0, r5, #0
	bl 0x02009c4c
	b .L_02001be4_6
.L_02001be4_0:
	adds r0, r5, #0
	bl 0x02009ca4
	b .L_02001be4_6
.L_02001be4_4:
	adds r0, r5, #0
	bl 0x02009cf0
	b .L_02001be4_6
.L_02001be4_5:
	adds r0, r5, #0
	bl 0x02009d60
	b .L_02001be4_6
.L_02001be4_3:
	adds r0, r5, #0
	bl 0x02009ca4
.L_02001be4_6:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200dfa4
	.global Func_02001c4c
	.thumb_func
Func_02001c4c:
	push {r5, r6, lr}
	movs r1, #1
	adds r6, r0, #0
	bl 0x0200d6c8
	ldr r5, [pc, #72]
	adds r0, r5, #0
	bl 0x0200d6e8
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #129
	movs r2, #60
	adds r0, r6, #0
	lsls r1, r1, #1
	bl 0x0200d718
	adds r0, r5, #1
	bl 0x0200d6e8
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200d6f8
	adds r5, #2
	adds r0, r6, #0
	movs r1, #4
	bl 0x0200d6b0
	adds r0, r5, #0
	bl 0x0200d6e8
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200d6f8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000241e
	.global Func_02001ca4
	.thumb_func
Func_02001ca4:
	push {r5, r6, lr}
	ldr r5, [pc, #68]
	adds r6, r0, #0
	adds r0, r5, #0
	bl 0x0200d6e8
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200d6f8
	adds r0, r6, #0
	movs r1, #1
	bl 0x0200d6c8
	adds r0, r5, #1
	bl 0x0200d6e8
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200d6f8
	adds r5, #2
	adds r0, r6, #0
	movs r1, #4
.L_02001cd4:
	bl 0x0200d6b0
	adds r0, r5, #0
	bl 0x0200d6e8
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200d6f8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x2421
	.2byte 0x0000
	.global Func_02001cf0
	.thumb_func
Func_02001cf0:
	push {r5, r6, lr}
	ldr r6, [pc, #100]
	adds r5, r0, #0
	adds r0, r6, #0
	bl 0x0200d6e8
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200d6f8
	movs r0, #120
	bl 0x0200d628
	movs r2, #60
	adds r0, r5, #0
	ldr r1, [pc, #76]
	bl 0x0200d718
	adds r0, r6, #1
	bl 0x0200d6e8
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200d6f8
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200d6c8
	adds r0, r6, #2
	bl 0x0200d6e8
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200d6f8
	adds r6, #3
	adds r0, r5, #0
	movs r1, #4
	bl 0x0200d6b0
	adds r0, r6, #0
	bl 0x0200d6e8
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200d6f8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00002424
	.4byte 0x00000101
	.global Func_02001d60
	.thumb_func
Func_02001d60:
	push {r5, r6, lr}
	ldr r6, [pc, #92]
	adds r5, r0, #0
	adds r0, r6, #0
	bl 0x0200d6e8
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200d6f8
	adds r0, r5, #0
	movs r1, #4
	bl 0x0200d6b0
	adds r0, r6, #1
	bl 0x0200d6e8
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200d6f8
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200d6c8
	adds r0, r6, #2
	bl 0x0200d6e8
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200d6f8
	adds r6, #3
	adds r0, r5, #0
	movs r1, #3
	bl 0x0200d6b0
	adds r0, r6, #0
	bl 0x0200d6e8
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200d6f8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00002428
	.global Func_02001dc4
	.thumb_func
Func_02001dc4:
	push {lr}
	movs r1, #1
	movs r0, #0
	bl 0x0200d6a8
	movs r0, #113
	bl 0x0200d7a8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #60
	movs r0, #15
	bl 0x0200d718
	movs r0, #15
	bl 0x02009be4
	ldr r3, [pc, #56]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #64
	str r2, [r3]
	ldr r3, [pc, #44]
	adds r2, #43
	adds r3, r3, r2
	movs r2, #3
	strb r2, [r3]
	movs r0, #98
	movs r1, #2
	bl 0x0200d760
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl 0x0200d6a0
	bl 0x0200d638
	ldr r0, [pc, #16]
	bl 0x0200d618
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x0000094c
	.global Func_02001e2c
	.thumb_func
Func_02001e2c:
	push {lr}
	movs r1, #1
	movs r0, #0
	bl 0x0200d6a8
	movs r0, #113
	bl 0x0200d7a8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #60
	movs r0, #11
	bl 0x0200d718
	movs r0, #11
	bl 0x02009be4
	ldr r3, [pc, #56]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #64
	str r2, [r3]
	ldr r3, [pc, #44]
	adds r2, #43
	adds r3, r3, r2
	movs r2, #3
	strb r2, [r3]
	movs r0, #98
	movs r1, #2
	bl 0x0200d760
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl 0x0200d6a0
	bl 0x0200d638
	ldr r0, [pc, #16]
	bl 0x0200d618
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x00000949
	.global Func_02001e94
	.thumb_func
Func_02001e94:
	push {r5, lr}
	movs r0, #0
	movs r1, #1
	bl 0x0200d6a8
	movs r0, #12
	movs r1, #1
	bl 0x0200d6a8
	movs r0, #13
	movs r1, #1
	bl 0x0200d6a8
	movs r1, #1
	movs r0, #14
	bl 0x0200d6a8
	movs r0, #113
	bl 0x0200d7a8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #12
	bl 0x0200d718
	movs r0, #30
	bl 0x0200d628
	movs r2, #0
	movs r1, #0
	movs r0, #12
	bl 0x0200d6d0
	ldr r5, [pc, #376]
	adds r0, r5, #0
	bl 0x0200d6e8
	movs r0, #12
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #128
	movs r0, #13
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200d718
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200d718
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl 0x0200d6d0
	movs r1, #13
	movs r2, #0
	movs r0, #0
	bl 0x0200d6d0
	movs r0, #65
	bl 0x0200d628
	movs r1, #160
	movs r0, #13
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200d708
	movs r1, #208
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #14
	bl 0x0200d708
	adds r0, r5, #1
	bl 0x0200d6e8
	movs r0, #13
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #3
	movs r0, #14
	bl 0x0200d6b0
	adds r0, r5, #2
	bl 0x0200d6e8
	movs r1, #0
	movs r0, #14
	bl 0x0200d6f8
	adds r0, r5, #3
	bl 0x0200d6e8
	movs r0, #12
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #1
	movs r0, #13
	bl 0x0200d6c8
	adds r0, r5, #4
	bl 0x0200d6e8
	movs r0, #13
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #3
	movs r0, #14
	bl 0x0200d6b0
	adds r0, r5, #5
	bl 0x0200d6e8
	movs r0, #14
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #3
	movs r0, #14
	bl 0x0200d6b0
	movs r0, #60
	bl 0x0200d628
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl 0x0200d6d0
	movs r1, #0
	movs r2, #0
	movs r0, #14
	bl 0x0200d6d0
	movs r0, #70
	bl 0x0200d628
	movs r1, #168
	lsls r1, r1, #2
	movs r2, #88
	movs r0, #12
	bl 0x0200d680
	movs r0, #12
	bl 0x0200d698
	movs r2, #0
	movs r0, #12
	movs r1, #0
	bl 0x0200d6d0
	movs r1, #3
	movs r0, #12
	bl 0x0200d6b0
	adds r5, #6
	movs r0, #30
	bl 0x0200d628
	adds r0, r5, #0
	bl 0x0200d6e8
	movs r0, #12
	movs r1, #0
	bl 0x0200d6f8
	ldr r3, [pc, #88]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #64
.L_02002008:
	str r2, [r3]
	ldr r0, [pc, #80]
	movs r1, #31
	bl 0x0200d768
	ldr r3, [pc, #76]
	ldr r2, [pc, #76]
	adds r3, r3, r2
	movs r2, #3
	strb r2, [r3]
	movs r0, #98
	movs r1, #3
	bl 0x0200d760
.L_02002024:
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl 0x0200d6a0
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl 0x0200d6a0
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl 0x0200d6a0
	bl 0x0200d638
	ldr r0, [pc, #32]
	bl 0x0200d618
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x2438
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x00a1
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	.2byte 0x022b
	.2byte 0x0000
	.4byte 0x0000094a
	.global Func_0200206c
	.thumb_func
Func_0200206c:
	push {lr}
	bl 0x0200d630
	movs r1, #172
	movs r2, #176
	movs r0, #12
	lsls r1, r1, #18
	lsls r2, r2, #15
	bl 0x0200d6a0
	movs r1, #176
	movs r2, #176
	movs r0, #13
	lsls r1, r1, #18
	lsls r2, r2, #15
	bl 0x0200d6a0
	movs r1, #180
	movs r2, #192
	lsls r2, r2, #15
	movs r0, #14
	lsls r1, r1, #18
	bl 0x0200d6a0
	movs r0, #12
	movs r1, #5
	bl 0x0200d6a8
	movs r0, #13
	movs r1, #5
	bl 0x0200d6a8
	movs r0, #14
	movs r1, #5
	bl 0x0200d6a8
	movs r0, #0
	movs r1, #13
	movs r2, #0
	bl 0x0200d6d0
	bl 0x0200d638
	bl 0x0200d770
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020020cc
	.thumb_func
Func_020020cc:
	push {lr}
	movs r1, #1
	movs r0, #0
	bl 0x0200d6a8
	movs r0, #113
	bl 0x0200d7a8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #60
	movs r0, #16
	bl 0x0200d718
	movs r0, #16
	bl 0x02009be4
	ldr r3, [pc, #56]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #64
	str r2, [r3]
	ldr r3, [pc, #44]
	adds r2, #43
	adds r3, r3, r2
	movs r2, #3
	strb r2, [r3]
	movs r0, #98
	movs r1, #2
	bl 0x0200d760
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl 0x0200d6a0
	bl 0x0200d638
	ldr r0, [pc, #16]
	bl 0x0200d618
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x0000094b
	.global Func_02002134
	.thumb_func
Func_02002134:
	push {r5, lr}
	bl 0x0200d630
	movs r2, #0
	movs r0, #0
	movs r1, #0
	bl 0x0200d690
	movs r0, #0
	movs r1, #1
	bl 0x0200d660
	movs r0, #0
	movs r1, #1
	bl 0x0200d6a8
	movs r1, #0
	movs r2, #0
	movs r0, #12
	bl 0x0200d6d0
	movs r0, #113
	bl 0x0200d7a8
	movs r1, #128
	movs r2, #60
	lsls r1, r1, #1
	movs r0, #12
	bl 0x0200d718
	ldr r5, [pc, #76]
	adds r0, r5, #0
	bl 0x0200d6e8
	movs r0, #12
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #129
	movs r2, #50
	lsls r1, r1, #1
	movs r0, #0
	adds r5, #1
	bl 0x0200d718
	adds r0, r5, #0
	bl 0x0200d6e8
	movs r1, #0
	movs r0, #12
	bl 0x0200d6f8
	bl 0x0200d778
	movs r0, #60
	bl 0x0200d628
	movs r0, #60
	bl 0x0200d758
	bl 0x0200d638
	movs r0, #137
	lsls r0, r0, #2
	bl 0x0200d618
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000240d
	.global Func_020021c4
	.thumb_func
Func_020021c4:
	push {r5, lr}
	bl 0x0200d630
	movs r2, #0
	movs r0, #0
	movs r1, #0
	bl 0x0200d690
	movs r0, #0
	movs r1, #1
	bl 0x0200d660
	movs r1, #1
	movs r0, #0
	bl 0x0200d6a8
	movs r0, #113
	bl 0x0200d7a8
	movs r1, #128
	movs r0, #21
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200d718
	movs r1, #128
	movs r0, #13
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200d718
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl 0x0200d6d0
	movs r2, #0
	movs r1, #0
	movs r0, #13
	bl 0x0200d6d0
	ldr r5, [pc, #76]
	adds r0, r5, #0
	bl 0x0200d6e8
	movs r0, #13
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #0
	adds r5, #1
	bl 0x0200d718
	adds r0, r5, #0
	bl 0x0200d6e8
	movs r1, #0
	movs r0, #13
	bl 0x0200d6f8
	bl 0x0200d778
	movs r0, #60
	bl 0x0200d628
	movs r0, #60
	bl 0x0200d758
	bl 0x0200d638
	ldr r0, [pc, #16]
	bl 0x0200d618
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000240d
	.4byte 0x00000225
	.global Func_0200226c
	.thumb_func
Func_0200226c:
	push {r5, lr}
	sub sp, #8
	movs r3, #81
	str r3, [sp, #4]
	movs r5, #21
	movs r0, #2
	movs r1, #82
	movs r2, #1
	movs r3, #2
	str r5, [sp, #0]
	bl 0x0200d5d0
	movs r3, #34
	str r3, [sp, #4]
	movs r0, #21
	movs r1, #32
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200d5d8
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020022a0
	.thumb_func
Func_020022a0:
	push {r5, lr}
	sub sp, #8
	movs r3, #55
	str r3, [sp, #4]
	movs r5, #6
	movs r0, #2
	movs r1, #84
	movs r2, #1
	movs r3, #2
	str r5, [sp, #0]
	bl 0x0200d5d0
	movs r3, #10
	str r3, [sp, #4]
	movs r0, #5
	movs r1, #9
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200d5d8
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020022d4
	.thumb_func
Func_020022d4:
	push {r5, lr}
	sub sp, #8
	movs r3, #62
	str r3, [sp, #4]
	movs r5, #27
	movs r0, #2
	movs r1, #86
	movs r2, #1
	movs r3, #2
	str r5, [sp, #0]
	bl 0x0200d5d0
	movs r3, #17
	str r3, [sp, #4]
	movs r0, #26
	movs r1, #16
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200d5d8
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02002308
	.thumb_func
Func_02002308:
	push {r5, r6, lr}
	ldr r3, [pc, #112]
	ldr r2, [pc, #112]
	ldr r3, [r3]
	adds r3, r3, r2
	movs r2, #0
.L_02002314:
	ldrsh r3, [r3, r2]
	sub sp, #8
	cmp r3, #0
	beq .L_02002314_0
	ldr r0, [pc, #100]
	bl 0x0200d610
	cmp r0, #0
	bne .L_02002314_0
	movs r1, #1
	ldr r0, [pc, #92]
	bl 0x0200d600
	movs r0, #188
	bl 0x0200d7a8
	movs r0, #1
	bl 0x0200d628
	movs r5, #17
	movs r1, #77
	movs r2, #1
	movs r3, #2
	movs r6, #82
	movs r0, #6
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200d5d0
	movs r0, #5
	bl 0x0200d628
	movs r1, #77
	movs r2, #1
	movs r3, #2
	movs r0, #7
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200d5d0
	movs r0, #1
	bl 0x0200d628
	bl 0x0200a26c
	ldr r0, [pc, #20]
	bl 0x0200d618
.L_02002314_0:
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x0cb8
	.2byte 0x0000
	.4byte 0x00000947
	.4byte 0x00001528
	.global Func_0200238c
	.thumb_func
Func_0200238c:
	push {r5, r6, lr}
	ldr r3, [pc, #112]
	ldr r2, [pc, #112]
	ldr r3, [r3]
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	sub sp, #8
	cmp r3, #0
	beq .L_0200238c_0
	ldr r0, [pc, #100]
	bl 0x0200d610
	cmp r0, #0
	bne .L_0200238c_0
	movs r1, #1
	ldr r0, [pc, #92]
	bl 0x0200d600
	movs r0, #188
	bl 0x0200d7a8
	movs r0, #1
	bl 0x0200d628
	movs r5, #3
	movs r1, #77
	movs r2, #1
	movs r3, #2
	movs r6, #55
	movs r0, #6
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200d5d0
	movs r0, #5
	bl 0x0200d628
	movs r1, #77
	movs r2, #1
	movs r3, #2
	movs r0, #7
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200d5d0
	movs r0, #1
	bl 0x0200d628
	bl 0x0200a2a0
	ldr r0, [pc, #20]
	bl 0x0200d618
.L_0200238c_0:
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x00000cb8
	.4byte 0x00000948
	.4byte 0x00001528
	.global Func_02002410
	.thumb_func
Func_02002410:
	push {r5, r6, lr}
	sub sp, #8
	movs r3, #82
	str r3, [sp, #4]
	movs r6, #17
	movs r0, #5
	movs r1, #77
	movs r2, #1
	movs r3, #2
	str r6, [sp, #0]
	bl 0x0200d5d0
	movs r3, #55
	str r3, [sp, #4]
	movs r5, #3
	movs r0, #5
	movs r1, #77
	movs r2, #1
	movs r3, #2
	str r5, [sp, #0]
	bl 0x0200d5d0
	movs r3, #35
	str r3, [sp, #4]
	movs r0, #15
	movs r1, #33
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	bl 0x0200d5d8
	movs r3, #10
	str r3, [sp, #4]
	movs r0, #3
	movs r1, #8
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200d5d8
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_02002468
	.thumb_func
Func_02002468:
	push {r5, r6, lr}
	sub sp, #8
	movs r3, #82
	str r3, [sp, #4]
	movs r6, #17
	movs r0, #8
	movs r1, #77
	movs r2, #1
	movs r3, #2
	str r6, [sp, #0]
	bl 0x0200d5d0
	movs r3, #55
	str r3, [sp, #4]
	movs r5, #3
	movs r0, #8
	movs r1, #77
	movs r2, #1
	movs r3, #2
	str r5, [sp, #0]
	bl 0x0200d5d0
	movs r3, #35
	str r3, [sp, #4]
	movs r0, #18
	movs r1, #35
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	bl 0x0200d5d8
	movs r3, #10
	str r3, [sp, #4]
	movs r0, #2
	movs r1, #10
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200d5d8
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_020024c0
	.thumb_func
Func_020024c0:
	push {lr}
	ldr r0, [pc, #24]
	bl 0x0200d618
	ldr r0, [pc, #20]
	bl 0x0200d620
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x0200d6a0
	pop {r0}
	bx r0
	.4byte 0x00000944
	.4byte 0x00000217
	.global Func_020024e4
	.thumb_func
Func_020024e4:
	push {lr}
	ldr r0, [pc, #24]
	bl 0x0200d618
	bl 0x0200a2d4
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200d6a0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000945
	.global Func_02002504
	.thumb_func
Func_02002504:
	push {lr}
	ldr r0, [pc, #28]
	bl 0x0200d618
	movs r0, #134
	lsls r0, r0, #2
	bl 0x0200d620
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x0200d6a0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000946
	.global Func_02002528
	.thumb_func
Func_02002528:
	bx lr
	.2byte 0x0000
	.global Func_0200252c
	.thumb_func
Func_0200252c:
	push {lr}
	movs r0, #0
	bl 0x0200d650
	cmp r0, #0
	beq .L_0200252c_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #2
	bl 0x0200d6a0
.L_0200252c_0:
	movs r0, #0
	bl 0x0200d650
	cmp r0, #0
	beq .L_0200252c_1
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #3
	bl 0x0200d6a0
.L_0200252c_1:
	movs r0, #0
	bl 0x0200d650
	cmp r0, #0
	beq .L_0200252c_2
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200d6a0
.L_0200252c_2:
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200d708
	movs r0, #2
	ldr r1, [pc, #120]
	ldr r2, [pc, #120]
	bl 0x0200d658
	movs r1, #228
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #192
	bl 0x0200d680
	movs r0, #3
	ldr r1, [pc, #96]
	ldr r2, [pc, #100]
	bl 0x0200d658
	movs r1, #220
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #184
	bl 0x0200d680
	movs r0, #1
	ldr r1, [pc, #76]
	ldr r2, [pc, #76]
	bl 0x0200d658
	movs r1, #224
	lsls r1, r1, #1
	movs r2, #240
	movs r0, #1
	bl 0x0200d680
	movs r0, #2
	bl 0x0200d698
	movs r1, #12
	movs r2, #0
	movs r0, #2
	bl 0x0200d6d0
	movs r0, #1
	bl 0x0200d698
	movs r0, #3
	bl 0x0200d698
	movs r0, #1
	movs r1, #12
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #3
	movs r1, #12
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #15
	bl 0x0200d628
	pop {r0}
	bx r0
	.4byte 0x0000b333
	.4byte 0x00005999
	.global Func_020025f8
	.thumb_func
Func_020025f8:
	push {lr}
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #11
	sub sp, #8
	bl 0x0200d5f0
	movs r0, #141
	bl 0x0200d7a8
	movs r0, #80
	bl 0x0200d628
	movs r0, #144
	lsls r0, r0, #1
	bl 0x0200d7a8
	movs r0, #5
	bl 0x0200d628
	movs r0, #145
	bl 0x0200d7a8
	movs r3, #26
	movs r2, #55
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #4
	movs r0, #16
	movs r1, #75
	movs r2, #7
	bl 0x0200d5d0
	movs r0, #1
	movs r1, #1
	negs r0, r0
	negs r1, r1
	ldr r2, [pc, #76]
	bl 0x0200d5f0
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200d718
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200d718
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200d718
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200d718
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #12
	bl 0x0200d718
	movs r0, #60
	bl 0x0200d628
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000e666
	.global Func_0200269c
	.thumb_func
Func_0200269c:
	push {lr}
	movs r1, #1
	movs r0, #11
	bl 0x0200d748
	bl 0x0200d740
	movs r0, #60
	bl 0x0200d628
	ldr r0, [pc, #96]
	bl 0x0200d6e8
	movs r0, #13
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #128
	movs r2, #128
	movs r0, #11
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200d658
	movs r1, #128
	movs r2, #128
	movs r0, #15
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200d658
	movs r1, #236
	movs r0, #11
	lsls r1, r1, #1
	movs r2, #180
	bl 0x0200d680
	movs r1, #236
	movs r2, #180
	movs r0, #15
	lsls r1, r1, #1
	bl 0x0200d680
	movs r1, #1
	movs r0, #11
	bl 0x0200d728
	movs r0, #11
	bl 0x0200d698
	movs r0, #11
	movs r1, #4
	bl 0x0200d6a8
	movs r0, #30
	bl 0x0200d628
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000247c
	.global Func_02002718
	.thumb_func
Func_02002718:
	push {lr}
	movs r1, #128
	movs r2, #128
	movs r0, #2
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200d658
	movs r1, #252
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #216
	bl 0x0200d680
	movs r1, #128
	movs r2, #128
	movs r0, #3
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200d658
	movs r1, #220
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #232
	bl 0x0200d680
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #9
.L_02002756:
	lsls r2, r2, #8
	bl 0x0200d658
	movs r1, #240
	lsls r1, r1, #1
	movs r2, #224
	movs r0, #1
	bl 0x0200d680
	movs r0, #1
	bl 0x0200d698
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200d708
	movs r0, #2
	bl 0x0200d698
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #2
	bl 0x0200d708
	movs r0, #3
	bl 0x0200d698
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200d708
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200d708
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020027b0
	.thumb_func
Func_020027b0:
	push {r5, lr}
	ldr r0, [pc, #472]
	bl 0x0200d610
	cmp r0, #0
	beq .L_020027b0_0
	bl 0x0200b02e
.L_020027b0_0:
	movs r0, #156
	lsls r0, r0, #2
	bl 0x0200d618
	bl 0x0200d630
	ldr r0, [pc, #448]
	bl 0x0200d610
	cmp r0, #0
	bne .L_020027b0_1
	b 0x0200a9b0
.L_020027b0_1:
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200d658
	movs r1, #228
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #216
	bl 0x0200d688
	movs r0, #0
	movs r1, #12
	movs r2, #0
	bl 0x0200d6d0
	bl 0x0200a52c
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200d718
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200d708
	movs r2, #0
	movs r1, #4
	movs r0, #12
	bl 0x0200d6b8
	movs r0, #12
	bl 0x0200d650
	movs r1, #1
	bl 0x0200d5e8
	movs r0, #30
	bl 0x0200d628
	movs r0, #2
	ldr r1, [pc, #348]
	ldr r2, [pc, #348]
	bl 0x0200d658
	movs r1, #232
	lsls r1, r1, #1
	movs r2, #192
	movs r0, #2
	bl 0x0200d680
	movs r0, #2
	bl 0x0200d698
	movs r0, #30
	bl 0x0200d628
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200d708
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200d708
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200d708
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200d708
	movs r1, #228
	movs r2, #160
	lsls r2, r2, #17
	movs r0, #13
	lsls r1, r1, #17
	bl 0x0200d6a0
	movs r0, #128
	movs r1, #128
	lsls r1, r1, #7
	lsls r0, r0, #10
	bl 0x0200d730
	ldr r5, [pc, #248]
	adds r0, r5, #0
	bl 0x0200d6e8
	movs r0, #13
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #229
	movs r2, #136
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #13
	bl 0x0200d680
	movs r0, #13
	bl 0x0200d698
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #13
	bl 0x0200d708
	movs r0, #40
	bl 0x0200d628
	movs r1, #8
	movs r2, #8
	negs r1, r1
	movs r0, #13
	bl 0x0200d690
	movs r0, #13
	bl 0x0200d698
	movs r0, #60
	bl 0x0200d628
	movs r0, #155
	bl 0x0200d7a8
	adds r0, r5, #1
	movs r1, #1
	bl 0x0200d600
	movs r2, #8
	negs r2, r2
	movs r1, #8
	movs r0, #13
	bl 0x0200d690
	bl 0x0200a5f8
	movs r0, #120
	bl 0x0200d628
	movs r0, #0
	movs r1, #2
	bl 0x0200d6c0
	movs r0, #2
	movs r1, #2
	bl 0x0200d6c0
	movs r0, #1
	movs r1, #2
	bl 0x0200d6c0
	movs r1, #2
	movs r0, #3
	bl 0x0200d6c0
	movs r0, #20
	bl 0x0200d628
	adds r5, #2
	bl 0x0200a718
	movs r2, #0
	movs r1, #0
	movs r0, #13
	bl 0x0200d6d0
	adds r0, r5, #0
	bl 0x0200d6e8
	movs r1, #0
	movs r0, #13
	bl 0x0200d6f8
	bl 0x0200a69c
	ldr r3, [pc, #68]
	movs r2, #224
	ldr r3, [r3]
.L_02002960:
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #64
	str r2, [r3]
	movs r0, #1
	bl 0x0200d628
	ldr r3, [pc, #52]
	ldr r2, [pc, #52]
	adds r3, r3, r2
	movs r2, #3
	strb r2, [r3]
	ldr r0, [pc, #48]
	movs r1, #4
	bl 0x0200d768
	movs r0, #98
	movs r1, #4
	bl 0x0200d760
	b 0x0200b02a
	.2byte 0x0000
	.2byte 0x0301
	.2byte 0x0000
	.2byte 0x0942
	.2byte 0x0000
	.2byte 0xb333
	.2byte 0x0000
	.2byte 0x5999
	.2byte 0x0000
	.2byte 0x247d
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.4byte 0x02000240
	.4byte 0x0000022b
	.4byte 0x000000a3
	.2byte 0x2101
	.2byte 0x2000
	.2byte 0xf002
	.2byte 0xfe78
	.2byte 0x2011
	.2byte 0xf002
	.2byte 0xfef5
	.2byte 0x201e
	.2byte 0xf002
	.2byte 0xfe32
	.2byte 0x4dfe
	.2byte 0x1c28
	.2byte 0xf002
	.2byte 0xfe8e
	.2byte 0x200c
	.2byte 0x2100
	.2byte 0xf002
	.2byte 0xfe92
	.2byte 0x210c
	.2byte 0x2200
	.2byte 0x2000
	.2byte 0xf002
	.2byte 0xfe7d
	.2byte 0x208c
	.2byte 0xf002
	.2byte 0xfe22
	.2byte 0x2180
	.2byte 0x200c
	.2byte 0x0209
	.2byte 0x2200
	.2byte 0xf002
	.2byte 0xfe8c
	.2byte 0x2200
	.2byte 0x2104
	.2byte 0x200c
	.2byte 0xf002
	.2byte 0xfe5f
	.2byte 0x200c
	.2byte 0xf002
	.2byte 0xfe28
	.2byte 0x2101
	.2byte 0xf002
	.2byte 0xfdf1
	.2byte 0x1c68
	.2byte 0xf002
	.2byte 0xfe6e
	.2byte 0x200c
	.2byte 0x2100
	.2byte 0xf002
	.2byte 0xfe72
	.2byte 0x2180
	.2byte 0x2280
	.2byte 0x2000
	.2byte 0x0209
	.2byte 0x01d2
	.2byte 0xf002
	.2byte 0xfe1b
	.2byte 0x21e4
	.2byte 0x2000
	.2byte 0x0049
	.2byte 0x22d8
	.2byte 0xf002
	.2byte 0xfe2d
	.2byte 0x2000
	.2byte 0x210c
	.2byte 0x2200
	.2byte 0xf002
	.2byte 0xfe4c
	.2byte 0xf7ff
	.2byte 0xfd78
	.2byte 0x2002
	.2byte 0x49e1
	.2byte 0x4ae1
	.2byte 0xf002
	.2byte 0xfe09
	.2byte 0x21e8
	.2byte 0x22c0
	.2byte 0x0049
	.2byte 0x2002
	.2byte 0xf002
	.2byte 0xfe17
	.2byte 0x2002
	.2byte 0xf002
	.2byte 0xfe20
	.2byte 0x201e
	.2byte 0xf002
	.2byte 0xfde5
	.2byte 0x1ca8
	.2byte 0xf002
	.2byte 0xfe42
	.2byte 0x2002
	.2byte 0x2100
	.2byte 0xf002
	.2byte 0xfe46
	.2byte 0x2180
	.2byte 0x2200
	.2byte 0x0049
	.2byte 0x200c
	.2byte 0xf002
	.2byte 0xfe50
	.2byte 0x206e
	.2byte 0xf002
	.2byte 0xfdd5
	.2byte 0x203c
	.2byte 0xf002
	.2byte 0xfe92
	.2byte 0x1ce8
	.2byte 0xf002
	.2byte 0xfe2f
	.2byte 0x2100
	.2byte 0x200c
	.2byte 0xf002
	.2byte 0xfe33
	.2byte 0x201e
	.2byte 0xf002
	.2byte 0xfdc8
	.2byte 0x2103
	.2byte 0x2002
	.2byte 0xf002
	.2byte 0xfe08
	.2byte 0x2014
	.2byte 0xf002
	.2byte 0xfdc1
	.2byte 0x2002
	.2byte 0x2101
	.2byte 0xf002
	.2byte 0xfdfd
	.2byte 0x2101
	.2byte 0x200c
	.2byte 0xf002
	.2byte 0xfe05
	.2byte 0x2014
	.2byte 0xf002
	.2byte 0xfdb6
	.2byte 0x200c
	.2byte 0x49c3
	.2byte 0x4ac3
	.2byte 0xf002
	.2byte 0xfdc9
	.2byte 0x2182
	.2byte 0x22d0
	.2byte 0x0089
	.2byte 0x200c
	.2byte 0xf002
	.2byte 0xfdd7
	.2byte 0x200c
	.2byte 0xf002
	.2byte 0xfde0
	.2byte 0x2101
	.2byte 0x200c
	.2byte 0xf002
	.2byte 0xfde4
	.2byte 0x2014
	.2byte 0xf002
	.2byte 0xfda1
	.2byte 0x21b0
	.2byte 0x0209
	.2byte 0x2200
	.2byte 0x200c
	.2byte 0xf002
	.2byte 0xfe0b
	.2byte 0x201e
	.2byte 0xf002
	.2byte 0xfd98
	.2byte 0x21a0
	.2byte 0x01c9
	.2byte 0x2200
	.2byte 0x200c
	.2byte 0xf002
	.2byte 0xfe02
	.2byte 0x201e
	.2byte 0xf002
	.2byte 0xfd8f
	.2byte 0x2200
	.2byte 0x2102
	.2byte 0x200c
	.2byte 0xf002
	.2byte 0xfdde
	.2byte 0x2014
	.2byte 0xf002
	.2byte 0xfd87
	.2byte 0x1d28
	.2byte 0xf002
	.2byte 0xfde4
	.2byte 0x2100
	.2byte 0x200c
	.2byte 0xf002
	.2byte 0xfde8
	.2byte 0x2028
	.2byte 0xf002
	.2byte 0xfd7d
	.2byte 0x2103
	.2byte 0x2002
	.2byte 0xf002
	.2byte 0xfdbd
	.2byte 0x2014
	.2byte 0xf002
	.2byte 0xfd76
	.2byte 0x2184
.L_02002b3e:
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #12
	bl 0x0200d718
	movs r0, #120
	bl 0x0200d628
	adds r0, r5, #5
	bl 0x0200d6e8
	movs r1, #0
	movs r0, #12
	bl 0x0200d6f8
	movs r0, #25
	bl 0x0200d628
	movs r1, #3
	movs r0, #2
	bl 0x0200d6b0
	movs r0, #30
	bl 0x0200d628
	movs r1, #3
.L_02002b72:
	movs r0, #12
	bl 0x0200d6b0
	movs r0, #40
	bl 0x0200d628
	movs r1, #240
	lsls r1, r1, #1
	movs r2, #200
	movs r0, #2
	bl 0x0200d680
	movs r0, #2
	bl 0x0200d698
	movs r2, #0
	movs r1, #12
	movs r0, #2
	bl 0x0200d6d8
	movs r0, #60
	bl 0x0200d628
	adds r0, r5, #6
	bl 0x0200d6e8
	movs r1, #0
	movs r0, #2
	bl 0x0200d6f8
	movs r0, #20
	bl 0x0200d628
	movs r1, #4
	movs r0, #12
	bl 0x0200d6a8
	movs r0, #80
	bl 0x0200d628
	adds r0, r5, #7
	bl 0x0200d6e8
	movs r0, #12
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #228
	movs r2, #160
	lsls r2, r2, #17
	lsls r1, r1, #17
	movs r0, #13
	bl 0x0200d6a0
	movs r0, #19
	bl 0x0200d7a8
	adds r0, r5, #0
	adds r0, #8
	bl 0x0200d6e8
	movs r0, #13
	movs r1, #0
	bl 0x0200d6f8
	movs r0, #0
	movs r1, #13
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #2
	movs r1, #13
	movs r2, #0
	bl 0x0200d6d0
	movs r1, #13
	movs r2, #0
	movs r0, #1
	bl 0x0200d6d0
	movs r0, #5
	bl 0x0200d628
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200d708
	movs r2, #0
	movs r1, #13
	movs r0, #12
	bl 0x0200d6d0
	movs r0, #30
	bl 0x0200d628
	movs r0, #61
	bl 0x0200d7a8
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200d730
	movs r0, #13
	movs r1, #1
	bl 0x0200d748
	bl 0x0200d740
	movs r0, #13
	ldr r1, [pc, #380]
	ldr r2, [pc, #372]
	bl 0x0200d658
	movs r1, #228
	movs r2, #152
	lsls r2, r2, #1
	movs r0, #13
	lsls r1, r1, #1
	bl 0x0200d680
	movs r1, #1
	movs r0, #13
	bl 0x0200d728
	movs r0, #13
	bl 0x0200d698
	movs r0, #1
	movs r1, #1
	bl 0x0200d728
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200d718
	movs r1, #129
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200d718
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200d718
	movs r1, #129
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200d718
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #12
	bl 0x0200d718
	movs r0, #60
	bl 0x0200d628
	movs r2, #0
	movs r0, #12
	movs r1, #13
	bl 0x0200d6d0
	movs r1, #2
	movs r0, #12
	bl 0x0200d6c0
	movs r0, #60
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #9
	bl 0x0200d6e8
	movs r0, #12
	movs r1, #0
	bl 0x0200d6f8
	movs r2, #0
	movs r1, #12
	movs r0, #13
	bl 0x0200d6d0
	adds r0, r5, #0
	adds r0, #10
	bl 0x0200d6e8
	movs r1, #0
	movs r0, #13
	bl 0x0200d6f8
	movs r0, #60
	bl 0x0200d628
	movs r2, #0
	movs r1, #2
	movs r0, #13
.L_02002d10:
	bl 0x0200d6d0
	movs r0, #30
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #11
	bl 0x0200d6e8
	movs r0, #13
	movs r1, #0
	bl 0x0200d6f8
	movs r0, #3
	movs r1, #2
	movs r2, #0
	bl 0x0200d6d8
	movs r1, #1
	movs r2, #0
	movs r0, #0
	bl 0x0200d6d8
	movs r0, #60
	bl 0x0200d628
	movs r0, #0
	movs r1, #13
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #2
	movs r1, #13
	movs r2, #0
	bl 0x0200d6d0
.L_02002d58:
	movs r0, #1
.L_02002d5a:
	movs r1, #13
	movs r2, #0
	bl 0x0200d6d0
	movs r2, #0
	movs r0, #3
	movs r1, #13
	bl 0x0200d6d0
	movs r1, #1
	movs r0, #13
	bl 0x0200d6c0
	movs r0, #60
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #12
	bl 0x0200d6e8
	movs r0, #13
	movs r1, #0
	bl 0x0200d6f8
	movs r2, #0
	ldr r1, [pc, #72]
	movs r0, #1
	bl 0x0200d718
	movs r0, #60
	bl 0x0200d628
	movs r1, #4
	movs r0, #13
	bl 0x0200d6a8
	adds r0, r5, #0
	adds r0, #13
	bl 0x0200d6e8
	movs r0, #13
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #228
	lsls r1, r1, #1
	movs r2, #248
	movs r0, #1
	bl 0x0200d680
	b .L_02002d5a_0
	.2byte 0x2464
	.2byte 0x0000
	.2byte 0xb333
	.2byte 0x0000
	.2byte 0x5999
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.2byte 0x3333
	.2byte 0x0000
	.2byte 0xcccc
	.2byte 0x0000
	.4byte 0x00000103
.L_02002d5a_0:
	movs r0, #1
	bl 0x0200d698
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #1
	bl 0x0200d708
	adds r0, r5, #0
	adds r0, #14
	bl 0x0200d6e8
	movs r0, #1
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #236
	lsls r1, r1, #1
	movs r2, #216
	movs r0, #2
	bl 0x0200d680
	movs r0, #2
	bl 0x0200d698
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #2
	bl 0x0200d708
	movs r0, #10
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #15
	bl 0x0200d6e8
	movs r0, #2
	movs r1, #0
	bl 0x0200d6f8
	movs r2, #0
	ldr r1, [pc, #508]
	movs r0, #12
	bl 0x0200d718
.L_02002e3c:
	movs r0, #60
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #16
	bl 0x0200d6e8
	movs r0, #12
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #220
	lsls r1, r1, #1
	movs r2, #216
	movs r0, #3
	bl 0x0200d680
	movs r0, #3
	bl 0x0200d698
	movs r2, #0
	movs r0, #3
	movs r1, #13
	bl 0x0200d6d0
	movs r1, #3
	movs r0, #3
	bl 0x0200d6b0
	movs r0, #10
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #17
	bl 0x0200d6e8
	movs r0, #3
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #4
	movs r0, #13
	bl 0x0200d6a8
	adds r0, r5, #0
	adds r0, #18
	bl 0x0200d6e8
	movs r0, #13
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200d718
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200d718
	movs r1, #129
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200d718
	movs r1, #129
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200d718
	movs r1, #132
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #13
	bl 0x0200d718
	movs r0, #60
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #19
.L_02002eea:
	bl 0x0200d6e8
	movs r1, #0
	movs r0, #13
	bl 0x0200d6f8
	movs r0, #20
	bl 0x0200d628
	movs r2, #0
	ldr r1, [pc, #312]
	movs r0, #1
	bl 0x0200d718
	movs r0, #60
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #20
	bl 0x0200d6e8
	movs r0, #1
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #1
	movs r0, #13
	bl 0x0200d6c0
	movs r0, #60
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #21
	bl 0x0200d6e8
	movs r0, #13
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #228
	movs r2, #140
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #13
	bl 0x0200d680
	movs r0, #13
	bl 0x0200d698
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #13
	bl 0x0200d708
	movs r0, #80
	bl 0x0200d628
	movs r1, #8
	movs r2, #8
	negs r1, r1
	movs r0, #13
	bl 0x0200d690
	movs r0, #13
	bl 0x0200d698
	movs r0, #60
	bl 0x0200d628
	movs r0, #155
	bl 0x0200d7a8
	ldr r0, [pc, #188]
	movs r1, #1
	bl 0x0200d600
	movs r2, #8
	movs r0, #13
	movs r1, #8
	negs r2, r2
	bl 0x0200d690
	movs r2, #0
	movs r1, #11
	movs r0, #13
	bl 0x0200d6d0
	bl 0x0200a5f8
	movs r0, #52
	bl 0x0200d7a8
	adds r0, r5, #0
	adds r0, #23
	bl 0x0200d6e8
.L_02002fae:
	movs r1, #0
	movs r0, #13
	bl 0x0200d6f8
	movs r0, #60
	bl 0x0200d628
	movs r0, #0
	movs r1, #11
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #1
	movs r1, #11
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #2
	movs r1, #11
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #3
	movs r1, #11
	movs r2, #0
	bl 0x0200d6d0
	movs r1, #11
.L_02002fe6:
	movs r2, #0
	movs r0, #12
	bl 0x0200d6d0
	bl 0x0200a718
	bl 0x0200a69c
	ldr r0, [pc, #72]
	bl 0x0200d618
	ldr r3, [pc, #68]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
.L_02003006:
	adds r2, #64
	str r2, [r3]
	movs r0, #1
	bl 0x0200d628
	ldr r3, [pc, #52]
	ldr r2, [pc, #56]
	adds r3, r3, r2
	movs r2, #3
	strb r2, [r3]
	ldr r0, [pc, #52]
	movs r1, #4
	bl 0x0200d768
	movs r0, #98
	movs r1, #4
	bl 0x0200d760
	bl 0x0200d638
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0105
	.2byte 0x0000
	.2byte 0x0103
	.2byte 0x0000
	.2byte 0x247e
	.2byte 0x0000
	.2byte 0x0942
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.4byte 0x02000240
	.4byte 0x0000022b
	.4byte 0x000000a3
.L_02003054:
	.global Func_02003054
	.thumb_func
Func_02003054:
	push {r5, lr}
	ldr r0, [pc, #796]
	sub sp, #8
	bl 0x0200d618
	ldr r0, [pc, #792]
	bl 0x0200d618
	movs r3, #26
	movs r2, #55
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #4
	movs r2, #7
	movs r1, #75
	movs r0, #16
	bl 0x0200d5d0
	movs r0, #4
	bl 0x02008c90
	bl 0x0200d630
	movs r0, #12
	bl 0x0200d650
	movs r1, #1
	bl 0x0200d5e8
	movs r1, #228
	movs r2, #184
	movs r0, #0
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x0200d6a0
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200d708
	movs r1, #220
	movs r2, #192
	movs r0, #1
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x0200d6a0
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200d708
	movs r1, #244
	movs r2, #184
	movs r0, #3
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x0200d6a0
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200d708
	movs r1, #236
	movs r2, #184
	movs r0, #2
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x0200d6a0
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200d708
	movs r1, #130
	movs r2, #224
	movs r0, #12
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl 0x0200d6a0
	movs r1, #176
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200d708
	movs r1, #224
	movs r2, #237
.L_02003114:
	movs r0, #11
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x0200d6a0
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200d708
	movs r1, #224
	movs r2, #238
	lsls r2, r2, #16
	movs r0, #15
	lsls r1, r1, #17
	bl 0x0200d6a0
	movs r0, #15
	movs r1, #3
	bl 0x0200d710
	movs r0, #11
	movs r1, #3
	bl 0x0200d6a8
	movs r1, #229
	movs r2, #243
	movs r0, #13
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x0200d6a0
	movs r1, #128
	movs r2, #0
	movs r0, #13
	lsls r1, r1, #7
	bl 0x0200d708
	movs r1, #5
	movs r0, #13
	bl 0x0200d6a8
	bl 0x0200d770
	movs r0, #120
	bl 0x0200d628
	ldr r5, [pc, #516]
	adds r0, r5, #0
	bl 0x0200d6e8
	movs r0, #13
	movs r1, #0
	bl 0x0200d6f8
	movs r2, #60
.L_02003186:
	ldr r1, [pc, #504]
	movs r0, #1
	bl 0x0200d718
	adds r0, r5, #1
	bl 0x0200d6e8
	movs r0, #1
	movs r1, #0
	bl 0x0200d6f8
	movs r0, #0
	ldr r1, [pc, #480]
	movs r2, #1
	bl 0x0200d718
	movs r0, #2
	ldr r1, [pc, #468]
	movs r2, #1
	bl 0x0200d718
	ldr r1, [pc, #460]
	movs r2, #1
	movs r0, #3
	bl 0x0200d718
	movs r0, #60
	bl 0x0200d628
	movs r2, #0
	movs r1, #13
	movs r0, #12
	bl 0x0200d6d0
	movs r0, #60
	bl 0x0200d628
	adds r0, r5, #2
	bl 0x0200d6e8
	movs r0, #12
	movs r1, #0
	bl 0x0200d6f8
	movs r0, #0
	movs r1, #12
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #1
	movs r1, #12
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #2
	movs r1, #12
	movs r2, #0
	bl 0x0200d6d0
	movs r1, #12
	movs r2, #0
	movs r0, #3
	bl 0x0200d6d0
	movs r0, #60
	bl 0x0200d628
	movs r1, #128
	movs r2, #232
	movs r0, #12
	lsls r1, r1, #2
	bl 0x0200d680
	movs r1, #4
	movs r0, #12
	bl 0x0200d6a8
	movs r0, #60
	bl 0x0200d628
	adds r0, r5, #3
	bl 0x0200d6e8
	movs r1, #0
	movs r0, #12
	bl 0x0200d6f8
	movs r0, #15
	bl 0x0200d628
	movs r1, #2
	movs r0, #13
	bl 0x0200d6c0
	adds r0, r5, #4
	bl 0x0200d6e8
	movs r0, #13
	movs r1, #0
	bl 0x0200d6f8
	movs r0, #0
	movs r1, #13
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #2
	movs r1, #13
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #3
	movs r1, #13
	movs r2, #0
.L_0200326a:
	bl 0x0200d6d0
	movs r2, #0
	movs r0, #1
	movs r1, #13
	bl 0x0200d6d0
	movs r1, #1
	movs r0, #2
	bl 0x0200d6c0
	adds r0, r5, #5
	bl 0x0200d6e8
	movs r0, #2
	movs r1, #0
	bl 0x0200d6f8
	movs r2, #0
	movs r0, #3
	ldr r1, [pc, #240]
	bl 0x0200d718
	movs r1, #4
	movs r0, #3
	bl 0x0200d6a8
	movs r0, #80
	bl 0x0200d628
	movs r1, #1
	movs r0, #3
	bl 0x0200d6a8
	adds r0, r5, #6
	bl 0x0200d6e8
	movs r0, #3
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #129
	movs r2, #65
	movs r0, #12
	lsls r1, r1, #1
	bl 0x0200d718
	movs r1, #2
	movs r0, #12
	bl 0x0200d6c0
	movs r0, #100
	bl 0x0200d628
	adds r0, r5, #7
	bl 0x0200d6e8
	movs r0, #13
	movs r1, #0
	bl 0x0200d6f8
	movs r2, #0
	movs r1, #0
	movs r0, #1
	bl 0x0200d6d0
	movs r0, #10
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #8
	bl 0x0200d6e8
	movs r0, #1
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #1
	movs r0, #2
	bl 0x0200d6c0
	movs r0, #30
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #9
	bl 0x0200d6e8
	movs r0, #2
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #0
	movs r2, #0
	movs r0, #3
	bl 0x0200d6d0
	adds r0, r5, #0
	adds r0, #10
	bl 0x0200d6e8
	movs r1, #0
	movs r0, #3
	bl 0x0200d6f0
	movs r0, #0
	movs r1, #0
	bl 0x0200d648
	cmp r0, #0
	bne .L_0200326a_0
	movs r0, #2
	movs r1, #3
	bl 0x0200d6a8
	movs r0, #1
	movs r1, #3
	bl 0x0200d6a8
	movs r0, #3
	movs r1, #3
	bl 0x0200d6a8
	movs r0, #12
	movs r1, #13
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #60
	bl 0x0200d628
	b .L_0200326a_1
	.2byte 0x0000
	.2byte 0x0301
	.2byte 0x0000
	.2byte 0x0941
	.2byte 0x0000
	.2byte 0x2481
	.2byte 0x0000
	.2byte 0x0101
	.2byte 0x0000
	.4byte 0x00000105
.L_0200326a_0:
	adds r0, r5, #0
	adds r0, #11
	bl 0x0200d6e8
	movs r0, #3
	movs r1, #0
	bl 0x0200d6f8
	movs r0, #0
	movs r1, #12
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #1
	movs r1, #12
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #2
	movs r1, #12
	movs r2, #0
	bl 0x0200d6d0
	movs r1, #12
	movs r2, #0
	movs r0, #3
	bl 0x0200d6d0
	movs r0, #20
	bl 0x0200d628
	movs r1, #0
	movs r2, #0
	movs r0, #12
	bl 0x0200d6d0
	movs r0, #60
	bl 0x0200d628
	movs r2, #0
	movs r1, #13
	movs r0, #12
	bl 0x0200d6d0
	movs r0, #80
	bl 0x0200d628
	movs r1, #3
	movs r0, #12
	bl 0x0200d6b0
	movs r0, #30
	bl 0x0200d628
	movs r2, #0
	movs r1, #0
	movs r0, #12
	bl 0x0200d6d0
	adds r0, r5, #0
	adds r0, #12
	bl 0x0200d6e8
	movs r1, #0
	movs r0, #12
	bl 0x0200d6f8
	movs r0, #60
	bl 0x0200d628
	movs r0, #2
	movs r1, #12
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #1
	movs r1, #12
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #3
	movs r1, #12
	movs r2, #0
	bl 0x0200d6d0
	movs r2, #0
	movs r0, #0
	movs r1, #12
	bl 0x0200d6d0
	movs r0, #2
	movs r1, #3
	bl 0x0200d6a8
	movs r0, #1
	movs r1, #3
	bl 0x0200d6a8
	movs r0, #3
	movs r1, #3
	bl 0x0200d6a8
	movs r1, #3
	movs r0, #0
	bl 0x0200d6a8
	movs r0, #80
	bl 0x0200d628
	movs r0, #12
	movs r1, #13
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #2
	movs r1, #13
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #1
	movs r1, #13
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #3
	movs r1, #13
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #0
	movs r1, #13
	movs r2, #0
	bl 0x0200d6d0
.L_0200326a_1:
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200d658
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200d658
	movs r1, #128
	movs r2, #128
	movs r0, #3
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200d658
	movs r1, #128
	movs r2, #128
	movs r0, #2
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200d658
	movs r1, #208
	lsls r1, r1, #1
	movs r2, #216
	movs r0, #1
	bl 0x0200d680
	movs r0, #1
	bl 0x0200d698
	movs r1, #208
	lsls r1, r1, #1
	movs r2, #248
	movs r0, #1
	bl 0x0200d680
	movs r0, #1
	bl 0x0200d698
	movs r1, #220
	lsls r1, r1, #1
	movs r2, #248
	movs r0, #1
	bl 0x0200d680
	movs r0, #1
	bl 0x0200d698
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200d708
	movs r1, #220
	lsls r1, r1, #1
	movs r2, #216
	movs r0, #0
	bl 0x0200d680
	movs r0, #0
	bl 0x0200d698
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200d708
	movs r1, #244
	lsls r1, r1, #1
	movs r2, #248
	movs r0, #3
	bl 0x0200d680
	movs r0, #3
	bl 0x0200d698
	movs r1, #228
	lsls r1, r1, #1
	movs r2, #248
	movs r0, #3
	bl 0x0200d680
	movs r0, #3
	bl 0x0200d698
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200d708
	movs r1, #228
	lsls r1, r1, #1
	movs r2, #216
	movs r0, #2
	bl 0x0200d680
	movs r0, #2
	bl 0x0200d698
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200d708
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200d6d8
	movs r2, #0
	movs r1, #2
	movs r0, #3
	bl 0x0200d6d8
	movs r0, #60
	bl 0x0200d628
	movs r0, #0
	movs r1, #3
	bl 0x0200d6b0
	movs r0, #2
	movs r1, #3
	bl 0x0200d6a8
	movs r0, #1
	movs r1, #3
	bl 0x0200d6a8
	movs r1, #3
	movs r0, #3
	bl 0x0200d6a8
	movs r0, #100
	bl 0x0200d628
	movs r0, #226
	bl 0x0200d7a8
	movs r0, #13
	movs r1, #7
	bl 0x0200d6a8
	movs r0, #0
	ldr r1, [pc, #1016]
	ldr r2, [pc, #1020]
	bl 0x0200d658
	movs r1, #24
	movs r0, #0
	negs r1, r1
	movs r2, #0
	bl 0x0200d690
	movs r0, #1
	ldr r1, [pc, #996]
	ldr r2, [pc, #996]
	bl 0x0200d658
	movs r1, #24
	movs r0, #1
	negs r1, r1
	movs r2, #0
	bl 0x0200d690
	movs r0, #3
	ldr r1, [pc, #972]
	ldr r2, [pc, #976]
	bl 0x0200d658
	movs r1, #24
	movs r0, #3
	negs r1, r1
	movs r2, #0
	bl 0x0200d690
	movs r0, #2
	ldr r1, [pc, #952]
	ldr r2, [pc, #952]
	bl 0x0200d658
	movs r1, #24
	movs r0, #2
	negs r1, r1
	movs r2, #0
	bl 0x0200d690
	movs r0, #11
	ldr r1, [pc, #928]
	ldr r2, [pc, #932]
	bl 0x0200d658
	movs r0, #15
	ldr r1, [pc, #920]
	ldr r2, [pc, #920]
	bl 0x0200d658
	movs r1, #24
	movs r0, #11
	negs r1, r1
	movs r2, #0
	bl 0x0200d690
	movs r1, #24
	negs r1, r1
	movs r2, #0
	movs r0, #15
	bl 0x0200d690
	movs r0, #0
	bl 0x0200d698
	movs r0, #144
	lsls r0, r0, #1
	bl 0x0200d7a8
	movs r0, #60
	bl 0x0200d628
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200d708
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200d708
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200d708
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #3
	bl 0x0200d708
	movs r0, #60
	bl 0x0200d628
	movs r0, #0
	movs r1, #3
	bl 0x0200d6a8
	movs r0, #1
	movs r1, #3
	bl 0x0200d6a8
	movs r0, #2
	movs r1, #3
	bl 0x0200d6a8
	movs r1, #3
	movs r0, #3
	bl 0x0200d6a8
	movs r0, #100
	bl 0x0200d628
	movs r0, #0
	ldr r1, [pc, #780]
	ldr r2, [pc, #784]
	bl 0x0200d658
	movs r0, #1
	ldr r1, [pc, #772]
	ldr r2, [pc, #772]
	bl 0x0200d658
	movs r0, #2
	ldr r1, [pc, #760]
	ldr r2, [pc, #764]
	bl 0x0200d658
	movs r0, #3
	ldr r1, [pc, #752]
	ldr r2, [pc, #752]
	bl 0x0200d658
	movs r1, #228
	movs r2, #184
	lsls r1, r1, #1
	movs r0, #0
	bl 0x0200d680
	movs r0, #0
	bl 0x0200d698
	movs r0, #0
	movs r1, #1
	bl 0x0200d6a8
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200d708
	movs r1, #232
	movs r2, #128
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #1
	bl 0x0200d680
	movs r0, #1
	bl 0x0200d698
	movs r1, #240
	lsls r1, r1, #1
	movs r2, #248
	movs r0, #1
	bl 0x0200d680
	movs r0, #1
	bl 0x0200d698
	movs r1, #220
	movs r2, #192
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200d680
	movs r0, #1
	bl 0x0200d698
	movs r0, #1
	movs r1, #1
	bl 0x0200d6a8
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200d708
	movs r1, #244
	lsls r1, r1, #1
	movs r2, #248
	movs r0, #3
	bl 0x0200d680
	movs r0, #3
	bl 0x0200d698
	movs r1, #244
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #184
	bl 0x0200d680
	movs r1, #236
	movs r2, #184
	lsls r1, r1, #1
	movs r0, #2
	bl 0x0200d680
	movs r0, #2
	bl 0x0200d698
	movs r0, #2
	movs r1, #1
	bl 0x0200d6a8
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #2
	bl 0x0200d708
	movs r0, #3
	bl 0x0200d698
	movs r0, #3
	movs r1, #1
	bl 0x0200d6a8
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #3
	bl 0x0200d708
	movs r0, #30
	bl 0x0200d628
	movs r0, #60
	bl 0x0200d628
	ldr r5, [pc, #532]
	adds r0, r5, #0
	bl 0x0200d6e8
	movs r0, #1
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #6
	movs r0, #13
	bl 0x0200d6a8
	movs r0, #120
	bl 0x0200d628
	movs r1, #2
	movs r0, #13
	bl 0x0200d6c0
	movs r0, #60
	bl 0x0200d628
	movs r1, #7
	movs r0, #13
	bl 0x0200d6a8
	adds r0, r5, #1
	bl 0x0200d6e8
	movs r1, #0
	movs r0, #13
	bl 0x0200d6f8
	movs r0, #20
	bl 0x0200d628
	movs r1, #2
	movs r2, #0
	movs r0, #3
	bl 0x0200d6d8
	movs r0, #10
	bl 0x0200d628
	movs r0, #3
	ldr r1, [pc, #444]
	movs r2, #80
	bl 0x0200d718
	movs r0, #3
	movs r1, #13
	movs r2, #0
	bl 0x0200d6d0
	movs r2, #0
	movs r1, #13
	movs r0, #2
	bl 0x0200d6d0
	adds r0, r5, #2
	bl 0x0200d6e8
	movs r0, #3
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #129
	movs r2, #70
	movs r0, #13
	lsls r1, r1, #1
	bl 0x0200d718
	movs r1, #2
	movs r0, #13
	bl 0x0200d6c0
	movs r0, #60
	bl 0x0200d628
	movs r1, #5
	movs r0, #13
	bl 0x0200d6a8
	movs r0, #70
	bl 0x0200d628
.L_0200386a:
	adds r0, r5, #3
	bl 0x0200d6e8
	movs r0, #13
	movs r1, #0
	bl 0x0200d6f8
	movs r0, #13
	movs r1, #7
	bl 0x0200d6a8
	movs r1, #132
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #2
	bl 0x0200d718
	adds r0, r5, #4
	bl 0x0200d6e8
	movs r0, #2
	movs r1, #0
	bl 0x0200d6f8
	movs r0, #12
	movs r1, #3
	bl 0x0200d6b0
	movs r0, #1
	ldr r1, [pc, #308]
	movs r2, #60
	bl 0x0200d718
	movs r1, #220
	lsls r1, r1, #1
	movs r2, #208
	movs r0, #1
	bl 0x0200d680
	movs r0, #1
	bl 0x0200d698
	movs r2, #0
	movs r1, #0
	movs r0, #1
	bl 0x0200d6d0
	adds r0, r5, #5
.L_020038ca:
	bl 0x0200d6e8
	movs r1, #0
	movs r0, #1
	bl 0x0200d6f8
	movs r0, #20
	bl 0x0200d628
	movs r0, #0
	ldr r1, [pc, #248]
	movs r2, #0
.L_020038e2:
	bl 0x0200d718
	movs r0, #2
	ldr r1, [pc, #236]
	movs r2, #0
	bl 0x0200d718
	movs r0, #3
	ldr r1, [pc, #228]
	movs r2, #0
	bl 0x0200d718
	ldr r1, [pc, #220]
	movs r2, #0
	movs r0, #12
	bl 0x0200d718
	movs r0, #70
	bl 0x0200d628
	movs r0, #1
	movs r1, #2
	movs r2, #0
	bl 0x0200d6d0
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #75
	bl 0x0200d718
	movs r2, #0
	movs r0, #1
	movs r1, #0
	bl 0x0200d6d0
	movs r1, #4
	movs r0, #1
	bl 0x0200d6a8
	adds r0, r5, #6
	bl 0x0200d6e8
	movs r0, #1
	movs r1, #0
	bl 0x0200d6f8
	movs r2, #0
	ldr r1, [pc, #148]
	movs r0, #12
	bl 0x0200d718
	movs r0, #60
	bl 0x0200d628
	adds r0, r5, #7
	bl 0x0200d6e8
	movs r0, #12
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #224
	movs r2, #208
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200d680
	movs r0, #1
	bl 0x0200d698
	movs r0, #1
	movs r1, #1
	bl 0x0200d6a8
	movs r2, #0
	movs r1, #12
	movs r0, #1
	bl 0x0200d6d0
	adds r0, r5, #0
	adds r0, #8
	bl 0x0200d6e8
	movs r0, #1
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #1
	movs r0, #12
.L_02003996:
	bl 0x0200d6c0
	movs r0, #60
	bl 0x0200d628
	movs r2, #0
	movs r1, #12
	movs r0, #1
	bl 0x0200d6d0
	movs r0, #60
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #9
	bl 0x0200d6e8
	movs r0, #12
	movs r1, #0
	bl 0x0200d6f8
	b .L_02003996_0
	.2byte 0x0000
	.2byte 0x1999
	.2byte 0x0000
	.2byte 0x0ccc
	.2byte 0x0000
	.2byte 0xb333
	.2byte 0x0000
	.2byte 0x5999
	.2byte 0x0000
	.2byte 0x248e
	.2byte 0x0000
	.2byte 0x0101
	.2byte 0x0000
	.2byte 0x0103
	.2byte 0x0000
.L_02003996_0:
	movs r1, #4
	movs r0, #1
	bl 0x0200d6a8
	movs r0, #60
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #10
	bl 0x0200d6e8
	movs r0, #1
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #1
	movs r0, #2
	bl 0x0200d6c0
	movs r0, #30
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #11
	bl 0x0200d6e8
	movs r0, #2
	movs r1, #0
	bl 0x0200d6f8
	movs r2, #0
	movs r1, #3
	movs r0, #1
	bl 0x0200d6d0
	movs r0, #20
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #12
.L_02003a30:
	bl 0x0200d6e8
	movs r1, #0
	movs r0, #1
	bl 0x0200d6f8
	movs r0, #30
	bl 0x0200d628
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #80
	bl 0x0200d718
	movs r2, #0
	movs r1, #1
	movs r0, #3
	bl 0x0200d6d0
	adds r0, r5, #0
	adds r0, #13
	bl 0x0200d6e8
	movs r0, #3
	movs r1, #0
	bl 0x0200d6f8
	movs r2, #0
	movs r1, #13
	movs r0, #3
	bl 0x0200d6d0
	adds r0, r5, #0
	adds r0, #14
	bl 0x0200d6e8
	movs r0, #3
	movs r1, #0
	bl 0x0200d6f8
	movs r0, #0
	movs r1, #13
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #2
	movs r1, #13
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #1
	movs r1, #13
	movs r2, #0
	bl 0x0200d6d0
	movs r1, #13
	movs r2, #0
	movs r0, #3
	bl 0x0200d6d0
	movs r0, #120
	bl 0x0200d628
	movs r1, #129
	movs r2, #30
	movs r0, #13
	lsls r1, r1, #1
	bl 0x0200d718
	movs r1, #1
	movs r0, #13
	bl 0x0200d6c0
	movs r0, #120
	bl 0x0200d628
	movs r1, #1
	movs r0, #12
	bl 0x0200d6c0
	movs r0, #60
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #15
	bl 0x0200d6e8
	movs r0, #12
	movs r1, #0
	bl 0x0200d6f8
	movs r0, #1
	ldr r1, [pc, #408]
	movs r2, #110
	bl 0x0200d718
	movs r1, #228
	movs r2, #212
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200d680
	movs r0, #1
	bl 0x0200d698
	movs r1, #1
	movs r0, #1
	bl 0x0200d6a8
	adds r0, r5, #0
	adds r0, #16
	bl 0x0200d6e8
	movs r0, #1
	movs r1, #0
	bl 0x0200d6f8
	movs r2, #0
	movs r1, #1
	movs r0, #2
	bl 0x0200d6d0
	movs r0, #60
	bl 0x0200d628
	movs r1, #1
	movs r0, #2
	bl 0x0200d6c0
	adds r0, r5, #0
	adds r0, #17
	bl 0x0200d6e8
	movs r0, #2
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #13
	movs r2, #0
	movs r0, #1
	bl 0x0200d6d0
	movs r0, #80
	bl 0x0200d628
	movs r1, #2
	movs r2, #0
	movs r0, #1
	bl 0x0200d6d0
	movs r0, #60
	bl 0x0200d628
	movs r1, #13
	movs r2, #0
	movs r0, #1
	bl 0x0200d6d0
	movs r0, #30
	bl 0x0200d628
	movs r0, #0
	movs r1, #13
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #3
	movs r1, #13
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #13
	movs r1, #13
	movs r2, #0
	bl 0x0200d6d0
	movs r2, #0
	movs r1, #13
	movs r0, #12
	bl 0x0200d6d0
	movs r0, #80
	bl 0x0200d628
	movs r1, #3
	movs r0, #1
	bl 0x0200d6b0
	movs r0, #30
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #18
	bl 0x0200d6e8
	movs r0, #1
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #2
	movs r0, #13
	bl 0x0200d6c0
	movs r0, #70
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #19
	bl 0x0200d6e8
	movs r0, #13
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #128
	movs r2, #60
	lsls r1, r1, #1
	movs r0, #3
	bl 0x0200d718
	adds r0, r5, #0
	adds r0, #20
	bl 0x0200d6e8
	movs r0, #3
	movs r1, #0
	bl 0x0200d6f8
	movs r2, #0
	movs r1, #0
	movs r0, #2
	bl 0x0200d6d0
	movs r0, #80
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #21
	bl 0x0200d6e8
	movs r1, #0
	movs r0, #2
	bl 0x0200d6f0
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl 0x0200d6d0
	movs r1, #228
	movs r2, #152
	movs r0, #14
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200d6a0
	movs r1, #128
	movs r2, #128
	movs r0, #14
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200d658
	movs r0, #0
	movs r1, #0
	bl 0x0200d648
	cmp r0, #0
	bne .L_02003a30_0
	movs r0, #19
	bl 0x0200d7a8
	adds r0, r5, #0
	adds r0, #22
	bl 0x0200d6e8
	movs r0, #14
	movs r1, #0
	bl 0x0200d6f8
	b .L_02003a30_1
	.2byte 0x0000
	.4byte 0x00000107
.L_02003a30_0:
	movs r0, #19
	bl 0x0200d7a8
	adds r0, r5, #0
	adds r0, #23
	bl 0x0200d6e8
	movs r0, #14
	movs r1, #0
	bl 0x0200d6f8
.L_02003a30_1:
	movs r0, #0
	movs r1, #14
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #1
	movs r1, #14
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #3
	movs r1, #14
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #2
	movs r1, #14
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #12
	movs r1, #14
	movs r2, #0
	bl 0x0200d6d0
.L_02003cd0:
	movs r0, #13
	movs r1, #14
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #228
	movs r1, #1
	movs r2, #240
	movs r3, #1
	lsls r2, r2, #16
	lsls r0, r0, #17
	negs r1, r1
	bl 0x0200d738
	movs r1, #1
	movs r0, #13
	bl 0x0200d6c0
	movs r0, #60
	bl 0x0200d628
	movs r0, #8
	bl 0x0200d7a8
	ldr r5, [pc, #1016]
	adds r0, r5, #0
	bl 0x0200d6e8
	movs r0, #13
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #228
	movs r2, #140
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #14
	bl 0x0200d680
	movs r0, #14
	bl 0x0200d698
	movs r1, #220
	movs r2, #128
	movs r0, #14
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x0200d680
	movs r0, #228
	movs r1, #1
	movs r2, #224
	movs r3, #1
	lsls r2, r2, #16
	negs r1, r1
	lsls r0, r0, #17
	bl 0x0200d738
	adds r0, r5, #1
	bl 0x0200d6e8
	movs r1, #0
	movs r0, #14
	bl 0x0200d6f8
	movs r0, #14
	bl 0x0200d698
	movs r1, #208
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200d708
	movs r1, #129
	movs r2, #80
	movs r0, #13
	lsls r1, r1, #1
	bl 0x0200d718
	movs r1, #4
	movs r0, #14
	bl 0x0200d6a8
	movs r0, #89
	bl 0x0200d628
	adds r0, r5, #2
	bl 0x0200d6e8
	movs r0, #14
	movs r1, #0
	bl 0x0200d6f8
	movs r0, #13
	movs r1, #2
	bl 0x0200d6c8
	movs r1, #3
	movs r0, #14
	bl 0x0200d6b0
	adds r0, r5, #3
	bl 0x0200d6e8
	movs r0, #14
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #128
	movs r2, #80
	movs r0, #13
	lsls r1, r1, #1
	bl 0x0200d718
	movs r1, #4
	movs r0, #14
	bl 0x0200d6a8
	movs r0, #80
	bl 0x0200d628
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #14
	bl 0x0200d708
	movs r0, #20
	bl 0x0200d628
	adds r0, r5, #4
	bl 0x0200d6e8
	movs r0, #14
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #1
	movs r0, #13
	bl 0x0200d6c0
	adds r0, r5, #5
	bl 0x0200d6e8
	movs r1, #0
	movs r0, #13
	bl 0x0200d6f8
	movs r0, #30
	bl 0x0200d628
	movs r0, #14
	movs r1, #3
	bl 0x0200d6b0
	movs r2, #0
	movs r1, #13
	movs r0, #14
	bl 0x0200d6d0
	movs r0, #20
	bl 0x0200d628
	adds r0, r5, #6
	bl 0x0200d6e8
	movs r0, #14
	movs r1, #0
	bl 0x0200d6f8
	movs r0, #13
	movs r1, #1
	bl 0x0200d6c0
	movs r1, #129
	movs r2, #80
	lsls r1, r1, #1
	movs r0, #13
	bl 0x0200d718
	adds r0, r5, #7
	bl 0x0200d6e8
	movs r0, #13
	movs r1, #0
	bl 0x0200d6f8
	movs r2, #60
	ldr r1, [pc, #692]
	movs r0, #14
	bl 0x0200d718
	adds r0, r5, #0
	adds r0, #8
	bl 0x0200d6e8
	movs r0, #14
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #3
	movs r0, #14
	bl 0x0200d6b0
	adds r0, r5, #0
	adds r0, #9
	bl 0x0200d6e8
	movs r1, #0
	movs r0, #14
	bl 0x0200d6f8
	movs r0, #20
	bl 0x0200d628
	ldr r1, [pc, #640]
	movs r0, #13
	bl 0x0200d720
	movs r0, #80
	bl 0x0200d628
	movs r0, #13
	movs r1, #0
	bl 0x0200d720
	movs r1, #248
	lsls r1, r1, #1
	movs r2, #240
	movs r0, #14
	bl 0x0200d680
	movs r0, #14
	bl 0x0200d698
	movs r1, #12
	movs r2, #0
	movs r0, #14
	bl 0x0200d6d0
	movs r0, #20
	bl 0x0200d628
	movs r2, #0
	movs r1, #14
	movs r0, #12
	bl 0x0200d6d0
	adds r0, r5, #0
	adds r0, #10
	bl 0x0200d6e8
	movs r0, #14
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #1
	movs r0, #12
	bl 0x0200d6c0
	adds r0, r5, #0
	adds r0, #11
	bl 0x0200d6e8
	movs r1, #0
	movs r0, #12
	bl 0x0200d6f8
	movs r0, #40
	bl 0x0200d628
	movs r1, #3
	movs r0, #14
	bl 0x0200d6b0
	movs r0, #20
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #12
	bl 0x0200d6e8
	movs r0, #14
	movs r1, #0
	bl 0x0200d6f8
	movs r0, #14
	movs r1, #3
	bl 0x0200d6a8
	movs r1, #129
	movs r2, #60
	lsls r1, r1, #1
	movs r0, #12
	bl 0x0200d718
	adds r0, r5, #0
	adds r0, #13
	bl 0x0200d6e8
	movs r0, #12
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #128
	movs r2, #70
	lsls r1, r1, #1
	movs r0, #14
	bl 0x0200d718
	adds r0, r5, #0
	adds r0, #14
	bl 0x0200d6e8
	movs r0, #14
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #3
	movs r0, #12
	bl 0x0200d6a8
	movs r0, #140
	bl 0x0200d628
	movs r1, #3
	movs r0, #14
	bl 0x0200d6a8
	movs r0, #120
	bl 0x0200d628
	movs r2, #0
	movs r0, #0
	movs r1, #14
	bl 0x0200d6d8
	movs r0, #0
	movs r1, #3
	bl 0x0200d6a8
	movs r0, #2
	movs r1, #3
	bl 0x0200d6a8
	movs r0, #1
	movs r1, #3
	bl 0x0200d6a8
	movs r1, #3
	movs r0, #3
	bl 0x0200d6a8
	movs r0, #120
	bl 0x0200d628
	movs r1, #132
	movs r2, #180
	lsls r1, r1, #1
	movs r0, #14
	bl 0x0200d718
	adds r0, r5, #0
	adds r0, #15
	bl 0x0200d6e8
	movs r0, #14
	movs r1, #0
	bl 0x0200d6f8
	movs r2, #80
	ldr r1, [pc, #328]
	movs r0, #12
	bl 0x0200d718
	adds r0, r5, #0
	adds r0, #16
	bl 0x0200d6e8
	movs r0, #12
	movs r1, #0
	bl 0x0200d6f8
	movs r0, #0
	movs r1, #1
	movs r2, #0
	bl 0x0200d6d8
	movs r1, #2
	movs r2, #0
	movs r0, #3
	bl 0x0200d6d8
	movs r0, #80
	bl 0x0200d628
	movs r1, #0
	movs r2, #0
	movs r0, #14
	bl 0x0200d6d0
	movs r0, #60
	bl 0x0200d628
	movs r0, #0
	movs r1, #14
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #1
	movs r1, #14
	movs r2, #0
	bl 0x0200d6d0
	movs r0, #2
	movs r1, #14
	movs r2, #0
	bl 0x0200d6d0
	movs r2, #0
	movs r1, #14
	movs r0, #3
	bl 0x0200d6d0
	movs r0, #60
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #17
	bl 0x0200d6e8
	movs r0, #14
	movs r1, #0
	bl 0x0200d6f8
	movs r0, #0
	ldr r1, [pc, #200]
	movs r2, #0
	bl 0x0200d718
	movs r0, #1
	ldr r1, [pc, #188]
	movs r2, #0
	bl 0x0200d718
	movs r0, #2
	ldr r1, [pc, #180]
	movs r2, #0
	bl 0x0200d718
	movs r2, #0
	ldr r1, [pc, #168]
	movs r0, #3
	bl 0x0200d718
	movs r0, #100
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #18
	bl 0x0200d6e8
	movs r0, #1
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #1
	movs r0, #2
	bl 0x0200d6c0
	movs r0, #60
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #19
	bl 0x0200d6e8
	movs r0, #2
	movs r1, #0
	bl 0x0200d6f8
	movs r2, #0
	movs r1, #2
	movs r0, #14
	bl 0x0200d6d0
	movs r0, #20
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #20
	bl 0x0200d6e8
	movs r0, #14
	movs r1, #0
	bl 0x0200d6f8
	movs r0, #0
	ldr r1, [pc, #76]
	movs r2, #0
	bl 0x0200d718
	movs r0, #1
	ldr r1, [pc, #64]
	movs r2, #0
	bl 0x0200d718
	movs r0, #2
	ldr r1, [pc, #56]
	movs r2, #0
	bl 0x0200d718
	movs r2, #0
	ldr r1, [pc, #44]
	movs r0, #3
	bl 0x0200d718
	movs r0, #120
	bl 0x0200d628
	movs r1, #4
	movs r0, #14
	bl 0x0200d6a8
	movs r0, #120
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #21
	bl 0x0200d6e8
	movs r0, #14
	b .L_02003cd0_0
	.4byte 0x000024a6
	.4byte 0x00000103
	.4byte 0x00000101
.L_02003cd0_0:
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #129
	movs r2, #90
	lsls r1, r1, #1
	movs r0, #14
	bl 0x0200d718
	adds r0, r5, #0
	adds r0, #22
	bl 0x0200d6e8
	movs r0, #14
	movs r1, #0
	bl 0x0200d6f8
	movs r0, #0
	movs r1, #3
	bl 0x0200d6a8
	movs r0, #2
	movs r1, #3
	bl 0x0200d6a8
	movs r0, #3
	movs r1, #3
	bl 0x0200d6a8
	movs r0, #1
	movs r1, #3
	bl 0x0200d6a8
	movs r1, #3
	movs r0, #12
	bl 0x0200d6a8
	movs r0, #80
	bl 0x0200d628
	movs r1, #4
	movs r0, #14
	bl 0x0200d6a8
	movs r0, #120
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #23
	bl 0x0200d6e8
	movs r0, #14
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #128
	movs r2, #60
	lsls r1, r1, #1
	movs r0, #12
	bl 0x0200d718
	adds r0, r5, #0
	adds r0, #24
	bl 0x0200d6e8
	movs r0, #12
	movs r1, #0
	bl 0x0200d6f8
	movs r2, #0
	movs r1, #12
	movs r0, #14
	bl 0x0200d6d0
	movs r0, #20
	bl 0x0200d628
	movs r1, #1
	movs r0, #14
	bl 0x0200d6c0
	movs r0, #50
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #25
	bl 0x0200d6e8
	movs r0, #14
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #3
	movs r0, #1
	bl 0x0200d6a8
	movs r0, #70
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #26
	bl 0x0200d6e8
	movs r0, #1
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #3
	movs r0, #3
	bl 0x0200d6a8
	adds r0, r5, #0
	adds r0, #27
	bl 0x0200d6e8
	movs r0, #3
	movs r1, #0
	bl 0x0200d6f8
	movs r2, #0
	movs r0, #14
	movs r1, #0
	bl 0x0200d6d0
	movs r1, #3
	movs r0, #14
	bl 0x0200d6b0
	movs r0, #20
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #28
	bl 0x0200d6e8
	movs r0, #14
	movs r1, #0
	bl 0x0200d6f8
	movs r2, #90
	ldr r1, [pc, #1000]
	movs r0, #2
	bl 0x0200d718
	adds r0, r5, #0
	adds r0, #29
	bl 0x0200d6e8
	movs r0, #2
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #14
	bl 0x0200d708
	movs r0, #20
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #30
	bl 0x0200d6e8
	movs r0, #14
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #1
	movs r0, #12
	bl 0x0200d6c0
	movs r0, #60
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #31
	bl 0x0200d6e8
	movs r0, #12
	movs r1, #0
	bl 0x0200d6f8
	movs r2, #0
	movs r1, #12
	movs r0, #14
	bl 0x0200d6d0
	movs r0, #20
	bl 0x0200d628
	movs r1, #3
	movs r0, #14
	bl 0x0200d6b0
	movs r0, #20
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #32
	bl 0x0200d6e8
	movs r0, #14
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #128
	movs r2, #70
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200d718
	adds r0, r5, #0
	adds r0, #33
	bl 0x0200d6e8
	movs r0, #1
	movs r1, #0
	bl 0x0200d6f8
	movs r2, #0
	movs r1, #0
	movs r0, #14
	bl 0x0200d6d0
	movs r0, #30
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #34
	bl 0x0200d6e8
	movs r0, #14
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #128
	movs r2, #80
	lsls r1, r1, #1
	movs r0, #3
	bl 0x0200d718
	adds r0, r5, #0
	adds r0, #35
	bl 0x0200d6e8
	movs r0, #3
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #3
	movs r0, #14
	bl 0x0200d6b0
	adds r0, r5, #0
	adds r0, #36
	bl 0x0200d6e8
	movs r0, #14
	movs r1, #0
	bl 0x0200d6f8
	movs r2, #0
	movs r1, #0
	movs r0, #2
	bl 0x0200d6d0
	movs r0, #30
	bl 0x0200d628
	movs r1, #3
	movs r0, #2
	bl 0x0200d6a8
	movs r0, #100
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #37
	bl 0x0200d6e8
	movs r1, #0
	movs r0, #2
	bl 0x0200d6f0
	movs r0, #0
	movs r1, #0
	bl 0x0200d648
	cmp r0, #0
	beq .L_02003cd0_1
	adds r0, r5, #0
	adds r0, #38
	bl 0x0200d6e8
	movs r0, #14
	movs r1, #0
	bl 0x0200d6f8
.L_02003cd0_1:
	movs r0, #30
	bl 0x0200d628
	movs r0, #1
	movs r1, #3
	bl 0x0200d6a8
	movs r0, #2
	movs r1, #3
	bl 0x0200d6a8
	movs r1, #3
	movs r0, #3
	bl 0x0200d6a8
	movs r0, #100
	bl 0x0200d628
	movs r2, #0
	movs r0, #0
	movs r1, #12
	bl 0x0200d6d8
	movs r0, #0
	movs r1, #3
	bl 0x0200d6a8
	movs r1, #3
	movs r0, #12
	bl 0x0200d6a8
	movs r0, #100
	bl 0x0200d628
	movs r0, #1
	movs r1, #2
	bl 0x0200d6a8
	movs r0, #0
	bl 0x0200d650
	cmp r0, #0
	beq .L_02003cd0_2
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x0200d678
.L_02003cd0_2:
	movs r0, #1
	bl 0x0200d698
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200d6a0
	movs r0, #2
	movs r1, #2
	bl 0x0200d6a8
	movs r0, #0
	bl 0x0200d650
	cmp r0, #0
	beq .L_02003cd0_3
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #2
	bl 0x0200d678
.L_02003cd0_3:
	movs r0, #2
	bl 0x0200d698
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x0200d6a0
	movs r0, #3
	movs r1, #2
	bl 0x0200d6a8
	movs r0, #0
	bl 0x0200d650
	cmp r0, #0
	beq .L_02003cd0_4
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #3
	bl 0x0200d678
.L_02003cd0_4:
	movs r0, #3
	bl 0x0200d698
	movs r1, #0
	movs r2, #0
	movs r0, #3
	bl 0x0200d6a0
	movs r0, #30
	bl 0x0200d628
	movs r0, #12
	ldr r1, [pc, #464]
	ldr r2, [pc, #468]
	bl 0x0200d658
	movs r1, #236
	movs r2, #184
	lsls r1, r1, #1
	movs r0, #12
	bl 0x0200d680
	movs r0, #12
	bl 0x0200d698
	movs r0, #12
	movs r1, #1
	bl 0x0200d6a8
	movs r1, #192
	movs r0, #12
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200d708
	movs r2, #0
	movs r0, #14
	movs r1, #0
	bl 0x0200d6d0
	movs r1, #3
	movs r0, #14
	bl 0x0200d6a8
	movs r0, #20
	bl 0x0200d628
	movs r2, #0
	movs r1, #13
	movs r0, #14
	bl 0x0200d6d0
	movs r0, #20
	bl 0x0200d628
	adds r0, r5, #0
	adds r0, #39
	bl 0x0200d6e8
	movs r0, #14
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #2
	movs r0, #13
	bl 0x0200d6c8
	adds r0, r5, #0
	adds r0, #40
	bl 0x0200d6e8
	movs r0, #13
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #3
	movs r0, #0
	bl 0x0200d6a8
	movs r0, #30
	bl 0x0200d628
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200d658
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #12
	lsls r1, r1, #9
	bl 0x0200d658
	movs r0, #0
	movs r1, #1
	bl 0x0200d728
	movs r1, #240
	movs r2, #248
	lsls r1, r1, #1
	movs r0, #0
	bl 0x0200d680
	movs r0, #40
	bl 0x0200d628
	movs r0, #12
	movs r1, #0
	bl 0x0200d710
	movs r0, #0
	movs r1, #0
	bl 0x0200d710
	movs r1, #240
	lsls r1, r1, #1
	movs r2, #216
	movs r0, #12
	bl 0x0200d680
	movs r0, #0
	bl 0x0200d698
	movs r1, #14
	movs r2, #0
	movs r0, #0
	bl 0x0200d6d0
	movs r0, #12
	bl 0x0200d698
	movs r0, #12
	movs r1, #14
	movs r2, #0
	bl 0x0200d6d0
	movs r1, #192
	movs r2, #0
	movs r0, #12
	lsls r1, r1, #6
	bl 0x0200d708
	movs r1, #3
	movs r0, #0
	bl 0x0200d6a8
	movs r0, #5
	bl 0x0200d628
	movs r1, #3
	movs r0, #12
	bl 0x0200d6a8
	movs r0, #100
	bl 0x0200d628
	movs r1, #3
	movs r0, #14
	bl 0x0200d6a8
	movs r0, #100
	bl 0x0200d628
	movs r1, #240
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #248
	bl 0x0200d680
	movs r1, #228
	lsls r1, r1, #1
	movs r2, #248
	movs r0, #0
	bl 0x0200d680
	movs r0, #12
	bl 0x0200d698
	movs r1, #228
	lsls r1, r1, #1
	movs r2, #248
	movs r0, #12
	bl 0x0200d680
	movs r0, #0
	bl 0x0200d698
	movs r1, #228
	movs r2, #180
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #0
	bl 0x0200d680
	movs r0, #12
	bl 0x0200d698
	movs r1, #228
	movs r2, #180
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #12
	bl 0x0200d680
	movs r0, #0
	bl 0x0200d698
	movs r1, #176
	movs r2, #180
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #0
	bl 0x0200d680
	movs r0, #12
.L_020045d8:
	bl 0x0200d698
	movs r1, #176
	movs r2, #180
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #12
	bl 0x0200d680
	movs r0, #20
	bl 0x0200d628
	movs r0, #17
	bl 0x0200d7a8
	ldr r3, [pc, #32]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #67
	str r2, [r3]
	bl 0x0200d778
	movs r0, #1
	b .L_020045d8_0
	.2byte 0x0101
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.2byte 0x3333
	.2byte 0x0000
	.4byte 0x03001ebc
.L_020045d8_0:
	bl 0x0200d628
	movs r0, #210
	bl 0x0200d628
	movs r0, #4
	bl 0x0200d758
	bl 0x0200d638
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.global Func_02004638
	.thumb_func
Func_02004638:
	push {lr}
	ldr r3, [pc, #160]
	ldr r3, [r3]
	cmp r3, #7
	bhi .L_02004638_0
	ldr r2, [pc, #156]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	movs r0, r0
	.2byte 0xc66c
	lsls r0, r0, #8
	.2byte 0xc670
	lsls r0, r0, #8
	.2byte 0xc674
	lsls r0, r0, #8
	.2byte 0xc678
	lsls r0, r0, #8
	.2byte 0xc67c
	lsls r0, r0, #8
	stmia r6!, {r2, r4, r7}
	lsls r0, r0, #8
	stmia r6!, {r7}
	lsls r0, r0, #8
	stmia r6!, {r2, r7}
	lsls r0, r0, #8
	ldr r0, [pc, #116]
	b .L_02004638_1
	.2byte 0x481d
	.2byte 0xe008
	.2byte 0x481d
	.2byte 0xe006
	.2byte 0x481d
	.2byte 0xe004
	.2byte 0x481d
	.2byte 0xe002
	.2byte 0x481d
	.2byte 0xe000
	.2byte 0x481d
.L_02004638_1:
	bl 0x0200d6e8
	movs r0, #21
	movs r1, #0
	bl 0x0200d6f8
	b .L_02004638_0
	.2byte 0x21d0
	.2byte 0x0209
	.2byte 0x2200
	.2byte 0x2015
	.2byte 0xf001
	.2byte 0xf834
	.2byte 0x2032
	.2byte 0xf000
	.2byte 0xffc1
	.2byte 0x21b0
	.2byte 0x0209
	.2byte 0x2200
	.2byte 0x2015
	.2byte 0xf001
	.2byte 0xf82b
	.2byte 0x2032
	.2byte 0xf000
	.2byte 0xffb8
	.2byte 0x21a0
	.2byte 0x01c9
	.2byte 0x2200
	.2byte 0x2015
	.2byte 0xf001
	.2byte 0xf822
	.2byte 0x2032
	.2byte 0xf000
	.2byte 0xffaf
	.2byte 0x480d
	.2byte 0xf001
	.2byte 0xf80c
	.2byte 0x2015
	.2byte 0x2100
	.2byte 0xf001
	.2byte 0xf810
.L_02004638_0:
	pop {r0}
	bx r0
	.4byte 0x0200dfa4
	.4byte 0x0200c64c
	.4byte 0x00002414
	.2byte 0x2415
	.2byte 0x0000
	.2byte 0x2416
	.2byte 0x0000
	.2byte 0x2417
	.2byte 0x0000
	.2byte 0x2418
	.2byte 0x0000
	.2byte 0x241a
	.2byte 0x0000
	.2byte 0x241b
	.2byte 0x0000
	.2byte 0x2419
	.2byte 0x0000
	.global Func_02004704
	.thumb_func
Func_02004704:
	push {r5, lr}
	movs r0, #21
	ldr r1, [pc, #128]
	movs r2, #30
	bl 0x0200d718
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #21
	bl 0x0200d708
	movs r0, #50
	bl 0x0200d628
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #21
	bl 0x0200d708
	movs r0, #50
	bl 0x0200d628
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #21
	bl 0x0200d708
	movs r0, #50
	bl 0x0200d628
	ldr r5, [pc, #72]
	adds r0, r5, #0
	bl 0x0200d6e8
	movs r0, #21
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #4
	movs r0, #21
	bl 0x0200d6a8
	movs r0, #60
	bl 0x0200d628
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #21
	bl 0x0200d708
	adds r5, #1
	movs r0, #40
	bl 0x0200d628
	adds r0, r5, #0
	bl 0x0200d6e8
	movs r0, #21
	movs r1, #0
	bl 0x0200d6f8
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00000101
	.4byte 0x00002411
	.global Func_02004794
	.thumb_func
Func_02004794:
	push {r5, lr}
	bl 0x0200d630
	ldr r0, [pc, #364]
	bl 0x0200d610
	cmp r0, #0
	beq .L_02004794_0
	ldr r0, [pc, #356]
	bl 0x0200d6e8
	movs r0, #18
	movs r1, #0
	bl 0x0200d6f8
	bl 0x0200d638
	b 0x0200c902
.L_02004794_0:
	ldr r0, [pc, #340]
	bl 0x0200d610
	cmp r0, #0
	beq 0x0200c7d6
.L_020047c2:
	ldr r0, [pc, #336]
	bl 0x0200d6e8
	movs r1, #0
	movs r0, #25
	bl 0x0200d6f0
	bl 0x0200d638
	b .L_020047c2_0
	.2byte 0x2181
	.2byte 0x2019
	.2byte 0x0049
	.2byte 0x221e
	.2byte 0xf000
	.2byte 0xff9b
	.2byte 0x2200
	.2byte 0x2100
	.2byte 0x2019
	.2byte 0xf000
	.2byte 0xff72
	.2byte 0x4d4a
	.2byte 0x1c28
	.2byte 0xf000
	.2byte 0xff7a
	.2byte 0x2019
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xff7e
	.2byte 0x2200
	.2byte 0x2019
	.2byte 0x2118
	.2byte 0xf000
	.2byte 0xff65
	.2byte 0x2101
	.2byte 0x2018
	.2byte 0xf000
	.2byte 0xff9d
	.2byte 0xf000
	.2byte 0xff97
	.2byte 0x203c
	.2byte 0xf000
	.2byte 0xff08
	.2byte 0x2101
	.2byte 0x2000
	.2byte 0xf000
	.2byte 0xff94
	.2byte 0x2014
	.2byte 0xf000
	.2byte 0xff01
	.2byte 0x223c
	.2byte 0x493c
	.2byte 0x2019
	.2byte 0xf000
	.2byte 0xff74
	.2byte 0x1c68
	.2byte 0xf000
	.2byte 0xff59
	.2byte 0x2019
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xff5d
	.2byte 0x223c
	.2byte 0x4937
	.2byte 0x2019
	.2byte 0xf000
	.2byte 0xff68
	.2byte 0x1ca8
	.2byte 0xf000
	.2byte 0xff4d
	.2byte 0x2100
	.2byte 0x2019
	.2byte 0xf000
	.2byte 0xff51
	.2byte 0x2046
	.2byte 0xf000
	.2byte 0xfee6
	.2byte 0x2180
	.2byte 0x2019
	.2byte 0x0049
	.2byte 0x223c
	.2byte 0xf000
	.2byte 0xff58
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0x2019
	.2byte 0xf000
	.2byte 0xff2f
	.2byte 0x1ce8
	.2byte 0xf000
	.2byte 0xff38
	.2byte 0x2100
	.2byte 0x2019
	.2byte 0xf000
	.2byte 0xff38
	.2byte 0x2000
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xfee0
	.2byte 0x2800
	.2byte 0xd107
	.2byte 0x1d28
	.2byte 0xf000
	.2byte 0xff2b
	.2byte 0x2019
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xff2b
	.2byte 0xe006
	.2byte 0x1d68
	.2byte 0xf000
	.2byte 0xff23
	.2byte 0x2019
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xff23
	.2byte 0x203c
	.2byte 0xf000
	.2byte 0xfebc
	.2byte 0x223c
	.2byte 0x491a
	.2byte 0x2019
	.2byte 0xf000
	.2byte 0xff2f
	.2byte 0x4d1a
	.2byte 0x1c28
	.2byte 0xf000
	.2byte 0xff13
	.2byte 0x2100
	.2byte 0x2019
	.2byte 0xf000
	.2byte 0xff13
	.2byte 0x2101
	.2byte 0x2019
	.2byte 0xf000
	.2byte 0xfefb
	.2byte 0x1c68
	.2byte 0xf000
	.2byte 0xff08
	.2byte 0x2100
	.2byte 0x2019
	.2byte 0xf000
	.2byte 0xff08
	.2byte 0x3502
	.2byte 0x2103
	.2byte 0x2019
	.2byte 0xf000
	.2byte 0xfee3
	.2byte 0x1c28
	.2byte 0xf000
	.2byte 0xfefc
	.2byte 0x2100
	.2byte 0x2019
	.2byte 0xf000
	.2byte 0xfefc
	.2byte 0x4805
	.2byte 0xf000
	.2byte 0xfe8d
	.2byte 0xf000
	.2byte 0xfe9b
.L_020047c2_0:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0941
	.2byte 0x0000
	.2byte 0x2566
	.2byte 0x0000
	.2byte 0x0313
	.2byte 0x0000
	.4byte 0x00002457
	.2byte 0x244f
	.2byte 0x0000
	.2byte 0x0105
	.2byte 0x0000
	.2byte 0x0107
	.2byte 0x0000
	.2byte 0x2455
	.2byte 0x0000
	.global Func_02004928
	.thumb_func
Func_02004928:
	push {lr}
	ldr r0, [pc, #44]
	bl 0x0200d610
	cmp r0, #0
	beq .L_02004928_0
	ldr r0, [pc, #36]
	bl 0x0200d6e8
	movs r0, #25
	movs r1, #0
	bl 0x0200d6f8
	b .L_02004928_1
.L_02004928_0:
	ldr r0, [pc, #24]
	bl 0x0200d6e8
	movs r0, #25
	movs r1, #0
	bl 0x0200d6f8
.L_02004928_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000941
	.4byte 0x00002568
	.4byte 0x00002458
	.global Func_02004964
	.thumb_func
Func_02004964:
	push {lr}
	ldr r0, [pc, #44]
	bl 0x0200d610
	cmp r0, #0
	beq .L_02004964_0
	ldr r0, [pc, #36]
	bl 0x0200d6e8
	movs r0, #24
	movs r1, #0
	bl 0x0200d6f8
	b .L_02004964_1
.L_02004964_0:
	ldr r0, [pc, #24]
	bl 0x0200d6e8
	movs r0, #24
	movs r1, #0
	bl 0x0200d6f8
.L_02004964_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000941
	.4byte 0x00002569
	.4byte 0x0000244e
	.global Func_020049a0
	.thumb_func
Func_020049a0:
	push {r5, lr}
	ldr r0, [pc, #416]
	bl 0x0200d610
	cmp r0, #0
	bne .L_020049a0_0
	b .L_020049a0_1
.L_020049a0_0:
	ldr r0, [pc, #408]
	bl 0x0200d610
	cmp r0, #0
	beq .L_020049a0_2
	b .L_020049a0_3
.L_020049a0_2:
	movs r0, #197
	lsls r0, r0, #2
	bl 0x0200d610
	cmp r0, #0
	beq .L_020049a0_4
	b .L_020049a0_3
.L_020049a0_4:
	ldr r5, [pc, #384]
	adds r0, r5, #0
	bl 0x0200d6e8
	movs r0, #24
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #1
	movs r0, #24
	bl 0x0200d6c8
	movs r0, #30
	bl 0x0200d628
	movs r0, #24
	ldr r1, [pc, #356]
	ldr r2, [pc, #360]
	bl 0x0200d658
	movs r1, #4
	movs r2, #0
	negs r1, r1
	movs r0, #24
	bl 0x0200d690
	movs r0, #24
	bl 0x0200d698
	movs r1, #3
	movs r0, #24
	bl 0x0200d6a8
	movs r0, #60
	bl 0x0200d628
	movs r0, #24
	ldr r1, [pc, #324]
	ldr r2, [pc, #312]
	bl 0x0200d658
	movs r1, #6
	movs r0, #24
	negs r1, r1
	movs r2, #0
	bl 0x0200d690
	movs r2, #0
	movs r1, #0
	movs r0, #24
	bl 0x0200d6d0
	movs r0, #24
	bl 0x0200d698
	adds r0, r5, #1
	bl 0x0200d6e8
	movs r0, #24
	movs r1, #0
	bl 0x0200d6f8
	movs r0, #24
	movs r1, #1
	bl 0x0200d6c8
	movs r2, #0
	movs r1, #24
	movs r0, #25
	bl 0x0200d6d0
	adds r0, r5, #2
	bl 0x0200d6e8
	movs r1, #0
	movs r0, #24
	bl 0x0200d6f8
	movs r0, #70
	bl 0x0200d628
	movs r1, #3
	movs r0, #25
	bl 0x0200d6a8
	movs r0, #60
	bl 0x0200d628
	movs r0, #25
	ldr r1, [pc, #224]
	ldr r2, [pc, #216]
	bl 0x0200d658
	movs r1, #220
	lsls r1, r1, #2
	movs r2, #112
	movs r0, #25
	bl 0x0200d680
	movs r0, #25
	bl 0x0200d698
	movs r1, #208
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #25
	bl 0x0200d708
	adds r0, r5, #3
	bl 0x0200d6e8
	movs r0, #24
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #3
	movs r0, #24
	bl 0x0200d6a8
	movs r0, #70
	bl 0x0200d628
	movs r2, #0
	movs r1, #8
	movs r0, #24
	bl 0x0200d690
	movs r0, #24
	bl 0x0200d698
	adds r5, #4
	movs r1, #5
	movs r0, #24
	bl 0x0200d6a8
	adds r0, r5, #0
	bl 0x0200d6e8
	movs r0, #24
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #224
	lsls r1, r1, #2
	movs r2, #120
	movs r0, #0
	bl 0x0200d680
	movs r0, #0
	bl 0x0200d698
	movs r2, #0
	movs r1, #25
	movs r0, #0
	bl 0x0200d6d8
	movs r0, #60
	bl 0x0200d628
	movs r0, #25
	movs r1, #3
	bl 0x0200d6a8
	movs r0, #30
	bl 0x0200d628
	movs r0, #197
	lsls r0, r0, #2
	bl 0x0200d618
	b .L_020049a0_5
.L_020049a0_3:
	ldr r0, [pc, #64]
	bl 0x0200d6e8
	movs r0, #24
	movs r1, #0
	bl 0x0200d6f8
	b .L_020049a0_5
.L_020049a0_1:
	ldr r0, [pc, #52]
	bl 0x0200d6e8
	movs r0, #24
	movs r1, #0
	bl 0x0200d6f8
.L_020049a0_5:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000941
	.4byte 0x0000094e
	.4byte 0x00002561
	.4byte 0x00001999
	.4byte 0x00000ccc
	.4byte 0x00003333
	.4byte 0x00006666
	.4byte 0x00002567
	.4byte 0x0000244d
	.global Func_02004b68
	.thumb_func
Func_02004b68:
	push {lr}
	movs r0, #26
	movs r1, #1
	movs r2, #5
	bl 0x0200d798
	ldr r0, [pc, #8]
	bl 0x0200d618
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000094e
	.global Func_02004b84
	.thumb_func
Func_02004b84:
	push {lr}
	movs r1, #2
	movs r0, #14
	bl 0x0200d6c8
	ldr r0, [pc, #16]
	bl 0x0200d6e8
	movs r0, #14
	movs r1, #0
	bl 0x0200d6f8
	pop {r0}
	bx r0
	.4byte 0x00002441
	.global Func_02004ba4
	.thumb_func
Func_02004ba4:
	push {lr}
	movs r1, #2
	movs r0, #13
	bl 0x0200d6c8
	ldr r0, [pc, #16]
	bl 0x0200d6e8
	movs r0, #13
	movs r1, #0
	bl 0x0200d6f8
	pop {r0}
	bx r0
	.4byte 0x00002440
	.global Func_02004bc4
	.thumb_func
Func_02004bc4:
	push {lr}
	movs r1, #2
	movs r0, #12
	bl 0x0200d6c8
	ldr r0, [pc, #16]
	bl 0x0200d6e8
	movs r0, #12
	movs r1, #0
	bl 0x0200d6f8
	pop {r0}
	bx r0
	.4byte 0x0000243f
	.global Func_02004be4
	.thumb_func
Func_02004be4:
	push {lr}
	ldr r0, [pc, #16]
	bl 0x0200d6e8
	movs r1, #0
	movs r0, #18
	bl 0x0200d700
	pop {r0}
	bx r0
	.4byte 0x00002459
	.global Func_02004bfc
	.thumb_func
Func_02004bfc:
	push {r5, lr}
	ldr r0, [pc, #244]
	bl 0x0200d610
	cmp r0, #0
	beq .L_02004bfc_0
	ldr r0, [pc, #236]
	bl 0x0200d6e8
	movs r0, #20
	movs r1, #0
	bl 0x0200d6f8
	b .L_02004bfc_1
.L_02004bfc_0:
	bl 0x0200d630
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl 0x0200d6d0
	ldr r0, [pc, #212]
	bl 0x0200d610
	cmp r0, #0
	bne .L_02004bfc_2
	movs r2, #0
	movs r1, #4
	movs r0, #20
	bl 0x0200d6b8
	movs r0, #20
	bl 0x0200d670
	movs r0, #20
	bl 0x0200d668
	movs r0, #20
	bl 0x0200d628
	ldr r5, [pc, #176]
	adds r0, r5, #0
	bl 0x0200d6e8
	movs r0, #20
	movs r1, #0
	bl 0x0200d6f8
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #20
	adds r5, #1
	bl 0x0200d718
	adds r0, r5, #0
	bl 0x0200d6e8
	movs r1, #0
	movs r0, #20
	bl 0x0200d6f8
	movs r0, #30
	bl 0x0200d628
	movs r0, #20
	movs r1, #4
	bl 0x0200d6a8
	movs r0, #30
	bl 0x0200d628
.L_02004bfc_2:
	ldr r5, [pc, #116]
	adds r0, r5, #0
	bl 0x0200d6e8
	movs r0, #20
	movs r1, #0
	bl 0x0200d6f8
	ldr r1, [pc, #104]
	movs r2, #40
	movs r0, #20
	bl 0x0200d718
	adds r0, r5, #1
	bl 0x0200d6e8
	movs r1, #0
	movs r0, #20
	bl 0x0200d6f0
	movs r0, #0
	movs r1, #0
	bl 0x0200d648
	cmp r0, #0
	bne .L_02004bfc_3
	adds r0, r5, #2
	bl 0x0200d6e8
	movs r0, #20
	movs r1, #0
	bl 0x0200d6f0
	ldr r0, [pc, #36]
	bl 0x0200d618
	b .L_02004bfc_4
.L_02004bfc_3:
	adds r0, r5, #3
	bl 0x0200d6e8
	movs r0, #20
	movs r1, #0
	bl 0x0200d6f0
.L_02004bfc_4:
	ldr r0, [pc, #20]
	bl 0x0200d618
	bl 0x0200d638
.L_02004bfc_1:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00000226
	.4byte 0x00002434
	.4byte 0x00000227
	.4byte 0x0000242e
	.4byte 0x00002430
	.4byte 0x00000101
	.global Func_02004d0c
	.thumb_func
Func_02004d0c:
	push {lr}
	ldr r0, [pc, #48]
	bl 0x0200d610
	cmp r0, #0
	beq .L_02004d0c_0
	ldr r0, [pc, #40]
	bl 0x0200d6e8
	movs r0, #20
	movs r1, #0
	bl 0x0200d6f8
	b 0x0200cd3c
.L_02004d0c_0:
	ldr r3, [pc, #28]
.L_02004d2a:
	movs r2, #191
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	strh r0, [r3]
	bl 0x0200d790
	bl 0x0200cbfc
	pop {r0}
	bx r0
	.2byte 0x0226
	.2byte 0x0000
	.2byte 0x2435
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.global Func_02004d4c
	.thumb_func
Func_02004d4c:
	bx lr
	.2byte 0x0000
	.global Func_02004d50
	.thumb_func
Func_02004d50:
	push {r5, lr}
	ldr r5, [pc, #44]
	adds r0, r5, #0
	bl 0x0200d6e8
	ldr r0, [pc, #40]
	movs r1, #0
	bl 0x0200d6f8
	movs r0, #234
	bl 0x0200d608
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	beq .L_02004d50_0
	adds r0, r5, #2
	movs r1, #1
	bl 0x0200d600
.L_02004d50_0:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000256c
	.4byte 0x0000800d
	.global Func_02004d88
	.thumb_func
Func_02004d88:
	push {lr}
	ldr r0, [pc, #16]
	bl 0x0200d6e8
	movs r0, #13
	movs r1, #0
	bl 0x0200d6f8
	pop {r0}
	bx r0
	.4byte 0x0000256d
	.global Func_02004da0
	.thumb_func
Func_02004da0:
	push {r5, r6, r7, lr}
	ldr r5, [pc, #408]
	bl 0x0200d5a0
	lsls r3, r0, #3
	subs r3, r3, r0
	lsrs r3, r3, #16
	ldr r7, [pc, #400]
	movs r1, #224
	str r3, [r5]
	lsls r1, r1, #1
	adds r5, r7, r1
	movs r3, #0
	ldrsh r2, [r5, r3]
	ldr r3, [pc, #388]
	ldrh r1, [r5]
	cmp r2, r3
	bne .L_02004da0_0
	movs r0, #224
	lsls r0, r0, #4
	bl 0x0200d5f8
	bl 0x0200cf60
	ldrh r1, [r5]
.L_02004da0_0:
	lsls r3, r1, #16
	ldr r2, [pc, #368]
	asrs r3, r3, #16
	cmp r3, r2
	bne .L_02004da0_1
	bl 0x0200d0e4
	ldrh r1, [r5]
.L_02004da0_1:
	lsls r3, r1, #16
	ldr r2, [pc, #356]
	asrs r3, r3, #16
	cmp r3, r2
	bne .L_02004da0_2
	bl 0x0200d324
	ldrh r1, [r5]
.L_02004da0_2:
	lsls r3, r1, #16
	ldr r2, [pc, #344]
	asrs r3, r3, #16
	cmp r3, r2
	beq .L_02004da0_3
	b 0x0200cf32
.L_02004da0_3:
	ldr r3, [pc, #340]
	movs r1, #224
	ldr r3, [r3]
	lsls r1, r1, #1
	movs r2, #129
	adds r3, r3, r1
	lsls r2, r2, #2
	str r2, [r3]
	movs r0, #12
	bl 0x0200d650
	movs r1, #0
	bl 0x0200d5e8
	movs r2, #0
	movs r0, #12
	movs r1, #0
	bl 0x0200d708
	movs r0, #12
	movs r1, #0
	bl 0x0200d6a8
	bl 0x0200d520
	movs r0, #8
	bl 0x0200d650
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02004da0_4
	movs r1, #0
	bl 0x0200d5e8
.L_02004da0_4:
	adds r3, r5, #0
	adds r3, #35
	movs r6, #2
	strb r6, [r3]
	movs r0, #9
	bl 0x0200d650
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02004da0_5
	movs r1, #0
	bl 0x0200d5e8
.L_02004da0_5:
	adds r3, r5, #0
	adds r3, #35
	strb r6, [r3]
	movs r0, #10
	bl 0x0200d650
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02004da0_6
	movs r1, #0
	bl 0x0200d5e8
.L_02004da0_6:
	adds r3, r5, #0
	adds r3, #35
	movs r0, #224
	strb r6, [r3]
	lsls r0, r0, #4
	bl 0x0200d5f8
	movs r2, #225
	lsls r2, r2, #1
	adds r5, r7, r2
	movs r1, #0
	ldrsh r3, [r5, r1]
	ldrh r2, [r5]
	cmp r3, #4
	bne .L_02004da0_7
	movs r0, #192
	lsls r0, r0, #4
	bl 0x0200d5f8
	bl 0x0200b054
	ldrh r2, [r5]
.L_02004da0_7:
	lsls r3, r2, #16
	movs r2, #192
	lsls r2, r2, #10
	cmp r3, r2
	bne 0x0200cf14
	movs r0, #192
	lsls r0, r0, #4
	bl 0x0200d5f8
	ldr r0, [pc, #164]
	bl 0x0200d610
	cmp r0, #0
	beq 0x0200cf14
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl 0x0200d6a0
	movs r1, #216
	movs r2, #172
	movs r0, #16
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200d6a0
	movs r1, #160
	movs r0, #16
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200d708
	movs r1, #228
	movs r2, #144
	movs r0, #13
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200d6a0
	movs r1, #160
	movs r0, #13
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200d708
	movs r1, #228
	movs r2, #160
	lsls r1, r1, #17
	movs r0, #17
.L_02004f02:
	lsls r2, r2, #17
	bl 0x0200d6a0
	movs r0, #17
	bl 0x0200d650
	movs r1, #0
	bl 0x0200d5e8
	movs r0, #15
	bl 0x0200d650
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02004f02_0
	movs r1, #0
	bl 0x0200d5e8
.L_02004f02_0:
	adds r2, r5, #0
	movs r3, #2
	adds r2, #35
	strb r3, [r2]
	ldr r3, [pc, #44]
	str r3, [r5, #24]
	movs r0, #0
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.2byte 0xdfa4
	.2byte 0x0200
	.2byte 0x0240
	.2byte 0x0200
	.2byte 0x00a0
	.2byte 0x0000
	.2byte 0x00a1
	.2byte 0x0000
	.2byte 0x00a2
	.2byte 0x0000
	.2byte 0x00a3
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x0941
	.2byte 0x0000
	.4byte 0x0000cccc
	.global Func_02004f60
	.thumb_func
Func_02004f60:
	push {lr}
	sub sp, #8
	bl 0x0200d470
	ldr r3, [pc, #340]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #2
	cmp r3, #17
	bhi .L_02004f60_0
	ldr r2, [pc, #328]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	movs r0, r0
	.2byte 0xcfcc
	lsls r0, r0, #8
	.2byte 0xcfcc
	lsls r0, r0, #8
	.2byte 0xcfcc
	lsls r0, r0, #8
	.2byte 0xcfcc
	lsls r0, r0, #8
	.2byte 0xcfcc
	lsls r0, r0, #8
	.2byte 0xcfcc
	lsls r0, r0, #8
	beq .L_02004f60_1
	lsls r0, r0, #8
	beq .L_02004f60_2
	lsls r0, r0, #8
	beq .L_02004f60_3
	lsls r0, r0, #8
	beq .L_02004f60_4
	lsls r0, r0, #8
	.2byte 0xcffa
	lsls r0, r0, #8
	beq .L_02004f60_5
	lsls r0, r0, #8
	beq .L_02004f60_6
	lsls r0, r0, #8
	beq .L_02004f60_7
	lsls r0, r0, #8
	beq .L_02004f60_8
	lsls r0, r0, #8
	beq .L_02004f60_9
	lsls r0, r0, #8
	beq .L_02004f60_10
	lsls r0, r0, #8
	.2byte 0xcffa
	lsls r0, r0, #8
	ldr r3, [pc, #248]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #200
.L_02004f60_8:
	adds r2, #64
	str r2, [r3]
.L_02004f60_9:
	lsls r1, r1, #4
	ldr r0, [pc, #236]
.L_02004f60_10:
	bl 0x0200d590
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #228]
	bl 0x0200d590
	movs r1, #200
	ldr r0, [pc, #224]
	lsls r1, r1, #4
	bl 0x0200d590
	b .L_02004f60_11
	.2byte 0x4b33
	.2byte 0x22e0
	.2byte 0x681b
	.2byte 0x0052
	.2byte 0x189b
	.2byte 0x20c0
	.2byte 0x3249
	.2byte 0x601a
	.2byte 0xe023
	.2byte 0x4b2e
	.2byte 0x22e0
	.2byte 0x681b
	.2byte 0x0052
	.2byte 0x189b
	.2byte 0x21c8
	.2byte 0x3240
	.2byte 0x601a
	.2byte 0x0109
	.2byte 0x482e
	.2byte 0xf000
	.2byte 0xfab6
	.2byte 0x21c8
	.2byte 0x0109
	.2byte 0x482c
	.2byte 0xf000
	.2byte 0xfab1
	.2byte 0x2001
	.2byte 0xf000
	.2byte 0xfaaa
	.2byte 0xf000
	.2byte 0xfac0
	.2byte 0x2001
	.2byte 0xf000
	.2byte 0xfaa5
	.2byte 0x236e
	.2byte 0x2209
	.2byte 0x9300
	.2byte 0x9201
	.2byte 0x2065
	.2byte 0x2109
	.2byte 0x220a
	.2byte 0x2308
	.2byte 0xf000
	.2byte 0xfabf
.L_02004f60_11:
	movs r0, #224
	lsls r0, r0, #4
	bl 0x0200d5f8
	b .L_02004f60_1
	.2byte 0x4b1a
	.2byte 0x22e0
	.2byte 0x681b
	.2byte 0x0052
	.2byte 0x189b
	.2byte 0x21c8
	.2byte 0x3240
	.2byte 0x601a
.L_02004f60_5:
	ldr r0, [pc, #112]
	lsls r1, r1, #4
.L_02004f60_6:
	bl 0x0200d590
.L_02004f60_7:
	b .L_02004f60_1
.L_02004f60_0:
	ldr r3, [pc, #80]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #224
	adds r2, #64
	str r2, [r3]
	lsls r0, r0, #4
	bl 0x0200d5f8
.L_02004f60_1:
	movs r0, #18
	movs r1, #1
.L_02004f60_2:
	bl 0x0200d6e0
.L_02004f60_3:
	movs r0, #17
	movs r1, #1
.L_02004f60_4:
	bl 0x0200d6e0
	movs r0, #21
	movs r1, #1
	bl 0x0200d6e0
	movs r0, #12
	movs r1, #1
	bl 0x0200d6e0
	movs r1, #1
	movs r0, #13
	bl 0x0200d6e0
	movs r0, #1
	bl 0x0200d588
	sub sp, #-8
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x0200cf84
	.4byte 0x03001ebc
	.4byte 0x02009719
	.4byte 0x0200975d
	.4byte 0x020097bd
	.2byte 0x9529
	.2byte 0x0200
	.2byte 0x99e9
	.2byte 0x0200
	.4byte 0x0200969d
	.global Func_020050e4
	.thumb_func
Func_020050e4:
	push {r5, lr}
	bl 0x0200d4b0
	movs r0, #9
	movs r1, #1
	bl 0x0200d6e0
	movs r0, #10
.L_020050e4_8:
	movs r1, #1
	bl 0x0200d6e0
	movs r0, #17
.L_020050e4_9:
	movs r1, #1
	bl 0x0200d6e0
	ldr r0, [pc, #500]
.L_020050e4_10:
	bl 0x0200d610
.L_020050e4_14:
	cmp r0, #0
	beq .L_020050e4_0
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl 0x0200d6a0
.L_020050e4_0:
	ldr r0, [pc, #484]
	bl 0x0200d610
.L_020050e4_25:
	cmp r0, #0
	beq .L_020050e4_1
	movs r0, #11
	movs r1, #0
.L_020050e4_15:
	movs r2, #0
	bl 0x0200d6a0
.L_020050e4_1:
	ldr r0, [pc, #468]
.L_020050e4_16:
	bl 0x0200d610
	cmp r0, #0
	beq .L_020050e4_2
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl 0x0200d6a0
.L_020050e4_2:
	ldr r0, [pc, #452]
	bl 0x0200d610
.L_020050e4_22:
	cmp r0, #0
	beq .L_020050e4_3
.L_020050e4_23:
	movs r0, #8
	movs r1, #0
.L_020050e4_6:
	movs r2, #0
	bl 0x0200d6a0
.L_020050e4_3:
	ldr r3, [pc, #436]
.L_020050e4_7:
	movs r2, #225
	lsls r2, r2, #1
.L_020050e4_24:
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #1
	cmp r3, #30
	bls .L_020050e4_4
	b .L_020050e4_5
.L_020050e4_4:
	ldr r2, [pc, #420]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	movs r0, r0
	bne .L_020050e4_6
	lsls r0, r0, #8
	bne 0x0200d150
	lsls r0, r0, #8
	bne .L_020050e4_7
	lsls r0, r0, #8
	bcs 0x0200d0f0
	lsls r0, r0, #8
	bcs .L_020050e4_8
	lsls r0, r0, #8
	bcs 0x0200d0f8
	lsls r0, r0, #8
	bcs .L_020050e4_9
	lsls r0, r0, #8
	bcs 0x0200d100
	lsls r0, r0, #8
	bcs .L_020050e4_10
	lsls r0, r0, #8
	bcs .L_020050e4_11
	lsls r0, r0, #8
	bcs .L_020050e4_12
	lsls r0, r0, #8
	bcs 0x0200d294
	lsls r0, r0, #8
	bcs .L_020050e4_13
	lsls r0, r0, #8
	bcs 0x0200d100
	lsls r0, r0, #8
	bcs .L_020050e4_10
	lsls r0, r0, #8
.L_020050e4_11:
	bcs .L_020050e4_14
	lsls r0, r0, #8
	bcs .L_020050e4_15
	lsls r0, r0, #8
	bcs 0x0200d128
	lsls r0, r0, #8
.L_020050e4_13:
	bcs .L_020050e4_16
	lsls r0, r0, #8
	bcs .L_020050e4_17
	lsls r0, r0, #8
	bcs .L_020050e4_18
	lsls r0, r0, #8
	bcs .L_020050e4_19
	lsls r0, r0, #8
	bcs .L_020050e4_20
	lsls r0, r0, #8
	bcs .L_020050e4_21
	lsls r0, r0, #8
	bcs .L_020050e4_22
	lsls r0, r0, #8
.L_020050e4_17:
	bcs .L_020050e4_23
	lsls r0, r0, #8
	bcs .L_020050e4_6
	lsls r0, r0, #8
	bcs 0x0200d150
	lsls r0, r0, #8
.L_020050e4_20:
	bcs .L_020050e4_7
	lsls r0, r0, #8
.L_020050e4_21:
	bcs .L_020050e4_24
	lsls r0, r0, #8
	bcs .L_020050e4_25
	lsls r0, r0, #8
	ldr r3, [pc, #288]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #224
	adds r2, #64
	str r2, [r3]
	lsls r0, r0, #4
	bl 0x0200d5f8
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #268]
	b .L_020050e4_26
	.2byte 0x4b41
	.2byte 0x22e0
	.2byte 0x681b
	.2byte 0x0052
	.2byte 0x189b
	.2byte 0x20c0
	.2byte 0x3249
	.2byte 0x601a
	.2byte 0x0100
	.2byte 0xf000
	.2byte 0xf9ec
	.2byte 0x2018
	.2byte 0xf000
	.2byte 0xfa15
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xf9de
	.2byte 0x20c5
	.2byte 0x0080
	.2byte 0xf000
	.2byte 0xf9ee
	.2byte 0x2800
	.2byte 0xd04a
	.2byte 0x21da
	.2byte 0x22f0
	.2byte 0x2019
	.2byte 0x0489
	.2byte 0x03d2
	.2byte 0xf000
	.2byte 0xfa2d
	.2byte 0xe042
	.2byte 0x4b31
	.2byte 0x22e0
	.2byte 0x681b
	.2byte 0x0052
	.2byte 0x189b
	.2byte 0x20e0
.L_020050e4_18:
	adds r2, #64
	str r2, [r3]
.L_020050e4_19:
	lsls r0, r0, #4
	bl 0x0200d5f8
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #180]
.L_020050e4_26:
	bl 0x0200d590
	movs r0, #1
	bl 0x0200d588
	bl 0x0200d5b8
	movs r0, #1
	bl 0x0200d588
	b .L_020050e4_27
	.2byte 0x4b25
	.2byte 0x22e0
	.2byte 0x681b
	.2byte 0x0052
	.2byte 0x189b
	.2byte 0x3240
	.2byte 0x601a
	.2byte 0x4824
	.2byte 0xf000
	.2byte 0xf9c1
	.2byte 0x2800
.L_020050e4_12:
	beq .L_020050e4_27
	bl 0x0200a06c
	b .L_020050e4_27
	.2byte 0x4b1d
	.2byte 0x22e0
	.2byte 0x681b
	.2byte 0x0052
	.2byte 0x189b
	.2byte 0x3240
	.2byte 0x601a
	.2byte 0xf7fc
	.2byte 0xfee1
	.2byte 0xe010
	.2byte 0x21c8
	.2byte 0x481c
	.2byte 0x0109
	.2byte 0xf000
	.2byte 0xf96d
	.2byte 0xe00a
.L_020050e4_5:
	ldr r3, [pc, #84]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #224
	adds r2, #64
	str r2, [r3]
	lsls r0, r0, #4
	bl 0x0200d5f8
.L_020050e4_27:
	movs r0, #8
	bl 0x0200d650
	adds r5, r0, #0
	movs r0, #8
	bl 0x0200d650
	movs r1, #0
	bl 0x0200d5e8
	movs r0, #8
	movs r1, #1
	bl 0x0200d710
	movs r3, #192
	lsls r3, r3, #8
	str r3, [r5, #24]
	str r3, [r5, #28]
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x0000094c
	.4byte 0x00000949
	.4byte 0x0000094b
	.4byte 0x00000f2e
	.4byte 0x02000240
	.4byte 0x0200d170
	.4byte 0x03001ebc
	.4byte 0x02009151
	.4byte 0x0200938d
	.2byte 0x094a
	.2byte 0x0000
	.2byte 0x9a45
	.2byte 0x0200
	.global Func_02005324
	.thumb_func
Func_02005324:
	push {r5, lr}
	ldr r3, [pc, #296]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #64
	str r2, [r3]
	bl 0x0200d4dc
	ldr r0, [pc, #280]
	bl 0x0200d610
	cmp r0, #0
	beq .L_02005324_0
	bl 0x02008c78
.L_02005324_0:
	ldr r0, [pc, #272]
	bl 0x0200d618
	movs r0, #134
	lsls r0, r0, #2
	bl 0x0200d618
	ldr r0, [pc, #260]
	bl 0x0200d610
	cmp r0, #0
	beq .L_02005324_1
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x0200d6a0
	ldr r0, [pc, #236]
	bl 0x0200d620
.L_02005324_1:
	ldr r0, [pc, #240]
	bl 0x0200d610
	cmp r0, #0
	beq .L_02005324_2
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200d6a0
	bl 0x0200a2d4
.L_02005324_2:
	ldr r0, [pc, #220]
	bl 0x0200d610
	cmp r0, #0
	beq .L_02005324_3
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x0200d6a0
	movs r0, #134
	lsls r0, r0, #2
	bl 0x0200d620
.L_02005324_3:
	ldr r0, [pc, #196]
	bl 0x0200d610
	cmp r0, #0
	beq .L_02005324_4
	bl 0x0200a26c
.L_02005324_4:
	ldr r0, [pc, #184]
	bl 0x0200d610
	cmp r0, #0
	beq .L_02005324_5
	bl 0x0200a2a0
.L_02005324_5:
	bl 0x0200d630
	movs r0, #8
	bl 0x0200d650
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02005324_6
	adds r2, r5, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
.L_02005324_6:
	movs r0, #9
	bl 0x0200d650
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02005324_7
	adds r2, r5, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
.L_02005324_7:
	movs r0, #10
	bl 0x0200d650
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02005324_8
	adds r2, r5, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
.L_02005324_8:
	movs r0, #11
	bl 0x0200d650
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02005324_9
	movs r1, #0
	bl 0x0200d5e8
.L_02005324_9:
	adds r3, r5, #0
	adds r3, #35
	movs r2, #2
	strb r2, [r3]
	movs r0, #12
	bl 0x0200d650
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02005324_10
	adds r3, r5, #0
	adds r3, #89
	ldrb r2, [r3]
	movs r1, #16
	orrs r2, r1
	strb r2, [r3]
.L_02005324_10:
	movs r0, #11
	bl 0x0200d650
	movs r1, #0
	bl 0x0200d5e8
	bl 0x0200d638
	movs r0, #224
	lsls r0, r0, #4
	bl 0x0200d5f8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00000943
	.4byte 0x00000217
	.4byte 0x00000944
	.4byte 0x00000945
	.4byte 0x00000946
	.4byte 0x00000947
	.4byte 0x00000948
	.global Func_02005470
	.thumb_func
Func_02005470:
	push {lr}
	ldr r0, [pc, #52]
	bl 0x0200d610
	cmp r0, #0
	beq .L_02005470_0
	movs r0, #0
	bl 0x02008ee0
.L_02005470_0:
	ldr r0, [pc, #40]
	bl 0x0200d610
	cmp r0, #0
	beq .L_02005470_1
	movs r0, #1
	bl 0x02008ee0
.L_02005470_1:
	movs r0, #215
	lsls r0, r0, #2
	bl 0x0200d610
	cmp r0, #0
	beq .L_02005470_2
	movs r0, #2
	bl 0x02008ee0
.L_02005470_2:
	pop {r0}
	bx r0
	.4byte 0x0000035a
	.4byte 0x0000035b
	.global Func_020054b0
	.thumb_func
Func_020054b0:
	push {lr}
	movs r0, #214
	lsls r0, r0, #2
	bl 0x0200d610
	cmp r0, #0
	beq .L_020054b0_0
	movs r0, #0
	bl 0x02008e30
.L_020054b0_0:
	ldr r0, [pc, #16]
	bl 0x0200d610
	cmp r0, #0
	beq .L_020054b0_1
	movs r0, #1
	bl 0x02008e30
.L_020054b0_1:
	pop {r0}
	bx r0
	.4byte 0x00000359
	.global Func_020054dc
	.thumb_func
Func_020054dc:
	push {lr}
	ldr r0, [pc, #52]
	bl 0x0200d610
	cmp r0, #0
	beq .L_020054dc_0
	movs r0, #0
	bl 0x02008d54
.L_020054dc_0:
	ldr r0, [pc, #40]
	bl 0x0200d610
	cmp r0, #0
	beq .L_020054dc_1
	movs r0, #1
	bl 0x02008d54
.L_020054dc_1:
	ldr r0, [pc, #28]
	bl 0x0200d610
	cmp r0, #0
	beq .L_020054dc_2
	movs r0, #2
	bl 0x02008d54
.L_020054dc_2:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000355
	.4byte 0x00000356
	.4byte 0x00000357
	.global Func_02005520
	.thumb_func
Func_02005520:
	push {lr}
	movs r0, #212
	lsls r0, r0, #2
	bl 0x0200d610
	cmp r0, #0
	beq .L_02005520_0
	movs r0, #0
	bl 0x02008c90
.L_02005520_0:
	ldr r0, [pc, #68]
	bl 0x0200d610
	cmp r0, #0
	beq .L_02005520_1
	movs r0, #1
	bl 0x02008c90
.L_02005520_1:
	ldr r0, [pc, #56]
	bl 0x0200d610
	cmp r0, #0
	beq .L_02005520_2
	movs r0, #2
	bl 0x02008c90
.L_02005520_2:
	ldr r0, [pc, #44]
	bl 0x0200d610
	cmp r0, #0
	beq .L_02005520_3
	movs r0, #3
	bl 0x02008c90
.L_02005520_3:
	movs r0, #213
	lsls r0, r0, #2
	bl 0x0200d610
	cmp r0, #0
	beq .L_02005520_4
	movs r0, #4
	bl 0x02008c90
.L_02005520_4:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000351
	.4byte 0x00000352
	.4byte 0x00000353
	.include "games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/IMPORT.INC"
@ The compiler library links here from its licensed container.
	.section .rodata.part1,"a",%progbits
	push {r4, r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	mov r4, r8
	push {r4, r5, r6, r7}
	sub sp, #8
	mov r10, r0
	mov r9, r1
	mov r8, r2
	bl 0x0200dac0
	cmp r0, #0
	beq .L_020057b0_0
.L_020057b0_4:
	mov r0, r10
	b .L_020057b0_1
.L_020057b0_0:
	mov r0, r9
	bl 0x0200dac0
	cmp r0, #0
	bne .L_020057b0_2
	mov r0, r10
	bl 0x0200dad0
	cmp r0, #0
	beq .L_020057b0_3
	mov r0, r9
	bl 0x0200dad0
	cmp r0, #0
	beq .L_020057b0_4
	mov r0, r10
	mov r1, r9
	ldr r2, [r0, #4]
	ldr r3, [r1, #4]
	cmp r2, r3
	beq .L_020057b0_4
	bl 0x0200dab8
	b .L_020057b0_1
.L_020057b0_3:
	mov r0, r9
	bl 0x0200dad0
	cmp r0, #0
	bne .L_020057b0_2
	mov r0, r9
	bl 0x0200dae0
	cmp r0, #0
	beq .L_020057b0_5
	mov r0, r10
	bl 0x0200dae0
	cmp r0, #0
	beq .L_020057b0_4
	mov r2, r8
	mov r3, r10
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	ldmia r3!, {r0, r4}
	stmia r2!, {r0, r4}
	mov r1, r10
	mov r4, r9
	ldr r3, [r1, #4]
	ldr r2, [r4, #4]
	mov r0, r8
	ands r3, r2
	str r3, [r0, #4]
	b .L_020057b0_1
.L_020057b0_5:
	mov r0, r10
	bl 0x0200dae0
	cmp r0, #0
	beq .L_020057b0_6
.L_020057b0_2:
	mov r0, r9
	b .L_020057b0_1
.L_020057b0_6:
	mov r1, r10
	ldr r1, [r1, #8]
	mov r2, r9
	ldr r2, [r2, #8]
	mov lr, r1
	mov r1, r9
	mov r3, r10
	ldr r0, [r1, #12]
	ldr r1, [r1, #16]
	mov r4, lr
	ldr r6, [r3, #12]
	ldr r7, [r3, #16]
	subs r3, r4, r2
	mov r12, r2
	str r0, [sp, #0]
	str r1, [sp, #4]
	cmp r3, #0
	bge .L_020057b0_7
	negs r3, r3
.L_020057b0_7:
	cmp r3, #63
	bgt .L_020057b0_8
	cmp lr, r12
	ble .L_020057b0_9
	mov r2, r12
	mov r1, lr
	movs r0, #1
	subs r1, r1, r2
	mov r11, r0
	mov r12, r1
.L_020057b0_10:
	movs r3, #1
	ldr r0, [sp, #4]
	negs r3, r3
	add r12, r3
	ldr r3, [sp, #0]
	lsls r5, r0, #31
	ldr r1, [sp, #0]
	lsrs r0, r3, #1
	adds r3, r5, #0
	mov r4, r11
	orrs r3, r0
	ldr r0, [sp, #4]
	ands r1, r4
	lsrs r4, r0, #1
	adds r0, r1, #0
	orrs r0, r3
	movs r2, #0
	str r0, [sp, #0]
	adds r0, r2, #0
	orrs r0, r4
	mov r1, r12
	str r0, [sp, #4]
	cmp r1, #0
	bne .L_020057b0_10
	mov r12, lr
.L_020057b0_9:
	cmp r12, lr
	ble .L_020057b0_11
	movs r2, #1
	mov r11, r2
.L_020057b0_12:
	movs r3, #1
	lsls r5, r7, #31
	mov r1, r11
	lsrs r0, r6, #1
	add lr, r3
	ands r1, r6
	movs r2, #0
	adds r3, r5, #0
	lsrs r4, r7, #1
	orrs r3, r0
	adds r6, r1, #0
	adds r7, r2, #0
	orrs r6, r3
	orrs r7, r4
	cmp r12, lr
	bgt .L_020057b0_12
	b .L_020057b0_11
.L_020057b0_8:
	cmp lr, r12
	ble .L_020057b0_13
	movs r0, #0
	movs r1, #0
	str r0, [sp, #0]
	str r1, [sp, #4]
	b .L_020057b0_11
.L_020057b0_13:
	mov lr, r12
	movs r6, #0
	movs r7, #0
.L_020057b0_11:
	mov r1, r10
	mov r2, r9
	ldr r0, [r1, #4]
	ldr r3, [r2, #4]
	cmp r0, r3
	beq .L_020057b0_14
	ldr r1, [sp, #0]
	ldr r2, [sp, #4]
	subs r1, r1, r6
	sbcs r2, r7
	cmp r0, #0
	bne .L_020057b0_15
	ldr r3, [sp, #0]
	ldr r4, [sp, #4]
	adds r2, r7, #0
	adds r1, r6, #0
	subs r1, r1, r3
	sbcs r2, r4
.L_020057b0_15:
	cmp r2, #0
	blt .L_020057b0_16
	movs r3, #0
	mov r4, r8
	str r3, [r4, #4]
	mov r0, lr
	mov r3, r8
	str r0, [r4, #8]
	str r1, [r3, #12]
	str r2, [r3, #16]
	b .L_020057b0_17
.L_020057b0_16:
	mov r4, r8
	movs r3, #1
	mov r0, lr
	str r3, [r4, #4]
	str r0, [r4, #8]
	movs r4, #0
	negs r3, r1
	sbcs r4, r2
	mov r1, r8
	str r3, [r1, #12]
	str r4, [r1, #16]
.L_020057b0_17:
	mov r2, r8
	ldr r4, [r2, #12]
	ldr r5, [r2, #16]
	ldr r1, [pc, #176]
	movs r2, #1
	negs r2, r2
	asrs r3, r2, #31
	adds r2, r2, r4
	adcs r3, r5
	cmp r3, r1
	bhi .L_020057b0_18
	cmp r3, r1
	bne .L_020057b0_19
	movs r0, #2
	negs r0, r0
	cmp r2, r0
	bhi .L_020057b0_18
.L_020057b0_19:
	lsrs r2, r4, #31
	lsls r3, r5, #1
	adds r1, r2, #0
	mov r2, r8
	orrs r1, r3
	ldr r3, [r2, #8]
	lsls r0, r4, #1
	subs r3, #1
	str r0, [r2, #12]
	str r1, [r2, #16]
	str r3, [r2, #8]
	ldr r6, [pc, #128]
	movs r2, #1
	negs r2, r2
	asrs r3, r2, #31
	adds r2, r2, r0
	adcs r3, r1
	cmp r3, r6
	bhi .L_020057b0_18
	adds r5, r1, #0
	adds r4, r0, #0
	cmp r3, r6
	bne .L_020057b0_19
	movs r4, #2
	negs r4, r4
	cmp r2, r4
	bhi .L_020057b0_18
	adds r5, r1, #0
	adds r4, r0, #0
	b .L_020057b0_19
.L_020057b0_14:
	mov r1, r8
	mov r2, lr
	str r0, [r1, #4]
	str r2, [r1, #8]
	ldr r3, [sp, #0]
	ldr r4, [sp, #4]
	adds r6, r6, r3
	adcs r7, r4
	mov r4, r8
	str r6, [r4, #12]
	str r7, [r4, #16]
.L_020057b0_18:
	movs r3, #3
	mov r0, r8
	str r3, [r0]
	ldr r1, [pc, #64]
	ldr r3, [r0, #16]
	cmp r3, r1
	bls .L_020057b0_20
	ldr r5, [r0, #12]
	ldr r6, [r0, #16]
	movs r2, #1
	adds r3, r5, #0
	ands r3, r2
	lsls r2, r6, #31
	mov r12, r2
	lsrs r0, r5, #1
	mov r1, r12
	orrs r1, r0
	movs r4, #0
	lsrs r2, r6, #1
	mov r0, r8
	orrs r3, r1
	orrs r4, r2
	str r3, [r0, #12]
	str r4, [r0, #16]
	ldr r3, [r0, #8]
	adds r3, #1
	str r3, [r0, #8]
.L_020057b0_20:
	mov r0, r8
.L_020057b0_1:
	sub sp, #-8
	pop {r3, r4, r5, r6}
	mov r8, r3
	mov r9, r4
	mov r10, r5
	mov r11, r6
	pop {r4, r5, r6, r7, pc}
	.4byte 0x0fffffff
	.4byte 0x1fffffff
	push {r4, r5, r6, lr}
	sub sp, #76
	add r4, sp, #8
	add r6, sp, #56
	mov r5, sp
	str r0, [r4]
	str r1, [r4, #4]
	adds r0, r4, #0
	adds r1, r6, #0
	str r2, [r5]
	str r3, [r5, #4]
	bl 0x0200de04
	add r4, sp, #36
	adds r0, r5, #0
	adds r1, r4, #0
	bl 0x0200de04
	adds r1, r4, #0
	add r2, sp, #16
	adds r0, r6, #0
	bl 0x0200d7ec
	bl 0x0200dc38
	sub sp, #-76
	pop {r4, r5, r6, pc}
	.2byte 0x0000
	push {r4, r5, r6, lr}
	sub sp, #76
	add r4, sp, #8
	add r6, sp, #56
	mov r5, sp
	str r0, [r4]
	str r1, [r4, #4]
	adds r0, r4, #0
	adds r1, r6, #0
	str r2, [r5]
	str r3, [r5, #4]
	bl 0x0200de04
	add r4, sp, #36
	adds r0, r5, #0
	adds r1, r4, #0
	bl 0x0200de04
	ldr r3, [r4, #4]
	movs r2, #1
	eors r3, r2
	str r3, [r4, #4]
	add r2, sp, #16
	adds r1, r4, #0
	adds r0, r6, #0
	bl 0x0200d7ec
	bl 0x0200dc38
	sub sp, #-76
	pop {r4, r5, r6, pc}
	.2byte 0x0000
	.2byte 0x4800
	.2byte 0x4770
	.2byte 0xdf90
	.2byte 0x0200
	push {lr}
	ldr r3, [r0]
	movs r2, #0
	cmp r3, #1
	bhi .L_020057b0_21
	movs r2, #1
.L_020057b0_21:
	adds r0, r2, #0
	pop {pc}
	push {lr}
	ldr r3, [r0]
	movs r2, #0
	cmp r3, #4
	bne .L_020057b0_22
	movs r2, #1
.L_020057b0_22:
	adds r0, r2, #0
	pop {pc}
	push {lr}
	ldr r3, [r0]
	movs r2, #0
	cmp r3, #2
	bne .L_020057b0_23
	movs r2, #1
.L_020057b0_23:
	adds r0, r2, #0
	pop {pc}
	push {r4, r5, lr}
	sub sp, #20
	mov r5, sp
	movs r3, #3
	lsrs r2, r0, #31
	str r3, [r5]
	str r2, [r5, #4]
	cmp r0, #0
	bne .L_020057b0_24
	movs r3, #2
	str r3, [r5]
	b .L_020057b0_25
.L_020057b0_24:
	movs r3, #60
	str r3, [r5, #8]
	cmp r2, #0
	beq .L_020057b0_26
	movs r3, #128
	lsls r3, r3, #24
	cmp r0, r3
	bne .L_020057b0_27
	ldr r1, [pc, #72]
	ldr r0, [pc, #68]
	b .L_020057b0_28
.L_020057b0_27:
	negs r3, r0
	asrs r4, r3, #31
	b .L_020057b0_29
.L_020057b0_26:
	adds r3, r0, #0
	asrs r4, r0, #31
.L_020057b0_29:
	str r3, [r5, #12]
	str r4, [r5, #16]
	ldr r3, [r5, #16]
	ldr r2, [pc, #56]
	cmp r3, r2
	bhi .L_020057b0_25
	adds r0, r5, #0
	mov r12, r2
.L_020057b0_30:
	ldr r3, [r0, #12]
	ldr r4, [r0, #16]
	lsrs r1, r3, #31
	lsls r2, r4, #1
	adds r4, r1, #0
	lsls r3, r3, #1
	orrs r4, r2
	str r3, [r0, #12]
	str r4, [r0, #16]
	ldr r3, [r0, #8]
	subs r3, #1
	str r3, [r0, #8]
	ldr r3, [r0, #16]
	cmp r3, r12
	bls .L_020057b0_30
.L_020057b0_25:
	adds r0, r5, #0
	bl 0x0200dc38
.L_020057b0_28:
	sub sp, #-20
	pop {r4, r5, pc}
	.4byte 0xc1e00000
	.4byte 0x00000000
	.4byte 0x0fffffff
	push {r4, lr}
	sub sp, #28
	mov r3, sp
	add r4, sp, #8
	str r0, [r3]
	str r1, [r3, #4]
	adds r0, r3, #0
	adds r1, r4, #0
	bl 0x0200de04
	adds r0, r4, #0
	bl 0x0200dbf8
	cmp r0, #0
	bne .L_020057b0_31
	adds r0, r4, #0
	bl 0x0200dbd8
	cmp r0, #0
	beq .L_020057b0_32
.L_020057b0_31:
	movs r0, #0
	b .L_020057b0_33
.L_020057b0_32:
	adds r0, r4, #0
	bl 0x0200dbe8
	cmp r0, #0
	bne .L_020057b0_34
	ldr r3, [r4, #8]
	movs r0, #0
	cmp r3, #0
	blt .L_020057b0_33
	cmp r3, #30
	ble .L_020057b0_35
.L_020057b0_34:
	ldr r3, [r4, #4]
	negs r0, r3
	orrs r0, r3
	ldr r3, [pc, #28]
	lsrs r0, r0, #31
	adds r0, r0, r3
	b .L_020057b0_33
.L_020057b0_35:
	movs r2, #60
	subs r2, r2, r3
	ldr r0, [r4, #12]
	ldr r1, [r4, #16]
	bl 0x0200dc08
	ldr r3, [r4, #4]
	cmp r3, #0
	beq .L_020057b0_33
	negs r0, r0
.L_020057b0_33:
	sub sp, #-28
	pop {r4, pc}
	.4byte 0x7fffffff
	push {lr}
	ldr r3, [r0]
	movs r2, #0
	cmp r3, #1
	bhi .L_020057b0_36
	movs r2, #1
.L_020057b0_36:
	adds r0, r2, #0
	pop {pc}
	push {lr}
	ldr r3, [r0]
	movs r2, #0
	cmp r3, #4
	bne .L_020057b0_37
	movs r2, #1
.L_020057b0_37:
	adds r0, r2, #0
	pop {pc}
	push {lr}
	ldr r3, [r0]
	movs r2, #0
	cmp r3, #2
	bne .L_020057b0_38
	movs r2, #1
.L_020057b0_38:
	adds r0, r2, #0
	pop {pc}
	push {r4, r5, r6, lr}
	adds r6, r2, #0
	cmp r6, #0
	beq .L_020057b0_39
	movs r3, #32
	subs r3, r3, r6
	cmp r3, #0
	bgt .L_020057b0_40
	negs r3, r3
	adds r4, r1, #0
	movs r5, #0
	lsrs r4, r3
	b .L_020057b0_41
.L_020057b0_40:
	adds r2, r1, #0
	lsls r2, r3
	adds r3, r0, #0
	lsrs r3, r6
	adds r5, r1, #0
	adds r4, r3, #0
	lsrs r5, r6
	orrs r4, r2
.L_020057b0_41:
	adds r1, r5, #0
	adds r0, r4, #0
.L_020057b0_39:
	pop {r4, r5, r6, pc}
	push {r4, r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r4, r0, #0
	ldr r3, [r4, #4]
	sub sp, #8
	ldr r5, [r4, #12]
	ldr r6, [r4, #16]
	mov r10, r3
	movs r7, #0
	bl 0x0200ddd4
	cmp r0, #0
	beq .L_020057b0_42
	ldr r2, [pc, #228]
	ldr r1, [pc, #220]
	adds r4, r6, #0
	adds r3, r5, #0
	orrs r4, r2
	ldr r7, [pc, #220]
	b .L_020057b0_43
.L_020057b0_42:
	adds r0, r4, #0
	bl 0x0200dde4
	cmp r0, #0
	bne .L_020057b0_44
	adds r0, r4, #0
	bl 0x0200ddf4
	cmp r0, #0
	beq .L_020057b0_45
	movs r5, #0
	movs r6, #0
	b .L_020057b0_46
.L_020057b0_45:
	adds r3, r6, #0
	orrs r3, r5
	cmp r3, #0
	beq .L_020057b0_46
	ldr r0, [r4, #8]
	ldr r2, [pc, #184]
	cmp r0, r2
	bge .L_020057b0_47
	subs r2, r2, r0
	cmp r2, #56
	ble .L_020057b0_48
	movs r5, #0
	movs r6, #0
	b .L_020057b0_49
.L_020057b0_48:
	movs r3, #0
	mov r8, r3
	movs r3, #1
	lsls r3, r2
	subs r3, #1
	asrs r4, r3, #31
	ands r4, r6
	ands r3, r5
	orrs r3, r4
	cmp r3, #0
	beq .L_020057b0_50
	movs r3, #1
	mov r8, r3
.L_020057b0_50:
	adds r1, r6, #0
	adds r0, r5, #0
	bl 0x0200dc08
	movs r4, #0
	mov r3, r8
	adds r5, r0, #0
	adds r6, r1, #0
	orrs r5, r3
	orrs r6, r4
.L_020057b0_49:
	movs r3, #255
	adds r1, r5, #0
	ands r1, r3
	movs r2, #0
	cmp r1, #128
	bne .L_020057b0_51
	cmp r2, #0
	bne .L_020057b0_51
	adds r3, #1
	adds r1, r5, #0
	ands r1, r3
	adds r3, r2, #0
	orrs r3, r1
	cmp r3, #0
	beq .L_020057b0_52
	movs r3, #128
	movs r4, #0
	b .L_020057b0_53
.L_020057b0_51:
	movs r3, #127
	movs r4, #0
.L_020057b0_53:
	adds r5, r5, r3
	adcs r6, r4
.L_020057b0_52:
	ldr r3, [pc, #80]
	cmp r6, r3
	bls .L_020057b0_54
	movs r7, #1
	b .L_020057b0_54
.L_020057b0_47:
	movs r3, #128
	lsls r3, r3, #3
	cmp r0, r3
	blt .L_020057b0_55
.L_020057b0_44:
	ldr r7, [pc, #56]
	movs r5, #0
	movs r6, #0
	b .L_020057b0_46
.L_020057b0_55:
	ldr r3, [pc, #60]
	adds r1, r5, #0
	adds r7, r0, r3
	movs r3, #255
	ands r1, r3
	movs r2, #0
	cmp r1, #128
	bne .L_020057b0_56
	cmp r2, #0
	bne .L_020057b0_56
	adds r3, #1
	adds r1, r5, #0
	ands r1, r3
	adds r3, r2, #0
	orrs r3, r1
	cmp r3, #0
	beq .L_020057b0_57
	movs r3, #128
	movs r4, #0
	b .L_020057b0_58
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x000007ff
	.4byte 0xfffffc02
	.4byte 0x0fffffff
	.4byte 0x000003ff
.L_020057b0_56:
	movs r3, #127
	movs r4, #0
.L_020057b0_58:
	adds r5, r5, r3
	adcs r6, r4
.L_020057b0_57:
	ldr r3, [pc, #96]
	cmp r6, r3
	bls .L_020057b0_54
	lsls r1, r6, #31
	lsrs r2, r5, #1
	adds r3, r1, #0
	orrs r3, r2
	lsrs r4, r6, #1
	adds r6, r4, #0
	adds r5, r3, #0
	adds r7, #1
.L_020057b0_54:
	lsls r1, r6, #24
	lsrs r2, r5, #8
	adds r3, r1, #0
	orrs r3, r2
	lsrs r4, r6, #8
.L_020057b0_43:
	adds r6, r4, #0
	adds r5, r3, #0
.L_020057b0_46:
	mov r0, sp
	ldr r3, [r0, #4]
	ldr r2, [pc, #60]
	ldr r1, [pc, #64]
	ands r3, r2
	ands r1, r6
	orrs r3, r1
	str r3, [r0, #4]
	ldr r3, [pc, #40]
	ldrh r2, [r0, #6]
	ands r7, r3
	ldr r3, [pc, #52]
	lsls r1, r7, #4
	ands r3, r2
	orrs r3, r1
	strh r3, [r0, #6]
	mov r3, r10
	ldrb r2, [r0, #7]
	lsls r1, r3, #7
	movs r3, #127
	ands r3, r2
	orrs r3, r1
	strb r3, [r0, #7]
	ldr r3, [r0, #4]
	str r5, [r0, #4]
	str r3, [r0]
	ldr r1, [r0, #4]
	ldr r0, [r0]
	sub sp, #-8
	b .L_020057b0_59
	.4byte 0x000007ff
	.4byte 0x1fffffff
	.4byte 0xfff00000
	.4byte 0x000fffff
	.4byte 0xffff800f
.L_020057b0_59:
	pop {r3, r4}
	mov r8, r3
	mov r10, r4
	pop {r4, r5, r6, r7, pc}
	push {lr}
	ldr r3, [r0]
	movs r2, #0
	cmp r3, #1
	bhi .L_020057b0_60
	movs r2, #1
.L_020057b0_60:
	adds r0, r2, #0
	pop {pc}
	push {lr}
	ldr r3, [r0]
	movs r2, #0
	cmp r3, #4
	bne .L_020057b0_61
	movs r2, #1
.L_020057b0_61:
	adds r0, r2, #0
	pop {pc}
	push {lr}
	ldr r3, [r0]
	movs r2, #0
	cmp r3, #2
	bne .L_020057b0_62
	movs r2, #1
.L_020057b0_62:
	adds r0, r2, #0
	pop {pc}
	push {r4, r5, r6, r7, lr}
	ldr r4, [r0, #4]
	sub sp, #8
	mov r2, sp
	str r4, [r2]
	ldr r3, [r0]
	str r3, [r2, #4]
	lsls r3, r3, #12
	lsrs r6, r3, #12
	ldrh r3, [r2, #6]
	lsls r3, r3, #17
	adds r7, r1, #0
	lsrs r1, r3, #21
	ldrb r3, [r2, #7]
	lsrs r3, r3, #7
	adds r5, r4, #0
	str r3, [r7, #4]
	cmp r1, #0
	bne .L_020057b0_63
	adds r3, r4, #0
	orrs r3, r6
	cmp r3, #0
	bne .L_020057b0_64
	movs r3, #2
	str r3, [r7]
	b .L_020057b0_65
.L_020057b0_64:
	ldr r3, [pc, #132]
	lsrs r1, r5, #24
	lsls r2, r6, #8
	adds r4, r1, #0
	str r3, [r7, #8]
	orrs r4, r2
	lsls r3, r5, #8
	adds r6, r4, #0
	adds r5, r3, #0
	movs r3, #3
	str r3, [r7]
	ldr r3, [pc, #116]
	cmp r6, r3
	bhi .L_020057b0_66
	mov r12, r3
.L_020057b0_67:
	lsrs r1, r5, #31
	lsls r2, r6, #1
	adds r4, r1, #0
	lsls r3, r5, #1
	orrs r4, r2
	adds r6, r4, #0
	adds r5, r3, #0
	ldr r3, [r7, #8]
	subs r3, #1
	str r3, [r7, #8]
	cmp r6, r12
	bls .L_020057b0_67
	b .L_020057b0_66
.L_020057b0_63:
	ldr r2, [pc, #84]
	cmp r1, r2
	bne .L_020057b0_68
	adds r3, r4, #0
	orrs r3, r6
	cmp r3, #0
	bne .L_020057b0_69
	movs r3, #4
	str r3, [r7]
	b .L_020057b0_65
.L_020057b0_69:
	movs r2, #128
	lsls r2, r2, #12
	adds r4, r6, #0
	movs r3, #0
	ands r4, r2
	orrs r3, r4
	cmp r3, #0
	beq .L_020057b0_70
	movs r3, #1
.L_020057b0_70:
	str r3, [r7]
.L_020057b0_66:
	str r5, [r7, #12]
	str r6, [r7, #16]
	b .L_020057b0_65
.L_020057b0_68:
	ldr r2, [pc, #44]
	adds r3, r1, r2
	lsrs r1, r5, #24
	lsls r2, r6, #8
	adds r4, r1, #0
	orrs r4, r2
	ldr r1, [pc, #36]
	ldr r2, [pc, #36]
	str r3, [r7, #8]
	movs r3, #3
	str r3, [r7]
	orrs r4, r2
	lsls r3, r5, #8
	str r3, [r7, #12]
	str r4, [r7, #16]
.L_020057b0_65:
	sub sp, #-8
	pop {r4, r5, r6, r7, pc}
	.4byte 0xfffffc02
	.4byte 0x0fffffff
	.4byte 0x000007ff
	.4byte 0xfffffc01
	.4byte 0x00000000
	.4byte 0x10000000
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
AlchemyRuntime_02005f90:
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00011999
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00011999
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00011999
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00011999
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000005
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00009999
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000019
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000019
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00009999
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x03980000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02500000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x03980000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x03980000
	.4byte 0x00000000
	.4byte 0x02500000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a8
	.4byte 0xc0000278
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000a0
	.4byte 0xc00000a8
	.4byte 0x00280000
	.4byte 0x01280018
	.4byte 0x00000118
	.4byte 0xffff0002
	.4byte 0x000000a0
	.4byte 0x40000068
	.4byte 0x00280000
	.4byte 0x01280018
	.4byte 0x00000118
	.4byte 0xffff0003
	.4byte 0x00000110
	.4byte 0xc0000380
	.4byte 0x00180000
	.4byte 0x02180268
	.4byte 0x000003a8
	.4byte 0xffff0004
	.4byte 0x00000178
	.4byte 0xc0000380
	.4byte 0x00180000
	.4byte 0x02180268
	.4byte 0x000003a8
	.4byte 0xffff0005
	.4byte 0x000000f8
	.4byte 0x40000358
	.4byte 0x00180000
	.4byte 0x02180268
	.4byte 0x000003a8
	.4byte 0xffff0006
	.4byte 0x000001e8
	.4byte 0x400002a8
	.4byte 0x00180000
	.4byte 0x02180268
	.4byte 0x000003a8
	.4byte 0xffff0007
	.4byte 0x00000048
	.4byte 0x400002b8
	.4byte 0x00180000
	.4byte 0x02180268
	.4byte 0x000003a8
	.4byte 0xffff0008
	.4byte 0x00000060
	.4byte 0x40000358
	.4byte 0x00180000
	.4byte 0x02180268
	.4byte 0x000003a8
	.4byte 0xffff0009
	.4byte 0x00000178
	.4byte 0x400000b8
	.4byte 0x01480000
	.4byte 0x02380008
	.4byte 0x000000e8
	.4byte 0xffff000a
	.4byte 0x000001f8
	.4byte 0x40000058
	.4byte 0x01480000
	.4byte 0x02380008
	.4byte 0x000000e8
	.4byte 0xffff000b
	.4byte 0x000001b8
	.4byte 0x40000068
	.4byte 0x01480000
	.4byte 0x02380008
	.4byte 0x000000e8
	.4byte 0xffff000c
	.4byte 0x00000088
	.4byte 0xc0000200
	.4byte 0x00280000
	.4byte 0x01180148
	.4byte 0x00000228
	.4byte 0xffff000d
	.4byte 0x00000378
	.4byte 0xc0000118
	.4byte 0x02780000
	.4byte 0x03b80028
	.4byte 0x00000148
	.4byte 0xffff000e
	.4byte 0x000002b8
	.4byte 0x40000108
	.4byte 0x02780000
	.4byte 0x03b80028
	.4byte 0x00000148
	.4byte 0xffff000f
	.4byte 0x00000338
	.4byte 0x40000078
	.4byte 0x02780000
	.4byte 0x03b80028
	.4byte 0x00000148
	.4byte 0xffff0010
	.4byte 0x00000298
	.4byte 0xc0000278
	.4byte 0x02680000
	.4byte 0x03a80158
	.4byte 0x000002b8
	.4byte 0xffff0011
	.4byte 0x00000378
	.4byte 0xc0000208
	.4byte 0x02680000
	.4byte 0x03a80158
	.4byte 0x000002b8
	.4byte 0xffff0012
	.4byte 0x000002b8
	.4byte 0x400001b0
	.4byte 0x02680000
	.4byte 0x03a80158
	.4byte 0x000002b8
	.4byte 0xffff0013
	.4byte 0x000001b8
	.4byte 0xc00001c0
	.4byte 0x01400000
	.4byte 0x02300138
	.4byte 0x000001f8
	.4byte 0xffff0014
	.4byte 0x000002b8
	.4byte 0xc0000338
	.4byte 0x02680000
	.4byte 0x035802e8
	.4byte 0x000003b8
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a8
	.4byte 0xc0000278
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000088
	.4byte 0x40000188
	.4byte 0x00280000
	.4byte 0x01340008
	.4byte 0x000001c0
	.4byte 0xffff0002
	.4byte 0x00000088
	.4byte 0x40000050
	.4byte 0x00280000
	.4byte 0x01340008
	.4byte 0x000001c0
	.4byte 0xffff0003
	.4byte 0x00000108
	.4byte 0x400001a0
	.4byte 0x00280000
	.4byte 0x01340008
	.4byte 0x000001c0
	.4byte 0xffff0004
	.4byte 0x00000178
	.4byte 0xc00000e8
	.4byte 0x01480000
	.4byte 0x02380008
	.4byte 0x00000110
	.4byte 0xffff0005
	.4byte 0x000001f8
	.4byte 0xc0000068
	.4byte 0x01480000
	.4byte 0x02380008
	.4byte 0x00000110
	.4byte 0xffff0006
	.4byte 0x00000178
	.4byte 0x400000b0
	.4byte 0x01480000
	.4byte 0x02380008
	.4byte 0x00000110
	.4byte 0xffff0007
	.4byte 0x00000198
	.4byte 0xc00001c8
	.4byte 0x01480000
	.4byte 0x02480110
	.4byte 0x00000200
	.4byte 0xffff0008
	.4byte 0x000001c8
	.4byte 0x40000160
	.4byte 0x01480000
	.4byte 0x02480110
	.4byte 0x00000200
	.4byte 0xffff0009
	.4byte 0x000001f8
	.4byte 0xc00001c8
	.4byte 0x01480000
	.4byte 0x02480110
	.4byte 0x00000200
	.4byte 0xffff000a
	.4byte 0x00000088
	.4byte 0xc0000280
	.4byte 0x00100000
	.4byte 0x010001d8
	.4byte 0x000002b0
	.4byte 0xffff000b
	.4byte 0x00000288
	.4byte 0x40000048
	.4byte 0x02380000
	.4byte 0x03280010
	.4byte 0x000000b0
	.4byte 0xffff000c
	.4byte 0x000002e8
	.4byte 0x40000050
	.4byte 0x02380000
	.4byte 0x03280010
	.4byte 0x000000b0
	.4byte 0xffff000d
	.4byte 0x000002b8
	.4byte 0xc0000178
	.4byte 0x02400000
	.4byte 0x03300108
	.4byte 0x000001b0
	.4byte 0xffff000e
	.4byte 0x000000c8
	.4byte 0x40000350
	.4byte 0x00980000
	.4byte 0x01a802a8
	.4byte 0x000003e0
	.4byte 0xffff000f
	.4byte 0x00000108
	.4byte 0x40000308
	.4byte 0x00980000
	.4byte 0x01a802a8
	.4byte 0x000003e0
	.4byte 0xffff0010
	.4byte 0x00000178
	.4byte 0x40000338
	.4byte 0x00980000
	.4byte 0x01a802a8
	.4byte 0x000003e0
	.4byte 0xffff0011
	.4byte 0x00000218
	.4byte 0xc00002b8
	.4byte 0x01e80000
	.4byte 0x02f80248
	.4byte 0x00000380
	.4byte 0xffff0012
	.4byte 0x000002a8
	.4byte 0x40000340
	.4byte 0x01e80000
	.4byte 0x02f80248
	.4byte 0x00000380
	.4byte 0xffff0013
	.4byte 0x000002c8
	.4byte 0xc0000358
	.4byte 0x01e80000
	.4byte 0x02f80248
	.4byte 0x00000380
	.4byte 0xffff0014
	.4byte 0x00000390
	.4byte 0xc00000c0
	.4byte 0x03100000
	.4byte 0x04000018
	.4byte 0x000000f0
	.4byte 0xffff0015
	.4byte 0x00000348
	.4byte 0xc0000298
	.4byte 0x03000000
	.4byte 0x03f001b8
	.4byte 0x000002c0
	.4byte 0xffff0016
	.4byte 0x000003b8
	.4byte 0x40000210
	.4byte 0x03000000
	.4byte 0x03f001b8
	.4byte 0x000002c0
	.4byte 0xffff0017
	.4byte 0x00000368
	.4byte 0xc0000178
	.4byte 0x03000000
	.4byte 0x03f80108
	.4byte 0x000001b0
	.4byte 0xffff0018
	.4byte 0x00000388
	.4byte 0x40000070
	.4byte 0x03100000
	.4byte 0x04000018
	.4byte 0x000000f0
	.4byte 0xffff001e
	.4byte 0x00000198
	.4byte 0xc00001c8
	.4byte 0x01480000
	.4byte 0x02480110
	.4byte 0x00000200
	.4byte 0xffff001f
	.4byte 0x00000288
	.4byte 0xc0000058
	.4byte 0x02380000
	.4byte 0x03280010
	.4byte 0x000000b0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a8
	.4byte 0xc0000278
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000001a8
	.4byte 0x40000248
	.4byte 0x00c80000
	.4byte 0x01e001e8
	.4byte 0x00000288
	.4byte 0xffff0002
	.4byte 0x00000158
	.4byte 0x40000238
	.4byte 0x00c80000
	.4byte 0x01e001e8
	.4byte 0x00000288
	.4byte 0xffff0003
	.4byte 0x00000138
	.4byte 0xc00001c0
	.4byte 0x00080000
	.4byte 0x02380008
	.4byte 0x000001e0
	.4byte 0xffff0004
	.4byte 0x000001b8
	.4byte 0x40000128
	.4byte 0x00080000
	.4byte 0x02380008
	.4byte 0x000001e0
	.4byte 0xffff0005
	.4byte 0x00000138
	.4byte 0x400000f8
	.4byte 0x00080000
	.4byte 0x02380008
	.4byte 0x000001e0
	.4byte 0xffff0006
	.4byte 0x00000068
	.4byte 0x400000b8
	.4byte 0x00080000
	.4byte 0x02380008
	.4byte 0x000001e0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a8
	.4byte 0xc0000278
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000038
	.4byte 0xc0000148
	.4byte 0x00080000
	.4byte 0x01080008
	.4byte 0x000001a8
	.4byte 0xffff0002
	.4byte 0x000000b8
	.4byte 0xc0000170
	.4byte 0x00080000
	.4byte 0x01080008
	.4byte 0x000001a8
	.4byte 0xffff0003
	.4byte 0x00000148
	.4byte 0x40000158
	.4byte 0x01200000
	.4byte 0x02400020
	.4byte 0x000001b0
	.4byte 0xffff0004
	.4byte 0x000001b8
	.4byte 0xc00000e0
	.4byte 0x01200000
	.4byte 0x02400020
	.4byte 0x000001b0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000006a
	.4byte 0x00131002
	.4byte 0x00203068
	.4byte 0x0030b063
	.4byte 0x000000a0
	.4byte 0x0010109f
	.4byte 0x03c0409f
	.4byte 0x002030a0
	.4byte 0x003020a0
	.4byte 0x004090a0
	.4byte 0x0050c0a0
	.4byte 0x0060d0a0
	.4byte 0x007100a0
	.4byte 0x008010a1
	.4byte 0x009040a0
	.4byte 0x00a100a1
	.4byte 0x00b010a2
	.4byte 0x00c050a0
	.4byte 0x00d060a0
	.4byte 0x00e110a0
	.4byte 0x00f130a0
	.4byte 0x010070a0
	.4byte 0x0110e0a0
	.4byte 0x012140a0
	.4byte 0x0130f0a0
	.4byte 0x014120a0
	.4byte 0x000001ff
	.4byte 0x000000a1
	.4byte 0x03c0409f
	.4byte 0x001080a0
	.4byte 0x002040a1
	.4byte 0x003070a1
	.4byte 0x004020a1
	.4byte 0x0050b0a1
	.4byte 0x0060d0a1
	.4byte 0x007030a1
	.4byte 0x0080a0a1
	.4byte 0x0090e0a1
	.4byte 0x00a080a1
	.4byte 0x00b050a1
	.4byte 0x00c110a1
	.4byte 0x00d060a1
	.4byte 0x00e090a1
	.4byte 0x00f150a1
	.4byte 0x0100a0a0
	.4byte 0x0110c0a1
	.4byte 0x012140a1
	.4byte 0x013160a1
	.4byte 0x014120a1
	.4byte 0x0150f0a1
	.4byte 0x016130a1
	.4byte 0x017180a1
	.4byte 0x018170a1
	.4byte 0x000001ff
	.4byte 0x000000a2
	.4byte 0x0010b0a0
	.4byte 0x002030a2
	.4byte 0x003020a2
	.4byte 0x004020a3
	.4byte 0x005010a3
	.4byte 0x006030a3
	.4byte 0x03c0409f
	.4byte 0x000000a3
	.4byte 0x001050a2
	.4byte 0x002040a2
	.4byte 0x005060a2
	.4byte 0x0040309f
	.4byte 0x03c0409f
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00004000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00004000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x00004000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00004000
	.4byte 0xffff0044
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00004000
	.4byte 0xffff0031
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00028000
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
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0014
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0014
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0014
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0085
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00005000
	.4byte 0xffff0085
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00003000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00013000
	.4byte 0xffff0085
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x0001d000
	.4byte 0xffff0085
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x01b00000
	.4byte 0x00025000
	.4byte 0xffff0085
	.4byte 0x00000001
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00003000
	.4byte 0xffff0085
	.4byte 0x0200e07c
	.4byte 0x02e00000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x0000b000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x00005000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x0000b000
	.4byte 0xffff0085
	.4byte 0x00000001
	.4byte 0x01b00000
	.4byte 0x00000000
	.4byte 0x03000000
	.4byte 0x0000b000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00f5
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x03580000
	.4byte 0x00024000
	.4byte 0xffff0085
	.4byte 0x0200e00c
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00004000
	.4byte 0xffff0085
	.4byte 0x0200dfa8
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00004000
	.4byte 0xffff0085
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00004000
	.4byte 0xffff0085
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00005000
	.4byte 0xffff0085
	.4byte 0x00000001
	.4byte 0x02c00000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x0000b000
	.4byte 0xffff0085
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x0000b000
	.4byte 0xffff0085
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x00003000
	.4byte 0xffff0085
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x00004000
	.4byte 0xffff0085
	.4byte 0x0200e1a8
	.4byte 0x03980000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00004000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00010000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02300000
	.4byte 0x00008000
	.4byte 0xffff0066
	.4byte 0x00000002
	.4byte 0x00940000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00005000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02300000
	.4byte 0x00010000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x0001d000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00015000
	.4byte 0xffff002c
	.4byte 0x0200e070
	.4byte 0x03b00000
	.4byte 0x00000000
	.4byte 0x007a0000
	.4byte 0x00024000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x03880000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00003000
	.4byte 0x0049005b
	.4byte 0x00000001
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x02400000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x011a0000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00004000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x0002c000
	.4byte 0xffff00f1
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x0002c000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x0002c000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x0002c000
	.4byte 0xffff00bb
	.4byte 0x00000007
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0xffff0031
	.4byte 0x00000001
	.4byte 0x02100000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00028000
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
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0032
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff002c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0120
	.4byte 0x00000007
	.4byte 0x01d00000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
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
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000021
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000021
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
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0xffff0012
	.4byte 0x00000012
	.4byte 0x00000001
	.4byte 0xffff0013
	.4byte 0x00000013
	.4byte 0x00000001
	.4byte 0xffff0014
	.4byte 0x00000014
	.4byte 0x00000003
	.4byte 0xffff0063
	.4byte 0x02008f95
	.4byte 0x0000c402
	.4byte 0x035a0028
	.4byte 0x02008f31
	.4byte 0x00004402
	.4byte 0x035a0028
	.4byte 0x02008f31
	.4byte 0x0000c402
	.4byte 0x035b0029
	.4byte 0x02008f31
	.4byte 0x00004402
	.4byte 0x035b0029
	.4byte 0x02008f31
	.4byte 0x0000c402
	.4byte 0x035c002a
	.4byte 0x02008f31
	.4byte 0x00004402
	.4byte 0x035c002a
	.4byte 0x02008f31
	.4byte 0x00000002
	.4byte 0x02240032
	.4byte 0x0200a135
	.4byte 0x00000002
	.4byte 0x02250033
	.4byte 0x0200a1c5
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00002445
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002446
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00002447
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002448
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x0200cbfd
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x0200c705
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00002449
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000244a
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x0000244b
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x0000244c
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x0200cd0d
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x0200c639
	.4byte 0x00000006
	.4byte 0xffff005d
	.4byte 0x02009651
	.4byte 0x00000006
	.4byte 0xffff005e
	.4byte 0x02009709
	.4byte 0x00000006
	.4byte 0xffff005f
	.4byte 0x0200a135
	.4byte 0x00000006
	.4byte 0xffff0060
	.4byte 0x0200a1c5
	.4byte 0x00000013
	.4byte 0x0fa20064
	.4byte 0x001000bc
	.4byte 0x00000013
	.4byte 0x0fa30065
	.4byte 0x001000e5
	.4byte 0x00000013
	.4byte 0x0fa40066
	.4byte 0x001000aa
	.4byte 0x00000013
	.4byte 0x0fa50067
	.4byte 0x00200064
	.4byte 0x00000033
	.4byte 0x0fa60068
	.4byte 0x001000b6
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000031
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
	.4byte 0x00000031
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0xffff0012
	.4byte 0x00000012
	.4byte 0x00000001
	.4byte 0xffff0013
	.4byte 0x00000013
	.4byte 0x00000001
	.4byte 0xffff0014
	.4byte 0x00000014
	.4byte 0x00000001
	.4byte 0xffff0015
	.4byte 0x00000015
	.4byte 0x00000001
	.4byte 0xffff0016
	.4byte 0x00000016
	.4byte 0x00000001
	.4byte 0xffff0017
	.4byte 0x00000017
	.4byte 0x00000001
	.4byte 0xffff0018
	.4byte 0x00000018
	.4byte 0x00000002
	.4byte 0x0949001e
	.4byte 0x02009e2d
	.4byte 0x00000002
	.4byte 0x094a001f
	.4byte 0x02009e95
	.4byte 0x00000002
	.4byte 0x094b0020
	.4byte 0x0200a0cd
	.4byte 0x00000002
	.4byte 0x094c0021
	.4byte 0x02009dc5
	.4byte 0x00000003
	.4byte 0xffff0063
	.4byte 0x02008f95
	.4byte 0x00004402
	.4byte 0x03580028
	.4byte 0x02008e81
	.4byte 0x0000c402
	.4byte 0x03590029
	.4byte 0x02008e81
	.4byte 0x00004402
	.4byte 0x03590029
	.4byte 0x02008e81
	.4byte 0x00009415
	.4byte 0xffff0008
	.4byte 0x020090a9
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x020090a9
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x0200cbe5
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x0000245c
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x0000245d
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x0000245e
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x0000242c
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x0000242d
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0200cbc5
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0200cba5
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0200cb85
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x0200c795
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x0200c9a1
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x0200cb69
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x0000245f
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00002460
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00002461
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00002462
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00002436
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00002437
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002442
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002443
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002444
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x0200c965
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x0200c929
	.4byte 0x00000006
	.4byte 0xffff005b
	.4byte 0x020092e1
	.4byte 0x00000006
	.4byte 0xffff005c
	.4byte 0x020094bd
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000021
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
	.4byte 0x00000031
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000003
	.4byte 0xffff0063
	.4byte 0x02008f95
	.4byte 0x00000003
	.4byte 0xffff0062
	.4byte 0x02008fb5
	.4byte 0x0000c402
	.4byte 0x03550028
	.4byte 0x02008dcd
	.4byte 0x0000c402
	.4byte 0x03560029
	.4byte 0x02008dcd
	.4byte 0x0000c402
	.4byte 0x0357002a
	.4byte 0x02008dcd
	.4byte 0x00004402
	.4byte 0x0357002a
	.4byte 0x02008dcd
	.4byte 0x00004602
	.4byte 0xffff0032
	.4byte 0x02008bad
	.4byte 0x00000202
	.4byte 0xffff0046
	.4byte 0x02008ba1
	.4byte 0x00000003
	.4byte 0x02180033
	.4byte 0x0200a38d
	.4byte 0x00000003
	.4byte 0x02170034
	.4byte 0x0200a309
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x0200a411
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x0200a469
	.4byte 0x00004e15
	.4byte 0xffff0008
	.4byte 0x0200a4c1
	.4byte 0x00004e15
	.4byte 0xffff0009
	.4byte 0x0200a4e5
	.4byte 0x00004e15
	.4byte 0xffff000a
	.4byte 0x0200a505
	.4byte 0x00008c15
	.4byte 0x0943000c
	.4byte 0x02008bed
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000003
	.4byte 0xffff0063
	.4byte 0x02008f95
	.4byte 0x0000c402
	.4byte 0x03500028
	.4byte 0x02008ce1
	.4byte 0x0000c402
	.4byte 0x03510029
	.4byte 0x02008ce1
	.4byte 0x0000c402
	.4byte 0x0352002a
	.4byte 0x02008ce1
	.4byte 0x0000c402
	.4byte 0x0353002b
	.4byte 0x02008ce1
	.4byte 0x0000c402
	.4byte 0x0354002c
	.4byte 0x02008ce1
	.4byte 0x00000002
	.4byte 0x02700014
	.4byte 0x0200a7b1
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x0000256a
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x0000256b
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0000256c
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000256d
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x0200cd51
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x0200cd89
	.4byte 0x00000013
	.4byte 0x0fa70065
	.4byte 0x001000bf
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000d
	.4byte 0x00000038
	.4byte 0x00000008
	.4byte 0x00000038
	.4byte 0x00000003
	.4byte 0x00000038
	.4byte 0x0000001c
	.4byte 0x00000055
	.4byte 0x0000001c
	.4byte 0x00000041
	.4byte 0x0000001f
	.4byte 0x0000003d
	.4byte 0x0000000f
	.4byte 0x00000039
	.4byte 0x00000006
	.4byte 0x0000003d
	.4byte 0x00000010
	.4byte 0x00000077
	.4byte 0x00000010
	.4byte 0x00000071
	.4byte 0x0000001c
	.4byte 0x00000048
	.4byte 0x0000002b
	.4byte 0x0000005c
