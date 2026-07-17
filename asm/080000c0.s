@ ROM-start dispatch-stub table: stable thumb entry points into
@ ROM routines for code loaded elsewhere. Regenerate with
@ python3 tools/export_dispatch_table.py baserom.gba
	.thumb

	.macro dispatch target
	ldr r4, 1f
	bx r4
1:	.4byte \target
	.endm

	dispatch Func_080030f8 + 1
	dispatch Func_080040e8 + 1
	dispatch Func_080041d8 + 1
	dispatch Func_08004278 + 1
	dispatch Func_080042c8 + 1
	dispatch Func_0800439c + 1
	dispatch Func_08004420 + 1
	dispatch Func_08004458 + 1
	dispatch Func_080044d0 + 1
	dispatch Func_080045a4 + 1
	dispatch Func_080045d4 + 1
	dispatch Func_08002322 + 1
	dispatch Func_0800231c + 1
	dispatch Func_0800447c + 1
	dispatch Func_0800307c + 1
	dispatch Func_08004858 + 1
	dispatch Func_080048b0 + 1
	dispatch Func_080048f4 + 1
	dispatch Func_08002dd8 + 1
	dispatch Func_0800488c + 1
	dispatch Func_080048a0 + 1
	dispatch Func_08004938 + 1
	dispatch Func_08004970 + 1
	dispatch Func_08002df0 + 1
	dispatch Func_080046c4 + 1
	dispatch Func_08004718 + 1
	dispatch Func_0800473c + 1
	dispatch Func_08004698 + 1
	dispatch Func_080053e8 + 1
	dispatch Func_08005340 + 1
	dispatch Func_0800403c + 1
	dispatch Func_08003f3c + 1
	dispatch Func_08003f78 + 1
	dispatch Func_08003fa4 + 1
	dispatch Func_08004080 + 1
	dispatch Func_08004760 + 1
	dispatch Func_08003d28 + 1
	dispatch Func_08003dec + 1
	dispatch Func_0800479c + 1
	dispatch Func_08004838 + 1
	dispatch Func_080049ac + 1
	dispatch Func_08004a94 + 1
	dispatch Func_080049e8 + 1
	dispatch Func_08004a5c + 1
	dispatch Func_08004ab0 + 1
	dispatch Func_08004bd4 + 1
	dispatch Func_08004c1c + 1
	dispatch Func_08004c6c + 1
	dispatch Func_08004cb4 + 1
	dispatch Func_08004cf0 + 1
	dispatch Func_08004d2c + 1
	dispatch Func_08004e54 + 1
	dispatch Func_080051d8 + 1
	dispatch Func_08005268 + 1
	dispatch Func_08005208 + 1
	dispatch Func_08005258 + 1
	dispatch Func_08002f10 + 1
	dispatch Func_08002f0c + 1
	dispatch Func_08002f40 + 1
	dispatch Func_08002f3c + 1
	dispatch Func_08002fb0 + 1
	dispatch Func_08003b70 + 1
	dispatch Func_08003bb4 + 1
	dispatch Func_08003bf8 + 1
	dispatch Func_08003c3c + 1
	dispatch Func_08003c80 + 1
	dispatch Func_08003ce0 + 1
	dispatch Func_08002ef8 + 1
	dispatch Func_08002efc + 1
	dispatch Func_08002f00 + 1
	dispatch Func_08002f04 + 1
	dispatch Func_08002f08 + 1
	dispatch Func_08005d10 + 1
	dispatch Func_08005ee0 + 1
	dispatch Func_0800383c + 1
	dispatch Func_0800387c + 1
	dispatch Func_080038bc + 1
	dispatch Func_080038fc + 1
	dispatch Func_0800393c + 1
	dispatch Func_0800397c + 1
	dispatch Func_080039bc + 1
	dispatch Func_080039fc + 1
	dispatch Func_08003a3c + 1
	dispatch Func_0800352c + 1
	dispatch Func_080051e8 + 1
	dispatch Func_08006384 + 1
	dispatch Func_08006358 + 1
	dispatch Func_080037d4 + 1
	dispatch Func_080063bc + 1
	dispatch Func_08006408 + 1
	dispatch Func_08006458 + 1
	dispatch Func_08006488 + 1
	dispatch Func_080064b8 + 1
	dispatch Func_080064f4 + 1
	dispatch Func_0800655c + 1
	dispatch Func_08004358 + 1
