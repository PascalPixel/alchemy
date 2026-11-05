
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
	.section .text.x0200aa20,"ax",%progbits
	.global Func_02002a20
	.thumb_func
Func_02002a20:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r0, .L_0200ab8c
	sub sp, #24
	str r0, [sp, #8]
	str r0, [sp, #12]
	ldr r3, .L_0200ab90
	ldr r2, .L_0200ab94
	movs r1, #0
	ldrsh r3, [r3, r1]
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	lsrs r3, r3, #5
	mov r10, r3
.L_0200aa48:
	ldr r1, .L_0200ab98
	movs r2, #0
	ldrsh r5, [r1, r2]
	ldrh r3, [r1]
	cmp r5, #0
	bne .L_0200ab28
	ldr r4, .L_0200ab9c
	movs r2, #128
	ldr r0, [r4]
	lsls r2, r2, #6
	ldrh r3, [r0]
	adds r0, #2
	lsls r3, r3, #16
	asrs r3, r3, #16
	str r0, [r4]
	cmp r3, r2
	beq .L_0200aaee
	cmp r3, r2
	bgt .L_0200aa80
	movs r7, #1
	negs r7, r7
	cmp r3, r7
	beq .L_0200ab16
	movs r1, #128
	lsls r1, r1, #5
	cmp r3, r1
	beq .L_0200aad4
	b .L_0200aa48
.L_0200aa80:
	movs r2, #128
	lsls r2, r2, #7
	cmp r3, r2
	beq .L_0200aaa2
	cmp r3, r2
	bgt .L_0200aa96
	movs r2, #192
	lsls r2, r2, #6
	cmp r3, r2
	beq .L_0200aaba
	b .L_0200aa48
.L_0200aa96:
	movs r5, #254
	lsls r5, r5, #7
	adds r5, #255
	cmp r3, r5
	beq .L_0200ab0c
	b .L_0200aa48
.L_0200aaa2:
	movs r7, #0
	ldrsh r3, [r0, r7]
	ldr r2, .L_0200aba0
	lsls r3, r3, #8
	str r3, [r2]
	adds r3, r0, #2
	ldrh r2, [r3]
	adds r3, #2
	ldr r1, .L_0200aba4
	str r3, [r4]
	ldr r3, .L_0200aba8
	b .L_0200ab06
.L_0200aaba:
	ldr r2, .L_0200aba4
	ldr r1, .L_0200abac
	ldrh r3, [r2]
	strh r3, [r1]
	ldr r1, .L_0200aba8
	ldrh r3, [r0]
	strh r3, [r2]
	adds r3, r0, #2
	ldrh r2, [r3]
	adds r3, #2
	str r3, [r4]
	ldr r3, .L_0200abb0
	b .L_0200ab06
.L_0200aad4:
	ldr r2, .L_0200abb4
	ldr r1, .L_0200abb8
	ldrh r3, [r2]
	strh r3, [r1]
	ldr r1, .L_0200abbc
	ldrh r3, [r0]
	strh r3, [r2]
	adds r3, r0, #2
	ldrh r2, [r3]
	adds r3, #2
	str r3, [r4]
	ldr r3, .L_0200abc0
	b .L_0200ab06
.L_0200aaee:
	ldr r2, .L_0200abc4
	ldr r1, .L_0200abc8
	ldrh r3, [r2]
	strh r3, [r1]
	ldr r1, .L_0200abcc
	ldrh r3, [r0]
	strh r3, [r2]
	adds r3, r0, #2
	ldrh r2, [r3]
	adds r3, #2
	str r3, [r4]
	ldr r3, .L_0200abd0
.L_0200ab06:
	strh r2, [r1]
	strh r5, [r3]
	b .L_0200aa48
.L_0200ab0c:
	ldrh r3, [r0]
	strh r3, [r1]
	adds r3, r0, #2
	str r3, [r4]
	b .L_0200aa48
.L_0200ab16:
	ldr r0, .L_0200abd4
	bl Task_RemoveFar
	ldr r3, .L_0200ab90
	movs r1, #0
	ldrsh r0, [r3, r1]
	bl Resource_ResetEntry
	b .L_0200aef8
.L_0200ab28:
	subs r3, #1
	ldr r2, .L_0200abbc
	strh r3, [r1]
	mov r8, r2
	movs r3, #0
	ldrsh r7, [r2, r3]
	ldr r2, .L_0200abb4
	cmp r7, #0
	bne .L_0200ab42
	movs r5, #0
	ldrsh r4, [r2, r5]
	str r4, [sp, #0]
	b .L_0200ab72
.L_0200ab42:
	ldr r3, .L_0200abb8
	movs r0, #0
	ldrsh r6, [r3, r0]
	movs r1, #0
	ldrsh r3, [r2, r1]
	ldr r2, .L_0200abc0
	subs r3, r3, r6
	ldrh r5, [r2]
	adds r1, r7, #0
	adds r5, #1
	strh r5, [r2]
	lsls r5, r5, #16
	asrs r5, r5, #16
	adds r0, r5, #0
	muls r0, r3
	bl __divsi3
	adds r6, r6, r0
	str r6, [sp, #0]
	cmp r5, r7
	blt .L_0200ab72
	ldr r3, .L_0200ab88
	mov r2, r8
	strh r3, [r2]
.L_0200ab72:
	ldr r3, .L_0200abcc
	ldr r2, .L_0200abc4
	movs r4, #0
	ldrsh r7, [r3, r4]
	mov r8, r3
	cmp r7, #0
	bne .L_0200abd8
	movs r7, #0
	ldrsh r5, [r2, r7]
	str r5, [sp, #4]
	b .L_0200ac08
.L_0200ab88:
	.4byte 0x00000000
.L_0200ab8c:
	.4byte gData_020064d0
.L_0200ab90:
	.4byte gData_02005158
.L_0200ab94:
	.4byte gResourceTableEntries
.L_0200ab98:
	.4byte gData_020064bc
.L_0200ab9c:
	.4byte gData_020064c0
.L_0200aba0:
	.4byte gData_020064a0
.L_0200aba4:
	.4byte gData_02006504
.L_0200aba8:
	.4byte gData_0200649c
.L_0200abac:
	.4byte gData_02006500
.L_0200abb0:
	.4byte gData_020064a8
.L_0200abb4:
	.4byte gData_020064a4
.L_0200abb8:
	.4byte gData_02006498
.L_0200abbc:
	.4byte gData_02006490
.L_0200abc0:
	.4byte gData_02006508
.L_0200abc4:
	.4byte gData_020064b4
.L_0200abc8:
	.4byte gData_020064b8
.L_0200abcc:
	.4byte gData_020064c4
.L_0200abd0:
	.4byte gData_020064ac
.L_0200abd4:
	.4byte Func_02002a20
.L_0200abd8:
	ldr r3, .L_0200ac2c
	movs r0, #0
	ldrsh r6, [r3, r0]
	movs r1, #0
	ldrsh r3, [r2, r1]
	ldr r2, .L_0200ac30
	subs r3, r3, r6
	ldrh r5, [r2]
	adds r1, r7, #0
	adds r5, #1
	strh r5, [r2]
	lsls r5, r5, #16
	asrs r5, r5, #16
	adds r0, r5, #0
	muls r0, r3
	bl __divsi3
	adds r6, r6, r0
	str r6, [sp, #4]
	cmp r5, r7
	blt .L_0200ac08
	ldr r3, .L_0200ac28
	mov r2, r8
	strh r3, [r2]
.L_0200ac08:
	ldr r3, .L_0200ac34
	ldr r0, [sp, #0]
	movs r4, #0
	ldrsh r7, [r3, r4]
	add r5, sp, #16
	lsls r0, r0, #16
	mov r11, r3
	ldr r2, .L_0200ac38
	mov r8, r5
	mov r9, r0
	cmp r7, #0
	bne .L_0200ac3c
	movs r1, #0
	ldrsh r6, [r2, r1]
	b .L_0200ac6a
	.2byte 0x0000
.L_0200ac28:
	.4byte 0x00000000
.L_0200ac2c:
	.4byte gData_020064b8
.L_0200ac30:
	.4byte gData_020064ac
.L_0200ac34:
	.4byte gData_0200649c
.L_0200ac38:
	.4byte gData_02006504
.L_0200ac3c:
	ldr r3, .L_0200aca4
	adds r1, r7, #0
	movs r4, #0
	ldrsh r6, [r3, r4]
	movs r5, #0
	ldrsh r3, [r2, r5]
	ldr r2, .L_0200aca8
	subs r3, r3, r6
	ldrh r5, [r2]
	adds r5, #1
	strh r5, [r2]
	lsls r5, r5, #16
	asrs r5, r5, #16
	adds r0, r5, #0
	muls r0, r3
	bl __divsi3
	adds r6, r6, r0
	cmp r5, r7
	blt .L_0200ac6a
	ldr r3, .L_0200aca0
	mov r7, r11
	strh r3, [r7]
.L_0200ac6a:
	mov r0, r8
	ldr r3, [r0, #4]
	ldr r2, .L_0200acac
	ands r3, r2
	str r3, [r0, #4]
	mov r3, r9
	lsrs r1, r3, #16
	ldr r3, [sp, #16]
	ands r3, r2
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	orrs r3, r1
	ands r3, r2
	lsls r1, r1, #16
	orrs r3, r1
	str r3, [sp, #16]
	bl AffineMatrix_BuildForEffect
	ldr r2, .L_0200acb0
	lsls r0, r0, #16
	ldr r3, [r2]
	asrs r0, r0, #16
	adds r3, r3, r6
	mov r8, r0
	str r3, [r2]
	b .L_0200acb4
.L_0200aca0:
	.4byte 0x00000000
.L_0200aca4:
	.4byte gData_02006500
.L_0200aca8:
	.4byte gData_020064a8
.L_0200acac:
	.4byte 0xffff0000
.L_0200acb0:
	.4byte gData_020064a0
.L_0200acb4:
	cmp r3, #0
	bge .L_0200acba
	adds r3, #255
.L_0200acba:
	asrs r6, r3, #8
	ldr r3, .L_0200af08
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #2
	bne .L_0200acc8
	b .L_0200ae34
.L_0200acc8:
	cmp r3, #2
	bgt .L_0200acd2
	cmp r3, #1
	beq .L_0200acdc
	b .L_0200ae8e
.L_0200acd2:
	cmp r3, #3
	beq .L_0200ad5a
	cmp r3, #4
	beq .L_0200adda
	b .L_0200ae8e
.L_0200acdc:
	movs r5, #0
.L_0200acde:
	ldr r7, [sp, #0]
	lsls r3, r5, #5
	subs r3, #48
	muls r3, r7
	movs r1, #56
	ldr r0, .L_0200af0c
	cmp r3, #0
	bge .L_0200acf0
	adds r3, #255
.L_0200acf0:
	asrs r3, r3, #8
	adds r3, r6, r3
	movs r4, #48
	adds r2, r3, #0
	adds r4, #255
	adds r3, #152
	adds r2, #88
	cmp r3, r4
	bhi .L_0200ad4e
	movs r7, #0
	mov r12, r7
	ldr r7, [sp, #12]
	mov r4, r12
	stmia r7!, {r4}
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r2, r3
	adds r3, r7, #0
	str r3, [sp, #12]
	lsls r3, r2, #16
	orrs r3, r1
	orrs r3, r0
	mov r0, r8
	lsls r2, r0, #25
	orrs r3, r2
	movs r2, #224
	lsls r2, r2, #3
	orrs r3, r2
	adds r2, r7, #0
	stmia r2!, {r3}
	movs r3, #244
	lsls r3, r3, #8
	mov r4, r10
	adds r1, r2, #0
	orrs r3, r4
	str r1, [sp, #12]
	stmia r2!, {r3}
	ldr r0, [sp, #8]
	adds r7, r2, #0
	adds r1, r0, #0
	adds r1, #12
	str r1, [sp, #8]
	movs r1, #236
	str r7, [sp, #12]
	bl Func_020047cc
.L_0200ad4e:
	movs r2, #8
	adds r5, #1
	add r10, r2
	cmp r5, #3
	ble .L_0200acde
	b .L_0200ae8e
.L_0200ad5a:
	movs r5, #0
.L_0200ad5c:
	ldr r4, [sp, #0]
	lsls r3, r5, #5
	subs r3, #16
	muls r3, r4
	movs r1, #48
	ldr r0, .L_0200af0c
	cmp r3, #0
	bge .L_0200ad6e
	adds r3, #255
.L_0200ad6e:
	asrs r3, r3, #8
	adds r3, r6, r3
	movs r7, #48
	adds r2, r3, #0
	adds r7, #255
	adds r3, #152
	adds r2, #88
	cmp r3, r7
	bhi .L_0200adce
	movs r3, #128
	ldr r7, [sp, #12]
	lsls r3, r3, #1
	adds r3, #255
	ands r2, r3
	movs r3, #0
	stmia r7!, {r3}
	lsls r3, r2, #16
	orrs r3, r1
	orrs r3, r0
	mov r0, r8
	lsls r2, r0, #25
	orrs r3, r2
	movs r2, #224
	lsls r2, r2, #3
	adds r4, r7, #0
	orrs r3, r2
	adds r2, r7, #0
	str r4, [sp, #12]
	stmia r2!, {r3}
	ldr r3, .L_0200af10
	adds r1, r2, #0
	movs r4, #0
	ldrsh r3, [r3, r4]
	movs r2, #244
	add r3, r10
	lsls r2, r2, #8
	adds r0, r1, #0
	orrs r3, r2
	stmia r0!, {r3}
	adds r7, r0, #0
	ldr r0, [sp, #8]
	str r7, [sp, #12]
	adds r1, r0, #0
	adds r1, #12
	str r1, [sp, #8]
	movs r1, #236
	bl Func_020047cc
.L_0200adce:
	movs r2, #8
	adds r5, #1
	add r10, r2
	cmp r5, #1
	ble .L_0200ad5c
	b .L_0200ae8e
.L_0200adda:
	.ifdef TLA_EDITION_FR
	movs r5, #0
.L_shaman_french_graphic_loop:
	ldr r4, [sp, #0]
	lsls r3, r5, #5
	subs r3, #16
	muls r3, r4
	movs r1, #64
	ldr r0, .L_0200af0c
	cmp r3, #0
	bge .L_shaman_french_graphic_position
	adds r3, #255
.L_shaman_french_graphic_position:
	asrs r3, r3, #8
	adds r3, r6, r3
	movs r7, #48
	adds r2, r3, #0
	adds r7, #255
	adds r3, #152
	adds r2, #88
	cmp r3, r7
	bhi .L_shaman_french_graphic_next
	movs r3, #128
	ldr r7, [sp, #12]
	lsls r3, r3, #1
	adds r3, #255
	ands r2, r3
	movs r3, #0
	stmia r7!, {r3}
	lsls r3, r2, #16
	orrs r3, r1
	orrs r3, r0
	mov r0, r8
	lsls r2, r0, #25
	orrs r3, r2
	movs r2, #224
	lsls r2, r2, #3
	adds r4, r7, #0
	orrs r3, r2
	adds r2, r7, #0
	str r4, [sp, #12]
	stmia r2!, {r3}
	ldr r3, .L_0200af10
	adds r1, r2, #0
	movs r4, #0
	ldrsh r3, [r3, r4]
	movs r2, #244
	add r3, r10
	lsls r2, r2, #8
	adds r0, r1, #0
	orrs r3, r2
	stmia r0!, {r3}
	adds r7, r0, #0
	ldr r0, [sp, #8]
	str r7, [sp, #12]
	adds r1, r0, #0
	adds r1, #12
	str r1, [sp, #8]
	movs r1, #236
	bl Func_020047cc
.L_shaman_french_graphic_next:
	movs r2, #8
	adds r5, #1
	add r10, r2
	cmp r5, #1
	ble .L_shaman_french_graphic_loop
	b .L_0200ae8e
	.else
	adds r3, r6, #0
	movs r4, #152
	adds r2, r6, #0
	adds r3, #120
	lsls r4, r4, #1
	movs r1, #48
	ldr r0, .L_0200af14
	adds r2, #56
	cmp r3, r4
	bcs .L_0200ae8e
	ldr r5, [sp, #8]
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	adds r4, r5, #0
	ands r2, r3
	movs r3, #0
	stmia r4!, {r3}
	lsls r3, r2, #16
	orrs r3, r1
	mov r5, r8
	orrs r3, r0
	lsls r2, r5, #25
	orrs r3, r2
	movs r2, #224
	lsls r2, r2, #3
	adds r7, r4, #0
	orrs r3, r2
	str r7, [sp, #12]
	stmia r4!, {r3}
	ldr r3, .L_0200af10
	adds r7, r4, #0
	str r7, [sp, #12]
	movs r2, #244
	movs r0, #0
	ldrsh r3, [r3, r0]
	lsls r2, r2, #8
	add r3, r10
	orrs r3, r2
	str r3, [r4]
	ldr r0, [sp, #8]
	movs r1, #236
	bl Func_020047cc
	b .L_0200ae8e
	.endif
.L_0200ae34:
	adds r3, r6, #0
	movs r4, #152
	movs r0, #128
	adds r2, r6, #0
	adds r3, #152
	lsls r4, r4, #1
	movs r1, #48
	lsls r0, r0, #24
	adds r2, #88
	cmp r3, r4
	bcs .L_0200ae8e
	ldr r5, [sp, #8]
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	adds r4, r5, #0
	ands r2, r3
	movs r3, #0
	stmia r4!, {r3}
	lsls r3, r2, #16
	orrs r3, r1
	mov r5, r8
	orrs r3, r0
	lsls r2, r5, #25
	orrs r3, r2
	movs r2, #224
	lsls r2, r2, #3
	adds r7, r4, #0
	orrs r3, r2
	str r7, [sp, #12]
	stmia r4!, {r3}
	ldr r3, .L_0200af10
	adds r7, r4, #0
	str r7, [sp, #12]
	movs r2, #244
	movs r0, #0
	ldrsh r3, [r3, r0]
	lsls r2, r2, #8
	add r3, r10
	orrs r3, r2
	str r3, [r4]
	ldr r0, [sp, #8]
	movs r1, #236
	bl Func_020047cc
.L_0200ae8e:
	ldr r0, .L_0200af18
	ldr r1, .L_0200af1c
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_0200aec0
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r0
	strh r2, [r0]
	movs r2, #252
	adds r3, #4
	lsls r2, r2, #6
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_0200aec0:
	strh r4, [r1]
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r3, [r0]
	cmp r3, #31
	bgt .L_0200aef6
	lsls r2, r3, #1
	adds r2, r2, r3
	adds r3, #1
	strh r3, [r0]
	ldr r5, [sp, #4]
	movs r3, #16
	lsls r2, r2, #2
	subs r3, r3, r5
	adds r2, r2, r0
	lsls r3, r3, #8
	adds r2, #4
	orrs r3, r5
	stmia r2!, {r3}
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	stmia r2!, {r3}
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r2]
.L_0200aef6:
	strh r4, [r1]
.L_0200aef8:
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.ifndef TLA_EDITION_FR
	.2byte 0x0000
	.endif
.L_0200af08:
	.4byte gData_020064b0
.L_0200af0c:
	.4byte 0x80004000
.L_0200af10:
	.4byte gData_02006494
	.ifndef TLA_EDITION_FR
.L_0200af14:
	.4byte 0xc0004000
	.endif
.L_0200af18:
	.4byte gIoWriteQueue
.L_0200af1c:
	.4byte 0x04000208
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
