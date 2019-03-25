/****************************************************************************************************************************************************
 * Copyright (c) 2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *    * Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *    * Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *
 *    * Neither the name of the Freescale Semiconductor, Inc. nor the names of
 *      its contributors may be used to endorse or promote products derived from
 *      this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************************************************************************************/

#include <FslGraphics/Vertices/VertexPosition.hpp>
#include <FslGraphics3D/Procedural/BasicMesh.hpp>
#include <FslGraphics3D/Procedural/MeshBuilder.hpp>
#include <gtest/gtest.h>


namespace
{
  using namespace Fsl;
  using namespace Fsl::Graphics3D;
  using namespace Fsl::Procedural;
  using SimpleMeshBuilder = MeshBuilder<VertexPosition, uint16_t>;
  using SimpleMesh = GenericMesh<VertexPosition, uint16_t>;

  VertexPosition g_vertices[] = {VertexPosition(-1.0f, -1.0f, 0.0f), VertexPosition(-1.0f, 1.0f, 0.0f), VertexPosition(1.0f, 1.0f, 0.0f),
                                 VertexPosition(1.0f, 1.0f, 0.0f)};

  uint16_t g_indices[] = {0, 1, 2, 2, 1, 3};

  const size_t g_vertexCount = sizeof(g_vertices) / sizeof(g_vertices[0]);
  const size_t g_indexCount = sizeof(g_indices) / sizeof(g_indices[0]);
  const PrimitiveType g_primitiveType = PrimitiveType::TriangleList;
  const PrimitiveType g_otherPrimitiveType = PrimitiveType::TriangleStrip;
}

// EXPECT_EQ(1, res);


TEST(MeshBuilder, AppendListToIncompatible)
{
  SimpleMesh mesh(g_vertices, g_vertexCount, g_indices, g_indexCount, g_primitiveType);

  EXPECT_EQ(2u, mesh.GetPrimitiveCount());

  SimpleMeshBuilder builder(g_otherPrimitiveType);
  EXPECT_THROW(builder.Append(mesh), std::invalid_argument);
}


TEST(MeshBuilder, AppendTriangleList)
{
  SimpleMesh mesh(g_vertices, g_vertexCount, g_indices, g_indexCount, g_primitiveType);

  EXPECT_EQ(2u, mesh.GetPrimitiveCount());

  SimpleMeshBuilder builder(g_primitiveType);

  builder.Append(mesh);

  EXPECT_EQ(mesh.GetIndexCount(), builder.GetIndexCount());
  EXPECT_EQ(mesh.GetVertexCount(), builder.GetVertexCount());

  builder.Append(mesh);
  EXPECT_EQ(mesh.GetIndexCount() * 2, builder.GetIndexCount());
  EXPECT_EQ(mesh.GetVertexCount() * 2, builder.GetVertexCount());
}

namespace
{
  void AppendInstances(const int instances, const bool shareInstanceVertices)
  {
    SimpleMesh mesh(g_vertices, g_vertexCount, g_indices, g_indexCount, g_primitiveType);

    EXPECT_EQ(2u, mesh.GetPrimitiveCount());

    SimpleMeshBuilder builder(g_primitiveType);
    builder.AppendInstances(mesh, instances, shareInstanceVertices);

    EXPECT_EQ(mesh.GetIndexCount() * instances, builder.GetIndexCount());
    EXPECT_EQ(mesh.GetVertexCount() * (!shareInstanceVertices ? instances : 1), builder.GetVertexCount());
  }
}


TEST(MeshBuilder, AppendInstances1TriangleListVertexSharingOff)
{
  AppendInstances(1, false);
}


TEST(MeshBuilder, AppendInstances1TriangleListVertexSharingOn)
{
  AppendInstances(1, true);
}


TEST(MeshBuilder, AppendInstances2TriangleListVertexSharingOff)
{
  AppendInstances(2, false);
}


TEST(MeshBuilder, AppendInstances2TriangleListVertexSharingOn)
{
  AppendInstances(1, true);
}


TEST(MeshBuilder, AppendInstances999TriangleListVertexSharingOff)
{
  AppendInstances(999, false);
}


TEST(MeshBuilder, AppendInstances999TriangleListVertexSharingOn)
{
  AppendInstances(999, true);
}
