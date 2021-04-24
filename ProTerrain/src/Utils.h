#pragma once

static float smoothstep(const float& t)
{
    return t * t * (3 - 2 * t);
}

static float quintic(const float& t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

static float smoothstepDeriv(const float& t)
{
    return t * (6 - 6 * t);
}

static float quinticDeriv(const float& t)
{
    return 30 * t * t * (t * (t - 2) + 1);
}

static float lerp(const float& lo, const float& hi, const float& t)
{
    return lo * (1 - t) + hi * t;
}
