#pragma once

#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Texture.h"

class Model
{
public:
	Model();

	void LoadModel(const std::string& fileName);
	void RenderModel();
	void ClearModel();

	~Model();

private:
	std::vector<Mesh*> _meshList;
	std::vector<Texture*> _textureList;
	std::vector<unsigned int> _meshToTexture;

	void LoadNode(aiNode* node, const aiScene* scene);
	void LoadMesh(aiMesh* mesh);
	void LoadMaterial(const aiScene* scene);
};

