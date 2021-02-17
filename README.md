# hydraulic-erosion

A online interactive editor for generating realistic terrain with procedural weathering and erosion iterative simulation. 

Interactive Editor
Export as .obj and .png height maps

[Current Design Documents](https://docs.google.com/document/d/1hJRhi_4-uAbqBfdUcYZhbcMNVP9Pfz1o3PNYXmdTL0M/edit?usp=sharing)


## Export Results
The following heightmap (512x512) was exported to Blender and rendered with Eevee
![output result to blender](https://i.imgur.com/tBGFZuf.png)
from the original heightmap generated at 512x512 at 300k iterations.\
![raw heightmap](https://i.imgur.com/ukNUbmf.png)

[Full Size Image](https://i.imgur.com/drLibPI.jpg)

## Command line tool
The simulator comes in a command line tool format as well in addition to the online interface. Usage:
```
$ erosion-sim

Command line tool for generating realistic terrain heightmap through a iterative erosion simulation. Program takes in either a pre-existing heightmap in .png format or can generate a new heightmap using OpenSimplex noise, and can output in .png .obj, and .stl file format for importing to other software. 

Copyright (c) 2021 Henry Jiang 
MIT LICENSE

USAGE: erosion-sim [options]
	--help
	INPUTS:
		If input image is not specified, then noise parameters must be passed in. If arguments are missing, then the default values will be used.
		-i <imagefile>          Pre-existing heightmap. Must be square. 
		--default_random        Use default noise settings with a random seed
		--seed seed             Sets the noise seed
		--octaves octaves       Sets how many layers of noise there will be
		--persistence persist   Sets how much each layer's contribution
		--scale scale           Sets the inital noise scale
		--height height         Sets the inital height
	SIM PARAMS:	
		-n, --iterations iters  The number of iterations. Heavy impact. Recommends [ 5 * size ^ 2 ] iterations.
		If any arguments below is not supplied, then the default values are used.
		--use_default           Use the default erosion parameters
		--drop_radius radius    Specify the droplet radius. Heavy impact.
		--droplet_life time     How long the droplet lives for. Heavy impact.
		--inertia mass          Sets droplet's inertia
		--sediment_cap cap      Sets max carrying capacity
		--deposit_speed speed   Sets the deposit speed
		--erode_speed speed     Sets the erosion speed
		--evaporate_speed speed Sets the evaporation speed
	OUTPUTS:
		-o <outputfile>         Specify the output filename(s)
		--quality size          Only for obj. Specify the output quality 
		--format={png|obj|stl}  Specify the export format
		--save-preview          Save the heightmap before erosion
```
