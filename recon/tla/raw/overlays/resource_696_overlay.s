
	.include "games/tla/include/ed_asm.h"
	.set SHAMAN_GRAPHIC1_SCALE, 256
	.set SHAMAN_GRAPHIC4_START_SCALE, 512
	.set SHAMAN_GRAPHIC4_SMALL_SCALE, 256
	.set SHAMAN_GRAPHIC4_LARGE_SCALE, 384
	.ifdef TLA_EDITION_ES
	.set SHAMAN_GRAPHIC4_START_SCALE, 384
	.endif
	.ifdef TLA_EDITION_IT
	.set SHAMAN_GRAPHIC4_START_SCALE, 384
	.endif
	.ifdef TLA_EDITION_FR
	.set SHAMAN_GRAPHIC1_SCALE, 384
	.set SHAMAN_GRAPHIC4_SMALL_SCALE, 384
	.set SHAMAN_GRAPHIC4_LARGE_SCALE, 512
	.endif
	.set SHAMAN_GRAPHIC7_PALETTE, (2 * 32)
	.ifdef TLA_EDITION_DE
	.set SHAMAN_GRAPHIC7_PALETTE, (5 * 32)
	.endif
	.ifdef TLA_EDITION_ES
	.set SHAMAN_GRAPHIC7_PALETTE, (5 * 32)
	.endif
	.ifdef TLA_EDITION_FR
	.set SHAMAN_GRAPHIC7_PALETTE, (5 * 32)
	.endif
	.ifdef TLA_EDITION_IT
	.set SHAMAN_GRAPHIC7_PALETTE, (5 * 32)
	.endif
.syntax unified
	.thumb
	.section .text.x0200a90c,"ax",%progbits
	.balign 4
	.global Func_0200290c
	.thumb_func
Func_0200290c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r0, .L_0200aa18
	sub sp, #20
	bl Func_0200498c
	movs r0, #65
	bl PartyInventory_FindOwner
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	beq .L_0200a9aa
	mov r0, sp
	bl Party_ListActiveOwners
	movs r3, #0
	movs r2, #8
	mov r9, r0
	mov r8, r3
	mov r7, sp
	mov r10, r2
	cmp r8, r9
	bge .L_0200a992
.L_0200a942:
	movs r1, #0
	ldrsh r5, [r7, r1]
	adds r7, #2
	adds r0, r5, #0
	bl Unit_Get
	adds r6, r0, #0
	movs r5, #0
	adds r6, #216
.L_0200a954:
	cmp r5, #14
	bgt .L_0200a984
	ldrh r0, [r6]
	cmp r0, #0
	beq .L_0200a984
	bl Func_02004894
	ldrb r3, [r0, #2]
	movs r2, #192
	adds r3, #255
	lsls r3, r3, #24
	lsls r2, r2, #18
	cmp r3, r2
	bhi .L_0200a97a
	mov r3, r10
	adds r3, #255
	lsls r3, r3, #24
	lsrs r3, r3, #24
	mov r10, r3
.L_0200a97a:
	mov r3, r10
	adds r6, #2
	adds r5, #1
	cmp r3, #0
	bne .L_0200a954
.L_0200a984:
	mov r1, r10
	cmp r1, #0
	beq .L_0200a992
	movs r2, #1
	add r8, r2
	cmp r8, r9
	blt .L_0200a942
.L_0200a992:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r3, r1
	ldrh r3, [r2]
	mov r1, r10
	adds r3, #1
	strh r3, [r2]
	cmp r1, #0
	beq .L_0200a9c8
.L_0200a9aa:
	movs r1, #2
	adds r1, #255
	movs r2, #20
	movs r0, #15
	bl Func_020049cc
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #15
	movs r1, #0
	bl Func_020049a4
	bl Func_0200289c
	b .L_0200aa0a
.L_0200a9c8:
	movs r0, #128
	adds r3, #1
	lsls r0, r0, #6
	strh r3, [r2]
	adds r0, #15
	movs r1, #0
	bl Func_020049a4
	ldr r5, .L_0200aa1c
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r1, [r5]
	movs r0, #16
	bl Actor_WatchAndSetScript
	ldr r1, [r5]
	movs r0, #17
	bl Actor_WatchAndSetScript
	ldr r1, [r5]
	movs r0, #15
	bl Actor_WatchAndSetScript
	movs r0, #158
	lsls r0, r0, #4
	bl GameFlag_SetBit
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
.L_0200aa0a:
	add sp, #20
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200aa18:
	.4byte MsgShamanMissingShamanRod
.L_0200aa1c:
	.4byte gMainRoutineWork
	.section .bss,"aw",%nobits
	.space 0x00000004
	.global gData_02006350
gData_02006350:
	.space 0x00000140
	.global gData_02006490
gData_02006490:
	.space 0x00000004
	.global gData_02006494
gData_02006494:
	.space 0x00000004
	.global gData_02006498
gData_02006498:
	.space 0x00000004
	.global gData_0200649c
gData_0200649c:
	.space 0x00000004
	.global gData_020064a0
gData_020064a0:
	.space 0x00000004
	.global gData_020064a4
gData_020064a4:
	.space 0x00000004
	.global gData_020064a8
gData_020064a8:
	.space 0x00000004
	.global gData_020064ac
gData_020064ac:
	.space 0x00000004
	.global gData_020064b0
gData_020064b0:
	.space 0x00000004
	.global gData_020064b4
gData_020064b4:
	.space 0x00000004
	.global gData_020064b8
gData_020064b8:
	.space 0x00000004
	.global gData_020064bc
gData_020064bc:
	.space 0x00000004
	.global gData_020064c0
gData_020064c0:
	.space 0x00000004
	.global gData_020064c4
gData_020064c4:
	.space 0x0000000c
	.global gData_020064d0
gData_020064d0:
	.space 0x00000030
	.global gData_02006500
gData_02006500:
	.space 0x00000004
	.global gData_02006504
gData_02006504:
	.space 0x00000004
	.global gData_02006508
gData_02006508:
