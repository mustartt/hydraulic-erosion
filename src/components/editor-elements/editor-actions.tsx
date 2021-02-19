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

interface ActionsProps {
  beginErosion: () => void;
  beginDownload: () => void;
};

const Actions:React.FC<ActionsProps> = (props) => {
    const classes = useStyles();

    //const [play, setPlay] = useState(true);
    const { beginErosion, beginDownload } = props;

    return (
        <div className={classes.root}>
            <Button variant="contained" onClick={() => {
              beginErosion();
            }}>
              <Icon>play_arrow</Icon>
                Start
            </Button>
            <Button variant="contained" onClick={() => {
              beginDownload();
            }}>
              <Icon>get_app</Icon>
              Export
            </Button>
        </div>
    );
}

export default Actions