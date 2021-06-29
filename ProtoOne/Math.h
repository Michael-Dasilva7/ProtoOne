#ifndef MATH_H_
#define MATH_H_

#include <cmath>

const float PI = 3.14159265359f;

inline float Deg2Rad(float deg)
{
    return (PI / 180.f) * deg;
}

inline float Rad2Deg(float rad)
{
    return (180.f / PI) * rad;
}

inline int RoundToInt(float x)
{
    return (int)(x + 0.5f);
}


enum CardinalDir
{
    DIR_RIGHT,
    DIR_DOWN,
    DIR_LEFT,
    DIR_UP,
};

// Lookup table to convert CardinalDir to angular value in degrees.
// These values assume that y axis points down.
const float CardinalAngle[4]
{
    0.0f,       // DIR_RIGHT
    90.0f,      // DIR_DOWN
    180.0f,     // DIR_LEFT
    270.0f,     // DIR_UP
};


inline float StandardizeAngle(float angle)
{
    while (angle < -180.0f) {
        angle += 360.0f;
    }
    while (angle >= 180.0f) {
        angle -= 360.0f;
    }
    return angle;
}

inline float SignedAngleDiff(float srcAngle, float dstAngle)
{
    float diff = dstAngle - srcAngle;
    return StandardizeAngle(diff);      // return result in range [-180, 180)
}


struct Vec2 {
    float x, y;

    Vec2()
        : x(0), y(0)
    { }

    Vec2(float x, float y)
        : x(x), y(y)
    { }

    Vec2 operator+ (const Vec2& other) const
    {
        return Vec2(x + other.x, y + other.y);
    }

    Vec2 operator- (const Vec2& other) const
    {
        return Vec2(x - other.x, y - other.y);
    }

    Vec2 operator- () const
    {
        return Vec2(-x, -y);
    }

    Vec2 operator* (float s) const
    {
        return Vec2(x*s, y*s);
    }

    Vec2 operator/ (float s) const
    {
        return Vec2(x/s, y/s);
    }

    Vec2& operator+= (const Vec2& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vec2& operator-= (const Vec2& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vec2& operator*= (float s)
    {
        x *= s;
        y *= s;
        return *this;
    }

    Vec2& operator/= (float s)
    {
        x /= s;
        y /= s;
        return *this;
    }


    float Length() const
    {
        return std::sqrt(x*x + y*y);
    }

    float LengthSquared() const
    {
        return x*x + y*y;
    }

    void Normalize()
    {
        float len = Length();
        if (len > 0) {
            x /= len;
            y /= len;
        }
    }

    // return a normalized copy of this vector
    Vec2 Direction() const
    {
        Vec2 result(x, y);      // create copy
        result.Normalize();     // normalize copy
        return result;
    }
};

inline float Dist(const Vec2& u, const Vec2& v)
{
    return (u - v).Length();
}

//
// global overloated operator* for float multiplied by Vec2
//
inline Vec2 operator* (float s, const Vec2& v)
{
    return Vec2(s * v.x, s * v.y);
}

#endif
