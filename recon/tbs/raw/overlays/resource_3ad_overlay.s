.syntax unified
	.thumb
	.section .text.x02008044,"ax",%progbits
	.balign 4
	.global Scene_GetPlacements
	.thumb_func
Scene_GetPlacements:
	push {lr}
	ldr r3, [pc, #28]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_02000044_0
	ldr r0, [pc, #16]
	b .L_02000044_1
.L_02000044_0:
	ldr r0, [pc, #16]
.L_02000044_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000006a
	.4byte 0x02009cd8
	.4byte 0x02009cc0
	.section .text.x02009a0c,"ax",%progbits
	.global Scene_Initialize
	.thumb_func
Scene_Initialize:
	push {r5, lr}
	ldr r3, [pc, #168]
	movs r2, #224
	ldr r3, [r3]
	movs r5, #129
	lsls r2, r2, #1
	lsls r5, r5, #2
	str r5, [r3, r2]
	ldr r3, [pc, #156]
	ldrsh r2, [r3, r2]
	ldr r3, [pc, #156]
	cmp r2, r3
	bne .L_02001a0c_0
	movs r0, #8
	bl 0x02009b3c
	movs r1, #0
	bl 0x02009af4
	movs r0, #9
	bl 0x02009b3c
	movs r1, #0
	bl 0x02009af4
	movs r0, #10
	bl 0x02009b3c
	movs r1, #0
	bl 0x02009af4
	movs r0, #11
	bl 0x02009b3c
	movs r1, #0
	bl 0x02009af4
	movs r0, #11
	bl 0x02009b3c
	ldr r3, [pc, #100]
	str r3, [r0, #28]
	ldr r0, [pc, #100]
	bl 0x02009b04
	cmp r0, #0
	beq .L_02001a0c_1
	bl 0x02008210
.L_02001a0c_1:
	ldr r0, [pc, #92]
	bl 0x02009b04
	cmp r0, #0
	beq .L_02001a0c_2
	bl 0x02008384
.L_02001a0c_2:
	movs r0, #128
	lsls r0, r0, #2
	bl 0x02009b04
	cmp r0, #0
	beq .L_02001a0c_3
	bl 0x020080d4
.L_02001a0c_3:
	ldr r0, [pc, #64]
	bl 0x02009b04
	cmp r0, #0
	beq .L_02001a0c_4
	movs r0, #11
	movs r1, #5
	bl 0x02009b74
.L_02001a0c_4:
	adds r0, r5, #0
	bl 0x02009b04
	cmp r0, #0
	beq .L_02001a0c_0
	movs r0, #9
	movs r1, #5
	bl 0x02009b74
.L_02001a0c_0:
	movs r0, #0
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x0000006a
	.4byte 0x0000f333
	.4byte 0x00000201
	.4byte 0x00000202
	.4byte 0x00000203
