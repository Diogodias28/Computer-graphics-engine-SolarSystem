#include "box.hpp"

void genBox(int length, int grid, std::string fileName) {

    std::vector<Point> controlPoints;
    std::vector<Point> normals;
    std::vector<Point> texCoords;
    float part = (float)length / grid; 
    float half = length / 2.0f;

    for (int i = 0; i < grid; i++) {
        for (int j = 0; j < grid; j++) {
            float a = -half + i * part;
            float b = -half + j * part;
            float ta = (a + half) / length;
            float tb = (b + half) / length;
            float tap = (a + part + half) / length;
            float tbp = (b + part + half) / length;

            // Face Frontal
            controlPoints.push_back(makePoint(a + part, b + part, half));
            controlPoints.push_back(makePoint(a, b + part, half));
            controlPoints.push_back(makePoint(a, b, half));

            controlPoints.push_back(makePoint(a + part, b + part, half));
            controlPoints.push_back(makePoint(a, b, half));
            controlPoints.push_back(makePoint(a + part, b, half));

            for(int k = 0; k < 6; k++) {
                normals.push_back(makePoint(0, 0, 1));
            }

            texCoords.push_back(makeTexturePoint(tap, tbp));
            texCoords.push_back(makeTexturePoint(ta, tbp));
            texCoords.push_back(makeTexturePoint(ta, tb));

            texCoords.push_back(makeTexturePoint(tap, tbp));
            texCoords.push_back(makeTexturePoint(ta, tb));
            texCoords.push_back(makeTexturePoint(tap, tb));


			// Face de trás
			controlPoints.push_back(makePoint(a, b, -half));
			controlPoints.push_back(makePoint(a, b + part, -half));
			controlPoints.push_back(makePoint(a + part, b, -half));
	  
			controlPoints.push_back(makePoint(a, b + part, -half));
			controlPoints.push_back(makePoint(a + part, b + part, -half));
			controlPoints.push_back(makePoint(a + part, b, -half));
            
            for (int k = 0; k < 6; k++) {
                normals.push_back(makePoint(0, 0, -1));
            }

            texCoords.push_back(makeTexturePoint(ta, tb));
            texCoords.push_back(makeTexturePoint(ta, tbp));
            texCoords.push_back(makeTexturePoint(tap, tb));

            texCoords.push_back(makeTexturePoint(ta, tbp));
            texCoords.push_back(makeTexturePoint(tap, tbp));
            texCoords.push_back(makeTexturePoint(tap, tb));


			// Face da Esquerda
			controlPoints.push_back(makePoint(-half, a, b));
			controlPoints.push_back(makePoint(-half, a, b + part));
			controlPoints.push_back(makePoint(-half, a + part, b));
	  
			controlPoints.push_back(makePoint(-half, a, b + part));
			controlPoints.push_back(makePoint(-half, a + part, b + part));
			controlPoints.push_back(makePoint(-half, a + part, b));
            
            for (int k=0; k < 6; k++) {
                normals.push_back(makePoint(-1, 0, 0));
            }

            texCoords.push_back(makeTexturePoint(ta, tb));
            texCoords.push_back(makeTexturePoint(ta, tbp));
            texCoords.push_back(makeTexturePoint(tap, tb));

            texCoords.push_back(makeTexturePoint(ta, tbp));
            texCoords.push_back(makeTexturePoint(tap, tbp));
            texCoords.push_back(makeTexturePoint(tap, tb));


			// Face da direita
			controlPoints.push_back(makePoint(half, a + part, b + part));
			controlPoints.push_back(makePoint(half, a, b + part));
			controlPoints.push_back(makePoint(half, a, b));
	  
			controlPoints.push_back(makePoint(half, a + part, b + part));
			controlPoints.push_back(makePoint(half, a, b));
			controlPoints.push_back(makePoint(half, a + part, b));

            for (int k = 0; k < 6; k++) {
                normals.push_back(makePoint(-1, 0, 0));
            }

            texCoords.push_back(makeTexturePoint(tap, tbp));
            texCoords.push_back(makeTexturePoint(ta, tbp));
            texCoords.push_back(makeTexturePoint(ta, tb));

            texCoords.push_back(makeTexturePoint(tap, tbp));
            texCoords.push_back(makeTexturePoint(ta, tb));
            texCoords.push_back(makeTexturePoint(tap, tb));


			// Face Superior
			controlPoints.push_back(makePoint(a, half, b));
			controlPoints.push_back(makePoint(a + part, half, b + part));
			controlPoints.push_back(makePoint(a + part, half, b));
	  
			controlPoints.push_back(makePoint(a, half, b));
			controlPoints.push_back(makePoint(a, half, b + part));
			controlPoints.push_back(makePoint(a + part, half, b + part));
            
            for (int k = 0; k < 6; k++) {
                normals.push_back(makePoint(0, 1, 0));
            }

            texCoords.push_back(makeTexturePoint(ta, tb));
            texCoords.push_back(makeTexturePoint(tap, tbp));
            texCoords.push_back(makeTexturePoint(tap, tb));

            texCoords.push_back(makeTexturePoint(ta, tb));
            texCoords.push_back(makeTexturePoint(ta, tbp));
            texCoords.push_back(makeTexturePoint(tap, tbp));


			// Bottom Face
			controlPoints.push_back(makePoint(a, -half, b));
			controlPoints.push_back(makePoint(a + part, -half, b));
			controlPoints.push_back(makePoint(a, -half, b + part));
	  
			controlPoints.push_back(makePoint(a, -half, b + part));
			controlPoints.push_back(makePoint(a + part, -half, b));
			controlPoints.push_back(makePoint(a + part, -half, b + part));
			
            for (int k = 0; k < 6; k++) {
                normals.push_back(makePoint(0, -1, 0));
            }

            texCoords.push_back(makeTexturePoint(ta, tb));
            texCoords.push_back(makeTexturePoint(tap, tb));
            texCoords.push_back(makeTexturePoint(ta, tbp));

            texCoords.push_back(makeTexturePoint(ta, tbp));
            texCoords.push_back(makeTexturePoint(tap, tb));
            texCoords.push_back(makeTexturePoint(tap, tbp));
        }
    }
    writeToFile(controlPoints, normals, texCoords, fileName);}
