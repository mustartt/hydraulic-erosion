import React, { useEffect, useRef } from "react";
import './viewport.css'

import { ViewportParam } from '../setting-types';

interface ViewportProps {
  api: any,
  viewportParam: ViewportParam,
  ready: boolean
};

const ImageViewer: React.FC<ViewportProps> = (props) => {
  const {ready, api, viewportParam} = props;
  const size = viewportParam.map_size;
  const canvasRef = useRef<HTMLCanvasElement>(null);

  useEffect(() => {
    const context = canvasRef.current?.getContext('2d')
    if (context) {
      if (ready) {
        let imageData = context.createImageData(size, size);
        for (let x = 0; x < size; x++) {
          for (let y = 0; y < size; y++) {
            let i = (y * size + x) * 4; // RGBA 
            let value = api.sample(x, y) * 255;
            //let value = 128;
            imageData.data[i] = value;
            imageData.data[i + 1] = value;
            imageData.data[i + 2] = value;
            imageData.data[i + 3] = 255;
          }
        }

        context.putImageData(imageData, 0, 0);
      } else {
        context.fillStyle = '#ff0000';
        context.fillRect(0, 0, context.canvas.width, context.canvas.height);
      }
    }
  });

  return <canvas ref={canvasRef} width={size} height={size}/>;
}

const Viewport: React.FC<ViewportProps> = (props) => {
  return (
    <div className="viewport">
      <ImageViewer {...props}></ImageViewer>
    </div>
    );
};

export default Viewport;
