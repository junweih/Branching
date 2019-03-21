#define _USE_MATH_DEFINES
#include "warpfunctions.h"
#include <math.h>
#include "utils.h"
glm::vec3 WarpFunctions::squareToDiskUniform(const glm::vec2 &sample)
{
    //TODO
    float r = glm::sqrt(sample.x);
    float theta = 2 * M_PI * sample.y;
    return glm::vec3(r * glm::cos(theta), r * glm::sin(theta), 0);
//    throw std::runtime_error("You haven't yet implemented uniform disk warping!");
}

glm::vec3 WarpFunctions::squareToDiskConcentric(const glm::vec2 &sample)
{
    //TODO
    float phi, r, u, v;
    float a = 2 * sample.x - 1;
    float b = 2 * sample.y - 1;

    if (a > -b) {
        if (a > b) {
            r = a;
            phi = (M_PI / 4) * (b / a);
        }
        else {
            r = b;
            phi = (M_PI / 4) * (2 - (a / b));
        }
    }
    else {
        if (a < b) {
            r = -a;
            phi = (M_PI / 4) * (4 + (b / a));
        }
        else {
            r = -b;
            if (b != 0) {
                phi = (M_PI / 4) * (6 - (a / b));
            }
            else {
                phi = 0;
            }
        }
    }
    u = r * cos(phi);
    v = r * sin(phi);
    return glm::vec3(u, v, 0);


//    throw std::runtime_error("You haven't yet implemented concentric disk warping!");
}

float WarpFunctions::squareToDiskPDF(const glm::vec3 &sample)
{
    //TODO
    return 1 / M_PI;
}

glm::vec3 WarpFunctions::squareToSphereUniform(const glm::vec2 &sample)
{
    //TODO
    float z = 1 - 2 * sample.x;
    float x = cos(2.f * M_PI * sample.y) * glm::sqrt(1 - z*z);
    float y = sin(2.f * M_PI * sample.y) * glm::sqrt(1 - z*z);
    return glm::vec3(x, y, z);
//    throw std::runtime_error("You haven't yet implemented uniform sphere warping!");
}

float WarpFunctions::squareToSphereUniformPDF(const glm::vec3 &sample)
{
    //TODO
    return 1 / (4*M_PI);
}

glm::vec3 WarpFunctions::squareToSphereCapUniform(const glm::vec2 &sample, float thetaMin)
{
    //TODO
    float z = 1 - sample.x * sin(glm::radians(thetaMin));
    float x = cos(2.f * M_PI * sample.y) * glm::sqrt(1 - z*z);
    float y = sin(2.f * M_PI * sample.y) * glm::sqrt(1 - z*z);
    return glm::vec3(x, y, z);
//    throw std::runtime_error("You haven't yet implemented sphere cap warping!");
}

float WarpFunctions::squareToSphereCapUniformPDF(const glm::vec3 &sample, float thetaMin)
{
    //TODO
    return 1.f / (2.f * M_PI * (1 - cos(glm::radians(180 - thetaMin))));
}

glm::vec3 WarpFunctions::squareToHemisphereUniform(const glm::vec2 &sample)
{
    //TODO
    float z = sample.x;
    float x = cos(2.f * M_PI * sample.y) * glm::sqrt(1 - z*z);
    float y = sin(2.f * M_PI * sample.y) * glm::sqrt(1 - z*z);
    return glm::vec3(x, y, z);
//    throw std::runtime_error("You haven't yet implemented uniform hemisphere warping!");
}

float WarpFunctions::squareToHemisphereUniformPDF(const glm::vec3 &sample)
{
    //TODO
    return 1 / (2.f * M_PI);
}

glm::vec3 WarpFunctions::squareToHemisphereCosine(const glm::vec2 &sample)
{
    //TODO
    float phi, r, u, v;
    float a = 2 * sample.x - 1;
    float b = 2 * sample.y - 1;

    if (a > -b) {
        if (a > b) {
            r = a;
            phi = (M_PI / 4) * (b / a);
        }
        else {
            r = b;
            phi = (M_PI / 4) * (2 - (a / b));
        }
    }
    else {
        if (a < b) {
            r = -a;
            phi = (M_PI / 4) * (4 + (b / a));
        }
        else {
            r = -b;
            if (b != 0) {
                phi = (M_PI / 4) * (6 - (a / b));
            }
            else {
                phi = 0;
            }
        }
    }
    u = r * cos(phi);
    v = r * sin(phi);
    float z = sqrt( 1 - u*u - v*v);
    return glm::vec3(u, v, z);

//    throw std::runtime_error("You haven't yet implemented cosine-weighted hemisphere warping!");
}

float WarpFunctions::squareToHemisphereCosinePDF(const glm::vec3 &sample)
{
    //TODO
    return sample.z / ( M_PI);
}
