#include "Model.h"

Model::Model()
{
}

void Model::ClearModel()
{
	for (size_t i = 0; i < _meshList.size(); i++)
	{
		if (_meshList[i])
		{
			delete _meshList[i];
			_meshList[i] = nullptr;
		}
	}
	for (size_t i = 0; i < _textureList.size(); i++)
	{
		if (_textureList[i])
		{
			delete _textureList[i];
			_textureList[i] = nullptr;
		}
	}
}

void Model::RenderModel()
{
	for (size_t i = 0; i < _meshList.size(); i++)
	{
		unsigned int materialIndex = _meshToTexture[i];

		if (materialIndex < _textureList.size() && _textureList[materialIndex])
		{
			_textureList[materialIndex]->UseTexture();
		}

		_meshList[i]->RenderMesh();
	}
}

void Model::LoadModel(const std::string& fileName)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate
													 | aiProcess_FlipUVs
													 | aiProcess_GenSmoothNormals
													 | aiProcess_JoinIdenticalVertices);
	if (!scene)
	{
		printf((const char*)("ERROR: Model %s failed to load: %s", fileName, importer.GetErrorString()));
		return;
	}

	LoadNode(scene->mRootNode, scene);
	LoadMaterial(scene);
}

void Model::LoadNode(aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		LoadMesh(scene->mMeshes[node->mMeshes[i]]);
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene);
	}
}

void Model::LoadMesh(aiMesh* mesh)
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
		if (mesh->mTextureCoords[0])
		{
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		}
		else
		{
			vertices.insert(vertices.end(), { 0.0f, 0.0f });
		}
		vertices.insert(vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z });
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh* newMesh = new	Mesh();
	newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	_meshList.push_back(newMesh);
	_meshToTexture.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterial(const aiScene* scene)
{
	_textureList.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];

		_textureList[i] = nullptr;
		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				int index = std::string(path.data).rfind("\\");
				std::string fileName = std::string(path.data).substr(index + 1);

				std::string texturePath = std::string("Textures/") + fileName;

				_textureList[i] = new Texture(texturePath.c_str());

				if (_textureList[i] != nullptr && !_textureList[i]->LoadTexture())
				{
					printf("ERROR: failed to load texture at %s\n", texturePath.c_str());
					delete _textureList[i];
					_textureList[i] = nullptr;
				}
			}
		}

		if (_textureList[i] == nullptr)
		{
			_textureList[i] = new Texture("Textures/white_square.png");
			_textureList[i]->LoadTexture();
		}
	}
}

Model::~Model()
{
}