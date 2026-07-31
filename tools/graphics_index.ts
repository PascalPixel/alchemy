// assets/graphics/ は 8,000 枚超の平坦な一覧である。移設は書き出し経路を
// 変えるため、ここでは索引だけを生成して意味を与える。画像は動かさない。
//
// Catalogue the flat graphics directory so it can be navigated without moving a
// byte. assets/graphics/ holds 8,000+ files in one directory because a hierarchy
// was flattened; relocating them would change what the asset build resolves, and
// that needs a byte-identical build to prove. An index needs nothing but the
// listing, so it is safe here and useful immediately.
//
//   bun tools/graphics_index.ts --write      rewrite assets/graphics/index.json
//   bun tools/graphics_index.ts --check      fail if the tracked index is stale
//   bun tools/graphics_index.ts --self-test
import { readdirSync, readFileSync, writeFileSync, existsSync } from "node:fs";
import { dirname, join, resolve } from "node:path";
import { canonicalJson } from "./canonical_json.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const GRAPHICS = join(ROOT, "assets", "graphics");
const INDEX = join(GRAPHICS, "index.json");

export interface Family {
  /** Stable key, e.g. "battle/characters/chr_000". */
  readonly path: string;
  /** Japanese romaji term the project already uses for this shape, if any. */
  readonly term: string | null;
  readonly files: number;
  readonly members: readonly string[];
}

/**
 * The naming vocabulary already in the tree. `koma` (駒) is the established term
 * for an animation frame and `iwa` (岩) for rock; both predate this tool. New
 * families should extend this list rather than inventing English equivalents —
 * Japanese romaji, snake_case, is the convention on this project.
 */
const TERMS: ReadonlyArray<readonly [RegExp, string]> = [
  [/_koma_\d+\.png$/i, "koma"],
  [/^sentou_/i, "sentou"],
  [/^fonts_/i, "moji"],
  [/^palettes_|\.gbapal\.png$/i, "iro"],
];

function termFor(name: string): string | null {
  for (const [pattern, term] of TERMS) if (pattern.test(name)) return term;
  return null;
}

/**
 * Group a flat listing into the hierarchy its names imply. The directory was
 * flattened by replacing `/` with `_`, so the structure is recoverable from the
 * names alone: `battle_characters_chr_000_koma_000.png` was
 * `battle/characters/chr_000/koma_000.png`.
 */
export function groupFamilies(listing: readonly string[]): Family[] {
  const groups = new Map<string, string[]>();
  for (const name of listing) {
    if (name === "index.json") continue;
    let key: string;
    // Keep the ordinal: chr_000 is a character, chr is not a family of 5,465.
    const character = /^((?:battle|field)_characters_[a-z]+_[0-9a-f]+)_/i.exec(name) ??
      /^((?:battle|field)_characters_[a-z]+)_/i.exec(name);
    const mapResource = /^(map_resource_[0-9a-f]+)_/i.exec(name);
    const resource = /^(resource_[0-9a-f]+)_/i.exec(name);
    if (character) key = character[1].replace(/_/g, "/");
    else if (mapResource) key = `maps/${mapResource[1]}`;
    else if (resource) key = `resources/${resource[1]}`;
    else if (/^[0-9a-f]{8}\./i.test(name)) key = "unnamed-by-address";
    else key = name.split("_")[0] || "other";
    if (!groups.has(key)) groups.set(key, []);
    groups.get(key)!.push(name);
  }
  return [...groups.entries()]
    .map(([path, members]) => ({
      path,
      term: termFor(members[0] ?? ""),
      files: members.length,
      members: [...members].sort(),
    }))
    .sort((left, right) => left.path.localeCompare(right.path));
}

function build(): { document: unknown; families: Family[] } {
  const families = groupFamilies(readdirSync(GRAPHICS).sort());
  return {
    families,
    document: {
      format: 1,
      kind: "golden-sun-graphics-index",
      note:
        "Derived from the flat listing of assets/graphics. Regenerate with " +
        "`bun tools/graphics_index.ts --write`; do not hand-edit.",
      files: families.reduce((sum, family) => sum + family.files, 0),
      families: families.map(({ path, term, files, members }) => ({ path, term, files, members })),
    },
  };
}

function selfTest(): void {
  const grouped = groupFamilies([
    "battle_characters_chr_000_koma_000.png",
    "battle_characters_chr_000_koma_001.png",
    "battle_characters_chr_000_bank.json",
    "field_characters_chr_00a_koma_000.png",
    "map_resource_1be_charblock1.4bpp.png",
    "resource_17_iwa.8bpp.png",
    "0800777c.gbapal.png",
    "index.json",
  ]);
  const byPath = new Map(grouped.map((family) => [family.path, family]));
  // The ordinal must survive: one family per character, not one for all of them.
  if (byPath.get("battle/characters/chr/000")?.files !== 3) {
    throw new Error("character family grouping lost the ordinal");
  }
  if (byPath.get("field/characters/chr/00a")?.files !== 1) {
    throw new Error("field character family grouping failed");
  }
  if (byPath.get("maps/map_resource_1be")?.files !== 1) throw new Error("map family grouping failed");
  if (byPath.get("resources/resource_17")?.files !== 1) throw new Error("resource family grouping failed");
  if (byPath.get("unnamed-by-address")?.files !== 1) throw new Error("address-named grouping failed");
  if (grouped.some((family) => family.members.includes("index.json"))) {
    throw new Error("the index listed itself");
  }
  const koma = grouped.find((family) => family.term === "koma");
  if (!koma) throw new Error("the koma vocabulary term was not applied");
  if (grouped.some((family) => family.files !== family.members.length)) {
    throw new Error("family count disagrees with its members");
  }
  console.log("self-test=ok tool=graphics_index");
}

function main(argv: string[]): void {
  if (argv.includes("--self-test")) return selfTest();
  const { document, families } = build();
  const json = canonicalJson(document);
  if (argv.includes("--check")) {
    if (!existsSync(INDEX) || readFileSync(INDEX, "utf8") !== json) {
      throw new Error("tracked graphics index is stale; run: bun tools/graphics_index.ts --write");
    }
    console.log(`graphics-index=current families=${families.length}`);
    return;
  }
  if (argv.includes("--write")) {
    writeFileSync(INDEX, json);
    console.log(
      `index=${resolve(INDEX).slice(ROOT.length + 1)} families=${families.length} ` +
      `files=${families.reduce((sum, family) => sum + family.files, 0)}`,
    );
    return;
  }
  console.log(`families=${families.length} files=${families.reduce((s, f) => s + f.files, 0)}`);
}

if (import.meta.main) main(Bun.argv.slice(2));
