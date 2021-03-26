#ifndef FSLGRAPHICS3D_BASICSCENE_SCENE_HPP
#define FSLGRAPHICS3D_BASICSCENE_SCENE_HPP
/****************************************************************************************************************************************************
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
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

#include <FslBase/BasicTypes.hpp>
#include <FslGraphics3D/BasicScene/Mesh.hpp>
#include <FslGraphics3D/BasicScene/MeshAllocatorFunc.hpp>
#include <FslGraphics3D/BasicScene/SceneNode.hpp>
#include <memory>

namespace Fsl
{
  namespace Graphics3D
  {
    class Scene
    {
      MeshAllocatorFunc m_meshAllocator;
      std::shared_ptr<SceneNode> m_rootNode;
      float m_scaleFactor;

    public:
      Scene();
      explicit Scene(MeshAllocatorFunc meshAllocator);

      virtual ~Scene() = default;

      //! @brief The amount the scene was scaled during loading (if any)
      float GetScaleFactor() const;

      //! @brief set the scale factor (intended to be set by the loader)
      void SetScaleFactor(const float scaleFactor);

      //! @brief Get the scene root node;
      std::shared_ptr<SceneNode> GetRootNode() const;

      //! @brief Set the root node
      void SetRootNode(const std::shared_ptr<SceneNode>& rootNode);

      //! @brief Get the mesh allocator (warning it might not be set)
      MeshAllocatorFunc GetMeshAllocator() const;

      //! @brief Get the current scene mesh count
      virtual int32_t GetMeshCount() const = 0;

      //! @brief Get the current scene mesh count
      virtual std::shared_ptr<Mesh> GetMeshAt(const int32_t index) const = 0;

      //! @brief Add a mesh to the scene
      virtual void AddMesh(const std::shared_ptr<Mesh>& mesh) = 0;
    };
  }
}

#endif
