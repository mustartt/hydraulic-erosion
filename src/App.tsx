import React, { useEffect, useState } from 'react';
import './App.css';
import { createMuiTheme, ThemeProvider } from '@material-ui/core';
import { NoiseParam, ErosionParam, ViewportParam, EditorParam } from './setting-types';
import Editor from './components/editor';
import Viewport from './components/viewport';

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
  /* Setup params */
  const [noiseParam, updateNoiseParam] = useState(defaultNoiseParam);
  const [erosionParam, updateErosionParam] = useState(defaultErosionParam);
  const [viewportParam, updateViewportParam] = useState(defaultViewportParam);

  const handleNoiseUpdate = (newNoiseParam: NoiseParam) : void => {
    updateNoiseParam(newNoiseParam);
    console.log(newNoiseParam);
  };

  const handleErosionUpdate = (newErosionParam: ErosionParam) : void => {
    updateErosionParam(newErosionParam);
    console.log(newErosionParam);
  };

  const handleViewportUpdate = (newViewportParam: ViewportParam) : void => {
    updateViewportParam(newViewportParam);
    console.log(newViewportParam);
  }

  const props: EditorParam = {
    noiseParam,
    erosionParam,
    viewportParam,
    handleNoiseUpdate,
    handleErosionUpdate,
    handleViewportUpdate
  }

  /* Load WASM Module */
  const [ready, setReady] = useState<boolean>(false);
  const [module, setModule] = useState<any>(null);
  const [api, setAPI] = useState<any>(null);

  useEffect(() => {
    Module.then((module: any) => {
      console.log("WASM Module is loaded.");
      
      /* Creating the api object */
      const api_obj = {
        version: module.cwrap('version', 'number', []),
        create_map: module.cwrap('initialize', null, ['number', 'number']),
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
        // sample: (x: number, y: number) => number
      };
      
      setReady(true);
      setAPI(api_obj);
      setModule(module);
    });
  }, []);

  return (
    <div className="app-container">
      <ThemeProvider theme={theme}>
        <Viewport></Viewport>
        <Editor {...props}></Editor>
      </ThemeProvider>
    </div>
  );
}

export default App;
