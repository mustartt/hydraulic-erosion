import React, { useEffect, useState } from 'react';
import './App.css';
import { createMuiTheme, ThemeProvider } from '@material-ui/core';
import { NoiseParam, ErosionParam, ViewportParam, EditorParam } from './setting-types';
import Editor from './components/editor';
import Viewport from './components/viewport';

import MainModule from './output';
import { read } from 'fs';
const Module = MainModule();

// Use Material UI Dark theme
const theme = createMuiTheme({
  palette: {
    type: 'dark'
  }
});


/* Default Settings */
const defaultNoiseParam: NoiseParam = {
  seed: 12345,
  octaves: 5,
  persistence: 0.45,
  scale: 3,
  height: 1
};

const defaultErosionParam: ErosionParam = {
  droplet_lifetime: 30,
  inertia: 0.05,
  sediment_capacity_factor: 4,
  min_sediment_capacity_factor: 0.01,
  deposit_speed: 0.3,
  erode_speed: 0.3,
  evaporate_speed: 0.1,
  gravity: 4,
  drop_radius: 3
};

const defaultViewportParam: ViewportParam = {
  mesh_size: 256,
  map_size: 512,
  output_height: 0.5
};


const App = () => {  
  /* Load WASM Module */
  const [ready, setReady] = useState<boolean>(false);
  const [module, setModule] = useState<any>(null);
  const [api, setAPI] = useState<any>(null);

  useEffect(() => {
    Module.then((module: any) => {
      console.log("WASM Module is loading.");
      
      /* Creating the api object */
      const api_obj = {
        version: module.cwrap('version', 'number', []),
        create_map: module.cwrap('initialize', 'number', ['number', 'number']),
        default_erosion_param: module.cwrap('use_default_erosion_params', null, 
          ['number', 'number', 'number', 'number', 'number', 'number']),
        set_param: module.cwrap('set_parameters', null, 
          ['number', 'number', 'number', 'number', 'number', 'number', 'number', 
          'number', 'number', 'number', 'number', 'number', 'number', 'number']),
        generate_noise: module.cwrap('generate_noise', null, []),
        erode: module.cwrap('erode_iter', null, ['number']),
        teardown: module.cwrap('teardown', null, []),
        save_obj: module.cwrap('save_obj', null, ['string', 'number']),
        save_png: module.cwrap('save_png', null, ['string']),
        save_stl: module.cwrap('save_stl', null, ['string']),
        sample: module.cwrap('sample', null, ['number', 'number'])
      };      

      // BUG: Top half of the heightmap has a blackbar over it
      api_obj.create_map(null, viewportParam.map_size);
      api_obj.default_erosion_param(noiseParam.seed, noiseParam.octaves, 
                                    noiseParam.persistence, noiseParam.scale, 
                                    noiseParam.height, erosionParam.drop_radius);

      api_obj.generate_noise();

      setAPI(api_obj);
      setModule(module);
      setReady(true);

      console.log("WASM Module is loaded.");
    });
  }, []);

  
  /* Setup params */
  const [noiseParam, setNoiseParam] = useState(defaultNoiseParam);
  const [erosionParam, setErosionParam] = useState(defaultErosionParam);
  const [viewportParam, setViewportParam] = useState(defaultViewportParam);

  const handleNoiseUpdate = (param: NoiseParam) : void => {
    const newNoiseParam = {...param};
    
    api.set_param(newNoiseParam.seed, newNoiseParam.octaves, newNoiseParam.persistence, 
                  newNoiseParam.scale, newNoiseParam.height, erosionParam.droplet_lifetime,
                  erosionParam.inertia, erosionParam.sediment_capacity_factor, 
                  erosionParam.min_sediment_capacity_factor,
                  erosionParam.deposit_speed, erosionParam.erode_speed,
                  erosionParam.evaporate_speed, erosionParam.gravity,
                  erosionParam.drop_radius);
    api.generate_noise();

    setNoiseParam(newNoiseParam);
  };

  const handleErosionUpdate = (param: ErosionParam) : void => {
    const newErosionParam = {...param};
    
    api.set_param(noiseParam.seed, noiseParam.octaves, noiseParam.persistence, 
      noiseParam.scale, noiseParam.height, newErosionParam.droplet_lifetime,
      newErosionParam.inertia, newErosionParam.sediment_capacity_factor, 
      newErosionParam.min_sediment_capacity_factor,
      newErosionParam.deposit_speed, newErosionParam.erode_speed,
      newErosionParam.evaporate_speed, newErosionParam.gravity,
      newErosionParam.drop_radius);
    api.generate_noise();

    setErosionParam(newErosionParam);
  };

  const handleViewportUpdate = (param: ViewportParam) : void => {
    const newViewportParam = {...param};
    // Do some stuff here...
    setViewportParam(newViewportParam);
  }

  const props: EditorParam = {
    noiseParam,
    erosionParam,
    viewportParam,
    handleNoiseUpdate,
    handleErosionUpdate,
    handleViewportUpdate
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
        <Editor {...props}></Editor>
      </ThemeProvider>
    </div>
  );
}

export default App;
