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
            <Paper>
                <Typography variant="h4">
                    Settings
                </Typography>

                <Starting></Starting>
                <NoiseEditor></NoiseEditor>
                <SimEditor></SimEditor>
                <ViewEditor></ViewEditor>
                <Actions></Actions>
            </Paper>
        </div>
    );
}

export default Editor