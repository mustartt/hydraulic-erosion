
export interface NoiseParam {
  seed: number,
  octaves: number,
  persistence: number,
  scale: number,
  height: number
};

export interface ErosionParam {
  droplet_lifetime: number,
  inertia: number,
  sediment_capacity_factor: number,
  min_sediment_capacity_factor: number,
  deposit_speed: number,
  erode_speed: number,
  evaporate_speed: number,
  gravity: number,
  drop_radius: number
};

export interface ViewportParam {
  mesh_size: number,
  map_size: number,
  output_height: number 
}

export type HandleNoiseUpdate = (newNoiseParam: NoiseParam) => void;
export type HandleErosionUpdate = (newErosionParam: ErosionParam) => void;
export type HandleViewportUpdate = (newViewParam: ViewportParam) => void;

export interface EditorParam {
  noiseParam: NoiseParam,
  erosionParam: ErosionParam,
  viewportParam: ViewportParam,
  handleNoiseUpdate: HandleNoiseUpdate,
  handleErosionUpdate: HandleErosionUpdate,
  handleViewportUpdate: HandleViewportUpdate
}

