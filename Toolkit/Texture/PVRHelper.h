/*
 * READING (NON LOSSY) PVR FILES (MADE OF A BASIC HEADER FOLLOWED BY THE RAW TEXTURE-DATA):
 * - PIXEL TYPES SUPPORTED: RGBA_8888, RGBA_4444, RGB_888, RGB_565, ALPHA_8
 * - NOT SUPPORTING NON-POWER-OF-TWO TEXTURES
 * - NOT SUPPORTING EMBEDDED MIPMAP-LEVELS
 * - GZIP-COMPRESSED: WE USE ZLIB DIRECTLY TO DECOMPRESS THE DATA IN AN EFFICIENT MANNER, SO IT WILL WORK ONLY ON PURE FILE-SYSTEMS (OSX, IOS)
 * - RAW: SHOULD WORK WITH WINDOWS AND ANDROID
 *
 * ADVANTAGES:
 * - FASTER AND MORE EFFICIENT THAN PNG OR TIFF
 * - ALLOWS FOR 16-BIT TEXTURES OR ALPHA TEXTURES
 * - NO ISSUES WITH ALPHA-PREMULTIPLICATION ON IOS
 *
 * REGARDING RAW PVR FILES 
 * - PROS: READING A RAW PVR FILE SHOULD ALWAYS BE THE FASTEST AND MOST-EFFICIENT CHOICE
 * - PROS: ON IOS AND ANDROID, THE APPLICATION-SIZE SHOULD BE THE SAME (BECAUSE THE DATA IN THE APPLICATION-PACKAGE IS ZIPPED)
 * - CONS: BIG APPLICATION SIZES ON OSX AND WINDOWS (BECAUSE THE DATA IN THE APPLICATION-PACKAGE IS NOT ZIPPED)
 * - CONS: BIG "UNPACKAGED APPLICATION-SIZE" ON IOS (THE SIZE TAKEN ONCE THE APPLICATION-PACKAGE IS UNZIPPED)
 * - RECAP: SEEMS TO BE THE IDEAL FORMAT FOR ANDROID, AND EVENTUALLY FOR IOS (IF LOADING-TIME IS MORE IMPORTANT THAN "UNPACKAGED APPLICATION-SIZE")
 *
 * TODO: TEST ON ANDROID AND WINDOWS
 */

#pragma once

#include "cinder/Buffer.h"
#include "cinder/Filesystem.h"
#include "cinder/gl/Texture.h"

class PVRHelper
{
public:
    static ci::Buffer loadCompressedPVR(const ci::fs::path &filePath);
    static ci::gl::Texture* getPVRTexture(const ci::Buffer &buffer, bool useMipmap = false, GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE);
};
