

#include <OVR_CAPI.h>
#include <iostream>

#ifndef NUPRESENCE_OVR_MANAGER
#define NUPRESENCE_OVR_MANAGER

class OVRManager {
public:
    OVRManager();
    ~OVRManager();

    void printCurrentPose();
	ovrSizei getResolution();
    
private:
	bool riftPresent;
    ovrSession session;
	ovrGraphicsLuid luid;
	ovrHmdDesc hmdDesc;

};

#endif