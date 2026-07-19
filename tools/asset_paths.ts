import { existsSync } from "node:fs";
import { dirname, resolve } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

// リソース図形の格納先を解決する。map-tiles/ へ移設済みの資源を優先し、
// 未移設の資源は従来位置へ退避する。返り値はROOT基準の相対パス。
export function resourceGraphicsDir(name: string): string {
  const relocated = `assets/graphics/map-tiles/resource_${name}`;
  if (existsSync(resolve(ROOT, relocated))) return relocated;
  return `assets/graphics/resource_${name}`;
}
