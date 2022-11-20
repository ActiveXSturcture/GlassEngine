#pragma once

class CameraBase
{
private:
    float m_aspectRadio;
    float nearPlane;
    float farPlane;
public:
    CameraBase(/* args */);
    ~CameraBase()=default;
    virtual float* GetWorldViewProjectMatrix()=0;
};

