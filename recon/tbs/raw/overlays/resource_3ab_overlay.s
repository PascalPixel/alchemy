.syntax unified
	.thumb
	.section .text.x02008314,"ax",%progbits
	.balign 4
	.global Func_02000314
	.thumb_func
Func_02000314:
	push {lr}
	ldr r3, [pc, #32]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #24]
	cmp r2, r3
	beq .L_02000314_0
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_02000314_0
	ldr r0, [pc, #20]
	b .L_02000314_1
.L_02000314_0:
	ldr r0, [pc, #20]
.L_02000314_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000068
	.4byte 0x0000009f
	.4byte 0x02009d3c
	.4byte 0x02009bec
	.section .text.x02008350,"ax",%progbits
	.balign 4
	.global Func_02000350
	.thumb_func
Func_02000350:
	push {lr}
	ldr r3, [pc, #32]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #24]
	cmp r2, r3
	beq .L_02000350_0
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_02000350_0
	ldr r0, [pc, #20]
	b .L_02000350_1
.L_02000350_0:
	ldr r0, [pc, #20]
.L_02000350_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000068
	.4byte 0x0000009f
	.4byte 0x02009e04
	.4byte 0x02009dcc
	.global Func_02000388
	.thumb_func
Func_02000388:
	push {lr}
	ldr r3, [pc, #32]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #24]
	cmp r2, r3
	beq .L_02000388_0
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_02000388_0
	ldr r0, [pc, #20]
	b .L_02000388_1
.L_02000388_0:
	ldr r0, [pc, #20]
.L_02000388_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000068
	.4byte 0x0000009f
	.4byte 0x02009f64
	.4byte 0x02009e14
	.section .text.x020086e4,"ax",%progbits
	.balign 4
	.global Func_020006e4
	.thumb_func
Func_020006e4:
	push {lr}
	ldr r3, [pc, #52]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #44]
	cmp r2, r3
	bne .L_020006e4_0
	ldr r0, [pc, #40]
	bl 0x020099c0
	ldr r0, [pc, #40]
	b .L_020006e4_1
.L_020006e4_0:
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_020006e4_2
	ldr r0, [pc, #24]
	bl 0x020099c0
	cmp r0, #0
	beq .L_020006e4_2
	ldr r0, [pc, #28]
	b .L_020006e4_1
.L_020006e4_2:
	ldr r0, [pc, #28]
.L_020006e4_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x0000009f
	.4byte 0x00000941
	.4byte 0x0200a3b4
	.4byte 0x00000068
	.4byte 0x0200a1bc
	.4byte 0x02009fc4
	.section .text.x02009668,"ax",%progbits
	.balign 4
	.global Func_02001668
	.thumb_func
Func_02001668:
	push {r5, lr}
	ldr r5, [pc, #276]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #268]
	cmp r2, r3
	bne .L_02001668_0
	ldr r3, [pc, #264]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #192
	str r2, [r3]
	bl 0x020085f0
	ldr r0, [pc, #252]
	bl 0x020099c0
	cmp r0, #0
	bne .L_02001668_1
	movs r0, #20
	bl 0x02009840
.L_02001668_1:
	movs r0, #8
	bl 0x02009a00
	cmp r0, #0
	beq .L_02001668_2
	movs r1, #0
	bl 0x02009998
.L_02001668_2:
	ldr r0, [pc, #224]
	bl 0x020099c8
.L_02001668_0:
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #212]
	cmp r2, r3
	bne .L_02001668_3
	ldr r3, [pc, #192]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #192
	str r2, [r3]
	ldr r3, [pc, #196]
	movs r1, #225
	adds r2, r5, r3
	movs r3, #10
	strh r3, [r2]
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #4
	bne .L_02001668_4
	ldr r0, [pc, #176]
	bl 0x020099c0
	cmp r0, #0
	bne .L_02001668_4
	bl 0x0200931c
.L_02001668_4:
	ldr r3, [pc, #136]
	movs r1, #225
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	bne .L_02001668_5
	ldr r0, [pc, #148]
	bl 0x020099c0
	cmp r0, #0
	bne .L_02001668_5
	bl 0x020095bc
.L_02001668_5:
	ldr r0, [pc, #136]
	bl 0x020099c0
	cmp r0, #0
	beq .L_02001668_6
	ldr r0, [pc, #132]
	bl 0x020099c0
	cmp r0, #0
	bne .L_02001668_6
	movs r1, #200
	ldr r0, [pc, #124]
	lsls r1, r1, #4
	bl 0x02009928
.L_02001668_6:
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #116]
	bl 0x02009928
	ldr r0, [pc, #112]
	bl 0x020099d0
	ldr r0, [pc, #112]
	bl 0x020099d0
	ldr r0, [pc, #108]
	bl 0x020099d0
	ldr r0, [pc, #108]
	bl 0x020099d0
	ldr r0, [pc, #104]
	bl 0x020099d0
	ldr r0, [pc, #104]
	bl 0x020099d0
	ldr r0, [pc, #100]
	bl 0x020099d0
	ldr r0, [pc, #100]
	bl 0x020099d0
	ldr r0, [pc, #96]
	bl 0x020099d0
	ldr r0, [pc, #96]
	bl 0x020099d0
.L_02001668_3:
	movs r0, #0
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000068
	.4byte 0x03001ebc
	.4byte 0x00000fd1
	.4byte 0x00000201
	.4byte 0x0000009f
	.4byte 0x00000242
	.4byte 0x00000109
	.4byte 0x00000941
	.4byte 0x0000094d
	.4byte 0x02008ac5
	.4byte 0x02009241
	.4byte 0x00000944
	.4byte 0x00000945
	.4byte 0x00000946
	.4byte 0x00000947
	.4byte 0x00000948
	.4byte 0x00000943
	.4byte 0x00000949
	.4byte 0x0000094a
	.4byte 0x0000094b
	.4byte 0x0000094c
