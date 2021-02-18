import React from 'react';
import { makeStyles } from '@material-ui/core/styles';
import { Typography } from '@material-ui/core';

import InputSlider from './value-slider';

import { EditorParam } from '../../setting-types';

const useStyles = makeStyles((theme) => ({
    root: {
      '& > *': {
        margin: theme.spacing(1),
      },
    },
  }));

const SimEditor:React.FC<EditorParam> = (prop) => {
    const classes = useStyles();
    const { erosionParam, handleErosionUpdate } = prop;

    return (
        <div className={classes.root}>
            <Typography className={classes.root} variant="h6">
                Erosion Settings
            </Typography>
            <InputSlider name="Droplet Radius" 
              val={erosionParam.drop_radius} 
              min={1} max={6} step={1} 
              handleChange={(val) => {
                erosionParam.drop_radius = val;
                handleErosionUpdate(erosionParam);
            }}/>
            <InputSlider name="Droplet Lifetime" 
              val={erosionParam.droplet_lifetime} 
              min={10} max={50} step={1} 
              handleChange={(val) => {
                erosionParam.droplet_lifetime = val;
                handleErosionUpdate(erosionParam);
            }}/>
            <InputSlider name="Inertia" 
              val={erosionParam.inertia} 
              min={0.0001} max={0.3} step={.001} 
              handleChange={(val) => {
                erosionParam.inertia = val;
                handleErosionUpdate(erosionParam);
            }}/>
            <InputSlider name="Sediment Capacity Factor" 
              val={erosionParam.sediment_capacity_factor} 
              min={1} max={10} step={0.1} 
              handleChange={(val) => {
                erosionParam.sediment_capacity_factor = val;
                handleErosionUpdate(erosionParam);
            }}/>
            <InputSlider name="Deposit Speed" 
              val={erosionParam.deposit_speed} 
              min={0.05} max={0.95} step={0.05} 
              handleChange={(val) => {
                erosionParam.deposit_speed = val;
                handleErosionUpdate(erosionParam);
            }}/>
            <InputSlider name="Erode Speed" 
              val={erosionParam.erode_speed} 
              min={0.05} max={0.95} step={0.05} 
              handleChange={(val) => {
                erosionParam.erode_speed = val;
                handleErosionUpdate(erosionParam);
            }}/>
            <InputSlider name="Evaporate Speed" 
              val={erosionParam.evaporate_speed} 
              min={0.001} max={0.05} step={0.0001} 
              handleChange={(val) => {
                erosionParam.evaporate_speed = val;
                handleErosionUpdate(erosionParam);
            }}/>
            <InputSlider name="Gravity" 
              val={erosionParam.gravity} 
              min={1} max={10} step={0.01} 
              handleChange={(val) => {
                erosionParam.gravity = val;
                handleErosionUpdate(erosionParam);
            }}/>

        </div>
    );
}

export default SimEditor