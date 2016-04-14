#pragma once

#include <GL/OOGL.hpp>
#include <OVR_CAPI_GL.h>

static GL::Mat4 ovrToGL(const OVR::Matrix4f& M) {
	GL::Mat4 glview;
	memcpy(&(glview.m), &(M.M), 16 * sizeof(float));
	return glview.Transpose();
}