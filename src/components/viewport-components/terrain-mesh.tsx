import { useForkRef } from "@material-ui/core";
import React, { useRef } from "react";
import { MeshProps, useFrame } from "react-three-fiber";
import { BufferAttribute, Mesh } from "three";


const vertices = new Float32Array( [
	-1.0, -1.0,  1.0,
	 1.0, -1.0,  1.0,
	 1.0,  1.0,  1.0,

	 1.0,  1.0,  1.0,
	-1.0,  1.0,  1.0,
	-1.0, -1.0,  1.0
] );

const TerrainMesh:React.FC<MeshProps> = (props) => {
  
  const mesh = useRef<Mesh>()
  
  useFrame(() => {
    
  });

  return (
    <mesh
      {...props}
      ref={mesh}
      scale={[1,1,1]}
      position={[0,0,0]}>
        <bufferGeometry 
          attributes={{'position': new BufferAttribute(vertices, 3)}}>
        </bufferGeometry>
        <meshBasicMaterial color={'red'} />
    </mesh> 
  );
}

export default TerrainMesh;