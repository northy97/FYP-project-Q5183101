
#include "Physics.h"
#include <vector>
#include "Particle.h"

void Physics::updateForces(std::vector<Particle>& particles)
{
	for (std::vector<Particle>::size_type i = 0; i < particles.size(); i++) { //simple gravity
		particles[i].acceleration += glm::vec3(0.0, -1, 0.0);
	}
}
