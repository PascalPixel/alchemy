@ Halt loop of TLA's resident runtime: the ARM svc 0x30000 (BIOS Stop)
@ followed by a branch back to it. Start-up passes its run address
@ 0x030007ac with interrupt index 13, the Game Pak interrupt, to the
@ interrupt set-up routine at 0x08013438 (ldr r2, [pc, #72]; movs r0, #13
@ at 0x08013216). Neither approved compiler emits svc. TBS's German,
@ Spanish, French and Italian editions carry the same 8 bytes inside the ROM
@ interrupt table that start-up copies into the runtime's handler words
@ (TBS-de 0x08007378, copied to 0x03000108) and install that copy as their
@ Game Pak handler the same way (TBS-de 0x08002e86); the English and
@ Japanese TBS editions do not have them.
@ credit: library — TBS/TLA common runtime halt loop
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x08000d64
	.set AlchemyRunAddress, 0x030007ac
.L_030007ac:
	svc	0x00030000
	b	.L_030007ac
