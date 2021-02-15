import React from 'react'
import { Paper, Typography } from '@material-ui/core'

import './editor.css'

import Starting from './editor-elements/starting'
import NoiseEditor from './editor-elements/noise'
import SimEditor from './editor-elements/simulation'
import ViewEditor from './editor-elements/views'
import Actions from './editor-elements/editor-actions'

const Editor = () => {
    return (
        <div className="editor-pane">
            <Paper className="prop-pane">
                <Typography variant="h4">Settings</Typography>
                <Starting></Starting>
            </Paper>
            <Paper className="prop-pane">
                <NoiseEditor></NoiseEditor>
            </Paper>
            <Paper className="prop-pane">
                <SimEditor></SimEditor>
            </Paper>
            <Paper className="prop-pane">
                <ViewEditor></ViewEditor>
            </Paper>
            <Paper className="prop-pane">
                <Actions></Actions>
            </Paper>
        </div>
    );
}

export default Editor