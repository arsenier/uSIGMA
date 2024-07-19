#pragma once

class Integrator
{
private:
    float I;
    float Ts;

public:
    const float &out;

    Integrator(float _Ts) : out(I)
    {
        I = 0;
        Ts = _Ts;
    }
    float tick(float in)
    {
        I += in * Ts;
        return I;
    }
};

class Aperiodic
{
private:
    Integrator I;
    float K, T;
    float Ts;

public:
    const float &out;

    Aperiodic(float _Ts, float _T = 1, float _K = 1)
        : I(_Ts), out(I.out)
    {
        Ts = _Ts;
        T = _T;
        K = _K;
    }
    float tick(float in)
    {
        const float inK = in * K;
        const float err = inK - out;
        const float out_i = err / T;
        I.tick(out_i);
        return out;
    }
};
