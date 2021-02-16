import React from 'react';
import { makeStyles } from '@material-ui/core/styles';
import { Typography } from '@material-ui/core'

import InputSlider from './value-slider';

import { EditorParam } from '../../setting-types';

const useStyles = makeStyles((theme) => ({
  root: {
    '& > *': {
      margin: theme.spacing(1),
    },
  },
}));

const ViewEditor:React.FC<EditorParam> = (prop) => {
  const classes = useStyles();
  const { viewportParam, handleViewportUpdate } = prop;

  return (
    <div className={classes.root}>
      <Typography className={classes.root} variant="h6">
          Viewport Settings
      </Typography>
      <InputSlider name="Mesh Size" val={256} min={32} max={1024} step={16} 
      handleChange={(val) => {
        viewportParam.mesh_size = val;
        handleViewportUpdate(viewportParam);
      }}/>
      <InputSlider name="Map Size" val={512} min={128} max={1024} step={16} 
      handleChange={(val) => {
        viewportParam.map_size = val;
        handleViewportUpdate(viewportParam);
      }}/>
      <InputSlider name="Output Height" val={0.5} min={0} max={1} step={0.01} 
      handleChange={(val) => {
        viewportParam.output_height = val;
        handleViewportUpdate(viewportParam);
      }}/>
    </div>
  );
}

export default ViewEditor