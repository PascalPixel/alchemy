.syntax unified
	.thumb
	.section .text.x020095a0,"ax",%progbits
	.balign 4
	.global Func_020015a0
	.thumb_func
Func_020015a0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r1, #132
	lsls r1, r1, #1
	adds r1, r1, r3
	ldr r3, .L_02009610
	mov r8, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	movs r6, #0
	lsls r3, r2, #2
	adds r3, r3, r2
	ldr r2, .L_02009614
	lsls r3, r3, #6
	adds r5, r3, r2
.L_020095c6:
	ldr r7, .L_02009618
	mov r1, r8
	ldrb r0, [r7]
	movs r2, #6
	ldrsh r3, [r1, r2]
	subs r0, r0, r6
	subs r0, r0, r3
	adds r0, #160
	lsls r0, r0, #9
	bl Math_Sine
	movs r2, #128
	lsls r2, r2, #9
	adds r0, r0, r2
	mov r2, r8
	movs r1, #6
	ldrsh r3, [r2, r1]
	asrs r0, r0, #15
	adds r3, r3, r0
	adds r6, #1
	strh r3, [r5]
	adds r5, #2
	cmp r6, #160
	bne .L_020095c6
	ldr r1, .L_02009610
	ldr r2, .L_0200960c
	ldrh r3, [r1]
	eors r3, r2
	strh r3, [r1]
	ldr r3, [r7]
	adds r3, #1
	str r3, [r7]
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200960c:
	.4byte 0x00000001
.L_02009610:
	.4byte gData_02003c14
.L_02009614:
	.4byte gData_02003990
.L_02009618:
	.4byte gData_02003c10
	.section .bss,"aw",%nobits
	.space 0x00000008
	.global gData_02003990
gData_02003990:
	.space 0x00000280
	.global gData_02003c10
gData_02003c10:
	.space 0x00000004
	.global gData_02003c14
gData_02003c14:
