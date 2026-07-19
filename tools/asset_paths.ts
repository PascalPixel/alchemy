import { existsSync } from "node:fs";
import { dirname, resolve } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

// リソース図形の格納先を解決する。map/ へ移設済みの資源を優先し、
// 未移設の資源は従来位置へ退避する。返り値はROOT基準の相対パス。
export function resourceGraphicsDir(name: string): string {
  const relocated = `assets/graphics/map/resource_${name}`;
  if (existsSync(resolve(ROOT, relocated))) return relocated;
  return `assets/graphics/resource_${name}`;
}

// 人物バンクの格納先を解決する。分類済みの battle/characters と
// field/characters を順に探し、未移設は family 直下へ退避する。
// plan は "chr_NNN/bank.json" 形式。返り値は絶対パス。
export function characterBankPath(familyRoot: string, plan: string): string {
  for (const semantic of ["battle", "field"]) {
    const relocated = resolve(ROOT, "assets/graphics", semantic, "characters", plan);
    if (existsSync(relocated)) return relocated;
  }
  return resolve(familyRoot, plan);
}
