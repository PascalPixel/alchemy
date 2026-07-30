# 第6回 100ファイル humanization — 2026-07-28

コメントだけの変更を禁止し、型・ABI・演算・制御の実質的な整理だけを
採用した。

## 結果

- 100候補を確認。
- 77候補を実質的な整理として採用。
- 23候補を不一致、未定義動作、低価値変更として復元。
- 隣接する12本の型・ABIも整理。
- 最終差分はC 89本、ヘッダ6本、本書1本。
- claimed C: 1,345本、失敗0、90,664 bytes。
- source-only: 8,388,608 bytes、未所有0。
- full ROM: 完全一致、fallback 0。
- 全self-test成功。

object command、script/object複合work、menu descriptor、owner state、
audio player/trackの共有型を統合した。ポインタを`s32`へ落とす経路、
strict alias、符号付きshift/overflow、相互に異なる構造体tagの
cross-TU宣言を修正した。

## 復元・保留

- `src/0800dcdc.c`
- `src/080167ac.c`
- `src/0801ce48.c`
- `src/0801ce6c.c`
- `src/0801ce90.c`
- `src/0801cee0.c`
- `src/0801e3c8.c`
- `src/0801edcc.c`
- `src/0801f730.c`
- `src/08079c8c.c`
- `src/08079d1c.c`
- `src/08096c80.c`
- `src/08096d2c.c`
- `src/08096d84.c`
- `src/08098294.c`
- `src/080a33d4.c`
- `src/080a3cf8.c`
- `src/080a413c.c`
- `src/080b6b40.c`
- `src/080b9d34.c`
- `src/080ba27c.c`
- `src/080fb768.c`
- `src/080fb77c.c`

## 次の100候補

### C1 — low-level runtime/state layouts

`0800fe9c`, `0800bf34`, `0800a97c`, `0800f9cc`, `080046c4`, `080071a8`,
`08004620`, `08004420`, `0800bfa4`, `0800403c`

### C2 — field/UI state

`0801a910`, `0801eea0`, `0801b148`, `0801f5f0`, `0801f9b4`, `08017c1c`,
`0801b1ec`, `0801fa3c`, `08017464`, `0801b398`

### C3 — engine control/object traversal

`0802706c`, `08025180`, `080228bc`, `08028ef0`, `080208e4`, `08028df4`,
`08021a18`, `08028e54`, `080291e4`, `08029504`

### C4 — script/battle object state

`0808bc44`, `0808c44c`, `0808d828`, `0808d7d8`, `0808c4c0`, `0808eee4`,
`0808c3a4`, `0808f304`, `0808d458`, `0808adf0`

### C5 — motion/effect runtime

`08093570`, `08090378`, `080947e4`, `08093710`, `08090584`, `08092878`,
`08099d18`, `08096ab0`, `08099128`, `0809088c`

### C6 — menu/state raw-layout cohort

`080a99b0`, `080a8088`, `080aca04`, `080aaf58`, `080a8034`, `080aa460`,
`080ae714`, `080a68a8`, `080ae88c`, `080a9bd8`

### C7 — menu/state structural cohort

`080a9cf8`, `080a9b94`, `080a8b8c`, `080a5578`, `080a6874`, `080ae958`,
`080ab21c`, `080a6a00`, `080a9a5c`, `080a45cc`

### C8 — battle/runtime object cohort

`080b17e4`, `080bdfec`, `080bd808`, `080b0958`, `080bd7dc`, `080be02c`,
`080b2ffc`, `080bd3c8`, `080b9a70`, `080b04dc`

### C9 — late runtime/data-control cohort

`080c9138`, `080c1084`, `080c23e8`, `080cdd14`, `080c1f50`, `080c2368`,
`080c0eec`, `080cd4b4`, `080c0098`, `080c1438`

### C10 — system/table/global-layout cohort

`080c0e70`, `080cef64`, `080d6578`, `080dfddc`, `080da24c`, `080e155c`,
`080f7e34`, `080f037c`, `080f3858`, `080b6eb4`
