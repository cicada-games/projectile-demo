#ifndef VEC_H
#define VEC_H
#include <math.h>


typedef struct {
    int x, y;
} Vec2_i;

typedef struct {
    float x, y, z;
} Vec3_f;

typedef struct {
    float x, y;
} Vec2_f;


Vec3_f add_vec3_f( Vec3_f a, Vec3_f b ) {
    Vec3_f result = {
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    };
    return result;
}

Vec3_f sub_vec3_f( Vec3_f a, Vec3_f b ) {
    Vec3_f result = {
        a.x - b.x,
        a.y - b.y,
        a.z - b.z
    };
    return result;
}


Vec3_f scalar_mult_vec3_f( Vec3_f a, float b) {
    Vec3_f result = {
        a.x * b,
        a.y * b,
        a.z * b
    };
    return result;
}

Vec3_f point_on_ray( Vec3_f origin, Vec3_f viewport_point, float t) {
    Vec3_f result, dir;
    dir = sub_vec3_f( viewport_point, origin );
    result = add_vec3_f( origin, scalar_mult_vec3_f( dir, t ) );
    return result;

}

int vec_3_f_equal( Vec3_f a, Vec3_f b ) {
    int result = a.x == b.x && a.y == b.y && a.z == b.z;
    return  result;

}

float dot_product_vec3_f( Vec3_f a, Vec3_f b ) {
    float  result = 0.0f;
    result += a.x * b.x;
    result += a.y * b.y;
    result += a.z * b.z;
    return result;
}

float mag_vec3_f( Vec3_f v ) {
    float result;
    result = sqrtf( dot_product_vec3_f(v, v) );
    return result;
}

float mag_no_root_vec_3_f( Vec3_f v ) {
    float result;
    result = dot_product_vec3_f(v, v);
    return result;
}


Vec2_f sub_vec2_f( Vec2_f a, Vec2_f b ) {
    Vec2_f result = {
        a.x - b.x,
        a.y - b.y,
    };
    return result;
}

float dot_product_vec2_f( Vec2_f a, Vec2_f b ) {
    float  result = 0.0f;
    result += a.x * b.x;
    result += a.y * b.y;
    return result;
}

float mag_vec2_f( Vec2_f v ) {
    return sqrtf( dot_product_vec2_f( v, v ) );
}

Vec2_f unit_vec2_f( Vec2_f v ) {
    Vec2_f unit_vec;

    unit_vec.x = v.x / mag_vec2_f( v );
    unit_vec.y = v.y / mag_vec2_f( v );
    
    return unit_vec;
}

Vec2_f add_vec2_f( Vec2_f v, Vec2_f u ) {
    Vec2_f result;
    result.x = v.x + u.x;
    result.y = v.y + u.y;

    return result;
}

#endif