.syntax unified
	.thumb
	.global BattleFormation_SelectRandomAvailableMember
	.thumb_func
BattleFormation_SelectRandomAvailableMember:
	push {r5, r6, lr}
	movs r3, #165
	lsls r3, r3, #2
	sub sp, #20
	movs r5, #0
	cmp r0, r3
	bcc .L_08127c90
	movs r0, #1
.L_08127c90:
	lsls r3, r0, #1
	ldr r2, .L_08127cd0
	adds r3, r3, r0
	lsls r3, r3, #3
	adds r0, r3, r2
	mov r6, sp
	adds r2, r0, #0
	adds r2, #15
	adds r4, r6, #0
	movs r1, #4
.L_08127ca4:
	ldrb r3, [r2]
	adds r2, #1
	cmp r3, #0
	beq .L_08127cb4
	ldrh r3, [r0]
	adds r5, #1
	adds r3, #8
	stmia r4!, {r3}
.L_08127cb4:
	subs r1, #1
	adds r0, #2
	cmp r1, #0
	bge .L_08127ca4
	bl Random16
	adds r3, r5, #0
	muls r3, r0
	lsrs r3, r3, #16
	lsls r3, r3, #2
	ldr r0, [r6, r3]
	add sp, #20
	pop {r5, r6, pc}
	.2byte 0x0000
.L_08127cd0:
	.4byte Data_0812ce7c
