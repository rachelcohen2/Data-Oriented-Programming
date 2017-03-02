// fluid simulation example
#include <sys/types.h>
#include <cmath>
#include <vector>
#include <stdlib.h>
#include <chrono> // for timing 
#include "Vec3.hpp"



// particle smoothing radius
const float radius = 0.005;

// size of simulation domain
const Vec3 domain = vec3(1.f, 1.f, 1.f);

// rest density and stiffness
const float baseDensity = 0.005f;

const float stiffness = 1.f;

// gravity strength and direction
const Vec3 gravity = vec3(0.f, 0.f, 1.f);


// number of particles to simulate
const uint32_t movingParticles = 1000000;


const Vec3 particleVector = vec3(0.f, 0.f, 0.f);



////////////////////////////////////
////class definitions


//grid has a list of grid cells
class Grid {

public:

  // uniform grid data structure given size of simulation domain and particle radius
  Grid();
  ~Grid();

  // give index into pid array for a particle
  uint32_t index(const Vec3 &pos);

  // give cell index in pid array given particle location
  uint32_t gridX, gridY, gridZ, gridSize; // grid dimensions

  uint32_t strideX, strideY, strideZ; // change to index for change in x/y/z


  // particle IDs within each grid cell
  // id[cell][particle_in_cell] = particle ID
  // id[cell] as fixed allocated array, particle in cell as std::vector
  std::vector<uint32_t> *id;

};


//rewritten to contain arrays of particle data
class Particles{

public:

  // initialize particle positions
  Particles();

  // put particles into grid to find neighbors
  void findNeighbors();

  // compute local density for each particle
  void computeDensity();

  // compute local pressure for each particle
  void computePressure();

  // force due to gravity and pressure differences
  void computeForces();

  // update position and velocity for timestep of length dt
  void updatePose(float dt);

  // simulate one timestep of length dt
  void simulate(float dt);

  //number of particles
  //can no longer return particle.size(), must return variable value
  int size() { return particleSize; }


private:

  //since getting rid of Particle class 
  void newParticle(const Vec3 &pos);

  //to evaluate number of particles
  int particleSize = 0;

  //re-written to contain arrays of particle data, elements are base data structure 
  //data elements from class Particle are now implemented here  
  std::vector<float> density;               

  std::vector<float> pressure; 

  std::vector<Vec3> force;

  std::vector<Vec3> velocity;

  std::vector<Vec3> position;

  // uniform grid of particles, used to find neighbors
  Grid grid;

};


//create a new particle to replace the Particle class with zero velocity
//replaces previous Particle::Particle implementation
void Particles::newParticle(const Vec3 &pos){

  //these are type float 
  density.push_back(0);
  pressure.push_back(0);

  //these are type Vec3
  force.push_back(particleVector);
  velocity.push_back(particleVector);
  position.push_back(pos);

  particleSize++;

}



// smoothing weight between point p1 and point p2
float weight(const Vec3 &p1, const Vec3 &p2) {

  float dist = length(p2-p1);             // distance from p1 to p2

  float t = fmin(0.f, 1.f - dist/radius); // distance scaled by smoothing radius

  return t*t*t;               // smoothing kernel = (1-dist/radius)^3

}


Vec3 grad(const Vec3 &p1, const Vec3 &p2) {

  Vec3 dir = p2 - p1;                     // direction from p1 to p2

  float dist = length(dir);               // distance between p1 and p2

  float t = fmin(0.f, 1.f - dist/radius); // distance scaled by smoothing radius

  return (-3*t*t) * normalize(dir); // gradient of kernel = -3*(1-dist/radius)^2 in radial direction

}



// allocate uniform grid data structure given size of simulation domain and particle radius
Grid::Grid()

  : gridX(ceil(domain.x/radius) + 2), // with border cells to make boundaries easier

    gridY(ceil(domain.y/radius) + 2),

    gridZ(ceil(domain.z/radius) + 2),

    gridSize(gridX*gridY*gridZ),

    strideX(gridY*gridZ), strideY(gridZ), strideZ(1)

{

  // auto-growth of std::vector does not work well nested
  // allocate grid once up front
  id = new std::vector<uint32_t>[gridSize];

}



// since grid was manually allocated, be sure to delete it
Grid::~Grid() {

  delete[] id;

}



// give index into pid array for a particle
uint32_t Grid::index(const Vec3 &pos) {

  // dice into grid, with one-cell border on each side
  Vec3 gridPos = pos/radius + 1.f;

  int x = int(gridPos.x), y = int(gridPos.y), z = int(gridPos.z);

  // don't return index in the border region
  // this is clamp(x, 1, gridX-2), etc.
  x = x<1 ? 1 : x>gridX-2 ? gridX-2 : x;

  y = y<1 ? 1 : y>gridY-2 ? gridY-2 : y;

  z = z<1 ? 1 : z>gridZ-2 ? gridZ-2 : z;

  // turn into cell index
  return x * strideX + y * strideY + z * strideZ;

}


