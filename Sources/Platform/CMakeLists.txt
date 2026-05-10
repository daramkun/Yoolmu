cmake_minimum_required(VERSION 4.0)

include(../../CMake/Yoolmu.cmake)

YoolmuProject(Yoolmu::Platform dll)
YoolmuProjectReference(Yoolmu::Platform Yoolmu::Core)
