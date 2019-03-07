typedef struct body {
    float x;
    float y;
    float speed_x;
    float speed_y;
    float mass;
} s_body;

//const double s_soft = 1000000;
//const float gravity = 0.66742;


#define SOFTENER 2000000
#define GRAVITY 0.67742

__kernel void compute_forces (__global s_body *bodies, int nb_bodies)
{
    int idx = get_global_id (0);
    int i = 0;
    while (i < nb_bodies) {
            float dx = bodies[idx].x - bodies[i].x;
            float dy = bodies[idx].y - bodies[i].y;
            double r = sqrt((dx) * (dx) +
                            (dy) * (dy) + SOFTENER);
            if (r > 0) {
                double k = GRAVITY * bodies[i].mass / (r * r * r) * 1.4;
                bodies[idx].speed_x += k * (bodies[i].x - bodies[idx].x);
                bodies[idx].speed_y += k * (bodies[i].y - bodies[idx].y);
            }
        ++i;

    }
    bodies[idx].x += bodies[idx].speed_x;
    bodies[idx].y += bodies[idx].speed_y;
}

