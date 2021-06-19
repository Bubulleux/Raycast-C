#include "define.h"

t_vector vector_get_normal(t_vector vector)
{
    double magnetude = vector_get_magnetude(vector);
    t_vector vector_out;
    vector_out.x = vector.x / magnetude;
    vector_out.y = vector.y / magnetude;
    return vector_out;
}

double vector_get_magnetude(t_vector vector)
{
    return sqrt(pow(vector.x, 2) + pow(vector.y, 2));
}

t_vector new_vector(double x, double y)
{
    t_vector vector;
    vector.x = x;
    vector.y = y;
    return vector;
}