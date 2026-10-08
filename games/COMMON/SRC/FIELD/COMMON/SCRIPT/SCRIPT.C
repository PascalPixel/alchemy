//--------------------------------------------------------------------------
// SCRIPT.C
// オブジェクトスクリプト：位置の移動と byte_54 の命令
//--------------------------------------------------------------------------

#include "SCRIPT.H"

//--------------------------------------------------------------------------
// スクリプトの読む位置を移し、1 を返して次の命令を続けて実行する
//--------------------------------------------------------------------------
#if GAME_TLA
s32 Script_JumpToNextOperandPointer(struct ScriptInterpreter *sc)
#else
s32 Script_AdvancePastCursor(struct ScriptInterpreter *sc)
#endif
{
	s32 zero;

#if GAME_TLA
	// 命令の次の語が指すスクリプトへ飛ぶ
	sc->script = (const s32 *)sc->script[sc->cursor + 1];
#else
	// 命令の次の語を新しい先頭にする
	sc->script = sc->script + sc->cursor + 1;
#endif
	zero = 0;
	sc->cursor = zero;
	return 1;
}

//--------------------------------------------------------------------------
// byte_54 を 0 にして、次の命令へ
//--------------------------------------------------------------------------
s32 Script_ClearByte54(struct ScriptInterpreter *sc)
{
	sc->byte_54 = 0;
	sc->cursor++;
	return 1;
}

//--------------------------------------------------------------------------
// byte_54 を 1 にして、次の命令へ
//--------------------------------------------------------------------------
s32 Script_SetByte54(struct ScriptInterpreter *sc)
{
	sc->byte_54 = 1;
	sc->cursor++;
	return 1;
}
