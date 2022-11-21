#pragma once
#include "RendererBase.hpp"
namespace RenderCore
{
    struct FMatrix4X4
    {
        union
        {
            struct
            {
                float _11, _12, _13, _14;
                float _21, _22, _23, _24;
                float _31, _32, _33, _34;
                float _41, _42, _43, _44;
            };
            float m[4][4];
        };

        FMatrix4X4() = default;
        FMatrix4X4(const FMatrix4X4 &) = default;
        FMatrix4X4 &operator=(const FMatrix4X4 &) = default;

        FMatrix4X4(FMatrix4X4 &&) = default;
        FMatrix4X4 &operator=(FMatrix4X4 &&) = default;

        static FMatrix4X4& Identity(){
            return FMatrix4X4(1.0f,0.0f,0.0f,0.0f,
                                0.0f,1.0f,0.0f,0.0f,
                                0.0f,0.0f,1.0f,0.0f,
                                0.0f,0.0f,0.0f,1.0f);
        }

        constexpr FMatrix4X4(float m00, float m01, float m02, float m03,
                             float m10, float m11, float m12, float m13,
                             float m20, float m21, float m22, float m23,
                             float m30, float m31, float m32, float m33) noexcept
            : _11(m00), _12(m01), _13(m02), _14(m03),
              _21(m10), _22(m11), _23(m12), _24(m13),
              _31(m20), _32(m21), _33(m22), _34(m23),
              _41(m30), _42(m31), _43(m32), _44(m33) {}
        explicit FMatrix4X4(const float *pArray) noexcept;

        float operator()(size_t Row, size_t Column) const noexcept { return m[Row][Column]; }
        float &operator()(size_t Row, size_t Column) noexcept { return m[Row][Column]; }
    };

    class RENDERCORE_DLL CameraBase
    {

    protected:
        float m_aspectRadio;
        float nearPlane;
        float farPlane;
        float WorldPosX, WorldPosY, WorldPosZ;
        float Roll, Pitch, Yaw; /*Degrees*/
        FMatrix4X4 Transform, Rotate, Project;

    public:
        CameraBase(float WorldPosX, float WorldPosY, float WorldPosZ, float Roll, float Pitch, float Yaw);
        CameraBase(const CameraBase &base) = default;
        virtual ~CameraBase() = default;
        void BuildWorldViewProjectMatrix();
        void MoveForward(float distance);
        void MoveSide(float distance);
        void MoveUp(float distance);

        virtual void *UploadBuffer() = 0;

        virtual void Tick() = 0;
    };
};
