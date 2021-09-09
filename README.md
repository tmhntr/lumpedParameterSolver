# model
A program for constructing and solving 0D models, from modular components. 

Current model components installed include:
- an RC windkessel compartment
- a 4-chamber heart with varied elastance forcing term
- myogenic activation function
- myogenic flow function
- virtual physiological rat circulation and hemodynamics

Current solver options include:
- sundials linear dense solver 

coming soon:
- dialysis

to install and run:
enter the following lines into terminal

if you do not have sundials installed run:
brew upgrade
brew install sundials

to clone this git repository run:
git clone -b external-build https://github.com/tmhntr/model.git 

mkdir model/build
cd model/build
cmake .. && make && ./rat

