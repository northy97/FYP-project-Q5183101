#include "fluid.hpp"



Fluid::Fluid( void ) {// initial amount of particles and position 
    for (float x = -particleRadius * 9; x <= particleRadius *9; x += particleDiameter) {
        for (float y = -particleRadius * 9; y <= particleRadius * 9; y += particleDiameter) {
            for (float z = -particleRadius * 9; z <= particleRadius * 9; z += particleDiameter)

                m_Particles.push_back(Particle(mass, Vector3f(x, y, z)));
        }
    }
}

void Fluid::drawScene( void ) {
    float sphereRadius = powf((3 * mass) / (4 * M_PI * restDensity), 1.0f / 3.0f);
    for (int i = 0; i < m_Particles.size(); i++) {
        glPushMatrix();
        glTranslatef(m_Particles[i].mPos.x, m_Particles[i].mPos.y, m_Particles[i].mPos.z);
        glutSolidSphere(sphereRadius, 30, 30);// render each particle of fluid
        glPopMatrix();
    }

    glDisable(GL_LIGHTING);

    

    // Draw bottom surface edges of boundary
    glBegin(GL_LINE_LOOP);
    glVertex3f(-containerSize / 2, -containerSize / 2, -containerSize / 2);
    glVertex3f(containerSize / 2, -containerSize / 2, -containerSize / 2);
    glVertex3f(containerSize / 2, -containerSize / 2, containerSize / 2);
    glVertex3f(-containerSize / 2, -containerSize / 2, containerSize / 2);
    
    glEnd();

    glColor3f(1.0f, 0.0f, 0.0f);

 
    glEnable(GL_LIGHTING);

}

void Fluid::runSimulation( void ) {
    // Calculate pressure and density 

    
    for (int i = 0; i < m_Particles.size(); i++) {
        m_Particles[i].mDensity  = calcDensity(m_Particles[i].mPos);
        m_Particles[i].mPressure = calcPressure(m_Particles[i].mDensity);
    }

    // Compute internal forces
    for (int i = 0; i < m_Particles.size(); i++) {
        m_Particles[i].mPressureForce  = calcPressureForce(i, m_Particles[i].mDensity, m_Particles[i].mPressure, m_Particles[i].mPos);
 
        m_Particles[i].mViscoForce = calcViscosityForce(i, m_Particles[i].mVeloc, m_Particles[i].mPos);
    }

    // Compute external forces
    for (int i = 0; i < m_Particles.size(); i++) {
        m_Particles[i].mGravitationalForce = calcGravitationalForce(m_Particles[i].mDensity);
        m_Particles[i].mSurfaceNormal      = calcSurfaceNormal(m_Particles[i].mPos);
        if (m_Particles[i].mSurfaceNormal.length() >= thresholdBoundary)
            m_Particles[i].mSurfaceTensionForce = calcSurfaceTensionForce(m_Particles[i].mSurfaceNormal, m_Particles[i].mPos);
        else
            m_Particles[i].mSurfaceTensionForce = Vector3f(0.0f, 0.0f, 0.0f);
    }

    // Time integration and collision handling
    static float time = 0.0f;
    time += TimeStep;
    Vector3f totalForce;
    for (int i = 0; i < m_Particles.size(); i++) {
    
        totalForce = m_Particles[i].mPressureForce + m_Particles[i].mViscoForce + m_Particles[i].mGravitationalForce + m_Particles[i].mSurfaceTensionForce;
        employEulerIntegrator(m_Particles[i], totalForce);

        Vector3f contactPoint;
        Vector3f unitSurfaceNormal;
        if (collisionCheck(m_Particles[i], contactPoint, unitSurfaceNormal)) {
            velocityUpdate(m_Particles[i].mVeloc, unitSurfaceNormal, (m_Particles[i].mPos - contactPoint).length());
            m_Particles[i].mPos = contactPoint;
        }
    }
}


float Fluid::calcPressure( float density ) {
    return gasStiffness * (density - restDensity);
}

float Fluid::calcDensity(Vector3f position) {
    float sum = 0.0f;
    for (int j = 0; j < m_Particles.size(); j++)
        sum += m_Particles[j].mMass * useKernal(position - m_Particles[j].mPos, SUPPORT_RADIUS);
    return sum;
}

Vector3f Fluid::calcPressureForce( int indexOfCurrentParticle, float density, float pressure, Vector3f position ) {
    Vector3f sum(0.0f, 0.0f, 0.0f);
    for (int j = 0; j < m_Particles.size(); j++) {
        if (j == indexOfCurrentParticle)
            continue;
        sum += pressureKernelGradiant(position - m_Particles[j].mPos, SUPPORT_RADIUS) * (pressure / (density * density) + m_Particles[j].mPressure / 
            (m_Particles[j].mDensity * m_Particles[j].mDensity)) * m_Particles[j].mMass;
    }
    return -(sum * density);
}


