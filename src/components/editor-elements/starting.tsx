import React from 'react'
import { makeStyles } from '@material-ui/core/styles';
import { Button, Icon } from '@material-ui/core'

const useStyles = makeStyles((theme) => ({
    root: {
      '& > *': {
        margin: theme.spacing(1),
      },
    },
  }));

const Starting = () => {
    const classes = useStyles();

    return (
        <div className={classes.root}>
            <Button variant="contained">
              <Icon>add</Icon>
            </Button>
            <Button variant="contained">
              <Icon>refresh</Icon>
            </Button>
            <Button variant="contained">
              <Icon>publish</Icon>
            </Button>
        </div>
    );
}

export default Starting