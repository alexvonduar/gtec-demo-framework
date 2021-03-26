#!/bin/bash
set -o errexit

source $WORKSPACE/.Config/Jenkins/ubuntu/PrepareJenkinsEnvironment.sh

if [ ! -n "${FSL_CI_FEATURES+1}" ]; then
#export FSL_CI_FEATURES=[EarlyAccess,EGL,GoogleUnitTest,OpenCL1.2,OpenCV4,OpenGLES2,OpenGLES3,OpenGLES3.1,OpenGLES3.2,OpenVG,OpenVX1.1,Vulkan]
export FSL_CI_FEATURES=[EarlyAccess,EGL,OpenCV,OpenCV4,OpenGLES2,OpenGLES3,OpenGLES3.1,OpenVG,OpenVX1.1,Vulkan]
fi

FslBuildCheck.py -r -vv --BuildTime --UseFeatures $FSL_CI_FEATURES --tidy $FSL_CI_BUILD_PARAM