Vector3f Fluid::calcGravitationalForce( float density )
{
    return gravitationalConstant * density;
}

Vector3f Fluid::calcSurfaceNormal( Vector3f position ) 
{
    Vector3f sum(0.0f, 0.0f, 0.0f);
    for (int j = 0; j < m_Particles.size(); j++)
    {
        sum += DefaultKernalGradiant(position - m_Particles[j].mPos, SUPPORT_RADIUS) * (m_Particles[j].mMass / m_Particles[j].mDensity);
    }
        
    return sum;
}

Vector3f Fluid::calcSurfaceTensionForce( Vector3f surfaceNormal, Vector3f position ) {
    float sum = 0.0f;
    for (int j = 0; j < m_Particles.size(); j++)
        sum += (m_Particles[j].mMass / m_Particles[j].mDensity) * defaultKernelLap(position - m_Particles[j].mPos, SUPPORT_RADIUS);
    return -(surfaceNormal.normalize() * surfaceTension * sum);
}

Vector3f Fluid::calcViscosityForce(int indexOfCurrentParticle, Vector3f velocity, Vector3f position) {
    Vector3f sum(0.0f, 0.0f, 0.0f);
    for (int j = 0; j < m_Particles.size(); j++) {
        if (j == indexOfCurrentParticle)
            continue;
        sum += (m_Particles[j].mVeloc - velocity) * (m_Particles[j].mMass / m_Particles[j].mDensity) * viscosKernalLap(position - m_Particles[j].mPos, SUPPORT_RADIUS);
    }
    return sum * viscosity;
}

// used to update the position and velocity by the timestep 
void Fluid::employEulerIntegrator( Particle &particle, Vector3f totalForce ) {
    particle.mAccel = totalForce / particle.mDensity;
    particle.mVeloc = particle.mVeloc + particle.mAccel * TimeStep;
    particle.mPos = particle.mPos + particle.mVeloc * TimeStep;
}


// run collision check
bool Fluid::collisionCheck( Particle particle, Vector3f &contactPoint, Vector3f &unitSurfaceNormal ) {
    if (abs(particle.mPos.x) <= containerSize / 2 && abs(particle.mPos.y) <= containerSize / 2 && abs(particle.mPos.z) <= containerSize / 2)
        return false;

    char maxComponent = 'x';
    float maxValue    = abs(particle.mPos.x);

    if (maxValue < abs(particle.mPos.y))
    {
        maxComponent = 'y';
        maxValue     = abs(particle.mPos.y);
    }

    if (maxValue < abs(particle.mPos.z))
    {
        maxComponent = 'z';
        maxValue     = abs(particle.mPos.z);
    }
  
    // generate basic collisions for the box boundaries using the current position with the greatest absolute value.
    switch (maxComponent) {
        

        case 'y':
            if (particle.mPos.y < -containerSize / 2) 
            {
                contactPoint = particle.mPos;            contactPoint.y = -containerSize / 2;
                if (particle.mPos.x < -containerSize / 2)     contactPoint.x = -containerSize / 2;
                else if (particle.mPos.x > containerSize / 2) contactPoint.x =  containerSize / 2;
                if (particle.mPos.z < -containerSize / 2)     contactPoint.z = -containerSize / 2;
                else if (particle.mPos.z > containerSize / 2) contactPoint.z =  containerSize / 2;
                unitSurfaceNormal = Vector3f( 0.0f,  1.0f,  0.0f);
            }
            else if (particle.mPos.y > containerSize / 2) 
            {
                contactPoint = particle.mPos;            contactPoint.y =  containerSize / 2;
                if (particle.mPos.x < -containerSize / 2)     contactPoint.x = -containerSize / 2;
                else if (particle.mPos.x > containerSize / 2) contactPoint.x =  containerSize / 2;
                if (particle.mPos.z < -containerSize / 2)     contactPoint.z = -containerSize / 2;
                else if (particle.mPos.z > containerSize / 2) contactPoint.z =  containerSize / 2;
                unitSurfaceNormal = Vector3f( 0.0f, -1.0f,  0.0f);
            }
            break;

        case 'z':
            if (particle.mPos.z < -containerSize / 2)
            {
                contactPoint = particle.mPos;            contactPoint.z = -containerSize / 2;
                if (particle.mPos.x < -containerSize / 2)     contactPoint.x = -containerSize / 2;
                else if (particle.mPos.x > containerSize / 2) contactPoint.x = containerSize / 2;
                if (particle.mPos.y < -containerSize / 2)     contactPoint.y = -containerSize / 2;
                else if (particle.mPos.y > containerSize / 2) contactPoint.y = containerSize / 2;
                unitSurfaceNormal = Vector3f(0.0f, 0.0f, 1.0f);
            }

            else if (particle.mPos.z > containerSize / 2)
            {
                contactPoint = particle.mPos;            contactPoint.z = containerSize / 2;
                if (particle.mPos.x < -containerSize / 2)     contactPoint.x = -containerSize / 2;
                else if (particle.mPos.x > containerSize / 2) contactPoint.x = containerSize / 2;
                if (particle.mPos.y < -containerSize / 2)     contactPoint.y = -containerSize / 2;
                else if (particle.mPos.y > containerSize / 2) contactPoint.y = containerSize / 2;
                unitSurfaceNormal = Vector3f(0.0f, 0.0f, -1.0f);
            }
            break;
        case 'x':
            if (particle.mPos.x < -containerSize / 2) {
                contactPoint = particle.mPos;            contactPoint.x = -containerSize / 2;
                if (particle.mPos.y < -containerSize / 2)     contactPoint.y = -containerSize / 2;
                else if (particle.mPos.y > containerSize / 2) contactPoint.y = containerSize / 2;
                if (particle.mPos.z < -containerSize / 2)     contactPoint.z = -containerSize / 2;
                else if (particle.mPos.z > containerSize / 2) contactPoint.z = containerSize / 2;
                unitSurfaceNormal = Vector3f(1.0f, 0.0f, 0.0f);
            }
            else if (particle.mPos.x > containerSize / 2) {
                contactPoint = particle.mPos;        contactPoint.x = containerSize / 2;
                if (particle.mPos.y < -containerSize / 2)     contactPoint.y = -containerSize / 2;
                else if (particle.mPos.y > containerSize / 2) contactPoint.y = containerSize / 2;
                if (particle.mPos.z < -containerSize / 2)     contactPoint.z = -containerSize / 2;
                else if (particle.mPos.z > containerSize / 2) contactPoint.z = containerSize / 2;
                unitSurfaceNormal = Vector3f(-1.0f, 0.0f, 0.0f);
            }
            break;
       
    }
    return true;
}

