> **C/H hard blocker:** Never use `asm(...)`, `__asm(...)`, `__asm_(...)`, `__asm__(...)`, fixed-register bindings, or empty assembly barriers. Byte equality never overrides this rule.

# 第3回 100ファイル humanization — 2026-07-28

100候補を20ファイルずつ5レーンで処理した。各候補を単独検証し、
不一致は追跡せず保留した。結合後に別担当で意味・ABI・未定義動作を
再確認した。

## 結果

- 100候補を確認。
- 88候補をバイト一致のまま整理。
- 11候補を保留し、`HEAD`の内容へ復元。
- `0801c0c8`は既に日本語ソース規約に合っていたため変更なし。
- 隣接するdispatch wrapper 5本もABIを統一。
- 最終差分はC 93本、ヘッダ3本、本書1本。
- claimed C: 1,345本、失敗0、90,664 bytes。
- source-only: 8,388,608 bytes、未所有0。
- full ROM: 完全一致、fallback 0。
- 全self-test成功。

確認時にDMAレジスタと非同期完了フラグへ`volatile`を追加し、
隠しレジスタ引数、音楽トラック型、dispatch objectの大きさ、
符号付き演算の未定義動作を修正した。

## 保留

- `src/08006a00.c`
- `src/08006ba8.c`
- `src/08006c24.c`
- `src/0800c388.c`
- `src/080118a8.c`
- `src/0801999c.c`
- `src/0801c2e4.c`
- `src/0801c3e8.c`
- `src/0808b05c.c`
- `src/0808b074.c`
- `src/080fb690.c`

## 次の100候補

### K1 — transfer/task continuation

`08003adc`, `08003b70`, `08003bb4`, `08003bf8`, `08003c3c`, `08003c80`,
`08003dec`, `08003f04`, `08003f78`, `08004080`

### K2 — event dispatch/load records

`0800b798`, `0800b868`, `0800b93c`, `0800b9a4`, `0800b9f4`, `0800bacc`,
`0800baf8`, `0800be20`, `0800be70`, `0800befc`

### K3 — script command continuation

`0800d820`, `0800d850`, `0800d880`, `0800d8c4`, `0800d8f4`, `0800d900`,
`0800d924`, `0800d98c`, `0800d9f0`, `0800da18`

### K4 — script operand continuation

`0800e7d4`, `0800e810`, `0800e850`, `0800e890`, `0800e8d0`, `0800e8fc`,
`0800e928`, `0800e964`, `0800e9a0`, `0800e9dc`

### K5 — map/runtime control

`08015e8c`, `08015ec0`, `08015ef4`, `08016230`, `080163ec`, `08016478`,
`08016498`, `080164ac`, `0801656c`, `08016584`

### K6 — battle command/effect sequence

`0801e74c`, `0801e858`, `0801e8b0`, `0801e940`, `0801e9a0`, `0801e9d4`,
`0801ea08`, `0801eadc`, `0801eb64`, `0801eb90`

### K7 — object animation and placement

`08092980`, `080929d8`, `08092b54`, `08092b94`, `08093040`, `08093054`,
`080933d4`, `080933f8`, `08093530`, `08093554`

### K8 — battle scene/effect object control

`080958e4`, `08095bac`, `08095bd8`, `08095f9c`, `080967e4`, `08096af0`,
`08096b28`, `08096b88`, `08096bec`, `08096c48`

### K9 — menu/action state machine

`080a1050`, `080a1070`, `080a10d0`, `080a1114`, `080a14f0`, `080a153c`,
`080a172c`, `080a1778`, `080a17c4`, `080a1814`

### K10 — character/stat progression

`080bf208`, `080bf250`, `080bf2b4`, `080bf318`, `080bf37c`, `080bf3bc`,
`080bf400`, `080bf440`, `080bf484`, `080bf4c4`
