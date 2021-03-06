import React from 'react'
import { makeStyles } from '@material-ui/core/styles';
import { Typography } from '@material-ui/core'

import InputSlider from './value-slider'

import { EditorParam } from '../../setting-types';

const useStyles = makeStyles((theme) => ({
  root: {
    '& > *': {
      margin: theme.spacing(1),
    },
  },
}));


const NoiseEditor: React.FC<EditorParam> 
= (prop) => {

  const classes = useStyles();
  const { noiseParam, handleNoiseUpdate } = prop;

  return (
      <div className={classes.root}>
          <Typography className={classes.root} variant="h6">
              Noise Settings
          </Typography>
          <InputSlider name="Seed" val={noiseParam.seed} min={0} max={999} step={1} handleChange={(val) => {
            noiseParam.seed = val;
            handleNoiseUpdate(noiseParam);
          }}/>
          <InputSlider name="Octaves" val={noiseParam.octaves} min={1} max={10} step={1} handleChange={(val) => {
            noiseParam.octaves = val;
            handleNoiseUpdate(noiseParam);
          }}/>
          <InputSlider name="Persistence" val={noiseParam.persistence} min={0} max={1} step={0.01} handleChange={(val) => {
            noiseParam.persistence = val;
            handleNoiseUpdate(noiseParam);
          }}/>
          <InputSlider name="Scale" val={noiseParam.scale} min={0.1} max={3} step={0.05} handleChange={(val) => {
            noiseParam.scale = val;
            handleNoiseUpdate(noiseParam);
          }}/>
          <InputSlider name="Height" val={noiseParam.height} min={0} max={1.5} step={0.01} handleChange={(val) => {
            noiseParam.height = val;
            handleNoiseUpdate(noiseParam);
          }}/>
      </div>
  );
}

export default NoiseEditor;