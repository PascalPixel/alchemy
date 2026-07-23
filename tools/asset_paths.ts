// Tool role: library; imported by tools/build_assets.ts, tools/compose_scene.ts, tools/static_sprite_series.ts (+1 more).
import { existsSync, readdirSync } from "node:fs";
import { dirname, resolve } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

// リソース図形の格納先を解決する。map/ へ移設済みの資源を優先し、
// 未移設の資源は従来位置へ退避する。返り値はROOT基準の相対パス。
let graphicsListing: string[] | null = null;
export function resourceGraphicsDir(name: string): string {
  // Flat layout: return the resource file-name prefix within assets/graphics.
  if (graphicsListing === null) graphicsListing = readdirSync(resolve(ROOT, "assets/graphics"));
  const relocated = `map_resource_${name}_`;
  if (graphicsListing.some((file) => file.startsWith(relocated))) return `assets/graphics/map_resource_${name}`;
  return `assets/graphics/resource_${name}`;
}

// 人物バンクの格納先を解決する。分類済みの battle/characters と
// field/characters を順に探し、未移設は family 直下へ退避する。
// plan は "chr_NNN/bank.json" 形式。返り値は絶対パス。
export function characterBankPath(familyRoot: string, plan: string): string {
  const flat = plan.replace(/\//g, "_");
  for (const semantic of ["battle", "field"]) {
    const relocated = resolve(ROOT, "assets/graphics", `${semantic}_characters_${flat}`);
    if (existsSync(relocated)) return relocated;
  }
  const local = resolve(familyRoot, flat);
  if (existsSync(local)) return local;
  return resolve(familyRoot, plan);
}
