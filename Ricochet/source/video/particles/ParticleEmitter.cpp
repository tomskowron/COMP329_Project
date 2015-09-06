#include "ParticleEmitter.h"


void ParticleEmitter::update()
{
    accumelator += gdata.m_timeDelta;

    vector<IParticle*> live_particles;

    for (int i = 0; i < particles.size(); ++i)
    {
        IParticle* p = particles.at(i);
        if (p->particle_age >= p->particle_dur)
        {
            p->particle_destroy();
            delete p;
        }
        else
        {
            live_particles.push_back(p);
        }
    }
    particles.clear();
    particles = live_particles;

    for (int i = 0; i < particles.size(); ++i)
    {
        particles.at(i)->particle_update();
    }

    if (enabled)    createParticle();

}

void ParticleEmitter::freeResources()
{
    for (int i = 0; i < particles.size(); ++i)
    {
        particles.at(i)->particle_destroy();
        delete particles.at(i);
    }

    particles.clear();
}

bool ParticleEmitter::condition(IParticle* p)
{
    bool del = (p->particle_age >= p->particle_dur);
    if (del)
    {
        p->particle_destroy();
    }
    return del;
}

void ParticleEmitter::drawParticles()
{
    for (int i = 0; i < particles.size(); ++i)
    {
        particles.at(i)->particle_draw();
    }
}

