//
// Lol Engine
//
// Copyright: (c) 2004-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"
#include "image-private.h"

namespace lol
{

static vec4 u8tof32(u8vec4 pixel)
{
    //vec4 ret;
    //ret.r = pow((float)pixel.r / 255.f, global_gamma);
    //ret.g = pow((float)pixel.g / 255.f, global_gamma);
    //ret.b = pow((float)pixel.b / 255.f, global_gamma);
    return (vec4)pixel / 255.f;
}

static u8vec4 f32tou8(vec4 pixel)
{
    return (u8vec4)(pixel * 255.99f);
}

/*
 * Pixel-level image manipulation
 */

PixelFormat Image::GetFormat() const
{
    return m_data->m_format;
}

void Image::SetFormat(PixelFormat fmt)
{
    PixelFormat old_fmt = m_data->m_format;

    /* Preliminary intermediate conversions */
    if (old_fmt == PixelFormat::RGBA_8 && fmt == PixelFormat::Y_F32)
        SetFormat(PixelFormat::RGBA_F32);
    else if (old_fmt == PixelFormat::RGB_8 && fmt == PixelFormat::Y_F32)
        SetFormat(PixelFormat::RGBA_F32);
    else if (old_fmt == PixelFormat::Y_F32 && fmt == PixelFormat::RGBA_8)
        SetFormat(PixelFormat::RGBA_F32);
    else if (old_fmt == PixelFormat::Y_F32 && fmt == PixelFormat::RGB_8)
        SetFormat(PixelFormat::RGBA_F32);

    old_fmt = m_data->m_format;

    /* Set the new active pixel format */
    m_data->m_format = fmt;

    ivec2 size = GetSize();
    int count = size.x * size.y;

    /* If we never used this format, allocate a new buffer: we will
     * obviously need it. */
    if (m_data->m_pixels[(int)fmt] == nullptr)
    {
        m_data->m_pixels[(int)fmt] = new uint8_t[count * BytesPerPixel(fmt)];
    }

    /* If the requested format is already the current format, or if the
     * current format is invalid, there is nothing to convert. */
    if (fmt == old_fmt || old_fmt == PixelFormat::Unknown)
        return;

    /* Convert pixels */
    if (old_fmt == PixelFormat::RGBA_8 && fmt == PixelFormat::RGBA_F32)
    {
        u8vec4 *src = (u8vec4 *)m_data->m_pixels[(int)old_fmt];
        vec4 *dest = (vec4 *)m_data->m_pixels[(int)fmt];

        for (int n = 0; n < count; ++n)
            dest[n] = u8tof32(src[n]);
    }
    else if (old_fmt == PixelFormat::RGB_8 && fmt == PixelFormat::RGBA_F32)
    {
        u8vec3 *src = (u8vec3 *)m_data->m_pixels[(int)old_fmt];
        vec4 *dest = (vec4 *)m_data->m_pixels[(int)fmt];

        for (int n = 0; n < count; ++n)
            dest[n] = u8tof32(u8vec4(src[n], 255));
    }
    else if (old_fmt == PixelFormat::RGBA_F32 && fmt == PixelFormat::RGBA_8)
    {
        vec4 *src = (vec4 *)m_data->m_pixels[(int)old_fmt];
        u8vec4 *dest = (u8vec4 *)m_data->m_pixels[(int)fmt];

        for (int n = 0; n < count; ++n)
            dest[n] = f32tou8(src[n]);
#if 0
        init_tables();

        for (int y = 0; y < size.y; y++)
            for (int x = 0; x < size.x; x++)
                for (i = 0; i < 4; i++)
                {
                    double p, e;
                    uint8_t d;

                    p = src[4 * (y * size.x + x) + i];

                    if (p < 0.) d = 0.;
                    else if (p > 1.) d = 255;
                    else d = (int)(255.999 * pow(p, 1. / global_gamma));

                    dest[4 * (y * size.x + x) + i] = d;

                    e = (p - u8tof32(d)) / 16;
                    if (x < size.x - 1)
                        src[4 * (y * size.x + x + 1) + i] += e * 7;
                    if (y < size.y - 1)
                    {
                        if (x > 0)
                            src[4 * ((y + 1) * size.x + x - 1) + i] += e * 3;
                        src[4 * ((y + 1) * size.x + x) + i] += e * 5;
                        if (x < size.x - 1)
                            src[4 * ((y + 1) * size.x + x + 1) + i] += e;
                    }
                }
#endif
    }
    else if (old_fmt == PixelFormat::Y_F32 && fmt == PixelFormat::RGBA_F32)
    {
        float *src = (float *)m_data->m_pixels[(int)old_fmt];
        vec4 *dest = (vec4 *)m_data->m_pixels[(int)fmt];

        for (int n = 0; n < count; ++n)
            dest[n] = vec4(vec3(src[n]), 1.0f);
    }
    else if (old_fmt == PixelFormat::RGBA_F32 && fmt == PixelFormat::Y_F32)
    {
        vec4 *src = (vec4 *)m_data->m_pixels[(int)old_fmt];
        float *dest = (float *)m_data->m_pixels[(int)fmt];

        vec3 const coeff(0.299f, 0.587f, 0.114f);

        for (int n = 0; n < count; ++n)
            dest[n] = dot(coeff, src[n].rgb);
    }
    else
    {
        ASSERT(false, "Unable to find proper conversion");
    }
}

} /* namespace lol */
