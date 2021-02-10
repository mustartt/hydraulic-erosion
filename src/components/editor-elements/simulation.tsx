import React from 'react'
import { makeStyles } from '@material-ui/core/styles';
import { Typography } from '@material-ui/core'

import InputSlider from './value-slider'

const useStyles = makeStyles((theme) => ({
    root: {
      '& > *': {
        margin: theme.spacing(1),
      },
    },
  }));

const SimEditor = () => {
    const classes = useStyles();

    return (
        <div className={classes.root}>
            <Typography className={classes.root} variant="h6">
                Erosion Settings
            </Typography>
            <InputSlider name="Slider 1" val={50} min={0} max={100} step={10}></InputSlider>
            <InputSlider name="Slider 2" val={50} min={0} max={100} step={10}></InputSlider>
            <InputSlider name="Slider 3" val={50} min={0} max={100} step={10}></InputSlider>
        </div>
    );
}

export default SimEditor