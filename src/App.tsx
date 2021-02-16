import React, { useState } from 'react';
import './App.css';

import { createMuiTheme, ThemeProvider } from '@material-ui/core';

import Editor from './components/editor';
import Viewport from './components/viewport';

import { NoiseParam, ErosionParam, ViewportParam, EditorParam } from './setting-types';

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

  const [noiseParam, updateNoiseParam] = useState(defaultNoiseParam);
  const [erosionParam, updateErosionParam] = useState(defaultErosionParam);
  const [viewportParam, updateViewportParam] = useState(defaultViewportParam);

  const handleNoiseUpdate = (newNoiseParam: NoiseParam) : void => {
    // call updateNoiseParam 
    console.log(newNoiseParam);
  };

  const handleErosionUpdate = (newErosionParam: ErosionParam) : void => {
    // call updateErosionParam
    console.log(newErosionParam);
  };

  const handleViewportUpdate = (newViewportParam: ViewportParam) : void => {
    // call updateViewportParam
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
