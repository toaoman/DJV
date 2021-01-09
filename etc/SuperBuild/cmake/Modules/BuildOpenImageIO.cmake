include(ExternalProject)

ExternalProject_Add(
    OpenImageIO
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/OpenImageIO
    DEPENDS OpenCV OpenVDB OpenEXR TIFF JPEG PNG Boost FreeType TBB ZLIB
    GIT_REPOSITORY https://github.com/OpenImageIO/oiio.git
    GIT_TAG Release-2.2.9.0
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/OpenImageIO-patch/src/cmake/externalpackages.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/OpenImageIO/src/OpenImageIO/src/cmake/externalpackages.cmake
    CMAKE_ARGS
        -DCMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
        -DCMAKE_PREFIX_PATH=${CMAKE_INSTALL_PREFIX}
        -DCMAKE_POLICY_DEFAULT_CMP0091:STRING=NEW
        -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
        -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
        -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
        -DOIIO_BUILD_TOOLS=OFF
        -DOIIO_BUILD_TESTS=OFF
        -DBUILD_DOCS=OFF
        -DUSE_PYTHON=OFF)
