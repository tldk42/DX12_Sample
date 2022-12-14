#include "MathHelper.h"
#include <float.h>
#include <cmath>

using namespace DirectX;

const float MathHelper::Infinity = FLT_MAX;
const float MathHelper::Pi = 3.1415926535f;

float MathHelper::AngleFromXY(float x, float y)
{
    float theta = 0.f;

    if (x >= 0.f)
    {
        theta = atanf(y / x);

        if (theta < 0.f)
            theta += 2.f * Pi;
    }
    else
        theta = atanf(y / x) + Pi;
    return theta;
}

DirectX::XMVECTOR MathHelper::RandUnitVec3()
{
    XMVECTOR One = XMVectorSet(1.f, 1.f, 1.f, 1.f);
    XMVECTOR Zero = XMVectorZero();

    while (true)
    {
        XMVECTOR v = XMVectorSet(MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), 0.0f);

        if (XMVector3Greater(XMVector3LengthSq(v), One))
            continue;
        return XMVector3Normalize(v);
    }
}

DirectX::XMVECTOR MathHelper::RandHemisphereUnitVec3(DirectX::XMVECTOR n)
{
    XMVECTOR One = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
    XMVECTOR Zero = XMVectorZero();

    // Keep trying until we get a point on/in the hemisphere.
    while (true)
    {
        // Generate random point in the cube [-1,1]^3.
        XMVECTOR v = XMVectorSet(MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), 0.0f);

        // Ignore points outside the unit sphere in order to get an even distribution 
        // over the unit sphere.  Otherwise points will clump more on the sphere near 
        // the corners of the cube.

        if (XMVector3Greater(XMVector3LengthSq(v), One))
            continue;

        // Ignore points in the bottom hemisphere.
        if (XMVector3Less(XMVector3Dot(n, v), Zero))
            continue;

        return XMVector3Normalize(v);
    }
}
