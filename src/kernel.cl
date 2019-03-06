typedef struct body {
    float x;
    float y;
    float speed_x;
    float speed_y;
    float mass;
} s_body;


//__global s_body **bodies
//__kernel void SAXPY (__global s_body *bodies)
////__kernel void SAXPY ()
//{
//    int idx = get_global_id (0);
////    printf("%d\n", idx);
//    bodies[idx].x = bodies[idx].x + 1;
////    bodies[idx].y = 0;
////    printf("Le kernel marche !\n");
//}

//const double s_soft = 1000000;
//const float gravity = 0.66742;


__kernel void SAXPY (__global s_body *bodies)
{
    int idx = get_global_id (0);
//    bodies[idx].x = bodies[idx].x + 1;
    int i = 0;
    while (i < 10000) {
            float dx = bodies[idx].x - bodies[i].x;
            float dy = bodies[idx].y - bodies[i].y;
            double r = sqrt((dx) * (dx) +
                            (dy) * (dy) + 10000000);
            if (r > 0) {
                double k = 0.066742 * bodies[i].mass / (r * r * r);
                bodies[idx].speed_x += k * (bodies[i].x - bodies[idx].x);
                bodies[idx].speed_y += k * (bodies[i].y - bodies[idx].y);
            }

        ++i;

    }
}

