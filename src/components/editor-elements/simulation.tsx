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
            <InputSlider name="Droplet Radius" val={2} min={1} max={6} step={1} handleChange={(val) => {
              erosionParam.drop_radius = val;
              handleErosionUpdate(erosionParam);
            }}/>
            <InputSlider name="Droplet Lifetime" val={30} min={10} max={50} step={1} handleChange={(val) => {
              erosionParam.droplet_lifetime = val;
              handleErosionUpdate(erosionParam);
            }}/>
            <InputSlider name="Inertia" val={0.05} min={0.001} max={0.5} step={.001} handleChange={(val) => {
              erosionParam.inertia = val;
              handleErosionUpdate(erosionParam);
            }}/>
            <InputSlider name="Sediment Capacity Factor" val={4} min={1} max={10} step={0.1} handleChange={(val) => {
              erosionParam.sediment_capacity_factor = val;
              handleErosionUpdate(erosionParam);
            }}/>
            <InputSlider name="Deposit Speed" val={0.3} min={0.05} max={0.95} step={0.05} handleChange={(val) => {
              erosionParam.deposit_speed = val;
              handleErosionUpdate(erosionParam);
            }}/>
            <InputSlider name="Erode Speed" val={0.3} min={0.05} max={0.95} step={0.05} handleChange={(val) => {
              erosionParam.erode_speed = val;
              handleErosionUpdate(erosionParam);
            }}/>
            <InputSlider name="Evaporate Speed" val={0.1} min={0.01} max={0.5} step={0.01} handleChange={(val) => {
              erosionParam.evaporate_speed = val;
              handleErosionUpdate(erosionParam);
            }}/>
            <InputSlider name="Gravity" val={4} min={1} max={10} step={0.01} handleChange={(val) => {
              erosionParam.gravity = val;
              handleErosionUpdate(erosionParam);
            }}/>

        </div>
    );
}

export default SimEditor