#pragma once
#include "RendererBase.hpp"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_constants.hpp>
namespace RenderCore
{
    struct FMatrix4X4
    {
        float m[4][4];

        FMatrix4X4() = default;
        FMatrix4X4(const FMatrix4X4 &) = default;
        FMatrix4X4 &operator=(const FMatrix4X4 &) = default;

        FMatrix4X4(FMatrix4X4 &&) = default;
        FMatrix4X4 &operator=(FMatrix4X4 &&) = default;

        static FMatrix4X4 &Identity()
        {
            return FMatrix4X4(1.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 1.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 1.0f, 0.0f,
                              0.0f, 0.0f, 0.0f, 1.0f);
        }
        void Transpose()
        {

        }
        FMatrix4X4(float (&mat)[4][4])
        {
           for(int i{0};i<4;i++)
            {
                for(int j{0};j<4;j++)
                {
                    m[i][j] = mat[i][j];
                }
            }
        }
        FMatrix4X4(float m00, float m01, float m02, float m03,
                             float m10, float m11, float m12, float m13,
                             float m20, float m21, float m22, float m23,
                             float m30, float m31, float m32, float m33) noexcept
        {
            m[0][0] = m00;
            m[0][1] = m01;
            m[0][2] = m02;
            m[0][3] = m03;
            m[1][0] = m10;
            m[1][1] = m11;
            m[1][2] = m12;
            m[1][3] = m13;
            m[2][0] = m20;
            m[2][1] = m21;
            m[2][2] = m22;
            m[2][3] = m23;
            m[3][0] = m30;
            m[3][1] = m31;
            m[3][2] = m32;
            m[3][3] = m33;
        }
        explicit FMatrix4X4(const float *pArray) noexcept;
        void mul(const FMatrix4X4& m2)
        {
            float result[4][4];
            for(size_t i{0};i<4;i++)
            {
                for(size_t j{0};j<4;j++)
                {
                    float r{0};
                    for (size_t o = 0; o < 4; o++)
                    {
                        r += m[i][o] * m2.m[o][j];
                    }
                    result[i][j] = r;
                }
            }
            for(size_t i{0};i<4;i++)
            {
                for(size_t j{0};j<4;j++)
                {
                    m[i][j] = result[i][j];
                }
            }
        }
        float operator()(size_t Row, size_t Column) const noexcept { return m[Row][Column]; }
        float &operator()(size_t Row, size_t Column) noexcept { return m[Row][Column]; }
    };

    struct FVector3f
    {
        float x, y, z;
        FVector3f() = default;
        FVector3f(const FVector3f &) = default;
        FVector3f &operator=(const FVector3f &) = default;

        FVector3f(FVector3f &&) = default;
        FVector3f &operator=(FVector3f &&) = default;

        constexpr FVector3f(float m0, float m1, float m2) noexcept : x(m0), y(m1), z(m2) {}
        // float &operator[](uint16_t num) const noexcept {return }
        float Dot(const FVector3f &dir) { return x * dir.x + y * dir.y + z * dir.z; }
        static float Dot(const FVector3f &dir1, const FVector3f &dir2)
        {
            return dir1.x * dir2.x + dir1.y * dir2.y + dir1.z * dir2.z;
        }
        const FVector3f &Cross(const FVector3f &dir)
        {
            return FVector3f(
                y * dir.z - z * dir.y,
                z * dir.x - x * dir.z,
                x * dir.y - y * dir.x);
        }
    };

    class RENDERCORE_DLL CameraBase
    {

    protected:
        float m_aspectRadio;
        float nearPlane;
        float farPlane;
        float Roll, Pitch, Yaw; /*Degrees*/
        float FOV;
        float aspectRadio;
        float WorldPosX,WorldPosY,WorldPosZ;
    public:
        CameraBase(float WorldPosX, float WorldPosY, float WorldPosZ, float Roll, float Pitch, float Yaw, float FOV, float aspectRadio, float nearPlane, float farPlane);
        CameraBase(const CameraBase &base) = default;
        virtual ~CameraBase() = default;
        virtual void MoveForward(float distance)=0;
        virtual void MoveSide(float distance)=0;
        virtual void MoveUp(float distance)=0;

        virtual void RotateYaw(float degree)=0;
        virtual void RotatePitch(float degree)=0;
        virtual void RotateRoll(float degree)=0;
        virtual void UpdateViewMatrix()=0;
        virtual void *UploadBuffer() = 0;

        virtual void Tick() = 0;
    };
};
