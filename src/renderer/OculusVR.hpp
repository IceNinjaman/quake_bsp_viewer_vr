#ifndef OCULUSVR_INCLUDED
#define OCULUSVR_INCLUDED

#include "InputHandlers.hpp"
#include "renderer/OpenGL.hpp"
#include "renderer/OculusVRDebug.hpp"
#include "renderer/OVRCameraFrustum.hpp"
#include "Extras/OVR_Math.h"
#include "OVR_CAPI_0_7_0.h"

/*
 * Oculus Rift DK2 setup class (as of SDK 0.7.0.0)
 */
class OculusVR
{
public:
    OculusVR() : m_hmd(nullptr),
                 m_debugData(nullptr),
                 m_cameraFrustum(nullptr),
                 m_msaaEnabled(false)
    {
    }

    ~OculusVR();
    bool  InitVR();
    bool  InitVRBuffers(int windowWidth, int windowHeight);
    bool  InitNonDistortMirror(int windowWidth, int windowHeight); // create non-distorted mirror if necessary (debug purposes)
    void  DestroyVR();
    const ovrSizei GetResolution() const;
    void  OnRenderStart();
    const OVR::Matrix4f OnEyeRender(int eyeIndex);
    void  OnEyeRenderFinish(int eyeIndex);
    const OVR::Matrix4f GetEyeMVPMatrix(int eyeIdx) const;
    void  SubmitFrame();

    void  BlitMirror(ovrEyeType numEyes=ovrEye_Count, int offset = 0);   // regular OculusVR mirror view
    void  OnNonDistortMirrorStart();        // non-distorted mirror rendering start (debug purposes)
    void  BlitNonDistortMirror(int offset); // non-distorted mirror rendering (debug purposes)

    void  OnKeyPress(KeyCode key);
    void  CreateDebug();
    void  UpdateDebug();
    void  RenderDebug();
    void  RenderTrackerFrustum();   
    bool  IsDebugHMD() const { return (m_hmdDesc.AvailableHmdCaps & ovrHmdCap_DebugDevice) != 0; }
    bool  IsDK2() const { return m_hmdDesc.Type == ovrHmd_DK2; }
    void  ShowPerfStats(ovrPerfHudMode statsMode);
    void  SetMSAA(bool val) { m_msaaEnabled = val; }
    bool  MSAAEnabled() const { return m_msaaEnabled; }
private:
    // A buffer struct used to store eye textures and framebuffers.
    // We create one instance for the left eye, one for the right eye.
    // Final rendering is done via blitting two separate frame buffers into one render target.
    struct OVRBuffer
    {  
        OVRBuffer(const ovrHmd &hmd, int eyeIdx);
        void OnRender();
        void OnRenderFinish();
        void SetupMSAA(); 
        void OnRenderMSAA();
        void OnRenderMSAAFinish();
        void Destroy(const ovrHmd &hmd);

        ovrTexture m_eyeTexture;
        ovrSizei   m_eyeTextureSize;
        GLuint     m_eyeFbo      = 0;
        GLuint     m_depthBuffer = 0;

        GLuint m_msaaEyeFbo   = 0;   // framebuffer for MSAA texture
        GLuint m_eyeTexMSAA   = 0;   // color texture for MSAA
        GLuint m_depthTexMSAA = 0;   // depth texture for MSAA

        ovrSwapTextureSet *m_swapTextureSet = nullptr;
    };

    // data and buffers used to render to HMD
    ovrHmd            m_hmd;
    ovrHmdDesc        m_hmdDesc;
    ovrEyeRenderDesc  m_eyeRenderDesc[ovrEye_Count];
    ovrPosef          m_eyeRenderPose[ovrEye_Count];
    ovrVector3f       m_hmdToEyeViewOffset[ovrEye_Count];
    OVRBuffer        *m_eyeBuffers[ovrEye_Count];

    OVR::Matrix4f     m_projectionMatrix[ovrEye_Count];
    OVR::Matrix4f     m_eyeViewOffset[ovrEye_Count];
    OVR::Matrix4f     m_eyeOrientation[ovrEye_Count];
    OVR::Matrix4f     m_eyePose[ovrEye_Count];

    // frame timing data and tracking info
    ovrFrameTiming    m_frameTiming;
    ovrTrackingState  m_trackingState;

    // mirror texture used to render HMD view to OpenGL window
    ovrGLTexture     *m_mirrorTexture;

    // debug non-distorted mirror texture data
    GLuint            m_nonDistortTexture;
    GLuint            m_nonDistortDepthBuffer;
    GLuint            m_mirrorFBO;
    GLuint            m_nonDistortFBO;
    int               m_nonDistortViewPortWidth;
    int               m_nonDistortViewPortHeight;
    bool              m_msaaEnabled;

    // debug hardware output data
    OculusVRDebug    *m_debugData;

    // debug camera frustum renderer
    OVRCameraFrustum *m_cameraFrustum;
};


#endif