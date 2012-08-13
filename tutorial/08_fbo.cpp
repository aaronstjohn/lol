//
// Lol Engine - Framebuffer Object tutorial
//
// Copyright: (c) 2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"
#include "loldebug.h"
#include "lolgl.h"

using namespace std;
using namespace lol;

#if defined _WIN32
#   include <direct.h>
#endif

extern char const *lolfx_08_fbo;

class FBO : public WorldEntity
{
public:
    FBO()
    {
        m_vertices << vec2( 1.0,  1.0);
        m_vertices << vec2(-1.0, -1.0);
        m_vertices << vec2( 1.0, -1.0);
        m_vertices << vec2(-1.0, -1.0);
        m_vertices << vec2( 1.0,  1.0);
        m_vertices << vec2(-1.0,  1.0);
        m_ready = false;
    }

    virtual void TickGame(float seconds)
    {
        WorldEntity::TickGame(seconds);

        m_time += seconds;
        m_hotspot = 0.4f * vec3(lol::cos(m_time * 2.f) + lol::cos(m_time * 3.3f),
                                lol::sin(m_time * 4.4f) + lol::sin(m_time * 3.2f),
                                lol::sin(m_time * 5.f));
        m_color = 0.25f * vec3(3.f + lol::sin(m_time * 4.5f + 1.f),
                               3.f + lol::sin(m_time * 2.8f + 1.3f),
                               3.f + lol::sin(m_time * 3.7f));
    }

    virtual void TickDraw(float seconds)
    {
        WorldEntity::TickDraw(seconds);

        if (!m_ready)
        {
            m_shader = Shader::Create(lolfx_08_fbo);
            m_coord = m_shader->GetAttribLocation("in_Position", VertexUsage::Position, 0);
            m_uni_point = m_shader->GetUniformLocation("in_Point");
            m_uni_color = m_shader->GetUniformLocation("in_Color");
            m_uni_flag = m_shader->GetUniformLocation("in_Flag");
            m_uni_texture = m_shader->GetUniformLocation("in_Texture");

            m_vdecl = new VertexDeclaration(VertexStream<vec2>(VertexUsage::Position));

            m_vbo = new VertexBuffer(m_vertices.Bytes());
            void *vertices = m_vbo->Lock(0, 0);
            memcpy(vertices, &m_vertices[0], m_vertices.Bytes());
            m_vbo->Unlock();

            m_fbo = new FrameBuffer(Video::GetSize());
            m_fbo->Bind();
            glClearColor(0.0, 0.0, 0.0, 1.0f);
            glClearDepth(1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            m_fbo->Unbind();

            m_ready = true;

            /* FIXME: this object never cleans up */
        }

        m_fbo->Bind();
        /* FIXME: we should just disable depth test in the shader */
        glClear(GL_DEPTH_BUFFER_BIT);
        m_shader->Bind();
        m_shader->SetUniform(m_uni_flag, 0.f);
        m_shader->SetUniform(m_uni_point, m_hotspot);
        m_shader->SetUniform(m_uni_color, m_color);
        m_vdecl->SetStream(m_vbo, m_coord);
        m_vdecl->Bind();
        m_vdecl->DrawElements(MeshPrimitive::Triangles, 0, 2);
        m_vdecl->Unbind();
        m_shader->Unbind();
        m_fbo->Unbind();

        m_shader->Bind();
        m_shader->SetUniform(m_uni_flag, 1.f);
        m_shader->SetTexture(m_uni_texture, m_fbo->GetTexture(), 0);
        m_vdecl->SetStream(m_vbo, m_coord);
        m_vdecl->Bind();
        m_vdecl->DrawElements(MeshPrimitive::Triangles, 0, 2);
        m_vdecl->Unbind();
        m_shader->Unbind();
    }

private:
    Array<vec2> m_vertices;
    Shader *m_shader;
    ShaderAttrib m_coord;
    ShaderUniform m_uni_flag, m_uni_point, m_uni_color, m_uni_texture;
    VertexDeclaration *m_vdecl;
    VertexBuffer *m_vbo;
    FrameBuffer *m_fbo;
    double m_time;
    vec3 m_hotspot, m_color;
    bool m_ready;
};

int main(int argc, char **argv)
{
    Application app("Tutorial 08: Framebuffer Object", ivec2(640, 480), 60.0f);

#if defined _MSC_VER && !defined _XBOX
    _chdir("..");
#elif defined _WIN32 && !defined _XBOX
    _chdir("../..");
#endif

    new FBO();

    app.Run();
    return EXIT_SUCCESS;
}

