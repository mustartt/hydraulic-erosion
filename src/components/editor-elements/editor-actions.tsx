import React, { useState } from 'react'
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

    const [play, setPlay] = useState(true);

    return (
        <div className={classes.root}>
            <Button variant="contained" onClick={() => {
              setPlay(val => !val);
            }}>
              { play ? 
                <>
                  <Icon>play_arrow</Icon>
                    Start
                </> : <> 
                  <Icon>stop</Icon>
                    Stop
                </>}
            </Button>
            <Button variant="contained">
              <Icon>get_app</Icon>
              Export
            </Button>
        </div>
    );
}

export default Actions