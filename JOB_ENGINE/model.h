#ifndef MODEL_H
#define MODEL_H

#include<glad/glad.h>
#include<vector>
#include "mesh.h"

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>


class Model
{

public:
	Model();
	~Model();

	void loadModel(std::string path);
	void Draw();


private:
	std::vector<Mesh>meshes;
	std::string directory;
	
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);










};










#endif // !MODEL_H
