.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_MACHI/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	push {lr}
	movs r0, #23
	movs r1, #2
	movs r2, #6
	bl 0x02009610
	pop {r0}
	bx r0
	.global Func_02000040
	.thumb_func
Func_02000040:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200975c
	.global Func_02000048
	.thumb_func
Func_02000048:
	movs r0, #0
	bx lr
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x020098c4
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {lr}
	ldr r0, [pc, #20]
	bl 0x02009518
	cmp r0, #0
	beq .L_02000054_0
	ldr r0, [pc, #12]
	b .L_02000054_1
.L_02000054_0:
	ldr r0, [pc, #12]
.L_02000054_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x000009a7
	.4byte 0x02009a98
	.4byte 0x02009900
	.global Func_02000078
	.thumb_func
Func_02000078:
	push {r5, r6, lr}
	adds r6, r0, #0
	bl 0x02009550
	movs r5, #128
	lsls r5, r5, #9
	str r5, [r0, #24]
	adds r0, r6, #0
	bl 0x02009550
	str r5, [r0, #28]
	ldr r0, [pc, #44]
	bl 0x020095c8
	adds r0, r6, #0
	movs r1, #0
	bl 0x020095d8
	movs r1, #192
	adds r0, r6, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x020095e0
	movs r0, #20
	bl 0x02009528
	ldr r1, [pc, #16]
	adds r0, r6, #0
	bl 0x02009560
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x000026af
	.4byte 0x02009638
	.global Func_020000c4
	.thumb_func
Func_020000c4:
	push {r5, r6, lr}
	ldr r5, [pc, #64]
	adds r6, r0, #0
	adds r0, r5, #0
	bl 0x020095c8
	movs r1, #0
	adds r0, r6, #0
	bl 0x020095d0
	movs r0, #0
	movs r1, #0
	bl 0x02009548
	cmp r0, #0
	bne .L_020000c4_0
	movs r0, #10
	bl 0x02009528
	adds r0, r5, #1
	bl 0x020095c8
	b .L_020000c4_1
.L_020000c4_0:
	adds r0, r5, #2
	bl 0x020095c8
.L_020000c4_1:
	adds r0, r6, #0
	movs r1, #0
	bl 0x020095d8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000028be
	.global Func_0200010c
	.thumb_func
Func_0200010c:
	push {lr}
	ldr r0, [pc, #64]
	bl 0x02009520
	ldr r0, [pc, #60]
	bl 0x020095c8
	movs r0, #18
	movs r1, #0
	bl 0x020095d8
	movs r1, #128
	movs r2, #128
	movs r0, #18
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x02009558
	movs r1, #16
	movs r0, #18
	negs r1, r1
	movs r2, #0
	bl 0x02009628
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl 0x020095e0
	movs r0, #10
	bl 0x02009528
	pop {r0}
	bx r0
	.4byte 0x000009bb
	.4byte 0x000028b8
	.global Func_02000158
	.thumb_func
Func_02000158:
	push {r5, r6, r7, lr}
	ldr r3, [pc, #132]
	movs r1, #182
	ldr r3, [r3]
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	ldrsh r7, [r3, r2]
	ldr r2, [pc, #120]
	lsls r3, r7, #2
	ldrsh r5, [r2, r3]
	movs r0, #0
	adds r3, #2
	ldrsh r6, [r2, r3]
	bl 0x02009550
	movs r3, #2
	adds r0, #85
	strb r3, [r0]
	movs r0, #158
	bl 0x02009630
	cmp r7, #6
	bne .L_02000158_0
	lsls r1, r5, #16
	lsls r2, r6, #16
	lsrs r1, r1, #16
	lsrs r2, r2, #16
	ldr r0, [pc, #84]
	bl 0x020094f0
	movs r2, #16
	movs r0, #0
	movs r1, #0
	negs r2, r2
	bl 0x02009620
	b .L_02000158_1
.L_02000158_0:
	lsls r1, r5, #16
	lsls r2, r6, #16
	lsrs r1, r1, #16
	lsrs r2, r2, #16
	ldr r0, [pc, #60]
	bl 0x020094f0
	movs r2, #16
	movs r0, #0
	movs r1, #2
	negs r2, r2
	bl 0x02009578
.L_02000158_1:
	movs r0, #10
	bl 0x02009528
	ldr r3, [pc, #24]
	movs r2, #228
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #16
	str r2, [r3]
	adds r0, r7, #0
	bl 0x020095f8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x02009ca8
	.4byte 0x02009cee
	.4byte 0x02009cd8
	.global Func_020001f0
	.thumb_func
Func_020001f0:
	push {lr}
	ldr r3, [pc, #32]
	movs r1, #182
	ldr r2, [r3]
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r3, #228
	lsls r3, r3, #1
	adds r2, r2, r3
	movs r3, #16
	str r3, [r2]
	bl 0x020095f8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.global Func_02000218
	.thumb_func
Func_02000218:
	push {lr}
	bl 0x02009530
	ldr r0, [pc, #1016]
	bl 0x020095c8
	movs r1, #248
	movs r2, #212
	movs r0, #0
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl 0x02009590
	movs r1, #192
	movs r2, #0
	movs r0, #0
	lsls r1, r1, #8
	bl 0x020095e0
	movs r0, #8
	movs r1, #0
	bl 0x02009598
	movs r1, #0
	movs r0, #9
	bl 0x02009598
	bl 0x02009600
	bl 0x02009608
	movs r0, #20
	bl 0x02009528
	movs r2, #16
	movs r3, #192
	lsls r3, r3, #8
	movs r1, #8
	negs r2, r2
	movs r0, #22
	bl 0x02009618
	movs r0, #22
	bl 0x02009588
	movs r0, #20
	bl 0x02009528
	movs r1, #129
	movs r0, #22
	lsls r1, r1, #1
	movs r2, #60
	bl 0x020095f0
	movs r1, #128
	movs r2, #128
	movs r0, #22
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x02009558
	movs r2, #16
	movs r1, #0
	negs r2, r2
	movs r0, #22
	bl 0x02009628
	movs r0, #10
	bl 0x02009528
	movs r0, #22
	movs r1, #8
	movs r2, #40
	bl 0x020095b8
	movs r0, #22
	movs r1, #9
	movs r2, #40
	bl 0x020095b8
	movs r2, #40
	movs r1, #8
	movs r0, #22
	bl 0x020095b8
	movs r0, #10
	bl 0x02009528
	movs r1, #2
	movs r0, #22
	bl 0x020095b0
	movs r0, #20
	bl 0x02009528
	movs r1, #0
	movs r0, #22
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #22
	bl 0x020095e0
	movs r0, #30
	bl 0x02009528
	movs r1, #0
	movs r0, #22
	bl 0x020095d0
	movs r1, #0
	movs r0, #0
	bl 0x02009548
	movs r0, #30
	bl 0x02009528
	movs r1, #2
	movs r0, #8
	bl 0x020095b0
	movs r0, #20
	bl 0x02009528
	movs r1, #0
	movs r0, #8
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r2, #30
	movs r0, #22
	movs r1, #8
	bl 0x020095b8
	movs r1, #2
	movs r0, #22
	bl 0x020095b0
	movs r0, #20
	bl 0x02009528
	movs r0, #22
	movs r1, #0
	bl 0x020095d8
	movs r1, #1
	movs r0, #8
	bl 0x02009598
	movs r0, #20
	bl 0x02009528
	movs r1, #2
	movs r0, #8
	bl 0x020095b0
	movs r0, #30
	bl 0x02009528
	movs r1, #0
	movs r0, #8
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #3
	movs r0, #22
	bl 0x020095a0
	movs r0, #20
	bl 0x02009528
	movs r1, #0
	movs r0, #22
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #2
	movs r0, #9
	bl 0x020095b0
	movs r0, #20
	bl 0x02009528
	movs r1, #0
	movs r0, #9
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r0, #22
	movs r1, #9
	movs r2, #0
	bl 0x020095b8
	movs r2, #30
	movs r0, #0
	movs r1, #9
	bl 0x020095b8
	movs r1, #1
	movs r0, #9
	bl 0x02009598
	movs r0, #10
	bl 0x02009528
	movs r1, #132
	movs r2, #40
	movs r0, #22
	lsls r1, r1, #1
	bl 0x020095f0
	movs r1, #0
	movs r0, #22
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #3
	movs r0, #0
	bl 0x020095a0
	movs r0, #20
	bl 0x02009528
	movs r0, #10
	bl 0x02009528
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #22
	bl 0x020095e0
	movs r0, #30
	bl 0x02009528
	movs r1, #2
	movs r0, #22
	bl 0x020095b0
	movs r0, #20
	bl 0x02009528
	movs r1, #0
	movs r0, #22
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #2
	movs r0, #9
	bl 0x020095b0
	movs r0, #30
	bl 0x02009528
	movs r1, #0
	movs r0, #9
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #129
	movs r0, #22
	lsls r1, r1, #1
	movs r2, #50
	bl 0x020095f0
	movs r2, #20
	movs r0, #22
	movs r1, #9
	bl 0x020095b8
	movs r1, #0
	movs r0, #22
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #2
	movs r0, #8
	bl 0x020095b0
	movs r0, #20
	bl 0x02009528
	movs r1, #0
	movs r0, #8
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #22
	bl 0x020095e0
	movs r0, #30
	bl 0x02009528
	movs r1, #0
	movs r0, #22
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #3
	movs r0, #0
	bl 0x020095a0
	movs r0, #20
	bl 0x02009528
	movs r0, #20
	bl 0x02009528
	movs r1, #129
	movs r0, #22
	lsls r1, r1, #1
	movs r2, #50
	bl 0x020095f0
	movs r0, #22
	ldr r1, [pc, #340]
	ldr r2, [pc, #340]
	bl 0x02009558
	movs r1, #128
	movs r2, #180
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #22
	bl 0x02009570
	movs r0, #10
	bl 0x02009528
	movs r1, #0
	movs r2, #0
	movs r0, #22
	bl 0x020095e0
	movs r0, #30
	bl 0x02009528
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #22
	bl 0x020095e0
	movs r0, #30
	bl 0x02009528
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #22
	bl 0x020095e0
	movs r0, #30
	bl 0x02009528
	movs r1, #0
	movs r0, #22
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #2
	movs r0, #9
	bl 0x020095b0
	movs r0, #20
	bl 0x02009528
	movs r1, #0
	movs r0, #9
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #128
	movs r0, #22
	lsls r1, r1, #1
	movs r2, #40
	bl 0x020095f0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #22
	bl 0x020095e0
	movs r0, #20
	bl 0x02009528
	movs r1, #0
	movs r0, #22
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #3
	movs r0, #9
	bl 0x020095a0
	movs r0, #30
	bl 0x02009528
	movs r1, #0
	movs r0, #9
	bl 0x020095d8
	movs r0, #20
	bl 0x02009528
	movs r1, #2
	movs r0, #22
	bl 0x020095b0
	movs r0, #20
	bl 0x02009528
	movs r0, #22
	ldr r1, [pc, #136]
	ldr r2, [pc, #140]
	bl 0x02009558
	movs r1, #128
	movs r2, #192
	movs r0, #22
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x02009570
	movs r2, #0
	movs r1, #0
	movs r0, #22
	bl 0x020095e0
	movs r0, #20
	bl 0x02009528
	movs r1, #0
	movs r0, #22
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #2
	movs r0, #8
	bl 0x020095b0
	movs r0, #20
	bl 0x02009528
	movs r1, #0
	movs r0, #8
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #22
	bl 0x020095e0
	movs r0, #40
	bl 0x02009528
	movs r1, #3
	movs r0, #22
	bl 0x020095a0
	movs r0, #20
	bl 0x02009528
	movs r1, #0
	movs r0, #22
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	b .L_02000218_0
	.4byte 0x00002694
	.4byte 0x0001cccc
	.4byte 0x0000e666
	.4byte 0x00019999
	.4byte 0x0000cccc
.L_02000218_0:
	movs r0, #22
	ldr r1, [pc, #392]
	ldr r2, [pc, #392]
	bl 0x02009558
	movs r2, #16
	movs r1, #0
	movs r0, #22
	bl 0x02009628
	movs r0, #10
	bl 0x02009528
	movs r1, #0
	movs r0, #22
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #80
	bl 0x020095f0
	movs r0, #22
	ldr r1, [pc, #348]
	movs r2, #80
	bl 0x020095f0
	movs r0, #22
	movs r1, #8
	movs r2, #40
	bl 0x020095b8
	movs r0, #22
	movs r1, #9
	movs r2, #40
	bl 0x020095b8
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #22
	bl 0x020095e0
	movs r0, #30
	bl 0x02009528
	movs r1, #2
	movs r0, #22
	bl 0x020095b0
	movs r0, #20
	bl 0x02009528
	movs r1, #0
	movs r0, #22
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #2
	movs r0, #9
	bl 0x020095b0
	movs r0, #20
	bl 0x02009528
	movs r1, #0
	movs r0, #9
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl 0x020095e0
	movs r2, #30
	movs r1, #9
	movs r0, #0
	bl 0x020095b8
	movs r0, #10
	bl 0x02009528
	movs r1, #2
	movs r0, #8
	bl 0x020095b0
	movs r0, #20
	bl 0x02009528
	movs r1, #0
	movs r0, #8
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #128
	movs r0, #22
	lsls r1, r1, #8
	movs r2, #0
	bl 0x020095e0
	movs r0, #0
	movs r1, #8
	movs r2, #70
	bl 0x020095b8
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #22
	bl 0x020095e0
	movs r0, #40
	bl 0x02009528
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #0
	bl 0x020095e0
	movs r0, #40
	bl 0x02009528
	movs r1, #2
	movs r0, #22
	bl 0x020095b0
	movs r0, #20
	bl 0x02009528
	movs r1, #0
	movs r0, #22
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #3
	movs r0, #0
	bl 0x020095a0
	movs r0, #20
	bl 0x02009528
	movs r1, #3
	movs r0, #22
	bl 0x020095a0
	movs r0, #30
	bl 0x02009528
	movs r0, #22
	ldr r1, [pc, #68]
	ldr r2, [pc, #68]
	bl 0x02009558
	movs r0, #22
	movs r1, #2
	bl 0x02009598
	movs r0, #0
	bl 0x02009550
	cmp r0, #0
	beq .L_02000218_1
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #22
	bl 0x02009568
.L_02000218_1:
	movs r0, #22
	bl 0x02009588
	movs r1, #0
	movs r2, #0
	movs r0, #22
	bl 0x02009590
	movs r0, #10
	bl 0x02009528
	bl 0x02009538
	pop {r0}
	bx r0
	.4byte 0x00013333
	.4byte 0x00009999
	.4byte 0x00000101
	.global Func_020007c4
	.thumb_func
Func_020007c4:
	push {r5, r6, lr}
	ldr r0, [pc, #184]
	sub sp, #8
	bl 0x02009520
	bl 0x02009530
	ldr r0, [pc, #176]
	bl 0x020095c8
	movs r2, #188
	movs r0, #0
	movs r1, #104
	lsls r2, r2, #1
	bl 0x02009570
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x020095e0
	movs r1, #32
	movs r0, #1
	negs r1, r1
	movs r2, #0
	movs r3, #0
	bl 0x02009618
	movs r1, #16
	movs r3, #224
	movs r0, #3
	negs r1, r1
	movs r2, #16
	lsls r3, r3, #8
	bl 0x02009618
	movs r3, #192
	lsls r3, r3, #8
	movs r2, #16
	movs r1, #0
	movs r0, #2
	bl 0x02009618
	movs r0, #1
	bl 0x02009588
	movs r0, #30
	bl 0x02009528
	movs r1, #0
	movs r0, #1
	bl 0x020095d0
	movs r0, #10
	bl 0x02009528
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #0
	movs r2, #0
	bl 0x020095e0
	movs r0, #10
	bl 0x02009528
	movs r0, #0
	movs r1, #0
	bl 0x02009548
	cmp r0, #0
	bne .L_020007c4_0
	movs r0, #20
	bl 0x02009528
	movs r1, #4
	movs r0, #3
	bl 0x020095a0
	movs r0, #20
	bl 0x02009528
	movs r0, #3
	movs r1, #0
	bl 0x020095d8
	ldr r3, [pc, #24]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_020007c4_1
	.4byte 0x000009ba
	.4byte 0x0000288e
	.4byte 0x03001ebc
.L_020007c4_0:
	movs r0, #20
	bl 0x02009528
	movs r1, #4
	movs r0, #3
	bl 0x020095a0
	movs r0, #20
	bl 0x02009528
	ldr r3, [pc, #876]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #3
	movs r1, #0
	bl 0x020095d8
.L_020007c4_1:
	movs r0, #10
	bl 0x02009528
	movs r1, #132
	movs r2, #40
	movs r0, #2
	lsls r1, r1, #1
	bl 0x020095f0
	movs r1, #0
	movs r0, #2
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #3
	movs r0, #0
	bl 0x02009598
	movs r0, #40
	bl 0x02009528
	movs r0, #1
	movs r1, #3
	bl 0x02009598
	movs r0, #2
	movs r1, #3
	bl 0x02009598
	movs r1, #3
	movs r0, #3
	bl 0x020095a0
	movs r0, #30
	bl 0x02009528
	movs r1, #16
	movs r0, #0
	negs r1, r1
	movs r2, #0
	bl 0x02009628
	movs r2, #16
	movs r0, #2
	movs r1, #0
	negs r2, r2
	bl 0x02009620
	movs r2, #8
	movs r0, #3
	movs r1, #0
	negs r2, r2
	bl 0x02009620
	movs r2, #16
	negs r2, r2
	movs r0, #0
	movs r1, #0
	bl 0x02009628
	movs r0, #3
	movs r1, #1
	bl 0x02009598
	movs r0, #2
	movs r1, #1
	bl 0x02009598
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x020095e0
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x020095e0
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x020095e0
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x020095e0
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #22
	bl 0x020095e0
	movs r0, #80
	bl 0x02009528
	ldr r2, [pc, #652]
	ldr r1, [pc, #652]
	movs r0, #22
	bl 0x02009558
	movs r0, #22
	bl 0x02009550
	movs r3, #2
	adds r0, #85
	strb r3, [r0]
	movs r1, #2
	movs r0, #22
	bl 0x020095e8
	movs r5, #4
	movs r3, #2
	movs r1, #0
	movs r2, #1
	movs r6, #18
	movs r0, #34
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x02009500
	movs r0, #158
	bl 0x02009630
	movs r0, #20
	bl 0x02009528
	movs r1, #144
	movs r2, #156
	lsls r1, r1, #15
	lsls r2, r2, #17
	movs r0, #22
	bl 0x02009590
	movs r0, #20
	bl 0x02009528
	movs r0, #22
	movs r1, #0
	movs r2, #16
	bl 0x02009628
	movs r3, #2
	movs r2, #1
	movs r1, #0
	movs r0, #32
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x02009500
	movs r0, #159
	bl 0x02009630
	movs r0, #20
	bl 0x02009528
	movs r1, #0
	movs r0, #22
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r0, #0
	movs r1, #3
	bl 0x02009598
	movs r0, #1
	movs r1, #3
	bl 0x02009598
	movs r0, #2
	movs r1, #3
	bl 0x02009598
	movs r1, #3
	movs r0, #3
	bl 0x020095a0
	movs r0, #40
	bl 0x02009528
	movs r0, #22
	movs r1, #16
	movs r2, #0
	bl 0x02009628
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #22
	bl 0x020095e0
	movs r0, #20
	bl 0x02009528
	movs r0, #10
	bl 0x02009528
	movs r1, #3
	movs r0, #22
	bl 0x020095a0
	movs r0, #30
	bl 0x02009528
	movs r1, #0
	movs r0, #22
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #3
	movs r0, #3
	bl 0x020095a0
	movs r0, #30
	bl 0x02009528
	movs r1, #0
	movs r0, #3
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #2
	movs r0, #22
	bl 0x020095b0
	movs r0, #20
	bl 0x02009528
	movs r1, #0
	movs r0, #22
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #2
	movs r0, #1
	bl 0x020095b0
	movs r0, #20
	bl 0x02009528
	movs r1, #0
	movs r0, #1
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #129
	movs r2, #40
	movs r0, #22
	lsls r1, r1, #1
	bl 0x020095f0
	movs r1, #0
	movs r0, #22
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #128
	movs r2, #40
	movs r0, #2
	lsls r1, r1, #1
	bl 0x020095f0
	movs r1, #0
	movs r0, #2
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #3
	movs r0, #22
	bl 0x020095a0
	movs r0, #30
	bl 0x02009528
	movs r1, #0
	movs r0, #22
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x020095c0
	movs r1, #2
	movs r2, #0
	movs r0, #3
	bl 0x020095c0
	movs r0, #60
	bl 0x02009528
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x020095e0
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x020095e0
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x020095e0
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #3
	bl 0x020095e0
	movs r0, #40
	bl 0x02009528
	movs r0, #10
	bl 0x02009528
	movs r1, #3
	movs r0, #22
	bl 0x020095a0
	movs r0, #30
	bl 0x02009528
	movs r1, #0
	movs r0, #22
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #129
	movs r2, #40
	movs r0, #3
	lsls r1, r1, #1
	bl 0x020095f0
	movs r1, #0
	movs r0, #3
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #3
	movs r0, #22
	bl 0x020095a0
	movs r0, #20
	bl 0x02009528
	movs r1, #0
	movs r0, #22
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x020095e0
	movs r0, #20
	bl 0x02009528
	movs r1, #0
	movs r0, #1
	bl 0x020095d0
	movs r0, #0
	movs r1, #0
	bl 0x02009548
	cmp r0, #0
	bne .L_020007c4_2
	movs r0, #20
	bl 0x02009528
	movs r1, #4
	movs r0, #22
	bl 0x020095a0
	movs r0, #20
	bl 0x02009528
	movs r0, #22
	movs r1, #0
	bl 0x020095d8
	ldr r3, [pc, #16]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_020007c4_3
	.4byte 0x03001ebc
	.4byte 0x00006666
	.4byte 0x0000cccc
.L_020007c4_2:
	movs r0, #20
	bl 0x02009528
	movs r1, #4
	movs r0, #22
	bl 0x020095a0
	movs r0, #20
	bl 0x02009528
	ldr r3, [pc, #956]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #22
	movs r1, #0
	bl 0x020095d8
.L_020007c4_3:
	movs r0, #10
	bl 0x02009528
	movs r1, #129
	movs r2, #40
	movs r0, #2
	lsls r1, r1, #1
	bl 0x020095f0
	movs r1, #0
	movs r0, #2
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x020095e0
	movs r1, #131
	movs r2, #50
	movs r0, #22
	lsls r1, r1, #1
	bl 0x020095f0
	movs r1, #0
	movs r0, #22
	bl 0x020095d8
	movs r0, #20
	bl 0x02009528
	movs r1, #176
	movs r2, #166
	lsls r2, r2, #17
	movs r0, #25
	lsls r1, r1, #15
	bl 0x02009590
	movs r0, #1
	movs r1, #0
	negs r0, r0
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #40
	bl 0x020095f0
	movs r0, #3
	movs r1, #4
	movs r2, #13
	bl 0x020095a8
	movs r2, #30
	movs r0, #3
	movs r1, #4
	bl 0x020095a8
	movs r1, #0
	movs r0, #3
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #3
	movs r0, #22
	bl 0x020095a0
	movs r0, #30
	bl 0x02009528
	movs r0, #22
	movs r1, #0
	bl 0x020095d8
	ldr r0, [pc, #768]
	bl 0x02009518
	cmp r0, #0
	beq .L_020007c4_4
	bl 0x02009090
.L_020007c4_4:
	ldr r0, [pc, #760]
	bl 0x020095c8
	movs r0, #10
	bl 0x02009528
	movs r1, #2
	movs r0, #22
	bl 0x020095b0
	movs r0, #20
	bl 0x02009528
	movs r1, #0
	movs r0, #22
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #3
	movs r0, #22
	bl 0x020095a0
	movs r0, #30
	bl 0x02009528
	movs r1, #0
	movs r0, #22
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #128
	movs r2, #40
	movs r0, #1
	lsls r1, r1, #1
	bl 0x020095f0
	movs r1, #0
	movs r0, #1
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #2
	movs r0, #22
	bl 0x020095b0
	movs r0, #20
	bl 0x02009528
	movs r1, #0
	movs r0, #22
	bl 0x020095d8
	movs r0, #20
	bl 0x02009528
	movs r1, #3
	movs r0, #2
	bl 0x020095a0
	movs r0, #30
	bl 0x02009528
	movs r1, #0
	movs r0, #2
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #3
	movs r0, #22
	bl 0x020095a0
	movs r0, #30
	bl 0x02009528
	movs r0, #25
	ldr r1, [pc, #596]
	ldr r2, [pc, #600]
	bl 0x02009558
	movs r0, #25
	movs r1, #0
	movs r2, #16
	bl 0x02009580
	movs r1, #0
	movs r2, #16
	movs r0, #22
	bl 0x02009628
	movs r0, #30
	bl 0x02009528
	movs r0, #25
	movs r1, #0
	movs r2, #0
	bl 0x02009590
	ldr r3, [pc, #540]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r0, #242
	bl 0x02009540
	movs r0, #10
	bl 0x02009528
	movs r0, #22
	bl 0x02009550
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r2, #16
	strb r3, [r0]
	movs r1, #0
	negs r2, r2
	movs r0, #22
	bl 0x02009628
	movs r0, #22
	bl 0x02009550
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	movs r1, #130
	strb r3, [r0]
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #22
	bl 0x020095e0
	movs r0, #30
	bl 0x02009528
	movs r0, #10
	bl 0x02009528
	movs r1, #128
	movs r2, #40
	movs r0, #22
	lsls r1, r1, #1
	bl 0x020095f0
	movs r1, #0
	movs r0, #22
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #3
	movs r0, #22
	bl 0x020095a0
	movs r0, #30
	bl 0x02009528
	movs r1, #0
	movs r0, #22
	bl 0x020095d8
	movs r0, #20
	bl 0x02009528
	movs r0, #0
	movs r1, #3
	bl 0x02009598
	movs r0, #1
	movs r1, #3
	bl 0x02009598
	movs r0, #2
	movs r1, #3
	bl 0x02009598
	movs r1, #3
	movs r0, #3
	bl 0x020095a0
	movs r0, #50
	bl 0x02009528
	movs r0, #10
	bl 0x02009528
	movs r1, #3
	movs r0, #22
	bl 0x020095a0
	movs r0, #30
	bl 0x02009528
	movs r1, #16
	movs r0, #22
	negs r1, r1
	movs r2, #0
	bl 0x02009628
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #22
	bl 0x020095e0
	movs r0, #20
	bl 0x02009528
	movs r5, #4
	movs r3, #2
	movs r1, #0
	movs r2, #1
	movs r6, #18
	movs r0, #34
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x02009500
	movs r0, #158
	bl 0x02009630
	movs r0, #10
	bl 0x02009528
	movs r2, #16
	movs r0, #22
	movs r1, #0
	negs r2, r2
	bl 0x02009628
	movs r1, #0
	movs r2, #0
	movs r0, #22
	bl 0x02009590
	movs r0, #10
	bl 0x02009528
	movs r3, #2
	movs r1, #0
	movs r2, #1
	movs r0, #32
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x02009500
	movs r0, #159
	bl 0x02009630
	movs r0, #50
	bl 0x02009528
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x020095e0
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x020095b8
	movs r2, #0
	movs r1, #0
	movs r0, #2
	bl 0x020095b8
	movs r0, #20
	bl 0x02009528
	movs r1, #0
	movs r0, #1
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #2
	movs r0, #3
	bl 0x020095b0
	movs r0, #20
	bl 0x02009528
	movs r1, #0
	movs r0, #3
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #3
	movs r0, #2
	bl 0x020095a0
	movs r0, #30
	bl 0x02009528
	movs r1, #0
	movs r0, #2
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #3
	movs r0, #0
	bl 0x02009598
	movs r0, #40
	bl 0x02009528
	movs r0, #1
	movs r1, #3
	bl 0x02009598
	movs r0, #2
	movs r1, #3
	bl 0x02009598
	movs r1, #3
	movs r0, #3
	bl 0x020095a0
	movs r0, #30
	bl 0x02009528
	movs r0, #1
	ldr r1, [pc, #80]
	ldr r2, [pc, #80]
	bl 0x02009558
	movs r0, #3
	ldr r1, [pc, #68]
	ldr r2, [pc, #72]
	bl 0x02009558
	movs r0, #2
	ldr r1, [pc, #60]
	ldr r2, [pc, #60]
	bl 0x02009558
	movs r0, #1
	movs r1, #2
	bl 0x02009598
	movs r0, #0
	bl 0x02009550
	cmp r0, #0
	beq .L_020007c4_5
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x02009568
	b .L_020007c4_5
	.4byte 0x03001ebc
	.4byte 0x000009bf
	.4byte 0x000028a5
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x00013333
	.4byte 0x00009999
.L_020007c4_5:
	movs r0, #1
	bl 0x02009588
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x02009590
	movs r0, #3
	movs r1, #2
	bl 0x02009598
	movs r0, #0
	bl 0x02009550
	cmp r0, #0
	beq .L_020007c4_6
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #3
	bl 0x02009568
.L_020007c4_6:
	movs r0, #3
	bl 0x02009588
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl 0x02009590
	movs r0, #2
	movs r1, #2
	bl 0x02009598
	movs r0, #0
	bl 0x02009550
	cmp r0, #0
	beq .L_020007c4_7
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #2
	bl 0x02009568
.L_020007c4_7:
	movs r0, #2
	bl 0x02009588
	movs r1, #0
	movs r2, #0
	movs r0, #2
	bl 0x02009590
	movs r0, #10
	bl 0x02009528
	bl 0x02009538
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001090
	.thumb_func
Func_02001090:
	push {lr}
	ldr r0, [pc, #292]
	bl 0x020095c8
	movs r0, #20
	bl 0x02009528
	movs r1, #2
	movs r0, #22
	bl 0x020095b0
	movs r0, #20
	bl 0x02009528
	movs r1, #0
	movs r0, #22
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #4
	movs r0, #22
	bl 0x020095a0
	movs r0, #20
	bl 0x02009528
	movs r1, #0
	movs r0, #22
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x020095f0
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x020095f0
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #0
	bl 0x020095f0
	movs r1, #128
	movs r2, #55
	lsls r1, r1, #1
	movs r0, #2
	bl 0x020095f0
	movs r0, #10
	bl 0x02009528
	movs r1, #2
	movs r0, #1
	bl 0x020095b0
	movs r0, #20
	bl 0x02009528
	movs r1, #0
	movs r0, #1
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #4
	movs r0, #22
	bl 0x020095a0
	movs r0, #20
	bl 0x02009528
	movs r1, #0
	movs r0, #22
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #129
	movs r2, #40
	movs r0, #3
	lsls r1, r1, #1
	bl 0x020095f0
	movs r1, #0
	movs r0, #3
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #3
	movs r0, #1
	bl 0x020095a0
	movs r0, #30
	bl 0x02009528
	movs r1, #0
	movs r0, #1
	bl 0x020095d8
	movs r0, #10
	bl 0x02009528
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #2
	bl 0x020095e0
	movs r0, #65
	bl 0x02009528
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #2
	bl 0x020095e0
	movs r0, #40
	bl 0x02009528
	movs r1, #3
	movs r0, #2
	bl 0x020095a0
	movs r0, #30
	bl 0x02009528
	movs r0, #2
	movs r1, #0
	bl 0x020095d8
	pop {r0}
	bx r0
	.4byte 0x000028b0
	.global Func_020011bc
	.thumb_func
Func_020011bc:
	push {lr}
	bl 0x02009530
	ldr r0, [pc, #32]
	bl 0x020095c8
	movs r0, #1
	movs r1, #0
	bl 0x020095d8
	movs r2, #16
	movs r0, #0
	movs r1, #0
	negs r2, r2
	bl 0x02009628
	bl 0x02009538
	pop {r0}
	bx r0
	.4byte 0x000028b7
	.global Func_020011e8
	.thumb_func
Func_020011e8:
	push {lr}
	ldr r0, [pc, #20]
	bl 0x02009518
	cmp r0, #0
	beq .L_020011e8_0
	ldr r0, [pc, #12]
	b .L_020011e8_1
.L_020011e8_0:
	ldr r0, [pc, #12]
.L_020011e8_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x000009a7
	.4byte 0x02009ee4
	.4byte 0x02009d04
	.global Func_0200120c
	.thumb_func
Func_0200120c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, [pc, #396]
	movs r0, #225
	lsls r0, r0, #1
	adds r0, r0, r3
	movs r1, #0
	ldrsh r3, [r0, r1]
	sub sp, #8
	mov r9, r0
	ldrh r2, [r0]
	cmp r3, #90
	bne .L_0200120c_0
	ldr r0, [pc, #376]
	bl 0x02009520
	ldr r0, [pc, #376]
	bl 0x02009520
	mov r3, r9
	ldrh r2, [r3]
.L_0200120c_0:
	movs r0, #182
	lsls r3, r2, #16
	lsls r0, r0, #15
	cmp r3, r0
	bne .L_0200120c_1
	ldr r0, [pc, #352]
	bl 0x02009520
.L_0200120c_1:
	ldr r3, [pc, #352]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	movs r0, #228
	adds r2, r1, r3
	lsls r0, r0, #1
	subs r3, #192
	str r3, [r2]
	adds r2, r1, r0
	movs r3, #24
	str r3, [r2]
	movs r1, #3
	movs r0, #19
	bl 0x02009598
	movs r0, #19
	bl 0x02009550
	movs r7, #0
	adds r0, #89
	strb r7, [r0]
	movs r0, #19
	bl 0x02009550
	movs r1, #0
	bl 0x02009508
	movs r1, #3
	movs r0, #20
	bl 0x02009598
	movs r0, #20
	bl 0x02009550
	adds r0, #89
	strb r7, [r0]
	movs r0, #20
	bl 0x02009550
	movs r1, #0
	bl 0x02009508
	movs r1, #3
	movs r0, #21
	bl 0x02009598
	movs r0, #21
	bl 0x02009550
	adds r0, #89
	strb r7, [r0]
	movs r0, #21
	bl 0x02009550
	movs r1, #0
	bl 0x02009508
	movs r0, #25
	bl 0x02009550
	movs r1, #0
	bl 0x02009508
	movs r0, #25
	bl 0x02009550
	movs r1, #1
	adds r3, r0, #0
	mov r10, r1
	adds r3, #92
	mov r2, r10
	strb r2, [r3]
	subs r3, #7
	strb r7, [r3]
	movs r3, #160
	ldr r6, [r0, #80]
	lsls r3, r3, #12
	str r3, [r0, #12]
	adds r3, r6, #0
	adds r3, #39
	strb r7, [r3]
	movs r3, #33
	ldrb r2, [r6, #5]
	negs r3, r3
	ands r3, r2
	ldrb r2, [r6, #9]
	strb r3, [r6, #5]
	movs r3, #15
	mov r8, r3
	movs r1, #193
	ands r3, r2
	strb r3, [r6, #9]
	lsls r1, r1, #3
	movs r0, #17
	bl 0x020094d8
	adds r5, r0, #0
	movs r0, #242
	bl 0x02009510
	movs r1, #128
	lsls r1, r1, #3
	adds r5, r5, r1
	adds r2, r5, #0
	movs r1, #128
	ldrb r0, [r6, #28]
	bl 0x020094e8
	movs r0, #17
	bl 0x020094e0
	ldr r0, [pc, #120]
	bl 0x02009518
	adds r6, r0, #0
	cmp r6, #0
	beq .L_0200120c_2
	movs r0, #24
	bl 0x02009550
	movs r1, #0
	bl 0x02009508
	movs r0, #24
	bl 0x02009550
	movs r3, #14
	adds r0, #89
	strb r7, [r0]
	movs r5, #4
	str r3, [sp, #0]
	movs r0, #20
	movs r1, #23
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x020094f8
	mov r2, r8
	str r2, [sp, #0]
	movs r0, #20
	movs r1, #23
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x020094f8
	movs r3, #16
	str r3, [sp, #0]
	movs r0, #20
	movs r1, #23
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x020094f8
	ldr r0, [pc, #44]
	bl 0x02009518
	cmp r0, #0
	bne .L_0200120c_3
	b .L_0200120c_4
.L_0200120c_3:
	movs r1, #224
	movs r2, #184
	movs r0, #18
	lsls r1, r1, #14
	lsls r2, r2, #16
	bl 0x02009590
	b .L_0200120c_4
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000009a7
	.4byte 0x000009bf
	.4byte 0x03001ebc
	.4byte 0x000009bb
.L_0200120c_2:
	movs r0, #8
	bl 0x02009550
	adds r3, r0, #0
	adds r3, #89
	strb r6, [r3]
	adds r2, r0, #0
	adds r2, #35
	ldrb r3, [r2]
	movs r5, #2
	orrs r3, r5
	strb r3, [r2]
	ldr r3, [r0, #80]
	adds r3, #38
	strb r6, [r3]
	movs r3, #192
	ldr r2, [r0, #80]
	lsls r3, r3, #8
	strh r3, [r2, #30]
	movs r0, #9
	bl 0x02009550
	ldr r6, [pc, #60]
	adds r3, r0, #0
	adds r3, #89
	strb r6, [r3]
	adds r2, r0, #0
	adds r2, #35
	ldrb r3, [r2]
	orrs r3, r5
	strb r3, [r2]
	ldr r3, [r0, #80]
	adds r3, #38
	strb r6, [r3]
	movs r3, #128
	ldr r2, [r0, #80]
	lsls r3, r3, #7
	strh r3, [r2, #30]
	movs r3, #13
	str r3, [sp, #0]
	movs r5, #23
	movs r0, #20
	movs r1, #23
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x020094f8
	movs r3, #14
	str r3, [sp, #0]
	movs r0, #20
	movs r1, #23
	b .L_0200120c_5
	.2byte 0x0000
	.4byte 0x00000000
.L_0200120c_5:
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x020094f8
	movs r3, #78
	str r3, [sp, #0]
	movs r0, #20
	movs r1, #23
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x020094f8
	movs r3, #17
	str r3, [sp, #0]
	movs r0, #20
	movs r1, #23
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x020094f8
	movs r3, #18
	str r3, [sp, #0]
	movs r0, #20
	movs r1, #23
	movs r3, #1
	movs r2, #1
	str r5, [sp, #4]
	bl 0x020094f8
	mov r0, r9
	ldrh r3, [r0]
	movs r1, #128
	subs r3, #20
	lsls r3, r3, #16
	lsls r1, r1, #9
	cmp r3, r1
	bhi .L_0200120c_4
	ldr r0, [pc, #88]
	bl 0x02009518
	cmp r0, #0
	bne .L_0200120c_4
	ldr r0, [pc, #80]
	bl 0x02009520
	movs r0, #11
	bl 0x02009550
	adds r3, r0, #0
	mov r2, r10
	adds r3, #91
	strb r2, [r3]
	movs r0, #17
	bl 0x02009550
	adds r3, r0, #0
	adds r3, #91
	mov r0, r10
	strb r0, [r3]
	bl 0x02008218
	movs r0, #11
	bl 0x02009550
	adds r3, r0, #0
	adds r3, #91
	strb r6, [r3]
	movs r0, #17
	bl 0x02009550
	adds r3, r0, #0
	adds r3, #91
	strb r6, [r3]
.L_0200120c_4:
	movs r0, #0
	sub sp, #-8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x000009b8
	.include "games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_MACHI/IMPORT.INC"
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000050
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000048
	.4byte 0x400000b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000098
	.4byte 0x400000d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x000000c8
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x00000128
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x00000168
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x000001b0
	.4byte 0x40000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0007
	.4byte 0x00000048
	.4byte 0x40000148
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x00000068
	.4byte 0x40000138
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0009
	.4byte 0x00000168
	.4byte 0x40000118
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x000001b8
	.4byte 0x40000148
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x000001a8
	.4byte 0x40000108
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0014
	.4byte 0x000000f8
	.4byte 0xc00001a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0015
	.4byte 0x000000f8
	.4byte 0x40000040
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000b2
	.4byte 0x0011e0b4
	.4byte 0x002010b3
	.4byte 0x003170b4
	.4byte 0x004060b3
	.4byte 0x005040b3
	.4byte 0x0060b0b3
	.4byte 0x007150b4
	.4byte 0x008160b4
	.4byte 0x009090b3
	.4byte 0x00a020b3
	.4byte 0x00b0a0b3
	.4byte 0x0141e002
	.4byte 0x0153b002
	.4byte 0x000001ff
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00024000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x01200000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00024000
	.4byte 0xffff006c
	.4byte 0x02009638
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x0000c000
	.4byte 0xffff006a
	.4byte 0x00000002
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0xffff008f
	.4byte 0x00000001
	.4byte 0x01300000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00014000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0xffff0067
	.4byte 0x00000002
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00004000
	.4byte 0xffff006f
	.4byte 0x00000002
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00008000
	.4byte 0xffff006b
	.4byte 0x00000002
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0xffff0066
	.4byte 0x00000002
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00004000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00004000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00004000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x00004000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00004000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00020000
	.4byte 0x005e005c
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00004000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00014000
	.4byte 0xffff006c
	.4byte 0x00000002
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00004000
	.4byte 0xffff006a
	.4byte 0x00000002
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0xffff008f
	.4byte 0x00000001
	.4byte 0x01300000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00004000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0xffff0067
	.4byte 0x00000002
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00004000
	.4byte 0xffff006f
	.4byte 0x00000002
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00008000
	.4byte 0xffff006b
	.4byte 0x00000002
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0xffff0066
	.4byte 0x00000002
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00008000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00004000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00004000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x00004000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00004000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00020000
	.4byte 0x005e005c
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0xffff0112
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00024000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00090004
	.4byte 0x000b0009
	.4byte 0x0008000c
	.4byte 0x00080012
	.4byte 0x00080016
	.4byte 0x0007001a
	.4byte 0x00120004
	.4byte 0x00000000
	.4byte 0x000f0016
	.4byte 0x0012001b
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0x00020001
	.4byte 0x00220008
	.4byte 0x00010000
	.4byte 0x00010002
	.4byte 0x0022ffff
	.4byte 0x00020002
	.4byte 0x00080002
	.4byte 0x00020024
	.4byte 0x00020002
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0xffff0014
	.4byte 0x00000014
	.4byte 0x00000001
	.4byte 0xffff0015
	.4byte 0x00000015
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008159
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008159
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008159
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02008159
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008159
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008159
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008159
	.4byte 0x00000002
	.4byte 0xffff0008
	.4byte 0x020081f1
	.4byte 0x0000c602
	.4byte 0xffff0009
	.4byte 0x02008159
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte 0x02008159
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x020081f1
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000026ad
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000026ae
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008079
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000026b0
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000026b1
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000026b2
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000026b3
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000026b4
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x000026b5
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x000026b6
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x000026f8
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000026bb
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000026bc
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000026bd
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000026be
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000026bf
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000026c0
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000026c1
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000026c2
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000026c3
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x000026c4
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000026f9
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x02008031
	.4byte 0x00000023
	.4byte 0x0fb60064
	.4byte 0x001000bd
	.4byte 0x00000133
	.4byte 0x0fb70065
	.4byte 0x001000ba
	.4byte 0x00008413
	.4byte 0x0fb80066
	.4byte 0x00100097
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x09ba0032
	.4byte 0x020087c5
	.4byte 0x00000002
	.4byte 0xffff0033
	.4byte 0x020091bd
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008159
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008159
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008159
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02008159
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008159
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008159
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008159
	.4byte 0x00000002
	.4byte 0xffff0008
	.4byte 0x020081f1
	.4byte 0x0000c602
	.4byte 0xffff0009
	.4byte 0x02008159
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte 0x02008159
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x020081f1
	.4byte 0x00000000
	.4byte 0x09bb0012
	.4byte 0x0200810d
	.4byte 0x00008d15
	.4byte 0x09bb0412
	.4byte 0x0200810d
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000028bb
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000028bc
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000028bd
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x020080c5
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000028c1
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000028c2
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000028c3
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000028c4
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x000028c5
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x000028c6
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x000028c7
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x000028b9
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000028c8
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000028c9
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000028ca
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000028cb
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000028cc
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000028cd
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000028ce
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000028cf
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000028d0
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x000028d1
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000028ba
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x02008031
	.4byte 0x00000023
	.4byte 0x0fb60064
	.4byte 0x001000bd
	.4byte 0x00000133
	.4byte 0x0fb70065
	.4byte 0x001000ba
	.4byte 0x00000013
	.4byte 0x0fb80066
	.4byte 0x00100097
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
