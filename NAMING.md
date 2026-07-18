# 命名規約 / Naming

This repository deliberately gives reconstructed source the voice of a 2001
Japanese console-game codebase. Semantic names are terse, practical, and a
little playful; address-only names remain only until the data flow says what a
thing does.

## 名前

- Prefer short ASCII Japanese nouns for assets: `iwamuro`, `ougonmon`, `taki`,
  `ki`, `hashi`. Use the spelling a Japanese programmer of the period could
  plausibly have typed, not a modern localization phrase.
- Prefer compact period abbreviations in code where they stay readable:
  `Flg`, `Ev`, `Btl`, `Chr`, `Tbl`, `Work`, `Pos`, `No`, `Get`, and `Set`.
- Name from recovered function or composition. A visible rock chamber may be
  `iwamuro`; a tile that merely resembles foliage does not become a particular
  species until its repeated composition supports that reading.
- Japanese labels may accompany the source name (`岩室`, `黄金門`). English is
  a short gloss for contributors, not the canonical voice of the asset.
- Do not add `maybe`, `guess`, or confidence scores to source identifiers. If
  the semantics are too weak for a useful period name, retain the stable
  numeric resource name and revisit it when the pipeline supplies more context.

These are canonical reconstruction names. They are intentionally styled as if
they belonged to the original project, but an exact historical spelling is
claimed only when surviving game data actually contains it.

## コメント

Comments diagnose the overall pipeline, never one screenshot. State the data
relationship, invariant, or failure class and the family of outputs it affects.
A particular scene may be a regression fixture, but it is not the explanation.

ソース内のコメントはUTF-8の日本語だけで記述する。英文の併記はしない。
当時らしい簡潔な漢字かな交じり文を基本とし、外来語にはカタカナを使う。
半角カナも使用できるが、識別子、命令名、数値など必要なASCII表記はそのまま残す。
コメントは画像一枚の感想ではなく、処理系全体の関係、不変条件、障害分類を書く。
例：

```text
マップチップ切替。ヘッダ値に従い表示窓へ文字ブロックを割り当てる。
セル番号は下位12bit。上位4bitは別の属性値として扱う。
左右反転を戻す。表示時の反転を取り消して標準タイルへ戻す。
```

Avoid diary comments, image-by-image observations, model speculation, and
claims that a name or comment was recovered when only its semantics were.
