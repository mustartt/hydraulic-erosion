import React, { useEffect, useState, useRef } from 'react';
import './App.css';
import { createMuiTheme, ThemeProvider } from '@material-ui/core';
import { NoiseParam, ErosionParam, ViewportParam, EditorParam } from './setting-types';
import Editor from './components/editor';
import Viewport from './components/viewport';

/* wasm module */
import MainModule from './output';
const Module = MainModule();

// Use Material UI Dark theme
const theme = createMuiTheme({
  palette: {
    type: 'dark'
  }
});


/* Default Settings */
const defaultNoiseParam: NoiseParam = {
  seed: 1,
  octaves: 4,
  persistence: 0.45,
  scale: 1,
  height: 1
};

const defaultErosionParam: ErosionParam = {
  droplet_lifetime: 30,
  inertia: 0.05,
  sediment_capacity_factor: 4,
  min_sediment_capacity_factor: 0.01,
  deposit_speed: 0.3,
  erode_speed: 0.3,
  evaporate_speed: 0.01,
  gravity: 4,
  drop_radius: 3
};

const defaultViewportParam: ViewportParam = {
  mesh_size: 256,
  map_size: 512,
  output_height: 0.5
};




const App:React.FC = () => {  
  /* Load WASM Module */
  const [ready, setReady] = useState<boolean>(false);
  const [module, setModule] = useState<any>(null);
  const [api, setAPI] = useState<any>(null);

  useEffect(() => {
    Module.then((module: any) => {
      console.log("WASM Module is loading.");
      
      /* Creating the api object */
      const api_obj = {
        // returns the version of the WASM Module
        version: module.cwrap('version', 'number', []),
        // allocates the heightmap
        create_map: module.cwrap('initialize', 'number', ['number']),
        // returns the points to the heightmap
        get_map: module.cwrap('get_heightmap', 'number', []),
        // samples the heightmap at x y
        sample: module.cwrap('sample', null, ['number', 'number']),
        // use default erosion. Set noise param
        default_erosion_param: module.cwrap('use_default_erosion_params', null, 
          ['number', 'number', 'number', 'number', 'number']),
        // set full param <- Always use this
        set_param: module.cwrap('set_parameters', null, 
          ['number', 'number', 'number', 'number', 'number', 'number', 'number', 
          'number', 'number', 'number', 'number', 'number', 'number']),
        // generates noise
        generate_noise: module.cwrap('generate_noise', null, []),
        // erode n iterations with radius r
        erode: module.cwrap('erode_iter', null, ['number', 'number']),
        // frees the map
        free_map: module.cwrap('free_heightmap', null, []),
        /* Export Functions */
        save_obj: module.cwrap('save_obj', null, ['string', 'number']),
        save_png: module.cwrap('save_png', null, ['string']),
        save_stl: module.cwrap('save_stl', null, ['string']),
        download: (filename: string) => {
          let mime = "application/octet-stream";
          let content = module.FS.readFile(filename);
          console.log(`Offering download of "${filename}", with ${content.length} bytes...`);
        
          var a = document.createElement('a');
          a.download = filename;
          a.href = URL.createObjectURL(new Blob([content], {type: mime}));
          a.style.display = 'none';
        
          document.body.appendChild(a);
          a.click();
          setTimeout(() => {
            document.body.removeChild(a);
            URL.revokeObjectURL(a.href);
          }, 2000);
        }
      };      

      api_obj.create_map(viewportParam.map_size);
      api_obj.set_param(noiseParam.seed, noiseParam.octaves, 
        noiseParam.persistence,
        noiseParam.scale, noiseParam.height,
        erosionParam.droplet_lifetime, erosionParam.inertia, 
        erosionParam.sediment_capacity_factor, 
        erosionParam.min_sediment_capacity_factor,
        erosionParam.deposit_speed, 
        erosionParam.erode_speed,
        erosionParam.evaporate_speed,
        erosionParam.gravity);
      api_obj.generate_noise();
      api_obj.save_png('output.png');

      setAPI(api_obj);
      setModule(module);
      setReady(true);

      console.log("WASM Module is loaded.");
      console.log("Version:", api_obj.version());
    });
  }, []);
  
  /* Setup params */
  const [noiseParam, setNoiseParam] = useState(defaultNoiseParam);
  const [erosionParam, setErosionParam] = useState(defaultErosionParam);
  const [viewportParam, setViewportParam] = useState(defaultViewportParam);

  const handleNoiseUpdate = (param: NoiseParam) : void => {
    const newNoiseParam = {...param};
    
    api.set_param(newNoiseParam.seed, newNoiseParam.octaves, 
      newNoiseParam.persistence,
      newNoiseParam.scale, newNoiseParam.height,
      erosionParam.droplet_lifetime, erosionParam.inertia, 
      erosionParam.sediment_capacity_factor, 
      erosionParam.min_sediment_capacity_factor,
      erosionParam.deposit_speed, 
      erosionParam.erode_speed,
      erosionParam.evaporate_speed,
      erosionParam.gravity);
    api.generate_noise();
    api.save_png('output.png');

    setNoiseParam(newNoiseParam);
  };

  const handleErosionUpdate = (param: ErosionParam) : void => {
    const newErosionParam = {...param};
    // discard any erosion change
    // regenerate based on noise param
    setErosionParam(newErosionParam);
  };

  const handleViewportUpdate = (param: ViewportParam) : void => {
    const newViewportParam = {...param};
    api.free_map();
    api.create_map(param.map_size);
    api.generate_noise();

    setViewportParam(newViewportParam);
  }

  const [toggle, setToggle] = useState(true);

  const beginErosion = () => {
    let size = viewportParam.map_size;
    let iter = size * size * 3;
    console.time('erode');
    api.erode(iter, erosionParam.drop_radius);
    console.timeEnd('erode');
    setToggle(!toggle);
    api.save_png('output.png');
  }

  const beginDownload = () => {
    api.download('output.png');
  }

  const props: EditorParam = {
    noiseParam,
    erosionParam,
    viewportParam,
    handleNoiseUpdate,
    handleErosionUpdate,
    handleViewportUpdate,
    beginErosion,
    beginDownload
  }

  const viewportProps = {
    api: api,
    viewportParam: viewportParam,
    ready: ready
  }

  return (
    <div className="app-container">
      <ThemeProvider theme={theme}>
        <Viewport {...viewportProps}></Viewport>
        <Editor {...props} 
          beginErosion={beginErosion}
          beginDownload={beginDownload}></Editor>
      </ThemeProvider>
    </div>
  );
}

export default App;
