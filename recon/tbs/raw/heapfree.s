@ Not yet C: the German/Spanish cartridge-interrupt routine has no C2 proof.
@ Runtime_ReleaseHeapBlock and MemFree are in games/tbs/src/system/heapfree.s.
.syntax unified
	.ifdef TBS_EDITION_DE
	.set GAMEPAK_STOP, 1
	.endif
	.ifdef TBS_EDITION_ES
	.set GAMEPAK_STOP, 1
	.endif
	.ifdef GAMEPAK_STOP
@ カートリッジ割り込みなら音を止めて止まる
	.section .text.gamepak, "ax", %progbits
	.balign 4
	.arm
	.global Runtime_StopOnGamePakIrq
Runtime_StopOnGamePakIrq:
	mov	ip, #0x04000000
	add	r3, ip, #0x200
	ldr	r2, [r3]
	and	r1, r2, r2, lsr #16
	mov	r2, #0
	ands	r0, r1, #0x2000
	strbne	r0, [r3, #-0x17c]
.L_08002e1c:
	bne	.L_08002e1c
	.endif
