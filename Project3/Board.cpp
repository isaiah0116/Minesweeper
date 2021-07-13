#include "Board.h"

unordered_map<string, sf::Texture> Board::textures;

void Board::LoadTexture(string filename) {
	string filePath = "images/";
	filePath += filename;
	filePath += ".png";

	textures[filename].loadFromFile(filePath);
}

sf::Texture& Board::GetTexture(string textureKey) {
	if (textures.find(textureKey) == textures.end()) {
		LoadTexture(textureKey);
	}

	return textures[textureKey];
}
