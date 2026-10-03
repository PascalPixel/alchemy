@ Uncredited disassembly re-derived from the owned TBS English ROM, 200 bytes.
@ Ordinary C is kept in ../en/main/080c1a34.c; its frame is unresolved.
	.syntax unified
	.thumb
	.text
	.balign 4
	.global Party_ComputeEligibleMemberAverage
	.type Party_ComputeEligibleMemberAverage, %function
	.thumb_func
Party_ComputeEligibleMemberAverage:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .Lformation
	lsls r0, r0, #4
	adds r0, r0, r3
	ldrb r3, [r0, #6]
	movs r2, #0
	sub sp, #28
	mov r9, r2
	mov r11, r2
	mov r10, r0
	movs r7, #0
	cmp r3, #0
	bne .Lminimum_done
	mov r2, r10
	adds r2, #6
.Lminimum_next:
	adds r7, #1
	cmp r7, #4
	bhi .Lminimum_done
	adds r2, #1
	ldrb r3, [r2]
	cmp r3, #0
	beq .Lminimum_next
.Lminimum_done:
	cmp r7, #5
	bne .Lmembers_begin
	movs r0, #1
	negs r0, r0
	b .Lreturn
.Lmembers_begin:
	movs r3, #11
	add r3, r10
	movs r7, #0
	mov r8, r3
.Lmember:
	mov r2, r8
	ldrb r3, [r2]
	movs r2, #1
	add r8, r2
	cmp r3, #0
	beq .Lmember_next
	mov r3, r10
	adds r3, #1
	ldrb r6, [r3, r7]
	adds r0, r6, #0
	adds r0, #8
	bl Owner_GetRecordFar
	adds r5, r0, #0
	cmp r5, #0
	beq .Lmember_next
	ldrb r3, [r5, #15]
	cmp r3, #3
	bls .Leligible
	movs r0, #186
	lsls r0, r0, #1
	bl GameFlag_IsSet
	cmp r0, #0
	bne .Leligible
	movs r3, #193
	lsls r3, r3, #3
	adds r0, r6, r3
	bl GameFlag_IsSet
	cmp r0, #0
	beq .Lineligible
.Leligible:
	ldrb r3, [r5, #15]
	movs r2, #1
	add r11, r3
	add r9, r2
.Lmember_next:
	adds r7, #1
	cmp r7, #4
	bls .Lmember
	mov r3, r9
	cmp r3, #0
	bne .Laverage
	movs r0, #3
	negs r0, r0
	b .Lreturn
.Lineligible:
	movs r0, #2
	negs r0, r0
	b .Lreturn
.Laverage:
	mov r0, r11
	mov r1, r9
	bl __divsi3
.Lreturn:
	add sp, #28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.balign 4
.Lformation:
	.4byte BattleFormation_Records
	.size Party_ComputeEligibleMemberAverage, .-Party_ComputeEligibleMemberAverage
