import React from 'react';
import { makeStyles } from '@material-ui/core/styles';
import Grid from '@material-ui/core/Grid';
import Typography from '@material-ui/core/Typography';
import Slider from '@material-ui/core/Slider';
import Input from '@material-ui/core/Input';

const useStyles = makeStyles({
  root: {
    width: 245,
  },
  input: {
    width: 55,
  },
});


interface SliderType {
  name: string,
  val: number,
  min: number,
  max: number,
  step: number,
  handleChange: (newVal: number) => void
}


export default function InputSlider(prop: SliderType) {
  const classes = useStyles();
  const [value, setStateValue] = React.useState<number | string | Array<number | string>>(prop.val);

  const setValue = (newValue: number | number[] | string) => {
    setStateValue(newValue);
    prop.handleChange(Number(newValue));
  }

  const handleSliderChange = (event: any, newValue: number | number[]) => {
    if (newValue !== value) {
      setValue(newValue);
    }
  };

  const handleInputChange = (event: React.ChangeEvent<HTMLInputElement>) => {
    setValue(event.target.value === '' ? '' : Number(event.target.value));
  };

  const handleBlur = () => {
    if (value < prop.min) {
      setValue(prop.min);
    } else if (value > prop.max) {
      setValue(prop.max);
    }
  };

  return (
    <div className={classes.root}>
      <Typography id="input-slider" gutterBottom>
        {prop.name}
      </Typography>
      <Grid container spacing={2} alignItems="center">
        <Grid item xs>
          <Slider
            value={typeof value === 'number' ? value : 0}
            onChange={handleSliderChange}
            aria-labelledby="input-slider"
            min={prop.min}
            max={prop.max}
            step={prop.step}
          />
        </Grid>
        <Grid item>
          <Input
            className={classes.input}
            value={value}
            margin="dense"
            onChange={handleInputChange}
            onBlur={handleBlur}
            inputProps={{
              step: prop.step,
              min: prop.min,
              max: prop.max,
              type: 'number',
              'aria-labelledby': 'input-slider',
            }}
          />
        </Grid>
      </Grid>
    </div>
  );
}