// collision impulse 
void Fluid::velocityUpdate( Vector3f &velocity, Vector3f unitSurfaceNormal, float penetrationDepth ) {
    velocity = velocity - unitSurfaceNormal * (1 + restitution * penetrationDepth / (TimeStep * velocity.length())) * velocity.dot(unitSurfaceNormal);
}



// this exntire next section is  just the functions to generate the smoothing kernels 
Vector3f Fluid::DefaultKernalGradiant( Vector3f distVector, float supportRadius ) {
    float dist = distVector.length();
    if (dist > supportRadius)
        return Vector3f(0.0f, 0.0f, 0.0f);
    else
        return -(distVector * (945 / (32 * M_PI * powf(supportRadius, 9.0f))) * powf(supportRadius * supportRadius - dist * dist, 2.0f));// is used to calculate smothing gradiant
}

float Fluid::defaultKernelLap( Vector3f distVector, float supportRadius ) {
    float dist = distVector.length();
    if (dist > supportRadius)
        return 0.0f;
    else
        return -(945 / (32 * M_PI * powf(supportRadius, 9.0f))) * (supportRadius * supportRadius - dist * dist) * (3 * supportRadius * supportRadius - 7 * dist * dist);
}

Vector3f Fluid::pressureKernelGradiant( Vector3f distVector, float supportRadius ) {
    float dist = distVector.length();
    if (dist > supportRadius)
        return Vector3f(0.0f, 0.0f, 0.0f);
    else if (dist < 10e-5) 
        return -(Vector3f(1.0f, 1.0f, 1.0f).normalize() * (45 / (M_PI * powf(supportRadius, 6.0f))) * powf(supportRadius - dist, 2.0f));
    else
        return -(distVector.normalize() * (45 / (M_PI * powf(supportRadius, 6.0f))) * powf(supportRadius - dist, 2.0f));
}


float Fluid::viscosKernalLap( Vector3f distVector, float supportRadius ) {
    float dist = distVector.length();
    if (dist > supportRadius)
        return 0.0f;
    else
        return (45 / (M_PI * powf(supportRadius, 6.0f))) * (supportRadius - dist);
}

float Fluid::useKernal(Vector3f distVector, float supportRadius) {
    float dist = distVector.length();
    if (dist > supportRadius)
        return 0.0f;
    else
        return (315 / (64 * M_PI * powf(supportRadius, 9.0f))) * powf(supportRadius * supportRadius - dist * dist, 3.0f);
}