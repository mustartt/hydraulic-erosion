import React, { useRef } from "react";
import { Canvas, MeshProps, useFrame } from "react-three-fiber";
import { Mesh } from "three";

import "./viewport.css";


// testing:
const Box: React.FC<MeshProps> = (props) => {
  const mesh = useRef<Mesh>();

  useFrame(() => {
    if (mesh.current) {
      mesh.current.rotation.x += 0.005
      mesh.current.rotation.y += 0.005
    }
  });

  return (
    <mesh
      {...props}
      ref={mesh}
      scale={[1,1,1]}>
      <boxBufferGeometry args={[1,1,1]} />
      <meshStandardMaterial color={'orange'}/>
    </mesh>
  )
}


const Viewport = () => {
  return (
    <div className="viewport">
      <Canvas>
        <ambientLight></ambientLight>
        <pointLight position={[10, 10, 10]}></pointLight>
        <Box position={[-1, 0, 0]}/>
      </Canvas>
    </div>
    );
};

export default Viewport;
