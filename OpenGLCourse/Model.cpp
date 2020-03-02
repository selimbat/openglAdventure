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
		printf("material %i\n", i + 1);

		aiMaterial* material = scene->mMaterials[i];
		if (material->GetTextureCount(aiTextureType_AMBIENT) > 0)
		{
			printf("	ambiant\n");
		}
		if (material->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION) > 0)
		{
			printf("	ambiant occlusion\n");
		}
		if (material->GetTextureCount(aiTextureType_BASE_COLOR) > 0)
		{
			printf("	base color\n");
		}
		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			printf("	diffuse\n");
		}
		if (material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS) > 0)
		{
			printf("	diffuse roughness\n");
		}
		if (material->GetTextureCount(aiTextureType_DISPLACEMENT) > 0)
		{
			printf("	displacement\n");
		}
		if (material->GetTextureCount(aiTextureType_EMISSION_COLOR) > 0)
		{
			printf("	emission color\n");
		}
		if (material->GetTextureCount(aiTextureType_EMISSIVE) > 0)
		{
			printf("	emissive\n");
		}
		if (material->GetTextureCount(aiTextureType_HEIGHT) > 0)
		{
			printf("	height\n");
		}
		if (material->GetTextureCount(aiTextureType_LIGHTMAP) > 0)
		{
			printf("	light map\n");
		}
		if (material->GetTextureCount(aiTextureType_METALNESS) > 0)
		{
			printf("	metalness\n");
		}
		if (material->GetTextureCount(aiTextureType_NONE) > 0)
		{
			printf("	none\n");
		}
		if (material->GetTextureCount(aiTextureType_NORMALS) > 0)
		{
			printf("	normals\n");
		}
		if (material->GetTextureCount(aiTextureType_NORMAL_CAMERA) > 0)
		{
			printf("	normal camera\n");
		}
		if (material->GetTextureCount(aiTextureType_OPACITY) > 0)
		{
			printf("	opacity\n");
		}
		if (material->GetTextureCount(aiTextureType_REFLECTION) > 0)
		{
			printf("	reflection\n");
		}
		if (material->GetTextureCount(aiTextureType_SHININESS) > 0)
		{
			printf("	shininess\n");
		}
		if (material->GetTextureCount(aiTextureType_SPECULAR) > 0)
		{
			printf("	specular\n");
		}
		if (material->GetTextureCount(aiTextureType_UNKNOWN) > 0)
		{
			printf("	unknown\n");
		}
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