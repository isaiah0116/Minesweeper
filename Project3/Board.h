#pragma once
#include "SFML/Graphics.hpp"
#include <unordered_map>
#include <string>
using std::unordered_map;
using std::string;

class Board
{
	static unordered_map<string, sf::Texture> textures;
	static void LoadTexture(string filename);
public:
	static sf::Texture& GetTexture(string textureKey);
};

