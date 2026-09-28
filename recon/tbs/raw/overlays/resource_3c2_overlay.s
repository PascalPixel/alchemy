.syntax unified
	.thumb
	.section .text.x0200806c,"ax",%progbits
	.balign 4
	.global Func_0200006c
	.thumb_func
Func_0200006c:
	push {r5, r6, lr}
	adds r6, r0, #0
	movs r0, #0
	bl 0x02008b64
	movs r2, #128
	ldrh r3, [r0, #6]
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, [pc, #24]
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_0200006c_0
	movs r0, #31
	adds r1, r6, #0
	bl 0x02008c1c
	b .L_0200006c_1
	.2byte 0x0000
	.4byte 0xffffc000
.L_0200006c_0:
	ldr r0, [pc, #84]
	bl 0x02008b34
	cmp r0, #0
	beq .L_0200006c_2
	ldr r5, [pc, #80]
	adds r0, r5, #0
	bl 0x02008bc4
	movs r1, #0
	adds r0, r6, #0
	bl 0x02008bcc
	movs r0, #0
	movs r1, #0
	bl 0x02008b5c
	cmp r0, #0
	bne .L_0200006c_3
	movs r0, #10
	bl 0x02008b44
	adds r0, r5, #1
	bl 0x02008bc4
	b .L_0200006c_4
.L_0200006c_3:
	adds r0, r5, #2
	bl 0x02008bc4
.L_0200006c_4:
	adds r0, r6, #0
	movs r1, #0
	bl 0x02008bd4
	b .L_0200006c_1
.L_0200006c_2:
	ldr r0, [pc, #24]
	bl 0x02008bc4
	adds r0, r6, #0
	movs r1, #0
	bl 0x02008bd4
.L_0200006c_1:
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x0000096f
	.4byte 0x0000261c
	.4byte 0x000025cf
	.section .text.x020081d4,"ax",%progbits
	.balign 4
	.global Func_020001d4
	.thumb_func
Func_020001d4:
	push {r5, r6, lr}
	ldr r5, [pc, #64]
	adds r6, r0, #0
	adds r0, r5, #0
	bl 0x02008bc4
	movs r1, #0
	adds r0, r6, #0
	bl 0x02008bcc
	movs r0, #0
	movs r1, #0
	bl 0x02008b5c
	cmp r0, #0
	bne .L_020001d4_0
	movs r0, #10
	bl 0x02008b44
	adds r0, r5, #1
	bl 0x02008bc4
	b .L_020001d4_1
.L_020001d4_0:
	adds r0, r5, #2
	bl 0x02008bc4
.L_020001d4_1:
	adds r0, r6, #0
	movs r1, #0
	bl 0x02008bd4
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00002624
	.section .text.x02008240,"ax",%progbits
	.balign 4
	.global Func_02000240
	.thumb_func
Func_02000240:
	push {lr}
	movs r0, #155
	lsls r0, r0, #4
	bl 0x02008b3c
	ldr r0, [pc, #1016]
	bl 0x02008b34
	cmp r0, #0
	beq .L_02000240_0
	b .L_02000240_1
.L_02000240_0:
	movs r0, #30
	bl 0x02008c34
	bl 0x02008b4c
	movs r0, #184
	movs r1, #1
	movs r2, #208
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #15
	bl 0x02008bec
	movs r1, #184
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #160
	bl 0x02008b7c
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02008bdc
	movs r2, #16
	movs r3, #192
	lsls r3, r3, #8
	negs r2, r2
	movs r1, #0
	movs r0, #19
	bl 0x02008c0c
	movs r0, #19
	bl 0x02008b84
	bl 0x02008bf4
	ldr r0, [pc, #928]
	bl 0x02008bc4
	movs r0, #10
	bl 0x02008b44
	movs r1, #2
	movs r0, #20
	bl 0x02008bac
	movs r0, #20
	bl 0x02008b44
	movs r1, #0
	movs r0, #20
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	movs r1, #2
	movs r0, #19
	bl 0x02008bac
	movs r0, #20
	bl 0x02008b44
	movs r0, #19
	ldr r1, [pc, #876]
	ldr r2, [pc, #880]
	bl 0x02008b6c
	movs r2, #16
	movs r0, #19
	movs r1, #0
	negs r2, r2
	bl 0x02008c14
	movs r1, #0
	movs r2, #0
	movs r0, #19
	bl 0x02008bdc
	movs r0, #30
	bl 0x02008b44
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #19
	bl 0x02008bdc
	movs r0, #30
	bl 0x02008b44
	movs r1, #0
	movs r2, #0
	movs r0, #19
	bl 0x02008bdc
	movs r0, #30
	bl 0x02008b44
	movs r1, #128
	movs r2, #40
	movs r0, #19
	lsls r1, r1, #1
	bl 0x02008be4
	movs r1, #0
	movs r0, #19
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	movs r1, #2
	movs r0, #21
	bl 0x02008bac
	movs r0, #20
	bl 0x02008b44
	movs r1, #0
	movs r0, #21
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	movs r0, #19
	ldr r1, [pc, #752]
	ldr r2, [pc, #752]
	bl 0x02008b6c
	movs r2, #24
	movs r0, #19
	movs r1, #0
	negs r2, r2
	bl 0x02008c14
	movs r0, #19
	movs r1, #48
	movs r2, #0
	bl 0x02008c14
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #19
	bl 0x02008bdc
	movs r0, #30
	bl 0x02008b44
	movs r1, #0
	movs r0, #19
	bl 0x02008bd4
	movs r0, #20
	bl 0x02008b44
	movs r1, #128
	movs r2, #40
	movs r0, #19
	lsls r1, r1, #1
	bl 0x02008be4
	movs r1, #0
	movs r0, #19
	bl 0x02008bd4
	movs r0, #20
	bl 0x02008b44
	movs r1, #129
	movs r2, #40
	movs r0, #20
	lsls r1, r1, #1
	bl 0x02008be4
	movs r1, #0
	movs r0, #20
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #19
	bl 0x02008bdc
	movs r0, #20
	bl 0x02008b44
	movs r1, #0
	movs r0, #19
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	movs r1, #2
	movs r0, #21
	bl 0x02008bac
	movs r0, #20
	bl 0x02008b44
	movs r1, #4
	movs r0, #21
	bl 0x02008b9c
	movs r0, #20
	bl 0x02008b44
	movs r1, #0
	movs r0, #21
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #19
	bl 0x02008bdc
	movs r0, #30
	bl 0x02008b44
	movs r2, #40
	movs r0, #19
	ldr r1, [pc, #548]
	bl 0x02008be4
	movs r1, #0
	movs r0, #19
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	movs r0, #20
	movs r1, #2
	bl 0x02008ba4
	movs r1, #2
	movs r0, #21
	bl 0x02008bac
	movs r0, #20
	bl 0x02008b44
	movs r0, #0
	ldr r1, [pc, #496]
	ldr r2, [pc, #500]
	bl 0x02008b6c
	movs r1, #184
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #104
	bl 0x02008b7c
	movs r0, #0
	movs r1, #16
	movs r2, #0
	bl 0x02008c14
	movs r2, #0
	movs r1, #0
	movs r0, #0
	bl 0x02008bdc
	movs r0, #20
	bl 0x02008b44
	movs r0, #10
	bl 0x02008b44
	movs r1, #4
	movs r0, #19
	bl 0x02008b9c
	movs r0, #20
	bl 0x02008b44
	movs r1, #0
	movs r0, #19
	bl 0x02008bd4
	movs r0, #20
	bl 0x02008b44
	movs r1, #129
	movs r2, #50
	movs r0, #19
	lsls r1, r1, #1
	bl 0x02008be4
	movs r1, #0
	movs r0, #19
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	movs r1, #2
	movs r0, #20
	bl 0x02008bac
	movs r0, #20
	bl 0x02008b44
	movs r1, #0
	movs r0, #20
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	movs r0, #0
	movs r1, #20
	movs r2, #0
	bl 0x02008bb4
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #19
	bl 0x02008bdc
	movs r0, #20
	bl 0x02008b44
	movs r1, #0
	movs r0, #19
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	movs r1, #2
	movs r0, #21
	bl 0x02008bac
	movs r0, #20
	bl 0x02008b44
	movs r1, #0
	movs r0, #21
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	movs r0, #0
	movs r1, #21
	movs r2, #0
	bl 0x02008bb4
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #19
	bl 0x02008bdc
	movs r0, #40
	bl 0x02008b44
	movs r1, #3
	movs r0, #19
	bl 0x02008b9c
	movs r0, #30
	bl 0x02008b44
	movs r1, #0
	movs r0, #19
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	movs r1, #2
	movs r0, #21
	bl 0x02008bac
	movs r0, #20
	bl 0x02008b44
	movs r1, #4
	movs r0, #21
	bl 0x02008b9c
	movs r0, #20
	bl 0x02008b44
	movs r1, #0
	movs r0, #21
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	movs r2, #30
	movs r0, #19
	movs r1, #0
	bl 0x02008bbc
	movs r1, #0
	movs r0, #19
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	movs r1, #2
	movs r0, #20
	bl 0x02008bac
	movs r0, #20
	bl 0x02008b44
	movs r1, #0
	movs r0, #20
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	movs r0, #0
	movs r1, #20
	movs r2, #0
	bl 0x02008bb4
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #19
	bl 0x02008bdc
	movs r0, #40
	bl 0x02008b44
	movs r0, #10
	bl 0x02008b44
	movs r1, #2
	movs r0, #21
	bl 0x02008bac
	movs r0, #20
	bl 0x02008b44
	movs r1, #0
	movs r0, #21
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	movs r0, #0
	movs r1, #21
	movs r2, #0
	bl 0x02008bb4
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #19
	bl 0x02008bdc
	movs r0, #40
	bl 0x02008b44
	movs r0, #0
	movs r1, #3
	bl 0x02008b94
	movs r1, #3
	movs r0, #19
	bl 0x02008b9c
	movs r0, #30
	bl 0x02008b44
	movs r0, #10
	bl 0x02008b44
	movs r1, #4
	movs r0, #20
	bl 0x02008b9c
	movs r0, #20
	bl 0x02008b44
	movs r1, #0
	movs r0, #20
	b .L_02000240_2
	.4byte 0x0000098a
	.4byte 0x000025eb
	.4byte 0x00013333
	.4byte 0x00009999
	.4byte 0x00000107
.L_02000240_2:
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x02008be4
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #19
	bl 0x02008be4
	movs r0, #10
	bl 0x02008b44
	movs r0, #0
	movs r1, #20
	movs r2, #0
	bl 0x02008bb4
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #19
	bl 0x02008bdc
	movs r0, #40
	bl 0x02008b44
	movs r0, #10
	bl 0x02008b44
	movs r1, #2
	movs r0, #21
	bl 0x02008bac
	movs r0, #20
	bl 0x02008b44
	movs r1, #0
	movs r0, #21
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #50
	bl 0x02008be4
	movs r2, #0
	movs r1, #21
	movs r0, #0
	bl 0x02008bb4
	movs r0, #30
	bl 0x02008b44
	movs r1, #2
	movs r0, #21
	bl 0x02008bac
	movs r0, #30
	bl 0x02008b44
	movs r0, #10
	bl 0x02008b44
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #19
	bl 0x02008bdc
	movs r0, #30
	bl 0x02008b44
	movs r1, #0
	movs r0, #19
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	movs r1, #2
	movs r0, #21
	bl 0x02008bac
	movs r0, #20
	bl 0x02008b44
	movs r1, #3
	movs r0, #21
	bl 0x02008b9c
	movs r0, #30
	bl 0x02008b44
	movs r0, #10
	bl 0x02008b44
	movs r1, #129
	movs r2, #40
	movs r0, #19
	lsls r1, r1, #1
	bl 0x02008be4
	movs r1, #0
	movs r0, #19
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	movs r1, #4
	movs r0, #20
	bl 0x02008b9c
	movs r0, #20
	bl 0x02008b44
	movs r1, #0
	movs r0, #20
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	ldr r1, [pc, #732]
	movs r2, #50
	movs r0, #19
	bl 0x02008be4
	movs r0, #10
	bl 0x02008b44
	movs r0, #0
	movs r1, #20
	movs r2, #0
	bl 0x02008bb4
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #19
	bl 0x02008bdc
	movs r0, #20
	bl 0x02008b44
	movs r0, #10
	bl 0x02008b44
	movs r1, #2
	movs r0, #20
	bl 0x02008bac
	movs r0, #20
	bl 0x02008b44
	movs r1, #0
	movs r0, #20
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	movs r1, #128
	movs r2, #40
	movs r0, #19
	lsls r1, r1, #1
	bl 0x02008be4
	movs r1, #0
	movs r0, #19
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	movs r1, #2
	movs r0, #21
	bl 0x02008bac
	movs r0, #20
	bl 0x02008b44
	movs r0, #21
	movs r1, #0
	bl 0x02008bd4
	movs r1, #2
	movs r0, #21
	bl 0x02008bac
	movs r0, #20
	bl 0x02008b44
	movs r0, #10
	bl 0x02008b44
	movs r2, #0
	movs r1, #0
	movs r0, #19
	bl 0x02008bbc
	movs r0, #30
	bl 0x02008b44
	movs r1, #0
	movs r0, #19
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	movs r1, #4
	movs r0, #20
	bl 0x02008b9c
	movs r0, #20
	bl 0x02008b44
	movs r1, #0
	movs r0, #20
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x02008be4
	movs r1, #129
	movs r0, #19
	lsls r1, r1, #1
	movs r2, #80
	bl 0x02008be4
	movs r1, #129
	movs r2, #50
	movs r0, #21
	lsls r1, r1, #1
	bl 0x02008be4
	movs r1, #0
	movs r0, #21
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	movs r1, #2
	movs r0, #21
	bl 0x02008bac
	movs r0, #20
	bl 0x02008b44
	movs r0, #21
	movs r1, #0
	bl 0x02008bd4
	movs r1, #3
	movs r0, #21
	bl 0x02008bac
	movs r0, #20
	bl 0x02008b44
	movs r0, #10
	bl 0x02008b44
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #19
	bl 0x02008bdc
	movs r0, #30
	bl 0x02008b44
	movs r1, #2
	movs r0, #19
	bl 0x02008bac
	movs r0, #10
	bl 0x02008b44
	movs r1, #0
	movs r0, #19
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	movs r1, #2
	movs r0, #20
	bl 0x02008bac
	movs r0, #20
	bl 0x02008b44
	movs r1, #0
	movs r0, #20
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	movs r0, #0
	movs r1, #20
	movs r2, #0
	bl 0x02008bb4
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #19
	bl 0x02008bdc
	movs r0, #30
	bl 0x02008b44
	movs r1, #3
	movs r0, #19
	bl 0x02008b9c
	movs r0, #30
	bl 0x02008b44
	movs r1, #0
	movs r0, #19
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	movs r0, #20
	movs r1, #2
	bl 0x02008ba4
	movs r1, #2
	movs r0, #21
	bl 0x02008bac
	movs r0, #30
	bl 0x02008b44
	movs r0, #10
	bl 0x02008b44
	movs r0, #19
	movs r1, #0
	movs r2, #20
	bl 0x02008bbc
	movs r1, #12
	negs r1, r1
	movs r2, #0
	movs r0, #19
	bl 0x02008c14
	movs r0, #20
	bl 0x02008b44
	movs r1, #0
	movs r0, #19
	bl 0x02008bcc
	movs r0, #0
	movs r1, #0
	bl 0x02008b5c
	cmp r0, #0
	bne .L_02000240_3
	movs r0, #20
	bl 0x02008b44
	movs r0, #19
	movs r1, #0
	bl 0x02008bd4
	ldr r3, [pc, #220]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02000240_4
.L_02000240_3:
	movs r0, #10
	bl 0x02008b44
	ldr r3, [pc, #196]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #19
	movs r1, #0
	bl 0x02008bd4
.L_02000240_4:
	movs r0, #10
	bl 0x02008b44
	movs r1, #2
	movs r0, #19
	bl 0x02008bac
	movs r0, #20
	bl 0x02008b44
	movs r1, #0
	movs r0, #19
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	movs r1, #129
	movs r2, #50
	movs r0, #19
	lsls r1, r1, #1
	bl 0x02008be4
	movs r1, #0
	movs r0, #19
	bl 0x02008bd4
	movs r0, #10
	bl 0x02008b44
	movs r1, #3
	movs r0, #0
	bl 0x02008b9c
	movs r0, #20
	bl 0x02008b44
	movs r1, #3
	movs r0, #19
	bl 0x02008b9c
	movs r0, #30
	bl 0x02008b44
	movs r0, #30
	bl 0x02008c34
	movs r0, #19
	ldr r1, [pc, #80]
	ldr r2, [pc, #80]
	bl 0x02008b6c
	movs r0, #19
	movs r1, #2
	bl 0x02008b94
	movs r0, #0
	bl 0x02008b64
	cmp r0, #0
	beq .L_02000240_5
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #19
	bl 0x02008b74
.L_02000240_5:
	movs r0, #19
	bl 0x02008b84
	movs r1, #0
	movs r2, #0
	movs r0, #19
	bl 0x02008b8c
	movs r0, #10
	bl 0x02008b44
	bl 0x02008c04
	bl 0x02008b54
.L_02000240_1:
	pop {r0}
	bx r0
	.4byte 0x00000101
	.4byte 0x03001ebc
	.4byte 0x00013333
	.4byte 0x00009999
	.section .rodata,"a",%progbits
	.global gSuharaHeyaEntrances
gSuharaHeyaEntrances:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000100
	.4byte 0xc00001c8
	.4byte 0x00000000
	.4byte 0x01500118
	.4byte 0x000001e0
	.4byte 0xffff0002
	.4byte 0x000001b0
	.4byte 0xc00002d8
	.4byte 0x01380000
	.4byte 0x02280210
	.4byte 0x000002f0
	.4byte 0xffff0003
	.4byte 0x00000280
	.4byte 0xc00000a8
	.4byte 0x01f00000
	.4byte 0x02e00010
	.4byte 0x000000c0
	.4byte 0xffff0004
	.4byte 0x00000070
	.4byte 0xc00000a8
	.4byte 0x00000000
	.4byte 0x00f00010
	.4byte 0x000000d0
	.4byte 0xffff0005
	.4byte 0x00000160
	.4byte 0xc00000d8
	.4byte 0x01000000
	.4byte 0x01f00010
	.4byte 0x000000e8
	.4byte 0xffff0006
	.4byte 0x000001b0
	.4byte 0xc0000276
	.4byte 0x01380000
	.4byte 0x02280210
	.4byte 0x000002f0
	.4byte 0xffff0007
	.4byte 0x000002a8
	.4byte 0x80000078
	.4byte 0x01f00000
	.4byte 0x02e00010
	.4byte 0x000000c0
	.4byte 0xffff000a
	.4byte 0x00000068
	.4byte 0xc0000168
	.4byte 0x00000000
	.4byte 0x01500118
	.4byte 0x000001e0
	.4byte 0xffff000b
	.4byte 0x00000098
	.4byte 0xc0000168
	.4byte 0x00000000
	.4byte 0x01500118
	.4byte 0x000001e0
	.4byte 0xffff000c
	.4byte 0x000001d8
	.4byte 0x40000158
	.4byte 0x01580000
	.4byte 0x02480118
	.4byte 0x000001b8
	.4byte 0xffff000d
	.4byte 0x000002c8
	.4byte 0x40000158
	.4byte 0x02580000
	.4byte 0x03480118
	.4byte 0x000001b8
	.4byte 0xffff000e
	.4byte 0x00000058
	.4byte 0x40000160
	.4byte 0x00000000
	.4byte 0x01500118
	.4byte 0x000001e0
	.4byte 0xffff000f
	.4byte 0x00000058
	.4byte 0xc0000248
	.4byte 0x00100000
	.4byte 0x01000210
	.4byte 0x000002b0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSuharaHeyaRegions
gSuharaHeyaRegions:
	.4byte 0x001c0053
	.4byte 0x005b0164
	.4byte 0x016c0024
	.4byte 0x000effff
	.4byte 0xffec0053
	.4byte 0x005b0254
	.4byte 0x025cfff4
	.4byte 0x000fffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSuharaHeyaExits
gSuharaHeyaExits:
	.4byte 0x000000a8
	.4byte 0x001010a7
	.4byte 0x002020a7
	.4byte 0x003030a7
	.4byte 0x004040a7
	.4byte 0x005050a7
	.4byte 0x00a0c0a8
	.4byte 0x00b0d0a8
	.4byte 0x00c0a0a8
	.4byte 0x00d0b0a8
	.4byte 0x00e0f0a8
	.4byte 0x00f0e0a8
	.4byte 0x000001ff
	.global gSuharaHeyaPlacements
gSuharaHeyaPlacements:
	.4byte 0xffff0066
	.4byte 0x00000002
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00004000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x0001c000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0xffff006f
	.4byte 0x00000002
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff0076
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00004000
	.4byte 0xffff0077
	.4byte 0x00000001
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00004000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x0001e000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00014000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00014000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00014000
	.4byte 0xffff007a
	.4byte 0x00000001
	.4byte 0x01b00000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSuharaHeyaPlacements96f
gSuharaHeyaPlacements96f:
	.4byte 0xffff0066
	.4byte 0x00000001
	.4byte 0x01900000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x0001c000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x0001c000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0xffff006f
	.4byte 0x00000002
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff0076
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00004000
	.4byte 0xffff0077
	.4byte 0x00000001
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00004000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x0001e000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x02b00000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00012000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x0001a000
	.4byte 0xffff006d
	.4byte 0x00000002
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00004000
	.4byte 0xffff007a
	.4byte 0x00000001
	.4byte 0x01b00000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00004000
	.4byte 0x09b00039
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00020000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00024000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSuharaHeyaEvents
gSuharaHeyaEvents:
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x0200821d
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x0200821d
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x0200821d
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x0200821d
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x0200821d
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x0200821d
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x0200821d
	.4byte 0x00000002
	.4byte 0xffff000c
	.4byte 0x0200821d
	.4byte 0x00000002
	.4byte 0xffff000d
	.4byte 0x0200821d
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000025c7
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000025c8
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000025c9
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000025ca
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000025cb
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000025cc
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000025cd
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000025ce
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0200806d
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000025d0
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x02008101
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000025d2
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000025d3
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000025d4
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x0200816d
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000025d6
	.4byte 0x00000173
	.4byte 0x0fb00064
	.4byte 0x001000e2
	.4byte 0x00000173
	.4byte 0xffff0064
	.4byte 0x004029bd
	.4byte 0x00000173
	.4byte 0xffff0065
	.4byte 0x004029bc
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSuharaHeyaEvents96f
gSuharaHeyaEvents96f:
	.4byte 0x00000002
	.4byte 0x09b00032
	.4byte 0x02008241
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x0200821d
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x0200821d
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x0200821d
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x0200821d
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x0200821d
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x0200821d
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x0200821d
	.4byte 0x00000002
	.4byte 0xffff000c
	.4byte 0x0200821d
	.4byte 0x00000002
	.4byte 0xffff000d
	.4byte 0x0200821d
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000025e9
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000025ea
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00002612
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00002613
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002614
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002615
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00002616
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00002617
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002618
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002619
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x0000261a
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000261b
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0200806d
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000261f
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x02008101
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002621
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002627
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002628
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00002622
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002623
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x020081d5
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002629
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x0000262a
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x0000262b
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x0200816d
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x0000262d
	.4byte 0x00000173
	.4byte 0x0fb00064
	.4byte 0x001000e2
	.4byte 0x00000173
	.4byte 0xffff0064
	.4byte 0x004029bd
	.4byte 0x00000173
	.4byte 0xffff0065
	.4byte 0x004029bc
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
