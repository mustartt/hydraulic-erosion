import React from 'react';
import './App.css';

import { createMuiTheme, ThemeProvider } from '@material-ui/core'

import Editor from './components/editor'
import Viewport from './components/viewport'

// Use Material UI Dark theme
const theme = createMuiTheme({
  palette: {
    type: 'dark'
  }
});

function tests(){

    const [noise, setNoise] = useState(0)
    
    return(
    <div>
    <button onClick = {() => setNoise(70 * (noise +1))}> Noise <noise> </button>
    <div>
    )
}

const App = () => {
  return (
    <div className="app-container">
      <ThemeProvider theme={theme}>
        <Viewport></Viewport>
        <Editor></Editor>
      </ThemeProvider>
    </div>
  );
}

export default App;