//wherever Particle class was used, replace with newParticle private member function of Particles class 
//instead of particle.push_back(), use call to newParticle 
Particles::Particles(){

  for(int i = 0; i < movingParticles; i++){

    newParticle(vec3(drand48() * domain.x,

		     drand48() * domain.y,

		     drand48() * domain.z));

  }

  for(float x=0; x < domain.x; x += 0.5*baseDensity) {

    for(float y=0; y < domain.y; y += 0.5*baseDensity) {

      newParticle(vec3(x, y, 0));

      newParticle(vec3(x, y, domain.z));

    }

  }

  for(float y=0; y < domain.y; y += 0.5*baseDensity) {

    for(float z=0; z < domain.z; z += 0.5*baseDensity) {

      newParticle(vec3(0, y, z));

      newParticle(vec3(domain.x, y, z));

    }

  }

  for(float x=0; x < domain.x; x += 0.5*baseDensity) {

    for(float z=0; z < domain.z; z += 0.5*baseDensity) {

      newParticle(vec3(x, 0, z));

      newParticle(vec3(x, domain.y, z));

    }

  }

}



void Particles::findNeighbors(){

  // clear old grid data
  for(int i=0; i != grid.gridSize; ++i)
    grid.id[i].clear();

  // add each particle to its grid cell
  //no longer reference particle[i].position, just referenced position[i]
  for(int i=0; i != particleSize; ++i)

    grid.id[grid.index(position[i])].push_back(i);

}




void Particles::computeDensity(){

  //no longer reference particle.size(), used variable particleSize 
  for(int p = 0; p < particleSize; p++){
    uint32_t gridIndex = grid.index(position[p]);


    // start with a density of 0
    density[p] = 0.f;



    // for each neighboring grid cell
    for(int x=-1; x <= 1; ++x) {

      for(int y=-1; y <= 1; ++y) {

	for(int z=-1; z <= 1; ++z) {

	  uint32_t neighborCell = gridIndex + x*grid.strideX + y*grid.strideY + z*grid.strideZ;



	  // for each particle in that cell
	  for(int cellIdx=0; cellIdx != grid.id[neighborCell].size(); ++cellIdx) {

	    uint32_t n = grid.id[neighborCell][cellIdx];



	    // add local contribution to density
	    // this would be weighted by mass, but I simplified to mass=1
	    //no longer reference particle[n].position, just reference position[n]
	    density[p] += weight(position[n], position[p]);

	  }

	}

      }

    }

    // compute pressure from density
    pressure[p] = stiffness *(density[p] - baseDensity);

  }

}

void Particles::computeForces() {

  // only for moving particles
  for(int p = 0; p != movingParticles; p++) {

    //  particle[p].force = gravity;
    force[p] = gravity;

    // where is this particle in the grid?
    uint32_t gridIndex = grid.index(position[p]);



    // pressure force from neighbors
    for(int x=-1; x <= 1; ++x) {

      for(int y=-1; y <= 1; ++y) {

	for(int z=-1; z <= 1; ++z) {

	  uint32_t neighborCell = gridIndex + x*grid.strideX + y*grid.strideY + z*grid.strideZ;



	  // for each particle in that cell
	  for(int cellIdx=0; cellIdx != grid.id[neighborCell].size(); ++cellIdx) {

	    uint32_t n = grid.id[neighborCell][cellIdx];



                        force[p] = force[p] + 

			  grad(position[n], position[p]) * pressure[n] / density[p];

	  }

	}

      }

    }

  }

}

void Particles::updatePose(float dt) {

  for(int i = 0; i < movingParticles; i++){
    //no longer reference particle[i].velocity, just reference velocity[i], etc
    velocity[i] = velocity[i] + force[i] * dt;

    position[i] = position[i] + velocity[i] * dt;

  }

}

void Particles::simulate(float dt){

  // update neighbor lists
  findNeighbors();

  // find local density
  computeDensity();

  // forces due to gravity and pressure differences
  computeForces();

  // update position and velocity
  updatePose(dt);

}



int main(){


  Particles p;

  printf("One step fluid simulation with %d particles\n", p.size());

  auto startClock = std::chrono::high_resolution_clock::now();

  p.simulate(0.01);

  auto stopClock = std::chrono::high_resolution_clock::now();

  //calculate time from clock started to clock stopped 
  std::chrono::duration<double> clockDuration = stopClock - startClock;

  printf("SoA Time: %f seconds\n", clockDuration.count());

  return 0;

}
