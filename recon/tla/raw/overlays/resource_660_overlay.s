.syntax unified
	.thumb
	.section .text.x020084f0,"ax",%progbits
	.balign 4
	.global Func_020004f0
	.thumb_func
Func_020004f0:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #27
	bl GameFlag_Test
	ldr r1, .L_02008558
	ldr r4, .L_0200855c
	cmp r0, #0
	beq .L_0200851c
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r1, #0
	ldrsh r2, [r3, r1]
	cmp r2, r4
	beq .L_02008538
	ldr r3, .L_02008560
	cmp r2, r3
	bne .L_0200854a
	ldr r0, .L_02008564
	b .L_02008556
.L_0200851c:
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	cmp r2, r4
	bne .L_02008540
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #99
	bne .L_0200853c
.L_02008538:
	ldr r0, .L_02008568
	b .L_02008556
.L_0200853c:
	ldr r0, .L_0200856c
	b .L_02008556
.L_02008540:
	ldr r3, .L_02008560
	cmp r2, r3
	bne .L_0200854a
	ldr r0, .L_02008570
	b .L_02008556
.L_0200854a:
	ldr r3, .L_02008574
	cmp r2, r3
	bne .L_02008554
	ldr r0, .L_02008578
	b .L_02008556
.L_02008554:
	ldr r0, .L_0200857c
.L_02008556:
	pop {pc}
.L_02008558:
	.4byte gPartyState
.L_0200855c:
	.4byte 0x00000040
.L_02008560:
	.4byte 0x00000041
.L_02008564:
	.4byte Data_02000fe8
.L_02008568:
	.4byte Data_02000e44
.L_0200856c:
	.4byte Data_02000b80
.L_02008570:
	.4byte Data_02000d18
.L_02008574:
	.4byte 0x00000042
.L_02008578:
	.4byte Data_02000de4
.L_0200857c:
	.4byte Data_02000b5c
