#include "Laser.h"

Laser::Laser()
{
    //ctor
}

Laser::~Laser()
{
    //dtor
}

void Laser::rotateLaser()
{
    if (!rotating) return;

    float unit_time = (r_time / r_duration);
    if (increasing) {
        r_time += gdata.m_timeDelta;
        if (unit_time >= 1) {
            unit_time = 1;
            increasing = false;
        }
    } else {
        r_time -= gdata.m_timeDelta;
        if (unit_time <= 0) {
            unit_time = 0;
            increasing = true;
        }
    }

    float new_angle = start_angle + (delta_angle * unit_time);
    rotated_laser = laser_dir;
    rotated_laser.rotate(new_angle);

}

void Laser::moveLaser()
{
    if (!m_moving) return;

    Vector2 delta;
    Vector2 dist_v = m_point1 - m_point2;
    float dist = dist_v.getMagnitude();

    delta = m_point2 - m_point1;
    delta.normalise();
    delta.setMagnitude(dist * (m_time / m_duration));

    if (m_dest == m_point1) // time is increasing
    {
        m_time += gdata.m_timeDelta;
        if (m_time >= m_duration)
        {
            m_time = m_duration - (m_time - m_duration);
            m_dest = m_point2;
        }
    }
    else if (m_dest == m_point2)
    {
        m_time -= gdata.m_timeDelta;
        if (m_time <= 0)
        {
            m_time = 0 - m_time;
            m_dest = m_point1;
        }
    }

    laserPos.set(m_point1 + delta);
}

void Laser::raycast()
{
    b2Vec2 p1 = laserPos.toBulletVector();
	float rayLength = 50;

	b2RayCastInput	input;
	input.p1 = p1;
	input.p2 = b2Vec2(p1.x + rotated_laser.x, p1.y + rotated_laser.y);
	input.maxFraction = rayLength;

	float closest_fraction = rayLength;
	b2Body* body = nullptr;
	for (b2Body* b = gdata.world->GetBodyList(); b; b = b->GetNext())
	{
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			b2RayCastOutput output;
			if (!f->RayCast(&output, input, 0)) continue;
			if (output.fraction < closest_fraction)
			{
				closest_fraction = output.fraction;
				body = b;
			}
		}

	}


	if (body->GetUserData() != nullptr)
	{
		Object* p = static_cast<Object*>(body->GetUserData());

		if (body->GetUserData() != nullptr && p->m_type == PLAYER)
		{
			cout << "you died" << endl;
			gdata.reload = true;
		}
	}

	laser = rotated_laser;
	laser.setMagnitude(closest_fraction);
}


void Laser::onUpdate()
{
    rotated_laser = laser_dir;
    rotateLaser();
    moveLaser();
    raycast();
}

void Laser::onDraw()
{
    Vector2 s = gdata.toScreenPixels(laserPos.x,laserPos.y);
    Vector2 e = laserPos + laser;
    e = gdata.toScreenPixels(e);

    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(s.x,s.y),sf::Color(255,0,0,255)),
        sf::Vertex(sf::Vector2f(e.x,e.y),sf::Color(255,0,0,255))
        };

    gdata.window->draw(line,2,sf::Lines);

}






