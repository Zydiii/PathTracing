#pragma once
#include <fstream>
#include <string>
#include <stdexcept> 
#include "Option.h"
#include "../math/MathHelper.h"
#include "../math/Vec.h"

class FileHelper {
public:
	static void writeImageFile(Vec3d c[], Option option) {
        std::string filePath = "./result/TestSceneResult_" + std::to_string(option.samps * 4) + ".ppm";
        std::ofstream file;
        file.open(filePath);
        if (file.fail())
        {
            std::string error = "Unable to open " + filePath;
            throw std::invalid_argument(error);
        }
        file << "P3\n" << option.width << "\n" << option.height << "\n255\n";
        for (int i = 0; i < option.width * option.height; i++)
            file << toInt(c[i].x) << " " << toInt(c[i].y) << " " << toInt(c[i].z) << " ";
        if (file.eof())   
            file.close();
        else
        {
            std::string error = "Unable to readfile " + filePath;
            throw std::runtime_error(error);
        }
	}
};
