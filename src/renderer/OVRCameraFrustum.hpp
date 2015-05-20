#ifndef OVRCAMERAFRUSTUM_HPP
#define OVRCAMERAFRUSTUM_HPP
#include "renderer/OpenGL.hpp"
#include "OVR_CAPI_0_5_0.h"
#include "OVR_CAPI_GL.h"

/*
 * Rendering class for IR tracking camera bounds (frustum)
 */
class OVRCameraFrustum
{
public:
    ~OVRCameraFrustum();
    void Recalculate(ovrHmd hmd);
    void OnRender();
private:
    GLuint m_vertexArray;
    GLuint m_vertexBuffers[3];
};

#endif