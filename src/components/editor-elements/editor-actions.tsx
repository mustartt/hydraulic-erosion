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

const Actions = () => {
    const classes = useStyles();

    return (
        <div className={classes.root}>
            <Button variant="contained">
              <Icon>play_arrow</Icon>
              Start
            </Button>
            <Button variant="contained">
              <Icon>get_app</Icon>
              Export
            </Button>
        </div>
    );
}

export default Actions